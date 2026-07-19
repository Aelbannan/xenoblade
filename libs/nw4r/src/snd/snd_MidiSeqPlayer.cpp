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
extern "C" void OnUpdateFrameSoundThread__Q44nw4r3snd6detail9SeqPlayerFv() {}
// LLM-HARNESS-END: us-80419900
// LLM-HARNESS-BEGIN: us-80419904
extern "C" void OnShutdownSoundThread__Q44nw4r3snd6detail9SeqPlayerFv() {}
// LLM-HARNESS-END: us-80419904
// LLM-HARNESS-BEGIN: us-80419914
extern "C" void InvalidateWaveData__Q44nw4r3snd6detail9SeqPlayerFPCvPCv() {}
// LLM-HARNESS-END: us-80419914
// LLM-HARNESS-BEGIN: us-80419918
extern "C" void OnUpdateVoiceSoundThread__Q54nw4r3snd6detail11SoundThread14PlayerCallbackFv() {}
// LLM-HARNESS-END: us-80419918
// LLM-HARNESS-BEGIN: us-8041991c
extern "C" bool IsPause__Q44nw4r3snd6detail9SeqPlayerCFv() { return false; }
// LLM-HARNESS-END: us-8041991c
// LLM-HARNESS-BEGIN: us-80419924
extern "C" bool IsStarted__Q44nw4r3snd6detail9SeqPlayerCFv() { return false; }
// LLM-HARNESS-END: us-80419924
// LLM-HARNESS-BEGIN: us-8041992c
extern "C" bool IsActive__Q44nw4r3snd6detail9SeqPlayerCFv() { return false; }
// LLM-HARNESS-END: us-8041992c
