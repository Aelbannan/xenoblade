#include <nw4r/snd.h>

namespace nw4r {
namespace snd {

SoundStartable::StartResult SoundStartable::detail_StartSound(
    SoundHandle* pHandle, u32 id, const StartInfo* pStartInfo) {

    // Retail signature is the slim 3-arg form (retail 0x80423324).
    StartResult result =
        detail_SetupSound(pHandle, id, false, pStartInfo);

    if (result != START_SUCCESS) {
        return result;
    }

    pHandle->StartPrepared();
    return START_SUCCESS;
}

} // namespace snd
} // namespace nw4r
