#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

// CVS_THREAD_BUF: Voice thread that manages a dedicated buffer allocation.
// Allocates 0x104 (260) bytes for its voice processing buffer.
//
// Layout (object size 0x40 = 64 bytes):
//   0x00-0x18  base CVS_THREAD fields
//   0x1C       vtable pointer
//   0x20-0x2B  3 voice-handle slots (slotHandles[3])
//   0x2C-0x37  3 voice-ID slots    (slotIds[3])
//   0x38       field_0x38: number of scheduled voices (limit / count)
//   0x3C       field_0x3c: current slot index
class CVS_THREAD_BUF : public CVS_THREAD {
public:
    static const int BUFFER_SIZE = 0x104;

    CVoiceHandle* slotHandles[3];  // 0x20-0x2B: per-slot voice handles
    int           slotIds[3];      // 0x2C-0x37: per-slot voice IDs
    int           field_0x38;      // 0x38: scheduled voice count (limit)
    int           field_0x3c;      // 0x3C: current slot index

    int blank1() override;
};

// Scheduler queue descriptor filled by func_802A5174 during construction.
// 0x0/0x4 are the destination arrays (handles and IDs), 0x8 the running count.
struct CVSPlayQueue {
    CVoiceHandle** handleArr;  // 0x0
    int*           idArr;      // 0x4
    int            count;      // 0x8
};

// C-linkage imports (genuine retail C-ABI symbols from sibling voice modules).
extern "C" {
    int    func_802A3E88(CVS_THREAD* self);
    void   func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
    int    func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
    int    func_802A3D54(CCharVoice* voicePtr, int voiceId, int arg);
    CVoiceHandle* func_802A330C(int size, int align);
    u8*    func_802A34E4(int size);
    void   __ct__cf_CVS_THREAD();
    int    func_802A77E8(CVoiceHandle* handle);
    CVoiceHandle* func_802A7A54(int a);
    void*  func_800B708C__Fi(int id);
    void*  func_8016FE34(void* src);
}

// Globals: post-init state triples and this subclass's vtable.
extern u32 lbl_eu_805399E0[3];
extern u32 lbl_eu_805399EC[3];
extern u32 lbl_eu_805399F8[3];
extern u32 lbl_eu_80539A04[7];