# This Ghidra script scans the Black Mesa Linux `server_srv.so` binary and does the following:
#
# - Writes vftables, datamaps and netprops to all corresponding classes
#   - You will need to dump the datamap/netprops to disk as a XML file via `sm_dump_datamaps_xml` and `sm_dump_netprops_xml`.
# - Writes to `gpGlobals` and `g_pGameRules` with their respective structures
# - Parses the binary for the list of activities.

from ghidra.program.model.data import (
    StructureDataType,
    CategoryPath,
    DataTypeConflictHandler,
    PointerDataType,
    BooleanDataType,
    FloatDataType,
    ArrayDataType,
    CharDataType,
    ShortDataType,
    IntegerDataType,
    UnsignedCharDataType,
    UnsignedShortDataType,
    UnsignedIntegerDataType,
    EnumDataType,
)
from ghidra.app.decompiler import (
    DecompInterface,
    DecompileOptions,
)
import ghidra.program.model.symbol.SourceType as SourceType
import ghidra.program.model.pcode.PcodeOp as PcodeOp
import docking.widgets.filechooser.GhidraFileChooser as GhidraFileChooser
import xml.etree.ElementTree as ET
import re



def parse_xml_file(path):
    """Read SourceMod XML dumps robustly under Ghidra/Jython.

    Some dumps are saved with UTF BOM/UTF-16 or contain console/log text
    before the XML document. xml.etree.ElementTree.parse() in Jython often
    fails on those files with: "Content is not allowed in prolog".
    """
    with open(path, "rb") as f:
        raw = f.read()

    # Decode common encodings/BOMs. Jython 2.x treats bytes as str; b'' works.
    if raw.startswith(b"\xef\xbb\xbf"):
        text = raw[3:].decode("utf-8", "replace")
    elif raw.startswith(b"\xff\xfe"):
        text = raw[2:].decode("utf-16-le", "replace")
    elif raw.startswith(b"\xfe\xff"):
        text = raw[2:].decode("utf-16-be", "replace")
    elif b"\x00" in raw[:200]:
        # A lot of Windows redirects/save dialogs produce UTF-16 without a
        # useful XML declaration.
        try:
            text = raw.decode("utf-16", "replace")
        except:
            text = raw.decode("utf-8", "replace")
    else:
        try:
            text = raw.decode("utf-8")
        except:
            text = raw.decode("cp1251", "replace")

    text = text.replace(u"\x00", u"").lstrip(u"\ufeff\r\n\t ")

    # If the file contains Sourcemod console/log output before XML, skip it.
    start = text.find("<?xml")
    if start < 0:
        start = text.find("<")
    if start < 0:
        raise BaseException("File `{}` does not look like XML. First bytes: {!r}".format(path, raw[:80]))
    if start > 0:
        print("Warning: skipped {} leading character(s) before XML in `{}`.".format(start, path))
        text = text[start:]

    # We re-encode as UTF-8 for ElementTree, so keep/force declaration in sync.
    text = re.sub(r'(<\?xml[^>]*encoding\s*=\s*)(["\']).*?\2',
                  r'\1\2utf-8\2', text, count=1, flags=re.I)

    try:
        return ET.fromstring(text.encode("utf-8"))
    except Exception as e:
        raise BaseException("Could not parse XML file `{}`: {}. First XML characters: {!r}".format(path, e, text[:120]))



def sanitize_identifier(name, fallback):
    if name is None:
        name = fallback
    name = str(name)
    name = re.sub(r"[^0-9A-Za-z_]", "_", name)
    name = re.sub(r"_+", "_", name).strip("_")
    if len(name) == 0:
        name = fallback
    if name[0].isdigit():
        name = fallback + "_" + name
    return name


def make_unique_name(name, existing_names):
    if name not in existing_names:
        return name
    index = 1
    base = name
    while True:
        candidate = base + "_" + str(index)
        if candidate not in existing_names:
            return candidate
        index += 1


def get_function_display_name(fn):
    try:
        return fn.getSymbol().getName(True)
    except:
        try:
            return fn.getName(True)
        except:
            return fn.getName()


def get_function_short_name(fn):
    name = get_function_display_name(fn)
    if "(" in name:
        name = name[:name.find("(")]
    if "::" in name:
        name = name.split("::")[-1]
    if "." in name:
        name = name.split(".")[-1]
    return name


def get_pcode_input_address(pcode, index):
    try:
        varnode = pcode.getInput(index)
    except:
        return None

    # Direct address input.
    try:
        address = varnode.getAddress()
        if address is not None and address.isMemoryAddress():
            return address
    except:
        pass

    # Constant input that is actually an address.
    try:
        if varnode.isConstant():
            return toAddr(varnode.getOffset())
    except:
        pass

    # Common decompiler pattern: pointer value is produced by another pcode op.
    try:
        definition = varnode.getDef()
        if definition is not None:
            for i in range(definition.getNumInputs()):
                candidate = definition.getInput(i)
                try:
                    address = candidate.getAddress()
                    if address is not None and address.isMemoryAddress():
                        return address
                except:
                    pass
                try:
                    if candidate.isConstant():
                        return toAddr(candidate.getOffset())
                except:
                    pass
    except:
        pass

    # Original script fallback.
    try:
        return toAddr(varnode.getDef().getInput(0).getAddress().getUnsignedOffset())
    except:
        return None


def read_c_string_at(address, max_length=256):
    if address is None:
        return None
    chars = []
    for index in range(max_length):
        try:
            value = getByte(address.add(index))
        except:
            return None
        if value < 0:
            value += 256
        if value == 0:
            break
        # Activity names are ASCII identifiers. Reject obvious non-strings.
        if value < 0x20 or value > 0x7e:
            return None
        chars.append(chr(value))
    else:
        return None
    if len(chars) == 0:
        return None
    return "".join(chars)


def get_or_create_c_string(address):
    data = getDataAt(address)
    if data is not None:
        try:
            representation = data.getDefaultValueRepresentation()
            if representation.startswith('"') and representation.endswith('"'):
                value = representation.strip('"')
                if len(value) > 0:
                    return value
        except:
            pass

    value = read_c_string_at(address)
    if value is None:
        return None

    # Define the string in Ghidra so repeated runs and xrefs become cleaner.
    try:
        clearListing(address, address.add(len(value)))
    except:
        try:
            removeDataAt(address)
        except:
            pass
    try:
        createAsciiString(address)
    except:
        try:
            createAsciiString(address, len(value) + 1)
        except:
            pass
    return value


def add_struct_field(datatype, field_type, field_name, comment):
    component = datatype.add(field_type)
    component.setFieldName(field_name)
    if comment is not None and len(comment) > 0:
        try:
            component.setComment(comment)
        except:
            pass
    return component


def add_alignment_padding(datatype, alignment, field_name):
    length = datatype.getLength()
    padding = (alignment - (length % alignment)) % alignment
    if padding > 0:
        add_struct_field(datatype, ArrayDataType(CharDataType(), padding, 1), field_name, "Compiler alignment padding.")

dtm = currentProgram.getDataTypeManager()
sm = currentProgram.getSymbolTable()

class StructVftable:
    def __init__(self, symbol):
        self.name = symbol.getParentSymbol().getName()
        self.vftable = []

        address = symbol.getAddress().add(8)
        while True:
            try:
                fn = getFunctionAt(getDataAt(address).getValue())
                name = fn.getName()

                # Avoid naming conflicts by appending.
                count = 0
                for e in self.vftable:
                    if e["name"].startswith(name):
                        count += 1
                if count > 0:
                    name += "_" + str(count)

                self.vftable.append({
                    "fn": fn,
                    "name": name,
                })
            except:
                break
            address = address.add(4)
        
        if len(self.vftable) == 0:
            raise BaseException("Obtained `0` entries for vftable `{}`.".format(self.name))

class StructVftTableList:
    def __init__(self):
        self.structs = []

        monitor.setIndeterminate(True)
        monitor.setMessage("Parsing vftables...")

        symbols = list(sm.getAllSymbols(False))
        monitor.initialize(len(symbols))

        for symbol in symbols:
            monitor.checkCancelled()
            monitor.incrementProgress(1)

            if symbol.getName() == "vtable":
                try:
                    self.structs.append(StructVftable(symbol))
                except:
                    continue
                struct = self.structs[len(self.structs) - 1]
                monitor.setMessage("Parsed `{}` vftable entries for `{}`...".format(len(struct.vftable), struct.name))

class StructDatamap:
    def __init__(self, serverclass):
        self.name = serverclass.get("element").replace("_", "").lower()
        self.variables = []
        self._parse(serverclass, 0)
        self.variables.sort(key=lambda datamap: datamap["offset"])
    def _parse(self, table, tableoffset):
        for datamap in table.findall("datamap"):
            name = datamap.get("name")
            offset = int(datamap.find("offset").text) 
            size = int(datamap.find("bytes").text)
            
            # Fixes issues with m_vec...[] overlapping with m_vec....
            if name.endswith("]"):
                continue
            
            # Skip entries with a size of 0.
            # These are usually function pointers without a size or offset.
            if size == 0:
                continue
                
            # Skip entries that overlap with the vftable.
            if tableoffset + offset <= 4:
                continue
            
            if any(name == e["name"] for e in self.variables):
                continue
            
            try:
                self.variables.append({
                    "name": name,
                    "offset": tableoffset + offset,
                    "type": StructDatamap._parse_to_ghidra_type(name, size),
                })
            except:
                continue
        
        for subtable in table.findall("subtable"):
            offset = int(subtable.get("offset"))

            # Skip entries that overlap with the vftable.
            if offset <= 4:
                continue
            
            self._parse(subtable, tableoffset + offset)
    @staticmethod
    def _parse_to_ghidra_type(name, size):
        if name.startswith("m_isz") or name.startswith("m_sz") or name.startswith("m_str"):
            return PointerDataType(CharDataType())
        elif name.startswith("m_pfn"):
            return PointerDataType()
        if name.startswith("m_b"):
            return BooleanDataType()
        elif name.startswith("m_fl"):
            return FloatDataType()
        elif name.startswith("m_vec") or name.startswith("m_ang"):
            return ArrayDataType(FloatDataType(), 3, 4)
        elif name.startswith("m_h"):
            return IntegerDataType()
        elif name.startswith("m_i"):
            if size == 1:
                return CharDataType()
            elif size == 2:
                return ShortDataType()
            elif size == 4:
                return IntegerDataType()
        elif name.startswith("m_n") or name.startswith("m_f"):
            if size == 1:
                return UnsignedCharDataType()
            elif size == 2:
                return UnsignedShortDataType()
            elif size == 4:
                return UnsignedIntegerDataType()
        elif name.startswith("m_p"):
            return PointerDataType()
        raise BaseException("Could not parse datamap variable `{}`.".format(name))

class StructDatamapList:
    def __init__(self, path):
        self.structs = []

        monitor.setIndeterminate(True)
        monitor.setMessage("Parsing datamaps...")

        serverclasses = parse_xml_file(path)
        monitor.initialize(len(serverclasses))
        
        for serverclass in serverclasses:
            monitor.checkCancelled()
            monitor.incrementProgress(1)

            self.structs.append(StructDatamap(serverclass))
            struct = self.structs[len(self.structs) - 1]
            monitor.setMessage("Parsed `{}` datamap entries for `{}`...".format(len(struct.variables), struct.name))

class StructNetprop:
    def __init__(self, serverclass):
        self.name = serverclass.get("name")
        self.variables = []
        self._parse(serverclass.find("sendtable"), 0)
        self.variables.sort(key=lambda netprop: netprop["offset"])

    @staticmethod
    def _join_name(prefix, name):
        if prefix is None or len(prefix) == 0:
            return sanitize_identifier(name, "field")
        return sanitize_identifier(str(prefix) + "_" + str(name), "field")

    def _append_variable(self, name, offset, variable_type):
        name = sanitize_identifier(name, "field")
        if any(name == e["name"] for e in self.variables):
            return
        self.variables.append({
            "name": name,
            "offset": offset,
            "type": variable_type,
        })

    def _parse(self, table, tableoffset, prefix=None, table_name=None):
        if table is None:
            return

        for property in table.findall("property"):
            raw_name = property.get("name") or ""
            type = property.find("type").text
            offset = int(property.find("offset").text)
            bits = int(property.find("bits").text)
            flags = property.find("flags").text or ""
            sendtable = property.find("sendtable")

            # Inheritance.
            if raw_name == "baseclass":
                self._parse(sendtable, 0, prefix, table_name)
                continue

            # This is a data table containing other offsets. Keep the original
            # behaviour for normal datatables, but preserve numbered array or
            # subclass entries by prefixing child fields with the parent name.
            if type == "datatable":
                next_prefix = prefix
                next_table_name = raw_name

                if raw_name.isdigit():
                    owner = prefix if prefix is not None else table_name
                    if owner is None:
                        # No useful name can be formed for this anonymous entry.
                        continue
                    next_prefix = StructNetprop._join_name(owner, raw_name)
                    next_table_name = None
                elif prefix is not None:
                    next_prefix = StructNetprop._join_name(prefix, raw_name)
                    next_table_name = None

                self._parse(sendtable, tableoffset + offset, next_prefix, next_table_name)
                continue

            # This is a member variable.
            name = raw_name
            if raw_name.isdigit():
                owner = prefix if prefix is not None else table_name
                if owner is None:
                    continue
                name = StructNetprop._join_name(owner, raw_name)
            elif prefix is not None:
                name = StructNetprop._join_name(prefix, raw_name)

            # Fixes issues with m_vec...[] overlapping with m_vec....
            if name.endswith("]"):
                continue

            # Check whether offset overlaps with vftable or is an invalid offset.
            if tableoffset + offset <= 4:
                continue

            try:
                self._append_variable(name, tableoffset + offset, StructNetprop._parse_to_ghidra_type(name, type, bits, flags))
            except:
                continue

    @staticmethod
    def _parse_to_ghidra_type(name, type, bits, flags):
        if name.startswith("m_fl") or type == "float":
            return FloatDataType()
        elif name.startswith("m_vec") or type == "vector":
            return ArrayDataType(FloatDataType(), 3, 4)
        elif type == "string":
            return PointerDataType(CharDataType())
        elif type == "integer":
            if name.startswith("m_b"):
                return BooleanDataType()
            if bits > 0 and bits <= 32:
                unsigned = flags.find("Unsigned") != -1
                if bits <= 8:
                    return UnsignedCharDataType() if unsigned else CharDataType()
                elif bits <= 16:
                    return UnsignedShortDataType() if unsigned else ShortDataType()
                elif bits <= 32:
                    return UnsignedIntegerDataType() if unsigned else IntegerDataType()
        raise BaseException("Could not determine type `" + type + "` for netprop variable `" + name + "`.")

class StructNetpropList:
    def __init__(self, path):
        self.structs = []

        monitor.setIndeterminate(True)
        monitor.setMessage("Parsing netprops...")

        serverclasses = parse_xml_file(path)
        monitor.initialize(len(serverclasses))

        for index, serverclass in enumerate(serverclasses):
            monitor.checkCancelled()
            monitor.incrementProgress(1)

            self.structs.append(StructNetprop(serverclass))
            struct = self.structs[len(self.structs) - 1]
            monitor.setMessage("Parsed `{}` netprop entries for `{}`...".format(len(struct.variables), struct.name))

def write_variable(datatype, variable):
    end = variable["offset"] + variable["type"].getLength()
    if datatype.getLength() < end:
        datatype.growStructure(end - datatype.getLength())
    datatype.replaceAtOffset(variable["offset"], variable["type"], 0, variable["name"], "")

def write_properties(vftables, datamaps, netprops):
    monitor.setIndeterminate(True)
    monitor.setMessage("Writing vftables, datamaps and netprops...")

    datatypes = list(dtm.getAllDataTypes())
    monitor.initialize(len(datatypes))

    for datatype in datatypes:
        monitor.checkCancelled()
        monitor.incrementProgress(1)

        name = datatype.getName()
        short_name = datatype.getName().lower().replace("_", "")

        if vftables is not None:
            for struct in vftables.structs:
                if struct.name == name:
                    monitor.setMessage("Writing `{}` vftable entries for `{}`...".format(len(struct.vftable), struct.name))

                    vftable_datatype = dtm.addDataType(StructureDataType(datatype.getCategoryPath(), struct.name + "_vftable", 0), DataTypeConflictHandler.REPLACE_HANDLER)
                    for e in struct.vftable:
                        vftable_datatype.add(PointerDataType()).setFieldName(e["name"])
                        e["fn"].setCallingConvention("__thiscall")
                    
                    datatype.deleteAll()
                    write_variable(datatype, {
                        "name": "vftable",
                        "offset": 0,
                        "type": PointerDataType(vftable_datatype),
                    })
                    
                    break
        
        if datamaps is not None:
            for struct in datamaps.structs:
                if struct.name == short_name or struct.name == short_name[1:]:
                    monitor.setMessage("Writing `{}` datamap entries for `{}`...".format(len(struct.variables), struct.name))

                    for variable in struct.variables:
                        write_variable(datatype, variable)
                    
                    break
        
        if netprops is not None:
            for struct in netprops.structs:
                if struct.name == name:
                    monitor.setMessage("Writing `{}` netprop entries for `{}`...".format(len(struct.variables), struct.name))

                    for variable in struct.variables:
                        write_variable(datatype, variable)
                    
                    break

class ActivityList:
    def __init__(self):
        self.variants = []

        monitor.setIndeterminate(True)
        monitor.setMessage("Parsing activities...")
        
        options = DecompileOptions()
        ifc = DecompInterface()
        ifc.setOptions(options)
        ifc.openProgram(currentProgram)
        ifc.setSimplificationStyle("decompile")
        
        for symbol in sm.getAllSymbols(False):
            if symbol.getName() == "ActivityList_RegisterSharedActivities":
                res = ifc.decompileFunction(getFunctionContaining(symbol.getAddress()), 30, monitor)
                pcodes = list(res.getHighFunction().getPcodeOps())
                existing_names = set()

                for pcode in pcodes:
                    if pcode.getOpcode() != PcodeOp.CALL:
                        continue

                    try:
                        target_symbol = getSymbolAt(pcode.getInput(0).getAddress())
                        if target_symbol is None or target_symbol.getName() != "ActivityList_RegisterSharedActivity":
                            continue
                    except:
                        continue

                    name_address = get_pcode_input_address(pcode, 1)
                    name_value = get_or_create_c_string(name_address)
                    if name_value is None:
                        print("Warning: could not read activity name at `{}`.".format(name_address))
                        continue

                    variant_name = sanitize_identifier(name_value, "ACTIVITY")
                    variant_name = make_unique_name(variant_name, existing_names)
                    existing_names.add(variant_name)

                    try:
                        value = pcode.getInput(2).getAddress().getUnsignedOffset()
                    except:
                        try:
                            value = pcode.getInput(2).getOffset()
                        except:
                            print("Warning: could not read activity value for `{}`.".format(variant_name))
                            continue

                    self.variants.append({
                        "name": variant_name,
                        "value": value,
                    })
                return
        
        raise BaseException("Could not find `ActivityList_RegisterSharedActivities`...")

    def write(self):
        monitor.setIndeterminate(True)
        monitor.setMessage("Creating `Activity` enum structure...")

        datatype = EnumDataType(CategoryPath("/Demangler"), "Activity", 4)
        for variant in self.variants:
            datatype.add(variant["name"], variant["value"])
        datatype = dtm.addDataType(datatype, DataTypeConflictHandler.REPLACE_HANDLER)
        
        monitor.setMessage("Setting function return types to `Activity`...")

        symbols = list(sm.getAllSymbols(True))
        monitor.initialize(len(symbols))

        for symbol in symbols:
            monitor.checkCancelled()
            monitor.incrementProgress(1)

            if symbol.getName().startswith("Get") and symbol.getName().endswith("Activity"):
                fn = getFunctionAt(symbol.getAddress())
                if fn is not None:
                    fn.setReturnType(datatype, SourceType.USER_DEFINED)
                    monitor.setMessage("Changed function `{}` return type to `Activity`...".format(symbol.getName(True)))

def write_gp_globals():
    monitor.setIndeterminate(True)
    monitor.setMessage("Retyping `gpGlobals` to `CGlobalVars`...")

    gp_global_datatype = dtm.addDataType(StructureDataType(CategoryPath("/Demangler"), "CGlobalVars", 0), DataTypeConflictHandler.REPLACE_HANDLER)
    add_struct_field(gp_global_datatype, FloatDataType(), "realtime", "Absolute time; use platform time for high precision real time.")
    add_struct_field(gp_global_datatype, IntegerDataType(), "framecount", "Absolute frame counter.")
    add_struct_field(gp_global_datatype, FloatDataType(), "absoluteframetime", "Non-paused frame time.")
    add_struct_field(gp_global_datatype, FloatDataType(), "curtime", "Current simulation time.")
    add_struct_field(gp_global_datatype, FloatDataType(), "frametime", "Time spent on the last server or client frame.")
    add_struct_field(gp_global_datatype, IntegerDataType(), "maxClients", "Current maxplayers setting.")
    add_struct_field(gp_global_datatype, IntegerDataType(), "tickcount", "Simulation tick counter.")
    add_struct_field(gp_global_datatype, FloatDataType(), "interval_per_tick", "Simulation tick interval.")
    add_struct_field(gp_global_datatype, FloatDataType(), "interpolation_amount", "Client interpolation fraction of the next tick.")
    add_struct_field(gp_global_datatype, IntegerDataType(), "simTicksThisFrame", "Number of simulation ticks in this frame.")
    add_struct_field(gp_global_datatype, IntegerDataType(), "network_protocol", "Current network protocol.")
    add_struct_field(gp_global_datatype, PointerDataType(), "pSaveData", "Current CSaveRestoreData pointer.")
    add_struct_field(gp_global_datatype, BooleanDataType(), "m_bClient", "True in client code; false in server code.")
    add_alignment_padding(gp_global_datatype, 4, "_pad_m_bClient")
    add_struct_field(gp_global_datatype, IntegerDataType(), "nTimestampNetworkingBase", "Tick base used when networking simulation timestamps; normally 100.")
    add_struct_field(gp_global_datatype, IntegerDataType(), "nTimestampRandomizeWindow", "Entity-index randomization window for timestamp networking; normally 32.")

    for symbol in sm.getAllSymbols(True):
        if symbol.getName() == "gpGlobals":
            removeDataAt(symbol.getAddress())
            createData(symbol.getAddress(), PointerDataType(gp_global_datatype))
            break

def write_gamerules():
    monitor.setIndeterminate(True)
    monitor.setMessage("Retyping `g_pGameRules` to `CBM_MP_GameRules`...")

    gamerules_datatype = dtm.getDataType("/Demangler/CBM_MP_GameRules")
    for symbol in sm.getAllSymbols(True):
        if symbol.getName() == "g_pGameRules":
            removeDataAt(symbol.getAddress())
            createData(symbol.getAddress(), PointerDataType(gamerules_datatype))
            break



def write_common_function_return_types():
    monitor.setIndeterminate(False)
    monitor.setMessage("Setting bool return types for Is/Can/Will/Has functions...")

    functions = list(currentProgram.getFunctionManager().getFunctions(True))
    monitor.initialize(len(functions))
    boolean_type = BooleanDataType()
    changed = 0

    for fn in functions:
        monitor.checkCancelled()
        monitor.incrementProgress(1)

        name = get_function_short_name(fn)
        if re.match(r"^(Is|Can|Will|Has)[A-Z]", name) is None:
            continue

        try:
            fn.setReturnType(boolean_type, SourceType.USER_DEFINED)
            changed += 1
            monitor.setMessage("Changed function `{}` return type to bool...".format(get_function_display_name(fn)))
        except Exception as e:
            print("Warning: could not set bool return type for `{}`: {}".format(get_function_display_name(fn), e))

    print("Changed return type to bool for {} function(s).".format(changed))


def write_sendprop_function_signatures():
    monitor.setIndeterminate(False)
    monitor.setMessage("Retyping SendProp string argument to char*...")

    target_names = set(["SendPropInt", "SendPropFloat", "SendPropBool", "SendPropVector"])
    char_ptr_type = PointerDataType(CharDataType())

    # Original note asked for the 2nd argument. If Ghidra shows these
    # functions without an implicit/hidden first parameter, change this to 0.
    string_argument_ordinal = 1

    functions = list(currentProgram.getFunctionManager().getFunctions(True))
    monitor.initialize(len(functions))
    changed = 0
    skipped = 0

    for fn in functions:
        monitor.checkCancelled()
        monitor.incrementProgress(1)

        name = get_function_short_name(fn)
        if name not in target_names:
            continue

        try:
            if fn.getParameterCount() <= string_argument_ordinal:
                skipped += 1
                print("Warning: `{}` has only {} parameter(s); could not retype argument #{}.".format(get_function_display_name(fn), fn.getParameterCount(), string_argument_ordinal + 1))
                continue

            parameter = fn.getParameter(string_argument_ordinal)
            parameter.setDataType(char_ptr_type, SourceType.USER_DEFINED)
            try:
                parameter.setName("pVarName", SourceType.USER_DEFINED)
            except:
                pass
            changed += 1
            monitor.setMessage("Retyped `{}` argument #{} to char*...".format(get_function_display_name(fn), string_argument_ordinal + 1))
        except Exception as e:
            skipped += 1
            print("Warning: could not retype SendProp argument for `{}`: {}".format(get_function_display_name(fn), e))

    print("Retyped SendProp string argument for {} function(s); skipped {}.".format(changed, skipped))

chooser = GhidraFileChooser(None)
chooser.setTitle("select xml datamaps file (dumped with `sm_dump_datamaps_xml`)")
selected = chooser.getSelectedFile()
if selected is None:
    raise BaseException("No datamaps XML file selected.")
path_datamap = selected.getPath()
chooser.setTitle("select xml netprops file (dumped with `sm_dump_netprops_xml`)")
selected = chooser.getSelectedFile()
if selected is None:
    raise BaseException("No netprops XML file selected.")
path_netprop = selected.getPath()

write_properties(StructVftTableList(), StructDatamapList(path_datamap), StructNetpropList(path_netprop))
ActivityList().write()
write_gp_globals()
write_gamerules()
write_common_function_return_types()
write_sendprop_function_signatures()