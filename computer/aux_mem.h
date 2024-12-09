#include <string>

using namespace std;

using byte = unsigned char;
using word = unsigned short;
using u32 = unsigned int;

#define IS_NUMBER(value)                (value == '0' || value == '1' || value == '2' || value == '3' || value == '4' || value == '5' || value == '6' || value == '7' || value == '8' || value == '9')
#define INT_REPRESENTATION_OF_HEX(hex_value)    (IS_NUMBER (hex_value) ? ((u32) hex_value) - 48 : (hex_value == 'A' ? 10 : (hex_value == 'B' ? 11 : (hex_value == 'C' ? 12 : (hex_value == 'D' ? 13 : (hex_value == 'E' ? 14 : (hex_value == 'F' ? 15 : 0)))))))
typedef struct AUX_MEM {
    static constexpr u32 MAX_MEM = 1024 * 64;
    word data[MAX_MEM];

    void Init ();   // Load programs to be used
    void WriteWord (word value, u32 address); // Write 2 bytes
    void Load_Program (string file_name, word& prog_id, word& address);

    // Read 1 word
    word operator[] (u32 address) const {
        if (address >= MAX_MEM)     return 0;
        return data[address];
    }

    // Write 1 word
    word& operator[] (u32 address) {
        return data[address];
    }
} aux_mem_t;

bool Next_Ins (string& value, string& program, byte& instruction);
byte String_to_Hex (string value);
