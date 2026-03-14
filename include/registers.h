#ifndef REGISTERS_H
#define REGISTERS_H

struct Registers {
    int IC;              // Instruction Counter
    char IR[5];          // Instruction Register
    char R[5];           // General Purpose Register
    bool C;              // Toggle Flag
    int SI;              // Supervisor Interrupt
};

#endif // REGISTERS_H
