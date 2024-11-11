#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;

int main() {
    SetConsoleMode (GetStdHandle (STD_OUTPUT_HANDLE), ENABLE_PROCESSED_OUTPUT);

    bool running = true;
    
    while (running) {
        while (kbhit ()) {
            static char last_ch = 0;
            char ch = getch ();
            switch (last_ch) {
                case 13: cout << int('\n') << endl; break;

                default: cout << int(ch) << endl; break;
            }
        }

        Sleep (10); 
    }

    return 0;
}