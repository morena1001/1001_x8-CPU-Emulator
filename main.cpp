#include <stdio.h>
#include <stdlib.h>

#include "1001_x8.h"

int main () {

    mem_t mem;
    cpu_t cpu;

    cpu.Reset (mem);

    mem.WriteByte (0x21, 0xFFFD);
    mem.WriteWord (0x1000, 0xFFFE);
    
    mem.WriteByte (0x01, 0x1000);
    mem.WriteByte (0x00, 0x1001);
    mem.WriteWord (0xBCDA, 0x1002);
    mem.WriteByte (0x35, 0x1004);
    mem.WriteByte (0x2F, 0xBCDA);

    cpu.PC = 0x1000;
    cpu.Execute (mem);

    printf("%X\r\n", cpu.GPR[A]);

    return 0;
}