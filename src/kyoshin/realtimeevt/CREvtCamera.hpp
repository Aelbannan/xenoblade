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

// Opaque parent representation (14 bytes for cf::CREvtObj)
// Full definition TBD during parent TU decomp.
#define CREVT_CAMERA_PARENT_SIZE 0x14

// Opaque event-file descriptor (mField88 target; +0x04 is a flag word)
struct CREvtCamFile {
    u32 field_00;
    u32 field_04;
};

// Minimal view of cf::CTaskREvent (instance returned by func_801644B4) --
// only the 0x1B8 flag word is accessed from this TU.
struct CREvtCamTaskEvent {
    u8 pad[0x1B8];
    u32 field_1B8;
};

class CREvtCamera {
public:
    // Inherited from cf::CREvtObj (size 0x14)
    u8 unk00[CREVT_CAMERA_PARENT_SIZE];  // +0x00 to +0x13

    // +0x14: unknown (always 0)
    u32 mField14;
    // +0x18: constructor parameter (stores the second arg)
    u32 mField18;
    // +0x1C: pointer to a scene/model object (loaded by func_80180414)
    u32 mField1C;
    // +0x20: pointer to something (freed in destructor)
    u32 mField20;
    // +0x24: pointer to something (freed in func_80180620)
    u32 mField24;

    // +0x28: first 3x4 matrix (MTX34)
    f32 mMatrix28[3][4];
    // +0x58: second 3x4 matrix (MTX34)
    f32 mMatrix58[3][4];

    // +0x88: event-file descriptor pointer (set in func_80180664)
    CREvtCamFile* mField88;
    // +0x8C: float (default from lbl_eu_806678A4, updated from game manager)
    f32 mField8C;
    // +0x90: scene-item handle (set by func_80180414)
    u32 mField90;
    // +0x94: scene-item handle (set by func_80180414)
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
    int func_801809A8() const { return 1; }
    u32 func_801809B0() const { return mField1C; }
};