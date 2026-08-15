#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

// Retail compiles the interrupt-lock RAII inline and never emits an out-of-line
// AutoInterruptLock destructor; spelled out as explicit calls so the TU stays
// byte-identical without the spurious weak/local destructor symbols.

u32 PoolImpl::CreateImpl(void* pBuffer, u32 size, u32 stride) {
    BOOL old = OSDisableInterrupts();

    u8* pPtr = static_cast<u8*>(ut::RoundUp(pBuffer, 4));
    stride = ut::RoundUp(stride, 4);

    // Account for the aligned buffer address
    u32 length = (size - ut::GetOffsetFromPtr(pBuffer, pPtr)) / stride;

    for (u32 i = 0; i < length; i++, pPtr += stride) {
        PoolImpl* pHead = reinterpret_cast<PoolImpl*>(pPtr);
        pHead->mNext = mNext;
        mNext = pHead;
    }

    OSRestoreInterrupts(old);

    return length;
}

void PoolImpl::DestroyImpl(void* pBuffer, u32 size) {
    BOOL old = OSDisableInterrupts();

    void* pBegin = pBuffer;
    void* pEnd = static_cast<u8*>(pBegin) + size;

    PoolImpl* pIt = mNext;
    PoolImpl* pPrev = this;

    for (; pIt != NULL; pIt = pIt->mNext) {
        if (pBegin <= pIt && pIt < pEnd) {
            pPrev->mNext = pIt->mNext;
        } else {
            pPrev = pIt;
        }
    }

    OSRestoreInterrupts(old);
}

int PoolImpl::CountImpl() const {
    BOOL old = OSDisableInterrupts();

    int num = 0;

    for (PoolImpl* pIt = mNext; pIt != NULL; pIt = pIt->mNext) {
        num++;
    }

    OSRestoreInterrupts(old);

    return num;
}

void* PoolImpl::AllocImpl() {
    BOOL old = OSDisableInterrupts();

    if (mNext == NULL) {
        OSRestoreInterrupts(old);
        return NULL;
    }

    PoolImpl* pHead = mNext;
    mNext = pHead->mNext;

    OSRestoreInterrupts(old);

    return pHead;
}

void PoolImpl::FreeImpl(void* pElem) {
    BOOL old = OSDisableInterrupts();

    PoolImpl* pHead = static_cast<PoolImpl*>(pElem);
    pHead->mNext = mNext;
    mNext = pHead;

    OSRestoreInterrupts(old);
}

} // namespace detail
} // namespace snd
} // namespace nw4r
