////////////////////////////////////////////////////////////////////////////////
/// (C) 2022 kindid.co.uk
/// MIT licensed
/// RISCV-32 emulator written for educational purposes
////////////////////////////////////////////////////////////////////////////////

#define DEBUG_ON
#include "here.h"
#include "riscv32_reg.h"

void riscv32_reg::wri(u32 idx, u32 v) {
    // this u8 conversion shouldn't be necessary
    here << "reg:wr:" << hex_pad(u8(idx)) << " = " << hex_pad(v) << " was " << hex_pad(r[idx]);
    if (idx != 0) {
        r[idx] = v;
    }
}

u32 riscv32_reg::rdi(u32 idx) {
    here << "reg:rd:" << hex_pad(u8(idx)) << " = " << hex_pad(r[idx]);
    return r[idx];
}
