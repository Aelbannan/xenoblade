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
        BasicSound* pLowest = GetLowestPrioritySound();

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
        BasicSound* pLowest = GetLowestPrioritySound();

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
