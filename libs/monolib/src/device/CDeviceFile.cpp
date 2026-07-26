// Auto-scaffolded catalog TU for monolib/src/device/CDeviceFile
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern "C" void __ct__11CDeviceFileFPCcP11CWorkThread(void* self, const char* name, void* parent) {}

extern "C" void __dt___reslist_base_CFileHandle(void* self, int type) {}

extern "C" void __dt__reslist_CFileHandle(void* self, int type) {}

extern "C" void __dt__11CDeviceFileFv(void* self, int type) {}

extern u32 lbl_eu_80665660;
extern "C" u32 getInstance__11CDeviceFileFv(void) { return lbl_eu_80665660; }

extern "C" int isInitialized__11CDeviceFileFv(void* self) { return 0; }

extern u8 lbl_eu_806636A8;
extern "C" u8 func_8044E768__11CDeviceFileFv() { return lbl_eu_806636A8; }

extern "C" void func_8044E770__11CDeviceFileFP11CWorkThread(void* self, void* parent) {}

extern "C" void func_8044E780__11CDeviceFileFv(void* self) {}

extern "C" void readFile__11CDeviceFileFUlPCcP10IWorkEventii(void* self) {}

extern "C" void readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii() {}

extern "C" void getFileSize__11CDeviceFileFPCc() {}

extern "C" void removeFileJob__11CDeviceFileFP14CDeviceFileJob() {}

extern "C" void func_8044F0E4__11CDeviceFileFPCc() {}

extern "C" void cancel__11CDeviceFileFP11CFileHandle() {}

extern "C" void func_8044F154__11CDeviceFileFP11CFileHandlei() {}

extern "C" void func_8044F1B8__11CDeviceFileFP11CFileHandlei() {}

extern "C" void func_8044F400__11CDeviceFileFP11CFileHandleUl(void) {}

extern "C" void setHandleFlag1__11CDeviceFileFP11CFileHandle() {}

extern "C" void setHandleFlag2__11CDeviceFileFP11CFileHandle() {}

extern "C" void wkStandbyLogin__11CDeviceFileFv() {}

extern "C" void wkStandbyLogout__11CDeviceFileFv() {}

extern "C" void func_eu_804520B0(void* r3) {
    extern int lbl_eu_80665664;
    extern void* lbl_eu_80657580[];
    int idx = lbl_eu_80665664;
    lbl_eu_80657580[idx] = r3;
    lbl_eu_80665664 = idx + 1;
}

extern "C" void func_eu_804520D0() {}

extern "C" u8 lbl_eu_806636AA[6];
extern "C" void func_eu_804521A8(s8 val) {
    lbl_eu_806636AA[0] = val;
}

extern "C" void func_eu_804521B0() {
    lbl_eu_806636AA[0] = -1;
}

extern "C" u8 lbl_eu_806636A9;
extern "C" void func_eu_804521BC(u8 val) {
    lbl_eu_806636A9 = val;
}

extern "C" u8 func_eu_804521C4() {
    return lbl_eu_806636A9;
}

extern "C" void __ct__10CEventFileF3CBMP11CFileHandle() {}

extern "C" void getFileDataPtr__10CEventFileFv() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_eu_80452248() {
    extern unsigned char lbl_eu_806575C0[];
    lbl_eu_806575C0[0] = 0;
    *(unsigned int*)(lbl_eu_806575C0 + 0x100) = 0;
}
