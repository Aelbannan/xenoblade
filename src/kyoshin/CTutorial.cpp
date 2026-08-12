// Auto-scaffolded catalog TU for kyoshin/CTutorial
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/CTutorial.hpp"

#include "kyoshin/code_80135FDC.hpp"
u8 CTutorial::func_8029ACAC() { return this->field_46; }
u8 CTutorial::func_8029ACB4() { return this->field_47; }
u8 CTutorial::func_8029ACBC() { return this->field_52; }

/* Draw the tutorial layout when the active gate (0x44) is set. r4 (the draw
 * info) is passed straight through to the layout draw helper. */
void CTutorial::func_8029ABB8(nw4r::lyt::DrawInfo* drawInfo) {
    if (field_44 != 0) {
        func_80137038(mpLayout, drawInfo, 0, 1);
    }
}

/* Idempotent start: if not already running, mark state 1 and fire the 0x8
 * UI sound/effect event. */
void CTutorial::func_8029ACC4() {
    if (field_45 == 0) {
        field_45 = 1;
        field_47 = 0;
        func_80138078(8);
    }
}

u8 CTutorial::func_8029AE5C() {
    s8 last = field_51;
    s8 cur = field_50;
    if (cur == last - 1) {
        return (u8)((last == 1) ? 2 : 3);
    }
    return cur != 0;
}

void func_8029AE9C(){}

/* Advance-animation (0x40) reached the end frame: state 3, visible. */
__declspec(noinline) void CTutorial::func_8029AF30() {
    if (func_80137444(mpAnimTrans1, lbl_eu_80668C08) != 0) {
        field_45 = 3;
        field_47 = 1;
    }
}

void func_8029AF7C(){}

/* Rewind-animation (0x3C) reached the start frame: state 0, visible. */
__declspec(noinline) void CTutorial::func_8029B010() {
    if (func_80137510(mpAnimTrans0, lbl_eu_80668C08) != 0) {
        field_45 = 0;
        field_47 = 1;
    }
}

void func_8029B05C(){}

extern "C" __declspec(noinline) void func_8029B124(CTutorial* self) {}

// Page-counter tick: play the confirm sound while the counter is nonzero,
// decrement it (u8-domain), clamp negatives to zero, then refresh.
extern "C" void func_8029ADF8(CTutorial* self) {
    if (self->field_50 != 0)
        func_80138078(8);
    u8 next = (u8)self->field_50 - 1;
    self->field_50 = (s8)next;
    if ((s8)next < 0)
        self->field_50 = 0;
    func_8029B124(self);
}

bool CTutorial::OnFileEvent(CEventFile* pEventFile) { return false; }

void func_8029B498(){}

extern "C" void func_8029AA34() {}
extern "C" void func_8029AB28(CTutorial* self) {
    if (self->field_44 == 0) {
        return;
    }
    if (self->field_45 == 1) {
        func_8029AE9C();
    } else if (self->field_45 == 2) {
        self->func_8029AF30();
    } else if (self->field_45 == 4) {
        func_8029AF7C();
    } else if (self->field_45 == 5) {
        self->func_8029B010();
    }
    self->mpLayout->Animate(0);
}
extern "C" void func_8029ABD8() {}
extern "C" void func_8029ACEC() {}
// Page-navigation helper: set the page-complete flag when the target is the
// last page, otherwise advance the counter (clamped by a), then refresh and
// play the confirm sound.
// Page-navigation helper: mark complete when b is the last page, else advance
// the counter clamped by a, then refresh and play the confirm sound. NOTE:
// retail emits two dead field loads (lbz 0x50/0x51) before the (s8) param
// casts reuse those registers; MWCC DCEs them from high-level C, leaving the
// function 2 instructions short (0x68 vs 0x70) — open item.
extern "C" void func_8029AD88(CTutorial* self) {
    // Page-navigation on the CURRENT field bytes: field_51's s8 view is the
    // "last page", field_50 the cursor. When the cursor reaches last-1 the
    // page is marked complete; otherwise advance and clamp.
    u8 raw50 = self->field_50;
    u8 raw51 = self->field_51;
    if ((s8)raw50 == (s8)raw51 - 1) {
        self->field_52 = 1;
        return;
    }
    self->field_50 = (u8)(raw50 + 1);
    if ((s8)(u8)(raw50 + 1) >= (s8)raw51) {
        self->field_50 = (u8)(raw51 - 1);
    }
    func_8029B124(self);
    func_80138078(8);
}

