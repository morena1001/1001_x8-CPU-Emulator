#include <iostream>
#include <string>
#include <map>
#include <fstream>

#include "loader.h"

using namespace std;

word Load_Program (string file_path, cpu_t& cpu, mem_t& mem) {
    ifstream output_file;

    string program;
    map<word, word> headers;
    map<word, word> variables;
    bool PC_set = false;
    bool end_of_program = false;
    word address = 0x1000;
    word var_address = 0xDE00;

    output_file.open (file_path);

    if (output_file.is_open ()) {
        while (getline (output_file, program)) {
            string value;
            word instruction;

            value = program.substr (0, 3);
            value.pop_back ();

            while (1) {
                instruction = stoh (value);

                if (!end_of_program) {
                    if (instruction == 0x35) {
                        // Load opcode to memory
                        if (!PC_set) {
                            cpu.PC = address;
                            
                            PC_set = true;
                        }
                        mem.WriteByte (instruction, address++);
                    }

                    if (instruction == 0x36) {
                        word id;

                        program.erase (0, 3);

                        if (program == "") {
                            break;
                        }
                        value = program.substr (0, 3);
                        value.pop_back ();
                        instruction = stoh (value);

                        id = instruction;

                        program.erase (0, 3);

                        if (program == "") {
                            break;
                        }
                        value = program.substr (0, 3);
                        value.pop_back ();
                        instruction = stoh (value);    

                        id |= (instruction << 8);

                        address += 3;
                        headers[id] = address;
                    } else {
                        // Load opcode to memory
                        if (!PC_set) {
                            cpu.PC = address;
                            
                            PC_set = true;
                        }
                        mem.WriteByte (instruction, address++);

                        if (REG_AS_OPERAND1 (instruction)) {
                            program.erase (0, 3);

                            if (program == "") {
                                break;
                            }
                            value = program.substr (0, 3);
                            value.pop_back ();
                            instruction = stoh (value);

                            mem.WriteByte (instruction, address++);
                        } else if (MEM_AS_OPERAND1 (instruction)) {

                            // mem.WriteWord (var_address, address);
                            

                            // for (int i = 0; i < 2; i++) {
                            //     program.erase (0, 3);

                            //     if (program == "") {
                            //         break;
                            //     }
                            //     value = program.substr (0, 3);
                            //     value.pop_back ();
                            //     instruction = stoh (value);

                            //     mem.WriteByte (instruction, address++);
                            // }
                        } else if (LAB_AS_OPERAND1 (instruction)) {
                            word id;

                            program.erase (0, 3);

                            if (program == "") {
                                break;
                            }
                            value = program.substr (0, 3);
                            value.pop_back ();
                            instruction = stoh (value);

                            id = instruction;

                            program.erase (0, 3);

                            if (program == "") {
                                break;
                            }
                            value = program.substr (0, 3);
                            value.pop_back ();
                            instruction = stoh (value);    

                            id |= (instruction << 8);

                            mem.WriteWord (headers[id], address);
                            address += 2;
                        }
                        


                        if (REG_AS_OPERAND2 (instruction)) {
                            program.erase (0, 3);

                            if (program == "") {
                                break;
                            }
                            value = program.substr (0, 3);
                            value.pop_back ();
                            instruction = stoh (value);

                            mem.WriteByte (instruction, address++);
                        } else if (MEM_AS_OPERAND2 (instruction)) {
                            for (int i = 0; i < 2; i++) {
                                // program.erase (0, 3);

                                // if (program == "") {
                                //     break;
                                // }
                                // value = program.substr (0, 3);
                                // value.pop_back ();
                                // instruction = stoh (value);

                                // mem.WriteByte (instruction, address++);
                            }
                        } else if (IMM_AS_OPERAND2 (instruction)) {
                            program.erase (0, 3);

                            if (program == "") {
                                break;
                            }
                            value = program.substr (0, 3);
                            value.pop_back ();
                            instruction = stoh (value);

                            mem.WriteByte (instruction, address++);
                        }
                    }
                } else {
                    
                }

                   


                program.erase (0, 3);

                if (program == "") {
                    break;
                }
                value = program.substr (0, 3);
                value.pop_back ();
            }
            // cout << program << endl;
        }

    }  else {
        cout << "Could not open " << file_path << "." << endl; 
        return 0;
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
