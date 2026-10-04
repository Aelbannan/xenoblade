#pragma once

/**
 * @file CREvtModel.hpp
 * CREvtModel class definition.
 *
 * CREvtModel is the base class for realtime event model objects.
 * It inherits from cf::CREvtObj and manages model data, animations,
 * and rendering for realtime event sequences.
 */

#include <types.h>
#include "kyoshin/realtimeevt/CREvtObj.hpp"

class CScnItemModel;

class __declspec(novtable) CREvtModel : public cf::CREvtObj {
public:
    CREvtModel(void* pData, void* pArg);
    /* vtable 0x00 */ virtual ~CREvtModel(); // slot +0x00: __ct__80172668 (overrides __dt__Q22cf8CREvtObjFv)
    /* vtable 0x04 */ virtual void vfunc_04();           // slot +0x04: EvtModelResetPose (overrides base pure slot)
    /* vtable 0x08 */ virtual void vfunc_08();           // slot +0x08: releaseEvtAnim28 (overrides CREvtObjVfunc08Default)
    /* vtable 0x0C */ virtual void vfunc_0C();           // slot +0x0C: getField20, cross-TU (overrides EvtObj_ConstFalse)
    /* vtable 0x10 */ virtual void vfunc_10();           // slot +0x10: isEvtFlagBit4Set, tests (mFlags>>4)&1 (overrides CREvtObjVfunc10Default)
    /* vtable 0x14 */ virtual void vfunc_14();           // slot +0x14: isEvtFlagBit0Set, tests mFlags&1 (overrides REvtCam_AlwaysTrue1)
    /* vtable 0x18 */ virtual bool isBusy(); // slot +0x18: CREvtObjIsBusyDefault, same as base = inherited, cross-TU
    /* vtable 0x1C */ virtual void vfunc_1C();           // slot +0x1C: EvtModelUpdate (overrides EvtObj_RunCallback)
    /* vtable 0x20 */ virtual int getGuestState(); // vfunc_20
    /* vtable 0x24 */ virtual void CREvtObjVfunc24Default();
    /* vtable 0x28 */ virtual void setVisible(int visible); // vfunc_28
    /* vtable 0x2C */ virtual void EvtModelSetEventData(void* pData, void* pModelData);
    /* vtable 0x30 */ virtual void onEvent(unsigned int visible); // vfunc_30
    /* vtable 0x34 */ virtual bool reset(); // vfunc_34
    /* vtable 0x38 */ virtual void EvtModelReleaseAnims();

    // Non-virtual methods
    int EvtModelReleaseAnimsOnce();
    void* evtModelDataPtr();
    void EvtModelUpdate();
    void EvtModelResetPose();
    void releaseEvtAnim28();
    int isEvtDataFlagSet();
    void emptyEvtModelHook();
    void EvtModelSetVisible();
    void EvtModelSetupShadow(CScnItemModel* pModel);
    void EvtModelApplyHideParts(void* pData, CScnItemModel* pModel, int flag);
    void* createEvtModelObj();
    int isEvtFlagBit0Set();
    int isEvtFlagBit4Set();
    int zeroEvtModelHook();

    // Fields (CREvtObj base: 0x00-0x13)
    /* 0x14 */ u32 mField14;
    /* 0x18 */ u32 mFlags;
    /* 0x1C */ void* mPtr1C;
    /* 0x20 */ CScnItemModel* mModel;  // model pointer
    /* 0x24 */ void* mData;             // animation data
    /* 0x28 */ void* mPtr28;
    /* 0x2C */ int mField2C;
    /* 0x30 */ int mField30;
    /* 0x34 */ int mField34;            // animation index counter
};

// Pose block returned by simGetLeafActData and filled by EvtModelResetPose:
// translation at 0x00, rotation at 0x0C.
struct Vec3 {
    float a[3];
};
struct CREvtModelPose {
    /* 0x00 */ Vec3 translate;
    /* 0x0C */ Vec3 rotate;
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void EvtModelApplyHideParts(void* self, void* animData, void* model, int flag);
extern "C" void EvtModelSetupShadow(void* self, void* model);