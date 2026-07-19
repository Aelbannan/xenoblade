#include <nw4r/snd.h>

namespace nw4r {
namespace snd {
namespace detail {

SeqTrack* MmlSeqTrackAllocator::AllocTrack(SeqPlayer* pPlayer) {
    MmlSeqTrack* pTrack = mTrackPool.Alloc();

    if (pTrack != NULL) {
        pTrack->SetSeqPlayer(pPlayer);
        pTrack->SetMmlParser(mParser);
    }

    return pTrack;
}

void MmlSeqTrackAllocator::FreeTrack(SeqTrack* pTrack) {
    pTrack->SetSeqPlayer(NULL);
    mTrackPool.Free(static_cast<MmlSeqTrack*>(pTrack));
}

u32 MmlSeqTrackAllocator::Create(void* pBuffer, u32 size) {
    return mTrackPool.Create(pBuffer, size);
}

void MmlSeqTrackAllocator::Destroy(void* pBuffer, u32 size) {
    mTrackPool.Destroy(pBuffer, size);
}

} // namespace detail
} // namespace snd
} // namespace nw4r

// LLM-HARNESS-BEGIN: us-8041ac84
extern "C" bool GetAllocatableTrackCount__Q44nw4r3snd6detail20MmlSeqTrackAllocatorCFv() { return false; }
// LLM-HARNESS-END: us-8041ac84
