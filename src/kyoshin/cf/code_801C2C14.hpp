#pragma once

#include <types.h>

struct FxSoundPair;
struct FxSoundSlot;

// C++-linkage helpers owned by kyoshin/cf/code_801C2C14.
FxSoundPair* FxPair_Init(FxSoundPair* self);
int clearFxSlotEffect(FxSoundSlot* slots, int index, int frame);
bool func_801C358C(FxSoundSlot* slots, int idx, int fxType, u32 memSize, u32 size);
