#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

namespace {

// Single-callsite helpers; MWCC inlines them back into their callers.
// Factoring the min-search loop out reorders the inlining expansion of the IR
// web, which is the only lever that shifts MWCC's Chaitin color tie-break
// between pLowest and the other loop locals.
BasicSound* FindLowestPrioritySound(
    int& lowestPrio,
    BasicSoundExtPlayList& rList) {
    lowestPrio = BasicSound::PRIORITY_MAX + 1;
    BasicSound* pLowest = NULL;

    for (BasicSoundExtPlayList::Iterator it = rList.GetBeginIter();
         it != rList.GetEndIter(); ++it) {

        BasicSound* pCurrent = &*it;
        int currentPrio = pCurrent->CalcCurrentPlayerPriority();

        if (lowestPrio > currentPrio) {
            pLowest = pCurrent;
            lowestPrio = currentPrio;
        }
    }

    return pLowest;
}

} // namespace

bool ExternalSoundPlayer::AppendSound(BasicSound* pSound) {
    SoundThread::AutoLock lock;

    int priority = pSound->CalcCurrentPlayerPriority();

    if (mPlayableCount == 0) {
        return false;
    }

    while (GetPlayingSoundCount() >= mPlayableCount) {
        int lowestPrio;
        BasicSound* pLowest =
            FindLowestPrioritySound(lowestPrio, mSoundList);

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
        int lowestPrio;
        BasicSound* pLowest =
            FindLowestPrioritySound(lowestPrio, mSoundList);

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
