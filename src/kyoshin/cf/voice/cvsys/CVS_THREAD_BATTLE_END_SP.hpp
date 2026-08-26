#pragma once

#include <types.h>
#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/voice/CCharVoice.hpp"

/**
 * CVS_THREAD_BATTLE_END_SP - voice thread for special battle-end sequences.
 *
 * Object layout (allocated 0x34 bytes):
 *   0x20..0x2A  slots[3]      array of active CVoiceHandle pointers
 *   0x2C        count         number of active voice slots
 *   0x30        cmdString     pointer to the current command string
 *
 * Overrides blank1() (vtable index 2) to return the logical allocation
 * size 0x46 (70) -- same getter as the plain BATTLE_END variant.
 */
class CVS_THREAD_BATTLE_END_SP : public CVS_THREAD {
public:
    static const int BUFFER_SIZE = 0x46;

    // Bias added to |command param| to obtain the voice ID passed to
    // func_802A3C44 (retail plays IDs 0xCE4..).
    enum { VOICE_ID_BIAS = 0xCE4 };

    CVoiceHandle* slots[3];  // 0x20: active voice slots
    s32 count;               // 0x2C: number of active slots
    char* cmdString;         // 0x30: current command string

    int blank1() override;   // vtable[2], returns 0x46 (body is the unmangled retail symbol func_802ABB38)
};

// Phantom vtable view over CVoiceHandle so the is-active check is emitted as a
// true r12-chained indirect virtual call (`lwz r12,0(rS); lwz r12,0x2BC(r12); ...`),
// matching retail. Never instantiated (only cast + slot call), so no vtable is
// emitted and no method needs a definition. MWCC places declared slot P at
// vtable offset (P+2)*4 (2 implicit leading slots), so declared slot 173 lands
// at 0x2BC (is-active check).
#define CBESP_PAD4(n)  virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3();
#define CBESP_PAD8(n)  CBESP_PAD4(n##0) CBESP_PAD4(n##1)
#define CBESP_PAD16(n) CBESP_PAD8(n##0) CBESP_PAD8(n##1)
#define CBESP_PAD32(n) CBESP_PAD16(n##0) CBESP_PAD16(n##1)
#define CBESP_PAD64(n) CBESP_PAD32(n##0) CBESP_PAD32(n##1)

struct CVS_THREAD_BATTLE_END_SP_Vtbl {
    CBESP_PAD64(0)   // slots 0-63
    CBESP_PAD64(1)   // slots 64-127
    CBESP_PAD32(2)   // slots 128-159
    CBESP_PAD8(3)    // slots 160-167
    CBESP_PAD4(4)    // slots 168-171
    virtual void v172();              // slot 172
    virtual int isVoiceActive();      // slot 173 -> vtable offset 0x2BC
};

#undef CBESP_PAD4
#undef CBESP_PAD8
#undef CBESP_PAD16
#undef CBESP_PAD32
#undef CBESP_PAD64

// State-triple overlay over the base thread's installed voice-event handler
// ptmf (bytes 0x00-0x0B): each variant's init tables write three words that
// together form the 12-byte member-function pointer.
struct CVS_THREAD_STATE3 {
    u32 word0;  // 0x00: ptmf word 0
    u32 word4;  // 0x04: ptmf word 1
    u32 word8;  // 0x08: ptmf word 2
};

// Raw layout of the object exposing the implicit vtable pointer at 0x1C
// (owned by the CVS_THREAD base) so the factory can override it with this
// variant's vtable (same pattern as CVS_THREAD_DOWN_raw et al.).
struct CVS_THREAD_BATTLE_END_SP_raw {
    u32 word0;              // 0x00: init-state/ptmf word 0
    u32 word4;              // 0x04: init-state/ptmf word 1
    u32 word8;              // 0x08: init-state/ptmf word 2
    u32 field_0x0C;         // 0x0C
    u32 field_0x10;         // 0x10
    u32 field_0x14;         // 0x14
    u32 field_0x18;         // 0x18
    u32* vtable;            // 0x1C: vtable pointer
    CVoiceHandle* slots[3]; // 0x20: active voice slots
    s32 count;              // 0x2C: number of active slots
    char* cmdString;        // 0x30: current command string
};

// Circular voice-node list returned by the voice manager func_800B6BA4().
// Each node's field_8 points at the CCharVoice embedded in the handle
// (i.e. handle + 0x3E9C).
struct CVoiceListNode {
    CVoiceListNode* field_0;  // 0x00: next node
    CVoiceListNode* field_4;  // 0x04: previous node (mirrors CVS_THREAD_EHP.hpp CVoiceHandleListNode)
    CCharVoice* field_8;      // 0x08: &handle->voice
};

struct CVoiceManager {
    u32 field_0;                // 0x00 (unused here)
    CVoiceListNode* field_4;    // 0x04: circular-list sentinel
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim).
extern "C" {
    int  func_802A3E88(CVS_THREAD* self);
    void func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
    int  func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
    int  func_802A77E8(CVoiceHandle* handle);
    int  func_802A8140();
    CVoiceHandle* func_802A330C(int size, int align);
    u8*  func_802A34E4(int size);
    // Zero-arg declaration: the factory keeps the allocator result live in r3
    // so the ctor call needs no argument reload (matches retail).
    void __ct__cf_CVS_THREAD();
    int  labs(int n);

    // Runtime rethrow (NMWException.h): declared noreturn so MWCC elides the
    // __end__catch epilogue of a catch-all handler that ends with `bl __throw`.
    __declspec(noreturn) void __throw(char* throwtype, u32 location,
                                      u32 dtor);
}

// C++-linkage voice-manager query (retail mangled name func_800B6BA4__Fv).
CVoiceManager* func_800B6BA4();

// Data tables / glove for this thread variant.
// Unknown-size array declarations prevent MWCC sda21 folding, forcing the
// lis/addi (ADDR16_HA/LO) accesses retail uses for these symbols; the scalar
// lbl_eu_80668DB0 stays small-data (retail references it via @sda21).
extern u32 lbl_eu_8053A194[];   // final init-state triple
extern u32 lbl_eu_8053A1A0[3];  // reset init-state triple (func_802AB900)
extern u32 lbl_eu_8053A1AC[3];  // init-state triple (func_802ABA70)
extern u32 lbl_eu_8053A100[];   // command-list table for the constructor
extern char lbl_eu_8053A1B8[];   // CVS_THREAD_BATTLE_END_SP vtable
extern u8 lbl_eu_80668DB0;       // sentinel command string