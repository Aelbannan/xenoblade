// Auto-scaffolded catalog TU for kyoshin/CMapSel
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CMapSel.hpp"
#include <nw4r/lyt.h>

void __ct__CMapSel(){}

/* CMapSel complete-object destructor.  Matches the retail __dt__7CMapSelFv
   byte-exact: null-check, member dtors in reverse-declaration order, then
   conditional delete if flags > 0. */
extern "C" void* __dt__7CMapSelFv(CMapSel* _this, int flags) {
    if (_this != 0) {
        __dt__6CCur18Fv(_this->mCursor, -1);
        __dt__10CScrollBarFv(_this->mScrollBar, -1);
        __dt__17UnkClass_8045F564Fv(_this->mMemRegion, -1);
        if (flags > 0) {
            __dl__FPv(_this);
        }
    }
    return _this;
}

void func_8024343C(){}

void func_802434A0(){}

/* func_80243560 — Draw the map-select UI: layout, scrollbar, and cursor.
   Guards on field_0x30 (widget active) and mState (not uninitialized). */
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

void func_80243560(CMapSel* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->field_0x30 != 0 && self->mState != 0) {
        func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(self->mLayout), drawInfo, 0, 1);
        func_801F35B0(self->mScrollBar, drawInfo);
        func_801D20B0(self->mCursor, drawInfo);
    }
}

void func_802435CC(){}

void func_80243680(){}

// FULL_MATCH: reads the initial-setup flag at +0x33 (set to 1 in ctor, cleared on play/close)

void func_802436CC(){}

void func_8024371C(){}

void func_80243768(){}

void func_80243838(){}

void func_8024391C(){}

void func_802439CC(){}

u8 func_80243A9C(void* self){
    CMapSelFull* s = static_cast<CMapSelFull*>(self);
    s8 x = s->field_8D;
    s8 y = s->field_8C;
    return s->mGridData[x + y];
}

void func_80243ABC(){}

void func_80243B88(){}

/* func_80243BE8 — Poll animation completion on mAnimTransform2, then enable both
   animations, set state to 5 (post-close cleanup), and notify the scrollbar. */
void func_80243BE8(CMapSel* self) {
    /* Check if mAnimTransform2 has reached the target frame. */
    if (func_80137510((void*)self->mAnimTransform2, lbl_eu_8066873C)) {
        nw4r::lyt::Layout* layout = (nw4r::lyt::Layout*)self->mLayout;
        /* Disable mAnimTransform2, enable mAnimTransform1. */
        layout->SetAnimationEnable((nw4r::lyt::AnimTransform*)self->mAnimTransform2, false);
        layout->SetAnimationEnable((nw4r::lyt::AnimTransform*)self->mAnimTransform1, true);
        self->mState = 5;
        func_801F369C(self->mScrollBar);
    }
}

void func_80243C6C(){}

void func_80243CB8(){}

void func_80243CFC(){}

void func_80243E08(){}

void func_80243ED8(){}

void func_80243FC4(){}

void func_80244020(){}

void CMapSel::OnFileEvent() {}
