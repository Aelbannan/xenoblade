#include <nw4r/snd.h>
#include <nw4r/ut.h>

using nw4r::snd::detail::SeqPlayer;

void SeqPlayer::OnUpdateFrameSoundThread() {
    Update();
}
void SeqPlayer::OnShutdownSoundThread() { Stop(); }
void SeqPlayer::InvalidateWaveData(const void*, const void*) {}
extern "C" void OnUpdateVoiceSoundThread__Q54nw4r3snd6detail11SoundThread14PlayerCallbackFv(void) {
}
bool SeqPlayer::IsPause() const {
    return mPauseFlag;
}
bool SeqPlayer::IsStarted() const {
    return mStartedFlag;
}
bool SeqPlayer::IsActive() const {
    return mActiveFlag;
}
