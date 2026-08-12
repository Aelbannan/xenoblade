// Auto-scaffolded catalog TU for kyoshin/cf/CfResReloadImpl
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectModel.hpp"
#include "kyoshin/cf/CfResReloadImpl.hpp"
#include "monolib/math/Random.hpp"

void* memset(void*, int, unsigned long);

// Retail ctor symbol __ct__cf_CfResReloadImpl uses the legacy flattened
// mangling, which a real member ctor cannot reproduce (MWCC would emit
// __ct__Q22cf15CfResReloadImplF...); names starting with "__" are emitted
// verbatim, so the literal name is used (same as the CREvtObj precedent).
cf::CfResReloadImpl* __ct__cf_CfResReloadImpl(cf::CfResReloadImpl* self, cf::CfResReloadParent* parent) {
    self->field_00 = parent;
    self->field_04 = lbl_eu_80667698;
    self->field_0A = -1;
    self->field_0C = 0;
    self->field_0E = -1;
    self->field_10 = lbl_eu_80530FF0;
    self->field_1C = 3;
    self->field_1E = 0;
    self->field_1F = 0;
    self->field_08 = 0;  // retail writes the type field last
    memset(&self->field_14, 0, sizeof(self->field_14));
    return self;
}

// EQUIVALENT_MATCH: checks if type field at offset +8 is >= 3
int checkTypeIsValid(cf::CfResReloadImpl* arg) {
    unsigned int v = *(unsigned short*)((char*)arg + 8);
    return (int)(((v | 0xFFFFFFFCu) - ((v - 3u) >> 1)) >> 31);
}

// Checks whether the current reload resource is in use: when the parent's
// +0x64 flag bit 2 is set, the timer has expired, the type field is 1 and the
// parent's +0x6C flag bit 5 is set, the lookup entry's +0x04 pointer must
// match the parent resource (+0x70) and the +0x40 vtable slot of the entry
// object must be nonzero. Otherwise returns the normalized (field_08 >= 3)
// flag computed by checkTypeIsValid's expression.
int func_8016CE5C(cf::CfResReloadImpl* self) {
    cf::CfResReloadParent* par = self->field_00;
    if (!(par->field_64 & 0x4)) {
        goto fail;
    }
    if (!(self->field_04 > lbl_eu_80667698)) {
        goto fail;
    }
    if (self->field_08 != 1) {
        goto fail;
    }
    if (!(par->field_6C & 0x20)) {
        goto fail;
    }
    {
        cf::CfResLookupEntry* e = func_80062EC4(self->field_0A);
        u8* f4 = e->field_04;
        if (f4 == self->field_00->field_70) {
            cf::CfResEntryObjIf* obj = e->field_2C;
            if (obj->_v040() != 0) {
                return 1;
            }
        }
    }
fail:
    u32 t = self->field_08;
    return (int)(((t | 0xFFFFFFFCu) - ((t - 3u) >> 1)) >> 31);
}

// FULL_MATCH: vtable slot 6 - returns constant resource type identifier
int getTypeId() { return 12; }

// Computes a pseudo-random reload byte: calls the secondary-interface slot
// +0x34 with (this, 1), and when both that result and func_80063A60 are
// nonzero, hashes the value (multiply-high + correction) into field_1F.
// Returns the byte, or 1 when it is zero.
int func_8016CF24(cf::CfResReloadImpl* self) {
    int v = ((cf::CfResReloadVtIf*)self)->_v034(1);
    if (self->field_1F == 0 && v != 0) {
        int h = func_80063A60(v);
        if (h != 0) {
            // retail: addis/subi for h+0xBCFFF, lis/addi magic, mulhwu (magic
            // first) + subf/srwi/add correction + extrwi; t is a CSE'd temp
            u32 m = 0x5AC056B1u;
            u32 q1 = __mulhwu(m, h + 0xBCFFF);
            self->field_1F =
                (u8)(((((h + 0xBCFFF - q1) >> 1) + q1) >> 19) & 0xFF);
        }
    }
    u8 f = self->field_1F;
    if (f == 0) {
        return 1;
    }
    return f;
}

// Getter for the 2-entry work-word array at +0x14 (index guarded to < 2;
// out-of-range reads return 0).
u32 func_8016CFBC(cf::CfResReloadImpl* self, int index) {
    if (index < 2) {
        return self->field_14[index];
    }
    return 0;
}

// Resolves the current state's attribute id (field_0A) and, when valid,
// looks up the sound slot; if a live sound object exists, forwards arg3 to
// nw4r BasicSound::SetPlayerPriority. f1/f2 are passed through to
// func_801BFE20.
void func_8016CFDC(cf::CfResReloadImpl* self, int arg2, int arg3, float f1, float f2, int priority) {
    s16 v = self->field_0A;
    int id = -1;
    if (v < 0) {
        id = -1;
    } else {
        cf::CfResLookupEntry* e = func_80062EC4(v);
        if (e->field_00 & 0x800) {
            id = e->field_32 + 5;
        }
    }
    if (id != -1) {
        cf::SoundSlotEntry* s =
            func_801BFAE4((u16)func_801BFE20(id, arg2, self->field_00->field_74, f1, f2));
        if (s != 0 && priority != 0 && s->field_00 != 0) {
            s->field_00->SetPlayerPriority(priority);
        }
    }
}

// Resolves the current state's attribute id (field_0A) and, when valid,
// forwards it with two caller args to func_801BFE8C. Void return - retail
// sets up no return value at the epilogue.
void func_8016D0C0(cf::CfResReloadImpl* self, int arg2, int arg3) {
    s16 v = self->field_0A;
    int r = -1;
    if (v < 0) {
        r = -1;
    } else {
        cf::CfResLookupEntry* e = func_80062EC4(v);
        if (e->field_00 & 0x800) {
            r = e->field_32 + 5;
        }
    }
    if (r != -1) {
        func_801BFE8C(r, arg2, arg3);
    }
}

// Same attribute-id resolution as func_8016D0C0, forwarding the id with
// three caller args to func_801BFF04. Void return.
void func_8016D144(cf::CfResReloadImpl* self, int arg2, int arg3, int arg4) {
    s16 v = self->field_0A;
    int r = -1;
    if (v < 0) {
        r = -1;
    } else {
        cf::CfResLookupEntry* e = func_80062EC4(v);
        if (e->field_00 & 0x800) {
            r = e->field_32 + 5;
        }
    }
    if (r != -1) {
        func_801BFF04(r, arg2, arg3, arg4);
    }
}

// Returns the attribute id (entry +0x32 + 5) for the current state index, or
// -1 when the index is invalid or the entry's +0x800 flag is clear. The
// redundant r = -1 on the invalid-index path and the trailing != -1 check
// are both present in retail.
int func_8016D1D8(cf::CfResReloadImpl* self) {
    s16 v = self->field_0A;
    int r = -1;
    if (v < 0) {
        r = -1;
    } else {
        cf::CfResLookupEntry* e = func_80062EC4(v);
        if (e->field_00 & 0x800) {
            r = e->field_32 + 5;
        }
    }
    if (r == -1) {
        goto invalid;
    }
normal:
    return r;
invalid:
    return -1;
}

// Iterates the two secondary-interface slots (+0x34 with 0 and 1); when a
// slot reports a live resource id, searches the device table via
// func_80068928 and bumps the matching entry's +0x38/+0x3A counters (the
// +0x3A bump only when parent flag bit 0 is set).
void func_8016D240(cf::CfResReloadImpl* self) {
    void* inst = CfRes_getInstanceField();
    if (inst == 0) {
        return;
    }
    for (int i = 0; i < 2; i++) {
        int v = ((cf::CfResReloadVtIf*)self)->_v034(i);
        if (v != 0) {
            cf::DeviceSearchEntry* e = func_80068928((u8*)inst, (u32)v, 0x59, 0x61);
            if (e != 0) {
                e->field_38++;
                if (self->field_00->field_6C & 1) {
                    e->field_3A++;
                }
            }
        }
    }
}

// Picks the reload lookup param (5/3/3/4) from a global flag and two parent
// flag words, then queries the table with the caller's arg2 forwarded as the
// middle parameter; sets parent flag bits 0x30 on success and returns whether
// the query succeeded (retail neg/or/srwi normalize reused for branch+return).
int func_8016D2FC(cf::CfResReloadImpl* self, int arg2) {
    s16 v = self->field_0A;
    int p = 5;
    if (lbl_eu_80663E28 & 0x20) {
        p = 3;
    } else if (self->field_00->field_68 & 0x04000000) {
        p = 3;
    } else if (self->field_00->field_64 & 0x10000) {
        p = 4;
    }
    int ret = func_80062998(v, arg2, p);
    // retail computes the != 0 test as a 0/1 value (neg/or/rlwinm) once and
    // reuses it for the branch and the return value.
    int ok = (u32)(-ret | ret) >> 31;
    if (ok) {
        self->field_00->field_6C |= 0x30;
    }
    return ok;
}

// Picks the lookup param (5 or 3) from parent flag bit 26, queries the
// table with the caller's arg2 forwarded, sets parent flag bit 4 when the
// query succeeds, and returns whether it succeeded.
int func_8016D390(cf::CfResReloadImpl* self, int arg2) {
    s16 v = self->field_0A;
    int p = 5;
    if (self->field_00->field_68 & 0x04000000) {
        p = 3;
    }
    int ret = func_80062998(v, arg2, p);
    // retail computes the != 0 test as a 0/1 value (neg/or/srwi) once and
    // reuses it for the branch and the return value.
    int ok = (u32)(-ret | ret) >> 31;
    if (ok) {
        self->field_00->field_6C |= 0x10;
    }
    return ok;
}

// Reload driver: gated by the game-manager state flag, either decrements the
// reload counter or performs a full reload: dispatch the secondary-interface
// slot +0x34, query the resource instance table (findResEntry /
// func_80068564 depending on parent flag bit 0x10000), install the found
// row's values into field_0C/field_0A, set parent flag bits 0x30 and dispatch
// the +0x10 vtable slot. A failed lookup can instead seed the 0x40 reload
// flag and run the effect helper. All parent accesses go through
// self->field_00 directly (retail reloads it at every site).
extern "C" void func_8016D3F8(cf::CfResReloadImpl* self) {
    u8* inst;
    int v;
    cf::ResReloadFindEntry* found;
    ((cf::CfResParentVtIf*)self->field_00)->_v17C();
    func_800BBB50((cf::CfObjectModel*)self->field_00);
    ((cf::CfResParentVtIf*)self->field_00)->_v178();
    self->field_00->field_90 = 0;
    self->field_00->field_94 = 0;
    if (((cf::CfGameManager*)self->field_00)->func_80082900() == 0) {
        return;
    }
    if (!(self->field_00->field_68 & 0x100000)) {
        return;
    }
    u32 f6c = self->field_00->field_6C;
    if (f6c & 0x2) {
        self->field_00->field_6C &= ~0x3;
        return;
    }
    if (!(f6c & 0x1)) {
        return;
    }
    if (self->field_1C > 0) {
        self->field_1C--;
        return;
    }
    v = ((cf::CfResReloadVtIf*)self)->_v034(1);
    inst = (u8*)CfRes_getInstanceField();
    if (inst == 0) {
        return;
    }
    int r5 = (self->field_00->field_64 >> 16) & 1;
    if (r5 == 0) {
        if ((lbl_eu_80663E24 & 0x02000000) || (lbl_eu_80663E24 & 0x400)) {
            return;
        }
        if (lbl_eu_80663E24 & 0x40000) {
            return;
        }
    }
    int p = 5;
    if (lbl_eu_80663E28 & 0x20) {
        p = 3;
    } else if (self->field_00->field_68 & 0x04000000) {
        p = 3;
    } else if (r5 != 0) {
        p = 4;
    }
    int ret;
    if (r5 != 0) {
        ret = func_80062B3C(v, p);
    } else {
        ret = func_80062BAC(v);
    }
    found = 0;
    if (ret != 0) {
        u32 spC;
        u32 sp8;
        if (self->field_00->field_64 & 0x10000) {
            found = findResEntry(inst, v, &spC, &sp8);
        } else {
            found = func_80068564(inst, v, &spC, &sp8);
        }
        if (found != 0) {
            self->field_0C = (u16)spC;
            self->field_0A = (u16)sp8;
            self->field_00->field_6C |= 0x30;
            self->field_08 = 1;
            ((cf::CfResReloadVtIf*)self)->_v010();
            found->field_0C = 0;
        }
    }
    if (found == 0) {
        if (cf::CfGameManager::func_800829B8() == 0) {
            if (lbl_eu_80663E28 & 0x40) {
                if (self->field_00->field_64 & 0x4) {
                    func_80063A60(v);
                    lbl_eu_80663E28 |= 0x40;
                }
            }
        }
    }
    if (func_800B4A24(self->field_00) != 0) {
        if (found == 0) {
            func_800B1BBC(1);
        }
    }
}

// Reload advance: dispatches the secondary-interface +0x28 slot when parent
// flag bit 1 is set or the entry's resource pointer no longer matches, then
// runs the reload machinery: in-use validation (+0x40 slot), game-manager
// flag gates, timer decrement, resource-object creation (slots +0x18/+0x1C),
// entry re-install (+0x28) and the object/sound cleanup tail. The `ok` bit
// (r29) is also reused for the +0x68 flag extracted near the end.
extern "C" void func_8016D688(cf::CfResReloadImpl* self) {
    cf::CfResLookupEntry* entry = func_80062EC4(self->field_0A);
    int ok = 1;
    u32 f6c = self->field_00->field_6C;
    if ((f6c & 0x2) != 0) {
        ((cf::CfResReloadVtIf*)self)->_v028();
        return;
    }
    if (entry->field_04 != self->field_00->field_70) {
        ((cf::CfResReloadVtIf*)self)->_v028();
        CfRes_stub_63990();
        func_800AD4B0(self->field_00);
        return;
    }
    if ((f6c & 0x2) != 0) {  // redundant re-test, kept for byte-identity (cr1)
        ((cf::CfResReloadVtIf*)self)->_v028();
        return;
    }
    if (f6c & 0x20) {
        if (((cf::CfResEntryIf2*)entry->field_2C)->_v040(entry) == 0) {
            ok = 0;
        }
    }
    u32 f64 = self->field_00->field_64;
    if (!(f64 & 0x10000)) {
        if (lbl_eu_80663E24 & 0x09800000) {
            return;
        }
    }
    if (ok == 0) {
        return;
    }
    if (f64 & 0x4) {
        ok = 0;
        if (lbl_eu_80663E28 & 0x10) {
            if (func_800B4A24(self->field_00) == 0) {
                ok = 1;
            }
        }
        if (ok == 0 && self->field_04 > lbl_eu_80667698) {
            if (((cf::CfGameManager*)self->field_00)->func_80085840() == 0) {
                return;
            }
            if (self->field_04 > lbl_eu_80667698) {
                self->field_04 -= ((float (*)())func_80069EA0)();
                return;
            }
        }
        self->field_04 = lbl_eu_80667698;
    }
    self->field_08++;
    func_80434A4C__Q23mtl10MemManagerFb(false);
    if (self->field_00->field_6C & 0x20) {
        if (self->field_00->field_98 == 0) {
            u8* slot18 = ((cf::CfResEntryIf2*)entry->field_2C)->_v018(entry);
            self->field_00->field_90 = slot18;
            u8* h = func_80489A60(lbl_eu_80663E14, self->field_00->field_90, -1, 1, 0, 0x76);
            func_800BBADC(self->field_00, h);
        }
    }
    if (self->field_00->field_6C & 0x10) {
        if (self->field_00->field_9C == 0) {
            u8 buf[0x40];
            u32 word48 = 0;
            buf[0] = 0;
            func_800AA33C(buf, entry->field_04, 1, 0);
            u8* slot1C = ((cf::CfResEntryIf2*)entry->field_2C)->_v01C(entry);
            self->field_00->field_94 = slot1C;
            CfRes_getD80Flag();
            self->field_00->field_9C = func_800584B8(self->field_00->field_94, buf);
        }
    }
    if (((cf::CfResEntryIf2*)entry->field_2C)->_v028(entry) != 0) {
        self->field_00->field_6DC = entry;
        u32 v = (u32)entry->field_04 & ~0x1F;
        if (self->field_00->field_64 & 0x8) {
            self->field_00->field_6E0 = v | 0x88000000;
        } else {
            self->field_00->field_6E0 = v | 0x80000000;
        }
    }
    func_80434A4C__Q23mtl10MemManagerFb(true);
    int ok2 = (self->field_00->field_68 >> 20) & 1;
    if (ok2 != 0) {
        func_800BB618(self->field_00, 0);
        ((cf::CfResParentVtIf*)self->field_00)->_v168(lbl_eu_8066769C);
    }
    func_800BCFA0(self->field_00);
    self->field_1E = 0;
    self->field_1C = 0;
    if (!(self->field_00->field_64 & 0x08000000)) {
        self->field_1E = 1;
    }
    if (lbl_eu_80663E24 & 0x40000) {
        func_800BE824(self->field_00, 0);
    } else if (self->field_00->field_C4 != 0) {
        u8* slot30 = ((cf::CfResEntryIf2*)entry->field_2C)->_v030(entry);
        if (slot30 != 0) {
            func_804B0A6C(self->field_00->field_60C, slot30);
        }
    }
    if (ok2 != 0) {
        self->field_00->field_68 |= 0x00100000;
    }
    if (self->field_00->field_64 & 0x8) {
        func_800BE12C(self->field_00, self->field_00->field_6C4, 0, -1, 1);
    }
    self->field_00->field_6C &= ~0x3;
    if (self->field_00->field_98 != 0) {
        ((cf::CfResParentObjIf*)self->field_00->field_98)->_v064(0);
    }
    if (self->field_00->field_64 & 0x4) {
        cf::CfResEneObj* ene = (cf::CfResEneObj*)self->field_00;
        if (self->field_00 != 0) {
            ene = (cf::CfResEneObj*)((u8*)self->field_00 - 0x3E9C);
        }
        if (ene->field_45CA & 0x6) {
            if (self->field_00->field_98 != 0) {
                ((cf::CfResParentObjIf*)self->field_00->field_98)->_v088(0);
            }
            func_800BC3B0((cf::CfObjectMove*)self->field_00, lbl_eu_806676A0);
        }
    }
    ((cf::CfResReloadVtIf*)self)->_v01C();
}

// Periodic reload tick: dispatches the +0x28 secondary-interface slot when
// the parent's +0x6C bit 1 is set, then - when the type field is nonzero -
// validates the enemy object state (via func_800AD860) and the parent flags,
// possibly stopping or advancing the reload. Ends by nudging the enemy flag
// word, restoring the player heal and bumping the type counter.
extern "C" void func_8016DAF8(cf::CfResReloadImpl* self) {
    if (self->field_00->field_6C & 0x2) {
        ((cf::CfResReloadVtIf*)self)->_v028();
    }
    if (self->field_08 == 0) {
        return;
    }
    cf::CfResEneObj* obj = (cf::CfResEneObj*)func_800AD860(self->field_00);
    if (obj != 0 && (obj->field_45CA & 0x2)) {
        if (((cf::CfResParentVtIf*)self->field_00)->_v098() != 0) {
            self->field_1C = 0;
            goto eee4;
        }
        return;
    }
eee4:
    {
        int r5 = (self->field_00->field_64 >> 27) & 1;
        if (self->field_00->field_C4 != 0) {
            u32 f = self->field_00->field_C4->field_0C;
            if ((f & 0x10000) || (f & 0x40)) {
                r5 = 1;
            }
        }
        if (obj != 0 && (self->field_00->field_68 & 0x100000) && r5 == 0) {
            if (!(obj->field_45CA & 0x6)) {
                obj->field_45CA &= 0xFFFE;
                if ((self->field_00->field_C4 == 0 ||
                     !(self->field_00->field_C4->field_0C & 0x2)) &&
                    !(obj->field_3374 & 0x10000)) {
                    obj->field_45CA |= 0x1;
                }
                if (self->field_00->field_C4 != 0 &&
                    (self->field_00->field_C4->field_4EC & 0x2)) {
                    obj->field_45CA &= 0xFFFE;
                }
                if (!(obj->field_45CA & 0x1)) {
                    u8 v = self->field_1E;
                    self->field_1E = (u8)(v - 1);
                    if ((s8)v <= 0) {
                        self->field_1E = 0;
                        obj->field_45CA &= 0xFFFE;
                        goto effc;
                    }
                }
                func_800BC4A0((cf::CfObjectMove*)self->field_00);
                ((cf::CfResParentVtIf*)self->field_00)->_v168(lbl_eu_806676A4);
                return;
            effc:;
            }
        }
        if (obj != 0) {
            obj->field_45CA &= 0xFFF9;
        }
        func_800BC3B0((cf::CfObjectMove*)self->field_00, lbl_eu_806676A8);
        self->field_08++;
    }
}

// Cast-only SI for CfResReloadImpl sub-object virtual calls
struct ResReloadIf {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void* vf3C(void* a);
    virtual void _v040(); virtual int vf44();
    virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
    virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
    virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
    virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
    virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
    virtual void _v098(); virtual void _v09C(); virtual void _v0A0(); virtual void _v0A4();
    virtual void _v0A8(); virtual void _v0AC(); virtual void vfB0();
    virtual void _v0B4(); virtual void _v0B8(); virtual void _v0BC(); virtual void _v0C0();
    virtual void _v0C4(); virtual void _v0C8(); virtual void _v0CC(); virtual void _v0D0();
    virtual void _v0D4(); virtual void _v0D8(); virtual void _v0DC(); virtual float vfE0();
    virtual void _v0E4(); virtual void _v0E8(); virtual void _v0EC(); virtual float vfF0();
    virtual void _v0F4(); virtual void _v0F8(); virtual void _v0FC(); virtual void _v100();
    virtual void _v104(); virtual void _v108(); virtual void _v10C(); virtual void _v110();
    virtual void _v114(); virtual void _v118(); virtual void _v11C(); virtual void _v120();
    virtual void _v124(); virtual void _v128(); virtual void _v12C(); virtual void _v130();
    virtual void _v134(); virtual void _v138(); virtual void _v13C(); virtual void _v140();
    virtual void _v144(); virtual void _v148(); virtual void _v14C(); virtual void _v150();
    virtual void _v154(); virtual void _v158(); virtual void _v15C(); virtual void _v160();
    virtual void _v164(); virtual void _v168(); virtual void _v16C(); virtual void _v170();
    virtual void _v174(); virtual void _v178(); virtual void _v17C(); virtual void* vf180();
    virtual void _v184(); virtual void _v188(); virtual void _v18C(); virtual void _v190();
    virtual void _v194(); virtual void _v198(); virtual void _v19C(); virtual void _v1A0();
    virtual void _v1A4(); virtual void _v1A8(); virtual void _v1AC(); virtual void _v1B0();
    virtual void _v1B4(); virtual void _v1B8(); virtual void _v1BC(); virtual void _v1C0();
    virtual void _v1C4(); virtual void _v1C8(); virtual void _v1CC(); virtual void _v1D0();
    virtual void _v1D4(); virtual void _v1D8(); virtual void _v1DC(); virtual void _v1E0();
    virtual void _v1E4(); virtual void _v1E8(); virtual void _v1EC(); virtual void _v1F0();
    virtual void _v1F4(); virtual void _v1F8(); virtual void _v1FC(); virtual int vf200();
};

extern "C" void func_8016DCE4(u8* self) {
    if (!((ResReloadIf*)*(void**)self)->vf44()) {
        void* r = ((ResReloadIf*)*(void**)self)->vf180();
        ((ResReloadIf*)*(void**)self)->vf3C(r);
    }
    if (*(void**)((u8*)*(void**)self + 0x38))
        ((ResReloadIf*)*(void**)((u8*)*(void**)self + 0x38))->vfB0();
    if (*(void**)((u8*)*(void**)self + 0x98)) {
        float v;
        if (((ResReloadIf*)*(void**)self)->vf200()) {
            void* b = *(void**)self;
            if (b) b = (u8*)b - 0x3e9c;
            v = ((ResReloadIf*)b)->vfF0();
        } else {
            v = ((ResReloadIf*)*(void**)self)->vfE0();
        }
        void* obj = *(void**)((u8*)*(void**)self + 0x98);
        *(float*)((u8*)obj + 0x304) = v;
        *(float*)((u8*)obj + 0x308) = v;
        *(float*)((u8*)obj + 0x30c) = v;
    }
}

// Resets the parent flag bits 0x002773F3, restarts the reload counter, and
// clears the state index unless the counter was already zero.
void func_8016DDE8(cf::CfResReloadImpl* self) {
    func_800BAB64(self->field_00);
    self->field_00->field_6C &= 0xFFD88C0C;  // clears 0x002773F3 flag bits
    self->field_1C = 3;
    self->field_08 = 0;
    func_800AD4B0(self->field_00);
    if (self->field_0C != 0) {
        self->field_0C = 0;
        self->field_0A = -1;
    }
}

// EU-only helper: if the state index is valid, notify the parent's resource
// (+0x70) with that index, then dispatch the +0x28 slot of the secondary
// interface.
void func_eu_8016F1C4(cf::CfResReloadImpl* self) {
    s16 v = self->field_0A;
    if (v >= 0) {
        func_eu_80063174(v, self->field_00->field_70);
    }
    ((cf::CfResReloadVtIf*)self)->_v028();
}

// Early-return unless the parent's +0x6C flag bit 1 (0x2) is set, then
// tail-call through the +0x10 secondary vtable slot +0x28.
void func_8016DE68(cf::CfResReloadImpl* self) {
    if (!(self->field_00->field_6C & 0x2)) {
        return;
    }
    ((cf::CfResReloadVtIf*)self)->_v028();
}

// PMTF dispatch: u16 field_08 selects one of the 4 member pointers in
// lbl_eu_80530FC0; MWCC lowers the call to mulli/lis/addi + `bl __ptmf_scall`.
void func_8016DE8C(cf::CfResReloadImpl* self) {
    u16 idx = self->field_08;
    if (idx < 4) {
        (self->*lbl_eu_80530FC0[idx])();
    }
}

// FULL_MATCH: vtable slot 23 - returns constant resource sub-type
int getSubTypeId() { return 1; }

void setWorkValue(cf::CfResReloadImpl* thisPtr, int index, unsigned int value) {
    if (index < 2) {
        ((unsigned int*)((char*)thisPtr + 0x14))[index] = value;
    }
}

void* __dt__Q22cf15CfResReloadImplFv(void* this_, int deletingFlag) { if (this_ && deletingFlag > 0) ::operator delete(this_); return this_; }

// Reload-state globals owned by this unit (.sbss / .sdata2). Defined here so
// MWCC knows their addresses (enables load/store reordering like retail).
u16 lbl_eu_80664278;
u16 lbl_eu_8066427A;
u16 lbl_eu_8066427C;
u16 lbl_eu_8066427E;
u16 lbl_eu_80664280;
float lbl_eu_80664284;
u16 lbl_eu_80664288;
float lbl_eu_806676C8;

float lbl_eu_806676B0;
float lbl_eu_80666210;
// 2^52 doubles used by the s16/u16 -> f32 conversion magic (MWCC's pool
// reuses these named .sdata2 symbols instead of emitting TU-local @N labels).
double lbl_eu_806676C0 = 0x4330000000000000ll;
double lbl_eu_806676D0 = 0x4330000000000000ll;

u16 getReloadParam0() { return lbl_eu_80664278; }

void updateReloadFloat(short param_1)
{
    lbl_eu_80664278 = param_1;
    lbl_eu_80664284 = lbl_eu_806676B0 * lbl_eu_80666210;
}

// Refreshes the reload timer globals for a given type: stores the type,
// seeds the reload counter (either a fixed 1 with a constant timer, or a
// BDAT-derived value via func_8016E654), then picks the delay float by the
// same flag/selector logic as func_8016E9CC and drives the two sound slots.
extern "C" void func_8016DF4C(u32 type) {
    lbl_eu_80664280 = (u16)type;
    if ((type & 0xFFFF) == 0) {
        lbl_eu_80664278 = 1;
        lbl_eu_80664284 = lbl_eu_806676B0 * lbl_eu_80666210;
    } else {
        u16 v;
        // Cast through the stub so the -ipa inliner cannot fold the call.
        ((u16(*)(u16, u16, u16, u16*, u16*))func_8016E654)(
            lbl_eu_80664280, 0x64, (u16)func_8006A6D0(), &v, &lbl_eu_80664278);
        // u16 -> float via the 2^52 double-magic trick.
        lbl_eu_80664284 = (f32)v * lbl_eu_80666210;
    }
    float delay;
    if (lbl_eu_80663E24 & 0x00400000) {
        goto defdelay;
    }
    if ((lbl_eu_80663E24 & 0x02000000) || (lbl_eu_80663E24 & 0x400)) {
    defdelay:
        delay = lbl_eu_806676B4;
    } else {
        int sel = 0;
        u16 p = lbl_eu_80664280;
        if (p == 1) {
            sel = 0x65;
        } else if (p == 2) {
            sel = 0x66;
        }
        int ok = 0;
        if (sel != 0) {
            if (func_801AAAA0(sel) != 0) {
                ok = 1;
            }
        }
        if (ok != 0) {
            delay = lbl_eu_806676B8;
        } else {
            delay = lbl_eu_806676B4;
        }
    }
    func_8018896C(0, 0, delay, lbl_eu_806676BC);
    func_8018896C(1, 0, delay, lbl_eu_806676BC);
}

u16 getReloadParam2() { return lbl_eu_80664280; }

u16 getReloadParam1() { return lbl_eu_8066427E; }

// Initializes a 0x10-byte struct from global reload params
void initReloadInfoStruct(void* self) {
    unsigned char* p = static_cast<unsigned char*>(self);
    memset(self, 0, 0x10);
    *reinterpret_cast<float*>(p + 0x0) = lbl_eu_80664284;
    *reinterpret_cast<unsigned short*>(p + 0x4) = lbl_eu_80664278;
    *reinterpret_cast<unsigned short*>(p + 0x6) = lbl_eu_8066427E;
    *reinterpret_cast<unsigned short*>(p + 0xC) = lbl_eu_80664280;
    *reinterpret_cast<unsigned short*>(p + 0x8) = lbl_eu_8066427A;
    *reinterpret_cast<unsigned short*>(p + 0xA) = lbl_eu_8066427C;
}

// Reads a 0x10-byte reload-info struct back into the reload-state globals
// (inverse of initReloadInfoStruct) and clears lbl_eu_80664288. Loads are
// assigned in retail order; declarations are reversed to steer MWCC's
// register colors toward retail's descending r7..r3.
void func_8016E100(cf::ReloadInfo* p) {
    f32 v0;
    u16 v5, v4, v3, v2, v1;
    v0 = p->field_00;
    v1 = p->field_04;
    v2 = p->field_06;
    v3 = p->field_0C;
    v4 = p->field_08;
    lbl_eu_80664284 = v0;
    lbl_eu_80664278 = v1;
    lbl_eu_8066427E = v2;
    lbl_eu_80664280 = v3;
    lbl_eu_8066427A = v4;
    v5 = p->field_0A;
    lbl_eu_8066427C = v5;
    lbl_eu_80664288 = 0;
}

// Clears the reload-state globals; the float is re-seeded from lbl_eu_806676C8.
void func_8016E13C() {
    f32 f = lbl_eu_806676C8;
    lbl_eu_80664278 = 0;
    lbl_eu_8066427A = 0;
    lbl_eu_8066427C = 0;
    lbl_eu_8066427E = 0;
    lbl_eu_80664284 = f;
    lbl_eu_80664280 = 0;
    lbl_eu_80664288 = 0;
}

void updateReloadTypeState(u16 r3, u16 r4) {
    extern u16 lbl_eu_8066427A;
    extern u16 lbl_eu_8066427C;
    extern u16 lbl_eu_80664288;
    extern float lbl_eu_806676C8;
    if (lbl_eu_8066427A == r3 && lbl_eu_8066427C == r4) {
        lbl_eu_80664288 &= 0xFFFE;
    } else {
        lbl_eu_80664288 |= 0x1;
    }
    lbl_eu_80664284 = lbl_eu_806676C8;
    lbl_eu_8066427A = r3;
    lbl_eu_8066427C = r4;
}

// Reload-distance check: reads three s16-derived floats plus a u16-derived
// radius from the BDAT table (two column sets selected by arg2), returns 1
// when the radius is non-positive or the horizontal distance between the
// self position and the point falls inside the radius. The y-difference is
// checked separately after the horizontal radius test.
extern "C" int func_8016E1AC(cf::CfResReloadImpl* self, u32 arg2, int arg3) {
    void* bdat = lbl_eu_806640A8;
    u16 row = (u16)lbl_eu_80664184;
    f32 vals[3];
    f32 radius;
    if (arg2 == 2) {
        vals[0] = (f32)(s16)(u16)getBdatStringColumnValue(bdat, lbl_eu_80503140 + 0x5, row);
        vals[1] = (f32)(s16)(u16)getBdatStringColumnValue(bdat, lbl_eu_80503140 + 0xf, row);
        vals[2] = (f32)(s16)(u16)getBdatStringColumnValue(bdat, lbl_eu_80503140 + 0x19, row);
        radius = (f32)(u16)getBdatStringColumnValue(bdat, lbl_eu_80503140 + 0x23, row);
    } else {
        vals[0] = (f32)(s16)(u16)getBdatStringColumnValue(bdat, lbl_eu_80503140 + 0x2d, row);
        vals[1] = (f32)(s16)(u16)getBdatStringColumnValue(bdat, lbl_eu_80503140 + 0x37, row);
        vals[2] = (f32)(s16)(u16)getBdatStringColumnValue(bdat, lbl_eu_80503140 + 0x41, row);
        radius = (f32)(u16)getBdatStringColumnValue(bdat, lbl_eu_80503140 + 0x4b, row);
    }
    if (radius <= lbl_eu_8066A208) {
        return 1;
    }
    if (arg3 != 0) {
        radius -= lbl_eu_806676CC;
    }
    f32* pos = (f32*)self;
    f32 d0 = pos[0] - vals[0];
    f32 d1 = pos[1] - vals[1];
    f32 d2 = pos[2] - vals[2];
    if (d0 * d0 + d2 * d2 >= radius * radius) {
        return 0;
    }
    f32 ad = d1;
    if (ad < 0.0f) {
        ad = -ad;
    }
    if (ad >= radius) {
        return 0;
    }
    return 1;
}

// Checks the reload progression against the BDAT table: reads the column
// values for the type, compares them against the game-manager counter, and
// reports whether the current flag byte matches the type's expected bit
// mask (4 for type 1, 0x10 otherwise). Returns 2 on out-of-range.
extern "C" int func_8016E430(u32 type) {
    void* bdat = lbl_eu_806640A8;
    u16 row = (u16)lbl_eu_80664184;
    u16 gm = (u16)cf::CfGameManager::func_800822F4();
    u16 a;
    u16 b;
    if (type == 1) {
        a = (u16)getBdatStringColumnValue(bdat, lbl_eu_80503140 + 0x55, row);
        b = (u16)getBdatStringColumnValue(bdat, lbl_eu_80503140 + 0x61, row);
    } else if (type == 2) {
        a = (u16)getBdatStringColumnValue(bdat, lbl_eu_80503140 + 0x6d, row);
        b = (u16)getBdatStringColumnValue(bdat, lbl_eu_80503140 + 0x79, row);
    } else {
        a = 0;
        b = 0;
    }
    if (gm < a) {
        return 2;
    }
    if (a == 0 && b == 0) {
        return 2;
    }
    if (a > gm) {
        return 2;
    }
    if (gm > b) {
        return 2;
    }
    u8 v = (u8)getBdatStringColumnValue(bdat, lbl_eu_80503140 + 0x85, row);
    u32 mask = (type == 1) ? 0x4 : 0x10;
    return (v & mask) == 0 ? 1 : 0;
}

// Reads a BDAT column (table lbl_eu_806640A8, row (u16)lbl_eu_80664184) and
// returns whether the low byte's bit pattern matches the given sub-type:
// bit 0 -> 1, bit pair 1-2 -> sub 1, 3-4 -> sub 2, 5-6 -> sub 3, 7-8 -> sub 4.
// The column name depends on the type field (2 picks a different column).
extern "C" int func_8016E578(u32 type, int sub) {
    char* col = lbl_eu_80503140 + 0x94;
    u16 row = (u16)lbl_eu_80664184;
    if (type == 2) {
        col = lbl_eu_80503140 + 0x8c;
    }
    u32 res = getBdatStringColumnValue(lbl_eu_806640A8, col, row);
    u16 v = (u16)res;
    if (v & 1) {
        return 1;
    }
    if ((v & 0x6) && sub == 1) {
        return 1;
    }
    if ((v & 0x18) && sub == 2) {
        return 1;
    }
    if ((v & 0x60) && sub == 3) {
        return 1;
    }
    if ((v & 0x180) && sub == 4) {
        return 1;
    }
    return 0;
}

// Computes reload parameters for a resource type: reads four BDAT columns
// (the column offset depends on whether the type is 2), returns the byte
// value of the first column (optionally boosted by 5/10 when the type's
// bit-pair matches arg3) and writes the second-column*10 product (or a magic
// constant / random-window value) to out1. out2 receives the fourth column.
// Retail cmplwi compares mean type/arg2 are unsigned.
extern "C" int func_8016E654(u32 type, u32 arg2, int arg3, u16* out1, u16* out2) {
    void* bdat = lbl_eu_806640A8;
    u16 row = (u16)lbl_eu_80664184;
    // Byte locals force the full-word store of the call result; the int
    // copies materialize the byte (lbz) and keep the if-chain adds unmasked
    // (retail: stw r3,slot + lbz rX,slot instead of an inline rlwinm).
    u8 v1 = (u8)getBdatStringColumnValue(bdat, type == 2 ? lbl_eu_80503140 + 0x9c : lbl_eu_80503140 + 0xa8, row);
    int r1 = v1;
    u8 v2 = (u8)getBdatStringColumnValue(bdat, type == 2 ? lbl_eu_80503140 + 0xb4 : lbl_eu_80503140 + 0xc0, row);
    int r2 = v2;
    u16 v3 = (u16)getBdatStringColumnValue(bdat, type == 2 ? lbl_eu_80503140 + 0x8c : lbl_eu_80503140 + 0x94, row);
    u8 v4 = (u8)getBdatStringColumnValue(bdat, type == 2 ? lbl_eu_80503140 + 0xcc : lbl_eu_80503140 + 0xd4, row);

    *out2 = v4;
    int m = r2 * 10;
    u16 m16 = (u16)m;
    *out1 = m16;

    if ((v3 & 0x600) && arg2 < 3) {
        *out1 = (v3 & 0x400) ? 0x2d0 : 0x168;
        return 0x64;
    }
    if ((v3 & 0x6) && arg3 == 1) {
        r1 += (v3 & 0x4) ? 0xa : 0x5;
    } else if ((v3 & 0x18) && arg3 == 2) {
        r1 += (v3 & 0x10) ? 0xa : 0x5;
    } else if ((v3 & 0x60) && arg3 == 3) {
        r1 += (v3 & 0x40) ? 0xa : 0x5;
    } else if ((v3 & 0x180) && arg3 == 4) {
        r1 += (v3 & 0x80) ? 0xa : 0x5;
    }
    *out1 = (u16)(m16 - ml::math::mtRand(m16 / 6));
    return r1;
}

// Searches up to *counter reload candidates for one whose delay window fits
// the current state: picks a random offset (mtRand) and queries the reload
// helper, then validates the candidate against the type check (func_8016E430)
// and the 64288-flag state. On success writes the reload info to out1..out3
// and returns the candidate delay; on exhaustion resets to 0xB4 and returns 1.
extern "C" u16 func_8016E854(cf::CfResReloadImpl* self, u16* out1, u16* counter, u16* out2,
                  u16* out3) {
    u16 f0;
    u16 initial = *counter;
    if (lbl_eu_806640A8 != 0) {
        u16 rnd = (u16)func_8006A6D0();
        f0 = lbl_eu_80664288 & 1;
        // Retail reloads the flag word for the mask store even though f0 read
        // it moments earlier; the volatile load keeps MWCC from CSE-eliminating
        // the second read.
        u16 masked = *(volatile u16*)&lbl_eu_80664288 & 0xFFFE;
        *counter = 2;
        lbl_eu_80664288 = masked;
        u16 mrand = (u16)ml::math::mtRand(0x64);
        while (*counter >= 1) {
            int typeRet;
            u16 mrand2;
            u16 spA;
            u16 spB;
            if (((int(*)(cf::CfResReloadImpl*, u16, int))func_8016E1AC)(
                    self, *counter, 1) == 0) {
                goto dec;
            }
            typeRet = func_8016E430(*counter);
            if (typeRet == 0) {
                goto dec;
            }
            mrand2 = (u16)ml::math::mtRand(0x64);
            u16 ret;
            ret =
                ((u16(*)(u16, u16, u16, u16*, u16*))func_8016E654)(
                    *counter, mrand, rnd, &spA, &spB);
            u16 spBv = (u16)spB;
            if (spBv == 0) {
                goto dec;
            }
            if (ret == 0x64) {
                mrand2 = mrand;
            }
            u16 sel = ret;
            if (typeRet == 1) {
                sel = 0x64;
            } else if (f0 != 0) {
                if (spBv == lbl_eu_80664278) {
                    sel = 0x64;
                }
            } else if (*counter == initial) {
                goto dec;
            }
            if (mrand2 >= sel) {
                goto dec;
            }
            *out1 = (u16)spA;
            *out2 = mrand2;
            *out3 = sel;
            return (u16)spB;
        dec:
            (*counter)--;
        }
    }
    *counter = 0;
    *out2 = 0;
    *out3 = 0;
    *out1 = 0xB4;
    return 1;
}

// Returns a delay/timer float: two flag checks against lbl_eu_80663E24
// short-circuit to the default; otherwise the value depends on
// lbl_eu_80664280 (1 -> 0x65, 2 -> 0x66) passing through func_801AAAA0.
float func_8016E9CC() {
    if (lbl_eu_80663E24 & 0x00400000) {
        return lbl_eu_806676B4;
    }
    if ((lbl_eu_80663E24 & 0x02000000) || (lbl_eu_80663E24 & 0x400)) {
        return lbl_eu_806676B4;
    }
    int sel = 0;
    u16 p = lbl_eu_80664280;
    if (p == 1) {
        sel = 0x65;
    } else if (p == 2) {
        sel = 0x66;
    }
    int ok = 0;
    if (sel != 0) {
        if (func_801AAAA0(sel) != 0) {
            ok = 1;
        }
    }
    if (ok != 0) {
        return lbl_eu_806676B8;
    }
    return lbl_eu_806676B4;
}

// Main reload driver: when the type check (func_8016E430) reports 1, refresh
// the reload-count global from the BDAT column for the current type; otherwise
// tick the timer float and, once it expires, run the candidate search
// (func_8016E854), convert the result to the timer and drive the sound slots.
extern "C" void func_8016EA68(cf::CfResReloadImpl* self) {
    int typeRet = func_8016E430(lbl_eu_80664280);
    if (typeRet == 1) {
        u16 p = lbl_eu_80664280;
        u16 row = (u16)lbl_eu_80664184;
        void* bdat = lbl_eu_806640A8;
        char* col = lbl_eu_80503140 + 0xd4;
        if (p == 2) {
            col = lbl_eu_80503140 + 0xcc;
        }
        u32 res = getBdatStringColumnValue(bdat, col, row);
        lbl_eu_80664278 = (u8)res;
        return;
    }
    f32 t = lbl_eu_80664284 - func_80069EE4();
    lbl_eu_80664284 = t;
    if (t > lbl_eu_806676C8) {
        u16 p = lbl_eu_80664280;
        if (p == 0) {
            return;
        }
        if ((u16)(p - 1) <= 1 &&
            ((int(*)(cf::CfResReloadImpl*, u16, int))func_8016E1AC)(self, p, 0) ==
                0) {
            lbl_eu_80664284 = lbl_eu_806676C8;
            return;
        }
        u16 rnd = (u16)func_8006A6D0();
        u16 r = (u16)func_8016E578(lbl_eu_80664280, rnd);
        if (r == 0 || typeRet == 0) {
            u16 o1;
            u16 o2;
            u16 o3;
            u16 ret = func_8016E854(self, &o1, &lbl_eu_80664280, &o2, &o3);
            lbl_eu_80664278 = ret;
            // u16 -> float via the 2^52 double-magic trick.
            lbl_eu_80664284 = (f32)o1 * lbl_eu_80666210;
            float delay;
            if ((lbl_eu_80663E24 & 0x00400000) || (lbl_eu_80663E24 & 0x02000000) ||
                (lbl_eu_80663E24 & 0x400)) {
                delay = lbl_eu_806676B4;
            } else {
                int sel = 0;
                u16 p2 = lbl_eu_80664280;
                if (p2 == 1) {
                    sel = 0x65;
                } else if (p2 == 2) {
                    sel = 0x66;
                }
                int ok = 0;
                if (sel != 0) {
                    if (func_801AAAA0(sel) != 0) {
                        ok = 1;
                    }
                }
                if (ok != 0) {
                    delay = lbl_eu_806676B8;
                } else {
                    delay = lbl_eu_806676B4;
                }
            }
            func_8018896C(0, 0, delay, lbl_eu_806676BC);
            func_8018896C(1, 0, delay, lbl_eu_806676BC);
        }
    }
}

// Refreshes the reload param 1 global (lbl_eu_8066427E) from the BDAT table
// when a table is loaded: the column is selected by lbl_eu_80664280
// (0 -> table lbl_eu_80531068 indexed by func_8006A6D0, 1 -> +0xDC string,
// 2 -> +0xE4 string). Then forwards the object to func_8016EA68.
void func_8016EC58(cf::CfResReloadImpl* self) {
    void* bdat = lbl_eu_806640A8;
    u16 row = (u16)lbl_eu_80664184;
    if (bdat != 0) {
        u16 p = lbl_eu_80664280;
        switch (p) {
        case 0:
            lbl_eu_8066427E =
                (u8)getBdatStringColumnValue(bdat, lbl_eu_80531068[func_8006A6D0()], row);
            break;
        case 1:
            lbl_eu_8066427E =
                (u8)getBdatStringColumnValue(bdat, lbl_eu_80503140 + 0xdc, row);
            break;
        case 2:
            lbl_eu_8066427E =
                (u8)getBdatStringColumnValue(bdat, lbl_eu_80503140 + 0xe4, row);
            break;
        }
    }
    func_8016EA68(self);
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_8016ED44() {
    extern unsigned short lbl_eu_80664288;
    lbl_eu_80664288 = 0;
}
