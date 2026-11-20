#pragma once
#include "kyoshin/cf/CfCamSymbols.hpp"
#include <types.h>
#include "kyoshin/cf/CtrlActApi.hpp"
#include <nw4r/math/math_types.h>
#include "monolib/math/CVec3.hpp"
#include "monolib/math/CMat34.hpp"
#include "monolib/math/CMat33.hpp"
#include "monolib/math/Random.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CtrlPc.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"
#include "kyoshin/cf/object/CObjectParam.hpp"

namespace cf {
class CtrlAct;
// Forward declarations for the real slot signatures of the CfObject/CfObjectPc
// vtables below (the owning headers are not included here).
class CBattleState;
struct CBattleStateEntry;
struct CBattleStateSrcEntry;
class CActorParamStatusTable;
struct CActorParam_UnkStruct1;
struct CActorParam_UnkStruct3;
struct CActorParam_UnkStruct4;
struct CActorParam10Arg;
struct CActorParam11Arg;
struct CActorParam12Arg;
} // namespace cf
using cf::CBattleState;
using cf::CBattleStateEntry;
using cf::CBattleStateSrcEntry;
using cf::CActorParamStatusTable;
using cf::CActorParam_UnkStruct1;
using cf::CActorParam_UnkStruct3;
using cf::CActorParam_UnkStruct4;
using cf::CActorParam10Arg;
using cf::CActorParam11Arg;
using cf::CActorParam12Arg;

// u32 -> double conversion magic: the retail code builds the 0x43300000-
// prefixed bit pattern in memory and subtracts the shared sdata2 magic
// double (lbl_eu_80666D78) so the pool reloc matches the retail constant
// instead of an MWCC-synthesised @N entry (CfMapEffectManager.hpp convention).
union F64Conv {
    u32 w[2];
    double d;
};

// 4-byte flag word at cf::CtrlAct+0x58. MWCC lays bitfields out MSB-first:
// the first-declared field occupies the most significant bits, so
// mKind(28-31)/mPhase(26-27)/mAngleState(24-25)/mParam(16-23) map to the
// retail rlwimi masks (28,0-3), (26,4-5), (24,6-7), (16,8-15).
struct CtrlActFlags {
    u32 mKind : 4;        // 28-31 action kind (switch in ctrlActSetupActionKind)
    u32 mPhase : 2;       // 26-27 phase counter (incremented by progressPhase)
    u32 mAngleState : 2;  // 24-25 facing state (snapFacingAng)
    u32 mParam : 8;       // 16-23 action parameter
    u32 mBit15 : 1;       // 15
    u32 mBit14 : 1;       // 14 (0x4000, set by testFacingGap)
    u32 mBit13 : 1;       // 13 (0x2000, latched by testFacingGap)
    u32 : 13;             // 0-12
};

// self->mField28 view: vtable slot 0x138 returns a float vector (first
// float read as the height/timer value by progressPhase). Typed virtuals so
// retail's true C++ dispatch shape (lwzu/lwz/mtctr/bctrl) is reproduced.
class CtrlActViewSub28 {
public:
    virtual void setStateBitMask(u32 bits);  // +0x008 setStateBitMask__Q22cf12CObjectStateFUl
    virtual int checkStateFlags(int mask);  // +0x00C checkStateFlags__Q22cf12CObjectStateFi
    virtual void setStateBitFlag(u32 mask);  // +0x010 setStateBitFlag__Q22cf12CObjectStateFUl
    virtual void clearStateWord4();  // +0x014 clearStateWord4__Q22cf12CObjectStateFv
    virtual void applyStateFlags(int arg);  // +0x018 applyStateFlags__Q22cf12CObjectStateFi
    virtual void clearStateWord8();  // +0x01C clearStateWord8__Q22cf12CObjectStateFv
    virtual void clearStateFlags8(int arg);  // +0x020 clearStateFlags8__Q22cf12CObjectStateFi
    virtual int checkStateFlags8(int arg);  // +0x024 checkStateFlags8__Q22cf12CObjectStateFi
    virtual int checkStateFlagsC(int mask);  // +0x028 checkStateFlagsC__Q22cf12CObjectStateFi
    virtual int setStateBitMask0(int mask, int flag);  // +0x02C setStateBitMask0__Q22cf12CObjectStateFii
    virtual void* getStateData();  // +0x030 getStateData__Q22cf12CObjectStateFv
    virtual void* setStateBitMask2();  // +0x034 setStateBitMask2__Q22cf12CObjectStateFv
    virtual void setStateBitMask3();  // +0x038 setStateBitMask3__Q22cf12CObjectStateFv
    virtual void CObjectParam_setObjectName(const char* name);  // +0x03C CObjectParam_setObjectName__Q22cf12CObjectParamFPCc
    virtual void* CObjectParam_getParamPtr();  // +0x040 CObjectParam_getParamPtr__Q22cf12CfObjectMoveFv
    virtual int CObjectParam_hasObjectName();  // +0x044 CObjectParam_hasObjectName__Q22cf12CObjectParamFv
    virtual void CObjectParam_UnkVirtualFunc4();  // +0x048 CObjectParam_UnkVirtualFunc4__Q22cf12CObjectParamFv
    virtual BOOL CObjectParam_getSelfObjectId();  // +0x04C CObjectParam_getSelfObjectId__Q22cf12CObjectParamFv
    virtual void CObjectParam_signalActionEnd(int flag);  // +0x050 CObjectParam_signalActionEnd__Q22cf12CObjectParamFi
    virtual ~CtrlActViewSub28();  // +0x054 __dt__Q22cf12CfObjectMoveFv
    virtual int CfObject_UnkVirtualFunc2() = 0;  // +0x058 CfObject_UnkVirtualFunc2__Q22cf13CfObjectModelFv
    virtual void CfObject_UnkVirtualFunc3();  // +0x05C CfObject_UnkVirtualFunc3__Q22cf8CfObjectFv
    virtual void CfObject_UnkVirtualFunc4() = 0;  // +0x060 CfObject_UnkVirtualFunc4__Q22cf12CfObjectMoveFv
    virtual void CfObject_updateMoveRate();  // +0x064 CfObject_updateMoveRate__Q22cf12CfObjectMoveFv
    virtual void CfObject_releaseMoveTargets();  // +0x068 CfObject_releaseMoveTargets__Q22cf12CfObjectMoveFv
    virtual void CfObject_UnkVirtualFunc7() = 0;  // +0x06C CfObject_UnkVirtualFunc7__Q22cf12CfObjectMoveFv
    virtual void CfObject_notifyEventDone() = 0;  // +0x070 CfObject_notifyEventDone__Q22cf13CfObjectModelFv
    virtual bool CfObject_isMoveActiveNow();  // +0x074 CfObject_isMoveActiveNow__Q22cf12CfObjectMoveFv
    virtual void CfObject_forwardSubObject();  // +0x078 CfObject_forwardSubObject__Q22cf12CfObjectMoveFv
    virtual int CfObject_notifyMemberRemoved();  // +0x07C CfObject_notifyMemberRemoved__Q22cf8CfObjectFv
    virtual void CfObject_refreshSubB0();  // +0x080 CfObject_refreshSubB0__Q22cf12CfObjectMoveFv
    virtual int CfObject_queryTargetState();  // +0x084 CfObject_queryTargetState__Q22cf12CfObjectMoveFv
    virtual void* CfObject_pushRefreshValue(float value);  // +0x088 CfObject_pushRefreshValue__Q22cf12CfObjectMoveFf
    virtual float CfObject_getMoveSpeedRate();  // +0x08C CfObject_getMoveSpeedRate__Q22cf12CfObjectMoveFv
    virtual void* CfObject_pushRefreshExtra(float value);  // +0x090 CfObject_pushRefreshExtra__Q22cf12CfObjectMoveFf
    virtual float CfObject_readRefreshValue();  // +0x094 CfObject_readRefreshValue__Q22cf12CfObjectMoveFv
    virtual int CfObject_checkTargetState();  // +0x098 CfObject_checkTargetState__Q22cf12CfObjectMoveFv
    virtual void CfObject_setMoveTargetVec(const ml::CVec3* vec);  // +0x09C CfObject_setMoveTargetVec__Q22cf12CfObjectMoveFPCQ22ml5CVec3
    virtual void CfObject_setPosXZ(float a, float b);  // +0x0A0 CfObject_setPosXZ__Q22cf13CfObjectModelFff
    virtual void CfObject_setPosXYZ();  // +0x0A4 CfObject_setPosXYZ__Q22cf8CfObjectFv
    virtual void CfObject_syncMoveTarget(const ml::CVec3* vec);  // +0x0A8 CfObject_syncMoveTarget__Q22cf12CfObjectMoveFPCQ22ml5CVec3
    virtual ml::CVec3* CfObject_getPosVector();  // +0x0AC CfObject_getPosVector__Q22cf12CfObjectMoveFv
    virtual void* CfObject_getPosTriple();  // +0x0B0 CfObject_getPosTriple__Q22cf13CfObjectModelFv
    virtual void CfObject_snapMoveTarget(ml::CVec3* pos, float scale);  // +0x0B4 CfObject_snapMoveTarget__Q22cf12CfObjectMoveFPQ22ml5CVec3f
    virtual void CfObject_applyMoveOffset(const ml::CVec3* vec, float amount);  // +0x0B8 CfObject_applyMoveOffset__Q22cf12CfObjectMoveFPCQ22ml5CVec3f
    virtual void CfObject_setRotVec(void* src);  // +0x0BC CfObject_setRotVec__Q22cf12CfObjectMoveFPv
    virtual void* CfObject_getField48();  // +0x0C0 CfObject_getField48__Q22cf13CfObjectModelFv
    virtual void CfObject_setMoveHeadAngle(float value);  // +0x0C4 CfObject_setMoveHeadAngle__Q22cf12CfObjectMoveFf
    virtual void CfObject_setMoveYaw(float value);  // +0x0C8 CfObject_setMoveYaw__Q22cf12CfObjectMoveFf
    virtual float CfObject_getMoveHeadAngle();  // +0x0CC CfObject_getMoveHeadAngle__Q22cf13CfObjectModelFv
    virtual void CfObject_syncMoveHead(float deg);  // +0x0D0 CfObject_syncMoveHead__Q22cf12CfObjectMoveFf
    virtual void CfObject_applyMoveYaw(float amount);  // +0x0D4 CfObject_applyMoveYaw__Q22cf12CfObjectMoveFf
    virtual float CfObject_getScaledAngle();  // +0x0D8 CfObject_getScaledAngle__Q22cf13CfObjectModelFv
    virtual void CfObject_setObjScale(float value);  // +0x0DC CfObject_setObjScale__Q22cf13CfObjectModelFf
    virtual float CfObject_getObjScale();  // +0x0E0 CfObject_getObjScale__Q22cf13CfObjectModelFv
    virtual int CfObject_resetAnimSlots(u32 arg);  // +0x0E4 CfObject_resetAnimSlots__Q22cf12CfObjectMoveFUl
    virtual int CfObject_clearAnimEntry(u32 arg);  // +0x0E8 CfObject_clearAnimEntry__Q22cf12CfObjectMoveFUl
    virtual int CfObject_armAnimSlot0(u32 id, u32 tag);  // +0x0EC CfObject_armAnimSlot0__Q22cf12CfObjectMoveFUlUl
    virtual int CfObject_armAnimSlot0N(u32 id, u32 nameIdx);  // +0x0F0 CfObject_armAnimSlot0N__Q22cf12CfObjectMoveFUlUl
    virtual int CfObject_UnkVirtualFunc41();  // +0x0F4 CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv
    virtual int CfObject_armAnimSlot1(u32 id, u32 tag);  // +0x0F8 CfObject_armAnimSlot1__Q22cf12CfObjectMoveFUlUl
    virtual int CfObject_armAnimSlot1N(u32 id, u32 nameIdx);  // +0x0FC CfObject_armAnimSlot1N__Q22cf12CfObjectMoveFUlUl
    virtual int CfObject_UnkVirtualFunc44();  // +0x100 CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv
    virtual void CfObject_rearmAnimSlot(u32 value, u32 index);  // +0x104 CfObject_rearmAnimSlot__Q22cf12CfObjectMoveFUlUl
    virtual void CfObject_setMoveTargetPtr(void* arg);  // +0x108 CfObject_setMoveTargetPtr__Q22cf12CfObjectMoveFPv
    virtual void CfObject_createMoveTarget(int kind);  // +0x10C CfObject_createMoveTarget__Q22cf12CfObjectMoveFi
    virtual void* CfObject_getCurrentTarget();  // +0x110 CfObject_getCurrentTarget__Q22cf12CfObjectMoveFv
    virtual void CfObject_forwardNpcAction(int action);  // +0x114 CfObject_forwardNpcAction__Q22cf12CfObjectMoveFi
    virtual int CfObject_getNpcActionState();  // +0x118 CfObject_getNpcActionState__Q22cf12CfObjectMoveFv
    virtual int CfObject_getNpcTargetAction();  // +0x11C CfObject_getNpcTargetAction__Q22cf12CfObjectMoveFv
    virtual void* CfObject_findNodeMatrix(const char* name) const;  // +0x120 CfObject_findNodeMatrix__Q22cf13CfObjectModelCFPCc
    virtual void* CfObject_getIndexedNodeMatrix(u32 idx) const;  // +0x124 CfObject_getIndexedNodeMatrix__Q22cf13CfObjectModelCFUl
    virtual u32 CfObject_getSubPosWord();  // +0x128 CfObject_getSubPosWord__Q22cf13CfObjectModelFv
    virtual void* CfObject_getPosSample(int arg) const;  // +0x12C CfObject_getPosSample__Q22cf13CfObjectModelCFi
    virtual float CfObject_getHeightDiff() const;  // +0x130 CfObject_getHeightDiff__Q22cf13CfObjectModelCFv
    virtual void CfObject_setMoveValue(float value);  // +0x134 CfObject_setMoveValue__Q22cf12CfObjectMoveFf
    virtual float* CfObject_getMoveRateScale();  // +0x138 CfObject_getMoveRateScale__Q22cf12CfObjectMoveFv
};

// Typed view of the +0x3E9C voice-owner interface for the height probe:
// slot 0x8C returns the actor height float (handleActKind gate).
class CtrlActVoiceHeightIntf {
public:
    virtual void setStateBitMask(u32 bits);  // +0x008 setStateBitMask__Q22cf12CObjectStateFUl
    virtual int checkStateFlags(int mask);  // +0x00C checkStateFlags__Q22cf12CObjectStateFi
    virtual void setStateBitFlag(u32 mask);  // +0x010 setStateBitFlag__Q22cf12CObjectStateFUl
    virtual void clearStateWord4();  // +0x014 clearStateWord4__Q22cf12CObjectStateFv
    virtual void applyStateFlags(int arg);  // +0x018 applyStateFlags__Q22cf12CObjectStateFi
    virtual void clearStateWord8();  // +0x01C clearStateWord8__Q22cf12CObjectStateFv
    virtual void clearStateFlags8(int arg);  // +0x020 clearStateFlags8__Q22cf12CObjectStateFi
    virtual int checkStateFlags8(int arg);  // +0x024 checkStateFlags8__Q22cf12CObjectStateFi
    virtual int checkStateFlagsC(int mask);  // +0x028 checkStateFlagsC__Q22cf12CObjectStateFi
    virtual int setStateBitMask0(int mask, int flag);  // +0x02C setStateBitMask0__Q22cf12CObjectStateFii
    virtual void* getStateData();  // +0x030 getStateData__Q22cf12CObjectStateFv
    virtual void* setStateBitMask2();  // +0x034 setStateBitMask2__Q22cf12CObjectStateFv
    virtual void setStateBitMask3();  // +0x038 setStateBitMask3__Q22cf12CObjectStateFv
    virtual void CObjectParam_setObjectName(const char* name);  // +0x03C CObjectParam_setObjectName__Q22cf12CObjectParamFPCc
    virtual void* CObjectParam_getParamPtr();  // +0x040 CObjectParam_getParamPtr__Q22cf12CfObjectMoveFv
    virtual int CObjectParam_hasObjectName();  // +0x044 CObjectParam_hasObjectName__Q22cf12CObjectParamFv
    virtual void CObjectParam_UnkVirtualFunc4();  // +0x048 CObjectParam_UnkVirtualFunc4__Q22cf12CObjectParamFv
    virtual BOOL CObjectParam_getSelfObjectId();  // +0x04C CObjectParam_getSelfObjectId__Q22cf12CObjectParamFv
    virtual void CObjectParam_signalActionEnd(int flag);  // +0x050 CObjectParam_signalActionEnd__Q22cf12CObjectParamFi
    virtual ~CtrlActVoiceHeightIntf();  // +0x054 __dt__Q22cf12CfObjectMoveFv
    virtual int CfObject_UnkVirtualFunc2() = 0;  // +0x058 CfObject_UnkVirtualFunc2__Q22cf13CfObjectModelFv
    virtual void CfObject_UnkVirtualFunc3();  // +0x05C CfObject_UnkVirtualFunc3__Q22cf8CfObjectFv
    virtual void CfObject_UnkVirtualFunc4() = 0;  // +0x060 CfObject_UnkVirtualFunc4__Q22cf12CfObjectMoveFv
    virtual void CfObject_updateMoveRate();  // +0x064 CfObject_updateMoveRate__Q22cf12CfObjectMoveFv
    virtual void CfObject_releaseMoveTargets();  // +0x068 CfObject_releaseMoveTargets__Q22cf12CfObjectMoveFv
    virtual void CfObject_UnkVirtualFunc7() = 0;  // +0x06C CfObject_UnkVirtualFunc7__Q22cf12CfObjectMoveFv
    virtual void CfObject_notifyEventDone() = 0;  // +0x070 CfObject_notifyEventDone__Q22cf13CfObjectModelFv
    virtual bool CfObject_isMoveActiveNow();  // +0x074 CfObject_isMoveActiveNow__Q22cf12CfObjectMoveFv
    virtual void CfObject_forwardSubObject();  // +0x078 CfObject_forwardSubObject__Q22cf12CfObjectMoveFv
    virtual int CfObject_notifyMemberRemoved();  // +0x07C CfObject_notifyMemberRemoved__Q22cf8CfObjectFv
    virtual void CfObject_refreshSubB0();  // +0x080 CfObject_refreshSubB0__Q22cf12CfObjectMoveFv
    virtual int CfObject_queryTargetState();  // +0x084 CfObject_queryTargetState__Q22cf12CfObjectMoveFv
    virtual void* CfObject_pushRefreshValue(float value);  // +0x088 CfObject_pushRefreshValue__Q22cf12CfObjectMoveFf
    virtual f32 CfObject_getMoveSpeedRate();  // +0x08C CfObject_getMoveSpeedRate__Q22cf12CfObjectMoveFv
};

// player->vf167() (vtable slot 0x2A4) result view: the action/attack state
// block cleared and refilled by ctrlActDispatchEntry.
struct CtrlActSub2A4 {
    u32 mField0;             // 0x00
    u32 mField4;             // 0x04
    u32 mField8;             // 0x08
    u32 mFieldC;             // 0x0C
    u32 mField10;            // 0x10
    u32 mField14;            // 0x14
    u32 mField18;            // 0x18
    u32 mField1C;            // 0x1C
    u32 mField20;            // 0x20
    u32 mField24;            // 0x24
    u32 mField28;            // 0x28
    u32 mField2C;            // 0x2C
    u32 mField30;            // 0x30
    u32 mField34;            // 0x34
    u32 mField38;            // 0x38
    u32 mField3C;            // 0x3C
    u32 mField40;            // 0x40
    u32 mField44;            // 0x44 (opaque words 0x08-0x47, copied verbatim)
    u32 mField48;            // 0x48
    u32 mField4C;            // 0x4C
    u32 mField50;            // 0x50 (CtrlActAtkParam* in parseActEntry)
    f32 mField54;            // 0x54
    f32 mField58;            // 0x58
    f32 mField5C;            // 0x5C
    f32 mField60;            // 0x60
    f32 mField64;            // 0x64
    f32 mField68;            // 0x68
    f32 mField6C;            // 0x6C
    s16 mField70;            // 0x70
    s16 mField72;            // 0x72
    u32 mField74;            // 0x74
    u32 mField78;            // 0x78 (flag word: 0x40002000 base + kind bits)
    u32 mField7C;            // 0x7C
    u16 mField80;            // 0x80
    // (0x82-0x83 unnamed alignment padding: skipped by member-wise copies,
    // matching retail)
    u8 _84[0x34];           // 0x84..0xB7 (bulk-cleared by parseActEntry)
    u32 mFieldB8;
};

// Real player/source dispatch for ctrlActDispatchEntry. In the retail tables
// the words after CActorState's first slot (+0x00C RTTI, +0x010 offset-to-top)
// are the header of CActorParam's CBattleState base group, not virtual
// functions. Inheriting the real CActorParam makes MWCC emit that group and
// gives these calls the base's real virtuals at their retail slots.
class CtrlActPlayerReal : public cf::CActorParam {
};


// Action-source view (MapFx_ResolveOwner result): only the 0x2BC gate is
// dispatched through here in ctrlActDispatchEntry.
class CtrlActSrcReal : public cf::CActorParam {
};


// Player actor view (CtrlPc+0x5C) covering the fields/vtable slots this TU
// touches: slot 0x288 (getAtkParam base), slot 0x2A4 (action-state block) and
// slot 0x1C0 (height/radius probe returning a float vector).
struct PlayerTable {
    void* p00[0x30 / 4];
    u32* (*method30)(void* self);              // mField4 sub-object slot 0x30
    void* r34[(0x70 - 0x34) / 4];
    void (*method70)(void* self, void* arg);   // 0x70 (parseActEntry L_3460)
    void* r74[(0xF0 - 0x74) / 4];
    f32 (*methodF0)(void* self);               // 0xF0 (returns a height scalar)
    void* rF4[(0x1C0 - 0xF4) / 4];
    float* (*method1C0)(void* self);           // 0x1C0 (returns float vector, [0] = height)
    void* r1C4[(0x1D0 - 0x1C4) / 4];
    float* (*method1D0)(void* self);           // 0x1D0 (returns float vector, [0] = height)
    void* r1D4[(0x23C - 0x1D4) / 4];
    f32 (*method23C)(void* self);              // 0x23C (returns a scalar scale factor)
    void* r240[(0x27C - 0x240) / 4];
    void* (*method27C)(void* self);            // 0x27C (arts set base, runArtsAttack)
    void* r280[(0x288 - 0x280) / 4];
    void* (*method288)(void* self);            // 0x288
    void* r28C[(0x2A4 - 0x28C) / 4];
    CtrlActSub2A4* (*method2A4)(void* self);   // 0x2A4
    void* r2A8[(0x2BC - 0x2A8) / 4];
    int (*method2BC)(void* self);              // 0x2BC (usable gate, updateActMain)
    void* r2C0[(0x5B4 - 0x2C0) / 4];
    f32 (*method5B4)(void* self);              // 0x5B4 (base facing angle, tickTargetAct)
    void* r5B8[(0x5C8 - 0x5B8) / 4];
    void (*method5C8)(void* self, int flag);   // 0x5C8 (parseActEntry)
};

// 32-byte battle-list entry (player +0x358C array, indexed modulo 0x3598).
struct CtrlActBattleEntry {
    u8 _00[0xD];
    u8 mFieldD;              // 0x0D kind byte (3 = use vf1C0 height probe)
    u8 _0E[0x18 - 0x0E];
    void* mField18;          // 0x18 sub-object (height float at +0x60)
};
struct CtrlActBattleEntryObj {
    u8 _00[0x60];
    f32 mField60;            // 0x60 (height value read by updateActMain)
};

struct CtrlActPlayerView {
    PlayerTable* table;     // 0x00
    CtrlPlayerSub4* mField4;     // 0x04 (sub-object, vf30 -> u32* word holder)
    void* mField8;               // 0x08 embedded arts container (vtbl pointer)
    u8 _0C[0x1530 - 0x0C];
    u32 mField1530;              // 0x1530
    u8 _1534[0x3374 - 0x1534];
    u32 mField3374;              // 0x3374 (bit 15 test by isMoveBlocked)
    u8 _3378[0x3380 - 0x3378];
    u32 mField3380;              // 0x3380 (status block, aiActionUpdateEntriesDelta/B8BC)
    u8 _3384[0x3388 - 0x3384];
    u16 mField3388;              // 0x3388 (bit 4 test by updateActMain)
    u8 _338A[0x358C - 0x338A];
    CtrlActBattleEntry* mField358C;  // 0x358C (battle-list entry array base)
    u32 mField3590;              // 0x3590 (ring offset)
    u32 mField3594;              // 0x3594 (entry count)
    u32 mField3598;              // 0x3598 (ring modulus)
    u8 _359C[0x3E6C - 0x359C];
    u16 mField3E6C;              // 0x3E6C (bit 12 test by runArtsAttack)
    u8 _3E6E[0x3E9C - 0x3E6E];
    CVoiceOwnerIntfPc mSub3E9C;  // 0x3E9C embedded voice-owner interface
    u8 _3EA0[0x3ED4 - 0x3EA0];
    void* mField3ED4;            // 0x3ED4 (sub-object, slot 0x70 dispatch)
    u8 _3ED8[0x3F00 - 0x3ED8];
    u32 mField3F00;              // 0x3F00 (bit 1 / bit 2 tests)
    u8 _3F04[0x3F10 - 0x3F04];
    u32 mField3F10;              // 0x3F10 (battle-target actor id, runArtsAttack)
    u8 _3F14[0x3F60 - 0x3F14];
    CtrlPlayerSub3F60* mField3F60;  // 0x3F60 (voice/battle target)
    u8 _3F64[0x44D8 - 0x3F64];
    f32 mField44D8;              // 0x44D8 (proximity radius, scanPlayerTgt)
};

// Action-source object (MapFx_ResolveOwner result): vtable slots 0x2BC (usable
// gate) and 0x5B4 (base facing), +0x04 sub-object, +0x3E9C voice-owner.
struct SrcTable {
    void* p00[0x2BC / 4];
    int (*method2BC)(void* self);              // 0x2BC
    void* r2C0[(0x5B4 - 0x2C0) / 4];
    f32 (*method5B4)(void* self);              // 0x5B4
};
struct CtrlActSrc {
    SrcTable* table;        // 0x00
    CtrlPlayerSub4* mField4;     // 0x04
    u8 _08[0x3388 - 0x08];
    u16 mField3388;              // 0x3388 (bit 3 latch, parseActEntry case)
    u8 _338A[0x3E9C - 0x338A];
    CVoiceOwnerIntfPc mOwner3E9C;  // 0x3E9C embedded voice-owner interface
    u8 _3EA0[0x44D8 - 0x3EA0];
    f32 mField44D8;              // 0x44D8 (probe scale offset, loadActionSrc)
    u8 _44DC[0x455A - 0x44DC];
    u16 mField455A;              // 0x455A (battle-list lock word, tickTargetAct)
};

// Attack-parameter record returned by getAtkParam (index*0x88 + 0x10).
struct CtrlActAtkParam {
    u8 _00[0x42];
    u8 mField42;               // 0x42 (kind gate, runArtsAttack)
    u8 _43[0x48 - 0x43];
    u16 mField48;              // 0x48 (arts id, runArtsAttack)
    u8 _4A[0x76 - 0x4A];
    u8 mField76;               // 0x76 (count; +1 written by ctrlActDispatchEntry)
    u8 _77;
    u32 mField78;              // 0x78 (flag word read by parseActEntry)
};

// Player +0x08 embedded arts container: vtable slot 0x20 takes an id.
struct ArtsTable {
    void* p00[0x20 / 4];
    void (*method20)(void* self, int id);      // 0x20
};

struct CtrlActBattleSubView {
    u32 mField0;               // 0x00
    u8 _4[0x824 - 0x4];
    u32 mField824;             // 0x824 (bit 20 = 0x100000 test)
    u8 _828[0x219C - 0x828];
    u8 mField219C;             // 0x219C (fed to installVisionStateCallback)
};

// Argument view shared by ctrlActDispatchEntry / runArtsAttack: actor id + kind
// byte + attack index.
struct CtrlActAtkArg {
    u32 mField0;               // 0x00 actor id
    u8 _4[0xD - 0x4];
    u8 mFieldD;                // 0x0D kind byte (2/3/0x54/0x55)
    u8 _E[0x12 - 0xE];
    s16 mField12;              // 0x12 attack index (signed)
    u8 _14[0x18 - 0x14];
    u32 mField18;              // 0x18
};

// Real cf::CtrlAct layout (cf::CtrlPc-family action controller). With -RTTI
// on, MWCC emits two leading vtable entries, so the Nth declared virtual sits
// at vtable offset (N+2)*4.
namespace cf {
class __declspec(novtable) CtrlAct {
public:
    // Retail vtable lbl_eu_8052B080 (36 words, see CBattleManager.cpp):
    // declared virtual N occupies word (N+2)/4. Names and signatures are
    // those of the functions in the retail slots.
    virtual ~CtrlAct();  // 0x08 __dt__Q22cf7CtrlActFv
    virtual void CtrlPc_UpdatePadState() = 0;  // 0x0C pure; retail slot is 0, overridden in CtrlPc/CtrlEnemy/CtrlRemote vtables
    virtual int CtrlNpcAlwaysFalse();  // 0x10
    virtual unsigned long ctrlActIsFlagBit15Clear();  // 0x14
    virtual int ret0_800888E8();  // name-guess: no retail member name; return-0 stub at 0x800892C0, slot 0x18
    virtual int ret0_800888E0();  // name-guess: no retail member name; return-0 stub at 0x800892B8, slot 0x1C
    virtual int ret0_800888D8();  // name-guess: no retail member name; return-0 stub at 0x800892B0, slot 0x20
    virtual int ret0_800888D0();  // name-guess: no retail member name; return-0 stub at 0x800892A8, slot 0x24
    virtual int ret0_800888C8();  // name-guess: no retail member name; return-0 stub at 0x800892A0, slot 0x28
    virtual int ret0_800888C0();  // name-guess: no retail member name; return-0 stub at 0x80089298, slot 0x2C
    virtual int ret0_800888B8();  // name-guess: no retail member name; return-0 stub at 0x80089290, slot 0x30
    virtual int ret0_800888B0();  // name-guess: no retail member name; return-0 stub at 0x80089288, slot 0x34
    virtual int ret0_800888A8();  // name-guess: no retail member name; return-0 stub at 0x80089280, slot 0x38
    virtual int ret0_800888A0();  // name-guess: no retail member name; return-0 stub at 0x80089278, slot 0x3C
    virtual int ret0_80088898();  // name-guess: no retail member name; return-0 stub at 0x80089270, slot 0x40
    virtual int ret0_80088890();  // name-guess: no retail member name; return-0 stub at 0x80089268, slot 0x44
    virtual int ret0_80088888();  // name-guess: no retail member name; return-0 stub at 0x80089260, slot 0x48
    virtual int ret0_80088880();  // name-guess: no retail member name; return-0 stub at 0x80089258, slot 0x4C
    virtual int ret0_80088878();  // name-guess: no retail member name; return-0 stub at 0x80089250, slot 0x50
    virtual int ret0_80088870();  // name-guess: no retail member name; return-0 stub at 0x80089248, slot 0x54
    virtual void parseActEntry();  // 0x58
    virtual void aimVoiceOwner();  // 0x5C
    virtual void updateActMain();  // 0x60
    virtual int ctrlActComputeAimVec(ml::CVec3* a, f32* b, ml::CVec3* c, int d, int e); // 0x64
    virtual int ctrlActAimAtTarget(ml::CVec3* out, const CVoicePos* pos);             // 0x68
    virtual int ctrlActFaceTargetVec(ml::CVec3* out);                                   // 0x6C
    virtual int ctrlActProbeTargetReset(ml::CVec3* out, int flag);                         // 0x70
    virtual int ctrlActCheckBlockedProbe(ml::CVec3* out);                                   // 0x74
    virtual int ctrlActDispatchEntry(void* entry);  // 0x78 (arts entry)
    virtual int runArtsAttack(void* entry);  // 0x7C (arts entry)
    virtual int ret1_800888F4();  // name-guess: no retail member name; return-1 stub at 0x800892CC, slot 0x80
    virtual void CtrlEnemy_emptyStub_800888F0();  // name-guess: no retail member name; empty void stub at 0x800892C8, slot 0x84
    virtual int handleActKind();  // 0x88
    virtual void dispatchCtrlPcActions() = 0;  // name-guess: overrides are func_80098EF8/CtrlEnemy_UnkFunc_800877A8 (no real member name); slot 0x8C pure
    // NOTE: the retail table ends at +0x8C; there are no further slots.

    u32 mField4;               // 0x04 (flag word, bit 1 set by ctrlActDispatchEntry)
    f32 mField8;               // 0x08
    f32 mFieldC;               // 0x0C target angle
    f32 mField10;              // 0x10 computed facing
    f32 mField14;              // 0x14 aim/fx state value
    u32 mField18;              // 0x18 (attack counter written by ctrlActDispatchEntry)
    u32 mField1C;              // 0x1C (ctor sets 2)
    u32 mField20;              // 0x20
    u32 mField24;              // 0x24 (ctor sets -1)
    CtrlActViewSub28* mField28; // 0x28 (vtable slot 0x138 float source)
    u32 mField2C;              // 0x2C
    ml::CVec3 mPos30;          // 0x30 (start of the 0x2C-byte block cleared in the handlers)
    f32 mField3C;              // 0x3C
    f32 mField40;              // 0x40
    f32 mField44;              // 0x44
    f32 mField48;              // 0x48
    f32 mField4C;              // 0x4C (facing angle written by progressPhase)
    f32 mField50;              // 0x50
    f32 mField54;              // 0x54 action timer
    CtrlActFlags mFlags58;     // 0x58
    CtrlActPlayerView* mPlayer;// 0x5C player actor object
    ml::CVec3 mPos60;          // 0x60 position block (tickTargetAct walks it)
    f32 mField6C;              // 0x6C (facing reference, tickTargetAct)
    f32 mField70;              // 0x70 (facing blend value, updateActMain)
    u16 mField74;              // 0x74 (action state bits 1-3, testHeightGap)
    s16 mField76;              // 0x76 (countdown, updateActMain)
    u8 mField78;               // 0x78 (probe countdown, scanPlayerTgt)
    u8 mField79;               // 0x79
    u8 mField7A;               // 0x7A (cleared by handleActKind kind 9/10)
    u8 mField7B;               // 0x7B (probe counter, scanPlayerTgt)
};
} // namespace cf

// Extended view of the +0x3E9C voice-owner interface exposing the vtable
// slot 0x1AC used by aimVoiceOwner (the CVoiceOwnerIntfPc view in CtrlPc.hpp
// stops at 0xC4). Index 105 -> vtable 0x1AC.
class CtrlActVoiceOwnerIntf : public CVoiceOwnerIntfPc {
public:
    virtual void CfObject_setMoveYaw(float value);  // +0x0C8 CfObject_setMoveYaw__Q22cf12CfObjectMoveFf
    virtual float CfObject_getMoveHeadAngle();  // +0x0CC CfObject_getMoveHeadAngle__Q22cf13CfObjectModelFv
    virtual void CfObject_syncMoveHead(float deg);  // +0x0D0 CfObject_syncMoveHead__Q22cf12CfObjectMoveFf
    virtual void CfObject_applyMoveYaw(float amount);  // +0x0D4 CfObject_applyMoveYaw__Q22cf12CfObjectMoveFf
    virtual float CfObject_getScaledAngle();  // +0x0D8 CfObject_getScaledAngle__Q22cf13CfObjectModelFv
    virtual void CfObject_setObjScale(float value);  // +0x0DC CfObject_setObjScale__Q22cf13CfObjectModelFf
    virtual float CfObject_getObjScale();  // +0x0E0 CfObject_getObjScale__Q22cf13CfObjectModelFv
    virtual int CfObject_resetAnimSlots(u32 arg);  // +0x0E4 CfObject_resetAnimSlots__Q22cf12CfObjectMoveFUl
    virtual int CfObject_clearAnimEntry(u32 arg);  // +0x0E8 CfObject_clearAnimEntry__Q22cf12CfObjectMoveFUl
    virtual int CfObject_armAnimSlot0(u32 id, u32 tag);  // +0x0EC CfObject_armAnimSlot0__Q22cf12CfObjectMoveFUlUl
    virtual int CfObject_armAnimSlot0N(u32 id, u32 nameIdx);  // +0x0F0 CfObject_armAnimSlot0N__Q22cf12CfObjectMoveFUlUl
    virtual int CfObject_UnkVirtualFunc41();  // +0x0F4 CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv
    virtual int CfObject_armAnimSlot1(u32 id, u32 tag);  // +0x0F8 CfObject_armAnimSlot1__Q22cf12CfObjectMoveFUlUl
    virtual int CfObject_armAnimSlot1N(u32 id, u32 nameIdx);  // +0x0FC CfObject_armAnimSlot1N__Q22cf12CfObjectMoveFUlUl
    virtual int CfObject_UnkVirtualFunc44();  // +0x100 CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv
    virtual void CfObject_rearmAnimSlot(u32 value, u32 index);  // +0x104 CfObject_rearmAnimSlot__Q22cf12CfObjectMoveFUlUl
    virtual void CfObject_setMoveTargetPtr(void* arg);  // +0x108 CfObject_setMoveTargetPtr__Q22cf12CfObjectMoveFPv
    virtual void CfObject_createMoveTarget(int kind);  // +0x10C CfObject_createMoveTarget__Q22cf12CfObjectMoveFi
    virtual void* CfObject_getCurrentTarget();  // +0x110 CfObject_getCurrentTarget__Q22cf12CfObjectMoveFv
    virtual void CfObject_forwardNpcAction(int action);  // +0x114 CfObject_forwardNpcAction__Q22cf12CfObjectMoveFi
    virtual int CfObject_getNpcActionState();  // +0x118 CfObject_getNpcActionState__Q22cf12CfObjectMoveFv
    virtual int CfObject_getNpcTargetAction();  // +0x11C CfObject_getNpcTargetAction__Q22cf12CfObjectMoveFv
    virtual void* CfObject_findNodeMatrix(const char* name) const;  // +0x120 CfObject_findNodeMatrix__Q22cf13CfObjectModelCFPCc
    virtual void* CfObject_getIndexedNodeMatrix(u32 idx) const;  // +0x124 CfObject_getIndexedNodeMatrix__Q22cf13CfObjectModelCFUl
    virtual u32 CfObject_getSubPosWord();  // +0x128 CfObject_getSubPosWord__Q22cf13CfObjectModelFv
    virtual void* CfObject_getPosSample(int arg) const;  // +0x12C CfObject_getPosSample__Q22cf13CfObjectModelCFi
    virtual float CfObject_getHeightDiff() const;  // +0x130 CfObject_getHeightDiff__Q22cf13CfObjectModelCFv
    virtual void CfObject_setMoveValue(float value);  // +0x134 CfObject_setMoveValue__Q22cf12CfObjectMoveFf
    virtual u32* CfObject_getMoveRateScale();  // +0x138 CfObject_getMoveRateScale__Q22cf12CfObjectMoveFv
    virtual void CfObject_setMoveScale(float value);  // +0x13C CfObject_setMoveScale__Q22cf12CfObjectMoveFf
    virtual float CfObject_getMoveFactor();  // +0x140 CfObject_getMoveFactor__Q22cf12CfObjectMoveFv
    virtual void CfObject_setAnimSlotEntry(u32 a, u32 b);  // +0x144 CfObject_setAnimSlotEntry__Q22cf12CfObjectMoveFUlUl
    virtual u32 CfObject_getSlotBits(u32 arg);  // +0x148 CfObject_getSlotBits__Q22cf12CfObjectMoveFUl
    virtual u32 CfObject_readKindFlagWord();  // +0x14C CfObject_readKindFlagWord__Q22cf13CfObjectModelFv
    virtual void CfObject_setMoveBusyState(int flag);  // +0x150 CfObject_setMoveBusyState__Q22cf12CfObjectMoveFi
    virtual void CfObject_setMoveReadyFlag(int flag);  // +0x154 CfObject_setMoveReadyFlag__Q22cf12CfObjectMoveFi
    virtual void setPointEnabled(int flag) = 0;  // +0x158 setPointEnabled__Q22cf12CfObjectMoveFi
    virtual void CfObject_setVisibleFlag(int flag);  // +0x15C CfObject_setVisibleFlag__Q22cf13CfObjectModelFi
    virtual int CfObject_UnkVirtualFunc68(const ml::CVec3* vec = nullptr);  // +0x160 CfObject_UnkVirtualFunc68__Q22cf13CfObjectModelFPCQ22ml5CVec3
    virtual u32 CfObject_isVisibleFlag();  // +0x164 CfObject_isVisibleFlag__Q22cf13CfObjectModelFv
    virtual void CfObject_syncModelRate(float value);  // +0x168 CfObject_syncModelRate__Q22cf12CfObjectMoveFf
    virtual float CfObject_getProgressLow();  // +0x16C CfObject_getProgressLow__Q22cf13CfObjectModelFv
    virtual void CfObject_setProgressHigh(float value);  // +0x170 CfObject_setProgressHigh__Q22cf13CfObjectModelFf
    virtual float CfObject_getProgressHigh();  // +0x174 CfObject_getProgressHigh__Q22cf13CfObjectModelFv
    virtual void CfObjectModel_releaseModelSub();  // +0x178 CfObjectModel_releaseModelSub__Q22cf12CfObjectMoveFv
    virtual void CfObjectModel_releaseModelList();  // +0x17C CfObjectModel_releaseModelList__Q22cf12CfObjectMoveFv
    virtual void CfObjectModel_getModelName();  // +0x180 CfObjectModel_getModelName__Q22cf13CfObjectModelFv
    virtual void CfObjectModel_getAnimState();  // +0x184 CfObjectModel_getAnimState__Q22cf12CfObjectMoveFv
    virtual void CfObjectModel_getAnimFlags();  // +0x188 CfObjectModel_getAnimFlags__Q22cf12CfObjectMoveFv
    virtual void CfObjectModel_checkTargetNode();  // +0x18C CfObjectModel_checkTargetNode__Q22cf12CfObjectMoveFPv
    virtual void CfObjectModel_snapSubTarget();  // +0x190 CfObjectModel_snapSubTarget__Q22cf13CfObjectModelFPQ22ml5CVec3f
    virtual void CfObjectModel_getSubFlag04();  // +0x194 CfObjectModel_getSubFlag04__Q22cf13CfObjectModelFv
    virtual void CfObjectModel_getModelFloatA8();  // +0x198 CfObjectModel_getModelFloatA8__Q22cf13CfObjectModelFv
    virtual void CfObjectModel_setModelFloatA8();  // +0x19C CfObjectModel_setModelFloatA8__Q22cf13CfObjectModelFf
    virtual void CfObjectModel_getModelRate();  // +0x1A0 CfObjectModel_getModelRate__Q22cf13CfObjectModelFv
    virtual void CfObjectModel_setModelRate();  // +0x1A4 CfObjectModel_setModelRate__Q22cf13CfObjectModelFf
    virtual void CfObjectModel_aimAtPosition();  // +0x1A8 CfObjectModel_aimAtPosition__Q22cf13CfObjectModelFPCQ22ml5CVec3
    virtual void CfObjectModel_bindModelTo(void* a, const char* b);  // +0x1AC CfObjectModel_bindModelTo__Q22cf13CfObjectModelFPQ22cf8CfObjectPCc
};

// Voice-owner vtable slots used by the action handlers with non-base
// signatures: 0x8C returns the actor height float, 0x9C takes a vec arg,
// 0xC8 takes a float arg (v47/0xC4 stays the base CVoiceOwnerIntfPc form).
struct CtrlActPosBlock {
    u8 _00[0xC];
    f32 mFieldC;             // 0x0C (anchor x)
    u8 _10[0x1C - 0x10];
    f32 mField1C;            // 0x1C (anchor y)
    u8 _20[0x2C - 0x20];
    f32 mField2C;            // 0x2C (anchor z)
};


// Stack frame block built by rotateActPose phase 0: the rotated placement
// offset followed by a Y-rotation basis (cos/sin columns and the CFC y-scale).
// Retail writes the whole block but only reads the offset back.
struct CtrlActRotFrame {
    ml::CVec3 mOffset;  // 0x00 rotated/scaled direction (fed into mPos30)
    ml::CVec3 mAxisX;   // 0x0C (cos, 0, -sin)
    ml::CVec3 mAxisY;   // 0x18 (0, CFC, 0)
    ml::CVec3 mAxisZ;   // 0x24 (sin, 0, cos)
};

// View of the action-source object (MapFx_ResolveOwner result): the embedded
// voice-owner region sits at +0x3E9C (mirror of CtrlRemote's sweep view).
struct CtrlActSweepView {
    u8 _00[0x3E9C];
    u8 mOwner3E9C[0x10];  // 0x3E9C voice-owner region (opaque)
};

// 0x20-byte action entry pulled by aiFetchAction (battle-list action record).
struct CtrlActActionEntry {
    u32 mField0;             // 0x00 actor id
    u8 _04[0xD - 0x04];
    u8 mKind;                // 0x0D kind byte (switch selector)
    u8 _0E[0x10 - 0x0E];
    u16 mFlags10;            // 0x10 flags (bit 0x20 phase, bit 0x400 follow)
    s16 mArtsIdx;            // 0x12 signed arts index
    u8 _14[0x18 - 0x14];
    void* mPtr18;            // 0x18 entry object (floats at +0x2C/+0x7C)
    u32 mState;              // 0x1C state value
};

// Entry object behind CtrlActActionEntry::mPtr18.
struct CtrlActEntryObj {
    u8 _00[0x2C];
    f32 mField2C;            // 0x2C source value copied to +0x7C
    u8 _30[0x7C - 0x30];
    f32 mField7C;            // 0x7C
};

// Effect-request block built on the stack (0x34 bytes incl. tail) and passed
// to CBattleMan_RunBattleEvent.
struct CtrlActFxReq {
    u32 mField0;             // 0x00
    u32 mField4;             // 0x04
    u8 _08[0xC - 0x08];
    u16 mIdC;                // 0x0C effect id (0x111/0x112)
    u8 _0E[0x20 - 0x0E];
    f32 mField20;            // 0x20 (lbl_eu_80666D48 seed)
    u8 _24[0x34 - 0x24];
};

// (case 22 dispatches through the real cf::CAttackParam virtual
// CAttackParam_getArtsGaugeMax, CArtsSet.hpp; no local pad.)

// Battle-manager view for the virtual slot 0x2C dispatch (parseActEntry).
struct Bm2Table {
    void* p00[0x2C / 4];
    void (*fn2C)(void* self, void* player, void* sub);
};
struct CtrlActBm2 {
    Bm2Table* table;
};



// Chain-object view for the getEffOwner path of parseActEntry.
struct CtrlActChainObj {
    u8 _00[0xA0];
    u16 mFieldA0;            // 0xA0 (bit 1 latch)
    u8 _A2[0x45C0 - 0xA2];
    u16 mField45C0;          // 0x45C0 (probe id fed to CPartsChange_FindActorById)
};

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------
extern "C" long testActorStatus(void* actor, void* outVal, unsigned long flags); // matches CfMapItemManager.hpp

extern "C" int getAnimModelId(void* battleObj);  // battle-state page id
extern "C" int isBattleStatusActive(void* obj, int id);
// updateActMain / tickTargetAct extra imports (retail C-ABI names).
extern "C" f32 CfCmd_GetThreshold();                    // battle-list fallback height
// Chain-start gate (chain/CChain.cpp); retail call site uses the plain name.
extern "C" int CChain_ValidateTarget(void* a, void* b);
// monolib coli segment probes (retail C-ABI names).
extern "C" int ColiQuerySpecDispatch(void* self, void* src, int a, int b);
extern "C" int Coli_WalkQuery(void* a, void* b, void* c, int d, int e, int f);
extern "C" int ColiWalkSegment(void* a, void* b, void* c, void* d, int e, int f, int g);
extern "C" int Coli_SweepSegNodes(void* a, void* b, void* c, int d, int e);
// runArtsAttack imports: arts-param lookups, battle-manager sub-views and the
// arts-id predicate. (getInstance__Q22cf14CBattleManagerFv is declared in
// kyoshin/cf/CBattleManager.hpp.)
extern "C" void* getArtsSlotByFlatIdx(void* base, int index);
extern "C" void* getArtsParamByIdx(void* base, int index);
extern "C" int installVisionStateCallback(void* a, void* b);
extern "C" int isBattleEventKind3(int val);
// parseActEntry imports: status-block timers, battle-entry fetch, effect
// requests and the chain/battle helpers.
extern "C" int aiFetchAction(void* obj, void* out);
extern "C" void CCharVoiceMan_EnqueueCtrlActVoiceA(void* a, void* b);
extern "C" void CCharVoiceMan_EnqueueCtrlActVoiceB(void* a, void* b);
extern "C" void CCharVoiceMan_EnqueueCtrlActVoiceC(void* a, void* b);
// getEffOwner is declared (mangled C++) in CfGameManager.hpp.
extern "C" void* CPartsChange_GetActorTable(void* p);
extern "C" void* CPartsChange_FindActorById(void* p, u16 arg);
// In-TU kind setter used by the parseActEntry switch arms.
void ctrlActSetupActionKind(cf::CtrlAct* self, u32 kind, int param);
// Collision-query API: ScnRes_VertRayForward_E398 is declared in CtrlMoveBase.hpp
// (retail C-ABI name; 6-arg probe passing two FP args after the GPR args).
extern "C" int ScnRes_SegQueryForward_E348(void* a, void* b, int c, int d, int e);
extern "C" int ScnRes_GetEntryCount_E4AC(void);
extern "C" void* ScnRes_GetEntryPtr_E50C(u32 index);
extern "C" void* ScnRes_GetEntryHead2_E520(int index);
extern "C" int ScnRes_EntryFlagThunk_E5A4(int a, int b);
// Attack-param table lookup (CArtsSet.cpp): base + index*0x88 + 0x10.
extern "C" void* getAtkParam(void* base, int index);
// Enum-list actor-id query (target 5 scan loop).
// findFirstCleanObjectId is declared in CtrlPc.hpp (extern "C" void* form).
// In-TU facing helper called by progressPhase (retail plain C name).
extern "C" int testFacingGap(cf::CtrlAct* self, CtrlActSrc* src);

// C++-mangled retail helper findObjectById__Fi (actor id -> action source).
void* findObjectById(int id);

// cf::CfGameManager singleton accessor (mangles to getInstance__Q22cf13CfGameManagerFv)
// and the presentation/event flag probe isGlobalCamFlagSet__Fi.


// In-TU action-handler entry points: C linkage so the call relocs carry the
// retail plain names (testFacingGap convention).
extern "C" int handleActKind(cf::CtrlAct* self);
extern "C" void setupActTimer(cf::CtrlAct* self);
extern "C" void progressPhase(cf::CtrlAct* self);
extern "C" void rotateActPose(cf::CtrlAct* self);
extern "C" void ctrlActState4Handler(cf::CtrlAct* self);
extern "C" void testAimWobble(cf::CtrlAct* self);
extern "C" void stepActMotion(cf::CtrlAct* self);
extern "C" void loadActionSrc(cf::CtrlAct* self);
extern "C" void ctrlActHandleCase4Move(cf::CtrlAct* self);
extern "C" void aimNearestFoe(cf::CtrlAct* self);
extern "C" void snapFacingAng(cf::CtrlAct* self);
extern "C" void scanPlayerTgt(cf::CtrlAct* self, ml::CVec3* pos);
extern "C" int isMoveBlocked(cf::CtrlAct* self);
extern "C" int testHeightGap(cf::CtrlAct* self, int flag);

// Scene object pointer fed to Scn_GetFrameDelta (.sbss).
class CScn;

// Retail vtables stored manually by the novtable ctors (.data).
extern const u32 lbl_eu_80527BB0[];
extern const u32 lbl_eu_8052B080[];

// Sub-object / player resolvers used by the cf::CtrlAct ctor.
typedef void CtrlActorObj;
// Sub-object resolver used by the cf::CtrlAct ctor (retail plain C name).
extern "C" CtrlActorObj* func_800BBC0C(void* param);

// sdata2 float constants (const so MWCC references the retail pool slots).
extern const f32 lbl_eu_80666CF8;   // 0.0f
extern const f32 lbl_eu_80666CFC;
extern const f32 lbl_eu_80666D00;
extern const f32 lbl_eu_80666D04;
extern const f32 lbl_eu_80666D08;
extern const f32 lbl_eu_80666D0C;
extern const f32 lbl_eu_80666D10;
extern const f32 lbl_eu_80666D14;
extern const f32 lbl_eu_80666D18;
extern const f32 lbl_eu_80666D1C;
extern const f32 lbl_eu_80666D20;
extern const f32 lbl_eu_80666D24;
extern const f32 lbl_eu_80666D28;
extern const f32 lbl_eu_80666D2C;
extern const f64 lbl_eu_80666D30;   // 0.1 (double)
extern const f32 lbl_eu_80666D38;
extern const f32 lbl_eu_80666D3C;
extern const f32 lbl_eu_80666D40;
extern const f32 lbl_eu_80666D44;
extern const f32 lbl_eu_80666D48;
extern const f32 lbl_eu_80666D4C;
extern const f64 lbl_eu_80666D50;   // 2^52 (u32->double conversion magic)
extern const f32 lbl_eu_80666D58;
extern const f32 lbl_eu_80666D5C;
extern const f32 lbl_eu_80666D60;
extern const f32 lbl_eu_80666D64;
extern const f32 lbl_eu_80666D68;
extern const f32 lbl_eu_80666D6C;
extern const f32 lbl_eu_80666D70;
extern const f64 lbl_eu_80666D78;   // 2^52 (u32->double conversion magic)
extern const f32 lbl_eu_80666D80;
extern const f32 lbl_eu_80666D84;
extern const f32 lbl_eu_80666D88;
extern const f32 lbl_eu_80666D8C;
extern const f32 lbl_eu_80666D90;
extern const f32 lbl_eu_80666DB4;
extern const f32 lbl_eu_80666DB8;
extern const f32 lbl_eu_80666DBC;
extern const f32 lbl_eu_80666DC0;
extern const f32 lbl_eu_80666DC8;
extern const f32 lbl_eu_80666DCC;
extern const f32 lbl_eu_80666D94;
extern const f32 lbl_eu_80666D98;
extern const f32 lbl_eu_80666D9C;
extern const f32 lbl_eu_80666DA0;
extern const f32 lbl_eu_80666DA4;
extern const f32 lbl_eu_80666DA8;
extern const f32 lbl_eu_80666DAC;
extern const f32 lbl_eu_80666DB0;
extern const f32 lbl_eu_80666DC4;
extern const f32 lbl_eu_80666DA4;
extern const f32 lbl_eu_80666DA8;
extern const f32 lbl_eu_80666DAC;
extern const f32 lbl_eu_80666DB0;

// 2-float blocks (base + f2 offset pairs for the facing-state table).
// Non-const: sinit_800D79B4 writes these at static-init time.
extern f32 lbl_eu_80663EF8[2];   // sda2 (sda21 addressing)
extern f32 lbl_eu_80573A20[4];   // .data (declared >8B to force lis/addi)

// 3-word enum-list filter table (.rodata) fed to startEnumObjects (target 5).
extern const u32 lbl_eu_804FC810[3];

// Label string fed to the voice-owner slot 0x1AC (.rodata).
extern char lbl_eu_804FC81C[];

// Global collision-probe object pointer (.sbss; monolib coli unit).

// nw4r db Warning() file/format strings (.rodata; also used by CtrlNpc.cpp).
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];

// In-TU action entry points matched to retail (C linkage keeps the plain
// call reloc names).
extern "C" void updateActMain(cf::CtrlAct* self);
extern "C" void tickTargetAct(cf::CtrlAct* self, ml::CVec3* pos, CtrlActSrc* src, int flag);
