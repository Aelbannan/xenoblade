#include <nw4r/snd.h>

#include <revolution/AX.h>
#include <revolution/OS.h>
#include <revolution/SC.h>

const char* NW4R_SND_Version_ =
    "<< NW4R    - SND \tfinal   build: Nov 20 2008 03:40:00 (0x4199_60831) >>";

namespace nw4r {
namespace snd {

namespace {
bool sInitialized;
}

int SoundSystem::sMaxVoices;

// Retail bss companions for this TU (the large work buffer and the cookie)
extern "C" {
char lbl_eu_80638900[0x10];
// Real name from the owning header: SoundSystem::sTaskThread.
detail::TaskThread sTaskThread;
}

void SoundSystem::InitSoundSystem(s32 soundThreadPriority,
                                  s32 dvdThreadPriority) {
    SoundSystemParam param;
    param.soundThreadPriority = soundThreadPriority;
    param.soundThreadStackSize = 0x4000;
    param.dvdThreadPriority = dvdThreadPriority;
    param.dvdThreadStackSize = 0x4000;
    static char defaultSoundSystemWork[0x16E00];
    InitSoundSystem(param, defaultSoundSystemWork, 0x16E00);
}

void SoundSystem::InitSoundSystem(const SoundSystemParam& rParam, void* pWork,
                                  u32 workSize) {
#pragma unused(workSize)

    if (sInitialized) {
        return;
    }

    sInitialized = true;

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

    SoundSystem::sMaxVoices = AXGetMaxVoices();

    // AxVoiceManager: save pointer, advance after first GetRequiredMemSize,
    // call again for setup size
    u8* pAxVoiceWork = pPtr;
    pPtr += detail::AxVoiceManager::GetInstance().GetRequiredMemSize(
        SoundSystem::sMaxVoices);
    detail::AxVoiceManager::GetInstance().Setup(
        pAxVoiceWork,
        detail::AxVoiceManager::GetInstance().GetRequiredMemSize(
            SoundSystem::sMaxVoices));

    // VoiceManager: same pattern
    u8* pVoiceWork = pPtr;
    pPtr += detail::VoiceManager::GetInstance().GetRequiredMemSize(
        SoundSystem::sMaxVoices);
    detail::VoiceManager::GetInstance().Setup(
        pVoiceWork,
        detail::VoiceManager::GetInstance().GetRequiredMemSize(
            SoundSystem::sMaxVoices));

    // ChannelManager: do NOT advance pPtr; pass maxVoices, use current pPtr for Setup
    detail::ChannelManager::GetInstance().GetRequiredMemSize(
        SoundSystem::sMaxVoices);
    detail::ChannelManager::GetInstance().Setup(
        pPtr,
        detail::ChannelManager::GetInstance().GetRequiredMemSize(
            SoundSystem::sMaxVoices));

    // Initialize the sequence player subsystem before creating threads
    detail::SeqPlayer::InitSeqPlayer();

    sTaskThread.Create(rParam.dvdThreadPriority, pWork,
                       rParam.dvdThreadStackSize);

    detail::SoundThread::GetInstance().Create(rParam.soundThreadPriority,
                                              pSoundThreadStack,
                                              rParam.soundThreadStackSize);
}

void SoundSystem::ShutdownSoundSystem() {
    if (!sInitialized) {
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

    sInitialized = false;
}

bool SoundSystem::IsInitializedSoundSystem() {
    return sInitialized;
}

} // namespace snd
} // namespace nw4r
