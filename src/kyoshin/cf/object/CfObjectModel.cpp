// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectModel
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include "kyoshin/cf/object/CfObjectModel.hpp"
#include "kyoshin/cf/CfBdat.hpp"

cf::CfObjectModel::CfObjectModel() {}

void cf::CfObjectModel::CfObject_UnkVirtualFunc2() {}

cf::CfObjectModel::~CfObjectModel() {}

// POD deleting destructor (retail symbol is address-derived, no class name):
// delete self only when a delete flag is passed, then return self.
u8* __dt__800BAA24(u8* object, s32 deleteFlag) {
    if (object != 0 && deleteFlag > 0) {
        __dl__FPv(object);
    }
    return object;
}

void cf::CfObjectModel::CfObject_UnkVirtualFunc6() {}

void cf::CfObjectModel::CfObjectModel_UnkVirtualFunc1() {}

void CfObjectModel_UnkVirtualFunc2__Q22cf13CfObjectModelFv() {}

void func_800BAB64(){}

void cf::CfObjectModel::CfObject_UnkVirtualFunc8() {}

void cf::CfObject::CfObject_UnkVirtualFunc63() {}

void cf::CfObjectModel::CfObject_UnkVirtualFunc63() {}

// Call the cf-chain vtable slot +0x14C (retail CfObject_UnkVirtualFunc63
// returns a flag word) and return bit 1 of it.
u32 func_800BAD98(cf::CfObject* obj) {
    return (reinterpret_cast<cf::CfObjectVt14C*>(obj)->m14C() >> 1) & 1;
}

// Same as func_800BAD98 but returns bit 2 of the flag word.
u32 func_800BADC8(cf::CfObject* obj) {
    return (reinterpret_cast<cf::CfObjectVt14C*>(obj)->m14C() >> 2) & 1;
}

// Same as func_800BAD98 but returns bit 3 of the flag word.
u32 func_800BADF8(cf::CfObject* obj) {
    return (reinterpret_cast<cf::CfObjectVt14C*>(obj)->m14C() >> 3) & 1;
}

// Call the cf-chain vtable slot +0x14C and return bit 7 of the flag word.
u32 func_800BAE28(cf::CfObject* obj) {
    return (reinterpret_cast<cf::CfObjectVt14C*>(obj)->m14C() >> 7) & 1;
}

void cf::CfObjectModel::CfObject_UnkVirtualFunc19() {}

void cf::CfObjectModel::CfObject_UnkVirtualFunc22() {}

void cf::CfObject::CfObject_UnkVirtualFunc25() {}

void cf::CfObjectModel::CfObject_UnkVirtualFunc20() {}

extern "C" void* func_8048315C(void*);

// Return a pointer-typed word: the sub-object's derived value +0xB8, or
// this +0x3C when there is no sub-object.
u32 cf::CfObjectModel::CfObject_UnkVirtualFunc23() {
    if (mSubObj98 != 0) {
        return static_cast<u32>(reinterpret_cast<uintptr_t>(func_8048315C(mSubObj98)) + 0xB8);
    }
    return static_cast<u32>(reinterpret_cast<uintptr_t>(this) + 0x3C);
}

void* CfObject_UnkVirtualFunc24__Q22cf13CfObjectModelFv(void* self) { void* p = *(void**)((u8*)self + 0x98); if (p != 0) { return (u8*)p + 0x310; } return (u8*)self + 0x3c; }

void cf::CfObjectModel::CfObject_UnkVirtualFunc27(void* src) {}

void* CfObject_UnkVirtualFunc28__Q22cf13CfObjectModelFv(void* self) {
    struct Data {
        u8 _pad00[0x48];
        u8 field_48[0x50];  // 0x48-0x97
        void* mPtr;         // 0x98
    };
    Data* data = static_cast<Data*>(self);
    void* ptr = data->mPtr;
    if (ptr) {
        return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(func_8048315C(ptr)) + 0xC4);
    } else {
        return data->field_48;
    }
}

void cf::CfObjectModel::CfObject_UnkVirtualFunc29() {}

float CfObject_UnkVirtualFunc31__Q22cf13CfObjectModelFv(void* self) {
    void* ptr = *(void**)((unsigned char*)self + 0x98);
    if (ptr != 0) {
        void* result = func_8048315C(ptr);
        return *(float*)((unsigned char*)result + 0xc8);
    } else {
        return *(float*)((unsigned char*)self + 0x4c);
    }
}

extern "C" void CfObject_UnkVirtualFunc32__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, float f) {
    // Scale f by the global factor, then tail-call the virtual
    // CfObject_UnkVirtualFunc29 (vtable+0xC4) with the scaled value. The
    // upcast to CfObject resolves the base (float) overload that CfObjectModel
    // hides with its no-arg override.
    ((cf::CfObject*)self)->CfObject_UnkVirtualFunc29(f * lbl_eu_8066A210);
}

// Scale the CfObject_UnkVirtualFunc31 result by the global scale factor.
float cf::CfObjectModel::CfObject_UnkVirtualFunc34() {
    float value = CfObject_UnkVirtualFunc31();
    return value * lbl_eu_8066A20C;
}

void CfObject_UnkVirtualFunc35__Q22cf13CfObjectModelFv(void* self, float f) {
    void* p = *(void**)((u8*)self + 0x98);
    if (p != 0) {
        *(float*)((u8*)p + 0x304) = f;
        *(float*)((u8*)p + 0x308) = f;
        *(float*)((u8*)p + 0x30c) = f;
    }
    *(float*)((u8*)self + 0x60) = f;
}

float CfObject_UnkVirtualFunc36__Q22cf13CfObjectModelFv(void* self) { return *(float*)((u8*)self + 0x60); }

// Returns bit 4 of the sub-object's +0x7A4 flag word (0 when no sub-object).
u32 func_800BB340(cf::CfObjectModel* self) {
    cf::CfObjectModelSub98* sub = self->mSubObj98;
    if (sub != 0) {
        // Retail dispatches to the sub-object's vtable slot +0xA8 and returns
        // its value; the slot returns u32 in retail, so it goes through the
        // CfObjectModelSub98Vt proxy (the base header's Func22 is void).
        return reinterpret_cast<cf::CfObjectModelSub98Vt*>(sub)->mA8();
    }
    return 0;
}

float cf::CfObjectModel::CfObject_UnkVirtualFunc56() { return 0.0f; }

void cf::CfObjectModel::CfObject_UnkVirtualFunc52() {}

cf::CfObject* cf::CfObjectModel::CfObject_UnkVirtualFunc53() { return 0; }

// Return the sub-object's +0x14AC word when a sub-object exists and its
// +0x7A4 0x40000000 flag is set, else 0. The retail symbol is Fv with a
// value return, so the exact mangled name is written verbatim.
u32 CfObject_UnkVirtualFunc54__Q22cf13CfObjectModelFv(cf::CfObjectModel* self) {
    cf::CfObjectModelSub98* sub = self->mSubObj98;
    u32 flag = (sub != 0) && ((sub->field_7A4 & 0x40000000) != 0);
    if (flag != 0) {
        return sub->field_14AC;
    }
    return 0;
}

void cf::CfObjectModel::CfObject_UnkVirtualFunc55() {}

void* cf::CfObjectModel::CObjectParam_UnkVirtualFunc2() { return 0; }

// Return the sub-object's vtable+0x18 result, or a static null placeholder
// when there is no sub-object (retail tail-calls the sub-object's slot).
void* cf::CfObjectModel::CfObjectModel_UnkVirtualFunc3() {
    if (mSubObj98 != 0) {
        return reinterpret_cast<CfObjectModelSub98Vt18*>(mSubObj98)->m18();
    }
    return lbl_eu_804FC548;
}

// If the sub-object's +0x7A4 bit-1 flag differs from the requested flag,
// forward (sub, flag) to the model visibility helper as a tail call.
void func_800BB618(cf::CfObjectModel* self, u32 flag) {
    cf::CfObjectModelSub98* sub = self->mSubObj98;
    if (sub != 0 && ((sub->field_7A4 >> 1) & 1) != flag) {
        func_804838DC(sub, flag);
    }
}

void cf::CfObjectModel::CfObject_UnkVirtualFunc66(int) {}

void cf::CfObjectModel::CfObject_UnkVirtualFunc67() {}

// Store the value at +0xA0, then forward it (as a tail call) to the
// sub-object's vtable slot +0x48 when a sub-object is present. The retail
// symbol is Fv (no params) even though the function reads f1, so the exact
// mangled name is written verbatim (MWCC emits names containing __ as-is).
void CfObject_UnkVirtualFunc70__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, float value) {
    cf::CfObjectModelSub98* sub = self->mSubObj98;
    self->field_A0 = value;
    if (sub == 0) {
        return;
    }
    reinterpret_cast<cf::CfObjectModelSub98Vt*>(sub)->CObjectParam_UnkVirtualFunc4();
}

void CfObject_UnkVirtualFunc72__Q22cf13CfObjectModelFv(void* self, float val) { *(float*)((u8*)self + 0xa4) = val; }

void CfObjectModel_UnkVirtualFunc10__Q22cf13CfObjectModelFv(void* self, float val) { *(float*)((u8*)self + 0xa8) = val; }

void CfObjectModel_UnkVirtualFunc12__Q22cf13CfObjectModelFv(void* self, float val) { *(float*)((u8*)self + 0xac) = val; }

void cf::CfObjectModel::CfObjectModel_UnkVirtualFunc13() {}

void cf::CfObjectModel::CfObjectModel_UnkVirtualFunc14() {}

extern "C" void CfObjectModel_UnkVirtualFunc15__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, int val) {
    // Store (val != 0) at field_BC, then tail-call the virtual
    // CfObjectModel_UnkVirtualFunc14 (vtable+0x1AC). val stays in r4 for the
    // callee (the retail Fv annotation omits the parameter).
    self->field_BC = (u8)(((u32)(-val) | (u32)(val)) >> 31);
    self->CfObjectModel_UnkVirtualFunc14();
}

extern "C" u32 func_800BB934(cf::CfObjectModel* self) {
    // Return bit 0 of the sub-object's +0x7A8 flag word (0 when no sub-object).
    cf::CfObjectModelSub98* sub = self->mSubObj98;
    if (sub != 0) {
        return sub->field_7A8 & 1;
    }
    return 0;
}

extern "C" u32 CfObject_UnkVirtualFunc69__Q22cf13CfObjectModelFv(cf::CfObjectModel* self) {
    // Return bit 1 of the sub-object's +0x7A4 flag word (0 when no sub-object).
    cf::CfObjectModelSub98* sub = self->mSubObj98;
    if (sub != 0) {
        return (sub->field_7A4 >> 1) & 1;
    }
    return 0;
}

void cf::CfObjectModel::CfObject_UnkVirtualFunc68() {}

void func_800BBA08(){}

// Tail-call the sub-object's vtable slot 0x8C (CfObject_UnkVirtualFunc15
// in the base vtable layout); the 2nd argument rides along untouched in r4.
void func_800BBA7C(cf::CfObjectModel* self, f32* vec) {
    if (self->mSubObj98 != 0) {
        ((cf::CfObject*)self->mSubObj98)->CfObject_UnkVirtualFunc15();
    }
}

// Return bit 4 of the sub-object's +0x7A4 flag word (0 when no sub-object).
u32 cf::CfObjectModel::CfObjectModel_UnkVirtualFunc8() {
    if (mSubObj98 != 0) {
        return (mSubObj98->field_7A4 >> 4) & 1;
    }
    return 0;
}

// Tail-call the sub-object's vtable slot 0xB4 (CfObject_UnkVirtualFunc25
// in the base vtable layout) when a sub-object is present.
void cf::CfObjectModel::CfObjectModel_UnkVirtualFunc7() {
    if (mSubObj98 != 0) {
        ((cf::CfObject*)mSubObj98)->CfObject_UnkVirtualFunc25();
    }
}

void func_800BBADC(){}

void func_800BBB50(){}

int CfObjectModel_UnkVirtualFunc6__Q22cf13CfObjectModelFv(void* self) { return 0; }

// CfObject.hpp declares this with (float, float) params but the retail symbol is Fv (no params),
// so we emit the exact mangled name via extern "C" to match the retail .o symbol table.
extern "C" void CfObject_UnkVirtualFunc20__Q22cf8CfObjectFv(cf::CfObject* self, float a, float b) {
    // MWCC pairs the constant load with its store and emits stores in FPR
    // order (f0, f1, f2) - retail stores a, b, c with the constant last.
    // Documented open item (MWCC_REFERENCE pair-copy register allocation,
    // func_800BC510): invariant to source store order / locals / helpers.
    float c = lbl_eu_80666A68;
    self->mPos3C = a;
    self->mPos44 = b;
    self->mPos40 = c;
}

void func_800BBC04() {
    func_80142428();
}

const char* func_800BBC08(u16 index) {
    return cf::CfBdat::func_801424A8(index);
}

void func_800BBC0C(){}
