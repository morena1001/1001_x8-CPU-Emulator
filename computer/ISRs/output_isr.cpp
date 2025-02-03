#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "isr.h"

void Print (cpu_t& cpu, mem_t& mem) {
    // Screen Pixel Memory begins at 0xDA5D, ends at 0xDE1C. 40 x 24 size
    byte type    = cpu.GPR[C];
    word length  = (word) (cpu.GPR[D]) | (word) (cpu.GPR[E] << 8);
    word address = (word) (cpu.GPR[F]) | (word) (cpu.GPR[G] << 8);
    byte value   = cpu.GPR[F];

    if (type == 0 || type == 2) {
        cout << "OUTPUT :" << endl << cpu.GPR[F];
    } else if (type == 1) {
        // word output[length];
        // for (word i = 0; i < length; i++) {
        //     output[i] = cpu.ReadByte (address + i, mem);
        // }

        cout << "OUTPUT :" << endl;
        // for (word i = 0; i < length; i++) {
        //     cout << ""
        // }
    } else {
        cout << "ERROR  :" << endl << "Type " << INT_REPRESENTATION_OF_HEX (type) << " unsupported." << endl;
    }


    // FOR USE WITH SCREEN FOLDER
    // byte x_pos   = cpu.GPR[A];
    // byte y_pos   = cpu.GPR[B];
    // byte type    = cpu.GPR[C];
    // word length  = (word) (cpu.GPR[D]) | (word) (cpu.GPR[E] << 8);
    // word address = (word) (cpu.GPR[F]) | (word) (cpu.GPR[G] << 8);
    // byte value   = cpu.GPR[F];



    // word output_addr = 0xDA5D;
    // if (x_pos >= 40 || y_pos >= 24) {
    //     output_addr += (x_pos_gen * 40) + (y_pos_gen * 24);
    // } else {
    //     output_addr += (x_pos * 40) + (y_pos * 24);
    // }

    // switch (type) {
    //     case 0: { // Register
    //         if (value > 7)  value = 0;
    //         mem.WriteByte (cpu.GPR[value], output_addr);

    //     }
    // }
}

char index_to_char (byte index) {
    // Numbes
    if (index >= 0 && index <= 9)       return char (index + 48);
    // letters
    if (index >= 10 && index <= 35)     return char (index + 55);

    switch (index) {
        case char_space_idx   : return char (32);
        case char_excl_idx    : return char (33);
        case char_quote_idx   : return char (34);
        case char_hash_idx    : return char (35);
        case char_dollar_idx  : return char(36);
        case char_percent_idx : return char(37);
        case char_amp_idx     : return char(38);
        case char_apostR_idx  : return char(39);
        case char_brcL_idx    : return char(40);
        case char_brcR_idx    : return char(41);
        case char_ast_idx     : return char(42);
        case char_plus_idx    : return char(43);
        case char_comma_idx   : return char(44);
        case char_dash_idx    : return char(45);
        case char_period_idx  : return char(46);
        case char_fwdslsh_idx : return char(47);

        case char_colon_idx  : return char (58);
        case char_smcol_idx  : return char (59);
        case char_angleL_idx : return char (60);
        case char_equal_idx  : return char (61);
        case char_angleR_idx : return char (62);
        case char_quest_idx  : return char (63);
        case char_at_idx     : return char (64);

        case char_brkL_idx   : return char (91);
        case char_bkslsh_idx : return char (92);
        case char_brkR_idx   : return char (93);
        case char_caret_idx  : return char (94);
        case char_under_idx  : return char (95);
        case char_apostL_idx : return char (96);

        case char_curL_idx  : return char (123);
        case char_vert_idx  : return char (124);
        case char_curR_idx  : return char (125);
        case char_tilde_idx : return char (126);
    }

    return '\0';
}
