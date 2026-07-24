#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

NW4R_UT_RTTI_DEF_DERIVED(StrmSound, BasicSound);

StrmSound::StrmSound(SoundInstanceManager<StrmSound>* pManager)
    : BasicSound(0, 0), mManager(pManager), mTempSpecialHandle(NULL) {}

bool StrmSound::Prepare(StrmBufferPool* pPool,
                        StrmPlayer::StartOffsetType offsetType, s32 offset,
                        int voices, ut::FileStream* pStream) {
    if (pPool == NULL) {
        return false;
    }

    InitParam();

    if (!mStrmPlayer.Setup(pPool)) {
        return false;
    }

    if (!mStrmPlayer.Prepare(pStream, voices, offsetType, offset)) {
        mStrmPlayer.Shutdown();
        return false;
    }

    return true;
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

// LLM-HARNESS-BEGIN: us-8042751c
extern "C" void InitParam__Q44nw4r3snd6detail9StrmSoundFv() {}
// LLM-HARNESS-END: us-8042751c
// LLM-HARNESS-BEGIN: us-80427650
extern "C" void Setup__Q44nw4r3snd6detail9StrmSoundFPQ44nw4r3snd6detail14StrmBufferPooliUs() {}
// LLM-HARNESS-END: us-80427650
// LLM-HARNESS-BEGIN: us-8042772c
extern "C" void UpdateMoveValue__Q44nw4r3snd6detail9StrmSoundFv() {}
// LLM-HARNESS-END: us-8042772c
// LLM-HARNESS-BEGIN: us-804277ac
extern "C" void UpdateParam__Q44nw4r3snd6detail9StrmSoundFv() {}
// LLM-HARNESS-END: us-804277ac
// LLM-HARNESS-BEGIN: us-80427984
extern "C" void OnUpdatePlayerPriority__Q44nw4r3snd6detail9StrmSoundFv() {}
// LLM-HARNESS-END: us-80427984
// LLM-HARNESS-BEGIN: us-80427a84
extern "C" void* GetBasicPlayer__Q44nw4r3snd6detail9StrmSoundFv(void* self) { return (void*)((u8*)self + 0x10c); }
// LLM-HARNESS-END: us-80427a84
// LLM-HARNESS-BEGIN: us-80427a8c
extern "C" void* GetBasicPlayer__Q44nw4r3snd6detail9StrmSoundCFv(void* self) { return (void*)((u8*)self + 0x10c); }
// LLM-HARNESS-END: us-80427a8c
// LLM-HARNESS-BEGIN: us-80427a94
extern "C" u8 IsPrepared__Q44nw4r3snd6detail9StrmSoundCFv(void* self) { return ((u8*)self)[0x22f]; }
// LLM-HARNESS-END: us-80427a94
// LLM-HARNESS-BEGIN: us-80427a9c
extern "C" int GetRuntimeTypeInfo__Q44nw4r3snd6detail9StrmSoundCFv(void) { return lbl_eu_80665520@sda21; }
// LLM-HARNESS-END: us-80427a9c
