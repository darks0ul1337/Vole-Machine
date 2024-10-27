#include<bits/stdc++.h>
using namespace std;

class CPU;
class Memory;


class Machine {
protected:
	CPU* processor;
	Memory* memory;
public:
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
	};
}

void Machine::outputState() {
	cout << "Output" << endl;
}




class Memory{
private:
    int Size;
    vector<string> data;
public:
    Memory(int S) : Size(S) , data(S,"00"){}
    string getCell(int address);
    void setCell(int address, string value);
};

string Memory::getCell(int address){
        if (address >= 0 && address < Size) {
            return data[address];
        }
        else{
            cout << "Address is out of range";
        }
    }

void Memory::setCell(int address, string value){
        if (address >= 0 && address < Size) {
        data[address] = value;
        }
        else{
            cout << "Address is out of range";
        }
    }




class CPU{
    int counter = 0;
    Regester* instructionRegester;
public:
    string fetch(Memory& memory);
};

string CPU::fetch(Memory &memory) {
    string currentInstruction;
    currentInstruction = memory.getCell(counter++) + memory.getCell(counter++);
    return currentInstruction;
}

int main() {

	return 0;
}
