#pragma once

#include <types.h>

class CScn;
class CScnItemAnim;
struct CScn80496B0C;
struct CScnModel80496B0C;
namespace ml { struct CVec3; }

// Canonical declarations for the free helpers defined by
// libs/monolib/src/scn/CScn_80496B0C.cpp. MWCC gives these C++ linkage names;
// callers include this header instead of keeping flat extern "C" copies.
void scn80496ReleaseChildren(CScn80496B0C* self);
void scn80496SetupChildren(CScn80496B0C* self, CScn* scn,
                           CScnModel80496B0C* model, u16 slot);
void scn80496SetNodeIdByName(CScn80496B0C* self, const char* name);
u32 scn80496IsNodeFlagBit(CScn80496B0C* self);
u32 scn80496BindChrAnmPack(CScn80496B0C* self, CScnItemAnim* anim, int index,
                           u32 c, u32 d, u32 e, u32 f);
void scn80496SetFloat178(u8* self, float val);
f32 scn80496GetChild1V6(CScn80496B0C* self);
f32 scn80496GetChild2V6(CScn80496B0C* self);
void scn80496RefreshBlendTag(CScn80496B0C* self, u32 tag);
u32 scn80496ChildIdsDiffer(CScn80496B0C* self);
void scn80496RefreshTRS(CScn80496B0C* self, ml::CVec3* outA, ml::CVec3* outB,
                        ml::CVec3* outC, u32 nodeIdx);
