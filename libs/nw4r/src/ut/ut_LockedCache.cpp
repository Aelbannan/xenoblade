#include <nw4r/ut.h>

namespace nw4r {
namespace ut {
namespace {

class LCImpl {
public:
    class Lock_ {
    public:
        Lock_(LCImpl& rImpl) : mMutex(rImpl.mMutex) {
            OSLockMutex(&mMutex);
        }
        ~Lock_() {
            OSUnlockMutex(&mMutex);
        }

    private:
        OSMutex& mMutex; // at 0x0
    };

public:
    LCImpl() : mIsEnabled(false) {
        OSInitMutex(&mMutex);
    }

    // NOTE: the Lock_ guard inlines to plain OSLock/OSUnlockMutex pairs
    // (Enable/Disable match retail byte-for-byte), but MWCC also emits a
    // TU-local deleting-dtor copy for the guard class that the retail split
    // does not keep (retail linker GC'd it; a drop_text_symbols rule covers
    // the post-link object). The guard stays: spelling the calls directly
    // perturbs the allocator (mutex address rematerialized instead of kept
    // live in r31) and regresses both functions, so the copy is irreducible
    // in source shape.
    void Enable() {
        Lock_ lock(*this);

        if (!mIsEnabled) {
            LCEnable();
            mIsEnabled = true;
        }
    }

    void Disable() {
        Lock_ lock(*this);

        if (mIsEnabled) {
            LC::QueueWaitEx(0);
            LCDisable();
            mIsEnabled = false;
        }
    }

    bool Lock() {
        OSLockMutex(&mMutex);

        if (mIsEnabled) {
            LC::QueueWaitEx(0);
            return true;
        }

        OSUnlockMutex(&mMutex);
        return false;
    }

    void Unlock() {
        LC::QueueWaitEx(0);
        OSUnlockMutex(&mMutex);
    }

public:
    bool mIsEnabled;        // at 0x0
    mutable OSMutex mMutex; // at 0x4
};

} // namespace

// Retail .bss 0x80653E88-0x80653EA8 (0x20): LCImpl (0x1C) + 0x4 pad. Absorbed from nw4r_data.s.
struct LCImplPadded : LCImpl {
    unsigned char _pad[4];
};
extern "C" {
LCImplPadded lbl_eu_80653E88 __attribute__((aligned(8)));
}
// NOTE: the compiler-generated __sinit_ for this TU already reproduces the
// retail static-init (mIsEnabled = false + OSInitMutex tail call); no
// hand-written sinit is needed (it would only duplicate .text).

namespace LC {

void Enable() {
    lbl_eu_80653E88.Enable();
}

void Disable() {
    lbl_eu_80653E88.Disable();
}

bool Lock() {
    return lbl_eu_80653E88.Lock();
}

void Unlock() {
    lbl_eu_80653E88.Unlock();
}

void LoadBlocks(void* pDst, void* pSrc, u32 blocks) {
    LCLoadBlocks(pDst, pSrc, blocks);
}

void StoreBlocks(void* pDst, void* pSrc, u32 blocks) {
    LCStoreBlocks(pDst, pSrc, blocks);
}

void StoreData(void* pDst, void* pSrc, u32 size) {
    LCStoreData(pDst, pSrc, size);
}

} // namespace LC
} // namespace ut
} // namespace nw4r
