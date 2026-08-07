#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/device/CFileHandle.hpp"

#include <nw4r/lyt.h>

/*
  Skip-timer widget backing data. Used by CMenuSkipTimer for cutscene skip UI.

  Two layout-compatible (non-polymorphic) classes, matching the CBgTex/CBattery
  pattern: 0x0 holds a manually-set vtable pointer; retail dtors do not reset it
  (so the classes must NOT be virtual here, or MWCC would emit a vtable store).

  CSkipTimer owns a memory region, nw4r layout/animation handles, state flags,
  a CSysWin panel blob (0x3C bytes at +0x34), and a nested CSkipTimer2 sub-
  controller (0x28 bytes at +0x70). Total size 0x98 bytes.
*/

// Layout-compatible access to the color-alpha byte at +0xB8 of an
// nw4r::lyt::Pane (returned by FindPaneByName). Used to mirror the pane
// alpha between the two slot panes in func_8029F504 / func_8029F5CC.
struct PaneAlphaB8 {
    u8 pad[0xB8];   /* 0x000 */
    u8 field_0xB8;  /* 0x0B8 color-alpha byte */
};

// Abstract class matching vtable slot 2 (offset 0x8) of the object that
// CSkipTimer2::mField18 points to. Pure virtual so MWCC emits no vtable.
class UnkObjField18 {
public:
    virtual void vf2(int) = 0;  // slot 2 (0x08) - release-style virtual
};

class CSkipTimer2 {
public:
    CSkipTimer2(void* parent);
    ~CSkipTimer2();

    // --- member fields ---
    /* 0x00 */ void* mVtbl;              // lbl_eu_80539884, set at construction
    /* 0x04 */ UnkClass_8045F564 mMemRegion;  // scratch region for layout build
    /* 0x14 */ void* mParent;            // parent handle, set at construction
    /* 0x18 */ UnkObjField18* mField18;  // object released via vtable[2]
    /* 0x1C */ nw4r::lyt::AnimTransform* mAnimTransform;
    /* 0x20 */ u8 mField20;              // init 0
    /* 0x21 */ u8 mField21;              // init 0
    /* 0x22 */ u8 mField22;              // init 0
    /* 0x23 */ u8 mField23;              // init 1
    /* 0x24 */ u8 mField24;              // init 0
};
// Total size: 0x28 bytes

class CSkipTimer {
public:
    CSkipTimer();
    ~CSkipTimer();

    bool OnFileEvent(CEventFile* pEventFile);

    // Returns the skip-timer active flag at +0x30.
    u8 getActive() const { return mActive; }
    u8 func_8029FF1C() { return mActive; }

    // --- member fields ---
    /* 0x00 */ void* mVtbl;                       // vtable pointer, set at construction
    /* 0x04 */ UnkClass_8045F564 mMemRegion;      // scratch region for layout build
    /* 0x14 */ CFileHandle* mFileHandle;          // file handle, freed in func_8029FE30
    /* 0x18 */ nw4r::lyt::Layout* mLayout;        // layout 1 (pane stamping / accessor release)
    /* 0x1C */ nw4r::lyt::Layout* mLayout2;       // layout 2 (draw + animation binding)
    /* 0x20 */ nw4r::lyt::AnimTransform* mAnimTransform20;
    /* 0x24 */ nw4r::lyt::AnimTransform* mAnimTransform24;
    /* 0x28 */ u8 mField28;                       // init 0
    /* 0x29 */ u8 mField29;                       // init 0
    /* 0x2A */ u8 mField2A;                       // init 0
    /* 0x2B */ u8 mField2B;                       // init 1
    /* 0x2C */ s16 mField2C;                      // init 0 (switch key)
    /* 0x2E */ u16 mField2E;                      // init 0
    /* 0x30 */ u8 mActive;                        // active flag; init 0, read by getActive
    /* 0x31 */ u8 pad31[3];
    /* 0x34 */ u8 mSysWinData[0x3C];              // CSysWin panel blob
    /* 0x70 */ u8 mSkipTimer2Data[0x28];          // CSkipTimer2 sub-controller
};
// Total size: 0x98 bytes (verified via CMenuSkipTimer mTimerData[0x98])