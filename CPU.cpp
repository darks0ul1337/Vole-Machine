//
// Created by pc on 11/2/2024.
//

#include "CPU.h"
string ALU::bin_to_hex(string num) { // Convert binary string to hexadecimal.
    bitset<8> set(num); // Create a bitset to hold the binary number.
    stringstream res;
    res << hex << uppercase << set.to_ulong(); // Convert binary to hexadecimal.
    string final_res = res.str();
    if (final_res.length() < 2) { // Ensure output is two characters.
        final_res = '0' + final_res;
    }
    return final_res;
}

string ALU::hex_to_bin(string num) { // Convert hexadecimal string to binary.
    string bin;
    stringstream ss;
    ss << hex << num; // Input hex string.
    unsigned int n;
    ss >> n;
    for (int i = 8 - 1; i >= 0; i--) // Convert each bit to binary.
        bin += (((n >> i) & 1) ? "1" : "0");
    return bin;
}

string ALU::HexToDec(const string& num) { // Convert hexadecimal string to decimal.
    int result = 0;
    int base = 1;

    for (int i = num.length() - 1; i >= 0; i--) { // Process each character in hex string.
        char digit = num[i];

        if (digit >= '0' && digit <= '9') { // Convert '0'-'9' to decimal.
            result += (digit - '0') * base;
        }
        else if (digit >= 'A' && digit <= 'F') { // Convert 'A'-'F' to decimal.
            result += (digit - 'A' + 10) * base;
        }
        base *= 16; // Increment base by power of 16.
    }
    return to_string(result); // Return decimal as string.
}

string ALU::DecToHex(int num) { // Convert decimal integer to hexadecimal string.
    const string hexChars = "0123456789ABCDEF"; // Hexadecimal characters.
    string result;

    do {
        result = hexChars[num % 16] + result; // Append hex character.
        num /= 16;
    } while (num > 0);

    return result;
}

int ALU::BinToDec(const string& num) {
    int result = 0;
    int index = num.length() - 1;

    // Calculate decimal value by multiplying each bit by its positional power of 2
    for (char bit : num) {
        result += (bit - '0') * pow(2, index);
        index--;  // Decrease the index for the next digit position
    }

    return result;
}

void ALU::Add(int idxReg1, int idxReg2, int idxReg3, Memory& Register) { // Add binary values.
    string i1 = hex_to_bin(Register.getCell(idxReg2)); // Convert data to binary.
    string i2 = hex_to_bin(Register.getCell(idxReg3));

    if (i2.length() > i1.length()) { // Equalize lengths for addition.
        i1 = string(i2.length() - i1.length(), '0') + i1;
    } else {
        i2 = string(i1.length() - i2.length(), '0') + i2;
    }
    string result;
    int carry = 0;
    for (int i = i1.length() - 1; i >= 0; i--) { // Perform binary addition.
        int bit1 = i1[i] - '0';
        int bit2 = i2[i] - '0';
        int sum = bit1 + bit2 + carry;
        carry = sum / 2;
        result = to_string(sum % 2) + result;
    }
    if (carry && result.length() < 8) { // Handle carry.
        result = "1" + result;
    }
    Register.setCell(idxReg1, bin_to_hex(result)); // Store result in hexadecimal.
}

void ALU::Add(Memory& Register, int idxReg1, int idxReg2, int idxReg3) { // Add floating-point values.
    struct Float8 { // Nested struct for custom 8-bit floating-point numbers.
        unsigned int sign : 1; // 1-bit sign.
        unsigned int exponent : 3; // 3-bit exponent.
        unsigned int mantissa : 4; // 4-bit mantissa.

        static Float8 fromBin(string num) { // Convert binary string to Float8.
            Float8 f8{};
            f8.sign = (num[0] == '1') ? 1 : 0;
            f8.exponent = stoi(num.substr(1, 3));
            f8.mantissa = stoi(num.substr(4));
            return f8;
        }

        static Float8 fromFloat(float num) { // Convert float to Float8 format.
            Float8 f8{};
            f8.sign = (num < 0) ? 1 : 0;
            num = fabs(num);
            int exponent = 0;
            while (num >= 2.0) {
                num /= 2.0;
                exponent++;
            }
            while (num < 1.0 && exponent > -4) {
                num *= 2.0;
                exponent--;
            }
            f8.exponent = exponent + 4;
            f8.mantissa = static_cast<int>(num * 16) & 0xF;
            return f8;
        }

        float toFloat() const { // Convert Float8 to float.
            float mantissaValue = static_cast<float>(mantissa) / 16.0f;
            float result = (1.0 + mantissaValue) * pow(2, static_cast<int>(exponent) - 4);
            return (sign == 1) ? -result : result;
        }

        static Float8 add(const Float8& a, const Float8& b) { // Add two Float8 numbers.
            float result = a.toFloat() + b.toFloat();
            return fromFloat(result);
        }

        string printBinary() const { // Return binary representation of Float8.
            stringstream ss;
            ss << sign << bitset<3>(exponent) << bitset<4>(mantissa);
            return ss.str();
        }
    };
    Float8 i1 = Float8::fromBin(hex_to_bin(Register.getCell(idxReg2))); // Convert registers to Float8.
    Float8 i2 = Float8::fromBin(hex_to_bin(Register.getCell(idxReg3)));
    Float8 result = Float8::add(i1, i2); // Perform addition.
    Register.setCell(idxReg1, bin_to_hex(result.printBinary())); // Store result.
}


void ALU::OR(Memory& Register, int idxReg1, int idxReg2, int idxReg3){
    int R1 = stoi(HexToDec(Register.getCell(idxReg2)));
    int R2 = stoi(HexToDec(Register.getCell(idxReg3)));
    int result = R1 | R2 ;
    Register.setCell(idxReg1,DecToHex(result));
}

void ALU::AND(Memory& Register, int idxReg1, int idxReg2, int idxReg3){
    int R1 = stoi(HexToDec(Register.getCell(idxReg2)));
    int R2 = stoi(HexToDec(Register.getCell(idxReg3)));
    int result = R1 & R2 ;
    Register.setCell(idxReg1,DecToHex(result));
}

void ALU::XOR(Memory& Register, int idxReg1, int idxReg2, int idxReg3){
    int R1 = stoi(HexToDec(Register.getCell(idxReg2)));
    int R2 = stoi(HexToDec(Register.getCell(idxReg3)));
    int result = R1 ^ R2 ;
    Register.setCell(idxReg1,DecToHex(result));
}

bool ALU::isValid(string &inst) { // Check whether the instruction is a valid or not.
    if (!((inst[0] >= '1' && inst[0] <= '9') || (inst[0] >= 'A' && inst[0] <= 'D') || (inst[0] >= 'a' && inst[0] <= 'd'))) {
        return false; // Return false if the instruction is not valid.
    }
    for (int i = 1; i < 4; i++) {
        if (!((inst[i] >= '0' && inst[i] <= '9') || (inst[i] >= 'A' && inst[i] <= 'F' )||(inst[i] >= 'a' && inst[i] <= 'f'))) {
            return false; // Return false if character is not hex.
        }
    }
    for (int i = 0; i < 4; ++i) {
        if (!isdigit(inst[i])){
            inst[i] = toupper(inst[i]);
        }
    }
    return true;
}

void CU::store(int idxReg, int idxMem, Memory& Register, Memory& memory) { // Store function implementation.
    memory.setCell(idxMem, Register.getCell(idxReg)); // Copy data from register to memory.
}

void CU::move(int idxReg1, int idxReg2, Memory& Register) { // Move function implementation.
    Register.setCell(idxReg2, Register.getCell(idxReg1)); // Copy data between registers.
}

void CU::Load(Memory& memory, Memory& Register, int idxReg, int idxMem) { // Load function from memory.
    Register.setCell(idxReg, memory.getCell(idxMem)); // Copy data from memory to register.
}

void CU::Load(Memory& Register, int idxReg, string idxMem) { // Load function with immediate value.
    Register.setCell(idxReg, idxMem); // Directly store value in register.
}


void CU::Jump(Memory& Register, int& counter, int idxReg, int idxMem) { // Conditional jump.
    if (Register.getCell(idxReg) == Register.getCell(0))  // If condition is met.
        counter = idxMem; // Set program counter to new address.
}


void CU::rotate(Memory& Register, int idxReg, int rotations){
    string R = alu.hex_to_bin(Register.getCell(idxReg));
    R = R.substr(R.length()-rotations,rotations) + R.substr(0,R.length()-rotations) ;
    Register.setCell(idxReg,alu.bin_to_hex(R));
}

void CU::Jump(int& counter, int idxReg, int idxMem, Memory& Register){
    stringstream ss;
    string regValue = alu.hex_to_bin(Register.getCell(idxReg));
    string reg0Value = alu.hex_to_bin(Register.getCell(0));
    if (regValue.length() > reg0Value.length())
        ss << setw(8) << setfill('0') << reg0Value;
    else
        ss << setw(8) << setfill('0') << regValue;
    if (regValue[0] > reg0Value[0])
        return;
    else if (regValue[0] < reg0Value[0])
        counter = idxMem; // Set program counter to new address.
    else{
        if (alu.BinToDec(regValue) > alu.BinToDec(reg0Value))
            counter = idxMem; // Set program counter to new address.
    }
}

void CU::halt() { // Halt program execution.
    exit(0);
}

void CPU::runNextStep(Memory& memory) { // Perform one complete step: fetch, decode, and execute.
    fetch(memory); // Fetch instruction from memory.
    execute(Register, memory, decode(instructionRegister)); // Decode and execute the fetched instruction.
}

void CPU::setCounter(string counter) {
    int count;
    cout <<counter<<endl;

    do {
        count = stoi(alu.HexToDec(counter));
    } while (count % 2 == 1 && count > 0);
    programCounter = count;
}

int CPU::getCounter() {
    return programCounter;
}

void CPU::fetch(Memory& memory) { // Fetch the instruction at the current program counter.
    instructionRegister = memory.getCell(programCounter) + memory.getCell(programCounter + 1); // Fetch 2 bytes.
    programCounter += 2; // Move program counter to next instruction.
}

vector<string> CPU::decode(string instruction) { // Decode the instruction into components based on the opcode.
    switch (instruction[0]) { // Use the first character to determine the operation.
        case '1': // Load a value from memory to a register.
            return { "1", alu.HexToDec(instruction.substr(1, 1)), alu.HexToDec(instruction.substr(2, 2)) };
        case '2': // Load an immediate value to a register.
            return { "2", alu.HexToDec(instruction.substr(1, 1)), instruction.substr(2, 2) };
        case '3': // Store a register's value into memory.
            return { "3", alu.HexToDec(instruction.substr(1, 1)), alu.HexToDec(instruction.substr(2, 2)) };
        case '4': // Move data between registers.
            return { "4", alu.HexToDec(instruction.substr(1, 1)), alu.HexToDec(instruction.substr(2, 1)), alu.HexToDec(instruction.substr(3, 1)) };
        case '5': // Add two binary values and store result.
            return { "5", alu.HexToDec(instruction.substr(1, 1)), alu.HexToDec(instruction.substr(2, 1)), alu.HexToDec(instruction.substr(3, 1)) };
        case '6': // Add two floating-point values and store result.
            return { "6", alu.HexToDec(instruction.substr(1, 1)), alu.HexToDec(instruction.substr(2, 1)), alu.HexToDec(instruction.substr(3, 1)) };
        case '7':
            return { "7", alu.HexToDec(instruction.substr(1, 1)), alu.HexToDec(instruction.substr(2, 1)), alu.HexToDec(instruction.substr(3, 1)) };
        case '8':
            return { "8", alu.HexToDec(instruction.substr(1, 1)), alu.HexToDec(instruction.substr(2, 1)), alu.HexToDec(instruction.substr(3, 1)) };
        case '9':
            return { "9", alu.HexToDec(instruction.substr(1, 1)), alu.HexToDec(instruction.substr(2, 1)), alu.HexToDec(instruction.substr(3, 1)) };
        case 'A':
            return { "A", alu.HexToDec(instruction.substr(1, 1)), alu.HexToDec(instruction.substr(2, 1)), alu.HexToDec(instruction.substr(3, 1)) };
        case 'B': // Jump to a specific memory address if a condition is met.
            return { "B", alu.HexToDec(instruction.substr(1, 1)), alu.HexToDec(instruction.substr(2, 2)) };
        case 'C': // Halt the CPU.
            return { "C" };
        case 'D':
            return  { "D" , alu.HexToDec(instruction.substr(1, 1)), alu.HexToDec(instruction.substr(2, 2))};
        case '0': // If opcode is unrecognized, halt the program.
            cout << "Opcode not found. Halting" << endl;
            return { "C" };
        default:
            break;
    }
}

void CPU::execute(Memory& Reg, Memory& memory, vector<string> instruction) { // Execute the decoded instruction.
    switch (instruction[0][0]) { // Determine the operation based on opcode.
        case '1': // Load a value from memory to a register.
            cu.Load(memory, Reg, stoi(instruction[1]), stoi(instruction[2]));
            break;
        case '2': // Load an immediate value to a register.
            cu.Load(Reg, stoi(instruction[1]), instruction[2]);
            break;
        case '3': // Store a register's value into memory.
            cu.store(stoi(instruction[1]), stoi(instruction[2]), Reg, memory);
            break;
        case '4': // Move data between registers.
            cu.move(stoi(instruction[2]), stoi(instruction[3]), Reg);
            break;
        case '5': // Add two binary values.
            alu.Add(stoi(instruction[1]), stoi(instruction[2]), stoi(instruction[3]), Reg);
            break;
        case '6': // Add two floating-point values.
            alu.Add(Reg, stoi(instruction[1]), stoi(instruction[2]), stoi(instruction[3]));
            break;
        case '7':
            alu.OR(Reg,stoi(instruction[1]), stoi(instruction[2]), stoi(instruction[3]));
            break;
        case '8':
            alu.AND(Reg,stoi(instruction[1]), stoi(instruction[2]), stoi(instruction[3]));
            break;
        case '9':
            alu.XOR(Reg,stoi(instruction[1]), stoi(instruction[2]), stoi(instruction[3]));
            break;
        case 'A':
            cu.rotate(Reg,stoi(instruction[1]), stoi(instruction[3]));
            break;
        case 'B': // Jump to a specific memory address if a condition is met.
            cu.Jump(Reg, programCounter, stoi(instruction[1]), stoi(instruction[2]));
            break;
        case 'C': // Halt the CPU.
            cu.halt();
            break;
        case 'D':
            cu.Jump(programCounter, stoi(instruction[1]), stoi(instruction[2]), Reg);
            break;
        default:
            break;
    }
}
string CPU::getReg(int idx){
    return Register.getCell(idx);
};
