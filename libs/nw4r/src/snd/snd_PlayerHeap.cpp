#include <nw4r/snd/snd_PlayerHeap.h>

#include <nw4r/snd/snd_DisposeCallbackManager.h>
#include <nw4r/snd/snd_SoundThread.h>

namespace nw4r {
namespace snd {
namespace detail {

PlayerHeap::PlayerHeap()
    : mSound(NULL),
      mPlayer(NULL),
      mStart(NULL),
      mUnk0x10(NULL),
      mEnd(NULL) {}

PlayerHeap::~PlayerHeap() {
    {
        SoundThread::AutoLock lock;

        void* pStart = mStart;
        u32 bytes = static_cast<u32>(static_cast<u8*>(mEnd) -
                                     static_cast<u8*>(pStart));

        DisposeCallbackManager::GetInstance().Dispose(pStart, bytes, NULL);
        DisposeCallbackManager::GetInstance().DisposeWave(pStart, bytes, NULL);
        mEnd = mStart;
    }
    mEnd = NULL;
}

bool PlayerHeap::Create(void* pBase, u32 size) {
    u32 base = reinterpret_cast<u32>(pBase);
    u32 aligned = (base + 0x1F) & ~0x1Fu;
    u32 limit = size + base;

    if (aligned > limit) {
        return false;
    }

    mStart = reinterpret_cast<void*>(aligned);
    mUnk0x10 = reinterpret_cast<void*>(limit);
    mEnd = reinterpret_cast<void*>(aligned);
    return true;
}

void* PlayerHeap::Alloc(u32 size) {
    u8* pCur = static_cast<u8*>(mEnd);
    u8* pNext = pCur + size;

    if (pNext > static_cast<u8*>(mUnk0x10)) {
        return NULL;
    }

    mEnd = reinterpret_cast<void*>(
        (reinterpret_cast<u32>(pNext) + 0x1F) & ~0x1Fu);
    return pCur;
}

void PlayerHeap::Clear() {
    SoundThread::AutoLock lock;

    DisposeCallbackManager::GetInstance().Dispose(
        mStart, reinterpret_cast<u32>(mEnd) - reinterpret_cast<u32>(mStart),
        NULL);
    DisposeCallbackManager::GetInstance().DisposeWave(
        mStart, reinterpret_cast<u32>(mEnd) - reinterpret_cast<u32>(mStart),
        NULL);
    mEnd = mStart;
}

u32 PlayerHeap::GetFreeSize() const {
    return static_cast<u32>(static_cast<u8*>(mUnk0x10) - static_cast<u8*>(mEnd));
}

void PlayerHeap::AttachSound(BasicSound* pSound) {
    mSound = pSound;
}

void PlayerHeap::DetachSound(BasicSound* pSound) {
    (void)pSound;
    mSound = NULL;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
