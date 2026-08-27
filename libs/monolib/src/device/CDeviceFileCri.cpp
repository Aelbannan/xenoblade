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
    volatile int unk10;   //0x10 (volatile: retail re-reads the field from
                          // memory at every use instead of caching it)
    u8 field_0x14[0x24];  //0x14
    u32 field_0x38;       //0x38 current read offset
    u32 mLength;          //0x3C
    u8 field_0x40[0x18];  //0x40
    u32 field_0x58;       //0x58 flags (bit 3 = allocation/read pending)
    char nameShort[0x128]; //0x5C inline short-name buffer
    char nameLong[0x20];   //0x184 long-name buffer
    u32 unk1A4;            //0x1A4 name selector (0 -> use short name)
};

// Retail destroy__11CFileHandleFv really takes (handle, size, allocParam,
// blockSize) - the header declares it with only the handle. Declaring the
// full signature here (C linkage ignores the namespace for the symbol name)
// makes the call site emit the same four argument registers as retail.
namespace {
extern "C" void destroy__11CFileHandleFv(CFileHandle* handle, u32 size, u32 allocParam, u32 blockSize);
}

CDeviceFileCri* CDeviceFileCri::getInstance() { return sInstance; }

void CDeviceFileCri::setCriFilePathA(const wchar_t* pData) { *(wchar_t**)&lbl_eu_806636C8[0] = (wchar_t*)pData; }
void CDeviceFileCri::setCriFilePathB(const wchar_t* pData) { *(wchar_t**)&lbl_eu_806636CC[0] = (wchar_t*)pData; }
void CDeviceFileCri::setCriFilePathC(const wchar_t* pData) { *(wchar_t**)&lbl_eu_806636D0[0] = (wchar_t*)pData; }

extern "C" void sinit_80450B2C() {
    lbl_eu_806576C8[0] = 0;
    *(u32*)(lbl_eu_806576C8 + 0x80) = 0;
}

void CDeviceFileCri::teardownAdxf(unsigned long) {
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
    checkDriveStatus__14CDeviceFileCriFv((CDeviceFileCri*)((char*)self - 0x1C4));
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

// Replica of CWorkThread::isException() (retail inlines both the flag test
// and CMsgParam<8>::find() here; find's out-of-line copy lives in CWorkRoot),
// walking the queue by byte offset
// (CDeviceFileCri-relative: mMsgQueue@0x80 -> mArrayPtr/mFront/mSize/mCapacity).
// Loop shape mirrors CMsgParam<N>::find's return-i / return-minus-one body:
// that is what makes MWCC drive the trip count through CTR (mtctr/bdnz) and
// fold exhaustion into the shared 'li r6,-1' tail.
inline int criFindException(CDeviceFileCri* self) {
    for (int i = 0; (u32)i < *(u32*)((char*)self + 0x1AC); i++) {
        u32 slot = (*(u32*)((char*)self + 0x1A8) + i) %
                   *(u32*)((char*)self + 0x1B0);
        if ((*(CMsgParamEntry**)((char*)self + 0x1A4))[slot].command ==
            CWorkThread::EVT_EXCEPTION) {
            return i;
        }
    }
    return -1;
}

inline bool criIsException(CDeviceFileCri* self) {
    // Raw bitwise-and test: retail branches on the record form of the
    // extract (rlwinm.) rather than a normalized bool.
    if (self->mFlags & CWorkThread::THREAD_FLAG_EXCEPTION) {
        return true;
    }
    // Sign-bit test (srwi/xori in retail): found >= 0.
    return criFindException(self) >= 0;
}

} // namespace

bool CDeviceFileCri::checkDriveStatus() {
    if (isOff__11CWorkSystemFv()) return true;
    if (!func_eu_804521C4()) return true;
    
    int status = DVDGetDriveStatus();

    if (status == -1) {
        // Fatal DVD error: raise the global exception handler, then fall
        // through to the shared "keep running" tail below.
        logExceptionMessage__10CExceptionFv(lbl_eu_80522CA0);
    } else if (status == 6) {
        // Recoverable DVD errors: only surface the message when an exception
        // is not already pending (flag or queued EVT_EXCEPTION), then stay
        // busy. The message pointers are static pointer VARIABLES (.sdata) -
        // retail loads their CONTENTS (lwz @sda21), not their addresses.
        if (!criIsException(this)) {
            func_80457CA4__10CExceptionFP11CWorkThreadPCwUl(this, (const wchar_t*)lbl_eu_806636C8[0], 4);
        }
        return false;
    } else if (status == 11) {
        if (!criIsException(this)) {
            func_80457CA4__10CExceptionFP11CWorkThreadPCwUl(this, (const wchar_t*)lbl_eu_806636CC[0], 4);
        }
        return false;
    } else if (status == 4) {
        if (!criIsException(this)) {
            func_80457CA4__10CExceptionFP11CWorkThreadPCwUl(this, (const wchar_t*)lbl_eu_806636C8[0], 4);
        }
        return false;
    }

    return true;
}

void CDeviceFileCri::closeAdxfHandle() {
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
    // sp+0x1C8 down to sp+0x8 (fileInfo). pathLen's store is otherwise dead,
    // so it rides in one aggregate with pathBuf (whose address escapes into
    // strcpy, keeping the store alive); declaring the aggregate first places
    // pathLen@0x1C8 above pathBuf@0xC8 exactly like retail.
    struct PathEntry {
        char pathBuf[0x100];
        int pathLen;
    } path;
    // Same trick for the stripped name: one escaping aggregate keeps nameLen
    // memory-resident (store per def) like retail; declared first among the
    // remaining locals it lands at nameBuf@0x44 / nameLen@0xC4.
    struct NameEntry {
        char nameBuf[0x80];
        int nameLen;
    } name;
    DVDFileInfo fileInfo;

    path.pathLen = strlen(pPath);
    strcpy(path.pathBuf, pPath);

    if (arg1 != 0) {
        func_eu_804520D0(path.pathBuf);
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
    // Declaration order drives MWCC's register assignment here: ext->r31,
    // i->r30, extLen->r29, then the scan pointer ->r28 and cached length
    // ->r27, matching retail.
    const char* ext = lbl_eu_80522CA0 + 6;
    int i = 0;
    int extLen = strlen(ext);
    char* pName = name.nameBuf;
    int curLen = name.nameLen;

    // Find the extension; the scan index becomes -1 when the whole name is
    // scanned without a match. MWCC hoists name.nameLen into a register for
    // the loop condition and re-reads it after the loop.
    int c;
    // Find the extension; the scan index becomes -1 when the whole name is
    // scanned without a match.
    while (i < curLen) {
        c = strncmp(pName, ext, extLen);
        if (c != 0) {
            pName++;
            i++;
            continue;
        }
        break;
    }
    if (i >= curLen) i = -1;

    if (i != -1 && i < curLen) {
        name.nameBuf[i] = '\0';
        name.nameLen = i;
    }

    int ret = func_804DDCD4(name.nameBuf, pPath);
    if (ret > -1) return ret;

    int entrynum = DVDConvertPathToEntrynum(path.pathBuf);
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

void CDeviceFileCri::cancelJobsForPath(const char* pPath) {
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
    // condition). Declaration placement mirrors cancelJobsForPath (100%): node
    // inside the for-init, job inside the body.
    //
    // KNOWN RESIDUAL (wall-class): retail branches straight to the epilogue
    // with the raw vcall result in r3 - the function effectively falls off
    // the end without materializing a return value (returns garbage, like
    // sjmem_PutChunk). Every reachable `return false` adds a dead 'li r3,0'
    // (+4 bytes), and every missing-return spelling pins r3 as the implicit
    // result VR from entry, shifting all temps (item lands in r4 behind an
    // 'or r3,r4,r4' this-move). Best-known draft keeps the explicit return.
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
                break;
            }
        }
    }
    return false;
}

void CDeviceFileCri::func_8044FC38() {
    // Count children by walking the intrusive node chain directly, with the
    // loop check at the bottom (retail branches straight into the cmplw).
    // Declarations left uninitialized and ordered so MWCC's allocator maps
    // them to retail's registers (node->r3, count->r4, head->r5, self->r6).
    u32 count = 0;
    CDeviceFileCri* self = lbl_eu_80665668;
    _reslist_node<CWorkThread*>* head = self->mChildren.mStartNodePtr;
    _reslist_node<CWorkThread*>* node = head->mNext;
    while (head != node) {
        node = node->mNext;
        count++;
    }
    if (count == 0) return;

    // First child: seed job from the item unconditionally, then null it via
    // two plain ifs (that's what yields retail's beq-over-li polarity).
    CWorkThread* child = self->mChildren.mStartNodePtr->mNext->mItem;
    CDeviceFileJobReadDvd* job = (CDeviceFileJobReadDvd*)child;
    if (child == nullptr) {
        job = nullptr;
    } else if (child->mType != THREAD_CDEVICEFILEJOBREADDVD) {
        job = nullptr;
    }

    // Teardown re-reads the global between every call (the callee may mutate
    // the singleton pointer, so MWCC cannot keep the cached copy live).
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
    // Inline getFirstCDeviceFileJobReadDvd (seed-then-null shape).
    CDeviceFileJobReadDvd* job = (CDeviceFileJobReadDvd*)mChildren.front();
    if (job == nullptr) {
        job = nullptr;
    } else if (job->mType != THREAD_CDEVICEFILEJOBREADDVD) {
        job = nullptr;
    }
    if (job == nullptr) return false;

    CFileHandle* handle = job->mHandle;

    if (ADXF_IsOpened(mADXFHandle) == 0) {
        if (ADXF_GetNumReqSct(mADXFHandle) == 4) {
            // Abort: cancel the handle, then tear down ADXF state + job
            // (retail re-inlines this block per site).
            call__11CFileHandleF3CBM(handle, 3);
            CDeviceFileCri* self = lbl_eu_80665668;
            u32 count = self->mChildren.size();
            if (count != 0) {
                CDeviceFileJobReadDvd* job2 = (CDeviceFileJobReadDvd*)self->mChildren.front();
                if (job2 == nullptr) {
                    job2 = nullptr;
                } else if (job2->mType != THREAD_CDEVICEFILEJOBREADDVD) {
                    job2 = nullptr;
                }
                if (self->mADXFHandle != nullptr) {
                    ADXF_Stop(self->mADXFHandle);
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
    int rem = fileSize & 0x7FF;
    if (rem != 0) {
        fileSize = (fileSize + 0x800) - rem;
    }

    int readSize = fileSize;
    ADXF_GetFsizeSct(mADXFHandle);

    if (fileSize <= 0) {
        call__11CFileHandleF3CBM(handle, 3);
        CDeviceFileCri* self = lbl_eu_80665668;
        u32 count = self->mChildren.size();
        if (count != 0) {
            CDeviceFileJobReadDvd* job2 = (CDeviceFileJobReadDvd*)self->mChildren.front();
            if (job2 == nullptr) {
                job2 = nullptr;
            } else if (job2->mType != THREAD_CDEVICEFILEJOBREADDVD) {
                job2 = nullptr;
            }
            if (self->mADXFHandle != nullptr) {
                ADXF_Stop(self->mADXFHandle);
                ADXF_GetNumReqSct(lbl_eu_80665668->mADXFHandle);
                ADXF_Close(lbl_eu_80665668->mADXFHandle);
                lbl_eu_80665668->mADXFHandle = nullptr;
            }
            removeFileJob__11CDeviceFileFP14CDeviceFileJob(job2);
            lbl_eu_80665668->mState = 0;
        }
        return false;
    }

    // Clamp the read size to the file's remaining sector-aligned length.
    CFileHandleLayout* layout = (CFileHandleLayout*)handle;
    if (layout->mLength != 0) {
        readSize = layout->mLength;
    }
    if ((u32)(readSize + layout->field_0x38) > (u32)fileSize) {
        readSize -= (readSize + layout->field_0x38) - fileSize;
    }

    destroy__11CFileHandleFv(handle, readSize, 0x20, 0x800);

    if (layout->field_0x58 & 0x8) {
        // Allocation failed: cancel the handle and clean up the job.
        call__11CFileHandleF3CBM(handle, 3);
        CDeviceFileCri* self = lbl_eu_80665668;
        u32 count = self->mChildren.size();
        if (count != 0) {
            CDeviceFileJobReadDvd* job2 = (CDeviceFileJobReadDvd*)self->mChildren.front();
            if (job2 == nullptr) {
                job2 = nullptr;
            } else if (job2->mType != THREAD_CDEVICEFILEJOBREADDVD) {
                job2 = nullptr;
            }
            if (self->mADXFHandle != nullptr) {
                ADXF_Stop(self->mADXFHandle);
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
    // Seed-then-null shape (retail): extract straight into job.
    CDeviceFileJobReadDvd* job = (CDeviceFileJobReadDvd*)mChildren.front();
    if (job == nullptr) {
        job = nullptr;
    } else if (job->mType != THREAD_CDEVICEFILEJOBREADDVD) {
        job = nullptr;
    }

    int numReq = ADXF_GetNumReqSct(mADXFHandle);
    if ((u32)(numReq - 1) <= 1) return false;

    if (numReq == 3) {
        // Abort path below is branched OVER (retail bne .LE6C).
        CFileHandleLayout* handle = (CFileHandleLayout*)job->mHandle;
        // Copy one 0x800-sector chunk into the handle's data (retail passes
        // mData+unk10 as the memcpy DESTINATION; the flush covers it too).
        u32 offset = handle->field_0x38 & 0x7FF;
        u32 copySize = 0x800 - offset;
        if (copySize > handle->mLength) copySize = handle->mLength;
        // (retail: subfic 0x800-offset, ble-clamp against mLength)

        memcpy(handle->mData + handle->unk10, (char*)mBuffer + offset, copySize);
        DCFlushRangeNoSync(handle->mData + handle->unk10, copySize);

        advanceReadProgress__11CFileHandleFi((CFileHandle*)handle, copySize);

        // Retail reads unk10 ONCE here (cached in r4) - no volatile re-read.
        int pos = handle->unk10;
        bool complete = (pos != 0 && pos == handle->mLength);
        if (!complete) {
            goto notComplete; // retail keeps this tail OUT OF LINE, emitted
                              // after the whole abort block - mirror layout.
        }
        ADXF_Stop(mADXFHandle);
        ADXF_Close(mADXFHandle);
        mADXFHandle = nullptr;
        mActiveWorkID = job->mWorkID;
        mIdleCounter = 0;
        mState = 8;
        return true;
    }

    // Abort: cancel the read job.
    call__11CFileHandleF3CBM(job->mHandle, 3);
    // Retail caches the singleton here (r6); the ADXF args/state stores still
    // re-read the global between calls.
    CDeviceFileCri* self = lbl_eu_80665668;
    u32 count = self->mChildren.size();
    if (count != 0) {
        CDeviceFileJobReadDvd* job2 =
            (CDeviceFileJobReadDvd*)self->mChildren.front();
        if (job2 == nullptr) {
            job2 = nullptr;
        } else if (job2->mType != THREAD_CDEVICEFILEJOBREADDVD) {
            job2 = nullptr;
        }

        if (self->mADXFHandle != nullptr) {
            ADXF_Stop(self->mADXFHandle);
            ADXF_GetNumReqSct(lbl_eu_80665668->mADXFHandle);
            ADXF_Close(lbl_eu_80665668->mADXFHandle);
            lbl_eu_80665668->mADXFHandle = nullptr;
        }

        CDeviceFile::removeFileJob(job2);
        lbl_eu_80665668->mState = 0;
    }
    return false;

notComplete:
    mState = 4;
    return true;
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

        advanceReadProgress__11CFileHandleFi(handle, aligned);

        // Materialized-then-tested completion flag (li r3,0/1 + cmpwi).
        int complete = handle->unk10 != 0 && handle->unk10 == handle->mLength;        if (complete == 0) {
            goto state7;
        }
        ADXF_Stop(mADXFHandle);
        ADXF_Close(mADXFHandle);
        mADXFHandle = nullptr;
        mActiveWorkID = job->mWorkID;
        mState = 8;
        mIdleCounter = 0;
        return true;
    } else {
        // Abort: reset the handle position and cancel the read job.
        job->mHandle->unk10 = 0;
    call__11CFileHandleF3CBM(job->mHandle, 3);
    // Retail caches the singleton here (r6) for the count walk and child
    // extraction; the ADXF args/state stores still re-read the global.
    // KNOWN RESIDUAL: reslist::size()'s inlined counter/iterator land in
    // r3/r4 swapped vs retail (every manual-walk spelling tried shifts
    // other webs; static cap for this shape).
    CDeviceFileCri* self = lbl_eu_80665668;
    u32 count = self->mChildren.size();
    if (count != 0) {
        // Retail reuses the outer child/job registers for the second
        // getFirst extraction (the old job died with the CBM call).
        child = self->mChildren.front();
        job = (CDeviceFileJobReadDvd*)child;
        if (child == nullptr) {
            job = nullptr;
        } else if (child->mType != THREAD_CDEVICEFILEJOBREADDVD) {
            job = nullptr;
        }

        if (self->mADXFHandle != nullptr) {
            ADXF_Stop(lbl_eu_80665668->mADXFHandle);
            ADXF_GetNumReqSct(lbl_eu_80665668->mADXFHandle);
            ADXF_Close(lbl_eu_80665668->mADXFHandle);
            lbl_eu_80665668->mADXFHandle = nullptr;
        }

        CDeviceFile::removeFileJob(job);
        lbl_eu_80665668->mState = 0;
    }
    return false;
    }

state7:
    mState = 7;
    return true;
}

bool CDeviceFileCri::func_8045042C() {    // Seed-then-null shape (retail): keeps job's web identical to child's.
    CWorkThread* child = mChildren.front();
    CDeviceFileJobReadDvd* job = (CDeviceFileJobReadDvd*)child;
    if (child == nullptr) {
        job = nullptr;
    } else if (child->mType != THREAD_CDEVICEFILEJOBREADDVD) {
        job = nullptr;
    }

    int numReq = ADXF_GetNumReqSct(mADXFHandle);
    if ((u32)(numReq - 1) <= 1) return false;

    if (numReq == 3) {
        // Success: copy out the unread tail of the read.
        // Layout view (volatile unk10): retail re-reads the position field
        // from memory at every use instead of keeping it in a register.
        CFileHandleLayout* handle = (CFileHandleLayout*)job->mHandle;
        // The subf./ble guard skips both the copy and the cache flush when
        // the remainder is empty, but advanceReadProgress always runs.
        int remaining = handle->mLength - handle->unk10;
        if (remaining > 0) {
            memcpy(handle->mData + handle->unk10, mBuffer, remaining);
            DCFlushRange(handle->mData + handle->unk10, remaining);
        }

        advanceReadProgress__11CFileHandleFi((CFileHandle*)handle, remaining);

        ADXF_Stop(mADXFHandle);
        ADXF_Close(mADXFHandle);
        mADXFHandle = nullptr;
        mActiveWorkID = job->mWorkID;
    } else {
        // Abort: cancel the job, then tear down the singleton's ADXF state.
        call__11CFileHandleF3CBM(job->mHandle, 3);
        // Retail caches the singleton ONCE here (r6) for the size walk, the
        // front() extraction and the first ADXF-handle null check, but
        // re-reads the global between every call inside the teardown.
        CDeviceFileCri* self = lbl_eu_80665668;
        const reslist<CWorkThread*>& children = self->mChildren;
        u32 count = children.size();
        if (count != 0) {
            // Retail reuses the outer job register for the second getFirst
            // extraction (the old job died with the cancel call).
            child = self->mChildren.front();
            job = (CDeviceFileJobReadDvd*)child;
            if (child == nullptr) {
                job = nullptr;
            } else if (child->mType != THREAD_CDEVICEFILEJOBREADDVD) {
                job = nullptr;
            }

            if (self->mADXFHandle != nullptr) {
                ADXF_Stop(lbl_eu_80665668->mADXFHandle);
                ADXF_GetNumReqSct(lbl_eu_80665668->mADXFHandle);
                ADXF_Close(lbl_eu_80665668->mADXFHandle);
                lbl_eu_80665668->mADXFHandle = nullptr;
            }

            CDeviceFile::removeFileJob(job);
            lbl_eu_80665668->mState = 0;
        }
        return false;
    }

    mIdleCounter = 0;
    mState = 8;
    return true;
}

void CDeviceFileCri::wkUpdate() {
    // Retail dispatches through jumptable_eu_8056C330: a dense switch over
    // mState with deliberate FALLTHROUGH 0 -> 1 -> 2 (each case funnels its
    // failure paths through a result test before dropping into the next).
    if (!checkDriveStatus()) return;

    switch (mState) {
    case 0: {
        // Start an async ADX open for the first DVD-read child's file.
        // Result flag is assigned per-path (not initialized up front) so
        // each failure route materializes its own constant like retail.
        int open;
        if (mChildren.empty()) {
            open = 0;
        } else {
            // NOTE: retail dereferences the first child directly here (no
            // null/type guard); the guard only appears at the re-extraction.
            CDeviceFileJobReadDvd* job =
                (CDeviceFileJobReadDvd*)mChildren.front();

            // Skip the job while an exception is pending (flag or queued
            // EVT_EXCEPTION scan over the child's message queue, whose
            // layout mirrors our own at the same offsets) - except when it
            // sits in a retryable state (2 or 3).
            int ready;
            if (job->mFlags & THREAD_FLAG_EXCEPTION) {
                ready = 1;
            } else {
                ready = criFindException((CDeviceFileCri*)job) >= 0;
            }
            if (ready == 0) {
                int jobState = job->mState;
                if (jobState == 2 || jobState == 3) {
                    ready = 1;
                }
            }

            if (ready && !(job->mFlags & THREAD_FLAG_NO_EVENT)) {
                // Retail RE-EXTRACTS the first child here instead of reusing
                // the cached job pointer.
                CWorkThread* child2 = mChildren.front();
                CDeviceFileJobReadDvd* job2 = (CDeviceFileJobReadDvd*)child2;
                if (child2 == nullptr) {
                    job2 = nullptr;
                } else if (job2->mType != THREAD_CDEVICEFILEJOBREADDVD) {
                    job2 = nullptr;
                }
                job2->unk210 = 1;

                CFileHandleLayout* handle = (CFileHandleLayout*)job2->mHandle;
                const char* pFilename =
                    (handle->unk1A4 != 0) ? handle->nameLong : handle->nameShort;
                mADXFHandle = ADXF_OpenNw(pFilename, 0);
                mState = 1;
                open = 1;
            } else {
                open = 0;
            }
        }
        if (open == 0) break;
        // fallthrough
    }

    case 1:
        if (!func_8044FCFC()) break;

        if (--mTimeoutCounter <= 0) {
            CWorkThread* child = mChildren.front();
            CDeviceFileJobReadDvd* job = (CDeviceFileJobReadDvd*)child;
            if (child == nullptr) {
                job = nullptr;
            } else if (job->mType != THREAD_CDEVICEFILEJOBREADDVD) {
                job = nullptr;
            }
            CFileHandleLayout* handle = (CFileHandleLayout*)job->mHandle;

            u32 sectors = handle->field_0x38 >> 11;
            if (sectors != 0) {
                ADXF_Seek(mADXFHandle, sectors, 0);
            }
            if (handle->field_0x38 & 0x7FF) {
                // Unaligned remainder: pull one sector into the scratch
                // buffer and continue into the state-2 handling below.
                ADXF_ReadNw(mADXFHandle, 1, mBuffer);
                mState = 3;
            } else {
                mState = 4;
                break;
            }
        } else {
            break;
        }
        // fallthrough

    case 2:
        if (!func_80450058()) break;
        if (--mTimeoutCounter > 0) break;

        {
            CWorkThread* child = mChildren.front();
            CDeviceFileJobReadDvd* job = (CDeviceFileJobReadDvd*)child;
            if (child == nullptr) {
                job = nullptr;
            } else if (job->mType != THREAD_CDEVICEFILEJOBREADDVD) {
                job = nullptr;
            }
            CFileHandleLayout* handle = (CFileHandleLayout*)job->mHandle;
            u32 remaining = handle->mLength - handle->unk10;

            if (remaining > 0x800) {
                ADXF_ReadNw(mADXFHandle, remaining >> 11,
                            handle->mData + handle->unk10);
                mState = 5;
            } else {
                mState = 7;
            }
        }
        break;

    case 3:
        func_80450058();
        break;

    case 4:
        func_8045042C();
        break;

    case 5:
        func_80450260();
        break;

    case 6: {
        // Idle watchdog: while the active child matches our work ID and has
        // no event pending, count idle frames; after 30, fire its CBM3.
        if (mChildren.empty()) {
            mState = 0;
            break;
        }
        CDeviceFileJobReadDvd* job = (CDeviceFileJobReadDvd*)mChildren.front();
        if (mActiveWorkID != job->mWorkID) {
            mState = 0;
            break;
        }
        if (job->mFlags & THREAD_FLAG_NO_EVENT) {
            mState = 0;
            break;
        }
        if (++mIdleCounter < 30) break;

        CWorkThread* child = mChildren.front();
        CDeviceFileJobReadDvd* job2 = (CDeviceFileJobReadDvd*)child;
        if (child == nullptr) {
            job2 = nullptr;
        } else if (job2->mType != THREAD_CDEVICEFILEJOBREADDVD) {
            job2 = nullptr;
        }
        job2->callCBM3();
        mIdleCounter = 0;
        mState = 0;
        break;
    }

    case 7: {
        CWorkThread* child = mChildren.front();
        CDeviceFileJobReadDvd* job = (CDeviceFileJobReadDvd*)child;
        if (child == nullptr) {
            job = nullptr;
        } else if (job->mType != THREAD_CDEVICEFILEJOBREADDVD) {
            job = nullptr;
        }
        CFileHandleLayout* handle = (CFileHandleLayout*)job->mHandle;
        if (handle->mLength != handle->unk10) {
            ADXF_ReadNw(mADXFHandle, 1, mBuffer);
        }
        mState = 6;
        break;
    }

    case 8:
        func_8045042C();
        break;
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
    bool result = checkDriveStatus();
    
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

// [.data] 0x8056C330-0x8056C420 (0xF0 = 240B): wkUpdate jumptable (emitted
// by MWCC itself as the switch's anon table, landing at .data 0x0 =
// 0x8056C330) + vtable + RTTI base list.
namespace CDeviceFileCriBlob {
extern "C" void* __dt__14CDeviceFileCriFv();
extern "C" void wkUpdate__14CDeviceFileCriFv();
extern "C" void wkStandbyLogin__14CDeviceFileCriFv();
extern "C" void wkStandbyLogout__14CDeviceFileCriFv();
extern "C" void wkStandbyExceptionRetry__14CDeviceFileCriFUl();
extern "C" void teardownAdxf__14CDeviceFileCriFUl();
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
    (u32)&CDeviceFileCriBlob::teardownAdxf__14CDeviceFileCriFUl,
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
// setCriFilePathA/B1C/B24) and must NOT get a stub (split budget).
