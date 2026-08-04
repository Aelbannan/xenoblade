// Auto-scaffolded catalog TU for monolib/src/scn/CScnEffectActNw4r
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern void func_80495E60();
void func_8049BEA4(void* self){ func_80495E60(); }

extern void func_80484E5C();
void func_8049BEAC(void* self){ func_80484E5C(); }

extern void func_804838DC();
void func_8049BEB4(void* self){ func_804838DC(); }

extern void func_8048315C();
void func_8049BEE4(void* self){ func_8048315C(); }

extern "C" void func_8049BEEC(void* self, const void* other) {
    u8* dst = *(u8**)((u8*)self + 4);
    *(u32*)(dst + 0x304) = *(u32*)((u8*)other + 0);
    *(u32*)(dst + 0x308) = *(u32*)((u8*)other + 4);
    *(u32*)(dst + 0x30C) = *(u32*)((u8*)other + 8);
}

void func_8049BF0C(){}

void func_8049BF34(){}

void func_8049BF5C(){}

void func_8049BF84(){}

void func_8049BFAC(){}

void func_8049C060(){}

void func_8049C18C(){}

void func_8049C244(){}

void func_8049C314(){}

extern void func_80482AB8();
void func_8049C394(void* self){ func_80482AB8(); }

extern void func_80482AD4();
void func_8049C39C(void* self){ func_80482AD4(); }

extern void func_8048856C();
void func_8049C3A4(void* self){ func_8048856C(); }

void func_8049C3AC(){}

extern "C" u32 func_8049C420(void* self) { return (*(u32*)((char*)*(void**)((u8*)self + 4) + 0x7A4) >> 30) & 1; }

extern "C" u32 func_8049C430(void* self) { return *(u32*)((u8*)self + 0x188); }

// Virtual dispatch target: v_i at vtable offset 8+4*i (MWCC RTTI header).
struct VTarget {
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual void v6() = 0;
    virtual void v7() = 0;
    virtual void v8() = 0;
    virtual void v9() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual void v12() = 0;
    virtual void v13() = 0;
    virtual void v14() = 0;
    virtual void v15() = 0;
    virtual void v16() = 0;
    virtual void v17() = 0;
    virtual void v18() = 0;
    virtual void v19() = 0;
    virtual void v20() = 0;
    virtual void v21() = 0;
    virtual void v22() = 0;
    virtual void v23() = 0;
    virtual void v24() = 0;
    virtual void v25() = 0;
    virtual void v26() = 0;
    virtual void v27() = 0;
    virtual void v28() = 0;
    virtual void v29() = 0;
    virtual void v30() = 0;
    virtual void v31() = 0;
    virtual void v32() = 0;
    virtual void v33() = 0;
    virtual void v34() = 0;
    virtual void v35() = 0;
    virtual void v36() = 0;
    virtual void v37() = 0;
    virtual void v38() = 0;
    virtual void v39() = 0;
    virtual void v40() = 0;
    virtual void v41() = 0;
    virtual void v42() = 0;
    virtual void v43() = 0;
    virtual ~VTarget() {}
};

extern "C" void func_8049BA44(void* self) {
    VTarget* obj = (VTarget*)*(void**)((u8*)self + 4);
    obj->v4();
}
extern "C" void func_8049BEBC(void* self) {
    VTarget* obj = (VTarget*)*(void**)((u8*)self + 4);
    obj->v43();
}
extern "C" void func_8049BED0(void* self) {
    VTarget* obj = (VTarget*)*(void**)((u8*)self + 4);
    obj->v16();
}
