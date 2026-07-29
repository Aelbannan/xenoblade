// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectModel
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectModel.hpp"
#include "kyoshin/cf/CfBdat.hpp"

cf::CfObjectModel::CfObjectModel() {}

void cf::CfObjectModel::CfObject_UnkVirtualFunc2() {}

cf::CfObjectModel::~CfObjectModel() {}

void __dt__800BAA24(){}

void cf::CfObjectModel::CfObject_UnkVirtualFunc6() {}

void cf::CfObjectModel::CfObjectModel_UnkVirtualFunc1() {}

void CfObjectModel_UnkVirtualFunc2__Q22cf13CfObjectModelFv() {}

void func_800BAB64(){}

void cf::CfObjectModel::CfObject_UnkVirtualFunc8() {}

void cf::CfObject::CfObject_UnkVirtualFunc63() {}

void cf::CfObjectModel::CfObject_UnkVirtualFunc63() {}

void func_800BAD98(){}

void func_800BADC8(){}

void func_800BADF8(){}

void func_800BAE28(){}

void cf::CfObjectModel::CfObject_UnkVirtualFunc19() {}

void cf::CfObjectModel::CfObject_UnkVirtualFunc22() {}

void cf::CfObject::CfObject_UnkVirtualFunc25() {}

void cf::CfObjectModel::CfObject_UnkVirtualFunc20() {}

u32 cf::CfObjectModel::CfObject_UnkVirtualFunc23() { return 0; }

void* CfObject_UnkVirtualFunc24__Q22cf13CfObjectModelFv(void* self) { void* p = *(void**)((u8*)self + 0x98); if (p != 0) { return (u8*)p + 0x310; } return (u8*)self + 0x3c; }

void cf::CfObjectModel::CfObject_UnkVirtualFunc27() {}

void* func_8048315C(void*);

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

extern "C" void CfObject_UnkVirtualFunc32__Q22cf13CfObjectModelFv(void* self, float f) {
    extern float lbl_eu_8066A210;
    // Thunk: scale f by global factor, then tail-call through vtable[0xC4] (CfObject_UnkVirtualFunc29)
    // Single-expression load to encourage MWCC to keep the vtable pointer in r12
    ((void (*)(void*, float))((*(void***)self)[0xC4 / 4]))(self, f * lbl_eu_8066A210);
}

void cf::CfObjectModel::CfObject_UnkVirtualFunc34() {}

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

void func_800BB340(){}

float cf::CfObjectModel::CfObject_UnkVirtualFunc56() { return 0.0f; }

void cf::CfObjectModel::CfObject_UnkVirtualFunc52() {}

cf::CfObject* cf::CfObjectModel::CfObject_UnkVirtualFunc53() { return 0; }

void cf::CfObjectModel::CfObject_UnkVirtualFunc54() {}

void cf::CfObjectModel::CfObject_UnkVirtualFunc55() {}

void cf::CfObjectModel::CObjectParam_UnkVirtualFunc2() {}

void cf::CfObjectModel::CfObjectModel_UnkVirtualFunc3() {}

void func_800BB618(){}

void cf::CfObjectModel::CfObject_UnkVirtualFunc66() {}

void cf::CfObjectModel::CfObject_UnkVirtualFunc67() {}

void cf::CfObjectModel::CfObject_UnkVirtualFunc70(float value) {}

void CfObject_UnkVirtualFunc72__Q22cf13CfObjectModelFv(void* self, float val) { *(float*)((u8*)self + 0xa4) = val; }

void CfObjectModel_UnkVirtualFunc10__Q22cf13CfObjectModelFv(void* self, float val) { *(float*)((u8*)self + 0xa8) = val; }

void CfObjectModel_UnkVirtualFunc12__Q22cf13CfObjectModelFv(void* self, float val) { *(float*)((u8*)self + 0xac) = val; }

void cf::CfObjectModel::CfObjectModel_UnkVirtualFunc13() {}

void cf::CfObjectModel::CfObjectModel_UnkVirtualFunc14() {}

extern "C" void CfObjectModel_UnkVirtualFunc15__Q22cf13CfObjectModelFv(void* self, int val) {
    // Compute (-val | val) >> 31 = (val != 0) ? 1 : 0, store at field_BC
    // Then tail-call to CfObjectModel_UnkVirtualFunc14 (vtable+0x1AC) passing val in r4
    u8 flag = (u8)(((u32)(-val) | (u32)(val)) >> 31);
    *((u8*)self + 0xBC) = flag;
    // Load vtable and tail-call to CfObjectModel_UnkVirtualFunc14 (vtable+0x1AC)
    ((void (*)(void*, int))((*(void***)self)[0x1AC / 4]))(self, val);
}

void func_800BB934(){}

void cf::CfObjectModel::CfObject_UnkVirtualFunc69() {}

void cf::CfObjectModel::CfObject_UnkVirtualFunc68() {}

void func_800BBA08(){}

void func_800BBA7C(){}

void cf::CfObjectModel::CfObjectModel_UnkVirtualFunc8() {}

void cf::CfObjectModel::CfObjectModel_UnkVirtualFunc7() {}

void func_800BBADC(){}

void func_800BBB50(){}

int CfObjectModel_UnkVirtualFunc6__Q22cf13CfObjectModelFv(void* self) { return 0; }

// CfObject.hpp declares this with (float, float) params but the retail symbol is Fv (no params),
// so we emit the exact mangled name via extern "C" to match the retail .o symbol table.
extern "C" void CfObject_UnkVirtualFunc20__Q22cf8CfObjectFv(void* self, float a, float b) {
    extern float lbl_eu_80666A68;
    // Load constant first, then store params around it to match retail scheduling:
    // lfs f0, constant -> stfs f1, 0x3c -> stfs f0, 0x40 -> stfs f2, 0x44 -> blr
    float c = lbl_eu_80666A68;
    *(float*)((char*)self + 0x3c) = a;
    *(float*)((char*)self + 0x40) = c;
    *(float*)((char*)self + 0x44) = b;
}

// Forward declaration - function is defined in CfBdat.cpp with C linkage (retail uses unmangled name)
extern "C" void func_80142428();

void func_800BBC04() {
    func_80142428();
}

const char* func_800BBC08(u16 index) {
    return cf::CfBdat::func_801424A8(index);
}

void func_800BBC0C(){}
