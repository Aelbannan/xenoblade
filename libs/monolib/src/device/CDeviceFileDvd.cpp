#include "libs/monolib/src/device/CDeviceFileDvd.hpp"

#include <revolution/DVD.h>

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

void CDeviceFileDvd::isRequestFile(const char* pPath) {}

void CDeviceFileDvd::cancel(CFileHandle* pHandle) {}

void CDeviceFileDvd::cancelCurrent() {}

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
