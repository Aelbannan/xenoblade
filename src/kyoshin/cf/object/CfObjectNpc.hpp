#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObjectMove.hpp"

// C-ABI helper imports used by CfObjectNpc (retail-unmangled symbols).
extern "C" void* func_8003AA34();
extern "C" u32 getBdatStringColumnValue(void* bdatFile, const char* column, int row);
extern "C" void* getFP__FPCc(const char* path);
extern "C" void func_800BE33C(void* obj, int flag);
extern "C" void func_800BE824(void* obj, int flag);
extern "C" void func_804B0AD4(void* obj, int param, float a, float b);
extern "C" u32 func_80061FFC();
extern "C" f32 func_80496288(void* ptr);   // global frame-delta scalar
extern "C" int func_8013EB90(int v);

// Retail data labels referenced by this unit. lbl_eu_80663E24 and
// lbl_eu_80663E14 come from CfObjectMove.hpp.
extern u8 lbl_eu_804FC580[];   // BDAT column-name string table base
extern float lbl_eu_80666AE0;
extern float lbl_eu_80666AE4;
extern float lbl_eu_80666AE8;
extern float lbl_eu_80666AEC;
extern float lbl_eu_80666AF0;
extern float lbl_eu_80666AF4;
extern float lbl_eu_80666AF8;
extern float lbl_eu_80666AFC;
extern float lbl_eu_80666B00;

namespace cf {
    // size: 0x724
    class CfObjectNpc : public CfObjectMove {
    public:
        CfObjectNpc();
        virtual ~CfObjectNpc();

        // NPC-specific member functions
        bool func_800BF46C();
        void func_800BF4DC();
        void func_800BF764();
        void func_800BF8CC();
        s16 func_800BF920();
        u8 func_800BF984();

        // Reads the f32 returned by vtable slot 0x8C (declared void in the
        // base header, but retail leaves a scaled float in f1).
        float readSlot8c();

        // 0x0: vtable
        // 0x0-718: CfObjectMove

        // NPC-specific fields
        u8  pad_718[4];   // 0x718 - unknown
        u8  mIconType;    // 0x71C - NPC icon type (from BDAT icon_type column)
        u8  pad_71D;      // 0x71D - alignment padding
        s16 mRltMeet;     // 0x71E - relationship meet value, -1 = uninitialized
        f32 mTimer;       // 0x720 - timer for NPC dialogue trigger
    };
}
