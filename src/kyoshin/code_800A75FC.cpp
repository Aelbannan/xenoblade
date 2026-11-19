// Auto-scaffolded catalog TU for kyoshin/code_800A75FC
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/code_800A75FC.hpp"
#include "monolib/util/MemManager.hpp"

// Forward declarations for external functions in other TUs.
struct GlobalStruct_80572B94;

// Arts-list helpers defined in cf/CtrlObjectParam.cpp. The TU's own header
// (include/kyoshin/code_800A75FC.hpp) already declares the rest of the family;
// these two are missing there. The allocator takes retail's third register
// (r5 = 0 at every retail call site, verified in kyoshin/code_800A75FC asm)
// which the callee never reads.
extern "C" void* CtrlObjectParam_AllocArtsListNode(void* list, u32 value, u32 flag);
// Word values (index/value) travel as void* at this TU's call sites, which is
// ABI-identical to the owner's u32 parameters (all four go in registers).
extern "C" int CtrlObjectParam_WriteArtsListEntry(void* list, void* target, void* index, void* value);

// View of retail blocks that store a size word at +0xC.
struct SizeWordAtC {
    u32 start; // 0x0
    u8 pad04[0x8];
    u32 size;  // 0xC
};



// Retail linker labels referenced from this TU are declared in
// include/kyoshin/code_800A75FC.hpp.

void* memset(void*, int, unsigned long);

// --- Global singleton struct at sKyoshinHeapState (0xB4 bytes, BSS) ---
struct GlobalStruct_80572B94 {
    int field_0x00;   // -1
    int field_0x04;   // -1
    int field_0x08;   // -1
    u8 _0C[4];
    int field_0x10;   // 0
    int field_0x14;   // 0
    int field_0x18;   // 0
    int field_0x1C;   // 0
    int field_0x20;   // 0
    int field_0x24;
    int field_0x28;
    int field_0x2C;
    int field_0x30;
    int field_0x34;   // 0
    int field_0x38;   // 0
    int field_0x3C;   // 0
    int field_0x40;   // 0 (BSS)
    int field_0x44;   // 0 (BSS)
    u8 _48[0x4C - 0x48];
    int field_0x4C;
    int field_0x50;   // 0
    int field_0x54;   // 0x54
    int field_0x58;   // 0x58-0x5C
    u32 field_0x5C[0xD];      // 0x5C-0x90 base table (13 words)
    u32 field_0x90;   // 0x90
    int field_0x94;
    int field_0x98;
    int field_0x9C;
    int field_0xA0;    // 0xA0
    int field_0xA4;    // 0xA4
    int field_0xA8;    // 0xA8
    int field_0xAC;    // 0xAC
    int field_0xB0;   // 0
};

// Global singleton object at sKyoshinHeapState
extern GlobalStruct_80572B94 sKyoshinHeapState;

extern "C" GlobalStruct_80572B94* __dt__800A75FC(GlobalStruct_80572B94* ths, int flag) {
    if (ths != 0) {
        // Release the three allocations handles, then reset the object.
        if ((u32)ths->field_0x00 != 0xFFFFFFFF) {
            mtl::MemManager::erase((u32)ths->field_0x00);
            ths->field_0x00 = -1;
        }
        if ((u32)ths->field_0x04 != 0xFFFFFFFF) {
            mtl::MemManager::erase((u32)ths->field_0x04);
            ths->field_0x04 = -1;
        }
        if ((u32)ths->field_0x08 != 0xFFFFFFFF) {
            mtl::MemManager::erase((u32)ths->field_0x08);
            ths->field_0x08 = -1;
        }
        ths->field_0x10 = 0;
        ths->field_0x20 = 0;
        ths->field_0x14 = 0;
        ths->field_0x18 = 0;
        ths->field_0x1C = 0;
        ths->field_0x34 = 0;
        ths->field_0xB0 = 0;
        ths->field_0x38 = 0;
        ths->field_0x3C = 0;
        ths->field_0x50 = 0;
        memset(&ths->field_0xA0, 0, 0xC);
        memset(ths->field_0x5C, 0, 0x34);
        if (flag > 0) {
            delete ths;
        }
    }
    return ths;
}

extern "C" void KyoshinHeap_CreateRegions(GlobalStruct_80572B94* g) {
    u8* blk = (u8*)sHeapRegionBlocks;
    // NOTE: keep non-const: const materializes a dead WEAK @LOCAL@..@zero
    // word in .sdata2 (retail has none); all uses already fold to li.
    u32 zero = 0;

    // --- memory debug statistics (retail keeps these as unused locals) ---
    double memStat[2];
    ((u32*)memStat)[0] = 0x43300000;
    ((u32*)memStat)[2] = 0x43300000;
    mtl::MemManager::getPercentAlloc(mtl::MemManager::getHandleMEM1());
    ((u32*)memStat)[1] = mtl::MemManager::getBlockSize(mtl::MemManager::getHandleMEM1()) >> 20;
    u32 rsize1 = mtl::MemManager::getRegionSize(mtl::MemManager::getHandleMEM1());
    ((u32*)memStat)[3] = (rsize1 - mtl::MemManager::getBlockSize(mtl::MemManager::getHandleMEM1())) >> 20;
    mtl::MemManager::getPercentAlloc(mtl::MemManager::getHandleMEM2());
    ((u32*)memStat)[1] = mtl::MemManager::getBlockSize(mtl::MemManager::getHandleMEM2()) >> 20;
    u32 rsize2 = mtl::MemManager::getRegionSize(mtl::MemManager::getHandleMEM2());
    ((u32*)memStat)[3] = (rsize2 - mtl::MemManager::getBlockSize(mtl::MemManager::getHandleMEM2())) >> 20;

    // --- Create the three main handle allocations if not present ---
    if ((u32)g->field_0x00 == 0xFFFFFFFF) {
        u32 h = mtl::MemManager::create(
            mtl::MemManager::getHandleMEM1(), 0x003B1E00, &lbl_eu_804FBF60[0]);
        g->field_0x00 = (int)h;
        u32 object = (u32)mtl::MemManager::getMaxAllocData(h);
        u8* dst = blk + 0x10;
        u32 objectAligned = object + 0x100;
        if (objectAligned & 0x1F) objectAligned = (objectAligned + 0x20) - (objectAligned & 0x1F);
        CtrlObjectParam_InitArtsListEntry(dst, objectAligned, 0x003B1C00, 0);
    }

    if ((u32)g->field_0x04 == 0xFFFFFFFF) {
        u32 h = mtl::MemManager::create(
            mtl::MemManager::getHandleMEM2(), 0x2872200, &lbl_eu_804FBF60[8]);
        g->field_0x04 = (int)h;
        u32 object = (u32)mtl::MemManager::getMaxAllocData(h);
        u8* dst = blk + 0x30;
        u32 objectAligned = object + 0x100;
        if (objectAligned & 0x1F) objectAligned = (objectAligned + 0x20) - (objectAligned & 0x1F);
        CtrlObjectParam_InitArtsListEntry(dst, objectAligned, 0x2872000, 1);
    }

    if ((u32)g->field_0x08 == 0xFFFFFFFF) {
        u32 h = mtl::MemManager::create(
            mtl::MemManager::getHandleMEM2(), 0x48B00, &lbl_eu_804FBF60[0x10]);
        g->field_0x08 = (int)h;
        u32 object = (u32)mtl::MemManager::getMaxAllocData(h);
        u8* dst = blk + 0x50;
        u32 objectAligned = object + 0x100;
        if (objectAligned & 0x1F) objectAligned = (objectAligned + 0x20) - (objectAligned & 0x1F);
        CtrlObjectParam_InitArtsListEntry(dst, objectAligned, 0x48900, 1);
    }

    // --- Sub-allocations from the three regions ---
    g->field_0x20 = (int)CtrlObjectParam_AllocArtsListNode(blk + 0x10, 0x300000, 0);
    g->field_0xA0 = (int)CtrlObjectParam_AllocArtsListNode(blk + 0x10, 0x00051000, 0);
    g->field_0xA4 = (int)CtrlObjectParam_AllocArtsListNode(blk + 0x10, 0x46800, 0);
    g->field_0xA8 = (int)CtrlObjectParam_AllocArtsListNode(blk + 0x10, 0x10000, 0);
    g->field_0x1C = (int)CtrlObjectParam_AllocArtsListNode(blk + 0x10, 0xA000, 0);

    g->field_0xAC = (int)CtrlObjectParam_AllocArtsListNode(blk + 0x30, 0xF4000, 0);
    g->field_0x2C = (int)CtrlObjectParam_AllocArtsListNode(blk + 0x30, 0xA90000, 0);
    g->field_0x24 = (int)CtrlObjectParam_AllocArtsListNode(blk + 0x30, 0xD00800, 0);
    g->field_0x38 = (int)CtrlObjectParam_AllocArtsListNode(blk + 0x30, 0x106000, 0);
    g->field_0x3C = (int)CtrlObjectParam_AllocArtsListNode(blk + 0x30, 0x1A0000, 0);
    g->field_0x50 = (int)CtrlObjectParam_AllocArtsListNode(blk + 0x30, 0xF800, 0);
    g->field_0x4C = (int)CtrlObjectParam_AllocArtsListNode(blk + 0x30, 0x40000, 0);
    g->field_0x9C = (int)CtrlObjectParam_AllocArtsListNode(blk + 0x30, 0x5F8800, 0);
    g->field_0x28 = (int)CtrlObjectParam_AllocArtsListNode(blk + 0x30, 0x718800, 0);

    g->field_0x30 = zero;
    g->field_0x40 = (int)CtrlObjectParam_AllocArtsListNode(blk + 0x50, 0x20000, 0);
    g->field_0x44 = (int)CtrlObjectParam_AllocArtsListNode(blk + 0x50, 0x28000, 0);

    // base addresses derived from field_0x2C
    u32 base = g->field_0x2C;
    g->field_0x5C[0] = base;
    g->field_0x5C[1] = base + 0xA6000;
    g->field_0x5C[2] = base + 0x14C000;
    g->field_0x5C[3] = base + 0x1F2000;
    g->field_0x5C[4] = base + 0x298000;
    g->field_0x5C[5] = base + 0x33E000;
    g->field_0x5C[6] = base + 0x3E4000;
    g->field_0x90 = base + 0x48A000;
    g->field_0x94 = base + 0x79D800;
    g->field_0x98 = base + 0x8C5000;

    // --- Ensure the global singleton is constructed (repeated defensive guards) ---
    GlobalStruct_80572B94* gbl = &sKyoshinHeapState;
    if (sKyoshinHeapInitialized == 0) {
        gbl->field_0x00 = -1;
        gbl->field_0x04 = -1;
        gbl->field_0x08 = -1;
        gbl->field_0x10 = 0;
        gbl->field_0x20 = 0;
        gbl->field_0x14 = 0;
        gbl->field_0x18 = 0;
        gbl->field_0x1C = 0;
        gbl->field_0x34 = 0;
        gbl->field_0xB0 = 0;
        gbl->field_0x38 = 0;
        gbl->field_0x3C = 0;
        gbl->field_0x50 = 0;
        memset(&gbl->field_0xA0, 0, 0xC);
        memset(gbl->field_0x5C, 0, 0x34);
        __register_global_object(gbl, (void*)__dt__800A75FC, (void*)(blk + 0xc0));
        sKyoshinHeapInitialized = 1;
    }

    CtrlObjectParam_InitArtsListEntry(blk + 0x70, gbl->field_0x9C, 0x5F8800, 3);

    if (sKyoshinHeapInitialized == 0) {
        gbl->field_0x00 = -1;
        gbl->field_0x04 = -1;
        gbl->field_0x08 = -1;
        gbl->field_0x10 = 0;
        gbl->field_0x20 = 0;
        gbl->field_0x14 = 0;
        gbl->field_0x18 = 0;
        gbl->field_0x1C = 0;
        gbl->field_0x34 = 0;
        gbl->field_0xB0 = 0;
        gbl->field_0x38 = 0;
        gbl->field_0x3C = 0;
        gbl->field_0x50 = 0;
        memset(&gbl->field_0xA0, 0, 0xC);
        memset(gbl->field_0x5C, 0, 0x34);
        __register_global_object(gbl, (void*)__dt__800A75FC, (void*)(blk + 0xc0));
        sKyoshinHeapInitialized = 1;
    }

    CtrlObjectParam_InitArtsListEntry(blk + 0xb0, gbl->field_0x28, 0x718800, 4);
    gbl->field_0x54 = 0;
    gbl->field_0x58 = 0;

    if (sKyoshinHeapInitialized == 0) {
        gbl->field_0x00 = -1;
        gbl->field_0x04 = -1;
        gbl->field_0x08 = -1;
        gbl->field_0x10 = 0;
        gbl->field_0x20 = 0;
        gbl->field_0x14 = 0;
        gbl->field_0x18 = 0;
        gbl->field_0x1C = 0;
        gbl->field_0x34 = 0;
        gbl->field_0xB0 = 0;
        gbl->field_0x38 = 0;
        gbl->field_0x3C = 0;
        gbl->field_0x50 = 0;
        memset(&gbl->field_0xA0, 0, 0xC);
        memset(gbl->field_0x5C, 0, 0x34);
        __register_global_object(gbl, (void*)__dt__800A75FC, (void*)(blk + 0xc0));
        sKyoshinHeapInitialized = 1;
    }

    CtrlObjectParam_InitArtsListEntry(blk + 0x90, gbl->field_0x24, 0xD00800, 5);
    gbl->field_0x10 = zero;
    gbl->field_0x14 = zero;
    gbl->field_0x18 = zero;
    gbl->field_0x34 = zero;
    gbl->field_0xB0 = zero;

    u32 alloc = (int)CtrlObjectParam_AllocArtsListNode(blk + 0xb0, 0x700000, 0);
    g->field_0x54 = alloc;
    CtrlObjectParam_WriteArtsListEntry(blk + 0xb0, (void*)alloc, (void*)2, (void*)0x11111111);
    g->field_0x58 = zero;
}

extern "C" void KyoshinHeap_Init() {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(&g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    KyoshinHeap_CreateRegions(&sKyoshinHeapState);
}

extern "C" void KyoshinHeap_Shutdown() {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    GlobalStruct_80572B94* g = &sKyoshinHeapState;
    // Release the three region allocations, then reset the object state.
    if ((u32)g->field_0x00 != 0xFFFFFFFF) {
        mtl::MemManager::erase((u32)g->field_0x00);
        g->field_0x00 = -1;
    }
    if ((u32)g->field_0x04 != 0xFFFFFFFF) {
        mtl::MemManager::erase((u32)g->field_0x04);
        g->field_0x04 = -1;
    }
    if ((u32)g->field_0x08 != 0xFFFFFFFF) {
        mtl::MemManager::erase((u32)g->field_0x08);
        g->field_0x08 = -1;
    }
    g->field_0x10 = 0;
    g->field_0x20 = 0;
    g->field_0x14 = 0;
    g->field_0x18 = 0;
    g->field_0x1C = 0;
    g->field_0x34 = 0;
    g->field_0xB0 = 0;
    g->field_0x38 = 0;
    g->field_0x3C = 0;
    g->field_0x50 = 0;
    memset(&g->field_0xA0, 0, 0xC);
    memset(g->field_0x5C, 0, 0x34);
}

extern "C" int KyoshinHeap_GetField10() {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(&g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    return sKyoshinHeapState.field_0x10;
}

extern "C" int KyoshinHeap_GetField18() {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    return sKyoshinHeapState.field_0x18;
}

extern "C" int KyoshinHeap_GetField34() {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    return sKyoshinHeapState.field_0x34;
}

extern "C" int KyoshinHeap_GetField20() {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    return sKyoshinHeapState.field_0x20;
}

extern "C" int KyoshinHeap_GetField1C() {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    return sKyoshinHeapState.field_0x1C;
}

extern "C" int KyoshinHeap_GetFieldA0() {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    return sKyoshinHeapState.field_0xA0;
}

extern "C" int KyoshinHeap_GetFieldA4() {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    return sKyoshinHeapState.field_0xA4;
}

extern "C" int KyoshinHeap_GetFieldA8() {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    return sKyoshinHeapState.field_0xA8;
}

extern "C" u32 KyoshinHeap_GetBaseTblPlus96k(int index) {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    return sKyoshinHeapState.field_0x5C[index] + 0x96000;
}

extern "C" u32 KyoshinHeap_GetBaseTblRaw(int index) {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    return sKyoshinHeapState.field_0x5C[index];
}

extern "C" void* KyoshinHeap_GetPtrTblA(unsigned int param1, unsigned int param2) {
    LblPtrTable* table = lbl_eu_805282A0[param2];
    if (param1 >= 0xe) {
        return table->slots[0];
    }
    return table->slots[param1];
}

extern "C" void* KyoshinHeap_GetPtrTblB(unsigned int param1, unsigned int param2) {
    LblPtrTable* table = lbl_eu_80528398[param2];
    if (param1 >= 0xe) {
        return table->slots[0];
    }
    return table->slots[param1];
}

extern "C" u32 KyoshinHeap_GetBlk90Raw(int index) {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    return sKyoshinHeapState.field_0x90 + index * 0x106800;
}

extern "C" u32 KyoshinHeap_GetBlk90Plus32k(int index) {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    u32 v = sKyoshinHeapState.field_0x90 + index * 0x106800;
    if (v != 0) {
        v += 0x32000;
    }
    return v;
}

extern "C" u32 KyoshinHeap_GetBlk90PlusFA(int index) {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    u32 v = sKyoshinHeapState.field_0x90 + index * 0x106800;
    if (v != 0) {
        v += 0x32000;
    }
    if (v != 0) {
        v += 0xC8000;
    }
    return v;
}

extern "C" u32 KyoshinHeap_GetBlk94(int index) {
    if (index >= 3) return 0;
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    return sKyoshinHeapState.field_0x94 + index * 0x62800;
}

extern "C" u32 KyoshinHeap_GetBlk98(int index) {
    if (index >= 3) return 0;
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    return sKyoshinHeapState.field_0x98 + index * 0x99000;
}

extern "C" int KyoshinHeap_Alloc38(int param) {
    if (param == 0) return 0;
    if (param > 0) {
        return (int)CtrlObjectParam_AllocArtsListNode((void*)sHeapList38, param, 0);
    } else {
        return func_800A36A4((void*)sHeapList38, -param, 0);
    }
}

extern "C" int KyoshinHeap_Contains38(u32 param) {
    if (param == 0) return 0;
    u32 start = ((SizeWordAtC*)sHeapList38)->start;
    u32 size = ((SizeWordAtC*)sHeapList38)->size;
    u32 end = start + size;
    int result = 0;
    if (start <= param && param < end) {
        result = 1;
    }
    return result;
}

extern "C" int KyoshinHeap_Register38(void* p1, void* p2, void* p3) {
    int result = 0;
    if (p1 != 0) {
        CtrlObjectParam_WriteArtsListEntry((void*)sHeapList38, p1, p2, p3);
        result = 1;
    }
    return result;
}

extern "C" void KyoshinHeap_Free38(void* p) {
    if (p == 0) return;
    CtrlObjectParam_MergeArtsListNode((void*)sHeapList38, p);
}

extern "C" void KyoshinHeap_Reset38(void) {
    CtrlObjectParam_GetArtsListMaxSize(reinterpret_cast<cf::CtrlObjectParamArtsList*>(sHeapList38));
}

extern "C" u32 KyoshinHeap_GetCount38() {
    return CtrlObjectParam_GetArtsListTotalSize((void*)sHeapList38) / 774144;
}

extern "C" int KyoshinHeap_GetActive54() {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    return sKyoshinHeapState.field_0x54 != 0 ? sKyoshinHeapState.field_0x54
                                           : sKyoshinHeapState.field_0x58;
}

extern "C" u32 KyoshinHeap_GetSize500000(){ return 0x500000; }

extern "C" int KyoshinHeap_GetField58() {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    return sKyoshinHeapState.field_0x58;
}

extern "C" int KyoshinHeap_AllocNode(int size, int arg2) {
    if (size == 0) return 0;
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    GlobalStruct_80572B94* g = &sKyoshinHeapState;
    if (g->field_0x54 != 0) {
        if (g->field_0x54 != 0) {
            CtrlObjectParam_MergeArtsListNode((void*)sHeapList78, (void*)g->field_0x54);
        }
        g->field_0x54 = 0;
    }
    if (g->field_0x58 == 0) {
        int h = (int)CtrlObjectParam_AllocArtsListNode((void*)sHeapList78, 0x500000, 0);
        g->field_0x58 = h;
        if (h != 0) {
            CtrlObjectParam_WriteArtsListEntry((void*)sHeapList78, (void*)h, (void*)2, (void*)0x99999999);
        }
    }
    int r;
    if (size > 0) {
        r = (int)CtrlObjectParam_AllocArtsListNode((void*)sHeapList78, size, arg2);
    } else {
        r = func_800A36A4((void*)sHeapList78, -size, arg2);
    }
    if (arg2 != 0) {
        if (r != 0) {
            if (r != 0) {
                CtrlObjectParam_MergeArtsListNode((void*)sHeapList78, (void*)r);
            }
        }
    }
    return r;
}

extern "C" int KyoshinHeap_Contains78(u32 param) {
    if (param == 0) return 0;
    u32 start = ((SizeWordAtC*)sHeapList78)->start;
    u32 size = ((SizeWordAtC*)sHeapList78)->size;
    u32 end = start + size;
    int result = 0;
    if (start <= param && param < end) {
        result = 1;
    }
    return result;
}

extern "C" int KyoshinHeap_HasActive54() {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    return sKyoshinHeapState.field_0x54 != 0;
}

extern "C" int KyoshinHeap_GetField54() {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    GlobalStruct_80572B94* g = &sKyoshinHeapState;
    // Retail keeps field_0x54 cached and returns it directly if already set.
    if (g->field_0x54 != 0) {
        return g->field_0x54;
    }
    // Second lazy-init guard (retail duplicates the guard inside this function).
    if (sKyoshinHeapInitialized == 0) {
        GlobalStruct_80572B94* g2 = &sKyoshinHeapState;
        g2->field_0x00 = -1;
        g2->field_0x04 = -1;
        g2->field_0x08 = -1;
        g2->field_0x10 = 0;
        g2->field_0x20 = 0;
        g2->field_0x14 = 0;
        g2->field_0x18 = 0;
        g2->field_0x1C = 0;
        g2->field_0x34 = 0;
        g2->field_0xB0 = 0;
        g2->field_0x38 = 0;
        g2->field_0x3C = 0;
        g2->field_0x50 = 0;
        memset(&g2->field_0xA0, 0, 0xC);
        memset(g2->field_0x5C, 0, 0x34);
        __register_global_object(g2, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    CtrlObjectParam_InitArtsListEntry((void*)sHeapList78, g->field_0x28, 0x70E800, 4);
    g->field_0x54 = 0;
    g->field_0x58 = 0;
    int h = (int)CtrlObjectParam_AllocArtsListNode((void*)sHeapList78, 0x700000, 0);
    g->field_0x54 = h;
    CtrlObjectParam_WriteArtsListEntry((void*)sHeapList78, (void*)h, (void*)2, (void*)0x22222222);
    return g->field_0x54;
}

extern "C" int KyoshinHeap_Register78(void* p1, void* p2, void* p3) {
    int result = 0;
    if (p1 != 0) {
        CtrlObjectParam_WriteArtsListEntry((void*)sHeapList78, p1, p2, p3);
        result = 1;
    }
    return result;
}

extern "C" void KyoshinHeap_Free78(void* p)
{
    if (p != 0) {
        CtrlObjectParam_MergeArtsListNode(sHeapList78, p);
    }
}

extern "C" void KyoshinHeap_Reset78Stage() {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    GlobalStruct_80572B94* g = &sKyoshinHeapState;
    CtrlObjectParam_InitArtsListEntry((void*)sHeapList78, g->field_0x28, 0x70E800, 4);
    g->field_0x54 = 0;
    g->field_0x58 = 0;
}

extern "C" void KyoshinHeap_Reset58Stage() {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    GlobalStruct_80572B94* g = &sKyoshinHeapState;
    CtrlObjectParam_InitArtsListEntry((void*)sHeapList58, g->field_0x24, 0xD00800, 5);
    g->field_0x10 = 0;
    g->field_0x14 = 0;
    g->field_0x18 = 0;
    g->field_0x34 = 0;
    g->field_0xB0 = 0;
}

extern "C" void KyoshinHeap_AllocFld10(int index) {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    GlobalStruct_80572B94* g = &sKyoshinHeapState;
    // Retail reproduces this guard twice (MWCC CSEs the repeated null-test).
    if (g->field_0x10 != 0) {
        if (g->field_0x10 != 0) {
            CtrlObjectParam_MergeArtsListNode((void*)sHeapList58, (void*)g->field_0x10);
        }
    }
    u32 r;
    if (index == 0) {
        r = 0;
    } else if (index > 0) {
        r = (int)CtrlObjectParam_AllocArtsListNode((void*)sHeapList58, index, 0);
    } else {
        r = func_800A36A4((void*)sHeapList58, -index, 0);
    }
    g->field_0x10 = r;
}

extern "C" void KyoshinHeap_AllocFld14(int index) {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(&g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    GlobalStruct_80572B94* g = &sKyoshinHeapState;
    // Retail reproduces this guard twice (MWCC CSEs the repeated null-test).
    if (g->field_0x14 != 0) {
        if (g->field_0x14 != 0) {
            CtrlObjectParam_MergeArtsListNode((void*)sHeapList58, (void*)g->field_0x14);
        }
    }
    u32 r;
    if (index == 0) {
        r = 0;
    } else if (index > 0) {
        r = (int)CtrlObjectParam_AllocArtsListNode((void*)sHeapList58, index, 0);
    } else {
        r = func_800A36A4((void*)sHeapList58, -index, 0);
    }
    g->field_0x14 = r;
}

extern "C" void KyoshinHeap_AllocFld18(int index) {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    GlobalStruct_80572B94* g = &sKyoshinHeapState;
    // Retail reproduces this guard twice (MWCC CSEs the repeated null-test).
    if (g->field_0x18 != 0) {
        if (g->field_0x18 != 0) {
            CtrlObjectParam_MergeArtsListNode((void*)sHeapList58, (void*)g->field_0x18);
        }
    }
    u32 r;
    int neg = -index;
    if (neg == 0) {
        r = 0;
    } else if (neg > 0) {
        r = (int)CtrlObjectParam_AllocArtsListNode((void*)sHeapList58, neg, 0);
    } else {
        r = func_800A36A4((void*)sHeapList58, index, 0);
    }
    g->field_0x18 = r;
}

extern "C" void KyoshinHeap_AllocFld34(int index) {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(&g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    GlobalStruct_80572B94* g = &sKyoshinHeapState;
    // Retail reproduces this guard twice (MWCC CSEs the repeated null-test).
    if (g->field_0x34 != 0) {
        if (g->field_0x34 != 0) {
            CtrlObjectParam_MergeArtsListNode((void*)sHeapList58, (void*)g->field_0x34);
        }
    }
    u32 r;
    int neg = -index;
    if (neg == 0) {
        r = 0;
    } else if (neg > 0) {
        r = (int)CtrlObjectParam_AllocArtsListNode((void*)sHeapList58, neg, 0);
    } else {
        r = func_800A36A4((void*)sHeapList58, index, 0);
    }
    g->field_0x34 = r;
    g->field_0xB0 = index;
}

extern "C" int KyoshinHeap_GetFieldAC() {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    return sKyoshinHeapState.field_0xAC;
}


extern "C" int KyoshinHeap_GetField50() {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(&g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    return sKyoshinHeapState.field_0x50;
}

extern "C" int KyoshinHeap_GetField38() {
    if (sKyoshinHeapInitialized == 0) {
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(&g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    return sKyoshinHeapState.field_0x38;
}

extern "C" int KyoshinHeap_GetField3C() {
    if (sKyoshinHeapInitialized == 0) {
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(&g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    return sKyoshinHeapState.field_0x3C;
}

extern "C" int KyoshinHeap_GetField40() {
    if (sKyoshinHeapInitialized == 0) {
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(&g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    return sKyoshinHeapState.field_0x40;
}

extern "C" int KyoshinHeap_GetField44() {
    if (sKyoshinHeapInitialized == 0) {
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(&g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    return sKyoshinHeapState.field_0x44;
}

extern "C" int KyoshinHeap_GetField4C(int index) {
    if (sKyoshinHeapInitialized == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &sKyoshinHeapState;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(&g->field_0xA0, 0, 0xC);
        memset(g->field_0x5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)sHeapSingletonRegistration);
        sKyoshinHeapInitialized = 1;
    }
    GlobalStruct_80572B94* g = &sKyoshinHeapState;
    if (index == 0) {
        return g->field_0x4C;
    }
    if (index == 1) {
        return g->field_0x4C + 0x20000;
    }
    return 0;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Static-initializer for the array of 6 registered objects at sHeapRegionBlocks.
// Each 0x20-wide entry holds a registration node at +0x0 and object data at
// +0x10; CtrlObjectParam_InitListElement constructs the object and __register_global_object
// attaches the destructor (__dt__800A34E0).
extern char sHeapRegionBlocks[];

extern "C" void sinit_800A9F40() {
    char* base = sHeapRegionBlocks;
    CtrlObjectParam_InitListElement(base + 0x10);
    __register_global_object(base + 0x10, (void*)__dt__800A34E0, base);
    CtrlObjectParam_InitListElement(base + 0x30);
    __register_global_object((void*)(base + 0x30), (void*)__dt__800A34E0, (void*)(base + 0x20));
    CtrlObjectParam_InitListElement(base + 0x50);
    __register_global_object((void*)(base + 0x50), (void*)__dt__800A34E0, (void*)(base + 0x40));
    CtrlObjectParam_InitListElement(base + 0x70);
    __register_global_object((void*)(base + 0x70), (void*)__dt__800A34E0, (void*)(base + 0x60));
    CtrlObjectParam_InitListElement(base + 0x90);
    __register_global_object((void*)(base + 0x90), (void*)__dt__800A34E0, (void*)(base + 0x80));
    CtrlObjectParam_InitListElement(base + 0xB0);
    __register_global_object((void*)(base + 0xB0), (void*)__dt__800A34E0, (void*)(base + 0xA0));
}

// ---------------------------------------------------------------------------
// Retail .bss / .sbss storage owned by this TU.
//
// The retail split places these objects in code_800A75FC's .bss (0x1B8 total)
// and .sbss (the initialized flag).  They are referenced by every
// KyoshinHeap_* accessor in this TU, so the definitions (not just the header's
// extern declarations) must live here: the link pairs this TU's object with
// the retail .bss/.sbss slots, and an undefined label aborts it.
//
// Sizes are the retail symbol sizes from the split object
// (build/us/obj/kyoshin/code_800A75FC.o), so MWCC lands each object at its
// retail offset: AC8 0x70, B38 0x20, B58 0x20, B78 0x10, B88 0x0C, B94 0xB4.
// Defined in retail offset order (MWCC emits zero-init data in definition
// order); all of them are > the small-data threshold, so they stay in .bss.
char sHeapRegionBlocks[0x70];
char sHeapList38[0x20];
char sHeapList58[0x20];
char sHeapList78[0x10];
char sHeapSingletonRegistration[0x0C];
GlobalStruct_80572B94 sKyoshinHeapState;

// .sbss: singleton-initialized flag.  One byte of storage (the accessors use
// byte loads/stores), so MWCC auto-routes it to .sbss; the retail symbol is
// 8 bytes wide (alignment padding before the next gap label).
s8 sKyoshinHeapInitialized;

// ---------------------------------------------------------------------------
// Retail .rodata / .data / remaining .bss / .sbss storage owned by this TU.
//
// .rodata holds the three heap names passed to mtl::MemManager::create by
// KyoshinHeap_Init.  .data holds the per-stage 14-word offset tables and the
// two 6-slot LblPtrTable arrays the runtime fills in.  Values are the retail
// split's bytes (build/us/obj/kyoshin/code_800A75FC.o); the tables are read
// only by the heap entry points that are still open (KyoshinHeap_CreateRegions /
// KyoshinHeap_AllocNode / KyoshinHeap_GetField54), so nothing in this TU references them yet.
// The all-zero arrays need #pragma section data_type: MWCC migrates an
// all-zero aggregate to .bss even with __declspec(section ...), and the
// nocommon/aligned attribute forms do not survive it either.  Keeping every
// section at its retail size matters: ldscript.lcf concatenates sections
// positionally, so a short .data/.bss/.sbss would move every later unit's data.
__declspec(section ".rodata") char lbl_eu_804FBF60[0x18] = "CF_MEM1\0CF_MEM2\0CF_BDAT";
#pragma section data_type ".data" ".data"
u32 lbl_eu_80528188[14] = { 0x96000, 0x20800, 0x2F800, 0x32000, 0x2F800, 0x2A800, 0x39800, 0x32000, 0x2F800, 0x94000, 0x94000, 0x94000, 0x2F800, 0x2F800 };   // retail .data+0x000
u32 lbl_eu_805281C0[14] = { 0x0, 0x28000, 0x1E000, 0x2D000, 0x23000, 0x2A800, 0x32000, 0x2A800, 0x1E000, 0x800, 0x800, 0x800, 0x23000, 0x23000 };   // retail .data+0x038
u32 lbl_eu_805281F8[14] = { 0x0, 0x1B800, 0x16800, 0xF000, 0x16800, 0x14000, 0x11800, 0x11800, 0x19000, 0x800, 0x800, 0x800, 0x16800, 0x16800 };   // retail .data+0x070
u32 lbl_eu_80528230[14] = { 0x0, 0x19000, 0x1E000, 0x16800, 0x19000, 0x19000, 0x14000, 0x16800, 0x1E000, 0x800, 0x800, 0x800, 0x19000, 0x19000 };   // retail .data+0x0A8
u32 lbl_eu_80528268[14] = { 0x0, 0x19000, 0x14000, 0x11800, 0x14000, 0x14000, 0x5000, 0x11800, 0x11800, 0x800, 0x800, 0x800, 0x14000, 0x14000 };   // retail .data+0x0E0
__declspec(section ".data") LblPtrTable* lbl_eu_805282A0[6] = { 0 };   // retail .data+0x118
u32 lbl_eu_805282B8[14] = { 0x0, 0x20800, 0x2F800, 0x32000, 0x2F800, 0x2A800, 0x39800, 0x32000, 0x2F800, 0x94000, 0x94000, 0x94000, 0x2F800, 0x2F800 };   // retail .data+0x130
u32 lbl_eu_805282F0[14] = { 0x0, 0x48800, 0x4D800, 0x5F000, 0x52800, 0x55000, 0x6B800, 0x5C800, 0x4D800, 0x94800, 0x94800, 0x94800, 0x52800, 0x52800 };   // retail .data+0x168
u32 lbl_eu_80528328[14] = { 0x0, 0x64000, 0x64000, 0x6E000, 0x69000, 0x69000, 0x7D000, 0x6E000, 0x66800, 0x95000, 0x95000, 0x95000, 0x69000, 0x69000 };   // retail .data+0x1A0
u32 lbl_eu_80528360[14] = { 0x0, 0x7D000, 0x82000, 0x84800, 0x82000, 0x82000, 0x91000, 0x84800, 0x84800, 0x95800, 0x95800, 0x95800, 0x82000, 0x82000 };   // retail .data+0x1D8
__declspec(section ".data") LblPtrTable* lbl_eu_80528398[6] = { 0 };   // retail .data+0x210
#pragma section data_type ".data" ".bss"

// .bss remainder (retail 0x180..0x1B8): zero-initialised storage with no
// source reference (its users are the open heap entry points).
char lbl_eu_80572C48[0x38];

// .sbss stage state (retail 0x80663E98..0x80663EC8).  sKyoshinHeapInitialized is the
// singleton-init byte flag defined above; the retail slot for it spans eight
// bytes (symbol size 8), so lbl_eu_80663EA0 is declared 8-aligned to keep the
// remaining retail symbols at their retail offsets.  All-zero storage; only
// the flag is referenced from this TU today.
__attribute__((aligned(8))) s8 lbl_eu_80663EA0;
u32 lbl_eu_80663EA4;
s8  lbl_eu_80663EA8;
u32 lbl_eu_80663EAC;
s8  lbl_eu_80663EB0;
u32 lbl_eu_80663EB4;
s8  lbl_eu_80663EB8;
u32 lbl_eu_80663EBC;
s8  lbl_eu_80663EC0;
u32 lbl_eu_80663EC4;
