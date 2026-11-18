// cf::CfObjectObj - reconstructed functions for kyoshin/cf/object/CfObjectObj
// Functions are kept in retail .text order so hexdiff's offset-based symbol
// fallback maps the unmangled retail names (ObjObj_ReleaseModels_FAB0, ObjObj_SetMoveId_FBF4) onto the mangled decomp ones.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectObj.hpp"
#include "monolib/util/MemManager.hpp"
#include "kyoshin/cf/CfResObjImpl.hpp"

// Retail .rodata owned by this TU (splits: 804FC5B8-804FC5EC): the RTTI
// class-name strings plus the trailing u16 table. Nothing in this TU's
// .text reads them (novtable scheme); they are referenced cross-unit.
// The 7 pad bytes after the first string are retail section bytes.
__declspec(section ".rodata") __attribute__((used))
const char lbl_eu_804FC5B8[0x18] = "cf::CfObjectTbox";
__declspec(section ".rodata") __attribute__((used))
const char lbl_eu_804FC5D0[0x10] = "cf::CfObjectObj";
__declspec(section ".rodata") __attribute__((used))
const u16 lbl_eu_804FC5E0[6] = { 0x14, 0x0A, 0x00, 0x0A, 0x14, 0x00 };

// Local overlay: base CfObjectModel keeps mSubObjB0 as void*; this TU owns a
// CfResObjImpl at +0xB0 (same scheme as CfObjectPoint typing mSubObj38).
struct ObjB0View {
    u8 pad[0xB0];
    cf::CfResObjImpl* mSubObjB0;
};

// CfObjectPc.hpp (read-only) currently fails to compile due to an internal
// virtual-override mismatch (CActorParam_getCurrencyBalance return type); only
// the incomplete type is needed here (getCfObjectPc pointer arithmetic).
namespace cf {
class CfObjectPc;
}

// us-800c03d4  - constructor: the CfObjectMove base ctor runs first; novtable
// (header) suppresses the implicit vptr store, so the retail vtable label is
// assigned explicitly. Zero the helper field, then guard the CfResObjImpl
// resource allocation behind a __dynamic_cast check (the cast succeeds only
// when the object already carries the resource impl, in which case the
// 0x1C-byte allocation is skipped).
cf::CfObjectObj::CfObjectObj() {
    *(void**)this = (void*)lbl_eu_80529B4C;
    field_71C = 0;
    if (__dynamic_cast(this, 0, &lbl_eu_80661D18, &lbl_eu_80661D20, 0) == 0) {
        // The ctor returns the object in r3, so assigning it back keeps `res`
        // in volatile r3 for the mSubObjB0 store (no callee-saved slot).
        cf::CfResObjImpl* res =
            (cf::CfResObjImpl*)mtl::MemManager::allocate(0x1c, CfRes_getAllocHandle());
        if (res != 0) {
            res = __ct__cf_CfResObjImpl(res, this);
        }
        ((ObjB0View*)this)->mSubObjB0 = res;
    }
}

// us-800c045c  - virtual destructor: restore the retail vtable (novtable),
// run the slot +0x68 virtual cleanup; MWCC appends the CfObjectMove base
// dtor call (flag 0) and the delete-flag wrapper.
cf::CfObjectObj::~CfObjectObj() {
    *(void**)this = (void*)lbl_eu_80529B4C;
    CfObject_releaseMoveTargets();
}

// us-800c04d0  - simple bool/int return after a virtual init call.
int cf::CfObjectObj::initialize() {
    // Qualified call forces direct (non-virtual) dispatch to the CfObjectModel impl.
    this->CfObjectModel::CfObject_UnkVirtualFunc2();
    return 1;
}

// us-800c04f4  - retail is a 4-byte blr stub.
void cf::CfObjectObj::reset() {}

// us-800c04f8  - init helper: two vtable init calls, clears two flags,
// dispatches two helper-ids through the 0x144 virtual, then sets bit-derived
// flags on arg4/arg5. Returns 1.
extern "C" int ObjObj_ReleaseModels_FAB0(cf::CfObjectObj* ths, u32 arg4, u32 arg5) {
    // 0x17c then 0x178 virtual init calls on the model.
    ths->CfObjectModel_releaseModelList();
    ths->CfObjectModel_releaseModelSub();
    // Clear helper-id / dispatch flags (0x90, 0x94 live in locked base padding).
    cf::CfObjectMove90View* flags90 = (cf::CfObjectMove90View*)ths;
    flags90->field_90 = 0;
    flags90->field_94 = 0;
    // vtable+0x144 helper dispatch (CfObject_setAnimSlotEntry, retail
    // 2-arg slot); genuine virtual dispatch reproduces the r12
    // double-load with args-then-dispatch ordering.
    ths->CfObject_setAnimSlotEntry(1, arg4);
    ths->CfObject_setAnimSlotEntry(0, arg5);
    // Mark the sub-object active (+0xB0 is CfResObjImpl*; field_08 is u16).
    ((ObjB0View*)ths)->mSubObjB0->field_08 = 1;
    cf::CfObjectMoveFlags6C* flags6C = (cf::CfObjectMoveFlags6C*)ths;
    if (arg4 != 0)
        flags6C->field_6C |= 0x20;
    if (arg5 != 0)
        flags6C->field_6C |= 0x10;
    return 1;
}

// us-800c05d8  - run the model update, then consume a pending helper id.
void cf::CfObjectObj::update() {
    // Qualified call forces direct (non-virtual) dispatch to CfObjectMove's
    // member impl (retail uses a plain bl, not a vtable dispatch).
    this->CfObjectMove::CfObject_UnkVirtualFunc4();
    if (this->CfObject_isMoveActiveNow() != 0 && this->field_71C != 0) {
        ObjImplSyncSlotState(this->mSubObj38, this->field_71C);
        this->field_71C = 0;
    }
}

// us-800c063c  - dispatch a helper id, or store it if not dispatchable.
// Retail branches straight to the epilogue when mSubObj38 is null (no store
// then); the id is stored only when mSubObj38 != 0 but the virtual check fails.
extern "C" void ObjObj_SetMoveId_FBF4(cf::CfObjectObj* ths, u16 id) {
    if (ths->mSubObj38 != 0) {
        if (ths->CfObject_isMoveActiveNow() != 0) {
            ObjImplSyncSlotState(ths->mSubObj38, id);
            ths->field_71C = 0;
        } else {
            ths->field_71C = id;
        }
    }
}

// us-800c06b0  - find the containing CfObjectPc for a CfObjectMove subobject.
// The CfObjectMove subobject lives at +0x3E9C inside CfObjectPc, so the reverse
// offset recovers the outer object. Only valid when the flag is set.
cf::CfObjectPc* getCfObjectPc(cf::CfObjectMove* objMove) {
    if (objMove != 0 && (objMove->unk64 & 0x2) != 0) {
        // Dead null-guard: return objMove as a raw address (never taken here) so
        // MWCC emits `beqlr cr1` without any class-base adjustment.
        if (objMove != 0)
            return (cf::CfObjectPc*)((u8*)objMove - 0x3E9C);
        return (cf::CfObjectPc*)((u8*)objMove);
    }
    return 0;
}

// Hand-assembled vtable data for this unit (the class is still
// __declspec(novtable)); retail keeps it in this TU's .data.
extern "C" {
u32 __RTTI__Q22cf12CObjectState[];
u32 __RTTI__Q22cf8CfObject[];
u32 __RTTI__Q22cf12CObjectParam[];
u32 __RTTI__Q22cf13CfObjectModel[];
u32 __RTTI__Q22cf12CfObjectMove[];
void CObjectState_setStateBitMask__Q22cf12CObjectStateFUl();
void CObjectState_checkStateFlags__Q22cf12CObjectStateFi();
void CObjectState_setStateBitFlag__Q22cf12CObjectStateFUl();
void CObjectState_clearStateWord4__Q22cf12CObjectStateFv();
void CObjectState_applyStateFlags__Q22cf12CObjectStateFi();
void CObjectState_clearStateWord8__Q22cf12CObjectStateFv();
void CObjectState_clearStateFlags8__Q22cf12CObjectStateFi();
void CObjectState_checkStateFlags8__Q22cf12CObjectStateFi();
void CObjectState_checkStateFlagsC__Q22cf12CObjectStateFi();
void CObjectState_setStateBitMask0__Q22cf12CObjectStateFii();
void CObjectState_getStateData__Q22cf12CObjectStateFv();
void CObjectState_setStateBitMask2__Q22cf12CObjectStateFv();
void CObjectState_setStateBitMask3__Q22cf12CObjectStateFv();
void CObjectParam_setObjectName__Q22cf12CObjectParamFPCc();
void CObjectParam_getParamPtr__Q22cf12CfObjectMoveFv();
void CObjectParam_hasObjectName__Q22cf12CObjectParamFv();
void reset__Q22cf11CfObjectObjFv();
void CObjectParam_getSelfObjectId__Q22cf12CObjectParamFv();
void CObjectParam_signalActionEnd__Q22cf12CObjectParamFi();
void __dt__Q22cf11CfObjectObjFv();
void initialize__Q22cf11CfObjectObjFv();
void CfObject_UnkVirtualFunc3__Q22cf8CfObjectFv();
void update__Q22cf11CfObjectObjFv();
void CfObject_updateMoveRate__Q22cf12CfObjectMoveFv();
void CfObject_releaseMoveTargets__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc7__Q22cf12CfObjectMoveFv();
void CfObject_notifyEventDone__Q22cf13CfObjectModelFv();
void CfObject_isMoveActiveNow__Q22cf12CfObjectMoveFv();
void CfObject_forwardSubObject__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc11__Q22cf8CfObjectFv();
void CfObject_refreshSubB0__Q22cf12CfObjectMoveFv();
void CfObject_queryTargetState__Q22cf12CfObjectMoveFv();
void CfObject_pushRefreshValue__Q22cf12CfObjectMoveFf();
void CfObject_getMoveSpeedRate__Q22cf12CfObjectMoveFv();
void CfObject_pushRefreshExtra__Q22cf12CfObjectMoveFf();
void CfObject_readRefreshValue__Q22cf12CfObjectMoveFv();
void CfObject_checkTargetState__Q22cf12CfObjectMoveFv();
void CfObject_setMoveTargetVec__Q22cf12CfObjectMoveFPCQ22ml5CVec3();
void CfObject_setPosXZ__Q22cf13CfObjectModelFff();
void CfObject_UnkVirtualFunc21__Q22cf8CfObjectFv();
void CfObject_syncMoveTarget__Q22cf12CfObjectMoveFPCQ22ml5CVec3();
void CfObject_getPosVector__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc24__Q22cf13CfObjectModelFv();
void CfObject_snapMoveTarget__Q22cf12CfObjectMoveFPQ22ml5CVec3f();
void CfObject_applyMoveOffset__Q22cf12CfObjectMoveFPCQ22ml5CVec3f();
void CfObject_setRotVec__Q22cf12CfObjectMoveFPv();
void CfObject_UnkVirtualFunc28__Q22cf13CfObjectModelFv();
void CfObject_setMoveHeadAngle__Q22cf12CfObjectMoveFf();
void CfObject_setMoveYaw__Q22cf12CfObjectMoveFf();
void CfObject_getMoveHeadAngle__Q22cf13CfObjectModelFv();
void CfObject_syncMoveHead__Q22cf12CfObjectMoveFf();
void CfObject_applyMoveYaw__Q22cf12CfObjectMoveFf();
void CfObject_UnkVirtualFunc34__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc35__Q22cf13CfObjectModelFf();
void CfObject_UnkVirtualFunc36__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc37__Q22cf12CfObjectMoveFUl();
void CfObject_UnkVirtualFunc38__Q22cf12CfObjectMoveFUl();
void CfObject_armAnimSlot0__Q22cf12CfObjectMoveFUlUl();
void CfObject_armAnimSlot0N__Q22cf12CfObjectMoveFUlUl();
void CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv();
void CfObject_armAnimSlot1__Q22cf12CfObjectMoveFUlUl();
void CfObject_armAnimSlot1N__Q22cf12CfObjectMoveFUlUl();
void CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv();
void CfObject_rearmAnimSlot__Q22cf12CfObjectMoveFUlUl();
void CfObject_setMoveTargetPtr__Q22cf12CfObjectMoveFPv();
void CfObject_createMoveTarget__Q22cf12CfObjectMoveFi();
void CfObject_getCurrentTarget__Q22cf12CfObjectMoveFv();
void CfObject_forwardNpcAction__Q22cf12CfObjectMoveFi();
void CfObject_getNpcActionState__Q22cf12CfObjectMoveFv();
void CfObject_getNpcTargetAction__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc52__Q22cf13CfObjectModelCFPCc();
void CfObject_getIndexedNodeMatrix__Q22cf13CfObjectModelCFUl();
void CfObject_UnkVirtualFunc54__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc55__Q22cf13CfObjectModelCFi();
void CfObject_getHeightDiff__Q22cf13CfObjectModelCFv();
void CfObject_UnkVirtualFunc57__Q22cf12CfObjectMoveFf();
void CfObject_getMoveRateScale__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc59__Q22cf12CfObjectMoveFf();
void CfObject_UnkVirtualFunc60__Q22cf12CfObjectMoveFv();
void CfObject_setAnimSlotEntry__Q22cf12CfObjectMoveFUlUl();
void CfObject_UnkVirtualFunc62__Q22cf12CfObjectMoveFUl();
void CfObject_readKindFlagWord__Q22cf13CfObjectModelFv();
void CfObject_setMoveBusyState__Q22cf12CfObjectMoveFi();
void CfObject_setMoveReadyFlag__Q22cf12CfObjectMoveFi();
void setPointEnabled__Q22cf12CfObjectMoveFi();
void CfObject_UnkVirtualFunc67__Q22cf13CfObjectModelFi();
void CfObject_UnkVirtualFunc68__Q22cf13CfObjectModelFPCQ22ml5CVec3();
void CfObject_UnkVirtualFunc69__Q22cf13CfObjectModelFv();
void CfObject_syncModelRate__Q22cf12CfObjectMoveFf();
void CfObject_getProgressLow__Q22cf13CfObjectModelFv();
void CfObject_setProgressHigh__Q22cf13CfObjectModelFf();
void CfObject_getProgressHigh__Q22cf13CfObjectModelFv();
void CfObjectModel_releaseModelSub__Q22cf12CfObjectMoveFv();
void CfObjectModel_releaseModelList__Q22cf12CfObjectMoveFv();
void CfObjectModel_getModelName__Q22cf13CfObjectModelFv();
void CfObjectModel_getAnimState__Q22cf12CfObjectMoveFv();
void CfObjectModel_getAnimFlags__Q22cf12CfObjectMoveFv();
void CfObjectModel_checkTargetNode__Q22cf12CfObjectMoveFPv();
void CfObjectModel_snapSubTarget__Q22cf13CfObjectModelFPQ22ml5CVec3f();
void CfObjectModel_getSubFlag04__Q22cf13CfObjectModelFv();
void CfObjectModel_getModelFloatA8__Q22cf13CfObjectModelFv();
void CfObjectModel_setModelFloatA8__Q22cf13CfObjectModelFf();
void CfObjectModel_getModelRate__Q22cf13CfObjectModelFv();
void CfObjectModel_setModelRate__Q22cf13CfObjectModelFf();
void CfObjectModel_aimAtPosition__Q22cf13CfObjectModelFPCQ22ml5CVec3();
void CfObjectModel_bindModelTo__Q22cf13CfObjectModelFPQ22cf8CfObjectPCc();
void CfObjectModel_snapToBone__Q22cf13CfObjectModelFPQ22cf8CfObjectPCc();
void CfObjectModel_setBoneRotate__Q22cf13CfObjectModelFi();
void CfObjectModel_getBindSource__Q22cf13CfObjectModelFv();
void CfObjectModel_detachEffectSlot__Q22cf12CfObjectMoveFPv();
void CfObjectModel_setModelVisible__Q22cf12CfObjectMoveFi();
void CfObjectModel_getModelVisible__Q22cf13CfObjectModelFv();
void CfObjectMove_dropMoveRequest__Q22cf12CfObjectMoveFv();
void CfObjectMove_detachModelList__Q22cf12CfObjectMoveFv();
void CfObjectMove_releaseSlotById__Q22cf12CfObjectMoveFi();
void CfObjectMove_recordMoveValue__Q22cf12CfObjectMoveFf();
void CfObjectMove_getMovementRate__Q22cf12CfObjectMoveFv();
void CfObjectMove_setTargetC4Arg__Q22cf12CfObjectMoveFUl();
void CfObjectMove_refreshNpcState__Q22cf12CfObjectMoveFiiiPCQ22ml5CVec3d();
void CfObjectMove_updateNpcTarget__Q22cf12CfObjectMoveFPCQ22ml5CVec3fii();
void CfObjectMove_queryNpcAdvance__Q22cf12CfObjectMoveFv();
void CfObjectMove_setNpcMoveRange__Q22cf12CfObjectMoveFf();
void CfObjectMove_writeTargetField__Q22cf12CfObjectMoveFi();
void CfObjectMove_advanceNpcTarget__Q22cf12CfObjectMoveFPCQ22ml5CVec3();
void CfObjectMove_commandNpcTarget__Q22cf12CfObjectMoveFi();
void CfObjectMove_restartNpcTarget__Q22cf12CfObjectMoveFiii();
void CfObjectMove_hasActorScale__Q22cf12CfObjectMoveFv();
void CfObjectMove_attachEffectSlot__Q22cf12CfObjectMoveFUlUlUlUlUl();
void CfObjectMove_setEffectSlotBit__Q22cf12CfObjectMoveFUl();
void CfObjectMove_transferSlotBits__Q22cf12CfObjectMoveFUl();
void CfObjectMove_isEffectSlotUsed__Q22cf12CfObjectMoveFUl();
void CfObjectMove_signalEffectSlot__Q22cf12CfObjectMoveFUl();
void CfObjectMove_flushEffectSlots__Q22cf12CfObjectMoveFv();
void CfObjectMove_setAllEffectBits__Q22cf12CfObjectMoveFv();
void CfObjectMove_loadResourceById__Q22cf12CfObjectMoveFUl();
}

void* lbl_eu_80529B18[] = {
    (void*)&__RTTI__Q22cf12CObjectState, 0, (void*)&__RTTI__Q22cf12CObjectParam, 0,
    (void*)&__RTTI__Q22cf8CfObject, 0, (void*)&__RTTI__Q22cf13CfObjectModel, 0,
    (void*)&__RTTI__Q22cf12CfObjectMove, 0, (void*)&lbl_eu_80661D20, 0,
    0,
};
void* lbl_eu_80529B4C[] = {
    (void*)&lbl_eu_80661D20, 0, (void*)CObjectState_setStateBitMask__Q22cf12CObjectStateFUl, (void*)CObjectState_checkStateFlags__Q22cf12CObjectStateFi,
    (void*)CObjectState_setStateBitFlag__Q22cf12CObjectStateFUl, (void*)CObjectState_clearStateWord4__Q22cf12CObjectStateFv, (void*)CObjectState_applyStateFlags__Q22cf12CObjectStateFi, (void*)CObjectState_clearStateWord8__Q22cf12CObjectStateFv,
    (void*)CObjectState_clearStateFlags8__Q22cf12CObjectStateFi, (void*)CObjectState_checkStateFlags8__Q22cf12CObjectStateFi, (void*)CObjectState_checkStateFlagsC__Q22cf12CObjectStateFi, (void*)CObjectState_setStateBitMask0__Q22cf12CObjectStateFii,
    (void*)CObjectState_getStateData__Q22cf12CObjectStateFv, (void*)CObjectState_setStateBitMask2__Q22cf12CObjectStateFv, (void*)CObjectState_setStateBitMask3__Q22cf12CObjectStateFv, (void*)CObjectParam_setObjectName__Q22cf12CObjectParamFPCc,
    (void*)CObjectParam_getParamPtr__Q22cf12CfObjectMoveFv, (void*)CObjectParam_hasObjectName__Q22cf12CObjectParamFv, (void*)reset__Q22cf11CfObjectObjFv, (void*)CObjectParam_getSelfObjectId__Q22cf12CObjectParamFv,
    (void*)CObjectParam_signalActionEnd__Q22cf12CObjectParamFi, (void*)__dt__Q22cf11CfObjectObjFv, (void*)initialize__Q22cf11CfObjectObjFv, (void*)CfObject_UnkVirtualFunc3__Q22cf8CfObjectFv,
    (void*)update__Q22cf11CfObjectObjFv, (void*)CfObject_updateMoveRate__Q22cf12CfObjectMoveFv, (void*)CfObject_releaseMoveTargets__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc7__Q22cf12CfObjectMoveFv,
    (void*)CfObject_notifyEventDone__Q22cf13CfObjectModelFv, (void*)CfObject_isMoveActiveNow__Q22cf12CfObjectMoveFv, (void*)CfObject_forwardSubObject__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc11__Q22cf8CfObjectFv,
    (void*)CfObject_refreshSubB0__Q22cf12CfObjectMoveFv, (void*)CfObject_queryTargetState__Q22cf12CfObjectMoveFv, (void*)CfObject_pushRefreshValue__Q22cf12CfObjectMoveFf, (void*)CfObject_getMoveSpeedRate__Q22cf12CfObjectMoveFv,
    (void*)CfObject_pushRefreshExtra__Q22cf12CfObjectMoveFf, (void*)CfObject_readRefreshValue__Q22cf12CfObjectMoveFv, (void*)CfObject_checkTargetState__Q22cf12CfObjectMoveFv, (void*)CfObject_setMoveTargetVec__Q22cf12CfObjectMoveFPCQ22ml5CVec3,
    (void*)CfObject_setPosXZ__Q22cf13CfObjectModelFff, (void*)CfObject_UnkVirtualFunc21__Q22cf8CfObjectFv, (void*)CfObject_syncMoveTarget__Q22cf12CfObjectMoveFPCQ22ml5CVec3, (void*)CfObject_getPosVector__Q22cf12CfObjectMoveFv,
    (void*)CfObject_UnkVirtualFunc24__Q22cf13CfObjectModelFv, (void*)CfObject_snapMoveTarget__Q22cf12CfObjectMoveFPQ22ml5CVec3f, (void*)CfObject_applyMoveOffset__Q22cf12CfObjectMoveFPCQ22ml5CVec3f, (void*)CfObject_setRotVec__Q22cf12CfObjectMoveFPv,
    (void*)CfObject_UnkVirtualFunc28__Q22cf13CfObjectModelFv, (void*)CfObject_setMoveHeadAngle__Q22cf12CfObjectMoveFf, (void*)CfObject_setMoveYaw__Q22cf12CfObjectMoveFf, (void*)CfObject_getMoveHeadAngle__Q22cf13CfObjectModelFv,
    (void*)CfObject_syncMoveHead__Q22cf12CfObjectMoveFf, (void*)CfObject_applyMoveYaw__Q22cf12CfObjectMoveFf, (void*)CfObject_UnkVirtualFunc34__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc35__Q22cf13CfObjectModelFf,
    (void*)CfObject_UnkVirtualFunc36__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc37__Q22cf12CfObjectMoveFUl, (void*)CfObject_UnkVirtualFunc38__Q22cf12CfObjectMoveFUl, (void*)CfObject_armAnimSlot0__Q22cf12CfObjectMoveFUlUl,
    (void*)CfObject_armAnimSlot0N__Q22cf12CfObjectMoveFUlUl, (void*)CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv, (void*)CfObject_armAnimSlot1__Q22cf12CfObjectMoveFUlUl, (void*)CfObject_armAnimSlot1N__Q22cf12CfObjectMoveFUlUl,
    (void*)CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv, (void*)CfObject_rearmAnimSlot__Q22cf12CfObjectMoveFUlUl, (void*)CfObject_setMoveTargetPtr__Q22cf12CfObjectMoveFPv, (void*)CfObject_createMoveTarget__Q22cf12CfObjectMoveFi,
    (void*)CfObject_getCurrentTarget__Q22cf12CfObjectMoveFv, (void*)CfObject_forwardNpcAction__Q22cf12CfObjectMoveFi, (void*)CfObject_getNpcActionState__Q22cf12CfObjectMoveFv, (void*)CfObject_getNpcTargetAction__Q22cf12CfObjectMoveFv,
    (void*)CfObject_UnkVirtualFunc52__Q22cf13CfObjectModelCFPCc, (void*)CfObject_getIndexedNodeMatrix__Q22cf13CfObjectModelCFUl, (void*)CfObject_UnkVirtualFunc54__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc55__Q22cf13CfObjectModelCFi,
    (void*)CfObject_getHeightDiff__Q22cf13CfObjectModelCFv, (void*)CfObject_UnkVirtualFunc57__Q22cf12CfObjectMoveFf, (void*)CfObject_getMoveRateScale__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc59__Q22cf12CfObjectMoveFf,
    (void*)CfObject_UnkVirtualFunc60__Q22cf12CfObjectMoveFv, (void*)CfObject_setAnimSlotEntry__Q22cf12CfObjectMoveFUlUl, (void*)CfObject_UnkVirtualFunc62__Q22cf12CfObjectMoveFUl, (void*)CfObject_readKindFlagWord__Q22cf13CfObjectModelFv,
    (void*)CfObject_setMoveBusyState__Q22cf12CfObjectMoveFi, (void*)CfObject_setMoveReadyFlag__Q22cf12CfObjectMoveFi, (void*)setPointEnabled__Q22cf12CfObjectMoveFi, (void*)CfObject_UnkVirtualFunc67__Q22cf13CfObjectModelFi,
    (void*)CfObject_UnkVirtualFunc68__Q22cf13CfObjectModelFPCQ22ml5CVec3, (void*)CfObject_UnkVirtualFunc69__Q22cf13CfObjectModelFv, (void*)CfObject_syncModelRate__Q22cf12CfObjectMoveFf, (void*)CfObject_getProgressLow__Q22cf13CfObjectModelFv,
    (void*)CfObject_setProgressHigh__Q22cf13CfObjectModelFf, (void*)CfObject_getProgressHigh__Q22cf13CfObjectModelFv, (void*)CfObjectModel_releaseModelSub__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_releaseModelList__Q22cf12CfObjectMoveFv,
    (void*)CfObjectModel_getModelName__Q22cf13CfObjectModelFv, (void*)CfObjectModel_getAnimState__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_getAnimFlags__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_checkTargetNode__Q22cf12CfObjectMoveFPv,
    (void*)CfObjectModel_snapSubTarget__Q22cf13CfObjectModelFPQ22ml5CVec3f, (void*)CfObjectModel_getSubFlag04__Q22cf13CfObjectModelFv, (void*)CfObjectModel_getModelFloatA8__Q22cf13CfObjectModelFv, (void*)CfObjectModel_setModelFloatA8__Q22cf13CfObjectModelFf,
    (void*)CfObjectModel_getModelRate__Q22cf13CfObjectModelFv, (void*)CfObjectModel_setModelRate__Q22cf13CfObjectModelFf, (void*)CfObjectModel_aimAtPosition__Q22cf13CfObjectModelFPCQ22ml5CVec3, (void*)CfObjectModel_bindModelTo__Q22cf13CfObjectModelFPQ22cf8CfObjectPCc,
    (void*)CfObjectModel_snapToBone__Q22cf13CfObjectModelFPQ22cf8CfObjectPCc, (void*)CfObjectModel_setBoneRotate__Q22cf13CfObjectModelFi, (void*)CfObjectModel_getBindSource__Q22cf13CfObjectModelFv, (void*)CfObjectModel_detachEffectSlot__Q22cf12CfObjectMoveFPv,
    (void*)CfObjectModel_setModelVisible__Q22cf12CfObjectMoveFi, (void*)CfObjectModel_getModelVisible__Q22cf13CfObjectModelFv, (void*)CfObjectMove_dropMoveRequest__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_detachModelList__Q22cf12CfObjectMoveFv,
    (void*)CfObjectMove_releaseSlotById__Q22cf12CfObjectMoveFi, (void*)CfObjectMove_recordMoveValue__Q22cf12CfObjectMoveFf, (void*)CfObjectMove_getMovementRate__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_setTargetC4Arg__Q22cf12CfObjectMoveFUl,
    (void*)CfObjectMove_refreshNpcState__Q22cf12CfObjectMoveFiiiPCQ22ml5CVec3d, (void*)CfObjectMove_updateNpcTarget__Q22cf12CfObjectMoveFPCQ22ml5CVec3fii, (void*)CfObjectMove_queryNpcAdvance__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_setNpcMoveRange__Q22cf12CfObjectMoveFf,
    (void*)CfObjectMove_writeTargetField__Q22cf12CfObjectMoveFi, (void*)CfObjectMove_advanceNpcTarget__Q22cf12CfObjectMoveFPCQ22ml5CVec3, (void*)CfObjectMove_commandNpcTarget__Q22cf12CfObjectMoveFi, (void*)CfObjectMove_restartNpcTarget__Q22cf12CfObjectMoveFiii,
    (void*)CfObjectMove_hasActorScale__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_attachEffectSlot__Q22cf12CfObjectMoveFUlUlUlUlUl, (void*)CfObjectMove_setEffectSlotBit__Q22cf12CfObjectMoveFUl, (void*)CfObjectMove_transferSlotBits__Q22cf12CfObjectMoveFUl,
    (void*)CfObjectMove_isEffectSlotUsed__Q22cf12CfObjectMoveFUl, (void*)CfObjectMove_signalEffectSlot__Q22cf12CfObjectMoveFUl, (void*)CfObjectMove_flushEffectSlots__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_setAllEffectBits__Q22cf12CfObjectMoveFv,
    (void*)CfObjectMove_loadResourceById__Q22cf12CfObjectMoveFUl,
};
void* lbl_eu_80529D70[] = {
    (void*)&__RTTI__Q22cf12CObjectState, 0, (void*)&__RTTI__Q22cf12CObjectParam, 0,
    (void*)&__RTTI__Q22cf8CfObject, 0, (void*)&__RTTI__Q22cf13CfObjectModel, 0,
    (void*)&__RTTI__Q22cf12CfObjectMove, 0, 0, 0,
};
