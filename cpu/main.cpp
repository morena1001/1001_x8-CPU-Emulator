#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>

#include "loader.h" // Includes 1001_x8.h file

using namespace std;

int main (int argc, char** argv) {
    if (argc < 2) {
        cout << "ERROR : A .output file is needed to be loaded onto the cpu." << endl;
        return 0;
    }

    mem_t mem;
    cpu_t cpu;
    cpu.Reset (mem);

    bool output_registers = false;
    bool output_memory = false;
    bool txt_memory = false;

    string argument;
    for (int i = 1; i < argc; i++) {
        argument = argv[i];

        if (argument[0] == '-') {
            output_registers |= argument[1] == 'r';
            output_memory |= argument[1] == 'm';
            txt_memory |= argument[1] == 'o';
        } else {
            if (argument.length () > 7 && argument.substr (argument.length () - 6, -1) == "output") {
                // RUN LOADER WITH THE GIVEN FILE
                Load_Program (argument, cpu, mem);
            } else {
                cout << "ERROR : " << argument << " is not a valid file type for the loader" << endl;
                return 0;
            }
        }
    }

    cpu.Execute (mem);

    if (output_registers) {
        cout << "A : " << htos (cpu.GPR[A]) << endl;
        cout << "B : " << htos (cpu.GPR[B]) << endl;
        cout << "C : " << htos (cpu.GPR[C]) << endl;
        cout << "D : " << htos (cpu.GPR[D]) << endl;
        cout << "E : " << htos (cpu.GPR[E]) << endl;
        cout << "F : " << htos (cpu.GPR[F]) << endl;
        cout << "G : " << htos (cpu.GPR[G]) << endl;
        cout << "H : " << htos (cpu.GPR[H]) << endl;
    }

    if (txt_memory) {
        ofstream file ("memory.txt");
        char byte_info[4];

        for (int i = 0; i < mem.MAX_MEM; i++) {
            byte value = mem.data[i];
            sprintf (byte_info, "%s%X ", value < 0x10 ? "0" : "", value);
            file << byte_info;
        }
    }

    else if (output_memory) {
        for (int i = 0; i < mem.MAX_MEM; i++) {
            byte value = mem.data[i];
            printf("%s%X ", value < 0x10 ? "0" : "", value);
        }
    }

    return 0;
}
