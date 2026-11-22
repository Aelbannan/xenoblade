#pragma once

#include <types.h>
#include <revolution/MTX.h>
#include "monolib/effect/EffectNode.hpp"

// Partial layouts for cross-TU data/helpers declared below (retail C-linkage
// pseudo-imports relocated from code_804CC2B8.cpp). Keep signatures/linkage
// identical - the names are the retail symbol names.
struct MemManGlob {
    u32 field_0x00;
    const void* field_0x04;
};

struct U16Pair {
    u16 a;
    u16 b;
};

struct EffectStruct;
struct EffectInit;

// Real C++ entry points defined in code_804CC2B8.cpp.
void EffFxInitStructLinks(EffectStruct* self);
void EffFxRenderDispatch(EffectInit* self, void* arg);
void EffFxEmptyStub();

// C-linkage imports
extern "C" f32 lbl_eu_8066B0D8;
extern "C" f32 lbl_eu_8066B158;
extern "C" f32 lbl_eu_8066B15C;
extern "C" const u8 sEffectPosPalette0[];
extern "C" const u8 sEffectPosPalette1[];
extern "C" void* Scn_FindCamItem(void* src, int index);
extern "C" u32 lbl_eu_8065FC18[];
extern "C" s32 sCachedZMode;
extern "C" s32 sCachedTevMode;
extern "C" s32 sCachedMatColor;
extern "C" s32 sCachedTexGenCount;
extern "C" u32 lbl_eu_8066B124;
extern "C" void Scn_HasWorkItem(void* p);
extern "C" void __dt__804D80F0(void* p, int flag);
extern "C" EffectNode* SchedNode_FetchByIndex(s32 index);
extern "C" void SchedNode_ReleaseHandle(s16 index);
extern "C" s32 SchedNode_ClaimActivate(s32 index);
extern "C" u32 SchedNode_CountTotal(void);
extern "C" void cfCam_nopCtorSlot(Vec* p);
extern "C" void noopAnimVec3(Vec* p);
extern "C" void lytBindRefreshPlayer(void* obj);
extern "C" void lytBindRefreshMatrix(void* obj, void* node);
extern "C" void lytBindRefreshTrans(void* obj, void* node);
extern "C" void lytBindSetupForPane(void* obj, u32 a, u32 b, u32 c, u32 d);
extern "C" U16Pair* SchedRes_EntryByIndex(const void* p);
extern "C" void CERotZMatrix(Mtx* out);
extern "C" void CERotEulerOrder(Mtx* out, const Vec* p, s32 field);
extern "C" void CERotEulerXYZ(Mtx* out, const Vec* angles);
extern "C" void Scn_CopyCamProjMatrix(const void* src, Mtx out, s32 flag);

// Active scene root cached by the effect system (defined in this unit's .sbss).
struct CScnRootEnv;
extern "C" CScnRootEnv* sActiveSceneRoot;
