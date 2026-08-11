// Auto-scaffolded catalog TU for kyoshin/cf/CfResReloadImpl
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectModel.hpp"
#include "kyoshin/cf/CfResReloadImpl.hpp"

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

void func_8016CE5C(){}

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
            u32 t = h + 0xBCFFF;
            u32 q1 = (u32)(((u64)t * 0x5AC056B1u) >> 32);
            self->field_1F = (u8)(((((t - q1) >> 1) + q1) >> 19) & 0xFF);
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

void func_8016CFDC(){}

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

void func_8016D240(){}

// Picks the reload lookup param (5/3/3/4) from a global flag and two parent
// flag words, then queries the table with the caller's arg2 forwarded as the
// middle parameter; sets parent flag bits 0x30 on success.
void func_8016D2FC(cf::CfResReloadImpl* self, int arg2) {
    s16 v = self->field_0A;
    int p = 5;
    if (lbl_eu_80663E28 & 0x20) {
        p = 3;
    } else if (self->field_00->field_68 & 0x04000000) {
        p = 3;
    } else if (self->field_00->field_64 & 0x10000) {
        p = 4;
    }
    u32 ret = func_80062998(v, arg2, p);
    // retail materializes the != 0 test as a 0/1 value: neg/or/rlwinm
    if ((u32)(-ret | ret) >> 31) {
        self->field_00->field_6C |= 0x30;
    }
}

// Picks the lookup param (5 or 3) from parent flag bit 26, queries the
// table with the caller's arg2 forwarded, and sets parent flag bit 4 when
// the query succeeds.
void func_8016D390(cf::CfResReloadImpl* self, int arg2) {
    s16 v = self->field_0A;
    int p = 5;
    if (self->field_00->field_68 & 0x04000000) {
        p = 3;
    }
    u32 ret = func_80062998(v, arg2, p);
    // retail materializes the != 0 test as a 0/1 value: neg/or/srwi
    if ((u32)(-ret | ret) >> 31) {
        self->field_00->field_6C |= 0x10;
    }
}

void func_8016D3F8(){}

void func_8016D688(){}

void func_8016DAF8(){}

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

u16 getReloadParam0() { return lbl_eu_80664278; }

void updateReloadFloat(short param_1)
{
    lbl_eu_80664278 = param_1;
    lbl_eu_80664284 = lbl_eu_806676B0 * lbl_eu_80666210;
}

void func_8016DF4C(){}

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
    v5 = p->field_0A;
    lbl_eu_80664284 = v0;
    lbl_eu_80664278 = v1;
    lbl_eu_8066427E = v2;
    lbl_eu_80664280 = v3;
    lbl_eu_8066427A = v4;
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

void func_8016E1AC(){}

void func_8016E430(){}

void func_8016E578(){}

void func_8016E654(){}

void func_8016E854(){}

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

void func_8016EA68(){}

void func_8016EC58(){}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_8016ED44() {
    extern unsigned short lbl_eu_80664288;
    lbl_eu_80664288 = 0;
}
