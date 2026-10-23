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

extern "C" void ChainList_Clear(cf::CChainList* self);

namespace cf {
    // Inlined-construction mirrors for the CChain tail sub-objects. Retail
    // initializes each with a tiny inlined ctor that stores its .data vtable
    // pointer (and zeroes the leading counter), so these are plain structs
    // with manual vptr fields, not polymorphic classes. The implicit CChain
    // ctor then emits the retail construction order exactly (member -> two
    // timers -> chance interleaved between the out-of-line actor-list / time /
    // combo ctors).

    // 0xEC: opaque body + vtable at +0xE8; ctor also resets the list via
    // ChainList_Clear.
    class CChainMemberInit {
    public:
        u8 unk0[0xE8];   //0x00
        void* mVtbl;     //0xE8

        CChainMemberInit() {
            mVtbl = &lbl_eu_80538284;
            ChainList_Clear((CChainList*)this);
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
        void CChain_ResetChainState();
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
        void UpdatePerFrame();
    };

    // View of the battle manager tail for CChain_CheckActivateReady:
    // +0x20C8 (retail lha) and the sudden-commu sub-object at +0x216C
    // (address taken by SuddenCommuIsStateActive).
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
    // vtable slot 0x110 (CChainActor_CheckVoice compares its +0x14 float against
    // lbl_eu_80668A60).
    class CChainVoiceRes {
    public:
        u8 _pad0[0x14];
        f32 field_14;           // 0x14
    };

    // Holder for CChainActor_CheckVoice:
    // loads the battle object through this holder before probing slot
    // 0x110 / the move sub-object / +0x3F60).
    class CChainVoiceHolder {
    public:
        CChainBattleObj* field_0; // 0x0
    };

    // 32-byte table entry probed by CChain_DriveChainExtend at +0xD (signed: retail
    // compares the byte with cmpwi).
    class CChainArt214Entry {
    public:
        u8 field_0[0xD];
        s8 field_0xD;              // 0xD
    };

    // Gauge sub-object returned by CChain_RegisterCombo's slot-0x298 dispatch: the
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

    // Count byte probed by CChain_RegisterCombo at +0x44 of the object reached
    // through the arts-holder's +0x50 pointer.
    struct CChainGaugeCnt44 {
        u8 pad[0x44];
        u8 field_0x44;           //0x44
    };

    // Layout mirror of CChain used by CChain_NotifyStateChange:
    // as a CChainList member so member-array accesses fold the 0x1DC8 offset
    // into the load (a C-style cast of &self->mChainMember would materialize
    // the base into a register first).
    struct CChainMemberListMirror {
        u8 unk0[0x18];
        CChainActorList mChainActorList; //0x18
        CChainList mChainMember;         //0x1DC8
        u8 unk1EB4[0x1F0C - 0x1EB4];
    };

    // Head-field view of CChain used by CChain_TickChain's state machine: the
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

    // 0x20-byte stack scratch written by CChain_TickChain case 5: the 0xe-byte
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
