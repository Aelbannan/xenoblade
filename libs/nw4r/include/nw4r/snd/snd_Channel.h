#ifndef NW4R_SND_CHANNEL_H
#define NW4R_SND_CHANNEL_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_EnvGenerator.h>
#include <nw4r/snd/snd_Lfo.h>
#include <nw4r/snd/snd_MoveValue.h>
#include <nw4r/snd/snd_Types.h>
#include <nw4r/snd/snd_Voice.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

// Forward declarations
struct WaveData;

class Channel {
    friend class ChannelManager;

public:
    enum ChannelCallbackStatus {
        CALLBACK_STATUS_STOPPED,
        CALLBACK_STATUS_DROP,
        CALLBACK_STATUS_FINISH,
        CALLBACK_STATUS_CANCEL
    };

    typedef void (*ChannelCallback)(Channel* pDropChannel,
                                    ChannelCallbackStatus status,
                                    u32 callbackArg);

    enum LfoTarget { LFO_TARGET_PITCH, LFO_TARGET_VOLUME, LFO_TARGET_PAN };

public:
    Channel();
    ~Channel();

    void InitParam(ChannelCallback pCallback, u32 callbackArg);
    void Update(bool periodic);
    void Start(const WaveData& rData, int length, u32 offset);
    void Release();
    void NoteOff();
    void Stop();

    void SetAttack(int attack) {
        mEnvelope.SetAttack(attack);
    }
    void SetDecay(int decay) {
        mEnvelope.SetDecay(decay);
    }
    void SetSustain(int sustain) {
        mEnvelope.SetSustain(sustain);
    }
    void SetRelease(int release) {
        mEnvelope.SetRelease(release);
    }
    bool IsRelease() const {
        return mEnvelope.GetStatus() == EnvGenerator::STATUS_RELEASE;
    }

    void SetLfoParam(const LfoParam& rParam) {
        mLfo.SetParam(rParam);
    }
    void SetLfoTarget(LfoTarget target) {
        mLfoTarget = target;
    }

    void Pause(bool pause) {
        mPauseFlag = pause;
        mVoice->Pause(pause);
    }
    void SetReleasePriorityFix(bool fix) {
        mReleasePriorityFixFlag = fix;
    }

    bool IsPause() const {
        return mPauseFlag != false;
    }
    bool IsActive() const {
        return mActiveFlag;
    }
    bool IsAutoUpdateSweep() const {
        return mAutoSweep;
    }

    void SetUserVolume(f32 volume) {
        mUserVolume = volume;
    }
    void SetUserPitch(f32 pitch) {
        mUserPitch = pitch;
    }
    void SetUserPitchRatio(f32 ratio) {
        mUserPitchRatio = ratio;
    }
    void SetUserPan(f32 pan) {
        mUserPan = pan;
    }
    void SetUserSurroundPan(f32 pan) {
        mUserSurroundPan = pan;
    }
    void SetUserLpfFreq(f32 freq) {
        mUserLpfFreq = freq;
    }

    void SetRemoteFilter(int filter) {
        mRemoteFilter = filter;
    }
    void SetOutputLine(int flag) {
        mOutputLineFlag = flag;
    }

    void SetMainOutVolume(f32 volume) {
        mMainOutVolume = volume;
    }
    void SetMainSend(f32 send) {
        mMainSend = send;
    }
    void SetFxSend(AuxBus bus, f32 send) {
        mFxSend[bus] = send;
    }

    void SetRemoteOutVolume(int remote, f32 volume) {
        mRemoteOutVolume[remote] = volume;
    }
    void SetRemoteSend(int remote, f32 send) {
        mRemoteSend[remote] = send;
    }
    void SetRemoteFxSend(int remote, f32 send) {
        mRemoteFxSend[remote] = send;
    }

    void UpdateSweep(int count);
    void SetSweepParam(f32 pitch, int time, bool autoUpdate);
    void SetBiquadFilter(int type, f32 value);
    f32 GetSweepValue() const;

    void SetInitVolume(f32 volume) {
        mInitVolume = volume;
    }
    void SetInitPan(f32 pan) {
        mInitPan = pan;
    }
    void SetInitSurroundPan(f32 pan) {
        mInitSurroundPan = pan;
    }
    void SetTune(f32 tune) {
        mTune = tune;
    }
    void SetSilence(bool silence, int fadeTime) {
        mSilenceVolume.SetTarget(silence ? 0 : SILENCE_VOLUME_MAX, fadeTime);
    }

    void SetKey(int key) {
        mKey = key;
    }
    void SetOriginalKey(int key) {
        mOriginalKey = key;
    }

    s32 GetLength() const {
        return mLength;
    }
    void SetLength(s32 length) {
        mLength = length;
    }

    void SetPanMode(PanMode mode) {
        mPanMode = mode;
    }
    void SetPanCurve(PanCurve curve) {
        mPanCurve = curve;
    }

    Channel* GetNextTrackChannel() const {
        return mNextLink;
    }
    void SetNextTrackChannel(Channel* pChannel) {
        mNextLink = pChannel;
    }

    static Channel* AllocChannel(int channels, int voices, int priority,
                                 ChannelCallback pCallback, u32 callbackArg);
    static void FreeChannel(Channel* pChannel);

private:
    static const u8 SILENCE_VOLUME_MAX = 255;

    static const int KEY_INIT = 60;
    static const int ORIGINAL_KEY_INIT = 60;

    static const int PRIORITY_RELEASE = 1;

private:
    static void VoiceCallbackFunc(Voice* pDropVoice,
                                  Voice::VoiceCallbackStatus status,
                                  void* pCallbackArg);

private:
    EnvGenerator mEnvelope; // at 0x0
    Lfo mLfo;               // at 0x1C
    u8 mBiquadFilterType;   // at 0x34

    bool mPauseFlag;              // at 0x35
    bool mActiveFlag;             // at 0x36
    bool mAllocFlag;              // at 0x37
    bool mAutoSweep;              // at 0x38
    bool mReleasePriorityFixFlag; // at 0x39
    u8 field_0x3A;                // at 0x3A
    u8 mLfoTarget;                // at 0x3B
    u8 mPanMode;                  // at 0x3C
    u8 mPanCurve;                 // at 0x3D
    u8 mRemoteFilter;             // at 0x3E
    u8 PADDING_0x3F;              // at 0x3F

    f32 mUserVolume;      // at 0x40
    f32 mUserPitchRatio;  // at 0x44
    f32 mUserPan;         // at 0x48
    f32 mUserSurroundPan; // at 0x4C
    f32 mUserLpfFreq;     // at 0x50

    f32 mBiquadFilterValue;   // at 0x54
    int mOutputLineFlag;      // at 0x58
    f32 mMainOutVolume;       // at 0x5C
    f32 mMainSend;            // at 0x60
    f32 mFxSend[AUX_BUS_NUM]; // at 0x64

    f32 mRemoteOutVolume[WPAD_MAX_CONTROLLERS]; // at 0x70
    f32 mRemoteSend[WPAD_MAX_CONTROLLERS];      // at 0x80
    f32 mRemoteFxSend[WPAD_MAX_CONTROLLERS];    // at 0x90

    f32 mUserPitch;    // at 0xA0
    f32 mSweepPitch;   // at 0xA4
    int mSweepCounter; // at 0xA8
    int mSweepLength;  // at 0xAC

    f32 mInitVolume;                   // at 0xB0
    f32 mInitPan;                      // at 0xB4
    f32 mInitSurroundPan;              // at 0xB8
    f32 mTune;                         // at 0xBC
    MoveValue<u8, u16> mSilenceVolume; // at 0xC0

    int mKey;         // at 0xC8
    int mOriginalKey; // at 0xCC
    int mLength;      // at 0xD0

    u32 field_0xD4;    // at 0xD4
    u32 field_0xD8;    // at 0xD8
    u32 field_0xDC;    // at 0xDC

    ChannelCallback mCallback; // at 0xE0
    u32 mCallbackData;         // at 0xE4

    Channel* mNextLink; // at 0xE8
    u32 field_0xEC;     // at 0xEC
    Voice* mVoice;      // at 0xF0
    u32 field_0xF4;     // at 0xF4

public:
    NW4R_UT_LINKLIST_NODE_DECL(); // at 0xF8
};

NW4R_UT_LINKLIST_TYPEDEF_DECL(Channel);

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
