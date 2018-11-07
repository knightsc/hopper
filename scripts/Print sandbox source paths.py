import os

def readString(addr):
    """Read and return a string at the given address"""
    seg = Document.getCurrentDocument().getSegmentAtAddress(addr)
    string = ""
    while not seg.readByte(addr) == 0:
        string += chr(seg.readByte(addr))
        addr += 1
    return string

def getMatching(source, proc_names):
    result = []
    for name in proc_names:
        if source in name:
            result.append(name)
    return result

doc = Document.getCurrentDocument()
seg = doc.getSegmentByName("__TEXT")
sec = doc.getSectionByName("__cstring")
txt = doc.getSectionByName("__text")

start = sec.getStartingAddress()
end = start + sec.getLength()
files = {}

proc_names = set()
for addr in range(txt.getStartingAddress(), txt.getStartingAddress() + txt.getLength()):
    name = seg.getNameAtAddress(addr)
    if name:
        proc = seg.getProcedureAtAddress(addr)
        if proc:
            proc_names.add(proc.signatureString())

for addr in range(start, end):
    if seg.getTypeAtAddress(addr) == Segment.TYPE_ASCII:
        string = readString(addr)
        if "BuildRoot" in string:
            files[string] = set()          

            name = "_sb_" + os.path.basename(string) + "_"
            files[string].update(getMatching(name, proc_names))

            xrefs = seg.getReferencesOfAddress(addr)
            # if there are no XREFs the above method apparently
            # returns "None" instead of an empty list...
            if xrefs:
                for xref in xrefs:
                    xrefSegment = doc.getSegmentAtAddress(xref)
                    proc = xrefSegment.getProcedureAtAddress(xref)
                    files[string].add(proc.signatureString())
                
            addr += len(string)

for key in sorted(files.iterkeys()):
    doc.log(key)
    for method in sorted(files[key], key=lambda x: x.split()[1]):
        doc.log("    " + method)