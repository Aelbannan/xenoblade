#include "monolib/lib/CLibG3d.hpp"

CLibG3d::CLibG3d(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, 0) {}

CLibG3d::~CLibG3d() {}

CLibG3d* CLibG3d::getInstance() {
    extern CLibG3d* lbl_eu_806656F0;
    return lbl_eu_806656F0;
}

bool CLibG3d::isInitialized() { return false; }

bool CLibG3d::wkStandbyLogin() { return true; }

bool CLibG3d::wkStandbyLogout() { return true; }
