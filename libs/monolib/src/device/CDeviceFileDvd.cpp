#include "monolib/device/CDeviceFileDvd.hpp"

#include <revolution/DVD.h>
#include <string.h>

// CDeviceFile helpers (declared here to keep the includes minimal; the
// retail names are C++ member symbols).
struct CDeviceFileJob;
class CDeviceFile {
public:
    static void removeFileJob(CDeviceFileJob* pJob);
};

// Local vtable-positioned view of the job classes: the retail dispatch is
// vtable[41] (`lwz r12,0xa4(r12)`), which is `cancel(const char*)` after the
// real IWorkEvent(32) + CWorkThread(7) + job-dtor chain. The dummies only
// position the slot (never called); declaring cancel as a real virtual makes
// MWCC emit the r12 dispatch shape.
class CDeviceFileJobVtbl {
public:
    virtual ~CDeviceFileJobVtbl();
    virtual void s00();
    virtual void s01();
    virtual void s02();
    virtual void s03();
    virtual void s04();
    virtual void s05();
    virtual void s06();
    virtual void s07();
    virtual void s08();
    virtual void s09();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void s33();
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual bool cancel(const char* pFilename);
    virtual bool cancel(void* pStruct);
};

// Typed view of the device file job object laid out at retail offsets.
class CDeviceFileJob {
public:
    u8 field_0x00[0x48];
    u32 field_0x48;                  // required drive/media state
    s32 field_0x4C;                  // work id recorded on completion
    s32 field_0x50;                  // job state (0x44 while opening)
    u8 field_0x54[0x7C - 0x54];
    u32 field_0x7C;                  // flag bits (bit0: cancel requested)
    u8 field_0x80[0x1A4 - 0x80];
    u32 field_0x1A4;                 // directory entry array base
    u32 field_0x1A8;                 // ring head index
    s32 field_0x1AC;                 // entry count
    u32 field_0x1B0;                 // ring modulus
    u8 field_0x1B4[0x1C4 - 0x1B4];
    CFileHandle* field_0x1C4;
    char mErrorText[0x208 - 0x1C8];  //0x1C8
    u32 field_0x208;                 //error text length
    u8 field_0x210;                  //dvd file opened flag
    u8 field_0x211[0x214 - 0x211];
    DVDFileInfo dvdInfo;             //0x214
};

// Node of the device file-job linked list rooted at the reslist sentinel.
struct CDeviceFileJobLink {
    CDeviceFileJobLink* next;  //0x00
    u32 field_0x04;
    CDeviceFileJob* job;       //0x08
};

// The file-job list sentinel pointer lives at CWorkThread+0x60 (the reslist
// mStartNodePtr slot, shared with mChildren's list head).
inline CDeviceFileJobLink** jobListSlot(CDeviceFileDvd* dev) {
    return reinterpret_cast<CDeviceFileJobLink**>(reinterpret_cast<u8*>(dev) + 0x60);
}

inline CDeviceFileJob* getActiveJob(CDeviceFileDvd* dev) {
    CDeviceFileJobLink* sentinel = *jobListSlot(dev);
    CDeviceFileJob* job = sentinel->next->job;
    if (job == nullptr || job->field_0x50 != 0x44) return nullptr;
    return job;
}

CDeviceFileDvd::CDeviceFileDvd(const char* pName, CWorkThread* pParent)
    : CWorkThread(pName, pParent, 0x100) {
    field_0x1C4 = 0;
    field_0x1C8 = -1;
    field_0x1CC = 0;
    field_0x1D0 = 0;
    field_0x1D4 = 0;
    CDeviceFileDvd::spInstance = this;
}

CDeviceFileDvd::~CDeviceFileDvd() {
    CDeviceFileDvd::spInstance = nullptr;
}

CDeviceFileDvd* CDeviceFileDvd::getInstance() { return CDeviceFileDvd::spInstance; }

int CDeviceFileDvd::getFileSize(const char* pPath) {
    DVDFileInfo fileInfo;

    int entrynum = DVDConvertPathToEntrynum(pPath);
    if (entrynum < 0) return -1;

    if (!DVDFastOpen(entrynum, &fileInfo)) return -1;

    int size = fileInfo.size;
    DVDClose(&fileInfo);
    return size;
}

void CDeviceFileDvd::isRequestFile(const char* pPath) {
    // Walk the file-system job chain (singleton +0x60 list); for each node
    // whose job is in the still-opening state (field_50 == 0x44), call the
    // job's cancel(pPath) virtual. The singleton/field_60 sentinel is
    // re-read every iteration (retail lwz r3,0(r0) in the loop check).
    u8* node = *(u8**)(*(u32*)((u8*)CDeviceFileDvd::spInstance + 0x60));
    while (node != (u8*)*(u32*)((u8*)CDeviceFileDvd::spInstance + 0x60)) {
        u8* job = *(u8**)(node + 8);
        if (job == 0) {
            job = 0;
        } else if (*(s32*)(job + 0x50) != 0x44) {
            job = 0;
        }
        if (job != 0) {
            reinterpret_cast<CDeviceFileJobVtbl*>(job)->cancel(pPath);
        }
        node = *(u8**)(node);
    }
}

// Local views of the resource/event classes used by CFileHandle. Declared
// here (exact retail names/signatures) so the calls resolve to the retail
// mangled symbols without pulling in their full headers.
enum CBM {
    CBM_0,
    CBM_1,
    CBM_2,
    CBM_3,
    CBM_4,
    CBM_5
};

// KEEP: owner-side copy of ::CEventFile (canonical layout in
// monolib/work/CEventFile.hpp). This TU keeps private CBM/CFileHandle copies
// that conflict with the monolib headers, and it needs the CBM ctor.
class CEventFile {
public:
    CEventFile(CBM cbm, CFileHandle* pHandle);

    u32 unk0;
    CFileHandle* mFileHandle;
    u8 _pad08[0x0C];
    u32 field_14;
};

// Vtable-positioned view: retail dispatches vtable+0x10 with a CEventFile*
// argument. MWCC emits two destructor slots, so only two placeholders precede
// the event handler.
class CRsrc {
public:
    static CRsrc* getRsrc(u32 id);
    bool isExistFile(const char* pName, void** ppData, u32* pLength);
    static bool releaseCache(const void* pData);
};

struct CFileHandle {
    u32 field_0x00;          //0x00 (device state)
    u32 field_0x04;          //0x04 (loaded buffer)
    u32 field_0x08;          //0x08 (result data ptr)
    u32 field_0x0C;          //0x0C (result size)
    s32 field_0x10;          //0x10 (read progress)
    u32 field_0x14;          //0x14 (alloc handle)
    u32 field_0x18[4];       //0x18 (fallback alloc handles)
    IWorkEvent* field_0x28;  //0x28 (event receiver)
    u32 field_0x2C;          //0x2C
    s32* field_0x30;         //0x30 (out: data ptr)
    s32* field_0x34;         //0x34 (out: length)
    u32 field_0x38;          //0x38 (read offset)
    u32 field_0x3C;          //0x3C (file length)
    u32 field_0x40[6];       //0x40-0x57 (init-cleared words)
    u32 field_0x58;
    char mName[0x100];       //0x5C
    u32 field_0x15C;
    char field_0x160[0x20];
    u32 field_0x180;
    char field_0x184[0x20];
    u32 field_0x1A4;

    ~CFileHandle();
    bool call(CBM cbm);    bool checkExistRsrc(CBM cbm);
    void* getRsrc() const;
    void init(int);
    CFileHandle* setup1(const char* pPath, u32 size, IWorkEvent* pEvent);
    CFileHandle* setup2(const char* pPath, u32 size, IWorkEvent* pEvent);
    void destroy(u32 size = 0, u32 param_5 = 0, int param_6 = 0);
    void addFallbackHandle(unsigned long);
    int advanceReadProgress(int);
};

// Static (retail pHandle in r3) chain walker mirroring isRequestFile: call the
// job's cancel(handle) virtual (vtable[42]) for each job in the still-opening
// state. The handle is passed through (r4 = r30).
void CDeviceFileDvd::cancel(CFileHandle* pHandle) {
    u8* node = *(u8**)(*(u32*)((u8*)CDeviceFileDvd::spInstance + 0x60));
    while (node != (u8*)*(u32*)((u8*)CDeviceFileDvd::spInstance + 0x60)) {
        u8* job = *(u8**)(node + 8);
        if (job == 0) {
            job = 0;
        } else if (*(s32*)(job + 0x50) != 0x44) {
            job = 0;
        }
        if (job != 0) {
            reinterpret_cast<CDeviceFileJobVtbl*>(job)->cancel((void*)pHandle);
        }
        node = *(u8**)(node);
    }
}

void CDeviceFileDvd::cancelCurrent() {
    // Resolve the active job from the file-system chain, keep it only while
    // it is in the still-opening state (s32 field_50 == 0x44), close its DVD
    // file info at +0x214 when the close flag (+0x1D0) is set, then remove
    // the job and mark the state word +0x1C4 with 4. The singleton is
    // re-loaded at the end (retail lwz r3,0(r0) after the calls).
    u8* job = *(u8**)(*(u32*)(*(u32*)((u8*)CDeviceFileDvd::spInstance + 0x60)) + 8);
    if (job == 0) {
        job = 0;
    } else if (*(s32*)(job + 0x50) != 0x44) {
        job = 0;
    }
    if (*(u8*)((u8*)CDeviceFileDvd::spInstance + 0x1D0) != 0) {
        DVDClose(reinterpret_cast<DVDFileInfo*>(job + 0x214));
    }
    CDeviceFile::removeFileJob(reinterpret_cast<CDeviceFileJob*>(job));
    *(u32*)((u8*)CDeviceFileDvd::spInstance + 0x1C4) = 4;
}

bool CDeviceFileDvd::transState0() {
    // Nothing to do while the file-job list is empty.
    CDeviceFileJobLink* sentinel = *jobListSlot(this);
    if (sentinel->next == sentinel) {
        return false;
    }

    // Refuse to start while a cancel is pending or the queued directory
    // entries already contain an entry of type 2 (invalid request).
    CDeviceFileJob* pNodeJob = sentinel->next->job;
    bool blocked;
    if ((pNodeJob->field_0x7C & 0x10) != 0) {
        blocked = true;
    } else {
        blocked = false;
        for (s32 i = 0; i < pNodeJob->field_0x1AC; i++) {
            u32 slot = (pNodeJob->field_0x1A8 + i) % pNodeJob->field_0x1B0;
            u32 entry = ((u32*)pNodeJob->field_0x1A4)[slot];
            if (entry == 2) {
                blocked = true;
                break;
            }
        }
    }
    if (blocked) return false;
    if (pNodeJob->field_0x48 != 2 && pNodeJob->field_0x48 != 3) return false;
    if ((pNodeJob->field_0x7C & 1) != 0) return false;

    CDeviceFileJob* pJob = getActiveJob(CDeviceFileDvd::spInstance);
    pJob->field_0x210 = 1;
    CFileHandle* pHandle = pJob->field_0x1C4;

    s32 entrynum = DVDConvertPathToEntrynum(pHandle->mName);
    bool opened = false;
    if (entrynum >= 0) {
        opened = DVDFastOpen(entrynum, &pJob->dvdInfo) != 0;
    }
    if (!opened) {
        // Failed to locate/open: report the error on the handle and drop
        // the job.
        pHandle->call(CBM_3);
        CDeviceFileJob* pCur = getActiveJob(CDeviceFileDvd::spInstance);
        if (CDeviceFileDvd::spInstance->field_0x1D0 != 0) {
            DVDClose(&pCur->dvdInfo);
        }
        CDeviceFile::removeFileJob(pCur);
        CDeviceFileDvd::spInstance->field_0x1C4 = 4;
        return false;
    }

    field_0x1D0 = 1;
    // Read at most the whole DVD file, clamped by the requested size.
    u32 amount = pHandle->field_0x3C;
    if (amount == 0) amount = pJob->dvdInfo.size;
    u32 total = amount + pHandle->field_0x38;
    if (total > pJob->dvdInfo.size) total = pJob->dvdInfo.size;
    pHandle->destroy(total, 0x20, 0x20);
    field_0x1C4 = 1;
    return true;
}

bool CDeviceFileDvd::transState3() {
    CDeviceFileJob* pJob = getActiveJob(CDeviceFileDvd::spInstance);
    s32 status = DVDGetCommandBlockStatus(&pJob->dvdInfo.block);

    if (status <= 2) {
        CFileHandle* pHandle = pJob->field_0x1C4;
        u32 transferred = DVDGetTransferredSize(&pJob->dvdInfo.block);
        // Advance the read progress by the transferred delta, clamped to the
        // remaining bytes.
        u32 delta = transferred - field_0x1CC;
        u32 remaining = pHandle->field_0x3C - pHandle->field_0x10;
        if (remaining < delta) delta = remaining;
        pHandle->advanceReadProgress(delta);
        field_0x1CC = transferred;

        bool done =
            pHandle->field_0x10 != 0 && (u32)pHandle->field_0x10 == pHandle->field_0x3C;
        if (!done && (pJob->field_0x7C & 1) == 0) {
            return false;
        }

        DVDClose(&pJob->dvdInfo);
        field_0x1D0 = 0;
        if (status == 0) {
            // Completed normally: remember the work id and finish up later.
            field_0x1C8 = pJob->field_0x4C;
            field_0x1C4 = 4;
            return true;
        }
        // Cancelled mid-read: report the error on the handle and drop the job.
        pHandle->field_0x10 = 0;
        pHandle->call(CBM_3);
        CDeviceFileJob* pCur = getActiveJob(CDeviceFileDvd::spInstance);
        if (CDeviceFileDvd::spInstance->field_0x1D0 != 0) {
            DVDClose(&pCur->dvdInfo);
        }
        CDeviceFile::removeFileJob(pCur);
        CDeviceFileDvd::spInstance->field_0x1C4 = 4;
        return false;
    }

    // Drive error / disc change: close and record the matching error text.
    DVDClose(&pJob->dvdInfo);
    field_0x1D0 = 0;
    const char* msg = nullptr;
    switch (status) {
    case -1:
        msg = "\x83\x47\x83\x89\x81\x5b\x82\xaa\x94\xad\x90\xb6\x82\xb5\x82\xdc\x82\xb5\x82\xbd\x81\x42";
        break;
    case 4:
    case 6:
        msg = "\x8b\x90\x90\x5f\x82\xcc\x83\x66\x83\x42\x83\x58\x83\x4e\x82\xf0\x83\x5a\x83\x62\x83\x67\x82\xb5\x82\xc4\x82\xad\x82\xbe\x82\xb3\x82\xa2\x81\x42";
        break;
    case 7:
        msg = "\x83\x82\x81\x5b\x83\x5e\x81\x5b\x82\xaa\x8e\x7e\x82\xdc\x82\xc1\x82\xc4\x82\xa2\x82\xdc\x82\xb7\x81\x42";
        break;
    case 11:
        msg = "\x83\x66\x83\x42\x83\x58\x83\x4e\x82\xf0\x93\xc7\x82\xdf\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\x81\x42\x8f\xda\x82\xb5\x82\xad\x82\xcd\x81\x45\x81\x45\x81\x45";
        break;
    }
    if (msg != nullptr) {
        pJob->field_0x208 = strlen(msg);
        strcpy(pJob->mErrorText, msg);
    }

    CFileHandle* pHandle = pJob->field_0x1C4;
    pHandle->field_0x10 = 0;
    if (status == -1) {
        pHandle->call(CBM_3);
        CDeviceFileJob* pCur = getActiveJob(CDeviceFileDvd::spInstance);
        if (CDeviceFileDvd::spInstance->field_0x1D0 != 0) {
            DVDClose(&pCur->dvdInfo);
        }
        CDeviceFile::removeFileJob(pCur);
        CDeviceFileDvd::spInstance->field_0x1C4 = 4;
        return false;
    }
    pHandle->call(CBM_3);
    CDeviceFileJob* pCur2 = getActiveJob(CDeviceFileDvd::spInstance);
    if (CDeviceFileDvd::spInstance->field_0x1D0 != 0) {
        DVDClose(&pCur2->dvdInfo);
    }
    CDeviceFile::removeFileJob(pCur2);
    CDeviceFileDvd::spInstance->field_0x1C4 = 4;
    return false;
}

void CDeviceFileDvd::wkUpdate() {
    switch (field_0x1C4) {
    case 0:
        if (!transState0()) {
            break;
        }
        // fallthrough
    case 1: {
        // Kick off the async DVD read for the active job's handle. The read
        // length is rounded up to a 32-byte boundary.
        CDeviceFileJob* pJob = getActiveJob(CDeviceFileDvd::spInstance);
        CFileHandle* pHandle = pJob->field_0x1C4;
        void* dst = (void*)(pHandle->field_0x04 + pHandle->field_0x10);
        u32 size = pHandle->field_0x3C;
        if ((size & 0x1F) != 0) {
            size = (size & ~0x1F) + 0x20;
        }
        if (!DVDReadAsyncPrio(&pJob->dvdInfo, dst, size, pHandle->field_0x38,
                              nullptr, 2)) {
            DVDClose(&pJob->dvdInfo);
            field_0x1D0 = 0;
            pHandle->call(CBM_3);
            CDeviceFileJob* pCur = getActiveJob(CDeviceFileDvd::spInstance);
            if (CDeviceFileDvd::spInstance->field_0x1D0 != 0) {
                DVDClose(&pCur->dvdInfo);
            }
            CDeviceFile::removeFileJob(pCur);
            CDeviceFileDvd::spInstance->field_0x1C4 = 4;
            break;
        }
        field_0x1D4 = 0xA;
        field_0x1CC = 0;
        field_0x1C4 = 2;
        // fallthrough
    }
    case 2: {
        // Poll the drive; give the retry counter a few frames before timing
        // out into state 3.
        int ready;
        s32 status = DVDGetDriveStatus();
        if (status <= 2) {
            field_0x1C4 = 3;
            ready = 1;
        } else {
            field_0x1D4 = field_0x1D4 - 1;
            if ((s32)field_0x1D4 <= 0) {
                field_0x1C4 = 3;
                ready = 1;
            } else {
                ready = 0;
            }
        }
        if (!ready) {
            break;
        }
        // fallthrough
    }
    case 3:
        if (!transState3()) {
            break;
        }
        // fallthrough
    case 4: {
        // Reset to idle unless the finished job still matches the active work
        // id with no cancel flag set (keep state 4 so the result is picked
        // up).
        CDeviceFileJobLink* sentinel = *jobListSlot(this);
        bool keep = false;
        if (sentinel->next != sentinel) {
            CDeviceFileJob* pJob = sentinel->next->job;
            keep = (pJob->field_0x4C == field_0x1C8) && (pJob->field_0x7C & 1) == 0;
        }
        if (!keep) {
            field_0x1C4 = 0;
        }
        break;
    }
    }
}

bool CDeviceFileDvd::wkStandbyLogin() {
    DVDInit();
    return CWorkThread::wkStandbyLogin();
}

bool CDeviceFileDvd::wkStandbyLogout() {
    // Log out only when there are no pending file jobs and the work/CLib
    // systems are already gone.
    if (mChildren.empty() && CWorkSystem::getInstance() == nullptr &&
        CLib::getInstance() == nullptr) {
        return CWorkThread::wkStandbyLogout();
    }
    return false;
}

bool CDeviceFileDvd::wkStandbyExceptionRetry(WORK_ID param) {
    if ((s32)field_0x1C4 == 3) {
        // Walk the file-system job chain (singleton +0x60 list); the active
        // job is the node after the sentinel (mirrors getActiveJob).
        CDeviceFileJob* p = (*jobListSlot(this))->next->job;
        if (p == nullptr) {
            p = nullptr;
        } else if (p->field_0x50 != 0x44) {
            p = nullptr;
        }
        p->field_0x1C4->field_0x10 = 0;
        field_0x1C4 = 4;
    }
    return true;
}

CFileHandle* CFileHandle::setup1(const char* pPath, u32 size, IWorkEvent* pEvent) {
    field_0x58 = 0;
    mName[0] = '\0';
    field_0x15C = 0;
    field_0x160[0] = '\0';
    field_0x180 = 0;
    field_0x184[0] = '\0';
    field_0x1A4 = 0;
    init(0);
    field_0x14 = size;
    field_0x15C = strlen(pPath);
    strcpy(mName, pPath);
    field_0x28 = pEvent;
    return this;
}

CFileHandle* CFileHandle::setup2(const char* pPath, u32 size, IWorkEvent* pEvent) {
    field_0x58 = 0;
    mName[0] = '\0';
    field_0x15C = 0;
    field_0x160[0] = '\0';
    field_0x180 = 0;
    field_0x184[0] = '\0';
    field_0x1A4 = 0;
    init(1);
    field_0x04 = size;
    field_0x08 = size;
    field_0x15C = strlen(pPath);
    strcpy(mName, pPath);
    field_0x28 = pEvent;
    return this;
}

bool CFileHandle::checkExistRsrc(CBM cbm) {
    if (field_0x00 != 2) return false;
    if (CRsrc::getRsrc(field_0x14) == NULL) return false;
    void* pData;
    u32 length;
    if (!CRsrc::getRsrc(field_0x14)->isExistFile(mName, &pData, &length)) {
        return false;
    }
    field_0x08 = (u32)pData;
    field_0x3C = length;
    field_0x0C = length;
    field_0x04 = 0;
    call(cbm);
    return true;
}

CFileHandle::~CFileHandle() {
    if (field_0x00 == 2 || field_0x00 == 0) {
        void* pCache = (void*)field_0x04;
        if (pCache != NULL && field_0x30 == 0) {
            if (field_0x00 == 2) {
                // DVD-backed cache must be released before freeing.
                if (!CRsrc::releaseCache((const void*)field_0x04) && field_0x04 != 0) {
                    delete (void*)field_0x04;
                    field_0x04 = 0;
                }
            } else {
                delete pCache;
                field_0x04 = 0;
            }
        }
    }
}

bool CFileHandle::call(CBM cbm) {
    if (cbm == CBM_3 || cbm == CBM_5) {
        if (field_0x30 != NULL) *field_0x30 = 0;
        if (field_0x34 != NULL) *field_0x34 = -1;
    } else if (cbm == CBM_1 || cbm == CBM_4) {
        if (field_0x30 != NULL) *field_0x30 = field_0x08;
        if (field_0x34 != NULL) *field_0x34 = field_0x3C;
    }
    if (field_0x28 != NULL) {
        CEventFile event(cbm, this);
        return field_0x28->OnFileEvent(&event);
    }
    return false;
}

void CFileHandle::init(int param) {
    field_0x00 = param;
    field_0x3C = 0;
    field_0x04 = 0;
    field_0x08 = 0;
    field_0x10 = 0;
    field_0x14 = -1;
    field_0x18[0] = -1;
    field_0x18[1] = -1;
    field_0x18[2] = -1;
    field_0x18[3] = -1;
    field_0x28 = 0;
    field_0x0C = 0;
    field_0x2C = 0;
    field_0x30 = 0;
    field_0x34 = 0;
    field_0x38 = 0;
    field_0x40[0] = 0;
    field_0x40[1] = 0;
    field_0x40[3] = 0;
    field_0x40[4] = 0;
    field_0x40[2] = 4;
    field_0x40[5] = 0;
    field_0x58 = 0;
    // Store order mirrors the retail init sequence (0x48 after 0x50).
}

void CFileHandle::addFallbackHandle(unsigned long param) {
    for (int i = 0; i < 4; i++) {
        if (field_0x18[i] == 0xFFFFFFFF) {
            field_0x18[i] = param;
            return;
        }
        // Next fallback slot (retail bumps a byte pointer by 4).
        // (one slot per iteration; empty slots read 0xFFFFFFFF).
    }
}

void CFileHandle::destroy(u32 size, u32 param_5, int param_6) {
    switch (field_0x00) {
    case 0:
    case 2: {
        // Free any previously loaded buffer first.
        void* pCache = (void*)field_0x04;
        if (pCache != NULL && field_0x30 == 0) {
            if (field_0x00 == 2) {
                // DVD-backed cache must be released before freeing.
                if (!CRsrc::releaseCache((const void*)field_0x04) && field_0x04 != 0) {
                    delete (void*)field_0x04;
                    field_0x04 = 0;
                }
            } else {
                delete pCache;
                field_0x04 = 0;
            }
        }

        // Choose an allocator handle: prefer field_0x14 when its largest
        // block covers size+0x100; otherwise scan the four fallbacks. A
        // fallback whose low halfword is 0xFFFF is a placeholder and is
        // replaced by the MEM2 handle.
        mtl::ALLOC_HANDLE handle = (mtl::ALLOC_HANDLE)field_0x14;
        if (mtl::MemManager::getMaxAllocSize(handle) <= size + 0x100) {
            for (int i = 0; i < 4; i++) {
                handle = field_0x18[i];
                bool invalid = (((u32)handle + 0x10000) & 0xFFFF) == 0xFFFF;
                if (!invalid && mtl::MemManager::getMaxAllocSize(handle) > size + 0x100) {
                    break;
                }
            }
            if ((((u32)handle + 0x10000) & 0xFFFF) == 0xFFFF) {
                handle = mtl::MemManager::getHandleMEM2();
            }
        }
        // Round up to a param_5 boundary, then allocate from head or tail
        // depending on flag bit 1 of field_0x58.
        bool useTail = ((field_0x58 >> 2) & 1) != 0;
        mtl::MemManager::setOptimalAlloc(useTail);
        if (field_0x58 & 2) {
            void* pBlock = mtl::MemManager::allocate_tail(handle, size, param_5);
            field_0x3C = size;
            field_0x04 = (u32)pBlock;
            field_0x08 = (u32)pBlock;
            field_0x0C =
                (size % param_5 != 0) ? size + param_5 - size % param_5 : size;
        } else {
            void* pBlock = mtl::MemManager::allocate_head(handle, size, param_5);
            field_0x3C = size;
            field_0x04 = (u32)pBlock;
            field_0x08 = (u32)pBlock;
            field_0x0C =
                (size % param_5 != 0) ? size + param_5 - size % param_5 : size;
        }
        if (field_0x04 == 0) {
            field_0x58 |= 8;
        } else {
            field_0x58 &= ~8;
        }
        mtl::MemManager::setOptimalAlloc(false);
        break;
    }
    case 4: {
        // Re-allocation path without releasing anything.
        bool useTail = ((field_0x58 >> 2) & 1) != 0;
        mtl::MemManager::setOptimalAlloc(useTail);
        if (field_0x58 & 2) {
            void* pBlock = mtl::MemManager::allocate_tail(
                (mtl::ALLOC_HANDLE)field_0x14, size, param_5);
            field_0x3C = size;
            field_0x04 = (u32)pBlock;
            field_0x08 = (u32)pBlock;
            field_0x0C =
                (size % param_5 != 0) ? size + param_5 - size % param_5 : size;
        } else {
            void* pBlock = mtl::MemManager::allocate_head(
                (mtl::ALLOC_HANDLE)field_0x14, size, param_5);
            field_0x3C = size;
            field_0x04 = (u32)pBlock;
            field_0x08 = (u32)pBlock;
            field_0x0C =
                (size % param_5 != 0) ? size + param_5 - size % param_5 : size;
        }
        if (field_0x04 == 0) {
            field_0x58 |= 8;
        } else {
            field_0x58 &= ~8;
        }
        mtl::MemManager::setOptimalAlloc(false);
        break;
    }
    case 1:
        field_0x3C = size;
        field_0x0C = param_6;
        break;
    }
}

int CFileHandle::advanceReadProgress(int param) {
    field_0x10 += param;
    return 1;
}

void* CFileHandle::getRsrc() const { return CRsrc::getRsrc(field_0x14); }

CDeviceFileDvd* CDeviceFileDvd::spInstance;
