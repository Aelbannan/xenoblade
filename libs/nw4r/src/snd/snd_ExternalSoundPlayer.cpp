#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

ExternalSoundPlayer::ExternalSoundPlayer() : mPlayableCount(1), mVolume(1.0f) {}

ExternalSoundPlayer::~ExternalSoundPlayer() {
    NW4R_UT_LINKLIST_FOREACH_SAFE (it, mSoundList,
                                   { it->SetExternalSoundPlayer(NULL); })
}

void ExternalSoundPlayer::SetPlayableSoundCount(int count) {
    mPlayableCount = count;

    while (GetPlayingSoundCount() > GetPlayableSoundCount()) {
        GetLowestPrioritySound()->Shutdown();
    }
}

void ExternalSoundPlayer::InsertSoundList(BasicSound* pSound) {
    mSoundList.PushBack(pSound);
    pSound->SetExternalSoundPlayer(this);
}

void ExternalSoundPlayer::RemoveSoundList(BasicSound* pSound) {
    mSoundList.Erase(pSound);
    pSound->SetExternalSoundPlayer(NULL);
}

BasicSound* ExternalSoundPlayer::GetLowestPrioritySound() {
    int lowestPrio = BasicSound::PRIORITY_MAX + 1;
    BasicSound* pLowest = NULL;

    for (BasicSoundExtPlayList::Iterator it = mSoundList.GetBeginIter();
         it != mSoundList.GetEndIter(); ++it) {

        int priority = it->CalcCurrentPlayerPriority();

        if (lowestPrio > priority) {
            pLowest = &*it;
            lowestPrio = priority;
        }
    }

    return pLowest;
}

} // namespace detail
} // namespace snd
} // namespace nw4r

// LLM-HARNESS-BEGIN: us-80417ac4
extern "C" void AppendSound__Q44nw4r3snd6detail19ExternalSoundPlayerFPQ44nw4r3snd6detail10BasicSound() {}
// LLM-HARNESS-END: us-80417ac4
// LLM-HARNESS-BEGIN: us-80417c64
extern "C" void RemoveSound__Q44nw4r3snd6detail19ExternalSoundPlayerFPQ44nw4r3snd6detail10BasicSound() {}
// LLM-HARNESS-END: us-80417c64
// LLM-HARNESS-BEGIN: us-80417cac
extern "C" void detail_CanPlaySound__Q44nw4r3snd6detail19ExternalSoundPlayerFi(int) {}
// LLM-HARNESS-END: us-80417cac
