#include <stdint.h>

#define NEEDS_NO_OPERANDS(opcode)       ((opcode >= 0x0A && opcode <= 0x0D) || opcode == 0x23 || (opcode >= 0x2C && opcode <= 0x35)) 
#define NEEDS_ONE_OPERAND(opcode)       ((opcode >= 0x07 && opcode <= 0x09) || opcode == 0x11 || (opcode >= 0x19 && opcode <= 0x22) || (opcode >= 0x24 && opcode <= 0x2B))
#define NEEDS_TWO_OPERANDS(opcode)      ((opcode >= 0x01 && opcode <= 0x06) || (opcode >= 0x0E && opcode <= 0x10) || (opcode >= 0x12 && opcode <= 0x18))
#define IS_A_JMP_OPERAND(opcode)        ((opcode >= 0x21 && opcode <= 0x24) || opcode == 0x2B)

#define STRING_FROM_HEX(value)          (value == 10 ? "A" : (value == 11 ? "B" : (value == 12 ? "C" : (value == 13 ? "D" : (value == 14 ? "E" : (value == 15 ? "F" : to_string (value)))))))

// Data types used in 1001_x8
using byte = uint8_t;
using word = unsigned short;
using u32 = unsigned int;

