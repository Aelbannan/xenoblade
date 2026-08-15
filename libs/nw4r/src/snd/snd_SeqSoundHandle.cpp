#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace snd {

// Retail emits only DetachSound in this TU; the ctor is inlined at call sites
// (no retail symbol exists), so mark it inline to avoid a standalone 0x148
// emission that blows the split budget (0x38).
inline SeqSoundHandle::SeqSoundHandle(SoundHandle* pHandle) : mSound(NULL) {
    if (pHandle == NULL) {
        return;
    }

    if (!pHandle->IsAttachedSound()) {
        return;
    }

    detail::SeqSound* pSound =
        ut::DynamicCast<detail::SeqSound*>(pHandle->detail_GetAttachedSound());

    if (pSound != NULL) {
        mSound = pSound;

        if (mSound->IsAttachedTempGeneralHandle()) {
            mSound->DetachTempGeneralHandle();
        }

        if (mSound->IsAttachedTempSpecialHandle()) {
            mSound->DetachTempSpecialHandle();
        }

        mSound->mTempSpecialHandle = this;
    }
}

void SeqSoundHandle::DetachSound() {
    if (IsAttachedSound()) {
        if (mSound->mTempSpecialHandle == this) {
            mSound->mTempSpecialHandle = NULL;
        }
    }

    if (mSound != NULL) {
        mSound = NULL;
    }
}

} // namespace snd
} // namespace nw4r
