#include<bits/stdc++.h>
using namespace std;


class Memory{
private:
    int size;
    vector<string> data;
public:
    Memory(int S) : size(S) , data(S, "00"){}
    string getCell(int address);
    void setCell(int address, string value);
};

string Memory::getCell(int address){
    if (address >= 0 && address < size) {
        return data[address];
    }
    else{
        cout << "Address is out of range";
    }
}

void Memory::setCell(int address, string value){
    if (address >= 0 && address < size) {
        data[address] = value;
    }
    else{
        cout << "Address is out of range";
    }
}

class CU{
public:
    void store(int idxReg,int idxMem,Memory& Register,Memory& memory);
    void move(int idxReg1,int idxReg2,Memory& Register);
};

void CU::store(int idxReg,int idxMem,Memory& Register,Memory& memory){
    memory.setCell(idxMem,Register.getCell(idxReg));
}

void CU::move(int idxReg1,int idxReg2,Memory& Register){
    Register.setCell(idxReg2,Register.getCell(idxReg1));
}

class ALU{
public:
    string HexToDec(const string &num);
    string DecToHex(int num);

};

string ALU::DecToHex(int num) {
    string hexaList = "0123456789ABCDEF";
    string result = "";
    int decimal = num;

    while (decimal > 0) {
        result = hexaList[decimal % 16] + result;
        decimal = decimal / 16;
    }

    return result.empty() ? "0" : result;  // Return "0" if num is 0
}

string ALU::HexToDec(const string& num) {
	string hexDigits = "0123456789ABCDEF";
	int result = 0;
	int index = num.length() - 1;

	for (char digit : num) {
		result += hexDigits.find(digit) * static_cast<int>(pow(16, index));
		index--;
	}
	string str = to_string(result);
	return str.insert(0, 2 - str.length(), '0');
}

class CPU{
    int programCounter = 0;
    string instructionRegister;
    Memory Register;
    CU cu;
    ALU alu;

public:
    CPU() : Register(16) , cu(), alu(){}
    void decode(string instruction);
    string fetch(Memory& memory);
};

string CPU::fetch(Memory &memory) {
    string currentInstruction;
    currentInstruction = memory.getCell(programCounter) + memory.getCell(programCounter+1);
    programCounter += 2;
    return currentInstruction;
}
void CPU::decode(string instruction) {
    switch (instruction[0])	{
        case '1':
            // load
        case '2':
            // load
        case '3':
            // store
        case '4':
            // move
        case '5':
            // add
        case '6':
            // add
        case 'B':
            // jump
        case 'C':
            // stop
        default:
            break;
    }
}


class Machine {
protected:
    CPU* cpu;
    Memory mainMemory;
public:
    Machine() : cpu() , mainMemory(256){}
    vector<string> loadProgramFile();
    void outputState();
};
vector<string> Machine::loadProgramFile() {
    fstream input_instructions("test_cases.txt");
    vector <string> instructions;
    string line;
    while (getline(input_instructions, line))
    {
        stringstream ss(line);
        string instruction;
        while (ss >> instruction) {

            instructions.push_back(instruction);
        }
    }
}



void Machine::outputState() {
    cout << "Output" << endl;
}





int main() {

    return 0;
}

