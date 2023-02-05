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
    // fetch next instruction to be executed by loading data onto bus from dram
    // at the specified address given by program-counter
    uint32_t inst = cpu_load(cpu, cpu->program_counter, 32);
    return inst;
}
