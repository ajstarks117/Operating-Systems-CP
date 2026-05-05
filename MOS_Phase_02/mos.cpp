// MOS Phase-1 (Stage-1) Simulation
// Input: input.txt | Output: output.txt

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "memory.h"
using namespace std;

#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

// Phase 2 Registers and Counters
char IR[4];
char R[4];
int  IC;
bool C;
int  SI;
int  PI;
int  TI;
int  PTR; // Page Table Register
bool terminateFlag; // To signal job termination

struct PCB {
    int jobID;
    int TTL; // Total Time Limit
    int TLL; // Total Line Limit
    int TTC; // Total Time Counter
    int LLC; // Line Limit Counter
} pcb;

Memory M;
bool blockUsed[30]; // To track allocated blocks (30 blocks of 10 words each)
ifstream inputFile;
ofstream outputFile;
bool debug = true;

// Prototypes
void LOAD();
void STARTEXECUTION();
void EXECUTEUSERPROGRAM();
void MOS();
int  ADDRESSMAP(int VA);
int  ALLOCATE();
void TERMINATE(int EM);

// Restore Random Allocation for Phase 2
int ALLOCATE() {
    int block;
    do {
        block = rand() % 30;
    } while (blockUsed[block]);
    blockUsed[block] = true;
    return block;
}

// Map Virtual Address (VA) to Real Address (RA)
// Returns real address or sets PI/returns -1 on error
int ADDRESSMAP(int VA) {
    if (VA < 0 || VA >= 100) {
        PI = 2; // Operand Error
        return -1;
    }

    int pageNum = VA / 10;
    int offset = VA % 10;

    // Read Page Table Entry from memory
    // Page table is stored at block PTR*10
    char pte[4];
    M.read(PTR * 10 + pageNum, pte);

    if (pte[0] == '*') {
        PI = 3; // Page Fault
        return -1;
    }

    // Convert PTE (stored as characters) to integer block number
    int block = (pte[2] - '0') * 10 + (pte[3] - '0');
    return block * 10 + offset;
}

// Strip trailing \r from lines (Windows line endings)
string stripCR(const string& s) {
    if (!s.empty() && s.back() == '\r')
        return s.substr(0, s.length() - 1);
    return s;
}

// Extract 2-digit operand from IR[2] and IR[3]
int getAddress() {
    if (!isdigit(IR[2]) || !isdigit(IR[3])) {
        PI = 2; // Operand Error
        return -1;
    }
    return (IR[2] - '0') * 10 + (IR[3] - '0');
}

// SI=1 : Read data card into memory (at mapped real address)
void READ() {
    string line;
    if (!getline(inputFile, line)) return;
    line = stripCR(line);

    if (line.substr(0, 4) == "$END") {
        TERMINATE(1); // Out of Data
        return;
    }

    int addr = getAddress();
    if(PI != 0) return; 
    int ra = ADDRESSMAP(addr);
    
    if (ra == -1) return; // PI already set (should be handled in MOS)

    while ((int)line.length() < 40) line += ' ';

    for (int i = 0; i < 10; ++i) {
        char word[4];
        for (int j = 0; j < 4; ++j)
            word[j] = line[i * 4 + j];
        M.write(ra + i, word);
    }
}

// SI=2 : Write 10 words from memory to output file
void WRITE() {
    // Check BEFORE incrementing
    if (pcb.LLC + 1 > pcb.TLL) {
        TERMINATE(2); // Line Limit Exceeded
        return;
    }
    
    pcb.LLC++;

    int addr = getAddress();
    if (PI != 0) return;
    int ra = ADDRESSMAP(addr);
    if (ra == -1) return; // PI already set (Operand Error or Page Fault)

    for (int i = 0; i < 10; ++i) {
        char word[4];
        M.read(ra + i, word);
        
        for (int j = 0; j < 4; ++j) outputFile << word[j];
    }
    outputFile << endl;
}

void TERMINATE(int EM) {
    terminateFlag = true;
    outputFile << "\nJOB ID: " << setfill('0') << setw(4) << pcb.jobID;
    switch (EM) {
        case 0: outputFile << " - NO ERROR"; break;
        case 1: outputFile << " - OUT OF DATA"; break;
        case 2: outputFile << " - LINE LIMIT EXCEEDED"; break;
        case 3: outputFile << " - TIME LIMIT EXCEEDED"; break;
        case 4: outputFile << " - OPERATION CODE ERROR"; break;
        case 5: outputFile << " - OPERAND ERROR"; break;
        case 6: outputFile << " - INVALID PAGE FAULT"; break;
    }
    outputFile << "\nIC: " << setw(2) << IC << "  IR: ";
    for(int i=0; i<4; i++) outputFile << IR[i];
    outputFile << "  TTC: " << pcb.TTC << "  LLC: " << pcb.LLC << endl;
    outputFile << endl << endl;

    if (debug) {
        M.displayMemory();
    }
}

// Interrupt handler (Decision Table)
// Interrupt handler (Decision Table)
void MOS() {
    if (TI == 0) {
        if (SI == 1) { READ(); SI = 0; }
        else if (SI == 2) { WRITE(); SI = 0; }
        else if (SI == 3) { TERMINATE(0); SI = 0; }
        else if (PI == 1) { TERMINATE(4); PI = 0; }
        else if (PI == 2) { TERMINATE(5); PI = 0; }
        else if (PI == 3) {
            // Valid Page Fault check
            string op = ""; op += IR[0]; op += IR[1];
            if (op == "GD" || op == "SR") {
                int va = getAddress();
                int pageNum = va / 10;
                int frame = ALLOCATE();
                char newPte[4] = {' ', ' ', (char)((frame / 10) + '0'), (char)((frame % 10) + '0')};
                M.write(PTR * 10 + pageNum, newPte);
                PI = 0;
                IC--; // Re-execute the instruction
            } else {
                TERMINATE(6); // Invalid Page Fault
                PI = 0;
            }
        }
    } else if (TI == 2) {
        if (SI == 1) {
            TERMINATE(3);
        }
        else if (SI == 2) {
            // FIX #5: Check LINE LIMIT priority before TIME LIMIT
            if (pcb.LLC + 1 > pcb.TLL) {
                TERMINATE(2);  // LINE LIMIT EXCEEDED takes priority
            } else {
                WRITE();
                TERMINATE(3);  // TIME LIMIT EXCEEDED
            }
        }
        else if (SI == 3) {
            TERMINATE(0);
        }
        else if (PI == 1) {
            TERMINATE(3);
        }
        else if (PI == 2) {
            TERMINATE(3);
        }
        else if (PI == 3) {
            TERMINATE(3);
        }
        else {
            TERMINATE(3);
        }
        TI = 0;
    }
}

// Fetch-Decode-Execute cycle (Slave Mode)
void EXECUTEUSERPROGRAM() {
    while (!terminateFlag) {
        // Map IC to Real Address
        int ra = ADDRESSMAP(IC);
        if (PI != 0) {
            MOS(); 
            continue;
        }

        // Fetch
        char word[4];
        M.read(ra, word);
        for (int i = 0; i < 4; ++i)
            IR[i] = word[i];
        IC++;

        // Increment Time Counter
        pcb.TTC++;
        if ((IR[0] == 'G' && IR[1] == 'D') || (IR[0] == 'S' && IR[1] == 'R')) {
            pcb.TTC++;
        }
        // FIXED: Added closing brace above
        if (pcb.TTC > pcb.TTL) TI = 2;

        // Check H first (no operand needed)
        if (IR[0] == 'H') {
            SI = 3; 
            MOS();
            continue;
        }

        // Decode operand for all other instructions
        int va = getAddress();
        if (PI != 0) { // Operand Error from getAddress
            MOS();
            continue;
        }

        int ra_op = ADDRESSMAP(va);
        if (PI != 0) { // Page Fault or Operand Error
            MOS();
            continue;
        }
        
        // Operation Code Check
        string op = ""; op += IR[0]; op += IR[1];
        if (op != "LR" && op != "SR" && op != "CR" && op != "BT" && op != "GD" && op != "PD") {
            PI = 1; 
            MOS();
            continue;
        }

        if (IR[0] == 'L' && IR[1] == 'R')
            M.read(ra_op, R);
        else if (IR[0] == 'S' && IR[1] == 'R')
            M.write(ra_op, R);
        else if (IR[0] == 'C' && IR[1] == 'R') {
            char temp[4];
            M.read(ra_op, temp);
            C = true;
            for (int i = 0; i < 4; ++i)
                if (R[i] != temp[i]) { C = false; break; }
        }
        else if (IR[0] == 'B' && IR[1] == 'T') {
            if (C) IC = va;
        }
        else if (IR[0] == 'G' && IR[1] == 'D') {
            SI = 1; 
            MOS();
        }
        else if (IR[0] == 'P' && IR[1] == 'D') {
            SI = 2; 
            MOS();
        }

        if (TI != 0 && !terminateFlag) MOS();
    }
}

void STARTEXECUTION() {
    IC = 0;
    terminateFlag = false;
    EXECUTEUSERPROGRAM();
}

// Loader: reads input, parses control cards, loads packed instructions via paging
void LOAD() {
    string line;
    while (getline(inputFile, line)) {
        line = stripCR(line);
        if (line.empty()) continue;

        if (line.substr(0, 4) == "$AMJ") {
            // Reset VM state for new job
            M.initialize();
            for(int i=0; i<30; i++) blockUsed[i] = false;
            IC = 0; C = false; SI = 0; PI = 0; TI = 0;
            memset(IR, ' ', 4);
            memset(R,  ' ', 4);

            // Initialize PCB
            pcb.jobID = stoi(line.substr(4, 4));
            pcb.TTL = stoi(line.substr(8, 4));
            pcb.TLL = stoi(line.substr(12, 4));
            pcb.TTC = 0;
            pcb.LLC = 0;

            // Allocate block for Page Table
            PTR = ALLOCATE();
            for(int i=0; i<10; i++) {
                char pte[4] = {'*', '*', '*', '*'};
                M.write(PTR * 10 + i, pte);
            }

            cout << "[LOADER] New job: " << pcb.jobID << " TTL: " << pcb.TTL << " TLL: " << pcb.TLL << endl;
        }
        else if (line.substr(0, 4) == "$DTA") {
            STARTEXECUTION();
        }
        else if (line.substr(0, 4) == "$END") {
            cout << "[LOADER] Job " << pcb.jobID << " finished." << endl;
        }
        else {
            // Program Card: Allocate a frame and update page table
            int frame = ALLOCATE();
            int pageNum = 0;
            // Find first empty page table entry
            for(int i=0; i<10; i++) {
                char pte[4];
                M.read(PTR * 10 + i, pte);
                if(pte[0] == '*') {
                    pageNum = i;
                    char newPte[4] = {' ', ' ', (char)((frame / 10) + '0'), (char)((frame % 10) + '0')};
                    M.write(PTR * 10 + i, newPte);
                    break;
                }
            }

            // Load 10 words into the allocated frame
            int len = (int)line.length();
            int i = 0;
            int m = 0;
            while (i < len && m < 10) {
                char word[4] = {' ', ' ', ' ', ' '};
                if (line[i] == 'H') {
                    word[0] = 'H';
                    i += 1;
                } else {
                    for (int j = 0; j < 4 && (i + j) < len; ++j)
                        word[j] = line[i + j];
                    i += 4;
                }
                M.write(frame * 10 + m, word);
                m++;
            }
        }
    }
}

int main() {
    srand(time(0));
    inputFile.open("input.txt");
    if (!inputFile.is_open()) {
        cerr << "Error: Cannot open input.txt" << endl;
        return 1;
    }

    outputFile.open("output.txt");
    if (!outputFile.is_open()) {
        cerr << "Error: Cannot open output.txt" << endl;
        return 1;
    }

    cout << "=== MOS Phase-2 (Stage-II) Simulation ===" << endl;
    LOAD();
    inputFile.close();
    outputFile.close();
    cout << "Simulation complete. Check output.txt" << endl;

    return 0;
}
