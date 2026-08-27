#pragma once

#include <types.h>

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/voice/CCharVoice.hpp"

/**
 * CVS_THREAD_DOWN - voice thread variant for "Down" (knocked-down) audio.
 *
 * Inherits from CVS_THREAD and returns a thread-local buffer.
 * Total object size: 0x28 (40 bytes).
 * Fields at 0x20/0x24 store CVoiceHandle pointers to the two voice slots.
 */
class CVS_THREAD_DOWN : public CVS_THREAD {
public:
    CVoiceHandle* field_0x20;  // 0x20: voice handle pointer (slot 1)
    CVoiceHandle* field_0x24;  // 0x24: voice handle pointer (slot 2)
};

// vtable-access helper: exposes the CVoiceHandle vtable method at slot 175
// (offset 0x2BC, the is-active check) as a real virtual so MWCC emits the
// retail r12 dispatch (`lwz r12,0(rS); lwz r12,0x2bc(r12)`).  MWCC places
// declared slot P at vtable offset (P+2)*4 (2 implicit leading slots), so
// declared slot 173 lands at 0x2BC.
#define CVT_PAD4(n)  virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3();
#define CVT_PAD8(n)  CVT_PAD4(n##0) CVT_PAD4(n##1)
#define CVT_PAD16(n) CVT_PAD8(n##0) CVT_PAD8(n##1)
#define CVT_PAD32(n) CVT_PAD16(n##0) CVT_PAD16(n##1)
#define CVT_PAD64(n) CVT_PAD32(n##0) CVT_PAD32(n##1)

struct CVS_THREAD_DOWN_Vtbl {
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

// Named view over the CVS_THREAD init-state triple (bytes 0x00-0x08), which
// the factory copies wholesale from lbl_eu_80539A68. Same layout as the first
// three CVS_THREAD words; only used to avoid anonymous-field access here.
struct CVS_THREAD_DOWN_INIT {
    u32 word0;  // 0x00
    u32 word1;  // 0x04
    u32 word2;  // 0x08
};

// Raw layout of the CVS_THREAD_DOWN object exposing the implicit vtable
// pointer at 0x1C (owned by the CVS_THREAD base) so the factory can
// override it with the DOWN vtable, plus the two voice slots.
struct CVS_THREAD_DOWN_raw {
    u32* state0;                // 0x00: init-state word 1
    u32 state1;                 // 0x04: init-state word 2
    u32 state2;                 // 0x08: init-state word 3
    u32 unkC;                   // 0x0C
    u32 unk10;                  // 0x10
    u32 unk14;                  // 0x14
    u32 unk18;                  // 0x18
    void* vtable;               // 0x1C
    CVoiceHandle* field_0x20;   // 0x20
    CVoiceHandle* field_0x24;   // 0x24
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" {
    int func_802A3E88(CVS_THREAD* self);
    void func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
    int func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
    CVoiceHandle* func_802A330C(int size, int align);
    u8* func_802A34E4(int size);
}

extern "C" void __ct__cf_CVS_THREAD(void* self);

// Runtime rethrow (NMWException.h): declared noreturn so MWCC elides the
// __end__catch epilogue of a catch-all handler that ends with `bl __throw`
// (retail catch-all handlers end at the rethrow).
extern "C" __declspec(noreturn) void __throw(char* throwtype, void* location, void* dtor);

// Init-state triples (copied into the base state words 0x00-0x08) and this
// subclass's vtable. Global-scope data symbols: MWCC does not mangle them.
extern u32 lbl_eu_80539A68[3];
extern u32 lbl_eu_80539A74[3];
extern u32 lbl_eu_80539A80[3];
extern u32 lbl_eu_80539A8C[7];