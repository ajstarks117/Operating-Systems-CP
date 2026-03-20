// MOS Phase-1 (Stage-1) Simulation
// Input: input.txt | Output: output.txt

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "memory.h"
using namespace std;

// Global Registers
char IR[4];
char R[4];
int  IC;
bool C;
int  SI;

Memory M;
ifstream inputFile;
ofstream outputFile;
bool debug = false;

void LOAD();
void STARTEXECUTION();
void EXECUTEUSERPROGRAM();
void MOS();

// Strip trailing \r from lines (Windows line endings)
string stripCR(const string& s) {
    if (!s.empty() && s.back() == '\r')
        return s.substr(0, s.length() - 1);
    return s;
}

// Extract 2-digit operand from IR[2] and IR[3]
int getAddress() {
    return (IR[2] - '0') * 10 + (IR[3] - '0');
}

// SI=1 : Read data card (40 chars → 10 words) into memory
void READ() {
    string line;
    if (!getline(inputFile, line)) return;
    line = stripCR(line);

    int addr = getAddress();
    while ((int)line.length() < 40) line += ' ';

    for (int i = 0; i < 10; ++i) {
        char word[4];
        for (int j = 0; j < 4; ++j)
            word[j] = line[i * 4 + j];
        M.write(addr + i, word);
    }
}

// SI=2 : Write 10 words from memory to output file
void WRITE() {
    int addr = getAddress();
    for (int i = 0; i < 10; ++i) {
        char word[4];
        M.read(addr + i, word);
        for (int j = 0; j < 4; ++j)
            outputFile << word[j];
    }
    outputFile << endl;
}

// SI=3 : Terminate current job
void TERMINATE() {
    outputFile << endl;
    outputFile << endl;
}

// Interrupt handler
void MOS() {
    switch (SI) {
        case 1: READ();      break;
        case 2: WRITE();     break;
        case 3: TERMINATE(); break;
    }
}

// Fetch-Decode-Execute cycle
void EXECUTEUSERPROGRAM() {
    while (true) {
        // Bounds check
        if (IC < 0 || IC >= MEM_SIZE) break;

        // Fetch
        char word[4];
        M.read(IC, word);
        for (int i = 0; i < 4; ++i)
            IR[i] = word[i];
        IC++;

        // Check H first (no operand needed)
        if (IR[0] == 'H') {
            SI = 3; MOS(); break;
        }

        // Decode operand for all other instructions
        int addr = getAddress();

        if (IR[0] == 'L' && IR[1] == 'R')
            M.read(addr, R);
        else if (IR[0] == 'S' && IR[1] == 'R')
            M.write(addr, R);
        else if (IR[0] == 'C' && IR[1] == 'R') {
            char temp[4];
            M.read(addr, temp);
            C = true;
            for (int i = 0; i < 4; ++i)
                if (R[i] != temp[i]) { C = false; break; }
        }
        else if (IR[0] == 'B' && IR[1] == 'T') {
            if (C) IC = addr;
        }
        else if (IR[0] == 'G' && IR[1] == 'D') {
            SI = 1; MOS();
        }
        else if (IR[0] == 'P' && IR[1] == 'D') {
            SI = 2; MOS();
        }
    }

    if (debug) {
        cout << "\n-- Memory Dump --\n";
        M.displayMemory();
    }
}

void STARTEXECUTION() {
    IC = 0;
    EXECUTEUSERPROGRAM();
}

// Loader: reads input, parses control cards, loads packed instructions
void LOAD() {
    string line;
    int m = 0;

    while (getline(inputFile, line)) {
        line = stripCR(line);
        if (line.empty()) continue;

        if (line.substr(0, 4) == "$AMJ") {
            M.initialize();
            IC = 0; C = false; SI = 0;
            memset(IR, ' ', 4);
            memset(R,  ' ', 4);
            m = 0;
            cout << "[LOADER] New job detected." << endl;
        }
        else if (line.substr(0, 4) == "$DTA") {
            cout << "[LOADER] $DTA — starting execution." << endl;
            STARTEXECUTION();
        }
        else if (line.substr(0, 4) == "$END") {
            cout << "\n-- Memory Before End --" << endl;
            M.displayMemory();
            cout << "[LOADER] $END — job finished." << endl;
        }
        else {
            // Packed instructions: multiple 4-char instructions per line
            int len = (int)line.length();
            for (int i = 0; i < len; i += 4) {
                char word[4] = {' ', ' ', ' ', ' '};
                for (int j = 0; j < 4 && (i + j) < len; ++j)
                    word[j] = line[i + j];

                M.write(m, word);
                m++;
            }
        }
    }
}

int main() {
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

    cout << "=== MOS Phase-1 (Stage-1) Simulation ===" << endl;
    LOAD();
    inputFile.close();
    outputFile.close();
    cout << "Simulation complete. Check output.txt" << endl;

    return 0;
}
