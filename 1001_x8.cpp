#include <stdio.h>
#include <stdlib.h>

using namespace std;

// CUSTOM DEFINED VARIABLE TYPES
using byte = unsigned char;
using word = unsigned short;
using u32  = unsigned int;

struct Mem {
    static constexpr u32 MAX_MEM = 1024 * 64;
    byte data[MAX_MEM];

    void Init () {
        for (u32 i = 0; i < MAX_MEM; i++) {
            data[i] = 0;
        }
    } 

    // Read 1 byte
    byte operator[] (u32 address) const {
        // MAKE SURE THAT ADDRESS < MAX_MEM
        return data[address];
    }   

    // Write 1 byte
    byte& operator[] (u32 address) {
        return data[address];
    }

    // Write 2 bytes
    void WriteWord (word value, u32 address, u32& cycles) {
        data[address] = value & 0xFF;
        data[address + 1] = (value >> 8);
        cycles -=2;
    }
};

struct CPU {
    word PC; // Program counter
    word SP; // Stack pointer

    byte A, X, Y; // Registers

    byte CF : 1; // Carry flag
    byte ZF : 1; // Zero flag
    byte ID : 1; // Interrupt disable
    byte DM : 1; // Decimal mode
    byte BC : 1; // Break command
    byte OF : 1; // Overflow flag
    byte NF : 1; // Negative flag

    // Opcodes 
    static constexpr byte 
        INS_LD_IM = 0x04,
        INS_LD_MEM = 0x05,
        INS_JMP = 0x00;

    void Reset (Mem& memory) {
        PC = 0xFFFC;
        SP = 0x0100;
        
        CF = ZF = ID = DM = BC = OF = NF = 0;
        A = X = Y = 0;
        memory.Init ();
    }

    byte FetchByte (u32& cycles, Mem& memory) {
        byte data = memory[PC];
        PC++;
        cycles--;
        return data;
    }

    word FetchWord (u32& cycles, Mem& memory) {
        word data = memory[PC];
        PC++;

        
    }
};
