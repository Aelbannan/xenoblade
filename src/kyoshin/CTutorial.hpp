#pragma once

#include <types.h>

#include "monolib/lib/UnkClass_8045F564.hpp"

// Forward declarations for nw4r layout types used by CTutorial members.
namespace nw4r {
namespace lyt {
class Layout;
class DrawInfo;
class AnimTransform;
class ArcResourceAccessor;
class Pane;
}
}

class CEventFile;
class CFileHandle;

// Full object layout for CTutorial (used by C-linkage accessors)
struct CTutorialFull {
    u8 _00[0x46];
    u8 field_46;
    u8 field_47;
    u8 _48[0x52 - 0x48];
    u8 field_52;
};

// Font-handle object returned by CDeviceFont::func_80452C10. The first member
// is the vtable pointer; vtable slot 9 (offset 0x24) is the font accessor that
// feeds func_8013676C.
struct CTutorialFontVtbl {
    u32 mSlots[10];
};
struct CTutorialFontObj {
    CTutorialFontVtbl* mpVtbl;
};

class CTutorial {
public:
    CTutorial();
    virtual ~CTutorial();
    bool OnFileEvent(CEventFile* pEventFile);

    // Draw the tutorial layout when the active flag is set (tail-calls
    // func_80137038).
    void func_8029ABB8(nw4r::lyt::DrawInfo* drawInfo);
    // Idempotent start: mark state 1 and fire the 0x8 UI event once.
    void func_8029ACC4();
    // Current tutorial phase (0-3) derived from the page counters at 0x50/0x51.
    u8 func_8029AE5C();
    // Advance-animation completion: state 3, visible.
    void func_8029AF30();
    // Rewind-animation completion: state 0, visible.
    void func_8029B010();

    u8 func_8029ACAC();
    u8 func_8029ACB4();
    u8 func_8029ACBC();

    // Load / unload the three tutorial data files (retail func_8029AA34 /
    // func_8029ABD8).
    void func_8029AA34();
    void func_8029ABD8();
    void func_8029B124();
    // Page-navigation tick (retail func_8029AD88).
    void func_8029AD88();
    // Advance-anim start handler (retail func_8029AE9C).
    void func_8029AE9C();
    // Rewind-anim start handler (retail func_8029AF7C).
    void func_8029AF7C();
    // Reset the 0x3340..0x33BE UI flag block (retail func_8029B498).
    void func_8029B498();

    // IWorkEvent-compatible vtable (+0x00), then the widget data:
    UnkClass_8045F564 mRegion0;         // 0x04 - layout build region (0x1800)
    UnkClass_8045F564 mRegion1;         // 0x14 - data region (0x1000)
    CFileHandle* mFileHandle0;          // 0x24 - tutorial layout arc
    CFileHandle* mFileHandle1;          // 0x28 - locale-specific data
    CFileHandle* mFileHandle2;          // 0x2C - game data
    nw4r::lyt::ArcResourceAccessor* mAccessor0; // 0x30
    nw4r::lyt::ArcResourceAccessor* mAccessor1; // 0x34
    nw4r::lyt::Layout* mpLayout;             // 0x38
    nw4r::lyt::AnimTransform* mpAnimTrans0;  // 0x3C
    nw4r::lyt::AnimTransform* mpAnimTrans1;  // 0x40
    u8 field_44;                              // 0x44: active/draw gate
    u8 field_45;                              // 0x45: state
    u8 field_46;                              // 0x46
    u8 field_47;                              // 0x47: visible flag
    u8 field_48;                              // 0x48: current locale id
    u8 _pad49[0x4C - 0x49];                  // 0x49-0x4B
    u8* field_4C;                             // 0x4C: data buffer (deallocated on unload)
    s8 field_50;                              // 0x50: current page counter
    s8 field_51;                              // 0x51: page counter bound
    u8 field_52;                              // 0x52
    u8 field_53;                              // 0x53: region/flags byte
};

// Layout draw helper (retail reloc is the mangled
// func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii).
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
// Fire a per-window sound/effect event by id (retail reloc
// func_80138078__FUl; declared with C++ linkage so MWCC mangles it).
void func_80138078(u32);
// .sdata2 animation-frame bound shared by the tutorial widget animators.
extern const float lbl_eu_80668C08;

// Source string pool (split1 .rodata): arc path, sprintf formats and layout/
// resource tags all live at offsets into this pool.
extern char lbl_eu_80510290[];
// Shared loaded-BDAT pointer used by the tutorial data loader.
extern u32 lbl_eu_80664A30;

// Font/sound helpers (retail reloc func_801355A0__Fv; C++ linkage so MWCC
// mangles the free function).
u32 func_801355A0();

// Unmangled retail callees - C ABI. The retail reloc names for these are the
// literal unmangled identifiers, so they must be declared with C linkage.
extern "C" void* func_8003AA34();
extern "C" void func_8029B05C(CTutorial* pTutorial);
