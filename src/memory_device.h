////////////////////////////////////////////////////////////////////////////////
/// (C) 2022 kindid.co.uk
/// MIT licensed
/// RISCV-32 emulator written for educational purposes
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "types.h"

// todo;u64 (template)
struct memory_device {
    memory_device(u8 * memory, u64 length);
    //
    u8 read_u8(u32 address);
    u16 read_u16(u32 address);
    u32 read_u32(u32 address);
    u64 read_u64(u32 address);
    void write(u32 address, u8 data);
    void write(u32 address, u16 data);
    void write(u32 address, u32 data);
    void write(u32 address, u64 data);
    //
    u8 * memory;
    u64 length;
};
