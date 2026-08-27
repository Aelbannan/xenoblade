// CDeviceFontLoader - Font file loading work thread
//
// Implemented as a plain struct + extern "C" functions to match the retail
// binary's symbol names exactly:
//   - MWCC 1.1 mangles the ctor as __ct__17CDeviceFontLoaderFPCcP11CWorkThread
//     (retail: __ct__CDeviceFontLoader), and
//   - inheriting from CWorkThread makes MWCC emit a local vtable plus weak
//     stubs for inherited virtuals (wkRender/wkRenderAfter/...), which blows
//     the .text split budget by 0x10.
// The retail vtable (lbl_eu_8056C8A8, defined in monolibdata2.s) is used
// directly by the ctor instead.

#include <string.h>
#include <types.h>

#include "monolib/work.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/util/MemManager.hpp"

// Size: 0x210
struct CDeviceFontLoader {
    // 0x000-0x1C3: CWorkThread member layout (public in CWorkThread.hpp)
    void* vtable;                    // 0x00
    ml::FixStr<64> mName;            // 0x04
    CWorkThread::ThreadState mState; // 0x48
    WORK_ID mWorkID;                 // 0x4C
    CWorkThread::ThreadType mType;   // 0x50
    mtl::ALLOC_HANDLE mAllocHandle;  // 0x54
    CWorkThread* mParent;            // 0x58
    reslist<CWorkThread*> mChildren; // 0x5C
    u32 mFlags;                      // 0x7C
    CMsgParam<8> mMsgQueue;          // 0x80
    u32 unk1BC;                      // 0x1BC
    WORK_ID mExceptionWorkID;        // 0x1C0

    // 0x1C4-0x20F: CDeviceFontLoader members
    void* mSomeData;          // 0x1C4
    char mFileName[64];       // 0x1C8
    u32 mFileNameLen;         // 0x208
    CFileHandle* mFileHandle; // 0x20C
};

// extern "C" to produce retail-exact symbol names
extern "C" u32 lbl_eu_8056C8A8[40]; // vtable (defined below)
extern "C" u32 lbl_eu_80663748[2];  // .sdata RTTI locator (defined below)
extern "C" u32 lbl_eu_8056C948[6];  // .data RTTI chain (defined below)
extern "C" {

// CWorkThread / runtime helpers referenced by the retail symbol names
void __ct__11CWorkThreadFPCcP11CWorkThreadi(void* self, const char* name, void* parent, int capacity);
void __dt__11CWorkThreadFv(void* self, int dealloc);
void __dl__FPv(void* p);
bool wkStandbyLogin__11CWorkThreadFv(void* self);
bool wkStandbyLogout__11CWorkThreadFv(void* self);
void wkSetEvent__11CWorkThreadFQ211CWorkThread3EVT(void* self, int evt);
// CDeviceFont::func_80452D80 - Fv mangling, but retail caller passes a
// second (ignored) argument in r4 (the just-claimed file data pointer)
void func_80452D80__11CDeviceFontFv(void* self, void* arg2);







void* __ct__CDeviceFontLoader(CDeviceFontLoader* self, const char* name, CWorkThread* parent) {
    __ct__11CWorkThreadFPCcP11CWorkThreadi(self, name, parent, 0);
    *(void**)self = (void*)lbl_eu_8056C8A8;
    self->mFileName[0] = '\0';
    self->mFileNameLen = 0;
    self->mFileHandle = nullptr;
    self->mType = CWorkThread::THREAD_CDEVICEFONTLOADER;
    return self;
}

void* __dt__17CDeviceFontLoaderFv(CDeviceFontLoader* self, int dealloc) {
    if (self != nullptr) {
        __dt__11CWorkThreadFv(self, 0);
        if (dealloc > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void setFontPath__17CDeviceFontLoaderFv(CDeviceFontLoader* self, void* arg1, const char* pPath) {
    self->mSomeData = arg1;
    self->mFileNameLen = strlen(pPath);
    strcpy(self->mFileName, pPath);
}

bool wkStandbyLogin__17CDeviceFontLoaderFv(CDeviceFontLoader* self) {
    if (!CWorkSystemPack::arePacksLoaded()) {
        return false;
    }

    if (CDeviceFile::getInstance() == nullptr) {
        goto fail;
    }

    if (!CDeviceFile::isInitialized()) {
        goto fail;
    }

    self->mFileHandle = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM1(),
        self->mFileName, (IWorkEvent*)self, 0, 0);
    CDeviceFile::func_8044F154(self->mFileHandle, 0);
    CDeviceFile::setHandleFlag1(self->mFileHandle);
    return wkStandbyLogin__11CWorkThreadFv(self);

fail:
    return false;
}

bool wkStandbyLogout__17CDeviceFontLoaderFv(CDeviceFontLoader* self) {
    if (self->mFileHandle != nullptr) {
        CDeviceFile::cancel(self->mFileHandle);
        self->mFileHandle = nullptr;
    }

    if (self->mChildren.empty()) {
        return wkStandbyLogout__11CWorkThreadFv(self);
    }

    return false;
}

bool OnFileEvent__17CDeviceFontLoaderFP10CEventFile(CDeviceFontLoader* self, CEventFile* pEventFile) {
    if (pEventFile->mFileHandle != self->mFileHandle) {
        goto skip;
    }

    if (pEventFile->unk0 == 1) {
        void* pData = self->mFileHandle->mData;
        self->mFileHandle->mData = nullptr;
        func_80452D80__11CDeviceFontFv((void*)self->mSomeData, pData);
        wkSetEvent__11CWorkThreadFQ211CWorkThread3EVT(self, CWorkThread::EVT_NONE);
    }

    self->mFileHandle = nullptr;

skip:
    return true;
}

} // extern "C"


// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
namespace FontLoaderBlob {
// [.data] 0x8056C8A8-0x8056C960 (184B): CDeviceFontLoader vtable (160B) +
// RTTI chain (24B).  The ctor installs lbl_eu_8056C8A8 directly and every
// vtable slot is a flat extern "C" symbol (see functions above), so MWCC
// never auto-emits __vt__ here.
extern "C" void WorkEvent1__10IWorkEventFPvPCc();
extern "C" void OnFileEvent__17CDeviceFontLoaderFP10CEventFile();
extern "C" u32 __RTTI__10IWorkEvent;
extern "C" u32 __RTTI__11CWorkThread;
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
}

extern "C" u32 lbl_eu_8056C8A8[40] = {
    (u32)&lbl_eu_80663748, 0x00000000,
    (u32)&__dt__17CDeviceFontLoaderFv,
    (u32)&FontLoaderBlob::WorkEvent1__10IWorkEventFPvPCc,
    (u32)&OnFileEvent__17CDeviceFontLoaderFP10CEventFile,
    (u32)&FontLoaderBlob::WorkEvent3__10IWorkEventFPv,
    (u32)&FontLoaderBlob::WorkEvent4__10IWorkEventFv,
    (u32)&FontLoaderBlob::OnPauseTrigger__10IWorkEventFb,
    (u32)&FontLoaderBlob::WorkEvent6__10IWorkEventFv,
    (u32)&FontLoaderBlob::WorkEvent7__10IWorkEventFv,
    (u32)&FontLoaderBlob::WorkEvent8__10IWorkEventFv,
    (u32)&FontLoaderBlob::WorkEvent9__10IWorkEventFv,
    (u32)&FontLoaderBlob::WorkEvent10__10IWorkEventFv,
    (u32)&FontLoaderBlob::WorkEvent11__10IWorkEventFv,
    (u32)&FontLoaderBlob::WorkEvent12__10IWorkEventFv,
    (u32)&FontLoaderBlob::WorkEvent13__10IWorkEventFv,
    (u32)&FontLoaderBlob::WorkEvent14__10IWorkEventFv,
    (u32)&FontLoaderBlob::WorkEvent15__10IWorkEventFv,
    (u32)&FontLoaderBlob::WorkEvent16__10IWorkEventFv,
    (u32)&FontLoaderBlob::WorkEvent17__10IWorkEventFv,
    (u32)&FontLoaderBlob::WorkEvent18__10IWorkEventFv,
    (u32)&FontLoaderBlob::WorkEvent19__10IWorkEventFv,
    (u32)&FontLoaderBlob::WorkEvent20__10IWorkEventFv,
    (u32)&FontLoaderBlob::WorkEvent21__10IWorkEventFv,
    (u32)&FontLoaderBlob::WorkEvent22__10IWorkEventFv,
    (u32)&FontLoaderBlob::WorkEvent23__10IWorkEventFv,
    (u32)&FontLoaderBlob::WorkEvent24__10IWorkEventFv,
    (u32)&FontLoaderBlob::WorkEvent25__10IWorkEventFv,
    (u32)&FontLoaderBlob::WorkEvent26__10IWorkEventFv,
    (u32)&FontLoaderBlob::WorkEvent27__10IWorkEventFv,
    (u32)&FontLoaderBlob::WorkEvent28__10IWorkEventFv,
    (u32)&FontLoaderBlob::WorkEvent29__10IWorkEventFv,
    (u32)&FontLoaderBlob::WorkEvent30__10IWorkEventFv,
    (u32)&FontLoaderBlob::WorkEvent31__10IWorkEventFv,
    (u32)&FontLoaderBlob::wkUpdate__11CWorkThreadFv,
    (u32)&FontLoaderBlob::wkRender__11CWorkThreadFv,
    (u32)&FontLoaderBlob::wkRenderAfter__11CWorkThreadFv,
    (u32)&wkStandbyLogin__17CDeviceFontLoaderFv,
    (u32)&wkStandbyLogout__17CDeviceFontLoaderFv,
    (u32)&FontLoaderBlob::wkStandbyExceptionRetry__11CWorkThreadFUl,
};
extern "C" u32 lbl_eu_8056C948[6] = {
    (u32)&FontLoaderBlob::__RTTI__10IWorkEvent, 0x00000000,
    (u32)&FontLoaderBlob::__RTTI__11CWorkThread, 0x00000000, 0x00000000, 0x00000000,
};

// [.rodata] 0x80522EB8-0x80522ED0 (24B): "CDeviceFontLoader"
extern "C" const u32 lbl_eu_80522EB8[6] = {
    0x43446576, 0x69636546, 0x6F6E744C, 0x6F616465, 0x72000000, 0x00000000,
};

// [.sdata] 0x80663748-0x80663750 (8B): RTTI locator { name, RTTI chain }
extern "C" u32 lbl_eu_80663748[2] = {
    (u32)&lbl_eu_80522EB8, (u32)&lbl_eu_8056C948,
};
