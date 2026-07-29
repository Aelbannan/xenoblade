#pragma once

#include <types.h>
#include "kyoshin/cf/voice/CCharVoice.hpp"
#include "kyoshin/code_8027513C.hpp"

class CScn;

// Polymorphic sub-object at CVoiceHandle+0x04 (used by CVS_THREAD_EHP).
struct CVSubObj {
    void** vtable;
};

class CVS_THREAD{
public:
    u32* unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u32 unk18;

    CVS_THREAD();

    //Virtual table (0x1c)
    virtual void func_802A3B50();
    virtual void func_802A3BEC();
    virtual int blank1();
    virtual void func_802A1EA0();
    virtual void func_802A3740();
    virtual int blank2();
    int func_802A5ECC() { return 240; }
};

// Voice-handle type.  CCharVoice voice is embedded at offset 0x3E9C.
// Fields at 0x3F00 and 0x3F60 are accessed by func_802A4798 / func_802A3EF0.
struct CVoiceHandle {
    void** vtable;                              // 0x00
    CVSubObj* field_0x04;                       // 0x04: sub-object pointer
    u8 _pad[0x3E9C - 0x08];                     // 0x08-0x3E9B
    CCharVoice voice;                           // 0x3E9C (0x40 bytes)
    u8 _pad2[0x3F00 - (0x3E9C + 0x40)];         // 0x3EDC-0x3EFF
    u32 field_0x3F00;                            // 0x3F00: flags
    u8 _pad3a[0x3F08 - 0x3F04];                 // 0x3F04-0x3F07
    u32 field_0x3f08;                            // 0x3F08: manager flag
    u8 _pad3b[0x3F60 - 0x3F0C];                 // 0x3F0C-0x3F5F
    void* field_0x3F60;                          // 0x3F60: party/voice data
};

// Inner struct reached via CVoiceHandle::field_0x3F60->field_0x08.
// Value at +0x18 is checked for range [1,6] by func_802A4798.
struct Field3F60Inner {
    u8 _pad[0x8];
    void* field_0x08;   // pointer to struct with field_0x18
};

// Target of Field3F60Inner::field_0x08; value at +0x18 checked.
struct Field3F60Inner2 {
    u8 _pad[0x18];
    s32 field_0x18;     // checked for [1,6] range
};

// Result object returned by CVoiceHandle vtable[169] (0x2A4/4).
// Provides voice-manager data through field_0x04 and field_0x50.
struct UnkVoiceObj {
    u8 _00[4];
    int field_0x04;         // passed to func_800B708C
    u8 _08[0x50 - 0x08];
    void* field_0x50;       // pointer to UnkVoiceSub
};

// Target of UnkVoiceObj::field_0x50; contains battle-voice flags.
struct UnkVoiceSub {
    u8 _00[0x48];
    u16 field_0x48;         // checked by func_80146148 / func_80146300
    u8 _4A[0x77 - 0x4A];
    u8 field_0x77;          // used by func_802A4120
};

extern "C" unsigned int func_802A35A0(unsigned int value);

// Functions from this TU
int func_802A3D54(CCharVoice* voicePtr, int voiceId, int arg);
int func_802A3E88(CVS_THREAD* self);
int func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
void func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
int func_802A4798(CVoiceHandle* handle);
int func_802A3EF0(CVoiceHandle* handle);
int func_802A3FD4(CVoiceHandle* handle);
int func_802A4120(CVoiceHandle* handle);
int func_802A4430(CVoiceHandle* handle);

// Sibling TU functions
CVoiceHandle* func_802A330C(int size, int align);
int func_802A3E28();
int func_802A77E8(CVoiceHandle* handle);
UnkCamObj* func_8049603C(CScn* scene);

// External functions
bool func_80146148(int value);
int func_80146300(int value, int flag);

// Global symbols
extern "C" {
extern CScn* lbl_eu_80663E14;
extern float lbl_eu_80668C88;
extern float lbl_eu_80662CB0;
extern float lbl_eu_80662CB4;
extern float lbl_eu_80662CB8;
}
