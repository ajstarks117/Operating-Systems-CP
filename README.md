# Multiprogramming Operating System (MOS) — Phase 1

This repository contains a comprehensive Phase 1 simulation of a Multiprogramming Operating System (MOS) based on the standard architecture defined in typical Operating Systems Laboratory manuals (e.g., VIT Lab Manual).

The simulation implements a virtual machine that executes machine-level instructions, manages its own memory, and handles system services through interrupts.

---

## 1. Hardware Simulation Concepts

### Virtual Machine Registers
- **IR (Instruction Register)**: A 4-character register that stores the current instruction fetched from memory.
- **R (General Purpose Register)**: A 4-character register used for data movement and comparisons.
- **IC (Instruction Counter)**: An integer register storing the address of the next instruction to be fetched (Address range: 00-99).
- **C (Toggle/Flag)**: A boolean flag used for conditional branching (`BT` instruction).
- **SI (Supervisor Interrupt)**: An integer register used to signal the Master Mode (MOS) for specific service requests.

### Memory Configuration
- **Total Capacity**: 100 words.
- **Word Size**: 4 characters (total 400 bytes).
- **Structure**: `char memory[100][4]`.
- **Initialization**: Every memory cell is initialized with blank spaces (`' '`) between jobs.

---

## 2. Operating System Modules

### Loader (LOAD)
The loader is responsible for parsing the input file (`input.txt`) and populating memory.
- **Control Cards**:
  - `$AMJ`: Resets the virtual machine (Memory, Registers, IC) to prepare for a new job.
  - `$DTA`: Signals the end of program instructions and triggers the start of execution.
  - `$END`: Marks the end of a job block.
- **Instruction Packing**: Instructions are packed into 40-character lines. The loader parses these in 4-character chunks and stores them sequentially starting from memory address `00`.

### CPU Execution Loop
The `EXECUTEUSERPROGRAM` function implements the core Fetch-Decode-Execute cycle:
1. **Fetch**: Load the word at `memory[IC]` into `IR` and increment `IC`.
2. **Decode**: Identify the opcode (first two characters) and the operand address (last two characters).
3. **Execute**: Perform the hardware operation or raise an interrupt.

### Master Mode (MOS Interrupt Handler)
When a service-related instruction is encountered, the CPU halts user-mode execution by setting the `SI` register:
- **SI = 1 (READ)**: Triggered by `GD` (Get Data). Reads a 40-character data card from `input.txt` into 10 consecutive memory words.
- **SI = 2 (WRITE)**: Triggered by `PD` (Put Data). Writes 10 consecutive memory words from the given address to `output.txt`.
- **SI = 3 (TERMINATE)**: Triggered by `H` (Halt). Finalizes the job and appends two blank lines to the output.

---

## 3. Instruction Set Architecture (ISA)

| Opcode | Instruction | Description |
|--------|-------------|-------------|
| **LR** | Load Register | `R = memory[address]` |
| **SR** | Store Register | `memory[address] = R` |
| **CR** | Compare | Sets `C = true` if `R == memory[address]`, else `false`. |
| **BT** | Branch on True | If `C == true`, sets `IC = address` (Jump). |
| **GD** | Get Data | Reads 40 chars into memory starting at `address`. Raises `SI=1`. |
| **PD** | Put Data | Writes 10 words from `address` to output. Raises `SI=2`. |
| **H**  | Halt | Stops the current program. Raises `SI=3`. |

---

## 4. How to Run

1. **Environmental Setup**: Ensure `input.txt` is present in the root directory.
2. **Compilation**:
   ```ps1
   g++ mos.cpp -o mosrun.exe
   ```
3. **Execution**:
   ```ps1
   .\mosrun.exe
   ```
4. **Output**: View the results in `output.txt`. The console will display a memory dump showing the final state of the machine for each job.

---

## 5. Input File Format Example
```text
$AMJ000100030001
GD10PD10H
$DTA
HELLOWORLD123456789012345678901234567890
$END0001
```
*Note: Instructions are written in a single continuous line as per Phase 1 standards.*