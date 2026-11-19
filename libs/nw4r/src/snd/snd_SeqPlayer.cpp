#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace {

// Retail inlines the parser-parameter slice access; keep the base-address test
// readable through the real member.
inline const u8* GetParserBaseAddr(nw4r::snd::detail::SeqTrack* pTrack) {
    return pTrack->GetParserTrackParam().baseAddr;
}

} // namespace

namespace nw4r {
namespace snd {
namespace detail {

volatile s16 SeqPlayer::mGlobalVariable[GLOBAL_VARIABLE_NUM];

// NOTE (matching residual): the header declares static bool
// mGobalVariableInitialized (@typo) but retail never defines or reads it -
// InitSeqPlayer eagerly fills the global-variable array with -1 before any
// sound can run, so there is no lazy-init guard byte in the retail data pool.

// FinishPlayer (retail inlines it into ~SeqPlayer/Setup/InvalidateData via
// IPA; no out-of-line copy survives in the retail slice).
inline void SeqPlayer::FinishPlayer() {
    SoundThread::AutoLock lock;

    if (this->mStartedFlag) {
        SoundThread::GetInstance().UnregisterPlayerCallback(this);
        this->mStartedFlag = false;
    }

    if (this->mActiveFlag) {
        DisposeCallbackManager::GetInstance().UnregisterDisposeCallback(
            this);
        this->mActiveFlag = false;
    }

    // Explicit continue keeps two scope-exit edges (retail emits the
    // track-lock unlock twice); FreeTrack's argument reloads the slot.
    for (int i = 0; i < TRACK_NUM; i++) {
        SoundThread::AutoLock trackLock;

        SeqTrack* pTrack = GetPlayerTrack(i);
        if (pTrack == NULL) {
            continue;
        }
        pTrack->Close();
        this->mSeqTrackAllocator->FreeTrack(mTracks[i]);
        mTracks[i] = NULL;
    }
}

SeqPlayer::SeqPlayer() {
    mActiveFlag = false;
    mStartedFlag = false;
    mPauseFlag = false;
    mReleasePriorityFixFlag = false;

    mTempoRatio = 1.0f;
    mTickFraction = 0.0f;
    mSkipTickCounter = 0;
    mSkipTimeCounter = 0.0f;
    mPanRange = 1.0f;
    mTickCounter = 0;
    mVoiceOutCount = 0;

    mSeqUserprocCallback = NULL;
    mSeqUserprocCallbackArg = NULL;

    mParserParam.tempo = DEFAULT_TEMPO;
    mParserParam.timebase = DEFAULT_TIMEBASE;
    mParserParam.volume = 127;
    mParserParam.priority = DEFAULT_PRIORITY;
    mParserParam.callback = NULL;

    for (int i = 0; i < LOCAL_VARIABLE_NUM; i++) {
        mLocalVariable[i] = DEFAULT_VARIABLE_VALUE;
    }
    for (int i = 0; i < TRACK_NUM; i++) {
        mTracks[i] = NULL;
    }
}

// FinishPlayer, fully inlined at every retail call site (no out-of-line
// symbol exists in main.dol): unregisters the callbacks, then closes every
// open track under the sound-thread mutex.
void SeqPlayer::Stop() {
    SoundThread::AutoLock lock;

    FinishPlayer();
}

SeqPlayer::~SeqPlayer() {
    {
        SoundThread::AutoLock lock;

        // FinishPlayer fully inlined here by -ipa (same as Stop).
        FinishPlayer();
    }

    // Retail destructor takes the hidden delete flag; free the player when
    // the deletion bit is set.
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
    // Header layout puts mTracks at 0x118; retail keeps it at 0x120.
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

        // FinishPlayer inlined by retail; it owns its own lock scope.
        FinishPlayer();
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

        // SetPlayerTrack inlined by retail (owns its own lock).
        u32 trackFlags = allocTrackFlags;
        for (int i = 0; trackFlags != 0; trackFlags >>= 1, i++) {
            if (trackFlags & 1) {
                SeqTrack* pTrack = pAllocator->AllocTrack(this);

                SoundThread::AutoLock trackLock;

                // Explicit continue keeps two scope-exit edges (retail emits
                // the mutex unlock on both paths).
                if (i > TRACK_NUM - 1) {
                    continue;
                }
                mTracks[i] = pTrack;
                pTrack->SetPlayerTrackNo(i);
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

    // GetPlayerTrack(i) inlined by retail: MWCC keeps (this + i*4) as the
    // induction pointer and folds the 0x120 track-array base into the load.
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
    mParserParam.priority = priority;
}

void SeqPlayer::SetReleasePriorityFix(bool flag) {
    mReleasePriorityFixFlag = flag;
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
        // First scan has no calls on the non-matching path, so MWCC walks it
        // entirely in volatile registers: the induction pointer is based at
        // the player with the 0x120 track-array base folded into the load.
        for (int i = 0; i < TRACK_NUM; i++) {
            SeqTrack* pTrack = GetPlayerTrack(i);
            if (pTrack == NULL) {
                continue;
            }

            const u8* pBase = GetParserBaseAddr(pTrack);
            if (pStart <= pBase && pBase <= pEnd) {
                // SeqPlayer::Stop() inlined by retail.
                // Hoisted NULL for the track-slot store; retail pins it in
                // a callee-saved register across the close loop.
                SeqTrack* pRemovedTrack = NULL;

                {
                    SoundThread::AutoLock stopLock;

                    FinishPlayer();
                }
                break;
            }
        }
    }
}

void SeqPlayer::SetPlayerTrack(int idx, SeqTrack* pTrack) {
    SoundThread::AutoLock lock;

    if (idx > TRACK_NUM - 1) {
        return;
    }

    mTracks[idx] = pTrack;
    pTrack->SetPlayerTrackNo(idx);
}

SeqTrack* SeqPlayer::GetPlayerTrack(int idx) {
    if (idx > TRACK_NUM - 1) {
        return NULL;
    }

    return mTracks[idx];
}

int SeqPlayer::ParseNextTick(bool doNoteOn) {
    SoundThread::AutoLock lock;

    bool active = false;
    // Retail hoists this NULL into a callee-saved register before the loop
    // (loop-invariant store value for the close-track path).
    SeqTrack* pRemovedTrack = NULL;

    for (int i = 0; i < TRACK_NUM; i++) {
        // GetPlayerTrack(i) inlined by retail: bounds-checked load off the
        // 0x120 track array.
        SeqTrack* pTrack = GetPlayerTrack(i);
        if (pTrack == NULL) {
            continue;
        }

        pTrack->UpdateChannelLength();

        if (pTrack->ParseNextTick(doNoteOn) < 0) {
            // Inlined CloseTrack (retail inlines it; owns its own lock). The
            // do/break form keeps two scope-exit edges, matching retail's
            // duplicated mutex-unlock sequence.
            do {
                SoundThread::AutoLock lock2;

                SeqTrack* pClose = GetPlayerTrack(i);
                if (pClose == NULL) {
                    break;
                }

                pClose->Close();
                // Retail reloads the track slot for the FreeTrack argument
                // (pClose does not stay live across the Close call).
                mSeqTrackAllocator->FreeTrack(mTracks[i]);
                mTracks[i] = pRemovedTrack;
            } while (false);
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

    {
        // Second AutoLock scope: retail re-locks the sound thread around the
        // parameter update pass and unlocks twice on exit.
        SoundThread::AutoLock paramLock;

        for (int i = 0; i < TRACK_NUM; i++) {
            // GetPlayerTrack(i) inlined by retail: bounds-checked load off the
            // 0x120 track array.
            SeqTrack* pTrack = GetPlayerTrack(i);

            if (pTrack != NULL) {
                pTrack->UpdateChannelParam();
            }
        }
    }
}

// FinishPlayer, inlined by retail (no out-of-line symbol).
void SeqPlayer::UpdateTick(int msec) {
    // Base tempo: mTempoRatio * (timebase * tempo) / 60000, with the integer
    // product converted through the retail 2^52 pool constant.
    f32 tickPerMsec =
        mTempoRatio * (f32)(mParserParam.timebase * mParserParam.tempo) /
        60000.0f;
    if (0.0f == tickPerMsec) {
        return;
    }

    f32 nextMsec = mTickFraction / tickPerMsec;
    f32 restMsec = (f32)msec;

    while (nextMsec < restMsec) {
        restMsec -= nextMsec;

        if (ParseNextTick(true) != 0) {
            FinishPlayer();
            return;
        }

        mTickCounter++;

        tickPerMsec = mTempoRatio *
                      (f32)(mParserParam.timebase * mParserParam.tempo) /
                      60000.0f;
        if (0.0f == tickPerMsec) {
            return;
        }

        nextMsec = 1.0f / tickPerMsec;
    }

    nextMsec -= restMsec;
    mTickFraction = nextMsec * tickPerMsec;
}

void SeqPlayer::SkipTick() {
    // Release every voice on every open track before skipping ahead.
    for (int i = 0; i < TRACK_NUM; i++) {
        SeqTrack* pTrack = GetPlayerTrack(i);

        if (pTrack != NULL) {
            pTrack->ReleaseAllChannel(127);
            pTrack->FreeAllChannel();
        }
    }

    int tickCount = 0;
    while (mSkipTickCounter != 0 ||
           mSkipTimeCounter *
                   (mTempoRatio * (mParserParam.timebase * mParserParam.tempo) /
                    60000.0f) >=
               1.0f) {
        if (tickCount >= MAX_SKIP_TICK_PER_FRAME) {
            return;
        }

        // Consume one skipped tick, or the time equivalent of one tick.
        if (mSkipTickCounter != 0) {
            mSkipTickCounter--;
        } else {
            mSkipTimeCounter -=
                1.0f / (mTempoRatio *
                        (mParserParam.timebase * mParserParam.tempo) /
                        60000.0f);
        }

        if (ParseNextTick(false)) {
            FinishPlayer();
            return;
        }

        mTickCounter++;
        tickCount++;
    }

    mSkipTimeCounter = 0.0f;
}

// Eagerly resets the shared global variable array (retail symbol InitSeqPlayer).
void SeqPlayer::InitSeqPlayer() {
    for (int i = 0; i < GLOBAL_VARIABLE_NUM; i++) {
        mGlobalVariable[i] = DEFAULT_VARIABLE_VALUE;
    }
}

Channel* SeqPlayer::NoteOn(int bankNo, const NoteOnInfo& rInfo) {
    return mParserParam.callback->NoteOn(this, bankNo, rInfo);
}

void SeqPlayer::SetSeqUserprocCallback(SeqUserprocCallback callback,
                                       void* arg) {
    mSeqUserprocCallback = callback;
    mSeqUserprocCallbackArg = arg;
}

void SeqPlayer::CallSeqUserprocCallback(u16 usertype, SeqTrack* pTrack) {
    if (mSeqUserprocCallback == NULL) {
        return;
    }

    SeqUserprocCallbackParam param;
    param.variablePtr = &mLocalVariable[0];
    param.variablePtr2 = mGlobalVariable;
    param.variablePtr3 = pTrack->GetVariablePtr(0);
    param.value = pTrack->GetParserTrackParam().mmlParserParam.cmpFlag;

    mSeqUserprocCallback(usertype, &param, mSeqUserprocCallbackArg);

    pTrack->GetParserTrackParam().mmlParserParam.cmpFlag = param.value;
}

void SeqPlayer::ChannelCallback(Channel* /* pChannel */) {}

} // namespace detail
} // namespace snd
} // namespace nw4r
