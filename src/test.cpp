#include <iostream>
#include "../include/memory.h"

using namespace std;

int main() {

    Memory mem;

    cout << "Initial Memory State:\n";
    mem.displayMemory();

    char instr1[4] = {'L','R','2','0'};
    char instr2[4] = {'S','R','3','0'};
    char instr3[4] = {'H',' ',' ',' '};

    mem.write(0, instr1);
    mem.write(1, instr2);
    mem.write(2, instr3);

    cout << "\nMemory After Writes:\n";
    mem.displayMemory();

    char buffer[4];

    mem.read(1, buffer);

    cout << "\nReading Address 1:\n";
    for(int i = 0; i < 4; i++)
        cout << buffer[i];

    cout << endl;

    return 0;
}