#pragma once

// TU-private imports for kyoshin/cf/chain/CChainTimer.cpp (moved out of the public
// CChainTimer.hpp, which keeps the classes).

#include "kyoshin/cf/chain/CChainTimer.hpp"
#include "kyoshin/cf/chain/CChainTimeImports.hpp"
// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------
extern "C" {
int lookupWorkAtAddr(void* addr);
int isBattleStatusActive(void*, int);
void* MapFx_ResolveOwner(void* source);
unsigned long SuddenCommuIsStateActive(void* self);
void CfModel_GetSpeedRate(void* obj, f32* vec);
void* getArtsParamAtCnt(void* self, unsigned int index);
void* getArtsParamRC(const void* arts, int index, int subindex);
int checkArtsFull(void* param, void* actor, int flags);
void resetBattleGauge__Q22cf13CfGameManagerFv();
void syncBattleState__Q22cf13CfGameManagerFv(void* object);
void processFieldEffects__Q22cf13CfGameManagerFv();
void CMenuArtsSelect_ResetSlotAnims();
extern "C" void ChainMusic_SaveSlots(void* self, void* owner);
extern "C" void ChainMusic_RestoreSlots(void* self, void* owner);
int isArtsUsable(int val, int flag);
// (IdTable_SumValues: single winning decl lives on chain/CChainActorList.hpp.)
int* CtrlObjectParam_GetSlotTableBase();
}
// C++-mangled retail helper findObjectById__Fi (actor id -> action source).
extern void* findObjectById(int id);

// Data imports (plain globals - MWCC does not mangle global data names).
extern f32 lbl_eu_80668A90;
extern f32 lbl_eu_80668A94;
extern f32 lbl_eu_80668A98;
extern f32 lbl_eu_80668AA8;
extern f64 lbl_eu_80668AA0;
extern f64 lbl_eu_80668AB0;
extern f64 lbl_eu_80668AB8;
extern f32 lbl_eu_80668AC0;
extern f32 lbl_eu_80668AC4;
extern f32 lbl_eu_80668AC8;
extern f32 lbl_eu_80656C40[3];
extern f32 lbl_eu_80656C64[3];
