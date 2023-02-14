#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "./src/risc.h"

void read_file(CPU *cpu, char *filename) {
    FILE *file;
    uint8_t *buffer;
    unsigned long file_len;

    file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Unable to open file %s", filename);
    }

    fseek(file, 0, SEEK_END);
    file_len = ftell(file);
    fseek(file, 0, SEEK_SET);

    // read file into local buffer
    buffer = (uint8_t *)malloc(file_len + 1);
    fread(buffer, file_len, 1, file);
    fclose(file);

    memcpy(cpu->bus->dram->mem, buffer, file_len * sizeof(uint8_t));
    free(buffer);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: rvemu <filename>\n");
        exit(1);
    }

    CPU cpu;
    cpu_initialize(&cpu);
    read_file(&cpu, argv[1]);

    uint64_t dram_len = sizeof(cpu.bus->dram->mem) / cpu.bus->dram->mem[0];
    while (1) {
        uint32_t inst = cpu_fetch(&cpu);

        cpu.program_counter += 4;
        if (!cpu_execute(&cpu, inst))
            break;

        cpu_dump_registers(&cpu);

        if (cpu.program_counter == 0)
            break;
    }

    return 0;
}
