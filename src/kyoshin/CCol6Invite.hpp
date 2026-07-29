#pragma once

#include <types.h>
#include "monolib/work/CProcess.hpp"

/*
  Collision-6 invite process. Singleton managed by CCol6System.
  Inherits CProcess for work-thread lifecycle (Init/Term/Move/Draw).
  Size 0x78 bytes.
*/
class CCol6Invite : public CProcess {
public:
    static CCol6Invite* Create(CProcess* parent, u16 arg2, u8 arg3, u8 arg4);
    virtual ~CCol6Invite();

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
    /* 0x60 */ s32 mIndex;        // init -1
    /* 0x64 */ u8 mFlag64;        // byte flag, init 0
    /* 0x65 */ u8 mFlag65;        // byte flag, init 0
    /* 0x66 */ u8 mFlag66;        // byte flag, init 0
    /* 0x67 */ u8 mActive;        // byte flag, init 1
    /* 0x68 */ u32 mField68;      // init 0
    /* 0x6C */ u32 mField6C;      // vtable-like ptr, init lbl_eu_8052FF3C + 0x24
    /* 0x70 */ u16 mArg2;         // from r4
    /* 0x72 */ u8 mArg3;          // from r5
    /* 0x73 */ u8 mArg4;          // from r6
    /* 0x74 */ u8 mField74;       // init 0
};

// Singleton instance pointer (lbl_eu_8066423C in retail).
extern CCol6Invite* gCol6Invite;

// Standalone string formatting helper.
void func_eu_801651A0(char* buffer, const char* format, ...);
