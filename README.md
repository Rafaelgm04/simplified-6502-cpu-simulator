# Simplified 6502 CPU Simulator

An educational CPU simulator written in C that models a small, simplified subset of the MOS 6502 architecture. The project implements registers, 64 KB of memory, processor status flags, a reset vector, a fetch-decode-execute loop, stack operations, branches, arithmetic instructions, and basic cycle counting.

> This project is designed for learning purposes. It is not a complete or cycle-accurate implementation of the original MOS 6502.

## Features

- 64 KB addressable memory
- 16-bit Program Counter (`PC`)
- 8-bit Stack Pointer (`SP`)
- Accumulator and index registers (`A`, `X`, and `Y`)
- Processor status register with Carry, Zero, Interrupt, Decimal, Break, Overflow, and Negative flags
- Reset vector read from addresses `0xFFFC` and `0xFFFD`
- Memory read and write helpers
- Fetch-decode-execute loop
- Basic instruction cycle counter
- Error handling for unknown opcodes
- Built-in demonstration program loaded at address `0x8000`

## Implemented Instructions

| Category | Instructions |
|---|---|
| Control | `NOP`, `BRK` |
| Immediate load | `LDA`, `LDX`, `LDY` |
| Increment and decrement | `INX`, `INY`, `DEX`, `DEY` |
| Register transfer | `TAX`, `TXA`, `TAY`, `TYA` |
| Jump and branch | `JMP`, `BEQ`, `BNE` |
| Stack | `PHA`, `PLA` |
| Flag control | `CLC`, `SEC` |
| Arithmetic | `ADC`, `SBC` |

## Technologies Used

- C
- C standard library
- Fixed-width integer types from `stdint.h`
- GCC or another C11-compatible compiler
- Bitwise operations for processor flags and register behavior

## Project Structure

```text
.
├── main_6502.c   # CPU, memory, instruction execution, and demonstration program
├── Flags         # Study notes about the processor status flags
└── README.md      # Project documentation
```

## How It Works

The simulator stores the CPU state in a `CPU` structure and the complete 64 KB memory space in a `MEM` structure. During reset, the Program Counter is initialized from the two-byte reset vector stored at `0xFFFC` and `0xFFFD`.

The main loop repeatedly performs two operations:

1. `cpu_fetch` reads the next opcode from memory and advances the Program Counter.
2. `cpu_execute` decodes the opcode and updates the CPU, memory, flags, stack, or Program Counter.

The demonstration program executes the following sequence:

```text
LDA #$01
TAX
INX
ADC #$02
BRK
```

This loads `1` into the accumulator, transfers it to `X`, increments `X`, adds `2` to the accumulator, and stops execution.

## Requirements

- GCC
- A Linux, macOS, or Windows environment with a C compiler

On Ubuntu or Debian, GCC can be installed with:

```bash
sudo apt update
sudo apt install build-essential
```

## How to Compile

```bash
gcc -std=c11 -Wall -Wextra -pedantic main_6502.c -o cpu6502
```

## How to Run

Linux or macOS:

```bash
./cpu6502
```

Windows with MinGW:

```powershell
.\cpu6502.exe
```

## Example Execution

```text
PC: 8002  A: 01  X: 00  Y: 00  P: 20  SP: FD  Clock: 5
PC: 8003  A: 01  X: 01  Y: 00  P: 20  SP: FD  Clock: 9
PC: 8004  A: 01  X: 02  Y: 00  P: 20  SP: FD  Clock: 13
PC: 8006  A: 03  X: 02  Y: 00  P: 20  SP: FD  Clock: 18
BRK executado. Encerrando a execucao.
```

## What I Implemented

I implemented the CPU and memory structures, register initialization, the 6502 reset-vector behavior, processor flags, memory access functions, opcode fetching, instruction decoding, stack access through page `0x0100`, relative branches, immediate arithmetic, and a small machine-code program used to demonstrate the simulator.

## Current Limitations

- Only a subset of the 6502 instruction set is implemented.
- Most addressing modes are not implemented yet.
- The cycle counter is educational and is not fully cycle-accurate.
- Programs are currently written directly into memory inside `main`.
- Interrupt handling, memory-mapped devices, and binary program loading are not implemented.

## What I Learned

- How a CPU follows the fetch-decode-execute cycle
- How registers and memory interact during instruction execution
- How little-endian addresses are assembled from two bytes
- How processor status flags are represented with bit masks
- How a stack can be mapped to a fixed memory page
- How arithmetic, branches, and jumps change the processor state
- How machine-code instructions can be represented and executed in software

## Possible Improvements

- Separate the CPU, memory, and demonstration program into different modules
- Add all official 6502 instructions and addressing modes
- Implement a binary or hexadecimal program loader
- Add unit tests for each instruction and status flag
- Add an interactive debugger with breakpoints and memory inspection
- Improve cycle accuracy and branch timing
- Add a terminal interface showing registers, flags, memory, and the current instruction

## Author

Developed by [Rafael Gonçalves Moreira](https://github.com/Rafaelgm04).
