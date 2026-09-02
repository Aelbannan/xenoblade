#pragma once

#include <types.h>
#include "kyoshin/realtimeevt/CREvtModel.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/work/CEventFile.hpp"

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

// Emote model dispatch via CREvtSceneModel (0xB4); pad deleted.
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

// Emote model dispatch via CREvtSceneModel (real owner of 0xB4).
// Keep alias for field_7A8 access compatibility.

// Flattened layout of the retail CREvtModelMap object. The retail class
// inherits cf::CREvtObj / CREvtModel / IWorkEvent; the base headers are
// declared with a member-function-pointer field whose compiled size (16
// bytes) does not match retail's 12-byte __ptmf, so inheriting them shifts
// every field by +4. Declaring the base region explicitly keeps the retail
// offsets (vtable 0x00, ptmf 0x08, flags 0x18, IWorkEvent vtable 0x38).
class CREvtModelMap {
public:
    // cf::CREvtObj base region (0x00-0x13)
    /* 0x00 */ void* vtable;        // CREvtModelMap vtable (lbl_eu_80531D80)
    /* 0x04 */ u32 field_04;
    /* 0x08 */ u32 mCallback[3];    // __ptmf triplet (12 bytes)

    // CREvtModel base region (0x14-0x37)
    /* 0x14 */ u32 mField14;
    /* 0x18 */ u32 mFlags;
    /* 0x1C */ void* mPtr1C;
    /* 0x20 */ void* mModel;         // model pointer
    /* 0x24 */ void* mData;          // animation data
    /* 0x28 */ void* mPtr28;
    /* 0x2C */ s32 mField2C;
    /* 0x30 */ s32 mField30;
    /* 0x34 */ s32 mField34;         // animation index counter

    // IWorkEvent subobject + derived fields
    /* 0x38 */ void* vtableIWork;    // IWorkEvent vtable (lbl_eu_80531D80 + 0x44)
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
    /* 0xE4 */ CREvtSceneModel* mEmoteModel;
    /* 0xE8 */ char mModelName[0x100];
    /* 0x1E8 */ s32 mCreatureCount;
    /* 0x1EC */ s32 mCreatureId;
    /* 0x1F4 */ u32 mPtmf[3]; // ptmf triplet (func/this-adj/vtbl-adj)

    // Non-virtual helpers (retail names are C-linkage free functions in the
    // .cpp; these members are legacy/compile-only and not matched).
    bool isMapModel();
    bool isBusy();
    int getGuestState();
    void setGuestModeOn();
};

// CREvtModelMap vtable (0x80531D80; main 0x44 bytes + IWorkEvent 0x80).
extern u8 lbl_eu_80531D80[];
