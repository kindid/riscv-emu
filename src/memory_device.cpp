////////////////////////////////////////////////////////////////////////////////
/// (C) 2012 kindid.co.uk
/// MIT licensed
////////////////////////////////////////////////////////////////////////////////

#define DEBUG_ON
#include "here.h"

#include "memory_device.h"

memory_device::memory_device(u8 * memory, u64 length)
    : memory(memory)
    , length(length)
{
    here << "memory::memory";
}

u8 memory_device::read_u8(u32 address) {
    here << "mem:rd08:" << hex_pad(u64(address)) << " -> u8("  << hex_pad(*((u8*)(memory + address))) << ")";
    return *((u8*)(memory + address));
}

u16 memory_device::read_u16(u32 address) {
    here << "mem:rd10:"<< hex_pad(u64(address)) << " -> u16("  << hex_pad(*((u16*)(memory + address))) << ")";
    return *((u16*)(memory + address));
}

u32 memory_device::read_u32(u32 address) {
    here << "mem:rd20:"<< hex_pad(u64(address)) << " -> u32("  << hex_pad(*((u32*)(memory + address))) << ")";
    return *((u32*)(memory + address));
}

u64 memory_device::read_u64(u32 address) {
    here << "mem:rd40:"<< hex_pad(u64(address)) << " -> u64("  << hex_pad(*((u64*)(memory + address))) << ")";
    return *((u64*)(memory + address));
}

void memory_device::write(u32 address, u8 data) {
    here << "mem:wr08:" << hex_pad(u8(address)) << " -> u8("  << hex_pad(data) << ")";
    memory[address] = data;
}

void memory_device::write(u32 address, u16 data) {
    here << "mem:wr10:" << hex_pad(u16(address)) << " -> u16("  << hex_pad(data) << ")";
    *((u16 *) (memory + address)) = data;
}

void memory_device::write(u32 address, u32 data) {
    here << "mem:wr20:" << hex_pad(u32(address)) << " -> u32("  << hex_pad(data) << ")";
    *((u32 *)(memory + address)) = data;
}

void memory_device::write(u32 address, u64 data) {
    here << "mem:wr40:" << hex_pad(u64(address)) << " -> u64("  << hex_pad(data) << ")";
    *((u64 *)(memory + address)) = data;
}

