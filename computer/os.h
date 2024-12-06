#include "loader.h"

using namespace std;

typedef struct OS {
    byte p_stack [120];
} os_t;

void os_Init (cpu_t &cpu, mem_t &mem);
