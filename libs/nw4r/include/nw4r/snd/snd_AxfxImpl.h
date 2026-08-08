#ifndef NW4R_SND_AXFX_IMPL_H
#define NW4R_SND_AXFX_IMPL_H
#include <nw4r/types_nw4r.h>

#include <revolution/AXFX.h>
#include <revolution/MEM.h>

namespace nw4r {
namespace snd {
namespace detail {

struct AxfxImpl {
    MEMiHeapHead* mHeap; // at 0x0
    u32 mAllocCount;     // at 0x4
    bool mIsActive;      // at 0x8

    static const u32 HEAP_SIZE_MIN = MEM_FRM_HEAP_MIN_SIZE + 32;

    AxfxImpl() : mHeap(NULL), mAllocCount(0), mIsActive(false) {}

    bool CreateHeap(void* pBuffer, u32 size);
    void DestroyHeap();

    u32 GetHeapTotalSize() {
        if (mHeap == NULL) {
            return 0;
        }

        return MEMGetHeapTotalSize(mHeap);
    }

    void HookAlloc(AXFXAllocHook* pAllocHook, AXFXFreeHook* pFreeHook);
    void RestoreAlloc(AXFXAllocHook allocHook, AXFXFreeHook freeHook);

    static void* Alloc(u32 size);
    static void Free(void* pBlock);

    static AxfxImpl* mCurrentFx;
    static u32 mAllocatedSize;
};

} // namespace detail
} // namespace snd
} // namespace nw4r

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// Retail SDA names (US) - Free/Alloc/Hook must use these for reloc match.
extern "C" nw4r::snd::detail::AxfxImpl* lbl_eu_806654C8;
extern "C" u32 lbl_eu_806654CC;

#endif
