#include <iostream>
#include <SDL2/SDL.h>

#include "screen.h"

using namespace std;

int main (int argc, char** argv) {
    SDL_Init (SDL_INIT_EVERYTHING);
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    SDL_CreateWindowAndRenderer (WIDTH, HEIGHT, 0, &window, &renderer);

    SDL_SetRenderDrawColor (renderer, 50, 48, 47, 255);
    SDL_RenderClear (renderer);
    SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);

    uint8_t char_pos_y = 1, char_pos_x = 1;

    for (int i = 0; i < 69; i++) {
        for (int j = 0; j < 7; j++) {
            for (int k = 0; k < 5; k++) {
                if ((characters[i][j] >> k) & 1) 
                    SDL_RenderDrawPoint (renderer, k + char_pos_x, j + char_pos_y);
            }
        }
        char_pos_x += 7;
        SDL_RenderPresent (renderer);
    }
    
    SDL_RenderPresent (renderer);

    // for (int i = 0; i < 7; i++) {
    //     for (int j = 0; j < 5; j++) {
    //         if ((characters[char_A_idx][i] >> j) & 1)
    //             SDL_RenderDrawPoint (renderer, j + char_pos_x, i + char_pos_y);
    //     }
    // }
    // char_pos_x += 6;
    // SDL_RenderPresent (renderer);

    // for (int i = 0; i < 7; i++) {
    //     for (int j = 0; j < 5; j++) {
    //         if ((characters[char_B_idx][i] >> j) & 1)
    //             SDL_RenderDrawPoint (renderer, j + char_pos_x, i + char_pos_y);
    //     }
    // }
    // char_pos_x +=6;
    // SDL_RenderPresent (renderer);

    // for (int i = 0; i < 7; i++) {
    //     for (int j = 0; j < 5; j++) {
    //         if ((characters[char_C_idx][i] >> j) & 1)
    //             SDL_RenderDrawPoint (renderer, j + char_pos_x, i + char_pos_y);
    //     }
    // }
    // char_pos_x +=6;
    // SDL_RenderPresent (renderer);
    


    SDL_Event windowEvent;
    while (1) {
        if (SDL_PollEvent (&windowEvent)) {
            if (windowEvent.type == SDL_QUIT)       break;
                

        }
    }

    SDL_DestroyWindow (window);
    SDL_Quit ();

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