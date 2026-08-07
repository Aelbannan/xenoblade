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

private:
    bool mIsEnabled;        // at 0x0
    mutable OSMutex mMutex; // at 0x4
};

} // namespace

// BSS object (retail symbol lbl_eu_80653E88). C-linkage definition so the
// static-init reloc carries the retail global name instead of an
// anonymous-namespace mangling.
extern "C" {
LCImpl lbl_eu_80653E88;
}

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
