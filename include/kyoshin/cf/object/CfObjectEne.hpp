#ifndef CF_OBJECT_ENE_HPP
#define CF_OBJECT_ENE_HPP

#include "kyoshin/cf/CfBdat.hpp"
#include "kyoshin/cf/object/CfObjectMoveHelpers.hpp"
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

// size: 0x45D0
//
// Retail __vt__Q22cf11CfObjectEne (0x5E4 bytes, four groups like
// CfObjectActor's). Overrides of CfObjectMove-chain slots get an @16028@
// thunk in the +0x3E9C group plus a direct slot appended at the tail
// (0x5D4..0x5DC, in declaration order); the destructor reuses the
// CfObjectActor tail slot 0x5A0, and the two CActorParam overrides reuse
// their offset-0 slots.
class CfObjectEne : public CfObjectActor {
public:
    CfObjectEne();
    virtual ~CfObjectEne();                       //0x5A0 (CfObject slot 0x54)
    virtual int CfObject_UnkVirtualFunc2();       //0x5D4 (CfObject slot 0x58: initialise)
    virtual void CfObject_UnkVirtualFunc4();      //0x5D8 (CfObject slot 0x60: per-frame update)
    virtual void CObjectParam_UnkVirtualFunc4();  //0x5DC (CObjectParam slot 0x48: load parameters)
    virtual void initEnemyBdatParams();           //0x5E0
    virtual void CActorParam_UnkVirtualFunc166(); //0x32C (arts-set loader)
    virtual void CActorParam_UnkVirtualFunc167(); //0x330 (attack-set loader)

    void applyEnemyReward();
    u32 getEneBdatColumn(const char* column);
    u32 getEneBdatByIndex(const char* col, u32 index);

    //0x0000-0x45C0: CfObjectActor
    u16 field_0x45c0;
    u16 field_0x45c2;
    u16 field_0x45c4;
    u16 field_0x45c6;
    u16 field_0x45c8;
    u16 field_0x45ca;
    f32 field_0x45cc;
};

// Absolute-offset view of the u16 BDAT row id at self+0x456C (inside the
// CfObjectMove subobject region). func_800AF7E4 shifts it right 4 to key the
// getBdatStringColumnValue lookup; sibling views CfPartsFlagView /
// CfPartsChgObj read the same word.

// Block view anchored at CAIAction+4 (and at CfObjectActor+0x3380+4 in the
// CfObjectActor dtor's inlined copy). The CAIAction dtor null-checks the
// block address AND the +8 address (MWCC folds both into addic.-beq guards;
// see MWCC_CASES CacheList pattern), then zeroes the +0x20C/+0x210
// words (= CAIAction unk210/unk214, store order 0x210 first).

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
struct CfActorObj89CView {
    u8 _pad[0x89C];
    u32 field_0x89C; // 0x89C
};

// Absolute-offset views of CActorParam byte fields used by the CfObjectEne
// CActorParam getter/setter stubs. The CActorParam member block is +8 shifted
// by the base-class layout (see CActorParamGaugeView in CActorParam.hpp), so
// raw views pin the retail offsets.


// Absolute-offset view of the CActorParam byte at 0x15F4 (the CActorParam
// member block is +8 shifted by the base-class layout, so the declared
// unk15F4[4] member lands at 0x15FC; raw view pins the retail offset). Used
// by CActorParam_UnkVirtualFunc15.

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

    // --- CfObjectEne ctor/dtor raw views ---
    // The header's C++ MI layout is +0x80 off retail (see the CfObjectActor
    // dtor note in CfObjectEne.cpp), so the ctor/dtor reach the subobject
    // vtable slots, the reload child pointer and the tail fields through raw
    // views pinning the retail offsets.

    // Offset base for the thin-MI real-owner views below (no virtuals,
    // no slots - just the byte offset so this+0x3E9C dispatch folds).
    struct CfEneMoveOff {
        u8 _pad[0x3E9C];
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

    // +0x44A8 sub-object touched at the end of initEnemyBattleState (the u16 at
    // +0xB2 receives 0xC8).
    struct CfEneReloadArea {
        u8 _pad[0xB2];
        u16 field_0xB2; // 0xB2
    };

    // Name/state fields used by initEnemyBdatParams: the bdat lookup name at
    // +0x3F14 (getFP arg) and the u16 row id at +0x3F28.

    // Distinct absolute-offset views of the +0x3E9C CfObjectMove subobject
    // base, one per plain (non-virtual) call site in initEnemyBattleState so MWCC
    // re-materializes `addi r3, r31, 0x3e9c` per call instead of CSE-ing the
    // address into a callee-saved register (which would grow the frame).
    // NOTE: distinct view types do NOT stop MWCC from CSE-ing these
    // addresses across calls in initEnemyBattleState (value numbering ignores
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

    // Object behind CfObjectActor's +0x3F60 pointer (applyEnemyReward): the
    // model-id word at +0x4B0 and the +0x4EC flag word (bit 8 gates).
    struct CfEneMoveTgtView {
        u8 _pad[0x4B0];
        s32 field_0x4B0;           // 0x4B0 model id (signed compare)
        u8 _pad4B4[0x4EC - 0x4B4]; // 0x4B4
        u32 field_0x4EC;           // 0x4EC flags (bit 8)
    };


    // doubles (same convention as CfActorF64Conv in CfObjectActor.hpp).

    // applyEnemyReward reaches all of the battle manager through the real
    // cf::CBattleManager class declared in kyoshin/cf/CBattleManager.hpp:
    //   mChain.mHead.field_2  (+0x1AA) party/phase count,
    //   unk20C8.unk0          (+0x20C8) mode selector,
    //   mSuddenCommu          (+0x216C) SuddenCommu state buffer.
    // The former CfEneBmView overlay duplicated those three absolute offsets,
    // so it was removed; callers now name the real members directly.
    //
    // The +0x48 mActorList3 head is read in CfObjectPc.cpp through the same
    // public battle-manager layout (reslist::mStartNodePtr).
    // No layout or vtable change results from removing this former overlay.
    // Addressable raw-column holder for initEnemyBdatParams: full-word store of the
    // getBdatStringColumnValue result, punned u16 read on reload (retail's
    // stw-then-lhz pair around the slot +0xF0 vcall).
    union CfEneColNarrow {
        u32 w;
        u16 h;
        u8 b;
    };

    // loadEnemyParams / initEnemyBdatParams dispatch through the owning-class
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

    // --- loadEnemyParams views ---

    // Battle-gauge/rates region behind the primary vtable slots +0x1C4
    // (float at +0x0) and +0x20C (floats at +0x44/+0x48; the loadEnemyParams
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

    // Primary param-region fields written by loadEnemyParams: the u32 at
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

    // 0x34-byte stack buffer built by loadEnemyParams and passed to the +0x8
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

    // loadEnemyParams slot map (all dispatched through the owning-class
    // CActorParam aliases; no TU-local proxy remains): +0xDC/+0xE4 (u8 arg),
    // +0xE8/+0x198/+0x1A4/+0x1BC/+0x334 (float arg), +0x170/+0x188/+0x25C
    // (int arg), +0x21C (ptr arg), +0x254 (u32 arg), +0x1C4/+0x20C (rates),
    // +0x27C (arts-set base), +0x288 (arts-slot u16 array), +0x2BC (guard),
    // +0x32C/+0x330 (loaders 166/167), +0x5E0 (no-arg).

};


// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)

// C-ABI free-function import: retail CfGameManager "party count" getter
// (Fv-mangled member name; returns the u32 at ::cf::CfBdat::spCurRouteListFileData). Declared
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

// Verbatim `__`-named free base ctor and the CfResReloadImpl ctor
// (defined in CfObjectActor.cpp / CfResReloadImpl.cpp as plain global
// functions). extern "C" keeps the call-site relocs at the retail names
// (a plain C++ declaration makes MWCC append a __F<params> mangling; same
// convention as CfCam.hpp / CTaskGame.hpp).
extern "C" cf::CfResReloadImpl* __ct__cf_CfResReloadImpl(cf::CfResReloadImpl* self,
                                                          cf::CfResReloadParent* parent);

// C-ABI imports used by the CfObjectEne ctor (func_80061FFC + MemManager),
// initEnemyBattleState and the dtor (retail symbols are unmangled).
extern "C" unsigned long func_80061FFC();
extern "C" void CfObjectMove_setRegionAttached(void* obj, int flag);
// func_80174B4C is declared in CfObjectActor.hpp with its full 5-arg ABI
// signature; a second C-linkage declaration here would be an illegal overload.
extern "C" void func_804B0AD4(void* obj, int param, float a, float b);
extern "C" void func_801F4DDC(void* obj, void* actor);
extern "C" void func_801F4D50(void* obj, void* actor);
// getUnk80664658: declared once in kyoshin/code_801F3BE0.hpp (owner).

// C-ABI imports used by initEnemyBdatParams / updateEnemyBattleState / applyEnemyReward
// (retail symbols are unmangled).
// Bdat_GetRowBase_B41C / Bdat_GetMaxRow_B1EC: canonical form per ocBdat.cpp definitions
// (u32(void*)); CfObjectPc.hpp's block uses the same spelling.
extern "C" void* Bdat_GetTable_AA34();
extern "C" u32 Bdat_GetRowBase_B41C(void* bdat);   // bdat first row (must match CfObjectPc.hpp)
extern "C" u32 Bdat_GetMaxRow_B1EC(void* bdat);   // bdat row count (must match CfObjectPc.hpp)
extern "C" void func_8014B7B0(u8* obj);
extern "C" void func_8015396C(u8* obj, u32 a, u32 b);
// func_80174C98 / func_80174B4C are owned by kyoshin/cf/CfMapItemManager.hpp.
extern "C" void CActorParam_updateTick(void* actor, f32 value, u32 arg);
extern "C" unsigned long func_801BA2C8(void* self);
// func_80148778 and getInstance__Q22cf14CBattleManagerFv are declared in
// CAIAction.hpp / kyoshin/cf/CBattleManager.hpp (reachable via
// CfObjectActor.hpp) - keep
// the shared declarations (C-linkage overloads with different pointer types
// are illegal in MWCC).

// Verbatim-name free function (defined in CfObjectEne.cpp; updateEnemyBattleState
// tail-dispatches the bdat-match path through it). extern "C" keeps the
// call-site reloc at the unmangled retail name applyEnemyReward.

// C-ABI imports used by the CfObjectEne arts/attack loaders
// (CActorParam_UnkVirtualFunc166/167; defined in CArtsSet.cpp and
// CAttackSet.cpp with unmangled retail names).
extern "C" void* getAtkParam(void* base, int index);                  // attack-param record lookup
// arts slot value at row*0x10 + col*0x2 + 4 (unsigned short result)
extern "C" unsigned short func_80153CAC(const void* base, int index);
extern "C" void* getArtsParamByIdx(void* base, int index);           // arts-param record lookup
extern "C" void setArtsSlotByIdx(void* base, unsigned short value, int index); // arts slot write


// applyEnemyReward drop-channel tables (.sbss):
// u16 mode word at 0x80663E42 and the per-mode channel halfword table whose
// entries are addressed at byte offset mode*100 from 0x80663E44.
extern u16 gmGlobalFlag42;
extern u16 gmGlobalFlag44;


// ml::rad2deg / ml::deg2rad are already declared `const float` in
// CfObjectModel.hpp (in the CfObjectActor.hpp chain) - reuse those.

// .sbss bdat table pointers used by CObjectParam_UnkVirtualFunc4.
extern u8* lbl_eu_8066409C;

#endif
