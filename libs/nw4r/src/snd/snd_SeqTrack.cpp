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
// functions below address the retail layout through the TU-local
// SeqTrackParamView below (track channel list at +0xC4, link at
// Channel+0xF4, variables at +0xA0). Widths are exact (retail lha/sth
// => s16, lwz/stw => s32/pointers).
struct SeqTrackParamView {
    u8 _00[0x1C];                // 0x00..0x1B (header-typed prefix)
    const u8* seqBase;           // 0x1C
    const u8* seqCursor;         // 0x20
    u8 b24;                      // 0x24
    u8 b25;                      // 0x25
    u8 b26;                      // 0x26
    u8 b27;                      // 0x27
    u8 _28[0x18];                // 0x28..0x3F
    u8 b40;                      // 0x40
    u8 _41[3];                   // 0x41..0x43
    s32 tickWait;                // 0x44
    u8 muteFlag;                 // 0x48
    u8 b49;                      // 0x49
    u8 waitFlag;                 // 0x4A
    u8 b4B;                      // 0x4B
    u8 damper;                   // 0x4C
    u8 _4D[3];                   // 0x4D..0x4F
    s32 w50;                     // 0x50
    s32 w54;                     // 0x54
    LfoParam lfo;                // 0x58..0x67
    u8 b68;                      // 0x68
    u8 _69[3];                   // 0x69..0x6B
    f32 f6C;                     // 0x6C
    u8 rampA0;                   // 0x70
    u8 rampA1;                   // 0x71
    s16 rampTgt0;                // 0x72
    s16 rampCur0;                // 0x74
    u8 rampB0;                   // 0x76
    u8 rampB1;                   // 0x77
    s16 rampTgt1;                // 0x78
    s16 rampCur1;                // 0x7A
    u8 rampC0;                   // 0x7C
    u8 rampC1;                   // 0x7D
    s16 rampTgt2;                // 0x7E
    s16 rampCur2;                // 0x80
    u8 b82;                      // 0x82
    u8 b83;                      // 0x83
    u8 b84;                      // 0x84
    u8 b85;                      // 0x85
    u8 b86;                      // 0x86
    u8 b87;                      // 0x87
    u8 b88;                      // 0x88
    u8 b89;                      // 0x89
    u8 b8A;                      // 0x8A
    u8 b8B;                      // 0x8B
    u8 b8C;                      // 0x8C
    u8 b8D;                      // 0x8D
    u8 b8E;                      // 0x8E
    u8 _8F;                      // 0x8F
    s16 w90;                     // 0x90
    u8 b92;                      // 0x92
    u8 b93;                      // 0x93
    u8 b94;                      // 0x94
    u8 b95;                      // 0x95
    u8 b96;                      // 0x96
    u8 _97;                      // 0x97
    f32 f98;                     // 0x98
    f32 f9C;                     // 0x9C
    volatile s16 variables[SeqTrack::VARIABLE_NUM]; // 0xA0..0xBF
    SeqPlayer* player;           // 0xC0
    Channel* channelList;        // 0xC4
};

// Per-channel retail link slot: the track-list next pointer lives at
// Channel+0xF4 (NOT Channel::mNextLink at +0xE8), so the list walks below
// address it through this view instead of the class accessor.
struct SeqChannelLinkView {
    u8 _00[0xF4];
    Channel* nextTrackChannel;  // 0xF4
};
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
    SeqTrackParamView* v = (SeqTrackParamView*)this;

    mExtVolume = lbl_eu_8066A018;
    mExtPitch = lbl_eu_8066A018;
    mExtPan = lbl_eu_8066A01C;
    mExtSurroundPan = lbl_eu_8066A01C;
    mPanRange = lbl_eu_8066A018;
    // Retail stores bit-pattern zero via integer registers here.
    v->seqBase = NULL;
    v->seqCursor = NULL;

    v->b24 = 1;
    v->b25 = 1;
    v->b26 = 0;
    v->b27 = 0;

    v->b40 = 0;
    v->tickWait = 0;
    v->muteFlag = 0;
    v->b49 = 0;
    v->waitFlag = 0;
    v->b4B = 0;
    v->damper = 0;
    v->w50 = 0;
    v->w54 = 0;

    v->lfo.Init();

    v->b68 = 0;
    v->f6C = lbl_eu_8066A01C;
    v->rampA0 = 127;
    v->rampA1 = 127;
    v->rampTgt0 = 0;
    v->rampCur0 = 0;
    v->rampB0 = 0;
    v->rampB1 = 0;
    v->rampTgt1 = 0;
    v->rampCur1 = 0;
    v->rampC0 = 0;
    v->rampC1 = 0;
    v->rampTgt2 = 0;
    v->rampCur2 = 0;
    v->b82 = 127;
    v->b83 = 127;
    v->b84 = 0;
    v->b85 = 2;
    v->b86 = 0;
    v->b87 = 0;
    v->b88 = 64;
    v->b89 = 60;
    v->b8A = 0;
    v->b8B = 255;
    v->b8C = 255;
    v->b8D = 255;
    v->b8E = 255;
    v->w90 = 255;
    v->b92 = 127;
    v->b93 = 0;
    v->b94 = 0;
    v->b95 = 0;
    v->f98 = lbl_eu_8066A01C;
    v->b96 = 0;
    v->f9C = lbl_eu_8066A01C;

    volatile s16* variables = v->variables;
    for (int i = 0; i < VARIABLE_NUM; i++) {
        variables[i] = DEFAULT_VARIABLE_VALUE;
    }
}

void SeqTrack::SetSeqData(const void* pBase, s32 offset) {
    const u8* base = static_cast<const u8*>(pBase);
    ((SeqTrackParamView*)this)->seqBase = base;
    ((SeqTrackParamView*)this)->seqCursor = base + offset;
}

void SeqTrack::Open() {
    ((SeqTrackParamView*)this)->waitFlag = false;
    ((SeqTrackParamView*)this)->b40 = false;
    ((SeqTrackParamView*)this)->tickWait = 0;
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
    for (Channel* pIt = ((SeqTrackParamView*)this)->channelList; pIt != NULL;
         pIt = ((SeqChannelLinkView*)pIt)->nextTrackChannel) {

        if (pIt->GetLength() > 0) {
            pIt->SetLength(pIt->GetLength() - 1);
        }

        // Note-off pending channels unless already released or held by damper.
        if (pIt->GetLength() == 0 && !pIt->IsRelease() &&
            ((SeqTrackParamView*)this)->damper == 0) {
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

// Retail-layout view of SeqTrack for ParseNextTick: the shared header's
// member offsets past +0x20 do not match retail (see NOTE above), so the
// tick loop addresses its counters through this TU-local view. Widths are
// exact (retail lha => s16).
struct SeqTrackTickView {
    u8 _00[0x20];
    const u8* seqCursor;  // 0x20
    u8 _24[0x20];         // 0x24..0x43
    s32 tickWait;         // 0x44
    u8 _48[2];            // 0x48..0x49
    u8 waitFlag;          // 0x4A
    u8 _4B[0x25];         // 0x4B..0x6F
    u8 _70[2];            // 0x70..0x71
    s16 rampTgt0;         // 0x72
    s16 rampCur0;         // 0x74
    u8 _76[2];            // 0x76..0x77
    s16 rampTgt1;         // 0x78
    s16 rampCur1;         // 0x7A
    u8 _7C[2];            // 0x7C..0x7D
    s16 rampTgt2;         // 0x7E
    s16 rampCur2;         // 0x80
    u8 _82[0x42];         // 0x82..0xC3
    Channel* channelList;  // 0xC4
};

int SeqTrack::ParseNextTick(bool doNoteOn) {
    SoundThread::AutoLock lock;

    if (!mOpenFlag) {
        return 0;
    }

    SeqTrackTickView* p = (SeqTrackTickView*)this;

    // Advance three bounded s16 ramp counters.
    if (p->rampCur0 < p->rampTgt0) {
        p->rampCur0++;
    }
    if (p->rampCur1 < p->rampTgt1) {
        p->rampCur1++;
    }
    if (p->rampCur2 < p->rampTgt2) {
        p->rampCur2++;
    }

    // Waiting for running notes to finish before parsing further events.
    if (p->waitFlag) {
        if (p->channelList != NULL) {
            return 1;
        }

        p->waitFlag = false;
    }

    // Tick wait countdown: keep returning while wait remains positive.
    if (p->tickWait > 0 && --p->tickWait > 0) {
        return 1;
    }

    if (p->seqCursor != NULL) {
        while (p->tickWait == 0 && p->waitFlag == 0) {
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

    Channel* channelList = ((SeqTrackParamView*)this)->channelList;
    for (Channel* pIt = channelList; pIt != NULL;
         pIt = ((SeqChannelLinkView*)pIt)->nextTrackChannel) {

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

    Channel* channelList = ((SeqTrackParamView*)this)->channelList;
    for (Channel* pIt = channelList; pIt != NULL;
         pIt = ((SeqChannelLinkView*)pIt)->nextTrackChannel) {

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

    Channel** channelList = &((SeqTrackParamView*)this)->channelList;
    for (Channel* pIt = *channelList; pIt != NULL;
         pIt = ((SeqChannelLinkView*)pIt)->nextTrackChannel) {

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

    SeqPlayer* player = ((SeqTrackParamView*)p)->player;
    if (player != NULL) {
        player->ChannelCallback(pDropChannel);
    }

    Channel** channelList = &((SeqTrackParamView*)p)->channelList;
    if (*channelList == pDropChannel) {
        *channelList = ((SeqChannelLinkView*)pDropChannel)->nextTrackChannel;
        return;
    }

    for (Channel* pIt = *channelList;
         ((SeqChannelLinkView*)pIt)->nextTrackChannel != NULL;
         pIt = ((SeqChannelLinkView*)pIt)->nextTrackChannel) {

        if (((SeqChannelLinkView*)pIt)->nextTrackChannel == pDropChannel) {
            ((SeqChannelLinkView*)pIt)->nextTrackChannel =
                ((SeqChannelLinkView*)pDropChannel)->nextTrackChannel;
            return;
        }
    }
}

void SeqTrack::SetMute(SeqMute mute) {
    SoundThread::AutoLock lock;

    u8* muteFlag = &((SeqTrackParamView*)this)->muteFlag;
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
        SeqTrackParamView* v = (SeqTrackParamView*)this;
        return &v->variables[idx];
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
