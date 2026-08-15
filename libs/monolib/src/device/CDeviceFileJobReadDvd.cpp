#include "monolib/device.hpp"
#include "monolib/core.hpp"

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
extern "C" u8 lbl_eu_8056C598[];

// Extern "C" calls whose retail names do NOT collide with header member
// declarations.  Base-class member calls (CWorkThread/CDeviceFile/CDeviceFileDvd/
// CRsrc) go through the real classes so the compiler emits the retail mangled
// symbols directly.
extern "C" void __ct__14CDeviceFileJobFPCcP11CWorkThread(void* self, const char* pName, CWorkThread* pParent);
extern "C" void __dt__11CWorkThreadFv(void* self, int flag);
extern "C" void __dl__FPv(void*);

extern "C" CDeviceFileJobReadDvdLayout* __ct__21CDeviceFileJobReadDvdFPCcP11CWorkThread(
    CDeviceFileJobReadDvdLayout* self, const char* pName, CWorkThread* pParent) {
    __ct__14CDeviceFileJobFPCcP11CWorkThread(self, pName, pParent);
    self->vtbl = (void*)lbl_eu_8056C598;
    self->mType = THREAD_CDEVICEFILEJOBREADDVD;
    // Why not initialize mDvdFileInfo??
    return self;
}

extern "C" CDeviceFileJobReadDvdLayout* __dt__21CDeviceFileJobReadDvdFv(
    CDeviceFileJobReadDvdLayout* self, int flag) {
    if (self != 0) {
        __dt__11CWorkThreadFv(self, 0);
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
        CDeviceFileCri::func_8044FC38();
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
        CDeviceFileCri::func_8044FC38();
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
        return wt->wkStandbyLogin();
    } else if (wt->wkIsCurrent()) {
        if (self->mHandle->checkExistRsrc(CBM_1)) {
            CDeviceFile::removeFileJob(reinterpret_cast<CDeviceFileJob*>(self));
        }

        return wt->wkStandbyLogin();
    }

    return false;
}

extern "C" bool wkStandbyLogout__21CDeviceFileJobReadDvdFv(CDeviceFileJobReadDvdLayout* self) {
    //Cancel the associated file handle if there is one
    if (self->mHandle != nullptr) {
        reinterpret_cast<CDeviceFileJobReadDvdVtbl*>(self)->cancel(self->mHandle);
        return false;
    }

    return reinterpret_cast<CWorkThread*>(self)->wkStandbyLogout(); //Call base
}

extern "C" void callCBM3__21CDeviceFileJobReadDvdFv(CDeviceFileJobReadDvdLayout* self) {
    if (self->mHandle != nullptr) {
        self->mHandle->call(CBM_3);
    }

    CDeviceFile::removeFileJob(reinterpret_cast<CDeviceFileJob*>(self));
}
