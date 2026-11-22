// CDeviceFontLoader - Font file loading work thread

#include <string.h>
#include <types.h>

#include "monolib/work.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CDeviceFontLoader.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/util/MemManager.hpp"

// CDeviceFont::createFontInfo - Fv mangling, but the retail caller passes the
// loader's font arg in r3 and the just-claimed file data in r4.
extern "C" void createFontInfo__11CDeviceFontFv(u32 arg, u8* pData);

CDeviceFontLoader::CDeviceFontLoader(const char* pName, CWorkThread* pParent)
    : CWorkThread(pName, pParent, 0) {
    mFileName[0] = '\0';
    mFileNameLen = 0;
    mFileHandle = nullptr;
    mType = CWorkThread::THREAD_CDEVICEFONTLOADER;
}

CDeviceFontLoader::~CDeviceFontLoader() {}

void setFontPath__17CDeviceFontLoaderFv(CDeviceFontLoader* self, u32 arg, const char* pPath) {
    self->mFontArg = arg;
    self->mFileNameLen = strlen(pPath);
    strcpy(self->mFileName, pPath);
}

bool CDeviceFontLoader::wkStandbyLogin() {
    if (!CWorkSystemPack::arePacksLoaded()) {
        return false;
    }

    if (CDeviceFile::getInstance() != nullptr && CDeviceFile::isInitialized()) {
        mFileHandle = CDeviceFile::readFile(mtl::MemManager::getHandleMEM1(), mFileName, this, 0, 0);
        CDeviceFile::tryUpdateJobPriority(mFileHandle, 0);
        CDeviceFile::setHandleFlag1(mFileHandle);
        return CWorkThread::wkStandbyLogin();
    }

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
    if (pEventFile->mFileHandle == mFileHandle) {
        if (pEventFile->unk0 == 1) {
            u8* pData = mFileHandle->mData;
            mFileHandle->mData = nullptr;
            createFontInfo__11CDeviceFontFv(mFontArg, pData);
            wkSetEvent(EVT_NONE);
        }

        mFileHandle = nullptr;
    }

    return true;
}
