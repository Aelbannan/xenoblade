// Auto-scaffolded catalog TU for monolib/src/lib/CLibLayout
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "monolib/lib/CLibLayout.hpp"

CLibLayout::CLibLayout(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, 0) {}

extern "C" void* __dt__8045F000(void* self, int flags) {
    if (self != 0) {
        *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(self) + 0x48) = 0;
        *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(self) + 0x44) = 0;
        if (flags > 0)
            operator delete(self);
    }
    return self;
}

CLibLayout::~CLibLayout() {}

void CLibLayout::getAllocHandle(void) {}

// Returns the global CLibLayout singleton instance.
CLibLayout* CLibLayout::getInstance() {
    extern CLibLayout* lbl_eu_80665710; // sbss singleton pointer
    return lbl_eu_80665710;
}

bool CLibLayout::isInitialized() { return false; }

void CLibLayout::createLayout() {}

nw4r::lyt::ArcResourceAccessor* CLibLayout::createArcResourceAccessor() { return 0; }

void CLibLayout::createPicture() {}

void CLibLayout::createTextbox() {}

void CLibLayout::deleteTextboxOrPicture() {}

// No-op override; CLibLayout performs no per-frame work.
void CLibLayout::wkUpdate() {}

bool CLibLayout::wkStandbyLogin() { return true; }

bool CLibLayout::wkStandbyLogout() { return true; }

void CLibLayout::func_8045F438() {}

void CLibLayout::func_8045F4E4() {}
