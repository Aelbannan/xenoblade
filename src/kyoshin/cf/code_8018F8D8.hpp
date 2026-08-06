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