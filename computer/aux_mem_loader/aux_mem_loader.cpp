#include <iostream>
#include <string>
#include <map>
#include <fstream>

#include "aux_mem_loader.h"

using namespace std;

void Split_Bytes (aux_mem_t& aux_mem, word& mem_addr, byte& low, byte& high);
void Grab_Low (aux_mem_t& aux_mem, word& mem_addr, byte& low);
void Grab_High (aux_mem_t& aux_mem, word& mem_addr, byte& high);
void Grab_Word (aux_mem_t& aux_mem, word& mem_addr, byte& low, byte& high, bool& high_next);
void Grab_Byte (aux_mem_t& aux_mem, word& mem_addr, byte& low, byte& high, bool& high_next);

void Load_Program_From_AuxMem (cpu_t& cpu, mem_t& mem, aux_mem_t& aux_mem) {
    map<word, word> headers;
    multimap<word, word> unInit_headers;
    map<word, word> variables;

    map<word, word> subroutines;
    multimap<word, word> unInit_subroutines;

    word address = 0x0100;
    word var_address = 0xD85D;
    word p_stack_addr = 0xFF85;

    bool PC_set = false;
    bool end_of_program = false;
    bool high_next = false;

    word mem_addr = mem[p_stack_addr];
    word instruction, opcode;
    byte low, high;
    word var_count = aux_mem[2 + mem_addr++];

    for (int i = 0; i < mem[p_stack_addr + 1]; i++) {
        // Parse all variables
        if (var_count != 0) {
            // Grab 1 word for var_id
            Grab_Word (aux_mem, mem_addr, low, high, high_next);

            word var_id = (word) (low) & (word) (high << 8);
            variables[var_id] = var_address++;

            // Grab one byte for value of variable
            Grab_Byte (aux_mem, mem_addr, low, high, high_next);
            cout << mem_addr << " " << low << " " << high << " " << high_next << endl;

            mem.WriteByte (high_next ? low : high, variables[var_id]);

            var_count--;
            continue;
        }
    }

    //     while (1) {
    //         // Grab the next instruction from the .output file
    //         if (!Pop_Next_Ins (value, program, instruction))      return;

    //         // Read a variable and initialize it in memory
    //         if (var_count != 0) {
    //             word var_id = instruction;

    //             if (!Pop_Next_Ins (value, program, instruction))      return;
    //             var_id |= (instruction << 8);

    //             if (!Pop_Next_Ins (value, program, instruction));
    //             variables[var_id] = var_address++;
    //             mem.WriteByte (instruction, variables[var_id]);

    //             if (program.empty ())       return;
    //             var_count--;
    //             continue;
    //         }

    //         // HALT instruction signifies the end of a subroutine
    //         if (IS_HALT_OPCODE (instruction)) {
    //             if (!PC_set)        cpu.PC = address;
    //             mem.WriteByte (instruction, address);
    //             continue;
    //         }

    //         // Label ids have an extra instruction preceding it, 0x36, to signify that a label is present
    //         if (IS_LABEL_ENCODING (instruction)) {
    //             word label_id;

    //             if (!Pop_Next_Ins (value, program, instruction))    return;
    //             label_id = instruction;

    //             if (!Pop_Next_Ins (value, program, instruction))    return;
    //             label_id |= (instruction << 8);
    //             headers[label_id] = address;

    //             while (unInit_headers.count (label_id) != 0) {
    //                 multimap<word, word>::iterator it = unInit_headers.find (label_id);
    //                 mem.WriteWord (headers[label_id], it->second);
    //                 unInit_headers.erase (it);
    //             }

    //             if (!Pop_Next_Ins (value, program, instruction))    return;
    //         }

    //         // Subroutine ids have an extra instruction preceding it, 0x41, to singify that a subroutine is present
    //         if (IS_SUBR_ENCODING (instruction)) {
    //             word subr_id;

    //             if (!Pop_Next_Ins (value, program, instruction))    return;
    //             subr_id = instruction;

    //             if (!Pop_Next_Ins (value, program, instruction))    return;
    //             subr_id |= (instruction << 8);
    //             subroutines[subr_id] = address;

    //             while (unInit_subroutines.count (subr_id) != 0) {
    //                 multimap<word, word>::iterator it = unInit_subroutines.find (subr_id);
    //                 mem.WriteByte (subroutines[subr_id], it->second);
    //                 unInit_subroutines.erase (it);
    //             }

    //             if (!Pop_Next_Ins (value, program, instruction))    return;
    //         }

    //         // The START instruction denotes a non linear entry point of the cpu 
    //         if (IS_START_ENCODING (instruction)) {
    //             cpu.PC = address;
    //             PC_set = true;

    //             continue;
    //         }

    //         // Update the start of the program. Any subroutines appear the main program of code.
    //         if (!PC_set) {
    //             cpu.PC = address;
    //             PC_set = true;
    //         }

    //         // Write the opcode instruction to memory
    //         mem.WriteByte (instruction, address++);
    //         opcode = instruction;

    //         // Check the first operand and upload correct instruction to memory
    //         if (REG_AS_OPERAND1 (opcode)) {
    //             if (!Pop_Next_Ins (value, program, instruction))    return;
    //             mem.WriteByte (instruction, address++);
    //         } else if (MEM_AS_OPERAND1 (opcode)) {
    //             word var_id;

    //             if (!Pop_Next_Ins (value, program, instruction))    return;
    //             var_id = instruction;

    //             if (!Pop_Next_Ins (value, program, instruction))    return;
    //             var_id |= (instruction << 8);

    //             mem.WriteWord (variables[var_id], address);
    //             address += 2;
    //         } else if (LAB_AS_OPERAND1 (opcode)) {
    //             word label_id;

    //             if (!Pop_Next_Ins (value, program, instruction))    return;
    //             label_id = instruction;

    //             if (!Pop_Next_Ins (value, program, instruction))    return;
    //             label_id |= (instruction << 8);

    //             if (headers.count (label_id) == 0)      unInit_headers.insert (pair<word, word> (label_id, address));
    //             else                                    mem.WriteWord (headers[label_id], address);
                
    //             address += 2;
    //         } else if (SBR_AS_OPERAND1 (opcode)) {
    //             word subr_id;

    //             if (!Pop_Next_Ins (value, program, instruction))    return;
    //             subr_id = instruction;

    //             if (!Pop_Next_Ins (value, program, instruction))    return;
    //             subr_id |= (instruction << 8);

    //             if (subroutines.count (subr_id == 0))       unInit_subroutines.insert (pair<word, word> (subr_id, address));
    //             else                                        mem.WriteWord (subroutines[subr_id], address);
    //         }

    //         // Check the second operand and upload correct instruction to memory
    //         if (REG_AS_OPERAND2 (opcode)) {
    //             if (!Pop_Next_Ins (value, program, instruction))    return;
    //             mem.WriteByte (instruction, address++);
    //         } else if (MEM_AS_OPERAND2 (opcode)) {
    //             word var_id;

    //             if (!Pop_Next_Ins (value, program, instruction))    return;
    //             var_id = instruction;

    //             if (!Pop_Next_Ins (value, program, instruction))    return;
    //             var_id |= (instruction << 8);

    //             mem.WriteWord (variables[var_id], address);
    //             address += 2;
    //         } else if (IMM_AS_OPERAND2 (opcode)) {
    //             if (!Pop_Next_Ins (value, program, instruction))    return;
    //             mem.WriteByte (instruction, address++);
    //         }
    //     }
    // }
}

// string htos (byte value) {
//     string hex_val;

//     hex_val = STRING_FROM_HEX ((int) (value) % 16);
//     hex_val = to_string ((int) (value) / 16) + hex_val;

//     return hex_val;
// }

void Grab_Word (aux_mem_t& aux_mem, word& mem_addr, byte& low, byte& high, bool& high_next) {
    if (high_next) {
        Grab_High (aux_mem, mem_addr, high);
        mem_addr++;
        Grab_Low (aux_mem, mem_addr, low);
    } else {
        Split_Bytes (aux_mem, mem_addr, low, high);
    }
}

void Grab_Byte (aux_mem_t& aux_mem, word& mem_addr, byte& low, byte& high, bool& high_next) {
    if (high_next) {
        Grab_High (aux_mem, mem_addr, high);
        mem_addr++;
    } else {
        Grab_Low (aux_mem, mem_addr, low);
    }

    high_next ^= 1;
}

void Grab_Low (aux_mem_t& aux_mem, word& mem_addr, byte& low) {
    low = (aux_mem[mem_addr] & 0xFF);
}

void Grab_High (aux_mem_t& aux_mem, word& mem_addr, byte& high) {
    high = (aux_mem[mem_addr] >> 8);
}

void Split_Bytes (aux_mem_t& aux_mem, word& mem_addr, byte& low, byte& high) {
    low = (aux_mem[mem_addr] & 0xFF);
    high = (aux_mem[mem_addr] >> 8);
    mem_addr++;
}

// bool Pop_Next_Ins (string& value, string& program, word& instruction) {
//     if (program.empty ())   return false;

//     value = program.substr (0, 3);
//     value.pop_back ();
//     instruction = stoh (value);
//     program.erase (0, 3);

//     return true;
//     // return !(program.empty ());
// }
