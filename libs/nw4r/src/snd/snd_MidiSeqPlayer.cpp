#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

/**
 * Dummy class to instantiate necessary weak functions
 */
#if !defined(NONMATCHING)
class MidiSeqPlayer : public SeqPlayer {
private:
    MidiSeqPlayer();

    // Dummy implementation, must prevent instantiating SeqPlayer version
    virtual void ChannelCallback(Channel* /* pChannel */) {}
};

MidiSeqPlayer::MidiSeqPlayer() {}
#endif

} // namespace detail
} // namespace snd
} // namespace nw4r

// LLM-HARNESS-BEGIN: us-80419900
extern "C" void OnUpdateFrameSoundThread__Q44nw4r3snd6detail9SeqPlayerFv(void) {
    Update__Q44nw4r3snd6detail9SeqPlayerFv();
}
// LLM-HARNESS-END: us-80419900
// LLM-HARNESS-BEGIN: us-80419904
extern "C" void OnShutdownSoundThread__Q44nw4r3snd6detail9SeqPlayerFv() {}
// LLM-HARNESS-END: us-80419904
// LLM-HARNESS-BEGIN: us-80419914
extern "C" void InvalidateWaveData__Q44nw4r3snd6detail9SeqPlayerFPCvPCv(void) {
}
// LLM-HARNESS-END: us-80419914
// LLM-HARNESS-BEGIN: us-80419918
extern "C" void OnUpdateVoiceSoundThread__Q54nw4r3snd6detail11SoundThread14PlayerCallbackFv(void) {
}
// LLM-HARNESS-END: us-80419918
// LLM-HARNESS-BEGIN: us-8041991c
extern "C" u8 IsPause__Q44nw4r3snd6detail9SeqPlayerCFv(void* self) { return ((u8*)self)[0xee]; }
// LLM-HARNESS-END: us-8041991c
// LLM-HARNESS-BEGIN: us-80419924
extern "C" u8 IsStarted__Q44nw4r3snd6detail9SeqPlayerCFv(void* self) { return ((u8*)self)[0xed]; }
// LLM-HARNESS-END: us-80419924
// LLM-HARNESS-BEGIN: us-8041992c
extern "C" u8 IsActive__Q44nw4r3snd6detail9SeqPlayerCFv(void* self) { return ((u8*)self)[0xec]; }
// LLM-HARNESS-END: us-8041992c
