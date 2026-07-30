#include "monolib/lib/CLibG3d.hpp"

CLibG3d::CLibG3d(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent) {}

CLibG3d::~CLibG3d() {}

CLibG3d* CLibG3d::getInstance() {
    extern CLibG3d* lbl_eu_806656F0;
    return lbl_eu_806656F0;
}

void CLibG3d::isInitialized() {}

void CLibG3d::wkStandbyLogin() {}

void CLibG3d::wkStandbyLogout() {}
