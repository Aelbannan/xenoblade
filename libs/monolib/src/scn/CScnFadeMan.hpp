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

class CScnFadeMan : public IScnRender {
public:
    CScnFadeMan(CScn* scene);
    virtual ~CScnFadeMan();
    void update();
    virtual void cbRenderBefore();

    CScn* mScene;             //0x4
    ml::CCol4 mCurrentColor;  //0x8
    ml::CCol4 mStartColor;    //0x18
    ml::CCol4 mDestColor;     //0x28
    s32 mCurrentFrame;        //0x38
    s32 mFrameCount;          //0x3c
};

// Shared sdata2 constant-pool floats referenced by name so the relocs resolve to
// the retail labels (lbl_eu_8066AB7C = combined with the 8.8 fixed-point fraction
// byte in update(); lbl_eu_8066AB80 = 1.0f) instead of a local pool entry.
// Declared at global scope (plain extern, NOT extern "C": MWCC emits the same
// symbol without mangling these names).
extern float lbl_eu_8066AB7C; // 8.8 fixed -> float combine scale

extern float lbl_eu_8066AB80; // 1.0f