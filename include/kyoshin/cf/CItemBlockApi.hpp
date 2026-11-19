#pragma once

#include <types.h>

// Item-block accessors defined in CItem.cpp.
extern "C" void* CItemBlock_getKindSlot(s32 kind, u32 index, u32 slot);
extern "C" char* CItemBlock_getSlotU16(u32 index, u32 slot);
