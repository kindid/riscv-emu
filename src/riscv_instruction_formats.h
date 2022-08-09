////////////////////////////////////////////////////////////////////////////////
/// (C) 2022 kindid.co.uk
/// MIT licensed
/// RISCV-32 emulator written for educational purposes
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "bitops.h"

struct riscv_r_type {
    riscv_r_type(u32 instruction) {
        funct7 = extract(instruction, 25, 7);
        rs2 = extract(instruction, 20, 5);
        rs1 = extract(instruction, 15, 5);
        funct3 = extract(instruction, 12, 3);
        rd = extract(instruction, 7, 5);
        opcode = extract(instruction, 0, 6);
    }
    u8 funct7;          // 25, 7
    u8 rs2;             // 20, 5
    u8 rs1;             // 15, 5
    u8 funct3;          // 12, 3
    u8 rd;              //  7, 5
    u8 opcode;          //  0, 7
};

struct riscv_i_type {
    riscv_i_type(u32 instruction) {
        imm = extract(instruction, 20, 12);
        rs1 = extract(instruction, 15, 5);
        funct3 = extract(instruction, 12, 3);
        rd = extract(instruction, 7, 5);
        opcode = extract(instruction, 0, 6);
    }
    u16 imm;
    s32 simm() const { return sign_extend(u32(imm), 11u); }
    u8 rs1;
    u8 funct3;
    u8 rd;
    u8 opcode;
};

struct riscv_s_type {
    riscv_s_type(u32 instruction) {
        imm = extract(instruction, 7, 5) | (extract(instruction, 25, 7) << 5);
        rs2 = extract(instruction, 20, 5);
        rs1 = extract(instruction, 15, 5);
        funct3 = extract(instruction, 12, 3);
        opcode = extract(instruction, 0, 6);
    }
    s32 simm() { return sign_extend(u32(imm), 11u); }
    u16 imm;
    u8 rs2;
    u8 rs1;
    u8 funct3;
    u8 opcode;
};

struct riscv_b_type {
    riscv_b_type(u32 instruction) {
        imm = extract(instruction, 7, 1) << 11;
        imm |= extract(instruction, 8, 4) << 1;
        imm |= extract(instruction, 25, 6) << 5;
        imm |= extract(instruction, 31, 1) << 12;
        imm = sign_extend(u32(imm), 12);
        rs2 = extract(instruction, 20, 5);
        rs1 = extract(instruction, 15, 5);
        funct3 = extract(instruction, 12, 3);
        opcode = extract(instruction, 0, 6);
    }
    u32 imm;    // should be s16, or better s<12>
    s32 simm() { return sign_extend(u32(imm), 12u); }
    u8 rs2;
    u8 rs1;
    u8 funct3;
    u8 opcode;
};

struct riscv_u_type {
    riscv_u_type(u32 instruction) {
        imm = extract(instruction, 12, 20);
        rd = extract(instruction, 7, 5);
        opcode = extract(instruction, 0, 6);
    }
    u32 imm;
    u8 rd;
    u8 opcode;
};

struct riscv_j_type {
    riscv_j_type(u32 instruction) {
        u32 tmp_imm = 0;
        tmp_imm |= extract(instruction, 12, 8) << 12;
        tmp_imm |= extract(instruction, 20, 1) << 11;
        tmp_imm |= extract(instruction, 21, 10) << 1;
        tmp_imm |= extract(instruction, 31, 1) << 20;
        tmp_imm = sign_extend(tmp_imm, 20);
        imm = tmp_imm;
        rd = extract(instruction, 7, 5);
        opcode = extract(instruction, 0, 6);
    }
    s32 imm;
    u8 rd;
    u8 opcode;
};
