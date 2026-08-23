#define private public
#include <nw4r/snd.h>
#include <nw4r/ut.h>
#undef private

namespace nw4r {
namespace snd {
namespace detail {

extern "C" nw4r::ut::detail::RuntimeTypeInfo lbl_eu_80665538(&lbl_eu_806654D0);

// A WSD file image loaded into memory. Typedef of 'const void' so signatures
// spell out a named type while still mangling as PCv (const void*).
typedef const void WsdData;

// Retail WaveSound::Prepare(const void*, s32, StartOffsetType, s32,
// const WsdCallback*, u32) has NO explicit voice budget - the sound's own
// GetVoiceOutCount() supplies the player's voice budget. The locked header
// snd_WaveSound.h (outside this session's writable scope) only declares a
// 7-parameter overload with an 'int voices' parameter, and MWCC cannot emit
// a member without an in-class declaration, so per MWCC_PATTERNS.md
// "Explicit retail-name entry points" the retail-named symbol is defined
// here with C linkage.
extern"C" bool Prepare__Q44nw4r3snd6detail9WaveSoundFPCvlQ54nw4r3snd6detail9WsdPlayer15StartOffsetTypelPCQ54nw4r3snd6detail9WsdPlayer11WsdCallbackUl(
    WaveSound* self, WsdData* pWsdData, s32 wsdOffset,
    WsdPlayer::StartOffsetType startType, s32 startOffset,
    const WsdPlayer::WsdCallback* pCallback, u32 callbackArg) {
    self->InitParam();

    if (!self->mWsdPlayer.Prepare(pWsdData, wsdOffset, startType, startOffset,
                                  self->GetVoiceOutCount(), pCallback,
                                  callbackArg)) {
        return false;
    }

    self->mPreparedFlag = true;
    return true;
}

WaveSound::WaveSound(SoundInstanceManager<WaveSound>* pManager, int priority, int arg)
    : BasicSound(priority, arg),
      mManager(pManager),
      mTempSpecialHandle(NULL),
      mPreparedFlag(false) {}

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

// Re-insert this sound into its instance manager's priority list, keeping
// the list sorted by current player priority.
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
