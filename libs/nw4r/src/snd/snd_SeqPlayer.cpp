#include <nw4r/snd.h>
#include <nw4r/ut.h>

// .sdata2 constant-pool entries referenced by the functions below.
extern const f32 lbl_eu_8066A000; // 1.0f
extern const f32 lbl_eu_8066A004; // 0.0f
extern const f64 lbl_eu_8066A008; // 2^52 (signed int -> float magic)
extern const f32 lbl_eu_8066A010; // 60000.0f

namespace {

// Signed-int -> f32 conversion using the retail 0x43300000/2^52 trick.
union IntToF32Conv {
    f64 d;
    u32 w[2];
};

// Retail layout mirror: userproc callback/arg at +0x118/+0x11C and the real
// track pointer array at +0x120. The shared header places mTracks at 0x118,
// so everything written here goes through this mirror instead.
struct SeqPlayerRetailSlots {
    char field_0x0[0x118];
    void* callback;                       // at 0x118
    void* arg;                            // at 0x11C
    nw4r::snd::detail::SeqTrack* tracks[16]; // at 0x120
};

} // namespace

namespace nw4r {
namespace snd {
namespace detail {

volatile s16 SeqPlayer::mGlobalVariable[GLOBAL_VARIABLE_NUM];
// NOTE (matching residual): the header declares static bool
// mGobalVariableInitialized (@typo) but retail never defines or reads it -
// InitSeqPlayer eagerly fills the global-variable array with -1 before any
// sound can run, so there is no lazy-init guard byte in the retail data pool.

SeqPlayer::SeqPlayer() {
    mActiveFlag = false;
    mStartedFlag = false;
    mPauseFlag = false;
    mReleasePriorityFixFlag = false;

    mTempoRatio = lbl_eu_8066A000;
    mTickFraction = lbl_eu_8066A004;
    mSkipTickCounter = 0;
    mSkipTimeCounter = lbl_eu_8066A004;
    mPanRange = lbl_eu_8066A000;
    mTickCounter = 0;
    mVoiceOutCount = 0;

    reinterpret_cast<SeqPlayerRetailSlots*>(this)->callback = NULL;
    reinterpret_cast<SeqPlayerRetailSlots*>(this)->arg = NULL;

    mParserParam.tempo = DEFAULT_TEMPO;
    mParserParam.timebase = DEFAULT_TIMEBASE;
    mParserParam.volume = 127;
    mParserParam.priority = DEFAULT_PRIORITY;
    mParserParam.callback = NULL;

    for (int i = 0; i < LOCAL_VARIABLE_NUM; i++) {
        mLocalVariable[i] = DEFAULT_VARIABLE_VALUE;
    }
    for (int i = 0; i < TRACK_NUM; i++) {
        reinterpret_cast<SeqPlayerRetailSlots*>(this)->tracks[i] = NULL;
    }
}

// FinishPlayer, fully inlined at every retail call site (no out-of-line
// symbol exists in main.dol): unregisters the callbacks, then closes every
// open track under the sound-thread mutex.
void SeqPlayer::Stop() {
    SoundThread::AutoLock lock;

    {
        SoundThread::AutoLock finishLock;

        if (mStartedFlag) {
            SoundThread::GetInstance().UnregisterPlayerCallback(this);
            mStartedFlag = false;
        }

        if (mActiveFlag) {
            DisposeCallbackManager::GetInstance().UnregisterDisposeCallback(
                this);
            mActiveFlag = false;
        }

        SeqTrack** pSlot =
            reinterpret_cast<SeqPlayerRetailSlots*>(this)->tracks;
        for (int i = 0; i < TRACK_NUM; i++, pSlot++) {
            SoundThread::AutoLock trackLock;

            SeqTrack* pTrack = (i > TRACK_NUM - 1) ? NULL : *pSlot;
            if (pTrack != NULL) {
                pTrack->Close();
                mSeqTrackAllocator->FreeTrack(pTrack);
                *pSlot = NULL;
            }
        }
    }
}

SeqPlayer::~SeqPlayer() {
    SoundThread::AutoLock lock;

    {
        SoundThread::AutoLock finishLock;

        if (mStartedFlag) {
            SoundThread::GetInstance().UnregisterPlayerCallback(this);
            mStartedFlag = false;
        }

        if (mActiveFlag) {
            DisposeCallbackManager::GetInstance().UnregisterDisposeCallback(
                this);
            mActiveFlag = false;
        }

        SeqTrack** pSlot =
            reinterpret_cast<SeqPlayerRetailSlots*>(this)->tracks;
        for (int i = 0; i < TRACK_NUM; i++, pSlot++) {
            SoundThread::AutoLock trackLock;

            SeqTrack* pTrack = (i > TRACK_NUM - 1) ? NULL : *pSlot;
            if (pTrack != NULL) {
                pTrack->Close();
                mSeqTrackAllocator->FreeTrack(pTrack);
                *pSlot = NULL;
            }
        }
    }
}

void SeqPlayer::InitParam(int voices, NoteOnCallback* pCallback) {
    BasicPlayer::InitParam();

    mStartedFlag = false;
    mPauseFlag = false;
    mTempoRatio = 1.0f;
    mSkipTickCounter = 0;
    mSkipTimeCounter = 0.0f;
    mPanRange = 1.0f;
    mTickCounter = 0;
    mVoiceOutCount = voices;

    mParserParam.tempo = DEFAULT_TEMPO;
    mParserParam.timebase = DEFAULT_TIMEBASE;
    mParserParam.volume = 127;
    mParserParam.priority = 64;
    mParserParam.callback = pCallback;

    mTickFraction = 0.0f;

    for (int i = 0; i < LOCAL_VARIABLE_NUM; i++) {
        mLocalVariable[i] = DEFAULT_VARIABLE_VALUE;
    }
    for (int i = 0; i < TRACK_NUM; i++) {
        mTracks[i] = NULL;
    }
}

// FinishPlayer (inlined by retail) - unregisters callbacks and closes every
// open track under the sound-thread mutex.
SeqPlayer::SetupResult SeqPlayer::Setup(SeqTrackAllocator* pAllocator,
                                        u32 allocTrackFlags, int voices,
                                        NoteOnCallback* pCallback) {
    SoundThread::AutoLock lock;

    {
        SoundThread::AutoLock stopLock;
        {
            SoundThread::AutoLock finishLock;

            if (mStartedFlag) {
                SoundThread::GetInstance().UnregisterPlayerCallback(this);
                mStartedFlag = false;
            }

            if (mActiveFlag) {
                DisposeCallbackManager::GetInstance().UnregisterDisposeCallback(
                    this);
                mActiveFlag = false;
            }

            SeqTrack** pSlot =
                reinterpret_cast<SeqPlayerRetailSlots*>(this)->tracks;
            for (int i = 0; i < TRACK_NUM; i++, pSlot++) {
                SoundThread::AutoLock trackLock;

                SeqTrack* pClose = (i > TRACK_NUM - 1) ? NULL : *pSlot;
                if (pClose != NULL) {
                    pClose->Close();
                    mSeqTrackAllocator->FreeTrack(pClose);
                    *pSlot = NULL;
                }
            }
        }
    }

    InitParam(voices, pCallback);

    {
        ut::AutoInterruptLock intLock;

        int count = 0;
        for (u32 trackFlags = allocTrackFlags; trackFlags != 0;
             trackFlags >>= 1) {
            if (trackFlags & 1) {
                count++;
            }
        }

        if (count > pAllocator->GetAllocatableTrackCount()) {
            return SETUP_ERR_CANNOT_ALLOCATE_TRACK;
        }

        u32 trackFlags = allocTrackFlags;
        for (int i = 0; trackFlags != 0; trackFlags >>= 1, i++) {
            if (trackFlags & 1) {
                SeqTrack* pTrack = pAllocator->AllocTrack(this);
                SetPlayerTrack(i, pTrack);
            }
        }
    }

    DisposeCallbackManager::GetInstance().RegisterDisposeCallback(this);

    mSeqTrackAllocator = pAllocator;
    mActiveFlag = true;

    return SETUP_SUCCESS;
}

void SeqPlayer::SetSeqData(const void* pBase, s32 offset) {
    SoundThread::AutoLock lock;

    SeqTrack* pTrack = GetPlayerTrack(0);

    if (pBase != NULL) {
        pTrack->SetSeqData(pBase, offset);
        pTrack->Open();
    }
}

bool SeqPlayer::Start() {
    SoundThread::AutoLock lock;

    SoundThread::GetInstance().RegisterPlayerCallback(this);
    mStartedFlag = true;

    return true;
}

void SeqPlayer::Pause(bool flag) {
    SoundThread::AutoLock lock;

    mPauseFlag = flag;

    for (int i = 0; i < TRACK_NUM; i++) {
        SeqTrack* pTrack = GetPlayerTrack(i);

        if (pTrack != NULL) {
            pTrack->PauseAllChannel(flag);
        }
    }
}

void SeqPlayer::Skip(OffsetType type, int offset) {
    SoundThread::AutoLock lock;

    if (!mActiveFlag) {
        return;
    }

    switch (type) {
    case OFFSET_TYPE_TICK: {
        mSkipTickCounter += offset;
        break;
    }

    case OFFSET_TYPE_MILLISEC: {
        mSkipTimeCounter += offset;
        break;
    }
    }
}

void SeqPlayer::SetTempoRatio(f32 tempo) {
    mTempoRatio = tempo;
}

void SeqPlayer::SetChannelPriority(int priority) {
    *(u8*)((char*)this + 0x109) = priority;
}

void SeqPlayer::SetReleasePriorityFix(bool flag) {
    *(unsigned char*)((unsigned char*)this + 0xEF) = (unsigned char)flag;
}

void SeqPlayer::SetLocalVariable(int idx, s16 value) {
    mLocalVariable[idx] = value;
}

void SeqPlayer::SetGlobalVariable(int idx, s16 value) {
    mGlobalVariable[idx] = value;
}

void SeqPlayer::SetTrackVolume(u32 trackFlags, f32 volume) {
    // Body of SetTrackParam<f32> inlined with a direct call: taking
    // &SeqTrack::SetVolume as a PMF argument makes MWCC materialize a static
    // 12-byte member-pointer table that the retail linker GC'd (these
    // functions have no retail counterpart).
    ut::AutoInterruptLock lock;

    for (int i = 0; i < TRACK_NUM && trackFlags != 0; trackFlags >>= 1, i++) {
        if (trackFlags & 1) {
            SeqTrack* pTrack = GetPlayerTrack(i);

            if (pTrack != NULL) {
                pTrack->SetVolume(volume);
            }
        }
    }
}

void SeqPlayer::SetTrackPitch(u32 trackFlags, f32 pitch) {
    ut::AutoInterruptLock lock;

    for (int i = 0; i < TRACK_NUM && trackFlags != 0; trackFlags >>= 1, i++) {
        if (trackFlags & 1) {
            SeqTrack* pTrack = GetPlayerTrack(i);

            if (pTrack != NULL) {
                pTrack->SetPitch(pitch);
            }
        }
    }
}

void SeqPlayer::InvalidateData(const void* pStart, const void* pEnd) {
    SoundThread::AutoLock lock;

    if (mActiveFlag) {
        for (int i = 0; i < TRACK_NUM; i++) {
            SeqTrack* pTrack = (i > TRACK_NUM - 1) ? NULL : mTracks[i];
            if (pTrack == NULL) {
                continue;
            }

            const u8* pBase = pTrack->GetParserTrackParam().baseAddr;
            if (pStart <= pBase && pBase <= pEnd) {
                // SeqPlayer::Stop() inlined by retail.
                SoundThread::AutoLock stopLock;

                {
                    SoundThread::AutoLock finishLock;

                    if (mStartedFlag) {
                        SoundThread::GetInstance().UnregisterPlayerCallback(
                            this);
                        mStartedFlag = false;
                    }

                    if (mActiveFlag) {
                        DisposeCallbackManager::GetInstance()
                            .UnregisterDisposeCallback(this);
                        mActiveFlag = false;
                    }

                    SeqTrack** pSlot = reinterpret_cast<SeqPlayerRetailSlots*>(
                        this)->tracks;
                    for (int j = 0; j < TRACK_NUM; j++, pSlot++) {
                        SoundThread::AutoLock trackLock;

                        SeqTrack* pClose = (j > TRACK_NUM - 1) ? NULL : *pSlot;
                        if (pClose != NULL) {
                            pClose->Close();
                            mSeqTrackAllocator->FreeTrack(pClose);
                            *pSlot = NULL;
                        }
                    }
                }
                break;
            }
        }
    }
}

SeqTrack* SeqPlayer::GetPlayerTrack(int idx) {
    if (idx > TRACK_NUM - 1) {
        return NULL;
    }

    SeqTrack** tracks = (SeqTrack**)((u8*)this + 0x120);
    return tracks[idx];
}

void SeqPlayer::SetPlayerTrack(int idx, SeqTrack* pTrack) {
    SoundThread::AutoLock lock;

    if (idx > TRACK_NUM - 1) {
        return;
    }

    mTracks[idx] = pTrack;
    pTrack->SetPlayerTrackNo(idx);
}

int SeqPlayer::ParseNextTick(bool doNoteOn) {
    SoundThread::AutoLock lock;

    bool active = false;

    for (int i = 0; i < TRACK_NUM; i++) {
        SeqTrack* pTrack = (i > TRACK_NUM - 1) ? NULL : mTracks[i];
        if (pTrack == NULL) {
            continue;
        }

        pTrack->UpdateChannelLength();

        if (pTrack->ParseNextTick(doNoteOn) < 0) {
            // Inlined CloseTrack (retail inlines it; owns its own lock).
            SoundThread::AutoLock lock2;

            SeqTrack* pClose = (i > TRACK_NUM - 1) ? NULL : mTracks[i];
            if (pClose != NULL) {
                pClose->Close();
                mSeqTrackAllocator->FreeTrack(pClose);
                mTracks[i] = NULL;
            }
        }

        if (pTrack->IsOpened()) {
            active = true;
        }
    }

    if (!active) {
        return 1;
    }

    return 0;
}

volatile s16* SeqPlayer::GetVariablePtr(int idx) {
    if (idx < LOCAL_VARIABLE_NUM) {
        return &mLocalVariable[idx];
    }

    if (idx < VARIABLE_NUM) {
        return &mGlobalVariable[idx - LOCAL_VARIABLE_NUM];
    }

    return NULL;
}

void SeqPlayer::Update() {
    SoundThread::AutoLock lock;

    if (!mActiveFlag) {
        return;
    }

    if (!mStartedFlag) {
        return;
    }

    if (mSkipTickCounter != 0 || mSkipTimeCounter > 0.0f) {
        SkipTick();

    } else if (!mPauseFlag) {
        UpdateTick(3);
    }

    // UpdateChannelParam inlined by retail.
    for (int i = 0; i < TRACK_NUM; i++) {
        SeqTrack* pTrack = GetPlayerTrack(i);

        if (pTrack != NULL) {
            pTrack->UpdateChannelParam();
        }
    }
}

// FinishPlayer, inlined by retail (no out-of-line symbol).
void SeqPlayer::UpdateTick(int msec) {
    IntToF32Conv conv;

    // Base tempo: mTempoRatio * (timebase * tempo) / 60000, with the integer
    // product converted through the retail 2^52 pool constant.
    conv.w[0] = 0x43300000;
    conv.w[1] =
        (u32)(mParserParam.timebase * mParserParam.tempo) ^ 0x80000000;
    f32 tickPerMsec = mTempoRatio * (f32)(conv.d - lbl_eu_8066A008) /
                      lbl_eu_8066A010;
    if (tickPerMsec == lbl_eu_8066A004) {
        return;
    }

    conv.w[1] = (u32)msec ^ 0x80000000;
    f32 restMsec = (f32)(conv.d - lbl_eu_8066A008);
    f32 nextMsec = mTickFraction / tickPerMsec;

    while (nextMsec < restMsec) {
        restMsec -= nextMsec;

        if (ParseNextTick(true) != 0) {
            SoundThread::AutoLock finishLock;

            if (mStartedFlag) {
                SoundThread::GetInstance().UnregisterPlayerCallback(this);
                mStartedFlag = false;
            }

            if (mActiveFlag) {
                DisposeCallbackManager::GetInstance()
                    .UnregisterDisposeCallback(this);
                mActiveFlag = false;
            }

            SeqTrack** pSlot =
                reinterpret_cast<SeqPlayerRetailSlots*>(this)->tracks;
            for (int i = 0; i < TRACK_NUM; i++, pSlot++) {
                SoundThread::AutoLock trackLock;

                SeqTrack* pClose = (i > TRACK_NUM - 1) ? NULL : *pSlot;
                if (pClose != NULL) {
                    pClose->Close();
                    mSeqTrackAllocator->FreeTrack(pClose);
                    *pSlot = NULL;
                }
            }
            return;
        }

        mTickCounter++;

        conv.w[1] =
            (u32)(mParserParam.timebase * mParserParam.tempo) ^ 0x80000000;
        tickPerMsec = mTempoRatio * (f32)(conv.d - lbl_eu_8066A008) /
                      lbl_eu_8066A010;
        if (tickPerMsec == lbl_eu_8066A004) {
            return;
        }

        nextMsec = lbl_eu_8066A000 / tickPerMsec;
    }

    nextMsec -= restMsec;
    mTickFraction = nextMsec * tickPerMsec;
}

void SeqPlayer::SkipTick() {
    SeqTrack** pSlot = reinterpret_cast<SeqPlayerRetailSlots*>(this)->tracks;
    for (int i = 0; i < TRACK_NUM; i++, pSlot++) {
        SeqTrack* pTrack = (i > TRACK_NUM - 1) ? NULL : *pSlot;

        if (pTrack != NULL) {
            pTrack->ReleaseAllChannel(127);
            pTrack->FreeAllChannel();
        }
    }

    // Retail tests mSkipTickCounter (kept in a register across the loop
    // condition) against MAX_SKIP_TICK_PER_FRAME at the top of each pass.
    while (mSkipTickCounter != 0 || mSkipTimeCounter * GetBaseTempo() >= 1.0f) {
        if (mSkipTickCounter >= MAX_SKIP_TICK_PER_FRAME) {
            return;
        }

        if (mSkipTickCounter != 0) {
            mSkipTickCounter--;
        } else {
            mSkipTimeCounter -= 1.0f / GetBaseTempo();
        }

        if (ParseNextTick(false) != 0) {
            // FinishPlayer inlined by retail.
            SoundThread::AutoLock finishLock;

            if (mStartedFlag) {
                SoundThread::GetInstance().UnregisterPlayerCallback(this);
                mStartedFlag = false;
            }

            if (mActiveFlag) {
                DisposeCallbackManager::GetInstance()
                    .UnregisterDisposeCallback(this);
                mActiveFlag = false;
            }

            SeqTrack** pSlot =
                reinterpret_cast<SeqPlayerRetailSlots*>(this)->tracks;
            for (int i = 0; i < TRACK_NUM; i++, pSlot++) {
                SoundThread::AutoLock trackLock;

                SeqTrack* pClose = (i > TRACK_NUM - 1) ? NULL : *pSlot;
                if (pClose != NULL) {
                    pClose->Close();
                    mSeqTrackAllocator->FreeTrack(pClose);
                    *pSlot = NULL;
                }
            }
            return;
        }

        mTickCounter++;
    }

    mSkipTimeCounter = lbl_eu_8066A004;
}

void SeqPlayer::InitGlobalVariable() {
    for (int i = 0; i < GLOBAL_VARIABLE_NUM; i++) {
        mGlobalVariable[i] = DEFAULT_VARIABLE_VALUE;
    }
}

Channel* SeqPlayer::NoteOn(int bankNo, const NoteOnInfo& rInfo) {
    return mParserParam.callback->NoteOn(this, bankNo, rInfo);
}

} // namespace detail
} // namespace snd
} // namespace nw4r


void SetSeqUserprocCallback__Q44nw4r3snd6detail9SeqPlayerFPFUsPQ34nw4r3snd24SeqUserprocCallbackParamPv_vPv(
    void* _this, void* callback, void* arg)
{
    *(u32*)((u8*)_this + 0x118) = (u32)callback;
    *(u32*)((u8*)_this + 0x11C) = (u32)arg;
}
// Retail global variable array; attached to SeqPlayer via the symbol map.
extern volatile s16 lbl_eu_806382C0[16];

void InitSeqPlayer__Q44nw4r3snd6detail9SeqPlayerFv() {
    for (int i = 0; i < 16; i++) {
        lbl_eu_806382C0[i] = -1;
    }
}

void CallSeqUserprocCallback__Q44nw4r3snd6detail9SeqPlayerFUsPQ44nw4r3snd6detail8SeqTrack(
    nw4r::snd::detail::SeqPlayer* selfPtr, unsigned short usertype, nw4r::snd::detail::SeqTrack* trackPtr)
{
    struct SeqUserprocCallbackParam {
        volatile s16* variablePtr;   // 0x0
        volatile s16* variablePtr2;  // 0x4
        volatile s16* variablePtr3;  // 0x8
        u8 value;                    // 0xC
        u8 field_0xD;
        u8 field_0xE;
        u8 field_0xF;
    };
    struct SeqPlayerProcLayout {
        char field_0x0[0x118];
        void (*callback)(unsigned short, SeqUserprocCallbackParam*, void*); // 0x118
        void* arg;                                            // 0x11C
        char field_0x120[0x160 - 0x120];
        volatile s16 variableV;                               // 0x160
    };
    struct SeqTrackProcLayout {
        char field_0x0[0x24];
        u8 value; // 0x24
    };

    SeqPlayerProcLayout* self = (SeqPlayerProcLayout*)selfPtr;
    if (self->callback == NULL) {
        return;
    }

    SeqUserprocCallbackParam param;
    param.variablePtr = &self->variableV; // player variable region (0x160)
    param.variablePtr2 = lbl_eu_806382C0; // global variable array
    param.variablePtr3 = trackPtr->GetVariablePtr(0);
    param.value = ((SeqTrackProcLayout*)trackPtr)->value;

    self->callback(usertype, &param, self->arg);

    ((SeqTrackProcLayout*)trackPtr)->value = param.value;
}
extern "C" void ChannelCallback__Q44nw4r3snd6detail9SeqPlayerFPQ44nw4r3snd6detail7Channel() {}
