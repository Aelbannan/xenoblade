#pragma once

#include <types.h>

class CCharEffect;
class CScnItemModel;
struct CCharEffectSlot;

// Real C++ linkage helpers defined in CCharEffect.cpp (retail symbols carry
// these manglings). Callers include this owning header instead of keeping
// per-TU extern "C" copies with invented calling conventions.
void CharEffSetPointEnabled(CCharEffect* self, u32 param);
void CharEffRegisterSlot(CCharEffect* self, u32 idx, CCharEffectSlot* value, u16 a, u16 b, u8 c);
void CharEffSetSlotBit(CCharEffect* self, u32 idx);
void CharEffMoveBit(CCharEffect* self, u32 idx);
void CharEffReleaseSlots0A10(CCharEffect* self);
void CharEffRemoveByPtr(CCharEffect* self, void* ptr);
void CharEffDetachBySubId(CCharEffect* self, CScnItemModel* manager);
