#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace snd {

SoundPlayer::SoundPlayer() {
    // Retail inlines field init (no OSMutex; no separate InitParam in this TU).
    mPlayableCount = 1;
    mPlayableLimit = 0x7FFFFFFF;
    mVolume = 1.0f;
    mUnk0x30 = 0.0f;
    mOutputLineFlag = OUTPUT_LINE_MAIN;
    mMainOutVolume = 1.0f;
    mOutputLineFlagEnable = false;
    mUsePlayerHeap = false;
    mUnk0x40 = 0.0f;
    mUnk0x54 = 0.0f;

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        mRemoteOutVolume[i] = 1.0f;
    }
    for (int i = 0; i < AUX_BUS_NUM; i++) {
        mFxSend[i] = 0.0f;
    }
}

SoundPlayer::~SoundPlayer() {
    StopAllSound(0);
}

void SoundPlayer::Update() {
    detail::SoundThread::AutoLock lock;

    NW4R_UT_LINKLIST_FOREACH_SAFE (it, mSoundList, { it->Update(); })

    detail_SortPriorityList();
}

void SoundPlayer::StopAllSound(int frames) {
    detail::SoundThread::AutoLock lock;

    NW4R_UT_LINKLIST_FOREACH_SAFE (it, mSoundList, { it->Stop(frames); })
}

void SoundPlayer::PauseAllSound(bool flag, int frames) {
    detail::SoundThread::AutoLock lock;

    NW4R_UT_LINKLIST_FOREACH_SAFE (it, mSoundList,
                                   { it->Pause(flag, frames); })
}

f32 SoundPlayer::GetRemoteOutVolume(int idx) const {
    return mRemoteOutVolume[idx];
}

void SoundPlayer::SetFxSend(AuxBus bus, f32 send) {
    mFxSend[bus] = send;
}

void SoundPlayer::detail_SortPriorityList(detail::BasicSound* pSound) {
    // Retail: lock → erase → unlock → lock → insert → unlock (SoundThread mutex).
    {
        detail::SoundThread::AutoLock lock;
        mPriorityList.Erase(pSound);
    }
    {
        detail::SoundThread::AutoLock lock;

        detail::BasicSoundPlayerPrioList::Iterator it =
            mPriorityList.GetBeginIter();

        for (; it != mPriorityList.GetEndIter(); ++it) {
            if (pSound->CalcCurrentPlayerPriority() <
                it->CalcCurrentPlayerPriority()) {
                break;
            }
        }

        mPriorityList.Insert(it, pSound);
    }
}

void SoundPlayer::detail_SortPriorityList() {
    detail::SoundThread::AutoLock lock;

    // Retail early-outs when fewer than 2 entries (count @ list+0).
    if (mPriorityList.GetSize() < 2) {
        return;
    }

    // Static buckets (retail lbl_eu_806382F0), not a stack array.
    static detail::BasicSoundPlayerPrioList
        listsByPrio[detail::BasicSound::PRIORITY_MAX + 1];

    while (!mPriorityList.IsEmpty()) {
        detail::BasicSound& rSound = mPriorityList.GetFront();
        mPriorityList.PopFront();
        listsByPrio[rSound.CalcCurrentPlayerPriority()].PushBack(&rSound);
    }

    for (int i = 0; i < detail::BasicSound::PRIORITY_MAX + 1; i++) {
        while (!listsByPrio[i].IsEmpty()) {
            detail::BasicSound& rSound = listsByPrio[i].GetFront();
            listsByPrio[i].PopFront();
            mPriorityList.PushBack(&rSound);
        }
    }
}

bool SoundPlayer::detail_AppendSound(detail::BasicSound* pSound) {
    detail::SoundThread::AutoLock lock;

    int priority = pSound->CalcCurrentPlayerPriority();

    if (GetPlayableSoundCount() == 0) {
        return false;
    }

    while (GetPlayingSoundCount() >= GetPlayableSoundCount()) {
        detail::BasicSound* pDrop = detail_GetLowestPrioritySound();

        if (pDrop == NULL) {
            return false;
        }

        if (priority < pDrop->CalcCurrentPlayerPriority()) {
            return false;
        }

        pDrop->Shutdown();
    }

    mSoundList.PushBack(pSound);

    {
        detail::SoundThread::AutoLock lock2;

        detail::BasicSoundPlayerPrioList::Iterator it =
            mPriorityList.GetBeginIter();

        for (; it != mPriorityList.GetEndIter(); ++it) {
            if (pSound->CalcCurrentPlayerPriority() <
                it->CalcCurrentPlayerPriority()) {
                break;
            }
        }

        mPriorityList.Insert(it, pSound);
    }

    pSound->AttachSoundPlayer(this);
    return true;
}

void SoundPlayer::detail_RemoveSound(detail::BasicSound* pSound) {
    {
        detail::SoundThread::AutoLock lock;
        mPriorityList.Erase(pSound);
    }
    {
        detail::SoundThread::AutoLock lock;
        mSoundList.Erase(pSound);
        pSound->DetachSoundPlayer(this);
    }
}

bool SoundPlayer::detail_CanPlaySound(int priority) {
    detail::SoundThread::AutoLock lock;

    if (GetPlayableSoundCount() == 0) {
        return false;
    }

    if (GetPlayingSoundCount() >= GetPlayableSoundCount()) {
        detail::BasicSound* pLowest = detail_GetLowestPrioritySound();

        if (pLowest == NULL) {
            return false;
        }

        if (priority < pLowest->CalcCurrentPlayerPriority()) {
            return false;
        }
    }

    return true;
}

void SoundPlayer::detail_AppendPlayerHeap(detail::PlayerHeap* pHeap) {
    detail::SoundThread::AutoLock lock;

    pHeap->SetSoundPlayer(this);
    mHeapList.PushBack(pHeap);
}

detail::PlayerHeap*
SoundPlayer::detail_AllocPlayerHeap(detail::BasicSound* pSound) {
    detail::SoundThread::AutoLock lock;

    if (mHeapList.IsEmpty()) {
        return NULL;
    }

    detail::PlayerHeap& rHeap = mHeapList.GetFront();
    mHeapList.PopFront();

    rHeap.AttachSound(pSound);
    pSound->AttachPlayerHeap(&rHeap);
    rHeap.Clear();

    return &rHeap;
}

void SoundPlayer::detail_FreePlayerHeap(detail::BasicSound* pSound) {
    detail::SoundThread::AutoLock lock;

    detail::PlayerHeap* pHeap = pSound->GetPlayerHeap();

    if (pHeap == NULL) {
        return;
    }

    pHeap->DetachSound(pSound);
    pSound->DetachPlayerHeap(pHeap);
    mHeapList.PushBack(pHeap);
}

void SoundPlayer::SetPlayableSoundCount(int count) {
    detail::SoundThread::AutoLock lock;

    // Retail always clamps to [0, mPlayableLimit] (no mUsePlayerHeap gate).
    mPlayableCount = ut::Clamp(count, 0, static_cast<int>(mPlayableLimit));

    while (GetPlayingSoundCount() > GetPlayableSoundCount()) {
        detail::BasicSound* pDropSound = detail_GetLowestPrioritySound();
        pDropSound->Shutdown();
    }
}

void nw4r::snd::SoundPlayer::detail_SetPlayableSoundLimit(int limit) {
    mPlayableLimit = limit;
}

} // namespace snd
} // namespace nw4r
