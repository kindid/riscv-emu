#include "riscv_im_builder.h"


// todo; use a memory_device
struct riscv_im_builder {
    riscv_im_builder(u8 * memory = nullptr)
        : memory(memory)
    {}

    // todo; *very* strict checking
    // dude - combine this with the
    u32 i_type(u32 opcode, u32 rd, u32 funct3, u32 rs1, u32 imm) {
        return opcode | (rd << 7) | (funct3 << 12) | (rs1 << 15) | (imm << 20);
    }
    u32 r_type(u32 opcode, u32 rd, u32 funct3, u32 rs1, u32 rs2, u32 funct7) {
        return opcode | (rd << 7) | (funct3 << 12) | (rs1 << 15) | (rs2 << 20) << (funct7 << 25);
    }
    u32 s_type(u32 opcode, u32 imm1, u32 funct3, u32 rs1, u32 rs2, u32 imm2) {
        return opcode | (imm1 << 7) | (funct3 << 12) | (rs1 << 15) | (rs2 << 20) << (imm2 << 25);
    }

    void push(u32 op) {
        ((u32 *)(memory))[pc] = op;
        pc += 4;
    }

    void push(u16 op) {
        ((u16 *)(memory))[pc] = op;
        pc += 2;
    }

    void addi(u32 rd, u32 rs1, u32 imm)     { push(i_type(0b0010011, rd, 0b000, rs1, imm)); }
    void slli(u32 rd, u32 rs1, u32 shamt)   { push(r_type(0b0010011, rd, 0b001, rs1, shamt, 0b0000000)); }
    void slti(u32 rd, u32 rs1, u32 imm)     { push(i_type(0b0010011, rd, 0b010, rs1, imm)); }
    void sltiu(u32 rd, u32 rs1, u32 imm)    { push(i_type(0b0010011, rd, 0b011, rs1, imm)); }
    void xori(u32 rd, u32 rs1, u32 imm)     { push(i_type(0b0010011, rd, 0b100, rs1, imm)); }
    void srli(u32 rd, u32 rs1, u32 shamt)   { push(r_type(0b0010011, rd, 0b101, rs1, shamt, 0b0000000)); }
    void srai(u32 rd, u32 rs1, u32 shamt)   { push(r_type(0b0010011, rd, 0b101, rs1, shamt, 0b0100000)); }
    void ori(u32 rd, u32 rs1, u32 imm)      { push(i_type(0b0010011, rd, 0b110, rs1, imm)); }
    void andi(u32 rd, u32 rs1, u32 imm)     { push(i_type(0b0010011, rd, 0b111, rs1, imm)); }

    u8 * memory;
    u8 pc = 0;
};

