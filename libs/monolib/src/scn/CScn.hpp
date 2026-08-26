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

// CScn vtable label (emitted in this TU; referenced by the destructor).
extern u8 lbl_eu_8056E8D0[];

class UnkScn8C;

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
