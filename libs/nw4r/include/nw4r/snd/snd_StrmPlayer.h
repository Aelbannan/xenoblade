#ifndef NW4R_SND_STRM_PLAYER_H
#define NW4R_SND_STRM_PLAYER_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_BasicPlayer.h>
#include <nw4r/snd/snd_InstancePool.h>
#include <nw4r/snd/snd_SoundThread.h>
#include <nw4r/snd/snd_StrmChannel.h>
#include <nw4r/snd/snd_StrmFile.h>
#include <nw4r/snd/snd_Task.h>
#include <nw4r/snd/snd_Voice.h>

#include <nw4r/ut.h>

#include <revolution/OS.h>

namespace nw4r {
namespace snd {
namespace detail {

// Retail StrmPlayer field layouts. The sibling StrmInfo/StrmChannel headers
// describe older sound-archive formats and do not match the fields stored in
// this player in the shipped binary; these are the canonical player types.
struct StrmPlayerInfo {
    s32 format;                  // at 0x0
    u8 loopFlag;                 // at 0x4
    u8 _pad0x5[3];               // at 0x5
    s32 numChannels;             // at 0x8
    s32 sampleRate;              // at 0xC
    u16 blockHeaderOffset;       // at 0x10
    u16 _pad0x12;                // at 0x12
    u32 loopStart;               // at 0x14
    u32 loopEnd;                 // at 0x18
    u32 dataOffset;              // at 0x1C
    u32 numBlocks;               // at 0x20
    u32 blockSize;               // at 0x24
    s32 blockSamples;            // at 0x28
    u32 lastBlockSize;           // at 0x2C
    u32 lastBlockSamples;        // at 0x30
    u32 lastBlockPaddedSize;     // at 0x34
    u32 adpcmDataInterval;       // at 0x38
    u32 adpcmDataSize;           // at 0x3C
};                               // sizeof 0x40

struct StrmPlayerAdpcmParamSet {
    AdpcmParam param;         // at 0x0 (0x28)
    AdpcmLoopParam loopParam; // at 0x28 (0x6)
};                            // sizeof 0x2E

struct StrmPlayerChannel {
    void* bufferAddress;                 // at 0x0
    StrmPlayerAdpcmParamSet adpcm;       // at 0x4 (0x2E)
    u16 mAdpcmLoopPredScale;             // at 0x32
};                                       // sizeof 0x34

struct StrmPlayerTrackInfo {
    u8 volume;             // at 0x0
    u8 pan;                // at 0x1
    u8 _pad0x2[2];         // at 0x2
    int channelCount;      // at 0x4
    u8 channelIndex[0x20]; // at 0x8
};                         // sizeof 0x28

class StrmPlayer : public BasicPlayer, public SoundThread::PlayerCallback {
    friend class StrmHeaderLoadTask;
    friend class StrmDataLoadTask;

public:
    enum StartOffsetType {
        START_OFFSET_TYPE_SAMPLE,
        START_OFFSET_TYPE_MILLISEC
    };

    struct StrmHeader {
        // TODO(kiwi) Why 8 if the player only supports 2???
        static const int STRM_CHANNEL_MAX = 8;

        StrmInfo strmInfo;                     // at 0x0
        AdpcmInfo adpcmInfo[STRM_CHANNEL_MAX]; // at 0x38
        u16 loopYn1[STRM_CHANNEL_MAX];         // at 0x1B8
        u16 loopYn2[STRM_CHANNEL_MAX];         // at 0x1D8
    };

public:
    StrmPlayer();
    virtual ~StrmPlayer(); // at 0x8

    virtual bool Start();          // at 0xC
    virtual void Stop();           // at 0x10
    virtual void Pause(bool flag); // at 0x14

    virtual bool IsActive() const {
        return *(bool*)((u8*)this + 289);
    } // at 0x18
    virtual bool IsStarted() const {
        return *(bool*)((u8*)this + 290);
    } // at 0x1C
    virtual bool IsPause() const {
        return *(bool*)((u8*)this + 295);
    }; // at 0x20

    virtual void OnUpdateFrameSoundThread() {
        Update();
    } // at 0xC
    virtual void OnUpdateVoiceSoundThread() {
        UpdateBuffer();
    } // at 0x10
    virtual void OnShutdownSoundThread() {
        Stop();
    } // at 0x14

    bool IsPrepared() const {
        return mPreparedFlag;
    }

    void SetTaskErrorFlag() {
        mTaskErrorFlag = true;
    }
    void SetTaskCancelFlag() {
        mTaskCancelFlag = true;
    }

    bool Setup(StrmBufferPool* pBufferPool);
    int Setup(StrmBufferPool* pBufferPool, int channels, u16 trackMask,
              int voices);
    void Shutdown();

    bool Prepare(ut::FileStream* pFileStream, int voices,
                 StartOffsetType offsetType, int offset);
    bool Prepare(ut::FileStream* pFileStream, StartOffsetType offsetType,
                 int offset);
    void InitParam();

    struct PlayerTrack {
        u8 activeFlag;                    // at 0x0
        u8 _pad0x1[3];                    // at 0x1
        Voice* voice;                     // at 0x4
        StrmPlayerTrackInfo fileInfo;     // at 0x8 (0x28)
        f32 volume;                       // at 0x30
        f32 field_0x34;                   // at 0x34
    };                                    // sizeof 0x38

    PlayerTrack* GetPlayerTrack(int index);

    void Update();
    bool AllocVoices(int voices);

public:
    /******************************************************************************
     * StrmHeaderLoadTask
     ******************************************************************************/
    struct StrmHeaderLoadTask : public Task {
        StrmHeaderLoadTask();

        virtual void Execute();  // at 0xC
        virtual void Cancel();   // at 0x10
        virtual void OnCancel(); // at 0x14

        StrmPlayer* strmPlayer;          // at 0x10
        ut::FileStream* fileStream;      // at 0x14
        StartOffsetType startOffsetType; // at 0x18
        s32 startOffset;                 // at 0x1C
    };

    /******************************************************************************
     * StrmDataLoadTask
     ******************************************************************************/
    struct StrmDataLoadTask : public Task {
        StrmDataLoadTask();

        virtual void Execute();  // at 0xC
        virtual void Cancel();   // at 0x10
        virtual void OnCancel(); // at 0x14

        StrmPlayer* strmPlayer;     // at 0x10
        ut::FileStream* fileStream; // at 0x14
        u32 size;                   // at 0x18
        s32 offset;                 // at 0x1C
        u32 blockSize;              // at 0x20
        s32 bufferBlockIndex;       // at 0x24
        bool needUpdateAdpcmLoop;   // at 0x28

        NW4R_UT_LINKLIST_NODE_DECL(); // at 0x2C
    };

    NW4R_UT_LINKLIST_TYPEDEF_DECL(StrmDataLoadTask);

private:
    static const int DATA_BLOCK_COUNT_MIN = 4;
    static const int DATA_BLOCK_COUNT_MAX = 32;
    static const int DATA_BLOCK_SIZE_MAX = 0x2000;

    static const int LOAD_BUFFER_SIZE = 0x4000;

private:
    bool LoadHeader(ut::FileStream* pFileStream, StartOffsetType offsetType,
                    int offset);
    bool LoadStreamData(ut::FileStream* pFileStream, int offset, u32 size,
                        u32 blockSize, int blockIndex,
                        bool needUpdateAdpcmLoop);

    bool SetupPlayer(const StrmHeader* pStrmHeader);

    bool AllocChannels(int channels, int voices);
    void FreeChannels();

    void UpdateBuffer();
    void UpdateLoopAddress(u32 startSample, u32 endSample);
    void UpdatePlayingBlockIndex();
    void UpdateDataLoopAddress(s32 endBlock);
    void SetLoopEndToZeroBuffer(int endBlock);
    void UpdateLoadingBlockIndex();
    void UpdatePauseStatus();

    int CalcLoadingBufferBlockCount() const;
    bool CalcStartOffset(s32* pBlockIndex, u32* pBlockOffset, s32* pLoopCount);

    static void VoiceCallbackFunc(Voice* pDropVoice,
                                  Voice::VoiceCallbackStatus status,
                                  void* pCallbackArg);

    void SetAdpcmLoopContext(int channels, u16* pPredScale);

public:
    // Retail field map used by the player and its task helpers.
    // BasicPlayer (0xD4) + SoundThread::PlayerCallback (0xC) put the
    // stream-specific state at retail offset 0xE0.
    StrmPlayerInfo mStrmInfo; // at 0xE0

    bool mSetupFlag;           // at 0x120
    bool mActiveFlag;          // at 0x121
    bool mStartedFlag;         // at 0x122
    bool mPreparedFlag;        // at 0x123
    bool mTaskErrorFlag;       // at 0x124
    bool mTaskCancelFlag;      // at 0x125
    bool mLoadingDelayFlag;    // at 0x126
    bool mPauseFlag;           // at 0x127
    bool mPauseStatus;         // at 0x128
    bool mLoadWaitFlag;        // at 0x129
    bool mNoRealtimeLoadFlag;  // at 0x12A
    bool mSkipUpdateAdpcmLoop; // at 0x12B
    bool mValidAdpcmLoop;      // at 0x12C
    bool mPlayFinishFlag;      // at 0x12D
    bool mLoadFinishFlag;      // at 0x12E
    bool mBufferAllocFlag;     // at 0x12F

    s32 mLoopCounter;    // at 0x130
    int mPrepareCounter; // at 0x134

    int mChangeNumBlocks;      // at 0x138
    int mDataBlockSize;        // at 0x13C
    int mBufferBlockCount;     // at 0x140
    int mBufferBlockCountBase; // at 0x144

    int mLoadingBufferBlockCount; // at 0x148
    int mLoadingBufferBlockIndex; // at 0x14C
    int mLoadingDataBlockIndex;   // at 0x150

    int mPlayingBufferBlockCount; // at 0x154
    int mPlayingBufferBlockIndex; // at 0x158
    int mPlayingDataBlockIndex;   // at 0x15C
    int mLoopStartBlockIndex;     // at 0x160
    int mLastBlockIndex;          // at 0x164

    s32 mCurBlockIndex;           // at 0x168 (pending load-task count)

    StartOffsetType mStartOffsetType; // at 0x16C
    int mStartOffset;                 // at 0x170

    StrmHeaderLoadTask mStrmHeaderLoadTask;                       // at 0x174
    StrmDataLoadTaskList mStrmDataLoadTaskList;                   // at 0x194
    InstancePool<StrmDataLoadTask> mStrmDataLoadTaskPool;         // at 0x1A0
    StrmDataLoadTask mStrmDataLoadTaskArea[DATA_BLOCK_COUNT_MAX]; // at 0x1A4

    StrmBufferPool* mBufferPool; // at 0x824
    union {
        Voice* mVoice;               // at 0x828 (voice while playing)
        ut::FileStream* mFileStream; // at 0x828 (stream while preparing)
    };
    s32 mTrackCount;   // at 0x82C
    s32 mChannelCount; // at 0x830
    s32 mVoiceOutCount; // at 0x834

    StrmPlayerChannel mChannels[16]; // at 0x838 (16 * 0x34 = 0x340)
    PlayerTrack mTracks[8];          // at 0xB78 (8 * 0x38 = 0x1C0)

    static u8 sLoadBuffer[LOAD_BUFFER_SIZE];
    static OSMutex sLoadBufferMutex;
    static bool sStaticInitFlag;
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
