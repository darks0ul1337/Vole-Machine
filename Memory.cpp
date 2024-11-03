//
// Created by pc on 11/2/2024.
//

#include "Memory.h"


string Memory::getCell(int address) { // Implementation of the getCell function.
    if (address >= 0 && address < size) { // Check if address is within valid range.
        return data[address]; // Return data at specified address.
    }
    cout << "Address is out of range" << endl; // Print error if address is invalid.
    return "00"; // Return default value when out of range.
}

void Memory::setCell(int address, string value) { // Implementation of setCell function.
    if (address >= 0 && address < size) { // Check if address is within valid range.
        data[address] = value; // Set data at specified address.
    }
    else {
        cout << "Address is out of range" << endl; // Print error if address is invalid.
    }
}
