#pragma once

#include <types.h>
#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

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

struct CVS_THREAD_TENSION_UP_Vtbl {
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

// Named view over the CVS_THREAD init-state triple (bytes 0x00-0x08).
struct CVS_THREAD_TENSION_UP_INIT {
    u32 word0;  // 0x00
    u32 word1;  // 0x04
    u32 word2;  // 0x08
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" u32 lbl_eu_80539D20[3];
extern "C" u32 lbl_eu_80539D2C[3];
extern "C" u32 lbl_eu_80539D38[3];
extern "C" u32 lbl_eu_80539D44[3];  // vtable for CVS_THREAD_TENSION_UP

/**
 * CVS_THREAD_TENSION_UP - voice thread for tension-up audio events.
 *
 * Inherits from CVS_THREAD and returns a smaller thread-local buffer
 * (130 bytes) compared to other variants like BUF (260) or FAINT (240).
 */
class CVS_THREAD_TENSION_UP : public CVS_THREAD {
public:
    static const int BUFFER_SIZE = 0x82;

    CVoiceHandle* field_0x20;  // 0x20: back-pointer to owning manager
    s32 field_0x24;            // 0x24: thread index
    u8 field_0x28;             // 0x28: flag (0 = normal, 1 = reversed)

    int blank1() override;
};
