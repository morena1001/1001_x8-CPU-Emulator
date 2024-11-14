#include <string>

using word = unsigned short;

using namespace std;

#define IS_OP1_VAR_OPCODE(opcode)       (opcode == 0x04 || opcode == 0x06 || opcode == 0x09 || opcode == 0x1A || opcode == 0x1C || opcode == 0x1F || opcode == 0x20)
#define IS_OP2_VAR_OPCODE(opcode)       (opcode == 0x01 || opcode == 0x06 || opcode == 0x17)
#define IS_JUMP_OPCODE(opcode)          (opcode == 0x21 || opcode == 0x22 || (opcode >= 0x24 && opcode <= 0x2B))

word Load_Program (string file_path);
