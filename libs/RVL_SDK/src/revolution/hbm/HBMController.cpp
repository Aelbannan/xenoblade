#include <homebuttonLib.h>

#include <revolution/MTX.h>
#include <revolution/OS.h>
#include <revolution/PAD.h>
#include <revolution/WPAD.h>

extern "C" void* WPADIsUsedCallbackByKPAD();
extern "C" void WPADSetCallbackByKPAD(void* callback);
extern "C" u8 WPADGetRadioSensitivity(s32 chan);

namespace homebutton {

bool Controller::sBatteryFlag[WPAD_MAX_CONTROLLERS];
OSAlarm Controller::sAlarm[WPAD_MAX_CONTROLLERS];
OSAlarm Controller::sAlarmSoundOff[WPAD_MAX_CONTROLLERS];
Controller* Controller::sThis[WPAD_MAX_CONTROLLERS];
bool Controller::sSetInfoAsync[WPAD_MAX_CONTROLLERS];

void Controller::wpadConnectCallback(s32 chan, s32 result) {
    if (sThis[chan] == NULL) {
        return;
    }

    switch (result) {
    case WPAD_ERR_OK:
        if (!sThis[chan]->mCallbackFlag) {
            WPADSetExtensionCallback(chan, &wpadExtensionCallback);
            sThis[chan]->mCallbackFlag = true;
        }

        WPADControlSpeaker(chan, WPAD_SPEAKER_OFF, NULL);
        break;

    case WPAD_ERR_NO_CONTROLLER:
        WPADSetExtensionCallback(chan, sThis[chan]->mOldExtensionCallback);
        sThis[chan]->mCallbackFlag = false;
        sThis[chan]->mCheckSoundTimeFlag = false;
        sThis[chan]->mCheckSoundIntervalFlag = false;
        break;
    }

    if (sThis[chan]->mOldConnectCallback != NULL &&
        sThis[chan]->mOldConnectCallback != &wpadConnectCallback) {
        sThis[chan]->mOldConnectCallback(chan, result);
    }
}

void Controller::wpadExtensionCallback(s32 chan, s32 result) {
    Controller* ctrl = sThis[chan];

    if (ctrl == NULL) {
        return;
    }

    if (result == WPAD_DEV_UNKNOWN) {
        s32 ctrlChan = ctrl->mHBController.chan;

        if (WPADIsSpeakerEnabled(ctrlChan)) {
            WPADControlSpeaker(ctrlChan, WPAD_SPEAKER_MUTE, NULL);

            OSSetAlarmUserData(&sAlarmSoundOff[ctrlChan],
                               reinterpret_cast<void*>(ctrlChan));
            OSCancelAlarm(&sAlarmSoundOff[ctrlChan]);
            OSSetAlarm(&sAlarmSoundOff[ctrlChan], OS_MSEC_TO_TICKS(1000),
                       soundOnCallback);

            ctrl->mSoundOffFlag = true;
        }
    }

    ctrl = sThis[chan];
    if (ctrl->mOldExtensionCallback != NULL) {
        ctrl->mOldExtensionCallback(chan, result);
    }
}

void Controller::soundOnCallback(OSAlarm* pAlarm, OSContext* /* pContext */) {
    int chan = reinterpret_cast<int>(OSGetAlarmUserData(pAlarm));
    Controller* ctrl = sThis[chan];

    int c = ctrl->mHBController.chan;
    if (WPADIsSpeakerEnabled(c)) {
        WPADControlSpeaker(c, WPAD_SPEAKER_UNMUTE, NULL);
    }

    ctrl->mSoundOffFlag = false;
}

Controller::Controller(int chan, RemoteSpk* pSpk) {
    mHBController.chan = chan;
    mHBController.rumble = false;
    mHBController.spVol = 1.0f;

    remotespk = pSpk;
    mOldConnectCallback = NULL;
    mOldExtensionCallback = NULL;
    mCallbackFlag = false;
    mSoundOffFlag = false;
    mRumbleFlag = true;

    if (chan < WPAD_MAX_CONTROLLERS) {
        sBatteryFlag[chan] = false;
        OSCreateAlarm(&sAlarm[chan]);
        OSCreateAlarm(&sAlarmSoundOff[chan]);
        sThis[chan] = this;
        sSetInfoAsync[chan] = false;
    }
}

Controller::~Controller() {
    OSCancelAlarm(&sAlarm[mHBController.chan]);
    OSCancelAlarm(&sAlarmSoundOff[mHBController.chan]);

    sThis[mHBController.chan] = NULL;
}

void Controller::initCallback() {
    s32 type;

    void* prev = WPADIsUsedCallbackByKPAD();
    WPADSetCallbackByKPAD(NULL);

    mOldConnectCallback =
        WPADSetConnectCallback(mHBController.chan, wpadConnectCallback);

    WPADSetCallbackByKPAD(prev);

    mOldExtensionCallback =
        WPADSetExtensionCallback(mHBController.chan, wpadExtensionCallback);

    mRumbleFlag = true;

    switch (WPADProbe(mHBController.chan, &type)) {
    case WPAD_ERR_OK:
        mCallbackFlag = true;
        break;
    case WPAD_ERR_NO_CONTROLLER:
        mCallbackFlag = false;
        break;
    }
}

void Controller::clearCallback() {
    WPADControlSpeaker(mHBController.chan, WPAD_SPEAKER_ON, NULL);
    WPADSetCallbackByKPAD(NULL);
    WPADSetConnectCallback(mHBController.chan, mOldConnectCallback);
    WPADSetCallbackByKPAD((void*)1);
    WPADSetExtensionCallback(mHBController.chan, mOldExtensionCallback);
}

void Controller::setKpad(const HBMKPadData* pPadData, bool updatePos) {
    if (pPadData->kpad == NULL) {
        return;
    }

    if (updatePos) {
        if ((pPadData->kpad->dev_type == WPAD_DEV_CLASSIC && pPadData->use_devtype == WPAD_DEV_CLASSIC) ||
            (pPadData->kpad->dev_type == 7 && pPadData->use_devtype == 7)) {
            mHBController.x = pPadData->pos.x;
            mHBController.y = pPadData->pos.y;
        } else {
            mHBController.x = pPadData->kpad->pos.x;
            mHBController.y = pPadData->kpad->pos.y;
        }
    }

    mHBController.trig = pPadData->kpad->trig;
    mHBController.hold = pPadData->kpad->hold;
    mHBController.release = pPadData->kpad->release;

    if ((pPadData->kpad->dev_type == WPAD_DEV_CLASSIC && pPadData->use_devtype == WPAD_DEV_CLASSIC) ||
        (pPadData->kpad->dev_type == 7 && pPadData->use_devtype == 7)) {
        u32 h = pPadData->kpad->ex_status.cl.hold;
        u32 t = pPadData->kpad->ex_status.cl.trig;
        u32 r = pPadData->kpad->ex_status.cl.release;

        if (h & WPAD_BUTTON_CL_A) mHBController.hold |= WPAD_BUTTON_A;
        if (t & WPAD_BUTTON_CL_A) mHBController.trig |= WPAD_BUTTON_A;
        if (r & WPAD_BUTTON_CL_A) mHBController.release |= WPAD_BUTTON_A;
        if (h & WPAD_BUTTON_CL_PLUS) mHBController.hold |= WPAD_BUTTON_PLUS;
        if (t & WPAD_BUTTON_CL_PLUS) mHBController.trig |= WPAD_BUTTON_PLUS;
        if (r & WPAD_BUTTON_CL_PLUS) mHBController.release |= WPAD_BUTTON_PLUS;
        if (h & WPAD_BUTTON_CL_MINUS) mHBController.hold |= WPAD_BUTTON_MINUS;
        if (t & WPAD_BUTTON_CL_MINUS) mHBController.trig |= WPAD_BUTTON_MINUS;
        if (r & WPAD_BUTTON_CL_MINUS) mHBController.release |= WPAD_BUTTON_MINUS;
        if (h & WPAD_BUTTON_CL_HOME) mHBController.hold |= WPAD_BUTTON_HOME;
        if (t & WPAD_BUTTON_CL_HOME) mHBController.trig |= WPAD_BUTTON_HOME;
        if (r & WPAD_BUTTON_CL_HOME) mHBController.release |= WPAD_BUTTON_HOME;
    }
}

void Controller::clrKpadButton() {
    mHBController.trig = 0;
    mHBController.hold = 0;
    mHBController.release = 0;
}

extern const float lbl_80518684;
extern "C" const float lbl_80518688;
extern "C" const float lbl_8051868C;

void Controller::setInValidPos() {
    mHBController.x = lbl_80518684;
    mHBController.y = lbl_80518688;
}

void Controller::connect() {
    remotespk->Connect(mHBController.chan);
}

void Controller::disconnect() {}

void Controller::setSpeakerVol(f32 vol) {
    mHBController.spVol = vol;
}

void Controller::playSound(int id) {
    if (mSoundOffFlag) {
        return;
    }

    // HBM_MAX_VOLUME (lbl_8051868C) scaled by speaker volume, cast to s8 for WPAD
    remotespk->Play(mHBController.chan, id,
                    (s8)(lbl_8051868C * getSpeakerVol()));

    if (WPADIsSpeakerEnabled(mHBController.chan)) {
        if (!mCheckSoundTimeFlag) {
            mPlaySoundTime = OSGetTime();
        }

        mCheckSoundTimeFlag = true;
        mCheckSoundIntervalFlag = false;
    }
}

bool Controller::isPlayingSoundId(int id) const {
    if (!remotespk->isPlaying(mHBController.chan)) {
        return false;
    }

    if (!remotespk->isPlayingId(mHBController.chan, id)) {
        return false;
    }

    return true;
}

void Controller::initSound() {
    mCheckSoundTimeFlag = false;
    mCheckSoundIntervalFlag = false;
}

void Controller::updateSound() {
    int chan = mHBController.chan;

    if (!remotespk->isPlaying(chan)) {
        // Sound has stopped: arm the interval check, and once it has been
        // silent for one second clear the sound-activity flags.
        if (mCheckSoundTimeFlag) {
            if (!mCheckSoundIntervalFlag) {
                mStopSoundTime = OSGetTime();
                mCheckSoundIntervalFlag = true;
            } else if (OS_TICKS_TO_MSEC((u32)OSGetTime() - (u32)mStopSoundTime) >=
                       1000) {
                mCheckSoundTimeFlag = false;
                mCheckSoundIntervalFlag = false;
            }
        }
    } else {
        // Sound is still playing. After a long play time, or when the
        // remote's radio sensitivity is poor, mute the speaker briefly.
        if (mCheckSoundTimeFlag) {
            mCheckSoundIntervalFlag = false;
            if (OS_TICKS_TO_MSEC((u32)OSGetTime() - (u32)mPlaySoundTime) >=
                480000) {
                mCheckSoundTimeFlag = false;
                mCheckSoundIntervalFlag = false;

                int chan = mHBController.chan;
                if (WPADIsSpeakerEnabled(chan)) {
                    WPADControlSpeaker(chan, WPAD_SPEAKER_MUTE, NULL);

                    OSAlarm* alarm = &sAlarmSoundOff[chan];
                    OSSetAlarmUserData(alarm, reinterpret_cast<void*>(chan));
                    OSCancelAlarm(alarm);
                    OSSetAlarm(alarm, OS_MSEC_TO_TICKS(1000),
                               soundOnCallback);

                    mSoundOffFlag = true;
                }
                return;
            }
        }

        if (!mSoundOffFlag) {
            if (WPADGetRadioSensitivity(chan) <= 85) {
                int chan = mHBController.chan;
                if (WPADIsSpeakerEnabled(chan)) {
                    WPADControlSpeaker(chan, WPAD_SPEAKER_MUTE, NULL);

                    OSAlarm* alarm = &sAlarmSoundOff[chan];
                    OSSetAlarmUserData(alarm, reinterpret_cast<void*>(chan));
                    OSCancelAlarm(alarm);
                    OSSetAlarm(alarm, OS_MSEC_TO_TICKS(1000),
                               soundOnCallback);

                    mSoundOffFlag = true;
                }
            }
        }
    }
}

bool Controller::isPlayReady() const {
    return remotespk->isPlayReady(mHBController.chan);
}

HBController* Controller::getController() {
    return &mHBController;
}

void Controller::startMotor() {
    if (mHBController.chan < WPAD_MAX_CONTROLLERS && mRumbleFlag &&
        !remotespk->isPlaying(mHBController.chan)) {
        mHBController.rumble = true;
        WPADControlMotor(mHBController.chan, WPAD_MOTOR_RUMBLE);
    }
}

void Controller::stopMotor() {
    if (mHBController.chan < WPAD_MAX_CONTROLLERS && isRumbling()) {
        clrRumble();
        WPADControlMotor(mHBController.chan, WPAD_MOTOR_STOP);
    }
}

s32 Controller::getInfoAsync(WPADInfo* pInfo) {
    if (mHBController.chan >= WPAD_MAX_CONTROLLERS) {
        return -2;
    }

    if (remotespk->isPlaying(mHBController.chan) || isRumbling()) {
        return -2;
    }

    s32 chan = mHBController.chan;
    if (chan < WPAD_MAX_CONTROLLERS) {
        sSetInfoAsync[chan] = true;
    }

    return WPADGetInfoAsync(mHBController.chan, pInfo, ControllerCallback);
}

void Controller::ControllerCallback(s32 chan, s32 result) {
    if (result == WPAD_ERR_OK && chan < WPAD_MAX_CONTROLLERS) {
        sBatteryFlag[chan] = true;
    }
    if (chan < WPAD_MAX_CONTROLLERS) {
        sSetInfoAsync[chan] = false;
    }
}

bool Controller::getBatteryFlag() const {
    if (mHBController.chan >= 4) {
        return false;
    }
    return sBatteryFlag[mHBController.chan];
}

void Controller::clrBatteryFlag() {
    if (mHBController.chan >= WPAD_MAX_CONTROLLERS) {
        return;
    }

    sBatteryFlag[mHBController.chan] = false;
}

} // namespace homebutton
