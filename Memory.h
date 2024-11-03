//
// Created by pc on 11/2/2024.
//

#ifndef UNTITLED28_MEMORY_H
#define UNTITLED28_MEMORY_H
#include <bits/stdc++.h>
using namespace std;

class Memory { // Define a Memory class to represent memory storage.
private:
    int size; // Store the size of the memory.
    vector<string> data; // A vector to hold the data in the memory cells as strings.
public:
    explicit Memory(int S) : size(S), data(S, "00") {} // Constructor to initialize memory with a default value "00".
    string getCell(int address); // Function to retrieve data from a specific memory address.
    void setCell(int address, string value); // Function to set data at a specific memory address.
};



#endif //UNTITLED28_MEMORY_H
