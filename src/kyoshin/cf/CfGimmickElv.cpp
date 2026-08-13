// CfGimmickElv - elevator gimmick (cf namespace)
// Decompiled from retail ASM at build/us/asm/kyoshin/cf/CfGimmickElv.s

#include "kyoshin/cf/CfGimmickElv.hpp"

// External globals
extern "C" {
extern u8 lbl_eu_805358C8[];   // CfGimmickElv vtable
extern u8 lbl_eu_805357E8[];   // bdat column pointer table
extern u8 lbl_eu_8050867C[];   // bdat column name strings
extern u32 lbl_eu_80664130;    // bdat table pointer (via sda21)
extern u32 lbl_eu_806646BC;    // global flag
typedef void (CfGimmickElvData::*CfGimmickElvStateFunc)();
extern CfGimmickElvStateFunc lbl_eu_80535868[];   // PTMF state table
extern u32 lbl_eu_805765B0[];  // bit array
extern u32 lbl_eu_805765A0[];  // vec3 constants
extern void* lbl_eu_80663E14;  // global pointer (via sda21)
extern f32 lbl_eu_80668380;    // float constant (0.0f)
extern f32 lbl_eu_80668384;    // float constant (1.0f)
extern f64 lbl_eu_80668388;    // double constant
extern f32 lbl_eu_80668390;    // float constant
extern f32 lbl_eu_80668394;    // float constant
extern f32 lbl_eu_80668398;    // float constant
extern f32 lbl_eu_8066839C;    // float constant
extern f32 lbl_eu_806683A0;    // float constant
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
    u8* colBase = lbl_eu_805357E8;

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
    func_8020938C(self, self->vec0, (void*)lbl_eu_805357E8, &stackVar, 0);
    func_802095D8(self, self->vec1, (void*)lbl_eu_805357E8, &stackVar, 0);
    func_80209488(self, self->vec2, (void*)lbl_eu_805357E8, &stackVar, 0);
    func_8020938C(self, self->elvVec0, (void*)lbl_eu_805357E8, &stackVar, 1);
    func_802095D8(self, self->elvVec3, (void*)lbl_eu_805357E8, &stackVar, 1);
    func_80209488(self, self->elvVec6, (void*)lbl_eu_805357E8, &stackVar, 1);
    func_8020938C(self, self->elvVec1, (void*)lbl_eu_805357E8, &stackVar, 2);
    func_802095D8(self, self->elvVec4, (void*)lbl_eu_805357E8, &stackVar, 2);
    func_80209488(self, self->elvVec7, (void*)lbl_eu_805357E8, &stackVar, 2);
    func_8020938C(self, self->elvVec2, (void*)lbl_eu_805357E8, &stackVar, 3);
    func_802095D8(self, self->elvVec5, (void*)lbl_eu_805357E8, &stackVar, 3);
    func_80209488(self, self->elvVec8, (void*)lbl_eu_805357E8, &stackVar, 3);

    // Virtual call: vtable[8] (offset 0x20)
    void (*vfunc)(CfGimmickElvData*) = *(void(**)(CfGimmickElvData*))(*(u32*)self + 0x20);
    vfunc(self);

    // More bdat columns
    u8* nameBase = lbl_eu_8050867C;
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
        fval = fval - *(f64*)&lbl_eu_80668388;  // subtract double constant
        func_80462FD8__8CTaskLODFv(self->lod0, fval);
    }

    // LOD init loop for lod0..lod2
    int i;
    for (i = 0; i < 3; i++) {
        u8 lod = ((u8*)self)[0x70 + i];
        if (lod != 0) {
            func_80462EF4__8CTaskLODFv(lod, lbl_eu_80668380);
        }
    }

    if (self->lod0 != 0) {
        func_80462F4C__8CTaskLODFv(self->lod0, 0);
        func_80462ED0__8CTaskLODFv(self->lod0, 0);
    }

    // LOD1 setup
    if (self->lod1 != 0) {
        int mode = 0;
        if (self->flag1B1 & 0x10) {
            mode = 1;
            void* snd = func_804BC9EC__Fv();
            func_804BCC30(snd, self->lod1);
            func_80462E3C__8CTaskLODFv(self->lod1, lbl_eu_80668384);
            func_80462D04__8CTaskLODFv(self->lod1);
        }
        func_80462F4C__8CTaskLODFv(self->lod1, mode);
        func_80462ED0__8CTaskLODFv(self->lod1, mode);
    }

    // LOD2 setup
    if (self->lod2 != 0) {
        int mode = 0;
        if (self->flag1B2 & 0x10) {
            mode = 1;
            void* snd = func_804BC9EC__Fv();
            func_804BCC30(snd, self->lod2);
            func_80462E3C__8CTaskLODFv(self->lod2, lbl_eu_80668384);
            func_80462D04__8CTaskLODFv(self->lod2);
        }
        func_80462F4C__8CTaskLODFv(self->lod2, mode);
        func_80462ED0__8CTaskLODFv(self->lod2, mode);
    }

    // Flag setup based on unk66 bits
    if (self->unk66 & 0x08) {  // rlwinm. r0, r0, 0, 28, 28 = bit 3
        if (self->lod0 != 0) {
            func_80462F10__8CTaskLODFv(self->lod0);
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
    self->val1C8 = lbl_eu_80668380;
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
    if (lbl_eu_806646BC & 2) {
        // PTMF call through state table
        (self->*lbl_eu_80535868[self->state])();
        func_8020B34C(self);
    }
}

// ============================================================
// func_8020B264 (0x80 bytes) - LOD visibility toggle
// ============================================================
extern "C" void func_8020B264(CfGimmickElvData* self, int show) {
    if (self->lod0 != 0) {
        self->flags &= ~0x1000E0;
        if (show != 0) {
            self->flags |= 0x40;
            func_80462F10__8CTaskLODFv(self->lod0);
        } else {
            self->flags |= 0x20;
            int lod = self->lod0;
            f32 zero = lbl_eu_80668380;
            func_80462EF4__8CTaskLODFv(lod, zero);
        }
        func_80463014__8CTaskLODFv(self->lod0);
    }
}

// ============================================================
// func_8020B2E4 (0x68 bytes) - copy sub-objects
// ============================================================
extern "C" void func_8020B2E4(CfGimmickElvData* self) {
    func_802089BC(self->vec2, self->vec0, self->vec1);
    func_802089BC(self->elvVec6, self->elvVec0, self->elvVec3);
    func_802089BC(self->elvVec7, self->elvVec1, self->elvVec4);
    func_802089BC(self->elvVec8, self->elvVec2, self->elvVec5);
}

// ============================================================
// func_8020B34C (0x128 bytes) - init sub-objects on demand
// ============================================================
extern "C" void func_8020B34C(CfGimmickElvData* self) {
    // Init unk7C if needed
    if ((self->unk66 & 1) && self->unk7C == 0) {
        self->unk7C = (u32)func_8020A35C((void*)func_8020A608(self->unk6A, 0), 0xF, self->vec0);
    }

    // Init unk1A4 if needed
    if ((self->flag1B0 & 1) && self->unk1A4 == 0) {
        self->unk1A4 = (u32)func_8020A35C((void*)func_8020A608(self->unk6A, 0), 0xF, self->elvVec0);
    }

    // Check flags bit 8 (0x100)
    if (self->flags & 0x100) {
        // LOD1 effect (bit 23)
        if ((self->flag1B1 & 1) && (self->flags & 0x00800000)) {
            func_8020A6B0(&self->unk1A8, self->elvVec1, self->unk6A, lbl_eu_80668390, 1, 0);
        }
        // LOD2 effect (bit 24)
        if ((self->flag1B2 & 1) && (self->flags & 0x01000000)) {
            func_8020A6B0(&self->unk1AC, self->elvVec2, self->unk6A, lbl_eu_80668390, 1, 0);
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
    if (self->flags & 0x2000) {
        // Already active - check completion
        if (self->val1B4 != 0) {
            if (func_8020971C(self) == 0) {
                self->state = 1;
            }
        } else {
            self->state = 1;
        }
        return;
    }

    self->flags |= 0x01E00100;

    if (self->flags & 0x200) {
        // Check if player is near
        if (func_8020A5DC(self) != 0) {
            // Not near - clear flags
            self->flags &= ~0x01E0;
            return;
        }
    }

    int allReady = 0;

    // Check unk66 bit 0
    if (self->unk66 & 1) {
        allReady = (func_8020A87C(self, self->unk7C) != 0) ? 1 : 0;
    }

    // Check flag1B0 bit 0
    if (self->flag1B0 & 1) {
        allReady |= (func_8020A87C(self, self->unk1A4) != 0) ? 1 : 0;
    }

    // Check flag1B1 bit 0
    if (self->flag1B1 & 1) {
        allReady |= (func_8020A87C(self, self->unk1A8) != 0) ? 1 : 0;
    }

    // Check flag1B2 bit 0
    if (self->flag1B2 & 1) {
        allReady |= (func_8020A87C(self, self->unk1AC) != 0) ? 1 : 0;
    }

    if (allReady) {
        func_8020A484(self->unk6A);
        self->flags |= 0x200;
        return;
    }

    // Check val1B4
    if (self->val1B4 != 0) {
        if (func_8020971C(self) == 0) {
            self->state = 1;
        }
    } else {
        self->state = 1;
    }
}

// ============================================================
// func_8020B5C4 (0x280 bytes) - main update (movement)
// ============================================================
extern "C" void func_8020B5C4(CfGimmickElvData* self) {
    self->flags |= 0x100;

    if (self->flags & 2) {
        // Direction 1 (up)
        if (self->flags & 0x20) {
            self->flags |= 0x00800000;
            if (func_80209754(self->flag1B1, self->elvVec7, self->elvVec1,
                             self->elvVec4, self->unk1A8)) {
                self->state = 3;
                self->flags &= ~0x01C00000;
                if (self->val1B6 != 0) {
                    func_80208C48(self, self->elvVec1);
                }
            }
        }
    } else {
        // Direction 0 (down)
        self->flags |= 0x00200000;
        if (func_80209754(self->unk66, self->vec2, self->vec0,
                         self->vec1, self->unk7C)) {
            self->state = 2;
            self->flags |= 0x1000;
            if (self->val1B6 != 0) {
                func_80208C48(self, self->vec0);
            }

            // Post-move: check flag1B3
            if (self->flag1B3 != 0) {
                func_80208EE4(self);
                void* gm = func_800817BC__Q22cf13CfGameManagerFv(self->flag1B3, 0);
                self->unk78 = (u32)gm;
                if (gm != NULL) {
                    *(CfGimmickElvData**)((u8*)gm + 0xB0) = self;
                    // Virtual call: gm->vtable[0x9C/4](gm, self->vec0)
                    void (*vfn)(void*, void*) = *(void(**)(void*, void*))(*(u32*)gm + 0x9C);
                    vfn(gm, self->vec0);
                    // Virtual call: gm->vtable[0xC4/4](gm, self->vec0.y)
                    f32 yval = *(f32*)(self->vec0 + 4);
                    void (*vfn2)(void*, f32) = *(void(**)(void*, f32))(*(u32*)gm + 0xC4);
                    vfn2(gm, yval);
                }
            }
        }
    }

    // Second axis
    if (self->flags & 4) {
        if (self->flags & 0x40) {
            self->flags |= 0x01000000;
            if (func_80209754(self->flag1B2, self->elvVec8, self->elvVec2,
                             self->elvVec5, self->unk1AC)) {
                self->state = 2;
                self->flags &= ~0x01C00000;
                if (self->val1B6 != 0) {
                    func_80208C48(self, self->elvVec2);
                }
            }
        }
    } else {
        self->flags |= 0x00400000;
        if (func_80209754(self->flag1B0, self->elvVec6, self->elvVec0,
                         self->elvVec3, self->unk1A4)) {
            self->state = 3;
            self->flags |= 0x1000;
            if (self->val1B6 != 0) {
                func_80208C48(self, self->elvVec0);
            }

            if (self->flag1B3 != 0) {
                func_80208EE4(self);
                void* gm = func_800817BC__Q22cf13CfGameManagerFv(self->flag1B3, 0);
                self->unk78 = (u32)gm;
                if (gm != NULL) {
                    *(CfGimmickElvData**)((u8*)gm + 0xB0) = self;
                    void (*vfn)(void*, void*) = *(void(**)(void*, void*))(*(u32*)gm + 0x9C);
                    vfn(gm, self->elvVec0);
                    f32 yval = *(f32*)(self->elvVec0 + 4);
                    void (*vfn2)(void*, f32) = *(void(**)(void*, f32))(*(u32*)gm + 0xC4);
                    vfn2(gm, yval);
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
            if (self->val1C8 >= lbl_eu_80668394) {
                // Complete
                func_80462D5C__8CTaskLODFv(lod);
                void* snd = func_804BC9EC__Fv();
                func_804BCC3C(snd, lod);
                u32 d = self->direction;
                u32 lb = 8 << d;
                u32 db = 2 << d;
                self->flags = (self->flags & ~lb) | db;
            } else {
                f32 t = self->val1C8 / lbl_eu_80668394;
                func_80462E3C__8CTaskLODFv(lod, lbl_eu_80668384 - t);
            }
        } else {
            f32 cur = func_80462F2C__8CTaskLODFv(lod);
            if (cur >= self->val1C0) {
                func_80462F4C__8CTaskLODFv(lod, 0);
                u32 d = self->direction;
                u32 lb = 8 << d;
                u32 db = 2 << d;
                self->flags = (self->flags & ~lb) | db;
            }
        }
    } else {
        // Not yet fading
        if (flag & 0x10) {
            func_80462E3C__8CTaskLODFv(lod, lbl_eu_80668384);
        } else {
            func_80462F4C__8CTaskLODFv(lod, 1);
            func_80462F70__8CTaskLODFv(lod, 0);
        }

        f32 cur = func_80462FF4__8CTaskLODFv(lod);
        self->val1C0 = cur;
        self->val1C8 = lbl_eu_80668380;
        self->flags |= lodBit;

        if (self->val1B8 != 0) {
            void* vec = (dir != 0) ? (void*)self->elvVec0 : (void*)self->vec0;
            func_80208C48(self, vec);
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
    f32 dt = func_80496288(lbl_eu_80663E14);
    self->val1C8 += dt;

    f32 fadeTime = lbl_eu_80668394;
    f32 zero = lbl_eu_80668380;
    int allDone = 1;
    int i;

    for (i = 0; i < 2; i++) {
        u32 dirBit = 2 << i;
        if (!(self->flags & dirBit)) {
            u8 lod = ((u8*)self)[0x71 + i];
            if (lod == 0) {
                self->flags &= ~dirBit;
                continue;
            }

            allDone = 0;
            u8 flag = (i == 0) ? self->flag1B1 : self->flag1B2;
            u32 lodBit = 8 << i;

            if (self->flags & lodBit) {
                if (flag & 0x10) {
                    if (self->val1C8 >= fadeTime) {
                        void* snd = func_804BC9EC__Fv();
                        func_804BCC30(snd, lod);
                        func_80462E3C__8CTaskLODFv(lod, lbl_eu_80668384);
                        self->flags &= ~lodBit;
                        self->flags &= ~dirBit;
                    } else {
                        f32 t = self->val1C8 / fadeTime;
                        func_80462E3C__8CTaskLODFv(lod, t);
                    }
                } else {
                    f32 cur = func_80462F2C__8CTaskLODFv(lod);
                    if (cur <= zero) {
                        self->flags &= ~lodBit;
                        self->flags &= ~dirBit;
                        func_80462F4C__8CTaskLODFv(lod, 0);
                    }
                }
            } else {
                if (flag & 0x10) {
                    func_80462D04__8CTaskLODFv(lod);
                    func_80462E3C__8CTaskLODFv(lod, lbl_eu_80668380);
                } else {
                    func_80462F4C__8CTaskLODFv(lod, 1);
                    func_80462F70__8CTaskLODFv(lod, 1);
                }

                self->flags |= lodBit;
                if (self->val1B8 != 0) {
                    void* vec = (i != 0) ? (void*)self->elvVec0 : (void*)self->vec0;
                    func_80208C48(self, vec);
                }
                self->val1C8 = zero;
            }
        }
    }

    if (!(self->flags & 0x1000)) {
        func_80209F5C(self);
    }

    if (allDone) {
        self->state = 6;
        if (!(self->flags & 0x1000)) {
            self->val1C8 = lbl_eu_80668394;
        } else {
            self->val1C8 = lbl_eu_80668380;
        }
    }
}

// ============================================================
// func_8020BCA8 (0x5CC bytes) - main state update (complex)
// Stub for now - will iterate with hexdiff
// ============================================================
extern "C" void func_8020BCA8(CfGimmickElvData* self) {
    u32 dirBit = 0x20 << self->direction;
    if (self->flags & dirBit) {
        self->state = 4;
        self->val1C8 = lbl_eu_80668380;
        return;
    }

    f32 dt = func_80496288(lbl_eu_80663E14);
    self->val1C8 -= dt;
    if (self->val1C8 > lbl_eu_80668380) {
        func_80209F5C(self);
    } else {
        self->val1C8 = lbl_eu_80668380;
    }

    u8 lod0 = self->lod0;
    if (lod0 == 0) {
        self->flags &= ~(0x20 << self->direction);
        self->state = 4;
        self->val1C8 = lbl_eu_80668380;
        return;
    }

    int arrived = 0;

    if (self->flags & 0x80) {
        // Moving up
        f32 cur = func_80462F2C__8CTaskLODFv(lod0);
        f32 target = self->val1C0;

        if (self->direction != 0) {
            if (cur >= target) arrived = 1;
            if (self->flags & 0x02000000) {
                if (self->flags & 0x4000) {
                    f32 spd = func_80496288(lbl_eu_80663E14);
                    cur = lbl_eu_80668398 * cur + target;
                } else {
                    f32 spd = func_80496288(lbl_eu_80663E14);
                    cur = target + cur;
                }
                if (cur > self->val1C0) cur = self->val1C0;
                func_80462EF4__8CTaskLODFv(lod0, cur);
            }
        } else {
            if (cur <= lbl_eu_80668380) arrived = 1;
            if (self->flags & 0x02000000) {
                if (self->flags & 0x4000) {
                    f32 spd = func_80496288(lbl_eu_80663E14);
                    cur = target - lbl_eu_80668398 * cur;
                } else {
                    f32 spd = func_80496288(lbl_eu_80663E14);
                    cur = target - cur;
                }
                if (cur < lbl_eu_80668380) cur = lbl_eu_80668380;
                func_80462EF4__8CTaskLODFv(lod0, cur);
            }
        }

        if (arrived) {
            // Arrived at destination
            if (self->flags & 0x400) {
                func_80462F94__8CTaskLODFv(self->lod3, self->val1D6);
            }

            self->flags = (self->flags & ~0x020FC000) | (0x20 << self->direction);
            func_80462F4C__8CTaskLODFv(lod0, 0);

            if (self->val1BE != 0) {
                void* vec = (self->direction != 0) ? (void*)self->elvVec0 : (void*)self->vec0;
                func_80208C48(self, vec);
            }

            if (self->unk80 != 0) {
                func_801BFED0(1, self->unk80, 0xA);
                self->unk80 = 0;
            }
        } else {
            // Check sound
            if (self->flags & 0x00100000) {
                if (func_801BFABC(1) != 0) {
                    self->flags &= ~0x001C0000;
                    void* snd = func_804BC9EC__Fv();
                    func_804BCC54(snd, self->val1D4);
                    if (snd != NULL) {
                        f32 pos[3];
                        pos[0] = *(f32*)((u8*)snd + 0x0C);
                        pos[1] = *(f32*)((u8*)snd + 0x1C);
                        pos[2] = *(f32*)((u8*)snd + 0x2C);
                        u16 sfxId = func_80208C60(self, self->val1BC);
                        self->unk80 = sfxId;
                        if (sfxId != 0) {
                            func_801BFF78(1, sfxId, 0x10);
                        }
                    } else {
                        func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(1, self->val1BC, 0, 0, lbl_eu_80668384);
                        self->unk80 = 1;
                        self->val1D4 = 0xFFFF;
                    }
                }
            }

            // Update sound position
            if (self->unk80 != 0 && self->val1D4 != 0xFFFF) {
                void* snd = func_804BC9EC__Fv();
                func_804BCC54(snd, self->val1D4);
                if (snd != NULL) {
                    f32 pos[3];
                    pos[0] = *(f32*)((u8*)snd + 0x0C);
                    pos[1] = *(f32*)((u8*)snd + 0x1C);
                    pos[2] = *(f32*)((u8*)snd + 0x2C);
                    func_801BFAE8(self->unk80, pos);
                }
            }

            // Check timer
            if (self->flags & 0x400) {
                f32 spd = func_80496288(lbl_eu_80663E14);
                self->val1CC += spd;
                if (self->val1CC >= lbl_eu_80668394) {
                    self->val1CC = lbl_eu_80668380;
                    if (self->flags & 0x800) {
                        func_80462F94__8CTaskLODFv(self->lod3, self->val1D6);
                        if (self->val1B6 != 0) {
                            self->flags &= ~0x01C00000;
                            // Distance check for auto-reverse
                            // ... complex vec3 distance calculation
                        }
                    } else {
                        func_80462F94__8CTaskLODFv(self->lod3, 5);
                        self->flags |= 0x800;
                    }
                }
            }
        }
    } else {
        // Not moving - init
        func_80462F4C__8CTaskLODFv(lod0, 1);
        if (self->direction != 0) {
            func_80462F70__8CTaskLODFv(lod0, 0);
            f32 cur = func_80462FF4__8CTaskLODFv(lod0);
            self->val1C0 = cur;
        } else {
            func_80462F70__8CTaskLODFv(lod0, 1);
        }

        self->flags |= 0x80;

        if (self->val1BA != 0) {
            void* vec = (self->direction == 0) ? (void*)self->elvVec0 : (void*)self->vec0;
            func_80208C48(self, vec);
        }

        if (self->val1BC != 0) {
            if (self->unk80 != 0) {
                func_801BFED0(1, self->unk80, 0xA);
                self->unk80 = 0;
            }
            void* snd = func_804BC9EC__Fv();
            func_804BCC6C(snd, lod0);
            self->val1D4 = (u16)(u32)snd;
            self->flags |= 0x00100000;
        }

        if (self->flags & 0x1000) {
            if (self->lod3 != 0) {
                u16 v = func_80462FB8__8CTaskLODFv(self->lod3);
                self->val1D6 = v;
                self->val1CC = lbl_eu_80668394;
                self->flags |= 0x400;
            }
            self->flags = (self->flags & ~0x01C00000) | 0x00200000;
        }
    }

    self->state = 4;
    self->val1C8 = lbl_eu_80668380;
}

// ============================================================
// func_8020C274 (0xC0 bytes) - completion check
// ============================================================
extern "C" void func_8020C274(CfGimmickElvData* self) {
    if (self->flags & 0x20) {
        if (!(self->flag1B1 & 5)) {
            u32 idx = self->unk158;
            // Jump table call
            int (*fn)(void*, void*, void*) = (int(*)(void*, void*, void*))jumptable_eu_80535830[idx];
            if (fn(self->elvVec7, lbl_eu_805765A0, self->elvVec1) != 0) {
                return;
            }
        }
    } else {
        if (!(self->flag1B2 & 5)) {
            u32 idx = self->unk1A0;
            int (*fn)(void*, void*, void*) = (int(*)(void*, void*, void*))jumptable_eu_80535830[idx];
            if (fn(self->elvVec8, lbl_eu_805765A0, self->elvVec2) != 0) {
                return;
            }
        }
    }
    self->state = 1;
}
