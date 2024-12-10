#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>

// #include "loader.h" // Includes 1001_x8.h file
#include "os/os.h" // Includes loader.h and 1001_x8.h files
// #include "aux_mem.h"

using namespace std;

int main (int argc, char** argv) {
    // if (argc < 2) {
    //     cout << "ERROR : A .output file is needed to be loaded onto the cpu." << endl;
    //     return 0;
    // }

    mem_t mem;
    cpu_t cpu;
    os_t os;
    aux_mem_t aux_mem;
    
    cpu.Reset (mem);
    aux_mem.Init ();
    os.Init (cpu, mem, aux_mem);

    Load_Program_From_AuxMem (cpu, mem, aux_mem);

    // cpu.Execute (mem);



    cout << "A : " << htos (cpu.GPR[A]) << endl;
    cout << "B : " << htos (cpu.GPR[B]) << endl;
    cout << "C : " << htos (cpu.GPR[C]) << endl;
    cout << "D : " << htos (cpu.GPR[D]) << endl;
    cout << "E : " << htos (cpu.GPR[E]) << endl;
    cout << "F : " << htos (cpu.GPR[F]) << endl;
    cout << "G : " << htos (cpu.GPR[G]) << endl;
    cout << "H : " << htos (cpu.GPR[H]) << endl;
    
    ofstream file ("memory.txt");
    char byte_info[4];
    int i = 0;
    file << "STACK MEMORY : \n";
    for (i; i < 0x00FF; i++) {
        byte value = mem.data[i];
        sprintf (byte_info, "%s%X ", value < 0x10 ? "0" : "", value);
        file << byte_info;
    }
    
    file << "\n\nRAM : \n";
    for (++i; i < 0xD85C; i++) {
        byte value = mem.data[i];
        sprintf (byte_info, "%s%X ", value < 0x10 ? "0" : "", value);
        file << byte_info;
    }

    file << "\n\nVARIABLE MEMORY : \n";
    for (++i; i < 0xDA5C; i++) {
        byte value = mem.data[i];
        sprintf (byte_info, "%s%X ", value < 0x10 ? "0" : "", value);
        file << byte_info;
    }

    file << "\n\nSCREEN PIXEL MEMORY : \n";
    for (++i; i < 0xDE1C; i++) {
        byte value = mem.data[i];
        sprintf (byte_info, "%s%X ", value < 0x10 ? "0" : "", value);
        file << byte_info;
    }

    file << "\n\nCHARACTER MEMORY : \n";
    for (++i; i < 0xDFFF; i++) {
        byte value = mem.data[i];
        sprintf (byte_info, "%s%X ", value < 0x10 ? "0" : "", value);
        file << byte_info;
    }

    file << "\n\nROM : \n";
    for (++i; i < 0xFFFF + 1; i++) {
        byte value = mem.data[i];
        sprintf (byte_info, "%s%X ", value < 0x10 ? "0" : "", value);
        file << byte_info;
    }

    ofstream file2 ("aux_mem.txt");
    char word_info[8];
    for (int i = 0; i < aux_mem.MAX_MEM; i++) {
        word value = aux_mem.data[i];
        sprintf (word_info, "%s%s%s%X ", value < 0x1000 ? "0" : "", value < 0x100 ? "0" : "", value < 0x10 ? "0" : "", value);
        file2 << word_info;
    }



    // bool output_registers = false;
    // bool output_memory = false;
    // bool txt_memory = false;
    // bool chunk_memory = false;

    // string argument;
    // for (int i = 1; i < argc; i++) {
    //     argument = argv[i];

    //     if (argument[0] == '-') {
    //         output_registers |= argument[1] == 'r';
    //         output_memory |= argument[1] == 'm';
    //         txt_memory |= argument[1] == 'o';
    //         chunk_memory |= argument[1] == 'c';
    //     } else {
    //         if (argument.length () > 7 && argument.substr (argument.length () - 6, -1) == "output") {
    //             // RUN LOADER WITH THE GIVEN FILE
    //             Load_Program (argument, cpu, mem);
    //         } else {
    //             cout << "ERROR : " << argument << " is not a valid file type for the loader" << endl;
    //             return 0;
    //         }
    //     }
    // }

    // cpu.Execute (mem);

    // if (output_registers) {
    //     cout << "A : " << htos (cpu.GPR[A]) << endl;
    //     cout << "B : " << htos (cpu.GPR[B]) << endl;
    //     cout << "C : " << htos (cpu.GPR[C]) << endl;
    //     cout << "D : " << htos (cpu.GPR[D]) << endl;
    //     cout << "E : " << htos (cpu.GPR[E]) << endl;
    //     cout << "F : " << htos (cpu.GPR[F]) << endl;
    //     cout << "G : " << htos (cpu.GPR[G]) << endl;
    //     cout << "H : " << htos (cpu.GPR[H]) << endl;
    // }

    // if (txt_memory) {
        // ofstream file ("memory.txt");
        // char byte_info[4];

        // if (!chunk_memory) {
        //     for (int i = 0; i < mem.MAX_MEM; i++) {
        //         byte value = mem.data[i];
        //         sprintf (byte_info, "%s%X ", value < 0x10 ? "0" : "", value);
        //         file << byte_info;
        //     }
        // } else {
            // int i = 0;
            // file << "STACK MEMORY : \n";
            // for (i; i < 0x00FF; i++) {
            //     byte value = mem.data[i];
            //     sprintf (byte_info, "%s%X ", value < 0x10 ? "0" : "", value);
            //     file << byte_info;
            // }
            
            // file << "\n\nRAM : \n";
            // for (++i; i < 0xD85C; i++) {
            //     byte value = mem.data[i];
            //     sprintf (byte_info, "%s%X ", value < 0x10 ? "0" : "", value);
            //     file << byte_info;
            // }

            // file << "\n\nVARIABLE MEMORY : \n";
            // for (++i; i < 0xDA5C; i++) {
            //     byte value = mem.data[i];
            //     sprintf (byte_info, "%s%X ", value < 0x10 ? "0" : "", value);
            //     file << byte_info;
            // }

            // file << "\n\nSCREEN PIXEL MEMORY : \n";
            // for (++i; i < 0xDE1C; i++) {
            //     byte value = mem.data[i];
            //     sprintf (byte_info, "%s%X ", value < 0x10 ? "0" : "", value);
            //     file << byte_info;
            // }

            // file << "\n\nCHARACTER MEMORY : \n";
            // for (++i; i < 0xDFFF; i++) {
            //     byte value = mem.data[i];
            //     sprintf (byte_info, "%s%X ", value < 0x10 ? "0" : "", value);
            //     file << byte_info;
            // }

            // file << "\n\nROM : \n";
            // for (++i; i < 0xFFFF; i++) {
            //     byte value = mem.data[i];
            //     sprintf (byte_info, "%s%X ", value < 0x10 ? "0" : "", value);
            //     file << byte_info;
            // }
        // }
    // }

    // else if (output_memory) {
    //     if (!chunk_memory) {
    //         for (int i = 0; i < mem.MAX_MEM; i++) {
    //             byte value = mem.data[i];
    //             printf("%s%X ", value < 0x10 ? "0" : "", value);
    //         }
    //     } else {
    //         int i = 0;
    //         printf ("STACK MEMORY : \n");
    //         for (i; i < 0x00FF; i++) {
    //             byte value = mem.data[i];
    //             printf("%s%X ", value < 0x10 ? "0" : "", value);
    //         }

    //         printf ("\n\nRAM: \n");
    //         for (++i; i < 0xD85C; i++) {
    //             byte value = mem.data[i];
    //             printf("%s%X ", value < 0x10 ? "0" : "", value);
    //         }

    //         printf ("\n\nVARIABLE MEMORY : \n");
    //         for (++i; i < 0xDA5C; i++) {
    //             byte value = mem.data[i];
    //             printf("%s%X ", value < 0x10 ? "0" : "", value);
    //         }

    //         printf ("\n\nSCREEN PIXEL MEMORY : \n");
    //         for (++i; i < 0xDE1C; i++) {
    //             byte value = mem.data[i];
    //             printf("%s%X ", value < 0x10 ? "0" : "", value);
    //         }

    //         printf ("\n\nCHARACTER MEMORY : \n");
    //         for (++i; i < 0xDFFF; i++) {
    //             byte value = mem.data[i];
    //             printf("%s%X ", value < 0x10 ? "0" : "", value);
    //         }

    //         printf ("\n\nROM : \n");
    //         for (++i; i < 0xFFFF; i++) {
    //             byte value = mem.data[i];
    //             printf("%s%X ", value < 0x10 ? "0" : "", value);
    //         }
    //     }
    // }

    return 0;
}
