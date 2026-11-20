// Decompiled code for kyoshin/CCol6Invite

#include "kyoshin/CCol6Invite.hpp"
#include "kyoshin/CCol6System.hpp"
#include "monolib/util/MemManager.hpp"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// Retail data symbols referenced by this factory.
CCol6Invite* CCol6Invite::sInstance;

// Constructor/factory for the CCol6Invite singleton (retail unmangled symbol
// __ct__CCol6Invite). Allocates 0x78 bytes from work memory, constructs the
// real IUIWindow/IWorkEvent bases, stores itself as the singleton and registers
// as a child of `self`. The singleton store + Regist run even when the
// allocation failed (the object register stays NULL).
extern "C" CCol6Invite* __ct__CCol6Invite(CCol6Invite* self, u16 arg2, u8 arg3, u8 arg4) {
    // Return NULL if the singleton already exists.
    if (CCol6Invite::sInstance != 0) {
        return 0;
    }
    u32 workMem = CWorkThreadSystem::getWorkMem();
    CCol6Invite* obj = (CCol6Invite*)mtl::MemManager::allocate(0x78, workMem);
    // Placement-new emits the single null guard around the real base
    // construction; the resulting pointer is reused for the singleton store.
    obj = new (obj) CCol6Invite(arg2, arg3, arg4);

    CCol6Invite::sInstance = obj;
    ((CProcess*)obj)->Regist(self, false);
    return CCol6Invite::sInstance;
}

// Destructor defined in CCol6System.cpp as the extern "C" free function
// __dt__11CCol6InviteFv (retail form with nested null-guards + delete).

// --- this-adjusting thunks over CCol6Hint / CCol6System (defined in the
// CCol6System TU). Retail
// it off to the CProcess/CCol6Hint/CCol6System base and forward.

// Col6HintDtorThunk6C: CCol6Hint dtor at subobject +0x6c.
void Col6HintDtorThunk6C(u8* self) {
    typedef void* (*DtorFn)(CCol6Hint*);
    ((DtorFn)__dt__9CCol6HintFv)(reinterpret_cast<CCol6Hint*>(self - 0x6c));  // DEBT(callcast): Fv-mangled dtor is emitted as 2-arg D1 (this, flags); 1-arg view preserves r4 passthrough
}

// Col6HintRenderThunk70: CCol6Hint::cbRenderBefore at subobject +0x70.
void Col6HintRenderThunk70(u8* self) {
    reinterpret_cast<CCol6Hint*>(self - 0x70)->cbRenderBefore();
}

// Col6SystemDtorThunk6C: CCol6System dtor at subobject +0x6c.
void Col6SystemDtorThunk6C(u8* self) {
    typedef void* (*DtorFn)(CCol6System*);
    ((DtorFn)__dt__11CCol6SystemFv)(reinterpret_cast<CCol6System*>(self - 0x6c));  // DEBT(callcast): Fv-mangled dtor is emitted as 2-arg D1 (this, flags); 1-arg view preserves r4 passthrough
}

// Col6SystemRenderThunk70: CCol6System::cbRenderBefore at subobject +0x70.
void Col6SystemRenderThunk70(u8* self) {
    reinterpret_cast<CCol6System*>(self - 0x70)->cbRenderBefore();
}

// Col6SystemDtorThunk70: CCol6System dtor at subobject +0x70.
void Col6SystemDtorThunk70(u8* self) {
    typedef void* (*DtorFn)(CCol6System*);
    ((DtorFn)__dt__11CCol6SystemFv)(reinterpret_cast<CCol6System*>(self - 0x70));  // DEBT(callcast): Fv-mangled dtor is emitted as 2-arg D1 (this, flags); 1-arg view preserves r4 passthrough
}

// Col6CheckBatDtorThunk6C (retail func_801640D0): CCol6CheckBat dtor at
// subobject +0x6c. Backs `this` off and tail-calls the non-deleting destructor.
void Col6CheckBatDtorThunk6C(u8* self) {
    // Retail thunk: subi r3,r3,0x6c; b __dt__13CCol6CheckBatFv -- r4 (flags)
    // passes through untouched, so call through a 1-arg pointer view.
    typedef void* (*DtorFn)(CCol6CheckBat*);
    ((DtorFn)__dt__13CCol6CheckBatFv)(reinterpret_cast<CCol6CheckBat*>(self - 0x6c));  // DEBT(callcast): Fv-mangled dtor is emitted as 2-arg D1 (this, flags); 1-arg view preserves r4 passthrough
}

// Col6HintDtorThunk70 (retail func_801640F0): CCol6Hint dtor at subobject
// +0x70. Backs `this` off and tail-calls the non-deleting destructor.
void Col6HintDtorThunk70(u8* self) {
    // Retail thunk: subi r3,r3,0x70; b __dt__9CCol6HintFv -- r4 (flags)
    // passes through untouched, so call through a 1-arg pointer view.
    typedef void* (*DtorFn)(CCol6Hint*);
    ((DtorFn)__dt__9CCol6HintFv)(reinterpret_cast<CCol6Hint*>(self - 0x70));  // DEBT(callcast): Fv-mangled dtor is emitted as 2-arg D1 (this, flags); 1-arg view preserves r4 passthrough
}

// Col6InviteDtorThunk6C: CCol6Invite dtor at subobject +0x6c. Backs `this` off to the
// CCol6Invite embedded subobject and tail-calls the non-deleting destructor.
void Col6InviteDtorThunk6C(u8* self) {
    // Retail thunk: subi r3,r3,0x6c; b __dt__11CCol6InviteFv -- r4 (flags)
    // passes through untouched, so call through a 1-arg pointer view.
    typedef void* (*DtorFn)(CCol6Invite*);
    ((DtorFn)__dt__11CCol6InviteFv)(reinterpret_cast<CCol6Invite*>(self - 0x6c));  // DEBT(callcast): Fv-mangled dtor is emitted as 2-arg D1 (this, flags); 1-arg view preserves r4 passthrough
}

// Standalone string formatting helper.
// Formats a string into buffer, stores length at buffer+0x100.
// Retail symbol is unmangled (Col6FormatBufferString), so this has C linkage.
extern "C" void Col6FormatBufferString(char* buffer, const char* format, ...) {
    char buf[0x100];
    va_list va;
    va_start(va, format);
    vsnprintf(buf, 0x100, format, va);
    va_end(va);
    u32* length = (u32*)(buffer + 0x100);
    *length = strlen(buf);
    strcpy(buffer, buf);
}