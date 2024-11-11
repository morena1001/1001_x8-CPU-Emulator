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

void CPU::PushStack (byte value, mem_t& memory) {
    memory.WriteByte (value, SP);
    SP++;
}

byte CPU::PullStack (mem_t& memory) {
    byte data = ReadByte (SP - 1, memory);
    SP--;
    return data;
}

void CPU::Execute (mem_t& memory) {
    byte ins = FetchByte (memory);

    while (ins != INS_HALT) {
        switch (ins) {
            case INS_LDFM: {
                byte reg = FetchByte (memory);
                word address = FetchWord (memory);
                GPR[reg] = ReadByte (address, memory);         

                Assert_SF (reg);
                Assert_ZF (reg);
            } break;

            case INS_LDFI: {
                byte reg = FetchByte (memory);
                byte imm = FetchByte (memory);
                GPR[reg] = imm;

                Assert_SF (reg);
                Assert_ZF (reg);
            } break;

            case INS_LDFR: {
                byte reg1 = FetchByte (memory);
                byte reg2 = FetchByte (memory);
                GPR[reg1] = GPR[reg2];
            } break;

            case INS_STOR: {
                word address = FetchWord (memory);
                byte reg = FetchByte (memory);
                memory.WriteByte (GPR[reg], address);
            } break;

            case INS_SWPR: {
                byte reg1 = FetchByte (memory);
                byte reg2 = FetchByte (memory);

                GPR[H] = GPR[reg1];
                GPR[reg1] = GPR[reg2];
                GPR[reg2] = GPR[H];
            } break;

            case INS_SWPM: {
                word address1 = FetchWord (memory);
                word address2 = FetchWord (memory);
                
                GPR[G] = ReadByte (address1, memory);
                GPR[H] = ReadByte (address2, memory);
                memory.WriteByte (GPR[H], address1);
                memory.WriteByte (GPR[G], address2);
            } break;

            case INS_PULR: {
                byte reg = FetchByte (memory);
                GPR[reg] = PullStack (memory);
    
                Assert_SF (reg);
                Assert_ZF (reg);
            } break;

            case INS_PSHR: {
                byte reg = FetchByte (memory);
                PushStack (GPR[reg], memory);
            } break;

            case INS_PSHM: {
                word address = FetchWord (memory);
                PushStack (ReadByte (address, memory), memory);
            } break;


            case INS_JMP: {
                word address = FetchWord (memory);
                PC = address;
            } break;

            default : {
                // printf ("Instruction 0x%X not handled\r\n", ins);
            } break;
        }

        ins = FetchByte (memory);
    }
}

void CPU::Assert_CF (byte reg) {

}

void CPU::Assert_ZF (byte reg) {
    if (GPR[reg] == 0) {
        ZF = 1;
    } else {
        ZF = 0;
    }
}

void CPU::Assert_OF (byte reg) {

}

void CPU::Assert_SF (byte reg) {
    if ((GPR[reg] >> 7) == 1) {
        SF = 1;
    } else {
        SF = 0;
    }
}
