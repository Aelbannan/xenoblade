// Auto-scaffolded catalog TU for monolib/src/scn/CScnItemModelNw4r
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// Multiple targets map to the same function — single definition
extern "C" u32 func_804871A8(void* self) { return *(u32*)((u8*)self + 0x4ac); }

extern "C" u32 func_8048736C(void* self) { return *(u32*)((u8*)self + 0x14c0); }

extern "C" void func_804876C0() {}

extern "C" void func_804EB798(void* self);
extern "C" void func_804876DC(void* self) { ((void(*)(void*))func_804EB798)((char*)self + 0x17a0); }

extern "C" void func_804EB7F8(void* self);
extern "C" void func_804876E4(void* self) { ((void(*)(void*))func_804EB7F8)((char*)self + 0x17a0); }

extern "C" void func_80487B84() {}

extern "C" void func_80487C78() {}

extern "C" void func_804E679C(void* self);
extern "C" void func_80487E40(void* self) { ((void(*)(void*))func_804E679C)((char*)self + 0x1700); }

extern "C" void func_804E6898(void* self);
extern "C" void func_80487E48(void* self) { ((void(*)(void*))func_804E6898)((char*)self + 0x1700); }

extern "C" u32 func_80487E50(void* self) { return *(u32*)((u8*)self + 0x1708); }

extern "C" void func_80487E58() {}

extern "C" void func_80487EB8() {}

extern "C" void func_80487EE0() {}

extern "C" void func_804884F8() {}

extern "C" void func_8048856C() {}

extern "C" void func_804885C8() {}

extern "C" void func_804885FC() {}

extern "C" void func_804888B4() {}

extern "C" void func_80488938() {}

extern "C" void func_80488954() {}

extern "C" void func_80488984() {}

extern "C" void func_80488A28() {}

extern "C" void func_80488AAC() {}

extern "C" void func_80488B50() {}

extern "C" void func_804E5E38(void* self);
extern "C" void func_80488C20(void* self) { ((void(*)(void*))func_804E5E38)((char*)self + 0x16c8); }

extern "C" void func_80488C28() {}

extern "C" void func_80488C78() {}

extern "C" void func_80488CF8() {}

extern "C" void func_80488D14() {}

extern "C" void func_80488D54() {}

extern "C" void func_80488EF4() {}

extern "C" void func_80488F44() {}

extern "C" void func_80488FEC() {}

extern "C" void func_80489000() {}

extern "C" void func_80489014() {}

extern "C" void func_8048917C() {}

extern "C" void func_80489200() {}

extern "C" void func_80489584() {}

extern "C" void func_80489924() {}

extern "C" void func_804899F4() {}

extern "C" void func_80489A60() {}

extern "C" void func_80489C94() {}

extern "C" void func_80489E80() {}

extern "C" void func_80489FDC() {}

extern "C" void func_8048A0B4() {}

extern "C" void func_8048A17C() {}

extern "C" void func_8048A588() {}

extern "C" void func_8048AB0C() {}

extern "C" void func_8048AB2C() {}

extern "C" void func_8048B1F4() {}

extern "C" void func_8048B30C() {}

extern "C" void func_8048B3F0() {}

extern "C" u32 func_8048B4BC(void* self) { return *(u32*)((u8*)self + 0x828); }

extern "C" void func_8048B4C4() {}

extern "C" void func_8048B54C() {}

extern "C" void func_8048B608() {}

extern "C" void func_8048B68C() {}

extern "C" void func_8048B728() {}

extern "C" void func_8048B7C0() {}

extern "C" void func_8048BA58() {}

extern "C" void func_80496FC4(void* self);
extern "C" void func_8048BAD4(void* self) { ((void(*)(void*))func_80496FC4)((char*)self + 0xc); }

extern "C" void func_8048BADC() {}

extern "C" void func_8048BBF0() {}

extern "C" void func_8048BD04() {}

extern "C" void* func_8048BD1C(void* self, unsigned int size) {
    void* buffer = *(void**)((char*)self + 8);
    unsigned int current = *(unsigned int*)((char*)buffer + 0x860);
    if (size >= 0xC00 - current) return 0;
    unsigned int newOffset = current + size;
    *(unsigned int*)((char*)buffer + 0x860) = newOffset;
    return (char*)buffer + current + 0x864;
}

extern "C" void func_8048BD50(void) {}

extern "C" void func_8048BD54(void* self) { ((void(*)(void*))func_80489200)((char*)self - 0x1464); }

extern "C" void func_8048BD5C(void* self) { ((void(*)(void*))func_8048917C)((char*)self - 0x1464); }

extern "C" void func_8048BD64(void* self) { ((void(*)(void*))func_80489014)((char*)self - 0x1464); }

extern "C" void __dt__17CScnItemModelNw4rFv(void*);
extern "C" void func_8048BD6C(void* self) { ((void(*)(void*))__dt__17CScnItemModelNw4rFv)((char*)self - 0x1464); }

extern "C" void func_8048BD74(void* self) { ((void(*)(void*))func_804899F4)((char*)self - 0x1468); }

extern "C" void func_8048BD7C(void* self) { ((void(*)(void*))func_80489924)((char*)self - 0x1468); }

extern "C" void func_8048BD84(void* self) { ((void(*)(void*))func_80489584)((char*)self - 0x1468); }

extern "C" void func_8048BD8C(void* self) { ((void(*)(void*))__dt__17CScnItemModelNw4rFv)((char*)self - 0x1468); }
