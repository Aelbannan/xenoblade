#ifndef NW4R_SND_BASIC_SOUND_H
#define NW4R_SND_BASIC_SOUND_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_MoveValue.h>
#include <nw4r/snd/snd_Types.h>

#include <nw4r/ut.h>

#include <revolution/WPAD.h>

namespace nw4r {
namespace snd {

// Forward declarations
class SoundHandle;
class SoundPlayer;

namespace detail {
class BasicPlayer;
class ExternalSoundPlayer;
class PlayerHeap;
} // namespace detail

namespace detail {

class BasicSound {
    friend class nw4r::snd::SoundHandle;

public:
    NW4R_UT_RTTI_DECL(BasicSound);

    struct AmbientParamUpdateCallback {
        enum ParamUpdateFlags {
            PARAM_UPDATE_VOLUME = (1 << 0),
            PARAM_UPDATE_PAN = (1 << 1),
            PARAM_UPDATE_SURROUND_PAN = (1 << 2),
            PARAM_UPDATE_PRIORITY = (1 << 3),
        };

        virtual void detail_Update(SoundParam* pParam, u32 id,
                                   BasicSound* pSound, const void* pArg,
                                   u32 flags) = 0; // at 0xC
    };

    struct AmbientArgUpdateCallback {
        virtual void detail_Update(void* pArg,
                                   const BasicSound* pSound) = 0; // at 0xC
    };

    struct AmbientArgAllocaterCallback {
        virtual void* detail_AllocAmbientArg(u32 size) = 0; // at 0xC

        virtual void
        detail_FreeAmbientArg(void* pArg,
                              const BasicSound* pSound) = 0; // at 0x10
    };

    struct AmbientArgInfo {
        AmbientParamUpdateCallback* paramUpdateCallback;   // at 0x0
        AmbientArgUpdateCallback* argUpdateCallback;       // at 0x4
        AmbientArgAllocaterCallback* argAllocaterCallback; // at 0x8
        void* arg;                                         // at 0xC
        u32 argSize;                                       // at 0x10
    };

    static const u32 INVALID_ID = 0xFFFFFFFF;
    static const int PRIORITY_MAX = 127;

public:
    BasicSound();
    virtual ~BasicSound() {}

    virtual void Update();
    virtual void StartPrepared();
    virtual void Stop(int frames);
    virtual void Pause(bool flag, int frames);
    virtual void Shutdown();
    // US retail: GetBasicPlayer at vtable+0x24.
    virtual BasicPlayer& GetBasicPlayer() = 0;
    virtual const BasicPlayer& GetBasicPlayer() const = 0;
    virtual bool IsPrepared() const = 0;
    virtual bool IsPause() const;

    virtual bool IsAttachedTempSpecialHandle() = 0;
    virtual void DetachTempSpecialHandle() = 0;

    void SetAutoStopCounter(int count);
    void FadeIn(int frames);
    void InitParam();
    void SetInitialVolume(f32 vol);
    void SetVolume(f32 vol, int frames);
    void SetPitch(f32 pitch);
    void SetPan(f32 pan);
    void SetSurroundPan(f32 pan);
    void SetLpfFreq(f32 freq);
    void SetPlayerPriority(int priority);
    void SetRemoteFilter(int filter);
    void SetPanMode(PanMode mode);
    void SetPanCurve(PanCurve curve);

    PlayerHeap* GetPlayerHeap() {
        return mHeap;
    }
    void SetPlayerHeap(PlayerHeap* pHeap) {
        mHeap = pHeap;
    }

    bool IsAttachedGeneralHandle();
    void DetachGeneralHandle();

    bool IsAttachedTempGeneralHandle();
    void DetachTempGeneralHandle();

    SoundPlayer* GetSoundPlayer() {
        return mSoundPlayer;
    }
    void SetSoundPlayer(SoundPlayer* pPlayer) {
        mSoundPlayer = pPlayer;
    }

    ExternalSoundPlayer* GetExternalSoundPlayer() {
        return mExtSoundPlayer;
    }
    void SetExternalSoundPlayer(ExternalSoundPlayer* pExtPlayer) {
        mExtSoundPlayer = pExtPlayer;
    }

    AmbientParamUpdateCallback* GetAmbientParamUpdateCallback() {
        return mAmbientParamUpdateCallback;
    }

    AmbientArgUpdateCallback* GetAmbientArgUpdateCallback() {
        return mAmbientArgUpdateCallback;
    }
    void ClearAmbientArgUpdateCallback() {
        mAmbientArgUpdateCallback = NULL;
    }

    AmbientArgAllocaterCallback* GetAmbientArgAllocaterCallback() {
        return mAmbientArgAllocaterCallback;
    }

    void* GetAmbientArg() {
        return mAmbientArg;
    }

    SoundParam& GetAmbientParam() {
        return mAmbientParam;
    }

    void SetAmbientParamCallback(AmbientParamUpdateCallback* pParamUpdate,
                                 AmbientArgUpdateCallback* pArgUpdate,
                                 AmbientArgAllocaterCallback* pArgAlloc,
                                 void* pArg);

    void SetPriority(int priority) {
        mPriority = priority;
    }

    u32 GetId() const {
        return mId;
    }
    void SetId(u32 id);

    f32 GetMoveVolume() {
        return mExtMoveVolume.GetValue();
    }

    f32 GetInitialVolume() const;
    f32 GetPan() const;
    f32 GetSurroundPan() const;
    f32 GetPitch() const;

    void SetOutputLine(int flag);
    bool IsEnabledOutputLine() const;
    int GetOutputLine() const;

    f32 GetMainOutVolume() const;
    void SetMainOutVolume(f32 vol);

    f32 GetRemoteOutVolume(int remote) const;
    void SetRemoteOutVolume(int remote, f32 vol);

    void SetFxSend(AuxBus bus, f32 send);

    int CalcCurrentPlayerPriority() const {
        return ut::Clamp(mPriority + mAmbientParam.priority, 0, PRIORITY_MAX);
    }

private:
    PlayerHeap* mHeap;                    // at 0x4
    SoundHandle* mGeneralHandle;          // at 0x8
    SoundHandle* mTempGeneralHandle;      // at 0xC
    SoundPlayer* mSoundPlayer;            // at 0x10
    ExternalSoundPlayer* mExtSoundPlayer; // at 0x14

    AmbientParamUpdateCallback* mAmbientParamUpdateCallback;   // at 0x18
    AmbientArgUpdateCallback* mAmbientArgUpdateCallback;       // at 0x1C
    AmbientArgAllocaterCallback* mAmbientArgAllocaterCallback; // at 0x20
    void* mAmbientArg;                                         // at 0x24
    u32 mUnk0x28;                                              // at 0x28
    u32 mUnk0x2C;                                              // at 0x2C
    SoundParam mAmbientParam;                                  // at 0x30
    u32 mUnk0x4C;                                              // at 0x4C
    u32 mUnk0x50;                                              // at 0x50 (ctor arg)
    u32 mUnk0x54;                                              // at 0x54
    u32 mUnk0x58;                                              // at 0x58
    u32 mUnk0x5C;                                              // at 0x5C

    MoveValue<f32, int> mFadeVolume;      // at 0x60
    MoveValue<f32, int> mPauseFadeVolume; // at 0x70

    bool mStartFlag;                      // at 0x80
    bool mStartedFlag;                    // at 0x81
    bool mAutoStopFlag;                   // at 0x82
    bool mFadeOutFlag;                    // at 0x83
    int mPauseState;                      // at 0x84
    bool mPauseFadeFlag;                  // at 0x88
    bool mPauseFlag;                      // at 0x89
    u8 mPad0x8A[2];                       // at 0x8A
    int mAutoStopCounter;                 // at 0x8C
    u32 mUpdateCounter;                   // at 0x90
    u8 mPriority;                         // at 0x94
    u8 mUnk0x95;                          // at 0x95
    bool mOutputLineFlagEnable;           // at 0x96
    u8 mPad0x97;                          // at 0x97
    u32 mId;                              // at 0x98

    MoveValue<f32, int> mExtMoveVolume; // at 0x9C
    f32 mInitVolume;                    // at 0xAC
    f32 mExtPan;                        // at 0xB0
    f32 mExtSurroundPan;                // at 0xB4
    f32 mExtPitch;                      // at 0xB8
    f32 mUnk0xBC;                       // at 0xBC
    f32 mUnk0xC0;                       // at 0xC0
    int mOutputLineFlag;                // at 0xC4
    f32 mMainOutVolume;                 // at 0xC8
    f32 mUnk0xCC[4];                    // at 0xCC
    f32 mRemoteOutVolume[WPAD_MAX_CONTROLLERS]; // at 0xDC

public:
    NW4R_UT_LINKLIST_NODE_DECL_EX(Prio);       // at 0xEC
    NW4R_UT_LINKLIST_NODE_DECL_EX(PlayerPlay); // at 0xF4
    NW4R_UT_LINKLIST_NODE_DECL_EX(PlayerPrio); // at 0xFC
    NW4R_UT_LINKLIST_NODE_DECL_EX(ExtPlay);    // at 0x104
};

NW4R_UT_LINKLIST_TYPEDEF_DECL_EX(BasicSound, Prio);
NW4R_UT_LINKLIST_TYPEDEF_DECL_EX(BasicSound, PlayerPlay);
NW4R_UT_LINKLIST_TYPEDEF_DECL_EX(BasicSound, PlayerPrio);
NW4R_UT_LINKLIST_TYPEDEF_DECL_EX(BasicSound, ExtPlay);

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
