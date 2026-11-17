// CDeviceFontLoader - Font file loading work thread

#include <string.h>
#include <types.h>

#include "monolib/work.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CDeviceFontLoader.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/util/MemManager.hpp"

extern "C" u32 lbl_eu_8056C8A8[40]; // vtable (defined below)
extern "C" u32 __RTTI__17CDeviceFontLoader[2];  // .sdata RTTI locator (defined below)
extern "C" u32 lbl_eu_8056C948[6];  // .data RTTI chain (defined below)

extern "C" {
// Base ctor called on raw storage by the unmangled __ct__CDeviceFontLoader.
void __ct__11CWorkThreadFPCcP11CWorkThreadi(CWorkThread* self, const char* name,
                                           CWorkThread* parent, int capacity);
// CDeviceFont::createFontInfo - Fv mangling, but the retail caller passes the
// loader's font arg in r3 and the just-claimed file data in r4.
void createFontInfo__11CDeviceFontFv(u32 arg, u8* pData);

CDeviceFontLoader* __ct__CDeviceFontLoader(CDeviceFontLoader* self, const char* pName,
                                           CWorkThread* pParent) {
    __ct__11CWorkThreadFPCcP11CWorkThreadi(self, pName, pParent, 0);
    *reinterpret_cast<u32**>(self) = lbl_eu_8056C8A8; // novtable: hand-laid vtable
    self->mFileName[0] = '\0';
    self->mFileNameLen = 0;
    self->mFileHandle = nullptr;
    self->mType = CWorkThread::THREAD_CDEVICEFONTLOADER;
    return self;
}
} // extern "C"

CDeviceFontLoader::~CDeviceFontLoader() {}

void setFontPath__17CDeviceFontLoaderFv(CDeviceFontLoader* self, u32 arg, const char* pPath) {
    self->mFontArg = arg;
    self->mFileNameLen = strlen(pPath);
    strcpy(self->mFileName, pPath);
}

bool CDeviceFontLoader::wkStandbyLogin() {
    if (!CWorkSystemPack::arePacksLoaded()) {
        return false;
    }

    if (CDeviceFile::getInstance() != nullptr && CDeviceFile::isInitialized()) {
        mFileHandle = CDeviceFile::readFile(mtl::MemManager::getHandleMEM1(), mFileName, this, 0, 0);
        CDeviceFile::tryUpdateJobPriority(mFileHandle, 0);
        CDeviceFile::setHandleFlag1(mFileHandle);
        return CWorkThread::wkStandbyLogin();
    }

    return false;
}

bool CDeviceFontLoader::wkStandbyLogout() {
    if (mFileHandle != nullptr) {
        CDeviceFile::cancel(mFileHandle);
        mFileHandle = nullptr;
    }

    if (mChildren.empty()) {
        return CWorkThread::wkStandbyLogout();
    }

    return false;
}

bool CDeviceFontLoader::OnFileEvent(CEventFile* pEventFile) {
    if (pEventFile->mFileHandle == mFileHandle) {
        if (pEventFile->unk0 == 1) {
            u8* pData = mFileHandle->mData;
            mFileHandle->mData = nullptr;
            createFontInfo__11CDeviceFontFv(mFontArg, pData);
            wkSetEvent(EVT_NONE);
        }

        mFileHandle = nullptr;
    }

    return true;
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// Load-bearing: the slot targets are declared inside a namespace. This TU builds
// with -RTTI on and defines a polymorphic class, so global-scope extern "C"
// __RTTI__ declarations (e.g. via data_vtables.hpp) clash with MWCC's own RTTI
// objects (error 10322 "illegal name overloading").
namespace FontLoaderBlob {
// [.data] 0x8056C8A8-0x8056C960 (184B): CDeviceFontLoader vtable (160B) +
// RTTI chain (24B).  The ctor installs lbl_eu_8056C8A8 directly and every
// vtable slot is a flat extern "C" symbol (see functions above), so MWCC
// never auto-emits __vt__ here.
extern "C" void WorkEvent1__10IWorkEventFPvPCc();
extern "C" void OnFileEvent__17CDeviceFontLoaderFP10CEventFile();
extern "C" void __dt__17CDeviceFontLoaderFv();
extern "C" void wkStandbyLogin__17CDeviceFontLoaderFv();
extern "C" void wkStandbyLogout__17CDeviceFontLoaderFv();
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
    (u32)&__RTTI__17CDeviceFontLoader, 0x00000000,
    (u32)&FontLoaderBlob::__dt__17CDeviceFontLoaderFv,
    (u32)&FontLoaderBlob::WorkEvent1__10IWorkEventFPvPCc,
    (u32)&FontLoaderBlob::OnFileEvent__17CDeviceFontLoaderFP10CEventFile,
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
    (u32)&FontLoaderBlob::wkStandbyLogin__17CDeviceFontLoaderFv,
    (u32)&FontLoaderBlob::wkStandbyLogout__17CDeviceFontLoaderFv,
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
extern "C" u32 __RTTI__17CDeviceFontLoader[2] = {
    (u32)&lbl_eu_80522EB8, (u32)&lbl_eu_8056C948,
};
