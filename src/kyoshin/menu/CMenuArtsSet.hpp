#pragma once

#include <types.h>
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/IUICf.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"

class CTitleAHelp;

// CArtsInfo layout stand-in (0x74 bytes). kyoshin/CArtsInfo.hpp is NOT
// included here: it declares the C-linkage import __dt__17UnkClass_8045F564Fv
// with (void*, int) while CSysWin.hpp re-declares the same C-linkage name
// with (UnkClass_8045F564*, int) - MWCC rejects the pair as illegal
// overloading (10197) when both headers are visible in one TU. This TU only
// embeds the object by value, so an opaque layout-compatible buffer suffices
// (all CArtsInfo function imports used here are declared below).
extern "C" void __ct__CArtsInfo(void* self);
class CArtsInfo {
public:
    u8 data[0x74];
    CArtsInfo() { __ct__CArtsInfo(this); }
    ~CArtsInfo();
};

// Pane object whose +0x44/+0x48 floats receive the scale (CMenuArtsSet_setPaneScale).
struct SArtsPaneScaleObj {
    char _pad[0x44];
    float mField44;
    float mField48;
};

// 2D position used as source for CMenuArtsSet_setPaneScale.
struct SArtsVec2 {
    float x;
    float y;
};

// Short variant of SArtsSub8022FA58 for the sub-object at 0x124 in
// CMenuArtsSet (0x24 bytes: 0x124-0x147). The full-size sibling (mSubObj148)
// starts at 0x148, so this view stops after field_0x21. field_0x15 lands at
// absolute 0x139 (written by CMenuArtsSet_setArmedFlag).
class SArtsSub8022FA58Short {
public:
    SArtsSub8022FA58Short(u32 a, u32 b);
    ~SArtsSub8022FA58Short();
    u32 field_0x00;
    u32 field_0x04;
    u32 field_0x08;
    u32 field_0x0C;
    u32 field_0x10;
    u8  field_0x14;
    u8  field_0x15;        // absolute 0x139 in CMenuArtsSet
    u8  field_0x16;
    u8  field_0x17;
    u8  field_0x18;
    u8  field_0x19[8];     // 0x19-0x20 (two words; includes the 0x20 cursor)
    u8  field_0x21;
    // 0x22-0x23: tail alignment padding - must stay unnamed so the struct
    // assign skips it (a named pad member would be copied memberwise)
};

// Anonymous menu-sub-object constructed by __ct__8022FA58.
// Shared by the func_8023xxxx helpers (ArtsCharList_getId, ArtsSubCursor_setVisible,
// ArtsSlotCursor_clampFromEmpty, artsGoPrevItem).
class SArtsSub8022FA58 {
public:
    u32 field_0x00;
    u32 field_0x04;
    u32 field_0x08;
    u32 field_0x0C;
    u32 field_0x10;
    u8  field_0x14;
    u8  field_0x15;
    u8  field_0x16;
    u8  field_0x17;
    u8  field_0x18;
    u8  field_0x19[7];       // 0x19-0x1F
    s8  field_0x20;
    u8  field_0x21;
    u8  field_0x22;
    u8  field_0x23;
    u8  field_0x24;
    u8  field_0x25;
    u8  field_0x26;
    u8  field_0x27;
    u8  field_0x28;
    char _pad_0x29[0x31 - 0x29];
    u8  field_0x30;
    u8  field_0x31;
};

// Object constructed by __ct__8023042C: 8 leading words (0x00-0x1C) then
// byte fields 0x20-0x28.
class SArts3042C {
public:
    SArts3042C(u32 a, u32 b);
    ~SArts3042C();
    u32 field_0x00;
    u32 field_0x04;
    u32 field_0x08;
    u32 field_0x0C;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1C;
    s8  field_0x20;
    u8  field_0x21;
    u8  field_0x22;
    u8  field_0x23;
    u8  field_0x24;
    u8  field_0x25;
    u8  field_0x26;
    u8  field_0x27;
    u8  field_0x28;
};

// Object constructed by __ct__802319AC: leading words up to 0x1C, byte state
// 0x20-0x27, a word at 0x28, and a table 0x2C-0x12B with 0x12C-0x12E flags.
class SArts319AC {
public:
    SArts319AC(u32 a, u32 b, u32 c);
    ~SArts319AC();
    u32 field_0x00;
    u32 field_0x04;
    u32 field_0x08;
    u32 field_0x0C;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1C;
    u8  field_0x20;
    u8  field_0x21;
    u8  field_0x22;
    u8  field_0x23;
    u8  field_0x24;
    u8  field_0x25;
    u8  field_0x26;
    u32 field_0x28;
    u32 mTable[0x40];     // 0x2C-0x12B (16-byte rows; retail copies as word loop)
    u8  field_0x12C;
    u8  field_0x12D;
    u8  field_0x12E;
};

// Signed variant whose 0x20/0x21 bytes are tested with sign-extend
// compares in ArtsTable_isAtTop.
class SArts3270C {
public:
    char _pad[0x20];
    s8 field_0x20;
    s8 field_0x21;
};

// Object holding a flag at 0x16F and a value at 0x2A1 (CMenuArtsSet_isListBusy).
class SArts34C84 {
public:
    char _pad[0x16F];
    u8 field_0x16F;
    char _pad2[0x2A1-0x170];
    u8 field_0x2A1;
};

// Object with a word at 0x1C and flags at 0x28/0x30 (CMenuArtsSet_markLayoutsReady).
class SArts35108 {
public:
    char _pad[0x1C];
    u32 field_0x1C;
    char _pad20[0x28-0x20];
    u8 field_0x28;
    char _pad29[0x30-0x29];
    u8 field_0x30;
};

// Pointee of SArtsSub8022FA58.field_0x08; field_0x10 read by ArtsSubCursor_setVisible.
class SArts080C {
public:
    char _pad[0x10];
    u8* field_0x10;
};

// 2-byte-cursor + arts table object used by ArtsTable_getSelectedId. The arts table
// is an array of 16-byte rows starting at 0x2C (0x100 bytes = 16 rows),
// indexed by (field_0x20 + field_0x21) << 4.
class SArts322BC {
public:
    char _pad_00[0x20];
    s8 field_0x20;
    s8 field_0x21;
    u8  field_0x22;
    u8  field_0x23;
    u8  field_0x24;
    u8  field_0x25;
    u8  field_0x26;
    u8  field_0x27;
    u8* field_0x28;     // 0x28 scrollbar/table object passed to CScrollBar_PlaceThumb
    u8 mTable[0x100];   // 0x2C-0x12B
    u8 field_0x12C;     // 0x12C
    u8 field_0x12D;     // 0x12D
    u8 field_0x12E;     // 0x12E
};

// AnimTransform driver used by CMenuArtsSet_finishCloseAnim.
class SArts3506C {
public:
    char _pad_00[0x20];
    nw4r::lyt::AnimTransform* mAnim20;      // 0x20 nw4r::lyt::AnimTransform*
    char _pad_24[0x2C - 0x24];
    u32 field_0x2C;     // 0x2C
    char _pad_30[0x31 - 0x30];
    u8 field_0x31;      // 0x31
};

// Cursor/state object used by ArtsTable_resetCursor (and ArtsTableCursor_refresh/80232C78,
// which read the 0x00/0x08/0x14 pointers and the 0x20 cursor).
class SArts3150CDriver;
class SArts327B0 {
public:
    SArts3150CDriver* field_0x00;   // 0x00 driver (field_0x10 -> layout obj)
    char _pad_04[0x08 - 0x04];
    SArts080C* field_0x08;          // 0x08
    char _pad_0C[0x14 - 0x0C];
    SArts080C* field_0x14;          // 0x14
    char _pad_18[0x20 - 0x18];
    s8 field_0x20;                  // 0x20 cursor
    u8 field_0x21;                  // 0x21
    char _pad_22[0x28 - 0x22];
    u8* field_0x28;                 // 0x28
};

// Element of the 0x49-byte array indexed by ArtsElem_setDataByte.
class SArtsSubDElem {
public:
    char _pad[0x30];
    u8 field_0x30;          // 0x30 flag
    u8 data[0x18];          // 0x31-0x48
};

// Object with two SArts080C pointees at 0x08/0x14 plus byte state at
// 0x20/0x21/0x27. ArtsBusyPane_toggle toggles the busy flag at 0x27 and drives
// both pointees (each read via their field_0x10).
class SArts313E0 {
public:
    SArts3150CDriver* field_0x00;   // 0x00 driver (field_0x10 -> layout obj)
    char _pad_04[0x08 - 0x04];
    SArts080C* field_0x08;      // 0x08
    char _pad_0C[0x14 - 0x0C];
    SArts080C* field_0x14;      // 0x14
    char _pad_18[0x20 - 0x18];
    u8 field_0x20;              // 0x20
    s8 field_0x21;              // 0x21 (signed: used as an index in ArtsBusyPane_refreshCursor)
    char _pad_22[0x27 - 0x22];
    u8 field_0x27;              // 0x27 busy flag
};

// Object with two SArts2FObj* pointees at 0x08/0x14, each released via
// their vtable slot 0x08 (v2) then nulled (ArtsLayoutPair_releaseSlots / ArtsTableLayout_release).
class SArts2FObj;
class SArts3066C {
public:
    char _pad_00[0x08];
    SArts2FObj* field_0x08;     // 0x08
    char _pad_0C[0x14 - 0x0C];
    SArts2FObj* field_0x14;     // 0x14
};

// Object with two SArts080C pointees at 0x08/0x14 and a flag at 0x12D
// (ArtsTable_toggleBusyPanes toggles the flag and drives both pointees).
class SArts32888 {
public:
    char _pad_00[0x08];
    SArts080C* field_0x08;      // 0x08
    char _pad_0C[0x14 - 0x0C];
    SArts080C* field_0x14;      // 0x14
    char _pad_18[0x12D - 0x18];
    u8 field_0x12D;             // 0x12D
};

// Object holding a float at offset 0x10 (written by ArtsLayout_runLayoutOut etc.).
class SArtsFloat10 {
public:
    char _pad[0x10];
    float field_0x10;           // 0x10
};

// Polymorphic pointee with a driver method at vtable +0x2C. MWCC reserves a
// 2-entry vtable header, so emitted slot N sits at declaration index N-2: the
// driver is the 10th declared virtual (index 9) and lands at +0x2C.
class SArtsV11Obj {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v11(void* a, int flag); // vtable +0x2C (emitted slot 11)
};

// Layout driver at 0x08/0x0C/0x10 with 0x23/0x24 state. ArtsLayout_runLayoutOut/
// ArtsTableLayout_runLayoutOut run the anim and, when not busy (0x23==0), drive two
// sub-panels via v11 then bump state 0x24.
class SArts31480 {
public:
    char _pad_00[0x08];
    SArtsV11Obj* field_0x08;            // 0x08
    nw4r::lyt::AnimTransform* field_0x0C; // 0x0C
    SArtsFloat10* field_0x10;           // 0x10
    char _pad_14[0x23 - 0x14];
    u8 field_0x23;                      // 0x23 busy flag
    u8 field_0x24;                      // 0x24
};

// Layout driver at 0x14/0x18/0x1C with 0x23/0x24 state (ArtsLayout_runTwinLayoutOut /
// ArtsTableLayout_runTwinLayoutOut). Identical shape to SArts31480 but shifted by 0xC.
class SArts315BC {
public:
    char _pad_00[0x14];
    SArtsV11Obj* field_0x14;            // 0x14
    nw4r::lyt::AnimTransform* field_0x18; // 0x18
    SArtsFloat10* field_0x1C;           // 0x1C
    char _pad_20[0x23 - 0x20];
    u8 field_0x23;                      // 0x23 busy flag
    u8 field_0x24;                      // 0x24
};

// Pointee of SArts3150C.field_0x08: polymorphic (v11 driver at vtable +0x2C,
// same shape as SArtsV11Obj) with an extra u8* at +0x10 read by
// ArtsLayout_runLayoutIn and fed to setPaneVisible.
class SArts3150CDriver {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v11(void* a, int flag); // vtable +0x2C (emitted slot 11)
    char _pad_04[0x10 - 0x04];
    u8* field_0x10;                     // 0x10
};

// Object driven by ArtsLayout_runLayoutIn: a v11 driver at 0x08, a SArtsFloat10 at
// 0x0C, an AnimTransform at 0x10, a SArts080C at 0x14, and 0x23/0x24 state.
class SArts3150C {
public:
    char _pad_00[0x08];
    SArts3150CDriver* field_0x08;        // 0x08
    SArtsFloat10* field_0x0C;            // 0x0C
    nw4r::lyt::AnimTransform* field_0x10; // 0x10
    SArts080C* field_0x14;               // 0x14
    char _pad_18[0x23 - 0x18];
    u8 field_0x23;                      // 0x23
    u8 field_0x24;                      // 0x24
};

// Pointee of SArts3150CDriver.field_0x10 (only used by ArtsLayoutPair_completeLayoutIn): a
// method at vtable +0x3C returns the pane fed to setPaneVisible. MWCC
// reserves a 2-entry vtable header, so declared index 13 emits slot 15
// (4*15 = 0x3C).
class SArts3CObj {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void* v13(void* label, int flag); // idx 13 -> vtable +0x3C
};

// Layout driver shared by the ArtsLayoutPair_loadSlots / ArtsTableLayout_load layout-pair
// init functions: v11 at +0x2C, v14 at +0x38, and a SArts3CObj pane at
// +0x10 (its v13 at +0x3C fetches the label pane).
class SArts304C4Driver {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v11(void* a, int flag); // index 9 -> vtable +0x2C
    virtual void v12();
    virtual void v13();
    virtual void v14(int a);             // index 12 -> vtable +0x38
    char _pad_04[0x10 - 0x04];
    SArts3CObj* field_0x10;              // 0x10
};

// Layout-pair object initialized by ArtsLayoutPair_loadSlots / ArtsTableLayout_load: an arc
// accessor at 0x04, two layout drivers at 0x08/0x14 (each parking panes via
// v11/v14 and exposing a SArts3CObj at +0x10), and AnimTransforms at
// 0x0C/0x10/0x18/0x1C. Word-compatible with the SArtsSub8022FA58 /
// SArts327B0 prefixes the tail calls cast back to.
class SArts304C4 {
public:
    u32 field_0x00;
    nw4r::lyt::ArcResourceAccessor* field_0x04; // 0x04
    SArts304C4Driver* field_0x08;               // 0x08 layout driver 1
    nw4r::lyt::AnimTransform* field_0x0C;       // 0x0C
    nw4r::lyt::AnimTransform* field_0x10;       // 0x10
    SArts304C4Driver* field_0x14;               // 0x14 layout driver 2
    nw4r::lyt::AnimTransform* field_0x18;       // 0x18
    nw4r::lyt::AnimTransform* field_0x1C;       // 0x1C
    char _pad_20[0x34 - 0x20];                  // tail matches SArtsSub8022FA58
};

// Object driven by ArtsLayoutPair_completeLayoutIn (view of the mSubObj124 sub-object): a
// v11 driver at 0x08, a SArtsFloat10 at 0x0C, an AnimTransform at 0x10,
// and 0x16/0x18 state bytes.
class SArts30070 {
public:
    char _pad_00[0x08];
    SArts3150CDriver* field_0x08;        // 0x08 driver (v11 +0x2C, field_0x10)
    SArtsFloat10* field_0x0C;            // 0x0C
    nw4r::lyt::AnimTransform* field_0x10; // 0x10
    char _pad_14[0x16 - 0x14];
    u8 field_0x16;                      // 0x16
    u8 field_0x17;                      // 0x17
    u8 field_0x18;                      // 0x18
};

// Twin of SArts30070 with the 0x0C/0x10 roles swapped (ArtsLayoutPair_completeLayoutOut): the
// AnimTransform sits at 0x0C and the float object at 0x10.
class SArts2FF74 {
public:
    char _pad_00[0x08];
    SArts3150CDriver* field_0x08;        // 0x08 driver (v11 +0x2C, field_0x10)
    nw4r::lyt::AnimTransform* field_0x0C; // 0x0C anim
    SArtsFloat10* field_0x10;            // 0x10 float object
    char _pad_14[0x16 - 0x14];
    u8 field_0x16;                      // 0x16
    u8 field_0x17;                      // 0x17
    u8 field_0x18;                      // 0x18
};



// Layout-out driver for the mSubObj148+0x2C twin (ArtsLayout_runTwinLayoutIn /
// ArtsTableLayout_runTwinLayoutIn): a v11 driver at 0x14 (also read via field_0x10), a
// SArtsFloat10 at 0x18, the AnimTransform at 0x1C, a show/hide pane at
// 0x08, and 0x23/0x24 state.
class SArts31648 {
public:
    char _pad_00[0x08];
    SArts080C* field_0x08;               // 0x08 pane (field_0x10 -> setPaneVisible)
    char _pad_0C[0x14 - 0x0C];
    SArts3150CDriver* field_0x14;        // 0x14 v11 driver (field_0x10 too)
    SArtsFloat10* field_0x18;            // 0x18 float object
    nw4r::lyt::AnimTransform* field_0x1C; // 0x1C layout-out anim
    char _pad_20[0x23 - 0x20];
    u8 field_0x23;                      // 0x23 busy flag
    u8 field_0x24;                      // 0x24 state
};

// Overlay used by CMenuArtsSet_isBusy: a CArtsInfo at 0x74, a CSysWin at
// 0xE8, and read-only byte flags at 0x16F/0x2A1/0x2A6. Overlaps the
// CMenuArtsSet object but viewed standalone so its flags are reachable.
class SArts33888 {
public:
    char _pad_00[0x74];
    CArtsInfo field_0x74;              // 0x74 (0x74 bytes -> ends 0xE8)
    CSysWin field_0xE8;            // 0xE8
    char _pad_120[0x16F - (0xE8 + sizeof(CSysWin))];
    u8 field_0x16F;                    // 0x16F
    char _pad_170[0x2A1 - 0x170];
    u8 field_0x2A1;                    // 0x2A1
    char _pad_2A2[0x2A6 - 0x2A2];
    u8 field_0x2A6;                    // 0x2A6
};

// Object used by ArtsBusyPane_showPrimary: two pointees (each read via their field_0x10)
// plus 0x23/0x24 byte state.
class SArts30D18 {
public:
    char _pad_00[0x08];
    SArts080C* field_0x08;      // 0x08
    char _pad_0C[0x14 - 0x0C];
    SArts080C* field_0x14;      // 0x14
    char _pad_18[0x23 - 0x18];
    u8 field_0x23;              // 0x23
    u8 field_0x24;              // 0x24
};

// Object for CMenuArtsSet_isDemoLockedArt/CMenuArtsSet_isLockedArtOpen: an SArtsSub8022FA58 at 0x124 whose
// field_0x14 (absolute 0x138) is used as the table index, plus the
// 0x168/0x16A byte flags.
class SArts34D14 {
public:
    char _pad_00[0x124 - 0x00];
    SArtsSub8022FA58 mSubObj124;     // 0x124 (0x34 bytes incl. tail pad)
    char _pad_158[0x168 - (0x124 + sizeof(SArtsSub8022FA58))]; // to 0x168
    u8 field_0x168;                 // 0x168
    u8 field_0x169;                 // 0x169
    u8 field_0x16A;                 // 0x16A
};

// Polymorphic pointee of SArts2FDF4.field_0x08. Slot 2 (vtable offset 0x08)
// is called by ArtsLayoutPair_release; slot 14 (vtable offset 0x38) by ArtsLayoutPair_advance.
class SArts2FObj {
public:
    virtual void v2(int a); // vtable +0x08 (declared index 0; MWCC 2-entry header)
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14(int a); // vtable +0x38 (declared index 12)
};

// View of the mSubObj124 layout-pair object rebuilt by artsRebuildUI: label
// layout at 0x00, arc accessor at 0x04, main layout driver at 0x08 and its
// two AnimTransforms at 0x0C/0x10.
struct SArtsFAD0 {
    SArts304C4Driver* field_0x00;               // 0x00 label layout (+0x10 pane)
    nw4r::lyt::ArcResourceAccessor* field_0x04; // 0x04
    SArts304C4Driver* field_0x08;               // 0x08 main layout driver
    nw4r::lyt::AnimTransform* field_0x0C;       // 0x0C close anim
    nw4r::lyt::AnimTransform* field_0x10;       // 0x10 open anim
};

// Object used by ArtsLayoutPair_release / ArtsLayoutPair_advance.
class SArts2FDF4 {
public:
    char _pad_00[0x08];
    SArts2FObj* field_0x8;
    char _pad_0C[0x18 - 0x0C];
    u8 field_0x18;
};

// Pointee driven via vtable +0x38 by ArtsLayoutPair_advancePhases / ArtsTableLayout_advancePhases. MWCC
// reserves a 2-entry vtable header, so the 13th declared virtual (index 12)
// is the emitted slot at 4*(12+2) = 0x38.
class SArts38Obj {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12(int a); // idx 12 -> vtable +0x38
};

// State-machine object driven by ArtsLayoutPair_advancePhases / ArtsTableLayout_advancePhases: two +0x38
// pointees at 0x08/0x14 plus the 0x22/0x24/0x25 byte states. Overlaps the
// SArtsSub8022FA58 sub-object at CMenuArtsSet::0x148 and the +0x2C twin.
class SArts306F0 {
public:
    char _pad_00[0x08];
    SArts38Obj* field_0x08;        // 0x08
    char _pad_0C[0x14 - 0x0C];
    SArts38Obj* field_0x14;        // 0x14
    char _pad_18[0x22 - 0x18];
    u8 field_0x22;                 // 0x22
    u8 field_0x23;                 // 0x23
    u8 field_0x24;                 // 0x24
    u8 field_0x25;                 // 0x25
};

// Layout-pair draw box: two layouts at 0x08/0x14 plus visibility flag at
// 0x22 and 0x28 (and arts-table flag at 0x12E). Used by ArtsDrawBox_draw and
// ArtsTableDrawBox_draw.
class SArtsDrawBox {
public:
    char _00[0x08];
    nw4r::lyt::Layout* mLayout08;   // 0x08
    char _0C[0x14 - 0x0C];
    nw4r::lyt::Layout* mLayout14;   // 0x14
    char _18[0x22 - 0x18];
    u8 field_0x22;                  // 0x22
    char _23[0x28 - 0x23];
    u8 field_0x28;                  // 0x28
    char _29[0x12E - 0x29];
    u8 field_0x12E;                 // 0x12E
};

// Large polymorphic object reached at CtrlObjectParam_GetCharEntry(...) + 0x17C in
// artsShiftProg. Declares enough virtuals to reach vtable slots 0x1F8
// (index 126) and 0x200 (index 128).
class SArtsManager {
public:
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67();
    virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75();
    virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79();
    virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83();
    virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87();
    virtual void v88(); virtual void v89(); virtual void v90(); virtual void v91();
    virtual void v92(); virtual void v93(); virtual void v94(); virtual void v95();
    virtual void v96(); virtual void v97(); virtual void v98(); virtual void v99();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111();
    virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115();
    virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123();
    virtual s32 v126(int a);   // vtable +0x1F8 (index 124: 2-slot MWCC vt prefix + 124*4)
    virtual void v127();
    virtual s32 v128();        // vtable +0x200 (index 126)
    virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132();
    virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136();
    virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140();
    virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
    virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148();
    virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152();
    virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156();
    // Fillers: v157 must sit at declaration index 157 to emit vtable +0x27C.
    virtual void vFillA();
    virtual void vFillB();
    // Returns the arts-slot query receiver consumed by getArtsSlotRC
    // (artsSyncSlots); ArtsSlot_isWindowEmpty ignores the result.
    virtual void* v157();  // declared index 157 -> vtable +0x27C
};

// Arc-resource-accessor view of SArts308B0View.field_0x04: its second
// declared virtual (vtable +0x0C) finds a named resource inside an archive
// type (a FourCC passed by value) and returns it (NULL when absent).
class SArtsFindObj {
public:
    virtual void v0();
    virtual void* vfind(u32 type, const char* name, int flag); // +0x0C
};

// Layout-owner view of SArts308B0View.field_0x00: +0x10 holds the pane-fetch
// driver whose v13 (+0x3C) returns the pane fed to setPaneVisible.
class SArts308B0Layout {
public:
    char _pad[0x10];
    SArts3CObj* field_0x10;
};

// 8-word blocks reloaded by artsSyncSlots: lbl_eu_8050AB08 lands on the
// object's +0x28 pane-name block, lbl_eu_8050AB88 on a stack-local copy.
struct SArts308B0Block {
    u32 w[8];   // 0x20 bytes (MWCC counted word-copy loop)
};
extern const SArts308B0Block lbl_eu_8050AB08;
extern const SArts308B0Block lbl_eu_8050AB88;

// View of the sub-object driven by artsSyncSlots (mSubObj148): two leading
// pointers, the character id byte at 0x26, and the 8-word block at 0x28.
class SArts308B0View {
public:
    void* field_0x00;      // layout owner (SArts308B0Layout)
    void* field_0x04;      // arc resource accessor (SArtsFindObj)
    char _pad08[0x26 - 0x08];
    u8 field_0x26;         // character id (written from the u8 argument)
    char _pad27[0x28 - 0x27];
    SArts308B0Block field_0x28;
};

// Pane whose position VEC2 sits at +0x4C (copyVEC2 target/source).
struct SArts32C78Pane {
    char _pad[0x4C];
    float xy[2];            // 0x4C
};

// One 16-byte row of the arts table embedded at SArts32C78 offset 0x2C,
// indexed by ((s8)field_0x21 + i) in artsRenderRow.
struct SArts32C78Row {
    u8 field_0x00;          // slot RC matched against getArtsSlotRC
    u8 field_0x01;
    u8 field_0x02;          // level (below 10 draws the numeric gauge)
    u8 _pad03;
    u16 field_0x04;         // name message id
    u16 field_0x06;         // sub message id
    u8 field_0x08;          // pane-offset flag selects (f30)
    u8 field_0x09;          // pane-offset flag selects (f31)
    u8 _pad0A[2];
    char* field_0x0C;       // description text id for LayoutSetTextBoxFmtValue
};

// View of the CArtsList arts-table object driven by artsRenderRow (retail
// reads offsets 0x00/0x04/0x21/0x26 plus the 16-byte rows at 0x2C and the
// count byte at 0x12C).
class SArts32C78 {
public:
    void* field_0x00;           // layout owner (+0x10 -> SArts3CObj pane driver)
    SArtsFindObj* field_0x04;   // arc resource accessor
    char _pad08[0x21 - 0x08];
    s8 field_0x21;              // signed first row index
    char _pad22[0x26 - 0x22];
    u8 field_0x26;              // character id
    char _pad27[0x2C - 0x27];
    SArts32C78Row mRows[16];    // 0x2C-0x12B
    u8 field_0x12C;             // visible row count
    u8 field_0x12D;
    u8 field_0x12E;
};

// Arts-slot record lookup on the receiver returned by SArtsManager::v157
// (plain-C retail symbol; sel is the demo/character gate complement).
extern "C" u16 getArtsSlotRC(void* recv, short sel, u8 idx);

// Root returned by CtrlObjectParam_GetCharEntry; the manager object sits at offset 0x17C and
// an array of 0x49-byte arts elements starts at offset 0xE8 (ArtsSlot_isWindowEmpty).
struct SArtsManagerRoot {
    char _pad[0xE8];
    SArtsSubDElem mElemsE8[1];   // 0xE8 - arts element array (0x49-byte rows)
    char _pad2[0x17C - (0xE8 + sizeof(SArtsSubDElem))];
    SArtsManager mObj17C;   // 0x17C
};

// Pointee of CMenuArtsSet.field_0x1C. MWCC reserves a 2-entry vtable header
// (emitted slot N = declaration index N+2), so: v0 (index 0) is the release
// call at +0x08 in CMenuArtsSet_teardown, v11 (index 9) drives the two panes at
// +0x2C in CMenuArtsSet_completeLayoutOut, v14 (index 12) is the state-advance at +0x38
// driven by artsStepState.
class SArts1C {
public:
    virtual void v0(int a);   // index 0 -> vtable +0x08
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v11(void* a, int flag); // index 9 -> vtable +0x2C
    virtual void v12();
    virtual void v13();
    virtual void v14(int a);   // index 12 -> vtable +0x38
    char _pad_04[0x10 - 0x04];
    SArts3CObj* field_0x10;    // 0x10 pane (read by CArtsList::OnFileEvent)
};

// View of the object returned by getFontInfo (CDeviceFont); its vtable
// slot +0x24 (declared index 7; MWCC reserves a 2-entry header) yields the
// font handle pushed onto the layout root pane by ApplyFontToTxtPanes.
class SDevFontV {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual u32 v7();   // vtable +0x24
};

// Vtable view of the CSysWin sub-object at CArtsList::0xE8 (CArtsList_startLoad).
// Declares virtuals 0..34 so vtable slot 0x88 (= index 34) is reachable.
class SArtsWinE8 {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();       // vtable +0x88
};

// Embedded cursor (CCur18) body inside CArtsInfo: only the tail (abs
// 0x60-0x71) is copied by CArtsList::CArtsList's temp copy-init.
struct SArtsCursorBody {
    u32 field_0x00;   // abs 0x5C - not copied by the ctor
    u32 field_0x04;   // abs 0x60
    u32 field_0x08;   // abs 0x64
    u32 field_0x0C;   // abs 0x68
    u32 field_0x10;   // abs 0x6C
    u8  field_0x14;   // abs 0x70
    u8  field_0x15;   // abs 0x71
    u8  _pad_16[2];
};

// Body view of CArtsInfo (everything after the vtable at +0x00), used by
// CArtsList::CArtsList's copy-init from a stack temp: the copy starts at
// +0x04 (skipping the vtable) and covers the 0x10-byte mem region (via
// __as__17UnkClass_8045F564FRC17UnkClass_8045F564) plus the scalar fields through abs 0x71.
struct SArtsInfoBody {
    UnkClass_8045F564 mMemRegion;   // body+0x00 (abs 0x04)
    int field_0x14;                 // body+0x10 (abs 0x14)
    int field_0x18;                 // body+0x14
    int field_0x1C;                 // body+0x18
    void* field_0x20;               // body+0x1C
    void* field_0x24;               // body+0x20
    void* field_0x28;               // body+0x24
    void* field_0x2C;               // body+0x28
    void* field_0x30;               // body+0x2C
    void* field_0x34;               // body+0x30
    void* field_0x38;               // body+0x34
    void* field_0x3C;               // body+0x38
    u8 field_0x40;                  // body+0x3C
    char _pad_41[3];
    int field_0x44;                 // body+0x40
    u8 field_0x48;                  // body+0x44
    u8 field_0x49;                  // body+0x45
    char _pad_4A[2];
    int field_0x4C;                 // body+0x48
    int field_0x50;                 // body+0x4C
    u8 field_0x54;                  // body+0x50
    u8 field_0x55;                  // body+0x51
    u8 field_0x56;                  // body+0x52
    char _pad_57;
    u16 field_0x58;                 // body+0x54
    s8 field_0x5A;                  // body+0x56
    char _pad_5B;
    SArtsCursorBody mCursor;        // body+0x58 (abs 0x5C)
};

class CArtsList : public IWorkEvent {
public:
    CArtsList();
    virtual ~CArtsList();
    virtual bool OnFileEvent(CEventFile* pEventFile);

    u8 CMenuArtsSet_getReadyFlag();
    u32 CMenuArtsSet_isArtsInfoActive();
    void CMenuArtsSet_setArmedFlag(u8 val);
    u32 CMenuArtsSet_isSlotWindowEmpty();

    UnkClass_8045F564 mMemRegion;   // 0x04 (0x10 bytes)
    CFileHandle* field_0x14;        // 0x14  file handle
    nw4r::lyt::ArcResourceAccessor* field_0x18;  // 0x18 arc accessor
    SArts1C* field_0x1C;            // 0x1C layout driver
    nw4r::lyt::AnimTransform* field_0x20;        // 0x20
    nw4r::lyt::AnimTransform* field_0x24;        // 0x24
    u8 field_0x28;                  // 0x28
    s32 field_0x2C;                 // 0x2C
    u8 field_0x30;                  // 0x30
    u8 field_0x31;                  // 0x31
    CScrollBar mScrollBar;          // 0x34 (0x40 bytes)
    CArtsInfo mSubObj74;            // 0x74 (0x74 bytes)
    CSysWin mSysWinE8;              // 0xE8 (0x3C bytes)
    SArtsSub8022FA58Short mSubObj124;   // 0x124 (0x24 bytes)
    SArts3042C mSubObj148;          // 0x148 (0x2C bytes)
    SArts319AC mSubObj174;          // 0x174 (0x130 bytes)
    u8 field_0x2A4;                 // 0x2A4
    u8 field_0x2A5;                 // 0x2A5
    u8 field_0x2A6;                 // 0x2A6
};

extern const float lbl_eu_80668638; // initial menu timer value (.sdata2)

// Arts-set menu: IUICf task base (0x00-0x57), the IScnRender render-callback
// subobject at +0x58, then the embedded widgets.
class CMenuArtsSet : public IUICf, public IScnRender {
public:
    CMenuArtsSet(CScn* scene)
        : mScene(scene), mBgTex(0), mTitleAHelp(0, 0), mList(), mState(0),
          mTimer(lbl_eu_80668638) {}
    virtual ~CMenuArtsSet();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();

    static CMenuArtsSet* create(CProcess* parent, CScn* scene);
    static CMenuArtsSet* sInstance;

    CScn* mScene;           // 0x5C
    CBgTex mBgTex;          // 0x60
    CTitleAHelp mTitleAHelp; // 0x80
    CArtsList mList;        // 0xB8
    u8 mState;              // 0x360
    float mTimer;           // 0x364
};
