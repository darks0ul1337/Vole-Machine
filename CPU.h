//
// Created by pc on 11/2/2024.
//

#ifndef UNTITLED28_CPU_H
#define UNTITLED28_CPU_H
#include <bits/stdc++.h>
#include "Memory.h"
using namespace std;

class ALU { // Define an ALU class for arithmetic and logic operations.
public:
    string hex_to_bin(string num); // Convert hexadecimal to binary.
    string bin_to_hex(string num); // Convert binary to hexadecimal.
    string HexToDec(const string& num); // Convert hexadecimal to decimal.
    string DecToHex(int num); // Convert decimal to hexadecimal.
    int BinToDec(const string& num);
    void Add(int idReg1, int idReg2, int idReg3, Memory& Register); // Add binary values in registers.
    void Add(Memory& Register, int idxReg1, int idxReg2, int idxReg3); // Add floating-point values in registers.
    void OR(Memory& Register, int idxReg1, int idxReg2, int idxReg3);
    void AND(Memory& Register, int idxReg1, int idxReg2, int idxReg3);
    void XOR(Memory& Register, int idxReg1, int idxReg2, int idxReg3);
    bool isValid(string &inst); // Check if a string is a valid hexadecimal.
};

class CU { // Define a CU (Control Unit) class to manage operations.
private:
    bool is_halted = false;
    ALU alu; // Create instance of ALU class.
public:
    void store(int idxReg, int idxMem, Memory& Register, Memory& memory); // Store data from register to memory.
    void move(int idxReg1, int idxReg2, Memory& Register); // Move data between registers.
    void Load(Memory& memory, Memory& Register, int idxReg, int idxMem); // Load data from memory to register.
    void Load(Memory& Register, int idxReg, string idxMem); // Load immediate value to register.
    void Jump(Memory& Register, int& counter, int idxReg, int idxMem); // Conditional jump based on register value.
    void rotate(Memory& Register, int idxReg, int rotations);
    void Jump(int& counter, int idxReg, int idxMem, Memory& Register);
    void halt(); // Halt program execution.
    friend class CPU;
};




class CPU { // Define the CPU class to manage the processor's functions.
private:
    int programCounter; // Track the current execution point in memory.
    string instructionRegister; // Hold the current instruction to be executed.
    Memory Register; // Instantiate memory specifically for registers.
    CU cu; // Instantiate the Control Unit (CU) for controlling operations.
    ALU alu; // Instantiate the Arithmetic Logic Unit (ALU) for calculations.

public:
    CPU() : Register(16), cu(), alu() {} // Constructor initializes 16 registers and default CU and ALU instances.
    void runNextStep(Memory& memory); // Run the next step in the CPU's operation cycle.
    void fetch(Memory& memory); // Fetch the next instruction from memory.
    vector<string> decode(string instruction); // Decode the instruction into components.
    void execute(Memory& Reg, Memory& memory, vector<string> instruction); // Execute a decoded instruction.
    void setCounter(string counter);
    int getCounter();
    string getReg(int idx);
    bool get_is_halted();
    friend class Machine; // Grant Machine class access to CPU's private members.
};



#endif //UNTITLED28_CPU_H
