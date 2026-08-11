#include <nw4r/snd.h>
#include <nw4r/ut.h>

#include <climits>
#include <cstring>

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

struct StrmTrackRetailLayout {
    u8 activeFlag;              // at 0x0
    u8 _pad0x1[3];              // at 0x1
    Voice* voice;               // at 0x4
    u8 _pad0x8[0xC - 0x8];      // at 0x8
    int channelCount;           // at 0xC
    u8 trackInfo[0x30 - 0x10];  // at 0x10
    f32 volume;                 // at 0x30
    f32 field_0x34;             // at 0x34
};                              // sizeof 0x38

struct StrmPlayerRetailLayout {
    u8 _pad0x0[0x124];                            // 0x000..0x124
    bool mTaskErrorFlag;                          // at 0x124
    bool mTaskCancelFlag;                         // at 0x125
    u8 _pad0x126[0x194 - 0x126];                  // 0x126..0x194
    // StrmPlayer::StrmDataLoadTaskList (private typedef) expanded here; fully
    // qualified because a namespace-scope friend decl shadows the nested type.
    nw4r::ut::LinkList<StrmPlayer::StrmDataLoadTask,
                       offsetof(StrmPlayer::StrmDataLoadTask, node)>
        mStrmDataLoadTaskList;                     // at 0x194 (0xC)
    InstancePool<StrmPlayer::StrmDataLoadTask> mStrmDataLoadTaskPool; // at 0x1A0 (0x4)
    u8 _pad0x1A4[0x82C - 0x1A4];                  // 0x1A4..0x82C
    s32 mTrackCount;                              // at 0x82C
    u8 _pad0x830[0xB78 - 0x830];                  // 0x830..0xB78
    StrmTrackRetailLayout mTracks[8];             // at 0xB78 (8 * 0x38)
};                                                // sizeof 0xD38

// Voice pitch-modulation gate byte at retail offset +0xA1 (see snd_Voice.cpp
// field_0xA1); snd_Voice.h's stale layout lacks the byte at this offset.
struct VoicePitchGateLayout {
    u8 _pad0x0[0xA1];
    u8 field_0xA1; // at 0xA1
};

} // namespace

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
    SoundThread::AutoLock lock;

    if (!mPreparedFlag) {
        return false;
    }

    if (!mStartedFlag) {
        s32 blockIndex = 0;
        u32 blockOffset = 0;
        s32 loopCount = 0;

        if (!CalcStartOffset(&blockIndex, &blockOffset, &loopCount)) {
            return false;
        }

        mLoopCounter += loopCount;

        WaveData waveData;
        waveData.sampleFormat = mStrmInfo.format;
        waveData.loopFlag = true;
        waveData.numChannels = mChannelCount;
        waveData.sampleRate = mStrmInfo.sampleRate;
        waveData.loopStart = 0;

        AxVoice::Format format =
            WaveFileReader::GetAxVoiceFormatFromWaveFileFormat(
                mStrmInfo.format);

        waveData.loopEnd = AxVoice::GetSampleByByte(
            mDataBlockSize * mPlayingBufferBlockCount, format);

        for (int i = 0; i < mChannelCount; i++) {
            ChannelParam& rParam = waveData.channelParam[i];

            rParam.dataAddr = mChannels[i].bufferAddress;
            rParam.adpcmInfo = mChannels[i].adpcmInfo;

            // Isn't the scale only the *bottom nibble*?
            rParam.adpcmInfo.param.pred_scale =
                *static_cast<u8*>(mChannels[i].bufferAddress);

            rParam.adpcmInfo.param.yn1 = mAdpcmLoopYn1[i];
            rParam.adpcmInfo.param.yn2 = mAdpcmLoopYn2[i];
        }

        ut::AutoInterruptLock lock;

        if (mVoice != NULL) {
            mVoice->Setup(waveData, blockOffset);
            mVoice->SetVoiceType(AxVoice::VOICE_TYPE_STREAM);

            if (blockIndex == mStrmInfo.numBlocks - 2) {
                UpdateDataLoopAddress(1);
            } else if (blockIndex == mStrmInfo.numBlocks - 1) {
                UpdateDataLoopAddress(0);
            }

            mVoice->Start();
            UpdatePauseStatus();
        }

        mStartedFlag = true;
    }

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
    ut::detail::AutoLock<OSMutex> lock(sLoadBufferMutex);

    StrmFileLoader loader(*pFileStream);
    if (!loader.LoadFileHeader(sLoadBuffer,
                               ut::RoundUp(sizeof(StrmHeader), 64))) {
        return false;
    }

    StrmHeader header;
    loader.ReadStrmInfo(&header.strmInfo);

    for (int i = 0; i < header.strmInfo.numChannels; i++) {
        loader.ReadAdpcmInfo(&header.adpcmInfo[i], i);
    }

    if (header.strmInfo.format == WaveFile::FORMAT_ADPCM) {
        if (offset == 0) {
            for (int i = 0; i < header.strmInfo.numChannels; i++) {
                header.loopYn1[i] = header.adpcmInfo[i].param.yn1;
                header.loopYn2[i] = header.adpcmInfo[i].param.yn2;
            }
        } else {
            int startSample;
            if (offsetType == START_OFFSET_TYPE_SAMPLE) {
                startSample = offset;
            } else if (offsetType == START_OFFSET_TYPE_MILLISEC) {
                startSample = offset * header.strmInfo.sampleRate / 1000;
            }

            s32 block =
                startSample / static_cast<s32>(header.strmInfo.blockSamples);

            loader.ReadAdpcBlockData(header.loopYn1, header.loopYn2, block,
                                     header.strmInfo.numChannels);
        }
    }

    if (!SetupPlayer(&header)) {
        return false;
    }

    mPrepareCounter = 0;
    for (int i = 0; i < mBufferBlockCountBase; i++) {
        UpdateLoadingBlockIndex();
        mPrepareCounter++;

        if (mLoadFinishFlag) {
            break;
        }
    }

    if (mStrmInfo.numBlocks <= 2 && !mStrmInfo.loopFlag) {
        SetLoopEndToZeroBuffer(mStrmInfo.numBlocks - 1);
    }

    return true;
}

bool StrmPlayer::LoadStreamData(ut::FileStream* pFileStream, int offset,
                                u32 size, u32 blockSize, int blockIndex,
                                bool needUpdateAdpcmLoop) {
    ut::DvdFileStream* pDvdStream =
        ut::DynamicCast<ut::DvdFileStream*>(pFileStream);

    if (pDvdStream != NULL) {
        pDvdStream->SetPriority(DVD_PRIO_HIGH);
    }

    ut::detail::AutoLock<OSMutex> lock(sLoadBufferMutex);
    DCInvalidateRange(sLoadBuffer, size);

    pFileStream->Seek(offset, ut::FileStream::SEEK_ORIGIN_BEG);
    s32 bytesRead = pFileStream->Read(sLoadBuffer, size);

    if (bytesRead != size) {
        return false;
    }

    u16 adpcmPredScale[CHANNEL_MAX];
    for (int i = 0; i < mChannelCount; i++) {
        if (needUpdateAdpcmLoop) {
            adpcmPredScale[i] = sLoadBuffer[i * ut::RoundUp(blockSize, 32) +
                                            mStrmInfo.blockHeaderOffset];
        }

        const void* pSrc =
            ut::AddOffsetToPtr(sLoadBuffer, i * ut::RoundUp(blockSize, 32) +
                                                mStrmInfo.blockHeaderOffset);

        void* pDst = ut::AddOffsetToPtr(mChannels[i].bufferAddress,
                                        mDataBlockSize * blockIndex);

        u32 len = ut::RoundUp(blockSize, 32);
        std::memcpy(pDst, pSrc, len);
        DCFlushRange(pDst, len);
    }

    if (needUpdateAdpcmLoop) {
        SetAdpcmLoopContext(mChannelCount, adpcmPredScale);
    }

    if (!mPreparedFlag && --mPrepareCounter == 0) {
        mPreparedFlag = true;
    }

    return true;
}

bool StrmPlayer::SetupPlayer(const StrmHeader* pStrmHeader) {
    u32 poolBlockSize = mBufferPool->GetBlockSize();
    mStrmInfo = pStrmHeader->strmInfo;

    s32 blockIndex = 0;
    u32 blockOffset = 0;
    s32 loopCount = 0;
    if (!CalcStartOffset(&blockIndex, &blockOffset, &loopCount)) {
        return false;
    }

    if (mStrmInfo.format == WaveFile::FORMAT_ADPCM) {
        for (int i = 0; i < mStrmInfo.numChannels; i++) {
            mChannels[i].adpcmInfo = pStrmHeader->adpcmInfo[i];
            mAdpcmLoopYn1[i] = pStrmHeader->loopYn1[i];
            mAdpcmLoopYn2[i] = pStrmHeader->loopYn2[i];
        }
    }

    mLoopStartBlockIndex = mStrmInfo.loopStart / mStrmInfo.blockSamples;
    mLastBlockIndex = mStrmInfo.numBlocks - 1;

    mDataBlockSize = mStrmInfo.blockSize;
    if (mDataBlockSize > DATA_BLOCK_SIZE_MAX) {
        return false;
    }

    mBufferBlockCount = poolBlockSize / mDataBlockSize;
    if (mBufferBlockCount < DATA_BLOCK_COUNT_MIN) {
        return false;
    } else if (mBufferBlockCount > DATA_BLOCK_COUNT_MAX) {
        mBufferBlockCount = DATA_BLOCK_COUNT_MAX;
    }

    mBufferBlockCountBase = mBufferBlockCount - 1;
    mChangeNumBlocks = mBufferBlockCountBase;

    mPlayingDataBlockIndex = blockIndex;
    mLoadingDataBlockIndex = blockIndex;

    mLoadingBufferBlockIndex = 0;
    mPlayingBufferBlockIndex = 0;

    if (mNoRealtimeLoadFlag) {
        mLoadingBufferBlockCount = mStrmInfo.numBlocks;
    } else {
        mLoadingBufferBlockCount = CalcLoadingBufferBlockCount();
    }

    mPlayingBufferBlockCount = mLoadingBufferBlockCount;

    ut::AutoInterruptLock lock;
    mChannelCount = ut::Min<int>(mStrmInfo.numChannels, CHANNEL_MAX);
    if (!AllocChannels(mChannelCount, mVoiceOutCount)) {
        return false;
    }

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
                    pTrack->channelCount, voices, 0xFF,
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
void UpdateVoiceParams__Q44nw4r3snd6detail10StrmPlayerFPQ54nw4r3snd6detail10StrmPlayer9StrmTrack(){}
extern "C" void OnUpdateFrameSoundThread__Q44nw4r3snd6detail10StrmPlayerFv() {}
extern "C" void OnUpdateVoiceSoundThread__Q44nw4r3snd6detail10StrmPlayerFv() {}

extern "C" u8 IsPause__Q44nw4r3snd6detail10StrmPlayerCFv(StrmPlayer* self) { return self->IsPause(); }
extern "C" u8 IsStarted__Q44nw4r3snd6detail10StrmPlayerCFv(StrmPlayer* self) { return self->IsStarted(); }
extern "C" u8 IsActive__Q44nw4r3snd6detail10StrmPlayerCFv(StrmPlayer* self) { return self->IsActive(); }
