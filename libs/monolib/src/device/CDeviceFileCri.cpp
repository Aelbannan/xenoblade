#include "monolib/device.hpp"
#include "monolib/core.hpp"
#include <revolution/DVD.h>
#include <revolution/os/OSCache.h>
#include <string.h>

// Retail singleton pointer (sda21 .sbss) — the decomp's static sInstance member
// resolves to the same address; referencing this name directly keeps the reloc
// byte-identical to retail.
extern CDeviceFileCri* lbl_eu_80665668;
// Shared rodata string pool: the ".adx" extension marker begins at +6.
extern const char lbl_eu_80522CA0[];

// Layout view of CFileHandle exposing the u32 at 0x38 (which sits inside the
// unk18 padding in CFileHandle.hpp) and the flags word at 0x58 — used to read
// the current sector offset / pending-flag. Layout-identical to CFileHandle's
// first 0x5C bytes.
struct CFileHandleLayout {
    u8 field_0x0[0x4];    //0x0
    u8* mData;            //0x4
    u8 field_0x8[0x8];    //0x8
    int unk10;            //0x10
    u8 field_0x14[0x24];  //0x14
    u32 field_0x38;       //0x38 current read offset
    u32 mLength;          //0x3C
    u8 field_0x40[0x18];  //0x40
    u32 field_0x58;       //0x58 flags (bit 3 = allocation/read pending)
};

// Retail destroy__11CFileHandleFv really takes (handle, size, allocParam,
// blockSize) — the header declares it with only the handle. Declaring the
// full signature here (C linkage ignores the namespace for the symbol name)
// makes the call site emit the same four argument registers as retail.
namespace {
extern "C" void destroy__11CFileHandleFv(CFileHandle* handle, u32 size, u32 allocParam, u32 blockSize);
}

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

extern "C" void* __dt__14CDeviceFileCriFv(void*, int);

// Deleting destructor (retail __dt__FP10IExceptionFv): free self when
// mode > 0, return self.
extern "C" void* __dt__FP10IExceptionFv(void* self, int mode) {
    extern void* __dl__FPv(void*);
    if (self != 0 && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

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

// Free-function dtor form (retail __dt__14CDeviceFileCriFv): the member dtor
// double-emits the vptr stores and the CWorkThread base dtor; the extern-C
// form emits each once. The CException subobject lives at +0x1C4 (retail
// passes this+0x1C4 to func_804591DC) and the base dtor is the 1-arg
// __dt__11CWorkThreadFv with flag 0.
extern "C" void __dt__11CWorkThreadFv(void* self, int flags);
extern "C" void* __dl__FPv(void* self);

extern "C" void* __dt__14CDeviceFileCriFv(void* self, int flags) {
    if (self != 0) {
        *(u32**)self = (u32*)lbl_eu_8056C354;
        *(u32**)((char*)self + 0x1C4) = (u32*)((char*)lbl_eu_8056C354 + 0xA0);
        IException* pException = (IException*)self;
        if (self != 0) {
            pException = (IException*)((u8*)self + 0x1C4);
        }
        func_804591DC__10CExceptionFP10IException((CException*)pException);
        lbl_eu_80665668 = 0;
        void* buffer = *(void**)((u8*)self + 0x1D8);
        if (buffer != 0) {
            mtl::MemManager::deallocate(buffer);
            *(void**)((u8*)self + 0x1D8) = 0;
        }
        __dt__11CWorkThreadFv(self, 0);
        if (flags > 0) {
            __dl__FPv(self);
        }
    }
    return self;
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
        // Fatal DVD error: raise the global exception handler, then fall
        // through to the shared "keep running" tail below.
        func_80459118__10CExceptionFv(lbl_eu_80522CA0);
    } else if (status == 6) {
        // Recoverable DVD errors: only surface the message when an exception
        // is not already pending (flag or queued EVT_EXCEPTION), then stay
        // busy.
        if (!isException()) {
            func_80457CA4__10CExceptionFP11CWorkThreadPCwUl(this, lbl_eu_806636C8, 4);
        }
        return false;
    } else if (status == 11) {
        if (!isException()) {
            func_80457CA4__10CExceptionFP11CWorkThreadPCwUl(this, lbl_eu_806636CC, 4);
        }
        return false;
    } else if (status == 4) {
        if (!isException()) {
            func_80457CA4__10CExceptionFP11CWorkThreadPCwUl(this, lbl_eu_806636C8, 4);
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
    DVDFileInfo fileInfo;

    int pathLen = strlen(pPath);   // retail spills this (dead in the binary)
    strcpy(pathBuf, pPath);

    if (arg1 != 0) {
        func_eu_804520D0(pathBuf);
    }

    // Strip a leading '/' from the name; nameLen is the strlen of the SOURCE
    // (not of the copied buffer) in retail.
    nameBuf[0] = '\0';
    int nameLen = 0;
    if (pPath[0] == '/') {
        nameLen = strlen(pPath + 1);
        strcpy(nameBuf, pPath + 1);
    } else {
        nameLen = strlen(pPath);
        strcpy(nameBuf, pPath);
    }

    // ".adx" extension marker lives at lbl_eu_80522CA0 + 6.
    const char* ext = lbl_eu_80522CA0 + 6;
    int extLen = strlen(ext);

    // Find the extension; the scan index becomes -1 when the whole name is
    // scanned without a match.
    int i = 0;
    while (i < nameLen) {
        if (strncmp(&nameBuf[i], ext, extLen) == 0) break;
        i++;
    }
    if (i >= nameLen) i = -1;

    if (i != -1 && i < nameLen) {
        nameBuf[i] = '\0';
        nameLen = i;
    }

    int ret = func_804DDCD4(nameBuf, pPath);
    if (ret > -1) return ret;

    int entrynum = DVDConvertPathToEntrynum(pathBuf);
    if (entrynum < 0) return ret;

    if (!DVDFastOpen(entrynum, &fileInfo)) return ret;

    int size = fileInfo.size;
    int rem = size & 0x7FF;
    if (rem != 0) {
        size = size + 0x800 - rem;
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
    // Count the singleton's children; the list walk is the size() inline.
    u32 count = lbl_eu_80665668->mChildren.size();
    if (count == 0) return;

    // Grab the first CDeviceFileJobReadDvd child (mType == 0x44) directly —
    // the retail inlines this without the empty() guard.
    CWorkThread* child = lbl_eu_80665668->mChildren.front();
    CDeviceFileJobReadDvd* job;
    if (child != nullptr && child->mType == THREAD_CDEVICEFILEJOBREADDVD) {
        job = (CDeviceFileJobReadDvd*)child;
    } else {
        job = nullptr;
    }

    if (lbl_eu_80665668->mADXFHandle != nullptr) {
        ADXF_Stop(lbl_eu_80665668->mADXFHandle);
        ADXF_GetNumReqSct(lbl_eu_80665668->mADXFHandle);
        ADXF_Close(lbl_eu_80665668->mADXFHandle);
        lbl_eu_80665668->mADXFHandle = nullptr;
    }

    CDeviceFile::removeFileJob(job);
    lbl_eu_80665668->mState = 0;
}

bool CDeviceFileCri::func_8044FCFC() {
    // Inline getFirstCDeviceFileJobReadDvd: retail reads front() directly
    // (no empty() guard) and validates the child type.
    CWorkThread* child = mChildren.front();
    CDeviceFileJobReadDvd* job;
    if (child == nullptr || child->mType != THREAD_CDEVICEFILEJOBREADDVD) {
        job = nullptr;
    } else {
        job = (CDeviceFileJobReadDvd*)child;
    }
    if (job == nullptr) return false;

    CFileHandle* handle = job->mHandle;
    CFileHandleLayout* layout = (CFileHandleLayout*)handle;

    if (!ADXF_IsOpened(mADXFHandle)) {
        if (ADXF_GetNumReqSct(mADXFHandle) == 4) {
            // Abort: cancel the handle, then inline closeADXFAndCleanup
            // against the singleton (retail re-inlines this block per site).
            call__11CFileHandleF3CBM(handle, 3);
            u32 count = lbl_eu_80665668->mChildren.size();
            if (count != 0) {
                CWorkThread* child2 = lbl_eu_80665668->mChildren.front();
                CDeviceFileJobReadDvd* job2;
                if (child2 == nullptr || child2->mType != THREAD_CDEVICEFILEJOBREADDVD) {
                    job2 = nullptr;
                } else {
                    job2 = (CDeviceFileJobReadDvd*)child2;
                }
                if (lbl_eu_80665668->mADXFHandle != nullptr) {
                    ADXF_Stop(lbl_eu_80665668->mADXFHandle);
                    ADXF_GetNumReqSct(lbl_eu_80665668->mADXFHandle);
                    ADXF_Close(lbl_eu_80665668->mADXFHandle);
                    lbl_eu_80665668->mADXFHandle = nullptr;
                }
                removeFileJob__11CDeviceFileFP14CDeviceFileJob(job2);
                lbl_eu_80665668->mState = 0;
            }
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
    ADXF_GetFsizeSct(mADXFHandle);

    if (alignedSize <= 0) {
        call__11CFileHandleF3CBM(handle, 3);
        u32 count = lbl_eu_80665668->mChildren.size();
        if (count != 0) {
            CWorkThread* child2 = lbl_eu_80665668->mChildren.front();
            CDeviceFileJobReadDvd* job2;
            if (child2 == nullptr || child2->mType != THREAD_CDEVICEFILEJOBREADDVD) {
                job2 = nullptr;
            } else {
                job2 = (CDeviceFileJobReadDvd*)child2;
            }
            if (lbl_eu_80665668->mADXFHandle != nullptr) {
                ADXF_Stop(lbl_eu_80665668->mADXFHandle);
                ADXF_GetNumReqSct(lbl_eu_80665668->mADXFHandle);
                ADXF_Close(lbl_eu_80665668->mADXFHandle);
                lbl_eu_80665668->mADXFHandle = nullptr;
            }
            removeFileJob__11CDeviceFileFP14CDeviceFileJob(job2);
            lbl_eu_80665668->mState = 0;
        }
        return false;
    }

    // Clamp the read size to the file's sector-aligned length.
    if (layout->mLength != 0) {
        readSize = layout->mLength;
    }
    if ((u32)(readSize + layout->field_0x38) > (u32)alignedSize) {
        readSize -= (readSize + layout->field_0x38) - alignedSize;
    }

    destroy__11CFileHandleFv(handle, readSize, 0x20, 0x800);

    if (layout->field_0x58 & 0x8) {
        // Allocation failed: cancel the handle and clean up the job.
        call__11CFileHandleF3CBM(handle, 3);
        u32 count = lbl_eu_80665668->mChildren.size();
        if (count != 0) {
            CWorkThread* child2 = lbl_eu_80665668->mChildren.front();
            CDeviceFileJobReadDvd* job2;
            if (child2 == nullptr || child2->mType != THREAD_CDEVICEFILEJOBREADDVD) {
                job2 = nullptr;
            } else {
                job2 = (CDeviceFileJobReadDvd*)child2;
            }
            if (lbl_eu_80665668->mADXFHandle != nullptr) {
                ADXF_Stop(lbl_eu_80665668->mADXFHandle);
                ADXF_GetNumReqSct(lbl_eu_80665668->mADXFHandle);
                ADXF_Close(lbl_eu_80665668->mADXFHandle);
                lbl_eu_80665668->mADXFHandle = nullptr;
            }
            removeFileJob__11CDeviceFileFP14CDeviceFileJob(job2);
            lbl_eu_80665668->mState = 0;
        }
        return false;
    }

    mTimeoutCounter = 0;
    if (layout->field_0x38 != 0) {
        mState = 2;
        return true;
    }
    mState = 4;
    return false;
}

bool CDeviceFileCri::func_80450058() {
    CWorkThread* child = mChildren.front();
    CDeviceFileJobReadDvd* job =
        (child != nullptr && child->mType == THREAD_CDEVICEFILEJOBREADDVD)
            ? (CDeviceFileJobReadDvd*)child : nullptr;

    int numReq = ADXF_GetNumReqSct(mADXFHandle);
    if ((u32)(numReq - 1) <= 1) return false;

    if (numReq == 3) {
        CFileHandleLayout* handle = (CFileHandleLayout*)job->mHandle;
        // Copy one 0x800-sector chunk starting at the handle's sector offset.
        u32 offset = handle->field_0x38 & 0x7FF;
        u32 copySize = 0x800 - offset;
        if (copySize > handle->mLength) copySize = handle->mLength;

        memcpy((char*)mBuffer + offset, handle->mData + handle->unk10, copySize);
        DCFlushRangeNoSync(handle->mData + handle->unk10, copySize);

        func_80451CBC__11CFileHandleFi((CFileHandle*)handle, copySize);

        bool complete = (handle->unk10 != 0 && handle->unk10 == handle->mLength);
        if (complete) {
            ADXF_Stop(mADXFHandle);
            ADXF_Close(mADXFHandle);
            mADXFHandle = nullptr;
            mActiveWorkID = job->mWorkID;
            mIdleCounter = 0;
            mState = 8;
        } else {
            mState = 4;
        }
        return true;
    }

    // Abort: cancel the read job.
    call__11CFileHandleF3CBM(job->mHandle, 3);
    u32 count = lbl_eu_80665668->mChildren.size();
    if (count != 0) {
        CWorkThread* child2 = lbl_eu_80665668->mChildren.front();
        CDeviceFileJobReadDvd* job2 =
            (child2 != nullptr && child2->mType == THREAD_CDEVICEFILEJOBREADDVD)
                ? (CDeviceFileJobReadDvd*)child2 : nullptr;

        if (lbl_eu_80665668->mADXFHandle != nullptr) {
            ADXF_Stop(lbl_eu_80665668->mADXFHandle);
            ADXF_GetNumReqSct(lbl_eu_80665668->mADXFHandle);
            ADXF_Close(lbl_eu_80665668->mADXFHandle);
            lbl_eu_80665668->mADXFHandle = nullptr;
        }

        CDeviceFile::removeFileJob(job2);
        lbl_eu_80665668->mState = 0;
    }
    return false;
}

bool CDeviceFileCri::func_80450260() {
    CWorkThread* child = mChildren.front();
    CDeviceFileJobReadDvd* job =
        (child != nullptr && child->mType == THREAD_CDEVICEFILEJOBREADDVD)
            ? (CDeviceFileJobReadDvd*)child : nullptr;

    int numReq = ADXF_GetNumReqSct(mADXFHandle);
    if ((u32)(numReq - 1) <= 1) return false;

    if (numReq == 3) {
        CFileHandle* handle = job->mHandle;
        // Advance the read position to the next 0x800-sector boundary.
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
        } else {
            mState = 7;
        }
        return true;
    }

    // Abort: reset the handle position and cancel the read job.
    job->mHandle->unk10 = 0;
    call__11CFileHandleF3CBM(job->mHandle, 3);
    u32 count = lbl_eu_80665668->mChildren.size();
    if (count != 0) {
        CWorkThread* child2 = lbl_eu_80665668->mChildren.front();
        CDeviceFileJobReadDvd* job2 =
            (child2 != nullptr && child2->mType == THREAD_CDEVICEFILEJOBREADDVD)
                ? (CDeviceFileJobReadDvd*)child2 : nullptr;

        if (lbl_eu_80665668->mADXFHandle != nullptr) {
            ADXF_Stop(lbl_eu_80665668->mADXFHandle);
            ADXF_GetNumReqSct(lbl_eu_80665668->mADXFHandle);
            ADXF_Close(lbl_eu_80665668->mADXFHandle);
            lbl_eu_80665668->mADXFHandle = nullptr;
        }

        CDeviceFile::removeFileJob(job2);
        lbl_eu_80665668->mState = 0;
    }
    return false;
}

bool CDeviceFileCri::func_8045042C() {
    CWorkThread* child = mChildren.front();
    CDeviceFileJobReadDvd* job =
        (child != nullptr && child->mType == THREAD_CDEVICEFILEJOBREADDVD)
            ? (CDeviceFileJobReadDvd*)child : nullptr;

    int numReq = ADXF_GetNumReqSct(mADXFHandle);
    if ((u32)(numReq - 1) <= 1) return false;

    if (numReq == 3) {
        CFileHandle* handle = job->mHandle;
        // Copy the remainder of the pending read into the device buffer.
        int remaining = handle->mLength - handle->unk10;
        if (remaining > 0) {
            memcpy(mBuffer, handle->mData + handle->unk10, remaining);
            DCFlushRange(handle->mData + handle->unk10, remaining);
        }

        func_80451CBC__11CFileHandleFi(handle, remaining);

        ADXF_Stop(mADXFHandle);
        ADXF_Close(mADXFHandle);
        mADXFHandle = nullptr;
        mActiveWorkID = job->mWorkID;
        mIdleCounter = 0;
        mState = 8;
        return true;
    }

    // numReq != 3: abort the job and clean up the singleton's ADXF state.
    call__11CFileHandleF3CBM(job->mHandle, 3);
    u32 count = lbl_eu_80665668->mChildren.size();
    if (count != 0) {
        CWorkThread* child2 = lbl_eu_80665668->mChildren.front();
        CDeviceFileJobReadDvd* job2 =
            (child2 != nullptr && child2->mType == THREAD_CDEVICEFILEJOBREADDVD)
                ? (CDeviceFileJobReadDvd*)child2 : nullptr;

        if (lbl_eu_80665668->mADXFHandle != nullptr) {
            ADXF_Stop(lbl_eu_80665668->mADXFHandle);
            ADXF_GetNumReqSct(lbl_eu_80665668->mADXFHandle);
            ADXF_Close(lbl_eu_80665668->mADXFHandle);
            lbl_eu_80665668->mADXFHandle = nullptr;
        }

        CDeviceFile::removeFileJob(job2);
        lbl_eu_80665668->mState = 0;
    }
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
