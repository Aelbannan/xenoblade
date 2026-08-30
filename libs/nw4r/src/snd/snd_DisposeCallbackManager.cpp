#include <nw4r/snd.h>

namespace nw4r {
namespace snd {
namespace detail {

DisposeCallbackManager& DisposeCallbackManager::GetInstance() {
    ut::AutoInterruptLock lock;
    static DisposeCallbackManager instance;
    return instance;
}

void DisposeCallbackManager::RegisterDisposeCallback(
    DisposeCallback* pCallback) {
    mCallbackList.PushBack(pCallback);
}

void DisposeCallbackManager::UnregisterDisposeCallback(
    DisposeCallback* pCallback) {
    mCallbackList.Erase(pCallback);
}

void DisposeCallbackManager::Dispose(void* pData, u32 size, void* pArg) {
#pragma unused(pArg)

    const u8* pStart = static_cast<const u8*>(pData);
    const u8* pEnd = static_cast<const u8*>(pData) + size;
    SoundThread::AutoLock lock;

    DisposeCallbackList::Iterator it =
        GetInstance().mCallbackList.GetBeginIter();

    while (it != GetInstance().mCallbackList.GetEndIter()) {
        DisposeCallbackList::Iterator curr = it++;
        // @bug Unnecessary iteration
        curr++->InvalidateData(pStart, pEnd);
    }
}

void DisposeCallbackManager::DisposeWave(void* pData, u32 size, void* pArg) {
#pragma unused(pArg)

    const u8* pStart = static_cast<const u8*>(pData);
    const u8* pEnd = static_cast<const u8*>(pData) + size;
    SoundThread::AutoLock lock;

    DisposeCallbackList::Iterator it =
        GetInstance().mCallbackList.GetBeginIter();

    while (it != GetInstance().mCallbackList.GetEndIter()) {
        DisposeCallbackList::Iterator curr = it++;
        // @bug Unnecessary iteration
        curr++->InvalidateWaveData(pStart, pEnd);
    }
}

} // namespace detail
} // namespace snd
} // namespace nw4r

// Absorb: pad .sbss 0x1->0x8
unsigned int snd_DisposeCallbackManager_sbss_pad;
