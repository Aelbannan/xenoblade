// Auto-scaffolded catalog TU for kyoshin/CTutorial
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/CTutorial.hpp"

#include "kyoshin/code_80135FDC.hpp"

#include <functions.hpp>
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
        if (last == 1) {
            return 2;
        }
        return 3;
    }
    return cur != 0;
}

// Advance-anim start: disable anim0 / enable anim1, mark state 2, and re-bind
// both transforms onto the layout before kicking an animation tick.
void CTutorial::func_8029AE9C() {
    if (func_80137444(mpAnimTrans0, lbl_eu_80668C08) != 0) {
        field_45 = 2;
        mpLayout->SetAnimationEnable(mpAnimTrans0, false);
        mpLayout->SetAnimationEnable(mpAnimTrans1, true);
        mpLayout->Animate(0);
    }
}

// Compiler-generated complete-object dtor: destroy the two memory regions in
// reverse declaration order, then free the object when the delete flag is set.
CTutorial::~CTutorial() {}

/* Advance-animation (0x40) reached the end frame: state 3, visible. */
__declspec(noinline) void CTutorial::func_8029AF30() {
    if (func_80137444(mpAnimTrans1, lbl_eu_80668C08) != 0) {
        field_45 = 3;
        field_47 = 1;
    }
}

// Rewind-anim start: mirror of func_8029AE9C with the transforms swapped.
void CTutorial::func_8029AF7C() {
    if (func_80137510(mpAnimTrans1, lbl_eu_80668C08) != 0) {
        field_45 = 5;
        mpLayout->SetAnimationEnable(mpAnimTrans1, false);
        mpLayout->SetAnimationEnable(mpAnimTrans0, true);
        mpLayout->Animate(0);
    }
}

/* Rewind-animation (0x3C) reached the start frame: state 0, visible. */
__declspec(noinline) void CTutorial::func_8029B010() {
    if (func_80137510(mpAnimTrans0, lbl_eu_80668C08) != 0) {
        field_45 = 0;
        field_47 = 1;
    }
}

void func_8029B05C(){}

extern "C" __declspec(noinline) void func_8029B124(CTutorial* self) {}

// Reset a block of UI flags (0x3340..0x33BE) owned by this tutorial, then set
// the owner id 0x270 entry to a heap/direct-address classification mask.
void CTutorial::func_8029B498() {
    for (s16 i = 0x3340; i < 0x33bf; i++) {
        func_8009D018(i, (u32)this);
    }
    // (a | -a) >> 31 is -1 for any nonzero address, 0 for null; masked to the
    // 0x7F000000 window by the caller-side rlwinm.
    s32 addr = (s32)this;
    func_8009D018(0x270, ((addr | -addr) >> 31) & 0x7f000000);
}

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
    switch (self->field_45) {
    case 1:
        self->func_8029AE9C();
        break;
    case 2:
        self->func_8029AF30();
        break;
    case 4:
        self->func_8029AF7C();
        break;
    case 5:
        self->func_8029B010();
        break;
    default:
        break;
    }
    self->mpLayout->Animate(0);
}
extern "C" void func_8029ABD8() {}
extern "C" void func_8029ACEC() {}
// Page-navigation helper: mark complete when the counter already sits on the
// last page, else advance it clamped to bound-1, then refresh and play the
// confirm sound.
void CTutorial::func_8029AD88() {
    if ((s8)field_50 == (s8)field_51 - 1) {
        field_52 = 1;
        return;
    }
    field_50 = field_50 + 1;
    if ((s8)(u8)(field_50) >= (s8)field_51) {
        field_50 = field_51 - 1;
    }
    ::func_8029B124(this);
    func_80138078(8);
}

