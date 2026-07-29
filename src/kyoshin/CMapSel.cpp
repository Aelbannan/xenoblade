// Auto-scaffolded catalog TU for kyoshin/CMapSel
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CMapSel.hpp"
#include "kyoshin/code_80135FDC.hpp"

extern "C" void func_801D216C(void*, int);
extern "C" void func_80138078(u32);
extern "C" float lbl_eu_8066873C;

// Forward declarations for local functions called by matched code
void func_80243CFC(CMapSel*);
void func_80244020(CMapSel*);

void __ct__CMapSel(){}

CMapSel::~CMapSel() {}

void func_8024343C(){}

void func_802434A0(){}

void func_80243560(){}

void func_802435CC(){}

void func_80243680(){}

// FULL_MATCH: reads the initial-setup flag at +0x33 (set to 1 in ctor, cleared on play/close)

// State 0 -> 1: enter loading state, clear flag, initialize
void func_802436CC(CMapSel* self) {
    if (self->mState == 0) {
        self->mState = 1;
        self->mFlag33 = 0;
        func_80243CFC(self);
        func_80244020(self);
    }
}

// State 3 -> 4: close the map select widget and play close sound (sfx 6)
void func_8024371C(CMapSel* self) {
    if (self->mState == 3) {
        self->mState = 4;
        self->mFlag33 = 0;
        func_801D216C(self->field_0x74, 0);
        func_80138078(6);
    }
}

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

void func_80243BE8(){}

// Wait for anim transform 1 to finish, then reset to state 0 with flag set
void func_80243C6C(CMapSel* self) {
    nw4r::lyt::AnimTransform* anim = self->mAnimTransform1;
    if (func_80137510(anim, lbl_eu_8066873C)) {
        self->mFlag33 = 1;
        self->mState = 0;
    }
}

void func_80243CB8(){}

void func_80243CFC(){}

void func_80243E08(){}

void func_80243ED8(){}

void func_80243FC4(){}

void func_80244020(){}

void CMapSel::OnFileEvent() {}
