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
    virtual ~VTarget() {}
};

// Auto-scaffolded catalog TU for monolib/src/scn/CScnLightMan
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

struct CScnLightMan {
    ~CScnLightMan();
};


void __ct__CScnLightMan(){}

CScnLightMan::~CScnLightMan() {}

void func_8048D124(void* arg1, void* arg2) {
    extern void func_804BF944(void*);
    func_804BF944(arg2);
    char* ptr = *(char**)((char*)arg1 + 8);
    ptr[0x121c] = 1;
}

void func_8048D160(){}

extern "C" void func_8048D1B0(void* self) {
    VTarget* obj = (VTarget*)*(void**)((u8*)self + 8);
    obj->v3();
}
