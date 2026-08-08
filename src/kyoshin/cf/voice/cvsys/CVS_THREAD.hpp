#pragma once

#include <types.h>
#include "kyoshin/cf/voice/CCharVoice.hpp"

// ── Helper types for voice-ID selector functions ──────────────────────────

// Target pointer chain: obj->field_0x3F60 -> field_0x08 -> field_0x18
struct UnkTargetInner {
    u8 _00[0x18];
    s32 field_0x18;
};

struct UnkTarget {
    u8 _00[8];
    UnkTargetInner* field_0x08;
};

// Equipment data returned by vtable method at index 0xA9.
struct UnkEquipData {
    u8 _00[0x28];
    u16 subState;
    u8 _2A[0x4D];
    u8 field_0x77;
};

struct UnkWorkObj {
    u8 _00[0x50];
    UnkEquipData* field_0x50;
};

// Extended voice-handle covering the target field at 0x3F60.
// Low-offset fields mirror CVS_THREAD layout for shared access.
// Canonical definition: the embedded CCharVoice lives at +0x3E9C and the
// manager flags at +0x3F00 (bit 1 = thread factory active). Subclass
// headers must NOT redefine this type -- it is shared by every
// CVS_THREAD_* variant TU.
struct CVoiceHandle {
    u32* vtable;                         // 0x00
    u32 unk4;                            // 0x04
    u32 unk8;                            // 0x08
    u32 unkC;                            // 0x0C
    u32 unk10;                           // 0x10
    u32 unk14;                           // 0x14
    u32 unk18;                           // 0x18
    u8 _pad[0x3E9C - 0x1C];             // 0x1C-0x3E9B
    CCharVoice voice;                    // 0x3E9C-0x3EDB: embedded voice object
    u8 _pad2[0x3F00 - 0x3EDC];          // 0x3EDC-0x3EFF
    u32 field_0x3F00;                    // 0x3F00: manager flags (bit 1 = factory active)
    u8 _pad3[0x3F60 - 0x3F04];          // 0x3F04-0x3F5F
    UnkTarget* unkTarget;                // 0x3F60
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

extern "C" unsigned int func_802A35A0(unsigned int value);
