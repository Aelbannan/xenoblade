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
    class CVisionSubRef {
    public:
        u8 unk0[0x2C];      // 0x00
        float field_2C;     // 0x2C
        u8 unk30[0x7C - 0x30];
        float field_7C;     // 0x7C
    };

    class CVisionSub {
    public:
        u32 field_00;        // 0x00 model / id
        u32 field_04;        // 0x04
        u8 unk08[0x0C - 0x08];
        CVisionSubRef* field_0C;  // 0x0C
        u8 unk10[0x824 - 0x10];
        u32 field_824;       // 0x824 flag bits
        u8 unk828[0x830 - 0x828];
        f32 field_830;       // 0x830 (effective alpha/fader)
    };

    // Member-function pointer triplet (ptmf) at CVision+0x261B8.
    struct CVisionPtmf {
        u32 mPfn;    // 0x00
        u32 mObj;    // 0x04
        u32 mDelta;  // 0x08
    };

    // One of the 0x4818-byte members of CVision::unk20D4.
    class CVisionSlot {
    public:
        float f_15E8;   // 0x15E8
        u16 h_15EC;     // 0x15EC
        u16 h_15EE;     // 0x15EE
        u8 unk15F0[0x2CC0 - 0x15F0];
        u32 w_2CC0;     // 0x2CC0
        u32 w_2CC4;     // 0x2CC4
        u8 unk2CC8[0x3D6C - 0x2CC8];
        float f_3D6C;   // 0x3D6C
        u8 unk3D70[0x3FBC - 0x3D70];
        u32 w_3FBC;     // 0x3FBC
        float f_3FC0;   // 0x3FC0
        u8 unk3FC4[0x4800 - 0x3FC4];
        u32 w_4800;     // 0x4800
        u32 w_4804;     // 0x4804
        float f_4808;   // 0x4808
        float f_480C;   // 0x480C
        u32 w_4810;     // 0x4810
        u32 w_4814;     // 0x4814
    };

    //size: 0x2623C
    class CVision : public IObjectInfo {
    public:
        CVision();
        virtual ~CVision();
        virtual void IObjectInfo_UnkVirtualFunc1();
        virtual void vt_10();       //0x10
        virtual void vt_14();       //0x14
        virtual void vt_18();       //0x18
        virtual void vt_1C();       //0x1C (no-arg state refresh)
        virtual void vt_20(u32 r4); //0x20
        virtual void vt_24();       //0x24
        virtual void vt_28();       //0x28
        virtual void vt_2C(u32 r4); //0x2C
        virtual void vt_30(u32 r4);  //0x30
        virtual void vt_34();       //0x34
        void func_801A929C(u32 r4);

        //0x0: vtable
        //0x0-4: IObjectInfo
        CVisionSub sub; //0x04 (0x834 bytes)
        u8 unk838[0x20D4 - 0x838]; //0x838..0x20D4
        CVisionSlot unk20D4[8]; //0x20D4 (0x240C0 bytes, ends 0x26194)
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

// ---------------------------------------------------------------------------
// C-linkage imports: retail symbol names relocated from CVision.cpp.
// Keep `extern "C"` and the exact symbol names -- they ARE the retail names.
// ---------------------------------------------------------------------------
extern "C" void func_800ACC50(void* self, float v);
extern "C" void func_804E36DC(CSchedule* self, f32 dt);
extern "C" void* func_800451D8(u32 cls, int param);
extern "C" void __dl__FPv(void* ptr);

// Class-spec table (8-byte stride, only low word used) for func_801A808C.
struct CVisionClassTableEntry { u32 field_00; u32 field_04; };
extern "C" CVisionClassTableEntry lbl_eu_80503F60[4];

// sdata2 float constants used by func_801A929C / vision state checks.
extern "C" f32 lbl_eu_80667CD4;
extern "C" f32 lbl_eu_80667CF0;
extern "C" f32 lbl_eu_80667CD0;
extern "C" f32 lbl_eu_80667CE4;
extern "C" f32 lbl_eu_80667CEC;
extern "C" f32 lbl_eu_80667CF8;
extern "C" f32 lbl_eu_80667CFC;
extern "C" f32 lbl_eu_80667D04;
extern "C" f32 lbl_eu_80667CF4;
extern "C" f32 lbl_eu_80667D00;
extern "C" f32 lbl_eu_80667CE0;
extern "C" f32 lbl_eu_80667D08;
extern "C" f32 lbl_eu_80667D0C;
extern "C" f32 lbl_eu_80667D10;
extern "C" f64 lbl_eu_80667D28;
extern "C" f32 lbl_eu_80667D30;
extern "C" f32 lbl_eu_80667D34;
extern "C" f32 lbl_eu_80667D38;
extern "C" f32 lbl_eu_80667D3C;
extern "C" f32 lbl_eu_80667D40;
extern "C" f32 lbl_eu_80667D44;
extern "C" f32 lbl_eu_80667D48;
extern "C" f32 lbl_eu_80667CE8;

extern "C" void func_80081E90__Q22cf13CfGameManagerFv(u32 a, u32 b, u32 c);
extern "C" void func_8006E5D8();
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* func_8004392C(u32 a, u32 b, void* c, u32 d, u32 e, f32 f = 0.0f);
extern "C" bool func_801537E0(void* obj);
extern "C" void func_801537F0(void* obj);
extern "C" int func_800B708C__Fi(int id);
extern "C" float func_800F42AC(void* obj);
extern "C" void func_800F449C(void* obj);
extern "C" int func_80133F48(int id, float f);
extern "C" void* getPlayer__Q22cf13CfGameManagerFi(int idx);
extern "C" void func_801BFE8C(u32 a, u32 b, u32 c);
extern "C" void func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(u32 a, u32 b, u32 c, u32 d, f32 e);
extern "C" void func_80043D90(void* list);
extern "C" void* func_80043F18(void* list);
extern "C" void __dt__80043E88(void* list, int tags);
extern "C" u8* getGlobalSda(void);
extern "C" void func_8006E2FC(int id);
extern "C" u32 lbl_eu_80663E24;
extern "C" u8 lbl_eu_80663DA0;
extern "C" void* __dynamic_cast(void* src, long offset, const void* src_type,
                                const void* dst_type, void* src2dst);

// Player-list enumeration (CfObjectEnumList).
extern "C" void func_800F4A98(void* list, int type, int filter);
extern "C" void* func_800F6EAC(void* list, u32 idx);
extern "C" void func_80081F90__Q22cf13CfGameManagerFv(u32 a, u32 b);
extern "C" void func_80174B4C(void* obj, u32 flag);

// Typeinfo names for __dynamic_cast in the player-list loops.
extern "C" const void* lbl_eu_806618E8;
extern "C" const void* lbl_eu_806618F0;

extern "C" void func_8014AC38(void* a, void* b);
extern "C" int func_8014B8BC(void* a, void* b);
extern "C" int func_800F4730(void);
extern "C" void* func_800F4648(void* self);
extern "C" int func_800F46C0(void* a, void* b);
extern "C" void* func_800EA444(void);
extern "C" void func_800A26A4(void* a, int b, void* c, int d, int e, int f, int g);
extern "C" void func_8026178C(int mode);
extern "C" void func_80496288(void);
extern "C" void func_80170AB0(void* self, void* dst);
extern "C" void func_80174C24(void* obj, u32 flag);
extern "C" f64 lbl_eu_80667D18;
extern "C" f64 lbl_eu_80667D20;
extern "C" f32 lbl_eu_80667CDC;
extern "C" f32 lbl_eu_80667CD8;

// Callback ptmf tables (12-byte triplets).
extern "C" u32 lbl_eu_805331D0[3];
extern "C" u32 lbl_eu_805331F4[3];
extern "C" u32 lbl_eu_8053320C[3];
extern "C" u32 lbl_eu_805331E8[3];
extern "C" u32 lbl_eu_805331E0[3];
extern "C" u32 lbl_eu_805331CC[3];
extern "C" u32 lbl_eu_80533128[3];

extern "C" u32 lbl_eu_805331DC[3];
extern "C" u32 lbl_eu_805331C4[3];
extern "C" u32 lbl_eu_80533164[3];
extern "C" u32 lbl_eu_80533230[3];
extern "C" u32 lbl_eu_80533134[3];
extern "C" u32 lbl_eu_80533158[3];
extern "C" u32 lbl_eu_80533224[3];
extern "C" f32 lbl_eu_80667CD8;
extern "C" void* __RTTI__Q22cf13CfObjectActor;
extern "C" void func_8009D018(u32 a, u32 b);
extern "C" void func_801412D0(u32 a);
extern "C" void func_8013F244(void);
extern "C" void func_8016FF14(void* obj, void* dst);
extern "C" void func_801C01A8(u32 a, u32 b, f32 c);
extern "C" void func_801C028C(u32 a, u32 b);

extern "C" long __ptmf_test(void* ptmf);
extern "C" int __ptmf_cmpr(void* a, void* b);
extern "C" u32 __ptmf_null[3];
extern "C" u32 lbl_eu_8053317C[3];
extern "C" u32 lbl_eu_80533200[3];
extern "C" int func_eu_80053FD4(void);
extern "C" s32 CfRes_getE24Bit22(void);
extern "C" u32 func_801B481C(void);
extern "C" void* getInstance__Q22cf13CfGameManagerFv(void);

