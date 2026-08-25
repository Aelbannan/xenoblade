// Auto-scaffolded catalog TU for kyoshin/cf/CCharEffect
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CCharEffect.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/code_802B8A3C.hpp"
#include <string.h>

// The retail symbol __ct__CCharEffect is a plain (non-member) function, not
// a mangled member ctor: cf::CCharEffect's real dtor is
// __dt__Q22cf11CCharEffectFv elsewhere, and CfGameManagerUnityHelpers.hpp
// calls this ctor as a plain extern "C" function, so it cannot be a C++
// member ctor. Stores the vtable (lbl_eu_8052FDB8), clears the latch flags
// and the fixed words, then zeroes both 0xB0-byte slot arrays.
void* __ct__CCharEffect(CCharEffect* self) {
    self->mSlots[0] = (void*)lbl_eu_8052FDB8;
    self->mFlags = 0;
    self->mManager = NULL;
    self->mBattleObj = NULL;
    self->mField2A4 = 0;
    self->mField2A8 = 0;
    self->mField2AC = 0;
    self->mField2B0 = 0;
    self->mField2B4 = 0;
    self->mField2B8 = 0;
    self->mField2BC = 0;
    self->mField2C0 = 0;
    self->mField2C4 = 0;
    self->mField2C8 = 0;
    self->mField2FC = 0;
    memset(&self->mSlots[1], 0, 0xb0);
    memset(self->mSlots2, 0, 0xb0);
    return self;
}

void func_8015BB3C(){}

// func_8015BD24: walk the 44 effect slots (mSlots[1..44]) and forward a
// value to each non-NULL slot via its vtable method at offset 0x158.
void func_8015BD24(CCharEffect* self, u32 param) {
    for (u32 i = 0; i < 0x2c; i++) {
        CCharEffectSlot* obj = (CCharEffectSlot*)self->mSlots[i + 1];
        if (obj != NULL) {
            obj->v158(param);
        }
    }
}

// func_8015BD94: detach all 44 effect slots (mSlots[1..44]) and the 8
// entries of the 0x2CC list. Each non-NULL slot has its owner back-pointer
// cleared, its in-use flag (0x40) set, and the slot/twin entries NULLed.
// The loops walk 4 slots per group (same shape as func_eu_8015D258).
// func_8015BD94: detach all 44 effect slots (mSlots[1..44]) and the 8
// entries of the 0x2CC list. Each non-NULL slot has its owner back-pointer
// cleared, its in-use flag (0x40) set, and the slot/twin entries NULLed.
// The loops walk 4 slots per group; the step-3 trip counter next to the
// 4-slot walk (same shape as func_eu_8015D258) is kept by the retail
// build, so it is kept here too. Slots are re-read from the array for the
// flag update (retail reloads the pointer after the back-pointer store).
void func_8015BD94(CCharEffect* self) {
    void** p = self->mSlots;
    void** t = self->mSlots2;
    for (u32 i = 0; i < 0x21; i += 3) {
        for (u32 k = 0; k < 4; k++) {
            CCharEffectSlot* s = (CCharEffectSlot*)p[k + 1];
            if (s != NULL) {
                s->field_B0 = NULL;
                ((CCharEffectSlot*)p[k + 1])->field_68 |= 0x40;
                p[k + 1] = NULL;
                t[k] = NULL;
            }
        }
        p += 4;
        t += 4;
    }
    void** q = self->mSlotList2;
    for (u32 i = 0; i < 0x6; i += 3) {
        for (u32 k = 0; k < 4; k++) {
            CCharEffectSlot* s = (CCharEffectSlot*)q[k];
            if (s != NULL) {
                s->field_B0 = NULL;
                ((CCharEffectSlot*)q[k])->field_68 |= 0x40;
                q[k] = NULL;
            }
        }
        q += 4;
    }
}

// func_8015BF04: remove a slot object from the effect lists. Clears the
// owner back-pointer at slot+0xB0 first, then scans the 44 primary slots
// (mSlots[1..44]): on a match the twin entry and the slot pointer are
// cleared, and unless the slot's removal-guard byte (+0xB4) is set (or the
// index is above 9) the matching bit is set in the 0x2A8/0x2BC word. Falls
// back to the 8-entry list at 0x2CC.
void func_8015BF04(CCharEffect* self, CCharEffectSlot* p) {
    p->field_B0 = NULL;
    for (u32 i = 0; i < 0x2c; i++) {
        if (self->mSlots[i + 1] == p) {
            u32 bit = 1u << i;
            u32* dst = &self->mField2A8;
            if (i >= 0x20) {
                bit = 1u << (i - 0x20);
                dst = &self->mField2BC;
            }
            self->mSlots[i + 1] = NULL;
            self->mSlots2[i] = NULL;
            if (p->field_B4 != 0) return;
            if (i > 9) return;
            *dst |= bit;
            return;
        }
    }
    for (u32 i = 0; i < 8; i++) {
        if (self->mSlotList2[i] == p) {
            self->mSlotList2[i] = NULL;
            return;
        }
    }
}

// func_8015BFCC: register a slot value at index idx. Sets the bitmap bit
// (OR into 0x2AC/0x2C0) while clearing the two mirror words
// (0x2B0/0x2B4 or 0x2C4/0x2C8), defaulting a NULL value to
// lbl_eu_80664228. Also stores the per-slot u16/u8 data and latches
// mFlags bit 1.
void func_8015BFCC(CCharEffect* self, u32 idx, CCharEffectSlot* value, u16 a, u16 b, u8 c) {
    u32 bit = 1u << idx;
    u32* dstOr = &self->mField2AC;
    u32* dstClr1 = &self->mField2B0;
    u32* dstClr2 = &self->mField2B4;
    if (idx >= 0x20) {
        bit = 1u << (idx - 0x20);
        dstOr = &self->mField2C0;
        dstClr1 = &self->mField2C4;
        dstClr2 = &self->mField2C8;
    }
    *dstOr |= bit;
    *dstClr1 &= ~bit;
    *dstClr2 &= ~bit;
    if (value == NULL) {
        value = (CCharEffectSlot*)lbl_eu_80664228;
    }
    self->mSlots2[idx] = value;
    self->mField_164[idx] = a;
    self->mField_214[idx] = b;
    self->mField_26E[idx] = c;
    self->mFlags |= 2;
}

// func_8015C074: set one bit of the effect bitmap. Bits 0-31 live in the
// word at 0x2B4, bits 32+ in the word at 0x2C8; mFlags bit1 is latched.
void func_8015C074(CCharEffect* self, u32 idx) {
    u32 bit = 1u << idx;
    u32* dst = &self->mField2B4;
    if (idx >= 0x20) {
        idx -= 0x20;
        bit = 1u << idx;
        dst = &self->mField2C8;
    }
    *dst |= bit;
    self->mFlags |= 2;
}

// func_8015C0B0: clear a bit in one word and set it in another. For
// idx < 32 the clear/set words are at 0x2AC/0x2B0, for idx >= 32 at
// 0x2C0/0x2C4; mFlags bit1 is latched.
void func_8015C0B0(CCharEffect* self, u32 idx) {
    u32 bit = 1u << idx;
    u32* set = &self->mField2B0;
    u32* clr = &self->mField2AC;
    if (idx >= 0x20) {
        idx -= 0x20;
        bit = 1u << idx;
        set = &self->mField2C4;
        clr = &self->mField2C0;
    }
    *clr &= ~bit;
    *set |= bit;
    self->mFlags |= 2;
}

// func_8015C100: release the effect slots 0x0A..0x10. For each index the
// slot is detached (clear back-pointer, set flag 0x40, NULL the slot), the
// twin entry and per-slot s16/u8 tables are reset (0xFFFF/-1 for the first
// two s16 tables, 0 for the third and the u8 table), and the five bitmap
// words (0x2A4/0x2B4/0x2A8/0x2AC/0x2B0 or the 0x2B8/0x2C8/0x2BC/0x2C0/
// 0x2C4 mirror set) have the bit cleared.
void func_8015C100(CCharEffect* self) {
    for (u32 idx = 0xa; idx < 0x11; idx++) {
        u32 bit = 1u << idx;
        u32* w2 = &self->mField2B4;
        u32* w1 = &self->mField2A4;
        u32* w3 = &self->mField2A8;
        u32* w4 = &self->mField2AC;
        u32* w5 = &self->mField2B0;
        if (idx >= 0x20) {
            bit = 1u << (idx - 0x20);
            w1 = &self->mField2B8;
            w2 = &self->mField2C8;
            w3 = &self->mField2BC;
            w4 = &self->mField2C0;
            w5 = &self->mField2C4;
        }
        CCharEffectSlot* slot = (CCharEffectSlot*)self->mSlots[idx + 1];
        if (slot != NULL) {
            ((CCharEffectSlot*)self->mSlots[idx + 1])->field_B0 = NULL;
            ((CCharEffectSlot*)self->mSlots[idx + 1])->field_68 |= 0x40;
            self->mSlots[idx + 1] = NULL;
        }
        self->mSlots2[idx] = NULL;
        self->mField_1BC[idx] = -1;
        self->mField_164[idx] = -1;
        *w1 &= ~bit;
        *w2 &= ~bit;
        *w3 &= ~bit;
        *w4 &= ~bit;
        *w5 &= ~bit;
        self->mField_214[idx] = 0;
        self->mField_26E[idx] = 0;
    }
}

// func_eu_8015D258: for each of the 11 slot groups (4 slots per 0x10 block)
// push f1 into each non-NULL slot's sub-object float at +0x4C. The EU build
// keeps a step-3 trip counter alongside the 4-slot group walk.
void func_eu_8015D258(CCharEffect* self, f32 f1) {
    for (u32 i = 0, j = 0; i < 0x21; i += 3, j += 4) {
        CCharEffectSlot* obj = (CCharEffectSlot*)self->mSlots[j + 1];
        if (obj != NULL) {
            CCharEffectSlotSub* sub = (CCharEffectSlotSub*)obj->field_94;
            if (sub != NULL) {
                sub->field_4C = f1;
            }
        }
        obj = (CCharEffectSlot*)self->mSlots[j + 2];
        if (obj != NULL) {
            CCharEffectSlotSub* sub = (CCharEffectSlotSub*)obj->field_94;
            if (sub != NULL) {
                sub->field_4C = f1;
            }
        }
        obj = (CCharEffectSlot*)self->mSlots[j + 3];
        if (obj != NULL) {
            CCharEffectSlotSub* sub = (CCharEffectSlotSub*)obj->field_94;
            if (sub != NULL) {
                sub->field_4C = f1;
            }
        }
        obj = (CCharEffectSlot*)self->mSlots[j + 4];
        if (obj != NULL) {
            CCharEffectSlotSub* sub = (CCharEffectSlotSub*)obj->field_94;
            if (sub != NULL) {
                sub->field_4C = f1;
            }
        }
    }
}

// func_8015C214: remove p from the 44-slot list (clearing the matching
// twin entry), else from the 8-slot list at 0x2CC.
void func_8015C214(CCharEffect* self, void* p) {
    for (u32 i = 0; i < 0x2c; i++) {
        if (self->mSlots[i + 1] == p) {
            self->mSlots[i + 1] = NULL;
            self->mSlots2[i] = NULL;
            return;
        }
    }
    for (u32 i = 0; i < 8; i++) {
        if (self->mSlotList2[i] == p) {
            self->mSlotList2[i] = NULL;
            return;
        }
    }
}

bool func_8015C294(unsigned int* param1, int param2) {
    return param1[param2 + 1] != 0;
}

// func_8015C2B0: apply an effect to a target object. Resolves the effect
// data source from the manager, ORs the (flags>>7)&1 bit with the result
// of func_80053F40 (data holder + 0x10, type&0xFF) to get a boolean flag,
// then attaches the target to the manager, drives its vtable-0x194 method
// with the flag, and dispatches a per-type handler (byte table
// lbl_eu_80501DF8: 1 = copy the data string into the target, 2 = scale
// the target's vtable-0xDC argument by lbl_eu_80667530 * data->field_2E8).
void func_8015C2B0(CCharEffect* self, CCharEffectVTableIf* eff, u32 type, u32 flags) {
    CCharEffectData* data = (CCharEffectData*)((CCharEffectMgr*)self->mManager)->field_98;
    if (data == NULL) return;

    u8* p = ((CCharEffectMgr*)self->mManager)->field_C4;
    p += 0x10;
    u32 x = ((flags >> 7) & 1) | func_80053F40(p, type & 0xFF);
    u32 bitFlag = (x != 0);
    char* name = ((CCharEffectData*)((CCharEffectMgr*)self->mManager)->field_98)->v018();
    if (strstr(name, lbl_eu_80501E38) != NULL && type == 3) {
        bitFlag = 1;
    }

    func_800ACF78(eff, (CCharEffectMgr*)self->mManager, 0);
    eff->v194(bitFlag);

    s8 v = lbl_eu_80501DF8[type];
    if (v == 1) {
        CCharEffectData* d = (CCharEffectData*)((CCharEffectMgr*)self->mManager)->field_98;
        func_800ACEF8(eff, &d->field_304);
    } else if (v == 2) {
        CCharEffectData* d = (CCharEffectData*)((CCharEffectMgr*)self->mManager)->field_98;
        eff->v0DC(lbl_eu_80667530 * d->field_2E8);
    }

    func_80484EB0((u8*)((CCharEffectMgr*)self->mManager)->field_98);
    func_800ACC50((u8*)eff);
}

void func_8015C404(){}

// func_8015C8F4: detach every effect slot whose sub-object id (+0x14)
// matches the manager's vtable-0xA8 result. Each match clears the slot
// back-pointer and sub-object id, sets slot flag 0x40, and NULLs the slot
// pointer. Returns early when no manager is given.
void func_8015C8F4(CCharEffect* self, CCharEffectVTableIf* manager) {
    u32 id;
    if (manager == NULL) return;
    void** p = (void**)self;
    for (u32 i = 0; i < 0x2c; i++) {
        CCharEffectSlot* slot = (CCharEffectSlot*)p[1];
        if (slot != NULL) {
            CCharEffectSlotSub* sub = (CCharEffectSlotSub*)slot->field_94;
            if (sub != NULL) {
                id = sub->field_14;
                if (id == manager->v0A8()) {
                    ((CCharEffectSlot*)p[1])->field_B0 = NULL;
                    ((CCharEffectSlotSub*)((CCharEffectSlot*)p[1])->field_94)->field_14 = 0;
                    ((CCharEffectSlot*)p[1])->field_68 |= 0x40;
                    p[1] = NULL;
                }
            }
        }
        p++;
    }
}

void func_8015C9A0(){}

int func_8015CB88() { return 1; }

// func_8015CB90: 1 when the current game time is not 4.
int func_8015CB90() {
    return cf::CfGameManager::func_80086DBC() != 4;
}

// func_8015CBC0: 1 when the current game time is 4.
int func_8015CBC0() {
    return cf::CfGameManager::func_80086DBC() == 4;
}

// func_8015CBEC: resolve the current actor id from the battle object's
// sub-object (vtable 0x30) and query its flags via func_80174C98
// (selector 0x802). Returns 1 when no battle object is present.
int func_8015CBEC(CCharEffect* self) {
    cf::CCharEffectBattleObj* battleObj = self->mBattleObj;
    if (battleObj != NULL) {
        int id = *battleObj->field_04->bf30();
        return func_80174C98(battleObj, &id, 0x802);
    }
    return 1;
}

// func_8015CC50: like func_8015CBEC but queries selector 0x803 and returns
// 0 when no battle object is present.
int func_8015CC50(CCharEffect* self) {
    cf::CCharEffectBattleObj* battleObj = self->mBattleObj;
    if (battleObj != NULL) {
        int id = *battleObj->field_04->bf30();
        return func_80174C98(battleObj, &id, 0x803);
    }
    return 0;
}

int func_8015CCB4() { return 0; }
