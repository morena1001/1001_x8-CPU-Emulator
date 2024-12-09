#include "loader.h"
#include "aux_mem.h"

using namespace std;

typedef struct OS {
    byte p_stack [120];

    void Init (cpu_t &cpu, mem_t &mem, aux_mem_t &aux_mem);
} os_t;

void Status_Reset (cpu_t &cpu, mem_t &mem);
