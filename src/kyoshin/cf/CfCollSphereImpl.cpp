#include "types.h"
#include "kyoshin/cf/CfCollSphereImpl.hpp"
#include "kyoshin/cf/object/CfObjectColl.hpp"

// CfCollSphere_DebugDrawSimple: debug draw for sphere collision shape (single call).
// r3 (the impl's this) is unused.
extern "C" void CfCollSphere_DebugDrawSimple(void* /*unused*/, cf::CfObjectColl* shape) {
    u32 uval = static_cast<u32>(shape->field_0xB8);
    ml::CVec3* pos = shape->CfObject_getPosVector();
    renderSphere__Q22cf18CfDebugDrawManagerFv(pos, static_cast<float>(uval));
}

// CfCollSphere_DebugDrawXform: debug draw for sphere with extra transform params.
extern "C" void CfCollSphere_DebugDrawXform(void* /*unused*/, cf::CfObjectColl* shape, cf::CfObject* query, ml::CVec3* out) {
    u32 uval = static_cast<u32>(shape->field_0xB8);
    ml::CVec3* pos = shape->CfObject_getPosVector();
    VecMath_PushOutSphere(query, pos, static_cast<float>(uval), out);
}

// --- absorb: retail data sections (split1) ---
// .rodata 0x70
__declspec(section ".rodata") __attribute__((aligned(8), used)) const unsigned char __rodata_CfCollSphereImpl[0x70] = {
    0x63,0x66,0x3A,0x3A,0x43,0x66,0x4F,0x62,0x6A,0x65,0x63,0x74,0x43,0x6F,0x6C,0x6C,0x00,0x00,0x00,0x00,
    0x63,0x66,0x3A,0x3A,0x43,0x66,0x43,0x6F,0x6C,0x6C,0x43,0x61,0x70,0x73,0x75,0x6C,0x65,0x49,0x6D,0x70,0x6C,0x00,0x00,0x00,
    0x63,0x66,0x3A,0x3A,0x43,0x66,0x43,0x6F,0x6C,0x6C,0x43,0x79,0x6C,0x69,0x6E,0x64,0x65,0x72,0x49,0x6D,0x70,0x6C,0x00,
    0x00,
    0x63,0x66,0x3A,0x3A,0x43,0x66,0x43,0x6F,0x6C,0x6C,0x41,0x41,0x42,0x42,0x49,0x6D,0x70,0x6C,0x00,
    0x00,
    0x63,0x66,0x3A,0x3A,0x43,0x66,0x43,0x6F,0x6C,0x6C,0x53,0x70,0x68,0x65,0x72,0x65,0x49,0x6D,0x70,0x6C,0x00,0x00,0x00,0x00,
};

// .data 0x220 -- 113 pointer relocs, 23 immediates (all zero)
extern "C" {
extern char __RTTI__Q22cf12CfObjectColl[];
extern char __RTTI__Q22cf17CfCollCapsuleImpl[];
extern char __RTTI__Q22cf18CfCollCylinderImpl[];
extern char lbl_eu_80661C90[];
extern char __RTTI__Q22cf16CfCollSphereImpl[];
extern char __RTTI__Q22cf10CfCollImpl[];
extern char lbl_eu_80661CB0[];
extern char __RTTI__Q22cf8CfObject[];
extern char __RTTI__Q22cf12CObjectParam[];
extern char __RTTI__Q22cf12CObjectState[];
extern void CObjectState_setStateBitMask__Q22cf12CObjectStateFUl();
extern void CObjectState_checkStateFlags__Q22cf12CObjectStateFi();
extern void CObjectState_setStateBitFlag__Q22cf12CObjectStateFUl();
extern void CObjectState_clearStateWord4__Q22cf12CObjectStateFv();
extern void CObjectState_applyStateFlags__Q22cf12CObjectStateFi();
extern void CObjectState_clearStateWord8__Q22cf12CObjectStateFv();
extern void CObjectState_clearStateFlags8__Q22cf12CObjectStateFi();
extern void CObjectState_checkStateFlags8__Q22cf12CObjectStateFi();
extern void CObjectState_checkStateFlagsC__Q22cf12CObjectStateFi();
extern void CObjectState_setStateBitMask0__Q22cf12CObjectStateFii();
extern void CObjectState_getStateData__Q22cf12CObjectStateFv();
extern void CObjectState_setStateBitMask2__Q22cf12CObjectStateFv();
extern void CObjectState_setStateBitMask3__Q22cf12CObjectStateFv();
extern void CObjectParam_setObjectName__Q22cf12CObjectParamFPCc();
extern void CObjectParam_getParamPtr__Q22cf12CObjectParamFv();
extern void CObjectParam_hasObjectName__Q22cf12CObjectParamFv();
extern void loadPointData__Q22cf13CfObjectPointFv();
extern void CObjectParam_getSelfObjectId__Q22cf12CObjectParamFv();
extern void CObjectParam_signalActionEnd__Q22cf12CObjectParamFi();
extern void __dt__Q22cf12CfObjectCollFv();
extern void resetCollTimer__Q22cf12CfObjectCollFv();
extern void enableCollision__Q22cf12CfObjectCollFv();
extern void updatePointState__Q22cf13CfObjectPointFv();
extern void CfObject_updateMoveRate__Q22cf8CfObjectFv();
extern void releasePointLink__Q22cf13CfObjectPointFv();
extern void refreshCollLink__Q22cf12CfObjectCollFPCQ22ml5CVec3();
extern void setChildPoint__Q22cf13CfObjectPointFPQ22cf13CfObjectPoint();
extern void CfObject_isMoveActiveNow__Q22cf8CfObjectFv();
extern void CfObject_forwardSubObject__Q22cf8CfObjectFv();
extern void CfObject_notifyMemberRemoved__Q22cf8CfObjectFv();
extern void CfObject_refreshSubB0__Q22cf8CfObjectFv();
extern void CfObject_queryTargetState__Q22cf8CfObjectFv();
extern void CfObject_pushRefreshValue__Q22cf8CfObjectFf();
extern void CfObject_getMoveSpeedRate__Q22cf8CfObjectFv();
extern void CfObject_pushRefreshExtra__Q22cf8CfObjectFf();
extern void CfObject_readRefreshValue__Q22cf8CfObjectFv();
extern void CfObject_checkTargetState__Q22cf8CfObjectFv();
extern void func_80047814__Q22cf13CfObjectPointFPCQ22ml5CVec3();
extern void setPointPosition__Q22cf13CfObjectPointFv();
extern void CfObject_setPosXYZ__Q22cf8CfObjectFv();
extern void syncCollVectors__Q22cf12CfObjectCollFv();
extern void CfObject_getPosVector__Q22cf8CfObjectFv();
extern void CfObject_getPosTriple__Q22cf8CfObjectFv();
extern void CfObject_snapMoveTarget__Q22cf8CfObjectFPQ22ml5CVec3f();
extern void copyCollPosition__Q22cf12CfObjectCollFv();
extern void CfObject_setRotVec__Q22cf8CfObjectFPv();
extern void CfObject_getField48__Q22cf8CfObjectFv();
extern void CfObject_setMoveHeadAngle__Q22cf8CfObjectFf();
extern void CfObject_setMoveYaw__Q22cf8CfObjectFf();
extern void CfObject_getMoveHeadAngle__Q22cf8CfObjectFv();
extern void CfObject_syncMoveHead__Q22cf8CfObjectFf();
extern void CfObject_applyMoveYaw__Q22cf8CfObjectFf();
extern void CfObject_getScaledAngle__Q22cf8CfObjectFv();
extern void CfObject_setObjScale__Q22cf8CfObjectFf();
extern void CfObject_getObjScale__Q22cf8CfObjectFv();
extern void CfObject_resetAnimSlots__Q22cf8CfObjectFUl();
extern void CfObject_clearAnimEntry__Q22cf8CfObjectFUl();
extern void CfObject_armAnimSlot0__Q22cf8CfObjectFUlUl();
extern void CfObject_armAnimSlot0N__Q22cf8CfObjectFUlUl();
extern void CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv();
extern void CfObject_armAnimSlot1__Q22cf8CfObjectFUlUl();
extern void CfObject_armAnimSlot1N__Q22cf8CfObjectFUlUl();
extern void CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv();
extern void CfObject_rearmAnimSlot__Q22cf8CfObjectFUlUl();
extern void CfObject_setMoveTargetPtr__Q22cf8CfObjectFPv();
extern void CfObject_createMoveTarget__Q22cf8CfObjectFi();
extern void CfObject_getCurrentTarget__Q22cf8CfObjectFv();
extern void CfObject_forwardNpcAction__Q22cf8CfObjectFi();
extern void CfObject_getNpcActionState__Q22cf8CfObjectFv();
extern void CfObject_getNpcTargetAction__Q22cf8CfObjectFv();
extern void CfObject_findNodeMatrix__Q22cf8CfObjectCFPCc();
extern void CfObject_getIndexedNodeMatrix__Q22cf8CfObjectCFUl();
extern void CfObject_getSubPosWord__Q22cf8CfObjectFv();
extern void CfObject_getPosSample__Q22cf8CfObjectCFi();
extern void CfObject_getHeightDiff__Q22cf8CfObjectCFv();
extern void CfObject_setMoveValue__Q22cf8CfObjectFf();
extern void CfObject_getMoveRateScale__Q22cf8CfObjectFv();
extern void CfObject_setMoveScale__Q22cf8CfObjectFf();
extern void CfObject_getMoveFactor__Q22cf8CfObjectFv();
extern void CfObject_setAnimSlotEntry__Q22cf8CfObjectFUlUl();
extern void CfObject_getSlotBits__Q22cf8CfObjectFUl();
extern void CfObject_readKindFlagWord__Q22cf8CfObjectFv();
extern void CfObject_setMoveBusyState__Q22cf8CfObjectFi();
extern void CfObject_setMoveReadyFlag__Q22cf8CfObjectFv();
extern void setPointEnabled__Q22cf13CfObjectPointFi();
extern void CfObject_setVisibleFlag__Q22cf8CfObjectFi();
extern void isCollEnabled__Q22cf13CfObjectPointFv();
extern void CfObject_isVisibleFlag__Q22cf8CfObjectFv();
extern void CfObject_syncModelRate__Q22cf8CfObjectFf();
extern void CfObject_getProgressLow__Q22cf8CfObjectFv();
extern void CfObject_setProgressHigh__Q22cf8CfObjectFf();
extern void CfObject_getProgressHigh__Q22cf8CfObjectFv();
extern void CollCapsule_DrawDebugShape();
extern void collCapsuleQueryOffset();
extern void CollCylinder_DrawDebugShape();
extern void collCylinderQueryOffset();
extern void CollAABB_DrawDebugShape();
extern void CollAABB_RenderQuery();
}
__declspec(section ".data") __attribute__((aligned(8), used)) const void* __data_CfCollSphereImpl[136] = {
    &__RTTI__Q22cf12CfObjectColl, (void*)0,
    &CObjectState_setStateBitMask__Q22cf12CObjectStateFUl,
    &CObjectState_checkStateFlags__Q22cf12CObjectStateFi,
    &CObjectState_setStateBitFlag__Q22cf12CObjectStateFUl,
    &CObjectState_clearStateWord4__Q22cf12CObjectStateFv,
    &CObjectState_applyStateFlags__Q22cf12CObjectStateFi,
    &CObjectState_clearStateWord8__Q22cf12CObjectStateFv,
    &CObjectState_clearStateFlags8__Q22cf12CObjectStateFi,
    &CObjectState_checkStateFlags8__Q22cf12CObjectStateFi,
    &CObjectState_checkStateFlagsC__Q22cf12CObjectStateFi,
    &CObjectState_setStateBitMask0__Q22cf12CObjectStateFii,
    &CObjectState_getStateData__Q22cf12CObjectStateFv,
    &CObjectState_setStateBitMask2__Q22cf12CObjectStateFv,
    &CObjectState_setStateBitMask3__Q22cf12CObjectStateFv,
    &CObjectParam_setObjectName__Q22cf12CObjectParamFPCc,
    &CObjectParam_getParamPtr__Q22cf12CObjectParamFv,
    &CObjectParam_hasObjectName__Q22cf12CObjectParamFv,
    &loadPointData__Q22cf13CfObjectPointFv,
    &CObjectParam_getSelfObjectId__Q22cf12CObjectParamFv,
    &CObjectParam_signalActionEnd__Q22cf12CObjectParamFi,
    &__dt__Q22cf12CfObjectCollFv,
    &resetCollTimer__Q22cf12CfObjectCollFv,
    &enableCollision__Q22cf12CfObjectCollFv,
    &updatePointState__Q22cf13CfObjectPointFv,
    &CfObject_updateMoveRate__Q22cf8CfObjectFv,
    &releasePointLink__Q22cf13CfObjectPointFv,
    &refreshCollLink__Q22cf12CfObjectCollFPCQ22ml5CVec3,
    &setChildPoint__Q22cf13CfObjectPointFPQ22cf13CfObjectPoint,
    &CfObject_isMoveActiveNow__Q22cf8CfObjectFv,
    &CfObject_forwardSubObject__Q22cf8CfObjectFv,
    &CfObject_notifyMemberRemoved__Q22cf8CfObjectFv,
    &CfObject_refreshSubB0__Q22cf8CfObjectFv,
    &CfObject_queryTargetState__Q22cf8CfObjectFv,
    &CfObject_pushRefreshValue__Q22cf8CfObjectFf,
    &CfObject_getMoveSpeedRate__Q22cf8CfObjectFv,
    &CfObject_pushRefreshExtra__Q22cf8CfObjectFf,
    &CfObject_readRefreshValue__Q22cf8CfObjectFv,
    &CfObject_checkTargetState__Q22cf8CfObjectFv,
    &func_80047814__Q22cf13CfObjectPointFPCQ22ml5CVec3,
    &setPointPosition__Q22cf13CfObjectPointFv,
    &CfObject_setPosXYZ__Q22cf8CfObjectFv,
    &syncCollVectors__Q22cf12CfObjectCollFv,
    &CfObject_getPosVector__Q22cf8CfObjectFv,
    &CfObject_getPosTriple__Q22cf8CfObjectFv,
    &CfObject_snapMoveTarget__Q22cf8CfObjectFPQ22ml5CVec3f,
    &copyCollPosition__Q22cf12CfObjectCollFv,
    &CfObject_setRotVec__Q22cf8CfObjectFPv,
    &CfObject_getField48__Q22cf8CfObjectFv,
    &CfObject_setMoveHeadAngle__Q22cf8CfObjectFf,
    &CfObject_setMoveYaw__Q22cf8CfObjectFf,
    &CfObject_getMoveHeadAngle__Q22cf8CfObjectFv,
    &CfObject_syncMoveHead__Q22cf8CfObjectFf,
    &CfObject_applyMoveYaw__Q22cf8CfObjectFf,
    &CfObject_getScaledAngle__Q22cf8CfObjectFv,
    &CfObject_setObjScale__Q22cf8CfObjectFf,
    &CfObject_getObjScale__Q22cf8CfObjectFv,
    &CfObject_resetAnimSlots__Q22cf8CfObjectFUl,
    &CfObject_clearAnimEntry__Q22cf8CfObjectFUl,
    &CfObject_armAnimSlot0__Q22cf8CfObjectFUlUl,
    &CfObject_armAnimSlot0N__Q22cf8CfObjectFUlUl,
    &CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv,
    &CfObject_armAnimSlot1__Q22cf8CfObjectFUlUl,
    &CfObject_armAnimSlot1N__Q22cf8CfObjectFUlUl,
    &CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv,
    &CfObject_rearmAnimSlot__Q22cf8CfObjectFUlUl,
    &CfObject_setMoveTargetPtr__Q22cf8CfObjectFPv,
    &CfObject_createMoveTarget__Q22cf8CfObjectFi,
    &CfObject_getCurrentTarget__Q22cf8CfObjectFv,
    &CfObject_forwardNpcAction__Q22cf8CfObjectFi,
    &CfObject_getNpcActionState__Q22cf8CfObjectFv,
    &CfObject_getNpcTargetAction__Q22cf8CfObjectFv,
    &CfObject_findNodeMatrix__Q22cf8CfObjectCFPCc,
    &CfObject_getIndexedNodeMatrix__Q22cf8CfObjectCFUl,
    &CfObject_getSubPosWord__Q22cf8CfObjectFv,
    &CfObject_getPosSample__Q22cf8CfObjectCFi,
    &CfObject_getHeightDiff__Q22cf8CfObjectCFv,
    &CfObject_setMoveValue__Q22cf8CfObjectFf,
    &CfObject_getMoveRateScale__Q22cf8CfObjectFv,
    &CfObject_setMoveScale__Q22cf8CfObjectFf,
    &CfObject_getMoveFactor__Q22cf8CfObjectFv,
    &CfObject_setAnimSlotEntry__Q22cf8CfObjectFUlUl,
    &CfObject_getSlotBits__Q22cf8CfObjectFUl,
    &CfObject_readKindFlagWord__Q22cf8CfObjectFv,
    &CfObject_setMoveBusyState__Q22cf8CfObjectFi,
    &CfObject_setMoveReadyFlag__Q22cf8CfObjectFv,
    &setPointEnabled__Q22cf13CfObjectPointFi,
    &CfObject_setVisibleFlag__Q22cf8CfObjectFi,
    &isCollEnabled__Q22cf13CfObjectPointFv,
    &CfObject_isVisibleFlag__Q22cf8CfObjectFv,
    &CfObject_syncModelRate__Q22cf8CfObjectFf,
    &CfObject_getProgressLow__Q22cf8CfObjectFv,
    &CfObject_setProgressHigh__Q22cf8CfObjectFf,
    &CfObject_getProgressHigh__Q22cf8CfObjectFv,
    &__RTTI__Q22cf12CObjectState, (void*)0,
    &__RTTI__Q22cf12CObjectParam, (void*)0,
    &__RTTI__Q22cf8CfObject, (void*)0,
    &lbl_eu_80661CB0, (void*)0, (void*)0, (void*)0,
    &__RTTI__Q22cf17CfCollCapsuleImpl, (void*)0, &CollCapsule_DrawDebugShape, &collCapsuleQueryOffset,
    &__RTTI__Q22cf10CfCollImpl, (void*)0, (void*)0, (void*)0,
    &__RTTI__Q22cf18CfCollCylinderImpl, (void*)0, &CollCylinder_DrawDebugShape, &collCylinderQueryOffset,
    &__RTTI__Q22cf10CfCollImpl, (void*)0, (void*)0, (void*)0,
    &lbl_eu_80661C90, (void*)0, &CollAABB_DrawDebugShape, &CollAABB_RenderQuery,
    &__RTTI__Q22cf10CfCollImpl, (void*)0, (void*)0, (void*)0,
    &__RTTI__Q22cf16CfCollSphereImpl, (void*)0, &CfCollSphere_DebugDrawSimple, &CfCollSphere_DebugDrawXform,
    &__RTTI__Q22cf10CfCollImpl, (void*)0, (void*)0, (void*)0,
};

// .sdata 0x28 -- 10 pointer relocs
extern "C" {
extern char lbl_eu_804FC088[];
extern char lbl_eu_804FC09C[];
extern char lbl_eu_804FC0B4[];
extern char lbl_eu_804FC0CC[];
extern char lbl_eu_804FC0E0[];
extern char lbl_eu_80528778[];
extern char lbl_eu_805287B0[];
extern char lbl_eu_805287D0[];
extern char lbl_eu_805287F0[];
extern char lbl_eu_80528810[];
}
__declspec(section ".sdata") __attribute__((aligned(8), used)) const void* __sdata_CfCollSphereImpl[10] = {
    &lbl_eu_804FC088, &lbl_eu_80528778,
    &lbl_eu_804FC09C, &lbl_eu_805287B0,
    &lbl_eu_804FC0B4, &lbl_eu_805287D0,
    &lbl_eu_804FC0CC, &lbl_eu_805287F0,
    &lbl_eu_804FC0E0, &lbl_eu_80528810,
};
