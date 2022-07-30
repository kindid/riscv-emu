////////////////////////////////////////////////////////////////////////////////
/// (C) 2022 kindid.co.uk
/// MIT licensed
/// RISCV-32 emulator written for educational purposes
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>

#include "types.h"
#include "memory_device.h"
#include "riscv32_reg.h"

struct riscv32_csr {
    constexpr static u32 csr_mstatus =          0x300;
    constexpr static u32 csr_misa =             0x301;
    constexpr static u32 csr_medeleg =          0x302;
    constexpr static u32 csr_mideleg =          0x303;
    constexpr static u32 csr_mie =              0x304;
    constexpr static u32 csr_mtvec =            0x305;

    constexpr static u32 csr_mucounteren =      0x320;
    constexpr static u32 csr_mscounteren =      0x321;
    constexpr static u32 csr_mhcounteren =      0x322;
    constexpr static u32 csr_mhpmevent3 =       0x323;
    constexpr static u32 csr_mhpmevent4 =       0x324;
    constexpr static u32 csr_mhpmevent5 =       0x325;
    constexpr static u32 csr_mhpmevent6 =       0x326;
    constexpr static u32 csr_mhpmevent7 =       0x327;
    constexpr static u32 csr_mhpmevent8 =       0x328;
    constexpr static u32 csr_mhpmevent9 =       0x329;
    constexpr static u32 csr_mhpmevent10 =      0x32a;
    constexpr static u32 csr_mhpmevent11 =      0x32b;
    constexpr static u32 csr_mhpmevent12 =      0x32c;
    constexpr static u32 csr_mhpmevent13 =      0x32d;
    constexpr static u32 csr_mhpmevent14 =      0x32e;
    constexpr static u32 csr_mhpmevent15 =      0x32f;
    constexpr static u32 csr_mhpmevent16 =      0x330;
    constexpr static u32 csr_mhpmevent17 =      0x331;
    constexpr static u32 csr_mhpmevent18 =      0x332;
    constexpr static u32 csr_mhpmevent19 =      0x333;
    constexpr static u32 csr_mhpmevent20 =      0x334;
    constexpr static u32 csr_mhpmevent21 =      0x335;
    constexpr static u32 csr_mhpmevent22 =      0x336;
    constexpr static u32 csr_mhpmevent23 =      0x337;
    constexpr static u32 csr_mhpmevent24 =      0x338;
    constexpr static u32 csr_mhpmevent25 =      0x339;
    constexpr static u32 csr_mhpmevent26 =      0x33a;
    constexpr static u32 csr_mhpmevent27 =      0x33b;
    constexpr static u32 csr_mhpmevent28 =      0x33c;
    constexpr static u32 csr_mhpmevent29 =      0x33d;
    constexpr static u32 csr_mhpmevent30 =      0x33e;
    constexpr static u32 csr_mhpmevent31 =      0x33f;

    constexpr static u32 csr_mscratch =         0x340;
    constexpr static u32 csr_mepc =             0x341;
    constexpr static u32 csr_mcause =           0x342;
    constexpr static u32 csr_mbadaddr =         0x343;
    constexpr static u32 csr_mip =              0x344;

    constexpr static u32 csr_mbase =            0x380;
    constexpr static u32 csr_mbound =           0x381;
    constexpr static u32 csr_mibase =           0x382;
    constexpr static u32 csr_mibound =          0x383;
    constexpr static u32 csr_mdbase =           0x384;
    constexpr static u32 csr_mdbound =          0x385;

    constexpr static u32 csr_tselect =          0x7a0;
    constexpr static u32 csr_tdata1 =           0x7a1;
    constexpr static u32 csr_tdata2 =           0x7a2;
    constexpr static u32 csr_tdata3 =           0x7a3;

    constexpr static u32 csr_dcsr =             0x7b0;
    constexpr static u32 csr_dpc =              0x7b1;
    constexpr static u32 csr_dscratch =         0x7b2;

    constexpr static u32 csr_mcycle =           0xb00;
    constexpr static u32 csr_minstret =         0xb01;
    constexpr static u32 csr_mhpmcounter3 =     0xb03;
    constexpr static u32 csr_mhpmcounter4 =     0xb04;
    constexpr static u32 csr_mhpmcounter5 =     0xb05;
    constexpr static u32 csr_mhpmcounter6 =     0xb06;
    constexpr static u32 csr_mhpmcounter7 =     0xb07;
    constexpr static u32 csr_mhpmcounter8 =     0xb08;
    constexpr static u32 csr_mhpmcounter9 =     0xb09;
    constexpr static u32 csr_mhpmcounter10 =    0xb0a;
    constexpr static u32 csr_mhpmcounter11 =    0xb0b;
    constexpr static u32 csr_mhpmcounter12 =    0xb0c;
    constexpr static u32 csr_mhpmcounter13 =    0xb0d;
    constexpr static u32 csr_mhpmcounter14 =    0xb0e;
    constexpr static u32 csr_mhpmcounter15 =    0xb0f;
    constexpr static u32 csr_mhpmcounter16 =    0xb10;
    constexpr static u32 csr_mhpmcounter17 =    0xb11;
    constexpr static u32 csr_mhpmcounter18 =    0xb12;
    constexpr static u32 csr_mhpmcounter19 =    0xb13;
    constexpr static u32 csr_mhpmcounter20 =    0xb14;
    constexpr static u32 csr_mhpmcounter21 =    0xb15;
    constexpr static u32 csr_mhpmcounter22 =    0xb16;
    constexpr static u32 csr_mhpmcounter23 =    0xb17;
    constexpr static u32 csr_mhpmcounter24 =    0xb18;
    constexpr static u32 csr_mhpmcounter25 =    0xb19;
    constexpr static u32 csr_mhpmcounter26 =    0xb1a;
    constexpr static u32 csr_mhpmcounter27 =    0xb10;
    constexpr static u32 csr_mhpmcounter28 =    0xb1c;
    constexpr static u32 csr_mhpmcounter29 =    0xb1d;
    constexpr static u32 csr_mhpmcounter30 =    0xb1e;
    constexpr static u32 csr_mhpmcounter31 =    0xb1f;

    constexpr static u32 csr_mcycleh =          0xb80;
    constexpr static u32 csr_minstreth =        0xb81;
    constexpr static u32 csr_mhpmcounter3h =    0xb83;
    constexpr static u32 csr_mhpmcounter4h =    0xb84;
    constexpr static u32 csr_mhpmcounter5h =    0xb85;
    constexpr static u32 csr_mhpmcounter6h =    0xb86;
    constexpr static u32 csr_mhpmcounter7h =    0xb87;
    constexpr static u32 csr_mhpmcounter8h =    0xb88;
    constexpr static u32 csr_mhpmcounter9h =    0xb89;
    constexpr static u32 csr_mhpmcounter10h =   0xb8a;
    constexpr static u32 csr_mhpmcounter11h =   0xb8b;
    constexpr static u32 csr_mhpmcounter12h =   0xb8c;
    constexpr static u32 csr_mhpmcounter13h =   0xb8d;
    constexpr static u32 csr_mhpmcounter14h =   0xb8e;
    constexpr static u32 csr_mhpmcounter15h =   0xb8f;
    constexpr static u32 csr_mhpmcounter16h =   0xb90;
    constexpr static u32 csr_mhpmcounter17h =   0xb91;
    constexpr static u32 csr_mhpmcounter18h =   0xb92;
    constexpr static u32 csr_mhpmcounter19h =   0xb93;
    constexpr static u32 csr_mhpmcounter20h =   0xb94;
    constexpr static u32 csr_mhpmcounter21h =   0xb95;
    constexpr static u32 csr_mhpmcounter22h =   0xb96;
    constexpr static u32 csr_mhpmcounter23h =   0xb97;
    constexpr static u32 csr_mhpmcounter24h =   0xb98;
    constexpr static u32 csr_mhpmcounter25h =   0xb99;
    constexpr static u32 csr_mhpmcounter26h =   0xb9a;
    constexpr static u32 csr_mhpmcounter27h =   0xb90;
    constexpr static u32 csr_mhpmcounter28h =   0xb9c;
    constexpr static u32 csr_mhpmcounter29h =   0xb9d;
    constexpr static u32 csr_mhpmcounter30h =   0xb9e;
    constexpr static u32 csr_mhpmcounter31h =   0xb9f;

    constexpr static u32 csr_mvendorid =        0xf11;
    constexpr static u32 csr_marchid =          0xf12;
    constexpr static u32 csr_mimpid =           0xf13;
    constexpr static u32 csr_hartid =           0xf14;

    // all of them - tbh there needs to be better "smarts" here
    // string names for all register (debug/trace)
    // identification of unused blocks
    // user/supervisor access bits
    u32 reg[4096];
};

struct riscv_exec
{
    riscv_exec(memory_device * __memory_device);

    virtual void reset();

    virtual void ebreak() {}
    virtual void ecall() {}

    void execute();

    void illegal();

    void (*illegal_cb)(riscv_exec *) = nullptr;

    std::string dump();

    // move reg rd/wr out of here

    // map to "actual" faults (not sure of the css registers)
    constexpr static u32 illegal_op             = 0x00000001;
    constexpr static u32 not_implemented_yet    = 0x00000002;

    u32 pc; // needs to be 64 bit in 64 bit mode
    // rv64 has *64 bit* registers - factor this in
    riscv32_reg reg;

    memory_device * _memory_device;
    // gone!
    u32 faults; // probably in csr
    // call out for e.call and e.break
};
