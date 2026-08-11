#pragma once

#include <types.h>

// Environment-light-control object layout used by CTaskEnvironment::Init
// (reached through the scene manager slot at +0x7C; retail monolib
// CScnEnvLgtCtrl). Only the flag word Init touches is declared.
struct CTaskEnvLgtCtrlAccess {
    u32 field_0;   // +0x00 flag word (Init ORs 0x80)
};

// Scene object held by CTaskEnvironment at +0x54 (retail CScn manager layout;
// the env-light-control slot sits at +0x7C).
struct CTaskEnvScene {
    u8 field_0x00[0x7C];
    CTaskEnvLgtCtrlAccess* mEnvLgtCtrl;   // +0x7C
};

// Global environment manager (retail lbl_eu_80663D58). Recovered layout:
// +0x68 record index, +0x6C record array (60-byte records).
struct CTaskEnvGlobal {
    u8 field_0x00[0x68];
    u32 field_0x68;   // +0x68 record index
    u8* field_0x6C;   // +0x6C record array (60-byte records)
    u8 field_0x70[0x7C - 0x70];
    u32 field_0x7C;   // +0x7C
};

// C-linkage import (retail symbol): env-light-control enable (monolib scn).
extern "C" void func_804C1094(CTaskEnvLgtCtrlAccess* ctrl, int flag);

// Retail global manager pointer (unmangled at global scope - no extern "C").
extern CTaskEnvGlobal* lbl_eu_80663D58;

// CRTP task base (canonical monolib template). CTaskEnvironment derives from
// CTTask<CTaskEnvironment>; the out-of-line Move/Draw/dtor specializations are
// emitted in the unit cpp, and the derived Move/Draw overrides live here.
#include "monolib/work/CTTask.hpp"

class CTaskEnvironment : public CTTask<CTaskEnvironment> {
public:
    CTaskEnvironment();
    virtual ~CTaskEnvironment();
    void Init();
    void Term();
    void Move();
    void Draw();

    /* 0x54 */ CTaskEnvScene* mScene;
};
