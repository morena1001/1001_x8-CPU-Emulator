#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

#include <conio.h>

// #include "1001_x8.h"
#include "../ISRs/isr.h"

void Load_OS (byte data[mem_t::MAX_MEM], word& address);
void Load_PL (byte data[mem_t::MAX_MEM], word& address);

using namespace std;

/*
 * MEM STRUCT FUNCTIONS
 */

void MEM::Init () {
    for (u32 i = 0; i < MAX_MEM; i++)       data[i] = 0;

    // Load character bitmaps
    for (u32 i = 0xDE1D, k = 0; i < 0xE000; k++) {
        for (byte j = 0; j < 7; j++, i++) {
            data[i] = char_bitmaps[k][j];
        }
    }

    // Load bootloader
    data[0xFFFD] = 0x21;

    // ADDRESS FOR REGULAR PROGRAM
    // data[0xFFFE] = 0x00;
    // data[0xFFFF] = 0x01;

    // ADDRESS FOR OS PROGRAM
    data[0xFFFE] = 0x17;
    data[0xFFFF] = 0xE0;

    // Load OS
    word address = 0xE000;
    Load_OS (data, address);

    // Load Program Loader
    Load_PL (data, address);
}

void MEM::WriteByte (byte value, u32 address) {
    data[address] =  value;
}

void MEM::WriteWord (word value, u32 address) {
    WriteByte (value & 0xFF, address);
    WriteByte (value >> 8, address + 1);
}

/*
 * CPU STRUCT FUNCTIONS
 */

void CPU::Reset (mem_t& memory) {
    PC = 0xFFFD;
    SP = 0x0000;

    CF = ZF = OF = SF = 0;
    GPR[A] = GPR[B] = GPR[C] = GPR[D] = GPR[E] = GPR[F] = GPR[G] = GPR[H] = 0;

    memory.Init ();
}

byte CPU::FetchByte (mem_t& memory) {
    return memory[PC++];
}

word CPU::FetchWord (mem_t& memory) {
    word data = (word) (FetchByte (memory)) | ((word) (FetchByte (memory)) << 8);
    return data;
}

byte CPU::ReadByte (u32 address, mem_t& memory) {
    return memory[address];
}

word CPU::ReadWord (u32 address, mem_t& memory) {
    word data = (word) (ReadByte (address, memory)) | ((word) (ReadByte (address + 1, memory)) << 8);
}

void CPU::PushStack (byte value, mem_t& memory) {
    memory.WriteByte (value, SP);
    SP++;
}

byte CPU::PullStack (mem_t& memory) {
    byte data = ReadByte (SP - 1, memory);
    memory.WriteByte (0x00, SP - 1);
    SP--;
    return data;
}

string from_hex (int value) {
    switch (value) {
        case 10 :
            return "A";

        case 11 :
            return "B";

        case 12 :
            return "C";

        case 13 :
            return "D";

        case 14 :
            return "E";

        case 15 :
            return "F";

        default :
            return to_string (value);
    }

    return "";
}

string huh (byte value) {
    return from_hex ((int) value / 16) + from_hex ((int) value % 16);
}

void CPU::Execute (mem_t& memory, aux_mem_t& aux_mem) {
    byte ins = FetchByte (memory);
    char ch;
    word skip_to = 0xEFC6;//0x0101; //0xE822;//0xE80A;//0xE550;//0xE65B; //0xE80A;  //0xE889;//0xE7E5;  //0xE7DC;//0xE7D4;//0xE708;     //0xE55D;//0xE6B3;//0xE4FB;//0xE2C0;//0xE307;    //0xE4F8;//0xE304;//0xE2BB;    
    bool skipped = false;
    bool jump = false;

    while (ins != INS_HALT) {
        if (!skipped && PC == skip_to && PC < 0xFFFD)      skipped = true;
        // if (!skipped && PC > 0xE523 && ins == 0x18 && ReadByte (PC, memory) == 0x01 && ReadByte (PC + 1, memory) == 0x09) skipped = true;
        if (skipped) {//&& jump ) {//&& PC >= 0xE089 && PC <= 0xFFF0) {     
            cout << "VARIABLES" << endl;
            for (int i = 0xFF55; i < 0xFF85; i++) {
                switch (i) {
                    case 0xFF55 : cout << "aux_offset_L        : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF56 : cout << "aux_offset_H        : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF57 : cout << "low_byte            : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF58 : cout << "high_byte           : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF59 : cout << "high_next           : " << huh (ReadByte (i, memory)) << endl << endl; break;

                    case 0xFF5A : cout << "instruction         : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF5B : cout << "opcode              : " << huh (ReadByte (i, memory)) << endl << endl; break;

                    case 0xFF5C : cout << "p_stack_offset      : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF5D : cout << "end_addr_L          : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF5E : cout << "end_addr_H          : " << huh (ReadByte (i, memory)) << endl << endl; break;

                    case 0xFF5F : cout << "pc_set              : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF60 : cout << "pc_start_addr_L     : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF61 : cout << "pc_start_addr_H     : " << huh (ReadByte (i, memory)) << endl << endl; break;
                
                    case 0xFF62 : cout << "RAM_offset_L        : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF63 : cout << "RAM_offset_H        : " << huh (ReadByte (i, memory)) << endl << endl; break;

                    case 0xFF64 : cout << "var_offset_L        : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF65 : cout << "var_offset_H        : " << huh (ReadByte (i, memory)) << endl << endl; break;


                    case 0xFF66 : cout << "l_offset_L          : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF67 : cout << "l_offset_H          : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF68 : cout << "unL_offset_L        : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF69 : cout << "unL_offset_H        : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF6A : cout << "s_offset_L          : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF6B : cout << "s_offset_H          : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF6C : cout << "unS_offset_L        : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF6D : cout << "unS_offset_H        : " << huh (ReadByte (i, memory)) << endl << endl; break;

                    case 0xFF6E : cout << "l_size_L            : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF6F : cout << "l_size_H            : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF70 : cout << "unL_size_L          : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF71 : cout << "unL_size_H          : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF72 : cout << "s_size_L            : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF73 : cout << "s_size_H            : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF74 : cout << "unS_size_L          : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF75 : cout << "unS_size_H          : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF76 : cout << "var_size_L          : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF77 : cout << "var_size_H          : " << huh (ReadByte (i, memory)) << endl << endl; break;


                    case 0xFF78 : cout << "l_start_addr_L      : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF79 : cout << "l_start_addr_H      : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF7A : cout << "unL_start_addr_L    : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF7B : cout << "unL_start_addr_H    : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF7C : cout << "s_start_addr_L      : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF7D : cout << "s_start_addr_H      : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF7E : cout << "unS_start_addr_L    : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF7F : cout << "unS_start_addr_H    : " << huh (ReadByte (i, memory)) << endl << endl; break;

                    case 0xFF80 : cout << "i                   : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF81 : cout << "j                   : " << huh (ReadByte (i, memory)) << endl << endl; break;

                    case 0xFF82 : cout << "p_stack_counter     : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF83 : cout << "aux_mem_addr_L      : " << huh (ReadByte (i, memory)) << endl; break;
                    case 0xFF84 : cout << "aux_mem_addr_H      : " << huh (ReadByte (i, memory)) << endl << endl; break;

                    default : cout << huh (i >> 8) << huh (i & 0xFF) << "                : " << huh (ReadByte (i, memory)) << endl; break;
                }
            }

            // cout << endl << "AUX MEM MAPPED IO" << endl;
            // cout << "0xDA58 SL : " << huh (ReadByte (0xDA58, memory)) << endl;
            // cout << "0xDA59 SH : " << huh (ReadByte (0xDA59, memory)) << endl;
            // cout << "0xDA5A SR : " << huh (ReadByte (0xDA5A, memory)) << endl;
            // cout << "0xDA5B GL : " << huh (ReadByte (0xDA5B, memory)) << endl;
            // cout << "0xDA5C GH : " << huh (ReadByte (0xDA5C, memory)) << endl;

            cout << endl << "FINAL PROGRAM" << endl;
            for (byte i = 0; i < 12; i++)       cout << huh (ReadByte (0x0100 + i, memory)) << " ";
            cout << huh (ReadByte (0xD858, memory));
            cout << endl;

            // cout << endl << "P STACK ADDRESSES" << endl;
            // for (byte i = 0; i < 16; i++)       cout << huh (ReadByte (0xFF85 + i, memory)) << " ";
            // cout << endl;

            cout << endl << "REGISTERS" << endl;
            cout << "A : " << huh (GPR[A]) << endl;
            cout << "B : " << huh (GPR[B]) << endl;
            cout << "C : " << huh (GPR[C]) << endl;
            cout << "D : " << huh (GPR[D]) << endl;
            cout << "E : " << huh (GPR[E]) << endl;
            cout << "F : " << huh (GPR[F]) << endl;
            cout << "G : " << huh (GPR[G]) << endl;
            cout << "H : " << huh (GPR[H]) << endl;

            cout << endl << "FLAGS" << endl;
            cout << "Carry    : " << huh (CF) << endl;
            cout << "Overflow : " << huh (OF) << endl;
            cout << "Zero     : " << huh (ZF) << endl;
            cout << "Sign     : " << huh (SF) << endl;

            cout << endl << "INSTRUCTIONS" << endl;
            if (NEEDS_1_BYTE (ins))             cout << huh (ins) << endl; 
            else if (NEEDS_2_BYTES (ins))       cout << huh (ins) << " " << huh (ReadByte (PC, memory)) << endl; 
            else if (NEEDS_3_BYTES (ins))       cout << huh (ins) << " " << huh (ReadByte (PC, memory)) << " " << huh (ReadByte (PC + 1, memory)) << endl; 
            else if (NEEDS_4_BYTES (ins))       cout << huh (ins) << " " << huh (ReadByte (PC, memory)) << " " << huh (ReadByte (PC + 1, memory)) << " " << huh (ReadByte (PC + 2, memory)) << endl;
            else if (NEEDS_5_BYTES (ins))       cout << huh (ins) << " " << huh (ReadByte (PC, memory)) << " " << huh (ReadByte (PC + 1, memory)) << " " << huh (ReadByte (PC + 2, memory)) << " " << huh (ReadByte (PC + 3, memory)) << endl;
            
            cout << endl << "PROGRAM COUNTER" << endl;
            cout << huh (PC >> 8) << huh (PC & 0xFF) << endl << endl;

            cout << "Press 'q' to quit, 'r' to run program to end, or 'enter' to move to next instruction... ";
            while (true) {
                if (kbhit ()) {
                    ch = getch ();
                    if (ch == 'q')      return;
                    else if (ch == 'r') {
                        skipped = false;
                        jump = false;
                        skip_to = 0xFFFF;
                        break;
                    }
                    else                break;
                }
            }
            cout << endl << endl << endl << endl;
        }        

        if (ReadByte (0xDA5A, memory) == 1) {
            word aux_address = ((word) ReadByte (0xDA58, memory)) | ((word) ReadByte (0xDA59, memory) << 8);
            memory.WriteWord (aux_mem[aux_address], 0xDA5B);
            memory.WriteByte (0, 0xDA5A);
            // printf ("YEAH               %d %d %d %d\n", ReadByte (0xDA58, memory), ReadByte (0xDA59, memory), ReadByte (0xDA5B, memory), ReadByte (0xDA5C, memory));
        }

        // cout << huh (ins) << " : " << huh (PC >> 8) << huh (PC & 0xFF) << "         " << huh (ReadByte (PC, memory)) << " " << huh (ReadByte (PC + 1, memory)) << " " << huh (ReadByte (PC + 2, memory)) << " " << huh (ReadByte (PC + 3, memory)) << endl;

        switch (ins) {
            case INS_LDFM: {
                byte reg = FetchByte (memory);
                word address = FetchWord (memory);
                GPR[reg] = ReadByte (address, memory); 

                Set_SF (reg);
                Set_ZF (reg);
            } break;

            case INS_LDFI: {
                byte reg = FetchByte (memory);
                byte imm = FetchByte (memory);
                GPR[reg] = imm;

                Set_SF (reg);
                Set_ZF (reg);
            } break;

            case INS_LDFR: {
                byte reg1 = FetchByte (memory);
                byte reg2 = FetchByte (memory);
                GPR[reg1] = GPR[reg2];
            } break;

            case INS_STOR: {
                word address = FetchWord (memory);
                byte reg = FetchByte (memory);
                memory.WriteByte (GPR[reg], address);

                // if (address == 0xDA59)      memory.WriteByte (1, 0xDA5A);
            } break;

            case INS_SWPR: {
                byte reg1 = FetchByte (memory);
                byte reg2 = FetchByte (memory);

                GPR[H] = GPR[reg1];
                GPR[reg1] = GPR[reg2];
                GPR[reg2] = GPR[H];
            } break;

            case INS_SWPM: {
                word address1 = FetchWord (memory);
                word address2 = FetchWord (memory);
                
                GPR[G] = ReadByte (address1, memory);
                GPR[H] = ReadByte (address2, memory);
                memory.WriteByte (GPR[H], address1);
                memory.WriteByte (GPR[G], address2);
            } break;

            case INS_PULR: {
                byte reg = FetchByte (memory);
                GPR[reg] = PullStack (memory);
    
                Set_SF (reg);
                Set_ZF (reg);
            } break;

            case INS_PSHR: {
                byte reg = FetchByte (memory);
                PushStack (GPR[reg], memory);
            } break;

            case INS_PSHM: {
                word address = FetchWord (memory);
                PushStack (ReadByte (address, memory), memory);
            } break;

            case INS_PULC: {
                PC = (word) (PullStack (memory)) | ((word) (PullStack (memory)) << 8);
            } break;

            case INS_PSHC: {
                PushStack (PC & 0xFF, memory);
                PushStack (PC >> 8, memory);
            } break;

            case INS_PULF: {
                byte flags = PullStack (memory);

                CF = flags & 1;
                ZF = (flags >> 1) & 1;
                OF = (flags >> 2) & 2;
                SF = (flags >> 3) & 2;
            } break;

            case INS_PSHF: {
                byte flags = CF | (ZF << 1) | (OF << 2) | (SF << 3);
                PushStack (flags, memory);
            }

            case INS_AND: {
                byte reg1 = FetchByte (memory);
                byte reg2 = FetchByte (memory);

                GPR[reg1] &= GPR[reg2];
                
                Set_SF (reg1);
                Set_ZF (reg1);
            } break;

            case INS_EOR: {
                byte reg1 = FetchByte (memory);
                byte reg2 = FetchByte (memory);

                GPR[reg1] ^= GPR[reg2];
                
                Set_SF (reg1);
                Set_ZF (reg1);
            } break;

            case INS_ORA: {
                byte reg1 = FetchByte (memory);
                byte reg2 = FetchByte (memory);

                GPR[reg1] |= GPR[reg2];
                
                Set_SF (reg1);
                Set_ZF (reg1);
            } break;

            case INS_NOT: {
                byte reg = FetchByte (memory);

                GPR[reg] = ~GPR[reg];

                Set_SF (reg);
                Set_ZF (reg);
            } break;

            case INS_ADD: {
                byte reg1 = FetchByte (memory);
                byte reg2 = FetchByte (memory);

                byte of_part1 = !((GPR[reg1] >> 7) ^ (GPR[reg2] >> 7));

                word sum = GPR[reg1] + GPR[reg2] + CF;
                GPR[reg1] = (sum & 0xFF);
                

                byte of_part2 = (GPR[reg1] >> 7) ^ (GPR[reg2] >> 7);

                Set_CF (sum);
                Set_SF (reg1);
                Set_ZF (reg1);
                Set_OF (of_part1 & of_part2);
            } break;

            case INS_SUB: {
                byte reg1 = FetchByte (memory);
                byte reg2 = FetchByte (memory);

                byte of_part1 = !((GPR[reg1] >> 7) ^ ((~GPR[reg2]) >> 7));

                word diff = GPR[reg1] + (~GPR[reg2]) + (~CF);
                GPR[reg1] = (diff & 0xFF);

                byte of_part2 = (GPR[reg1] >> 7) ^ ((~GPR[reg2]) >> 7);

                Set_CF (diff);
                Set_SF (reg1);
                Set_ZF (reg1);
                Set_OF (of_part1 & of_part2);
            } break;

            case INS_MULT: {
                byte reg1 = FetchByte (memory);
                byte reg2 = FetchByte (memory);

                byte of_part1 = !((GPR[reg1] >> 7) ^ ((~GPR[reg2]) >> 7));

                word mult = reg1 * reg2;
                GPR[reg1] = (mult & 0xFF);

                byte of_part2 = (GPR[reg1] >> 7) ^ ((~GPR[reg2]) >> 7);

                Set_CF (mult);
                Set_SF (reg1);
                Set_ZF (reg1);
                Set_OF (of_part1 & of_part2);
            } break;

            case INS_DIV: {
                byte reg1 = FetchByte (memory);
                byte reg2 = FetchByte (memory);

                byte of_part1 = !((GPR[reg1] >> 7) ^ ((~GPR[reg2]) >> 7));

                word div = reg1 / reg2;
                GPR[reg1] = (div & 0xFF);

                byte of_part2 = (GPR[reg1] >> 7) ^ ((~GPR[reg2]) >> 7);

                Set_CF (div);
                Set_SF (reg1);
                Set_ZF (reg1);
                Set_OF (of_part1 & of_part2);
            } break;

            case INS_CMPR: {
                byte reg1 = FetchByte (memory);
                byte reg2 = FetchByte (memory);

                CF = GPR[reg1] >= GPR[reg2];
                ZF = GPR[reg1] == GPR[reg2];

                Set_SF (reg1);
            } break;

            case INS_CMPM: {
                byte reg = FetchByte (memory);
                word address = FetchWord (memory);
                byte value = ReadByte (address, memory);

                CF = GPR[reg] >= value;
                ZF = GPR[reg] == value;

                Set_SF (reg);                
            } break;

            case INS_CMPI: {
                byte reg = FetchByte (memory);
                byte imm = FetchByte (memory);

                CF = GPR[reg] >= imm;
                ZF = GPR[reg] == imm;

                Set_SF (reg);
            } break;

            case INS_INCR: {
                byte reg = FetchByte (memory);

                GPR[reg] += 1;

                Set_SF (reg);
                Set_ZF (reg);
            } break;

            case INS_INCM: {
                word address = FetchWord (memory);
                byte value = ReadByte (address, memory);

                memory.WriteByte (value + 1, address);
            } break;

            case INS_DECR: {
                byte reg = FetchByte (memory);
                GPR[reg] -= 1;

                Set_SF (reg);
                Set_ZF (reg);
            } break;

            case INS_DECM: {
                word address = FetchWord (memory);
                byte value = ReadByte (address, memory);

                memory.WriteByte (value - 1, address);
            } break;

            case INS_SHLR: {
                byte reg = FetchByte (memory);

                CF = (GPR[reg] >> 7);

                GPR[reg] *= 2;

                Set_SF (reg);
                Set_ZF (reg);
            } break;

            case INS_SHRR: {
                byte reg = FetchByte (memory);

                CF = (GPR[reg] & 0x01);

                GPR[reg] /= 2;

                Set_SF (reg);
                Set_ZF (reg);
            } break;

            case INS_SHLM: {
                word address = FetchWord (memory);
                byte value = ReadByte (address, memory);

                memory.WriteByte (value * 2, address);
            } break;

            case INS_SHRM: {
                word address = FetchWord (memory);
                byte value = ReadByte (address, memory);

                memory.WriteByte (value / 2, address);
            } break;

            case INS_JMP: {
                word address = FetchWord (memory);
                PC = address;
            } break;

            case INS_JSR: {
                PushStack ((PC + 2) & 0xFF, memory);
                PushStack ((PC + 2) >> 8, memory);

                word address = FetchWord (memory);
                PC = address;
            } break;

            case INS_RSR: {
                word msb = (PullStack (memory) << 8);
                word lsb = PullStack (memory);
                PC = msb | lsb;
            } break;

            case INS_JSC: {
                if (!SF) {
                    word address = FetchWord (memory);
                    PC = address;
                } else {
                    PC +=2;
                }
            } break;

            case INS_JSS: {
                if (SF) {
                    word address = FetchWord (memory);
                    PC = address;
                } else {
                    PC +=2;
                }
            } break; 

            case INS_JCC: {
                if (!CF) {
                    word address = FetchWord (memory);
                    PC = address;
                } else {
                    PC +=2;
                }
            } break;

            case INS_JCS: {
                if (CF) {
                    word address = FetchWord (memory);
                    PC = address;
                } else {
                    PC +=2;
                }
            } break;

            case INS_JOC: {
                if (!OF) {
                    word address = FetchWord (memory);
                    PC = address;
                } else {
                    PC +=2;
                }
            } break;

            case INS_JOS: {
                if (OF) {
                    word address = FetchWord (memory);
                    PC = address;
                } else {
                    PC +=2;
                }
            } break;

            case INS_JZC: {
                if (!ZF) {
                    word address = FetchWord (memory);
                    PC = address;
                } else {
                    PC +=2;
                }
            } break;

            case INS_JZS: {
                if (ZF) {
                    word address = FetchWord (memory);
                    PC = address;
                } else {
                    PC +=2;
                }
            } break;

            case INS_CCF: {
                CF = 0;
            } break;

            case INS_SCF: {
                CF = 1;
            } break;

            case INS_CSF: {
                SF = 0;
            } break;

            case INS_SSF: {
                SF = 1;
            } break;

            case INS_COF: {
                OF = 0;
            } break;

            case INS_SOF: {
                OF = 1;
            } break;

            case INS_CZF: {
                ZF = 0;
            } break;

            case INS_SZF: {
                ZF = 1;
            } break;

            case INS_NOP: {} break;

            case INS_HALT: {
                return;
            } break;

            case INS_LDFO: {
                byte reg = FetchByte (memory);
                word address = FetchWord (memory);
                GPR[reg] = ReadByte (address + ((word) (GPR[E]) | (word) (GPR[F] << 8)), memory);         

                Set_SF (reg);
                Set_ZF (reg);
            } break;

            case INS_STOO: {
                word address = FetchWord (memory);
                byte reg = FetchByte (memory);
                memory.WriteByte (GPR[reg], address + ((word) (GPR[E]) | (word) (GPR[F] << 8)));
            } break;

            case INS_SWPO: {
                word address1 = FetchWord (memory);
                word address2 = FetchWord (memory);
                
                GPR[G] = ReadByte (address1 + ((word) (GPR[E]) | (word) (GPR[F] << 8)), memory);
                GPR[H] = ReadByte (address2 + ((word) (GPR[C]) | (word) (GPR[D] << 8)), memory);
                memory.WriteByte (GPR[H], address1 + ((word) (GPR[E]) | (word) (GPR[F] << 8)));
                memory.WriteByte (GPR[G], address2 + ((word) (GPR[C]) | (word) (GPR[D] << 8)));
            } break;

            case INS_PSHO: {
                word address = FetchWord (memory);
                PushStack (ReadByte (address + ((word) (GPR[E]) | (word) (GPR[F] << 8)), memory), memory);
            } break;

            case INS_CMPO: {
                byte reg = FetchByte (memory);
                word address = FetchWord (memory);
                byte value = ReadByte (address + ((word) (GPR[E]) | (word) (GPR[F] << 8)), memory);

                CF = GPR[reg] >= value;
                ZF = GPR[reg] == value;

                Set_SF (reg);
            } break;

            case INS_INCO: {
                word address = FetchWord (memory);
                byte value = ReadByte (address + ((word) (GPR[E]) | (word) (GPR[F] << 8)), memory);

                memory.WriteByte (value + 1, address + ((word) (GPR[E]) | (word) (GPR[F] << 8)));
            } break;

            case INS_DECO: {
                word address = FetchWord (memory);
                byte value = ReadByte (address + ((word) (GPR[E]) | (word) (GPR[F] << 8)), memory);

                memory.WriteByte (value - 1, address + ((word) (GPR[E]) | (word) (GPR[F] << 8)));
            } break;

            case INS_SHLO: {
                word address = FetchWord (memory);
                byte value = ReadByte (address + ((word) (GPR[E]) | (word) (GPR[F] << 8)), memory);

                memory.WriteByte (value * 2, address + ((word) (GPR[E]) | (word) (GPR[F] << 8)));
            } break;

            case INS_SHRO: {
                word address = FetchWord (memory);
                byte value = ReadByte (address + ((word) (GPR[E]) | (word) (GPR[F] << 8)), memory);

                memory.WriteByte (value / 2, address + ((word) (GPR[E]) | (word) (GPR[F] << 8)));
            } break;

            case INS_COPM: {
                word address1 = FetchWord (memory);
                word address2 = FetchWord (memory);

                GPR[H] = ReadByte (address2, memory);
                memory.WriteByte (GPR[H], address1);

                // if (address1 == 0xDA59)      memory.WriteByte (1, 0xDA5A);
            } break;

            case INS_COPO: {
                word address1 = FetchWord (memory);
                word address2 = FetchWord (memory);

                GPR[H] = ReadByte (address2 + ((word) (GPR[C]) | (word) (GPR[D] << 8)), memory);
                memory.WriteByte (GPR[H], address1 + ((word) (GPR[E]) | (word) (GPR[F] << 8)));

                // if (address1 == 0xDA59)      memory.WriteByte (1, 0xDA5A);
            } break;

            case INS_STRI: {
                word address = FetchWord (memory);
                byte imm = FetchByte (memory);

                memory.WriteByte (imm, address);
                // if (address == 0xDA59)       memory.WriteByte (1, 0xDA5A);
            } break;

            case INS_STRO: {
                word address = FetchWord (memory);
                byte imm = FetchByte (memory);

                memory.WriteByte (imm, address + ((word) (GPR[E]) | (word) (GPR[F] << 8)));
                // if (address == 0xDA59)      memory.WriteByte (1, 0xDA5A);
            } break;

            case INS_JMPO: {
                word address = FetchWord (memory);
                PC = address + ((word) (GPR[E]) | (word) (GPR[F] << 8));

                // system ("pause");
            } break;

            case INS_SYS: {
                byte imm = FetchByte (memory);

                switch (imm) {
                    case 0x02: {
                        byte type  = GPR[C];
                        byte value = GPR[F];

                        if (type == 0 || type == 2) {
                            cout << "OUTPUT :" << endl << huh (GPR[F]) << endl << endl;
                        } else if (type == 1) {
                            cout << "OUTPUT :" << endl << endl;
                        } else {
                            cout << "ERROR  :" << endl << "Type " << huh (type) << " unsupported." << endl << endl;
                        }
                    } break;
                }
            } break;

            default : {
                printf ("Instruction 0x%X not handled\r\n", ins);
                cout << huh (PC >> 8) << huh (PC & 0xFF) << endl;
                cout << huh (ins) << " " << huh (ReadByte (PC, memory)) << " " << huh (ReadByte (PC + 1, memory)) << " " << huh (ReadByte (PC + 2, memory)) << " " << huh (ReadByte (PC + 3, memory)) << endl;
                cout << huh (ins) << " " << huh (ReadByte (PC - 4, memory)) << " " << huh (ReadByte (PC - 3, memory)) << " " << huh (ReadByte (PC - 2, memory)) << " " << huh (ReadByte (PC - 1, memory)) << endl;
                // printf ("%d %d %d    %d\n", memory[PC - 1], memory[PC], memory[PC + 1], PC);
                return;
            } break;
        }

        ins = FetchByte (memory);
    }

    // cout << "VARIABLES" << endl;
    //         for (int i = 0xFF55; i < 0xFF85; i++) {
    //             switch (i) {
    //                 case 0xFF55 : cout << "aux_offset_L        : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF56 : cout << "aux_offset_H        : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF57 : cout << "low_byte            : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF58 : cout << "high_byte           : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF59 : cout << "high_next           : " << huh (ReadByte (i, memory)) << endl << endl; break;

    //                 case 0xFF5A : cout << "instruction         : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF5B : cout << "opcode              : " << huh (ReadByte (i, memory)) << endl << endl; break;

    //                 case 0xFF5C : cout << "p_stack_offset      : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF5D : cout << "end_addr_L          : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF5E : cout << "end_addr_H          : " << huh (ReadByte (i, memory)) << endl << endl; break;

    //                 case 0xFF5F : cout << "pc_set              : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF60 : cout << "pc_start_addr_L     : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF61 : cout << "pc_start_addr_H     : " << huh (ReadByte (i, memory)) << endl << endl; break;
                
    //                 case 0xFF62 : cout << "RAM_offset_L        : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF63 : cout << "RAM_offset_H        : " << huh (ReadByte (i, memory)) << endl << endl; break;

    //                 case 0xFF64 : cout << "var_offset_L        : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF65 : cout << "var_offset_H        : " << huh (ReadByte (i, memory)) << endl << endl; break;


    //                 case 0xFF66 : cout << "l_offset_L          : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF67 : cout << "l_offset_H          : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF68 : cout << "unL_offset_L        : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF69 : cout << "unL_offset_H        : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF6A : cout << "s_offset_L          : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF6B : cout << "s_offset_H          : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF6C : cout << "unS_offset_L        : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF6D : cout << "unS_offset_H        : " << huh (ReadByte (i, memory)) << endl << endl; break;

    //                 case 0xFF6E : cout << "l_size_L            : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF6F : cout << "l_size_H            : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF70 : cout << "unL_size_L          : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF71 : cout << "unL_size_H          : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF72 : cout << "s_size_L            : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF73 : cout << "s_size_H            : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF74 : cout << "unS_size_L          : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF75 : cout << "unS_size_H          : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF76 : cout << "var_size_L          : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF77 : cout << "var_size_H          : " << huh (ReadByte (i, memory)) << endl << endl; break;


    //                 case 0xFF78 : cout << "l_start_addr_L      : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF79 : cout << "l_start_addr_H      : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF7A : cout << "unL_start_addr_L    : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF7B : cout << "unL_start_addr_H    : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF7C : cout << "s_start_addr_L      : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF7D : cout << "s_start_addr_H      : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF7E : cout << "unS_start_addr_L    : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF7F : cout << "unS_start_addr_H    : " << huh (ReadByte (i, memory)) << endl << endl; break;

    //                 case 0xFF80 : cout << "i                   : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF81 : cout << "j                   : " << huh (ReadByte (i, memory)) << endl << endl; break;

    //                 case 0xFF82 : cout << "p_stack_counter     : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF83 : cout << "aux_mem_addr_L      : " << huh (ReadByte (i, memory)) << endl; break;
    //                 case 0xFF84 : cout << "aux_mem_addr_H      : " << huh (ReadByte (i, memory)) << endl << endl; break;

    //                 default : cout << huh (i >> 8) << huh (i & 0xFF) << "                : " << huh (ReadByte (i, memory)) << endl; break;
    //             }
    //         }
}

void CPU::Set_CF (word value) {
    CF = value > 0xFF;
}

void CPU::Set_ZF (byte reg) {
    if (GPR[reg] == 0) {
        ZF = 1;
    } else {
        ZF = 0;
    }
}

void CPU::Set_OF (byte comparison_value) {
    OF = comparison_value;
}

void CPU::Set_SF (byte reg) {
    if ((GPR[reg] >> 7) == 1) {
        SF = 1;
    } else {
        SF = 0;
    }
}



void Load_OS (byte data[mem_t::MAX_MEM], word& address) {
    ifstream file;

    file.open ("os/final os program.txt");

    if (file.is_open ()) {
        byte instruction;
        string line;

        getline (file, line);

        while (line.length () != 0) {
            instruction = (INT_REPRESENTATION_OF_HEX (line[0]) * 16) + (INT_REPRESENTATION_OF_HEX (line[1]));
            data[address++] = instruction;
            line.erase (0, 3);
        }
    }
}

void Load_PL (byte data[mem_t::MAX_MEM], word& address) {
    ifstream file;

    file.open ("aux_mem_loader/final loader program.txt");

    if (file.is_open ()) {
        byte instruction;
        string line;

        getline (file, line);

        while (line.length () != 0) {
            instruction = (INT_REPRESENTATION_OF_HEX (line[0]) * 16) + (INT_REPRESENTATION_OF_HEX (line[1]));
            data[address++] = instruction;
            line.erase (0, 3);
        }
    }
}
