// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectModel
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include "kyoshin/cf/object/CfObjectModel.hpp"
#include "kyoshin/cf/CfBdat.hpp"
#include "monolib/math/CAttrTransform.hpp"
#include "kyoshin/action/CActParamAnim.hpp"
#include <nw4r/g3d/res/g3d_resmdl.h>
#include <nw4r/db/db_assert.h>
#include "kyoshin/code_801862C0.hpp"

namespace {
// Cast-only replica of the cf-chain vtable head (CObjectParam occupies
// 0x08-0x50, the first CfObject slot lands at 0x54). novtable: never
// constructed, only reinterpret_cast, so MWCC emits no vtable data and the
// m68() call compiles to a genuine load-vptr/load-slot/bctrl dispatch.
struct __declspec(novtable) ModelDtorVt : public cf::CObjectParam {
    virtual void m54();  // 0x54 (base-dtor placeholder)
    virtual void m58();  // 0x58
    virtual void m5C();  // 0x5C
    virtual void m60();  // 0x60
    virtual void m64();  // 0x64
    virtual void m68();  // 0x68: cleanup slot called by the model dtor
};

// View of the +0xB0 sub-object: it stores its function-table pointer at
// +0x10 (not +0x00), so the proxy derives from a 0x10-byte prefix base and
// inherits the vptr slot right after it; slot +0x08 is the deleting dtor.
struct SubObjB0Head {
    u8 _pad00[0x10];   // 0x00-0x0F
};
struct __declspec(novtable) SubObjB0If : SubObjB0Head {
    virtual void m08(s32 flag);  // table +0x08: deleting destructor
};
} // namespace

// Complete-object constructor (retail __ct__Q22cf13CfObjectModelFv). Two
// stages, mirroring retail scheduling: stage 1 installs the model vtable,
// zeroes/fills the shared base region (0x04-0x8F) with the position
// constants, and issues one virtual dispatch (slot +0x5C). Stage 2 sets the
// 0x00100000 flag bit, installs the cf::CfObject base vtable (the derived
// ctor finishes by leaving the object looking like a plain CfObject;
// subclasses overwrite the vtable afterwards), then initializes the
// model-specific tail 0x90-0xC0 with field_BD = 1. Constants are read from
// the globals in both stages (retail reloads them after the call instead of
// keeping them in nonvolatile FPRs - the prologue saves no floats).
cf::CfObjectModel::CfObjectModel() {
    // Unnamed gap fields reached through small overlays; everything else
    // uses the real inherited members.
    struct Gap34 { u8 _p00[0x34]; u32 w34; };          // 0x34
    struct Gap10 { u8 _p00[0x10]; u8 b10; };           // 0x10 (retail zeroes only the byte)
    struct Gap48 { u8 _p00[0x48]; float f48; };        // 0x48
    struct Gap50 { u8 _p00[0x50]; float f50; };        // 0x50
    struct Sub54Flt { float a, b, c; };                // 0x54-0x5F
    struct Gap6C { u8 _p00[0x6C]; u32 w6c; u32 w70; u32 w74; u8 b78; };  // 0x6C-0x78
    struct Gap88 { u8 _p00[0x88]; u32 w88; u16 h8c; u16 h8e; };           // 0x88-0x8F
    struct Tail90 {                                    // 0x90-0xC3
        u32 w90, w94;
        cf::CfObjectModelSub98* sub98;
        u32 w9c;
        float fa0;
        float fa4[3];
        void* subB0;
        u32 wb4, wb8;
        u8 bbc, bbd;
        u8 _pbe[2];
        u32 wc0;
    };
    Gap10* g10 = reinterpret_cast<Gap10*>(this);
    Gap34* g34 = reinterpret_cast<Gap34*>(this);
    Gap48* g48 = reinterpret_cast<Gap48*>(this);
    Gap50* g50 = reinterpret_cast<Gap50*>(this);
    Gap6C* g6c = reinterpret_cast<Gap6C*>(this);
    Gap88* g88 = reinterpret_cast<Gap88*>(this);
    Tail90* t90 = reinterpret_cast<Tail90*>(this);
    unk4 = 0;
    unk8 = 0;
    unkC = 0;
    g10->b10 = 0;  // retail stores only the byte at 0x10
    field_30 = 0;
    g34->w34 = 0;
    *(void**)this = (void*)lbl_eu_805294E0;
    mSubObj38 = 0;
    mPos3C = lbl_eu_80666A68;
    mPos40 = lbl_eu_80666A68;
    mPos44 = lbl_eu_80666A68;
    g48->f48 = lbl_eu_80666A68;
    mField4C = lbl_eu_80666A68;
    g50->f50 = lbl_eu_80666A68;
    Sub54Flt* sub54 = reinterpret_cast<Sub54Flt*>(&mSubObj54);
    sub54->a = lbl_eu_80666A68;
    sub54->b = lbl_eu_80666A68;
    sub54->c = lbl_eu_80666A68;
    mFloat60 = lbl_eu_80666A6C;
    mFlags68 = 0;
    g6c->w6c = 0;
    g6c->w70 = 0;
    g6c->w74 = 0;
    g6c->b78 = 0;
    g88->w88 = 0;
    g88->h8c = 0;
    g88->h8e = 0;
    reinterpret_cast<ModelDtorVt*>(this)->m5C();  // vtable +0x5C
    // Phase 2: flag bit, base vtable install, model-tail init.
    mFlags68 |= 0x00100000;
    *(void**)this = (void*)lbl_eu_80529318;
    t90->w90 = 0;
    t90->w94 = 0;
    t90->sub98 = 0;
    t90->w9c = 0;
    t90->fa0 = lbl_eu_80666A68;
    t90->fa4[0] = lbl_eu_80666A6C;
    t90->fa4[1] = lbl_eu_80666A6C;
    t90->fa4[2] = lbl_eu_80666A6C;
    t90->subB0 = 0;
    t90->wb4 = 0;
    t90->wb8 = 0;
    t90->bbc = 0;
    t90->bbd = 1;
    t90->wc0 = 0;
}

// Retail returns 1 even though the base CfObject header declares the slot as
// void; defined with the verbatim mangled name so the vtable slot 0x58
// (pure-virtual override) still resolves to this symbol.
int CfObject_UnkVirtualFunc2__Q22cf13CfObjectModelFv(cf::CfObjectModel* self) {
    self->CfObject_UnkVirtualFunc8();  // vtable +0x70
    self->mFlags68 |= 0x00400000;
    return 1;
}

// Complete-object destructor (retail __dt__Q22cf13CfObjectModelFv). Written
// out by hand because MWCC's auto dtor machinery would emit an implicit D1
// vtable store and a synthesized base-dtor call that retail does not have.
// Sequence: read the +0x68 flags word, install the cf::CfObject vtable,
// unregister from the manager when bit 0x40000000 is set, run the slot-0x68
// cleanup, destroy the +0xB0 sub-object, then restore the model vtable and
// run slot-0x68 once more before the delete-flag wrapper.
extern "C" __declspec(noinline) cf::CfObjectModel* __dt__Q22cf13CfObjectModelFv(
    cf::CfObjectModel* self, s32 deleteFlag) {
    // Null guard branches straight past the whole body including the
    // delete-flag wrapper (retail beq lands on the shared epilogue).
    if (self == 0) {
        goto finish;
    }
    u32 flags = self->mFlags68;
        *(void**)self = (void*)lbl_eu_80529318;
        if (flags & 0x40000000) {
            func_80186474(func_801862C0(), self);
        }
        reinterpret_cast<ModelDtorVt*>(self)->m68();
        if (self->mSubObjB0 != 0) {
            if (self->mSubObjB0 != 0) {
                // Redundant nested re-check reproduces retail's dead second
                // beq (MWCC keeps both branch targets).
                reinterpret_cast<SubObjB0If*>(self->mSubObjB0)->m08(1);
            }
            self->mSubObjB0 = 0;
        }
        if (self != 0) {
            *(void**)self = (void*)lbl_eu_805294E0;
            reinterpret_cast<ModelDtorVt*>(self)->m68();
        }
    if (deleteFlag > 0) {
        operator delete(self);
    }
finish:
    return self;
}

// POD deleting destructor (retail symbol is address-derived, no class name):
// delete self only when a delete flag is passed, then return self.
u8* __dt__800BAA24(u8* object, s32 deleteFlag) {
    if (object != 0 && deleteFlag > 0) {
        __dl__FPv(object);
    }
    return object;
}

// Teardown: unregister from the effect chain, release the +0x38 sub-object,
// run the model-specific cleanup pair (vtable 0x17C then 0x178) and reset
// the +0x70/+0x90/+0x94 words plus the +0x68 flag mask.
void cf::CfObjectModel::CfObject_UnkVirtualFunc6() {
    notifyDetach_(this);
    if (mSubObj38 != 0) {
        reinterpret_cast<cf::CfObjectSub38If*>(mSubObj38)->mAC();
        if (mSubObj38 != 0) {
            // Redundant nested check on the reloaded value mirrors retail's
            // two beq targets (MWCC keeps both branches).
            if (mSubObj38 != 0) {
                reinterpret_cast<cf::CfObjectSub38If*>(mSubObj38)->m08(1);
            }
            mSubObj38 = 0;
        }
    }
    CfObjectModel_UnkVirtualFunc2();  // vtable +0x17C
    CfObjectModel_UnkVirtualFunc1();  // vtable +0x178
    // The +0x70/+0x90/+0x94 words are exposed as opaque byte arrays by the
    // header, so the zero stores go through a local overlay.
    struct Model7090 {
        u8 _pad00[0x70];
        u32 field_70;   // 0x70
        u8 _pad74[0x90 - 0x74];
        u32 field_90;   // 0x90
        u32 field_94;   // 0x94
    };
    Model7090* data = reinterpret_cast<Model7090*>(this);
    u32 flags = mFlags68;
    data->field_90 = 0;
    flags &= 0x40100000;
    data->field_94 = 0;
    data->field_70 = 0;
    mFlags68 = flags;
}

void cf::CfObjectModel::CfObjectModel_UnkVirtualFunc1() {
    if (mSubObj98 != 0) {
        func_80495E60(mSubObj98);
        mSubObj98 = 0;
    }
}

void CfObjectModel_UnkVirtualFunc2__Q22cf13CfObjectModelFv() {}

void func_800BAB64(cf::CfObjectModel* self) {
    self->CfObjectModel_UnkVirtualFunc2();  // vtable +0x17C
    self->CfObjectModel_UnkVirtualFunc1();  // vtable +0x178
    // Zero the two words at +0x90/+0x94 (the header exposes them as the
    // opaque byte array field_0x90, so write through a local overlay).
    struct Model90 {
        u8 _pad00[0x90];
        u32 field_90;  // 0x90
        u32 field_94;  // 0x94
    };
    Model90* data = reinterpret_cast<Model90*>(self);
    data->field_90 = 0;
    data->field_94 = 0;
}

// Replaces the +0x38 sub-object: release the old one (vtable 0xAC then
// 0x8-with-flag), store the new one and attach it (vtable 0x2C with this
// as parent, then 0xA0). Retail symbol is Fv even though the body reads the
// new object from r4 - forced-name form.
void CfObject_UnkVirtualFunc8__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, void* newObj) {
    if (self->mSubObj38 != 0) {
        reinterpret_cast<cf::CfObjectSub38If*>(self->mSubObj38)->mAC();
    }
    if (self->mSubObj38 != 0) {
        // Redundant re-check on the reloaded pointer: MWCC shares the compare
        // between the two tests and emits two beq targets (skip-call /
        // skip-block), matching retail.
        if (self->mSubObj38 != 0) {
            reinterpret_cast<cf::CfObjectSub38If*>(self->mSubObj38)->m08(1);
        }
        self->mSubObj38 = 0;
    }
    self->mSubObj38 = newObj;
    if (newObj != 0) {
        reinterpret_cast<cf::CfObjectSub38If*>(newObj)->m2C(self);
        reinterpret_cast<cf::CfObjectSub38If*>(self->mSubObj38)->mA0();
    }
}

// Retail returns a flag word even though the base header declares the slot
// void; the mangled name is written verbatim (MWCC does not mangle global
// functions) so vtable references still resolve to this symbol. The word at
// +0x70 (first field of the derived layout) selects the flag via its top 5
// bits; unk64's low 16 bits are the fallback/default value.
u32 CfObject_UnkVirtualFunc63__Q22cf8CfObjectFv(cf::CfObject* self) {
    struct Object70 {
        u8 _pad00[0x70];
        u32 field_70;  // 0x70
    };
    Object70* obj = reinterpret_cast<Object70*>(self);
    u32 word70 = obj->field_70;
    u32 result = self->unk64 & 0xFFFF;
    if (word70 == 0) {
        return result;
    }
    u32 kind = word70 >> 27;
    if (kind - 2 > 4) {
        return result;
    }
    switch (kind) {
    case 2:
        return 2;
    case 3:
        return 8;
    case 4:
        return 4;
    case 6:
        return 0x80;
    }
    return result;
}

// Returns a packed flag: the +0x70 word's top-5-bit type (2/3/4/6 map to
// 2/8/4/0x80), refreshed from func_800AA714's hash of the vtable+0x180
// result when both are non-zero; the +0x64 low 16 bits are the default.
// Retail returns a u32 even though the base header declares the slot void -
// written with the verbatim mangled name.
u32 CfObject_UnkVirtualFunc63__Q22cf13CfObjectModelFv(cf::CfObjectModel* self) {
    struct Object70 {
        u8 _pad00[0x70];
        u32 field_70;  // 0x70
    };
    Object70* obj = reinterpret_cast<Object70*>(self);
    u32 result = self->unk64 & 0xFFFF;
    u32 word = obj->field_70;
    void* p = self->CfObjectModel_UnkVirtualFunc3();  // vtable +0x180
    if (p != 0) {
        u32 q = func_800AA714(reinterpret_cast<const char*>(p));
        if (q != 0) {
            word = q;
        }
    }
    if (word != 0) {
        u32 kind = word >> 27;
        if (kind - 2 <= 4) {
            switch (kind) {
            case 2:
                result = 2;
                break;
            case 3:
                result = 8;
                break;
            case 4:
                result = 4;
                break;
            case 6:
                result = 0x80;
                break;
            }
        }
    }
    return result;
}

// Call the cf-chain vtable slot +0x14C (retail CfObject_UnkVirtualFunc63
// returns a flag word) and return bit 1 of it.
u32 func_800BAD98(cf::CfObject* obj) {
    return (reinterpret_cast<cf::CfObjectVt14C*>(obj)->m14C() >> 1) & 1;
}

// Same as func_800BAD98 but returns bit 2 of the flag word.
u32 func_800BADC8(cf::CfObject* obj) {
    return (reinterpret_cast<cf::CfObjectVt14C*>(obj)->m14C() >> 2) & 1;
}

// Same as func_800BAD98 but returns bit 3 of the flag word.
u32 func_800BADF8(cf::CfObject* obj) {
    return (reinterpret_cast<cf::CfObjectVt14C*>(obj)->m14C() >> 3) & 1;
}

// Call the cf-chain vtable slot +0x14C and return bit 7 of the flag word.
u32 func_800BAE28(cf::CfObject* obj) {
    return (reinterpret_cast<cf::CfObjectVt14C*>(obj)->m14C() >> 7) & 1;
}

// Copies the vector into the sub-object transform's position and refreshes
// it, then mirrors the vector into this object's +0x3C position (retail
// symbol is Fv even though the body reads r4).
void CfObject_UnkVirtualFunc19__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, const ml::CVec3* vec) {
    if (self->mSubObj98 != 0) {
        ml::CAttrTransform* transform = static_cast<ml::CAttrTransform*>(func_8048315C(self->mSubObj98));
        transform->mPos = *vec;
        transform->update();
    }
    // Retail copies through GPRs (lwz/stw): MWCC emits integer copies for
    // CVec3 (nameless-union float) struct assignments, not lfs/stfs.
    *reinterpret_cast<ml::CVec3*>(&self->mPos3C) = *vec;
}

// Copies the vector into the sub-object transform's position and refreshes
// it, then mirrors the vector into this object's +0x3C position (retail
// symbol is Fv even though the body reads r4; same body as the +0x9C slot).
void CfObject_UnkVirtualFunc22__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, const ml::CVec3* vec) {
    if (self->mSubObj98 != 0) {
        ml::CAttrTransform* transform = static_cast<ml::CAttrTransform*>(func_8048315C(self->mSubObj98));
        transform->mPos = *vec;
        transform->update();
    }
    *reinterpret_cast<ml::CVec3*>(&self->mPos3C) = *vec;
}

// Collision/ground-query helpers used by UnkVirtualFunc25 below (retail ABI:
// func_804BE398 takes four GPR args plus two FP args; func_800A7094 takes a
// probe vector, result vector, filter word and two floats).
// func_804BE398/BE4B4/BE4E0 come from CActParamAnim.hpp (retail ABI: four
// GPR args plus two FP args on the probe).
extern "C" int func_800A7094(ml::CVec3* pos, ml::CVec3* out, int filter,
                             float f, float g);

// Base ground-snap entry point. Retail symbol is Fv (the base header declares
// slot +0xB4 parameter-less) but the body consumes the caller's r4/f1 as
// (position, scale). Selects a collision filter word from bits 30/29 of
// +0x64, probes downward from the position, optionally re-probes from a
// lifted point, and finally dispatches vtable slot +0x9C with the probed
// position still live in r4.
extern "C" void CfObject_UnkVirtualFunc25__Q22cf8CfObjectFv(
    cf::CfObject* self, ml::CVec3* pos, float scale) {
    ml::CVec3 probe;
    ml::CVec3 hitPos;
    ml::CVec3 normal;
    ml::CVec3 lifted;

    u32 flags = self->unk64;
    // Filter word: default mid variant; bit30 clear picks the narrow one,
    // bit29 (when bit30 set) picks the wide one. Each unk64 bit test is
    // normalized through the double-cntlzw booleanize idiom (retail
    // booleanizes twice).
    u32 filter = 0x44a09;
    if ((((u32)__cntlzw((u32)__cntlzw(flags & 0x2) >> 5)) >> 5) == 0) {
        filter = 0x4a05;
    } else if ((((u32)__cntlzw((u32)__cntlzw(flags & 0x4) >> 5)) >> 5) != 0) {
        filter = 0x44a11;
    }

    if ((((u32)__cntlzw((u32)__cntlzw(flags & 0x2) >> 5)) >> 5) != 0) {
        func_800A7094(pos, &probe, filter, scale, lbl_eu_80666A68);

        // Probe down from the hit point...
        int found = func_804BE398(&probe, 0, 0x40000, 0, lbl_eu_80666A70,
                                  lbl_eu_80666A74);
        if (found != 0) {
            func_804BE4B4(&hitPos, 0);
            func_804BE4E0(&normal, 0);
            // ...then re-probe from a lifted copy of the hit point.
            ml::CVec3 offset(lbl_eu_80666A68, lbl_eu_80666A78, lbl_eu_80666A68);
            ml::CVec3 sum = probe + offset;
            lifted.set(sum);
            if (func_804BE398(&lifted, 0x44a05, 0, 0, lbl_eu_80666A7C,
                              lbl_eu_80666A74)) {
                found = 0;
            }
        }
        if (found != 0) {
            probe.y -= lbl_eu_80666A6C;
        }
    } else {
        func_800A7094(pos, &probe, 0x4a05, scale, lbl_eu_80666A68);
    }
    self->CfObject_UnkVirtualFunc19();
}

// Retail symbol is Fv (no params) but the body consumes two floats in f1/f2 -
// forced-name form; packs them with a constant into a stack vector and calls
// the cf-chain vtable slot +0xB4 (CfObject_UnkVirtualFunc25, hidden args).
void CfObject_UnkVirtualFunc20__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, float a, float b) {
    float arr[3];
    arr[1] = lbl_eu_80666A80;
    arr[0] = a;
    arr[2] = b;
    reinterpret_cast<cf::CfObjectVtB4*>(self)->mB4(arr, lbl_eu_80666A84);
}

// Return a pointer-typed word: the sub-object's derived value +0xB8, or
// this +0x3C when there is no sub-object.
u32 cf::CfObjectModel::CfObject_UnkVirtualFunc23() {
    if (mSubObj98 != 0) {
        return static_cast<u32>(reinterpret_cast<uintptr_t>(func_8048315C(mSubObj98)) + 0xB8);
    }
    return static_cast<u32>(reinterpret_cast<uintptr_t>(this) + 0x3C);
}

void* CfObject_UnkVirtualFunc24__Q22cf13CfObjectModelFv(void* self) { void* p = *(void**)((u8*)self + 0x98); if (p != 0) { return (u8*)p + 0x310; } return (u8*)self + 0x3c; }

// Copies the vector into the sub-object transform's rotation and refreshes
// it, then mirrors the vector into this object's +0x48 position (retail
// symbol is Fv even though the body reads r4). The +0x48..+0x53 region is
// exposed by the base header as _pad48/mField4C/_pad50, so the vector write
// goes through a local overlay.
struct ModelVec48 {
    u8 _pad00[0x48];
    ml::CVec3 mVec48;  // 0x48-0x53
};
void CfObject_UnkVirtualFunc27__Q22cf13CfObjectModelFPv(cf::CfObjectModel* self, const ml::CVec3* vec) {
    if (self->mSubObj98 != 0) {
        ml::CAttrTransform* transform = static_cast<ml::CAttrTransform*>(func_8048315C(self->mSubObj98));
        transform->mRot = *vec;
        transform->update();
    }
    reinterpret_cast<ModelVec48*>(self)->mVec48 = *vec;
}

void* CfObject_UnkVirtualFunc28__Q22cf13CfObjectModelFv(void* self) {
    struct Data {
        u8 _pad00[0x48];
        u8 field_48[0x50];  // 0x48-0x97
        void* mPtr;         // 0x98
    };
    Data* data = static_cast<Data*>(self);
    void* ptr = data->mPtr;
    if (ptr) {
        return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(func_8048315C(ptr)) + 0xC4);
    } else {
        return data->field_48;
    }
}

// Stores the value into the sub-object transform's rotation-Y and refreshes
// it, then stores it at +0x4C (retail symbol is Fv even though the body
// reads f1).
void CfObject_UnkVirtualFunc29__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, float value) {
    if (self->mSubObj98 != 0) {
        ml::CAttrTransform* transform = static_cast<ml::CAttrTransform*>(func_8048315C(self->mSubObj98));
        transform->mRot.y = value;
        transform->update();
    }
    self->mField4C = value;
}

float CfObject_UnkVirtualFunc31__Q22cf13CfObjectModelFv(void* self) {
    void* ptr = *(void**)((unsigned char*)self + 0x98);
    if (ptr != 0) {
        void* result = func_8048315C(ptr);
        return *(float*)((unsigned char*)result + 0xc8);
    } else {
        return *(float*)((unsigned char*)self + 0x4c);
    }
}

extern "C" void CfObject_UnkVirtualFunc32__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, float f) {
    // Scale f by the global factor, then tail-call the virtual
    // CfObject_UnkVirtualFunc29 (vtable+0xC4) with the scaled value. The
    // upcast to CfObject resolves the base (float) overload that CfObjectModel
    // hides with its no-arg override.
    ((cf::CfObject*)self)->CfObject_UnkVirtualFunc29(f * lbl_eu_8066A210);
}

// Scale the CfObject_UnkVirtualFunc31 result by the global scale factor.
float cf::CfObjectModel::CfObject_UnkVirtualFunc34() {
    float value = CfObject_UnkVirtualFunc31();
    return value * lbl_eu_8066A20C;
}

void CfObject_UnkVirtualFunc35__Q22cf13CfObjectModelFv(void* self, float f) {
    void* p = *(void**)((u8*)self + 0x98);
    if (p != 0) {
        *(float*)((u8*)p + 0x304) = f;
        *(float*)((u8*)p + 0x308) = f;
        *(float*)((u8*)p + 0x30c) = f;
    }
    *(float*)((u8*)self + 0x60) = f;
}

float CfObject_UnkVirtualFunc36__Q22cf13CfObjectModelFv(void* self) { return *(float*)((u8*)self + 0x60); }

// Returns bit 4 of the sub-object's +0x7A4 flag word (0 when no sub-object).
u32 func_800BB340(cf::CfObjectModel* self) {
    cf::CfObjectModelSub98* sub = self->mSubObj98;
    if (sub != 0) {
        // Retail dispatches to the sub-object's vtable slot +0xA8 and returns
        // its value; the slot returns u32 in retail, so it goes through the
        // CfObjectModelSub98Vt proxy (the base header's Func22 is void).
        return reinterpret_cast<cf::CfObjectModelSub98Vt*>(sub)->mA8();
    }
    return 0;
}

// Height difference between the two positions the sub-object helper computes;
// a fixed fallback constant when there is no sub-object.
// const self (forced-name free function): MWCC hoists the mSubObj98 load
// (lwz r3,0x98(r3)) above the LR save (stw r0,0x24(r1)) — the load-hoist
// family lever (CfObjectMove_UnkVirtualFunc9 / CScnEffectActNw4r getters);
// a non-const member emits stw-then-lwz (2 structural).
extern "C" float CfObject_UnkVirtualFunc56__Q22cf13CfObjectModelFv(const cf::CfObjectModel* self) {
    if (self->mSubObj98 == 0) {
        return lbl_eu_80666A68;
    }
    CfObjectModelVec3 outA, outB;
    func_80490A44(self->mSubObj98, &outA, &outB);
    return outB.y - outA.y;
}

// Looks up a named node in the model resource and returns the matching
// per-node matrix slot (mtx id * 0x30 into the table base at
// sub->field_147C->field_EC). Retail symbol is Fv but the body reads the
// node name from r4 - forced-name form.
u8* CfObject_UnkVirtualFunc52__Q22cf13CfObjectModelFv(const cf::CfObjectModel* self, const char* name) {
    cf::CfObjectModelSub98* sub = self->mSubObj98;
    if (sub == 0) {
        return 0;
    }
    nw4r::g3d::ResMdl resMdl(sub->field_146C);
    cf::CfObjectModelActTable* table = sub->field_147C;
    nw4r::g3d::ResNode node = resMdl.GetResNode(name);
    if (node.ptr() == 0) {
        return 0;
    }
    // Dead-but-present assert on the same validity check: MWCC reuses the
    // outer compare's cr0 flag for both branches.
    if (!node.IsValid()) {
        nw4r::db::Panic(lbl_eu_80529678, 0x53, lbl_eu_80529658);
    }
    return table->field_EC + node.GetMtxID() * 0x30;
}

// Same node-matrix lookup as CfObject_UnkVirtualFunc52 but by node index
// (retail symbol is Fv even though the body reads the index from r4).
u8* CfObject_UnkVirtualFunc53__Q22cf13CfObjectModelFv(const cf::CfObjectModel* self, u32 idx) {
    cf::CfObjectModelSub98* sub = self->mSubObj98;
    if (sub == 0) {
        return 0;
    }
    nw4r::g3d::ResMdl resMdl(sub->field_146C);
    cf::CfObjectModelActTable* table = sub->field_147C;
    nw4r::g3d::ResNode node = resMdl.GetResNode(idx);
    if (node.ptr() == 0) {
        return 0;
    }
    if (!node.IsValid()) {
        nw4r::db::Panic(lbl_eu_80529678, 0x53, lbl_eu_80529658);
    }
    return table->field_EC + node.GetMtxID() * 0x30;
}

// Return the sub-object's +0x14AC word when a sub-object exists and its
// +0x7A4 0x40000000 flag is set, else 0. The retail symbol is Fv with a
// value return, so the exact mangled name is written verbatim.
u32 CfObject_UnkVirtualFunc54__Q22cf13CfObjectModelFv(cf::CfObjectModel* self) {
    cf::CfObjectModelSub98* sub = self->mSubObj98;
    u32 flag = (sub != 0) && ((sub->field_7A4 & 0x40000000) != 0);
    if (flag != 0) {
        return sub->field_14AC;
    }
    return 0;
}

// Forwards the arg through the sub-object's vtable+0xA8 result to its
// Forwards the arg through the sub-object's vtable+0xA8 result to its
// vtable+0x44 slot (retail symbol is Fv even though the body reads r4).
u32 CfObject_UnkVirtualFunc55__Q22cf13CfObjectModelFv(const cf::CfObjectModel* self, int arg) {
    cf::CfObjectModelSub98* sub = self->mSubObj98;
    if (sub == 0) {
        return 0;
    }
    return reinterpret_cast<cf::CfObjectParamVt44*>(reinterpret_cast<cf::CfObjectModelSub98Vt*>(sub)->mA8())->m44(arg);
}

// Returns this+0x10 when the vtable+0x44 flag is set, else the vtable+0x180
// (CfObjectModel_UnkVirtualFunc3) result.
void* cf::CfObjectModel::CObjectParam_UnkVirtualFunc2() {
    if (CObjectParam_UnkVirtualFunc3() != 0) {
        return reinterpret_cast<void*>(&mPtr10);
    }
    return CfObjectModel_UnkVirtualFunc3();
}

// Return the sub-object's vtable+0x18 result, or a static null placeholder
// when there is no sub-object (retail tail-calls the sub-object's slot).
void* cf::CfObjectModel::CfObjectModel_UnkVirtualFunc3() {
    if (mSubObj98 != 0) {
        return reinterpret_cast<CfObjectModelSub98Vt18*>(mSubObj98)->m18();
    }
    return lbl_eu_804FC548;
}

// If the sub-object's +0x7A4 bit-1 flag differs from the requested flag,
// forward (sub, flag) to the model visibility helper as a tail call.
void func_800BB618(cf::CfObjectModel* self, u32 flag) {
    cf::CfObjectModelSub98* sub = self->mSubObj98;
    if (sub != 0 && ((sub->field_7A4 >> 1) & 1) != flag) {
        func_804838DC(sub, flag);
    }
}

// Sets/clears the +0x68 0x00100000 flag, then forwards (sub, flag) to the
// model visibility helper as a tail call when the sub-object's +0x7A4 bit-1
// flag differs (retail symbol is Fv even though the body reads r4).
void CfObject_UnkVirtualFunc66__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, int flag) {
    if (flag != 0) {
        self->mFlags68 |= 0x00100000;
    } else {
        self->mFlags68 &= ~0x00100000;
    }
    cf::CfObjectModelSub98* sub = self->mSubObj98;
    if (sub != 0 && ((sub->field_7A4 >> 1) & 1) != flag) {
        func_804838DC(sub, flag);
    }
}

// Sets/clears the +0x68 0x00400000 flag, then, when a sub-object exists and
// its +0x7A4 bit-1 flag differs from (flag && bit-20 of the updated +0x68
// word), forwards (sub, cond) to the model visibility helper as a tail call
// (retail symbol is Fv even though the body reads r4). bit20 is extracted
// unconditionally (retail computes it before the sub-object null check), and
// cond is materialized twice (once for the compare, once for the call arg).
void CfObject_UnkVirtualFunc67__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, int flag) {
    if (flag != 0) {
        self->mFlags68 |= 0x00400000;
    } else {
        self->mFlags68 &= ~0x00400000;
    }
    int bit20 = (self->mFlags68 >> 20) & 1;
    cf::CfObjectModelSub98* sub = self->mSubObj98;
    if (sub != 0 && ((sub->field_7A4 >> 1) & 1) != ((flag != 0) && (bit20 != 0))) {
        func_804838DC(sub, (flag != 0) && (bit20 != 0));
    }
}

// Store the value at +0xA0, then forward it (as a tail call) to the
// sub-object's vtable slot +0x48 when a sub-object is present. The retail
// symbol is Fv (no params) even though the function reads f1, so the exact
// mangled name is written verbatim (MWCC emits names containing __ as-is).
void CfObject_UnkVirtualFunc70__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, float value) {
    cf::CfObjectModelSub98* sub = self->mSubObj98;
    self->field_A0 = value;
    if (sub == 0) {
        return;
    }
    reinterpret_cast<cf::CfObjectModelSub98Vt*>(sub)->CObjectParam_UnkVirtualFunc4();
}

void CfObject_UnkVirtualFunc72__Q22cf13CfObjectModelFv(void* self, float val) { *(float*)((u8*)self + 0xa4) = val; }

void CfObjectModel_UnkVirtualFunc10__Q22cf13CfObjectModelFv(void* self, float val) { *(float*)((u8*)self + 0xa8) = val; }

void CfObjectModel_UnkVirtualFunc12__Q22cf13CfObjectModelFv(void* self, float val) { *(float*)((u8*)self + 0xac) = val; }

// Retail symbol is Fv (no params) but the body reads r4 and forwards it to
// the sub-object's vtable slot +0x6C; written with the verbatim mangled
// name. The sub-object is read fresh from +0x98 for each call (retail
// reloads it after the func_80484E10 call).
void CfObjectModel_UnkVirtualFunc13__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, u32 arg) {
    if (self->mSubObj98 != 0) {
        func_80484E10(self->mSubObj98, 1, self->field_BD);
        reinterpret_cast<cf::CfObjectModelSub98Vt*>(self->mSubObj98)->m6C(arg);
    }
}

// Retail symbol is Fv (no params) but the body reads r4/r5; written with the
// verbatim mangled name. Copies another object's named node position into
// this object via the sub-object, remembering the source and name.
void CfObjectModel_UnkVirtualFunc14__Q22cf13CfObjectModelFv(
    cf::CfObjectModel* self, cf::CfObjectModel* other, const char* name) {
    if (self->field_BC == 0) {
        self->field_B4 = 0;
        self->field_B8 = 0;
        self->field_BD = 1;
    }
    if (self->mSubObj98 == 0) {
        return;
    }
    if (other != 0) {
        if (other->mSubObj98 == 0 || name == 0) {
            return;
        }

        // Resolve the name through the source object; fall back to the retail
        // placeholder label (+1) when the lookup reports failure.
        const char* target = name;
        if (reinterpret_cast<cf::CfObjectVt120*>(other)->m120(name) == 0) {
            target = reinterpret_cast<const char*>(&lbl_eu_804FC548[1]);
        }

        ml::CVec3 pos;
        cf::CfObjectModelSub98* srcSub = other->mSubObj98;
        if (srcSub != 0) {
            float* mtx = reinterpret_cast<cf::CfObjectParamVt3C*>(srcSub)->m3C(target);
            pos = ml::CVec3(mtx[3], mtx[7], mtx[11]);
        } else {
            pos = zero__Q22ml5CVec3;
        }
        self->CfObjectModel_UnkVirtualFunc13(&pos);
        self->field_B4 = other;
        self->field_B8 = target;
        return;
    }
    // Release the current binding when inactive.
    if (self->field_BC == 0) {
        func_80484E10(self->mSubObj98, 0, 1);
    }
}

extern "C" void CfObjectModel_UnkVirtualFunc15__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, int val) {
    // Store (val != 0) at field_BC, then tail-call the virtual
    // CfObjectModel_UnkVirtualFunc14 (vtable+0x1AC). val stays in r4 for the
    // callee (the retail Fv annotation omits the parameter).
    self->field_BC = (u8)(((u32)(-val) | (u32)(val)) >> 31);
    self->CfObjectModel_UnkVirtualFunc14();
}

extern "C" u32 func_800BB934(cf::CfObjectModel* self) {
    // Return bit 0 of the sub-object's +0x7A8 flag word (0 when no sub-object).
    cf::CfObjectModelSub98* sub = self->mSubObj98;
    if (sub != 0) {
        return sub->field_7A8 & 1;
    }
    return 0;
}

extern "C" u32 CfObject_UnkVirtualFunc69__Q22cf13CfObjectModelFv(cf::CfObjectModel* self) {
    // Return bit 1 of the sub-object's +0x7A4 flag word (0 when no sub-object).
    cf::CfObjectModelSub98* sub = self->mSubObj98;
    if (sub != 0) {
        return (sub->field_7A4 >> 1) & 1;
    }
    return 0;
}

// Returns 1 when a sub-object exists with its +0x7A4 bit-1 set and the
// vtable +0x16C result is below the low constant while the vtable +0x174
// result is above the high constant (0 otherwise). The vtable slots return
// floats in retail though the base header declares them void, so they are
// reached through the CfObjectVt174 proxy. The explicit reset inside the
// sub-object check reproduces the retail's redundant second li r31,0.
int CfObject_UnkVirtualFunc68__Q22cf13CfObjectModelFv(cf::CfObjectModel* self) {
    int result = 0;
    cf::CfObjectModelSub98* sub = self->mSubObj98;
    if (sub != 0) {
        result = 0;
        if ((sub->field_7A4 & 0x2) != 0) {
            if (reinterpret_cast<cf::CfObjectVt174*>(self)->m16C() < lbl_eu_80666A6C &&
                reinterpret_cast<cf::CfObjectVt174*>(self)->m174() > lbl_eu_80666A68) {
                result = 1;
            }
        }
    }
    return result;
}

// Refreshes the sub-object's vtable +0x98 slot, then fills a stack vector
// with the global position constant and forwards it (with 0 flags) to the
// sub-object's vtable +0x90 slot. mSubObj98 is read fresh for each call
// (retail reloads +0x98 after the first bctrl).
void func_800BBA08(cf::CfObjectModel* self) {
    if (self->mSubObj98 != 0) {
        reinterpret_cast<cf::CfObjectModelSub98Vt*>(self->mSubObj98)->m98();
        float vec[3];
        vec[0] = lbl_eu_80666A6C;
        vec[1] = lbl_eu_80666A6C;
        vec[2] = lbl_eu_80666A6C;
        reinterpret_cast<cf::CfObjectModelSub98Vt*>(self->mSubObj98)->m90(vec, 0);
    }
}

// Tail-call the sub-object's vtable slot 0x8C (CfObject_UnkVirtualFunc15
// in the base vtable layout); the 2nd argument rides along untouched in r4.
void func_800BBA7C(cf::CfObjectModel* self, f32* vec) {
    if (self->mSubObj98 != 0) {
        ((cf::CfObject*)self->mSubObj98)->CfObject_UnkVirtualFunc15();
    }
}

// Return bit 4 of the sub-object's +0x7A4 flag word (0 when no sub-object).
u32 cf::CfObjectModel::CfObjectModel_UnkVirtualFunc8() {
    if (mSubObj98 != 0) {
        return (mSubObj98->field_7A4 >> 4) & 1;
    }
    return 0;
}

// Tail-call the sub-object's vtable slot 0xB4 (CfObject_UnkVirtualFunc25
// in the base vtable layout) when a sub-object is present.
void cf::CfObjectModel::CfObjectModel_UnkVirtualFunc7() {
    if (mSubObj98 != 0) {
        ((cf::CfObject*)mSubObj98)->CfObject_UnkVirtualFunc25();
    }
}

// Installs a new sub-object, then reattaches the old +0xC0 target through
// the effect-manager helper when both exist (the helper receives the new
// sub-object's vtable +0xA8 value), and finally clears +0xC0. The inner
// arg != 0 re-check is redundant but present in retail (MWCC reuses the
// cr1 flag from the entry compare). The +0xC0 word sits past the header's
// declared CfObjectModel end (0xBD), so it is accessed through a local
// overlay struct (CfObjectMap/CfObjectMove derive from CfObjectModel, so
// the shared class layout must not grow).
void func_800BBADC(cf::CfObjectModel* self, cf::CfObjectModelSub98* arg) {
    struct ModelC0 {
        u8 _pad00[0xC0];
        void* field_C0;  // 0xC0
    };
    ModelC0* obj = reinterpret_cast<ModelC0*>(self);
    self->mSubObj98 = arg;
    if (arg != 0 && obj->field_C0 != 0) {
        u32 v = (arg != 0) ? reinterpret_cast<cf::CfObjectModelSub98Vt*>(arg)->mA8() : 0;
        reattachTrg__(obj->field_C0, reinterpret_cast<void*>(v));
    }
    obj->field_C0 = 0;
}

// When the sub-object's vtable +0xA8 value is non-zero and the +0xC0 target
// word is empty, re-query the +0xA8 value (sub-object read fresh), install it
// at +0xC0 and notify the effect reattachment helper. The `c0 == v` re-check
// is dead (c0 is 0 there) but present in retail.
void func_800BBB50(cf::CfObjectModel* self) {
    struct ModelC0 {
        u8 _pad00[0xC0];
        void* field_C0;  // 0xC0
    };
    ModelC0* obj = reinterpret_cast<ModelC0*>(self);
    cf::CfObjectModelSub98* sub = self->mSubObj98;
    u32 v = (sub != 0) ? reinterpret_cast<cf::CfObjectModelSub98Vt*>(sub)->mA8() : 0;
    if (v != 0) {
        u32 c0 = reinterpret_cast<u32>(obj->field_C0);
        if (c0 == 0 && c0 != v) {
            cf::CfObjectModelSub98* sub2 = self->mSubObj98;
            u32 v2 = (sub2 != 0) ? reinterpret_cast<cf::CfObjectModelSub98Vt*>(sub2)->mA8() : 0;
            obj->field_C0 = reinterpret_cast<void*>(v2);
            detachTrgPrt_(reinterpret_cast<u8*>(v2));
        }
    }
}

int CfObjectModel_UnkVirtualFunc6__Q22cf13CfObjectModelFv(void* self) { return 0; }

// CfObject.hpp declares this with (float, float) params but the retail symbol is Fv (no params),
// so we emit the exact mangled name via extern "C" to match the retail .o symbol table.
extern "C" void CfObject_UnkVirtualFunc20__Q22cf8CfObjectFv(cf::CfObject* self, float a, float b) {
    // Open item (MWCC_CASES pair-copy register allocation, func_800BC510):
    // MWCC emits stores in FPR order c,a,b; retail stores a,b,c (constant
    // last). Invariant to source store order / locals / volatile / const /
    // pointer shapes; -ipa off breaks the leaf shape; GC/3.0a5.2 fixes the
    // order but the TU requires Wii/1.1 (never_inline attr, 22 byte-identical
    // siblings). Witness rejects (store offsets are non-register bits).
    float c = lbl_eu_80666A68;
    self->mPos3C = a;
    self->mPos44 = b;
    self->mPos40 = c;
}

void func_800BBC04() {
    func_80142428();
}

const char* func_800BBC08(u16 index) {
    return cf::CfBdat::getBdatStringEntry(index);
}

// Return the object when any of the +0x64 flag bits (2, 4, 8, 0x80, 0x100)
// is set, else 0 (retail reads cf::CfObject::unk64). The null check and the
// last flag check share the retail return-0 tail (both beq the same block).
// Return the object when any of the +0x64 flag bits (2, 4, 8, 0x80, 0x100)
// is set, else 0 (retail reads cf::CfObject::unk64). Goto-gate shape: the
// null check and the last flag test both branch to the shared return-0 tail
// (retail: two beq to the same block; plain early returns give bclr/duplicated
// tails).
cf::CfObject* func_800BBC0C(cf::CfObject* self) {
    if (self == 0) goto ret0;
    u32 flags = self->unk64;
    if ((flags & 0x2) != 0) goto found;
    if ((flags & 0x4) != 0) goto found;
    if ((flags & 0x8) != 0) goto found;
    if ((flags & 0x80) != 0) goto found;
    if ((flags & 0x100) != 0) goto found;
    goto ret0;
found:
    return self;
ret0:
    return 0;
}
