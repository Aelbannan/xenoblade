// Auto-scaffolded catalog TU for kyoshin/cf/CPartsChange
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__cf_CPartsChange() {}

extern "C" void __dt__Q22cf12CPartsChangeFv() {}

extern "C" void func_80192E80() {}

extern "C" void func_80192F94() {}

extern "C" void func_801930A0() {}

extern "C" void func_801931D0() {}

extern "C" void __ct__80193270() {}

extern "C" void __dt___reslist_base_cf_CfPartyInfo() {}

extern "C" void __dt__reslist_cf_CfPartyInfo() {}

extern "C" void __dt__80193538() {}

extern "C" bool func_80193670() { return false; }

extern "C" void func_80193678() {}

extern "C" void func_80193710() {}

extern "C" bool func_80193804() { return false; }

extern "C" void func_8019380C(void) {}

extern "C" void func_80193810() {}

extern "C" void func_8019397C() {}

extern "C" void func_80193A88(void* obj, int enable) { unsigned short* flag = (unsigned short*)((unsigned char*)obj + 0x1e); if (enable) *flag |= 0x400; else *flag &= ~0x400; }

extern "C" void func_80193AB0() {}

extern "C" u32 func_80193B04(void* self) { return *(u32*)((u8*)self + 0x94); }

extern "C" void func_80193B0C() {}

extern "C" void func_80193C74() {}

extern "C" u16 func_80193CC8(void* self) { return *(u16*)((u8*)self + 0x9E); }

extern "C" void func_80193CD0() {}

extern "C" void func_80193D48() {}

extern "C" void func_80194264() {}

extern "C" void func_801943E4() {}

extern "C" void func_80194610() {}

extern "C" void func_801949E0() {}

extern "C" void func_80194AFC() {}

extern "C" void func_80194D5C() {}

extern "C" void func_8019514C() {}

extern "C" u32 func_80195284(void* self) { return (*(u16*)((u8*)self + 30) >> 10) & 0x1u; }

extern "C" void func_80195290() {}

extern "C" void func_801952CC() {}

extern "C" void func_80195384() {}

extern "C" void func_801953E8() {}

extern "C" u32 func_80195AC0(void* self) { return (*(u16*)((u8*)self + 30) >> 3) & 0x1u; }

extern "C" void func_80195ACC(void* self, float val) { *(float*)((u8*)self + 0x8c) = val; }

extern "C" void func_80195AD4(void* self, u16 val) { *(u16*)((u8*)self + 0x45C4) = val; }

extern "C" void func_80195ADC(void* self, u16 val) { *(u16*)((u8*)self + 0x45C8) = val; }

extern "C" u16 func_80195AE4(void* self) { return *(u16*)((u8*)self + 0x45C6); }

extern "C" void* func_80195AEC(void* self) { return (void*)((u8*)self + 0x30); }

extern "C" void* func_80195AF4(void* self) { return (void*)((u8*)self + 0x60c); }

extern "C" void func_80195AFC(void* self, u8 val) { ((u8*)self)[0x2D] = val; }

extern "C" void func_80195B04() {}

extern "C" void func_80195BD4() {}

extern "C" void func_80195E5C() {}

extern "C" bool func_8019641C(const void* a, const void* b) {
    return *(const float*)((const char*)a + 4) < *(const float*)((const char*)b + 4);
}

extern "C" void func_80196434() {}

extern "C" void func_80196864() {}

extern "C" void func_80196C94() {}

extern "C" void func_80196E04() {}

extern "C" void func_8019747C() {}

extern "C" void func_801974CC() {}

extern "C" void func_80197538() {}

extern "C" void func_80197AA0() {}

extern "C" void func_80197B28() {}

extern "C" void func_80197B4C() {}

extern "C" void func_80197BA4() {}

extern "C" void func_80197C6C() {}

extern "C" void func_80197DE8() {}

extern "C" u32 lbl_eu_8066430C;
extern "C" void func_80198108(int arg) {
    if (lbl_eu_8066430C == 0) return;
    unsigned char* p = (unsigned char*)lbl_eu_8066430C + 0x10000;
    *(unsigned short*)(p - 0x4d88) = (unsigned short)arg;
    if (arg == 0) return;
    unsigned char* p2 = (unsigned char*)lbl_eu_8066430C + 0x10000;
    *(unsigned short*)(p2 - 0x4d8a) = 0;
}

extern "C" void func_80198138() {}

extern "C" void func_80198284() {}

extern "C" void func_80198310() {}

extern "C" void func_80198340() {}

extern "C" void func_80198400() {}

extern "C" void* func_801984E4(void* self, unsigned long idx) {
    return *(void**)((char*)self + (idx << 3));
}

extern "C" void func_801984F0() {}

extern "C" void func_80198524() {}

extern "C" void func_801986CC() {}

extern "C" void func_80198710(void *r3, void *r4, float f1, int r5, int r6, float f2, float f3) {
    int r8 = *(int*)((char*)r4 + 0);
    int r7 = *(int*)((char*)r4 + 4);
    int r0 = *(int*)((char*)r4 + 8);
    *(int*)((char*)r3 + 0) = r8;
    *(int*)((char*)r3 + 4) = r7;
    *(int*)((char*)r3 + 8) = r0;
    *(float*)((char*)r3 + 0x18) = f1;
    *(int*)((char*)r3 + 0xc) = r5;
    *(int*)((char*)r3 + 0x14) = r6;
    *(float*)((char*)r3 + 0x1c) = f2;
    *(float*)((char*)r3 + 0x20) = f3;
    if (r6 <= 0) {
        *(int*)((char*)r3 + 0x14) = 1;
    }
    extern float lbl_eu_80667B28;
    *(float*)((char*)r3 + 0x28) = lbl_eu_80667B28;
    *(char*)((char*)r3 + 0x2c) = 0;
    *(char*)((char*)r3 + 0x2d) = 1;
    *(char*)((char*)r3 + 0x2e) = 0;
}

extern "C" void func_8019876C() {}

extern "C" void func_801987A4() {}

extern "C" void func_80198AC4() {}

extern "C" void func_80198AE0() {}

extern "C" void func_80198C24() {}

extern "C" void func_80198D44() {}

extern "C" void func_80198E0C() {}

extern "C" void func_80198EE8() {}

extern "C" void func_80198FC4() {}

extern "C" void func_801990F0() {}

extern "C" void func_8019922C() {}
