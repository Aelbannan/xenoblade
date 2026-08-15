#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "kyoshin/cf/object/CAIAction.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"

// Retail float constants referenced by CfObjectActor functions (SDA2 access).
extern float lbl_eu_80667738;   // CActorParam_UnkVirtualFunc34 / CfObjectActor_UnkVirtualFunc5 threshold

extern float lbl_eu_8066776C;   // CfObjectActor_UnkVirtualFunc8 fallback value

extern float lbl_eu_8066773C;   // CActorParam_UnkVirtualFunc179 CfObjectMove vf70 arg

extern float lbl_eu_80667740;   // CActorParam_UnkVirtualFunc33 clamp threshold

extern u32 lbl_eu_80663E24;     // event/presentation bitfield (.sbss; bit 0x10000000 gates the 33 path); non-volatile extern (see CfObjectMove.hpp); reads that must stay separate use explicit volatile casts

extern u32 lbl_eu_80663E28;     // mode bitfield (.sbss; bit 0x800 gates the 33 path)

extern const double lbl_eu_80667748;  // 2^52 + 2^31 magic (CfObjectActor_UnkVirtualFunc7 s16 -> float)

extern const double lbl_eu_80667750;  // +0.5 rounding add (CActorParam_UnkVirtualFunc35 gauge)

extern const double lbl_eu_80667758;  // -0.5 rounding add (CActorParam_UnkVirtualFunc35 gauge)

extern float lbl_eu_80667760;         // CActorParam_UnkVirtualFunc35 0xC3 ratio divisor

extern float lbl_eu_80667764;         // CActorParam_UnkVirtualFunc35 0xFC/0x100 gauge scale

extern float lbl_eu_80667768;         // CActorParam_UnkVirtualFunc35 0x100 decay add

// ptmf constant (3 words: this-offset / fn / this) compared against
// CBattleManager+0x28354 by CActorParam_UnkVirtualFunc35.
extern u32 lbl_eu_80531090[3];

// C-linkage imports (retail symbols are unmangled).
extern "C" int func_80055F94(u8* obj, int arg);   // CActParamData short-value helper
extern "C" void func_800BE12C(u8* obj, int a, int b, int c, int d);  // status add/remove

// Retail symbol is Fv but the real ABI passes (self, arg, f1, f2, f3);
// CfObjectActor::CActorParam_UnkVirtualFunc140 tail-calls this base entry
// (defined in CActorParam.cpp).
extern "C" void CActorParam_UnkVirtualFunc140__Q22cf11CActorParamFv(cf::CActorParam* self, void* arg, f32 a, f32 b, f32 c);

// Retail symbols are Fv; the real ABI passes (self, arg). The CfObjectActor
// overrides of 179/180 forward to these base entries (defined in CActorParam.cpp).
extern "C" void CActorParam_UnkVirtualFunc179__Q22cf11CActorParamFv(cf::CActorParam* self, void* arg);
extern "C" void CActorParam_UnkVirtualFunc180__Q22cf11CActorParamFv(cf::CActorParam* self, void* arg);

// C-ABI imports (retail symbols unmangled).
extern "C" void func_801A891C(void* obj, int arg);   // vision slot release

extern "C" int func_8027990C(void* battleSub, void* actor);  // battle sub-object query

extern "C" void func_8010975C(u8);

extern "C" void func_80109770(u8);

extern "C" void func_80109734(void* obj, u32 value);

extern "C" void func_80109784(void* obj, u32 a, int b);

extern "C" void func_8010989C(u8);

extern "C" void func_80109888(u8);

extern "C" void func_80109874(u8);

extern "C" bool func_8006EF04__Fi(int mask);

extern "C" void* getInstance__Q22cf13CfGameManagerFv(void);

extern "C" float func_800D81A8(void* obj, void* target, void* source);

extern "C" void func_8018C820(void* obj, int value);   // party-gauge add

extern "C" void func_800F38E0(void* battleMgr, void* actor, int flag);

extern "C" void func_80277A7C(void* battleSub, void* actor, float value);

extern "C" int __ptmf_cmpr(void* a, void* b);

namespace cf {
    //min size: 0x45BC
    class CfObjectActor : public CActorParam, public CAIAction, public CfObjectMove {
    public:
        CfObjectActor();
        //vtable 4 (CfObjectActor)
        virtual ~CfObjectActor();                  //0x5A0
        virtual void CfObjectActor_UnkVirtualFunc2();  //0x5A4
        virtual void CfObjectActor_UnkVirtualFunc3();  //0x5A8
        virtual void CfObjectActor_UnkVirtualFunc4();  //0x5AC
        virtual void CfObjectActor_UnkVirtualFunc5();  //0x5B0
        virtual float CfObjectActor_UnkVirtualFunc6();  //0x5B4
        virtual float CfObjectActor_UnkVirtualFunc7();  //0x5B8 (retail returns a float in f1)
        virtual float CfObjectActor_UnkVirtualFunc8();  //0x5BC (retail returns a float in f1)
        virtual void CfObjectActor_UnkVirtualFunc9();  //0x5C0
        virtual void CfObjectActor_UnkVirtualFunc10(); //0x5C4
        virtual void CfObjectActor_UnkVirtualFunc11(void* arg); //0x5C8
        virtual void CfObjectActor_UnkVirtualFunc12(); //0x5CC
        virtual void CfObjectActor_UnkVirtualFunc13(); //0x5D0

        //0x0: vtable 1
        //0x0-3380: CActorParam
        //0x3380: vtable 2
        //0x3380-3e9c: CAIAction
        //0x3e9c: vtable 3
        //0x3e9c-45b4: CfObjectMove
        u8 unk45B4[0x8];
    void CActorParam_UnkVirtualFunc1();
    void CActorParam_UnkVirtualFunc140();
    void CActorParam_UnkVirtualFunc179();
    void CActorParam_UnkVirtualFunc180();
    void CActorParam_UnkVirtualFunc33();
    void CActorParam_UnkVirtualFunc35();
    void CActorParam_UnkVirtualFunc34();
    void CActorParam_UnkVirtualFunc54();
    void CActorParam_UnkVirtualFunc60();
    void CActorParam_UnkVirtualFunc4();
    void CActorParam_UnkVirtualFunc21();
    void CActorParam_UnkVirtualFunc23();
    void func_801725DC();
    };

    // Vtable proxy for CfObjectActor's primary vtable (offset 0x00), slots
    // +0x118/+0x11C/+0x128/+0x12C/+0x154 (CActorParam_UnkVirtualFunc46/47/
    // 50/51/85: retail passes/returns floats through the slots even though
    // the base header declares them no-arg). See CActorParam_UnkVirtualFunc35.
    class CfActorParamVt118 {
    public:
        virtual void _d008(); virtual void _d00C(); virtual void _d010(); virtual void _d014();
        virtual void _d018(); virtual void _d01C(); virtual void _d020(); virtual void _d024();
        virtual void _d028(); virtual void _d02C(); virtual void _d030(); virtual void _d034();
        virtual void _d038(); virtual void _d03C(); virtual void _d040(); virtual void _d044();
        virtual void _d048(); virtual void _d04C(); virtual void _d050(); virtual void _d054();
        virtual void _d058(); virtual void _d05C(); virtual void _d060(); virtual void _d064();
        virtual void _d068(); virtual void _d06C(); virtual void _d070(); virtual void _d074();
        virtual void _d078(); virtual void _d07C(); virtual void _d080(); virtual void _d084();
        virtual void _d088(); virtual void _d08C(); virtual void _d090(); virtual void _d094();
        virtual void _d098(); virtual void _d09C(); virtual void _d0A0(); virtual void _d0A4();
        virtual void _d0A8(); virtual void _d0AC(); virtual void _d0B0(); virtual void _d0B4();
        virtual void _d0B8(); virtual void _d0BC(); virtual void _d0C0(); virtual void _d0C4();
        virtual void _d0C8(); virtual void _d0CC(); virtual void _d0D0(); virtual void _d0D4();
        virtual void _d0D8(); virtual void _d0DC(); virtual void _d0E0(); virtual void _d0E4();
        virtual void _d0E8(); virtual void _d0EC(); virtual void _d0F0(); virtual void _d0F4();
        virtual void _d0F8(); virtual void _d0FC(); virtual void _d100(); virtual void _d104();
        virtual void _d108(); virtual void _d10C(); virtual void _d110(); virtual void _d114();
        virtual void m118(float val);  // vtable +0x118 (setter)
        virtual void m11C(float val);  // vtable +0x11C (setter)
        virtual void _d120(); virtual void _d124();
        virtual float m128();          // vtable +0x128 (getter)
        virtual float m12C();          // vtable +0x12C (getter)
        virtual void _d130(); virtual void _d134(); virtual void _d138(); virtual void _d13C();
        virtual void _d140(); virtual void _d144(); virtual void _d148(); virtual void _d14C();
        virtual void _d150();
        virtual void m154(float val);  // vtable +0x154
    };

    // Vtable proxy for the CBattleState subobject vtable (this+0x8), slot
    // +0x24 (retail passes a pointer through the slot). See
    // CActorParam_UnkVirtualFunc35.
    class CfBattleVt24 {
    public:
        virtual void _d008(); virtual void _d00C(); virtual void _d010(); virtual void _d014();
        virtual void _d018(); virtual void _d01C(); virtual void _d020();
        virtual void m24(void* arg);  // vtable +0x24
    };

    // Status-entry view for the func_80149154 results used by
    // CActorParam_UnkVirtualFunc35: u32 marker at +0x8, s32 value at +0x10,
    // s16 gauge at +0x14.
    struct CfStatusEntry {
        u8 _pad[0x8];
        u32 field_0x8;   // 0x8
        u8 _pad0C[0x4];  // 0xC
        s32 field_0x10;  // 0x10
        s16 field_0x14;  // 0x14
    };

    // 12-byte ptmf copy built by CActorParam_UnkVirtualFunc35 from
    // lbl_eu_80531090 and compared against CBattleManager+0x28354.
    struct CfPtmf3 {
        u32 field_0;  // 0x0
        u32 field_4;  // 0x4
        u32 field_8;  // 0x8
    };

    // Vtable proxy for CfObjectActor vtable 1 slot +0x120
    // (CActorParam_UnkVirtualFunc35): retail passes (float, int, int, int)
    // through the slot even though the base header declares it no-arg. Dummy
    // slots pin the offset (MWCC puts the Nth declared virtual at (N+1)*4).
    class CfActorVt120 {
    public:
        virtual void _d008(); virtual void _d00C(); virtual void _d010(); virtual void _d014();
        virtual void _d018(); virtual void _d01C(); virtual void _d020(); virtual void _d024();
        virtual void _d028(); virtual void _d02C(); virtual void _d030(); virtual void _d034();
        virtual void _d038(); virtual void _d03C(); virtual void _d040(); virtual void _d044();
        virtual void _d048(); virtual void _d04C(); virtual void _d050(); virtual void _d054();
        virtual void _d058(); virtual void _d05C(); virtual void _d060(); virtual void _d064();
        virtual void _d068(); virtual void _d06C(); virtual void _d070(); virtual void _d074();
        virtual void _d078(); virtual void _d07C(); virtual void _d080(); virtual void _d084();
        virtual void _d088(); virtual void _d08C(); virtual void _d090(); virtual void _d094();
        virtual void _d098(); virtual void _d09C(); virtual void _d0A0(); virtual void _d0A4();
        virtual void _d0A8(); virtual void _d0AC(); virtual void _d0B0(); virtual void _d0B4();
        virtual void _d0B8(); virtual void _d0BC(); virtual void _d0C0(); virtual void _d0C4();
        virtual void _d0C8(); virtual void _d0CC(); virtual void _d0D0(); virtual void _d0D4();
        virtual void _d0D8(); virtual void _d0DC(); virtual void _d0E0(); virtual void _d0E4();
        virtual void _d0E8(); virtual void _d0EC(); virtual void _d0F0(); virtual void _d0F4();
        virtual void _d0F8(); virtual void _d0FC(); virtual void _d100(); virtual void _d104();
        virtual void _d108(); virtual void _d10C(); virtual void _d110(); virtual void _d114();
        virtual void _d118(); virtual void _d11C();
        virtual void m120(float value, int a, int b, int c);  // vtable +0x120
    };

    // Vtable proxy for the CBattleState subobject vtable (this+0x8), slot
    // +0x20 (CBattleState_UnkVirtualFunc7; retail passes an int - the base
    // header declares the slot no-arg). See CfObjectActor_UnkVirtualFunc3.
    class CfBattleVt20 {
    public:
        virtual void _d008(); virtual void _d00C(); virtual void _d010(); virtual void _d014();
        virtual void _d018(); virtual void _d01C();
        virtual void m20(int val);  // vtable +0x20
    };

    // Object holding a float at +0x7C (target of the CAIAction trailer word
    // read by CfObjectActor_UnkVirtualFunc8).
    struct CfFloat7C {
        u8 _pad[0x7C];
        float field_0x7C;  // 0x7C
    };

    // View of CfObjectActor's word at absolute offset 0x3E74 (CAIAction
    // trailer-area word, used as a pointer by CfObjectActor_UnkVirtualFunc8).
    struct CfActorField3E74 {
        u8 _pad[0x3E74];
        u32 field_0x3E74;  // 0x3E74
    };

    // View of CfObjectActor's word at absolute offset 0x3F10 (the
    // CfObjectModel.field_0x74 slot inside the CfObjectMove subobject) - used
    // by CActorParam_UnkVirtualFunc140 to compare two actors.
    struct CfActorField3F10 {
        u8 _pad[0x3F10];
        u32 field_0x3F10;  // 0x3F10
    };

    // View of CfObjectActor's flag word at absolute offset 0x3F00
    // (CfObjectMove+0x64) - compared between two actors by
    // CfObjectActor_UnkVirtualFunc9 (bits 0x2 / 0x4 select the dispatch).
    struct CfActorField3F00 {
        u8 _pad[0x3F00];
        u32 field_0x3F00;  // 0x3F00
    };

    // Same flag word viewed relative to the CfObjectMove subobject pointer
    // (func_800B708C returns one; CfObjectActor_UnkVirtualFunc10 reads it).
    struct CfMoveFlags64 {
        u8 _pad[0x64];
        u32 field_0x64;  // 0x64
    };

    // Absolute-offset views for CfObjectMove-subobject fields. The C++ base
    // layout in the headers is 8 bytes off retail (CfObjectMove lands at
    // +0x3EA4 instead of +0x3E9C), so subobject fields are reached through
    // raw views instead of static_cast upcasts.
    struct CfActorField3EE8 {
        u8 _pad[0x3EE8];
        float field_0x3EE8;  // 0x3EE8 (CfObjectMove+0x4C base height)
    };
    struct CfActorField3F60 {
        u8 _pad[0x3F60];
        u8* field_0x3F60;  // 0x3F60 (CfObjectMove+0xC4 move target)
    };
    struct CfActorField45B8 {
        u8 _pad[0x45B8];
        u8* field_0x45B8;  // 0x45B8 (action-source handle id)
    };

    // Downcast of a func_800B708C result (a CfObjectMove-subobject pointer)
    // back to the owning CfObjectActor. The retail's guarded `subi r3,r3,0x3e9c`
    // is exactly the C++ ternary null-guard, so the helper is written inline
    // as `m != 0 ? (CfObjectActor*)((u8*)m - 0x3E9C) : 0` at the call site.

    // View of CfObjectActor's pointer at absolute offset 0x04 (the
    // CActorState sub-object pointer): its vtable slot +0x30 returns a u32*
    // whose first word is the actor id queried by func_80174C98.
    struct CfActorField04 {
        u8 _pad[0x4];
        u8* field_0x04;  // 0x04
    };

    // Vtable proxy for the +0x04 sub-object's slot +0x30 (returns the actor-id
    // word holder). Dummy slots pin the offset (first declared virtual at
    // vtable+0x08; same shape as CHelpBattleSub4 in CTitle.hpp).
    class CfActorUnk4Vt30 {
    public:
        virtual void _d008(); virtual void _d00C(); virtual void _d010(); virtual void _d014();
        virtual void _d018(); virtual void _d01C(); virtual void _d020(); virtual void _d024();
        virtual void _d028(); virtual void _d02C();
        virtual u32* vf30();  // vtable +0x30
    };

    // u16/u32/s16 -> float conversion helper (CfMapEffectManager.hpp
    // convention): the retail builds the 0x43300000-prefixed bit pattern in
    // memory and subtracts the shared sdata2 magic double, so the pool reloc
    // matches the retail constant instead of an MWCC-synthesised @N entry.
    union CfActorF64Conv {
        u32 w[2];
        double d;
    };

    typedef void (*CfBattleVt20Fn)(u8* self, int val);   // CBattleState vtable slot +0x20
    typedef void (*CfActorVt5C4Fn)(u8* self, float value);  // CfObjectActor vtable slot +0x5C4

    // Function-pointer views of the CBattleState subobject vtable (this+0x08),
    // slot +0x20 (CBattleState_UnkVirtualFunc7; retail passes an int through
    // the slot). Used instead of the CfBattleVt20 virtual-call proxy because
    // the retail folds the +8 subobject offset into the vtable load; a virtual
    // call through a cast materializes this+0x8 into a register first.
    struct CfActorBattleVtPtr {
        u8 _pad[0x8];
        u32* vt;  // 0x08
    };
    struct CfBattleVt20Table {
        u32 _pad[0x20 / 4];   // 0x00-0x1F
        CfBattleVt20Fn fn20;  // 0x20
    };

    // Function-pointer view of CfObjectActor's primary vtable (offset 0x00),
    // slot +0x5C4 (CfObjectActor_UnkVirtualFunc10; retail passes a float
    // through the slot). CfObjectActor_UnkVirtualFunc10 dispatches this slot
    // on a downcast actor without needing a 370-slot virtual proxy.
    struct CfActorVtPtr {
        u32* vt;  // 0x00
    };
    // Function-pointer view of CfObjectActor's primary vtable (offset 0x00),
    // slot +0x5C4 (CfObjectActor_UnkVirtualFunc10; retail passes a float
    // through the slot). CfObjectActor_UnkVirtualFunc10 dispatches this slot
    // on a downcast actor; the fake interface with real virtuals emits the
    // retail r12 dispatch (lwz r12,0(r3); lwz r12,0x5C4(r12)) instead of the
    // manual cast's scratch r4 (MWCC_REFERENCE CModelDispEquip: N virtuals +
    // 8 hidden slots = slot index, so 0x5C4/4 - 8 = 361 fillers).
    struct CfActorVt5C4If {
        virtual void f000();
        virtual void f001();
        virtual void f002();
        virtual void f003();
        virtual void f004();
        virtual void f005();
        virtual void f006();
        virtual void f007();
        virtual void f008();
        virtual void f009();
        virtual void f00A();
        virtual void f00B();
        virtual void f00C();
        virtual void f00D();
        virtual void f00E();
        virtual void f00F();
        virtual void f010();
        virtual void f011();
        virtual void f012();
        virtual void f013();
        virtual void f014();
        virtual void f015();
        virtual void f016();
        virtual void f017();
        virtual void f018();
        virtual void f019();
        virtual void f01A();
        virtual void f01B();
        virtual void f01C();
        virtual void f01D();
        virtual void f01E();
        virtual void f01F();
        virtual void f020();
        virtual void f021();
        virtual void f022();
        virtual void f023();
        virtual void f024();
        virtual void f025();
        virtual void f026();
        virtual void f027();
        virtual void f028();
        virtual void f029();
        virtual void f02A();
        virtual void f02B();
        virtual void f02C();
        virtual void f02D();
        virtual void f02E();
        virtual void f02F();
        virtual void f030();
        virtual void f031();
        virtual void f032();
        virtual void f033();
        virtual void f034();
        virtual void f035();
        virtual void f036();
        virtual void f037();
        virtual void f038();
        virtual void f039();
        virtual void f03A();
        virtual void f03B();
        virtual void f03C();
        virtual void f03D();
        virtual void f03E();
        virtual void f03F();
        virtual void f040();
        virtual void f041();
        virtual void f042();
        virtual void f043();
        virtual void f044();
        virtual void f045();
        virtual void f046();
        virtual void f047();
        virtual void f048();
        virtual void f049();
        virtual void f04A();
        virtual void f04B();
        virtual void f04C();
        virtual void f04D();
        virtual void f04E();
        virtual void f04F();
        virtual void f050();
        virtual void f051();
        virtual void f052();
        virtual void f053();
        virtual void f054();
        virtual void f055();
        virtual void f056();
        virtual void f057();
        virtual void f058();
        virtual void f059();
        virtual void f05A();
        virtual void f05B();
        virtual void f05C();
        virtual void f05D();
        virtual void f05E();
        virtual void f05F();
        virtual void f060();
        virtual void f061();
        virtual void f062();
        virtual void f063();
        virtual void f064();
        virtual void f065();
        virtual void f066();
        virtual void f067();
        virtual void f068();
        virtual void f069();
        virtual void f06A();
        virtual void f06B();
        virtual void f06C();
        virtual void f06D();
        virtual void f06E();
        virtual void f06F();
        virtual void f070();
        virtual void f071();
        virtual void f072();
        virtual void f073();
        virtual void f074();
        virtual void f075();
        virtual void f076();
        virtual void f077();
        virtual void f078();
        virtual void f079();
        virtual void f07A();
        virtual void f07B();
        virtual void f07C();
        virtual void f07D();
        virtual void f07E();
        virtual void f07F();
        virtual void f080();
        virtual void f081();
        virtual void f082();
        virtual void f083();
        virtual void f084();
        virtual void f085();
        virtual void f086();
        virtual void f087();
        virtual void f088();
        virtual void f089();
        virtual void f08A();
        virtual void f08B();
        virtual void f08C();
        virtual void f08D();
        virtual void f08E();
        virtual void f08F();
        virtual void f090();
        virtual void f091();
        virtual void f092();
        virtual void f093();
        virtual void f094();
        virtual void f095();
        virtual void f096();
        virtual void f097();
        virtual void f098();
        virtual void f099();
        virtual void f09A();
        virtual void f09B();
        virtual void f09C();
        virtual void f09D();
        virtual void f09E();
        virtual void f09F();
        virtual void f0A0();
        virtual void f0A1();
        virtual void f0A2();
        virtual void f0A3();
        virtual void f0A4();
        virtual void f0A5();
        virtual void f0A6();
        virtual void f0A7();
        virtual void f0A8();
        virtual void f0A9();
        virtual void f0AA();
        virtual void f0AB();
        virtual void f0AC();
        virtual void f0AD();
        virtual void f0AE();
        virtual void f0AF();
        virtual void f0B0();
        virtual void f0B1();
        virtual void f0B2();
        virtual void f0B3();
        virtual void f0B4();
        virtual void f0B5();
        virtual void f0B6();
        virtual void f0B7();
        virtual void f0B8();
        virtual void f0B9();
        virtual void f0BA();
        virtual void f0BB();
        virtual void f0BC();
        virtual void f0BD();
        virtual void f0BE();
        virtual void f0BF();
        virtual void f0C0();
        virtual void f0C1();
        virtual void f0C2();
        virtual void f0C3();
        virtual void f0C4();
        virtual void f0C5();
        virtual void f0C6();
        virtual void f0C7();
        virtual void f0C8();
        virtual void f0C9();
        virtual void f0CA();
        virtual void f0CB();
        virtual void f0CC();
        virtual void f0CD();
        virtual void f0CE();
        virtual void f0CF();
        virtual void f0D0();
        virtual void f0D1();
        virtual void f0D2();
        virtual void f0D3();
        virtual void f0D4();
        virtual void f0D5();
        virtual void f0D6();
        virtual void f0D7();
        virtual void f0D8();
        virtual void f0D9();
        virtual void f0DA();
        virtual void f0DB();
        virtual void f0DC();
        virtual void f0DD();
        virtual void f0DE();
        virtual void f0DF();
        virtual void f0E0();
        virtual void f0E1();
        virtual void f0E2();
        virtual void f0E3();
        virtual void f0E4();
        virtual void f0E5();
        virtual void f0E6();
        virtual void f0E7();
        virtual void f0E8();
        virtual void f0E9();
        virtual void f0EA();
        virtual void f0EB();
        virtual void f0EC();
        virtual void f0ED();
        virtual void f0EE();
        virtual void f0EF();
        virtual void f0F0();
        virtual void f0F1();
        virtual void f0F2();
        virtual void f0F3();
        virtual void f0F4();
        virtual void f0F5();
        virtual void f0F6();
        virtual void f0F7();
        virtual void f0F8();
        virtual void f0F9();
        virtual void f0FA();
        virtual void f0FB();
        virtual void f0FC();
        virtual void f0FD();
        virtual void f0FE();
        virtual void f0FF();
        virtual void f100();
        virtual void f101();
        virtual void f102();
        virtual void f103();
        virtual void f104();
        virtual void f105();
        virtual void f106();
        virtual void f107();
        virtual void f108();
        virtual void f109();
        virtual void f10A();
        virtual void f10B();
        virtual void f10C();
        virtual void f10D();
        virtual void f10E();
        virtual void f10F();
        virtual void f110();
        virtual void f111();
        virtual void f112();
        virtual void f113();
        virtual void f114();
        virtual void f115();
        virtual void f116();
        virtual void f117();
        virtual void f118();
        virtual void f119();
        virtual void f11A();
        virtual void f11B();
        virtual void f11C();
        virtual void f11D();
        virtual void f11E();
        virtual void f11F();
        virtual void f120();
        virtual void f121();
        virtual void f122();
        virtual void f123();
        virtual void f124();
        virtual void f125();
        virtual void f126();
        virtual void f127();
        virtual void f128();
        virtual void f129();
        virtual void f12A();
        virtual void f12B();
        virtual void f12C();
        virtual void f12D();
        virtual void f12E();
        virtual void f12F();
        virtual void f130();
        virtual void f131();
        virtual void f132();
        virtual void f133();
        virtual void f134();
        virtual void f135();
        virtual void f136();
        virtual void f137();
        virtual void f138();
        virtual void f139();
        virtual void f13A();
        virtual void f13B();
        virtual void f13C();
        virtual void f13D();
        virtual void f13E();
        virtual void f13F();
        virtual void f140();
        virtual void f141();
        virtual void f142();
        virtual void f143();
        virtual void f144();
        virtual void f145();
        virtual void f146();
        virtual void f147();
        virtual void f148();
        virtual void f149();
        virtual void f14A();
        virtual void f14B();
        virtual void f14C();
        virtual void f14D();
        virtual void f14E();
        virtual void f14F();
        virtual void f150();
        virtual void f151();
        virtual void f152();
        virtual void f153();
        virtual void f154();
        virtual void f155();
        virtual void f156();
        virtual void f157();
        virtual void f158();
        virtual void f159();
        virtual void f15A();
        virtual void f15B();
        virtual void f15C();
        virtual void f15D();
        virtual void f15E();
        virtual void f15F();
        virtual void f160();
        virtual void f161();
        virtual void f162();
        virtual void f163();
        virtual void f164();
        virtual void f165();
        virtual void f166();
        virtual void f167();
        virtual void f168();
        virtual void f169();
        virtual void f16A();
        virtual void f16B();
        virtual void f16C();
        virtual void f16D();
        virtual void f16E();
        virtual void fn5C4(float value);  // slot 0x5C4 (361 fillers + 2 hidden)
    };


    // Argument passed through the CActorParam_UnkVirtualFunc179/180 slots
    // (retail ABI r4): u16 dispatch id at +0xC.
    struct CfActorParamArg {
        u8 _pad[0xC];
        u16 field_0xC;  // 0xC
    };

    // Absolute-offset view of CfObjectActor's CActorParam-region gauge/flag
    // fields (the C++ class layout in the headers is not trusted for these
    // functions; raw views pin the retail offsets).
    struct CfActorParamFields {
        u8 _pad[0x160C];
        s16 field_0x160C;  // 0x160C (arts/tension gauge current)
        s16 field_0x160E;  // 0x160E (gauge max)
        u8 _pad1610[0x1614 - 0x1610];
        s16 field_0x1614;  // 0x1614 (second gauge current)
        s16 field_0x1616;  // 0x1616 (second gauge max)
        u8 _pad1618[0x1634 - 0x1618];
        u32 field_0x1634;  // 0x1634
        u32 field_0x1638;  // 0x1638
        u32 field_0x163C;  // 0x163C
        u32 field_0x1640;  // 0x1640
        u32 field_0x1644;  // 0x1644
        u8 _pad1648[0x17E8 - 0x1648];
        f32 field_0x17E8;  // 0x17E8
        u8 _pad17EC[0x17F4 - 0x17EC];
        f32 field_0x17F4;  // 0x17F4 (gauge max)
        u8 _pad17F8[0x3374 - 0x17F8];
        u32 field_0x3374;  // 0x3374 (flags, bit 0x40000 gates the battle-state path)
    };

    // Stack buffer built by CActorParam_UnkVirtualFunc179 and passed to the
    // CBattleState subobject vtable slot +0x18 (exactly 0x34 bytes, matching
    // the memset size).
    struct CfActor179Buf {
        u32 field_0x00;       // 0x00
        u32 field_0x04;       // 0x04
        u8  field_0x08[4];    // 0x08
        u16 field_0x0C;       // 0x0C
        u32 field_0x10;       // 0x10
        u16 field_0x14;       // 0x14
        u16 field_0x16;       // 0x16
        u16 field_0x18;       // 0x18
        u8  field_0x1C[4];    // 0x1C
        f32 field_0x20;       // 0x20
        u8  field_0x24[0xC];  // 0x24
        u32 field_0x30;       // 0x30
    };

    // CfObjectActor's pointer at absolute offset 0x3ED4 (CfObjectMove+0x38
    // sub-object pointer; vtable slots +0x88/+0x8C take (self, arg) - see
    // CfSub3ED4Vt).
    struct CfActorField3ED4 {
        u8 _pad[0x3ED4];
        u8* field_0x3ED4;  // 0x3ED4
    };

    // Vtable proxy for the CBattleState subobject vtable (this+0x8), slot
    // +0x18 (CBattleState_UnkVirtualFunc5; retail passes a pointer through
    // the slot - the base header declares it with a typed entry arg).
    class CfBattleVt18 {
    public:
        virtual void _d008(); virtual void _d00C(); virtual void _d010(); virtual void _d014();
        virtual void m18(void* entry);  // vtable +0x18
    };

    // Vtable proxy for the +0x3ED4 sub-object (CfObjectMove+0x38): slots
    // +0x88/+0x8C take (self, arg). Dummy slots pin the offsets.
    class CfSub3ED4Vt {
    public:
        virtual void _d008(); virtual void _d00C(); virtual void _d010(); virtual void _d014();
        virtual void _d018(); virtual void _d01C(); virtual void _d020(); virtual void _d024();
        virtual void _d028(); virtual void _d02C(); virtual void _d030(); virtual void _d034();
        virtual void _d038(); virtual void _d03C(); virtual void _d040(); virtual void _d044();
        virtual void _d048(); virtual void _d04C(); virtual void _d050(); virtual void _d054();
        virtual void _d058(); virtual void _d05C(); virtual void _d060(); virtual void _d064();
        virtual void _d068(); virtual void _d06C(); virtual void _d070(); virtual void _d074();
        virtual void _d078(); virtual void _d07C(); virtual void _d080(); virtual void _d084();
        virtual void m88(void* arg);  // vtable +0x88
        virtual void m8C(void* arg);  // vtable +0x8C
    };

    // Vtable proxy for CfObjectActor's primary vtable (offset 0x00), slots
    // +0x168/+0x174/+0x178 (CActorParam_UnkVirtualFunc53/56/57) and
    // +0x180/+0x18C/+0x190 (CActorParam_UnkVirtualFunc59/62/63): the base
    // header declares them no-arg but retail passes/returns ints through them.
    class CfActorParamVt168 {
    public:
        virtual void _d008(); virtual void _d00C(); virtual void _d010(); virtual void _d014();
        virtual void _d018(); virtual void _d01C(); virtual void _d020(); virtual void _d024();
        virtual void _d028(); virtual void _d02C(); virtual void _d030(); virtual void _d034();
        virtual void _d038(); virtual void _d03C(); virtual void _d040(); virtual void _d044();
        virtual void _d048(); virtual void _d04C(); virtual void _d050(); virtual void _d054();
        virtual void _d058(); virtual void _d05C(); virtual void _d060(); virtual void _d064();
        virtual void _d068(); virtual void _d06C(); virtual void _d070(); virtual void _d074();
        virtual void _d078(); virtual void _d07C(); virtual void _d080(); virtual void _d084();
        virtual void _d088(); virtual void _d08C(); virtual void _d090(); virtual void _d094();
        virtual void _d098(); virtual void _d09C(); virtual void _d0A0(); virtual void _d0A4();
        virtual void _d0A8(); virtual void _d0AC(); virtual void _d0B0(); virtual void _d0B4();
        virtual void _d0B8(); virtual void _d0BC(); virtual void _d0C0(); virtual void _d0C4();
        virtual void _d0C8(); virtual void _d0CC(); virtual void _d0D0(); virtual void _d0D4();
        virtual void _d0D8(); virtual void _d0DC(); virtual void _d0E0(); virtual void _d0E4();
        virtual void _d0E8(); virtual void _d0EC(); virtual void _d0F0(); virtual void _d0F4();
        virtual void _d0F8(); virtual void _d0FC(); virtual void _d100(); virtual void _d104();
        virtual void _d108(); virtual void _d10C(); virtual void _d110(); virtual void _d114();
        virtual void _d118(); virtual void _d11C(); virtual void _d120(); virtual void _d124();
        virtual void _d128(); virtual void _d12C(); virtual void _d130(); virtual void _d134();
        virtual void _d138(); virtual void _d13C(); virtual void _d140(); virtual void _d144();
        virtual void _d148(); virtual void _d14C(); virtual void _d150(); virtual void _d154();
        virtual void _d158(); virtual void _d15C(); virtual void _d160(); virtual void _d164();
        virtual void m168(int val);  // vtable +0x168 (setter)
        virtual void _d16C(); virtual void _d170();
        virtual int m174();          // vtable +0x174 (getter)
        virtual int m178();          // vtable +0x178 (getter)
        virtual void _d17C();
        virtual void m180(int val);  // vtable +0x180 (setter)
        virtual void _d184(); virtual void _d188();
        virtual int m18C();          // vtable +0x18C (getter)
        virtual int m190();          // vtable +0x190 (getter)
    };
}
