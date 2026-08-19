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

// Game-manager core object returned by cf::CfGameManager::func_80083298().
class CGameMgrCoreIf {
public:
    virtual void v08();
    virtual void v0C();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1C();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2C();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3C();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4C();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5C();
    virtual void v60();
    virtual void v64();
    virtual void v68();
    virtual void v6C();
    virtual void v70();
    virtual void v74();
    virtual void v78();
    virtual void v7C();
    virtual void v80();
    virtual void v84();
    virtual void v88();
    virtual void v8C();
    virtual void v90();
    virtual void v94();
    virtual void v98();
    virtual void v9C();
    virtual void vA0();
    virtual void vA4();
    virtual void vA8();
    virtual void vAC();
    virtual void vB0();
    virtual void vB4();
    virtual void vB8();
    virtual void vBC();
    virtual void vC0();
    virtual void vC4();
    virtual void vC8();
    virtual void vCC();
    virtual void vD0();
    virtual void vD4();
    virtual void vD8();
    virtual void vDC();
    virtual void vE0();
    virtual void vE4();
    virtual void vE8();
    virtual void vEC();
    virtual void vF0();
    virtual void vF4();
    virtual void vF8();
    virtual void vFC();
    virtual void v100();
    virtual void v104();
    virtual void v108();
    virtual void v10C();
    virtual void v110();
    virtual void v114();
    virtual void v118();
    virtual void v11C();
    virtual void v120();
    virtual void v124();
    virtual void v128();
    virtual void v12C();
    virtual void v130();
    virtual void v134();
    virtual void v138();
    virtual void v13C();
    virtual void v140();
    virtual void v144();
    virtual void v148();
    virtual void v14C();
    virtual void v150();
    virtual void v154();
    virtual void v158(int value);   // vtable +0x158: set guest mode
    virtual void v15C();
    virtual void v160();
    virtual void v164();
    virtual void v168();
    virtual void v16C();
    virtual void v170();
    virtual void v174();
    virtual void v178();
    virtual void v17C();
    virtual void v180();
    virtual void v184();
    virtual void v188();
    virtual void v18C();
    virtual void v190(int value);   // vtable +0x190: set visible
};

// Emote model object (created by func_80495E8C, stored in mEmoteModel).
class CEmoteModelIf {
public:
    virtual void v08();
    virtual void v0C();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1C();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2C();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3C();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4C();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5C();
    virtual void v60();
    virtual void v64();
    virtual void v68();
    virtual void v6C();
    virtual void v70();
    virtual void v74();
    virtual void v78();
    virtual void v7C();
    virtual void v80();
    virtual void v84();
    virtual void v88();
    virtual void v8C();
    virtual void v90();
    virtual void v94();
    virtual void v98();
    virtual void v9C();
    virtual void vA0();
    virtual void vA4();
    virtual void vA8();
    virtual void vAC();
    virtual void vB0();
    virtual void vB4(int value);    // vtable +0xB4: set visible
};

// Fake view of the CREvtModelMap vtable (lbl_eu_80531D80) for slots this TU
// dispatches through (e.g. updatePosition calls vtable+0x30).
class CREvtMapVtIf {
public:
    virtual void v08();
    virtual void v0C();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1C();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2C(int value);    // vtable +0x2C: setVisible
    virtual void v30(int value);    // vtable +0x30
    virtual void v34();
    virtual void v38();
    virtual void v3C();             // vtable +0x3C: cleanup virtual
};

// Creature-list node (circular list whose head lives at CfGameManager +0x04):
// link at +0x00, creature object at +0x08.
class CCreatureIf;
struct CCreatureNode {
    CCreatureNode* next;   // +0x00
    u8 _pad[4];            // +0x04
    CCreatureIf* obj;      // +0x08
};

// Creature object (nodes of the CfGameManager creature list); this TU
// dispatches vtable+0x158 (set visible) and vtable+0x160 (get status byte).
class CCreatureIf {
public:
    virtual void v08(); virtual void v0C(); virtual void v10(); virtual void v14();
    virtual void v18(); virtual void v1C(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2C(); virtual void v30(); virtual void v34();
    virtual void v38(); virtual void v3C(); virtual void v40(); virtual void v44();
    virtual void v48(); virtual void v4C(); virtual void v50(); virtual void v54();
    virtual void v58(); virtual void v5C(); virtual void v60(); virtual void v64();
    virtual void v68(); virtual void v6C(); virtual void v70(); virtual void v74();
    virtual void v78(); virtual void v7C(); virtual void v80(); virtual void v84();
    virtual void v88(); virtual void v8C(); virtual void v90(); virtual void v94();
    virtual void v98(); virtual void v9C(); virtual void vA0(); virtual void vA4();
    virtual void vA8(); virtual void vAC(); virtual void vB0(); virtual void vB4();
    virtual void vB8(); virtual void vBC(); virtual void vC0(); virtual void vC4();
    virtual void vC8(); virtual void vCC(); virtual void vD0(); virtual void vD4();
    virtual void vD8(); virtual void vDC(); virtual void vE0(); virtual void vE4();
    virtual void vE8(); virtual void vEC(); virtual void vF0(); virtual void vF4();
    virtual void vF8(); virtual void vFC(); virtual void v100(); virtual void v104();
    virtual void v108(); virtual void v10C(); virtual void v110(); virtual void v114();
    virtual void v118(); virtual void v11C(); virtual void v120(); virtual void v124();
    virtual void v128(); virtual void v12C(); virtual void v130(); virtual void v134();
    virtual void v138(); virtual void v13C(); virtual void v140(); virtual void v144();
    virtual void v148(); virtual void v14C(); virtual void v150(); virtual void v154();
    virtual void v158(int value);   // vtable +0x158: set visible
    virtual void v15C();
    virtual u8 v160();              // vtable +0x160: get status byte
};

// Emote model object (created by func_80495E8C, stored in mEmoteModel).
// workEvent ORs bit 2 of the word at +0x7A8.
struct CEmoteModelObj {
    u8 _pad[0x7A8];
    u32 field_7A8;   // +0x7A8
};

// Guest-flag word at +0x100 of the game-manager core object returned by
// cf::CfGameManager::func_80083298().
struct CGameMgrCoreFlagsIf {
    u8 _pad[0x100];
    u32 field_100;   // +0x100
};

// View of the parent object stored in CREvtModelMap::mPtr1C (a CREvtModel):
// the packed map token sits at +0x20, a task/state field at +0x30.
struct CREvtModelParentIf {
    u8 _pad[0x20];
    u32 mPacked;     // +0x20
    u8 _pad2[0xC];   // 0x24-0x2F
    s32 mField30;    // +0x30
};

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
    /* 0xE4 */ CEmoteModelObj* mEmoteModel;
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
