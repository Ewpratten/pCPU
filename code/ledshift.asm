: FN Skip
mov %19 #1
jmp %36
: Reset ICounter
mov %19 #1
jmp %49
: Loop
mul %19 #2
cmpg $0 #8
jmp %28
mov %19 $0
call #64
jmp %36