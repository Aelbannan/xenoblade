// Auto-scaffolded catalog TU for monolib/src/scn/CScn_80496B0C
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void func_8049771C(void* self, float val) { *(float*)((u8*)self + 0x178) = val; }











void func_80497724(){}

// Virtual dispatch target: v_i at vtable offset 8+4*i (MWCC RTTI header).
struct VTarget {
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual ~VTarget() {}
};

extern "C" void func_80497760(void* self) {
    void* p = *(void**)((u8*)self + 8);
    if (p == 0) return;
    p = *(void**)((u8*)p + 0x84);
    if (p == 0) return;
    ((VTarget*)*(void**)((u8*)p + 0x10))->v5();
}

extern "C" { extern const float lbl_eu_8066AAC0; }
extern "C" float func_80497790(void* self) {
    void* p = *(void**)((u8*)self + 8);
    if (p == 0) return lbl_eu_8066AAC0;
    p = *(void**)((u8*)p + 0x84);
    if (p != 0) return *(float*)((u8*)p + 0x1C);
    return lbl_eu_8066AAC0;
}

extern "C" float func_804977C0(void* self) {
    void* p = *(void**)((u8*)self + 0xC);
    if (p == 0) return lbl_eu_8066AAC0;
    p = *(void**)((u8*)p + 0x84);
    if (p != 0) return *(float*)((u8*)p + 0x1C);
    return lbl_eu_8066AAC0;
}

void func_804977F0(){}

void func_8049782C(){}

extern "C" u32 func_804978B8(void* self) { return *(u32*)(*(u32*)((u8*)self + 8) + 0x84) != 0; }

void func_804978D0(){}

void func_80497914(){}

extern "C" u32 func_8049798C(void* self) { return *(u32*)(*(u32*)((u8*)self + 0xC) + 0x84) != 0; }

void func_804979A4(){}

void func_80497AA8(){}

void func_80497F34(){}

void __dt__80497FEC(){}

void func_804980E4(){}

void func_80498288(){}

void func_804986F8(){}

void func_80498A5C(){}
