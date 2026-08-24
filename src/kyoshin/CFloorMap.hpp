#pragma once

#include <types.h>
#include <nw4r/lyt.h>

// Forward declarations for types used in CFloorMap
class CScrollBar;
class CBaseCur;
class CSysWin;
class CFileHandle;
class CEventFile;

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

// Full CFloorMap layout based on retail analysis.
// Total size: 0x3348 bytes.
// The vtable pointer is at offset 0x00. Like CSysWin, retail stores it
// manually from the constructor (no MWCC-emitted __vt__9CFloorMap), so the
// class declares no virtuals and the fields start right after the vtable ptr.
class CFloorMap {
public:
    CFloorMap();
    ~CFloorMap();
    u32 OnFileEvent(CEventFile* event);

    // Fields from CFloorMapFull (adjusted for vtable ptr at +0x00)
    // CFloorMapFull has _00[0x24] at 0x00 which includes the vtable ptr.
    void* mVtbl;                           // +0x00 - IWorkEvent vtable (set by ctor)
    u8 mMemRegion04[0x10];                 // 0x04-0x13 - UnkClass_8045F564
    u8 mMemRegion14[0x10];                   // 0x14-0x23 - UnkClass_8045F564
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
    u8 _43[0x44 - 0x43];                     // 0x43
    f32 pos_x_44;                            // 0x44
    f32 pos_y_48;                            // 0x48
    f32 field_4C;                            // 0x4C
    f32 field_50;                            // 0x50
    f32 field_54;                            // 0x54
    u8 cursor_active_58;                     // 0x58
    u8 _59[0x5A - 0x59];                     // 0x59
    u16 field_5A;                            // 0x5A-0x5B
    s8 cursor_idx_5C;                        // 0x5C
    u8 field_5D;                             // 0x5D
    u8 _5E[0x60 - 0x5E];                     // 0x5E-0x5F
    u8 mScrollBar[0x40];                     // 0x60-0x9F - CScrollBar
    u8 mCursorA0[0x18];                      // 0xA0-0xB7 - CCur18
    u8 mSysWinB8[0x3C];                      // 0xB8-0xF3 - CSysWin
    u8 mSysWinF4[0x3C];                      // 0xF4-0x12F - CSysWin
    u32 layout_130;                          // 0x130
    u32 field_134;                           // 0x134 - ArcResourceAccessor for layout_130
    u32 layout_138;                          // 0x138
    u32 field_13C;                           // 0x13C - ArcResourceAccessor for layout_138
    u32 layout_140;                          // 0x140
    u32 field_144;                           // 0x144 - ArcResourceAccessor for layout_140
    u32 field_148;                           // 0x148 - base row-marker pane (build temp)
    u8 field_14C;                            // 0x14C
    u8 _14D[0x150 - 0x14D];                  // 0x14D-0x14F
    UnkLayoutEntry layouts_150[0x14];        // 0x150-0x1EF
    u8 layout_count_1F0;                     // 0x1F0
    u8 _1F1[0x1F4 - 0x1F1];                  // 0x1F1-0x1F3
    u32 field_1F4;                           // 0x1F4-0x1F7
    u8 field_1F8;                            // 0x1F8 - current-floor id
    u8 _1F9[0x1FC - 0x1F9];                  // 0x1F9-0x1FB
    u32 layout_1FC;                          // 0x1FC
    u8 _200[0x204 - 0x200];                  // 0x200-0x203
    u8 field_204;                            // 0x204
    s8 field_205;                            // 0x205
    s8 field_206;                            // 0x206
    u8 field_207;                            // 0x207
    u8 field_208;                            // 0x208
    u8 _209[0x32CC - 0x209];                 // 0x209-0x32CB
    u8 field_32CC;                           // 0x32CC
    u8 _32CD[0x32D4 - 0x32CD];               // 0x32CD-0x32D3
    u32 layout_32D4;                         // 0x32D4
    u32 layout_32D8;                         // 0x32D8
    u32 layout_32DC;                         // 0x32DC
    u32 layout_32E0;                         // 0x32E0
    u8 field_32E4;                           // 0x32E4
    u8 field_32E5;                           // 0x32E5
    u8 _32E6[0x32EC - 0x32E6];               // 0x32E6-0x32EB
    u32 layout_32EC;                         // 0x32EC
    u32 layout_32F0;                         // 0x32F0
    u32 layout_32F4;                         // 0x32F4
    u32 layout_32F8;                         // 0x32F8
    u8 field_32FC;                           // 0x32FC
    u8 field_32FD;                           // 0x32FD
    u8 _32FE[0x3304 - 0x32FE];               // 0x32FE-0x3303
    u32 layout_3304;                         // 0x3304
    u32 layout_3308;                         // 0x3308
    u32 layout_330C;                         // 0x330C
    u32 layout_3310;                         // 0x3310
    u8 field_3314;                           // 0x3314
    u8 field_3315;                           // 0x3315
    u8 _3316[0x331C - 0x3316];               // 0x3316-0x331B
    u32 layout_331C;                         // 0x331C
    u32 layout_3320;                         // 0x3320
    u32 layout_3324;                         // 0x3324
    u32 layout_3328;                         // 0x3328
    u8 field_332C;                           // 0x332C
    u8 field_332D;                           // 0x332D
    u8 _332E[0x3330 - 0x332E];               // 0x332E-0x332F
    u32 field_3330;                          // 0x3330
    u32 layout_3334;                         // 0x3334
    u32 field_3338;                          // 0x3338 - ArcResourceAccessor for layout_3334
    u8 field_333C;                           // 0x333C
    u8* field_3340;                          // 0x3340
    f32 field_3344;                          // 0x3344
};


// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" char lbl_eu_8050BEA8[];

// Argument block (0x18 bytes, passed by reference) for the embedded layout
// widgets' virtual init entry (vtable+8) called by the cursor ctor.
struct CFloorMapWidgetArg {
    const void* nameOrTable; // +0x00 - colour table or pane-name string
    u32 _04;                 // +0x04 - zeroed
    u32 _08;
    u32 _0C;
    u32 _10;
    u8 field_14;             // +0x14 - zeroed
    u8 field_15;             // +0x15 - always 1
};

// 0x18-byte embedded layout widget reset by the cursor ctor (four of them at
// cursor +0x30D4/+0x30EC/+0x3104/+0x311C). The ctor clears everything except
// the vtable, installs the shared arc resource accessor at +0x08 and calls
// the virtual init entry.
struct CFloorMapWidget18 {
    void* vtbl;      // +0x00 - left untouched (member ctor already ran)
    void* field_04;  // +0x04
    void* accessor;  // +0x08 - ArcResourceAccessor (cursor +0x04)
    void* field_0C;
    void* field_10;
    u8 field_14;
    u8 field_15;
};

// View of the cursor object covering the four embedded widgets.
struct CFloorMapWidgets {
    u8 _00[0x30D4];
    CFloorMapWidget18 widgets[4]; // 0x30D4, 0x30EC, 0x3104, 0x311C
};

// View of the embedded sys-win subobjects (offsets 0xB8/0xF4): their shared
// vtable slot at offset 0x88 (index 34) is invoked by the map-load routine.
class CFloorMapWinVf88 {
public:
    virtual void vf00();
    virtual void vf04();
    virtual void vf08();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual void vf30();
    virtual void vf34();
    virtual void vf38();
    virtual void vf3C();
    virtual void vf40();
    virtual void vf44();
    virtual void vf48();
    virtual void vf4C();
    virtual void vf50();
    virtual void vf54();
    virtual void vf58();
    virtual void vf5C();
    virtual void vf60();
    virtual void vf64();
    virtual void vf68();
    virtual void vf6C();
    virtual void vf70();
    virtual void vf74();
    virtual void vf78();
    virtual void vf7C();
    virtual void vf88();   // vtable offset 0x88
};

// Ctor view of the cursor object: layout/accessor head plus the widgets.
struct CFloorMapCtorView {
    nw4r::lyt::Layout* layout;                // +0x00
    nw4r::lyt::ArcResourceAccessor* accessor; // +0x04
    u8 _08[0x30D4 - 0x08];
    CFloorMapWidget18 widgets[4]; // 0x30D4, 0x30EC, 0x3104, 0x311C
};

// Virtual shape of the embedded layout widget (init entry at vtable+8).
class CFloorMapWidget {
public:
    virtual void v00();
    virtual void v01();
    virtual void Init(CFloorMapWidgetArg* arg);
};

// Colour/name query used by the cursor ctor: returns a pointer to four s16
// colour entries for the named layout group.
s16* func_80139658(nw4r::lyt::Layout* layout, const char* name, int index);
extern const char lbl_eu_805370EC[];
extern const char lbl_eu_805370D4[];
extern const char lbl_eu_805370BC[];
extern const char lbl_eu_80537104[];

// C-linkage sub-object destructor imports used by ~CFloorMap (retail emits the
// unmangled names at the call sites).
extern "C" void __dt__7CSysWinFv(void*, int);
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dt__10CScrollBarFv(void*, int);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
// CCur18 construction (retail unmangled) + shared UI accessor import.
extern "C" void __ct__CCur18(void*, nw4r::lyt::ArcResourceAccessor*);
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4();

// Same-TU helper definitions used by CFloorMap::OnFileEvent (retail flat
// names; the extern "C" declaration fixes the definition linkage).
struct CFloorMapLayoutData0;
struct CFloorMapRowList;
extern "C" void func_80244764(CFloorMapLayoutData0*);
extern "C" void func_80246330(void*);
extern "C" u32 func_8024FB78(void*);
extern "C" void func_8024830C(void*, void*);
extern "C" void func_80247490(void*, u8, u32, f32);
void func_8024808C(CFloorMapFull* self, u32 idx);
extern "C" void func_8024B6F8(CFloorMapRowList*, void*, u32, u32, u32);
extern "C" u32 getAllocHandle__10CLibLayoutFv();

// Per-map layout-name table copied by OnFileEvent branch 2 (28 u32 names,
// .rodata).
extern const u32 lbl_eu_8050BAB0[];

// ============================================================================
// Sub-object layouts (from retail field analysis)
// ============================================================================

// Layout-init wrapper used by func_80244C60/func_80244DD8: a layout slot, its
// arc resource accessor and the two animation transforms bound to it.
struct CFloorMapLayoutData {
    u8 _00[0x04];
    nw4r::lyt::Layout* layout;                // +0x04
    nw4r::lyt::ArcResourceAccessor* accessor; // +0x08
    nw4r::lyt::AnimTransform* anim0C;         // +0x0C
    nw4r::lyt::AnimTransform* anim10;         // +0x10
};

// Layout-init view for func_80244764: layout pointer at +0x00 with the arc
// resource accessor right behind it at +0x04 (func_80136E84 takes
// &layout/accessor pair, then GetRootPane reads layout+0x10).
struct CFloorMapLayoutData0 {
    nw4r::lyt::Layout* layout;                // +0x00
    nw4r::lyt::ArcResourceAccessor* accessor; // +0x04
};

// 0x30C-byte floor-entry block. The row/page count byte sits at +0x30C of
// each block (viewed via CFloorMapFloorEntryView); the blocks start at
// CFloorMapCursor+0x0C, so entry i's count is at cursor + i*0x30C + 0x318.
struct CFloorMapFloorEntry {
    u8 _00[0x30C];
};

// Count-byte view over a CFloorMapFloorEntry block (kept separate so the
// entry array stride stays 0x30C, matching retail's mulli 0x30C).
struct CFloorMapFloorEntryView {
    u8 _00[0x30C];
    u8 count; // +0x30C
};

// Two consecutive 0x30C floor-entry blocks, copied as one 0x618-byte chunk
// from the staging image into the resident cursor.
struct CFloorMapEntryBlocks {
    u8 data[0x618];
};

// 5-byte cursor-state run (flag/floor/page/row/entry0-count) copied byte-wise;
// its last byte aliases the first byte of the entry-blocks chunk.
struct CFloorMapHeadFlags {
    u8 b[5];
};

// One 0x1C-byte widget-init record staged by __ct__80244F50: a default name
// pointer followed by the live widget body (four words + two flag bytes).
struct CFloorMapRecSlot {
    void* name;               // +0x00
    struct {
        u32 words[4];         // +0x04
        u8 flagA;             // +0x14
        u8 flagB;             // +0x15
    } body;
    u8 _pad[2];               // +0x16
};

// Full 0x3138-byte staging image built by __ct__80244F50 on the stack and
// copied member-wise into the resident cursor at CFloorMap+0x1FC.
struct CFloorMapStage {
    void* mData;              // +0x000
    u32 accessor;             // +0x004
    u8 headFlags[4];          // +0x008 (flag / floor / page / row)
    CFloorMapEntryBlocks blocks; // +0x00C
    u8 _624[0x30D0 - 0x624];
    u8 floorCount;               // +0x30D0
    u8 _30D1[0x30D4 - 0x30D1];
    CFloorMapRecSlot slots[4];   // +0x30D4
    u32 scrollBarPtr;            // +0x3134
};

// Floor-map cursor sub-object (lives at CFloorMap+0x1FC): current floor/page/
// row indices plus the position-target holder and scrollbar widget pointers.
struct CFloorMapCursor {
    nw4r::lyt::Layout* mData; // +0x00
    u8 _04[0x08 - 0x04];
    u8 field_08;              // +0x08 - current-floor flag
    s8 field_09;              // +0x09 - floor index
    s8 field_0A;              // +0x0A - page index
    s8 field_0B;              // +0x0B - row index
    u8 _0C[0x30C];            // +0x0C..0x317 - first floor-entry body
    u8 entry0Count;           // +0x318 (first entry's count)
    u8 _319[0x30D0 - 0x319];
    u8 field_30D0;            // +0x30D0 - floor count
    u8 _30D1[0x30D8 - 0x30D1];
    nw4r::lyt::Layout* field_30D8; // +0x30D8 - map layout holder
    u8 _30DC[0x30F0 - 0x30DC];
    nw4r::lyt::Layout* field_30F0; // +0x30F0 - second map layout holder
    u8 _30F4[0x3101 - 0x30F4];
    u8 field_3101;            // +0x3101 - map-id mismatch flag
    u8 _3102[0x3108 - 0x3102];
    void* field_3108;         // +0x3108 - holder for the pane to position
    u8 _310C[0x3134 - 0x310C];
    void* field_3134;         // +0x3134 - scrollbar widget pointer
};

// 0x30C-stride floor-name entry written by func_80245950 (base at
// cursor + fi*0x30C): the floor-id/name fields sit at +0x10..+0x17 (the
// marker-block header at base+0x10) and the row-count byte at +0x318 (the
// marker block's count at +0x308).
struct CFloorMapFloorNameEntry {
    u8 _00[0x10];
    u8 field_10;              // +0x10 - floor id (1-based)
    u8 _11;
    u16 field_12;             // +0x12 - floor-name id (from func_80136330)
    u32 field_14;             // +0x14 - floor-name string (from func_8013639C)
    u8 _18[0x318 - 0x18];
    u8 field_318;             // +0x318 - row/entry count
};

// 0x18-byte map-marker record shared by the local entry table (built by
// func_80245DF8) and the per-floor marker lists the entries are merged into.
struct CFloorMapMarkerEntry {
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
// +0x214); used by the marker flag check in func_80245DF8.
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
// the pointer at +0x04 are handed to func_8024808C by OnFileEvent.
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

// Linked-list node view used by func_80248558: the child panes of the map
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

// View of the layout-slot block passed to func_80248558 (the address of
// CFloorMap::mLayout140): +0x00 is the map Layout*, +0x08 the pane whose
// FindPaneByName resolves the row-marker pane names. +0x0C holds the
// current map-row id (read by func_802497B0's marker merge loop).
struct CFloorMapLayoutSlots {
    nw4r::lyt::Layout* layout; // +0x00
    u32 _04;                   // +0x04
    nw4r::lyt::Pane* pane;     // +0x08
    u8 field_0C;               // +0x0C - current map-row id
};

// .sdata2 conversion magics: 0x4330000080000000 (signed s16->f32, retail
// lbl_eu_80668770) and 0x4330000000000000 (unsigned u8->f32, retail
// lbl_eu_80668788). Defined in CFloorMap.cpp.
extern const double lbl_eu_80668770;
extern const double lbl_eu_80668788;

// u16/u8 -> f32 conversion scratch (CfTFile.cpp / CSysWinScenarioLog.cpp
// convention): build the 0x43300000-prefixed bit pattern in memory and
// subtract the named .sdata2 magic so the pool reloc matches retail instead
// of an MWCC-synthesised @N entry. The two magics are defined in
// CFloorMap.cpp (CMiniMap.cpp idiom).
union CFloorMapF64Conv {
    u32 w[2];
    f64 d;
};

// s16 -> f32 conversions that reference the named retail .sdata2 magics
// (lbl_eu_80668770 = 0x4330000080000000, lbl_eu_80668788 = 0x4330000000000000)
// so the pool relocs match retail instead of MWCC-synthesised @N entries.
static inline f32 CFloorMapS16ToF32(s16 v) {
    CFloorMapF64Conv conv;
    conv.w[0] = 0x43300000;
    conv.w[1] = (u32)(s16)v ^ 0x80000000;
    return (f32)(conv.d - lbl_eu_80668770);
}

static inline f32 CFloorMapU8ToF32(u8 v) {
    CFloorMapF64Conv conv;
    conv.w[0] = 0x43300000;
    conv.w[1] = (u32)v;
    return (f32)(conv.d - lbl_eu_80668788);
}

// .sdata2 floats used by the marker/coordinate maths.
extern f32 lbl_eu_80668764;   // 0.0f
extern f32 lbl_eu_80668778;   // marker y scale factor
extern f32 lbl_eu_8066877C;   // marker x scale factor

// Minimal view of CCur18 (CFloorMap+0xA0) for the virtual call at vtable
// offset 0x10 used by func_8024D614 (cursor position update).
class CCur18View {
public:
    virtual void v00() = 0;      // 0x08
    virtual void v01() = 0;      // 0x0C
    virtual void v02(void*) = 0; // 0x10
};

// Field view of the opaque CCur18 sub-object (CBaseCur layout + padding).
// Used to copy a stack-constructed CCur18 into the embedded mCursorA0 region
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

// Object used by func_8024B6F8 (row-visibility update); only +0x00 is used.
struct CFloorMapRowList {
    nw4r::lyt::Layout* mData; // +0x00
};

// Per-frame widget updates (retail-unmangled C-linkage names; declared in
// COption.hpp / CItemBoxGrid.hpp / CMapSel.hpp under the same extern "C").
extern "C" void func_8003AA8C__5CBdatFUl(u32);     // CBdat::func(u32) - release shared BDAT handle
extern "C" void waitForDrawDone__9CDeviceVIFv();   // CDeviceVIF::waitForDrawDone
extern "C" void func_801F35DC(void* scrollBar);    // CScrollBar teardown
extern "C" void func_8022B7F4(void* sysWin);       // CSysWin teardown
// C++-mangled imports from code_80135FDC.cpp (unmangled identifiers mangle to
// the retail symbols, same scheme as CItemBoxInfo.hpp).
void func_801390E0(CFileHandle**);
void func_80139124(nw4r::lyt::ArcResourceAccessor*);

// Cast-only view of nw4r::lyt::Layout for the deleting-destructor dispatch at
// vtable slot 2 (+0x08 after the RTTI prefix); arg 1 in r4 (see
// CItemBoxLayoutDtorVt).
struct CFloorMapLayoutDtorVt {
    virtual void destroy(int flags);  // slot 2 => +0x08
};

extern "C" void func_801F3540(void* scrollBar);   // CScrollBar per-frame update
extern "C" void func_801D202C(void* cursor);      // CCur18 per-frame update
extern "C" void func_8022B748(void* sysWin);      // CSysWin per-frame update
// Advance the attached anim-transform to a frame (returns nonzero when done);
// retail symbol keeps the C++ mangled name func_80137444__FPQ34nw4r3lyt13AnimTransformf.
u32 func_80137444(nw4r::lyt::AnimTransform*, float);

// Linked list of marker objects returned by func_800B6CF8/func_800B6C58/
// func_800B6BEC (same layout as CMiniMap's MiniMapList): head sentinel at
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

// 3-float position view for the marker objects' GetPos() (vtable+0xAC).
struct CFloorMapVec3 {
    f32 x;                                   // +0x00
    f32 y;                                   // +0x04
    f32 z;                                   // +0x08
};

// Player-object vtable view for func_8024FB78: GetPos() sits at vtable+0xAC
// (41 dummy slots before it) and returns a pointer whose +0x04 is the y.
class CFloorMapPlayerObj {
public:
    virtual void v000(); virtual void v004(); virtual void v008(); virtual void v00C();
    virtual void v010(); virtual void v014(); virtual void v018(); virtual void v01C();
    virtual void v020(); virtual void v024(); virtual void v028(); virtual void v02C();
    virtual void v030(); virtual void v034(); virtual void v038(); virtual void v03C();
    virtual void v040(); virtual void v044(); virtual void v048(); virtual void v04C();
    virtual void v050(); virtual void v054(); virtual void v058(); virtual void v05C();
    virtual void v060(); virtual void v064(); virtual void v068(); virtual void v06C();
    virtual void v070(); virtual void v074(); virtual void v078(); virtual void v07C();
    virtual void v080(); virtual void v084(); virtual void v088(); virtual void v08C();
    virtual void v090(); virtual void v094(); virtual void v098(); virtual void v09C();
    virtual void v0A0();
    virtual CFloorMapVec3* GetPos();         // vtable+0xAC
    virtual float fn0xCC();                  // vtable+0xCC - map zoom factor
};

// Marker object view used by the func_8024A748 list walks (fields read:
// flags at +0x64, name at +0x74, id at +0x8C).
class CFloorMapObj {
public:
    virtual void v000(); virtual void v004(); virtual void v008(); virtual void v00C();
    virtual void v010(); virtual void v014(); virtual void v018(); virtual void v01C();
    virtual void v020(); virtual void v024(); virtual void v028(); virtual void v02C();
    virtual void v030(); virtual void v034(); virtual void v038(); virtual void v03C();
    virtual void v040(); virtual void v044(); virtual void v048(); virtual void v04C();
    virtual void v050(); virtual void v054(); virtual void v058(); virtual void v05C();
    virtual void v060(); virtual void v064(); virtual void v068(); virtual void v06C();
    virtual void v070(); virtual void v074(); virtual void v078(); virtual void v07C();
    virtual void v080(); virtual void v084(); virtual void v088(); virtual void v08C();
    virtual void v090(); virtual void v094(); virtual void v098(); virtual void v09C();
    virtual void v0A0(); virtual void v0A4();
    virtual CFloorMapVec3* GetPos();         // vtable+0xAC
    virtual void v0B0(); virtual void v0B4(); virtual void v0B8(); virtual void v0BC();
    virtual void v0C0(); virtual void v0C4(); virtual void v0C8(); virtual void v0CC();
    virtual void v0D0(); virtual void v0D4(); virtual void v0D8(); virtual void v0DC();
    virtual void v0E0(); virtual void v0E4(); virtual void v0E8(); virtual void v0EC();
    virtual void v0F0(); virtual void v0F4(); virtual void v0F8(); virtual void v0FC();
    virtual void v100(); virtual void v104(); virtual void v108(); virtual void v10C();
    virtual void v110(); virtual void v114(); virtual void v118(); virtual void v11C();
    virtual void v120(); virtual void v124(); virtual void v128(); virtual void v12C();
    virtual void v130(); virtual void v134(); virtual void v138(); virtual void v13C();
    virtual void v140(); virtual void v144(); virtual void v148(); virtual void v14C();
    virtual void v150(); virtual void v154(); virtual void v158();
    virtual void* fn0x160();                 // vtable+0x160 - marker pane lookup
    u8 _0C[0x64 - 0x0C];                     // 0x0C-0x63
    u32 m64;                                 // 0x64 - flags
    u8 _68[0x74 - 0x68];
    u32 m74;                                 // 0x74 - name string
    u8 _78[0x8C - 0x78];
    u16 m8C;                                 // 0x8C - id
    u8 _8E[0x91 - 0x8E];
    u8 m91;                                  // 0x91 - marker kind (6 = landmark)
};

// 0x4C-byte landmark entry walked by func_8024A748 case 2 (func_80193804
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
extern "C" u8 func_8013600C(const char*, const char*, u32);
extern "C" u16 func_8013606C(const void*, const void*, u32);
extern "C" void* func_800B6CF8(int);
extern "C" void* func_800B6C58();
extern "C" void* func_800B6BEC();
extern "C" void* func_80193804();
extern "C" void func_80141DC4(float*, int = 0);

// C-linkage UI helper imports. func_80136E84/func_80136F08 keep their retail
// mangled names through MWCC's natural C++ mangling; the flat-named helpers
// need extern "C" so MWCC does not append a parameter suffix.
void func_80136E84(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
void func_80136F08(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**, nw4r::lyt::ArcResourceAccessor*, char*);
extern "C" void func_8013676C(nw4r::lyt::Pane*, u32);
// C-linkage name/label helpers from code_80135FDC.cpp. func_80136190 builds
// a pane name from a format string, a base string and an index (JP retail
// symbol func_80136190__FPcPcUl = (char*, char*, u32); fixed args so MWCC
// does not emit the varargs CR1 clear); func_80136B4C updates a pane.
extern "C" char* func_80136190(char*, char*, u32);
extern "C" void func_80136B4C(nw4r::lyt::Layout*, char*, char*, u32);
extern "C" void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
extern "C" void func_801375A0(nw4r::math::VEC3*, nw4r::lyt::Pane*);
extern "C" void func_801F3850(void*, u16);
// C linkage so the same-TU definition emits the retail symbol
// func_80246200 instead of the mangled C++ name at every call site.
extern "C" void func_80246200(void*);
extern "C" void func_80137B44(nw4r::lyt::Layout*, const char*, u32);
extern "C" void* func_80137E7C(void*, const char*, void*);
extern "C" u32 func_8009CF8C(u32);

// Flat C-ABI imports used by the map-cursor functions (retail symbols are
// unmangled). func_80138078 keeps C++ linkage so MWCC emits the retail
// mangled name func_80138078__FUl.
extern "C" int CSysWin_getUnk34(void*);
extern "C" int CSysWin_isActive(void*);
extern "C" u32 func_80248558(void*);
extern "C" void func_8024577C(void*, u16);
extern "C" void func_802452C4(void*);
extern "C" void func_8024B4CC(nw4r::math::VEC3*, void*, nw4r::lyt::Pane*);
extern void func_80138078(u32);

// C-ABI imports used by the floor-map update functions (retail plain names).
extern "C" void code80135FDC_setVec3(float*, float, float, float);
extern "C" void copyVEC3(void*, const void*);
extern "C" void func_80137738(nw4r::math::VEC3*, const nw4r::math::VEC3*);
extern "C" char* func_8013639C(const void*, const void*, int);
extern "C" char* func_80138F78(u32);
extern "C" u16 func_80136254(const void*, const void*, int);
extern "C" u8 func_801361E8(u32, const char*, u32);
extern "C" s16 func_80136330(u32, const char*, u32);
extern "C" u32 func_8003B1EC(u32);
extern "C" void* getFP__FPCc(const char*);
extern "C" void* func_8003AA34();
extern "C" void func_801F367C(void*);
extern "C" CFloorMapGimmickGlobal* getUnk80664658();
extern "C" void func_801F3670(void*, const float*);
extern "C" void func_801F36BC(void*, int, int);
extern "C" void func_8022C1B4(nw4r::math::VEC3*, void*, u8);
extern "C" void func_801390E0__FPP11CFileHandle(CFileHandle**);
extern "C" void Panic__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
extern "C" int atoi(const char*);

// CSysWin sub-object helpers used by the floor-map input handlers (retail
// flat names).
extern "C" void func_8022B8E4(void* sysWin);
extern "C" void func_801D216C(void* cursor, int mode);
extern "C" void func_8022B9B4(void* sysWin, char* nameA, char* nameB);
extern "C" void func_8022BF6C(void* sysWin, char* nameA, char* nameB);
extern "C" void func_8022BFC8(void* sysWin, u32 flags);
extern "C" void func_8022B8B8(void* sysWin);

// .sdata2 floats / .sbss globals for the floor-map update functions.
extern f32 lbl_eu_80668760;   // 1.0f (per-frame countdown step / anim advance)
extern f32 lbl_eu_8066879C;
extern f32 lbl_eu_8066A1F8;
extern f32 lbl_eu_80668790;
extern f32 lbl_eu_80668768;
extern f32 lbl_eu_8066876C;
extern f32 lbl_eu_806687BC;
extern f32 lbl_eu_806687A0;
extern f32 lbl_eu_80668794;

// .sbss color tables used by func_80139A18 (init in sinit_80250CB4).
extern u16 lbl_eu_806647A0[4];
extern u16 lbl_eu_806647A8[4];
extern u16 lbl_eu_806647B0[4];
extern u16 lbl_eu_806647B8[4];
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

// C-ABI helpers used by func_80245950 (defined in code_80135FDC.cpp).
extern "C" int func_8013AC3C(u8 max, u8 count, u32 off);
extern "C" void func_80139A18(void*, void*, void*, void*);
extern u32 lbl_eu_8066479C;
extern u32 lbl_eu_806640A8;
extern u8 lbl_eu_80664798;
extern u8 lbl_eu_8052CB40[];
extern u8 lbl_eu_8052CB1C[];

// MEM2 + device-file imports (retail mangled names, declared verbatim).
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
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

// Result of cf::UnkClass_800821F8's vtable+0x1C getter; +0x04 holds the map
// scale numerator used to size the current-position marker.
class CFloorMapMgrSrc {
public:
    u8 _00[4];
    f32 scale04; // +0x04
};

// Virtual view of cf::UnkClass_800821F8 (only the vtable+0x1C entry is used).
class CFloorMapMgrView {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual CFloorMapMgrSrc* getScaleSrc(); // vtable+0x1C
};

// View into the object returned by CDeviceFont::func_80452C10: vtable+0x24
// (slot 9, no args) yields the u32 passed to func_8013676C. All-pure so no
// vtable is emitted.
class CFloorMapFontView {
public:
    virtual void v00() = 0;         // 0x08
    virtual void v01() = 0;         // 0x0C
    virtual void v02() = 0;         // 0x10
    virtual void v03() = 0;         // 0x14
    virtual void v04() = 0;         // 0x18
    virtual void v05() = 0;         // 0x1C
    virtual void v06() = 0;         // 0x20
    virtual u32 getFontHandle() = 0; // 0x24
};
