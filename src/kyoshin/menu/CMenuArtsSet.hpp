#pragma once

#include <types.h>
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CScrollBar.hpp"

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
    u8  field_0x19[7];     // 0x19-0x1F
    s8  field_0x20;
    u8  field_0x21;
    char _pad_0x22[0x24 - 0x22];
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
    u8  field_0x27;
    u32 field_0x28;
    u8  mTable[0x100];    // 0x2C-0x12B
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
    u32 field_0x28;
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

// Cursor/state object used by func_802327B0.
class SArts327B0 {
public:
    char _pad_00[0x20];
    u8 field_0x20;
    u8 field_0x21;
    char _pad_22[0x28 - 0x22];
    u8* field_0x28;   // 0x28
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
    char _pad_00[0x08];
    SArts080C* field_0x08;      // 0x08
    char _pad_0C[0x14 - 0x0C];
    SArts080C* field_0x14;      // 0x14
    char _pad_18[0x20 - 0x18];
    u8 field_0x20;              // 0x20
    u8 field_0x21;              // 0x21
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

// Polymorphic pointee with a driver method at vtable slot 0x2C (index 11).
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
    virtual void v9();
    virtual void v10();
    virtual void v11(void* a, int flag); // vtable +0x2C
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
    virtual void v0();
    virtual void v1();
    virtual void v2(int a); // vtable +0x08
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
    virtual void v14(int a); // vtable +0x38
};

// Object used by func_8022FD9C / func_8022FDF4.
class SArts2FDF4 {
public:
    char _pad_00[0x08];
    SArts2FObj* field_0x8;
    char _pad_0C[0x18 - 0x0C];
    u8 field_0x18;
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
    virtual void v124(); virtual void v125();
    virtual s32 v126(int a);   // vtable +0x1F8
    virtual void v127();
    virtual s32 v128();        // vtable +0x200
};

// Root returned by func_8009EC9C; the manager object sits at offset 0x17C.
struct SArtsManagerRoot {
    char _pad[0x17C];
    SArtsManager mObj17C;   // 0x17C
};

// Pointee of CMenuArtsSet.field_0x1C; vtable slot 14 (+0x38) is driven by
// func_8023359C etc.
class SArts1C {
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
    virtual void v14(int a);   // vtable +0x38
};

class CMenuArtsSet {
public:
    CMenuArtsSet();

    void func_80231848(void* src);
    u8 func_80233880();
    void func_80233968();
    void func_802349F8(u8 val);
    void func_80234A00();

    // +0x00: vtable (placeholder for the 4-byte vtable pointer)
    void* _vtable;                       // 0x00
    char _pad_04[0x1C - 0x04];   // 0x04-0x1B
    SArts1C* field_0x1C;                // 0x1C
    char _pad_20[0x24 - 0x20];   // 0x20-0x23
    nw4r::lyt::AnimTransform* field_0x24; // 0x24
    u8 field_0x28;                   // 0x28
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
    SArtsSub8022FA58 mSubObj148;            // 0x148-0x17B
    char _pad_17C[0x196 - 0x17C];           // 0x17C-0x195
    u8 field_0x196;                         // 0x196
    char _pad_197[0x2A6 - 0x197];           // 0x197-0x2A5
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

class CArtsList {
public:
    CArtsList();
    virtual ~CArtsList();
    void OnFileEvent();

    // vptr at 0x00 (implicit)
    UnkClass_8045F564 mMemRegion;   // 0x04 (0x10 bytes)
    u32 field_0x14;                 // 0x14  file handle
    char _pad18[0x30 - 0x18];       // 0x18-0x2F
    u8 field_0x30;                  // 0x30
    char _pad31[0x34 - 0x31];       // 0x31-0x33
    CScrollBar mScrollBar;          // 0x34 (0x40 bytes)
    CArtsInfo mSubObj74;            // 0x74 (0x74 bytes)
    CSysWin mSysWinE8;              // 0xE8
};

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" void func_801F34F4(void*);
extern "C" u32 CSysWin_isReady(void*);
extern "C" u32 CScrollBar_isVisible(void*);
extern "C" void func_80138078__FUl(u32);
extern "C" void* func_8009EC9C(u32);
extern "C" u32 func_800A32BC(void*);

// CArtsInfo helpers defined in CArtsInfo.cpp (plain-C retail symbols).
extern "C" int func_80235A90(CArtsInfo* self);
extern "C" void func_80235AC0(CArtsInfo* self);
extern "C" void func_80235814(CArtsInfo* self);

// Table lookup helper (defined later in this TU). Plain-C retail symbol;
// kept out-of-line so callers emit a real bl instead of inlining.
extern "C" __declspec(noinline) u8 func_8023040C(SArtsSub8022FA58* self, u32 idx);

extern "C" void func_80230160(SArtsSub8022FA58* self);
extern "C" void func_802316F8(SArtsSub8022FA58* self);
extern "C" void func_80124270(void* p, u32 v);

// Cross-unit CArtsInfo helpers (C-linkage, defined in CArtsInfo.cpp).
// Declared u32 so the ==0/!=0 tests compare the raw register (no rlwinm).
extern "C" u32 func_80235A98(CArtsInfo* self);
extern "C" void func_80235AA0(CArtsInfo* self);
extern "C" u32 func_80235F50(CArtsInfo* self);
// C-linkage CSysWin state query (defined in CSysWin.cpp).
extern "C" u32 CSysWin_getUnk34(void* self);
// Drive/refresh helper (same TU, defined below; C-name for reloc parity).
extern "C" void func_80235124(CMenuArtsSet* self);

extern "C" void func_801F3540(void* obj34);
extern "C" void func_8023587C(CArtsInfo* self);
extern "C" void func_8022B748(CSysWinFull* self);
extern "C" void func_802306F0(void* obj148);
extern "C" void func_80231CB4(void* obj174);
extern "C" void func_80234EB8(CMenuArtsSet* self);
extern "C" void func_80234FDC(CMenuArtsSet* self);
extern "C" void func_80234F7C(CMenuArtsSet* self);
extern "C" void func_8023506C(SArts3506C* self);
extern "C" void func_802350B8(CMenuArtsSet* self);

// lib/lyt draw (func_801F3540).
extern "C" void func_801F35B0(void* obj34, nw4r::lyt::DrawInfo* info);

// func_80137510 is a flat C symbol (anim-frame check helper).
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);