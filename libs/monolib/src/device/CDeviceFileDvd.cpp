#include "libs/monolib/src/device/CDeviceFileDvd.hpp"

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

// Retail vtable (rodata) - referenced manually so the ctor's vptr-store relocs
// are byte-identical to retail (the class is non-virtual; MWCC emits no vtable).
extern "C" u32 lbl_eu_8056C420[46];  // defined below (dissolved monolibdata2)
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
class IWorkEvent {
public:
    virtual ~IWorkEvent();
    virtual void s00();
    virtual bool onFileEvent(CEventFile* pEvent);
};

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
    s32 field_0x10;          //0x10
    u32 field_0x14;          //0x14 (rsrc id)
    u8 field_0x18[0x28 - 0x18];
    IWorkEvent* field_0x28;  //0x28 (event receiver)
    u8 field_0x2C[0x30 - 0x2C];
    s32* field_0x30;         //0x30 (out: data ptr)
    s32* field_0x34;         //0x34 (out: length)
    u8 field_0x38[0x3C - 0x38];
    u32 field_0x3C;          //0x3C (file length)
    u8 field_0x40[0x58 - 0x40];
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
    void destroy() const;
    void func_80451984(unsigned long);
    int func_80451CBC(int);
};

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
        return field_0x28->onFileEvent(&event);
    }
    return false;
}

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

void* CFileHandle::getRsrc() const { return CRsrc::getRsrc(field_0x14); }

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056C420-0x8056C4D8 (0xB8): CDeviceFileDvd vtable (retail bytes).
// The member functions defined in this TU emit the exact retail mangled
// names; the IWorkEvent/CWorkThread slots are cross-TU externs.
extern "C" void __dt__14CDeviceFileDvdFv();
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
extern "C" void wkUpdate__14CDeviceFileDvdFv();
extern "C" void wkRender__11CWorkThreadFv();
extern "C" void wkRenderAfter__11CWorkThreadFv();
extern "C" void wkStandbyLogin__14CDeviceFileDvdFv();
extern "C" void wkStandbyLogout__14CDeviceFileDvdFv();
extern "C" void wkStandbyExceptionRetry__14CDeviceFileDvdFUl();
extern "C" u32 __RTTI__10IWorkEvent;
extern "C" u32 __RTTI__11CWorkThread;
extern "C" u32 lbl_eu_806636E0[2];  // .sdata RTTI locator (defined below)
extern "C" const u32 lbl_eu_80522CA8[38];  // .rodata name string (defined below)
extern "C" u32 lbl_eu_8056C4C0;  // .data tail object (CDeviceFileDvd sibling TU)

extern "C" u32 lbl_eu_8056C420[46] = {
    (u32)&lbl_eu_806636E0, 0x00000000, (u32)&__dt__14CDeviceFileDvdFv, (u32)&WorkEvent1__10IWorkEventFPvPCc, (u32)&OnFileEvent__10IWorkEventFP10CEventFile, (u32)&WorkEvent3__10IWorkEventFPv, (u32)&WorkEvent4__10IWorkEventFv, (u32)&OnPauseTrigger__10IWorkEventFb,
    (u32)&WorkEvent6__10IWorkEventFv, (u32)&WorkEvent7__10IWorkEventFv, (u32)&WorkEvent8__10IWorkEventFv, (u32)&WorkEvent9__10IWorkEventFv, (u32)&WorkEvent10__10IWorkEventFv, (u32)&WorkEvent11__10IWorkEventFv, (u32)&WorkEvent12__10IWorkEventFv, (u32)&WorkEvent13__10IWorkEventFv,
    (u32)&WorkEvent14__10IWorkEventFv, (u32)&WorkEvent15__10IWorkEventFv, (u32)&WorkEvent16__10IWorkEventFv, (u32)&WorkEvent17__10IWorkEventFv, (u32)&WorkEvent18__10IWorkEventFv, (u32)&WorkEvent19__10IWorkEventFv, (u32)&WorkEvent20__10IWorkEventFv, (u32)&WorkEvent21__10IWorkEventFv,
    (u32)&WorkEvent22__10IWorkEventFv, (u32)&WorkEvent23__10IWorkEventFv, (u32)&WorkEvent24__10IWorkEventFv, (u32)&WorkEvent25__10IWorkEventFv, (u32)&WorkEvent26__10IWorkEventFv, (u32)&WorkEvent27__10IWorkEventFv, (u32)&WorkEvent28__10IWorkEventFv, (u32)&WorkEvent29__10IWorkEventFv,
    (u32)&WorkEvent30__10IWorkEventFv, (u32)&WorkEvent31__10IWorkEventFv, (u32)&wkUpdate__14CDeviceFileDvdFv, (u32)&wkRender__11CWorkThreadFv, (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__14CDeviceFileDvdFv, (u32)&wkStandbyLogout__14CDeviceFileDvdFv, (u32)&wkStandbyExceptionRetry__14CDeviceFileDvdFUl,
    (u32)&__RTTI__10IWorkEvent, 0x00000000, (u32)&__RTTI__11CWorkThread, 0x00000000, 0x00000000, 0x00000000,
};

// [.rodata] 0x80522CA8-0x80522D40 (0x98): "CDeviceFileDvd" + SJIS text (raw).
extern "C" const u32 lbl_eu_80522CA8[38] = {
    0x43446576, 0x69636546, 0x696C6544, 0x76640000, 0x83478389, 0x815B82AA, 0x94AD90B6, 0x82B582DC,
    0x82B582BD, 0x8142008B, 0x90905F82, 0xCC836683, 0x42835883, 0x4E82F083, 0x5A836283, 0x6782B582,
    0xC482AD82, 0xBE82B382, 0xA2814200, 0x8382815B, 0x835E815B, 0x82AA8E7E, 0x82DC82C1, 0x82C482A2,
    0x82DC82B7, 0x81420083, 0x66834283, 0x58834E82, 0xF093C782, 0xDF82DC82, 0xB982F182, 0xC582B582,
    0xBD81428F, 0xDA82B582, 0xAD82CD81, 0x45814581, 0x45000000, 0x00000000,
};

// [.sdata] 0x806636E0-0x806636E8 (8B): RTTI locator {name, vtable-tail}.
extern "C" u32 lbl_eu_806636E0[2] = { (u32)&lbl_eu_80522CA8, (u32)&lbl_eu_8056C4C0 };

// [.sbss] 0x80665670-0x80665678 (8B): singleton slot (4B used) + pad.
CDeviceFileDvd* lbl_eu_80665670;
u32 lbl_eu_80665670pad;
DECOMP_FORCEACTIVE(CDeviceFileDvd_cpp, lbl_eu_80665670pad);
