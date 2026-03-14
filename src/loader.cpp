#include "../include/loader.h"
#include <iostream>
#include <fstream>

using namespace std;

void Loader::load(const string& filename, Memory& mem){
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Could not open job file: " << filename << endl;
        return;
    }
    string line;
    int mem_ptr = 0; // Tracks where we are writing in memory
    bool isDataArea = false;

    while (getline(file, line)) {
        // Detect Control Cards
        if (line.substr(0, 4) == "$AMJ") {
            cout << "[LOADER] Found $AMJ. Preparing to load instructions..." << endl;
            mem_ptr = 0; // Instructions always start at memory location 00
        } 
        else if (line.substr(0, 4) == "$DTA") {
            cout << "[LOADER] Found $DTA. Program loaded. Ready for execution." << endl;
            isDataArea = true;
            // Later, we will tell the CPU to start executing here!
        } 
        else if (line.substr(0, 4) == "$END") {
            cout << "[LOADER] Found $END. End of job." << endl;
        } 
        else if (!isDataArea)
        {
            // If it's not a control card, it's an instruction line!
            // Read 4 characters at a time and write to memory
            for (size_t i = 0; i < line.length() && mem_ptr < MEMORY_SIZE; i += 4) {
                char instr[4] = {' ', ' ', ' ', ' '};
                
                for (int j = 0; j < 4; j++) {
                    if (i + j < line.length()) {
                        instr[j] = line[i + j];
                    }
                }
                mem.write(mem_ptr, instr);
                mem_ptr++;

            }   
        }
    }
    file.close();
}