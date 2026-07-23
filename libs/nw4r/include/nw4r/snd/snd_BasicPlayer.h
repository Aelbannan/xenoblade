#ifndef NW4R_SND_BASIC_PLAYER_H
#define NW4R_SND_BASIC_PLAYER_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_Types.h>

#include <nw4r/ut.h>

#include <revolution/WPAD.h>

namespace nw4r {
namespace snd {
namespace detail {

// Embedded at BasicPlayer+0x4. sizeof == 0xCC; BasicPlayer::mId follows at 0xD0.
struct PlayerParamSet {
    void Init();

    f32 mVolume;      // at 0x0
    f32 mPitch;       // at 0x4
    f32 mPan;         // at 0x8
    f32 mSurroundPan; // at 0xC
    f32 mLpfFreq;     // at 0x10
    f32 mUnk0x14;     // at 0x14
    u8 mUnk0x18;      // at 0x18
    u8 mRemoteFilter; // at 0x19 (BasicPlayer+0x1D)
    u8 mPad0x1A[2];   // at 0x1A
    int mOutputLine;  // at 0x1C
    f32 mMainOutVolume;                         // at 0x20
    f32 mMainSend;                              // at 0x24
    PanMode mPanMode;                           // at 0x28 (BasicPlayer+0x2C)
    PanCurve mPanCurve;                         // at 0x2C (BasicPlayer+0x30)
    f32 mFxSend[AUX_BUS_NUM];                   // at 0x30 (BasicPlayer+0x34)
    f32 mRemoteOutVolume[WPAD_MAX_CONTROLLERS]; // at 0x3C (BasicPlayer+0x40)
    f32 mRemoteSend[WPAD_MAX_CONTROLLERS];      // at 0x4C (BasicPlayer+0x50)
    f32 mRemoteFxSend[WPAD_MAX_CONTROLLERS];    // at 0x5C (BasicPlayer+0x60)
    // Xenoblade extension: 4× (1,1,0,0,0,0) float groups through +0xC8.
    f32 mUnk0x6C[24]; // at 0x6C
};

class BasicPlayer {
public:
    BasicPlayer();
    virtual ~BasicPlayer() {} // at 0x8

    virtual bool Start() = 0;           // at 0xC
    virtual void Stop() = 0;            // at 0x10
    virtual void Pause(bool flag) = 0;  // at 0x14
    virtual bool IsActive() const = 0;  // at 0x18
    virtual bool IsStarted() const = 0; // at 0x1C
    virtual bool IsPause() const = 0;   // at 0x20

    void InitParam();

    u32 GetId() const {
        return mId;
    }
    void SetId(u32 id) {
        mId = id;
    }

    f32 GetVolume() const {
        return mParam.mVolume;
    }
    void SetVolume(f32 volume) {
        mParam.mVolume = volume;
    }

    f32 GetPitch() const {
        return mParam.mPitch;
    }
    void SetPitch(f32 pitch) {
        mParam.mPitch = pitch;
    }

    f32 GetPan() const {
        return mParam.mPan;
    }
    void SetPan(f32 pan) {
        mParam.mPan = pan;
    }

    f32 GetSurroundPan() const {
        return mParam.mSurroundPan;
    }
    void SetSurroundPan(f32 pan) {
        mParam.mSurroundPan = pan;
    }

    f32 GetLpfFreq() const {
        return mParam.mLpfFreq;
    }
    void SetLpfFreq(f32 freq) {
        mParam.mLpfFreq = freq;
    }

    int GetOutputLine() const {
        return mParam.mOutputLine;
    }
    void SetOutputLine(int flags) {
        mParam.mOutputLine = flags;
    }

    f32 GetMainOutVolume() const {
        return mParam.mMainOutVolume;
    }
    void SetMainOutVolume(f32 volume) {
        mParam.mMainOutVolume = volume;
    }

    f32 GetMainSend() const {
        return mParam.mMainSend;
    }
    void SetMainSend(f32 send) {
        mParam.mMainSend = send;
    }

    void SetFxSend(AuxBus bus, f32 send);
    f32 GetFxSend(AuxBus bus) const;

    void SetRemoteOutVolume(int remote, f32 volume);
    f32 GetRemoteOutVolume(int remote) const;

    f32 GetRemoteSend(int remote) const;
    f32 GetRemoteFxSend(int remote) const;

    int GetRemoteFilter() const {
        return mParam.mRemoteFilter;
    }
    void SetRemoteFilter(int filter);

    PanMode GetPanMode() const {
        return mParam.mPanMode;
    }
    void SetPanMode(PanMode mode) {
        mParam.mPanMode = mode;
    }

    PanCurve GetPanCurve() const {
        return mParam.mPanCurve;
    }
    void SetPanCurve(PanCurve curve) {
        mParam.mPanCurve = curve;
    }

private:
    PlayerParamSet mParam; // at 0x4
    u32 mId;               // at 0xD0
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
