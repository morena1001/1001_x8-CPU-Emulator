#include <iostream>
#include <SDL2/SDL.h>

#include "screen.h"

using namespace std;

int main (int argc, char** argv) {
    SDL_Init (SDL_INIT_EVERYTHING);
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    SDL_CreateWindowAndRenderer (WIDTH_3_POINT_4024, HEIGHT_3_POINT_4024, 0, &window, &renderer);

    SDL_SetRenderDrawColor (renderer, 50, 48, 47, 255);
    SDL_RenderClear (renderer);
    SDL_SetRenderDrawColor (renderer, 224, 158, 42, 255);

    uint16_t char_pos_y = 1, char_pos_x = 1;
    int window_width, window_height;
    SDL_GetWindowSizeInPixels (window, &window_width, &window_height);
    uint8_t point_size = POINT_SIZE_4024 (window_height);
    uint8_t total_char_width  = (CHARACTER_WIDTH + 1) * point_size;
    uint8_t total_char_height = (CHARACTER_HEIGHT + 1) * point_size;

    // for (int i = 0; i < 69; i++) {
    //     for (int j = 0; j < CHARACTER_HEIGHT * point_size; j++) {
    //         for (int k = 0; k < CHARACTER_WIDTH * point_size; k++) {
    //             if ((characters[i][j / point_size] >> (k / point_size)) & 1) {
    //                 SDL_RenderDrawPoint (renderer, k + char_pos_x, j + char_pos_y);
    //             }
    //         }
    //     }

    //     char_pos_x += total_char_width - 1;
    //     if (char_pos_x >= (window_width - 1)) {
    //         char_pos_x = 1;
    //         char_pos_y += total_char_height - 1;
    //     }
        
    //     SDL_RenderPresent (renderer);
    // }



    for (int i = 0; i < 40 * 29; i++) {
        for (int j = 0; j < CHARACTER_HEIGHT * point_size; j++) {
            for (int k = 0; k < CHARACTER_WIDTH * point_size; k++) {
                if ((characters[i % 69][j / point_size] >> (k / point_size)) & 1) {
                    SDL_RenderDrawPoint (renderer, k + char_pos_x, j + char_pos_y);
                }
            }
        }

        char_pos_x += total_char_width - 1;
        if (char_pos_x >= (window_width - 1)) {
            char_pos_x = 1;
            char_pos_y += total_char_height - 1;
        }
        
        SDL_RenderPresent (renderer);
    }



    // for (int i = 0; i < 69; i++) {
    //     for (int j = 0; j < CHARACTER_HEIGHT * point_size; j++) {
    //         for (int k = 0; k < CHARACTER_WIDTH * point_size; k++) {
    //             if ((characters[i][j / point_size] >> (k / point_size)) & 1) {
    //                 SDL_RenderDrawPoint (renderer, k + char_pos_x, j + char_pos_y);
    //             }
    //         }
    //     }

    //     char_pos_y += total_char_height - 1;
    //     if (char_pos_y >= (window_height - 1)) {
    //         char_pos_y = 1;
    //         char_pos_x += total_char_width - 1;
    //     }
        
    //     SDL_RenderPresent (renderer);
    // }



    // for (int i = 0; i < 40 * 29; i++) {
    //     for (int j = 0; j < CHARACTER_HEIGHT * point_size; j++) {
    //         for (int k = 0; k < CHARACTER_WIDTH * point_size; k++) {
    //             if ((characters[i % 69][j / point_size] >> (k / point_size)) & 1) {
    //                 SDL_RenderDrawPoint (renderer, k + char_pos_x, j + char_pos_y);
    //             }
    //         }
    //     }

    //     char_pos_y += total_char_height - 1;
    //     if (char_pos_y >= (window_height - 1)) {
    //         char_pos_y = 1;
    //         char_pos_x += total_char_width - 1;
    //     }
        
    //     SDL_RenderPresent (renderer);
    // }




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