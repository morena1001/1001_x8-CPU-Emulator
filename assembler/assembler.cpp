#include <stdio.h>

#include <iostream>
#include <fstream>
#include <string>
#include <map>

#define NEEDS_NO_OPERANDS(opcode)       ((opcode >= 0x0A && opcode <= 0x0D) || opcode == 0x23 || (opcode >= 0x2C && opcode <= 0x35) || opcode == 0x37) 
#define NEEDS_ONE_OPERAND(opcode)       ((opcode >= 0x07 && opcode <= 0x09) || opcode == 0x11 || (opcode >= 0x19 && opcode <= 0x22) || (opcode >= 0x24 && opcode <= 0x2B))
#define NEEDS_TWO_OPERANDS(opcode)      ((opcode >= 0x01 && opcode <= 0x06) || (opcode >= 0x0E && opcode <= 0x10) || (opcode >= 0x12 && opcode <= 0x18))
#define IS_A_JMP_OPERAND(opcode)        ((opcode >= 0x21 && opcode <= 0x22) || (opcode >= 0x24 && opcode <= 0x2B))

#define REG_AS_OPERAND1(opcode)         ((opcode >= 0x01 && opcode <= 0x03) || opcode == 0x05 || opcode == 0x07 || opcode == 0x08 || (opcode >= 0x0E && opcode <= 0x19) || opcode == 0x1B || opcode == 0x1D || opcode == 0x1E)
#define MEM_AS_OPERAND1(opcode)         (opcode == 0x04 || opcode == 0x06 || opcode == 0x09 || opcode == 0x1A || opcode == 0x1C || opcode == 0x1F || opcode == 0x20)
#define LAB_AS_OPERAND1(opcode)         (opcode == 0x21 || opcode == 0x22 || (opcode >= 0x24 && opcode <= 0x2B))

#define REG_AS_OPERAND2(opcode)         (opcode == 0x03 || opcode == 0x04 || opcode == 0x05 || opcode == 0x0E || opcode == 0x0F || opcode == 0x10 || (opcode >= 0x12 && opcode <= 0x16))
#define MEM_AS_OPERAND2(opcode)         (opcode == 0x01 || opcode == 0x06 || opcode == 0x17)
#define IMM_AS_OPERAND2(opcode)         (opcode == 0x02 || opcode == 0x18) 

#define STRING_FROM_HEX(value)          (value == 10 ? "A" : (value == 11 ? "B" : (value == 12 ? "C" : (value == 13 ? "D" : (value == 14 ? "E" : (value == 15 ? "F" : to_string (value)))))))

// Data types used in 1001_x8
using byte = uint8_t;
using word = unsigned short;
using u32 = unsigned int;

using namespace std;

void Opcodes_Init (map<string, word>& opcodes);
void Registers_Init (map<char, word>& registers);
void Remove_Space_and_Comments (u32& idx, string& line);

bool Opcode_and_Operands_Match (word opcode, string op1, string op2, u32 line_number);
string htos (byte value);

int main (int argc, char** argv) {
    ifstream program;
    string file_name;

    // Check if the prorgram was exectued with a given source file, else load a sample program
    if (argc == 2) {
        program.open (argv[1]);
        file_name = argv[1];

        if (file_name.substr (file_name.length () - 2, -1) != "lk") {
            cout << "Incorrect file type. Should be .lk." << endl;
            return 0;
        }

        file_name = file_name.substr (0, file_name.length () - 2);
    } else {
        program.open ("sample_program.lk");
    }

    // variables for instructions in .main section
    byte instructions[8192];
    u32 ins_idx = 0;
    map<string, word> opcodes;
    map<char, word> registers;

    Opcodes_Init (opcodes);
    Registers_Init (registers);

    word label_id = 0x0001;
    map<string, word> labels;

    // Variables for .variables section
    word variable_id = 0x0001;
    byte variable_instructions[512 * 3];
    byte var_idx = 0;
    map<string, word> variables;

    string line;
    u32 idx;
    u32 line_number = 0;

    bool halt_present = false;
    bool in_variables_section = false;
    bool in_main_section = false;

    if (program.is_open ()) {
        while (getline (program, line)) {
            
            // Increment line number for error throwing
            line_number++;

            Remove_Space_and_Comments (idx, line);

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
                    return 0;
                }

                // Remove $ from string, and check if there is a value after the variable name
                line = line.substr (1, -1);
                idx = line.find_first_of (" ");

                if (idx == string::npos) {
                    cout << line_number << " : Variables must be initialized where declared." << endl;
                    return 0;
                }

                // Split line in two, for name and value
                string var_name = line.substr (0, idx);
                byte value = (byte) stoi (line.substr (idx + 1, -1));

                // Add the variables unique ID to the instructions, and pair name with ID in map
                variable_instructions[var_idx++] = (variable_id & 0xFF);
                variable_instructions[var_idx++] = (variable_id >> 8);
                variable_instructions[var_idx++] = value;
                variables[var_name] = variable_id++;
            }



            // If in main section, process commands accordingly
            if (in_main_section) {
                // Check if the line is a label to be able to perform JMP commands
                if (line.at (0) == '#') {
                    string label_name = line.substr (1, -1);
                    instructions[ins_idx++] = 0x36;
                    instructions[ins_idx++] = (label_id & 0xFF);
                    instructions[ins_idx++] = (label_id >> 8);
                    labels[label_name] = label_id++;

                    continue;
                }

                // All commands are split into opcode and operands
                idx = line.find_first_of (" ");
                string opcode = line.substr (0, idx == string::npos ? -1 : idx);
                string operands = idx == string::npos ? "" : line.substr (idx + 1, -1);

                // Check that the opcode is valid, and add it to the instructions
                if (opcodes.count (opcode) == 0) {
                    cout << line_number << " : Opcode " << opcode << " is undefined." << endl;
                    return 0;
                }
                instructions[ins_idx++] = opcodes[opcode];

                // The HALT command signifies the end of a program
                if (!line.compare ("HALT")) {
                    halt_present = true;
                }

                // Remove extra white space
                idx = operands.find_first_not_of (" ");
                operands.erase (operands.begin (), operands.begin () + idx);

                // Split the operands into operand 1 and 2 if operands are needed
                string op1 = "", op2 = "";
                if (!NEEDS_NO_OPERANDS (opcodes[opcode])) {
                    idx = operands.find_first_of (" ");
                    op1 = operands.substr (0, idx == string::npos ? -1 : idx);
                    op2 = idx == string::npos ? "" : operands.substr (idx + 1, -1);
                } 

                if (!Opcode_and_Operands_Match (opcodes[opcode], op1, op2, line_number)) {
                    return 0;
                }

                // If an opcode needs no operands, continue with the next line
                if (NEEDS_NO_OPERANDS (opcodes[opcode])) {
                    continue;
                }

                // Switch for the first operand. They can either be a variable ($), register (@), or a label (#)
                switch (op1[0]) {
                    case '$': { // variable
                        if (!MEM_AS_OPERAND1 (opcodes[opcode])) {
                            cout << line_number << " : Opcode " << opcode << " cannot have a variable as its 1st operand." << endl;
                            return 0;
                        }

                        string var_name = op1.substr (1, -1);
                        if (variables.count (var_name) == 0) {
                            cout << line_number << " : Variable " << var_name << " is undefined." << endl;
                            return 0;
                        }

                        instructions[ins_idx++] = (variables[var_name] & 0xFF);
                        instructions[ins_idx++] = (variables[var_name] >> 8);
                    } break;

                    case '@': { // register
                        if (!REG_AS_OPERAND1 (opcodes[opcode])) {
                            cout << line_number << " : Opcode " << opcode << " cannot have a register as its 1st operand." << endl;
                            return 0;
                        }

                        char reg = op1[1];
                        if (registers.count (reg) == 0) {
                            cout << line_number << " : Register " << opcode << " is undefined." << endl;
                            return 0;
                        }

                        instructions[ins_idx++] = registers[reg];
                    } break;

                    case '#': { // label
                        if (!LAB_AS_OPERAND1 (opcodes[opcode])) {
                            cout << line_number << " : Opcode " << opcode << " cannot have a label as its 1st operand." << endl;
                            return 0;
                        }

                        string label = op1.substr (1, -1);
                        // Check if the opcode with the label operand is a jump
                        if (!IS_A_JMP_OPERAND(opcodes[opcode])) {
                            cout << line_number << " : Opcode " << opcode << " is not a jump instruction." << endl;
                            return 0;
                        }

                        // Check if the label is defined
                        if (labels.count (label) == 0) {
                            cout << line_number << " : Label " << label << " is undefined." << endl;
                            return 0;  
                        }

                        instructions[ins_idx++] = (labels[label] & 0xFF);
                        instructions[ins_idx++] = (labels[label] >> 8);
                    } break;

                    default: {
                        cout << line_number << " : Symbol " << op1[0] << " is undefined." << endl;
                        return 0;
                    } break;
                }

                // Go to next line if there is only one operand
                if (op2.length() == 0) {
                    continue;
                }

                // Switch for the first operand. They can either be a literal (%), variable ($), or register (@)
                switch (op2[0]) {
                    case '%': { // literal
                        if (!IMM_AS_OPERAND2 (opcodes[opcode])) {
                            cout << line_number << " : Opcode " << opcode << " cannot have an immediate as its 2nd operand." << endl;
                            return 0;
                        }

                        word num = stoi (op2.substr (1, -1));

                        instructions[ins_idx++] = (byte) num;
                    } break;

                    case '$': { // variable
                        if (!MEM_AS_OPERAND2 (opcodes[opcode])) {
                            cout << line_number << " : Opcode " << opcode << " cannot have a variable as its 2nd operand." << endl;
                            return 0;
                        }

                        string var_name = op2.substr (1, -1);

                        if (variables.count (var_name) == 0) {
                            cout << line_number << " : Variable " << var_name << " is undefined." << endl;
                            return 0;
                        }

                        instructions[ins_idx++] = (variables[var_name] & 0xFF);
                        instructions[ins_idx++] = (variables[var_name] >> 8);
                    } break;

                    case '@': { // register
                        if (!REG_AS_OPERAND2 (opcodes[opcode])) {
                            cout << line_number << " : Opcode " << opcode << " cannot have a register as its 2nd operand." << endl;
                            return 0;
                        }

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

        ofstream output_file (file_name + "output");
               
        u32 i = 0;
        byte single_instruction = instructions[i++];
        char machine_code_instruction[4];
        while (single_instruction != 0x35) {
            sprintf (machine_code_instruction, "%s%X ", single_instruction < 0x10 ? "0" : "", single_instruction);
            output_file << machine_code_instruction;
            
            single_instruction = instructions[i++];
        } 
        sprintf (machine_code_instruction, "%s%X ", single_instruction < 0x10 ? "0" : "", single_instruction);
        output_file << machine_code_instruction;

        i = 0;
        single_instruction = variable_instructions[i++];

        while (i < var_idx + 1) {
            sprintf (machine_code_instruction, "%s%X ", single_instruction < 0x10 ? "0" : "", single_instruction);
            output_file << machine_code_instruction;
            
            single_instruction = variable_instructions[i++];
        }

        cout << "Output file " << file_name << "output created." << endl; 
    } else {
        cout << "Unable to open file" << endl;
    }

    return 0;
}

void Opcodes_Init (map<string, word>& opcodes) {
    opcodes.insert (pair<string, word> ("LDFM", 0x01));
    opcodes.insert (pair<string, word> ("LDFI", 0x02));
    opcodes.insert (pair<string, word> ("LDFR", 0x03));
    opcodes.insert (pair<string, word> ("STOR", 0x04));
    opcodes.insert (pair<string, word> ("SWPR", 0x05));
    opcodes.insert (pair<string, word> ("SWPM", 0x06));
    opcodes.insert (pair<string, word> ("PULR", 0x07));
    opcodes.insert (pair<string, word> ("PSHR", 0x08));
    opcodes.insert (pair<string, word> ("PSHM", 0x09));
    opcodes.insert (pair<string, word> ("PULC", 0x0A));
    opcodes.insert (pair<string, word> ("PSHC", 0x0B));
    opcodes.insert (pair<string, word> ("PULF", 0x0C));
    opcodes.insert (pair<string, word> ("PSHF", 0x0D));
    opcodes.insert (pair<string, word> ("AND", 0x0E));
    opcodes.insert (pair<string, word> ("EOR", 0x0F));
    opcodes.insert (pair<string, word> ("ORA", 0x10));
    opcodes.insert (pair<string, word> ("NOT", 0x11));
    opcodes.insert (pair<string, word> ("ADD", 0x12));
    opcodes.insert (pair<string, word> ("SUB", 0x13));
    // opcodes.insert (pair<string, word> ("MULT", 0x14));
    // opcodes.insert (pair<string, word> ("DIV", 0x15));
    opcodes.insert (pair<string, word> ("CMPR", 0x16));
    opcodes.insert (pair<string, word> ("CMPM", 0x17));
    opcodes.insert (pair<string, word> ("CMPI", 0x18));
    opcodes.insert (pair<string, word> ("INCR", 0x19));
    opcodes.insert (pair<string, word> ("INCM", 0x1A));
    opcodes.insert (pair<string, word> ("DECR", 0x1B));
    opcodes.insert (pair<string, word> ("DECM", 0x1C));
    opcodes.insert (pair<string, word> ("SHLR", 0x1D));
    opcodes.insert (pair<string, word> ("SHRR", 0x1E));
    opcodes.insert (pair<string, word> ("SHLM", 0x1F));
    opcodes.insert (pair<string, word> ("SHRM", 0x20));
    opcodes.insert (pair<string, word> ("JMP", 0x21));
    opcodes.insert (pair<string, word> ("JSR", 0x22));
    opcodes.insert (pair<string, word> ("RSR", 0x23));
    opcodes.insert (pair<string, word> ("JSC", 0x24));
    opcodes.insert (pair<string, word> ("JSS", 0x25));
    opcodes.insert (pair<string, word> ("JCC", 0x26));
    opcodes.insert (pair<string, word> ("JCS", 0x27));
    opcodes.insert (pair<string, word> ("JOC", 0x28));
    opcodes.insert (pair<string, word> ("JOS", 0x29));
    opcodes.insert (pair<string, word> ("JZC", 0x2A));
    opcodes.insert (pair<string, word> ("JZS", 0x2B));
    opcodes.insert (pair<string, word> ("CCF", 0x2C));
    opcodes.insert (pair<string, word> ("SCF", 0x2D));
    opcodes.insert (pair<string, word> ("CSF", 0x2E));
    opcodes.insert (pair<string, word> ("SSF", 0x2F));
    opcodes.insert (pair<string, word> ("COF", 0x30));
    opcodes.insert (pair<string, word> ("SOF", 0x31));
    opcodes.insert (pair<string, word> ("CZF", 0x32));
    opcodes.insert (pair<string, word> ("SZF", 0x33));
    opcodes.insert (pair<string, word> ("NOP", 0x34));
    opcodes.insert (pair<string, word> ("HALT", 0x35));
    opcodes.insert (pair<string, word> ("STRT", 0x37));
}

void Registers_Init (map<char, word>& registers) {
    registers.insert (pair<char, word> ('A', 0x00));
    registers.insert (pair<char, word> ('B', 0x01));
    registers.insert (pair<char, word> ('C', 0x02));
    registers.insert (pair<char, word> ('D', 0x03));
    registers.insert (pair<char, word> ('E', 0x04));
    registers.insert (pair<char, word> ('F', 0x05));
    registers.insert (pair<char, word> ('G', 0x06));
    registers.insert (pair<char, word> ('H', 0x07));
}

void Remove_Space_and_Comments (u32& idx, string& line) {
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
}

bool Opcode_and_Operands_Match (word opcode, string op1, string op2, u32 line_number) {
    // If a no-operand opcode has at least 1 operand, throw error
    if (NEEDS_NO_OPERANDS (opcode) && op1.length() != 0) {
        cout << line_number << " : Opcode " << htos(opcode) << " does not need 1 or more operands." << endl;
        return false;
    }

    // If a one-operand opcode has no operands, throw error
    if (NEEDS_ONE_OPERAND (opcode) && op1.length() == 0) {
        cout << line_number << " : Opcode " << htos(opcode) << " needs 1 operand." << endl;
        return false;  
    }

    // If a one-operand opcode has more that 1 operand, throw error
    if (NEEDS_ONE_OPERAND (opcode) && op2.length() != 0) {
        cout << line_number << " : Opcode " << htos(opcode) << " does not need 2 opcodes." << endl;
        return false;
    }

    // If a two-operand opcode has less than 2 operands, throw error
    if (NEEDS_TWO_OPERANDS (opcode) && (op2.length() == 0 || op1.length() == 0)) {
        cout << line_number << " : Opcode " << htos(opcode) << " needs 2 opcodes." << endl;\
        return false;
    }

    return true;
}

string htos (byte value) {
    string hex_val;

    hex_val = STRING_FROM_HEX ((int) (value) % 16);
    hex_val = to_string ((int) (value) / 16) + hex_val;

    return hex_val;
}
