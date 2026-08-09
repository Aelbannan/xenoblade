#pragma once

#include <types.h>
#include "kyoshin/realtimeevt/CREvtModel.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/work/CEventFile.hpp"

class CREvtModelMap : public CREvtModel, public IWorkEvent {
public:
    CREvtModelMap();
    virtual ~CREvtModelMap();
    virtual bool OnFileEvent(CEventFile* pEventFile);

    // CREvtModel virtual overrides
    /* vtable 0x1C */ virtual bool isBusy();           // func_80180C88
    /* vtable 0x24 */ virtual int getGuestState();      // func_8018196C
    /* vtable 0x2C */ virtual void setVisible(int r4); // func_801818BC
    /* vtable 0x34 */ virtual void onEvent(int r4);    // func_801815AC
    /* vtable 0x38 */ virtual bool reset();            // func_80180CBC (retail returns r3=1)

    // Non-virtual helpers
    bool isMapModel();          // func_80180C60
    void setGuestModeOff();     // func_80180DCC
    void setGuestModeOn();      // func_80180E1C
    void loadCreature();        // func_80180E60
    void loadFiles();           // func_80180EBC
    void workEvent();           // func_8018140C
    void updatePosition();      // func_8018152C
    bool handleFileEvent(CEventFile* pEvent); // func_80181988

    // Fields (inherited from CREvtModel: 0x00-0x37)
    // 0x38: IWorkEvent vtable pointer
    /* 0x3C */ u8 mVisible;
    /* 0x3D */ u8 mIsGuest;
    /* 0x3E */ u8 _pad3E[2];
    /* 0x40 */ CFileHandle* mFileHandle1;
    /* 0x44 */ void* mFileData1;
    /* 0x48 */ CFileHandle* mFileHandle2;
    /* 0x4C */ void* mFileData2;
    /* 0x50 */ char mBasePath[0x40];  // guest path buffer (0x50-0x8F)
    /* 0x90 */ u32 mBasePathLen;       // length of mBasePath
    /* 0x94 */ CFileHandle* mFileHandle3;
    /* 0x98 */ void* mFileData3;
    /* 0x9C */ u8 mPad9C;
    /* 0x9D */ u8 _pad9D[0x3F];
    /* 0xDC */ void* mUnkDC;
    /* 0xE0 */ void* mLoadedModelData;
    /* 0xE4 */ void* mEmoteModel;
    /* 0xE8 */ char mModelName[0x100];
    /* 0x1E8 */ s32 mCreatureCount;
    /* 0x1EC */ s32 mCreatureId;
    /* 0x1F4 */ u32 mPtmf[3]; // ptmf triplet (func/this-adj/vtbl-adj)
};