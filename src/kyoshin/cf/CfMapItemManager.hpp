#pragma once

#include <types.h>

// Owner module header for kyoshin/cf/CfMapItemManager.cpp (retail battle
// flag/status helpers). These are the single unified declarations; TUs that
// call them include this header.

// Battle-status word probe: tests the actor's status flags against `id`
// (raw bitmask window outside [0x800,0x807], per-category dispatch inside).
// `outFlags` points at the actor's u32 flag word.
extern "C" s32 testActorStatus(void* obj, void* outFlags, u32 id);

// Battle-status flag setter (clear/mask write on the actor's status word).
extern "C" void dispatchItemLoad(void* actor, u32 flags);

// Map-item record enable (hikari-item singleton flag write; retail unmangled).
extern "C" void setItemEnable(void* obj, void* arg);
// Pick-up handling entry (retail unmangled; takes the manager singleton).
extern "C" void tryPickupMapItem(void* self);
