#pragma once

#include <types.h>
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include <nw4r/lyt.h>
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

// Vtable for UI::CPassiveSkillCur (installed by __ct__UI_CPassiveSkillCur).
extern "C" void* lbl_eu_80537A14[];

// Vtable for the skill-info object (installed by __ct__UI_CPassiveSkillInfo).
extern void* lbl_eu_80537A08[];

// Vtable for UI::CPassiveSkill (installed by __ct__UI_CPassiveSkill).
extern void* lbl_eu_80537968[];

// Vtable for UI::CPassiveSkillLine (installed by __ct__UI_CPassiveSkillLine).
extern "C" void* lbl_eu_805379FC[];

namespace UI {

class CPassiveSkillCur {
public:
    /* Cursor object, layout-compatible with CBaseCur plus one extra byte at
     * +0x16. The retail ctor stores its vtable manually (no CBaseCur base
     * call), so the vptr is a plain member here. The step functions
     * (CPassiveSkillCur_stepToState2/CPassiveSkillCur_stepToState3/CPassiveSkillCur_stepToState5) write the state byte at
     * +0x18; CPassiveSkillCur_setRootPos uses the second layout at +0x24 (its root pane is
     * the CBaseCur::SetPaneTranslate target). */
    void* mVtbl;                                // +0x00
    nw4r::lyt::ArcResourceAccessor* mArcResAcc; // +0x04
    nw4r::lyt::Layout* mpLayout;                // +0x08
    nw4r::lyt::AnimTransform* mpAnimTrans0;     // +0x0C
    nw4r::lyt::AnimTransform* mpAnimTrans1;     // +0x10
    u8 mActive;                                 // +0x14
    u8 mVisible;                                // +0x15
    u8 field_16;                                // +0x16
    u8 field_17;                                // +0x17
    u8 field_18;                                // +0x18 - step/state byte
    u8 field_19;                                // +0x19
    u8 field_1A;                                // +0x1A
    u8 _pad1B[0x05];                            // +0x1B..0x1F
    u8 _pad20[0x04];                            // +0x20..0x23 - second-layout holder base (CPassiveSkillCur_releaseBoth)
    nw4r::lyt::Layout* field_24;                // +0x24 - layout whose root pane CPassiveSkillCur_setRootPos drives

    ~CPassiveSkillCur();
};

// Opaque 0x18-byte region at CPassiveSkillInfo+0x3C driven by CPassiveSkillRegion_syncSlotPanes:
// the layout at +0x8 has its root pane searched by the two pane-name
// pointers (lbl_eu_80668908 / lbl_eu_8066890C).
struct UI_PassiveSkillRegion3C {
    u8 _pad0[0x8];                    // +0x00..0x07
    nw4r::lyt::Layout* field_8;       // +0x08 - layout whose root pane is pane-name searched
    u8 _padC[0xC];                    // +0x0C..0x17
};

// 0x20-byte skill slot in the character skill blob (charData+0x3534): a
// word at +0 (non-zero = occupied), a u16 id at +4 (matched by
// CPassiveSkillLine_countSkillId), and a flag byte at +0x14 whose bit 0 is the learned
// marker checked by CPassiveSkillLine_isLearned.
struct CPSkillSlot {
    u32 word;             // +0x00
    u16 id;               // +0x04
    u8 _pad6[0xE];        // +0x06..0x13
    u8 byte14;            // +0x14
    u8 _pad15[0xB];       // +0x15..0x1F
};

// 0xC4-byte skill record: 6 slots (index 1..5 used) + padding.
struct CPSkillRecord {
    CPSkillSlot slots[6]; // +0x00..0xBF
    u8 _padC0[0x4];       // +0xC0..0xC3
};

// Skill blob at charData+0x3534: 5 primary records then 6 secondary ones
// (the secondary area starts at +0x3D4 = 5 * 0xC4; CPassiveSkillLine_isLearned /
// CPassiveSkillLine_isOccupied index records2 by the table row, CPassiveSkillLine_countSkillId scans both).
struct CPSkillBlob {
    CPSkillRecord records[5];   // +0x00..0x3D3
    CPSkillRecord records2[6];  // +0x3D4..0x86B
};

// Skill-blob view for CPassiveSkillLine_setCellMessage: the 5+6 records plus the per-row
// SP-cost table at +0x888 (u32 stride 4, masked to u16 at the read site).
struct CPSkillBlobCost {
    CPSkillRecord records[5];   // +0x00..0x3D3
    CPSkillRecord records2[6];  // +0x3D4..0x86B
    u8 _pad86C[0x18];           // +0x86C..0x883
    u32 totalSP;                // +0x884
    u32 costs[8];               // +0x888
};

// Character-data view for CPassiveSkillLine_refreshSP: the skill blob at +0x3534 of the
// CtrlObjectParam_GetCharEntry result plus the total-SP word at +0x884 past the blob base.
struct CPSkillBlobTotal {
    CPSkillRecord records[5];   // +0x00..0x3D3 (aliases CPSkillBlob.records)
    CPSkillRecord records2[6];  // +0x3D4..0x86B (aliases CPSkillBlob.records2)
    u8 _pad86C[0x18];           // +0x86C..0x883
    u32 totalSP;                // +0x884
};

// 8-byte skill-grid cell embedded at CPassiveSkillLine+0x20 (the ctor's
// memset 0xC8 zeroes the whole 5x5 grid; CPassiveSkillLine_teardown deletes each
// layout through its virtual dtor). The +0x04 slot holds the cell layout's
// animation transform, whose frame CPassiveSkillLine_updateSkillTabs drives via SetFrame.
struct CPSkillGridCell {
    nw4r::lyt::Layout* mpLayout;        // +0x00
    nw4r::lyt::AnimTransform* mpAnimTrans; // +0x04
};

// Skill-learn record view for CPassiveSkill_learnAllSkills's learn-all loop: rows are
// 0xC4 bytes apart starting at charData+0x393C, and bit 0 of the byte at
// +0x393C is the learned marker.
struct CPSkillLearnRecord {
    u8 _pad0[0x393C];    // +0x00..0x393B
    u8 byte393C;         // +0x393C - learned flag (bit 0)
};

class CPassiveSkillInfo {
public:
    CPassiveSkillInfo();
    ~CPassiveSkillInfo();

    // Sized 0x58 so the sibling CCur18 cursor in CPassiveSkillLine lands at
    // +0x178 (0x120 + 0x58). Cursor-compatible head (Layout* at +0x8, as
    // CPassiveSkillCur_setNameMsg135/CPassiveSkillCur_setNameMsg136 read it); the +0x1B/+0x38 gate flags and
    // +0x24 second layout drive the CPassiveSkillInfo_draw draw, and the syswin
    // pointer at +0x54 is read by CPassiveSkillLine_syswinAdvance (aliases
    // UI_CPassiveSkill.field_19C when the line is the +0x28 sub-object).
    u8 _pad0[0x8];                       // +0x00..0x07
    nw4r::lyt::Layout* mpLayout;         // +0x08 - main layout
    u8 _padC[0xD];                       // +0x0C..0x18
    u8 field_19;                         // +0x19 - active flag (read by CPassiveSkillLine_stepStateC/CPassiveSkillLine_stepState4)
    u8 field_1A;                         // +0x1A
    u8 field_1B;                         // +0x1B - draw gate flag
    f32 field_1C;                        // +0x1C - frame float (compared by CPassiveSkillLine_stepState4)
    u8 _pad20[0x4];                      // +0x20..0x23 - UI_PassiveSkillInit base (CPassiveSkillLine_decCharIdx passes +0x140)
    nw4r::lyt::Layout* field_24;         // +0x24 - second layout
    u8 _pad28[0x10];                     // +0x28..0x37
    u8 field_38;                         // +0x38 - draw gate flag
    u8 _pad39[0x3];                      // +0x39..0x3B
    UI_PassiveSkillRegion3C field_3C;    // +0x3C..0x53 - region driven by CPassiveSkillRegion_syncSlotPanes
    CSysWin* field_54;                   // +0x54 - syswin pointer
};

class CPassiveSkillLine {
public:
    ~CPassiveSkillLine();

    void* mVtbl;                         // +0x000 - vtable (lbl_eu_805379FC, set by the ctor)
    void* mArg;                          // +0x004 - caller arg (accessor, set by the ctor)
    nw4r::lyt::Layout* field_8;          // +0x008 - layout driven by CPassiveSkillLine_enterState4
    nw4r::lyt::AnimTransform* field_C;   // +0x00C - anim transform checked by CPassiveSkillLine_stepToState7, bound by CPassiveSkillLine_startClose
    nw4r::lyt::AnimTransform* field_10;  // +0x010 - anim transform bound/enabled by CPassiveSkillLine_enterState4
    nw4r::lyt::AnimTransform* field_14;  // +0x014 - anim transform unbound by CPassiveSkillLine_startClose
    nw4r::lyt::Layout* field_18;         // +0x018 - secondary layout (CPassiveSkillLine_enterState9)
    nw4r::lyt::AnimTransform* field_1C;  // +0x01C - secondary anim transform (CPassiveSkillLine_enterState9)
    CPSkillGridCell cells[5][5];         // +0x020..0x0E7 - 5x5 skill grid (zeroed by ctor memset 0xC8)
    u8 field_E8;                         // +0x0E8 - menu state byte (set to 4 by CPassiveSkillLine_enterState4)
    u8 field_E9;                         // +0x0E9 - menu state byte
    u8 field_EA[0x8];                    // +0x0EA..0x0F1 - grid column skill ids (CPassiveSkillLine_isLearned/CPassiveSkillLine_isOccupied)
    u8 field_F2;                         // +0x0F2 - zeroed by the ctor
    u8 field_F3;                         // +0x0F3 - countdown byte (CPassiveSkillLine_decCharIdx)
    u8 field_F4;                         // +0x0F4 - zeroed by the ctor
    u8 field_F5;                         // +0x0F5 - cleared by CPassiveSkillLine_decCharIdx
    u8 field_F6;                         // +0x0F6 - cleared by CPassiveSkillLine_decCharIdx
    u8 field_F7;                         // +0x0F7 - skill grid row byte (cleared by CPassiveSkillLine_stepState14)
    u8 field_F8;                         // +0x0F8 - skill grid col byte (cleared by CPassiveSkillLine_stepState14)
    u8 field_F9;                         // +0x0F9 - zeroed by the ctor
    u8 field_FA;                         // +0x0FA - zeroed by the ctor
    u8 field_FB;                         // +0x0FB - tab-availability flag (read by CPassiveSkillLine_cursorRight/CPassiveSkillLine_cursorLeft)
    u8 field_FC;                         // +0x0FC - tab-availability flag (read by CPassiveSkillLine_cursorRight/CPassiveSkillLine_cursorLeft)
    u8 field_FD;                         // +0x0FD - cleared by CPassiveSkillLine_decCharIdx
    u8 field_FE;                         // +0x0FE - cleared by CPassiveSkillLine_decCharIdx
    u8 _padFF[0x1];                      // +0x0FF
    f32 field_100;                       // +0x100 - frame float (lbl_eu_80668904, set by the ctor)
    u8 field_104[0x19];                  // +0x104..0x11C - per-cell affordable/learned flag (row*5+i-1)
    u8 field_11D;                        // +0x11D - set to 1 by CPassiveSkillLine_closeMoveCur
    u8 field_11E;                        // +0x11E - close-mode selector (1 -> state 0xC, else 0xF)
    u8 field_11F;                        // +0x11F - zeroed by the ctor
    CPassiveSkillInfo mInfo;             // +0x120 - embedded info sub-object
    CCur18 mCur;                         // +0x178 - embedded CCur18 cursor
};

class CPassiveSkill {
public:
    ~CPassiveSkill();

    void* mVtbl;                 // +0x000 - lbl_eu_80537968 (set by __ct__UI_CPassiveSkill)
    u8 _pad4[0x4];               // +0x004..0x007
    UnkClass_8045F564 mUnk8;     // +0x008 - embedded mem-region class (0x10 bytes)
    u32 field_18;                // +0x018
    u32 field_1C;                // +0x01C
    u32 field_20;                // +0x020
    u8 field_24;                 // +0x024
    u8 field_25;                 // +0x025
    u8 field_26;                 // +0x026
    u8 field_27;                 // +0x027
    CPassiveSkillLine mLine;     // +0x028 - embedded line sub-object (0x190 bytes)
    CSysWin mSysWin;             // +0x1B8 - embedded system window
};

} // namespace UI

void CPassiveSkillLine_incCharIdx(UI::CPassiveSkillLine* self);
void CPassiveSkillLine_cursorDown(UI::CPassiveSkillLine* self);
void CPassiveSkillLine_cursorRight(UI::CPassiveSkillLine* self);

// Offset view used by the layout-release helper
// pointer it drops sits at +0x4. CPassiveSkillCur_releaseBoth passes the +0x20 region of a
// CPassiveSkillCur, so field_4 aliases that object's +0x24 layout.
struct UI_PassiveSkillLayoutRef {
    u8 _pad0[0x4];                  // +0x00
    nw4r::lyt::Layout* field_4;     // +0x04
};

// Layout-release helpers for the skill cursors (retail symbols unmangled).
extern "C" void CPassiveSkillCur_releaseLayout(UI::CPassiveSkillCur* self);
extern "C" void CPassiveSkillLayoutRef_release(UI_PassiveSkillLayoutRef* self);
extern "C" void CPassiveSkillCur_releaseBoth(UI::CPassiveSkillCur* self);

// Abstract view into the embedded CSysWin's vtable for CPassiveSkill_loadSkillData: retail
// dispatches vtable +0x88 (index 34, no args) after the temp-window copy.
// All-pure so no vtable is emitted (same scheme as COptionSysWinView).
class UI_CPassiveSkillSysWinView {
public:
    // v00/v01 omitted: retail slots 0/1 are the RTTI pair MWCC prepends for
    // this view, so class-index 0 is the third retail slot.
    virtual void v02() = 0;
    virtual void v03() = 0;
    virtual void v04() = 0;
    virtual void v05() = 0;
    virtual void v06() = 0;
    virtual void v07() = 0;
    virtual void v08() = 0;
    virtual void v09() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual void v12() = 0;
    virtual void v13() = 0;
    virtual void v14() = 0;
    virtual void v15() = 0;
    virtual void v16() = 0;
    virtual void v17() = 0;
    virtual void v18() = 0;
    virtual void v19() = 0;
    virtual void v20() = 0;
    virtual void v21() = 0;
    virtual void v22() = 0;
    virtual void v23() = 0;
    virtual void v24() = 0;
    virtual void v25() = 0;
    virtual void v26() = 0;
    virtual void v27() = 0;
    virtual void v28() = 0;
    virtual void v29() = 0;
    virtual void v30() = 0;
    virtual void v31() = 0;
    virtual void v32() = 0;
    virtual void v33() = 0;
    virtual void v34() = 0; // class-index 32 -> retail vtable +0x88 (MWCC adds the RTTI pair)
};
struct UI_PassiveSkillInit;
// Skill-info animation helpers dispatched by CPassiveSkillInit_step (retail symbols
// unmangled).
extern "C" void CPassiveSkillInit_stepMode0(UI_PassiveSkillInit* self);
extern "C" void CPassiveSkillInit_stepMode1(UI_PassiveSkillInit* self);
// Skill-info pane-animation trigger (retail symbol unmangled); called by
// CPassiveSkillLine_decCharIdx.
extern "C" void CPassiveSkillInit_triggerPane10(UI_PassiveSkillInit* self);

// Skill-name pane-text setter for the cursor (retail symbol unmangled); called
// by CPassiveSkillLine_enterState4.
extern "C" void CPassiveSkillCur_setNameMsg135(UI::CPassiveSkillCur* self);

/* 0x1C-byte structure initialized by CPassiveSkillInit_init: a caller-provided pointer
 * at +0x0, zeroed words, and byte flags (+0x19 starts at 1). The +0x8/+0xC/
 * +0x10/+0x14 words are pointers: +0x8/+0xC are anim transforms (frames at
 * +0x10 enabled/set by CPassiveSkillInit_stepMode1), and +0x10/+0x14 are panes shown via
 * setPaneVisible. */
struct UI_PassiveSkillInit {
    u32 field_0;                     // +0x00 - caller pointer
    nw4r::lyt::Layout* field_4;      // +0x04 - layout animated via vtbl 0x38 by CPassiveSkillInit_step
    nw4r::lyt::AnimTransform* field_8; // +0x08 - anim transform enabled/frame-set by CPassiveSkillInit_stepMode1
    nw4r::lyt::AnimTransform* field_C; // +0x0C - anim transform whose frame is set
    nw4r::lyt::Pane* field_10;       // +0x10 - pane toggled by CPassiveSkillInit_triggerPane10
    nw4r::lyt::Pane* field_14;       // +0x14 - pane toggled by CPassiveSkillInit_triggerPane14
    u8 field_18;                     // +0x18
    u8 field_19;                     // +0x19 - active flag (init 1)
    u8 field_1A;                     // +0x1A
    u8 field_1B;                     // +0x1B - mode byte
};

// 0x18-byte layout-init view for CPassiveSkillLayoutInit_init: arc accessor at +0x0, the
// layout/anim-transform outputs at +0x4/+0x8/+0xC, and the two pane finders
// at +0x10/+0x14.
struct UI_PassiveSkillLayoutInit {
    nw4r::lyt::ArcResourceAccessor* mArcResAcc; // +0x00
    nw4r::lyt::Layout* mpLayout;                // +0x04
    nw4r::lyt::AnimTransform* mpAnimTrans0;     // +0x08
    nw4r::lyt::AnimTransform* mpAnimTrans1;     // +0x0C
    nw4r::lyt::Pane* field_10;                  // +0x10
    nw4r::lyt::Pane* field_14;                  // +0x14
};

// Object constructed by __ct__UI_CPassiveSkillInfo: a vtable head at +0x0, a
// caller arg at +0x4, zeroed words, byte flags (+0x19..0x1B start at 1), a
// frame float at +0x1C, and a CPassiveSkillInit_init-initialized sub-struct at +0x20.
class UI_CPassiveSkillInfo {
public:
    void* vptr;                 // +0x00 - lbl_eu_80537A08
    u32 arg;                    // +0x04 - caller pointer
    nw4r::lyt::Layout* field_8; // +0x08 - layout animated by CPassiveSkillInfo_stepToState3/CPassiveSkillInfo_stepToState3Alt
    u32 field_C;                // +0x0C
    u32 field_10;               // +0x10
    nw4r::lyt::AnimTransform* field_14; // +0x14 - anim transform stepped by CPassiveSkillInfo_stepToState3/CPassiveSkillInfo_stepToState3Alt
    u8 field_18;                // +0x18
    u8 field_19;                // +0x19
    u8 field_1A;                // +0x1A
    u8 field_1B;                // +0x1B
    f32 field_1C;               // +0x1C - frame float
    UI_PassiveSkillInit sub;    // +0x20 - init via CPassiveSkillInit_init
};

// Step-cursor object driven by CPassiveSkillStep_bindState6/CPassiveSkillStep_bindState7: a layout at +0x8
// with an animation transform at +0x14, plus state bytes at +0x18/+0x19.
struct UI_PassiveSkillStep {
    u8 _pad0[0x8];                       // +0x00..0x07
    nw4r::lyt::Layout* mpLayout;         // +0x08
    u8 _padC[0x8];                       // +0x0C..0x13
    nw4r::lyt::AnimTransform* field_14;  // +0x14
    u8 field_18;                         // +0x18 - step state byte
    u8 field_19;                         // +0x19
};

// AnimTransform frame value written by the skill-init animation trigger
// (retail .sdata2 float; value resolved at link time).
extern const float lbl_eu_80668904;

// Frame value passed to advanceAnimTransform/AnimRewindFrame by the cursor steps
// (retail .sdata2 float; value resolved at link time).
extern const float lbl_eu_80668900;

// Closing-marker float compared against the info frame float by
// CPassiveSkillLine_stepState4 (retail .sdata2 float; value resolved at link time).
extern const float lbl_eu_80668918;

// Pane-name pointers for the CPassiveSkillRegion_syncSlotPanes two-slot toggle (retail .sdata2
// const data; values resolved at link time).
extern const char* const lbl_eu_80668908;
extern const char* const lbl_eu_8066890C;

class UI_CPassiveSkill {
public:
    void thunk28_67CE0();
    void thunk28_67F88();
    void thunk28_67BA0();
    void thunk28_68518();
    void thunk28_68C38();

    /* Skill-menu UI object. Field offsets recovered from retail access
     * patterns in this TU. The +0x28 sub-object is where the thunk28_*
     * methods forward (their funcs take it as `self`). */
    u8 _pad0[0x24];      // 0x000
    u8 field_24;         // 0x024 - visibility gate flag
    u8 field_25;         // 0x025 - visibility gate flag
    u8 field_26;         // 0x026
    u8 field_27;         // 0x027 - cleared by CPassiveSkill_open
    u8 _pad28[0xC0];     // 0x028 - sub-object region (thunk28_* target)
    u8 field_E8;         // 0x0E8 - menu state
    u8 field_E9;         // 0x0E9 - menu state
    u8 _padEA[0x0D];     // 0x0EA
    s8 field_F7;         // 0x0F7 - cursor row (signed)
    s8 field_F8;         // 0x0F8 - cursor column (signed)
    u8 _padF9[0x0B];     // 0x0F9
    u8 field_104[0x35];  // 0x104 - skill grid data (53 cells)
    u8 field_139;        // 0x139 - visibility gate flag
    u8 field_13A;        // 0x13A - mode flag
    u8 _pad13B[0x45];    // 0x13B
    u8 field_180;        // 0x180
    u8 _pad181[0x1B];    // 0x181
    u8* field_19C;       // 0x19C - lazily-set pointer to the +0x1B8 CSysWin
    u8 _pad1A0[0x18];    // 0x1A0
    u8 field_1B8[0x40];  // 0x1B8 - CSysWin sub-object region (thunk1b8 target)
};

void CPassiveSkill_update(UI_CPassiveSkill* self);
int CPassiveSkill_getVisibleIfReady(UI_CPassiveSkill* self);
int CPassiveSkill_getActiveIfBusy(UI_CPassiveSkill* self);
u32 CPassiveSkill_cellAtLeast8(u8* self);
void CPassiveSkill_open(UI_CPassiveSkill* self);
void CPassiveSkill_close(UI_CPassiveSkill* self);
void CPassiveSkill_confirm(UI_CPassiveSkill* self);
u8 CPassiveSkill_pickOpenState(UI_CPassiveSkill* self);
s32 CPassiveSkill_isSyswinOrLearned(UI_CPassiveSkill* self);

void UI_CPassiveSkill_thunk28_68594(u8* self);
void UI_CPassiveSkill_thunk28_67E2C(u8* self);
void UI_CPassiveSkill_thunk28_68250(u8* self);
void UI_CPassiveSkill_thunk28_67C44(u8* self);
void UI_CPassiveSkill_thunk28_68518(u8* self);

class CPassiveSkill {
public:
    void OnFileEvent();

    // TODO: add fields
};

// C-ABI CSysWin helper (defined in CSysWin.cpp; CSysWinSelect.hpp's copy can't
// be included here because its CScn clashes with harness_catalog.hpp's). The
// retail symbol is unmangled, so the declaration needs C linkage to make the
// call reloc plain `CSysWin_isReady` (same pattern as CItemBoxLine.hpp).
// CSysWin sub-object constructor (defined in CSysWin.cpp as a C-linkage free
// function; retail symbol __ct__7CSysWinFUc). Called by __ct__UI_CPassiveSkill.
extern "C" void __ct__7CSysWinFUc(CSysWin* self, int arg);


// CSysWin window-state helper (CSysWin.cpp). Retail symbol unmangled.

// CSysWin window-active helper (CSysWin.cpp). Retail symbol unmangled.

// Cursor-position builder for the syswin (CSysWin.cpp, retail symbol
// unmangled); writes a VEC3 that the CCur18 "Move" virtual consumes.

// Cursor quiet helper (CCur.cpp, takes the embedded CCur18 cursor). Retail
// symbol unmangled.

// Cursor per-frame update (CCur.cpp; retail symbol unmangled).

// CSysWin window advance/close helper (CSysWin.cpp). Retail symbol unmangled.

// CSysWin draw helper (CSysWin.cpp). Retail symbol unmangled.

// CSysWin advance helper (CSysWin.cpp; retail symbol unmangled).

// +0x1B8 sub-object state query (retail UI_CPassiveSkill_thunk1b8_CSysWin_getUnk34).
// Defined as an extern "C" global so call sites emit the plain retail reloc
// name (the C++ member mangling would drift at the reloc gate).
extern "C" int UI_CPassiveSkill_thunk1b8_CSysWin_getUnk34(UI_CPassiveSkill* self);

// Shared string table (split .rodata) used by CPassiveSkillCur_setNameMsg135's pane-text lookup.
extern char lbl_eu_8050DC20[];

// Skill-row table (split .rodata) indexed by (id-1)*8 + grid-entry - 1 by
// CPassiveSkillLine_isLearned / CPassiveSkillLine_isOccupied; the byte selects the character-data record
// row (CtrlObjectParam_GetCharEntry result + 0x3534).
extern const u8 sPassiveSkillCharRows[0x40];

// C-library vararg formatter (flat retail symbol; same convention as
// CUICfManager.hpp / CArtsInfo.hpp).
extern "C" int sprintf(char* str, const char* fmt, ...);

// View into the object returned by CDeviceFont::getFontInfo: vtable+0x24
// (index 7, no args) yields the u32 pushed onto the root pane by
// ApplyFontToTxtPanes. All-pure so no vtable is emitted (CMapSel.hpp / COption.hpp
// carry the same view under different class names).
class CDeviceFontView {
public:
    virtual void vf0() = 0; // index 0 -> +0x08
    virtual void vf1() = 0; // index 1 -> +0x0C
    virtual void vf2() = 0; // index 2 -> +0x10
    virtual void vf3() = 0; // index 3 -> +0x14
    virtual void vf4() = 0; // index 4 -> +0x18
    virtual void vf5() = 0; // index 5 -> +0x1C
    virtual void vf6() = 0; // index 6 -> +0x20
    virtual u32 vf7() = 0;  // index 7 -> +0x24
};

// CDeviceFont font-create helper (retail symbol is the literal mangled
// identifier; extern "C" so calls bind to it, same convention as
// CItemBoxGrid.hpp / COption.hpp).
extern "C" void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32 arg, nw4r::lyt::Layout* layout);

// Character/party name-text value pushed into layout panes by setLayoutTextBoxFont
// (retail symbol unmangled; same convention as CPartyState.hpp).

// Texture-dimension record read by CPassiveSkillInfo_init: the 'timg' texture
// resource's +0x8 chain pointer leads (via +0x0) to a pair of u16 dims.
struct CPSkillTexCoords {
    u16 c0;   // +0x00
    u16 c2;   // +0x02
};

// Chain record at CPSkillTexRes+0x8: its +0x0 gives the coord record above.
struct CPSkillTexChain {
    CPSkillTexCoords* pCoords;   // +0x00
};

// Head of the 'timg' texture resource: the +0x8 pointer is the chain record.
struct CPSkillTexRes {
    u8 _pad0[0x8];            // +0x00..0x07
    CPSkillTexChain* chain;   // +0x08
};

// Cursor draw helper (CCur.cpp; retail symbol unmangled).


// Pane-animation colour fetch (retail symbol unmangled): reads the 4 s16
// animation vector of a named pane (index 0 = base, 1 = alternate).

// RGB-colour builder: declared once with C++ linkage in kyoshin/CTitle.hpp;
// callers include that header instead of carrying a local extern copy.

// Bind a texture resource to a pane (retail symbol unmangled).

// Pane colour setter (retail symbol unmangled): applies the two colours to
// the pane (CItemBoxLine.hpp convention; PaneMatSetTevColorsByName is the layout form).

// Pane byte at +0xB8 written by CPassiveSkillLine_updateLearnGrid's cell update (opaque state).
struct CPSkillPaneB8 {
    u8 _pad0[0xB8];   // +0x00..0xB7
    u8 field_B8;      // +0xB8
};

// setPaneVisible (declared in CQuestWindow_Pane.hpp) sets a pane's visible
// flag; isPaneVisible queries it and is declared once in CTitleAHelp.hpp.

// Message-table lookup used by CPassiveSkillLine_setSlotNamePanes (retail symbol unmangled):
// returns the u16 id for (table, label, category).

// Character-data lookup by id (retail symbol unmangled; also declared in
// CMapSel.hpp / CItemBoxGrid.hpp).
#include "kyoshin/cf/CtrlRemoteApi.hpp"

// Character index splitter used by CPassiveSkill_learnAllSkills (retail symbol unmangled;
// 3-arg form also declared in CPcKizunagram.hpp).

// Shared BDAT character table pointer (.sdata; value resolved at link time).

// Shared arc resource accessor (retail symbol unmangled; also in CPresentWin.hpp).

// Skill grid data tables (.sdata pointers; values resolved at link time).
// gpPassiveSkillBdat is passed as the message table to BdatGetU16Direct by
// CPassiveSkillLine_setSlotNamePanes; CPassiveSkillLine_refreshLearnGrid reads all three (80664880 name lookup,
// 80664888 slot-category lookup, 8066488C learned-slot lookup).
extern void* gpPassiveSkillBdat;
extern void* gpPassiveSkillHelpBdat;
extern void* gpPassiveSkillIconBdat;
extern void* gpPassiveSkillValueBdat;
extern void* gpPassiveSkillCharBdat;
extern void* gpPassiveSkillEffectBdat;

// Character-data view for CPassiveSkillLine_updateSkillTabs: the current-grid-row counter word
// at +0x3DD0 of the CtrlObjectParam_GetCharEntry result (compared against the loop row).
struct CPSkillCharData3DD0 {
    u8 _pad0[0x3DD0];   // +0x00..0x3DCF
    u32 field_3DD0;     // +0x3DD0
};

// Frame-bound floats used by CPassiveSkillLine_stepState14's window check (retail .sdata2
// floats; values resolved at link time).
extern const float lbl_eu_8066891C;
extern const float lbl_eu_80668920;

// Per-character slot-spacing table read by CPassiveSkillLine_refreshLearnGrid (retail .sdata2
// floats, indexed by (id-1)*2 to reach an 8-byte per-character stride;
// only the first float of each pair is read).
extern const f32 lbl_eu_80668930[];

// +0x28 sub-object update (retail func_80269B68, 0x410 bytes). C linkage so the
// call reloc from CPassiveSkill_open matches retail's plain `CPassiveSkillLine_openRefresh` name
// (MWCC would otherwise mangle the C++ reference to CPassiveSkillLine_openRefresh__FPUc).
extern "C" void CPassiveSkillLine_openRefresh(UI::CPassiveSkillLine* self);

// Pane-position helper (defined in code_80135FDC.cpp; retail symbol
// unmangled): writes the ancestor-translate sum for a pane into output.
// Used by CPassiveSkillLine_refreshGridPositions to position the skill-grid cell panes.

// .sdata2 int->double magic constants (0x4330000000000000 unsigned and
// 0x4330000080000000 signed-conversion variants) used by the s32/u32->f64
// conversions in CPassiveSkillLine_updateSkillTabs and the alpha ladder in CPassiveSkillLine_refreshGridAlpha.
extern const f64 lbl_eu_80668910;
extern const f64 lbl_eu_80668928;
extern const f32 lbl_eu_8066893C;
extern const f32 lbl_eu_80668940;

// .sdata2 category-filter word + trailing byte read by CPassiveSkillLine_syncEquipState: the
// loop's category byte for grid index i is bytes[i] of the word and the
// 5th category comes from the byte.
extern u32 lbl_eu_80668934;
extern u8 lbl_eu_80668938;

// 3-word scene-name table (.rodata) copied to the stack by CPassiveSkillLine_syncEquipState
// and indexed by the +0xF3 countdown byte (when < 3).
extern const u32 sPassiveSkillSceneNames[3];

// Character-data view for CPassiveSkillLine_syncEquipState: the five s16 equipment ids at
// +0x1C..+0x24 (categories 4..8) on the CtrlObjectParam_GetCharEntry result.
struct CPSkillCharData {
    u8 _pad0[0x1C];   // +0x00..0x1B
    s16 equip4;       // +0x1C
    s16 equip5;       // +0x1E
    s16 equip6;       // +0x20
    s16 equip7;       // +0x22
    s16 equip8;       // +0x24
};

// Equipment lookup helpers for CPassiveSkillLine_syncEquipState (retail symbols unmangled;
// same flat declarations as CItemBoxInfo.hpp / CEquipChange.hpp).
extern "C" void CtrlObjectParam_SetEquipSlot0(void*, int);
extern "C" void CtrlObjectParam_SetEquipSlot1(void*, int);
extern "C" void CtrlObjectParam_SetEquipSlot4(void*, int);
extern "C" void CtrlObjectParam_SyncParamFromActor(void*);

namespace cf {
class CfObjectMove;
class CfObjEnumList;
}

// One slot of the CfObjEnumList element array (getEntryAt result): the
// CfObjectMove pointer sits at +0x04.
struct CPSkillEnumListSlot {
    u8 _pad0[0x4];             // +0x00..0x03
    cf::CfObjectMove* move;    // +0x04
};

// CfObjEnumList element-count field at +0x620 (get__16CfEnumListHolderFv result).
struct CPSkillEnumListCount {
    u8 _pad0[0x620];           // +0x00..0x61F
    u32 count;                 // +0x620
};

// cf enum-list helpers used by CPassiveSkillLine_syncEquipState (retail symbols unmangled;
// declared with typed holders in this header only - no co-included header
// re-declares them).
extern "C" void startEnumObjects(void*, u32, u32);
extern "C" CPSkillEnumListSlot* getEntryAt(void*, u32);
extern "C" cf::CfObjectMove* getCfObjectPc__FPQ22cf12CfObjectMove(cf::CfObjectMove* move);
namespace cf { class CfObjectPc; }
void CfObjectPc_setupFrame(cf::CfObjectPc* obj);
typedef void (*CfObjectPcSetupFrameFn)(cf::CfObjectPc*, u32);  // DEBT(callcast): retail passes live r4 to 1-arg CfObjectPc_setupFrame (mangled arity 1; LOSTFIX proof)

// 5-byte category filter copied from the .sdata2 constants by CPassiveSkillLine_syncEquipState:
// the word is stored as a unit (lwz/stw) then the trailing byte (stb), and
// the loop reads it back as bytes.
union CPSkillCatFilter {
    struct {
        u32 word;    // +0x00
        u8 byte4;    // +0x04
    } w;
    u8 bytes[5];     // +0x00..0x04
};

// First word of a CItem_kindPtr item record: the top 12 bits are the item
// category (CPassiveSkillLine_syncEquipState reads them via BdatGetItemId).
struct CPSkillItem {
    u32 word;    // +0x00
};

// u32 word-pair / f64 view for MWCC's 0x43300000 u8->float conversion: the
// source stores the magic word + value, then subtracts the named .sdata2
// magic double lbl_eu_80668910 so the pool reloc matches retail instead of
// an MWCC-synthesised @N entry (CfObjectImplMove.hpp convention).
union CPSkillF64Conv {
    u32 w[2];
    f64 d;
};
