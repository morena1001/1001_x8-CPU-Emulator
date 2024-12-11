#include <iostream>
#include <string>
#include <map>
#include <fstream>

#include "aux_mem_loader.h"

using namespace std;

void Split_Bytes (aux_mem_t& aux_mem, aux_loader_t& loader);
void Grab_Low (aux_mem_t& aux_mem, aux_loader_t& loader);
void Grab_High (aux_mem_t& aux_mem, aux_loader_t& loader);
word Grab_Word (aux_mem_t& aux_mem, aux_loader_t& loader);
byte Grab_Byte (aux_mem_t& aux_mem, aux_loader_t& loader);

void Load_Program_From_AuxMem (cpu_t& cpu, mem_t& mem, aux_mem_t& aux_mem, aux_loader_t& loader) {
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

    word instruction, opcode;
    word var_count = Grab_Word (aux_mem, loader);
    word end_addr = (mem[p_stack_addr + 2]) | ((mem[p_stack_addr + 3]) << 8);

    for (loader.mem_addr; loader.mem_addr < end_addr;) {
        // Parse all variables
        if (var_count != 0) {
            word var_id = Grab_Word (aux_mem, loader);
            variables[var_id] = var_address++;

            byte value = Grab_Byte (aux_mem, loader);
            mem.WriteByte (value, variables[var_id]);

            var_count--;
            continue;
        }

        instruction = Grab_Byte (aux_mem, loader);

        if (IS_HALT_OPCODE (instruction)) {
            if (!PC_set)    cpu.PC = address;
            mem.WriteByte (instruction, address++);
            continue;
        }

        // Label ids have an extra instruction preceding it, 0x36, to signify that a label is present
        if (IS_LABEL_ENCODING (instruction)) {
            word label_id = Grab_Word (aux_mem, loader);
            headers[label_id] = address;

            while (unInit_headers.count (label_id) != 0) {
                multimap<word, word>::iterator it = unInit_headers.find (label_id);
                mem.WriteByte (headers[label_id], it->second);
                unInit_headers.erase (it);
            }

            instruction = Grab_Byte (aux_mem, loader);
        }

        // Subroutine ids have an extra instruction preceding it, 0x41, to singify that subroutine is present
        if (IS_SUBR_ENCODING (instruction)) {
            word subr_id = Grab_Word (aux_mem, loader);
            subroutines[subr_id] = address;

            while (unInit_subroutines.count (subr_id) != 0) {
                multimap<word, word>::iterator it = unInit_subroutines.find (subr_id);
                mem.WriteByte (subroutines[subr_id], it->second);
                unInit_subroutines.erase (it);
            }

            instruction = Grab_Byte (aux_mem, loader);
        }

        // The START instruction denotes a non linear entry point of the cpu 
        if (IS_START_ENCODING (instruction)) {
            cpu.PC = address;
            PC_set = true;

            continue;
        } 

        // Update the start of the program. Any subroutines appear the main program of code.
        if (!PC_set) {
            cpu.PC = address;
            PC_set = true;
        }

        // Write the opcode instruction to memory
        mem.WriteByte (instruction, address++);
        opcode = instruction;

        // Check the first operand and upload the correct instruction to memory
        if (REG_AS_OPERAND1 (opcode)) {
            instruction = Grab_Byte (aux_mem, loader);
            mem.WriteByte (instruction, address++);
        } else if (MEM_AS_OPERAND1 (opcode)) {
            bool is_addr = IS_ADDR_ENCODING (Grab_Byte (aux_mem, loader));
            word var_id = Grab_Word (aux_mem, loader);

            if (is_addr)        mem.WriteWord (var_id, address);
            else                mem.WriteWord (variables[var_id], address);

            address += 2;
        } else if (LAB_AS_OPERAND1 (opcode)) {
            word label_id = Grab_Word (aux_mem, loader);

            if (headers.count (label_id) == 0)      unInit_headers.insert (pair<word, word> (label_id, address));
            else                                    mem.WriteWord (headers[label_id], address);

            address += 2;
        } else if (SBR_AS_OPERAND1 (opcode)) {
            word subr_id = Grab_Word (aux_mem, loader);

            if (subroutines.count (subr_id) == 0)   unInit_subroutines.insert (pair<word, word> (subr_id, address));
            else                                    mem.WriteWord (subroutines[subr_id, address], address);
        }

        // Check the second operand and upload correct instruction to memory
        if (REG_AS_OPERAND2 (opcode)) {
            instruction = Grab_Byte (aux_mem, loader);
            mem.WriteByte (instruction, address++);
        } else if (MEM_AS_OPERAND2 (opcode)) {
            word is_addr = IS_ADDR_ENCODING (Grab_Byte (aux_mem, loader));
            word var_id = Grab_Word (aux_mem, loader);

            if (is_addr)        mem.WriteWord (var_id, address);
            else                mem.WriteWord (variables[var_id], address);

            address += 2;
        } else if (IMM_AS_OPERAND2 (opcode)) {
            instruction = Grab_Byte (aux_mem, loader);
            mem.WriteByte (instruction, address++);
        }
    }
}

word Grab_Word (aux_mem_t& aux_mem, aux_loader_t& loader) {
    if (loader.high_next) {
        Grab_High (aux_mem, loader);
        loader.mem_addr++;
        Grab_Low (aux_mem, loader);
    } else {
        Split_Bytes (aux_mem, loader);
    }
    return (word) (loader.low) | (word) (loader.high << 8);
}

byte Grab_Byte (aux_mem_t& aux_mem, aux_loader_t& loader) {
    if (loader.high_next) {
        Grab_High (aux_mem, loader);
        loader.high_next ^= 1;
        loader.mem_addr++;
        return loader.high;
    } else {
        Grab_Low (aux_mem, loader);
        loader.high_next ^= 1;
        return loader.low;
    }
}

void Split_Bytes (aux_mem_t& aux_mem, aux_loader_t& loader) {
    loader.low = (aux_mem[loader.mem_addr] & 0xFF);
    loader.high = (aux_mem[loader.mem_addr] >> 8);
    loader.mem_addr++;
}

void Grab_Low (aux_mem_t& aux_mem, aux_loader_t& loader) {
    loader.low = (aux_mem[loader.mem_addr] & 0xFF);
}

void Grab_High (aux_mem_t& aux_mem, aux_loader_t& loader) {
    loader.high = (aux_mem[loader.mem_addr] >> 8);
}
