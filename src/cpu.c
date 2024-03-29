#include <stdint.h>
#include <stdio.h>

#include "risc.h"
#include "opcodes.h"


void cpu_initialize(CPU *cpu) {
    cpu->registers[0] =  0x00; // initialize zero-register
    cpu->registers[2] = DRAM_BASE + DRAM_SIZE; // initialize stack-pointer

    cpu->program_counter = DRAM_BASE; // set program-counter to base address
}

uint64_t cpu_load(CPU *cpu, uint64_t addr, uint64_t size) {
    return bus_load(cpu->bus, addr, size);
}

void cpu_store(CPU *cpu, uint64_t addr, uint64_t size, uint64_t value) {
    bus_store(cpu->bus, addr, size, value);
}

uint32_t cpu_fetch(CPU *cpu) {
    /* fetch next instruction to be executed by loading data onto bus from dram
       at the specified address given by program-counter */
    uint32_t inst = cpu_load(cpu, cpu->program_counter, 32);
    return inst;
}


/* ------ INSTRUCTION EXECUTORS ------- */

void cpu_exec_ADD(CPU *cpu, uint32_t inst) {
    int64_t rs1 = cpu->registers[cpu_decode_rs1(inst)];
    int64_t rs2 = cpu->registers[cpu_decode_rs2(inst)];
    cpu->registers[cpu_decode_rd(inst)] = (uint64_t)(rs1 + rs2);
    printf("add\n");
}

void cpu_exec_SUB(CPU *cpu, uint32_t inst) {
    int64_t rs1 = cpu->registers[cpu_decode_rs1(inst)];
    int64_t rs2 = cpu->registers[cpu_decode_rs2(inst)];
    cpu->registers[cpu_decode_rd(inst)] = (uint64_t)(rs1 - rs2);
    printf("sub\n");
}

void cpu_exec_SLL(CPU *cpu, uint32_t inst) {
    uint64_t rs1 = cpu_decode_rs1(inst);
    uint64_t rs2 = cpu_decode_rs2(inst);
    cpu->registers[cpu_decode_rd(inst)] = cpu->registers[rs1] << (int64_t)cpu->registers[rs2];
    printf("sll\n");
}

void cpu_exec_SLT(CPU *cpu, uint32_t inst) {
    uint64_t rs1 = cpu_decode_rs1(inst);
    uint64_t rs2 = cpu_decode_rs2(inst);
    cpu->registers[cpu_decode_rd(inst)] = (cpu->registers[rs1] < (int64_t)cpu->registers[rs2]) ? 1: 0;
    printf("slt\n");
}

void cpu_exec_SLTU(CPU *cpu, uint32_t inst) {
    uint64_t rs1 = cpu_decode_rs1(inst);
    uint64_t rs2 = cpu_decode_rs2(inst);
    cpu->registers[cpu_decode_rd(inst)] = (cpu->registers[rs1] < cpu->registers[rs2]) ? 1: 0;
    printf("sltu\n");
}

void cpu_exec_XOR(CPU *cpu, uint32_t inst) {
    uint64_t rs1 = cpu_decode_rs1(inst);
    uint64_t rs2 = cpu_decode_rs2(inst);
    cpu->registers[cpu_decode_rd(inst)] = cpu->registers[rs1] ^ cpu->registers[rs2];
    printf("xor\n");
}

void cpu_exec_SRL(CPU *cpu, uint32_t inst) {
    uint64_t rs1 = cpu_decode_rs1(inst);
    uint64_t rs2 = cpu_decode_rs2(inst);
    cpu->registers[cpu_decode_rd(inst)] = cpu->registers[rs1] >> (int64_t)cpu->registers[rs2];
    printf("srl\n");
}

void cpu_exec_SRA(CPU *cpu, uint32_t inst) {
    uint64_t rs1 = cpu_decode_rs1(inst);
    uint64_t rs2 = cpu_decode_rs2(inst);
    cpu->registers[cpu_decode_rd(inst)] = (int32_t)cpu->registers[rs1] >> (int64_t)cpu->registers[rs2];
    printf("sra\n");
}

void cpu_exec_OR(CPU *cpu, uint32_t inst) {
    uint64_t rs1 = cpu_decode_rs1(inst);
    uint64_t rs2 = cpu_decode_rs2(inst);
    cpu->registers[cpu_decode_rd(inst)] = cpu->registers[rs1] | cpu->registers[rs2];
    printf("or\n");
}

void cpu_exec_AND(CPU *cpu, uint32_t inst) {
    uint64_t rs1 = cpu_decode_rs1(inst);
    uint64_t rs2 = cpu_decode_rs2(inst);
    cpu->registers[cpu_decode_rd(inst)] = cpu->registers[rs1] & cpu->registers[rs2];
    printf("and\n");
}

void cpu_exec_ADDI(CPU *cpu, uint32_t inst) {
    uint64_t imm = cpu_decode_imm_I(inst);
    uint64_t rs1 = cpu_decode_rs1(inst);
    uint64_t rd = cpu_decode_rd(inst);
    cpu->registers[rd] = cpu->registers[rs1] + (int64_t)imm;
    printf("addi\n");
}

void cpu_exec_SLLI(CPU* cpu, uint32_t inst) {
    uint64_t rs1 = cpu_decode_rs1(inst);
    uint64_t shamt = cpu_decode_shamt(inst);
    uint64_t rd = cpu_decode_rd(inst);
    cpu->registers[rd] = cpu->registers[rs1] << shamt;
    printf("slli\n");
}

void cpu_exec_SLTI(CPU* cpu, uint32_t inst) {
    uint64_t imm = cpu_decode_imm_I(inst);
    uint64_t rs1 = cpu_decode_rs1(inst);
    uint64_t rd = cpu_decode_rd(inst);
    cpu->registers[rd] = (cpu->registers[rs1] < (int64_t)imm) ? 1 : 0;
    printf("slti\n");
}

void cpu_exec_SLTIU(CPU* cpu, uint32_t inst) {
    uint64_t imm = cpu_decode_imm_I(inst);
    uint64_t rs1 = cpu_decode_rs1(inst);
    uint64_t rd = cpu_decode_rd(inst);
    cpu->registers[rd] = (cpu->registers[rs1] < imm) ? 1 : 0;
    printf("sltiu\n");
}

void cpu_exec_XORI(CPU* cpu, uint32_t inst) {
    uint64_t imm = cpu_decode_imm_I(inst);
    uint64_t rs1 = cpu_decode_rs1(inst);
    uint64_t rd = cpu_decode_rd(inst);
    cpu->registers[rd] = cpu->registers[rs1] ^ imm;
    printf("xori\n");
}

void cpu_exec_SRLI(CPU* cpu, uint32_t inst) {
    uint64_t imm = cpu_decode_imm_I(inst);
    uint64_t rs1 = cpu_decode_rs1(inst);
    uint64_t rd = cpu_decode_rd(inst);
    cpu->registers[rd] = cpu->registers[rs1] >> imm;
    printf("srli\n");
}

void cpu_exec_SRAI(CPU* cpu, uint32_t inst) {
    uint64_t imm = cpu_decode_imm_I(inst);
    uint64_t rs1 = cpu_decode_rs1(inst);
    uint64_t rd = cpu_decode_rd(inst);
    cpu->registers[rd] = (int32_t)cpu->registers[rs1] >> imm;
    printf("srai\n");
}

void cpu_exec_ORI(CPU* cpu, uint32_t inst) {
    uint64_t imm = cpu_decode_imm_I(inst);
    uint64_t rs1 = cpu_decode_rs1(inst);
    uint64_t rd = cpu_decode_rd(inst);
    cpu->registers[rd] = cpu->registers[rs1] | imm;
    printf("ori\n");
}

void cpu_exec_ANDI(CPU* cpu, uint32_t inst) {
    uint64_t imm = cpu_decode_imm_I(inst);
    uint64_t rs1 = cpu_decode_rs1(inst);
    uint64_t rd = cpu_decode_rd(inst);
    cpu->registers[rd] = cpu->registers[rs1] & imm;
    printf("andi\n");
}

void cpu_exec_SB(CPU* cpu, uint32_t inst) {
    uint64_t imm = cpu_decode_imm_S(inst);
    uint64_t addr = cpu->registers[cpu_decode_rs1(inst)] + (int64_t)(imm);
    cpu_store(cpu, addr, 8, cpu->registers[cpu_decode_rs2(inst)]);
    printf("sb\n");
}

void cpu_exec_SH(CPU* cpu, uint32_t inst) {
    uint64_t imm = cpu_decode_imm_S(inst);
    uint64_t addr = cpu->registers[cpu_decode_rs1(inst)] + (int64_t)(imm);
    cpu_store(cpu, addr, 16, cpu->registers[cpu_decode_rs2(inst)]);
    printf("sh\n");
}

void cpu_exec_SW(CPU* cpu, uint32_t inst) {
    uint64_t imm = cpu_decode_imm_S(inst);
    uint64_t addr = cpu->registers[cpu_decode_rs1(inst)] + (int64_t)(imm);
    cpu_store(cpu, addr, 32, cpu->registers[cpu_decode_rs2(inst)]);
    printf("sw\n");
}

void cpu_exec_SD(CPU* cpu, uint32_t inst) {
    uint64_t imm = cpu_decode_imm_S(inst);
    uint64_t addr = cpu->registers[cpu_decode_rs1(inst)] + (int64_t)(imm);
    cpu_store(cpu, addr, 64, cpu->registers[cpu_decode_rs2(inst)]);
    printf("sd\n");
}

int32_t cpu_execute(CPU *cpu, uint32_t inst) {
    int opcode = inst & 0x7f;           // inst[6:0]
    int funct3 = (inst >> 12) & 0x7;    // inst[14:12]
    int funct7 = (inst >> 25) & 0x7f;   // inst[31:25]

    // emulate register (0x0) is hardwired with bits equal to 0 at each cycle
    cpu->registers[0] = 0;

    switch (opcode) {
        case R_TYPE:
            switch (funct3) {
                case ADDSUB:
                    switch (funct7) {
                        case ADD:
                            cpu_exec_ADD(cpu, inst); break;
                        case SUB:
                            cpu_exec_SUB(cpu, inst); break;
                        default: ;
                    } break;
                case SLL:
                    cpu_exec_SLL(cpu, inst); break;
                case SLT:
                    cpu_exec_SLT(cpu, inst); break;
                case SLTU:
                    cpu_exec_SLTU(cpu, inst); break;
                case XOR:
                    cpu_exec_XOR(cpu, inst); break;
                case SR:
                    switch (funct7) {
                        case SRL:
                            cpu_exec_SRL(cpu, inst); break;
                        case SRA:
                            cpu_exec_SRA(cpu, inst); break;
                        default: ;
                    } break;
                case OR:
                    cpu_exec_OR(cpu, inst); break;
                case AND:
                    cpu_exec_AND(cpu, inst); break;
                default: ;
            } break;

        case I_TYPE:
            switch (funct3) {
                case ADDI:
                    cpu_exec_ADDI(cpu, inst); break;
                case SLLI:
                    cpu_exec_SLLI(cpu, inst); break;
                case SLTI:
                    cpu_exec_SLTI(cpu, inst); break;
                case SLTIU:
                    cpu_exec_SLTIU(cpu, inst); break;
                case XORI:
                    cpu_exec_XORI(cpu, inst); break;
                case SRI:
                    switch (funct7) {
                        case SRLI:
                            cpu_exec_SRLI(cpu, inst); break;
                        case SRAI:
                            cpu_exec_SRAI(cpu, inst); break;
                        default: ;
                    } break;

                case ORI:
                    cpu_exec_ORI(cpu, inst); break;
                case ANDI:
                    cpu_exec_ANDI(cpu, inst); break;
                default: ;
            } break;

        case S_TYPE:
            switch (funct3) {
                case SB:
                    cpu_exec_SB(cpu, inst); break;
                case SH:
                    cpu_exec_SH(cpu, inst); break;
                case SW:
                    cpu_exec_SW(cpu, inst); break;
                case SD:
                    cpu_exec_SD(cpu, inst); break;
                default: ;
            } break;

        default:
            fprintf(
                stderr,
                "[-] ERROR-> opcode:0x%x, funct3:0x%x, funct3:0x%x\n",
                opcode, funct3, funct7
            );
            return 0;
    }

    return 1;
}


/* ------ INSTRUCTION DECODERS ------- */

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
       cast-extending: immediates are signed integers (bit 31 contains sign)
       -preserve signed int before casting to 64-bit unsigned */
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

void cpu_dump_registers(CPU *cpu) {
    char *abi_registers[] = {
        "zero", "ra",  "sp",  "gp",
          "tp", "t0",  "t1",  "t2",
          "s0", "s1",  "a0",  "a1",
          "a2", "a3",  "a4",  "a5",
          "a6", "a7",  "s2",  "s3",
          "s4", "s5",  "s6",  "s7",
          "s8", "s9", "s10", "s11",
          "t3", "t4",  "t5",  "t6",
    };

    for (int i=0; i<8; i++) {
        printf("   %4s: %#-16.2lx  ", abi_registers[i],    cpu->registers[i]);
        printf("   %2s: %#-16.2lx  ", abi_registers[i+8],  cpu->registers[i+8]);
        printf("   %2s: %#-16.2lx  ", abi_registers[i+16], cpu->registers[i+16]);
        printf("   %3s: %#-16.2lx\n", abi_registers[i+24], cpu->registers[i+24]);
    }
}
