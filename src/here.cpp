////////////////////////////////////////////////////////////////////////////////
/// (C) 2012 kindid.co.uk
/// MIT licensed
////////////////////////////////////////////////////////////////////////////////

#include "here.h"

static constexpr char hex_chars[16]{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

std::string hex_pad(u64 v, u32 width)
{
    std::string chars(width, '0');
    for (unsigned p = 0; p < width; p++) {
        chars[width - p - 1] = hex_chars[v & 0xf];
        v >>= 4;
    }
    return chars;
}

std::string hex_pad(u32 v, u32 width)
{
    return hex_pad(u64(v), width);
}

std::string hex_pad(u16 v, u32 width)
{
    return hex_pad(u64(v), width);
}

std::string hex_pad(u8 v, u32 width)
{
    return hex_pad(u64(v), width);
}

std::string hex_pad(s8 v, u32 width)
{
    return hex_pad(u8(v), width);
}

std::string hex_pad(s16 v, u32 width)
{
    return hex_pad(u16(v), width);
}

std::string hex_pad(s32 v, u32 width)
{
    if (v < 0) {
        return "-" + hex_pad(u32(-v), width);
    } else {
        return hex_pad(u32(v), width);
    }
}

std::string hex_pad(s64 v, u32 width)
{
    return hex_pad(u64(v), width);
}
