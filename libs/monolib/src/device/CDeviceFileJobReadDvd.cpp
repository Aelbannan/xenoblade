#include <types.h>
#include "monolib/work/CWorkThread.hpp"
#include "monolib/device/CDeviceFileJob.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CDeviceFileDvd.hpp"
#include "monolib/core/CRsrc.hpp"
#include "monolib/util.hpp"
#include <revolution/DVD.h>
#include "monolib/device/CDeviceFileCri.hpp"
#include "monolib/device/CDeviceFileJobReadDvd.hpp"

CDeviceFileJobReadDvd::CDeviceFileJobReadDvd(const char* pName, CWorkThread* pParent)
    : CDeviceFileJob(pName, pParent) {
    mType = CWorkThread::THREAD_CDEVICEFILEJOBREADDVD;
}

CDeviceFileJobReadDvd::~CDeviceFileJobReadDvd() {}

bool CDeviceFileJobReadDvd::cancel(const char* pFilename) {
    if (mHandle == nullptr) return false;
    if (mHandle->mName != pFilename) return false;

    if (!wkIsCurrent()) {
        CDeviceFile::removeFileJob(this);
    } else if (CDeviceFile::func_8044E768()) {
        CDeviceFileCri::abortRead();
    } else {
        CDeviceFileDvd::cancelCurrent();
    }
    return true;
}

bool CDeviceFileJobReadDvd::cancel(CFileHandle* pHandle) {
    if (mHandle == nullptr || mHandle != pHandle) return false;

    if (!wkIsCurrent()) {
        CDeviceFile::removeFileJob(this);
    } else if (CDeviceFile::func_8044E768()) {
        CDeviceFileCri::abortRead();
    } else {
        CDeviceFileDvd::cancelCurrent();
    }
    return true;
}

void CDeviceFileJobReadDvd::wkUpdate() {
    if (mHandle != nullptr && mHandle->unkInline2()) {
        if (mHandle->getRsrc() != nullptr) {
            CRsrc::entry(mHandle->getRsrc(), mHandle->mName.c_str(), &mHandle->unk160,
                         mHandle->getData(), mHandle->getLength(), false);
        }

        mHandle->call(CBM_1);
        CDeviceFile::removeFileJob(this);
    }
}

bool CDeviceFileJobReadDvd::wkStandbyLogin() {
    if (isNoEvent()) {
        return CWorkThread::wkStandbyLogin();
    } else if (wkIsCurrent()) {
        if (mHandle->checkExistRsrc(CBM_1)) {
            CDeviceFile::removeFileJob(this);
        }
        return CWorkThread::wkStandbyLogin();
    }
    return false;
}

bool CDeviceFileJobReadDvd::wkStandbyLogout() {
    if (mHandle != nullptr) {
        cancel(mHandle);
        return false;
    }
    return CWorkThread::wkStandbyLogout();
}

void CDeviceFileJobReadDvd::callCBM3() {
    if (mHandle != nullptr) {
        mHandle->call(CBM_3);
    }
    CDeviceFile::removeFileJob(this);
}

// name-guess: retail font-path literals pooled in this TU's rodata; the
// functions that used them were dead-stripped from the linked game.
void CDeviceFileJobReadDvd_setEuFontPath() {
    const char* path = "font/font_eu5.brfna";
    CDeviceFile::getFileSize(path);
}

// name-guess: see above (font/MenuFont.brfna).
void CDeviceFileJobReadDvd_setMenuFontPath() {
    const char* path = "font/MenuFont.brfna";
    CDeviceFile::getFileSize(path);
}
