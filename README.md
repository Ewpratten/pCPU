# pCPU (Physical CPU)
A computer programmed with physical switches

## What is this project?

pCPU is a pseudo-16bit CPU I built on a breadboard, powered by an arduino pro micro. The arduino runs what is basically a HAL written in C (can be found at `main.ino`). This HAL reads values from physical switches, and at the press of a button will write the current value to system memory at the current address, then move to the next address. All programs are run from address %20 (`0001 0100`) and the system uses an instruction set I designed for this project.

## Instruction set

The pCPU instruction set is an extension of the ISA I designed for my old [Dirobium project](https://github.com/Ewpratten/Dirobium). A main difference is that jumps can be made in both directions with the same instruction, and all operations will store their result in register 0.

## System

The system consists of 512 bytes of memory, and 4 general-use registers.

### System memory

System memory is split into two parts. Addresses %0 to %19 are allocated to system I/O, and addresses %20 and up are for program memory.

#### I/O Memory

| Address | Use                           |
|---------|-------------------------------|
| %0      | Hardware button toggle status |
| %1      | Raw hardware button data      |
| %2      | Raw display data              |

*Note: any program that wants to directly interact with I/O memory must first make a syscall to `#64` with the contents of register `$0` as `0` to disable the lock from the internal debugger*

## Syscalls & Reference

For info about syscalls, and ISA reference, check out the [reference doc](ref.md)