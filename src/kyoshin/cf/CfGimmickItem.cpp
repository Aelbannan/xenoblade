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

    // ok stays false (=> state 4) unless an item object can be kept alive:
    // a busy actor only stays when func_8020A5DC still reports work, and an
    // idle one is (re)spawned via func_8020A484 + busy bit.
    int ok;
    if (self->field_6A != 0) {
        if (self->field_74 & kItemFlagBusy) {
            if (func_8020A5DC() != 0) {
                ok = 1;
            } else {
                self->field_74 &= ~kItemFlagBusy;
                ok = 0;
            }
        } else {
            func_8020A484();
            self->field_74 |= kItemFlagBusy;
            ok = 1;
        }
    } else {
        ok = 0;
    }

    if (ok == 0) {
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
        u32 seq = func_800822F4__Q22cf13CfGameManagerFv();
        if (self->field_6C > seq || seq > self->field_6E)
            return;
    }

    // Respawn-gate: when field_94 is set, the current count must sit inside
    // the [field_96, field_97] window or the spawn is skipped.
    if (self->field_94 != 0) {
        int count = (int)func_80082354__Q22cf13CfGameManagerFv(self->field_94);
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
// Remaining functions (scaffolds - NOT YET DECOMPILED)
// ---------------------------------------------------------------------------

void func_802108D8(){}

// ---------------------------------------------------------------------------
// FULL_MATCH: Virtual function override -- no-op
// ---------------------------------------------------------------------------

extern "C" void func_80210C1C() {}