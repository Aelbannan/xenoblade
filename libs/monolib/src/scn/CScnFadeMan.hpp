#pragma once

#include <types.h>
#include "monolib/scn/IScnRender.hpp"
#include "monolib/math/CCol4.hpp"
#include "monolib/core/CViewFrame.hpp" // ml::CRect / ml::CRect16 + forward-declared CView

#include "monolib/scn/CScn.hpp"

// Static CView helpers needed by cbRenderBefore. getCurrentView is declared in
// monotlib/include/monolib/core/CView.hpp; fitRectToView is not, so both are
// surfaced here as a minimal definition (this TU never fully includes CView.hpp).
class CView {
public:
    static CView* getCurrentView();
    static void fitRectToView(ml::CRect& rect, CView* view);
};

// Scene fade manager. Retail vtable (0x8056EB50): [RTTI, 0, dtor,
// cbRenderBefore] - a plain polymorphic class with those two virtuals.
class CScnFadeMan {
public:
    CScnFadeMan(CScn* scene);
    virtual ~CScnFadeMan();
    virtual void cbRenderBefore();
    void update();

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

