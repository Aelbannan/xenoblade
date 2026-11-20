#include <nw4r/snd.h>
#include <nw4r/ut.h>

#include <climits>
#include <cstring>

// .sdata2 constant-pool entries referenced by UpdateVoiceParams.
extern const f32 lbl_eu_8066A060; // 0.0f
extern const f64 lbl_eu_8066A068; // 2^52 (signed int -> float magic)
extern const f64 lbl_eu_8066A070; // 2^52 (u8 -> float magic)
extern const f32 lbl_eu_8066A078; // 1.0f

// DvdFileStream RTTI type-info object (.sbss); LoadStreamData walks the
// runtime type-info chain against it (see ut_RuntimeTypeInfo.h).
extern const nw4r::ut::detail::RuntimeTypeInfo lbl_eu_80665550;

namespace nw4r {
namespace snd {
namespace detail {

u8 StrmPlayer::sLoadBuffer[LOAD_BUFFER_SIZE];
OSMutex StrmPlayer::sLoadBufferMutex;
bool StrmPlayer::sStaticInitFlag;

// SoundThread::mMutex (private in the locked header) at +0x354.
struct SoundThreadRetailLayout {
    u8 _pad0x0[0x354];
    OSMutex mMutex; // at 0x354
};

inline OSMutex* GetSoundThreadMutex() {
    return &reinterpret_cast<SoundThreadRetailLayout*>(&SoundThread::GetInstance())
                ->mMutex;
}

// Mirrors the locked header's RuntimeTypeInfo::IsDerivedFrom inline (early
// returns); the retail LoadStreamData walks the chain against lbl_eu_80665550
// instead of the C++ static member symbol.
inline bool TypeInfoIsDerivedFrom(const ut::detail::RuntimeTypeInfo* pInfo,
                                  const ut::detail::RuntimeTypeInfo* pTarget) {
    for (const ut::detail::RuntimeTypeInfo* pIt = pInfo; pIt != NULL;
         pIt = pIt->mParentTypeInfo) {
        if (pIt == pTarget) {
            return true;
        }
    }

    return false;
}

// Byte/int -> float through the shared 2^52 magic doubles, referenced by name
// so the .sdata2 relocs match retail (see ut_PackedFont.cpp F64Conv pattern).
inline f32 U8ToF32(u8 value) {
    union {
        f64 d;
        u32 w[2];
    } conv;
    conv.w[1] = value;
    conv.w[0] = 0x43300000;
    return (f32)(conv.d - lbl_eu_8066A070);
}

// The cast on its own: MWCC lowers (f32)(conv.d - magic) to the retail
// low-word fsubs only through this helper boundary (cf. ut_TextWriterBase
// ConvF32S); a direct inline cast emits a double fsub + frsp.
inline f32 DToF32(f64 d) {
    return (f32)(d - lbl_eu_8066A070);
}

inline f32 DToF32Signed(f64 d) {
    return (f32)(d - lbl_eu_8066A068);
}

inline f32 S32ToF32(s32 value) {
    union {
        f64 d;
        u32 w[2];
    } conv;
    conv.w[0] = 0x43300000;
    conv.w[1] = (u32)value ^ 0x80000000;
    return (f32)(conv.d - lbl_eu_8066A068);
}

// Voice pitch-modulation gate byte at retail offset +0xA1 (see snd_Voice.cpp
// field_0xA1); snd_Voice.h's stale layout lacks the byte at this offset.
struct VoicePitchGateLayout {
    u8 _pad0x0[0xA1];
    u8 field_0xA1; // at 0xA1
};

// ---------------------------------------------------------------------------
// Retail-symbol stand-ins for signatures the locked header cannot express.
// The identifiers are already MWCC-mangled (they end in __F<args>); C linkage
// keeps MWCC from prepending the namespace when the declaration sits inside
// nw4r::snd::detail (same convention as snd_StrmFile.cpp's entry points).
// Definitions live in this TU (SetupPlayer, StrmPlayer members) or in
// snd_StrmFile.cpp (StrmFileLoader reader entry points).
// ---------------------------------------------------------------------------
extern "C" bool ReadStrmInfo__Q44nw4r3snd6detail14StrmFileLoaderCFPQ54nw4r3snd6detail14StrmFileReader8StrmInfo(
    const StrmFileLoader* pLoader, StrmPlayerInfo* pStrmInfo);
extern "C" bool ReadStrmTrackInfo__Q44nw4r3snd6detail14StrmFileLoaderCFPQ54nw4r3snd6detail14StrmFileReader13StrmTrackInfoi(
    const StrmFileLoader* pLoader, StrmPlayerTrackInfo* pTrackInfo,
    int trackNo);
extern "C" int GetChannelCount__Q44nw4r3snd6detail14StrmFileLoaderCFv(
    const StrmFileLoader* pLoader);
extern "C" bool ReadAdpcmInfo__Q44nw4r3snd6detail14StrmFileLoaderCFPQ44nw4r3snd6detail10AdpcmParamPQ44nw4r3snd6detail14AdpcmLoopParami(
    const StrmFileLoader* pLoader, AdpcmParam* pParam,
    AdpcmLoopParam* pLoopParam, int channel);
extern "C" bool SetupPlayer__Q44nw4r3snd6detail10StrmPlayerFv(
    StrmPlayer* pStrmPlayer);
extern "C" bool CalcStartOffset__Q44nw4r3snd6detail10StrmPlayerFPlPUlPl(
    StrmPlayer* pStrmPlayer, s32* pBlockIndex, u32* pBlockOffset,
    s32* pLoopCount);
extern "C" void SetBiquadFilter__Q44nw4r3snd6detail5VoiceFif(Voice* pVoice,
                                                             int type,
                                                             f32 value);

// Defined later in this TU (retail-mangled member emitted as a free function).
extern "C" void UpdateVoiceParams__Q44nw4r3snd6detail10StrmPlayerFPQ54nw4r3snd6detail10StrmPlayer9StrmTrack(
    StrmPlayer* pStrmPlayer, StrmPlayer::PlayerTrack* pTrack);

// Shared disk-status probe: downcast the stream at +0x828 to DvdFileStream by
// walking the runtime type-info chain against lbl_eu_80665550, then report
// whether the DVD drive is busy. Used by UpdateBuffer / Update.
inline bool IsDvdStreamBusy(ut::FileStream* pFileStream) {
    // Stream result declared first steers callee-saved coloring (r29), the
    // type-info address second (r30); both arms assign so the failure paths
    // share one NULL store.
    ut::FileStream* pStream;
    const ut::detail::RuntimeTypeInfo* pDvdTypeInfo = &lbl_eu_80665550;

    if (pFileStream != NULL &&
        TypeInfoIsDerivedFrom(pFileStream->GetRuntimeTypeInfo(),
                              pDvdTypeInfo)) {
        pStream = pFileStream;
    } else {
        pStream = NULL;
    }

    // Named u32 local forces MWCC's branchless normalize idiom
    // (xori/cntlzw/slw/srwi for the unsigned >1 drive-busy test).
    // if/else (not init-then-if) puts the zero-store on the fallthrough
    // path after the branch, matching retail.
    u32 busy;
    if (pStream == NULL) {
        busy = 0;
    } else {
        busy = (u32)DVDGetDriveStatus() > 1;
    }
    return busy != 0;
}

// Retail SetTrackVolume(unsigned long, float) is a StrmPlayer member, but the
// stale header has no declaration for it; define the retail mangled symbol as
// a free function (same ABI: r3 = this, r4 = track, f1 = volume). Kept OUT of
// the anonymous namespace above so the symbol name is emitted verbatim.
void SetTrackVolume__Q44nw4r3snd6detail10StrmPlayerFUlf(StrmPlayer* pStrmPlayer,
                                                        unsigned long track,
                                                        f32 volume) {
    ut::AutoInterruptLock lock;

    StrmPlayer* self =
        pStrmPlayer;

    for (int i = 0; i < self->mTrackCount && track != 0; i++) {
        if (track & 1) {
            self->mTracks[i].volume = volume;
        }

        track >>= 1;
    }
}

StrmPlayer::StrmPlayer()
    : mSetupFlag(false), mActiveFlag(false), mFileStream(NULL) {
    // One-time init of the shared load-buffer mutex (plain linker label in
    // retail, guarded by an .sdata flag byte).
    // Qualified: snd_StrmPlayer.h also declares a (stale) class member of
    // this name, and unqualified lookup inside a member would pick it.
    if (StrmPlayer::sStaticInitFlag == 0) {
        OSInitMutex(&StrmPlayer::sLoadBufferMutex);
        StrmPlayer::sStaticInitFlag = 1;
    }

    mStrmDataLoadTaskPool.Create(
        mStrmDataLoadTaskArea, DATA_BLOCK_COUNT_MAX * sizeof(StrmDataLoadTask));
}

StrmPlayer::~StrmPlayer() {
    Shutdown();
}

StrmPlayer::PlayerTrack* StrmPlayer::GetPlayerTrack(int index) {
    if (index > 7) {
        return NULL;
    }

    // mTracks is at retail offset +0xB78; see layout-mirror note above.
    StrmPlayer* self = this;
    return reinterpret_cast<PlayerTrack*>(&self->mTracks[index]);
}

bool StrmPlayer::Setup(StrmBufferPool* pBufferPool) {
    SoundThread::AutoLock lock;

    if (mSetupFlag) {
        Shutdown();
    }

    InitParam();
    mBufferPool = pBufferPool;
    mSetupFlag = true;

    return true;
}

void StrmPlayer::Shutdown() {
    Stop();

    {
        SoundThread::AutoLock lock;

        if (!mSetupFlag) {
            return;
        }

        mBufferPool = NULL;
        mStrmDataLoadTaskPool.Destroy(
            mStrmDataLoadTaskArea, DATA_BLOCK_COUNT_MAX * sizeof(StrmDataLoadTask));

        mSetupFlag = false;
    }
}

bool StrmPlayer::Prepare(ut::FileStream* pFileStream, int voices,
                         StartOffsetType offsetType, int offset) {
    SoundThread::AutoLock lock;

    mFileStream = pFileStream;
    mVoiceOutCount = voices;
    mStartOffsetType = offsetType;
    mStartOffset = offset;

    mTaskErrorFlag = false;
    mTaskCancelFlag = false;
    mLoadingDelayFlag = false;
    mActiveFlag = true;
    SoundThread::GetInstance().RegisterPlayerCallback(this);

    StrmHeaderLoadTask* pTask = &mStrmHeaderLoadTask;
    pTask->strmPlayer = this;
    pTask->fileStream = mFileStream;
    pTask->startOffsetType = mStartOffsetType;
    pTask->startOffset = mStartOffset;
    TaskManager::GetInstance().AppendTask(pTask);

    return true;
}

// Retail 3-argument overload (no voices argument; mVoiceOutCount keeps its
// previous value).
bool StrmPlayer::Prepare(ut::FileStream* pFileStream,
                         StartOffsetType offsetType, int offset) {
    SoundThread::AutoLock lock;

    StrmPlayer* self = this;

    self->mFileStream = pFileStream;
    self->mStartOffsetType = offsetType;
    self->mStartOffset = offset;

    // Reset the task state flags and mark the player active.
    self->mTaskErrorFlag = false;
    self->mTaskCancelFlag = false;
    self->mLoadingDelayFlag = false;
    self->mActiveFlag = true;

    // Registering derives the SoundThread::PlayerCallback subobject pointer
    // from this (MWCC emits the multiple-inheritance adjust + null check).
    SoundThread::GetInstance().RegisterPlayerCallback(this);

    StrmPlayer::StrmHeaderLoadTask* pTask = &self->mStrmHeaderLoadTask;
    pTask->strmPlayer = this;
    pTask->fileStream = self->mFileStream;
    pTask->startOffsetType = self->mStartOffsetType;
    pTask->startOffset = self->mStartOffset;
    TaskManager::GetInstance().AppendTask(reinterpret_cast<Task*>(pTask));

    return true;
}

bool StrmPlayer::Start() {
    // Retail re-evaluates SoundThread::GetInstance() + the mutex offset at
    // every lock/unlock site (the address is not kept in a saved register).
    OSLockMutex(
        &reinterpret_cast<SoundThreadRetailLayout*>(&SoundThread::GetInstance())
             ->mMutex);

    StrmPlayer* self = this;

    if (!self->mPreparedFlag) {
        OSUnlockMutex(
            &reinterpret_cast<SoundThreadRetailLayout*>(
                 &SoundThread::GetInstance())
                 ->mMutex);
        return false;
    }

    if (!self->mStartedFlag) {
        // Declaration order drives MWCC callee-saved coloring (descending).
        u32 loopFlag;
        u32 loopStartSample;
        u32 loopEndSamples;
        StrmPlayer::PlayerTrack* pTrack;
        int i;
        int ch;
        StrmPlayerChannel* pChannel;
        u32 level;

        if (!AllocVoices(self->mVoiceOutCount)) {
            // Indexed access: MWCC strength-reduces to a this-based stride
            // register with the 0x838 displacement kept inline (retail shape).
            for (i = 0; i < self->mChannelCount; i++) {
                if (self->mChannels[i].bufferAddress != NULL) {
                    self->mBufferPool->Free(self->mChannels[i].bufferAddress);
                    self->mChannels[i].bufferAddress = NULL;
                }
            }

            OSUnlockMutex(
                &reinterpret_cast<SoundThreadRetailLayout*>(
                     &SoundThread::GetInstance())
                     ->mMutex);
            return false;
        }

        s32 blockIndex = 0;
        u32 blockOffset = 0;
        s32 loopCount = 0;
        if (!CalcStartOffset__Q44nw4r3snd6detail10StrmPlayerFPlPUlPl(
                this, &blockIndex, &blockOffset, &loopCount)) {
            OSUnlockMutex(
                &reinterpret_cast<SoundThreadRetailLayout*>(
                     &SoundThread::GetInstance())
                     ->mMutex);
            return false;
        }

        self->mLoopCounter += loopCount;

        // Loop-end sample count for the wave data: bytes * samples-per-byte
        // for the stream format (2 = PCM8, 1 = PCM16, 3 = ADPCM).
        u32 bytes = self->mDataBlockSize * self->mPlayingBufferBlockCount;

        loopEndSamples = 0;
        switch (self->mStrmInfo.format) {
        case 3:
            // DSP ADPCM: 8 bytes hold 14 samples; a partial frame decodes
            // two samples per byte after the first header byte.
            loopEndSamples = (bytes >> 3) * 14;
            {
                u32 rem = bytes & 7;
                if (rem != 0) {
                    loopEndSamples += (rem - 1) * 2;
                }
            }
            break;

        case 2:
            loopEndSamples = bytes;
            break;

        case 1:
            loopEndSamples = bytes >> 1;
            break;
        }

        // Hoisted wave-info constants (retail keeps these in callee-saved
        // registers across the track loop).
        loopFlag = true;
        loopStartSample = 0;

        // Walking track pointer (stride 0x38), as in retail.
        pTrack = self->mTracks;
        for (i = 0; i < self->mTrackCount; pTrack++, i++) {
            if (!pTrack->activeFlag) {
                continue;
            }

            WaveInfo waveInfo;
            waveInfo.sampleFormat =
                static_cast<SampleFormat>(self->mStrmInfo.format);
            waveInfo.loopFlag = loopFlag;
            waveInfo.numChannels = pTrack->fileInfo.channelCount;
            waveInfo.sampleRate = self->mStrmInfo.sampleRate;
            waveInfo.loopStart = loopStartSample;
            waveInfo.loopEnd = loopEndSamples;

            for (ch = 0; ch < pTrack->fileInfo.channelCount; ch++) {
                if (ch >= 2) {
                    pChannel = NULL;
                } else {
                    s32 channelIndex = pTrack->fileInfo.channelIndex[ch];
                    if (channelIndex >= 16) {
                        pChannel = NULL;
                    } else {
                        pChannel = &self->mChannels[channelIndex];
                    }
                }

                if (pChannel != NULL) {
                    ChannelParam& rParam = waveInfo.channelParam[ch];
                    rParam.dataAddr = pChannel->bufferAddress;
                    rParam.adpcmInfo.param = pChannel->adpcm.param;
                    rParam.adpcmInfo.loopParam = pChannel->adpcm.loopParam;
                    // Only the first byte of the buffer holds the ADPCM
                    // pred/scale; store it in the pred_scale slot.
                    rParam.adpcmInfo.param.pred_scale =
                        *reinterpret_cast<u8*>(pChannel->bufferAddress);
                }
            }

            level = OSDisableInterrupts();
            if (pTrack->voice != NULL) {
                pTrack->voice->Setup(waveInfo, blockOffset);
                pTrack->voice->SetVoiceType(AxVoice::VOICE_TYPE_STREAM);
                pTrack->voice->Start();
            }
            OSRestoreInterrupts(level);
        }

        if (blockIndex == self->mStrmInfo.numBlocks - 2) {
            UpdateDataLoopAddress(1);
        } else if (blockIndex == self->mStrmInfo.numBlocks - 1) {
            UpdateDataLoopAddress(0);
        }

        UpdatePauseStatus();
        self->mStartedFlag = true;
    }

    OSUnlockMutex(
        &reinterpret_cast<SoundThreadRetailLayout*>(&SoundThread::GetInstance())
             ->mMutex);
    return true;
}

void StrmPlayer::Stop() {
    StrmPlayer* self = this;

    // Free-loop walking track pointer, declared early: MWCC colors the
    // callee-saved locals in creation order.
    StrmPlayer::PlayerTrack* pTrack;

    {
        SoundThread::AutoLock lock;

        // Stop the voice on every player-track slot (fixed bound of 8, not
        // mTrackCount).
        for (int i = 0; i < 8; i++) {
            if (self->mTracks[i].activeFlag &&
                self->mTracks[i].voice != NULL) {
                self->mTracks[i].voice->Stop();
            }
        }

        if (self->mActiveFlag) {
            SoundThread::GetInstance().UnregisterPlayerCallback(this);
        }
    }

    TaskManager::GetInstance().CancelTask(
        reinterpret_cast<Task*>(&self->mStrmHeaderLoadTask));

    // Cancel every queued stream-data load task (interrupt-protected walk
    // from the back of the pending list).
    {
        ut::AutoInterruptLock lock;

        while (!self->mStrmDataLoadTaskList.IsEmpty()) {
            Task* pTask = &self->mStrmDataLoadTaskList.GetBack();
            TaskManager::GetInstance().CancelTask(pTask);
        }
    }

    // Release every allocated channel buffer.
    for (int i = 0; i < self->mChannelCount; i++) {
        if (self->mChannels[i].bufferAddress != NULL) {
            self->mBufferPool->Free(self->mChannels[i].bufferAddress);
            self->mChannels[i].bufferAddress = NULL;
        }
    }

    // Free every player-track voice.
    {
        ut::AutoInterruptLock lock;

        pTrack = self->mTracks;
        for (int i = 0; i < self->mTrackCount; i++) {
            if (pTrack->activeFlag && pTrack->voice != NULL) {
                pTrack->voice->Free();
                pTrack->voice = NULL;
            }
            pTrack++;
        }
    }

    {
        SoundThread::AutoLock lock;

        if (self->mFileStream != NULL) {
            self->mFileStream->Close();
            self->mFileStream = NULL;
        }
    }

    self->mStartedFlag = false;
    self->mPreparedFlag = false;
    self->mActiveFlag = false;
}

void StrmPlayer::Pause(bool flag) {
    // Retail re-evaluates SoundThread::GetInstance() + the mutex offset at
    // every lock/unlock site.
    OSLockMutex(
        &reinterpret_cast<SoundThreadRetailLayout*>(&SoundThread::GetInstance())
             ->mMutex);

    StrmPlayer* self = this;

    self->mPauseFlag = flag;

    if (flag) {
        self->mLoadWaitFlag = true;
    }

    UpdatePauseStatus();

    OSUnlockMutex(
        &reinterpret_cast<SoundThreadRetailLayout*>(&SoundThread::GetInstance())
             ->mMutex);
}

void StrmPlayer::InitParam() {
    BasicPlayer::InitParam();

    // The stale header cannot express the retail layout; reset every field
    // through the mirror struct in retail store order.
    StrmPlayer* self = this;

    self->mStartedFlag = false;
    self->mPreparedFlag = false;
    self->mLoadFinishFlag = false;

    self->mPauseFlag = false;
    self->mPauseStatus = false;

    self->mLoadWaitFlag = false;
    self->mNoRealtimeLoadFlag = false;

    self->mPlayFinishFlag = false;

    self->mSkipUpdateAdpcmLoop = false;
    self->mValidAdpcmLoop = false;

    self->mBufferAllocFlag = false;
    self->mLoopCounter = 0;

    self->mVoiceOutCount = 1;
    self->mCurBlockIndex = 0;

    for (int i = 0; i < 8; i++) {
        self->mTracks[i].activeFlag = false;
        self->mTracks[i].volume = lbl_eu_8066A078;
        self->mTracks[i].field_0x34 = lbl_eu_8066A060;
        self->mTracks[i].voice = NULL;
    }

    for (int i = 0; i < 16; i++) {
        self->mChannels[i].bufferAddress = NULL;
    }
}

bool StrmPlayer::LoadHeader(ut::FileStream* pFileStream,
                            StartOffsetType offsetType, int offset) {
    // Retail materializes the mutex address first (r31) and captures this
    // (r29) before locking; both stay in callee-saved registers for the whole
    // function (mr r3, r31 / bl OSUnlockMutex at every exit).
    // Retail keeps the mutex address in a saved register for the whole
    // function; declaration order drives callee-saved coloring (Rule A):
    // retail holds offset in r30 and this in r29.
    OSMutex* pMutex = &StrmPlayer::sLoadBufferMutex;
    s32 startOffset = offset;
    StrmPlayer* self = this;
    OSLockMutex(pMutex);

    StrmFileLoader loader(*pFileStream);
    if (!loader.LoadFileHeader(StrmPlayer::sLoadBuffer, 0x4000)) {
        OSUnlockMutex(pMutex);
        return false;
    }

    if (!ReadStrmInfo__Q44nw4r3snd6detail14StrmFileLoaderCFPQ54nw4r3snd6detail14StrmFileReader8StrmInfo(
            &loader, &self->mStrmInfo)) {
        OSUnlockMutex(&StrmPlayer::sLoadBufferMutex);
        return false;
    }

    if (self->mChannelCount == 0) {
        s32 channels =
            GetChannelCount__Q44nw4r3snd6detail14StrmFileLoaderCFv(&loader);
        if (channels > 16) {
            channels = 16;
        }

        self->mChannelCount = channels;
    }

    for (int i = 0; i < self->mTrackCount; i++) {
        // Address expressed this-relative (this + 0xB80 + i*0x38) so MWCC
        // strength-reduces into a stride register against this, matching
        // retail (add r4, this, stride / addi r4, r4, 0xB80).
        if (!ReadStrmTrackInfo__Q44nw4r3snd6detail14StrmFileLoaderCFPQ54nw4r3snd6detail14StrmFileReader13StrmTrackInfoi(
                &loader,
                reinterpret_cast<StrmPlayerTrackInfo*>(
                    reinterpret_cast<u8*>(self) +
                    i * sizeof(StrmPlayer::PlayerTrack) + 0xB80),
                i)) {
            OSUnlockMutex(pMutex);
            return false;
        }
    }

    // ADPCM streams carry per-channel adpcm data and a start-offset block.
    // Signed compare: retail emits the subi/cntlzw/srwi. equality idiom.
    if (static_cast<s32>(self->mStrmInfo.format) == 3) {
        for (int i = 0; i < self->mChannelCount; i++) {
            // Channel block addressed this-relative (see track-loop note);
            // adpcm param at +0x83C, loop param at +0x864.
            u8* pChannelBlock =
                reinterpret_cast<u8*>(self) + i * 0x34 + 0x838;
            if (!ReadAdpcmInfo__Q44nw4r3snd6detail14StrmFileLoaderCFPQ44nw4r3snd6detail10AdpcmParamPQ44nw4r3snd6detail14AdpcmLoopParami(
                    &loader, reinterpret_cast<AdpcmParam*>(pChannelBlock + 4),
                    reinterpret_cast<AdpcmLoopParam*>(pChannelBlock + 0x2C),
                    i)) {
                OSUnlockMutex(pMutex);
                return false;
            }
        }

        if (startOffset != 0) {
            // The start offset is already a sample index for SAMPLE streams;
            // MILLISEC streams convert it through the sample rate.
            if (offsetType == START_OFFSET_TYPE_SAMPLE) {
                startOffset = offset;
            } else if (offsetType == START_OFFSET_TYPE_MILLISEC) {
                startOffset = startOffset * self->mStrmInfo.sampleRate / 1000;
            }

            s32 block = startOffset / self->mStrmInfo.blockSamples;

            u16 yn1[16];
            u16 yn2[16];

            if (!loader.ReadAdpcBlockData(yn1, yn2, block,
                                          self->mStrmInfo.numChannels)) {
                OSUnlockMutex(pMutex);
                return false;
            }

            for (int i = 0; i < self->mStrmInfo.numChannels; i++) {
                // this-relative store addresses (this + 0x34*i + 0x860/0x862)
                // match retail's stride-walk fan-out.
                u8* pChannelBlock = reinterpret_cast<u8*>(self) + i * 0x34;
                *reinterpret_cast<u16*>(pChannelBlock + 0x860) = yn1[i];
                *reinterpret_cast<u16*>(pChannelBlock + 0x862) = yn2[i];
            }
        }
    }

    if (!SetupPlayer__Q44nw4r3snd6detail10StrmPlayerFv(this)) {
        OSUnlockMutex(pMutex);
        return false;
    }

    self->mPrepareCounter = 0;
    for (int i = 0; i < self->mBufferBlockCountBase; i++) {
        UpdateLoadingBlockIndex();
        self->mPrepareCounter++;

        if (self->mLoadFinishFlag) {
            break;
        }
    }

    if (self->mStrmInfo.numBlocks <= 2 && !self->mStrmInfo.loopFlag) {
        SetLoopEndToZeroBuffer(self->mStrmInfo.numBlocks - 1);
    }

    OSUnlockMutex(pMutex);
    return true;
}

// Retail DvdFileStream layout: the stream priority lives at +0x68 and the
// locked header has no accessor that compiles to the inline store.
struct DvdFileStreamRetailLayout {
    u8 _pad0x0[0x68];
    s32 priority; // at 0x68
};

bool StrmPlayer::LoadStreamData(ut::FileStream* pFileStream, int offset,
                                u32 size, u32 blockSize, int blockIndex,
                                bool needUpdateAdpcmLoop) {
    // Downcast to DvdFileStream by walking the retail runtime type-info chain
    // (lbl_eu_80665550 is the DvdFileStream type-info object). Retail hoists
    // the type-info address into a register before the null-check branch.
    ut::DvdFileStream* pDvdStream;
    const ut::detail::RuntimeTypeInfo* pDvdTypeInfo = &lbl_eu_80665550;
    if (pFileStream != NULL &&
        TypeInfoIsDerivedFrom(pFileStream->GetRuntimeTypeInfo(),
                              pDvdTypeInfo)) {
        pDvdStream = static_cast<ut::DvdFileStream*>(pFileStream);
    } else {
        pDvdStream = NULL;
    }

    if (pDvdStream != NULL) {
        reinterpret_cast<DvdFileStreamRetailLayout*>(pDvdStream)->priority =
            DVD_PRIO_HIGH;
    }

    // Retail keeps the mutex address in a saved register (mr r3, r28 / bl
    // OSUnlockMutex at every exit).
    OSMutex* pMutex = &StrmPlayer::sLoadBufferMutex;
    OSLockMutex(pMutex);
    DCInvalidateRange(StrmPlayer::sLoadBuffer, 0x4000);

    StrmPlayer* self = this;

    s32 streamOffset = offset + self->mStrmInfo.blockHeaderOffset;
    // Separate working pointer into the shared load buffer (retail keeps the
    // extern-array reference for Read and this pointer for the fan-out).
    u8* pLoadBuf = StrmPlayer::sLoadBuffer;
    u16 adpcmPredScale[16];
    u16* pPredScale = adpcmPredScale;

    DCInvalidateRange(StrmPlayer::sLoadBuffer, 0x4000);

    // Loads arrive in groups of up to 2 channels per stream block; each group
    // is read at streamOffset and fanned out to the channel buffers.
    for (int block = 0; block < self->mChannelCount;) {
        s32 blockCount = 2;
        if (block + blockCount > self->mChannelCount) {
            blockCount = self->mChannelCount - block;
        }

        u32 readSize = blockSize * blockCount;
        pFileStream->Seek(streamOffset, ut::FileStream::SEEK_ORIGIN_BEG);
        if (pFileStream->Read(StrmPlayer::sLoadBuffer, readSize) != readSize) {
            OSUnlockMutex(pMutex);
            return false;
        }

        for (int ch = 0; ch < blockCount; ch++) {
            if (needUpdateAdpcmLoop) {
                pPredScale[block] = pLoadBuf[blockSize * ch];
            }

            u8* pDst = reinterpret_cast<u8*>(
                           self->mChannels[block].bufferAddress) +
                       self->mDataBlockSize * blockIndex;
            std::memcpy(pDst, pLoadBuf + blockSize * ch, blockSize);
            DCFlushRange(pDst, blockSize);

            block++;
        }

        streamOffset += readSize;
    }

    if (needUpdateAdpcmLoop && self->mStrmInfo.format == 3) {
        for (int i = 0; i < self->mChannelCount && i < 16; i++) {
            self->mChannels[i].mAdpcmLoopPredScale = pPredScale[i];
        }

        self->mValidAdpcmLoop = true;
    }

    if (!self->mPreparedFlag) {
        self->mPrepareCounter--;
        if (self->mPrepareCounter == 0) {
            self->mPreparedFlag = true;
        }
    }

    OSUnlockMutex(pMutex);
    return true;
}

bool StrmPlayer::AllocChannels(int channels, int voices) {
    ut::AutoInterruptLock lock;

    for (int i = 0; i < channels; i++) {
        void* pBuffer = mBufferPool->Alloc();

        if (pBuffer == NULL) {
            for (int j = 0; j < i; j++) {
                mBufferPool->Free(mChannels[j].bufferAddress);
            }

            return false;
        }

        mChannels[i].bufferAddress = pBuffer;
    }

    Voice* pVoice = VoiceManager::GetInstance().AllocVoice(
        channels, voices, Voice::PRIORITY_MAX, VoiceCallbackFunc, this);

    if (pVoice == NULL) {
        for (int i = 0; i < channels; i++) {
            mBufferPool->Free(mChannels[i].bufferAddress);
        }

        return false;
    }

    mVoice = pVoice;
    return true;
}

void StrmPlayer::FreeChannels() {
    ut::AutoInterruptLock lock;

    for (int i = 0; i < mChannelCount; i++) {
        if (mChannels[i].bufferAddress != NULL) {
            mBufferPool->Free(mChannels[i].bufferAddress);

            mChannels[i].bufferAddress = NULL;
        }
    }

    mChannelCount = 0;

    if (mVoice != NULL) {
        mVoice->Free();
        mVoice = NULL;
    }
}

void StrmPlayer::Update() {
    StrmPlayer* self = this;

    if (!self->mActiveFlag) {
        return;
    }

    if (self->mTaskErrorFlag && !self->mTaskCancelFlag) {
        Stop();
        return;
    }

    // A dropped voice (track active with no voice) stops the whole player.
    // When the stream is not started the scan is skipped but processing
    // continues below (no early return).
    if (self->mStartedFlag) {
        StrmPlayer::PlayerTrack* pTrack = self->mTracks;
        for (int i = 0; i < self->mTrackCount; i++) {
            if (pTrack->activeFlag && pTrack->voice == NULL) {
                Stop();
                return;
            }
            pTrack++;
        }
    }

    if (self->mLoadWaitFlag && self->mStrmDataLoadTaskList.IsEmpty()) {
        if (!IsDvdStreamBusy(self->mFileStream)) {
            self->mLoadWaitFlag = false;
            UpdatePauseStatus();
        }
    }

    if (self->mLoadingDelayFlag) {
        self->mLoadingDelayFlag = false;
    }

    for (int i = 0; i < self->mTrackCount; i++) {
        UpdateVoiceParams__Q44nw4r3snd6detail10StrmPlayerFPQ54nw4r3snd6detail10StrmPlayer9StrmTrack(
            this, &self->mTracks[i]);
    }
}

void StrmPlayer::UpdateBuffer() {
    StrmPlayer* self = this;

    if (!self->mStartedFlag) {
        return;
    }

    if (!self->mTracks[0].activeFlag) {
        return;
    }

    Voice* pTrackVoice = self->mTracks[0].voice;
    if (pTrackVoice == NULL) {
        return;
    }

    bool dvdBusy = IsDvdStreamBusy(self->mFileStream);
    if (dvdBusy) {
        self->mLoadWaitFlag = true;
        UpdatePauseStatus();
    }

    if (!self->mPlayFinishFlag && !self->mNoRealtimeLoadFlag &&
        !self->mLoadWaitFlag) {
        // s32 local: retail compares the loop bound with a signed cmp.
        s32 block =
            pTrackVoice->GetCurrentPlayingSample() /
            self->mStrmInfo.blockSamples;

        // While (not do-while): retail jumps to the loop test first.
        while (self->mPlayingBufferBlockIndex != block) {
            if (!self->mLoadWaitFlag &&
                !self->mStrmDataLoadTaskList.IsEmpty() &&
                self->mCurBlockIndex >=
                    self->mBufferBlockCountBase - 2) {
                self->mLoadingDelayFlag = true;
                self->mLoadWaitFlag = true;
                UpdatePauseStatus();
                break;
            }

            UpdatePlayingBlockIndex();
            UpdateLoadingBlockIndex();
        }
    }
}

void StrmPlayer::UpdateLoopAddress(u32 startSample, u32 endSample) {
    // Retail holds the interrupt level in a saved register for the whole
    // function (mr r3, r31 / bl OSRestoreInterrupts at the single exit).
    u32 level = OSDisableInterrupts();

    StrmPlayer* self = this;

    // Field flags go through a walking track pointer; the channel-index
    // lookup walks a raw byte pointer from the player base -- each track's
    // fileInfo.channelIndex lives at player + 0xB88 + 0x38*i (track array at
    // 0xB78 + fileInfo offset 0x10), matching retail's second induction
    // register and folded load displacement.
    // Declaration order drives MWCC's callee-saved coloring.
    StrmPlayerChannel* pChannel;
    StrmPlayer::PlayerTrack* pTrackField;
    u8* pIdxWalk;
    int i;
    Voice* pVoice;
    int j;

    pTrackField = self->mTracks;
    pIdxWalk = reinterpret_cast<u8*>(self);

    for (i = 0; i < self->mTrackCount;
         pTrackField++, pIdxWalk += sizeof(StrmPlayer::PlayerTrack), i++) {
        if (!pTrackField->activeFlag) {
            continue;
        }

        pVoice = pTrackField->voice;
        if (pVoice == NULL) {
            continue;
        }

        for (j = 0; j < pTrackField->fileInfo.channelCount; j++) {
            // Per-channel record; channels beyond the first two or with an
            // out-of-range channel index resolve to NULL (the buffer address
            // is still loaded through the null pointer, as in retail).
            if (j >= 2) {
                pChannel = NULL;
            } else {
                // mTracks[i].fileInfo.channelIndex[j]
                int channelIndex = pIdxWalk[0xB88 + j];
                if (channelIndex >= 16) {
                    pChannel = NULL;
                } else {
                    pChannel = &self->mChannels[channelIndex];
                }
            }

            pVoice->SetLoopStart(j, pChannel->bufferAddress, startSample);
            pVoice->SetLoopEnd(j, pChannel->bufferAddress, endSample);
        }

        pVoice->SetLoopFlag(true);
    }

    OSRestoreInterrupts(level);
}

void StrmPlayer::UpdatePlayingBlockIndex() {
    StrmPlayer* self = this;

    self->mPlayingDataBlockIndex++;

    // Wrapped past the last data block: loop back to the loop-start block
    // (loop counter saturates at INT_MAX) and refresh the voice loop address.
    if (self->mPlayingDataBlockIndex > self->mLastBlockIndex &&
        self->mStrmInfo.loopFlag) {
        self->mPlayingDataBlockIndex = self->mLoopStartBlockIndex;

        if (self->mLoopCounter < INT_MAX) {
            self->mLoopCounter++;
        }

        UpdateLoopAddress(0, self->mPlayingBufferBlockCount *
                                 self->mStrmInfo.blockSamples);
    }

    self->mPlayingBufferBlockIndex++;

    if (self->mPlayingBufferBlockIndex >= self->mPlayingBufferBlockCount) {
        s32 loadingCount = self->mLoadingBufferBlockCount;
        self->mPlayingBufferBlockIndex = 0;
        self->mPlayingBufferBlockCount = loadingCount;

        UpdateLoopAddress(0, loadingCount * self->mStrmInfo.blockSamples);
    }

    // One block before the buffer end: refresh the ADPCM loop context of
    // every streaming ADPCM voice, then clear the pending-update flags.
    if (self->mPlayingBufferBlockIndex == self->mPlayingBufferBlockCount - 1)
    {
        // One block before the buffer end: refresh the ADPCM loop context of
        // every streaming ADPCM voice.
        if (!self->mSkipUpdateAdpcmLoop && self->mValidAdpcmLoop) {
            // Walking track pointer; declaration order drives MWCC's
            // callee-saved coloring.
            StrmPlayer::PlayerTrack* pTrack = self->mTracks;
            for (int i = 0; i < self->mTrackCount; pTrack++, i++) {
            if (!pTrack->activeFlag || pTrack->voice == NULL) {
                continue;
            }

            Voice* pVoice = pTrack->voice;
            if (pVoice->GetFormat() != SAMPLE_FORMAT_DSP_ADPCM) {
                continue;
            }

            u32 level = OSDisableInterrupts();

            for (int j = 0; j < pTrack->fileInfo.channelCount; j++) {
                // Channels beyond the first two or with an out-of-range
                // channel index resolve to NULL; retail still loads the
                // pred-scale through the (null) pointer.
                StrmPlayerChannel* pChannel;
                if (j >= 2) {
                    pChannel = NULL;
                } else {
                    // Signed local: retail compares channelIndex >= 16 with
                    // a signed cmpi (see AxVoice.cpp IsAdpcm note).
                    s32 channelIndex = pTrack->fileInfo.channelIndex[j];
                    if (channelIndex >= 16) {
                        pChannel = NULL;
                    } else {
                        pChannel = &self->mChannels[channelIndex];
                    }
                }

                AdpcmLoopParam loopParam;
                loopParam.loop_pred_scale = pChannel->mAdpcmLoopPredScale;
                loopParam.loop_yn1 = 0;
                loopParam.loop_yn2 = 0;

                pVoice->SetAdpcmLoop(j, &loopParam);
            }

            pVoice->SetVoiceType(AxVoice::VOICE_TYPE_STREAM);

            OSRestoreInterrupts(level);
            }
        }

        self->mValidAdpcmLoop = false;
        self->mSkipUpdateAdpcmLoop = false;
    }

    if (self->mPlayingDataBlockIndex == self->mLastBlockIndex - 1) {
        UpdateDataLoopAddress(self->mPlayingBufferBlockIndex + 1);
    }
}

void StrmPlayer::UpdateDataLoopAddress(s32 endBlock) {
    // Same idiom as SetLoopEndToZeroBuffer (byte-matched): self first, then
    // interrupt level, walking track pointer, inner-scope loop counters.
    StrmPlayer* self = this;

    if (self->mStrmInfo.loopFlag) {
        u32 level;
        s32 startBlock = endBlock + 1;
        if (startBlock >= self->mPlayingBufferBlockCount) {
            startBlock -= self->mPlayingBufferBlockCount;
        }

        level = OSDisableInterrupts();

        UpdateLoopAddress(startBlock * self->mStrmInfo.blockSamples,
                          self->mStrmInfo.lastBlockSamples +
                              endBlock * self->mStrmInfo.blockSamples);

        if (static_cast<s32>(self->mStrmInfo.format) == 3) {
            // Refresh the ADPCM loop context of every streaming voice.
            StrmPlayer::PlayerTrack* pTrack = self->mTracks;
            for (int i = 0; i < self->mTrackCount; pTrack++, i++) {
                if (!pTrack->activeFlag) {
                    continue;
                }

                Voice* pVoice = pTrack->voice;
                if (pVoice == NULL) {
                    continue;
                }

                if (pVoice->GetFormat() == 3) {
                    pVoice->SetVoiceType(AxVoice::VOICE_TYPE_NORMAL);

                    for (int j = 0; j < pTrack->fileInfo.channelCount;
                         j++) {
                        // Channels beyond the first two or with an
                        // out-of-range channel index resolve to NULL; the
                        // member offset of loopParam is still folded into
                        // the (null) pointer.
                        StrmPlayerChannel* pChannel;
                        if (j >= 2) {
                            pChannel = NULL;
                        } else {
                            s32 channelIndex =
                                pTrack->fileInfo.channelIndex[j];
                            if (channelIndex >= 16) {
                                pChannel = NULL;
                            } else {
                                pChannel =
                                    &self->mChannels[channelIndex];
                            }
                        }

                        pVoice->SetAdpcmLoop(j,
                                             &pChannel->adpcm.loopParam);
                    }
                }
            }

            if (endBlock == self->mPlayingBufferBlockCount - 1) {
                self->mSkipUpdateAdpcmLoop = true;
            }
        }

        OSRestoreInterrupts(level);
        return;
    }

    SetLoopEndToZeroBuffer(endBlock);
}

void StrmPlayer::SetLoopEndToZeroBuffer(int endBlock) {
    // Hoisted mirror pointer + walking track pointer; declaration order drives
    // MWCC's callee-saved coloring.
    StrmPlayer* self = this;

    u32 level = OSDisableInterrupts();

    StrmPlayer::PlayerTrack* pTrack = self->mTracks;
    for (int i = 0; i < self->mTrackCount; pTrack++, i++) {
        if (!pTrack->activeFlag) {
            continue;
        }

        Voice* pVoice = pTrack->voice;
        if (pVoice == NULL) {
            continue;
        }

        for (int j = 0; j < pTrack->fileInfo.channelCount; j++) {
            // Channels beyond the first two or with an out-of-range channel
            // index resolve to NULL; retail still loads the buffer address
            // through the (null) pointer.
            StrmPlayerChannel* pChannel;
            if (j >= 2) {
                pChannel = NULL;
            } else {
                int channelIndex = pTrack->fileInfo.channelIndex[j];
                if (channelIndex >= 16) {
                    pChannel = NULL;
                } else {
                    pChannel = &self->mChannels[channelIndex];
                }
            }

            pVoice->StopAtPoint(
                j, pChannel->bufferAddress,
                self->mStrmInfo.lastBlockSamples +
                    endBlock * self->mStrmInfo.blockSamples);
        }
    }

    OSRestoreInterrupts(level);

    self->mPlayFinishFlag = true;
}

void StrmPlayer::UpdateLoadingBlockIndex() {
    StrmPlayer* self = this;

    self->mCurBlockIndex++;

    if (self->mLoadFinishFlag) {
        return;
    }

    // The last data block loads the padded final block size.
    s32 blockSize;
    if (self->mLoadingDataBlockIndex <
        static_cast<s32>(self->mStrmInfo.numBlocks) - 1) {
        blockSize = self->mStrmInfo.blockSize;
    } else {
        blockSize = self->mStrmInfo.lastBlockPaddedSize;
    }

    // The per-block stream stride always uses the raw block size.
    u32 stride = self->mStrmInfo.blockHeaderOffset +
                 self->mStrmInfo.blockSize * self->mStrmInfo.numChannels;
    u32 size = self->mStrmInfo.blockHeaderOffset +
               blockSize * self->mChannelCount;
    s32 offset =
        self->mStrmInfo.dataOffset + self->mLoadingDataBlockIndex * stride;

    bool needUpdateAdpcmLoop = false;
    if (self->mLoadingBufferBlockIndex == 0 &&
        self->mStrmInfo.format == 3) {
        needUpdateAdpcmLoop = true;
    }

    StrmDataLoadTask* pTask = self->mStrmDataLoadTaskPool.Alloc();

    pTask->strmPlayer = this;
    pTask->fileStream = self->mFileStream;
    pTask->size = size;
    pTask->offset = offset;
    pTask->blockSize = blockSize;
    pTask->bufferBlockIndex = self->mLoadingBufferBlockIndex;
    pTask->needUpdateAdpcmLoop = needUpdateAdpcmLoop;

    u32 level = OSDisableInterrupts();

    self->mStrmDataLoadTaskList.PushBack(pTask);

    TaskManager::TaskPriority priority =
        self->mStartedFlag ? TaskManager::PRIORITY_HIGH
                           : TaskManager::PRIORITY_MIDDLE;

    TaskManager::GetInstance().AppendTask(pTask, priority);

    self->mLoadingDataBlockIndex++;

    if (self->mLoadingDataBlockIndex > self->mLastBlockIndex) {
        if (self->mStrmInfo.loopFlag) {
            self->mLoadingDataBlockIndex = self->mLoopStartBlockIndex;
        } else {
            self->mLoadFinishFlag = true;
            OSRestoreInterrupts(level);
            return;
        }
    }

    self->mLoadingBufferBlockIndex++;

    if (self->mLoadingBufferBlockIndex >= self->mLoadingBufferBlockCount) {
        self->mLoadingBufferBlockIndex = 0;
        self->mLoadingBufferBlockCount = CalcLoadingBufferBlockCount();
    }

    OSRestoreInterrupts(level);
}

void StrmPlayer::UpdatePauseStatus() {
    ut::AutoInterruptLock lock;

    bool pauseStatus = false;

    if (this->mPauseFlag) {
        pauseStatus = true;
    }

    if (this->mLoadWaitFlag) {
        pauseStatus = true;
    }

    if (pauseStatus !=
        this->mPauseStatus) {
        for (int i = 0;
             i < this->mTrackCount;
             i++) {
            if (this
                ->mTracks[i].activeFlag &&
                this
                    ->mTracks[i].voice != NULL) {
                this
                    ->mTracks[i].voice->Pause(pauseStatus);
            }
        }

        this->mPauseStatus =
            pauseStatus;
    }
}

int StrmPlayer::CalcLoadingBufferBlockCount() const {
    int restBlocks = (mLastBlockIndex - mLoadingDataBlockIndex) + 1;
    int loopBlocks = (mLastBlockIndex - mLoopStartBlockIndex) + 1;

    if ((mBufferBlockCountBase + 1 - restBlocks) % loopBlocks == 0) {
        return mBufferBlockCountBase + 1;
    }

    return mBufferBlockCountBase;
}

bool StrmPlayer::CalcStartOffset(s32* pBlockIndex, u32* pBlockOffset,
                                 s32* pLoopCount) {
    StrmPlayer* self = this;

    if (self->mStrmInfo.blockSamples == 0) {
        return false;
    }

    int startSample;
    if (self->mStartOffsetType == START_OFFSET_TYPE_SAMPLE) {
        startSample = self->mStartOffset;
    } else if (self->mStartOffsetType == START_OFFSET_TYPE_MILLISEC) {
        startSample = (self->mStartOffset *
                       static_cast<s64>(self->mStrmInfo.sampleRate)) /
                      1000;
    }

    *pLoopCount = 0;

    // Loop-end comparison is unsigned (cmplw in retail).
    if (static_cast<u32>(startSample) >= self->mStrmInfo.loopEnd) {
        if (self->mStrmInfo.loopFlag) {
            s32 loopStart = self->mStrmInfo.loopStart;
            s32 loopEnd = self->mStrmInfo.loopEnd;
            s32 loopLength = loopEnd - loopStart;

            s32 startOffset2 = startSample - loopEnd;
            s32 startLoop = startOffset2 / loopLength;
            *pLoopCount = startLoop + 1;

            startSample = loopStart + (startOffset2 - (startLoop * loopLength));
        } else {
            return false;
        }
    }

    *pBlockIndex = startSample / static_cast<s32>(self->mStrmInfo.blockSamples);

    // Block offset remainder is computed unsigned (divwu in retail).
    *pBlockOffset = static_cast<u32>(startSample) %
                    static_cast<u32>(self->mStrmInfo.blockSamples);

    return true;
}

void StrmPlayer::VoiceCallbackFunc(Voice* pDropVoice,
                                   Voice::VoiceCallbackStatus status,
                                   void* pCallbackArg) {
    // The voice manager passes the owning StrmTrack (not the player) as the
    // callback argument; dropping the voice clears track->voice (+0x4).
    StrmPlayer::PlayerTrack* pTrack = static_cast<StrmPlayer::PlayerTrack*>(pCallbackArg);
    ut::AutoInterruptLock lock;

    switch (status) {
    case Voice::CALLBACK_STATUS_FINISH_WAVE:
    case Voice::CALLBACK_STATUS_CANCEL: {
        pDropVoice->Free();
        pTrack->voice = NULL;
        break;
    }

    case Voice::CALLBACK_STATUS_DROP_VOICE:
    case Voice::CALLBACK_STATUS_DROP_DSP: {
        pTrack->voice = NULL;
        break;
    }

    default: {
        return;
    }
    }
}

void StrmPlayer::SetAdpcmLoopContext(int channels, u16* pPredScale) {
    if (mStrmInfo.format != WaveFile::FORMAT_ADPCM) {
        return;
    }

    for (int i = 0; i < channels && i < CHANNEL_MAX; i++) {
        mChannels[i].mAdpcmLoopPredScale = pPredScale[i];
    }

    mValidAdpcmLoop = true;
}

StrmPlayer::StrmHeaderLoadTask::StrmHeaderLoadTask()
    : strmPlayer(NULL), fileStream(NULL), startOffset(0) {}

void StrmPlayer::StrmHeaderLoadTask::Execute() {
    if (!strmPlayer->LoadHeader(fileStream, startOffsetType, startOffset)) {
        // mTaskErrorFlag is at retail offset +0x124; see layout-mirror note.
        strmPlayer->mTaskErrorFlag =
            true;
    }
}

void StrmPlayer::StrmHeaderLoadTask::Cancel() {}

void StrmPlayer::StrmHeaderLoadTask::OnCancel() {
    // mTaskCancelFlag is at retail offset +0x125; see layout-mirror note.
    strmPlayer->mTaskCancelFlag =
        true;

    if (fileStream != NULL && fileStream->CanCancel()) {
        if (fileStream->CanAsync()) {
            fileStream->CancelAsync(NULL, NULL);
        } else {
            fileStream->Cancel();
        }
    }
}

StrmPlayer::StrmDataLoadTask::StrmDataLoadTask()
    : strmPlayer(NULL),
      fileStream(NULL),
      size(0),
      offset(0),
      blockSize(0),
      bufferBlockIndex(-1),
      needUpdateAdpcmLoop(false) {}

void StrmPlayer::StrmDataLoadTask::Execute() {
    if (!strmPlayer->LoadStreamData(fileStream, offset, size, blockSize,
                                    bufferBlockIndex, needUpdateAdpcmLoop)) {
        // mTaskErrorFlag is at retail offset +0x124; see layout-mirror note.
        strmPlayer->mTaskErrorFlag =
            true;
    }

    ut::AutoInterruptLock lock;

    // List at +0x194 / pool at +0x1A0 are retail offsets; see layout-mirror
    // note. The pool Free inlines the deleting-dtor call plus FreeImpl, and
    // each statement re-reads strmPlayer so retail reloads the player pointer
    // from the task after the Erase call.
    strmPlayer
        ->mStrmDataLoadTaskList.Erase(this);
    strmPlayer
        ->mStrmDataLoadTaskPool.Free(this);

    // Decrement the pending load-task count at player +0x168 while the
    // interrupt lock is still held.
    strmPlayer->mCurBlockIndex--;
}

void StrmPlayer::StrmDataLoadTask::Cancel() {
    ut::AutoInterruptLock lock;

    // List at +0x194 / pool at +0x1A0 are retail offsets; see layout-mirror
    // note. Two separate strmPlayer reads so the member is reloaded after the
    // Erase call, matching retail.
    strmPlayer
        ->mStrmDataLoadTaskList.Erase(this);
    strmPlayer
        ->mStrmDataLoadTaskPool.Free(this);
}

void StrmPlayer::StrmDataLoadTask::OnCancel() {
    // mTaskCancelFlag is at retail offset +0x125; see layout-mirror note.
    strmPlayer->mTaskCancelFlag =
        true;

    if (fileStream != NULL && fileStream->CanCancel()) {
        if (fileStream->CanAsync()) {
            fileStream->CancelAsync(NULL, NULL);
        } else {
            fileStream->Cancel();
        }
    }
}

} // namespace detail
} // namespace snd
} // namespace nw4r

using nw4r::snd::detail::StrmPlayer;

// Referenced by AllocVoices below: the retail address of the StrmPlayer
// static voice callback. StrmPlayer::VoiceCallbackFunc is a private member, so
// it cannot be named from the global-scope AllocVoices free function; C
// linkage keeps the pre-mangled identifier verbatim (MWCC re-mangles C++
// free functions). Declaration only -- the definition is the member below.
extern "C" void VoiceCallbackFunc__Q44nw4r3snd6detail10StrmPlayerFPQ44nw4r3snd6detail5VoiceQ54nw4r3snd6detail5Voice19VoiceCallbackStatusPv(
    nw4r::snd::detail::Voice*, nw4r::snd::detail::Voice::VoiceCallbackStatus,
    void*);

bool StrmPlayer::AllocVoices(int voices) {
    StrmPlayer* self = this;

    // Retail uses a scope interrupt lock; the lock's scalarized mOldState
    // member occupies the last callee-saved slot.
    nw4r::ut::AutoInterruptLock lock;

    for (int i = 0; i < self->mTrackCount; i++) {
        nw4r::snd::detail::StrmPlayer::PlayerTrack* pTrack =
            &self->mTracks[i];
        if (pTrack->activeFlag) {
            nw4r::snd::detail::Voice* pVoice =
                nw4r::snd::detail::VoiceManager::GetInstance().AllocVoice(
                    pTrack->fileInfo.channelCount, voices, 0xFF,
                    &VoiceCallbackFunc__Q44nw4r3snd6detail10StrmPlayerFPQ44nw4r3snd6detail5VoiceQ54nw4r3snd6detail5Voice19VoiceCallbackStatusPv,
                    pTrack);

            if (pVoice == NULL) {
                // Allocation failed: free the voices already granted to
                // earlier tracks and bail out.
                for (int j = 0; j < i; j++) {
                    nw4r::snd::detail::StrmPlayer::PlayerTrack* pFreeTrack =
                        &self->mTracks[j];
                    if (pFreeTrack->voice != NULL) {
                        pFreeTrack->voice->Free();
                        pFreeTrack->voice = NULL;
                    }
                }

                return false;
            }

            pTrack->voice = pVoice;
            // Disable per-voice pitch modulation for streamed voices (see
            // snd_Voice.cpp field_0xA1 pitch-modulation gate).
            reinterpret_cast<nw4r::snd::detail::VoicePitchGateLayout*>(pVoice)
                ->field_0xA1 = 1;
        }
    }

    return true;
}
// Retail SetupPlayer() (no arguments) is a StrmPlayer member, but the stale
// header declares a different SetupPlayer(const StrmHeader*); emit the retail
// symbol as a free function (same ABI: r3 = this). Reads the already-populated
// mStrmInfo and allocates the channel buffers from the pool.
bool SetupPlayer__Q44nw4r3snd6detail10StrmPlayerFv(StrmPlayer* pStrmPlayer) {
    StrmPlayer* self = pStrmPlayer;

    u32 poolBlockSize = self->mBufferPool->GetBlockSize();

    s32 blockIndex = 0;
    u32 blockOffset = 0;
    s32 loopCount = 0;
    if (!CalcStartOffset__Q44nw4r3snd6detail10StrmPlayerFPlPUlPl(
            pStrmPlayer, &blockIndex, &blockOffset, &loopCount)) {
        return false;
    }

    self->mDataBlockSize = self->mStrmInfo.blockSize;
    s32 loopStartBlockIndex =
        self->mStrmInfo.loopStart / self->mStrmInfo.blockSamples;
    self->mLastBlockIndex = self->mStrmInfo.numBlocks - 1;
    self->mLoopStartBlockIndex = loopStartBlockIndex;

    if (self->mDataBlockSize > 0x2000) {
        return false;
    }

    self->mBufferBlockCount = poolBlockSize / self->mDataBlockSize;
    if (self->mBufferBlockCount < 4) {
        return false;
    }
    if (self->mBufferBlockCount > 0x20) {
        self->mBufferBlockCount = 0x20;
    }

    self->mBufferBlockCountBase = self->mBufferBlockCount - 1;
    self->mChangeNumBlocks = self->mBufferBlockCountBase;

    self->mPlayingDataBlockIndex = blockIndex;
    self->mLoadingDataBlockIndex = blockIndex;
    self->mLoadingBufferBlockIndex = 0;
    self->mPlayingBufferBlockIndex = 0;

    if (self->mNoRealtimeLoadFlag) {
        self->mLoadingBufferBlockCount = self->mStrmInfo.numBlocks;
    } else {
        // Retail sizes the preload from mBufferBlockCount here.
        s32 restBlocks =
            (self->mLastBlockIndex - self->mBufferBlockCount) + 1;

        s32 loadingCount = self->mBufferBlockCountBase;
        if ((self->mBufferBlockCountBase + 1 - restBlocks) %
                ((self->mLastBlockIndex - self->mLoopStartBlockIndex) + 1) ==
            0) {
            loadingCount = self->mBufferBlockCountBase + 1;
        }

        self->mLoadingBufferBlockCount = loadingCount;
    }

    self->mPlayingBufferBlockCount = self->mLoadingBufferBlockCount;

    u32 level = OSDisableInterrupts();
    if (!self->mBufferAllocFlag) {
        int ok;
        int i;
        for (i = 0; i < self->mChannelCount; i++) {
            void* pBuffer = self->mBufferPool->Alloc();
            if (pBuffer == NULL) {
                // Roll back the buffers granted so far.
                int j;
                for (j = 0; j < i; j++) {
                    self->mBufferPool->Free(
                        self->mChannels[j].bufferAddress);
                    self->mChannels[j].bufferAddress = NULL;
                }

                ok = 0;
                goto report;
            }

            self->mChannels[i].bufferAddress = pBuffer;
        }

        ok = 1;
    report:
        if (ok == 0) {
            OSRestoreInterrupts(level);
            return false;
        }

        self->mBufferAllocFlag = true;
    }
    OSRestoreInterrupts(level);
    return true;
}

// Retail 4-argument StrmPlayer::Setup(StrmBufferPool*, int, u16, int) is a
// member, but the stale header declares only the 1-argument overload; emit
// the retail symbol as a free function (same ABI).
// Retail 4-argument StrmPlayer::Setup(StrmBufferPool*, int, u16, int).
int StrmPlayer::Setup(StrmBufferPool* pBufferPool, int channels, u16 trackMask,
                      int voices) {
    StrmPlayer* self = this;

    SoundThread::AutoLock lock;

    if (self->mSetupFlag) {
        // Tear down any active playback first (Shutdown inlines the virtual
        // Stop plus the task-pool release under its own AutoLock).
        Shutdown();
    }

    InitParam();

    self->mChannelCount = channels > 16 ? 16 : channels;

    // Enable one player-track slot per set bit in the mask (max 8 slots).
    int count = 0;
    u32 mask = trackMask;
    while (mask != 0) {
        if (mask & 1) {
            if (count >= 8) {
                break;
            }
            self->mTracks[count].activeFlag = true;
        }

        mask >>= 1;
        count++;
    }

    int trackCount = count > 8 ? 8 : count;
    self->mTrackCount = trackCount;

    if (trackCount == 0) {
        return 2;
    }

    self->mVoiceOutCount = voices;
    self->mBufferPool = pBufferPool;

    u32 level = OSDisableInterrupts();

    if (self->mChannelCount > 0) {
        // Allocate one buffer per channel; on failure release every buffer
        // allocated so far and report error code 1.
        int ok;
        int i = 0;
        for (; i < self->mChannelCount; i++) {
            void* pBuffer = self->mBufferPool->Alloc();
            if (pBuffer == NULL) {
                for (int j = 0; j < i; j++) {
                    self->mBufferPool->Free(
                        self->mChannels[j].bufferAddress);
                    self->mChannels[j].bufferAddress = NULL;
                }

                ok = 0;
                goto report;
            }

            self->mChannels[i].bufferAddress = pBuffer;
        }

        ok = 1;

    report:
        if (ok == 0) {
            OSRestoreInterrupts(level);
            return 1;
        }

        self->mBufferAllocFlag = true;
    }

    OSRestoreInterrupts(level);

    self->mSetupFlag = true;

    return 0;
}

// Retail UpdateVoiceParams(StrmPlayer::StrmTrack*) is a StrmPlayer member, but
// the stale header has no declaration for it; emit the retail mangled symbol
// as a free function (r3 = this, r4 = track).
extern "C" void UpdateVoiceParams__Q44nw4r3snd6detail10StrmPlayerFPQ54nw4r3snd6detail10StrmPlayer9StrmTrack(
    StrmPlayer* pStrmPlayer,
    nw4r::snd::detail::StrmPlayer::PlayerTrack* pTrack) {
    if (!pTrack->activeFlag) {
        return;
    }

    StrmPlayer* self = pStrmPlayer;

    // Per-track mix, the nw4r detail idiom: seed accumulators from 1.0f/0.0f
    // and fold in each component. The u8 (file volume byte) and s32
    // (pan - 63/64) -> f32 conversions are MWCC-native 2^52-magic sequences
    // that pool into this unit's .sdata2, matching the retail pool exactly.
    f32 volume = 1.0f;
    volume *= self->GetVolume();
    volume *= pTrack->fileInfo.volume / 127.0f;
    volume *= pTrack->volume;

    f32 pitch = 1.0f;
    pitch *= self->GetPitch();

    f32 pan = 0.0f;
    pan += self->GetPan();
    // Pan bytes 0/1 select the hard-left path (retail keeps it as the
    // fall-through branch of the inverted test).
    if (pTrack->fileInfo.pan <= 1) {
        pan += (pTrack->fileInfo.pan - 63) / 63.0f;
    } else {
        pan += (pTrack->fileInfo.pan - 64) / 63.0f;
    }
    pan += pTrack->field_0x34;

    f32 surroundPan = 0.0f;
    surroundPan += self->GetSurroundPan();

    f32 lpfFreq = 1.0f;
    lpfFreq += self->GetLpfFreq();

    f32 biquadValue = self->GetBiquadFilterValue();
    u8 remoteFilter = self->GetRemoteFilter();
    u8 biquadType = self->GetBiquadFilterType();

    f32 mainSend = 0.0f;
    mainSend += self->GetMainSend();

    f32 fxSend[nw4r::snd::AUX_BUS_NUM];
    for (int i = 0; i < nw4r::snd::AUX_BUS_NUM; i++) {
        fxSend[i] = 0.0f;
        fxSend[i] += pStrmPlayer->GetFxSend(static_cast<nw4r::snd::AuxBus>(i));
    }

    f32 remoteOutVolume[WPAD_MAX_CONTROLLERS];
    f32 remoteSend[WPAD_MAX_CONTROLLERS];
    f32 remoteFxSend[WPAD_MAX_CONTROLLERS];
    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        remoteOutVolume[i] = pStrmPlayer->GetRemoteOutVolume(i);

        remoteSend[i] = 0.0f;
        remoteSend[i] += pStrmPlayer->GetRemoteSend(i);

        remoteFxSend[i] = 0.0f;
        remoteFxSend[i] += pStrmPlayer->GetRemoteFxSend(i);
    }

    // The voice cursor is declared before the interrupt level so MWCC
    // creates (and colors) it first.
    nw4r::snd::detail::Voice* pVoice;

    u32 level = OSDisableInterrupts();
    pVoice = pTrack->voice;
    if (pVoice != NULL) {
        pVoice->SetVolume(volume);
        pVoice->SetPitch(pitch);
        pVoice->SetPan(pan);
        pVoice->SetSurroundPan(surroundPan);
        pVoice->SetLpfFreq(lpfFreq);
        SetBiquadFilter__Q44nw4r3snd6detail5VoiceFif(pVoice, biquadType,
                                                    biquadValue);
        pVoice->SetRemoteFilter(remoteFilter);
        pVoice->SetOutputLine(self->GetOutputLine());
        pVoice->SetMainOutVolume(self->GetMainOutVolume());
        pVoice->SetMainSend(mainSend);

        for (int i = 0; i < nw4r::snd::AUX_BUS_NUM; i++) {
            pVoice->SetFxSend(static_cast<nw4r::snd::AuxBus>(i), fxSend[i]);
        }

        for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            pVoice->SetRemoteOutVolume(i, remoteOutVolume[i]);
            pVoice->SetRemoteSend(i, remoteSend[i]);
            pVoice->SetRemoteFxSend(i, remoteFxSend[i]);
        }

        for (int i = 0; i < self->mVoiceOutCount; i++) {
            pVoice->SetVoiceOutParam(i, self->GetVoiceOutParam(i));
        }
    }
    OSRestoreInterrupts(level);
}
extern "C" void OnUpdateFrameSoundThread__Q44nw4r3snd6detail10StrmPlayerFv() {}
extern "C" void OnUpdateVoiceSoundThread__Q44nw4r3snd6detail10StrmPlayerFv() {}

extern "C" u8 IsPause__Q44nw4r3snd6detail10StrmPlayerCFv(StrmPlayer* self) { return self->IsPause(); }
extern "C" u8 IsStarted__Q44nw4r3snd6detail10StrmPlayerCFv(StrmPlayer* self) { return self->IsStarted(); }
extern "C" u8 IsActive__Q44nw4r3snd6detail10StrmPlayerCFv(StrmPlayer* self) { return self->IsActive(); }
unsigned char absorb_pad_snd_StrmPlayer_sbss_4[4];

