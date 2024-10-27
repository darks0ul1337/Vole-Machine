#include <iostream>
#include <fstream>
#include "Polynomial.h" // Include your Polynomial library
#include <string>

using namespace std;
class CPU;
class Memory;
vector <string> testcases;
string line;
class Machine {
	CPU* processor;
	Memory* memory;
public:
	void loadProgramFile();
	void outputState();
};
void Machine::loadProgramFile() {
	fstream tests("test_cases.txt");
	while (getline(tests, line))
	{
		testcases.push_back(line);
	};
}

void Machine::outputState() {
	cout << "Output" << endl;
}

int main() {

	return 0;
}
