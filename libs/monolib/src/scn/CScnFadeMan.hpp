#pragma once

#include <types.h>
#include "monolib/scn/IScnRender.hpp"
#include "monolib/math/CCol4.hpp"
#include "monolib/core/CViewFrame.hpp" // ml::CRect / ml::CRect16 + forward-declared CView

// Minimal CScn surface used by CScnFadeMan (inherits CTTask + IWorkEvent).
// Not the full include/monolib/scn/CScn.hpp (that header re-declares the
// func_8049C7B0/C72C/C794 primitives of THIS TU, which would conflict).
class CScn {
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
};

// Static CView helpers needed by cbRenderBefore. getCurrentView is declared in
// monotlib/include/monolib/core/CView.hpp; func_8043EA88 is not, so both are
// surfaced here as a minimal definition (this TU never fully includes CView.hpp).
class CView {
public:
    static CView* getCurrentView();
    static void func_8043EA88(ml::CRect& rect, CView* view);
};

// Non-virtual reconstruction of CScnFadeMan. Retail CScnFadeMan inherits
// IScnRender (vptr at 0x0), but modeling that inheritance makes MWCC emit its
// own __vt__11CScnFadeMan / RTTI symbols and reference __vt__11CScnFadeMan in
// the ctor/dtor vptr stores, whereas the retail relocs name the .data blob
// lbl_eu_8056EB50. The vtable slot is therefore stored manually from the
// retail blob (same trick as CDeviceFontLoader) so the ctor/dtor relocs carry
// the retail name and no extra vtable/RTTI data is emitted.
class CScnFadeMan {
public:
    CScnFadeMan(CScn* scene);
    ~CScnFadeMan();
    void update();

    u8* m_vtable;             //0x0 retail vtable pointer (lbl_eu_8056EB50)
    CScn* mScene;             //0x4
    ml::CCol4 mCurrentColor;  //0x8
    ml::CCol4 mStartColor;    //0x18
    ml::CCol4 mDestColor;     //0x28
    s32 mCurrentFrame;        //0x38
    s32 mFrameCount;          //0x3c
};

// Shared sdata2 constant-pool floats referenced by name so the relocs resolve to
// the retail labels instead of a local pool entry. Declared at global scope with
// plain extern (no C linkage block): MWCC emits the same symbol without
// mangling these names.
extern float lbl_eu_8066AB7C; // 8.8 fixed -> float combine scale
extern float lbl_eu_8066AB80; // 1.0f
extern float lbl_eu_8066AB78; // 0.0f
extern float lbl_eu_8066AB90; // 1.2f

// Retail CScnFadeMan vtable blob (.data:0x8056EB50, 16B) owned by this TU:
// [RTTI locator lbl_eu_80663A38, 0, dtor, cbRenderBefore].
extern "C" u32 lbl_eu_8056EB50[4];
