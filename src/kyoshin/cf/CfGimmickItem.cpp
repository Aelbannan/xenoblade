/**
 * @file CfGimmickItem.cpp
 * @brief Translation unit for cf::CfGimmickItem (item gimmick).
 *
 * Functions decompiled from the retail ASM at build/us/asm/kyoshin/cf/.
 * cf::CfGimmickItem derives from cf::CfGimmick; the PTMF state table is
 * referenced as a data symbol.
 */

#include "kyoshin/cf/CfBdat.hpp"
#include "kyoshin/cf/CfGimmickItem.hpp"

// BDAT column result: the raw call result lives in a word frame slot and is
// narrowed on use (lbz/lhz); the word union models that storage.
union BdatCol {
    u32 w;
    u8 b;
    u16 h;
};


// ---------------------------------------------------------------------------
// Constructor / Destructor
// ---------------------------------------------------------------------------

enum {
    kItemFlagWork = 0x10,   // "working" flag set/cleared during the move step
    kItemFlagBusy = 0x20,   // actor busy flag (spawned/attached)
};

cf::CfGimmickItem::CfGimmickItem(int rowId) {
    this->field_82 = 6;

    void* mgr = Bdat_GetTable_AA34();
    // Retail keeps the bdat table handle in a dedicated frame slot and
    // reloads it for every column read (its address escapes below).
    u32 bdat = (u32)::cf::CfBdat::spCurItemGmFileData;
    this->field_64 = rowId;

    // Area position / extents (collider view) / rotation.
    CfGimmick_LoadBdatAreaPos(this, &this->field_04.x, mgr, &bdat);
    CfGimmick_LoadBdatAreaExtents(this, (cf::CfGimmick*)this->field_1C, mgr, &bdat);
    CfGimmick_LoadBdatAreaRotation(this, &this->field_10.x, mgr, &bdat);

    // Column name buffers "A_Item"/"A_Lost" get the slot letter written in
    // (the table pointer holds them at lbl_eu_806627B8[0]/[1]).
    BdatCol c66;
    c66.w = getBdatStringColumnValue((void*)bdat, "rectype", rowId);
    this->field_66 = c66.b;
    for (int i = 0; i < 3; ++i) {
        char* lostColumn = lbl_eu_806627B8[1];
        char c = (char)('A' + i);
        char* itemColumn = lbl_eu_806627B8[0];
        itemColumn[0] = lostColumn[0] = c;
        BdatCol cid;
        cid.w = getBdatStringColumnValue((void*)bdat, itemColumn, rowId);
        lostColumn = lbl_eu_806627B8[1];
        this->field_84[i] = cid.h;
        BdatCol clost;
        clost.w = getBdatStringColumnValue((void*)bdat, lostColumn, rowId);
        if (clost.b != 0) {
            this->field_74 |= (1u << i);
        }
    }

    BdatCol c8A;
    c8A.w = getBdatStringColumnValue((void*)bdat, "geItem", rowId);
    this->field_8A = c8A.h;
    BdatCol c6A;
    c6A.w = getBdatStringColumnValue((void*)bdat, "MSG1", rowId);
    this->field_6A = c6A.h;
    BdatCol c8C;
    c8C.w = getBdatStringColumnValue((void*)bdat, "MSG2", rowId);
    this->field_8C = c8C.h;
    BdatCol c98;
    c98.w = getBdatStringColumnValue((void*)bdat, "time", rowId);
    this->field_98 = c98.b;
    BdatCol c70;
    c70.w = getBdatStringColumnValue((void*)bdat, "LODObj", rowId);
    this->field_70 = c70.b;
    BdatCol c99;
    c99.w = getBdatStringColumnValue((void*)bdat, "LODSub", rowId);
    this->field_99 = c99.b;
    BdatCol c9A;
    c9A.w = getBdatStringColumnValue((void*)bdat, "LODType", rowId);
    this->field_9A = c9A.b;
    BdatCol c9B;
    c9B.w = getBdatStringColumnValue((void*)bdat, "EFF", rowId);
    this->field_9B = c9B.b;
    BdatCol c92;
    c92.w = getBdatStringColumnValue((void*)bdat, "camID", rowId);
    this->field_92 = c92.h;
    BdatCol c8E;
    c8E.w = getBdatStringColumnValue((void*)bdat, "swtSE", rowId);
    this->field_8E = c8E.h;
    BdatCol c90;
    c90.w = getBdatStringColumnValue((void*)bdat, "finSE", rowId);
    this->field_90 = c90.h;
    BdatCol c9C;
    c9C.w = getBdatStringColumnValue((void*)bdat, "ct", rowId);
    this->field_9C = c9C.b;
    BdatCol c6C;
    c6C.w = getBdatStringColumnValue((void*)bdat, ((char**)gGimmickBdatColumns)[13], rowId);
    this->field_6C = c6C.h;
    BdatCol c6E;
    c6E.w = getBdatStringColumnValue((void*)bdat, ((char**)gGimmickBdatColumns)[14], rowId);
    this->field_6E = c6E.h;
    BdatCol c94;
    c94.w = getBdatStringColumnValue((void*)bdat, ((char**)gGimmickBdatColumns)[15], rowId);
    this->field_94 = c94.h;
    BdatCol c96;
    c96.w = getBdatStringColumnValue((void*)bdat, "quest_Max", rowId);
    this->field_96 = c96.b;
    BdatCol c97;
    c97.w = getBdatStringColumnValue((void*)bdat, "quest_Min\0\0\0", rowId);
    this->field_97 = c97.b;

    CfGimmick_UpdateColliderMatrix();

    this->field_9E = 0;
    this->field_A0 = 0;
    if (this->field_70 != 0) {
        attachLODObject__8CTaskLODFv(this->field_70, 0);
        removeLODEntry__8CTaskLODFv(this->field_70, lbl_eu_80668448);
    }

    if (CfGimmick_CheckStateFlag2CC8((u8*)(u32)this->field_64) != 0 && this->field_9C == 3) {
        this->field_9E = 5;
    }
}

cf::CfGimmickItem::~CfGimmickItem() {
    CfGimmick_ClearManagerBinding(this);
    CfGimmick_UnregisterSpawnedObject(&this->field_7C);
}

void cf::CfGimmickItem::CfGimmick_Update() {
    // Dispatch the current state through the 12-byte PTMF table.
    (this->*((cf::CfGimmickItemState*)lbl_eu_80535A50)[field_9E])();

    if (field_74 & kItemFlagWork) {
        if (field_66 & 1) {
            // Attach an item object at the placement point while working.
            CfGimmick_UpdateProximity(&field_7C, &field_04, field_6A,
                          lbl_eu_8066844C, 0, 0);
        }
        field_74 &= ~kItemFlagWork;
    } else {
        CfGimmick_UnregisterSpawnedObject(&field_7C);
    }
}

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
    if (CfGimmick_CheckTriggerGated((u32)self->field_66, (cf::CfGimmick*)self->field_1C, &self->field_04,
                      &self->field_10.x, (u32)self->field_7C.field_00) == 0)
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
            CfGimmick_PlaySoundAtPos((u32)self->field_8E, &self->field_04);
        }
    }

    if (ok) {
        self->field_9E = 2;
    } else {
        self->field_9E = 1;
    }
}

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
            CfGimmick_PlaySoundAtPos((u32)self->field_90, &self->field_04);
        }

        if (self->field_9B != 0) {
            cf::CfGimmickItemMgr* obj =
                (cf::CfGimmickItemMgr*)createBattleActor__Q22cf13CfGameManagerFv(self->field_9B, 0);
            self->field_78 = (cf::CfGameManager*)obj;
            if (obj != 0) {
                obj->field_B0 = self;
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

void GimItem_PlaceDispatch(cf::CfGimmickItem* self) {
    if ((self->field_66 & 1) != 0 ||
        jumptable_eu_80535830[self->field_60](
            (cf::CfGimmick*)self->field_1C, &gGimmickAnchorPoint, &self->field_04) == 0) {
        self->field_9E = 0;
    }
}

extern "C" void GimItem_Noop() {}

// ----- typified retail data (replaces copy_data_sections) -----
// .rodata: the RTTI class name "cf::CfGimmickItem" is compiler-emitted first.
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
// .sdata column-name buffer pointers (the compiler appends the RTTI record).
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
#pragma section data_type ".data" ".data"
// ----- end typified retail data -----
