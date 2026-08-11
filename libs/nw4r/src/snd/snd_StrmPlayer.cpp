#include <nw4r/snd.h>
#include <nw4r/ut.h>

#include <climits>
#include <cstring>

// ---------------------------------------------------------------------------
// Retail data-symbol imports. The stale header declares the load buffer and
// its mutex as C++ static members (mangled names); retail references the
// plain linker labels, so the functions below use these directly.
// ---------------------------------------------------------------------------
extern u8 lbl_eu_8064FE00[0x4000]; // StrmPlayer load buffer (.bss)
extern OSMutex lbl_eu_80653E00;    // StrmPlayer load-buffer mutex (.bss)

// .sdata2 constant-pool entries referenced by UpdateVoiceParams.
extern const f32 lbl_eu_8066A060; // 0.0f
extern const f64 lbl_eu_8066A068; // 2^52 (signed int -> float magic)
extern const f64 lbl_eu_8066A070; // 2^52 (u8 -> float magic)
extern const f32 lbl_eu_8066A078; // 1.0f
extern const f32 lbl_eu_8066A07C; // 100.0f
extern const f32 lbl_eu_8066A080; // 64.0f

// DvdFileStream RTTI type-info object (.sbss); LoadStreamData walks the
// runtime type-info chain against it (see ut_RuntimeTypeInfo.h).
extern const nw4r::ut::detail::RuntimeTypeInfo lbl_eu_80665550;

namespace nw4r {
namespace snd {
namespace detail {

u8 StrmPlayer::sLoadBuffer[LOAD_BUFFER_SIZE] ALIGN(32);
OSMutex StrmPlayer::sLoadBufferMutex;

bool StrmPlayer::sStaticInitFlag = false;

/******************************************************************************
 * Retail-layout mirror
 *
 * The shared header (snd_StrmPlayer.h) currently defines a STALE StrmPlayer
 * layout vs retail main.dol: mPlayerTracks at +0x910 instead of +0xB78, task
 * flags at +0x184/+0x185 instead of +0x124/+0x125, the data-load task
 * list/pool at +0x1F0/+0x1FC instead of +0x194/+0x1A0, and no track count at
 * +0x82C. The header is outside this session's writable scope, so the
 * functions below express the RETAIL field offsets through this mirror struct
 * instead. When the header layout is corrected, drop the mirror and use the
 * real members again.
 *
 * Offsets verified against build/us/asm/nw4r/src/snd/snd_StrmPlayer.s:
 *   StrmTrack volume at +0x30 (stfs at +0xBA8 in SetTrackVolume),
 *   mTaskErrorFlag at +0x124 / mTaskCancelFlag at +0x125 (stb in task
 *   Execute/OnCancel), track count at +0x82C (lwz in SetTrackVolume),
 *   mStrmDataLoadTaskList at +0x194 / pool at +0x1A0 (Erase/FreeImpl in
 *   StrmDataLoadTask::Cancel), mPlayerTracks at +0xB78 (GetPlayerTrack).
 ******************************************************************************/
namespace {

// Retail StrmInfo layout (Xenoblade): u32 format, u8 loopFlag, u32
// numChannels, ... - the stale header's StrmInfo has a different packed
// layout, so StrmPlayer stream state is read through this mirror.
struct StrmInfoRetailLayout {
    u32 format;                  // at 0x0
    u8 loopFlag;                 // at 0x4
    u8 _pad0x5[3];               // at 0x5
    u32 numChannels;             // at 0x8
    s32 sampleRate;              // at 0xC
    u16 blockHeaderOffset;       // at 0x10
    u16 _pad0x12;                // at 0x12
    u32 loopStart;               // at 0x14
    u32 loopEnd;                 // at 0x18
    u32 dataOffset;              // at 0x1C
    u32 numBlocks;               // at 0x20
    u32 blockSize;               // at 0x24
    u32 blockSamples;            // at 0x28
    u32 lastBlockSize;           // at 0x2C
    u32 lastBlockSamples;        // at 0x30
    u32 lastBlockPaddedSize;     // at 0x34
    u32 adpcmDataInterval;       // at 0x38
    u32 adpcmDataSize;           // at 0x3C
};                               // sizeof 0x40

// nw4r::snd::VoiceOutParam (6 floats, 0x18 bytes). The stale BasicPlayer
// header stores these as an anonymous mUnk0x6C[24] float array at +0x70.
struct VoiceOutParamRetail {
    f32 volume;       // at 0x0
    f32 pitch;        // at 0x4
    f32 pan;          // at 0x8
    f32 surroundPan;  // at 0xC
    f32 lpfFreq;      // at 0x10
    f32 remoteFilter; // at 0x14
};                    // sizeof 0x18

// AdpcmParam + AdpcmLoopParam as one 0x2E-byte block; both StrmChannel and
// the stream WaveInfo channel params carry it and it is copied in one piece
// (see Start()).
struct AdpcmParamSetRetail {
    AdpcmParam param;         // at 0x0 (0x28)
    AdpcmLoopParam loopParam; // at 0x28 (0x6)
};                            // sizeof 0x2E

// Retail StrmChannel: the stale header's bufferSize/adpcmInfo layout differs.
struct StrmChannelRetail {
    void* bufferAddress;       // at 0x0
    AdpcmParamSetRetail adpcm; // at 0x4 (0x2E)
    u16 mAdpcmLoopPredScale;   // at 0x32 (pred-scale slot used by loads)
};                             // sizeof 0x34

// StrmFileReader::StrmTrackInfo - the per-track file info block written by
// ReadStrmTrackInfo (volume/pan/channelCount/channelIndex), at track + 0x8.
struct StrmTrackFileInfoRetail {
    u8 volume;             // at 0x0 (track + 0x8)
    u8 pan;                // at 0x1 (track + 0x9)
    u8 _pad0x2[2];         // at 0x2
    int channelCount;      // at 0x4 (track + 0xC)
    u8 channelIndex[0x20]; // at 0x8 (track + 0x10)
};                         // sizeof 0x28

struct StrmTrackRetailLayout {
    u8 activeFlag;               // at 0x0
    u8 _pad0x1[3];               // at 0x1
    Voice* voice;                // at 0x4
    StrmTrackFileInfoRetail fileInfo; // at 0x8 (0x28)
    f32 volume;                  // at 0x30
    f32 field_0x34;              // at 0x34
};                               // sizeof 0x38

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

inline f32 S32ToF32(s32 value) {
    union {
        f64 d;
        u32 w[2];
    } conv;
    conv.w[0] = 0x43300000;
    conv.w[1] = (u32)value ^ 0x80000000;
    return (f32)(conv.d - lbl_eu_8066A068);
}

// Retail stream WaveInfo (Voice::Setup's parameter): u32 numChannels /
// sampleRate (the stale header's WaveData layout differs).
struct WaveChannelParamRetail {
    void* dataAddr;            // at 0x0
    AdpcmParamSetRetail adpcm; // at 0x4 (0x2E)
    u16 _pad0x32;              // at 0x32
};                             // sizeof 0x34

struct WaveInfoRetailLayout {
    u32 sampleFormat;                    // at 0x0
    u8 loopFlag;                         // at 0x4
    u8 _pad0x5[3];                       // at 0x5
    u32 numChannels;                     // at 0x8
    u32 sampleRate;                      // at 0xC
    u32 loopStart;                       // at 0x10
    u32 loopEnd;                         // at 0x14
    WaveChannelParamRetail channelParam[2]; // at 0x18 (2 * 0x34)
};                                       // sizeof 0x80

struct StrmPlayerRetailLayout {
    // BasicPlayer param block (retail PlayerParamSet at +0x4).
    u8 _pad0x0[0x4];                       // 0x000..0x004
    f32 mVolume;                           // at 0x004
    f32 mPitch;                            // at 0x008
    f32 mPan;                              // at 0x00C
    f32 mSurroundPan;                      // at 0x010
    f32 mLpfFreq;                          // at 0x014
    f32 mBiquadFilterValue;                // at 0x018
    u8 mBiquadFilterType;                  // at 0x01C
    u8 mRemoteFilter;                      // at 0x01D
    u8 _pad0x1E[2];                        // 0x01E..0x020
    s32 mOutputLine;                       // at 0x020
    f32 mMainOutVolume;                    // at 0x024
    f32 mMainSend;                         // at 0x028
    u8 _pad0x2C[0x70 - 0x2C];              // 0x02C..0x070
    VoiceOutParamRetail mVoiceOutParam[4]; // at 0x070 (4 * 0x18)
    u8 _pad0xD0[0xE0 - 0xD0];              // 0x0D0..0x0E0
    StrmInfoRetailLayout mStrmInfo;        // at 0x0E0 (0x40)
    bool mSetupFlag;                       // at 0x120
    bool mActiveFlag;                      // at 0x121
    bool mStartedFlag;                     // at 0x122
    bool mPreparedFlag;                    // at 0x123
    bool mTaskErrorFlag;                   // at 0x124
    bool mTaskCancelFlag;                  // at 0x125
    u8 _pad0x126[0x12A - 0x126];           // 0x126..0x12A
    bool mNoRealtimeLoadFlag;              // at 0x12A
    u8 _pad0x12B[0x12C - 0x12B];           // 0x12B..0x12C
    bool mValidAdpcmLoop;                  // at 0x12C
    u8 _pad0x12D[0x12E - 0x12D];           // 0x12D..0x12E
    bool mLoadFinishFlag;                  // at 0x12E
    bool mBufferAllocFlag;                 // at 0x12F
    s32 mLoopCounter;                      // at 0x130
    s32 mPrepareCounter;                   // at 0x134
    s32 mChangeNumBlocks;                  // at 0x138
    s32 mDataBlockSize;                    // at 0x13C
    s32 mBufferBlockCount;                 // at 0x140
    s32 mBufferBlockCountBase;             // at 0x144
    s32 mLoadingBufferBlockCount;          // at 0x148
    s32 mLoadingBufferBlockIndex;          // at 0x14C
    s32 mLoadingDataBlockIndex;            // at 0x150
    s32 mPlayingBufferBlockCount;          // at 0x154
    s32 mPlayingBufferBlockIndex;          // at 0x158
    s32 mPlayingDataBlockIndex;            // at 0x15C
    s32 mLoopStartBlockIndex;              // at 0x160
    s32 mLastBlockIndex;                   // at 0x164
    u8 _pad0x168[0x194 - 0x168];           // 0x168..0x194
    // StrmPlayer::StrmDataLoadTaskList (private typedef) expanded here; fully
    // qualified because a namespace-scope friend decl shadows the nested type.
    nw4r::ut::LinkList<StrmPlayer::StrmDataLoadTask,
                       offsetof(StrmPlayer::StrmDataLoadTask, node)>
        mStrmDataLoadTaskList;              // at 0x194 (0xC)
    InstancePool<StrmPlayer::StrmDataLoadTask> mStrmDataLoadTaskPool; // at 0x1A0 (0x4)
    u8 _pad0x1A4[0x824 - 0x1A4];            // 0x1A4..0x824
    StrmBufferPool* mBufferPool;            // at 0x824
    Voice* mVoice;                          // at 0x828
    s32 mTrackCount;                        // at 0x82C
    s32 mChannelCount;                      // at 0x830
    s32 mVoiceOutCount;                     // at 0x834
    StrmChannelRetail mChannels[16];        // at 0x838 (16 * 0x34 = 0x340)
    StrmTrackRetailLayout mTracks[8];       // at 0xB78 (8 * 0x38 = 0x1C0)
};                                          // sizeof 0xD38

// Voice pitch-modulation gate byte at retail offset +0xA1 (see snd_Voice.cpp
// field_0xA1); snd_Voice.h's stale layout lacks the byte at this offset.
struct VoicePitchGateLayout {
    u8 _pad0x0[0xA1];
    u8 field_0xA1; // at 0xA1
};

} // namespace

// ---------------------------------------------------------------------------
// Retail-symbol stand-ins for signatures the locked header cannot express.
// The identifiers are already MWCC-mangled (they end in __F<args>); C linkage
// keeps MWCC from prepending the namespace when the declaration sits inside
// nw4r::snd::detail (same convention as snd_StrmFile.cpp's entry points).
// Definitions live in this TU (SetupPlayer, StrmPlayer members) or in
// snd_StrmFile.cpp (StrmFileLoader reader entry points).
// ---------------------------------------------------------------------------
extern "C" bool ReadStrmInfo__Q44nw4r3snd6detail14StrmFileLoaderCFPQ54nw4r3snd6detail14StrmFileReader8StrmInfo(
    const StrmFileLoader* pLoader, StrmInfoRetailLayout* pStrmInfo);
extern "C" bool ReadStrmTrackInfo__Q44nw4r3snd6detail14StrmFileLoaderCFPQ54nw4r3snd6detail14StrmFileReader13StrmTrackInfoi(
    const StrmFileLoader* pLoader, StrmTrackFileInfoRetail* pTrackInfo,
    int trackNo);
extern "C" int GetChannelCount__Q44nw4r3snd6detail14StrmFileLoaderCFv(
    const StrmFileLoader* pLoader);
extern "C" bool ReadAdpcmInfo__Q44nw4r3snd6detail14StrmFileLoaderCFPQ44nw4r3snd6detail10AdpcmParamPQ44nw4r3snd6detail14AdpcmLoopParami(
    const StrmFileLoader* pLoader, AdpcmParam* pParam,
    AdpcmLoopParam* pLoopParam, int channel);
extern "C" bool SetupPlayer__Q44nw4r3snd6detail10StrmPlayerFv(
    StrmPlayer* pStrmPlayer);
extern "C" bool AllocVoices__Q44nw4r3snd6detail10StrmPlayerFi(
    StrmPlayer* pStrmPlayer, int voices);
extern "C" bool CalcStartOffset__Q44nw4r3snd6detail10StrmPlayerFPlPUlPl(
    StrmPlayer* pStrmPlayer, s32* pBlockIndex, u32* pBlockOffset,
    s32* pLoopCount);
extern "C" void Setup__Q44nw4r3snd6detail5VoiceFRCQ44nw4r3snd6detail8WaveInfoUl(
    Voice* pVoice, const WaveInfoRetailLayout& rWaveInfo, u32 offset);
extern "C" void SetBiquadFilter__Q44nw4r3snd6detail5VoiceFif(Voice* pVoice,
                                                             int type,
                                                             f32 value);
extern "C" void SetVoiceOutParam__Q44nw4r3snd6detail5VoiceFiRCQ34nw4r3snd13VoiceOutParam(
    Voice* pVoice, int index, const VoiceOutParamRetail& rParam);

// Retail SetTrackVolume(unsigned long, float) is a StrmPlayer member, but the
// stale header has no declaration for it; define the retail mangled symbol as
// a free function (same ABI: r3 = this, r4 = track, f1 = volume). Kept OUT of
// the anonymous namespace above so the symbol name is emitted verbatim.
void SetTrackVolume__Q44nw4r3snd6detail10StrmPlayerFUlf(StrmPlayer* pStrmPlayer,
                                                        unsigned long track,
                                                        f32 volume) {
    ut::AutoInterruptLock lock;

    StrmPlayerRetailLayout* self =
        reinterpret_cast<StrmPlayerRetailLayout*>(pStrmPlayer);

    for (int i = 0; i < self->mTrackCount && track != 0; i++) {
        if (track & 1) {
            self->mTracks[i].volume = volume;
        }

        track >>= 1;
    }
}

StrmPlayer::StrmPlayer()
    : mSetupFlag(false), mActiveFlag(false), mFileStream(NULL), mVoice(NULL) {

    if (!sStaticInitFlag) {
        OSInitMutex(&sLoadBufferMutex);
        sStaticInitFlag = true;
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

    // mPlayerTracks is at retail offset +0xB78; see layout-mirror note above.
    StrmPlayerRetailLayout* self =
        reinterpret_cast<StrmPlayerRetailLayout*>(this);
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

bool StrmPlayer::Start() {
    // Retail re-evaluates SoundThread::GetInstance() + the mutex offset at
    // every lock/unlock site (the address is not kept in a saved register).
    OSLockMutex(
        &reinterpret_cast<SoundThreadRetailLayout*>(&SoundThread::GetInstance())
             ->mMutex);

    StrmPlayerRetailLayout* self =
        reinterpret_cast<StrmPlayerRetailLayout*>(this);

    if (!self->mPreparedFlag) {
        OSUnlockMutex(
            &reinterpret_cast<SoundThreadRetailLayout*>(
                 &SoundThread::GetInstance())
                 ->mMutex);
        return false;
    }

    if (!self->mStartedFlag) {
        if (!AllocVoices__Q44nw4r3snd6detail10StrmPlayerFi(
                this, self->mVoiceOutCount)) {
            for (int i = 0; i < self->mChannelCount; i++) {
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
        u32 loopEndSamples;
        if (self->mStrmInfo.format == 2) {
            loopEndSamples = bytes;
        } else if (self->mStrmInfo.format >= 3) {
            loopEndSamples = (bytes >> 3) * 14;
            u32 rem = bytes & 7;
            if (rem != 0) {
                loopEndSamples += (rem - 1) * 2;
            }
        } else if (self->mStrmInfo.format == 1) {
            loopEndSamples = bytes >> 1;
        } else {
            loopEndSamples = 0;
        }

        for (int i = 0; i < self->mTrackCount; i++) {
            StrmTrackRetailLayout& rTrack = self->mTracks[i];
            if (!rTrack.activeFlag) {
                continue;
            }

            WaveInfoRetailLayout waveInfo;
            waveInfo.sampleFormat = self->mStrmInfo.format;
            waveInfo.loopFlag = true;
            waveInfo.numChannels = rTrack.fileInfo.channelCount;
            waveInfo.sampleRate = self->mStrmInfo.sampleRate;
            waveInfo.loopStart = 0;
            waveInfo.loopEnd = loopEndSamples;

            for (int ch = 0; ch < rTrack.fileInfo.channelCount; ch++) {
                StrmChannelRetail* pChannel;
                if (ch >= 2) {
                    pChannel = NULL;
                } else {
                    u8 channelIndex = rTrack.fileInfo.channelIndex[ch];
                    if (channelIndex >= 16) {
                        pChannel = NULL;
                    } else {
                        pChannel = &self->mChannels[channelIndex];
                    }
                }

                if (pChannel != NULL) {
                    WaveChannelParamRetail& rParam =
                        waveInfo.channelParam[ch];
                    rParam.dataAddr = pChannel->bufferAddress;
                    rParam.adpcm = pChannel->adpcm;
                    // Only the first byte of the buffer holds the ADPCM
                    // pred/scale; store it in the pred_scale slot.
                    rParam.adpcm.param.pred_scale =
                        *reinterpret_cast<u8*>(pChannel->bufferAddress);
                }
            }

            u32 level = OSDisableInterrupts();
            if (rTrack.voice != NULL) {
                Setup__Q44nw4r3snd6detail5VoiceFRCQ44nw4r3snd6detail8WaveInfoUl(
                    rTrack.voice, waveInfo, blockOffset);
                rTrack.voice->SetVoiceType(AxVoice::VOICE_TYPE_STREAM);
                rTrack.voice->Start();
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
    {
        SoundThread::AutoLock lock;

        if (mVoice != NULL) {
            mVoice->Stop();
        }

        FreeChannels();

        if (mActiveFlag) {
            SoundThread::GetInstance().UnregisterPlayerCallback(this);
        }
    }

    TaskManager::GetInstance().CancelTask(&mStrmHeaderLoadTask);

    {
        ut::AutoInterruptLock lock;

        while (!mStrmDataLoadTaskList.IsEmpty()) {
            TaskManager::GetInstance().CancelTask(
                &mStrmDataLoadTaskList.GetBack());
        }
    }

    if (mFileStream != NULL) {
        mFileStream->Close();
        mFileStream = NULL;
    }

    mStartedFlag = false;
    mPreparedFlag = false;
    mActiveFlag = false;
}

void StrmPlayer::Pause(bool flag) {
    SoundThread::AutoLock lock;

    mPauseFlag = flag;

    if (flag) {
        mLoadWaitFlag = true;
    }

    UpdatePauseStatus();
}

void StrmPlayer::InitParam() {
    BasicPlayer::InitParam();

    mStartedFlag = false;
    mPreparedFlag = false;
    mLoadFinishFlag = false;

    mPauseFlag = false;
    mPauseStatus = false;

    mLoadWaitFlag = false;
    mNoRealtimeLoadFlag = false;

    mPlayFinishFlag = false;

    mSkipUpdateAdpcmLoop = false;
    mValidAdpcmLoop = false;

    mChannelCount = 0;
    mLoopCounter = 0;
    mVoiceOutCount = 1;

    for (int i = 0; i < CHANNEL_MAX; i++) {
        mChannels[i].bufferAddress = NULL;
        mChannels[i].bufferSize = 0;
    }
}

bool StrmPlayer::LoadHeader(ut::FileStream* pFileStream,
                            StartOffsetType offsetType, int offset) {
    // Retail keeps the mutex address in a saved register for the whole
    // function (mr r3, r31 / bl OSUnlockMutex at every exit).
    OSMutex* pMutex = &lbl_eu_80653E00;
    OSLockMutex(pMutex);

    StrmFileLoader loader(*pFileStream);
    if (!loader.LoadFileHeader(lbl_eu_8064FE00, 0x4000)) {
        OSUnlockMutex(pMutex);
        return false;
    }

    StrmPlayerRetailLayout* self =
        reinterpret_cast<StrmPlayerRetailLayout*>(this);

    if (!ReadStrmInfo__Q44nw4r3snd6detail14StrmFileLoaderCFPQ54nw4r3snd6detail14StrmFileReader8StrmInfo(
            &loader, &self->mStrmInfo)) {
        OSUnlockMutex(pMutex);
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
        if (!ReadStrmTrackInfo__Q44nw4r3snd6detail14StrmFileLoaderCFPQ54nw4r3snd6detail14StrmFileReader13StrmTrackInfoi(
                &loader, &self->mTracks[i].fileInfo, i)) {
            OSUnlockMutex(pMutex);
            return false;
        }
    }

    // ADPCM streams carry per-channel adpcm data and a start-offset block.
    if (self->mStrmInfo.format == 3) {
        for (int i = 0; i < self->mChannelCount; i++) {
            if (!ReadAdpcmInfo__Q44nw4r3snd6detail14StrmFileLoaderCFPQ44nw4r3snd6detail10AdpcmParamPQ44nw4r3snd6detail14AdpcmLoopParami(
                    &loader, &self->mChannels[i].adpcm.param,
                    &self->mChannels[i].adpcm.loopParam, i)) {
                OSUnlockMutex(pMutex);
                return false;
            }
        }

        if (offset != 0) {
            int startSample = offset;
            if (offsetType == START_OFFSET_TYPE_SAMPLE) {
                // Start at the given sample index.
            } else if (offsetType == START_OFFSET_TYPE_MILLISEC) {
                startSample = offset * self->mStrmInfo.sampleRate / 1000;
            }

            s32 block = startSample / self->mStrmInfo.blockSamples;
            u16 yn1[16];
            u16 yn2[16];

            if (!loader.ReadAdpcBlockData(yn1, yn2, block,
                                          self->mStrmInfo.numChannels)) {
                OSUnlockMutex(pMutex);
                return false;
            }

            for (int i = 0; i < self->mStrmInfo.numChannels; i++) {
                self->mChannels[i].adpcm.param.yn1 = yn1[i];
                self->mChannels[i].adpcm.param.yn2 = yn2[i];
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

bool StrmPlayer::LoadStreamData(ut::FileStream* pFileStream, int offset,
                                u32 size, u32 blockSize, int blockIndex,
                                bool needUpdateAdpcmLoop) {
    // Downcast to DvdFileStream by walking the retail runtime type-info chain
    // (lbl_eu_80665550 is the DvdFileStream type-info object; the locked
    // header's DynamicCast would reference the C++ static member instead).
    const ut::detail::RuntimeTypeInfo* pDvdTypeInfo = &lbl_eu_80665550;
    ut::DvdFileStream* pDvdStream;
    if (pFileStream != NULL &&
        TypeInfoIsDerivedFrom(pFileStream->GetRuntimeTypeInfo(),
                              pDvdTypeInfo)) {
        pDvdStream = static_cast<ut::DvdFileStream*>(pFileStream);
    } else {
        pDvdStream = NULL;
    }

    if (pDvdStream != NULL) {
        pDvdStream->SetPriority(DVD_PRIO_HIGH);
    }

    // Retail keeps the mutex address in a saved register (mr r3, r28 / bl
    // OSUnlockMutex at every exit).
    OSMutex* pMutex = &lbl_eu_80653E00;
    OSLockMutex(pMutex);
    DCInvalidateRange(lbl_eu_8064FE00, 0x4000);

    StrmPlayerRetailLayout* self =
        reinterpret_cast<StrmPlayerRetailLayout*>(this);

    u16 adpcmPredScale[16];
    s32 streamOffset = offset + self->mStrmInfo.blockHeaderOffset;

    // Loads arrive in groups of up to 2 channels per stream block; each group
    // is read at streamOffset and fanned out to the channel buffers.
    for (int block = 0; block < self->mChannelCount;) {
        s32 blockCount = 2;
        if (block + 2 > self->mChannelCount) {
            blockCount = self->mChannelCount - block;
        }

        u32 readSize = blockSize * blockCount;
        pFileStream->Seek(streamOffset, ut::FileStream::SEEK_ORIGIN_BEG);
        if (pFileStream->Read(lbl_eu_8064FE00, readSize) != readSize) {
            OSUnlockMutex(pMutex);
            return false;
        }

        for (int ch = 0; ch < blockCount; ch++, block++) {
            if (needUpdateAdpcmLoop) {
                adpcmPredScale[block + ch] =
                    lbl_eu_8064FE00[blockSize * ch];
            }

            u8* pDst = reinterpret_cast<u8*>(
                           self->mChannels[block + ch].bufferAddress) +
                       self->mDataBlockSize * blockIndex;
            u8* pSrc = lbl_eu_8064FE00 + blockSize * ch;

            std::memcpy(pDst, pSrc, blockSize);
            DCFlushRange(pDst, blockSize);
        }

        streamOffset += readSize;
    }

    if (needUpdateAdpcmLoop && self->mStrmInfo.format == 3) {
        for (int i = 0; i < self->mChannelCount && i < 16; i++) {
            self->mChannels[i].mAdpcmLoopPredScale = adpcmPredScale[i];
        }

        self->mValidAdpcmLoop = true;
    }

    if (!self->mPreparedFlag && --self->mPrepareCounter == 0) {
        self->mPreparedFlag = true;
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
        mChannels[i].bufferSize = mBufferPool->GetBlockSize();
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
            mChannels[i].bufferSize = 0;
        }
    }

    mChannelCount = 0;

    if (mVoice != NULL) {
        mVoice->Free();
        mVoice = NULL;
    }
}

void StrmPlayer::Update() {
    if (!mActiveFlag) {
        return;
    }

    if (mTaskErrorFlag && !mTaskCancelFlag) {
        Stop();
        return;
    }

    if (mStartedFlag && mVoice == NULL) {
        Stop();
        return;
    }

    if (mLoadWaitFlag && mStrmDataLoadTaskList.IsEmpty()) {
        mLoadWaitFlag = false;
        UpdatePauseStatus();
    }

    if (mLoadingDelayFlag) {
        mLoadingDelayFlag = false;
    }

    if (mVoice != NULL) {
        f32 volume = 1.0f;
        volume *= GetVolume();

        f32 pitchRatio = 1.0f;
        pitchRatio *= GetPitch();

        f32 pan = 0.0f;
        pan += GetPan();

        f32 surroundPan = 0.0f;
        surroundPan += GetSurroundPan();

        f32 lpfFreq = 1.0f;
        lpfFreq += GetLpfFreq();

        int remoteFilter = 0;
        remoteFilter += GetRemoteFilter();

        f32 mainSend = 0.0f;
        mainSend += GetMainSend();

        f32 fxSend[AUX_BUS_NUM];
        for (int i = 0; i < AUX_BUS_NUM; i++) {
            fxSend[i] = 0.0f;
            fxSend[i] += GetFxSend(static_cast<AuxBus>(i));
        }

        f32 remoteOutVolume[WPAD_MAX_CONTROLLERS];
        f32 remoteSend[WPAD_MAX_CONTROLLERS];
        f32 remoteFxSend[WPAD_MAX_CONTROLLERS];
        for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            remoteOutVolume[i] = GetRemoteOutVolume(i);

            remoteSend[i] = 0.0f;
            remoteSend[i] += GetRemoteSend(i);

            remoteFxSend[i] = 0.0f;
            remoteFxSend[i] += GetRemoteFxSend(i);
        }

        ut::AutoInterruptLock lock;

        if (mVoice != NULL) {
            mVoice->SetVolume(volume);
            mVoice->SetPitch(pitchRatio);
            mVoice->SetPan(pan);
            mVoice->SetSurroundPan(surroundPan);
            mVoice->SetLpfFreq(lpfFreq);
            mVoice->SetRemoteFilter(remoteFilter);
            mVoice->SetOutputLine(GetOutputLine());
            mVoice->SetMainOutVolume(GetMainOutVolume());
            mVoice->SetMainSend(mainSend);

            for (int i = 0; i < AUX_BUS_NUM; i++) {
                mVoice->SetFxSend(static_cast<AuxBus>(i), fxSend[i]);
            }

            for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                mVoice->SetRemoteOutVolume(i, remoteOutVolume[i]);
                mVoice->SetRemoteSend(i, remoteSend[i]);
                mVoice->SetRemoteFxSend(i, remoteFxSend[i]);
            }
        }
    }
}

void StrmPlayer::UpdateBuffer() {
    if (mStartedFlag && mVoice != NULL) {
        if (AxManager::GetInstance().IsDiskError()) {
            mLoadWaitFlag = true;
            UpdatePauseStatus();
        }

        if (!mPlayFinishFlag && !mNoRealtimeLoadFlag && !mLoadWaitFlag) {
            u32 sample = mVoice->GetCurrentPlayingSample();
            int block = sample / mStrmInfo.blockSamples;

            while (mPlayingBufferBlockIndex != block) {
                if (!mLoadWaitFlag && mStrmDataLoadTaskList.GetSize() >=
                                          mBufferBlockCountBase - 2) {

                    mLoadingDelayFlag = true;
                    mLoadWaitFlag = true;
                    UpdatePauseStatus();
                    break;
                }

                UpdatePlayingBlockIndex();
                UpdateLoadingBlockIndex();
            }
        }
    }
}

void StrmPlayer::UpdateLoopAddress(u32 startSample, u32 endSample) {
    ut::AutoInterruptLock lock;

    for (int i = 0; i < mChannelCount; i++) {
        mVoice->SetLoopStart(i, mChannels[i].bufferAddress, startSample);
        mVoice->SetLoopEnd(i, mChannels[i].bufferAddress, endSample);
    }

    mVoice->SetLoopFlag(true);
}

void StrmPlayer::UpdatePlayingBlockIndex() {
    mPlayingDataBlockIndex++;

    if (mPlayingDataBlockIndex > mLastBlockIndex && mStrmInfo.loopFlag) {
        mPlayingDataBlockIndex = mLoopStartBlockIndex;

        if (mLoopCounter < INT_MAX) {
            mLoopCounter++;
        }

        UpdateLoopAddress(0, mPlayingBufferBlockCount * mStrmInfo.blockSamples);
    }

    mPlayingBufferBlockIndex++;

    if (mPlayingBufferBlockIndex >= mPlayingBufferBlockCount) {
        mPlayingBufferBlockIndex = 0;
        mPlayingBufferBlockCount = mLoadingBufferBlockCount;

        UpdateLoopAddress(0, mPlayingBufferBlockCount * mStrmInfo.blockSamples);
    }

    if (mPlayingBufferBlockIndex == mPlayingBufferBlockCount - 1 &&
        mVoice->GetFormat() == AxVoice::FORMAT_ADPCM) {

        if (!mSkipUpdateAdpcmLoop && mValidAdpcmLoop) {
            ut::AutoInterruptLock lock;

            for (int i = 0; i < mChannelCount; i++) {
                AdpcmLoopParam loopParam;
                loopParam.loop_pred_scale = mAdpcmLoopPredScale[i];
                loopParam.loop_yn1 = 0;
                loopParam.loop_yn2 = 0;

                mVoice->SetAdpcmLoop(i, &loopParam);
            }

            mVoice->SetVoiceType(AxVoice::VOICE_TYPE_STREAM);
        }

        mValidAdpcmLoop = false;
        mSkipUpdateAdpcmLoop = false;
    }

    if (mPlayingDataBlockIndex == mLastBlockIndex - 1) {
        UpdateDataLoopAddress(mPlayingBufferBlockIndex + 1);
    }
}

void StrmPlayer::UpdateDataLoopAddress(s32 endBlock) {
    if (mStrmInfo.loopFlag) {
        s32 startBlock = endBlock + 1;
        if (startBlock >= mPlayingBufferBlockCount) {
            startBlock -= mPlayingBufferBlockCount;
        }

        ut::AutoInterruptLock lock;

        UpdateLoopAddress(startBlock * mStrmInfo.blockSamples,
                          mStrmInfo.lastBlockSamples +
                              (endBlock * mStrmInfo.blockSamples));

        if (mStrmInfo.format == WaveFile::FORMAT_ADPCM) {
            if (mVoice->GetFormat() == AxVoice::FORMAT_ADPCM) {
                mVoice->SetVoiceType(AxVoice::VOICE_TYPE_NORMAL);

                for (int i = 0; i < mChannelCount; i++) {
                    mVoice->SetAdpcmLoop(i, &mChannels[i].adpcmInfo.loopParam);
                }
            }

            if (endBlock == mPlayingBufferBlockCount - 1) {
                mSkipUpdateAdpcmLoop = true;
            }
        }

        return;
    }

    SetLoopEndToZeroBuffer(endBlock);
}

void StrmPlayer::SetLoopEndToZeroBuffer(int endBlock) {
    {
        ut::AutoInterruptLock lock;

        for (int i = 0; i < mChannelCount; i++) {
            mVoice->StopAtPoint(i, mChannels[i].bufferAddress,
                                mStrmInfo.lastBlockSamples +
                                    (endBlock * mStrmInfo.blockSamples));
        }
    }

    mPlayFinishFlag = true;
}

void StrmPlayer::UpdateLoadingBlockIndex() {
    if (mLoadFinishFlag) {
        return;
    }

    u32 blockSize =
        mLoadingDataBlockIndex < static_cast<s32>(mStrmInfo.numBlocks - 1)
            ? mStrmInfo.blockSize
            : mStrmInfo.lastBlockPaddedSize;

    u32 loadSize = mStrmInfo.blockHeaderOffset +
                   mChannelCount * ut::RoundUp(blockSize, 32);

    s32 loadOffset =
        mStrmInfo.dataOffset +
        mLoadingDataBlockIndex * (mStrmInfo.blockHeaderOffset +
                                  mStrmInfo.blockSize * mStrmInfo.numChannels);

    bool needUpdateAdpcmLoop = mLoadingBufferBlockIndex == 0 &&
                               mStrmInfo.format == WaveFile::FORMAT_ADPCM;

    StrmDataLoadTask* pTask = mStrmDataLoadTaskPool.Alloc();
    pTask->strmPlayer = this;
    pTask->fileStream = mFileStream;
    pTask->size = loadSize;
    pTask->offset = loadOffset;
    pTask->blockSize = blockSize;
    pTask->bufferBlockIndex = mLoadingBufferBlockIndex;
    pTask->needUpdateAdpcmLoop = needUpdateAdpcmLoop;

    ut::AutoInterruptLock lock;
    mStrmDataLoadTaskList.PushBack(pTask);

    TaskManager::GetInstance().AppendTask(
        pTask, mStartedFlag ? TaskManager::PRIORITY_HIGH
                            : TaskManager::PRIORITY_MIDDLE);

    mLoadingDataBlockIndex++;

    if (mLoadingDataBlockIndex > mLastBlockIndex) {
        if (mStrmInfo.loopFlag) {
            mLoadingDataBlockIndex = mLoopStartBlockIndex;
        } else {
            mLoadFinishFlag = true;
            return;
        }
    }

    mLoadingBufferBlockIndex++;

    if (mLoadingBufferBlockIndex >= mLoadingBufferBlockCount) {
        mLoadingBufferBlockIndex = 0;
        mLoadingBufferBlockCount = CalcLoadingBufferBlockCount();
    }
}

void StrmPlayer::UpdatePauseStatus() {
    ut::AutoInterruptLock lock;

    bool paused = false;

    if (mPauseFlag) {
        paused = true;
    }

    if (mLoadWaitFlag) {
        paused = true;
    }

    if (paused != mPauseStatus) {
        if (mVoice != NULL) {
            mVoice->Pause(paused);
        }

        mPauseStatus = paused;
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
    if (mStrmInfo.blockSamples == 0) {
        return false;
    }

    int startSample;
    if (mStartOffsetType == START_OFFSET_TYPE_SAMPLE) {
        startSample = mStartOffset;
    } else if (mStartOffsetType == START_OFFSET_TYPE_MILLISEC) {
        startSample =
            (mStartOffset * static_cast<s64>(mStrmInfo.sampleRate)) / 1000;
    }

    *pLoopCount = 0;

    if (startSample >= mStrmInfo.loopEnd) {
        if (mStrmInfo.loopFlag) {
            s32 loopStart = mStrmInfo.loopStart;
            s32 loopEnd = mStrmInfo.loopEnd;
            s32 loopLength = loopEnd - loopStart;

            s32 startOffset2 = startSample - loopEnd;
            *pLoopCount = startOffset2 / loopLength + 1;

            s32 startLoop = startOffset2 / loopLength;
            startSample = loopStart + (startOffset2 - (startLoop * loopLength));
        } else {
            return false;
        }
    }

    *pBlockIndex = startSample / static_cast<s32>(mStrmInfo.blockSamples);

    if (mStrmInfo.format == WaveFile::FORMAT_ADPCM) {
        s32 startBlock = startSample / mStrmInfo.blockSamples;
        *pBlockOffset = startSample - (startBlock * mStrmInfo.blockSamples);
    }

    return true;
}

void StrmPlayer::VoiceCallbackFunc(Voice* pDropVoice,
                                   Voice::VoiceCallbackStatus status,
                                   void* pCallbackArg) {
    StrmPlayer* pStrmPlayer = static_cast<StrmPlayer*>(pCallbackArg);
    ut::AutoInterruptLock lock;

    switch (status) {
    case Voice::CALLBACK_STATUS_FINISH_WAVE:
    case Voice::CALLBACK_STATUS_CANCEL: {
        pDropVoice->Free();
        pStrmPlayer->mVoice = NULL;
        break;
    }

    case Voice::CALLBACK_STATUS_DROP_VOICE:
    case Voice::CALLBACK_STATUS_DROP_DSP: {
        pStrmPlayer->mVoice = NULL;
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
        mAdpcmLoopPredScale[i] = pPredScale[i];
    }

    mValidAdpcmLoop = true;
}

StrmPlayer::StrmHeaderLoadTask::StrmHeaderLoadTask()
    : strmPlayer(NULL), fileStream(NULL), startOffset(0) {}

void StrmPlayer::StrmHeaderLoadTask::Execute() {
    if (!strmPlayer->LoadHeader(fileStream, startOffsetType, startOffset)) {
        // mTaskErrorFlag is at retail offset +0x124; see layout-mirror note.
        reinterpret_cast<StrmPlayerRetailLayout*>(strmPlayer)->mTaskErrorFlag =
            true;
    }
}

void StrmPlayer::StrmHeaderLoadTask::Cancel() {}

void StrmPlayer::StrmHeaderLoadTask::OnCancel() {
    // mTaskCancelFlag is at retail offset +0x125; see layout-mirror note.
    reinterpret_cast<StrmPlayerRetailLayout*>(strmPlayer)->mTaskCancelFlag =
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
        strmPlayer->SetTaskErrorFlag();
    }

    ut::AutoInterruptLock lock;
    strmPlayer->mStrmDataLoadTaskList.Erase(this);
    strmPlayer->mStrmDataLoadTaskPool.Free(this);
}

void StrmPlayer::StrmDataLoadTask::Cancel() {
    ut::AutoInterruptLock lock;

    // List at +0x194 / pool at +0x1A0 are retail offsets; see layout-mirror
    // note. Two separate strmPlayer reads so the member is reloaded after the
    // Erase call, matching retail.
    reinterpret_cast<StrmPlayerRetailLayout*>(strmPlayer)
        ->mStrmDataLoadTaskList.Erase(this);
    reinterpret_cast<StrmPlayerRetailLayout*>(strmPlayer)
        ->mStrmDataLoadTaskPool.Free(this);
}

void StrmPlayer::StrmDataLoadTask::OnCancel() {
    // mTaskCancelFlag is at retail offset +0x125; see layout-mirror note.
    reinterpret_cast<StrmPlayerRetailLayout*>(strmPlayer)->mTaskCancelFlag =
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
// free functions). Declaration only — the definition is the member below.
extern "C" void VoiceCallbackFunc__Q44nw4r3snd6detail10StrmPlayerFPQ44nw4r3snd6detail5VoiceQ54nw4r3snd6detail5Voice19VoiceCallbackStatusPv(
    nw4r::snd::detail::Voice*, nw4r::snd::detail::Voice::VoiceCallbackStatus,
    void*);

// Retail AllocVoices(int) is a StrmPlayer member, but the stale header has no
// declaration for it; define the retail mangled symbol as a free function
// (same ABI: r3 = this, r4 = voices). Kept OUT of the anonymous namespace so
// the symbol name is emitted verbatim.
bool AllocVoices__Q44nw4r3snd6detail10StrmPlayerFi(StrmPlayer* pStrmPlayer,
                                                   int voices) {
    nw4r::snd::detail::StrmPlayerRetailLayout* self =
        reinterpret_cast<nw4r::snd::detail::StrmPlayerRetailLayout*>(
            pStrmPlayer);

    u32 level = OSDisableInterrupts();

    nw4r::snd::detail::StrmTrackRetailLayout* pBaseTrack = self->mTracks;
    nw4r::snd::detail::StrmTrackRetailLayout* pTrack = pBaseTrack;

    for (int i = 0; i < self->mTrackCount; pTrack++, i++) {
        if (pTrack->activeFlag) {
            nw4r::snd::detail::Voice* pVoice =
                nw4r::snd::detail::VoiceManager::GetInstance().AllocVoice(
                    pTrack->fileInfo.channelCount, voices, 0xFF,
                    &VoiceCallbackFunc__Q44nw4r3snd6detail10StrmPlayerFPQ44nw4r3snd6detail5VoiceQ54nw4r3snd6detail5Voice19VoiceCallbackStatusPv,
                    pTrack);

            if (pVoice == NULL) {
                // Allocation failed: free the voices already granted to
                // earlier tracks and bail out.
                nw4r::snd::detail::StrmTrackRetailLayout* pFreeTrack =
                    pBaseTrack;
                for (int j = 0; j < i; pFreeTrack++, j++) {
                    if (pFreeTrack->voice != NULL) {
                        pFreeTrack->voice->Free();
                        pFreeTrack->voice = NULL;
                    }
                }

                OSRestoreInterrupts(level);
                return false;
            }

            pTrack->voice = pVoice;
            // Disable per-voice pitch modulation for streamed voices (see
            // snd_Voice.cpp field_0xA1 pitch-modulation gate).
            reinterpret_cast<nw4r::snd::detail::VoicePitchGateLayout*>(pVoice)
                ->field_0xA1 = 1;
        }
    }

    OSRestoreInterrupts(level);
    return true;
}
// Retail SetupPlayer() (no arguments) is a StrmPlayer member, but the stale
// header declares a different SetupPlayer(const StrmHeader*); emit the retail
// symbol as a free function (same ABI: r3 = this). Reads the already-populated
// mStrmInfo and allocates the channel buffers from the pool.
bool SetupPlayer__Q44nw4r3snd6detail10StrmPlayerFv(StrmPlayer* pStrmPlayer) {
    nw4r::snd::detail::StrmPlayerRetailLayout* self =
        reinterpret_cast<nw4r::snd::detail::StrmPlayerRetailLayout*>(
            pStrmPlayer);

    u32 poolBlockSize = self->mBufferPool->GetBlockSize();

    s32 blockIndex = 0;
    u32 blockOffset = 0;
    s32 loopCount = 0;
    if (!CalcStartOffset__Q44nw4r3snd6detail10StrmPlayerFPlPUlPl(
            pStrmPlayer, &blockIndex, &blockOffset, &loopCount)) {
        return false;
    }

    self->mLoopStartBlockIndex =
        self->mStrmInfo.loopStart / self->mStrmInfo.blockSamples;
    self->mLastBlockIndex = self->mStrmInfo.numBlocks - 1;
    self->mDataBlockSize = self->mStrmInfo.blockSize;

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
        s32 restBlocks =
            (self->mLastBlockIndex - self->mLoadingDataBlockIndex) + 1;

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
        bool success = true;
        for (int i = 0; i < self->mChannelCount; i++) {
            void* pBuffer = self->mBufferPool->Alloc();
            if (pBuffer == NULL) {
                for (int j = 0; j < i; j++) {
                    self->mBufferPool->Free(
                        self->mChannels[j].bufferAddress);
                    self->mChannels[j].bufferAddress = NULL;
                }

                success = false;
                break;
            }

            self->mChannels[i].bufferAddress = pBuffer;
        }

        if (!success) {
            OSRestoreInterrupts(level);
            return false;
        }

        self->mBufferAllocFlag = true;
    }
    OSRestoreInterrupts(level);
    return true;
}

// Retail UpdateVoiceParams(StrmPlayer::StrmTrack*) is a StrmPlayer member, but
// the stale header has no declaration for it; emit the retail mangled symbol
// as a free function (r3 = this, r4 = track).
void UpdateVoiceParams__Q44nw4r3snd6detail10StrmPlayerFPQ54nw4r3snd6detail10StrmPlayer9StrmTrack(
    StrmPlayer* pStrmPlayer,
    nw4r::snd::detail::StrmTrackRetailLayout* pTrack) {
    if (!pTrack->activeFlag) {
        return;
    }

    nw4r::snd::detail::StrmPlayerRetailLayout* self =
        reinterpret_cast<nw4r::snd::detail::StrmPlayerRetailLayout*>(
            pStrmPlayer);

    // Per-track mix: player volume * file volume byte / 100 * track volume,
    // with pan composed from the file pan byte (range -64..63) plus the
    // track's own pan offset. Byte/int -> float conversions run through the
    // shared 2^52 magic doubles, referenced by name so the .sdata2 relocs
    // match retail (see ut_PackedFont.cpp F64Conv pattern).
    union {
        f64 d;
        u32 w[2];
    } conv;

    conv.w[1] = pTrack->fileInfo.volume;
    conv.w[0] = 0x43300000;
    f32 volume = lbl_eu_8066A078 * self->mVolume *
                 ((f32)(conv.d - lbl_eu_8066A070) / lbl_eu_8066A07C) *
                 pTrack->volume;
    f32 pitch = lbl_eu_8066A078 * self->mPitch;
    f32 pan = lbl_eu_8066A060 + self->mPan;
    if (pTrack->fileInfo.pan > 1) {
        conv.w[0] = 0x43300000;
        conv.w[1] = (u32)(pTrack->fileInfo.pan - 64) ^ 0x80000000;
        pan += (f32)(conv.d - lbl_eu_8066A068) / lbl_eu_8066A080;
    } else {
        conv.w[0] = 0x43300000;
        conv.w[1] = (u32)(pTrack->fileInfo.pan - 63) ^ 0x80000000;
        pan += (f32)(conv.d - lbl_eu_8066A068) / lbl_eu_8066A080;
    }
    pan += pTrack->field_0x34;

    f32 surroundPan = lbl_eu_8066A060 + self->mSurroundPan;
    f32 lpfFreq = lbl_eu_8066A078 + self->mLpfFreq;
    f32 biquadValue = self->mBiquadFilterValue;
    u8 biquadType = self->mBiquadFilterType;
    u8 remoteFilter = self->mRemoteFilter;
    f32 mainSend = lbl_eu_8066A060 + self->mMainSend;

    f32 fxSend[nw4r::snd::AUX_BUS_NUM];
    for (int i = 0; i < nw4r::snd::AUX_BUS_NUM; i++) {
        fxSend[i] = lbl_eu_8066A060 +
                    pStrmPlayer->GetFxSend(static_cast<nw4r::snd::AuxBus>(i));
    }

    f32 remoteOutVolume[WPAD_MAX_CONTROLLERS];
    f32 remoteSend[WPAD_MAX_CONTROLLERS];
    f32 remoteFxSend[WPAD_MAX_CONTROLLERS];
    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        remoteOutVolume[i] = pStrmPlayer->GetRemoteOutVolume(i);
        remoteSend[i] =
            lbl_eu_8066A060 + pStrmPlayer->GetRemoteSend(i);
        remoteFxSend[i] =
            lbl_eu_8066A060 + pStrmPlayer->GetRemoteFxSend(i);
    }

    u32 level = OSDisableInterrupts();
    nw4r::snd::detail::Voice* pVoice = pTrack->voice;
    if (pVoice != NULL) {
        pVoice->SetVolume(volume);
        pVoice->SetPitch(pitch);
        pVoice->SetPan(pan);
        pVoice->SetSurroundPan(surroundPan);
        pVoice->SetLpfFreq(lpfFreq);
        SetBiquadFilter__Q44nw4r3snd6detail5VoiceFif(pVoice, biquadType,
                                                    biquadValue);
        pVoice->SetRemoteFilter(remoteFilter);
        pVoice->SetOutputLine(self->mOutputLine);
        pVoice->SetMainOutVolume(self->mMainOutVolume);
        pVoice->SetMainSend(mainSend);

        for (int i = 0; i < nw4r::snd::AUX_BUS_NUM; i++) {
            pVoice->SetFxSend(static_cast<nw4r::snd::AuxBus>(i), fxSend[i]);
        }

        for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            pVoice->SetRemoteOutVolume(i, remoteOutVolume[i]);
            pVoice->SetRemoteSend(i, remoteSend[i]);
            pVoice->SetRemoteFxSend(i, remoteFxSend[i]);
        }

        const nw4r::snd::detail::VoiceOutParamRetail* pOutParam =
            self->mVoiceOutParam;
        for (int i = 0; i < self->mVoiceOutCount; i++) {
            SetVoiceOutParam__Q44nw4r3snd6detail5VoiceFiRCQ34nw4r3snd13VoiceOutParam(
                pVoice, i, *pOutParam);
            pOutParam++;
        }
    }
    OSRestoreInterrupts(level);
}
extern "C" void OnUpdateFrameSoundThread__Q44nw4r3snd6detail10StrmPlayerFv() {}
extern "C" void OnUpdateVoiceSoundThread__Q44nw4r3snd6detail10StrmPlayerFv() {}

extern "C" u8 IsPause__Q44nw4r3snd6detail10StrmPlayerCFv(StrmPlayer* self) { return self->IsPause(); }
extern "C" u8 IsStarted__Q44nw4r3snd6detail10StrmPlayerCFv(StrmPlayer* self) { return self->IsStarted(); }
extern "C" u8 IsActive__Q44nw4r3snd6detail10StrmPlayerCFv(StrmPlayer* self) { return self->IsActive(); }
