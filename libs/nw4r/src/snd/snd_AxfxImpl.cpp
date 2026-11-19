// AXFX alloc/free hook plumbing: routes AXFX memory requests into a
// dedicated frame heap owned by the currently-hooked AxfxImpl instance.
// lbl_eu_806654C8 / lbl_eu_806654CC are retail SDA symbols (current fx /
// total allocated size) shared with the hook callbacks.

#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

// Retail .sbss slots owned by this TU (split 0x806654C8-0x806654D0): current
// hook target (0xC8) and total allocated size (0xCC). Defined here; the
// bodies above use them through the header's extern "C" declarations.

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
    // result unused): the running total is shared with the AX callback thread,
    // so the slot is genuinely volatile. Declaring lbl_eu_806654CC volatile in
    // snd_AxfxImpl.h was measured to flip the epilogue save/restore order of
    // HookAlloc and Alloc (both FULL today), hence the qualifier at the use.
    (void)*(volatile u32*)&lbl_eu_806654CC;  // volatile-ok: AX-thread-shared counter; volatile decl breaks HookAlloc/Alloc
    lbl_eu_806654C8 = NULL;
}

void* AxfxImpl::Alloc(u32 size) {
    u8* pBlock = static_cast<u8*>(
        MEMAllocFromFrmHeap(lbl_eu_806654C8->mHeap, size));

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

// Retail .sbss storage owned by this TU (split 0x806654C8-0x806654D0).
// Zero-initialized -> .sbss, matching retail size 0x8.
extern "C" nw4r::snd::detail::AxfxImpl* lbl_eu_806654C8 = NULL;
extern "C" u32 lbl_eu_806654CC = 0;
