#include<bits/stdc++.h>
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
		stringstream ss(line);
		string name;
		while (ss >> name) {

		testcases.push_back(name);
		}
	};
}

void Machine::outputState() {
	cout << "Output" << endl;
}

int main() {

	return 0;
}
