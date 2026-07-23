#include <nw4r/snd.h>
#include <nw4r/ut.h>

// Retail SDA names (US) - Free/Alloc/Hook must use these for reloc match.
extern "C" nw4r::snd::detail::AxfxImpl* lbl_eu_806654C8;
extern "C" u32 lbl_eu_806654CC;

namespace nw4r {
namespace snd {
namespace detail {

AxfxImpl* AxfxImpl::mCurrentFx = NULL;
u32 AxfxImpl::mAllocatedSize = 0;

bool AxfxImpl::CreateHeap(void* pBuffer, u32 size) {
    mHeap = MEMCreateFrmHeap(pBuffer, size);
    return mHeap != NULL;
}

void AxfxImpl::DestroyHeap() {
    if (mHeap != NULL) {
        MEMDestroyFrmHeap(mHeap);
    }
}

void AxfxImpl::HookAlloc(AXFXAllocHook* pAllocHook, AXFXFreeHook* pFreeHook) {
    AXFXGetHooks(pAllocHook, pFreeHook);
    AXFXSetHooks(Alloc, Free);
    lbl_eu_806654C8 = this;
}

void AxfxImpl::RestoreAlloc(AXFXAllocHook allocHook, AXFXFreeHook freeHook) {
    AXFXSetHooks(allocHook, freeHook);
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
