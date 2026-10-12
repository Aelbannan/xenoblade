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
    this->vtable = (u32*)lbl_eu_80535A98;
    CfGimmick_ClearManagerBinding((cf::CfGimmick*)this);
    CfGimmick_UnregisterSpawnedObject(&this->field_7C);
    __dt__Q22cf9CfGimmickFv((cf::CfGimmick*)this, 0);
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
    __ct__cf_CfGimmick((cf::CfGimmick*)self);
    self->vtable = (u32*)lbl_eu_80535A98;
    self->field_82 = 6;

    void* mgr = Bdat_GetTable_AA34();
    // Retail keeps the bdat table handle in a dedicated frame slot and
    // reloads it for every column read (its address escapes below).
    u32 bdat = lbl_eu_8066413C;
    self->field_64 = rowId;

    // Init the three sub-objects (placement vec, pad10, vobj).
    CfGimmick_LoadBdatAreaPos((cf::CfGimmick*)self, &self->vvec04.x, mgr, &bdat);
    CfGimmick_LoadBdatAreaExtents((cf::CfGimmick*)self, (cf::CfGimmick*)&self->vobj, mgr, &bdat);
    CfGimmick_LoadBdatAreaRotation((cf::CfGimmick*)self, (f32*)self->pad10, mgr, &bdat);

    // Column name buffers "A_Item"/"A_Lost" get the slot letter written in
    // (the table pointer holds them at lbl_eu_806627B8[0]/[1]).
    volatile BdatCol c66;
    c66.w = getBdatStringColumnValue((void*)bdat, "rectype", rowId);
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
    c8A.w = getBdatStringColumnValue((void*)bdat, "geItem", rowId);
    self->field_8A = c8A.h;
    volatile BdatCol c6A;
    c6A.w = getBdatStringColumnValue((void*)bdat, "MSG1", rowId);
    self->field_6A = c6A.h;
    volatile BdatCol c8C;
    c8C.w = getBdatStringColumnValue((void*)bdat, "MSG2", rowId);
    self->field_8C = c8C.h;
    volatile BdatCol c98;
    c98.w = getBdatStringColumnValue((void*)bdat, "time", rowId);
    self->field_98 = c98.b;
    volatile BdatCol c70;
    c70.w = getBdatStringColumnValue((void*)bdat, "LODObj", rowId);
    self->field_70 = c70.b;
    volatile BdatCol c99;
    c99.w = getBdatStringColumnValue((void*)bdat, "LODSub", rowId);
    self->field_99 = c99.b;
    volatile BdatCol c9A;
    c9A.w = getBdatStringColumnValue((void*)bdat, "LODType", rowId);
    self->field_9A = c9A.b;
    volatile BdatCol c9B;
    c9B.w = getBdatStringColumnValue((void*)bdat, "EFF", rowId);
    self->field_9B = c9B.b;
    volatile BdatCol c92;
    c92.w = getBdatStringColumnValue((void*)bdat, "camID", rowId);
    self->field_92 = c92.h;
    volatile BdatCol c8E;
    c8E.w = getBdatStringColumnValue((void*)bdat, "swtSE", rowId);
    self->field_8E = c8E.h;
    volatile BdatCol c90;
    c90.w = getBdatStringColumnValue((void*)bdat, "finSE", rowId);
    self->field_90 = c90.h;
    volatile BdatCol c9C;
    c9C.w = getBdatStringColumnValue((void*)bdat, "ct", rowId);
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
    c96.w = getBdatStringColumnValue((void*)bdat, "quest_Max", rowId);
    self->field_96 = c96.b;
    volatile BdatCol c97;
    c97.w = getBdatStringColumnValue((void*)bdat, "quest_Min\0\0\0", rowId);
    self->field_97 = c97.b;

    // vtable slot 0x20 virtual call.
    ((void (***)(cf::CfGimmickItem*))self)[0][8](self);

    self->field_9E = 0;
    self->field_A0 = 0;
    if (self->field_70 != 0) {
        attachLODObject__8CTaskLODFv(self->field_70, 0);
        removeLODEntry__8CTaskLODFv(self->field_70, lbl_eu_80668448);
    }

    if (CfGimmick_CheckStateFlag2CC8((u8*)(u32)self->field_64) != 0 && self->field_9C == 3) {
        self->field_9E = 5;
    }
    return self;
}

// ---------------------------------------------------------------------------
// GimItem_StateDispatch -- PTMF state dispatch + one-frame spawn/cleanup
// ---------------------------------------------------------------------------

// Field-74 state flag bits (retail rlwinm masks).
enum {
    kItemFlagWork = 0x10,   // "working" flag set/cleared during the move step
    kItemFlagBusy = 0x20,   // actor busy flag (spawned/attached)
};

extern "C" void GimItem_StateDispatch(cf::CfGimmickItem* self) {
    // Dispatch the current state through the 12-byte PTMF table.
    (self->*((cf::CfGimmickItemState*)lbl_eu_80535A50)[self->field_9E])();

    if (self->field_74 & kItemFlagWork) {
        if (self->field_66 & 1) {
            // Attach an item object at the placement point while working.
            CfGimmick_UpdateProximity(&self->field_7C, &self->vvec04, self->field_6A,
                          lbl_eu_8066844C, 0, 0);
        }
        self->field_74 &= ~kItemFlagWork;
    } else {
        CfGimmick_UnregisterSpawnedObject(&self->field_7C);
    }
}

// ---------------------------------------------------------------------------
// GimItem_BusyActivate -- busy-flag handled item activation; falls back to state 4
// ---------------------------------------------------------------------------

void GimItem_BusyActivate(cf::CfGimmickItem* self) {
    CfGimmick_SetGlobalFlagC0042();

    // a busy actor only stays when CfGimmick_IsMessageSystemBusy still reports work, and an
    // idle one is (re)spawned via CfGimmick_PostGimmickName + busy bit.  The two fail
    // paths branch to the shared `zero` merge (single `li r3,0`); the ready
    // paths set ok=1 and jump straight to the test.
    u32 ok;
    if ((ok = self->field_6A) == 0) {
        goto st4;
    }
    if (self->field_74 & kItemFlagBusy) {
        if (CfGimmick_IsMessageSystemBusy() != 0) {
            ok = 1;
        } else {
            self->field_74 &= ~kItemFlagBusy;
            goto zero;
        }
    } else {
        CfGimmick_PostGimmickName(self->field_6A);
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
// GimItem_CollectScan -- detect what can be collected and move to the next state
// ---------------------------------------------------------------------------

void GimItem_CollectScan(cf::CfGimmickItem* self) {
    for (int i = 0; i < 3; ++i) {
        if (self->field_84[i] != 0 && (self->field_74 & (1 << i))) {
            CItem_consumeFamilyCnt(self->field_84[i], 1);
        }
    }

    if (self->field_8A != 0) {
        CItem_thunkAllocRecord(self->field_8A, 1);
    }

    if (self->field_9C == 3) {
        self->field_9E = 5;
    } else if (self->field_9C == 2) {
        self->field_9E = 5;
    } else {
        self->field_9E = 4;
    }

    if (self->field_64 != 0) {
        CfGimmick_TriggerSound2CC8(self->field_64, 1);
    }
}

// ---------------------------------------------------------------------------
// GimItem_PlaceDispatch -- placement dispatch; on miss reset the item state
// ---------------------------------------------------------------------------

void GimItem_PlaceDispatch(cf::CfGimmickItem* self) {
    if ((self->field_66 & 1) != 0 ||
        jumptable_eu_80535830[self->mType](
            (cf::CfGimmick*)&self->vobj, &lbl_eu_805765A0, &self->vvec04) == 0) {
        self->field_9E = 0;
    }
}

// ---------------------------------------------------------------------------
// GimItem_WorkFrameUpdate -- working-frame update: range window, respawn gate, spawn
// ---------------------------------------------------------------------------

void GimItem_WorkFrameUpdate(cf::CfGimmickItem* self) {
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
    if (CfGimmick_CheckTriggerGated((u32)self->field_66, (cf::CfGimmick*)&self->vobj, &self->vvec04,
                      (const f32*)self->pad10, (u32)self->field_7C.field_00) == 0)
        return;

    // Every non-empty item slot must be collectible, otherwise abort.
    int ok = 0;
    for (int i = 0; i < 3; ++i) {
        if (self->field_84[i] != 0) {
            if (CItem_findRecByFamily(self->field_84[i]) == 0) {
                ok = 0;
                break;
            }
            ok = 1;
        }
    }

    // While toggled-on, fire the per-frame effect at the placement point.
    if (self->field_66 & 1) {
        if (self->field_8E != 0) {
            CfGimmick_PlaySoundAtPos((u32)self->field_8E, &self->vvec04);
        }
    }

    if (ok) {
        self->field_9E = 2;
    } else {
        self->field_9E = 1;
    }
}

// ---------------------------------------------------------------------------
// GimItem_LodFxUpdate -- per-frame LOD/effect/manager update + countdown reset
// ---------------------------------------------------------------------------

void GimItem_LodFxUpdate(cf::CfGimmickItem* self) {
    CfGimmick_SetGlobalFlagC0042();

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
            CfGimmick_PlaySoundAtPos((u32)self->field_90, &self->vvec04);
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
            CamEvtProgramEventCam(self->field_92);
        }
    }

    // Countdown: while field_A0 < field_98 just increment; once the window
    // expires, keep the item alive only if a busy actor still reports work
    // (same ok/zero merge as GimItem_BusyActivate), otherwise reset to state 3.
    if (self->field_A0 < self->field_98) {
        self->field_A0++;
    } else {
        int ok;
        if (self->field_8C == 0) {
            goto rst;
        }
        if (self->field_74 & kItemFlagBusy) {
            if (CfGimmick_IsMessageSystemBusy() != 0) {
                ok = 1;
            } else {
                self->field_74 &= ~kItemFlagBusy;
                goto zero;
            }
        } else {
            CfGimmick_PostGimmickName(self->field_8C);
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

extern "C" void GimItem_Noop() {}

// ----- typified retail data (replaces copy_data_sections) -----
// .rodata RTTI class-name (0x12) + 2-byte pad gap to the column-name pool.
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_80508798[0x14] = "cf::CfGimmickItem";
// .rodata BDAT column-name pool: the ctor passes plain string literals
// ("rectype" .. "quest_Min"); MWCC pools them in first-use order, which
// reproduces the retail layout byte-exactly. A prior revision kept
// these bytes as a static pad + extern lbl_eu_805087AC refs; naming the pad
// reschedules the ctor (92.1% -> 21.4%, verified), and the label has no rule
// entry (frozen), so the honest original shape -- literals pooled by MWCC
// -- is used and the synthetic row dissolves (no references remain).
// MWCC does not pad section tails and named objects sort before the literal
// pool, so retail's three trailing zero bytes ride inside the last pooled
// literal ("quest_Min" + three explicit NULs; strcmp semantics identical).
// .data vtable-shaped blocks as real pointer tables (in-file bytes are
// zero at reloc sites, matching retail; all-zero char blobs would be exiled
// to .bss by MWCC). 80535A18 is referenced by CfGimmickJump's code; 80535A3C
// feeds this TU's .sdata pair B0. Forward-declared with exact types.
extern u32 lbl_eu_806627B0[2];   // defined below (.sdata)
extern u32 lbl_eu_80535A3C[5];   // defined below (.data)
extern char lbl_eu_80661BE0[];
extern char lbl_eu_80662708[];
extern "C" {
void CfGimmick_DetachManager(void);
void CfGimmickJump_UpdateState(void);
void GimNoopB(void);
void GimNoopC(void);
void GimNoopD(void);
void CfGimmickJump_RefreshMatrices(void);
void __dt__Q22cf13CfGimmickJumpFv(void*, int);  // ABI deleting dtor
}
__declspec(section ".data") __attribute__((used, aligned(4)))
u32 lbl_eu_80535A18[9] = {
    (u32)lbl_eu_806627B0, 0,
    (u32)__dt__Q22cf13CfGimmickJumpFv,
    (u32)CfGimmick_DetachManager, (u32)CfGimmickJump_UpdateState,
    (u32)GimNoopB, (u32)GimNoopC,
    (u32)GimNoopD, (u32)CfGimmickJump_RefreshMatrices,
};
__declspec(section ".data") __attribute__((used, aligned(4)))
u32 lbl_eu_80535A3C[5] = {
    (u32)lbl_eu_80661BE0, 0, (u32)lbl_eu_80662708, 0, 0,
};
// .data PTMF state table: 6 x (this-delta 0, -1 marker, func addr).
// Func slots are zero in-file (linker fills them); -1 words are immediates.
__declspec(section ".data") __attribute__((used, aligned(4)))
char lbl_eu_80535A50[0x48] = {
    0x00,0x00,0x00,0x00, 0xFF,0xFF,0xFF,0xFF, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0xFF,0xFF,0xFF,0xFF, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0xFF,0xFF,0xFF,0xFF, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0xFF,0xFF,0xFF,0xFF, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0xFF,0xFF,0xFF,0xFF, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0xFF,0xFF,0xFF,0xFF, 0x00,0x00,0x00,0x00,
};
// .sdata2 item-name strings (7 chars + NUL + 1 pad byte each). Declared
// before the .sdata pair that takes their addresses.
__declspec(section ".sdata2") __attribute__((used, aligned(1)))
char lbl_eu_80668438[8] = "A_Item";
__declspec(section ".sdata2") __attribute__((used, aligned(1)))
char lbl_eu_80668440[8] = "A_Lost";
// .sdata typeinfo pairs. A0/A8/B0 point at other TUs' labels (Select
// precedent: reloc slots are zero in-file); B8 points at this TU's sdata2
// strings below.
extern char lbl_eu_805086D8[];
extern char lbl_eu_80535924[];
extern char lbl_eu_805086F8[];
extern char lbl_eu_805359D4[];
extern char lbl_eu_80508728[];
__declspec(section ".sdata") __attribute__((used, aligned(8)))
u32 lbl_eu_806627A0[2] = { (u32)lbl_eu_805086D8, (u32)lbl_eu_80535924 };
__declspec(section ".sdata") __attribute__((used, aligned(8)))
u32 lbl_eu_806627A8[2] = { (u32)lbl_eu_805086F8, (u32)lbl_eu_805359D4 };
__declspec(section ".sdata") __attribute__((used, aligned(8)))
u32 lbl_eu_806627B0[2] = { (u32)lbl_eu_80508728, (u32)lbl_eu_80535A3C };
__declspec(section ".sdata") __attribute__((used, aligned(8)))
char* lbl_eu_806627B8[2] = { lbl_eu_80668438, lbl_eu_80668440 };
// .sdata2 float pool as four named non-const scalars (header declares them
// extern f32, so const would collide with 10563). Non-const cannot fold and
// __declspec(section) pins them to .sdata2; MWCC keeps the two 0.0f slots
// separate and codegen is unchanged (ctor stays 92.1%). Landed this wave;
// the struct + macro it replaces is gone.
#pragma section data_type ".sdata2" ".sdata2"
extern __declspec(section ".sdata2") f32 lbl_eu_80668448 = 0.0f;
extern __declspec(section ".sdata2") f32 lbl_eu_8066844C = 5.0f;
extern __declspec(section ".sdata2") f32 lbl_eu_80668450 = 0.0f;
extern __declspec(section ".sdata2") f32 lbl_eu_80668454 = 20.0f;
#pragma section data_type ".data" ".data"
// ----- end typified retail data -----
