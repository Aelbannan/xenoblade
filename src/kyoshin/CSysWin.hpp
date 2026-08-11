#pragma once

#include <types.h>
#include <nw4r/lyt.h>

#include "monolib/device/CFileHandle.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/CEventFile.hpp"

// Full object layout for CSysWin (used by C-linkage accessors)
struct CSysWinFull {
    u8 _00[0x28];
    u8 field_28;
    u8 _29[0x34 - 0x29];
    u8 field_34;
    u8 _35;
    u8 field_36;
};

// CSysWin vtable (retail .data, split unit). Stored at +0x00 by __ct__CSysWin.
extern u32 lbl_eu_80536510[];

// Shared string pool (split1 .rodata): "/menu/jp/System.arc" and the window
// label strings (func_8022BF6C indexes +0x77 and +0x82).
extern char lbl_eu_8050A478[];

// C-linkage CSysWin helper imports used by CSysWin.cpp. MWCC does not mangle
// global-scope data, so the data symbol needs no extern "C"; func_80138078 is
// declared with C++ linkage so MWCC mangles it to the retail func_80138078__FUl.
extern const f32 lbl_eu_806685F0;  // .sdata2 float (layout animation duration)
void func_80138078(u32);     // UI sound effect (func_80138078__FUl)

// C-linkage member-destructor/ctor imports used by the free-function dtor and
// ctor below (retail emits the unmangled names at the call sites).
extern "C" void __dt__17UnkClass_8045F564Fv(UnkClass_8045F564*, int);
extern "C" UnkClass_8045F564* __ct__17UnkClass_8045F564Fv(UnkClass_8045F564* self);

// CTagProcessor lives in its own TU (kyoshin/CTagProcessor.cpp); its ctor is
// referenced by its literal retail symbol (unmangled __ct__CTagProcessor).
class CTagProcessor;
extern "C" void* __ct__CTagProcessor(void*);

// CLibLayout helpers referenced by their literal retail (pre-mangled) names.
extern "C" nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor__10CLibLayoutFv();

// C-linkage layout helpers (defined in code_80135FDC.cpp / retail-unmangled).
extern "C" void func_80137924(nw4r::math::VEC3*, nw4r::lyt::Pane*,
                               nw4r::lyt::Pane*, nw4r::lyt::Pane*);
extern "C" void func_80124270(void*, u32);
extern "C" void func_80124288(nw4r::lyt::Pane*, float*);

// C-linkage helper imports for the window content setters (func_8022B9B4 /
// func_8022C348). Retail emits the unmangled names at the call sites.
extern "C" void func_80127BC4(float* dst, float* src);  // copy 2 floats (VEC2)
extern "C" void func_80127BF4(CTagProcessor* tagProc, nw4r::lyt::Pane* pane,
                               nw4r::lyt::Pane* context);
extern "C" void func_801D2150(nw4r::lyt::Pane* pane,
                               const nw4r::math::VEC3* trans);
// Retail code80135FDC_setVec3 leaves its first arg (a pointer) in r3, so
// callers reuse it as the returned pointer for func_801D2150 (see
// CScrollBar.cpp).
extern "C" nw4r::math::VEC3* code80135FDC_setVec3(float* out, float x, float y, float z);

// .sdata2 floats for the label-pane placement ladder in func_8022B9B4
// (thresholds + icon positions).
extern const f32 lbl_eu_80668598;
extern const f32 lbl_eu_8066859C;
extern const f32 lbl_eu_806685A0;
extern const f32 lbl_eu_806685A4;
extern const f32 lbl_eu_806685A8;
extern const f32 lbl_eu_806685AC;
extern const f32 lbl_eu_806685B0;
extern const f32 lbl_eu_806685B4;
extern const f32 lbl_eu_806685B8;
extern const f32 lbl_eu_806685BC;
extern const f32 lbl_eu_806685C0;
extern const f32 lbl_eu_806685C4;
extern const f32 lbl_eu_806685C8;
extern const f32 lbl_eu_806685CC;
extern const f32 lbl_eu_806685D0;
extern const f32 lbl_eu_806685D4;
extern const f32 lbl_eu_806685D8;

// .sdata2 globals used by func_8022BFC8: the 5-entry window-label index table
// (4-byte word at 806685DC + 5th byte at 806685E0) and the int->float
// conversion magic double (0x4330000000000000) at 806685E8.
extern u32 lbl_eu_806685DC;
extern u8 lbl_eu_806685E0;
extern f64 lbl_eu_806685E8;

// cf::CfGameManager helper. US retail names it with the no-arg Fv suffix, but
// the call site passes an argument; declare the retail symbol verbatim under
// C linkage so calls bind to it (a C++ declaration would re-mangle the
// __Q22cf13CfGameManagerFv suffix into an Fi overload).
extern "C" int func_80086F9C__Q22cf13CfGameManagerFv(int arg);

class CSysWin;

// func_8022BFC8 (us-8022dec0) - window-kind advance helper, called by
// func_8022C2A4. C-linkage so the call site binds to the retail symbol; the
// definition lives in CSysWin.cpp (not yet matched).
extern "C" void func_8022BFC8(CSysWin* self, u8 kind);

/*
 * CSysWin - system message window widget.
 *
 * Layout-compatible with IWorkEvent (vptr at +0) for CDeviceFile::readFile,
 * but deliberately declares NO virtuals: the retail vtable (lbl_eu_80536510)
 * is stored manually by __ct__CSysWin so MWCC never emits its own
 * __vt__7CSysWin. US retail uses the short C-linkage ctor symbol
 * (__ct__CSysWin), so the ctor is written as an extern "C" free function in
 * CSysWin.cpp that stores the vtable, placement-constructs the embedded
 * UnkClass_8045F564, then zero/one-inits the state fields.
 */
class CSysWin {
public:
    bool OnFileEvent(CEventFile* pEventFile);

    void* mVtbl;                      // +0x00 - lbl_eu_80536510 (set by __ct__CSysWin)
    UnkClass_8045F564 mMemRegion;     // +0x04 - scratch region for layout build
    CFileHandle* mFileHandle;         // +0x14 - loaded System.arc file handle
    CTagProcessor* mTagProcessor;     // +0x18 - window tag processor (allocated in OnFileEvent)
    nw4r::lyt::ArcResourceAccessor* mArcAccessor;  // +0x1C - System.arc resource accessor
    nw4r::lyt::Layout* mLayout;       // +0x20
    nw4r::lyt::AnimTransform* mAnimTrans;  // +0x24
    u8 field_28;                      // +0x28 - ready flag (1 in ctor, 0 after load)
    u8 _29[3];                        // +0x29
    u32 field_2C;                     // +0x2C
    u32 field_30;                     // +0x30
    u8 field_34;                      // +0x34 - open flag
    u8 field_35;                      // +0x35 - anim phase (0 idle, 1 open, 2 advance, 3 rewind)
    u8 field_36;                      // +0x36 - drawn flag
    u8 field_37;                      // +0x37 - ctor arg (window kind selector)
    u8 field_38;                      // +0x38 - window kind (ctor init 5)
    u8 field_39;                      // +0x39
};

// func_8022C348 (us-8022e240) - layout/kind setup after a file load; called by
// OnFileEvent. Defined as a stub in CSysWin.cpp (not yet matched).
extern "C" void func_8022C348(CSysWin* self);

// func_8022B90C (us-8022d744) - switch the visible pane for a window kind.
// Defined in CSysWin.cpp; declared here so func_8022C348 can call it.
extern "C" void func_8022B90C(CSysWin* self, int kind);
