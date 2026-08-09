// Auto-scaffolded catalog TU for monolib/src/device/CDeviceFileDvd
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

struct CDeviceFileDvd {
    ~CDeviceFileDvd();
    void cancel() const;
    void cancelCurrent();
    void getFileSize();
    void isRequestFile();
    void transState0();
    void transState3();
    int wkStandbyExceptionRetry(unsigned long);
    void wkStandbyLogin();
    void wkStandbyLogout();
    void wkUpdate();
};

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


void __ct__CDeviceFileDvd(){}

CDeviceFileDvd::~CDeviceFileDvd() {}

extern "C" u32 getInstance__14CDeviceFileDvdFv(void) {
    extern u32 lbl_eu_80665670;
    return lbl_eu_80665670;
}

void CDeviceFileDvd::getFileSize() {}

void CDeviceFileDvd::isRequestFile() {}

void CDeviceFileDvd::cancel() const {}

void CDeviceFileDvd::cancelCurrent() {}

void CDeviceFileDvd::transState0() {}

void CDeviceFileDvd::transState3() {}

void CDeviceFileDvd::wkUpdate() {}

void CDeviceFileDvd::wkStandbyLogin() {}

void CDeviceFileDvd::wkStandbyLogout() {}

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
