#pragma once

#include <types.h>
#include "kyoshin/cf/object/CObjectParam.hpp"
#include "monolib/math/CVec3.hpp"

namespace cf {
    struct CfObjectSub54 {
        ml::CVec3 mPos;              // 0x00 position delta triplet
    };

    class CfObjectModel;

    class CfObjectSub38 {
    public:
        void notifySubEffect();
        void updateSubEffect();
        void m08(int);
        void m2C(CfObjectModel*);
        void mA0();
        void mAC();
        void forwardActorEventEnd(void*);
        void forwardActorEvent(void*);
    };

    //min size: 0x70
    class CfObject : public CObjectParam {
    public:
        //vtable 1 (CfObject)
        virtual ~CfObject();                      //0x54
        virtual int CfObject_UnkVirtualFunc2() = 0;  //0x58 (initialise; every retail body returns 1)
        virtual void CfObject_UnkVirtualFunc3(); //0x5C
        virtual void CfObject_UnkVirtualFunc4() = 0;  //0x60
        // Wave-62: Unk primary flipped. Fv bodies in CfObjectMove keep Unk
        // linker names (hand-built tables / CREvtModel); Unk aliases below.
        virtual void CfObject_updateMoveRate();      //0x64 (was UnkVirtualFunc5)
        virtual void CfObject_releaseMoveTargets();  //0x68 (was UnkVirtualFunc6)
        virtual void CfObject_UnkVirtualFunc7() = 0;  //0x6C
        virtual void CfObject_notifyEventDone() = 0;  //0x70
        virtual bool CfObject_isMoveActiveNow();      //0x74
        virtual void CfObject_forwardSubObject();     //0x78
        virtual int CfObject_notifyMemberRemoved();      //0x7C
        virtual void CfObject_refreshSubB0();         //0x80 (was UnkVirtualFunc12)
        virtual int CfObject_queryTargetState();     //0x84
        virtual void* CfObject_pushRefreshValue(float value);     //0x88
        virtual float CfObject_getMoveSpeedRate();    //0x8C (Move/Coll return float; retail Fv)
        virtual void* CfObject_pushRefreshExtra(float value);     //0x90
        virtual float CfObject_readRefreshValue();     //0x94
        virtual int CfObject_checkTargetState();     //0x98
        // Retail bodies read r4 as a position vector (setEffPosVec_ /
        // Model/Move/ocUnit UVF19). Fv linker names are uneducated.
        virtual void CfObject_setMoveTargetVec(const ml::CVec3* vec); //0x9C
        virtual void CfObject_setPosXZ(float a, float b);     //0xA0
        virtual void CfObject_setPosXYZ();     //0xA4
        // Same ABI as UVF19 (copy vec into +0x3C). Eff's override is a pure
        // forward to UVF19 (callVirt19); Model/Move override with extra work.
        virtual void CfObject_syncMoveTarget(const ml::CVec3* vec); //0xA8
        virtual ml::CVec3* CfObject_getPosVector();      //0xAC
        // Retail returns the position-triplet pointer (+0x3C; Model override
        // returns +0x98+0x310 when the sub-object is present). void* matches
        // the retail bodies and the Coll TU view; Fv mangling unchanged.
        virtual void* CfObject_getPosTriple();     //0xB0
        // Retail ground-snap / moveEffOfs: (pos, scale) in r4/f1.
        virtual void CfObject_snapMoveTarget(ml::CVec3* pos, float scale); //0xB4
        // Forwards to UVF19 (base) or UVF25 (Eff callVirt25). First arg is a
        // position vector (u32 casts at call sites are pointers).
        virtual void CfObject_applyMoveOffset(const ml::CVec3* vec, float amount); //0xB8
        virtual void CfObject_setRotVec(void* src);     //0xBC (was UnkVirtualFunc27)
        virtual void* CfObject_getField48();     //0xC0
        virtual void CfObject_setMoveHeadAngle(float value);     //0xC4
        virtual void CfObject_setMoveYaw(float value);     //0xC8 (was UnkVirtualFunc30)
        virtual float CfObject_getMoveHeadAngle();     //0xCC
        // Wave-64: Unk primary flipped. Fv bodies in CfObjectMove keep Unk
        // linker names (hand-built tables / CREvtModel); Unk aliases below.
        virtual void CfObject_syncMoveHead(float deg);     //0xD0 (degrees -> setMoveHeadAngle)
        virtual void CfObject_applyMoveYaw(float amount); //0xD4 (was UnkVirtualFunc33)
        virtual float CfObject_getScaledAngle();     //0xD8
        // Retail setEffScale_ / Coll UVF35: float in f1 -> +0x60.
        virtual void CfObject_setObjScale(float value); //0xDC
        // Retail: lfs f1,0x60(r3); blr - scale getter used to feed UVF35.
        virtual float CfObject_getObjScale();     //0xE0
        // One word in, one word out: the base bodies return 0; CfObjectMove's
        // overrides forward the argument to CfObjectModel_checkTargetNode
        // (0xE4) / the +0xB0 sub-object's slot 0x4C (0xE8) and return that
        // call's result.
        virtual int CfObject_resetAnimSlots(u32 arg);     //0xE4
        virtual int CfObject_clearAnimEntry(u32 arg);     //0xE8
        // Wave-64: Unk primary flipped. Fv bodies in CfObjectMove keep Unk
        // linker names (hand-built tables / CREvtModel); Unk aliases below.
        virtual int CfObject_armAnimSlot0(u32 id, u32 tag);     //0xEC (bdat arm, slot 0)
        // Wave-64: Unk primary flipped. Fv bodies in CfObjectMove keep Unk
        // linker names (hand-built tables / CREvtModel). Move's Fv override
        // is the named bdat arm for slot 0 (armAnimSlot0N alias); float
        // callers use getActorScale (CfResReloadImpl). Same Fv arity.
        virtual int CfObject_armAnimSlot0N(u32 id, u32 nameIdx);     //0xF0 (named bdat arm, slot 0)
        virtual int CfObject_UnkVirtualFunc41();      //0xF4
        virtual int CfObject_armAnimSlot1(u32 id, u32 tag);     //0xF8 (bdat arm, slot 1)
        virtual int CfObject_armAnimSlot1N(u32 id, u32 nameIdx);     //0xFC (named bdat arm, slot 1)
        virtual int CfObject_UnkVirtualFunc44();      //0x100
        virtual void CfObject_rearmAnimSlot(u32 value, u32 index);     //0x104
        virtual void CfObject_setMoveTargetPtr(void* arg);     //0x108
        virtual void CfObject_createMoveTarget(int kind);     //0x10C
        // Move override returns target object pointer (pluginCfs: +0xC4/+0x6C0).
        virtual void* CfObject_getCurrentTarget();     //0x110
        virtual void CfObject_forwardNpcAction(int action);     //0x114
        virtual int CfObject_getNpcActionState();     //0x118 (was UnkVirtualFunc50)
        virtual int CfObject_getNpcTargetAction();     //0x11C (was UnkVirtualFunc51)
        virtual void* CfObject_findNodeMatrix(const char* name) const;     //0x120
        virtual void* CfObject_getIndexedNodeMatrix(u32 idx) const; //0x124
        virtual u32 CfObject_getSubPosWord();     //0x128
        virtual void* CfObject_getPosSample(int arg) const;     //0x12C
        virtual float CfObject_getHeightDiff() const;     //0x130 (was UnkVirtualFunc56)
        virtual void CfObject_setMoveValue(float value);     //0x134
        virtual u32* CfObject_getMoveRateScale();     //0x138
        virtual void CfObject_setMoveScale(float value); //0x13C (Move stores f1; retail Fv)
        virtual float CfObject_getMoveFactor();     //0x140
        virtual void CfObject_setAnimSlotEntry(u32 a, u32 b);     //0x144
        virtual u32 CfObject_getSlotBits(u32 arg);     //0x148 (retail takes an index, returns bits)
        virtual u32 CfObject_readKindFlagWord();     //0x14C
        virtual void CfObject_setMoveBusyState(int flag);     //0x150
        virtual void CfObject_setMoveReadyFlag(int flag);     //0x154

        // Slot 0x158: display/point enable flag (Model toggles mFlags68
        // 0x100000 and forwards to the renderer sub-object; Point does the
        // identical flag op). Prior recovery named it on the Point family
        // (CfObjectColl.hpp CfObjectPoint::setPointEnabled, same slot);
        // adopted here as the shared base name.
        virtual void setPointEnabled(int flag) = 0; //0x158
        // Legacy Unk spelling (non-virtual inline): same-arity alias so
        // other TUs keep compiling unchanged (Wave-35 rule). MWCC inlines
        // it into the identical virtual dispatch.
        void CfObject_UnkVirtualFunc66(int flag) { setPointEnabled(flag); } //0x158
        void update();
        virtual void CfObject_setVisibleFlag(int flag);     //0x15C
        virtual int CfObject_UnkVirtualFunc68(const ml::CVec3* vec = nullptr); //0x160
        virtual u32 CfObject_isVisibleFlag();     //0x164
        // Wave-64: Unk primary flipped. Fv bodies in CfObjectMove keep Unk
        // linker names (hand-built tables / CREvtModel); Unk aliases below.
        virtual void CfObject_syncModelRate(float value); //0x168 (was UnkVirtualFunc70)
        // Retail: return float in f1 (Model reads +0xA0; Coll returns const).
        // CfResPcImpl forwards that into CScnItemModel::setModelFloatRate.
        virtual float CfObject_getProgressLow();     //0x16C
        virtual void CfObject_setProgressHigh(float value);     //0x170 (Model stores +0xA4; read back by getProgressHigh)
        virtual float CfObject_getProgressHigh();     //0x174

        // Wave-37 caller aliases (non-virtual, inline): legacy UnkVirtual
        // spellings for the 0x16C/0x174 float queries. Same arity; forward
        // to the renamed virtuals so other TUs keep compiling unchanged.
        void CfObject_UnkVirtualFunc26(const ml::CVec3* vec, float amount) { CfObject_applyMoveOffset(vec, amount); }
        void CfObject_UnkVirtualFunc22(const ml::CVec3* vec) { CfObject_syncMoveTarget(vec); }
        void CfObject_UnkVirtualFunc25(ml::CVec3* pos, float scale) { CfObject_snapMoveTarget(pos, scale); }

        // Wave-37 named forwarders (non-virtual, inline): real API spellings
        // for slots whose virtuals keep their placeholder names because C++
        // definitions live in other TUs (CtrlEnemy/ocUnit). Same arity;
        // MWCC inlines them into the identical virtual dispatch.
        int CfObject_checkSubReady(const ml::CVec3* vec = nullptr) { return CfObject_UnkVirtualFunc68(vec); }
        // Wave-62 Unk aliases (non-virtual, inline): legacy Unk spellings for
        // the flipped 0x64/0x68/0x80/0xBC/0xC8 slots. Same arity; MWCC inlines
        // each into the identical virtual dispatch. Hand-built tables keep
        // Unk Fv linker names (Move forced-name bodies).
        void CfObject_UnkVirtualFunc6() { CfObject_releaseMoveTargets(); } //0x68
        void CfObject_UnkVirtualFunc27(void* src) { CfObject_setRotVec(src); } //0xBC
        // Wave-39 UnkVirtual mop-up aliases (non-virtual, inline): same-slot
        // forwarders so call sites spell behavior, not slot placeholders.
        // The virtuals keep their Unk names (hand-built vtables in
        // CREvtModel.cpp spell the current mangled names); MWCC inlines each
        // alias into the identical virtual dispatch.
        // Wave-39 caller alias (non-virtual, inline): real API spelling
        // for slot 0x12C (indexed position sample; Model forwards the index
        // through the sub-object chain). Same arity; MWCC inlines it into
        // the identical virtual dispatch.
        // Wave-39 caller aliases (non-virtual, inline): behavior-derived
        // spellings for the holder-notification slots used by CBattleManager's
        // list-removal paths. Same arity; MWCC inlines them into the
        // identical virtual dispatch.
        void CfObject_notifyActiveMember() { CfObject_UnkVirtualFunc44(); } //0x100: 0x4-flagged holder notify (registerBattleActor / evalBattleTarget)
        // Wave-40 UnkVirtual mop-up aliases (non-virtual, inline): same-slot
        // forwarders for the ocUnit call sites; Unk Fv bodies stay for
        // hand-built vtables (CfObjectModel / CfCollSphereImpl / CREvtModel).
        // Wave-50 caller alias (non-virtual, inline): behavior-derived
        // spelling for the gated sub-object word (+0x128) consumed as a
        // fallback aim position by the follow camera (cfCam_pickFollowAim sel==0
        // path; Model impl: field_14AC when the 0x40000000 flag is set).
        // Same arity; MWCC inlines it into the identical virtual dispatch.
        // Convenience overload: typed vec call sites cast into the void* slot.
        void CfObject_setRotVec(const ml::CVec3* vec) { CfObject_setRotVec((void*)vec); } //0xBC
        // Wave-41 UnkVirtual mop-up alias (non-virtual, inline): behavior-derived
        // spelling for the indexed slot-bits query (+0x148) used by the
        // model-display family (CModelDisp / CModelDispEquip / MakeCrystal).
        // Same arity; MWCC inlines it into the identical virtual dispatch.
        // Wave-42 UnkVirtual mop-up aliases (non-virtual, inline):
        // behavior-derived spellings for the scale/head slots used by the
        // effect family (CfObjectEff createEffect_/callVirt32). Same arity;
        // MWCC inlines them into the identical virtual dispatch.
        // Wave-64 Unk aliases (non-virtual, inline): legacy Unk spellings for
        // the flipped 0xD0/0xD4/0x124/0x130/0x168 slots. Same arity; MWCC
        // inlines each into the identical virtual dispatch. Hand-built
        // tables keep Unk Fv linker names (Move / GameManager forced-name
        // bodies).
        float CfObject_UnkVirtualFunc56() { return CfObject_getHeightDiff(); } //0x130
        // Wave-44 UnkVirtual mop-up alias (non-virtual, inline): behavior-derived
        // spelling for the object-scale getter (+0xE0, reads +0x60) feeding
        // CfObject_setObjScale. Same arity; MWCC inlines it into the
        // identical virtual dispatch.
        // Wave-53 / Wave-64 Unk alias: legacy Unk spelling for +0xF0.
        // Virtual primary is getActorScale; armAnimSlot0N is the Move-side
        // named-bdat arm spelling of the same slot.
        // Wave-48 UnkVirtual mop-up aliases (non-virtual, inline): behavior-derived
        // spellings for the reset-hook slots used by the NPC char reset helper
        // (CtrlNpcCharInitState). Same arity; MWCC inlines them into the identical
        // virtual dispatch.
        void CfObject_initEventState() { CfObject_UnkVirtualFunc2(); } //0x58: event-state init step (Model impl: notifyEventDone + raise 0x400000; Obj/Ene/Pc call it from initialize())
        void CfObject_syncEnableState() { CfObject_UnkVirtualFunc3(); } //0x5C: enable-state propagation hook (base: no-op; Pc: enablePcFlag on the move sub-object)
        // Wave-61 caller alias: per-frame move update (+0x60). Virtual keeps
        // UnkVirtualFunc4 (Move/Obj/Npc Class_ direct bls; hand vtables).
        void CfObject_runMoveUpdate() { CfObject_UnkVirtualFunc4(); } //0x60: per-frame move refresh (Move impl)
        // Wave-64 UnkVirtual mop-up aliases (non-virtual, inline): legacy Unk
        // spellings for the Move-family anim-slot managers (+0xE4..+0x104).
        // armAnimSlot0 (+0xEC) is the flipped virtual primary; Unk alias
        // below. Other slots keep Unk virtuals (hand-built vtables). Base
        // stubs return 0; Move overrides do the real work. Same Fv arity;
        // MWCC inlines into the identical dispatch. UVF41 (+0xF4) stays Unk:
        // base stub only, no Move override, no named call-site alias yet.
        // Wave-64: named bdat arm for slot 0 (Move UVF40 Fv); same slot as
        // getActorScale (float path). Discard float on the arm spelling.
        // Wave-63: behavior spelling of +0xA4 (Point setPosXYZ; Coll UVF21).

        //not sure if belongs here?
        // Retail requestVoice__Q22cf8CfObjectFiUlff returns the voice
        // dispatch result (0 when no sub-object); callers ignore it.
        bool requestVoice(int, u32, float, float);


        //0x0: vtable
        // CObjectParam ends at 0x38 (field_30 at +0x30, unk34 at +0x34).
        void* mSubObj38;
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
        u32 field_0x6C;             // 0x6C-0x6F
    };

    class CfObjectModel;

    // mSubObj38 (+0x38) is a polymorphic sub-object.
    // (CfObjectModel UVF6 + notifyEventDone, both FULL):
    //   +0x08 = setStateBitMask(r4=1)
    //   +0x2C = CScnItemModel::attachModelParent(u32) (r4=parent, r5 residue)
    //   +0xA0 = CScnItemModel::notifyModelAttached() (no FP setup; CfObject UVF20 needs f1/f2)
    //   +0xAC = CfObject_getPosVector() (return discarded)
    // Call through those owning classes; the former +0x38 merged pad view
    // (spacer _fNN / mXX decls) is deleted. Move/Eff/Actor TUs must cast to
    // the real owner per slot (their Wave-63 primary files).

    // The same sub-object answers a char-effect query through its own vtable
    // slot 0xE4 (no arguments; returns the effect handle or 0). Its class is
    // not declared yet, so this layout-only interface pins that one slot.
    class CfSubObj38Slots {
    public:
        virtual void slot08();
        virtual void slot0C();
        virtual void slot10();
        virtual void slot14();
        virtual void slot18();
        virtual void slot1C();
        virtual void slot20();
        virtual void slot24();
        virtual void slot28();
        virtual void slot2C();
        virtual void slot30();
        virtual void slot34();
        virtual void slot38();
        virtual void slot3C();
        virtual void slot40();
        virtual void slot44();
        virtual void slot48();
        virtual void slot4C();
        virtual void slot50();
        virtual void slot54();
        virtual void slot58();
        virtual void slot5C();
        virtual void slot60();
        virtual void slot64();
        virtual void slot68();
        virtual void slot6C();
        virtual void slot70();
        virtual void slot74();
        virtual void slot78();
        virtual void slot7C();
        virtual void slot80();
        virtual void slot84();
        virtual void slot88();
        virtual void slot8C();
        virtual void slot90();
        virtual void slot94();
        virtual void slot98();
        virtual void slot9C();
        virtual void slotA0();
        virtual void slotA4();
        virtual void slotA8();
        virtual void slotAC();
        virtual void slotB0();
        virtual void slotB4();
        virtual void slotB8();
        virtual void slotBC();
        virtual void slotC0();
        virtual void slotC4();
        virtual void slotC8();
        virtual void slotCC();
        virtual void slotD0();
        virtual void slotD4();
        virtual void slotD8();
        virtual void slotDC();
        virtual void slotE0();
        virtual int getCharEffect();  //0xE4
    };
}
