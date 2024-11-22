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
    map<word, word> variables;

    word address = 0x1000;
    word var_address = 0xCD33;

    bool PC_set = false;
    bool end_of_program = false;

    if (!output_file.is_open ()) {
        cout << "Could not open " << file_path << "." << endl; 
        return;
    }

    if (getline (output_file, program)) {
        string value;
        word instruction, opcode;

        if (!Pop_Next_Ins (value, program, instruction))      return;

        while (1) {
            // Any code after the HALT instruction is variable declaration
            if (end_of_program) {
                word var_id;
                if (!Pop_Next_Ins (value, program, instruction))      return;
                var_id = instruction;

                if (!Pop_Next_Ins (value, program, instruction))      return;
                var_id |= (instruction << 8);

                if (!Pop_Next_Ins (value, program, instruction));
                mem.WriteByte (instruction, variables[var_id]);

                if (program.empty ())       return;

                continue;
            } 

            // HALT instruction denotes the end of the running program, the rest is for variables
            if (IS_HALT_OPCODE (instruction)) {
                if (!PC_set)    cpu.PC = address;
                end_of_program = true;
                mem.WriteByte (instruction, address);

                continue;
            }

            // Labels ids have an extra instruction preceding it, 0x36, to signify that a label is present
            if (IS_LABEL_ENCODING (instruction)) {
                word label_id;

                if (!Pop_Next_Ins (value, program, instruction))    return;
                label_id = instruction;

                if (!Pop_Next_Ins (value, program, instruction))    return;
                label_id |= (instruction << 8);
                headers[label_id] = address;

                if (!Pop_Next_Ins (value, program, instruction))    return;
            }

            // The START instruction denotes a non linear entry point of the cpu 
            if (IS_START_ENCODING (instruction)) {
                cpu.PC = address;
                PC_set = true;
                if (!Pop_Next_Ins (value, program, instruction))    return;
                
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

                if (!Pop_Next_Ins (value, program, instruction))    return;
                var_id = instruction;

                if (!Pop_Next_Ins (value, program, instruction))    return;
                var_id |= (instruction << 8);

                if (variables.count (var_id) == 0)  variables[var_id] = var_address++;
                mem.WriteWord (variables[var_id], address);
                address += 2;
            } else if (LAB_AS_OPERAND1 (opcode)) {
                word label_id;

                if (!Pop_Next_Ins (value, program, instruction))    return;
                label_id = instruction;

                if (!Pop_Next_Ins (value, program, instruction))    return;
                label_id |= (instruction << 8);

                mem.WriteWord (headers[label_id], address);
                address += 2;
            }

            // Check the second operand and upload correct instruction to memory
            if (REG_AS_OPERAND2 (opcode)) {
                if (!Pop_Next_Ins (value, program, instruction))    return;
                mem.WriteByte (instruction, address++);
            } else if (MEM_AS_OPERAND2 (opcode)) {
                word var_id;

                if (!Pop_Next_Ins (value, program, instruction))    return;
                var_id = instruction;

                if (!Pop_Next_Ins (value, program, instruction))    return;
                var_id |= (instruction << 8);

                if (variables.count (var_id) == 0)  variables[var_id] = var_address++;
                mem.WriteWord (variables[var_id], address);
                address += 2;
            } else if (IMM_AS_OPERAND2 (opcode)) {
                if (!Pop_Next_Ins (value, program, instruction))    return;
                mem.WriteByte (instruction, address++);
            }

            if (!Pop_Next_Ins (value, program, instruction));//        return;
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

    return !(program.empty ());
}
