#include "riscv_exec.h"
#include "riscv_instruction_formats.h"

#include "bitops.h"
#include "bits.h"

#include <sstream>

#define DEBUG_ON
#include "here.h"

// todo - template
//  allow compile time setting of memory device, register set (static polymor

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
    // print it. get the first byte - if the bottom bits are 11 then it's a 32 bit instruction
    // i don't bother with the larger ones
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

void riscv_exec::execute()
{
    // todo; this does *not* support 'c' mode (yet)
    u32 instruction = u32(_memory_device->read_u32(pc));
//    here << hex_pad(pc) << " : " << hex_pad(instruction);// << " : " << hex_pad(u64(_memory_device->memory)) << "\n";
    //
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
    bits<32> tmp_instr(instruction);
    auto op_len = tmp_instr.extract<0,2>();

    u32 op_len_fmt = extract(instruction, 0, 2);
//    here << h_wrap(op_len_fmt);
    if (op_len == 11_b2) {
        // the output of this is a 32 bit instruction which can be parsed with the code below - nice

        // 32 bit instruction so read the hi word
        //instruction = instruction | (u32(_memory_device->read_u16(pc + 2)) << 16u);

//        u32 decider = extract(instruction, 25, 7);
        //u32 op = extract(instruction, 2, 5);
        auto op = tmp_instr.extract<2, 5>();
//                extract(instruction, 2, 5);
        //here << binary_string(bits<5>(op));

        if (op == 00000_b5) {
            riscv_i_type i(instruction);
            if (i.funct3 == 0b00) {                                 // lb //
                here << "lb " << regstr(i.rd) << "," << regstr(i.simm(), i.rs1);
                reg.wri(i.rd, sign_extend(_memory_device->read_u8(reg.rdi(i.rs1) + i.simm())));
            }
            else if (i.funct3 == 0b001) {                           // lh //
                here << "lh " << regstr(i.rd) << "," << regstr(i.simm(), i.rs1);
                reg.wri(i.rd, sign_extend(_memory_device->read_u16(reg.rdi(i.rs1) + i.simm())));
            }
            else if (i.funct3 == 0b010) {                           // lw //
                here << "lw " << regstr(i.rd) << "," << regstr(i.simm(), i.rs1);
                reg.wri(i.rd, _memory_device->read_u32(reg.rdi(i.rs1) + i.simm()));
            }
            else if (i.funct3 == 0b011) {                           // illegal //
                illegal();
            }
            else if (i.funct3 == 0b100) {                           // lbu //
                here << "lbu " << regstr(i.rd) << "," << regstr(i.simm(), i.rs1);
                reg.wri(i.rd, u32(_memory_device->read_u8(reg.rdi(i.rs1) + i.simm())));
            }
            else if (i.funct3 == 0b101) {                           // lhu //
                here << "lhu " << regstr(i.rd) << "," << regstr(i.simm(), i.rs1);
                reg.wri(i.rd, u32(_memory_device->read_u16(reg.rdi(i.rs1) + i.simm())));
            }
            else if (i.funct3 == 0b110) {                           // illegal //
                illegal();
            }
            else if (i.funct3 == 0b111) {                           // illegal //
                illegal();
            }
        }
        else if (op == 00001_b5) {                                   // flw (rv32f)
            illegal();
        }
        else if (op == 00010_b5) {
            illegal();
        }
        else if (op == 00011_b5) {                             // fence fence.i
            // ?12..14
            // 000 fence  24..27 = pred, succ = 23..20. all others 0
            // 001 fence.i. all others 0
        }
        else if (op == 00100_b5) {                             // immediate instructions //
            riscv_i_type i(instruction);
            if (i.funct3 == 0b000) {                              // addi //
//                addi	x13,x15,1024 # 1400 <main-0xec54>
                // c/c++ problem. u8 is treated as a character - it clearly ain't
                here << "addi " << regstr(i.rd) << "," << regstr(i.rs1) << ',' << i.simm();
                reg.wri(i.rd, reg.rdi(i.rs1) + i.simm());
            }
            else if (i.funct3 == 0b001) {                         // slli //
                // todo; finish error checking (top bits) -> 31..25 are 0. anything else is a fail
                u32 decider = extract(i.imm, 5, 8);
                if (decider == 0b0000000) {
                    here << "slli " << regstr(i.rd) << "," << regstr(i.rs1) << hex_pad(i.simm());
//                            'x' << u32(i.rd) << ",x" << u32(i.rs1) << ',' << i.simm();
                    u32 shamt = extract(i.imm, 0, 5);
                    reg.wri(i.rd, reg.rdi(i.rs1) << shamt);
                } else {
                    illegal();
                }
            }
            else if (i.funct3 == 0b010) {                         // slti //
                here << "slti " << regstr(i.rd) << "," << regstr(i.rs1) << ',' << i.simm();
                if (s32(reg.rdi(i.rs1)) < i.simm()) {
                    reg.wri(i.rd, 1);
                } else {
                    reg.wri(i.rd,  0);
                }
            }
            else if (i.funct3 == 0b011) {                         // sltiu //
                here << "sltiu " << regstr(i.rd) << "," << regstr(i.rs1) << ',' << i.imm;
                if (reg.rdi(i.rs1) < i.imm) {
                    reg.wri(i.rd, 1);
                } else {
                    reg.wri(i.rd, 0);
                }
            }
            else if (i.funct3 == 0b100) {                         // xori //
                here << "xori " << regstr(i.rd) << "," << regstr(i.rs1) << ',' << i.simm();
                reg.wri(i.rd, reg.rdi(i.rs1) ^ i.simm());
            }
            else if (i.funct3 == 0b101) {                         // srli srai //
                // todo; check bits 25,26?. 26 = 0 but 25 = 'X' (eh?) i think this is doc cockup
                //u32 decider = extract(instruction, 25, 7);
                u32 decider = extract(i.imm, 5, 8);
                if (decider == 0b0000000) {                     // srli //
                    u32 shamt = extract(i.imm, 0, 5);
                    here << "srli " << regstr(i.rd) << ',' << regstr(i.rs1) << ',' << shamt;
                    // todo; move to 'bits' for this
                    reg.wri(i.rd, u32(s32(reg.rdi(i.rs1)) >> shamt));
                }
                else if (decider == 0b0100000) {                // srai //
                    u32 shamt = extract(instruction, 19, 5);
                    here << "srai " << regstr(i.rd) << ',' << regstr(i.rs1) << ',' << shamt;
                    reg.wri(i.rd, u32(s32(reg.rdi(i.rs1)) >> shamt));
                }
                else {
                    illegal();
                }
            }
            else if (i.funct3 == 0b110) {                         // ori //
                here << "ori " << regstr(i.rd) << ',' << regstr(i.rs1) << ',' << i.simm();
                reg.wri(i.rd, reg.rdi(i.rs1) | i.simm());
            }
            else if (i.funct3 == 0b111) {                         // andi //
                here << "andi " << regstr(i.rd) << ',' << regstr(i.rs1) << ',' << i.simm();
                reg.wri(i.rd, reg.rdi(i.rs1) & i.simm());
            }
        }
        else if (op == 00101_b5) {                             // auipc //
            riscv_u_type u(instruction);
            here << "aupic " << "r" << u.rd << "," << "0x" << hex_pad(u.imm);
            reg.wri(u.rd, pc + (u.imm << 12u));
        }
        else if (op == 00110_b5) {                             // 64bit addiw, slliw, srliw, sraiw //
            illegal();
        }
        else if (op == 00111_b5) {
            illegal();
        }
        else if (op == 01000_b5) {
            riscv_s_type s(instruction);
            if (s.funct3 == 0b000) {                            // sb
                here << "sb " << regstr(s.rs2) << "," << regstr(s.simm(), s.rs1);
                _memory_device->write(reg.rdi(s.rs1) + s.simm(), u8(reg.rdi(s.rs2)));
            }
            else if (s.funct3 == 0b001) {                       // sh
                here << "sh " << regstr(s.rs2) << "," << regstr(s.simm(), s.rs1);
                _memory_device->write(reg.rdi(s.rs1) + s.simm(), u16(reg.rdi(s.rs2)));
            }
            else if (s.funct3 == 0b010) {                       // sw
                here << "sw " << regstr(s.rs2) << "," << regstr(s.simm(), s.rs1);
                _memory_device->write(reg.rdi(s.rs1) + s.simm(), u32(reg.rdi(s.rs2)));
            }
            else if (s.funct3 == 0b011) {                       // sd (rv64)
                illegal();
            }
            else {
                illegal();
            }
        }
        else if (op == 01001_b5) {                             // fsw (f), fsd (d) //
            illegal();
        }
        else if (op == 01010_b5) {
            illegal();
        }
        else if (op == 01011_b5) {                             // rv32a (atomics) //
            illegal();
        }
        else if (op == 01100_b5) {
            riscv_r_type r(instruction);            // not liking this. call them all 'parts' or something
            if (r.funct7 == 0b0000000) {                        // add sll slt sltu xor srl or and
                if (r.funct3 == 0x000) {                        // add //
                    here << "add " << regstr(r.rd) << ',' << regstr(r.rs1) << ',' << regstr(r.rs2);
                    reg.wri(r.rd, reg.rdi(r.rs1) + reg.rdi(r.rs2));
                }
                else if (r.funct3 == 0b001) {                   // sll //
                    here << "sll " << regstr(r.rd) << ',' << regstr(r.rs1) << ',' << regstr(r.rs2);
                    reg.wri(r.rd, reg.rdi(r.rs1) << reg.rdi(r.rs2));
                }
                else if (r.funct3 == 0b010) {                   // slt //
                    here << "slt " << regstr(r.rd) << ',' << regstr(r.rs1) << ',' << regstr(r.rs2);
                    if ((s32)(reg.rdi(r.rs1) < (s32)(reg.rdi(r.rs2)))) {
                        reg.wri(r.rd, 1);
                    } else {
                        reg.wri(r.rd,  0);
                    }
                }
                else if (r.funct3 == 0b011) {                   // sltu //
                    here << "sltu " << regstr(r.rd) << ',' << regstr(r.rs1) << ',' << regstr(r.rs2);
                    if (reg.rdi(r.rs1) < reg.rdi(r.rs2)) {
                        reg.wri(r.rd, 1);
                    } else {
                        reg.wri(r.rd,  0);
                    }
                }
                else if (r.funct3 == 0b100) {                   // xor //
                    here << "xor " << regstr(r.rd) << ',' << regstr(r.rs1) << ',' << regstr(r.rs2);
                    reg.wri(r.rd, reg.rdi(r.rs1) ^ reg.rdi(r.rs2));
                }
                else if (r.funct3 == 0b101) {                   // srl //
                    here << "srl " << regstr(r.rd) << ',' << regstr(r.rs1) << ',' << regstr(r.rs2);
                    reg.wri(r.rd, reg.rdi(r.rs1) >> reg.rdi(r.rs2));
                }
                else if (r.funct3 == 0b110) {                   // or //
                    here << "or " << regstr(r.rd) << ',' << regstr(r.rs1) << ',' << regstr(r.rs2);
                    reg.wri(r.rd, reg.rdi(r.rs1) | reg.rdi(r.rs2));
                }
                else if (r.funct3 == 0b111) {                   // and //
                    here << "amd " << regstr(r.rd) << ',' << regstr(r.rs1) << ',' << regstr(r.rs2);
                    reg.wri(r.rd, reg.rdi(r.rs1) & reg.rdi(r.rs2));
                }
            }
            else if (r.funct7 == 0b0100000) {                   // sub sra
                if (r.funct3 == 0b000) {                        // sub
                    here << "sub " << regstr(r.rd) << ',' << regstr(r.rs1) << ',' << regstr(r.rs2);
                    reg.wri(r.rd, reg.rdi(r.rs1) - reg.rdi(r.rs2));
                }
                else if (r.funct3 == 0b101) {                   // sra
                    here << "sra " << regstr(r.rd) << ',' << regstr(r.rs1) << ',' << regstr(r.rs2);
                    reg.wri(r.rd, s32(reg.rdi(r.rs1)) >> s32(reg.rdi(r.rs2)));
                }
                else {
                    illegal();
                }
            }
            else if (r.funct7 == 0b0000001) {                   // mul mulh mulhsu mulhu div div rem remu
                if (r.funct3 == 0x000) {                        // mul //
                    here << "mul " << regstr(r.rd) << ',' << regstr(r.rs1) << ',' << regstr(r.rs2);
                    reg.wri(r.rd, reg.rdi(r.rs1) * reg.rdi(r.rs2));
                }
                else if (r.funct3 == 0b001) {                   // mulh //
                    here << "mulh " << regstr(r.rd) << ',' << regstr(r.rs1) << ',' << regstr(r.rs2);
                    reg.wri(r.rd, (s64(reg.rdi(r.rs1)) * s64(reg.rdi(r.rs2)) >> 32u));
                }
                else if (r.funct3 == 0b010) {                   // mulhsu //
                    here << "mulhsu " << regstr(r.rd) << ',' << regstr(r.rs1) << ',' << regstr(r.rs2);
                    reg.wri(r.rd, (u64(reg.rdi(r.rs1)) * s64(reg.rdi(r.rs2)) >> 32u));
                }
                else if (r.funct3 == 0b011) {                   // mulhu //
                    here << "mulhu " << regstr(r.rd) << ',' << regstr(r.rs1) << ',' << regstr(r.rs2);
                    reg.wri(r.rd, (u64(reg.rdi(r.rs1)) * u64(reg.rdi(r.rs2)) >> 32u));
                }
                else if (r.funct3 == 0b100) {                   // div //
                    here << "div " << regstr(r.rd) << ',' << regstr(r.rs1) << ',' << regstr(r.rs2);
                    reg.wri(r.rd, s32(reg.rdi(r.rs1)) / s32(reg.rdi(r.rs2)));
                }
                else if (r.funct3 == 0b101) {                   // divu //
                    here << "divu " << regstr(r.rd) << ',' << regstr(r.rs1) << ',' << regstr(r.rs2);
                    reg.wri(r.rd, reg.rdi(r.rs1) / reg.rdi(r.rs2));
                }
                else if (r.funct3 == 0b110) {                   // rem //
                    // todo; i very much doubt this is correct - "undefined behaviour"
                    here << "rem " << regstr(r.rd) << ',' << regstr(r.rs1) << ',' << regstr(r.rs2);
                    reg.wri(r.rd, s32(reg.rdi(r.rs1)) % s32(reg.rdi(r.rs2)));
                }
                else if (r.funct3 == 0b111) {                   // remu //
                    here << "remu " << regstr(r.rd) << ',' << regstr(r.rs1) << ',' << regstr(r.rs2);
                    reg.wri(r.rd, reg.rdi(r.rs1) % reg.rdi(r.rs2));
                }
            }
        }
        else if (op == 01101_b5) {                              // lui
            riscv_u_type u(instruction);
            // overload comma operator? evil (it would print a comma)
            here << "lui " << regstr(u.rd) << ',' << "0x" << hex_pad(u.imm);
            reg.wri(u.rd, u.imm << 12);
        }
        else if (op == 01110_b5) {                             // 64 bit alu ops addw, subw, sllw, srlw, sraw //
            illegal();
        }
        else if (op == 01111_b5) {                             // illegal (something to do with variable offsets instruction encoding???)
            illegal();
        }
        else if (op == 10000_b5) {                             // fmadd.s //
            illegal();
        }
        else if (op == 10001_b5) {                             // fmsub.s //
            illegal();
        }
        else if (op == 10010_b5) {                             // fnmsub.s //
            illegal();
        }
        else if (op == 10011_b5) {                             // fnmadd.s //
            illegal();
        }
        else if (op == 10100_b5) {                             // floating point conversions/comparisons etc //
            illegal();
        }
        else if (op == 10101_b5) {
            illegal();
        }
        else if (op == 10110_b5) {
            illegal();
        }
        else if (op == 10111_b5) {
            illegal();
        }
        else if (op == 11000_b5) {
            riscv_b_type b(instruction);
            if (b.funct3 == 0b000) {
                // fix this imm output - it is genuinely signed
                here << "beq " << regstr(b.rs1) << ',' << regstr(b.rs2) << ',' << b.simm();
                if (reg.rdi(b.rs1) == reg.rdi(b.rs2)) {         // beq //
                    pc_update = b.imm;
                }
            }
            else if (b.funct3 == 0b001) {                         // bne //
                here << "bne " << regstr(b.rs1) << ',' << regstr(b.rs2) << ',' << b.simm();
                if (reg.rdi(b.rs1) != reg.rdi(b.rs2)) {
                    pc_update = b.imm;
                }
            }
            else if (b.funct3 == 0b010) {
                illegal();
            }
            else if (b.funct3 == 0b011) {
                illegal();
            }
            else if (b.funct3 == 0b100) {                         // blt //
                here << "blt " << regstr(b.rs1) << ',' << regstr(b.rs2) << b.simm();
                if (s32(reg.rdi(b.rs1) < s32(reg.rdi(b.rs2)))) {
                    pc_update = b.imm;
                }
            }
            else if (b.funct3 == 0b101) {                         // bge //
                here << "bge " << regstr(b.rs1) << ',' << regstr(b.rs2) << b.simm();
                if (s32(reg.rdi(b.rs1)) != s32(reg.rdi(b.rs2))) {
                    pc_update = b.imm;
                }
            }
            else if (b.funct3 == 0b110) {                         // bltu //
                here << "bltu " << regstr(b.rs1) << ',' << regstr(b.rs2) << ',' << b.simm();
                if (reg.rdi(b.rs1) < reg.rdi(b.rs2)) {
                    pc_update = b.imm;
                }
            }
            else if (b.funct3 == 0b111) {                         // bgeu //
                here << "bgeu " << regstr(b.rs1) << ',' << regstr(b.rs2) << ',' << b.simm();
                if (reg.rdi(b.rs1) >= reg.rdi(b.rs2)) {
                    pc_update = b.imm;
                }
            }
        }
        else if (op == 11001_b5) {                             // jalr //
            riscv_i_type i(instruction);
            if (i.funct3 == 0b000) {
                here << "jalr " << regstr(i.rd) << ',' << regstr(i.simm(), i.rs1);
                reg.wri(i.rd, pc + 4);    // note - if this instruction has come from the compressed stream this is +2
                pc_update = 0;
                pc = reg.rdi(i.rs1) + i.simm();
            }
            else {
                // i wonder what these are for?
                illegal();
            }
        }
        else if (op == 11010_b5) {
            illegal();
        }
        else if (op == 11011_b5) {                             // jal //
            riscv_j_type j(instruction);
            here << "jal " << regstr(j.rd) << "," << j.imm; //hex_pad(j.imm);
            reg.wri(j.rd, pc + 4);  // todo; in compressed mode t
            pc_update = j.imm;
        }
        else if (op == 11100_b5) {
            riscv_i_type i(instruction);
            auto funct3 = extract(instruction, 12, 3);
            auto rd = extract(instruction, 7, 5);
            auto rs1 = extract(instruction, 15, 5);
            if (funct3 == 0b000 && rd == 0b00000 && rs1 == 0b00000) {                              // ecall ebreak //
                auto imm = extract(instruction, 20, 12);
                if (imm == 0b000000000000) {                    // ecall //
                    here << "ecall";
                }
                else if (imm == 0b000000000001) {               // ebreak //
                    here << "ebreak";
                }
                else {
                    illegal();
                }
            }
            else if (funct3 == 0b001) {                         // csrrw
                auto csr = extract(instruction, 20, 12);
                if (csr == riscv32_csr::csr_mvendorid) {
                    reg.wri(rs1,  0x6b63757f);
                }
            }
            else if (funct3 == 0b010) {                         // csrrs
            }
            else if (funct3 == 0b011) {                         // csrrc
            }
            else if (funct3 == 0b100) {                         // illegal
                illegal();
            }
            else if (funct3 == 0b101) {                         // csrrwi
            }
            else if (funct3 == 0b110) {                         // csrrsi
            }
            else if (funct3 == 0b111) {                         // csrrci
            }
        }
        else if (op == 11101_b5) {
            illegal();
        }
        else if (op == 11110_b5) {
            illegal();
        }
        else if (op == 11111_b5) {
            illegal();
        }
    }
    else {
        illegal();
    }
    // objectify
    pc += pc_update;
}

// todo csr's etc
std::string riscv_exec::dump() {
    //        std::string s;
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
    // std::cout << "MADE IT " << s;
    // return s;
    //        return "FARGDIOSDF";
}
