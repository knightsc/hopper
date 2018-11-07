doc = Document.getCurrentDocument()
seg = doc.getCurrentSegment()

filepath = doc.askFile('Select jtool companion file', None, False)

with open(filepath) as fp:
    for line in fp:
        adr, sym = line.split(':')
        adr = long(adr, 16)
        seg = doc.getSegmentAtAddress(adr)
        if seg:
            if 'func_' not in sym:
                doc.setNameAtAddress(adr, sym)