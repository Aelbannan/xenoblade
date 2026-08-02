// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_scnmdlexpand
// Replace stubs with high-level C/C++ during decomp.

#include <nw4r/g3d.h>

// lbl_* are the ONLY symbols allowed with extern "C".
extern "C" const char lbl_eu_8051D7D0[];
extern "C" const char lbl_eu_8051D788[];
extern "C" const char lbl_eu_8051D768[];
extern "C" const char lbl_eu_8051D640[];

// Concrete type forward for Remove parameter.
namespace nw4r { namespace g3d { class ScnObj; } }

// Target 1: GetTypeObj
extern "C" const char* GetTypeObj__Q34nw4r3g3d12ScnMdlExpandCFv() {
    return (const char*)lbl_eu_8051D7D0;
}

// Target 3: IsDerivedFrom
bool IsDerivedFrom__Q34nw4r3g3d12ScnMdlExpandCFQ44nw4r3g3d6G3dObj7TypeObj(
    const unsigned char* /* self */, const unsigned char* pOther) {
    unsigned int id = *reinterpret_cast<const unsigned int*>(pOther);
    if (id == reinterpret_cast<unsigned int>(lbl_eu_8051D7D0)) return true;
    if (id == reinterpret_cast<unsigned int>(lbl_eu_8051D788)) return true;
    if (id == reinterpret_cast<unsigned int>(lbl_eu_8051D768)) return true;
    const unsigned int baseAddr = reinterpret_cast<unsigned int>(lbl_eu_8051D640);
    const unsigned int diff = baseAddr - id;
    const unsigned int c = __cntlzw(diff);
    return (c >> 5) != 0;
}

// Target 2: GetTypeName
const char* GetTypeName__Q34nw4r3g3d12ScnMdlExpandCFv(const unsigned char* self) {
    const unsigned char* const* vtbl = *reinterpret_cast<const unsigned char* const* const*>(self);
    const char* (*vfunc)(const unsigned char*) =
        reinterpret_cast<const char* (*)(const unsigned char*)>(vtbl[0x14 / sizeof(unsigned char*)]);
    const char* result = vfunc(self);
    return result + 4;
}

// Target 4: destructor
void __dt__Q34nw4r3g3d12ScnMdlExpandFv(unsigned char* self, int flags) {
    (void)self; (void)flags;
}

// Target 5: Remove
bool Remove__Q34nw4r3g3d12ScnMdlExpandFPQ34nw4r3g3d6ScnObj(unsigned char* self, nw4r::g3d::ScnObj* pObj) {
    (void)self; (void)pObj;
    return false;
}

// Remaining stubs
void Construct__Q34nw4r3g3d12ScnMdlExpandFP12MEMAllocatorPUlUlPQ34nw4r3g3d12ScnMdlSimple() {}
void PushBack__Q34nw4r3g3d12ScnMdlExpandFPQ34nw4r3g3d6ScnObjUl() {}
void Insert__Q34nw4r3g3d12ScnMdlExpandFUlPQ34nw4r3g3d6ScnObj() {}
void Remove__Q34nw4r3g3d12ScnMdlExpandFUl() {}
void G3dProc__Q34nw4r3g3d12ScnMdlExpandFUlUlPv() {}