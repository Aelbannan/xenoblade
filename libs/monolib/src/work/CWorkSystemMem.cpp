#include "monolib/work.hpp"

CWorkSystemMem::CWorkSystemMem(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, 1) {
    // novtable: write the retail vptr (0x8056BAA8) + mHandle = -1 by hand so
    // the stores land in retail order (vptr first, then mHandle).
    *(void**)this = lbl_eu_8056BAA8;
    mHandle = mtl::INVALID_HANDLE;
    lbl_eu_80665620 = this;
    mHandle = mtl::MemManager::create(mtl::MemManager::getHandleMEM2(), REGION_SIZE, mName.c_str());
}

CWorkSystemMem::~CWorkSystemMem(){
    lbl_eu_80665620 = nullptr;
}

mtl::ALLOC_HANDLE CWorkSystemMem::getHandle(){
    return lbl_eu_80665620->mHandle;
}

bool CWorkSystemMem::wkStandbyLogin(){
    return CWorkThread::wkStandbyLogin();
}

bool CWorkSystemMem::wkStandbyLogout(){
    if(mChildren.empty()){
        if(mParent->hasSingleChild() && mParent->wkGetChild() == this){
            mtl::MemManager::erase(mHandle);
            mHandle = mtl::INVALID_HANDLE;
            return CWorkThread::wkStandbyLogout();
        }
    }

    return false;
}
