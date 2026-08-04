// Auto-scaffolded catalog TU for monolib/src/scn/CScnItemModelNw4r
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// Multiple targets map to the same function — single definition
u32 func_804871A8(void* self) { return *(u32*)((u8*)self + 0x4ac); }

u32 func_8048736C(void* self) { return *(u32*)((u8*)self + 0x14c0); }

extern "C" void func_804876C0(void* self) {
    *(u16*)((u8*)self + 0x17FA) |= 1;
    *(u16*)((u8*)self + 0x181A) |= 1;
}

void func_804EB798(void* self);
void func_804876DC(void* self) { ((void(*)(void*))func_804EB798)((char*)self + 0x17a0); }

void func_804EB7F8(void* self);
void func_804876E4(void* self) { ((void(*)(void*))func_804EB7F8)((char*)self + 0x17a0); }

void func_80487B84(){}

void func_80487C78(){}

void func_804E679C(void* self);
void func_80487E40(void* self) { ((void(*)(void*))func_804E679C)((char*)self + 0x1700); }

void func_804E6898(void* self);
void func_80487E48(void* self) { ((void(*)(void*))func_804E6898)((char*)self + 0x1700); }

u32 func_80487E50(void* self) { return *(u32*)((u8*)self + 0x1708); }

void func_80487E58(){}

void func_80487EB8(){}

void func_80487EE0(){}

void func_804884F8(){}

void func_8048856C(){}

void func_804885C8(){}

void func_804885FC(){}

void func_804888B4(){}

void func_80488938(){}

void func_80488954(){}

void func_80488984(){}

void func_80488A28(){}

void func_80488AAC(){}

void func_80488B50(){}

void func_804E5E38(void* self);
void func_80488C20(void* self) { ((void(*)(void*))func_804E5E38)((char*)self + 0x16c8); }

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

void func_8048AB0C(){}

void func_8048AB2C(){}

void func_8048B1F4(){}

void func_8048B30C(){}

void func_8048B3F0(){}

u32 func_8048B4BC(void* self) { return *(u32*)((u8*)self + 0x828); }

void func_8048B4C4(){}

void func_8048B54C(){}

void func_8048B608(){}

void func_8048B68C(){}

void func_8048B728(){}

void func_8048B7C0(){}

void func_8048BA58(){}

void func_80496FC4(void* self);
void func_8048BAD4(void* self) { ((void(*)(void*))func_80496FC4)((char*)self + 0xc); }

void func_8048BADC(){}

void func_8048BBF0(){}

void func_8048BD04(){}

void* func_8048BD1C(void* self, unsigned int size) {
    void* buffer = *(void**)((char*)self + 8);
    unsigned int current = *(unsigned int*)((char*)buffer + 0x860);
    if (size >= 0xC00 - current) return 0;
    unsigned int newOffset = current + size;
    *(unsigned int*)((char*)buffer + 0x860) = newOffset;
    return (char*)buffer + current + 0x864;
}

void func_8048BD50(void) {}

void func_8048BD54(void* self) { ((void(*)(void*))func_80489200)((char*)self - 0x1464); }

void func_8048BD5C(void* self) { ((void(*)(void*))func_8048917C)((char*)self - 0x1464); }

void func_8048BD64(void* self){ ((void(*)(void*))func_80489014)((char*)self - 0x1464); }

void __dt__17CScnItemModelNw4rFv(void*);
void func_8048BD6C(void* self) { ((void(*)(void*))__dt__17CScnItemModelNw4rFv)((char*)self - 0x1464); }

extern "C" void func_8048BD74(void* self) { ((void(*)(void*))func_804899F4)((char*)self - 0x1468); }

extern "C" void func_8048BD7C(void* self) { ((void(*)(void*))func_80489924)((char*)self - 0x1468); }

extern "C" void func_8048BD84(void* self) { ((void(*)(void*))func_80489584)((char*)self - 0x1468); }

extern "C" void func_8048BD8C(void* self) { ((void(*)(void*))__dt__17CScnItemModelNw4rFv)((char*)self - 0x1468); }
