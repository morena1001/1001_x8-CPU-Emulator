#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#include "1001_x8.h"
#include "loader.h"

using namespace std;

void test1 (mem_t& mem, cpu_t& cpu);
void test2 (mem_t& mem, cpu_t& cpu);
void test3 (mem_t& mem, cpu_t& cpu);
void test4 (mem_t& mem, cpu_t& cpu);


int main (int argc, char** argv) {
    if (argc < 2) {
        cout << "ERROR : A .output file is needed to be loaded onto the cpu." << endl;
        return 0;
    }

    bool output_registers = false;
    bool output_memory = false;

    string argument;
    for (int i = 1; i < argc; i++) {
        argument = argv[i];

        if (argument[0] == '-') {
            output_registers = argument[1] == 'r';
            output_memory = argument[1] == 'm';
        } else {
            if (argument.length () > 7 && argument.substr (argument.length () - 6, -1) == "output") {
                // RUN LOADER WITH THE GIVEN FILE
                Load_Program (argument);
            } else {
                cout << "ERROR : " << argument << " is not a valid file type for the loader" << endl;
                return 0;
            }
        }
    }

    mem_t mem;
    cpu_t cpu;

    cpu.Reset (mem);

    // test1 (mem, cpu);
    // test2 (mem, cpu);
    // test3 (mem, cpu);
    test4 (mem, cpu);

    cpu.Execute (mem);

    if (output_registers) {
        printf("A : %s%d\n", cpu.GPR[A] < 0x10 ? "0" : "", cpu.GPR[A]);
        printf("B : %s%d\n", cpu.GPR[B] < 0x10 ? "0" : "", cpu.GPR[B]);
        printf("C : %s%d\n", cpu.GPR[C] < 0x10 ? "0" : "", cpu.GPR[C]);
        printf("D : %s%d\n", cpu.GPR[D] < 0x10 ? "0" : "", cpu.GPR[D]);
        printf("E : %s%d\n", cpu.GPR[E] < 0x10 ? "0" : "", cpu.GPR[E]);
        printf("F : %s%d\n", cpu.GPR[F] < 0x10 ? "0" : "", cpu.GPR[F]);
        printf("G : %s%d\n", cpu.GPR[G] < 0x10 ? "0" : "", cpu.GPR[G]);
        printf("H : %s%d\n", cpu.GPR[H] < 0x10 ? "0" : "", cpu.GPR[H]);
    }

    if (output_memory) {
        for (int i = 0; i < mem.MAX_MEM; i++) {
            byte value = mem.data[i];
            printf("%s%X ", value < 0x10 ? "0" : "", value);
        }
    }

    return 0;
}

void test1 (mem_t& mem, cpu_t& cpu) {
    // Test LOAD

    // $a -> %47 ; 0x2F at 0xBCDA
    mem.WriteByte (0x2F, 0xBCDA);

    // JMP %4096 ; 0x1000
    mem.WriteByte (0x21, 0xFFFD);
    mem.WriteWord (0x1000, 0xFFFE);

    // LOAD @A $a
    mem.WriteByte (0x01, 0x1000);
    mem.WriteByte (0x00, 0x1001);
    mem.WriteWord (0xBCDA, 0x1002);

    // HALT
    mem.WriteByte (0x35, 0x1004);
}

void test2 (mem_t& mem, cpu_t& cpu) {
    // Test LDFI, LDFR, and STOR

    // JMP %4096 ; 0x1000
    mem.WriteByte (0x21, 0xFFFD);
    mem.WriteWord (0x1000, 0xFFFE);

    // LDFI @A %41 ; 0x29
    mem.WriteByte (0x02, 0x1000);
    mem.WriteByte (0x00, 0x1001);
    mem.WriteByte (0x29, 0x1002);

    // LDFR @B @A 
    mem.WriteByte (0x03, 0x1003);
    mem.WriteByte (0x01, 0x1004);
    mem.WriteByte (0x00, 0x1005);

    // $b -> %0 ; address is 0xFF00
    // STOR $b @B
    mem.WriteByte (0x04, 0x1006);
    mem.WriteWord (0xFF00, 0x1007);
    mem.WriteByte (0x01, 0x1009);

    // HALT
    mem.WriteByte (0x35, 0x100A);
}

void test3 (mem_t& mem, cpu_t& cpu) {
    // Test SWPR and SWPM

    // $a -> 31 ; 0x1F at 0xAAAA
    mem.WriteByte (0x1F, 0xAAAA);
    
    // $a -> 100 ; 0x64 at 0xBBBB
    mem.WriteByte (0x64, 0xBBBB);

    // JMP %4096 ; 0x1000
    mem.WriteByte (0x21, 0xFFFD);
    mem.WriteWord (0x1000, 0xFFFE);

    // LDFI @A %31 ; 0x1F
    mem.WriteByte (0x02, 0x1000);
    mem.WriteByte (0x00, 0x1001);
    mem.WriteByte (0x1F, 0x1002);

    // LDFI @B %100 ; 0x64
    mem.WriteByte (0x02, 0x1003);
    mem.WriteByte (0x01, 0x1004);
    mem.WriteByte (0x64, 0x1005);

    // SWPR @A @B
    mem.WriteByte (0x05, 0x1006);
    mem.WriteByte (0x00, 0x1007);
    mem.WriteByte (0x01, 0x1008);

    // // SWPM $a $b
    mem.WriteByte (0x06, 0x1009);
    mem.WriteByte (0xAA, 0x100A);
    mem.WriteByte (0xAA, 0x100B);
    mem.WriteByte (0xBB, 0x100C);
    mem.WriteByte (0xBB, 0x100D);

    // HALT
    mem.WriteByte (0x35, 0x100E);
}

void test4 (mem_t& mem, cpu_t& cpu) {
    // Test PULR, PSHR, PSHM, PULC, PSHC, PULF, PSHF

    // JMP %4096 ; 0x1000
    mem.WriteByte (0x21, 0xFFFD);
    mem.WriteWord (0x1000, 0xFFFE);

    // LDFI @A %31 ; 0x1F
    mem.WriteByte (0x02, 0x1000);
    mem.WriteByte (0x00, 0x1001);
    mem.WriteByte (0x1F, 0x1002);

    // PSHR @A
    mem.WriteByte (0x08, 0x1003);
    mem.WriteByte (0x00, 0x1004);

    // PULR @B
    mem.WriteByte (0x07, 0x1005);
    mem.WriteByte (0x01, 0x1006);

    // HALT
    mem.WriteByte (0x35, 0x1007);
}
