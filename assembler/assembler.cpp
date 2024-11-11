#include <stdio.h>

#include <iostream>
#include <fstream>

using namespace std;

int main (int argc, char** argv) {
    ifstream program;
    program.open ("sample_program.lk");
    string line;

    if (program.is_open ()) {
        while (getline (program, line)) {
            cout << line << endl;
        }
        program.close ();
    } else {
        cout << "Unable to open file" << endl;
    }

    return 0;
}