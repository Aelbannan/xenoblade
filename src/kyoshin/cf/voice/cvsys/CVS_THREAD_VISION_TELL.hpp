#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/voice/CCharVoice.hpp"

// CVoiceHandle is defined once in the shared base header CVS_THREAD.hpp.

/**
 * CVS_THREAD_VISION_TELL - voice thread for vision-tell audio events.
 *
 * Overrides blank1() to return 0x32 (50), indicating a small buffer
 * allocation for this thread type. Object size: 0x28 (40 bytes).
 * Fields at 0x20/0x24 store CVoiceHandle pointers for two voice slots.
 */
class CVS_THREAD_VISION_TELL : public CVS_THREAD {
public:
    // Size of the thread-local voice buffer in bytes.
    static const int BUFFER_SIZE = 0x32;

    CVoiceHandle* field_0x20;  // 0x20: voice handle pointer (slot 1)
    CVoiceHandle* field_0x24;  // 0x24: voice handle pointer (slot 2)

    int blank1() override;
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

struct CVS_THREAD_VISION_TELL_Vtbl {
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

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" u32 lbl_eu_80539DB0[3];  // Init data tables for slot states (3 u32s each)
extern "C" u32 lbl_eu_80539DBC[3];
extern "C" u32 lbl_eu_80539DC8[3];

// Random voice ID tables (short arrays in .sdata).
extern "C" short lbl_eu_80662D58[4];
extern "C" short lbl_eu_80662D60[4];

// Vtable for CVS_THREAD_VISION_TELL (0x1C = 28 bytes = 7 entries).
extern "C" u32 lbl_eu_80539DD4[7];

// C-linkage imports used by this TU's thread helpers / factory. Genuine
// retail C/ABI symbols imported from sibling voice modules.
extern "C" {
    int           func_802A3E88(CVS_THREAD* self);
    void          func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
    int           func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
    int           func_802A77E8(CVoiceHandle* handle);
    int           func_802A7850(int iter);
    int           func_802A7B90(CVoiceHandle* handle1, CVoiceHandle* handle2);
    int           func_802A7EB0(CVoiceHandle* handle1, CVoiceHandle* handle2);
    CVoiceHandle* func_802A330C(int size, int align);
    u8*           func_802A34E4(int size);
    void          __ct__cf_CVS_THREAD(void* self);
}
