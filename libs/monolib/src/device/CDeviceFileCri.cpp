#include "monolib/device.hpp"
#include "monolib/core.hpp"
#include <revolution/ARC.h>
#include <revolution/DVD.h>
#include <string.h>

extern "C" {
    int func_804DDCD4(const char* pPath, const char* pFilename);
    void func_eu_804520D0(const char* pPath);
    bool func_eu_804521C4();
    extern wchar_t* lbl_eu_806636C8;
    extern wchar_t* lbl_eu_806636CC;
    extern wchar_t* lbl_eu_806636D0;
}

CDeviceFileCri* CDeviceFileCri::sInstance = nullptr;

static bool hasActiveCDeviceFileJobReadDvd() {
    CDeviceFileCri* inst = CDeviceFileCri::getInstance();
    if (inst == nullptr) return nullptr;
    reslist<CWorkThread*>& children = inst->mChildren;
    CWorkThread* node = children.front();
    while (node != children.end()) {
        CWorkThread* child = node->wkGetChild();
        if (child != nullptr && child->mType == 68) {
            return child;
        }
        node = node->wkGetChild();
    }
    return nullptr;
}

void CDeviceFileCri::closeADXFAndCleanup() {
    CDeviceFileCri* inst = getInstance();
    CWorkThread* node = inst->mChildren.front();
    while (node != inst->mChildren.end()) {
        CWorkThread* child = node->wkGetChild();
        if (child != nullptr && child->mType != 68) {
            child = nullptr;
        }
        if (child != nullptr) {
            break;
        }
        node = node->wkGetChild();
    }

    if (inst->mADXFHandle != nullptr) {
        ADXF_Stop(inst->mADXFHandle);
        ADXF_GetNumReqSct(inst->mADXFHandle);
        ADXF_Close(inst->mADXFHandle);
        inst->mADXFHandle = nullptr;
    }

    if (child != nullptr) {
        CDeviceFile::removeFileJob(static_cast<CDeviceFileJob*>(child));
    }
    inst->mState = 0;
}

CDeviceFileJobReadDvd* CDeviceFileCri::getFirstCDeviceFileJobReadDvd() {
    CWorkThread* child = mChildren.front()->wkGetChild();
    if (child == nullptr) return nullptr;
    if (child->mType != 68) return nullptr;
    return static_cast<CDeviceFileJobReadDvd*>(child);
}

CDeviceFileCri::CDeviceFileCri(const char* pName, CWorkThread* pParent, int capacity)
    : CWorkThread(pName, pParent, capacity) {
    *(u32*)this = (u32)&lbl_eu_8056C354;
    *(u32*)((char*)this + 0x1C4) = (u32)&lbl_eu_8056C354 + 0xA0;
    unk1D0 = 0;
    mActiveWorkID = -1;
    unk1D0 = 0;
    mADXFHandle = nullptr;
    mBuffer = nullptr;
    mIdleCounter = 0;
    mRetryCounter = 0;
    mExceptionPending = 0;
    mTimeoutCounter = 0;
    sInstance = this;
    mBuffer = CDevice::getDevSys2Handle()->allocate_head(0x800, 0x20);
    IException* exception = (this != nullptr) ?
        (IException*)((char*)this + 0x1C4) : nullptr;
    CException::getInstance()->func_804591BC(exception);
}

void CDeviceFileCri::func_8044F964() {
    CDeviceFileCri* inst = getInstance();
    if (inst != nullptr && inst->mADXFHandle != nullptr) {
        ADXF_Stop(inst->mADXFHandle);
        ADXF_Close(inst->mADXFHandle);
        inst->mADXFHandle = nullptr;
    }
}

int CDeviceFileCri::getFileSize(const char* pPath, int arg1) {
    int pathLen = strlen(pPath);
    char pathBuf[256];
    char strippedPath[256];
    strcpy(pathBuf, pPath);

    if (arg1 != 0) {
        func_eu_804520D0(pathBuf);
    }

    int strippedLen;
    if (pPath[0] == '/') {
        strippedLen = strlen(pPath + 1);
        strcpy(strippedPath, pPath + 1);
    } else {
        strippedLen = strlen(pPath);
        strcpy(strippedPath, pPath);
    }

    const wchar_t* baseDir = lbl_eu_806636C8 + 3;
    int baseLen = strlen((const char*)baseDir);

    int pos = 0;
    const char* search = strippedPath;
    while (pos < strippedLen) {
        if (strncmp(search, (const char*)baseDir, baseLen) == 0) {
            break;
        }
        search++;
        pos++;
    }

    if (pos != -1 && pos < strippedLen) {
        strippedPath[pos] = '\0';
        strippedLen = pos;
    }

    int result = func_804DDCD4(strippedPath, pPath);
    if (result > -1) {
        return result;
    }

    int entrynum = DVDConvertPathToEntrynum(pathBuf);
    if (entrynum < 0) {
        return result;
    }

    DVDFileInfo fileInfo;
    if (!DVDFastOpen(entrynum, &fileInfo)) {
        return result;
    }

    u32 fileSize = fileInfo.length;
    if (fileSize & 0x7FF) {
        fileSize = (fileSize + 0x800) & ~0x7FF;
    }

    DVDClose(&fileInfo);
    return fileSize;
}

void CDeviceFileCri::func_8044FB08(const char* pPath) {
    CDeviceFileCri* inst = getInstance();
    CWorkThread* child = inst->mChildren.front()->wkGetChild();
    while (child != inst->mChildren.end()) {
        CWorkThread* job = child->wkGetChild();
        if (job != nullptr) {
            if (job->mType != 68) {
                job = nullptr;
            }
        }
        if (job != nullptr) {
            CDeviceFileJobReadDvd* dvdJob = static_cast<CDeviceFileJobReadDvd*>(job);
            dvdJob->mHandle->call(CBM_3);
        }
        child = child->wkGetChild();
    }
}

bool CDeviceFileCri::cancel(CFileHandle* pHandle) {
    CDeviceFileCri* inst = getInstance();
    CWorkThread* child = inst->mChildren.front()->wkGetChild();
    while (child != inst->mChildren.end()) {
        CWorkThread* job = child->wkGetChild();
        if (job != nullptr) {
            if (job->mType != 68) {
                job = nullptr;
            }
        }
        if (job != nullptr) {
            CDeviceFileJobReadDvd* dvdJob = static_cast<CDeviceFileJobReadDvd*>(job);
            if (dvdJob->mHandle->call(CBM_3)) {
                return true;
            }
        }
        child = child->wkGetChild();
    }
    return false;
}

void CDeviceFileCri::func_8044FC38() {
    CDeviceFileCri* inst = getInstance();
    int count = 0;
    CWorkThread* child = inst->mChildren.front();
    while (child != inst->mChildren.end()) {
        count++;
        child = child->wkGetChild();
    }

    if (count == 0) return;

    CWorkThread* firstChild = inst->mChildren.front()->wkGetChild();
    CDeviceFileJobReadDvd* dvdJob = nullptr;
    if (firstChild != nullptr) {
        if (firstChild->mType == 68) {
            dvdJob = static_cast<CDeviceFileJobReadDvd*>(firstChild);
        }
    }

    if (inst->mADXFHandle != nullptr) {
        ADXF_Stop(inst->mADXFHandle);
        ADXF_GetNumReqSct(inst->mADXFHandle);
        ADXF_Close(inst->mADXFHandle);
        inst->mADXFHandle = nullptr;
    }

    if (dvdJob != nullptr) {
        CDeviceFile::removeFileJob(dvdJob);
    }
    inst->mState = 0;
}

bool CDeviceFileCri::func_8044FCFC() {
    CDeviceFileJobReadDvd* job = getFirstCDeviceFileJobReadDvd();
    if (job == nullptr) return false;

    CFileHandle* handle = job->mHandle;
    bool isOpen = ADXF_IsOpened(mADXFHandle);
    if (!isOpen) {
        if (ADXF_GetNumReqSct(mADXFHandle) == 4) {
            handle->call(CBM_3);
            closeADXFAndCleanup();
            return false;
        }
        return false;
    }

    u32 fileSize = ADXF_GetFsizeByte(mADXFHandle);
    u32 alignedSize = fileSize;
    if (fileSize & 0x7FF) {
        alignedSize = (fileSize + 0x800) & ~(0x7FF);
    }
    u32 remaining = alignedSize;

    ADXF_GetFsizeSct(mADXFHandle);
    if (remaining <= 0) {
        handle->call(CBM_3);
        closeADXFAndCleanup();
        return false;
    }

    u32 readSize = handle->mLength;
    if (readSize != 0) {
        remaining = readSize;
    }

    if (handle->unk10 + remaining > alignedSize) {
        remaining = remaining - (handle->unk10 + remaining - alignedSize);
    }

    handle->destroy(remaining, 0x20, 0x800);
    if (handle->unk58 & 0x8) {
        handle->call(CBM_3);
        closeADXFAndCleanup();
        return false;
    }

    mTimeoutCounter = 0;
    if (handle->unk10 != 0) {
        mState = 2;
        return true;
    }
    mState = 4;
    return false;
}

bool CDeviceFileCri::func_80450058() {
    CDeviceFileJobReadDvd* job = getFirstCDeviceFileJobReadDvd();
    int reqSct = ADXF_GetNumReqSct(mADXFHandle);
    if (reqSct - 1 <= 1) return false;

    if (reqSct == 3) {
        CFileHandle* handle = job->mHandle;
        u32 pos = handle->unk10;
        u32 len = handle->mLength;
        u32 remaining = len - (pos & 0x7FF);
        if (remaining > handle->mLength) {
            remaining = handle->mLength;
        }

        char* dst = (char*)mBuffer + (pos & 0x7FF);
        memcpy(dst, (char*)handle->mData + handle->unk10, remaining);
        DCFlushRangeNoSync((char*)handle->mData + handle->unk10, remaining);
        handle->func_80451CBC(remaining);

        if (handle->unk10 != 0 && handle->unk10 == handle->mLength) {
            ADXF_Stop(mADXFHandle);
            ADXF_Close(mADXFHandle);
            mADXFHandle = nullptr;
            mActiveWorkID = job->mWorkID;
            mState = 8;
            mIdleCounter = 0;
            return true;
        }
        return false;
    }

    CFileHandle* handle = job->mHandle;
    handle->call(CBM_3);
    closeADXFAndCleanup();
    return false;
}

bool CDeviceFileCri::func_80450260() {
    CDeviceFileJobReadDvd* job = getFirstCDeviceFileJobReadDvd();
    int reqSct = ADXF_GetNumReqSct(mADXFHandle);
    if (reqSct - 1 <= 1) return false;

    if (reqSct == 3) {
        CFileHandle* handle = job->mHandle;
        u32 remaining = handle->mLength - handle->unk10;
        u32 sectors = remaining >> 11;
        handle->func_80451CBC(sectors << 11);

        if (handle->unk10 != 0 && handle->unk10 == handle->mLength) {
            ADXF_Stop(mADXFHandle);
            ADXF_Close(mADXFHandle);
            mADXFHandle = nullptr;
            mActiveWorkID = job->mWorkID;
            mState = 8;
            mIdleCounter = 0;
            return true;
        }
        return false;
    }

    CFileHandle* handle = job->mHandle;
    handle->unk10 = 0;
    handle->call(CBM_3);
    closeADXFAndCleanup();
    return false;
}

bool CDeviceFileCri::func_8045042C() {
    CDeviceFileJobReadDvd* job = getFirstCDeviceFileJobReadDvd();
    int reqSct = ADXF_GetNumReqSct(mADXFHandle);
    if (reqSct - 1 <= 1) return false;

    if (reqSct == 3) {
        CFileHandle* handle = job->mHandle;
        int remaining = handle->mLength - handle->unk10;
        if (remaining > 0) {
            memcpy(mBuffer, (char*)handle->mData + handle->unk10, remaining);
            DCFlushRange((char*)handle->mData + handle->unk10, remaining);
        }
        handle->func_80451CBC(remaining);
        ADXF_Stop(mADXFHandle);
        ADXF_Close(mADXFHandle);
        mADXFHandle = nullptr;
        mActiveWorkID = job->mWorkID;
        mState = 8;
        mIdleCounter = 0;
        return true;
    }

    CFileHandle* handle = job->mHandle;
    handle->call(CBM_3);
    closeADXFAndCleanup();
    return false;
}

void CDeviceFileCri::wkUpdate() {
    if (!func_8044F744()) return;

    switch (mState) {
    case 0: {
        if (mChildren.front() == mChildren.end()) break;
        CWorkThread* child = mChildren.front()->wkGetChild();
        if (child == nullptr || child->mType != 68) break;

        CDeviceFileJobReadDvd* dvdJob = static_cast<CDeviceFileJobReadDvd*>(child);
        CFileHandle* handle = dvdJob->mHandle;

        if (handle->unk1A4 != 0) {
            handle->unk210 = 1;
            mADXFHandle = ADXF_OpenNw(handle->mName.c_str(), 0);
        } else {
            handle->unk210 = 1;
            mADXFHandle = ADXF_OpenNw((const char*)handle + 0x184, 0);
        }
        mState = 1;
        break;
    }
    case 1:
        if (func_8044FCFC()) break;
        break;
    case 2:
        if (mTimeoutCounter-- <= 0) {
            CDeviceFileJobReadDvd* job = getFirstCDeviceFileJobReadDvd();
            if (job != nullptr) {
                CFileHandle* handle = job->mHandle;
                u32 sectors = handle->unk10 >> 11;
                if (sectors != 0) {
                    ADXF_Seek(mADXFHandle, sectors, 0);
                }
                if (handle->unk10 & 0x7FF) {
                    ADXF_ReadNw(mADXFHandle, 1, mBuffer);
                    mState = 3;
                } else {
                    mState = 4;
                    mTimeoutCounter = 0;
                }
            }
        }
        break;
    case 3:
        if (func_80450058()) break;
        break;
    case 4: {
        CDeviceFileJobReadDvd* job = getFirstCDeviceFileJobReadDvd();
        CFileHandle* handle = job->mHandle;
        int remaining = handle->mLength - handle->unk10;
        if (remaining > 0) {
            int partialBytes = handle->unk10 & 0x7FF;
            int readSize = 0x800 - partialBytes;
            if (readSize > handle->mLength) readSize = handle->mLength;
            memcpy((char*)mBuffer + partialBytes, (char*)handle->mData + handle->unk10, readSize);
            DCFlushRangeNoSync((char*)handle->mData + handle->unk10, readSize);
            handle->func_80451CBC(readSize);
        }
        if (handle->unk10 != 0 && handle->unk10 == handle->mLength) {
            ADXF_Stop(mADXFHandle);
            ADXF_Close(mADXFHandle);
            mADXFHandle = nullptr;
            mActiveWorkID = job->mWorkID;
            mIdleCounter = 0;
            mState = 8;
            return;
        }
        mState = 5;
        return;
    }
    case 5: {
        CDeviceFileJobReadDvd* job = getFirstCDeviceFileJobReadDvd();
        CFileHandle* handle = job->mHandle;
        int remaining = handle->mLength - handle->unk10;
        int sectors = remaining >> 11;
        handle->func_80451CBC(sectors << 11);
        if (handle->unk10 == handle->mLength) {
            ADXF_Stop(mADXFHandle);
            ADXF_Close(mADXFHandle);
            mADXFHandle = nullptr;
            mActiveWorkID = job->mWorkID;
            mState = 8;
            mIdleCounter = 0;
            return;
        }
        int bulkSectors = (handle->mLength - handle->unk10) >> 11;
        ADXF_ReadNw(mADXFHandle, bulkSectors, (char*)handle->mData + handle->unk10);
        mState = 6;
        return;
    }
    case 6:
        if (func_80450260()) break;
        break;
    case 7: {
        CDeviceFileJobReadDvd* job = getFirstCDeviceFileJobReadDvd();
        CFileHandle* handle = job->mHandle;
        int remaining = handle->mLength - handle->unk10;
        if (remaining != 0) {
            ADXF_ReadNw(mADXFHandle, 1, mBuffer);
        }
        mState = 6;
        break;
    }
    case 8: {
        if (mChildren.front() == mChildren.end()) {
            mState = 0;
            break;
        }
        CWorkThread* child = mChildren.front()->wkGetChild();
        if (child == nullptr || child->mType != 68) {
            mState = 0;
            break;
        }
        CDeviceFileJobReadDvd* dvdJob = static_cast<CDeviceFileJobReadDvd*>(child);
        if (dvdJob->mWorkID == mActiveWorkID && !dvdJob->isException()) {
            mIdleCounter++;
            if (mIdleCounter >= 30) {
                CDeviceFileJobReadDvd* job = static_cast<CDeviceFileJobReadDvd*>(
                    mChildren.front()->wkGetChild());
                if (job != nullptr && job->mType == 68) {
                    job->callCBM3();
                }
                mIdleCounter = 0;
            }
        }
        mState = 0;
        break;
    }
    }
}

bool CDeviceFileCri::func_8044F744() {
    if (CWorkSystem::isOff()) return true;
    if (!func_eu_804521C4()) return true;

    int status = DVDGetDriveStatus();
    if (status == -1) {
        CException::getInstance()->func_80459118();
        return false;
    }

    if (status == 4 || status == 11) {
        if (!hasActiveCDeviceFileJobReadDvd()) {
            CException::func_80457CA4(this, lbl_eu_806636C8, 4);
        }
        return false;
    }

    if (status == 6) {
        if (!(mFlags & THREAD_FLAG_EXCEPTION)) {
            if (!hasActiveCDeviceFileJobReadDvd()) {
                CException::func_80457CA4(this, lbl_eu_806636C8, 4);
            }
        }
        return false;
    }

    return true;
}

void CDeviceFileCri::wkStandbyLogin() {
    CWorkThread::wkStandbyLogin();
}

bool CDeviceFileCri::wkStandbyLogout() {
    if (mChildren.front() == mChildren.end()) {
        if (CWorkSystem::getInstance() == nullptr && CLib::getInstance() == nullptr) {
            return CWorkThread::wkStandbyLogout();
        }
    }
    return false;
}

bool CDeviceFileCri::wkStandbyExceptionRetry(u32 wid) {
    if (!func_8044F744()) {
        if (mExceptionPending) {
            mExceptionPending = false;
            return true;
        }
        mRetryCounter = 0;
        return false;
    }

    mExceptionPending = true;
    u32 framerate = CDeviceVI::getTargetFramerate();
    mRetryCounter++;
    if (mRetryCounter < framerate * 6) {
        CException::getInstance()->func_8045925C();
        return false;
    }
    mExceptionPending = false;
    return true;
}

void CDeviceFileCri::func_80450AB8() {
    CDeviceFileCri* inst = getInstance();
    if (inst != nullptr && inst->mADXFHandle != nullptr) {
        ADXF_Stop(inst->mADXFHandle);
        ADXF_GetNumReqSct(inst->mADXFHandle);
        ADXF_Close(inst->mADXFHandle);
        inst->mADXFHandle = nullptr;
    }
}

void CDeviceFileCri::func_80450B14(const wchar_t* pData) {
    lbl_eu_806636C8 = (wchar_t*)pData;
}

void CDeviceFileCri::func_80450B1C(const wchar_t* pData) {
    lbl_eu_806636CC = (wchar_t*)pData;
}

void CDeviceFileCri::func_80450B24(const wchar_t* pData) {
    lbl_eu_806636D0 = (wchar_t*)pData;
}

void CDeviceFileCri::getInstance() {}
void CDeviceFileCri::UnkStruct_8044F65C_UnkVirtualFunc1() {}
void CDeviceFileCri::UnkStruct_8044F65C_UnkVirtualFunc2() {}
void CDeviceFileCri::UnkStruct_8044F65C_UnkVirtualFunc3() {}

static void sinit_80450B2C() {
    // TODO: static initializer
}

extern "C" {
void func_80450B44(void* this_ptr, u32 arg) {
    CDeviceFileCri* obj = (CDeviceFileCri*)((char*)this_ptr - 0x1C4);
    obj->func_80450AB8();
}

void func_80450B4C(void* this_ptr) {
    CDeviceFileCri* obj = (CDeviceFileCri*)((char*)this_ptr - 0x1C4);
    obj->~CDeviceFileCri();
}
}
