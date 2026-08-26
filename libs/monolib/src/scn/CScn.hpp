#pragma once

#include <types.h>
#include "libs/monolib/src/scn/CScnFrame.hpp"

// Local helpers shared by CScn.cpp matching targets.
// NOTE: this header is included by explicit path ("libs/monolib/src/scn/CScn.hpp"),
// separate from the public monolib/scn/CScn.hpp.

// Float constants block referenced by scene timing helpers (.sdata2).
extern f32 lbl_eu_8066AAB4;
extern f32 lbl_eu_8066AAB8;
extern f32 lbl_eu_8066AABC;

// CScn vtable label (defined in the CScn_80496B0C TU data surgery; the
// destructor repoints the +0x10 / +0x54 slots at it).
extern u8 lbl_eu_8056E8D0[];

class IScnRender;
class UnkScn8C;

// --- Layout mirrors of the public monolib/scn/CScn.hpp declarations. ---
// This TU carries its own complete CScn class definition (identical layout)
// so that a user-declared ~CScn() suppresses MWCC's implicitly generated
// destructor, whose body cannot match retail.

struct ScnRenderCB {
    IScnRender* cb; //0x0
    u32 prio; //0x4
    u8 flag; //0x8
}; // size = 0xC

// Camera-work blob at CScn+0x68 (CScnCameraMan-related); Draw reads +0x34 via lwz+extsh.
struct UnkScn68 {
    u8 unk00[0x34];
    s32 unk34; //0x34
};

// Object at CScn+0x8C; Draw calls vf+0x18 / vf+0x1C (RTTI vtable).
struct UnkScn8C {
    virtual ~UnkScn8C() {}
    virtual void vf0C() = 0;
    virtual void vf10() = 0;
    virtual void vf14() = 0;
    virtual void vf18() = 0;
    virtual void vf1C() = 0;
};

// View of the CScn+0x88 subobject's vtable slot 0 (the destructor entry),
// invoked as a plain virtual so the flag register matches retail (r4=1).
struct ScnSubDtor {
    virtual void dtorSlot(int flag);
};

// IScnRender draw callback shape: vt+0xC(cb, scn).
struct IScnRenderDraw {
    virtual ~IScnRenderDraw() {}
    virtual void onRender(CScn* scn) = 0;
};

class CScn : public CTTask<CScn>, public IWorkEvent{
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
    virtual void Init();
    virtual void Draw();
    virtual ~CScn();

    //0x000: vtable 1 (CTTask)
    //0x000-054: CTTask
    //0x054: vtable 2 (IWorkEvent)
    u8 unk58[0x68 - 0x058]; //0x058
    UnkScn68* mCamWork; //0x068
    u8 unk6C[0x8C - 0x06C]; //0x06C
    UnkScn8C* mUnk8C; //0x08C
    u8 unk90[0x0B4 - 0x090]; //0x090
    ICulling* unkB4; //0x0B4
    u32 unkB8; //0x0B8
    ScnRenderCB mRenderCBs[64]; //0x0BC
    u32 mRenderCBCount; //0x3BC
    char unk3C0[0x3E4 - 0x3C0]; //0x3C0
    u8 unk_3E4; //0x3E4
    u8 unk_3E5; //0x3E5
    u8 unk_3E6; //0x3E6
    u8 unk_3E7; //0x3E7
    u8 unk_3E8; //0x3E8
    u8 unk_3EA[0x3EC - 0x3EA]; //0x3EA
}; // size = 0x3EC

class CView;

extern "C" s32 func_8049AF80(UnkScn68* cam);
extern "C" CView* func_8049B0A0(UnkScn68* cam, s16 idx);
extern "C" void* func_80486068(void*, void*, void*, void*, int);
extern "C" int func_80486070(int, int, int);
extern "C" void __ct__8049E710();
extern "C" void __ct__804820F8();
extern "C" void func_8049C72C(int);
extern "C" void* func_8049C794(void*);
extern "C" void* func_8049C7B0(void*);
extern "C" int func_8049AED4(int);
extern "C" int func_8049B158(int);
extern "C" bool func_8049B240(int);
extern "C" void* func_8049B2C4(void*);

// Camera-item payload block copied by func_80496120: the 64-byte projection
// matrix (MTX44) at item+0x194.
struct ScnCamParams {
    u32 w[16]; // 0x00..0x3C
};

// View of the camera item returned by func_8049B158, exposing the +0x194
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
    void func_8048CEDC();
};

class CScnEnvLgtCtrl;
extern "C" u32 func_804C12A4(CScnEnvLgtCtrl* self);

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
