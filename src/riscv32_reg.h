////////////////////////////////////////////////////////////////////////////////
/// (C) 2022 kindid.co.uk
/// MIT licensed
/// RISCV-32 emulator written for educational purposes
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "types.h"

struct riscv32_reg {
    constexpr static unsigned int_reg_count = 32;
    constexpr static unsigned fpu_reg_count = 32;

    void wri(u32 idx, u32 v);
    u32 rdi(u32 idx);

//    void wrf(f32 idx, f32 v);
//    f32 rdf(f32 idx);

    u32 r[int_reg_count];
    u32 f[fpu_reg_count];

    // todo; move dump function here
};
