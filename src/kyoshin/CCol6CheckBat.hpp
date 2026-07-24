#pragma once

#include <types.h>
#include "monolib/work/CProcess.hpp"

/*
  Collision-6 check-bat process. Singleton managed by CCol6System.
  Inherits CProcess for work-thread lifecycle (Init/Term/Move/Draw).
  Size 0x74 bytes.
*/
class CCol6CheckBat : public CProcess {
public:
    CCol6CheckBat();
    virtual ~CCol6CheckBat();

    // CProcess overrides
    void Init() override;
    void Term() override;
    void Move() override;
    void Draw() override;

    /* 0x3C */ u8 mCallbackA[12]; // pointer-to-member-function null (3 words)
    /* 0x48 */ u8 mCallbackB[12]; // pointer-to-member-function null (3 words)
    /* 0x54 */ u32 mField54;      // init 0
    /* 0x58 */ u32 mField58;      // init 0
    /* 0x5C */ u32 mField5C;      // init 0
    /* 0x60 */ s32 mIndex;        // init -1; iteration cursor
    /* 0x64 */ u8 mFlag64;        // byte flag, init 0
    /* 0x65 */ u8 mFlag65;        // byte flag, init 0
    /* 0x66 */ u8 mFlag66;        // byte flag, init 0
    /* 0x67 */ u8 mActive;        // byte flag, init 1
    /* 0x68 */ u32 mField68;      // init 0
    /* 0x6C */ u32 mField6C;      // vtable-like ptr, init lbl_eu_8053021C + 0x24
    /* 0x70 */ u8 mFlag70;        // byte flag, init 0
};

// Singleton instance pointer (lbl_eu_80664230 in retail).
extern CCol6CheckBat* gCol6CheckBat;
