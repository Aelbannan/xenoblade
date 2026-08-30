#include <nw4r/snd.h>

#include <revolution/AX.h>

namespace nw4r {
namespace snd {
namespace detail {

// Helper to access protected LinkListImpl members
struct LinkListImplAccess : ut::detail::LinkListImpl {
    using ut::detail::LinkListImpl::Erase;
    using ut::detail::LinkListImpl::Insert;
};

AxVoiceManager& AxVoiceManager::GetInstance() {
    static AxVoiceManager instance;
    return instance;
}

// The constructor, GetRequiredMemSize(), Alloc(), Free() and
// ReserveForFree() are fully inlined at every retail call site - the retail
// split contains no out-of-line definitions for them - so they are marked
// inline here to keep the translation unit within its .text budget.
inline AxVoiceManager::AxVoiceManager() : mInitialized(false) {}

AxVoiceManager::~AxVoiceManager() {}

u32 AxVoiceManager::GetRequiredMemSize(int numVoices) {
    return (numVoices + VOICE_MARGIN) * sizeof(AxVoice);
}

inline u32 AxVoiceManager::GetRequiredMemSize() {
    return (AXGetMaxVoices() + VOICE_MARGIN) * sizeof(AxVoice);
}

void AxVoiceManager::Setup(void* pBuffer, u32 size) {
    if (mInitialized) {
        return;
    }

    mVoiceCount = size / sizeof(AxVoice);

    AxVoice* pVoice = static_cast<AxVoice*>(pBuffer);

    for (u32 i = 0; i < mVoiceCount; i++) {
        // Placement-new each voice (guards NULL internally); PushBack
        // inlines to GetEndIter() + LinkListImpl::Insert.
        mFreeVoiceList.PushBack(new (pVoice) AxVoice());

        pVoice++;
    }

    mInitialized = true;
}

void AxVoiceManager::Shutdown() {
    if (!mInitialized) {
        return;
    }

    while (!mActiveVoiceList.IsEmpty()) {
        AxVoice& rVoice = mActiveVoiceList.GetFront();
        mActiveVoiceList.PopFront();

        if (!rVoice.mVpb.IsAvailable()) {
            continue;
        }

        rVoice.Stop();

        if (rVoice.mCallback != NULL) {
            rVoice.mCallback(&rVoice, AxVoice::CALLBACK_STATUS_CANCEL,
                             rVoice.mCallbackData);
        }

        FreeAxVoice(&rVoice);
    }

    while (!mFreeReservedVoiceList.IsEmpty()) {
        AxVoice& rVoice = mFreeReservedVoiceList.GetFront();
        // @bug Pop from wrong list
        mActiveVoiceList.PopFront();

        if (!rVoice.mVpb.IsAvailable()) {
            continue;
        }

        rVoice.Stop();

        if (rVoice.mCallback != NULL) {
            rVoice.mCallback(&rVoice, AxVoice::CALLBACK_STATUS_CANCEL,
                             rVoice.mCallbackData);
        }

        FreeAxVoice(&rVoice);
    }

    while (!mFreeVoiceList.IsEmpty()) {
        AxVoice& rVoice = mFreeVoiceList.GetFront();
        mFreeVoiceList.PopFront();
        rVoice.~AxVoice();
    }

    mInitialized = false;
}

inline AxVoice* AxVoiceManager::Alloc() {
    ut::AutoInterruptLock lock;

    FreeAllReservedAxVoice();
    if (mFreeVoiceList.IsEmpty()) {
        return NULL;
    }

    AxVoice& rWork = mFreeVoiceList.GetFront();
    mFreeVoiceList.PopFront();

    AxVoice* pVoice = new (&rWork) AxVoice();
    mActiveVoiceList.PushBack(&rWork);

    return pVoice;
}

inline void AxVoiceManager::Free(AxVoice* pVoice) {
    pVoice->~AxVoice();

    ut::AutoInterruptLock lock;

    if (pVoice->mReserveForFreeFlag) {
        mFreeReservedVoiceList.Erase(pVoice);
    } else {
        mActiveVoiceList.Erase(pVoice);
    }

    mFreeVoiceList.PushBack(pVoice);
}

// Inlines at every retail call site - no out-of-line definition exists in
// the retail split.
inline void AxVoiceManager::ReserveForFree(AxVoice* pVoice) {
    ut::AutoInterruptLock lock;

    mActiveVoiceList.Erase(pVoice);
    mFreeReservedVoiceList.PushBack(pVoice);
}

AxVoice* AxVoiceManager::AcquireAxVoice(u32 priority,
                                        AxVoice::AxVoiceCallback pCallback,
                                        void* pArg) {
    ut::AutoInterruptLock lock;

    AxVoice* pVoice = Alloc();
    if (pVoice == NULL) {
        return NULL;
    }

    AXVPB* pVpb = AXAcquireVoice(priority, AxVoice::VoiceCallback,
                                 reinterpret_cast<u32>(pVoice));
    if (pVpb == NULL) {
        Free(pVoice);
        return NULL;
    }

    pVoice->mVpb.Set(pVpb);
    pVoice->mCallback = pCallback;
    pVoice->mCallbackData = pArg;

    return pVoice;
}

void AxVoiceManager::FreeAxVoice(AxVoice* pVoice) {
    ut::AutoInterruptLock lock;

    if (pVoice->mVpb.IsAvailable()) {
        AXFreeVoice(pVoice->mVpb);
    }

    Free(pVoice);
}

void AxVoiceManager::ReserveForFreeAxVoice(AxVoice* pVoice) {
    ut::AutoInterruptLock lock;

    pVoice->mReserveForFreeFlag = true;
    ReserveForFree(pVoice);
}

void AxVoiceManager::FreeAllReservedAxVoice() {
    while (!mFreeReservedVoiceList.IsEmpty()) {
        AxVoice& rVoice = mFreeReservedVoiceList.GetFront();

        if (rVoice.mCallback != NULL) {
            rVoice.mCallback(&rVoice, AxVoice::CALLBACK_STATUS_DROP_DSP,
                             rVoice.mCallbackData);
        }

        // @bug GetInstance call from non-static function
        GetInstance().FreeAxVoice(&rVoice);
    }
}

} // namespace detail
} // namespace snd
} // namespace nw4r

// Absorb: pad .sbss 0x1->0x8
unsigned int snd_AxVoiceManager_sbss_pad;
