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
// ADDR32 relocs. CfGimmick_DetachManager now uses the flat retail name
// (defined by kyoshin/cf/CfGimmick.cpp since the linkage wave).
extern "C" {
void __dt__Q22cf12CfObjectTboxFv(void*, int); // ABI deleting dtor
void __dt__Q22cf15CfGimmickObjectFv(void*, int); // ABI deleting dtor
void CObjectParam_UnkVirtualFunc1__Q22cf12CObjectParamFv(void);
void CObjectParam_UnkVirtualFunc2__Q22cf12CfObjectMoveFv(void);
void CObjectParam_getSelfObjectId__Q22cf12CObjectParamFv(void);
void CObjectParam_hasObjectName__Q22cf12CObjectParamFv(void);
void CObjectParam_signalActionEnd__Q22cf12CObjectParamFv(void);
void CObjectState_UnkVirtualFunc8__Q22cf12CObjectStateFv(void);
void CObjectState_UnkVirtualFunc9__Q22cf12CObjectStateFv(void);
void CObjectState_applyStateFlags__Q22cf12CObjectStateFv(void);
void CObjectState_checkStateFlags__Q22cf12CObjectStateFv(void);
void CObjectState_clearStateFlags8__Q22cf12CObjectStateFv(void);
void CObjectState_clearStateWord4__Q22cf12CObjectStateFv(void);
void CObjectState_clearStateWord8__Q22cf12CObjectStateFv(void);
void CObjectState_getStateData__Q22cf12CObjectStateFv(void);
void CObjectState_setStateBitFlag__Q22cf12CObjectStateFv(void);
void CObjectState_setStateBitMask0__Q22cf12CObjectStateFv(void);
void CObjectState_setStateBitMask2__Q22cf12CObjectStateFv(void);
void CObjectState_setStateBitMask3__Q22cf12CObjectStateFv(void);
void CObjectState_setStateBitMask__Q22cf12CObjectStateFv(void);
void CfGimmick_DetachManager(void);
void CfObjectModel_UnkVirtualFunc10__Q22cf13CfObjectModelFv(void);
void CfObjectModel_UnkVirtualFunc11__Q22cf13CfObjectModelFv(void);
void CfObjectModel_UnkVirtualFunc12__Q22cf13CfObjectModelFv(void);
void CfObjectModel_UnkVirtualFunc13__Q22cf13CfObjectModelFv(void);
void CfObjectModel_UnkVirtualFunc14__Q22cf13CfObjectModelFv(void);
void CfObjectModel_UnkVirtualFunc15__Q22cf13CfObjectModelFv(void);
void CfObjectModel_UnkVirtualFunc16__Q22cf13CfObjectModelFv(void);
void CfObjectModel_UnkVirtualFunc17__Q22cf13CfObjectModelFv(void);
void CfObjectModel_UnkVirtualFunc18__Q22cf12CfObjectMoveFv(void);
void CfObjectModel_UnkVirtualFunc19__Q22cf12CfObjectMoveFv(void);
void CfObjectModel_UnkVirtualFunc1__Q22cf12CfObjectMoveFv(void);
void CfObjectModel_UnkVirtualFunc20__Q22cf13CfObjectModelFv(void);
void CfObjectModel_UnkVirtualFunc2__Q22cf12CfObjectMoveFv(void);
void CfObjectModel_UnkVirtualFunc3__Q22cf13CfObjectModelFv(void);
void CfObjectModel_UnkVirtualFunc4__Q22cf12CfObjectMoveFv(void);
void CfObjectModel_UnkVirtualFunc5__Q22cf12CfObjectMoveFv(void);
void CfObjectModel_UnkVirtualFunc6__Q22cf12CfObjectMoveFv(void);
void CfObjectModel_UnkVirtualFunc7__Q22cf13CfObjectModelFv(void);
void CfObjectModel_UnkVirtualFunc8__Q22cf13CfObjectModelFv(void);
void CfObjectModel_UnkVirtualFunc9__Q22cf13CfObjectModelFv(void);
void CfObjectMove_UnkVirtualFunc10__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_UnkVirtualFunc15__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_UnkVirtualFunc6__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_advanceNpcTarget__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_attachEffectSlot__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_commandNpcTarget__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_detachModelList__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_dropMoveRequest__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_flushEffectSlots__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_getMovementRate__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_isEffectSlotUsed__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_loadResourceById__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_queryNpcAdvance__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_recordMoveValue__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_refreshNpcState__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_releaseSlotById__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_restartNpcTarget__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_setAllEffectBits__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_setEffectSlotBit__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_signalEffectSlot__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_transferSlotBits__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_updateNpcTarget__Q22cf12CfObjectMoveFv(void);
void CfObjectMove_writeTargetField__Q22cf12CfObjectMoveFv(void);
void CfObject_UnkVirtualFunc11__Q22cf8CfObjectFv(void);
void CfObject_UnkVirtualFunc12__Q22cf12CfObjectMoveFv(void);
void CfObject_UnkVirtualFunc20__Q22cf13CfObjectModelFv(void);
void CfObject_UnkVirtualFunc21__Q22cf8CfObjectFv(void);
void CfObject_UnkVirtualFunc22__Q22cf12CfObjectMoveFv(void);
void CfObject_UnkVirtualFunc24__Q22cf13CfObjectModelFv(void);
void CfObject_UnkVirtualFunc25__Q22cf12CfObjectMoveFv(void);
void CfObject_UnkVirtualFunc26__Q22cf12CfObjectMoveFv(void);
void CfObject_UnkVirtualFunc27__Q22cf12CfObjectMoveFPv(void);
void CfObject_UnkVirtualFunc28__Q22cf13CfObjectModelFv(void);
void CfObject_UnkVirtualFunc30__Q22cf12CfObjectMoveFv(void);
void CfObject_UnkVirtualFunc32__Q22cf12CfObjectMoveFv(void);
void CfObject_UnkVirtualFunc33__Q22cf12CfObjectMoveFv(void);
void CfObject_UnkVirtualFunc34__Q22cf13CfObjectModelFv(void);
void CfObject_UnkVirtualFunc35__Q22cf13CfObjectModelFv(void);
void CfObject_UnkVirtualFunc36__Q22cf13CfObjectModelFv(void);
void CfObject_UnkVirtualFunc37__Q22cf12CfObjectMoveFv(void);
void CfObject_UnkVirtualFunc38__Q22cf12CfObjectMoveFv(void);
void CfObject_UnkVirtualFunc39__Q22cf12CfObjectMoveFv(void);
void CfObject_UnkVirtualFunc3__Q22cf8CfObjectFv(void);
void CfObject_UnkVirtualFunc40__Q22cf12CfObjectMoveFv(void);
void CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv(void);
void CfObject_UnkVirtualFunc42__Q22cf12CfObjectMoveFv(void);
void CfObject_UnkVirtualFunc43__Q22cf12CfObjectMoveFv(void);
void CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv(void);
void CfObject_UnkVirtualFunc45__Q22cf12CfObjectMoveFv(void);
void CfObject_UnkVirtualFunc50__Q22cf12CfObjectMoveFv(void);
void CfObject_UnkVirtualFunc51__Q22cf12CfObjectMoveFv(void);
void CfObject_UnkVirtualFunc52__Q22cf13CfObjectModelFv(void);
void CfObject_UnkVirtualFunc53__Q22cf13CfObjectModelFv(void);
void CfObject_UnkVirtualFunc54__Q22cf13CfObjectModelFv(void);
void CfObject_UnkVirtualFunc55__Q22cf13CfObjectModelFv(void);
void CfObject_UnkVirtualFunc56__Q22cf13CfObjectModelFv(void);
void CfObject_UnkVirtualFunc57__Q22cf12CfObjectMoveFv(void);
void CfObject_UnkVirtualFunc59__Q22cf12CfObjectMoveFv(void);
void CfObject_UnkVirtualFunc5__Q22cf12CfObjectMoveFv(void);
void CfObject_UnkVirtualFunc60__Q22cf12CfObjectMoveFv(void);
void CfObject_UnkVirtualFunc62__Q22cf12CfObjectMoveFv(void);
void CfObject_UnkVirtualFunc63__Q22cf13CfObjectModelFv(void);
void CfObject_UnkVirtualFunc66__Q22cf12CfObjectMoveFv(void);
void CfObject_UnkVirtualFunc67__Q22cf13CfObjectModelFv(void);
void CfObject_UnkVirtualFunc68__Q22cf13CfObjectModelFv(void);
void CfObject_UnkVirtualFunc69__Q22cf13CfObjectModelFv(void);
void CfObject_UnkVirtualFunc6__Q22cf12CfObjectMoveFv(void);
void CfObject_UnkVirtualFunc70__Q22cf12CfObjectMoveFv(void);
void CfObject_UnkVirtualFunc71__Q22cf13CfObjectModelFv(void);
void CfObject_UnkVirtualFunc72__Q22cf13CfObjectModelFv(void);
void CfObject_UnkVirtualFunc73__Q22cf13CfObjectModelFv(void);
void CfObject_UnkVirtualFunc7__Q22cf12CfObjectMoveFv(void);
void CfObject_checkTargetState__Q22cf12CfObjectMoveFv(void);
void CfObject_createMoveTarget__Q22cf12CfObjectMoveFv(void);
void CfObject_forwardNpcAction__Q22cf12CfObjectMoveFv(void);
void CfObject_forwardSubObject__Q22cf12CfObjectMoveFv(void);
void CfObject_getCurrentTarget__Q22cf12CfObjectMoveFv(void);
void CfObject_getMoveHeadAngle__Q22cf13CfObjectModelFv(void);
void CfObject_getMoveRateScale__Q22cf12CfObjectMoveFv(void);
void CfObject_getMoveSpeedRate__Q22cf12CfObjectMoveFv(void);
void CfObject_getPosVector__Q22cf12CfObjectMoveFv(void);
void CfObject_isMoveActiveNow__Q22cf12CfObjectMoveFv(void);
void CfObject_notifyEventDone__Q22cf13CfObjectModelFv(void);
void CfObject_pushRefreshExtra__Q22cf12CfObjectMoveFv(void);
void CfObject_pushRefreshValue__Q22cf12CfObjectMoveFf(void);
void CfObject_queryTargetState__Q22cf12CfObjectMoveFv(void);
void CfObject_readRefreshValue__Q22cf12CfObjectMoveFv(void);
void CfObject_setAnimSlotEntry__Q22cf12CfObjectMoveFv(void);
void CfObject_setMoveBusyState__Q22cf12CfObjectMoveFi(void);
void CfObject_setMoveHeadAngle__Q22cf12CfObjectMoveFv(void);
void CfObject_setMoveReadyFlag__Q22cf12CfObjectMoveFv(void);
void CfObject_setMoveTargetPtr__Q22cf12CfObjectMoveFv(void);
void CfObject_setMoveTargetVec__Q22cf12CfObjectMoveFv(void);
void GimObjClearStepBits(void);
void GimObjDriveStepMachine(void);
void GimObjFinishStep(void);
void GimObjRebuildColliders(void);
void GimObjSetBusyBit(void);
void initTbox__Q22cf12CfObjectTboxFv(void);
void initialize__Q22cf11CfObjectObjFv(void);
void reset__Q22cf11CfObjectObjFv(void);
}
extern char lbl_eu_80662700[]; // .sdata RTTI (defined by CPartyStateWin.o)
extern char lbl_eu_80661BE0[];
extern char lbl_eu_80662708[];

using namespace cf;

// us-801faaf0 - constructor: base CfResObjImpl ctor (parent passed through in
// r4), then store the retail vtable label at +0x10 (manual-vtable scheme).
extern "C" cf::CfResTboxImpl* __ct__cf_CfResTboxImpl(CfResTboxImpl* ths, CfResTboxParent* parent) {
    __ct__cf_CfResObjImpl((cf::CfResObjImpl*)ths, (void*)parent);
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
void ResTbox_OpenResource(CfResTboxImpl* ths) {
    u32 outC;
    u32 out8;
    u8 buf64[0x44];

    ((cf::CfObjectModel*)ths->field_00)->CfObjectModel_releaseModelList();
((cf::CfObjectModel*)ths->field_00)->CfObjectModel_releaseModelSub();
    ths->field_00->field_90 = 0;
    CfResTboxParent* parent = ths->field_00;
    parent->field_94 = 0;
    if (((cf::CfGameManager*)parent)->getEffectFlagState() == 0) return;
    if (lbl_eu_80663E24 & 0x100000) return;
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
            CfRes_lookupStrTable((char*)ths->field_00->field_90, 0, (u32**)&outC);
float anim = ((cf::CfObject*)ths->field_00)->CfObject_getObjScale();
        u8* obj = scnImN4BuildByIdx((u8*)lbl_eu_80663E14, ths->field_00->field_90, 6, 1, 0, 0x70);
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
                CfRes_lookupStrTable((char*)ths->field_00->field_94, 1, (u32**)&out8);
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
((cf::CfObject*)ths->field_00->field_38)->CfObject_getPosTriple();
    }
    if (ths->field_00->field_98 != 0) {
        ths->field_00->field_98->vfunc88(1);
    }
}

void ResTbox_EmptyStub91B0(void) {}

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
    lbl_eu_80662700, 0, (const void*)__dt__Q22cf15CfGimmickObjectFv,
    (const void*)CfGimmick_DetachManager,
    (const void*)GimObjDriveStepMachine, (const void*)GimObjSetBusyBit,
    (const void*)GimObjClearStepBits, (const void*)GimObjFinishStep,
    (const void*)GimObjRebuildColliders,
};
// T2 (80534F94): 5-word descriptor row (sdata locator, null, sdata, 0, 0).
__declspec(section ".data") __attribute__((used, aligned(4)))
const void* lbl_eu_80534F94[5] = {
    lbl_eu_80661BE0, 0, lbl_eu_80662708, 0,
    0,
};
// T3 (80534FA8): 4-word descriptor row (sdata locator + zeros).
__declspec(section ".data") __attribute__((used, aligned(4)))
const void* lbl_eu_80534FA8[4] = {
    lbl_eu_80661BE0, 0, 0, 0,
};
// T4 (80534FB8): CfObjectTbox full vtable (RTTI, null, 135 virtuals, null).
// Layout from the retail .rela.data (every slot but +0x04/+0x224 carries
// ADDR32); the trailing +0x224 word is a literal zero like retail.
__declspec(section ".data") __attribute__((used, aligned(4)))
const void* lbl_eu_80534FB8[138] = {
    lbl_eu_80661D18, 0, (const void*)CObjectState_setStateBitMask__Q22cf12CObjectStateFv, (const void*)CObjectState_checkStateFlags__Q22cf12CObjectStateFv,
    (const void*)CObjectState_setStateBitFlag__Q22cf12CObjectStateFv, (const void*)CObjectState_clearStateWord4__Q22cf12CObjectStateFv, (const void*)CObjectState_applyStateFlags__Q22cf12CObjectStateFv, (const void*)CObjectState_clearStateWord8__Q22cf12CObjectStateFv,
    (const void*)CObjectState_clearStateFlags8__Q22cf12CObjectStateFv, (const void*)CObjectState_UnkVirtualFunc8__Q22cf12CObjectStateFv, (const void*)CObjectState_UnkVirtualFunc9__Q22cf12CObjectStateFv, (const void*)CObjectState_setStateBitMask0__Q22cf12CObjectStateFv,
    (const void*)CObjectState_getStateData__Q22cf12CObjectStateFv, (const void*)CObjectState_setStateBitMask2__Q22cf12CObjectStateFv, (const void*)CObjectState_setStateBitMask3__Q22cf12CObjectStateFv, (const void*)CObjectParam_UnkVirtualFunc1__Q22cf12CObjectParamFv,
    (const void*)CObjectParam_UnkVirtualFunc2__Q22cf12CfObjectMoveFv, (const void*)CObjectParam_hasObjectName__Q22cf12CObjectParamFv, (const void*)reset__Q22cf11CfObjectObjFv, (const void*)CObjectParam_getSelfObjectId__Q22cf12CObjectParamFv,
    (const void*)CObjectParam_signalActionEnd__Q22cf12CObjectParamFv, (const void*)__dt__Q22cf12CfObjectTboxFv, (const void*)initialize__Q22cf11CfObjectObjFv, (const void*)CfObject_UnkVirtualFunc3__Q22cf8CfObjectFv,
    (const void*)initTbox__Q22cf12CfObjectTboxFv, (const void*)CfObject_UnkVirtualFunc5__Q22cf12CfObjectMoveFv, (const void*)CfObject_UnkVirtualFunc6__Q22cf12CfObjectMoveFv, (const void*)CfObject_UnkVirtualFunc7__Q22cf12CfObjectMoveFv,
    (const void*)CfObject_notifyEventDone__Q22cf13CfObjectModelFv, (const void*)CfObject_isMoveActiveNow__Q22cf12CfObjectMoveFv, (const void*)CfObject_forwardSubObject__Q22cf12CfObjectMoveFv, (const void*)CfObject_UnkVirtualFunc11__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc12__Q22cf12CfObjectMoveFv, (const void*)CfObject_queryTargetState__Q22cf12CfObjectMoveFv, (const void*)CfObject_pushRefreshValue__Q22cf12CfObjectMoveFf, (const void*)CfObject_getMoveSpeedRate__Q22cf12CfObjectMoveFv,
    (const void*)CfObject_pushRefreshExtra__Q22cf12CfObjectMoveFv, (const void*)CfObject_readRefreshValue__Q22cf12CfObjectMoveFv, (const void*)CfObject_checkTargetState__Q22cf12CfObjectMoveFv, (const void*)CfObject_setMoveTargetVec__Q22cf12CfObjectMoveFv,
    (const void*)CfObject_UnkVirtualFunc20__Q22cf13CfObjectModelFv, (const void*)CfObject_UnkVirtualFunc21__Q22cf8CfObjectFv, (const void*)CfObject_UnkVirtualFunc22__Q22cf12CfObjectMoveFv, (const void*)CfObject_getPosVector__Q22cf12CfObjectMoveFv,
    (const void*)CfObject_UnkVirtualFunc24__Q22cf13CfObjectModelFv, (const void*)CfObject_UnkVirtualFunc25__Q22cf12CfObjectMoveFv, (const void*)CfObject_UnkVirtualFunc26__Q22cf12CfObjectMoveFv, (const void*)CfObject_UnkVirtualFunc27__Q22cf12CfObjectMoveFPv,
    (const void*)CfObject_UnkVirtualFunc28__Q22cf13CfObjectModelFv, (const void*)CfObject_setMoveHeadAngle__Q22cf12CfObjectMoveFv, (const void*)CfObject_UnkVirtualFunc30__Q22cf12CfObjectMoveFv, (const void*)CfObject_getMoveHeadAngle__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc32__Q22cf12CfObjectMoveFv, (const void*)CfObject_UnkVirtualFunc33__Q22cf12CfObjectMoveFv, (const void*)CfObject_UnkVirtualFunc34__Q22cf13CfObjectModelFv, (const void*)CfObject_UnkVirtualFunc35__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc36__Q22cf13CfObjectModelFv, (const void*)CfObject_UnkVirtualFunc37__Q22cf12CfObjectMoveFv, (const void*)CfObject_UnkVirtualFunc38__Q22cf12CfObjectMoveFv, (const void*)CfObject_UnkVirtualFunc39__Q22cf12CfObjectMoveFv,
    (const void*)CfObject_UnkVirtualFunc40__Q22cf12CfObjectMoveFv, (const void*)CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv, (const void*)CfObject_UnkVirtualFunc42__Q22cf12CfObjectMoveFv, (const void*)CfObject_UnkVirtualFunc43__Q22cf12CfObjectMoveFv,
    (const void*)CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv, (const void*)CfObject_UnkVirtualFunc45__Q22cf12CfObjectMoveFv, (const void*)CfObject_setMoveTargetPtr__Q22cf12CfObjectMoveFv, (const void*)CfObject_createMoveTarget__Q22cf12CfObjectMoveFv,
    (const void*)CfObject_getCurrentTarget__Q22cf12CfObjectMoveFv, (const void*)CfObject_forwardNpcAction__Q22cf12CfObjectMoveFv, (const void*)CfObject_UnkVirtualFunc50__Q22cf12CfObjectMoveFv, (const void*)CfObject_UnkVirtualFunc51__Q22cf12CfObjectMoveFv,
    (const void*)CfObject_UnkVirtualFunc52__Q22cf13CfObjectModelFv, (const void*)CfObject_UnkVirtualFunc53__Q22cf13CfObjectModelFv, (const void*)CfObject_UnkVirtualFunc54__Q22cf13CfObjectModelFv, (const void*)CfObject_UnkVirtualFunc55__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc56__Q22cf13CfObjectModelFv, (const void*)CfObject_UnkVirtualFunc57__Q22cf12CfObjectMoveFv, (const void*)CfObject_getMoveRateScale__Q22cf12CfObjectMoveFv, (const void*)CfObject_UnkVirtualFunc59__Q22cf12CfObjectMoveFv,
    (const void*)CfObject_UnkVirtualFunc60__Q22cf12CfObjectMoveFv, (const void*)CfObject_setAnimSlotEntry__Q22cf12CfObjectMoveFv, (const void*)CfObject_UnkVirtualFunc62__Q22cf12CfObjectMoveFv, (const void*)CfObject_UnkVirtualFunc63__Q22cf13CfObjectModelFv,
    (const void*)CfObject_setMoveBusyState__Q22cf12CfObjectMoveFi, (const void*)CfObject_setMoveReadyFlag__Q22cf12CfObjectMoveFv, (const void*)CfObject_UnkVirtualFunc66__Q22cf12CfObjectMoveFv, (const void*)CfObject_UnkVirtualFunc67__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc68__Q22cf13CfObjectModelFv, (const void*)CfObject_UnkVirtualFunc69__Q22cf13CfObjectModelFv, (const void*)CfObject_UnkVirtualFunc70__Q22cf12CfObjectMoveFv, (const void*)CfObject_UnkVirtualFunc71__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc72__Q22cf13CfObjectModelFv, (const void*)CfObject_UnkVirtualFunc73__Q22cf13CfObjectModelFv, (const void*)CfObjectModel_UnkVirtualFunc1__Q22cf12CfObjectMoveFv, (const void*)CfObjectModel_UnkVirtualFunc2__Q22cf12CfObjectMoveFv,
    (const void*)CfObjectModel_UnkVirtualFunc3__Q22cf13CfObjectModelFv, (const void*)CfObjectModel_UnkVirtualFunc4__Q22cf12CfObjectMoveFv, (const void*)CfObjectModel_UnkVirtualFunc5__Q22cf12CfObjectMoveFv, (const void*)CfObjectModel_UnkVirtualFunc6__Q22cf12CfObjectMoveFv,
    (const void*)CfObjectModel_UnkVirtualFunc7__Q22cf13CfObjectModelFv, (const void*)CfObjectModel_UnkVirtualFunc8__Q22cf13CfObjectModelFv, (const void*)CfObjectModel_UnkVirtualFunc9__Q22cf13CfObjectModelFv, (const void*)CfObjectModel_UnkVirtualFunc10__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc11__Q22cf13CfObjectModelFv, (const void*)CfObjectModel_UnkVirtualFunc12__Q22cf13CfObjectModelFv, (const void*)CfObjectModel_UnkVirtualFunc13__Q22cf13CfObjectModelFv, (const void*)CfObjectModel_UnkVirtualFunc14__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc15__Q22cf13CfObjectModelFv, (const void*)CfObjectModel_UnkVirtualFunc16__Q22cf13CfObjectModelFv, (const void*)CfObjectModel_UnkVirtualFunc17__Q22cf13CfObjectModelFv, (const void*)CfObjectModel_UnkVirtualFunc18__Q22cf12CfObjectMoveFv,
    (const void*)CfObjectModel_UnkVirtualFunc19__Q22cf12CfObjectMoveFv, (const void*)CfObjectModel_UnkVirtualFunc20__Q22cf13CfObjectModelFv, (const void*)CfObjectMove_dropMoveRequest__Q22cf12CfObjectMoveFv, (const void*)CfObjectMove_detachModelList__Q22cf12CfObjectMoveFv,
    (const void*)CfObjectMove_releaseSlotById__Q22cf12CfObjectMoveFv, (const void*)CfObjectMove_recordMoveValue__Q22cf12CfObjectMoveFv, (const void*)CfObjectMove_getMovementRate__Q22cf12CfObjectMoveFv, (const void*)CfObjectMove_UnkVirtualFunc6__Q22cf12CfObjectMoveFv,
    (const void*)CfObjectMove_refreshNpcState__Q22cf12CfObjectMoveFv, (const void*)CfObjectMove_updateNpcTarget__Q22cf12CfObjectMoveFv, (const void*)CfObjectMove_queryNpcAdvance__Q22cf12CfObjectMoveFv, (const void*)CfObjectMove_UnkVirtualFunc10__Q22cf12CfObjectMoveFv,
    (const void*)CfObjectMove_writeTargetField__Q22cf12CfObjectMoveFv, (const void*)CfObjectMove_advanceNpcTarget__Q22cf12CfObjectMoveFv, (const void*)CfObjectMove_commandNpcTarget__Q22cf12CfObjectMoveFv, (const void*)CfObjectMove_restartNpcTarget__Q22cf12CfObjectMoveFv,
    (const void*)CfObjectMove_UnkVirtualFunc15__Q22cf12CfObjectMoveFv, (const void*)CfObjectMove_attachEffectSlot__Q22cf12CfObjectMoveFv, (const void*)CfObjectMove_setEffectSlotBit__Q22cf12CfObjectMoveFv, (const void*)CfObjectMove_transferSlotBits__Q22cf12CfObjectMoveFv,
    (const void*)CfObjectMove_isEffectSlotUsed__Q22cf12CfObjectMoveFv, (const void*)CfObjectMove_signalEffectSlot__Q22cf12CfObjectMoveFv, (const void*)CfObjectMove_flushEffectSlots__Q22cf12CfObjectMoveFv, (const void*)CfObjectMove_setAllEffectBits__Q22cf12CfObjectMoveFv,
    (const void*)CfObjectMove_loadResourceById__Q22cf12CfObjectMoveFv, 0,
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
