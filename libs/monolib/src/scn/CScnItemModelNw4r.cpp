// Auto-scaffolded catalog TU for monolib/src/scn/CScnItemModelNw4r
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// Multiple targets map to the same function - single definition
u32 func_804871A8(u8* self) { return *(u32*)((u8*)self + 0x4ac); }

u32 func_8048736C(u8* self) { return *(u32*)((u8*)self + 0x14c0); }

extern "C" void func_804876C0(u8* self) {
    *(u16*)((u8*)self + 0x17FA) |= 1;
    *(u16*)((u8*)self + 0x181A) |= 1;
}

void func_804EB798(u8* self);
void func_804876DC(u8* self) { ((void(*)(void*))func_804EB798)((char*)self + 0x17a0); }

void func_804EB7F8(u8* self);
void func_804876E4(u8* self) { ((void(*)(void*))func_804EB7F8)((char*)self + 0x17a0); }

void func_80487B84(){}

void func_80487C78(){}

void func_804E679C(u8* self);
void func_80487E40(u8* self) { ((void(*)(void*))func_804E679C)((char*)self + 0x1700); }

void func_804E6898(u8* self);
void func_80487E48(u8* self) { ((void(*)(void*))func_804E6898)((char*)self + 0x1700); }

u32 func_80487E50(u8* self) { return *(u32*)((u8*)self + 0x1708); }

void func_80487E58(){}

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
    virtual void* v24() = 0;
    virtual ~VTarget() {}
};

extern "C" void* func_80487EB8(u8* self) {
    void* obj = *(void**)((u8*)self + 0x7C4);
    if (obj != 0) return ((VTarget*)obj)->v24();
    return *(void**)((u8*)self + 0x179C);
}

void func_80487EE0(){}

void func_804884F8(){}

void func_8048856C(){}

extern "C" u32 func_804885C8(u8* self) {
    u32 r = 0;
    if (*(u32*)((u8*)self + 0x7C8) != 0 || *(u32*)((u8*)self + 0x854) != 0 || (*(u32*)((u8*)self + 0x7A8) & 0x800)) {
        r = 1;
    }
    return r;
}

void func_804885FC(){}

void func_804888B4(){}

void func_80488938(){}

void func_80488954(){}

void func_80488984(){}

void func_80488A28(){}

void func_80488AAC(){}

void func_80488B50(){}

void func_804E5E38(u8* self);
void func_80488C20(u8* self) { ((void(*)(void*))func_804E5E38)((char*)self + 0x16c8); }

void func_80488C28(){}

void func_80488C78(){}

void func_80488CF8(){}

void func_80488D14(){}

void func_80488D54(){}

void func_80488EF4(){}

void func_80488F44(){}

void func_80488FEC(){}

void func_80489000(){}

void func_80489014(){}

void func_8048917C(){}

void func_80489200(){}

void func_80489584(){}

void func_80489924(){}

void func_804899F4(){}

void func_80489A60(){}

void func_80489C94(){}

void func_80489E80(){}

void func_80489FDC(){}

void func_8048A0B4(){}

void func_8048A17C(){}

void func_8048A588(){}

extern "C" void func_8048AB0C(u8* self, u32 mode) {
    if ((s32)mode == 1) {
        *(void**)((u8*)self + 0x7EC) = (u8*)self + 0x1650;
    } else {
        *(void**)((u8*)self + 0x7EC) = 0;
    }
}

void func_8048AB2C(){}

void func_8048B1F4(){}

void func_8048B30C(){}

void func_8048B3F0(){}

u32 func_8048B4BC(u8* self) { return *(u32*)((u8*)self + 0x828); }

void func_8048B4C4(){}

void func_8048B54C(){}

void func_8048B608(){}

void func_8048B68C(){}

void func_8048B728(){}

void func_8048B7C0(){}

void func_8048BA58(){}

void func_80496FC4(u8* self);
void func_8048BAD4(u8* self) { ((void(*)(void*))func_80496FC4)((char*)self + 0xc); }

void func_8048BADC(){}

void func_8048BBF0(){}

void func_8048BD04(){}

void* func_8048BD1C(u8* self, unsigned int size) {
    void* buffer = *(void**)((char*)self + 8);
    unsigned int current = *(unsigned int*)((char*)buffer + 0x860);
    if (size >= 0xC00 - current) return 0;
    unsigned int newOffset = current + size;
    *(unsigned int*)((char*)buffer + 0x860) = newOffset;
    return (char*)buffer + current + 0x864;
}

void func_8048BD50(void) {}

void func_8048BD54(u8* self) { ((void(*)(void*))func_80489200)((char*)self - 0x1464); }

void func_8048BD5C(u8* self) { ((void(*)(void*))func_8048917C)((char*)self - 0x1464); }

void func_8048BD64(u8* self){ ((void(*)(void*))func_80489014)((char*)self - 0x1464); }

void __dt__17CScnItemModelNw4rFv(void*);
void func_8048BD6C(u8* self) { ((void(*)(void*))__dt__17CScnItemModelNw4rFv)((char*)self - 0x1464); }

extern "C" void func_8048BD74(u8* self) { ((void(*)(void*))func_804899F4)((char*)self - 0x1468); }

extern "C" void func_8048BD7C(u8* self) { ((void(*)(void*))func_80489924)((char*)self - 0x1468); }

extern "C" void func_8048BD84(u8* self) { ((void(*)(void*))func_80489584)((char*)self - 0x1468); }

extern "C" void func_8048BD8C(u8* self) { ((void(*)(void*))__dt__17CScnItemModelNw4rFv)((char*)self - 0x1468); }

extern "C" void func_80487374() {}
extern "C" void func_804873EC() {}
extern "C" void func_804875B8() {}
extern "C" void func_804876EC() {}
extern "C" void func_8048776C() {}
extern "C" void func_80487818() {}
extern "C" void func_80487B18() {}
