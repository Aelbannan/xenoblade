#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

bool ExternalSoundPlayer::AppendSound(BasicSound* pSound) {
    SoundThread::AutoLock lock;

    int priority = pSound->CalcCurrentPlayerPriority();

    if (mPlayableCount == 0) {
        return false;
    }

    while (GetPlayingSoundCount() >= mPlayableCount) {
        int lowestPrio = BasicSound::PRIORITY_MAX + 1;
        BasicSound* pLowest = NULL;

        for (BasicSoundExtPlayList::Iterator it = mSoundList.GetBeginIter();
             it != mSoundList.GetEndIter(); ++it) {

            int currentPrio = it->CalcCurrentPlayerPriority();

            if (lowestPrio > currentPrio) {
                lowestPrio = currentPrio;
                pLowest = &*it;
            }
        }

        if (pLowest == NULL) {
            return false;
        }

        if (priority < pLowest->CalcCurrentPlayerPriority()) {
            return false;
        }

        pLowest->Shutdown();
    }

    mSoundList.PushBack(pSound);
    pSound->AttachExternalSoundPlayer(this);

    return true;
}

void ExternalSoundPlayer::RemoveSound(BasicSound* pSound) {
    mSoundList.Erase(pSound);
    pSound->DetachExternalSoundPlayer(this);
}

bool ExternalSoundPlayer::detail_CanPlaySound(int count) {
    // No playable slots configured: nothing can play.
    if (mPlayableCount == 0) {
        return false;
    }

    // At (or above) the playable limit we need a lowest-priority candidate to
    // evict; otherwise playback is always allowed (retail structure funnels
    // all allowed paths into the single trailing return true).
    if (GetPlayingSoundCount() >= mPlayableCount) {
        int lowestPrio = BasicSound::PRIORITY_MAX + 1;
        BasicSound* pLowest = NULL;

        for (BasicSoundExtPlayList::Iterator it = mSoundList.GetBeginIter();
             it != mSoundList.GetEndIter(); ++it) {

            BasicSound* pNode = &*it;
            int currentPrio = it->CalcCurrentPlayerPriority();

            if (lowestPrio > currentPrio) {
                pLowest = pNode;
                lowestPrio = currentPrio;
            }
        }

        if (pLowest == NULL) {
            return false;
        }

        if (count < pLowest->CalcCurrentPlayerPriority()) {
            return false;
        }
    }

    return true;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
