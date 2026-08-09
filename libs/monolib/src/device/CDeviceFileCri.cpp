#include "monolib/device.hpp"
#include "monolib/core.hpp"
#include <revolution/DVD.h>
#include <revolution/os/OSCache.h>
#include <string.h>

CDeviceFileCri* CDeviceFileCri::getInstance() { return sInstance; }

void CDeviceFileCri::func_80450B14(const wchar_t* pData) { lbl_eu_806636C8 = (wchar_t*)pData; }
void CDeviceFileCri::func_80450B1C(const wchar_t* pData) { lbl_eu_806636CC = (wchar_t*)pData; }
void CDeviceFileCri::func_80450B24(const wchar_t* pData) { lbl_eu_806636D0 = (wchar_t*)pData; }

extern "C" void sinit_80450B2C() {
    lbl_eu_806576C8[0] = 0;
    *(u32*)(lbl_eu_806576C8 + 0x80) = 0;
}

void CDeviceFileCri::func_80450AB8(unsigned long) {
    if (sInstance != nullptr && sInstance->mADXFHandle != nullptr) {
        ADXF_Stop(sInstance->mADXFHandle);
        ADXF_GetNumReqSct(sInstance->mADXFHandle);
        ADXF_Close(sInstance->mADXFHandle);
        sInstance->mADXFHandle = nullptr;
    }
}

extern "C" void func_80450B44(void* self, u32 arg) {
    func_8044F744__14CDeviceFileCriFv((CDeviceFileCri*)((char*)self - 0x1C4));
}

extern "C" void __dt__14CDeviceFileCriFv(void*, int);

extern "C" void func_80450B4C(void* self) {
    ((void(*)(void*))__dt__14CDeviceFileCriFv)((char*)self - 0x1C4);
}

CDeviceFileCri::CDeviceFileCri(const char* pName, CWorkThread* pParent, int capacity)
    : CWorkThread(pName, pParent, capacity) {
    *(u32**)this = (u32*)lbl_eu_8056C354;
    *(u32**)((char*)this + 0x1C4) = (u32*)((char*)lbl_eu_8056C354 + 0xA0);
    
    mState = 0;
    mActiveWorkID = (WORK_ID)-1;
    unk1D0 = 0;
    mADXFHandle = nullptr;
    mBuffer = nullptr;
    mIdleCounter = 0;
    mRetryCounter = 0;
    mExceptionPending = 0;
    mTimeoutCounter = 0;
    
    sInstance = this;
    
    mtl::ALLOC_HANDLE handle = CDevice::getDevSys2Handle();
    mBuffer = mtl::MemManager::allocate_head(handle, 0x800, 0x20);
    
    IException* pException = (IException*)((char*)this + 0x1C4);
    func_804591BC__10CExceptionFP10IException((CException*)this, pException);
}

CDeviceFileCri::~CDeviceFileCri() {
    if (this == nullptr) return;
    
    *(u32**)this = (u32*)lbl_eu_8056C354;
    *(u32**)((char*)this + 0x1C4) = (u32*)((char*)lbl_eu_8056C354 + 0xA0);
    
    func_804591DC__10CExceptionFP10IException((CException*)this);
    
    sInstance = nullptr;
    
    if (mBuffer != nullptr) {
        mtl::MemManager::deallocate(mBuffer);
        mBuffer = nullptr;
    }
    
    CWorkThread::~CWorkThread();
}

CDeviceFileJobReadDvd* CDeviceFileCri::getFirstCDeviceFileJobReadDvd() {
    if (mChildren.empty()) return nullptr;
    CWorkThread* child = mChildren.front();
    if (child == nullptr || child->mType != THREAD_CDEVICEFILEJOBREADDVD) return nullptr;
    return (CDeviceFileJobReadDvd*)child;
}

void CDeviceFileCri::closeADXFAndCleanup() {
    CDeviceFileJobReadDvd* job = getFirstCDeviceFileJobReadDvd();
    if (mADXFHandle != nullptr) {
        ADXF_Stop(mADXFHandle);
        ADXF_GetNumReqSct(mADXFHandle);
        ADXF_Close(mADXFHandle);
        mADXFHandle = nullptr;
    }
    removeFileJob__11CDeviceFileFP14CDeviceFileJob(job);
    mState = 0;
}

bool CDeviceFileCri::func_8044F744() {
    if (isOff__11CWorkSystemFv()) return true;
    if (!func_eu_804521C4()) return true;
    
    int status = DVDGetDriveStatus();
    
    if (status == -1) {
        func_80459118__10CExceptionFv("DVD fatal error");
        return false;
    }
    
    if (status == 6 || status == 11 || status == 4) {
        bool hasWork = false;
        if (mFlags & THREAD_FLAG_PAUSE) {
            hasWork = true;
        } else if (!mChildren.empty()) {
            CWorkThread* child = mChildren.front();
            if (child != nullptr && child->mType == THREAD_CDEVICEFILEJOBREADDVD) {
                hasWork = true;
            }
        }
        
        if (!hasWork) {
            const wchar_t* msg = (status == 11) ? lbl_eu_806636CC : lbl_eu_806636C8;
            func_80457CA4__10CExceptionFP11CWorkThreadPCwUl(this, msg, 4);
        }
        return false;
    }
    
    return true;
}

void CDeviceFileCri::func_8044F964() {
    if (sInstance == nullptr) return;
    if (sInstance->mADXFHandle == nullptr) return;
    
    ADXF_Stop(sInstance->mADXFHandle);
    ADXF_Close(sInstance->mADXFHandle);
    sInstance->mADXFHandle = nullptr;
}

int CDeviceFileCri::getFileSize(const char* pPath, int arg1) {
    char pathBuf[0x80];
    char nameBuf[0x80];
    
    strcpy(pathBuf, pPath);
    if (arg1 != 0) {
        func_eu_804520D0(pathBuf);
    }
    
    if (pPath[0] == '/') {
        strcpy(nameBuf, pPath + 1);
    } else {
        strcpy(nameBuf, pPath);
    }
    
    int nameLen = strlen(nameBuf);
    const char* dotStr = ".adx";
    int dotStrLen = strlen(dotStr);
    
    int foundIdx = -1;
    for (int i = 0; i < nameLen; i++) {
        if (strncmp(&nameBuf[i], dotStr, dotStrLen) == 0) {
            foundIdx = i;
            break;
        }
    }
    
    if (foundIdx != -1 && foundIdx < nameLen) {
        nameBuf[foundIdx] = '\0';
    }
    
    int ret = func_804DDCD4(nameBuf, pPath);
    if (ret > 0) return ret;
    
    int entrynum = DVDConvertPathToEntrynum(pathBuf);
    if (entrynum < 0) return ret;
    
    DVDFileInfo fileInfo;
    if (!DVDFastOpen(entrynum, &fileInfo)) return ret;
    
    int size = fileInfo.size;
    int rem = size & 0x7FF;
    if (rem != 0) {
        size = (size + 0x800) - rem;
    }
    
    DVDClose(&fileInfo);
    return size;
}

void CDeviceFileCri::func_8044FB08(const char* pPath) {
    CDeviceFileJobReadDvd* job = sInstance->getFirstCDeviceFileJobReadDvd();
    if (job != nullptr) {
        job->cancel(pPath);
    }
}

bool CDeviceFileCri::cancel(CFileHandle* pHandle) {
    CDeviceFileJobReadDvd* job = sInstance->getFirstCDeviceFileJobReadDvd();
    if (job != nullptr) {
        return job->cancel(pHandle);
    }
    return false;
}

void CDeviceFileCri::func_8044FC38() {
    CDeviceFileCri* inst = sInstance;
    u32 count = inst->mChildren.size();
    if (count == 0) return;
    
    CDeviceFileJobReadDvd* job = inst->getFirstCDeviceFileJobReadDvd();
    
    if (inst->mADXFHandle != nullptr) {
        ADXF_Stop(inst->mADXFHandle);
        ADXF_GetNumReqSct(inst->mADXFHandle);
        ADXF_Close(inst->mADXFHandle);
        inst->mADXFHandle = nullptr;
    }
    
    removeFileJob__11CDeviceFileFP14CDeviceFileJob(job);
    inst->mState = 0;
}

bool CDeviceFileCri::func_8044FCFC() {
    CDeviceFileJobReadDvd* job = getFirstCDeviceFileJobReadDvd();
    if (job == nullptr) return false;
    
    CFileHandle* handle = job->mHandle;
    
    if (!ADXF_IsOpened(mADXFHandle)) {
        int numReq = ADXF_GetNumReqSct(mADXFHandle);
        if (numReq == 4) {
            call__11CFileHandleF3CBM(handle, 3);
            closeADXFAndCleanup();
        }
        return false;
    }
    
    int fileSize = ADXF_GetFsizeByte(mADXFHandle);
    int alignedSize = fileSize;
    int rem = fileSize & 0x7FF;
    if (rem != 0) {
        alignedSize = (fileSize + 0x800) - rem;
    }
    
    int readSize = alignedSize;
    
    if (alignedSize <= 0) {
        call__11CFileHandleF3CBM(handle, 3);
        closeADXFAndCleanup();
        return false;
    }
    
    if (handle->unk10 != 0) {
        readSize = handle->unk10;
    }
    
    u32 endOff = readSize + handle->unk10;
    if (endOff > (u32)alignedSize) {
        readSize -= (endOff - alignedSize);
    }
    
    destroy__11CFileHandleFv(handle);
    
    if (handle->unk14 & 0x10) {
        call__11CFileHandleF3CBM(handle, 3);
        closeADXFAndCleanup();
        return false;
    }
    
    mTimeoutCounter = 0;
    if (handle->unk10 != 0) {
        mState = 2;
        return true;
    } else {
        mState = 4;
        return false;
    }
}

bool CDeviceFileCri::func_80450058() {
    CDeviceFileJobReadDvd* job = getFirstCDeviceFileJobReadDvd();
    CFileHandle* handle = job->mHandle;
    
    int numReq = ADXF_GetNumReqSct(mADXFHandle);
    if ((u32)(numReq - 1) <= 1) return false;
    
    if (numReq == 3) {
        u32 offset = handle->unk10 & 0x7FF;
        u32 copySize = 0x800 - offset;
        if (copySize > handle->mLength) copySize = handle->mLength;
        
        memcpy((char*)mBuffer + offset, (char*)handle->mData + handle->unk10, copySize);
        DCFlushRangeNoSync((char*)handle->mData + handle->unk10, copySize);
        
        func_80451CBC__11CFileHandleFi(handle, copySize);
        
        bool complete = (handle->unk10 != 0 && handle->unk10 == handle->mLength);
        
        if (complete) {
            ADXF_Stop(mADXFHandle);
            ADXF_Close(mADXFHandle);
            mADXFHandle = nullptr;
            mActiveWorkID = job->mWorkID;
            mIdleCounter = 0;
            mState = 8;
            return true;
        }
        
        if (handle->unk10 == 0) {
            mState = 4;
            return true;
        }
        
        return false;
    }
    
    call__11CFileHandleF3CBM(handle, 3);
    closeADXFAndCleanup();
    return false;
}

bool CDeviceFileCri::func_80450260() {
    CDeviceFileJobReadDvd* job = getFirstCDeviceFileJobReadDvd();
    CFileHandle* handle = job->mHandle;
    
    int numReq = ADXF_GetNumReqSct(mADXFHandle);
    if ((u32)(numReq - 1) <= 1) return false;
    
    if (numReq == 3) {
        u32 remaining = handle->mLength - handle->unk10;
        u32 aligned = remaining & ~0x7FFu;
        
        func_80451CBC__11CFileHandleFi(handle, aligned);
        
        bool complete = (handle->unk10 != 0 && handle->unk10 == handle->mLength);
        if (complete) {
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
    
    call__11CFileHandleF3CBM(handle, 3);
    closeADXFAndCleanup();
    return false;
}

bool CDeviceFileCri::func_8045042C() {
    CDeviceFileJobReadDvd* job = getFirstCDeviceFileJobReadDvd();
    CFileHandle* handle = job->mHandle;
    
    int numReq = ADXF_GetNumReqSct(mADXFHandle);
    if ((u32)(numReq - 1) <= 1) return false;
    
    if (numReq == 3) {
        int remaining = handle->mLength - handle->unk10;
        if (remaining > 0) {
            memcpy(mBuffer, (char*)handle->mData + handle->unk10, remaining);
            DCFlushRange((char*)handle->mData + handle->unk10, remaining);
        }
        
        func_80451CBC__11CFileHandleFi(handle, remaining);
        
        ADXF_Stop(mADXFHandle);
        ADXF_Close(mADXFHandle);
        mADXFHandle = nullptr;
        mActiveWorkID = job->mWorkID;
        mState = 8;
        mIdleCounter = 0;
        return true;
    }
    
    call__11CFileHandleF3CBM(handle, 3);
    closeADXFAndCleanup();
    return false;
}

void CDeviceFileCri::wkUpdate() {
    if (!func_8044F744()) return;
    
    u32 state = mState;
    if (state > 8) return;
    
    switch (state) {
    case 0: {
        if (mChildren.empty()) break;
        CWorkThread* child = mChildren.front();
        if (child == nullptr || child->mType != THREAD_CDEVICEFILEJOBREADDVD) break;
        
        CDeviceFileJobReadDvd* job = (CDeviceFileJobReadDvd*)child;
        
        bool ready = false;
        if (job->mFlags & THREAD_FLAG_PAUSE) {
            ready = true;
        } else {
            u32 jState = job->mState;
            if (jState == 2 || jState == 3) {
                ready = true;
            }
        }
        
        if (!ready) break;
        if (job->mFlags & THREAD_FLAG_EXCEPTION) break;
        
        CFileHandle* handle = job->mHandle;
        handle->unk10 = 1;
        
        const char* filename;
        if (handle->unk14 == 0) {
            filename = handle->mName.c_str() + 0x5C;
        } else {
            filename = handle->mName.c_str() + 0x184;
        }
        
        mADXFHandle = ADXF_OpenNw(filename, 0);
        mState = 1;
        break;
    }
    case 1: {
        if (!func_8044FCFC()) break;
        
        mTimeoutCounter--;
        if (mTimeoutCounter > 0) break;
        
        CDeviceFileJobReadDvd* job = getFirstCDeviceFileJobReadDvd();
        CFileHandle* handle = job->mHandle;
        u32 remaining = handle->mLength - handle->unk10;
        
        if (remaining > 0x800) {
            ADXF_ReadNw(mADXFHandle, remaining >> 11, (char*)handle->mData + handle->unk10);
            mState = 5;
        } else {
            mState = 7;
        }
        break;
    }
    case 2: {
        if (!func_80450058()) break;
        
        mTimeoutCounter--;
        if (mTimeoutCounter > 0) break;
        
        CDeviceFileJobReadDvd* job = getFirstCDeviceFileJobReadDvd();
        CFileHandle* handle = job->mHandle;
        u32 remaining = handle->mLength - handle->unk10;
        
        if (remaining > 0x800) {
            ADXF_ReadNw(mADXFHandle, remaining >> 11, (char*)handle->mData + handle->unk10);
            mState = 5;
        } else {
            mState = 7;
        }
        break;
    }
    case 3: {
        func_80450058();
        break;
    }
    case 4: {
        func_8045042C();
        break;
    }
    case 5: {
        func_80450260();
        break;
    }
    case 6: {
        if (mChildren.empty()) break;
        CWorkThread* child = mChildren.front();
        if (child == nullptr || child->mType != THREAD_CDEVICEFILEJOBREADDVD) break;
        
        CDeviceFileJobReadDvd* job = (CDeviceFileJobReadDvd*)child;
        CFileHandle* handle = job->mHandle;
        
        u32 remaining = handle->mLength - handle->unk10;
        if (remaining != 0) {
            ADXF_ReadNw(mADXFHandle, 1, mBuffer);
        }
        mState = 6;
        break;
    }
    case 7: {
        func_8045042C();
        break;
    }
    case 8: {
        if (mChildren.empty()) {
            mState = 0;
            break;
        }
        
        CWorkThread* child = mChildren.front();
        if (child == nullptr || child->mType != THREAD_CDEVICEFILEJOBREADDVD) {
            mState = 0;
            break;
        }
        
        if (mActiveWorkID != child->mWorkID) {
            mState = 0;
            break;
        }
        
        if (!(child->mFlags & THREAD_FLAG_EXCEPTION)) {
            mIdleCounter++;
            if (mIdleCounter >= 30) {
                callCBM3__21CDeviceFileJobReadDvdFv((CDeviceFileJobReadDvd*)child);
                mIdleCounter = 0;
            }
            break;
        }
        
        mState = 0;
        break;
    }
    }
}

bool CDeviceFileCri::wkStandbyLogin() {
    return CWorkThread::wkStandbyLogin();
}

bool CDeviceFileCri::wkStandbyLogout() {
    if (mChildren.empty()) {
        CWorkSystem* ws = (CWorkSystem*)getInstance__11CWorkSystemFv();
        if (ws == nullptr) {
            void* clib = getInstance__4CLibFv();
            if (clib == nullptr) {
                return CWorkThread::wkStandbyLogout();
            }
        }
    }
    return false;
}

bool CDeviceFileCri::wkStandbyExceptionRetry(u32 wid) {
    bool result = func_8044F744();
    
    if (!result) {
        if (mExceptionPending) {
            mExceptionPending = false;
            return true;
        }
        mRetryCounter = 0;
        return false;
    }
    
    mExceptionPending = true;
    
    u32 framerate = getTargetFramerate__9CDeviceVIFv();
    u32 maxRetries = framerate * 6;
    
    mRetryCounter++;
    if (mRetryCounter < maxRetries) {
        func_8045925C__10CExceptionFv((CException*)this);
        return false;
    }
    
    mExceptionPending = false;
    return true;
}
