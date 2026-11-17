#pragma once

#include <types.h>
#include "kyoshin/realtimeevt/CREvtModel.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/work/CEventFile.hpp"

class CScnItemModel;

namespace cf {

// cf::CREvtModelPc - realtime-event party-character model (kind 2, 0xB4
// bytes). Bases (retail vtable 0x805321F0 + RTTI base list): public
// cf::CREvtModel at 0x00, private IWorkEvent at 0x38. OnFileEvent overrides
// only the IWorkEvent slot, so MWCC appends its direct entry after the
// IWorkEvent vtable (the secondary slot holds the @56@ thunk).
//
// Six file slots (0 = model, 1-4 = parts, 5 = optional extra): packed
// resource token, pending read request, loaded data and load status
// (1 = arena, 2 = archive heap, 3 = streamed MEM2).
class CREvtModelPc : public CREvtModel, IWorkEvent {
public:
    CREvtModelPc(void* pData);
    virtual ~CREvtModelPc();
    virtual int vfunc_0C();
    virtual int vfunc_14();
    virtual bool isBusy();
    virtual int vfunc_20();
    virtual void vfunc_24();
    virtual void setEventData(void* pData, void* pModelData);
    virtual int reset();
    virtual void releaseAnims();
    virtual bool OnFileEvent(CEventFile* pEvent);

    // mCallback states (installed as ptmf constants).
    void initCounter();
    void loadFiles();
    void checkLoaded();
    void updatePos();

    void createModel();

    /* 0x3C */ u32 mFileHandle[6];      // packed resource tokens
    /* 0x54 */ CFileHandle* mFileReq[6];
    /* 0x6C */ void* mData[6];
    /* 0x84 */ s32 mStatus[6];
    /* 0x9C */ u8 _pad9C[0xA8 - 0x9C];  // never accessed by this TU
    /* 0xA8 */ f32 mSpeed;
    /* 0xAC */ u8 mLoaded;
    /* 0xB0 */ s32 mCounter;
};

} // namespace cf

using cf::CREvtModelPc;
