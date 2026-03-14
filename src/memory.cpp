#include "../include/memory.h"
#include <iostream>
using namespace std;

void Memory::read(int address, char* data){
    if(address < 0 || address >= MEMORY_SIZE){
        cout << "Error : Address out of bounds"<<endl;
    }
    else {
        for(int i=0;i<4;i++){
            data[i] = memory[address][i];
        }
    }
}

void Memory::write(int address, char* data){
    if(address < 0 || address >= MEMORY_SIZE){
        cout << "Error : Address out of bounds"<<endl;
    }
    else{
        for(int i=0;i<4;i++){
            memory[address][i] = data[i];
        }
    }
}

void Memory::displayMemory(){
    for(int i = 0; i < MEMORY_SIZE; i++){
        cout << i << ": ";
        for(int j = 0; j < 4; j++){
            cout << '[' << memory[i][j] << ']';
        }
        cout << '\n';
    }
}
