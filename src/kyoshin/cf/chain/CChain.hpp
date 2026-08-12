#pragma once

#include <types.h>
#include "kyoshin/cf/chain/CChainActorList.hpp"
#include "kyoshin/cf/chain/CChainMember.hpp"
#include "kyoshin/cf/chain/CChainTimer.hpp"
#include "kyoshin/cf/chain/CChainTime.hpp"
#include "kyoshin/cf/chain/CChainChance.hpp"
#include "kyoshin/cf/chain/CChainCombo.hpp"

namespace cf {
    //size: 0x1F0C?
    class CChain {
    public:
        CChain();
        ~CChain(){}
        void func_8027728C();
        bool chkActorList();

        u8 unk0[0x18];
        CChainActorList mChainActorList; //0x18
        CChainMember mChainMember; //0x1DC8
        CChainTimer mChainTimer1; //0x1EB4
        CChainTimer mChainTimer2; //0x1EBC
        CChainTime mChainTime; //0x1EC4
        CChainChance mChainChance; //0x1EDC
        CChainCombo mChainCombo; //0x1EF4
        u8 unk1F0C[0x14];
    };

    // Tail view of CChainChance (its trailing unk14[4] region) as a signed
    // accumulator: retail func_80277A7C reads/writes it with lwz/stw at
    // CChain+0x1EF0 (chain gauge accumulation, clamped at 0x98967F).
    struct CChainChanceTail {
        u8 field_0[0x14]; //0x0
        s32 field_0x14;   //0x14
    };

    // View of the battle manager tail for func_802795D4: s16 state word at
    // +0x20C8 (retail lha) and the sudden-commu sub-object at +0x216C
    // (address taken by func_801BA2C8).
    struct CBattleManagerTail {
        u8 field_0[0x20C8];            //0x0
        s16 field_0x20C8;              //0x20C8
        u8 field_0x20CA[0x216C - 0x20CA];
        u8 field_0x216C[0];            //0x216C (address taken)
    };

    // Battle-object view for func_80279F6C (chain-actor unk0 target): move
    // sub-object pointer at +0x4 (vtable slot 0x20 called with a flag), the
    // voice-act sub-object at +0x3380 (address taken by func_801537E0), and
    // a u16 flag word at +0x3388.
    class CChainBattleObjF {
    public:
        u8 field_0[0x4];       //0x0
        CChainSub4* field_4;   //0x4
        u8 field_8[0x3380 - 0x8];
        u8 field_0x3380;       //0x3380
        u8 field_0x3381[0x3388 - 0x3381];
        u16 field_0x3388;      //0x3388
    };

    // Interface for the chain-actor manual vtable at +0x70: declared virtual
    // #23 lands at vtable byte offset +0x64 (func_80279694 dispatches
    // (self,key) there). Same pad-first / -RTTI layout trick as
    // CChainActorVtIf: the vptr sits after the 0x70 pad.
    class CChainActorVtIf64 {
    public:
        u8 pad70[0x70];
        virtual void v000() = 0;
        virtual void v001() = 0;
        virtual void v002() = 0;
        virtual void v003() = 0;
        virtual void v004() = 0;
        virtual void v005() = 0;
        virtual void v006() = 0;
        virtual void v007() = 0;
        virtual void v008() = 0;
        virtual void v009() = 0;
        virtual void v010() = 0;
        virtual void v011() = 0;
        virtual void v012() = 0;
        virtual void v013() = 0;
        virtual void v014() = 0;
        virtual void v015() = 0;
        virtual void v016() = 0;
        virtual void v017() = 0;
        virtual void v018() = 0;
        virtual void v019() = 0;
        virtual void v020() = 0;
        virtual void v021() = 0;
        virtual void v022() = 0;
        virtual void v023(int key) = 0; // slot 25 / +0x64
    };

    // Battle-object view for func_80279A4C: base vtable at +0x0 (declared
    // virtual #167 lands at +0x2A4, returning the arts-category holder),
    // move sub-object pointer at +0x4, battle id at +0x3F28.
    class CChainBattleObj2A4 {
    public:
        virtual void v000() = 0; virtual void v001() = 0; virtual void v002() = 0; virtual void v003() = 0;
        virtual void v004() = 0; virtual void v005() = 0; virtual void v006() = 0; virtual void v007() = 0;
        virtual void v008() = 0; virtual void v009() = 0; virtual void v010() = 0; virtual void v011() = 0;
        virtual void v012() = 0; virtual void v013() = 0; virtual void v014() = 0; virtual void v015() = 0;
        virtual void v016() = 0; virtual void v017() = 0; virtual void v018() = 0; virtual void v019() = 0;
        virtual void v020() = 0; virtual void v021() = 0; virtual void v022() = 0; virtual void v023() = 0;
        virtual void v024() = 0; virtual void v025() = 0; virtual void v026() = 0; virtual void v027() = 0;
        virtual void v028() = 0; virtual void v029() = 0; virtual void v030() = 0; virtual void v031() = 0;
        virtual void v032() = 0; virtual void v033() = 0; virtual void v034() = 0; virtual void v035() = 0;
        virtual void v036() = 0; virtual void v037() = 0; virtual void v038() = 0; virtual void v039() = 0;
        virtual void v040() = 0; virtual void v041() = 0; virtual void v042() = 0; virtual void v043() = 0;
        virtual void v044() = 0; virtual void v045() = 0; virtual void v046() = 0; virtual void v047() = 0;
        virtual void v048() = 0; virtual void v049() = 0; virtual void v050() = 0; virtual void v051() = 0;
        virtual void v052() = 0; virtual void v053() = 0; virtual void v054() = 0; virtual void v055() = 0;
        virtual void v056() = 0; virtual void v057() = 0; virtual void v058() = 0; virtual void v059() = 0;
        virtual void v060() = 0; virtual void v061() = 0; virtual void v062() = 0; virtual void v063() = 0;
        virtual void v064() = 0; virtual void v065() = 0; virtual void v066() = 0; virtual void v067() = 0;
        virtual void v068() = 0; virtual void v069() = 0; virtual void v070() = 0; virtual void v071() = 0;
        virtual void v072() = 0; virtual void v073() = 0; virtual void v074() = 0; virtual void v075() = 0;
        virtual void v076() = 0; virtual void v077() = 0; virtual void v078() = 0; virtual void v079() = 0;
        virtual void v080() = 0; virtual void v081() = 0; virtual void v082() = 0; virtual void v083() = 0;
        virtual void v084() = 0; virtual void v085() = 0; virtual void v086() = 0; virtual void v087() = 0;
        virtual void v088() = 0; virtual void v089() = 0; virtual void v090() = 0; virtual void v091() = 0;
        virtual void v092() = 0; virtual void v093() = 0; virtual void v094() = 0; virtual void v095() = 0;
        virtual void v096() = 0; virtual void v097() = 0; virtual void v098() = 0; virtual void v099() = 0;
        virtual void v100() = 0; virtual void v101() = 0; virtual void v102() = 0; virtual void v103() = 0;
        virtual void v104() = 0; virtual void v105() = 0; virtual void v106() = 0; virtual void v107() = 0;
        virtual void v108() = 0; virtual void v109() = 0; virtual void v110() = 0; virtual void v111() = 0;
        virtual void v112() = 0; virtual void v113() = 0; virtual void v114() = 0; virtual void v115() = 0;
        virtual void v116() = 0; virtual void v117() = 0; virtual void v118() = 0; virtual void v119() = 0;
        virtual void v120() = 0; virtual void v121() = 0; virtual void v122() = 0; virtual void v123() = 0;
        virtual void v124() = 0; virtual void v125() = 0; virtual void v126() = 0; virtual void v127() = 0;
        virtual void v128() = 0; virtual void v129() = 0; virtual void v130() = 0; virtual void v131() = 0;
        virtual void v132() = 0; virtual void v133() = 0; virtual void v134() = 0; virtual void v135() = 0;
        virtual void v136() = 0; virtual void v137() = 0; virtual void v138() = 0; virtual void v139() = 0;
        virtual void v140() = 0; virtual void v141() = 0; virtual void v142() = 0; virtual void v143() = 0;
        virtual void v144() = 0; virtual void v145() = 0; virtual void v146() = 0; virtual void v147() = 0;
        virtual void v148() = 0; virtual void v149() = 0; virtual void v150() = 0; virtual void v151() = 0;
        virtual void v152() = 0; virtual void v153() = 0; virtual void v154() = 0; virtual void v155() = 0;
        virtual void v156() = 0; virtual void v157() = 0; virtual void v158() = 0; virtual void v159() = 0;
        virtual void v160() = 0; virtual void v161() = 0; virtual void v162() = 0; virtual void v163() = 0;
        virtual void v164() = 0; virtual void v165() = 0; virtual void v166() = 0;
        virtual CChainCombo_ArtsCategoryHolder* v167() = 0; // index 167 -> +0x2A4

        CChainSub4* field_04;        //0x04 move sub-object (slot 0x30 -> word holder)
        u8 _pad08[0x3F28 - 0x08];
        u16 field_3F28;              //0x3F28 battle id
    };

    // Arts-category object probed by func_80279A4C at +0x77 (retail lbz).
    struct CChainArtsCat77 {
        u8 field_0[0x77];
        u8 field_0x77;   //0x77
    };

    // Layout mirror of CChain used by func_80279694: the member list viewed
    // as a CChainList member so member-array accesses fold the 0x1DC8 offset
    // into the load (a C-style cast of &self->mChainMember would materialize
    // the base into a register first).
    struct CChainMemberListMirror {
        u8 unk0[0x18];
        CChainActorList mChainActorList; //0x18
        CChainList mChainMember;         //0x1DC8
        u8 unk1EB4[0x1F0C - 0x1EB4];
    };
}

// Forward decls for the CChain tail overlays (full defs live in
// CMenuBattleChain.hpp / CUIErrMesWin.hpp; func_8027728C only passes their
// addresses through to the reset helpers).
struct CBattleChainMenuState;
struct CErrMesEntry;

// func_80276D30 is defined in this TU (C linkage inherited from the extern
// "C" block below, so both the definition and the call reloc stay unmangled).

// Chain-voice data symbols (.sdata float / .sbss byte).
extern float lbl_eu_80668A18;
extern u8 lbl_eu_80663DA0;

// Chain-voice global state (retail .sdata lbl_eu_80662A20): -1 is the
// "not loaded" sentinel. The func_80276C*/func_8027715* helpers probe it with
// the unsigned (value + 0x10000) == 0xffff sentinel test (MWCC addis/cmplwi
// idiom, cf. func_80190394 in code_8018F8D8.cpp).
extern s32 lbl_eu_80662A20;

// C-linkage callees (retail symbols are unmangled globals - C linkage keeps
// the call relocs verbatim). Voice-manager helpers are defined in
// kyoshin/cf/voice/CCharVoiceMan.cpp; the chain-actor-list helpers in
// CChainActorList.cpp; the chain-menu / err-mes helpers in
// CMenuBattleChain.cpp / CUIErrMesWin.cpp.

// Circular gimmick object list consumed by func_800B6BC8 (layout mirror of
// the CfGimmick.hpp structs - that header's func_800B3A88 C-import clashes
// with CChainEffect.hpp's void* variant, so its func_800B6BC8 C-import and
// the list structs are mirrored here instead).
struct CChainGimmickListNode {
    CChainGimmickListNode* next; //0x00
    u8 gap04[0x08 - 0x04];
    void* object;               //0x08
};
struct CChainGimmickList {
    void* field_00;             //0x00
    CChainGimmickListNode* head; //0x04
};

// CfGameManager mode-flag word (.sbss; sda21 access).
extern u32 lbl_eu_80663E28;

extern "C" {
    void func_802A1500();
    void func_802A35B8(u32 arg);
    int func_802A3748(u32 arg);
    int func_802A3214();
    int func_802A3680(int a, CChainBattleObjTail* b, int c);
    void func_8027B164(cf::CChainActorList* self);
    cf::CChainActor* func_8027CA98(cf::CChainList* self, u32 key);
    void func_8027C45C(cf::CChainList* self);
    void func_802AB3D0(CBattleChainMenuState* self);
    u8* func_802B48A0(CErrMesEntry* self);
    int func_80276D30(u8* self);
    // Same-TU voice reset helper (defined in CChain.cpp); C linkage keeps
    // the call reloc from func_8027732C verbatim.
    void func_80276C58();
    // Chain-actor-list helpers (CChainActorList.cpp). C linkage keeps the
    // call reloc names verbatim (MWCC otherwise mangles global functions).
    void func_8027B200(cf::CChainActorList* self);
    void func_8027B2CC(cf::CChainActorList* self);
    void func_8027C49C(cf::CChainList* self);
    void func_8027C560(cf::CChainList* self);
    void func_8027C0B0(cf::CChainChanceS* self);
    int  func_8027B814(cf::CChainActorList* self, u32 key);
    int  func_8027BC14(cf::CChainActorList* self, u32 key);
    int  func_8027C5E4(cf::CChainList* self, u32 key);
    // Same-TU helper (defined in CChain.cpp after its first caller).
    void func_80277B38(cf::CChain* self);
    // Chain-menu / err-mes helpers (CMenuBattleChain.cpp / CUIErrMesWin.cpp).
    void func_802AB410(CBattleChainMenuState* self);
    void func_802B48B8(CErrMesEntry* self);
}
