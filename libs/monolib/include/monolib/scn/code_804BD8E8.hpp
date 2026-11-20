#pragma once

#include <types.h>

// Canonical
// libs/monolib/src/scn/code_804BD8E8.cpp. The mangled retail names are the
// exported symbols; callers include this header instead of declaring copies.

// Returns non-zero while the collision-resource state is active.
u32 ScnRes_IsStateActive_E4A0();

// Non-zero when a resource entry carries the high flag bit.
int ScnRes_HasHighFlagEntry_E5AC();

// Registers a scene collision callback; arguments are raw word values.
void ScnRes_Notify4Word_E3E0(u32 cb, u32 mtx, u32 mask, u32 flags);

// Cross-navigation flag accessors (raw word return).
int ScnRes_GetCrossFlagA_E5B8();
int ScnRes_GetCrossFlagB_E5C0();

// Scale-table and entry-value queries.
s32 ScnRes_FetchScaleRow_E53C(f32* dst, s32 index);
int ScnRes_IsEntryValued_E604(int index);

// Static initializer emitted by this TU.
void sinit_804BF540();

// Starts a resource-registration pass over an offset vector.
s32 ScnRes_RegPassSelect_E2E8(const f32* offset, u32 flags, int mode, int select);
