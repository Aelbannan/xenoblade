#pragma once

#include <types.h>
#include "libs/monolib/src/scn/CScnFrame.hpp"

// Local helpers shared by CScn.cpp matching targets.
// NOTE: this header is included by explicit path ("libs/monolib/src/scn/CScn.hpp"),
// separate from the public monolib/scn/CScn.hpp.

// Format string for the scene name built by CScnNw4r::create.
extern char lbl_eu_80524090[];

// Float constants block referenced by scene timing helpers (.sdata2).
extern f32 lbl_eu_8066AAB4;
extern f32 lbl_eu_8066AAB8;
extern f32 lbl_eu_8066AABC;

// CScn vtable labels (defined in the CScn_80496B0C TU data surgery). The
// constructor/destructor repoint the +0x10 / +0x54 slots at them.
extern u8 lbl_eu_8056E8D0[];
extern u8 lbl_eu_8056E984[];

// .sdata2 constant initialized into CScn+0x98 by the constructor.
extern f32 lbl_eu_8066AAB0;

// The canonical CScn class (and ScnRenderCB / UnkScn68 / UnkScn8C) lives in
// the public monolib/scn/CScn.hpp; this header only adds CScn-TU helpers.
#include "monolib/scn/CScn.hpp"

// View of the CScn+0x88 subobject's vtable slot 0 (the destructor entry),
// invoked as a plain virtual so the flag register matches retail (r4=1).
struct ScnSubDtor {
    virtual void dtorSlot(int flag);
};

// Generic subsystem handle released by CScn::Term(): first virtual slot
// (vt+8) called with flag=1.
struct ScnTermRelease {
    virtual void release(int flag);
};

// IScnRender draw callback shape: vt+0xC(cb, scn).
struct IScnRenderDraw {
    virtual ~IScnRenderDraw() {}
    virtual void onRender(CScn* scn) = 0;
};

class CView;

// CamMan_SyncCameraToView / CamMan_GetViewThread are CScnCameraMan members
// (libs/monolib/src/scn/CScnCameraMan.hpp); CScn.cpp casts mCamWork to call them.
extern "C" void* simFwdAnimSetup102(void*, void*, void*, void*, int);
extern "C" int simFwdAnimReset(int, int, int);
extern "C" void __ct__8049E710();
extern "C" void __ct__804820F8();
extern "C" void startFade(int);
extern "C" void* isFadeIdle(void*);
extern "C" void* isFadeSettled(void*);
// The former flat imports CamMan_SwapViewParam_AED4, CamMan_FindItemA_B158,
// CamMan_FindViewItemA and CamMan_FindViewItemB are real CScnCameraMan members
// (libs/monolib/src/scn/CScnCameraMan.hpp); the scene-API wrappers in CScn.cpp
// cast mCamWork and call the members directly.

// Camera-item payload block copied by Scn_CopyCamProjMatrix: the 64-byte projection
// matrix (MTX44) at item+0x194.
struct ScnCamParams {
    u32 w[16]; // 0x00..0x3C
};

// View of the camera item returned by CamMan_FindItemA_B158, exposing the +0x194
// parameter block (projection matrix).
struct ScnCamItemView {
    u8 pad00[0x194]; //0x00
    u32 proj[16]; //0x194 projection matrix (MTX44)
};

// Blob reached via CScn+0x84; holds per-scene floats at +0x08 / +0x0C.
struct ScnFloats {
    u8 unk00[0x08];
    float unk08; //0x08
    float unk0C; //0x0C
};

// Minimal member-function surfaces for the scene subsystem managers whose
// full headers redefine CScn (so they cannot be included from this TU).
class CScnFilterMan {
public:
    void update();
};

class CScnFadeMan {
public:
    void update();
};

// Scene item pool (fields live in CScnItemPool.cpp); CScn only drives its
// per-frame update hooks.
class CScnItemPool {
public:
    void update();
    void updateItemD0();
};

class CScnEnvLgtCtrl;
extern "C" u32 scnLgtStepLightItems(CScnEnvLgtCtrl* self);

// Typed view of the CScn member fields written by __ct__CScn. Mirrors the
// public-header layout without modifying it.
struct CScnCtorView {
    u8 pad00[0x10];          //0x00
    void* unk10;             //0x10 primary vtable ptr
    u8 pad14[0x3C - 0x14];   //0x14
    u32 mMoveFunc[3];        //0x3C
    u32 mDrawFunc[3];        //0x48
    void* unk54;             //0x54 secondary (IWorkEvent) vtable ptr
    u32 unk58[15];           //0x58..0x90 zeroed block
    u8 pad94[4];             //0x94 (left uninitialized)
    f32 unk98;               //0x98
    u16 unk9C;               //0x9C
    u16 unk9E[10];           //0x9E..0xB0
    u16 unkB2;               //0xB2
    u32 unkB4;               //0xB4
    u8 unkB8;                //0xB8
};

// Typed view of the CScn member fields touched by CScn::~CScn(). Mirrors the
// public-header layout without modifying it.
struct CScnDtorView {
    u8 pad00[0x10];          //0x00
    void* unk10;             //0x10 accessor table ptr
    u8 pad14[0x54 - 0x14];   //0x14
    void* unk54;             //0x54 secondary (IWorkEvent) vtable ptr
    u8 pad58[0x88 - 0x58];   //0x58
    UnkScn8C* unk88;         //0x88 scene-root subobject
};

// Direct-call destructor hit by CScn::Term() on the +0x7c subobject
// (retail flat symbol __dt__804C0E48).
extern "C" void __dt__804C0E48(void* self, int flag);

// Typed view of the CScn member fields touched by CScn::Term(). Mirrors the
// public-header layout without modifying it.
struct CScnTermView {
    u8 pad00[0x5C];           //0x00
    void* unk5C;              //0x5C
    void* unk60;              //0x60
    void* unk64;              //0x64
    void* unk68;              //0x68
    void* unk6C;              //0x6C
    void* unk70;              //0x70
    void* unk74;              //0x74
    void* unk78;              //0x78
    void* unk7C;              //0x7C
    void* unk80;              //0x80
    void* unk84;              //0x84
    void* unk88;              //0x88
    UnkScn8C* unk8C;          //0x8C
};

// Typed view of the CScn member fields touched by CScn::Move(). Mirrors the
// public-header layout without modifying it.
struct CScnMoveView {
    u8 pad00[0x60];           //0x00
    CScnItemPool* itemPool;   //0x60
    void* unk64;              //0x64
    UnkScn68* camWork;        //0x68
    void* unk6C;              //0x6C
    CScnFilterMan* filterMan; //0x70
    void* unk74;              //0x74
    void* unk78;              //0x78
    CScnEnvLgtCtrl* lgtCtrl;  //0x7C
    CScnFadeMan* fadeMan;     //0x80
    CScnFrame* frame;         //0x84
    void* unk88;              //0x88
    UnkScn8C* unk8C;          //0x8C
    u8 pad90[0x3E4 - 0x90];   //0x90
    u8 unk3E4;                //0x3E4
    u8 unk3E5;                //0x3E5
    u8 unk3E6;                //0x3E6
    u8 unk3E7;                //0x3E7
    u8 unk3E8;                //0x3E8
};
