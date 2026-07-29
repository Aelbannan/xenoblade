#include <homebuttonLib.h>

#include <revolution/ARC.h>
#include <revolution/OS.h>
#include <revolution/WENC.h>
#include <revolution/WPAD.h>

#include <cstring>

namespace homebutton {

RemoteSpk* RemoteSpk::spInstance = NULL;

void RemoteSpk::SetInstance(RemoteSpk* pThis) {
    spInstance = pThis;
}

RemoteSpk* RemoteSpk::GetInstance() {
    return spInstance;
}

void RemoteSpk::GetPCMFromSeID(int id, s16*& rpPcm, int& rLength) {
    ARCFileInfo af;
    ARCFastOpen(&handle, id, &af);

    rpPcm = static_cast<s16*>(ARCGetStartAddrInMem(&af));
    rLength = ARCGetLength(&af);

    ARCClose(&af);
}

void RemoteSpk::UpdateSpeaker(OSAlarm* /* pAlarm */, OSContext* /* pContext */) {
    s16 pcmBuffer[40];
    u8 adpcmBuffer[20];

    RemoteSpk* pRmtSpk = GetInstance();
    if (pRmtSpk == NULL) {
        return;
    }

    ChanInfo* pInfo = pRmtSpk->info;

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++, pInfo++) {
        if (pInfo->in_pcm != NULL && WPADIsSpeakerEnabled(i)) {
            BOOL intrStatus = OSDisableInterrupts();

            if (WPADCanSendStreamData(i)) {
                int samples = (int)((u32)pInfo->length >> 1);
                int encSize = 40;
                const s16* pSrc = pInfo->in_pcm;
                s16* pDst = pcmBuffer;
                s8 vol = pInfo->vol;

                if (samples <= 40) {
                    encSize = samples;
                }

                if (encSize != 0) {
                    for (int j = encSize; j > 0; j--) {
                        *pDst++ = static_cast<s16>(*pSrc++ * vol / 10);
                    }
                }

                if (samples <= 40) {
                    for (int j = 40 - samples; j > 0; j--) {
                        *pDst++ = 0;
                    }
                }

                WENCGetEncodeData(&pInfo->wencinfo, !pInfo->first, pcmBuffer,
                                  40, adpcmBuffer);
                WPADSendStreamData(i, adpcmBuffer, 20);

                pInfo->first = false;
                pInfo->cannotSendCnt = 0;
                pInfo->in_pcm += 40;
                pInfo->length -= 80;

                if (pInfo->length <= 0) {
                    pInfo->seId = -1;
                    pInfo->in_pcm = NULL;
                }
            } else {
                pInfo->cannotSendCnt++;
                if (pInfo->cannotSendCnt > 60) {
                    pInfo->in_pcm = NULL;
                }
            }

            OSRestoreInterrupts(intrStatus);
        }
    }
}

void RemoteSpk::ClearPcm() {
    info[0].in_pcm = NULL;
    info[0].seId = -1;
    info[1].in_pcm = NULL;
    info[1].seId = -1;
    info[2].in_pcm = NULL;
    info[2].seId = -1;
    info[3].in_pcm = NULL;
    info[3].seId = -1;
}

RemoteSpk::RemoteSpk(void* pSpkSeBuf) {
    SetInstance(this);

    if (pSpkSeBuf != NULL) {
        available = ARCInitHandle(pSpkSeBuf, &handle);
    } else {
        available = false;
    }

    OSCreateAlarm(&speakerAlarm);

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        OSCreateAlarm(&info[i].alarm);
        info[i].in_pcm = NULL;
        info[i].seId = -1;
        info[i].first = true;
        info[i].playReady = true;
    }
}

RemoteSpk::~RemoteSpk() {
    SetInstance(NULL);
    available = false;

    OSCancelAlarm(&speakerAlarm);

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        OSCancelAlarm(&info[i].alarm);
    }
}

void RemoteSpk::Start() {
    if (!available) {
        return;
    }

    OSCreateAlarm(&speakerAlarm);

    OSSetPeriodicAlarm(&speakerAlarm, OSGetTime(), OS_NSEC_TO_TICKS(6666667),
                       &UpdateSpeaker);

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        OSCreateAlarm(&info[i].alarm);
        info[i].in_pcm = NULL;
        info[i].seId = -1;
        info[i].first = true;
        info[i].playReady = true;
    }
}

void RemoteSpk::Stop() {
    OSCancelAlarm(&speakerAlarm);

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        OSCancelAlarm(&info[i].alarm);
    }
}

void RemoteSpk::DelaySpeakerOnCallback(OSAlarm* pAlarm,
                                       OSContext* /* pContext */) {
    s32 chan = reinterpret_cast<s32>(OSGetAlarmUserData(pAlarm));
    s32 result = WPADControlSpeaker(chan, WPAD_SPEAKER_ON, &SpeakerOnCallback);
}

void RemoteSpk::SpeakerOnCallback(s32 chan, s32 result) {
    RemoteSpk* pRmtSpk = GetInstance();
    if (pRmtSpk == NULL) {
        return;
    }

    switch (result) {
    case WPAD_ERR_OK:
        pRmtSpk->info[chan].first = true;
        WPADControlSpeaker(chan, WPAD_SPEAKER_PLAY, SpeakerOffCallback);
        break;
    case WPAD_ERR_TRANSFER:
        pRmtSpk->info[chan].playReady = false;
        break;
    case WPAD_ERR_COMMUNICATION_ERROR:
        OSSetAlarmUserData(&pRmtSpk->info[chan].alarm,
                           reinterpret_cast<void*>(chan));
        OSCancelAlarm(&pRmtSpk->info[chan].alarm);
        OSSetAlarm(&pRmtSpk->info[chan].alarm, OS_MSEC_TO_TICKS(50),
                   DelaySpeakerOnCallback);
        break;
    }
}

void RemoteSpk::DelaySpeakerPlayCallback(OSAlarm* pAlarm,
                                         OSContext* /* pContext */) {
    s32 chan = reinterpret_cast<s32>(OSGetAlarmUserData(pAlarm));
    s32 result =
        WPADControlSpeaker(chan, WPAD_SPEAKER_PLAY, &SpeakerPlayCallback);
}

void RemoteSpk::SpeakerPlayCallback(s32 chan, s32 result) {
    RemoteSpk* pRmtSpk = GetInstance();
    if (pRmtSpk == NULL) {
        return;
    }

    switch (result) {
    case WPAD_ERR_OK: {
        pRmtSpk->info[chan].playReady = true;
        break;
    }

    case WPAD_ERR_NO_CONTROLLER: {
        pRmtSpk->info[chan].playReady = false;
        break;
    }

    case WPAD_ERR_COMMUNICATION_ERROR: {
        OSSetAlarmUserData(&pRmtSpk->info[chan].alarm,
                           reinterpret_cast<void*>(chan));

        OSCancelAlarm(&pRmtSpk->info[chan].alarm);
        OSSetAlarm(&pRmtSpk->info[chan].alarm, OS_MSEC_TO_TICKS(50),
                   &DelaySpeakerPlayCallback);
        break;
    }
    }
}

void RemoteSpk::Connect(s32 chan) {
    if (!available) {
        return;
    }

    int result = WPADControlSpeaker(chan, WPAD_SPEAKER_ON, &SpeakerOnCallback);

    std::memset(&info[chan].wencinfo, 0, sizeof(WENCInfo));
    info[chan].first = true;
    info[chan].playReady = false;
}

void RemoteSpk::Play(s32 chan, int seID, s8 vol) {
    if (!available) {
        return;
    }

    s16* pPcm;
    int length;
    GetPCMFromSeID(seID, pPcm, length);

    info[chan].cannotSendCnt = 0;
    info[chan].seId = seID;
    info[chan].length = length;
    info[chan].vol = vol;
    info[chan].in_pcm = pPcm;
}

bool homebutton::RemoteSpk::isPlaying(s32 chan) const {
    return info[chan].in_pcm != NULL;
}

bool RemoteSpk::isPlayingId(s32 chan, int seId) const {
    if (isPlaying(chan) && info[chan].seId == seId) {
        return true;
    }

    return false;
}

namespace homebutton {

bool RemoteSpk::isPlayReady(s32 chan) const {
    return info[chan].playReady != false;
}

void RemoteSpk::SpeakerOffCallback(s32 chan, s32 result) {
    RemoteSpk* pRmtSpk = GetInstance();
    if (pRmtSpk == NULL) {
        return;
    }

    switch (result) {
    case WPAD_ERR_OK:
        pRmtSpk->info[chan].playReady = true;
        break;
    case WPAD_ERR_TRANSFER:
        pRmtSpk->info[chan].playReady = false;
        break;
    case WPAD_ERR_NO_CONTROLLER:
        pRmtSpk->info[chan].playReady = false;
        break;
    case WPAD_ERR_COMMUNICATION_ERROR:
        OSSetAlarmUserData(&pRmtSpk->info[chan].alarm,
                           reinterpret_cast<void*>(chan));
        OSCancelAlarm(&pRmtSpk->info[chan].alarm);
        OSSetAlarm(&pRmtSpk->info[chan].alarm, OS_MSEC_TO_TICKS(50),
                   DelaySpeakerOffCallback);
        break;
    }
}

void RemoteSpk::DelaySpeakerOffCallback(OSAlarm* pAlarm,
                                        OSContext* /* pContext */) {
    s32 chan = reinterpret_cast<s32>(OSGetAlarmUserData(pAlarm));
    WPADControlSpeaker(chan, WPAD_SPEAKER_PLAY, &SpeakerOffCallback);
}

void RemoteSpk::Disconnect(s32 chan) {
    if (!available) {
        return;
    }

    WPADControlSpeaker(chan, WPAD_SPEAKER_ON, &SpeakerOnCallback);

    // Zero the entire WENCInfo struct (8 words = 32 bytes)
    u32* pWenc = reinterpret_cast<u32*>(&info[chan].wencinfo);
    pWenc[0] = 0;
    pWenc[1] = 0;
    pWenc[2] = 0;
    pWenc[3] = 0;
    pWenc[4] = 0;
    pWenc[5] = 0;
    pWenc[6] = 0;
    pWenc[7] = 0;

    info[chan].first = true;
    info[chan].playReady = false;
}

} // namespace homebutton

} // namespace homebutton
