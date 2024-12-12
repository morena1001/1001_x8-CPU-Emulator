#include <string>

// #include "../cpu/1001_x8.h"
#include "../loader/loader.h"
#include "../aux_mem/aux_mem.h"

using namespace std;

#define IS_OP1_VAR_OPCODE(opcode)       (opcode == 0x04 || opcode == 0x06 || opcode == 0x09 || opcode == 0x1A || opcode == 0x1C || opcode == 0x1F || opcode == 0x20)
#define IS_OP2_VAR_OPCODE(opcode)       (opcode == 0x01 || opcode == 0x06 || opcode == 0x17)
#define IS_JUMP_OPCODE(opcode)          (opcode == 0x21 || opcode == 0x22 || (opcode >= 0x24 && opcode <= 0x2B))
#define IS_HALT_OPCODE(opcode)          (opcode == 0x35)
#define IS_LABEL_ENCODING(opcode)       (opcode == 0x36)
#define IS_START_ENCODING(opcode)       (opcode == 0x37)
#define IS_SUBR_ENCODING(opcode)        (opcode == 0x41)
#define IS_ADDR_ENCODING(opcode)        (opcode == 0x42)

#define IS_NUMBER(value)                (value == '0' || value == '1' || value == '2' || value == '3' || value == '4' || value == '5' || value == '6' || value == '7' || value == '8' || value == '9')
#define INT_REPRESENTATION_OF_HEX(hex_value)    (IS_NUMBER (hex_value) ? ((u32) hex_value) - 48 : (hex_value == 'A' ? 10 : (hex_value == 'B' ? 11 : (hex_value == 'C' ? 12 : (hex_value == 'D' ? 13 : (hex_value == 'E' ? 14 : (hex_value == 'F' ? 15 : 0)))))))
#define STRING_FROM_HEX(value)          (value == 10 ? "A" : (value == 11 ? "B" : (value == 12 ? "C" : (value == 13 ? "D" : (value == 14 ? "E" : (value == 15 ? "F" : to_string (value)))))))      


#define REG_AS_OPERAND1(opcode)         ((opcode >= 0x01 && opcode <= 0x03) || opcode == 0x05 || opcode == 0x07 || opcode == 0x08 || (opcode >= 0x0E && opcode <= 0x19) || opcode == 0x1B || opcode == 0x1D || opcode == 0x1E || opcode == 0x38 || opcode == 0x3C)
#define MEM_AS_OPERAND1(opcode)         (opcode == 0x04 || opcode == 0x06 || opcode == 0x09 || opcode == 0x1A || opcode == 0x1C || opcode == 0x1F || opcode == 0x20 || (opcode >= 0x39 && opcode <= 0x3B) || (opcode >= 0x3D && opcode <= 0x40))
#define LAB_AS_OPERAND1(opcode)         (opcode == 0x21 || (opcode >= 0x24 && opcode <= 0x2B))
#define SBR_AS_OPERAND1(opcode)         (opcode == 0x22)

#define REG_AS_OPERAND2(opcode)         (opcode == 0x03 || opcode == 0x04 || opcode == 0x05 || opcode == 0x0E || opcode == 0x0F || opcode == 0x10 || (opcode >= 0x12 && opcode <= 0x16) || opcode == 0x39)
#define MEM_AS_OPERAND2(opcode)         (opcode == 0x01 || opcode == 0x06 || opcode == 0x17 || opcode == 0x38 || opcode == 0x3A || opcode == 0x3C)
#define IMM_AS_OPERAND2(opcode)         (opcode == 0x02 || opcode == 0x18) 

typedef struct aux_loader {
    word mem_addr;
    byte low, high; 
    bool high_next;
} aux_loader_t;

void Load_Program_From_AuxMem (cpu_t& cpu, mem_t& mem, aux_mem_t& aux_mem, aux_loader_t& loader);
string htos (byte value);
