////////////////////////////////////////////////////////////////////////////////
/// (C) 2022 kindid.co.uk
/// MIT licensed
/// RISCV-32 emulator written for educational purposes
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "types.h"

inline constexpr u32 extract(u32 v, u32 first, u32 bits) {
    return (v >> first) & ((1u << bits) - 1);
}

inline u32 sign_extend(u8 x, u32 y = 7) {
    u32 mask = y << 1;
    return s32((u32(x) ^ mask) - mask);
}

inline u32 sign_extend(u16 x, u8 y = 15u) {
    u32 mask = y << 1;
    return s32((u32(x) ^ mask) - mask);
}

inline u32 sign_extend(u32 x, u32 bit) {
    return s32((u32(x) ^ (1u << bit)) - (1u << bit));
}

