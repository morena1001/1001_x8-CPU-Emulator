#include <stdint.h>

#define NEEDS_NO_OPERANDS(opcode)       ((opcode >= 0x0A && opcode <= 0x0D) || opcode == 0x23 || (opcode >= 0x2C && opcode <= 0x35) || opcode == 0x37) 
#define NEEDS_ONE_OPERAND(opcode)       ((opcode >= 0x07 && opcode <= 0x09) || opcode == 0x11 || (opcode >= 0x19 && opcode <= 0x22) || (opcode >= 0x24 && opcode <= 0x2B) || opcode == 0x3B || (opcode >= 0x3D && opcode <= 0x40))
#define NEEDS_TWO_OPERANDS(opcode)      ((opcode >= 0x01 && opcode <= 0x06) || (opcode >= 0x0E && opcode <= 0x10) || (opcode >= 0x12 && opcode <= 0x18) || (opcode >= 0x38 && opcode <= 0x3A) || opcode == 0x3C)
#define IS_A_JMP_OPERAND(opcode)        ((opcode >= 0x21 && opcode <= 0x22) || (opcode >= 0x24 && opcode <= 0x2B))

#define REG_AS_OPERAND1(opcode)         ((opcode >= 0x01 && opcode <= 0x03) || opcode == 0x05 || opcode == 0x07 || opcode == 0x08 || (opcode >= 0x0E && opcode <= 0x19) || opcode == 0x1B || opcode == 0x1D || opcode == 0x1E || opcode == 0x38 || opcode == 0x3C)
#define MEM_AS_OPERAND1(opcode)         (opcode == 0x04 || opcode == 0x06 || opcode == 0x09 || opcode == 0x1A || opcode == 0x1C || opcode == 0x1F || opcode == 0x20 || (opcode >= 0x39 && opcode <= 0x3B) || (opcode >= 0x3D && opcode <= 0x40))
#define LAB_AS_OPERAND1(opcode)         (opcode == 0x21 || opcode == 0x22 || (opcode >= 0x24 && opcode <= 0x2B))

#define REG_AS_OPERAND2(opcode)         (opcode == 0x03 || opcode == 0x04 || opcode == 0x05 || opcode == 0x0E || opcode == 0x0F || opcode == 0x10 || (opcode >= 0x12 && opcode <= 0x16) || opcode == 0x39)
#define MEM_AS_OPERAND2(opcode)         (opcode == 0x01 || opcode == 0x06 || opcode == 0x17 || opcode == 0x38 || opcode == 0x3A || opcode == 0x3C)
#define IMM_AS_OPERAND2(opcode)         (opcode == 0x02 || opcode == 0x18) 

#define LABEL_ENCODING                  0x36
#define START_ENCODING                  0x37

#define STRING_FROM_HEX(value)          (value == 10 ? "A" : (value == 11 ? "B" : (value == 12 ? "C" : (value == 13 ? "D" : (value == 14 ? "E" : (value == 15 ? "F" : to_string (value)))))))

// Data types used in 1001_x8
using byte = uint8_t;
using word = unsigned short;
using u32 = unsigned int;

