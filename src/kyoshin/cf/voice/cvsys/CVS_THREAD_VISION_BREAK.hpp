#pragma once

#include <types.h>
#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/voice/CCharVoice.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "monolib/util/reslist.hpp"

/**
 * CVS_THREAD_VISION_BREAK - voice thread for vision-break status effect.
 *
 * Overrides blank1() to return 0x0F (15), indicating a small buffer
 * allocation for this thread type. Object size: 0x28 (40 bytes).
 * Fields at 0x20/0x24 store CVoiceHandle pointer and a phase/state counter.
 */
class CVS_THREAD_VISION_BREAK : public CVS_THREAD {
public:
    // Size of the thread-local buffer in bytes.
    static const int BUFFER_SIZE = 0x0F;

    CVoiceHandle* field_0x20;   // 0x20: voice handle pointer
    s32 field_0x24;             // 0x24: phase counter / state selector

    int func_802A95FC() { return BUFFER_SIZE; }
};


// Phantom vtable view over CVoiceHandle so virtual dispatch is emitted as a
// true r12-chained indirect call (`lwz r12,0(rS); lwz r12,0x2BC(r12); ...`),
// matching retail. Never instantiated (only cast + slot call), so no vtable
// is emitted and no method needs a definition. MWCC places declared slot P at
// vtable offset (P+2)*4 (2 implicit leading slots), so declared slot 173
// lands at 0x2BC (is-active check).
#define CVT_PAD4(n)  virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3();
#define CVT_PAD8(n)  CVT_PAD4(n##0) CVT_PAD4(n##1)
#define CVT_PAD16(n) CVT_PAD8(n##0) CVT_PAD8(n##1)
#define CVT_PAD32(n) CVT_PAD16(n##0) CVT_PAD16(n##1)
#define CVT_PAD64(n) CVT_PAD32(n##0) CVT_PAD32(n##1)

struct CVoiceHandleVTable {
    CVT_PAD64(0)   // slots 0-63
    CVT_PAD64(1)   // slots 64-127
    CVT_PAD32(2)   // slots 128-159
    CVT_PAD8(3)    // slots 160-167
    CVT_PAD4(4)    // slots 168-171
    virtual void v172();              // slot 172
    virtual int isVoiceActive();      // slot 173 -> vtable offset 0x2BC
};

#undef CVT_PAD4
#undef CVT_PAD8
#undef CVT_PAD16
#undef CVT_PAD32
#undef CVT_PAD64

// Raw layout of the object exposing the implicit vtable pointer at 0x1C
// (owned by the CVS_THREAD base) so the factory can override it.
struct CVS_THREAD_VISION_BREAK_raw {
    u32* state0;                // 0x00
    u32 state1;                 // 0x04
    u32 state2;                 // 0x08
    u32 unkC;                   // 0x0C
    u32 unk10;                  // 0x10
    u32 unk14;                  // 0x14
    u32 unk18;                  // 0x18
    void* vtable;               // 0x1C
    CVoiceHandle* field_0x20;   // 0x20
    s32 field_0x24;             // 0x24
};

// Runtime rethrow (NMWException.h): declared noreturn so MWCC elides the
// __end__catch epilogue of a catch-all handler that ends with `bl __throw`.
extern "C" __declspec(noreturn) void __throw(char* throwtype, void* location, void* dtor);

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)

// Global object-manager list accessor (C++ linkage; retail mangled name
// func_800B6BA4__Fv). Returns a reslist of CfObject pointers.
reslist<cf::CfObject*>* func_800B6BA4();

// Init-data triples (3 u32s each: {field_0, field_4, callback}). Plain
// globals: MWCC does not mangle global data names.
extern u32 lbl_eu_80539D70[3]; // Initial state triple
extern u32 lbl_eu_80539D7C[3]; // Reset state triple (for func_802A93FC)
extern u32 lbl_eu_80539D88[];  // CVS_THREAD_VISION_BREAK vtable

// Voice-ID tables indexed by mtRand(3)*2 (halfword entries).
extern s16 lbl_eu_80662D40[4]; // Voice IDs for field_0x24 >= 3 phase
extern s16 lbl_eu_80662D48[4]; // Voice IDs for field_0x24 < 3 phase

// C-linkage imports from sibling voice TUs (retail symbol names - keep
// linkage/signatures verbatim). Genuine retail C/ABI symbols.
extern "C" {
    int           func_802A3E88(CVS_THREAD* self);
    void          func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
    int           func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
    int           func_802A3D54(CCharVoice* voicePtr, int voiceId, int arg);
    CVoiceHandle* func_802A330C(int size, int align);
    void*         func_802A34E4(int size);
    CVoiceHandle* func_802A7A54(int arg);
    void*         func_8016FE34(void* r3);
    void          func_800BE924(void* voice);

    // Base CVS_THREAD constructor (C-linkage).
    void __ct__cf_CVS_THREAD(void* self);

    // Exception runtime rethrow is declared (noreturn) above the class.
}