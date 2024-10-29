#include <bits/stdc++.h>

using namespace std;

string bin_to_hex(string num) {
	bitset<8> set(num);
	stringstream res;
	res << hex << uppercase << set.to_ulong();
	return res.str();
}
string hex_to_bin(string num) {
	string bin = "";
	stringstream ss;
	ss << hex << num;
	unsigned int n;
	ss >> n;
	for (int i = 8 - 1; i >= 0; i--)
		bin = bin + (((n >> i) & 1) ? "1" : "0");
	return bin;
}


class Memory {
private:
	int size;
	vector<string> data;
public:
	Memory(int S) : size(S), data(S, "00") {}
	string getCell(int address);
	void setCell(int address, string value);
};

string Memory::getCell(int address) {
	if (address >= 0 && address < size) {
		return data[address];
	}
	else {
		cout << "Address is out of range";
	}
}

void Memory::setCell(int address, string value) {
	if (address >= 0 && address < size) {
		data[address] = value;
	}
	else {
		cout << "Address is out of range";
	}
}

class CU {
public:
	void store(int idxReg, int idxMem, Memory& Register, Memory& memory);
	void move(int idxReg1, int idxReg2, Memory& Register);
	void Load(Memory& memory, Memory& Register, int idxReg, int idxMem);
	void Load(Memory& Register, int idxReg, string idxMem);
	void Add(int idReg1, int idReg2, int idReg3, Memory& Register);
	void Add(Memory& Register, int idxReg1, int idxReg2, int idxReg3);
	void Jump(Memory& Register, int& counter, int idxReg, int idxMem);
	void halt();
};

void CU::store(int idxReg, int idxMem, Memory& Register, Memory& memory) {
	memory.setCell(idxMem, Register.getCell(idxReg));
}

void CU::move(int idxReg1, int idxReg2, Memory& Register) {
	Register.setCell(idxReg2, Register.getCell(idxReg1));
}

void CU::Load(Memory& memory, Memory& Register, int idxReg, int idxMem) {
	Register.setCell(idxReg, memory.getCell(idxMem));
}

void CU::Load(Memory& Register, int idxReg, string idxMem) {
	Register.setCell(idxReg, idxMem);
}

void CU::Add(int idxReg1, int idxReg2, int idxReg3, Memory& Register) {
	string i1 = hex_to_bin(Register.getCell(idxReg2));
	string i2 = hex_to_bin(Register.getCell(idxReg3));

	if (i2.length() > i1.length()) {
		i1 = string(i2.length() - i1.length(), '0') + i1;
	}
	else {
		i2 = string(i1.length() - i2.length(), '0') + i2;
	}
	string result = "";
	int carry = 0;
	for (int i = i1.length() - 1; i >= 0; i--) {
		int bit1 = i1[i] - '0';
		int bit2 = i2[i] - '0';
		int sum;

		sum = bit1 + bit2 + carry;

		carry = sum / 2;
		result = to_string(sum % 2) + result;
	}
	if (carry && result.length() < 8) {
		result = "1" + result;
	}
	Register.setCell(idxReg1, result);
}

void CU::Add(Memory& Register, int idxReg1, int idxReg2, int idxReg3) {
	struct Float8 {
		unsigned int sign : 1;
		unsigned int exponent : 3;
		unsigned int mantissa : 4;
		static Float8 fromBin(string num) {
			Float8 f8;
			f8.sign = (num[0] == '1') ? 1 : 0;
			f8.exponent = stoi(num.substr(1, 3));
			f8.mantissa = stoi(num.substr(4));
			cout << int((num[0] == '1') ? 1 : 0) << stoi(num.substr(1, 3)) << stoi(num.substr(4)) << endl;
			return f8;
		}
		static Float8 fromFloat(float num) {
			Float8 f8;
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
		float toFloat() const {
			float mantissaValue = static_cast<float>(mantissa) / 16.0f;
			float result = (1.0 + mantissaValue) * pow(2, static_cast<int>(exponent) - 4);
			return (sign == 1) ? -result : result;
		}
		static Float8 add(const Float8& a, const Float8& b) {
			float result = a.toFloat() + b.toFloat();
			return fromFloat(result);
		}
		string printBinary() const {
			stringstream ss;
			ss << sign << bitset<3>(exponent) << bitset<4>(mantissa);
			return ss.str();
		}
	};
	Float8 i1 = Float8::fromBin(hex_to_bin(Register.getCell(idxReg2)));
	Float8 i2 = Float8::fromBin(hex_to_bin(Register.getCell(idxReg3)));
	Float8 result = Float8::add(i1, i2);
	Register.setCell(idxReg1, bin_to_hex(result.printBinary()));
}

void CU::Jump(Memory& Register, int& counter, int idxReg, int idxMem) {
	if (Register.getCell(idxReg) == Register.getCell(0)) {
		if (idxMem % 2 == 0)
			counter = idxMem;
		else
			halt();
	}
}

void CU::halt() {
	exit(0);
};


class ALU {
public:
	string HexToDec(const string& num);
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
	vector<char> validChar = { '1', '2', '3', '4', '5','6', '7', '8', '9', 'A','B', 'C', 'D', 'E', 'F' };
	for (char c : inst) {
		if (find(validChar.begin(), validChar.end(), c) != validChar.end()) {
			continue;
		}
		else {
			return 0;
		}
	}
	return 1;
}


class CPU {
private:
	int programCounter = 0;
	string instructionRegister;
	Memory Register;
	CU cu;
	ALU alu;

public:
	CPU() : Register(16), cu(), alu() {}
	void runNextStep(Memory& memory);
	void fetch(Memory& memory);
	vector<string>  decode(string instruction);
	void excute(Memory& Register, Memory& memory, vector<string> instruction);
	friend class Machine;
};

void CPU::runNextStep(Memory& memory) {
	fetch(memory);
	excute(Register, memory, decode(instructionRegister));
}

void CPU::fetch(Memory& memory) {
	instructionRegister = memory.getCell(programCounter) + memory.getCell(programCounter + 1);
	programCounter += 2;
}

vector<string> CPU::decode(string instruction) {
	switch (instruction[0]) {
	case '1':
		return { "1", alu.HexToDec(string(instruction[1],1)), alu.HexToDec(instruction.substr(2, 2)) };
	case '2':
		return { "2", alu.HexToDec(string(instruction[1],1)),instruction.substr(2, 2) };
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

void CPU::excute(Memory& Register, Memory& memory, vector<string> instruction) {
	switch (instruction[0][0]) {
	case '1':
		cu.Load(memory, Register, stoi(instruction[1]), stoi(instruction[2]));
	case '2':
		cu.Load(Register, stoi(instruction[1]), instruction[2]);
	case '3':
		cu.store(stoi(instruction[1]), stoi(instruction[2]), Register, memory);
	case '4':
		cu.move(stoi(instruction[2]), stoi(instruction[3]), Register);
	case '5':
		cu.Add(stoi(instruction[1]), stoi(instruction[2]), stoi(instruction[3]), Register);
	case '6':
		cu.Add(Register, stoi(instruction[1]), stoi(instruction[2]), stoi(instruction[3]));
	case 'B':
		cu.Jump(Register, programCounter, stoi(instruction[1]), stoi(instruction[2]));
	case 'C':
		cu.halt();
	default:
		break;
	}
}


class Machine {
private:
	CPU cpu;
	Memory mainMemory;
public:
	Machine() : cpu(), mainMemory(256) {}
	void loadProgramFile();
	void run();
	void outputState();
};

void Machine::loadProgramFile() {
	fstream input_instructions("test_cases.txt");
	int idxMem = 0;
	string line;
	while (getline(input_instructions, line))
	{
		stringstream ss(line);
		string instruction;
		while (ss >> instruction) {
			mainMemory.setCell(idxMem, instruction.substr(0, 2));
			idxMem++;
			mainMemory.setCell(idxMem, instruction.substr(2, 2));
			idxMem++;
		}
	}
}

void Machine::outputState() {
	cout << "Status of the Memory:" << endl;
	for (int i = 0; i < 256; i++) {
		cout << mainMemory.getCell(i) << ' ';
		if ((i + 1) % 16 == 0) {
			cout << '\n';
		}
	}
	cout << "Status of the Register:" << endl;

	ALU alu;
	for (int i = 0; i < 16; i++) {
		cout << "Register " << alu.HexToDec(to_string(i)) << ": " << cpu.Register.getCell(i);

	}
}

void Machine::run() {
	while (true) {
		cpu.runNextStep(mainMemory);
		outputState();
	}
}



int main() {
	Machine machine;

	machine.loadProgramFile();

	// Run the program
	cout << "Running program...\n";
	machine.run();

	return 0;
}
