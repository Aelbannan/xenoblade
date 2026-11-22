#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

namespace {

// The MML envelope-value commands accept the same 0..127 range as the retail
// guard (`<= 127`, not `!= 0xFF`), so keep one shared bound.
const int ENV_VALUE_MAX = 127;

} // namespace

void SeqTrack::SetPlayerTrackNo(int no) {
    mPlayerTrackNo = no;
}

SeqTrack::SeqTrack() : mOpenFlag(false) {
    mParserTrackParam.volume.InitValue(0);
    mParserTrackParam.pan.InitValue(0);
    mParserTrackParam.surroundPan.InitValue(0);

    mPlayer = NULL;
    mChannelList = NULL;

    InitParam();
}

SeqTrack::~SeqTrack() {
    Close();
}

void SeqTrack::InitParam() {
    mExtVolume = 1.0f;
    mExtPitch = 1.0f;
    mExtPan = 0.0f;
    mExtSurroundPan = 0.0f;
    mPanRange = 1.0f;

    ParserTrackParam& rParam = mParserTrackParam;

    // Retail stores the two sequence cursors with integer registers.
    rParam.baseAddr = NULL;
    rParam.currentAddr = NULL;

    rParam.mmlParserParam.cmpFlag = true;
    rParam.mmlParserParam.noteWaitFlag = true;
    rParam.mmlParserParam.tieFlag = false;
    rParam.mmlParserParam.monophonicFlag = false;

    rParam.mmlParserParam.callStackDepth = 0;
    rParam.wait = 0;

    rParam.muteFlag = false;
    rParam.silenceFlag = false;
    rParam.noteFinishWait = false;
    rParam.portaFlag = false;
    rParam.damperFlag = false;

    rParam.bankNo = 0;
    rParam.prgNo = 0;

    rParam.lfoParam.Init();

    rParam.lfoTarget = 0;
    rParam.sweepPitch = 0.0f;

    rParam.volume.InitValue(DEFAULT_VOLUME);
    rParam.pan.InitValue(0);
    rParam.surroundPan.InitValue(0);

    rParam.volume2 = DEFAULT_VOLUME;
    rParam.velocityScale = DEFAULT_VOLUME;
    rParam.pitchBend = 0;
    rParam.bendRange = DEFAULT_BENDRANGE;
    rParam.initPan = 0;
    rParam.transpose = 0;
    rParam.priority = DEFAULT_PRIORITY;
    rParam.portaKey = DEFAULT_PORTA_KEY;
    rParam.portaTime = 0;
    rParam.attack = DEFAULT_ENV;
    rParam.decay = DEFAULT_ENV;
    rParam.sustain = DEFAULT_ENV;
    rParam.release = DEFAULT_ENV;
    rParam.hold = DEFAULT_ENV;

    rParam.mainSend = DEFAULT_MAINSEND;
    rParam.fxSend[AUX_A] = 0;
    rParam.fxSend[AUX_B] = 0;
    rParam.fxSend[AUX_C] = 0;

    rParam.lpfFreq = 0.0f;
    rParam.biquadType = 0;
    rParam.biquadFreq = 0.0f;

    volatile s16* pVariable = mTrackVariable;
    for (int i = 0; i < VARIABLE_NUM; i++) {
        pVariable[i] = DEFAULT_VARIABLE_VALUE;
    }
}

void SeqTrack::SetSeqData(const void* pBase, s32 offset) {
    const u8* base = static_cast<const u8*>(pBase);

    mParserTrackParam.baseAddr = base;
    mParserTrackParam.currentAddr = base + offset;
}

void SeqTrack::Open() {
    mParserTrackParam.noteFinishWait = false;
    mParserTrackParam.mmlParserParam.callStackDepth = 0;
    mParserTrackParam.wait = 0;

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

    for (Channel* pIt = mChannelList; pIt != NULL;
         pIt = pIt->GetNextTrackChannel()) {

        if (pIt->GetLength() > 0) {
            pIt->SetLength(pIt->GetLength() - 1);
        }

        // Note-off pending channels unless already released or held by damper.
        if (pIt->GetLength() == 0 && !pIt->IsRelease() &&
            !mParserTrackParam.damperFlag) {

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

    // Advance the three value movers.
    mParserTrackParam.volume.Update();
    mParserTrackParam.pan.Update();
    mParserTrackParam.surroundPan.Update();

    // Waiting for running notes to finish before parsing further events.
    if (mParserTrackParam.noteFinishWait) {
        if (mChannelList != NULL) {
            return 1;
        }

        mParserTrackParam.noteFinishWait = false;
    }

    if (mParserTrackParam.wait > 0 && --mParserTrackParam.wait > 0) {
        return 1;
    }

    if (mParserTrackParam.currentAddr != NULL) {
        while (mParserTrackParam.wait == 0 &&
               !mParserTrackParam.noteFinishWait) {

            if (Parse(doNoteOn) == PARSE_RESULT_FINISH) {
                return -1;
            }
        }
    }

    return 1;
}

void SeqTrack::StopAllChannel() {
    SoundThread::AutoLock lock;

    for (Channel* pIt = mChannelList; pIt != NULL;
         pIt = pIt->GetNextTrackChannel()) {

        Channel::FreeChannel(pIt);
        pIt->Stop();
    }

    mChannelList = NULL;
}

void SeqTrack::ReleaseAllChannel(int release) {
    SoundThread::AutoLock lock;

    UpdateChannelParam();

    for (Channel* pIt = mChannelList; pIt != NULL;
         pIt = pIt->GetNextTrackChannel()) {

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

    for (Channel* pIt = mChannelList; pIt != NULL;
         pIt = pIt->GetNextTrackChannel()) {

        if (pIt->IsActive() && flag != pIt->IsPause()) {
            pIt->Pause(flag);
        }
    }
}

void SeqTrack::AddChannel(Channel* pChannel) {
    if (pChannel->GetTag() > 0) {
        for (Channel* pIt = mChannelList; pIt != NULL;
             pIt = pIt->GetNextTrackChannel()) {

            if (pIt->GetTag() == pChannel->GetTag()) {
                pIt->Release();
            }
        }
    }

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

    f32 parserVolume = mParserTrackParam.volume.GetValue() / 127.0f;
    f32 parserVolume2 = mParserTrackParam.volume2 / 127.0f;
    f32 parserMainVolume = mPlayer->GetParserPlayerParam().volume / 127.0f;

    volume *= (parserVolume * parserVolume);
    volume *= (parserVolume2 * parserVolume2);
    volume *= (parserMainVolume * parserMainVolume);
    volume *= mExtVolume;
    volume *= mPlayer->GetVolume();

    f32 pitch = (mParserTrackParam.pitchBend / 128.0f) * mParserTrackParam.bendRange;

    f32 pitchRatio = 1.0f;
    pitchRatio *= mPlayer->GetPitch();
    pitchRatio *= mExtPitch;

    f32 pan = 0.0f;
    pan += ut::Clamp(mParserTrackParam.pan.GetValue() / 63.0f, -1.0f, 1.0f);
    pan *= mPanRange;
    pan *= mPlayer->GetPanRange();
    pan += mExtPan;
    pan += mPlayer->GetPan();

    mParserTrackParam.pan.GetValue();

    f32 surroundPan = 0.0f;
    surroundPan +=
        ut::Clamp(mParserTrackParam.surroundPan.GetValue() / 63.0f, 0.0f, 2.0f);
    surroundPan += mExtSurroundPan;
    surroundPan += mPlayer->GetSurroundPan();

    f32 lpfFreq = 0.0f;
    lpfFreq += mParserTrackParam.lpfFreq;
    lpfFreq += mPlayer->GetLpfFreq();

    int biquadType = mParserTrackParam.biquadType;
    f32 biquadValue = mParserTrackParam.biquadFreq;

    if (mPlayer->GetBiquadFilterType() != 0) {
        biquadType = mPlayer->GetBiquadFilterType();
        biquadValue = mPlayer->GetBiquadFilterValue();
    }

    int remoteFilter = mPlayer->GetRemoteFilter();

    f32 mainSend = 0.0f;
    mainSend += (mParserTrackParam.mainSend / 127.0f) - 1.0f;
    mainSend += mPlayer->GetMainSend();

    f32 fxSend[AUX_BUS_NUM];
    for (int i = 0; i < AUX_BUS_NUM; i++) {
        fxSend[i] = 0.0f;
        fxSend[i] += mParserTrackParam.fxSend[i] / 127.0f;
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
        pIt->SetBiquadFilter(biquadType, biquadValue);
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

        for (int i = 0; i < mPlayer->GetVoiceOutCount(); i++) {
            pIt->SetVoiceOutParam(i, mPlayer->GetVoiceOutParam(i));
        }

        pIt->SetLfoParam(mParserTrackParam.lfoParam);
        pIt->SetLfoTarget(static_cast<Channel::LfoTarget>(mParserTrackParam.lfoTarget));
    }
}

void SeqTrack::FreeAllChannel() {
    SoundThread::AutoLock lock;

    for (Channel* pIt = mChannelList; pIt != NULL;
         pIt = pIt->GetNextTrackChannel()) {

        Channel::FreeChannel(pIt);
    }

    mChannelList = NULL;
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

    SeqPlayer* pPlayer = p->mPlayer;
    if (pPlayer != NULL) {
        pPlayer->ChannelCallback(pDropChannel);
    }

    if (p->mChannelList == pDropChannel) {
        p->mChannelList = pDropChannel->GetNextTrackChannel();
        return;
    }

    for (Channel* pIt = p->mChannelList; pIt->GetNextTrackChannel() != NULL;
         pIt = pIt->GetNextTrackChannel()) {

        if (pIt->GetNextTrackChannel() == pDropChannel) {
            pIt->SetNextTrackChannel(pDropChannel->GetNextTrackChannel());
            return;
        }
    }
}

void SeqTrack::SetMute(SeqMute mute) {
    SoundThread::AutoLock lock;

    switch (mute) {
    case MUTE_OFF: {
        mParserTrackParam.muteFlag = false;
        break;
    }

    case MUTE_STOP: {
        StopAllChannel();
        mParserTrackParam.muteFlag = true;
        break;
    }

    case MUTE_RELEASE: {
        ReleaseAllChannel(-1);
        FreeAllChannel();
        mParserTrackParam.muteFlag = true;
        break;
    }

    case MUTE_NO_STOP: {
        mParserTrackParam.muteFlag = true;
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
        return &mTrackVariable[idx];
    }

    return NULL;
}

Channel* SeqTrack::NoteOn(int key, int velocity, s32 length, bool tie) {
    SoundThread::AutoLock lock;

    ParserTrackParam& rParam = mParserTrackParam;

    SeqPlayer* pPlayer = GetSeqPlayer();
    Channel* pChannel = NULL;

    velocity = velocity * rParam.velocityScale / 127;

    if (tie) {
        pChannel = GetLastChannel();
        if (pChannel != NULL) {
            pChannel->SetKey(static_cast<u8>(key));

            f32 initVolume = velocity / 127.0f;
            pChannel->SetInitVolume(initVolume * initVolume);
        }
    }

    if (rParam.mmlParserParam.monophonicFlag) {
        pChannel = GetLastChannel();

        if (pChannel != NULL) {
            if (pChannel->IsRelease()) {
                pChannel->Stop();
                pChannel = NULL;
            } else {
                pChannel->SetKey(static_cast<u8>(key));

                f32 initVolume = velocity / 127.0f;
                pChannel->SetInitVolume(initVolume * initVolume);
                pChannel->SetLength(length);
            }
        }
    }

    if (pChannel == NULL) {
        NoteOnInfo info = {
            rParam.prgNo,   // prgNo
            key,            // key
            velocity,       // velocity
            tie ? -1 : length, // length
            rParam.initPan, // initPan

            pPlayer->GetParserPlayerParam().priority + // priority
                rParam.priority,

            mPlayer->GetVoiceOutCount(), // voiceOutCount
            ChannelCallbackFunc,         // channelCallback
            reinterpret_cast<u32>(this)  // channelCallbackData
        };

        pChannel = mPlayer->NoteOn(rParam.bankNo, info);
        if (pChannel == NULL) {
            return NULL;
        }

        AddChannel(pChannel);
    }

    if (rParam.attack <= ENV_VALUE_MAX) {
        pChannel->SetAttack(rParam.attack);
    }
    if (rParam.decay <= ENV_VALUE_MAX) {
        pChannel->SetDecay(rParam.decay);
    }
    if (rParam.sustain <= ENV_VALUE_MAX) {
        pChannel->SetSustain(rParam.sustain);
    }
    if (rParam.release <= ENV_VALUE_MAX) {
        pChannel->SetRelease(rParam.release);
    }
    if (rParam.hold <= ENV_VALUE_MAX) {
        pChannel->SetHold(rParam.hold);
    }

    f32 sweepPitch = rParam.sweepPitch;
    if (rParam.portaFlag) {
        sweepPitch += rParam.portaKey - key;
    }

    if (rParam.portaTime == 0) {
        pChannel->SetSweepParam(sweepPitch, length, false);
    } else {
        int time = rParam.portaTime;
        time *= time;
        time *= sweepPitch >= 0.0f ? sweepPitch : -sweepPitch;
        time = static_cast<u32>(time >> 5);
        time = static_cast<u32>(time << 2) + time;
        pChannel->SetSweepParam(sweepPitch, time, true);
    }

    rParam.portaKey = key;

    pChannel->SetSilence(rParam.silenceFlag != 0, 0);
    pChannel->SetReleasePriorityFix(mPlayer->IsReleasePriorityFix());
    pChannel->SetPanMode(mPlayer->GetPanMode());
    pChannel->SetPanCurve(mPlayer->GetPanCurve());

    return pChannel;
}

} // namespace detail
} // namespace snd
} // namespace nw4r

// Retail emits the out-of-line copy of this inline virtual in this unit.
u32 nw4r::snd::SoundArchivePlayer::detail_ConvertLabelStringToSoundId(
    const char* pLabel) {
    return mSoundArchive->ConvertLabelStringToSoundId(pLabel);
}
