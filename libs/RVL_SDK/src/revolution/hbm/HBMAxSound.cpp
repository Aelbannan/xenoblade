// RVL_SDK/src/revolution/hbm/HBMAxSound

#include <harness_catalog.h>

#include <revolution/AX.h>
#include <revolution/OS.h>

namespace homebutton {

void PlaySeq(int /* seqId */) {}

void InitAxSound(const void* /* pWork */, void* /* pWorkEnd */,
                 u32 /* workSize */) {}

void ShutdownAxSound() {}

void AxSoundMain() {}

void StopAllSeq() {}

void SetVolumeAllSeq(float /* volume */) {}

void SetSoundMode(u32 /* mode */) {}

} // namespace homebutton

// --- hard-symbol stubs (scaffold_hard_symbols) ---
namespace {

/******************************************************************************
 *
 * sWork (retail .bss 0x805DA058)
 *
 ******************************************************************************/
struct HBMWork {
    u8 pad0[0x14348];              // 0x00000
    AXOutCallback prevFrameCb;     // 0x14348 (previous AX callback, chained)
    u8 pad1[0x14680 - 0x1434C];    // 0x1434C
    OSMessageQueue msgQueue;       // 0x14680
};

HBMWork* sWork;

void GetFreePlayer(int /* soundId */) {}

void AudioFrameCallback() {
    if (sWork != NULL) {
        OSSendMessage(&sWork->msgQueue, reinterpret_cast<OSMessage>(1), 0);

        if (sWork->prevFrameCb != NULL) {
            sWork->prevFrameCb();
        }
    }
}

void AudioSoundThreadProc(void* /* arg */) {}

} // namespace

// Force-emit the anonymous-namespace helpers (retail references them from
// InitAxSound via AXRegisterCallback / OSCreateThread / PlaySeq).
DECOMP_FORCEACTIVE(HBMAxSound_cpp, GetFreePlayer, AudioFrameCallback,
                   AudioSoundThreadProc);
