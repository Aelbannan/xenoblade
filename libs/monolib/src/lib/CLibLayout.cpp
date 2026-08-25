// Auto-scaffolded catalog TU for monolib/src/lib/CLibLayout
// Replace stubs with high-level C++ during decomp.

#include <harness_catalog.h>
#include "libs/monolib/src/lib/CLibLayout.hpp"
#include "monolib/core/CProcRoot.hpp"
#include "monolib/device/CDeviceGX.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include <revolution/mem/mem_allocator.h>
#include <nw4r/lyt/lyt_init.h>

// Global CLibLayout singleton (retail sbss symbol). Plain global decl so MWCC
// emits lbl_eu_80665710 verbatim (C++ does not mangle global namespace vars).
// Dissolved monolibdata2: retail sbss is 0x10 bytes (lbl_eu_80665710 8, lbl_eu_80665718 4, lbl_eu_8066571C 4)
CLibLayout* lbl_eu_80665710;
u32 lbl_eu_80665710_pad; // second word of 8-byte retail slot

// Own-class vtable slots referenced by the manual vtable below. The C++
// mangled names of these members equal these identifiers, so the C-linkage
// aliases resolve to the definitions in this TU (CLibG3d pattern).
extern "C" void __dt__10CLibLayoutFv();
extern "C" void wkUpdate__10CLibLayoutFv();
extern "C" void wkStandbyLogin__10CLibLayoutFv();
extern "C" void wkStandbyLogout__10CLibLayoutFv();
// Inherited IWorkEvent/CWorkThread vtable slots. NOTE: data_vtables.hpp is NOT
// included here -- its __RTTI__* externs trip MWCC 10322 (illegal name
// overloading) in this TU (novtable class + -ipa file); the base-list stand-ins
// below are retargeted by the §17.6 UNIT_RULES instead.
extern "C" int WorkEvent1__10IWorkEventFPvPCc(void*, const char*);
extern "C" int OnFileEvent__10IWorkEventFP10CEventFile(void*);
extern "C" int WorkEvent3__10IWorkEventFPv(void*);
extern "C" int WorkEvent4__10IWorkEventFv();
extern "C" void OnPauseTrigger__10IWorkEventFb(int);
extern "C" int WorkEvent6__10IWorkEventFv();
extern "C" int WorkEvent7__10IWorkEventFv();
extern "C" int WorkEvent8__10IWorkEventFv();
extern "C" int WorkEvent9__10IWorkEventFv();
extern "C" int WorkEvent10__10IWorkEventFv();
extern "C" int WorkEvent11__10IWorkEventFv();
extern "C" int WorkEvent12__10IWorkEventFv();
extern "C" int WorkEvent13__10IWorkEventFv();
extern "C" int WorkEvent14__10IWorkEventFv();
extern "C" int WorkEvent15__10IWorkEventFv();
extern "C" int WorkEvent16__10IWorkEventFv();
extern "C" int WorkEvent17__10IWorkEventFv();
extern "C" int WorkEvent18__10IWorkEventFv();
extern "C" int WorkEvent19__10IWorkEventFv();
extern "C" int WorkEvent20__10IWorkEventFv();
extern "C" int WorkEvent21__10IWorkEventFv();
extern "C" int WorkEvent22__10IWorkEventFv();
extern "C" int WorkEvent23__10IWorkEventFv();
extern "C" int WorkEvent24__10IWorkEventFv();
extern "C" int WorkEvent25__10IWorkEventFv();
extern "C" int WorkEvent26__10IWorkEventFv();
extern "C" int WorkEvent27__10IWorkEventFv();
extern "C" int WorkEvent28__10IWorkEventFv();
extern "C" int WorkEvent29__10IWorkEventFv();
extern "C" int WorkEvent30__10IWorkEventFv();
extern "C" int WorkEvent31__10IWorkEventFv();
extern "C" void wkRender__11CWorkThreadFv();
extern "C" void wkRenderAfter__11CWorkThreadFv();
extern "C" void wkStandbyExceptionRetry__11CWorkThreadFUl(unsigned int);
// [.data] retail vtable label (defined at the bottom of this TU).
extern "C" u32 lbl_eu_8056D350[40];
// [.rodata] layout-region name string (defined at the bottom of this TU,
// right after lbl_eu_805231B0 -- retail .rodata byte order).
extern const char lbl_eu_805231BC[0x14];
#include <decomp.h>

CLibLayout::CLibLayout(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, 0) {
    // novtable class: install the retail vtable by hand (retail stores it
    // right after the base ctor call).
    *(void**)this = (void*)lbl_eu_8056D350;
    mAllocHandle = -1;
    hashDivisor = 0x10;
    hashTable = reinterpret_cast<CLibLayoutHashElem**>(mHashData);
    hashCount = 0;
    hashAccum = 0;
    mRangeStart = 0;
    mRangeEnd = 0;
    instanceCount = 0;
    lbl_eu_80665710 = this;
    mType = THREAD_CLIBLAYOUT;
}

extern "C" void* __dt__8045F000(void* self, int flags) {
    if (self != 0) {
        *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(self) + 0x48) = 0;
        *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(self) + 0x44) = 0;
        if (flags > 0)
            operator delete(self);
    }
    return self;
}

CLibLayout::~CLibLayout() {
    lbl_eu_80665710 = NULL;
    if (mHashData) {
        hashCount = 0;
        hashAccum = 0;
    }
}

// Returns the handle of the nw4r layout allocation region.
mtl::ALLOC_HANDLE CLibLayout::getAllocHandle(void) {
    return lbl_eu_80665710->mAllocHandle;
}

// Returns the global CLibLayout singleton instance.
CLibLayout* CLibLayout::getInstance() {
    return lbl_eu_80665710;
}

// Mirror of the tail layout of CWorkThread's mMsgQueue member (mMsgQueue
// internals at 0x1A4-0x1B0, per retail isInitialized: arrayPtr/front/size/
// capacity at +0x1A4/+0x1A8/+0x1AC/+0x1B0) so the inline EVT_EXCEPTION scan
// below reads the ring buffer directly (CLibG3d pattern).
struct CMsgQueueData {
    u8 pad[0x1A4];               // CWorkThread prefix + vtable + mEntries[8]
    CMsgParamEntry* mArrayPtr;   // 0x1A4 (mMsgQueue.mArrayPtr)
    u32 mFront;                  // 0x1A8 (mMsgQueue.mFront)
    u32 mSize;                   // 0x1AC (mMsgQueue.mSize)
    u32 mCapacity;               // 0x1B0 (mMsgQueue.mCapacity)
};

bool CLibLayout::isInitialized() {
    // Layout counts as "not initialized" while an exception is pending, or
    // while an EVT_EXCEPTION message is still queued waiting to be processed.
    CLibLayout* inst = lbl_eu_80665710;
    bool busy;
    if (inst->checkFlag(THREAD_FLAG_EXCEPTION)) {
        busy = true;
    } else {
        // Scan the message queue for a pending EVT_EXCEPTION. Written inline
        // (mirroring CMsgParam::find()) rather than as a member call so the
        // scan index stays a direct local - an inlined callee's index would
        // otherwise win the r6/r7 split and break the retail register layout
        // (retail is a leaf function with no stack frame: inst=r6, index=r7).
        CMsgQueueData* q = reinterpret_cast<CMsgQueueData*>(inst);
        int i;
        int foundIndex;
        for (i = 0; i < q->mSize; i++) {
            if (q->mArrayPtr[(q->mFront + i) % q->mCapacity].command == EVT_EXCEPTION) {
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
        mtl::MemManager::allocate(0x20, lbl_eu_80665710->mAllocHandle)))
        nw4r::lyt::Layout();
}

nw4r::lyt::ArcResourceAccessor* CLibLayout::createArcResourceAccessor() {
    return new (lbl_eu_80665710->mAllocHandle) nw4r::lyt::ArcResourceAccessor();
}

nw4r::lyt::Picture* CLibLayout::createPicture(void) {
    // Allocate the Picture buffer from the singleton's nw4r layout allocator.
    nw4r::lyt::Picture* picture = static_cast<nw4r::lyt::Picture*>(MEMAllocFromAllocator(
        &lbl_eu_80665710->mAllocator, 0xF0));

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
        &lbl_eu_80665710->mAllocator, 0x104));

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
    MEMFreeToAllocator(&lbl_eu_80665710->mAllocator, this);
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
        // Alloc/free callbacks are the extern "C" func_8045F438/func_8045F4E4,
        // invoked with the allocator pointer and the requested size/block.
        mAllocFuncs.allocFunc = &func_8045F438__10CLibLayoutFv;
        mAllocFuncs.freeFunc = &func_8045F4E4__10CLibLayoutFv;
        mAllocator.funcs = &mAllocFuncs;
        mAllocator.heap = NULL;
        mAllocator.heapParam1 = 0x20;
        mAllocator.heapParam2 = h;

        u8* pData = static_cast<u8*>(mtl::MemManager::getMaxAllocData(h));
        mRangeStart = reinterpret_cast<u32>(pData);
        mRangeEnd = mRangeStart + 0x3FFE0;
        lbl_eu_80665478 = &mAllocator;
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

// Volatile word view of a CLibLayout singleton: blocks CSE so retail's
// double hashCount load and per-access reloads survive.
#define CLIBLAYOUT_WORD(inst, i) \
    (*static_cast<const volatile u32*>(static_cast<const volatile void*>( \
        static_cast<const volatile u8*>(static_cast<const volatile void*>(inst)) \
            + (i) * 4)))

// Allocator callback. Resolves the buffer either from the hash table's
// registered frame heap or from the default accelerator-allocator handle.
// extern "C" (see header) reproduces the retail synthetic "Fv" symbol.
void* func_8045F438__10CLibLayoutFv(MEMAllocator* allocator, u32 size) {
    // Every access is spelled through the global singleton so MWCC CSEs all
    // the global loads into one anonymous scratch temp (retail keeps the
    // singleton in a scratch reg, not a named local). The volatile views stop
    // dominator-CSE from merging hashCount's two loads (branch test + body).
    // Retail keeps ONE register live across three different values -- the
    // preloaded mAllocHandle, the hash index, and the element's fallback
    // handle -- so all three share a single local.
    void* buf;
    CLibLayout* inst = lbl_eu_80665710;
    u32 val = inst->mAllocHandle;
    if (static_cast<const volatile CLibLayout*>(inst)->hashCount != 0) {
        s32 idx = static_cast<const volatile CLibLayout*>(inst)->hashAccum;
        idx += static_cast<const volatile CLibLayout*>(inst)->hashCount;
        // Signed % : retail emits subi/divw/mullw/subf (divw is signed).
        idx = (idx - 1) % inst->hashDivisor;
        CLibLayoutHashElem* elem = inst->hashTable[idx];
        // Registering with a frame heap takes priority: allocate there and
        // query the frame heap's free size, returning the allocated buffer.
        if (elem->field_4 != NULL) {
            void* pBuf = MEMAllocFromFrmHeapEx(elem->field_4, size, 4);
            MEMGetAllocatableSizeForFrmHeapEx(elem->field_4, 4);
            return pBuf;
        }
        // No frame heap registered: fall through using the element's handle.
        val = elem->field_0;
    }
    // Align parameter lives in the allocator's heapParam1 field.
    return mtl::MemManager::allocate_head(val, size, allocator->heapParam1);
}
#undef CLIBLAYOUT_WORD

void func_8045F4E4__10CLibLayoutFv(MEMAllocator* allocator, void* block) {
    // Free callback for the nw4r layout allocator: blocks inside the layout
    // region (or inside a tracked instance's buffer range) go back to the
    // MemManager; anything else is ignored.
    // Retail emits exactly two lbl_eu_80665710 loads (range check + loop head)
    // and walks a fused base-increment induction (singleton-copy += 4).
    if (lbl_eu_80665710->mRangeStart <= (u32)block &&
        lbl_eu_80665710->mRangeEnd > (u32)block) {
        if (block != NULL) {
            mtl::MemManager::deallocate(block);
        }
        return;
    }

    // NOTE: keep both lbl_eu_80665710 reads INLINE (no anchor local, no
    // volatile view, no pointer cursor). Verified negatives (this + prior
    // sessions): naming either global read CSEs the second load away
    // (.text 0x7c); an anchor local spanning the early-return call forces a
    // callee-saved r5 + prologue frame (+0x24); a volatile word read of
    // instanceCount DOES move the loop-head reload into retail's r5 but
    // degrades ctr/bdnz to an explicit counter (+8 bytes); caching that
    // volatile read in the for-init gets folded back to one temp (colors
    // revert). Residual vs retail is purely the {singleton-walker, element}
    // register colors (r5,r3) vs (r3,r5) -- see attempts.jsonl open item;
    // recorded sole remaining lever is a unit mw_version sweep (configure.py).
    for (u32 i = 0; i < lbl_eu_80665710->instanceCount; i++) {
        UnkClass_8045F564* inst = lbl_eu_80665710->instanceArray[i];
        if (inst->unk8 <= (u32)block && inst->unkC > (u32)block) {
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

// ===== Dissolved monolibdata2 data owned by this TU =====
// Forward decls (definition order follows the retail address order).
extern "C" u32 lbl_eu_806637D8[2];           // [.sdata] RTTI locator
extern "C" const char lbl_eu_805231B0[0xC]; // [.rodata] "CLibLayout" name
// NOTE: the base typeinfo objects (__RTTI__10IWorkEvent / __RTTI__11CWorkThread)
// cannot be spelled here: declaring an __RTTI__* name in a TU with a
// novtable-predeclared class trips an MWCC -ipa file ICE ("illegal name
// overloading"). The base list below references legal stand-in spellings;
// the §17.6 UNIT_RULES retarget those two slots to the retail typeinfos.
extern u32 rtti_10IWorkEvent[];
extern u32 rtti_11CWorkThread[];
// [.data] 0x8056D350-0x8056D3F0 (0xA0): __vt__10CLibLayout. Base class
// CWorkThread is novtable in retail, so the vtable/RTTI are all manual.
extern "C" u32 lbl_eu_8056D350[40] = {
    (u32)&lbl_eu_806637D8, 0x00000000, (u32)&__dt__10CLibLayoutFv,
    (u32)&WorkEvent1__10IWorkEventFPvPCc, (u32)&OnFileEvent__10IWorkEventFP10CEventFile,
    (u32)&WorkEvent3__10IWorkEventFPv, (u32)&WorkEvent4__10IWorkEventFv,
    (u32)&OnPauseTrigger__10IWorkEventFb,
    (u32)&WorkEvent6__10IWorkEventFv, (u32)&WorkEvent7__10IWorkEventFv,
    (u32)&WorkEvent8__10IWorkEventFv, (u32)&WorkEvent9__10IWorkEventFv,
    (u32)&WorkEvent10__10IWorkEventFv, (u32)&WorkEvent11__10IWorkEventFv,
    (u32)&WorkEvent12__10IWorkEventFv, (u32)&WorkEvent13__10IWorkEventFv,
    (u32)&WorkEvent14__10IWorkEventFv, (u32)&WorkEvent15__10IWorkEventFv,
    (u32)&WorkEvent16__10IWorkEventFv, (u32)&WorkEvent17__10IWorkEventFv,
    (u32)&WorkEvent18__10IWorkEventFv, (u32)&WorkEvent19__10IWorkEventFv,
    (u32)&WorkEvent20__10IWorkEventFv, (u32)&WorkEvent21__10IWorkEventFv,
    (u32)&WorkEvent22__10IWorkEventFv, (u32)&WorkEvent23__10IWorkEventFv,
    (u32)&WorkEvent24__10IWorkEventFv, (u32)&WorkEvent25__10IWorkEventFv,
    (u32)&WorkEvent26__10IWorkEventFv, (u32)&WorkEvent27__10IWorkEventFv,
    (u32)&WorkEvent28__10IWorkEventFv, (u32)&WorkEvent29__10IWorkEventFv,
    (u32)&WorkEvent30__10IWorkEventFv, (u32)&WorkEvent31__10IWorkEventFv,
    (u32)&wkUpdate__10CLibLayoutFv,
    (u32)&wkRender__11CWorkThreadFv, (u32)&wkRenderAfter__11CWorkThreadFv,
    (u32)&wkStandbyLogin__10CLibLayoutFv, (u32)&wkStandbyLogout__10CLibLayoutFv,
    (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};
// [.data] 0x8056D3F0-0x8056D408 (0x18): RTTI base list [IWorkEvent,0,
// CWorkThread,0, 0,0].
extern "C" u32 lbl_eu_8056D3F0[6] = {
    (u32)&rtti_10IWorkEvent, 0x00000000, (u32)&rtti_11CWorkThread,
    0x00000000, 0x00000000, 0x00000000,
};
// [.sdata] 0x806637D8-0x806637E0 (0x8): __RTTI__10CLibLayout locator.
extern "C" u32 lbl_eu_806637D8[2] = { (u32)&lbl_eu_805231B0, (u32)&lbl_eu_8056D3F0 };

// [.rodata] 0x805231B0-0x805231BC (0xC): "CLibLayout" RTTI name string + pad.
// NOTE: no __declspec(align(8)) here -- retail emits this 0xB-byte string
// first at +0x0 followed directly by lbl_eu_805231BC at +0xC; an 8-byte
// alignment makes MWCC re-order/re-pad .rodata (decomp grew to 0x24).
extern "C" const char lbl_eu_805231B0[0xC] = "CLibLayout";
// [.rodata] 0x805231BC-0x805231D0 (0x14): layout region name + thread name;
// packs tight against lbl_eu_805231B0 (retail has no pad at +0xC).
extern const char lbl_eu_805231BC[0x14] = "Layout Mem\0LAYOUT\0";
