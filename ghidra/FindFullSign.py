# Generates a full contiguous SourceMod-ready signature for the current function.
# Handles non-contiguous Ghidra function bodies by wildcarding gaps between ranges.
# Designed mainly for Windows x86 binaries.
#
#@author nosoop, modified
#@category _NEW_
#@keybinding
#@menupath
#@toolbar

from __future__ import print_function

import collections

import ghidra.program.model.lang.OperandType as OperandType


# ============================================================
# CONFIG
# ============================================================

# False = clean output for normal use.
# True  = verbose debug/dev output.
DEV_MODE = False

# If True, the script still checks uniqueness internally.
# In DEV_MODE = False it will print only errors/warnings, not success info.
CHECK_UNIQUENESS = True

# If True, throws an exception when signature is not unique.
REQUIRE_UNIQUE = False


BytePattern = collections.namedtuple("BytePattern", ["is_wildcard", "byte", "reason"])


def dev_print(*args):
    if DEV_MODE:
        print(*args)


def ida_str(entry):
    if entry.is_wildcard:
        return "?"
    return "{:02X}".format(entry.byte)


def sourcemod_str(entry):
    # SourceMod wildcard is byte 0x2A, usually written as \x2A.
    if entry.is_wildcard:
        return r"\x2A"
    return r"\x{:02X}".format(entry.byte)


def should_mask_operand(ins, op_index):
    """
    Mask unstable operands.

    On Windows x86 this usually catches:
      - CALL/JMP relative targets;
      - absolute addresses;
      - globals/imports;
      - dynamic address references.
    """
    optype = ins.getOperandType(op_index)

    if optype & OperandType.ADDRESS:
        return True

    if optype & OperandType.DYNAMIC:
        return True

    return False


def get_masked_instruction(ins):
    """
    Returns BytePattern entries for one instruction.
    """
    mask = [0] * ins.length
    proto = ins.getPrototype()

    for op_index in range(proto.getNumOperands()):
        if not should_mask_operand(ins, op_index):
            continue

        operand_mask = proto.getOperandValueMask(op_index)

        if operand_mask is None:
            continue

        operand_mask_bytes = operand_mask.getBytes()

        for i in range(min(len(mask), len(operand_mask_bytes))):
            mask[i] |= operand_mask_bytes[i] & 0xFF

    ins_bytes = ins.getBytes()

    for m, b in zip(mask, ins_bytes):
        b = b & 0xFF

        if m == 0xFF:
            yield BytePattern(True, None, "operand")
        else:
            yield BytePattern(False, b, "byte")


def get_body_ranges(fn):
    ranges = []

    it = fn.getBody().getAddressRanges()

    while it.hasNext():
        r = it.next()
        ranges.append(r)

    ranges.sort(key=lambda r: r.getMinAddress().getOffset())
    return ranges


def get_last_function_address_from_entry(fn, ranges):
    """
    Returns the highest address belonging to this function at or after entry point.
    """
    entry = fn.getEntryPoint()
    last = None

    for r in ranges:
        if r.getMaxAddress().compareTo(entry) < 0:
            continue

        if last is None or r.getMaxAddress().compareTo(last) > 0:
            last = r.getMaxAddress()

    if last is None:
        raise Exception("Could not determine last function address")

    return last


def collect_instruction_patterns(fn):
    """
    Builds a map:
        address offset -> BytePattern

    Only real instructions inside the function body are added here.
    Gaps between Ghidra body ranges are handled separately.
    """
    listing = currentProgram.getListing()
    ins_iter = listing.getInstructions(fn.getBody(), True)

    result = {}
    instruction_count = 0

    while ins_iter.hasNext():
        ins = ins_iter.next()
        instruction_count += 1

        addr = ins.getAddress()

        for i, entry in enumerate(get_masked_instruction(ins)):
            result[addr.add(i).getOffset()] = entry

    return result, instruction_count


def collect_contiguous_function_signature(fn):
    """
    Builds one contiguous pattern from function entry point to the highest
    function-body address.

    If Ghidra split the function into several ranges, the bytes between ranges
    are wildcarded instead of being skipped. This is important because SourceMod
    signatures are contiguous byte patterns.
    """
    ranges = get_body_ranges(fn)

    if len(ranges) == 0:
        raise Exception("Function body has no ranges")

    entry = fn.getEntryPoint()
    end = get_last_function_address_from_entry(fn, ranges)

    instruction_patterns, instruction_count = collect_instruction_patterns(fn)

    byte_pattern = []

    gap_count = 0
    real_byte_count = 0
    operand_wildcard_count = 0

    addr = entry

    while addr.compareTo(end) <= 0:
        offset = addr.getOffset()

        if offset in instruction_patterns:
            entry_pattern = instruction_patterns[offset]
            byte_pattern.append(entry_pattern)

            if entry_pattern.is_wildcard:
                operand_wildcard_count += 1
            else:
                real_byte_count += 1
        else:
            # Byte between Ghidra function-body ranges.
            # Keep it as wildcard because SourceMod signatures must be contiguous.
            byte_pattern.append(BytePattern(True, None, "gap"))
            gap_count += 1

        addr = addr.add(1)

    return byte_pattern, instruction_count, ranges, gap_count, real_byte_count, operand_wildcard_count


def make_findbytes_pattern(byte_pattern):
    """
    Pattern for Ghidra findBytes().
    """
    pattern = ""

    for entry in byte_pattern:
        if entry.is_wildcard:
            pattern += "."
        else:
            pattern += r"\x{:02x}".format(entry.byte)

    return pattern


def print_signature(fn, byte_pattern, instruction_count, ranges, gap_count, real_byte_count, operand_wildcard_count):
    ida_signature = " ".join(ida_str(entry) for entry in byte_pattern)
    sourcemod_signature = "".join(sourcemod_str(entry) for entry in byte_pattern)

    total_wildcards = sum(1 for entry in byte_pattern if entry.is_wildcard)

    if DEV_MODE:
        print("")
        print("============================================================")
        print("Full contiguous function signature")
        print("============================================================")
        print("Function:             {}".format(fn.getName()))
        print("Entry point:          {}".format(fn.getEntryPoint()))
        print("Body:                 {}".format(fn.getBody()))
        print("Body ranges:          {}".format(len(ranges)))
        print("Instructions:         {}".format(instruction_count))
        print("Signature bytes:      {}".format(len(byte_pattern)))
        print("Stable bytes:         {}".format(real_byte_count))
        print("Operand wildcards:    {}".format(operand_wildcard_count))
        print("Gap wildcards:        {}".format(gap_count))
        print("Total wildcards:      {}".format(total_wildcards))
        print("")

        if len(ranges) > 1:
            print("Note:")
            print("  Ghidra function body is split into multiple ranges.")
            print("  Bytes between ranges were kept as wildcard bytes.")
            print("  This is required because SourceMod signatures are contiguous.")
            print("")

        print("IDA-style signature:")
        print(ida_signature)
        print("")

        print("SourceMod signature:")
        print(sourcemod_signature)
        print("")

        print("SourceMod gamedata line:")
        print('"windows" "{}"'.format(sourcemod_signature))
        print("")

    else:
        print("")
        print("Function: {}".format(fn.getName()))
        print("")
        print("IDA-style signature:")
        print(ida_signature)
        print("")
        print("SourceMod gamedata line:")
        print('"windows" "{}"'.format(sourcemod_signature))
        print("")


def check_uniqueness(byte_pattern):
    pattern = make_findbytes_pattern(byte_pattern)
    match_limit = 128

    matches = findBytes(None, pattern, match_limit)

    if DEV_MODE:
        print("Uniqueness check:")

    if matches is None or len(matches) == 0:
        msg = "Signature has no matches. This usually means the generated pattern does not match a contiguous byte range."

        if DEV_MODE:
            print("  No matches found.")
            print("  " + msg)
        else:
            print("WARNING: " + msg)

        if REQUIRE_UNIQUE:
            raise Exception(msg)

        return matches

    if len(matches) == 1:
        if DEV_MODE:
            print("  Signature is unique.")
            print("  Match: {}".format(matches[0]))

        return matches

    msg = "Signature matched {} locations.".format(len(matches))

    if DEV_MODE:
        print("  " + msg)

        for addr in matches:
            print("    {}".format(addr))

        if len(matches) >= match_limit:
            print("  Match limit reached; there may be more matches.")
    else:
        print("WARNING: " + msg)

    if REQUIRE_UNIQUE:
        raise Exception(msg)

    return matches


if __name__ == "__main__":
    fm = currentProgram.getFunctionManager()
    fn = fm.getFunctionContaining(currentAddress)

    if fn is None:
        raise Exception("Cursor is not inside a function")

    byte_pattern, instruction_count, ranges, gap_count, real_byte_count, operand_wildcard_count = collect_contiguous_function_signature(fn)

    if len(byte_pattern) == 0:
        raise Exception("Generated signature is empty")

    print_signature(
        fn,
        byte_pattern,
        instruction_count,
        ranges,
        gap_count,
        real_byte_count,
        operand_wildcard_count
    )

    if CHECK_UNIQUENESS:
        check_uniqueness(byte_pattern)