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

    // {u32, f32, u32} entry repeated 8 times in UnkClass_801A3728 (0xC stride).
    struct CVisionU32F32U32 {
        u32 a;   // 0x00
        f32 b;   // 0x04
        u32 c;   // 0x08
    };

    class UnkClass_801A3728 {
    public:
        UnkClass_801A3728(){}
        ~UnkClass_801A3728(){}

        u32 w0;             // 0x00
        f32 f4;             // 0x04
        u32 w8;             // 0x08
        CVisionU32F32U32 arr[7]; // 0x0C (7 x 0xC = 0x54, ends 0x60)
        u32 w60;            // 0x60 (= this)
        u32 field_64;       // 0x64 (cleared by dtor)
        u32 field_68;       // 0x68 (cleared by dtor)
        u32 w6C;            // 0x6C (= 8)
        u16 unk70;          // 0x70
        u8 unk72[2];        // 0x72
        float unk74;        // 0x74
    };

    // Sub-object embedded at CVisionBattleObj+0x8 (its vtable is at +0x8,
    // slot 0x30 used by func_801A7704). Never instantiated -> no vtable
    // emitted. With -RTTI on, declared index N lands at vtable offset (N+2)*4.
    class CVisionBattleSub8 {
    public:
        virtual void s00();
        virtual void s04();
        virtual void s08();
        virtual void s0C();
        virtual void s10();
        virtual void s14();
        virtual void s18();
        virtual void s1C();
        virtual void s20();
        virtual void s24();
        virtual void s30(u32 a);   // declared index 10 -> vtable 0x30
    };

    // Object list created by func_80043D90 / filled by func_800F4A98; the
    // element count lives at +0x620.
    struct CVisionEnumList {
        u8 unk0[0x620];
        u32 count;   // 0x620
    };

    // CfObjectActor-derived object with the fields touched by
    // func_801A7704's second enumeration loop.
    class CVisionActorRef {
    public:
        u8 unk0[0x68];
        u32 field_68;   // 0x68
        u8 unk6C[0x8C - 0x6C];
        u16 field_8C;   // 0x8C
    };

    // Sub-object anchored at CVision+0x4 (valid when field_00 != 0). Size 0x834.
    class CVisionSubRef {
    public:
        u8 unk0[0x2C];      // 0x00
        float field_2C;     // 0x2C
        u8 unk30[0x7C - 0x30];
        float field_7C;     // 0x7C
    };

    // 0xBC-byte init block shared by the 10-entry CVisionSub::items array
    // (at +0xCC) and the CVisionSlot item array at +0x3FC8 (the CVisionSlot
    // +0x4720 struct is one of these blocks followed by a 0x2C-byte tail).
    // The constructors zero every field in this exact order; the two w_74/
    // w_78 stores at the end repeat the pair cleared near the start.
    class CVisionInitBlock {
    public:
        u32 w_00;           // 0x00
        u32 w_04;           // 0x04
        u8 unk_08[0x40];    // 0x08 (memset 0x40)
        u32 w_48;           // 0x48
        u32 w_4C;           // 0x4C (= -1)
        u32 w_50;           // 0x50
        f32 f_54;           // 0x54
        f32 f_58;           // 0x58
        f32 f_5C;           // 0x5C
        f32 f_60;           // 0x60
        f32 f_64;           // 0x64
        f32 f_68;           // 0x68
        f32 f_6C;           // 0x6C
        s16 h_70;           // 0x70 (s16: struct-copy emits lha)
        s16 h_72;           // 0x72 (s16: struct-copy emits lha)
        u32 w_74;           // 0x74
        u32 w_78;           // 0x78 (cleared by a 1-word loop together with w_7C bound)
        u32 w_7C;           // 0x7C
        u16 h_80;           // 0x80
        u8 unk_82[2];       // 0x82
        u8 unk_84[0x34];    // 0x84 (memset 0x34)
        u32 w_B8;           // 0xB8
    };

    // 5-word argument block passed by value to CVisionBattleObj::vf1E8.
    struct CVisionArg5 {
        u32 w[5];
    };

    // Large per-model battle object (func_8016FE34 result). Never
    // instantiated, so no vtable is emitted; filler virtuals keep the named
    // slots at their retail vtable offsets (declared index N -> (N+2)*4).
    class CVisionBattleObj {
    public:
        virtual void f000();   virtual void f001();   virtual void f002();   virtual void f003();
        virtual void f004();   virtual void f005();   virtual void f006();   virtual void f007();
        virtual void f008();   virtual void f009();   virtual void f010();   virtual void f011();
        virtual void f012();   virtual void f013();   virtual void f014();   virtual void f015();
        virtual void f016();   virtual void f017();   virtual void f018();   virtual void f019();
        virtual void f020();   virtual void f021();   virtual void f022();   virtual void f023();
        virtual void f024();   virtual void f025();   virtual void f026();   virtual void f027();
        virtual void f028();   virtual void f029();   virtual void f030();   virtual void f031();
        virtual void f032();   virtual void f033();   virtual void f034();   virtual void f035();
        virtual void f036();   virtual void f037();   virtual void f038();   virtual void f039();
        virtual void f040();   virtual void f041();   virtual void f042();   virtual void f043();
        virtual void f044();   virtual void f045();   virtual void f046();   virtual void f047();
        virtual void f048();   virtual void f049();   virtual void f050();   virtual void f051();
        virtual void f052();   virtual void f053();   virtual void f054();   virtual void f055();
        virtual void f056();   virtual void f057();   virtual void f058();   virtual void f059();
        virtual void f060();   virtual void f061();   virtual void f062();   virtual void f063();
        virtual void* vf108();   // index 64 -> vtable 0x108
        virtual void f065();   virtual void f066();   virtual void f067();   virtual void f068();
        virtual void f069();   virtual void f070();   virtual void f071();   virtual void f072();
        virtual void f073();   virtual void f074();   virtual void f075();   virtual void f076();
        virtual void f077();   virtual void f078();   virtual void f079();   virtual void f080();
        virtual void f081();   virtual void f082();   virtual void f083();   virtual void f084();
        virtual void f085();   virtual void f086();   virtual void f087();   virtual void f088();
        virtual void f089();   virtual void f090();   virtual void f091();   virtual void f092();
        virtual void f093();   virtual void f094();   virtual void f095();   virtual void f096();
        virtual void f097();   virtual void f098();   virtual void f099();   virtual void f100();
        virtual void f101();   virtual void f102();   virtual void f103();   virtual void f104();
        virtual void f105();   virtual void f106();   virtual void f107();   virtual void f108();
        virtual void f109();   virtual void f110();   virtual void f111();   virtual void f112();
        virtual void f113();   virtual void f114();   virtual void f115();   virtual void f116();
        virtual void f117();   virtual void f118();   virtual void f119();
        virtual void* vf1E8(CVisionArg5 a5);  // index 120 -> vtable 0x1e8
        virtual void f121();   virtual void f122();   virtual void f123();   virtual void f124();
        virtual void f125();   virtual void f126();   virtual void f127();   virtual void f128();
        virtual void f129();   virtual void f130();   virtual void f131();   virtual void f132();
        virtual void f133();   virtual void f134();   virtual void f135();   virtual void f136();
        virtual void f137();   virtual void f138();   virtual void f139();   virtual void f140();
        virtual void f141();   virtual void f142();   virtual void f143();   virtual void f144();
        virtual void f145();   virtual void f146();   virtual void f147();   virtual void f148();
        virtual void f149();   virtual void f150();   virtual void f151();   virtual void f152();
        virtual void f153();   virtual void f154();   virtual void f155();   virtual void f156();
        virtual void f157();   virtual void f158();   virtual void f159();   virtual void f160();
        virtual void f161();
        virtual int vf290();     // index 162 -> vtable 0x290
        virtual void f163();
        virtual CVisionInitBlock* vf298();        // index 164 -> vtable 0x298
        virtual CVisionInitBlock* vf29C(u32 idx); // index 165 -> vtable 0x29c
        virtual void f166();
        virtual CVisionInitBlock* vf2A4();        // index 167 -> vtable 0x2a4
        virtual int vf2A8();                      // index 168 -> vtable 0x2a8
        virtual void f169();   virtual void f170();   virtual void f171();   virtual void f172();
        virtual int vf2BC();   // index 173 -> vtable 0x2bc
        virtual void f174();
        virtual void vf2C4(void* a, f32 x, f32 y, f32 z);  // index 175 -> vtable 0x2c4
        virtual void f176();   virtual void f177();   virtual void f178();   virtual void f179();
        virtual void f180();   virtual void f181();   virtual void f182();   virtual void f183();
        virtual void f184();   virtual void f185();   virtual void f186();   virtual void f187();
        virtual void f188();   virtual void f189();   virtual void f190();
        virtual void vf304(u32 a);  // index 191 -> vtable 0x304
        virtual int vf308();        // index 192 -> vtable 0x308

        u32 field_04;             // 0x04
        CVisionBattleSub8 sub8;   // 0x08
    };

    class CVisionSub {
    public:
        u32 field_00;        // 0x00 model / id
        u32 field_04;        // 0x04
        u8 unk08[0x0C - 0x08];
        CVisionSubRef* field_0C;  // 0x0C
        u32 field_10;        // 0x10
        u32 field_14;        // 0x14
        u8 unk18[0x58 - 0x18];    // 0x18 (memset 0x40)
        u32 field_58;        // 0x58
        u32 field_5C;        // 0x5C (= -1)
        u32 field_60;        // 0x60
        f32 f_64;            // 0x64
        f32 f_68;            // 0x68
        f32 f_6C;            // 0x6C
        f32 f_70;            // 0x70
        f32 f_74;            // 0x74
        f32 f_78;            // 0x78
        f32 f_7C;            // 0x7C
        u16 h_80;            // 0x80
        u16 h_82;            // 0x82
        u32 field_84;        // 0x84
        u32 field_88;        // 0x88 (1-word clear target)
        u32 field_8C;        // 0x8C
        u16 field_90;        // 0x90
        u8 unk92[0x94 - 0x92];
        u8 unk94[0xC8 - 0x94];    // 0x94 (memset 0x34)
        u32 field_C8;        // 0xC8
        CVisionInitBlock items[10]; // 0xCC (10 x 0xBC = 0x758, ends 0x824)
        u32 field_824;       // 0x824 flag bits
        u8 unk828[0x830 - 0x828];
        f32 field_830;       // 0x830 (effective alpha/fader)
    };

    // Field block at CVisionSub+0x10, copied wholesale from the vf298
    // result in func_801A5444 (0xBC bytes, ends just before items at +0xCC).
    class CVisionSubMid {
    public:
        u32 field_10;   // 0x10
        u32 field_14;   // 0x14
        u8 unk18[0x40]; // 0x18 (memset 0x40)
        u32 field_58;   // 0x58
        u32 field_5C;   // 0x5C (= -1)
        u32 field_60;   // 0x60
        f32 f_64; f32 f_68; f32 f_6C; f32 f_70; f32 f_74; f32 f_78; f32 f_7C;
        s16 h_80;       // 0x80
        s16 h_82;       // 0x82
        u32 field_84;   // 0x84
        u32 field_88;   // 0x88
        u32 field_8C;   // 0x8C
        u16 field_90;   // 0x90
        u8 unk92[2];    // 0x92
        u8 unk94[0x34]; // 0x94 (memset 0x34)
        u32 field_C8;   // 0xC8
    };

    // Member-function pointer triplet (ptmf) at CVision+0x261B8.
    struct CVisionPtmf {
        u32 mPfn;    // 0x00
        u32 mObj;    // 0x04
        u32 mDelta;  // 0x08
    };

    // One of the 0x4818-byte members of CVision::unk20D4. The constructor
    // (retail __ct__801A33AC) runs the CBattleState / sub-object ctors and
    // the memset / word-clear loops below, then resets the tail fields.
    class CVisionSlot {
    public:
        u32 field_00;       // 0x00 vtable (lbl_eu_8052813C)
        u32 field_04;       // 0x04
        u32 field_08;       // 0x08
        u32 field_0C;       // 0x0C
        u8 unk10[0x15D8];   // 0x10 CBattleState ctor region (0x15D8 bytes)
        float f_15E8;   // 0x15E8
        u16 h_15EC;     // 0x15EC
        u16 h_15EE;     // 0x15EE
        u8 unk15F0[0x78];   // 0x15F0 func_801751DC sub-object
        u8 unk1668[0x78];   // 0x1668 func_801751DC sub-object
        u8 unk16E0[0x1520]; // 0x16E0 (memset 0x1520, ends 0x2C00)
        u8 unk2C00[0xC0];   // 0x2C00 (8 x 0x18 memset loop, ends 0x2CC0)
        u32 w_2CC0;     // 0x2CC0
        u32 w_2CC4;     // 0x2CC4
        u8 unk2CC8[0x34];   // 0x2CC8 cf::_sArtsSet ctor region (ends 0x2CFC)
        u32 field_2CFC; // 0x2CFC CArtsSet vtable (lbl_eu_8052F5C0)
        u8 unk2D00[0xD20];  // 0x2D00 24 x 0x8C CArtsParam region (ends 0x3A20)
        u8 unk3A20[0xC];    // 0x3A20
        u32 field_3A2C; // 0x3A2C CAttackSet vtable (lbl_eu_8052F5A8)
        u8 unk3A30[0x330];  // 0x3A30 6 x 0x88 CAttackParam region (ends 0x3D60)
        u8 unk3D60[0xC];    // 0x3D60
        float f_3D6C;   // 0x3D6C
        u8 unk3D70[0x1C];   // 0x3D70
        u8 unk3D8C[0x200];  // 0x3D8C 16 x 0x20 double-memset loop (ends 0x3F8C)
        u32 field_3F8C; // 0x3F8C (= this + 0x3D8C)
        u32 field_3F90; // 0x3F90
        u32 field_3F94; // 0x3F94
        u32 field_3F98; // 0x3F98 (= 0x10)
        u8 unk3F9C[4];  // 0x3F9C
        u8 unk3FA0[0xE];    // 0x3FA0 (memset 0xe)
        u8 unk3FAE[0xE];    // 0x3FAE
        u32 w_3FBC;     // 0x3FBC
        float f_3FC0;   // 0x3FC0
        u8 unk3FC4[4];  // 0x3FC4
        CVisionInitBlock items[10]; // 0x3FC8 (10 x 0xBC, ends 0x4720)
        CVisionInitBlock block4720; // 0x4720
        u8 unk47DC[0x10];   // 0x47DC
        u8 unk47EC[0x14];   // 0x47EC func_80174658 sub-object (ends 0x4800)
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

// Same-TU siblings (defined in CVision.cpp; C linkage keeps the call relocs
// and definitions at the unmangled retail names - CSuddenCommu.hpp idiom).
extern "C" void func_801A897C(cf::CVision* self, int slot, int r28);
extern "C" void func_801A8244(cf::CVision* self, void* r25, int r26, int r27, int r28);

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
extern "C" const f64 lbl_eu_80667D28;
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
extern "C" void func_800F4A98(void* list, u32 type, u32 filter);
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
extern "C" void* func_800EA444(void* self);
extern "C" void func_800A26A4(void* a, int b, void* c, int d, int e, int f, int g);
extern "C" int func_8026178C(void* data, u32 mode);
extern "C" f32 func_80496288(void*);
extern "C" void func_80170AB0(void* self, void* dst);
extern "C" void func_80174C24(void* obj, u32 flag);
extern "C" f64 lbl_eu_80667D18;
extern "C" f64 lbl_eu_80667D20;
extern "C" f32 lbl_eu_80667CDC;
extern "C" f32 lbl_eu_80667CD8;
extern "C" f32 lbl_eu_80667D50;
extern "C" f32 lbl_eu_80667D54;
extern "C" const f64 lbl_eu_80667D58;

// Mode-dispatch helpers used by func_801A8244's per-mode FX setup.
extern "C" void func_801ACD5C(u32 a);
extern "C" void func_800F3970(void* a, void* b, u32 c, u32 d, u32 e);
extern "C" void func_802A2078(u32 a, u32 b, void* c);
extern "C" void func_80280D04(u32 a);

// Per-slot FX helper used by func_801A897C's loop (func_80174C98 is
// declared by the chain headers; func_800F477C is declared inline below).
extern "C" f32 func_800F4424(void* a);

// 5-word data table copied into a local and handed to CVisionBattleObj::vf1E8.
extern "C" u32 lbl_eu_80503F80[5];

// Flag word OR-ed with 0x8 in func_801A8244.
extern "C" u32 lbl_eu_80663E28;

// Typeinfo name for __dynamic_cast in func_801A7704's second list loop.
extern "C" const void* lbl_eu_80661970;

// Callback ptmf tables (12-byte triplets).
extern "C" u32 lbl_eu_805331D0[3];
extern "C" u32 lbl_eu_805331F4[3];
extern "C" u32 lbl_eu_8053320C[3];
extern "C" u32 lbl_eu_805331E8[3];
extern "C" u32 lbl_eu_805331E0[3];
extern "C" u32 lbl_eu_805331CC[3];
// ptmf-triplet table: entries used at byte offsets 0x114/0x120/0x12c.
extern "C" u32 lbl_eu_80533128[0x4E];
// 80533170/80533140/8053314C are 12-byte callback triplets installed in
// func_801A506C / func_801A4578.
extern "C" cf::CVisionPtmf lbl_eu_80533170;
extern "C" cf::CVisionPtmf lbl_eu_80533140;
extern "C" cf::CVisionPtmf lbl_eu_8053314C;

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
extern "C" void* func_801412D0(u32 a);
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

// Sub-object constructors / vtable data used by the slot constructor
// (retail __ct__801A33AC). func_801751DC / func_80174658 are C-linkage
// constructors defined in CfMapItemManager.cpp; the lbl_eu_8052xxxx symbols
// are the CArtsSet / CAttackSet / CVisionSlot vtables stored by the slot ctor.
// They are declared as byte arrays so MWCC takes their addresses with
// absolute lis/addi (ADDR16_HA/LO) instead of sda21, matching the retail.
extern "C" void func_801751DC(void* self);
extern "C" void func_80174658(void* self);
// Declared larger than the 8-byte SDA threshold so MWCC takes their addresses
// with absolute lis/addi (ADDR16_HA/LO) instead of sda21, matching the retail.
extern "C" u8 lbl_eu_8052813C[0x40];
extern "C" u8 lbl_eu_8052F5C0[0x40];
extern "C" u8 lbl_eu_8052F5A8[0x40];
// Direct ctor calls for the sub-objects (placement new would add a null guard
// the retail does not have).
extern "C" void __ct__Q22cf12CBattleStateFv(void* self);
extern "C" void __ct__Q22cf9_sArtsSetFv(void* self);
extern "C" void __ct__Q22cf10CArtsParamFv(void* self);
extern "C" void __ct__Q22cf12CAttackParamFv(void* self);

