using byte = unsigned char;
using word = unsigned short;
using u32 = unsigned int;

typedef struct AUX_MEM {
    static constexpr u32 MAX_MEM = 1024 * 64;
    word data[MAX_MEM];

    void Init ();   // Load programs to be used
    void WriteWord (word value, u32 address); // Write 2 bytes

    // Read 1 word
    word operator[] (u32 address) const {
        if (address >= MAX_MEM)     return 0;
        return data[address];
    }

    // Write 1 word
    word& operator[] (u32 address) {
        return data[address];
    }
} AUX_MEM;
