#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/menu/code_8018C5FC.hpp"
#include "kyoshin/cf/chain/CChainApi.hpp"
#include "kyoshin/cf/object/CfObjectActor.hpp"
#include "kyoshin/code_802B8A3C.hpp"  // findObjectById imports
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)

#include "kyoshin/cf/CfMapItemManager.hpp"  // dispatchItemLoad / testActorStatus


// Object behind
struct CfObjectActorInner {
    u8 _pad00[0x37c];
    u32 mArgStorage;  // 0x37c
};

namespace cf {

CfObjectActor::CfObjectActor() : CActorParam(static_cast<CfObjectMove*>(this), NULL) {
    dispatchItemLoad(this, 3);
    CAIAction_setOwner(reinterpret_cast<u32>(this));
    mActionSrcHandle = 0;
    mActionSrcId = -1;
}

// Both bases carry a name slot (CBattleState via CActorParam, and
// CObjectParam via CfObjectMove); the override picks the object's name.
void* CfObjectActor::CObjectParam_getParamPtr() {
    return CfObjectMove::CObjectParam_getParamPtr();
}

} // namespace cf

// Retail defs owned by this TU's split range (0x80171310/0x80171EAC); the
// bodies are CVision logic awaiting that wave's decomp. extern "C" + real
// arity per CVision.hpp so this TU provides the retail symbols (C++
// linkage would mangle them __Fv and leave CVision.o's refs dangling).
extern "C" void Vision_CopyEngageActor(void* obj, void* dst) { (void)obj; (void)dst; }
extern "C" void Vision_CopyFxActor(void* self, void* dst) { (void)self; (void)dst; }

namespace cf {

void CfObjectActor::CActorParam_addHate(void* arg, float a, float b, float c) {
    if (arg == 0) return;
    u32 selfId = field_0x74;
    if (selfId == static_cast<cf::CfObjectActor*>(arg)->field_0x74) return;
    // Non-virtual call of the CActorParam base (retail: tail-branch).
    this->cf::CActorParam::CActorParam_addHate(arg, a, b, c);
}

void CfObjectActor::CBattleState_notifyEntryUpdated(CBattleStateEntry* arg) {
    CActorParam::CBattleState_notifyEntryUpdated(arg);
    switch (arg->unk0C) {
    case 0xE5:
        CfObject_syncModelRate(lbl_eu_8066773C);
        break;
    case 0x10: {
        if ((unk3374 & 0x40000) != 0 && unk1634 != 0) {
            cf::CBattleStateEntry buf;
            std::memset(&buf, 0, 0x34);
            buf.unk00 = field_0x74;
            buf.unk04 = 0;
            buf.unk0C = (u16)unk1634;
            buf.unk10 = unk163C;
            buf.unk14 = (u16)unk1638;
            buf.unk16 = (u16)unk1640;
            buf.unk18 = (u16)unk1644;
            buf.unk20 = 0.0f;
            buf.unk30 = 0x10001;
            CBattleState_enterStatusEntry(&buf);
        }
        break;
    }
    default:
        break;
    }
    static_cast<cf::CfObjectSub38*>(mSubObj38)->forwardActorEvent(arg);
}

void CfObjectActor::CBattleState_getLinkedActorId(CBattleStateEntry* arg) {
    CActorParam::CBattleState_getLinkedActorId(arg);
    int v = arg->unk0C;
    switch (v) {
    case 0xE5:
        CfObject_syncModelRate(0.0f);
        break;
    case 0x10: {
        if ((unk3374 & 0x40000) != 0 && unk1634 != 0) {
            CBattleState_clearStatusId((int)unk1634);
        }
        break;
    }
    default:
        break;
    }
    static_cast<cf::CfObjectSub38*>(mSubObj38)->forwardActorEventEnd(arg);
}

void CfObjectActor::CfObjectActor_clearStatusPair() {
    // Two virtual calls on the CBattleState subobject vtable (this+0x8),
    // slot +0x20 (CBattleState_clearStatusId; retail fake-Fv ABI passes
    // the status id in r4). Calling through `this` directly lets MWCC keep
    // `this` in r31 and recompute the +8 adjusted-this per call (retail
    // shape); the reinterpret_cast form CSEs this+8 into r31.
    this->CBattleState_clearStatusId(0xf);
    this->CBattleState_clearStatusId(0x10);
}

void CfObjectActor::CfObjectActor_flushStatusPair() {
    // Two calls to the CBattleState subobject vtable slot +0x20 (retail
    // passes an int through the slot; fake-Fv ABI). Same shape as
    // CfObjectActor_clearStatusPair: calling through `this` keeps this in
    // r31 and recomputes the +8 adjusted-this per call.
    this->CBattleState_clearStatusId(0xf);
    this->CBattleState_clearStatusId(0x10);
}

void CfObjectActor::CActorParam_setHp(float val) {
    // The direct cast makes MWCC synthesize the shared 2^52 magic double;
    // the decomp pools an @N entry of the same value at the same reloc site
    // (name-only drift - accepted, cf. getAdjustedFacing).
    float f = (float)(int)val;
    if ((gmGameFlags & 0x10000000) == 0 && (gmPauseFlags & 0x800) == 0) {
        if (f < 1.0f) {
            void* bm = cf::CBattleManager::getInstance();
            if (CChain_ProbeActorReady((cf::CChain*)((u8*)bm + 0x1A8), (CChainBattleObjTail*)this) != 0) {
                f = 1.0f;
            }
        }
        unk17E4.field_04 = f;
        releaseVisionSlot(this, 0);
    }
}

void CfObjectActor::CActorParam_applyDamage(float value, int a, int b, int c) {
    float f = (float)(int)value;   // f31: rounded gauge delta
    if ((gmGameFlags & 0x10000000) != 0) return;
    if ((gmPauseFlags & 0x800) != 0) return;
    int v27 = 0;
    getInstance__Q22cf13CfGameManagerFv();
    if (isGlobalCamFlagSet(0x04000000) != 0) {
        cf::CfPtmf3 ptmf;
        ptmf.field_0 = sVisionPairEngagePtmf[0];
        ptmf.field_4 = sVisionPairEngagePtmf[1];
        ptmf.field_8 = sVisionPairEngagePtmf[2];
        if (__ptmf_cmpr((u8*)cf::CBattleManager::getInstance() + 0x28354, &ptmf) == 0) {
            v27 = 1;
        }
    }
    if (v27 != 0) return;

    c &= ~3;   // low 2 flag bits are cleared in place (retail clrlwi r31, r31, 2)
    if (f > 0.0f) {
        BtlDmg_SetField779((c & (0x80000000 | 0x200)) != 0);
        BtlDmg_SetByte778(0);
        BtlDmg_SetByte777(0);
    } else {
        BtlDmg_SetField779((c & (0x80000000 | 0x100)) != 0);
        BtlDmg_SetByte778((c & (0x80000000 | 0x400)) != 0);
        if ((c & 0x80500000) != 0) {
            BtlDmg_SetByte777(1);
        } else if ((c & 0x80A00000) != 0) {
            BtlDmg_SetByte777(2);
        } else {
            BtlDmg_SetByte777(0);
        }
    }
    if (f < 0.0f && isBattleStatusActive((u8*)this + 8, 1) != 0) {
        f = 0.0f;
    }
    // Status 0x33: adjust the gauge value inside the status entry. `ep` is
    // one register reused across every status block (retail keeps the entry
    // pointers in a single callee-saved register).
    cf::CfStatusEntry* ep = 0;
    if (isBattleStatusActive((u8*)this + 8, 0x33) != 0) {
        if ((c & 0x90000000) == 0) {
            ep = (cf::CfStatusEntry*)findBattleStatusEntry((u8*)this + 8, 0x33);
            if (ep != 0) {
                if ((c & (0x80000000 | 0x8000)) != 0) {
                    BtlDmg_FilterNotifyDamage((void*)field_0x74, 3, 0x10);
                    if (f > 0.0f) {
                        ep->field_0x14 = (s16)(s32)f;
                        releaseVisionSlot(this, 0);
                        return;
                    }
                    CBattleState_applyEventEntry((cf::CBattleStateEntry*)ep);
                    if (0.0f == f) return;
                } else {
                    if (f <= 0.0f) {
                        ep->field_0x14 = (s16)((float)(s16)ep->field_0x14 + f);
                        BtlDmg_FilterNotifyDamage((void*)field_0x74, 3, 0x10);
                        if ((s16)ep->field_0x14 > 0) {
                            releaseVisionSlot(this, 0);
                            return;
                        }
                        f = (float)(s16)ep->field_0x14;
                        CBattleState_applyEventEntry((cf::CBattleStateEntry*)ep);
                        if (0.0f == f) return;
                    }
                }
            }
        }
    }
    if (f > 0.0f && isBattleStatusActive((u8*)this + 8, 0x120) != 0) return;
    // Round the gauge to the nearest int (half away from zero), then clamp.
    float v = unk17E4.field_04 + f;
    unk17E4.field_04 = v;
    double round;
    if (v > 0.0f) {
        round = 0.5;
    } else {
        round = -0.5;
    }
    v = (float)(s32)((double)v + round);
    unk17E4.field_04 = v;
    if (v < 1.0f) {
        unk17E4.field_04 = 0.0f;
    }
    v = unk17E4.field_04;
    if (v < 0.0f) {
        unk17E4.field_04 = 0.0f;
    } else if (v > unk17E4.field_10) {
        unk17E4.field_04 = unk17E4.field_10;
    }
    BtlDmg_SetDamageType((u8)a);
    BtlDmg_SetDamageDir((u8)b);
    if (f <= 0.0f) {
        BtlDmg_NotifyDamage((void*)field_0x74, (u32)(s32)-f);
        CChain_AccumGaugeDelta((cf::CChain*)((u8*)cf::CBattleManager::getInstance() + 0x1A8), (cf::CChainFlag*)this, -f);
    } else {
        BtlDmg_NotifyDamage((void*)field_0x74, (u32)(s32)f);
    }
    if (f <= 0.0f) {
        // Status 0xC3: scale a ratio from the entry value.
        if (isBattleStatusActive((u8*)this + 8, 0xC3) != 0) {
            ep = (cf::CfStatusEntry*)findBattleStatusEntry((u8*)this + 8, 0xC3);
            if (ep != 0) {
                f = 1.0f;
                if (ep->field_0x10 != 0) {
                    float g = this->CActorParam_getDamageScale();
                    f = (float)(s32)ep->field_0x10 * (g / 100.0f);
                }
                if (this->CActorParam_getHp() < f) {
                    this->CActorParam_setHp(f);
                }
            }
        }
        // Status 0x92: party-list length gated proc.
        if (isBattleStatusActive((u8*)this + 8, 0x92) != 0) {
            void* bm = cf::CBattleManager::getInstance();
            u8* base = *(u8**)((u8*)bm + 8);
            u8* node = *(u8**)base;
            int count = 0;
            while (node != base) {
                node = *(u8**)node;
                count++;
            }
            if (count != 0) {
                int v27b = 0;
                if (isBattleStatusActive((u8*)this + 8, 0xA1) != 0) {
                    v27b = ((cf::CfStatusEntry*)findBattleStatusEntry((u8*)this + 8, 0xA1))->field_0x10;
                }
                ep = (cf::CfStatusEntry*)findBattleStatusEntry((u8*)this + 8, 0x92);
                if (ep != 0) {
                    if (this->CActorParam_getHp() < 1.0f) {
                        s32 val = ep->field_0x10;
                        if (ep->field_0x8 == 0x2000) val += v27b;
                        if ((c & 0xA0000000) != 0) {
                            this->CActorParam_setHp(1.0f);
                        } else if (rand() % 100 < val) {
                            this->CActorParam_setHp(1.0f);
                        }
                    }
                }
            }
        }
        // Status 0xFC: gauge drain block.
        if (isBattleStatusActive((u8*)this + 8, 0xFC) != 0) {
            ep = (cf::CfStatusEntry*)findBattleStatusEntry((u8*)this + 8, 0xFC);
            if (ep != 0) {
                if (this->CActorParam_getHp() < 1.0f) {
                    float g = this->CActorParam_getDamageScale();
                    f = lbl_eu_80667764 * ((float)(s32)ep->field_0x10 * g);
                    cf::CBattleManager::getInstance();
                    f *= calcBattleDamage(0, this, 0);
                    this->CActorParam_addHp(f);
                    CBattleState_clearStatusId(0xFC);
                    cf::PartyGaugeAddClamped(reinterpret_cast<cf::UnkClass_8018C5FC*>((u8*)cf::CBattleManager::getInstance() + 0x194), 0x32);
                }
            }
        }
        // Status 0x100: same drain block plus a decay add and flag.
        if (isBattleStatusActive((u8*)this + 8, 0x100) != 0) {
            ep = (cf::CfStatusEntry*)findBattleStatusEntry((u8*)this + 8, 0x100);
            if (ep != 0) {
                if (this->CActorParam_getHp() < 1.0f) {
                    float g = this->CActorParam_getDamageScale();
                    f = lbl_eu_80667764 * ((float)(s32)ep->field_0x10 * g);
                    cf::CBattleManager::getInstance();
                    f *= calcBattleDamage(0, this, 0);
                    this->CActorParam_addHp(f);
                    CBattleState_clearStatusId(0x100);
                    this->CActorParam_addArtsGauge(50.0f);
                    CBattleMan_ClearEventNotify(cf::CBattleManager::getInstance(), (u32)this, (u16)0x93);
                }
            }
        }
    }
    if (this->CActorParam_getHp() < 1.0f) {
        if (CChain_ProbeActorReady((cf::CChain*)((u8*)cf::CBattleManager::getInstance() + 0x1A8), (CChainBattleObjTail*)this) != 0) {
            this->CActorParam_setHp(1.0f);
        }
    }
    releaseVisionSlot(this, 0);
}

void CfObjectActor::CActorParam_addHp(float value) {
    if (value <= 0.0f) {
        this->CActorParam_applyDamage(value, 0, 0, 0);
    } else {
        this->CActorParam_applyDamage(value, 3, 0, 0);
    }
}

void CfObjectActor::CfObjectActor_takeDamageValue(float value, int arg) {
    if (value <= 0.0f) {
        this->CActorParam_applyDamage(value, 0, 0, arg);
    } else {
        this->CActorParam_applyDamage(value, 3, 0, arg);
    }
}

void CfObjectActor::CActorParam_addGauge(int delta) {
    s16 cur = unk160C;
    s16 max = unk160E;
    // Explicit (s16) truncation of the int param -> retail's extsh r0, r4
    // (an s16-typed param lets MWCC fold the sign-extension into the add).
    int sum = cur + (s16)delta;
    unk160C = (u16)sum;
    if ((s16)sum < 0) {
        unk160C = 0;
    } else if ((s16)sum > max) {
        unk160C = (u16)max;
    }
    u32* idPtr = reinterpret_cast<u32*>(
        reinterpret_cast<cf::CObjectState*>(
            reinterpret_cast<cf::CActorState*>(this)->unk4)
            ->getStateData());
    u32 id = *idPtr;
    if (testActorStatus(this, (int*)&id, 0x802) != 0) {
        // MWCC evaluates == right-to-left: retail calls 0x178 first, 0x174 second.
        if (this->CActorParam_getGauge() == this->CActorParam_getGaugeMax()) {
            int mx = this->CActorParam_getGaugeMax();
            this->CActorParam_setGauge(mx - 1);
        }
    }
}

void CfObjectActor::CActorParam_addSecondGauge(int delta) {
    s16 cur = unk1614;
    s16 max = unk1616;
    // Explicit (s16) truncation of the int param -> retail's extsh r0, r4
    // (an s16-typed param lets MWCC fold the sign-extension into the add).
    int sum = cur + (s16)delta;
    unk1614 = (u16)sum;
    if ((s16)sum < 0) {
        unk1614 = 0;
    } else if ((s16)sum > max) {
        unk1614 = (u16)max;
    }
    u32* idPtr = reinterpret_cast<u32*>(
        reinterpret_cast<cf::CObjectState*>(
            reinterpret_cast<cf::CActorState*>(this)->unk4)
            ->getStateData());
    u32 id = *idPtr;
    if (testActorStatus(this, (int*)&id, 0x802) != 0) {
        s16 cur2 = unk1614;
        s16 max2 = unk1616;
        int sum2 = cur2 + (s16)delta;
        unk1614 = (u16)sum2;
        if ((s16)sum2 < 0) {
            unk1614 = 0;
        } else if ((s16)sum2 > max2) {
            unk1614 = (u16)max2;
        }
        // MWCC evaluates == right-to-left: retail calls 0x190 first, 0x18C second.
        if (this->CActorParam_getSecondGauge() == this->CActorParam_getSecondGaugeMax()) {
            int mx = this->CActorParam_getSecondGaugeMax();
            this->CActorParam_setSecondGauge(mx - 1);
        }
    }
}

void CfObjectActor::CActorParam_UnkVirtualFunc6(int val) {
    this->cf::CActorParam::CActorParam_UnkVirtualFunc6(val);
    u32* idPtr = reinterpret_cast<u32*>(
        reinterpret_cast<cf::CObjectState*>(
            reinterpret_cast<cf::CActorState*>(this)->unk4)
            ->getStateData());
    u32 id = *idPtr;
    if (testActorStatus(this, (int*)&id, 0x1c) != 0) {
        CfObjectMove_setAnimModeArgs((u8*)this + 16028, 0x2f, 1, -1, 1);  // +0x3E9C: CfObjectMove subobject
    }
}

void CfObjectActor::CActorParam_setScale(float v) {
    *(float*)((u8*)this + 0x15e8) = v;
    static_cast<cf::CfObject*>((cf::CfObjectMove*)((u8*)this + 0x3e9c))->CfObject_setObjScale(v);
}

float CfObjectActor::CActorParam_getScale() {
    return static_cast<cf::CfObject*>((cf::CfObjectMove*)((u8*)this + 0x3e9c))->CfObject_getObjScale();
}

float CfObjectActor::CfObject_getMoveHeadAngle() {
    return mField4C;
}

float CfObjectActor::CfObjectActor_getAdjustedFacing() {
    // Base height at 0x3EE8; if the move target (0x3F60) is set, add the
    // signed short value the CActParamData helper returns (s16 -> float via
    // MWCC's 2^52 double-trick, which emits the retail fsubs sequence).
    u8* p = static_cast<u8*>(mTargetC4);
    if (p != 0) {
        float base = mField4C;
        return base + (float)(s16)getParamSelS16(p + 16, 0);
    }
    return mField4C;
}

float CfObjectActor::CfObjectActor_readActionScale() {
    // Word at absolute offset 0x3E74 (CAIAction trailer area) is used as a
    // pointer to an object holding a float at +0x7C.
    cf::CfFloat7C* p = reinterpret_cast<cf::CfFloat7C*>(reinterpret_cast<cf::CfActorField3E74*>(this)->field_0x3E74);
    if (p == 0) return lbl_eu_8066776C;
    return p->field_0x7C;
}

int CfObjectActor::CfObjectActor_sharesMoveFlags(cf::CfObjectActor* arg) {
    if (arg == 0) return 0;
    if (arg == this) return 0;
    u32 selfFlags = unk64;
    if ((selfFlags & 2) != 0 && (arg->unk64 & 2) != 0) return 1;
    if ((selfFlags & 4) != 0 && (arg->unk64 & 4) != 0) return 1;
    return 0;
}

void* CfObjectActor::CfObject_pushRefreshValue(float value) {
    // Forward to the CfObjectMove base, then re-dispatch on the actor behind
    // the action-source handle when its +0x64 flags select it.
    CfObjectMove::CfObject_pushRefreshValue(value);
    u32 p = mActionSrcHandle;
    if (p != 0) {
        cf::CfObjectMove* src = static_cast<cf::CfObjectMove*>(findObjectById(p));
        // Merged condition: both false paths share one zeroing block. The
        // true arm is the null-guarded static_cast downcast (retail re-checks
        // src before the guarded subi 16028; CfObjectMove sits at +0x3E9C).
        // The re-dispatch is VIRTUAL through slot +0x5C4 (retail bcctrl with
        // f1 carried through); a direct call lets MWCC auto-inline and
        // duplicate the body.
        cf::CfObjectActor* actor;
        if (src != 0 &&
            ((src->unk64 & 2) != 0 ||
             (src->unk64 & 4) != 0)) {
            actor = static_cast<cf::CfObjectActor*>(src);
        } else {
            actor = 0;
        }
        if (actor != 0) {
            actor->CfObject_pushRefreshValue(value);
        }
    }
    return this;
}

void CfObjectActor::CfObjectMove_setTargetC4Arg(u32 id) {
    if (mTargetC4 != 0) {
        static_cast<CfObjectActorInner*>(mTargetC4)->mArgStorage = id;
    }
    mActionSrcId = id;
}

} // namespace cf

// .data table (retail 0xC) as a typed pointer table. Reloc sites read as
// zero in the .o (relocs skipped when bytes match), so it stays MATCH.
// ptmf constant {this-delta 0, non-virtual -1, CVision engage handler}.
// name-guess: sole member is openVisionPairEngage; mirrors CVision.cpp's
// named visionCallback* triplets.
extern "C" void openVisionPairEngage(void* self);
__declspec(section ".data") __attribute__((used))
const u32 sVisionPairEngagePtmf[3] = { 0, 0xFFFFFFFFu, (u32)openVisionPairEngage };

// Remaining shared .sdata2 constants, as typed consts in retail order.
__declspec(section ".sdata2") __attribute__((used))
const float lbl_eu_8066773C = 0.5f;
__declspec(section ".sdata2") __attribute__((used))
const float lbl_eu_80667764 = 0.01f;
__declspec(section ".sdata2") __attribute__((used))
extern const float lbl_eu_8066776C = -1.0f;
