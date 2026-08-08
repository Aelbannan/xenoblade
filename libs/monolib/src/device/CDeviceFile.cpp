// Auto-scaffolded catalog TU for monolib/src/device/CDeviceFile
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// Forward declarations: only names are needed for MWCC mangling; bodies use
// raw offsets via the retail labels (catalog TU style).
class CWorkThread;
class CFileHandle;

struct CDeviceFile {
    CDeviceFile(const char* name, void* parent);
    ~CDeviceFile();
    void cancel() const;
    static int func_8044E770(CWorkThread* parent);
    void func_8044E780();
    void func_8044F0E4();
    void func_8044F154() const;
    void func_8044F1B8() const;
    void func_8044F400() const;
    void getFileSize();
    int isInitialized();
    void readCommonArchiveFile();
    void readFile();
    void removeFileJob();
    static void setHandleFlag1(CFileHandle* pFileHandle);
    static void setHandleFlag2(CFileHandle* pFileHandle);
    void wkStandbyLogin();
    void wkStandbyLogout();
};

struct CEventFile {
    CEventFile();
    void* getFileDataPtr();
};


CDeviceFile::CDeviceFile(const char* name, void* parent) {}

void __dt___reslist_base_CFileHandle(void* self, int type){}

void __dt__reslist_CFileHandle(void* self, int type){}

CDeviceFile::~CDeviceFile() {}

extern u32 lbl_eu_80665660;
u32 getInstance__11CDeviceFileFv(void) { return lbl_eu_80665660; }

int CDeviceFile::isInitialized() { return 0; }

extern u8 lbl_eu_806636A8;
extern "C" u8 func_8044E768__11CDeviceFileFv() { return lbl_eu_806636A8; }

int CDeviceFile::func_8044E770(CWorkThread* parent) {
    // spInstance (lbl_eu_80665660)->field_1C8 = parent; return true
    *(void**)((char*)(u32)lbl_eu_80665660 + 0x1C8) = parent;
    return 1;
}

void CDeviceFile::func_8044E780() {}

void CDeviceFile::readFile() {}

void CDeviceFile::readCommonArchiveFile() {}

void CDeviceFile::getFileSize() {}

void CDeviceFile::removeFileJob() {}

void CDeviceFile::func_8044F0E4() {}

void CDeviceFile::cancel() const {}

void CDeviceFile::func_8044F154() const {}

void CDeviceFile::func_8044F1B8() const {}

void CDeviceFile::func_8044F400(void) const {}

void CDeviceFile::setHandleFlag1(CFileHandle* pFileHandle) {
    *(u32*)((char*)pFileHandle + 0x58) |= 2;
}

void CDeviceFile::setHandleFlag2(CFileHandle* pFileHandle) {
    *(u32*)((char*)pFileHandle + 0x58) |= 4;
}

void CDeviceFile::wkStandbyLogin() {}

void CDeviceFile::wkStandbyLogout() {}

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

extern "C" u8 lbl_eu_806636A9;
extern "C" void func_eu_804521BC(u8 val) {
    lbl_eu_806636A9 = val;
}

extern "C" u8 func_eu_804521C4() {
    return lbl_eu_806636A9;
}

CEventFile::CEventFile() {}

void* CEventFile::getFileDataPtr() {
    u32* p = *(u32**)((char*)this + 4);
    void* r = (void*)p[1];
    p[1] = 0;
    return r;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_eu_80452248() {
    extern unsigned char lbl_eu_806575C0[];
    lbl_eu_806575C0[0] = 0;
    *(unsigned int*)(lbl_eu_806575C0 + 0x100) = 0;
}
