#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

NW4R_UT_RTTI_DEF_DERIVED(StrmSound, BasicSound);

StrmSound::StrmSound(SoundInstanceManager<StrmSound>* pManager)
    : BasicSound(0, 0), mManager(pManager), mTempSpecialHandle(NULL) {}

StrmSound::~StrmSound() {}

void StrmSound::InitParam() {
    BasicSound::InitParam();
}

bool StrmSound::Setup(StrmBufferPool* pPool, int voices, u16 unk) {
    InitParam();
    return mStrmPlayer.Setup(pPool, GetVoiceOutCount(), unk, 0);
}

bool StrmSound::Prepare(StrmPlayer::StartOffsetType offsetType, s32 offset,
                        ut::FileStream* pStream) {
    if (!mStrmPlayer.Prepare(pStream, offsetType, offset)) {
        mStrmPlayer.Shutdown();
        return false;
    }
    return true;
}

void StrmSound::UpdateMoveValue() {
    BasicSound::UpdateMoveValue();
    
    for (int i = 0; i < 8; i++) {
        if (!mStrmPlayer.GetPlayerTrack(i)) continue;
        
        if (mStrmPlayer.mMoveBlocks[i].count < mStrmPlayer.mMoveBlocks[i].limit)
            mStrmPlayer.mMoveBlocks[i].count++;
    }
}

void StrmSound::Shutdown() {
    BasicSound::Shutdown();
    mManager->Free(this);
}

void StrmSound::SetPlayerPriority(int priority) {
    BasicSound::SetPlayerPriority(priority);
    mManager->UpdatePriority(this, CalcCurrentPlayerPriority());
}

bool StrmSound::IsAttachedTempSpecialHandle() {
    return mTempSpecialHandle != NULL;
}

void StrmSound::DetachTempSpecialHandle() {
    mTempSpecialHandle->DetachSound();
}

} // namespace detail
} // namespace snd
} // namespace nw4r

void UpdateParam__Q44nw4r3snd6detail9StrmSoundFv(){}
void OnUpdatePlayerPriority__Q44nw4r3snd6detail9StrmSoundFv(){}
namespace nw4r {
namespace snd {
namespace detail {

extern "C" nw4r::ut::detail::RuntimeTypeInfo lbl_eu_80665520;

const nw4r::ut::detail::RuntimeTypeInfo* StrmSound::GetRuntimeTypeInfo() const {
    return &lbl_eu_80665520;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
