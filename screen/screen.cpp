#include <iostream>
#include <SDL2/SDL.h>

#include "screen.h"

using namespace std;

int main (int argc, char** argv) {
    SDL_Init (SDL_INIT_EVERYTHING);
    SDL_Window* window;
    SDL_Renderer* renderer;
    screen_info_t info;

    // Set Point Size and Pixel Size;
    info.point_size = 3;
    info.size = _40x24;
    
    SDL_CreateWindowAndRenderer (WIDTH (info.size, info.point_size), HEIGHT (info.size, info.point_size), 0, &window, &renderer);

    SDL_SetRenderDrawColor (renderer, 50, 48, 47, 255);
    SDL_RenderClear (renderer);
    SDL_SetRenderDrawColor (renderer, 224, 158, 42, 255);


    SDL_GetWindowSizeInPixels (window, &info.window_width, &info.window_height);
    info.total_char_width = (CHARACTER_WIDTH + 1) * info.point_size;
    info.total_char_height = (CHARACTER_HEIGHT + 1) * info.point_size;

    int time = INT32_MAX / 1750;
    bool cursor_blink = true;
    Draw_Character (renderer, info, '_');


    // for (int i = 0; i < 69; i++) {
    //     Draw_Character (renderer, info, characters_helper[i]);
    //     // Move_Cursor_Down (info);
    //     Move_Cursor_Left (info);
    // }

    // info.pos_x = info.window_width - info.total_char_width + 1;
    // info.pos_y = info.window_height - info.total_char_height + 1;
    // info.cursor_x = 39;
    // info.cursor_y = 23;
    // for (int i = 0; i < 69; i++) {
    //     Draw_Character (renderer, info, characters_helper[i]);
    //     Move_Cursor_Right (info);
    // }

    SDL_Event windowEvent;
    while (1) {
        time--;
        if (time == 0) {
            time = INT32_MAX / 1750;
            cursor_blink ^= 1;

            if (cursor_blink) {
                Draw_Character (renderer, info, '_');
            } else {
                Draw_Character (renderer, info, ' ');
            }
        }

        if (SDL_PollEvent (&windowEvent)) {
            if (windowEvent.type == SDL_QUIT)       break;
                

        }
    }

    SDL_DestroyWindow (window);
    SDL_Quit ();

}

uint8_t Character_Index (char character) {
    int ascii = (int) character;

    // NUMBERS
    if (ascii >= 48 && ascii <= 57) {
        return ascii - 48;
    }

    // CAPITALIZED LETTERS
    if (ascii >= 65 && ascii <= 90) {
        return ascii - 55;
    }

    // LOWERCASE LETTERS
    if (ascii >= 97 && ascii <= 122) {
        return ascii - 87;
    }

    switch (ascii) {
        case 32 : return char_space_idx;
        case 33 : return char_excl_idx;
        case 34 : return char_quote_idx;
        case 35 : return char_hash_idx;
        case 36 : return char_dollar_idx;
        case 37 : return char_percent_idx;
        case 38 : return char_amp_idx;
        case 39 : return char_apostR_idx;
        case 40 : return char_brcL_idx;
        case 41 : return char_brcR_idx;
        case 42 : return char_ast_idx;
        case 43 : return char_plus_idx;
        case 44 : return char_comma_idx;
        case 45 : return char_dash_idx;
        case 46 : return char_period_idx;
        case 47 : return char_fwdslsh_idx;
        // CASES 48 TO 57 ARE NUMBERS
        case 58 : return char_colon_idx;
        case 59 : return char_smcol_idx;
        case 60 : return char_angleL_idx;
        case 61 : return char_equal_idx;
        case 62 : return char_angleR_idx;
        case 63 : return char_quest_idx;
        case 64 : return char_at_idx;
        // CASES 65 TO 90 ARE UPPERCASE LETTERS
        case 91 : return char_brkL_idx;
        case 92 : return char_bkslsh_idx;
        case 93 : return char_brkR_idx;
        case 94 : return char_caret_idx;
        case 95 : return char_under_idx;
        case 96 : return char_apostL_idx;
        // CASES 97 TO 122 ARE LOWERCASE LETTERS
        case 123 : char_curL_idx;
        case 124 : char_vert_idx;
        case 125 : char_curR_idx;
        case 126 : char_tilde_idx;
        default  : return ascii;
    }
}

void Draw_Character (SDL_Renderer* renderer, screen_info_t& info, char character) {
    int i = Character_Index (character);

    for (int j = 0; j < CHARACTER_HEIGHT * info.point_size; j++) {
        for (int k = 0; k < CHARACTER_WIDTH * info.point_size; k++) {
            if ((characters[i % 69][j / info.point_size] >> (k / info.point_size)) & 1) {
                SDL_SetRenderDrawColor (renderer, 224, 158, 42, 255);
                SDL_RenderDrawPoint (renderer, k + info.pos_x, j + info.pos_y);
            } else {
                SDL_SetRenderDrawColor (renderer, 50, 48, 47, 255);
                SDL_RenderDrawPoint (renderer, k + info.pos_x, j + info.pos_y);
            }
        }
    }

    SDL_RenderPresent (renderer);
}

void Move_Cursor_Left (screen_info_t& info) {
    // If cursor is not at left edge, move cursor
    if (info.cursor_x < (info.size == _40x24 ? 39 : 79)) {
        info.cursor_x++;
        info.pos_x += info.total_char_width - 1;
    } 
    // Else move cursor down 1 if not at bottom edge
    else if (info.cursor_y < (info.size == _40x24 ? 23 : 47)) {
        info.cursor_x = 0;
        info.pos_x = 1;
        Move_Cursor_Down (info);
    }
}

void Move_Cursor_Right (screen_info_t& info) {
    // If cursor is not at the right edge, move cursor
    if (info.cursor_x > 0) {
        info.cursor_x--;
        info.pos_x -= info.total_char_width - 1;
    }
    // Else move cursor up 1 if not at the top edge
    else if (info.cursor_y > 0) {
        info.cursor_x = (info.size == _40x24 ? 39 : 79);
        info.pos_x = info.window_width - info.total_char_width + 1;
        Move_Cursor_Up (info);
    }
}

void Move_Cursor_Up (screen_info_t& info) {
    // If cursor is not at the top edge, move cursor
    if (info.cursor_y > 0) {
        info.cursor_y--;
        info.pos_y -= info.total_char_height - 1;
    }
}

void Move_Cursor_Down (screen_info_t& info) {
    // If cursor is not at bottom edge, move cursor
    if (info.cursor_y < (info.size == _40x24 ? 23 : 47)) {
        info.cursor_y++;
        info.pos_y += info.total_char_height - 1;
    }
}



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

    // for (int i = 0; i < 40 * 29; i++) {
    //     for (int j = 0; j < CHARACTER_HEIGHT * point_size; j++) {
    //         for (int k = 0; k < CHARACTER_WIDTH * point_size; k++) {
    //             if ((characters[i % 69][j / point_size] >> (k / point_size)) & 1) {
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