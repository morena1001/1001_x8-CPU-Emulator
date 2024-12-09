#include <iostream>

#include "os.h"

void OS::Init (cpu_t &cpu, mem_t &mem, aux_mem_t &aux_mem) {
    // Reset 
    Status_Reset (cpu, mem);

    // Read all programs loaded in aux_mem
    int address = 0;
    byte stack_counter = 0;
    while (aux_mem[address] != 0x0000) {
        p_stack[stack_counter++] = address + 2;
        p_stack[stack_counter++] = aux_mem[address + 1] - 1;
        address = aux_mem[address + 1];
    }

    for (int i = 0; i < stack_counter; i++) {
        cout << (int) p_stack [i] << endl;
    }
}

void Status_Reset (cpu_t &cpu, mem_t &mem) {
    // Reset registers
    for (byte i = 0; i < 8; i++)        cpu.GPR[i] = 0x00;
    
    // Reset flags
    cpu.CF = 0;
    cpu.ZF = 0;
    cpu.OF = 0;
    cpu.SF = 0;

    // Reset memory
    for (int i = 0x0000; i < 0xDA5C; i++) {
        mem[i] = 0x00;
    }
}