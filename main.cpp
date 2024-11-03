#include <bits/stdc++.h>
#include "Vole_Machine.h"


int main() { // Main function to initialize and run the Machine.
    Machine machine;

    machine.loadProgramFile(); // Load the program from the file.

    // Run the program
    cout << "Running program...\n";
    machine.run();

    return 0;
}
