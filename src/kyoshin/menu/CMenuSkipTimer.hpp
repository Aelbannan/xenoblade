#pragma once

#include "kyoshin/IUICf.hpp"
#include "monolib/scn/IScnRender.hpp"
#include <types.h>

// Intermediate base: adds two flag bytes after IUICf/CTTask<IUICf> (0x54 bytes).
class CMenuSkipTimerBase : public IUICf {
public:
    u8 mFlag1; // 0x54
    u8 mFlag2; // 0x55
    // 0x56-0x57: padding
};

// Skip-timer widget shown during cutscene skipping.
// Inherits from IScnRender for the +0x58 subobject layout (vtable + virtual dispatch).
// NOTE: Does NOT re-declare virtual overrides — the base class IScnRender provides
// the virtual slot; actual dispatch goes through the auto-generated thunks
// (@88@... symbols in the object file).
class CMenuSkipTimer : public CMenuSkipTimerBase, public IScnRender {
public:
    CMenuSkipTimer();
    ~CMenuSkipTimer();

    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // --- member fields ---
    u32 mParam;                    // 0x5C
    u8 mBgTexData[0x20];           // 0x60 — CBgTex placeholder
    u8 mHelpData[0x38];            // 0x80 — CTitleAHelp placeholder
    u8 mTimerData[0x98];           // 0xB8 — CSkipTimer placeholder
    u8 mFlag3;                     // 0x150
};
