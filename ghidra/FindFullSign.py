# -*- coding: utf-8 -*-
#@author 
#@category Signature
#@keybinding 
#@menupath 
#@toolbar 

from ghidra.program.model.symbol import SymbolType
from ghidra.util.task import ConsoleTaskMonitor
import string

MAX_BYTES = 64  # Количество байт из начала функции
wildcard_threshold = 3  # сколько подряд байт можно маскировать

def is_likely_pointer(value, program):
    """A simple heuristic — if the value points inside a memory segment, we consider it variable"""
    memory = program.getMemory()
    return memory.contains(toAddr(value))

# -*- coding: utf-8 -*-
#@author 
#@category Signature
#@keybinding 
#@menupath 
#@toolbar 

from ghidra.program.model.symbol import SymbolType
from ghidra.util.task import ConsoleTaskMonitor
import struct

MAX_BYTES = 64  # Number of bytes from the start of the function

def is_likely_pointer(value, program):
    memory = program.getMemory()
    return memory.contains(toAddr(value))

def get_function_bytes(func, program):
    listing = currentProgram.getListing()
    instructions = listing.getInstructions(func.getBody(), True)

    raw_bytes = []
    for ins in instructions:
        try:
            inst_bytes = ins.getBytes()
            raw_bytes.extend(bytearray(inst_bytes))
            if len(raw_bytes) >= MAX_BYTES:
                break
        except:
            continue
    return raw_bytes[:MAX_BYTES]

def to_uint32_le(byte_list):
    return struct.unpack("<I", bytearray(byte_list))[0]

def bytes_to_patterns(raw_bytes, program):
    readable = []
    byte_mask = []

    i = 0
    while i < len(raw_bytes):
        b = raw_bytes[i]

        # Checking for a 4-byte pointer
        if i + 4 <= len(raw_bytes):
            val = to_uint32_le(raw_bytes[i:i+4])
            if is_likely_pointer(val, program):
                readable.extend(['??'] * 4)
                byte_mask.extend(['\\x2A'] * 4)
                i += 4
                continue

        readable.append("{:02X}".format(b))
        byte_mask.append("\\x{:02X}".format(b))
        i += 1

    return readable, byte_mask

def find_selected_function():
    sym = getSymbolAt(currentAddress)
    if sym and sym.getSymbolType() == SymbolType.FUNCTION:
        return getFunctionAt(currentAddress)
    func = getFunctionContaining(currentAddress)
    return func

def main():
    func = find_selected_function()
    if not func:
        print("Select a function by address with the label!")
        return

    raw_bytes = get_function_bytes(func, currentProgram)
    readable, byte_mask = bytes_to_patterns(raw_bytes, currentProgram)

    print("\nGood signature:")
    print(" ".join(readable))

    print("\nBite Mask (SourceMod):")
    print("\"" + "".join(byte_mask) + "\"")

main()
