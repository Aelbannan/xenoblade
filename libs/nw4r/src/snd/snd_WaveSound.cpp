#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

extern "C" nw4r::ut::detail::RuntimeTypeInfo lbl_eu_80665538(&lbl_eu_806654D0);

WaveSound::WaveSound(SoundInstanceManager<WaveSound>* pManager, int priority, int arg)
    : BasicSound(priority, arg),
      mManager(pManager),
      mTempSpecialHandle(NULL),
      mPreparedFlag(false) {}

// Retail passes this sound's own voice-out count as the player's voice
// budget rather than taking a voices parameter.
bool WaveSound::Prepare(const void* pWsdData, s32 wsdOffset,
                        WsdPlayer::StartOffsetType startType, s32 startOffset,
                        int voices, const WsdPlayer::WsdCallback* pCallback,
                        u32 callbackArg) {
    InitParam();

    if (!mWsdPlayer.Prepare(pWsdData, wsdOffset, startType, startOffset,
                            GetVoiceOutCount(), pCallback, callbackArg)) {
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

// Re-insert this sound into its instance manager's priority list, keeping the
// list sorted by current player priority (mPriority + mUnk0x50, clamped to
// [0, PRIORITY_MAX]).
void WaveSound::OnUpdatePlayerPriority() {
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
namespace nw4r {
namespace snd {
namespace detail {

const nw4r::ut::detail::RuntimeTypeInfo* WaveSound::GetRuntimeTypeInfo() const {
    return &lbl_eu_80665538;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
