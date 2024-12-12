#include <iostream>
#include <string>
#include <map>
#include <fstream>

#include "loader.h"

using namespace std;

bool Pop_Next_Ins (string& value, string& program, word& instruction);

void Load_Program (string file_path, cpu_t& cpu, mem_t& mem) {
    ifstream output_file;
    output_file.open (file_path);

    string program;

    map<word, word> headers;
    multimap<word, word> unInit_headers;
    map<word, word> variables;

    map<word, word> subroutines;
    multimap<word, word> unInit_subroutines;

    word address = 0xE000;
    word var_address = 0xD85D;

    bool PC_set = false;
    bool end_of_program = false;

    if (!output_file.is_open ()) {
        cout << "Could not open " << file_path << "." << endl; 
        return;
    }

    if (getline (output_file, program)) {
        string value;
        word instruction, opcode;

        word var_count;

        // The first two bytes of a program are ALWAYS the number of variables used
        if (!Pop_Next_Ins (value, program, instruction))      return;
        var_count = instruction;

        if (!Pop_Next_Ins (value, program, instruction))      return;
        var_count |= (instruction << 8);

        while (1) {
            // Grab the next instruction from the .output file
            if (!Pop_Next_Ins (value, program, instruction))      return;

            // Read a variable and initialize it in memory
            if (var_count != 0) {
                word var_id = instruction;

                if (!Pop_Next_Ins (value, program, instruction))      return;
                var_id |= (instruction << 8);

                if (!Pop_Next_Ins (value, program, instruction));
                variables[var_id] = var_address++;
                mem.WriteByte (instruction, variables[var_id]);

                if (program.empty ())       return;
                var_count--;
                continue;
            }

            // HALT instruction signifies the end of a subroutine
            if (IS_HALT_OPCODE (instruction)) {
                if (!PC_set)        cpu.PC = address;
                mem.WriteByte (instruction, address);
                continue;
            }

            // Label ids have an extra instruction preceding it, 0x36, to signify that a label is present
            if (IS_LABEL_ENCODING (instruction)) {
                word label_id;

                if (!Pop_Next_Ins (value, program, instruction))    return;
                label_id = instruction;

                if (!Pop_Next_Ins (value, program, instruction))    return;
                label_id |= (instruction << 8);
                headers[label_id] = address;

                cout << "NO SHOT " << headers[label_id] << "  " << label_id << endl;

                while (unInit_headers.count (label_id) != 0) {
                    multimap<word, word>::iterator it = unInit_headers.find (label_id);
                    mem.WriteWord (headers[label_id], it->second);
                    unInit_headers.erase (it);
                }

                if (!Pop_Next_Ins (value, program, instruction))    return;
            }

            // Subroutine ids have an extra instruction preceding it, 0x41, to singify that a subroutine is present
            if (IS_SUBR_ENCODING (instruction)) {
                word subr_id;

                if (!Pop_Next_Ins (value, program, instruction))    return;
                subr_id = instruction;

                if (!Pop_Next_Ins (value, program, instruction))    return;
                subr_id |= (instruction << 8);
                subroutines[subr_id] = address;

                while (unInit_subroutines.count (subr_id) != 0) {
                    multimap<word, word>::iterator it = unInit_subroutines.find (subr_id);
                    mem.WriteByte (subroutines[subr_id], it->second);
                    unInit_subroutines.erase (it);
                }

                // if (!Pop_Next_Ins (value, program, instruction))    return;
                continue;
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

            // Check the first operand and upload correct instruction to memory
            if (REG_AS_OPERAND1 (opcode)) {
                if (!Pop_Next_Ins (value, program, instruction))    return;
                mem.WriteByte (instruction, address++);
            } else if (MEM_AS_OPERAND1 (opcode)) {
                word var_id;
                bool addr;

                if (!Pop_Next_Ins (value, program, instruction))    return;
                addr = IS_ADDR_ENCODING (instruction);

                if (!Pop_Next_Ins (value, program, instruction))    return;
                var_id = instruction;

                if (!Pop_Next_Ins (value, program, instruction))    return;
                var_id |= (instruction << 8);

                if (addr)           mem.WriteWord (var_id, address);
                else                mem.WriteWord (variables[var_id], address);

                address += 2;
            } else if (LAB_AS_OPERAND1 (opcode)) {
                word label_id;

                if (!Pop_Next_Ins (value, program, instruction))    return;
                label_id = instruction;

                if (!Pop_Next_Ins (value, program, instruction))    return;
                label_id |= (instruction << 8);

                cout << "YAY " << headers.count (label_id) << " " << label_id << endl;

                if (headers.count (label_id) == 0)      unInit_headers.insert (pair<word, word> (label_id, address));
                else                                    mem.WriteWord (headers[label_id], address);
                
                address += 2;
            } else if (SBR_AS_OPERAND1 (opcode)) {
                word subr_id;

                if (!Pop_Next_Ins (value, program, instruction))    return;
                subr_id = instruction;

                if (!Pop_Next_Ins (value, program, instruction))    return;
                subr_id |= (instruction << 8);

                if (subroutines.count (subr_id) == 0)       unInit_subroutines.insert (pair<word, word> (subr_id, address));
                else                                        mem.WriteWord (subroutines[subr_id], address);

                address += 2;
            }

            // Check the second operand and upload correct instruction to memory
            if (REG_AS_OPERAND2 (opcode)) {
                if (!Pop_Next_Ins (value, program, instruction))    return;
                mem.WriteByte (instruction, address++);
            } else if (MEM_AS_OPERAND2 (opcode)) {
                word var_id;
                bool addr;

                if (!Pop_Next_Ins (value, program, instruction))    return;
                addr = IS_ADDR_ENCODING (instruction);

                if (!Pop_Next_Ins (value, program, instruction))    return;
                var_id = instruction;

                if (!Pop_Next_Ins (value, program, instruction))    return;
                var_id |= (instruction << 8);

                if (addr)           mem.WriteWord (var_id, address);
                else                mem.WriteWord (variables[var_id], address);

                address += 2;
            } else if (IMM_AS_OPERAND2 (opcode)) {
                if (!Pop_Next_Ins (value, program, instruction))    return;
                mem.WriteByte (instruction, address++);
            }
        }
    }
}

word stoh (string value) {
    word hex_val;

    // Make sure that an instruction is a byte, 2 hex values, long
    if (value.length () != 2)   return 0;

    // Conversion of hex in string to int
    hex_val = (INT_REPRESENTATION_OF_HEX (value[0]) * 16) + (INT_REPRESENTATION_OF_HEX (value[1]));

    return hex_val;
}

string htos (byte value) {
    string hex_val;

    hex_val = STRING_FROM_HEX ((int) (value) % 16);
    hex_val = to_string ((int) (value) / 16) + hex_val;

    return hex_val;
}

bool Pop_Next_Ins (string& value, string& program, word& instruction) {
    if (program.empty ())   return false;

    value = program.substr (0, 3);
    value.pop_back ();
    instruction = stoh (value);
    program.erase (0, 3);

    return true;
    // return !(program.empty ());
}
