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

    /* 0x3C */ u32 mCallbackA[3]; // pointer-to-member-function null (3 words)
    /* 0x48 */ u32 mCallbackB[3]; // pointer-to-member-function null (3 words)
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
extern "C" CCol6Invite* lbl_eu_8066423C;

// Forward declarations for the CCol6Hint/CCol6System subobject destructor
// forwards used by the this-adjusting thunks (func_801640E0 / func_80164100 /
// func_80164110). These symbols are emitted by the member destructor
// definitions in the CCol6System TU; declaring them as C-linkage lets the thunk
// tail-call the single-arg (non-deleting) destructor with only r3 adjusted.
class CCol6Hint;
class CCol6System;
extern "C" void* __dt__9CCol6HintFv(CCol6Hint*, int flags);
extern "C" void* __dt__11CCol6SystemFv(CCol6System*, int flags);

// func_80164118 backs `this` off to the CCol6Invite embedded subobject and
// tail-calls the non-deleting destructor. Avoids a virtual dispatch so the
// thunk compiles to `subi r3,#-0x6c; b __dt__11CCol6InviteFv`.
class CCol6Invite;
extern "C" void* __dt__11CCol6InviteFv(CCol6Invite*, int flags);

// Standalone string formatting helper.
void func_eu_801651A0(char* buffer, const char* format, ...);
