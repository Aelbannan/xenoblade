// Auto-scaffolded catalog TU for monolib/src/device/CDeviceFile
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <string.h>

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// Definitions for the retail data sections (see the full blob at the bottom
// of the file).  The three 1/1/6-byte .sdata bytes (language override +
// standby flag) are separate u8 objects so references emit the retail
// per-address symbols (chars have 1-byte alignment, so packing matches).
extern "C" u8 lbl_eu_806636A8 = 0x01;
u8 lbl_eu_806636A9 = 0x01;   // standby-login done flag
u8 lbl_eu_806636AA = 0xFF;   // language override (-1 = auto)
u32 lbl_eu_80665660;   // CDeviceFile singleton pointer (.sbss)
int lbl_eu_80665664;   // filename-substitution table entry count (.sbss)
void* lbl_eu_80657580[16]; // filename-substitution table (.bss, 64B)
extern u8 lbl_eu_806575C0[0x108]; // .bss 264B (defined at bottom)
extern "C" u32 lbl_eu_8056C30C[3];
extern "C" u32 lbl_eu_8056C324[3]; // reslist<CFileHandle> vtable (defined below)
extern "C" u32 lbl_eu_8056C250[40]; // CDeviceFile vtable (defined below)
extern const char lbl_eu_80522BE4[]; // device-name string table (defined below)

// Minimal class views: the member/static signatures below mirror the retail
// mangled names exactly, so calls emit the retail reloc names. Full
// definitions live in the include/ headers, which this TU cannot use because
// it defines its own CDeviceFile.
class CWorkThread;
class CFileHandle;
class CDeviceFileJob;
class CDeviceFile;
struct CWorkThreadListNode;

// --- imports declared locally (this TU defines its own class views, so the
// --- real monolib headers cannot be included here) ---

class IWorkEvent;

// Language-substitution helper (defined below in this TU).
void func_eu_804520D0(char* pPath);

// Pack-archive queries shared with CWorkSystemPack.
extern "C" int func_804DE010(const char* pName);
extern "C" bool func_804DDD54(const char* pName, const char* pPath, char** outPkbPath,
                              u32* outB, u32* outC, u32* outD);

// Job ctor is old-MWCC-mangled; called through its retail name.
extern "C" void* __ct__21CDeviceFileJobReadDvdFPCcP11CWorkThread(void* self,
                                                                 const char* pName,
                                                                 CWorkThread* pParent);

namespace mtl {
class MemManager {
public:
    static u8* allocate(u32 size, u32 handle);
    static void* allocate_array(u32 size, u32 handle);
};
}

class CWorkThreadSystem {
public:
    static u32 getWorkMem();
};

class CWorkUtil {
public:
    static void entryWork(CWorkThread* pChild, CWorkThread* pParent, bool prepend);
};

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

// CMsgParam message-queue slot (0x24 bytes each; command at +0).
struct CMsgParamEntry {
    u32 command; //0x0 (message id / EVT value)
    u32 wid;     //0x4
    u32 unk8;
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u16 unk20;
    u8 unk22;
    u8 unk23;
};

class CWorkThread {
public:
    CWorkThread(const char* pName, CWorkThread* pParent, int capacity);
    ~CWorkThread();
    enum EVT {
        EVT_NONE,
        EVT_1,
        EVT_EXCEPTION,
        EVT_3,
        EVT_4,
        EVT_PAUSE,
        EVT_UNPAUSE,
        EVT_7,
        EVT_8,
        EVT_9,
        EVT_APPEXCEPTION_ON,
        EVT_APPEXCEPTION_OFF,
    };

    enum ThreadFlags {
        THREAD_FLAG_EXCEPTION = (1 << 4),
    };

    enum ThreadState {
        THREAD_STATE_LOGIN = 2,
        THREAD_STATE_RUN = 3,
    };

    bool wkStandbyLogin();
    bool wkStandbyLogout();
    void wkSetEvent(EVT evt);

    //0x0..0x1C4: CWorkThread base (name/state/id/type/alloc/parent)
    u32* volatile vptr;              //0x0 (vtable, set by each ctor)
    u8 field_0x4[0x48 - 0x4];        //0x4
    int mState;                      //0x48 (ThreadState)
    u8 field_0x4C[0x50 - 0x4C];      //0x4C
    u32 field_0x50;                  //0x50 (device job kind tag; valid range 0x41..0x4E)
    u8 field_0x54[0x5C - 0x54];      //0x54
    u32 field_0x5C;                  //0x5C (children reslist vtable)
    CWorkThreadListNode* mChildList; //0x60 (children reslist mStartNodePtr)
    u8 field_0x64[0x70 - 0x64];      //0x64
    u32 field_0x70;                  //0x70 (device job pool; 0x0C-stride CDevJobSlot entries)
    u32 field_0x74;                  //0x74 (device job pool entry count)
    u8 field_0x78[0x7C - 0x78];
    u32 mThreadFlags;                //0x7C (ThreadFlags; CDeviceBase::mFlags shadows the name at 0x1C4)
    u8 field_0x80[0x1A4 - 0x80];     //0x80..0x1A4 (CMsgParam<8> vtable + entries)
    CMsgParamEntry* mMsgArray;       //0x1A4 (CMsgParam::mArrayPtr)
    u32 mMsgFront;                   //0x1A8 (CMsgParam::mFront)
    u32 mMsgSize;                    //0x1AC (CMsgParam::mSize)
    u32 mMsgCapacity;                //0x1B0 (CMsgParam::mCapacity)
    u8 field_0x1B4[0x1C4 - 0x1B4];   //0x1B4..0x1C4
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

// Child-list node layout (reslist sentinel, see monolib/util/reslist.hpp)
struct CWorkThreadListNode {
    CWorkThreadListNode* mNext; //0x0
    CWorkThreadListNode* mPrev; //0x4
    CWorkThread* mItem;         //0x8
};

// _reslist_node<CFileHandle*> layout (job list member of CDeviceFile)
struct CFileHandleListNode {
    CFileHandleListNode* mNext; //0x0
    CFileHandleListNode* mPrev; //0x4
    CFileHandle* mItem;         //0x8
};

// _reslist_node<CFileHandle> layout (reslist<CFileHandle> member list)
struct CFileHandleReslistNode {
    CFileHandleReslistNode* mNext; //0x0
    CFileHandleReslistNode* mPrev; //0x4
    u32 mItem;                     //0x8
};

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

// _reslist_base<CFileHandle> layout
struct CFileHandleReslist {
    union {
        u32* m_vtable;                     //0x0
        u32* volatile m_vtable_raw;        //0x0 (base-construction store)
    };
    CFileHandleReslistNode* mStartNodePtr; //0x4
    CFileHandleReslistNode mStartNode;     //0x8..0x14 (embedded sentinel)
    CDeviceFilePoolEntry* mList;           //0x14
    int mCapacity;                         //0x18
    u8 field_0x1C;                         //0x1C (owns-list flag)
};

class CDeviceFileJob : public CWorkThread {
public:
    CFileHandle* mHandle;  //0x1C4 (current job's file handle)
};

class CDeviceBase : public CWorkThread {
public:
    CDeviceBase(const char* pName, CWorkThread* pParent, int capacity)
        : CWorkThread(pName, pParent, capacity) {}
    ~CDeviceBase() {}

    u32 volatile mFlags; //0x1C4
};

class CDeviceFile : public CDeviceBase {
public:
    CDeviceFile(const char* name, CWorkThread* parent);
    ~CDeviceFile();
    static void cancel(CFileHandle* pFileHandle);
    static int setParent(CWorkThread* parent);
    bool clearParent();
    static void cancelJobsForPath(const char* pPath);
    static bool tryUpdateJobPriority(CFileHandle* pFileHandle, int param);
    bool func_8044F1B8(CFileHandle* pFileHandle, int param);
    static void setHandleParam(CFileHandle* pHandle, unsigned long param);
    void getFileSize();
    int isInitialized();
    static CFileHandle* readCommonArchiveFile(unsigned long allocHandle, const char* pPath,
                                               IWorkEvent* pEvent, int offsetUnits, int sizeUnits);
    static CFileHandle* readFile(unsigned long allocHandle, const char* pPath,
                                 IWorkEvent* pEvent, int offsetUnits, int sizeUnits);
    static bool removeFileJob(CDeviceFileJob* job);
    static void setHandleFlag1(CFileHandle* pFileHandle);
    static void setHandleFlag2(CFileHandle* pFileHandle);
    bool wkStandbyLogin();
    bool wkStandbyLogout();

    //0x1C8..0x1F0: CDeviceFile tail
    CWorkThread* field_0x1C8;     //0x1C8 (standby state field)
    CFileHandleReslist mFileList; //0x1CC (reslist<CFileHandle>; mList=pool @0x1E0)
    u8 field_0x1EC[0x1F0 - 0x1EC]; //0x1EC
};

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
    vptr = (u32*)lbl_eu_8056C250;                // CDeviceFile vtable
    field_0x1C8 = NULL;
    mFileList.m_vtable_raw = lbl_eu_8056C324;    // _reslist_base vtable
    mFileList.mList = NULL;                      //0x1E0
    mFileList.mCapacity = 0;                     //0x1E4
    mFileList.field_0x1C = 0;
    mFileList.mStartNodePtr = &mFileList.mStartNode;
    mFileList.mStartNode.mNext = mFileList.mStartNodePtr;
    mFileList.mStartNode.mPrev = mFileList.mStartNodePtr;
    mFileList.m_vtable = lbl_eu_8056C30C;        // reslist vtable (derived)
    lbl_eu_80665660 = (u32)this;
    lbl_eu_806636AA = 0xFF;
    mFileList.mList = (CDeviceFilePoolEntry*)mtl::MemManager::allocate_array(
        0xC00, CWorkThreadSystem::getWorkMem());
    // Clear the leading word of every 0x0C sub-slot in all 32 entries.
    for (int i = 0; i < 32; i++) {
        mFileList.mList[i].head = 0;
        mFileList.mList[i].slot0C = 0;
        mFileList.mList[i].slot18 = 0;
        mFileList.mList[i].slot24 = 0;
        mFileList.mList[i].slot30 = 0;
        mFileList.mList[i].slot3C = 0;
        mFileList.mList[i].slot48 = 0;
        mFileList.mList[i].slot54 = 0;
    }
    mFileList.mCapacity = 0x100;
}

// reslist<CFileHandle> deleting destructors. Retail emits these under old
// (unmangled) template names, so they are written as C-linkage free functions.
extern "C" {

CFileHandleReslist* __dt___reslist_base_CFileHandle(CFileHandleReslist* t, int deleting) {
    if (t != 0) {
        t->m_vtable = lbl_eu_8056C324;
        CFileHandleReslistNode* node = t->mStartNodePtr->mNext;
        while (node != t->mStartNodePtr) {
            CFileHandleReslistNode* prev = node;
            node = node->mNext;
            prev->mNext = 0;
        }
        t->mStartNodePtr->mNext = t->mStartNodePtr;
        t->mStartNodePtr->mPrev = t->mStartNodePtr;
        if (t->field_0x1C == 0) {
            if (t->mList != 0) {
                delete[] t->mList;
                t->mList = 0;
            }
        }
        if (deleting > 0) {
            delete t;
        }
    }
    return t;
}

CFileHandleReslist* __dt__reslist_CFileHandle(CFileHandleReslist* t, int deleting) {
    if (t != 0) {
        if (t != 0) {
            t->m_vtable = lbl_eu_8056C324;
            CFileHandleReslistNode* node = t->mStartNodePtr->mNext;
            while (node != t->mStartNodePtr) {
                CFileHandleReslistNode* prev = node;
                node = prev->mNext;
                prev->mNext = 0;
            }
            t->mStartNodePtr->mNext = t->mStartNodePtr;
            t->mStartNodePtr->mPrev = t->mStartNodePtr;
            if (t->field_0x1C == 0) {
                if (t->mList != 0) {
                    delete[] t->mList;
                    t->mList = 0;
                }
            }
        }
        if (deleting > 0) {
            delete t;
        }
    }
    return t;
}

}

CDeviceFile::~CDeviceFile() {
    CFileHandleReslist* list = &mFileList;
    lbl_eu_80665660 = 0;
    if (list != 0) {
        if (list != 0) {
            list->m_vtable = lbl_eu_8056C324;
            CFileHandleReslistNode* node = list->mStartNodePtr->mNext;
            while (node != list->mStartNodePtr) {
                CFileHandleReslistNode* prev = node;
                node = node->mNext;
                prev->mNext = 0;
            }
            list->mStartNodePtr->mNext = list->mStartNodePtr;
            list->mStartNodePtr->mPrev = list->mStartNodePtr;
            if (list->field_0x1C == 0 && list->mList != 0) {
                delete[] list->mList;
                list->mList = 0;
            }
        }
    }
}

u32 getInstance__11CDeviceFileFv(void) { return lbl_eu_80665660; }

// A device thread is ready when it has no pending EVT_EXCEPTION (flag bit or
// queued message) and its worker state is LOGIN or RUN.
static inline bool isDeviceFileReady(CDeviceFile* dev) {
    bool hasException;
    if (dev->mThreadFlags & CWorkThread::THREAD_FLAG_EXCEPTION) {
        hasException = true;
    } else {
        // Scan the pending-message ring for a queued EVT_EXCEPTION; an index
        // of -1 means none was found.
        int idx;
        for (idx = 0; idx < dev->mMsgSize; idx++) {
            if (dev->mMsgArray[(dev->mMsgFront + idx) % dev->mMsgCapacity].command !=
                CWorkThread::EVT_EXCEPTION) {
                continue;
            }
            break;
        }
        if (idx >= dev->mMsgSize) {
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
    if (!isDeviceFileReady((CDeviceFile*)lbl_eu_80665660)) return 0;

    if (CDeviceFileDvd::getInstance() == NULL) return 0;
    if (!isDeviceFileReady(CDeviceFileDvd::getInstance())) return 0;

    if (CDeviceFileCri::getInstance() == NULL) return 0;
    return isDeviceFileReady(CDeviceFileCri::getInstance()) ? 1 : 0;
}

extern "C" u8 isStandbyReady__11CDeviceFileFv() { return lbl_eu_806636A8; }

int CDeviceFile::setParent(CWorkThread* parent) {
    // spInstance (lbl_eu_80665660)->field_1C8 = parent; return true
    *(void**)((char*)(u32)lbl_eu_80665660 + 0x1C8) = parent;
    return 1;
}

bool CDeviceFile::clearParent() {
    // spInstance (lbl_eu_80665660)->field_0x1C8 = 0; return true
    ((CDeviceFile*)(u32)lbl_eu_80665660)->field_0x1C8 = 0;
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
    pathLen = strlen(pPath);
    strcpy(pathCopy, pPath);
    func_eu_804520D0(pathCopy);

    if (archiveMode && allocHandle == 0) {
        return NULL;
    }

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
    slash = lbl_eu_80522BE4 + 0x1D;
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
        if (func_804DE010(nameBuf)) {
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
        mgr = (CDeviceFile*)lbl_eu_80665660;
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
        func_eu_804520D0(pkbBuf);
        pkbPath = pkbBuf;
    }
    if (pkbPath != NULL) {
        handle->field_0x1A4 = strlen(pkbPath);
        strcpy(handle->field_0x184, pkbPath);
    }

    // Only relative paths reach the device queue.
    pathLen = strlen(pathCopy);
    if (pathLen > 6 && strstr(pathCopy, lbl_eu_80522BE4) == pathCopy) {
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
        if (lbl_eu_806636A8 == 0) {
            CDeviceFile* dvd = CDeviceFileDvd::getInstance();
            job = (CDeviceFileJob*)mtl::MemManager::allocate(
                0x250, CWorkThreadSystem::getWorkMem());
            if (job != NULL) {
                job = (CDeviceFileJob*)__ct__21CDeviceFileJobReadDvdFPCcP11CWorkThread(
                    job, lbl_eu_80522BE4 + 7, dvd);
            }
            CWorkUtil::entryWork(job, dvd, false);
        } else {
            CDeviceFile* cri = CDeviceFileCri::getInstance();
            job = (CDeviceFileJob*)mtl::MemManager::allocate(
                0x250, CWorkThreadSystem::getWorkMem());
            if (job != NULL) {
                job = (CDeviceFileJob*)__ct__21CDeviceFileJobReadDvdFPCcP11CWorkThread(
                    job, lbl_eu_80522BE4 + 7, cri);
            }
            CWorkUtil::entryWork(job, cri, false);
        }
    }

    job->mHandle = handle;
    u32 prio = handle->field_0x48;
    if (!CDeviceFileDvd::getInstance()->func_8044F1B8(handle, prio)) {
        CDeviceFileCri::getInstance()->func_8044F1B8(handle, prio);
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

// File-size dispatch: device-specific getters are old-MWCC-mangled imports
// that take an extra (mangle-invisible) trailing arg forwarded from r4.
extern "C" int getFileSize__14CDeviceFileDvdFPCc(const char* pPath, int param);
extern "C" int getFileSize__14CDeviceFileCriFPCc(const char* pPath, int param);

// CDeviceFile::getFileSize(const char*). Returns -1 when the path is not a
// plain relative path (absolute prefix, drive colon, or backslash root).
// Only relative paths reach the DVD/CRI device query.
extern "C" int getFileSize__11CDeviceFileFPCc(const char* path, int param) {
    int result = -1;
    int absolute;
    int len = strlen(path);
    if (len > 6 && strstr(path, lbl_eu_80522BE4) == path) {
        absolute = 1;
    } else if (len > 1 && path[1] == ':') {
        absolute = 1;
    } else if (len <= 0) {
        absolute = 0;
    } else {
        s8 c = (s8)path[0];
        absolute = 0;
        if (c == '/') {
            absolute = 0;
        } else if (c == '\\') {
            absolute = 1;
        }
    }
    if (!absolute) {
        if (lbl_eu_806636A8) {
            result = getFileSize__14CDeviceFileCriFPCc(path, param);
        } else {
            result = getFileSize__14CDeviceFileDvdFPCc(path, param);
        }
    }
    return result;
}

bool CDeviceFile::removeFileJob(CDeviceFileJob* job) {
    bool result;
    CFileHandle* handle = job->mHandle;
    CDeviceFile* inst = (CDeviceFile*)lbl_eu_80665660;
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
    if (CDeviceFileDvd::getInstance()->func_8044F1B8(pFileHandle, param)) {
        return true;
    }
    return CDeviceFileCri::getInstance()->func_8044F1B8(pFileHandle, param);
}

bool CDeviceFile::func_8044F1B8(CFileHandle* pFileHandle, int param) {
    // Request-state check over this device's child-job ring: find the queued
    // job holding pFileHandle, then either bump its priority (sole child) or
    // preempt a lower-priority sibling by parking nodes in the 0x70 job pool.
    if (this == NULL) {
        return false;
    }

    CWorkThreadListNode* node = mChildList->mNext;
    CDeviceFileJob* foundJob;
    while (node != mChildList) {
        // A list item participates only when its kind tag is in [0x41, 0x4F).
        CDeviceFileJob* job = (CDeviceFileJob*)node->mItem;
        if (job != NULL) {
            if (job->field_0x50 < 0x41) {
                job = NULL;
            } else if (job->field_0x50 >= 0x4F) {
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
    if (node == mChildList) {
        return false;
    }

    // Count the children by walking the whole ring once.
    int idx = 0;
    CWorkThreadListNode* walk = mChildList->mNext;
    do {
        walk = walk->mNext;
        idx++;
    } while (walk != mChildList);

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

    if (mChildList->mNext != mChildList) {
        for (CWorkThreadListNode* scan = mChildList->mNext; scan != mChildList;
             scan = scan->mNext) {
            CDeviceFileJob* job = (CDeviceFileJob*)scan->mItem;
            if (job != NULL) {
                if (job->field_0x50 < 0x41) {
                    job = NULL;
                } else if (job->field_0x50 >= 0x4F) {
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
                for (freeIdx = 0; freeIdx < field_0x74; freeIdx++) {
                    if (((CDevJobSlot*)field_0x70)[freeIdx].mNode == NULL) break;
                }
                CWorkThreadListNode* slotNode =
                    (CWorkThreadListNode*)&((CDevJobSlot*)field_0x70)[freeIdx];
                if (&((CDevJobSlot*)field_0x70)[freeIdx].mJob != NULL) {
                    ((CDevJobSlot*)field_0x70)[freeIdx].mJob = foundJob;
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
    for (freeIdx2 = 0; freeIdx2 < field_0x74; freeIdx2++) {
        if (((CDevJobSlot*)field_0x70)[freeIdx2].mNode == NULL) break;
    }
    CWorkThreadListNode* slotNode2 =
        (CWorkThreadListNode*)&((CDevJobSlot*)field_0x70)[freeIdx2];
    if (&((CDevJobSlot*)field_0x70)[freeIdx2].mJob != NULL) {
        ((CDevJobSlot*)field_0x70)[freeIdx2].mJob = foundJob;
    }
    slotNode2->mNext = (CWorkThreadListNode*)mChildList;
    slotNode2->mPrev = ((CWorkThreadListNode*)mChildList)->mPrev;
    ((CWorkThreadListNode*)mChildList)->mPrev->mNext = slotNode2;
    ((CWorkThreadListNode*)mChildList)->mPrev = slotNode2;
    return true;
}

void CDeviceFile::setHandleParam(CFileHandle* pHandle, unsigned long param) {
    pHandle->addFallbackHandle(param);
}

void CDeviceFile::setHandleFlag1(CFileHandle* pFileHandle) {
    *(u32*)((char*)pFileHandle + 0x58) |= 2;
}

void CDeviceFile::setHandleFlag2(CFileHandle* pFileHandle) {
    *(u32*)((char*)pFileHandle + 0x58) |= 4;
}

bool CDeviceFile::wkStandbyLogin() {
    if (CDevice::isColdStartReady()) {
        const char* dvdName = lbl_eu_80522BE4 + 0x8B;
        CWorkThread* dvd = (CWorkThread*)mtl::MemManager::allocate(0x1D8, CWorkThreadSystem::getWorkMem());
        if (dvd != NULL) {
            dvd = (CWorkThread*)__ct__CDeviceFileDvd(dvd, dvdName, this);
        }
        CWorkUtil::entryWork(dvd, this, false);

        const char* criName = lbl_eu_80522BE4 + 0x9A;
        CWorkThread* cri = (CWorkThread*)mtl::MemManager::allocate(0x1F0, CWorkThreadSystem::getWorkMem());
        if (cri != NULL) {
            cri = (CWorkThread*)__ct__CDeviceFileCri(cri, criName, this);
        }
        CWorkUtil::entryWork(cri, this, false);

        lbl_eu_806636A9 = 1;
        return CWorkThread::wkStandbyLogin();
    }
    return false;
}

bool CDeviceFile::wkStandbyLogout() {
    // Log out only when the child list is empty and the work-system/lib
    // singletons have been released.
    if (mChildList->mNext == mChildList &&
        CWorkSystem::getInstance() == NULL &&
        CLib::getInstance() == NULL) {
        return ((CWorkThread*)this)->wkStandbyLogout();
    }
    return false;
}

extern "C" void func_eu_804520B0(void* r3) {
    int idx = (int)lbl_eu_80665664;
    lbl_eu_80657580[idx] = r3;
    lbl_eu_80665664 = idx + 1;
}

// Language-aware filename substitution. Walks the registered filename table
// (fed by func_eu_804520B0 / func_eu_804521A8) and, when the path contains a
// table entry's search string, overwrites that substring in place with the
// language-specific variant (each entry is a NULL-terminated string array
// where [0] is the search text and [lang] the replacement).
void func_eu_804520D0(char* pPath) {
    char* found;
    const char* const** pTable;
    int count;
    u8 lang;
    int i;
    const char* search;

    lang = CDeviceSC::getLanguage();
    u8 langOverride = lbl_eu_806636AA;
    if ((s8)langOverride >= 0) {
        lang = langOverride;
    }
    if (lang == 0) return;

    if (CDeviceSC::getLanguage() > 5) {
        lang = 1;
    }

    count = lbl_eu_80665664;
    pTable = (const char* const**)lbl_eu_80657580;
    for (i = 0; i < count; i++) {
        search = pTable[0][0];
        found = strstr(pPath, search);
        if (found != NULL) {
            memcpy(found, ((const char* const*)lbl_eu_80657580[i])[lang + 1], strlen(search));
            return;
        }
        pTable++;
    }
}

// These two retail data symbols are also declared in the "C-linkage imports"
// section of monolib/device/CDeviceFile.hpp. They stay declared here too
// because this catalog TU defines a local `struct CDeviceFile` that conflicts
// with the class in that header, so this TU cannot include it.
extern "C" void func_eu_804521A8(s8 val) {
    lbl_eu_806636AA = val;
}

extern "C" void func_eu_804521B0() {
    lbl_eu_806636AA = -1;
}

extern "C" void func_eu_804521BC(u8 val) {
    lbl_eu_806636A9 = val;
}

extern "C" u8 func_eu_804521C4() {
    return lbl_eu_806636A9;
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
    extern unsigned char lbl_eu_806575C0[];
    lbl_eu_806575C0[0] = 0;
    *(unsigned int*)(lbl_eu_806575C0 + 0x100) = 0;
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056C250-0x8056C330 (224B): CDeviceFile vtable (160B) + RTTI
// chain (28B) + reslist<CFileHandle>/_reslist_base<CFileHandle> vtables.
namespace DeviceFileBlob {
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
extern "C" void wkUpdate__11CWorkThreadFv();
extern "C" void wkRender__11CWorkThreadFv();
extern "C" void wkRenderAfter__11CWorkThreadFv();
extern "C" void wkStandbyExceptionRetry__11CWorkThreadFUl();
extern "C" void __dt__11CDeviceFileFv();       // defined below (member dtor)
extern "C" bool wkStandbyLogin__11CDeviceFileFv();  // defined below
extern "C" bool wkStandbyLogout__11CDeviceFileFv(); // defined below
extern "C" u32 __RTTI__10IWorkEvent;
extern "C" u32 __RTTI__11CWorkThread;
extern "C" u32 lbl_eu_806635F0;  // CFileHandle RTTI locator (foreign TU)
}
extern "C" u32 lbl_eu_806636B0[2]; // .sdata RTTI locator (defined below)
extern "C" u32 lbl_eu_806636B8[2];
extern "C" u32 lbl_eu_806636C0[2];
extern "C" u32 lbl_eu_8056C2F0[7];
extern "C" u32 lbl_eu_8056C30C[3];
extern "C" u32 lbl_eu_8056C318[3];

extern "C" u32 lbl_eu_8056C250[40] = {
    (u32)&lbl_eu_806636B0, 0x00000000, (u32)&DeviceFileBlob::__dt__11CDeviceFileFv,
    (u32)&DeviceFileBlob::WorkEvent1__10IWorkEventFPvPCc,
    (u32)&DeviceFileBlob::OnFileEvent__10IWorkEventFP10CEventFile,
    (u32)&DeviceFileBlob::WorkEvent3__10IWorkEventFPv,
    (u32)&DeviceFileBlob::WorkEvent4__10IWorkEventFv,
    (u32)&DeviceFileBlob::OnPauseTrigger__10IWorkEventFb,
    (u32)&DeviceFileBlob::WorkEvent6__10IWorkEventFv,
    (u32)&DeviceFileBlob::WorkEvent7__10IWorkEventFv,
    (u32)&DeviceFileBlob::WorkEvent8__10IWorkEventFv,
    (u32)&DeviceFileBlob::WorkEvent9__10IWorkEventFv,
    (u32)&DeviceFileBlob::WorkEvent10__10IWorkEventFv,
    (u32)&DeviceFileBlob::WorkEvent11__10IWorkEventFv,
    (u32)&DeviceFileBlob::WorkEvent12__10IWorkEventFv,
    (u32)&DeviceFileBlob::WorkEvent13__10IWorkEventFv,
    (u32)&DeviceFileBlob::WorkEvent14__10IWorkEventFv,
    (u32)&DeviceFileBlob::WorkEvent15__10IWorkEventFv,
    (u32)&DeviceFileBlob::WorkEvent16__10IWorkEventFv,
    (u32)&DeviceFileBlob::WorkEvent17__10IWorkEventFv,
    (u32)&DeviceFileBlob::WorkEvent18__10IWorkEventFv,
    (u32)&DeviceFileBlob::WorkEvent19__10IWorkEventFv,
    (u32)&DeviceFileBlob::WorkEvent20__10IWorkEventFv,
    (u32)&DeviceFileBlob::WorkEvent21__10IWorkEventFv,
    (u32)&DeviceFileBlob::WorkEvent22__10IWorkEventFv,
    (u32)&DeviceFileBlob::WorkEvent23__10IWorkEventFv,
    (u32)&DeviceFileBlob::WorkEvent24__10IWorkEventFv,
    (u32)&DeviceFileBlob::WorkEvent25__10IWorkEventFv,
    (u32)&DeviceFileBlob::WorkEvent26__10IWorkEventFv,
    (u32)&DeviceFileBlob::WorkEvent27__10IWorkEventFv,
    (u32)&DeviceFileBlob::WorkEvent28__10IWorkEventFv,
    (u32)&DeviceFileBlob::WorkEvent29__10IWorkEventFv,
    (u32)&DeviceFileBlob::WorkEvent30__10IWorkEventFv,
    (u32)&DeviceFileBlob::WorkEvent31__10IWorkEventFv,
    (u32)&DeviceFileBlob::wkUpdate__11CWorkThreadFv,
    (u32)&DeviceFileBlob::wkRender__11CWorkThreadFv,
    (u32)&DeviceFileBlob::wkRenderAfter__11CWorkThreadFv,
    (u32)&DeviceFileBlob::wkStandbyLogin__11CDeviceFileFv,
    (u32)&DeviceFileBlob::wkStandbyLogout__11CDeviceFileFv,
    (u32)&DeviceFileBlob::wkStandbyExceptionRetry__11CWorkThreadFUl,
};
extern "C" u32 lbl_eu_8056C2F0[7] = {
    (u32)&DeviceFileBlob::__RTTI__10IWorkEvent, 0x00000000,
    (u32)&DeviceFileBlob::__RTTI__11CWorkThread, 0x00000000,
    (u32)&DeviceFileBlob::lbl_eu_806635F0, 0x00000000, 0x00000000,
};
extern "C" u32 lbl_eu_8056C30C[3] = {
    (u32)&lbl_eu_806636B8, 0x00000000, (u32)&__dt__reslist_CFileHandle,
};
extern "C" u32 lbl_eu_8056C318[3] = {
    (u32)&lbl_eu_806636C0, 0x00000000, 0x00000000,
};
extern "C" u32 lbl_eu_8056C324[3] = {
    (u32)&lbl_eu_806636C0, 0x00000000, (u32)&__dt___reslist_base_CFileHandle,
};

// [.rodata] 0x80522BA0-0x80522C90 (240B): RTTI names + device-name table.
extern "C" __declspec(align(4)) const char lbl_eu_80522BA0[12] = {
    0x43,0x44,0x65,0x76,0x69,0x63,0x65,0x46,0x69,0x6C,0x65,0x00,
};
extern "C" __declspec(align(4)) const char lbl_eu_80522BAC[23] = {
    0x72,0x65,0x73,0x6C,0x69,0x73,0x74,0x3C,0x43,0x46,0x69,0x6C,0x65,0x48,0x61,0x6E,
    0x64,0x6C,0x65,0x20,0x2A,0x3E,0x00,
};
extern "C" __declspec(align(4)) const char lbl_eu_80522BC4[29] = {
    0x5F,0x72,0x65,0x73,0x6C,0x69,0x73,0x74,0x5F,0x62,0x61,0x73,0x65,0x3C,0x43,0x46,
    0x69,0x6C,0x65,0x48,0x61,0x6E,0x64,0x6C,0x65,0x20,0x2A,0x3E,0x00,
};
extern "C" const char lbl_eu_80522BE4[172] = {
    'U','S','B','K','E','Y',0x00,
    0x43,0x44,0x65,0x76,0x69,0x63,0x65,0x46,0x69,0x6C,0x65,0x4A,0x6F,0x62,0x52,0x65,0x61,0x64,0x44,0x76,0x64,0x00,
    0x2F,0x00,
    0x43,0x44,0x65,0x76,0x69,0x63,0x65,0x46,0x69,0x6C,0x65,0x4A,0x6F,0x62,0x56,0x61,0x6C,0x69,0x64,0x50,0x61,0x74,0x68,0x44,0x76,0x64,0x00,
    0x43,0x44,0x65,0x76,0x69,0x63,0x65,0x46,0x69,0x6C,0x65,0x4A,0x6F,0x62,0x56,0x61,0x6C,0x69,0x64,0x50,0x61,0x74,0x68,0x43,0x72,0x69,0x00,
    0x43,0x44,0x65,0x76,0x69,0x63,0x65,0x46,0x69,0x6C,0x65,0x4A,0x6F,0x62,0x56,0x61,0x6C,0x69,0x64,0x46,0x69,0x6C,0x65,0x44,0x76,0x64,0x00,
    0x43,0x44,0x65,0x76,0x69,0x63,0x65,0x46,0x69,0x6C,0x65,0x4A,0x6F,0x62,0x56,0x61,0x6C,0x69,0x64,0x46,0x69,0x6C,0x65,0x43,0x72,0x69,0x00,
    0x43,0x44,0x65,0x76,0x69,0x63,0x65,0x46,0x69,0x6C,0x65,0x44,0x76,0x64,0x00,
    0x43,0x44,0x65,0x76,0x69,0x63,0x65,0x46,0x69,0x6C,0x65,0x43,0x72,0x69,0x00,0x00,0x00,0x00,
};

// [.sdata] 0x806636A8-0x806636C8 (32B): flags + RTTI locators.
extern "C" u32 lbl_eu_806636B0[2] = { (u32)&lbl_eu_80522BA0, (u32)&lbl_eu_8056C2F0 };
extern "C" u32 lbl_eu_806636B8[2] = { (u32)&lbl_eu_80522BAC, (u32)&lbl_eu_8056C318 };
extern "C" u32 lbl_eu_806636C0[2] = { (u32)&lbl_eu_80522BC4, 0x00000000 };

// [.bss] 0x806575C0-0x806576C8 (264B).
u8 lbl_eu_806575C0[0x108];
