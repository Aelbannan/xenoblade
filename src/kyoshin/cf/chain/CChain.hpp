#pragma once

#include <types.h>
#include "kyoshin/cf/chain/CChainActorList.hpp"
#include "kyoshin/cf/chain/CChainMember.hpp"
#include "kyoshin/cf/chain/CChainTimer.hpp"
#include "kyoshin/cf/chain/CChainTime.hpp"
#include "kyoshin/cf/chain/CChainChance.hpp"
#include "kyoshin/cf/chain/CChainCombo.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// Retail .data vtable mirrors assigned by the inlined sub-object ctors below.
// Full-size (0x10) so MWCC emits the lis/addi HA-LO pair instead of sda21.
struct CChainVtSym { u8 bytes[0x10]; };
extern CChainVtSym lbl_eu_80538284;
extern CChainVtSym lbl_eu_80538278;
extern CChainVtSym lbl_eu_8053826C;

extern "C" void func_8027C45C(cf::CChainList* self);

namespace cf {
    // Inlined-construction mirrors for the CChain tail sub-objects. Retail
    // initializes each with a tiny inlined ctor that stores its .data vtable
    // pointer (and zeroes the leading counter), so these are plain structs
    // with manual vptr fields, not polymorphic classes. The implicit CChain
    // ctor then emits the retail construction order exactly (member -> two
    // timers -> chance interleaved between the out-of-line actor-list / time /
    // combo ctors).

    // 0xEC: opaque body + vtable at +0xE8; ctor also resets the list via
    // func_8027C45C.
    class CChainMemberInit {
    public:
        u8 unk0[0xE8];   //0x00
        void* mVtbl;     //0xE8

        CChainMemberInit() {
            mVtbl = &lbl_eu_80538284;
            func_8027C45C((CChainList*)this);
        }
    };

    // 0x8: leading s16 counter + vtable at +0x4.
    class CChainTimerInit {
    public:
        s16 unk0;        //0x0 (read/written signed by chain code)
        u8 unk2[2];
        void* mVtbl;     //0x4

        CChainTimerInit() {
            mVtbl = &lbl_eu_80538278;
            unk0 = 0;
        }
    };

    // 0x18: counter, an embedded timer-vtable pointer at +0x4, and its own
    // vtable at +0x10.
    class CChainChanceInit {
    public:
        s16 mChainCount;  //0x0
        u8 unk2[2];
        void* mTimerVtbl; //0x4
        u8 unk8[8];
        void* mVtbl;      //0x10
        s32 field_14;     //0x14 chain gauge accumulator (signed)

        CChainChanceInit() {
            mTimerVtbl = &lbl_eu_80538278;
            mVtbl = &lbl_eu_8053826C;
            mChainCount = 0;
        }
    };

    //size: 0x1F0C?
    class CChain {
    public:
        CChain();
        ~CChain(){}
        void func_8027728C();
        bool chkActorList();

        u8 unk0[0x18];
        CChainActorList mChainActorList; //0x18
        CChainMemberInit mChainMember; //0x1DC8
        CChainTimerInit mChainTimer1; //0x1EB4
        CChainTimerInit mChainTimer2; //0x1EBC
        CChainTime mChainTime; //0x1EC4
        CChainChanceInit mChainChance; //0x1EDC
        CChainCombo mChainCombo; //0x1EF4
        u8 unk1F0C[0x14];
    };

    // View of the battle manager tail for func_802795D4:
    // +0x20C8 (retail lha) and the sudden-commu sub-object at +0x216C
    // (address taken by func_801BA2C8).
    struct CBattleManagerTail {
        u8 field_0[0x20C8];            //0x0
        s16 field_0x20C8;              //0x20C8
        u8 field_0x20CA[0x216C - 0x20CA];
        u8 field_0x216C[0];            //0x216C (address taken)
    };

    // Battle-object / voice-sub-object slots folded onto their owners
    // (cf::CChainBattleObj / CChainVoiceSub / CChainSub8 / CChainSub4 in
    // CChainTimer.hpp): v042/v157/v162/v164/v167/v168/v173/v363,
    // field_04/field_0x3388(+mField3380)/field_358C-98/field_0x3E98/mSub8/
    // mSub(v01/v17/v18/v41/v066)/field_3F28/field_3F60. The TU-local
    // vtable-view classes are deleted; call sites use the owners directly.

    struct CChainArtsCat77 {
        u8 field_0[0x77];
        u8 field_0x77;   //0x77
    };

    // Timing-result object returned by the embedded voice sub-object's
    // vtable slot 0x110 (func_8027A8C8 compares its +0x14 float against
    // lbl_eu_80668A60).
    class CChainVoiceRes {
    public:
        u8 _pad0[0x14];
        f32 field_14;           // 0x14
    };

    // Holder for func_8027A8C8:
    // loads the battle object through this holder before probing slot
    // 0x110 / the move sub-object / +0x3F60).
    class CChainVoiceHolder {
    public:
        CChainBattleObj* field_0; // 0x0
    };

    // 32-byte table entry probed by func_80279214 at +0xD (signed: retail
    // compares the byte with cmpwi).
    class CChainArt214Entry {
    public:
        u8 field_0[0xD];
        s8 field_0xD;              // 0xD
    };

    // Gauge sub-object returned by func_8027750C's slot-0x298 dispatch: the
    // +9 state byte selects the two-round (0x4a/0x4b) or one-round (0x40)
    // gauge update, floats live at +0x58/+0x5c/+0x60 and the +0x74 bit 24
    // disables the integer truncation.
    class CChainGaugeSub {
    public:
        u8 _pad0[0x9];
        u8 field_9;              //0x9
        u8 _padA[0x58 - 0xA];
        f32 field_58;            //0x58
        f32 field_5C;            //0x5C
        f32 field_60;            //0x60
        u8 _pad64[0x74 - 0x64];
        u32 field_74;            //0x74
    };

    // Count byte probed by func_8027750C at +0x44 of the object reached
    // through the arts-holder's +0x50 pointer.
    struct CChainGaugeCnt44 {
        u8 pad[0x44];
        u8 field_0x44;           //0x44
    };

    // Layout mirror of CChain used by func_80279694:
    // as a CChainList member so member-array accesses fold the 0x1DC8 offset
    // into the load (a C-style cast of &self->mChainMember would materialize
    // the base into a register first).
    struct CChainMemberListMirror {
        u8 unk0[0x18];
        CChainActorList mChainActorList; //0x18
        CChainList mChainMember;         //0x1DC8
        u8 unk1EB4[0x1F0C - 0x1EB4];
    };

    // Head-field view of CChain used by func_80277B38's state machine: the
    // unk0[0x18] byte region with typed fields (s8 index bytes, u8 state/flag
    // bytes, the u32 arts-index word at +0xC and the f32 gauge at +0x14).
    struct CChainHeadView {
        s8 field_0;      //0x0 current member index
        s8 field_1;      //0x1 last member index
        u8 field_2;      //0x2 chain state
        u8 field_3;      //0x3
        u8 field_4;      //0x4
        u8 field_5;      //0x5 chain-direction flag
        u8 field_6;      //0x6
        u8 field_7;      //0x7
        u8 field_8;      //0x8
        u8 field_9;      //0x9
        u8 field_A;      //0xA
        u8 field_B;      //0xB
        u32 field_C;     //0xC arts index
        u8 field_10;     //0x10
        u8 field_11[3];  //0x11
        f32 field_14;    //0x14 chain gauge
    };

    // 0x20-byte stack scratch written by func_80277B38 case 5: the 0xe-byte
    // memset target at +0x4 (retail 0x1c absolute) overlaps the field bytes
    // written after both memsets (+0x6/+0xD/+0x10 are inside it, +0x12 and
    // +0x14 just past), so the scratch is one local, not two.
    struct CChainScratch20 {
        u8 field_0[0x4]; //0x0
        union {
            u8 mScratch[0xE]; //0x4 (memset target)
            struct {
                u8 field_4[2]; //0x4
                u8 field_6;    //0x6
                u8 field_7[6]; //0x7
                u8 field_D;    //0xD
                u8 field_E[2]; //0xE
                s16 field_10;  //0x10
            } mFields;
        };
        s16 field_12;     //0x12
        f32 field_14;     //0x14
        u8 field_18[0x8]; //0x18
    };
    // 0xe-byte sub-region of CChainScratch20 at +0x4 (standalone view).
    struct CChainScratch0E {
        u8 bytes[0xe];
    };
}

// Forward decls for the CChain tail overlays (full defs live in
// CMenuBattleChain.hpp / CUIErrMesWin.hpp; func_8027728C only passes their
// addresses through to the reset helpers).
struct CBattleChainMenuState;
struct CErrMesEntry;
struct CErrMesOwner;

// func_80276D30 is defined in this TU (C linkage inherited from the extern
// "C" block below, so both the definition and the call reloc stay unmangled).
// Its battle-object params use cf::CChainBattleObj (field_3F28 chain type).

// Chain-voice data symbols (.sdata float / .sbss byte).
extern float lbl_eu_80668A18;
extern float lbl_eu_80668A1C;
extern float lbl_eu_80668A40;
extern float lbl_eu_80668A44;
extern float lbl_eu_80668A48;
extern float lbl_eu_80668A50;
extern float lbl_eu_80668A54;
extern u8 lbl_eu_80663DA0;
// Battle-manager mode-flag word (.sbss) probed by func_80277B38's chain-start
// gate (bit 22). Same declaration as CBattleManager.hpp's extern "C" row.

// Voice-timing threshold compared against the voice sub-object's slot-0x110
// result by func_8027A8C8 (.sdata2 float; const lets MWCC schedule the
// pool load at retail's position, cf. MWCC_CASES SDA-pool hoist).
extern const f32 lbl_eu_80668A60;

// Chain facing-gate constants used by func_8027A58C (.sdata2 floats): the
// Atan2FIdx scale, the +/- normalization window around the gauge angle, and
// the two factors of the facing tolerance product.
extern const f32 lbl_eu_80668A58;
extern const f32 lbl_eu_80668A5C;
extern const f32 lbl_eu_8066A1F8;
extern const f32 lbl_eu_8066A1FC;
extern const f32 lbl_eu_8066A210;

// Chain-voice global state (retail .sdata lbl_eu_80662A20): -1 is the
// "not loaded" sentinel. The func_80276C*/func_8027715* helpers probe it with
// the unsigned (value + 0x10000) == 0xffff sentinel test (MWCC addis/cmplwi
// idiom, cf. CfCmd_Fwd16IfItem in code_8018F8D8.cpp).
extern s32 lbl_eu_80662A20;

// Chain-voice id tables picked by func_80276D30 (.sdata int arrays -> sda21
// addressing; lbl_eu_805381C8 is a 3-entry .data table -> lis/addi).
extern int lbl_eu_80662A24[1];
extern int lbl_eu_80662A28[2];
extern int lbl_eu_80662A30[2];
extern int lbl_eu_80662A38[2];
extern int lbl_eu_805381C8[3];

// C-linkage callees (retail symbols are unmangled globals - C linkage keeps
// the call relocs verbatim). Voice-manager helpers are defined in
// kyoshin/cf/voice/CCharVoiceMan.cpp; the chain-actor-list helpers in
// CChainActorList.cpp; the chain-menu / err-mes helpers in
// CMenuBattleChain.cpp / CUIErrMesWin.cpp.

// Circular gimmick object list consumed by getReslistB48 (layout mirror of
// the CfGimmick.hpp structs - that header's func_800B3A88 C-import clashes
// with CChainEffect.hpp's void* variant, so its getReslistB48 C-import and
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

// Gauge-delta literals consumed by func_8027750C (.sdata float / doubles).
extern float lbl_eu_80668A30;
extern double lbl_eu_80668A20;
extern double lbl_eu_80668A28;

// CfGameManager mode-flag word (.sbss; sda21 access).
extern u32 lbl_eu_80663E28;

namespace cf { class CfObjectActor; }

extern "C" {
    void func_802A1500();
    u32 func_8009CF8C(u32 resourceId);
    void func_802A35B8(u32 arg);
    int func_802A3748(u32 arg);
    int CCharVoiceMan_AllocChainVoiceId();
    int func_802A3680(int a, CChainBattleObjTail* b, int c);
    void func_8027B164(cf::CChainActorList* self);
    cf::CChainActor* func_8027CA98(cf::CChainList* self, u32 key);
    void func_8027C45C(cf::CChainList* self);
    void func_802AB3D0(CBattleChainMenuState* self);
    u8* func_802B48A0(CErrMesEntry* self);
    int func_80276D30(int mode, cf::CChainBattleObj* p1, cf::CChainBattleObj* p2);
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
    // Chain-start helpers used by func_8027936C / func_80278E0C
    // (CChainActorList.cpp / CMenuBattleChain.cpp).
    void func_8027BA0C(cf::CChainActorList* self, cf::CChainList* other, cf::CfObjectActor* target);
    void func_8027C924(cf::CChainList* self, int target);
    void func_8027C6B4(cf::CChainList* self, int target, int index);
    void func_8027BF58(cf::CChainFlag* self);
    void func_8027C040(cf::CChainFlag* self);
    void func_802AB474(CBattleChainMenuState* self);
    // Presentation/event flag-bit probe (code_800F42AC.cpp; retail symbol
    // carries the Fi suffix) and battle-manager sub-check (CBattleManager).
    bool func_8006EF04__Fi(int mask);
    bool isGlobalCamFlagSet__Fi(int mask);
    void* func_800EA444(void* bm);
    // Arts-select slot probe (CMenuArtsSelect.cpp): unmangled at the retail
    // call site, so declared here with C linkage rather than through the
    // class member (which mangles to func_80107C54__15CMenuArtsSelectFl).
    int func_80107C54(void* menu, int index);
    // Arts-menu-up query (CtrlPc.cpp family) used by func_8027A58C.
    int func_801B202C();
    // Battle-event dispatcher (defined in CBattleManager.cpp): single shared
    // decl on kyoshin/cf/CBattleManagerApi.hpp (extern "C", retail-unmangled
    // symbol); the C++-linkage copy here clashed with it (10197).
    // Voice-act / chain arts-combo helpers. func_801537F0 lives in
    // CVision.hpp (same signature, no clash); func_8014AE00 and
    // func_80293E24 have no header declaration, so they are declared here.
    void func_801537F0(void* self);
    void func_8014AE00(void* self);
    void func_80293E24(cf::CChainCombo* self, cf::CfObjectActor* actor);
    // Same-TU helper (defined in CChain.cpp after its first caller).
    void func_80277B38(cf::CChain* self);
    // Same-TU chain-voice reset (defined in CChain.cpp); used by the ctor.
    void func_8027711C(void* self);
    // Chain-menu / err-mes helpers (CMenuBattleChain.cpp / CUIErrMesWin.cpp).
    void func_802AB410(CBattleChainMenuState* self);
    void func_802B48B8(CErrMesEntry* self);
    // Error-message record register helpers (CUIErrMesWin.cpp) - same family
    // as func_802B48A0 / func_802B48B8 above (chain-state codes 1/2/3).
    void func_802B48E4(CErrMesEntry* self, CErrMesOwner* owner);
    void func_802B4968(CErrMesEntry* self, CErrMesOwner* owner);
    void func_802B4A68(CErrMesEntry* self, CErrMesOwner* owner);
    // Battle-chain menu helpers (CMenuBattleChain.cpp).
    void func_802AB5E4(CBattleChainMenuState* self);
    int func_802AB510(CBattleChainMenuState* self, u8* out);
    // Chain-state-machine helpers used by func_80277B38 (same-TU or
    // CChainActorList.cpp / CUIErrMesWin.cpp / CMenuBattleChain.cpp).
    void func_80276C30();
    void func_80278E0C(cf::CChain* self);
    void func_80278F84(cf::CChain* self);
    void func_80279214(cf::CChain* self);
    void func_8027B770(cf::CChainActorList* self, u32 key);
    void func_8027BB4C(cf::CChainActorList* self, cf::CChainList* list);
    int  func_8027BE84(cf::CChainActorList* self);
    int  func_8027CAE0(cf::CChainList* self, int target, int check);
    int  func_8027CA0C(cf::CChainList* self, int key);
    void func_80293EEC(cf::CChainCombo* self, cf::CfObjectActor* actor);
    int  func_8027C154(cf::CChainChance* self, cf::CChainBattleObj* target, cf::CChainBattleObj* source);
    int  func_8027C33C(cf::CChainAction* self, u8* out);
    void func_8027CBE8(cf::CChainCounter* self);
    void func_8027C098(cf::CChainChance* self);
    void func_802AB4B8(CBattleChainMenuState* self);
    void func_802AB590(CBattleChainMenuState* self);
    bool func_802AB59C(CBattleChainMenuState* self);
    void func_802B4B84(CErrMesEntry* self);
    void func_8014B120(u8* self, cf::CChainScratch20* scratch);
    void func_802818F8();
}
