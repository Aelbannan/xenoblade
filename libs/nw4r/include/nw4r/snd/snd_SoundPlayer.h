#ifndef NW4R_SND_SOUND_PLAYER_H
#define NW4R_SND_SOUND_PLAYER_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_BasicSound.h>
#include <nw4r/snd/snd_PlayerHeap.h>

#include <revolution/WPAD.h>

namespace nw4r {
namespace snd {

// Forward declarations
namespace detail {
class ExternalSoundPlayer;
class SeqSound;
template <typename T> class SoundInstanceManager;
class StrmSound;
class WaveSound;
} // namespace detail

class SoundPlayer {
public:
    SoundPlayer();
    ~SoundPlayer();

    void InitParam();
    void Update();

    void StopAllSound(int frames);
    void PauseAllSound(bool flag, int frames);

    void SetVolume(f32 volume);

    int detail_GetOutputLine() const {
        return mOutputLineFlag;
    }
    bool detail_IsEnabledOutputLine() const {
        return mOutputLineFlagEnable;
    }

    f32 GetRemoteOutVolume(int idx) const;

    void detail_InsertSoundList(detail::BasicSound* pSound);
    void detail_RemoveSoundList(detail::BasicSound* pSound);

    void detail_InsertPriorityList(detail::BasicSound* pSound);
    void detail_RemovePriorityList(detail::BasicSound* pSound);

    void detail_SortPriorityList(detail::BasicSound* pSound);
    void detail_SortPriorityList();

    bool detail_AppendSound(detail::BasicSound* pSound);
    void detail_RemoveSound(detail::BasicSound* pSound);
    bool detail_CanPlaySound(int priority);

    void SetFxSend(AuxBus bus, f32 send);

    detail::SeqSound* detail_AllocSeqSound(
        int priority, int startPriority,
        detail::BasicSound::AmbientArgInfo* pArgInfo,
        detail::ExternalSoundPlayer* pExtPlayer, u32 id,
        detail::SoundInstanceManager<detail::SeqSound>* pManager);

    detail::StrmSound* detail_AllocStrmSound(
        int priority, int startPriority,
        detail::BasicSound::AmbientArgInfo* pArgInfo,
        detail::ExternalSoundPlayer* pExtPlayer, u32 id,
        detail::SoundInstanceManager<detail::StrmSound>* pManager);

    detail::WaveSound* detail_AllocWaveSound(
        int priority, int startPriority,
        detail::BasicSound::AmbientArgInfo* pArgInfo,
        detail::ExternalSoundPlayer* pExtPlayer, u32 id,
        detail::SoundInstanceManager<detail::WaveSound>* pManager);

    int CalcPriorityReduction(detail::BasicSound::AmbientArgInfo* pArgInfo,
                              u32 id);

    void InitAmbientArg(detail::BasicSound* pSound,
                        detail::BasicSound::AmbientArgInfo* pArgInfo);

    void SetPlayableSoundCount(int count);
    void detail_SetPlayableSoundLimit(int limit);

    bool CheckPlayableSoundCount(int startPriority,
                                 detail::ExternalSoundPlayer* pExtPlayer);

    void detail_AppendPlayerHeap(detail::PlayerHeap* pHeap);
    detail::PlayerHeap* detail_AllocPlayerHeap(detail::BasicSound* pSound);
    void detail_FreePlayerHeap(detail::BasicSound* pSound);

    int GetPlayingSoundCount() const {
        return mSoundList.GetSize();
    }
    int GetPlayableSoundCount() const {
        return mPlayableCount;
    }

    f32 GetVolume() const {
        return mVolume;
    }

    detail::BasicSound* detail_GetLowestPrioritySound() {
        // @bug UB when the list is empty
        return &mPriorityList.GetFront();
    }

    f32 detail_GetMainOutVolume() const {
        return mMainOutVolume;
    }

private:
    detail::BasicSoundPlayerPlayList mSoundList;    // at 0x0
    detail::BasicSoundPlayerPrioList mPriorityList; // at 0xC
    detail::PlayerHeapList mHeapList;               // at 0x18

    u32 mPlayableCount; // at 0x24
    u32 mPlayableLimit; // at 0x28

    f32 mVolume;                                // at 0x2C
    f32 mUnk0x30;                               // at 0x30
    int mOutputLineFlag;                        // at 0x34
    f32 mMainOutVolume;                         // at 0x38
    bool mOutputLineFlagEnable;                 // at 0x3C
    bool mUsePlayerHeap;                        // at 0x3D
    u8 mPad0x3E[2];                             // at 0x3E
    f32 mUnk0x40;                               // at 0x40
    f32 mRemoteOutVolume[WPAD_MAX_CONTROLLERS]; // at 0x44
    f32 mUnk0x54;                               // at 0x54
    f32 mFxSend[AUX_BUS_NUM];                   // at 0x58
};

} // namespace snd
} // namespace nw4r

#endif
