#include <string>

#include "1001_x8.h"

using word = unsigned short;

using namespace std;

#define IS_OP1_VAR_OPCODE(opcode)       (opcode == 0x04 || opcode == 0x06 || opcode == 0x09 || opcode == 0x1A || opcode == 0x1C || opcode == 0x1F || opcode == 0x20)
#define IS_OP2_VAR_OPCODE(opcode)       (opcode == 0x01 || opcode == 0x06 || opcode == 0x17)
#define IS_JUMP_OPCODE(opcode)          (opcode == 0x21 || opcode == 0x22 || (opcode >= 0x24 && opcode <= 0x2B))
#define IS_HALT_OPCODE(opcode)          (opcode == 0x35)
#define IS_LABEL_ENCODING(opcode)       (opcode == 0x36)

#define IS_NUMBER(value)                (value == '0' || value == '1' || value == '2' || value == '3' || value == '4' || value == '5' || value == '6' || value == '7' || value == '8' || value == '9')
#define INT_REPRESENTATION_OF_HEX(hex_value)    (IS_NUMBER (hex_value) ? ((u32) hex_value) - 48 : (hex_value == 'A' ? 10 : (hex_value == 'B' ? 11 : (hex_value == 'C' ? 12 : (hex_value == 'D' ? 13 : (hex_value == 'E' ? 14 : (hex_value == 'F' ? 15 : 0)))))))
#define STRING_FROM_HEX(value)      (value == 10 ? "A" : (value == 11 ? "B" : (value == 12 ? "C" : (value == 13 ? "D" : (value == 14 ? "E" : (value == 15 ? "F" : "0"))))))      


#define REG_AS_OPERAND1(opcode)         ((opcode >= 0x01 && opcode <= 0x03) || opcode == 0x05 || opcode == 0x07 || opcode == 0x08 || (opcode >= 0x0E && opcode <= 0x19) || opcode == 0x1B || opcode == 0x1D || opcode == 0x1E)
#define MEM_AS_OPERAND1(opcode)         (opcode == 0x04 || opcode == 0x06 || opcode == 0x09 || opcode == 0x1A || opcode == 0x1C || opcode == 0x1F || opcode == 0x20)
#define LAB_AS_OPERAND1(opcode)         (opcode == 0x21 || opcode == 0x22 || (opcode <= 0x24 && opcode >= 0x2B))

#define REG_AS_OPERAND2(opcode)         (opcode == 0x03 || opcode == 0x05 || opcode == 0x0E || opcode == 0x0F || opcode == 0x10 || (opcode >= 0x12 && opcode <= 0x16))
#define MEM_AS_OPERAND2(opcode)         (opcode == 0x01 || opcode == 0x17)
#define IMM_AS_OPERAND2(opcode)         (opcode == 0x02 || opcode == 0x18) 

word Load_Program (string file_path, cpu_t& cpu, mem_t& mem);

word stoh (string value);
string htos (byte value);
