import sys

if len(sys.argv) != 2:
    print("Incorrect Usage")

file = ""
with open(sys.argv[1], "r") as f:
    file = f.read().split("\n")

ins = {
    "nop": 0,
    "mov": 1,
    "add": 2,
    "sub": 3,
    "mul": 4,
    "div": 5,
    "call": 6,
    "jmp": 7,
    "cmp": 8,
    "cmpl": 9,
    "cmpg": 10,
    "ncmp": 11
}

dt = {
    "#":128,
    "##":64,
    "$":192,
    "%":32,
    r"%%":160
}

m_ptr = 20

def bfmt(i):
    x = format(i, '08b')
    
    return x[0] + x[1] + x[2] + x[3] + " "+x[4] + x[5] + x[6] + x[7]

def pt(v, d):
    global m_ptr
    print(bfmt(m_ptr) + " # ", end="")
    m_ptr += 1
    
    print(bfmt(v) + " # ", end="")
    print(d, end="")
    print()


print(f"Assembled program {sys.argv[1]}")
print(" Mem Addr #  Mem Val  # Instruction")
for line in file:
    if line[0] == ":":
        continue
    pts = line.split(" ")
    
    for i,c in enumerate(pts):
        if i == 0:
            if c not in ins:
                break
            
            pt(ins[c], c)
        
        else:
            
            ofs = 1
            dattype = c[0]
            if c[1] in dt:
                dattype += c[1]
                ofs = 2
            
            pt(dt[dattype], dattype)
            
            number = int(c[ofs:])
            
            if number > 255:
                pt((number >> 8) & 0xff, number)
                pt((number &0xff), "")
            else:
                pt(number, number)
            
print("End Assembled Program")
