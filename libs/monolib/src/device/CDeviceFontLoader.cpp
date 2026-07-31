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

// Retail vtable (data lives in monolibdata2.s)
extern u8 lbl_eu_8056C8A8[];

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

void func_80454F30__17CDeviceFontLoaderFv(CDeviceFontLoader* self, void* arg1, const char* pPath) {
    self->mSomeData = arg1;
    self->mFileNameLen = strlen(pPath);
    strcpy(self->mFileName, pPath);
}

bool wkStandbyLogin__17CDeviceFontLoaderFv(CDeviceFontLoader* self) {
    if (!CWorkSystemPack::func_804DE08C()) {
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

}
