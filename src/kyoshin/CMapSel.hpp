#pragma once

#include <types.h>

// Forward decls for nw4r layout types used in the layout below.
namespace nw4r { namespace math { struct VEC3; } }
namespace nw4r { namespace lyt { class Layout; class AnimTransform; class DrawInfo; class Pane; class ArcResourceAccessor; } }
struct CFileHandle;
class CEventFile;

/* Map selection UI widget. Displays a grid of available maps for the player
   to choose from. Uses IWorkEvent-compatible vtable at +0x00 (set by the
   constructor). Sub-objects are opaque byte arrays; their constructors and
   destructors are called via extern "C" retail symbols.

   Layout (0xB4 bytes):
     +0x00: IWorkEvent vtable pointer (set to lbl_eu_80536E10)
     +0x04: UnkClass_8045F564 (0x10 bytes)
     +0x14-+0x33: various fields
     +0x34: CScrollBar (0x40 bytes)
     +0x74: CCur18 (0x18 bytes)
     +0x8C: selection Y coordinate (s8)
     +0x8D: selection X coordinate (s8)
     +0x8E: grid data (0x26 bytes)

   State machine (mState at +0x31):
     0 = uninitialized/hidden
     1 = loading map data (active)
     2 = fully loaded and visible
     3 = transitioning out
     4 = hidden/closed
     5 = post-close cleanup */
class CMapSel {
public:
    CMapSel();
    ~CMapSel();  // defined in CMapSel.cpp as extern "C" __dt__7CMapSelFv
    u8 mVtbl[4];                              // +0x00 - IWorkEvent vtable ptr
    u8 mMemRegion[0x10];                      // +0x04 - UnkClass_8045F564
    CFileHandle* mFileHandle;                 // +0x14 - load-in-progress file handle
    CFileHandle* mFileHandle2;                // +0x18 - second (map-data) file handle
    nw4r::lyt::ArcResourceAccessor* mArcAccessor; // +0x1C
    void* mAllocatedMem;                      // +0x20 - loaded map-data buffer
    nw4r::lyt::Layout* mLayout;               // +0x24
    nw4r::lyt::AnimTransform* mAnimTransform1; // +0x28
    nw4r::lyt::AnimTransform* mAnimTransform2; // +0x2C
    u8 field_0x30;                            // +0x30
    u8 mState;                                // +0x31
    u8 field_0x32;                            // +0x32
    u8 mFlag33;                               // +0x33
    u8 func_802436C4() { return mFlag33; }
    bool OnFileEvent(CEventFile* event);
    u8 mScrollBar[0x40];                      // +0x34 - CScrollBar
    u8 mCursor[0x18];                         // +0x74 - CCur18
    s8 mSelY;                                 // +0x8C - selection Y coordinate
    s8 mSelX;                                 // +0x8D - selection X coordinate
    u8 mGridData[0x22];                       // +0x8E - grid lookup data
    float mTimer;                             // +0xB0 - transition countdown
};

// Extended layout for free-function accessors
struct CMapSelFull {
    u8 _00[0x8C];
    s8 field_8C;  // y coordinate
    s8 field_8D;  // x coordinate
    u8 mGridData[128]; // grid lookup data starting at +0x8E
};

// Abstract view into the embedded CCur18 cursor vtable. MWCC inserts an
// offset-to-top + RTTI prefix (2 entries), so vtable offset = (index + 2) * 4;
// index 2 -> +0x10 is the "Move" virtual that repositions the cursor (takes a
// VEC3*). All-pure so MWCC emits no vtable for the view itself.
class CCur18View {
public:
    virtual void vf02() = 0;                        // index 0 -> +0x08
    virtual void vf03(const u8*) = 0;               // index 1 -> +0x0C
    virtual void vf04(const nw4r::math::VEC3*) = 0; // index 2 -> +0x10 - Move
};

// Field view of the opaque CCur18 sub-object (CBaseCur layout + padding).
// Used to copy a stack-constructed CCur18 into the embedded mCursor region
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

// View into the object returned by CDeviceFont::func_80452C10: vtable+0x24
// (index 7, no args) yields the u32 passed to func_8013676C. All-pure so no
// vtable is emitted.
class CDeviceFontView {
public:
    virtual void vf0() = 0; // index 0 -> +0x08
    virtual void vf1() = 0; // index 1 -> +0x0C
    virtual void vf2() = 0; // index 2 -> +0x10
    virtual void vf3() = 0; // index 3 -> +0x14
    virtual void vf4() = 0; // index 4 -> +0x18
    virtual void vf5() = 0; // index 5 -> +0x1C
    virtual void vf6() = 0; // index 6 -> +0x20
    virtual u32 vf7() = 0;  // index 7 -> +0x24
};

// Opaque object whose first user virtual (vtable offset 0x08 after the RTTI
// prefix slots) acts like a deleting destructor taking an int flag. Abstract
// (all-pure) so MWCC emits no vtable for the view.
class CMapSelLayoutObj {
public:
    virtual void vfRelease(int) = 0;
};

// View of the embedded CCur18 cursor vtable: slot 3 (offset 0x0C), no args.
// Abstract so no vtable is emitted.
class CMapSelCurObj {
public:
    virtual void vf0(int) = 0;   // slot 2 (0x08)
    virtual void vfUpdate() = 0; // slot 3 (0x0C)
};

// Scrollbar destroy / layout-read helpers (unmangled retail symbols).
extern "C" void func_801F35DC(void* scrollbar);
extern "C" void func_801F34F4(void* scrollbar);

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dt__10CScrollBarFv(void*, int);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __dl__FPv(void*);
extern "C" void __ct__CScrollBar(void*, int);
extern "C" u8 func_801361E8(u32, const char*, u32);
extern "C" u32 func_8003B1EC(void*);
extern "C" u32 func_8009CF8C(u32);
extern "C" void func_801F35B0(void*, nw4r::lyt::DrawInfo*);
extern "C" void func_801D20B0(void*, void*);
extern "C" void func_801F369C(void*);
extern "C" void func_801F3540(u8*);                    // scrollbar per-frame update
// Cursor / scrollbar / layout C-ABI helpers (unmangled retail symbols).
extern "C" void func_801D202C(u8*);                    // cursor per-frame update
extern "C" void func_801F3670(u8*, const float*);      // scrollbar init (3-float vec)
extern "C" void func_801F36BC(u8*, int, int);          // scrollbar range setup
extern "C" void func_801F3850(u8*, u16);                // scrollbar thumb position
extern "C" void func_801F367C(u8*);                    // scrollbar show
// Layout/text C-ABI helpers normally declared by code_80135FDC.hpp. CMapSel.cpp
// defines CODE_80135FDC_CPP before including that header, so those exports are
// skipped there and the declarations below apply (retail CMapSel callers pass
// the texture resource as func_80137E7C's third argument). Same guard as the legacy
// func_80137510 pseudo-import so code_802405F4.cpp's define stays a no-op.
#ifndef KYOSHIN_SKIP_CMAPSEL_LEGACY_LAYOUT_IMPORTS
extern "C" char* func_8013639C(const void*, const void*, int);
extern "C" void func_8013676C(void*, u32);
extern "C" void func_80137E7C(void*, const char*, const void*);
extern "C" char* func_80138F78(u32);
#endif
// Cursor position helper: fill output with the point derived from two panes.
extern "C" void func_80137924(nw4r::math::VEC3*, nw4r::lyt::Pane*,
                               nw4r::lyt::Pane*, nw4r::lyt::Pane*);
extern "C" void func_80138078__FUl(u32);               // UI sound effect
// Grid-text helpers / cursor construction (C-ABI retail symbols).
extern "C" u16 func_80136254(const void*, const void*, int);
extern "C" void func_80124270(void*, u32);
extern "C" void __ct__CCur18(void*, nw4r::lyt::ArcResourceAccessor*);
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4();
extern "C" int sprintf(char*, const char*, ...);
// Map-select string pool (split1 .rodata): pane-name format at +0x1F, fixed
// pane name at +0x88.
extern "C" char lbl_eu_8050B4A8[];
// BDAT map-table pointers (.sbss, sda21-relative).
extern void* lbl_eu_806640A0;
extern void* lbl_eu_806640A8;
// Current map id (.sbss) - initial grid selection target.
extern u32 lbl_eu_80664184;
// IWorkEvent vtable for CMapSel (.data) - set by the constructor. Declared as
// an array so MWCC uses full 32-bit (lis/addi) addressing like the retail
// (a scalar extern would fall back to sda21 and produce a different reloc).
extern "C" void* lbl_eu_80536E10[];
extern "C" float lbl_eu_80668738;
extern "C" float lbl_eu_80668740;
extern "C" float lbl_eu_80668744;
// Canonical func_80137510 (nw4r::lyt::AnimTransform*, float) is declared in
// code_80135FDC.hpp. CMapSel.cpp includes that header and passes proper
// AnimTransform* pointers; the guard below stays for TUs that include both
// headers (identical redeclarations are harmless, but keep the old macro
// contract so code_802405F4.cpp's define stays a no-op).
#ifndef KYOSHIN_SKIP_CMAPSEL_LEGACY_LAYOUT_IMPORTS
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);
#endif
extern "C" const float lbl_eu_8066873C;
