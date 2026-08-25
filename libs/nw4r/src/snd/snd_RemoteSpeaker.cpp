#include <nw4r/snd.h>
#include <nw4r/ut.h>

#include <revolution/OS.h>
#include <revolution/WENC.h>
#include <revolution/WPAD.h>

#include <cstring>

namespace nw4r {
namespace snd {

RemoteSpeaker::RemoteSpeaker()
    : mInitFlag(false),
      mPlayFlag(false),
      mEnableFlag(false),
      mFirstEncodeFlag(false),
      mValidCallbackFlag(false),
      mCommandBusyFlag(false) {

    mState = STATE_INVALID;
    mUserCommand = COMMAND_NONE;
    mInternalCommand = COMMAND_NONE;
    mWpadCallback = NULL;

    OSCreateAlarm(&mContinueAlarm);
    OSSetAlarmUserData(&mContinueAlarm, this);

    OSCreateAlarm(&mIntervalAlarm);
    OSSetAlarmUserData(&mIntervalAlarm, this);
}

void RemoteSpeaker::InitParam() {
    ClearParam();

    mForceResumeFlag = false;
    mContinueFlag = false;
    mPlayFlag = false;
    mEnableFlag = true;
    mIntervalFlag = false;
}

void RemoteSpeaker::ClearParam() {
    mPlayFlag = false;
    mEnableFlag = false;

    OSCancelAlarm(&mContinueAlarm);
    mContinueFlag = false;

    OSCancelAlarm(&mIntervalAlarm);
    mIntervalFlag = false;
}

bool RemoteSpeaker::Setup(WPADCallback pCallback) {
    ut::AutoInterruptLock lock;

    InitParam();

    if (mWpadCallback != NULL) {
        mWpadCallback(mChannelIndex, WPAD_ERR_OK);
        mValidCallbackFlag = false;
    }

    mWpadCallback = pCallback;
    mUserCommand = COMMAND_SPEAKER_ON;
    mInitFlag = true;

    return true;
}

void RemoteSpeaker::Shutdown(WPADCallback pCallback) {
    ut::AutoInterruptLock lock;

    ClearParam();

    if (mWpadCallback != NULL) {
        mWpadCallback(mChannelIndex, WPAD_ERR_OK);
        mValidCallbackFlag = false;
    }

    mUserCommand = COMMAND_SPEAKER_OFF;
    mWpadCallback = pCallback;
    mInitFlag = false;
}

bool RemoteSpeaker::EnableOutput(bool enable) {
    if (!mInitFlag) {
        return false;
    }

    mEnableFlag = enable;
    return true;
}

bool RemoteSpeaker::IsEnabledOutput() const {
    if (!mInitFlag) {
        return false;
    }

    return mEnableFlag;
}

void RemoteSpeaker::Update() {
    if (mCommandBusyFlag) {
        return;
    }

    SpeakerCommand command =
        mUserCommand != COMMAND_NONE ? mUserCommand : mInternalCommand;

    mUserCommand = COMMAND_NONE;
    mInternalCommand = COMMAND_NONE;

    ExecCommand(command);
}

void RemoteSpeaker::ExecCommand(SpeakerCommand command) {
    switch (command) {
    case COMMAND_NONE: {
        break;
    }

    case COMMAND_SPEAKER_ON: {
        mValidCallbackFlag = true;
        mCommandBusyFlag = true;
        mState = STATE_EXEC_SPEAKER_ON;
        WPADControlSpeaker(mChannelIndex, WPAD_SPEAKER_ON, SpeakerOnCallback);
        break;
    }

    case COMMAND_SPEAKER_PLAY: {
        mValidCallbackFlag = true;
        mCommandBusyFlag = true;
        mState = STATE_EXEC_SPEAKER_PLAY;
        WPADControlSpeaker(mChannelIndex, WPAD_SPEAKER_PLAY,
                           SpeakerPlayCallback);
        break;
    }

    case COMMAND_SPEAKER_OFF: {
        mValidCallbackFlag = true;
        mCommandBusyFlag = true;
        mState = STATE_EXEC_SPEAKER_OFF;
        WPADControlSpeaker(mChannelIndex, WPAD_SPEAKER_OFF, SpeakerOffCallback);
        break;
    }
    }
}

void RemoteSpeaker::UpdateStreamData(const s16* pRmtSamples) {
    if (!IsAvailable()) {
        return;
    }

    BOOL enabled;
    bool firstFlag;
    bool lastFlag;
    bool playFlag = true;
    bool silentFlag = mEnableFlag ? IsAllSampleZero(pRmtSamples) : true;

    if (silentFlag) {
        playFlag = false;
    }

    firstFlag = !mPlayFlag && playFlag;
    lastFlag = mPlayFlag && !playFlag;

    if (playFlag) {
        enabled = OSDisableInterrupts();

        if (!WPADCanSendStreamData(mChannelIndex)) {
            OSRestoreInterrupts(enabled);
            return;
        }

        u32 wencMode = !mFirstEncodeFlag;
        mFirstEncodeFlag = false;

        u8 adpcmBuffer[SAMPLES_PER_ENCODED_PACKET];
        WENCGetEncodeData(&mEncodeInfo, wencMode, pRmtSamples,
                          SAMPLES_PER_AUDIO_PACKET, adpcmBuffer);

        if (WPADSendStreamData(mChannelIndex, adpcmBuffer,
                               SAMPLES_PER_ENCODED_PACKET) != WPAD_ERR_OK) {
            mState = STATE_INVALID;
            mInternalCommand = COMMAND_SPEAKER_ON;
            ClearParam();
            mContinueFlag = false;
            mPlayFlag = false;
            mEnableFlag = true;
            mIntervalFlag = false;
            OSRestoreInterrupts(enabled);
            return;
        }

        OSRestoreInterrupts(enabled);
    }

    if (firstFlag) {
        enabled = OSDisableInterrupts();

        if (!mContinueFlag) {
            OSSetAlarm(&mContinueAlarm,
                       OS_SEC_TO_TICKS(CONTINUOUS_PLAY_INTERVAL_MINUTES * 60LL),
                       ContinueAlarmHandler);
            mContinueBeginTime = OSGetTime();
            mContinueFlag = true;
        }

        OSCancelAlarm(&mIntervalAlarm);
        mIntervalFlag = false;
        OSRestoreInterrupts(enabled);
    }

    if (lastFlag) {
        enabled = OSDisableInterrupts();

        mIntervalFlag = true;
        OSCancelAlarm(&mIntervalAlarm);
        OSSetAlarm(&mIntervalAlarm, OS_SEC_TO_TICKS(1LL), IntervalAlarmHandler);
        OSRestoreInterrupts(enabled);
    }

    mPlayFlag = playFlag;
}

bool RemoteSpeaker::IsAllSampleZero(const s16* pSample) {
    const u32* pBuffer = reinterpret_cast<const u32*>(pSample);
    bool zeroFlag = true;

    for (int i = 0; i < SAMPLES_PER_ENCODED_PACKET; i++) {
        if (pBuffer[i] != 0) {
            zeroFlag = false;
            break;
        }
    }

    return zeroFlag;
}

void RemoteSpeaker::SpeakerOnCallback(s32 chan, s32 result) {
    RemoteSpeaker& r =
        detail::RemoteSpeakerManager::GetInstance().GetRemoteSpeaker(chan);

    switch (result) {
    case WPAD_ERR_OK: {
        r.mFirstEncodeFlag = true;
        std::memset(&r.mEncodeInfo, 0, sizeof(WENCInfo));

        r.mState = STATE_SPEAKER_ON;
        r.mInternalCommand = COMMAND_SPEAKER_PLAY;
        break;
    }

    case WPAD_ERR_BUSY: {
        r.mInternalCommand = COMMAND_SPEAKER_ON;
        break;
    }

    case WPAD_ERR_TRANSFER: {
        r.mState = STATE_INVALID;
        break;
    }

    case WPAD_ERR_NO_CONTROLLER: {
        r.mState = STATE_INVALID;
        break;
    }

    default: {
        r.mState = STATE_INVALID;
        break;
    }
    }

    if (result != WPAD_ERR_OK && result != WPAD_ERR_BUSY) {
        r.NotifyCallback(chan, result);
    }

    r.mCommandBusyFlag = false;
}

void RemoteSpeaker::SpeakerPlayCallback(s32 chan, s32 result) {
    RemoteSpeaker& r =
        detail::RemoteSpeakerManager::GetInstance().GetRemoteSpeaker(chan);

    switch (result) {
    case WPAD_ERR_OK: {
        r.mState = STATE_SPEAKER_PLAY;
        break;
    }

    case WPAD_ERR_BUSY: {
        r.mInternalCommand = COMMAND_SPEAKER_PLAY;
        break;
    }

    case WPAD_ERR_TRANSFER: {
        r.mState = STATE_INVALID;
        break;
    }

    case WPAD_ERR_NO_CONTROLLER: {
        r.mState = STATE_INVALID;
        break;
    }

    default: {
        r.mState = STATE_INVALID;
        break;
    }
    }

    if (result != WPAD_ERR_BUSY) {
        r.NotifyCallback(chan, result);
    }

    r.mCommandBusyFlag = false;
}

void RemoteSpeaker::SpeakerOffCallback(s32 chan, s32 result) {
    RemoteSpeaker& r =
        detail::RemoteSpeakerManager::GetInstance().GetRemoteSpeaker(chan);

    switch (result) {
    case WPAD_ERR_OK: {
        r.mState = STATE_SPEAKER_OFF;
        break;
    }

    case WPAD_ERR_BUSY: {
        r.mInternalCommand = COMMAND_SPEAKER_OFF;
        break;
    }

    case WPAD_ERR_TRANSFER: {
        r.mState = STATE_INVALID;
        break;
    }

    case WPAD_ERR_NO_CONTROLLER: {
        r.mState = STATE_INVALID;
        break;
    }

    default: {
        r.mState = STATE_INVALID;
        break;
    }
    }

    if (result != WPAD_ERR_BUSY) {
        r.NotifyCallback(chan, result);
    }

    r.mCommandBusyFlag = false;
}

void RemoteSpeaker::NotifyCallback(s32 chan, s32 result) {
    if (mValidCallbackFlag && mWpadCallback != NULL) {
        mWpadCallback(chan, result);
        mWpadCallback = NULL;
    }
}

void RemoteSpeaker::ContinueAlarmHandler(OSAlarm* pAlarm, OSContext* pCtx) {
#pragma unused(pCtx)

    BOOL old = OSDisableInterrupts();
    OSGetAlarmUserData(pAlarm);
    OSRestoreInterrupts(old);
}

void RemoteSpeaker::IntervalAlarmHandler(OSAlarm* pAlarm, OSContext* pCtx) {
#pragma unused(pCtx)

    BOOL old = OSDisableInterrupts();
    void* ud = OSGetAlarmUserData(pAlarm);
    RemoteSpeaker* p = reinterpret_cast<RemoteSpeaker*>(ud);

    if (p->mContinueFlag) {
        OSCancelAlarm(&p->mContinueAlarm);
        p->mForceResumeFlag = false;
    }

    p->mContinueFlag = false;
    OSRestoreInterrupts(old);
}

} // namespace snd
} // namespace nw4r
