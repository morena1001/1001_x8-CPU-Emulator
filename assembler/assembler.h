#include <stdint.h>

#define NEEDS_NO_OPERANDS(opcode)       ((opcode >= 0x0A && opcode <= 0x0D) || opcode == 0x23 || (opcode >= 0x2C && opcode <= 0x35) || opcode == 0x37) 
#define NEEDS_ONE_OPERAND(opcode)       ((opcode >= 0x07 && opcode <= 0x09) || opcode == 0x11 || (opcode >= 0x19 && opcode <= 0x22) || (opcode >= 0x24 && opcode <= 0x2B) || opcode == 0x3B || (opcode >= 0x3D && opcode <= 0x40))
#define NEEDS_TWO_OPERANDS(opcode)      ((opcode >= 0x01 && opcode <= 0x06) || (opcode >= 0x0E && opcode <= 0x10) || (opcode >= 0x12 && opcode <= 0x18) || (opcode >= 0x38 && opcode <= 0x3A) || opcode == 0x3C)
#define IS_A_JMP_OPERAND(opcode)        ((opcode >= 0x21 && opcode <= 0x22) || (opcode >= 0x24 && opcode <= 0x2B))

#define REG_AS_OPERAND1(opcode)         ((opcode >= 0x01 && opcode <= 0x03) || opcode == 0x05 || opcode == 0x07 || opcode == 0x08 || (opcode >= 0x0E && opcode <= 0x19) || opcode == 0x1B || opcode == 0x1D || opcode == 0x1E || opcode == 0x38 || opcode == 0x3C)
#define MEM_AS_OPERAND1(opcode)         (opcode == 0x04 || opcode == 0x06 || opcode == 0x09 || opcode == 0x1A || opcode == 0x1C || opcode == 0x1F || opcode == 0x20 || (opcode >= 0x39 && opcode <= 0x3B) || (opcode >= 0x3D && opcode <= 0x40))
#define LAB_AS_OPERAND1(opcode)         (opcode == 0x21 || (opcode >= 0x24 && opcode <= 0x2B))
#define SBR_AS_OPERAND1(opcode)         (opcode == 0x22)

#define REG_AS_OPERAND2(opcode)         (opcode == 0x03 || opcode == 0x04 || opcode == 0x05 || opcode == 0x0E || opcode == 0x0F || opcode == 0x10 || (opcode >= 0x12 && opcode <= 0x16) || opcode == 0x39)
#define MEM_AS_OPERAND2(opcode)         (opcode == 0x01 || opcode == 0x06 || opcode == 0x17 || opcode == 0x38 || opcode == 0x3A || opcode == 0x3C)
#define IMM_AS_OPERAND2(opcode)         (opcode == 0x02 || opcode == 0x18) 

#define LABEL_ENCODING                  0x36
#define START_ENCODING                  0x37
#define SBR_ENCODING                    0x41

#define STRING_FROM_HEX(value)          (value == 10 ? "A" : (value == 11 ? "B" : (value == 12 ? "C" : (value == 13 ? "D" : (value == 14 ? "E" : (value == 15 ? "F" : to_string (value)))))))

// Data types used in 1001_x8
using byte = uint8_t;
using word = unsigned short;
using u32 = unsigned int;

struct label_t {
    word id;
    bool initialized;
};

struct subr_t {
    word id;
    bool initialized;
};


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
