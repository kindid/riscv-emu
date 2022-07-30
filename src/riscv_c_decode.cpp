#include "riscv_c_decode.h"
#include "here.h"

s32 unpack_c_bcc_target(u16 instruction) {
    s32 offset = 0;
    offset |= extract(instruction, 3, 2) << 1;
    offset |= extract(instruction, 10, 11) << 3;
    offset |= extract(instruction, 2, 1) << 5;
    offset |= extract(instruction, 5, 2) << 6;
    offset |= extract(instruction, 12, 1) << 8;
    if (offset & (1u << 8u)) {
        offset |= 0xffffff00;
    }
    return offset;
}

s32 unpack_j_jal_target(u16 instruction) {
    s32 offset = 0;
    //        [11|4|9:8|10|6|7|3:1|5]
    // 6 5 4
    offset |= extract(instruction, 5, 1) << 1;
    offset |= extract(instruction, 3, 3) << 2;
    offset |= extract(instruction, 6, 1) << 7;
    offset |= extract(instruction, 7, 1) << 6;
    offset |= extract(instruction, 8, 1) << 10;
    offset |= extract(instruction, 9, 2) << 8;
    offset |= extract(instruction, 11, 2) << 4;
    offset |= extract(instruction, 12, 2) << 11;
    if (offset & (1u << 11u)) {
        offset |= ~((1u << 11u) - 1);
    }
    return offset;
}

u32 riscv_c_decode::decode(u16 instruction)
{
    //memory[pc];
    //        u32 regime = instruction & 0b11;
    //        if (regime == 0b11) {
    // read first half word first - if the lower bits are 0x11 then it's a 32 bit op (not quite true... but.. whatever)
    u32 op_len_fmt = extract(instruction, 0, 2);
    // todo; make this an 'expander' class that only exists when the 'c' instructions are enabled
    // "simply" rewrite op here
    if (op_len_fmt == 0b00) {
        pc_update = 2;
        auto funct3 = extract(instruction, 13, 3);
        if (funct3 == 0b000) {                          // c.addi4spn illegal //
            // WOAH! this is nowhere *near* this simple
//            u32 offset = extract(instruction, 5, 8) << 2;
            u32 offset = 0u;
            offset |= extract(instruction, 5, 1) << 3;
            offset |= extract(instruction, 6, 1) << 2;
            offset |= extract(instruction, 7, 4) << 6;
            offset |= extract(instruction, 11, 2) << 4;
            if (offset == 0) {
                illegal();
            }
            else {
    //            u32 address = reg.rd(2) + offset;
                u32 rd = extract(instruction, 2, 3);
                reg.wri(rd, reg.rdi(2) + offset);
    //            reg.wr(rd, _memory_device->read_u32(address));
            }
        }
        else if (funct3 == 0b001) {                     // c.fld c.lq //
            // no fld instructions yet
            illegal();
        }
        else if (funct3 == 0b010) {                     // c.lw //
            riscv_cl_type l(instruction);
            here << "c.lw " << "r" << l.rd << ",#" << hex_pad(l.imm) << "(" << l.rs1 << ")";
            reg.wri(l.rd + 8, _memory_device->read_u32(reg.rdi(l.rs1 + 8) + l.imm));
        }
        else if (funct3 == 0b011) {                     // c.flw c.ld //
            illegal();
        }
        else if (funct3 == 0b100) {                     // reserved //
            illegal();
        }
        else if (funct3 == 0b101) {                     // c.fsd c.sq //
            illegal();
        }
        else if (funct3 == 0b110) {                     // c.sw //
            riscv_cs_type s(instruction);
            here << "c.sw " << "r" << s.rs1 << ",#" << hex_pad(s.imm) << "(" << s.rs2 << ")";
            _memory_device->write(s.rs1 + s.imm, reg.rdi(s.rs2));


//            u32 offset = 0u;
//            offset |= extract(instruction, 6, 1) << 2;
//            offset |= extract(instruction, 10, 3) << 3;
//            offset |= extract(instruction, 5, 1) << 6;
//            u32 rs1 = extract(instruction, 7, 3);
//            u32 rs2 = extract(instruction, 2, 3);
//            u32 address = reg.rdi(rs1) + offset;
//            _memory_device->write(address, reg.rdi(rs2));
        }
        else if (funct3 == 0b111) {                     // c.fsw c.sd //
            illegal();
        }
    }
    else if (op_len_fmt == 0b01) {
        pc_update = 2;
        auto funct3 = extract(instruction, 13, 3);
        if (funct3 == 0b000) {                          // nop c.addi //
            auto immediate = extract(instruction, 2, 5);
            immediate |= extract(instruction, 12, 1) << 5;
            auto rds1 = extract(instruction, 7, 5);
            if (rds1 == 0) {
                illegal();
            } else {
                if (immediate & (1u << 5u)) {
                    immediate |= ~((1u << 5u) - 1);
                }
                if (immediate != 0) {
                    // TODO
                    here << immediate;
                    reg.wri(rds1, reg.rdi(rds1) + immediate);
                } else {
                    illegal();
                }
            }
        }
        else if (funct3 == 0b001) {                     // c.jal(32) c.addiw(64) //
            pc_update = 0;
            reg.wri(1, pc + 2);
            pc = unpack_j_jal_target(instruction);
        }
        else if (funct3 == 0b010) {                     // c.li //
            auto rd = extract(instruction, 7, 5);
            if (rd == 0) {
                illegal();
            } else {
                s32 immediate = 0;
                immediate |= extract(instruction, 2, 5);
                immediate |= extract(instruction, 12, 1) << 5;
                if (immediate & (1 << 5u)) {
                    immediate |= ~((1u << 5u) - 1);
                }
                reg.wri(rd, immediate);
            }
        }
        else if (funct3 == 0b011) {                     // c.addi16sp c.lui //
            auto rd = extract(instruction, 7, 5);
            if (rd == 2) {                              // c.addi16sp //
                // stack offset
                s32 immediate = 0;
                immediate |= extract(instruction, 2, 1) << 4;
                immediate |= extract(instruction, 3, 2) << 7;
                immediate |= extract(instruction, 5, 1) << 6;
                immediate |= extract(instruction, 6, 1) << 4;
                if (extract(instruction, 12, 1) == 1) {
                    immediate |= ~((1 << 9) - 1);
                }
                reg.wri(2, 2 + immediate);
            }
            else if (rd == 0) {
                illegal();
            } else {
                s32 immediate = 0;
                immediate |= extract(instruction, 2, 5) << 12;
                immediate |= extract(instruction, 12, 1) << 17;
                if (immediate & (1 << 17u)) {
                    immediate |= ~((1u << 17u) - 1);
                }
                reg.wri(rd, immediate);
            }
        }
        else if (funct3 == 0b100) {                     // c.srli c.srli64 c.srai c.srai64 c.adni c.sub c.xor c.or c.and c.subw c.addw //
            auto rds = extract(instruction, 7, 3);
            auto funct2 = extract(instruction, 10, 2);
            if (funct2 == 0b00) {                       // c.srli
                s32 shamt = 0;
                shamt |= extract(instruction, 2, 4);
                if (extract(instruction, 12, 1)) {      // test
                    shamt |= ~((1 << 6) - 1);
                }
                reg.wri(rds, reg.rdi(rds) >> shamt);           // todo; can't be 0
            }
            else if (funct2 == 0b01) {                  // c.srai
                s32 shamt = 0;
                shamt |= extract(instruction, 2, 4);
                if (extract(instruction, 12, 1)) {      // test
                    shamt |= ~((1 << 6) - 1);           // todo; platform agnostic "undefined behaviour of signed right shift"
                }
                reg.wri(rds, (u32) ((s32)(reg.rdi(rds)) >> shamt));  // todo; can't be 0
            }
            else if (funct2 == 0b10) {
                s32 immediate = 0;
                immediate |= extract(instruction, 2, 4);
                if (extract(instruction, 12, 1)) {      // test
                    immediate |= ~((1 << 6) - 1);
                }
                reg.wri(rds, reg.rdi(rds) & immediate);
            }
            else if (funct2 == 0b11) {                      // c.sub c.xor c.or c.and c.subw(rv64) c.addw(rv64) res res
                if (!(instruction & (1u << 12))) {          // c.sub c.xor c.or c.and
                    auto funct52 = extract(instruction, 5, 2);
                    auto rs2 = extract(instruction, 2, 3);
                    if (funct52 == 0b00) {                  // c.sub
                        reg.wri(rds, reg.rdi(rds) - rs2);
//                        reg[rds] = reg[rds] - rs2;
                    }
                    else if (funct52 == 0b01) {             // c.xor
                        reg.wri(rds, reg.rdi(rds) ^ rs2);
                        //reg[rds] = reg[rds] ^ rs2;
                    }
                    else if (funct52 == 0b10) {             // c.or
                        reg.wri(rds, reg.rdi(rds) | rs2);
//                        reg[rds] = reg[rds] | rs2;
                    }
                    else if (funct52 == 0b11) {             // c.and
                        reg.wri(rds, reg.rdi(rds) & rs2);
//                        reg[rds] = reg[rds] & rs2;
                    }
                }
                else {                                      // c.subw(rv64) c.addw(rv64) res res
                    illegal();
                }
            }
        }
        else if (funct3 == 0b101) {                     // c.j //
            // 11 ,4 ,9:8, 10, 6, 7, 3:1, 5
            pc_update = 0;
            pc = unpack_j_jal_target(instruction);
        }
        else if (funct3 == 0b110) {                     // c.beqz //
            auto rs1 = extract(instruction, 7, 3);
            s32 offset = unpack_c_bcc_target(instruction);
            if (reg.rdi(rs1) == 0) {
                pc = pc + offset;
                pc_update = 0;
            }
        }
        else if (funct3 == 0b111) {                     // c.bnez //
            auto rs1 = extract(instruction, 7, 3);
            s32 offset = unpack_c_bcc_target(instruction);
            if (reg.rdi(rs1) != 0) {
                pc = pc + offset;
                pc_update = 0;
            }
        }
    }
    else if (op_len_fmt == 0b10) {
        pc_update = 2;
        auto funct3 = extract(instruction, 13, 3);
        if (funct3 == 0b000) {                          // c.slli c.slli64 //
            auto rds = extract(instruction, 7, 3);
            s32 shamt = 0;
            shamt |= extract(instruction, 2, 4);
            if (extract(instruction, 12, 1)) {      // test; todo; can't be 0
                shamt |= ~((1 << 6) - 1);
            }
            reg.wri(rds, reg.rdi(rds) >> shamt);
        }
        else if (funct3 == 0b001) {                     // c.fldsp c.lqsp //
            illegal();
        }
        else if (funct3 == 0b010) {                     // c.lwsp //
            // immediate = 2:3 -> 6:7, 4:6 -> 2:4, 12 -> 5
            u32 immediate = 0;
            immediate |= extract(instruction, 4, 3) << 2;
            immediate |= extract(instruction, 12, 1) << 5;
            immediate |= extract(instruction, 2, 2) << 6;
            auto rd = extract(instruction, 7, 3);
            reg.wri(rd, _memory_device->read_u32(reg.rdi(rd) + immediate));
        }
        else if (funct3 == 0b011) {                     // c.flwsp c.ldsp //
            illegal();
        }
        else if (funct3 == 0b100) {                     // c.jr c.mv c.ebreak c.jalr c.add //
            auto rds1 = extract(instruction, 7, 5);
            auto rs2 = extract(instruction, 2, 5);
            auto decider = extract(instruction, 12, 1);
            if (rs2 == 0) {                             // c.jr c.ebreak c.jalr
                if (rds1 == 0) {
                    if (decider == 0) {                 // illegal???
                        illegal();
                    }
                    else {                              // c.ebreak

                    }
                }
                else {                                  // c.jr c.mv c.jalr c.add
                    if (decider == 0) {
                        illegal();
                    }
                    else {                              // c.ebreak

                    }
                }
            }
            else {                                      // c.mv c.add
                if (rds1 == 0) {                         // illegal
                    illegal();
                }
                else {
                    if (decider == 0) {                 // c.mv (expands to add rd, x0, rs2)
                        reg.wri(rds1, reg.rdi(rs2));
                    }
                    else {                              // c.add
                        reg.wri(rds1, reg.rdi(rds1) + reg.rdi(rs2));
                    }
                }
            }
        }
        else if (funct3 == 0b101) {                     // c.fsdsp c.sqsp //
            illegal();
        }
        else if (funct3 == 0b110) {                     // c.swsp //
            auto rs2 = extract(instruction, 2, 5);
            u32 immediate = 0;
            immediate |= extract(instruction, 7, 2) << 6;
            immediate |= extract(instruction, 9, 4) << 2;
            _memory_device->write(reg.rdi(2) + immediate, reg.rdi(rs2));
        }
        else if (funct3 == 0b111) {                     // c.fswsp c.sdsp //
            illegal();
        }
    }
    return 0;
}
