#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/CSysWin.hpp"

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
// alpha between the two slot panes in CSkipTimer2OnFwdDone / CSkipTimer2OnRewindDone.
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
    CSkipTimer2(void* parent = nullptr);
    virtual ~CSkipTimer2();

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

class CSkipTimer : public IWorkEvent {
public:
    CSkipTimer();
    virtual ~CSkipTimer();

    virtual bool OnFileEvent(CEventFile* pEventFile);

    // Returns the skip-timer active flag at +0x30.
    u8 getActive() const { return mActive; }

    // --- member fields ---
    /* 0x04 */ UnkClass_8045F564 mMemRegion;      // scratch region for layout build
    /* 0x14 */ CFileHandle* mFileHandle;          // file handle, freed in CSkipTimerTeardown
    /* 0x18 */ nw4r::lyt::Layout* mLayout;        // layout 1 (pane stamping / accessor release)
    /* 0x1C */ nw4r::lyt::Layout* mLayout2;       // layout 2 (draw + animation binding)
    /* 0x20 */ nw4r::lyt::AnimTransform* mAnimTransform20;
    /* 0x24 */ nw4r::lyt::AnimTransform* mAnimTransform24;
    /* 0x28 */ u8 mField28;                       // init 0
    /* 0x29 */ u8 mField29;                       // init 0
    /* 0x2A */ u8 mField2A;                       // init 0
    /* 0x2B */ u8 mField2B;                       // init 1
    /* 0x2C */ s16 mField2C;                      // init 0 (switch key)
    /* 0x2E */ s16 mField2E;                      // init 0
    /* 0x30 */ u8 mActive;                        // active flag; init 0, read by getActive
    /* 0x34 */ CSysWin mSysWin;
    /* 0x70 */ CSkipTimer2 mSkipTimer2;
    void OnFwdGate();
    void ReengageInput();
    void EndSkip();
    void OnReverseDone();
    void ActivateSkipUI();
    void LockSkipButton();
};
// Total size: 0x98 bytes (verified via CMenuSkipTimer mTimerData[0x98])

// Free functions owned by this TU. Retail emits these with bare C names, so
// each has one C-linkage declaration here rather than a per-caller copy.
extern "C" void CSkipTimer2RebuildLayout(CSkipTimer2* self);
extern "C" void CSkipTimer2DriveState(CSkipTimer2* self);
extern "C" void CSkipTimer2Reset(CSkipTimer2* self);
extern "C" void CSkipTimer2Start(CSkipTimer2* self, u8 arg);
extern "C" void CSkipTimer2HoldRelease(CSkipTimer2* self, u8 arg);
extern "C" void CSkipTimerStampSlotText(CSkipTimer2* self, int arg1, int arg2);
extern "C" void CSkipTimer2OnFwdDone(CSkipTimer2* self);
extern "C" void CSkipTimer2OnRewindDone(CSkipTimer2* self);
extern "C" void func_8029F6EC(CSkipTimer2* self);
extern "C" void CSkipTimer2OnAdvanceEnd(CSkipTimer2* self);
extern "C" void CSkipTimer2MarkActive(CSkipTimer2* self);
extern "C" void CSkipTimer2RebindAnim(CSkipTimer2* self);
extern "C" void CSkipTimer2SelectSlotPane(CSkipTimer2* self, u8 arg);

extern "C" void CSkipTimer_BeginLoad(CSkipTimer* self);
extern "C" void CSkipTimerTick(CSkipTimer* self);
extern "C" void CSkipTimerDraw(CSkipTimer* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void CSkipTimerTeardown(CSkipTimer* self);
extern "C" int CSkipTimerIsReady(CSkipTimer* self);
extern "C" int CSkipTimerGetSkipButton(CSkipTimer* self);
extern "C" int isSkipActive(CSkipTimer* self);
extern "C" void CSkipTimerNextKey(CSkipTimer* self);
extern "C" void CSkipTimerPrevKey(CSkipTimer* self);
extern "C" void CSkipTimerFlagButton(CSkipTimer* self);
extern "C" void CSkipTimerLeaveSkip(CSkipTimer* self);
extern "C" void CSkipTimerConfirmSkip(CSkipTimer* self);
extern "C" void CSkipTimerEngageSkip(CSkipTimer* self);
extern "C" int CSkipTimerIsEngaged(CSkipTimer* self);
extern "C" void CSkipTimerActivateTimer(CSkipTimer* self);
extern "C" void CSkipTimer_UpdateSlotFrame(CSkipTimer* self);
extern "C" int CSkipTimerMapKeyToAction(CSkipTimer* self);
extern "C" void CSkipTimerBindAnim20(CSkipTimer* self);
extern "C" void CSkipTimerBindAnim24(CSkipTimer* self);
extern "C" void sinit_802A07D8();
