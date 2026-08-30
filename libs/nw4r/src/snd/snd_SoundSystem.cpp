#include <nw4r/snd.h>

#include <revolution/AX.h>
#include <revolution/OS.h>
#include <revolution/SC.h>

namespace nw4r {
namespace snd {

namespace detail {
inline void InitSeqPlayer() {
    InitSeqPlayer__Q44nw4r3snd6detail9SeqPlayerFv();
}
} // namespace detail

extern "C" {
__declspec(section ".data") __attribute__((aligned(8))) const unsigned char lbl_eu_8056AC30[0x48] = {
    0x3c, 0x3c, 0x20, 0x4e, 0x57, 0x34, 0x52, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x20, 0x53, 0x4e, 0x44,
    0x20, 0x09, 0x66, 0x69, 0x6e, 0x61, 0x6c, 0x20, 0x20, 0x20, 0x62, 0x75, 0x69, 0x6c, 0x64, 0x3a,
    0x20, 0x4e, 0x6f, 0x76, 0x20, 0x32, 0x30, 0x20, 0x32, 0x30, 0x30, 0x38, 0x20, 0x30, 0x33, 0x3a,
    0x34, 0x30, 0x3a, 0x30, 0x30, 0x20, 0x28, 0x30, 0x78, 0x34, 0x31, 0x39, 0x39, 0x5f, 0x36, 0x30,
    0x38, 0x33, 0x31, 0x29, 0x20, 0x3e, 0x3e, 0x00
};
__declspec(section ".sdata") __attribute__((aligned(4))) const char* lbl_eu_806634E8[2] = { (const char*)lbl_eu_8056AC30, nullptr };
}

// Retail bss companions for this TU (the large work buffer and the cookie)
extern "C" {
char lbl_eu_80638900[0x10];
char lbl_eu_80638C40[0x16E00];
bool lbl_eu_80665508;
int lbl_eu_8066550C;
detail::TaskThread lbl_eu_80638910;
}

void SoundSystem::InitSoundSystem(s32 soundThreadPriority,
                                  s32 dvdThreadPriority) {
    SoundSystemParam param;
    param.soundThreadPriority = soundThreadPriority;
    param.soundThreadStackSize = 0x4000;
    param.dvdThreadPriority = dvdThreadPriority;
    param.dvdThreadStackSize = 0x4000;
    InitSoundSystem(param, lbl_eu_80638C40, 0x16E00);
}

void SoundSystem::InitSoundSystem(const SoundSystemParam& rParam, void* pWork,
                                  u32 workSize) {
#pragma unused(workSize)

    if (lbl_eu_80665508) {
        return;
    }

    lbl_eu_80665508 = true;

    OSRegisterVersion(lbl_eu_806634E8[0]);

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

    lbl_eu_80638910.Create(rParam.dvdThreadPriority, pWork,
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
    lbl_eu_80638910.Destroy();

    detail::RemoteSpeakerManager::GetInstance().Shutdown();
    detail::ChannelManager::GetInstance().Shutdown();
    detail::VoiceManager::GetInstance().Shutdown();
    detail::AxVoiceManager::GetInstance().Shutdown();
    detail::AxManager::GetInstance().Shutdown();

    lbl_eu_80665508 = false;
}

} // namespace snd
} // namespace nw4r

bool IsInitializedSoundSystem__Q34nw4r3snd11SoundSystemFv() {
    return lbl_eu_80665508;
}
