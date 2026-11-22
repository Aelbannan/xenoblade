// Auto-scaffolded catalog TU for monolib/src/device/CDeviceFile
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <string.h>

#include "monolib/device/CDeviceFile.hpp"

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// Definitions for the retail data sections (see the full blob at the bottom
// of the file).  The three 1/1/6-byte .sdata bytes (language override +
// standby flag) are separate u8 objects so references emit the retail
// per-address symbols (chars have 1-byte alignment, so packing matches).
extern "C" u8 sStandbyReadyFlag = 0x01;
u8 sStandbyLoginDoneFlag = 0x01;   // standby-login done flag
s8 sLanguageOverride = -1;     // language override (-1 = auto)
u32 sDeviceFileInstance;   // CDeviceFile singleton pointer (.sbss)
int sSubstLangPathCount;   // filename-substitution table entry count (.sbss)
void* sSubstLangPathTable[16]; // filename-substitution table (.bss, 64B)
extern u8 sDeviceFileStaticState[0x108]; // .bss 264B (defined at bottom)

// Minimal class views
// mangled names exactly, so calls emit the retail reloc names. Full
// definitions live in the include/ headers, which this TU cannot use because
// it defines its own CDeviceFile.
class CWorkThread;
class CFileHandle;
class CDeviceFileJob;
class CDeviceFile;

// --- imports declared locally (this TU defines its own class views, so the
// --- real monolib headers cannot be included here) ---

class IWorkEvent;

// Language-substitution helper (defined below in this TU).
// Canonical C++ declaration: libs/monolib/include/monolib/device/CDeviceFileCri.hpp.
void DevFile_SubstLangPath(char* pPath);

// Pack-archive queries shared with CWorkSystemPack.
// Defined in monolib/src/work/CWorkSystemPack.cpp with C++ linkage.
int hasPackFile(const char* pName);
extern "C" bool func_804DDD54(const char* pName, const char* pPath, char** outPkbPath,
                              u32* outB, u32* outC, u32* outD);

// Job ctor is old-MWCC-mangled; called through its retail name.
extern "C" void* __ct__21CDeviceFileJobReadDvdFPCcP11CWorkThread(void* self,
                                                                 const char* pName,
                                                                 CWorkThread* pParent);

class CDevice {
public:
    static bool isColdStartReady();
};

class CDeviceSC {
public:
    static u8 getLanguage();
};

// CDeviceFileDvd/CDeviceFileCri ctors are old-MWCC-mangled (no arg suffix),
// so they must be called through C-linkage declarations with the exact
// retail names.
extern "C" void* __ct__CDeviceFileDvd(CWorkThread* pObj, const char* pName, CWorkThread* pParent);
extern "C" void* __ct__CDeviceFileCri(CWorkThread* pObj, const char* pName, CWorkThread* pParent);

// File call mode (see monolib/device/CFileHandle.hpp)
enum CBM {
    CBM_0,
    CBM_1,
    CBM_2,
    CBM_3,
    CBM_4,
    CBM_5
};

class CFileHandle {
public:
    ~CFileHandle();
    u32 getRsrc() const;
    void addFallbackHandle(unsigned long);
    CFileHandle* setup1(const char* pPath, unsigned long size, IWorkEvent* pEvent);
    CFileHandle* setup2(const char* pPath, unsigned long size, IWorkEvent* pEvent);

    u32 field_0x0;              //0x00
    u8* mData;                  //0x04 (loaded file buffer)
    u32 field_0x8;              //0x08
    u32 field_0xC;              //0x0C
    s32 field_0x10;             //0x10
    u32 field_0x14;             //0x14 (alloc handle)
    u32 field_0x18[4];          //0x18 (fallback alloc handles)
    u8 field_0x28[0x2C - 0x28];
    u32 field_0x2C;             //0x2C
    u8 field_0x30[0x38 - 0x30];
    u32 field_0x38;             //0x38 (read offset)
    u32 field_0x3C;             //0x3C (file length)
    u8 field_0x40[0x48 - 0x40];
    u32 field_0x48;             //0x48 (current request id / priority)
    u8 field_0x4C[0x58 - 0x4C];
    u32 field_0x58;             //0x58 (status flags; bit0 = data pending)
    char mName[0x100];          //0x5C
    u32 field_0x15C;
    char field_0x160[0x20];
    u32 field_0x180;
    char field_0x184[0x20];
    u32 field_0x1A4;
};

class CDeviceFileCri {
public:
    static CDeviceFile* getInstance();
    static void cancelJobsForPath(const char* pPath);
    static void cancel(CFileHandle* pHandle);
    static int getFileSize(const char* pPath, int param);
};

class CDeviceFileDvd {
public:
    static CDeviceFile* getInstance();
    static void isRequestFile(const char* pPath);
    static void cancel(CFileHandle* pHandle);
};

class CWorkSystem {
public:
    static CWorkSystem* getInstance();
};

class CLib {
public:
    static CLib* getInstance();
};

typedef _reslist_node<CWorkThread*> CWorkThreadListNode;

typedef _reslist_node<CFileHandle*> CFileHandleReslistNode;
typedef _reslist_node<CFileHandle*> CFileHandleListNode;

// File-pool entry (0x60 bytes); the ctor clears the leading word of each
// 0x0C sub-slot.
struct CDeviceFilePoolEntry {
    u32 head;           //0x00
    u8 field_0x4[0xC - 0x4];
    u32 slot0C;         //0x0C
    u8 field_0x10[0x18 - 0x10];
    u32 slot18;         //0x18
    u8 field_0x1C[0x24 - 0x1C];
    u32 slot24;         //0x24
    u8 field_0x28[0x30 - 0x28];
    u32 slot30;         //0x30
    u8 field_0x34[0x3C - 0x34];
    u32 slot3C;         //0x3C
    u8 field_0x40[0x48 - 0x40];
    u32 slot48;         //0x48
    u8 field_0x4C[0x54 - 0x4C];
    u32 slot54;         //0x54
    u8 field_0x58[0x60 - 0x58];
};

// Waiting-job pool slot (0x0C stride) used by the DVD/CRI devices: parks a
// child-list node detached during a preemption and records the owning job.
struct CDevJobSlot {
    CWorkThreadListNode* mNode;     //0x0 (relinked as mNext)
    CWorkThreadListNode* mPrevLink; //0x4 (relinked as mPrev)
    CDeviceFileJob* mJob;           //0x8
};

// Manager-singleton job pool slot (0x0C stride, appended to the tail of the
// 0x1D0 ring when a read request is registered).
struct CFileJobSlot {
    CWorkThreadListNode* mNode;     //0x0
    CWorkThreadListNode* mPrevLink; //0x4
    CFileHandle* mJob;              //0x8
};

class CDeviceFileJob : public CWorkThread {
public:
    CFileHandle* mHandle;  //0x1C4 (current job's file handle)
};

// KEEP: owner-side copy of ::CEventFile (canonical layout in
// monolib/work/CEventFile.hpp). This TU keeps private CBM/CFileHandle copies
// that conflict with the monolib headers, and it needs the CBM ctor.
struct CEventFile {
    CEventFile(CBM cbm, CFileHandle* handle);
    u8* getFileDataPtr();

    CBM field_0x0;           //0x0 (call mode)
    CFileHandle* field_0x4;  //0x4
    u32 field_0x8;           //0x8
    u8* field_0xC;           //0xC (handle->mName)
    u32 field_0x10;          //0x10
    u32 field_0x14;          //0x14
    u32 field_0x18;          //0x18
    u32 field_0x1C;          //0x1C (handle->getRsrc())
};


CDeviceFile::CDeviceFile(const char* pName, CWorkThread* pParent)
    : CDeviceBase(pName, pParent, 8) {
    mFlags = 0;                                  //0x1C4
    field_0x1C8 = NULL;
    sDeviceFileInstance = (u32)this;
    sLanguageOverride = -1;
    mFileList.mList = (CFileHandleReslistNode*)mtl::MemManager::allocate_array(
        0xC00, CWorkThreadSystem::getWorkMem());
    // Clear the leading word of every 0x0C sub-slot in all 32 entries.
    for (int i = 0; i < 32; i++) {
        ((CDeviceFilePoolEntry*)mFileList.mList)[i].head = 0;
        ((CDeviceFilePoolEntry*)mFileList.mList)[i].slot0C = 0;
        ((CDeviceFilePoolEntry*)mFileList.mList)[i].slot18 = 0;
        ((CDeviceFilePoolEntry*)mFileList.mList)[i].slot24 = 0;
        ((CDeviceFilePoolEntry*)mFileList.mList)[i].slot30 = 0;
        ((CDeviceFilePoolEntry*)mFileList.mList)[i].slot3C = 0;
        ((CDeviceFilePoolEntry*)mFileList.mList)[i].slot48 = 0;
        ((CDeviceFilePoolEntry*)mFileList.mList)[i].slot54 = 0;
    }
    mFileList.mCapacity = 0x100;
}

CDeviceFile::~CDeviceFile() {
    sDeviceFileInstance = 0;
}

CDeviceFile* CDeviceFile::getInstance() { return (CDeviceFile*)sDeviceFileInstance; }

// A device thread is ready when it has no pending EVT_EXCEPTION (flag bit or
// queued message) and its worker state is LOGIN or RUN.
static inline bool isDeviceFileReady(CDeviceFile* dev) {
    bool hasException;
    if (dev->mFlags & CWorkThread::THREAD_FLAG_EXCEPTION) {
        hasException = true;
    } else {
        // Scan the pending-message ring for a queued EVT_EXCEPTION; an index
        // of -1 means none was found.
        int idx;
        for (idx = 0; idx < dev->mMsgQueue.mRing.mSize; idx++) {
            if (dev->mMsgQueue.mRing.mArrayPtr[(dev->mMsgQueue.mRing.mFront + idx) % dev->mMsgQueue.mRing.mCapacity].command !=
                CWorkThread::EVT_EXCEPTION) {
                continue;
            }
            break;
        }
        if (idx >= dev->mMsgQueue.mRing.mSize) {
            idx = -1;
        }
        hasException = idx >= 0;
    }
    if (hasException) {
        return false;
    }
    if (dev->mState == CWorkThread::THREAD_STATE_LOGIN ||
        dev->mState == CWorkThread::THREAD_STATE_RUN) {
        return true;
    }
    return false;
}

int CDeviceFile::isInitialized() {
    // All three devices (manager, DVD, CRI) must be ready.
    if (!isDeviceFileReady((CDeviceFile*)sDeviceFileInstance)) return 0;

    if (CDeviceFileDvd::getInstance() == NULL) return 0;
    if (!isDeviceFileReady(CDeviceFileDvd::getInstance())) return 0;

    if (CDeviceFileCri::getInstance() == NULL) return 0;
    return isDeviceFileReady(CDeviceFileCri::getInstance()) ? 1 : 0;
}

extern "C" u8 isStandbyReady__11CDeviceFileFv() { return sStandbyReadyFlag; }

int CDeviceFile::setParent(CWorkThread* parent) {
    // spInstance (sDeviceFileInstance)->field_1C8 = parent; return true
    ((CDeviceFile*)(u32)sDeviceFileInstance)->field_0x1C8 = parent;
    return 1;
}

bool CDeviceFile::clearParent() {
    // spInstance (sDeviceFileInstance)->field_0x1C8 = 0; return true
    ((CDeviceFile*)(u32)sDeviceFileInstance)->field_0x1C8 = 0;
    return true;
}

// Shared body of readFile/readCommonArchiveFile: normalizes the path,
// resolves pack-archive indirection, allocates a CFileHandle via setup1/setup2
// and registers it in the manager's job list, then queues a read job on the
// active device (DVD or CRI).
static inline CFileHandle* readCommon(unsigned long allocHandle, const char* pPath,
                               IWorkEvent* pEvent, int offsetUnits, int sizeUnits,
                               bool archiveMode) {
    u32 outB;
    u32 outC;
    u32 outD;
    char* pkbPath;
    const char* slash;
    int slashLen;
    int idx;
    int absolute;
    CDeviceFile* mgr;
    CDeviceFileJob* job;
    char nameBuf[0x80];
    int nameLen;
    char pkbBuf[0x100];
    int pkbLen;
    char pathCopy[0x100];
    int pathLen;

    // Language-substituted copy of the full path (kept for setup1/setup2).
    // The DVD/CRI device recognises the USBKEY prefix on absolute paths.
    const char* usbPrefix = "USBKEY";
    pathLen = strlen(pPath);
    strcpy(pathCopy, pPath);
    DevFile_SubstLangPath(pathCopy);

    if (archiveMode && allocHandle == 0) {
        return NULL;
    }

    // Name of the read job registered with the active device.
    const char* jobName = "CDeviceFileJobReadDvd";

    // Name-only copy (stripped of any leading '/').
    nameBuf[0] = '\0';
    nameLen = 0;
    if (pPath[0] == '/') {
        nameLen = strlen(pPath + 1);
        strcpy(nameBuf, pPath + 1);
    } else {
        nameLen = strlen(pPath);
        strcpy(nameBuf, pPath);
    }

    pkbPath = NULL;

    // If the name carries a pack-archive extension, resolve the real file
    // through the pack system; outD/outC give sector-quantized sizes/offsets.
    slash = "/";
    slashLen = strlen(slash);
    for (idx = 0; idx < nameLen; idx++) {
        if (strncmp(&nameBuf[idx], slash, slashLen) == 0) {
            break;
        }
    }
    if (idx >= nameLen) {
        idx = -1;
    }
    if (idx != -1) {
        if (idx < nameLen) {
            nameBuf[idx] = '\0';
            nameLen = idx;
        }
        if (hasPackFile(nameBuf)) {
            if (func_804DDD54(nameBuf, pPath, &pkbPath, &outB, &outC, &outD)) {
                if (sizeUnits == 0) {
                    sizeUnits = outD << 11;
                }
                offsetUnits += outC << 11;
            }
        }
    }

    CFileHandle* handle = (CFileHandle*)mtl::MemManager::allocate(
        0x1A8, CWorkThreadSystem::getWorkMem());
    if (handle != NULL) {
        handle = archiveMode ? handle->setup2(pathCopy, allocHandle, pEvent)
                             : handle->setup1(pathCopy, allocHandle, pEvent);
    }

    if (handle != NULL) {
        // Append to the manager singleton's job ring via a free pool slot.
        mgr = (CDeviceFile*)sDeviceFileInstance;
        CFileJobSlot* slots = (CFileJobSlot*)mgr->mFileList.mList;
        CWorkThreadListNode* sentinel =
            (CWorkThreadListNode*)mgr->mFileList.mStartNodePtr;
        u32 freeIdx;
        for (freeIdx = 0; freeIdx < mgr->mFileList.mCapacity; freeIdx++) {
            if (slots[freeIdx].mNode == NULL) break;
        }
        if (&slots[freeIdx].mJob != NULL) {
            slots[freeIdx].mJob = handle;
        }
        CWorkThreadListNode* slotNode = (CWorkThreadListNode*)&slots[freeIdx];
        slotNode->mNext = sentinel;
        slotNode->mPrev = sentinel->mPrev;
        sentinel->mPrev->mNext = slotNode;
        sentinel->mPrev = slotNode;
    }

    handle->field_0x38 = offsetUnits;
    handle->field_0x3C = sizeUnits;
    if (offsetUnits != 0 || sizeUnits != 0) {
        handle->field_0x58 |= 1;
    }

    // Optional pack-base path: substitute and copy onto the handle.
    pkbBuf[0] = '\0';
    pkbLen = 0;
    if (pkbPath != NULL) {
        pkbLen = strlen(pkbPath);
        strcpy(pkbBuf, pkbPath);
        DevFile_SubstLangPath(pkbBuf);
        pkbPath = pkbBuf;
    }
    if (pkbPath != NULL) {
        handle->field_0x1A4 = strlen(pkbPath);
        strcpy(handle->field_0x184, pkbPath);
    }

    // Only relative paths reach the device queue.
    pathLen = strlen(pathCopy);
    if (pathLen > 6 && strstr(pathCopy, usbPrefix) == pathCopy) {
        absolute = 1;
    } else if (pathLen > 1 && pathCopy[1] == ':') {
        absolute = 1;
    } else if (pathLen <= 0) {
        absolute = 0;
    } else {
        s8 c = (s8)pathCopy[0];
        if (c == '/') {
            absolute = 0;
        } else if (c == '\\') {
            absolute = 1;
        } else {
            absolute = 0;
        }
    }

    job = NULL;
    if (!absolute) {
        if (sStandbyReadyFlag == 0) {
            CDeviceFile* dvd = CDeviceFileDvd::getInstance();
            job = (CDeviceFileJob*)mtl::MemManager::allocate(
                0x250, CWorkThreadSystem::getWorkMem());
            if (job != NULL) {
                job = (CDeviceFileJob*)__ct__21CDeviceFileJobReadDvdFPCcP11CWorkThread(
                    job, jobName, dvd);
            }
            CWorkUtil::entryWork(job, dvd, false);
        } else {
            CDeviceFile* cri = CDeviceFileCri::getInstance();
            job = (CDeviceFileJob*)mtl::MemManager::allocate(
                0x250, CWorkThreadSystem::getWorkMem());
            if (job != NULL) {
                job = (CDeviceFileJob*)__ct__21CDeviceFileJobReadDvdFPCcP11CWorkThread(
                    job, jobName, cri);
            }
            CWorkUtil::entryWork(job, cri, false);
        }
    }

    job->mHandle = handle;
    u32 prio = handle->field_0x48;
    if (!CDeviceFileDvd::getInstance()->updateJobPriority(handle, prio)) {
        CDeviceFileCri::getInstance()->updateJobPriority(handle, prio);
    }
    return handle;
}

CFileHandle* CDeviceFile::readFile(unsigned long allocHandle, const char* pPath,
                                   IWorkEvent* pEvent, int offsetUnits, int sizeUnits) {
    return readCommon(allocHandle, pPath, pEvent, offsetUnits, sizeUnits, false);
}

CFileHandle* CDeviceFile::readCommonArchiveFile(unsigned long allocHandle, const char* pPath,
                                                 IWorkEvent* pEvent, int offsetUnits,
                                                 int sizeUnits) {
    return readCommon(allocHandle, pPath, pEvent, offsetUnits, sizeUnits, true);
}

// Job-type name validation.  The shipped link has no caller, but the real
// function keeps these literals in this TU's compiled string pool (MWLD
// dead-strips the body from the linked view).
extern "C" bool isDeviceFileJobName(const char* name) {
    return name == "CDeviceFileJobValidPathDvd" ||
           name == "CDeviceFileJobValidPathCri" ||
           name == "CDeviceFileJobValidFileDvd" ||
           name == "CDeviceFileJobValidFileCri";
}

// File-size dispatch: the DVD getter is an old-MWCC-mangled import that
// takes an extra (mangle-invisible) trailing arg forwarded from r4.
extern "C" int getFileSize__14CDeviceFileDvdFPCc(const char* pPath, int param);

// CDeviceFile::getFileSize(const char*). Returns -1 when the path is not a
// plain relative path (absolute prefix, drive colon, or backslash root).
// Only relative paths reach the DVD/CRI device query.
extern "C" int getFileSize__11CDeviceFileFPCc(const char* path, int param) {
    int result = -1;
    int absolute;
    int len = strlen(path);
    if (len > 6 && strstr(path, "USBKEY") == path) {
        absolute = 1;
    } else if (len > 1 && path[1] == ':') {
        absolute = 1;
    } else if (len <= 0) {
        absolute = 0;
    } else {
        s8 c = (s8)path[0];
        if (c == '/') {
            absolute = 0;
        } else if (c == '\\') {
            absolute = 1;
        } else {
            absolute = 0;
        }
    }
    if (!absolute) {
        if (sStandbyReadyFlag) {
            result = CDeviceFileCri::getFileSize(path, param);
        } else {
            result = getFileSize__14CDeviceFileDvdFPCc(path, param);
        }
    }
    return result;
}

bool CDeviceFile::removeFileJob(CDeviceFileJob* job) {
    bool result;
    CFileHandle* handle = job->mHandle;
    CDeviceFile* inst = (CDeviceFile*)sDeviceFileInstance;
    if (handle == NULL) {
        result = false;
    } else {
        CFileHandleListNode* head = (CFileHandleListNode*)inst->mFileList.mStartNodePtr;
        CFileHandleListNode* next;
        CFileHandleListNode* node = head->mNext;
        // Unlink every list entry referencing this handle.
        while (node != head) {
            CFileHandle* item = node->mItem;
            next = node->mNext;
            if (item == handle) {
                CFileHandleListNode* prev = node->mPrev;
                prev->mNext = next;
                next->mPrev = prev;
                node->mNext = NULL;
            }
            node = next;
        }
        if (handle != NULL) {
            delete handle;
        }
        result = true;
    }
    job->mHandle = NULL;
    job->wkSetEvent(CWorkThread::EVT_NONE);
    return result;
}

void CDeviceFile::cancelJobsForPath(const char* pPath) {
    CDeviceFileCri::cancelJobsForPath(pPath);
    CDeviceFileDvd::isRequestFile(pPath);
}

void CDeviceFile::cancel(CFileHandle* pFileHandle) {
    if (pFileHandle != NULL) {
        CDeviceFileCri::cancel(pFileHandle);
        CDeviceFileDvd::cancel(pFileHandle);
    }
}

bool CDeviceFile::tryUpdateJobPriority(CFileHandle* pFileHandle, int param) {
    if (CDeviceFileDvd::getInstance()->updateJobPriority(pFileHandle, param)) {
        return true;
    }
    return CDeviceFileCri::getInstance()->updateJobPriority(pFileHandle, param);
}

bool CDeviceFile::updateJobPriority(CFileHandle* pFileHandle, int param) {
    // Request-state check over this device's child-job ring: find the queued
    // job holding pFileHandle, then either bump its priority (sole child) or
    // preempt a lower-priority sibling by parking nodes in the 0x70 job pool.
    if (this == NULL) {
        return false;
    }

    CWorkThreadListNode* node = mChildren.mStartNodePtr->mNext;
    CDeviceFileJob* foundJob;
    while (node != mChildren.mStartNodePtr) {
        // A list item participates only when its kind tag is in [0x41, 0x4F).
        CDeviceFileJob* job = (CDeviceFileJob*)node->mItem;
        if (job != NULL) {
            if (job->mType < 0x41) {
                job = NULL;
            } else if (job->mType >= 0x4F) {
                job = NULL;
            }
        } else {
            job = NULL;
        }
        foundJob = job;
        if (job->mHandle == pFileHandle) {
            break;
        }
        node = node->mNext;
    }
    if (node == mChildren.mStartNodePtr) {
        return false;
    }

    // Count the children by walking the whole ring once.
    int idx = 0;
    CWorkThreadListNode* walk = mChildren.mStartNodePtr->mNext;
    do {
        walk = walk->mNext;
        idx++;
    } while (walk != mChildren.mStartNodePtr);

    if (idx == 1) {
        pFileHandle->field_0x48 = param;
        return true;
    }

    // Detach the matched node from the child ring.
    CWorkThreadListNode* prev = node->mPrev;
    CWorkThreadListNode* next = node->mNext;
    prev->mNext = next;
    next->mPrev = prev;
    node->mNext = NULL;

    if (mChildren.mStartNodePtr->mNext != mChildren.mStartNodePtr) {
        for (CWorkThreadListNode* scan = mChildren.mStartNodePtr->mNext; scan != mChildren.mStartNodePtr;
             scan = scan->mNext) {
            CDeviceFileJob* job = (CDeviceFileJob*)scan->mItem;
            if (job != NULL) {
                if (job->mType < 0x41) {
                    job = NULL;
                } else if (job->mType >= 0x4F) {
                    job = NULL;
                }
            } else {
                job = NULL;
            }
            CFileHandle* jobHandle = job->mHandle;
            if (jobHandle != NULL && param < (int)jobHandle->field_0x48) {
                // Preempt the lower-priority job: park its node in the pool.
                pFileHandle->field_0x48 = param;
                u32 freeIdx;
                for (freeIdx = 0; freeIdx < mChildren.mCapacity; freeIdx++) {
                    if (((CDevJobSlot*)mChildren.mList)[freeIdx].mNode == NULL) break;
                }
                CWorkThreadListNode* slotNode =
                    (CWorkThreadListNode*)&((CDevJobSlot*)mChildren.mList)[freeIdx];
                if (&((CDevJobSlot*)mChildren.mList)[freeIdx].mJob != NULL) {
                    ((CDevJobSlot*)mChildren.mList)[freeIdx].mJob = foundJob;
                }
                slotNode->mNext = scan;
                slotNode->mPrev = scan->mPrev;
                scan->mPrev->mNext = slotNode;
                scan->mPrev = slotNode;
                return true;
            }
        }
    }

    // No preemption target: park against the ring sentinel itself.
    pFileHandle->field_0x48 = param;
    u32 freeIdx2;
    for (freeIdx2 = 0; freeIdx2 < mChildren.mCapacity; freeIdx2++) {
        if (((CDevJobSlot*)mChildren.mList)[freeIdx2].mNode == NULL) break;
    }
    CWorkThreadListNode* slotNode2 =
        (CWorkThreadListNode*)&((CDevJobSlot*)mChildren.mList)[freeIdx2];
    if (&((CDevJobSlot*)mChildren.mList)[freeIdx2].mJob != NULL) {
        ((CDevJobSlot*)mChildren.mList)[freeIdx2].mJob = foundJob;
    }
    slotNode2->mNext = (CWorkThreadListNode*)mChildren.mStartNodePtr;
    slotNode2->mPrev = ((CWorkThreadListNode*)mChildren.mStartNodePtr)->mPrev;
    ((CWorkThreadListNode*)mChildren.mStartNodePtr)->mPrev->mNext = slotNode2;
    ((CWorkThreadListNode*)mChildren.mStartNodePtr)->mPrev = slotNode2;
    return true;
}

void CDeviceFile::setHandleParam(CFileHandle* pHandle, unsigned long param) {
    pHandle->addFallbackHandle(param);
}

void CDeviceFile::setHandleFlag1(CFileHandle* pFileHandle) {
    pFileHandle->field_0x58 |= 2;
}

void CDeviceFile::setHandleFlag2(CFileHandle* pFileHandle) {
    pFileHandle->field_0x58 |= 4;
}

bool CDeviceFile::wkStandbyLogin() {
    if (CDevice::isColdStartReady()) {
        const char* dvdName = "CDeviceFileDvd";
        CWorkThread* dvd = (CWorkThread*)mtl::MemManager::allocate(0x1D8, CWorkThreadSystem::getWorkMem());
        if (dvd != NULL) {
            dvd = (CWorkThread*)__ct__CDeviceFileDvd(dvd, dvdName, this);
        }
        CWorkUtil::entryWork(dvd, this, false);

        const char* criName = "CDeviceFileCri";
        CWorkThread* cri = (CWorkThread*)mtl::MemManager::allocate(0x1F0, CWorkThreadSystem::getWorkMem());
        if (cri != NULL) {
            cri = (CWorkThread*)__ct__CDeviceFileCri(cri, criName, this);
        }
        CWorkUtil::entryWork(cri, this, false);

        sStandbyLoginDoneFlag = 1;
        return CWorkThread::wkStandbyLogin();
    }
    return false;
}

bool CDeviceFile::wkStandbyLogout() {
    // Log out only when the child list is empty and the work-system/lib
    // singletons have been released.
    if (mChildren.mStartNodePtr->mNext == mChildren.mStartNodePtr &&
        CWorkSystem::getInstance() == NULL &&
        CLib::getInstance() == NULL) {
        return CWorkThread::wkStandbyLogout();
    }
    return false;
}

extern "C" void func_eu_804520B0(void* fileName) {
    int idx = (int)sSubstLangPathCount;
    sSubstLangPathTable[idx] = fileName;
    sSubstLangPathCount = idx + 1;
}

// Language-aware filename substitution. Walks the registered filename table
// (fed by func_eu_804520B0 / DevFile_SetLangOverride) and, when the path contains a
// table entry's search string, overwrites that substring in place with the
// language-specific variant (each entry is a NULL-terminated string array
// where [0] is the search text and [lang] the replacement).
void DevFile_SubstLangPath(char* pPath) {
    char* found;
    const char* const** pTable;
    int count;
    u8 lang;
    int i;
    const char* search;

    lang = CDeviceSC::getLanguage();
    s8 langOverride = sLanguageOverride;
    if (langOverride >= 0) {
        lang = langOverride;
    }
    if (lang == 0) return;

    if (CDeviceSC::getLanguage() > 5) {
        lang = 1;
    }

    count = sSubstLangPathCount;
    pTable = (const char* const**)sSubstLangPathTable;
    for (i = 0; i < count; i++) {
        search = pTable[0][0];
        found = strstr(pPath, search);
        if (found != NULL) {
            memcpy(found, ((const char* const*)sSubstLangPathTable[i])[lang + 1], strlen(search));
            return;
        }
        pTable++;
    }
}

// These two retail data symbols are also declared in the "C-linkage imports"
// section of monolib/device/CDeviceFile.hpp. They stay declared here too
// because this catalog TU defines a local `struct CDeviceFile` that conflicts
// with the class in that header, so this TU cannot include it.
extern "C" void DevFile_SetLangOverride(s8 val) {
    sLanguageOverride = val;
}

extern "C" void func_eu_804521B0() {
    sLanguageOverride = -1;
}

extern "C" void DevFile_SetByteA9(u8 val) {
    sStandbyLoginDoneFlag = val;
}

extern "C" u8 DevFile_GetByteA9() {
    return sStandbyLoginDoneFlag;
}

CEventFile::CEventFile(CBM cbm, CFileHandle* handle) {
    field_0x0 = cbm;
    field_0x4 = handle;
    field_0x8 = handle->field_0x14;
    field_0xC = (u8*)handle->mName;
    field_0x10 = handle->field_0x8;
    field_0x14 = handle->field_0x3C;
    field_0x18 = handle->field_0x2C;
    field_0x1C = handle->getRsrc();
}

u8* CEventFile::getFileDataPtr() {
    CFileHandle* h = field_0x4;
    u8* r = h->mData;
    h->mData = NULL;
    return r;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_eu_80452248() {
    extern unsigned char sDeviceFileStaticState[];
    sDeviceFileStaticState[0] = 0;
    *(unsigned int*)(sDeviceFileStaticState + 0x100) = 0;
}

// [.bss] 0x806575C0-0x806576C8 (264B).
u8 sDeviceFileStaticState[0x108];
