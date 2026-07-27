// Auto-scaffolded catalog TU for kyoshin/cf/CPartsChange
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CPartsChange.hpp"

using cf::CfPartyInfo;
using cf::CfPartyInfoSortKey;
using cf::CfActorAccessors;
using cf::CfObjectPcExt;

void __ct__cf_CPartsChange(){}

cf::CPartsChange::~CPartsChange() {}

void func_80192E80(){}

void func_80192F94(){}

void func_801930A0(){}

void func_801931D0(){}

void __ct__80193270(){}

void __dt___reslist_base_cf_CfPartyInfo(){}

void __dt__reslist_cf_CfPartyInfo(){}

void __dt__80193538(){}

bool func_80193670() { return false; }

void func_80193678(){}

void func_80193710(){}

bool func_80193804() { return false; }

void func_8019380C(void){}

void func_80193810(){}

void func_8019397C(){}

void func_80193A88(CfActorAccessors* obj, int enable) { if (enable) obj->mFlags1E |= 0x400; else obj->mFlags1E &= ~0x400; }

void func_80193AB0(){}

u32 func_80193B04(CfActorAccessors* self) { return self->mField94; }

void func_80193B0C(){}

void func_80193C74(){}

u16 func_80193CC8(CfActorAccessors* self) { return self->mField9E; }

void func_80193CD0(){}

void func_80193D48(){}

void func_80194264(){}

void func_801943E4(){}

void func_80194610(){}

void func_801949E0(){}

void func_80194AFC(){}

void func_80194D5C(){}

void func_8019514C(){}

u32 func_80195284(CfActorAccessors* self) { return (self->mFlags1E >> 10) & 0x1u; }

void func_80195290(){}

void func_801952CC(){}

void func_80195384(){}

void func_801953E8(){}

u32 func_80195AC0(CfActorAccessors* self) { return (self->mFlags1E >> 3) & 0x1u; }

void func_80195ACC(CfActorAccessors* self, float val) { self->mField8C = val; }

void func_80195AD4(CfObjectPcExt* self, u16 val) { self->mField45C4 = val; }

void func_80195ADC(CfObjectPcExt* self, u16 val) { self->mField45C8 = val; }

u16 func_80195AE4(CfObjectPcExt* self) { return self->mField45C6; }

void* func_80195AEC(CfActorAccessors* self) { return &self->mField30; }

void* func_80195AF4(CfObjectPcExt* self) { return (void*)((u8*)self + 0x60c); }

void func_80195AFC(CfPartyInfo* self, u8 val) { self->field_2D = val; }

void func_80195B04(){}

void func_80195BD4(){}

void func_80195E5C(){}

bool func_8019641C(const CfPartyInfoSortKey* a, const CfPartyInfoSortKey* b) {
    return a->sortKey < b->sortKey;
}

void func_80196434(){}

void func_80196864(){}

void func_80196C94(){}

void func_80196E04(){}

void func_8019747C(){}

void func_801974CC(){}

void func_80197538(){}

void func_80197AA0(){}

void func_80197B28(){}

void func_80197B4C(){}

void func_80197BA4(){}

void func_80197C6C(){}

void func_80197DE8(){}

u32 lbl_eu_8066430C;
void func_80198108(int arg) {
    if (lbl_eu_8066430C == 0) return;
    unsigned char* p = (unsigned char*)lbl_eu_8066430C + 0x10000;
    *(unsigned short*)(p - 0x4d88) = (unsigned short)arg;
    if (arg == 0) return;
    unsigned char* p2 = (unsigned char*)lbl_eu_8066430C + 0x10000;
    *(unsigned short*)(p2 - 0x4d8a) = 0;
}

void func_80198138(){}

void func_80198284(){}

void func_80198310(){}

void func_80198340(){}

void func_80198400(){}

void* func_801984E4(void* self, unsigned long idx) {
    return *(void**)((char*)self + (idx << 3));
}

void func_801984F0(){}

void func_80198524(){}

void func_801986CC(){}

void func_80198710(void *r3, void *r4, float f1, int r5, int r6, float f2, float f3) {
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

void func_8019876C(){}

void func_801987A4(){}

void func_80198AC4(){}

void func_80198AE0(){}

void func_80198C24(){}

void func_80198D44(){}

void func_80198E0C(){}

void func_80198EE8(){}

void func_80198FC4(){}

void func_801990F0(){}

void func_8019922C(){}
