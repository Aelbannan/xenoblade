#include <nw4r/snd.h>
#include <nw4r/ut.h>

// Shared retail sdata2 float pool entries referenced by InitParam.
extern "C" const f32 lbl_eu_8066A018; // 1.0f
extern "C" const f32 lbl_eu_8066A01C; // 0.0f

namespace nw4r {
namespace snd {
namespace detail {

void SeqTrack::SetPlayerTrackNo(int no) {
    mPlayerTrackNo = no;
}

// NOTE: the header's member offsets past 0x20 do not match retail; the
// functions below address the retail layout through explicit offsets
// (track channel list at +0xC4, link at Channel+0xF4, variables at +0xA0).
SeqTrack::SeqTrack() {
    mOpenFlag = false;

    // Embedded LfoParam sub-object at +0x58 (retail layout).
    ((LfoParam*)((u8*)this + 0x58))->Init();

    u8* p = (u8*)this;
    // Three 6-byte {u8,u8,s16,s16} records at +0x70, default-cleared.
    *(u8*)(p + 0x70) = 0;
    *(u8*)(p + 0x71) = 0;
    *(s16*)(p + 0x72) = 0;
    *(s16*)(p + 0x74) = 0;
    *(u8*)(p + 0x76) = 0;
    *(u8*)(p + 0x77) = 0;
    *(s16*)(p + 0x78) = 0;
    *(s16*)(p + 0x7a) = 0;
    *(u8*)(p + 0x7c) = 0;
    *(u8*)(p + 0x7d) = 0;
    *(s16*)(p + 0x7e) = 0;
    *(s16*)(p + 0x80) = 0;

    *(SeqPlayer**)((u8*)this + 0xC0) = NULL; // mPlayer
    *(Channel**)((u8*)this + 0xC4) = NULL;   // track channel list

    InitParam();
}

SeqTrack::~SeqTrack() {
    Close();
}

void SeqTrack::InitParam() {
    u8* p = (u8*)this;

    mExtVolume = lbl_eu_8066A018;
    mExtPitch = lbl_eu_8066A018;
    mExtPan = lbl_eu_8066A01C;
    mExtSurroundPan = lbl_eu_8066A01C;
    mPanRange = lbl_eu_8066A018;
    // Retail stores bit-pattern zero via integer registers here.
    *(s32*)(p + 0x1C) = 0;
    *(s32*)(p + 0x20) = 0;

    *(u8*)(p + 0x24) = 1;
    *(u8*)(p + 0x25) = 1;
    *(u8*)(p + 0x26) = 0;
    *(u8*)(p + 0x27) = 0;

    *(u8*)(p + 0x40) = 0;
    *(s32*)(p + 0x44) = 0;
    *(u8*)(p + 0x48) = 0;
    *(u8*)(p + 0x49) = 0;
    *(u8*)(p + 0x4a) = 0;
    *(u8*)(p + 0x4b) = 0;
    *(u8*)(p + 0x4c) = 0;
    *(s32*)(p + 0x50) = 0;
    *(s32*)(p + 0x54) = 0;

    ((LfoParam*)(p + 0x58))->Init();

    *(u8*)(p + 0x68) = 0;
    *(f32*)(p + 0x6c) = lbl_eu_8066A01C;
    *(u8*)(p + 0x70) = 127;
    *(u8*)(p + 0x71) = 127;
    *(s16*)(p + 0x72) = 0;
    *(s16*)(p + 0x74) = 0;
    *(u8*)(p + 0x76) = 0;
    *(u8*)(p + 0x77) = 0;
    *(s16*)(p + 0x78) = 0;
    *(s16*)(p + 0x7a) = 0;
    *(u8*)(p + 0x7c) = 0;
    *(u8*)(p + 0x7d) = 0;
    *(s16*)(p + 0x7e) = 0;
    *(s16*)(p + 0x80) = 0;
    *(u8*)(p + 0x82) = 127;
    *(u8*)(p + 0x83) = 127;
    *(u8*)(p + 0x84) = 0;
    *(u8*)(p + 0x85) = 2;
    *(u8*)(p + 0x86) = 0;
    *(u8*)(p + 0x87) = 0;
    *(u8*)(p + 0x88) = 64;
    *(u8*)(p + 0x89) = 60;
    *(u8*)(p + 0x8a) = 0;
    *(u8*)(p + 0x8b) = 255;
    *(u8*)(p + 0x8c) = 255;
    *(u8*)(p + 0x8d) = 255;
    *(u8*)(p + 0x8e) = 255;
    *(s16*)(p + 0x90) = 255;
    *(u8*)(p + 0x92) = 127;
    *(u8*)(p + 0x93) = 0;
    *(u8*)(p + 0x94) = 0;
    *(u8*)(p + 0x95) = 0;
    *(f32*)(p + 0x98) = lbl_eu_8066A01C;
    *(u8*)(p + 0x96) = 0;
    *(f32*)(p + 0x9c) = lbl_eu_8066A01C;

    volatile s16* variables = (volatile s16*)(p + 0xA0);
    for (int i = 0; i < VARIABLE_NUM; i++) {
        variables[i] = DEFAULT_VARIABLE_VALUE;
    }
}

void SeqTrack::SetSeqData(const void* pBase, s32 offset) {
    const u8* base = static_cast<const u8*>(pBase);
    *(const u8**)((u8*)this + 0x1C) = base;
    *(const u8**)((u8*)this + 0x20) = base + offset;
}

void SeqTrack::Open() {
    *(u8*)((u8*)this + 0x4A) = false;
    *(u8*)((u8*)this + 0x40) = false;
    *(s32*)((u8*)this + 0x44) = 0;
    mOpenFlag = true;
}

void SeqTrack::Close() {
    SoundThread::AutoLock lock;

    ReleaseAllChannel(-1);
    FreeAllChannel();

    mOpenFlag = false;
}

void SeqTrack::UpdateChannelLength() {
    SoundThread::AutoLock lock;

    if (!mOpenFlag) {
        return;
    }

    // Track channel list at +0xC4; per-channel next link at +0xF4.
    for (Channel* pIt = *(Channel**)((u8*)this + 0xC4); pIt != NULL;
         pIt = *(Channel**)((u8*)pIt + 0xF4)) {

        if (pIt->GetLength() > 0) {
            pIt->SetLength(pIt->GetLength() - 1);
        }

        // Note-off pending channels unless already released or held by damper.
        if (pIt->GetLength() == 0 && !pIt->IsRelease() &&
            *(u8*)((u8*)this + 0x4C) == 0) {
            pIt->NoteOff();
        }

        if (!pIt->IsAutoUpdateSweep()) {
            pIt->UpdateSweep(1);
        }
    }
}

void SeqTrack::UpdateChannelRelease(Channel* pChannel) {
    SoundThread::AutoLock lock;

    if (pChannel->GetLength() == 0 && !pChannel->IsRelease() &&
        !mParserTrackParam.damperFlag) {

        pChannel->Release();
    }
}

int SeqTrack::ParseNextTick(bool doNoteOn) {
    SoundThread::AutoLock lock;

    if (!mOpenFlag) {
        return 0;
    }

    u8* p = (u8*)this;

    // Advance three bounded s16 ramp counters.
    if (*(s16*)(p + 0x74) < *(s16*)(p + 0x72)) {
        (*(s16*)(p + 0x74))++;
    }
    if (*(s16*)(p + 0x7A) < *(s16*)(p + 0x78)) {
        (*(s16*)(p + 0x7A))++;
    }
    if (*(s16*)(p + 0x80) < *(s16*)(p + 0x7e)) {
        (*(s16*)(p + 0x80))++;
    }

    // Waiting for running notes to finish before parsing further events.
    if (*(u8*)(p + 0x4A)) {
        if (*(Channel**)(p + 0xC4) != NULL) {
            return 1;
        }

        *(u8*)(p + 0x4A) = false;
    }

    // Tick wait countdown: keep returning while wait remains positive.
    if (*(s32*)(p + 0x44) > 0 && --*(s32*)(p + 0x44) > 0) {
        return 1;
    }

    if (*(const u8**)(p + 0x20) != NULL) {
        while (*(s32*)(p + 0x44) == 0 && *(u8*)(p + 0x4A) == 0) {
            if (Parse(doNoteOn) == PARSE_RESULT_FINISH) {
                return -1;
            }
        }
    }

    return 1;
}

void SeqTrack::StopAllChannel() {
    SoundThread::AutoLock lock;

    Channel** channelList = (Channel**)((u8*)this + 0xC4);
    for (Channel* pIt = *channelList; pIt != NULL;
         pIt = *(Channel**)((u8*)pIt + 0xF4)) {

        Channel::FreeChannel(pIt);
        pIt->Stop();
    }

    *channelList = NULL;
}

void SeqTrack::ReleaseAllChannel(int release) {
    SoundThread::AutoLock lock;

    UpdateChannelParam();

    Channel* channelList = *(Channel**)((u8*)this + 0xC4);
    for (Channel* pIt = channelList; pIt != NULL;
         pIt = *(Channel**)((u8*)pIt + 0xF4)) {

        if (pIt->IsActive()) {
            if (release >= 0) {
                pIt->SetRelease(static_cast<u8>(release));
            }

            pIt->Release();
        }
    }
}

void SeqTrack::PauseAllChannel(bool flag) {
    SoundThread::AutoLock lock;

    Channel* channelList = *(Channel**)((u8*)this + 0xC4);
    for (Channel* pIt = channelList; pIt != NULL;
         pIt = *(Channel**)((u8*)pIt + 0xF4)) {

        if (pIt->IsActive() && flag != pIt->IsPause()) {
            pIt->Pause(flag);
        }
    }
}

void SeqTrack::AddChannel(Channel* pChannel) {
    SoundThread::AutoLock lock;

    pChannel->SetNextTrackChannel(mChannelList);
    mChannelList = pChannel;
}

void SeqTrack::UpdateChannelParam() {
    SoundThread::AutoLock lock;

    if (!mOpenFlag) {
        return;
    }

    if (mChannelList == NULL) {
        return;
    }

    f32 volume = 1.0f;
    f32 parserVolume = mParserTrackParam.volume / 127.0f;
    f32 parserVolume2 = mParserTrackParam.volume2 / 127.0f;
    f32 parserMainVolume = mPlayer->GetParserPlayerParam().volume / 127.0f;

    volume *= (parserVolume * parserVolume);
    volume *= (parserVolume2 * parserVolume2);
    volume *= (parserMainVolume * parserMainVolume);
    volume *= mExtVolume;
    volume *= mPlayer->GetVolume();

    f32 pitch =
        (mParserTrackParam.pitchBend / 128.0f) * mParserTrackParam.bendRange;

    f32 pitchRatio = 1.0f;
    pitchRatio *= mPlayer->GetPitch();
    pitchRatio *= mExtPitch;

    f32 pan = 0.0f;
    pan += ut::Clamp(mParserTrackParam.pan / 63.0f, -1.0f, 1.0f);
    pan *= mPanRange;
    pan *= mPlayer->GetPanRange();
    pan += mExtPan;
    pan += mPlayer->GetPan();

    f32 surroundPan = 0.0f;
    surroundPan += ut::Clamp(mParserTrackParam.surroundPan / 63.0f, 0.0f, 2.0f);
    surroundPan += mExtSurroundPan;
    surroundPan += mPlayer->GetSurroundPan();

    f32 lpfFreq = 0.0f;
    lpfFreq += (mParserTrackParam.lpfFreq - 64) / 64.0f;
    lpfFreq += mExtLpfFreq;
    lpfFreq += mPlayer->GetLpfFreq();

    int remoteFilter = 0;
    remoteFilter += mPlayer->GetRemoteFilter();

    f32 mainSend = 0.0f;
    mainSend += (mParserTrackParam.mainSend / 127.0f) - 1.0f;
    mainSend += mExtMainSend;
    mainSend += mPlayer->GetMainSend();

    f32 fxSend[AUX_BUS_NUM];
    for (int i = 0; i < AUX_BUS_NUM; i++) {
        fxSend[i] = 0.0f;
        fxSend[i] += mParserTrackParam.fxSend[i] / 127.0f;
        fxSend[i] += mExtFxSend[i];
        fxSend[i] += mPlayer->GetFxSend(static_cast<AuxBus>(i));
    }

    f32 remoteSend[WPAD_MAX_CONTROLLERS];
    f32 remoteFxSend[WPAD_MAX_CONTROLLERS];
    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        remoteSend[i] = 0.0f;
        remoteSend[i] += mPlayer->GetRemoteSend(i);

        remoteFxSend[i] = 0.0f;
        remoteFxSend[i] += mPlayer->GetRemoteFxSend(i);
    }

    for (Channel* pIt = mChannelList; pIt != NULL;
         pIt = pIt->GetNextTrackChannel()) {

        pIt->SetUserVolume(volume);
        pIt->SetUserPitch(pitch);
        pIt->SetUserPitchRatio(pitchRatio);
        pIt->SetUserPan(pan);
        pIt->SetUserSurroundPan(surroundPan);
        pIt->SetUserLpfFreq(lpfFreq);
        pIt->SetRemoteFilter(remoteFilter);
        pIt->SetOutputLine(mPlayer->GetOutputLine());
        pIt->SetMainOutVolume(mPlayer->GetMainOutVolume());
        pIt->SetMainSend(mainSend);

        for (int i = 0; i < AUX_BUS_NUM; i++) {
            pIt->SetFxSend(static_cast<AuxBus>(i), fxSend[i]);
        }

        for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            pIt->SetRemoteOutVolume(i, mPlayer->GetRemoteOutVolume(i));
            pIt->SetRemoteSend(i, remoteSend[i]);
            pIt->SetRemoteFxSend(i, remoteFxSend[i]);
        }

        pIt->SetLfoParam(mParserTrackParam.lfoParam);
        pIt->SetLfoTarget(
            static_cast<Channel::LfoTarget>(mParserTrackParam.lfoTarget));
    }
}

void SeqTrack::FreeAllChannel() {
    SoundThread::AutoLock lock;

    Channel** channelList = (Channel**)((u8*)this + 0xC4);
    for (Channel* pIt = *channelList; pIt != NULL;
         pIt = *(Channel**)((u8*)pIt + 0xF4)) {

        Channel::FreeChannel(pIt);
    }

    *channelList = NULL;
}

void SeqTrack::ChannelCallbackFunc(Channel* pDropChannel,
                                   Channel::ChannelCallbackStatus status,
                                   u32 callbackArg) {
    SoundThread::AutoLock lock;
    SeqTrack* p = reinterpret_cast<SeqTrack*>(callbackArg);

    switch (status) {
    case Channel::CALLBACK_STATUS_STOPPED:
    case Channel::CALLBACK_STATUS_FINISH: {
        Channel::FreeChannel(pDropChannel);
        break;
    }

    default: {
        break;
    }
    }

    SeqPlayer* player = *(SeqPlayer**)((u8*)p + 0xC0);
    if (player != NULL) {
        player->ChannelCallback(pDropChannel);
    }

    Channel** channelList = (Channel**)((u8*)p + 0xC4);
    if (*channelList == pDropChannel) {
        *channelList = *(Channel**)((u8*)pDropChannel + 0xF4);
        return;
    }

    for (Channel* pIt = *channelList;
         *(Channel**)((u8*)pIt + 0xF4) != NULL;
         pIt = *(Channel**)((u8*)pIt + 0xF4)) {

        if (*(Channel**)((u8*)pIt + 0xF4) == pDropChannel) {
            *(Channel**)((u8*)pIt + 0xF4) =
                *(Channel**)((u8*)pDropChannel + 0xF4);
            return;
        }
    }
}

void SeqTrack::SetMute(SeqMute mute) {
    SoundThread::AutoLock lock;

    u8* muteFlag = (u8*)this + 0x48;
    switch (mute) {
    case MUTE_OFF: {
        *muteFlag = false;
        break;
    }

    case MUTE_STOP: {
        StopAllChannel();
        *muteFlag = true;
        break;
    }

    case MUTE_RELEASE: {
        ReleaseAllChannel(-1);
        FreeAllChannel();
        *muteFlag = true;
        break;
    }

    case MUTE_NO_STOP: {
        *muteFlag = true;
        break;
    }
    }
}

void SeqTrack::SetVolume(f32 volume) {
    mExtVolume = volume;
}

void SeqTrack::SetPitch(f32 pitch) {
    mExtPitch = pitch;
}

volatile s16* SeqTrack::GetVariablePtr(int idx) {
    if (idx < VARIABLE_NUM) {
        volatile s16* variables = (volatile s16*)((u8*)this + 0xA0);
        return &variables[idx];
    }

    return NULL;
}

Channel* SeqTrack::NoteOn(int key, int velocity, s32 length, bool tie) {
    SoundThread::AutoLock lock;

    SeqPlayer* pPlayer = GetSeqPlayer();
    Channel* pChannel = NULL;

    if (tie) {
        pChannel = GetLastChannel();
        if (pChannel != NULL) {
            pChannel->SetKey(static_cast<u8>(key));

            f32 initVolume = velocity / 127.0f;
            pChannel->SetInitVolume(initVolume * initVolume);
        }
    }

    if (pChannel == NULL) {
        NoteOnInfo info = {
            mParserTrackParam.prgNo,   // prgNo
            key,                       // key
            velocity,                  // velocity
            tie ? -1 : length,         // length
            mParserTrackParam.initPan, // initPan

            pPlayer->GetParserPlayerParam().priority + // priority
                GetParserTrackParam().priority,

            mPlayer->GetVoiceOutCount(), // voiceOutCount
            ChannelCallbackFunc,         // channelCallback
            reinterpret_cast<u32>(this)  // channelCallbackData
        };

        pChannel = mPlayer->NoteOn(mParserTrackParam.bankNo, info);
        if (pChannel == NULL) {
            return NULL;
        }

        AddChannel(pChannel);
    }

    if (mParserTrackParam.attack != 0xFF) {
        pChannel->SetAttack(mParserTrackParam.attack);
    }
    if (mParserTrackParam.decay != 0xFF) {
        pChannel->SetDecay(mParserTrackParam.decay);
    }
    if (mParserTrackParam.sustain != 0xFF) {
        pChannel->SetSustain(mParserTrackParam.sustain);
    }
    if (mParserTrackParam.release != 0xFF) {
        pChannel->SetRelease(mParserTrackParam.release);
    }

    f32 sweepPitch = mParserTrackParam.sweepPitch;
    if (mParserTrackParam.portaFlag) {
        sweepPitch += mParserTrackParam.portaKey - key;
    }

    if (mParserTrackParam.portaTime == 0) {
        pChannel->SetSweepParam(sweepPitch, length, false);
    } else {
        int time = mParserTrackParam.portaTime;
        time *= time;
        time *= sweepPitch >= 0.0f ? sweepPitch : -sweepPitch;
        time = static_cast<u32>(time >> 5);
        time = static_cast<u32>(time << 2) + time;
        pChannel->SetSweepParam(sweepPitch, time, true);
    }

    mParserTrackParam.portaKey = key;

    pChannel->SetSilence(mParserTrackParam.silenceFlag != 0, 0);
    pChannel->SetReleasePriorityFix(mPlayer->IsReleasePriorityFix());
    pChannel->SetPanMode(mPlayer->GetPanMode());
    pChannel->SetPanCurve(mPlayer->GetPanCurve());

    return pChannel;
}

} // namespace detail
} // namespace snd
} // namespace nw4r

extern "C" u32 detail_ConvertLabelStringToSoundId__Q34nw4r3snd18SoundArchivePlayerFPCc(nw4r::snd::SoundArchivePlayer* self, const char* str) {
    return ConvertLabelStringToSoundId__Q34nw4r3snd12SoundArchiveCFPCc(*(void**)((u8*)self + 0x10), str);
}
