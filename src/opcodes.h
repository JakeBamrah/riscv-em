#define R_TYPE  0x33
    #define ADDSUB  0x0
        #define ADD     0x00
        #define SUB     0x20
    #define SLL     0x1         // shift-left-logical
    #define SLT     0x2         // shift-less-than
    #define SLTU    0x3         // shift-less-than unsigned
    #define XOR     0x4
    #define SR      0x5
        #define SRL     0x00    // shift-right-logical
        #define SRA     0x20    // shift-right-arithmetic
    #define OR      0x6
    #define AND     0x7

#define I_TYPE  0x13
    #define ADDI    0x0
    #define SLLI    0x1
    #define SLTI    0x2
    #define SLTIU   0x3
    #define XORI    0x4
    #define SRI     0x5
        #define SRLI    0x00
        #define SRAI    0x20
    #define ORI     0x6
    #define ANDI    0x7





