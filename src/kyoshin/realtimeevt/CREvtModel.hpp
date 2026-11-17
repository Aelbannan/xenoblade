#pragma once

/**
 * @file CREvtModel.hpp
 * cf::CREvtModel - base class of the realtime-event model objects
 * (RTTI "cf::CREvtModel", derives from cf::CREvtObj, kind 3).
 *
 * Derived classes: cf::CREvtModelMap, cf::CREvtModelObj, cf::CREvtModelPc
 * (each also derives from IWorkEvent). The vtable (0x8053167C) and the base
 * list (0x805316C0) are emitted by CREvtModel.cpp; vfunc_0C and vfunc_24 are
 * defined in CTaskREvtSequence.cpp (retail text order).
 */

#include <types.h>
#include "kyoshin/realtimeevt/CREvtObj.hpp"

class CScnItemModel;

namespace cf {

class CREvtModel : public CREvtObj {
public:
    CREvtModel(void* pData, int kind);
    virtual ~CREvtModel();
    virtual void vfunc_04();        // reset the leaf pose of mModel
    virtual void vfunc_08();        // release mPtr28
    virtual int vfunc_0C();         // returns mModel (CTaskREvtSequence.cpp)
    virtual int vfunc_10();         // (mFlags >> 4) & 1
    virtual int vfunc_14();         // mFlags & 1
    virtual void vfunc_1C();        // per-frame update
    virtual int vfunc_20();         // returns 0
    virtual void vfunc_24();        // empty (CTaskREvtSequence.cpp)
    virtual void setVisible(int visible);
    virtual void setEventData(void* pData, void* pModelData);
    virtual void onEvent(u32 visible);  // empty
    virtual int reset();            // releaseAnims() once (flag 0x80)
    virtual void releaseAnims();

    void* evtModelDataPtr();
    int isEvtDataFlagSet();
    void setupShadow(CScnItemModel* pModel);
    void applyHideParts(void* pAnim, CScnItemModel* pModel, int flag);

    static CREvtModel* create(void* pData);

    /* 0x14 */ u32 mField14;            // kind (1 map, 2 pc, 3 obj)
    /* 0x18 */ volatile u32 mFlags;              // 0x400: toggled at the mPtr1C toggle frames
    /* 0x1C */ void* mPtr1C;            // event data (name at +0x10)
    /* 0x20 */ CScnItemModel* mModel;
    /* 0x24 */ void* mData;             // current animation
    /* 0x28 */ void* mPtr28;            // previous animation
    /* 0x2C */ int mField2C;
    /* 0x30 */ int mField30;
    /* 0x34 */ int mField34;            // toggle index
};

} // namespace cf

using cf::CREvtModel;

// Pose block returned by simGetLeafActData and filled by vfunc_04:
// translation at 0x00, rotation at 0x0C.
struct Vec3 {
    float a[3];
};
struct CREvtModelPose {
    /* 0x00 */ Vec3 translate;
    /* 0x0C */ Vec3 rotate;
};
