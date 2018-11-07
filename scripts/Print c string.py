import os

def readString(addr):
    """Read and return a string at the given address"""
    seg = Document.getCurrentDocument().getSegmentAtAddress(addr)
    string = ""
    while not seg.readByte(addr) == 0:
        string += chr(seg.readByte(addr))
        addr += 1
    return string


doc = Document.getCurrentDocument()
seg = doc.getCurrentSegment()

adr = doc.getCurrentAddress()

s = readString(adr)

doc.log(s)
