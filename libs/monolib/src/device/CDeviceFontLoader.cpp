// CDeviceFontLoader - Font file loading work thread

#include <string.h>
#include <types.h>

#include "monolib/work.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/util/MemManager.hpp"

// Forward-declare CDeviceFont with just the function we need
class CDeviceFont {
public:
    void func_80452D80();
};

// Size: 0x210
class CDeviceFontLoader : public CWorkThread {
public:
    CDeviceFontLoader(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, 0) {
        mType = THREAD_CDEVICEFONTLOADER;
        mFileName[0] = '\0';
        mFileNameLen = 0;
        mFileHandle = nullptr;
    }

    virtual ~CDeviceFontLoader();
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();
    virtual bool OnFileEvent(CEventFile* pEventFile);
    virtual void func_80454F30(void* arg1, const char* pPath);

    // 0x000-0x1C3: inherited from CWorkThread
    void* mSomeData;          // 0x1C4
    char mFileName[64];       // 0x1C8
    u32 mFileNameLen;         // 0x208
    CFileHandle* mFileHandle; // 0x20C
};


CDeviceFontLoader::~CDeviceFontLoader() {
}

void CDeviceFontLoader::func_80454F30(void* arg1, const char* pPath) {
    mSomeData = arg1;
    mFileNameLen = strlen(pPath);
    strcpy(mFileName, pPath);
}

bool CDeviceFontLoader::wkStandbyLogin() {
    if (!CWorkSystemPack::func_804DE08C()) {
        return false;
    }

    if (CDeviceFile::getInstance() == nullptr) {
        goto fail;
    }

    if (!CDeviceFile::isInitialized()) {
        goto fail;
    }

    mFileHandle = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM1(),
        mFileName, this, 0, 0);
    CDeviceFile::func_8044F154(mFileHandle, 0);
    CDeviceFile::setHandleFlag1(mFileHandle);
    return CWorkThread::wkStandbyLogin();

fail:
    return false;
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
    if (pEventFile->mFileHandle != mFileHandle) {
        goto skip;
    }

    if (pEventFile->unk0 == 1) {
        // Volatile dead load to match retail (1 reg-swap: r0 vs r4)
        volatile CFileHandle* vfh = mFileHandle;
        vfh->mData;
        mFileHandle->mData = nullptr;
        reinterpret_cast<CDeviceFont*>(mSomeData)->func_80452D80();
        wkSetEvent(EVT_NONE);
    }

    mFileHandle = nullptr;

skip:
    return true;
}
