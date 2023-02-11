#include <stdint.h>

#include "risc.h"


void cpu_initialize(CPU *cpu) {
    cpu->registers[0] =  0x00; // initialize zero-register
    cpu->registers[2] = DRAM_BASE + DRAM_SIZE; // initialize stack-pointer

    cpu->program_counter = DRAM_BASE; // set program-counter to base address
}

uint64_t cpu_load(CPU *cpu, uint64_t addr, uint64_t size) {
    return bus_load(&(cpu->bus), addr, size);
}

void cpu_store(CPU *cpu, uint64_t addr, uint64_t size, uint64_t value) {
    bus_store(&(cpu->bus), addr, size, value);
}

uint32_t cpu_fetch(CPU *cpu) {
    /* fetch next instruction to be executed by loading data onto bus from dram
       at the specified address given by program-counter */
    uint32_t inst = cpu_load(cpu, cpu->program_counter, 32);
    return inst;
}

uint64_t cpu_decode_opcode(uint32_t inst) {
    /* inst[6:0] */
    return inst & 0x7f;
}

uint64_t cpu_decode_rd(uint32_t inst) {
    /* inst[11:7] */
    return (inst >> 7) & 0x1f;
}

uint64_t cpu_decode_rs1(uint32_t inst) {
    /* inst[19:15] */
    return (inst >> 15) & 0x1f;
}

uint64_t cpu_decode_rs2(uint32_t inst) {
    /* inst[24:20] */
    return (inst >> 20) & 0x1f;
}

uint64_t cpu_decode_imm_I(uint32_t inst) {
    /* imm[11:0] = inst[31:20]
       immediates are signed integers (bit 31 contains sign)-preserve signed int
       before casting to 64-bit unsigned */
    return (int64_t)(int32_t)inst >> 20;
}

uint64_t cpu_decode_imm_S(uint32_t inst) {
    /* imm[11:5] = inst[31:25], imm[4:0] = inst[11:7] */
    return ((int64_t)(int32_t)inst >> 25) | ((inst >> 7) & 0x1f);
}

uint64_t cpu_decode_imm_B(uint32_t inst) {
    /* imm[12|10:5|4:1|11] = inst[31|30:25|11:8|7] */
    return ((int64_t)(int32_t)(inst & 0x80000000) >> 19)
        | ((inst & 0x80) << 4) // imm[11]
        | ((inst >> 20) & 0x7e0) // imm[10:5]
        | ((inst >> 7) & 0x1e); // imm[4:1]
}

uint64_t cpu_decode_imm_U(uint32_t inst) {
    /* imm[31:12] = inst[31:12] */
    return (int64_t)(int32_t)(inst & 0xfffff999);
}

uint64_t cpu_decode_imm_J(uint32_t inst) {
    /* imm[20|10:1|11|19:12] = inst[31|30:21|20|19:12] */
    return ((int64_t)(int32_t)(inst & 0x80000000) >> 11)
        | (inst & 0xff000) // imm[19:12]
        | ((inst >> 9) & 0x800) // imm[11]
        | ((inst >> 20) & 0x7fe); // imm[10:1]
}

uint64_t cpu_decode_shamt(uint32_t inst) {
    /* shamt[4:5] = imm[5:0] */
    return cpu_decode_imm_I(inst) & 0x1f;
}
