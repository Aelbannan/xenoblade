#include "libs/monolib/src/device/CDeviceFileDvd.hpp"

#include <revolution/DVD.h>

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

// Retail vtable (rodata) - referenced manually so the ctor's vptr-store relocs
// are byte-identical to retail (the class is non-virtual; MWCC emits no vtable).
extern u32 lbl_eu_8056C420[];
// Retail singleton pointer (sda21 .sbss).
extern CDeviceFileDvd* lbl_eu_80665670;

CDeviceFileDvd::CDeviceFileDvd(const char* pName, CWorkThread* pParent)
    : CWorkThread(pName, pParent, 0x100) {
    *(u32**)this = (u32*)lbl_eu_8056C420;
    field_0x1C4 = 0;
    field_0x1C8 = -1;
    field_0x1CC = 0;
    field_0x1D0 = 0;
    field_0x1D4 = 0;
    lbl_eu_80665670 = this;
}

CDeviceFileDvd::~CDeviceFileDvd() {
    lbl_eu_80665670 = nullptr;
}

CDeviceFileDvd* CDeviceFileDvd::getInstance() { return lbl_eu_80665670; }

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
    u8* node = *(u8**)(*(u32*)((u8*)lbl_eu_80665670 + 0x60));
    while (node != (u8*)*(u32*)((u8*)lbl_eu_80665670 + 0x60)) {
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

// Static (retail pHandle in r3) chain walker mirroring isRequestFile: call the
// job's cancel(handle) virtual (vtable[42]) for each job in the still-opening
// state. The handle is passed through (r4 = r30).
void CDeviceFileDvd::cancel(CFileHandle* pHandle) {
    u8* node = *(u8**)(*(u32*)((u8*)lbl_eu_80665670 + 0x60));
    while (node != (u8*)*(u32*)((u8*)lbl_eu_80665670 + 0x60)) {
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
    u8* job = *(u8**)(*(u32*)(*(u32*)((u8*)lbl_eu_80665670 + 0x60)) + 8);
    if (job == 0) {
        job = 0;
    } else if (*(s32*)(job + 0x50) != 0x44) {
        job = 0;
    }
    if (*(u8*)((u8*)lbl_eu_80665670 + 0x1D0) != 0) {
        DVDClose(reinterpret_cast<DVDFileInfo*>(job + 0x214));
    }
    CDeviceFile::removeFileJob(reinterpret_cast<CDeviceFileJob*>(job));
    *(u32*)((u8*)lbl_eu_80665670 + 0x1C4) = 4;
}

void CDeviceFileDvd::transState0() {}

void CDeviceFileDvd::transState3() {}

void CDeviceFileDvd::wkUpdate() {}

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

int CDeviceFileDvd::wkStandbyExceptionRetry(unsigned long param) {
    if (*(s32*)((u8*)this + 0x1C4) == 3) {
        u8* p = *(u8**)((u8*)this + 0x60);
        p = *(u8**)((u8*)p + 0x0);
        p = *(u8**)((u8*)p + 0x8);
        if (p == 0) {
            p = 0;
        } else if (*(s32*)((u8*)p + 0x50) != 0x44) {
            p = 0;
        }
        *(u32*)(*(u32*)((u8*)p + 0x1C4) + 0x10) = 0;
        *(u32*)((u8*)this + 0x1C4) = 4;
    }
    return 1;
}

struct CFileHandle {
    ~CFileHandle();
    void call() const;
    void checkExistRsrc() const;
    void destroy() const;
    void func_80451984(unsigned long);
    int func_80451CBC(int);
    void getRsrc() const;
    void init(int);
    void setup1() const;
    void setup2() const;
};

void CFileHandle::setup1() const {}

void CFileHandle::setup2() const {}

CFileHandle::~CFileHandle() {}

void CFileHandle::init(int param) {
    u8* s = (u8*)this;
    *(u32*)(s + 0x00) = (u32)param;
    *(u32*)(s + 0x3C) = 0;
    *(u32*)(s + 0x04) = 0;
    *(u32*)(s + 0x08) = 0;
    *(u32*)(s + 0x10) = 0;
    *(u32*)(s + 0x14) = -1;
    *(u32*)(s + 0x18) = -1;
    *(u32*)(s + 0x1C) = -1;
    *(u32*)(s + 0x20) = -1;
    *(u32*)(s + 0x24) = -1;
    *(u32*)(s + 0x28) = 0;
    *(u32*)(s + 0x0C) = 0;
    *(u32*)(s + 0x2C) = 0;
    *(u32*)(s + 0x30) = 0;
    *(u32*)(s + 0x34) = 0;
    *(u32*)(s + 0x38) = 0;
    *(u32*)(s + 0x40) = 0;
    *(u32*)(s + 0x44) = 0;
    *(u32*)(s + 0x4C) = 0;
    *(u32*)(s + 0x50) = 0;
    *(u32*)(s + 0x48) = 4;
    *(u32*)(s + 0x54) = 0;
    *(u32*)(s + 0x58) = 0;
}

void CFileHandle::func_80451984(unsigned long param) {
    u8* p = (u8*)this;
    for (int i = 0; i < 4; i++) {
        if (*(u32*)(p + 0x18) == 0xFFFFFFFF) {
            *(u32*)((u8*)this + i * 4 + 0x18) = (u32)param;
            return;
        }
        p += 4;
    }
}

void CFileHandle::destroy() const {}

int CFileHandle::func_80451CBC(int param) {
    *(s32*)((u8*)this + 0x10) += param;
    return 1;
}

void CFileHandle::call() const {}

void CFileHandle::checkExistRsrc() const {}

extern void getRsrc__5CRsrcFUl(unsigned long);
void CFileHandle::getRsrc() const { getRsrc__5CRsrcFUl((unsigned long)*(void**)((char*)this + 0x14)); }
