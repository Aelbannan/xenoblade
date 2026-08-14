#pragma once

#include <types.h>
#include <nw4r/lyt.h>
#include "kyoshin/CSysWin.hpp"

// Vtable for UI::CPassiveSkillCur (installed by __ct__UI_CPassiveSkillCur).
extern "C" void* lbl_eu_80537A14[];

// Vtable for the skill-info object (installed by __ct__UI_CPassiveSkillInfo).
extern void* lbl_eu_80537A08[];

// Vtable for UI::CPassiveSkill (installed by __ct__UI_CPassiveSkill).
extern void* lbl_eu_80537968[];

// Minimal view of the CCur18 cursor sub-object (CBaseCur layout + padding,
// 0x18 bytes). Only the destructor is referenced from this TU (external
// retail symbol __dt__6CCur18Fv); the retail cursor code lives in its own TU.
class CCur18 {
public:
    ~CCur18();
    u8 _data[0x18];
};

namespace UI {

class CPassiveSkillCur {
public:
    /* Cursor object, layout-compatible with CBaseCur plus one extra byte at
     * +0x16. The retail ctor stores its vtable manually (no CBaseCur base
     * call), so the vptr is a plain member here. The step functions
     * (func_80266294/func_80266344/func_802663C0) write the state byte at
     * +0x18; func_80266250 uses the second layout at +0x24 (its root pane is
     * the func_801D2150 target). */
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
    u8 _pad20[0x04];                            // +0x20..0x23 - second-layout holder base (func_80264C58)
    nw4r::lyt::Layout* field_24;                // +0x24 - layout whose root pane func_80266250 drives

    ~CPassiveSkillCur();
};

class CPassiveSkillInfo {
public:
    CPassiveSkillInfo();
    ~CPassiveSkillInfo();

    // Sized 0x58 so the sibling CCur18 cursor in CPassiveSkillLine lands at
    // +0x178 (0x120 + 0x58). Cursor-compatible head (Layout* at +0x8, as
    // func_802661A8/func_802661FC read it); the +0x1B/+0x38 gate flags and
    // +0x24 second layout drive the func_80264BE4 draw, and the syswin
    // pointer at +0x54 is read by func_80268F7C (aliases
    // UI_CPassiveSkill.field_19C when the line is the +0x28 sub-object).
    u8 _pad0[0x8];                       // +0x00..0x07
    nw4r::lyt::Layout* mpLayout;         // +0x08 - main layout
    u8 _padC[0xF];                       // +0x0C..0x1A
    u8 field_1B;                         // +0x1B - draw gate flag
    u8 _pad1C[0x8];                      // +0x1C..0x23
    nw4r::lyt::Layout* field_24;         // +0x24 - second layout
    u8 _pad28[0x10];                     // +0x28..0x37
    u8 field_38;                         // +0x38 - draw gate flag
    u8 _pad39[0x3];                      // +0x39..0x3B
    u8 field_3C[0x18];                   // +0x3C..0x53 - opaque region passed to func_802640B8
    CSysWin* field_54;                   // +0x54 - syswin pointer
};

class CPassiveSkillLine {
public:
    ~CPassiveSkillLine();

    u8 _pad0[0x8];                       // +0x000..0x007
    nw4r::lyt::Layout* field_8;          // +0x008 - layout driven by func_802698B8
    u8 _padC[0x4];                       // +0x00C..0x00F
    nw4r::lyt::AnimTransform* field_10;  // +0x010 - anim transform bound/enabled by func_802698B8
    u8 _pad14[0x4];                      // +0x014..0x017
    nw4r::lyt::Layout* field_18;         // +0x018 - secondary layout (func_802699A4)
    nw4r::lyt::AnimTransform* field_1C;  // +0x01C - secondary anim transform (func_802699A4)
    u8 _pad20[0xC8];                     // +0x020..0x0E7
    u8 field_E8;                         // +0x0E8 - menu state byte (set to 4 by func_802698B8)
    u8 field_E9;                         // +0x0E9 - menu state byte
    u8 _padEA[0x36];                     // +0x0EA..0x11F
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

// Offset view used by the layout-release helper func_80264344: the layout
// pointer it drops sits at +0x4. func_80264C58 passes the +0x20 region of a
// CPassiveSkillCur, so field_4 aliases that object's +0x24 layout.
struct UI_PassiveSkillLayoutRef {
    u8 _pad0[0x4];                  // +0x00
    nw4r::lyt::Layout* field_4;     // +0x04
};

// Layout-release helpers for the skill cursors (retail symbols unmangled).
extern "C" void func_80264060(UI::CPassiveSkillCur* self);
extern "C" void func_80264344(UI_PassiveSkillLayoutRef* self);
extern "C" void func_80264C58(UI::CPassiveSkillCur* self);

struct UI_PassiveSkillInit;
// Skill-info animation helpers dispatched by func_8026439C (retail symbols
// unmangled).
extern "C" void func_802644D4(UI_PassiveSkillInit* self);
extern "C" void func_80264588(UI_PassiveSkillInit* self);

// Skill-name pane-text setter for the cursor (retail symbol unmangled); called
// by func_802698B8.
extern "C" void func_802661A8(UI::CPassiveSkillCur* self);

/* 0x1C-byte structure initialized by func_802641D0: a caller-provided pointer
 * at +0x0, zeroed words, and byte flags (+0x19 starts at 1). The +0xC/+0x10/
 * +0x14 words are pointers: +0xC targets the layout's AnimTransform (its
 * mFrame at +0x10 is written by func_8026440C/func_80264470), and +0x10/+0x14
 * are panes shown via func_80124270. */
struct UI_PassiveSkillInit {
    u32 field_0;                     // +0x00 - caller pointer
    nw4r::lyt::Layout* field_4;      // +0x04 - layout animated via vtbl 0x38 by func_8026439C
    u32 field_8;                     // +0x08
    nw4r::lyt::AnimTransform* field_C; // +0x0C - anim transform whose frame is set
    nw4r::lyt::Pane* field_10;       // +0x10 - pane toggled by func_80264470
    nw4r::lyt::Pane* field_14;       // +0x14 - pane toggled by func_8026440C
    u8 field_18;                     // +0x18
    u8 field_19;                     // +0x19 - active flag (init 1)
    u8 field_1A;                     // +0x1A
    u8 field_1B;                     // +0x1B - mode byte
};

// Object constructed by __ct__UI_CPassiveSkillInfo: a vtable head at +0x0, a
// caller arg at +0x4, zeroed words, byte flags (+0x19..0x1B start at 1), a
// frame float at +0x1C, and a func_802641D0-initialized sub-struct at +0x20.
class UI_CPassiveSkillInfo {
public:
    void* vptr;                 // +0x00 - lbl_eu_80537A08
    u32 arg;                    // +0x04 - caller pointer
    u32 field_8;                // +0x08
    u32 field_C;                // +0x0C
    u32 field_10;               // +0x10
    u32 field_14;               // +0x14
    u8 field_18;                // +0x18
    u8 field_19;                // +0x19
    u8 field_1A;                // +0x1A
    u8 field_1B;                // +0x1B
    f32 field_1C;               // +0x1C - frame float
    UI_PassiveSkillInit sub;    // +0x20 - init via func_802641D0
};

// Step-cursor object driven by func_80264D98/func_80264E04: a layout at +0x8
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

// Frame value passed to func_80137444/func_80137510 by the cursor steps
// (retail .sdata2 float; value resolved at link time).
extern const float lbl_eu_80668900;

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
    u8 field_27;         // 0x027 - cleared by func_8026DA4C
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
// function; retail symbol __ct__CSysWin). Called by __ct__UI_CPassiveSkill.
extern "C" void __ct__CSysWin(CSysWin* self, int arg);

extern "C" u32 CSysWin_isReady(void* syswin);

// CSysWin window-state helper (CSysWin.cpp). Retail symbol unmangled.
extern "C" int CSysWin_getUnk34(void* syswin);

// CSysWin window-active helper (CSysWin.cpp). Retail symbol unmangled.
extern "C" int CSysWin_isActive(CSysWin* syswin);

// Cursor quiet helper (CCur.cpp, takes the embedded CCur18 cursor). Retail
// symbol unmangled.
extern "C" void func_801D216C(CCur18* cursor, u8 flag);

// CSysWin window advance/close helper (CSysWin.cpp). Retail symbol unmangled.
extern "C" void func_8022B8E4(CSysWin* syswin);

// +0x1B8 sub-object state query (retail UI_CPassiveSkill_thunk1b8_CSysWin_getUnk34).
// Defined as an extern "C" global so call sites emit the plain retail reloc
// name (the C++ member mangling would drift at the reloc gate).
extern "C" __declspec(noinline) int UI_CPassiveSkill_thunk1b8_CSysWin_getUnk34(UI_CPassiveSkill* self);

// Shared string table (split .rodata) used by func_802661A8's pane-text lookup.
extern char lbl_eu_8050DC20[];

// +0x28 sub-object update (retail func_80269B68, 0x410 bytes). C linkage so the
// call reloc from func_8026DA4C matches retail's plain `func_802676F8` name
// (MWCC would otherwise mangle the C++ reference to func_802676F8__FPUc).
extern "C" void func_802676F8(u8* self);
