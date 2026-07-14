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
        return mActiveFlag;
    } // at 0x18
    virtual bool IsStarted() const {
        return mStartedFlag;
    } // at 0x1C
    virtual bool IsPause() const {
        return mPauseFlag;
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
    void Shutdown();

    bool Prepare(ut::FileStream* pFileStream, int voices,
                 StartOffsetType offsetType, int offset);
    void InitParam();

    void Update();

private:
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

    static const int DATA_BLOCK_COUNT_MIN = 4;
    static const int DATA_BLOCK_COUNT_MAX = 32;
    static const int DATA_BLOCK_SIZE_MAX = 0x2000;

    // TODO(kiwi) How is this calculated?
    static const int LOAD_BUFFER_SIZE = 0x4000 + 32;

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

private:
    // Retail StrmPlayer reserves 0x68 bytes between its base subobjects and
    // the stream-specific state. Without this gap every member below is
    // addressed 0x68 bytes too early (for example mTaskCancelFlag at 0xBD
    // instead of 0x125).
    u8 _padding80[0x68];
    StrmInfo mStrmInfo; // at 0xE8

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

    StartOffsetType mStartOffsetType; // at 0x168
    int mStartOffset;                 // at 0x16C

    StrmHeaderLoadTask mStrmHeaderLoadTask;                       // at 0x170
    StrmDataLoadTaskList mStrmDataLoadTaskList;                   // at 0x190
    InstancePool<StrmDataLoadTask> mStrmDataLoadTaskPool;         // at 0x19C
    StrmDataLoadTask mStrmDataLoadTaskArea[DATA_BLOCK_COUNT_MAX]; // at 0x1A0

    StrmBufferPool* mBufferPool; // at 0x820
    ut::FileStream* mFileStream; // at 0x824
    Voice* mVoice;               // at 0x828
    s32 mChannelCount;           // at 0x82C
    s32 mVoiceOutCount;          // at 0x830

    StrmChannel mChannels[CHANNEL_MAX];   // at 0x834
    u16 mAdpcmLoopPredScale[CHANNEL_MAX]; // at 0x8A4
    u16 mAdpcmLoopYn1[CHANNEL_MAX];       // at 0x8A8
    u16 mAdpcmLoopYn2[CHANNEL_MAX];       // at 0x8AC

    static u8 sLoadBuffer[LOAD_BUFFER_SIZE] ALIGN(32);
    static OSMutex sLoadBufferMutex;

    static bool sStaticInitFlag;
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
