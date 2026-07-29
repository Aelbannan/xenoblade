#include <nw4r/snd.h>
#include <nw4r/ut.h>

#include <revolution/AX.h>

namespace nw4r {
namespace snd {
namespace detail {

// Helper to access protected LinkListImpl members
struct LinkListImplAccess : ut::detail::LinkListImpl {
    using ut::detail::LinkListImpl::Erase;
    using ut::detail::LinkListImpl::Insert;
};

VoiceManager& VoiceManager::GetInstance() {
    static VoiceManager instance;
    return instance;
}

VoiceManager::VoiceManager() : mInitialized(false) {}

VoiceManager::~VoiceManager() {}

u32 VoiceManager::GetRequiredMemSize() {
    return AXGetMaxVoices() * sizeof(Voice);
}

u32 VoiceManager::GetRequiredMemSize(int numVoices) {
    return numVoices * 0x124;
}

void VoiceManager::Setup(void* pBuffer, u32 size) {
    if (mInitialized) {
        return;
    }

    u32 voices = size / 0x124;
    u8* pPtr = static_cast<u8*>(pBuffer);

    for (u32 i = 0; i < voices; i++) {
        Voice* pVoice = reinterpret_cast<Voice*>(pPtr);
        if (pPtr != NULL) {
            pVoice = new (pPtr) Voice();
        }

        // Insert at back of free list: node at pVoice + 0x11C
        ut::LinkListNode* pNode = reinterpret_cast<ut::LinkListNode*>(
            reinterpret_cast<u8*>(pVoice) + 0x11C);
        ut::detail::LinkListImpl::Iterator it(
            reinterpret_cast<ut::LinkListNode*>(
                reinterpret_cast<u8*>(&mFreeVoiceList) + 0x4));
        reinterpret_cast<LinkListImplAccess*>(&mFreeVoiceList)->Insert(
            it, pNode);

        pPtr += 0x124;
    }

    mInitialized = true;
}

void VoiceManager::Shutdown() {
    if (!mInitialized) {
        return;
    }

    StopAllVoices();

    while (!mFreeVoiceList.IsEmpty()) {
        Voice& rVoice = mFreeVoiceList.GetFront();
        mFreeVoiceList.PopFront();
        rVoice.~Voice();
    }

    mInitialized = false;
}

void VoiceManager::StopAllVoices() {
    ut::AutoInterruptLock lock;

    while (!mPrioVoiceList.IsEmpty()) {
        Voice& rVoice = mPrioVoiceList.GetFront();

        rVoice.Stop();

        if (rVoice.mCallback != NULL) {
            rVoice.mCallback(&rVoice, Voice::CALLBACK_STATUS_CANCEL,
                             rVoice.mCallbackArg);
        }

        rVoice.Free();
    }
}

Voice* VoiceManager::AllocVoice(int channels, int voices, int priority,
                                Voice::VoiceCallback pCallback,
                                void* pCallbackArg) {

    ut::AutoInterruptLock lock;

    if (mFreeVoiceList.IsEmpty() && DropLowestPriorityVoice(priority) == 0) {
        return NULL;
    }

    Voice& rVoice = mFreeVoiceList.GetFront();
    if (!rVoice.Acquire(channels, voices, priority, pCallback, pCallbackArg)) {
        return NULL;
    }

    rVoice.mPriority = priority & Voice::PRIORITY_MAX;
    AppendVoiceList(&rVoice);
    UpdateEachVoicePriority(mPrioVoiceList.GetIteratorFromPointer(&rVoice),
                            mPrioVoiceList.GetEndIter());
    DisposeCallbackManager::GetInstance().RegisterDisposeCallback(&rVoice);

    return &rVoice;
}

void VoiceManager::FreeVoice(Voice* pVoice) {
    BOOL enabled = OSDisableInterrupts();

    DisposeCallbackManager::GetInstance().UnregisterDisposeCallback(pVoice);

    BOOL enabled2 = OSDisableInterrupts();

    // Compute node pointer using retail offset (0x11C within Voice)
    ut::LinkListNode* pNode = reinterpret_cast<ut::LinkListNode*>(
        reinterpret_cast<u8*>(pVoice) + 0x11C);

    // Erase from priority list
    reinterpret_cast<LinkListImplAccess*>(&mPrioVoiceList)->Erase(pNode);

    // Insert at back of free list (before sentinel node)
    ut::LinkListNode* pSentinel = reinterpret_cast<ut::LinkListNode*>(
        reinterpret_cast<u8*>(&mFreeVoiceList) + 0x4);
    ut::detail::LinkListImpl::Iterator it(pSentinel);
    reinterpret_cast<LinkListImplAccess*>(&mFreeVoiceList)->Insert(it, pNode);

    OSRestoreInterrupts(enabled2);
    OSRestoreInterrupts(enabled);
}

void VoiceManager::UpdateAllVoices() {
    // Save this into a local so it lives in a callee-saved register across the
    // three passes (matches retail keeping this in r29).
    u8* pBase = reinterpret_cast<u8*>(this);

    ut::LinkListNode* pEnd;
    ut::LinkListNode* pNode;

    // First pass: StopFinished on all voices
    pEnd = reinterpret_cast<ut::LinkListNode*>(pBase + 0x8);
    pNode = pEnd->GetNext();

    while (pNode != pEnd) {
        ut::LinkListNode* pCurr = pNode;
        pNode = pNode->GetNext();
        reinterpret_cast<Voice*>(
            reinterpret_cast<u8*>(pCurr) - 0x11C)->StopFinished();
    }

    // Second pass: Calc on all voices
    pEnd = reinterpret_cast<ut::LinkListNode*>(pBase + 0x8);
    pNode = pEnd->GetNext();

    while (pNode != pEnd) {
        ut::LinkListNode* pCurr = pNode;
        pNode = pNode->GetNext();
        reinterpret_cast<Voice*>(
            reinterpret_cast<u8*>(pCurr) - 0x11C)->Calc();
    }

    // Third pass: Update on all voices with interrupts disabled.
    // Compute sentinel BEFORE capturing the interrupt return value so the
    // register allocation matches retail (lwzu pattern).
    {
        pEnd = reinterpret_cast<ut::LinkListNode*>(pBase + 0x8);
        BOOL enabled = OSDisableInterrupts();
        pNode = pEnd->GetNext();

        while (pNode != pEnd) {
            ut::LinkListNode* pCurr = pNode;
            pNode = pNode->GetNext();
            reinterpret_cast<Voice*>(
                reinterpret_cast<u8*>(pCurr) - 0x11C)->Update();
        }

        OSRestoreInterrupts(enabled);
    }
}

void VoiceManager::NotifyVoiceUpdate() {
    BOOL enabled = OSDisableInterrupts();

    ut::LinkListNode* pNode;
    ut::LinkListNode* pEnd =
        reinterpret_cast<ut::LinkListNode*>(reinterpret_cast<u8*>(this) + 0x8);
    pNode = pEnd->GetNext();

    while (pNode != pEnd) {
        ut::LinkListNode* pCurr = pNode;
        pNode = pNode->GetNext();
        reinterpret_cast<Voice*>(reinterpret_cast<u8*>(pCurr) - 0x11C)
            ->ResetDelta();
    }

    OSRestoreInterrupts(enabled);
}

void VoiceManager::AppendVoiceList(Voice* pVoice) {
    ut::AutoInterruptLock lock;

    mFreeVoiceList.Erase(pVoice);

    VoiceList::RevIterator it = mPrioVoiceList.GetEndReverseIter();
    for (; it != mPrioVoiceList.GetBeginReverseIter(); ++it) {
        if (it->GetPriority() <= pVoice->GetPriority()) {
            break;
        }
    }

    mPrioVoiceList.Insert(it.GetBase(), pVoice);
}

void VoiceManager::RemoveVoiceList(Voice* pVoice) {
    ut::AutoInterruptLock lock;

    mPrioVoiceList.Erase(pVoice);
    mFreeVoiceList.PushBack(pVoice);
}

void VoiceManager::ChangeVoicePriority(Voice* pVoice) {
    ut::AutoInterruptLock lock;

    RemoveVoiceList(pVoice);
    AppendVoiceList(pVoice);

    UpdateEachVoicePriority(mPrioVoiceList.GetIteratorFromPointer(pVoice),
                            mPrioVoiceList.GetEndIter());
}

void VoiceManager::UpdateEachVoicePriority(const VoiceList::Iterator& rBegin,
                                           const VoiceList::Iterator& rEnd) {

    for (VoiceList::Iterator it = rBegin; it != rEnd; ++it) {
        if (it->GetPriority() <= 1) {
            return;
        }

        if (it->GetPriority() != Voice::PRIORITY_MAX) {
            it->UpdateVoicesPriority();
        }
    }
}

void VoiceManager::UpdateAllVoicesSync(u32 syncFlag) {
    BOOL enabled = OSDisableInterrupts();

    // Traverse priority list manually, matching retail pattern
    u8* pBase = reinterpret_cast<u8*>(&mPrioVoiceList);
    ut::LinkListNode* pEnd = reinterpret_cast<ut::LinkListNode*>(pBase + 0x4);
    ut::LinkListNode* pNode = pEnd->GetNext();

    while (pNode != pEnd) {
        ut::LinkListNode* pCurr = pNode;
        pNode = pNode->GetNext();

        // Get Voice* from node using retail offset (0x11C)
        Voice* pVoice = reinterpret_cast<Voice*>(
            reinterpret_cast<u8*>(pCurr) - 0x11C);

        if (pVoice->mIsActive) {
            // mSyncFlag accessed as u16 to match retail lhz/sth
            pVoice->mSyncFlag |= syncFlag;
        }
    }

    OSRestoreInterrupts(enabled);
}

int VoiceManager::DropLowestPriorityVoice(int priority) {
    int dropped = 0;

    if (mFreeVoiceList.IsEmpty()) {
        Voice& rVoice = mPrioVoiceList.GetFront();

        if (rVoice.GetPriority() > priority) {
            return 0;
        }

        dropped = rVoice.GetAxVoiceCount();

        rVoice.Stop();
        rVoice.Free();

        if (rVoice.mCallback != NULL) {
            rVoice.mCallback(&rVoice, Voice::CALLBACK_STATUS_DROP_VOICE,
                             rVoice.mCallbackArg);
        }
    }

    return dropped;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
