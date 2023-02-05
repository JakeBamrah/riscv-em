#include <stdint.h>


/*
------ DRAM -------
Data structure that stores all instructions to be executed.
*/

/* Start of dram address space-lower address spaces reserved for IO devices */
#define DRAM_BASE 0x80000000
#define DRAM_SIZE 1024 * 1024 * 1

typedef struct{
    uint8_t mem[DRAM_SIZE];
}DRAM;

/*
Reads and stores instructions from a specific dram address. Values are stored
as 8-bit chunks in little-endian order
 */
uint64_t dram_load(DRAM* dram, uint64_t addr, uint64_t size);
void dram_store(DRAM* dram, uint64_t addr, uint64_t size, uint64_t value);


/*
------ Memory BUS -------
A data connector allowing data transfer between DRAM and CPU.
*/

typedef struct{
    DRAM *dram;
}BUS;

uint64_t bus_load(BUS* bus, uint64_t addr, uint64_t size);
void bus_store(BUS* bus, uint64_t addr, uint64_t size, uint64_t value);


/* ------ CPU ------- */

typedef struct{
    uint64_t registers[32];
    uint64_t program_counter;
    BUS bus;
}CPU;

/* Initializes CPU registers and aligns program-counter with start of DRAM */
void cpu_initialize(CPU *cpu);

/* Fetches and returns and instruction from main memory (DRAM) */
uint32_t cpu_fetch(CPU *cpu);

/* A basic ALU and decoder—decodes a fetched instruction and executes it */
int cpu_execute(CPU *cpu, uint32_t inst);
