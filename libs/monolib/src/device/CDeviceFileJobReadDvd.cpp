// NOTE: deliberately NOT including monolib/device.hpp: it pulls in
// CDeviceFileJobReadDvd.hpp, whose member declarations would collide with the
// extern "C" definitions below (they carry the exact retail mangled names).
#include <types.h>
#include "monolib/work/CWorkThread.hpp"
#include "monolib/device/CDeviceFileJob.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CDeviceFileDvd.hpp"
#include "monolib/core/CRsrc.hpp"
#include "monolib/util.hpp"
#include <revolution/DVD.h>

// extern "C" forward decls
extern "C" u32 lbl_eu_806636F0[];
extern "C" u32 lbl_eu_8056C644[];
extern "C" u32 lbl_eu_806636E8[];
namespace M2Blob {
extern "C" void WorkEvent1__10IWorkEventFPvPCc(void);
extern "C" void OnFileEvent__10IWorkEventFP10CEventFile(void);
extern "C" void WorkEvent3__10IWorkEventFPv(void);
extern "C" void WorkEvent4__10IWorkEventFv(void);
extern "C" void OnPauseTrigger__10IWorkEventFb(void);
extern "C" void WorkEvent6__10IWorkEventFv(void);
extern "C" void WorkEvent7__10IWorkEventFv(void);
extern "C" void WorkEvent8__10IWorkEventFv(void);
extern "C" void WorkEvent9__10IWorkEventFv(void);
extern "C" void WorkEvent10__10IWorkEventFv(void);
extern "C" void WorkEvent11__10IWorkEventFv(void);
extern "C" void WorkEvent12__10IWorkEventFv(void);
extern "C" void WorkEvent13__10IWorkEventFv(void);
extern "C" void WorkEvent14__10IWorkEventFv(void);
extern "C" void WorkEvent15__10IWorkEventFv(void);
extern "C" void WorkEvent16__10IWorkEventFv(void);
extern "C" void WorkEvent17__10IWorkEventFv(void);
extern "C" void WorkEvent18__10IWorkEventFv(void);
extern "C" void WorkEvent19__10IWorkEventFv(void);
extern "C" void WorkEvent20__10IWorkEventFv(void);
extern "C" void WorkEvent21__10IWorkEventFv(void);
extern "C" void WorkEvent22__10IWorkEventFv(void);
extern "C" void WorkEvent23__10IWorkEventFv(void);
extern "C" void WorkEvent24__10IWorkEventFv(void);
extern "C" void WorkEvent25__10IWorkEventFv(void);
extern "C" void WorkEvent26__10IWorkEventFv(void);
extern "C" void WorkEvent27__10IWorkEventFv(void);
extern "C" void WorkEvent28__10IWorkEventFv(void);
extern "C" void WorkEvent29__10IWorkEventFv(void);
extern "C" void WorkEvent30__10IWorkEventFv(void);
extern "C" void WorkEvent31__10IWorkEventFv(void);
extern "C" void wkRender__11CWorkThreadFv(void);
extern "C" void wkRenderAfter__11CWorkThreadFv(void);
extern "C" void wkStandbyExceptionRetry__11CWorkThreadFUl(void);
extern "C" void CDeviceFileJob_UnkVirtualFunc1__14CDeviceFileJobFv(void);
extern "C" u32 __RTTI__10IWorkEvent;
extern "C" u32 __RTTI__11CWorkThread;
}

// Local vtable-positioned view of the read-DVD job used by wkStandbyLogout:
// retail dispatches cancel(CFileHandle*) at vtable[0xA8], but MWCC lays the
// real class's inherited chain one slot longer (its cancel lands at 0xAC), so
// the call goes through this dummy-shaped interface instead (same trick as
// CDeviceFileJobVtbl in CDeviceFileDvd.cpp). The dummies only position the
// slot; declaring cancel as a real virtual makes MWCC emit the r12 dispatch
// shape (lwz r12,0(r3); lwz r12,0xa8(r12); mtctr; bctrl).
class CDeviceFileJobReadDvdVtbl {
public:
    virtual ~CDeviceFileJobReadDvdVtbl();
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
    virtual bool cancel(CFileHandle* pHandle);
};

// ---------------------------------------------------------------------------
// Plain-layout view of the CDeviceFileJobReadDvd object (retail offsets):
//   0x000 vptr (set by the ctor from the shared data vtable lbl_eu_8056C598)
//   0x004 CWorkThread::mName (FixStr<64>), 0x048 mState, 0x04C mWorkID,
//   0x050 mType, ... 0x1BC unk1BC, 0x1C0 mExceptionWorkID
//   0x1C4 CFileHandle* mHandle, ... 0x214-0x2?? DVDFileInfo mDvdFileInfo
//
// The class is deliberately NOT the polymorphic header class: defining the
// members here as extern "C" free functions emits no local __vt__/RTTI and no
// weak vtable-support stubs / base-class dtors. The retail split is text-only
// and its budget is exact (the current build overflowed by 0xC0 = 4 weak/base
// helpers + a dead out-of-line cancelCurrent copy).
struct CDeviceFileJobReadDvdLayout {
    void* vtbl;                  // 0x0
    u8 mName[0x44];              // 0x4 (CWorkThread::mName FixStr<64>)
    u32 mState;                  // 0x48
    u32 mWorkID;                 // 0x4C
    u32 mType;                   // 0x50 (ThreadType)
    u8 pad54[0x1C4 - 0x54];      // 0x54..0x1C3
    CFileHandle* mHandle;        // 0x1C4
    u8 unk1C8;                   // 0x1C8
    u8 unk1C9[0x208 - 0x1C9];    // 0x1C9..0x207
    u32 unk208;                  // 0x208
    u32 unk20C;                  // 0x20C
    u8 unk210;                   // 0x210
    u8 pad211[0x214 - 0x211];    // 0x211..0x213
    DVDFileInfo mDvdFileInfo;    // 0x214
};

// Retail vtable (shared data split .data 0x8056C598), stored by the ctor.
extern "C" u32 lbl_eu_8056C598[43];

// Extern "C" calls whose retail names do NOT collide with header member
// declarations.  Base-class member calls (CWorkThread/CDeviceFile/CDeviceFileDvd/
// CRsrc) go through the real classes so the compiler emits the retail mangled
// symbols directly.
extern "C" void __ct__14CDeviceFileJobFPCcP11CWorkThread(void* self, const char* pName, CWorkThread* pParent);
extern "C" void __dt__11CWorkThreadFv(void* self, int flag);
extern "C" void __dl__FPv(void*);
// CDeviceFileCri.hpp pulls in CDeviceFileJobReadDvd.hpp (member collision), so
// this sibling helper is declared directly under its retail mangled name.
extern "C" void func_8044FC38__14CDeviceFileCriFv(void);

extern "C" CDeviceFileJobReadDvdLayout* __ct__21CDeviceFileJobReadDvdFPCcP11CWorkThread(
    CDeviceFileJobReadDvdLayout* self, const char* pName, CWorkThread* pParent) {
    __ct__14CDeviceFileJobFPCcP11CWorkThread(self, pName, pParent);
    self->vtbl = (void*)lbl_eu_8056C598;
    self->mType = CWorkThread::THREAD_CDEVICEFILEJOBREADDVD;
    // Why not initialize mDvdFileInfo??
    return self;
}

extern "C" CDeviceFileJobReadDvdLayout* __dt__21CDeviceFileJobReadDvdFv(
    CDeviceFileJobReadDvdLayout* self, int flag) {
    if (self != 0) {
        if (self != 0) {
            __dt__11CWorkThreadFv(self, 0);
        }
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

extern "C" bool cancel__21CDeviceFileJobReadDvdFPCc(CDeviceFileJobReadDvdLayout* self, const char* pFilename) {
    if (self->mHandle == nullptr) return false;
    // If the filename doesn't match the one in the handle, return early
    if (self->mHandle->mName != pFilename) return false;

    // Retail inlines cancelCurrent() here (no separate out-of-line copy).
    CWorkThread* wt = reinterpret_cast<CWorkThread*>(self);
    if (!wt->wkIsCurrent()) {
        CDeviceFile::removeFileJob(reinterpret_cast<CDeviceFileJob*>(self));
    } else if (CDeviceFile::func_8044E768()) {
        func_8044FC38__14CDeviceFileCriFv();
    } else {
        CDeviceFileDvd::cancelCurrent();
    }
    return true;
}

extern "C" bool cancel__21CDeviceFileJobReadDvdFP11CFileHandle(CDeviceFileJobReadDvdLayout* self, CFileHandle* pHandle) {
    if (self->mHandle == nullptr || self->mHandle != pHandle) return false;

    // Retail inlines cancelCurrent() here (no separate out-of-line copy).
    CWorkThread* wt = reinterpret_cast<CWorkThread*>(self);
    if (!wt->wkIsCurrent()) {
        CDeviceFile::removeFileJob(reinterpret_cast<CDeviceFileJob*>(self));
    } else if (CDeviceFile::func_8044E768()) {
        func_8044FC38__14CDeviceFileCriFv();
    } else {
        CDeviceFileDvd::cancelCurrent();
    }
    return true;
}

extern "C" void wkUpdate__21CDeviceFileJobReadDvdFv(CDeviceFileJobReadDvdLayout* self) {
    if (self->mHandle != nullptr && self->mHandle->unkInline2()) {
        if (self->mHandle->getRsrc() != nullptr) {
            CRsrc::entry(self->mHandle->getRsrc(), self->mHandle->mName.c_str(), &self->mHandle->unk160,
            self->mHandle->getData(), self->mHandle->getLength(), false);
        }

        self->mHandle->call(CBM_1);
        CDeviceFile::removeFileJob(reinterpret_cast<CDeviceFileJob*>(self));
    }
}

extern "C" bool wkStandbyLogin__21CDeviceFileJobReadDvdFv(CDeviceFileJobReadDvdLayout* self) {
    CWorkThread* wt = reinterpret_cast<CWorkThread*>(self);
    if (wt->isNoEvent()) {
        return wt->CWorkThread::wkStandbyLogin();
    } else if (wt->wkIsCurrent()) {
        if (self->mHandle->checkExistRsrc(CBM_1)) {
            CDeviceFile::removeFileJob(reinterpret_cast<CDeviceFileJob*>(self));
        }

        return wt->CWorkThread::wkStandbyLogin();
    }

    return false;
}

extern "C" bool wkStandbyLogout__21CDeviceFileJobReadDvdFv(CDeviceFileJobReadDvdLayout* self) {
    //Cancel the associated file handle if there is one
    if (self->mHandle != nullptr) {
        reinterpret_cast<CDeviceFileJobReadDvdVtbl*>(self)->cancel(self->mHandle);
        return false;
    }

    return reinterpret_cast<CWorkThread*>(self)->CWorkThread::wkStandbyLogout(); //Call base
}

extern "C" void callCBM3__21CDeviceFileJobReadDvdFv(void* selfPtr) {
    CDeviceFileJobReadDvdLayout* self = reinterpret_cast<CDeviceFileJobReadDvdLayout*>(selfPtr);
    if (self->mHandle != nullptr) {
        self->mHandle->call(CBM_3);
    }

    CDeviceFile::removeFileJob(reinterpret_cast<CDeviceFileJob*>(self));
}



// [.rodata] size 0x40 align 8   (0 relocs)
extern "C" __declspec(align(8)) const char lbl_eu_80522D50[24] = { 0x43, 0x44, 0x65, 0x76, 0x69, 0x63, 0x65, 0x46, 0x69, 0x6C, 0x65, 0x4A, 0x6F, 0x62, 0x52, 0x65, 0x61, 0x64, 0x44, 0x76, 0x64, 0x00, 0x00, 0x00 };
extern "C" __declspec(align(4)) const char lbl_eu_80522D68[20] = { 0x66, 0x6F, 0x6E, 0x74, 0x2F, 0x66, 0x6F, 0x6E, 0x74, 0x5F, 0x65, 0x75, 0x35, 0x2E, 0x62, 0x72, 0x66, 0x6E, 0x61, 0x00 };
extern "C" __declspec(align(4)) const char lbl_eu_80522D7C[20] = { 0x66, 0x6F, 0x6E, 0x74, 0x2F, 0x4D, 0x65, 0x6E, 0x75, 0x46, 0x6F, 0x6E, 0x74, 0x2E, 0x62, 0x72, 0x66, 0x6E, 0x61, 0x00 };

// [.sdata] size 0x8 align 8   (2 relocs)
extern "C" u32 lbl_eu_806636F0[2] = { (u32)&lbl_eu_80522D50, (u32)&lbl_eu_8056C644 };

// [.data] size 0xC8 align 8   (45 relocs)
extern "C" u32 lbl_eu_8056C598[43] = { (u32)&lbl_eu_806636F0, 0x00000000, (u32)&__dt__21CDeviceFileJobReadDvdFv, (u32)&M2Blob::WorkEvent1__10IWorkEventFPvPCc, (u32)&M2Blob::OnFileEvent__10IWorkEventFP10CEventFile, (u32)&M2Blob::WorkEvent3__10IWorkEventFPv, (u32)&M2Blob::WorkEvent4__10IWorkEventFv, (u32)&M2Blob::OnPauseTrigger__10IWorkEventFb, (u32)&M2Blob::WorkEvent6__10IWorkEventFv, (u32)&M2Blob::WorkEvent7__10IWorkEventFv, (u32)&M2Blob::WorkEvent8__10IWorkEventFv, (u32)&M2Blob::WorkEvent9__10IWorkEventFv, (u32)&M2Blob::WorkEvent10__10IWorkEventFv, (u32)&M2Blob::WorkEvent11__10IWorkEventFv, (u32)&M2Blob::WorkEvent12__10IWorkEventFv, (u32)&M2Blob::WorkEvent13__10IWorkEventFv, (u32)&M2Blob::WorkEvent14__10IWorkEventFv, (u32)&M2Blob::WorkEvent15__10IWorkEventFv, (u32)&M2Blob::WorkEvent16__10IWorkEventFv, (u32)&M2Blob::WorkEvent17__10IWorkEventFv, (u32)&M2Blob::WorkEvent18__10IWorkEventFv, (u32)&M2Blob::WorkEvent19__10IWorkEventFv, (u32)&M2Blob::WorkEvent20__10IWorkEventFv, (u32)&M2Blob::WorkEvent21__10IWorkEventFv, (u32)&M2Blob::WorkEvent22__10IWorkEventFv, (u32)&M2Blob::WorkEvent23__10IWorkEventFv, (u32)&M2Blob::WorkEvent24__10IWorkEventFv, (u32)&M2Blob::WorkEvent25__10IWorkEventFv, (u32)&M2Blob::WorkEvent26__10IWorkEventFv, (u32)&M2Blob::WorkEvent27__10IWorkEventFv, (u32)&M2Blob::WorkEvent28__10IWorkEventFv, (u32)&M2Blob::WorkEvent29__10IWorkEventFv, (u32)&M2Blob::WorkEvent30__10IWorkEventFv, (u32)&M2Blob::WorkEvent31__10IWorkEventFv, (u32)&wkUpdate__21CDeviceFileJobReadDvdFv, (u32)&M2Blob::wkRender__11CWorkThreadFv, (u32)&M2Blob::wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__21CDeviceFileJobReadDvdFv, (u32)&wkStandbyLogout__21CDeviceFileJobReadDvdFv, (u32)&M2Blob::wkStandbyExceptionRetry__11CWorkThreadFUl, (u32)&M2Blob::CDeviceFileJob_UnkVirtualFunc1__14CDeviceFileJobFv, (u32)&cancel__21CDeviceFileJobReadDvdFPCc, (u32)&cancel__21CDeviceFileJobReadDvdFP11CFileHandle };
extern "C" u32 lbl_eu_8056C644[7] = { (u32)&M2Blob::__RTTI__10IWorkEvent, 0x00000000, (u32)&M2Blob::__RTTI__11CWorkThread, 0x00000000, (u32)&lbl_eu_806636E8, 0x00000000, 0x00000000 };

