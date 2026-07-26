// Auto-scaffolded catalog TU for kyoshin/CCollepedia
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__CCLPCur() {}

extern "C" void __dt__802531C4() {}

extern "C" void func_80253204() {}

extern "C" void func_802532FC() {}

extern "C" void func_80253360() {}

extern "C" void func_8025338C() {}

extern "C" void func_802533F4() {}

struct CCollepedia {
    int field_0;
    int field_4;
    int field_8;
    unsigned char field_c;
    unsigned char field_d;
    unsigned char field_e;
};

extern "C" void func_8025348C(CCollepedia* obj, int val) {
    obj->field_0 = val;
    obj->field_4 = 0;
    obj->field_8 = 0;
    obj->field_c = 0;
    obj->field_d = 1;
    obj->field_e = 0;
}

extern "C" void __dt__802534B0() {}

extern "C" void func_802534F0() {}

extern "C" void func_80253794() {}

extern "C" void func_802537EC() {}

extern "C" void func_8025385C() {}

extern "C" void func_80253888() {}

extern "C" void func_802538B0() {}

extern "C" void func_80253904() {}

extern "C" void func_80253970() {}

extern "C" void func_80253A14() {}

extern "C" void func_80253A60() {}

extern "C" void func_80253AB0() {}

extern "C" void __dt__80253AFC() {}

extern "C" void func_80253B3C() {}

extern "C" bool func_80253EE8(u8* this_, u32 arg1, u32 arg2) {
    if (arg1 >= 6) return false;
    if (arg2 >= 5) return false;
    s8 idx = (s8)this_[1];
    u8 val = *(this_ + idx * 0x140 + arg1 * 0x34 + arg2 * 0xA + 0x16);
    return val == 2;
}

extern "C" void func_80253F3C() {}

extern "C" void func_80254040() {}

extern "C" void func_8025406C(unsigned char* obj) {
    obj[1] = obj[1] - 1;
    if ((signed char)obj[1] < 0) {
        obj[1] = obj[0] - 1;
    }
}

extern "C" void func_80254094() {}

extern "C" unsigned char func_802540DC(void* thisPtr) {
    signed char idx = *(signed char*)((char*)thisPtr + 1);
    unsigned char* base = (unsigned char*)thisPtr + idx * 320;
    return *(base + 4);
}

extern "C" void func_802540F4() {}

extern "C" unsigned char func_80254144(const unsigned char* ptr) {
    int idx = (signed char)ptr[1];
    return ptr[5 + idx * 0x140];
}

extern "C" void func_8025415C() {}

extern "C" void func_8025418C() {}

extern "C" void func_802541BC() {}

extern "C" u16 func_80254204(u8* p1, u32 arg2, u32 arg3) {
    if (arg2 >= 6) return 0;
    if (arg3 >= 5) return 0;
    s8 idx = (s8)p1[1];
    u8* ptr = p1 + idx * 0x140 + arg2 * 0x34 + arg3 * 0xa;
    return *(u16*)(ptr + 0x12);
}

extern "C" void func_8025424C() {}

extern "C" void func_802542B8() {}

extern "C" void func_802542D0() {}

extern "C" void func_80254350() {}

extern "C" void func_8025440C() {}

extern "C" void func_80254484() {}

extern "C" void func_8025449C() {}

extern "C" void func_802545C0() {}

extern "C" void __ct__CCollepedia() {}

extern "C" void __dt__11CCollepediaFv() {}

extern "C" void func_8025492C() {}

extern "C" void func_80254A20() {}

extern "C" void func_80254B64() {}

extern "C" void func_80254C04() {}

extern "C" void func_80254D0C() {}

extern "C" unsigned char func_80254D50(void* self) { unsigned char* base = (unsigned char*)self; return base[0x28f9] ? base[0x51] : 0; }

extern "C" void func_80254D6C(void* p) { unsigned char* b = (unsigned char*)p; if (b[0x49] != 0) return; b[0x49] = 1; b[0x51] = 0; }

extern "C" void func_80254D8C() {}

extern "C" void func_80254E64() {}

extern "C" void func_80254F2C() {}

extern "C" void func_80255000() {}

extern "C" void func_802550B4() {}

extern "C" void func_8025516C() {}

extern "C" void func_80255210() {}

extern "C" void func_802552B4() {}

extern "C" void func_802553AC() {}

extern "C" void func_80255688(void* self) {
    func_80253EE8((u8*)self + 0xE8, *(u8*)((u8*)self + 0xD9), *(u8*)((u8*)self + 0xD8));
}

extern "C" void func_80255698() {}

extern "C" void func_802556DC() {}

extern "C" void func_80255748() {}

extern "C" void func_802557E0() {}

extern "C" void func_80255894() {}

extern "C" void func_8025592C() {}

extern "C" void func_80255984() {}

extern "C" void func_802559DC() {}

extern "C" void func_80255AB4() {}

extern "C" void func_80255B60() {}

extern "C" void func_80255C28() {}

extern "C" void func_80255CC0() {}

extern "C" void func_80255D3C() {}

extern "C" void func_80255E90() {}

extern "C" void func_80255F98() {}

extern "C" void func_8025629C() {}

extern "C" void func_80256314() {}

extern "C" void func_8025641C() {}

extern "C" void OnFileEvent__11CCollepediaFP10CEventFile() {}
