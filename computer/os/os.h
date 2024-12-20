// #include "../loader/loader.h"
// #include "../aux_mem/aux_mem.h"
#include "../aux_mem_loader/aux_mem_loader.h"

using namespace std;

typedef struct OS {
    byte p_stack [120];

    void Init (cpu_t &cpu, mem_t &mem, aux_mem_t &aux_mem);
    void Run (cpu_t &cpu, mem_t &mem, aux_mem_t &aux_mem, aux_loader_t& loader);
} os_t;

void Status_Reset (cpu_t &cpu, mem_t &mem);
