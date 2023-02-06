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


/*
------ CPU INSTRUCTION DECODING -------

R-Type: Register type instructions
I-Type: Immediate type instructions
S-Type: Store type instructions
B-Type: Break type instructions
U-Type: Register type instructions
J-Type: Jump type instructions
*/

/* Returns opcode register—7-bit address of destination register [6:0] */
uint64_t cpu_decode_opcode(uint32_t inst);

/* Returns rd register—5-bit address of destination register [11:7] */
uint64_t cpu_decode_rd(uint32_t inst);

/* Returns rs1 register—4-bit address of destination register [19:15] */
uint64_t cpu_decode_rs1(uint32_t inst);

/* Returns rs2 register—4-bit address of destination register [24:20] */
uint64_t cpu_decode_rs2(uint32_t inst);

/* bits[11:0] of inst[31:20] */
uint64_t cpu_decode_imm_I(uint32_t inst);

/* bits [11:5] of inst[31:25], bits [4:0] of inst[11:7] */
uint64_t cpu_decode_imm_S(uint32_t inst);

/* imm[12|10:5|4:1|11] of inst[31|30:25|11:8|7] */
uint64_t cpu_decode_imm_B(uint32_t inst);

/* bits [31:12] */
uint64_t cpu_decode_imm_U(uint32_t inst);

/* imm[20|10:1|11|19:12] of inst[31|30:21|20|19:12] */
uint64_t cpu_decode_imm_J(uint32_t inst);

/* Returns shamt register—5-bit address of destination register [5:0] */
uint64_t cpu_decode_shamt(uint32_t inst);
