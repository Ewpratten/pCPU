import sys
import string

with open(sys.argv[1], "r") as f:
    
    file = f.read().split("\n")
    
    for line in file:
        if not line:
            continue
        if line[0] not in ["0","1","2","3","4","5","6","7","8","9"]:
            continue
        
        segs = line.split(" ")
        
        byte = segs[3] + segs[4]
        
        byte = int(byte, 2)
        
        print(chr(byte), end="")