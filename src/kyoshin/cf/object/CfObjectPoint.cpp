#include "monolib/scn/CScnTimeApi.hpp"
#include "monolib/math/CVec3.hpp"
#include "kyoshin/cf/object/CfObjectPoint.hpp"
#include "kyoshin/code_801862C0.hpp"
#include "kyoshin/plugin/ocBdat.hpp"

#include <types.h>
#include <decomp.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)



namespace cf {

class CfObjectPoint; // forward declaration (field type below)

// ---------------------------------------------------------------------------
// Real class tree, recovered from the retail vtable lbl_eu_8052A3B0
// (code_800C17DC.s: RTTI, 0, then one word per slot through +0x174).
// CfObjectPoint inherits CfObject; the words that differ from the CfObject
// table (lbl_eu_805294E0) are Point's overrides:
//
//   CObjectParam +0x48 loadPointData (was UnkVirtualFunc4)
//   CfObject +0x58 validatePointState, +0x5C resetPointFlags,
//            +0x60 updatePointState, +0x68 releasePointLink,
//            +0x6C notifyChildUpdate, +0x70 setChildPoint,
//            +0x9C func_80047814, +0xA0 setPointPosition,
//            +0x158 setPointEnabled, +0x160 isCollEnabled
//   (base words at +0x58/+0x60/+0x6C/+0x70/+0x158/+0x160 are pure/0;
//   +0x68 holds the base CfObject_UnkVirtualFunc6 impl;
//   +0xA0 holds the base CfObject_UnkVirtualFunc20 impl)
//
// The base views below spell Point's names at the overridden slots (same
// scheme as CfObjectColl.hpp) so every override is a true same-name C++
// override and call sites read this->method(...) on the owning type.
// All classes are novtable: no compiler __vt__ is emitted (the retail
// tables live in code_800C17DC.s); the dtor stores the retail labels by
// hand. Base dtor is inline-empty so no base-dtor call is synthesised.
// ---------------------------------------------------------------------------

struct CfObjectSub54 {
    u8 field_0x0[0xC];
};

class __declspec(novtable) CObjectState {
public:
    virtual void setStateBitMask(u32 bits);  //0x8
    virtual int checkStateFlags(int mask);  //0xC
    virtual void setStateBitFlag(u32 mask);  //0x10
    virtual void clearStateWord4();  //0x14 (was UnkVirtualFunc4; clears unk4)
    virtual void applyStateFlags(int arg);  //0x18
    virtual void clearStateWord8();  //0x1C (was UnkVirtualFunc6; clears unk8)
    virtual void clearStateFlags8(int arg);  //0x20
    virtual int checkStateFlags8(int arg);  //0x24
    virtual int CObjectState_checkStateFlagsC();  //0x28 (was UnkVirtualFunc9)
    // Retail arity note: the base implementation (ocUnit) takes
    // (mask, flag), but every retail caller in the Point family sets only
    // r4 (the child/parent pointer) and leaves r5 stale, discarding the
    // result. The single-pointer spelling matches that dispatch exactly.
    virtual int setStateBitMask0(CfObjectPoint* parent); //0x2C
    virtual void* getStateData(); //0x30
    virtual void* setStateBitMask2(); //0x34
    virtual void setStateBitMask3(); //0x38

    //0x0: vtable
    u32 unk4;          // 0x04
    u32 unk8;          // 0x08
    u32 unkC;          // 0x0C
};

class __declspec(novtable) CObjectParam : public CObjectState {
public:
    // Slot 0x3C: stores the object name (strlen to +0x30, copy to +0x10).
    virtual void setObjectName(const char* name); //0x3C
    virtual void* CObjectParam_getParamPtr(); //0x40 (was UnkVirtualFunc2)
    virtual int CObjectParam_hasObjectName(); //0x44 (was UnkVirtualFunc3)
    virtual void loadPointData(); //0x48 (overridden by CfObjectPoint)
    virtual BOOL CObjectParam_getSelfObjectId(); //0x4C
    virtual void CObjectParam_signalActionEnd(int flag); //0x50

    //0x0: vtable
    //0x0-10: CObjectState
    u8* mPtr10;            // 0x10-0x13 (pointer stored at offset 0x10)
    u8 _pad14[0x28 - 0x14]; // 0x14-0x27
};

class __declspec(novtable) CfObject : public CObjectParam {
public:
    // 0x54 is the destructor slot in every retail table (Point's word holds
    // __dt__Q22cf13CfObjectPointFv, CfObject's holds __dt__Q22cf8CfObjectFv).
    // It is spelled as a plain virtual, not a C++ dtor: even an inline-empty
    // base dtor makes MWCC emit a 0x40 weak D1 orphan that breaks the unit
    // split (the retail linker GC'd it). The implicit trivial dtor keeps
    // ~CfObjectPoint free of any base-dtor call; no compiler __vt__ is
    // emitted (novtable), so this name never leaves the TU.
    virtual void destroyObject(); //0x54 (dtor slot)

    virtual void validatePointState(); //0x58 (overridden by CfObjectPoint)
    virtual void resetPointFlags(); //0x5C (overridden by CfObjectPoint)
    virtual void updatePointState(); //0x60 (overridden by CfObjectPoint)
    virtual void CfObject_updateMoveRate();      //0x64 (was UnkVirtualFunc5)
    virtual void releasePointLink(); //0x68 (overridden by CfObjectPoint)
    // True body (defined below with a forced Fv retail name) forwards the
    // incoming r4 vector to the child; the view stays arg-free.
    virtual void notifyChildUpdate(); //0x6C (overridden by CfObjectPoint)
    // True body takes the hidden child pointer in r4 (see setChildPoint
    // below); the view stays arg-free.
    virtual void setChildPoint(); //0x70 (overridden by CfObjectPoint)
    virtual bool CfObject_isMoveActiveNow();      //0x74
    virtual void CfObject_forwardSubObject();     //0x78
    virtual void CfObject_notifyMemberRemoved();     //0x7C (was UnkVirtualFunc11)
    virtual void CfObject_refreshSubB0();     //0x80 (was UnkVirtualFunc12)
    virtual int CfObject_queryTargetState();     //0x84
    virtual void* CfObject_pushRefreshValue(float value);     //0x88
    virtual float CfObject_getMoveSpeedRate();    //0x8C
    virtual void* CfObject_pushRefreshExtra(float value);     //0x90
    virtual void CfObject_readRefreshValue();     //0x94
    virtual int CfObject_checkTargetState();     //0x98
    // Copies the pointed-to position block to this+0x3C (impl in pluginCfs).
    virtual void func_80047814(const ml::CVec3* pos); //0x9C (overridden by CfObjectPoint)
    // True body takes hidden (x, z) floats (see setPointPosition below).
    virtual void setPointPosition(); //0xA0 (overridden by CfObjectPoint)
    // True body stores (x, y, z) floats to +0x3C/0x40/0x44; the Point
    // update path calls it with ambient registers (no setup, bctrl tail).
    // Mirrors CfObject_setPosXZ at +0xA0; Coll body is UnkVirtualFunc21.
    virtual void CfObject_setPosXYZ();     //0xA4 (was UnkVirtualFunc21)
    virtual void syncMoveTarget(const ml::CVec3* vec); //0xA8
    virtual ml::CVec3* CfObject_getPosVector();      //0xAC
    virtual ml::CVec3* CfObject_getPosTriple(); //0xB0 (was UnkVirtualFunc24)
    virtual void CfObject_snapMoveTarget(ml::CVec3* pos, float scale); //0xB4
    virtual void CfObject_applyMoveOffset(const ml::CVec3* vec, float amount); //0xB8
    virtual void CfObject_setRotVec(const ml::CVec3* src); //0xBC (was UnkVirtualFunc27)
    // Base returns self+0x48; Model returns xform block / sub+0xC4.
    virtual u8* CfObject_getField48();       //0xC0 (was UnkVirtualFunc28)
    virtual void CfObject_setMoveHeadAngle(float value);     //0xC4
    virtual void CfObject_setMoveYaw(float value);     //0xC8 (was UnkVirtualFunc30)
    virtual float CfObject_getMoveHeadAngle();     //0xCC
    virtual void CfObject_syncMoveHead();     //0xD0 (was UnkVirtualFunc32)
    virtual void CfObject_applyMoveYaw(float amount); //0xD4 (was UnkVirtualFunc33)
    virtual float CfObject_getScaledAngle();     //0xD8 (was UnkVirtualFunc34)
    virtual void CfObject_setObjScale(float value); //0xDC (was UnkVirtualFunc35)
    virtual float CfObject_getObjScale();     //0xE0 (was UnkVirtualFunc36)
    // Wave-62: Move-family anim-slot managers spelled via CfObject.hpp
    // aliases (Point has no call sites; novtable layout-only decls).
    // UVF41 left Unk: base stub returns 0, no Move override, no alias.
    virtual void CfObject_resetAnimSlots();     //0xE4 (was UnkVirtualFunc37)
    virtual void CfObject_clearAnimEntry();     //0xE8 (was UnkVirtualFunc38)
    virtual void CfObject_armAnimSlot0();     //0xEC (was UnkVirtualFunc39)
    virtual float CfObject_getActorScale();     //0xF0 (was UnkVirtualFunc40)
    virtual void CfObject_UnkVirtualFunc41();     //0xF4 (base stub; no alias)
    virtual void CfObject_armAnimSlot1();     //0xF8 (was UnkVirtualFunc42)
    virtual void CfObject_armAnimSlot1N();     //0xFC (was UnkVirtualFunc43)
    virtual void CfObject_notifyActiveMember();     //0x100 (was UnkVirtualFunc44)
    virtual void CfObject_rearmAnimSlot();     //0x104 (was UnkVirtualFunc45)
    virtual void CfObject_setMoveTargetPtr(void* arg);     //0x108
    virtual void CfObject_createMoveTarget();     //0x10C
    virtual void* CfObject_getCurrentTarget();     //0x110
    virtual void CfObject_forwardNpcAction(u32 value);     //0x114
    virtual int CfObject_getNpcActionState();     //0x118 (was UnkVirtualFunc50)
    virtual int CfObject_getNpcTargetAction();     //0x11C (was UnkVirtualFunc51)
    virtual void* CfObject_findNodeMatrix(const char* name);     //0x120 (was UnkVirtualFunc52)
    virtual CfObject* CfObject_getIndexedNodeMatrix(); //0x124 (was UnkVirtualFunc53)
    virtual u32 CfObject_getSubPosWord();     //0x128 (was UnkVirtualFunc54)
    virtual void* CfObject_getPosSample(int arg);     //0x12C (was UnkVirtualFunc55)
    virtual float CfObject_getHeightDiff();     //0x130 (was UnkVirtualFunc56)
    virtual void CfObject_setMoveValue(float value);     //0x134 (was UnkVirtualFunc57)
    virtual u32* CfObject_getMoveRateScale();     //0x138
    virtual void CfObject_setMoveScale(float value); //0x13C (was UnkVirtualFunc59)
    virtual float CfObject_getMoveFactor();     //0x140 (was UnkVirtualFunc60)
    virtual void CfObject_setAnimSlotEntry(u32 a, u32 b);     //0x144
    virtual u32 CfObject_getSlotBits(u32 arg);     //0x148 (was UnkVirtualFunc62)
    virtual u32 CfObject_readKindFlagWord();     //0x14C
    virtual void CfObject_setMoveBusyState(int flag);     //0x150
    virtual void CfObject_setMoveReadyFlag(int flag);     //0x154
    virtual void setPointEnabled(int flag); //0x158 (overridden by CfObjectPoint)
    // Sets/clears mFlags68 0x00400000 (Model also drives visibility); retail Fv.
    virtual void CfObject_setVisibleFlag(int flag); //0x15C (was UnkVirtualFunc67)
    virtual int isCollEnabled(); //0x160 (defined by CfObjectPoint)
    // Model: bit 1 of sub-object +0x7A4; base returns 0.
    virtual int CfObject_isVisibleFlag(); //0x164 (was UnkVirtualFunc69)
    virtual void CfObject_syncModelRate(float value); //0x168 (was UnkVirtualFunc70)
    virtual float CfObject_getProgressLow();     //0x16C
    // Model stores f1 to +0xA4; pairs with getProgressHigh at +0x174.
    virtual void CfObject_setProgressHigh(float value); //0x170 (was UnkVirtualFunc72)
    virtual float CfObject_getProgressHigh();     //0x174

    //0x0: vtable
    // CObjectParam currently ends at 0x28.
    u8 field_0x28[0x10];
    CfObjectPoint* mSubObj38; // 0x38-0x3B
    float mPos3C;           // 0x3C-0x3F
    float mPos40;           // 0x40-0x43
    float mPos44;           // 0x44-0x47
    u8 _pad48[0x4C - 0x48];   // 0x48-0x4B
    float mField4C;           // 0x4C-0x4F
    u8 _pad50[0x54 - 0x50];   // 0x50-0x53
    CfObjectSub54 mSubObj54;     // 0x54-0x5F
    float mFloat60;            // 0x60-0x63
    u32 unk64;                  // 0x64-0x67
    u32 mFlags68;               // 0x68-0x6B
    u8 _pad6C[0x70 - 0x6C];    // 0x6C-0x6F
};

class __declspec(novtable) CfObjectPoint : public CfObject {
public:
    virtual ~CfObjectPoint();
    virtual void loadPointData(); //0x48
    virtual void validatePointState(); //0x58
    virtual void resetPointFlags(); //0x5C
    virtual void updatePointState(); //0x60
    virtual void releasePointLink(); //0x68
    virtual void notifyChildUpdate(); //0x6C
    virtual void setChildPoint(); //0x70
    virtual void func_80047814(const ml::CVec3* pos); //0x9C
    virtual void setPointPosition(); //0xA0
    virtual void setPointEnabled(int flag); //0x158
    virtual int isCollEnabled(); //0x160

    u8* mPtr70;       // 0x70-0x73
    u8 _pad74[4];     // 0x74-0x77
    char mName[0x14]; // 0x78-0x8B
    u16 mIndex8C;     // 0x8C-0x8D
    u8 _pad8E[2];     // 0x8E-0x8F
    u8 mFlag90;       // 0x90
    u8 mFlag91;       // 0x91
};

} // namespace cf

// Genuine C-linkage function: the retail symbol is the bare unmangled name
// (no __Q2 class qualifier), so it must be defined with C linkage. Other TUs
// (CfGimmick, CfMapMineManager) call it through their own C-linkage
// declarations; the first parameter is the object pointer.
extern "C" void func_800C13FC(cf::CfObjectPoint* ths, u32 a, u8 val) {
    ths->setObjectName(reinterpret_cast<const char*>(a));
    ths->mFlag91 = val;
}

namespace cf {

CfObjectPoint::~CfObjectPoint() {
    // D1 vtable store is explicit (novtable class suppresses MWCC's implicit
    // store); the null guard and delete-flag guard are auto-generated. The
    // flags word is read before the vtable store, matching retail's schedule.
    u32 flags = mFlags68;
    *reinterpret_cast<u32**>(this) = lbl_eu_8052A3B0;
    if (flags & 0x40000000) {
        ((void* (*)(void*, void*))func_80186474)(func_801862C0(), this);
    }
    releasePointLink();
    // D2 phase (base class): the base dtor is inline-empty so MWCC emits no
    // standalone base-dtor body and no base call. The null guard + CfObject
    // vtable store + slot-0x68 destroy are written out, reproducing retail's
    // inlined D2 block exactly.
    if (this != 0) {
        *reinterpret_cast<u32**>(this) = lbl_eu_805294E0;
        releasePointLink();
    }
}

void CfObjectPoint::resetPointFlags() {
    setPointEnabled(1);
    mFlag91 = 0;
    mFlag90 = 0;
}

void CfObjectPoint::loadPointData() {
    func_8003AA34();
    u8* fp = (u8*)getFP(mName);
    u32 name = getBdatStringColumnValue(fp, lbl_eu_804FC648, mIndex8C);
    this->setObjectName(reinterpret_cast<const char*>(name));
    u32 val = getBdatStringColumnValue(fp, lbl_eu_804FC648 + 5, mIndex8C);
    mFlag91 = *(u8*)&val;
}

void CfObjectPoint::releasePointLink() {
    if (mSubObj38 != nullptr) {
        CfObjectPoint* child = mSubObj38;
        child->CfObject_getPosVector();
        if (mSubObj38 != nullptr) {
            // Redundant nested check on the reloaded value mirrors retail's
            // two beq targets (MWCC keeps both branches).
            if (mSubObj38 != nullptr) {
                mSubObj38->setStateBitMask(1);
            }
            mSubObj38 = nullptr;
        }
    }
    mPtr70 = nullptr;
    mFlags68 &= 0x40000000;
}

void CfObjectPoint::updatePointState() {
    setStateBitMask3();

    if ((lbl_eu_80663E24 & 0x01000000) != 0) {
        if (checkStateFlags8(1) != 0) {
            goto main_body;
        }
        if (checkStateFlags(1) == 0) {
            goto done;
        }
    }

main_body:
    {
        float dist = func_80496288(lbl_eu_80663E14);
        if (dist > lbl_eu_80666B48) {
            if (checkStateFlags8(1) != 0) {
                if (checkStateFlags(1) == 0) {
                    if (func_8013EB90(1) == 0) {
                        clearStateFlags8(1);
                    }
                }
            }
        }
    }

    if (checkStateFlags(1) != 0) {
        applyStateFlags(1);
        setStateBitFlag(1);
    }

    if (mSubObj38 != nullptr) {
        mSubObj38->CfObject_setPosXYZ();
    }

done:
    return;
}

} // namespace cf

// Retail Fv: callers leave r4 = vec for syncMoveTarget on mSubObj38 (bctr thunk).
extern "C" void notifyChildUpdate__Q22cf13CfObjectPointFv(
    cf::CfObjectPoint* ths, const ml::CVec3* vec) {
    if (ths->mSubObj38 == nullptr)
        return;
    ths->mSubObj38->syncMoveTarget(vec);
}

// Forced-name form: the retail symbols setChildPoint/16F4/171C end in a
// decompiler-guessed "Fv" (the bodies consume r4/f1/f2 arguments), and the
// symbols.txt entries cannot be corrected to the true signatures (FPv/Fi/Fff)
// from this TU's writable scope. A real member would mangle to the corrected
// name, so these stay C-linkage functions with the mangled retail symbol as
// the identifier to emit the exact symbol.
extern "C" void setChildPoint__Q22cf13CfObjectPointFv(
    cf::CfObjectPoint* ths, cf::CfObjectPoint* child) {
    cf::CfObjectPoint* old = ths->mSubObj38;
    if (old != nullptr) {
        if (old != nullptr) {
            old->setStateBitMask(1);
        }
        ths->mSubObj38 = nullptr;
    }

    ths->mSubObj38 = child;
    if (child != nullptr) {
        child->setStateBitMask0(ths);
        ths->mSubObj38->setPointPosition();
    }
}

extern "C" void setPointEnabled__Q22cf13CfObjectPointFv(
    cf::CfObjectPoint* ths, int enable) {
    if (enable != 0) {
        ths->mFlags68 |= 0x00100000;
    } else {
        ths->mFlags68 = DECOMP_PPC_RLWINM(ths->mFlags68, 0, 12, 10);
    }
}

extern "C" void setPointPosition__Q22cf13CfObjectPointFv(
    cf::CfObjectPoint* ths, float x, float z) {
    // The const-qualified SDA float (see CfObjectPoint.hpp) lets MWCC hoist
    // the lfs above the prologue LR store (retail schedule); a non-const
    // declaration pins it after the global-pointer load.
    float y = lbl_eu_80666B4C;
    float pos[3];
    float first[3];
    float second[3];
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;

    if (lbl_eu_80663E14 == nullptr) {
        u8* source = *(u8**)((u8*)lbl_eu_80663E14 + 0x74);
        if (func_8049E51C(source) != 0) {
            if (func_804BE398(pos, 0, 0, 0,
                              lbl_eu_80666B50, lbl_eu_80666B48) != nullptr) {
                func_804BE4B4(first, 0);
                func_804BE4E0(second, 0);
                ((u32*)pos)[0] = ((u32*)first)[0];
                ((u32*)pos)[1] = ((u32*)first)[1];
                ((u32*)pos)[2] = ((u32*)first)[2];
            }
        }
    }

    // Word-copy pos into the object's position fields; retail stores 0x40
    // before 0x3c (assignment order), holding pos[0] in a register across.
    u32 p0 = *(u32*)&pos[0];
    *(u32*)&ths->mPos40 = *(u32*)&pos[1];
    *(u32*)&ths->mPos3C = p0;
    *(u32*)&ths->mPos44 = *(u32*)&pos[2];
}
