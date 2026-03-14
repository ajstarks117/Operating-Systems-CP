// #include <iostream>
// #include "../include/memory.h"

// using namespace std;

// int main() {

//     Memory mem;

//     cout << "Initial Memory State:\n";
//     mem.displayMemory();

//     char instr1[4] = {'L','R','2','0'};
//     char instr2[4] = {'S','R','3','0'};
//     char instr3[4] = {'H',' ',' ',' '};

//     mem.write(0, instr1);
//     mem.write(1, instr2);
//     mem.write(2, instr3);

//     cout << "\nMemory After Writes:\n";
//     mem.displayMemory();

//     char buffer[4];

//     mem.read(1, buffer);

//     cout << "\nReading Address 1:\n";
//     for(int i = 0; i < 4; i++)
//         cout << buffer[i];

//     cout << endl;

//     return 0;
// }

//testing loader with job to get data to block 10 and put data from block 10 and halt 

#include <iostream>
#include "../include/memory.h"
#include "../include/loader.h"

using namespace std;

int main() {
    Memory mem;
    Loader loader;

    cout << "--- Starting OS Simulation ---" << endl;
    
    // Note: Since your launch.json sets "cwd" to "${workspaceFolder}/src",
    // the relative path to the input folder is one level up ("../input/...").
    loader.load("../input/job1.txt", mem);

    cout << "\nMemory State after Loading:" << endl;
    mem.displayMemory();
   

    return 0;
}
/*Note before you have to compile the loader.cpp and memory.cpp together in order to run or you'll get a linker error if you try to do it using the run button as it compile files sequence by sequence 
the command is => 'g++ test.cpp memory.cpp loader.cpp -o test ; if ($?) { .\test }'  */