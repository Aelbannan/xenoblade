// Auto-scaffolded catalog TU for kyoshin/cf/CfResPcImpl
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CfResPcImpl.hpp"
// NOTE: CfBdat.hpp is intentionally not included here - its `s32` (long)
// getBdatStringColumnValue declaration clashes with code_801862C0.hpp's
// `int` spelling (pulled in via harness_catalog.hpp). The two static members
// this TU needs are declared in CfResPcImpl.hpp instead.
#include <nw4r/math/math_types.h>
#include <string.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// Resolve ml::FixStr<128>::format calls to the explicit specialization that
// CfScript.cpp defines (retail symbol format__Q22ml10FixStr<128>FPCce).  The
// generic-template call mangles to ...11FixStr... otherwise (reloc-name
// drift; same convention as pluginSnd.cpp).
template <> void ml::FixStr<128>::format(const char* fmt, ...);

namespace cf {}
extern "C" void func_800BE12C(void* obj, int a, int b, int c, int d);
using namespace cf;

// Retail ctor: writes the parent ref last (after the constant fields),
// zeroes the 8-slot table via memset and seeds the three state halfwords at
// +0x38/+0x3A/+0x3C to -1. The parent's +0x8E halfword is cleared when a
// parent is present.
cf::CfResPcImpl* __ct__cf_CfResPcImpl(cf::CfResPcImpl* self, cf::CfResPcParent* parent) {
    self->field_04 = lbl_eu_80667A40;
    self->field_08 = 0;
    self->field_0A = -1;
    self->field_0C = 0;
    self->field_0E = -1;
    self->field_10 = &lbl_eu_80532774;
    self->field_34 = 0;
    self->field_3E = 3;
    self->field_40 = 0;
    self->field_00 = parent;
    if (parent != 0) {
        parent->field_8E = 0;
    }
    memset(&self->field_14, 0, sizeof(self->field_14));
    self->field_38 = -1;
    self->field_3A = -1;
    self->field_3C = -1;
    return self;
}

// Retail dtor: re-installs the secondary vtable at +0x10, and when the
// state at field_0E is valid and the parent's +0x68 flag bit 21 is set and
// its vtable slot +0x74 reports the resource live, notifies the resource
// (func_801BFA64(state + 2)). The delete-flag path (__dl__FPv) is emitted
// by MWCC for a non-trivial dtor automatically.
cf::CfResPcImpl::~CfResPcImpl() {
    int ok;
    s16 state = field_0E;
    field_10 = &lbl_eu_80532774;
    if (state >= 0) {
        ok = 0;
        if (field_00->field_68 & 0x200000) {
            if (((cf::CfResPcParentVtIf*)field_00)->_v074() != 0) {
                ok = 1;
            }
        }
        if (ok != 0) {
            func_801BFA64(state + 2);
        }
    }
}

int func_8018CB14(void* p)
{
    return (unsigned int)*(unsigned short*)((char*)p + 8) >= 3;
}

int func_8018CB34() { return 2; }

// func_8018CB3C - PC resource teardown: dispatches the parent's vtable slots
// +0x1CC/+0x17C/+0x178 around func_800BBB50, zeroes the parent's
// +0x90/+0x94/+0x704 words, runs func_800BE1A4, then invalidates the three
// state halfwords (+0x38/+0x3A/+0x3C) and clears the +0x34 slot.
void func_8018CB3C(cf::CfResPcImpl* self) {
    ((cf::CfResPcParentVtIf*)self->field_00)->_v1CC();
    ((cf::CfResPcParentVtIf*)self->field_00)->_v17C();
    func_800BBB50(self->field_00);
    ((cf::CfResPcParentVtIf*)self->field_00)->_v178();
    self->field_00->field_90 = 0;
    self->field_00->field_94 = 0;
    self->field_00->field_704 = 0;
    func_800BE1A4(self->field_00);
    self->field_38 = -1;
    self->field_3A = -1;
    self->field_3C = -1;
    self->field_34 = 0;
}

// EU-only PC-resource helper: when the parent flag bit 31 (0x80000000) is
// set, queries the +0x10 table slot +0x34 for a resource object; when the
// instance field is live and the local state at field_38 is positive,
// notifies the resource (func_eu_80063174) and dispatches table slot +0x28.
void func_eu_8018E19C(cf::CfResPcImpl* self) {
    if (self->field_00->field_64 & 0x80000000) {
        int res = ((cf::CfResPcVtIf*)self)->_v034(0);
        if (res != 0) {
            u8* inst = CfRes_getInstanceField();
            s16 state = self->field_38;
            if (inst != 0 && state > 0) {
                func_eu_80063174(state, (u8*)res);
                ((cf::CfResPcVtIf*)self)->_v028();
            }
        }
    }
}

// Getter for the 8-slot table at +0x14; out-of-range indexes return 0.
u32 func_8018D134(cf::CfResPcImpl* self, int idx)
{
    if (idx < 8) {
        return self->field_14[idx];
    }
    return 0;
}

// func_8018CBE8 - PC state load: dispatches the +0x28 slot, queries the
// slot-table buffer via func_80080F48 and installs the queried ids into the
// secondary-interface slots (+0x30) and the parent +0x70C..+0x712 words.
// The +0x30 slot index order (0,1,2,4,3,5,6) matches retail.
void func_8018CBE8(cf::CfResPcImpl* self, u16 arg2) {
    cf::CfResPcLoadBuffer buf;
    ((cf::CfResPcVtIf*)self)->_v028();
    func_80080F48__Q22cf13CfGameManagerFv(self->field_00->field_8C, &buf, 0, 1);
    self->field_40 = arg2;
    if (arg2 == 0) {
        ((cf::CfResPcVtIf*)self)->_v030(0, buf.field_00[0]);
        ((cf::CfResPcVtIf*)self)->_v030(1, buf.field_00[1]);
        ((cf::CfResPcVtIf*)self)->_v030(2, buf.field_00[2]);
        ((cf::CfResPcVtIf*)self)->_v030(4, buf.field_00[3]);
        ((cf::CfResPcVtIf*)self)->_v030(3, buf.field_00[4]);
        ((cf::CfResPcVtIf*)self)->_v030(5, buf.field_00[5]);
        ((cf::CfResPcVtIf*)self)->_v030(6, buf.field_00[6]);
    }
    if (buf.field_1E != 0) {
        ((cf::CfResPcVtIf*)self)->_v030(7, buf.field_00[6]);
    }
    self->field_00->field_70C[0] = buf.field_1C;
    self->field_00->field_70C[1] = buf.field_1E;
    self->field_00->field_70C[2] = buf.field_20;
    self->field_00->field_70C[3] = buf.field_22;
    self->field_00->field_6C |= 0x1;
    self->field_08 = 0;
    self->field_3E = 3;
    self->field_00->field_6C |= 0x1;
}

// func_8018CD9C - PC sound-notify: when the state at field_0E is valid and
// the parent's +0x68 flag bit 21 is set and its vtable slot +0x74 reports
// the resource live, sends (state+2, arg2, parent->field_74, f1, f2) to
// func_801BFE20 and, when the resulting sound slot holds a live sound object
// and arg4 is nonzero, sets its player priority to arg4. arg3 (r5) is an
// unused register-slot parameter (retail never reads it).
void func_8018CD9C(cf::CfResPcImpl* self, int arg2, int arg3, int arg4, f32 f1, f32 f2) {
    s16 state = self->field_0E;
    if (state < 0) {
        return;
    }
    cf::CfResPcParent* parent = self->field_00;
    int ok = 0;
    if (parent->field_68 & 0x200000) {
        if (((cf::CfResPcParentVtIf*)parent)->_v074() != 0) {
            ok = 1;
        }
    }
    if (ok != 0) {
        cf::CfResPcSoundSlotEntry* slot = func_801BFAE4((u16)func_801BFE20(state + 2, arg2, self->field_00->field_74, f1, f2));
        if (slot != 0 && arg4 != 0 && slot->field_00 != 0) {
            slot->field_00->SetPlayerPriority(arg4);
        }
    }
}

// func_8018CE70 - same dispatch shape as func_8018CF08 but forwards state+2
// with only two caller args to func_801BFE8C.
void func_8018CE70(cf::CfResPcImpl* self, int arg2, int arg3) {
    s16 state = self->field_0E;
    if (state < 0) {
        return;
    }
    cf::CfResPcParent* parent = self->field_00;
    int ok = 0;
    if (parent->field_68 & 0x200000) {
        if (((cf::CfResPcParentVtIf*)parent)->_v074() != 0) {
            ok = 1;
        }
    }
    if (ok != 0) {
        func_801BFE8C(state + 2, arg2, arg3);
    }
}

// func_8018CF08 - dispatch a reload-type message when the state at field_0E
// is valid and the parent's +0x68 flag bit 21 (0x200000) is set and its
// vtable slot +0x74 reports the resource live; forwards state+2 with the
// three caller args to func_801BFF04.
void func_8018CF08(cf::CfResPcImpl* self, int arg2, int arg3, int arg4) {
    // ok declared first so MWCC assigns it r31 (retail keeps state in r30);
    // the zero-init stays after the parent load to match retail ordering.
    int ok;
    s16 state = self->field_0E;
    if (state < 0) {
        return;
    }
    cf::CfResPcParent* parent = self->field_00;
    ok = 0;
    if (parent->field_68 & 0x200000) {
        if (((cf::CfResPcParentVtIf*)parent)->_v074() != 0) {
            ok = 1;
        }
    }
    if (ok != 0) {
        func_801BFF04(state + 2, arg2, arg3, arg4);
    }
}

// func_8018CF90 - returns state+2 when the state at field_0E is valid and
// the parent's +0x68 flag bit 21 is set and its vtable slot +0x74 reports
// the resource live; otherwise -1.
int func_8018CF90(cf::CfResPcImpl* self) {
    // Same regalloc shape as func_8018CF08 (ok in r31, state in r30); the
    // nested-success / single-fail-exit layout makes MWCC share one
    // `li r3,-1` tail like retail.
    int ok;
    s16 state = self->field_0E;
    if (state >= 0) {
        cf::CfResPcParent* parent = self->field_00;
        ok = 0;
        if (parent->field_68 & 0x200000) {
            if (((cf::CfResPcParentVtIf*)parent)->_v074() != 0) {
                ok = 1;
            }
        }
        if (ok != 0) {
            return state + 2;
        }
    }
    return -1;
}

// func_8018D00C - PC lookup probe: when parent flag bit 1 is set and the
// state at field_0A is valid, packs the parent's +0x8C slot id into a token
// (0x3800AC0A when the id is 4 or 0xC/0xD and the bdat text id's bits 5..11
// equal 0x2B, else 0x3800000A) and queries the table via func_80062928;
// returns the packed token (0 when gated out). Retail reuses one register
// for result/idx/packed, so the source reuses one int variable.
int func_8018D00C(cf::CfResPcImpl* self) {
    int result = 0;
    cf::CfResPcParent* parent = self->field_00;
    if (parent->field_64 & 0x2) {
        s16 state = self->field_0A;
        result = parent->field_8C;
        if (state < 0) {
            return 0;
        }
        u32 token = (cf::CfBdat::func_801422A8(func_8009EC9C(result)->field_18) >> 20) & 0x7F;
        if ((result == 4 || (u32)(result - 0xC) <= 1) && token == 0x2B) {
            result = (result << 20) | 0x3800AC0A;
        } else {
            result = (result << 20) | 0x3800000A;
        }
        func_80062928(state, result, 3);
    }
    return result;
}

// func_8018D0C4 - PC resource teardown: detaches the active lookup entry's
// resource object (func_80065CA4), cleans the entry (parent flag bit 1), then
// clears the entry/reload slots at +0x6DC/+0x6E0. All parent accesses go
// through self->field_00 directly (retail reloads it at every site).
void func_8018D0C4(cf::CfResPcImpl* self) {
    cf::CfResPcLookupEntry* entry = self->field_00->field_6DC;
    if (entry != 0) {
        func_80065CA4(entry->field_2C, entry);
        if (self->field_00->field_64 & 0x2) {
            func_80066714(self->field_00->field_6DC, true);
        }
    }
    self->field_00->field_6DC = 0;
    self->field_00->field_6E0 = 0;
}

// func_8018D154 - PC resource request: when arg2 != 0 and the state at
// field_0A is valid, queries the resource table with (state, arg2, 3); on
// success dispatches the secondary vtable slot +0x30 with (arg3, arg2) and
// sets parent flag bits 0x20/0x40/0x80/0x100/0x200 for arg3 == 1..5, then
// resets the handler index field_08 to 1. Returns whether the query
// succeeded (retail neg/or/srwi normalize reused for branch+return).
extern "C" __declspec(noinline) int func_8018D154(cf::CfResPcImpl* self, u32 arg2, u32 arg3) {
    if (arg2 == 0) {
        return 0;
    }
    s16 state = self->field_0A;
    if (state < 0) {
        return 0;
    }
    int ret = func_80062928(state, arg2, 3);
    int ok = (u32)(-ret | ret) >> 31;
    if (ok) {
        ((cf::CfResPcVtIf*)self)->_v030(arg3, arg2);
        if ((int)arg3 == 1) {
            self->field_00->field_6C |= 0x20;
        } else if ((int)arg3 == 2) {
            self->field_00->field_6C |= 0x40;
        } else if ((int)arg3 == 3) {
            self->field_00->field_6C |= 0x80;
        } else if ((int)arg3 == 4) {
            self->field_00->field_6C |= 0x100;
        } else if ((int)arg3 == 5) {
            self->field_00->field_6C |= 0x200;
        }
        self->field_08 = 1;
    }
    return ok;
}

// retail: clrlwi r5, r4, 22; b func_8018D154 (r5 = arg2 & 0x3FF)
extern "C" void func_8018D288(void* self, u32 a, u32 b){
    func_8018D154((cf::CfResPcImpl*)self, a, a & 0x3FF);
}

// func_8018D290 - PC lookup probe: when the state at field_0A is valid,
// queries the resource table with (field_38, arg2, 5) or (state, arg2, 3)
// depending on parent flag bit 0; on success dispatches the secondary vtable
// slot +0x30 with (0, arg2), sets parent flag bit 4 (0x10) and selects
// handler index 1. Returns whether the query succeeded (retail
// neg/or/srwi normalize reused for branch+return).
int func_8018D290(cf::CfResPcImpl* self, int arg2) {
    s16 v = self->field_0A;
    if (v < 0) {
        return 0;
    }
    int ret;
    if (self->field_00->field_64 & 0x80000000) {
        ret = func_80062998(self->field_38, arg2, 5);
    } else {
        ret = func_80062928(v, arg2, 3);
    }
    int ok = (u32)(-ret | ret) >> 31;
    if (ok) {
        ((cf::CfResPcVtIf*)self)->_v030(0, arg2);
        self->field_00->field_6C |= 0x10;
        self->field_08 = 1;
    }
    return ok;
}

// func_8018D354 - PC lookup probe: when the state at field_0A is valid,
// queries the resource table with (field_3C, arg2, 5) or (state, arg2, 3)
// depending on parent flag bit 0; on success sets parent flag bit 17
// (0x20000) and caps the handler index field_08 at 2. Returns whether the
// query succeeded (retail neg/or/srwi normalize reused for branch+return).
int func_8018D354(cf::CfResPcImpl* self, int arg2) {
    s16 v = self->field_0A;
    if (v < 0) {
        return 0;
    }
    int ret;
    if (self->field_00->field_64 & 0x80000000) {
        ret = func_80062998(self->field_3C, arg2, 5);
    } else {
        ret = func_80062928(v, arg2, 3);
    }
    int ok = (u32)(-ret | ret) >> 31;
    if (ok) {
        self->field_00->field_6C |= 0x20000;
        if ((u32)self->field_08 > 1) {
            self->field_08 = 2;
        }
    }
    return ok;
}

// func_8018D3F0 - PC resource request: when arg3 <= 1 and the state at
// field_0A is valid, notifies the parent vtable slot +0x1D0 with arg3,
// resolves arg2 through bdat, queries the resource table with
// (state, resolved, 3) and dispatches the secondary vtable slot +0x30 with
// (6, arg2); on success sets parent flag bit 0x2000 (arg3 == 1) or 0x1000
// and caps the handler index at 2; when the query failed, success is
// reported only when arg2 == 0.
int func_8018D3F0(cf::CfResPcImpl* self, u32 arg2, u32 arg3) {
    if (arg3 > 1) {
        return 0;
    }
    s16 state = self->field_0A;
    if (state < 0) {
        return 0;
    }
    ((cf::CfResPcParentVtIf*)self->field_00)->_v1D0(arg3);
    int ret = func_80062928(state, cf::CfBdat::func_801422A8(arg2), 3);
    int ok = (u32)(-ret | ret) >> 31;
    ((cf::CfResPcVtIf*)self)->_v030(6, arg2);
    if (ok) {
        if ((int)arg3 == 1) {
            self->field_00->field_6C |= 0x2000;
        } else {
            self->field_00->field_6C |= 0x1000;
        }
        if (self->field_08 > 1) {
            self->field_08 = 2;
        }
    } else if (arg2 == 0) {
        ok = 1;
    }
    return ok;
}

// Setter for the 8-slot table at +0x14. Indexes 6/7 run the value through
// the bdat token resolver first (retail: subi/cmplwi range test, then
// func_801422A8__Q22cf6CfBdatFUl).
void func_8018D510(cf::CfResPcImpl* self, int idx, u32 value)
{
    if (idx < 8) {
        if ((u32)(idx - 6) <= 1) {
            value = cf::CfBdat::func_801422A8(value);
        }
        self->field_14[idx] = value;
    }
}

// func_8018D570 - PC per-state cleanup: when parent flag bit 0 is set and
// the instance field is live, walks the three state halfwords at +0x38/
// +0x3A/+0x3C; for each valid state (parent flag bit 16 clear) looks up the
// resource entry by state and, when its +0x04 id matches the secondary
// vtable slot +0x34 result, bumps the entry's +0x38 counter (and +0x3A too
// when parent flag bit 0 of +0x6C is set).
void func_8018D570(cf::CfResPcImpl* self) {
    if (!(self->field_00->field_64 & 0x80000000)) {
        return;
    }
    if (CfRes_getInstanceField() == 0) {
        return;
    }
    for (int i = 0; i < 3; i++) {
        s16 state = (&self->field_38)[i];
        if (state < 0) {
            continue;
        }
        if (self->field_00->field_64 & 0x10000) {
            continue;
        }
        cf::CfResPcEntry38* obj = func_80062EC4(state);
        if (obj == 0) {
            continue;
        }
        if (obj->field_04 != (u32)((cf::CfResPcVtIf*)self)->_v034(0)) {
            continue;
        }
        obj->field_38++;
        if (self->field_00->field_6C & 0x1) {
            obj->field_3A++;
        }
    }
}

// func_8018D65C - PC resource open/close: when arg2 != 0, dispatches the
// parent vtable slots +0x1CC/+0x17C/+0x178 around func_800BBB50, clears the
// parent +0x90/+0x94/+0x704 words and the three state halfwords, folds the
// parent +0x6C flag word (mask 0xFFD88C0C, then set bit 0x4), zeroes the
// handler index, latches field_3E = 3 and, when the state at field_0E is
// valid, notifies func_800638B4. When arg2 == 0, instead dispatches the
// secondary vtable slot +0x38 and forwards (state, result) to
// func_80063994, clears parent flag bit 0x4 and sets bit 0x1, then latches
// field_3E = 3.
void func_8018D65C(cf::CfResPcImpl* self, int arg2) {
    if (arg2 != 0) {
        ((cf::CfResPcParentVtIf*)self->field_00)->_v1CC();
        ((cf::CfResPcParentVtIf*)self->field_00)->_v17C();
        func_800BBB50(self->field_00);
        ((cf::CfResPcParentVtIf*)self->field_00)->_v178();
        self->field_00->field_90 = 0;
        self->field_00->field_94 = 0;
        self->field_00->field_704 = 0;
        self->field_38 = -1;
        self->field_3A = -1;
        self->field_3C = -1;
        self->field_00->field_6C &= 0xFFD88C0C;
        self->field_00->field_6C |= 0x4;
        s16 state = self->field_0E;
        self->field_08 = 0;
        self->field_3E = 3;
        if (state >= 0) {
            func_800638B4(state);
        }
    } else {
        s16 state = self->field_0E;
        if (state >= 0) {
            int result = ((cf::CfResPcVtIf*)self)->_v038();
            func_80063994(state, result);
        }
        self->field_00->field_6C &= ~0x4;
        self->field_00->field_6C |= 0x1;
        self->field_3E = 3;
    }
}

// func_8018D79C - PC reload driver: dispatches the parent teardown slots,
// clears the three state halfwords, then - when the game-manager state flag
// and the parent +0x68 flag bit 20 allow - either decrements the field_3E
// counter or performs a full reload: query the two table slots (+0x34 with 0
// and 6), find/install the resource entries, run the slot-0xB area handling,
// refresh the character-slot ids and finally dispatch the +0x50 slot (or
// install the +0x34 slot result via func_800685C8 / findResEntry).
void func_8018D79C(cf::CfResPcImpl* self) {
    ((cf::CfResPcParentVtIf*)self->field_00)->_v1CC();
    ((cf::CfResPcParentVtIf*)self->field_00)->_v17C();
    func_800BBB50(self->field_00);
    ((cf::CfResPcParentVtIf*)self->field_00)->_v178();
    ((cf::CfResPcParentVtIf*)self->field_00)->_v1D0(0);
    ((cf::CfResPcParentVtIf*)self->field_00)->_v1D0(1);
    self->field_00->field_90 = 0;
    self->field_00->field_94 = 0;
    self->field_38 = -1;
    self->field_3A = -1;
    self->field_3C = -1;
    self->field_34 = 0;
    if (((cf::CfGameManager*)self->field_00)->func_80082900() == 0) {
        return;
    }
    if (!(self->field_00->field_68 & 0x100000)) {
        return;
    }
    if (self->field_0A < 0) {
        return;
    }
    int cond = 1;
    if ((lbl_eu_80663E24 & 0x2000000) == 0 && (lbl_eu_80663E24 & 0x400) == 0 &&
        (lbl_eu_80663E24 & 0x40000) == 0) {
        cond = 0;
    }
    u8* inst = CfRes_getInstanceField();
    u32 f6c = self->field_00->field_6C;
    if (f6c & 0x2) {
        self->field_00->field_6C &= ~0x3;
        return;
    }
    if (!(f6c & 0x1)) {
        return;
    }
    if (self->field_3E > 0) {
        self->field_3E--;
        return;
    }
    int v034_0 = ((cf::CfResPcVtIf*)self)->_v034(0);
    int v034_6 = ((cf::CfResPcVtIf*)self)->_v034(6);
    int r29 = 1;
    int r25 = 0;
    if (self->field_00->field_64 & 0x80000000) {
        if (inst != 0) {
            int ret;
            if (cond != 0) {
                ret = func_80062B3C((u8*)v034_0, 4);
            } else {
                ret = func_80062BAC(v034_0);
            }
            r29 = 0;
            if (ret != 0) {
                u32 sp18;
                u32 sp14;
                cf::CfResPcFindEntry* found = findResEntry(inst, (u32)v034_0, &sp18, &sp14);
                if (found != 0) {
                    self->field_38 = (u16)sp14;
                    r25 = 1;
                    found->field_0C = 0;
                }
                if (v034_6 != 0 &&
                    ((self->field_00->field_6C & 0x80000) || (self->field_00->field_6C & 0x100000))) {
                    r25 = 1;
                    r29 = 1;
                }
            }
        }
    }
    if (r25 == 0) {
        return;
    }
    if (self->field_00->field_64 & 0x2) {
        int v6 = ((cf::CfResPcVtIf*)self)->_v034(6);
        if (v6 != 0) {
            int v6b = ((cf::CfResPcVtIf*)self)->_v034(6);
            if (((v6b >> 5) & 0x7F) != 0) {
                u16 slot = self->field_00->field_8C;
                if (slot == 0xB) {
                    if (self->field_0A >= 0 && !(self->field_00->field_64 & 0x80000000)) {
                        if (func_80062928(self->field_0A, 0x7E30400, 3) != 0) {
                            self->field_00->field_6C |= 0x40000;
                        }
                    }
                    if (self->field_0A >= 0) {
                        if (func_80062928(self->field_0A, 0xA630400, 3) != 0) {
                            self->field_00->field_6C |= 0x200000;
                            self->field_08 = 1;
                        }
                    }
                } else {
                    u32 t = ((u32)v6b >> 5 & 0x7F) << 10;
                    if (self->field_0A >= 0 && !(self->field_00->field_64 & 0x80000000)) {
                        if (func_80062928(self->field_0A, t | 0x78000000 | ((u32)slot << 20), 3) != 0) {
                            self->field_00->field_6C |= 0x40000;
                        }
                    }
                    u32 token = t | 0xA0000000 | ((u32)slot << 20);
                    if (token != 0 && self->field_0A >= 0) {
                        if (func_80062928(self->field_0A, token, 3) != 0) {
                            self->field_00->field_6C |= 0x200000;
                            self->field_08 = 1;
                        }
                    }
                }
            } else {
                self->field_00->field_6C &= ~0x20000;
            }
        }
    }
    cf::CfResPcCharData* charData = func_8009EC9C(self->field_00->field_8C);
    u32 idx = 0;
    for (int i = 1; i <= 5; i++, idx++) {
        int v = ((cf::CfResPcVtIf*)self)->_v034(i);
        u32 r5;
        if (self->field_00->field_64 & 0x8) {
            r5 = (u32)func_8009E120(charData, (u16)(i - 1));
        } else if (self->field_40 != 0) {
            r5 = (u32)v;
        } else {
            r5 = func_80141E90(self->field_00->field_8C, (s16)charData->field_0E[idx], (u16)(idx + 1), 0);
        }
        if ((u32)v != r5) {
            v = (int)r5;
            ((cf::CfResPcVtIf*)self)->_v030(i, r5);
        }
        func_8018D154((cf::CfResPcImpl*)self, (u32)v, (u32)i);
    }
    if (self->field_00->field_64 & 0x2) {
        func_800BE3E8(self->field_00, 1);
    }
    ((cf::CfResPcVtIf*)self)->_v04C(v034_0);
    if (r29 != 0) {
        if (self->field_00->field_6C & 0x100000) {
            cf::CfResPcLoadBuffer buf;
            func_80080F48__Q22cf13CfGameManagerFv(self->field_00->field_8C, &buf, self->field_00->field_64 >> 31, 0);
            self->field_00->field_70C[0] = buf.field_1C;
            self->field_00->field_70C[1] = buf.field_1E;
        }
        for (int j = 0; j < 2; j++) {
            if ((j == 0 && self->field_00->field_70C[0] != 0) || (j == 1 && self->field_00->field_70C[1] != 0)) {
                int v = ((cf::CfResPcVtIf*)self)->_v034(j + 6);
                if (v != 0) {
                    ((cf::CfResPcVtIf*)self)->_v03C(j, v);
                    if (self->field_00->field_70C[j] == 0) {
                        self->field_00->field_70C[j] = 4;
                    }
                }
            }
        }
    }
    int token = 0;
    int v6 = ((cf::CfResPcVtIf*)self)->_v034(6);
    if (v6 != 0) {
        token = (v6 & 0xFE0) | 0x58000400;
    }
    if (self->field_00->field_64 & 0x2) {
        if (token != 0) {
            ((cf::CfResPcVtIf*)self)->_v050(token);
        }
    } else if ((self->field_00->field_64 & 0x80000000) && r29 != 0 &&
               (lbl_eu_80663E24 & 0x40000) && inst != 0 && token != 0) {
        u32 sp10 = 0xFFFFFFFF;
        u8* res = 0;
        if (func_8007E908__Q22cf13CfGameManagerFv(self->field_00->field_8C) != 0) {
            res = (u8*)func_800685C8(inst, (u32)token, &sp10);
        }
        if (res != 0) {
            self->field_34 = (u32)res;
            self->field_3C = -1;
            self->field_00->field_6C |= 0x20000;
        } else if (func_80062B3C((u8*)token, 4) != 0) {
            u32 spC;
            u32 sp8;
            cf::CfResPcFindEntry* found = findResEntry(inst, (u32)token, &spC, &sp8);
            if (found != 0) {
                self->field_3C = (u16)sp8;
                self->field_00->field_6C |= 0x20000;
                found->field_0C = 0;
            }
        }
    }
}

void func_8018E7E4(cf::CfResPcImpl* self);

// func_8018DE8C - PC resource advance driver: validates the resource entries
// (in-use checks on the +0x40 slots), builds the model/sound handles into
// the parent slots (+0x90/+0x94/+0x98/+0x9C/+0x708/+0x6D4), folds the +0x6C
// flag word, refreshes the player state and finally hands off to
// func_8018E7E4. When the player is the current character and the event-flag
// bit is set, a positioning block moves the player model (VEC3Add ps math)
// and notifies the +0xB8 vtable slot.
void func_8018DE8C(cf::CfResPcImpl* self) {
    s16 state = self->field_0A;
    if (state < 0) {
        return;
    }
    u16 slot = self->field_00->field_8C;
    cf::CfResPcTableEntry* a = func_80062C28(state, 0);
    cf::CfResPcLookupEntry* b = func_80062C88(state);
    cf::CfResPcLookupEntry* c = func_80062D44(state);
    cf::CfResPcLookupEntry* f = func_80062E64(state);
    if ((self->field_00->field_64 & 0x80000000) != 0) {
        b = (cf::CfResPcLookupEntry*)func_80062EC4(self->field_38);
        c = 0;
    }
    int ok = 1;
    int v = ((cf::CfResPcVtIf*)self)->_v034(0);
    if ((u32)b->field_04 != (u32)v) {
        self->field_00->field_6C &= 0xFFD88C0F;
        self->field_08 = 0;
        // Log the mismatching resource handle through the format string.
        u8 msgBuf[0x84];
        ml::FixStr<128>* msg = (ml::FixStr<128>*)msgBuf;
        msg->clear();
        int v1 = ((cf::CfResPcVtIf*)self)->_v034(0);
        int v2 = ((cf::CfResPcVtIf*)self)->_v034(0);
        msg->format(lbl_eu_80503BC4, (const char*)func_800AA5C0((void*)v2), v1);
        return;
    }
    if (self->field_00->field_6C & 0x2) {
        self->field_00->field_6C &= 0xFFD88C0E;
        self->field_08 = 0;
        return;
    }
    int flag = 0;
    if ((cf::CfObjectMove*)self->field_00 == cf::CfGameManager::getPlayer(0) &&
        (lbl_eu_80663E24 & 0x4000)) {
        // Position the player: add a fixed offset to its current position,
        // optionally corrected by func_804BE470, then place it (slot +0xB8).
        nw4r::math::VEC3* p = ((cf::CfResPcParentVtIf*)self->field_00)->_v0AC();
        nw4r::math::VEC3 pos = *p;
        nw4r::math::VEC3 off;
        off.x = lbl_eu_80667A40;
        off.y = lbl_eu_80667A44;
        off.z = lbl_eu_80667A40;
        nw4r::math::VEC3 sum = pos + off;
        nw4r::math::VEC3 work = sum;
        nw4r::math::VEC3 scratch;
        if (func_804BE470(&work, &scratch, 0, 0, 0) != 0) {
            pos = work;
        }
        nw4r::math::VEC3 zero;
        zero.x = lbl_eu_80667A40;
        zero.y = lbl_eu_80667A40;
        zero.z = lbl_eu_80667A40;
        func_804BD94C(&pos, &zero, 0x44A05, 0, 0, 0, lbl_eu_80667A4C,
                      lbl_eu_80667A50, lbl_eu_80667A54, lbl_eu_8066AF20,
                      lbl_eu_80667A58);
        ((cf::CfResPcParentVtIf*)self->field_00)->_v0B8(&pos, lbl_eu_80667A5C);
        flag = 1;
    }
    for (int i = 1; i <= 5; i++) {
        if (self->field_00->field_6C & 0x20) {
            int v2 = ((cf::CfResPcVtIf*)self)->_v034(i);
            if (v2 != 0) {
                cf::CfResPcLookupEntry* e = (cf::CfResPcLookupEntry*)&a[i];
                if (((cf::CfResPcEntryObjIf*)e->field_2C)->_v040(e) == 0) {
                    ok = 0;
                    break;
                }
            }
        }
    }
    if (self->field_00->field_6C & 0x10) {
        if (((cf::CfResPcEntryObjIf*)b->field_2C)->_v040(b) == 0) {
            ok = 0;
        }
    }
    if (self->field_00->field_6C & 0x10000) {
        if (((cf::CfResPcEntryObjIf*)f->field_2C)->_v040(f) == 0) {
            ok = 0;
        }
    }
    if (self->field_00->field_6C & 0x40000) {
        if (((cf::CfResPcEntryObjIf*)c->field_2C)->_v040(c) == 0) {
            ok = 0;
        }
    }
    u32 f64 = self->field_00->field_64;
    if ((f64 & 0x80000000) && !(f64 & 0x10000) && (lbl_eu_80663E24 & 0x09800000)) {
        return;
    }
    if (ok == 0) {
        return;
    }
    if (((cf::CfGameManager*)f64)->func_80082900() == 0) {
        return;
    }
    self->field_08++;
    func_80434A4C__Q23mtl10MemManagerFb(false);
    if ((self->field_00->field_6C & 0x20) && self->field_00->field_98 == 0) {
        u8* h = 0;
        int v1 = ((cf::CfResPcVtIf*)self)->_v034(1);
        if (v1 != 0) {
            s16 st = self->field_0A;
            if (st < 0) {
                h = 0;
            } else {
                cf::CfResPcTableEntry* a2 = func_80062C28(st, 0);
                h = ((cf::CfResPcEntryObjIf*)a2[1].field_2C)->_v008(
                    (cf::CfResPcLookupEntry*)&a2[1], slot);
            }
        }
        self->field_00->field_90 = (u32)h;
        void* hnd = func_80495E8C(CfRes_getD80Flag(), (u32)self->field_00->field_90, -1, 1);
        u8* d = (u8*)__dynamic_cast(hnd, 0, &lbl_eu_806624C0, &lbl_eu_806624D0, 0);
        func_800BBADC(self->field_00, d);
        func_8048472C(self->field_00->field_98, lbl_eu_80503BC4 + 7);
        ((cf::CfResPcResObj*)self->field_00->field_98)->field_7A4 |= 0x400000;
        if (self->field_00->field_98 != 0) {
            self->field_00->field_98->_v064(0);
        }
        for (int i = 2; i <= 5; i++) {
            int v2 = ((cf::CfResPcVtIf*)self)->_v034(i);
            if (v2 != 0) {
                u8* res = ((cf::CfResPcEntryObjIf*)a[i].field_2C)->_v008((cf::CfResPcLookupEntry*)&a[i], slot);
                if (*(u32*)res != 0) {
                    func_804831C4(self->field_00->field_98, res);
                }
            }
        }
    }
    if (self->field_00->field_6C & 0x10) {
        // FixStr<64> name buffer (mString[64] + mLength); cleared inline
        // (retail stb/stw - the out-of-line FixStr ctor would emit a bl).
        u8 buf64[0x44];
        ml::FixStr<64>& name = *(ml::FixStr<64>*)buf64;
        name.mString[0] = 0;
        name.mLength = 0;
        if (self->field_00->field_9C == 0) {
            func_800AA33C(name, (u32)b->field_04, 1, 0);
            self->field_00->field_94 = (u32)((cf::CfResPcEntryObjIf*)b->field_2C)->_v008(b, slot);
            self->field_00->field_9C = (u32)func_800584B8(CfRes_getD80Flag(),
                                                         (u32)self->field_00->field_94,
                                                         (const char*)&name);
        }
        if (self->field_00->field_6C & 0x10000) {
            if (self->field_00->field_6D4 == 0) {
                func_800AA33C(name, (u32)f->field_04, 1, 0);
                self->field_00->field_708 = (u32)((cf::CfResPcEntryObjIf*)f->field_2C)->_v008(f, slot);
                self->field_00->field_6D4 = (u32)func_800584B8(CfRes_getD80Flag(),
                                                              (u32)self->field_00->field_708,
                                                              (const char*)&name);
            }
        }
    }
    if (self->field_00->field_6C & 0x40000) {
        self->field_00->field_6DC = c;
        int v6 = ((cf::CfResPcVtIf*)self)->_v034(6);
        self->field_00->field_6E0 = ((u32)slot << 20) | 0x78000000 | ((u32)v6 >> 10) & 0x3F8000;
    }
    func_80434A4C__Q23mtl10MemManagerFb(true);
    u32 f68 = self->field_00->field_68;
    int b20 = (f68 >> 11) & 1;
    if (f68 & 0x100000) {
        func_800BB618(self->field_00, 0);
        ((cf::CfResPcParentVtIf*)self->field_00)->_v168(lbl_eu_80667A60);
        if (!(self->field_00->field_68 & 0x10000000)) {
            func_800BC3B0((cf::CfObjectMove*)self->field_00, lbl_eu_80667A64);
        }
    }
    if ((cf::CfObjectMove*)self->field_00 != cf::CfGameManager::getPlayer(0)) {
        self->field_00->field_6CC = 1;
    }
    func_800BCFA0(self->field_00);
    if (b20 != 0) {
        self->field_00->field_68 |= 0x100000;
    }
    ((cf::CfResPcParentVtIf*)self->field_00)->_v134(lbl_eu_80666B08);
    ((cf::CfResPcParentVtIf*)self->field_00)->_v1D4(lbl_eu_80667A68 * lbl_eu_80666B08);
    if (self->field_00->field_64 & 0x2) {
        func_800BE12C(self->field_00, 1, 0, -1, 1);
        if (flag != 0) {
            if (self->field_00->field_C4 != 0) {
                self->field_00->field_C4->field_4EC =
                    (self->field_00->field_C4->field_4EC & 0xFFF5FFFF) | 0x40000;
                self->field_00->field_C4->field_4EC |= 0x40;
            }
        }
    }
    func_8018E7E4(self);
}

// func_8018E69C - PC resource detach: looks up six resource-table entries
// by the state at field_0A and detaches each according to the parent +0x6C
// flag bits (0x40000 -> b, 0x3000 -> d, 0x20000 -> e, 0x200000 -> c,
// 0x10000 -> f, 0x10 -> a; the 0x40000 case also runs func_80066714 with
// cleanup), then folds the +0x6C flag word with 0xFFD88C0F and resets the
// handler index field_08.
void func_8018E69C(cf::CfResPcImpl* self) {
    s16 state = self->field_0A;
    cf::CfResPcLookupEntry* a = func_80062C88(state);
    cf::CfResPcLookupEntry* b = func_80062D44(state);
    cf::CfResPcLookupEntry* c = func_80062CE4(state);
    cf::CfResPcLookupEntry* d = func_80062DA4(state);
    cf::CfResPcLookupEntry* e = func_80062E04(state);
    cf::CfResPcLookupEntry* f = func_80062E64(state);
    if (self->field_00->field_6C & 0x40000) {
        func_80065CA4(b->field_2C, b);
        func_80066714(b, true);
    }
    if (self->field_00->field_6C & 0x3000) {
        func_80065CA4(d->field_2C, d);
    }
    if (self->field_00->field_6C & 0x20000) {
        func_80065CA4(e->field_2C, e);
    }
    if (self->field_00->field_6C & 0x200000) {
        func_80065CA4(c->field_2C, c);
    }
    if (self->field_00->field_6C & 0x10000) {
        func_80065CA4(f->field_2C, f);
    }
    if (self->field_00->field_6C & 0x10) {
        func_80065CA4(a->field_2C, a);
    }
    self->field_00->field_6C &= 0xFFD88C0F;
    self->field_08 = 0;
}

// func_8018E7E4 - PC resource open: looks up three table entries by the
// state, validates the parent in-use flags through the entry objects' +0x40
// slots, then builds the model objects (+0x6F8 and +0x6FC from entry_d's
// +0x08 slot), installs the bdat ids into the model objects, loads the
// CMcaFile handle and finally cleans up the slot handles. All parent
// accesses go through self->field_00 directly (retail reloads it at every
// site; the model object at +0x98 is held in one register across the block).
void func_8018E7E4(cf::CfResPcImpl* self) {
    // Declared to steer MWCC onto the retail register map:
    // slot=r31, entry_c=r30, entry_d=r29, entry_e=r28, ok/state=r27.
    u16 slot;
    s16 state;
    cf::CfResPcLookupEntry* entry_c;
    cf::CfResPcLookupEntry* entry_d;
    cf::CfResPcLookupEntry* entry_e;
    int ok;
    slot = self->field_00->field_8C;
    state = self->field_0A;
    entry_c = func_80062CE4(state);
    entry_d = func_80062DA4(state);
    entry_e = func_80062E04(state);
    ok = 1;
    if (self->field_00->field_64 & 0x80000000) {
        if (self->field_00->field_6C & 0x20000) {
            if (self->field_34 != 0) {
                entry_e = (cf::CfResPcLookupEntry*)self->field_34;
            } else if (self->field_3C >= 0) {
                entry_e = (cf::CfResPcLookupEntry*)func_80062EC4(self->field_3C);
            } else {
                self->field_00->field_6C &= ~0x20000;
            }
        }
    }
    // Copy the 0x50-byte position table from the static block into
    // table[1..10]; table[0] shares its first word with the mLength slot of
    // the FixStr buffer below (retail stack layout).
    cf::CfResPcPosTableEntry table[11];
    *(cf::CfResPcPosTable*)(table + 1) = lbl_eu_80503B30;
    if ((self->field_00->field_6C & 0x3000) &&
        ((cf::CfResPcEntryObjIf*)entry_d->field_2C)->_v040(entry_d) == 0) {
        ok = 0;
    }
    if ((self->field_00->field_6C & 0x20000) &&
        ((cf::CfResPcEntryObjIf*)entry_e->field_2C)->_v040(entry_e) == 0) {
        ok = 0;
    }
    if ((self->field_00->field_6C & 0x200000) &&
        ((cf::CfResPcEntryObjIf*)entry_c->field_2C)->_v040(entry_c) == 0) {
        ok = 0;
    }
    if (ok != 0) {
        self->field_08++;
        ((cf::CfResPcParentVtIf*)self->field_00)->_v218();
        func_80434A4C__Q23mtl10MemManagerFb(false);
        cf::CfResPc98ObjIf* obj98 = self->field_00->field_98;
        if ((self->field_00->field_6C & 0x1000) && self->field_00->field_6F8[0] == 0) {
            self->field_00->field_700 = (cf::CfResPc700Obj*)((cf::CfResPcEntryObjIf*)entry_d->field_2C)->_v008(entry_d, slot);
            if (self->field_00->field_700->field_00 != 0) {
                self->field_00->field_6F8[0] = (cf::CfResPcResObj*)func_80489A60(lbl_eu_80663E14, (u8*)self->field_00->field_700, -1, 0, 0, 0x70);
                if (self->field_00->field_6F8[0] != 0) {
                    self->field_00->field_6F8[0]->field_7A4 |= 0x40000000;
                }
                if (obj98 != 0 && self->field_00->field_6F8[0] != 0) {
                    if ((self->field_00->field_64 & 0x2) && !(lbl_eu_80663E24 & 0x20400)) {
                        ((cf::CfResPcParentVtIf*)self->field_00)->_v16C();
                        ((cf::CfResPcResObjVtIf*)self->field_00->field_6F8[0])->_v048();
                    }
                    if (obj98->_v0C4((u8*)self->field_00->field_6F8[0],
                                     (u32)CfBdat::func_801424A8(self->field_00->field_70C[0]), 0) == 0) {
                        ((cf::CfResPcParentVtIf*)self->field_00)->_v1D0(0);
                    } else {
                        u32 bdat = (u32)CfBdat::func_801424A8(self->field_00->field_70C[0]);
                        func_804873EC(obj98, bdat, 1);
                        if (slot <= 10) {
                            func_804875B8(obj98, bdat, table[slot].field_00, table[slot].field_04);
                        }
                        ((cf::CfResPcResObjVtIf*)self->field_00->field_6F8[0])->_v09C(2, 0);
                    }
                }
            }
        }
        if ((self->field_00->field_6C & 0x2000) && self->field_00->field_6F8[1] == 0) {
            self->field_00->field_700 = (cf::CfResPc700Obj*)((cf::CfResPcEntryObjIf*)entry_d->field_2C)->_v008(entry_d, slot);
            if (self->field_00->field_700->field_00 != 0) {
                self->field_00->field_6F8[1] = (cf::CfResPcResObj*)func_80489A60(lbl_eu_80663E14, (u8*)self->field_00->field_700, -1, 0, 0, 0x70);
                if (self->field_00->field_6F8[1] != 0) {
                    self->field_00->field_6F8[1]->field_7A4 |= 0x40000000;
                }
                if (obj98 != 0 && self->field_00->field_6F8[1] != 0) {
                    if ((self->field_00->field_64 & 0x2) && (lbl_eu_80663E24 & 0x20400)) {
                        ((cf::CfResPcResObjVtIf2*)self->field_00->field_6F8[1])->_v048(lbl_eu_80667A60);
                    } else {
                        ((cf::CfResPcParentVtIf*)self->field_00)->_v16C();
                        ((cf::CfResPcResObjVtIf*)self->field_00->field_6F8[1])->_v048();
                    }
                    if (obj98->_v0C4((u8*)self->field_00->field_6F8[1],
                                     (u32)CfBdat::func_801424A8(self->field_00->field_70C[1]), 0) == 0) {
                        ((cf::CfResPcParentVtIf*)self->field_00)->_v1D0(1);
                    } else {
                        u32 bdat2 = (u32)CfBdat::func_801424A8(self->field_00->field_70C[1]);
                        func_804873EC(obj98, bdat2, 1);
                        if (slot <= 10) {
                            func_804875B8(obj98, bdat2, table[slot].field_00, table[slot].field_04);
                        }
                        ((cf::CfResPcResObjVtIf2*)self->field_00->field_6F8[1])->_v09C(2, 0);
                    }
                }
            }
        }
        if ((self->field_00->field_6C & 0x20000) && self->field_00->field_6D8 == 0) {
            ml::FixStr<64> buf;
            func_800AA33C(buf, (u32)entry_e->field_04, 1, 0);
            self->field_00->field_704 = (u32)((cf::CfResPcEntryObjIf*)entry_e->field_2C)->_v008(entry_e, slot);
            CfResPcMca mca;
            __ct__CMcaFile((CMcaFile*)&mca, (void*)self->field_00->field_704);
            self->field_00->field_6D8 = (u32)func_80495EAC((void*)CfRes_getD80Flag(), mca.field_0C, (void*)&buf);
            func_800BD644(self->field_00);
        }
        if (!(self->field_00->field_68 & 0x100000)) {
            for (int i = 0; i < 2; i++) {
                if (self->field_00->field_6F8[i] != 0) {
                    func_804838DC(self->field_00->field_6F8[i], 0);
                }
            }
        }
        func_80434A4C__Q23mtl10MemManagerFb(true);
        if (self->field_00->field_38 != 0) {
            self->field_00->field_38->_v0B0();
        }
    }
}

// func_8018EE18 - PC reload state machine: with parent flag bit 0 of +0x6C
// set, asks the player object (parent minus 0x3E9C) via its vtable slot
// +0x2BC whether the resource is still in use; when it is not, clears that
// flag bit. With bit 0 clear and bit 1 set, invalidates the three state
// halfwords, notifies func_800BAB64, clears the +0x6C flag bits and resets
// the handler index.
void func_8018EE18(cf::CfResPcImpl* self) {
    cf::CfResPcParent* parent = self->field_00;
    u32 flags6C = parent->field_6C;
    if (flags6C & 0x1) {
        int ok = 1;
        if (parent->field_64 & 0x2) {
            // Recover the player base (parent sits at +0x3E9C inside it);
            // retail adjusts the parent register in place, guarded by a null
            // check (skip the subi when parent is null).
            if (parent != 0) {
                parent = (cf::CfResPcParent*)((char*)parent - 0x3E9C);
            }
            if (((cf::CfResPcPlayerVtIf*)parent)->_v2BC() != 0) {
                ok = 0;
            }
        }
        if (ok == 0) {
            self->field_00->field_6C &= ~0x3;
        }
    } else if (flags6C & 0x2) {
        self->field_38 = -1;
        self->field_3A = -1;
        self->field_3C = -1;
        func_800BAB64(parent);
        self->field_00->field_6C &= 0xFFD88C0D;
        self->field_08 = 0;
    }
}

// Dispatch one of the 4 reload handlers selected by field_08 (u16) when the
// state at field_0A is valid (>= 0). MWCC lowers the pmf call to
// mulli/lis/addi + `bl __ptmf_scall` against the lbl_eu_80532730 table.
void func_8018EEF0(cf::CfResPcImpl* self)
{
    if (self->field_0A >= 0 && self->field_08 < 4) {
        (self->*lbl_eu_80532730[self->field_08])();
    }
}



cf::UnkClass_8018EF3C::UnkClass_8018EF3C()
{
    field_400 = 0;
    field_404 = 0;
}

cf::UnkClass_8018EF3C::~UnkClass_8018EF3C()
{
    field_404 = 0;
    field_400 = 0;
    memset(this, 0, 0x400);
}

void func_8018EFB4(unsigned char* p)
{
    *reinterpret_cast<unsigned int*>(p + 0x404) = 0;
    *reinterpret_cast<unsigned int*>(p + 0x400) = 0;
    memset(p, 0, 0x400);
}

int func_8018EFCC() { return 1; }

// Load the file-event archive for `param` through the archive pair at +0x408
// (slot chosen by `flag`), then always report success.
int func_8018EFD4(cf::UnkClass_8018EF3C* self, int flag, u32 param)
{
    cf::CfTFile* file;
    if (flag != 0) {
        file = self->field_408->field_04;
    } else {
        file = self->field_408->field_00;
    }
    func_80069ACC(file, param);
    return 1;
}

// func_8018F018 - PC file-event load: picks the archive from the +0x408 pair
// by arg2 (field_04 when arg2 != 0, field_00 otherwise) and, depending on the
// archive state (func_80069C14), either clears the event flags and cancels
// the archive (func_800699B0), runs the 9-event, the 0xa-event or the
// 0x26-event.
int func_8018F018(cf::CfResPcFileHost* self, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    cf::CfTFile* file;
    if (arg2 != 0) {
        file = self->field_408->field_04;
    } else {
        file = self->field_408->field_00;
    }
    int result = 0;
    if (func_80069C14((u8*)file) == 0) {
        if ((lbl_eu_80663E24 & 0x2) != 0) {
            // Retail reloads the flag word for the mask store even though the
            // test read it moments earlier; the volatile load keeps MWCC from
            // CSE-eliminating the second read (same pattern as func_8016E854).
            *(volatile u32*)&lbl_eu_80663E24 &= ~0x2;
            lbl_eu_80663E28 &= ~0x10000;
            func_800699B0(file);
        } else {
            func_80061A80((u32)self, 9, arg2, arg3, arg4, arg5);
            result = 1;
        }
    } else if (arg2 != 0) {
        func_80061870((u32)self, 0xa, arg2, arg3, 0, 0);
    } else {
        // Retail loads the flag word twice for the two bit pairs; the volatile
        // first read keeps MWCC from CSE-merging the second pair's load.
        u32 v24 = *(volatile u32*)&lbl_eu_80663E24;
        u32 v24b = lbl_eu_80663E24;
        if (((v24 & 0x2000000) | (v24 & 0x400)) == 0 &&
            ((v24b & 0x40000) | (v24b & 0x8000)) == 0) {
            func_80062600();
            if (func_800625A0((int)arg3, 5) != 0) {
                lbl_eu_80663E28 &= ~0x10000;
            } else {
                func_80061A80((u32)self, 0x26, 0, arg3, 0, 0);
            }
        }
    }
    return result;
}

// func_8018F164 - PC resource load request: when func_800625A0(arg3, 5)
// succeeds, clears the game-manager mode flag bit 16 (0x10000); otherwise
// runs the 0x26 event with (arg2+1, arg3) and reports success.
int func_8018F164(cf::CfResPcImpl* self, int arg2, int arg3) {
    int result = 0;
    if (func_800625A0(arg3, 5) != 0) {
        lbl_eu_80663E28 &= ~0x10000;
    } else {
        func_80061A80((u32)self, 0x26, (u16)(arg2 + 1), (u32)arg3, 0, 0);
        result = 1;
    }
    return result;
}

// func_8018F1FC - PC map/section reload: resets manager helpers, latches the
// area id/sub-id halfwords into the event-flag block (clearing its bits
// 19/20), reloads the map bdat pointers, detaches the active lookup entry,
// then requests the section via the manager and re-enables pads. Always
// returns 1.
int func_8018F1FC(cf::CfResPcHostGM* self, u32 arg2, u32 arg3) {
    func_8007FE18__Q22cf13CfGameManagerFv(0);
    func_800B1E2C(0);
    self->field_408->func_8007D84C();
    u32 v = lbl_eu_80663E24;
    lbl_eu_80663E42 = arg2;
    v &= ~0x1800;
    lbl_eu_80663E44 = arg3;
    lbl_eu_80663E24 = v;
    func_8016EEB0(self->field_408->unkA0);
    func_80186664(self->field_408->field_0xA4);
    cf::CfBdat::resetMapBdatFileDataPointers();
    cf::CfResPcLookupEntry* entry = (cf::CfResPcLookupEntry*)func_80063038();
    func_80065CA4(entry->field_2C, entry);
    func_80085334__Q22cf13CfGameManagerFv(1);
    func_80083328__Q22cf13CfGameManagerFv(self->field_408, arg2, (u16)arg3);
    self->field_408->func_80085248();
    cf::CfGameManager::enablePadFlags(0x1FE01EFF, true);
    if (func_8018892C(1) != 0) {
        func_8007C140__Q22cf13CfGameManagerFv(1);
    }
    func_8004302C(1, 0);
    return 1;
}

// func_8018F2EC - PC resource load request: when the game-manager event
// flag bit 22 (0x400000) is clear, runs the event (arg2, 1), enables the
// pad mask 0x1FE01EFF and seeds the event timer to 0x5A; then stops voice
// id 4 and pokes character slots 1..13.
int func_8018F2EC(u32 arg1, u16 arg2) {
    if (!(lbl_eu_80663E24 & 0x400000)) {
        func_8008413C__Q22cf13CfGameManagerFv(arg2, 1);
        cf::CfGameManager::enablePadFlags(0x1FE01EFF, true);
        lbl_eu_80663E46 = 0x5A;
    }
    func_80084AD4__Q22cf13CfGameManagerFv(4);
    for (int i = 1; i <= 13; i++) {
        func_8009F6D4(func_8009EC9C((u16)i));
    }
    return 0;
}

// func_8018F368 - slot cleanup for the manager's second inner object: gated
// on the inner +0x82C bit 2 flag, the game-manager event-flag bit 5 and
// arg3 matching the inner +0x830 word. Each of the three record groups
// (handles at inner+0x834/+0x934/+0xA34, counts at +0xE4C/+0xE4E/+0xE50)
// is reset via func_80062B3C(handle, 4). arg2 (r4) is an unused
// register-slot parameter (retail never reads it; func_8018F510 passes its
// own cond through the tail call). Returns 0.
extern "C" __declspec(noinline) int func_8018F368(void* self, u32 arg2, u32 arg3) {
    cf::CfResPcMgrInner2* inner = ((cf::CfResPcMgrHost*)self)->field_408->field_04;
    if (!(inner->field_82C & 0x4) && (lbl_eu_80663E24 & 0x20) && arg3 == inner->field_830) {
        cf::CfResPcSlotTable2* tab = &inner->field_834;
        for (int i = 0; i < tab->field_618; i++) {
            u8* handle = tab->field_000[i].field_00;
            if (handle != 0) {
                func_80062B3C(handle, 4);
            }
        }
        for (int i = 0; i < tab->field_61A; i++) {
            u8* handle = tab->field_100[i].field_00;
            if (handle != 0) {
                func_80062B3C(handle, 4);
            }
        }
        for (int i = 0; i < tab->field_61C; i++) {
            u8* handle = tab->field_200[i].field_00;
            if (handle != 0) {
                func_80062B3C(handle, 4);
            }
        }
    }
    return 0;
}

// func_8018F46C - slot cleanup: when the inner manager's +0x82C flag bit 2
// is clear, walks the 8-byte slot records (count at inner+0xE56) and, for
// each live handle, ensures the manager global is initialized
// (func_800A98A8(0x200000) when func_800A807C() reports empty) and resets
// the slot via func_80062758(handle, 4); then runs func_8018F63C, the
// manager's func_8007D834 and func_800B1EC8, and returns 0. extern "C" +
// noinline keep func_8018F510's call-site reloc at the retail C name; the
// void* boundary matches func_8018F510's existing call (MWCC rejects an
// implicit void*->T* conversion, and a cast there would touch a matched
// function).
extern "C" __declspec(noinline) int func_8018F46C(void* self) {
    cf::CfResPcMgrInner* inner = ((cf::CfResPcMgrHost*)self)->field_408->field_00;
    if (!(inner->field_82C & 0x4)) {
        cf::CfResPcSlotTable* tab = &inner->field_834;
        for (int i = 0; i < tab->field_622; i++) {
            u8* obj = tab->field_610[i].field_00;
            if (obj != 0) {
                if (func_800A807C() == 0) {
                    func_800A98A8(0x200000);
                }
                func_80062758(obj, 4);
            }
        }
    }
    func_8018F63C(self);
    func_8007D834__Q22cf13CfGameManagerFv(((cf::CfResPcMgrHost*)self)->field_408);
    func_800B1EC8();
    return 0;
}

extern "C" void func_8018F510(void* self, unsigned long cond, unsigned long arg3){
    if (cond != 0)
        func_8018F368(self, cond, arg3);
    else
        func_8018F46C(self);
}

// func_8018F520 - file-event loader: queries the file-event table
// (func_8009D5FC), loads the archive selected by arg2's packed bit fields
// into the manager name buffer at +0x28 (func_8009EB2C), re-opens it
// (func_8007DA0C) with the result's two u16 ids, runs the script slot and
// latches the game-manager event flag bit 13 (0x2000).
int func_8018F520(cf::CfResPcHost408* self, u32 arg1, u32 arg2) {
    // Local layout view of the CfGameManager.hpp CfFileEventIdsView result
    // (two leading u16 ids); avoids pulling that header into this TU.
    struct LocalEventIds { u16 field_0x0; u16 field_0x2; };
    LocalEventIds* p = (LocalEventIds*)func_8009D5FC();
    func_8009EB2C((arg2 >> 20) & 0x7f, (arg2 >> 10) & 0x3ff, self->field_408 + 0x28);
    func_8007DA0C__Q22cf13CfGameManagerFv(self->field_408, p->field_0x2, p->field_0x0);
    func_80068AEC(self->field_408 + 0x28);
    lbl_eu_80663E24 |= 0x2000;
    return 0;
}

// func_8018F5A4 - PC resource load: when the manager's unk90 is live
// (vtable slot +0x74 returns 0), runs the 0x2 event with four caller args
// and reports success; otherwise falls back to func_8007E864 with
// (arg2, arg3).
int func_8018F5A4(cf::CfResPcHostGM* self, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    int result = 0;
    cf::CfGameManager* manager = self->field_408;
    UnkClass_80083298* obj = manager->unk90;
    if (obj != 0 && obj->vfunc_0x74() == 0) {
        func_80061A80((u32)self, 2, arg2, arg3, arg4, arg5);
        result = 1;
    } else {
        func_8007E864__Q22cf13CfGameManagerFv(arg2, (u16)arg3);
    }
    return result;
}

// func_8018F63C - PC map/area file-event refresh: resets two manager
// helpers, loads the packed area token through the file-event table (mode
// 0x1b event when the area flag is set), then - unless a load flag is set -
// resets the three handler slots, checks a set of area/event conditions
// against the static table and finally copies the file pair and syncs the
// area ids.
__declspec(noinline) void func_8018F63C(void* self) {
    func_8007B0A0(0);
    func_800B06C8();
    if (lbl_eu_80663E24 & 0x80000) {
        func_80062860(((u32)lbl_eu_80663E44 << 10) | (((u32)lbl_eu_80663E42 << 20) | 0x98000000), 4);
        func_80061870((u32)self, 0x1b, 0,
                      ((u32)lbl_eu_80663E44 << 10) | (((u32)lbl_eu_80663E42 << 20) | 0x98000000), 0, 0);
    } else {
        func_80062860(((u32)lbl_eu_80663E44 << 10) | (((u32)lbl_eu_80663E42 << 20) | 0x98000000), 5);
    }
    if (!(lbl_eu_80663E28 & 0x2000000)) {
        func_80068B9C();
        func_80061870((u32)self, 6, 2, 0, 0, 0);
        func_80061870((u32)self, 0x19, 0, 0, 0, 0);
        func_80061870((u32)self, 0x1a, 0, 0, 0, 0);
        if (func_8009CF8C(0x3520) != 0) {
            lbl_eu_80663E28 |= 0x2;
        } else if (lbl_eu_80663E28 & 0x8000) {
            // Retail reloads the flag word for the mask store (test read it
            // moments earlier); volatile keeps MWCC from CSE-eliminating the
            // second read (same pattern as func_8018F018).
            *(volatile u32*)&lbl_eu_80663E28 |= 0x2;
        } else if ((u32)cf::CfGameManager::func_800822F4() == 0x11 &&
                   lbl_eu_80663E42 == 2 && lbl_eu_80663E44 == 1 &&
                   func_8009CF8C(0x1e85) == 0) {
            lbl_eu_80663E28 |= 0x2;
        } else if ((u32)cf::CfGameManager::func_800822F4() == 0x53 &&
                   lbl_eu_80663E42 == 4 && lbl_eu_80663E44 == 1 &&
                   ((lbl_eu_80663E24 & 0x100000) | (lbl_eu_80663E24 & 0x200)) == 0) {
            lbl_eu_80663E28 |= 0x2;
        }
        cf::CfResPcAreaEntry* entry = lbl_eu_805327F8;
        for (u32 i = 0; i < 8; i++, entry++) {
            if ((u32)cf::CfGameManager::func_800822F4() == entry->field_00 &&
                entry->field_04 == lbl_eu_80663E42 &&
                entry->field_06 == lbl_eu_80663E44 &&
                ((lbl_eu_80663E24 & 0x100000) | (lbl_eu_80663E24 & 0x200)) == 0) {
                lbl_eu_80663E28 |= 0x2;
                break;
            }
        }
    }
    lbl_eu_80663E24 |= 0x800;
    func_80069A78(((cf::CfResPcMgrFileHost*)self)->field_408->field_08,
                  ((cf::CfResPcMgrFileHost*)self)->field_408->field_00);
    if (((lbl_eu_80663E24 & 0x100000) | (lbl_eu_80663E24 & 0x200)) == 0) {
        func_8016E164(lbl_eu_80663E42, lbl_eu_80663E44);
    }
    if (!(lbl_eu_80663E28 & 0x2000000)) {
        func_800B7410();
    }
}
