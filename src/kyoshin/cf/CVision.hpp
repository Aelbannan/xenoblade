#pragma once

#include <types.h>
#include "kyoshin/cf/object/IObjectInfo.hpp"
#include "monolib/core/CSchedule.hpp"

namespace cf{
    // One of the four vision-effect objects referenced by CVision::effectArray.
    // Only the fields touched by this TU are declared; the rest are opaque.
    class CVisionEffect {
    public:
        u8 unk0[0x68];          // 0x00
        u32 field_68;           // 0x68 flag (| 0x40 in func_801A80FC)
        u8 unk6C[0x94 - 0x6C];  // 0x6C
        CSchedule* mSchedule;   // 0x94
        u8 unk98[0xB0 - 0x98];  // 0x98
        u32 field_B0;           // 0xB0 (owning CVision back-pointer / clear)
    };

    class UnkClass_801A36D0 {
    public:
        u8 unk0[0x4818];
    };

    class UnkClass_801A3728 {
    public:
        UnkClass_801A3728(){}
        ~UnkClass_801A3728(){}

        u8 unk0[0x64];          // 0x00
        u32 field_64;           // 0x64 (cleared by dtor)
        u32 field_68;           // 0x68 (cleared by dtor)
        u8 unk6C[0x70 - 0x6C];  // 0x6C
        u16 unk70;              // 0x70
        u8 unk72[2];            // 0x72
        float unk74;            // 0x74
    };

    // Sub-object anchored at CVision+0x4 (valid when field_00 != 0). Size 0x834.
    class CVisionSub {
    public:
        u32 field_00;        // 0x00 model / id
        u32 field_04;        // 0x04
        u8 unk08[0x0C - 0x08];
        void* field_0C;      // 0x0C
        u8 unk10[0x824 - 0x10];
        u32 field_824;       // 0x824 flag bits
        u8 unk828[0x834 - 0x828]; // 0x828 pad to 0x834
    };

    // Member-function pointer triplet (ptmf) at CVision+0x261B8.
    struct CVisionPtmf {
        u32 mPfn;    // 0x00
        u32 mObj;    // 0x04
        u32 mDelta;  // 0x08
    };

    // One of the 0x4818-byte members of CVision::unk20D4 (per-slot sub-state).
    class CVisionSlot {
    public:
        u8 unk0[0x4818];
    };

    //size: 0x2623C
    class CVision : public IObjectInfo {
    public:
        CVision();
        virtual ~CVision();
        virtual void IObjectInfo_UnkVirtualFunc1();
        void func_801A380C();
        void func_801A929C(u32 r4);

        //0x0: vtable
        //0x0-4: IObjectInfo
        CVisionSub sub; //0x04 (0x834 bytes)
        u8 unk838[0x20D4 - 0x838]; //0x838..0x20D4
        UnkClass_801A36D0 unk20D4[8]; //0x20D4 (0x240C0 bytes, ends 0x26194)
        f32 field_26194; //0x26194
        f32 field_26198; //0x26198
        f32 field_2619C; //0x2619C
        f32 field_261A0; //0x261A0
        u8 field_261A4; //0x261A4
        u8 unk261A5[3]; //0x261A5
        CVisionEffect* effectArray[4]; //0x261A8 (four pointed-to effect objects)
        CVisionPtmf mPtmf; //0x261B8 (12 bytes)
        UnkClass_801A3728 unk261C4; //0x261C4 (size 0x78, ends 0x2623C)
    };

}
