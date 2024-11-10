using byte = unsigned char;
using word = unsigned short;
using u32 = unsigned int;

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

    void Execute (mem_t& memory); // Execute instructions

    void Assert_CF (byte reg); // Set or unset CF
    void Assert_ZF (byte reg); // Set or unset ZF
    void Assert_OF (byte reg); // Set or unset OF
    void Assert_SF (byte reg); // Set or unset SF

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
        INS_HALT = 0x35;
} cpu_t;
