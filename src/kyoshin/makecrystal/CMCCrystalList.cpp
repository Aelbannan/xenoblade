// Auto-scaffolded catalog TU for kyoshin/makecrystal/CMCCrystalList
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/makecrystal/CMCCrystalList.hpp"

#include "kyoshin/code_80135FDC.hpp"

#include <revolution/os/OSError.h>

void __ct__CMCCrystalList(){}

CMCCrystalList::~CMCCrystalList() {}

void func_80222848(){}

void func_802228B8(){}

namespace nw4r { namespace lyt { class Layout; class DrawInfo; } }
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

void CMCCrystalList::func_80222964(nw4r::lyt::DrawInfo* drawInfo)
{
    if (mState != 0) {
        func_80137038(*reinterpret_cast<nw4r::lyt::Layout**>(reinterpret_cast<unsigned char*>(this) + 0x34), drawInfo, 0, 1);
    }
}

void __dt__80222984(){}

u8 CMCCrystalList::func_80222A50() { return mDataFlag; }

u8 CMCCrystalList::func_80222A58() { return mIsActive; }

// Callees are unmangled retail symbols (func_80223698 etc.). C++ linkage would
// mangle the reloc to func_80223698__Fv and drift from the retail name, so
// declare them with C linkage (harness-sanctioned fix for reloc name drift).
// They take the CMCCrystalList* they operate on (retail passes self in r3).
extern "C" void func_80223698(CMCCrystalList* self);
extern "C" void func_80223614(CMCCrystalList* self);
extern "C" void func_80223754(CMCCrystalList* self);
extern "C" void func_80223810();
extern "C" void func_802238CC();

void func_80222A60(CMCCrystalList* self)
{
    if (self->mStateIdx == 0) {
        self->mStateIdx = 1;
        self->mIsActive = 0;
        func_80223698(self);
    }
}

void func_80222A84(CMCCrystalList* self)
{
    if (self->mStateIdx == 3) {
        self->mStateIdx = 6;
        self->mIsActive = 0;
        func_80223810();
    }
}

void func_80222AA8(CMCCrystalList* self)
{
    if (self->mStateIdx == 3) {
        self->mStateIdx = 7;
        self->mIsActive = 0;
        func_80223810();
    }
}

void func_80222ACC(CMCCrystalList* self)
{
    if (self->mStateIdx == 3) {
        self->mStateIdx = 8;
        self->mIsActive = 0;
        func_802238CC();
    }
}

void func_80222AF0(CMCCrystalList* self)
{
    if (self->mStateIdx == 3) {
        self->mStateIdx = 4;
        self->mIsActive = 0;
        func_80223754(self);
    }
}

void func_80222B14(){}

void func_80222D9C(){}

void func_80222F64(){}

void func_80223004(){}

// Retail 0x802232E4: reset anim trans 4's frame to 0, then re-animate the
// layout (virtual Animate at vtable+0x38, r4 = 0 option).
void func_802232E4(CMCCrystalList* self)
{
    func_802238CC();
    self->mAnimTrans4->SetFrame(0.0f);
    self->mLayout->Animate(0);
}

void func_80223334(){}

// Retail 0x802233AC: wait for anim trans 1 (via func_80137444) to finish,
// then enter state 2 and run func_80223754.
void func_802233AC(CMCCrystalList* self)
{
    if (func_80137444(self->mAnimTrans1, lbl_eu_80668544)) {
        self->mStateIdx = 2;
        func_80223754(self);
    }
}

// Retail 0x802233F8: wait for anim trans 2 to finish, then activate state 3.
void func_802233F8(CMCCrystalList* self)
{
    if (func_80137444(self->mAnimTrans2, lbl_eu_80668544) != 0) {
        self->mStateIdx = 3;
        self->mIsActive = 1;
    }
}

// Retail 0x80223444: for each crystal slot in state 1, run func_80223614.
// u8 loop counter (retail masks the slot index to 8 bits before indexing).
void func_80223444(CMCCrystalList* self)
{
    for (u8 i = 0; i < 8; i++) {
        if (self->mSlotStates[i] == 1) {
            func_80223614(self);
        }
    }
}

// Retail 0x80223498: wait for anim trans 2 (via func_80137510) to finish,
// then enter state 5 and run func_80223698.
void func_80223498(CMCCrystalList* self)
{
    if (func_80137510(self->mAnimTrans2, lbl_eu_80668544) != 0) {
        self->mStateIdx = 5;
        func_80223698(self);
    }
}

// Retail 0x802234E4: wait for anim trans 1 (via func_80137510) to finish,
// then reset to state 0 and activate.
void func_802234E4(CMCCrystalList* self)
{
    if (func_80137510(self->mAnimTrans1, lbl_eu_80668544) != 0) {
        self->mStateIdx = 0;
        self->mIsActive = 1;
    }
}

// Retail 0x80223530: wait for anim trans 3 to finish, then activate state 3.
void func_80223530(CMCCrystalList* self)
{
    if (func_80137444(self->mAnimTrans3, lbl_eu_80668544) != 0) {
        self->mStateIdx = 3;
        self->mIsActive = 1;
    }
}

// Retail 0x8022357C: wait for anim trans 3 (via func_80137510) to finish,
// then activate state 3.
void func_8022357C(CMCCrystalList* self)
{
    if (func_80137510(self->mAnimTrans3, lbl_eu_80668544) != 0) {
        self->mStateIdx = 3;
        self->mIsActive = 1;
    }
}

// Retail 0x802235C8: wait for anim trans 4 (via func_80137444) to finish,
// then activate state 3.
void func_802235C8(CMCCrystalList* self)
{
    if (func_80137444(self->mAnimTrans4, lbl_eu_80668544) != 0) {
        self->mStateIdx = 3;
        self->mIsActive = 1;
    }
}

extern "C" void __declspec(noinline) func_80223614(CMCCrystalList* self){ OSReport("stub: func_80223614"); }

// Stubs for not-yet-decompiled sibling functions. extern "C" so the emitted
// symbol matches the unmangled retail name (reloc-name gate). __declspec(noinline)
// (same mechanism as DECOMP_DONT_INLINE, written out because DECOMP_* macros
// are banned in this session) keeps MWCC's -inline auto from folding the stub
// body into the callers.
extern "C" void __declspec(noinline) func_80223698(CMCCrystalList* self){ OSReport("stub: func_80223698"); }

extern "C" void __declspec(noinline) func_80223754(CMCCrystalList* self){ OSReport("stub: func_80223754"); }

extern "C" void __declspec(noinline) func_80223810(){ OSReport("stub: func_80223810"); }

extern "C" void __declspec(noinline) func_802238CC(){ OSReport("stub: func_802238CC"); }

void func_80223988(){}

bool CMCCrystalList::OnFileEvent(CEventFile* pEventFile) { return false; }
