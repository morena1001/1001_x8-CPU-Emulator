#include <stdio.h>

#include <iostream>
#include <fstream>
#include <string>
#include <map>

// Data types used in 1001_x8
using byte = unsigned char;
using word = unsigned short;
using u32 = unsigned int;

using namespace std;

int main (int argc, char** argv) {
    ifstream program;

    // Check if the prorgram was exectued with a given source file, else load a sample program
    if (argc == 2) {
        program.open (argv[1]);
    } else {
        program.open ("sample_program.lk");
    }

    // variables for instructions in .main section
    byte instructions[8192];
    for (u32 i = 0; i < 8192; i++)  instructions[i] = 0x00;
    u32 ins_idx = 0;
    map<string, word> opcodes;
    map<char, word> registers;
    opcodes["LDFM"] = 0x01;
    opcodes["LDFI"] = 0x02;
    opcodes["LDFR"] = 0x03;
    opcodes["STOR"] = 0x04;
    opcodes["SWPR"] = 0x05;
    opcodes["SWPM"] = 0x06;
    opcodes["PULR"] = 0x07;
    opcodes["PSHR"] = 0x08;
    opcodes["PSHM"] = 0x09;
    opcodes["PULC"] = 0x0A;
    opcodes["PSHC"] = 0x0B;
    opcodes["PULF"] = 0x0C;
    opcodes["PSHF"] = 0x0D;
    opcodes["AND"] = 0x0E;
    opcodes["EOR"] = 0x0F;
    opcodes["ORA"] = 0x10;
    opcodes["NOT"] = 0x11;
    opcodes["ADD"] = 0x12;
    opcodes["SUB"] = 0x13;
    opcodes["MULT"] = 0x14;
    opcodes["DIV"] = 0x15;
    opcodes["CMPR"] = 0x16;
    opcodes["CMPM"] = 0x17;
    opcodes["CMPI"] = 0x18;
    opcodes["INCR"] = 0x19;
    opcodes["INCM"] = 0x1A;
    opcodes["DECR"] = 0x1B;
    opcodes["DECM"] = 0x1C;
    opcodes["SHLR"] = 0x1D;
    opcodes["SHRR"] = 0x1E;
    opcodes["SHLM"] = 0x1F;
    opcodes["SHRM"] = 0x20;
    opcodes["JMP"] = 0x21;
    opcodes["JSR"] = 0x22;
    opcodes["RSR"] = 0x23;
    opcodes["JSC"] = 0x24;
    opcodes["JSS"] = 0x25;
    opcodes["JCC"] = 0x26;
    opcodes["JCS"] = 0x27;
    opcodes["JOC"] = 0x28;
    opcodes["JOS"] = 0x29;
    opcodes["JZC"] = 0x2A;
    opcodes["JZS"] = 0x2B;
    opcodes["CCF"] = 0x2C;
    opcodes["SCF"] = 0x2D;
    opcodes["CSF"] = 0x2E;
    opcodes["SSF"] = 0x2F;
    opcodes["COF"] = 0x30;
    opcodes["SOF"] = 0x31;
    opcodes["CZF"] = 0x32;
    opcodes["SZF"] = 0x33;
    opcodes["NOP"] = 0x34;
    opcodes["HALT"] = 0x35;

    registers['A'] = 0x00;
    registers['B'] = 0x01;
    registers['C'] = 0x02;
    registers['D'] = 0x03;
    registers['E'] = 0x04;
    registers['F'] = 0x05;
    registers['G'] = 0x06;
    registers['H'] = 0x07;

    // Variables for .variables section
    word unique_ID = 0x0001;
    byte variable_instructions[512 * 3];
    for (u32 i = 0; i < 512 * 3; i++)      variable_instructions[i] = 0x00;
    byte var_idx = 0;
    map<string, word> IDs;

    string line;
    u32 idx;
    u32 line_number = 0;

    bool halt_present = false;
    bool in_variables_section = false;
    bool in_main_section = false;

    if (program.is_open ()) {
        while (getline (program, line)) {
            
            cout << line << endl;
            line_number++;

            // remove white space from front
            idx = line.find_first_not_of (" ");
            line.erase (line.begin (), line.begin () + idx);

            // remove comments 
            idx = line.find_first_of (";");
            if (idx != string::npos) {
                line.erase (line.begin () + idx, line.end ());
            }

            // remove white space from back
            idx = line.find_last_not_of (" ");
            if (idx != string::npos) {
                line.erase (line.begin() + idx + 1, line.end ());
            }

            // skip empty lines
            if (line.length () < 2) {
                continue;
            }

            // Check if the line is the label for a section
            if (line.length () > 0 && line.at (0) == '.') {
                string section = line.substr (1, -1); 

                if (!section.compare ("variables")) {
                    in_variables_section = true;
                    in_main_section = false;
                    continue;
                } else if (!section.compare ("main")) {
                    in_main_section = true;
                    in_variables_section = false;
                    continue;
                } else {
                    cout << line_number << " : Section " << section << " is undefined.\n";
                    break;
                }
            }

            // If in variables section, process variables accordingly
            if (in_variables_section) {
                // Check that variables are in the correct format. i.e. "$a"
                if (line.at (0) != '$') {
                    cout << line_number << " : Wrong variable format" << endl;
                }

                // Remove $ from string, and split in two for name and value
                line = line.substr (1, -1);
                idx = line.find_first_of (" ");
                string var_name = line.substr (0, idx);
                byte value = (byte) stoi (line.substr (idx + 1, -1));

                // Add the variables unique ID to the instructions, and pair name with ID in map
                variable_instructions[var_idx++] = (unique_ID & 0xFF);
                variable_instructions[var_idx++] = (unique_ID >> 8);
                variable_instructions[var_idx++] = value;
                IDs[var_name] = unique_ID++;
            }

            // If in main section, process commands accordingly
            if (in_main_section) {
                // Check if the line is a label to be able to perform JMP commands
                if (line.at (0) == '#') {

                    continue;
                }

                // All commands are split into opcode and operands
                idx = line.find_first_of (" ");
                string opcode = line.substr (0, idx == string::npos ? -1 : idx);
                string operands = idx == string::npos ? "" : line.substr (idx + 1, -1);

                // Check that the opcode is valid, and it to the instructions
                if (opcodes.count (opcode) == 0) {
                    cout << line_number << " : Opcode " << opcode << "is undefined." << endl;
                    return 0;
                }
                instructions[ins_idx++] = opcodes[opcode];

                // The HALT command signifies the end of a program
                if (!line.compare ("HALT")) {
                    halt_present = true;
                    in_main_section = false;
                }

                // Check that the given opcode does or does not require any operands

                // If there are operands present, continue with the next line
                if (operands.length() == 0) {
                    continue;
                }

                // Split the operands into operand 1 and 2
                idx = operands.find_first_of (" ");
                string op1 = operands.substr (0, idx == string::npos ? -1 : idx);
                string op2 = idx == string::npos ? "" : operands.substr (idx + 1, -1);

                // Switch for the first operand. They can either be a literal (%), variable ($), or register (@)
                switch (op1[0]) {
                    case '%': {
                        word num = stoi (op1.substr (1, -1));

                        instructions[ins_idx++] = (byte) num;
                    } break;

                    case '$': {
                        string var_name = op1.substr (1, -1);
                        if (IDs.count (var_name) == 0) {
                            cout << line_number << " : Variable " << var_name << " is undefined." << endl;
                            return 0;
                        }

                        instructions[ins_idx++] = (IDs[var_name] & 0xFF);
                        instructions[ins_idx++] = (IDs[var_name] >> 8);
                    } break;

                    case '@': {
                        char reg = op1[1];
                        if (registers.count (reg) == 0) {
                            cout << line_number << " : Register " << opcode << " is undefined." << endl;
                            return 0;
                        }

                        instructions[ins_idx++] = registers[reg];
                    } break;

                    default: {
                        cout << line_number << " : Symbol " << op1[0] << " is undefined." << endl;
                        return 0;
                    } break;
                }

                // Switch for the first operand. They can either be a literal (%), variable ($), or register (@)
                switch (op2[0]) {
                    case '%': {
                        word num = stoi (op2.substr (1, -1));

                        instructions[ins_idx++] = (byte) num;
                    } break;

                    case '$': {
                        string var_name = op2.substr (1, -1);

                        if (IDs.count (var_name) == 0) {
                            cout << line_number << " : Variable " << var_name << " is undefined." << endl;
                            return 0;
                        }

                        instructions[ins_idx++] = (IDs[var_name] & 0xFF);
                        instructions[ins_idx++] = (IDs[var_name] >> 8);
                    } break;

                    case '@': {
                        char reg = op2[1];
                        if (registers.count (reg) == 0) {
                            cout << line_number << " : Register " << opcode << " is undefined." << endl;
                            return 0;
                        }

                        instructions[ins_idx++] = registers[reg];
                    } break;

                    default: {
                        cout << line_number << " : Symbol " << op2[0] << " is undefined." << endl;
                        return 0;
                    } break;
                }
            }
        }

        if (!halt_present) {
            cout << "End of program not define. Missing HALT command" << endl;
            return 0;
        }

        // Close the source file and convert processed information into .bin file
        program.close ();

        

    } else {
        cout << "Unable to open file" << endl;
    }

    return 0;
}
