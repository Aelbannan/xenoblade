#include <nw4r/snd.h>
#include <nw4r/ut.h>

// Retail .sdata2 pool (US) - see the sibling decls in snd_StrmPlayer.cpp.

namespace nw4r {
namespace snd {
namespace detail {

NW4R_UT_RTTI_DEF_DERIVED(StrmSound, BasicSound);

// Retail three-argument constructor (manager*, priority, ambientPriority).
StrmSound::StrmSound(SoundInstanceManager<StrmSound>* pManager, int priority,
                     int ambientPriority)
    : BasicSound(priority, ambientPriority), mTempSpecialHandle(NULL),
      mManager(pManager) {
    // Each track's move value starts settled at 1.0f.
    // NOTE: retail expands the tail-fill into its generic two-path struct-fill
    // idiom (hoisted guard over [&v1,&v8), fast 8-wide loop + remainder loop
    // with overlap checks). Only this form - explicit element-0 InitValue plus
    // a pointer-walk loop calling InitValue - makes MWCC fire that expansion;
    // index loops, local-template copies and memcpy all stay compact.
    MoveValue<f32, int>* pBase = mMoveValue;
    MoveValue<f32, int>* p = pBase + 1;
    MoveValue<f32, int>* pEnd = pBase + 8;
    pBase->InitValue(0.0f);
    for (; p < pEnd; ++p) {
        p->InitValue(0.0f);
    }
}

StrmSound::StrmSound(SoundInstanceManager<StrmSound>* pManager)
    : BasicSound(0, 0), mManager(pManager), mTempSpecialHandle(NULL) {}

StrmSound::~StrmSound() {}

void StrmSound::InitParam() {
    BasicSound::InitParam();

    // Each track's move value is first settled at 1.0f (InitValue), then a
    // 1-frame move toward 1.0f is started (SetTarget); SetTarget
    // re-derives the origin from GetValue() (1.0f here since
    // counter==frame==0).
    for (int i = 0; i < 8; i++) {
        mMoveValue[i].InitValue(0.0f);
        mMoveValue[i].SetTarget(1.0f, 1);
    }
}

int StrmSound::Setup(StrmBufferPool* pPool, int voices, u16 unk) {
    InitParam();
    return mStrmPlayer.Setup(pPool, voices, unk, GetVoiceOutCount());
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

// Retail SetTrackVolume(unsigned long, float) is a StrmPlayer member the stale
// header does not declare; snd_StrmPlayer.cpp defines the retail mangled
// symbol verbatim as a free function (same ABI: r3 = this, r4 = track,
// f1 = volume). Declared C-linkage here so the call-site reloc carries the
// exact retail symbol name.
extern "C" void SetTrackVolume__Q44nw4r3snd6detail10StrmPlayerFUlf(
    nw4r::snd::detail::StrmPlayer* pStrmPlayer, unsigned long track, f32 volume);

namespace nw4r {
namespace snd {
namespace detail {

void StrmSound::UpdateParam() {
    BasicSound::UpdateParam();

    for (int i = 0; i < 8; i++) {
        if (mStrmPlayer.GetPlayerTrack(i) == NULL) {
            continue;
        }

        // Arguments evaluate left-to-right: the track flag is computed before
        // the move-value interpolation, matching retail's schedule.
        SetTrackVolume__Q44nw4r3snd6detail10StrmPlayerFUlf(
            &mStrmPlayer, 1UL << i, mMoveValue[i].GetValue());
    }
}

void StrmSound::OnUpdatePlayerPriority() {
    mManager->UpdatePriority(this, this->CalcCurrentPlayerPriority());
}

const nw4r::ut::detail::RuntimeTypeInfo* StrmSound::GetRuntimeTypeInfo() const {
    return &typeInfo;
}

} // namespace detail
} // namespace snd
} // namespace nw4r

// ABSORB pad - make raw BSS/SBSS sizes match retail
unsigned char absorb_pad_snd_StrmSound_sbss_4[0x4];
