: A program that plays tones from the HW tone generator

~Data
: Define notes
mov %%300 #8
mov %%301 #16
mov %%302 #24
: Counter for loop
mov %%310 #0
: Max loops count
mov %%311 #5

: Main program
~Main
: Stop after n loops
cmp %%310 %%311
call #128
: Write tone 1
mov $0 %%300
call #32
: Write tone 2
mov $0 %%301
call #32
: Write tone 3
mov $0 %%302
call #32
: Incr loop counter
add %%311 #1
mov %%311 $0
: Loop
jmp ~Main