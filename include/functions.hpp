#pragma once

#include <types.h>

//Vec4 constructor? Defined before CTaskGame::Term
struct CTaskGame_setVec4_tmp {
    f32 unk00[4];
};
extern "C" CTaskGame_setVec4_tmp* CTaskGame_setVec4(CTaskGame_setVec4_tmp*, f32, f32, f32, f32);

void func_8004302C(int, int);
extern "C" void CtrlRemote_SetSharedBit(u32, u32);
// Unmangled retail symbol (not CtrlObjectParam_GetSlotTableBase__Fv).
extern "C" int* CtrlObjectParam_GetSlotTableBase();
void func_8009E574(int*, int, int, int);
