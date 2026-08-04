// Auto-scaffolded catalog TU for monolib/src/work/CWorkSystemCache
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

struct CWorkSystemCache {
    CWorkSystemCache();
    ~CWorkSystemCache();
    void wkStandbyLogin();
    void wkStandbyLogout();
    void wkUpdate();
};


CWorkSystemCache::CWorkSystemCache() {}

void __dt___reslist_base_CCacheItem(){}

void func_804D8EC8(){}

void __dt__reslist_CCacheItem(){}

CWorkSystemCache::~CWorkSystemCache() {}

void func_804D8FB4(){}

void func_804D8FDC(){}

void func_804D903C(){}

void CWorkSystemCache::wkUpdate() {}

void CWorkSystemCache::wkStandbyLogin(void) {}

void CWorkSystemCache::wkStandbyLogout() {}

extern "C" void func_804D91BC(void* self) {
    if (*(u8*)((u8*)self + 0x48) == 0) {
        *(u32*)((u8*)self + 0x44) -= 1;
    }
}

void func_804D91D8(){}

void func_804D920C(){}
