////////////////////////////////////////////////////////////////////////////////
/// (C) 2012 kindid.co.uk
/// MIT licensed
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <iostream>

#include "types.h"

std::string hex_pad(u8 v, u32 width = 2);
std::string hex_pad(u16 v, u32 width = 4);
std::string hex_pad(u32 v, u32 width = 8);
std::string hex_pad(u64 v, u32 width = 16);

std::string hex_pad(s8 v, u32 width = 2);
std::string hex_pad(s16 v, u32 width = 4);
std::string hex_pad(s32 v, u32 width = 8);
std::string hex_pad(s64 v, u32 width = 16);

struct DebugOutOn : public std::ostream
{
    DebugOutOn()
        : std::ostream(std::cout.rdbuf())   // macOS bug fix
    {
        old_flags = std::cout.flags();
    }
    virtual ~DebugOutOn() override
    {
        std::cout << std::endl;
        std::cout.flags(old_flags);
    }

    template <typename T> const DebugOutOn &operator<<(const T &v) const
    {
        std::cout << v;
        return *this;
    }

    template <u8> const DebugOutOn &operator<<(const u8 &v) const
    {
        std::cout << (u32)(v);
        return *this;
    }

    std::ios_base::fmtflags old_flags;
};

struct DebugOutOff //: public std::ostream
{
    template <typename T>
    const DebugOutOff &operator<<(const T &v) const { return *this; }
};

#ifdef DEBUG_ON
#define here DebugOutOn() << ("file://" __FILE__  ":") << __LINE__ << ": "
#define h_wrap(X) " " << #X << "=" << X
#else
#define here DebugOutOff() << __FILE__ << ":" << __LINE__ << ": "
#define h_wrap(X) " " << #X << "=" << X
#endif
