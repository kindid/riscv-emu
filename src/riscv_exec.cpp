////////////////////////////////////////////////////////////////////////////////
/// (C) 2022 kindid.co.uk
/// MIT licensed
/// RISCV-32 emulator written for educational purposes
////////////////////////////////////////////////////////////////////////////////

#include "riscv_exec.h"
#include "riscv_instruction_formats.h"

#include "bitops.h"

#include <sstream>

//#define DEBUG_ON
#include "here.h"

riscv_exec::riscv_exec(memory_device * __memory_device)
    : _memory_device(__memory_device)
{}

void riscv_exec::reset() {
    pc = 0x00000000;
    for (size_t r = 0; r < riscv32_reg::int_reg_count; r++) {
        reg.wri(r,0);
    }
}

void riscv_exec::illegal() {
    here << "illegal " << hex_pad(u64(illegal_cb));
    if (illegal_cb) {
        illegal_cb(this);
    } else {
        u8 first = _memory_device->read_u8(pc);
        if ((first & 3) == 3) {
            here << "illegal " << hex_pad(pc, 8) << " : " << hex_pad(_memory_device->read_u32(pc)) << "\n";
        } else {
            here << "illegal " << hex_pad(pc, 8) << " : " << hex_pad(_memory_device->read_u16(pc)) << "\n";
        }
        throw("illegal");
        faults |= illegal_op;
    }
}

/*
    diassembly formats

    op 'i space reg, reg, imm
    op space reg, reg, imm

    lui         rd, imm
    auipc       rd, imm
    jal         rd, imm
    jalr        rd, rs(imm)
    beq         rs1, rs2, imm
    bne         rs1, rs2, imm
    blt         rs1, rs2, dest
    bge         rs1, rs2, dest
    bltu        rs1, rs2, dest
    bgeu        rs1, rs2, dest
    lb          rs1, rs2, dest
    lh          rs, rd(imm)
    lw          rs, rd(imm)
    lbu         rs, rd(imm)
    lhu         rs, rd(imm)
    sb          rs, rd(imm)
    sh          rs, rd(imm)
    sw          rs, rd(imm)
    addi        rd, rs1, imm
    slti        rd, rs1, imm
    sltiu       rd, rs1, imm
    xori        rd, rs1, imm
    ori         rd, rs1, imm
    andi        rd, rs1, imm
    slli        rd, rs1, imm
    srli        rd, rs1, imm
    srai        rd, rs1, imm
    add         rd, rs1, rs2
    sub         rd, rs1, rs2
    sll         rd, rs1, rs2
    slt         rd, rs1, rs2
    sltu        rd, rs1, rs2
    xor         rd, rs1, rs2
    srl         rd, rs1, rs2
    sra         rd, rs1, rs2
    or          rd, rs1, rs2
    and         rd, rs1, rs2
    fence       rs1, rs2, flags (imm)
    fence.i
    ecall
    ebreak
    csrrw       rd, rs1, csr_reg
    csrrs       rd, rs1, csr_reg
    csrrc       rd, rs1, csr_reg
    csrrwi      rd, imm, csr_reg
    csrrsi      rd, imm, csr_reg
    csrrc       rd, imm, csr_reg

    ; and that's about it



*/

// move this to a 'disassembler' module (somewhere)
// tbf - parsing is annoying but it should be possible to build something useful
static std::string regstr(u8 reg) {
    return std::string("x") + std::to_string(reg);
}

static std::string regstr(s32 offset, u8 reg) {
    return std::to_string(offset) + "(" + std::string("x") + std::to_string(reg) + ")";
}

// the plan is kinda ugly...
// i'm going to use "crtp" to define 2 classes with a pile of interfaces like
// 'lb', 'lh', 'sb', 'sll' and so forth
// (what if I want to use *both* of these btw?)
// riscv_exec

// there are 2 modes through here but i don't want to have to write
// a secondary decoder - nor do i really want to "suffer" the

void riscv_exec::execute()
{
    // todo; this does *not* support 'c' mode (yet)
    u32 instruction = u32(_memory_device->read_u32(pc));
//    here << hex_pad(pc) << " : " << hex_pad(instruction);// << " : " << hex_pad(u64(_memory_device->memory)) << "\n";
    //
    here << "fetch << " << hex_pad(pc);
    s32 pc_update = 4;
    faults = 0;
    if (instruction == 0x00000000u) {
        illegal();
        return;
    }
    //memory[pc];
    //        u32 regime = instruction & 0b11;
    //        if (regime == 0b11) {
    // read first half word first - if the lower bits are 0x11 then it's a 32 bit op (not quite true... but.. whatever)
    // bits<2> op_len_fmt = instruction.extract<0,2>();
    //
//    bits<32> tmp_instr(instruction);
//    auto op_len = tmp_instr.extract<0,2>();

    u32 op_len_fmt = extract(instruction, 0, 2);
//    here << h_wrap(op_len_fmt);
//    if (op_len == 11_b2) {
    if (op_len_fmt == 0b11) {
        // the output of this is a 32 bit instruction which can be parsed with the code below - nice

        // 32 bit instruction so read the hi word
        //instruction = instruction | (u32(_memory_device->read_u16(pc + 2)) << 16u);

//        u32 decider = extract(instruction, 25, 7);
        u32 op = extract(instruction, 2, 5);
        //auto op = tmp_instr.extract<2, 5>();

//                extract(instruction, 2, 5);
        //here << binary_string(bits<5>(op));
        if (op == 0b00000) {
            riscv_i_type iw(instruction);
            if (iw.funct3 == 0b00) {                                 // lb //
                here << "lb " << regstr(iw.rd) << "," << regstr(iw.simm(), iw.rs1);
                reg.wri(iw.rd, sign_extend(_memory_device->read_u8(reg.rdi(iw.rs1) + iw.simm())));
            }
            else if (iw.funct3 == 0b001) {                           // lh //
                here << "lh " << regstr(iw.rd) << "," << regstr(iw.simm(), iw.rs1);
                reg.wri(iw.rd, sign_extend(_memory_device->read_u16(reg.rdi(iw.rs1) + iw.simm())));
            }
            else if (iw.funct3 == 0b010) {                           // lw //
                here << "lw " << regstr(iw.rd) << "," << regstr(iw.simm(), iw.rs1);
                reg.wri(iw.rd, _memory_device->read_u32(reg.rdi(iw.rs1) + iw.simm()));
            }
            else if (iw.funct3 == 0b011) {                           // illegal //
                illegal();
            }
            else if (iw.funct3 == 0b100) {                           // lbu //
                here << "lbu " << regstr(iw.rd) << "," << regstr(iw.simm(), iw.rs1);
                reg.wri(iw.rd, u32(_memory_device->read_u8(reg.rdi(iw.rs1) + iw.simm())));
            }
            else if (iw.funct3 == 0b101) {                           // lhu //
                here << "lhu " << regstr(iw.rd) << "," << regstr(iw.simm(), iw.rs1);
                reg.wri(iw.rd, u32(_memory_device->read_u16(reg.rdi(iw.rs1) + iw.simm())));
            }
            else if (iw.funct3 == 0b110) {                           // illegal //
                illegal();
            }
            else if (iw.funct3 == 0b111) {                           // illegal //
                illegal();
            }
        }
        else if (op == 0b00001) {                                   // flw (rv32f)
            illegal();
        }
        else if (op == 0b00010) {
            illegal();
        }
        else if (op == 0b00011) {                                   // fence fence.i
            // ?12..14
            // 000 fence  24..27 = pred, succ = 23..20. all others 0
            // 001 fence.i. all others 0
        }
        else if (op == 0b00100) {                                   // immediate instructions //
            riscv_i_type iw(instruction);
            if (iw.funct3 == 0b000) {                               // addi //
//                addi	x13,x15,1024 # 1400 <main-0xec54>
                // c/c++ problem. u8 is treated as a character - it clearly ain't
                here << "addi " << regstr(iw.rd) << "," << regstr(iw.rs1) << ',' << iw.simm();
                reg.wri(iw.rd, reg.rdi(iw.rs1) + iw.simm());
            }
            else if (iw.funct3 == 0b001) {                          // slli //
                u32 decider = extract(iw.imm, 5, 8);
                if (decider == 0b0000000) {
                    here << "slli " << regstr(iw.rd) << "," << regstr(iw.rs1) << hex_pad(iw.simm());
                    u32 shamt = extract(iw.imm, 0, 5);
                    reg.wri(iw.rd, reg.rdi(iw.rs1) << shamt);
                } else {
                    illegal();
                }
            }
            else if (iw.funct3 == 0b010) {                          // slti //
                here << "slti " << regstr(iw.rd) << "," << regstr(iw.rs1) << ',' << iw.simm();
                if (s32(reg.rdi(iw.rs1)) < iw.simm()) {
                    reg.wri(iw.rd, 1);
                } else {
                    reg.wri(iw.rd,  0);
                }
            }
            else if (iw.funct3 == 0b011) {                          // sltiu //
                here << "sltiu " << regstr(iw.rd) << "," << regstr(iw.rs1) << ',' << iw.imm;
                if (reg.rdi(iw.rs1) < iw.imm) {
                    reg.wri(iw.rd, 1);
                } else {
                    reg.wri(iw.rd, 0);
                }
            }
            else if (iw.funct3 == 0b100) {                          // xori //
                here << "xori " << regstr(iw.rd) << "," << regstr(iw.rs1) << ',' << iw.simm();
                reg.wri(iw.rd, reg.rdi(iw.rs1) ^ iw.simm());
            }
            else if (iw.funct3 == 0b101) {                          // srli srai //
                // todo; check bits 25,26?. 26 = 0 but 25 = 'X' (eh?) i think this is doc cockup
                u32 decider = extract(iw.imm, 5, 8);
                if (decider == 0b0000000) {                         // srli //
                    u32 shamt = extract(iw.imm, 0, 5);
                    here << "srli " << regstr(iw.rd) << ',' << regstr(iw.rs1) << ',' << shamt;
                    // todo; move to 'bits' for this
                    reg.wri(iw.rd, u32(s32(reg.rdi(iw.rs1)) >> shamt));
                }
                else if (decider == 0b0100000) {                    // srai //
                    u32 shamt = extract(instruction, 19, 5);
                    here << "srai " << regstr(iw.rd) << ',' << regstr(iw.rs1) << ',' << shamt;
                    reg.wri(iw.rd, u32(s32(reg.rdi(iw.rs1)) >> shamt));
                }
                else {
                    illegal();
                }
            }
            else if (iw.funct3 == 0b110) {                          // ori //
                here << "ori " << regstr(iw.rd) << ',' << regstr(iw.rs1) << ',' << iw.simm();
                reg.wri(iw.rd, reg.rdi(iw.rs1) | iw.simm());
            }
            else if (iw.funct3 == 0b111) {                          // andi //
                here << "andi " << regstr(iw.rd) << ',' << regstr(iw.rs1) << ',' << iw.simm();
                reg.wri(iw.rd, reg.rdi(iw.rs1) & iw.simm());
            }
        }
        else if (op == 0b00101) {                                   // auipc //
            riscv_u_type iw(instruction);
            here << "aupic " << regstr(iw.rd) << "," << "0x" << hex_pad(iw.imm);
            reg.wri(iw.rd, pc + (iw.imm << 12u));
        }
        else if (op == 0b00110) {                             // 64bit addiw, slliw, srliw, sraiw //
            illegal();
        }
        else if (op == 0b00111) {
            illegal();
        }
        else if (op == 0b01000) {
            riscv_s_type iw(instruction);
            if (iw.funct3 == 0b000) {                               // sb
                here << "sb " << regstr(iw.rs2) << "," << regstr(iw.simm(), iw.rs1);
                _memory_device->write(reg.rdi(iw.rs1) + iw.simm(), u8(reg.rdi(iw.rs2)));
            }
            else if (iw.funct3 == 0b001) {                          // sh
                here << "sh " << regstr(iw.rs2) << "," << regstr(iw.simm(), iw.rs1);
                _memory_device->write(reg.rdi(iw.rs1) + iw.simm(), u16(reg.rdi(iw.rs2)));
            }
            else if (iw.funct3 == 0b010) {                          // sw
                here << "sw " << regstr(iw.rs2) << "," << regstr(iw.simm(), iw.rs1);
                _memory_device->write(reg.rdi(iw.rs1) + iw.simm(), u32(reg.rdi(iw.rs2)));
            }
            else if (iw.funct3 == 0b011) {                          // sd (rv64)
                illegal();
            }
            else {
                illegal();
            }
        }
        else if (op == 0b01001) {                                   // fsw (f), fsd (d) //
            illegal();
        }
        else if (op == 0b01010) {
            illegal();
        }
        else if (op == 0b01011) {                                   // rv32a (atomics) //
            illegal();
        }
        else if (op == 0b01100) {
            riscv_r_type iw(instruction);
            if (iw.funct7 == 0b0000000) {                           // add sll slt sltu xor srl or and
                if (iw.funct3 == 0x000) {                           // add //
                    here << "add " << regstr(iw.rd) << ',' << regstr(iw.rs1) << ',' << regstr(iw.rs2);
                    reg.wri(iw.rd, reg.rdi(iw.rs1) + reg.rdi(iw.rs2));
                }
                else if (iw.funct3 == 0b001) {                      // sll //
                    here << "sll " << regstr(iw.rd) << ',' << regstr(iw.rs1) << ',' << regstr(iw.rs2);
                    reg.wri(iw.rd, reg.rdi(iw.rs1) << reg.rdi(iw.rs2));
                }
                else if (iw.funct3 == 0b010) {                      // slt //
                    here << "slt " << regstr(iw.rd) << ',' << regstr(iw.rs1) << ',' << regstr(iw.rs2);
                    if ((s32)(reg.rdi(iw.rs1) < (s32)(reg.rdi(iw.rs2)))) {
                        reg.wri(iw.rd, 1);
                    } else {
                        reg.wri(iw.rd,  0);
                    }
                }
                else if (iw.funct3 == 0b011) {                      // sltu //
                    here << "sltu " << regstr(iw.rd) << ',' << regstr(iw.rs1) << ',' << regstr(iw.rs2);
                    if (reg.rdi(iw.rs1) < reg.rdi(iw.rs2)) {
                        reg.wri(iw.rd, 1);
                    } else {
                        reg.wri(iw.rd,  0);
                    }
                }
                else if (iw.funct3 == 0b100) {                      // xor //
                    here << "xor " << regstr(iw.rd) << ',' << regstr(iw.rs1) << ',' << regstr(iw.rs2);
                    reg.wri(iw.rd, reg.rdi(iw.rs1) ^ reg.rdi(iw.rs2));
                }
                else if (iw.funct3 == 0b101) {                      // srl //
                    here << "srl " << regstr(iw.rd) << ',' << regstr(iw.rs1) << ',' << regstr(iw.rs2);
                    reg.wri(iw.rd, reg.rdi(iw.rs1) >> reg.rdi(iw.rs2));
                }
                else if (iw.funct3 == 0b110) {                      // or //
                    here << "or " << regstr(iw.rd) << ',' << regstr(iw.rs1) << ',' << regstr(iw.rs2);
                    reg.wri(iw.rd, reg.rdi(iw.rs1) | reg.rdi(iw.rs2));
                }
                else if (iw.funct3 == 0b111) {                      // and //
                    here << "amd " << regstr(iw.rd) << ',' << regstr(iw.rs1) << ',' << regstr(iw.rs2);
                    reg.wri(iw.rd, reg.rdi(iw.rs1) & reg.rdi(iw.rs2));
                }
            }
            else if (iw.funct7 == 0b0100000) {                      // sub sra
                if (iw.funct3 == 0b000) {                           // sub
                    here << "sub " << regstr(iw.rd) << ',' << regstr(iw.rs1) << ',' << regstr(iw.rs2);
                    reg.wri(iw.rd, reg.rdi(iw.rs1) - reg.rdi(iw.rs2));
                }
                else if (iw.funct3 == 0b101) {                      // sra
                    here << "sra " << regstr(iw.rd) << ',' << regstr(iw.rs1) << ',' << regstr(iw.rs2);
                    reg.wri(iw.rd, s32(reg.rdi(iw.rs1)) >> s32(reg.rdi(iw.rs2)));
                }
                else {
                    illegal();
                }
            }
            else if (iw.funct7 == 0b0000001) {                      // mul mulh mulhsu mulhu div div rem remu
                if (iw.funct3 == 0x000) {                           // mul //
                    here << "mul " << regstr(iw.rd) << ',' << regstr(iw.rs1) << ',' << regstr(iw.rs2);
                    reg.wri(iw.rd, reg.rdi(iw.rs1) * reg.rdi(iw.rs2));
                }
                else if (iw.funct3 == 0b001) {                      // mulh //
                    here << "mulh " << regstr(iw.rd) << ',' << regstr(iw.rs1) << ',' << regstr(iw.rs2);
                    reg.wri(iw.rd, (s64(reg.rdi(iw.rs1)) * s64(reg.rdi(iw.rs2)) >> 32u));
                }
                else if (iw.funct3 == 0b010) {                      // mulhsu //
                    here << "mulhsu " << regstr(iw.rd) << ',' << regstr(iw.rs1) << ',' << regstr(iw.rs2);
                    reg.wri(iw.rd, (u64(reg.rdi(iw.rs1)) * s64(reg.rdi(iw.rs2)) >> 32u));
                }
                else if (iw.funct3 == 0b011) {                      // mulhu //
                    here << "mulhu " << regstr(iw.rd) << ',' << regstr(iw.rs1) << ',' << regstr(iw.rs2);
                    reg.wri(iw.rd, (u64(reg.rdi(iw.rs1)) * u64(reg.rdi(iw.rs2)) >> 32u));
                }
                else if (iw.funct3 == 0b100) {                      // div //
                    here << "div " << regstr(iw.rd) << ',' << regstr(iw.rs1) << ',' << regstr(iw.rs2);
                    reg.wri(iw.rd, s32(reg.rdi(iw.rs1)) / s32(reg.rdi(iw.rs2)));
                }
                else if (iw.funct3 == 0b101) {                      // divu //
                    here << "divu " << regstr(iw.rd) << ',' << regstr(iw.rs1) << ',' << regstr(iw.rs2);
                    reg.wri(iw.rd, reg.rdi(iw.rs1) / reg.rdi(iw.rs2));
                }
                else if (iw.funct3 == 0b110) {                      // rem //
                    here << "rem " << regstr(iw.rd) << ',' << regstr(iw.rs1) << ',' << regstr(iw.rs2);
                    // todo; "undefined behaviour" be very careful with this implementation
                    reg.wri(iw.rd, s32(reg.rdi(iw.rs1)) % s32(reg.rdi(iw.rs2)));
                }
                else if (iw.funct3 == 0b111) {                      // remu //
                    here << "remu " << regstr(iw.rd) << ',' << regstr(iw.rs1) << ',' << regstr(iw.rs2);
                    reg.wri(iw.rd, reg.rdi(iw.rs1) % reg.rdi(iw.rs2));
                }
            }
        }
        else if (op == 0b01101) {                                   // lui
            riscv_u_type iw(instruction);
            // overload comma operator? evil (it would print a comma)
            here << "lui " << regstr(iw.rd) << ',' << "0x" << hex_pad(iw.imm);
            reg.wri(iw.rd, iw.imm << 12);
        }
        else if (op == 0b01110) {                                   // 64 bit alu ops addw, subw, sllw, srlw, sraw //
            illegal();
        }
        else if (op == 0b01111) {                                   // illegal - actually variable instruction size encoding
            illegal();
        }
        else if (op == 0b10000) {                                   // fmadd.s //
            illegal();
        }
        else if (op == 0b10001) {                                   // fmsub.s //
            illegal();
        }
        else if (op == 0b10010) {                                   // fnmsub.s //
            illegal();
        }
        else if (op == 0b10011) {                                   // fnmadd.s //
            illegal();
        }
        else if (op == 0b10100) {                                   // floating point conversions/comparisons etc //
            illegal();
        }
        else if (op == 0b10101) {
            illegal();
        }
        else if (op == 0b10110) {
            illegal();
        }
        else if (op == 0b10111) {
            illegal();
        }
        else if (op == 0b11000) {
            riscv_b_type iw(instruction);
            if (iw.funct3 == 0b000) {
                here << "beq " << regstr(iw.rs1) << ',' << regstr(iw.rs2) << ',' << iw.simm();
                if (reg.rdi(iw.rs1) == reg.rdi(iw.rs2)) {           // beq //
                    pc_update = iw.imm;
                }
            }
            else if (iw.funct3 == 0b001) {                          // bne //
                here << "bne " << regstr(iw.rs1) << ',' << regstr(iw.rs2) << ',' << iw.simm();
                if (reg.rdi(iw.rs1) != reg.rdi(iw.rs2)) {
                    pc_update = iw.imm;
                }
            }
            else if (iw.funct3 == 0b010) {
                illegal();
            }
            else if (iw.funct3 == 0b011) {
                illegal();
            }
            else if (iw.funct3 == 0b100) {                          // blt //
                here << "blt " << regstr(iw.rs1) << ',' << regstr(iw.rs2) << iw.simm();
                if (s32(reg.rdi(iw.rs1) < s32(reg.rdi(iw.rs2)))) {
                    pc_update = iw.imm;
                }
            }
            else if (iw.funct3 == 0b101) {                          // bge //
                here << "bge " << regstr(iw.rs1) << ',' << regstr(iw.rs2) << iw.simm();
                if (s32(reg.rdi(iw.rs1)) != s32(reg.rdi(iw.rs2))) {
                    pc_update = iw.imm;
                }
            }
            else if (iw.funct3 == 0b110) {                          // bltu //
                here << "bltu " << regstr(iw.rs1) << ',' << regstr(iw.rs2) << ',' << iw.simm();
                if (reg.rdi(iw.rs1) < reg.rdi(iw.rs2)) {
                    pc_update = iw.imm;
                }
            }
            else if (iw.funct3 == 0b111) {                          // bgeu //
                here << "bgeu " << regstr(iw.rs1) << ',' << regstr(iw.rs2) << ',' << iw.simm();
                if (reg.rdi(iw.rs1) >= reg.rdi(iw.rs2)) {
                    pc_update = iw.imm;
                }
            }
        }
        else if (op == 0b11001) {                                   // jalr //
            riscv_i_type iw(instruction);
            if (iw.funct3 == 0b000) {
                here << "jalr " << regstr(iw.rd) << ',' << regstr(iw.simm(), iw.rs1);
                reg.wri(iw.rd, pc + 4);                             // note - if this instruction has come from the compressed stream this is +2
                pc_update = 0;
                pc = reg.rdi(iw.rs1) + iw.simm();
            }
            else {
                illegal();
            }
        }
        else if (op == 0b11010) {
            illegal();
        }
        else if (op == 0b11011) {                                   // jal //
            riscv_j_type iw(instruction);
            here << "jal " << regstr(iw.rd) << "," << iw.imm;
            reg.wri(iw.rd, pc + 4);                                 // note - if this instruction has come from the compressed stream this is +2
            pc_update = iw.imm;
        }
        else if (op == 0b11100) {
            riscv_i_type i(instruction);
            auto funct3 = extract(instruction, 12, 3);
            auto rd = extract(instruction, 7, 5);
            auto rs1 = extract(instruction, 15, 5);
            if (funct3 == 0b000 && rd == 0b00000 && rs1 == 0b00000) {
                auto imm = extract(instruction, 20, 12);
                if (imm == 0b000000000000) {                        // ecall //
                    here << "ecall";
                    if (ecall) {
                        ecall(this);
                    }
                }
                else if (imm == 0b000000000001) {                   // ebreak //
                    here << "ebreak";
                    if (ebreak) {
                        ebreak(this);
                    }
                }
                else {
                    illegal();
                }
            }
            else if (funct3 == 0b001) {                             // csrrw
                auto csr = extract(instruction, 20, 12);
                if (csr == riscv32_csr::csr_mvendorid) {
                    reg.wri(rs1,  0x6b63757f);
                }
            }
            else if (funct3 == 0b010) {                             // csrrs
            }
            else if (funct3 == 0b011) {                             // csrrc
            }
            else if (funct3 == 0b100) {                             // illegal
                illegal();
            }
            else if (funct3 == 0b101) {                             // csrrwi
            }
            else if (funct3 == 0b110) {                             // csrrsi
            }
            else if (funct3 == 0b111) {                             // csrrci
            }
        }
        else if (op == 0b11101) {
            illegal();
        }
        else if (op == 0b11110) {
            illegal();
        }
        else if (op == 0b11111) {
            illegal();
        }
    }
    else {
        illegal();
    }
    //
    pc += pc_update;
}

// todo csr's etc
std::string riscv_exec::dump() {
    std::stringstream ss;
    ss << "| pc  | " << hex_pad(pc,8) << " |\n";
    for (unsigned i = 0; i < riscv32_reg::int_reg_count; i++) {
        // 2 chars for this hex digit.
        ss << "| x" << hex_pad(i, 2) << " | " << hex_pad(reg.r[i], 8);
        if ((i & 0b11) == 0b11) {
            ss << " |\n";
        } else {
            ss << " ";
        }
    }
    ss << '\n';
    ss.flush();
    return ss.str();
}
