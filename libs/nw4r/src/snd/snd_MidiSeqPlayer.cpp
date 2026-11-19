#include <nw4r/snd.h>
#include <nw4r/ut.h>

using nw4r::snd::detail::SeqPlayer;

void Update__Q44nw4r3snd6detail9SeqPlayerFv(void);

extern "C" void OnUpdateFrameSoundThread__Q44nw4r3snd6detail9SeqPlayerFv(void) {
    Update__Q44nw4r3snd6detail9SeqPlayerFv();
}
void SeqPlayer::OnShutdownSoundThread() { Stop(); }
extern "C" void InvalidateWaveData__Q44nw4r3snd6detail9SeqPlayerFPCvPCv(void) {
}
extern "C" void OnUpdateVoiceSoundThread__Q54nw4r3snd6detail11SoundThread14PlayerCallbackFv(void) {
}
extern "C" u8 IsPause__Q44nw4r3snd6detail9SeqPlayerCFv(SeqPlayer* self) {
    return self->SeqPlayer::IsPause();
}
extern "C" u8 IsStarted__Q44nw4r3snd6detail9SeqPlayerCFv(SeqPlayer* self) {
    return self->SeqPlayer::IsStarted();
}
extern "C" u8 IsActive__Q44nw4r3snd6detail9SeqPlayerCFv(SeqPlayer* self) {
    return self->SeqPlayer::IsActive();
}
