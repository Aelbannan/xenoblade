// AXFX alloc/free hook plumbing: routes AXFX memory requests into a
// dedicated frame heap owned by the currently-hooked AxfxImpl instance.
// AxfxImpl::mCurrentFx and AxfxImpl::mAllocatedSize are the static members
// shared with the hook callbacks.

#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

// The header declares the current hook target and the running allocation
// total as AxfxImpl static members. Define them here in declaration order so
// MWCC emits the two retail .sbss slots owned by this TU.

bool AxfxImpl::CreateHeap(void* pBuffer, u32 size) {
    if (pBuffer == NULL || size == 0) {
        mHeap = NULL;
        return false;
    }

    mHeap = MEMCreateFrmHeapEx(pBuffer, size, 0);
    return mHeap != NULL;
}

void AxfxImpl::DestroyHeap() {
    if (mHeap != NULL) {
        MEMDestroyFrmHeap(mHeap);
        mHeap = NULL;
    }
}

void AxfxImpl::HookAlloc(AXFXAllocHook* pAllocHook, AXFXFreeHook* pFreeHook) {
    AXFXGetHooks(pAllocHook, pFreeHook);
    AXFXSetHooks(Alloc, Free);
    mCurrentFx = this;
    mAllocatedSize = 0;
}

u32 AxfxImpl::RestoreAlloc(AXFXAllocHook allocHook, AXFXFreeHook freeHook) {
    AXFXSetHooks(allocHook, freeHook);
    mCurrentFx = NULL;
    return mAllocatedSize;
}

void* AxfxImpl::Alloc(u32 size) {
    u8* pBlock = static_cast<u8*>(
        MEMAllocFromFrmHeap(mCurrentFx->mHeap, size));

    mCurrentFx->mAllocCount++;
    mAllocatedSize += ut::RoundUp(size, 4);

    return pBlock;
}

void AxfxImpl::Free(void* pBlock) {
#pragma unused(pBlock)

    if (mCurrentFx->mAllocCount != 0) {
        mCurrentFx->mAllocCount--;
    }

    if (mCurrentFx->mAllocCount == 0) {
        MEMFreeToFrmHeap(mCurrentFx->mHeap, MEM_FRM_HEAP_FREE_ALL);
    }
}

} // namespace detail
} // namespace snd
} // namespace nw4r

// Retail .sbss storage owned by this TU (split 0x806654C8-0x806654D0).
// Zero-initialized -> .sbss, matching retail size 0x8.
nw4r::snd::detail::AxfxImpl* nw4r::snd::detail::AxfxImpl::mCurrentFx = NULL;
u32 nw4r::snd::detail::AxfxImpl::mAllocatedSize = 0;
