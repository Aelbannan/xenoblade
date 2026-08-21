// AXFX alloc/free hook plumbing: routes AXFX memory requests into a
// dedicated frame heap owned by the currently-hooked AxfxImpl instance.
// lbl_eu_806654C8 / lbl_eu_806654CC are retail SDA symbols (current fx /
// total allocated size) shared with the hook callbacks.

#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

AxfxImpl* AxfxImpl::mCurrentFx = NULL;
u32 AxfxImpl::mAllocatedSize = 0;

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
    lbl_eu_806654C8 = this;
    lbl_eu_806654CC = 0;
}

void AxfxImpl::RestoreAlloc(AXFXAllocHook allocHook, AXFXFreeHook freeHook) {
    AXFXSetHooks(allocHook, freeHook);
    // Retail has a dead load of mAllocatedSize here (lwz lbl_eu_806654CC,
    // result unused) - forced via volatile read to reproduce byte-identity.
    (void)*(volatile u32*)&lbl_eu_806654CC;
    lbl_eu_806654C8 = NULL;
}

void* AxfxImpl::Alloc(u32 size) {
    void* pBlock = MEMAllocFromFrmHeap(lbl_eu_806654C8->mHeap, size);

    lbl_eu_806654C8->mAllocCount++;
    lbl_eu_806654CC += ut::RoundUp(size, 4);

    return pBlock;
}

void AxfxImpl::Free(void* pBlock) {
#pragma unused(pBlock)

    if (lbl_eu_806654C8->mAllocCount != 0) {
        lbl_eu_806654C8->mAllocCount--;
    }

    if (lbl_eu_806654C8->mAllocCount == 0) {
        MEMFreeToFrmHeap(lbl_eu_806654C8->mHeap, MEM_FRM_HEAP_FREE_ALL);
    }
}

} // namespace detail
} // namespace snd
} // namespace nw4r
