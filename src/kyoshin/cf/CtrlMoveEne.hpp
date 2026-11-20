#pragma once
#include "kyoshin/cf/CfCamSymbols.hpp"
#include <types.h>
#include "kyoshin/cf/CtrlMoveBase.hpp"
#include <nw4r/math/math_triangular.h>
#include <monolib/math/CVec3.hpp>
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/object/CfObjectModel.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"
#include "kyoshin/cf/object/CfObjectEne.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/cf/CtrlMoveBase.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)

namespace cf {

// Target-state object behind CtrlMoveEne::mTargetC4: CfObject_checkTargetState
// reads bit 23 of the +0x270 state word.
struct CfObjectMoveC4Target {
    u8 _pad[0x270];         // 0x00-0x26F
    u32 field_0x270;        // 0x270
};

// One entry of the move-list array at +0x78 (0x14 bytes each): three floats,
// a flags halfword, two bytes and a scaled float.
struct CfMoveSubEntry {
    f32 field_0x0;                  // 0x00
    f32 field_0x4;                  // 0x04
    f32 field_0x8;                  // 0x08
    u16 field_0xC;                  // 0x0C flags
    u8 field_0xE;                   // 0x0E
    u8 field_0xF;                   // 0x0F
    f32 field_0x10;                 // 0x10
};


class CtrlEnemy;

class __declspec(novtable) CtrlMoveEne : public CCtrlMoveBase {
public:
    CtrlMoveEne(CtrlEnemy* owner);

    f32 field_0x4C;                    // 0x4C move-sub heading word / rate-flag word
    u32 field_0x50;                    // 0x50 bdat-derived state word
    u32 field_0x54;                    // 0x54 timer/state word
    u16 field_0x58;                    // 0x58 flags / entry count
    u16 field_0x5A;                    // 0x5A gate counter
    u32 field_0x5C;                    // 0x5C dispatch index / wander seed
    f32 field_0x60;                    // 0x60 timer / current speed
    f32 field_0x64;                    // 0x64 turn target / smoothed speed
    f32 field_0x68;                    // 0x68 turn current / latched arts speed
    f32 field_0x6C;                    // 0x6C turn target / wander timer word
    u16 field_0x70;                    // 0x70 flags
    u16 field_0x72;                    // 0x72 gate
    s16 field_0x74;                    // 0x74 timer
    s16 field_0x76;                    // 0x76 halt timer / party-probe counter
    CfMoveSubEntry mMoveEntries[8];    // 0x78-0x117 move-list entries
    u8 unk_118[0x12C - 0x118];         // 0x118-0x12B (never touched)
    void (CtrlMoveEne::*mMoveHook)();  // 0x12C-0x137 ptmf hook (CtrlMoveEne_DispatchMoveHook / ctor)
    ml::CVec3 mVec138;                  // 0x138 (ctor copies ml::CVec3::zero)
    ml::CVec3 mVec144;                  // 0x144 (ctor copies ml::CVec3::zero)
    ml::CVec3 mVec150;                  // 0x150 re-approach target point
    int field_0x15C;                    // 0x15C latched actor id (TryLatchTargetActor / ConsumeTargetResolve)
    f32 field_0x160;                    // 0x160 (ctor stores 10.0f; CtrlMoveEne_TickMoveControl reads)
    f32 field_0x164;                    // 0x164 (ctor zeroes)
    f32 field_0x168;                    // 0x168 (zeroed by CtrlMoveEne_ResetMoveState)
    f32 field_0x16C;                    // 0x16C derived fall height
    f32 field_0x170;                    // 0x170 (ctor stores 0.699999988079071f)
    f32 field_0x174;                    // 0x174 (ctor stores 0.699999988079071f)
    f32 field_0x178;                    // 0x178 (zeroed by CtrlMoveEne_ResetMoveState)
    u32 field_0x17C;                    // 0x17C flags word
    u32 field_0x180;                    // 0x180 flags word (ctor / CtrlMoveEne_InitMoveHook raise bit 0x800)
    u16 field_0x184;                    // 0x184 (zeroed by CtrlMoveEne_ResetMoveState)
    u16 field_0x186;                    // 0x186 (ctor zeroes)
    s16 field_0x188;                    // 0x188 (CheckKeepMoving countdown)
    u16 field_0x18A;                    // 0x18A (ctor stores mtRand(0x3C))
    u16 field_0x18C;                    // 0x18C (CtrlMoveEne_InitMoveHook stores 2)
    s16 field_0x18E;                    // 0x18E (CheckKeepMoving target flag)
    u16 field_0x190;                    // 0x190 (zeroed by CtrlMoveEne_ResetMoveState)
    u16 field_0x192;                    // 0x192 (zeroed by CtrlMoveEne_ResetMoveState)
    u16 field_0x194;                    // 0x194
    s16 field_0x196;                    // 0x196 (zeroed by CtrlMoveEne_ResetMoveState)
    s16 field_0x198;                    // 0x198 (CtrlMoveEne_TickMoveControl lha's and compares to 90)
    u8 unk_19A[2];                     // 0x19A-0x19B (never touched)
    f32 field_0x19C;                    // 0x19C (zeroed by CtrlMoveEne_ResetMoveState)

    // Retail installs these as pointer-to-member hooks: the 12-byte ptmf
    // records in sEneMoveHookTable hold each member's address, and CtrlMoveEne
    // copies them into its +0x12C move hook. Bodies in CtrlMoveEne.cpp.
    void RateZeroFlat(u32* out1, f32* out2);    // rate 0.0f
    void RateDivisorSlot0(u32* out1, f32* out2);
    void RateDivisorSlot12C(u32* out1, f32* out2);
    void RateDivisorSlot258(u32* out1, f32* out2);
    void RateDivisorSlot384(u32* out1, f32* out2);
    void RateUnitFlat(u32* out1, f32* out2);    // rate 1.0f
    void RateUnitSlot12C(u32* out1, f32* out2);
    void RateUnitSlot258(u32* out1, f32* out2);
    void RateUnitSlot384(u32* out1, f32* out2);
    void RateRandGate50Slot0(u32* out1, f32* out2);
    void RateRandGate46Slot12C(u32* out1, f32* out2);
    void RateRandGate3CSlot258(u32* out1, f32* out2);
    void RateRandGate32Slot384(u32* out1, f32* out2);
    void RateSelectDuel50(u32* out1, f32* out2);
    void RateSelectTriple5A3C(u32* out1, f32* out2);
};

// View of the battle-object sub at +0x3F60 (CtrlMoveEne_TickMoveControl toggles bit flags
// in its +0xC word and +0x4EC word).
struct CfObj3F60View {
    u8 _pad[0xC];                   // 0x00-0x0B
    u32 field_C;                    // 0x0C
    u8 _pad10[0x4EC - 0x10];        // 0x10-0x4EB
    u32 field_4EC;                  // 0x4EC
};

// CtrlMoveEne_UpdateBattleMove's extension of the +0x3F60 flag object: the +0x532 halfword.
struct CFunc8008B580F60 {
    u8 _pad[0x532];                 // 0x00-0x531
    s16 field_532;                  // 0x532
};



// The +0x04 object of the enemy battle object (CtrlMoveEne_UpdateBattleMove reads the first
// word of its +0x30 vtable-slot result (a CObjectState).
struct CFunc8008B580Word {
    u32 field_0;                        // 0x00
};

// +0x3E9C Move sub-object view (CObjectState / CfObject virtuals). Old
// CFunc8008A2C8Sub table deleted - call real UVFs via cast.
struct CFunc8008A2C8Sub {
    void* vptr;                                         // 0x00
};

// CtrlMoveEne_UpdateBattleAI's extension of the +0x3F60 flag object: +0x4F8 float and the
// +0x532 halfword.
struct CFunc8008A2C8F60 {
    u8 _pad[0xC];                                       // 0x00-0x0B
    u32 field_C;                                        // 0x0C
    u8 _pad10[0x4EC - 0x10];                            // 0x10-0x4EB
    u32 field_4EC;                                      // 0x4EC
    u8 _pad4F0[0x4F8 - 0x4F0];                          // 0x4F0-0x4F7
    f32 field_4F8;                                      // 0x4F8
    u8 _pad4FC[0x532 - 0x4FC];                          // 0x4FC-0x531
    s16 field_532;                                      // 0x532
};

// View of the enemy battle object passed to CtrlMoveEne_ResetMoveState (CfImplEneBattleObj
// layout): +0x3374 flags, +0x3E9C embedded sub-object, +0x3F60 the sub
// CtrlMoveEne_TickMoveControl toggles, +0x4550 flags. CtrlMoveEne_UpdateBattleMove's separate polymorphic
// view with the vtable slots +0x11C / +0x128 / +0x2BC and the +0x04 word.
struct CFunc8008B580Obj {
    // Former virtual mirror deleted. The main object is a CActorParam
    // (retail slots +0x11C addHp / +0x128 getHp / +0x2A4 getMoveRecord /
    // +0x2BC isBattleLocked, called via CActorParam*); the +0x3E9C
    // sub-object is CfObject-family (called via CfObject*). Data only;
    // never instantiated, so no vtable emitted.
    void* vtable;                       // 0x00
    CObjectState* field_04;             // 0x04 (CtrlMoveEne_UpdateBattleMove reads a word via its +0x30 vtable slot)
    u8 _pad08[0x3374 - 0x08];           // 0x08-0x3373
    u32 field_3374;                     // 0x3374 flags
    u8 _pad3378[0x3E9C - 0x3378];       // 0x3378-0x3E9B
    // 0x3E9C embedded sub-object, raw vtable-slot view (CtrlMoveEne_UpdateBattleAI calls
    // slots +0x0C/+0x8C/+0xAC/+0x110; CtrlMoveEne_UpdateBattleMove casts it to the +0x150
    // virtual-slot view).
    CFunc8008A2C8Sub mSub2;             // 0x3E9C embedded sub-object
    u8 _pad3EA0[0x3F60 - 0x3EA0];       // 0x3EA0-0x3F5F
    CfObj3F60View* field_3F60;          // 0x3F60 (CtrlMoveEne_TickMoveControl flag object)
    u8 _pad3F64[0x4550 - 0x3F64];       // 0x3F64-0x454F
    u32 field_4550;                     // 0x4550 flags
    u8 _pad4554[0x45C0 - 0x4554];       // 0x4554-0x45BF
    u16 field_45C0;                     // 0x45C0 (lookupCA0ById id)
    u16 field_45C4;                     // 0x45C4
    u16 field_45C6;                     // 0x45C6

};

// Plain (non-polymorphic) view of the enemy battle object used by the
// matched CtrlMoveEne_TickMoveControl / CtrlMoveEne_ResetMoveState / CtrlMoveEne_InitMoveHook and CtrlMoveEne_InitMoveList:
// keeping this struct free of virtuals preserves their compiled bytes.
struct CFunc8008D444Obj {
    u8 _pad[0x3374];                    // 0x00-0x3373
    u32 field_3374;                     // 0x3374 flags
    u8 _pad3378[0x3E9C - 0x3378];       // 0x3378-0x3E9B
    void* mSub;                           // 0x3E9C embedded sub-object vptr (called via CfObject*)
    u8 _pad3EA0[0x3F60 - 0x3EA0];       // 0x3EA0-0x3F5F
    CfObj3F60View* field_3F60;          // 0x3F60 (CtrlMoveEne_TickMoveControl flag object)
    u8 _pad3F64[0x4550 - 0x3F64];       // 0x3F64-0x454F
    u32 field_4550;                     // 0x4550 flags
};


// +0xC4 data word of the move sub-object (read by CtrlMoveEne_UpdateFleeWander /
// CtrlMoveEne_ScanGimmickTargets). Former virtual mirror deleted: all calls now go through
// CtrlMoveEneSub (inherited CfObject virtuals) or CfObject*.
struct CFunc8008F9ECSub {
    void* vtable;                       // 0x00
    u8 _pad04[0xC4 - 0x04];           // 0x04-0xC3
    void* field_C4;                   // 0xC4 target word (null-checked)
};

// +0x98 / +0xC4 data words of the move sub-object (read by CtrlMoveEne_UpdateMoveHeading /
// CtrlMoveEne_RunMoveStateMachine). Former virtual mirror deleted: all calls now go through
// CtrlMoveEneSub (inherited CfObject virtuals) or CfObject*.
struct CFunc8008EF04Sub {
    void* vtable;                       // 0x00
    u8 _pad04[0x98 - 0x04];           // 0x04-0x97
    void* field_98;                   // 0x98
    u8 _pad9C[0xC4 - 0x9C];           // 0x9C-0xC3
    void* field_C4;                   // 0xC4
};

// Object behind the move sub's +0x98 word (CtrlMoveEne_UpdateMoveHeading tests +0x7A4).
struct CFunc8008EF04Sub98 {
    u8 _pad[0x7A4];                 // 0x00-0x7A3
    u32 field_7A4;                  // 0x7A4 flags
};

// Movement sub-object behind CCtrlMoveData::field_0x28 (the same object
// CtrlMoveNpc.cpp calls through CfObject*). Its retail vtable carries the
// CfObject chain plus the CfObjectMove leg: this TU provides the +0x1D8 slot
// body (CfObjectMove_getMovementRate__Q22cf12CfObjectMoveFv, returning the
// +0x6E8 float), and call sites dereference the +0x138 / +0x1D8 results as
// floats. object/CfObjectMove.hpp cannot be included here (its 4-arg
// func_8004B9D4 clashes with this TU's retail 5-arg form, and its 5-arg
// CfObjectMove_setAnimModeArgs takes u8* while this TU passes through void*), so the Move
// leg is redeclared here with retail names and arity (Move1-4 copied from
// object/CfObjectMove.hpp; Move5 is float* per the +0x6E8 impl and the
// dereference/divide call sites). Never instantiated, so no vtable emitted.
class __declspec(novtable) CtrlMoveEneSub : public CfObjectModel {
public:
    virtual void CfObjectMove_dropMoveRequest();  //0x1C8
    virtual void CfObjectMove_detachModelList();  //0x1CC
    virtual void CfObjectMove_releaseSlotById(int arg);  //0x1D0
    virtual void CfObjectMove_recordMoveValue(float value);  //0x1D4
    virtual float* CfObjectMove_getMovementRate();  //0x1D8 (movement-rate query)
};


// init path stores u16s at 0x58/0x5A and words at 0x5C/0x60, while the


// Target object returned by the gimmick's +0x110 slot (CtrlMoveEne_ScanGimmickTargets marks it
// when the move data's +0x28 sub-object has a +0xC4 target and the distance is
// small): +0xB8 points at a move-data block, +0x1E0 receives the actor id and
// +0x200 is a flags word.
struct CFunc8009156CTargetB8Sub {
    u8 _pad[0xC4];                  // 0x00-0xC3
    void* field_C4;                 // 0xC4
};
struct CFunc8009156CTargetB8 {
    u8 _pad[0x28];                  // 0x00-0x27
    CFunc8009156CTargetB8Sub* field_28;  // 0x28
};
struct CFunc8009156CTarget {
    u8 _pad[0xB8];                  // 0x00-0xB7
    CFunc8009156CTargetB8* field_B8;  // 0xB8
    u8 _padBC[0x1E0 - 0xBC];        // 0xBC-0x1DF
    u32 field_1E0;                  // 0x1E0
    u8 _pad1E4[0x200 - 0x1E4];      // 0x1E4-0x1FF
    u32 field_200;                  // 0x200 flags
};

// +0x508 height word of the move sub's +0xC4 target object (CtrlMoveEne_ProbeApproachPoint
// gates the fall-gap span on it).
struct CFunc8008B9C0Target {
    u8 _pad[0x508];             // 0x00-0x507
    f32 field_0x508;            // 0x508
};

// Global coli-probe object behind sEneColiProbe (CtrlMoveEne_TrackMoveTarget passes it to
// ColiWalkSegment).
struct CFunc8008CDE8Probe {
    u8 _pad[0x60];              // 0x00-0x5F
    f32 field_0x60;             // 0x60
    f32 field_0x64;             // 0x64
    f32 field_0x68;             // 0x68
};

// Circular gimmick list returned by getReslistB48 (same layout as
// CfGimmickList): head node at +0x04 is the sentinel, nodes terminate when
// they wrap back to it.
struct CFunc8009156CNode {
    CFunc8009156CNode* next;        // 0x00
    u8 _pad04[0x08 - 0x04];         // 0x04-0x07
    CfObject* object;                 // 0x08
};
struct CFunc8009156CList {
    u8* field_00;                   // 0x00
    CFunc8009156CNode* head;        // 0x04
};

// ---------------------------------------------------------------------------
// CtrlMoveEne_InitBattleMove views
// ---------------------------------------------------------------------------

// +0x3E9C Move sub-object (CfObject::UVF29 at +0xC4). Old function table deleted.
struct CFunc8008E760Sub {
    void* vptr;                                         // 0x00
};

// Battle/actor view: +0x5B4 = CfObjectActor::UVF6 -> float. Old battle/actor tables
// deleted - cast to CfObjectActor*.
struct CFunc8008E760BattleObj {
    void* vptr;                                         // 0x00
    u8 _pad04[0x3E9C - 0x04];                           // 0x04-0x3E9B
    CFunc8008E760Sub mSub;                              // 0x3E9C
    u8 _pad3EA0[0x44D8 - 0x3EA0];                       // 0x3EA0-0x44D7
    f32 field_44D8;                                     // 0x44D8
};

// Battle object r30 (derived from the move-data +0x28 sub-object): the bdat
// columns at +0x3F14 / +0x3F28, the flag words and the halfwords at +0x45C0..
// +0x45C6.
struct CFunc8008E760Obj {
    u8 _pad[0x3374];                                    // 0x00-0x3373
    u32 field_3374;                                     // 0x3374
    u8 _pad3378[0x3E9C - 0x3378];                       // 0x3378-0x3E9B
    CFunc8008E760Sub mSub;                              // 0x3E9C
    u8 _pad3EA0[0x3F00 - 0x3EA0];                       // 0x3EA0-0x3EFF
    u32 field_3F00;                                     // 0x3F00
    u8 _pad3F04[0x3F14 - 0x3F04];                       // 0x3F04-0x3F13
    char field_3F14[0x14];                              // 0x3F14 (file-path string for getFP__FPCc)
    u16 field_3F28;                                     // 0x3F28 (bdat row id)
    u8 _pad3F2A[0x44D8 - 0x3F2A];                       // 0x3F2A-0x44D7
    f32 field_44D8;                                     // 0x44D8
    u8 _pad44DC[0x45C0 - 0x44DC];                       // 0x44DC-0x45BF
    u16 field_45C0;                                     // 0x45C0
    u16 field_45C4;                                     // 0x45C4
    u16 field_45C6;                                     // 0x45C6
};

// lookupCA0ById result (r28): the +0x8C float read by CtrlMoveEne_InitBattleMove.
struct CFunc8008E760B89 {
    u8 _pad[0x8C];                                      // 0x00-0x8B
    f32 field_8C;                                       // 0x8C
};

// CPartsChange_FindPartsElem result: the +0x24 mode byte CtrlMoveEne_InitBattleMove tests.
struct CFunc8008E760Elem {
    u8 _pad[0x24];                                      // 0x00-0x23
    u8 field_24;                                        // 0x24
};

// Stack block passed to InitRecord / CPartsChange_ProcessPartyInfo (CfPartyInfo layout:
// CtrlMoveEne_InitBattleMove clears its +0x2D byte when the +0x180 bit-0 flag is set).
struct CFunc8008E760PartyInfo {
    u32 field_00;                                       // 0x00
    u32 field_04;                                       // 0x04
    u32 field_08;                                       // 0x08
    s32 field_0C;                                       // 0x0C
    u32 field_10;                                       // 0x10
    s32 field_14;                                       // 0x14
    f32 field_18;                                       // 0x18
    f32 field_1C;                                       // 0x1C
    f32 field_20;                                       // 0x20
    u32 field_24;                                       // 0x24
    f32 field_28;                                       // 0x28
    u8 field_2C;                                        // 0x2C
    u8 field_2D;                                        // 0x2D
    u8 field_2E;                                        // 0x2E
};

// Move-data block view for the +0x74 halfword CtrlMoveEne_InitBattleMove ORs 0x2000 into.
struct CFunc8008E760Data {
    u8 _pad[0x74];                                      // 0x00-0x73
    u16 field_74;                                       // 0x74
};

// Move sub-object as CtrlMoveEne_StepMoveEntry calls it: vtable +0x0C (int query), +0x10
// (int arg), +0xAC (position getter) and +0xC4 (float angle); data words at
// +0x98 / +0xC4 / +0x63C.

struct CFunc80090DB4Sub {
    void* vtable;                       // 0x00
    u8 _pad04[0x98 - 0x04];                             // 0x04-0x97
    void* field_98;                                     // 0x98
    u8 _pad9C[0xC4 - 0x9C];                             // 0x9C-0xC3
    void* field_C4;                                     // 0xC4
    u8 _padC8[0x63C - 0xC8];                            // 0xC8-0x63B
    f32 field_63C;                                      // 0x63C
};

// Vtable of the +0x3E9C embedded sub-object as CtrlMoveEne_CheckKeepMoving calls it:
// slots +0x50 (int arg), +0x8C (float query), +0x9C (pointer arg), +0xAC
// (position getter), +0xC4 (float angle), +0x138 (float* divisor) and +0x150
// (int arg).

struct CFunc8008D51CSub {
    void* vtable;                       // 0x00
};

// One 0x18-byte slot of the +0x1928 array CtrlMoveEne_CheckKeepMoving zeroes (word at +0x14
// then the five floats).
struct CFunc8008D51CSlot {
    f32 f0;                                             // 0x00
    f32 f4;                                             // 0x04
    f32 f8;                                             // 0x08
    f32 fC;                                             // 0x0C
    f32 f10;                                            // 0x10
    u32 w14;                                            // 0x14
};
// CtrlMoveEne_CheckKeepMoving's view of the enemy battle object: the +0x1928 zero region,
// +0x3374 flags, the +0x3E9C sub-object, the +0x3F60 flag object and the
// +0x455A halfword.
struct CFunc8008D51CObj {
    u8 _pad[0x1928];                                    // 0x00-0x1927
    CFunc8008D51CSlot slots[8];                         // 0x1928-0x19E7
    u8 _pad19E8[0x3374 - 0x19E8];                       // 0x19E8-0x3373
    u32 field_3374;                                     // 0x3374
    u8 _pad3378[0x3E9C - 0x3378];                       // 0x3378-0x3E9B
    CFunc8008D51CSub mSub;                              // 0x3E9C
    u8 _pad3EA0[0x3F60 - 0x3EA0];                       // 0x3EA0-0x3F5F
    CfObj3F60View* field_3F60;                          // 0x3F60
    u8 _pad3F64[0x455A - 0x3F64];                       // 0x3F64-0x4559
    u16 field_455A;                                     // 0x455A
};

// Move sub-object as CtrlMoveEne_StepMoveList calls it: vtable +0x0C (int query), +0x10
// (int arg), +0x9C (pointer arg), +0xAC (position getter), +0xC4 (float
// angle) and +0xCC (heading query); data words at +0x98 / +0xC4 / +0x63C.

struct CFunc8008FE8CSub {
    void* vtable;                       // 0x00
    u8 _pad04[0x98 - 0x04];                             // 0x04-0x97
    void* field_98;                                     // 0x98
    u8 _pad9C[0xC4 - 0x9C];                             // 0x9C-0xC3
    void* field_C4;                                     // 0xC4
    u8 _padC8[0x63C - 0xC8];                            // 0xC8-0x63B
    f32 field_63C;                                      // 0x63C
};

// ---------------------------------------------------------------------------
// CtrlMoveEne_RunMoveStateMachine views
// ---------------------------------------------------------------------------

// Vtable of the +0x3E9C embedded sub-object as CtrlMoveEne_RunMoveStateMachine calls it:
// +0xAC (position getter) and +0x110 (target object).

struct CFunc80091864Sub {
    void* vtable;                       // 0x00
};

// Battle-object view: +0x5B4 = CfObjectActor::UVF6. Old actor table
// deleted - cast to CfObjectActor*.
struct CFunc80091864Actor {
    void* vptr;                                         // 0x00
    CObjectState* field_04;                             // 0x04
    u8 _pad08[0x3374 - 0x08];                           // 0x08-0x3373
    u32 field_3374;                                     // 0x3374 flags
    u8 _pad3378[0x3E9C - 0x3378];                       // 0x3378-0x3E9B
    CFunc80091864Sub mSub;                              // 0x3E9C
    u8 _pad3EA0[0x3F60 - 0x3EA0];                       // 0x3EA0-0x3F5F
    CfObj3F60View* field_3F60;                          // 0x3F60
    u8 _pad3F64[0x44D8 - 0x3F64];                       // 0x3F64-0x44D7
    f32 field_44D8;                                     // 0x44D8
    u8 _pad44DC[0x45C0 - 0x44DC];                       // 0x44DC-0x45BF
    u16 field_45C0;                                     // 0x45C0 (lookupCA0ById id)
    u16 field_45C4;                                     // 0x45C4
    u16 field_45C6;                                     // 0x45C6
};

// Object returned by the +0x110 slot: +0xE0 heading index, +0x200 flags.
struct CFunc80091864Target {
    u8 _pad[0x14];                                      // 0x00-0x13
    f32 field_14;                                       // 0x14
    u8 _pad18[0xE0 - 0x18];                             // 0x18-0xDF
    u32 field_E0;                                       // 0xE0
    u8 _padE4[0x200 - 0xE4];                            // 0xE4-0x1FF
    u32 field_200;                                      // 0x200 flags
};

// CtrlMoveEne_UpdateBattleAI helpers: the CBattleManager +0x1AA byte and the global
// settings +0x214 flag word.
struct CFunc8008A2C8BMan {
    u8 _pad[0x1AA];                                     // 0x00-0x1A9
    u8 field_1AA;                                       // 0x1AA
};
 // Proximity threshold table (retail .data floats indexed by the +0x17C
// mode bits in CtrlMoveEne_UpdateBattleAI).
extern const f32 lbl_eu_804FB9C8[];

} // namespace cf

// C-linkage imports/exports (retail symbol names - keep linkage/signatures
// verbatim): __ptmf_test is the compiler-rt PTMF predicate; moveBaseSyncVel is
// defined in CtrlMoveBase.cpp; CtrlMoveEne_TickMoveControl / CtrlMoveEne_CheckKeepMoving in this TU. The
// extern "C" keeps the call-site relocs at the unmangled retail names.
extern "C" long __ptmf_test(void* ptmf);
extern "C" void CtrlMoveEne_TickMoveControl(cf::CtrlMoveEne* self);
extern "C" int CtrlMoveEne_CheckKeepMoving(cf::CtrlMoveEne* self);
// Defined in CtrlMoveBase.cpp (maskMoveChildFlags); called by
// CtrlMoveEne_ResetMoveState on the move-data block / controller. The
// ctrlAct* helpers are declared in the owning kyoshin/cf/CtrlAct.hpp.
// Per-frame move helpers (CtrlMoveBase.cpp / CtrlMoveNpc.cpp): moveBaseUpdate
// computes the approach direction, setMoveFacing commits a velocity.
extern "C" int moveBaseUpdate(cf::CtrlMoveEne* self, ml::CVec3* out,
                             const ml::CVec3* src, int flagA, int flagB);
extern "C" void moveBaseChase(cf::CtrlMoveEne* self, ml::CVec3* out,
                              const ml::CVec3* src);
// Circular gimmick-object list accessor (CfGimmick.cpp) and battle-event
// helper (CfObjectImpl.cpp); testActorStatus queries an actor word (CfAct.cpp).
extern "C" cf::CFunc8009156CList* getReslistB48();
// Icon-display helper (CActParamAnim.cpp).
// bdat row-value reader (CfObjectEne.cpp): (obj, column, row) -> raw word.
// Move-data approach helper (CtrlMoveBase.cpp) and coli segment probe used by
// CtrlMoveEne_TrackMoveTarget (retail C-ABI names).
extern "C" int moveBaseRoute(cf::CtrlMoveEne* self, ml::CVec3* pos, int flag);
extern "C" int ColiWalkSegment(void* a, void* b, void* c, void* d, int e, int f,
                             void* g);
// Global coli probe object (retail .sbss pointer) and the probe-block buffer
// CtrlMoveEne_TrackMoveTarget passes to ColiWalkSegment.
// Its typed C-linkage declaration lives in the monolib coli owner header.
#include "monolib/coli/code_804B2FF0.hpp"
extern u8 sEneColiProbeBuffer[0x38];
// Collision-list height probe API used by CtrlMoveEne_ProbeApproachPoint (retail C-ABI names;
// same declarations as CtrlAct.hpp). ScnRes_VertRayForward_E398's canonical declaration
// lives in cfsys/CfObjectImplMove.hpp (visible via harness_catalog); its ABI
// there differs from this unit's 6-arg FP form, so call sites cast through
// BE398GroundProbe below instead of re-declaring (extern "C" overloading).
extern "C" int ScnRes_GetEntryCount_E4AC(void);
extern "C" void* ScnRes_GetEntryPtr_E50C(u32 index);
extern "C" void* ScnRes_GetEntryHead2_E520(int index);
extern "C" int ScnRes_EntryFlagThunk_E5A4(int a, int b);
// CtrlMoveBase.cpp helper CtrlMoveEne_ProbeApproachPoint re-probes with (retail C-ABI name).
// In-unit approach probe (retail CtrlMoveEne_ProbeApproachPoint): extern "C" keeps the
// call-site relocs in CtrlMoveEne_UpdateFleeWander / CtrlMoveEne_UpdatePursuit at the unmangled name.
extern "C" int CtrlMoveEne_ProbeApproachPoint(cf::CtrlMoveEne* self, ml::CVec3* out,
                             cf::CtrlMoveEne* other, f32 f1, f32 f2, int arg);
// CfGameManager member-count accessor: canonical static int initPadSubSystem()
// is declared in include/kyoshin/cf/CfGameManager.hpp.
// CtrlMoveEne_InitBattleMove call sites: battle-object probe helpers (code_800B06A4.cpp),
// parts-element lookup (CPartsChange.cpp), party-info state builder/dispatch
// (CPartsChange.cpp) and the file/bdat column readers (retail C-ABI names).
extern "C" void* lookupCA0BySelf(void* self);
extern "C" void* lookupCA0ById(u32 id);
extern "C" void* CPartsChange_ResolveLinkedObj(void);
extern "C" void* CPartsChange_GetActorTable(void);
extern "C" void* CPartsChange_FindPartsElem(void* a, void* b);
extern "C" void InitRecord(void* out, void* src, f32 a, int b, int c,
                               f32 d, f32 e);
extern "C" int CPartsChange_ProcessPartyInfo(void* a, void* b);
extern "C" int ScnRes_SegQueryForward_E348(void* a, void* b, int c, int d, int e);
extern "C" void aimMoveTarget(void* self, ml::CVec3* dst, const ml::CVec3* src,
                              int flag);
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];
// CtrlMoveEne_CheckKeepMoving call sites:
// refresh (retail C-ABI names; getAnimModelId is declared in CtrlNpc.hpp).
// setAnimPosVec is declared in include/kyoshin/cf/CfGameManager.hpp
// (returns void*); do not redeclare it here with a different return type.
extern "C" void CPartsChange_ClearElemTimerForObj(void* self);
// CtrlMoveEne_StepMoveList call site: direction-commit helper (CtrlMoveBase.cpp) and the
// C runtime rand() used for the turn jitter.
extern "C" void moveBaseLerp(void* self, ml::CVec3* vec);
extern "C" int rand(void);
// CtrlMoveEne_UpdateBattleAI call sites: battle-object helpers, the CfGameManager /
// CBattleManager singletons and the parts/actor lookups (retail names).
extern "C" int findObjectById__Fi(int id);
extern "C" void* MapFx_ResolveOwner(void* src);
extern "C" int isBattleStatusActive(void* self, int id);
extern "C" int CPartsChange_GetEnemySlotAt(void* self, int index);
// CfGameManager::getInstance is declared in include/kyoshin/cf/CfGameManager.hpp.
// NOTE: deliberately no extern "C" - matches the plain-C++-linkage form
// used by CPartsChange.hpp / CfGimmickObject.hpp / CtrlEnemy.hpp; an
// extern "C" duplicate here collides when those headers are visible.
void* getPlayer__Q22cf13CfGameManagerFi(int index);
#include "kyoshin/code_801F3BE0.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
extern "C" void unregisterBattleActor(void* self, void* obj);
// CtrlMoveEne_UpdateBattleAI call site: game-flag query (C++ linkage mangles to the retail
// isGlobalCamFlagSet__Fi).


// .sdata2 float constants compared/stored by the CtrlMoveEne_RateDivisorSlot0 family.
// Declared const so MWCC treats the pool loads as constants and hoists them
// to retail's early position (docs/MWCC_CASES.md SDA scheduling).

// Further sdata2 constants used by the ctor (CtrlMoveEne::CtrlMoveEne) and the
// CtrlMoveEne_TickMoveControl / CtrlMoveEne_InitMoveHook family.
// Heading-scale factor (CtrlMoveEne_UpdateBattleMove / CtrlMoveEne_UpdateMoveHeading / CtrlMoveEne_UpdateFleeWander scale
// the +0xCC heading index by it before CosFIdx/SinFIdx).
// Heading-difference cosine threshold (CtrlMoveEne_UpdateMoveHeading compares against it).
// CtrlMoveEne_ProbeApproachPoint scan constants: mtRand(201) walk scale, ground-probe depth,
// height band and the fall-gap thresholds (upper 0.5 / lower 1.5 scales).
extern const f32 lbl_eu_80661C30;   // 0.005 (mtRand(201) scale)
extern const f32 _lbl_eu_80666604;  // 10000 (init best gap)
extern const f32 _lbl_eu_80666608;  // CtrlMoveEne_InitBattleMove field_0x16C fallback (mode byte != 3)
// CtrlMoveEne_TrackMoveTarget turn-probe constants: random-walk scale (-1), pi/2 turn bias.
// CtrlMoveEne_UpdateFleeWander rand-scaled heading constants (angle scale, lerp offsets).
// CtrlMoveEne_ScanGimmickTargets gimmick-scan distance threshold.
// CtrlMoveEne_RunMoveStateMachine wander-scan constants: proximity radius scale and the
// facing-alignment cosine threshold.
// CtrlMoveEne_StepMoveList turn-range / close thresholds.
// CtrlMoveEne_UpdateBattleAI ground-probe / pursuit constants.
// CtrlMoveEne_InitBattleMove field_0x160/0x4 scale (mode 2), field_0x16C fallbacks for the
// bdat bytes 0xFC / 0xFB, and the field_0x16C squaring threshold.
// MWCC int->float conversion doubles (CtrlMoveEne_InitMoveHook): 80666620 is the signed
// 2^52+2^31 constant, 80666650 the plain 2^52 constant.
// CtrlMoveEne_CheckKeepMoving bdat-state pointer (retail .sdata word).
// bdat column-name table CtrlMoveEne_InitMoveHook passes to getEneBdatColumn (+0xA column).
extern const char lbl_eu_804FB9E8[];

// Ptmf hook constants: the ctor installs a null pointer-to-member (MWCC emits
// the compiler-runtime null PTMF reference itself) and then the hook at
// sEneMoveHookDefault; CtrlMoveEne_InitMoveHook installs the hooks at
// sEneMoveHookTable + 0x118 / 0x124 / 0x130.
extern void (cf::CtrlMoveEne::*const sEneMoveHookDefault)();
typedef void (cf::CtrlMoveEne::*CfMoveDispatchPtmf)(void* a, void* b);
struct CfMoveHookPtmfs {
    u8 _pad[0x118];                                  // 0x00-0x117
    void (cf::CtrlMoveEne::*hook118)();             // 0x118
    void (cf::CtrlMoveEne::*hook124)();             // 0x124
    void (cf::CtrlMoveEne::*hook130)();             // 0x130
};
// Full hook-table view (adds the +0x13C..+0x1B4 records used by
// CtrlMoveEne_InitMoveList / CtrlMoveEne_InitBattleMove / CtrlMoveEne_StepMoveEntry). Declared as the symbol's
// extern type so member access emits the retail addi + base-load ptmf copy.
struct CfMoveHookPtmfsAll {
    u8 _pad[0x118];                                  // 0x00-0x117
    void (cf::CtrlMoveEne::*hook118)();             // 0x118
    void (cf::CtrlMoveEne::*hook124)();             // 0x124
    void (cf::CtrlMoveEne::*hook130)();             // 0x130
    void (cf::CtrlMoveEne::*hook13C)();             // 0x13C
    void (cf::CtrlMoveEne::*hook148)();             // 0x148
    void (cf::CtrlMoveEne::*hook154)();             // 0x154
    void (cf::CtrlMoveEne::*hook160)();             // 0x160
    void (cf::CtrlMoveEne::*hook16C)();             // 0x16C
    void (cf::CtrlMoveEne::*hook178)();             // 0x178
    void (cf::CtrlMoveEne::*hook184)();             // 0x184
    void (cf::CtrlMoveEne::*hook190)();             // 0x190
    void (cf::CtrlMoveEne::*hook19C)();             // 0x19C
    void (cf::CtrlMoveEne::*hook1A8)();             // 0x1A8 (CtrlMoveEne_StepMoveEntry)
    void (cf::CtrlMoveEne::*hook1B4)();             // 0x1B4 (CtrlMoveEne_StepMoveEntry)
};
extern const CfMoveHookPtmfsAll sEneMoveHookTable;
// CtrlMoveEne_InitMoveList's extension of the hook table (+0x13C / +0x148 records).
struct CfMoveHookPtmfsE2D4 {
    u8 _pad[0x13C];                                  // 0x00-0x13B
    void (cf::CtrlMoveEne::*hook13C)();             // 0x13C
    void (cf::CtrlMoveEne::*hook148)();             // 0x148
};
// CtrlMoveEne_InitBattleMove's extension of the hook table (+0x154 / +0x160 / +0x16C
// records).
struct CfMoveHookPtmfsE760 {
    u8 _pad[0x154];                                  // 0x00-0x153
    void (cf::CtrlMoveEne::*hook154)();             // 0x154
    void (cf::CtrlMoveEne::*hook160)();             // 0x160
    void (cf::CtrlMoveEne::*hook16C)();             // 0x16C
};
// Ptmf hook constant installed by CtrlMoveEne_ScanGimmickTargets (3-word record at
// sEneGimmickScanHook).
extern void (cf::CtrlMoveEne::*const sEneGimmickScanHook)();
// Secondary vtable installed at +0x48 by the ctor.
extern const u8 lbl_eu_805279FC[];

// Imports for the ctor / CtrlMoveEne_TickMoveControl / CtrlMoveEne_InitMoveHook call sites (retail
// unmangled names). extern "C" keeps the call-site relocs unmangled - a plain
// C++ declaration makes MWCC append the __FP<params> mangling at the call
// site. maskMoveBaseFlags / writeMoveSlotFloat are defined in
// CtrlMoveBase.cpp; setSnapFlags in CActParamAnim.cpp; getEneBdatColumn in
// CfObjectEne.cpp; CtrlMoveEne_ResetMoveState / CtrlMoveEne_CheckActorState in this TU.
extern "C" void setSnapFlags(void* self, u32 a, u32 b, f32 value);
extern "C" void* setAnimPosVec(void* out, const ml::CVec3* src);
extern "C" void CtrlMoveEne_ResetMoveState(cf::CtrlMoveEne* self, cf::CFunc8008D444Obj* obj, int flag);
extern "C" void CtrlMoveEne_CheckActorState(cf::CtrlMoveEne* self, void* obj, int arg2, u32 arg3);
extern "C" void CtrlMoveEne_TrackMoveTarget(cf::CtrlMoveEne* self, ml::CVec3* out, f32 f1);
