// Auto-scaffolded catalog TU for monolib/src/device/CDeviceFile
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern u32 lbl_eu_80665660; // CDeviceFile singleton pointer (sdata2)

// Retail data imports (rodata / sbss) referenced by the functions below.
extern u32 lbl_eu_8056C324[];  // reslist<CFileHandle> vtable
extern char lbl_eu_80522BE4[]; // device-name string table
extern u8 lbl_eu_806636A9;     // work-system standby flag (sbss)

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

class CWorkThread {
public:
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

    bool wkStandbyLogin();
    bool wkStandbyLogout();
    void wkSetEvent(EVT evt);

    //0x0..0x1C4: CWorkThread base (name/state/id/type/alloc/parent)
    u8 field_0x0[0x5C];              //0x0
    u32 field_0x5C;                  //0x5C (children reslist vtable)
    CWorkThreadListNode* mChildList; //0x60 (children reslist mStartNodePtr)
    u8 field_0x64[0x1C4 - 0x64];     //0x64..0x1C4
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

class CDeviceFile : public CWorkThread {
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

    //0x1C4..0x1C8: CDeviceBase tail
    u8 field_0x1C4[4];             //0x1C4
    CWorkThread* field_0x1C8;      //0x1C8 (standby state field)
    u8 field_0x1CC[0x1D0 - 0x1CC];
    CFileHandleListNode* mJobList; //0x1D0 (reslist<CFileHandle*> mStartNodePtr)
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


CDeviceFile::CDeviceFile(const char* name, void* parent) {}

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

CDeviceFile::~CDeviceFile() {}

u32 getInstance__11CDeviceFileFv(void) { return lbl_eu_80665660; }
int CDeviceFile::isInitialized() { return 0; }

extern u8 lbl_eu_806636A8;
extern "C" u8 func_8044E768__11CDeviceFileFv() { return lbl_eu_806636A8; }

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
        CFileHandleListNode* head = inst->mJobList;
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
    extern int lbl_eu_80665664;
    extern void* lbl_eu_80657580[];
    int idx = lbl_eu_80665664;
    lbl_eu_80657580[idx] = r3;
    lbl_eu_80665664 = idx + 1;
}

void func_eu_804520D0(){}

// These two retail data symbols are also declared in the "C-linkage imports"
// section of monolib/device/CDeviceFile.hpp. They stay declared here too
// because this catalog TU defines a local `struct CDeviceFile` that conflicts
// with the class in that header, so this TU cannot include it.
extern "C" u8 lbl_eu_806636AA[6];
extern "C" void func_eu_804521A8(s8 val) {
    lbl_eu_806636AA[0] = val;
}

extern "C" void func_eu_804521B0() {
    lbl_eu_806636AA[0] = -1;
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
