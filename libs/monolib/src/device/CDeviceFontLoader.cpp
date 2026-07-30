// CDeviceFontLoader - Font file loading work thread

#include <string.h>
#include <types.h>

#include "monolib/work.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/util/MemManager.hpp"

// Size: 0x210
class CDeviceFontLoader : public CWorkThread {
public:
    CDeviceFontLoader(const char* pName, CWorkThread* pParent);
    virtual ~CDeviceFontLoader();
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();
    virtual bool OnFileEvent(CEventFile* pEventFile);

    // 0x000-0x1C3: inherited from CWorkThread
    void* mSomeData;          // 0x1C4
    char mFileName[64];       // 0x1C8
    u32 mFileNameLen;         // 0x208
    CFileHandle* mFileHandle; // 0x20C
};


CDeviceFontLoader::CDeviceFontLoader(const char* pName, CWorkThread* pParent)
    : CWorkThread(pName, pParent, 0)
{
    mType = THREAD_CDEVICEFONTLOADER;
    mFileName[0] = '\0';
    mFileNameLen = 0;
    mFileHandle = nullptr;
}

CDeviceFontLoader::~CDeviceFontLoader() {
}

bool CDeviceFontLoader::wkStandbyLogin() {
    if (!CWorkSystemPack::func_804DE08C()) {
        return false;
    }

    CDeviceFile* pDevFile = CDeviceFile::getInstance();
    if (pDevFile == nullptr || !CDeviceFile::isInitialized()) {
        return false;
    }

    mtl::ALLOC_HANDLE allocHandle = mtl::MemManager::getHandleMEM1();
    mFileHandle = CDeviceFile::readFile(allocHandle, mFileName, this, 0, 0);
    CDeviceFile::func_8044F154(mFileHandle, 0);
    CDeviceFile::setHandleFlag1(mFileHandle);

    return CWorkThread::wkStandbyLogin();
}

bool CDeviceFontLoader::wkStandbyLogout() {
    if (mFileHandle != nullptr) {
        CDeviceFile::cancel(mFileHandle);
        mFileHandle = nullptr;
    }

    if (mChildren.empty()) {
        return CWorkThread::wkStandbyLogout();
    }

    return false;
}

bool CDeviceFontLoader::OnFileEvent(CEventFile* pEventFile) {
    if (pEventFile->mFileHandle == mFileHandle) {
        if (pEventFile->unk0 == 1) {
            void* pData = mFileHandle->mData;
            mFileHandle->mData = nullptr;
            wkSetEvent(EVT_NONE);
        }
        mFileHandle = nullptr;
    }
    return true;
}
