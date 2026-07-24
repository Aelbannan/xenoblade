#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

NW4R_UT_RTTI_DEF_DERIVED(WaveSound, BasicSound);

WaveSound::WaveSound(SoundInstanceManager<WaveSound>* pManager)
    : BasicSound(0, 0),
      mManager(pManager),
      mTempSpecialHandle(NULL),
      mPreparedFlag(false) {}

bool WaveSound::Prepare(const void* pWsdData, s32 wsdOffset,
                        WsdPlayer::StartOffsetType startType, s32 startOffset,
                        int voices, const WsdPlayer::WsdCallback* pCallback,
                        u32 callbackArg) {
    InitParam();

    if (!mWsdPlayer.Prepare(pWsdData, wsdOffset, startType, startOffset, voices,
                            pCallback, callbackArg)) {
        return false;
    }

    mPreparedFlag = true;
    return true;
}

void WaveSound::Shutdown() {
    BasicSound::Shutdown();
    mManager->Free(this);
}

void WaveSound::SetChannelPriority(int priority) {
    mWsdPlayer.SetChannelPriority(priority);
}

void WaveSound::SetReleasePriorityFix(bool flag) {
    mWsdPlayer.SetReleasePriorityFix(flag);
}

void WaveSound::SetPlayerPriority(int priority) {
    BasicSound::SetPlayerPriority(priority);
    mManager->UpdatePriority(this, CalcCurrentPlayerPriority());
}

bool WaveSound::IsAttachedTempSpecialHandle() {
    return mTempSpecialHandle != NULL;
}

void WaveSound::DetachTempSpecialHandle() {
    mTempSpecialHandle->DetachSound();
}

} // namespace detail
} // namespace snd
} // namespace nw4r

// LLM-HARNESS-BEGIN: us-8042c170
extern "C" void OnUpdatePlayerPriority__Q44nw4r3snd6detail9WaveSoundFv() {}
// LLM-HARNESS-END: us-8042c170
// LLM-HARNESS-BEGIN: us-8042c270
extern "C" void* GetBasicPlayer__Q44nw4r3snd6detail9WaveSoundFv(void* self) { return (void*)((u8*)self + 0x10c); }
// LLM-HARNESS-END: us-8042c270
// LLM-HARNESS-BEGIN: us-8042c278
extern "C" void* GetBasicPlayer__Q44nw4r3snd6detail9WaveSoundCFv(void* self) { return (void*)((u8*)self + 0x10c); }
// LLM-HARNESS-END: us-8042c278
// LLM-HARNESS-BEGIN: us-8042c280
extern "C" u8 IsPrepared__Q44nw4r3snd6detail9WaveSoundCFv(void* self) { return ((u8*)self)[0x24c]; }
// LLM-HARNESS-END: us-8042c280
// LLM-HARNESS-BEGIN: us-8042c288
extern "C" int GetRuntimeTypeInfo__Q44nw4r3snd6detail9WaveSoundCFv(void) { return 0; }
// LLM-HARNESS-END: us-8042c288
