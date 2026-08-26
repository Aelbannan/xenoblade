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
// Retail layout mirror: SeqTrack stores the parser base address at +0x1C
// (the shared header's field layout differs).
struct SeqTrackRetailSlots {
    char field_0x0[0x1C];
    const u8* baseAddr; // at 0x1C
};

struct SeqPlayerRetailSlots {
    char field_0x0[0xEC];
    bool activeFlag;                          // at 0xEC
    bool startedFlag;                         // at 0xED
    char field_0xEE[0xF4 - 0xEE];
    f32 tempoRatio;                           // at 0xF4
    char field_0xF8[0xFC - 0xF8];
    u32 skipTickCounter;                      // at 0xFC
    f32 skipTimeCounter;                      // at 0x100
    char field_0x104[0x10A - 0x104];
    u8 timebase;                              // at 0x10A
    u16 tempo;                                // at 0x10C
    char field_0x10E[0x114 - 0x10E];
    nw4r::snd::detail::SeqTrackAllocator* allocator; // at 0x114
    void* callback;                       // at 0x118
    void* arg;                            // at 0x11C
    nw4r::snd::detail::SeqTrack* tracks[16]; // at 0x120
    char field_0x160[0x180 - 0x160];
    int tickCounter;                          // at 0x180
};

// Loads timebase*tempo through the 0x43300000/2^52 conversion union.
inline f64 LoadTempoProduct(SeqPlayerRetailSlots* pSlot, IntToF32Conv& conv) {
    conv.w[0] = 0x43300000;
    conv.w[1] = (u32)(pSlot->timebase * pSlot->tempo) ^ 0x80000000;
    return conv.d;
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

// FinishPlayer body (retail inlines it into Stop/~SeqPlayer via IPA). Kept
// as a standalone callee so `-ipa file` re-inlines it in CALLER context,
// reproducing retail's Chaitin register colors.
static inline void FinishPlayerImpl(SeqPlayer& rPlayer) {
    SeqPlayer* pPlayer = &rPlayer;
    SoundThread::AutoLock lock;

    if (reinterpret_cast<SeqPlayerRetailSlots*>(pPlayer)->startedFlag) {
        SoundThread::GetInstance().UnregisterPlayerCallback(pPlayer);
        reinterpret_cast<SeqPlayerRetailSlots*>(pPlayer)->startedFlag = false;
    }

    if (reinterpret_cast<SeqPlayerRetailSlots*>(pPlayer)->activeFlag) {
        DisposeCallbackManager::GetInstance().UnregisterDisposeCallback(
            pPlayer);
        reinterpret_cast<SeqPlayerRetailSlots*>(pPlayer)->activeFlag = false;
    }

    // Explicit continue keeps two scope-exit edges (retail emits the
    // track-lock unlock twice); FreeTrack's argument reloads the slot.
    // Induction pointer based at the player; the 0x120 track-array base is
    // folded into the access displacement (pSlot[0x48]), as in retail.
    SeqTrack** pSlot = reinterpret_cast<SeqTrack**>(pPlayer);
    for (int i = 0; i < SeqPlayer::TRACK_NUM; pSlot++, i++) {
        SoundThread::AutoLock trackLock;

        SeqTrack* pTrack =
            (i > SeqPlayer::TRACK_NUM - 1) ? NULL : pSlot[0x48];
        if (pTrack == NULL) {
            continue;
        }
        pTrack->Close();
        reinterpret_cast<SeqPlayerRetailSlots*>(pPlayer)
            ->allocator->FreeTrack(pSlot[0x48]);
        pSlot[0x48] = NULL;
    }
}

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

    FinishPlayerImpl(*this);
}

SeqPlayer::~SeqPlayer() {
    {
        SoundThread::AutoLock lock;

        // FinishPlayer fully inlined here by -ipa (same as Stop).
        FinishPlayerImpl(*this);
    }

    // Retail destructor takes the hidden delete flag; free the player when
    // the deletion bit is set.
}

void SeqPlayer::InitParam(int voices, NoteOnCallback* pCallback) {
    BasicPlayer::InitParam();

    mStartedFlag = false;
    mPauseFlag = false;
    mTempoRatio = lbl_eu_8066A000;
    mSkipTickCounter = 0;
    mSkipTimeCounter = lbl_eu_8066A004;
    mPanRange = lbl_eu_8066A000;
    mTickCounter = 0;
    mVoiceOutCount = voices;

    mParserParam.tempo = DEFAULT_TEMPO;
    mParserParam.timebase = DEFAULT_TIMEBASE;
    mParserParam.volume = 127;
    mParserParam.priority = 64;
    mParserParam.callback = pCallback;

    mTickFraction = lbl_eu_8066A004;

    for (int i = 0; i < LOCAL_VARIABLE_NUM; i++) {
        mLocalVariable[i] = DEFAULT_VARIABLE_VALUE;
    }
    // Header layout puts mTracks at 0x118; retail keeps it at 0x120.
    for (int i = 0; i < TRACK_NUM; i++) {
        reinterpret_cast<SeqPlayerRetailSlots*>(this)->tracks[i] = NULL;
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
        FinishPlayerImpl(*this);
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
                reinterpret_cast<SeqTrack**>(this)[0x48 + i] = pTrack;
                pTrack->SetPlayerTrackNo(i);
            }
        }
    }

    DisposeCallbackManager::GetInstance().RegisterDisposeCallback(this);

    reinterpret_cast<SeqPlayerRetailSlots*>(this)->allocator = pAllocator;
    reinterpret_cast<SeqPlayerRetailSlots*>(this)->activeFlag = true;

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
        SeqTrack* pTrack =
            (i > TRACK_NUM - 1)
                ? NULL
                : reinterpret_cast<SeqTrack**>(this)[0x48 + i];

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

    if (reinterpret_cast<SeqPlayerRetailSlots*>(this)->activeFlag) {
        // First scan has no calls on the non-matching path, so MWCC walks it
        // entirely in volatile registers: the induction pointer is based at
        // the player with the 0x120 track-array base folded into the load.
        SeqTrack** pTracks = reinterpret_cast<SeqTrack**>(this);
        for (int i = 0; i < TRACK_NUM; i++) {
            SeqTrack* pTrack =
                (i > TRACK_NUM - 1) ? NULL : reinterpret_cast<SeqTrack**>(this)[0x48 + i];
            if (pTrack == NULL) {
                continue;
            }

            const u8* pBase =
                reinterpret_cast<SeqTrackRetailSlots*>(pTrack)->baseAddr;
            if (pStart <= pBase && pBase <= pEnd) {
                // SeqPlayer::Stop() inlined by retail.
                // Hoisted NULL for the track-slot store; retail pins it in
                // a callee-saved register across the close loop.
                SeqTrack* pRemovedTrack = NULL;

                {
                    SoundThread::AutoLock stopLock;

                    FinishPlayerImpl(*this);
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

    SeqTrack** tracks = (SeqTrack**)((u8*)this + 0x120);
    return tracks[idx];
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
        SeqTrack* pTrack =
            (i > TRACK_NUM - 1)
                ? NULL
                : reinterpret_cast<SeqPlayerRetailSlots*>(this)->tracks[i];
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

                SeqTrack* pClose =
                    (i > TRACK_NUM - 1)
                        ? NULL
                        : reinterpret_cast<SeqPlayerRetailSlots*>(this)
                              ->tracks[i];
                if (pClose == NULL) {
                    break;
                }

                pClose->Close();
                // Retail reloads the track slot for the FreeTrack argument
                // (pClose does not stay live across the Close call).
                reinterpret_cast<SeqPlayerRetailSlots*>(this)
                    ->allocator->FreeTrack(
                        reinterpret_cast<SeqPlayerRetailSlots*>(this)
                            ->tracks[i]);
                reinterpret_cast<SeqPlayerRetailSlots*>(this)->tracks[i] =
                    pRemovedTrack;
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

    if (mSkipTickCounter != 0 || mSkipTimeCounter > lbl_eu_8066A004) {
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
            SeqTrack* pTrack =
                (i > TRACK_NUM - 1)
                    ? NULL
                    : reinterpret_cast<SeqTrack**>(this)[0x48 + i];

            if (pTrack != NULL) {
                pTrack->UpdateChannelParam();
            }
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
    f32 tickPerMsec =
        mTempoRatio *
        ((f32)(conv.d - lbl_eu_8066A008) / lbl_eu_8066A010);
    if (lbl_eu_8066A004 == tickPerMsec) {
        return;
    }

    // Retail keeps the 0x43300000 constant live across the loop by writing
    // both union halves on each conversion.
    conv.w[0] = 0x43300000;
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
                    // Retail reloads the slot for FreeTrack (pClose does not
                    // stay live across Close).
                    reinterpret_cast<SeqPlayerRetailSlots*>(this)
                        ->allocator->FreeTrack(*pSlot);
                    *pSlot = NULL;
                }
            }
            return;
        }

        mTickCounter++;

        conv.w[0] = 0x43300000;
        conv.w[1] =
            (u32)(mParserParam.timebase * mParserParam.tempo) ^ 0x80000000;
        // Multiply-last ordering keeps this tree distinct from the head
        // computation so MWCC's PRE pass cannot merge them.
        tickPerMsec =
            ((f32)(conv.d - lbl_eu_8066A008) / lbl_eu_8066A010) *
            mTempoRatio;
        if (lbl_eu_8066A004 == tickPerMsec) {
            return;
        }

        nextMsec = lbl_eu_8066A000 / tickPerMsec;
    }

    nextMsec -= restMsec;
    mTickFraction = nextMsec * tickPerMsec;
}

void SeqPlayer::SkipTick() {
    // Release every voice on every open track before skipping ahead.
    for (int i = 0; i < TRACK_NUM; i++) {
        SeqTrack* pTrack =
            (i > TRACK_NUM - 1)
                ? NULL
                : reinterpret_cast<SeqPlayerRetailSlots*>(this)->tracks[i];

        if (pTrack != NULL) {
            pTrack->ReleaseAllChannel(127);
            pTrack->FreeAllChannel();
        }
    }

    // Base tempo: mTempoRatio * (timebase * tempo) / 60000, with the integer
    // product converted through the retail 2^52 pool constant.
    IntToF32Conv conv;

    while (reinterpret_cast<SeqPlayerRetailSlots*>(this)->skipTickCounter !=
               0 ||
           reinterpret_cast<SeqPlayerRetailSlots*>(this)->skipTimeCounter *
                   (reinterpret_cast<SeqPlayerRetailSlots*>(this)->tempoRatio *
                    ((f32)(LoadTempoProduct(
                               reinterpret_cast<SeqPlayerRetailSlots*>(this),
                               conv) -
                           lbl_eu_8066A008) /
                     lbl_eu_8066A010)) >=
               lbl_eu_8066A000) {
        if (reinterpret_cast<SeqPlayerRetailSlots*>(this)->tickCounter >=
            MAX_SKIP_TICK_PER_FRAME) {
            return;
        }

        // Consume one skipped tick, or the time equivalent of one tick.
        if (reinterpret_cast<SeqPlayerRetailSlots*>(this)->skipTickCounter !=
            0) {
            reinterpret_cast<SeqPlayerRetailSlots*>(this)->skipTickCounter--;
        } else {
            f64 d = LoadTempoProduct(
                reinterpret_cast<SeqPlayerRetailSlots*>(this), conv);
            reinterpret_cast<SeqPlayerRetailSlots*>(this)->skipTimeCounter -=
                lbl_eu_8066A000 /
                (reinterpret_cast<SeqPlayerRetailSlots*>(this)->tempoRatio *
                 ((f32)(d - lbl_eu_8066A008) / lbl_eu_8066A010));
        }

        if (ParseNextTick(false)) {
            // FinishPlayer inlined by retail.
            SoundThread::AutoLock finishLock;

            if (reinterpret_cast<SeqPlayerRetailSlots*>(this)->startedFlag) {
                // Checked downcast to the embedded PlayerCallback subobject.
                SoundThread::GetInstance().UnregisterPlayerCallback(
                    reinterpret_cast<SoundThread::PlayerCallback*>(
                        reinterpret_cast<char*>(this) + 0xE0));
                reinterpret_cast<SeqPlayerRetailSlots*>(this)->startedFlag =
                    false;
            }

            if (reinterpret_cast<SeqPlayerRetailSlots*>(this)->activeFlag) {
                DisposeCallbackManager::GetInstance()
                    .UnregisterDisposeCallback(reinterpret_cast<DisposeCallback*>(
                        reinterpret_cast<char*>(this) + 0xD4));
                reinterpret_cast<SeqPlayerRetailSlots*>(this)->activeFlag =
                    false;
            }

            for (int i = 0; i < TRACK_NUM; i++) {
                SoundThread::AutoLock trackLock;

                SeqTrack* pClose =
                    (i > TRACK_NUM - 1)
                        ? NULL
                        : reinterpret_cast<SeqPlayerRetailSlots*>(this)
                              ->tracks[i];
                if (pClose == NULL) {
                    continue;
                }

                pClose->Close();
                // Retail reloads the track slot for FreeTrack (pClose does
                // not stay live across Close).
                reinterpret_cast<SeqPlayerRetailSlots*>(this)->allocator
                    ->FreeTrack(
                        reinterpret_cast<SeqPlayerRetailSlots*>(this)
                            ->tracks[i]);
                reinterpret_cast<SeqPlayerRetailSlots*>(this)->tracks[i] = NULL;
            }
            return;
        }

        reinterpret_cast<SeqPlayerRetailSlots*>(this)->tickCounter++;
    }

    reinterpret_cast<SeqPlayerRetailSlots*>(this)->skipTimeCounter =
        lbl_eu_8066A004;
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
