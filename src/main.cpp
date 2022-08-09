#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <type_traits>
#include <array>

#include "types.h"
#include "memory_device.h"

#define DEBUG_ON
#include "here.h"

#include "riscv_exec.h"

// todo;read from json - string of hex etc - mem block, plug in a run

struct code_block {
    u32 address;
    std::vector<u8> code;
};

std::vector<code_block> code_blocks = {
    {
        0x00010054, {                    // main
            0xb7, 0xb7, 0x77, 0x5e,        // lui	x15,0x5e77b
            0x37, 0x17, 0x00, 0x00,        // lui	x14,0x1
            0x93, 0x87, 0xe7, 0xb1,        // addi	x15,x15,-1250 # 5e77ab1e <__global_pointer$+0x5e7692b2>
            0x23, 0x20, 0xf7, 0x00,        // sw	x15,0(x14) # 1000 <main-0xf054>
            0x73, 0x00, 0x10, 0x00,        // ebreak
            0x67, 0x80, 0x00, 0x00         // jalr	x0,0(x1)
        }
    },
};

#if 0
std::vector<code_block> code_blocks = {
    {
        0x00010054, {                    // main
            0x13, 0x01, 0x01, 0xff,        // addi	x2,x2,-16
            0x23, 0x26, 0x11, 0x00,        // sw	x1,12(x2)
            0xef, 0x00, 0x00, 0x03,        // jal	x1,1008c <process>
            0x73, 0x00, 0x10, 0x00,        // ebreak
            0x83, 0x20, 0xc1, 0x00,        // lw	x1,12(x2)
            0x13, 0x01, 0x01, 0x01,        // addi	x2,x2,16
            0x67, 0x80, 0x00, 0x00         // jalr	x0,0(x1)
        }
    }, {
        0x00010070, {                    // fill
            0xb7, 0x17, 0x00, 0x00,        // lui	x15,0x1
            0x13, 0x87, 0x07, 0x04,        // addi	x14,x15,64 # 1040 <main-0xf014>
            0xb3, 0x86, 0xf7, 0x02,        // mul	x13,x15,x15
            0x93, 0x87, 0x17, 0x00,        // addi	x15,x15,1
            0xa3, 0x8f, 0xd7, 0xfe,        // sb	x13,-1(x15)
            0xe3, 0x9a, 0xe7, 0xfe,        // bne	x15,x14,10078 <fill+0x8>
            0x67, 0x80, 0x00, 0x00         // jalr	x0,0(x1)
        }
    }, {
        0x0001008c, {                    // process
            0x13, 0x01, 0x01, 0xff,        // addi	x2,x2,-16
            0x23, 0x26, 0x11, 0x00,        // sw	x1,12(x2)
            0xef, 0xf0, 0xdf, 0xfd,        // jal	x1,10070 <fill>
            0xb7, 0x17, 0x00, 0x00,        // lui	x15,0x1
            0x93, 0x86, 0x07, 0x04,        // addi	x13,x15,64 # 1040 <main-0xf014>
            0x03, 0xa7, 0x07, 0x00,        // lw	x14,0(x15)
            0x93, 0x87, 0x47, 0x00,        // addi	x15,x15,4
            0x13, 0x47, 0xf7, 0xff,        // xori	x14,x14,-1
            0x23, 0xae, 0xe7, 0xfe,        // sw	x14,-4(x15)
            0xe3, 0x98, 0xd7, 0xfe,        // bne	x15,x13,100a0 <process+0x14>
            0x83, 0x20, 0xc1, 0x00,        // lw	x1,12(x2)
            0x13, 0x01, 0x01, 0x01,        // addi	x2,x2,16
            0x67, 0x80, 0x00, 0x00         // jalr	x0,0(x1)
        }
    }
};
#endif
#if 0
std::vector<code_block> code_blocks = {
    {
        0x00010054, {
            0x6f, 0x00, 0x00, 0x02,      // jal     x0, 10074 <process>
        },
    }, {
        0x00010058, {
            0xb7, 0x17, 0x00, 0x00,      // lui     x15, 0x1
            0x93, 0x86, 0x07, 0x40,      // addi    x13, x15, 1024 # 1400 <main-0xec54>
            0x33, 0x87, 0xf7, 0x02,      // mul     x14, x15, x15
            0x93, 0x87, 0x17, 0x00,      // addi    x15, x15, 1
            0xa3, 0x8f, 0xe7, 0xfe,      // sb      x14, -1(x15)
            0xe3, 0x9a, 0xd7, 0xfe,      // bne     x15, x13, 10060 <fill+0x8>
            0x67, 0x80, 0x00, 0x00,      // jalr    x0, 0(x1)
        },
    }, {
        0x00010074, {
            0x13, 0x01, 0x01, 0xff,      // addi    x2, x2, -16
            0x23, 0x26, 0x11, 0x00,      // sw      x1, 12(x2)
            0xef, 0xf0, 0xdf, 0xfd,      // jal     x1, 10058 <fill>
            0xb7, 0x17, 0x00, 0x00,      // lui     x15, 0x1
            0x93, 0x86, 0x07, 0x40,      // addi    x13, x15, 1024 # 1400 <main-0xec54>
            0x03, 0xa7, 0x07, 0x00,      // lw      x14, 0(x15)
            0x93, 0x87, 0x47, 0x00,      // addi    x15, x15, 4
            0x13, 0x47, 0xf7, 0xff,      // xori    x14, x14, -1
            0x23, 0xae, 0xe7, 0xfe,      // sw      x14, -4(x15)
            0xe3, 0x98, 0xd7, 0xfe,      // bne     x15, x13, 10088 <process+0x14>
            0x83, 0x20, 0xc1, 0x00,      // lw      x1, 12(x2)
            0x13, 0x01, 0x01, 0x01,      // addi    x2, x2, 16
            0x67, 0x80, 0x00, 0x00,      // jalr    x0, 0(x1)
        }
    }
};

std::vector<code_block> code_blocks = {
    {
        0x00010054, {
            0x67, 0x80, 0x00, 0x00,      // jalr	x0,0(x1)
        }
    }
};
#endif

bool kill = false;

void illegal_cb(riscv_exec * re)
{
    here << "DEAD" << hex_pad(re->pc);
    kill = true;
}

void ebreak(riscv_exec * re)
{
    here << "KILLING";
    kill = true;
}

int main(int argc, char *argv[])
{
    here << "kindid risc v emu\n";

    u8 raw_memory[65536];   // RAM
    memory_device memory(raw_memory, 65536);

    riscv_exec cpu(&memory);
    cpu.illegal_cb = ::illegal_cb;
    cpu.ebreak = ::ebreak;
    //cpu.ebreak = ::ecall;
    //
    cpu.pc = code_blocks[0].address;
    cpu.reg.wri(2, 0x800);
    //
    for (auto & cb : code_blocks) {
        for (u32 offset = 0; offset < cb.code.size(); offset++) {
            raw_memory[cb.address + offset] = cb.code[offset];
        }
    }

    while (!kill) {
        cpu.execute();
    }

    for (u32 i = 0; i < 32; i++) {
        here << hex_pad(i+4096) << " @ " << hex_pad(raw_memory[i + 4096]);// << " == " << hex_pad(~(i * i));
    }

    return 0;
}
