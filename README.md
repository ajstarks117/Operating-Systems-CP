# Multiprogramming Operating System (MOS) Simulation

This repository contains the simulation of a Multiprogramming Operating System (MOS) implemented in C++. The project is divided into multiple phases, each adding complexity to the hardware simulation and OS services.

## Project Structure

- **[MOS_Phase_01](file:///d:/Coding/OS_CP/Operating-Systems-CP/MOS_Phase_01)**: Basic VM simulation with job loading and simple instruction execution.
- **[MOS_Phase_02](file:///d:/Coding/OS_CP/Operating-Systems-CP/MOS_Phase_02)**: Advanced simulation featuring **Virtual Memory (Paging)** and a robust **Error Handling** system with interrupts (SI, PI, TI).
- **[Docs](file:///d:/Coding/OS_CP/Operating-Systems-CP/Docs)**: Project documentation and reference materials.

---

## Phase 2 Highlights (Current Version)

Phase 2 introduces virtual memory through paging and handles multiple error conditions through hardware and software interrupts.

### 1. Hardware Simulation
- **Memory**: Expanded to 300 words (30 blocks).
- **PTR (Page Table Register)**: Points to the block containing the page table.
- **Interrupts**: 
  - `SI` (Supervisor): System calls for I/O.
  - `PI` (Program): Error conditions (Opcode, Operand, Page Fault).
  - `TI` (Timer): Job time limit exceeded.

### 2. Paging Mechanism
- Virtual addresses (00-99) are mapped to real addresses (000-299) using a Page Table.
- Frames are allocated on-demand to simulate dynamic memory management.

### 3. Error Handling
The system detects and reports:
- Time Limit Exceeded
- Line Limit Exceeded
- Out of Data
- Operation Code Error
- Operand Error
- Invalid Page Fault

---

## How to Run

### Phase 1
1. Navigate to `MOS_Phase_01`.
2. Compile: `g++ mos.cpp -o mos.exe`
3. Run: `.\mos.exe`

### Phase 2
1. Navigate to `MOS_Phase_02`.
2. Compile: `g++ mos.cpp -o mos.exe`
3. Run: `.\mos.exe`

Check the `output.txt` in the respective directory for results.