#pragma once

#include <types.h>

namespace cf {
struct CtrlObjectParamU16RowTable;
struct CtrlObjectParamSwap;
struct CtrlObjectParamArtsList;
struct CtrlObjectParamArtsView;
}

// Canonical C++ declarations for the free helpers defined in CtrlObjectParam.cpp.
void CtrlObjectParam_WriteU16RowEntry(cf::CtrlObjectParamU16RowTable* table, u16 index, u16 value);
int CtrlObjectParam_SwapSlotValues(cf::CtrlObjectParamSwap* self, unsigned int a, unsigned int b);
void CtrlObjectParam_SetItemBitFlag(unsigned int idx, int flag);
void CtrlObjectParam_SetStateBitFlag(unsigned int idx, int flag);
u32 CtrlObjectParam_GetArtsListMaxSize(cf::CtrlObjectParamArtsList* list);
extern "C" void* CtrlObjectParam_GetCharEntry(u32 idx);
extern "C" void* CtrlObjectParam_GetArtsStatsRow(void* rows, u32 index);
extern "C" int* CtrlObjectParam_GetSlotTableBase();
extern "C" void CtrlObjectParam_SyncParamFromActor(void* self);
