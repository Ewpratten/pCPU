: This program will cycle the lights on the pCPU
: Making an empty call to SYSCALL_DISP will put the screen into w mode
call #64
: Patterns
mov %2 #1
mov %2 #2
mov %2 #4
mov %2 #8
mov %2 #12
mov %2 #14
mov %2 #15
: End
call #128