#include "kyoshin/cf/chain/CChain.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfSoundMan.hpp"
#include "kyoshin/menu/CMenuArtsSelect.hpp"
#include "monolib/math/Random.hpp"

extern "C" CChainGimmickList* func_800B6BC8();
extern "C" int func_801537E0(void* self);
// func_800BE12C / func_800F3970 are also declared locally (with different arg
// spellings) by CBattleManager.cpp / CVision.cpp, which include CChain.hpp
// via CBattleManager.hpp - keep them out of the header like the trio above.
extern "C" void func_800BE12C(void* obj, int a, int b, int c, int d);
extern "C" void func_800F3970(void* bm, void* target, void* src, s32 a, s32 b);

struct ChIf {
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
    virtual void vf004C();
    virtual void vf0050();
};
struct ChIf2 {
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
    virtual void _v00FC();
    virtual void _v0100();
    virtual void _v0104();
    virtual void _v0108();
    virtual void _v010C();
    virtual void _v0110();
    virtual void _v0114();
    virtual void _v0118();
    virtual void _v011C();
    virtual void _v0120();
    virtual void _v0124();
    virtual void _v0128();
    virtual void _v012C();
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
    virtual void _v01E8();
    virtual void _v01EC();
    virtual void _v01F0();
    virtual void _v01F4();
    virtual void _v01F8();
    virtual void _v01FC();
    virtual void _v0200();
    virtual void _v0204();
    virtual void _v0208();
    virtual void _v020C();
    virtual void _v0210();
    virtual void _v0214();
    virtual void _v0218();
    virtual void _v021C();
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
    virtual void _v028C();
    virtual void _v0290();
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
    virtual void vf02BC();
};

namespace cf {
    CChain::CChain(){
        // Implicit member construction (actor-list / time / combo ctors, plus
        // the inlined vptr+field stores for member / timers / chance) runs
        // before this body; the body mirrors the retail reset block.
        int zero = 0;
        func_802AB3D0((CBattleChainMenuState*)&unk1F0C[0]);
        if (zero != 0) {
            mChainTime.mTimer = lbl_eu_80668A18;
            mChainTime.mEnabled = 0;
            mChainTime.mPaused = 1;
            lbl_eu_80663DA0 &= 0xFE;
        }
        unk1F0C[0x11] = zero;
        ((CChainHeadView*)this)->field_2 = zero;
        ((CChainHeadView*)this)->field_2 = 0;
        ((CChainHeadView*)this)->field_0 = 0;
        ((CChainHeadView*)this)->field_1 = -1;
        mChainTime.func_8027CE30();
        func_8027B164(&mChainActorList);
        func_8027C45C((cf::CChainList*)&mChainMember);
        mChainCombo.func1();
        func_802AB3D0((CBattleChainMenuState*)&unk1F0C[0]);
        func_8027711C(func_802B48A0((CErrMesEntry*)&unk1F0C[8]));
    }

    bool CChain::chkActorList() {
        extern bool func_8027B770(void*);
        return func_8027B770((char*)this + 0x18);
    }

    // Retail plain empty+delete dtor: no base/member dtor calls (the actor's
    // CChainTemp/CChainEffect regions are raw buffers at the retail level).
    CChainActor::~CChainActor() {}



}

extern "C" int CChain_getZero_78E04() { return 0; }
extern "C" int CChain_getZero_A584() { return 0; }
extern "C" int CChain_getZero_A9D0() { return 0; }
extern "C" int CChain_getZero_A9D8() { return 0; }
extern "C" int CChain_getZero_A9E0() { return 0; }
extern "C" int CChain_getZero_A9EC() { return 0; }
extern "C" int CChain_getZero_A9F4() { return 0; }
extern "C" int CChain_getZero_A9FC() { return 0; }
extern "C" int CChain_getZero_AA04() { return 0; }
extern "C" u16 CChain_getChainCount(u8* self) { return *(u16*)((u8*)*(void**)self + 0x3f28); }

bool CChain_isValidChain(unsigned char* a1, unsigned char* a2, int a3) {
    unsigned char v = a1[2];
    if (v < 1 || v > 0x18) return false;
    if (v == 5) {
        if (*(unsigned short*)(a2 + 0x3f28) != 5) return false;
        if (a3 == 0) return false;
    }
    return true;
}

// Returns the chain-voice state: 0 when the global voice id is the -1
// sentinel, otherwise the voice-node lookup result for that id.
int func_80277154() {
    if ((u32)lbl_eu_80662A20 + 0x10000 == 0xffff) return 0;
    return func_802A3748((u32)lbl_eu_80662A20);
}
// Chain-gauge accumulation: when the chain is active (type byte set and the
// battle-object flag bit 2 set), add the frame delta to the gauge at 0x1EF0,
// clamped to 0x98967F. Only chain types 0xA..0xC and 0x14..0x15 accumulate.
void func_80277A7C(cf::CChain* self, cf::CChainFlag* flag, float f1) {
    if (self->unk0[8] != 0 && (flag->field_0x3F00 & 4) != 0) {
        if ((int)f1 > 0) {
            int flag = 1;
            u8 v = self->unk0[2];
            int inLow = 0;
            if (0xA <= v && v <= 0xC) inLow = 1;
            if (inLow == 0) {
                int inHigh = 0;
                if (0x14 <= v && v <= 0x15) inHigh = 1;
                if (inHigh == 0) flag = 0;
            }
            if (flag == 0) return;
            cf::CChainChanceTail* tail =
                reinterpret_cast<cf::CChainChanceTail*>(&self->mChainChance);
            int sum = tail->field_0x14 + (int)f1;
            tail->field_0x14 = sum;
            if (sum > 0x98967F) tail->field_0x14 = 0x98967F;
        }
    }
}
extern "C" void func_8027711C(void* self);
extern "C" void func_80277B34(void* self) { func_8027711C(self); }
extern "C" void CChain_noop_78E00() {}
extern "C" void func_80278F5C(u8* self) { reinterpret_cast<ChIf*>((u8*)*(void**)self + 0x3e9c)->vf0050(); }
extern "C" void func_80278F70(u8* self) { reinterpret_cast<ChIf*>((u8*)*(void**)self + 0x3e9c)->vf004C(); }
extern "C" void CChain_noop_795D0() {}
extern "C" void CChain_noop_79768() {}
extern "C" void func_8027A324(u8* self) { reinterpret_cast<ChIf2*>(*(void**)self)->vf02BC(); }
extern "C" void CChain_noop_A9E8() {}
extern "C" void CChain_noop_AA0C() {}

// Reset the chain-voice manager and clear the global voice id back to the
// -1 (not loaded) sentinel. noinline: retail func_80277B38 emits a real bl.
__declspec(noinline) void func_80276C30() {
    func_802A1500();
    lbl_eu_80662A20 = -1;
}
// If a voice id is loaded, retire it (func_802A35B8) and clear the global
// back to the -1 sentinel. noinline: retail func_8027732C emits a real bl.
__declspec(noinline) void func_80276C58() {
    if ((u32)lbl_eu_80662A20 + 0x10000 != 0xffff) {
        if (func_802A3748((u32)lbl_eu_80662A20) == 0) {
            if ((u32)lbl_eu_80662A20 + 0x10000 != 0xffff) {
                func_802A35B8((u32)lbl_eu_80662A20);
                lbl_eu_80662A20 = -1;
            }
        }
    }
}
// Reload the chain voice: retire any currently loaded voice id, allocate a
// new voice node, and dispatch it against the battle-object sub-object.
// @p c (unused) mirrors the retail three-arg call shape.
void func_80276CAC(u8* a, CChainBattleObjTail* b, int c) {
    int ret = func_80276D30((int)a, (u8*)b, reinterpret_cast<u8*>(c));
    if ((u32)lbl_eu_80662A20 + 0x10000 != 0xffff) {
        func_802A35B8((u32)lbl_eu_80662A20);
        lbl_eu_80662A20 = -1;
    }
    int id = func_802A3214();
    lbl_eu_80662A20 = id;
    if ((u32)id + 0x10000 != 0xffff) {
        if (b != 0) b = (CChainBattleObjTail*)&b->field_0x3E9C;
        func_802A3680(id, b, ret);
    }
}
// Chain-voice selection: pick a voice id by mode. Modes 0/4/5 roll simple
// mtRand-based ids; modes 2/3 pick from per-chain-type id tables; mode 1
// maps both objects' +0x3F28 chain types to internal ids, rolls whether the
// voice falls on the "continue" id range (0x7D3..) and switches on the second
// object's mapped type.
int func_80276D30(int mode, u8* p1, u8* p2) {
    switch (mode) {
    case 0:
        return ml::math::mtRand(2) + 0x7d1;
    case 1: {
        // Declared in this order so MWCC maps them to r4/r5/r6 like retail.
        int m1;
        int t2;
        bool cont;
        cont = ml::math::mtRand(2) != 0;
        int v1 = ((CChainTypeView*)p1)->field_3F28;
        if (v1 == 8) m1 = 3;
        else if (v1 == 9) m1 = 8;
        else if (v1 == 0xa) m1 = 9;
        else if (v1 == 0xb) m1 = 0xa;
        else if (v1 == 0xc) m1 = 0xb;
        else if (v1 == 0xd) m1 = 0xc;
        else m1 = v1;
        int v2 = ((CChainTypeView*)p2)->field_3F28;
        if (v2 == 8) t2 = 3;
        else if (v2 == 9) t2 = 8;
        else if (v2 == 0xa) t2 = 9;
        else if (v2 == 0xb) t2 = 0xa;
        else if (v2 == 0xc) t2 = 0xb;
        else if (v2 == 0xd) t2 = 0xc;
        else t2 = v2;
        int result = -1;
        if (m1 == 8 || m1 == 0xa) cont = 1;
        switch (t2) {
        case 1: result = cont != 0 ? 0x7d3 : 0x1f5; break;
        case 2: result = cont != 0 ? 0x7d4 : 0x1f6; break;
        case 3: result = cont != 0 ? 0x7d5 : 0x1f7; break;
        case 4: result = cont != 0 ? 0x7d6 : 0x1f8; break;
        case 5: result = cont != 0 ? 0x7d7 : 0x1f9; break;
        case 6: result = cont != 0 ? 0x7d8 : 0x1fa; break;
        case 7: result = cont != 0 ? 0x7d9 : 0x1fb; break;
        case 8: result = 0x7de; break;
        case 0xa: result = 0x7da; break;
        default: break;
        }
        if (result == 0x7d8) {
            int flag = 0;
            if (m1 == 2 && t2 == 6 &&
                (u32)cf::CfGameManager::func_800822F4() < 0x91) {
                flag = 1;
            }
            if (flag != 0) result = 0xc25;
        }
        return result;
    }
    case 2: {
        int v = ((CChainTypeView*)p1)->field_3F28;
        int m;
        if (v == 8) m = 3;
        else if (v == 9) m = 8;
        else if (v == 0xa) m = 9;
        else if (v == 0xb) m = 0xa;
        else if (v == 0xc) m = 0xb;
        else if (v == 0xd) m = 0xc;
        else m = v;
        if (m == 8) return lbl_eu_80662A24[ml::math::mtRand(1)];
        if (m == 0xa) return lbl_eu_80662A28[ml::math::mtRand(2)];
        return lbl_eu_805381C8[ml::math::mtRand(3)];
    }
    case 3: {
        int v = ((CChainTypeView*)p1)->field_3F28;
        int m;
        if (v == 8) m = 3;
        else if (v == 9) m = 8;
        else if (v == 0xa) m = 9;
        else if (v == 0xb) m = 0xa;
        else if (v == 0xc) m = 0xb;
        else if (v == 0xd) m = 0xc;
        else m = v;
        if (m == 8 || m == 0xa) return lbl_eu_80662A30[ml::math::mtRand(1)];
        return lbl_eu_80662A38[ml::math::mtRand(2)];
    }
    case 4:
        return ml::math::mtRand(2) + 0x835;
    case 5:
        return ml::math::mtRand(2) + 0x837;
    default:
        return -1;
    }
}
#pragma push
#pragma auto_inline off
// If a voice id is loaded, retire it and clear the global back to the -1
// sentinel (auto_inline off keeps func_80277B34's call a real bl).
extern "C" void func_8027711C(void* self) {
    if ((u32)lbl_eu_80662A20 + 0x10000 != 0xffff) {
        func_802A35B8((u32)lbl_eu_80662A20);
        lbl_eu_80662A20 = -1;
    }
}
#pragma pop
// Full chain-state reset: seed the chain timer when the chain type is set,
// clear the type/actor bytes, then reset every sub-object (time, actor list,
// member list, combo, chain-menu state, error-message record, voice).
void cf::CChain::func_8027728C() {
    if (unk0[2] != 0) {
        mChainTime.mTimer = lbl_eu_80668A18;
        mChainTime.mEnabled = 0;
        mChainTime.mPaused = 1;
        lbl_eu_80663DA0 &= 0xFE;
    }
    unk0[2] = 0;
    ((CChainHeadView*)this)->field_0 = 0;
    ((CChainHeadView*)this)->field_1 = -1;
    mChainTime.func_8027CE30();
    func_8027B164(&mChainActorList);
    func_8027C45C((cf::CChainList*)&mChainMember);
    mChainCombo.func1();
    func_802AB3D0(reinterpret_cast<CBattleChainMenuState*>(&unk1F0C[0]));
    func_8027711C(func_802B48A0(reinterpret_cast<CErrMesEntry*>(&unk1F0C[8])));
}
// Per-frame chain update: after the battle-manager / mode-flag gates, sweep
// the actor list (dead-actor removal, member rebuild, run-linkage), then when
// a chain type is active decrement both chain timers and advance the chance /
// voice / time / menu sub-objects; finally forward the err-mes record.
void func_8027732C(cf::CChain* self) {
    if (cf::CfGameManager::func_800829B8() != 0) return;
    // rlwinm r0,r0,0,12,12: bit 19 (0x80000) of the mode-flag word.
    if ((lbl_eu_80663E28 & 0x80000) != 0) return;
    func_8027B200(&self->mChainActorList);
    func_8027C49C((cf::CChainList*)&self->mChainMember);
    func_8027B2CC((cf::CChainActorList*)((u8*)self + 0x18));
    if (self->unk0[2] != 0) {
        func_8027C560((cf::CChainList*)&self->mChainMember);
        if (self->mChainTimer1.unk0 > 0) self->mChainTimer1.unk0--;
        if (self->mChainTimer2.unk0 > 0) self->mChainTimer2.unk0--;
        func_8027C0B0((cf::CChainChanceS*)&self->mChainChance);
        func_80277B38(self);
        func_8027CF3C(&self->mChainTime);
        func_802AB410((CBattleChainMenuState*)&self->unk1F0C[0]);
        func_80276C58();
    }
    func_802B48B8((CErrMesEntry*)&self->unk1F0C[8]);
}
// Chain-start path for the arts combo: valid only for chain types in the
// gauge-accumulating ranges (0xA..0xC or 0x14..0x15); resolves the member
// entry for the target battle object, registers the combo, and refreshes the
// arts-voice flag from the current arts id.
void func_802773EC(cf::CChain* self, cf::CChainBattleObj2A4* target) {
    int ok = 0;
    int flag = 1;
    u8 v = self->unk0[2];
    if (0xA <= v && v <= 0xC) ok = 1;
    if (ok == 0) {
        if (0x14 <= v && v <= 0x15) ok = 1;
    }
    if (ok == 0) flag = 0;
    if (flag == 0) return;
    cf::CChainMemberListMirror* mv = (cf::CChainMemberListMirror*)self;
    u8 rawIdx = self->unk0[0];
    cf::CChainActor* actor = 0;
    if ((int)(s8)rawIdx < (int)mv->mChainMember.mCount) {
        actor = mv->mChainMember.mActors[(s8)rawIdx];
    }
    u32 actorKey = (actor != 0) ? actor->unk0 : 0;
    if ((u32)target != actorKey) return;
    if (self->unk0[3] != 0) return;
    func_80293E24(&self->mChainCombo, (cf::CfObjectActor*)target);
    if (self->unk0[8] == 0) return;
    cf::CChainArtsId* arts = (cf::CChainArtsId*)target->v167()->mArtsCategory;
    self->unk0[9] = (u8)(func_80146300(arts->field_48, 1) == 0);
}
// us-80279990: apply an arts-combo hit to the chain gauge. Validates the
// chain type range (0xA..0xC / 0x14..0x15) and that the current member owns
// the hit battle object, then runs one or two gauge-delta rounds against the
// object's gauge sub-object and refreshes the combo / next-actor state.
// One round: truncate the accumulated gauge (+0x5c + +0x60) unless flagged,
// scale it by the slot-0x290-gated func_8025FB10 delta and add it into
// +0x5c, then add the raw delta into +0x58.
#define CHAIN_GAUGE_ROUND(g, tgt, id) \
    do { \
        float sum = (g)->field_5C + (g)->field_60; \
        int amt; \
        if (sum >= lbl_eu_80668A1C) { \
            if (!((g)->field_74 & 1)) { \
                amt = (int)(sum + ((lbl_eu_80668A1C < sum) ? lbl_eu_80668A20 : lbl_eu_80668A28)); \
            } else { \
                amt = 0; \
            } \
        } else { \
            amt = 0; \
        } \
        if ((tgt)->v162() != 0) { \
            mul = lbl_eu_80668A30 * (float)func_8025FB10((void*)(tgt)->v162(), id); \
        } else { \
            mul = lbl_eu_80668A1C; \
        } \
        (g)->field_5C += (float)amt * mul; \
        if ((tgt)->v162() != 0) { \
            mul = lbl_eu_80668A30 * (float)func_8025FB10((void*)(tgt)->v162(), id); \
        } else { \
            mul = lbl_eu_80668A1C; \
        } \
        (g)->field_58 += mul; \
    } while (0)

void func_8027750C(cf::CChain* self, cf::CChainBattleObj750* target) {
    cf::CChainMemberListMirror* mv = (cf::CChainMemberListMirror*)self;
    cf::CChainHeadView* h = (cf::CChainHeadView*)self;
    int flag = 1;
    u8 v = h->field_2;
    int inLow = 0;
    if (0xA <= v && v <= 0xC) inLow = 1;
    if (inLow == 0) {
        int inHigh = 0;
        if (0x14 <= v && v <= 0x15) inHigh = 1;
        if (inHigh == 0) flag = 0;
    }
    if (flag == 0) return;

    s8 idx = h->field_0;
    cf::CChainActor* actor;
    if ((int)idx < (int)mv->mChainMember.mCount) {
        actor = mv->mChainMember.mActors[idx];
    } else {
        actor = 0;
    }
    u32 key = (actor != 0) ? actor->unk0 : 0;
    if ((u32)target != key) return;

    if (h->field_8 != 0) {
        cf::CChainGaugeSub* g = target->v164();
        float mul;
        if (g->field_9 != 0) {
            CHAIN_GAUGE_ROUND(g, target, 0x4a);
            if (func_8027CA0C((cf::CChainList*)&self->mChainMember, 1) != 0) {
                CHAIN_GAUGE_ROUND(g, target, 0x4b);
            }
        } else {
            CHAIN_GAUGE_ROUND(g, target, 0x40);
        }
    }

    // Tail: register the combo once, then mark the next actor index from the
    // arts-holder's count byte.
    if (h->field_3 == 0) {
        func_80293EEC(&self->mChainCombo, (cf::CfObjectActor*)target);
    }
    h->field_3 = 1;
    CChainCombo_ArtsCategoryHolder* holder = target->v167();
    if (holder->mArtsCategory != 0) {
        holder = target->v167();
        int cnt = ((cf::CChainGaugeCnt44*)holder->mArtsCategory)->field_0x44;
        if (target->v168() >= cnt - 1) {
            h->field_4 = 1;
        } else {
            h->field_4 = 0;
        }
    } else {
        h->field_4 = 1;
    }
}
// Chain-state driver: after the chain-start gate (battle-manager mode flag /
// current actor's run-key availability), either runs the chain-start block
// (re-arm the member list, chain time, actor reslist, menu / err-mes / combo
// records, then jump to state 0x1a) or dispatches the current state byte
// through the jump table (chain types 0..0x1a).
__declspec(noinline) void func_80277B38(cf::CChain* self) {
    cf::CChainHeadView* h = (cf::CChainHeadView*)self;
    cf::CChainMemberListMirror* v = (cf::CChainMemberListMirror*)self;
    int flag;
    if ((lbl_eu_80663E24 & 0x400000) != 0) {
        flag = 1;
    } else {
        u8 rawIdx = (u8)h->field_0;
        int count = (int)v->mChainMember.mCount;
        cf::CChainActor* actor;
        if ((int)(s8)rawIdx < count) {
            actor = v->mChainMember.mActors[(s8)rawIdx];
        } else {
            actor = 0;
        }
        if (actor == 0) {
            flag = 1;
        } else {
            if ((int)(s8)rawIdx < count) {
                actor = v->mChainMember.mActors[(s8)rawIdx];
            } else {
                actor = 0;
            }
            if (((cf::CChainActorVtIfB38*)actor)->v024() == 0) {
                flag = 1;
            } else {
                flag = v->mChainMember.mFlag;
            }
        }
    }
    if (flag != 0) {
        // Chain start: reset the chain voice while the chain type is still in
        // the early range, then re-arm the member list, chain time, actor
        // reslist and the menu / err-mes / combo records before state 0x1a.
        if (h->field_2 < 3 && h->field_8 != 0) func_802818F8();
        func_8027C6B4((cf::CChainList*)&self->mChainMember, 0, -1);
        self->mChainTime.mTimer = lbl_eu_80668A18;
        self->mChainTime.mEnabled = 0;
        self->mChainTime.mPaused = 1;
        lbl_eu_80663DA0 &= 0xFE;
        func_8027C924((cf::CChainList*)&self->mChainMember, 0);
        _reslist_node<cf::CChainActor*>* node =
            self->mChainActorList.mChainActorList.mStartNodePtr->mNext;
        while (node != self->mChainActorList.mChainActorList.mStartNodePtr) {
            ((cf::CChainActorVtIfB38*)node->mItem)->v005(0);
            node = node->mNext;
        }
        self->mChainActorList.unk1DA8[0] = 0;
        func_80082B38__Q22cf13CfGameManagerFv();
        func_802B4B84((CErrMesEntry*)&self->unk1F0C[8]);
        if (h->field_8 != 0) {
            func_8027CBE8((cf::CChainCounter*)&self->mChainChance.unk14[0]);
        }
        self->mChainCombo.func1();
        func_802AB4B8((CBattleChainMenuState*)&self->unk1F0C[0]);
        h->field_2 = 0x1a;
    }
    u8 actionOut;
    int localC;
    int local10;
    int local14;
    switch (h->field_2) {
    case 0: {
        // Chain-link start: re-run the actor's run key across every member,
        // register the chain-voice source on the actor list, set the
        // chain-voice flag, arm the chain time and reset the chain records.
        cf::CChainActor* actor0;
        cf::CChainActor* actor;
        if ((int)(s8)h->field_0 < (int)v->mChainMember.mCount) {
            actor0 = v->mChainMember.mActors[(s8)h->field_0];
        } else {
            actor0 = 0;
        }
        if ((int)(s8)h->field_0 < (int)v->mChainMember.mCount) {
            actor = v->mChainMember.mActors[(s8)h->field_0];
        } else {
            actor = 0;
        }
        int runKey = ((cf::CChainActorVtIfB38*)actor)->v024();
        void* src = func_8016FE34(func_800B708C(runKey));
        if (src != 0) {
            func_8027B770(&self->mChainActorList, (u32)src);
        }
        if ((int)(s8)h->field_0 < (int)v->mChainMember.mCount) {
            actor = v->mChainMember.mActors[(s8)h->field_0];
        } else {
            actor = 0;
        }
        int runKey2 = ((cf::CChainActorVtIfB38*)actor)->v024();
        for (int i = 0; i < (int)v->mChainMember.mCount; i++) {
            ((cf::CChainActorVtIfB38*)v->mChainMember.mActors[i])->v025(runKey2);
        }
        lbl_eu_80663DA0 |= 1;
        func_802AB590((CBattleChainMenuState*)&self->unk1F0C[0]);
        if ((int)(s8)h->field_0 < (int)v->mChainMember.mCount) {
            actor = v->mChainMember.mActors[(s8)h->field_0];
        } else {
            actor = 0;
        }
        func_800BE12C((u8*)((cf::CChainBattleObj*)actor->unk0) + 0x3e9c, 0x1b, 0,
                      3, 1);
        if ((int)(s8)h->field_0 < (int)v->mChainMember.mCount) {
            actor = v->mChainMember.mActors[(s8)h->field_0];
        } else {
            actor = 0;
        }
        func_800F3970(getInstance__Q22cf14CBattleManagerFv(), (void*)actor->unk0,
                      0, 0, 0);
        func_8027C098(&self->mChainChance);
        func_80276C30();
        ((cf::CChainActorVtIfB38*)actor0)->v012(0, 0);
        cf::CfSoundMan::func_801BFC38(0, 0x69, 0, 0, lbl_eu_80668A40);
        h->field_7 = 1;
        h->field_5 = 0;
        h->field_2++;
        break;
    }
    case 1: {
        // Chain-link candidate scan: the menu must be up and the actor must
        // not report an active chain; then the member / actor-list gates and
        // a voice sweep over every member move to state 3.
        cf::CChainActor* actor;
        s8 idx = h->field_0;
        if ((int)idx < (int)v->mChainMember.mCount) {
            actor = v->mChainMember.mActors[idx];
        } else {
            actor = 0;
        }
        if (func_802AB59C((CBattleChainMenuState*)&self->unk1F0C[0]) == 0) break;
        if (((cf::CChainActorVtIfB38*)actor)->v013() != 0) break;
        if (func_8027CAE0((cf::CChainList*)&self->mChainMember, 6, 1) == 0) break;
        if (func_8027CAE0((cf::CChainList*)&self->mChainMember, 0x1f, 0) != 0) break;
        if (func_8027BE84(&self->mChainActorList) != 0) break;
        for (int i = 0; i < (int)v->mChainMember.mCount; i++) {
            cf::CChainActor* actor2 = v->mChainMember.mActors[i];
            if (func_80148778(&((cf::CChainBattleObj*)actor2->unk0)->mSub8, 0x11) != 0) {
                ((cf::CChainBattleObj*)actor2->unk0)->mSub8.s06(0x11);
            }
        }
        h->field_2 = 3;
        break;
    }
    case 2: {
        if (h->field_8 != 0) {
            h->field_2 = 4;
        } else {
            h->field_2 = 6;
        }
        break;
    }
    case 3:
        func_80278E0C(self);
        break;
    case 4:
        func_80278F84(self);
        break;
    case 5: {
        // Chain-extend: clear the per-chain state bytes, re-run the actor's
        // run key, arm the chain-voice scratch records and the chain timer.
        cf::CChainActor* actor;
        s8 idx = h->field_0;
        if ((int)idx < (int)v->mChainMember.mCount) {
            actor = v->mChainMember.mActors[idx];
        } else {
            actor = 0;
        }
        h->field_3 = 0;
        h->field_4 = 0;
        h->field_6 = 1;
        int runKey = ((cf::CChainActorVtIfB38*)actor)->v024();
        for (int i = 0; i < (int)v->mChainMember.mCount; i++) {
            ((cf::CChainActorVtIfB38*)v->mChainMember.mActors[i])->v025(runKey);
        }
        cf::CChainScratch20 scratchB;
        memset(&scratchB.mScratch, 0, 0xe);
        memset(&scratchB, 0, sizeof(scratchB));
        scratchB.mFields.field_6 = 6;
        scratchB.mFields.field_D = 0xe;
        scratchB.mFields.field_10 = 0;
        scratchB.field_12 = -1;
        scratchB.field_14 = lbl_eu_80668A1C;
        func_8014B120(&((cf::CChainBattleObj*)actor->unk0)->mField3380, &scratchB);
        self->mChainTime.mTimer = lbl_eu_80668A44;
        self->mChainTime.mEnabled = 0;
        self->mChainTime.mPaused = 1;
        self->mChainTimer1.unk0 = 0x3c;
        h->field_2++;
        break;
    }
    case 6: {
        // Chain-link accept/reject: the arts-select actor-id check decides
        // whether the link is accepted (err-mes register + timer re-arm) or
        // rejected (move-flag cleanup, then state 0xd once the timer runs out).
        cf::CChainActor* actor;
        s8 idx = h->field_0;
        if ((int)idx < (int)v->mChainMember.mCount) {
            actor = v->mChainMember.mActors[idx];
        } else {
            actor = 0;
        }
        cf::CChainBattleObj214* battleObj = (cf::CChainBattleObj214*)actor->unk0;
        local14 = *(int*)battleObj->field_04->f30();
        if (func_80174C98(battleObj, &local14, 0xa) == 0) {
            if (h->field_6 != 0) {
                func_802B48E4((CErrMesEntry*)&self->unk1F0C[8], (CErrMesOwner*)battleObj);
                h->field_6 = 0;
            }
            if (self->mChainTimer1.unk0 > 0) break;
            battleObj->field_3594 = 0;
            battleObj->field_3590 = 0;
            h->field_2 = 0xd;
        } else {
            func_802B4A68((CErrMesEntry*)&self->unk1F0C[8], (CErrMesOwner*)battleObj);
            self->mChainTimer1.unk0 = 5;
            h->field_2++;
        }
        break;
    }
    case 7: {
        if (self->mChainTimer1.unk0 > 0) break;
        self->mChainTime.mTimer = lbl_eu_80668A48;
        self->mChainTime.mEnabled = 1;
        self->mChainTime.mPaused = 1;
        self->mChainTimer1.unk0 = 0xa;
        h->field_2++;
        break;
    }
    case 8: {
        if (self->mChainTimer1.unk0 > 0) break;
        self->mChainTime.mTimer = lbl_eu_80668A18;
        self->mChainTime.mEnabled = 0;
        self->mChainTime.mPaused = 1;
        h->field_2 = 0xa;
        break;
    }
    case 9: {
        self->mChainTimer1.unk0 = 0x1e;
        self->mChainTimer2.unk0 = 0x96;
        h->field_2++;
        break;
    }
    case 0xa:
        func_80279214(self);
        break;
    case 0xb: {
        if (self->mChainTimer1.unk0 > 0) break;
        cf::CChainActor* actor;
        s8 idx = h->field_0;
        if ((int)idx < (int)v->mChainMember.mCount) {
            actor = v->mChainMember.mActors[idx];
        } else {
            actor = 0;
        }
        if (((cf::CChainActorVtIfB38*)actor)->v013() != 0) break;
        h->field_2 = 0xd;
        break;
    }
    case 0xc: {
        // Member advance: re-register the last member on the list, then move
        // the current index (direction from field_5) and gate on the new
        // actor's chainable probe / the chain-voice flags.
        if ((s8)h->field_1 != -1) {
            func_8027C6B4((cf::CChainList*)&self->mChainMember, 0, (s8)h->field_1);
        }
        h->field_1 = h->field_0;
        if (h->field_7 != 0) {
            h->field_2 = 0xe;
            break;
        }
        s8 idx = h->field_0;
        s8 newIdx;
        if (h->field_5 == 0) {
            newIdx = (s8)(idx + 1);
            if (!((s16)v->mChainMember.mCount > (int)newIdx)) newIdx = 0;
        } else {
            newIdx = (s8)(idx - 1);
            if ((int)newIdx < 0) newIdx = (s8)((s16)v->mChainMember.mCount - 1);
        }
        cf::CChainActor* newActor;
        if ((int)(s8)newIdx < (int)v->mChainMember.mCount) {
            newActor = v->mChainMember.mActors[(s8)newIdx];
        } else {
            newActor = 0;
        }
        if (((cf::CChainActorVtIfB38*)newActor)->v007(0) == 0) {
            h->field_2 = 0x16;
            break;
        }
        if (h->field_A == 0 && h->field_8 != 0) {
            h->field_2 = 0x10;
            break;
        }
        h->field_2 = 0x16;
        break;
    }
    case 0xd: {
        // Member advance + chain re-link: re-register the current member,
        // arm the chain time, then fan the advance out through the actor's
        // manual vtable (1/3 flag pair, second actor as the move target).
        s8 idx = h->field_0;
        cf::CChainActor* actor;
        if ((int)idx < (int)v->mChainMember.mCount) {
            actor = v->mChainMember.mActors[idx];
        } else {
            actor = 0;
        }
        s8 newIdx;
        if (h->field_5 == 0) {
            newIdx = (s8)(idx + 1);
            if (!((s16)v->mChainMember.mCount > (int)newIdx)) newIdx = 0;
        } else {
            newIdx = (s8)(idx - 1);
            if ((int)newIdx < 0) newIdx = (s8)((s16)v->mChainMember.mCount - 1);
        }
        cf::CChainActor* nextActor;
        if ((int)(s8)newIdx < (int)v->mChainMember.mCount) {
            nextActor = v->mChainMember.mActors[(s8)newIdx];
        } else {
            nextActor = 0;
        }
        self->mChainTime.mTimer = lbl_eu_80668A48;
        self->mChainTime.mEnabled = 0;
        self->mChainTime.mPaused = 1;
        func_8027C6B4((cf::CChainList*)&self->mChainMember, 1, (s8)h->field_0);
        if (h->field_7 != 0) {
            ((cf::CChainActorVtIfB38*)actor)->v012(1, (int)nextActor);
        } else {
            ((cf::CChainActorVtIfB38*)actor)->v012(3, 0);
        }
        h->field_2++;
        break;
    }
    case 0xe: {
        cf::CChainActor* actor;
        s8 idx = h->field_0;
        if ((int)idx < (int)v->mChainMember.mCount) {
            actor = v->mChainMember.mActors[idx];
        } else {
            actor = 0;
        }
        if (((cf::CChainActorVtIfB38*)actor)->v013() != 0) break;
        s8 newIdx;
        if (h->field_5 == 0) {
            newIdx = (s8)(idx + 1);
            if (!((s16)v->mChainMember.mCount > (int)newIdx)) newIdx = 0;
        } else {
            newIdx = (s8)(idx - 1);
            if ((int)newIdx < 0) newIdx = (s8)((s16)v->mChainMember.mCount - 1);
        }
        h->field_0 = newIdx;
        if ((s8)h->field_0 >= (int)v->mChainMember.mCount - 1) {
            h->field_7 = 0;
        }
        h->field_2 = 3;
        break;
    }
    case 0xf: {
        // Chain-message gate: the chance check (current + next battle object)
        // decides whether the link continues or drops to state 0x16.
        s8 idx = h->field_0;
        s8 newIdx;
        if (h->field_5 == 0) {
            newIdx = (s8)(idx + 1);
            if (!((s16)v->mChainMember.mCount > (int)newIdx)) newIdx = 0;
        } else {
            newIdx = (s8)(idx - 1);
            if ((int)newIdx < 0) newIdx = (s8)((s16)v->mChainMember.mCount - 1);
        }
        cf::CChainActor* actor;
        cf::CChainActor* nextActor;
        if ((int)idx < (int)v->mChainMember.mCount) {
            actor = v->mChainMember.mActors[idx];
        } else {
            actor = 0;
        }
        if ((int)(s8)newIdx < (int)v->mChainMember.mCount) {
            nextActor = v->mChainMember.mActors[(s8)newIdx];
        } else {
            nextActor = 0;
        }
        if (func_8027C154(&self->mChainChance, (cf::CChainBattleObj*)actor->unk0,
                          (cf::CChainBattleObj*)nextActor->unk0) != 0) {
            u8 newState = (u8)(h->field_2 + 1);
            self->mChainTime.mTimer = lbl_eu_80668A48;
            self->mChainTime.mEnabled = 0;
            self->mChainTime.mPaused = 1;
            h->field_2 = newState;
        } else {
            h->field_2 = 0x16;
        }
        break;
    }
    case 0x10: {
        // Chain action select: the pad-action helper writes the chosen action;
        // 0xe when a real action came back, 0x16 otherwise.
        if (func_8027C33C((cf::CChainAction*)&self->mChainChance, &actionOut) != 0) {
            if (actionOut != 0) {
                h->field_2 = 0xe;
            } else {
                h->field_2 = 0x16;
            }
        }
        break;
    }
    case 0x11: {
        self->mChainTimer1.unk0 = 0x1e;
        h->field_10 = 1;
        h->field_2++;
        break;
    }
    case 0x12: {
        if (self->mChainTimer1.unk0 > 0) break;
        cf::CChainActor* actor;
        s8 idx = h->field_0;
        if ((int)idx < (int)v->mChainMember.mCount) {
            actor = v->mChainMember.mActors[idx];
        } else {
            actor = 0;
        }
        cf::CChainBattleObj214* battleObj = (cf::CChainBattleObj214*)actor->unk0;
        local10 = *(int*)battleObj->field_04->f30();
        if (func_80174C98(battleObj, &local10, 6) != 0) {
            h->field_2 = 5;
        }
        break;
    }
    case 0x13: {
        self->mChainTimer1.unk0 = 0x1e;
        cf::CChainActor* actor;
        s8 idx = h->field_0;
        if ((int)idx < (int)v->mChainMember.mCount) {
            actor = v->mChainMember.mActors[idx];
        } else {
            actor = 0;
        }
        cf::CChainBattleObjB38* battleObj =
            (actor != 0) ? (cf::CChainBattleObjB38*)actor->unk0 : 0;
        h->field_C = ((cf::CChainBattleObjB38*)battleObj)->mSub.v17();
        h->field_2++;
        break;
    }
    case 0x14: {
        // Arts-index check: when the embedded sub-object's arts index still
        // matches the stored one, wait for the timer then re-check the actor
        // id; otherwise refresh the voice-act marker. Both paths drop to
        // state 0xd (shared store in the retail).
        cf::CChainActor* actor;
        if ((int)(s8)h->field_0 < (int)v->mChainMember.mCount) {
            actor = v->mChainMember.mActors[(s8)h->field_0];
        } else {
            actor = 0;
        }
        cf::CChainBattleObjB38* battleObj =
            (actor != 0) ? (cf::CChainBattleObjB38*)actor->unk0 : 0;
        if (h->field_C == ((cf::CChainBattleObjB38*)battleObj)->mSub.v17()) {
            if (self->mChainTimer1.unk0 > 0) break;
            if ((int)(s8)h->field_0 < (int)v->mChainMember.mCount) {
                actor = v->mChainMember.mActors[(s8)h->field_0];
            } else {
                actor = 0;
            }
            cf::CChainBattleObj214* b214 = (cf::CChainBattleObj214*)actor->unk0;
            localC = *(int*)b214->field_04->f30();
            if (func_80174C98(b214, &localC, 6) == 0) break;
        } else {
            if ((int)(s8)h->field_0 < (int)v->mChainMember.mCount) {
                actor = v->mChainMember.mActors[(s8)h->field_0];
            } else {
                actor = 0;
            }
            cf::CChainBattleObj* b2 =
                (actor != 0) ? (cf::CChainBattleObj*)actor->unk0 : 0;
            func_801537F0(&b2->mField3380);
        }
        h->field_2 = 0xd;
        break;
    }
    case 0x15: {
        if (h->field_8 != 0) {
            func_8027CBE8((cf::CChainCounter*)&self->mChainChance.unk14[0]);
        }
        u8 newState = (u8)(h->field_2 + 1);
        self->mChainTime.mTimer = lbl_eu_80668A18;
        self->mChainTime.mEnabled = 0;
        self->mChainTime.mPaused = 1;
        h->field_2 = newState;
        break;
    }
    case 0x16: {
        // Chain-cancel sweep: when the chain-voice flag is set, roll the next
        // move direction and re-link (randomized); otherwise reset the whole
        // chain state (member list, time, actor reslist, records) and drop to
        // state 0x1a.
        if (func_8027CAE0((cf::CChainList*)&self->mChainMember, 6, 1) == 0) break;
        if (h->field_8 != 0) {
            s8 j;
            for (j = 0; (s8)j < (s16)v->mChainMember.mCount; j++) {
                cf::CChainActor* actor;
                if ((int)(s8)j < (int)v->mChainMember.mCount) {
                    actor = v->mChainMember.mActors[(s8)j];
                } else {
                    actor = 0;
                }
                u32 battleObj = (actor != 0) ? actor->unk0 : 0;
                func_800BE12C((u8*)battleObj + 0x3e9c, 0x1b, 0, 5, 1);
            }
            h->field_5 = (u8)(ml::math::mtRand(2) != 0);
            cf::CChainActor* actor;
            if ((int)(s8)h->field_0 < (int)v->mChainMember.mCount) {
                actor = v->mChainMember.mActors[(s8)h->field_0];
            } else {
                actor = 0;
            }
            ((cf::CChainActorVtIfB38*)actor)->v012(4, 0);
            s8 idx2 = (s8)h->field_0;
            s8 newIdx;
            if (h->field_5 == 0) {
                newIdx = (s8)(idx2 + 1);
                if (!((s16)v->mChainMember.mCount > (int)newIdx)) newIdx = 0;
            } else {
                newIdx = (s8)(idx2 - 1);
                if ((int)newIdx < 0) newIdx = (s8)((s16)v->mChainMember.mCount - 1);
            }
            h->field_0 = newIdx;
            h->field_2++;
        } else {
            func_8027C6B4((cf::CChainList*)&self->mChainMember, 0, -1);
            self->mChainTime.mTimer = lbl_eu_80668A18;
            self->mChainTime.mEnabled = 0;
            self->mChainTime.mPaused = 1;
            lbl_eu_80663DA0 &= 0xFE;
            func_8027C924((cf::CChainList*)&self->mChainMember, 0);
            _reslist_node<cf::CChainActor*>* node =
                self->mChainActorList.mChainActorList.mStartNodePtr->mNext;
            while (node != self->mChainActorList.mChainActorList.mStartNodePtr) {
                ((cf::CChainActorVtIfB38*)node->mItem)->v005(0);
                node = node->mNext;
            }
            self->mChainActorList.unk1DA8[0] = 0;
            func_80082B38__Q22cf13CfGameManagerFv();
            func_802B4B84((CErrMesEntry*)&self->unk1F0C[8]);
            if (h->field_8 != 0) {
                func_8027CBE8((cf::CChainCounter*)&self->mChainChance.unk14[0]);
            }
            self->mChainCombo.func1();
            func_802AB4B8((CBattleChainMenuState*)&self->unk1F0C[0]);
            h->field_2 = 0x1a;
        }
        break;
    }
    case 0x17: {
        // Chain re-link while the member changed: re-register the old member,
        // then run the shared chain-reset block and bump the state.
        if (h->field_0 != h->field_1) {
            cf::CChainActor* actor;
            if ((int)(s8)h->field_0 < (int)v->mChainMember.mCount) {
                actor = v->mChainMember.mActors[(s8)h->field_0];
            } else {
                actor = 0;
            }
            if (((cf::CChainActorVtIfB38*)actor)->v013() == 0) {
                if ((int)(s8)h->field_0 < (int)v->mChainMember.mCount) {
                    actor = v->mChainMember.mActors[(s8)h->field_0];
                } else {
                    actor = 0;
                }
                ((cf::CChainActorVtIfB38*)actor)->v012(5, 0);
                s8 idx = (s8)h->field_0;
                s8 newIdx;
                if (h->field_5 == 0) {
                    newIdx = (s8)(idx + 1);
                    if (!((s16)v->mChainMember.mCount > (int)newIdx)) newIdx = 0;
                } else {
                    newIdx = (s8)(idx - 1);
                    if ((int)newIdx < 0) newIdx = (s8)((s16)v->mChainMember.mCount - 1);
                }
                h->field_0 = newIdx;
            }
        }
        if (func_8027CAE0((cf::CChainList*)&self->mChainMember, 6, 1) == 0) break;
        func_8027C6B4((cf::CChainList*)&self->mChainMember, 0, -1);
        self->mChainTime.mTimer = lbl_eu_80668A18;
        self->mChainTime.mEnabled = 0;
        self->mChainTime.mPaused = 1;
        lbl_eu_80663DA0 &= 0xFE;
        func_8027C924((cf::CChainList*)&self->mChainMember, 0);
        _reslist_node<cf::CChainActor*>* node =
            self->mChainActorList.mChainActorList.mStartNodePtr->mNext;
        while (node != self->mChainActorList.mChainActorList.mStartNodePtr) {
            ((cf::CChainActorVtIfB38*)node->mItem)->v005(0);
            node = node->mNext;
        }
        self->mChainActorList.unk1DA8[0] = 0;
        func_80082B38__Q22cf13CfGameManagerFv();
        func_802B4B84((CErrMesEntry*)&self->unk1F0C[8]);
        if (h->field_8 != 0) {
            func_8027CBE8((cf::CChainCounter*)&self->mChainChance.unk14[0]);
        }
        self->mChainCombo.func1();
        func_802AB4B8((CBattleChainMenuState*)&self->unk1F0C[0]);
        h->field_2++;
        break;
    }
    case 0x18: {
        // Chain re-link (member changed): same advance as 0x17, then re-check
        // the actor's chain-state query before ending the chain (0x1a).
        if (h->field_0 != h->field_1) {
            cf::CChainActor* actor;
            if ((int)(s8)h->field_0 < (int)v->mChainMember.mCount) {
                actor = v->mChainMember.mActors[(s8)h->field_0];
            } else {
                actor = 0;
            }
            if (((cf::CChainActorVtIfB38*)actor)->v013() == 0) {
                if ((int)(s8)h->field_0 < (int)v->mChainMember.mCount) {
                    actor = v->mChainMember.mActors[(s8)h->field_0];
                } else {
                    actor = 0;
                }
                ((cf::CChainActorVtIfB38*)actor)->v012(5, 0);
                s8 idx = (s8)h->field_0;
                s8 newIdx;
                if (h->field_5 == 0) {
                    newIdx = (s8)(idx + 1);
                    if (!((s16)v->mChainMember.mCount > (int)newIdx)) newIdx = 0;
                } else {
                    newIdx = (s8)(idx - 1);
                    if ((int)newIdx < 0) newIdx = (s8)((s16)v->mChainMember.mCount - 1);
                }
                h->field_0 = newIdx;
            }
        }
        cf::CChainActor* actor;
        if ((int)(s8)h->field_0 < (int)v->mChainMember.mCount) {
            actor = v->mChainMember.mActors[(s8)h->field_0];
        } else {
            actor = 0;
        }
        if (((cf::CChainActorVtIfB38*)actor)->v013() != 0) break;
        h->field_2 = 0x1a;
        break;
    }
    case 0x19: {
        // Chain teardown: clear the member indices, rebuild the actor list
        // from the member list, reset the chain voice and the state byte.
        h->field_0 = 0;
        h->field_1 = -1;
        func_8027BB4C(&self->mChainActorList, (cf::CChainList*)&self->mChainMember);
        func_8027711C(self);
        h->field_2 = 0;
        break;
    }
    case 0x1a:
        break;
    }
}
// Chain-start driver: resolve the current member actor, clear the per-chain
// state bytes, seed the chain timer (0x96) and the chain gauge from the
// battle-object slot 0x5b4 query, then fan the run key out to every member
// (slot 0x6c), arm the arts-select voice (sub8 slot 0x20), refresh the
// tracked player, and only when a voice is loaded re-arm the chain menu.
// Chain-start driver: resolve the current member actor, clear the per-chain
// state bytes, seed the chain timer (0x96) and the chain gauge from the
// battle-object slot 0x5b4 query, then fan the run key out to every member
// (slot 0x6c), arm the arts-select voice (sub8 slot 0x20), refresh the
// tracked player, and only when a voice is loaded re-arm the chain menu.
void func_80278E0C(cf::CChain* self) {
    cf::CChainMemberListMirror* v = (cf::CChainMemberListMirror*)self;
    s8 idx = (s8)self->unk0[0];
    int runKey;
    int i;
    cf::CChainActor* actor;
    cf::CChainActor** p;
    if ((int)idx < (int)v->mChainMember.mCount) {
        actor = v->mChainMember.mActors[idx];
    } else {
        actor = 0;
    }
    self->unk0[3] = 0;
    self->unk0[4] = 0;
    self->unk0[6] = 1;
    self->unk0[0xa] = 0;
    self->unk0[0x10] = 0;
    *(f32*)&self->unk0[0x14] = ((cf::CChainBattleObj5B4*)actor->unk0)->v363();
    self->mChainTimer1.unk0 = 0x96;
    runKey = ((cf::CChainActorVtIf2*)actor)->v024();
    for (i = 0, p = (cf::CChainActor**)self; i < (int)v->mChainMember.mCount;
         p++, i++) {
        // mActors sits at CChain+0x1DC8 (0x772 pointer slots from self).
        ((cf::CChainActorVtIf2*)p[0x772])->v025(runKey);
    }
    ((cf::CChainBattleObjE*)actor->unk0)->mSub8.e06(0xeb);
    u8* spot = (u8*)actor->unk0;
    if (spot != 0) spot += 0x3e9c;
    func_80082A7C__Q22cf13CfGameManagerFv(spot);
    if ((s8)self->unk0[1] != -1) {
        func_802AB474((CBattleChainMenuState*)&self->unk1F0C[0]);
        ((cf::CChainActorVtIf2*)actor)->v012(2, 0);
        func_8027C040((cf::CChainFlag*)actor->unk0);
    }
    self->unk0[2]++;
}
// Chain-extension / cancel driver: resolve the current member actor, then
// when the chain-extend flag (unk0[6]) is set re-query the battle object's
// slot-0x5b4 gauge and continue the chain (or reset it when the actor no
// longer reports an active chain / the gauge did not move / the first timer
// expired); otherwise run the cancel path (slot 0x24 with flag 1) and the
// shared chain-state step (slots 0x2c/0x58 of the manual vtable) driving the
// chain time, battle-chain menu and err-mes record.
void func_80278F84(cf::CChain* self) {
    cf::CChainActor* actor;
    u8 local;
    // Inline the mirror casts: keeping them in named locals costs extra
    // callee-saved registers versus the retail allocation (r30/r31 only).
    if ((int)(s8)self->unk0[0] <
        (int)((cf::CChainMemberListMirror*)self)->mChainMember.mCount) {
        actor = ((cf::CChainMemberListMirror*)self)
            ->mChainMember.mActors[(s8)self->unk0[0]];
    } else {
        actor = 0;
    }
    if (self->unk0[6] != 0) {
        // Chain-extend path: refresh the gauge from the battle object, then
        // decide whether the extension continues (state-1 err-mes register) or
        // resets (state-2 register). The timer check is shared between the
        // inactive-actor and moved-gauge cases (retail beq into the middle).
        f32 gauge = *(f32*)&self->unk0[0x14];
        *(f32*)&self->unk0[0x14] = ((cf::CChainBattleObj5B4*)actor->unk0)->v363();
        if (((cf::CChainActorVtIf84*)actor)->v009() != 0) {
            if (*(f32*)&self->unk0[0x14] == gauge) {
                goto resetExt;
            }
        }
        if (self->mChainTimer1.unk0 > 0) {
            goto contExt;
        }
    resetExt:
        // Timer expired / gauge did not move / actor inactive: reset the
        // extension state.
        func_802B4968((CErrMesEntry*)&self->unk1F0C[8],
                      (CErrMesOwner*)actor->unk0);
        func_802AB5E4((CBattleChainMenuState*)&self->unk1F0C[0]);
        self->mChainTimer2.unk0 = 0x2d;
        self->unk0[6] = 0;
        goto shared;
    contExt:
        // Chain still active and the gauge moved: keep extending.
        func_802B48E4((CErrMesEntry*)&self->unk1F0C[8],
                     (CErrMesOwner*)actor->unk0);
        goto shared;
    } else if (((cf::CChainActorVtIf84*)actor)->v007(1) == 0 &&
               self->mChainTimer2.unk0 <= 0) {
        // Cancel accepted: switch to the cancel state and skip the shared
        // chain-state step.
        self->unk0[2] = 0xd;
        self->unk0[0xa] = 1;
        return;
    }
shared:
    // Shared chain-state step: when the actor still reports an active chain
    // or the first timer expired, re-arm the A1C chain-time record;
    // otherwise the A18 one. Then run the chain-menu step.
    if (((cf::CChainActorVtIf84*)actor)->v009() != 0 ||
        self->mChainTimer1.unk0 <= 0) {
        self->mChainTime.mTimer = lbl_eu_80668A1C;
        self->mChainTime.mEnabled = 1;
        self->mChainTime.mPaused = 1;
    } else {
        self->mChainTime.mTimer = lbl_eu_80668A18;
        self->mChainTime.mEnabled = 0;
        self->mChainTime.mPaused = 1;
    }
    if (func_802AB510((CBattleChainMenuState*)&self->unk1F0C[0], &local) == 0) return;
    self->mChainTime.mTimer = lbl_eu_80668A18;
    self->mChainTime.mEnabled = 0;
    self->mChainTime.mPaused = 1;
    // The result byte is re-read from the stack for each branch (its address
    // was handed to func_802AB510).
    if (local != 0) {
        bool cond = false;
        if (((cf::CChainActorVtIf84*)actor)->v020() == 1) {
            u32 flags = ((cf::CChainBattleObj*)actor->unk0)->field_3374;
            cond = (flags & 0x4000) != 0 || (flags & 0x8000) != 0;
        }
        if (cond) {
            func_802B4968((CErrMesEntry*)&self->unk1F0C[8],
                          (CErrMesOwner*)actor->unk0);
            self->unk0[2] = 0x12;
            return;
        }
    }
    if (local != 0) {
        if (((cf::CChainActorVtIf84*)actor)->v020() == 4) {
            func_802B4A68((CErrMesEntry*)&self->unk1F0C[8],
                          (CErrMesOwner*)actor->unk0);
            self->unk0[2] = 0x14;
            return;
        }
    }
    func_802B4A68((CErrMesEntry*)&self->unk1F0C[8],
                  (CErrMesOwner*)actor->unk0);
    self->unk0[2] = 0xa;
}
// Chain-extension driver: resolve the member actor and its battle object,
// then gate on the arts-table entry (indexed by field_3590 % field_3598):
// when the entry is the special id-1 record (or the +0x3594 gate is clear)
// the actor-id checks (func_80174C98 with 0xA/0xB masks) decide whether to
// re-arm the chain timer; otherwise the chain just continues while the
// second timer is still running.
void func_80279214(cf::CChain* self) {
    cf::CChainMemberListMirror* v = (cf::CChainMemberListMirror*)self;
    s8 idx = (s8)self->unk0[0];
    cf::CChainActor* actor;
    if ((int)idx < (int)v->mChainMember.mCount) {
        actor = v->mChainMember.mActors[idx];
    } else {
        actor = 0;
    }
    cf::CChainBattleObj214* battleObj =
        (cf::CChainBattleObj214*)(actor != 0 ? actor->unk0 : 0);
    if (battleObj->field_3594 != 0) {
        u32 rem = battleObj->field_3590 % battleObj->field_3598;
        cf::CChainArt214Entry* entry =
            (cf::CChainArt214Entry*)(battleObj->field_358C + rem * 32);
        if (entry->field_0xD != 1) {
            if (self->mChainTimer2.unk0 <= 0) goto setTimer;
            return;
        }
    }
    // Special entry / cleared gate: the arts-select actor-id checks gate the
    // timer re-arm (both masks must match for the plain re-arm path).
    {
        int local1 = *(int*)battleObj->field_04->f30();
        int res1 = func_80174C98(battleObj, &local1, 0xA);
        if (res1 != 0) goto merge;
        int local2 = *(int*)battleObj->field_04->f30();
        int res2 = func_80174C98(battleObj, &local2, 0xB);
        if (res2 != 0) {
        merge:
            if (self->mChainTimer1.unk0 > 0) return;
            if (self->unk0[4] != 0) goto setTimer;
            return;
        }
        // Both actor-id checks passed: plain re-arm when the timer expired.
        if (self->mChainTimer1.unk0 > 0) return;
        goto setTimer;
    }
setTimer:
    self->mChainTimer1.unk0 = 2;
    self->unk0[2]++;
}
// Chain-start validation + activation: after the presentation gate
// (func_8006EF04(0x4000000)) and the battle-object / battle-manager checks,
// resolve the target battle object (explicit key, or the first player's
// embedded spot minus 0x3E9C - the resolution runs twice, once for the gate
// key and once for the activation target), activate the member actors
// (slot 0x6c fan-out), re-arm the actor reslist (slot 0x1c), and initialize
// every chain sub-object. Returns 1 on success.
int func_8027936C(cf::CChain* self, u32 param) {
    cf::CfGameManager::getInstance();
    if (func_8006EF04__Fi(0x4000000)) return 0;
    if (param != 0 && (((cf::CChainFlag*)param)->field_0x3F00 & 4) != 0) {
        if (func_800EA444(getInstance__Q22cf14CBattleManagerFv()) != 0) return 0;
    }
    if (param == 0) {
        void* player = cf::CfGameManager::getPlayer(0);
        if (player != 0) player = (u8*)player - 0x3e9c;
        param = (u32)player;
    }
    if (param == 0) return 0;
    u32 target = param;
    if (param == 0) {
        void* player = cf::CfGameManager::getPlayer(0);
        if (player != 0) player = (u8*)player - 0x3e9c;
        target = (u32)player;
    }
    int result;
    if (target == 0) {
        result = 0;
    } else {
        cf::CBattleManagerTail* bm =
            (cf::CBattleManagerTail*)getInstance__Q22cf14CBattleManagerFv();
        if (bm->field_0x20C8 != 0) {
            result = 0;
        } else {
            bm = (cf::CBattleManagerTail*)getInstance__Q22cf14CBattleManagerFv();
            if (func_801BA2C8(&bm->field_0x216C[0]) != 0) {
                result = 0;
            } else if (self->unk0[2] != 0) {
                result = 0;
            } else {
                result = func_8027BC14((cf::CChainActorList*)((u8*)self + 0x18), target);
            }
        }
    }
    if (result == 0) return 0;
    func_8027BA0C(&self->mChainActorList, (cf::CChainList*)&self->mChainMember,
                  (cf::CfObjectActor*)param);
    int moveSub;
    int i;
    cf::CChainActor** p;
    cf::CChainMemberListMirror* v = (cf::CChainMemberListMirror*)self;
    moveSub = ((CChainSubVtIf*)&((CChainBattleObjTail*)param)->field_0x3E9C)->v017();
    for (i = 0, p = (cf::CChainActor**)self; i < (int)v->mChainMember.mCount;
         p++, i++) {
        // mActors sits at CChain+0x1DC8 (0x772 pointer slots from self).
        ((cf::CChainActorVtIf2*)p[0x772])->v025(moveSub);
    }
    func_8027C924((cf::CChainList*)&self->mChainMember, 1);
    _reslist_node<cf::CChainActor*>* node =
        self->mChainActorList.mChainActorList.mStartNodePtr->mNext;
    while (node != self->mChainActorList.mChainActorList.mStartNodePtr) {
        ((cf::CChainActorVtIf2*)node->mItem)->v005(1);
        node = node->mNext;
    }
    self->mChainActorList.unk1DA8[0] = 1;
    func_8027CEB0(&self->mChainTime,
                  (u8)((((cf::CChainFlag*)param)->field_0x3F00 >> 1) & 1));
    func_8027C6B4((cf::CChainList*)&self->mChainMember, 0, -1);
    cf::CChainActor* actor = func_8027CA98((cf::CChainList*)&self->mChainMember, param);
    ((cf::CChainActorVtIf2*)actor)->v011();
    ((s8*)self->unk0)[1] = -1;
    self->unk0[0] = 0;
    self->unk0[8] = (u8)((((cf::CChainFlag*)param)->field_0x3F00 >> 1) & 1);
    reinterpret_cast<cf::CChainChanceTail*>(&self->mChainChance)->field_0x14 = 0;
    self->mChainCombo.func1();
    func_802AB3D0((CBattleChainMenuState*)&self->unk1F0C[0]);
    func_802B48A0((CErrMesEntry*)&self->unk1F0C[8]);
    func_8027BF58((cf::CChainFlag*)param);
    self->unk0[2] = 1;
    return 1;
}
// Chain-start validation: resolves the target battle object (explicit key, or
// the first player's embedded spot minus 0x3E9C), then gates on battle state
// (chain-state word, sudden-commu pause) and the chain-type byte before
// running the actor-list activation check.
int func_802795D4(cf::CChain* self, u32 param) {
    if (param == 0) {
        void* player = cf::CfGameManager::getPlayer(0);
        if (player != 0) player = (u8*)player - 0x3e9c;
        param = (u32)player;
    }
    if (param == 0) return 0;
    cf::CBattleManagerTail* bm = (cf::CBattleManagerTail*)getInstance__Q22cf14CBattleManagerFv();
    if (bm->field_0x20C8 != 0) return 0;
    bm = (cf::CBattleManagerTail*)getInstance__Q22cf14CBattleManagerFv();
    if (func_801BA2C8(&bm->field_0x216C[0]) != 0) return 0;
    if (self->unk0[2] != 0) return 0;
    return func_8027BC14((cf::CChainActorList*)((u8*)self + 0x18), param);
}

// Dispatches a chain-state change to every registered actor: first the actor
// reslist (walked through the slot-array head at mList), then the member
// list, then the actor-list removal / member-list append helpers. Retail
// keeps the second helper's compare as a dead instruction - the original
// source chained both results with `== 0` (empty body).
void func_80279694(cf::CChain* self, u32 param) {
    _reslist_node<cf::CChainActor*>* node;
    node = self->mChainActorList.mChainActorList.mStartNodePtr->mNext;
    while (node != self->mChainActorList.mChainActorList.mStartNodePtr) {
        ((cf::CChainActorVtIf64*)node->mItem)->v023(param);
        node = node->mNext;
    }
    cf::CChainMemberListMirror* v = (cf::CChainMemberListMirror*)self;
    for (int i = 0; i < (int)v->mChainMember.mCount; i++) {
        ((cf::CChainActorVtIf64*)v->mChainMember.mActors[i])->v023(param);
    }
    // Retail keeps the second helper's compare as a dead instruction: the
    // original source nested `if (func_8027C5E4(...) == 0) return;` inside
    // the outer gate (the trailing branch folds, the compare stays).
    if (func_8027B814((cf::CChainActorList*)((u8*)self + 0x18), param) == 0) {
        if (func_8027C5E4((cf::CChainList*)&self->mChainMember, param) == 0) {
            return;
        }
    }
}

// Scans the gimmick object list for a chain battle object (voice sub-object
// base minus 0x3E9C) whose battle id is 0x9C5; the first such object is
// validated through the word-holder / arts-category checks and returns
// whether its arts category byte equals 4.
// Retail passes the chain object in r3 at every call site (e.g.
// func_8027990C's `mr r3, r28; bl func_80279A4C`); the body ignores it.
// C linkage keeps the call reloc name verbatim.
extern "C" int func_80279A4C(cf::CChain* self) {
    CChainGimmickList* list = func_800B6BC8();
    CChainGimmickListNode* head = list->head;
    CChainGimmickListNode* node = head->next;
    while (node != head) {
        cf::CChainBattleObj2A4* obj;
        if (node->object != 0) {
            obj = (cf::CChainBattleObj2A4*)((u8*)node->object - 0x3e9c);
        } else {
            obj = 0;
        }
        if (obj->field_3F28 == 0x9C5) {
            int local = *(int*)((cf::CChainSub4*)obj->field_04)->f30();
            if (func_80174C98(obj, &local, 0xA) == 0) return 0;
            CChainCombo_ArtsCategoryHolder* holder = obj->v167();
            if (holder->mArtsCategory == 0) return 0;
            return (((cf::CChainArtsCat77*)holder->mArtsCategory)->field_0x77 == 4)
                       ? 1
                       : 0;
        }
        node = node->next;
    }
    return 0;
}
// Chain-start gating: returns 1 for gauge-range chain types (0xA..0xC);
// otherwise re-validates the type range and requires the target to resolve
// to the first player's embedded spot before allowing the actor's manual
// vtable slot 0x28 hook (whose result becomes the return value).
int func_80279778(cf::CChain* self, CChainBattleObjTail* target) {
    int valid = 0;
    if (1 <= self->unk0[2] && self->unk0[2] <= 0x18) valid = 1;
    if (valid == 0) return 0;
    cf::CChainActor* actor = func_8027CA98((cf::CChainList*)&self->mChainMember, (u32)target);
    if (actor == 0) return 0;
    u32 v = self->unk0[2];
    // Retail lowers the [0xA, 0xC] check to bit arithmetic: the sign bit of
    // (v | ~0xA) - ((v - 0xA) >> 1) is v >= 0xA, and of
    // (0xC | ~v) - ((0xC - v) >> 1) is v <= 0xC (u32 wraps make both
    // byte-exact).
    int r = (int)(((v | ~0xA) - ((v - 0xA) >> 1)) >> 31);
    if (r != 0) {
        r = (int)(((0xC | ~v) - ((0xC - v) >> 1)) >> 31);
    }
    if (r == 0) {
        int valid2 = 0;
        if (1 <= self->unk0[2] && self->unk0[2] <= 0x18) valid2 = 1;
        if (valid2 == 0) {
            r = 0;
        } else {
            u8* p = (u8*)target;
            if (p != 0) p += 0x3e9c;
            if (p != (u8*)cf::CfGameManager::getPlayer(0)) {
                r = 0;
            } else {
                r = self->unk0[0x10];
            }
        }
    }
    if (r == 0) {
        r = ((cf::CChainActorVtIf28*)actor)->v008();
    }
    return r;
}

// Chain-actor availability probe: valid only for chain types 1..0x18, then
// resolves the member-list entry for the target battle object and gates on
// the chain battle-object availability (func_80279A4C) plus the target's
// +0x3F60 flag word bit 16 (0x10000).
int func_8027990C(cf::CChain* self, CChainBattleObjTail* target) {
    u32 v = self->unk0[2];
    int ok = 0;
    if (1 <= v && v <= 0x18) ok = 1;
    // The explicit else keeps MWCC's two-phase shape: the invalid path
    // re-writes ok = 0 before merging (retail `li r0,0; b`), with the
    // member lookup as the branch target.
    if (ok == 0) {
        ok = 0;
    } else {
        ok = func_8027CA98((cf::CChainList*)&self->mChainMember, (u32)target) != 0;
    }
    int result = 0;
    int memberOk = 0;
    if (ok != 0) {
        if (func_80279A4C(self) == 0) memberOk = 1;
    }
    if (memberOk != 0) {
        cf::CChainField3F60* f = (cf::CChainField3F60*)target->field_0x3F60;
        int flag = (f != 0 && (f->field_4EC & 0x10000) != 0);
        if (flag == 0) result = 1;
    }
    return result;
}
// Chain-membership probe: valid only for chain types 1..0x18, then reports
// whether the member list holds an entry whose unk0 equals @p target.
// (Separate ifs + goto: a flat `&&` makes MWCC fold the range to (v-1)<=23,
// which retail does not have - two explicit cmplwi instead.)
// Chain-membership probe: valid only for chain types 1..0x18, then reports
// whether the member list holds an entry whose unk0 equals @p target.
// u32 v + `1 <= v` (constant first): both defeat MWCC's range fold (which
// would emit a single (v-1)<=23 instead of retail's two cmplwi).
bool func_802799F0(cf::CChain* self, CChainBattleObjTail* target) {
    u32 v = self->unk0[2];
    int valid = 0;
    if (1 <= v && v <= 0x18) valid = 1;
    if (valid == 0) return false;
    return func_8027CA98((cf::CChainList*)&self->mChainMember, (u32)target) != 0;
}
// Chain-end voice cleanup: when the chain-cancel voice path is enabled
// (unk6C bit 0) silence every registered arts voice id on the battle
// object's embedded sub-object and clear the +0x3E98 word; when the move
// flag path is enabled (unk6C bit 1) set the +0x3388 bit-1 flag, drop the
// 0x4000 move flag (actor-id check), re-arm the voice-act marker, and clear
// the flag again at the end.
void func_80279B34(cf::CChainActor* self) {
    if (self->unk6C & 1) {
        if (func_80148778(&((cf::CChainBattleObjE*)self->unk0)->mSub8, 0x6) != 0)
            ((cf::CChainBattleObjE*)self->unk0)->mSub8.e06(0x6);
        if (func_80148778(&((cf::CChainBattleObjE*)self->unk0)->mSub8, 0x9) != 0)
            ((cf::CChainBattleObjE*)self->unk0)->mSub8.e06(0x9);
        if (func_80148778(&((cf::CChainBattleObjE*)self->unk0)->mSub8, 0xa) != 0)
            ((cf::CChainBattleObjE*)self->unk0)->mSub8.e06(0xa);
        if (func_80148778(&((cf::CChainBattleObjE*)self->unk0)->mSub8, 0xb) != 0)
            ((cf::CChainBattleObjE*)self->unk0)->mSub8.e06(0xb);
        if (func_80148778(&((cf::CChainBattleObjE*)self->unk0)->mSub8, 0xc) != 0)
            ((cf::CChainBattleObjE*)self->unk0)->mSub8.e06(0xc);
        if (func_80148778(&((cf::CChainBattleObjE*)self->unk0)->mSub8, 0xf) != 0)
            ((cf::CChainBattleObjE*)self->unk0)->mSub8.e06(0xf);
        if (func_80148778(&((cf::CChainBattleObjE*)self->unk0)->mSub8, 0x10) != 0)
            ((cf::CChainBattleObjE*)self->unk0)->mSub8.e06(0x10);
        if (func_80148778(&((cf::CChainBattleObjE*)self->unk0)->mSub8, 0xcb) != 0)
            ((cf::CChainBattleObjE*)self->unk0)->mSub8.e06(0xcb);
        if (((cf::CChainBattleObjE*)self->unk0)->field_0x3E98 != 0)
            ((cf::CChainBattleObjE*)self->unk0)->field_0x3E98 = 0;
    }
    if (self->unk6C & 2) {
        // volatile RMW keeps the retail reload of the flag word (the test
        // load and the or/store load are two separate lhz in the retail).
        if ((((cf::CChainBattleObjF*)self->unk0)->field_0x3388 & 2) == 0) {
            *(volatile u16*)&((cf::CChainBattleObjF*)self->unk0)->field_0x3388 |= 2;
        }
        cf::CChainBattleObjE* battleObj = (cf::CChainBattleObjE*)self->unk0;
        int local = *(int*)battleObj->field_4->f30();
        if (func_80174C98(battleObj, &local, 0x4000) == 0) {
            func_80174B4C((cf::CChainBattleObjE*)self->unk0, 0x4000);
        }
    }
    if (self->unk6C & 1) {
        if ((*(u16*)((u8*)&((cf::CChainBattleObjE*)self->unk0)->mField3380 + 8) & 4) == 0) {
            func_801537F0(&((cf::CChainBattleObjE*)self->unk0)->mField3380);
        }
    }
    if (self->unk6C & 2) {
        if ((((cf::CChainBattleObjF*)self->unk0)->field_0x3388 & 2) == 0)
            *(volatile u16*)&((cf::CChainBattleObjF*)self->unk0)->field_0x3388 |= 2;
    }
}
// Chain-end cleanup: run the actor's begin/end hooks (vtable slots 6/7) on
// the target's embedded sub-object when a target is set, then clear the
// target pointer and its flag word.
void func_80279DC0(cf::CChainActor* self) {
    CChainBattleObjTail* p = (CChainBattleObjTail*)self->unk0;
    if (p != NULL) {
        // Dead second null-check keeps the retail duplicated-branch shape
        // around the +0x3e9c re-bias.
        if (p != NULL) {
            p = (CChainBattleObjTail*)((u8*)p + 0x3e9c);
        }
        if (func_800B8920(p) != 0) {
            ((cf::CChainActorVtIfDC0*)self)->v004(0);
            ((cf::CChainActorVtIfDC0*)self)->v005(0);
        }
    }
    self->unk0 = 0;
    self->unk6C = 0;
}
// Toggles the chain-cancel/voice enable flag on the chain actor: the enabled
// side re-registers the music table and voice-act sub-object and starts the
// voice (vtable slots 0x14/0x20 on the +0x8 sub-object); the disabled side
// clears the voice-act sub-object. Skips when the requested state already
// matches unk6C bit 1.
void func_80279E48(cf::CChainActor* self, u32 param) {
    if (param != (self->unk6C & 1)) {
        if (param != 0) {
            func_8027CC3C(&self->mChainTemp, (void*)self->unk0);
            ((cf::CChainBattleObjE*)self->unk0)->mSub8.e03(0x2e);
            if (func_80148778(&((cf::CChainBattleObjE*)self->unk0)->mSub8, 0x11) != 0) {
                ((cf::CChainBattleObjE*)self->unk0)->mSub8.e06(0x11);
            }
            func_801537F0(&((cf::CChainBattleObjE*)self->unk0)->mField3380);
            ((cf::CChainBattleObj2A4*)self->unk0)->v042();
        } else {
            func_8027CD08(&self->mChainTemp, (void*)self->unk0);
            func_8014AE00(&((cf::CChainBattleObjE*)self->unk0)->mField3380);
            func_801537E0(&((cf::CChainBattleObjE*)self->unk0)->mField3380);
            ((cf::CChainBattleObjE*)self->unk0)->mSub8.e06(0x2e);
        }
        if (param != 0) {
            self->unk6C |= 1;
        } else {
            self->unk6C &= ~1;
        }
    }
}
// Toggles the chain-cancel/voice flag on the actor's battle object: the
// enabled side ORs bit 2 into the +0x3388 flag word and disables the voice
// manager; the disabled side resets the voice-act sub-object and its move
// flag. Skips when the requested state already matches unk6C bit 1.
void func_80279F6C(cf::CChainActor* self, u32 param) {
    if (param != ((self->unk6C >> 1) & 1)) {
        if (param != 0) {
            ((cf::CChainBattleObjF*)self->unk0)->field_0x3388 |= 2;
            func_80174B4C((void*)self->unk0, 0x4000);
        } else {
            func_801537E0(&((cf::CChainBattleObjF*)self->unk0)->field_0x3380);
            ((cf::CChainBattleObjF*)self->unk0)->field_4->f06(0x4000);
        }
        if (param != 0) {
            self->unk6C |= 2;
        } else {
            self->unk6C &= ~2;
        }
    }
}
// Chain-start availability probe: the chain can only start when the arts-flag
// resource (0x3357) is set, the actor's manual vtable slot 0x70 rejects the
// chain, every battle-object voice sub-id (0x9..0xcb) is free, the optional
// target's embedded sub-object is within the chain-voice distance threshold
// (paired-single VEC3 length), and the voice sub-object's +0xc4 target is in
// the expected state; the final slot-0xc call result decides.
int func_8027A024(cf::CChainActor* self, int param) {
    if (func_8009CF8C((u32)0x3357) == 0) return 0;
    if (((cf::CChainActorVtIf70*)self)->v026() != 0) return 0;
    cf::CChainBattleObj* battleObj = (cf::CChainBattleObj*)self->unk0;
    int local = *(int*)battleObj->field_04->f30();
    if (func_80174C98(battleObj, &local, 0x1a) != 0) return 0;
    // Retail reloads self->unk0 before every voice sub-id check (the bctrl
    // may have changed it), so the field is re-read inline, not cached.
    if (func_80148778(&((cf::CChainBattleObj*)self->unk0)->mSub8, 0x9) != 0)
        return 0;
    if (func_80148778(&((cf::CChainBattleObj*)self->unk0)->mSub8, 0xa) != 0)
        return 0;
    if (func_80148778(&((cf::CChainBattleObj*)self->unk0)->mSub8, 0xb) != 0)
        return 0;
    if (func_80148778(&((cf::CChainBattleObj*)self->unk0)->mSub8, 0xc) != 0)
        return 0;
    if (func_80148778(&((cf::CChainBattleObj*)self->unk0)->mSub8, 0xf) != 0)
        return 0;
    if (func_80148778(&((cf::CChainBattleObj*)self->unk0)->mSub8, 0x10) != 0)
        return 0;
    if (func_80148778(&((cf::CChainBattleObj*)self->unk0)->mSub8, 0xcb) != 0)
        return 0;
    int distOk = 0;
    if (param != 0) {
        void* src = func_8016FE34(func_800B708C(param));
        if (src != 0) {
            nw4r::math::VEC3* targetPos =
                ((cf::CChainVoiceSub*)((u8*)src + 0x3e9c))->v41();
            nw4r::math::VEC3* selfPos =
                ((cf::CChainVoiceSub*)((u8*)self->unk0 + 0x3e9c))->v41();
            nw4r::math::VEC3 delta;
            nw4r::math::VEC3 scratch;
            nw4r::math::VEC3* pDelta = &delta;
            nw4r::math::VEC3* pScratch = &scratch;
            nw4r::math::VEC3Sub(pDelta, selfPos, targetPos);
            // The diff is materialized to a second stack slot (lfs/stfs
            // round-trip) before the paired-single length-sq.
            scratch.x = delta.x;
            scratch.y = delta.y;
            scratch.z = delta.z;
            f32 distSq = nw4r::math::VEC3LenSq(pScratch);
            // Two separate threshold compares (retail duplicates the length-sq
            // result test per chain-cancel-voice flag path).
            if ((self->unk6C & 1) == 0) {
                distOk = (distSq < lbl_eu_80668A50) ? 1 : 0;
            } else {
                distOk = (distSq < lbl_eu_80668A54) ? 1 : 0;
            }
        }
    }
    if (distOk == 0) return 0;
    cf::CChainVoiceSubC* voiceSub =
        (cf::CChainVoiceSubC*)((u8*)self->unk0 + 0x3e9c);
    int flag = 1;
    if (voiceSub->field_C4 != 0 &&
        (((cf::CChainVoiceSubC4EC*)voiceSub->field_C4)->field_4EC & 0x10000) !=
            0) {
        flag = 0;
    }
    if (flag != 0) return 0;
    return voiceSub->v01(0x200) == 0;
}
// Arts-select availability probe: for every arts slot (0..8), if the arts
// select menu flags the slot as usable (or no menu is up) but the slot's
// arts param fails its usage check (func_80154280), the chain cannot start.
// The per-slot flag differs by path: menu-gated = 0, no-menu = 0x20,
// chain-cancel-voice path = -0x21 (0xFFFFFFDF).
int func_8027A338(cf::CChainActor* self, int param) {
    if (self->unk6C & 1) {
        if (param != 0) {
            // Menu-gated path: the slot must pass the chainable probe and the
            // arts-select menu's own slot check, else the arts param test.
            CMenuArtsSelect* menu = CMenuArtsSelect_getInstance();
            if (menu == 0) return 1;
            for (int i = 0; i <= 8; i++) {
                if (((cf::CChainActorVtIf2*)self)->v021() != 0 &&
                    func_80107C54(menu, i) != 0)
                    continue;
                int res;
                if (i < 8) {
                    cf::CChainBattleObj* battleObj = (cf::CChainBattleObj*)self->unk0;
                    res = func_80154280(getArtsParamAtCnt(battleObj->v157(), i),
                                        battleObj, 0);
                } else if (i == 8) {
                    res = func_80154280(
                        getArtsParamRC(((cf::CChainBattleObj*)self->unk0)->v157(), 2, 0),
                        (cf::CChainBattleObj*)self->unk0, 0);
                } else {
                    res = 0;
                }
                if (res == 0) return 1;
            }
            return 0;
        }
        for (int i = 0; i <= 8; i++) {
            int res;
            if (i == 8) {
                res = func_80154280(
                    getArtsParamRC(((cf::CChainBattleObj*)self->unk0)->v157(), 2, 0),
                    (cf::CChainBattleObj*)self->unk0, 0x20);
            } else {
                cf::CChainBattleObj* battleObj = (cf::CChainBattleObj*)self->unk0;
                res = func_80154280(getArtsParamAtCnt(battleObj->v157(), i),
                                    battleObj, 0x20);
            }
            if (res == 0) return 1;
        }
        return 0;
    }
    for (int i = 0; i <= 8; i++) {
        int res;
        if (i == 8) {
            res = func_80154280(
                getArtsParamRC(((cf::CChainBattleObj*)self->unk0)->v157(), 2, 0),
                (cf::CChainBattleObj*)self->unk0, -0x21);
        } else {
            cf::CChainBattleObj* battleObj = (cf::CChainBattleObj*)self->unk0;
            res = func_80154280(getArtsParamAtCnt(battleObj->v157(), i),
                                battleObj, -0x21);
        }
        if (res == 0) return 1;
    }
    return 0;
}
void func_8027A58C(){}
// Chain-voice usability check: probes the embedded voice sub-object's timing
// (vtable slot 0x110) against the lbl_eu_80668A60 threshold, then validates
// the battle object's actor ids (func_80174C98 with 0x1000 / 0x6 masks) and
// rejects the special 0x31 battle-object id.
int func_8027A8C8(cf::CChainVoiceHolder* self) {
    f32 t = self->field_0->mSub.v066()->field_14;
    if (lbl_eu_80668A60 < t) return 0;
    cf::CChainBattleObjVoice* battleObj = self->field_0;
    int local1 = *(int*)battleObj->field_04->f30();
    if (func_80174C98(battleObj, &local1, 0x1000) != 0) return 0;
    cf::CChainField3F60* f = self->field_0->field_3F60;
    if (f != 0 && func_8004C5EC(f) == 0x31) return 0;
    cf::CChainBattleObjVoice* battleObj2 = self->field_0;
    int local2 = *(int*)battleObj2->field_04->f30();
    if (func_80174C98(battleObj2, &local2, 0x6) == 0) return 0;
    return 1;
}

extern "C" void CChain_setFieldAndClear(void* self, int val) {
    *(int*)self = val;
    *(u16*)((u8*)self + 0x6c) = 0;
}
