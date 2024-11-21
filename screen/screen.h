#include <iostream>

using namespace std;

#define WIDTH       402
#define HEIGHT      338

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


// Initialization of all character bitmaps
uint8_t characters[69][7] = {
    {
        0b01110,
        0b10001,
        0b10011,
        0b10101,
        0b11001,
        0b10001,
        0b01110,
    }, {
        0b00100,
        0b00110,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b01110,        
    }, {
       0b01110,
        0b10001,
        0b10000,
        0b01100,
        0b00010,
        0b00001,
        0b11111 
    }, {
        0b01110,
        0b10001,
        0b10000,
        0b01110,
        0b10000,
        0b10001,
        0b01110
    }, {
        0b01000,
        0b01100,
        0b01010,
        0b01001,
        0b11111,
        0b01000,
        0b01000
    }, {
        0b11111,
        0b00001,
        0b01111,
        0b10000,
        0b10000,
        0b10001,
        0b01110
    }, {
        0b11100,
        0b00010,
        0b00001,
        0b01111,
        0b10001,
        0b10001,
        0b01110
    }, {
        0b11111,
        0b10000,
        0b01000,
        0b00100,
        0b00010,
        0b00010,
        0b00010
    }, {
        0b01110,
        0b10001,
        0b10001,
        0b01110,
        0b10001,
        0b10001,
        0b01110
    }, {
        0b01110,
        0b10001,
        0b10001,
        0b11110,
        0b10000,
        0b01000,
        0b00111
    }, {
        0b00100,
        0b01010,
        0b10001,
        0b10001,
        0b11111,
        0b10001,
        0b10001
    }, {
        0b01111,
        0b10001,
        0b10001,
        0b01111,
        0b10001,
        0b10001,
        0b01111
    }, {
        0b01110,
        0b10001,
        0b00001,
        0b00001,
        0b00001,
        0b10001,
        0b01110
    }, {
        0b01111,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b01111
    }, {
        0b11111,
        0b00001,
        0b00001,
        0b01111,
        0b00001,
        0b00001,
        0b11111
    }, {
        0b11111,
        0b00001,
        0b00001,
        0b01111,
        0b00001,
        0b00001,
        0b00001
    }, {
        0b01110,
        0b10001,
        0b00001,
        0b00001,
        0b11101,
        0b10001,
        0b01110
    }, {
        0b10001,
        0b10001,
        0b10001,
        0b11111,
        0b10001,
        0b10001,
        0b10001
    }, {
        0b01110,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b01110
    }, {
        0b10000,
        0b10000,
        0b10000,
        0b10000,
        0b10000,
        0b10001,
        0b01110
    }, {
        0b10001,
        0b01001,
        0b00101,
        0b00011,
        0b00101,
        0b01001,
        0b10001
    }, {
        0b00001,
        0b00001,
        0b00001,
        0b00001,
        0b00001,
        0b00001,
        0b11111
    }, {
        0b10001,
        0b11011,
        0b10101,
        0b10101,
        0b10001,
        0b10001,
        0b10001
    }, {
        0b10001,
        0b10001,
        0b10011,
        0b10101,
        0b11001,
        0b10001,
        0b10001
    }, {
        0b01110,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001
    }, {
        0b01111,
        0b10001,
        0b10001,
        0b01111,
        0b00001,
        0b00001,
        0b00001
    }, {
        0b01110,
        0b10001,
        0b10001,
        0b10001,
        0b10101,
        0b01001,
        0b10110
    }, {
        0b01111,
        0b10001,
        0b10001,
        0b01111,
        0b00101,
        0b01001,
        0b10001
    }, {
        0b01110,
        0b10001,
        0b00001,
        0b01110,
        0b10000,
        0b10001,
        0b01110
    }, {
        0b11111,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100
    }, {
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b01110
    }, {
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b01010,
        0b00100
    }, {
        0b10001,
        0b10001,
        0b10001,
        0b10101,
        0b10101,
        0b11011,
        0b10001
    }, {
        0b10001,
        0b10001,
        0b01010,
        0b00100,
        0b01010,
        0b10001,
        0b10001
    } , {
        0b10001,
        0b10001,
        0b01010,
        0b00100,
        0b00100,
        0b00100,
        0b00100
    }, {
        0b11111,
        0b10000,
        0b01000,
        0b00100,
        0b00010,
        0b00001,
        0b11111
    }, {
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00000,
        0b00100
    }, {
        0b01110,
        0b10001,
        0b10101,
        0b11101,
        0b01101,
        0b00001,
        0b11110
    }, {
        0b01010,
        0b01010,
        0b11111,
        0b01010,
        0b11111,
        0b01010,
        0b01010
    }, {
        0b00100,
        0b11110,
        0b00101,
        0b01110,
        0b10100,
        0b01111,
        0b00100
    }, {
        0b00011,
        0b10011,
        0b01000,
        0b00100,
        0b00010,
        0b11001,
        0b11000
    }, {
        0b00000,
        0b00000,
        0b00100,
        0b01010,
        0b10101,
        0b00000,
        0b00000
    }, {
        0b00010,
        0b00101,
        0b00101,
        0b00010,
        0b10101,
        0b01001,
        0b10110
    }, {
        0b00100,
        0b10101,
        0b01110,
        0b00100,
        0b01110,
        0b10101,
        0b00100
    }, {
        0b00100,
        0b00010,
        0b00001,
        0b00001,
        0b00001,
        0b00010,
        0b00100
    }, {
        0b00100,
        0b01000,
        0b10000,
        0b10000,
        0b10000,
        0b01000,
        0b00100
    }, {
        0b00000,
        0b00000,
        0b00000,
        0b11111,
        0b00000,
        0b00000,
        0b00000
    }, {
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b11111
    }, {
        0b00000,
        0b00100,
        0b00100,
        0b11111,
        0b00100,
        0b00100,
        0b00000
    }, {
        0b00000,
        0b00000,
        0b11111,
        0b00000,
        0b11111,
        0b00000,
        0b00000
    }, {
        0b01100,
        0b00010,
        0b00010,
        0b00001,
        0b00010,
        0b00010,
        0b01100
    }, {
        0b11111,
        0b00011,
        0b00011,
        0b00011,
        0b00011,
        0b00011,
        0b11111
    }, {
        0b00110,
        0b01000,
        0b01000,
        0b10000,
        0b01000,
        0b01000,
        0b00110
    }, {
        0b11111,
        0b11000,
        0b11000,
        0b11000,
        0b11000,
        0b11000,
        0b11111
    }, {
        0b00000,
        0b00001,
        0b00010,
        0b00100,
        0b01000,
        0b10000,
        0b00000
    }, {
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100
    }, {
        0b00000,
        0b00000,
        0b00100,
        0b00000,
        0b00100,
        0b00000,
        0b00000
    }, {
        0b00000,
        0b00000,
        0b00100,
        0b00000,
        0b00100,
        0b00100,
        0b00010
    }, {
        0b01010,
        0b01010,
        0b01010,
        0b00000,
        0b00000,
        0b00000,
        0b00000
    }, {
        0b00100,
        0b00100,
        0b00010,
        0b00000,
        0b00000,
        0b00000,
        0b00000
    }, {
        0b01000,
        0b00100,
        0b00010,
        0b00001,
        0b00010,
        0b00100,
        0b01000
    }, {
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00100,
        0b00100,
        0b00010
    }, {
        0b00010,
        0b00100,
        0b01000,
        0b10000,
        0b01000,
        0b00100,
        0b00010
    }, {
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00100
    }, {
        0b01110,
        0b10001,
        0b10000,
        0b01000,
        0b00100,
        0b00000,
        0b00100
    }, {
        0b00000,
        0b10000,
        0b01000,
        0b00100,
        0b00010,
        0b00001,
        0b00000
    }, {
        0b00000,
        0b00000,
        0b10000,
        0b01110,
        0b00001,
        0b00000,
        0b00000
    }, {
        0b00100,
        0b00100,
        0b01000,
        0b00000,
        0b00000,
        0b00000,
        0b00000
    }, {
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000
    }
};
