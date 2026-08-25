#ifndef CF_OBJECT_ENE_HPP
#define CF_OBJECT_ENE_HPP

#include "kyoshin/cf/object/CfObjectActor.hpp"

// CfResReloadImpl is only referenced as a pointer member (field_0x3f4c) below,
// so a forward declaration suffices. Including CfResReloadImpl.hpp here would
// duplicate the extern "C" func_800BB618 declaration (CfObjectMove.hpp already
// declares it with a different parameter type) and break every TU that pulls
// both headers (MWCC: illegal function overloading).
namespace cf {
class CfResReloadImpl;
struct CfResReloadParent;
}

namespace cf {

class CfObjectEne : public CfObjectActor {
public:
    virtual ~CfObjectEne();
    virtual void func_800ADB2C();
    virtual void func_800ADBD4();
    virtual void func_800ADDA8();
    virtual void func_800AEC68();
    void CActorParam_UnkVirtualFunc166();
    void CActorParam_UnkVirtualFunc167();

private:
    // field_0x8 is a secondary vtable pointer inherited from CfObjectActor
    // Multiple inheritance vtables:
    // 0x0: primary vtable
    // 0x8: secondary vtable
    // ...
    // 0x3380: another interface vtable
    // 0x3e9c: another interface vtable

    // member variables:
    u16 field_0x45c0;
    u16 field_0x45c2;
    u16 field_0x45c4;
    u16 field_0x45c6;
    u16 field_0x45c8;
    u16 field_0x45ca;
    f32 field_0x45cc;
    // ...
    CfResReloadImpl* field_0x3f4c;
};

// Absolute-offset view of the u16 BDAT row id at self+0x456C (inside the
// CfObjectMove subobject region). func_800AF7E4 shifts it right 4 to key the
// getBdatStringColumnValue lookup; sibling views CfPartsFlagView /
// CfPartsChgObj read the same word.
struct CfObjectEne456CView {
    u8 _pad[0x456C];
    u16 field_0x456C; // 0x456C (func_800AF7E4 row id, >> 4)
};

// Two zeroed words of the splitter-renamed dtor __dt__800ADA08's object
// (stores 0x208 then 0x204, then delete when the flag > 0).
struct CfDtor800ADA08View {
    u8 _pad[0x204];
    u32 field_0x204; // 0x204
    u32 field_0x208; // 0x208
};

// Block view anchored at CAIAction+4 (and at CfObjectActor+0x3380+4 in the
// CfObjectActor dtor's inlined copy). The CAIAction dtor null-checks the
// block address AND the +8 address (MWCC folds both into addic.-beq guards;
// see MWCC_CASES CacheList pattern), then zeroes the +0x20C/+0x210
// words (= CAIAction unk210/unk214, store order 0x210 first).
struct CfCAIActionBlockView {
    u8 _pad[8];
    void* field_8;   // +8 (address-null-checked second)
    u8 _padC[0x20C - 0xC];
    u32 field_20C;   // +0x20C (zeroed second)
    u32 field_210;   // +0x210 (zeroed first)
};

// CfObjectActor +0x3380 = the CAIAction subobject base. The header's
// multiple-inheritance layout is +0x80 off retail (CfObjectMove lands at
// +0x3F1C instead of +0x3E9C), so the dtor reaches this block through the
// raw view to pin the retail offset.
struct CfObjectCAISubView {
    u8 _pad[4];
    u32 field_4;     // +4 (block anchor; address-null-checked first)
};

// Absolute-offset views of CActorParam_UnkVirtualFunc88's fields: the +0x15E0
// sub-object pointer on self and the +0x89C word on that sub-object.
struct CfActorParam15E0View {
    u8 _pad[0x15E0];
    u8* field_0x15E0; // 0x15E0 (sub-object pointer; also read by UnkVirtualFunc128)
};
struct CfActorObj89CView {
    u8 _pad[0x89C];
    u32 field_0x89C; // 0x89C
};

// Vtable proxy for the CfObjectActor/CActorParam primary vtable (offset
// 0x00), slots +0x1DC (no-arg) and +0x1FC (r4 arg), dispatched by
// CActorParam_UnkVirtualFunc88. Dummy slots pin the offsets (the first
// declared virtual lands at vtable+0x08).
class CfActorParamVt1DC {
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
    virtual void _d168(); virtual void _d16C(); virtual void _d170(); virtual void _d174();
    virtual void _d178(); virtual void _d17C(); virtual void _d180(); virtual void _d184();
    virtual void _d188(); virtual void _d18C(); virtual void _d190(); virtual void _d194();
    virtual void _d198(); virtual void _d19C(); virtual void _d1A0(); virtual void _d1A4();
    virtual void _d1A8(); virtual void _d1AC(); virtual void _d1B0(); virtual void _d1B4();
    virtual void _d1B8(); virtual void _d1BC(); virtual void _d1C0(); virtual void _d1C4();
    virtual void _d1C8(); virtual void _d1CC(); virtual void _d1D0(); virtual void _d1D4();
    virtual void _d1D8();
    virtual void vf1DC();          // vtable +0x1DC (no-arg dispatch)
    virtual void _d1E0(); virtual void _d1E4(); virtual void _d1E8(); virtual void _d1EC();
    virtual void _d1F0(); virtual void _d1F4(); virtual void _d1F8();
    virtual void vf1FC(void* arg); // vtable +0x1FC (dispatch with r4 arg)
};

// Absolute-offset views of CActorParam byte fields used by the CfObjectEne
// CActorParam getter/setter stubs. The CActorParam member block is +8 shifted
// by the base-class layout (see CActorParamGaugeView in CActorParam.hpp), so
// raw views pin the retail offsets.
struct CfObjectEne1628View {
    u8 _pad[0x1628];
    u8 field_0x1628; // 0x1628 CActorParam_UnkVirtualFunc114 setter
};

struct CfObjectEne183AView {
    u8 _pad[0x183A];
    u8 field_0x183A; // 0x183A CActorParam_UnkVirtualFunc109
    u8 field_0x183B; // 0x183B CActorParam_UnkVirtualFunc110
    u8 field_0x183C; // 0x183C CActorParam_UnkVirtualFunc111
};

// Absolute-offset view of the CActorParam byte at 0x15F4 (the CActorParam
// member block is +8 shifted by the base-class layout, so the declared
// unk15F4[4] member lands at 0x15FC; raw view pins the retail offset). Used
// by CActorParam_UnkVirtualFunc15.
struct CfObjectEne15F4View {
    u8 _pad[0x15F4];
    u8 field_0x15F4; // 0x15F4 CActorParam_UnkVirtualFunc15
};

// 0x1520-byte block copied by CBattleState_UnkVirtualFunc24: retail emits a
// counted 8-byte copy loop (li 0x2a4; mtctr; lwzu/stwu; bdnz) = full struct
// assignment. The destination member is CBattleState::unk8 (offset 0x8); the
// retail `addi r5, r3, 4` + 4/8 displacements are the off-4 canonical form.
struct CBattleStateUnk1520Block {
    u8 data[0x1520];
};

// 0xC0-byte block copied by CActorParam_UnkVirtualFunc146: retail copies
// [src+0x0, +0xC0) -> [this+0x1928, +0x19E8), i.e. the 8-entry
// CActorParam_UnkStruct5 array (stride 0x18; 8 * 0x18 = 0xC0). The retail
// `addi r5, r3, 0x1924` + 4/8 displacements are the off-4 canonical form.
struct CActorParamUnk1928Block {
    u8 data[0xC0];
};

// Absolute-offset views of the CActorParam float gauge fields used by
// CActorParam_UnkVirtualFunc31/42 (member block is +8 shifted by the
// base-class layout; see CActorParamGaugeView in CActorParam.hpp).
struct CActorParam1660View {
    u8 _pad[0x1660];
    f32 field_0x1660; // 0x1660 gauge current (Func30 setter / Func32 getter / Func31 add+clamp)
};
struct CActorParam17ECView {
    u8 _pad[0x17EC];
    f32 field_0x17EC; // 0x17EC gauge current (Func41 setter / Func43 getter / Func42 add+clamp)
    f32 field_0x17F0; // 0x17F0
    f32 field_0x17F4; // 0x17F4
    f32 field_0x17F8; // 0x17F8 gauge max (Func44 getter / Func42 clamp top)
    f32 field_0x17FC; // 0x17FC
};

    // --- CfObjectEne ctor/dtor raw views ---
    // The header's C++ MI layout is +0x80 off retail (see the CfObjectActor
    // dtor note in CfObjectEne.cpp), so the ctor/dtor reach the subobject
    // vtable slots, the reload child pointer and the tail fields through raw
    // views pinning the retail offsets.

    // Vtable-pointer slots rewritten by the CfObjectEne ctor/dtor (retail
    // stores lbl_eu_80528A18 + 0x0/0xC/0x36C/0x37C at these positions).
    struct CfEneVtables {
        u32 vt0;             // +0x00 primary vtable (lbl_eu_80528A18)
        u8 _pad4[0x8 - 0x4];
        u32 vt8;             // +0x08 secondary vtable (+0xC)
        u8 _padC[0x3380 - 0xC];
        u32 vt3380;          // +0x3380 CAIAction subobject vtable (+0x36C)
        u8 _pad3384[0x3E9C - 0x3384];
        u32 vt3E9C;          // +0x3E9C CfObjectMove subobject vtable (+0x37C)
    };

    // Cast-only fake SI for the +0x3E9C CfObjectMove subobject vtable
    // (CfResReloadVtIf-style shift base). The dtor dispatches slot +0x68 and
    // func_800ADB2C slot +0x158 through a REAL virtual call on the holder's
    // subobject: MWCC then folds the vptr load (lwz r12, 0x3E9C(rX)) and
    // re-materializes the adjusted this (addi r3, rX, 0x3E9C) per call - the
    // retail shape (MWCC_CASES CfObjectActor_UnkVirtualFunc3/4). A manual
    // function-pointer dispatch CSEs the subobject address into a
    // callee-saved register and grows the frame instead. RTTI 8-byte vtable
    // header: slot 0x68 = (0x68-8)/4 = 24 dummies, slot 0x158 = 84 dummies.
    struct CfEneSubIf {
        virtual void _d008(); virtual void _d00C(); virtual void _d010(); virtual void _d014();
        virtual void _d018(); virtual void _d01C(); virtual void _d020(); virtual void _d024();
        virtual void _d028(); virtual void _d02C(); virtual void _d030(); virtual void _d034();
        virtual void _d038(); virtual void _d03C(); virtual void _d040(); virtual void _d044();
        virtual void _d048(); virtual void _d04C(); virtual void _d050(); virtual void _d054();
        virtual void _d058(); virtual void _d05C(); virtual void _d060(); virtual void _d064();
        virtual void vf68();             // vtable +0x68 (dtor dispatch)
        virtual void _d06C(); virtual void _d070(); virtual void _d074(); virtual void _d078();
        virtual void _d07C(); virtual void _d080(); virtual void _d084(); virtual void _d088();
        virtual void _d08C(); virtual void _d090(); virtual void _d094(); virtual void _d098();
        virtual void _d09C(); virtual void _d0A0(); virtual void _d0A4(); virtual void _d0A8();
        virtual void _d0AC(); virtual void _d0B0(); virtual void _d0B4(); virtual void _d0B8();
        virtual void _d0BC(); virtual void _d0C0(); virtual void _d0C4(); virtual void _d0C8();
        virtual void _d0CC(); virtual void _d0D0(); virtual void _d0D4(); virtual void _d0D8();
        virtual void _d0DC(); virtual void _d0E0(); virtual void _d0E4(); virtual void _d0E8();
        virtual void _d0EC(); virtual void _d0F0(); virtual void _d0F4(); virtual void _d0F8();
        virtual void _d0FC(); virtual void _d100(); virtual void _d104(); virtual void _d108();
        virtual void _d10C(); virtual void _d110(); virtual void _d114(); virtual void _d118();
        virtual void _d11C(); virtual void _d120(); virtual void _d124(); virtual void _d128();
        virtual void _d12C(); virtual void _d130(); virtual void _d134(); virtual void _d138();
        virtual void _d13C(); virtual void _d140(); virtual void _d144(); virtual void _d148();
        virtual void _d14C(); virtual void _d150(); virtual void _d154();
        virtual void vf158(int arg);     // vtable +0x158 (func_800ADB2C dispatch)
    };

    // Fake derived: CfEneSubIf is an INHERITED base at +0x3E9C, so a virtual
    // call through the derived this reproduces the retail folded vptr load
    // (lwz r12, 0x3E9C(rX)) plus the recomputed adjusted-this
    // (addi r3, rX, 0x3E9C) per call (MWCC_CASES
    // CfObjectActor_UnkVirtualFunc3/4).
    struct CfEneSubPad {
        u8 _pad[0x3E9C];
    };
    struct CfEneSubFake : CfEneSubPad, CfEneSubIf {};

    // CfResReloadImpl child pointer stored by the ctor at +0x3F4C.
    struct CfEneReloadSlot {
        u8 _pad[0x3F4C];
        void* field_0x3F4C; // 0x3F4C
    };

    // Tail fields zeroed by the ctor (u16s at 0x45C0/0x45C6/0x45C8/0x45CA,
    // f32 at 0x45CC; 0x45C2/0x45C4 are left untouched by retail).
    struct CfEneTailView {
        u8 _pad[0x45C0];
        u16 field_0x45C0;
        u16 field_0x45C2;
        u16 field_0x45C4;
        u16 field_0x45C6;
        u16 field_0x45C8;
        u16 field_0x45CA;
        f32 field_0x45CC;
    };

    // +0x44A8 sub-object touched at the end of func_800ADB2C (the u16 at
    // +0xB2 receives 0xC8).
    struct CfEneReloadArea {
        u8 _pad[0xB2];
        u16 field_0xB2; // 0xB2
    };

    // --- func_800AEC68 / func_800ADBD4 / func_800AF870 raw views ---

    // Name/state fields used by func_800AEC68: the bdat lookup name at
    // +0x3F14 (getFP arg) and the u16 row id at +0x3F28.
    struct CfEneLookupView {
        u8 _pad[0x3F14];
        char field_0x3F14[0x28 - 0x14]; // 0x3F14 (getFP name)
        u16 field_0x3F28;               // 0x3F28 (bdat row id)
    };

    // Base of the +0x3380 CAIAction subobject (func_8014B7B0 /
    // func_8015396C args in func_800AEC68). The two calls go through
    // DIFFERENT view types so MWCC re-materializes the addi r3, rX, 0x3380
    // per call (a CSE'd address would live in a callee-saved register and
    // grow the frame - same trick as the func_800ADB2C dispatch sites).
    struct CfEneAI3380View {
        u8 _pad[0x3380];
        u8 field_0x3380; // 0x3380 (subobject base)
    };
    struct CfEneAI3380View2 {
        u8 _pad[0x3380];
        u8 field_0x3380; // 0x3380 (subobject base)
    };

    // Distinct absolute-offset views of the +0x3E9C CfObjectMove subobject
    // base, one per plain (non-virtual) call site in func_800ADB2C so MWCC
    // re-materializes `addi r3, r31, 0x3e9c` per call instead of CSE-ing the
    // address into a callee-saved register (which would grow the frame).
    struct CfEneMoveBaseA {
        u8 _pad[0x3E9C];
        u8 base; // 0x3E9C
    };
    struct CfEneMoveBaseB {
        u8 _pad[0x3E9C];
        u8 base; // 0x3E9C
    };
    struct CfEneMoveBaseC {
        u8 _pad[0x3E9C];
        u8 base; // 0x3E9C
    };

    // Base of the +0x8 CBattleState subobject (func_80148778 arg and the
    // +0x14/+0x20 vtable dispatches in func_800ADBD4).
    struct CfEneB8View {
        u8 _pad[0x8];
        u8 field_0x8; // 0x8 (subobject base)
    };

    // Fake MI for the +0x8 subobject dispatch (slot +0x14 m14 and +0x20
    // m20): a real virtual call through the derived this folds the vptr load
    // into `lwz r12, 8(rX)` and re-materializes the adjusted this (addi r3,
    // rX, 8) - the retail shape (same scheme as CfEneSubFake).
    struct CfEneB8Pad {
        u8 _pad[0x8];
    };
    class CfBattleVt14If {
    public:
        virtual void _d008(); virtual void _d00C(); virtual void _d010();
        virtual void m14(int val); // vtable +0x14
    };
    struct CfEneB8V14 : CfEneB8Pad, CfBattleVt14If {};
    struct CfEneB8V20 : CfEneB8Pad, cf::CfBattleVt20 {};

    // Vtable slot +0x14 dispatch on the +0x8 subobject (retail folds the
    // vptr load to `lwz r12, 0x8(rX)`; slot called with a 0x35 id arg).
    typedef void (*CfEneB8Vt14Fn)(u8* self, int val);
    struct CfEneB8Vt14Table {
        u32 _pad[0x14 / 4]; // 0x00-0x13
        CfEneB8Vt14Fn fn14; // 0x14
    };

    // Flags/pointer fields touched by func_800ADBD4: the +0x3F34 target
    // pointer and the +0x7A4 flag word behind it.
    struct CfEneField3F34 {
        u8 _pad[0x3F34];
        u8* field_0x3F34; // 0x3F34
    };
    struct CfEneField7A4 {
        u8 _pad[0x7A4];
        u32 field_0x7A4; // 0x7A4
    };

    // Object behind CfObjectActor's +0x3F60 pointer (func_800AF870): the
    // model-id word at +0x4B0 and the +0x4EC flag word (bit 8 gates).
    struct CfEneMoveTgtView {
        u8 _pad[0x4B0];
        s32 field_0x4B0;           // 0x4B0 model id (signed compare)
        u8 _pad4B4[0x4EC - 0x4B4]; // 0x4B4
        u32 field_0x4EC;           // 0x4EC flags (bit 8)
    };

    // CBattleManager fields read by func_800AF870: party-count byte at
    // +0x1AA, mode s16 at +0x20C8 and the +0x216C func_801BA2C8 buffer.
    struct CfEneBmView {
        u8 _pad[0x1AA];
        u8 field_0x1AA;               // 0x1AA battle/party count
        u8 _pad1AB[0x20C8 - 0x1AB];   // 0x1AB
        s16 field_0x20C8;             // 0x20C8 mode
        u8 _pad20CA[0x216C - 0x20CA]; // 0x20CA
        u8 field_0x216C;              // 0x216C (func_801BA2C8 arg base)
    };

    // u16/u8/s32 -> f32 conversion slots pinned to the named sdata2 magic
    // doubles (same convention as CfActorF64Conv in CfObjectActor.hpp).
    union CfEneF64Conv {
        u32 w[2];
        double d;
    };

    // Primary-vtable call proxy for func_800AEC68: slots +0xD4 (float arg,
    // CActorParam_UnkVirtualFunc16), +0xF0 (returns float) and +0x288
    // (no-arg, x6 loop) on the CfObjectEne primary vtable. Dummy slots pin
    // the offsets (RTTI 8-byte vtable header; Nth declared virtual at
    // (N+1)*4).
    class CfEneVtD4 {
    public:
        virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
        virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
        virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
        virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
        virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
        virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
        virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
        virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
        virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
        virtual void _v098(); virtual void _v09C(); virtual void _v0A0(); virtual void _v0A4();
        virtual void _v0A8(); virtual void _v0AC(); virtual void _v0B0(); virtual void _v0B4();
        virtual void _v0B8(); virtual void _v0BC(); virtual void _v0C0(); virtual void _v0C4();
        virtual void _v0C8(); virtual void _v0CC(); virtual void _v0D0();
        virtual void mD4(float val); // vtable +0xD4 (float arg)
        virtual void _v0D8(); virtual void _v0DC(); virtual void _v0E0(); virtual void _v0E4();
        virtual void _v0E8(); virtual void _v0EC();
        virtual float mF0();         // vtable +0xF0 (returns float)
        virtual void _v0F4(); virtual void _v0F8(); virtual void _v0FC();
        virtual void _v100(); virtual void _v104(); virtual void _v108(); virtual void _v10C();
        virtual void _v110(); virtual void _v114(); virtual void _v118(); virtual void _v11C();
        virtual void _v120(); virtual void _v124(); virtual void _v128(); virtual void _v12C();
        virtual void _v130(); virtual void _v134(); virtual void _v138(); virtual void _v13C();
        virtual void _v140(); virtual void _v144(); virtual void _v148(); virtual void _v14C();
        virtual void _v150(); virtual void _v154(); virtual void _v158(); virtual void _v15C();
        virtual void _v160(); virtual void _v164(); virtual void _v168(); virtual void _v16C();
        virtual void _v170(); virtual void _v174(); virtual void _v178(); virtual void _v17C();
        virtual void _v180(); virtual void _v184(); virtual void _v188(); virtual void _v18C();
        virtual void _v190(); virtual void _v194(); virtual void _v198(); virtual void _v19C();
        virtual void _v1A0(); virtual void _v1A4(); virtual void _v1A8(); virtual void _v1AC();
        virtual void _v1B0(); virtual void _v1B4(); virtual void _v1B8(); virtual void _v1BC();
        virtual void _v1C0(); virtual void _v1C4(); virtual void _v1C8(); virtual void _v1CC();
        virtual void _v1D0(); virtual void _v1D4(); virtual void _v1D8(); virtual void _v1DC();
        virtual void _v1E0(); virtual void _v1E4(); virtual void _v1E8(); virtual void _v1EC();
        virtual void _v1F0(); virtual void _v1F4(); virtual void _v1F8(); virtual void _v1FC();
        virtual void _v200(); virtual void _v204(); virtual void _v208(); virtual void _v20C();
        virtual void _v210(); virtual void _v214(); virtual void _v218(); virtual void _v21C();
        virtual void _v220(); virtual void _v224(); virtual void _v228(); virtual void _v22C();
        virtual void _v230(); virtual void _v234(); virtual void _v238(); virtual void _v23C();
        virtual void _v240(); virtual void _v244(); virtual void _v248(); virtual void _v24C();
        virtual void _v250(); virtual void _v254(); virtual void _v258(); virtual void _v25C();
        virtual void _v260(); virtual void _v264(); virtual void _v268(); virtual void _v26C();
        virtual void _v270(); virtual void _v274(); virtual void _v278(); virtual void _v27C();
        virtual void _v280(); virtual void _v284();
        virtual void m288(); // vtable +0x288
    };

    // Primary-vtable +0x2BC dispatch (func_800AF870 guard; returns int).
    class CfEneVt2BC {
    public:
        virtual void _b008(); virtual void _b00C(); virtual void _b010(); virtual void _b014();
        virtual void _b018(); virtual void _b01C(); virtual void _b020(); virtual void _b024();
        virtual void _b028(); virtual void _b02C(); virtual void _b030(); virtual void _b034();
        virtual void _b038(); virtual void _b03C(); virtual void _b040(); virtual void _b044();
        virtual void _b048(); virtual void _b04C(); virtual void _b050(); virtual void _b054();
        virtual void _b058(); virtual void _b05C(); virtual void _b060(); virtual void _b064();
        virtual void _b068(); virtual void _b06C(); virtual void _b070(); virtual void _b074();
        virtual void _b078(); virtual void _b07C(); virtual void _b080(); virtual void _b084();
        virtual void _b088(); virtual void _b08C(); virtual void _b090(); virtual void _b094();
        virtual void _b098(); virtual void _b09C(); virtual void _b0A0(); virtual void _b0A4();
        virtual void _b0A8(); virtual void _b0AC(); virtual void _b0B0(); virtual void _b0B4();
        virtual void _b0B8(); virtual void _b0BC(); virtual void _b0C0(); virtual void _b0C4();
        virtual void _b0C8(); virtual void _b0CC(); virtual void _b0D0(); virtual void _b0D4();
        virtual void _b0D8(); virtual void _b0DC(); virtual void _b0E0(); virtual void _b0E4();
        virtual void _b0E8(); virtual void _b0EC(); virtual void _b0F0(); virtual void _b0F4();
        virtual void _b0F8(); virtual void _b0FC(); virtual void _b100(); virtual void _b104();
        virtual void _b108(); virtual void _b10C(); virtual void _b110(); virtual void _b114();
        virtual void _b118(); virtual void _b11C(); virtual void _b120(); virtual void _b124();
        virtual void _b128(); virtual void _b12C(); virtual void _b130(); virtual void _b134();
        virtual void _b138(); virtual void _b13C(); virtual void _b140(); virtual void _b144();
        virtual void _b148(); virtual void _b14C(); virtual void _b150(); virtual void _b154();
        virtual void _b158(); virtual void _b15C(); virtual void _b160(); virtual void _b164();
        virtual void _b168(); virtual void _b16C(); virtual void _b170(); virtual void _b174();
        virtual void _b178(); virtual void _b17C(); virtual void _b180(); virtual void _b184();
        virtual void _b188(); virtual void _b18C(); virtual void _b190(); virtual void _b194();
        virtual void _b198(); virtual void _b19C(); virtual void _b1A0(); virtual void _b1A4();
        virtual void _b1A8(); virtual void _b1AC(); virtual void _b1B0(); virtual void _b1B4();
        virtual void _b1B8(); virtual void _b1BC(); virtual void _b1C0(); virtual void _b1C4();
        virtual void _b1C8(); virtual void _b1CC(); virtual void _b1D0(); virtual void _b1D4();
        virtual void _b1D8(); virtual void _b1DC(); virtual void _b1E0(); virtual void _b1E4();
        virtual void _b1E8(); virtual void _b1EC(); virtual void _b1F0(); virtual void _b1F4();
        virtual void _b1F8(); virtual void _b1FC(); virtual void _b200(); virtual void _b204();
        virtual void _b208(); virtual void _b20C(); virtual void _b210(); virtual void _b214();
        virtual void _b218(); virtual void _b21C(); virtual void _b220(); virtual void _b224();
        virtual void _b228(); virtual void _b22C(); virtual void _b230(); virtual void _b234();
        virtual void _b238(); virtual void _b23C(); virtual void _b240(); virtual void _b244();
        virtual void _b248(); virtual void _b24C(); virtual void _b250(); virtual void _b254();
        virtual void _b258(); virtual void _b25C(); virtual void _b260(); virtual void _b264();
        virtual void _b268(); virtual void _b26C(); virtual void _b270(); virtual void _b274();
        virtual void _b278(); virtual void _b27C(); virtual void _b280(); virtual void _b284();
        virtual void _b288(); virtual void _b28C(); virtual void _b290(); virtual void _b294();
        virtual void _b298(); virtual void _b29C(); virtual void _b2A0(); virtual void _b2A4();
        virtual void _b2A8(); virtual void _b2AC(); virtual void _b2B0(); virtual void _b2B4();
        virtual void _b2B8();
        virtual int m2BC(); // vtable +0x2BC
    };

    // Vtable proxy for the +0x3E9C CfObjectMove subobject slots +0x64/+0x80
    // (no-arg) and +0x8C (returns float), used by func_800ADBD4 /
    // func_800AF870. Dummy slots pin the offsets (same scheme as CfEneSubIf).
    class CfEneSubVt64If {
    public:
        virtual void _s008(); virtual void _s00C(); virtual void _s010(); virtual void _s014();
        virtual void _s018(); virtual void _s01C(); virtual void _s020(); virtual void _s024();
        virtual void _s028(); virtual void _s02C(); virtual void _s030(); virtual void _s034();
        virtual void _s038(); virtual void _s03C(); virtual void _s040(); virtual void _s044();
        virtual void _s048(); virtual void _s04C(); virtual void _s050(); virtual void _s054();
        virtual void _s058(); virtual void _s05C(); virtual void _s060();
        virtual void m64();  // vtable +0x64
        virtual void _s06C(); virtual void _s070(); virtual void _s074(); virtual void _s078();
        virtual void _s07C(); virtual void _s080();
        virtual void m80();  // vtable +0x80
        virtual void _s084(); virtual void _s088();
        virtual float m8C(); // vtable +0x8C (returns float)
    };

    // Fake derived for the +0x3E9C subobject (same scheme as CfEneSubFake).
    struct CfEneSubVt64 : CfEneSubPad, CfEneSubVt64If {};
    // Distinct derived types for the m8C/m80/m64 dispatch sites in
    // func_800ADBD4 so MWCC re-materializes the addi r3, rX, 0x3e9c (and
    // folds the vptr load) per call instead of CSE-ing the subobject base
    // into a callee-saved register (the known cross-call address CSE).
    struct CfEneSubVt64b : CfEneSubPad, CfEneSubVt64If {};
    struct CfEneSubVt64c : CfEneSubPad, CfEneSubVt64If {};

    // --- CActorParam_UnkVirtualFunc123/120 record structs ---
    // The 0x84 field block copied verbatim per record by both loaders
    // (strcpy 'name' + strlen stored at +0x20, then the typed fields; the
    // +0x84 vtable word is skipped). Stride 0x88 for the 6 attack records
    // at +0x2750, stride 0x8C for the 24 arts records at +0x1A20 (the arts
    // loader additionally copies the u32 at +0x88).
    struct CfEneAttackEntry {
        char name[0x20]; // 0x00 (strcpy/strlen target)
        u32 field_0x20;  // 0x20 strlen result
        u32 field_0x24;  // 0x24
        u16 field_0x28;  // 0x28
        u8  field_0x2A;  // 0x2A
        u8  field_0x2B;  // 0x2B
        f32 field_0x2C;  // 0x2C
        f32 field_0x30;  // 0x30
        s16 field_0x34;  // 0x34
        s16 field_0x36;  // 0x36
        u32 field_0x38;  // 0x38
        u16 field_0x3C;  // 0x3C
        u8  field_0x3E;  // 0x3E
        u16 field_0x40;  // 0x40
        u8  field_0x42;  // 0x42
        u8  field_0x43;  // 0x43
        u8  field_0x44;  // 0x44
        u16 field_0x46;  // 0x46
        u16 field_0x48;  // 0x48
        s16 field_0x4A;  // 0x4A
        s16 field_0x4C;  // 0x4C
        f32 field_0x50;  // 0x50
        f32 field_0x54;  // 0x54
        u16 field_0x58;  // 0x58
        u16 field_0x5A;  // 0x5A
        u16 field_0x5C;  // 0x5C
        u16 field_0x5E;  // 0x5E
        f32 field_0x60;  // 0x60
        u16 field_0x64;  // 0x64
        u8  field_0x66;  // 0x66
        u8  field_0x67;  // 0x67
        s16 field_0x68;  // 0x68
        s16 field_0x6A;  // 0x6A
        u32 field_0x6C;  // 0x6C
        u8  field_0x70;  // 0x70
        u16 field_0x72;  // 0x72
        u16 field_0x74;  // 0x74
        u8  field_0x76;  // 0x76
        u8  field_0x77;  // 0x77
        u32 field_0x78;  // 0x78
        f32 field_0x7C;  // 0x7C
        f32 field_0x80;  // 0x80
        u32 field_0x84;  // 0x84 (skipped by the loader copy)
    };

    // Arts record (stride 0x8C): the +0x88 word is copied by Func120.
    struct CfEneArtsEntry {
        char name[0x20]; // 0x00 (strcpy/strlen target)
        u32 field_0x20;  // 0x20 strlen result
        u32 field_0x24;  // 0x24
        u16 field_0x28;  // 0x28
        u8  field_0x2A;  // 0x2A
        u8  field_0x2B;  // 0x2B
        f32 field_0x2C;  // 0x2C
        f32 field_0x30;  // 0x30
        s16 field_0x34;  // 0x34
        s16 field_0x36;  // 0x36
        u32 field_0x38;  // 0x38
        u16 field_0x3C;  // 0x3C
        u8  field_0x3E;  // 0x3E
        u16 field_0x40;  // 0x40
        u8  field_0x42;  // 0x42
        u8  field_0x43;  // 0x43
        u8  field_0x44;  // 0x44
        u16 field_0x46;  // 0x46
        u16 field_0x48;  // 0x48
        s16 field_0x4A;  // 0x4A
        s16 field_0x4C;  // 0x4C
        f32 field_0x50;  // 0x50
        f32 field_0x54;  // 0x54
        u16 field_0x58;  // 0x58
        u16 field_0x5A;  // 0x5A
        u16 field_0x5C;  // 0x5C
        u16 field_0x5E;  // 0x5E
        f32 field_0x60;  // 0x60
        u16 field_0x64;  // 0x64
        u8  field_0x66;  // 0x66
        u8  field_0x67;  // 0x67
        s16 field_0x68;  // 0x68
        s16 field_0x6A;  // 0x6A
        u32 field_0x6C;  // 0x6C
        u8  field_0x70;  // 0x70
        u16 field_0x72;  // 0x72
        u16 field_0x74;  // 0x74
        u8  field_0x76;  // 0x76
        u8  field_0x77;  // 0x77
        u32 field_0x78;  // 0x78
        f32 field_0x7C;  // 0x7C
        f32 field_0x80;  // 0x80
        u32 field_0x84;  // 0x84 (skipped by the loader copy)
        u32 field_0x88;  // 0x88 (copied by Func120)
    };

    // 12-byte attack-set header copied as a POD block by Func123.
    struct CfEneAttackHdr {
        u32 h0; // +0x00 / 0x2740
        u32 h4; // +0x04 / 0x2744
        u32 h8; // +0x08 / 0x2748
    };

    // Func123 destination: the 12-byte header at +0x2740 (a u64 pair + u32,
    // reproducing the retail lwz,lwz,stw,stw reversed store pair) and the
    // 6-record attack-param array at +0x2750 (matches CAttackSet layout).
    struct CfEneAttackArea {
        u8 _pad[0x2740];
        CfEneAttackHdr header;        // 0x2740
        u8 _pad274C[0x2750 - 0x274C]; // 0x274C (set vtable region)
        CfEneAttackEntry records[6];  // 0x2750
    };
    // Func123 source: same 12-byte header, records at +0x10.
    struct CfEneAttackData {
        CfEneAttackHdr header;       // 0x00
        u8 _padC[0x10 - 0xC];        // 0x0C
        CfEneAttackEntry records[6]; // 0x10
    };
    // Volatile-view wrappers for the Func123 header copy (see cpp).
    struct CfEneAttackHdrBlock {
        CfEneAttackHdr header;
    };
    struct CfEneAttackSrcHdrBlock {
        CfEneAttackHdr header;
    };

    // Func120 destination: 0x34-byte arts-set header at +0x19E8 and the
    // 24-record arts-param array at +0x1A20 (matches CArtsSet layout).
    struct CfEneArtsHdr34 {
        u16 field_0;  // 0x0
        u16 field_2;  // 0x2
        u32 field_4;  // 0x4
        u32 field_8;  // 0x8
        u32 field_C;  // 0xC
        u32 field_10; // 0x10
        u32 field_14; // 0x14
        u32 field_18; // 0x18
        u32 field_1C; // 0x1C
        u32 field_20; // 0x20
        u32 field_24; // 0x24
        u32 field_28; // 0x28
        u32 field_2C; // 0x2C
        u32 field_30; // 0x30
    };
    struct CfEneArtsArea {
        u8 _pad[0x19E8];
        CfEneArtsHdr34 header;          // 0x19E8
        u8 _pad1A1C[0x1A20 - 0x1A1C];   // 0x1A1C
        CfEneArtsEntry records[24];     // 0x1A20
    };
    // Common access-type wrappers so the Func120 header copy reads/writes
    // share one TBAA parent type (retail schedules them conservatively
    // serialized, as if src and dst may alias).
    struct CfEneArtsHdrBlock {
        CfEneArtsHdr34 header;
    };
    struct CfEneArtsSrcHdrBlock {
        CfEneArtsHdr34 header;
    };
    // Func120 source: header at +0x0, records at +0x38.
    struct CfEneArtsData {
        CfEneArtsHdr34 header;          // 0x00
        u8 _pad34[0x38 - 0x34];         // 0x34
        CfEneArtsEntry records[24];     // 0x38
    };

    // --- CfObjectEne arts/attack loaders (UnkVirtualFunc166/167) views ---

    // Attack-parameter record returned by getAtkParam (CAttackSet+0x4,
    // stride 0x88; cf. CtrlObjectParamAtkView). UnkVirtualFunc167 additionally
    // writes the f32 at +0x60 and stores u16 results into the +0x38..+0x40
    // halfwords the generic view types differently.
    struct CfEneAtkParamView {
        char name[0x20];          // 0x00 (strcpy/strlen target)
        u32 field_0x20;           // 0x20 (strlen result)
        u8 _pad24[0x2C - 0x24];
        f32 field_0x2C;           // 0x2C
        f32 field_0x30;           // 0x30
        u8 _pad34[0x36 - 0x34];
        s16 field_0x36;           // 0x36 (sign-extended byte)
        s16 field_0x38;           // 0x38
        s16 field_0x3A;           // 0x3A
        s16 field_0x3C;           // 0x3C
        s16 field_0x40;           // 0x40
        u8 field_0x42;            // 0x42
        u8 field_0x43;            // 0x43
        u8 field_0x44;            // 0x44
        s16 field_0x46;           // 0x46
        u8 _pad48[0x58 - 0x48];
        s16 field_0x58;           // 0x58
        s16 field_0x5A;           // 0x5A
        s16 field_0x5C;           // 0x5C
        s16 field_0x5E;           // 0x5E
        f32 field_0x60;           // 0x60 (UnkVirtualFunc167 gauge)
        s16 field_0x64;           // 0x64 (fctiwz product)
        u8 _pad66[0x76 - 0x66];
        u8 field_0x76;            // 0x76
        u8 field_0x77;            // 0x77
        u32 field_0x78;           // 0x78 (flag word)
    };

    // Arts-parameter record returned by getArtsParamByIdx (CArtsSet+0x38,
    // stride 0x8C; the +0x84 word is the per-record vtable pointer).
    // UnkVirtualFunc166 writes the bdat columns into the fields below.
    struct CfEneArtsParamView {
        char name[0x20];          // 0x00 (strcpy/strlen target)
        u32 field_0x20;           // 0x20 (strlen result)
        u16 field_0x28;           // 0x28
        u8 field_0x2A;            // 0x2A (conditional re-read store)
        u8 field_0x2B;            // 0x2B
        f32 field_0x2C;           // 0x2C
        f32 field_0x30;           // 0x30 (0xFF-gated fallback)
        s16 field_0x34;           // 0x34
        s16 field_0x36;           // 0x36
        s16 field_0x38;           // 0x38
        s16 field_0x3A;           // 0x3A
        u16 field_0x3C;           // 0x3C
        u8 field_0x3E;            // 0x3E
        u16 field_0x40;           // 0x40
        u8 field_0x42;            // 0x42
        u8 field_0x43;            // 0x43
        u8 field_0x44;            // 0x44
        u16 field_0x46;           // 0x46
        u16 field_0x48;           // 0x48
        u16 field_0x4A;           // 0x4A
        u16 field_0x4C;           // 0x4C
        f32 field_0x50;           // 0x50
        f32 field_0x54;           // 0x54
        u16 field_0x58;           // 0x58
        u16 field_0x5A;           // 0x5A
        u16 field_0x5C;           // 0x5C
        u16 field_0x5E;           // 0x5E
        f32 field_0x60;           // 0x60
        u16 field_0x64;           // 0x64
        u8 field_0x66;            // 0x66
        u8 field_0x67;            // 0x67
        s16 field_0x68;           // 0x68
        s16 field_0x6A;           // 0x6A
        u8 _pad6C[0x76 - 0x6C];
        u8 field_0x76;            // 0x76
        u8 field_0x77;            // 0x77
        u32 field_0x78;           // 0x78 (flag word)
    };

    // Record vtable dispatch (proven CArtsSet.cpp CAttackParamVtblRec
    // pattern): the 0x84-byte data base places the record's vptr at +0x84,
    // the first virtual at vtable slot 2 (offset +0x8). The per-record init
    // routine is dispatched by UnkVirtualFunc166 through this shape.
    struct CfEneAtkData84 {
        u8 field_0[0x84];
    };
    struct CfEneAtkVtblRec : CfEneAtkData84 {
        virtual void vtInit() = 0;  // vtable slot 2 (offset 8)
    };

    // --- func_800ADDA8 views ---

    // Battle-gauge/rates region behind the primary vtable slots +0x1C4
    // (float at +0x0) and +0x20C (floats at +0x44/+0x48; the func_800ADDA8
    // rates record stores through the whole range below).
    struct CfEneMoveRateView {
        f32 field_0x0;   // 0x00 (slot +0x1C4 result)
    };
    struct CfEneRatesView {
        u32 field_0x0;   // 0x00
        f32 field_0x4;   // 0x04
        u8 _pad8[0x0C - 0x8];
        f32 field_0xC;   // 0x0C
        f32 field_0x10;  // 0x10
        u8 _pad14[0x18 - 0x14];
        f32 field_0x18;  // 0x18
        s16 field_0x1C;  // 0x1C
        s16 field_0x1E;  // 0x1E
        s16 field_0x20;  // 0x20
        u8 _pad22[0x40 - 0x22];
        f32 field_0x40;  // 0x40
        f32 field_0x44;  // 0x44
        f32 field_0x48;  // 0x48
        u8 _pad4C[0x56 - 0x4C];
        u8 field_0x56;   // 0x56
        u8 field_0x57;   // 0x57
        u8 _pad58[0x60 - 0x58];
        u16 field_0x60;  // 0x60
        u16 field_0x62;  // 0x62
        u8 _pad64[0x70 - 0x64];
        u8 field_0x70;   // 0x70
        u8 _pad71[0x72 - 0x71];
        u8 field_0x72;   // 0x72
        u8 field_0x73;   // 0x73
    };

    // Primary param-region fields written by func_800ADDA8: the u32 at
    // +0x15F0, the mode bytes +0x1629..0x162C, the u16 -accumulated words
    // +0x1648/+0x164A/+0x164C and the tail u32s +0x3370/+0x3374/+0x3F00/
    // +0x3F04/+0x3F10.
    struct CfEneParamsView {
        u8 _pad[0x15F0];
        u32 field_0x15F0;           // 0x15F0
        u8 _pad15F4[0x1604 - 0x15F4];
        u32 field_0x1604;           // 0x1604 (weight /10 quotient)
        u32 field_0x1608;           // 0x1608
        u8 _pad160C[0x1629 - 0x160C];
        u8 field_0x1629;            // 0x1629
        u8 field_0x162A;            // 0x162A
        u8 field_0x162B;            // 0x162B
        u8 field_0x162C;            // 0x162C
        u8 _pad162D[0x1634 - 0x162D];
        u32 field_0x1634;           // 0x1634
        u32 field_0x1638;           // 0x1638
        u32 field_0x163C;           // 0x163C
        u32 field_0x1640;           // 0x1640
        u32 field_0x1644;           // 0x1644
        u16 field_0x1648;           // 0x1648
        u16 field_0x164A;           // 0x164A
        u16 field_0x164C;           // 0x164C
        u8 _pad164E[0x3370 - 0x164E];
        u32 field_0x3370;           // 0x3370
        u32 field_0x3374;           // 0x3374 (cf. CfActorParamFields)
        u8 _pad3378[0x3F00 - 0x3378];
        u32 field_0x3F00;           // 0x3F00
        u32 field_0x3F04;           // 0x3F04
        u8 _pad3F08[0x3F10 - 0x3F08];
        u32 field_0x3F10;           // 0x3F10
    };

    // 0x34-byte stack buffer built by func_800ADDA8 and passed to the +0x8
    // CBattleState subobject vtable slot +0x18 (same shape as CfActor179Buf;
    // the retail writes the +0x08 word with a full 32-bit store).
    struct CfEneSetupBuf {
        u32 field_0x0;   // 0x00 (this->3F10)
        u32 field_0x4;   // 0x04 (= 0)
        u32 field_0x8;   // 0x08 (0x2000 first build / 0 second build)
        u16 field_0xC;   // 0x0C (this->1634 / 0xC3)
        u32 field_0x10;  // 0x10 (this->163C / kind count)
        u16 field_0x14;  // 0x14 (this->1638)
        u16 field_0x16;  // 0x16 (this->1640)
        u16 field_0x18;  // 0x18 (this->1644)
        u8 _pad1A[0x20 - 0x1A];
        f32 field_0x20;  // 0x20 (= 0.0f)
        u8 _pad24[0x30 - 0x24];
        u32 field_0x30;  // 0x30 (= 1)
    };

    // --- func_800ADDA8 vtable proxies ---

    // Primary-vtable call proxy for the CfObjectEne vtable slots +0xDC/+0xE4
    // (u8 arg) and +0xE8 (float arg), used by the func_800ADDA8 bdat setup.
    // Dummy slots pin the offsets (RTTI 8-byte header; Nth declared virtual
    // at (N+1)*4 + 4 = slot 8 + 4N).
    class CfEneVtSetup1 {
    public:
        virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
        virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
        virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
        virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
        virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
        virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
        virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
        virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
        virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
        virtual void _v098(); virtual void _v09C(); virtual void _v0A0(); virtual void _v0A4();
        virtual void _v0A8(); virtual void _v0AC(); virtual void _v0B0(); virtual void _v0B4();
        virtual void _v0B8(); virtual void _v0BC(); virtual void _v0C0(); virtual void _v0C4();
        virtual void _v0C8(); virtual void _v0CC(); virtual void _v0D0(); virtual void _v0D4();
        virtual void _v0D8();
        virtual void vfDC(u8 value);         // vtable +0xDC
        virtual void _v0E0();
        virtual void vfE4(u8 value);         // vtable +0xE4
        virtual void vfE8(float value);      // vtable +0xE8
    };

    // Primary-vtable slots +0x170/+0x188 (int arg), +0x198/+0x1A4/+0x1BC
    // (float arg) used by func_800ADDA8's actor-state setup.
    class CfEneVtSetup2 {
    public:
        virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
        virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
        virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
        virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
        virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
        virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
        virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
        virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
        virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
        virtual void _v098(); virtual void _v09C(); virtual void _v0A0(); virtual void _v0A4();
        virtual void _v0A8(); virtual void _v0AC(); virtual void _v0B0(); virtual void _v0B4();
        virtual void _v0B8(); virtual void _v0BC(); virtual void _v0C0(); virtual void _v0C4();
        virtual void _v0C8(); virtual void _v0CC(); virtual void _v0D0(); virtual void _v0D4();
        virtual void _v0D8(); virtual void _v0DC(); virtual void _v0E0(); virtual void _v0E4();
        virtual void _v0E8(); virtual void _v0EC(); virtual void _v0F0(); virtual void _v0F4();
        virtual void _v0F8(); virtual void _v0FC(); virtual void _v100(); virtual void _v104();
        virtual void _v108(); virtual void _v10C(); virtual void _v110(); virtual void _v114();
        virtual void _v118(); virtual void _v11C(); virtual void _v120(); virtual void _v124();
        virtual void _v128(); virtual void _v12C(); virtual void _v130(); virtual void _v134();
        virtual void _v138(); virtual void _v13C(); virtual void _v140(); virtual void _v144();
        virtual void _v148(); virtual void _v14C(); virtual void _v150(); virtual void _v154();
        virtual void _v158(); virtual void _v15C(); virtual void _v160(); virtual void _v164();
        virtual void _v168(); virtual void _v16C();
        virtual void vf170(int value);       // vtable +0x170
        virtual void _v174(); virtual void _v178(); virtual void _v17C(); virtual void _v180();
        virtual void _v184();
        virtual void vf188(int value);       // vtable +0x188
        virtual void _v18C(); virtual void _v190(); virtual void _v194();
        virtual void vf198(float value);     // vtable +0x198
        virtual void _v19C(); virtual void _v1A0();
        virtual void vf1A4(float value);     // vtable +0x1A4
        virtual void _v1A8(); virtual void _v1AC(); virtual void _v1B0(); virtual void _v1B4();
        virtual void _v1B8();
        virtual void vf1BC(float value);     // vtable +0x1BC
    };

    // Primary-vtable slots +0x21C (ptr arg), +0x254 (u8 arg), +0x25C (int
    // arg), +0x264/+0x26C (float arg) used by func_800ADDA8's tail setup.
    class CfEneVtSetup3 {
    public:
        virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
        virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
        virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
        virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
        virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
        virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
        virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
        virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
        virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
        virtual void _v098(); virtual void _v09C(); virtual void _v0A0(); virtual void _v0A4();
        virtual void _v0A8(); virtual void _v0AC(); virtual void _v0B0(); virtual void _v0B4();
        virtual void _v0B8(); virtual void _v0BC(); virtual void _v0C0(); virtual void _v0C4();
        virtual void _v0C8(); virtual void _v0CC(); virtual void _v0D0(); virtual void _v0D4();
        virtual void _v0D8(); virtual void _v0DC(); virtual void _v0E0(); virtual void _v0E4();
        virtual void _v0E8(); virtual void _v0EC(); virtual void _v0F0(); virtual void _v0F4();
        virtual void _v0F8(); virtual void _v0FC(); virtual void _v100(); virtual void _v104();
        virtual void _v108(); virtual void _v10C(); virtual void _v110(); virtual void _v114();
        virtual void _v118(); virtual void _v11C(); virtual void _v120(); virtual void _v124();
        virtual void _v128(); virtual void _v12C(); virtual void _v130(); virtual void _v134();
        virtual void _v138(); virtual void _v13C(); virtual void _v140(); virtual void _v144();
        virtual void _v148(); virtual void _v14C(); virtual void _v150(); virtual void _v154();
        virtual void _v158(); virtual void _v15C(); virtual void _v160(); virtual void _v164();
        virtual void _v168(); virtual void _v16C(); virtual void _v170(); virtual void _v174();
        virtual void _v178(); virtual void _v17C(); virtual void _v180(); virtual void _v184();
        virtual void _v188(); virtual void _v18C(); virtual void _v190(); virtual void _v194();
        virtual void _v198(); virtual void _v19C(); virtual void _v1A0(); virtual void _v1A4();
        virtual void _v1A8(); virtual void _v1AC(); virtual void _v1B0(); virtual void _v1B4();
        virtual void _v1B8(); virtual void _v1BC(); virtual void _v1C0(); virtual void _v1C4();
        virtual void _v1C8(); virtual void _v1CC(); virtual void _v1D0(); virtual void _v1D4();
        virtual void _v1D8(); virtual void _v1DC(); virtual void _v1E0(); virtual void _v1E4();
        virtual void _v1E8(); virtual void _v1EC(); virtual void _v1F0(); virtual void _v1F4();
        virtual void _v1F8(); virtual void _v1FC(); virtual void _v200(); virtual void _v204();
        virtual void _v208(); virtual void _v20C(); virtual void _v210(); virtual void _v214();
        virtual void _v218();
        virtual void vf21C(void* arg);       // vtable +0x21C
        virtual void _v220(); virtual void _v224(); virtual void _v228(); virtual void _v22C();
        virtual void _v230(); virtual void _v234(); virtual void _v238(); virtual void _v23C();
        virtual void _v240(); virtual void _v244(); virtual void _v248(); virtual void _v24C();
        virtual void _v250();
        virtual void vf254(u8 value);        // vtable +0x254
        virtual void _v258();
        virtual void vf25C(int value);       // vtable +0x25C
        virtual void _v260();
        virtual void vf264(float value);     // vtable +0x264
        virtual void _v268();
        virtual void vf26C(float value);     // vtable +0x26C
    };

    // Primary-vtable call proxy for the arts/attack loader slots (166/167)
    // and func_800ADDA8's rates read: +0x1C4 (rates float view), +0x20C
    // (rates view), +0x27C (arts-set base), +0x288 (arts-slot u16 array).
    class CfEneVtActs {
    public:
        virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
        virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
        virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
        virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
        virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
        virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
        virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
        virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
        virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
        virtual void _v098(); virtual void _v09C(); virtual void _v0A0(); virtual void _v0A4();
        virtual void _v0A8(); virtual void _v0AC(); virtual void _v0B0(); virtual void _v0B4();
        virtual void _v0B8(); virtual void _v0BC(); virtual void _v0C0(); virtual void _v0C4();
        virtual void _v0C8(); virtual void _v0CC(); virtual void _v0D0(); virtual void _v0D4();
        virtual void _v0D8(); virtual void _v0DC(); virtual void _v0E0(); virtual void _v0E4();
        virtual void _v0E8(); virtual void _v0EC(); virtual void _v0F0(); virtual void _v0F4();
        virtual void _v0F8(); virtual void _v0FC(); virtual void _v100(); virtual void _v104();
        virtual void _v108(); virtual void _v10C(); virtual void _v110(); virtual void _v114();
        virtual void _v118(); virtual void _v11C(); virtual void _v120(); virtual void _v124();
        virtual void _v128(); virtual void _v12C(); virtual void _v130(); virtual void _v134();
        virtual void _v138(); virtual void _v13C(); virtual void _v140(); virtual void _v144();
        virtual void _v148(); virtual void _v14C(); virtual void _v150(); virtual void _v154();
        virtual void _v158(); virtual void _v15C(); virtual void _v160(); virtual void _v164();
        virtual void _v168(); virtual void _v16C(); virtual void _v170(); virtual void _v174();
        virtual void _v178(); virtual void _v17C(); virtual void _v180(); virtual void _v184();
        virtual void _v188(); virtual void _v18C(); virtual void _v190(); virtual void _v194();
        virtual void _v198(); virtual void _v19C(); virtual void _v1A0(); virtual void _v1A4();
        virtual void _v1A8(); virtual void _v1AC(); virtual void _v1B0(); virtual void _v1B4();
        virtual void _v1B8(); virtual void _v1BC(); virtual void _v1C0();
        virtual CfEneMoveRateView* vf1C4();  // vtable +0x1C4
        virtual void _v1C8(); virtual void _v1CC(); virtual void _v1D0(); virtual void _v1D4();
        virtual void _v1D8(); virtual void _v1DC(); virtual void _v1E0(); virtual void _v1E4();
        virtual void _v1E8(); virtual void _v1EC(); virtual void _v1F0(); virtual void _v1F4();
        virtual void _v1F8(); virtual void _v1FC(); virtual void _v200(); virtual void _v204();
        virtual void _v208();
        virtual CfEneRatesView* vf20C();     // vtable +0x20C
        virtual void _v210(); virtual void _v214(); virtual void _v218(); virtual void _v21C();
        virtual void _v220(); virtual void _v224(); virtual void _v228(); virtual void _v22C();
        virtual void _v230(); virtual void _v234(); virtual void _v238(); virtual void _v23C();
        virtual void _v240(); virtual void _v244(); virtual void _v248(); virtual void _v24C();
        virtual void _v250(); virtual void _v254(); virtual void _v258(); virtual void _v25C();
        virtual void _v260(); virtual void _v264(); virtual void _v268(); virtual void _v26C();
        virtual void _v270(); virtual void _v274(); virtual void _v278();
        virtual void* vf27C();               // vtable +0x27C
        virtual void _v280(); virtual void _v284();
        virtual u16* vf288();                // vtable +0x288
    };

    // Primary-vtable slots +0x32C/+0x330 (no-arg) and +0x334 (float arg)
    // used by func_800ADDA8's tail dispatch.
    class CfEneVtTail {
    public:
        virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
        virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
        virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
        virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
        virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
        virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
        virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
        virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
        virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
        virtual void _v098(); virtual void _v09C(); virtual void _v0A0(); virtual void _v0A4();
        virtual void _v0A8(); virtual void _v0AC(); virtual void _v0B0(); virtual void _v0B4();
        virtual void _v0B8(); virtual void _v0BC(); virtual void _v0C0(); virtual void _v0C4();
        virtual void _v0C8(); virtual void _v0CC(); virtual void _v0D0(); virtual void _v0D4();
        virtual void _v0D8(); virtual void _v0DC(); virtual void _v0E0(); virtual void _v0E4();
        virtual void _v0E8(); virtual void _v0EC(); virtual void _v0F0(); virtual void _v0F4();
        virtual void _v0F8(); virtual void _v0FC(); virtual void _v100(); virtual void _v104();
        virtual void _v108(); virtual void _v10C(); virtual void _v110(); virtual void _v114();
        virtual void _v118(); virtual void _v11C(); virtual void _v120(); virtual void _v124();
        virtual void _v128(); virtual void _v12C(); virtual void _v130(); virtual void _v134();
        virtual void _v138(); virtual void _v13C(); virtual void _v140(); virtual void _v144();
        virtual void _v148(); virtual void _v14C(); virtual void _v150(); virtual void _v154();
        virtual void _v158(); virtual void _v15C(); virtual void _v160(); virtual void _v164();
        virtual void _v168(); virtual void _v16C(); virtual void _v170(); virtual void _v174();
        virtual void _v178(); virtual void _v17C(); virtual void _v180(); virtual void _v184();
        virtual void _v188(); virtual void _v18C(); virtual void _v190(); virtual void _v194();
        virtual void _v198(); virtual void _v19C(); virtual void _v1A0(); virtual void _v1A4();
        virtual void _v1A8(); virtual void _v1AC(); virtual void _v1B0(); virtual void _v1B4();
        virtual void _v1B8(); virtual void _v1BC(); virtual void _v1C0(); virtual void _v1C4();
        virtual void _v1C8(); virtual void _v1CC(); virtual void _v1D0(); virtual void _v1D4();
        virtual void _v1D8(); virtual void _v1DC(); virtual void _v1E0(); virtual void _v1E4();
        virtual void _v1E8(); virtual void _v1EC(); virtual void _v1F0(); virtual void _v1F4();
        virtual void _v1F8(); virtual void _v1FC(); virtual void _v200(); virtual void _v204();
        virtual void _v208(); virtual void _v20C(); virtual void _v210(); virtual void _v214();
        virtual void _v218(); virtual void _v21C(); virtual void _v220(); virtual void _v224();
        virtual void _v228(); virtual void _v22C(); virtual void _v230(); virtual void _v234();
        virtual void _v238(); virtual void _v23C(); virtual void _v240(); virtual void _v244();
        virtual void _v248(); virtual void _v24C(); virtual void _v250(); virtual void _v254();
        virtual void _v258(); virtual void _v25C(); virtual void _v260(); virtual void _v264();
        virtual void _v268(); virtual void _v26C(); virtual void _v270(); virtual void _v274();
        virtual void _v278(); virtual void _v27C(); virtual void _v280(); virtual void _v284();
        virtual void _v288(); virtual void _v28C(); virtual void _v290(); virtual void _v294();
        virtual void _v298(); virtual void _v29C(); virtual void _v2A0(); virtual void _v2A4();
        virtual void _v2A8(); virtual void _v2AC(); virtual void _v2B0(); virtual void _v2B4();
        virtual void _v2B8(); virtual void _v2BC(); virtual void _v2C0(); virtual void _v2C4();
        virtual void _v2C8(); virtual void _v2CC(); virtual void _v2D0(); virtual void _v2D4();
        virtual void _v2D8(); virtual void _v2DC(); virtual void _v2E0(); virtual void _v2E4();
        virtual void _v2E8(); virtual void _v2EC(); virtual void _v2F0(); virtual void _v2F4();
        virtual void _v2F8(); virtual void _v2FC(); virtual void _v300(); virtual void _v304();
        virtual void _v308(); virtual void _v30C(); virtual void _v310(); virtual void _v314();
        virtual void _v318(); virtual void _v31C(); virtual void _v320(); virtual void _v324();
        virtual void _v328();
        virtual void vf32C();                // vtable +0x32C
        virtual void vf330();                // vtable +0x330
        virtual void vf334(float value);     // vtable +0x334
    };

    // Primary-vtable slot +0x5E0 (no-arg) used by func_800ADDA8.
    class CfEneVt5E0 {
    public:
        virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
        virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
        virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
        virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
        virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
        virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
        virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
        virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
        virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
        virtual void _v098(); virtual void _v09C(); virtual void _v0A0(); virtual void _v0A4();
        virtual void _v0A8(); virtual void _v0AC(); virtual void _v0B0(); virtual void _v0B4();
        virtual void _v0B8(); virtual void _v0BC(); virtual void _v0C0(); virtual void _v0C4();
        virtual void _v0C8(); virtual void _v0CC(); virtual void _v0D0(); virtual void _v0D4();
        virtual void _v0D8(); virtual void _v0DC(); virtual void _v0E0(); virtual void _v0E4();
        virtual void _v0E8(); virtual void _v0EC(); virtual void _v0F0(); virtual void _v0F4();
        virtual void _v0F8(); virtual void _v0FC(); virtual void _v100(); virtual void _v104();
        virtual void _v108(); virtual void _v10C(); virtual void _v110(); virtual void _v114();
        virtual void _v118(); virtual void _v11C(); virtual void _v120(); virtual void _v124();
        virtual void _v128(); virtual void _v12C(); virtual void _v130(); virtual void _v134();
        virtual void _v138(); virtual void _v13C(); virtual void _v140(); virtual void _v144();
        virtual void _v148(); virtual void _v14C(); virtual void _v150(); virtual void _v154();
        virtual void _v158(); virtual void _v15C(); virtual void _v160(); virtual void _v164();
        virtual void _v168(); virtual void _v16C(); virtual void _v170(); virtual void _v174();
        virtual void _v178(); virtual void _v17C(); virtual void _v180(); virtual void _v184();
        virtual void _v188(); virtual void _v18C(); virtual void _v190(); virtual void _v194();
        virtual void _v198(); virtual void _v19C(); virtual void _v1A0(); virtual void _v1A4();
        virtual void _v1A8(); virtual void _v1AC(); virtual void _v1B0(); virtual void _v1B4();
        virtual void _v1B8(); virtual void _v1BC(); virtual void _v1C0(); virtual void _v1C4();
        virtual void _v1C8(); virtual void _v1CC(); virtual void _v1D0(); virtual void _v1D4();
        virtual void _v1D8(); virtual void _v1DC(); virtual void _v1E0(); virtual void _v1E4();
        virtual void _v1E8(); virtual void _v1EC(); virtual void _v1F0(); virtual void _v1F4();
        virtual void _v1F8(); virtual void _v1FC(); virtual void _v200(); virtual void _v204();
        virtual void _v208(); virtual void _v20C(); virtual void _v210(); virtual void _v214();
        virtual void _v218(); virtual void _v21C(); virtual void _v220(); virtual void _v224();
        virtual void _v228(); virtual void _v22C(); virtual void _v230(); virtual void _v234();
        virtual void _v238(); virtual void _v23C(); virtual void _v240(); virtual void _v244();
        virtual void _v248(); virtual void _v24C(); virtual void _v250(); virtual void _v254();
        virtual void _v258(); virtual void _v25C(); virtual void _v260(); virtual void _v264();
        virtual void _v268(); virtual void _v26C(); virtual void _v270(); virtual void _v274();
        virtual void _v278(); virtual void _v27C(); virtual void _v280(); virtual void _v284();
        virtual void _v288(); virtual void _v28C(); virtual void _v290(); virtual void _v294();
        virtual void _v298(); virtual void _v29C(); virtual void _v2A0(); virtual void _v2A4();
        virtual void _v2A8(); virtual void _v2AC(); virtual void _v2B0(); virtual void _v2B4();
        virtual void _v2B8(); virtual void _v2BC(); virtual void _v2C0(); virtual void _v2C4();
        virtual void _v2C8(); virtual void _v2CC(); virtual void _v2D0(); virtual void _v2D4();
        virtual void _v2D8(); virtual void _v2DC(); virtual void _v2E0(); virtual void _v2E4();
        virtual void _v2E8(); virtual void _v2EC(); virtual void _v2F0(); virtual void _v2F4();
        virtual void _v2F8(); virtual void _v2FC(); virtual void _v300(); virtual void _v304();
        virtual void _v308(); virtual void _v30C(); virtual void _v310(); virtual void _v314();
        virtual void _v318(); virtual void _v31C(); virtual void _v320(); virtual void _v324();
        virtual void _v328(); virtual void _v32C(); virtual void _v330(); virtual void _v334();
        virtual void _v338(); virtual void _v33C(); virtual void _v340(); virtual void _v344();
        virtual void _v348(); virtual void _v34C(); virtual void _v350(); virtual void _v354();
        virtual void _v358(); virtual void _v35C(); virtual void _v360(); virtual void _v364();
        virtual void _v368(); virtual void _v36C(); virtual void _v370(); virtual void _v374();
        virtual void _v378(); virtual void _v37C(); virtual void _v380(); virtual void _v384();
        virtual void _v388(); virtual void _v38C(); virtual void _v390(); virtual void _v394();
        virtual void _v398(); virtual void _v39C(); virtual void _v3A0(); virtual void _v3A4();
        virtual void _v3A8(); virtual void _v3AC(); virtual void _v3B0(); virtual void _v3B4();
        virtual void _v3B8(); virtual void _v3BC(); virtual void _v3C0(); virtual void _v3C4();
        virtual void _v3C8(); virtual void _v3CC(); virtual void _v3D0(); virtual void _v3D4();
        virtual void _v3D8(); virtual void _v3DC(); virtual void _v3E0(); virtual void _v3E4();
        virtual void _v3E8(); virtual void _v3EC(); virtual void _v3F0(); virtual void _v3F4();
        virtual void _v3F8(); virtual void _v3FC(); virtual void _v400(); virtual void _v404();
        virtual void _v408(); virtual void _v40C(); virtual void _v410(); virtual void _v414();
        virtual void _v418(); virtual void _v41C(); virtual void _v420(); virtual void _v424();
        virtual void _v428(); virtual void _v42C(); virtual void _v430(); virtual void _v434();
        virtual void _v438(); virtual void _v43C(); virtual void _v440(); virtual void _v444();
        virtual void _v448(); virtual void _v44C(); virtual void _v450(); virtual void _v454();
        virtual void _v458(); virtual void _v45C(); virtual void _v460(); virtual void _v464();
        virtual void _v468(); virtual void _v46C(); virtual void _v470(); virtual void _v474();
        virtual void _v478(); virtual void _v47C(); virtual void _v480(); virtual void _v484();
        virtual void _v488(); virtual void _v48C(); virtual void _v490(); virtual void _v494();
        virtual void _v498(); virtual void _v49C(); virtual void _v4A0(); virtual void _v4A4();
        virtual void _v4A8(); virtual void _v4AC(); virtual void _v4B0(); virtual void _v4B4();
        virtual void _v4B8(); virtual void _v4BC(); virtual void _v4C0(); virtual void _v4C4();
        virtual void _v4C8(); virtual void _v4CC(); virtual void _v4D0(); virtual void _v4D4();
        virtual void _v4D8(); virtual void _v4DC(); virtual void _v4E0(); virtual void _v4E4();
        virtual void _v4E8(); virtual void _v4EC(); virtual void _v4F0(); virtual void _v4F4();
        virtual void _v4F8(); virtual void _v4FC(); virtual void _v500(); virtual void _v504();
        virtual void _v508(); virtual void _v50C(); virtual void _v510(); virtual void _v514();
        virtual void _v518(); virtual void _v51C(); virtual void _v520(); virtual void _v524();
        virtual void _v528(); virtual void _v52C(); virtual void _v530(); virtual void _v534();
        virtual void _v538(); virtual void _v53C(); virtual void _v540(); virtual void _v544();
        virtual void _v548(); virtual void _v54C(); virtual void _v550(); virtual void _v554();
        virtual void _v558(); virtual void _v55C(); virtual void _v560(); virtual void _v564();
        virtual void _v568(); virtual void _v56C(); virtual void _v570(); virtual void _v574();
        virtual void _v578(); virtual void _v57C(); virtual void _v580(); virtual void _v584();
        virtual void _v588(); virtual void _v58C(); virtual void _v590(); virtual void _v594();
        virtual void _v598(); virtual void _v59C(); virtual void _v5A0(); virtual void _v5A4();
        virtual void _v5A8(); virtual void _v5AC(); virtual void _v5B0(); virtual void _v5B4();
        virtual void _v5B8(); virtual void _v5BC(); virtual void _v5C0(); virtual void _v5C4();
        virtual void _v5C8(); virtual void _v5CC(); virtual void _v5D0(); virtual void _v5D4();
        virtual void _v5D8();
        virtual void vf5E0();                 // vtable +0x5E0
    };

    // +0x8 CBattleState subobject vtable proxies (cf. CfBattleVt18): slots
    // +0x7C/+0x84 take a u16 arg (func_800ADDA8 status flags).
    class CfBattleVt7C {
    public:
        virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
        virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
        virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
        virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
        virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
        virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
        virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
        virtual void _v078();
        virtual void vf7C(u16 value);        // vtable +0x7C
    };
    class CfBattleVt84 {
    public:
        virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
        virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
        virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
        virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
        virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
        virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
        virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
        virtual void _v078(); virtual void _v07C(); virtual void _v080();
        virtual void vf84(u16 value);        // vtable +0x84
    };

    // +0x3E9C CfObjectMove subobject vtable proxy for func_800ADDA8: slots
    // +0x3C (u32 arg), +0x134/+0x13C/+0x1D4 (float arg). Same folded-vptr
    // scheme as CfEneSubIf.
    class CfEneSubVt3CIf {
    public:
        virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
        virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
        virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
        virtual void vf3C(u32 value);        // vtable +0x3C
        virtual void _v040(); virtual void _v044(); virtual void _v048(); virtual void _v04C();
        virtual void _v050(); virtual void _v054(); virtual void _v058(); virtual void _v05C();
        virtual void _v060(); virtual void _v064(); virtual void _v068(); virtual void _v06C();
        virtual void _v070(); virtual void _v074(); virtual void _v078(); virtual void _v07C();
        virtual void _v080(); virtual void _v084(); virtual void _v088(); virtual void _v08C();
        virtual void _v090(); virtual void _v094(); virtual void _v098(); virtual void _v09C();
        virtual void _v0A0(); virtual void _v0A4(); virtual void _v0A8(); virtual void _v0AC();
        virtual void _v0B0(); virtual void _v0B4(); virtual void _v0B8(); virtual void _v0BC();
        virtual void _v0C0(); virtual void _v0C4(); virtual void _v0C8(); virtual void _v0CC();
        virtual void _v0D0(); virtual void _v0D4(); virtual void _v0D8(); virtual void _v0DC();
        virtual void _v0E0(); virtual void _v0E4(); virtual void _v0E8(); virtual void _v0EC();
        virtual void _v0F0(); virtual void _v0F4(); virtual void _v0F8(); virtual void _v0FC();
        virtual void _v100(); virtual void _v104(); virtual void _v108(); virtual void _v10C();
        virtual void _v110(); virtual void _v114(); virtual void _v118(); virtual void _v11C();
        virtual void _v120(); virtual void _v124(); virtual void _v128(); virtual void _v12C();
        virtual void _v130();
        virtual void vf134(float value);     // vtable +0x134
        virtual void _v138();
        virtual void vf13C(float value);     // vtable +0x13C
        virtual void _v140(); virtual void _v144(); virtual void _v148(); virtual void _v14C();
        virtual void _v150(); virtual void _v154(); virtual void _v158(); virtual void _v15C();
        virtual void _v160(); virtual void _v164(); virtual void _v168(); virtual void _v16C();
        virtual void _v170(); virtual void _v174(); virtual void _v178(); virtual void _v17C();
        virtual void _v180(); virtual void _v184(); virtual void _v188(); virtual void _v18C();
        virtual void _v190(); virtual void _v194(); virtual void _v198(); virtual void _v19C();
        virtual void _v1A0(); virtual void _v1A4(); virtual void _v1A8(); virtual void _v1AC();
        virtual void _v1B0(); virtual void _v1B4(); virtual void _v1B8(); virtual void _v1BC();
        virtual void _v1C0(); virtual void _v1C4(); virtual void _v1C8(); virtual void _v1CC();
        virtual void _v1D0();
        virtual void vf1D4(float value);     // vtable +0x1D4
    };
    struct CfEneSubVt3C : CfEneSubPad, CfEneSubVt3CIf {};
};


// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void CActorParam_UnkVirtualFunc180__Q22cf13CfObjectActorFv(void* self);
extern "C" void CActorParam_UnkVirtualFunc179__Q22cf13CfObjectActorFv(void* self);
extern "C" void CActorParam_UnkVirtualFunc1__Q22cf13CfObjectActorFv(void* self);
extern "C" void CActorParam_UnkVirtualFunc2__Q22cf13CfObjectActorFv(void);
extern "C" void CActorParam_UnkVirtualFunc3__Q22cf13CfObjectActorFv(void);
extern "C" void func_800ADDA8__Q22cf11CfObjectEneFv(void* self);
// Verbatim `__`-named free function (the retail class is not a polymorphic
// MI hierarchy per the headers; see the CfObjectActor dtor note in
// CfObjectEne.cpp). Plain C++ declaration keeps the verbatim name.
int func_800ADB2C__Q22cf11CfObjectEneFv(cf::CfObjectEne* self);
extern "C" void CfObjectActor_UnkVirtualFunc6__Q22cf13CfObjectActorFv(void* self);
extern "C" void CfObjectActor_UnkVirtualFunc11__Q22cf13CfObjectActorFv(void* self);
extern "C" void CfObjectActor_UnkVirtualFunc10__Q22cf13CfObjectActorFv(void* self);
extern "C" void func_800ADBD4__Q22cf11CfObjectEneFv(void* self);
// Verbatim `__`-named free deleting dtor (same rationale as
// func_800ADB2C__Q22cf11CfObjectEneFv above).
cf::CfObjectEne* __dt__Q22cf11CfObjectEneFv(cf::CfObjectEne* self, s32 deleteFlag);

// C-ABI free-function import: retail CfGameManager "party count" getter
// (Fv-mangled member name; returns the u32 at lbl_eu_806640A4). Declared
// extern "C" in CPartsChange.hpp too; re-declared here so the call reloc
// inside func_800AF82C keeps the exact retail symbol.
extern "C" u32 func_80086B34__Q22cf13CfGameManagerFv();

// C-ABI free-function import: retail CfGameManager BDAT-table getter
// (Fv-mangled member name). func_800AF7E4 passes its return (the bdat base)
// as the first getBdatStringColumnValue argument.
extern "C" u32 func_80086B24__Q22cf13CfGameManagerFv();

// Arts-slot write helper (defined in code_8025FB10.cpp; retail unmangled).
// CActorParam_UnkVirtualFunc88 forwards (obj, obj->field_0x89C, arg) to it.
extern "C" void func_802617B8(u8* obj, u32 value, int flag);

// Pre-mangled member dtor of the CfObjectMove subobject (defined in
// CfObjectMove.cpp). The CfObjectActor dtor calls it directly with a
// non-deleting flag 0 (explicit member-dtor syntax makes MWCC pass -1);
// declared here so the call reloc names the retail symbol.
extern "C" void __dt__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, int flag);

// Verbatim `__`-named free base ctor / vf2 and the CfResReloadImpl ctor
// (defined in CfObjectActor.cpp / CfObjectModel.cpp / CfResReloadImpl.cpp as
// plain global functions). extern "C" keeps the call-site relocs at the
// retail names (a plain C++ declaration makes MWCC append a __F<params>
// mangling; same convention as CfCam.hpp / CTaskGame.hpp).
extern "C" cf::CfObjectActor* __ct__Q22cf13CfObjectActorFv(cf::CfObjectActor* self);
extern "C" int CfObject_UnkVirtualFunc2__Q22cf13CfObjectModelFv(cf::CfObjectModel* self);
extern "C" cf::CfResReloadImpl* __ct__cf_CfResReloadImpl(cf::CfResReloadImpl* self,
                                                          cf::CfResReloadParent* parent);

// C-ABI imports used by the CfObjectEne ctor (func_80061FFC + MemManager),
// func_800ADB2C and the dtor (retail symbols are unmangled).
extern "C" unsigned long func_80061FFC();
extern "C" void func_800BE33C(void* obj, int flag);
extern "C" void func_800BE824(void* obj, int flag);
// func_80174B4C is declared in CfObjectActor.hpp with its full 5-arg ABI
// signature; a second C-linkage declaration here would be an illegal overload.
extern "C" void func_804B0AD4(void* obj, int param, float a, float b);
extern "C" void func_801F4DDC(void* obj, void* actor);
extern "C" void func_801F4D50(void* obj, void* actor);
extern "C" void* getUnk80664658();

// C-ABI imports used by func_800AEC68 / func_800ADBD4 / func_800AF870
// (retail symbols are unmangled).
extern "C" void func_8003AA34();
extern "C" u32 func_8003B41C(u8* bdat);   // bdat first row
extern "C" u32 func_8003B1EC(u8* bdat);   // bdat row count
extern "C" void func_8014B7B0(u8* obj);
extern "C" void func_8015396C(u8* obj, u32 a, u32 b);
extern "C" int func_80174C98(void* actor, int* outVal, int flags); // family-canonical form (CAIAction.hpp) - ABI-identical
extern "C" void func_801765A4(u8* actor, f32 value, u32 arg);
extern "C" int func_801BA2C8(u8* self);
// func_80148778 and getInstance__Q22cf14CBattleManagerFv are declared in
// CAIAction.hpp / kyoshin/cf/CBattleManagerApi.hpp (reachable via
// CfObjectActor.hpp) - keep
// the shared declarations (C-linkage overloads with different pointer types
// are illegal in MWCC).
// CfObjectMove subobject member (defined in CfObjectMove.cpp): func_800ADBD4
// calls it on the +0x3E9C subobject (retail addi r3, r30, 0x3e9c + bl).
extern "C" void CfObject_UnkVirtualFunc4__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self);

// Verbatim-name free function (defined in CfObjectEne.cpp; func_800ADBD4
// tail-dispatches the bdat-match path through it). extern "C" keeps the
// call-site reloc at the unmangled retail name func_800AF870.
extern "C" void func_800AF870(cf::CfObjectEne* self);

// C-ABI imports used by the CfObjectEne arts/attack loaders
// (CActorParam_UnkVirtualFunc166/167; defined in CArtsSet.cpp and
// CAttackSet.cpp with unmangled retail names).
extern "C" void* getAtkParam(void* base, int index);                  // attack-param record lookup
// arts slot value at row*0x10 + col*0x2 + 4 (unsigned short result)
extern "C" unsigned short func_80153CAC(const void* base, int index);
extern "C" void* getArtsParamByIdx(void* base, int index);           // arts-param record lookup
extern "C" void setArtsSlotByIdx(void* base, unsigned short value, int index); // arts slot write

// CfObjectEne vtable blob (.data; ctor/dtor store base + 0xC/0x36C/0x37C).
extern u8 lbl_eu_80528A18[];

// .sdata2 constants: 0.0f gauge floor (Func34/42/31), 1.0f Func34 rounding
// floor, and the +/-0.5 double rounding addends (Func34 round-to-nearest).
extern float lbl_eu_80666968;
extern float lbl_eu_80666980;
extern const double lbl_eu_806669B8;
extern const double lbl_eu_806669C0;

// func_800AEC68 / func_800AF870 constants (.sdata2): 2^52 u16->f32 magic,
// 2^52+2^31 s32->f32 magic and the scale constant.
extern const double lbl_eu_806669A0;
extern const double lbl_eu_806669A8;
extern const float lbl_eu_806669B0;

// func_800AF870 bdat file pointer (.sdata2) and drop-channel tables (.sbss):
// u16 mode word at 0x80663E42 and the per-mode channel halfword table whose
// entries are addressed at byte offset mode*100 from 0x80663E44.
extern u16 lbl_eu_80663E42;
extern u16 lbl_eu_80663E44;

// func_800AF870 bdat file pointer (.sdata2) and column-name string table
// (.rodata entries at +0x19A / +0x2C3 / +0x2CA / +0x2CE / +0x2D7).
extern void* lbl_eu_806640D4;   // unified pointer form (was u8*)
extern char lbl_eu_804FC168[];

// func_800ADB2C func_804B0AD4 float args (.sdata2).
extern float lbl_eu_8066696C;
extern float lbl_eu_80666970;

// .sdata2 constants used by the CfObjectEne arts/attack loaders
// (CActorParam_UnkVirtualFunc166/167) and the func_800ADDA8 setup.
extern float lbl_eu_8066697C;   // u8/u16 -> f32 divisor (166/167, func_800ADDA8)
extern float lbl_eu_80666984;   // func_800ADDA8 vf198/vf1A4 arg
extern float lbl_eu_80666988;   // func_800ADDA8 vf1BC scale
extern float lbl_eu_8066698C;   // func_800ADDA8 vf1BC divisor
extern float lbl_eu_80666990;   // func_800ADDA8 vf334 ratio divisor
extern float lbl_eu_80666994;   // 166 and func_800ADDA8 f32 divisor
extern float lbl_eu_80666998;   // func_800ADDA8 scaled-vfE8 multiplier
extern float lbl_eu_806669B4;   // 166 0xFF arts-gauge fallback
extern float lbl_eu_8066A1F8;   // max gauge value (166 / func_800ADDA8)

// lbl_eu_8066A20C / lbl_eu_8066A210 are already declared `const float` in
// CfObjectModel.hpp (in the CfObjectActor.hpp chain) - reuse those.

// .sbss/.sdata bdat table pointers and the two level-name letter buffers
// used by func_800ADDA8 (byte written at pointer+3).
extern u8* lbl_eu_80664094;
extern u8* lbl_eu_8066409C;
extern u8* lbl_eu_80661CB8;
extern u8* lbl_eu_80661CBC;

#endif
