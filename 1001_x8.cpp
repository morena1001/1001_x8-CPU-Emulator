#include <stdio.h>
#include <stdlib.h>

#include "1001_x8.h"

using namespace std;

/*
 * MEM STRUCT FUNCTIONS
 */

void MEM::Init () {
    for (u32 i = 0; i < MAX_MEM; i++)       data[i] = 0;
}

void MEM::WriteByte (byte value, u32 address) {
    data[address] =  value;
}

void MEM::WriteWord (word value, u32 address) {
    WriteByte (value & 0xFF, address);
    WriteByte (value >> 8, address + 1);
}

/*
 * CPU STRUCT FUNCTIONS
 */

void CPU::Reset (mem_t& memory) {
    PC = 0xFFFD;
    SP = 0x0000;

    CF = ZF = OF = SF = 0;
    GPR[A] = GPR[B] = GPR[C] = GPR[D] = GPR[E] = GPR[F] = GPR[G] = GPR[H] = 0;

    memory.Init ();
}

byte CPU::FetchByte (mem_t& memory) {
    return memory[PC++];
}

word CPU::FetchWord (mem_t& memory) {
    word data = (word) (FetchByte (memory)) | ((word) (FetchByte (memory)) << 8);
    return data;
}

byte CPU::ReadByte (u32 address, mem_t& memory) {
    return memory[address];
}

word CPU::ReadWord (u32 address, mem_t& memory) {
    word data = (word) (ReadByte (address, memory)) | ((word) (ReadByte (address + 1, memory)) << 8);
}

void CPU::Execute (mem_t& memory) {
    byte ins = FetchByte (memory);

    while (ins != INS_HALT) {
        switch (ins) {
            case INS_LDFM: {
                byte reg = FetchByte (memory);
                word address = FetchWord (memory);
                GPR[reg] = ReadByte (address, memory);                
            } break;
            case INS_LDFI: {
                byte reg = FetchByte (memory);
                byte imm = FetchByte (memory);
                GPR[reg] = imm;
            } break;
            case INS_JMP: {
                word address = FetchByte (memory);
                PC = address;
            } break;
            default : {
                printf ("Instruction %X not handled", ins);
            } break;
        }

        ins = FetchByte (memory);
    }
}

void Assert_CF (byte reg) {

}

void Assert_ZF (byte reg) {

}

void Assert_OF (byte reg) {

}

void Assert_SF (byte reg) {

}
