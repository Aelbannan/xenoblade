#pragma once

#include <types.h>
#include <nw4r/lyt.h>

#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "kyoshin/CSysWin.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"

// Forward declarations for types used in CFloorMap
class CScrollBar;
class CSysWin;
class CFileHandle;
class CEventFile;
class UnkClass_8045F564;


// 8-byte entry in the layout pointer array at 0x150
struct UnkLayoutEntry {
    u32 mPtr;   // +0x00: layout pointer
    u32 mUnk;   // +0x04: unknown
};

// Full object layout for CFloorMap (used by C-linkage accessors)
struct CFloorMapFull {
    u8 _00[0x08];                // 0x00-0x07
    nw4r::lyt::Pane* field_08;   // 0x08 - map root pane
    u8 field_0C;                 // 0x0C - current map-row id
    u8 _0D[0x24 - 0x0D];         // 0x0D-0x23
    u32 field_24;            // 0x24 - file handle
    u32 field_28;            // 0x28 - file handle
    u32 field_2C;            // 0x2C - file handle
    u32 field_30;            // 0x30
    u32 field_34;            // 0x34 - ArcResourceAccessor
    u32 field_38;            // 0x38 - ArcResourceAccessor
    u32 field_3C;            // 0x3C - allocated buffer pointer
    u8 field_40;             // 0x40 - draw/visibility flag A
    u8 field_41;             // 0x41 - draw/visibility flag B
    u8 field_42;             // 0x42 - draw/visibility flag C
    u8 _43[0x44 - 0x43];     // 0x43
    f32 pos_x_44;            // 0x44 - map-cursor x position
    f32 pos_y_48;            // 0x48 - map-cursor y position
    f32 field_4C;            // 0x4C - computed map-cursor x
    f32 field_50;            // 0x50 - computed map-cursor y
    f32 field_54;            // 0x54 - position divisor/scale
    u8 field_58;             // 0x58 - cursor active flag
    u8 _59[0x5A - 0x59];     // 0x59
    u16 field_5A;            // 0x5A-0x5B - map-row id
    u8 field_5C;             // 0x5C - cursor index (s8)
    u8 field_5D;             // 0x5D - cursor refresh-pending flag
    u8 _5E[0x60 - 0x5E];     // 0x5E-0x5F
    u32 mScrollBar;          // 0x60 - scrollbar widget
    u8 _64[0xA0 - 0x64];     // 0x64-0x9F
    u32 mCursor;             // 0xA0 - cursor widget
    u8 _A4[0xB8 - 0xA4];     // 0xA4-0xB7
    u32 mSysWinB8;           // 0xB8 - system window
    u8 _BC[0xF4 - 0xBC];     // 0xBC-0xF3
    u32 mSysWinF4;           // 0xF4 - system window
    u8 _F8[0x130 - 0xF8];    // 0xF8-0x12F
    u32 mLayout130;          // 0x130 - layout pointer (IWorkEvent)
    u8 _134[0x138 - 0x134];  // 0x134-0x137
    u32 mLayout138;          // 0x138 - layout pointer (IWorkEvent)
    u8 _13C[0x140 - 0x13C];  // 0x13C-0x13F
    u32 mLayout140;          // 0x140 - layout pointer
    u8 _144[0x150 - 0x144];  // 0x144-0x14F
    UnkLayoutEntry mLayouts150[0x14]; // 0x150-0x1F0 - array of 20 8-byte entries
    u8 field_1F0;            // 0x1F0 - count for layout array
    u8 _1F1[0x1F8 - 0x1F1];  // 0x1F1-0x1F7
    u8 field_1F8;            // 0x1F8 - current-floor id for the 0x150 layout loop
    u8 _1F9[0x1FC - 0x1F9];  // 0x1F9-0x1FB
    u32 mLayout1FC;          // 0x1FC - layout pointer (IWorkEvent)
    u8 _200[0x204 - 0x200];  // 0x200-0x203
    u8 field_204;            // 0x204
    u8 field_205;            // 0x205 (s8)
    s8 field_206;            // 0x206
    u8 field_207;            // 0x207
    u8 field_208;            // 0x208
    u8 _209[0x32CC - 0x209]; // 0x209-0x32CB
    u8 field_32CC;           // 0x32CC
    u8 _32CD[0x32D4 - 0x32CD]; // 0x32CD-0x32D3
    u32 mLayout32D4;         // 0x32D4 - layout pointer (IWorkEvent)
    u32 mLayout32D8;         // 0x32D8
    u32 mLayout32DC;         // 0x32DC
    u32 mLayout32E0;         // 0x32E0
    u8 field_32E4;           // 0x32E4
    u8 field_32E5;           // 0x32E5
    u8 _32E6[0x32EC - 0x32E6]; // 0x32E6-0x32EB
    u32 mLayout32EC;         // 0x32EC - layout pointer
    u32 mLayout32F0;         // 0x32F0
    u32 mLayout32F4;         // 0x32F4
    u32 mLayout32F8;         // 0x32F8
    u8 field_32FC;           // 0x32FC
    u8 field_32FD;           // 0x32FD
    u8 _32FE[0x3304 - 0x32FE]; // 0x32FE-0x3303
    u32 mLayout3304;         // 0x3304 - layout pointer
    u32 mLayout3308;         // 0x3308
    u32 mLayout330C;         // 0x330C
    u32 mLayout3310;         // 0x3310
    u8 field_3314;           // 0x3314
    u8 field_3315;           // 0x3315
    u8 _3316[0x331C - 0x3316]; // 0x3316-0x331B
    u32 mLayout331C;         // 0x331C - layout pointer
    u32 mLayout3320;         // 0x3320
    u32 mLayout3324;         // 0x3324
    u32 mLayout3328;         // 0x3328
    u8 field_332C;           // 0x332C
    u8 field_332D;           // 0x332D
    u8 _332E[0x3330 - 0x332E]; // 0x332E-0x332F
    u32 field_3330;          // 0x3330 - scrollbar widget
    u32 mLayout3334;         // 0x3334 - layout pointer
    u8 _3338[0x333C - 0x3338]; // 0x3338-0x333B
    u8 field_333C;           // 0x333C
    u8* field_3340;          // 0x3340 - allocated buffer pointer
    f32 field_3344;          // 0x3344 - buffer free countdown timer
};

// 0x18-byte map-marker record shared by the local entry table (built by
// FloorMap_RebuildMarkers) and the per-floor marker lists the entries are merged into.
struct CFloorMapMarkerEntry {
    CFloorMapMarkerEntry() {}
    u16 id;      // +0x00 row/map id
    u8 type;     // +0x02 marker type
    u8 _03;      // +0x03
    f32 x;       // +0x04
    f32 y;       // +0x08
    f32 z;       // +0x0C
    u32 unk;     // +0x10
    u8 flag;     // +0x14
    u8 _15[0x18 - 0x15];
};

// 0x30C-byte floor entry of the floor-map cursor (16 of them at cursor
// +0x10): floor id, floor-name id/height, floor-name string, up to 32 row
// markers and the row count. The row records' empty ctor is what leaves
// retail's collapsed array-ctor range test in the cursor ctor.
// name-guess: CFloorMapFloorEntry (no retail name for this record)
struct CFloorMapFloorEntry {
    u8 mFloorId;                       // +0x000
    u16 mNameId;                       // +0x002
    char* mName;                       // +0x004
    CFloorMapMarkerEntry mRows[32];    // +0x008
    u8 mRowCount;                      // +0x308
};

// Floor-map row cursors. Class names, vtables (__vt__9CFloorCur,
// __vt__12CFloorMarker, __vt__8CLandCur, __vt__10CTargetCur) and RTTI are
// retail (JP symbols); the single virtual slot is the layout loader.
// name-guess: initLayout and the member names (retail only names the classes).
class CFloorCur {
public:
    CFloorCur(nw4r::lyt::ArcResourceAccessor* acc = NULL)
        : mpLayout(NULL), mArcResAcc(acc), mpAnimTrans0(NULL), mpAnimTrans1(NULL),
          mActive(0), mVisible(1) {}
    virtual void initLayout();

    nw4r::lyt::Layout* mpLayout;                 // +0x04
    nw4r::lyt::ArcResourceAccessor* mArcResAcc;  // +0x08
    nw4r::lyt::AnimTransform* mpAnimTrans0;      // +0x0C
    nw4r::lyt::AnimTransform* mpAnimTrans1;      // +0x10
    u8 mActive;                                  // +0x14
    u8 mVisible;                                 // +0x15
};

class CFloorMarker : public CFloorCur {
public:
    CFloorMarker(nw4r::lyt::ArcResourceAccessor* acc = NULL) : CFloorCur(acc) {}
    virtual void initLayout();
};

class CLandCur : public CFloorCur {
public:
    CLandCur(nw4r::lyt::ArcResourceAccessor* acc = NULL) : CFloorCur(acc) {}
    virtual void initLayout();
};

class CTargetCur : public CFloorCur {
public:
    CTargetCur(nw4r::lyt::ArcResourceAccessor* acc = NULL) : CFloorCur(acc) {}
    virtual void initLayout();
};

// Floor-map cursor (CFloorMap+0x1FC, 0x3138 bytes): floor list layout,
// current floor/page/row, 16 floor entries and the four row cursors.
// name-guess: CFloorMapCursor / Init (retail keeps no name for the class;
// its ctor is the unnamed __ct__80244F50).
class CScrollBar;
class CFloorMapCursor {
public:
    CFloorMapCursor(CScrollBar* scrollBar, nw4r::lyt::ArcResourceAccessor* acc, u8 floor);
    ~CFloorMapCursor();
    void Init();

    nw4r::lyt::Layout* mData;                    // +0x0000
    nw4r::lyt::ArcResourceAccessor* mArcResAcc;  // +0x0004
    u8 field_08;                                 // +0x0008 - current-floor flag
    s8 field_09;                                 // +0x0009 - floor index
    s8 field_0A;                                 // +0x000A - page index
    s8 field_0B;                                 // +0x000B - row index
    bool field_0C;                               // +0x000C
    CFloorMapFloorEntry mEntries[16];            // +0x0010
    u8 field_30D0;                               // +0x30D0 - floor count
    CFloorCur mFloorCur;                         // +0x30D4
    CFloorMarker mFloorMarker;                   // +0x30EC
    CLandCur mLandCur;                           // +0x3104
    CTargetCur mTargetCur;                       // +0x311C
    CScrollBar* field_3134;                      // +0x3134 - scrollbar widget
};

// One 8-byte slot of the 20-entry layout array at CFloorMap+0x150: the
// loaded layout plus the truncated world-y of the row marker written by
// FloorMap_RefreshLayoutBlock.
// name-guess: CFloorMapLayoutSlot / CFloorMapLayoutBlock
struct CFloorMapLayoutSlot {
    CFloorMapLayoutSlot() : layout(NULL), posY(0) {}
    nw4r::lyt::Layout* layout; // +0x00
    s32 posY;                  // +0x04
};

// Layout block at CFloorMap+0x150 (the object FloorMap_RefreshLayoutBlock
// works on): 20 layout slots, the populated-slot count, the shared arc
// resource accessor and the current-floor id.
class CFloorMapLayoutBlock {
public:
    CFloorMapLayoutBlock(nw4r::lyt::ArcResourceAccessor* acc = NULL)
        : count(0), accessor(acc), curFloor(0) {
        for (int i = 0; i < 0x14; i++) {
            slots[i].layout = NULL;
            slots[i].posY = 0;
        }
    }
    ~CFloorMapLayoutBlock();
    CFloorMapLayoutSlot slots[20];            // +0x00..0x9F
    u8 count;                                 // +0xA0
    nw4r::lyt::ArcResourceAccessor* accessor; // +0xA4
    u8 curFloor;                              // +0xA8 - current-floor id
};

// Loaded map-image buffer plus its free countdown (CFloorMap+0x3340); the
// pair is assigned and copied as one 8-byte object.
// name-guess: CFloorMapBufTimer
struct CFloorMapBufTimer {
    CFloorMapBufTimer() {}
    CFloorMapBufTimer(u8* buf, f32 timer) : mpBuf(buf), mTimer(timer) {}
    u8* mpBuf;   // +0x00
    f32 mTimer;  // +0x04
};

// Full CFloorMap layout based on retail analysis.
// Total size: 0x3348 bytes. IWorkEvent-derived: the compiler-emitted
// __vt__9CFloorMap (.data 0x88: RTTI + 32 IWorkEvent slots) and the weak
// __RTTI__9CFloorMap (.sdata) come from this declaration.
class CFloorMap : public IWorkEvent {
public:
    CFloorMap();
    virtual ~CFloorMap();
    virtual bool OnFileEvent(CEventFile* event);
    u8 GetActiveFlag58();

    // Fields from CFloorMapFull (adjusted for vtable ptr at +0x00)
    // CFloorMapFull has _00[0x24] at 0x00 which includes the vtable ptr.
    UnkClass_8045F564 mMemRegion04;          // 0x04-0x13
    UnkClass_8045F564 mMemRegion14;          // 0x14-0x23
    u32 field_24;                            // 0x24
    u32 field_28;                            // 0x28
    u32 field_2C;                            // 0x2C
    u32 field_30;                            // 0x30
    u32 field_34;                            // 0x34
    u32 field_38;                            // 0x38
    u32 allocated_buf_3C;                    // 0x3C
    u8 field_40;                             // 0x40
    u8 field_41;                             // 0x41
    u8 field_42;                             // 0x42
    u8 field_43;                             // 0x43 - ctor sets 1
    f32 pos_x_44;                            // 0x44
    f32 pos_y_48;                            // 0x48
    f32 field_4C;                            // 0x4C
    f32 field_50;                            // 0x50
    f32 field_54;                            // 0x54
    u8 cursor_active_58;                     // 0x58
    u16 field_5A;                            // 0x5A-0x5B
    s8 cursor_idx_5C;                        // 0x5C
    u8 field_5D;                             // 0x5D
    CScrollBar mScrollBar;                   // 0x60-0x9F
    CCur18 mCursorA0;                        // 0xA0-0xB7
    CSysWin mSysWinB8;                       // 0xB8-0xF3
    CSysWin mSysWinF4;                       // 0xF4-0x12F
    u32 layout_130;                          // 0x130
    u32 field_134;                           // 0x134 - ArcResourceAccessor for layout_130
    u32 layout_138;                          // 0x138
    u32 field_13C;                           // 0x13C - ArcResourceAccessor for layout_138
    u32 layout_140;                          // 0x140
    u32 field_144;                           // 0x144 - ArcResourceAccessor for layout_140
    u32 field_148;                           // 0x148 - base row-marker pane (build temp)
    u8 field_14C;                            // 0x14C
    CFloorMapLayoutBlock mLayoutBlock;       // 0x150-0x1FB
    CFloorMapCursor mCursor;                 // 0x1FC-0x3333
    u32 layout_3334;                         // 0x3334
    u32 field_3338;                          // 0x3338 - ArcResourceAccessor for layout_3334
    bool field_333C;                         // 0x333C
    CFloorMapBufTimer mBufTimer;             // 0x3340
    void SetMapTable(u8 id);

    // name-guess: retail EU/US carry no names for these CFloorMap methods
    // (JP names only the ctor/dtor/OnFileEvent); called on the embedded
    // mFloorMap member by CMenuMapSelect / CMenuMapSelectSC.
    void LoadStageLayout();
    void LoadMapFiles();
    void UpdateFrame();
    void Draw(nw4r::lyt::DrawInfo* drawInfo);
    void Teardown();
    int GetField42Gated();
    int GetField40Flag();
    void UpdateRowPanePos();
    void CursorNextFloor();
    void UpdateCursorMove();
    void UpdateWorldMap();
    void HandleMapDown();
    void NormalizeMapPos();
    void ScrollDown();
    void ScrollUp();
    void ScrollRight();
    void ScrollLeft();
    void OpenRowWindow();
    u32 ClassifyByte41();
    u16 GetU16Field5A();
    void ConfirmSelect();
    void SetPaneFlag(u32 arg2);
    u8 GetBdatMaxRow();
    void Log2Field208();
    u8 GetField208Gated();
    u8 GetField208Plain();
    u32 CheckRowVisited();
    void Log2Field333C();
    u32 IsSysWinBusy();
    void MoveToCursorRow();
};

// ---- imports folded from the former CFloorMapImports.hpp ----
// Unit-private import declarations split out of CFloorMap.hpp so the class header
// stays includable next to other headers.
#include "kyoshin/CFloorMap.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"



// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" char lbl_eu_8050BEA8[];

// (Embedded sys-win sub-objects dispatch through CSysWin::loadSystemArc
// at +0x88; see kyoshin/CSysWin.hpp.)


// Same-TU helper definitions used by CFloorMap::OnFileEvent (retail flat
// names; the extern "C" declaration fixes the definition linkage).
struct CFloorMapLayoutData0;
struct CFloorMapRowList;
extern "C" void FloorMap_BuildLayoutBaseArc(CFloorMapLayoutData0*);
extern "C" void FloorMap_RefreshLayoutBlock(CFloorMapLayoutBlock*);
extern "C" u32 FloorMap_FindFloorRow(void*);
extern "C" void FloorMap_ResolveMapPos(void*, void*);
extern "C" void FloorMap_ReloadMarkerPanes(void*, u8, u32, f32);
extern "C" void FloorMap_PlaceFloorMarker(CFloorMapFull* self, u32 idx);
extern "C" void FloorMap_SetRowTexPalette(CFloorMapRowList*, void*, u32, u32, u32);

// Per-map layout-name table copied by OnFileEvent branch 2 (28 u32 names,
// .rodata).
extern const u32 lbl_eu_8050BAB0[];

// ============================================================================
// Sub-object layouts (from retail field analysis)
// ============================================================================

// Layout-init view for FloorMap_BuildLayoutBaseArc: layout pointer at +0x00 with the arc
// resource accessor right behind it at +0x04 (buildLayout takes
// &layout/accessor pair, then GetRootPane reads layout+0x10).
struct CFloorMapLayoutData0 {
    nw4r::lyt::Layout* layout;                // +0x00
    nw4r::lyt::ArcResourceAccessor* accessor; // +0x04
};

// 0x30C-stride floor-name entry written by FloorMap_RefreshFloorRows (base at
// cursor + fi*0x30C): the floor-id/name fields sit at +0x10..+0x17 (the
// marker-block header at base+0x10) and the row-count byte at +0x318 (the
// marker block's count at +0x308).
// 0x30C-stride floor-name entry written by
struct CFloorMapFloorNameEntry {
    u8 _00[0x10];
    u8 field_10;              // +0x10 - floor id (1-based)
    u8 _11;
    u16 field_12;             // +0x12 - floor-name id (from BdatGetS16Direct)
    u32 field_14;             // +0x14 - floor-name string (from BdatGetPtrDirect)
    u8 _18[0x318 - 0x18];
    u8 field_318;             // +0x318 - row/entry count
};


// View of one floor block's marker list at CFloorMapCursor + fi*0x30C + 0x10:
// a floor-height field at +0x02, the 0x18-byte marker records at +0x08 and
// the marker count byte at +0x308.
struct CFloorMapMarkerBlock {
    u8 _00[0x02];
    s16 height;                         // +0x02 - floor height, compared against marker y
    u8 _04[0x08 - 0x04];                // +0x04..0x07
    CFloorMapMarkerEntry records[32];   // +0x08, 32 x 0x18 = 0x300 -> ends at +0x308
    u8 count;                           // +0x308
};

// Global gimmick settings container returned by getUnk80664658 (flag word at
// +0x214); used by the marker flag check in FloorMap_RebuildMarkers.
struct CFloorMapGimmickGlobal {
    u8 _00[0x214];
    u32 field_214;   // +0x214
};


// Holder reached from CFloorMapCursor::field_3108; the pane to move is at +0x10.
struct CFloorMapCursorTarget {
    u8 _00[0x10];
    nw4r::lyt::Pane* pane; // +0x10
};

// 0x18-byte map row inside a floor block: the row-id u16 sits at +0x00 of
// each row; the row array starts at floorBlock+0x214. The byte at +0x02 and
// the pointer at +0x04 are handed to FloorMap_PlaceFloorMarker by OnFileEvent.
struct CFloorMapRow {
    u16 field_00;             // +0x00 row id
    u8 field_02;              // +0x02
    u8 _03;                   // +0x03
    u8 field_04[0x18 - 0x04]; // +0x04
};

// 0x30C-byte floor-block view (blocks live at CFloorMap + fi*0x30C; the
// row array starts at +0x214, the count byte at +0x514).
struct CFloorMapFloorBlock {
    u8 _00[0x20C];
    u8 field_20C;             // +0x20C
    u8 _20D[0x210 - 0x20D];
    char* name_210;           // +0x210
    CFloorMapRow rows[32];    // +0x214, 32 x 0x18 = 0x300 -> ends at +0x514
    u8 field_514;             // +0x514 row count
};

// Linked-list node view used by FloorMap_PickMarkerRow: the child panes of the map
// pane are walked through their raw ut::LinkListNode fields (node = Pane+4),
// so the embedded pane name sits at node+0xB8 (= Pane::mName at 0xBC).
struct CFloorMapChildNode {
    CFloorMapChildNode* mpNext; // +0x00
    u8 _04[0xB8 - 0x04];
    char name[0x20];           // +0xB8
};

// View of Pane::mChildList.mNode (Pane+0x14): mpNext at +0x00 is the first
// child's link node; the end sentinel is the mNode address itself.
struct CFloorMapChildList {
    CFloorMapChildNode* mpNext; // +0x00
    CFloorMapChildNode* mpPrev; // +0x04
};

// Child-list iterator view anchored at Pane+0x10: the child-list object
// starts at Pane+0x10 and the first child link sits at +0x14 (doubles as the
// end sentinel).
struct CFloorMapHolderChildren {
    u8 _00[0x10];
    CFloorMapChildNode* first; // +0x14
};

// Two-step anchor used by FloorMap_ReloadMarkerPanes (retail forms base+0x10 first, then
// reads the child-list node at +0x04): keeps MWCC from folding the address
// into a single pane+0x14 displacement.
struct CFloorMapGroupAnchor {
    u8 _00[4];
    CFloorMapChildList kids; // +0x04
};

// Translate view of an nw4r::lyt::Pane used by FloorMap_RefreshLayoutBlock's marker
// placement (writes +0x2C/+0x30/+0x34 directly).
struct CFloorMapPaneTransXYZ {
    u8 _00[0x2C];
    f32 trans2C; // +0x2C
    f32 trans30; // +0x30
    f32 trans34; // +0x34
};

// View of the layout-slot block passed to FloorMap_PickMarkerRow (the address of
// CFloorMap::mLayout140): +0x00 is the map Layout*, +0x08 the pane whose
// FindPaneByName resolves the row-marker pane names. +0x0C holds the
// current map-row id (read by FloorMap_PlaceMarkers's marker merge loop).
struct CFloorMapLayoutSlots {
    nw4r::lyt::Layout* layout; // +0x00
    u32 _04;                   // +0x04
    nw4r::lyt::Pane* pane;     // +0x08
    u8 field_0C;               // +0x0C - current map-row id
};

// (CCur18 cursors dispatch through CBaseCur::initLayout/cleanup/
// setRootPaneTranslate at +0x08/+0x0C/+0x10; see kyoshin/CBaseCur.hpp.)

// Object used by FloorMap_SetRowTexPalette (row-visibility update); only +0x00 is used.
struct CFloorMapRowList {
    nw4r::lyt::Layout* mData; // +0x00
};

// Per-frame widget updates (retail-unmangled C-linkage names; declared in
// COption.hpp / CItemBoxGrid.hpp / CMapSel.hpp under the same extern "C").
// C++-mangled imports from code_80135FDC.cpp (unmangled identifiers mangle to
// the retail symbols, same scheme as CItemBoxInfo.hpp).

// (Layout/Pane teardown goes through `delete`: the deleting-dtor vcall
// at +0x08 matches retail.)

// Advance the attached anim-transform to a frame (returns nonzero when done);
// retail symbol keeps the C++ mangled name advanceAnimTransform__FPQ34nw4r3lyt13AnimTransformf.

// (Player/marker positions dispatch through cf::CfObject: UVF23 at +0xAC,
// zoom through UVF31 at +0xCC; see kyoshin/cf/object/CfObject.hpp.)

// 3-float position view for the marker objects' GetPos() (vtable+0xAC).
struct CFloorMapVec3 {
    f32 x;                                   // +0x00
    f32 y;                                   // +0x04
    f32 z;                                   // +0x08
};

// Linked list of marker objects returned by prepareReslistArg/getReslistBC8/
// getReslistB68 (same layout as CMiniMap's MiniMapList): head sentinel at
// +0x04, node next at +0x00 and the object at +0x08.
struct CFloorMapObjList {
    u32 field_00;                            // +0x00
    struct CFloorMapObjNode* head;           // +0x04
};
struct CFloorMapObjNode {
    CFloorMapObjNode* next;                  // +0x00
    u8 _04[0x08 - 0x04];                     // 0x04-0x07
    void* object;                            // +0x08
};
// Marker object fields (non-polymorphic view): the vtable pointer stays
// at +0x00 so field offsets match retail. Position (+0xAC) and
// marker-pane (+0x160) slots dispatch through cf::CfObject (UVF23/UVF68).
struct CFloorMapObj {
    void* vtbl; // +0x00
    u8 _04[0x64 - 0x04];                   // 0x04-0x63
    u32 m64;                                 // 0x64 - flags
    u8 _68[0x74 - 0x68];
    u32 m74;                                 // 0x74 - name string
    u8 _78[0x8C - 0x78];
    u16 m8C;                                 // 0x8C - id
    u8 _8E[0x91 - 0x8E];
    u8 m91;                                  // 0x91 - marker kind (6 = landmark)
};

// 0x4C-byte landmark entry walked by FloorMap_PlaceWorldMarker case 2 (CPartsChange_GetLandmarkTable
// array); the id sits at +0x1C.
struct CFloorMapCase2Elem {
    f32 x;                                   // +0x00
    f32 y;                                   // +0x04
    f32 z;                                   // +0x08
    u8 _0C[0x1C - 0x0C];
    u16 m1C;                                 // +0x1C - id
    u8 _1E[0x4C - 0x1E];                     // stride 0x4C
};

// C-linkage name/table lookups used by the marker placement (retail flat names).
// Retail masks the raw results (rlwinm 24/16) after every call, so these
// return wider ints; callers apply the (u8)/(u16) truncations explicitly.
// (BdatGetU16ByTableKey: only CFloorMap.cpp needs the wider view, so its u32 copy
// lives TU-local there; the shared decl is the canonical u16 one on
// code_80135FDC.hpp.)
extern "C" void* prepareReslistArg(int);
extern "C" void* getReslistBC8();
extern "C" void* getReslistB68();
extern "C" void* CPartsChange_GetLandmarkTable();
extern "C" void bdatGetPosXYZ(float*, int = 0);

// C-linkage UI helper imports. buildLayout/bindLayoutAnimTransform keep their retail
// mangled names through MWCC's natural C++ mangling; the flat-named helpers
// need extern "C" so MWCC does not append a parameter suffix.
// C-linkage name/label helpers from code_80135FDC.cpp. BdatTouchStringCell builds
// a pane name from a format string, a base string and an index (JP retail
// symbol BdatTouchStringCell__FPcPcUl = (char*, char*, u32); fixed args so MWCC
// does not emit the varargs CR1 clear); LayoutSetTextBoxFmtValue updates a pane.
// C linkage so the same-TU definition emits the retail symbol
// FloorMap_RefreshCursorRows instead of the mangled C++ name at every call site.
extern "C" void FloorMap_RefreshCursorRows(void*);
#include "kyoshin/cf/CtrlRemoteApi.hpp"

// Flat C-ABI imports used by the map-cursor functions (retail symbols are
// unmangled). playUISound keeps C++ linkage so MWCC emits the retail
// mangled name playUISound__FUl.
extern "C" u32 FloorMap_PickMarkerRow(void*);
extern "C" void FloorMap_PlaceMarkersB(CFloorMapLayoutSlots*);
extern "C" void FloorMap_PlacePlayerMarkerScaled(CFloorMapFull*);
extern "C" void FloorMap_PlacePlayerMarker(void*);
extern "C" void FloorMap_PlaceWorldMarker(void*);
extern "C" void FloorMap_PlaceLandmarks(void*);
extern "C" void FloorMap_PlaceTypedMarkers(CFloorMapLayoutSlots*);
extern "C" void FloorMap_PlaceMarkers(void*);
extern "C" void FloorMap_PlaceMarkersD(CFloorMapLayoutSlots*);
extern "C" void FloorMap_PlaceMarkersC(CFloorMapLayoutSlots*);
// C linkage so the same-TU definition emits the retail plain name.
extern "C" void* FloorMap_CreateMarkerPicture(void*, const char*, f32, f32, void*, const char*);
extern "C" void FloorMap_SetCursorState(void*, u16);
extern "C" void FloorMap_CursorUp(void*);
extern "C" void FloorMap_AccumPaneTranslate(nw4r::math::VEC3*, void*, nw4r::lyt::Pane*);
extern void playUISound(u32);

// C-ABI imports used by the floor-map update functions (retail plain names).
// (code80135FDC_setVec3 comes from kyoshin/CSysWin.hpp: VEC3* form.)
extern "C" void copyVEC3(void*, const void*);
extern "C" u32 Bdat_GetMaxRow_B1EC(void* bdat);   // BDAT row count (canonical void* form)
extern "C" void* Bdat_GetTable_AA34();
extern "C" void TagProcMeasureText(f32* out, nw4r::lyt::Pane* pane, u16* str);
// getUnk80664658 is declared TU-locally in CFloorMap.cpp: the shared
// code_801F3BE0.hpp owner uses a different (C++-linkage) return type.


// CSysWin sub-object helpers used by the floor-map input handlers (retail
// flat names).
// (sysWinApplyKind comes from kyoshin/CSysWin.hpp: CSysWin* form.)

// .sbss globals for the floor-map update functions.
// .sbss color tables used by PaneMatSetTevColorsByName (init in sinit_80250CB4).
extern GXColorS10 sFloorRowColor0;
extern GXColorS10 sFloorRowColor1;
extern GXColorS10 sFoundRowColor0;
extern GXColorS10 sFoundRowColor1;
extern u32 lbl_eu_80664184;   // current world/map id (full word)

// Pane flag-byte view (Pane::mFlag is protected; retail reads/writes it
// directly at pane+0xBB).
struct CFloorMapPaneFlag {
    u8 _00[0xBB];
    u8 mFlag; // +0xBB
};

// Translate-x/y view of an nw4r::lyt::Pane (mTranslate at +0x2C is protected;
// retail rewrites only x/y of the marker picture, leaving z untouched).
struct CFloorMapPaneTransXY {
    u8 _00[0x2C];
    f32 transX; // +0x2C
    f32 transY; // +0x30
};

// Rotate view of an nw4r::lyt::Pane (mRotate at +0x38; retail stores the
// current-position marker's zoom scale into rotate.z).
struct CFloorMapPaneRotate {
    u8 _00[0x38];
    f32 rotX; // +0x38
    f32 rotY; // +0x3C
    f32 rotZ; // +0x40
};

// C-ABI helpers used by FloorMap_RefreshFloorRows (defined in code_80135FDC.cpp).
extern u32 spFloorDataTable;
extern u8 sCurrentMapId;

// MEM2 + device-file imports (retail mangled names, declared verbatim).
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);

// Layout-slot holder reached from the layout pointer fields (0x130/0x138/
// 0x140 and the 0x150 array): the root Pane of the layout is at +0x10.
struct CFloorMapLayoutHolder {
    u8 _00[0x10];
    nw4r::lyt::Pane* pane; // +0x10
};

// Retail called nw4r::lyt::Pane::SetTranslate with a by-value VEC3, so each
// call site copies the argument to a per-call stack temp with lwz/stw.  The
// decomp SDK header models SetTranslate as const-ref, which skips the copy.
// These inline helpers restore the by-value copy at the call site.  The POD
// _VEC3 argument makes MWCC emit the retail word copy; the cast re-views the
// temp as VEC3 for the inlined const-ref SetTranslate body.
static inline void CFloorMapSetPanePos(nw4r::lyt::Pane* pane, nw4r::math::_VEC3 v) {
    pane->SetTranslate(*(nw4r::math::VEC3*)&v);
}

static inline void CFloorMapSetSlotPos(CFloorMapLayoutHolder* slot, nw4r::math::_VEC3 v) {
    if (slot) {
        slot->pane->SetTranslate(*(nw4r::math::VEC3*)&v);
    }
}
// Result of the scale-source getter at manager +0x1C; +0x04 holds the map
// scale numerator used to size the current-position marker.
class CFloorMapMgrSrc {
public:
    u8 _00[4];
    f32 scale04; // +0x04
};

// Scale-source manager behind getCameraDataBlock(): +0x1C getter.
// IFactoryEvent supplies five real pre-slots (same pattern as
// pluginBtl's CBattleManager); never constructed here, so no vtable
// is emitted.
class CFloorMapMgr : public IWorkEvent {
public:
    virtual CFloorMapMgrSrc* getScaleSrc(); // +0x1C
};

// Per-table BDAT entry pointers + flat-name helpers used by FloorMap_PlaceTypedMarkers
// (retail symbols are unmangled).
extern void* lbl_eu_80573D18[0x1C];
// GimFindPosByRow is declared TU-locally in CFloorMap.cpp (the shared owner
// code_801F3BE0.hpp uses a different C++-linkage signature).

// (Font handles dispatch through IDeviceFontInfo::getFont at +0x24;
// see monolib/device/CDeviceFont.hpp.)

// Landmark table owned by CPartsChange: 0x200 entries followed by the live
// entry count (the count word is read by the floor-map marker placement).
struct CFloorMapLandmarkTable {
    CFloorMapCase2Elem entries[0x200]; // +0x0000
    u32 mCount;                        // +0x9800
};

