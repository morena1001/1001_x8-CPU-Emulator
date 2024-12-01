#include <iostream>
#include <SDL2/SDL.h>

#include "screen.h"

using namespace std;

int main (int argc, char** argv) {
    // Initialize window, renderer, screen info and input struct.

    SDL_Init (SDL_INIT_EVERYTHING);
    SDL_Window* window;
    SDL_Renderer* renderer;
    screen_info_t info;
    input_info_t input_char;
    input_info_t curr_pos_char;

    // Set Point Size and Pixel Size.
    info.point_size = 3;
    info.size = _40x24;
    
    // Set window and renderer.
    SDL_CreateWindowAndRenderer (WIDTH (info.size, info.point_size), HEIGHT (info.size, info.point_size), 0, &window, &renderer);

    // Set the background color.
    SDL_SetRenderDrawColor (renderer, 50, 48, 47, 255);
    SDL_RenderClear (renderer);
    // Set the color for the characters being drawn.
    SDL_SetRenderDrawColor (renderer, 224, 158, 42, 255);

    // Set window and screen height and width.
    SDL_GetWindowSizeInPixels (window, &info.window_width, &info.window_height);
    info.total_char_width = (CHARACTER_WIDTH + 1) * info.point_size;
    info.total_char_height = (CHARACTER_HEIGHT + 1) * info.point_size;

    // Set up the blinking cursor.
    int time = INT32_MAX / 1750;
    bool cursor_blink = true;
    curr_pos_char.character = '_';
    curr_pos_char.control = None;
    Draw_Character (renderer, info, curr_pos_char);

    // store the keyboard input.
    string keyname;
    uint16_t mod;

    SDL_Event event;
    while (1) {
        time--;
        // Blink the cursor from the underscore characters and the current character the cursor is at.
        if (time == 0) {
            time = INT32_MAX / 1750;
            cursor_blink ^= 1;

            if (cursor_blink) {
                curr_pos_char.character = '_';
                Draw_Character (renderer, info, curr_pos_char);
            } else {
                curr_pos_char.character = info.characters_in_screen[info.cursor_x][info.cursor_y];
                Draw_Character (renderer, info, curr_pos_char);
            }
        }

        if (SDL_PollEvent (&event)) {                
            switch (event.type) {
                case SDL_KEYDOWN : {
                    keyname = SDL_GetKeyName (event.key.keysym.sym);
                    mod = event.key.keysym.mod;
                    
                    input_char.character = !IS_CONTROL_KEYNAME (keyname) ? (keyname == "Space" ? ' ' : keyname[0]) : '\0';
                    input_char.control = String_To_Control (keyname, mod);
                    
                    if (!IS_CONTROL_KEYNAME (keyname) && (IS_SHIFT_CONTROL (input_char.control) || input_char.control == None)) {
                        Draw_Character (renderer, info, input_char);
                        info.characters_in_screen[info.cursor_x][info.cursor_y] = input_char.character;
                        info.column_of_line[info.cursor_y]++;
                        Move_Cursor_Right (info);
                    } else {
                        Execute_Control (renderer, info, input_char);
                    }
                } break;
                case SDL_KEYUP : {
                    keyname = SDL_GetKeyName (event.key.keysym.sym);
                    if (IS_CONTROL_KEYNAME (keyname))       input_char.control = None;
                    else                                    input_char.character = '\0';
                } break;
                case SDL_QUIT : {
                    cout << "DONE" << endl;
                    goto endloop;
                } break;
                default : {} break;
            }
        }
    }

    endloop:
    SDL_DestroyWindow (window);
    SDL_Quit ();

}

void Execute_Control (SDL_Renderer* renderer, screen_info_t& info, input_info_t& input_char) {
    // Don't know why it works, but it does.
    char temp = input_char.character;
    input_char.character = ' ';
    Draw_Character (renderer, info, input_char);
    
    if (input_char.control == Delete) {
        Shift_Characters_Left (renderer, info, input_char);
        info.column_of_line[info.cursor_y]--;
    } else if (input_char.control == Backspace) {      
        Shift_Characters_Left (renderer, info, input_char);
        Move_Cursor_Left (info);
        info.column_of_line[info.cursor_y]--;
    } else if (input_char.control == Return) {
        Move_Cursor_Down (info);
        info.pos_x = 1;
        info.cursor_x = 0;
        // Shift all characters down.
    } else if (input_char.control == Tab) {
        for (int i = 0; i < 5; i++) {
            Move_Cursor_Left (info);
            // Shift all characters right.
        }
    } else if (input_char.control == Escape) {
        // DOES NOTHING YET
    } else if (IS_CTRL_CONTROL (input_char.control) && input_char.character != '\0') {
        input_char.character = info.characters_in_screen[info.cursor_x][info.cursor_y];
        Draw_Character (renderer, info, input_char);
        input_char.character = temp;

        if      (input_char.character == 'W')       Move_Cursor_Up (info);
        else if (input_char.character == 'A')       Move_Cursor_Left (info);
        else if (input_char.character == 'S')       Move_Cursor_Down (info);
        else if (input_char.character == 'D')       Move_Cursor_Right (info);
    } else if (IS_SHIFT_AND_CONTROL (input_char.control) && input_char.character != '\0') {
        input_char.character = info.characters_in_screen[info.cursor_x][info.cursor_y];
        Draw_Character (renderer, info, input_char);
        input_char.character = temp;
        if (input_char.character == 'A') {
            while (info.cursor_x != 0) {
                Move_Cursor_Left (info);
            }
        }
        if (input_char.character == 'D') {
            for (int i = 0; i < info.column_of_line[info.cursor_y]; i++) {
                Move_Cursor_Right (info);
            }
        }
    }
}

void Draw_Character (SDL_Renderer* renderer, screen_info_t& info, input_info_t& input_char) {
    int i = Character_Index (input_char);

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

void Move_Cursor_Right (screen_info_t& info) {
    // If cursor is not at left edge, move cursor
    if (info.cursor_x < (info.size == _40x24 ? 39 : 79) && info.cursor_x < info.column_of_line[info.cursor_y]) {
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

void Move_Cursor_Left (screen_info_t& info) {
    // If cursor is not at the right edge, move cursor
    if (info.cursor_x > 0) {
        info.cursor_x--;
        info.pos_x -= info.total_char_width - 1;
    }
    // Else move cursor up 1 if not at the top edge
    else if (info.cursor_y > 0) {
        uint8_t max_width = info.size == _40x24 ? 39 : 79;
        info.cursor_x = (info.column_of_line[info.cursor_y - 1] < max_width ? info.column_of_line[info.cursor_y - 1] : max_width);
        info.pos_x = 1 + ((info.total_char_width - 1) * info.column_of_line[info.cursor_y - 1]);
        // info.pos_x = info.window_width - info.total_char_width + 1;
        Move_Cursor_Up (info);
    }
}

void Move_Cursor_Up (screen_info_t& info) {
    // If cursor is not at the top edge, move cursor
    if (info.cursor_y > 0) {
        info.cursor_y--;
        info.pos_y -= info.total_char_height - 1;

        if (info.column_of_line[info.cursor_y] < info.column_of_line[info.cursor_y + 1]) {
            info.cursor_x = info.column_of_line[info.cursor_y];
            info.pos_x = 1 + ((info.total_char_width - 1) * info.column_of_line[info.cursor_y]); 
        } 
    }
}

void Move_Cursor_Down (screen_info_t& info) {
    // If cursor is not at bottom edge, move cursor
    if (info.cursor_y < (info.size == _40x24 ? 23 : 47)) {
        info.cursor_y++;
        info.pos_y += info.total_char_height - 1;

        if (info.column_of_line[info.cursor_y] < info.column_of_line[info.cursor_y - 1]) {
            info.cursor_x = info.column_of_line[info.cursor_y];
            info.pos_x = 1 + ((info.total_char_width - 1) * info.column_of_line[info.cursor_y]); 
        }
    }
}

void Shift_Characters_Left (SDL_Renderer* renderer, screen_info_t& info, input_info_t& input_char) {
    // temporary store the values of the pixel position and cursor, as well as the character
    int16_t pos_x_temp = info.pos_x;
    int16_t pos_y_temp = info.pos_y;
    uint16_t cursor_x_temp = info.cursor_x;
    uint16_t cursor_y_temp = info.cursor_y;
    char temp_character = input_char.character;

    for (info.cursor_x; info.cursor_x < info.column_of_line[info.cursor_y]; Move_Cursor_Right (info)) {
        info.characters_in_screen[info.cursor_x][info.cursor_y] = info.characters_in_screen[info.cursor_x + 1][info.cursor_y];
        input_char.character = info.characters_in_screen[info.cursor_x][info.cursor_y];
        Draw_Character (renderer, info, input_char);
    }

    info.pos_x = pos_x_temp;
    info.pos_y = pos_y_temp;
    info.cursor_x = cursor_x_temp;
    info.cursor_y = cursor_y_temp;
    input_char.character = temp_character;
}

void Shift_Characters_Right (SDL_Renderer* renderer, screen_info_t& info, input_info_t& input_char) {

}

void Shift_Characters_Up (SDL_Renderer* renderer, screen_info_t& info, input_info_t& input_char) {

}

void Shift_Characters_Down (SDL_Renderer* renderer, screen_info_t& info, input_info_t& input_char) {
    
}

enum control_sequence String_To_Control (string keyname, uint16_t mod) {
    if (mod == 1)           return Left_Shift;
    if (mod == 2)           return Right_Shift;
    if (mod == 3)           return Both_Shift;

    if (mod == 64)          return Left_Ctrl;
    if (mod == 128)         return Right_Ctrl;
    if (mod == 192)         return Both_Ctrl;

    if (mod == 65)          return Left_Shift_and_Left_Ctrl;
    if (mod == 66)          return Right_Shift_and_Left_Ctrl;
    if (mod == 67)          return Both_Shift_and_Left_Ctrl;

    if (mod == 129)         return Left_Shift_and_Right_Ctrl;
    if (mod == 130)         return Right_Shift_and_Right_Ctrl;
    if (mod == 131)         return Both_Shift_and_Right_Ctrl;
    
    if (mod == 193)         return Left_Shift_and_Both_Ctrl;
    if (mod == 194)         return Right_Shift_and_Both_Ctrl;
    if (mod == 195)         return Both_Shift_and_Both_Ctrl;
    
    if (keyname == control_keynames[0])     return Delete;
    if (keyname == control_keynames[1])     return Backspace;
    if (keyname == control_keynames[2])     return Return;
    if (keyname == control_keynames[3])     return Tab;
    if (keyname == control_keynames[4])     return Left_Shift;
    if (keyname == control_keynames[5])     return Right_Shift;
    if (keyname == control_keynames[6])     return Left_Ctrl;
    if (keyname == control_keynames[7])     return Right_Ctrl;
    if (keyname == control_keynames[8])     return Escape;
}

uint8_t Character_Index (input_info_t& input_char) {
    if (input_char.character == '\0')     return char_space_idx;

    int ascii = (int)  input_char.character;

    // NUMBERS
    if (ascii >= 48 && ascii <= 57) {
        return ascii - (IS_SHIFT_CONTROL(input_char.control) ? (ascii == 48 ? 3 : 13) : 48);
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
        case 39 : return char_apostR_idx - (IS_SHIFT_CONTROL(input_char.control) ? 1 : 0);
        case 40 : return char_brcL_idx;
        case 41 : return char_brcR_idx;
        case 42 : return char_ast_idx;
        case 43 : return char_plus_idx;
        case 44 : return char_comma_idx - (IS_SHIFT_CONTROL(input_char.control) ? 1 : 0);
        case 45 : return char_dash_idx + (IS_SHIFT_CONTROL(input_char.control) ? 1 : 0);
        case 46 : return char_period_idx - (IS_SHIFT_CONTROL(input_char.control) ? 1 : 0);
        case 47 : return char_fwdslsh_idx - (IS_SHIFT_CONTROL(input_char.control) ? 1 : 0);
        // CASES 48 TO 57 ARE NUMBERS
        case 58 : return char_colon_idx;
        case 59 : return char_smcol_idx - (IS_SHIFT_CONTROL(input_char.control) ? 1 : 0);
        case 60 : return char_angleL_idx;
        case 61 : return char_equal_idx - (IS_SHIFT_CONTROL(input_char.control) ? 1 : 0);
        case 62 : return char_angleR_idx;
        case 63 : return char_quest_idx;
        case 64 : return char_at_idx;
        // CASES 65 TO 90 ARE UPPERCASE LETTERS
        case 91 : return char_brkL_idx - (IS_SHIFT_CONTROL(input_char.control) ? 1 : 0);
        case 92 : return char_bkslsh_idx + (IS_SHIFT_CONTROL(input_char.control) ? 1 : 0);
        case 93 : return char_brkR_idx - (IS_SHIFT_CONTROL(input_char.control) ? 1 : 0);
        case 94 : return char_caret_idx;
        case 95 : return char_under_idx;
        case 96 : return char_apostL_idx - (IS_SHIFT_CONTROL(input_char.control) ? 1 : 0);
        // CASES 97 TO 122 ARE LOWERCASE LETTERS
        case 123 : return char_curL_idx;
        case 124 : return char_vert_idx;
        case 125 : return char_curR_idx;
        case 126 : return char_tilde_idx;

        default  : return UINT8_MAX;
    }
}


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