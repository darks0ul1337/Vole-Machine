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
    void Load(Memory &memory,Memory& regiseter,int idxReg, int idxMem);
    void Load(Memory& regiseter,int idxReg, string idxMem);
};

void CU::store(int idxReg,int idxMem,Memory& Register,Memory& memory){
    memory.setCell(idxMem,Register.getCell(idxReg));
}

void CU::move(int idxReg1,int idxReg2,Memory& Register){
    Register.setCell(idxReg2,Register.getCell(idxReg1));
}

void CU::Load(Memory &memory,Memory& regiseter,int idxReg, int idxMem) {
    regiseter.setCell(idxReg,memory.getCell(idxMem));
}

void CU::Load(Memory& regiseter,int idxReg, string idxMem){
    regiseter.setCell(idxReg, idxMem);
}

class ALU{
public:
    string HexToDec(const string &num);
    string DecToHex(int num);
    bool isValid(string inst);
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


bool isValid(string inst) {
    vector<char> validChar = {'1', '2', '3', '4', '5','6', '7', '8', '9', 'A','B', 'C', 'D', 'E', 'F'};
    for(char c:inst){
        if (find(validChar.begin(), validChar.end(), c) != validChar.end()) {
            continue;
        }
        else{
            return 0;
        }
    }
    return 1;
}


class CPU{
    int programCounter = 0;
    string instructionRegister;
    Memory Register;
    CU cu;
    ALU alu;

public:
    CPU() : Register(16) , cu(), alu(){}
    vector<string>  decode(string instruction);
    string fetch(Memory& memory);
};

string CPU::fetch(Memory &memory) {
    string currentInstruction;
    currentInstruction = memory.getCell(programCounter) + memory.getCell(programCounter+1);
    programCounter += 2;
    return currentInstruction;
}
vector<string> CPU::decode(string instruction) {
	switch (instruction[0]) {
	case '1':
		return { "1", alu.HexToDec(string(instruction[1],1)), instruction.substr(2, 2) };
	case '2':
		return { "2", alu.HexToDec(string(instruction[1],1)),alu.HexToDec(instruction.substr(2, 2)) };
	case '3':
		return { "3", alu.HexToDec(string(instruction[1],1)),alu.HexToDec(instruction.substr(2, 2)) };
	case '4':
		return { "4", alu.HexToDec(string(instruction[1],1)), alu.HexToDec(string(instruction[2],1)), alu.HexToDec(string(instruction[3],1)) };
	case '5':
		return { "5", alu.HexToDec(string(instruction[1],1)), alu.HexToDec(string(instruction[2],1)),alu.HexToDec(string(instruction[3],1)) };
	case '6':
		return { "6", alu.HexToDec(string(instruction[1],1)), alu.HexToDec(string(instruction[2],1)),alu.HexToDec(string(instruction[3],1)) };
	case 'B':
		return { "B", alu.HexToDec(string(instruction[1],1)),alu.HexToDec(instruction.substr(2, 2)) };
	case 'C':
		return { "C" };

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

