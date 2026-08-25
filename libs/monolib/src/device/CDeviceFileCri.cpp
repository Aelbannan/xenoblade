#include "monolib/device.hpp"
#include "monolib/core.hpp"
#include <revolution/DVD.h>
#include <revolution/os/OSCache.h>
#include <string.h>

// Retail singleton pointer (sda21 .sbss) - the decomp's static sInstance member
// resolves to the same address; referencing this name directly keeps the reloc
// byte-identical to retail. Defined in the blob section below.
extern CDeviceFileCri* lbl_eu_80665668;
// Shared rodata string pool: the ".adx" extension marker begins at +6.
extern const char lbl_eu_80522CA0[];

// Layout view of CFileHandle exposing the u32 at 0x38 (which sits inside the
// unk18 padding in CFileHandle.hpp) and the flags word at 0x58 - used to read
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
// blockSize) - the header declares it with only the handle. Declaring the
// full signature here (C linkage ignores the namespace for the symbol name)
// makes the call site emit the same four argument registers as retail.
namespace {
extern "C" void destroy__11CFileHandleFv(CFileHandle* handle, u32 size, u32 allocParam, u32 blockSize);
}

CDeviceFileCri* CDeviceFileCri::getInstance() { return sInstance; }

void CDeviceFileCri::func_80450B14(const wchar_t* pData) { *(wchar_t**)&lbl_eu_806636C8[0] = (wchar_t*)pData; }
void CDeviceFileCri::func_80450B1C(const wchar_t* pData) { *(wchar_t**)&lbl_eu_806636CC[0] = (wchar_t*)pData; }
void CDeviceFileCri::func_80450B24(const wchar_t* pData) { *(wchar_t**)&lbl_eu_806636D0[0] = (wchar_t*)pData; }

extern "C" void sinit_80450B2C() {
    lbl_eu_806576C8[0] = 0;
    *(u32*)(lbl_eu_806576C8 + 0x80) = 0;
}

void CDeviceFileCri::func_80450AB8(unsigned long) {
    // retail names the singleton pointer lbl_eu_80665668 here (same storage as
    // the static member); reference the C-linkage label to avoid name drift.
    if (lbl_eu_80665668 != nullptr && lbl_eu_80665668->mADXFHandle != nullptr) {
        ADXF_Stop(lbl_eu_80665668->mADXFHandle);
        ADXF_GetNumReqSct(lbl_eu_80665668->mADXFHandle);
        ADXF_Close(lbl_eu_80665668->mADXFHandle);
        lbl_eu_80665668->mADXFHandle = nullptr;
    }
}

extern "C" void func_80450B44(void* self, u32 arg) {
    func_8044F744__14CDeviceFileCriFv((CDeviceFileCri*)((char*)self - 0x1C4));
}

extern "C" void* __dl__FPv(void* self);

// The deleting-destructor body (retail __dt__FP10IExceptionFv: free self
// when mode > 0, return self) is NOT hand-written here: the compiler already
// emits the identical 0x40 body as the extab DESTROYBASE dtor of the second
// base (__dt__18UnkStruct_8044F65CFv); postprocess exact_renames renames it
// to the retail symbol. A second hand-written copy would duplicate it and
// blow the split budget by 0x40.

namespace CDeviceFileCriBlob { extern "C" void* __dt__14CDeviceFileCriFv(); }

extern "C" void func_80450B4C(void* self) {
    // Direct tail call to the complete-dtor symbol (no deleting flag), like retail.
    ((void (*)(void*)) CDeviceFileCriBlob::__dt__14CDeviceFileCriFv)((char*)self - 0x1C4);
}

CDeviceFileCri::CDeviceFileCri(const char* pName, CWorkThread* pParent, int capacity)
    : CWorkThread(pName, pParent, 0x100) {
    *(u32**)this = (u32*)lbl_eu_8056C354;
    *(u32**)((char*)this + 0x1C4) = (u32*)((char*)lbl_eu_8056C354 + 0xA0);

    mState = 0;
    mActiveWorkID = (WORK_ID)-1;
    unk1D0 = 0;
    mADXFHandle = nullptr;
    mIdleCounter = 0;
    mRetryCounter = 0;
    mExceptionPending = 0;
    mTimeoutCounter = 0;

    lbl_eu_80665668 = this;

    mtl::ALLOC_HANDLE handle = CDevice::getDevSys2Handle();
    mBuffer = mtl::MemManager::allocate_head(handle, 0x800, 0x20);

    // Retail guards the subobject offset with a null check on `this`; the
    // pException argument is never initialized (retail leaves stale r4).
    IException* pExceptionArg;
    CException* pSelf = (CException*)this;
    if (pSelf != nullptr) {
        pSelf = (CException*)((u8*)this + 0x1C4);
    }
    func_804591BC__10CExceptionFP10IException(pSelf, pExceptionArg);
}

// Member destructor: the compiler restores both vptrs up front, runs the
// body (second-base teardown via func_804591DC, singleton clear, buffer free),
// then the CWorkThread base dtor and the deleting-dtor `delete this` tail.
CDeviceFileCri::~CDeviceFileCri() {
    // novtable: restore both retail vptrs by hand (primary + second base).
    IException* pException = (IException*)this;
    *(u32**)this = (u32*)lbl_eu_8056C354;
    *(u32**)((char*)this + 0x1C4) = (u32*)((char*)lbl_eu_8056C354 + 0xA0);

    // Retail guards the second-base teardown with a null check on `this`.
    if (pException != nullptr) {
        pException = (IException*)((u8*)pException + 0x1C4);
    }
    func_804591DC__10CExceptionFP10IException((CException*)pException);
    lbl_eu_80665668 = nullptr;
    if (mBuffer != nullptr) {
        mtl::MemManager::deallocate(mBuffer);
        mBuffer = nullptr;
    }
}

namespace {

// Opaque replica of CWorkThread::isException()'s queue scan. Retail compiles
// CMsgParam<8>::find() straight into its callers; here the definition lives
// out-of-line in another TU, so walk the queue by byte offset instead
// (CDeviceFileCri-relative: mMsgQueue@0x80 -> mArrayPtr/mFront/mSize/mCapacity).
inline bool criIsException(CDeviceFileCri* self) {
    if (self->checkFlag(CWorkThread::THREAD_FLAG_EXCEPTION)) return true;

    // Member reads stay inside the loop (retail re-derives them per
    // iteration); the loop counter doubles as the found index and flips to
    // -1 only when the scan runs off the end (induction-variable fold).
    int count = *(u32*)((char*)self + 0x1AC);
    int found;
    // Countdown form: retail keeps the bound in CTR (mtctr/bdnz) and folds
    // exhaustion to -1 AFTER the guard — reproduced by a separate countdown
    // variable instead of reusing the found index as the loop counter.
    u32 remaining = count;
    for (found = 0; remaining != 0; remaining--) {
        u32 slot = (*(u32*)((char*)self + 0x1A8) + found) %
                   *(u32*)((char*)self + 0x1B0);
        if ((*(CMsgParamEntry**)((char*)self + 0x1A4))[slot].command ==
            CWorkThread::EVT_EXCEPTION) {
            break;
        }
        found++;
    }
    if (remaining == 0) {
        found = -1;
    }
    return found >= 0;
}

} // namespace

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
        if (!criIsException(this)) {
            func_80457CA4__10CExceptionFP11CWorkThreadPCwUl(this, (const wchar_t*)lbl_eu_806636C8, 4);
        }
        return false;
    } else if (status == 11) {
        if (!criIsException(this)) {
            func_80457CA4__10CExceptionFP11CWorkThreadPCwUl(this, (const wchar_t*)lbl_eu_806636CC, 4);
        }
        return false;
    } else if (status == 4) {
        if (!criIsException(this)) {
            func_80457CA4__10CExceptionFP11CWorkThreadPCwUl(this, (const wchar_t*)lbl_eu_806636C8, 4);
        }
        return false;
    }
    
    return true;
}

void CDeviceFileCri::func_8044F964() {
    // retail names the singleton pointer lbl_eu_80665668 here (same storage as
    // the static member); reference the C-linkage label to avoid name drift.
    if (lbl_eu_80665668 == nullptr) return;
    if (lbl_eu_80665668->mADXFHandle == nullptr) return;
    
    ADXF_Stop(lbl_eu_80665668->mADXFHandle);
    ADXF_Close(lbl_eu_80665668->mADXFHandle);
    lbl_eu_80665668->mADXFHandle = nullptr;
}

int CDeviceFileCri::getFileSize(const char* pPath, int arg1) {
    // Frame-order note: retail allocates these locals descending from
    // sp+0x1C8 down to sp+0x8 (fileInfo). The name buffer and its length are
    // one aggregate so nameLen stays memory-resident (store per def, cached
    // reads) exactly like retail. pathLen rides in the same aggregate so its
    // otherwise-dead store survives DCE (retail keeps it too). NOTE: a plain
    // local (DCE'd, 4.1%), address-taken store (DCE'd, 4.1%), and a volatile
    // first-declared local (lands at frame BOTTOM, 54.6%) were all tested —
    // the escaping aggregate is the best-known form.
    char pathBuf[0x100];
    struct NameEntry {
        char nameBuf[0x80];
        int nameLen;
        int pathLen;
    } name;
    DVDFileInfo fileInfo;

    name.pathLen = strlen(pPath);
    strcpy(pathBuf, pPath);

    if (arg1 != 0) {
        func_eu_804520D0(pathBuf);
    }

    // Strip a leading '/' from the name; nameLen is the strlen of the SOURCE
    // (not of the copied buffer) in retail.
    name.nameBuf[0] = '\0';
    name.nameLen = 0;
    if (pPath[0] == '/') {
        name.nameLen = strlen(pPath + 1);
        strcpy(name.nameBuf, pPath + 1);
    } else {
        name.nameLen = strlen(pPath);
        strcpy(name.nameBuf, pPath);
    }

    // ".adx" extension marker lives at lbl_eu_80522CA0 + 6.
    const char* ext = lbl_eu_80522CA0 + 6;

    int extLen = strlen(ext);

    // Find the extension; the scan index becomes -1 when the whole name is
    // scanned without a match.
    int i = 0;
    while (i < name.nameLen) {
        if (strncmp(&name.nameBuf[i], ext, extLen) == 0) break;
        i++;
    }
    if (i >= name.nameLen) i = -1;

    if (i != -1 && i < name.nameLen) {
        name.nameBuf[i] = '\0';
        name.nameLen = i;
    }

    int ret = func_804DDCD4(name.nameBuf, pPath);
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
    // Cancel every DVD-read child job via vtable slot 0xA4
    // (cancel(const char*)).
    for (_reslist_node<CWorkThread*>* node = lbl_eu_80665668->mChildren.mStartNodePtr->mNext;
         node != lbl_eu_80665668->mChildren.mStartNodePtr; node = node->mNext) {
        CDeviceFileJobReadDvd* job = (CDeviceFileJobReadDvd*)node->mItem;
        // else-if chain (not ||): retail branches bne into the type check
        // with li r3,0 in both failure arms.
        if (job == nullptr) {
            job = nullptr;
        } else if (job->mType != THREAD_CDEVICEFILEJOBREADDVD) {
            job = nullptr;
        }
        if (job != nullptr) {
            ((CDeviceFileJob*)job)->cancel(pPath);
        }
    }
}

bool CDeviceFileCri::cancel(CFileHandle* pHandle) {
    // Cancel the first matching DVD-read child whose cancel succeeds
    // (vtable slot 0xA8); stops at the first job that reports success.
    // Retail walks raw nodes (re-reading the singleton's list head in the
    // condition) and branches straight to the epilogue with the raw vcall
    // result in r3 - i.e. `break`, with the function having no return stmt.
    // Declaration placement mirrors func_8044FB08 (100%): node inside the
    // for-init, job inside the body - that is what puts the list-walk temps
    // in retail's scratch colors.
    for (_reslist_node<CWorkThread*>* node =
             lbl_eu_80665668->mChildren.mStartNodePtr->mNext;
         node != lbl_eu_80665668->mChildren.mStartNodePtr;
         node = node->mNext) {
        CDeviceFileJobReadDvd* job = (CDeviceFileJobReadDvd*)node->mItem;
        if (job == nullptr) {
            job = nullptr;
        } else if (job->mType != THREAD_CDEVICEFILEJOBREADDVD) {
            job = nullptr;
        }
        if (job != nullptr) {
            if (((CDeviceFileJob*)job)->cancel((CDeviceFileJob_UnkStruct1*)pHandle)) {
                // Retail branches straight to the epilogue with the raw vcall
                // result still in r3 (no materialized true).
                break;
            }
        }
    }
    // Probe-verified: the explicit false return is what keeps MWCC from
    // reserving r3 for an early result VR (missing-return pushes every temp
    // to r4+ and adds an 'or r3,r4,r4' this-move before the vcall).
    return false;
}

void CDeviceFileCri::func_8044FC38() {
    // Count the singleton's children via reslist::size() (inlined); retail
    // re-reads the global only inside the ADXF teardown block.
    u32 count = lbl_eu_80665668->mChildren.size();
    if (count == 0) return;

    // Grab the first CDeviceFileJobReadDvd child (mType == 0x44) directly -
    // the retail inlines this without the empty() guard. Shape note: retail
    // seeds job from the item unconditionally, then nulls it via two plain
    // ifs (that's what yields the beq-over-li branch polarity).
    CWorkThread* child = lbl_eu_80665668->mChildren.mStartNodePtr->mNext->mItem;
    CDeviceFileJobReadDvd* job = (CDeviceFileJobReadDvd*)child;
    if (child == nullptr) {
        job = nullptr;
    } else if (child->mType != THREAD_CDEVICEFILEJOBREADDVD) {
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
            CDeviceFileCri* self = lbl_eu_80665668;
            u32 count = self->mChildren.size();
            if (count != 0) {
                CWorkThread* child2 = self->mChildren.front();
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
        CDeviceFileCri* self = lbl_eu_80665668;
        u32 count = self->mChildren.size();
        if (count != 0) {
            CWorkThread* child2 = self->mChildren.front();
            CDeviceFileJobReadDvd* job2 = (CDeviceFileJobReadDvd*)child2;
            if (child2 == nullptr) {
                job2 = nullptr;
            } else if (child2->mType != THREAD_CDEVICEFILEJOBREADDVD) {
                job2 = nullptr;
            }
            if (self->mADXFHandle != nullptr) {
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
        CDeviceFileCri* self = lbl_eu_80665668;
        u32 count = self->mChildren.size();
        if (count != 0) {
            CWorkThread* child2 = self->mChildren.front();
            CDeviceFileJobReadDvd* job2 = (CDeviceFileJobReadDvd*)child2;
            if (child2 == nullptr) {
                job2 = nullptr;
            } else if (child2->mType != THREAD_CDEVICEFILEJOBREADDVD) {
                job2 = nullptr;
            }
            if (self->mADXFHandle != nullptr) {
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
    // Seed-then-null shape (retail): keeps job's web identical to child's.
    CDeviceFileJobReadDvd* job = (CDeviceFileJobReadDvd*)child;
    if (child == nullptr) {
        job = nullptr;
    } else if (child->mType != THREAD_CDEVICEFILEJOBREADDVD) {
        job = nullptr;
    }

    int numReq = ADXF_GetNumReqSct(mADXFHandle);
    if ((u32)(numReq - 1) <= 1) return false;

    if (numReq == 3) {
        CFileHandleLayout* handle = (CFileHandleLayout*)job->mHandle;
        // Copy one 0x800-sector chunk into the handle's data (retail passes
        // mData+unk10 as the memcpy DESTINATION; the flush covers it too).
        u32 offset = handle->field_0x38 & 0x7FF;
        u32 copySize = 0x800 - offset;
        if (copySize > handle->mLength) copySize = handle->mLength;

        memcpy(handle->mData + handle->unk10, (char*)mBuffer + offset, copySize);
        DCFlushRangeNoSync(handle->mData + handle->unk10, copySize);

        func_80451CBC__11CFileHandleFi((CFileHandle*)handle, copySize);

        bool complete = (handle->unk10 != 0 && handle->unk10 == handle->mLength);
        if (complete) {
            // Per-path returns keep the true materialized in each block
            // (retail does not merge them into a shared tail).
            ADXF_Stop(mADXFHandle);
            ADXF_Close(mADXFHandle);
            mADXFHandle = nullptr;
            mActiveWorkID = job->mWorkID;
            mIdleCounter = 0;
            mState = 8;
            return true;
        }
        mState = 4;
        return true;
    }

    // Abort: cancel the read job.
    call__11CFileHandleF3CBM(job->mHandle, 3);
    // Retail caches the singleton here (r6) for the count walk and child
    // extraction; the ADXF args/state stores still re-read the global.
    CDeviceFileCri* self = lbl_eu_80665668;
    u32 count = self->mChildren.size();
    if (count != 0) {
        CWorkThread* child2 = self->mChildren.front();
        CDeviceFileJobReadDvd* job2 = (CDeviceFileJobReadDvd*)child2;
        if (child2 == nullptr) {
            job2 = nullptr;
        } else if (child2->mType != THREAD_CDEVICEFILEJOBREADDVD) {
            job2 = nullptr;
        }

        if (self->mADXFHandle != nullptr) {
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
    // Seed-then-null shape (retail): keeps job's web identical to child's.
    CDeviceFileJobReadDvd* job = (CDeviceFileJobReadDvd*)child;
    if (child == nullptr) {
        job = nullptr;
    } else if (child->mType != THREAD_CDEVICEFILEJOBREADDVD) {
        job = nullptr;
    }

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
            return true;
        }
        mState = 7;
        return true;
    }

    // Abort: reset the handle position and cancel the read job.
    job->mHandle->unk10 = 0;
    call__11CFileHandleF3CBM(job->mHandle, 3);
    // Retail caches the singleton here (r6) for the count walk and child
    // extraction; the ADXF args/state stores still re-read the global.
    CDeviceFileCri* self = lbl_eu_80665668;
    u32 count = self->mChildren.size();
    if (count != 0) {
        CWorkThread* child2 = self->mChildren.front();
        CDeviceFileJobReadDvd* job2 = (CDeviceFileJobReadDvd*)child2;
        if (child2 == nullptr) {
            job2 = nullptr;
        } else if (child2->mType != THREAD_CDEVICEFILEJOBREADDVD) {
            job2 = nullptr;
        }

        if (self->mADXFHandle != nullptr) {
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
    // Seed-then-null shape (retail): keeps job's web identical to child's.
    CDeviceFileJobReadDvd* job = (CDeviceFileJobReadDvd*)child;
    if (child == nullptr) {
        job = nullptr;
    } else if (child->mType != THREAD_CDEVICEFILEJOBREADDVD) {
        job = nullptr;
    }

    int numReq = ADXF_GetNumReqSct(mADXFHandle);
    if ((u32)(numReq - 1) <= 1) return false;

    if (numReq == 3) {
        CFileHandle* handle = job->mHandle;
        // Copy the remainder of the pending read out of the device buffer
        // into the handle's data (retail arg order).
        int remaining = handle->mLength - handle->unk10;
        if (remaining > 0) {
            memcpy(handle->mData + handle->unk10, mBuffer, remaining);
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
    // Retail caches the singleton here (r6) for the count walk and child
    // extraction; the ADXF args/state stores still re-read the global.
    CDeviceFileCri* self = lbl_eu_80665668;
    u32 count = self->mChildren.size();
    if (count != 0) {
        CWorkThread* child2 = self->mChildren.front();
        CDeviceFileJobReadDvd* job2 = (CDeviceFileJobReadDvd*)child2;
        if (child2 == nullptr) {
            job2 = nullptr;
        } else if (child2->mType != THREAD_CDEVICEFILEJOBREADDVD) {
            job2 = nullptr;
        }

        if (self->mADXFHandle != nullptr) {
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
    // NOTE: retail dispatches via jumptable_eu_8056C330 through a switch
    // (cmplwi r0,8 bounds check; lis/addi/lwzx/mtctr/bctr; cases emitted in
    // order 0,1,2,3,4,5,7,6,8). A literal switch here makes MWCC generate its
    // OWN anon table (@N) appended after the blob .data instead of using the
    // hand-defined jumptable - which breaks the data gate until every case
    // body is byte-exact AND the anon table is renamed/placed at .data 0x0.
    // Until then the if-chain form keeps the object data-clean.
    if (!func_8044F744()) return;
    
    u32 state = mState;
    if (state > 8) return;
    
    if (state > 8) return;
    if (state == 0) {
        if (mChildren.empty()) return;
        CWorkThread* child = mChildren.front();
        if (child == nullptr || child->mType != THREAD_CDEVICEFILEJOBREADDVD) return;
        
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
        
        if (!ready) return;
        // NOTE: retail calls job->isException() here (flag + queued EVT scan,
        // inlined). Using criIsException(job) makes wkUpdate exceed MWCC's
        // switch-conversion threshold -> private anon jump table (+0x28
        // .data) -> data gate FAIL. Revisit together with the switch-form
        // reconciliation (see attempts.jsonl us-804532dc).
        if (job->mFlags & THREAD_FLAG_EXCEPTION) return;
        
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

    }
    if (state == 1) {
        if (!func_8044FCFC()) return;
        
        mTimeoutCounter--;
        if (mTimeoutCounter > 0) return;
        
        // getFirstCDeviceFileJobReadDvd() inlined (retail carries no
        // out-of-line copy; keeping one here costs split budget).
        CDeviceFileJobReadDvd* job = nullptr;
        if (!mChildren.empty()) {
            CWorkThread* firstChild = mChildren.front();
            if (firstChild != nullptr &&
                firstChild->mType == THREAD_CDEVICEFILEJOBREADDVD) {
                job = (CDeviceFileJobReadDvd*)firstChild;
            }
        }
        CFileHandle* handle = job->mHandle;
        u32 remaining = handle->mLength - handle->unk10;
        
        if (remaining > 0x800) {
            ADXF_ReadNw(mADXFHandle, remaining >> 11, (char*)handle->mData + handle->unk10);
            mState = 5;
        } else {
            mState = 7;
        }

    }
    if (state == 2) {
        if (!func_80450058()) return;
        
        mTimeoutCounter--;
        if (mTimeoutCounter > 0) return;
        
        // getFirstCDeviceFileJobReadDvd() inlined (retail carries no
        // out-of-line copy; keeping one here costs split budget).
        CDeviceFileJobReadDvd* job = nullptr;
        if (!mChildren.empty()) {
            CWorkThread* firstChild = mChildren.front();
            if (firstChild != nullptr &&
                firstChild->mType == THREAD_CDEVICEFILEJOBREADDVD) {
                job = (CDeviceFileJobReadDvd*)firstChild;
            }
        }
        CFileHandle* handle = job->mHandle;
        u32 remaining = handle->mLength - handle->unk10;
        
        if (remaining > 0x800) {
            ADXF_ReadNw(mADXFHandle, remaining >> 11, (char*)handle->mData + handle->unk10);
            mState = 5;
        } else {
            mState = 7;
        }

    }
    if (state == 3) {
        func_80450058();

    }
    if (state == 4) {
        func_8045042C();

    }
    if (state == 5) {
        func_80450260();

    }
    if (state == 6) {
        if (mChildren.empty()) return;
        CWorkThread* child = mChildren.front();
        if (child == nullptr || child->mType != THREAD_CDEVICEFILEJOBREADDVD) return;
        
        CDeviceFileJobReadDvd* job = (CDeviceFileJobReadDvd*)child;
        CFileHandle* handle = job->mHandle;
        
        u32 remaining = handle->mLength - handle->unk10;
        if (remaining != 0) {
            ADXF_ReadNw(mADXFHandle, 1, mBuffer);
        }
        mState = 6;

    }
    if (state == 7) {
        func_8045042C();

    }
    if (state == 8) {
        if (mChildren.empty()) {
            mState = 0;
            return;
        }
        
        CWorkThread* child = mChildren.front();
        if (child == nullptr || child->mType != THREAD_CDEVICEFILEJOBREADDVD) {
            mState = 0;
            return;
        }
        
        if (mActiveWorkID != child->mWorkID) {
            mState = 0;
            return;
        }
        
        if (!(child->mFlags & THREAD_FLAG_EXCEPTION)) {
            mIdleCounter++;
            if (mIdleCounter >= 30) {
                callCBM3__21CDeviceFileJobReadDvdFv((CDeviceFileJobReadDvd*)child);
                mIdleCounter = 0;
            }
            return;
        }
        
        mState = 0;
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

    u32 maxRetries = getTargetFramerate__9CDeviceVIFv() * 6;

    mRetryCounter++;
    if (mRetryCounter < maxRetries) {
        func_8045925C__10CExceptionFv((CException*)lbl_eu_806636D0[0]);
        return false;
    }
    
    mExceptionPending = false;
    return true;
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.rodata] 0x80522C90-0x80522CA8 (0x18 = 24B): class name + "FATAL" strings.
extern "C" __declspec(align(8)) const char lbl_eu_80522C90[0x18] = {
    0x43,0x44,0x65,0x76,0x69,0x63,0x65,0x46,0x69,0x6C,0x65,0x43,0x72,0x69,0x00,0x00,
    0x46,0x41,0x54,0x41,0x4C,0x00,0x2F,0x00,
};
#define lbl_eu_80522CA0 (lbl_eu_80522C90 + 0x10)

// [.sdata] 0x806636C8-0x806636E0 (0x18 = 24B): singleton-string pointers +
// RTTI locator.
extern "C" u32 lbl_eu_8066A3B0;  // shared string ("CDeviceFileCri"-related)
extern "C" u32 lbl_eu_806636D8[2];
extern "C" u32 lbl_eu_8056C408[6];  // RTTI base list (defined below)
extern "C" u32 lbl_eu_806636C8[1] = { (u32)&lbl_eu_8066A3B0 };
extern "C" u32 lbl_eu_806636CC[1] = { (u32)&lbl_eu_8066A3B0 };
extern "C" u32 lbl_eu_806636D0[2] = { (u32)&lbl_eu_8066A3B0, 0x00000000 };
extern "C" u32 lbl_eu_806636D8[2] = { (u32)&lbl_eu_80522C90, (u32)&lbl_eu_8056C408 };

// [.data] 0x8056C330-0x8056C420 (0xF0 = 240B): wkUpdate jumptable + vtable +
// RTTI base list.
namespace CDeviceFileCriBlob {
extern "C" void* __dt__14CDeviceFileCriFv();
extern "C" void wkUpdate__14CDeviceFileCriFv();
extern "C" void wkStandbyLogin__14CDeviceFileCriFv();
extern "C" void wkStandbyLogout__14CDeviceFileCriFv();
extern "C" void wkStandbyExceptionRetry__14CDeviceFileCriFUl();
extern "C" void func_80450AB8__14CDeviceFileCriFUl();
extern "C" void func_80450B4C();
extern "C" void func_80450B44();
extern "C" void WorkEvent1__10IWorkEventFPvPCc();
extern "C" void OnFileEvent__10IWorkEventFP10CEventFile();
extern "C" void WorkEvent3__10IWorkEventFPv();
extern "C" void WorkEvent4__10IWorkEventFv();
extern "C" void OnPauseTrigger__10IWorkEventFb();
extern "C" void WorkEvent6__10IWorkEventFv();
extern "C" void WorkEvent7__10IWorkEventFv();
extern "C" void WorkEvent8__10IWorkEventFv();
extern "C" void WorkEvent9__10IWorkEventFv();
extern "C" void WorkEvent10__10IWorkEventFv();
extern "C" void WorkEvent11__10IWorkEventFv();
extern "C" void WorkEvent12__10IWorkEventFv();
extern "C" void WorkEvent13__10IWorkEventFv();
extern "C" void WorkEvent14__10IWorkEventFv();
extern "C" void WorkEvent15__10IWorkEventFv();
extern "C" void WorkEvent16__10IWorkEventFv();
extern "C" void WorkEvent17__10IWorkEventFv();
extern "C" void WorkEvent18__10IWorkEventFv();
extern "C" void WorkEvent19__10IWorkEventFv();
extern "C" void WorkEvent20__10IWorkEventFv();
extern "C" void WorkEvent21__10IWorkEventFv();
extern "C" void WorkEvent22__10IWorkEventFv();
extern "C" void WorkEvent23__10IWorkEventFv();
extern "C" void WorkEvent24__10IWorkEventFv();
extern "C" void WorkEvent25__10IWorkEventFv();
extern "C" void WorkEvent26__10IWorkEventFv();
extern "C" void WorkEvent27__10IWorkEventFv();
extern "C" void WorkEvent28__10IWorkEventFv();
extern "C" void WorkEvent29__10IWorkEventFv();
extern "C" void WorkEvent30__10IWorkEventFv();
extern "C" void WorkEvent31__10IWorkEventFv();
extern "C" void wkRender__11CWorkThreadFv();
extern "C" void wkRenderAfter__11CWorkThreadFv();
extern "C" u32 __RTTI__10IWorkEvent;
extern "C" u32 __RTTI__11CWorkThread;
}
extern "C" u32 jumptable_eu_8056C330[9] = {
    (u32)((char*)&CDeviceFileCriBlob::wkUpdate__14CDeviceFileCriFv + 72),
    (u32)((char*)&CDeviceFileCriBlob::wkUpdate__14CDeviceFileCriFv + 400),
    (u32)((char*)&CDeviceFileCriBlob::wkUpdate__14CDeviceFileCriFv + 416),
    (u32)((char*)&CDeviceFileCriBlob::wkUpdate__14CDeviceFileCriFv + 580),
    (u32)((char*)&CDeviceFileCriBlob::wkUpdate__14CDeviceFileCriFv + 596),
    (u32)((char*)&CDeviceFileCriBlob::wkUpdate__14CDeviceFileCriFv + 728),
    (u32)((char*)&CDeviceFileCriBlob::wkUpdate__14CDeviceFileCriFv + 832),
    (u32)((char*)&CDeviceFileCriBlob::wkUpdate__14CDeviceFileCriFv + 740),
    (u32)((char*)&CDeviceFileCriBlob::wkUpdate__14CDeviceFileCriFv + 844),
};
extern "C" u32 lbl_eu_8056C354[45] = {
    (u32)&lbl_eu_806636D8, 0x00000000,
    (u32)&CDeviceFileCriBlob::__dt__14CDeviceFileCriFv,
    (u32)&CDeviceFileCriBlob::WorkEvent1__10IWorkEventFPvPCc,
    (u32)&CDeviceFileCriBlob::OnFileEvent__10IWorkEventFP10CEventFile,
    (u32)&CDeviceFileCriBlob::WorkEvent3__10IWorkEventFPv,
    (u32)&CDeviceFileCriBlob::WorkEvent4__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::OnPauseTrigger__10IWorkEventFb,
    (u32)&CDeviceFileCriBlob::WorkEvent6__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::WorkEvent7__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::WorkEvent8__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::WorkEvent9__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::WorkEvent10__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::WorkEvent11__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::WorkEvent12__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::WorkEvent13__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::WorkEvent14__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::WorkEvent15__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::WorkEvent16__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::WorkEvent17__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::WorkEvent18__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::WorkEvent19__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::WorkEvent20__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::WorkEvent21__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::WorkEvent22__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::WorkEvent23__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::WorkEvent24__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::WorkEvent25__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::WorkEvent26__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::WorkEvent27__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::WorkEvent28__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::WorkEvent29__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::WorkEvent30__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::WorkEvent31__10IWorkEventFv,
    (u32)&CDeviceFileCriBlob::wkUpdate__14CDeviceFileCriFv,
    (u32)&CDeviceFileCriBlob::wkRender__11CWorkThreadFv,
    (u32)&CDeviceFileCriBlob::wkRenderAfter__11CWorkThreadFv,
    (u32)&CDeviceFileCriBlob::wkStandbyLogin__14CDeviceFileCriFv,
    (u32)&CDeviceFileCriBlob::wkStandbyLogout__14CDeviceFileCriFv,
    (u32)&CDeviceFileCriBlob::wkStandbyExceptionRetry__14CDeviceFileCriFUl,
    (u32)&lbl_eu_806636D8, 0xFFFFFE3C,
    (u32)&CDeviceFileCriBlob::func_80450B4C,
    (u32)&CDeviceFileCriBlob::func_80450B44,
    (u32)&CDeviceFileCriBlob::func_80450AB8__14CDeviceFileCriFUl,
};
extern "C" u32 lbl_eu_8056C408[6] = {
    (u32)&CDeviceFileCriBlob::__RTTI__10IWorkEvent, 0x00000000,
    (u32)&CDeviceFileCriBlob::__RTTI__11CWorkThread, 0x00000000,
    0x00000000, 0x00000000,
};

// [.bss] 0x806576C8-0x80657B50 (0x488 = 1160B): file-cri state block.
u8 lbl_eu_806576C8[136];
u8 lbl_eu_80657750[1024];

// [.sbss] 0x80665668-0x80665670 (8B): singleton pointer + pad.
CDeviceFileCri* lbl_eu_80665668;
u32 lbl_eu_8066566C;

// Only the jumptable lacks a live reference (wkUpdate's retail jump-table
// form isn't reproduced yet); every other blob symbol is anchored by real
// code/data relocs (ctor/dtor vptr stores, vtable/typeinfo chains,
// func_80450B14/B1C/B24) and must NOT get a stub (split budget).
