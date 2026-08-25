// Auto-scaffolded catalog TU for kyoshin/cf/CfGimmickObject
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CfGimmickObject.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

using namespace cf;

// getBdatStringColumnValue: canonical decl in plugin/ocBdat.hpp (pulled in
// by harness_catalog.hpp) with the s32(long) row parameter.

// Same-TU availability checks used by the ctor's initial step seeding.
int func_801F7978(cf::CfGimmickObject* self);
int func_801F7B44(cf::CfGimmickObject* self);

// Column fetch helpers mirroring retail's (u16)/(u8) truncation round-trip
// through the ctor frame (same shape as the matched CfGimmickItem ctor).
static inline u16 ctorCol16(void* table, const char* col, s32 row) {
    return (u16)getBdatStringColumnValue(table, col, row);
}
static inline u8 ctorCol8(void* table, const char* col, s32 row) {
    return (u8)getBdatStringColumnValue(table, col, row);
}

/**
 * Constructor -- populates every field from the object-gimmick bdat row
 * `row`, initializes the four sub-objects (placement vec / collider A /
 * collider B / reference point) and registers the LOD tasks into the shared
 * per-id flag word array (`flagWords`, 32 ids per u32). The tail of the
 * gimmick container (`tail`/`count`) is scanned to wire up same-area peers:
 * a duplicate area id suppresses the initial step-machine start, and the
 * first LOD-matching peer becomes the linked object at +0x168 (both sides).
 */
extern "C" cf::CfGimmickObject* __ct__cf_CfGimmickObject(
    cf::CfGimmickObject* self, s32 row, cf::CfGimmickObject** tail, int count,
    u32* flagWords) {
    __ct__cf_CfGimmick((void*)self);
    self->vtable = (void*)lbl_eu_80534F70;
    self->field_82 = 1;
    self->field_194 = 0;

    void* mgr = func_8003AA34();
    void* holder = ((CfGimmickTableSet*)mgr)->lbl_eu_80664128;
    char* colBase = lbl_eu_80507B60;
    self->field_64 = row;

    // Scalar columns from lbl_eu_80507B60 and the lbl_eu_805357E8 pointer
    // table, read in the exact retail order.
    self->field_66 = ctorCol8(holder, &colBase[0], row);

    // Sub-object init: placement vec (+0x04), collider A (+0x1C), collider B
    // (+0xF4), reference point (+0x10); then the vtable slot 0x20 hook.
    func_80208F34(self, &self->field_04, mgr, &holder);
    func_80209020(self, &self->field_1C, mgr, &holder);
    func_8020915C(self, &self->field_F4, mgr, &holder);
    func_80209288(self, &self->field_10, mgr, &holder);
    ((ICfGimmickObjectVt*)self)->vt06();

    // Scalar columns from lbl_eu_80507B60 and the lbl_eu_805357E8 pointer
    // table. Retail interleaves each call with the next column address, so
    // the reads/writes below are kept in the exact retail order.
    self->field_15E = ctorCol8(holder, &colBase[0x8], row);
    self->field_161 = ctorCol8(holder, &colBase[0xb], row);
    self->field_13C = ctorCol16(holder, *(char**)&lbl_eu_805357E8[0x3c], row);
    self->field_13E = ctorCol16(holder, *(char**)&lbl_eu_805357E8[0x40], row);
    self->field_6C = ctorCol16(holder, *(char**)&lbl_eu_805357E8[0x34], row);
    self->field_6E = ctorCol16(holder, *(char**)&lbl_eu_805357E8[0x38], row);
    self->field_140 = ctorCol16(holder, *(char**)&lbl_eu_805357E8[0x2c], row);
    self->field_142 = ctorCol16(holder, &colBase[0x13], row);
    self->field_15F = ctorCol8(holder, &colBase[0x1d], row);
    self->field_160 = ctorCol8(holder, &colBase[0x26], row);
    self->field_68 = ctorCol16(holder, &colBase[0x2f], row);
    self->field_70[0] = ctorCol8(holder, &colBase[0x36], row);
    self->field_70[1] = ctorCol8(holder, &colBase[0x3d], row);
    self->field_6A = ctorCol16(holder, &colBase[0x45], row);
    self->field_156 = ctorCol16(holder, &colBase[0x4d], row);
    self->field_162 = ctorCol8(holder, &colBase[0x55], row);
    self->field_163 = ctorCol8(holder, &colBase[0x61], row);
    self->field_164 = ctorCol8(holder, &colBase[0x6d], row);
    self->field_144[0] = ctorCol16(holder, &colBase[0x79], row);
    self->field_144[1] = ctorCol16(holder, &colBase[0x84], row);
    self->field_144[2] = ctorCol16(holder, &colBase[0x8f], row);
    self->field_14A[0] = ctorCol16(holder, &colBase[0x9a], row);
    self->field_14A[1] = ctorCol16(holder, &colBase[0xa5], row);
    self->field_14A[2] = ctorCol16(holder, &colBase[0xb0], row);
    self->field_158 = ctorCol16(holder, &colBase[0xbb], row);
    self->field_15A = ctorCol16(holder, &colBase[0xc5], row);
    self->field_15C = ctorCol16(holder, &colBase[0xcc], row);

    // Either non-zero id table raises its availability flag.
    if (self->field_144[0] != 0 || self->field_144[1] != 0 ||
        self->field_144[2] != 0)
        self->field_74 |= 0x800;
    if (self->field_14A[0] != 0 || self->field_14A[1] != 0 ||
        self->field_14A[2] != 0)
        self->field_74 |= 0x1000;

    self->field_154 = ctorCol16(holder, &colBase[0xd3], row);
    self->field_165 = ctorCol8(holder, &colBase[0xdc], row);
    self->field_14A[3] = ctorCol16(holder, &colBase[0xe7], row);

    // Per-area tables: the shared column-name buffers get the slot digit
    // ('1'/'2') stamped in before each batch of eight columns is read.
    for (int i = 0; i < 2; ++i) {
        char digit = (char)('1' + i);
        for (int k = 0; k < 8; ++k)
            lbl_eu_80534F00[k][1] = digit;
        CfGimmickObjectArea* e = &self->field_84[i];
        e->field_00 = ctorCol16(holder, lbl_eu_80534F00[0], row);
        e->field_02 = ctorCol16(holder, lbl_eu_80534F00[1], row);
        e->field_04 = ctorCol16(holder, lbl_eu_80534F00[2], row);
        e->field_06 = ctorCol16(holder, lbl_eu_80534F00[3], row);
        e->field_08 = ctorCol16(holder, lbl_eu_80534F00[4], row);
        e->field_0A = ctorCol16(holder, lbl_eu_80534F00[5], row);
        e->field_0C = ctorCol16(holder, lbl_eu_80534F00[6], row);
        e->field_0E = ctorCol16(holder, lbl_eu_80534F00[7], row);
    }

    self->field_152 = ctorCol16(holder, &colBase[0xf0], row);

    // Per-step entries (5 x 16 bytes). Steps 4+ hard-code field_00 to 0.
    for (int i = 0; i < 5; ++i) {
        char digit = (char)('1' + i);
        for (int k = 0; k < 10; ++k)
            lbl_eu_80534F20[k][1] = digit;
        CfGimmickObjectStep* s = &self->field_A4[i];
        if (i < 4)
            s->field_00 = ctorCol16(holder, lbl_eu_80534F20[0], row);
        else
            s->field_00 = 0;
        s->field_02 = ctorCol16(holder, lbl_eu_80534F20[1], row);
        s->field_04 = ctorCol8(holder, lbl_eu_80534F20[2], row);
        s->field_05 = ctorCol8(holder, lbl_eu_80534F20[3], row);
        s->field_06 = ctorCol8(holder, lbl_eu_80534F20[4], row);
        s->field_07 = ctorCol8(holder, lbl_eu_80534F20[5], row);
        s->field_08 = ctorCol8(holder, lbl_eu_80534F20[6], row);
        s->field_0A = ctorCol16(holder, lbl_eu_80534F20[7], row);
        s->field_0E = ctorCol8(holder, lbl_eu_80534F20[8], row);
        s->field_0C = ctorCol16(holder, lbl_eu_80534F20[9], row);
    }

    // Register both configured LOD tasks unless their shared flag bit is
    // already claimed by another gimmick; registration claims the bit.
    for (int i = 0; i < 2; ++i) {
        u8 lod = self->field_70[i];
        if (lod == 0)
            continue;
        u32 bit = 1u << (lod & 31);
        u32* word = &flagWords[lod >> 5];
        if ((*word & bit) != 0)
            continue;
        if ((self->field_161 & 0x2) != 0) {
            func_80462DB4__8CTaskLODFv(lod, 0);
            func_804BCC3C(func_804BC9EC__Fv(), lod);
        } else if (self->field_162 != 0 || self->field_163 != 0) {
            func_80462DB4__8CTaskLODFv(lod, 1);
            func_804BCC30(func_804BC9EC__Fv(), lod);
        }
        func_80462F4C__8CTaskLODFv(lod, 0);
        func_80462EF4__8CTaskLODFv(lod, lbl_eu_806681A0);
        func_80462E3C__8CTaskLODFv(lod, lbl_eu_806681A4);
        *word |= bit;
    }

    if ((self->field_C9 & 0x40) != 0)
        self->field_74 |= 0x100;

    // Active-mode seeding: reset the step machine, and when either high mode
    // bit is set force mode 3 and clear the working state.
    int stepInit = 1;
    self->field_188 = 0;
    self->field_18C = 0;
    self->field_180 = lbl_eu_806681A0;
    if ((self->field_15E & 0xC0) != 0) {
        if ((self->field_15E & 0x40) != 0) {
            self->field_74 |= 0x01000000;
            stepInit = 0;
        }
        u16 f66 = self->field_66 | 0x8000;
        self->field_15E = 3;
        u32 f74 = self->field_74 | 0x8000;
        self->field_74 = f74;
        self->field_60 = 0;
        self->field_138 = 0;
        self->field_66 = f66 & 0xFFDE;
    }
    if ((self->field_66 & 0x8061) == 0 && self->field_60 == 0 &&
        self->field_138 == 0)
        self->field_66 |= 0x8000;

    if ((self->field_66 & 0x20) != 0) {
        // Collision-gated: run the availability chain and jump straight to
        // step 3 when the map object and both checks pass.
        if (func_8020971C((void*)(u32)self->field_64) != 0 &&
            func_801F7978(self) != 0 && func_801F7B44(self) != 0) {
            func_801F5C2C(self, 0, 3);
            self->field_188 = 3;
        }
    } else if (self->field_15E == 3) {
        self->field_74 |= 0x200;
        if (func_8020971C((void*)(u32)self->field_64) != 0) {
            self->field_74 &= ~0x8200u;
            // Duplicate-area suppression: another spawned object sharing our
            // LOD id and any area/effect id blocks the initial step-5 start.
            int uniq = 1;
            u8 lod = self->field_70[0];
            if (lod != 0) {
                for (int j = 0; j < count; ++j) {
                    cf::CfGimmickObject* o = tail[j];
                    if (o->field_70[0] != lod)
                        continue;
                    if (o->field_140 == self->field_64 ||
                        o->field_84[0].field_08 == self->field_64 ||
                        o->field_84[1].field_08 == self->field_64) {
                        uniq = 0;
                        break;
                    }
                }
            }
            if (uniq != 0 && stepInit != 0)
                func_801F5C2C(self, 0, 5);
            else
                self->field_188 = 6;
        }
    }

    // Link the first container entry sharing our first LOD id as the peer
    // (+0x168, wired on both sides).
    self->field_168 = 0;
    if (self->field_70[0] != 0) {
        for (int j = 0; j < count; ++j) {
            cf::CfGimmickObject* o = tail[j];
            if (o->field_70[0] == self->field_70[0]) {
                self->field_168 = o;
                o->field_168 = self;
                break;
            }
        }
    }

    // Neither special-effect bit set means the per-frame driver starts busy.
    if ((self->field_161 & 0x4) == 0 && (self->field_161 & 0x10) == 0)
        self->field_74 |= 0x08000000;
    return self;
}

// Rebuild both collider matrices from the object's basis (+0x04) and the
// reference point (+0x10); collider A lives at +0x1C, collider B at +0xF4.
void func_801F7930(cf::CfGimmickObject* self) {
    func_802089BC(self->field_1C, self->field_04, &self->field_10);
    func_802089BC(self->field_F4, self->field_04, &self->field_10);
}

cf::CfGimmickObject::~CfGimmickObject() {
    this->vtable = (void*)lbl_eu_80534F70;
    func_80208EE4((void*)this);
    func_8020A434(&this->field_7C);
    __dt__Q22cf9CfGimmickFv((void*)this, 0);
    // MWCC appends the deleting-dtor prologue (null guard) and epilogue
    // (delete-flag ? operator delete(this) : skip) automatically.
}

// func_801F5B00 - step-machine driver. While the step index stays below 6,
// promote the 0x80000 busy flag to 0x40000000, then dispatch through the
// field_74 0x400 gate: the lbl_eu_80534EB8 pmf table when set, otherwise the
// func_801F634C helper (which may clear work via func_8020A0CC) or the
// lbl_eu_80534E70 pmf table. Stop on a 0 return and run the cleanup hook.
void func_801F5B00(cf::CfGimmickObject* self) {
    while (self->field_188 < 6) {
        if ((self->field_74 & 0x80000) != 0)
            // volatile read so the RMW re-loads field_74 (retail does not
            // reuse the branch-test value).
            *(volatile u32*)&self->field_74 |= 0x40000000;
        if ((self->field_74 & 0x400) != 0) {
            if ((self->*lbl_eu_80534EB8[self->field_188])() == 0)
                break;
        } else {
            if (func_801F634C(self) != 0) {
                if ((self->field_74 & 0x200) != 0)
                    func_8020A0CC();
                break;
            }
            if ((self->*lbl_eu_80534E70[self->field_188])() != 0)
                self->field_74 |= 0x400;
            else
                break;
        }
    }
    func_801F76A8(self);
}

void func_801F5BF8(void* self) {
    *(unsigned long*)((char*)self + 0x74) |= 0x10000;
}

extern "C" void func_801F5C08(u8* self) {
    *(unsigned long*)((char*)self + 0x74) &= ~0x18000;
}

extern "C" void func_801F5C18(u8* self) {
    *(unsigned short*)((char*)self + 0x188) = 6;
    *(unsigned long*)((char*)self + 0x74) = 0;
}

// func_801F5C2C - step-range activation. Re-seeds the step machine from the
// mode byte (+0x15E), clears the pending 0x400 state bit and gates the LOD
// re-register on the +0x162/+0x163 id bytes. Then scans the +0xA4 step table
// over [a, b): picks the last non-zero map-object status / area-manager id /
// sound entry, refreshes the linked map object (func_800BE12C) and the area
// manager attach, updates the +0x190/+0x192 sound state, and finally applies
// per-LOD frame updates to both registered LOD tasks.
__declspec(noinline) void func_801F5C2C(cf::CfGimmickObject* self, int a, int b) {
    if (self->field_15E == 1)
        self->field_188 = 0;
    else
        self->field_188 = 6;
    self->field_74 &= ~0x400u;

    if (self->field_162 == 9) {
        func_801F61B0(self, 1);
        return;
    }

    // LOD re-register selector: when +0x162 is in {4,5,6,8} the decision is
    // delegated to +0x163 (re-register only for {1,2,3,7}); otherwise it is
    // inverted (de-register for +0x163 in {4,5,6,8}).
    int sel = 1;
    u8 mode = self->field_162;
    // Range tests written as +0xfc wraps so MWCC emits addi/clrlwi/cmplwi.
    if ((u8)(mode + 0xfc) <= 2 || mode == 8) {
        u8 sub = self->field_163;
        sel = ((u8)(sub + 0xff) <= 2 || sub == 7) ? 1 : 0;
    } else {
        u8 sub = self->field_163;
        if ((u8)(sub + 0xfc) <= 2 || sub == 8)
            sel = 0;
    }

    if (sel == 0) {
        func_801F61B0(self, 0);
        return;
    }
    if ((self->field_161 & 0x2) != 0)
        func_801F61B0(self, 1);

    // Scan [a, b) for the last step with a map-object status.
    if (self->field_68 != 0) {
        u8 statusId = 0;
        u8 lodId = 0;
        for (int k = a; k < b; ++k) {
            if (self->field_A4[k].field_04 != 0) {
                statusId = self->field_A4[k].field_04;
                lodId = self->field_A4[k].field_06;
            }
        }
        if (statusId != 0) {
            u8* obj = (u8*)func_80186BC8(self->field_68);
            if (obj != 0) {
                // With flags 0x10000|0x8000|0x01000000 all raised the status
                // is forced to 0x24 (hidden-state override).
                u32 f = self->field_74;
                if ((f & 0x10000) != 0 && (f & 0x8000) != 0 &&
                    (f & 0x01000000) != 0)
                    func_800BE12C(obj, 0x24, 0, lodId, 1);
                else
                    func_800BE12C(obj, statusId, 0, lodId, 1);
            }
        }
    }

    // Declaration order mirrors retail's register allocation sequence.
    int sndStop = 0;   // a later step requested stopping the current sound
    u8 sndFlag = 0;    // sound flag byte of the picked step
    u16 sndId = 0;     // sound id (step field_0A)
    u16 effFlags = 0;  // effect bitmask of the picked manager step
    u8 mgrId = 0;      // area-manager id (step field_07)
    int effClear = 0;  // a later step requested detaching the current manager
    {
        for (int k = a; k < b; ++k) {
            if (self->field_A4[k].field_07 != 0) {
                if ((self->field_A4[k].field_08 & 0x20) != 0) {
                    mgrId = self->field_A4[k].field_07;
                    effFlags = self->field_A4[k].field_08;
                    effClear = 0;
                } else {
                    effClear = 1;
                    mgrId = 0;
                }
            } else if ((self->field_A4[k].field_08 & 0x44) != 0) {
                mgrId = 0;
                effClear = 1;
            }
            if (self->field_A4[k].field_0A != 0) {
                if ((self->field_A4[k].field_0E & 0x2) != 0) {
                    sndId = self->field_A4[k].field_0A;
                    sndFlag = self->field_A4[k].field_0E;
                    sndStop = 0;
                } else {
                    sndStop = 1;
                    sndId = 0;
                }
            } else if ((self->field_A4[k].field_0E & 0x1C) != 0) {
                sndStop = 1;
                sndId = 0;
            }
        }
    }

    if (mgrId != 0) {
        func_80208EE4(self);
        CfGimmickObjectMgr* mgr =
            func_800817BC__Q22cf13CfGameManagerFv(mgrId, 0);
        self->field_78 = mgr;
        if (mgr != 0)
            mgr->field_B0 = self;
        if ((effFlags & 0x10) != 0) {
            // Attach position basis (+0x04) and the reference height so the
            // area manager tracks the gimmick.
            ((void (*)(void*, void*))mgr->vtable[0x27])(mgr,
                                                        (void*)self->field_04);
            ((void (*)(void*, f32))mgr->vtable[0x31])(mgr, self->field_10.y);
        }
    } else if (effClear != 0) {
        func_80208EE4(self);
    }

    // The 0x18000 flag pair forces the pending sound to be dropped.
    if ((self->field_74 & 0x18000) == 0x18000) {
        sndId = 0;
        sndStop = 1;
    }
    if (sndId != 0) {
        // Defer the start: record id/kind for the step driver to fire later.
        self->field_190 = sndId;
        self->field_74 |= 0x300000;
        if ((sndFlag & 1) != 0)
            self->field_192 = 3;
        else if ((sndFlag & 0x20) != 0)
            self->field_192 = 2;
        else if ((sndFlag & 0x40) != 0)
            self->field_192 = 1;
        else
            self->field_192 = 0;
    } else if (sndStop != 0) {
        if (self->field_80 != 0) {
            func_801BFED0(1, self->field_80, 0xa);
            self->field_80 = 0;
        }
        self->field_74 &= ~0x100000u;
    }

    // Per-LOD refresh: scan [a, b) for the last LOD control byte / frame
    // count, then dispatch the task updates on both registered LOD slots.
    for (int i = 0; i < 2; i++) {
        u8 lod = self->field_70[i];
        if (lod == 0)
            continue;
        u16 frames = 0;
        u8 flags = 0;
        u8 lastId = 0;
        for (int k = a; k < b; ++k) {
            if ((self->field_A4[k].field_05 & 0x60) != 0)
                flags = self->field_A4[k].field_05;
            if (self->field_A4[k].field_0C != 0)
                frames = self->field_A4[k].field_0C;
        }
        // Last iterated step's LOD id byte.
        if (a < b)
            lastId = self->field_A4[b - 1].field_06;
        if (lastId != 0)
            func_80462F94__8CTaskLODFv(lod, lastId);
        // u16 -> f32 via the direct cast: MWCC emits the retail 2^52
        // double-magic idiom (lis 0x4330 stack-slot pair + lfd + fsubs).
        if (frames != 0)
            func_80462FD8__8CTaskLODFv(lod, (f32)frames);
        if (flags == 0)
            continue;
        if ((flags & 0x8) != 0) {
            if ((flags & 0x4) != 0)
                func_80462EF4__8CTaskLODFv(lod, lbl_eu_806681A0);
            else
                func_80462F10__8CTaskLODFv(lod);
            func_80462F4C__8CTaskLODFv(lod, 0);
        } else if ((flags & 0x1) != 0) {
            func_80462F10__8CTaskLODFv(lod);
            func_80462F70__8CTaskLODFv(lod, 0);
            if ((flags & 0x10) != 0) {
                func_80462ED0__8CTaskLODFv(lod, 0);
                func_80462F4C__8CTaskLODFv(lod, 0);
            } else {
                func_80462ED0__8CTaskLODFv(lod, 1);
                func_80462F4C__8CTaskLODFv(lod, 1);
            }
        } else {
            if ((flags & 0x4) != 0) {
                func_80462EF4__8CTaskLODFv(lod, lbl_eu_806681A0);
                func_80462F70__8CTaskLODFv(lod, 1);
                if ((flags & 0x10) != 0) {
                    func_80462ED0__8CTaskLODFv(lod, 0);
                    func_80462F4C__8CTaskLODFv(lod, 0);
                } else {
                    func_80462ED0__8CTaskLODFv(lod, 1);
                    func_80462F4C__8CTaskLODFv(lod, 1);
                }
            } else if ((flags & 0x30) != 0) {
                // Retail issues the rebuild call twice back-to-back.
                func_80462F4C__8CTaskLODFv(lod, 0);
                func_80462F4C__8CTaskLODFv(lod, 0);
            }
        }
    }
}

// Refresh the per-LOD task registrations and the linked map object.
void func_801F61B0(cf::CfGimmickObject* self, int mode) {
    for (int i = 0; i < 2; i++) {
        if (self->field_70[i] != 0) {
            func_80462DB4__8CTaskLODFv(self->field_70[i], mode);
            if (mode != 0) {
                func_804BCC30(func_804BC9EC__Fv(), self->field_70[i]);
            } else {
                func_804BCC3C(func_804BC9EC__Fv(), self->field_70[i]);
            }
        }
    }

    if (self->field_68 != 0) {
        ::CfGimmickObject* obj = (::CfGimmickObject*)func_80186BC8(self->field_68);
        if (obj != 0) {
            // Slot 0x158 through the abstract vtable view so MWCC emits the
            // canonical virtual-call register chain.
            ((ICfGimmickObjectVt*)obj)->setMode(mode);
            func_804B1DC0((char*)obj + 0x60c, mode);
        }
    }
}

// func_801F627C - LOD timer refresh. While the object is busy (flag
// 0x10000000) or the step is 2/3 behind the field_66 0x20 gate, update the
// +0x178 LOD diff (mode != 0: countdown from the second LOD timer; mode == 0:
// snapshot the first) and raise the 0x10 working bit. Separate if-statements
// keep MWCC from folding the ==2/==3 tests into a subi range check.
void func_801F627C(cf::CfGimmickObject* self, u8 lod, int mode) {
    if ((self->field_74 & 0x10000000) != 0)
        goto refresh;
    if ((self->field_66 & 0x20) == 0)
        return;
    if (self->field_188 == 2)
        goto refresh;
    if (self->field_188 != 3)
        return;
refresh:
    if ((self->field_74 & 0x10) != 0)
        return;
    if (mode != 0) {
        f32 base = func_80462F2C__8CTaskLODFv(lod);
        f32 v = func_80462FF4__8CTaskLODFv(lod);
        u32 w = self->field_74;
        self->field_178 = v - base;
        // volatile read so MWCC re-loads field_74 here instead of reusing w
        // (retail emits a fresh lwz after the branch).
        if ((w & 0x20000000) != 0)
            *(volatile u32*)&self->field_74 |= 0x40000000;
    } else {
        self->field_178 = func_80462F2C__8CTaskLODFv(lod);
    }
    self->field_74 |= 0x10;
}

// func_801F634C - per-frame LOD/countdown update for the step machine.
// Gates six independent countdowns on field_74 bits 0..5; each decrements a
// per-field timer by the frame delta and, on expiry, resets the field's LOD
// task alpha via func_80462E3C. Returns 1 while any work remains active.
//
// The s16->f32 conversion of field_18A uses the direct cast so MWCC emits
// the 0x43300000 double-slot + fsubs magic idiom (lbl_eu_806681A8).
int func_801F634C(cf::CfGimmickObject* self) {
    if ((self->field_74 & 0x3F) != 0) {
        int result = 0;
        f32 delta = func_80496288(lbl_eu_80663E14);

        // bit 0: +0x170 expiry (spawns the busy sound via func_80209F2C).
        if ((self->field_74 & 1) != 0) {
            self->field_170 -= delta;
            if (self->field_170 <= lbl_eu_806681A0) {
                self->field_74 &= ~1;
            } else {
                func_80209F2C();
                result = 1;
            }
        }

        // bit 1: +0x16C fade-out; the LOD alpha goes 1.0 -> 0.0 over
        // field_18A frames (ratio clamped at 1.0). The division is written
        // twice so MWCC recomputes the s16->f32 conversion for the fade
        // (retail re-runs the 0x4330 double-slot + fdivs after the branch).
        if ((self->field_74 & 2) != 0) {
            f32 v = self->field_16C - delta;
            self->field_16C = v;
            if (v <= lbl_eu_806681A0) {
                self->field_74 &= ~2;
                for (int i = 0; i < 2; i++) {
                    if (self->field_70[i] != 0)
                        func_80462E3C__8CTaskLODFv(self->field_70[i],
                                                   lbl_eu_806681A4);
                }
                result = 1;
            } else {
                if (v / (f32)self->field_18A < lbl_eu_806681A4) {
                    f32 fade =
                        lbl_eu_806681A4 - v / (f32)self->field_18A;
                    for (int i = 0; i < 2; i++) {
                        if (self->field_70[i] != 0)
                            func_80462E3C__8CTaskLODFv(self->field_70[i], fade);
                    }
                } else {
                    for (int i = 0; i < 2; i++) {
                        if (self->field_70[i] != 0)
                            func_80462E3C__8CTaskLODFv(self->field_70[i],
                                                       lbl_eu_806681A0);
                    }
                }
                result = 1;
            }
        }

        // bit 2: +0x16C countdown gated by the linked peer's busy flag; when
        // the peer (field_168) is busy the whole countdown is skipped.
        if ((self->field_74 & 4) != 0) {
            if (self->field_168 == 0 || (self->field_168->field_74 & 2) == 0) {
                f32 v = self->field_16C - delta;
                self->field_16C = v;
                if (v <= lbl_eu_806681A0) {
                    self->field_74 &= ~4;
                    func_801F61B0(self, 0);
                    for (int i = 0; i < 2; i++) {
                        if (self->field_70[i] != 0)
                            func_80462E3C__8CTaskLODFv(self->field_70[i],
                                                       lbl_eu_806681A4);
                    }
                } else {
                    f32 fade = v / (f32)self->field_18A;
                    for (int i = 0; i < 2; i++) {
                        if (self->field_70[i] != 0)
                            func_80462E3C__8CTaskLODFv(self->field_70[i], fade);
                    }
                }
            } else {
                self->field_74 &= ~4;
            }
            result = 1;
        }

        // bit 3: +0x174 expiry drives a per-LOD range step: the LOD2 target
        // is nudged toward the current value by (elapsed/120) * delta.
        if ((self->field_74 & 8) != 0) {
            f32 v = self->field_174 - delta;
            self->field_174 = v;
            if (v <= lbl_eu_806681A0) {
                self->field_74 &= ~8;
            } else {
                f32 step = (v / lbl_eu_806681B4) * delta;
                for (int i = 0; i < 2; i++) {
                    if (self->field_70[i] != 0) {
                        f32 cur = func_80462F2C__8CTaskLODFv(self->field_70[i]);
                        f32 tgt = func_80462FF4__8CTaskLODFv(self->field_70[i]);
                        f32 s = cur + step;
                        if (tgt <= s)
                            s -= tgt;
                        func_80462EF4__8CTaskLODFv(self->field_70[i], s);
                    }
                }
                result = 1;
            }
        }

        // bit 4: +0x178 expiry; with the 0x40000000 flag the LODs are hidden
        // (alpha 0 / mode 0) before clearing the flag.
        if ((self->field_74 & 0x10) != 0) {
            f32 v = self->field_178 - delta;
            self->field_178 = v;
            if (v <= lbl_eu_806681A0) {
                u32 flags = self->field_74;
                self->field_74 = flags & ~0x10;
                if ((flags & 0x40000000) != 0) {
                    for (int i = 0; i < 2; i++) {
                        if (self->field_70[i] != 0) {
                            func_80462EF4__8CTaskLODFv(self->field_70[i],
                                                       lbl_eu_806681A0);
                            func_80462F4C__8CTaskLODFv(self->field_70[i], 0);
                        }
                    }
                    self->field_74 &= ~0x40000000;
                }
            }
            result = 1;
            self->field_74 &= ~0x20000;
        }

        // bit 5: +0x17C expiry.
        if ((self->field_74 & 0x20) != 0) {
            func_8020A010();
            f32 v = self->field_17C - delta;
            self->field_17C = v;
            if (v <= lbl_eu_806681A0)
                self->field_74 &= ~0x20;
            result = 1;
            self->field_74 &= ~0x20000;
        }
        return result;
    }
    return 0;
}

// func_801F6780 - per-step gimmick update. The +0xA4 step table (indexed by
// field_188, 16 bytes per entry) drives: a +0x170 activation countdown,
// camera events (func_8007B0C8), per-LOD frame updates (func_801F6B98), the
// +0x68 map-object status (func_800BE12C), a player-control reset (getPlayer
// slot 0x110 -> func_80199678), the area-manager attach (func_800817BC with
// vtable slots 0x9C/0xC4) and the step sound (func_801BFED0 / func_801BFC38 /
// func_80208C60 / func_80208C48, plus the func_801BFAE4 volume slot).
void func_801F6780(cf::CfGimmickObject* self) {
    u32 f = self->field_74;
    CfGimmickObjectStep* step = &self->field_A4[self->field_188];
    if ((f & 0x08000000) == 0) {
        if (step->field_00 != 0) {
            // u16 -> f32 (MWCC's 2^52 double-slot trick, fused fsubs).
            self->field_74 |= 1;
            self->field_170 = (f32)step->field_00;
        }
        if (step->field_02 != 0) {
            func_8007B0C8(step->field_02);
            if ((self->field_161 & 0x20) != 0)
                func_8020A0F8();
        }
    }
    for (int i = 0; i < 2; i++) {
        if (self->field_70[i] != 0)
            func_801F6B98(self, i, (const CfGimmickLodFrame*)step);
    }
    if (step->field_04 != 0 && self->field_68 != 0) {
        void* obj = func_80186BC8(self->field_68);
        if (obj != 0)
            func_800BE12C((u8*)obj, step->field_04, 0, step->field_06, 1);
    }
    if ((step->field_08 & 0x4) != 0) {
        func_80208EE4(self);
    } else if ((step->field_08 & 0x20) != 0) {
        if (self->field_78 != 0)
            self->field_180 = lbl_eu_806681B8;
    }
    if ((step->field_08 & 1) != 0) {
        self->field_74 |= 0x80000;
    } else if ((step->field_08 & 0x2) != 0) {
        self->field_74 &= ~0x80000;
    }
    if ((step->field_08 & 0x80) != 0) {
        for (int i = 1; i < 3; i++) {
            void* player = getPlayer__Q22cf13CfGameManagerFi(i);
            if (player != 0) {
                void* sub = ((void* (*)(void*))((CfGimmickObjectMoveIf*)player)->vtable[0x44])(player);
                if (sub != 0)
                    func_80199678(((CfGimmickMoveSub*)sub)->ctrl, 1);
            }
        }
    }
    if (step->field_07 != 0) {
        func_80208EE4(self);
        CfGimmickObjectMgr* mgr =
            func_800817BC__Q22cf13CfGameManagerFv(step->field_07, 0);
        self->field_78 = mgr;
        if (mgr != 0) {
            mgr->field_B0 = self;
            if ((step->field_08 & 0x10) != 0) {
                ((void (*)(void*, void*))mgr->vtable[0x27])(mgr, (void*)self->field_04);
                ((void (*)(void*, f32))mgr->vtable[0x31])(mgr, self->field_10.y);
            }
        }
    }
    // Step sound: stop the old handle first (field_0E bits 2..4 select the
    // stop delay), then either (re)start it (func_801BFABC != 0) or re-arm
    // the sound state for the next frame (field_190/field_192 setup).
    if ((step->field_0E & 0x1C) != 0) {
        if (self->field_80 != 0) {
            if ((step->field_0E & 0x4) != 0)
                func_801BFED0(1, self->field_80, 0xa);
            else if ((step->field_0E & 0x8) != 0)
                func_801BFED0(1, self->field_80, 0x1e);
            else if ((step->field_0E & 0x10) != 0)
                func_801BFED0(1, self->field_80, 0x3c);
            self->field_80 = 0;
        }
        self->field_74 &= ~0x100000;
    }
    if (step->field_0A != 0) {
        if (self->field_80 != 0)
            func_801BFED0(1, self->field_80, 0xa);
        if (func_801BFABC(1) != 0) {
            if ((step->field_0E & 1) != 0) {
                self->field_80 = func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(
                    1, step->field_0A, 0, 0, lbl_eu_806681A4);
            } else if ((step->field_0E & 0x20) != 0) {
                self->field_80 =
                    func_80208C60(step->field_0A, self->field_04, lbl_eu_806681BC);
            } else if ((step->field_0E & 0x40) != 0) {
                self->field_80 =
                    func_80208C60(step->field_0A, self->field_04, lbl_eu_806681B4);
            } else {
                self->field_80 =
                    func_80208C48(step->field_0A, self->field_04);
            }
            if (self->field_80 != 0xffff && (step->field_0E & 0x2) != 0)
                func_801BFF78(1, self->field_80, 0x10);
            if ((lbl_eu_806646BC & 0x8) != 0)
                self->field_74 |= 0x00800000;
        } else {
            self->field_74 &= ~0x600000;
            self->field_190 = step->field_0A;
            self->field_74 |= 0x100000;
            if ((step->field_0E & 1) != 0) {
                self->field_192 = 3;
            } else {
                if ((step->field_0E & 0x20) != 0)
                    self->field_192 = 2;
                else if ((step->field_0E & 0x40) != 0)
                    self->field_192 = 1;
                else
                    self->field_192 = 0;
                if ((step->field_0E & 0x2) != 0)
                    self->field_74 |= 0x200000;
            }
        }
    }
    self->field_74 &= ~0x20000000;
}

// func_801F6B98 - per-LOD frame update. Seeded from a +0x0C frame count,
// then dispatched on the +0x05 flag byte: 0x4/0x2 re-register the LOD task,
// 0x1/0x2 raise a LOD timer, 0x30 rebuilds the LOD2 range and 0x8 flips the
// LOD register mode. A +0x06 id byte change refreshes the +0x194 slot.
void func_801F6B98(cf::CfGimmickObject* self, u8 lod,
                   const CfGimmickLodFrame* frame) {
    f32 constv;
    f32 a;
    if (frame->field_0C != 0)
        // u16 -> f32 via the direct cast: MWCC emits the retail 2^52
        // double-magic idiom (lis 0x4330 stack-slot pair + lfd + fsubs).
        func_80462FD8__8CTaskLODFv(lod, (f32)frame->field_0C);
    if ((frame->field_05 & ~0x40) != 0) {
        u8 flags = frame->field_05;
        if ((flags & 0x4) != 0) {
            if ((flags & 0x2) != 0)
                func_80462EF4__8CTaskLODFv(lod, lbl_eu_806681A0);
            else
                func_80462F10__8CTaskLODFv(lod);
            func_80462F4C__8CTaskLODFv(lod, 0);
        } else if ((flags & 0x1) != 0) {
            func_80462F70__8CTaskLODFv(lod, 0);
            func_80462F4C__8CTaskLODFv(lod, 1);
            func_801F627C(self, lod, 1);
        } else if ((flags & 0x2) != 0) {
            func_80462F70__8CTaskLODFv(lod, 1);
            func_80462F4C__8CTaskLODFv(lod, 1);
            func_801F627C(self, lod, 0);
        } else if ((flags & 0x30) != 0) {
            func_80462F4C__8CTaskLODFv(lod, 0);
            if ((flags & 0x20) != 0) {
                // Rebuild the LOD2 range across both registered LOD tasks.
                constv = lbl_eu_806681A4;
                self->field_74 |= 0x8;
                self->field_174 = lbl_eu_806681B4;
                int i;
                u8* slot;
                for (i = 0; i < 2; i++) {
                    slot = &self->field_70[i];
                    if (*slot != 0) {
                        a = func_80462F2C__8CTaskLODFv(*slot);
                        f32 b = func_80462FF4__8CTaskLODFv(*slot);
                        f32 c = a + constv;
                        if (b <= c)
                            c -= b;
                        func_80462EF4__8CTaskLODFv(*slot, c);
                    }
                }
            }
        }
        if ((flags & 0x8) != 0)
            func_80462ED0__8CTaskLODFv(lod, 0);
        else
            func_80462ED0__8CTaskLODFv(lod, 1);
    }
    if (self->field_194 != frame->field_06) {
        self->field_194 = frame->field_06;
        func_80462F94__8CTaskLODFv(lod, frame->field_06);
    }
}

// func_801F6D8C - party-membership gate. With the 0x8000 flag of +0x152 set,
// returns 1 when any flagged character slot (bits 0..14) IS registered in the
// party (func_8009E284 != 0); otherwise returns 1 when any flagged slot is
// NOT registered. field_152 is re-read every iteration because the party
// queries may mutate it.
int func_801F6D8C(cf::CfGimmickObject* self) {
    u16 flags = self->field_152;
    if (flags != 0) {
        if ((flags & 0x8000) != 0) {
            for (int i = 0; i < 15; i++) {
                if ((self->field_152 & (1 << i)) != 0 &&
                    func_8009E284(func_8009ECB0(), i + 1) != 0)
                    return 1;
            }
        } else {
            for (int i = 0; i < 15; i++) {
                if ((self->field_152 & (1 << i)) != 0 &&
                    func_8009E284(func_8009ECB0(), i + 1) == 0)
                    return 1;
            }
        }
    }
    return 0;
}

// func_801F6E60 - state refresh dispatcher. The u8 argument selects a
// per-state re-init: cases 1..3 arm a +0x16C countdown (flag 0x2) and heal
// the linked map object, cases 4..6 arm the peer-gated countdown (flag 0x4),
// and cases 7/8 refresh the LOD registrations. Uses a jump table.
__declspec(noinline) void func_801F6E60(cf::CfGimmickObject* self, u8 arg) {
    // Retail emits an explicit equality test before the switch bounds check,
    // so state 0 must be guarded here instead of folded in as a case label.
    if (arg == 0)
        return;
    switch (arg) {
    case 1:
    case 9:
        self->field_18A = 30;
        self->field_74 |= 2;
        if (self->field_68 != 0) {
            ::CfGimmickObject* obj =
                (::CfGimmickObject*)func_80186BC8(self->field_68);
            if (obj != 0) {
                ((ICfGimmickObjectVt*)obj)->setMapObjValue(lbl_eu_806681A4);
                func_800BC3B0((cf::CfObjectMove*)obj, lbl_eu_806681B0);
            }
        }
        func_801F61B0(self, 1);
        self->field_16C = lbl_eu_806681C8 + (f32)self->field_18A;
        break;
    case 2:
        self->field_18A = 60;
        self->field_74 |= 2;
        if (self->field_68 != 0) {
            ::CfGimmickObject* obj =
                (::CfGimmickObject*)func_80186BC8(self->field_68);
            if (obj != 0) {
                ((ICfGimmickObjectVt*)obj)->setMapObjValue(lbl_eu_806681A4);
                func_800BC3B0((cf::CfObjectMove*)obj, lbl_eu_806681B0);
            }
        }
        func_801F61B0(self, 1);
        self->field_16C = lbl_eu_806681C8 + (f32)self->field_18A;
        break;
    case 3:
        self->field_18A = 90;
        self->field_74 |= 2;
        if (self->field_68 != 0) {
            ::CfGimmickObject* obj =
                (::CfGimmickObject*)func_80186BC8(self->field_68);
            if (obj != 0) {
                ((ICfGimmickObjectVt*)obj)->setMapObjValue(lbl_eu_806681A4);
                func_800BC3B0((cf::CfObjectMove*)obj, lbl_eu_806681B0);
            }
        }
        func_801F61B0(self, 1);
        self->field_16C = lbl_eu_806681C8 + (f32)self->field_18A;
        break;
    case 4:
        // Peer-busy gate written as a materialized 0/1 flag: retail builds
        // the condition into r0 (li/b/li chain) before testing it.
        {
            register int run =
                self->field_168 == 0 || (self->field_168->field_74 & 2) == 0;
            if (run != 0) {
                self->field_18A = 30;
                self->field_74 |= 4;
                if (self->field_68 != 0) {
                    ::CfGimmickObject* obj =
                        (::CfGimmickObject*)func_80186BC8(self->field_68);
                    if (obj != 0) {
                        ((ICfGimmickObjectVt*)obj)
                            ->setMapObjValue(lbl_eu_806681A4);
                        func_800BC3D8((cf::CfObjectMove*)obj, lbl_eu_806681B0);
                    }
                }
                func_801F61B0(self, 1);
                self->field_16C = (f32)self->field_18A;
            }
        }
        break;
    case 5:
        {
            register int run =
                self->field_168 == 0 || (self->field_168->field_74 & 2) == 0;
            if (run != 0) {
                self->field_18A = 60;
                self->field_74 |= 4;
                if (self->field_68 != 0) {
                    ::CfGimmickObject* obj =
                        (::CfGimmickObject*)func_80186BC8(self->field_68);
                    if (obj != 0) {
                        ((ICfGimmickObjectVt*)obj)
                            ->setMapObjValue(lbl_eu_806681A4);
                        func_800BC3D8((cf::CfObjectMove*)obj, lbl_eu_806681B0);
                    }
                }
                func_801F61B0(self, 1);
                self->field_16C = (f32)self->field_18A;
            }
        }
        break;
    case 6:
        {
            register int run =
                self->field_168 == 0 || (self->field_168->field_74 & 2) == 0;
            if (run != 0) {
                self->field_18A = 90;
                self->field_74 |= 4;
                if (self->field_68 != 0) {
                    ::CfGimmickObject* obj =
                        (::CfGimmickObject*)func_80186BC8(self->field_68);
                    if (obj != 0) {
                        ((ICfGimmickObjectVt*)obj)
                            ->setMapObjValue(lbl_eu_806681A4);
                        func_800BC3D8((cf::CfObjectMove*)obj, lbl_eu_806681B0);
                    }
                }
                func_801F61B0(self, 1);
                self->field_16C = (f32)self->field_18A;
            }
        }
        break;
    case 7:
        func_801F61B0(self, 1);
        break;
    case 8:
        {
            register int run =
                self->field_168 == 0 || (self->field_168->field_74 & 2) == 0;
            if (run != 0)
                func_801F61B0(self, 0);
        }
        break;
    }
}

// Counts players on the fight list whose +0x456C high nybble matches `id`.
// Written as a static helper so both recount sites inline identically: the
// manager stays in r3 like an incoming parameter while the sentinel head
// loads into a fresh register (retail keeps those distinct).
static int cfCountMatchingPlayers(CfGimmickList* mgr, int found) {
    CfGimmickListNode* node = mgr->head->next;
    int count = 0;
    // Fight list always holds the sentinel, so retail emits this without a
    // pre-test branch.
    do {
        char* obj = (char*)node->object;
        if (obj != NULL)
            obj -= 0x3E9C;
        if ((u32)(((CfPlayerIdView*)obj)->id456C >> 4) == (u32)found)
            count++;
        node = node->next;
    } while (node != mgr->head);
    return count;
}

// func_801F72A4 - availability scan for the +0x14A id table. With the 0x2000
// flag set, counts how many live players with a matching id (high nibble of
// the +0x456C byte, low nibble = bit index into field_184) remain; the
// 0x400000 flag is raised while any remain. When the count drops to zero the
// table scan index (field_18C) advances to the next non-zero entry, the id is
// spawned via func_80195B04, and a clean end of the table clears 0x2000 /
// 0x400000 and raises 0x80000000 ("done").
int func_801F72A4(cf::CfGimmickObject* self, u16* table) {
    u32 flags = self->field_74;
    if ((flags & 0x80000000) != 0)
        return 1;

    int found = 0;
    for (int i = self->field_18C; i < 3; i++) {
        if (table[i] != 0) {
            found = table[i];
            break;
        }
    }
    if (found == 0)
        goto done_reset;

    if ((flags & 0x2000) != 0) {
        if (self->field_196 <= 0) {
            // Recount the matching players and (re)arm both counters.
            int count = cfCountMatchingPlayers(func_800B6BC8(), found);
            self->field_196 = (s16)count;
            self->field_198 = (s16)count;
            self->field_184 = 0;
            return 0;
        }
        // Count down the matching players; `remaining` tracks every id match
        // (decremented before the hp query), field_196 only the freshly dead.
        CfGimmickList* list = func_800B6BC8();
        CfGimmickListNode* node = list->head->next;
        int remaining = self->field_196;
        f32 zero = lbl_eu_806681A0;
        while (node != list->head) {
            // List objects point 0x3E9C into the player base; the null case
            // keeps the raw node object and dereferences the offset anyway.
            CfPlayerBase* base = (CfPlayerBase*)node->object;
            if (base != 0)
                base = (CfPlayerBase*)((char*)base - 0x3E9C);
            // Nibble is read unconditionally (kept in a saved reg across the
            // vtable call); the high nybble selects matching players.
            u32 nibble = ((CfPlayerIdView*)base)->id456C & 0xF;
            // Signed compare here (retail cmpw), unlike the recount loops.
            if (((CfPlayerIdView*)base)->id456C >> 4 == found) {
                remaining--;
                f32 hp = base->getHP();
                if (hp <= zero) {
                    u32 bits = self->field_184;
                    u32 bit = 1u << nibble;
                    if ((bits & bit) == 0) {
                        self->field_184 = bits | bit;
                        self->field_196 = (s16)(self->field_196 - 1);
                    }
                }
            }
            node = node->next;
        }
        if (remaining > 0)
            self->field_74 |= 0x04000000;
        if (self->field_196 >= 1)
            goto state9190;
        // Table exhausted: advance field_18C to the next non-zero entry.
        // The reset tail is duplicated here in retail (not a shared block).
        {
            int newIndex = self->field_18C + 1;
            int foundFlag = 1;
            for (int i = newIndex; i < 3; i++) {
                if (table[i] != 0) {
                    self->field_18C = (s16)i;
                    found = table[i];
                    foundFlag = 0;
                    break;
                }
            }
            if (foundFlag != 0) {
                self->field_18C = 0;
                self->field_184 = 0;
                self->field_74 =
                    (self->field_74 & 0xFBFFDFFF) | 0x80000000;
                return 1;
            }
            goto spawn;
        }
state9190:
        {
            int cur = self->field_196;
            // True path jumps past the rename call so the shared return-0
            // tail lands after it (retail layout).
            if (self->field_198 == cur || remaining != cur)
                goto skip_rename;
        }
        func_80193678(found);
        self->field_196 = self->field_198;
        self->field_184 = 0;
        return 0;
skip_rename:
        return 0;
    }
spawn:
    self->field_74 |= 0x2000;
    if (func_80195B04(found) == 0)
        goto done_reset;
    {
        int count = cfCountMatchingPlayers(func_800B6BC8(), found);
        self->field_196 = (s16)count;
        self->field_198 = (s16)count;
        self->field_184 = 0;
    }
    return 0;
done_reset:
    self->field_18C = 0;
    self->field_74 = (self->field_74 & 0xFBFFDFFF) | 0x80000000;
    return 1;
}

// func_801F75CC - countdown/activation step. While flag 0x4000 is set,
// decrement +0x170 by the scene allocator's frame delta and keep returning 1
// until it drops to lbl_eu_806681A0 (then clear 0x4000 and return 0).
// Otherwise (re)start the countdown from lbl_eu_806681B8, play the entry
// sound for the old handle, refresh the handle via func_80208C48 and return 1.
int func_801F75CC(cf::CfGimmickObject* self) {
    if (self->field_158 != 0) {
        func_8020A010();
        u32 flags = self->field_74;
        if ((flags & 0x4000) != 0) {
            self->field_170 -= func_80496288(lbl_eu_80663E14);
            if (self->field_170 > lbl_eu_806681A0)
                return 1;
            self->field_74 &= ~0x4000;
        } else {
            // Volatile RMW keeps the flag raise a tight fresh lwz/ori/stw
            // group ahead of the independent timer store.
            u16 v = *(volatile u16*)&self->field_80;
            u32 f = *(volatile u32*)&self->field_74;
            f32 c = lbl_eu_806681B8;
            self->field_74 = f | 0x4000;
            self->field_170 = c;
            if (v != 0xffff)
                func_801BFED0(1, v, 0xa);
            self->field_80 = func_80208C48(self->field_158, self->field_04);
            if ((lbl_eu_806646BC & 0x8) != 0)
                self->field_74 |= 0x00800000;
            return 1;
        }
    }
    return 0;
}

// func_801F76A8 - step-machine cleanup: resolves the +0x6A area registration
// (func_8020A6B0 / func_8020A434), runs the +0x180 effect countdown
// (func_80496288 delta; on expiry notify func_80208EE4, otherwise scale the
// remaining time into func_800ACC64's position vector), and plays / refreshes
// the step sound selected by field_192 (func_801BFC38 for kind 3,
// func_80208C60 for kinds 2/1, func_80208C48 otherwise). The sound-slot
// volume (func_801BFAE4 +0x1C) is forced to 0/1 by the global 0x8 flag.
void func_801F76A8(cf::CfGimmickObject* self) {
    if ((self->field_74 & 0x40000) != 0 && (lbl_eu_806646BC & 0x8) == 0) {
        func_8020A6B0(&self->field_7C, (const CfGimmickVec3*)self->field_04,
                      self->field_6A, lbl_eu_806681C8, 0, 0);
        self->field_74 &= ~0x40000;
    } else {
        func_8020A434(&self->field_7C);
    }
    if (self->field_180 != lbl_eu_806681A0) {
        if (self->field_78 != 0) {
            // Bare-global constant uses keep MWCC's CSE pinning the limit
            // load to f0 (retail shape).
            f32 delta = func_80496288(lbl_eu_80663E14);
            // Bare-global constant uses keep MWCC's CSE pinning the limit
            // load to f0 (retail shape).
            f32 v = self->field_180 - delta;
            self->field_180 = v;
            if (v <= lbl_eu_806681A0) {
                self->field_180 = lbl_eu_806681A0;
                func_80208EE4(self);
            } else {
                f32 vec[4];
                vec[3] = v / lbl_eu_806681B8;
                vec[0] = lbl_eu_806681A4;
                vec[1] = lbl_eu_806681A4;
                vec[2] = lbl_eu_806681A4;
                func_800ACC64(self->field_78, vec);
            }
        } else {
            self->field_180 = lbl_eu_806681A0;
        }
    }
    if ((self->field_74 & 0x100000) != 0) {
        if (func_801BFABC(1) != 0) {
            u32 f = self->field_74;
            self->field_74 = f & ~0x100000;
            if ((f & 0x400000) != 0) {
                self->field_74 &= ~0x400000;
                if (lbl_eu_806646C0 == 0)
                    lbl_eu_806646C0 = func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(
                        1, 0xcb, 0, 0, lbl_eu_806681A4);
            } else if (self->field_192 == 3) {
                self->field_80 = func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(
                    1, self->field_190, 0, 0, lbl_eu_806681A4);
            } else if (self->field_192 == 2) {
                self->field_80 =
                    func_80208C60(self->field_190, self->field_04, lbl_eu_806681BC);
            } else if (self->field_192 == 1) {
                self->field_80 =
                    func_80208C60(self->field_190, self->field_04, lbl_eu_806681B4);
            } else {
                self->field_80 = func_80208C48(self->field_190, self->field_04);
            }
            if ((self->field_74 & 0x200000) != 0) {
                if (self->field_80 != 0xffff)
                    func_801BFF78(1, self->field_80, 0x10);
            }
            if ((lbl_eu_806646BC & 0x8) != 0)
                self->field_74 |= 0x00800000;
        }
    }
    if ((self->field_74 & 0x800000) != 0) {
        if (self->field_80 != 0) {
            CfGimmickSoundSlot* slot = func_801BFAE4(self->field_80);
            if (slot != 0) {
                if ((lbl_eu_806646BC & 0x8) != 0) {
                    slot->field_1C = lbl_eu_806681A0;
                } else {
                    slot->field_1C = lbl_eu_806681A4;
                    self->field_74 &= ~0x800000;
                }
            } else {
                self->field_80 = 0;
                self->field_74 &= ~0x800000;
            }
        } else {
            self->field_74 &= ~0x800000;
        }
    }
}

// func_801F7978 - area availability check. While the 0x80000 busy flag is
// set for the +0x6A area id, a registered/ready party (func_8020A5DC / the
// +0x7C slot) keeps the 0x20000 ready flag; a failed check clears it and the
// caller must not run. Otherwise the +0x6C..0x6E / +0x13C..0x142 sequence
// windows, the +0x15F/0x160 random window and the +0x161 mode bits gate the
// per-frame effect.
int func_801F7978(cf::CfGimmickObject* self) {
    u16 areaId = self->field_6A;
    if (areaId != 0 && (self->field_74 & 0x80000) != 0) {
        // Shared `li r3,0` merge block: the two fail paths branch to `zero`,
        // the ready paths set result=1 and jump straight to the test.
        int result;
        if ((self->field_74 & 0x20000) != 0) {
            if (func_8020A5DC() != 0)
                result = 1;
            else {
                self->field_74 &= ~0x20000;
                goto zero;
            }
        } else {
            if (func_8020A87C(self, (u32)self->field_7C.field_00) != 0) {
                func_8020A484(areaId);
                self->field_74 |= 0x20000;
                result = 1;
            } else
                goto zero;
        }
        goto test;
    zero:
        result = 0;
    test:
        if (result != 0)
            return 0;
    }
    if (self->field_6E != 0) {
        u32 seq = func_800822F4__Q22cf13CfGameManagerFv();
        if (self->field_6C > seq || seq > self->field_6E)
            return 0;
    }
    if (self->field_13C != 0 &&
        func_80082354__Q22cf13CfGameManagerFv(self->field_13C) != self->field_13E)
        return 0;
    if (self->field_140 != 0 &&
        func_8020971C((void*)(u32)self->field_140) == 0)
        return 0;
    if (self->field_142 != 0 &&
        func_802096EC((void*)(u32)self->field_142) == 0)
        return 0;
    if (self->field_15F != 0 || self->field_160 != 0) {
        u8 hi;
        u8 lo;
        u16 rnd = (u16)func_8006A33C();
        hi = self->field_160;
        lo = self->field_15F;
        if ((u32)lo > (u32)hi) {
            if (hi < rnd && rnd < lo)
                return 0;
        } else {
            if (lo > rnd && rnd > hi)
                return 0;
        }
    }
    if ((self->field_161 & 0x10) != 0 && (self->field_161 & 0x4) == 0)
        self->field_74 |= 0x80000;
    return 1;
}

// func_801F7B44 - availability gate for the +0x156 area id (mirrors
// func_801F7978's first block), then scans both +0x84 area tables against
// the scenario/sequence windows, and finally the +0x74 party flags: while
// the 0x8000/0x10000 bits are raised and the global 0x4 bit is clear, the
// step machine is (re)initialized via func_801F5C2C.
int func_801F7B44(cf::CfGimmickObject* self) {
    u16 areaId = self->field_156;
    if (areaId != 0 && (self->field_74 & 0x80000) != 0) {
        // Shared `li r3,0` merge block: the two fail paths branch to `zero`,
        // the ready paths set result=1 and jump straight to the test.
        int result;
        if ((self->field_74 & 0x20000) != 0) {
            if (func_8020A5DC() != 0)
                result = 1;
            else {
                self->field_74 &= ~0x20000;
                goto zero;
            }
        } else {
            if (func_8020A87C(self, (u32)self->field_7C.field_00) != 0) {
                func_8020A484(areaId);
                self->field_74 |= 0x20000;
                result = 1;
            } else
                goto zero;
        }
        goto test;
    zero:
        result = 0;
    test:
        if (result != 0)
            return 0;
    }
    // Walk the two +0x84 area-table entries with a pointer local (retail
    // keeps `addi r30,r31,0x84` + 0x10 stepping in the loop).
    CfGimmickObjectArea* area = self->field_84;
    for (int i = 0; i < 2; i++, area++) {
        if (area->field_06 != 0) {
            u32 seq = func_800822F4__Q22cf13CfGameManagerFv();
            if (area->field_04 > seq || seq > area->field_06)
                return 0;
        }
        if (area->field_00 != 0 &&
            func_80082354__Q22cf13CfGameManagerFv(area->field_00) != area->field_02)
            return 0;
        if (area->field_08 != 0 && func_8020971C((void*)(u32)area->field_08) == 0)
            return 0;
        if (area->field_0A != 0 && func_802096EC((void*)(u32)area->field_0A) == 0)
            return 0;
        if (area->field_0C != 0 && func_801587E8(area->field_0C) == 0)
            return 0;
    }
    if (func_801F6D8C(self) != 0)
        return 0;
    if ((self->field_74 & 0x8000) != 0) {
        if ((self->field_74 & 0x10000) != 0 && (lbl_eu_806646BC & 0x4) == 0) {
            func_801F5C2C(self, 0, 5);
            self->field_74 &= ~0x18000;
            func_8020974C(self->field_64, 1);
        }
        return 0;
    }
    return 1;
}

// func_801F7D38 - per-frame update. Party membership (func_801F6D8C) puts
// the step machine into state 1; while the +0x15E mode byte is active the
// +0x6C..0x6E and per-area +0x04/0x06 sequence windows must contain the
// current scenario sequence. The +0x66 flags then drive the spawn/effect
// gate (func_802098EC) and, on the fail path, the +0x6A area registration
// retry (func_8020A5DC / func_8020A87C).
int func_801F7D38(cf::CfGimmickObject* self) {
    u16 areaId;
    if (func_801F6D8C(self) != 0) {
        self->field_188 = 1;
        return 0;
    }
    if (self->field_15E == 1) {
        if (self->field_6E != 0) {
            u32 seq = func_800822F4__Q22cf13CfGameManagerFv();
            if (self->field_6C > seq || seq > self->field_6E) {
                self->field_188 = 0;
                return 0;
            }
        }
        // Walk the two +0x84 area-table entries with a pointer local (retail
        // keeps `addi r30,r31,0x84` + 0x10 stepping in the loop).
        CfGimmickObjectArea* area = self->field_84;
        for (int i = 0; i < 2; i++, area++) {
            if (area->field_06 != 0) {
                u32 seq = func_800822F4__Q22cf13CfGameManagerFv();
                if (area->field_04 > seq || seq > area->field_06) {
                    self->field_188 = 0;
                    return 0;
                }
            }
        }
    }
    if (func_802098EC(self->field_66, (cf::CfGimmick*)&self->field_1C,
                      (const CfGimmickVec3*)self->field_04,
                      (const f32*)&self->field_10, self->field_7C.field_00) != 0) {
        int i;
        CfGimmickObjectArea* area = self->field_84;
        for (i = 0; i < 2; i++, area++) {
            if (area->field_0C != 0 && area->field_0E != 0)
                func_80159C04(area->field_0C, 1);
        }
        if ((self->field_161 & 0x8) != 0)
            self->field_74 &= ~0xC0000;
        if ((self->field_66 & 0x20) != 0)
            func_8020974C(self->field_64, 1);
        return 1;
    }
    if ((self->field_66 & 0x4) != 0) {
        areaId = self->field_6A;
        if (areaId != 0 && (self->field_74 & 0x80000) != 0) {
            if ((self->field_74 & 0x20000) != 0) {
                if (func_8020A5DC() == 0)
                    self->field_74 &= ~0x20000;
            } else {
                if (func_8020A87C(self, (u32)self->field_7C.field_00) != 0) {
                    func_8020A484(areaId);
                    self->field_74 |= 0x20000;
                }
            }
        }
    }
    return 0;
}

// func_801F7F24 - spawn/effect driver. Runs the field_66 gate: 0x20 =
// collision check via func_802098EC (success re-registers the map object),
// otherwise a jumptable_eu_80535830[idx] checker is run against either the
// reference point, the object list (func_800B6BC8 / func_800B6BEC) or the
// players (slot 0xAC spot). The +0x74 flag 0x800 tail then drives the
// field_164 state machine (availability table / id-table effects).
int func_801F7F24(cf::CfGimmickObject* self) {
    if (func_801F6D8C(self) != 0)
        return 1;

    // NOTE: field_66 is re-read at every use on purpose -- retail reloads it
    // per site (MWCC CSE only within call-free stretches), keeping it out of
    // the callee-saved set.
    int result = 1;
    if ((self->field_66 & 0x20) != 0) {
        if (func_802098EC(self->field_66, (cf::CfGimmick*)&self->field_1C,
                          (const CfGimmickVec3*)self->field_04,
                          (const f32*)&self->field_10,
                          self->field_7C.field_00) != 0) {
            result = 1;
            func_8020974C(self->field_64, 0);
        } else {
            result = 0;
        }
    } else if (self->field_66 != 0 && self->field_138 != 0) {
        if ((self->field_66 & 1) != 0) {
            // Reference-point check only.
            if (jumptable_eu_80535830[self->field_138](
                    (cf::CfGimmick*)&self->field_F4, &lbl_eu_805765A0,
                    (const CfGimmickVec3*)self->field_04) != 0)
                result = 0;
        } else if ((self->field_66 & 0xC0) != 0) {
            // Scan the gimmick object list, then (bit 0x80) the players.
            // Hoisting only the table BASE (not the index) matches retail,
            // which keeps it in a callee-saved reg and reloads field_138
            // around every virtual/getPlayer call.
            CfGimmickList* list = func_800B6BC8();
            for (CfGimmickListNode* node = list->head->next;
                 node != list->head; node = node->next) {
                if (jumptable_eu_80535830[self->field_138](
                        (cf::CfGimmick*)&self->field_F4,
                        ((CfGimmickPlayerFace*)node->object)->d41(),
                        (const CfGimmickVec3*)self->field_04) != 0) {
                    result = 0;
                    break;
                }
            }
            if ((self->field_66 & 0x80) != 0) {
                if (result != 0) {
                if ((self->field_66 & 0x10) != 0) {
                    CfGimmickPlayerFace* p = (CfGimmickPlayerFace*)
                        getPlayer__Q22cf13CfGameManagerFi(0);
                    if (p != NULL &&
                        jumptable_eu_80535830[self->field_138](
                            (cf::CfGimmick*)&self->field_F4, p->d41(),
                            (const CfGimmickVec3*)self->field_04) != 0)
                        result = 0;
                } else {
                    for (int i = 0; i < 3; i++) {
                        CfGimmickPlayerFace* p = (CfGimmickPlayerFace*)
                            getPlayer__Q22cf13CfGameManagerFi(i);
                        if (p == NULL)
                            continue;
                        if (jumptable_eu_80535830[self->field_138](
                                (cf::CfGimmick*)&self->field_F4, p->d41(),
                                (const CfGimmickVec3*)self->field_04) != 0) {
                            result = 0;
                            break;
                        }
                    }
                }
                }
            }
        } else if ((lbl_eu_806646BC & 0x4) != 0) {
            // Minimap-list variant of the scan.
            CfGimmickList* list = func_800B6BEC();
            for (CfGimmickListNode* node = list->head->next;
                 node != list->head; node = node->next) {
                if (jumptable_eu_80535830[self->field_138](
                        (cf::CfGimmick*)&self->field_F4,
                        ((CfGimmickPlayerFace*)node->object)->d41(),
                        (const CfGimmickVec3*)self->field_04) != 0) {
                    result = 0;
                    break;
                }
            }
        } else if ((self->field_66 & 0x10) != 0) {
            CfGimmickPlayerFace* p = (CfGimmickPlayerFace*)
                getPlayer__Q22cf13CfGameManagerFi(0);
            if (p != NULL &&
                jumptable_eu_80535830[self->field_138](
                    (cf::CfGimmick*)&self->field_F4, p->d41(),
                    (const CfGimmickVec3*)self->field_04) != 0)
                result = 0;
        } else {
            for (int i = 0; i < 3; i++) {
                CfGimmickPlayerFace* p = (CfGimmickPlayerFace*)
                    getPlayer__Q22cf13CfGameManagerFi(i);
                if (p == NULL)
                    continue;
                if (jumptable_eu_80535830[self->field_138](
                        (cf::CfGimmick*)&self->field_F4, p->d41(),
                        (const CfGimmickVec3*)self->field_04) != 0) {
                    result = 0;
                    break;
                }
            }
        }
    }

    // Flag tail: only runs while the 0x800 gate is raised and the global
    // busy bit is clear.
    u32 f74 = self->field_74;
    if ((f74 & 0x800) == 0)
        return result;
    if ((lbl_eu_80663E28 & 0x20) != 0)
        return 0;

    if (self->field_164 == 1) {
        if (func_801F72A4(self, self->field_144) != 0) {
            result = 1;
            self->field_74 &= 0x7FFFFFFF;
            return result;
        }
        // Table not done: while the 0x04000000 countdown is armed and the
        // check above saw something, raise the mode bits from +0xC9 or step
        // the step counter back.
        if ((self->field_74 & 0x04000000) != 0 && result != 0) {
            u8 mode = self->field_C9;
            u32 flags = self->field_74 & ~0x04000000u;
            self->field_74 = flags;
            if ((mode & 0xE0) != 0) {
                self->field_74 = flags | 0x38000000;
                return 1;
            }
            self->field_188 = self->field_188 - 1;
        }
        return 0;
    } else if (self->field_164 == 2) {
        if ((self->field_74 & 0x2000000) == 0) {
            // First pass: despawn every listed id and arm the 0x20000 flag.
            self->field_18C = 0;
            u16* idp = self->field_144;
            for (int i = 0; i < 3; i++, ++idp)
                if (*idp != 0)
                    func_80195B04(*idp);
            self->field_74 |= 0x20000;
            return 0;
        }
        // Gated pass: scenario windows must be open.
        if (self->field_6E != 0) {
            u32 seq = func_800822F4__Q22cf13CfGameManagerFv();
            if (self->field_6C <= seq && seq <= self->field_6E)
                return 0;
        }
        if (self->field_13C != 0 &&
            func_80082354__Q22cf13CfGameManagerFv(self->field_13C) ==
                self->field_13E)
            return 0;
        CfGimmickObjectArea* e = self->field_84;
        for (int i = 0; i < 2; i++, ++e) {
            if (e->field_06 != 0) {
                u32 seq = func_800822F4__Q22cf13CfGameManagerFv();
                if (e->field_04 <= seq && seq <= e->field_06)
                    return 0;
            }
            if (e->field_00 != 0 &&
                func_80082354__Q22cf13CfGameManagerFv(e->field_00) ==
                    e->field_02)
                return 0;
        }
        // Collect up to 10 players per listed id and raise their hit flags.
        u16* idp = self->field_144;
        for (int k = 0; k < 3; k++, ++idp) {
            u16 id = *idp;
            if (id == 0)
                continue;
            CfGimmickPlayerFlags* hits[10];
            int count = 0;
            CfGimmickList* list = func_800B6BC8();
            for (CfGimmickListNode* node = list->head->next;
                 node != list->head; node = node->next) {
                char* obj = (char*)node->object;
                // retail keeps the raw pointer when the node object is null
                if (obj != NULL)
                    obj -= 0x3E9C;
                if ((u32)((s16)((CfGimmickPlayerBase*)obj)->field_456C >> 4) ==
                    (u32)id) {
                    hits[count] = (CfGimmickPlayerFlags*)obj;
                    count++;
                    if (count >= 10)
                        break;
                }
            }
            for (int j = 0; j < count; j++) {
                CfGimmickPlayerFlags* o = hits[j];
                o->field_3F04 |= 0x40;
                o->field_3F08 |= 0x800;
                if (o != NULL)
                    func_80080F44__Q22cf13CfGameManagerFv((char*)o + 0x3E9C);
            }
        }
    } else {
        if ((self->field_74 & 0x2000000) == 0) {
            self->field_18C = 0;
            u16* idp = self->field_144;
            for (int i = 0; i < 3; i++, ++idp)
                if (*idp != 0)
                    func_80195B04(*idp);
            self->field_74 |= 0x20000;
        }
    }
    return result;
}

int func_801F8564() { return 1; }

// Busy-gate: an object flagged busy in field_74 (bit 12) with the global
// busy bit (lbl_eu_80663E28 bit 5) set, or whose +0x14A table fails
// func_801F72A4, is NOT available (returns 0).
int func_801F856C(cf::CfGimmickObject* self) {
    if (self->field_74 & 0x1000) {
        if (lbl_eu_80663E28 & 0x20)
            return 0;
        if (func_801F72A4(self, self->field_14A) == 0)
            return 0;
    }
    return 1;
}

// Advance the +0x188 step counter (down while flagged busy, up otherwise,
// clamped to 0..6), clearing the working bits on the way out.
int func_801F85C4(cf::CfGimmickObject* self) {
    func_801F6780(self);
    if (self->field_74 & 0x08000000) {
        s16 cur = *(volatile s16*)&self->field_188;
        u32 flags = *(volatile u32*)&self->field_74;
        self->field_188 = cur - 1;
        self->field_74 = flags & ~0x08000000;
        if ((s16)(cur - 1) < 0)
            self->field_188 = 0;
    } else {
        s16 v = (s16)(self->field_188 + 1);
        self->field_188 = v;
        if (v > 6)
            self->field_188 = 6;
    }
    self->field_74 &= ~0x400;
    return 1;
}

// func_801F8658 - per-frame step/activation driver (continuation of
// func_801F85C4's step-machine). Runs the countdown via func_801F75CC; when
// it expires, re-seeds the +0x17C timer from +0x15A, refreshes the area
// checks (func_801F6E60 / func_801F6780), sets the busy bits and advances
// the +0x188 step counter (down while busy, up otherwise, clamped 0..6).
int func_801F8658(cf::CfGimmickObject* self) {
    if (func_801F75CC(self) != 0) {
        if (self->field_C4 != 0)
            func_80209F5C();
        return 0;
    }
        // u16 -> f32 via the direct cast: MWCC emits the retail 2^52
        // double-magic idiom (lis 0x4330 stack-slot pair + lfd + fsubs).
        // Local + split flag-update mirrors retail's interleaved schedule.
        // Residual: the pooled magic double is anonymous (@sda21) in decomp
        // vs retail's lbl_eu_806681C0 - needs a pool_patterns rename rule.
        u16 seed = self->field_15A;
        if (seed != 0) {
            u32 flags = self->field_74;
            f32 seedF = (f32)seed;
            self->field_74 = flags | 0x20;
            self->field_17C = seedF;
        }
    func_801F6E60(self, self->field_162);
    func_801F6780(self);
    if ((self->field_74 & 0x100) != 0) {
        func_8020A068((int)self, 1, 1);
        self->field_74 |= 0x500000;
    }
    if ((self->field_74 & 0x200) != 0 && (self->field_74 & 0x08000000) == 0 &&
        (self->field_74 & 0x3F) != 0)
        func_8020A0CC();
    if ((self->field_74 & 0x08000000) != 0) {
        // Volatile reload forces the second lwz of field_74 (retail does
        // not reuse the branch-test register inside the branch).
        int vi = self->field_188 - 1;
        u32 f = *(volatile u32*)&self->field_74;
        self->field_188 = (s16)vi;
        self->field_74 = f & ~0x08000000;
        if ((s16)vi < 0)
            self->field_188 = 0;    } else {
        s16 v = (s16)(self->field_188 + 1);
        self->field_188 = v;
        if (v > 6)
            self->field_188 = 6;
    }
    self->field_74 &= ~0x400;
    return 1;
}

// func_801F879C - per-frame gimmick update. The +0x15C value seeds a
// countdown in +0x17C (0x80 flag raised while it runs); the 0x200 gate
// funnels busy objects through func_8020A0CC. Afterwards the +0x66 0x20 gate
// runs func_801F75CC, the +0x163 id refreshes the LOD registration
// (func_801F6E60), the field_161 0x40 / global 0x4 combo resets the step
// machine (func_801F5C2C), and func_801F6780 drives the step table. The
// +0x188 counter advances like func_801F89B8's (down while the 0x08000000
// busy flag is set, up otherwise, clamped 0..6); the 0x200/0x400/0x80/
// 0x2000000 work flags are cleared on the way out.
int func_801F879C(cf::CfGimmickObject* self) {
    if (self->field_15C != 0) {
        func_8020A010();
        if ((self->field_74 & 0x80) != 0) {
            // Named locals mirror func_801F75CC's matched countdown.
            f32 delta = func_80496288(lbl_eu_80663E14);
            f32 cur = self->field_17C - delta;
            f32 limit = lbl_eu_806681A0;
            self->field_17C = cur;
            if (cur > limit) {
                if ((self->field_74 & 0x200) != 0 &&
                    (self->field_74 & 0x08000000) == 0)
                    func_8020A0CC();
                return 0;
            }
            self->field_17C = limit;
        } else {
            // u16 -> f32 via the 2^52 double-magic trick (union references
            // the named constant so the pool reloc matches retail).
            union {
                u32 w[2];
                f64 d;
            } conv;
            conv.w[1] = self->field_15C;
            conv.w[0] = 0x43300000;
            self->field_74 |= 0x80;
            self->field_17C = (f32)(conv.d - lbl_eu_806681C0);
            if ((self->field_74 & 0x200) != 0 &&
                (self->field_74 & 0x08000000) == 0)
                func_8020A0CC();
            return 0;
        }
    }
    if ((self->field_66 & 0x20) != 0) {
        if (func_801F75CC(self) != 0)
            return 0;
    }
    func_801F6E60(self, self->field_163);
    if ((self->field_161 & 0x40) != 0 && (lbl_eu_806646BC & 0x4) != 0) {
        func_801F5C2C(self, 0, 5);
        self->field_74 &= 0xFDFFFB7F;
        return 0;
    }
    func_801F6780(self);
    if ((self->field_74 & 0x100) != 0) {
        func_8020A068((int)self, 0, 0);
        if (lbl_eu_806646C0 != 0) {
            func_801BFED0(1, lbl_eu_806646C0, 0xa);
            lbl_eu_806646C0 = 0;
        }
    }
    if ((self->field_74 & 0x200) != 0 &&
        (self->field_74 & 0x08000000) == 0 &&
        (self->field_74 & 0x3F) != 0)
        func_8020A0CC();
    if ((self->field_74 & 0x08000000) != 0) {
        s16 v = (s16)(self->field_188 - 1);
        self->field_188 = v;
        self->field_74 &= ~0x08000000;
        if (v < 0)
            self->field_188 = 0;
    } else {
        s16 v = (s16)(self->field_188 + 1);
        self->field_188 = v;
        if (v > 6)
            self->field_188 = 6;
    }
    self->field_74 &= 0xFDFFFB7F;
    return 1;
}

// func_801F89B8 - per-frame gimmick update (sibling of func_801F879C). The
// +0x150 field seeds a short countdown in +0x18E (0x40 flag raised while it
// runs; 0x200 funnels busy objects into func_8020A0CC). Then the +0x64 map
// object is (re)spawned if missing, func_801F6780 drives the step table, and
// the +0x188 step counter advances (down while the 0x08000000 busy flag is
// set, up otherwise, clamped 0..6). The 0x200/0x400/0x8000/0x10000 work
// flags are cleared on the way out.
int func_801F89B8(cf::CfGimmickObject* self) {
    if (self->field_14A[3] != 0) {
        func_8020A010();
        if ((self->field_74 & 0x200) != 0)
            func_8020A0CC();
        if ((self->field_74 & 0x40) != 0) {
            // Signed halfword countdown; still positive means keep waiting.
            s16 v = (s16)(self->field_18E - 1);
            self->field_18E = v;
            if (v > 0)
                return 0;
            self->field_74 &= ~0x40;
        } else {
            self->field_74 |= 0x40;
            self->field_18E = self->field_14A[3];
            return 0;
        }
    }
    // Respawn the map object if the scene no longer tracks it.
    if (func_8020971C((void*)(u32)self->field_64) == 0) {
        func_80140E00(6, self->field_64, 0);
        func_8015B25C(self->field_64);
    }
    if (self->field_64 != 0 && (self->field_66 & 0x20) == 0)
        func_8020974C(self->field_64, 1);
    func_801F6780(self);
    int result = 0;
    u32 flags = self->field_74;
    if ((flags & 0x1000) != 0) {
        if ((flags & 0x08000000) != 0) {
            // Busy: count down and drop the busy bit (fresh flag load).
            s16 v = (s16)(self->field_188 - 1);
            u32 f = self->field_74;
            self->field_188 = v;
            self->field_74 = f & ~0x08000000;
            if (v < 0)
                self->field_188 = 0;
        } else {
            s16 v = (s16)(self->field_188 + 1);
            self->field_188 = v;
            if (v > 6)
                self->field_188 = 6;
        }
        result = 1;
    } else if (self->field_15E == 1) {
        if ((self->field_161 & 0x80) != 0)
            self->field_188 = 2;
        else
            self->field_188 = 0;
        if ((self->field_74 & 0x800) != 0 && self->field_164 == 1) {
            int i;
            cf::CfGimmickObject* o = self;
            for (i = 0; i < 3; i++) {
                if (o->field_144[i] != 0)
                    func_80193678(o->field_144[i]);
            }
        }
    } else {
        self->field_188 = 6;
    }
    self->field_74 &= 0xFFFE79FF;
    return result;
}

// func_801F8BB8 - state refresh. Set the +0x188 step from the +0x15E mode
// byte (2 when +0x161 bit 7 is set, else 0; 6 when not active), fire the
// id-table effects (func_80193678) for the +0x14A / +0x144 entries when the
// matching field_74 bits are raised, then clear the 0x80000000/0x400 work
// flags and report "not busy".
// func_801F8BB8 helpers: fire func_80193678 for the first three nonzero
// entries of one of the gimmick's u16 id tables.
static inline void fireTable14A(cf::CfGimmickObject* o) {
    for (int i = 0; i < 3; i++) {
        u16 id = o->field_14A[i];
        if (id != 0)
            func_80193678(id);
    }
}

static inline void fireTable144(cf::CfGimmickObject* o) {
    for (int i = 0; i < 3; i++) {
        u16 id = o->field_144[i];
        if (id != 0)
            func_80193678(id);
    }
}
int func_801F8BB8(cf::CfGimmickObject* self) {
    if (self->field_15E == 1) {
        if ((self->field_161 & 0x80) != 0)
            self->field_188 = 2;
        else
            self->field_188 = 0;
        if ((self->field_74 & 0x1000) != 0)
            fireTable14A(self);
        if ((self->field_74 & 0x800) != 0) {
            if (self->field_164 == 1)
                fireTable144(self);
        }
    } else {
        self->field_188 = 6;
    }
    self->field_74 &= 0x7FFFFBFF;
    return 0;
}
