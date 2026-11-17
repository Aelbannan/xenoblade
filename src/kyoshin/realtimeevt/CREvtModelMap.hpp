#pragma once

#include <types.h>
#include "kyoshin/realtimeevt/CREvtModel.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/util/FixStr.hpp"

// Name/script info block pointed to by CREvtModelMap::mPtr1C; the 2-char
// model name sits at +0x10 (isMapModel tests it for "ma"), a flags word at
// +0x58 (bit0 = keep handle, bit1 = archive read, bit6 = external pack).
struct CREvtModelMapNameInfo {
    u8 field_0x0[0x10];
    char mName[2];
    u8 field_0x12[0x58 - 0x12];
    u32 field_0x58;   // +0x58: load flags
};

// Fake single-inheritance views of foreign vtables called by this TU.
// MWCC colors real virtual dispatch through r12 (ABI scratch); manual
// function-pointer casts color the vtable temp as r4/r5 and fail byte
// identity. With -RTTI on the first declared virtual lands at slot 0x08
// (two hidden typeinfo slots), so declaring v08() first and counting up in
// steps of 4 makes slot N land at vtable offset N (MWCC_CASES r12).

// Game-manager core dispatch now via cf::CfObject (0x158) and
// CfObjectMove extension (0x190); pads deleted.

// Emote model dispatch via CScnItemModel (0xB4); pad deleted.
#include "kyoshin/realtimeevt/CREvtCamera.hpp"

// Self dispatch now via CREvtModel virtuals (setVisible/onEvent/reset)
// on the real class tree.

// Creature-list node (circular list whose head lives at CfGameManager +0x04):
// link at +0x00, creature object at +0x08 (real vtable has 0x158/0x160).
#include "kyoshin/cf/CfGimmick.hpp"
struct CCreatureNode {
    CCreatureNode* next;   // +0x00
    u8 _pad[4];            // +0x04
    void* obj;      // +0x08
};

// Emote model is a CScnItemModel (real owner of 0xB4 vfuncB4 and +0x7A8
// flags7A8).

namespace cf {

// cf::CREvtModelMap - realtime-event map model (kind 1, 0x1F0 bytes).
// Bases (from the retail vtable 0x80531D80 and RTTI base list): public
// cf::CREvtModel at 0x00, private IWorkEvent at 0x38 (private bases are not
// listed in MWCC's RTTI base list). OnFileEvent overrides only the
// IWorkEvent slot, so MWCC appends its direct entry after the IWorkEvent
// vtable (the secondary slot holds the @56@ thunk).
class CREvtModelMap : public CREvtModel, IWorkEvent {
public:
    CREvtModelMap(void* pData);
    virtual ~CREvtModelMap();
    int isMapModel();
    virtual bool isBusy();
    virtual int reset();
    void probeSlot();
    void loadFiles();
    void finishLoad();
    void update();
    virtual void onEvent(u32 visible);
    virtual void setVisible(int visible);
    virtual int vfunc_20();
    virtual bool OnFileEvent(CEventFile* pEvent);

    // IWorkEvent subobject at 0x38
    /* 0x3C */ u8 mVisible;
    /* 0x3D */ u8 mIsGuest;
    /* 0x40 */ CFileHandle* mFileHandle1;   // .map
    /* 0x44 */ void* mFileData1;
    /* 0x48 */ CFileHandle* mFileHandle2;   // .lod
    /* 0x4C */ void* mFileData2;
    /* 0x50 */ ml::FixStr<64> mBasePath;    // guest path
    /* 0x94 */ CFileHandle* mFileHandle3;   // .lgt
    /* 0x98 */ void* mFileData3;
    /* 0x9C */ ml::FixStr<64> mStr9C;       // only constructed in this TU
    /* 0xE0 */ void* mLoadedModelData;
    /* 0xE4 */ CScnItemModel* mEmoteModel;
    /* 0xE8 */ u8 mCreatureState[0x100];   // per-creature point state saved while hidden
    /* 0x1E8 */ s32 mCreatureCount;
    /* 0x1EC */ s32 mCreatureId;

    static CREvtModelMap* sLoadingGuest;   // 0x806642B0
    static CREvtModelMap* sVisibleModel;   // 0x806642B4
};

} // namespace cf

using cf::CREvtModelMap;


