#pragma once

/**
 * @file CREvtCamera.hpp
 * CREvtCamera -- realtime event camera class.
 *
 * Class hierarchy (namespace cf):
 *   cf::CREvtObj  (vtable 0x80532320, size 0x14)
 *     +0x00: vtable pointer
 *     +0x04: u32 field (set to 0 in CREvtCamera's constructor)
 *     +0x08: __ptmf (12 bytes, initialized to null)
 *     +0x14: .. derived class fields start here
 *   +-- CREvtCamera  (vtable 0x80531CE8, size >= 0x448)
 *
 * A global singleton pointer is stored at lbl_eu_806642A8.
 */

#include <types.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
#include "kyoshin/realtimeevt/CREvtObj.hpp"
#include "libs/monolib/src/scn/CScnItemModel.hpp"

// Opaque event-file descriptor (mField88 target; +0x04 is a flag word)
struct CREvtCamFile {
    u32 field_00;
    u32 field_04;
};

// Minimal view of cf::CTaskREvent (instance returned by evtGetManagerAddr) --
// only the 0x1B8 flag word is accessed from this TU.
struct CREvtCamTaskEvent {
    u8 pad[0x1B8];
    u32 field_1B8;
};

// Camera manager returned by cf::CfGameManager::getCameraDataBlock() (the
// retail object behind the forward-declared UnkClass_800821F8). Only slots
// 0x14 (set camera position), 0x3C (set fov) and 0x58 (get fov) are
// dispatched from this TU; sibling pluginCam.cpp drives the same object
// (setPosition/setLookAt/setDirection/setRotation/setFov/setTarget/offset
// setters). Slot 0x10 is a CREvtCamera-side virtual (retail word:
// REvtCam_ClearAnimHandle); self-calls go through cf::CREvtObj::vfunc_10, not here.
class CREvtCamManager {
public:
    virtual void v08();
    virtual void v0C();
    virtual void v10();
    virtual void setPosition(const f32* pos);  // +0x14: set camera position
    virtual void v18();
    virtual void v1C();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2C();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void setFov(f32 value);  // +0x3C: set float (fov)
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4C();
    virtual void v50();
    virtual void v54();
    virtual f32 getFov();            // +0x58: get float
    u8 field_0x4[0x0C - 0x4];
    u32 field_0x0C;                      // +0x0C: player object pointer
};

// Scene model object stored in CREvtCamera::mField1C (created by
// Scn_SetupAnim) is a CScnItemModel: slot 0x3C is vfunc3C (animation handle
// by name) and +0x7A8 is flags7A8. Call sites use CScnItemModel directly;
// no TU-local view is emitted.

// Camera object returned by Scn_FindCamItem (camera state block).
struct CREvtCamObj {
    u8 field_0x0[0x1E0];
    f32 field_0x1E0;
    u8 field_0x1E4[0x1EC - 0x1E4];
    f32 field_0x1EC;
    f32 field_0x1F0;
};

// Player object reachable via CREvtCamManager::field_0x0C (CfObjectMove).
struct CREvtPlayerObj {
    u8 field_0x0[0x1EC];
    f32 field_0x1EC;
    f32 field_0x1F0;
};

namespace nw4r {
namespace g3d {
struct ChrAnmResult;
}
} // namespace nw4r::g3d

class CREvtCamera {
public:
    // Inherited from cf::CREvtObj (size 0x14)
    /* 0x00 */ u8* vtable;
    /* 0x04 */ u32 field_04;
    /* 0x08 */ u32 ptmf[3];

    // +0x14: unknown (always 0)
    u32 mField14;
    // +0x18: constructor parameter (stores the second arg)
    u32 mField18;
    // +0x1C: pointer to a scene/model object (loaded by REvtCam_SetupSceneModel)
    u32 mField1C;
    // +0x20: pointer to something (freed in destructor)
    u32 mField20;
    // +0x24: pointer to something (freed in REvtCam_ClearAnimHandle)
    u32 mField24;

    // +0x28: first 3x4 matrix (MTX34)
    f32 mMatrix28[3][4];
    // +0x58: second 3x4 matrix (MTX34)
    f32 mMatrix58[3][4];

    // +0x88: event-file descriptor pointer (set in func_80180664)
    CREvtCamFile* mField88;
    // +0x8C: float (default from lbl_eu_806678A4, updated from game manager)
    f32 mField8C;
    // +0x90: scene-item handle (set by REvtCam_SetupSceneModel)
    u32 mField90;
    // +0x94: scene-item handle (set by REvtCam_SetupSceneModel)
    u32 mField94;
    // +0x98: task-related counter/flag
    u32 mField98;
    // +0x9C: task-related counter/flag
    u32 mField9C;
    // +0xA0: task-related counter/flag
    u32 mFieldA0;
    // +0xA4: byte flag (set to 0/1)
    u8 mFieldA4;
    // +0xA5: byte flag (set in func_80180664)
    u8 mFieldA5;

    // +0xA6: array of short task IDs (count at mTaskCount2A8)
    s16 mTaskArrayA6[128];
    // +0x1A6: array of short task IDs (count at mTaskCount2AC)
    s16 mTaskArray1A6[128];

    // +0x2A8: count for mTaskArrayA6
    u32 mTaskCount2A8;
    // +0x2AC: count for mTaskArray1A6
    u32 mTaskCount2AC;
    // +0x2B0: some state/counter (checked against 1 and 2)
    u32 mField2B0;

    // +0x2B4: per-task flag byte array (indexed by mTaskCount434,
    //         associated with mTaskArray334)
    u8 mFlagArray2B4[128];
    // +0x334: array of short task IDs (count at mTaskCount434)
    s16 mTaskArray334[128];

    // +0x434: count for mTaskArray334 / mFlagArray2B4
    u32 mTaskCount434;
    // +0x438: byte flag (set to 0/1)
    u8 mField438;

    // +0x43C: some integer (initialized to -1)
    s32 mField43C;
    // +0x440: float (default from lbl_eu_806678A8, updated from game manager)
    f32 mField440;
    // +0x444: float (default from lbl_eu_806678AC, updated from game manager)
    f32 mField444;

    // Inline helpers (already FULL_MATCH)
    int REvtCam_AlwaysTrue1() const { return 1; }
    u32 REvtCam_GetField1CValue() const { return mField1C; }
};

// Retail data symbols referenced by this TU (imports; global-scope externs are
// unmangled by MWCC, so no extern "C" is needed here).
extern unsigned long lbl_eu_806642A8;  // CREvtCamera* singleton
// +0x00: "CamEvent" string pool (offsets used as sub-string names)
extern const char lbl_eu_805036D8[];
// +0x00: "Cam2", +0x04: "Cam" animation names
// (const char* string pointers)
extern const char* lbl_eu_80662448;
extern const char* lbl_eu_8066244C;
// CREvtCamera vtable (0x80531CE8)
extern u8 lbl_eu_80531CE8[];
// Default floats (camera fov / player fields)
extern f32 lbl_eu_806678A4;
extern f32 lbl_eu_806678A8;
extern f32 lbl_eu_806678AC;
extern f32 lbl_eu_806678B0;
// Double constant used by the manual u32->f64 bit trick in func_801804CC
// (0x4330000080000000).
extern f64 lbl_eu_806678B8;
