#include <nw4r/snd.h>
#include <nw4r/ut.h>

// Retail .sdata2 pool (US) - see the sibling decls in snd_StrmPlayer.cpp.
extern const f32 lbl_eu_8066A088; // 1.0f
extern const f32 lbl_eu_8066A08C; // move-value target set by InitParam

namespace nw4r {
namespace snd {
namespace detail {

extern "C" nw4r::ut::detail::RuntimeTypeInfo lbl_eu_80665520(&lbl_eu_806654D0);

namespace {

// Retail StrmSound layout mirrors (the snd_StrmSound.h member map is stale for
// these regions: mStrmPlayer is really at +0x10C, and the per-track move-value
// blocks at +0xE4C have no header members). Same overlay approach as
// UpdateMoveValue below.
struct StrmSoundRetail {
    u8 _pad0[0x10C];       // BasicSound (0x000..0x10C)
    u8 _strmPlayer[0xD38]; // StrmPlayer (0x10C..0xE44)
    StrmSoundHandle* mTempSpecialHandle;       // 0xE44
    SoundInstanceManager<StrmSound>* mManager; // 0xE48
    // MoveValue<f32,int> layout matches the header template (origin/target/
    // frame/counter), so use the real template: its inlined GetValue()
    // reproduces retail's hoisted 0x4330/2^52 int->float conversion exactly.
    // NOTE: the hoisted magic-double load carries an anonymous MWCC pool label
    // instead of retail's lbl_eu_8066A090 - needs a reloc-name map entry.
    MoveValue<f32, int> moveValue[8];          // 0xE4C (8 * 0x10 = 0x80)
};

} // namespace

// Local vtable emitted by this TU (retail lbl_eu_8056ACF0); the ctor stores
// it explicitly because it is constructed as a free-standing placement object.
extern "C" const u8 __vt__Q44nw4r3snd6detail9StrmSound[];

// Base/member constructors are invoked by their retail symbol names:
// BasicSound is abstract (pure virtual IsPrepared), so MWCC cannot be asked
// to placement-construct either subobject from here.
extern "C" void __ct__Q44nw4r3snd6detail10BasicSoundFii(
    nw4r::snd::detail::BasicSound* self, int priority, int ambientPriority);
extern "C" void __ct__Q44nw4r3snd6detail10StrmPlayerFv(
    nw4r::snd::detail::StrmPlayer* self);

// Retail three-argument constructor (manager*, priority, ambientPriority).
// The locked snd_StrmSound.h cannot gain the declaration, so the body is
// emitted under the retail mangled name, like UpdateParam further below.
// Retail order: BasicSound(priority, ambientPriority), derived vptr store,
// StrmPlayer member ctor (+0x10C), then temp-handle/manager/move-values.
extern "C" nw4r::snd::detail::StrmSound*
__ct__Q44nw4r3snd6detail9StrmSoundFPQ44nw4r3snd6detail50SoundInstanceManagerIQ44nw4r3snd6detail9StrmSoundEii(
    nw4r::snd::detail::StrmSound* self,
    nw4r::snd::detail::SoundInstanceManager<nw4r::snd::detail::StrmSound>*
        pManager,
    int priority, int ambientPriority) {
    // Declared in retail save order: 'this' colors to r31, manager to r30.
    nw4r::snd::detail::StrmSound* const pSound = self;
    nw4r::snd::detail::SoundInstanceManager<nw4r::snd::detail::StrmSound>*
        const pMgr = pManager;

    __ct__Q44nw4r3snd6detail10BasicSoundFii(pSound, priority,
                                            ambientPriority);

    *reinterpret_cast<const u8**>(pSound) =
        __vt__Q44nw4r3snd6detail9StrmSound;

    StrmSoundRetail* sv = reinterpret_cast<StrmSoundRetail*>(pSound);
    __ct__Q44nw4r3snd6detail10StrmPlayerFv(
        reinterpret_cast<StrmPlayer*>(sv->_strmPlayer));

    sv->mTempSpecialHandle = NULL;
    sv->mManager = pMgr;

    // Each track's move value starts settled at 1.0f.
    // NOTE: retail expands the tail-fill into its generic two-path struct-fill
    // idiom (hoisted guard over [&v1,&v8), fast 8-wide loop + remainder loop
    // with overlap checks). Only this form - explicit element-0 InitValue plus
    // a pointer-walk loop calling InitValue - makes MWCC fire that expansion;
    // index loops, local-template copies and memcpy all stay compact.
    MoveValue<f32, int>* pBase = sv->moveValue;
    MoveValue<f32, int>* p = pBase + 1;
    MoveValue<f32, int>* pEnd = pBase + 8;
    pBase->InitValue(lbl_eu_8066A088);
    for (; p < pEnd; ++p) {
        p->InitValue(lbl_eu_8066A088);
    }

    return pSound;
}

// NOTE: this definition must stay the FIRST function in this TU - MWCC emits
// functions in definition order and the diff tooling pairs the retail
// constructor by its leading .text offset.

StrmSound::StrmSound(SoundInstanceManager<StrmSound>* pManager)
    : BasicSound(0, 0), mManager(pManager), mTempSpecialHandle(NULL) {}

StrmSound::~StrmSound() {}

void StrmSound::InitParam() {
    BasicSound::InitParam();

    // Each track's move value is first settled at 1.0f (InitValue), then a
    // 1-frame move toward lbl_eu_8066A08C is started (SetTarget); SetTarget
    // re-derives the origin from GetValue() (1.0f here since
    // counter==frame==0).
    StrmSoundRetail* self = reinterpret_cast<StrmSoundRetail*>(this);
    for (int i = 0; i < 8; i++) {
        self->moveValue[i].InitValue(lbl_eu_8066A088);
        self->moveValue[i].SetTarget(lbl_eu_8066A08C, 1);
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

// Retail UpdateParam is a StrmSound virtual, but the stale header does not
// declare it; the body is emitted under the retail mangled name as a free
// function (same ABI: r3 = this), like SetTrackVolume in snd_StrmPlayer.cpp.
void UpdateParam__Q44nw4r3snd6detail9StrmSoundFv(StrmSound* pSound) {
    pSound->BasicSound::UpdateParam();

    StrmSoundRetail* self = reinterpret_cast<StrmSoundRetail*>(pSound);
    StrmSoundRetail* track = self;

    for (int i = 0; i < 8; i++) {
        StrmPlayer* player =
            reinterpret_cast<StrmPlayer*>(self->_strmPlayer);

        if (player->GetPlayerTrack(i) == NULL) {
            continue;
        }

        // Arguments evaluate left-to-right: the track flag is computed before
        // the move-value interpolation, matching retail's schedule.
        SetTrackVolume__Q44nw4r3snd6detail10StrmPlayerFUlf(
            reinterpret_cast<StrmPlayer*>(self->_strmPlayer), 1UL << i,
            track->moveValue[i].GetValue());
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
