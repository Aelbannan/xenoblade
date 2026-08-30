// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectPc
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"
// (getCameraDataBlock__Q22cf13CfGameManagerFv: CfSoundMan.hpp now carries the
// single winning decl from CfGameManagerApi.hpp; only playActorSound is
// needed from it.)
#include "kyoshin/cf/CfSoundMan.hpp"   // cf::CfSoundMan::playActorSound (single decl on owner header)
#include "monolib/scn/CScnTimeApi.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"
// (func_8009EC9C is declared by kyoshin/cf/object/CAIAction.hpp - u16 form;
// including CArtsInfo.hpp's u32 extern "C" here would be illegal overloading)
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

struct PCIf {
    virtual void _v0008();
    virtual void _v000C();
    virtual void _v0010();
    virtual void _v0014();
    virtual void _v0018();
    virtual void _v001C();
    virtual void _v0020();
    virtual void _v0024();
    virtual void _v0028();
    virtual void _v002C();
    virtual void _v0030();
    virtual void _v0034();
    virtual void _v0038();
    virtual void _v003C();
    virtual void _v0040();
    virtual void _v0044();
    virtual void _v0048();
    virtual void _v004C();
    virtual void _v0050();
    virtual void _v0054();
    virtual void _v0058();
    virtual void _v005C();
    virtual void _v0060();
    virtual void _v0064();
    virtual void _v0068();
    virtual void _v006C();
    virtual void _v0070();
    virtual void _v0074();
    virtual void _v0078();
    virtual void _v007C();
    virtual void _v0080();
    virtual void _v0084();
    virtual void _v0088();
    virtual void _v008C();
    virtual void _v0090();
    virtual void _v0094();
    virtual void _v0098();
    virtual void _v009C();
    virtual void _v00A0();
    virtual void _v00A4();
    virtual void _v00A8();
    virtual void _v00AC();
    virtual void _v00B0();
    virtual void _v00B4();
    virtual void _v00B8();
    virtual void _v00BC();
    virtual void _v00C0();
    virtual void _v00C4();
    virtual void _v00C8();
    virtual void _v00CC();
    virtual void _v00D0();
    virtual void _v00D4();
    virtual void _v00D8();
    virtual void _v00DC();
    virtual void _v00E0();
    virtual void _v00E4();
    virtual void _v00E8();
    virtual void _v00EC();
    virtual void _v00F0();
    virtual void _v00F4();
    virtual void _v00F8();
    virtual u32 _v00FC();
    virtual void _v0100();
    virtual void _v0104();
    virtual void _v0108();
    virtual void _v010C();
    virtual void _v0110();
    virtual void _v0114();
    virtual void _v0118();
    virtual void _v011C(double value);
    virtual void _v0120();
    virtual void _v0124();
    virtual float _v0128();
    virtual float _v012C();
    virtual void _v0130();
    virtual void _v0134();
    virtual void _v0138();
    virtual void _v013C();
    virtual void _v0140();
    virtual void _v0144();
    virtual void _v0148();
    virtual void _v014C();
    virtual void _v0150();
    virtual void _v0154();
    virtual void _v0158();
    virtual void _v015C();
    virtual void _v0160();
    virtual void _v0164();
    virtual void _v0168();
    virtual void _v016C();
    virtual void _v0170();
    virtual void _v0174();
    virtual void _v0178();
    virtual void _v017C();
    virtual void _v0180();
    virtual void _v0184();
    virtual void _v0188();
    virtual void _v018C();
    virtual void _v0190();
    virtual void _v0194();
    virtual void _v0198();
    virtual void _v019C();
    virtual void _v01A0();
    virtual void _v01A4();
    virtual void _v01A8();
    virtual void _v01AC();
    virtual void _v01B0();
    virtual void _v01B4();
    virtual void _v01B8();
    virtual void _v01BC();
    virtual void _v01C0();
    virtual void _v01C4();
    virtual void _v01C8();
    virtual void _v01CC();
    virtual void _v01D0();
    virtual void _v01D4();
    virtual void _v01D8();
    virtual void _v01DC();
    virtual void _v01E0();
    virtual void _v01E4();
    virtual u32 _v01E8();
    virtual u32 _v01EC();
    virtual void _v01F0();
    virtual void _v01F4();
    virtual void _v01F8();
    virtual void _v01FC();
    virtual void _v0200();
    virtual void _v0204(u32 value);
    virtual void _v0208();
    virtual void _v020C();
    virtual void _v0210();
    virtual void _v0214();
    virtual void _v0218();
    virtual void _v021C(u32 value);
    virtual void _v0220();
    virtual void _v0224();
    virtual void _v0228();
    virtual void _v022C();
    virtual void _v0230();
    virtual void _v0234();
    virtual void _v0238();
    virtual void _v023C();
    virtual void _v0240();
    virtual void _v0244();
    virtual void _v0248();
    virtual void _v024C();
    virtual void _v0250();
    virtual void _v0254();
    virtual void _v0258();
    virtual void _v025C();
    virtual void _v0260();
    virtual void _v0264();
    virtual void _v0268();
    virtual void _v026C();
    virtual void _v0270();
    virtual void _v0274();
    virtual void _v0278();
    virtual void _v027C();
    virtual void _v0280();
    virtual void _v0284();
    virtual void _v0288();
    virtual u32 _v028C();
    virtual u32 _v0290();
    virtual void _v0294();
    virtual void _v0298();
    virtual void _v029C();
    virtual void _v02A0();
    virtual void _v02A4();
    virtual void _v02A8();
    virtual void _v02AC();
    virtual void _v02B0();
    virtual void _v02B4();
    virtual void _v02B8();
    virtual int _v02BC();
    virtual void _v02C0();
    virtual void _v02C4();
    virtual void _v02C8();
    virtual void _v02CC();
    virtual void _v02D0();
    virtual void _v02D4();
    virtual void _v02D8();
    virtual void _v02DC();
    virtual void _v02E0();
    virtual void _v02E4();
    virtual void _v02E8();
    virtual void _v02EC();
    virtual void _v02F0();
    virtual void _v02F4();
    virtual void _v02F8();
    virtual void _v02FC();
    virtual void _v0300();
    virtual void _v0304();
    virtual int _v0308();
    virtual void _v030C();
    virtual void _v0310();
    virtual void _v0314();
    virtual void _v0318();
    virtual void _v031C();
    virtual void _v0320();
    virtual void _v0324();
    virtual void _v0328();
    virtual void _v032C();
    virtual void _v0330();
    virtual void _v0334();
    virtual void _v0338();
    virtual void _v033C();
    virtual void _v0340();
    virtual void _v0344();
    virtual void _v0348();
    virtual void _v034C();
    virtual void _v0350();
    virtual void _v0354();
    virtual void _v0358();
    virtual int _v035C();
    virtual void _v0360();
    virtual void _v0364();
    virtual void _v0368();
    virtual void _v036C();
    virtual void _v0370();
    virtual void _v0374();
    virtual void _v0378();
    virtual void _v037C();
    virtual void _v0380();
    virtual void _v0384();
    virtual void _v0388();
    virtual void _v038C();
    virtual void _v0390();
    virtual void _v0394();
    virtual void _v0398();
    virtual void _v039C();
    virtual void _v03A0();
    virtual void _v03A4();
    virtual void _v03A8();
    virtual void _v03AC();
    virtual void _v03B0();
    virtual void _v03B4();
    virtual void _v03B8();
    virtual void _v03BC();
    virtual void _v03C0();
    virtual void _v03C4();
    virtual void _v03C8();
    virtual void _v03CC();
    virtual void _v03D0();
    virtual void _v03D4();
    virtual void _v03D8();
    virtual void _v03DC();
    virtual void _v03E0();
    virtual void _v03E4();
    virtual void _v03E8();
    virtual void _v03EC();
    virtual void _v03F0();
    virtual void _v03F4();
    virtual void _v03F8();
    virtual void _v03FC();
    virtual void _v0400();
    virtual void _v0404();
    virtual void _v0408();
    virtual void _v040C();
    virtual void _v0410();
    virtual void _v0414();
    virtual void _v0418();
    virtual void _v041C();
    virtual void _v0420();
    virtual void _v0424();
    virtual void _v0428();
    virtual void _v042C();
    virtual void _v0430();
    virtual void _v0434();
    virtual void _v0438();
    virtual void _v043C();
    virtual void _v0440();
    virtual void _v0444();
    virtual void _v0448();
    virtual void _v044C();
    virtual void _v0450();
    virtual void _v0454();
    virtual void _v0458();
    virtual void _v045C();
    virtual void _v0460();
    virtual void _v0464();
    virtual void _v0468();
    virtual void _v046C();
    virtual void _v0470();
    virtual void _v0474();
    virtual void _v0478();
    virtual void _v047C();
    virtual void _v0480();
    virtual void _v0484();
    virtual void _v0488();
    virtual void _v048C();
    virtual void _v0490();
    virtual void _v0494();
    virtual void _v0498();
    virtual void _v049C();
    virtual void _v04A0();
    virtual void _v04A4();
    virtual void _v04A8();
    virtual void _v04AC();
    virtual void _v04B0();
    virtual void _v04B4();
    virtual void _v04B8();
    virtual void _v04BC();
    virtual void _v04C0();
    virtual void _v04C4();
    virtual void _v04C8();
    virtual void _v04CC();
    virtual void _v04D0();
    virtual void _v04D4();
    virtual void _v04D8();
    virtual void _v04DC();
    virtual void _v04E0();
    virtual void _v04E4();
    virtual void _v04E8();
    virtual void _v04EC();
    virtual void _v04F0();
    virtual void _v04F4();
    virtual void _v04F8();
    virtual void _v04FC();
    virtual void _v0500();
    virtual void _v0504();
    virtual void _v0508();
    virtual void _v050C();
    virtual void _v0510();
    virtual void _v0514();
    virtual void _v0518();
    virtual void _v051C();
    virtual void _v0520();
    virtual void _v0524();
    virtual void _v0528();
    virtual void _v052C();
    virtual void _v0530();
    virtual void _v0534();
    virtual void _v0538();
    virtual void _v053C();
    virtual void _v0540();
    virtual void _v0544();
    virtual void _v0548();
    virtual void _v054C();
    virtual void _v0550();
    virtual void _v0554();
    virtual void _v0558();
    virtual void _v055C();
    virtual void _v0560();
    virtual void _v0564();
    virtual void _v0568();
    virtual void _v056C();
    virtual void _v0570();
    virtual void _v0574();
    virtual void _v0578();
    virtual void _v057C();
    virtual void _v0580();
    virtual void _v0584();
    virtual void _v0588();
    virtual void _v058C();
    virtual void _v0590();
    virtual void _v0594();
    virtual void _v0598();
    virtual void _v059C();
    virtual void _v05A0();
    virtual void _v05A4();
    virtual void _v05A8();
    virtual void _v05AC();
    virtual void _v05B0();
    virtual void _v05B4();
    virtual void _v05B8();
    virtual void _v05BC();
    virtual void _v05C0();
    virtual void _v05C4();
    virtual void _v05C8();
    virtual void _v05CC();
    virtual void _v05D0();
    virtual void _v05D4();
    virtual void _v05D8();
    virtual void _v05DC();
    virtual void _v05E0();
    virtual void _v05E4();
    virtual void _v05E8();
    virtual u32 _v05EC();
    virtual void _v05F0();
    virtual void vf05F4();
    virtual void vf05F8();
    virtual void vf05FC();
    virtual void vf0600();
    virtual void vf0604();
    virtual void vf0608(int a, int b, int c, int d, int e);
};

// Interface over the +0x3E9C subobject vtable (lbl_eu_80529DA0 + 0x37C
// table): slots used by the CfObjectPc methods below.
class PcSubVtIf {
public:
    virtual void _p008(); virtual int _p00C(int flag); virtual void _p010(); virtual void _p014();
    virtual void _p018(); virtual void _p01C(); virtual void _p020(); virtual void _p024();
    virtual void _p028(); virtual void _p02C(); virtual void _p030(); virtual void _p034();
    virtual void _p038(); virtual void _p03C(); virtual void _p040(); virtual void _p044();
    virtual void _p048(); virtual void _p04C(); virtual void _p050(int flag); virtual void _p054();
    virtual void _p058(); virtual void _p05C(); virtual void _p060(); virtual void _p064();
    virtual void _p068(); virtual void _p06C(); virtual void _p070(); virtual void _p074();
    virtual void _p078(); virtual void _p07C(); virtual void _p080(); virtual void _p084();
    virtual void _p088(); virtual float _p08C(); virtual void _p090(); virtual void _p094();
    virtual void _p098(); virtual void _p09C(); virtual void _p0A0(); virtual void _p0A4();
    virtual void _p0A8(); virtual void _p0AC(); virtual void _p0B0(); virtual void _p0B4();
    virtual void _p0B8(); virtual void _p0BC(); virtual void _p0C0(); virtual void _p0C4();
    virtual void _p0C8(); virtual void _p0CC(); virtual void _p0D0(); virtual void _p0D4();
    virtual void _p0D8(); virtual void _p0DC(); virtual void _p0E0(); virtual void _p0E4();
    virtual void _p0E8(); virtual void _p0EC(); virtual void _p0F0(); virtual void _p0F4();
    virtual void _p0F8(); virtual void _p0FC(); virtual void _p100(); virtual void _p104();
    virtual void _p108(); virtual void _p10C(); virtual void _p110(); virtual void _p114();
    virtual void _p118(); virtual void _p11C(); virtual void _p120(); virtual void _p124();
    virtual void _p128(); virtual void _p12C(); virtual void _p130();
    virtual void m134(float);   // +0x134
    virtual void _p138();
    virtual void m13C(float);   // +0x13C
    virtual void _p140(); virtual void _p144(); virtual void _p148();
    virtual void _p14C(); virtual void _p150(); virtual void _p154();
    virtual void m158(int);     // +0x158
    virtual void _p15C(); virtual void _p160(); virtual void _p164(); virtual void _p168();
    virtual void _p16C(); virtual void _p170(); virtual void _p174(); virtual void _p178();
    virtual void _p17C(); virtual void _p180(); virtual void _p184(); virtual void _p188();
    virtual void _p18C(); virtual void _p190(); virtual void _p194(); virtual void _p198();
    virtual void _p19C(); virtual void _p1A0(); virtual void _p1A4(); virtual void _p1A8();
    virtual void _p1AC(int flag, const char* text); virtual void _p1B0(); virtual void _p1B4(); virtual void _p1B8();
    virtual void _p1BC();
    virtual void m1C0(int);     // +0x1C0
    virtual void _p1C4(); virtual void _p1C8(); virtual void _p1CC(); virtual void _p1D0();
    virtual void m1D4(float);   // +0x1D4
    virtual void _p1D8(); virtual void _p1DC(); virtual void _p1E0(); virtual void _p1E4();
    virtual void _p1E8(); virtual void _p1EC(); virtual void _p1F0(); virtual void _p1F4();
    virtual void _p1F8(); virtual void _p1FC(); virtual void _p200(); virtual void _p204();
    virtual void _p208();
    virtual int _p20C(int flag);   // +0x20C
    virtual int _p210(int flag);   // +0x210
};
// Fake-MI view: folds the vptr load into lwz r12, 0x3E9C(rX) and re-materializes
// the adjusted this (addi r3, rX, 0x3E9C), matching retail.
struct PcSubPad { u8 _pad[0x3E9C]; };
struct PcSubFake : PcSubPad, PcSubVtIf {};
// Distinct type with identical layout: spelling call sites through both types
// keeps MWCC from CSE-ing the sub-object address into one temp (retail
// re-materializes addi rX, rX, 0x3E9C at every call).
struct PcSubPadB { u8 _pad[0x3E9C]; };
struct PcSubFakeB : PcSubPadB, PcSubVtIf {};
// Third spelling for three-site alternation.
struct PcSubPadC { u8 _pad[0x3E9C]; };
struct PcSubFakeC : PcSubPadC, PcSubVtIf {};
// Member-subobject spellings: retail fuses the +0x3E9C adjust into the
// vptr load (lwzu r12, 0x3E9C(r3)), which MWCC emits for member
// subobject calls rather than cast-produced pointer temps.
struct PcWrapA { u8 _pad[0x3E9C]; PcSubVtIf mSub; };
struct PcWrapB { u8 _pad[0x3E9C]; PcSubVtIf mSub; };

// Interface over the arts-data entry object embedded at +0x17C (its vtable
// slot 0x20C fills the entry).
class ArtsEntryVt20CIf {
public:
    virtual void _q008(); virtual void _q00C(); virtual void _q010(); virtual void _q014();
    virtual void _q018(); virtual void _q01C(); virtual void _q020(); virtual void _q024();
    virtual void _q028(); virtual void _q02C(); virtual void _q030(); virtual void _q034();
    virtual void _q038(); virtual void _q03C(); virtual void _q040(); virtual void _q044();
    virtual void _q048(); virtual void _q04C(); virtual void _q050(); virtual void _q054();
    virtual void _q058(); virtual void _q05C(); virtual void _q060(); virtual void _q064();
    virtual void _q068(); virtual void _q06C(); virtual void _q070(); virtual void _q074();
    virtual void _q078(); virtual void _q07C(); virtual void _q080(); virtual void _q084();
    virtual void _q088(); virtual void _q08C(); virtual void _q090(); virtual void _q094();
    virtual void _q098(); virtual void _q09C(); virtual void _q0A0(); virtual void _q0A4();
    virtual void _q0A8(); virtual void _q0AC(); virtual void _q0B0(); virtual void _q0B4();
    virtual void _q0B8(); virtual void _q0BC(); virtual void _q0C0(); virtual void _q0C4();
    virtual void _q0C8(); virtual void _q0CC(); virtual void _q0D0(); virtual void _q0D4();
    virtual void _q0D8(); virtual void _q0DC(); virtual void _q0E0(); virtual void _q0E4();
    virtual void _q0E8(); virtual void _q0EC(); virtual void _q0F0(); virtual void _q0F4();
    virtual void _q0F8(); virtual void _q0FC(); virtual void _q100(); virtual void _q104();
    virtual void _q108(); virtual void _q10C(); virtual void _q110(); virtual void _q114();
    virtual void _q118(); virtual void _q11C(); virtual void _q120(); virtual void _q124();
    virtual void _q128(); virtual void _q12C(); virtual void _q130(); virtual void _q134();
    virtual void _q138(); virtual void _q13C(); virtual void _q140(); virtual void _q144();
    virtual void _q148(); virtual void _q14C(); virtual void _q150(); virtual void _q154();
    virtual void _q158(); virtual void _q15C(); virtual void _q160(); virtual void _q164();
    virtual void _q168(); virtual void _q16C(); virtual void _q170(); virtual void _q174();
    virtual void _q178(); virtual void _q17C(); virtual void _q180(); virtual void _q184();
    virtual void _q188(); virtual void _q18C(); virtual void _q190(); virtual void _q194();
    virtual void _q198(); virtual void _q19C(); virtual void _q1A0(); virtual void _q1A4();
    virtual void _q1A8(); virtual void _q1AC(); virtual void _q1B0(); virtual void _q1B4();
    virtual void _q1B8(); virtual void _q1BC(); virtual void _q1C0(); virtual void _q1C4();
    virtual void _q1C8(); virtual void _q1CC(); virtual void _q1D0(); virtual void _q1D4();
    virtual void _q1D8(); virtual void _q1DC(); virtual void _q1E0(); virtual void _q1E4();
    virtual void _q1E8(); virtual void _q1EC(); virtual void _q1F0(); virtual void _q1F4();
    virtual void _q1F8(); virtual void _q1FC(); virtual void _q200(); virtual void _q204();
    virtual void _q208();
    virtual u32 fill();         // +0x20C
};
struct ArtsEntryPad { u8 _pad[0x17C]; };
struct ArtsEntryFake : ArtsEntryPad, ArtsEntryVt20CIf {};

// +0x44A8 region u16 touched at the end of initialize.
struct PcRegion44A8 {
    u8 _pad[0xB2];
    u16 field_B2;
};

namespace cf {
// Minimal view of CfObjectPoint for validatePointState: only the vtable layout
// matters (retail dispatches slot 0x70 = CfObject_UnkVirtualFunc8). With RTTI
// on, declared virtual N sits at vtable offset (N+2)*4, so 26 padding slots
// put the callee at 0x70. The class is never instantiated in this TU, so the
// padding virtuals need no definitions and no vtable is emitted.
class CfObjectPoint {
public:
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual void _v058();
    virtual void _v05C();
    virtual void _v060();
    virtual void _v064();
    virtual void _v068();
    virtual void _v06C();
    virtual void CfObject_UnkVirtualFunc8();  // vtable offset 0x70
    int validatePointState();
};
}

cf::CfObjectPc::CfObjectPc() : CfObjectActor() {
    // novtable class: write the four retail sub-vtable pointers manually
    // (base labels 0x0 / +0xC / +0x36C / +0x37C at offsets 0x0 / 0x8 /
    // 0x3380 / 0x3E9C).
    *(void**)this = (void*)lbl_eu_80529DA0;
    *(void**)((u8*)this + 0x8) = (void*)(lbl_eu_80529DA0 + 0xC);
    *(void**)((u8*)this + 0x3380) = (void*)(lbl_eu_80529DA0 + 0x36C);
    *(void**)((u8*)this + 0x3E9C) = (void*)(lbl_eu_80529DA0 + 0x37C);

    // Allocate the 0x44-byte CfResPcImpl resource object and construct it in
    // place, passing the player sub-object (this + 0x3E9C, null-checked). The
    // ctor returns the object in r3, so no extra callee-saved register is
    // needed for the final store.
    u8* res = (u8*)allocate__Q23mtl10MemManagerFUlUl(0x44, func_80061FFC());
    if (res != NULL) {
        // Player sub-object (this + 0x3E9C), null-checked like a secondary-
        // base conversion; the declared base sizes are 8 bytes off retail.
        u8* parent = (u8*)this;
        if (this != NULL) {
            parent += 0x3E9C;
        }
        res = (u8*)__ct__cf_CfResPcImpl(res, (cf::CfObjectMove*)parent);
    }
    ((CfObjectPcSubFields*)this)->mPtr3F4C = res;
    reinterpret_cast<PCIf*>(this)->_v05D8();
}

// Complete destructor (retail symbol __dt__Q22cf10CfObjectPcFv). Free-
// function treatment like CfObjectEne: restores the four retail sub-vtable
// pointers, runs the syncArtsEntry slot on the primary vtable, explicitly
// destroys the CfObjectMove subobject, runs the inlined CAIAction block
// cleanup, then frees when deleteFlag > 0.
cf::CfObjectPc* __dt__Q22cf10CfObjectPcFv(cf::CfObjectPc* self, s32 deleteFlag) {
    if (self != 0) {
        u8* v = (u8*)lbl_eu_80529DA0;
        ((cf::CfPcVt*)self)->vt0 = (u32)v;
        ((cf::CfPcVt*)self)->vt8 = (u32)(v + 0xC);
        ((cf::CfPcVt*)self)->vt3380 = (u32)(v + 0x36C);
        ((cf::CfPcVt*)self)->vt3E9C = (u32)(v + 0x37C);
        reinterpret_cast<PCIf*>(self)->_v05E0();
        // Retail re-checks self here (cmpwi/beq) before destroying the
        // CfObjectMove subobject and running the CAIAction block cleanup.
        if (self != 0) {
            __dt__Q22cf12CfObjectMoveFv((cf::CfObjectMove*)((u8*)self + 0x3E9C), 0);
            cf::CfPcCAISub* ai = (cf::CfPcCAISub*)((u8*)self + 0x3380);
            if (ai != NULL) {
                CfPcCAIBlock* blk = (CfPcCAIBlock*)&ai->field_4;
                if (blk != NULL) {
                    if (&blk->field_8 != NULL) {
                        blk->field_210 = 0;
                        blk->field_20C = 0;
                    }
                }
            }
        }
        if (deleteFlag > 0)
            __dl__FPv(self);
    }
    return self;
}

// Per-frame PC setup: refreshes the CfResPcImpl resource object and runs
// slots 0x5EC/0x330/0x5F4; when this is the active player's move sub-object
// also resets the screen. For arts row 4, copies four bdat string bytes into
// +0x1629..+0x162C using the row from func_800A32BC (the column getter
// returns a string pointer; only its first byte is consumed).
void func_800BFDE0(cf::CfObjectPc* obj) {
    CfObjectPcSubFields* f = (CfObjectPcSubFields*)obj;
    func_8018CBE8(f->mPtr3F4C);
    reinterpret_cast<PCIf*>(obj)->_v05EC();
    reinterpret_cast<PCIf*>(obj)->_v0330();
    reinterpret_cast<PCIf*>(obj)->vf05F4();
    u8* moveSub = (u8*)obj;
    if (obj != NULL) {
        moveSub = (u8*)obj + 0x3E9C;
    }
    if (moveSub == (u8*)getPlayer__Q22cf13CfGameManagerFi(0)) {
        func_8012FAA8();
    }
    if (f->field_0x3F28 == 4) {
        // Declared in this order for MWCC register coloring
        // (names/file/data/col -> r31/r30/r29/r28); assigned in retail's
        // runtime order.
        // Declared here for register coloring; the first call references
        // the label directly so MWCC splits the @ha/@l address formation
        // around the call like retail.
        const char* names;
        void* file;
        CfObjectPcArtsData* data;
        int col;
        data = (CfObjectPcArtsData*)func_8009EC9C(f->field_0x3F28);
        file = lbl_eu_80664090;
        col = 4;
        // Signed compare (cmpi) in retail.
        if ((int)func_800A32BC(data) == 1) {
            col = 0xC;
        }
        // Union memory round-trip (same shape as CBattleState): MWCC homes
        // each union local to the stack, emitting stw/lbz pairs.
        union { u32 w; u8 b; } u0, u1, u2, u3;
        u0.w = getBdatStringColumnValue(file, (const char*)lbl_eu_804FC5EC, col);
        f->field_0x1629 = u0.b;
        names = (const char*)lbl_eu_804FC5EC;
        u1.w = getBdatStringColumnValue(file, names + 7, col);
        f->field_0x162A = u1.b;
        u2.w = getBdatStringColumnValue(file, names + 0xF, col);
        f->field_0x162B = u2.b;
        u3.w = getBdatStringColumnValue(file, names + 0x19, col);
        f->field_0x162C = u3.b;
        func_800A13C4((u8*)data, 1);
    }
}

// vf2 override: reset-style init (see CfObjectEne's func_800ADB2C for the
// sibling shape). All accesses go through the local `self` so MWCC keeps it
// in the saved this-copy register and reassigns it to the +0x44A8 region
// (retail reuses the same callee-saved register for both).
int cf::CfObjectPc::initialize() {
    CfObject_UnkVirtualFunc2__Q22cf13CfObjectModelFv(
        (cf::CfObjectModel*)((u8*)this + 0x3E9C));
    u8* self = (u8*)this;
    // Retail tests this with a record-form mask keeping only bit 0 (msb):
    // source shape (x & 0x80000000).
    if (!(((CfObjectPcSubFields*)self)->field_0x3F00 & 0x80000000)) {
        ((CfObjectPcSubFields*)self)->field_0x3F08 |= 1;
    }
    ((PcSubFake*)self)->m158(1);
    func_800BE33C((char*)self + 0x3E9C, 1);
    ((PcSubFakeB*)((u32)self))->m1C0(1);
    func_80174B4C(self, 0x8000000);
    func_80174B4C(self, 0x10000000);
    func_800BE824((u8*)((u32)self + 0x3E9C), 1);
    u8* region = self + 0x44A8;
    func_804B0AD4(region, 0, lbl_eu_80666B0C, lbl_eu_80666B10);
    ((PcRegion44A8*)region)->field_B2 = 100;
    return 1;
}

// Adjuster thunk: retarget to the +0x3E9C subobject and tail-call into
// syncArtsEntry (qualified call suppresses virtual dispatch; defined before
// syncArtsEntry so MWCC emits a tail-branch rather than inlining it).
void cf::CfObjectPc::CfObject_UnkVirtualFunc6() { ((cf::CfObjectPc*)((char*)this - 0x3e9c))->cf::CfObjectPc::syncArtsEntry(); }

void cf::CfObjectPc::syncArtsEntry() {
    reinterpret_cast<PCIf*>(this)->_v00B0();
    reinterpret_cast<PCIf*>(this)->_v00B8();
    reinterpret_cast<PCIf*>(this)->_v0314();
    // Look up the arts data object for this PC's index, then write the
    // per-entry value at +0x17C from the CActorParam virtual 0x28C result.
    CfObjectPcArtsData* artsData = (CfObjectPcArtsData*)func_8009EC9C(
        ((CfObjectPcSubFields*)this)->field_0x3F28);
    func_80175A50(&artsData->field_0x17C,
        reinterpret_cast<PCIf*>(this)->_v028C());
}

// NOTE: enablePcFlag is defined as the extern "C" wrapper below (not as a
// member): the adjuster thunks reference the global identifier, and the
// wrapper also satisfies the vtable slot for the virtual member.
extern "C" void enablePcFlag__Q22cf10CfObjectPcFv(cf::CfObjectPc* self) {
    ((CfObjectPcSubFields*)self)->field_0x4568 = 1;
    func_80174B4C((u8*)self, 0x00100000);
    ((CfObjectPcSubFields*)self)->field_0x45C0 = lbl_eu_80666B14;
}

// vf0x5E4: refresh the arts data for this PC's row and reset the sub-object
// pointer word at +0x15E0.
// NOTE: defined as the extern "C" wrapper (not as a member): the adjuster
// thunks reference the global identifier, and the wrapper also satisfies the
// vtable slot for the virtual member (same pattern as enablePcFlag).
extern "C" void func_800C00C0__Q22cf10CfObjectPcFv(cf::CfObjectPc* self) {
    CfObjectPcArtsData* data = (CfObjectPcArtsData*)func_8009EC9C(
        ((CfObjectPcSubFields*)self)->field_0x3F28);
    // Recomputed lookup: retail keeps both results live (the first feeds
    // func_8009EF9C across the call, the second forms the +0x3534 pointer).
    CfObjectPcArtsData* data2 = (CfObjectPcArtsData*)func_8009EC9C(
        ((CfObjectPcSubFields*)self)->field_0x3F28);
    ((CfObjectPcSubFields*)self)->field_0x15E0 = (u32)data2 + 0x3534;
    func_8009EF9C(data, 0);
    reinterpret_cast<PCIf*>(self)->vf05F4();
    // NOTE: retail re-materializes addi r3, r30, 0x3E9C before EACH of the
    // three calls below, but MWCC value-numbers the identical receiver
    // conversions into one callee-saved temp (see MWCC_PATTERNS.md 7j
    // negative result) - known unmatchable residual.
    // NOTE: retail re-materializes addi r3, r30, 0x3E9C before EACH of the
    // three calls below, but MWCC value-numbers the identical receiver
    // conversions into one callee-saved temp regardless of spelling (MI-base
    // cast, distinct types, member sub-object - all probed; see
    // MWCC_PATTERNS.md 7j negative result). Known unmatchable residual.
    ((PcSubFake*)self)->m134(lbl_eu_80666B18);
    ((PcSubFake*)self)->m13C(lbl_eu_80666B1C);
    ((PcSubFake*)self)->m1D4(lbl_eu_80666B20);
}


// (body defined below under its retail mangled name)

// Vtable slot 16 override (CfObjectMove vtable): the adjuster thunk passes the
// CfObjectMove sub-object, and the vtable caller supplies a second int in r4
// even though the retail mangled name is arg-less. Small args delegate to the
// base CfObjectMove implementation; larger ones forward a sub-object pointer
// to func_800CA42C.
void handleMoveState__Q22cf10CfObjectPcFv(cf::CfObjectPc* self, int arg) {
    if (arg < 0x2c) {
        // CfObjectMove sub-object lives at this+0x3E9C (manual offset: the
        // declared base-class sizes in the headers do not match the retail
        // layout). Direct (non-virtual) call to the base implementation.
        u8* subObj = reinterpret_cast<u8*>(self) + 0x3e9c;
        ((cf::CfObjectMove*)subObj)->CfObjectMove::CfObjectMove_UnkVirtualFunc16();
    } else {
        u8* obj = ((CfObjectPcSubFields*)self)->mPtr3ED4;
        if (obj != NULL) {
            func_800CA42C(obj);
        }
    }
}

// Scans the first six arts-data entries (indices 0..5) for one whose u16
// flag at +0x1A is non-zero ("occupied"); stops at the first hit.
void cf::CfObjectPc::scanArtsEntries() {
    CfObjectPcArtsData* data = (CfObjectPcArtsData*)func_8009EC9C(
        ((CfObjectPcSubFields*)this)->field_0x3F28);
    for (int i = 0; i <= 5; i++) {
        if (func_8009D7E4(&data->mEntries, i)->field_0x1A != 0) break;
    }
}

// Applies arts entry itemId when it matches the row index at +0x3F28:
// constructs the payload (dest), fills the entry object embedded at
// artsData+0x17C through its vtable slot 0x20C, feeds the result to this
// object's slot 0x21C, then runs slot 0x28C and stores via func_80175A50.
void func_800C01D4(cf::CfObjectPc* self, void* dest, s32 itemId) {
    if ((u32)itemId == (u32)((CfObjectPcSubFields*)self)->field_0x3F28) {
        __ct__8009ED08(dest, itemId);
        CfObjectPcArtsData* data =
            (CfObjectPcArtsData*)func_8009EC9C(itemId & 0xFFFF);
        reinterpret_cast<PCIf*>(self)->_v021C(((ArtsEntryFake*)data)->fill());
        // Copies the arts entry INTO this object's CActorParam (opposite
        // direction of syncArtsEntry).
        func_80175A50(reinterpret_cast<u8*>(reinterpret_cast<PCIf*>(self)->_v028C()),
            reinterpret_cast<u32>(&data->field_0x17C));
    }
}

// Looks up the data object for the index at +0x3F28, then runs its ctor.
void CActorParam_UnkVirtualFunc166__Q22cf10CfObjectPcFv(cf::CfObjectPc* self) {
    __ct__8009F8B8((u8*)func_8009EC9C(((CfObjectPcSubFields*)self)->field_0x3F28));
}

void CActorParam_UnkVirtualFunc167__Q22cf10CfObjectPcFv(cf::CfObjectPc* self) {
    func_800A03F4((u8*)func_8009EC9C(((CfObjectPcSubFields*)self)->field_0x3F28));
}

void resetArtsState__Q22cf10CfObjectPcFv(cf::CfObjectPc* self) {
    func_800A145C((u8*)func_8009EC9C(((CfObjectPcSubFields*)self)->field_0x3F28));
}

// Adjuster-style override (retail mangled name is arg-less Fv but the vtable
// caller supplies the parameter in r4): applies the arts data entry identified
// by the index at +0x3F28, passing the extra value through.
void applyArtsParam__Q22cf10CfObjectPcFv(cf::CfObjectPc* self, int param) {
    if (param == 0) return;
    func_800A18A4((u8*)func_8009EC9C(((CfObjectPcSubFields*)self)->field_0x3F28), param);
}

void cf::CfObjectPc::dispatchPlayerBranch() {
    // getPlayer(0) returns the player's CfObjectMove sub-object; compare it
    // against this object's CfObjectMove base (null-checked pointer adjust)
    // and dispatch to the matching per-frame handler. Dispatches go through
    // PCIf (fresh vtable class, retail slot offsets) because the shared base
    // headers declare four extra virtuals.
    u8* target = (u8*)this;
    if (this != NULL) {
        target += 0x3E9C;
    }
    if (target == (u8*)getPlayer__Q22cf13CfGameManagerFi(0)) {
        reinterpret_cast<PCIf*>(this)->vf05F8();
    } else {
        reinterpret_cast<PCIf*>(this)->vf05FC();
    }
}

// vf0x5F8: reinstall the default action table on the +0x3380 CAIAction
// subobject: clear it, then install entries 0 and 1 (both 6-frame, kinds
// 7 and 0xE, gauge 100).
void cf::CfObjectPc::resetActionTable() {
    u8* self = (u8*)this;
    func_8014B7B0(self + 0x3380);
    func_8014B804(self + 0x3380, 0, 1, 0, 6, 0, 0, 7, 0, 0, 0, 2, 100, 0);
    func_8014B804(self + 0x3380, 1, 1, 0, 6, 0, 0, 0xE, 0, 0, 0, 2, 100, 0);
}

void cf::CfObjectPc::setupActionTable() {
    // The three uses of this+0x3380 are written in different syntaxes so MWCC
    // does not CSE the address into a callee-saved register (retail recomputes
    // addi r3, r31, 0x3380 at each call site).
    func_8014B7B0((u8*)this + 0x3380);
    if (((CfObjectPcSubFields*)this)->field_0x3F28 == 4) {
        // Arts-count bdat row: enable the art column when the string value's
        // first byte is '1' (the value is returned packed; read its low byte).
        CfObjectPcArtsData* artsData = (CfObjectPcArtsData*)func_8009EC9C(
            ((CfObjectPcSubFields*)this)->field_0x3F28);
        u32 val = getBdatStringColumnValue(lbl_eu_806640F4,
            (const char*)lbl_eu_804FC5EC + 0x24, artsData->field_0xC);
        if (*(s8*)&val == 1) {
            func_8015396C(&((u8*)this)[0x3380], 0x70, 0xd);
            return;
        }
    }
    func_8015396C((u8*)((u32)this + 0x3380), 0, 0);
}



// Main per-frame update. Gate checks via the +0x4/+0x8 sub-objects decide
// between the cancel path (slot 0x20 of the +0x8 sub-object) and the full
// update: battle-manager list probe, action-table probes 0x65..0x6D/0x35,
// gauge scale refresh (+0x8C slot x global scale), flag OR-ing, and the
// player-only event branches; finally the move-subobject UnkVirtualFunc4.

// Retail helper defined below (stub): gate check feeding the cancel path.
extern "C" int func_800C0DD4(cf::CfObjectPc* self, int flag);

// Retail symbol is the arg-less Fv member; defined as a free function with
// C linkage so the first gate probe reads the state pointer straight off
// the incoming parameter register like retail (lwz r3, 0x4(r3)).
extern "C" void func_800C0524__Q22cf10CfObjectPcFv(cf::CfObjectPc* self) {
    CfObjectPcSubFields* f = (CfObjectPcSubFields*)self;
    // Two gate probes against the +0x4 sub-object state getter, written as
    // single expressions (CSuddenCommu idiom): the state word is copied into
    // a local through the pointer returned by slot 0x30.
    void* st1;
    void* st2;
    void* st3;
    int chk1 = func_80174C98(self, (int*)&(st1 = *(void**)((PcSub4Fake*)self)->_q030()), 0xE);
    int chk2 = func_80174C98(self, (int*)&(st2 = *(void**)((PcSub4FakeB*)self)->_q030()), 0x803);

    int any = (chk1 | chk2) != 0;
    int dd = func_800C0DD4(self, any);
    if ((any | dd) != 0) {
        if (func_80148778((u8*)self + 8, 0x35)) {
            ((PcSub8Fake*)self)->_s020(0x35);
        }
    } else {
        // Count entries in the battle-manager list at +0x48.
        BmListNode* head =
            &((BattleMgrListView*)getInstance__Q22cf14CBattleManagerFv())
                ->mListHead;
        int count = 0;
        for (BmListNode* n = head->mNext; n != head; n = n->mNext) {
            count++;
        }
        // Busy with any listed action -> clear action 0x35; idle -> install it.
        if (count != 0 ||
            func_80148778((u8*)self + 8, 0x65) ||
            func_80148778((u8*)self + 8, 0x66) ||
            func_80148778((u8*)self + 8, 0x67) ||
            func_80148778((u8*)self + 8, 0x67) ||
            func_80148778((u8*)self + 8, 0x69) ||
            func_80148778((u8*)self + 8, 0x6a) ||
            func_80148778((u8*)self + 8, 0x6b) ||
            func_80148778((u8*)self + 8, 0x6c) ||
            func_80148778((u8*)self + 8, 0x6d)) {
            if (func_80148778((u8*)self + 8, 0x35)) {
                ((PcSub8FakeB*)self)->_s020(0x35);
            }
        } else {
            if (!func_80148778((u8*)self + 8, 0x35)) {
                ((PcSub8FakeB*)self)->_s014(0x35);
            }
        }
    }

    // Gauge scale: slot 0x8C value times the global scale factor.
    float gauge = ((PcSubFakeB*)self)->_p08C();
    float scaled = func_80496288(lbl_eu_80663E14) * gauge;
    func_801765A4(self, scaled, 1);

    int chkFlag = func_80174C98(self, (int*)&(st3 = *(void**)((PcSub4FakeC*)self)->_q030()), 1);
    if (chkFlag != 0 && f->mPtr3F60 != NULL) {
        f->mPtr3F60->field_0x4EC |= 0x1000;
    }

    // Player-only event handling (null-checked adjust to the move sub-object).
    u8* moveSub = (u8*)self;
    if (self != NULL) {
        moveSub += 0x3E9C;
    }
    if (moveSub == (u8*)getPlayer__Q22cf13CfGameManagerFi(0)) {
        getInstance__Q22cf13CfGameManagerFv();
        if (isGlobalCamFlagSet__Fi(0x100000)) {
            // Distinct fake types per +0x3E9C dispatch so MWCC re-materializes
            // addi r3, r31, 0x3E9C like retail instead of caching one temp.
            if (chkFlag == 0 && ((PcSubFake*)self)->_p00C(1) == 0) {
                if (f->field_0x3F34 != NULL) {
                    f->field_0x3F34->field_0x7A4 |= 0x8000;
                }
                ((PcSubFakeB*)self)->_p080();
                ((PcSubFakeB*)self)->_p064();
                return;
            }
        } else {
            if (chkFlag != 0 && func_8013EB90(1) == 0) {
                func_80174B4C(self, 3);
                ((PcSubFake*)self)->_p050(0);
                ((PcSubFake*)self)->_p1AC(0,
                    (const char*)lbl_eu_804FC5EC + 0x2D);
            }
        }
    }
    CfObject_UnkVirtualFunc4__Q22cf12CfObjectMoveFv(
        (cf::CfObjectMove*)((u8*)self + 0x3E9C));
}

// Gauge-scale refresh: runs the CActorParam base handler, then reads a text
// literal through slot 0x308 and scales/divides +0x1824 by an indexed s16
// from it, normalized by the three float constants.
void cf::CfObjectPc::CActorParam_UnkVirtualFunc4() {
    CfObjectPcSubFields* f = (CfObjectPcSubFields*)this;
    // Direct (non-virtual) call to the CActorParam base implementation.
    ((cf::CActorParam*)this)->cf::CActorParam::CActorParam_UnkVirtualFunc4(nullptr);
    Lit10 buf = lbl_eu_804FC5E0;
    PCIf* p = reinterpret_cast<PCIf*>(this);
    int n = p->_v0308();
    if (n <= 1) {
        int idx = p->_v0308();
        // (float)(s32) builtin cast: MWCC emits the 0x43300000 biased-magic
        // conversion (single-rounded fsubs) whose pooled constant is the
        // retail lbl_eu_80666B30 literal.
        float v = (float)((s16*)buf.d)[idx];
        f->field_0x1824 =
            f->field_0x1824 * (lbl_eu_80666B24 + v / lbl_eu_80666B28);
    } else {
        int n2 = p->_v0308();
        if (n2 >= 3) {
            int idx = p->_v0308();
            float v = (float)((s16*)buf.d)[idx];
            f->field_0x1824 =
                f->field_0x1824 / (lbl_eu_80666B24 + v / lbl_eu_80666B28);
        }
    }
}

void CActorParam_UnkVirtualFunc173__Q22cf10CfObjectPcFv(void) {}

// CActorParam virtual 0x354 override. The retail mangled name is arg-less
// (Fv) but the dispatcher passes a float in f1; forward it to the base
// implementation (same naming quirk) after touching the battle singleton.
void CActorParam_UnkVirtualFunc176__Q22cf10CfObjectPcFv(cf::CfObjectPc* self, float value) {
    getInstance__Q22cf14CBattleManagerFv();
    CActorParam_UnkVirtualFunc176__Q22cf11CActorParamFv(self, value);
}

// Retail symbol CActorParam_UnkVirtualFunc86__Q22cf10CfObjectPcFv (vtable slot
// 0x1EC override). Written as a global with the mangled name: the base
// CActorParam override returns u32 in retail but the base header declares it
// void, so a real member override would fail to compile.
u32 CActorParam_UnkVirtualFunc86__Q22cf10CfObjectPcFv(cf::CfObjectPc* self) {
    // Arts count from the param object (vtable 0xFC); the arts table index is
    // count+1, capped at 99 entries. Returns 1 when over the cap.
    void* bdat = lbl_eu_806640DC;
    u32 idx = reinterpret_cast<PCIf*>(self)->_v00FC() + 1;
    if (idx > 0x63) return 1;
    u32 sval = getBdatStringColumnValue(bdat,
        (const char*)lbl_eu_804FC5EC + 0x34, idx);
    return sval - reinterpret_cast<PCIf*>(self)->_v01E8();
}

// Vtable caller supplies three args despite the arg-less Fv retail name.
// Adds arg1/arg2 to the two clamped counters (+0x1604/+0x1608), refreshes
// state via slots 0x290/0x35C/0x5EC, then notifies the effect target.
void CActorParam_UnkVirtualFunc88__Q22cf10CfObjectPcFv(
    cf::CfObjectPc* self, int arg1, int arg2, int arg3) {
    CfObjectPcSubFields* f = (CfObjectPcSubFields*)self;
    // Declared before `arts` so MWCC colors arts into the higher callee-saved
    // reg (retail: arts=r30, acted=r29).
    int acted = 0;
    CfObjectPcArtsData* arts =
        (CfObjectPcArtsData*)func_8009EC9C(f->field_0x3F28);
    // Clamp cap 0x5F5E0FF; keep the subtraction inline at each use so MWCC
    // recomputes it (lis/subi) instead of caching it in a register.
    u32 v1604 = f->field_0x1604 + (u32)arg1;
    f->field_0x1604 = v1604;
    if (v1604 > 0x05F60000u - 0x1F01u) {
        f->field_0x1604 = 0x05F60000u - 0x1F01u;
    }
    u32 v1608 = f->field_0x1608 + (u32)arg2;
    f->field_0x1608 = v1608;
    if (v1608 > 0x05F60000u - 0x1F01u) {
        f->field_0x1608 = 0x05F60000u - 0x1F01u;
    }
    Obj89cField* obj = (Obj89cField*)reinterpret_cast<PCIf*>(self)->_v0290();
    func_802617B8((u8*)obj, obj->field_0x89C, arg3);
    // Drain the action queue through slot 0x35C.
    while (reinterpret_cast<PCIf*>(self)->_v035C() != 0) {
        acted = 1;
        func_800A282C((u8*)arts, 1);
    }
    func_8010CE50(f->field_0x3F10, arg1, arg2, arg3);
    if (arg3 != 0 && func_800A2AF0((u8*)arts) != 0) {
        func_802761E0(f->field_0x3F10);
    }
    if (acted != 0) {
        func_80276148(f->field_0x3F10,
            reinterpret_cast<PCIf*>(self)->_v05EC());
        BattleMgrRangeView* bm =
            (BattleMgrRangeView*)getInstance__Q22cf14CBattleManagerFv();
        // Goto form mirrors retail: flag=0, two early-out compares
        // (second commuted), flag=1, then shared check label. Commuting
        // blocks MWCC's unsigned range-check fusion (MWCC_CASES
        // func_801575B0).
        int inBattle = 0;
        if (bm->field_0x1AA < 1) goto bmCheck;
        if (0x18 < bm->field_0x1AA) goto bmCheck;
        inBattle = 1;
    bmCheck:
        if (inBattle != 0) goto actedDone;
        if (bm->field_0x20C8 != 0) goto actedDone;
        void* state = ((PcSub4Fake*)self)->_q030();
        if (func_80174C98(self, (int*)&state, 6) ||
            (((state = ((PcSub4Fake*)self)->_q030()),
                func_80174C98(self, (int*)&state, 9)))) {
            func_800BE12C((u8*)self + 0x3E9C, 0x1B, 0, 6, 1);
        }
    actedDone:;
    }
    func_800A13C4((u8*)arts, 0);
    func_801A891C((u8*)self, 0);
}

// Global definition under the retail mangled name: the base CActorParam
// declares this virtual returning void, so an int-returning member override
// would not compile.
int CActorParam_UnkVirtualFunc178__Q22cf10CfObjectPcFv(cf::CfObjectPc* self) {
    CfObjectPcSubFields* f = (CfObjectPcSubFields*)self;
    int spent = reinterpret_cast<PCIf*>(self)->_v01EC();
    if (spent <= 0) {
        CfObjectPcArtsData* data =
            (CfObjectPcArtsData*)func_8009EC9C(f->field_0x3F28);
        func_800A11A4((u8*)data, 1);
        reinterpret_cast<PCIf*>(self)->_v0204(((ArtsEntryFake*)data)->fill());
        u32 gained = reinterpret_cast<PCIf*>(self)->_v01E8();
        u32 cur = f->field_0x1600;
        u32 total = (u32)spent + gained;
        // Best-of-N (19 formulations tried: inline chains x4 orders,
        // signed/mixed-sign casts, compound assigns, guard-folded store,
        // volatile pins, decl/textual reorders). MWCC collapses every
        // variant to "add r4, r4, r0"; retail has "add r4, r0, r4".
        total = cur + total;
        f->field_0x1600 = total;
        if (total > 0x05F60000u - 0x1F01u) {
            f->field_0x1600 = 0x05F60000u - 0x1F01u;
        }
        f->field_0x1604 = -spent;
        return 1;
    }
    return 0;
}

// Retail func_800C0DD4: per-frame gauge/damage refresh gate.
// 1) global gate word, 2) resource flags bits 11/23 drive cancel-style
// dispatches, 3) battle-manager state gates, 4) scans bdat rows for the
// entry matching (level byte, key halfword), then applies the scaled
// damage to the gauge (+0x45C0) with sound feedback. Returns 1 when the
// matched row was applied.
// C linkage: retail's symbol is the unmangled func_800C0DD4.
extern "C" int func_800C0DD4(cf::CfObjectPc* self, int flag) {
    if ((lbl_eu_80663E24 & 0xAFA40000u) != 0) {
        return 0;
    }
    if (((CfObjectPcSubFields*)self)->mPtr3F60 != NULL) {
        if (((CfObjectPcSubFields*)self)->mPtr3F60->field_0x4EC & 0x100000) {
            // slot 0x210 false -> primary slot 0x608 cancel call
            if (!((PcWrapA*)self)->mSub._p210(0x25)) {
                reinterpret_cast<PCIf*>(self)->vf0608(0x25, 0, -1, 0, 0);
            }
        } else {
            if (((PcWrapB*)self)->mSub._p210(0x25)) {
                ((PcWrapA*)self)->mSub._p20C(0x25);
            }
        }
        if ((((CfObjectPcSubFields*)self)->mPtr3F60->field_0x4EC & 0x100) == 0 ||
            reinterpret_cast<PCIf*>(self)->_v02BC() != 0) {
            goto resetGauge;
        }
        BattleMgrRangeView* bm =
            (BattleMgrRangeView*)getInstance__Q22cf14CBattleManagerFv();
        if (bm->field_0x1AA >= 1 && bm->field_0x1AA <= 0x18) {
            return 0;
        }
        if (func_801BA2C8((u8*)bm + 0x216C)) {
            return 0;
        }
        if (((BattleMgrRangeView*)getInstance__Q22cf14CBattleManagerFv())
                ->field_0x20C8 != 0) {
            return 0;
        }
        u32 fourB0 =
            ((CfObjectPcSubFields*)self)->mPtr3F60->field_0x4B0;
        // key lookup mirrors CfObjectEne.cpp: area word indexes a table at
        // lbl_eu_80663E44 with a x100 byte stride; result adds the offset back.
        u32 modeOff = (u32)lbl_eu_80663E42 * 100;
        int channel = (int)*(u16*)((u8*)&lbl_eu_80663E44 + modeOff) + (int)modeOff;
        func_8003AA34();
        void* mgr = lbl_eu_806640D4;
        int i = func_8003B41C(mgr);
        int end = i + func_8003B1EC(mgr);
        const char* names = (const char*)lbl_eu_804FC5EC;
        for (; i < end; i++) {
            u32 vA = getBdatStringColumnValue(mgr, names + 0x3E, i);
            if ((int)(u8)vA != (int)fourB0) continue;
            u32 vB = getBdatStringColumnValue(mgr, names + 0x45, i);
            if ((int)(u16)vB != channel) continue;
            u32 vC = getBdatStringColumnValue(mgr, names + 0x49, i);
            float sv = ((PcWrapB*)self)->mSub._p08C();
            float newGauge = func_80496288(lbl_eu_80663E14) * sv +
                ((CfObjectPcSubFields*)self)->field_0x45C0;
            ((CfObjectPcSubFields*)self)->field_0x45C0 = newGauge;
            double thr = (double)(int)((vC & 0xFF) * 30);
            if (newGauge < thr) return 1;
            u32 vD = getBdatStringColumnValue(mgr, names + 0x52, i);
            double base = (double)(int)(vD & 0xFF) - lbl_eu_80666B40;
            float dmg = lbl_eu_80666B38 *
                (base * reinterpret_cast<PCIf*>(self)->_v012C());
            if (func_80148778((u8*)self + 8, 0xE9)) {
                Res10View* e = (Res10View*)func_80149154((u8*)self + 8, 0xE9);
                double dT = (double)(int)e->field_0x10;
                dmg = dmg * (lbl_eu_80666B24 -
                    ((dT - lbl_eu_80666B30) / lbl_eu_80666B28));
            }
            float dv = (float)(int)dmg;
            if (dv == lbl_eu_80666B14) dv = lbl_eu_80666B24;
            u8* moveSub = (u8*)self;
            if (self != NULL) {
                moveSub += 0x3E9C;
            }
            if (moveSub != (u8*)getPlayer__Q22cf13CfGameManagerFi(0)) {
                float hp = reinterpret_cast<PCIf*>(self)->_v0128();
                if (hp <= dv) {
                    dv = reinterpret_cast<PCIf*>(self)->_v0128() -
                        lbl_eu_80666B24;
                }
            }
            if (dv != lbl_eu_80666B14) {
                reinterpret_cast<PCIf*>(self)->_v011C(-dv);
                cf::CfSoundMan::playActorSound(0, 0x1AE, 0, 0, lbl_eu_80666B24);
            }
            ((CfObjectPcSubFields*)self)->field_0x45C0 = lbl_eu_80666B14;
            return 1;
        }
    }
resetGauge:
    ((CfObjectPcSubFields*)self)->field_0x45C0 = lbl_eu_80666B14;
    return 0;
}

// this-adjust thunk: retarget to the +0x3E9C subobject and tail-branch into
// func_800C00C0.
void CObjectParam_UnkVirtualFunc4__Q22cf10CfObjectPcFv(void* self) { ((void(*)(void*))func_800C00C0__Q22cf10CfObjectPcFv)((char*)self - 0x3e9c); }

void CfObject_UnkVirtualFunc3__Q22cf10CfObjectPcFv(void* self) { ((void(*)(void*))enablePcFlag__Q22cf10CfObjectPcFv)((char*)self - 0x3e9c); }

// Tail-calls into initialize on the -0x3E9C adjusted this.
void cf::CfObjectPc::CfObject_UnkVirtualFunc2() { ((cf::CfObjectPc*)((char*)this - 0x3e9c))->cf::CfObjectPc::initialize(); }

void CfObjectMove_UnkVirtualFunc16__Q22cf10CfObjectPcFv(void* self) { ((void(*)(void*))handleMoveState__Q22cf10CfObjectPcFv)((char*)self - 0x3e9c); }


void CfObject_UnkVirtualFunc4__Q22cf10CfObjectPcFv(void* self) { ((void(*)(void*))func_800C0524__Q22cf10CfObjectPcFv)((char*)self - 0x3e9c); }

void finalizePcCleanup__Q22cf10CfObjectPcFv(void* self) { ((void(*)(void*))__dt__Q22cf10CfObjectPcFv)((char*)self - 0x3e9c); }

// Returns the object only when its flags word at +0x64 has bit 0x4000 set
// (the "valid" mark); otherwise returns NULL. The C++ name mangles to the
// retail symbol getValidObject__FPv.
UNKTYPE* getValidObject(UNKTYPE* r3) {
    if (r3 != NULL) {
        if (((FlagsObj800C1228*)r3)->field_0x64 & 0x4000) {
            return r3;
        }
    }
    return NULL;
}

int cf::CfObjectPoint::validatePointState() {
    CfObject_UnkVirtualFunc8();
    return 1;
}

extern "C" void triggerActionRefreshA__Q22cf10CfObjectPcFv(cf::CfObjectPc* self) { reinterpret_cast<PCIf*>(self)->vf05F4(); }

extern "C" void triggerActionRefreshB__Q22cf10CfObjectPcFv(cf::CfObjectPc* self) { reinterpret_cast<PCIf*>(self)->vf05F4(); }
