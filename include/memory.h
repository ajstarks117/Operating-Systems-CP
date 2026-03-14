#ifndef MEMORY_H
#define MEMORY_H

const int MEMORY_SIZE = 100;

class Memory {
    char memory[MEMORY_SIZE][4];
public: 
    Memory(){
        for(int i=0; i<100; i++){
            for(int j=0;j<4;j++){
                memory[i][j] = ' ';
            }
        }
    }

    void read(int address, char* data);
    void write(int address, char* data);
    void displayMemory();
};

#endif
