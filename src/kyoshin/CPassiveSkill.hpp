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

// Vtable for UI::CPassiveSkillLine (installed by __ct__UI_CPassiveSkillLine).
extern "C" void* lbl_eu_805379FC[];

// Minimal view of the CCur18 cursor sub-object (CBaseCur layout + padding,
// 0x18 bytes). Only the destructor is referenced from this TU (external
// retail symbol __dt__6CCur18Fv); the retail cursor code lives in its own TU.
class CCur18 {
public:
    ~CCur18();
    u8 _data[0x18];
};

// CCur18 constructor (retail symbol __ct__CCur18, defined in CCur.cpp).
// extern "C" so the call reloc from __ct__UI_CPassiveSkillLine is the plain
// retail name (CMapSel.hpp also declares it, but that header can't be
// included here because its CCur18View clashes with the one above).
extern "C" void __ct__CCur18(void*, nw4r::lyt::ArcResourceAccessor*);

// Abstract view into the embedded CCur18 cursor vtable (same scheme as
// CTitle.hpp/CMapSel.hpp). MWCC prepends offset-to-top + RTTI (2 entries),
// so vtable offset = (index + 2) * 4; index 2 -> +0x10 is the "Move"
// virtual that repositions the cursor (takes a VEC3*).
class CCur18View {
public:
    virtual void vf02() = 0;                        // index 0 -> +0x08
    virtual void vf03(const u8*) = 0;               // index 1 -> +0x0C
    virtual void vf04(const nw4r::math::VEC3*) = 0; // index 2 -> +0x10 - Move
};

// No-arg view of the CCur18 vtable for the teardown dispatch at +0x0C in
// func_802675D8: retail leaves r4 unset at the call site, so the virtual
// takes no arguments.
class CCur18TearView {
public:
    virtual void vf02() = 0;  // index 0 -> +0x08
    virtual void vf03() = 0;  // index 1 -> +0x0C
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

// Opaque 0x18-byte region at CPassiveSkillInfo+0x3C driven by func_802640B8:
// the layout at +0x8 has its root pane searched by the two pane-name
// pointers (lbl_eu_80668908 / lbl_eu_8066890C).
struct UI_PassiveSkillRegion3C {
    u8 _pad0[0x8];                    // +0x00..0x07
    nw4r::lyt::Layout* field_8;       // +0x08 - layout whose root pane is pane-name searched
    u8 _padC[0xC];                    // +0x0C..0x17
};

// 0x20-byte skill slot in the character skill blob (charData+0x3534): a
// word at +0 (non-zero = occupied), a u16 id at +4 (matched by
// func_8026D3CC), and a flag byte at +0x14 whose bit 0 is the learned
// marker checked by func_8026CC58.
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
// (the secondary area starts at +0x3D4 = 5 * 0xC4; func_8026CC58 /
// func_8026CD44 index records2 by the table row, func_8026D3CC scans both).
struct CPSkillBlob {
    CPSkillRecord records[5];   // +0x00..0x3D3
    CPSkillRecord records2[6];  // +0x3D4..0x86B
};

// Skill-blob view for func_80266724: the 5+6 records plus the per-row
// SP-cost table at +0x888 (u32 stride 4, masked to u16 at the read site).
struct CPSkillBlobCost {
    CPSkillRecord records[5];   // +0x00..0x3D3
    CPSkillRecord records2[6];  // +0x3D4..0x86B
    u8 _pad86C[0x18];           // +0x86C..0x883
    u32 totalSP;                // +0x884
    u32 costs[8];               // +0x888
};

// Character-data view for func_802665FC: the skill blob at +0x3534 of the
// func_8009EC9C result plus the total-SP word at +0x884 past the blob base.
struct CPSkillBlobTotal {
    CPSkillRecord records[5];   // +0x00..0x3D3 (aliases CPSkillBlob.records)
    CPSkillRecord records2[6];  // +0x3D4..0x86B (aliases CPSkillBlob.records2)
    u8 _pad86C[0x18];           // +0x86C..0x883
    u32 totalSP;                // +0x884
};

// 8-byte skill-grid cell embedded at CPassiveSkillLine+0x20 (the ctor's
// memset 0xC8 zeroes the whole 5x5 grid; func_802675D8 deletes each
// layout through its virtual dtor).
struct CPSkillGridCell {
    nw4r::lyt::Layout* mpLayout; // +0x00
    u32 _pad4;                   // +0x04
};

// Skill-learn record view for func_8026DD84's learn-all loop: rows are
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
    // func_802661A8/func_802661FC read it); the +0x1B/+0x38 gate flags and
    // +0x24 second layout drive the func_80264BE4 draw, and the syswin
    // pointer at +0x54 is read by func_80268F7C (aliases
    // UI_CPassiveSkill.field_19C when the line is the +0x28 sub-object).
    u8 _pad0[0x8];                       // +0x00..0x07
    nw4r::lyt::Layout* mpLayout;         // +0x08 - main layout
    u8 _padC[0xD];                       // +0x0C..0x18
    u8 field_19;                         // +0x19 - active flag (read by func_802696D8/func_80269370)
    u8 field_1A;                         // +0x1A
    u8 field_1B;                         // +0x1B - draw gate flag
    f32 field_1C;                        // +0x1C - frame float (compared by func_80269370)
    u8 _pad20[0x4];                      // +0x20..0x23 - UI_PassiveSkillInit base (func_80267C44 passes +0x140)
    nw4r::lyt::Layout* field_24;         // +0x24 - second layout
    u8 _pad28[0x10];                     // +0x28..0x37
    u8 field_38;                         // +0x38 - draw gate flag
    u8 _pad39[0x3];                      // +0x39..0x3B
    UI_PassiveSkillRegion3C field_3C;    // +0x3C..0x53 - region driven by func_802640B8
    CSysWin* field_54;                   // +0x54 - syswin pointer
};

class CPassiveSkillLine {
public:
    ~CPassiveSkillLine();

    void* mVtbl;                         // +0x000 - vtable (lbl_eu_805379FC, set by the ctor)
    void* mArg;                          // +0x004 - caller arg (accessor, set by the ctor)
    nw4r::lyt::Layout* field_8;          // +0x008 - layout driven by func_802698B8
    nw4r::lyt::AnimTransform* field_C;   // +0x00C - anim transform checked by func_8026916C, bound by func_80267B08
    nw4r::lyt::AnimTransform* field_10;  // +0x010 - anim transform bound/enabled by func_802698B8
    nw4r::lyt::AnimTransform* field_14;  // +0x014 - anim transform unbound by func_80267B08
    nw4r::lyt::Layout* field_18;         // +0x018 - secondary layout (func_802699A4)
    nw4r::lyt::AnimTransform* field_1C;  // +0x01C - secondary anim transform (func_802699A4)
    CPSkillGridCell cells[5][5];         // +0x020..0x0E7 - 5x5 skill grid (zeroed by ctor memset 0xC8)
    u8 field_E8;                         // +0x0E8 - menu state byte (set to 4 by func_802698B8)
    u8 field_E9;                         // +0x0E9 - menu state byte
    u8 field_EA[0x8];                    // +0x0EA..0x0F1 - grid column skill ids (func_8026CC58/func_8026CD44)
    u8 field_F2;                         // +0x0F2 - zeroed by the ctor
    u8 field_F3;                         // +0x0F3 - countdown byte (func_80267C44)
    u8 field_F4;                         // +0x0F4 - zeroed by the ctor
    u8 field_F5;                         // +0x0F5 - cleared by func_80267C44
    u8 field_F6;                         // +0x0F6 - cleared by func_80267C44
    u8 field_F7;                         // +0x0F7 - skill grid row byte (cleared by func_802694F4)
    u8 field_F8;                         // +0x0F8 - skill grid col byte (cleared by func_802694F4)
    u8 field_F9;                         // +0x0F9 - zeroed by the ctor
    u8 field_FA;                         // +0x0FA - zeroed by the ctor
    u8 field_FB;                         // +0x0FB - tab-availability flag (read by func_80267F88/func_80268250)
    u8 field_FC;                         // +0x0FC - tab-availability flag (read by func_80267F88/func_80268250)
    u8 field_FD;                         // +0x0FD - cleared by func_80267C44
    u8 field_FE;                         // +0x0FE - cleared by func_80267C44
    u8 _padFF[0x1];                      // +0x0FF
    f32 field_100;                       // +0x100 - frame float (lbl_eu_80668904, set by the ctor)
    u8 _pad104[0x19];                    // +0x104..0x11C
    u8 field_11D;                        // +0x11D - set to 1 by func_80269768
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
// Skill-info pane-animation trigger (retail symbol unmangled); called by
// func_80267C44.
extern "C" void func_80264470(UI_PassiveSkillInit* self);

// Skill-name pane-text setter for the cursor (retail symbol unmangled); called
// by func_802698B8.
extern "C" void func_802661A8(UI::CPassiveSkillCur* self);

/* 0x1C-byte structure initialized by func_802641D0: a caller-provided pointer
 * at +0x0, zeroed words, and byte flags (+0x19 starts at 1). The +0x8/+0xC/
 * +0x10/+0x14 words are pointers: +0x8/+0xC are anim transforms (frames at
 * +0x10 enabled/set by func_80264588), and +0x10/+0x14 are panes shown via
 * func_80124270. */
struct UI_PassiveSkillInit {
    u32 field_0;                     // +0x00 - caller pointer
    nw4r::lyt::Layout* field_4;      // +0x04 - layout animated via vtbl 0x38 by func_8026439C
    nw4r::lyt::AnimTransform* field_8; // +0x08 - anim transform enabled/frame-set by func_80264588
    nw4r::lyt::AnimTransform* field_C; // +0x0C - anim transform whose frame is set
    nw4r::lyt::Pane* field_10;       // +0x10 - pane toggled by func_80264470
    nw4r::lyt::Pane* field_14;       // +0x14 - pane toggled by func_8026440C
    u8 field_18;                     // +0x18
    u8 field_19;                     // +0x19 - active flag (init 1)
    u8 field_1A;                     // +0x1A
    u8 field_1B;                     // +0x1B - mode byte
};

// 0x18-byte layout-init view for func_80264204: arc accessor at +0x0, the
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
// frame float at +0x1C, and a func_802641D0-initialized sub-struct at +0x20.
class UI_CPassiveSkillInfo {
public:
    void* vptr;                 // +0x00 - lbl_eu_80537A08
    u32 arg;                    // +0x04 - caller pointer
    nw4r::lyt::Layout* field_8; // +0x08 - layout animated by func_802664EC/func_80266574
    u32 field_C;                // +0x0C
    u32 field_10;               // +0x10
    nw4r::lyt::AnimTransform* field_14; // +0x14 - anim transform stepped by func_802664EC/func_80266574
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

// Closing-marker float compared against the info frame float by
// func_80269370 (retail .sdata2 float; value resolved at link time).
extern const float lbl_eu_80668918;

// Pane-name pointers for the func_802640B8 two-slot toggle (retail .sdata2
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

// Cursor-position builder for the syswin (CSysWin.cpp, retail symbol
// unmangled); writes a VEC3 that the CCur18 "Move" virtual consumes.
extern "C" void func_8022C1B4(nw4r::math::VEC3* out, CSysWin* syswin, int idx);

// Cursor quiet helper (CCur.cpp, takes the embedded CCur18 cursor). Retail
// symbol unmangled.
extern "C" void func_801D216C(CCur18* cursor, u8 flag);

// Cursor per-frame update (CCur.cpp; retail symbol unmangled).
extern "C" void func_801D202C(void* cursor);

// CSysWin window advance/close helper (CSysWin.cpp). Retail symbol unmangled.
extern "C" void func_8022B8E4(CSysWin* syswin);

// CSysWin draw helper (CSysWin.cpp). Retail symbol unmangled.
extern "C" void func_8022B7C8(CSysWin* syswin, nw4r::lyt::DrawInfo* drawInfo);

// CSysWin advance helper (CSysWin.cpp; retail symbol unmangled).
extern "C" void func_8022B748(void* syswin);

// +0x1B8 sub-object state query (retail UI_CPassiveSkill_thunk1b8_CSysWin_getUnk34).
// Defined as an extern "C" global so call sites emit the plain retail reloc
// name (the C++ member mangling would drift at the reloc gate).
extern "C" __declspec(noinline) int UI_CPassiveSkill_thunk1b8_CSysWin_getUnk34(UI_CPassiveSkill* self);

// Shared string table (split .rodata) used by func_802661A8's pane-text lookup.
extern char lbl_eu_8050DC20[];

// Skill-row table (split .rodata) indexed by (id-1)*8 + grid-entry - 1 by
// func_8026CC58 / func_8026CD44; the byte selects the character-data record
// row (func_8009EC9C result + 0x3534).
extern u8 lbl_eu_8050DB60[];

// C-library vararg formatter (flat retail symbol; same convention as
// CUICfManager.hpp / CArtsInfo.hpp).
extern "C" int sprintf(char* str, const char* fmt, ...);

// View into the object returned by CDeviceFont::func_80452C10: vtable+0x24
// (index 7, no args) yields the u32 pushed onto the root pane by
// func_8013676C. All-pure so no vtable is emitted (CMapSel.hpp / COption.hpp
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
extern "C" void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32 arg, nw4r::lyt::Layout* layout);

// Character/party name-text value pushed into layout panes by func_801368C0
// (retail symbol unmangled; same convention as CPartyState.hpp).
extern "C" u32 func_801355BC();

// Texture-dimension record read by func_802646E8: the 'timg' texture
// resource's +0x8 pointer points to a pair of u16 dimensions.
struct CPSkillTexCoords {
    u16 c0;   // +0x00
    u16 c2;   // +0x02
};

// Head of the 'timg' texture resource: the +0x8 pointer gives the dimension
// record above.
struct CPSkillTexRes {
    u8 _pad0[0x8];            // +0x00..0x07
    CPSkillTexCoords* coords; // +0x08
};

// Cursor draw helper (CCur.cpp; retail symbol unmangled).
extern "C" void func_801D20B0(void* cursor, nw4r::lyt::DrawInfo* drawInfo);

// Cursor/skill-pane visibility helpers (retail symbols unmangled).
// func_80124270 (declared in CSysWin.hpp) sets a pane's visible flag;
// func_801C4648 queries it. C linkage so call relocs are the plain retail
// name (CTitleAHelp.hpp's C++ declaration would mangle it).
extern "C" bool func_801C4648(nw4r::lyt::Pane*);

// Message-table lookup used by func_80266950 (retail symbol unmangled):
// returns the u16 id for (table, label, category).
extern "C" u16 func_80136254(const void*, const void*, int);

// Character-data lookup by id (retail symbol unmangled; also declared in
// CMapSel.hpp / CItemBoxGrid.hpp).
extern "C" u32 func_8009CF8C(u32);

// Character index splitter used by func_8026DD84 (retail symbol unmangled;
// 3-arg form also declared in CPcKizunagram.hpp).
extern "C" void func_8013AB0C(u8*, u8*, int);

// Shared BDAT character table pointer (.sdata; value resolved at link time).
extern void* lbl_eu_80664090;

// Shared arc resource accessor (retail symbol unmangled; also in CPresentWin.hpp).
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4();

// Skill grid data table (.sdata pointer; value resolved at link time),
// passed as the message table to func_80136254 by func_80266950.
extern void* lbl_eu_8066488C;

// Frame-bound floats used by func_802694F4's window check (retail .sdata2
// floats; values resolved at link time).
extern const float lbl_eu_8066891C;
extern const float lbl_eu_80668920;

// +0x28 sub-object update (retail func_80269B68, 0x410 bytes). C linkage so the
// call reloc from func_8026DA4C matches retail's plain `func_802676F8` name
// (MWCC would otherwise mangle the C++ reference to func_802676F8__FPUc).
extern "C" void func_802676F8(u8* self);

// Pane-position helper (defined in code_80135FDC.cpp; retail symbol
// unmangled): writes the ancestor-translate sum for a pane into output.
// Used by func_8026D080 to position the skill-grid cell panes.
extern "C" void func_8013775C(nw4r::math::VEC3* output, nw4r::lyt::Pane* node);

// .sdata2 int->double magic constant (0x4330000000000000) and the frame
// scale divisor/multiplier used by func_8026D210's alpha ladder.
extern const f64 lbl_eu_80668910;
extern const f32 lbl_eu_8066893C;
extern const f32 lbl_eu_80668940;

// .sdata2 category-filter word + trailing byte read by func_8026CE30: the
// loop's category byte for grid index i is bytes[i] of the word and the
// 5th category comes from the byte.
extern u32 lbl_eu_80668934;
extern u8 lbl_eu_80668938;

// 3-word scene-name table (.rodata) copied to the stack by func_8026CE30
// and indexed by the +0xF3 countdown byte (when < 3).
extern u32 lbl_eu_8050DBB8[3];

// Character-data view for func_8026CE30: the five s16 equipment ids at
// +0x1C..+0x24 (categories 4..8) on the func_8009EC9C result.
struct CPSkillCharData {
    u8 _pad0[0x1C];   // +0x00..0x1B
    s16 equip4;       // +0x1C
    s16 equip5;       // +0x1E
    s16 equip6;       // +0x20
    s16 equip7;       // +0x22
    s16 equip8;       // +0x24
};

// Equipment lookup helpers for func_8026CE30 (retail symbols unmangled;
// same flat declarations as CItemBoxInfo.hpp / CEquipChange.hpp).
extern "C" void* func_80157C4C(u32 index, s16 value);
extern "C" u16 func_80139358(u32 value);
extern "C" void func_8009E024(void*, int);
extern "C" void func_8009E030(void*, int);
extern "C" void func_8009E03C(void*, int);
extern "C" void func_8009E048(void*, int);
extern "C" void func_8009E054(void*, int);
extern "C" void func_800A1370(void*);

namespace cf {
class CfObjectMove;
class CfObjEnumList;
}

// 8-byte enum-list holder around a cf::CfObjEnumList (func_80043D90 ctor /
// func_80043F18 accessor / __dt__80043E88 dtor); same scheme as
// CfEnumListHolder in CTaskGame.hpp / pluginUi.hpp.
struct CPSkillEnumListHolder {
    cf::CfObjEnumList* list;   // +0x00
    u32 handle;                // +0x04
};

// One slot of the CfObjEnumList element array (func_800F6EC0 result): the
// CfObjectMove pointer sits at +0x04.
struct CPSkillEnumListSlot {
    u8 _pad0[0x4];             // +0x00..0x03
    cf::CfObjectMove* move;    // +0x04
};

// CfObjEnumList element-count field at +0x620 (func_80043F18 result).
struct CPSkillEnumListCount {
    u8 _pad0[0x620];           // +0x00..0x61F
    u32 count;                 // +0x620
};

// cf enum-list helpers used by func_8026CE30 (retail symbols unmangled;
// declared with typed holders in this header only - no co-included header
// re-declares them).
extern "C" void func_80043D90(CPSkillEnumListHolder* holder);
extern "C" cf::CfObjEnumList* func_80043F18(CPSkillEnumListHolder* holder);
extern "C" void __dt__80043E88(CPSkillEnumListHolder* holder, int flags);
extern "C" void func_800F4A98(cf::CfObjEnumList* list, u32 type, u32 filter);
extern "C" CPSkillEnumListSlot* func_800F6EC0(cf::CfObjEnumList* list, u32 index);
extern "C" cf::CfObjectMove* func_800BFC68__FPQ22cf12CfObjectMove(cf::CfObjectMove* move);
extern "C" void func_800BFDE0(cf::CfObjectMove* move, u32 flag);

// 5-byte category filter copied from the .sdata2 constants by func_8026CE30:
// the word is stored as a unit (lwz/stw) then the trailing byte (stb), and
// the loop reads it back as bytes.
union CPSkillCatFilter {
    struct {
        u32 word;    // +0x00
        u8 byte4;    // +0x04
    } w;
    u8 bytes[5];     // +0x00..0x04
};

// First word of a func_80157C4C item record: the top 12 bits are the item
// category (func_8026CE30 reads them via func_80139358).
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
