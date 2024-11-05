//
// Created by pc on 11/2/2024.
//

#ifndef UNTITLED28_VOLE_MACHINE_H
#define UNTITLED28_VOLE_MACHINE_H
#include "Memory.h"
#include "CPU.h"
#include <bits/stdc++.h>

class Machine { // Define the Machine class to represent the entire computer.
private:
    CPU cpu; // CPU instance to control program execution.
    Memory mainMemory; // Main memory for holding the program and data.
public:
    Machine() : cpu(), mainMemory(256) {} // Initialize Machine with a CPU and 256 memory cells.
    void loadProgramFile(string filename); // Load program instructions from a file.
    void run(); // Run the loaded program.
    void outputState(); // Output the current state of memory and registers.
    string getCell(int idx);
    string getReg(int idx);
};



#endif //UNTITLED28_VOLE_MACHINE_H
