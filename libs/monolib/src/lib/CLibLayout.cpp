// Auto-scaffolded catalog TU for monolib/src/lib/CLibLayout
// Replace stubs with high-level C++ during decomp.

#include <harness_catalog.h>
#include "libs/monolib/src/lib/CLibLayout.hpp"
#include "monolib/core/CProcRoot.hpp"
#include "monolib/device/CDeviceGX.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include <revolution/mem/mem_allocator.h>
#include <nw4r/lyt/lyt_init.h>

// The retail singleton is CLibLayout's static spInstance (the symbol map
// names it spInstance__10CLibLayout); the following word of the 8-byte slot
// has no identified retail occupant and stays as a visible pad residual.
CLibLayout* CLibLayout::spInstance;
u32 lbl_eu_80665710_pad; // second word of 8-byte retail slot

// [.rodata] layout-region name string: the first of the two compiler-pooled
// literals in this TU (retail lbl_eu_805231B0/lbl_eu_805231BC order).
extern const char lbl_eu_805231BC[0x14];
#include <decomp.h>

CLibLayout::CLibLayout(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, 0) {
    mAllocHandle = -1;
    hashDivisor = 0x10;
    hashTable = reinterpret_cast<CLibLayoutHashElem**>(mHashData);
    hashCount = 0;
    hashAccum = 0;
    mRangeStart = 0;
    mRangeEnd = 0;
    instanceCount = 0;
    CLibLayout::spInstance = this;
    mType = THREAD_CLIBLAYOUT;
}

extern "C" CLibLayout* __dt__8045F000(CLibLayout* self, int flags) {
    if (self != 0) {
        // Retail clears mState (+0x48) then mName.mLength (+0x44).
        self->mState = (CWorkThread::ThreadState)0;
        self->mName.mLength = 0;
        if (flags > 0)
            operator delete(self);
    }
    return self;
}

CLibLayout::~CLibLayout() {
    CLibLayout::spInstance = NULL;
    if (mHashData) {
        hashCount = 0;
        hashAccum = 0;
    }
}

// Returns the handle of the nw4r layout allocation region.
mtl::ALLOC_HANDLE CLibLayout::getAllocHandle(void) {
    return CLibLayout::spInstance->mAllocHandle;
}

// Returns the global CLibLayout singleton instance.
CLibLayout* CLibLayout::getInstance() {
    return CLibLayout::spInstance;
}

bool CLibLayout::isInitialized() {
    // Layout counts as "not initialized" while an exception is pending, or
    // while an EVT_EXCEPTION message is still queued waiting to be processed.
    CLibLayout* inst = CLibLayout::spInstance;
    bool busy;
    if (inst->checkFlag(THREAD_FLAG_EXCEPTION)) {
        busy = true;
    } else {
        // Scan the message queue for a pending EVT_EXCEPTION. Written inline
        // (mirroring CMsgParam::find()) rather than as a member call so the
        // scan index stays a direct local - an inlined callee's index would
        // otherwise win the r6/r7 split and break the retail register layout
        // (retail is a leaf function with no stack frame: inst=r6, index=r7).
        const CMsgParamRing<8>& ring = inst->mMsgQueue.mRing;
        int i;
        int foundIndex;
        for (i = 0; i < ring.mSize; i++) {
            if (ring.mArrayPtr[(ring.mFront + i) % ring.mCapacity].command == EVT_EXCEPTION) {
                foundIndex = i;
                goto done;
            }
        }
        foundIndex = -1;  // not found
    done:
        busy = foundIndex >= 0;
    }
    return !busy
        && (inst->mState == THREAD_STATE_LOGIN
            || inst->mState == THREAD_STATE_RUN);
}

nw4r::lyt::Layout* CLibLayout::createLayout() {
    // Allocate the 0x20-byte Layout buffer, then placement-construct it.
    return new (static_cast<nw4r::lyt::Layout*>(
        mtl::MemManager::allocate(0x20, CLibLayout::spInstance->mAllocHandle)))
        nw4r::lyt::Layout();
}

nw4r::lyt::ArcResourceAccessor* CLibLayout::createArcResourceAccessor() {
    return new (CLibLayout::spInstance->mAllocHandle) nw4r::lyt::ArcResourceAccessor();
}

nw4r::lyt::Picture* CLibLayout::createPicture(void) {
    // Allocate the Picture buffer from the singleton's nw4r layout allocator.
    nw4r::lyt::Picture* picture = static_cast<nw4r::lyt::Picture*>(MEMAllocFromAllocator(
        &CLibLayout::spInstance->mAllocator, 0xF0));

    if (picture != NULL) {
        // Raw storage view of a TexMap: retail never runs a TexMap ctor here,
        // it builds the map in place with Set() (which fills every field).
        u8 texmapStorage[sizeof(nw4r::lyt::TexMap)];
        try {
            // Set(palette, id) resets bias-clamp and anisotropy to defaults.
            reinterpret_cast<nw4r::lyt::TexMap*>(texmapStorage)
                ->Set(reinterpret_cast<TPLPalette*>(this), 0);
            reinterpret_cast<nw4r::lyt::TexMap*>(texmapStorage)->SetBiasClampEnable(false);
            reinterpret_cast<nw4r::lyt::TexMap*>(texmapStorage)->SetAnisotropy(GX_ANISO_1);
            // Direct ctor call on the raw buffer: placement-new would add a
            // NULL guard the retail function does not have.
            __ct__Q34nw4r3lyt7PictureFRCQ34nw4r3lyt6TexMap(
                picture, *reinterpret_cast<const nw4r::lyt::TexMap*>(texmapStorage));
        } catch (...) {
            // Re-throw; the explicit call + noreturn decl keeps the retail
            // catch-all handler shape (no __end__catch epilogue).
            __throw(0, 0, 0);
        }
    }
    return picture;
}

nw4r::lyt::TextBox* CLibLayout::createTextbox() {
    // Allocate the 0x104-byte TextBox buffer from the singleton's nw4r layout
    // allocator, then run the retail TextBox(u16) ctor on it (0 = empty
    // string buffer). The ctor is invoked directly because placement-new would
    // add a NULL guard the retail function does not have.
    nw4r::lyt::TextBox* textbox = static_cast<nw4r::lyt::TextBox*>(MEMAllocFromAllocator(
        &CLibLayout::spInstance->mAllocator, 0x104));

    if (textbox != NULL) {
        try {
            __ct__Q34nw4r3lyt7TextBoxFUs(textbox, 0);
        } catch (...) {
            // Re-throw; the explicit call + noreturn decl keeps the retail
            // catch-all handler shape (no __end__catch epilogue).
            __throw(0, 0, 0);
        }
    }

    return textbox;
}

void CLibLayout::deleteTextboxOrPicture() {
    MEMFreeToAllocator(&CLibLayout::spInstance->mAllocator, this);
}

// No-op override; CLibLayout performs no per-frame work.
void CLibLayout::wkUpdate() {}

bool CLibLayout::wkStandbyLogin() {
    if (CDeviceGX::isInitialized()) {
        // Set up the layout allocation region + its custom nw4r allocator.
        nw4r::lyt::LytInit();
        mtl::ALLOC_HANDLE h = mtl::MemManager::create(
            mtl::MemManager::getHandleMEM2(), 0x3FFE0, lbl_eu_805231BC);
        mAllocHandle = h;
        // Alloc/free callbacks are the extern "C" layoutAllocCallback/layoutFreeCallback,
        // invoked with the allocator pointer and the requested size/block.
        mAllocFuncs.allocFunc = &layoutAllocCallback__10CLibLayoutFv;
        mAllocFuncs.freeFunc = &layoutFreeCallback__10CLibLayoutFv;
        mAllocator.funcs = &mAllocFuncs;
        mAllocator.heap = NULL;
        mAllocator.heapParam1 = 0x20;
        mAllocator.heapParam2 = h;

        u8* pData = static_cast<u8*>(mtl::MemManager::getMaxAllocData(h));
        mRangeStart = reinterpret_cast<u32>(pData);
        mRangeEnd = mRangeStart + 0x3FFE0;
        nw4r::lyt::Layout::SetAllocator(&mAllocator);
        return CWorkThread::wkStandbyLogin();
    }
    return false;
}

bool CLibLayout::wkStandbyLogout() {
    if (mChildren.empty() && CProcRoot::getInstance() == nullptr) {
        mtl::MemManager::erase(mAllocHandle);
        mAllocHandle = -1;
        return CWorkThread::wkStandbyLogout();
    }
    return false;
}

// Allocator callback. Resolves the buffer either from the hash table's
// registered frame heap or from the default accelerator-allocator handle.
// extern "C" (see header) reproduces the retail synthetic "Fv" symbol.
void* layoutAllocCallback__10CLibLayoutFv(MEMAllocator* allocator, u32 size) {
    // Every access is spelled through the global singleton so MWCC CSEs all
    // the global loads into one scratch temp (retail: singleton in scratch).
    // The volatile views stop dominator-CSE merging hashCount's two loads.
    // Retail keeps ONE register live across three different values -- the
    // preloaded mAllocHandle, the hash index, and the element's fallback
    // handle -- so all three share a single local.
    u8* buf;
    CLibLayout* inst = CLibLayout::spInstance;
    u32 val = inst->mAllocHandle;
    if (static_cast<const volatile CLibLayout*>(inst)->hashCount != 0) {
        s32 idx = inst->hashCount;
        idx += inst->hashAccum;
        // Signed % : retail emits subi/divw/mullw/subf (divw is signed).
        idx = (idx - 1) % inst->hashDivisor;
        u8* pBuf;
        CLibLayoutHashElem* elem = inst->hashTable[idx];
        // Registering with a frame heap takes priority: allocate there and
        // query the frame heap's free size, returning the allocated buffer.
        if (elem->field_4 != NULL) {
            pBuf = static_cast<u8*>(
                MEMAllocFromFrmHeapEx(elem->field_4, size, 4));
            MEMGetAllocatableSizeForFrmHeapEx(elem->field_4, 4);
            return pBuf;
        }
        // No frame heap registered: fall through using the element's handle.
        val = elem->field_0;
    }
    // Align parameter lives in the allocator's heapParam1 field.
    return mtl::MemManager::allocate_head(val, size, allocator->heapParam1);
}

void layoutFreeCallback__10CLibLayoutFv(MEMAllocator* allocator, void* block) {
    // Free callback for the nw4r layout allocator: blocks inside the layout
    // region (or inside a tracked instance's buffer range) go back to the
    // MemManager; anything else is ignored.
    // Retail emits exactly two CLibLayout::spInstance loads (range check + loop head)
    // and walks a fused base-increment induction (singleton-copy += 4).
    if (CLibLayout::spInstance->mRangeStart <= (u32)block &&
        CLibLayout::spInstance->mRangeEnd > (u32)block) {
        if (block != NULL) {
            mtl::MemManager::deallocate(block);
        }
        return;
    }

    // NOTE: keep both CLibLayout::spInstance reads INLINE (no anchor local, no
    // volatile view, no pointer cursor). Verified negatives (this + prior
    // sessions): naming either global read CSEs the second load away
    // (.text 0x7c); an anchor local spanning the early-return call forces a
    // callee-saved r5 + prologue frame (+0x24); a volatile word read of
    // instanceCount DOES move the loop-head reload into retail's r5 but
    // degrades ctr/bdnz to an explicit counter (+8 bytes); caching that
    // volatile read in the for-init gets folded back to one temp (colors
    // revert). FULL_MATCH fix: NO element local -- spell instanceArray[i]
    // inline at both uses; CSE merges them into one anonymous temp younger
    // than the walker, yielding retail colors walker->r5 / element->r3.
    for (u32 i = 0; i < CLibLayout::spInstance->instanceCount; i++) {
        if (CLibLayout::spInstance->instanceArray[i]->unk8 <= (u32)block &&
            CLibLayout::spInstance->instanceArray[i]->unkC > (u32)block) {
            if (block != NULL) {
                mtl::MemManager::deallocate(block);
            }
            return;
        }
    }
}

// Dissolved sbss tails
u32 lbl_eu_80665718;
u32 lbl_eu_8066571C;
// NOTE: no DECOMP_FORCEACTIVE here -- the sbss tails survive -ipa file GC
// without a stub (verified: .sbss stays 0x10 and data diff MATCHes), and the
// stub's .text cost would break the exactly-full 0x5E0 split budget.


// name-guess: orphan retail pool strings; unreferenced in the linked game.
const char* CLibLayout_getLayoutRegionName() {
    const char* region = "Layout Mem";
    const char* thread = "LAYOUT";
    return region[0] == 0 ? thread : region;
}
