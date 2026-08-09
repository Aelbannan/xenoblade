// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectMap
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectMap.hpp"

struct CMIf {
    virtual void _v0008();
    virtual void _v000C();
    virtual void _v0010();
    virtual void _v0014();
    virtual void _v0018();
    virtual void _v001C();
    virtual void _v0020();
    virtual void _v0024();
    virtual void _v0028();
    virtual void _v002C();
    virtual void _v0030();
    virtual void _v0034();
    virtual void _v0038();
    virtual void _v003C();
    virtual void _v0040();
    virtual void _v0044();
    virtual void _v0048();
    virtual void _v004C();
    virtual void _v0050();
    virtual void _v0054();
    virtual void _v0058();
    virtual void _v005C();
    virtual void _v0060();
    virtual void _v0064();
    virtual void _v0068();
    virtual void _v006C();
    virtual void _v0070();
    virtual void _v0074();
    virtual void _v0078();
    virtual void _v007C();
    virtual void _v0080();
    virtual void _v0084();
    virtual void _v0088();
    virtual void _v008C();
    virtual void _v0090();
    virtual void _v0094();
    virtual void _v0098();
    virtual void _v009C();
    virtual void _v00A0();
    virtual void _v00A4();
    virtual void _v00A8();
    virtual void _v00AC();
    virtual void _v00B0();
    virtual void _v00B4();
    virtual void _v00B8();
    virtual void _v00BC();
    virtual void _v00C0();
    virtual void vf00C4();
    virtual void _v00C8();
    virtual void _v00CC();
    virtual void vf00D0();
};

void cf::CfObjectMap::func_800B9A70() {}

void cf::CfObjectMap::func_800B9AB4() {}

void cf::CfObjectMap::func_800B9B78() {}

void func_800B9C14(){}

void func_800B9C64__Q22cf11CfObjectMapFv(void* self) {
    *(unsigned short*)((char*)self + 0x8e) = 0;
}

void func_800B9C70() {}

void func_800B9C74(){}

void cf::CfObjectMap::func_800B9E3C(unsigned long v) {
    extern void func_800B9C74(void*, unsigned long, unsigned long);
    func_800B9C74(this, (v >> 5) & 0x7F, (v >> 12) & 0x3FF);
}

void func_800B9E4C(){}

void func_800BA440(){}

void cf::CfObjectMap::func_800BA610() {}

extern "C" void func_800BA650__Q22cf11CfObjectMapFv(cf::CfObjectMap* self, float f) {
    extern void func_80484E5C(void*);
    void* unk = *(void**)((char*)self + 0x98);
    *(float*)((char*)self + 0x2F48) = f;
    if (unk != 0) {
        func_80484E5C(unk);
    }
}

void cf::CfObjectMap::func_800BA66C() {}

extern "C" void CfObjectModel_UnkVirtualFunc19__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, int arg) {
    unsigned int* field = (unsigned int*)((char*)self + 0x68);
    if (arg != 0)
        *field |= 0x200000;
    else
        *field &= ~0x200000;
}

extern "C" int CfObjectModel_UnkVirtualFunc5__Q22cf13CfObjectModelFv(cf::CfObjectModel* self) { return 0; }

extern "C" int CfObjectModel_UnkVirtualFunc4__Q22cf13CfObjectModelFv(cf::CfObjectModel* self) { return 0; }



extern "C" void func_800BA764__Q22cf11CfObjectMapFv() {}

cf::CfObjectMap::~CfObjectMap() {}

extern "C" void CfObject_UnkVirtualFunc33__Q22cf13CfObjectModelFv(cf::CfObjectModel* self) { reinterpret_cast<CMIf*>(self)->vf00D0(); }

extern "C" void CfObject_UnkVirtualFunc30__Q22cf13CfObjectModelFv(cf::CfObjectModel* self) { reinterpret_cast<CMIf*>(self)->vf00C4(); }
