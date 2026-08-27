/**
 * @file CfGimmickItem.cpp
 * @brief Translation unit for cf::CfGimmickItem (item gimmick).
 *
 * Functions decompiled from the retail ASM at build/us/asm/kyoshin/cf/.
 * The class carries a manual vtable + replicated CfGimmick base layout
 * (no C++ inheritance), so the base destructor is invoked by name and
 * the retail vtable / PTMF tables are referenced as data symbols.
 */

#include "kyoshin/cf/CfGimmickItem.hpp"

// BDAT column result: retail spills each raw call result to a word frame
// slot and narrows on use (lbz/lhz); a volatile word union reproduces that.
union BdatCol {
    u32 w;
    u8 b;
    u16 h;
};


// ---------------------------------------------------------------------------
// Constructor / Destructor
// ---------------------------------------------------------------------------

cf::CfGimmickItem::~CfGimmickItem() {
    this->vtable = (void*)lbl_eu_80535A98;
    func_80208EE4((void*)this);
    func_8020A434(&this->field_7C);
    __dt__Q22cf9CfGimmickFv((void*)this, 0);
    // MWCC appends the deleting-dtor prologue (null guard) and epilogue
    // (delete-flag ? operator delete(this) : skip) automatically.
}

// ---------------------------------------------------------------------------
// Constructor -- BDAT columns populate the item fields; sub-objects init
// (retail symbol is the raw __ct__cf_CfGimmickItem name, so it is emitted
// with C linkage like the sibling CfGimmickElv ctor).
// ---------------------------------------------------------------------------

extern "C" cf::CfGimmickItem* __ct__cf_CfGimmickItem(cf::CfGimmickItem* self,
                                                     u16 rowId) {
    __ct__cf_CfGimmick((void*)self);
    self->vtable = (void*)lbl_eu_80535A98;
    self->field_82 = 6;

    void* mgr = func_8003AA34();
    // Retail keeps the bdat table handle in a dedicated frame slot and
    // reloads it for every column read (its address escapes below).
    u32 bdat = lbl_eu_8066413C;
    self->field_64 = rowId;

    // Init the three sub-objects (placement vec, pad10, vobj).
    func_80208F34(self, &self->vvec04, mgr, (void*)&bdat);
    func_80209020(self, &self->vobj, mgr, (void*)&bdat);
    func_80209288(self, &self->pad10, mgr, (void*)&bdat);

    // Column name buffers "A_Item"/"A_Lost" get the slot letter written in
    // (the table pointer holds them at lbl_eu_806627B8[0]/[1]).
    volatile BdatCol c66;
    c66.w = getBdatStringColumnValue((void*)bdat, lbl_eu_805087AC, rowId);
    self->field_66 = c66.b;
    for (int i = 0; i < 3; ++i) {
        char c = (char)('A' + i);
        lbl_eu_806627B8[1][0] = c;
        lbl_eu_806627B8[0][0] = c;
        volatile BdatCol cid;
        cid.w = getBdatStringColumnValue((void*)bdat, lbl_eu_806627B8[0], rowId);
        self->field_84[i] = cid.h;
        volatile BdatCol clost;
        clost.w = getBdatStringColumnValue((void*)bdat, lbl_eu_806627B8[1], rowId);
        if (clost.b != 0) {
            self->field_74 |= (1u << i);
        }
    }

    volatile BdatCol c8A;
    c8A.w = getBdatStringColumnValue((void*)bdat, lbl_eu_805087AC + 0x08, rowId);
    self->field_8A = c8A.h;
    volatile BdatCol c6A;
    c6A.w = getBdatStringColumnValue((void*)bdat, lbl_eu_805087AC + 0x0F, rowId);
    self->field_6A = c6A.h;
    volatile BdatCol c8C;
    c8C.w = getBdatStringColumnValue((void*)bdat, lbl_eu_805087AC + 0x14, rowId);
    self->field_8C = c8C.h;
    volatile BdatCol c98;
    c98.w = getBdatStringColumnValue((void*)bdat, lbl_eu_805087AC + 0x19, rowId);
    self->field_98 = c98.b;
    volatile BdatCol c70;
    c70.w = getBdatStringColumnValue((void*)bdat, lbl_eu_805087AC + 0x1E, rowId);
    self->field_70 = c70.b;
    volatile BdatCol c99;
    c99.w = getBdatStringColumnValue((void*)bdat, lbl_eu_805087AC + 0x25, rowId);
    self->field_99 = c99.b;
    volatile BdatCol c9A;
    c9A.w = getBdatStringColumnValue((void*)bdat, lbl_eu_805087AC + 0x2C, rowId);
    self->field_9A = c9A.b;
    volatile BdatCol c9B;
    c9B.w = getBdatStringColumnValue((void*)bdat, lbl_eu_805087AC + 0x34, rowId);
    self->field_9B = c9B.b;
    volatile BdatCol c92;
    c92.w = getBdatStringColumnValue((void*)bdat, lbl_eu_805087AC + 0x38, rowId);
    self->field_92 = c92.h;
    volatile BdatCol c8E;
    c8E.w = getBdatStringColumnValue((void*)bdat, lbl_eu_805087AC + 0x3E, rowId);
    self->field_8E = c8E.h;
    volatile BdatCol c90;
    c90.w = getBdatStringColumnValue((void*)bdat, lbl_eu_805087AC + 0x44, rowId);
    self->field_90 = c90.h;
    volatile BdatCol c9C;
    c9C.w = getBdatStringColumnValue((void*)bdat, lbl_eu_805087AC + 0x4A, rowId);
    self->field_9C = c9C.b;
    volatile BdatCol c6C;
    c6C.w = getBdatStringColumnValue((void*)bdat, ((char**)lbl_eu_805357E8)[13], rowId);
    self->field_6C = c6C.h;
    volatile BdatCol c6E;
    c6E.w = getBdatStringColumnValue((void*)bdat, ((char**)lbl_eu_805357E8)[14], rowId);
    self->field_6E = c6E.h;
    volatile BdatCol c94;
    c94.w = getBdatStringColumnValue((void*)bdat, ((char**)lbl_eu_805357E8)[15], rowId);
    self->field_94 = c94.h;
    volatile BdatCol c96;
    c96.w = getBdatStringColumnValue((void*)bdat, lbl_eu_805087AC + 0x4D, rowId);
    self->field_96 = c96.b;
    volatile BdatCol c97;
    c97.w = getBdatStringColumnValue((void*)bdat, lbl_eu_805087AC + 0x57, rowId);
    self->field_97 = c97.b;

    // vtable slot 0x20 virtual call.
    ((void (***)(cf::CfGimmickItem*))self)[0][8](self);

    self->field_9E = 0;
    self->field_A0 = 0;
    if (self->field_70 != 0) {
        attachLODObject__8CTaskLODFv(self->field_70, 0);
        removeLODEntry__8CTaskLODFv(self->field_70, lbl_eu_80668448);
    }

    if (func_8020971C(self->field_64) != 0 && self->field_9C == 3) {
        self->field_9E = 5;
    }
    return self;
}

// ---------------------------------------------------------------------------
// func_80210668 -- PTMF state dispatch + one-frame spawn/cleanup
// ---------------------------------------------------------------------------

// Field-74 state flag bits (retail rlwinm masks).
enum {
    kItemFlagWork = 0x10,   // "working" flag set/cleared during the move step
    kItemFlagBusy = 0x20,   // actor busy flag (spawned/attached)
};

void func_80210668(cf::CfGimmickItem* self) {
    // Dispatch the current state through the 12-byte PTMF table.
    (self->*lbl_eu_80535A50[self->field_9E])();

    if (self->field_74 & kItemFlagWork) {
        if (self->field_66 & 1) {
            // Attach an item object at the placement point while working.
            func_8020A6B0(&self->field_7C, &self->vvec04, self->field_6A,
                          lbl_eu_8066844C, 0, 0);
        }
        self->field_74 &= ~kItemFlagWork;
    } else {
        func_8020A434(&self->field_7C);
    }
}

// ---------------------------------------------------------------------------
// func_80210844 -- busy-flag handled item activation; falls back to state 4
// ---------------------------------------------------------------------------

void func_80210844(cf::CfGimmickItem* self) {
    func_80209F2C();

    // a busy actor only stays when func_8020A5DC still reports work, and an
    // idle one is (re)spawned via func_8020A484 + busy bit.  The two fail
    // paths branch to the shared `zero` merge (single `li r3,0`); the ready
    // paths set ok=1 and jump straight to the test.
    u32 ok;
    if ((ok = self->field_6A) == 0) {
        goto st4;
    }
    if (self->field_74 & kItemFlagBusy) {
        if (func_8020A5DC() != 0) {
            ok = 1;
        } else {
            self->field_74 &= ~kItemFlagBusy;
            goto zero;
        }
    } else {
        func_8020A484(self->field_6A);
        self->field_74 |= kItemFlagBusy;
        ok = 1;
    }
    goto test;

zero:
    ok = 0;
test:
    if (ok == 0) {
st4:
        self->field_9E = 4;
    }
}

// ---------------------------------------------------------------------------
// func_80210AD0 -- detect what can be collected and move to the next state
// ---------------------------------------------------------------------------

void func_80210AD0(cf::CfGimmickItem* self) {
    for (int i = 0; i < 3; ++i) {
        if (self->field_84[i] != 0 && (self->field_74 & (1 << i))) {
            func_80159C04(self->field_84[i], 1);
        }
    }

    if (self->field_8A != 0) {
        func_801586D4(self->field_8A, 1);
    }

    if (self->field_9C == 3) {
        self->field_9E = 5;
    } else if (self->field_9C == 2) {
        self->field_9E = 5;
    } else {
        self->field_9E = 4;
    }

    if (self->field_64 != 0) {
        func_8020974C(self->field_64, 1);
    }
}

// ---------------------------------------------------------------------------
// func_80210BAC -- placement dispatch; on miss reset the item state
// ---------------------------------------------------------------------------

void func_80210BAC(cf::CfGimmickItem* self) {
    if ((self->field_66 & 1) != 0 ||
        jumptable_eu_80535830[self->mType](
            (cf::CfGimmick*)&self->vobj, &lbl_eu_805765A0, &self->vvec04) == 0) {
        self->field_9E = 0;
    }
}

// ---------------------------------------------------------------------------
// func_802106F8 -- working-frame update: range window, respawn gate, spawn
// ---------------------------------------------------------------------------

void func_802106F8(cf::CfGimmickItem* self) {
    // Only run while the current sequence counter sits inside the item's
    // active window [field_6C, field_6E] (either bound non-zero enables it).
    if (self->field_6C != 0 || self->field_6E != 0) {
        u32 seq = getQueuedFileEventCount__Q22cf13CfGameManagerFv();
        if (self->field_6C > seq || seq > self->field_6E)
            return;
    }

    // Respawn-gate: when field_94 is set, the current count must sit inside
    // the [field_96, field_97] window or the spawn is skipped.
    if (self->field_94 != 0) {
        int count = (int)getResourceFromTable__Q22cf13CfGameManagerFv(self->field_94);
        if (self->field_97 > count || self->field_96 < count)
            return;
    }

    self->field_74 |= 0x10;
    if (func_80209754(self->field_66, &self->vobj, &self->vvec04,
                      &self->pad10, (u32)self->field_7C.field_00) == 0)
        return;

    // Every non-empty item slot must be collectible, otherwise abort.
    int ok = 0;
    for (int i = 0; i < 3; ++i) {
        if (self->field_84[i] != 0) {
            if (func_801587E8(self->field_84[i]) == 0) {
                ok = 0;
                break;
            }
            ok = 1;
        }
    }

    // While toggled-on, fire the per-frame effect at the placement point.
    if (self->field_66 & 1) {
        if (self->field_8E != 0) {
            func_80208C48((void*)(u32)self->field_8E, &self->vvec04);
        }
    }

    if (ok) {
        self->field_9E = 2;
    } else {
        self->field_9E = 1;
    }
}

// ---------------------------------------------------------------------------
// func_802108D8 -- per-frame LOD/effect/manager update + countdown reset
// ---------------------------------------------------------------------------

void func_802108D8(cf::CfGimmickItem* self) {
    func_80209F2C();

    // While the 0x8 busy flag is clear, run the per-frame LOD filters, fire
    // the field_90 effect, link the area manager and play the field_92 sound.
    if ((self->field_74 & 8) == 0) {
        u8 fc;
        u8 lod;
        if ((lod = self->field_70) != 0 && (fc = self->field_9C) != 0) {
            // Mode bits 0..3 select the CTaskLOD filter calls (same shape as
            // the CfGimmickEne per-LOD filter block).
            if ((fc & 4) != 0) {
                if ((fc & 2) != 0) {
                    removeLODEntry__8CTaskLODFv(lod, lbl_eu_80668448);
                } else {
                    clearLODEntry__8CTaskLODFv(lod);
                }
                attachLODObject__8CTaskLODFv(lod, 0);
            } else if ((fc & 1) != 0) {
                detachLODObject__8CTaskLODFv(lod, 0);
                attachLODObject__8CTaskLODFv(lod, 1);
            } else if ((fc & 2) != 0) {
                detachLODObject__8CTaskLODFv(lod, 1);
                attachLODObject__8CTaskLODFv(lod, 1);
            }
            if ((fc & 8) != 0) {
                addLODEntry__8CTaskLODFv(lod, 0);
            } else {
                addLODEntry__8CTaskLODFv(lod, 1);
            }
            setLODObject__8CTaskLODFv(lod, self->field_99);
        }

        self->field_74 |= 8;
        if (self->field_90 != 0) {
            func_80208C48((void*)(u32)self->field_90, &self->vvec04);
        }

        if (self->field_9B != 0) {
            UnkClass_800817BC* obj =
                createBattleActor__Q22cf13CfGameManagerFv(self->field_9B, 0);
            self->field_78 = (u32)obj;
            if (obj != 0) {
                ((cf::CfGimmickItemMgr*)obj)->field_B0 = self;
            }
        }

        if (self->field_92 != 0) {
            func_8007B0C8(self->field_92);
        }
    }

    // Countdown: while field_A0 < field_98 just increment; once the window
    // expires, keep the item alive only if a busy actor still reports work
    // (same ok/zero merge as func_80210844), otherwise reset to state 3.
    if (self->field_A0 < self->field_98) {
        self->field_A0++;
    } else {
        int ok;
        if (self->field_8C == 0) {
            goto rst;
        }
        if (self->field_74 & kItemFlagBusy) {
            if (func_8020A5DC() != 0) {
                ok = 1;
            } else {
                self->field_74 &= ~kItemFlagBusy;
                goto zero;
            }
        } else {
            func_8020A484(self->field_8C);
            self->field_74 |= kItemFlagBusy;
            ok = 1;
        }
        goto test;
    zero:
        ok = 0;
    test:
        if (ok == 0) {
    rst:
            self->field_74 &= ~8;
            self->field_A0 = 0;
            self->field_9E = 3;
        }
    }
}

// ---------------------------------------------------------------------------
// FULL_MATCH: Virtual function override -- no-op
// ---------------------------------------------------------------------------

extern "C" void func_80210C1C() {}