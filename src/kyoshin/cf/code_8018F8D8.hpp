#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/device/CDeviceVI.hpp>
#include <monolib/scn/IScnRender.hpp>
#include <monolib/scn/CScn.hpp>
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CPartyStateWin.hpp"

class CMenuPTState : public CProcess, public IScnRender {
public:
    CMenuPTState();
    virtual ~CMenuPTState();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    void cbRenderBefore();

private:
    // CProcess (0x0-0x3C) + IScnRender (0x3C-0x40) bases, then retail base gap.
    u8 field_base_pad[0x1C];      // 0x40-0x5C
    CScn* field_0x5C;              // 0x5C
    CBgTex field_0x60;             // 0x60 (0x20 bytes)
    CPartyStateWin field_0x80;     // 0x80
};

// Container used by the catalog helper functions func_8019017C / func_80190034.
// The hosting object keeps a cf::CfGameManager* reference at offset 0x408.
struct CFuncHost {
    u8 _0[0x408];
    cf::CfGameManager* manager;  // 0x408
};

// Object used by func_eu_80191E88: a raw pointer at offset 0x408 that is
// treated as a byte buffer offset by +0x28 when passed to func_8009EB2C.
struct CFuncHost408 {
    u8 _0[0x408];
    u8* field_0x408;  // 0x408
};

// 4-float value type used by func_8008566C.
struct UnkFloat4 {
    float field_0x0;
    float field_0x4;
    float field_0x8;
    float field_0xC;
};

// Result struct returned by func_8009D5FC in func_eu_80191E88.
struct UnkR31_8019E88 {
    u16 field_0x0;
    u16 field_0x2;
};

// Typed vtable-proxy for issuing virtual calls at exact retail vtable slot
// byte offsets (PPC32: one pointer per 4 bytes). Only the slots actually
// called by this TU's functions get typed members; the rest are opaque
// padding. Used by func_8018FCA8 against the player (CfObjectMove) object.
struct CPlayerVtbl {
    void* padA8[0xa8 / 4];            // 0x000 - 0x0A4
    void (*fw_a8)(void* self, float* pos); // 0x0A8
    void* padAC[(0xc8 - 0xac) / 4];   // 0x0AC - 0x0C4
    void (*fw_c8)(void* self);        // 0x0C8
    void (*fw_cc)(void* self);        // 0x0CC
    void* padD0[(0xd4 - 0xd0) / 4];   // 0x0D0
    void (*fw_d4)(void* self, float* pos, float extra); // 0x0D4
    int (*fw_74)(void* self);         // 0xDC
};

// Accessor for the player flag word at offset 0x68 (func_8018FCA8).
struct CPlayerFlags {
    u8 pad[0x68];
    u32 flags; // 0x68
};

// Vtable proxy for the actor objects used by func_80190940 (slots 0x2BC,
// 0x308) and the sub-object held at +0x4 (slot 0x30).
struct CActorSubVtbl {
    void* pad[0x30 / 4];
    void* (*fw_30)(void* self); // 0x30, returns a pointer read at [0]
};

struct CActorVtbl {
    void* padA[0x2bc / 4];
    void (*fw_2bc)(void* self);       // 0x2BC
    void* padB[(0x308 - 0x2c0) / 4];  // 0x2C0 - 0x304
    void* (*fw_308)(void* self);      // 0x308
};