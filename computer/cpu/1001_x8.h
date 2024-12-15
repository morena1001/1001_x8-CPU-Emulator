#include "../aux_mem/aux_mem.h"

using byte = unsigned char;
using word = unsigned short;
using u32 = unsigned int;

#define IS_NUMBER(value)                (value == '0' || value == '1' || value == '2' || value == '3' || value == '4' || value == '5' || value == '6' || value == '7' || value == '8' || value == '9')
#define INT_REPRESENTATION_OF_HEX(hex_value)    (IS_NUMBER (hex_value) ? ((u32) hex_value) - 48 : (hex_value == 'A' ? 10 : (hex_value == 'B' ? 11 : (hex_value == 'C' ? 12 : (hex_value == 'D' ? 13 : (hex_value == 'E' ? 14 : (hex_value == 'F' ? 15 : 0)))))))
#define STRING_FROM_HEX(value)          (value == 10 ? "A" : (value == 11 ? "B" : (value == 12 ? "C" : (value == 13 ? "D" : (value == 14 ? "E" : (value == 15 ? "F" : to_string (value)))))))

#define A   0
#define B   1
#define C   2
#define D   3
#define E   4
#define F   5
#define G   6
#define H   7

typedef struct MEM {
    static constexpr u32 MAX_MEM = 1024 * 64;
    byte data[MAX_MEM];

    void Init ();   // Initialize memory to 0
    void WriteByte (byte value, u32 address); // Write 1 byte
    void WriteWord (word value, u32 address); // Write 2 bytes

    // Read 1 byte
    byte operator[] (u32 address) const {
        if (address >= MAX_MEM)     return 0;
        return data[address];
    }

    // Write 1 byte
    byte& operator[] (u32 address) {
        return data[address];
    }
} mem_t;

typedef struct CPU {
    word PC; // Program counter
    byte SP; // Stack pointer;

    byte GPR[8]; // General purpose registers A, B, C, D, E, F, G, H

    byte CF : 1; // Carry flag
    byte ZF : 1; // Zero flag
    byte OF : 1; // Overflow flag
    byte SF : 1; // Sign flag

    void Reset (mem_t& memory); // Reset all registers and flags
    byte FetchByte (mem_t& memory); // Fetch a byte, updates PC
    word FetchWord (mem_t& memory); // Fetch 2 bytes, updates PC
    byte ReadByte (u32 address, mem_t& memory); // Read a byte from address, does NOT update PC
    word ReadWord (u32 address, mem_t& memory); // Read 2 bytes from address, does NOT update PC

    void PushStack (byte value, mem_t& memory); // Push a byte to the stack, updates SP
    byte PullStack (mem_t& memory); // Pulls a byte from stack, updates SP

    void Execute (mem_t& memory, aux_mem_t& aux_mem); // Execute instructions

    void Set_CF (word value); // Set or unset CF
    void Set_ZF (byte reg); // Set or unset ZF
    void Set_OF (byte comparison_value); // Set or unset OF
    void Set_SF (byte reg); // Set or unset SF

    // Register opcodes
    static constexpr byte  
        REG_A = 0x00,
        REG_B = 0x01,
        REG_C = 0x02,
        REG_D = 0x03,
        REG_E = 0x04,
        REG_F = 0x05,
        REG_G = 0x06, 
        REG_H = 0x07;

    // Instruction opcodes
    static constexpr byte
        INS_LDFM = 0x01,
        INS_LDFI = 0x02,
        INS_LDFR = 0x03,
        INS_STOR = 0x04,
        INS_SWPR = 0x05,
        INS_SWPM = 0x06,
        INS_PULR = 0x07,
        INS_PSHR = 0x08,
        INS_PSHM = 0x09,
        INS_PULC = 0x0A,
        INS_PSHC = 0x0B,
        INS_PULF = 0x0C,
        INS_PSHF = 0x0D,
        INS_AND  = 0x0E,
        INS_EOR  = 0x0F,
        INS_ORA  = 0x10,
        INS_NOT  = 0x11,
        INS_ADD  = 0x12,
        INS_SUB  = 0x13,
        INS_MULT = 0x14,
        INS_DIV  = 0x15,
        INS_CMPR = 0x16,
        INS_CMPM = 0x17,
        INS_CMPI = 0x18,
        INS_INCR = 0x19,
        INS_INCM = 0x1A,
        INS_DECR = 0x1B,
        INS_DECM = 0x1C,
        INS_SHLR = 0x1D,
        INS_SHRR = 0x1E,
        INS_SHLM = 0x1F,
        INS_SHRM = 0x20,
        INS_JMP  = 0x21,
        INS_JSR  = 0x22,
        INS_RSR  = 0x23,
        INS_JSC  = 0x24,
        INS_JSS  = 0x25,
        INS_JCC  = 0x26,
        INS_JCS  = 0x27,
        INS_JOC  = 0x28,
        INS_JOS  = 0x29,
        INS_JZC  = 0x2A,
        INS_JZS  = 0x2B,
        INS_CCF  = 0x2C,
        INS_SCF  = 0x2D,
        INS_CSF  = 0x2E,
        INS_SSF  = 0x2F,
        INS_COF  = 0x30,
        INS_SOF  = 0x31,
        INS_CZF  = 0x32,
        INS_SZF  = 0x33,
        INS_NOP  = 0x34,
        INS_HALT = 0x35,
        // INS_STRT = 0x37;
        INS_LDFO = 0x38,
        INS_STOO = 0x39,
        INS_SWPO = 0x3A,
        INS_PSHO = 0x3B,
        INS_CMPO = 0x3C,
        INS_INCO = 0x3D,
        INS_DECO = 0x3E,
        INS_SHLO = 0x3F,
        INS_SHRO = 0x40;
} cpu_t;

const byte char_bitmaps[69][7] = {
    { // 0
        0b01110,
        0b10001,
        0b10011,
        0b10101,
        0b11001,
        0b10001,
        0b01110,
    }, { // 1
        0b00100,
        0b00110,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b01110,        
    }, { // 2
       0b01110,
        0b10001,
        0b10000,
        0b01100,
        0b00010,
        0b00001,
        0b11111 
    }, { // 3
        0b01110,
        0b10001,
        0b10000,
        0b01110,
        0b10000,
        0b10001,
        0b01110
    }, { // 4
        0b01000,
        0b01100,
        0b01010,
        0b01001,
        0b11111,
        0b01000,
        0b01000
    }, { // 5
        0b11111,
        0b00001,
        0b01111,
        0b10000,
        0b10000,
        0b10001,
        0b01110
    }, { // 6
        0b11100,
        0b00010,
        0b00001,
        0b01111,
        0b10001,
        0b10001,
        0b01110
    }, { // 7
        0b11111,
        0b10000,
        0b01000,
        0b00100,
        0b00010,
        0b00010,
        0b00010
    }, { // 8
        0b01110,
        0b10001,
        0b10001,
        0b01110,
        0b10001,
        0b10001,
        0b01110
    }, { // 9
        0b01110,
        0b10001,
        0b10001,
        0b11110,
        0b10000,
        0b01000,
        0b00111
    }, { // A
        0b00100,
        0b01010,
        0b10001,
        0b10001,
        0b11111,
        0b10001,
        0b10001
    }, { // B
        0b01111,
        0b10001,
        0b10001,
        0b01111,
        0b10001,
        0b10001,
        0b01111
    }, { // C
        0b01110,
        0b10001,
        0b00001,
        0b00001,
        0b00001,
        0b10001,
        0b01110
    }, { // D
        0b01111,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b01111
    }, { // E
        0b11111,
        0b00001,
        0b00001,
        0b01111,
        0b00001,
        0b00001,
        0b11111
    }, { // F
        0b11111,
        0b00001,
        0b00001,
        0b01111,
        0b00001,
        0b00001,
        0b00001
    }, { // G
        0b01110,
        0b10001,
        0b00001,
        0b00001,
        0b11101,
        0b10001,
        0b01110
    }, { // H
        0b10001,
        0b10001,
        0b10001,
        0b11111,
        0b10001,
        0b10001,
        0b10001
    }, { // I
        0b01110,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b01110
    }, { // J
        0b10000,
        0b10000,
        0b10000,
        0b10000,
        0b10000,
        0b10001,
        0b01110
    }, { // K
        0b10001,
        0b01001,
        0b00101,
        0b00011,
        0b00101,
        0b01001,
        0b10001
    }, { // L
        0b00001,
        0b00001,
        0b00001,
        0b00001,
        0b00001,
        0b00001,
        0b11111
    }, { // M
        0b10001,
        0b11011,
        0b10101,
        0b10101,
        0b10001,
        0b10001,
        0b10001
    }, { // N
        0b10001,
        0b10001,
        0b10011,
        0b10101,
        0b11001,
        0b10001,
        0b10001
    }, { // O
        0b01110,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b01110
    }, { // P
        0b01111,
        0b10001,
        0b10001,
        0b01111,
        0b00001,
        0b00001,
        0b00001
    }, { // Q
        0b01110,
        0b10001,
        0b10001,
        0b10001,
        0b10101,
        0b01001,
        0b10110
    }, { // R
        0b01111,
        0b10001,
        0b10001,
        0b01111,
        0b00101,
        0b01001,
        0b10001
    }, { // S
        0b01110,
        0b10001,
        0b00001,
        0b01110,
        0b10000,
        0b10001,
        0b01110
    }, { // T
        0b11111,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100
    }, { // U
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b01110
    }, { // V
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b01010,
        0b00100
    }, { // W
        0b10001,
        0b10001,
        0b10001,
        0b10101,
        0b10101,
        0b11011,
        0b10001
    }, { // X
        0b10001,
        0b10001,
        0b01010,
        0b00100,
        0b01010,
        0b10001,
        0b10001
    } , { // Y
        0b10001,
        0b10001,
        0b01010,
        0b00100,
        0b00100,
        0b00100,
        0b00100
    }, { // Z
        0b11111,
        0b10000,
        0b01000,
        0b00100,
        0b00010,
        0b00001,
        0b11111
    }, { // !
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00000,
        0b00100
    }, { // @
        0b01110,
        0b10001,
        0b10101,
        0b11101,
        0b01101,
        0b00001,
        0b11110
    }, { // #
        0b01010,
        0b01010,
        0b11111,
        0b01010,
        0b11111,
        0b01010,
        0b01010
    }, { // $
        0b00100,
        0b11110,
        0b00101,
        0b01110,
        0b10100,
        0b01111,
        0b00100
    }, { // %
        0b00011,
        0b10011,
        0b01000,
        0b00100,
        0b00010,
        0b11001,
        0b11000
    }, { // ^
        0b00000,
        0b00000,
        0b00100,
        0b01010,
        0b10001,
        0b00000,
        0b00000
    }, { // &
        0b00010,
        0b00101,
        0b00101,
        0b00010,
        0b10101,
        0b01001,
        0b10110
    }, { // *
        0b00100,
        0b10101,
        0b01110,
        0b00100,
        0b01110,
        0b10101,
        0b00100
    }, { // (
        0b00100,
        0b00010,
        0b00001,
        0b00001,
        0b00001,
        0b00010,
        0b00100
    }, { // )
        0b00100,
        0b01000,
        0b10000,
        0b10000,
        0b10000,
        0b01000,
        0b00100
    }, { // -
        0b00000,
        0b00000,
        0b00000,
        0b11111,
        0b00000,
        0b00000,
        0b00000
    }, { // _
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b11111
    }, { // +
        0b00000,
        0b00100,
        0b00100,
        0b11111,
        0b00100,
        0b00100,
        0b00000
    }, { // =
        0b00000,
        0b00000,
        0b11111,
        0b00000,
        0b11111,
        0b00000,
        0b00000
    }, { // {
        0b01100,
        0b00010,
        0b00010,
        0b00001,
        0b00010,
        0b00010,
        0b01100
    }, { // [
        0b00111,
        0b00001,
        0b00001,
        0b00001,
        0b00001,
        0b00001,
        0b00111
    }, { // }
        0b00110,
        0b01000,
        0b01000,
        0b10000,
        0b01000,
        0b01000,
        0b00110
    }, { // ]
        0b11100,
        0b10000,
        0b10000,
        0b10000,
        0b10000,
        0b10000,
        0b11100
    }, { // "\"
        0b00000,
        0b00001,
        0b00010,
        0b00100,
        0b01000,
        0b10000,
        0b00000
    }, { // |
        0b00000,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00000
    }, { // :
        0b00000,
        0b00000,
        0b00100,
        0b00000,
        0b00100,
        0b00000,
        0b00000
    }, { // ;
        0b00000,
        0b00000,
        0b00100,
        0b00000,
        0b00100,
        0b00100,
        0b00010
    }, { // "
        0b01010,
        0b01010,
        0b01010,
        0b00000,
        0b00000,
        0b00000,
        0b00000
    }, { // '
        0b00100,
        0b00100,
        0b00010,
        0b00000,
        0b00000,
        0b00000,
        0b00000
    }, { // <
        0b01000,
        0b00100,
        0b00010,
        0b00001,
        0b00010,
        0b00100,
        0b01000
    }, { // ,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00100,
        0b00100,
        0b00010
    }, { // >
        0b00010,
        0b00100,
        0b01000,
        0b10000,
        0b01000,
        0b00100,
        0b00010
    }, { // .
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00100
    }, { // ?
        0b01110,
        0b10001,
        0b10000,
        0b01000,
        0b00100,
        0b00000,
        0b00100
    }, { // /
        0b00000,
        0b10000,
        0b01000,
        0b00100,
        0b00010,
        0b00001,
        0b00000
    }, { // ~
        0b00000,
        0b00000,
        0b10000,
        0b01110,
        0b00001,
        0b00000,
        0b00000
    }, { // `
        0b00100,
        0b00100,
        0b01000,
        0b00000,
        0b00000,
        0b00000,
        0b00000
    }, { //  
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000
    }
};
