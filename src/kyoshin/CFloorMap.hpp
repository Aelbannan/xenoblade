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
// The vtable pointer is at offset 0x00 (compiler-generated).
// Fields below start at offset 0x04 (after vtable).
class CFloorMap {
public:
    CFloorMap();
    virtual ~CFloorMap();
    void OnFileEvent() const;

    // Fields from CFloorMapFull (adjusted for vtable ptr at +0x00)
    // CFloorMapFull has _00[0x24] at 0x00 which includes the vtable ptr.
    // Our class has the vtable ptr automatically, so _00 starts at 0x04.
    u8 _00[0x20];                            // 0x04-0x23
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
    u32 scrollbar_60;                        // 0x60
    u8 _64[0xA0 - 0x64];                     // 0x64-0x9F
    u32 cursor_A0;                           // 0xA0
    u8 _A4[0xB8 - 0xA4];                     // 0xA4-0xB7
    u32 syswin_B8;                           // 0xB8
    u8 _BC[0xF4 - 0xBC];                     // 0xBC-0xF3
    u32 syswin_F4;                           // 0xF4
    u8 _F8[0x130 - 0xF8];                    // 0xF8-0x12F
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
