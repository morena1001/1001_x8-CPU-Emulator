#include <iostream>

using namespace std;

enum pixel_size {
    _40x24,
    _80x48
};

enum control_sequence {
    None,
    Delete,
    Backspace,
    Return,
    Tab,
    Left_Shift,
    Right_Shift,
    Left_Ctrl,
    Right_Ctrl,
    Escape,
    Both_Shift,
    Both_Ctrl
};

typedef struct screen_info {
    int16_t pos_x = 1, pos_y = 1;
    uint16_t cursor_x = 0, cursor_y = 0;
    int window_width = 0, window_height = 0;
    uint8_t total_char_width, total_char_height;
    uint8_t point_size = 1;
    enum pixel_size size = _40x24;
} screen_info_t;

typedef struct input_info {
    char character;
    enum control_sequence control = None;
} input_info_t;

uint8_t Character_Index (input_info_t& input_char);

void Draw_Character (SDL_Renderer* renderer, screen_info_t& info, input_info_t& input_char);
void Move_Cursor_Left (screen_info_t& info);
void Move_Cursor_Right (screen_info_t& info);
void Move_Cursor_Up (screen_info_t& info);
void Move_Cursor_Down (screen_info_t& info);

enum control_sequence String_To_Control (string keyname, uint16_t mod);
void Execute_Control (SDL_Renderer* renderer, screen_info_t& info, input_info_t& input_char);




// Screen size of 40 * 24 pixels
#define WIDTH_1_POINT_4024          281
#define HEIGHT_1_POINT_4024         217

#define WIDTH_2_POINT_4024          481
#define HEIGHT_2_POINT_4024         385

#define WIDTH_3_POINT_4024          681
#define HEIGHT_3_POINT_4024         553

#define POINT_SIZE_4024(height)     (((height / 24) - 2) / 7)

// Screen size of 80 * 48 pixels
#define WIDTH_1_POINT_8048          561
#define HEIGHT_1_POINT_8048         433

#define WIDTH_2_POINT_8048          961
#define HEIGHT_2_POINT_8048         769

#define WIDTH_3_POINT_8048          1361
#define HEIGHT_3_POINT_8048         1105

#define POINT_SIZE_8048(height)     (((height / 48) - 2) / 7)

#define WIDTH(pixels, point)        (pixels == _40x24 ? (point == 1 ? WIDTH_1_POINT_4024 : (point == 2 ? WIDTH_2_POINT_4024 : (point == 3 ? WIDTH_3_POINT_4024 : WIDTH_1_POINT_4024))) : ((point == 1 ? WIDTH_1_POINT_8048 : (point == 2 ? WIDTH_2_POINT_8048 : (point == 3 ? WIDTH_3_POINT_8048 : WIDTH_3_POINT_8048)))))
#define HEIGHT(pixels, point)       (pixels == _40x24 ? (point == 1 ? HEIGHT_1_POINT_4024 : (point == 2 ? HEIGHT_2_POINT_4024 : (point == 3 ? HEIGHT_3_POINT_4024 : HEIGHT_1_POINT_4024))) : ((point == 1 ? HEIGHT_1_POINT_8048 : (point == 2 ? HEIGHT_2_POINT_8048 : (point == 3 ? HEIGHT_3_POINT_8048 : HEIGHT_3_POINT_8048)))))



#define CHARACTER_HEIGHT    7
#define CHARACTER_WIDTH     5

#define char_0_idx          0
#define char_1_idx          1
#define char_2_idx          2
#define char_3_idx          3
#define char_4_idx          4
#define char_5_idx          5
#define char_6_idx          6
#define char_7_idx          7
#define char_8_idx          8
#define char_9_idx          9
#define char_A_idx          10
#define char_B_idx          11
#define char_C_idx          12
#define char_D_idx          13
#define char_E_idx          14
#define char_F_idx          15
#define char_G_idx          16
#define char_H_idx          17
#define char_I_idx          18
#define char_J_idx          19
#define char_K_idx          20
#define char_L_idx          21
#define char_M_idx          22
#define char_N_idx          23
#define char_O_idx          24
#define char_P_idx          25
#define char_Q_idx          26
#define char_R_idx          27
#define char_S_idx          28
#define char_T_idx          29
#define char_U_idx          30
#define char_V_idx          31
#define char_W_idx          32
#define char_X_idx          33
#define char_Y_idx          34
#define char_Z_idx          35
#define char_excl_idx       36
#define char_at_idx         37 
#define char_hash_idx       38
#define char_dollar_idx     39
#define char_percent_idx    40
#define char_caret_idx      41
#define char_amp_idx        42
#define char_ast_idx        43
#define char_brcL_idx       44
#define char_brcR_idx       45
#define char_dash_idx       46
#define char_under_idx      47
#define char_plus_idx       48
#define char_equal_idx      49
#define char_curL_idx       50
#define char_brkL_idx       51
#define char_curR_idx       52
#define char_brkR_idx       53
#define char_bkslsh_idx     54
#define char_vert_idx       55
#define char_colon_idx      56
#define char_smcol_idx      57
#define char_quote_idx      58
#define char_apostR_idx     59
#define char_angleL_idx     60
#define char_comma_idx      61
#define char_angleR_idx     62
#define char_period_idx     63
#define char_quest_idx      64
#define char_fwdslsh_idx    65
#define char_tilde_idx      66
#define char_apostL_idx     67
#define char_space_idx      68

#define IS_CONTROL_KEYNAME(keyname)         (keyname == control_keynames[0] || keyname == control_keynames[1] || keyname == control_keynames[2] || keyname == control_keynames[3] || keyname == control_keynames[4] || keyname == control_keynames[5] || keyname == control_keynames[6] || keyname == control_keynames[7] || keyname == control_keynames[8])
#define IS_SHIFT_CONTROL(control)           (control == Left_Shift || control == Right_Shift || control == Both_Shift)
#define IS_CTRL_CONTROL(control)            (control == Left_Ctrl || control == Left_Ctrl || control == Both_Ctrl)

// Control keynames
string control_keynames[9] = {
    "Delete",
    "Backspace",
    "Return",
    "Tab",
    "Left Shift",
    "Right Shift",
    "Left Ctrl",
    "Right Ctrl",
    "Escape"
};


// Initialization of all character bitmaps
uint8_t characters[69][7] = {
    { // 0
        0b01110,
        0b10001,
        0b10011,
        0b10101,
        0b11001,
        0b10001,
        0b01110,
    }, { // 1
        0b00100,
        0b00110,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b01110,        
    }, { // 2
       0b01110,
        0b10001,
        0b10000,
        0b01100,
        0b00010,
        0b00001,
        0b11111 
    }, { // 3
        0b01110,
        0b10001,
        0b10000,
        0b01110,
        0b10000,
        0b10001,
        0b01110
    }, { // 4
        0b01000,
        0b01100,
        0b01010,
        0b01001,
        0b11111,
        0b01000,
        0b01000
    }, { // 5
        0b11111,
        0b00001,
        0b01111,
        0b10000,
        0b10000,
        0b10001,
        0b01110
    }, { // 6
        0b11100,
        0b00010,
        0b00001,
        0b01111,
        0b10001,
        0b10001,
        0b01110
    }, { // 7
        0b11111,
        0b10000,
        0b01000,
        0b00100,
        0b00010,
        0b00010,
        0b00010
    }, { // 8
        0b01110,
        0b10001,
        0b10001,
        0b01110,
        0b10001,
        0b10001,
        0b01110
    }, { // 9
        0b01110,
        0b10001,
        0b10001,
        0b11110,
        0b10000,
        0b01000,
        0b00111
    }, { // A
        0b00100,
        0b01010,
        0b10001,
        0b10001,
        0b11111,
        0b10001,
        0b10001
    }, { // B
        0b01111,
        0b10001,
        0b10001,
        0b01111,
        0b10001,
        0b10001,
        0b01111
    }, { // C
        0b01110,
        0b10001,
        0b00001,
        0b00001,
        0b00001,
        0b10001,
        0b01110
    }, { // D
        0b01111,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b01111
    }, { // E
        0b11111,
        0b00001,
        0b00001,
        0b01111,
        0b00001,
        0b00001,
        0b11111
    }, { // F
        0b11111,
        0b00001,
        0b00001,
        0b01111,
        0b00001,
        0b00001,
        0b00001
    }, { // G
        0b01110,
        0b10001,
        0b00001,
        0b00001,
        0b11101,
        0b10001,
        0b01110
    }, { // H
        0b10001,
        0b10001,
        0b10001,
        0b11111,
        0b10001,
        0b10001,
        0b10001
    }, { // I
        0b01110,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b01110
    }, { // J
        0b10000,
        0b10000,
        0b10000,
        0b10000,
        0b10000,
        0b10001,
        0b01110
    }, { // K
        0b10001,
        0b01001,
        0b00101,
        0b00011,
        0b00101,
        0b01001,
        0b10001
    }, { // L
        0b00001,
        0b00001,
        0b00001,
        0b00001,
        0b00001,
        0b00001,
        0b11111
    }, { // M
        0b10001,
        0b11011,
        0b10101,
        0b10101,
        0b10001,
        0b10001,
        0b10001
    }, { // N
        0b10001,
        0b10001,
        0b10011,
        0b10101,
        0b11001,
        0b10001,
        0b10001
    }, { // O
        0b01110,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b01110
    }, { // P
        0b01111,
        0b10001,
        0b10001,
        0b01111,
        0b00001,
        0b00001,
        0b00001
    }, { // Q
        0b01110,
        0b10001,
        0b10001,
        0b10001,
        0b10101,
        0b01001,
        0b10110
    }, { // R
        0b01111,
        0b10001,
        0b10001,
        0b01111,
        0b00101,
        0b01001,
        0b10001
    }, { // S
        0b01110,
        0b10001,
        0b00001,
        0b01110,
        0b10000,
        0b10001,
        0b01110
    }, { // T
        0b11111,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100
    }, { // U
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b01110
    }, { // V
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b01010,
        0b00100
    }, { // W
        0b10001,
        0b10001,
        0b10001,
        0b10101,
        0b10101,
        0b11011,
        0b10001
    }, { // X
        0b10001,
        0b10001,
        0b01010,
        0b00100,
        0b01010,
        0b10001,
        0b10001
    } , { // Y
        0b10001,
        0b10001,
        0b01010,
        0b00100,
        0b00100,
        0b00100,
        0b00100
    }, { // Z
        0b11111,
        0b10000,
        0b01000,
        0b00100,
        0b00010,
        0b00001,
        0b11111
    }, { // !
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00000,
        0b00100
    }, { // @
        0b01110,
        0b10001,
        0b10101,
        0b11101,
        0b01101,
        0b00001,
        0b11110
    }, { // #
        0b01010,
        0b01010,
        0b11111,
        0b01010,
        0b11111,
        0b01010,
        0b01010
    }, { // $
        0b00100,
        0b11110,
        0b00101,
        0b01110,
        0b10100,
        0b01111,
        0b00100
    }, { // %
        0b00011,
        0b10011,
        0b01000,
        0b00100,
        0b00010,
        0b11001,
        0b11000
    }, { // ^
        0b00000,
        0b00000,
        0b00100,
        0b01010,
        0b10001,
        0b00000,
        0b00000
    }, { // &
        0b00010,
        0b00101,
        0b00101,
        0b00010,
        0b10101,
        0b01001,
        0b10110
    }, { // *
        0b00100,
        0b10101,
        0b01110,
        0b00100,
        0b01110,
        0b10101,
        0b00100
    }, { // (
        0b00100,
        0b00010,
        0b00001,
        0b00001,
        0b00001,
        0b00010,
        0b00100
    }, { // )
        0b00100,
        0b01000,
        0b10000,
        0b10000,
        0b10000,
        0b01000,
        0b00100
    }, { // -
        0b00000,
        0b00000,
        0b00000,
        0b11111,
        0b00000,
        0b00000,
        0b00000
    }, { // _
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b11111
    }, { // +
        0b00000,
        0b00100,
        0b00100,
        0b11111,
        0b00100,
        0b00100,
        0b00000
    }, { // =
        0b00000,
        0b00000,
        0b11111,
        0b00000,
        0b11111,
        0b00000,
        0b00000
    }, { // {
        0b01100,
        0b00010,
        0b00010,
        0b00001,
        0b00010,
        0b00010,
        0b01100
    }, { // [
        0b00111,
        0b00001,
        0b00001,
        0b00001,
        0b00001,
        0b00001,
        0b00111
    }, { // }
        0b00110,
        0b01000,
        0b01000,
        0b10000,
        0b01000,
        0b01000,
        0b00110
    }, { // ]
        0b11100,
        0b10000,
        0b10000,
        0b10000,
        0b10000,
        0b10000,
        0b11100
    }, { // "\"
        0b00000,
        0b00001,
        0b00010,
        0b00100,
        0b01000,
        0b10000,
        0b00000
    }, { // |
        0b00000,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00000
    }, { // :
        0b00000,
        0b00000,
        0b00100,
        0b00000,
        0b00100,
        0b00000,
        0b00000
    }, { // ;
        0b00000,
        0b00000,
        0b00100,
        0b00000,
        0b00100,
        0b00100,
        0b00010
    }, { // "
        0b01010,
        0b01010,
        0b01010,
        0b00000,
        0b00000,
        0b00000,
        0b00000
    }, { // '
        0b00100,
        0b00100,
        0b00010,
        0b00000,
        0b00000,
        0b00000,
        0b00000
    }, { // <
        0b01000,
        0b00100,
        0b00010,
        0b00001,
        0b00010,
        0b00100,
        0b01000
    }, { // ,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00100,
        0b00100,
        0b00010
    }, { // >
        0b00010,
        0b00100,
        0b01000,
        0b10000,
        0b01000,
        0b00100,
        0b00010
    }, { // .
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00100
    }, { // ?
        0b01110,
        0b10001,
        0b10000,
        0b01000,
        0b00100,
        0b00000,
        0b00100
    }, { // /
        0b00000,
        0b10000,
        0b01000,
        0b00100,
        0b00010,
        0b00001,
        0b00000
    }, { // ~
        0b00000,
        0b00000,
        0b10000,
        0b01110,
        0b00001,
        0b00000,
        0b00000
    }, { // `
        0b00100,
        0b00100,
        0b01000,
        0b00000,
        0b00000,
        0b00000,
        0b00000
    }, { //  
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000
    }
};

// Helper array for mapping character to its position in characters array
char characters_helper[70] = {
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "!@#$%^&*()-_+={[}]\\|:;\"\'<,>.?/~` "
};
