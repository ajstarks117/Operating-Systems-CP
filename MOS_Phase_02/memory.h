#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <iomanip>
using namespace std;

const int MEM_SIZE  = 300;
const int WORD_SIZE = 4;

class Memory {
private:
    char memory[MEM_SIZE][WORD_SIZE];

public:
    Memory() { initialize(); }

    void initialize() {
        for (int i = 0; i < MEM_SIZE; ++i)
            for (int j = 0; j < WORD_SIZE; ++j)
                memory[i][j] = ' ';
    }

    void read(int address, char* buffer) {
        if (address < 0 || address >= MEM_SIZE) {
            cerr << "Memory READ error: address " << address << " out of bounds." << endl;
            return;
        }
        for (int i = 0; i < WORD_SIZE; ++i)
            buffer[i] = memory[address][i];
    }

    void write(int address, const char* data) {
        if (address < 0 || address >= MEM_SIZE) {
            cerr << "Memory WRITE error: address " << address << " out of bounds." << endl;
            return;
        }
        for (int i = 0; i < WORD_SIZE; ++i)
            memory[address][i] = data[i];
    }

    void displayMemory() {
        cout << "\n--- Memory Dump (300 words) ---" << endl;
        for (int i = 0; i < MEM_SIZE; ++i) {
            cout << setw(3) << setfill('0') << i << ": ";
            for (int j = 0; j < WORD_SIZE; ++j)
                cout << "[" << memory[i][j] << "]";
            cout << endl;
        }
        cout << "-------------------------------\n" << endl;
    }
};

#endif
