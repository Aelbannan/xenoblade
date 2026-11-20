#pragma once

#include <types.h>

struct CtrlWork;

// C++-linkage helpers owned by kyoshin/code_801A929C.
void BattleWork_MarkSlotFFFF(void* self);
void BattleWork_InitCtrlWork(CtrlWork* work);
bool BattleWork_CheckEffectTableReady(u32 id);
