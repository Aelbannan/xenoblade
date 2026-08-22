// Auto-scaffolded catalog TU for kyoshin/code_800A75FC
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/code_800A75FC.hpp"
#include "monolib/util/MemManager.hpp"

// Forward declarations for external functions in other TUs.
struct GlobalStruct_80572B94;

// Retail linker labels referenced from this TU are declared in
// include/kyoshin/code_800A75FC.hpp.

void* memset(void*, int, unsigned long);

// --- Global singleton struct at lbl_eu_80572B94 (0xB4 bytes, BSS) ---
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
    u8 field_0x5C[0x34];      // memset 0 (0x5C-0x90)
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

// Global singleton object at lbl_eu_80572B94
extern GlobalStruct_80572B94 lbl_eu_80572B94;

extern "C" GlobalStruct_80572B94* __dt__800A75FC(GlobalStruct_80572B94* self, int flag) {
    if (self != 0) {
        // Release the three allocations handles, then reset the object.
        if ((u32)self->field_0x00 != 0xFFFFFFFF) {
            mtl::MemManager::erase((u32)self->field_0x00);
            self->field_0x00 = -1;
        }
        if ((u32)self->field_0x04 != 0xFFFFFFFF) {
            mtl::MemManager::erase((u32)self->field_0x04);
            self->field_0x04 = -1;
        }
        if ((u32)self->field_0x08 != 0xFFFFFFFF) {
            mtl::MemManager::erase((u32)self->field_0x08);
            self->field_0x08 = -1;
        }
        self->field_0x10 = 0;
        self->field_0x20 = 0;
        self->field_0x14 = 0;
        self->field_0x18 = 0;
        self->field_0x1C = 0;
        self->field_0x34 = 0;
        self->field_0xB0 = 0;
        self->field_0x38 = 0;
        self->field_0x3C = 0;
        self->field_0x50 = 0;
        memset(&self->field_0xA0, 0, 0xC);
        memset(self->field_0x5C, 0, 0x34);
        if (flag > 0) {
            delete self;
        }
    }
    return self;
}

extern "C" void func_800A76EC(GlobalStruct_80572B94* g) {
    u8* blk = (u8*)lbl_eu_80572AC8;
    const u32 zero = 0;

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
        void* dst = blk + 0x10;
        u32 objectAligned = object + 0x100;
        if (objectAligned & 0x1F) objectAligned = (objectAligned + 0x20) - (objectAligned & 0x1F);
        func_800A3520(dst, objectAligned, 0x003B1C00, 0);
    }

    if ((u32)g->field_0x04 == 0xFFFFFFFF) {
        u32 h = mtl::MemManager::create(
            mtl::MemManager::getHandleMEM2(), 0x2872200, &lbl_eu_804FBF60[8]);
        g->field_0x04 = (int)h;
        u32 object = (u32)mtl::MemManager::getMaxAllocData(h);
        void* dst = blk + 0x30;
        u32 objectAligned = object + 0x100;
        if (objectAligned & 0x1F) objectAligned = (objectAligned + 0x20) - (objectAligned & 0x1F);
        func_800A3520(dst, objectAligned, 0x2872000, 1);
    }

    if ((u32)g->field_0x08 == 0xFFFFFFFF) {
        u32 h = mtl::MemManager::create(
            mtl::MemManager::getHandleMEM2(), 0x48B00, &lbl_eu_804FBF60[0x10]);
        g->field_0x08 = (int)h;
        u32 object = (u32)mtl::MemManager::getMaxAllocData(h);
        void* dst = blk + 0x50;
        u32 objectAligned = object + 0x100;
        if (objectAligned & 0x1F) objectAligned = (objectAligned + 0x20) - (objectAligned & 0x1F);
        func_800A3520(dst, objectAligned, 0x48900, 1);
    }

    // --- Sub-allocations from the three regions ---
    g->field_0x20 = func_800A3594(blk + 0x10, 0x300000, 0);
    g->field_0xA0 = func_800A3594(blk + 0x10, 0x00051000, 0);
    g->field_0xA4 = func_800A3594(blk + 0x10, 0x46800, 0);
    g->field_0xA8 = func_800A3594(blk + 0x10, 0x10000, 0);
    g->field_0x1C = func_800A3594(blk + 0x10, 0xA000, 0);

    g->field_0xAC = func_800A3594(blk + 0x30, 0xF4000, 0);
    g->field_0x2C = func_800A3594(blk + 0x30, 0xA90000, 0);
    g->field_0x24 = func_800A3594(blk + 0x30, 0xD00800, 0);
    g->field_0x38 = func_800A3594(blk + 0x30, 0x106000, 0);
    g->field_0x3C = func_800A3594(blk + 0x30, 0x1A0000, 0);
    g->field_0x50 = func_800A3594(blk + 0x30, 0xF800, 0);
    g->field_0x4C = func_800A3594(blk + 0x30, 0x40000, 0);
    g->field_0x9C = func_800A3594(blk + 0x30, 0x5F8800, 0);
    g->field_0x28 = func_800A3594(blk + 0x30, 0x718800, 0);

    g->field_0x30 = zero;
    g->field_0x40 = func_800A3594(blk + 0x50, 0x20000, 0);
    g->field_0x44 = func_800A3594(blk + 0x50, 0x28000, 0);

    // base addresses derived from field_0x2C
    u32 base = g->field_0x2C;
    ((u32*)g->field_0x5C)[0] = base;
    ((u32*)g->field_0x5C)[1] = base + 0xA6000;
    ((u32*)g->field_0x5C)[2] = base + 0x14C000;
    ((u32*)g->field_0x5C)[3] = base + 0x1F2000;
    ((u32*)g->field_0x5C)[4] = base + 0x298000;
    ((u32*)g->field_0x5C)[5] = base + 0x33E000;
    ((u32*)g->field_0x5C)[6] = base + 0x3E4000;
    g->field_0x90 = base + 0x48A000;
    g->field_0x94 = base + 0x79D800;
    g->field_0x98 = base + 0x8C5000;

    // --- Ensure the global singleton is constructed (repeated defensive guards) ---
    GlobalStruct_80572B94* gbl = &lbl_eu_80572B94;
    if (lbl_eu_80663E98 == 0) {
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
        lbl_eu_80663E98 = 1;
    }

    func_800A3520(blk + 0x70, gbl->field_0x9C, 0x5F8800, 3);

    if (lbl_eu_80663E98 == 0) {
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
        lbl_eu_80663E98 = 1;
    }

    func_800A3520(blk + 0xb0, gbl->field_0x28, 0x718800, 4);
    gbl->field_0x54 = 0;
    gbl->field_0x58 = 0;

    if (lbl_eu_80663E98 == 0) {
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
        lbl_eu_80663E98 = 1;
    }

    func_800A3520(blk + 0x90, gbl->field_0x24, 0xD00800, 5);
    gbl->field_0x10 = zero;
    gbl->field_0x14 = zero;
    gbl->field_0x18 = zero;
    gbl->field_0x34 = zero;
    gbl->field_0xB0 = zero;

    u32 alloc = func_800A3594(blk + 0xb0, 0x700000, 0);
    g->field_0x54 = alloc;
    func_800A39E8(blk + 0xb0, (void*)alloc, (void*)2, (void*)0x11111111);
    g->field_0x58 = zero;
}

void func_800A7CDC() {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    func_800A76EC(&lbl_eu_80572B94);
}

void func_800A7D9C() {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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

int func_800A7EFC() {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return lbl_eu_80572B94.field_0x10;
}

int func_800A7FBC() {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return lbl_eu_80572B94.field_0x18;
}

int func_800A807C() {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return lbl_eu_80572B94.field_0x34;
}

int func_800A813C() {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return lbl_eu_80572B94.field_0x20;
}

int func_800A81FC() {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return lbl_eu_80572B94.field_0x1C;
}

int func_800A82BC() {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return lbl_eu_80572B94.field_0xA0;
}

int func_800A837C() {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return lbl_eu_80572B94.field_0xA4;
}

int func_800A843C() {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return lbl_eu_80572B94.field_0xA8;
}

u32 func_800A84FC(int index) {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return ((u32*)lbl_eu_80572B94.field_0x5C)[index] + 0x96000;
}

u32 func_800A85D8(int index) {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return ((u32*)lbl_eu_80572B94.field_0x5C)[index];
}

void* func_800A86AC(unsigned int param1, unsigned int param2) {
    LblPtrTable* table = lbl_eu_805282A0[param2];
    if (param1 >= 0xe) {
        return table->slots[0];
    }
    return table->slots[param1];
}

void* func_800A86D8(unsigned int param1, unsigned int param2) {
    LblPtrTable* table = lbl_eu_80528398[param2];
    if (param1 >= 0xe) {
        return table->slots[0];
    }
    return table->slots[param1];
}

u32 func_800A8704(int index) {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return lbl_eu_80572B94.field_0x90 + index * 0x106800;
}

u32 func_800A87E0(int index) {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    u32 v = lbl_eu_80572B94.field_0x90 + index * 0x106800;
    if (v != 0) {
        v += 0x32000;
    }
    return v;
}

u32 func_800A88C8(int index) {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    u32 v = lbl_eu_80572B94.field_0x90 + index * 0x106800;
    if (v != 0) {
        v += 0x32000;
    }
    if (v != 0) {
        v += 0xC8000;
    }
    return v;
}

u32 func_800A89C0(int index) {
    if (index >= 3) return 0;
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return lbl_eu_80572B94.field_0x94 + index * 0x62800;
}

u32 func_800A8AAC(int index) {
    if (index >= 3) return 0;
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return lbl_eu_80572B94.field_0x98 + index * 0x99000;
}

int func_800A8B98(int param) {
    if (param == 0) return 0;
    if (param > 0) {
        return func_800A3594((void*)lbl_eu_80572B38, param, 0);
    } else {
        return func_800A36A4((void*)lbl_eu_80572B38, -param, 0);
    }
}

int func_800A8BD8(u32 param) {
    if (param == 0) return 0;
    u32 start = *(u32*)(lbl_eu_80572B38);
    u32 size = *(u32*)(lbl_eu_80572B38 + 0xC);
    u32 end = start + size;
    int result = 0;
    if (start <= param && param < end) {
        result = 1;
    }
    return result;
}

int func_800A8C1C(void* p1, void* p2, void* p3) {
    int result = 0;
    if (p1 != 0) {
        func_800A39E8((void*)lbl_eu_80572B38, p1, p2, p3);
        result = 1;
    }
    return result;
}

void func_800A8C68(void* p) {
    if (p == 0) return;
    func_800A37CC((void*)lbl_eu_80572B38, p);
}

void func_800A8C84(void) {
    func_800A3940(reinterpret_cast<cf::CtrlObjectParamArtsList*>(lbl_eu_80572B38));
}

u32 func_800A8C90() {
    return func_800A3998((void*)lbl_eu_80572B38) / 774144;
}

int func_800A8CD4() {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return lbl_eu_80572B94.field_0x54 != 0 ? lbl_eu_80572B94.field_0x54
                                           : lbl_eu_80572B94.field_0x58;
}

u32 func_800A8DA4(){ return 0x500000; }

int func_800A8DAC() {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return lbl_eu_80572B94.field_0x58;
}

int func_800A8E6C(int size, int arg2) {
    if (size == 0) return 0;
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    GlobalStruct_80572B94* g = &lbl_eu_80572B94;
    if (g->field_0x54 != 0) {
        if (g->field_0x54 != 0) {
            func_800A37CC((void*)lbl_eu_80572B78, (void*)g->field_0x54);
        }
        g->field_0x54 = 0;
    }
    if (g->field_0x58 == 0) {
        int h = func_800A3594((void*)lbl_eu_80572B78, 0x500000, 0);
        g->field_0x58 = h;
        if (h != 0) {
            func_800A39E8((void*)lbl_eu_80572B78, (void*)h, (void*)2, (void*)0x99999999);
        }
    }
    int r;
    if (size > 0) {
        r = func_800A3594((void*)lbl_eu_80572B78, size, arg2);
    } else {
        r = func_800A36A4((void*)lbl_eu_80572B78, -size, arg2);
    }
    if (arg2 != 0) {
        if (r != 0) {
            if (r != 0) {
                func_800A37CC((void*)lbl_eu_80572B78, (void*)r);
            }
        }
    }
    return r;
}

int func_800A9024(u32 param) {
    if (param == 0) return 0;
    u32 start = *(u32*)(lbl_eu_80572B78);
    u32 size = *(u32*)(lbl_eu_80572B78 + 0xC);
    u32 end = start + size;
    int result = 0;
    if (start <= param && param < end) {
        result = 1;
    }
    return result;
}

int func_800A9068() {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return lbl_eu_80572B94.field_0x54 != 0;
}

int func_800A9134() {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    GlobalStruct_80572B94* g = &lbl_eu_80572B94;
    // Retail keeps field_0x54 cached and returns it directly if already set.
    if (g->field_0x54 != 0) {
        return g->field_0x54;
    }
    // Second lazy-init guard (retail duplicates the guard inside this function).
    if (lbl_eu_80663E98 == 0) {
        GlobalStruct_80572B94* g2 = &lbl_eu_80572B94;
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
        __register_global_object(g2, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    func_800A3520((void*)lbl_eu_80572B78, g->field_0x28, 0x70E800, 4);
    g->field_0x54 = 0;
    g->field_0x58 = 0;
    int h = func_800A3594((void*)lbl_eu_80572B78, 0x700000, 0);
    g->field_0x54 = h;
    func_800A39E8((void*)lbl_eu_80572B78, (void*)h, (void*)2, (void*)0x22222222);
    return g->field_0x54;
}

int func_800A92F8(void* p1, void* p2, void* p3) {
    int result = 0;
    if (p1 != 0) {
        func_800A39E8((void*)lbl_eu_80572B78, p1, p2, p3);
        result = 1;
    }
    return result;
}

void func_800A9344(void* p)
{
    if (p != 0) {
        func_800A37CC(lbl_eu_80572B78, p);
    }
}

void func_800A9360() {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    GlobalStruct_80572B94* g = &lbl_eu_80572B94;
    func_800A3520((void*)lbl_eu_80572B78, g->field_0x28, 0x70E800, 4);
    g->field_0x54 = 0;
    g->field_0x58 = 0;
}

void func_800A9444() {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    GlobalStruct_80572B94* g = &lbl_eu_80572B94;
    func_800A3520((void*)lbl_eu_80572B58, g->field_0x24, 0xD00800, 5);
    g->field_0x10 = 0;
    g->field_0x14 = 0;
    g->field_0x18 = 0;
    g->field_0x34 = 0;
    g->field_0xB0 = 0;
}

void func_800A9534(int index) {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    GlobalStruct_80572B94* g = &lbl_eu_80572B94;
    // Retail reproduces this guard twice (MWCC CSEs the repeated null-test).
    if (g->field_0x10 != 0) {
        if (g->field_0x10 != 0) {
            func_800A37CC((void*)lbl_eu_80572B58, (void*)g->field_0x10);
        }
    }
    u32 r;
    if (index == 0) {
        r = 0;
    } else if (index > 0) {
        r = func_800A3594((void*)lbl_eu_80572B58, index, 0);
    } else {
        r = func_800A36A4((void*)lbl_eu_80572B58, -index, 0);
    }
    g->field_0x10 = r;
}

void func_800A965C(int index) {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    GlobalStruct_80572B94* g = &lbl_eu_80572B94;
    // Retail reproduces this guard twice (MWCC CSEs the repeated null-test).
    if (g->field_0x14 != 0) {
        if (g->field_0x14 != 0) {
            func_800A37CC((void*)lbl_eu_80572B58, (void*)g->field_0x14);
        }
    }
    u32 r;
    if (index == 0) {
        r = 0;
    } else if (index > 0) {
        r = func_800A3594((void*)lbl_eu_80572B58, index, 0);
    } else {
        r = func_800A36A4((void*)lbl_eu_80572B58, -index, 0);
    }
    g->field_0x14 = r;
}

void func_800A9784(int index) {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    GlobalStruct_80572B94* g = &lbl_eu_80572B94;
    // Retail reproduces this guard twice (MWCC CSEs the repeated null-test).
    if (g->field_0x18 != 0) {
        if (g->field_0x18 != 0) {
            func_800A37CC((void*)lbl_eu_80572B58, (void*)g->field_0x18);
        }
    }
    u32 r;
    int neg = -index;
    if (neg == 0) {
        r = 0;
    } else if (neg > 0) {
        r = func_800A3594((void*)lbl_eu_80572B58, neg, 0);
    } else {
        r = func_800A36A4((void*)lbl_eu_80572B58, index, 0);
    }
    g->field_0x18 = r;
}

void func_800A98A8(int index) {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    GlobalStruct_80572B94* g = &lbl_eu_80572B94;
    // Retail reproduces this guard twice (MWCC CSEs the repeated null-test).
    if (g->field_0x34 != 0) {
        if (g->field_0x34 != 0) {
            func_800A37CC((void*)lbl_eu_80572B58, (void*)g->field_0x34);
        }
    }
    u32 r;
    int neg = -index;
    if (neg == 0) {
        r = 0;
    } else if (neg > 0) {
        r = func_800A3594((void*)lbl_eu_80572B58, neg, 0);
    } else {
        r = func_800A36A4((void*)lbl_eu_80572B58, index, 0);
    }
    g->field_0x34 = r;
    g->field_0xB0 = index;
}

int func_800A99D0() {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return lbl_eu_80572B94.field_0xAC;
}


int func_800A9A90() {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return lbl_eu_80572B94.field_0x50;
}

int func_800A9B50() {
    if (lbl_eu_80663E98 == 0) {
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return lbl_eu_80572B94.field_0x38;
}

int func_800A9C10() {
    if (lbl_eu_80663E98 == 0) {
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return lbl_eu_80572B94.field_0x3C;
}

int func_800A9CD0() {
    if (lbl_eu_80663E98 == 0) {
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return lbl_eu_80572B94.field_0x40;
}

int func_800A9D90() {
    if (lbl_eu_80663E98 == 0) {
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return lbl_eu_80572B94.field_0x44;
}

int func_800A9E50(int index) {
    if (lbl_eu_80663E98 == 0) {
        // Lazy-init the global singleton once.
        GlobalStruct_80572B94* g = &lbl_eu_80572B94;
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
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    GlobalStruct_80572B94* g = &lbl_eu_80572B94;
    if (index == 0) {
        return g->field_0x4C;
    }
    if (index == 1) {
        return g->field_0x4C + 0x20000;
    }
    return 0;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Static-initializer for the array of 6 registered objects at lbl_eu_80572AC8.
// Each 0x20-wide entry holds a registration node at +0x0 and object data at
// +0x10; func_800A34C8 constructs the object and __register_global_object
// attaches the destructor (__dt__800A34E0).
extern char lbl_eu_80572AC8[];

void sinit_800A9F40() {
    char* base = lbl_eu_80572AC8;
    func_800A34C8(base + 0x10);
    __register_global_object(base + 0x10, (void*)__dt__800A34E0, base);
    func_800A34C8(base + 0x30);
    __register_global_object((void*)(base + 0x30), (void*)__dt__800A34E0, (void*)(base + 0x20));
    func_800A34C8(base + 0x50);
    __register_global_object((void*)(base + 0x50), (void*)__dt__800A34E0, (void*)(base + 0x40));
    func_800A34C8(base + 0x70);
    __register_global_object((void*)(base + 0x70), (void*)__dt__800A34E0, (void*)(base + 0x60));
    func_800A34C8(base + 0x90);
    __register_global_object((void*)(base + 0x90), (void*)__dt__800A34E0, (void*)(base + 0x80));
    func_800A34C8(base + 0xB0);
    __register_global_object((void*)(base + 0xB0), (void*)__dt__800A34E0, (void*)(base + 0xA0));
}
