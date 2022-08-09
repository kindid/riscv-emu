////////////////////////////////////////////////////////////////////////////////
/// (C) 2022 kindid.co.uk
/// MIT licensed
/// RISCV-32 emulator written for educational purposes
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "types.h"

// template this
struct riscv32_reg {
    constexpr static unsigned int_reg_count = 32;
    constexpr static unsigned fpu_reg_count = 32;

    void wri(u32 idx, u32 v);
    u32 rdi(u32 idx);

//    void wrf(f32 idx, f32 v);
//    f32 rdf(f32 idx);

    // use bits - the thing up above needs to define XLEN
    u32 r[int_reg_count]; // rename 'x'
    u32 f[fpu_reg_count]; // i guess these are 64 bits or even wider

    // todo; move dump function here
};
