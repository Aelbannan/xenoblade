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

// StrmPlayer::StrmDataLoadTask vtable (.data; plain linker label in retail,
// referenced by UpdateLoadingBlockIndex's inlined constructor).
extern u8 lbl_eu_8056ACC0[]; // StrmDataLoadTask vtable (.data)

u8 lbl_eu_80665518; // StrmPlayer load-buffer static-init flag (.sdata)

namespace nw4r {
namespace snd {
namespace detail {

u8 StrmPlayer::sLoadBuffer[LOAD_BUFFER_SIZE] ALIGN(32);

// NOTE: the load-buffer mutex (lbl_eu_80653E00) and static-init flag
// (lbl_eu_80665518) are plain linker labels in retail, not class statics;
// see the extern declarations above.
// bool StrmPlayer::sStaticInitFlag = false; - replaced by lbl_eu_80665518

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
// StrmPlayer::StrmHeaderLoadTask embedded at player +0x174 (Task base is
// 0x10 bytes; see snd_Task.h).
struct StrmHeaderLoadTaskRetail {
    u8 taskBase[0x10];              // Task base (vtable + links)
    StrmPlayer* strmPlayer;         // at task + 0x10
    ut::FileStream* fileStream;     // at task + 0x14
    s32 startOffsetType;            // at task + 0x18
    s32 startOffset;                // at task + 0x1C
};                                  // sizeof 0x20

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
    bool mLoadingDelayFlag;                 // at 0x126
    bool mPauseFlag;                       // at 0x127
    bool mPauseStatus;                     // at 0x128
    bool mLoadWaitFlag;                    // at 0x129
    bool mNoRealtimeLoadFlag;              // at 0x12A
    bool mSkipUpdateAdpcmLoop;             // at 0x12B
    bool mValidAdpcmLoop;                  // at 0x12C
    bool mPlayFinishFlag;                   // at 0x12D
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
    s32 field_0x168;                       // at 0x168 (pending load-task count)
    s32 mStartOffsetType;                   // at 0x16C
    s32 mStartOffset;                       // at 0x170
    StrmHeaderLoadTaskRetail mStrmHeaderLoadTask; // at 0x174 (0x20)
    // StrmPlayer::StrmDataLoadTaskList (private typedef) expanded here; fully
    // qualified because a namespace-scope friend decl shadows the nested type.
    nw4r::ut::LinkList<StrmPlayer::StrmDataLoadTask,
                       offsetof(StrmPlayer::StrmDataLoadTask, node)>
        mStrmDataLoadTaskList;              // at 0x194 (0xC)
    InstancePool<StrmPlayer::StrmDataLoadTask> mStrmDataLoadTaskPool; // at 0x1A0 (0x4)
    u8 mStrmDataLoadTaskArea[0x680];         // 0x1A4..0x824 (task pool area)
    StrmBufferPool* mBufferPool;            // at 0x824
    union {
        Voice* mVoice;                      // at 0x828 (voice while playing)
        ut::FileStream* mFileStream;        // at 0x828 (stream while preparing)
    };
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

// Defined later in this TU (retail-mangled member emitted as a free function).
extern "C" void UpdateVoiceParams__Q44nw4r3snd6detail10StrmPlayerFPQ54nw4r3snd6detail10StrmPlayer9StrmTrack(
    StrmPlayer* pStrmPlayer, StrmTrackRetailLayout* pTrack);

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
    : mSetupFlag(false), mActiveFlag(false), mFileStream(NULL) {
    // One-time init of the shared load-buffer mutex (plain linker label in
    // retail, guarded by an .sdata flag byte).
    if (lbl_eu_80665518 == 0) {
        OSInitMutex(&lbl_eu_80653E00);
        lbl_eu_80665518 = 1;
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

// Retail 3-argument overload (no voices argument; mVoiceOutCount keeps its
// previous value).
bool StrmPlayer::Prepare(ut::FileStream* pFileStream,
                         StartOffsetType offsetType, int offset) {
    SoundThread::AutoLock lock;

    StrmPlayerRetailLayout* self =
        reinterpret_cast<StrmPlayerRetailLayout*>(this);

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

    StrmHeaderLoadTaskRetail* pTask = &self->mStrmHeaderLoadTask;
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
        // Declaration order drives MWCC callee-saved coloring (descending).
        u32 loopFlag;
        u32 loopStartSample;
        u32 loopEndSamples;
        StrmTrackRetailLayout* pTrack;
        int i;
        int ch;
        StrmChannelRetail* pChannel;
        u32 level;

        if (!AllocVoices__Q44nw4r3snd6detail10StrmPlayerFi(
                this, self->mVoiceOutCount)) {
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
        // Signed cast: retail switches on an int-typed format (cmpwi tree).
        switch ((int)self->mStrmInfo.format) {
        case 1:
            loopEndSamples = bytes >> 1;
            break;
        case 2:
            loopEndSamples = bytes;
            break;
        case 3:
            loopEndSamples = (bytes >> 3) * 14;
            {
                u32 rem = bytes & 7;
                if (rem != 0) {
                    loopEndSamples += (rem - 1) * 2;
                }
            }
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

            WaveInfoRetailLayout waveInfo;
            waveInfo.sampleFormat = self->mStrmInfo.format;
            waveInfo.loopFlag = loopFlag;
            waveInfo.numChannels = pTrack->fileInfo.channelCount;
            waveInfo.sampleRate = self->mStrmInfo.sampleRate;
            waveInfo.loopStart = loopStartSample;
            waveInfo.loopEnd = loopEndSamples;

            for (ch = 0; ch < pTrack->fileInfo.channelCount; ch++) {
                if (ch >= 2) {
                    pChannel = NULL;
                } else {
                    u8 channelIndex = pTrack->fileInfo.channelIndex[ch];
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

            level = OSDisableInterrupts();
            if (pTrack->voice != NULL) {
                Setup__Q44nw4r3snd6detail5VoiceFRCQ44nw4r3snd6detail8WaveInfoUl(
                    pTrack->voice, waveInfo, blockOffset);
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
    StrmPlayerRetailLayout* self =
        reinterpret_cast<StrmPlayerRetailLayout*>(this);

    // Retail re-evaluates SoundThread::GetInstance() + the mutex offset at
    // every lock/unlock site (the address is not kept in a saved register).
    OSLockMutex(
        &reinterpret_cast<SoundThreadRetailLayout*>(&SoundThread::GetInstance())
             ->mMutex);

    // Stop the voice on every player-track slot (fixed bound of 8, not
    // mTrackCount).
    for (int i = 0; i < 8; i++) {
        if (self->mTracks[i].activeFlag && self->mTracks[i].voice != NULL) {
            self->mTracks[i].voice->Stop();
        }
    }

    if (self->mActiveFlag) {
        SoundThread::GetInstance().UnregisterPlayerCallback(this);
    }

    OSUnlockMutex(
        &reinterpret_cast<SoundThreadRetailLayout*>(&SoundThread::GetInstance())
             ->mMutex);

    TaskManager::GetInstance().CancelTask(
        reinterpret_cast<Task*>(&self->mStrmHeaderLoadTask));

    // Cancel every queued stream-data load task (interrupt-protected walk
    // from the back of the pending list).
    u32 level = OSDisableInterrupts();

    while (!self->mStrmDataLoadTaskList.IsEmpty()) {
        Task* pTask = &self->mStrmDataLoadTaskList.GetBack();
        TaskManager::GetInstance().CancelTask(pTask);
    }

    OSRestoreInterrupts(level);

    // Release every allocated channel buffer.
    for (int i = 0; i < self->mChannelCount; i++) {
        if (self->mChannels[i].bufferAddress != NULL) {
            self->mBufferPool->Free(self->mChannels[i].bufferAddress);
            self->mChannels[i].bufferAddress = NULL;
        }
    }

    // Free every player-track voice.
    level = OSDisableInterrupts();

    StrmTrackRetailLayout* pTrack = self->mTracks;
    for (int i = 0; i < self->mTrackCount; i++) {
        if (pTrack->activeFlag && pTrack->voice != NULL) {
            pTrack->voice->Free();
            pTrack->voice = NULL;
        }
        pTrack++;
    }

    OSRestoreInterrupts(level);

    OSLockMutex(
        &reinterpret_cast<SoundThreadRetailLayout*>(&SoundThread::GetInstance())
             ->mMutex);

    if (self->mFileStream != NULL) {
        self->mFileStream->Close();
        self->mFileStream = NULL;
    }

    OSUnlockMutex(
        &reinterpret_cast<SoundThreadRetailLayout*>(&SoundThread::GetInstance())
             ->mMutex);

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

    StrmPlayerRetailLayout* self =
        reinterpret_cast<StrmPlayerRetailLayout*>(this);

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
    StrmPlayerRetailLayout* self =
        reinterpret_cast<StrmPlayerRetailLayout*>(this);

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
    self->field_0x168 = 0;

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
    OSMutex* pMutex = &lbl_eu_80653E00;
    s32 startOffset = offset;
    StrmPlayerRetailLayout* self =
        reinterpret_cast<StrmPlayerRetailLayout*>(this);
    OSLockMutex(pMutex);

    StrmFileLoader loader(*pFileStream);
    if (!loader.LoadFileHeader(lbl_eu_8064FE00, 0x4000)) {
        OSUnlockMutex(pMutex);
        return false;
    }

    if (!ReadStrmInfo__Q44nw4r3snd6detail14StrmFileLoaderCFPQ54nw4r3snd6detail14StrmFileReader8StrmInfo(
            &loader, &self->mStrmInfo)) {
        OSUnlockMutex(&lbl_eu_80653E00);
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
                reinterpret_cast<StrmTrackFileInfoRetail*>(
                    reinterpret_cast<u8*>(self) +
                    i * sizeof(StrmTrackRetailLayout) + 0xB80),
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
            // SAMPLE keeps the offset as-is; MILLISEC converts through the
            // sample rate (MWCC strength-reduces the /1000 to a mulhw).
            // The offset parameter itself is reassigned, as in retail.
            if (offsetType == START_OFFSET_TYPE_MILLISEC) {
                startOffset = startOffset * self->mStrmInfo.sampleRate / 1000;
            }

            s32 block = startOffset / self->mStrmInfo.blockSamples;
            // Declared yn2-first: retail places the yn1 array at the higher
            // stack slot (sp+0x38 above sp+0x18).
            u16 yn2[16];
            u16 yn1[16];

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
    OSMutex* pMutex = &lbl_eu_80653E00;
    OSLockMutex(pMutex);
    DCInvalidateRange(lbl_eu_8064FE00, 0x4000);

    StrmPlayerRetailLayout* self =
        reinterpret_cast<StrmPlayerRetailLayout*>(this);

    s32 streamOffset = offset + self->mStrmInfo.blockHeaderOffset;
    // Separate working pointer into the shared load buffer (retail keeps the
    // extern-array reference for Read and this pointer for the fan-out).
    u8* pLoadBuf = lbl_eu_8064FE00;
    u16 adpcmPredScale[16];
    u16* pPredScale = adpcmPredScale;

    DCInvalidateRange(lbl_eu_8064FE00, 0x4000);

    // Loads arrive in groups of up to 2 channels per stream block; each group
    // is read at streamOffset and fanned out to the channel buffers.
    for (int block = 0; block < self->mChannelCount;) {
        s32 blockCount = 2;
        if (block + blockCount > self->mChannelCount) {
            blockCount = self->mChannelCount - block;
        }

        u32 readSize = blockSize * blockCount;
        pFileStream->Seek(streamOffset, ut::FileStream::SEEK_ORIGIN_BEG);
        if (pFileStream->Read(lbl_eu_8064FE00, readSize) != readSize) {
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
    StrmPlayerRetailLayout* self =
        reinterpret_cast<StrmPlayerRetailLayout*>(this);

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
        StrmTrackRetailLayout* pTrack = self->mTracks;
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
    StrmPlayerRetailLayout* self =
        reinterpret_cast<StrmPlayerRetailLayout*>(this);

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
                self->field_0x168 >=
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

    StrmPlayerRetailLayout* self =
        reinterpret_cast<StrmPlayerRetailLayout*>(this);

    // Field flags go through a walking track pointer; the channel-index
    // lookup walks a raw byte pointer from the player base -- each track's
    // fileInfo.channelIndex lives at player + 0xB88 + 0x38*i (track array at
    // 0xB78 + fileInfo offset 0x10), matching retail's second induction
    // register and folded load displacement.
    // Declaration order drives MWCC's callee-saved coloring.
    StrmChannelRetail* pChannel;
    StrmTrackRetailLayout* pTrackField;
    u8* pIdxWalk;
    int i;
    Voice* pVoice;
    int j;

    pTrackField = self->mTracks;
    pIdxWalk = reinterpret_cast<u8*>(self);

    for (i = 0; i < self->mTrackCount;
         pTrackField++, pIdxWalk += sizeof(StrmTrackRetailLayout), i++) {
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
    StrmPlayerRetailLayout* self =
        reinterpret_cast<StrmPlayerRetailLayout*>(this);

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
            for (int i = 0; i < self->mTrackCount; i++) {
            StrmTrackRetailLayout& rTrack = self->mTracks[i];
            if (!rTrack.activeFlag || rTrack.voice == NULL) {
                continue;
            }

            Voice* pVoice = rTrack.voice;
            // mFormat holds nw4r::snd::SampleFormat values (retail compares
            // the raw field against 3 = SAMPLE_FORMAT_DSP_ADPCM; see
            // AxVoice.cpp IsAdpcm note).
            if (pVoice->GetFormat() !=
                static_cast<AxVoice::Format>(SAMPLE_FORMAT_DSP_ADPCM)) {
                continue;
            }

            u32 level = OSDisableInterrupts();

            for (int j = 0; j < rTrack.fileInfo.channelCount; j++) {
                // Channels beyond the first two or with an out-of-range
                // channel index resolve to NULL; retail still loads the
                // pred-scale through the (null) pointer.
                StrmChannelRetail* pChannel;
                if (j >= 2) {
                    pChannel = NULL;
                } else {
                    // Signed local: retail compares channelIndex >= 16 with
                    // a signed cmpi (see AxVoice.cpp IsAdpcm note).
                    s32 channelIndex = rTrack.fileInfo.channelIndex[j];
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
    StrmPlayerRetailLayout* self =
        reinterpret_cast<StrmPlayerRetailLayout*>(this);

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
            StrmTrackRetailLayout* pTrack = self->mTracks;
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
                        StrmChannelRetail* pChannel;
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
    StrmPlayerRetailLayout* self =
        reinterpret_cast<StrmPlayerRetailLayout*>(this);

    u32 level = OSDisableInterrupts();

    StrmTrackRetailLayout* pTrack = self->mTracks;
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
            StrmChannelRetail* pChannel;
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

// Layout-compatible shell for StrmDataLoadTask, used by
// UpdateLoadingBlockIndex to hand-construct tasks: retail's inlined default
// constructor references the linker-label vtable (lbl_eu_8056ACC0) rather
// than the C++ vtable symbol, so the constructor is expressed explicitly.
struct StrmDataLoadTaskRetail {
    void* vtable;               // at 0x00
    u32 field_0x04;             // at 0x04 (task internals zeroed by ctor)
    u32 field_0x08;             // at 0x08
    u32 field_0x0C;             // at 0x0C
    StrmPlayer* strmPlayer;     // at 0x10
    ut::FileStream* fileStream; // at 0x14
    u32 size;                   // at 0x18
    s32 offset;                 // at 0x1C
    s32 blockSize;              // at 0x20
    s32 bufferBlockIndex;       // at 0x24
    bool needUpdateAdpcmLoop;   // at 0x28
    ut::LinkListNode node;      // at 0x2C
};

void StrmPlayer::UpdateLoadingBlockIndex() {
    StrmPlayerRetailLayout* self =
        reinterpret_cast<StrmPlayerRetailLayout*>(this);

    self->field_0x168++;

    if (self->mLoadFinishFlag) {
        return;
    }

    // Last data block loads the padded final block size.
    s32 blockSize;
    if (self->mLoadingDataBlockIndex <
        static_cast<s32>(self->mStrmInfo.numBlocks) - 1) {
        blockSize = self->mStrmInfo.blockSize;
    } else {
        blockSize = self->mStrmInfo.lastBlockPaddedSize;
    }

    // ADPCM streams refresh their loop context once per buffer rotation.
    // Declared before the address arithmetic so the flag register is colored
    // before the offset/size results (retail order).
    bool needUpdateAdpcmLoop = false;

    // Per-block stream stride uses the RAW block size (not the selected one).
    u32 stride = self->mStrmInfo.blockHeaderOffset +
                 self->mStrmInfo.blockSize * self->mStrmInfo.numChannels;

    u32 loadSize =
        self->mStrmInfo.blockHeaderOffset + blockSize * self->mChannelCount;

    s32 loadOffset =
        self->mStrmInfo.dataOffset + self->mLoadingDataBlockIndex * stride;

    if (self->mLoadingBufferBlockIndex == 0 &&
        static_cast<s32>(self->mStrmInfo.format) ==
            3 /* SAMPLE_FORMAT_DSP_ADPCM */) {
        needUpdateAdpcmLoop = true;
    }

    StrmDataLoadTask* pNew = reinterpret_cast<StrmDataLoadTask*>(
        reinterpret_cast<MemoryPool<StrmDataLoadTask>&>(
            self->mStrmDataLoadTaskPool)
            .Alloc());

    // Retail shape: the raw allocation stays in the caller-saved register
    // while both branches merge into a second pointer variable (the failed
    // allocation normalizes through the else-branch, leaving a dead re-test
    // after the copy).
    StrmDataLoadTaskRetail* pTask;
    if (pNew != NULL) {
        pTask = reinterpret_cast<StrmDataLoadTaskRetail*>(pNew);

        // Hand-rolled default construction: retail's inlined constructor
        // references the linker-label vtable instead of the C++ vtable.
        // Zero stores precede the vtable store, matching retail order.
        if (pTask != NULL) {
            pTask->field_0x04 = 0;
            pTask->field_0x08 = 0;
            pTask->field_0x0C = 0;
            pTask->vtable = &lbl_eu_8056ACC0;
            pTask->strmPlayer = NULL;
            pTask->fileStream = NULL;
            pTask->size = 0;
            pTask->offset = 0;
            pTask->blockSize = 0;
            pTask->bufferBlockIndex = -1;
            pTask->needUpdateAdpcmLoop = false;
            pTask->node.Init();
        }
    } else {
        pTask = NULL;
    }

    // Retail performs the field setup unconditionally.
    pTask->strmPlayer = reinterpret_cast<StrmPlayer*>(self);
    pTask->fileStream = self->mFileStream;
    pTask->size = loadSize;
    pTask->offset = loadOffset;
    pTask->blockSize = blockSize;
    pTask->bufferBlockIndex = self->mLoadingBufferBlockIndex;
    pTask->needUpdateAdpcmLoop = needUpdateAdpcmLoop;

    u32 level = OSDisableInterrupts();

    self->mStrmDataLoadTaskList.PushBack(
        reinterpret_cast<StrmDataLoadTask*>(pTask));
    // Priority computed between the insert and the TaskManager lookup.
    s32 priority = 1; // TaskManager::PRIORITY_MIDDLE
    if (self->mStartedFlag) {
        priority = 2; // TaskManager::PRIORITY_HIGH
    }

    TaskManager::GetInstance().AppendTask(
        reinterpret_cast<Task*>(pTask),
        static_cast<TaskManager::TaskPriority>(priority));

    self->mLoadingDataBlockIndex++;

    if (self->mLoadingDataBlockIndex > self->mLastBlockIndex) {
        if (!self->mStrmInfo.loopFlag) {
            self->mLoadFinishFlag = true;
            OSRestoreInterrupts(level);
            return;
        }

        self->mLoadingDataBlockIndex = self->mLoopStartBlockIndex;
    }

    self->mLoadingBufferBlockIndex++;

    if (self->mLoadingBufferBlockIndex >= self->mLoadingBufferBlockCount) {
        // Inlined CalcLoadingBufferBlockCount(): the next buffer holds
        // base+1 blocks iff (base+1 - remainingBlocks) divides evenly by
        // the loop length.
        int restBlocks =
            (self->mLastBlockIndex - self->mLoadingDataBlockIndex) + 1;
        int loopBlocks =
            (self->mLastBlockIndex - self->mLoopStartBlockIndex) + 1;
        int nextCount = self->mBufferBlockCountBase + 1;

        self->mLoadingBufferBlockIndex = 0;

        if ((nextCount - restBlocks) % loopBlocks == 0) {
            self->mLoadingBufferBlockCount = nextCount;
        } else {
            self->mLoadingBufferBlockCount = self->mBufferBlockCountBase;
        }
    }

    OSRestoreInterrupts(level);
}

void StrmPlayer::UpdatePauseStatus() {
    ut::AutoInterruptLock lock;

    bool pauseStatus = false;

    if (reinterpret_cast<StrmPlayerRetailLayout*>(this)->mPauseFlag) {
        pauseStatus = true;
    }

    if (reinterpret_cast<StrmPlayerRetailLayout*>(this)->mLoadWaitFlag) {
        pauseStatus = true;
    }

    if (pauseStatus !=
        reinterpret_cast<StrmPlayerRetailLayout*>(this)->mPauseStatus) {
        for (int i = 0;
             i < reinterpret_cast<StrmPlayerRetailLayout*>(this)->mTrackCount;
             i++) {
            if (reinterpret_cast<StrmPlayerRetailLayout*>(this)
                ->mTracks[i].activeFlag &&
                reinterpret_cast<StrmPlayerRetailLayout*>(this)
                    ->mTracks[i].voice != NULL) {
                reinterpret_cast<StrmPlayerRetailLayout*>(this)
                    ->mTracks[i].voice->Pause(pauseStatus);
            }
        }

        reinterpret_cast<StrmPlayerRetailLayout*>(this)->mPauseStatus =
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
    StrmPlayerRetailLayout* self =
        reinterpret_cast<StrmPlayerRetailLayout*>(this);

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
    StrmTrackRetailLayout* pTrack = static_cast<StrmTrackRetailLayout*>(pCallbackArg);
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
        // mTaskErrorFlag is at retail offset +0x124; see layout-mirror note.
        reinterpret_cast<StrmPlayerRetailLayout*>(strmPlayer)->mTaskErrorFlag =
            true;
    }

    ut::AutoInterruptLock lock;

    // List at +0x194 / pool at +0x1A0 are retail offsets; see layout-mirror
    // note. The pool Free inlines the deleting-dtor call plus FreeImpl, and
    // each statement re-reads strmPlayer so retail reloads the player pointer
    // from the task after the Erase call.
    reinterpret_cast<StrmPlayerRetailLayout*>(strmPlayer)
        ->mStrmDataLoadTaskList.Erase(this);
    reinterpret_cast<StrmPlayerRetailLayout*>(strmPlayer)
        ->mStrmDataLoadTaskPool.Free(this);

    // Decrement the pending load-task count at player +0x168 while the
    // interrupt lock is still held.
    reinterpret_cast<StrmPlayerRetailLayout*>(strmPlayer)->field_0x168--;
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

    // Retail uses a scope interrupt lock; the lock's scalarized mOldState
    // member occupies the last callee-saved slot.
    nw4r::ut::AutoInterruptLock lock;

    for (int i = 0; i < self->mTrackCount; i++) {
        nw4r::snd::detail::StrmTrackRetailLayout* pTrack =
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
                    nw4r::snd::detail::StrmTrackRetailLayout* pFreeTrack =
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
int Setup__Q44nw4r3snd6detail10StrmPlayerFPQ44nw4r3snd6detail14StrmBufferPooliUsi(
    StrmPlayer* pStrmPlayer,
    nw4r::snd::detail::StrmBufferPool* pBufferPool, int channels, u16 trackMask,
    int voices) {
    // The free-function definition sits at global scope; qualify lookups.
    using namespace nw4r::snd::detail;
    using nw4r::snd::detail::SoundThread;

    OSLockMutex(
        &reinterpret_cast<SoundThreadRetailLayout*>(&SoundThread::GetInstance())
             ->mMutex);

    nw4r::snd::detail::StrmPlayerRetailLayout* self =
        reinterpret_cast<nw4r::snd::detail::StrmPlayerRetailLayout*>(
            pStrmPlayer);

    if (self->mSetupFlag) {
        // Tear down any active playback first (virtual Stop()).
        reinterpret_cast<StrmPlayer*>(self)->Stop();

        OSLockMutex(
            &reinterpret_cast<SoundThreadRetailLayout*>(
                 &SoundThread::GetInstance())
                 ->mMutex);

        if (!self->mSetupFlag) {
            OSUnlockMutex(
                &reinterpret_cast<SoundThreadRetailLayout*>(
                     &SoundThread::GetInstance())
                     ->mMutex);
        } else {
            // Release the stream-data task pool.
            self->mBufferPool = NULL;
            self->mStrmDataLoadTaskPool.Destroy(
                self->mStrmDataLoadTaskArea,
                0x680); // DATA_BLOCK_COUNT_MAX * sizeof(StrmDataLoadTask)
            self->mSetupFlag = false;

            OSUnlockMutex(
                &reinterpret_cast<SoundThreadRetailLayout*>(
                     &SoundThread::GetInstance())
                     ->mMutex);
        }
    }

    reinterpret_cast<StrmPlayer*>(self)->InitParam();

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
        OSUnlockMutex(GetSoundThreadMutex());
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
            OSUnlockMutex(GetSoundThreadMutex());
            return 1;
        }

        self->mBufferAllocFlag = true;
    }

    OSRestoreInterrupts(level);

    self->mSetupFlag = true;

    OSUnlockMutex(GetSoundThreadMutex());
    return 0;
}

// Retail UpdateVoiceParams(StrmPlayer::StrmTrack*) is a StrmPlayer member, but
// the stale header has no declaration for it; emit the retail mangled symbol
// as a free function (r3 = this, r4 = track).
extern "C" void UpdateVoiceParams__Q44nw4r3snd6detail10StrmPlayerFPQ54nw4r3snd6detail10StrmPlayer9StrmTrack(
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
    // Sequential multiplies mirror retail's evaluation order.
    f32 volume = lbl_eu_8066A078 * self->mVolume;
    volume *= (f32)(conv.d - lbl_eu_8066A070) / lbl_eu_8066A07C;
    volume *= pTrack->volume;
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


