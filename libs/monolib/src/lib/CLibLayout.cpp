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

// Inline copy of CWorkThread::isRunning() visible only in this TU (same trick
// as CLibVM.cpp / CDeviceGX.cpp): the retail isInitialized inlines the member
// call with the this-arg bound to the instance, which births the global load
// before the find-loop index (inst=r6 / index=r7). CWorkRoot.cpp keeps the
// strong out-of-line definition.
inline bool CWorkThread::isRunning() const {
    bool exception;
    if (mFlags & THREAD_FLAG_EXCEPTION) {
        exception = true;
    } else {
        exception = mMsgQueue.find(EVT_EXCEPTION) >= 0;
    }
    bool result = false;
    if (!exception) {
        bool stateOK = mState == THREAD_STATE_LOGIN || mState == THREAD_STATE_RUN;
        if (stateOK) {
            result = true;
        }
    }
    return result;
}
bool CLibLayout::isInitialized() {
    extern CLibLayout* lbl_eu_80665710;
    return lbl_eu_80665710->isRunning();
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
        try {
            // TexMap(palette, id) resets bias-clamp and anisotropy to defaults.
            nw4r::lyt::TexMap texmap(reinterpret_cast<TPLPalette*>(this), 0);
            // Direct ctor call on the raw buffer: placement-new would add a
            // NULL guard the retail function does not have.
            __ct__Q34nw4r3lyt7PictureFRCQ34nw4r3lyt6TexMap(picture, texmap);
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

// Allocator callback. Resolves the buffer either from the hash table's
// registered frame heap or from the default accelerator-allocator handle.
// extern "C" (see header) reproduces the retail synthetic "Fv" symbol.
void* func_8045F438__10CLibLayoutFv(MEMAllocator* allocator, u32 size) {
    CLibLayout* hp = lbl_eu_80665710;
    u32 handle = hp->mAllocHandle;
    if (hp->hashCount != 0) {
        CLibLayoutHashElem* elem =
            hp->hashTable[(hp->hashCount + hp->hashAccum - 1) % hp->hashDivisor];
        // Registering with a frame heap takes priority: allocate there and
        // query the frame heap's free size, returning the allocated buffer.
        if (elem->field_4 != NULL) {
            void* buf = MEMAllocFromFrmHeapEx(elem->field_4, size, 4);
            MEMGetAllocatableSizeForFrmHeapEx(elem->field_4, 4);
            return buf;
        }
        // No frame heap registered: fall through using the element's handle.
        handle = elem->field_0;
    }
    // Align parameter lives in the allocator's heapParam1 field.
    return mtl::MemManager::allocate_head(handle, size, allocator->heapParam1);
}

void func_8045F4E4__10CLibLayoutFv(MEMAllocator* allocator, void* block) {
    // Free callback for the nw4r layout allocator: blocks inside the layout
    // region (or inside a tracked instance's buffer range) go back to the
    // MemManager; anything else is ignored.
    if (lbl_eu_80665710->mRangeStart <= (u32)block &&
        lbl_eu_80665710->mRangeEnd > (u32)block) {
        if (block != NULL) {
            mtl::MemManager::deallocate(block);
        }
        return;
    }

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
DECOMP_FORCEACTIVE(CLibLayout_cpp, lbl_eu_80665710, lbl_eu_80665710_pad, lbl_eu_80665718, lbl_eu_8066571C);
