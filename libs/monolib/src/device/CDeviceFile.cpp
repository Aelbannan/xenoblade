// Auto-scaffolded catalog TU for monolib/src/device/CDeviceFile
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <string.h>

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// Definitions for the retail data sections (see the full blob at the bottom
// of the file).  The three 1/1/6-byte .sdata bytes (language override +
// standby flag) are folded into one 8-byte array so MWCC packs the section
// exactly like the retail (separate u8 objects would 4-align).
extern "C" u8 lbl_eu_806636A8[8] = { 0x01, 0x01, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00 };
u32 lbl_eu_80665660;   // CDeviceFile singleton pointer (.sbss)
int lbl_eu_80665664;   // filename-substitution table entry count (.sbss)
void* lbl_eu_80657580[16]; // filename-substitution table (.bss, 64B)
extern u8 lbl_eu_806575C0[0x108]; // .bss 264B (defined at bottom)
extern "C" u32 lbl_eu_8056C324[3]; // reslist<CFileHandle> vtable (defined below)
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

namespace mtl {
class MemManager {
public:
    static u8* allocate(u32 size, u32 handle);
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
extern "C" void __ct__CDeviceFileDvd(CWorkThread* pObj, const char* pName, CWorkThread* pParent);
extern "C" void __ct__CDeviceFileCri(CWorkThread* pObj, const char* pName, CWorkThread* pParent);

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
    u8 field_0x0[0x48];              //0x0
    int mState;                      //0x48 (ThreadState)
    u8 field_0x4C[0x5C - 0x4C];      //0x4C
    u32 field_0x5C;                  //0x5C (children reslist vtable)
    CWorkThreadListNode* mChildList; //0x60 (children reslist mStartNodePtr)
    u8 field_0x64[0x7C - 0x64];      //0x64..0x7C
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
    void func_80451984(unsigned long);

    u32 field_0x0;      //0x0
    u8* mData;          //0x4 (loaded file buffer)
    u32 field_0x8;      //0x8
    u8 field_0xC[0x14 - 0xC];
    u32 field_0x14;     //0x14
    u8 field_0x18[0x2C - 0x18];
    u32 field_0x2C;     //0x2C
    u8 field_0x30[0x3C - 0x30];
    u32 field_0x3C;     //0x3C
    u8 field_0x40[0x5C - 0x40];
    u8 mName[0x20];     //0x5C (FixStr<32>)
};

class CDeviceFileCri {
public:
    static CDeviceFile* getInstance();
    static void func_8044FB08();
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

// _reslist_base<CFileHandle> layout
struct CFileHandleReslist {
    u32* m_vtable;                       //0x0 (points at lbl_eu_8056C324)
    CFileHandleReslistNode* mStartNodePtr; //0x4
    u8 field_0x8[0x14 - 0x8];            //0x8..0x14 (embedded mStartNode)
    CFileHandleReslistNode* mList;       //0x14
    int mCapacity;                       //0x18
    u8 field_0x1C;                       //0x1C (owns-list flag)
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

    u32 mFlags; //0x1C4
};

class CDeviceFile : public CDeviceBase {
public:
    CDeviceFile(const char* name, void* parent);
    ~CDeviceFile();
    static void cancel(CFileHandle* pFileHandle);
    static int func_8044E770(CWorkThread* parent);
    bool func_8044E780();
    static void func_8044F0E4(const char* pPath);
    static bool func_8044F154(CFileHandle* pFileHandle, int param);
    bool func_8044F1B8(CFileHandle* pFileHandle, int param);
    static void func_8044F400(CFileHandle* pHandle, unsigned long param);
    void getFileSize();
    int isInitialized();
    void readCommonArchiveFile();
    void readFile();
    static bool removeFileJob(CDeviceFileJob* job);
    static void setHandleFlag1(CFileHandle* pFileHandle);
    static void setHandleFlag2(CFileHandle* pFileHandle);
    bool wkStandbyLogin();
    bool wkStandbyLogout();

    //0x1C8..0x1F0: CDeviceFile tail
    CWorkThread* field_0x1C8;     //0x1C8 (standby state field)
    CFileHandleReslist mFileList; //0x1CC (reslist<CFileHandle>)
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


CDeviceFile::CDeviceFile(const char* name, void* parent)
    : CDeviceBase(name, (CWorkThread*)parent, 8) {}

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

int CDeviceFile::isInitialized() {
    CDeviceFile* inst = (CDeviceFile*)lbl_eu_80665660;

    // Singleton device check: an exception flag or a queued EVT_EXCEPTION
    // event means the device is not initialized yet.
    bool hasException1;
    if (inst->mThreadFlags & CWorkThread::THREAD_FLAG_EXCEPTION) {
        hasException1 = true;
    } else {
        int found1 = -1;
        for (int i1 = 0; i1 < inst->mMsgSize; i1++) {
            if (inst->mMsgArray[(inst->mMsgFront + i1) % inst->mMsgCapacity].command ==
                CWorkThread::EVT_EXCEPTION) {
                found1 = i1;
                break;
            }
        }
        hasException1 = found1 >= 0;
    }
    int ok1 = 0;
    if (!hasException1) {
        if (inst->mState == CWorkThread::THREAD_STATE_LOGIN ||
            inst->mState == CWorkThread::THREAD_STATE_RUN) {
            ok1 = 1;
        }
    }
    if (!ok1) return 0;

    if (CDeviceFileDvd::getInstance() == NULL) return 0;
    CDeviceFile* dvd = CDeviceFileDvd::getInstance();

    // DVD device check.
    bool hasException2;
    if (dvd->mThreadFlags & CWorkThread::THREAD_FLAG_EXCEPTION) {
        hasException2 = true;
    } else {
        int found2 = -1;
        for (int i2 = 0; i2 < dvd->mMsgSize; i2++) {
            if (dvd->mMsgArray[(dvd->mMsgFront + i2) % dvd->mMsgCapacity].command ==
                CWorkThread::EVT_EXCEPTION) {
                found2 = i2;
                break;
            }
        }
        hasException2 = found2 >= 0;
    }
    int ok2 = 0;
    if (!hasException2) {
        if (dvd->mState == CWorkThread::THREAD_STATE_LOGIN ||
            dvd->mState == CWorkThread::THREAD_STATE_RUN) {
            ok2 = 1;
        }
    }
    if (!ok2) return 0;

    if (CDeviceFileCri::getInstance() == NULL) return 0;
    CDeviceFile* cri = CDeviceFileCri::getInstance();

    // CRI device check (result is the return value).
    bool hasException3;
    if (cri->mThreadFlags & CWorkThread::THREAD_FLAG_EXCEPTION) {
        hasException3 = true;
    } else {
        int found3 = -1;
        for (int i3 = 0; i3 < cri->mMsgSize; i3++) {
            if (cri->mMsgArray[(cri->mMsgFront + i3) % cri->mMsgCapacity].command ==
                CWorkThread::EVT_EXCEPTION) {
                found3 = i3;
                break;
            }
        }
        hasException3 = found3 >= 0;
    }
    int ok3 = 0;
    if (!hasException3) {
        if (cri->mState == CWorkThread::THREAD_STATE_LOGIN ||
            cri->mState == CWorkThread::THREAD_STATE_RUN) {
            ok3 = 1;
        }
    }
    return ok3;
}

extern "C" u8 func_8044E768__11CDeviceFileFv() { return lbl_eu_806636A8[0]; }

int CDeviceFile::func_8044E770(CWorkThread* parent) {
    // spInstance (lbl_eu_80665660)->field_1C8 = parent; return true
    *(void**)((char*)(u32)lbl_eu_80665660 + 0x1C8) = parent;
    return 1;
}

bool CDeviceFile::func_8044E780() {
    // spInstance (lbl_eu_80665660)->field_0x1C8 = 0; return true
    ((CDeviceFile*)(u32)lbl_eu_80665660)->field_0x1C8 = 0;
    return true;
}

void CDeviceFile::readFile() {}

void CDeviceFile::readCommonArchiveFile() {}

void CDeviceFile::getFileSize() {}

bool CDeviceFile::removeFileJob(CDeviceFileJob* job) {
    bool result;
    if (job->mHandle != NULL) {
        CDeviceFile* inst = (CDeviceFile*)lbl_eu_80665660;
        CFileHandleListNode* head = (CFileHandleListNode*)inst->mFileList.mStartNodePtr;
        CFileHandleListNode* node = head->mNext;
        while (node != head) {
            CFileHandleListNode* next = node->mNext;
            if (node->mItem == job->mHandle) {
                CFileHandleListNode* prev = node->mPrev;
                prev->mNext = next;
                next->mPrev = prev;
                node->mNext = NULL;
            }
            node = next;
        }
        if (job->mHandle != NULL) {
            delete job->mHandle;
        }
        result = true;
    } else {
        result = false;
    }
    job->mHandle = NULL;
    job->wkSetEvent(CWorkThread::EVT_NONE);
    return result;
}

void CDeviceFile::func_8044F0E4(const char* pPath) {
    CDeviceFileCri::func_8044FB08();
    CDeviceFileDvd::isRequestFile(pPath);
}

void CDeviceFile::cancel(CFileHandle* pFileHandle) {
    if (pFileHandle != NULL) {
        CDeviceFileCri::cancel(pFileHandle);
        CDeviceFileDvd::cancel(pFileHandle);
    }
}

bool CDeviceFile::func_8044F154(CFileHandle* pFileHandle, int param) {
    if (CDeviceFileDvd::getInstance()->func_8044F1B8(pFileHandle, param)) {
        return true;
    }
    return CDeviceFileCri::getInstance()->func_8044F1B8(pFileHandle, param);
}

bool CDeviceFile::func_8044F1B8(CFileHandle* pFileHandle, int param) {
    // Placeholder body: the retail function is a 0x248B request-state check
    // that walks the child list at 0x60 and inspects each item. Kept
    // deliberately large so -inline auto leaves it out-of-line: the calls
    // from func_8044F154 must stay direct bls.
    if (this == NULL) {
        return false;
    }
    if (pFileHandle == NULL) {
        return false;
    }
    if (mChildList == NULL) {
        return false;
    }
    bool found = false;
    CWorkThreadListNode* node = mChildList->mNext;
    while (node != mChildList) {
        CWorkThread* item = node->mItem;
        if (item == NULL) {
            node = node->mNext;
            continue;
        }
        if (item == (CWorkThread*)pFileHandle) {
            found = true;
            break;
        }
        node = node->mNext;
    }
    if (!found) {
        return false;
    }
    if (param > 0) {
        return true;
    }
    return false;
}

void CDeviceFile::func_8044F400(CFileHandle* pHandle, unsigned long param) {
    pHandle->func_80451984(param);
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
            __ct__CDeviceFileDvd(dvd, dvdName, this);
        }
        CWorkUtil::entryWork(dvd, this, false);

        const char* criName = lbl_eu_80522BE4 + 0x9A;
        CWorkThread* cri = (CWorkThread*)mtl::MemManager::allocate(0x1F0, CWorkThreadSystem::getWorkMem());
        if (cri != NULL) {
            __ct__CDeviceFileCri(cri, criName, this);
        }
        CWorkUtil::entryWork(cri, this, false);

        lbl_eu_806636A8[1] = 1;
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
    u8 langOverride = lbl_eu_806636A8[2];
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
    lbl_eu_806636A8[2] = val;
}

extern "C" void func_eu_804521B0() {
    lbl_eu_806636A8[2] = -1;
}

extern "C" void func_eu_804521BC(u8 val) {
    lbl_eu_806636A8[1] = val;
}

extern "C" u8 func_eu_804521C4() {
    return lbl_eu_806636A8[1];
}

CEventFile::CEventFile(CBM cbm, CFileHandle* handle) {
    field_0x0 = cbm;
    field_0x4 = handle;
    field_0x8 = handle->field_0x14;
    field_0xC = handle->mName;
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
