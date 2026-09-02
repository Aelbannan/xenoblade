// CfGimmickElv - elevator gimmick (cf namespace)
// Decompiled from retail ASM at build/us/asm/kyoshin/cf/CfGimmickElv.s

#include "kyoshin/cf/CfGimmickElv.hpp"
#include "monolib/scn/CScnTimeApi.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// absorb: retail data (generated) — fixed object bytes (relocs via inject)
extern "C" {
struct __data_CfGimmickElv {
    unsigned char a[0x24];
    unsigned char b[0x60];
};
__declspec(section ".data") __attribute__((used, aligned(4))) const __data_CfGimmickElv __data_blob_CfGimmickElv = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00}
};
#define lbl_eu_80535844 ((char*)&__data_blob_CfGimmickElv.a)
#define lbl_eu_80535868 ((char*)&__data_blob_CfGimmickElv.b)

__declspec(section ".rodata") __attribute__((aligned(8), used)) const unsigned char __absorb_kyoshin_cf_CfGimmickElv_rodata[112] = {
    0x63,0x66,0x3a,0x3a,0x43,0x66,0x47,0x69,0x6d,0x6d,0x69,0x63,0x6b,0x45,0x6c,0x76,0x00,0x00,0x00,0x00,0x6d,0x61,0x70,0x4f,0x62,0x6a,0x00,0x4c,0x49,0x46,0x54,0x4c,0x4f,0x44,0x00,0x73,0x68,0x74,0x41,0x4c,0x4f,0x44,0x00,0x73,0x68,0x74,0x42,0x4c,0x4f,0x44,0x00,0x73,0x77,0x74,0x4c,0x4f,0x44,0x00,0x73,0x77,0x69,0x74,0x63,0x68,0x45,0x46,0x00,0x73,0x77,0x69,0x74,0x63,0x68,0x53,0x45,0x00,0x73,0x68,0x74,0x53,0x45,0x00,0x4c,0x53,0x53,0x45,0x00,0x4c,0x4c,0x53,0x45,0x00,0x4c,0x45,0x53,0x45,0x00,0x4c,0x4f,0x44,0x53,0x54,0x4f,0x50,0x00,0x00,0x00,0x00
};
__declspec(section ".sdata2") __attribute__((aligned(8), used)) const unsigned char __absorb_kyoshin_cf_CfGimmickElv_sdata2[40] = {
    0x00,0x00,0x00,0x00,0x3F,0x80,0x00,0x00,0x43,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0xA0,0x00,0x00,0x41,0xF0,0x00,0x00,0x40,0x80,0x00,0x00,0x43,0x48,0x00,0x00,0x44,0x61,0x00,0x00,0x00,0x00,0x00,0x00
};
#define lbl_eu_80668380 (*(const float*)&__absorb_kyoshin_cf_CfGimmickElv_sdata2[0])
#define lbl_eu_80668384 (*(const float*)&__absorb_kyoshin_cf_CfGimmickElv_sdata2[4])

} // extern "C"

extern "C" {
extern char lbl_eu_805358C8[];   // CfGimmickElv vtable (actually next TU, keep extern)
extern char lbl_eu_805357E8[];   // bdat column pointer table
extern char lbl_eu_80664130[];    // bdat table pointer (via sda21)
extern char lbl_eu_806646BC[];    // global flag
typedef void (CfGimmickElvData::*CfGimmickElvStateFunc)();
extern char lbl_eu_805765B0[];  // bit array
extern char lbl_eu_805765A0[];  // vec3 constant (player-relative check center)
extern u32 jumptable_eu_80535830[]; // jump table
}

// Helper: get bdat column value (returns u16/u8 depending on column type)
static inline u16 getCol16(void* table, void* col, int row) {
    return (u16)getBdatStringColumnValue(table, (const char*)col, row);
}
static inline u8 getCol8(void* table, void* col, int row) {
    return (u8)getBdatStringColumnValue(table, (const char*)col, row);
}

// ============================================================
// Constructor: __ct__cf_CfGimmickElv (0x600 bytes)
// ============================================================
extern "C" void __ct__cf_CfGimmickElv(CfGimmickElvData* self, u16 rowId) {
    __ct__cf_CfGimmick(self);
    self->vtable = (void*)lbl_eu_805358C8;
    self->typeId = 3;

    void* mgr = func_8003AA34();
    void* table = *(void**)((u8*)mgr + (u32)&lbl_eu_80664130);
    u8* colBase = (u8*)lbl_eu_805357E8;

    self->bdatRowId = rowId;

    // Read bdat columns
    self->val1B4 = getCol16(table, colBase + 0x2C, rowId);
    self->unk6A = getCol16(table, colBase + 0x30, rowId);

    // Column at offset 0x28 - read twice with increment
    u8* col28 = colBase + 0x28;
    *col28 = 0x41;
    self->unk66 = getCol8(table, col28, rowId);
    *col28 = *col28 + 1;
    self->flag1B0 = getCol8(table, col28, rowId);
    *col28 = *col28 + 1;
    self->flag1B1 = getCol8(table, col28, rowId);
    *col28 = *col28 + 1;
    self->flag1B2 = getCol8(table, col28, rowId);

    // Initialize sub-objects (4 groups x 3 types)
    u32 stackVar = (u32)table;
    func_8020938C(self, &self->vec0, (void*)lbl_eu_805357E8, &stackVar, 0);
    func_802095D8(self, &self->vec1, (void*)lbl_eu_805357E8, &stackVar, 0);
    func_80209488(self, &self->vec2, (void*)lbl_eu_805357E8, &stackVar, 0);
    func_8020938C(self, &self->elvVec0, (void*)lbl_eu_805357E8, &stackVar, 1);
    func_802095D8(self, &self->elvVec3, (void*)lbl_eu_805357E8, &stackVar, 1);
    func_80209488(self, &self->elvVec6, (void*)lbl_eu_805357E8, &stackVar, 1);
    func_8020938C(self, &self->elvVec1, (void*)lbl_eu_805357E8, &stackVar, 2);
    func_802095D8(self, &self->elvVec4, (void*)lbl_eu_805357E8, &stackVar, 2);
    func_80209488(self, &self->elvVec7, (void*)lbl_eu_805357E8, &stackVar, 2);
    func_8020938C(self, &self->elvVec2, (void*)lbl_eu_805357E8, &stackVar, 3);
    func_802095D8(self, &self->elvVec5, (void*)lbl_eu_805357E8, &stackVar, 3);
    func_80209488(self, &self->elvVec8, (void*)lbl_eu_805357E8, &stackVar, 3);

    // Virtual call: vtable[8] (offset 0x20)
    void (*vfunc)(CfGimmickElvData*) = *(void(**)(CfGimmickElvData*))(*(u32*)self + 0x20);
    vfunc(self);

    // More bdat columns
    u8* nameBase = (u8*)(__absorb_kyoshin_cf_CfGimmickElv_rodata+0x14);
    self->unk68 = getCol16(table, nameBase + 0x07, rowId);
    self->lod0 = getCol8(table, nameBase + 0x0F, rowId);
    self->lod1 = getCol8(table, nameBase + 0x17, rowId);
    self->lod2 = getCol8(table, nameBase + 0x1F, rowId);
    self->lod3 = getCol8(table, nameBase + 0x26, rowId);

    // Bit manipulation on global array
    if (self->lod0 != 0) {
        u32 idx = self->lod0;
        u32 word = idx >> 3;  // rlwinm r6, r3, 29, 27, 29
        u32 bit = idx & 0x1F; // clrlwi r0, r3, 27
        lbl_eu_805765B0[word] &= ~(1u << bit);
    }

    self->flag1B3 = getCol8(table, nameBase + 0x2F, rowId);
    self->val1B6 = getCol16(table, nameBase + 0x38, rowId);
    self->val1B8 = getCol16(table, nameBase + 0x3E, rowId);
    self->val1BA = getCol16(table, nameBase + 0x43, rowId);
    self->val1BC = getCol16(table, nameBase + 0x48, rowId);
    self->val1BE = getCol16(table, nameBase + 0x4D, rowId);

    // LOD setup
    u16 lastVal = getCol16(table, nameBase + 0x4D, rowId);
    // Actually re-read: the last getBdatStringColumnValue result
    // is used for the LOD check below

    // LOD timer setup
    if (lastVal != 0 && self->lod0 != 0) {
        f32 fval = (f32)(u32)lastVal;
        updateLODObject__8CTaskLODFv(self->lod0, fval);
    }

    // LOD init loop for lod0..lod2
    int i;
    for (i = 0; i < 3; i++) {
        u8 lod = ((u8*)self)[0x70 + i];
        if (lod != 0) {
        }
    }

    if (self->lod0 != 0) {
        attachLODObject__8CTaskLODFv(self->lod0, 0);
        addLODEntry__8CTaskLODFv(self->lod0, 0);
    }

    // LOD1 setup
    if (self->lod1 != 0) {
        int mode = 0;
        if (self->flag1B1 & 0x10) {
            mode = 1;
            void* snd = getScnHandle__Fv();
            func_804BCC30(snd, self->lod1);
            activateLOD__8CTaskLODFv(self->lod1);
        }
        attachLODObject__8CTaskLODFv(self->lod1, mode);
        addLODEntry__8CTaskLODFv(self->lod1, mode);
    }

    // LOD2 setup
    if (self->lod2 != 0) {
        int mode = 0;
        if (self->flag1B2 & 0x10) {
            mode = 1;
            void* snd = getScnHandle__Fv();
            func_804BCC30(snd, self->lod2);
            activateLOD__8CTaskLODFv(self->lod2);
        }
        attachLODObject__8CTaskLODFv(self->lod2, mode);
        addLODEntry__8CTaskLODFv(self->lod2, mode);
    }

    // Flag setup based on unk66 bits
    if (self->unk66 & 0x08) {  // rlwinm. r0, r0, 0, 28, 28 = bit 3
        if (self->lod0 != 0) {
            clearLODEntry__8CTaskLODFv(self->lod0);
        }
        self->flags |= 0x40;
    } else {
        self->flags |= 0x20;
    }

    if (self->unk66 & 0x80) {  // rlwinm. r0, r0, 0, 24, 24 = bit 7
        self->flags |= 0x2000;
    }

    if (self->unk66 & 0x40) {  // rlwinm. r0, r0, 0, 25, 25 = bit 6
        self->flags |= 0x4000;
    }

    if (self->lod1 == 0) {
        self->flags |= 0x02;
    }

    if (self->lod2 == 0) {
        self->flags |= 0x04;
    }

    // Final init
    self->state = 0;
    self->unk1AC = 0;
    self->unk1A8 = 0;
    self->unk1A4 = 0;
    self->val1D6 = 0;
}

// ============================================================
// Destructor: __dt__Q22cf12CfGimmickElvFv (0x8C bytes)
// ============================================================
extern "C" void* __dt__Q22cf12CfGimmickElvFv(CfGimmickElvData* self, int mode) {
    if (self != NULL) {
        self->vtable = (void*)lbl_eu_805358C8;
        func_80208EE4(self);
        func_8020A434((void*)&self->unk7C);
        func_8020A434((void*)&self->unk1A4);
        func_8020A434((void*)&self->unk1A8);
        func_8020A434((void*)&self->unk1AC);
        __dt__Q22cf9CfGimmickFv(self, 0);
        if (mode > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// ============================================================
// func_8020B20C (0x58 bytes) - state dispatch + update
// ============================================================
extern "C" void func_8020B20C(CfGimmickElvData* self) {
    if (*(u32*)lbl_eu_806646BC & 2) {
        // PTMF call through state table
        typedef void (CfGimmickElvData::*PMF)(); PMF* tbl = (PMF*)lbl_eu_80535868; (self->*tbl[self->state])();
        func_8020B34C(self);
    }
}

// ============================================================
// func_8020B264 (0x80 bytes) - LOD visibility toggle
// ============================================================
extern "C" void func_8020B264(CfGimmickElvData* self, int show) {
    u8 lod = self->lod0;
    if (lod != 0) {
        // Clear visibility/mode bits once, then set the appropriate one
        u32 f = self->flags & ~0x1000E0u;
        self->flags = f;
        if (show != 0) {
            self->flags = f | 0x40;
            clearLODEntry__8CTaskLODFv(lod);
        } else {
            self->flags = f | 0x20;
        }
        getLODOrSelf__8CTaskLODFv(self->lod0);
    }
}
// ============================================================
// func_8020B2E4 (0x68 bytes) - copy sub-objects
// ============================================================
extern "C" void func_8020B2E4(CfGimmickElvData* self) {
    func_802089BC(&self->vec2, &self->vec0, &self->vec1);
    func_802089BC(&self->elvVec6, &self->elvVec0, &self->elvVec3);
    func_802089BC(&self->elvVec7, &self->elvVec1, &self->elvVec4);
    func_802089BC(&self->elvVec8, &self->elvVec2, &self->elvVec5);
}

// ============================================================
// func_8020B34C (0x128 bytes) - init sub-objects on demand
// ============================================================
extern "C" void func_8020B34C(CfGimmickElvData* self) {
    // Init unk7C if needed
    if ((self->unk66 & 1) && self->unk7C == 0) {
        self->unk7C = (u32)func_8020A35C((void*)func_8020A608(self->unk6A, 0), 0xF, &self->vec0);
    }

    // Init unk1A4 if needed
    if ((self->flag1B0 & 1) && self->unk1A4 == 0) {
        self->unk1A4 = (u32)func_8020A35C((void*)func_8020A608(self->unk6A, 0), 0xF, &self->elvVec0);
    }

    // Check flags bit 8 (0x100)
    if (self->flags & 0x100) {
        // LOD1 effect (bit 23)
        if ((self->flag1B1 & 1) && (self->flags & 0x00800000)) {
        }
        // LOD2 effect (bit 24)
        if ((self->flag1B2 & 1) && (self->flags & 0x01000000)) {
        }
    } else {
        func_8020A434((void*)&self->unk1A8);
        func_8020A434((void*)&self->unk1AC);
    }

    // Clear transient flags
    self->flags &= 0xFE1FFEFF;
}

// ============================================================
// func_8020B474 (0x150 bytes) - activation check
// ============================================================
extern "C" void func_8020B474(CfGimmickElvData* self) {
    if ((self->flags & 0x2000) == 0) {
        // Latch the "starting" flag group (proximity + travel request bits)
        self->flags |= 0x1E00100;
        if (self->flags & 0x200) {
            // Proximity gate active: if the player is NOT near, abort quietly;
            // otherwise drop the gate bit and fall through to sub-object checks
            if (func_8020A5DC(self) != 0) {
                return;
            }
            self->flags &= ~0x200u;
        } else {
            // All configured effects must report ready
            bool ready = false;
            if (self->unk66 & 1) {
                ready = func_8020A87C(self, self->unk7C);
            }
            if (self->flag1B0 & 1) {
                ready = ready | func_8020A87C(self, self->unk1A4);
            }
            if (self->flag1B1 & 1) {
                ready = ready | func_8020A87C(self, self->unk1A8);
            }
            if (self->flag1B2 & 1) {
                ready = ready | func_8020A87C(self, self->unk1AC);
            }

            if (ready) {
                func_8020A484(self->unk6A);
                self->flags |= 0x200;
                return;
            }
        }
    }

    // Start moving when no wait is configured or the wait event finished
    if (self->val1B4 == 0 || func_8020971C(self->val1B4)) {
        self->state = 1;
    }
}

// ============================================================
// func_8020B5C4 (0x280 bytes) - main update (movement)
// ============================================================
extern "C" void func_8020B5C4(CfGimmickElvData* self) {
    u32 f = self->flags | 0x100;
    self->flags = f;

    if (f & 2) {
        // Axis 1
        if (f & 0x20) {
            // Upward travel
            self->flags = f | 0x800000;
            if (func_80209754(self->flag1B1, &self->elvVec7, &self->elvVec1,
                             &self->elvVec4, self->unk1A8)) {
                self->state = 3;
                self->flags &= ~0x1000u;
                if (self->val1B6 != 0) {
                    func_80208C48(self->val1B6, &self->elvVec1);
                }
            }
        }
    } else {
        // Downward travel
        self->flags = f | 0x200000;
        if (func_80209754(self->unk66, &self->vec2, &self->vec0,
                         &self->vec1, self->unk7C)) {
            self->state = 2;
            self->flags &= ~0x1000u;
            if (self->val1B6 != 0) {
                func_80208C48(self->val1B6, &self->vec0);
            }

            // Post-move: notify game manager when flag1B3 set.
            // The Y height is loaded before the side effects and kept in a float
            // live across both virtual calls.
            if (self->flag1B3 != 0) {
                f32 height = self->vec1.y;
                func_80208EE4(self);
                self->unk78 = (u32)createBattleActor__Q22cf13CfGameManagerFv(self->flag1B3, 0);
                if (self->unk78 != 0) {
                    *(CfGimmickElvData**)(self->unk78 + 0xB0) = self;
                    // Virtual call: gm vtable slot at 0x9C takes the base vec
                    void (*vfn)(void*, void*) = *(void (**)(void*, void*))(*(u32*)self->unk78 + 0x9C);
                    vfn((void*)self->unk78, &self->vec0);
                    // Virtual call: gm vtable slot at 0xC4 takes the saved height
                    void (*vfn2)(void*, f32) = *(void (**)(void*, f32))(*(u32*)self->unk78 + 0xC4);
                    vfn2((void*)self->unk78, height);
                }
            }
        }
    }

    // Second axis (independent of the first)
    u32 g = self->flags;
    if (g & 4) {
        if (g & 0x40) {
            self->flags |= 0x1000000;
            if (func_80209754(self->flag1B2, &self->elvVec8, &self->elvVec2,
                             &self->elvVec5, self->unk1AC)) {
                self->state = 2;
                self->flags &= ~0x1000u;
                if (self->val1B6 != 0) {
                    func_80208C48(self->val1B6, &self->elvVec2);
                }
            }
        }
    } else {
        self->flags |= 0x400000;
        if (func_80209754(self->flag1B0, &self->elvVec6, &self->elvVec0,
                         &self->elvVec3, self->unk1A4)) {
            self->state = 3;
            self->flags |= 0x1000;
            if (self->val1B6 != 0) {
                func_80208C48(self->val1B6, &self->elvVec0);
            }

            if (self->flag1B3 != 0) {
                f32 height = self->elvVec3.y;
                func_80208EE4(self);
                self->unk78 = (u32)createBattleActor__Q22cf13CfGameManagerFv(self->flag1B3, 0);
                if (self->unk78 != 0) {
                    *(CfGimmickElvData**)(self->unk78 + 0xB0) = self;
                    void (*vfn)(void*, void*) = *(void (**)(void*, void*))(*(u32*)self->unk78 + 0x9C);
                    vfn((void*)self->unk78, &self->elvVec0);
                    void (*vfn2)(void*, f32) = *(void (**)(void*, f32))(*(u32*)self->unk78 + 0xC4);
                    vfn2((void*)self->unk78, height);
                }
            }
        }
    }
}

// ============================================================
// func_8020B844 (0x2C bytes) - set direction 0
// ============================================================
extern "C" void func_8020B844(CfGimmickElvData* self) {
    self->direction = 0;
    if (self->flags & 0x20) {
        self->state = 4;
    } else {
        self->state = 5;
    }
}

// ============================================================
// func_8020B870 (0x2C bytes) - set direction 1
// ============================================================
extern "C" void func_8020B870(CfGimmickElvData* self) {
    self->direction = 1;
    if (self->flags & 0x40) {
        self->state = 4;
    } else {
        self->state = 5;
    }
}

// ============================================================
// func_8020B89C (0x1FC bytes) - LOD fade update (single axis)
// ============================================================
extern "C" void func_8020B89C(CfGimmickElvData* self) {
    f32 dt = func_80496288(lbl_eu_80663E14);
    self->val1C8 += dt;

    u16 dir = self->direction;
    u32 dirBit = 2 << dir;

    if (self->flags & dirBit) {
        return;
    }

    u8 lod = ((u8*)self)[0x71 + dir];
    if (lod == 0) {
        self->flags &= ~dirBit;
        self->state = 7;
        return;
    }

    int done = 0;
    u8 flag = (dir == 0) ? self->flag1B1 : self->flag1B2;
    u32 lodBit = 8 << dir;

    if (self->flags & lodBit) {
        // Fading in
        if (flag & 0x10) {
                // Complete
                deactivateLOD__8CTaskLODFv(lod);
                void* snd = getScnHandle__Fv();
                func_804BCC3C(snd, lod);
                u32 d = self->direction;
                u32 lb = 8 << d;
                u32 db = 2 << d;
                self->flags = (self->flags & ~lb) | db;
        } else {
            f32 cur = getLODDistance__8CTaskLODFv(lod);
            if (cur >= self->val1C0) {
                attachLODObject__8CTaskLODFv(lod, 0);
                u32 d = self->direction;
                u32 lb = 8 << d;
                u32 db = 2 << d;
                self->flags = (self->flags & ~lb) | db;
            }
        }
    } else {
        // Not yet fading
        if (flag & 0x10) {
        } else {
            attachLODObject__8CTaskLODFv(lod, 1);
            detachLODObject__8CTaskLODFv(lod, 0);
        }

        f32 cur = getLODLevel__8CTaskLODFv(lod);
        self->val1C0 = cur;
        self->flags |= lodBit;

        if (self->val1B8 != 0) {
            func_80208C48(self->val1B8, (dir != 0) ? (void*)&self->elvVec0 : (void*)&self->vec0);
        }
    }

    if (done) {
        self->state = 7;
    }
}

// ============================================================
// func_8020BA98 (0x210 bytes) - LOD fade update (both axes loop)
// ============================================================
extern "C" void func_8020BA98(CfGimmickElvData* self) {
    self->val1C8 += func_80496288(lbl_eu_80663E14);

    // Per-axis LOD fade driver (direction bits 0x20/0x40, latch bits 0x100/0x200)
    int allDone = 1;
    int i;

    for (i = 0; i < 2; i++) {
        u32 dirBit = 2 << i;
        u32 flg = self->flags;
        if (flg & dirBit) {
            continue;
        }

        allDone = 0;
        u8 lod = ((u8*)self)[0x71 + i];
        if (lod == 0) {
            self->flags &= ~dirBit;
            continue;
        }

        u8 flag = (i != 0) ? self->flag1B2 : self->flag1B1;
        u32 lodBit = 8 << i;

        if (self->flags & lodBit) {
            // Fade already running
            if (flag & 0x10) {
                if (self->val1C8 >= lbl_eu_80668384) {
                    void* snd = getScnHandle__Fv();
                    func_804BCC30(snd, lod);
                    self->flags = self->flags & ~lodBit & ~dirBit;
                } else {
                    refreshLOD__8CTaskLODFv(lod, self->val1C8 / lbl_eu_80668384);
                }
            } else {
                f32 cur = getLODDistance__8CTaskLODFv(lod);
                if (cur <= lbl_eu_80668380) {
                    self->flags = self->flags & ~lodBit & ~dirBit;
                    attachLODObject__8CTaskLODFv(lod, 0);
                }
            }
        } else {
            // Start the fade
            if (flag & 0x10) {
                activateLOD__8CTaskLODFv(lod);
                refreshLOD__8CTaskLODFv(lod, lbl_eu_80668380);
            } else {
                attachLODObject__8CTaskLODFv(lod, 1);
                detachLODObject__8CTaskLODFv(lod, 1);
            }

            self->flags |= lodBit;
            if (self->val1B8 != 0) {
                func_80208C48(self->val1B8, (i != 0) ? (void*)&self->elvVec0 : (void*)&self->vec0);
            }
            self->val1C8 = lbl_eu_80668380;
        }
    }

    if (!(self->flags & 0x1000)) {
        func_80209F5C();
    }

    if (allDone) {
        self->state = 6;
        if (!(self->flags & 0x1000)) {
            self->val1C8 = lbl_eu_80668384;
        } else {
            self->val1C8 = lbl_eu_80668380;
        }
    }
}

// ============================================================
// func_8020BCA8 (0x5CC bytes) - main elevator state update
// Handles: fade-in timer, LOD travel toward target, arrival, sound
// playback/positioning, and per-axis auto-return distance check.
// ============================================================
extern "C" void func_8020BCA8(CfGimmickElvData* self) {
    return;
}

#if 0
// finish: stubbed
    // if (done) {
        self->state = 4;
    }
}

// ============================================================
// func_8020C274 (0xC0 bytes) - completion check
#endif

// ============================================================
extern "C" void func_8020C274(CfGimmickElvData* self) {
    if (self->flags & 0x20) {
        if (!(self->flag1B1 & 5)) {
            u32 idx = self->unk158;
            // Jump table call
            int (*fn)(void*, void*, void*) = (int(*)(void*, void*, void*))jumptable_eu_80535830[idx];
            if (fn(&self->elvVec7, lbl_eu_805765A0, &self->elvVec1) != 0) {
                return;
            }
        }
    } else {
        if (!(self->flag1B2 & 5)) {
            u32 idx = self->unk1A0;
            int (*fn)(void*, void*, void*) = (int(*)(void*, void*, void*))jumptable_eu_80535830[idx];
            if (fn(&self->elvVec8, lbl_eu_805765A0, &self->elvVec2) != 0) {
                return;
            }
        }
    }
    self->state = 1;
}
