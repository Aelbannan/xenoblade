// Auto-scaffolded catalog TU for monolib/src/device/CDeviceFileDvd
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void __ct__CDeviceFileDvd(){}

void CDeviceFileDvd::~CDeviceFileDvd() {}

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

void CDeviceFileDvd::wkStandbyExceptionRetry() {}

void CFileHandle::setup1() const {}

void CFileHandle::setup2() const {}

void CFileHandle::~CFileHandle() const {}

void CFileHandle::init() const {}

void CFileHandle::func_80451984() const {}

void CFileHandle::destroy() const {}

void CFileHandle::func_80451CBC() const {}

void CFileHandle::call() const {}

void CFileHandle::checkExistRsrc() const {}

extern void getRsrc__5CRsrcFUl(unsigned long);
void CFileHandle::getRsrc() const { getRsrc__5CRsrcFUl((unsigned long)*(void**)((char*)this + 0x14)); }
