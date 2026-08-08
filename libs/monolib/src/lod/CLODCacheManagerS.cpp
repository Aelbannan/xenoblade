// Auto-scaffolded catalog TU for monolib/src/lod/CLODCacheManagerS
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "monolib/lod/CLODCacheManagerS.hpp"
#include "monolib/lod/LODMemMan.hpp"

namespace LOD {
struct CLODCacheManagerS {
    void func_804630C0();
    void func_80463118();
    void func_8046323C();
    void func_8046339C();
    void func_80463590();
};
struct UnkClass_8046368C {
    void func_8046368C() const;
    void func_8046369C() const;
    void func_804636AC() const;
    void func_8046374C() const;
    void func_8046375C() const;
};
}

using namespace LOD;

extern "C" void func_804630C0__Q23LOD17CLODCacheManagerSFv(void* self)
{
    struct Layout {
        unsigned int p00;
        unsigned int unused04;
        unsigned int p08;
        unsigned int unused0C;
        unsigned int p10;
        unsigned int unused14;
        unsigned int p18;
        unsigned int unused1C;
        unsigned int p20;
        unsigned int unused24;
        unsigned int p28;
        unsigned int unused2C;
        unsigned int p30;
    };

    Layout* cache = static_cast<Layout*>(self);
    lbl_eu_80665738 = static_cast<char*>(self) + cache->p00;
    lbl_eu_8066573C = static_cast<char*>(self) + cache->p08;
    lbl_eu_8066574C = static_cast<char*>(self) + cache->p10;
    lbl_eu_80665750 = static_cast<char*>(self) + cache->p18;
    lbl_eu_80665740 = static_cast<char*>(self) + cache->p20;
    lbl_eu_80665748 = static_cast<char*>(self) + cache->p30;
    lbl_eu_80665744 = static_cast<char*>(self) + cache->p28;
}

void LOD::CLODCacheManagerS::func_80463118() {}

void LOD::CLODCacheManagerS::func_8046323C() {}

void LOD::CLODCacheManagerS::func_8046339C() {}

void LOD::CLODCacheManagerS::func_80463590() {}

void LOD::UnkClass_8046368C::func_8046368C() const {}

void LOD::UnkClass_8046368C::func_8046369C() const {}

void LOD::UnkClass_8046368C::func_804636AC() const {}

void LOD::UnkClass_8046368C::func_8046374C() const {}

void LOD::UnkClass_8046368C::func_8046375C() const {}
