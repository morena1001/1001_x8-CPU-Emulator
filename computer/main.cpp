#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>

// #include "loader.h" // Includes 1001_x8.h file
#include "os/os.h" // Includes loader.h and 1001_x8.h files
// #include "aux_mem.h"

using namespace std;

int main (int argc, char** argv) {
    mem_t mem;
    cpu_t cpu;
    os_t os;
    aux_mem_t aux_mem;
    aux_loader_t loader;
    
    // Reset memory, flags, registers and pointers
    // Load bootloader and OS into memory
    cpu.Reset (mem);

    // Load all programs in jobs.txt into auxiliary memory
    aux_mem.Init ();

    // Reset flags, RAM, and registers
    os.Init (cpu, mem, aux_mem);

    // Load_Program ("../programs/os_program.output", cpu, mem);

    // Execute the OS pointed to by the bootloader
    cpu.Execute (mem, aux_mem);
    // Load a program from auxiliary memory 
    os.Run (cpu, mem, aux_mem, loader);





    // cout << "A : " << htos (cpu.GPR[A]) << endl;
    // cout << "B : " << htos (cpu.GPR[B]) << endl;
    // cout << "C : " << htos (cpu.GPR[C]) << endl;
    // cout << "D : " << htos (cpu.GPR[D]) << endl;
    // cout << "E : " << htos (cpu.GPR[E]) << endl;
    // cout << "F : " << htos (cpu.GPR[F]) << endl;
    // cout << "G : " << htos (cpu.GPR[G]) << endl;
    // cout << "H : " << htos (cpu.GPR[H]) << endl;
    
    ofstream file ("memory.txt");
    char byte_info[4];
    int i = 0;
    file << "STACK MEMORY : \n";
    for (i; i < 0x0100; i++) {
        byte value = mem.data[i];
        sprintf (byte_info, "%s%X ", value < 0x10 ? "0" : "", value);
        file << byte_info;
    }
    
    file << "\n\nRAM : \n";
    for (i; i < 0xD858; i++) {
        byte value = mem.data[i];
        sprintf (byte_info, "%s%X ", value < 0x10 ? "0" : "", value);
        file << byte_info;
    }

    file << "\n\nVARIABLE MEMORY : \n";
    for (i; i < 0xDA58; i++) {
        byte value = mem.data[i];
        sprintf (byte_info, "%s%X ", value < 0x10 ? "0" : "", value);
        file << byte_info;
    }

    file << "\n\nAUXILIARY MEMORY MAPPED IO : \n";
    for (i; i < 0xDA5D; i++) {
        byte value = mem.data[i];
        sprintf (byte_info, "%s%X ", value < 0x10 ? "0" : "", value);
        file << byte_info;
    }

    file << "\n\nSCREEN PIXEL MEMORY : \n";
    for (i; i < 0xDE1D; i++) {
        byte value = mem.data[i];
        sprintf (byte_info, "%s%X ", value < 0x10 ? "0" : "", value);
        file << byte_info;
    }

    file << "\n\nCHARACTER MEMORY : \n";
    for (i; i < 0xE000; i++) {
        byte value = mem.data[i];
        sprintf (byte_info, "%s%X ", value < 0x10 ? "0" : "", value);
        file << byte_info;
    }

    file << "\n\nROM : \n";
    for (i; i < 0xFFFF + 1; i++) {
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

    return 0;
}
