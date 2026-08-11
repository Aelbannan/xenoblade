// Auto-scaffolded catalog TU for kyoshin/CQstLogInfo
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CQstLogInfo.hpp"

extern float lbl_eu_80668590;
extern u32 func_80137444(nw4r::lyt::AnimTransform*, float);
extern u32 func_80137510(nw4r::lyt::AnimTransform*, float);

u8 CQstLogInfo::func_802296D0() { return mField38; }

u8 CQstLogInfo::func_802296D8() { return mField39; }

void CQstLogInfo::func_80229768(u16 val) { mField3A = val; }

// Target: us-8022b5a8 - func_80229770
// If the animation at 0x24 finishes, transition to state 2.
void CQstLogInfo::func_80229770() {
    if (func_80137444(field_0x24, lbl_eu_80668590) != 0) {
        field_0x34 = 2;
        func_80229900();
    }
}

// Target: us-8022b5f4 - func_802297BC
// If the animation at 0x28 finishes, transition to state 3 and set field 0x39.
void CQstLogInfo::func_802297BC() {
    if (func_80137444(field_0x28, lbl_eu_80668590) != 0) {
        field_0x34 = 3;
        mField39 = 1;
    }
}

// Target: us-8022b640 - func_80229808
// If the animation at 0x28 is done (via func_80137510), transition to state 5.
void CQstLogInfo::func_80229808() {
    if (func_80137510(field_0x28, lbl_eu_80668590) != 0) {
        field_0x34 = 5;
        func_802298A0();
    }
}

void func_80229854(){}

void CQstLogInfo::func_802298A0(){}

void CQstLogInfo::func_80229900(){}

void func_80229960(){}

void func_80229A0C(){}

void func_80229B54(){}

void func_80229CA0(){}

void func_80229CF0(){}

void func_8022A904(){}

void func_8022AFF8(){}

void CQstLogInfo::OnFileEvent() {}

// retail: if (mUnk30) tail func_80137038(mUnk20, drawInfo passthrough, 0, 1)
extern "C" void func_80229600(CQstLogInfo* self, void* drawInfo){
    if (self->mUnk30 == 0) return;
    func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(self->mUnk20, drawInfo, 0, 1);
}

extern "C" void func_802294C0() {}
extern "C" void func_80229510() {}
extern "C" void func_80229570() {}
extern "C" void func_80229620() {}
