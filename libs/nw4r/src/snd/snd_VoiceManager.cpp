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

// Retail-accurate mirror of the Voice object field layout. The class defined
// in snd_Voice.h has different (non-retail) member offsets, so the VoiceManager
// list/priority logic below accesses fields through this mirror at their exact
// retail offsets (node=0x11C, mPriority=0xA8, mCallback=0x94, mSyncFlag=0xA2,
// mIsActive=0x9C, ...). Kept local to this TU like the mirror in snd_Voice.cpp.
struct VoiceLayout {
    u8 _pad0[0xC];                                 // 0x0
    void* mAxVoice[2][4];                          // 0xC
    float mVoiceOutParam[4][6];                    // 0x2C
    int mChannelCount;                             // 0x8C
    int mVoiceOutCount;                            // 0x90
    void* mCallback;                               // 0x94
    void* mCallbackArg;                            // 0x98
    bool mIsActive;                                // 0x9C
    bool mIsStarting;                              // 0x9D
    bool mIsStarted;                               // 0x9E
    bool mIsPause;                                 // 0x9F
    bool mIsPausing;                               // 0xA0
    u8 field_0xA1;                                 // 0xA1
    u16 mSyncFlag;                                 // 0xA2
    u8 mRemoteFilter;                              // 0xA4
    u8 mBiquadType;                                // 0xA5
    int mPriority;                                 // 0xA8
    float mPan;                                    // 0xAC
    float mSurroundPan;                            // 0xB0
    float mLpfFreq;                                // 0xB4
    float mBiquadFreq;                             // 0xB8
    int mOutputLineFlag;                           // 0xBC
    float mMainOutVolume;                          // 0xC0
    float mMainSend;                               // 0xC4
    float mFxSend[AUX_BUS_NUM];                    // 0xC8
    float mRemoteOutVolume[WPAD_MAX_CONTROLLERS];  // 0xD4
    float mRemoteSend[WPAD_MAX_CONTROLLERS];       // 0xE4
    float mRemoteFxSend[WPAD_MAX_CONTROLLERS];     // 0xF4
    float mPitch;                                  // 0x104
    float mVolume;                                 // 0x108
    float mVeInitVolume;                           // 0x10C
    float mVeTargetVolume;                         // 0x110
    int field_0x114;                               // 0x114
    int field_0x118;                               // 0x118
    ut::LinkListNode node;                         // 0x11C
};

// VoiceList equivalent over the retail-accurate overlay (node at 0x11C).
// Cast a VoiceManager list member to this when iterating / inserting.
typedef ut::LinkList<VoiceLayout, 0x11C> VoiceLayoutList;
static inline Voice* ToVoice(VoiceLayout* pLayout) {
    return reinterpret_cast<Voice*>(pLayout);
}

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

    u32 count = size / 0x124;
    void* pPtr = pBuffer;

    for (u32 i = 0; i < count; i++) {
        // Placement-new each slot; the node lives at offset 0x11C.
        reinterpret_cast<VoiceLayoutList*>(&mFreeVoiceList)
            ->PushBack(reinterpret_cast<VoiceLayout*>(new (pPtr) Voice()));

        pPtr = static_cast<u8*>(pPtr) + 0x124;
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

    // Third pass: Update on all voices with interrupts disabled
    {
        BOOL enabled = OSDisableInterrupts();

        // Fresh locals so pass 3 gets its own register assignment
        // (retail keeps the iterators apart from passes 1-2).
        ut::LinkListNode* pUpdNode =
            reinterpret_cast<ut::LinkListNode*>(pBase + 0x8)->GetNext();
        ut::LinkListNode* pUpdEnd =
            reinterpret_cast<ut::LinkListNode*>(pBase + 0x8);

        while (pUpdNode != pUpdEnd) {
            ut::LinkListNode* pCurr = pUpdNode;
            pUpdNode = pUpdNode->GetNext();
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
    VoiceLayoutList* list = reinterpret_cast<VoiceLayoutList*>(&mPrioVoiceList);
    VoiceLayoutList::Iterator iter = list->GetBeginIter();
    while (iter != list->GetEndIter()) {
        VoiceLayout* voice = &*iter;
        ++iter;
        if (voice->mIsActive) {
            voice->mSyncFlag |= syncFlag;
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


