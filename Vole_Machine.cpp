//
// Created by pc on 11/2/2024.
//

#include "Vole_Machine.h"
void Machine::loadProgramFile() { // Load instructions from a file into main memory.
    cpu.setcounter();
    fstream input_instructions("test_cases.txt"); // Open file containing program instructions.
    int idxMem = cpu.getcounter(); // Start at the beginning of memory.
    string line;
    while (getline(input_instructions, line)) // Read each line from the file.
    {
        stringstream ss(line);
        string instruction;
        ALU checker; // Create an ALU instance to validate instructions.
        while (ss >> instruction) { // Process each instruction.
            if (checker.isValid(instruction)) { // Validate if instruction is a valid hexadecimal.
                mainMemory.setCell(idxMem, instruction.substr(0, 2)); // Store the first byte.
                idxMem++;
                mainMemory.setCell(idxMem, instruction.substr(2, 2)); // Store the second byte.
                idxMem++;
            }
            else {
                cout << "Wrong Instruction Input!" << endl; // Print error for invalid instruction.
            }
        }
    }
}

void Machine::outputState() { // Display the current memory and register states.
    ALU dtoh;
    cout << "Status of the Memory:" << endl;
    for (int i = 0; i < 256; i++) { // Output each memory cell's contents.
        cout << mainMemory.getCell(i) << ' ';
        if ((i + 1) % 16 == 0) { // Format output in rows of 16.
            cout << '\n';
        }
    }
    cout << "Status of the Register:" << endl;

    for (int i = 0; i < 16; i++) { // Output the contents of each register.
        cout << "Register " << dtoh.DecToHex(i) << ": " << cpu.Register.getCell(i) << endl;
    }
}

void Machine::run() {
    // Run the CPU, executing each instruction until a halt condition.
    while (true) { // Continuous execution loop.
        cpu.runNextStep(mainMemory); // Run the next step (fetch-decode-execute).
        outputState(); // Output the current state after each step.
    }
}