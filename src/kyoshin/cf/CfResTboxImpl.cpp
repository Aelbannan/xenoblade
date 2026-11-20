// Translation unit for kyoshin/cf/CfResTboxImpl
// Resource textbox implementation.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CfResTboxImpl.hpp"
#include "kyoshin/cf/object/CfObjectObj.hpp"
#include "kyoshin/cf/IResInfo.hpp"
#include "monolib/util/MemManager.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"
#include "libs/monolib/src/scn/CScnItemModel.hpp"

// Retail .data/.sdata2/.sbss externs for the hand-built tables below.
// Free-function spellings name the exact retail (link) symbols; the
// .text of this TU never calls them (10/10 matched), they only feed
// ADDR32 relocs. CfGimmick_DetachManager__Q22cf9CfGimmickFPQ22cf13CfGameManager is the
// virtual cf::CfGimmick::CfGimmick_DetachManager (kyoshin/cf/CfGimmick.cpp).
extern "C" {
void __dt__Q22cf12CfObjectTboxFv(void*, int); // ABI deleting dtor
void __dt__Q22cf15CfGimmickObjectFv(void*, int); // ABI deleting dtor
void CObjectParam_setObjectName__Q22cf12CObjectParamFPCc(void);
void CObjectParam_getParamPtr__Q22cf12CfObjectMoveFv(void);
void CObjectParam_getSelfObjectId__Q22cf12CObjectParamFv(void);
void CObjectParam_hasObjectName__Q22cf12CObjectParamFv(void);
void CObjectParam_signalActionEnd__Q22cf12CObjectParamFi(void);
void CObjectState_checkStateFlags8__Q22cf12CObjectStateFi(void);
void CObjectState_checkStateFlagsC__Q22cf12CObjectStateFi(void);
void CObjectState_applyStateFlags__Q22cf12CObjectStateFi(void);
void CObjectState_checkStateFlags__Q22cf12CObjectStateFi(void);
void CObjectState_clearStateFlags8__Q22cf12CObjectStateFi(void);
void CObjectState_clearStateWord4__Q22cf12CObjectStateFv(void);
void CObjectState_clearStateWord8__Q22cf12CObjectStateFv(void);
void CObjectState_getStateData__Q22cf12CObjectStateFv(void);
void CObjectState_setStateBitFlag__Q22cf12CObjectStateFUl(void);
void CObjectState_setStateBitMask0__Q22cf12CObjectStateFii(void);
void CObjectState_setStateBitMask2__Q22cf12CObjectStateFv(void);
void CObjectState_setStateBitMask3__Q22cf12CObjectStateFv(void);
void CObjectState_setStateBitMask__Q22cf12CObjectStateFUl(void);
void CfGimmick_DetachManager__Q22cf9CfGimmickFPQ22cf13CfGameManager(void);
void CfObjectModel_setModelFloatA8__Q22cf13CfObjectModelFf(void);
void CfObjectModel_getModelRate__Q22cf13CfObjectModelFv(void);
void CfObjectModel_setModelRate__Q22cf13CfObjectModelFf(void);
void CfObjectModel_aimAtPosition__Q22cf13CfObjectModelFPCQ22ml5CVec3(void);
void CfObjectModel_bindModelTo__Q22cf13CfObjectModelFPQ22cf8CfObjectPCc(void);
void CfObjectModel_snapToBone__Q22cf13CfObjectModelFPQ22cf8CfObjectPCc(void);
void CfObjectModel_setBoneRotate__Q22cf13CfObjectModelFi(void);
void CfObjectModel_getBindSource__Q22cf13CfObjectModelFv(void);
void CfObjectModel_detachEffectSlot__Q22cf12CfObjectMoveFPv(void);
void CfObjectModel_setModelVisible__Q22cf12CfObjectMoveFi(void);
void CfObjectModel_releaseModelSub__Q22cf12CfObjectMoveFv(void);
void CfObjectModel_getModelVisible__Q22cf13CfObjectModelFv(void);
void CfObjectModel_releaseModelList__Q22cf12CfObjectMoveFv(void);
void CfObjectModel_getModelName__Q22cf13CfObjectModelFv(void);
void CfObjectModel_getAnimState__Q22cf12CfObjectMoveFv(void);
void CfObjectModel_getAnimFlags__Q22cf12CfObjectMoveFv(void);
void CfObjectModel_checkTargetNode__Q22cf12CfObjectMoveFPv(void);
void CfObjectModel_snapSubTarget__Q22cf13CfObjectModelFPQ22ml5CVec3f(void);
void CfObjectModel_getSubFlag04__Q22cf13CfObjectModelFv(void);
void CfObjectModel_getModelFloatA8__Q22cf13CfObjectModelFv(void);
void CfObjectMove_setNpcMoveRange__Q22cf12CfObjectMoveFf(void);
void CfObjectMove_hasActorScale__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_setTargetC4Arg__Q22cf12CfObjectMoveFUl(void);
void CfObjectMove_advanceNpcTarget__Q22cf12CfObjectMoveFPCQ22ml5CVec3(void);
void CfObjectMove_attachEffectSlot__Q22cf12CfObjectMoveFUlUlUlUlUl(void);
void CfObjectMove_commandNpcTarget__Q22cf12CfObjectMoveFi(void);
void CfObjectMove_detachModelList__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_dropMoveRequest__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_flushEffectSlots__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_getMovementRate__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_isEffectSlotUsed__Q22cf12CfObjectMoveFUl(void);
void CfObjectMove_loadResourceById__Q22cf12CfObjectMoveFUl(void);
void CfObjectMove_queryNpcAdvance__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_recordMoveValue__Q22cf12CfObjectMoveFf(void);
void CfObjectMove_refreshNpcState__Q22cf12CfObjectMoveFiiiPCQ22ml5CVec3d(void);
void CfObjectMove_releaseSlotById__Q22cf12CfObjectMoveFi(void);
void CfObjectMove_restartNpcTarget__Q22cf12CfObjectMoveFiii(void);
void CfObjectMove_setAllEffectBits__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_setEffectSlotBit__Q22cf12CfObjectMoveFUl(void);
void CfObjectMove_signalEffectSlot__Q22cf12CfObjectMoveFUl(void);
void CfObjectMove_transferSlotBits__Q22cf12CfObjectMoveFUl(void);
void CfObjectMove_updateNpcTarget__Q22cf12CfObjectMoveFPCQ22ml5CVec3fii(void);
void CfObjectMove_writeTargetField__Q22cf12CfObjectMoveFi(void);
void CfObject_notifyMemberRemoved__Q22cf8CfObjectFv(void);
void CfObject_refreshSubB0__Q22cf12CfObjectMoveFv(void);
void CfObject_setPosXZ__Q22cf13CfObjectModelFff(void);
void CfObject_setPosXYZ__Q22cf8CfObjectFv(void);
void CfObject_syncMoveTarget__Q22cf12CfObjectMoveFPCQ22ml5CVec3(void);
void CfObject_getPosTriple__Q22cf13CfObjectModelFv(void);
void CfObject_snapMoveTarget__Q22cf12CfObjectMoveFPQ22ml5CVec3f(void);
void CfObject_applyMoveOffset__Q22cf12CfObjectMoveFPCQ22ml5CVec3f(void);
void CfObject_setRotVec__Q22cf12CfObjectMoveFPv(void);
void CfObject_getField48__Q22cf13CfObjectModelFv(void);
void CfObject_setMoveYaw__Q22cf12CfObjectMoveFf(void);
void CfObject_syncMoveHead__Q22cf12CfObjectMoveFf(void);
void CfObject_applyMoveYaw__Q22cf12CfObjectMoveFf(void);
void CfObject_getScaledAngle__Q22cf13CfObjectModelFv(void);
void CfObject_setObjScale__Q22cf13CfObjectModelFf(void);
void CfObject_getObjScale__Q22cf13CfObjectModelFv(void);
void CfObject_resetAnimSlots__Q22cf12CfObjectMoveFUl(void);
void CfObject_clearAnimEntry__Q22cf12CfObjectMoveFUl(void);
void CfObject_armAnimSlot0__Q22cf12CfObjectMoveFUlUl(void);
void CfObject_UnkVirtualFunc3__Q22cf8CfObjectFv(void);
void CfObject_armAnimSlot0N__Q22cf12CfObjectMoveFUlUl(void);
void CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv(void);
void CfObject_armAnimSlot1__Q22cf12CfObjectMoveFUlUl(void);
void CfObject_armAnimSlot1N__Q22cf12CfObjectMoveFUlUl(void);
void CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv(void);
void CfObject_rearmAnimSlot__Q22cf12CfObjectMoveFUlUl(void);
void CfObject_getNpcActionState__Q22cf12CfObjectMoveFv(void);
void CfObject_getNpcTargetAction__Q22cf12CfObjectMoveFv(void);
void CfObject_findNodeMatrix__Q22cf13CfObjectModelCFPCc(void);
void CfObject_getIndexedNodeMatrix__Q22cf13CfObjectModelCFUl(void);
void CfObject_getSubPosWord__Q22cf13CfObjectModelFv(void);
void CfObject_getPosSample__Q22cf13CfObjectModelCFi(void);
void CfObject_getHeightDiff__Q22cf13CfObjectModelCFv(void);
void CfObject_setMoveValue__Q22cf12CfObjectMoveFf(void);
void CfObject_setMoveScale__Q22cf12CfObjectMoveFf(void);
void CfObject_updateMoveRate__Q22cf12CfObjectMoveFv(void);
void CfObject_getMoveFactor__Q22cf12CfObjectMoveFv(void);
void CfObject_getSlotBits__Q22cf12CfObjectMoveFUl(void);
void CfObject_readKindFlagWord__Q22cf13CfObjectModelFv(void);
void setPointEnabled__Q22cf12CfObjectMoveFi(void);
void CfObject_setVisibleFlag__Q22cf13CfObjectModelFi(void);
void CfObject_UnkVirtualFunc68__Q22cf13CfObjectModelFPCQ22ml5CVec3(void);
void CfObject_isVisibleFlag__Q22cf13CfObjectModelFv(void);
void CfObject_releaseMoveTargets__Q22cf12CfObjectMoveFv(void);
void CfObject_syncModelRate__Q22cf12CfObjectMoveFf(void);
void CfObject_getProgressLow__Q22cf13CfObjectModelFv(void);
void CfObject_setProgressHigh__Q22cf13CfObjectModelFf(void);
void CfObject_getProgressHigh__Q22cf13CfObjectModelFv(void);
void CfObject_UnkVirtualFunc7__Q22cf12CfObjectMoveFv(void);
void CfObject_checkTargetState__Q22cf12CfObjectMoveFv(void);
void CfObject_createMoveTarget__Q22cf12CfObjectMoveFi(void);
void CfObject_forwardNpcAction__Q22cf12CfObjectMoveFi(void);
void CfObject_forwardSubObject__Q22cf12CfObjectMoveFv(void);
void CfObject_getCurrentTarget__Q22cf12CfObjectMoveFv(void);
void CfObject_getMoveHeadAngle__Q22cf13CfObjectModelFv(void);
void CfObject_getMoveRateScale__Q22cf12CfObjectMoveFv(void);
void CfObject_getMoveSpeedRate__Q22cf12CfObjectMoveFv(void);
void CfObject_getPosVector__Q22cf12CfObjectMoveFv(void);
void CfObject_isMoveActiveNow__Q22cf12CfObjectMoveFv(void);
void CfObject_notifyEventDone__Q22cf13CfObjectModelFv(void);
void CfObject_pushRefreshExtra__Q22cf12CfObjectMoveFf(void);
void CfObject_pushRefreshValue__Q22cf12CfObjectMoveFf(void);
void CfObject_queryTargetState__Q22cf12CfObjectMoveFv(void);
void CfObject_readRefreshValue__Q22cf12CfObjectMoveFv(void);
void CfObject_setAnimSlotEntry__Q22cf12CfObjectMoveFUlUl(void);
void CfObject_setMoveBusyState__Q22cf12CfObjectMoveFi(void);
void CfObject_setMoveHeadAngle__Q22cf12CfObjectMoveFf(void);
void CfObject_setMoveReadyFlag__Q22cf12CfObjectMoveFi(void);
void CfObject_setMoveTargetPtr__Q22cf12CfObjectMoveFPv(void);
void CfObject_setMoveTargetVec__Q22cf12CfObjectMoveFPCQ22ml5CVec3(void);
void GimObjClearStepBits(void);
void GimObjDriveStepMachine(void);
void GimObjFinishStep(void);

void initTbox__Q22cf12CfObjectTboxFv(void);
void initialize__Q22cf11CfObjectObjFv(void);
void reset__Q22cf11CfObjectObjFv(void);
}

namespace cf { class CfGimmickObject; }
void GimObjSetBusyBit(void* self);
void GimObjRebuildColliders(cf::CfGimmickObject* self);

extern char __RTTI__Q22cf15CfGimmickObject[]; // .sdata RTTI (defined by CPartyStateWin.o)
extern char __RTTI__Q22cf11IObjectInfo[];
extern char __RTTI__Q22cf9CfGimmick[];
extern char __RTTI__Q22cf12CfObjectTbox[];

using namespace cf;

// us-801faaf0 - constructor: base CfResObjImpl ctor (parent passed through in
// r4), then store the retail vtable label at +0x10 (manual-vtable scheme).
extern "C" cf::CfResTboxImpl* __ct__cf_CfResTboxImpl(CfResTboxImpl* ths, CfResObjParent* parent) {
    __ct__cf_CfResObjImpl((cf::CfResObjImpl*)ths, parent);
    ths->vtbl() = &lbl_eu_80535204;
    return ths;
}

extern "C" int ResTbox_GetFixedSize256() { return 256; }

// us-801fab34 - open test: dispatch the +0x14 vtable slot and negate the
// bool result (retail ends at the raw neg; an int return keeps the setnz
// off).
extern "C" int ResTbox_NegInUseState(CfResTboxImpl* ths) {
    return -(int)!ths->ResObj_IsInUse_C860();
}

// us-801fab6c - open/activate the textbox resource. Runs the parent's
// 0x17C/0x178 init slots, gates on CfGameManager state / global flags, then
// builds the textbox resources through the parent's +0x90/+0x94/+0x9C slots
// (selected by parent flag bits 0x20/0x10), clears flag bits 0x4/0x20000000,
// dispatches the +0x168 anim setter and finally runs the +0x38/+0x98
// sub-objects.
extern "C" void ResTbox_OpenResource(CfResTboxImpl* ths) {
    u32 outC;
    u32 out8;
    u8 buf64[0x44];

    ((cf::CfObjectModel*)ths->field_00)->CfObjectModel_releaseModelList();
    ((cf::CfObjectModel*)ths->field_00)->CfObjectModel_releaseModelSub();
    ths->field_00->field_90 = 0;
    CfResObjParent* parent = ths->field_00;
    parent->field_94 = 0;
    if (((cf::CfGameManager*)parent)->getEffectFlagState() == 0) return;
    if (gmGameFlags & 0x100000) return;
    if (isSceneActive__Q22cf13CfGameManagerFv() != 0) return;
    ResInfoEntry* entry = (ResInfoEntry*)CfRes_getInstPtr29C();
    int ok = 1;
    if (entry->field_0x2C->isInUse(entry) == 0) ok = 0;
    if (!ok) return;
    ths->field_08 = ths->field_08 + 1;
    mtl::MemManager::setMemInitFlag(false);
    if ((ths->field_00->field_6C & 0x20) && ths->field_00->field_98 == 0) {
        u32 handle1 = ths->ResObj_GetWork_CCBC(1);
        ths->field_00->field_90 = CfRes_findEntryById(entry, handle1);
        ths->field_00->field_90 =
            (u8*)CfRes_lookupStrTable((char*)ths->field_00->field_90, 0, (void**)&outC);
        float anim = ((cf::CfObject*)ths->field_00)->CfObject_getObjScale();
        u8* obj = scnImN4BuildByIdx((u8*)gmScene, ths->field_00->field_90, 6, 1, 0, 0x70);
        CfModel_InstallSub(ths->field_00, obj);
        ((cf::CfObject*)ths->field_00)->CfObject_setObjScale(anim);
    }
    if (ths->field_00->field_6C & 0x10) {
        ((ml::FixStr<64>*)buf64)->mString[0] = 0;
        ((ml::FixStr<64>*)buf64)->mLength = 0;
        if (ths->field_00->field_9C == 0) {
            u32 handle0 = ths->ResObj_GetWork_CCBC(0);
            Tok_FormatStr(*(ml::FixStr<64>*)buf64, handle0, 0, 0);
            u32 handle1 = ths->ResObj_GetWork_CCBC(1);
            ths->field_00->field_94 = CfRes_findEntryById(entry, handle1);
            ths->field_00->field_94 =
                (u8*)CfRes_lookupStrTable((char*)ths->field_00->field_94, 1, (void**)&out8);
            ths->field_00->field_9C = (u8*)initMcaFile(
                (u32)CfRes_getD80Flag(), (u32)ths->field_00->field_94, (const char*)buf64);
        }
    }
    mtl::MemManager::setMemInitFlag(true);
    if (ths->field_00->field_6C & 0x20000000) {
        f32 anim = lbl_eu_806681D0;
        ths->field_00->field_6C =
            *(volatile u32*)&ths->field_00->field_6C & ~0x20000000;
        ((cf::CfObject*)ths->field_00)->CfObject_syncModelRate(anim);
        CfObjectMove_setMoveSpeedGated((cf::CfObjectMove*)ths->field_00, lbl_eu_806681D4);
    }
    CfObjectMove_initAnimTarget((cf::CfObjectMove*)ths->field_00);
    if (ths->field_00->field_38 != 0) {
        ths->field_00->field_38->CfObject_getPosTriple();
    }
    if (ths->field_00->field_98 != 0) {
        ths->field_00->field_98->vfunc88(1);
    }
}

extern "C" void ResTbox_EmptyStub91B0(void) {}

extern "C" void ResTbox_CacheNullPtmfOnce(CfResTboxImpl* ths) {
    if (lbl_eu_80664660[0] == 0) {
        u32* src = __ptmf_null;
        u32* dst = (u32*)lbl_eu_805351E0;
        *dst++ = *src++;
        dst[0] = src[0];
        dst[1] = src[1];
        lbl_eu_80664660[0] = 1;
    }
    u16 idx = ths->field_08;
    if (idx < 3) {
        (ths->*((CfResTboxImplPMF&)lbl_eu_805351E0[idx]))();
    }
}

cf::CfResTboxImpl::~CfResTboxImpl() {}

extern "C" bool ResTbox_IsCellBitSet(unsigned char* p, int i, int j) {
    return p[i * 0x49 + j * 2] != 0;
}

extern "C" void ResTbox_SetCellBit80(unsigned char* base, int idx1, int idx2, int idx3) {
    unsigned char* p = base + idx1 * 73 + idx2 * 16 + idx3 * 2;
    p[1] |= 0x80;
}

void ResTbox_SetCellBit40(unsigned char* base, int idx1, int idx2, int idx3) {
    unsigned char* p = base + idx1 * 73 + idx2 * 16 + idx3 * 2;
    p[1] |= 0x40;
}

// --- hand-built retail data (splits: .data 80534F70-80535204,
// .sbss 80664660-80664668, .sdata2 806681D0-806681D8) ---
// T1 (80534F70): CfGimmickObject 9-slot vtable (RTTI, null, dtor, 6 funcs).
__declspec(section ".data") __attribute__((used, aligned(8)))
const void* lbl_eu_80534F70[9] = {
    __RTTI__Q22cf15CfGimmickObject, 0, (const void*)__dt__Q22cf15CfGimmickObjectFv,
    (const void*)CfGimmick_DetachManager__Q22cf9CfGimmickFPQ22cf13CfGameManager,
    (const void*)GimObjDriveStepMachine, (const void*)GimObjSetBusyBit,
    (const void*)GimObjClearStepBits, (const void*)GimObjFinishStep,
    (const void*)GimObjRebuildColliders,
};
// T2 (80534F94): 5-word descriptor row (sdata locator, null, sdata, 0, 0).
__declspec(section ".data") __attribute__((used, aligned(4)))
const void* lbl_eu_80534F94[5] = {
    __RTTI__Q22cf11IObjectInfo, 0, __RTTI__Q22cf9CfGimmick, 0,
    0,
};
// T3 (80534FA8): 4-word descriptor row (sdata locator + zeros).
__declspec(section ".data") __attribute__((used, aligned(4)))
const void* lbl_eu_80534FA8[4] = {
    __RTTI__Q22cf11IObjectInfo, 0, 0, 0,
};
// T4 (80534FB8): CfObjectTbox full vtable (RTTI, null, 135 virtuals, null).
// Layout from the retail .rela.data (every slot but +0x04/+0x224 carries
// ADDR32); the trailing +0x224 word is a literal zero like retail.
__declspec(section ".data") __attribute__((used, aligned(4)))
const void* lbl_eu_80534FB8[138] = {
    __RTTI__Q22cf12CfObjectTbox, 0, (const void*)CObjectState_setStateBitMask__Q22cf12CObjectStateFUl, (const void*)CObjectState_checkStateFlags__Q22cf12CObjectStateFi,
    (const void*)CObjectState_setStateBitFlag__Q22cf12CObjectStateFUl, (const void*)CObjectState_clearStateWord4__Q22cf12CObjectStateFv, (const void*)CObjectState_applyStateFlags__Q22cf12CObjectStateFi, (const void*)CObjectState_clearStateWord8__Q22cf12CObjectStateFv,
    (const void*)CObjectState_clearStateFlags8__Q22cf12CObjectStateFi, (const void*)CObjectState_checkStateFlags8__Q22cf12CObjectStateFi, (const void*)CObjectState_checkStateFlagsC__Q22cf12CObjectStateFi, (const void*)CObjectState_setStateBitMask0__Q22cf12CObjectStateFii,
    (const void*)CObjectState_getStateData__Q22cf12CObjectStateFv, (const void*)CObjectState_setStateBitMask2__Q22cf12CObjectStateFv, (const void*)CObjectState_setStateBitMask3__Q22cf12CObjectStateFv, (const void*)CObjectParam_setObjectName__Q22cf12CObjectParamFPCc,
    (const void*)CObjectParam_getParamPtr__Q22cf12CfObjectMoveFv, (const void*)CObjectParam_hasObjectName__Q22cf12CObjectParamFv, (const void*)reset__Q22cf11CfObjectObjFv, (const void*)CObjectParam_getSelfObjectId__Q22cf12CObjectParamFv,
    (const void*)CObjectParam_signalActionEnd__Q22cf12CObjectParamFi, (const void*)__dt__Q22cf12CfObjectTboxFv, (const void*)initialize__Q22cf11CfObjectObjFv, (const void*)CfObject_UnkVirtualFunc3__Q22cf8CfObjectFv,
    (const void*)initTbox__Q22cf12CfObjectTboxFv, (const void*)CfObject_updateMoveRate__Q22cf12CfObjectMoveFv, (const void*)CfObject_releaseMoveTargets__Q22cf12CfObjectMoveFv, (const void*)CfObject_UnkVirtualFunc7__Q22cf12CfObjectMoveFv,
    (const void*)CfObject_notifyEventDone__Q22cf13CfObjectModelFv, (const void*)CfObject_isMoveActiveNow__Q22cf12CfObjectMoveFv, (const void*)CfObject_forwardSubObject__Q22cf12CfObjectMoveFv, (const void*)CfObject_notifyMemberRemoved__Q22cf8CfObjectFv,
    (const void*)CfObject_refreshSubB0__Q22cf12CfObjectMoveFv, (const void*)CfObject_queryTargetState__Q22cf12CfObjectMoveFv, (const void*)CfObject_pushRefreshValue__Q22cf12CfObjectMoveFf, (const void*)CfObject_getMoveSpeedRate__Q22cf12CfObjectMoveFv,
    (const void*)CfObject_pushRefreshExtra__Q22cf12CfObjectMoveFf, (const void*)CfObject_readRefreshValue__Q22cf12CfObjectMoveFv, (const void*)CfObject_checkTargetState__Q22cf12CfObjectMoveFv, (const void*)CfObject_setMoveTargetVec__Q22cf12CfObjectMoveFPCQ22ml5CVec3,
    (const void*)CfObject_setPosXZ__Q22cf13CfObjectModelFff, (const void*)CfObject_setPosXYZ__Q22cf8CfObjectFv, (const void*)CfObject_syncMoveTarget__Q22cf12CfObjectMoveFPCQ22ml5CVec3, (const void*)CfObject_getPosVector__Q22cf12CfObjectMoveFv,
    (const void*)CfObject_getPosTriple__Q22cf13CfObjectModelFv, (const void*)CfObject_snapMoveTarget__Q22cf12CfObjectMoveFPQ22ml5CVec3f, (const void*)CfObject_applyMoveOffset__Q22cf12CfObjectMoveFPCQ22ml5CVec3f, (const void*)CfObject_setRotVec__Q22cf12CfObjectMoveFPv,
    (const void*)CfObject_getField48__Q22cf13CfObjectModelFv, (const void*)CfObject_setMoveHeadAngle__Q22cf12CfObjectMoveFf, (const void*)CfObject_setMoveYaw__Q22cf12CfObjectMoveFf, (const void*)CfObject_getMoveHeadAngle__Q22cf13CfObjectModelFv,
    (const void*)CfObject_syncMoveHead__Q22cf12CfObjectMoveFf, (const void*)CfObject_applyMoveYaw__Q22cf12CfObjectMoveFf, (const void*)CfObject_getScaledAngle__Q22cf13CfObjectModelFv, (const void*)CfObject_setObjScale__Q22cf13CfObjectModelFf,
    (const void*)CfObject_getObjScale__Q22cf13CfObjectModelFv, (const void*)CfObject_resetAnimSlots__Q22cf12CfObjectMoveFUl, (const void*)CfObject_clearAnimEntry__Q22cf12CfObjectMoveFUl, (const void*)CfObject_armAnimSlot0__Q22cf12CfObjectMoveFUlUl,
    (const void*)CfObject_armAnimSlot0N__Q22cf12CfObjectMoveFUlUl, (const void*)CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv, (const void*)CfObject_armAnimSlot1__Q22cf12CfObjectMoveFUlUl, (const void*)CfObject_armAnimSlot1N__Q22cf12CfObjectMoveFUlUl,
    (const void*)CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv, (const void*)CfObject_rearmAnimSlot__Q22cf12CfObjectMoveFUlUl, (const void*)CfObject_setMoveTargetPtr__Q22cf12CfObjectMoveFPv, (const void*)CfObject_createMoveTarget__Q22cf12CfObjectMoveFi,
    (const void*)CfObject_getCurrentTarget__Q22cf12CfObjectMoveFv, (const void*)CfObject_forwardNpcAction__Q22cf12CfObjectMoveFi, (const void*)CfObject_getNpcActionState__Q22cf12CfObjectMoveFv, (const void*)CfObject_getNpcTargetAction__Q22cf12CfObjectMoveFv,
    (const void*)CfObject_findNodeMatrix__Q22cf13CfObjectModelCFPCc, (const void*)CfObject_getIndexedNodeMatrix__Q22cf13CfObjectModelCFUl, (const void*)CfObject_getSubPosWord__Q22cf13CfObjectModelFv, (const void*)CfObject_getPosSample__Q22cf13CfObjectModelCFi,
    (const void*)CfObject_getHeightDiff__Q22cf13CfObjectModelCFv, (const void*)CfObject_setMoveValue__Q22cf12CfObjectMoveFf, (const void*)CfObject_getMoveRateScale__Q22cf12CfObjectMoveFv, (const void*)CfObject_setMoveScale__Q22cf12CfObjectMoveFf,
    (const void*)CfObject_getMoveFactor__Q22cf12CfObjectMoveFv, (const void*)CfObject_setAnimSlotEntry__Q22cf12CfObjectMoveFUlUl, (const void*)CfObject_getSlotBits__Q22cf12CfObjectMoveFUl, (const void*)CfObject_readKindFlagWord__Q22cf13CfObjectModelFv,
    (const void*)CfObject_setMoveBusyState__Q22cf12CfObjectMoveFi, (const void*)CfObject_setMoveReadyFlag__Q22cf12CfObjectMoveFi, (const void*)setPointEnabled__Q22cf12CfObjectMoveFi, (const void*)CfObject_setVisibleFlag__Q22cf13CfObjectModelFi,
    (const void*)CfObject_UnkVirtualFunc68__Q22cf13CfObjectModelFPCQ22ml5CVec3, (const void*)CfObject_isVisibleFlag__Q22cf13CfObjectModelFv, (const void*)CfObject_syncModelRate__Q22cf12CfObjectMoveFf, (const void*)CfObject_getProgressLow__Q22cf13CfObjectModelFv,
    (const void*)CfObject_setProgressHigh__Q22cf13CfObjectModelFf, (const void*)CfObject_getProgressHigh__Q22cf13CfObjectModelFv, (const void*)CfObjectModel_releaseModelSub__Q22cf12CfObjectMoveFv, (const void*)CfObjectModel_releaseModelList__Q22cf12CfObjectMoveFv,
    (const void*)CfObjectModel_getModelName__Q22cf13CfObjectModelFv, (const void*)CfObjectModel_getAnimState__Q22cf12CfObjectMoveFv, (const void*)CfObjectModel_getAnimFlags__Q22cf12CfObjectMoveFv, (const void*)CfObjectModel_checkTargetNode__Q22cf12CfObjectMoveFPv,
    (const void*)CfObjectModel_snapSubTarget__Q22cf13CfObjectModelFPQ22ml5CVec3f, (const void*)CfObjectModel_getSubFlag04__Q22cf13CfObjectModelFv, (const void*)CfObjectModel_getModelFloatA8__Q22cf13CfObjectModelFv, (const void*)CfObjectModel_setModelFloatA8__Q22cf13CfObjectModelFf,
    (const void*)CfObjectModel_getModelRate__Q22cf13CfObjectModelFv, (const void*)CfObjectModel_setModelRate__Q22cf13CfObjectModelFf, (const void*)CfObjectModel_aimAtPosition__Q22cf13CfObjectModelFPCQ22ml5CVec3, (const void*)CfObjectModel_bindModelTo__Q22cf13CfObjectModelFPQ22cf8CfObjectPCc,
    (const void*)CfObjectModel_snapToBone__Q22cf13CfObjectModelFPQ22cf8CfObjectPCc, (const void*)CfObjectModel_setBoneRotate__Q22cf13CfObjectModelFi, (const void*)CfObjectModel_getBindSource__Q22cf13CfObjectModelFv, (const void*)CfObjectModel_detachEffectSlot__Q22cf12CfObjectMoveFPv,
    (const void*)CfObjectModel_setModelVisible__Q22cf12CfObjectMoveFi, (const void*)CfObjectModel_getModelVisible__Q22cf13CfObjectModelFv, (const void*)CfObjectMove_dropMoveRequest__Q22cf12CfObjectMoveFv, (const void*)CfObjectMove_detachModelList__Q22cf12CfObjectMoveFv,
    (const void*)CfObjectMove_releaseSlotById__Q22cf12CfObjectMoveFi, (const void*)CfObjectMove_recordMoveValue__Q22cf12CfObjectMoveFf, (const void*)CfObjectMove_getMovementRate__Q22cf12CfObjectMoveFv, (const void*)CfObjectMove_setTargetC4Arg__Q22cf12CfObjectMoveFUl,
    (const void*)CfObjectMove_refreshNpcState__Q22cf12CfObjectMoveFiiiPCQ22ml5CVec3d, (const void*)CfObjectMove_updateNpcTarget__Q22cf12CfObjectMoveFPCQ22ml5CVec3fii, (const void*)CfObjectMove_queryNpcAdvance__Q22cf12CfObjectMoveFv, (const void*)CfObjectMove_setNpcMoveRange__Q22cf12CfObjectMoveFf,
    (const void*)CfObjectMove_writeTargetField__Q22cf12CfObjectMoveFi, (const void*)CfObjectMove_advanceNpcTarget__Q22cf12CfObjectMoveFPCQ22ml5CVec3, (const void*)CfObjectMove_commandNpcTarget__Q22cf12CfObjectMoveFi, (const void*)CfObjectMove_restartNpcTarget__Q22cf12CfObjectMoveFiii,
    (const void*)CfObjectMove_hasActorScale__Q22cf12CfObjectMoveFv, (const void*)CfObjectMove_attachEffectSlot__Q22cf12CfObjectMoveFUlUlUlUlUl, (const void*)CfObjectMove_setEffectSlotBit__Q22cf12CfObjectMoveFUl, (const void*)CfObjectMove_transferSlotBits__Q22cf12CfObjectMoveFUl,
    (const void*)CfObjectMove_isEffectSlotUsed__Q22cf12CfObjectMoveFUl, (const void*)CfObjectMove_signalEffectSlot__Q22cf12CfObjectMoveFUl, (const void*)CfObjectMove_flushEffectSlots__Q22cf12CfObjectMoveFv, (const void*)CfObjectMove_setAllEffectBits__Q22cf12CfObjectMoveFv,
    (const void*)CfObjectMove_loadResourceById__Q22cf12CfObjectMoveFUl, 0,
};
// sdata2 scalars read by ResTbox_OpenResource (1.0f rate, 20.0f speed).
__declspec(section ".sdata2") __attribute__((used))
float lbl_eu_806681D0 = 1.0f;
__declspec(section ".sdata2") __attribute__((used))
float lbl_eu_806681D4 = 20.0f;
// sbss once-flag (retail 8 bytes: the label covers the whole granule).
char lbl_eu_80664660[8] = { 0 };
// T5 (805351E0): 3 x 12-byte member-function slots. Retail encoding is
// {this-delta 0, -1 marker, func addr}; slot 0 is zeros (filled at runtime
// from __ptmf_null by ResTbox_CacheNullPtmfOnce). Declared as word triples
// (CfPmf3, see header) so the func addresses + markers are static data;
// readers reinterpret the slots as CfResTboxImplPMF (same 12-byte shape).
__declspec(section ".data") __attribute__((used, aligned(4)))
CfPmf3 lbl_eu_805351E0[3] = {
    { 0, 0, 0 },
    { 0, 0xFFFFFFFF, (u32)ResTbox_OpenResource },
    { 0, 0xFFFFFFFF, (u32)ResTbox_EmptyStub91B0 },
};
