#include <iostream>
#include <fstream>
#include <map>

#include "aux_mem.h"

using namespace std;

void AUX_MEM::Init () {
    ifstream file;
    file.open ("jobs.txt");

    if (file.is_open ()) {
        word prog_id = 0x0001;
        word address = 0x0000;
        
        string file_name;

        while (getline (file, file_name)) {
            Load_Program (file_name, prog_id, address);
        }
    }
}

void AUX_MEM::WriteWord (word value, u32 address) {
    data[address] = value;
}



void AUX_MEM::Load_Program (string file_name, word& prog_id, word& address) {
    ifstream file;
    file.open (file_name);

    if (file.is_open ()) {
        data[address++] = prog_id++;
        word end_address = address++;

        string program;
        string value;
        byte instruction;
        bool high;

        while (getline (file, program)) {
            while (1) {
                if (!Next_Ins (value, program, instruction)) break;
                data[address] = instruction;
                high = false;

                if (!Next_Ins (value, program, instruction)) break;
                data[address++] |= (instruction << 8);
                high = true;
            }
            data[end_address - 1] |= (high ? 0x0000 : 0x1000);
            address += (high ? 0 : 1);
            data[end_address] = address;
        }
    }
}

bool Next_Ins (string& value, string& program, byte& instruction) {
    if (program.empty ())   return false;

    value = program.substr (0, 3);
    value.pop_back ();
    instruction = String_to_Hex (value);
    program.erase (0, 3);

    return true;
}

byte String_to_Hex (string value) {
    byte hex_val;

    if (value.length () != 2)   return 0;
    hex_val = (INT_REPRESENTATION_OF_HEX (value[0]) * 16) + (INT_REPRESENTATION_OF_HEX (value[1]));

    return hex_val;
}
