#pragma once

#include <types.h>
#include <nw4r/lyt.h>
#include <nw4r/math/math_types.h>

#include "monolib/work/IWorkEvent.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/CEventFile.hpp"

class CTagProcessor;

/*
 * CSysWin - system message window widget.
 *
 * One IWorkEvent-derived window that owns a System.arc layout plus the shared
 * tag processor. The window runs a four-phase open/close animation state
 * machine (field_35) and switches the visible label pane by window kind (field_37).
 *
 * Layout (0x3C bytes):
 *   +0x00  IWorkEvent vptr
 *   +0x04  mMemRegion (UnkClass_8045F564, 4 words) - scratch region for the
 *          layout build
 *   +0x14  mFileHandle   - loaded System.arc file handle
 *   +0x18  mTagProcessor - window tag processor (allocated in OnFileEvent)
 *   +0x1C  mArcAccessor  - System.arc resource accessor
 *   +0x20  mLayout
 *   +0x24  mAnimTrans    - open/close animation transform
 *   +0x28  field_28        - 1 in the ctor, 0 while a load is pending, 1 after
 *                          setupAfterLoad
 *   +0x29  3 pad bytes
 *   +0x2C  field_2C        - "nul_close00" label pane found by setupAfterLoad
 *   +0x30  field_30        - "nul_slct" label pane found by setupAfterLoad
 *   +0x34  field_34         - 1 while the window is open
 *   +0x35  field_35        - 0 idle, 1 opening, 2 open/advance, 3 closing
 *   +0x36  field_36       - 1 while the window is drawn/animating
 *   +0x37  field_37         - window kind selected by setKindContent/switchKindPane
 *   +0x38  field_38  - window kind applied by applyKind
 *   +0x39  field_39  - classic-controller flag latched by applyKind
 *
 * Virtuals (retail vtable slot order): ~CSysWin (+0x08), OnFileEvent (+0x10)
 * from IWorkEvent's slot 2, and loadSystemArc appended last (+0x88).
 */
class CSysWin : public IWorkEvent {
public:
    CSysWin(u8 kind);
    virtual ~CSysWin();
    virtual bool OnFileEvent(CEventFile* pEventFile);
    virtual void loadSystemArc();

    // State flags (out of line: retail emits each as `lbz r3, off; blr`).
    // The gate/mask shape retail's callers use (plain `cmpi` on the return
    // register) means these must return int, not u8.
    int isReady();      // +0x28
    int isOpened();     // +0x34
    int isActive();     // +0x36

    void dispatchPhase();
    void drawLayout(nw4r::lyt::DrawInfo* drawInfo);
    void termLayout();
    void openPhase1();
    void advancePhase3();
    void switchKindPane(int kind);
    void setKindContent(char* textA, char* textB);
    void setTwoTextValues(char* textA, char* textB);
    void applyKind(u8 kind);
    void phase1To2Advance();
    void syncKindAdvance();
    void rewindToPhase0();
    void setupAfterLoad();

    // Screen position of the idx-th "txt_slct%02d" pane of `window`, scaled by
    // its "nul_proportion" pane. Retail passes the destination first and the
    // window second (r3/r4), so this is a static member, not a `this` method.
    static void getPaneScreenPos(nw4r::math::VEC3* out, CSysWin* window, u8 idx);

    UnkClass_8045F564 mMemRegion;     // +0x04 - scratch region for layout build
    CFileHandle* mFileHandle;         // +0x14 - loaded System.arc file handle
    CTagProcessor* mTagProcessor;     // +0x18 - window tag processor (allocated in OnFileEvent)
    nw4r::lyt::ArcResourceAccessor* mArcAccessor;  // +0x1C - System.arc resource accessor
    nw4r::lyt::Layout* mLayout;       // +0x20
    nw4r::lyt::AnimTransform* mAnimTrans;  // +0x24
    u8 field_28;                      // +0x28 - ready flag (1 in ctor, 0 after load)
    nw4r::lyt::Pane* field_2C;        // +0x2C - "nul_close00" label pane
    nw4r::lyt::Pane* field_30;        // +0x30 - "nul_slct" label pane
    u8 field_34;                      // +0x34 - open flag
    u8 field_35;                      // +0x35 - anim phase (0 idle, 1 open, 2 advance, 3 rewind)
    u8 field_36;                      // +0x36 - drawn/active flag
    u8 field_37;                      // +0x37 - ctor arg / selected window kind
    u8 field_38;                      // +0x38 - window kind applied by applyKind (ctor init 5)
    u8 field_39;                      // +0x39 - classic-controller flag
};
