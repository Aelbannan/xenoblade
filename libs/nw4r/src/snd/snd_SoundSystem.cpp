#include <nw4r/snd.h>

#include <revolution/AX.h>
#include <revolution/OS.h>
#include <revolution/SC.h>

namespace {

NW4R_LIB_VERSION(SND, "Jun  8 2007", "11:17:15", "0x4199_60831");

} // namespace

namespace nw4r {
namespace snd {

namespace detail {
inline void InitSeqPlayer() {
    InitSeqPlayer__Q44nw4r3snd6detail9SeqPlayerFv();
}
} // namespace detail

detail::TaskThread SoundSystem::sTaskThread;

void SoundSystem::InitSoundSystem(s32 soundThreadPriority,
                                  s32 dvdThreadPriority) {
    SoundSystemParam param;
    param.soundThreadPriority = soundThreadPriority;
    param.soundThreadStackSize = 0x4000;
    param.dvdThreadPriority = dvdThreadPriority;
    param.dvdThreadStackSize = 0x4000;
    InitSoundSystem(param, lbl_eu_80638C40, 0x16E00);
}

u32 SoundSystem::GetRequiredMemSize(const SoundSystemParam& rParam) {
    return rParam.soundThreadStackSize + rParam.dvdThreadStackSize +
           detail::AxVoiceManager::GetInstance().GetRequiredMemSize() +
           detail::VoiceManager::GetInstance().GetRequiredMemSize() +
           detail::ChannelManager::GetInstance().GetRequiredMemSize();
}

void SoundSystem::InitSoundSystem(const SoundSystemParam& rParam, void* pWork,
                                  u32 workSize) {
#pragma unused(workSize)

    if (lbl_eu_80665508) {
        return;
    }

    lbl_eu_80665508 = true;

    OSRegisterVersion(NW4R_SND_Version_);

    detail::AxManager::GetInstance().Init();

    SCInit();
    while (SCCheckStatus() == SC_STATUS_BUSY) {
        ;
    }

    switch (SCGetSoundMode()) {
    case SC_SND_MONO: {
        detail::AxManager::GetInstance().SetOutputMode(OUTPUT_MODE_MONO);
        break;
    }

    case SC_SND_STEREO: {
        detail::AxManager::GetInstance().SetOutputMode(OUTPUT_MODE_STEREO);
        break;
    }

    case SC_SND_SURROUND: {
        detail::AxManager::GetInstance().SetOutputMode(OUTPUT_MODE_DPL2);
        break;
    }

    default: {
        detail::AxManager::GetInstance().SetOutputMode(OUTPUT_MODE_STEREO);
        break;
    }
    }

    detail::RemoteSpeakerManager::GetInstance().Setup();

    u8* pPtr = static_cast<u8*>(pWork);
    pPtr += rParam.dvdThreadStackSize;

    u8* pSoundThreadStack = pPtr;
    pPtr += rParam.soundThreadStackSize;

    lbl_eu_8066550C = AXGetMaxVoices();

    // AxVoiceManager: save pointer, advance after first GetRequiredMemSize,
    // call again for setup size
    u8* pAxVoiceWork = pPtr;
    pPtr += detail::AxVoiceManager::GetInstance().GetRequiredMemSize(
        lbl_eu_8066550C);
    detail::AxVoiceManager::GetInstance().Setup(
        pAxVoiceWork,
        detail::AxVoiceManager::GetInstance().GetRequiredMemSize(
            lbl_eu_8066550C));

    // VoiceManager: same pattern
    u8* pVoiceWork = pPtr;
    pPtr += detail::VoiceManager::GetInstance().GetRequiredMemSize(
        lbl_eu_8066550C);
    detail::VoiceManager::GetInstance().Setup(
        pVoiceWork,
        detail::VoiceManager::GetInstance().GetRequiredMemSize(
            lbl_eu_8066550C));

    // ChannelManager: do NOT advance pPtr; pass maxVoices, use current pPtr for Setup
    detail::ChannelManager::GetInstance().GetRequiredMemSize(
        lbl_eu_8066550C);
    detail::ChannelManager::GetInstance().Setup(
        pPtr,
        detail::ChannelManager::GetInstance().GetRequiredMemSize(
            lbl_eu_8066550C));

    // Initialize the sequence player subsystem before creating threads
    detail::InitSeqPlayer();

    sTaskThread.Create(rParam.dvdThreadPriority, pWork,
                       rParam.dvdThreadStackSize);

    detail::SoundThread::GetInstance().Create(rParam.soundThreadPriority,
                                              pSoundThreadStack,
                                              rParam.soundThreadStackSize);
}

void SoundSystem::ShutdownSoundSystem() {
    if (!lbl_eu_80665508) {
        return;
    }

    detail::SoundThread::GetInstance().Shutdown();
    detail::TaskManager::GetInstance().CancelAllTask();
    sTaskThread.Destroy();

    detail::RemoteSpeakerManager::GetInstance().Shutdown();
    detail::ChannelManager::GetInstance().Shutdown();
    detail::VoiceManager::GetInstance().Shutdown();
    detail::AxVoiceManager::GetInstance().Shutdown();
    detail::AxManager::GetInstance().Shutdown();

    lbl_eu_80665508 = false;
}

void SoundSystem::WaitForResetReady() {
    if (!lbl_eu_80665508) {
        return;
    }

    u32 start = OSGetTick();

    while (!detail::AxManager::GetInstance().IsResetReady()) {
        if (OS_TICKS_TO_SEC(OSGetTick() - start) > 0) {
            break;
        }
    }
}

} // namespace snd
} // namespace nw4r

bool IsInitializedSoundSystem__Q34nw4r3snd11SoundSystemFv() {
    return lbl_eu_80665508;
}
