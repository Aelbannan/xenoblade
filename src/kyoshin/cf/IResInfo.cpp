// Auto-scaffolded catalog TU for kyoshin/cf/IResInfo
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/IResInfo.hpp"

void func_80065F24(){}

void func_eu_80066938(){}

void func_eu_8006693C() {}

void func_eu_80066940(){}

void func_80065FB4(){}

void func_8006611C(){}

void func_80066160(){}

void func_80066184(){}

void func_800661A8(){}

void func_8006626C(){}

void func_80066290(){}

u32 getResEntryId(int unused, const ResEntry* p)
{
    if (p != 0) {
        return p->id;
    }
    return 0;
}

void func_8006638C(){}

void func_800663D8(){}

void func_80066424(){}

void func_8006646C(){}

void func_800664B8(){}

void func_8006650C(){}

void func_80066554(){}

void func_800665A0(){}

void func_800665F4(){}

void func_8006660C(){}

void func_80066714(){}

void func_80066788(){}

unsigned int lbl_eu_80663E28;

bool testResInfoFlag(u32 flags) {
    return (lbl_eu_80663E28 & flags) != 0;
}

void func_80066C74(){}

void func_80066CF8(){}

void func_80066DAC(){}

void func_80066E7C(){}

void __ct__80066F9C(){}

void __dt___reslist_base_unsigned_short(){}

void __dt__reslist_unsigned_short(){}

void __dt__8006754C(){}

void __dt__80067670(){}

void func_800676F8(){}

void func_80067D38(){}

void func_80067DB4(){}

void func_80067E78(){}

void func_80067F10(){}

void func_80067FE0(){}

void func_80068078(){}

void func_80068110(){}

void func_80068254(){}

void func_80068358(){}

bool testWordFlag(const u32* p, u32 mask) {
    return (*p & mask) != 0;
}

void clearWordFlag(u32* self, u32 mask) {
    *self &= ~mask;
}

char* getEntryPtr(char* base, int a, int b) {
    return base + (a + b + 0x59) * 0x3C + 4;
}

void func_8006846C(){}

char* getEntryPtrGrid(char* self, int a, int b) {
    return self + (b + a * 11 + 12) * 60 + 4;
}

void func_80068494(){}

void func_80068564(){}

void func_800685BC(){}

void func_800685C8(){}

void func_8006861C(){}

u8* ResContainer::findResEntry(u32 id, u32* outIndex, u32* outValue) {
    ResEntry* entry = table;
    u32 index = 0x59;
    *outIndex = 0;
    *outValue = (u32)-1;
    if (id == 0)
        return 0;
    for (u32 i = 0; i < 0x28; i++, index++, entry++) {
        if (entry->id == id) {
            *outIndex = index;
            *outValue = entry->value;
            return entry->data;
        }
    }
    return 0;
}

void func_800686E4(){}

void func_80068928(){}

bool isResFlag0() { return false; }

bool func_80068998(){ return false; }

void resNoop() {}

bool isResFlag1() { return false; }

void func_800689AC(){}

bool func_800689B8(){ return false; }

bool isResFlag2() { return false; }

bool func_800689C8(){ return false; }

bool isResFlag3() { return false; }

bool isResFlag4() { return false; }

bool func_eu_800693E8(){ return false; }

bool func_800689E0(){ return false; }

bool func_800689E8(){ return false; }

bool func_800689F0(){ return false; }

bool func_800689F8(){ return false; }

bool func_80068A00(){ return false; }

bool func_80068A08(){ return false; }

bool func_80068A10(){ return false; }

extern "C" int getResMaxCount(void* self) { return 512; }
