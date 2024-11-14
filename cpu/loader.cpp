#include <iostream>
#include <string>
#include <map>
#include <fstream>

#include "loader.h"
#include "1001_x8.h"

using namespace std;

word Load_Program (string file_path) {
    ifstream output_file;

    string program;
    map<word, word> headers;
    map<word, word> variables;

    output_file.open (file_path);

    if (output_file.is_open ()) {
        while (getline (output_file, program)) {
            string value;

            // int idx = program.find_first_of (' ');
            value = program.substr (0, 3);
            while (1) {
                cout << value << " ";
                program.erase (0, 3);

                if (program == "") {
                    break;
                }
                value = program.substr (0, 3);
            }
            // cout << program << endl;
        }

    }  else {
        cout << "Could not open " << file_path << "." << endl; 
        return 0;
    }
}
