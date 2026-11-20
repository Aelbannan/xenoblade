#pragma once

#include <types.h>
#include "kyoshin/IUIWindow.hpp"
#include "monolib/work/IWorkEvent.hpp"

/*
  Collision-6 invite process. Singleton managed by CCol6System.
  Real MI shape: IUIWindow at +0x00 (IWorkEvent at +0x6C).
  Size 0x78 bytes.
*/
class CCol6Invite : public IUIWindow, public IWorkEvent {
public:
    static CCol6Invite* sInstance;

    CCol6Invite(u16 arg2, u8 arg3, u8 arg4)
        : mArg2(arg2), mArg3(arg3), mArg4(arg4), mField74(0) {}

    static CCol6Invite* Create(CProcess* parent, u16 arg2, u8 arg3, u8 arg4);

    // CProcess overrides
    void Init() override;
    void Term() override;
    void Move() override;

    virtual ~CCol6Invite();

    /* 0x70 */ u16 mArg2;         // from r4
    /* 0x72 */ u8 mArg3;          // from r5
    /* 0x73 */ u8 mArg4;          // from r6
    /* 0x74 */ u8 mField74;       // init 0
};


// The null pointer-to-member-function constant is three words wide; its
// canonical declaration lives in CCol6System.hpp (u32[3]).
struct PtmfNullWords {
    u32 w0;
    u32 w1;
    u32 w2;
};

// Forward declarations for the CCol6Hint/CCol6System subobject destructor
// forwards used by the this-adjusting thunks (Col6HintDtorThunk6C / Col6SystemDtorThunk6C /
// Col6SystemDtorThunk70). These symbols are emitted by the member destructor
// definitions in the CCol6System TU; declaring them as C-linkage lets the thunk
// tail-call the single-arg (non-deleting) destructor with only r3 adjusted.
class CCol6Hint;
class CCol6System;
class CCol6CheckBat;
extern "C" void* __dt__9CCol6HintFv(CCol6Hint*, int flags);
extern "C" void* __dt__13CCol6CheckBatFv(CCol6CheckBat*, int flags);
extern "C" void* __dt__11CCol6SystemFv(CCol6System*, int flags);

// Col6InviteDtorThunk6C backs `this` off to the CCol6Invite embedded subobject and
// tail-calls the non-deleting destructor. Avoids a virtual dispatch so the
// thunk compiles to `subi r3,#-0x6c; b __dt__11CCol6InviteFv`.
class CCol6Invite;
extern "C" void* __dt__11CCol6InviteFv(CCol6Invite*, int flags);

// Standalone string formatting helper (unmangled retail symbol, C linkage).
extern "C" void Col6FormatBufferString(char* buffer, const char* format, ...);
