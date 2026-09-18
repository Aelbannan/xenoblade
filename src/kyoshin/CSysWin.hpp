#pragma once

#include <types.h>
#include <nw4r/lyt.h>

#include "monolib/work/IWorkEvent.hpp"
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
// label strings (sysWinSetTwoTextValues indexes +0x77 and +0x82).
extern char lbl_eu_8050A478[];

// C-linkage CSysWin helper imports used by CSysWin.cpp. MWCC does not mangle
// global-scope data, so the data symbol needs no extern "C"; playUISound is
// declared with C++ linkage so MWCC mangles it to the retail playUISound__FUl.
extern const f32 lbl_eu_806685F0;  // .sdata2 float (layout animation duration)
void playUISound(u32);     // UI sound effect (playUISound__FUl)

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
extern "C" void setPaneVisible(void*, u32);
void func_801390E0(CFileHandle**);
extern "C" void writePanePos(nw4r::lyt::Pane*, float*);

// C-linkage helper imports for the window content setters (func_8022B9B4 /
// func_8022C348). Retail emits the unmangled names at the call sites.
// TagProcCalcPageLayout is declared by CTagProcessor.hpp with its retail signature
// (void*, CTagMsgView*, CTagOutView*) - cast panes at the call sites.
extern "C" void TagCopyVec2f(float* dst, float* src);  // copy 2 floats (VEC2)
extern "C" void Cur_SetPaneTranslate(nw4r::lyt::Pane* pane,
                               const nw4r::math::VEC3* trans);
// Retail code80135FDC_setVec3 leaves its first arg (a pointer) in r3, so
// callers reuse it as the returned pointer for Cur_SetPaneTranslate (see
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
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int arg);

class CSysWin;

// func_8022BFC8 (us-8022dec0) - window-kind advance helper, called by
// sysWinSyncKindAdvance. C-linkage so the call site binds to the retail symbol; the
// definition lives in CSysWin.cpp (not yet matched).
extern "C" void func_8022BFC8(CSysWin* self, u8 kind);

/*
 * CSysWin - system message window widget.
 *
 * Retail polymorphic class (vtable lbl_eu_80536510): IWorkEvent handlers at
 * +0x08..+0x84 (dtor override, WorkEvent1..31 with OnFileEvent overridden at
 * +0x10) plus its own loadSystemArc slot at +0x88 (sysWinInitFileRead).
 * __declspec(novtable): the table lives in the data blob, so no TU emits
 * __vt__7CSysWin; __ct__CSysWin stores the label manually (same idiom as
 * cf::CHelp / CBaseCur). The retail method symbols keep their short
 * C-linkage names; the matching extern "C" definitions stay in CSysWin.cpp
 * and only the dispatch goes through these virtuals.
 */
class __declspec(novtable) CSysWin : public IWorkEvent {
public:
    virtual ~CSysWin();            // +0x08 (def: __dt__7CSysWinFv)
    virtual bool OnFileEvent(CEventFile* pEventFile); // +0x10 override
    virtual void loadSystemArc();  // +0x88 (def: sysWinInitFileRead)

    // Overlay on the implicit vptr at +0x00 so the free-function ctor can
    // store the retail table label.
    void*& vtbl() { return *reinterpret_cast<void**>(this); }
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

// sysWinSwitchKindPane (us-8022d744) - switch the visible pane for a window kind.
// Defined in CSysWin.cpp; declared here so func_8022C348 can call it.
extern "C" void sysWinSwitchKindPane(CSysWin* self, int kind);
