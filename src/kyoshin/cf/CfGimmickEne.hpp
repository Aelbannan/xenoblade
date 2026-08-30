#pragma once

/**
 * @file CfGimmickEne.hpp
 * @brief cf::CfGimmickEne -- enemy gimmick entity class.
 *
 * CfGimmickEne extends CfGimmick for enemy-specific gimmick behaviour.
 * Fields are populated from BDAT columns at construction.
 *
 * FULL_MATCH symbols in this TU:
 *   func_8026E5BC -- vtable override CfGimmickEne[+0x20] (empty no-op)
 *   func_8026F31C -- dispatch table entry [5]           (empty no-op)
 */

#include "types.h"
#include "kyoshin/cf/CfGimmick.hpp"
#include <nw4r/g3d/g3d_scnproc.h>
#include <revolution/gx/GXTypes.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

namespace cf {

class CArtsSet;
class CArtsParam;
class CActorParam;
class CfGimmickEne;
struct CfGimmickEneSceneState;

// Minimal view of the scene object (CScn-family); +0x8C holds the active
// scene-state object.
struct CfGimmickEneScene {
    u8 pad00[0x8C];
    CfGimmickEneSceneState* field_8C;   // 0x8C
};

// Active-scene state object (CScn + 0x8C); byte 0x19 is the active flag.
struct CfGimmickEneSceneState {
    u8 pad00[0x19];
    u8 field_19;                    // 0x19 - scene active flag
};

// nw4r::g3d::ScnProc callback object; +0xF0 holds the owning CfGimmickEne.
struct CfGimmickEneScnProc {
    u8 pad00[0xF0];
    CfGimmickEne* field_F0;         // 0xF0 - owner gimmick
};

class CfGimmickEne : public CfGimmick {
public:
    ~CfGimmickEne();

    /* 0x82 */ u16 typeId;          // = 7 for enemy gimmicks
    /* 0x84 */ u16 field_84;        // character id, matched against actor->field_3F28
    /* 0x86 */ u16 field_86;        // arts id, matched against the actor's arts slots
    /* 0x88 */ u8 field_88[4];
    /* 0x8C */ u8 field_8C[2];
    /* 0x8E */ u8 field_8E[2];
    /* 0x90 */ u8 field_90[2];
    /* 0x92 */ u16 field_92;        // sound id
    /* 0x94 */ u16 field_94;        // outro sound id
    /* 0x96 */ u8 field_96;         // spawn mode
    /* 0x97 */ u8 colorR;
    /* 0x98 */ u8 colorG;
    /* 0x99 */ u8 colorB;
    /* 0x9A */ u8 field_9A;         // behaviour flags (bit 2: no actor bind)
    /* 0x9B */ u8 field_9B;
    /* 0x9C */ u8 field_9C;
    /* 0x9D */ u8 pad_9D;
    /* 0x9E */ u16 field_9E;
    /* 0xA0 */ u16 field_A0;
    /* 0xA2 */ u16 field_A2;
    /* 0xA4 */ u16 field_A4;        // active duration
    /* 0xA6 */ u16 field_A6;
    /* 0xA8 */ u8 field_A8;         // anim/state index
    /* 0xA9 */ u8 field_A9;         // alpha
    /* 0xAA */ u16 dispatchIdx;     // index into the lbl_eu_80537A20 dispatch table
    /* 0xAC */ f32 field_AC;        // running timer
    /* 0xB0 */ f32 field_B0;
    /* 0xB4 */ CActorParam* field_B4;  // bound actor (CActorParam family, retail offset 0x3F28 holds char id)
    /* 0xB8 */ CArtsParam* field_B8;         // matched arts param
    /* 0xBC */ nw4r::g3d::ScnProc* field_BC;  // ScnProc marker node
};

} // namespace cf

// Extern "C" exports -- linker names must match retail symbols.
extern "C" void func_8026E5BC();
extern "C" void func_8026F31C();

// CfGimmickEne vtable (stored at +0x00 by the constructor/destructor).
extern u8 lbl_eu_80537A78[];

// sdata2 float constants used by the arts-interaction logic (func_8026EBA8).
extern f32 lbl_eu_80668968;
extern f32 lbl_eu_8066896C;
extern f32 lbl_eu_8066897C;
extern f64 lbl_eu_80668980;
// Alpha scale / interpolation constants (func_8026ECEC).
extern f32 lbl_eu_80668988;
extern f32 lbl_eu_8066898C;

// sdata2 constants used by the marker-quad renderer (func_8026E7F8).
// The quad corner coordinates (-1.0/1.0) are written as literals so MWCC
// reloads them per FIFO write exactly like retail (extern refs get CSE'd or
// reordered); they land in this TU's sdata2 at 0x80668974/0x80668978.
extern GXColor lbl_eu_80668970;   // fixed material color (white)

// Dispatch table of member-function pointers indexed by CfGimmickEne::dispatchIdx.
typedef void (cf::CfGimmickEne::*CfGimmickEneDispatch)();
extern CfGimmickEneDispatch lbl_eu_80537A20[];

// C-linkage imports and same-TU call targets (retail symbol names are
// unmangled -- keep linkage/signatures verbatim so `bl` relocs match).
// The variadic same-TU stubs are never inlined by MWCC, so the retail `bl`
// to them survives while they are not yet decompiled.
extern "C" {

// CfGimmickEne ctor is an unmangled linker name -- declare C linkage here so
// the definition in the .cpp emits __ct__cf_CfGimmickEne (see siblings
// CfGimmickSaveOff / CfGimmickJump).
extern void* __ct__cf_CfGimmickEne(cf::CfGimmickEne* self, u16 row);

// Base-class ctor (CfGimmick.cpp, unmangled linker name).
extern void __ct__cf_CfGimmick(void* self);

// Base gimmick bdat/position helper (CfGimmick.cpp).
extern void func_80208F34(cf::CfGimmick* self, float* out, void* bdat,
                          void** holder);

// Bdat holder seed for this gimmick (sda21 pointer global).
extern void* lbl_eu_80664144;

// Fixed column-name string block used by the ctor (offsets 0x00..0x3A).
extern char lbl_eu_8050E6A4[];

// Runtime-patched column-name table (4 char*): the ctor writes 'a'+row into
// each name and '1'+i into name[0][4] to build the per-cell column names.
extern char* lbl_eu_80537A68[4];

// Color-column name buffer: the ctor writes '1'+i into [2] before each read.
extern char* lbl_eu_806629B0;

// CTaskLOD delegates (monolib lod) and sound helpers used by func_8026ECEC.
extern void setLODEnable__8CTaskLODFv(u8 lod, int mode);
extern void refreshLOD__8CTaskLODFv(u8 lod, f32 f);
extern void removeLODEntry__8CTaskLODFv(u8 lod, f32 f);
extern void clearLODEntry__8CTaskLODFv(u8 lod);
extern void attachLODObject__8CTaskLODFv(u8 lod, int mode);
extern void detachLODObject__8CTaskLODFv(u8 lod, int mode);
extern void addLODEntry__8CTaskLODFv(u8 lod, int mode);
extern void* getScnHandle__Fv();
extern void func_804BCC30(void* snd, u8 id);
extern void func_804BCC3C(void* snd, u8 id);

// Gimmick-state setter (CfGimmick.cpp) and sound playback (CfSoundMan, the
// retail symbol returns the started sound id).
extern void func_8020A068(int arg0, int flag, u32 value);
// Single shared flat-name form (unmangled retail symbol; see
// CfObjectImplMove.hpp).
extern "C" u16 playActorSound__Q22cf10CfSoundManFUlUlUlUlf(
    u32 a, u32 b, u32 c, u32 d, f32 vol);

// Scene / frame helpers (monolib scn).
extern cf::CfGimmickEneScene* func_8049698C();            // current-scene global
extern cf::CfGimmickEneSceneState* func_8048ECD0(cf::CfGimmickEneScene* scene);  // -> +0x8C

// Base gimmick deactivation helper (CfGimmick.cpp).
extern void func_80208EE4(cf::CfGimmick* self);

// Scene root-group accessor (monolib CScnMem): retail symbol is unmangled
// (C linkage), takes the shared scene pointer and a child index.
extern "C" nw4r::g3d::ScnObj* func_8048EC14(CScn* scene, unsigned int index);

// Gimmick-state setter on the CfGimmickGlobal (sound/event dispatcher).
extern void func_801F4B68(int value, int id);

// CArtsSet helpers (the arts set pointer comes from CActorParam::CActorParam_UnkVirtualFunc122()).
extern cf::CArtsParam* getArtsParamByIdx(cf::CArtsSet* artsSet, int index);
extern int func_80153CAC(const void* artsSet, int index);

// Same-TU call targets (scaffold stubs).
extern void func_8026E6E4(cf::CfGimmickEne* self);
extern void func_8026E7F8(cf::CfGimmickEne* self, int arg);

} // extern "C"
