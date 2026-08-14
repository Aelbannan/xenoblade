#pragma once

#include <types.h>
#include <nw4r/lyt.h>

// Forward declarations for types used in CFloorMap
class CScrollBar;
class CBaseCur;
class CSysWin;
class CFileHandle;

// 8-byte entry in the layout pointer array at 0x150
struct UnkLayoutEntry {
    u32 mPtr;   // +0x00: layout pointer
    u32 mUnk;   // +0x04: unknown
};

// Full object layout for CFloorMap (used by C-linkage accessors)
struct CFloorMapFull {
    u8 _00[0x24];            // 0x00-0x23
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
    u8 _43[0x58 - 0x43];     // 0x43-0x57
    u8 field_58;             // 0x58 - cursor active flag
    u8 _59[0x5C - 0x59];     // 0x59-0x5B
    u8 field_5C;             // 0x5C - cursor index (s8)
    u8 _5D[0x60 - 0x5D];     // 0x5D-0x5F
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
    u8 _1F1[0x1FC - 0x1F1];  // 0x1F1-0x1FB
    u32 mLayout1FC;          // 0x1FC - layout pointer (IWorkEvent)
    u8 _200[0x204 - 0x200];  // 0x200-0x203
    u8 field_204;            // 0x204
    u8 field_205;            // 0x205 (s8)
    u8 field_206;            // 0x206 (s8)
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
    u8 _332E[0x3334 - 0x332E]; // 0x332E-0x3333
    u32 mLayout3334;         // 0x3334 - layout pointer
    u8 _3338[0x333C - 0x3338]; // 0x3338-0x333B
    u8 field_333C;           // 0x333C
    u32 field_3340;          // 0x3340 - allocated buffer pointer
    u32 field_3344;          // 0x3344
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
    void OnFileEvent() const;

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
    u8 _43[0x58 - 0x43];                     // 0x43-0x57
    u8 cursor_active_58;                     // 0x58
    u8 _59[0x5C - 0x59];                     // 0x59-0x5B
    s8 cursor_idx_5C;                        // 0x5C
    u8 _5D[0x60 - 0x5D];                     // 0x5D-0x5F
    u8 mScrollBar[0x40];                     // 0x60-0x9F - CScrollBar
    u8 mCursorA0[0x18];                      // 0xA0-0xB7 - CCur18
    u8 mSysWinB8[0x3C];                      // 0xB8-0xF3 - CSysWin
    u8 mSysWinF4[0x3C];                      // 0xF4-0x12F - CSysWin
    u32 layout_130;                          // 0x130
    u8 _134[0x138 - 0x134];                  // 0x134-0x137
    u32 layout_138;                          // 0x138
    u8 _13C[0x140 - 0x13C];                  // 0x13C-0x13F
    u32 layout_140;                          // 0x140
    u8 _144[0x150 - 0x144];                  // 0x144-0x14F
    UnkLayoutEntry layouts_150[0x14];        // 0x150-0x1EF
    u8 layout_count_1F0;                     // 0x1F0
    u8 _1F1[0x1FC - 0x1F1];                  // 0x1F1-0x1FB
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
    u8 _332E[0x3334 - 0x332E];               // 0x332E-0x3333
    u32 layout_3334;                         // 0x3334
    u8 _3338[0x333C - 0x3338];               // 0x3338-0x333B
    u8 field_333C;                           // 0x333C
    u32 field_3340;                          // 0x3340
    u32 field_3344;                          // 0x3344
};


// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" char lbl_eu_8050BEA8[];

// C-linkage sub-object destructor imports used by ~CFloorMap (retail emits the
// unmangled names at the call sites).
extern "C" void __dt__7CSysWinFv(void*, int);
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dt__10CScrollBarFv(void*, int);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);

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

// 0x30C-byte floor-entry block; the row/page count byte sits at +0x30C of
// each block. The blocks start at CFloorMapCursor+0x0C, so entry i's count is
// at cursor + i*0x30C + 0x318.
struct CFloorMapFloorEntry {
    u8 _00[0x30C];
    u8 count; // +0x30C
};

// Floor-map cursor sub-object (lives at CFloorMap+0x1FC): current floor/page/
// row indices plus the position-target holder and scrollbar widget pointers.
struct CFloorMapCursor {
    nw4r::lyt::Layout* mData; // +0x00
    u8 _04[0x09 - 0x04];
    s8 field_09;              // +0x09 - floor index
    u8 field_0A;              // +0x0A - page index
    u8 field_0B;              // +0x0B - row index
    u8 _0C[0x30C];            // +0x0C..0x317 - first floor-entry body
    u8 entry0Count;           // +0x318 (first entry's count)
    u8 _319[0x3108 - 0x319];
    void* field_3108;         // +0x3108 - holder for the pane to position
    u8 _310C[0x3134 - 0x310C];
    void* field_3134;         // +0x3134 - scrollbar widget pointer
};

// Holder reached from CFloorMapCursor::field_3108; the pane to move is at +0x10.
struct CFloorMapCursorTarget {
    u8 _00[0x10];
    nw4r::lyt::Pane* pane; // +0x10
};

// Object used by func_8024B6F8 (row-visibility update); only +0x00 is used.
struct CFloorMapRowList {
    nw4r::lyt::Layout* mData; // +0x00
};

// C-linkage UI helper imports (retail flat names - plain global declarations
// so MWCC emits them unmangled).
void func_80136E84(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
void func_80136F08(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**, nw4r::lyt::ArcResourceAccessor*, char*);
void func_8013676C(nw4r::lyt::Pane*, u32);
void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
void func_801375A0(nw4r::math::VEC3*, nw4r::lyt::Pane*);
void func_801F3850(void*, u16);
void func_80246200(void*);
void* func_80137E7C(void*, const char*, void*);
u32 func_8009CF8C(u32);
void func_80138078(u32);
