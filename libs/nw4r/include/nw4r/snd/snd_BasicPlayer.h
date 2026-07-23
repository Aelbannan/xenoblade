#ifndef NW4R_SND_BASIC_PLAYER_H
#define NW4R_SND_BASIC_PLAYER_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_Types.h>

#include <nw4r/ut.h>

#include <revolution/WPAD.h>

namespace nw4r {
namespace snd {
namespace detail {

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
        return mVolume;
    }
    void SetVolume(f32 volume) {
        mVolume = volume;
    }

    f32 GetPitch() const {
        return mPitch;
    }
    void SetPitch(f32 pitch) {
        mPitch = pitch;
    }

    f32 GetPan() const {
        return mPan;
    }
    void SetPan(f32 pan) {
        mPan = pan;
    }

    f32 GetSurroundPan() const {
        return mSurroundPan;
    }
    void SetSurroundPan(f32 pan) {
        mSurroundPan = pan;
    }

    f32 GetLpfFreq() const {
        return mLpfFreq;
    }
    void SetLpfFreq(f32 freq) {
        mLpfFreq = freq;
    }

    int GetOutputLine() const {
        return mOutputLine;
    }
    void SetOutputLine(int flags) {
        mOutputLine = flags;
    }

    f32 GetMainOutVolume() const {
        return mMainOutVolume;
    }
    void SetMainOutVolume(f32 volume) {
        mMainOutVolume = volume;
    }

    f32 GetMainSend() const {
        return mMainSend;
    }
    void SetMainSend(f32 send) {
        mMainSend = send;
    }

    void SetFxSend(AuxBus bus, f32 send);
    f32 GetFxSend(AuxBus bus) const;

    void SetRemoteOutVolume(int remote, f32 volume);
    f32 GetRemoteOutVolume(int remote) const;

    f32 GetRemoteSend(int remote) const;
    f32 GetRemoteFxSend(int remote) const;

    int GetRemoteFilter() const {
        return mRemoteFilter;
    }
    void SetRemoteFilter(int filter) {
        mRemoteFilter = ut::Clamp(filter, 0, REMOTE_FILTER_MAX);
    }

    PanMode GetPanMode() const {
        return mPanMode;
    }
    void SetPanMode(PanMode mode) {
        mPanMode = mode;
    }

    PanCurve GetPanCurve() const {
        return mPanCurve;
    }
    void SetPanCurve(PanCurve curve) {
        mPanCurve = curve;
    }

private:
    // 0x4 was mislabeled as mId; retail mId is at 0xD0. Keep the word so
    // send-array bases (0x34/0x40/0x50/0x60) stay correct.
    u32 mPad0x4; // at 0x4

    f32 mVolume;      // at 0x8
    f32 mPitch;       // at 0xC
    f32 mPan;         // at 0x10
    f32 mSurroundPan; // at 0x14
    f32 mLpfFreq;     // at 0x18
    char UNK_0x1C[0x4];

    int mOutputLine;                            // at 0x20
    f32 mMainOutVolume;                         // at 0x24
    f32 mMainSend;                              // at 0x28
    u32 mUnk0x2C;                               // at 0x2C
    u32 mUnk0x30;                               // at 0x30
    f32 mFxSend[AUX_BUS_NUM];                   // at 0x34
    f32 mRemoteOutVolume[WPAD_MAX_CONTROLLERS]; // at 0x40
    f32 mRemoteSend[WPAD_MAX_CONTROLLERS];      // at 0x50
    f32 mRemoteFxSend[WPAD_MAX_CONTROLLERS];    // at 0x60
    u8 mRemoteFilter;                           // at 0x70
    u8 mPad0x71[3];                             // at 0x71
    PanMode mPanMode;                           // at 0x74
    PanCurve mPanCurve;                         // at 0x78
    u8 mPad0x7C[0x54];                          // at 0x7C
    u32 mId;                                    // at 0xD0
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
