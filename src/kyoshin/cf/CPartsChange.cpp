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

struct PartsChangeIf {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void vf30(u8 a, u32 b);
    virtual void _v034(); virtual int vf34();
};
extern "C" void func_80192F94(u8* self) {
    if (!((u8*)self)[0x2c]) return;
    if (!*(void**)((u8*)self + 8)) return;
    if (((u8*)self)[4]) return;
    int hit = 0;
    for (int i = 0; i < 0x20; i++) {
        u8 c = ((u8*)self)[0xc + i];
        if (c == 0xff) continue;
        if (hit) {
            ((PartsChangeIf*)*(void**)((u8*)self + 8))->vf30(c, 0);
        } else {
            if (((PartsChangeIf*)*(void**)((u8*)self + 8))->vf34()) hit = 1;
        }
    }
    if (hit) return;
    for (int i = 0; i < 0x20; i++) {
        if (((u8*)self)[0xc + i] != 0xff) {
            ((PartsChangeIf*)*(void**)((u8*)self + 8))->vf30(((u8*)self)[0xc + i], 1);
            return;
        }
    }
}

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

void CfActorAccessors::func_80193A88(int enable) { if (enable) mFlags1E |= 0x400; else mFlags1E &= ~0x400; }

void func_80193AB0(){}

u32 CfActorAccessors::func_80193B04() { return mField94; }

void func_80193B0C(){}

void func_80193C74(){}

u16 CfActorAccessors::func_80193CC8() { return mField9E; }

void func_80193CD0(){}

void func_80193D48(){}

void func_80194264(){}

void func_801943E4(){}

void func_80194610(){}

void func_801949E0(){}

void func_80194AFC(){}

void func_80194D5C(){}

void func_8019514C(){}

u32 CfActorAccessors::func_80195284() { return (mFlags1E >> 10) & 0x1u; }

void func_80195290(){}

void func_801952CC(){}

void func_80195384(){}

void func_801953E8(){}

u32 CfActorAccessors::func_80195AC0() { return (mFlags1E >> 3) & 0x1u; }

void CfActorAccessors::func_80195ACC(float val) { mField8C = val; }

void CfObjectPcExt::func_80195AD4(u16 val) { mField45C4 = val; }

void CfObjectPcExt::func_80195ADC(u16 val) { mField45C8 = val; }

u16 CfObjectPcExt::func_80195AE4() { return mField45C6; }

void* CfActorAccessors::func_80195AEC() { return &mField30; }

void* CfObjectPcExt::func_80195AF4() { return (void*)((u8*)this + 0x60c); }

void CfPartyInfo::func_80195AFC(u8 val) { field_2D = val; }

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

void CfPartyInfo::func_80198710(void* r4, float f1, int r5, int r6, float f2, float f3) {
    int r8 = *(int*)((char*)r4 + 0);
    int r7 = *(int*)((char*)r4 + 4);
    int r0 = *(int*)((char*)r4 + 8);
    field_00 = r8;
    field_04 = r7;
    field_08 = r0;
    field_18 = f1;
    field_0C = r5;
    field_14 = r6;
    field_1C = f2;
    field_20 = f3;
    if (r6 <= 0) {
        field_14 = 1;
    }
    extern float lbl_eu_80667B28;
    field_28 = lbl_eu_80667B28;
    field_2C = 0;
    field_2D = 1;
    field_2E = 0;
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
