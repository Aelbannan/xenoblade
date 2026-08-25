#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

// Retail .sbss type-info object: SeqSound::typeInfo at lbl_eu_806654F8
// (parent = BasicSound::typeInfo lbl_eu_806654D0). Definition stays in
// this TU.
extern "C" nw4r::ut::detail::RuntimeTypeInfo lbl_eu_806654F8(
    &lbl_eu_806654D0);

const nw4r::ut::detail::RuntimeTypeInfo*
SeqSound::GetRuntimeTypeInfo() const {
    return &lbl_eu_806654F8;
}

SeqSound::SeqSound(SoundInstanceManager<SeqSound>* pManager)
    // TODO(kiwi) Fakematch
    : BasicSound(0, 0),
      mTempSpecialHandle(reinterpret_cast<SeqSoundHandle*>(
          mPreparedFlag = mLoadingFlag = false)),
      mManager(pManager),
      mStartOffset(0),
      mFileStream(NULL) {

    OSInitMutex(&mMutex);
}

void SeqSound::InitParam() {
    BasicSound::InitParam();
    mStartOffset = 0;
}

SeqPlayer::SetupResult SeqSound::Setup(SeqTrackAllocator* pAllocator,
                                       u32 allocTrackFlags, int voices,
                                       NoteOnCallback* pCallback) {
    InitParam();
    return mSeqPlayer.Setup(pAllocator, allocTrackFlags, GetVoiceOutCount(),
                            pCallback);
}

void SeqSound::Prepare(const void* pBase, s32 seqOffset,
                       SeqPlayer::OffsetType startType, int startOffset) {
    mSeqPlayer.SetSeqData(pBase, seqOffset);
    Skip(startType, startOffset);

    mPreparedFlag = true;
}

void SeqSound::Prepare(ut::FileStream* pStream, s32 seqOffset,
                       SeqPlayer::OffsetType startType, int startOffset) {
    // The shared header places the tail members 8 bytes above their retail
    // offsets, so access them through a retail-layout overlay (same pattern
    // as the MoveBlocksRetail overlay in snd_StrmSound.cpp).
    // Retail also inlines LoadData() here; keep its check ordering.
    struct SeqSoundRetail {
        u8 _pad[0x298];
        s32 mSeqOffset;                          // at 0x298
        SeqPlayer::OffsetType mStartOffsetType;  // at 0x29C
        int mStartOffset;                        // at 0x2A0
        bool mLoadingFlag;                       // at 0x2A4
        ut::FileStream* mFileStream;             // at 0x2A8
        u8 _pad2[0x4B0 - 0x2B0];
        struct { // mirrors SeqLoadTask tail (fileStream at task+0x10)
            u8 _task[0x10];
            ut::FileStream* fileStream;
            void* buffer;
            int bufferSize;
            void (*callback)(bool, const void*, void*);
            SeqSound* callbackData;
        } mSeqLoadTask;                          // at 0x4AC
    };

    SeqSoundRetail* p = reinterpret_cast<SeqSoundRetail*>(this);

    // Retail PlayerHeap::Alloc is virtual (stale shared header declares it
    // non-virtual), so call it through a vtable-shape mirror.
    struct PlayerHeapMirror {
        virtual void* slot0();
        virtual void* Alloc(u32 size);
    };

    p->mFileStream = pStream;
    p->mSeqOffset = seqOffset;
    p->mStartOffsetType = startType;
    p->mStartOffset = startOffset;
    p->mLoadingFlag = true;

    // Mirrors LoadData()'s early-return structure after inlining.
    bool ok;
    s32 size;
    PlayerHeapMirror* pHeap =
        reinterpret_cast<PlayerHeapMirror*>(GetPlayerHeap());
    if (pHeap == NULL) {
        ok = false;
        goto check;
    }

    {
        size = p->mFileStream->GetSize();
        void* pData = pHeap->Alloc(size);

        if (pData == NULL) {
            ok = false;
            goto check;
        }

        p->mSeqLoadTask.fileStream = p->mFileStream;
        p->mSeqLoadTask.buffer = pData;
        p->mSeqLoadTask.bufferSize = size;
        p->mSeqLoadTask.callback = NotifyLoadAsyncEndSeqData;
        p->mSeqLoadTask.callbackData = this;

        TaskManager::GetInstance().AppendTask(
            reinterpret_cast<Task*>(&p->mSeqLoadTask));
        ok = true;
    }

check:
    if (!ok) {
        Shutdown();
    }
}

void SeqSound::NotifyLoadAsyncEndSeqData(bool success, const void* pBase,
                                         void* pCallbackArg) {
    SeqSound* p = static_cast<SeqSound*>(pCallbackArg);

    p->mLoadingFlag = false;

    if (!success) {
        p->Stop(0);
    } else {
        p->mSeqPlayer.SetSeqData(pBase, p->mSeqOffset);

        if (p->mStartOffset > 0) {
            p->mSeqPlayer.Skip(p->mStartOffsetType, p->mStartOffset);
        }

        p->mPreparedFlag = true;
    }
}

void SeqSound::Skip(SeqPlayer::OffsetType offsetType, int offset) {
    if (offset > 0) {
        mSeqPlayer.Skip(offsetType, offset);
    }
}

void SeqSound::Shutdown() {
    if (mLoadingFlag) {
        TaskManager::GetInstance().CancelTask(&mSeqLoadTask);
    }

    if (mFileStream) {
        mFileStream->Close();
        mFileStream = NULL;
    }

    BasicSound::Shutdown();
    mManager->Free(this);
}

void SeqSound::SetChannelPriority(int priority) {
    mSeqPlayer.SetChannelPriority(priority);
}

void SeqSound::SetReleasePriorityFix(bool flag) {
    mSeqPlayer.SetReleasePriorityFix(flag);
}

void SeqSound::SetPlayerPriority(int priority) {
    BasicSound::SetPlayerPriority(priority);
    mManager->UpdatePriority(this, BasicSound::CalcCurrentPlayerPriority());
}

bool SeqSound::IsAttachedTempSpecialHandle() {
    return mTempSpecialHandle != NULL;
}

void SeqSound::DetachTempSpecialHandle() {
    mTempSpecialHandle->DetachSound();
}

bool SeqSound::LoadData(SeqLoadCallback pCalllback, void* pCallbackArg) {
    mLoadingFlag = true;

    PlayerHeap* pHeap = static_cast<BasicSound*>(pCallbackArg)->GetPlayerHeap();
    if (pHeap == NULL) {
        return false;
    }

    u32 size = mFileStream->GetSize();
    void* pData = pHeap->Alloc(size);

    if (pData == NULL) {
        return false;
    }

    mSeqLoadTask.fileStream = mFileStream;
    mSeqLoadTask.buffer = pData;
    mSeqLoadTask.bufferSize = size;
    mSeqLoadTask.callback = pCalllback;
    mSeqLoadTask.callbackData = this;

    TaskManager::GetInstance().AppendTask(&mSeqLoadTask);
    return true;
}

SeqSound::SeqLoadTask::SeqLoadTask()
    : fileStream(NULL), buffer(NULL), callback(NULL), callbackData(NULL) {}

void SeqSound::SeqLoadTask::Execute() {
    fileStream->Seek(0, ut::FileStream::SEEK_ORIGIN_BEG);

    s32 bytesRead = fileStream->Read(buffer, bufferSize);
    fileStream = NULL;

    if (bytesRead == DVD_RESULT_CANCELED) {
        if (callback != NULL) {
            callback(false, NULL, callbackData);
        }
    } else if (bytesRead != bufferSize) {
        if (callback != NULL) {
            callback(false, NULL, callbackData);
        }
    } else {
        SeqFileReader reader(buffer);
        const void* pBase = reader.GetBaseAddress();

        if (callback != NULL) {
            callback(true, pBase, callbackData);
        }
    }
}

void SeqSound::SeqLoadTask::Cancel() {
    if (callback != NULL) {
        callback(false, NULL, callbackData);
    }
}

void SeqSound::SeqLoadTask::OnCancel() {
    callback = NULL;

    if (fileStream != NULL) {
        fileStream->Cancel();
    }
}

} // namespace detail
} // namespace snd
} // namespace nw4r

void SetSeqUserprocCallback__Q44nw4r3snd6detail9SeqPlayerFPFUsPQ34nw4r3snd24SeqUserprocCallbackParamPv_vPv(void* self, void (*callback)(unsigned short, void*, void*), void* arg);

void SetSeqUserprocCallback__Q44nw4r3snd6detail8SeqSoundFPFUsPQ34nw4r3snd24SeqUserprocCallbackParamPv_vPv(nw4r::snd::detail::SeqSound* thisPtr, void (*callback)(unsigned short, void*, void*), void* arg) { SetSeqUserprocCallback__Q44nw4r3snd6detail9SeqPlayerFPFUsPQ34nw4r3snd24SeqUserprocCallbackParamPv_vPv((void*)((u8*)thisPtr + 0x10c), callback, arg); }
// Flattened entry point: the SeqSound member layout lives in snd_SeqSound.h,
// but the fields this function needs are private, so they are reached through
// retail-offset overlay structs (same pattern as StrmSound's MoveBlocksRetail
// overlay in snd_StrmSound.cpp). The priority list itself stays on the real
// Flattened entry point: the SeqSound member layout lives in snd_SeqSound.h
// (not writable from this TU), so the one private field this function needs
// (mManager at 0x294) is reached through a retail-offset overlay struct.
// The manager's public UpdatePriority() inlines to exactly the retail body:
// AutoLock(OSLockMutex/OSUnlockMutex around one Erase + one sorted Insert by
// CalcCurrentPlayerPriority).
void OnUpdatePlayerPriority__Q44nw4r3snd6detail8SeqSoundFv(
    nw4r::snd::detail::SeqSound* self) {

    struct SeqSoundOfs {
        u8 _pad[0x294];
        nw4r::snd::detail::SoundInstanceManager<nw4r::snd::detail::SeqSound>*
            mManager; // at 0x294
    };

    nw4r::snd::detail::SeqSound* pSound = self; // retail's `mr r28, r3` copy

    reinterpret_cast<
        nw4r::snd::detail::SoundInstanceManager<nw4r::snd::detail::SeqSound>*>(
        reinterpret_cast<SeqSoundOfs*>(self)->mManager)
        ->UpdatePriority(pSound, self->CalcCurrentPlayerPriority());
}
extern "C" void* GetBasicPlayer__Q44nw4r3snd6detail8SeqSoundFv(void* self) { return (void*)((u8*)self + 0x10c); }
extern "C" void* GetBasicPlayer__Q44nw4r3snd6detail8SeqSoundCFv(void* self) { return (void*)((u8*)self + 0x10c); }
extern "C" u8 IsPrepared__Q44nw4r3snd6detail8SeqSoundCFv(void* self) { return *(static_cast<u8*>(self) + 677); }
