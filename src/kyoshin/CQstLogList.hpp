#pragma once

#include <types.h>
#include <nw4r/lyt.h>

#include "kyoshin/CSortMenu.hpp"  // CScrollBarData (0x40-byte scrollbar), CSortMenu, CScrollBar_isVisible

namespace nw4r { namespace math { struct VEC3; } }

/* 0x22-byte quest-log entry record, copied wholesale by func_80227994.
   Layout matches the retail copy: 2 head bytes + 4x8-byte word loop. */
struct CQstLogListEntry {
    s8 mField0;          // 0x0 (retail li r5,-1 store - signed)
    u8 mField1;          // 0x1
    u8 mData[0x20];      // 0x2..0x21
};

// 8-byte quest-info record (mirror of QstInfo in CMenuQstCnt.hpp).
struct CQstLogListQstInfo {
    u16 f0;              // 0x0 quest id
    u8  f2;              // 0x2
    u8  f3;              // 0x3
    u8  f4;              // 0x4
    u8  f5;              // 0x5
    u8  f6;              // 0x6
    u8  f7;              // 0x7
};

// Quest-info buffer maintained by func_80226FAC (mirror of QstData in
// CMenuQstCnt.cpp): an 8x0x400 array whose elements are destroyed via
// __dt__80227030, plus a 6-byte tail.
struct CQstLogListQstData {
    u8   mList[8 * 0x400];  // 0x0000 (0x2000 bytes)
    u16  field_2000;        // 0x2000
    s16  field_2002;        // 0x2002
    u8   field_2004;        // 0x2004
    u8   field_2005;        // 0x2005
};

// Mirror of the CSortMenu body (0xF0 bytes) for raw field copies in the
// constructor (the real CSortMenu class cannot be embedded - include
// conflict). Pointer fields are u32 because the region is opaque storage.
struct CQstLogListSortMenuData {
    u32 mVtbl;                 // +0x00
    u8  mUnk04[0x10];          // +0x04 UnkClass_8045F564
    u32 mFileHandle;           // +0x14
    u32 mArcResAcc;            // +0x18
    u32 mpLayout;              // +0x1C
    u32 mpAnimTrans0;          // +0x20
    u32 mpAnimTrans1;          // +0x24
    u8  field_0x28;            // +0x28
    u8  field_0x29;            // +0x29
    u8  field_0x2A;            // +0x2A
    u8  field_0x2B;            // +0x2B
    CScrollBarData mScrollBar; // +0x2C (0x40 bytes)
    s32 mArray[32];            // +0x6C (0x80 bytes)
    u8  mCount;                // +0xEC
    u8  mPage;                 // +0xED
    u8  mSubPage;              // +0xEE
};

/* Quest log list widget. Owned by CMenuQuestLog.
   Sorting: mSortEnabled (0x179) gates sort/filter logic,
   mSortDescending (0x17A) flips the sort direction arrow.

   Layout (from retail ASM):
     +0x00: vtable pointer
     +0x04: UnkClass_8045F564 sub-object (0x10 bytes)
     +0x14: CFileHandle* (readFile result from func_80227A60)
     +0x18: ArcResourceAccessor* (texture lookups in func_802286F4)
     +0x1C: nw4r layout
     +0x20/+0x24: animation transforms
     +0x28: CCur18 cursor sub-object (0x18 bytes)
     +0x40: CScrollBar (0x40 bytes)
     +0x80: CSortMenu (0xF0 bytes)
     +0x170: layout-loaded flag
     +0x182: quest-info buffer (8x0x400 array + 6-byte tail, 0x2006 bytes) */
// Field view of the opaque CCur18 sub-object (CBaseCur layout + padding).
// Used to copy a stack-constructed CCur18 into the embedded mCur18 region
// without touching the +0x00 vtable pointer (retail copies +0x04..+0x15).
struct CCur18Data {
    void* mVtbl;    // +0x00 - not copied
    void* field_4;  // +0x04
    void* field_8;  // +0x08
    void* field_C;  // +0x0C
    void* field_10; // +0x10
    u8 field_14;    // +0x14
    u8 field_15;    // +0x15
};

// Overlay structs for the 'timg' message resource consumed by OnFileEvent:
// the resource object holds a chain whose first field points at the u16
// row/column pair used to size the quest-log panes.
struct CQstLogListCoords {
    u16 c0;  // 0x0 column
    u16 c2;  // 0x2 row
};
struct CQstLogListMsgChain {
    CQstLogListCoords* pCoords;  // 0x0
};
struct CQstLogListMsgObj {
    u8 gap[0x8];
    CQstLogListMsgChain* chain;  // 0x8
};

struct CQstLogList {
    CQstLogList(u16 arg2);   // retail symbol __ct__CQstLogList (unmangled ctor)
    u8 func_80227CCC();
    u8 func_80227CD4();
    int OnFileEvent(CEventFile* event);   // file-load completion callback
    ~CQstLogList();

    u32 mVtable;                     // 0x00..0x03 (vtable pointer)
    u8 mUnk04[0x10];                 // 0x04..0x13 UnkClass_8045F564
    CFileHandle* mFileHandle;        // 0x14 - loaded arc file handle
    nw4r::lyt::ArcResourceAccessor* mArcResAcc;  // 0x18 - resource accessor
    nw4r::lyt::Layout* mpLayout;     // 0x1C - layout (func_80136A1C target)
    nw4r::lyt::AnimTransform* mpAnim0;  // 0x20 - animation transform 0
    nw4r::lyt::AnimTransform* mpAnim1;  // 0x24 - animation transform 1
    u8 mCur18[0x18];                 // 0x28..0x3F CCur18 cursor sub-object
    CScrollBarData mScrollBar;       // 0x40..0x7F - quest list scroll bar
    CQstLogListSortMenuData mSortMenuData;  // 0x80..0x16F CSortMenu (mirror storage)
    u8 field_0x170;                  // 0x170 - layout loaded flag
    u8 pad_0171[0x174 - 0x171];      // 0x171..0x173
    s32 field_0x174;                 // 0x174 - mode (0/2/5 set by anim checks)
    u8 field_0x178;                  // 0x178
    u8 mSortEnabled;                 // 0x179 - enables sorting/filtering (init=1)
    u8 mSortDescending;              // 0x17A - sort direction, 0=asc, 1=desc (init=0)
    s8 field_0x17B;                  // 0x17B - index byte (signed)
    s8 field_0x17C;                  // 0x17C - index byte (signed)
    s8 field_0x17D;                  // 0x17D - current row index byte (signed)
    s16 field_0x17E;                 // 0x17E - scroll position (signed halfword)
    u16 field_0x180;                 // 0x180 - quest-log list id (ctor arg)
    CQstLogListQstData mQstData;     // 0x182..0x2187 quest-info buffer (0x2006)
};

// In-TU helper functions. Retail symbols are unmangled (C linkage), so the
// declarations must be extern "C" to bind the bl reloc to the retail name.
extern "C" void func_802284E4(CQstLogList* self);
extern "C" void func_80228544(CQstLogList* self);
extern "C" void func_802285A4(CQstLogList* self);
extern "C" void func_80228B10(CQstLogList* self);
extern "C" void func_802286F4(CQstLogList* self, const char* name, int questId,
                              int index, int mode, u8 a6, u8 a7);
extern "C" void func_802289F8(CQstLogList* self);
extern "C" void func_80228C04(CQstLogList* self);
extern "C" void func_80228C98(CQstLogList* self);
extern "C" CQstLogListEntry* func_80227994(CQstLogListEntry* pDst, const CQstLogListEntry* pSrc);

// Abstract view into the embedded CCur18 cursor vtable (sibling copies live
// in CMapSel.hpp / CTitle.hpp). MWCC prefixes the vtable with offset-to-top +
// RTTI, so vtable +0x10 = index 2 = the "Move" virtual (takes a VEC3*).
class CCur18View {
public:
    virtual void vf02() = 0;                        // index 0 -> +0x08
    virtual void vf03() = 0;                        // index 1 -> +0x0C (retail passes no arg - r4 carries stale flag 1)
    virtual void vf04(const nw4r::math::VEC3*) = 0; // index 2 -> +0x10 - Move
};

// Sort-menu scroll/page handlers (defined in CSortMenu.cpp). Retail names are
// unmangled C linkage - keep extern "C" so the bl relocs bind to them.
extern "C" void func_801D3620(CSortMenu* _this);  // scroll up
extern "C" void func_801D3698(CSortMenu* _this);  // scroll down
extern "C" void func_801D3724(CSortMenu* _this);  // page up
extern "C" void func_801D377C(CSortMenu* _this);  // page down
extern "C" void func_801D3454(nw4r::math::VEC3* out, CSortMenu* _this);  // copy sort-menu state

// Imports with unmangled retail names (defined in CSortMenu.cpp / cursor TUs).
extern "C" int func_801D32DC(CSortMenu* _this);
extern "C" void func_801D350C(void*);                  // sort-menu array reset
extern "C" void func_801D3518(void*, void*);            // sort-menu array append
extern "C" void func_801D353C(void*, u8);               // sort-menu page set
extern "C" void func_801D216C(void*, u8);
extern "C" void func_801D3430(CQstLogListSortMenuData*, const nw4r::math::VEC3*);
extern "C" void func_801D3330(CQstLogListSortMenuData*);
extern "C" void func_801D202C(void*);                 // CCur18 per-frame update
extern "C" void func_801D3160(void*);                 // CSortMenu per-frame update
extern "C" void func_801D3258(void*);                 // CSortMenu destroy
extern "C" u8 func_801D3808(void*);                   // sort-menu selected page
extern "C" u8 func_801D3810(void*);                   // sort-menu selected entry

// Imports with retail names spelled verbatim (C linkage; MWCC would mangle
// plain declarations - the bl reloc must bind to the unmangled retail name).
extern "C" void func_801D3064(void*);
extern "C" void func_801D31F8(void*, void*);
extern "C" void func_801D20B0(void*, void*);
extern "C" void func_801D3408(void*);
extern "C" int func_801D3328(void*);
extern "C" void __dt__9CSortMenuFv(void*, int);
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __destroy_arr(void*, void*, int, int);
extern "C" void* __dt__80227030(void*, int);      // 0x400-byte QstInfo element dtor

// Sub-object constructor imports (retail C-linkage names).
extern "C" void __ct__17UnkClass_8045F564Fv(void*);
extern "C" void __ct__CCur18(u8*, nw4r::lyt::ArcResourceAccessor*);
extern "C" void __ct__CScrollBar(void*, int);
extern "C" void __ct__CSortMenu(CQstLogListSortMenuData*);
extern "C" void __ct__UnkClass_8011C974(void* self, const u32* src);   // 4-word mem-region copy
extern "C" void func_8011C998(CScrollBarData*, const CScrollBarData*);  // CScrollBar copy

// Quest-info buffer helpers (defined in CMenuQstCnt.cpp).
extern "C" void func_80226FAC(u8*);               // QstData construct (8x0x400)
extern "C" void copyQstInfo(u8*, const u8*);      // 8-byte QstInfo copy
extern "C" void func_80227260(u8*, u32, u32);
extern "C" u16 selectQstIndex(const u8*);
extern "C" CQstLogListQstInfo* func_802276F4(u8*, u16);
extern "C" u32 func_80138138(u16);
extern "C" void func_802270CC(u8*);               // QstData refresh
// Quest-list row count for the given quest-log list id (returns u16).
extern "C" u16 func_80227710(u8*, u16);

// BDAT helpers / msg-manager imports (C-ABI retail symbols).
extern "C" u32 func_8003B1EC(void*);              // BDAT row count
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4();
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int);
extern void* lbl_eu_806640A0;                      // BDAT table pointer (.sbss)

// Layout/text helpers with unmangled retail names (declared here; the
// code_80135FDC.hpp set is co-included for func_8013639C/func_80136B4C).
extern "C" u16 func_80136254(const void*, const void*, int);
extern "C" u8 func_8013600C(const void*, const void*, u32);
extern "C" void func_80137B44(nw4r::lyt::Layout*, const char*, u32);
extern "C" void func_80137F88(nw4r::lyt::Pane*, const char*);
extern "C" void func_80124270(nw4r::lyt::Pane*, u32);
extern "C" u32 func_8009CF8C(u32);

// C++ free functions - MWCC mangles these to the retail names
// (drawLayout__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii,
// playUISound__FUl).
void drawLayout(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
void playUISound(u32);          // UI sound effect

// Shared data pools (retail names; global scope - no C++ mangling).
extern char lbl_eu_80509AB4[];  // string pool (split1 .rodata)
extern char lbl_eu_80576670[];   // 0x22-byte quest text entry table
extern s32 lbl_eu_8066472C;      // quest-list page/scroll boundary (.sbss)
extern u32 lbl_eu_80664728;      // quest-list init flag (.sbss)
extern u32 lbl_eu_80664730;      // current quest id (.sbss)
extern u32 lbl_eu_80664734;      // current quest index (.sbss)
extern u32 lbl_eu_80664738;      // current quest resource (.sbss)
extern u32 lbl_eu_80662860;      // quest pane color (.sbss)
extern u8* lbl_eu_806640A8;      // BDAT table pointer (.sbss)
extern u32 lbl_eu_80573D18[];    // quest name table (split1 .rodata)
extern "C" u32 lbl_eu_80536288[];  // CQstLogList vtable (.data; array -> lis/addi)
// const routes into readonly sdata2 - MWCC hoists the lfs above the frame
// stores (MWCC_CASES:8787); plain extern float schedules it late.
extern const float lbl_eu_80668584;  // anim frame target (sda21)
// Scroll-bar init position constants (sda21 floats).
extern const float lbl_eu_80668578;
extern const float lbl_eu_8066857C;
extern const float lbl_eu_80668580;
