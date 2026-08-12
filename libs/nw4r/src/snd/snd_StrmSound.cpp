#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

extern "C" nw4r::ut::detail::RuntimeTypeInfo lbl_eu_80665520(&lbl_eu_806654D0);

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

    // The shared snd_StrmPlayer.h layout is stale (see the retail-layout
    // mirror note in snd_StrmPlayer.cpp): mMoveBlocks sits at this+0xE54
    // with 0x10-byte elements { limit; count; } in retail, so access it
    // through a retail-layout overlay instead of the header member.
    struct MoveBlocksRetail {
        u8 _pad[0xE54];
        struct MoveBlock {
            s32 limit;
            s32 count;
            u8 _pad2[8];
        } blocks[8];
    };
    MoveBlocksRetail* mb = reinterpret_cast<MoveBlocksRetail*>(this);
    for (int i = 0; i < 8; i++) {
        if (!mStrmPlayer.GetPlayerTrack(i)) continue;

        if (mb->blocks[i].count < mb->blocks[i].limit)
            mb->blocks[i].count++;
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

const nw4r::ut::detail::RuntimeTypeInfo* StrmSound::GetRuntimeTypeInfo() const {
    return &lbl_eu_80665520;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
