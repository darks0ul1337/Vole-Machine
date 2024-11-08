//
// Created by pc on 11/2/2024.
//

#include "Vole_Machine.h"
void Machine::loadProgramFile(string filename,string start) { // Load instructions from a file into main memory.
    cpu.setCounter(start);
    cout << start<<endl;
    fstream input_instructions(filename); // Open file containing program instructions.
    int idxMem = cpu.getCounter(); // Start at the beginning of memory.
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
        cpu.runNextStep(mainMemory); // Run the next step (fetch-decode-execute).
        outputState(); // Output the current state after each step.

}

string Machine::getCell(int idx){
    return mainMemory.getCell(idx);

}

string Machine::getReg(int idx){
    return cpu.getReg(idx);
}

int Machine::getCounter(){
    return cpu.getCounter();
}

bool Machine::get_is_halted(){
    return cpu.get_is_halted();
}

string Machine::getIR(){
    return cpu.instructionRegister;
}

void Machine::reseteProgram(){
    for(int i = 0; i < 256; i++){
        mainMemory.setCell(i, "00");
    }
    for(int i = 0; i < 16; i++){
        cpu.Register.setCell(i, "00");
    }
    cpu.programCounter = 0;
    cpu.instructionRegister = "0000";
    cpu.set_is_halted(false);
}

