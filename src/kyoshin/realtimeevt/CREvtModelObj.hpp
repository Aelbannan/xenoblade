#pragma once

#include <types.h>
#include "kyoshin/realtimeevt/CREvtModel.hpp"
#include "monolib/core/CSchedule.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/work/CEventFile.hpp"

namespace cf {

// Event data block at CREvtModel::mPtr1C as seen by the object model.
struct CREvtModelObjData {
    /* 0x00 */ u8 _pad00[0x10];
    /* 0x10 */ char mName[0x10];      // model name (appended to "/chr/jp/np/")
    /* 0x20 */ u32 mResId;            // packed resource token
    /* 0x24 */ u8 _pad24[0x28 - 0x24];
    /* 0x28 */ u32 mKind;             // 2..6: model category
    /* 0x2C */ u8 _pad2C[0x30 - 0x2C];
    /* 0x30 */ u32 mField30;          // 0xFFFF0000-range: standalone mode
    /* 0x34 */ u8 _pad34[0x48 - 0x34];
    /* 0x48 */ char mSuffix[0x10];    // event-name suffix key
    /* 0x58 */ u32 mFlags;
    /* 0x5C */ struct { s16 mStart; s16 mPeer; } mSlots[4];
};

// cf::CREvtModelObj - realtime-event object model (kind 3, 0x88 bytes).
// Bases (retail vtable 0x80531FA0 + RTTI base list): public cf::CREvtModel
// at 0x00, private IWorkEvent at 0x38, private CScheduleChild at 0x3C (the
// object registers itself with schedAttachChildSlot). OnFileEvent and
// OnScheduleDelete override secondary-base slots only, so MWCC appends their
// direct entries after the secondary vtables.
class CREvtModelObj : public CREvtModel, IWorkEvent, CScheduleChild {
public:
    CREvtModelObj(void* pData);
    virtual ~CREvtModelObj();
    virtual int vfunc_10();          // waiting in stepProbe / stepUpdate
    virtual int vfunc_14();          // flags 0x1 and 0x40 both set
    virtual bool isBusy();           // walk slot reached while loading
    virtual int vfunc_20();          // returns 1
    virtual void vfunc_24();         // clears flag 0x20
    virtual void setEventData(void* pData, void* pModelData);
    virtual int reset();
    virtual void releaseAnims();
    virtual bool OnFileEvent(CEventFile* pEvent);
    virtual void OnScheduleDelete(CSchedule* pSchedule);

    // mCallback states (non-virtual, installed as ptmf constants)
    void stepProbe();
    void stepLoad();
    void stepSetup();
    void stepNone();
    void stepUpdate();
    void scanPlayers();

    CREvtModelObjData* evtData() { return static_cast<CREvtModelObjData*>(mPtr1C); }

    // IWorkEvent at 0x38, CScheduleChild at 0x3C
    /* 0x40 */ CFileHandle* mFileHandle;
    /* 0x44 */ s32 mFileState;       // 0 none, 1 evt mem, 2 heap node, 3 file read
    /* 0x48 */ u8* mAllocData;
    /* 0x4C */ s32 mRefCount;        // users borrowing mAllocData
    /* 0x50 */ u32 mModelType;       // 1 arc, 2 chr, 3 ?
    /* 0x54 */ CREvtModelObj* mRefOwner; // model whose mAllocData we borrow
    /* 0x58 */ s32 mFileId;          // -1 = none
    /* 0x5C */ u32 mResId5C;
    /* 0x60 */ u32 mResId60;
    /* 0x64 */ u8 mFlag64;
    /* 0x68 */ u8* mExtractData;
    /* 0x6C */ CFileHandle* mFileHandle2;
    /* 0x70 */ u8* mAllocData2;
    /* 0x74 */ s32 mField74;
    /* 0x78 */ void* mAllocData3;
    /* 0x7C */ CSchedule* mSchedule;  // cleared by OnScheduleDelete
    /* 0x80 */ u32 mCount80;
    /* 0x84 */ s32 mState84;
};

} // namespace cf

using cf::CREvtModelObj;
