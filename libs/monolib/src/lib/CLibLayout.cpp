// Auto-scaffolded catalog TU for monolib/src/lib/CLibLayout
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "monolib/lib/CLibLayout.hpp"

extern "C" void __ct__10CLibLayoutFPCcP11CWorkThread() {}

extern "C" void* __dt__8045F000(void* self, int flags) {
    if (self != 0) {
        *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(self) + 0x48) = 0;
        *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(self) + 0x44) = 0;
        if (flags > 0)
            operator delete(self);
    }
    return self;
}

extern "C" void __dt__10CLibLayoutFv() {}

extern "C" void getAllocHandle__10CLibLayoutFv(void) {}

// Returns the global CLibLayout singleton instance.
CLibLayout* CLibLayout::getInstance() {
    extern CLibLayout* lbl_eu_80665710; // sbss singleton pointer
    return lbl_eu_80665710;
}

extern "C" void isInitialized__10CLibLayoutFv() {}

extern "C" void createLayout__10CLibLayoutFv() {}

extern "C" void createArcResourceAccessor__10CLibLayoutFv() {}

extern "C" void createPicture__10CLibLayoutFv() {}

extern "C" void createTextbox__10CLibLayoutFv() {}

extern "C" void deleteTextboxOrPicture__10CLibLayoutFv() {}

// No-op override; CLibLayout performs no per-frame work.
void CLibLayout::wkUpdate() {}

extern "C" void wkStandbyLogin__10CLibLayoutFv() {}

extern "C" void wkStandbyLogout__10CLibLayoutFv() {}

extern "C" void func_8045F438__10CLibLayoutFv() {}

extern "C" void func_8045F4E4__10CLibLayoutFv() {}
