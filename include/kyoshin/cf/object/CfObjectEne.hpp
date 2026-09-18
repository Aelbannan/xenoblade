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
    virtual void updateEnemyBattleState();
    virtual void func_800ADDA8();
    virtual void initEnemyBdatParams();
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

    // Table-pointer slots rewritten by the CfObjectEne ctor/dtor (retail
    // stores lbl_eu_80528A18 + 0x0/0xC/0x36C/0x37C at these positions).
    struct CfEneTablePtrs {
        u32 vt0;             // +0x00 primary vtable (lbl_eu_80528A18)
        u8 _pad4[0x8 - 0x4];
        u32 vt8;             // +0x08 secondary vtable (+0xC)
        u8 _padC[0x3380 - 0xC];
        u32 vt3380;          // +0x3380 CAIAction subobject vtable (+0x36C)
        u8 _pad3384[0x3E9C - 0x3384];
        u32 vt3E9C;          // +0x3E9C CfObjectMove subobject vtable (+0x37C)
    };


    // Offset base for the thin-MI real-owner views below (no virtuals,
    // no slots - just the byte offset so this+0x3E9C dispatch folds).
    struct CfEneMoveOff {
        u8 _pad[0x3E9C];
    };

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

    // Name/state fields used by initEnemyBdatParams: the bdat lookup name at
    // +0x3F14 (getFP arg) and the u16 row id at +0x3F28.
    struct CfEneLookupView {
        u8 _pad[0x3F14];
        char field_0x3F14[0x28 - 0x14]; // 0x3F14 (getFP name)
        u16 field_0x3F28;               // 0x3F28 (bdat row id)
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
    struct CfEneMoveBaseD {
        u8 _pad[0x3E9C];
        u8 base; // 0x3E9C
    };
    // NOTE: distinct view types do NOT stop MWCC from CSE-ing these
    // addresses across calls in func_800ADB2C (value numbering ignores
    // types); they only keep the source sites self-documenting.

    // Base of the +0x8 CBattleState subobject (func_80148778 arg and the
    // +0x14/+0x20 vtable dispatches in updateEnemyBattleState).
    struct CfEneB8View {
        u8 _pad[0x8];
        u8 field_0x8; // 0x8 (subobject base)
    };


    struct CfEneB8Off {
        u8 _pad[0x8];
    };

    // Real-owner views of the +0x3E9C CfObjectMove subobject (retail offsets).
    // Thin MI (offset + real base) keeps the folded lwz r12,0x3E9C(rX) shape;
    // dispatched methods are the owning-class virtuals (no dummy slots).
    struct CfObjectAt3E9C : CfEneMoveOff, cf::CfObject {};
    struct CObjectParamAt3E9C : CfEneMoveOff, cf::CObjectParam {};
    struct CfObjectMoveAt3E9C : CfEneMoveOff, cf::CfObjectMove {};
    // Real-owner view of the +0x8 CBattleState subobject. Callers use the
    // shared CBattleState aliases (setBattleParam/enterStatusEntry/...),
    // which forward to the same slots (see CBattleState.hpp).
    struct CBattleStateAt8 : CfEneB8Off, cf::CBattleState {};
    // Flags/pointer fields touched by updateEnemyBattleState: the +0x3F34 target
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

    // Addressable raw-column holder for initEnemyBdatParams: full-word store of the
    // getBdatStringColumnValue result, punned u16 read on reload (retail's
    // stw-then-lhz pair around the slot +0xF0 vcall).
    union CfEneColNarrow {
        u32 w;
        u16 h;
        u8 b;
    };

    // func_800ADDA8 / initEnemyBdatParams dispatch through the owning-class
    // CActorParam aliases (setEnemyType / setStatScale / getArtsSlotIds / ...);
    // no TU-local proxy remains.

    // --- CActorParam copyArtsSetBlock/copyAtkSetBlock record structs ---
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

    // Record init dispatches through the owning-class virtual
    // cf::CAttackParam::CAttackParam_clearArtsRecord (vtable slot +0x08,
    // vptr at +0x84; proven CArtsSet.cpp attack-record pattern). No TU-local
    // record iface remains.

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

    // func_800ADDA8 slot map (all dispatched through the owning-class
    // CActorParam aliases; no TU-local proxy remains): +0xDC/+0xE4 (u8 arg),
    // +0xE8/+0x198/+0x1A4/+0x1BC/+0x334 (float arg), +0x170/+0x188/+0x25C
    // (int arg), +0x21C (ptr arg), +0x254 (u32 arg), +0x1C4/+0x20C (rates),
    // +0x27C (arts-set base), +0x288 (arts-slot u16 array), +0x2BC (guard),
    // +0x32C/+0x330 (loaders 166/167), +0x5E0 (no-arg).

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
extern "C" void CfObjectActor_readFacingAngle__Q22cf13CfObjectActorFv(void* self);
extern "C" void CfObjectActor_UnkVirtualFunc11__Q22cf13CfObjectActorFv(void* self);
extern "C" void CfObjectActor_UnkVirtualFunc10__Q22cf13CfObjectActorFv(void* self);
extern "C" void updateEnemyBattleState__Q22cf11CfObjectEneFv(void* self);
// Verbatim `__`-named free deleting dtor (same rationale as
// func_800ADB2C__Q22cf11CfObjectEneFv above).
cf::CfObjectEne* __dt__Q22cf11CfObjectEneFv(cf::CfObjectEne* self, s32 deleteFlag);

// C-ABI free-function import: retail CfGameManager "party count" getter
// (Fv-mangled member name; returns the u32 at lbl_eu_806640A4). Declared
// extern "C" in CPartsChange.hpp too; re-declared here so the call reloc
// inside func_800AF82C keeps the exact retail symbol.
extern "C" u32 getGlobalWord640A4__Q22cf13CfGameManagerFv();

// C-ABI free-function import: retail CfGameManager BDAT-table getter
// (Fv-mangled member name). func_800AF7E4 passes its return (the bdat base)
// as the first getBdatStringColumnValue argument.
extern "C" u32 getGlobalPtr6409C__Q22cf13CfGameManagerFv();

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
extern "C" void CfObjectMove_setModelDisplayFlag(void* obj, int flag);
extern "C" void CfObjectMove_setRegionAttached(void* obj, int flag);
// func_80174B4C is declared in CfObjectActor.hpp with its full 5-arg ABI
// signature; a second C-linkage declaration here would be an illegal overload.
extern "C" void func_804B0AD4(void* obj, int param, float a, float b);
extern "C" void func_801F4DDC(void* obj, void* actor);
extern "C" void func_801F4D50(void* obj, void* actor);
// getUnk80664658: single typed view lives on CSuddenCommu.hpp (extern "C"
// block); this tail must not redeclare it with a divergent return type.

// C-ABI imports used by initEnemyBdatParams / updateEnemyBattleState / func_800AF870
// (retail symbols are unmangled).
// Bdat_GetRowBase_B41C / Bdat_GetMaxRow_B1EC: canonical form per ocBdat.cpp definitions
// (u32(void*)); CfObjectPc.hpp's block uses the same spelling.
extern "C" void* Bdat_GetTable_AA34();
extern "C" u32 Bdat_GetRowBase_B41C(void* bdat);   // bdat first row (must match CfObjectPc.hpp)
extern "C" u32 Bdat_GetMaxRow_B1EC(void* bdat);   // bdat row count (must match CfObjectPc.hpp)
extern "C" void func_8014B7B0(u8* obj);
extern "C" void func_8015396C(u8* obj, u32 a, u32 b);
// func_80174C98 / func_80174B4C are owned by kyoshin/cf/CfMapItemManager.hpp.
extern "C" void func_801765A4(void* actor, f32 value, u32 arg);
extern "C" unsigned long func_801BA2C8(void* self);
// func_80148778 and getInstance__Q22cf14CBattleManagerFv are declared in
// CAIAction.hpp / kyoshin/cf/CBattleManagerApi.hpp (reachable via
// CfObjectActor.hpp) - keep
// the shared declarations (C-linkage overloads with different pointer types
// are illegal in MWCC).
// CfObjectMove subobject member (defined in CfObjectMove.cpp): updateEnemyBattleState
// calls it on the +0x3E9C subobject (retail addi r3, r30, 0x3e9c + bl).
extern "C" void CfObject_UnkVirtualFunc4__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self);

// Verbatim-name free function (defined in CfObjectEne.cpp; updateEnemyBattleState
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

// initEnemyBdatParams / func_800AF870 constants (.sdata2): 2^52 u16->f32 magic,
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
extern const float lbl_eu_8066A1F8;   // max gauge value (166 / func_800ADDA8); const form aligned with CChain.hpp / CBattleManager TU decls

// lbl_eu_8066A20C / lbl_eu_8066A210 are already declared `const float` in
// CfObjectModel.hpp (in the CfObjectActor.hpp chain) - reuse those.

// .sbss/.sdata bdat table pointers and the two level-name letter buffers
// used by func_800ADDA8 (byte written at pointer+3).
extern u8* lbl_eu_80664094;
extern u8* lbl_eu_8066409C;
extern u8* lbl_eu_80661CB8;
extern u8* lbl_eu_80661CBC;

#endif
