#pragma once

#include <types.h>
#include <nw4r/lyt.h>

#include "kyoshin/CSortMenu.hpp"  // CScrollBarData (0x40-byte scrollbar), CSortMenu, CScrollBar_isVisible

/* Quest log list widget. Owned by CMenuQuestLog.
   Sorting: mSortEnabled (0x179) gates sort/filter logic,
   mSortDescending (0x17A) flips the sort direction arrow. */
struct CQstLogList {
    u8 func_80227CCC();
    u8 func_80227CD4();

    u8 pad_0000[0x1C];                  // 0x00..0x1B
    nw4r::lyt::Layout* mpLayout;        // 0x1C - layout (func_80136A1C target)
    nw4r::lyt::AnimTransform* mpAnim0;  // 0x20 - animation transform 0
    nw4r::lyt::AnimTransform* mpAnim1;  // 0x24 - animation transform 1
    u8 pad_0028[0x40 - 0x28];           // 0x28..0x3F (cursor sub-object; func_801D216C target)
    CScrollBarData mScrollBar;          // 0x40..0x7F - quest list scroll bar
    CSortMenu mSortMenu;                // 0x80..0x16F - sort menu (func_801D32DC target)
    u8 pad_0170[0x174 - 0x170];         // 0x170..0x173
    u32 field_0x174;                    // 0x174 - mode (0/2/5 set by anim checks)
    u8 field_0x178;                     // 0x178
    u8 mSortEnabled;                    // 0x179 - enables sorting/filtering (init=1)
    u8 mSortDescending;                 // 0x17A - sort direction, 0=asc, 1=desc (init=0)
    s8 field_0x17B;                     // 0x17B - index byte (signed)
    s8 field_0x17C;                     // 0x17C - index byte (signed)
};

/* 0x22-byte quest-log entry record, copied wholesale by func_80227994.
   Layout matches the retail copy: 2 head bytes + 4x8-byte word loop. */
struct CQstLogListEntry {
    u8 mField0;          // 0x0
    u8 mField1;          // 0x1
    u8 mData[0x20];      // 0x2..0x21
};

// In-TU helper functions. Retail symbols are unmangled (C linkage), so the
// declarations must be extern "C" to bind the bl reloc to the retail name.
extern "C" void func_802284E4(CQstLogList* self);
extern "C" void func_80228544(CQstLogList* self);
extern "C" void func_80228B10(CQstLogList* self);

// Imports with unmangled retail names (defined in CSortMenu.cpp / cursor TUs).
extern "C" u8 func_801D32DC(CSortMenu* _this);
extern "C" void func_801D216C(void*, u8);

// Shared data pools (retail names; global scope - no C++ mangling).
extern char lbl_eu_80509AB4[];  // string pool (split1 .rodata)
extern char lbl_eu_80576670[];   // 0x22-byte quest text entry table
// const routes into readonly sdata2 - MWCC hoists the lfs above the frame
// stores (MWCC_REFERENCE:8787); plain extern float schedules it late.
extern const float lbl_eu_80668584;  // anim frame target (sda21)
