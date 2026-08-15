#include <nw4r/snd.h>
#include <nw4r/ut.h>

// Retail .sdata2 pool (US) - see the sibling decls in snd_StrmPlayer.cpp.
extern const f32 lbl_eu_8066A088; // 1.0f
extern const f32 lbl_eu_8066A08C; // move-value target set by InitParam
extern const f64 lbl_eu_8066A090; // signed int->float magic (2^52)

// Retail SetTrackVolume(unsigned long, float) is a StrmPlayer member the
// stale header does not declare; snd_StrmPlayer.cpp defines the retail mangled
// symbol as a free function (same ABI: r3 = this, r4 = track, f1 = volume).
void SetTrackVolume__Q44nw4r3snd6detail10StrmPlayerFUlf(
    nw4r::snd::detail::StrmPlayer* pStrmPlayer, unsigned long track, f32 volume);

namespace nw4r {
namespace snd {
namespace detail {

extern "C" nw4r::ut::detail::RuntimeTypeInfo lbl_eu_80665520(&lbl_eu_806654D0);

namespace {

// Retail StrmSound layout mirrors (the snd_StrmSound.h member map is stale for
// these regions: mStrmPlayer is really at +0x10C, and the per-track move-value
// blocks at +0xE4C have no header members). Same overlay approach as
// UpdateMoveValue below.
struct MoveValueRetail {
    f32 origin;  // +0x0
    f32 target;  // +0x4
    s32 frame;   // +0x8
    s32 counter; // +0xC
};

struct StrmSoundRetail {
    u8 _pad0[0x10C];       // BasicSound (0x000..0x10C)
    u8 _strmPlayer[0xD38]; // StrmPlayer (0x10C..0xE44)
    StrmSoundHandle* mTempSpecialHandle;       // 0xE44
    SoundInstanceManager<StrmSound>* mManager; // 0xE48
    MoveValueRetail moveValue[8];              // 0xE4C (8 * 0x10 = 0x80)
};

// Signed int -> float through the shared 2^52 magic double, referenced by
// name so the .sdata2 reloc matches retail (see snd_StrmPlayer.cpp F64Conv).
inline f32 S32ToF32(s32 value) {
    union {
        f64 d;
        u32 w[2];
    } conv;
    conv.w[0] = 0x43300000;
    conv.w[1] = (u32)value ^ 0x80000000;
    return (f32)(conv.d - lbl_eu_8066A090);
}

// MoveValue<f32,int>::GetValue() expressed through S32ToF32 (keeps the retail
// .sdata2 reloc): counter < frame ? interpolate : target.
inline f32 GetMoveValue(const MoveValueRetail& mv) {
    if (mv.counter < mv.frame) {
        return mv.origin + S32ToF32(mv.counter) * (mv.target - mv.origin) /
                               S32ToF32(mv.frame);
    }
    return mv.target;
}

} // namespace

StrmSound::StrmSound(SoundInstanceManager<StrmSound>* pManager)
    : BasicSound(0, 0), mManager(pManager), mTempSpecialHandle(NULL) {}

StrmSound::~StrmSound() {}

void StrmSound::InitParam() {
    BasicSound::InitParam();

    // Each track's move value is first settled at 1.0f (InitValue), then a
    // 1-frame move toward lbl_eu_8066A08C is started (SetTarget), with the
    // origin re-derived from GetValue() (1.0f here since counter==frame==0).
    StrmSoundRetail* self = reinterpret_cast<StrmSoundRetail*>(this);
    for (int i = 0; i < 8; i++) {
        self->moveValue[i].origin = lbl_eu_8066A088;
        self->moveValue[i].target = lbl_eu_8066A088;
        self->moveValue[i].frame = 0;
        self->moveValue[i].counter = 0;

        self->moveValue[i].origin = GetMoveValue(self->moveValue[i]);
        self->moveValue[i].target = lbl_eu_8066A08C;
        self->moveValue[i].frame = 1;
        self->moveValue[i].counter = 0;
    }
}

bool StrmSound::Setup(StrmBufferPool* pPool, int voices, u16 unk) {
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

namespace nw4r {
namespace snd {
namespace detail {

// Retail UpdateParam is a StrmSound virtual, but the stale header does not
// declare it; the body is emitted under the retail mangled name as a free
// function (same ABI: r3 = this), like SetTrackVolume in snd_StrmPlayer.cpp.
void UpdateParam__Q44nw4r3snd6detail9StrmSoundFv(StrmSound* pSound) {
    pSound->BasicSound::UpdateParam();

    StrmSoundRetail* self = reinterpret_cast<StrmSoundRetail*>(pSound);
    StrmPlayer* player = reinterpret_cast<StrmPlayer*>(self->_strmPlayer);

    for (int i = 0; i < 8; i++) {
        if (player->GetPlayerTrack(i) == NULL) {
            continue;
        }

        f32 volume = GetMoveValue(self->moveValue[i]);
        SetTrackVolume__Q44nw4r3snd6detail10StrmPlayerFUlf(player, 1u << i,
                                                           volume);
    }
}

// See UpdateParam above: OnUpdatePlayerPriority is likewise undeclared in the
// stale header, so it is emitted under the retail mangled name.
void OnUpdatePlayerPriority__Q44nw4r3snd6detail9StrmSoundFv(StrmSound* pSound) {
    StrmSoundRetail* self = reinterpret_cast<StrmSoundRetail*>(pSound);
    self->mManager->UpdatePriority(pSound, pSound->CalcCurrentPlayerPriority());
}

const nw4r::ut::detail::RuntimeTypeInfo* StrmSound::GetRuntimeTypeInfo() const {
    return &lbl_eu_80665520;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
