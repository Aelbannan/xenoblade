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
extern "C" void startFade(CScnFadeMan*);
extern "C" void* isFadeIdle(CScnFadeMan*);
extern "C" void* isFadeSettled(CScnFadeMan*);
extern "C" void* getFadeColorPtr(CScnFadeMan*);
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

// Typed view of the CScn vtable slots written by __ct__CScn. The retail vtables
// are the .data blob at 0x8056E8D0 (lbl_eu_*), which MWCC's own __vt__4CScn is
// not placed at yet; until that vtable ownership is recovered the ctor/dtor
// store the retail addresses through these padding views.
struct CScnCtorView {
    u8 pad00[0x10];          //0x00
    void* unk10;             //0x10 primary vtable ptr
    u8 pad14[0x3C - 0x14];   //0x14
    u32 mMoveFunc[3];        //0x3C
    u32 mDrawFunc[3];        //0x48
    void* unk54;             //0x54 secondary (IWorkEvent) vtable ptr
};

// Typed view of the CScn vtable slots repointed by CScn::~CScn() (see the
// CScnCtorView note above).
struct CScnDtorView {
    u8 pad00[0x10];          //0x00
    void* unk10;             //0x10 accessor table ptr
    u8 pad14[0x54 - 0x14];   //0x14
    void* unk54;             //0x54 secondary (IWorkEvent) vtable ptr
};

// Direct-call destructor hit by CScn::Term() on the +0x7c subobject
// (retail flat symbol __dt__804C0E48).
extern "C" void __dt__804C0E48(void* self, int flag);
