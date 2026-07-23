#include <nw4r/snd.h>
#include <nw4r/ut.h>

#include <cstring>

// Alloc helpers live outside snd_SoundPlayer.cpp so that unit's .text fits the
// retail split (0xBA8). These symbols are not in the US SoundPlayer slice.

namespace nw4r {
namespace snd {

void SoundPlayer::InitParam() {
    mOutputLineFlag = OUTPUT_LINE_MAIN;
    mVolume = 1.0f;
    mOutputLineFlagEnable = false;

    mMainOutVolume = 1.0f;
    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        mRemoteOutVolume[i] = 1.0f;
    }
    for (int i = 0; i < AUX_BUS_NUM; i++) {
        mFxSend[i] = 0.0f;
    }
}

void SoundPlayer::SetVolume(f32 volume) {
    mVolume = ut::Clamp(volume, 0.0f, 1.0f);
}

void SoundPlayer::detail_InsertSoundList(detail::BasicSound* pSound) {
    mSoundList.PushBack(pSound);
    pSound->SetSoundPlayer(this);
}

void SoundPlayer::detail_RemoveSoundList(detail::BasicSound* pSound) {
    detail::SoundThread::AutoLock lock;

    mSoundList.Erase(pSound);
    pSound->SetSoundPlayer(NULL);
}

void SoundPlayer::detail_InsertPriorityList(detail::BasicSound* pSound) {
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

void SoundPlayer::detail_RemovePriorityList(detail::BasicSound* pSound) {
    detail::SoundThread::AutoLock lock;
    mPriorityList.Erase(pSound);
}

detail::SeqSound* SoundPlayer::detail_AllocSeqSound(
    int priority, int startPriority,
    detail::BasicSound::AmbientArgInfo* pArgInfo,
    detail::ExternalSoundPlayer* pExtPlayer, u32 id,
    detail::SoundInstanceManager<detail::SeqSound>* pManager) {

    detail::SoundThread::AutoLock lock;

    if (pManager == NULL) {
        return NULL;
    }

    int priorityReduction = CalcPriorityReduction(pArgInfo, id);

    startPriority = ut::Clamp(startPriority + priorityReduction, 0,
                              detail::BasicSound::PRIORITY_MAX);

    if (!CheckPlayableSoundCount(startPriority, pExtPlayer)) {
        return NULL;
    }

    detail::SeqSound* pSound = pManager->Alloc(startPriority);
    if (pSound == NULL) {
        return NULL;
    }

    detail_AllocPlayerHeap(pSound);

    if (pArgInfo != NULL) {
        InitAmbientArg(pSound, pArgInfo);
    }

    pSound->SetPriority(priority);
    pSound->GetAmbientParam().priority = priorityReduction;

    detail_InsertSoundList(pSound);

    if (pExtPlayer != NULL) {
        pExtPlayer->InsertSoundList(pSound);
    }

    detail_InsertPriorityList(pSound);

    return pSound;
}

detail::StrmSound* SoundPlayer::detail_AllocStrmSound(
    int priority, int startPriority,
    detail::BasicSound::AmbientArgInfo* pArgInfo,
    detail::ExternalSoundPlayer* pExtPlayer, u32 id,
    detail::SoundInstanceManager<detail::StrmSound>* pManager) {

    detail::SoundThread::AutoLock lock;

    if (pManager == NULL) {
        return NULL;
    }

    int priorityReduction = CalcPriorityReduction(pArgInfo, id);

    startPriority = ut::Clamp(startPriority + priorityReduction, 0,
                              detail::BasicSound::PRIORITY_MAX);

    if (!CheckPlayableSoundCount(startPriority, pExtPlayer)) {
        return NULL;
    }

    detail::StrmSound* pSound = pManager->Alloc(startPriority);
    if (pSound == NULL) {
        return NULL;
    }

    detail_AllocPlayerHeap(pSound);

    if (pArgInfo != NULL) {
        InitAmbientArg(pSound, pArgInfo);
    }

    pSound->SetPriority(priority);
    pSound->GetAmbientParam().priority = priorityReduction;

    detail_InsertSoundList(pSound);

    if (pExtPlayer != NULL) {
        pExtPlayer->InsertSoundList(pSound);
    }

    detail_InsertPriorityList(pSound);

    return pSound;
}

detail::WaveSound* SoundPlayer::detail_AllocWaveSound(
    int priority, int startPriority,
    detail::BasicSound::AmbientArgInfo* pArgInfo,
    detail::ExternalSoundPlayer* pExtPlayer, u32 id,
    detail::SoundInstanceManager<detail::WaveSound>* pManager) {

    detail::SoundThread::AutoLock lock;

    if (pManager == NULL) {
        return NULL;
    }

    int priorityReduction = CalcPriorityReduction(pArgInfo, id);

    startPriority = ut::Clamp(startPriority + priorityReduction, 0,
                              detail::BasicSound::PRIORITY_MAX);

    if (!CheckPlayableSoundCount(startPriority, pExtPlayer)) {
        return NULL;
    }

    detail::WaveSound* pSound = pManager->Alloc(startPriority);
    if (pSound == NULL) {
        return NULL;
    }

    detail_AllocPlayerHeap(pSound);

    if (pArgInfo != NULL) {
        InitAmbientArg(pSound, pArgInfo);
    }

    pSound->SetPriority(priority);
    pSound->GetAmbientParam().priority = priorityReduction;

    detail_InsertSoundList(pSound);

    if (pExtPlayer != NULL) {
        pExtPlayer->InsertSoundList(pSound);
    }

    detail_InsertPriorityList(pSound);

    return pSound;
}

int SoundPlayer::CalcPriorityReduction(
    detail::BasicSound::AmbientArgInfo* pArgInfo, u32 id) {

    int priority = 0;

    if (pArgInfo != NULL) {
        SoundParam param;

        pArgInfo->paramUpdateCallback->detail_Update(
            &param, id, NULL, pArgInfo->arg,
            detail::BasicSound::AmbientParamUpdateCallback::
                PARAM_UPDATE_PRIORITY);

        priority = param.priority;
    }

    return priority;
}

void SoundPlayer::InitAmbientArg(detail::BasicSound* pSound,
                                 detail::BasicSound::AmbientArgInfo* pArgInfo) {

    if (pArgInfo == NULL) {
        return;
    }

    void* pExtArg = pArgInfo->argAllocaterCallback->detail_AllocAmbientArg(
        pArgInfo->argSize);

    if (pExtArg == NULL) {
        return;
    }

    std::memcpy(pExtArg, pArgInfo->arg, pArgInfo->argSize);

    pSound->SetAmbientParamCallback(pArgInfo->paramUpdateCallback,
                                    pArgInfo->argUpdateCallback,
                                    pArgInfo->argAllocaterCallback, pExtArg);
}

bool SoundPlayer::CheckPlayableSoundCount(
    int startPriority, detail::ExternalSoundPlayer* pExtPlayer) {

    detail::SoundThread::AutoLock lock;

    if (GetPlayableSoundCount() == 0) {
        return false;
    }

    while (GetPlayingSoundCount() >= GetPlayableSoundCount()) {
        detail::BasicSound* pDropSound = detail_GetLowestPrioritySound();

        if (pDropSound == NULL) {
            return false;
        }

        if (startPriority < pDropSound->CalcCurrentPlayerPriority()) {
            return false;
        }

        pDropSound->Shutdown();
    }

    if (pExtPlayer != NULL) {
        if (pExtPlayer->GetPlayableSoundCount() == 0) {
            return false;
        }

        while (pExtPlayer->GetPlayingSoundCount() >=
               pExtPlayer->GetPlayableSoundCount()) {

            detail::BasicSound* pDropSound =
                pExtPlayer->GetLowestPrioritySound();

            if (pDropSound == NULL) {
                return false;
            }

            if (startPriority < pDropSound->CalcCurrentPlayerPriority()) {
                return false;
            }

            pDropSound->Shutdown();
        }
    }

    return true;
}

} // namespace snd
} // namespace nw4r
