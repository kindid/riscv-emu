#pragma once

#include "types.h"
#include "bitops.h"
#include "riscv32_reg.h"
#include "memory_device.h"

////////////////////////////////////////////////////////////////////////////////
/// Compressed Formats /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// compressed instructions? HOW???
// expand to 32 bit and execute whilst noting that pc update is different
// AFAICT

// CR       32 regs (5bit)                                                  Register
// funct4:4 rdrs1:5 rs2:5 op:2
struct riscv_cr_type {
    riscv_cr_type(u16 instruction) {
        funct4 = extract(instruction, 12, 4);
        rdrs1 = extract(instruction, 7, 5);
        rs2 = extract(instruction, 2, 5);
        opcode = extract(instruction, 0, 2);
    }
    u8 funct4;
    u8 rdrs1;
    u8 rs2;
    u8 opcode;
};

// CI       32 regs (5bit)                                                  Immediate
        // funct3:3 imm1 rdrs1:5 imm5 op:2

struct riscv_ci_type {
    // you can't determine the decoding of the immediate until you know which instructions/register to use
    riscv_ci_type(u16 instruction) {
        funct3 = extract(instruction, 13, 3);
        rdrs1 = extract(instruction, 7, 5);
        rs2 = extract(instruction, 2, 5);
        opcode = extract(instruction, 0, 2);
    }
    u8 funct3;
    u8 rdrs1;
    u8 rs2;
    u8 opcode;
};


// CSS      32 regs (5bit)                                                  Stack relative Store
        // funct3:3 imm:6 rs2:5 op:2

// CIW      8 regs (3 bit)  maps to x8 to x15 - x2 (stack) is implicit      Wide Immediate
        // funct3:3 imm:8 rd:3 op:2

struct riscv_ciw_type {
    riscv_ciw_type(u16 instruction) {
        funct3 = extract(instruction, 13, 3);
        imm = extract(instruction, 11, 2) << 4;
        imm |= extract(instruction, 7, 4) << 6;
        imm |= extract(instruction, 6, 1) << 2;
        imm |= extract(instruction, 5, 1) << 3;
        rd = extract(instruction, 2, 3);
        opcode = extract(instruction, 0, 2);
    }
    u8 funct3;
    u8 imm;
    u8 rd;
    u8 opcode;
};

// CL       8 regs (3 bit)  maps to x8 to x15                               Load
        // funct3:3 imm:3 rs1:3 imm:2 rd:3 op:2

struct riscv_cl_type {
    riscv_cl_type(u16 instruction) {
        funct3 = extract(instruction, 13, 3);
        imm = extract(instruction, 10, 3) << 3;
        rs1 = extract(instruction, 7, 3);
        imm |= extract(instruction, 6, 1) << 2;
        imm |= extract(instruction, 5, 1) << 6;
        rd = extract(instruction, 2, 3);
        opcode = extract(instruction, 0, 2);
    }
    u8 funct3;
    u8 imm;
    u8 rs1;
    u8 rd;
    u8 opcode;
};


// CS       8 regs (3 bit)  maps to x8 to x15                               Store
// funct3:3 imm:3 rs1:3 imm:2 rs2:3 op:2
struct riscv_cs_type {  // TODO:CHECK - I THINK IT'S WRONG
    riscv_cs_type(u16 instruction) {
        funct3 = extract(instruction, 13, 3);
        imm = extract(instruction, 10, 3) << 3;
        rs1 = extract(instruction, 7, 3);
        imm |= extract(instruction, 6, 1) << 2;
        imm |= extract(instruction, 5, 1) << 6;
        rs2 = extract(instruction, 2, 3);       // should probably be rd
        opcode = extract(instruction, 0, 2);
    }
    u8 funct3;
    u8 imm;
    u8 rs1;
    u8 rs2;
    u8 opcode;
};

// I think these all need to be super expanded



// CB       8 regs (3 bit)  maps to x8 to x15                               Branch
        // funct3:3 imm:3 rs1:3 imm:5 op:2

struct riscv_cb_type {
    // you can't determine the decoding of the immediate until you know which instructions/register to use
    riscv_cb_type(u16 instruction) {
        funct3 = extract(instruction, 13, 3);
        imm3 = extract(instruction, 10, 3);
        rs1rd = extract(instruction, 7, 3);
        imm2 = extract(instruction, 5, 2);
        // please note the diffs here
        imm5 = extract(instruction, 2, 5);
        rs2 = extract(instruction, 2, 3);
        opcode = extract(instruction, 0, 2);
    }
    u8 funct3;
    u8 imm3;
    u8 rs1rd;
    u8 imm5;
    u8 rs2;
    u8 imm2;
    u8 opcode;
};


// CJ                                                                       Jump
        // funct3:3 imm:10 op:2


// compressed formats...
// 3 : 6 : 5 : 2 : funct3 : uimm : rs2 : 10
// 3 : 6 : 5 : 2 : funct3 : uimm : rs2 : 10

// compressed instructions? HOW???
// expand to 32 bit and execute whilst noting that pc update is different
// AFAICT
// CR       32 regs (5bit)                                                  Register
        // funct4:4 rdrs1:5 rs2:5 op:2
// CI       32 regs (5bit)                                                  Immediate
        // funct3:3 imm1 rdrs1:5 imm5 op:2
// CSS      32 regs (5bit)                                                  Stack relative Store
        // funct3:3 imm:6 rs2:5 op:2
// CIW      8 regs (3 bit)  maps to x8 to x15 - x2 (stack) is implicit      Wide Immediate
        // funct3:3 imm:8 rd:3 op:2
// CL       8 regs (3 bit)  maps to x8 to x15                               Load
        // funct3:3 imm:3 rs1:3 imm:2 rd:3 op:2
// CS       8 regs (3 bit)  maps to x8 to x15                               Store
        // funct3:3 imm:3 rs1:3 imm:2 rs2:3 op:2
// CB       8 regs (3 bit)  maps to x8 to x15                               Branch
        // funct3:3 imm:3 rs1:3 imm:5 op:2
// CJ                                                                       Jump
        // funct3:3 imm:10 op:2

struct riscv_c_decode
{
    riscv32_reg reg;
    memory_device * _memory_device;
    void illegal() {}
    u32 decode(u16 compressed);
    s32 pc_update;
    u32 pc;
};

