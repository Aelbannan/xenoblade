#pragma once

#include <types.h>
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CScrollBar.hpp"

class CTitleAHelp;

// CArtsInfo layout stand-in (0x74 bytes). kyoshin/CArtsInfo.hpp is NOT
// included here: it declares the C-linkage import __dt__17UnkClass_8045F564Fv
// with (void*, int) while CSysWin.hpp re-declares the same C-linkage name
// with (UnkClass_8045F564*, int) - MWCC rejects the pair as illegal
// overloading (10197) when both headers are visible in one TU. This TU only
// embeds the object by value, so an opaque layout-compatible buffer suffices
// (all CArtsInfo function imports used here are declared below).
class CArtsInfo {
public:
    u8 data[0x74];
    ~CArtsInfo();
};

// 2D position used as source for func_80231848.
struct SArtsVec2 {
    float x;
    float y;
};

// Short variant of SArtsSub8022FA58 for the sub-object at 0x124 in
// CMenuArtsSet (0x24 bytes: 0x124-0x147). The full-size sibling (mSubObj148)
// starts at 0x148, so this view stops after field_0x21. field_0x15 lands at
// absolute 0x139 (written by func_802349F8).
class SArtsSub8022FA58Short {
public:
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
// Shared by the func_8023xxxx helpers (func_8023040C, func_8023080C,
// func_80231464, func_8022FE90).
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
    u8  field_0x27;
    u8  field_0x28;
};

// Object constructed by __ct__802319AC: leading words up to 0x1C, byte state
// 0x20-0x27, a word at 0x28, and a table 0x2C-0x12B with 0x12C-0x12E flags.
class SArts319AC {
public:
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
// compares in func_8023270C.
class SArts3270C {
public:
    char _pad[0x20];
    s8 field_0x20;
    s8 field_0x21;
};

// Object holding a flag at 0x16F and a value at 0x2A1 (func_80234C84).
class SArts34C84 {
public:
    char _pad[0x16F];
    u8 field_0x16F;
    char _pad2[0x2A1-0x170];
    u8 field_0x2A1;
};

// Object with a word at 0x1C and flags at 0x28/0x30 (func_80235108).
class SArts35108 {
public:
    char _pad[0x1C];
    u32 field_0x1C;
    char _pad20[0x28-0x20];
    u8 field_0x28;
    char _pad29[0x30-0x29];
    u8 field_0x30;
};

// Pointee of SArtsSub8022FA58.field_0x08; field_0x10 read by func_8023080C.
class SArts080C {
public:
    char _pad[0x10];
    u8* field_0x10;
};

// 2-byte-cursor + arts table object used by func_802322BC. The arts table
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
    u8* field_0x28;     // 0x28 scrollbar/table object passed to func_801F3850
    u8 mTable[0x100];   // 0x2C-0x12B
    u8 field_0x12C;     // 0x12C
    u8 field_0x12D;     // 0x12D
    u8 field_0x12E;     // 0x12E
};

// AnimTransform driver used by func_8023506C.
class SArts3506C {
public:
    char _pad_00[0x20];
    nw4r::lyt::AnimTransform* mAnim20;      // 0x20 nw4r::lyt::AnimTransform*
    char _pad_24[0x2C - 0x24];
    u32 field_0x2C;     // 0x2C
    char _pad_30[0x31 - 0x30];
    u8 field_0x31;      // 0x31
};

// Cursor/state object used by func_802327B0 (and func_80232B88/80232C78,
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

// Element of the 0x49-byte array indexed by func_80230FF0.
class SArtsSubDElem {
public:
    char _pad[0x30];
    u8 field_0x30;          // 0x30 flag
    u8 data[0x18];          // 0x31-0x48
};

// Object with two SArts080C pointees at 0x08/0x14 plus byte state at
// 0x20/0x21/0x27. func_802313E0 toggles the busy flag at 0x27 and drives
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
    s8 field_0x21;              // 0x21 (signed: used as an index in func_8023185C)
    char _pad_22[0x27 - 0x22];
    u8 field_0x27;              // 0x27 busy flag
};

// Object with two SArts2FObj* pointees at 0x08/0x14, each released via
// their vtable slot 0x08 (v2) then nulled (func_8023066C / func_80231C30).
class SArts2FObj;
class SArts3066C {
public:
    char _pad_00[0x08];
    SArts2FObj* field_0x08;     // 0x08
    char _pad_0C[0x14 - 0x0C];
    SArts2FObj* field_0x14;     // 0x14
};

// Object with two SArts080C pointees at 0x08/0x14 and a flag at 0x12D
// (func_80232888 toggles the flag and drives both pointees).
class SArts32888 {
public:
    char _pad_00[0x08];
    SArts080C* field_0x08;      // 0x08
    char _pad_0C[0x14 - 0x0C];
    SArts080C* field_0x14;      // 0x14
    char _pad_18[0x12D - 0x18];
    u8 field_0x12D;             // 0x12D
};

// Object holding a float at offset 0x10 (written by func_80231480 etc.).
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

// Layout driver at 0x08/0x0C/0x10 with 0x23/0x24 state. func_80231480/
// func_80232910 run the anim and, when not busy (0x23==0), drive two
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

// Layout driver at 0x14/0x18/0x1C with 0x23/0x24 state (func_802315BC /
// func_80232A4C). Identical shape to SArts31480 but shifted by 0xC.
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
// func_8023150C and fed to func_80124270.
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

// Object driven by func_8023150C: a v11 driver at 0x08, a SArtsFloat10 at
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

// Pointee of SArts3150CDriver.field_0x10 (only used by func_80230070): a
// method at vtable +0x3C returns the pane fed to func_80124270. MWCC
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

// Layout driver shared by the func_802304C4 / func_80231A48 layout-pair
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

// Layout-pair object initialized by func_802304C4 / func_80231A48: an arc
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

// Object driven by func_80230070 (view of the mSubObj124 sub-object): a
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

// Twin of SArts30070 with the 0x0C/0x10 roles swapped (func_8022FF74): the
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



// Layout-out driver for the mSubObj148+0x2C twin (func_80231648 /
// func_80232AD8): a v11 driver at 0x14 (also read via field_0x10), a
// SArtsFloat10 at 0x18, the AnimTransform at 0x1C, a show/hide pane at
// 0x08, and 0x23/0x24 state.
class SArts31648 {
public:
    char _pad_00[0x08];
    SArts080C* field_0x08;               // 0x08 pane (field_0x10 -> func_80124270)
    char _pad_0C[0x14 - 0x0C];
    SArts3150CDriver* field_0x14;        // 0x14 v11 driver (field_0x10 too)
    SArtsFloat10* field_0x18;            // 0x18 float object
    nw4r::lyt::AnimTransform* field_0x1C; // 0x1C layout-out anim
    char _pad_20[0x23 - 0x20];
    u8 field_0x23;                      // 0x23 busy flag
    u8 field_0x24;                      // 0x24 state
};

// Overlay used by func_80233888: a CArtsInfo at 0x74, a CSysWinFull at
// 0xE8, and read-only byte flags at 0x16F/0x2A1/0x2A6. Overlaps the
// CMenuArtsSet object but viewed standalone so its flags are reachable.
class SArts33888 {
public:
    char _pad_00[0x74];
    CArtsInfo field_0x74;              // 0x74 (0x74 bytes -> ends 0xE8)
    CSysWinFull field_0xE8;            // 0xE8
    char _pad_120[0x16F - (0xE8 + sizeof(CSysWinFull))];
    u8 field_0x16F;                    // 0x16F
    char _pad_170[0x2A1 - 0x170];
    u8 field_0x2A1;                    // 0x2A1
    char _pad_2A2[0x2A6 - 0x2A2];
    u8 field_0x2A6;                    // 0x2A6
};

// Object used by func_80230D18: two pointees (each read via their field_0x10)
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

// Object for func_80234D14/func_80234CA0: an SArtsSub8022FA58 at 0x124 whose
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
// is called by func_8022FD9C; slot 14 (vtable offset 0x38) by func_8022FDF4.
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

// View of the mSubObj124 layout-pair object rebuilt by func_8022FAD0: label
// layout at 0x00, arc accessor at 0x04, main layout driver at 0x08 and its
// two AnimTransforms at 0x0C/0x10.
struct SArtsFAD0 {
    SArts304C4Driver* field_0x00;               // 0x00 label layout (+0x10 pane)
    nw4r::lyt::ArcResourceAccessor* field_0x04; // 0x04
    SArts304C4Driver* field_0x08;               // 0x08 main layout driver
    nw4r::lyt::AnimTransform* field_0x0C;       // 0x0C close anim
    nw4r::lyt::AnimTransform* field_0x10;       // 0x10 open anim
};

// Object used by func_8022FD9C / func_8022FDF4.
class SArts2FDF4 {
public:
    char _pad_00[0x08];
    SArts2FObj* field_0x8;
    char _pad_0C[0x18 - 0x0C];
    u8 field_0x18;
};

// Pointee driven via vtable +0x38 by func_802306F0 / func_80231CB4. MWCC
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

// State-machine object driven by func_802306F0 / func_80231CB4: two +0x38
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
// 0x22 and 0x28 (and arts-table flag at 0x12E). Used by func_802307A4 and
// func_80231D68.
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

// Large polymorphic object reached at func_8009EC9C(...) + 0x17C in
// func_8022FF00. Declares enough virtuals to reach vtable slots 0x1F8
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
    // (func_802308B0); func_80231320 ignores the result.
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
// driver whose v13 (+0x3C) returns the pane fed to func_80124270.
class SArts308B0Layout {
public:
    char _pad[0x10];
    SArts3CObj* field_0x10;
};

// 8-word blocks reloaded by func_802308B0: lbl_eu_8050AB08 lands on the
// object's +0x28 pane-name block, lbl_eu_8050AB88 on a stack-local copy.
struct SArts308B0Block {
    u32 w[8];   // 0x20 bytes (MWCC counted word-copy loop)
};
extern const SArts308B0Block lbl_eu_8050AB08;
extern const SArts308B0Block lbl_eu_8050AB88;

// View of the sub-object driven by func_802308B0 (mSubObj148): two leading
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
// indexed by ((s8)field_0x21 + i) in func_80232C78.
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
    char* field_0x0C;       // description text id for func_80136B4C
};

// View of the CArtsList arts-table object driven by func_80232C78 (retail
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

// Root returned by func_8009EC9C; the manager object sits at offset 0x17C and
// an array of 0x49-byte arts elements starts at offset 0xE8 (func_80231320).
struct SArtsManagerRoot {
    char _pad[0xE8];
    SArtsSubDElem mElemsE8[1];   // 0xE8 - arts element array (0x49-byte rows)
    char _pad2[0x17C - (0xE8 + sizeof(SArtsSubDElem))];
    SArtsManager mObj17C;   // 0x17C
};

// Pointee of CMenuArtsSet.field_0x1C. MWCC reserves a 2-entry vtable header
// (emitted slot N = declaration index N+2), so: v0 (index 0) is the release
// call at +0x08 in func_80233760, v11 (index 9) drives the two panes at
// +0x2C in func_80234FDC, v14 (index 12) is the state-advance at +0x38
// driven by func_8023359C.
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
// font handle pushed onto the layout root pane by func_8013676C.
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

class CMenuArtsSet {
public:
    CMenuArtsSet();

    void func_80231848(void* src);
    u8 func_80233880();
    u32 func_80233968();
    void func_802349F8(u8 val);
    u32 func_80234A00();

    // +0x00: vtable (placeholder for the 4-byte vtable pointer)
    void* _vtable;                       // 0x00
    UnkClass_8045F564 mMemRegion;        // 0x04-0x13
    CFileHandle* field_0x14;             // 0x14 file handle (func_801390E0)
    nw4r::lyt::ArcResourceAccessor* field_0x18; // 0x18 (releaseArcResourceAccessor)
    SArts1C* field_0x1C;                 // 0x1C
    nw4r::lyt::AnimTransform* field_0x20; // 0x20
    nw4r::lyt::AnimTransform* field_0x24; // 0x24
    u8 field_0x28;                       // 0x28
    char _pad_29[0x2C - 0x29];   // 0x29-0x2B
    s32 field_0x2C;                // 0x2C
    char _pad_30[0x31 - 0x30];   // 0x30
    u8 mField31;                   // 0x31
    char _pad_32[0x34 - 0x32];   // 0x32-0x33
    u8 field_0x34[0x10];          // 0x34-0x43 opaque (draw object)
    float mField44;                // 0x44
    float mField48;                // 0x48
    char _pad_4C[0x58 - 0x4C];   // 0x4C-0x57
    char mSubObj58_start;          // 0x58 - sub-object (virtual interface, thunks adjust from here)
    char _pad_59[0x74 - 0x59];   // 0x59-0x73
    CArtsInfo mSubObj74;           // 0x74-0xE7
    CSysWinFull mSubObjE8;         // 0xE8 (field_34 read by CSysWin_getUnk34)
    char _pad_11F[0x124 - 0x11F];           // 0x11F-0x123
    SArtsSub8022FA58Short mSubObj124;       // 0x124-0x147 (field_0x15 = abs 0x139)
    // 0x148: busy/armed sub-object; 0x174: arts-table cursor object. The two
    // views overlap (mList174 aliases mSubObj148's tail bytes 0x2C-0x33 and
    // extends to 0x2A2, where its 0x12C/0x12D/0x12E bytes alias the shared
    // 0x2A0/0x2A1/0x2A2 flags), so they are declared as a union. Both arms
    // span 0x148-0x2A2 (0x15B bytes). mList174 is a real member so callers
    // re-derive addi r3, r31, 0x174 per call instead of CSE-caching it.
    union {
        struct {
            SArtsSub8022FA58 mSubObj148;        // 0x148-0x17B
            char _pad_17C[0x196 - 0x17C];       // 0x17C-0x195
            u8 field_0x196;                     // 0x196
            char _pad_197[0x2A1 - 0x197];       // 0x197-0x2A0
            u8 field_0x2A1;                     // 0x2A1 (arts menu busy flag)
            u8 field_0x2A2;                     // 0x2A2 (draw-active flag)
        };
        struct {
            char _pad_148[0x2C];                // 0x148-0x173
            SArts322BC mList174;                // 0x174-0x2A2
        };
    };
    // NOTE: the union has 4-byte alignment (mList174 contains a pointer), so
    // its storage spans 0x148-0x2A4 even though the arms logically end at
    // 0x2A2 - the following pad starts at 0x2A4 to keep field_0x2A6 at
    // absolute 0x2A6.
    char _pad_2A4[0x2A6 - 0x2A4];           // 0x2A4-0x2A5
    u8 field_0x2A6;                         // 0x2A6
};

// Vtable view of the CSysWin sub-object at CArtsList::0xE8 (func_8023352C).
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
// __ct__UnkClass_8011C974) plus the scalar fields through abs 0x71.
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

class CArtsList {
public:
    CArtsList();
    ~CArtsList();
    int OnFileEvent(CEventFile* pEventFile);

    void* mVtbl;                    // 0x00 - retail vtable lbl_eu_80536908
    UnkClass_8045F564 mMemRegion;   // 0x04 (0x10 bytes)
    CFileHandle* field_0x14;        // 0x14  file handle
    nw4r::lyt::ArcResourceAccessor* field_0x18;  // 0x18 arc accessor
    SArts1C* field_0x1C;            // 0x1C layout driver
    nw4r::lyt::AnimTransform* field_0x20;        // 0x20
    nw4r::lyt::AnimTransform* field_0x24;        // 0x24
    u8 field_0x28;                  // 0x28
    char _pad_29[0x2C - 0x29];
    s32 field_0x2C;                 // 0x2C
    u8 field_0x30;                  // 0x30
    u8 field_0x31;                  // 0x31
    char _pad_32[0x34 - 0x32];
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

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" void func_801F34F4(void*);
// CArtsList vtable (.data). Declared as an array so MWCC emits lis/addi
// (incomplete array types are not sdata-eligible).
extern "C" void* lbl_eu_80536908[];
// Sub-object ctor/dtor/body-copy imports for CArtsList::CArtsList's temp
// copy-init (retail emits the unmangled names at the call sites).
extern "C" void __ct__CScrollBar(CScrollBar* self, int arg);
extern "C" void __dt__10CScrollBarFv(void* self, int flags);
extern "C" void __ct__CArtsInfo(void* self);
extern "C" void __dt__9CArtsInfoFv(void* self, int flags);
extern "C" void __ct__CSysWin(void* self, int arg);
extern "C" void __dt__7CSysWinFv(void* self, int flags);
extern "C" void __ct__UnkClass_8011C974(void* dest, void* src);
// OnFileEvent imports: mem-region resize / font / layout-text helpers
// (retail emits the unmangled names at the call sites).
extern "C" void* createRegion__17UnkClass_8045F564FiiPCci(void*, int, int, const char*, int);
extern "C" void setMemInitFlag__Q23mtl10MemManagerFb(bool);
extern "C" bool Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(nw4r::lyt::ArcResourceAccessor*, void*, const char*);
extern "C" void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
extern "C" void func_8013676C(void*, u32);
extern "C" void* func_801355BC();
extern "C" void setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(nw4r::lyt::Layout*, char*, u32);
extern "C" u32 CSysWin_isReady(void*);
extern "C" u32 CScrollBar_isVisible(void*);
extern "C" void playUISound__FUl(u32);
extern "C" void* func_8009EC9C(u32);
extern "C" u32 func_800A32BC(void*);
// Arts-slot re-arm ctor (CtrlObjectParam.cpp; plain-C retail symbol).
extern "C" void __ct__8009F8B8(void* self);
// Actor-param refresh + player resolution tail of func_80230D74.
extern "C" void func_800A13C4(void* self, u32 flag);
extern "C" void* getPlayer__Q22cf13CfGameManagerFi(int index);
extern "C" void* getCfObjectPc__FPQ22cf12CfObjectMove(void* objMove);

// Player object returned by getCfObjectPc in func_80230D74's tail: the u16
// player id at +0x3F28 must equal the sub-object's 0x26 character id, then
// the refresh virtual at vtable +0x32C runs. With the 2-entry RTTI header,
// vtable +0x32C is declared index 201.
class SArtsPlayerRefresh {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075();
    virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091();
    virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111();
    virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115();
    virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123();
    virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127();
    virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131();
    virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135();
    virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147();
    virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151();
    virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155();
    virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159();
    virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163();
    virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167();
    virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171();
    virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175();
    virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179();
    virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183();
    virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187();
    virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191();
    virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195();
    virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199();
    virtual void v200();
    virtual void vf32C();   // index 201 -> vtable +0x32C

    u8 _pad04[0x3F28 - 0x4];
    u16 field_3F28;         // +0x3F28 player id
};

// CArtsInfo helpers defined in CArtsInfo.cpp (plain-C retail symbols).
extern "C" int func_80235A90(CArtsInfo* self);
extern "C" void func_80235AC0(CArtsInfo* self);
extern "C" void func_80235814(CArtsInfo* self);

// Table lookup helper (defined later in this TU). Plain-C retail symbol;
// kept out-of-line so callers emit a real bl instead of inlining.
extern "C" __declspec(noinline) u8 func_8023040C(SArtsSub8022FA58* self, u32 idx);

// BDAT message-table / texture-name helpers used by func_80230160 (imports).
extern void* lbl_eu_80664090;
extern "C" u32 func_80136254(const void* tbl, const void* key, int id);
extern "C" char* func_80138F78(u32 id);
extern "C" void func_80137E7C(nw4r::lyt::Layout* layout, const char* name, void* res);
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4();
extern "C" void func_80136B4C(nw4r::lyt::Layout*, char*, char*, int);
// Mangled retail symbol declared directly so the third argument stays a
// full register (retail passes the raw manager value without truncation).
extern "C" void setLayoutTextBoxNumber__FPQ34nw4r3lyt6LayoutPcUc(nw4r::lyt::Layout*, char*, int);

extern "C" void func_80230160(SArtsSub8022FA58* self);
extern "C" void func_802316F8(SArtsSub8022FA58* self);
extern "C" void func_80124270(void* p, u32 v);
// Scrollbar cursor refresh (defined in CScrollBar.cpp; C-linkage retail name).
extern "C" void func_801F3850(void* p, u16 v);
// Layout-out animation driver (same TU, C-linkage retail name).
extern "C" void func_8023185C(SArts313E0* self);

// Character-slot record view for func_80231014's mode-4/id-8 arm: the
// signed halfword at +0x24 is the current entry id (-1 = none).
struct SArtsCharData24 {
    char _pad[0x24];
    s16 field_0x24;
};
// Entry object returned by func_80157C4C; func_80231014 only reads the
// leading word (top 12 bits feed the category lookup).
struct SArtsEntryWord {
    u32 field_0x00;
};
// Mode-4/id-8 imports (code_80135FDC.cpp / CItem.cpp; C symbols).
extern "C" u16 func_80139358(u32 value);
extern "C" u8 func_801361E8(u32 entry, const char* text, u32 row);
extern "C" SArtsEntryWord* func_80157C4C(u32 kind);
// Current BDAT text-table handle consumed by func_801361E8 (.sdata word).
extern u32 lbl_eu_806640F8;

// Same-TU cursor/state helpers (retail unmangled names; definitions below).
extern "C" u8 func_80231014(SArtsSub8022FA58* self);
extern "C" u8 func_80231220(SArtsSub8022FA58* self);
extern "C" u8 func_802322BC(SArts322BC* self);
extern "C" u8 func_802322F4(SArts322BC* self, int key);
extern "C" u16 func_80232370(SArts322BC* self, int key);
extern "C" void func_802324C4(SArts322BC* self, int key);
extern "C" void func_80232B88(SArts327B0* self);
extern "C" void func_80232C78(SArts327B0* self);
extern "C" void copyVEC2(float* dst, const float* src);
extern "C" __declspec(noinline) int func_80234C84(SArts34C84* self);
extern "C" __declspec(noinline) int func_80234CA0(SArts34D14* self);
extern "C" __declspec(noinline) int func_80234D14(SArts34D14* self);
extern "C" u32 func_80234D68(CMenuArtsSet* self);

// Cross-unit CArtsInfo helpers (C-linkage, defined in CArtsInfo.cpp).
// Declared u32 so the ==0/!=0 tests compare the raw register (no rlwinm).
extern "C" u32 func_80235A98(CArtsInfo* self);
extern "C" void func_80235AA0(CArtsInfo* self);
extern "C" u32 func_80235F50(CArtsInfo* self);
// CArtsInfo cursor-step helpers (func_80233A50 / func_80233C08 busy paths).
extern "C" void func_80235EF0(CArtsInfo* self);
extern "C" void func_80235F14(CArtsInfo* self);
// CArtsInfo state-machine advance (defined in CArtsInfo.cpp; C symbol).
extern "C" void func_80235D24(CArtsInfo* self);
// CArtsInfo advance helpers used by func_802340C4 (CArtsInfo.cpp; C symbols).
extern "C" u32 func_80235F3C(CArtsInfo* self);
extern "C" void func_80235DD8(CArtsInfo* self);
extern "C" void func_80235AE0(CArtsInfo* self);
// Locked-entry refresh driven by func_802340C4 when the 0x168 mode is 4.
extern "C" void func_80234844(CMenuArtsSet* self);
// CSysWin active/advance helpers (defined in CSysWin.cpp; C symbols).
extern "C" int CSysWin_isActive(void* self);
extern "C" void func_8022B8E4(void* self);
// CArtsInfo draw (defined in CArtsInfo.cpp; C-linkage retail name).
extern "C" void func_80235958(CArtsInfo* self, void* drawInfo);
// CSysWin draw (defined in CSysWin.cpp; C-linkage retail name).
extern "C" void func_8022B7C8(CSysWinFull* self, nw4r::lyt::DrawInfo* drawInfo);
// Pane-visible check (defined in CTitleAHelp.cpp; C-linkage retail name).
extern "C" bool func_801C4648(nw4r::lyt::Pane* pane);
// C-linkage CSysWin state query (defined in CSysWin.cpp).
extern "C" u32 CSysWin_getUnk34(void* self);
// Drive/refresh helper (same TU, defined below; C-name for reloc parity).
extern "C" void func_80235124(CMenuArtsSet* self);

extern "C" void func_801F3540(void* obj34);
// Scroll-bar scroll-out request (func_80234FDC state-4 tail).
extern "C" void func_801F369C(void* obj34);
// Scroll-bar scroll-in with a 3-float init vector, and the tick that
// consumes it (func_80234EB8). Declared here rather than including
// CMenuPlayAward.hpp (same signature-clash reason as that header documents).
extern "C" void func_801F3670(u8* scrollBar, const float* vec);
extern "C" void func_801F367C(u8* scrollBar);
// Scroll-bar teardown (func_80233760).
extern "C" void func_801F35DC(void* obj34);
extern "C" void func_8023587C(CArtsInfo* self);
extern "C" void func_8022B748(CSysWinFull* self);
extern "C" void func_802306F0(SArts306F0* obj148);
extern "C" void func_80231648(SArts31648* self);
extern "C" void func_8023299C(SArts3150C* self);
extern "C" void func_80232AD8(SArts31648* self);
extern "C" void func_80231480(SArts31480* self);
extern "C" void func_802315BC(SArts315BC* self);
extern "C" void func_80231CB4(SArts306F0* obj148);
extern "C" void func_80234EB8(CMenuArtsSet* self);
extern "C" void func_80234928(CMenuArtsSet* self);
// Arts-table row-check (defined in this TU): returns 1 when the row for key
// (-1 = current cursor row) has state byte 4 with a clear +8 flag, or state
// byte 7 with a clear +9 flag.
extern "C" int func_80232638(SArts322BC* self, int key);
extern "C" void func_80234FDC(CMenuArtsSet* self);
extern "C" void func_80234F7C(CMenuArtsSet* self);
extern "C" void func_8023506C(SArts3506C* self);
extern "C" void func_802350B8(CMenuArtsSet* self);
// CMenuArtsSet sub-object release (func_80233760).
extern "C" void func_8023066C(SArts3066C* self);
// SArtsSub8022FA58 cursor-store helper (func_80234FDC). Same-TU definition;
// noinline so callers emit a real bl (retail shape).
extern "C" __declspec(noinline) void func_8023080C(SArtsSub8022FA58* self, u8 val);
// Same-TU release helpers kept out-of-line for retail bl parity.
extern "C" __declspec(noinline) void func_8022FD9C(SArts2FDF4* self);
extern "C" __declspec(noinline) void func_80231C30(SArts3066C* self);

// CMenuArtsSet state-machine / teardown / driver entry points (defined in
// this TU; C linkage matches the retail unmangled symbols).
extern "C" void func_80230374(SArtsSub8022FA58* self);
extern "C" void func_80231F60(SArts322BC* self);
extern "C" void func_80233760(CMenuArtsSet* self);
extern "C" void func_8023150C(SArts3150C* self);

// code_80135FDC helpers (retail C symbols; local decls because
// code_80135FDC.hpp cannot be co-included - func_80136190 signature clash).
extern "C" u8 code80135FDC_getByte_64077();
extern "C" u8 func_801392B4(u32);
extern "C" void func_80139198(u32);
// File-handle / arc-accessor teardown; retail symbols are the mangled C++
// names (closeFileHandle__FPP11CFileHandle etc.) so declare C++-linkage.
void func_801390E0(CFileHandle** self);
void releaseArcResourceAccessor(nw4r::lyt::ArcResourceAccessor* self);
// Layout/anim binding helpers (code_80135FDC.cpp). C++ linkage: MWCC mangles
// buildLayout/bindLayoutAnimTransform to the retail names (buildLayout__FPPQ...).
void buildLayout(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
void bindLayoutAnimTransform(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**, nw4r::lyt::ArcResourceAccessor*, char*);
// CArtsInfo teardown (defined in CArtsInfo.cpp, C symbol).
extern "C" void func_802359CC(CArtsInfo* self);
// CSysWinFull teardown (defined in CSysWin.cpp, C symbol).
extern "C" void func_8022B7F4(CSysWinFull* self);

// .sdata2 floats for the layout-animation drivers (func_80231480 etc.).
// const is load-bearing: it lets MWCC treat the pool load as a constant and
// hoist the lfs above the frame stores (retail position 3) - plain extern
// float places it after the stores (4-byte shift, MWCC_CASES).
extern const float lbl_eu_80668648;
extern const float lbl_eu_8066864C;
// .sdata2 floats for func_802316F8's 9-entry cursor position table.
extern const float lbl_eu_80668650;
extern const float lbl_eu_80668654;
// .sdata2 zero float for func_80231A48's pane-translate reset (SArtsVec2).
extern const float lbl_eu_80668658;
// .sdata2 floats for func_80234EB8's scroll-in vector (stack temp fed to
// func_801F3670).
extern const float lbl_eu_80668678;
extern const float lbl_eu_8066867C;

// Word blocks copied by the arts-table cursor drivers (func_80232B88 copies
// the 5-word lbl_eu_8050AC4C, func_802316F8 the 9-word lbl_eu_8050ABB4) via
// MWCC's counted block-copy path (lwzu/stwu loop).
struct SArtsB88Block {
    u32 w[5];   // 0x14 bytes
};
extern const SArtsB88Block lbl_eu_8050AC4C;
struct SArts316F8Block {
    u32 w[9];   // 0x24 bytes
};
extern const SArts316F8Block lbl_eu_8050ABB4;
// 9-word label block for the func_8023185C cursor refresh (field_0x21 twin
// of func_802316F8's lbl_eu_8050ABB4).
extern const SArts316F8Block lbl_eu_8050ABD8;

// Per-character arts-list block copied onto the stack by func_802320C0
// (18 bytes: nine {id,count} pairs, indexed by character; declared as
// words+tail so MWCC emits the counted block-copy loop).
struct SArts320C0Entry {
    u8 id;
    u8 count;
};
struct SArts320C0Block {
    u16 w[9];   // nine {id,count} pairs; flat array so MWCC emits the
                // counted block-copy loop (2x8 bytes + lhz tail)
};
extern const SArts320C0Block lbl_eu_8050ABFC;
// Scrollbar list rebuild (func_802320C0 tail).
extern "C" void func_801F36BC(void* scrollBar, u32 pageSize, u8 count);

// String-table lookup (func_80231220) and learn-arts flag-grid query
// (func_80231220); C-linkage retail symbols.
extern "C" u8 func_8013600C(const void*, const void*, u32);
extern "C" bool func_801F9268(unsigned char*, int, int);
// Accumulated pane translate (func_801375A0, defined in code_80135FDC.cpp)
// and pane translate setter (func_801D2150, defined in CSysWin.cpp).
extern "C" void func_801375A0(nw4r::math::VEC3* output, nw4r::lyt::Pane* pane);

// Window-arm chain used by func_80234A08: build a label string pair then
// drive the CSysWin sub-object. func_8022BFC8 is already declared in
// CSysWin.hpp as (CSysWin*, u8).
extern "C" char* func_80136190(char* a, char* b, u32 count);
extern "C" void func_8022B9B4(CSysWin* self, char* str, u32 flag);
extern "C" void func_8022B8B8(CSysWin* self);

// lib/lyt draw (func_801F3540).
extern "C" void func_801F35B0(void* obj34, nw4r::lyt::DrawInfo* info);

// func_80137510 is a flat C symbol (anim-frame check helper).
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);

// CArtsInfo field setters / refresh (defined in CArtsInfo.cpp; C symbols).
extern "C" void func_80235E84(CArtsInfo* self, u8 val);
extern "C" void func_80235E8C(CArtsInfo* self, u8 val);
extern "C" void func_80235E94(CArtsInfo* self, u8 val);
extern "C" void func_80235E9C(CArtsInfo* self, u16 val);
extern "C" void func_80235EA4(CArtsInfo* self);
// Arts-element scan / character-data refresh (C symbols).
extern "C" void func_80280DBC(u8* self);
extern "C" void func_800A1370(void*);
// BDAT string -> u16 (code_80135FDC.cpp). u32 3rd arg: retail passes the
// raw int (no clrlwi at the call site).
extern "C" u16 func_8013606C(const void*, const void*, u32);
// sprintf (varargs): C declaration mirrors CArtsInfo.hpp (crclr cr1eq at
// the call site comes from the varargs ABI).
extern "C" int sprintf(char*, const char*, ...);

// ---------------------------------------------------------------------------
// func_8022F544 (per-frame input dispatch) callees
// ---------------------------------------------------------------------------
// Party-change notice gate (CUICfManager.cpp).
extern "C" int func_8029A658();
// Same-TU dispatch helpers (definitions below; C linkage binds the calls to
// the retail unmangled symbols).
extern "C" u8 func_80233888(SArts33888* self);
extern "C" void func_80234A08(CMenuArtsSet* self);
extern "C" void func_80234780(CMenuArtsSet* self);
extern "C" void func_802346BC(CMenuArtsSet* self);
extern "C" void func_802340C4(CMenuArtsSet* self);
extern "C" void func_80233F78(CMenuArtsSet* self);
extern "C" void func_802339D4(CMenuArtsSet* self);
extern "C" void func_80233A50(CMenuArtsSet* self);
extern "C" void func_80233C08(CMenuArtsSet* self);
extern "C" void func_80233DC0(CMenuArtsSet* self);
extern "C" void func_80233E9C(CMenuArtsSet* self);
extern "C" void func_80234A94(CMenuArtsSet* self);
extern "C" u8 func_8023390C(CMenuArtsSet* self);
// Scene/fade helpers (imports).
extern "C" int func_800FEDF8();
extern "C" void func_800FF914();
// Title/help bar close + mode-set (CTitleAHelp.cpp; free C symbols - the
// member declarations in CTitleAHelp.hpp mangle differently).
extern "C" void func_801C414C(CTitleAHelp* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 mode);
// Member thunks called through their retail unmangled free-symbol forms by
// func_8022F544 (the member declarations mangle differently).
extern "C" void func_802349F8(CMenuArtsSet* self, u8 val);
extern "C" u32 func_80234A00(CMenuArtsSet* self);
extern "C" u32 func_80233968(CMenuArtsSet* self);