#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

const int WIDTH = 800, HEIGHT = 600;

int main (int argc, char** argv) {
    SDL_Init (SDL_INIT_EVERYTHING );
    SDL_Window *window = SDL_CreateWindow ("1001_x8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);

    if (window == NULL) {
        cout << "Could not create window: " << SDL_GetError () << endl;
        return 1;
    }

    SDL_Event windowEvent;

    while (1) {
        if (SDL_PollEvent (&windowEvent)) {
            if (windowEvent.type == SDL_QUIT)       break;


        }
    }

    SDL_DestroyWindow (window);
    SDL_Quit ();

    return 0;
}








// #include <iostream>
// #include <windows.h>
// #include <conio.h>

// using namespace std;

// int main() {
//     SetConsoleMode (GetStdHandle (STD_OUTPUT_HANDLE), 0b111);

//     bool running = true;
    
//     while (running) {
//         while (kbhit ()) {
//             // static char last_ch = 0;
//             char ch = getch ();
//             switch (ch) {
//                 case 13: cout << int('\n') << endl; break;

//                 default: cout << int(ch) << endl; break;
//             }
//         }

//         Sleep (10); 
//     }

//     return 0;
// }