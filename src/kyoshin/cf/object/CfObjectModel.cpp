// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectModel
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include "kyoshin/cf/object/CfObjectModel.hpp"
#include "libs/monolib/src/scn/CScnItemModel.hpp"
#include "kyoshin/cf/CfBdat.hpp"
#include "monolib/math/CAttrTransform.hpp"
#include "kyoshin/action/CActParamAnim.hpp"
#include <nw4r/g3d/res/g3d_resmdl.h>
#include <nw4r/db/db_assert.h>
#include "kyoshin/cf/CfResTboxImpl.hpp"  // complete type for delete below
#include "kyoshin/code_801862C0.hpp"

namespace {
// (The former SubObjB0 destroy-view is gone: the dtor deletes the +0xB0
// word through the real owner CfResTboxImpl, whose slot +0x08 is the
// deleting destructor. Both dtor cleanup calls go through the real
// CfObject_UnkVirtualFunc6 virtual (slot +0x68).)
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
    // NOTE (CObjectParam::field_30): the shared header declares field_30
    // right after unk14, landing at +0x20, but retail reads that member at
    // +0x30 (cf. CtrlObjectParam's CObjectParamRetailView: pad[0x30] then
    // field_30). So the +0x30 word here is the unnamed CfObject gap word,
    // NOT the header's field_30; retail zeroes it (stw 48) and never writes
    // +0x20 in this ctor. Fixing the header layout belongs to the owning
    // wave (it shifts every CfObject-family class); this TU writes the
    // retail offsets directly.
    struct Gap30 { u8 _p00[0x30]; u32 w30; };          // 0x30 (unnamed gap word)
    struct Gap34 { u8 _p00[0x34]; u32 w34; };          // 0x34
    struct Gap10 { u8 _p00[0x10]; u8 b10; };           // 0x10 (retail zeroes only the byte)
    struct Gap48 { u8 _p00[0x48]; float f48; };        // 0x48
    struct Gap50 { u8 _p00[0x50]; float f50; };        // 0x50
    struct Sub54Flt { float a, b, c; };                // 0x54-0x5F
    struct Gap6C { u8 _p00[0x6C]; u32 w6c; u32 w70; u32 w74; u8 b78; };  // 0x6C-0x78
    struct Gap88 { u8 _p00[0x88]; u32 w88; u16 h8c; u16 h8e; };           // 0x88-0x8F
    struct Tail90 {                                    // 0x90-0xC3
        // The old struct missed this prefix: every phase-2 store landed
        // 0x90 too low (clobbering the just-built base) and the +0x00 base
        // vtable store was dead-stripped to a zero store.
        u8 _pad00[0x90];                               // 0x00-0x8F
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
    Gap30* g30 = reinterpret_cast<Gap30*>(this);
    Gap34* g34 = reinterpret_cast<Gap34*>(this);
    Gap48* g48 = reinterpret_cast<Gap48*>(this);
    Gap50* g50 = reinterpret_cast<Gap50*>(this);
    Gap6C* g6c = reinterpret_cast<Gap6C*>(this);
    Gap88* g88 = reinterpret_cast<Gap88*>(this);
    Tail90* t90 = reinterpret_cast<Tail90*>(this);
    // NOTE: do NOT hoist the two pool constants into float locals here.
    // A phase-1 scoped {one, zero} pair makes MWCC synthesize its generated
    // __vt__Q22cf13CfObjectModel materialize (+12 bytes + UNDEF link break)
    // for a vtable this TU never emits. Direct pool refs keep f0 (retail
    // uses f1 for the zero group: pure reg-swap residual, size-neutral).
    unk4 = 0;
    unk8 = 0;
    unkC = 0;
    g10->b10 = 0;  // retail stores only the byte at 0x10
    g30->w30 = 0;  // +0x30 gap word (retail stw 48)
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
    // NOTE: real virtual dispatch through the CfObject base (slot +0x5C =
    // CfObject_UnkVirtualFunc3). The manual vtable store above dominates,
    // so MWCC reuses r12 for the slot load exactly like retail (no implicits
    // materialize: the TU never emits a compiler __vt__ for this class).
    static_cast<cf::CfObject*>(this)->CfObject_syncEnableState();  // vtable +0x5C
    // Phase 2: flag bit, base vtable install, model-tail init. Fresh scoped
    // constants (retail reloads both pools after the call: lfs f0 then lfs
    // f1); the flags word is hoisted (retail loads it into r0 right after
    // the call, well before the oris). The vtable store stays inline: a
    // named local gets LICM-hoisted into the prologue.
    u32 saveFlags = mFlags68;
    mFlags68 = saveFlags | 0x00100000;
    *(void**)this = (void*)lbl_eu_80529318;
    t90->w90 = 0;
    t90->w94 = 0;
    t90->sub98 = 0;
    t90->w9c = 0;
    // Phase-2-only constants (defined after the call, so they cannot go
    // nonvolatile; they let MWCC reuse one load per pool across the four
    // stores instead of reloading per use).
    float one2 = lbl_eu_80666A6C;
    float zero2 = lbl_eu_80666A68;
    t90->fa0 = zero2;
    t90->fa4[0] = one2;
    t90->fa4[1] = one2;
    t90->fa4[2] = one2;
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
    self->CfObject_notifyEventDone();  // vtable +0x70
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
    u32 flags;
    if (self == 0) {
        goto finish;
    }
    flags = self->mFlags68;
        *(void**)self = (void*)lbl_eu_80529318;
        if (flags & 0x40000000) {
            func_80186474(func_801862C0(), self);
        }
        // Real virtual dispatch through the installed CfObject vtable
        // (slot +0x68 = CfObject_releaseMoveTargets, the Wave-39 alias for
        // CfObject_UnkVirtualFunc6). The static_cast picks
        // the CfObject virtual (CfObjectModel hides the name with a
        // non-virtual member); no-arg call emits no r4 setup, matching
        // retail's residue-r4 bctrl.
        static_cast<cf::CfObject*>(self)->CfObject_releaseMoveTargets();
        if (self->mSubObjB0 != 0) {
            // The +0xB0 resource is a CfRes-family impl (vptr at +0x10,
            // deleting dtor at slot +0x08); delete emits the retail li r4,1
            // + virtual dispatch, and its null guard is retail's second beq
            // (single cmpwi, two beqs) - cf. CfObjectTbox ctor, MWCC_PATTERNS.
            delete reinterpret_cast<cf::CfResTboxImpl*>(self->mSubObjB0);
            self->mSubObjB0 = 0;
        }
        if (self != 0) {
            *(void**)self = (void*)lbl_eu_805294E0;
            // Same slot through the reinstalled model vtable: dispatches
            // CfObjectModel_teardownModel__Q22cf13CfObjectModelFv.
            static_cast<cf::CfObject*>(self)->CfObject_releaseMoveTargets();
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
void cf::CfObjectModel::CfObjectModel_teardownModel() {
    notifyDetach_(this);
    if (mSubObj38 != 0) {
        // +0xAC with no args, return discarded = CfObject_getPosVector
        // (us-800BB3A8: lwz r12,0xAC(r12)/bctrl, r3 unused after).
        reinterpret_cast<cf::CfObject*>(mSubObj38)->CfObject_getPosVector();
        if (mSubObj38 != 0) {
            // Redundant nested check on the reloaded value mirrors retail's
            // two beq targets (MWCC keeps both branches).
            if (mSubObj38 != 0) {
                // +0x08 with r4=1 = CObjectState_setStateBitMask
                // (us-800BB3C8: li r4,1 / lwz r12,0x08(r12) / bctrl).
                reinterpret_cast<cf::CfObject*>(mSubObj38)->CObjectState_setStateBitMask(1);
            }
            mSubObj38 = 0;
        }
    }
    CfObjectModel_releaseModelList();  // vtable +0x17C
    CfObjectModel_releaseModelSub();  // vtable +0x178
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

// Non-virtual out-of-line body: retail Fv linker name for hand table +0x178
// and Move's Class::CfObjectModel_UnkVirtualFunc1 base call. The C++ virtual
// at that slot is releaseModelSub (novtable; dispatch is positional).
void cf::CfObjectModel::CfObjectModel_UnkVirtualFunc1() {
    if (mSubObj98 != 0) {
        func_80495E60(mSubObj98);
        mSubObj98 = 0;
    }
}

void CfObjectModel_releaseModelList__Q22cf13CfObjectModelFv() {}

void func_800BAB64(cf::CfObjectModel* self) {
    self->CfObjectModel_releaseModelList();  // vtable +0x17C
    self->CfObjectModel_releaseModelSub();  // vtable +0x178
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
void CfObject_notifyEventDone__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, void* newObj) {
    if (self->mSubObj38 != 0) {
        // +0xAC with no args, return discarded = CfObject_getPosVector (same as UVF6 above).
        reinterpret_cast<cf::CfObject*>(self->mSubObj38)->CfObject_getPosVector();
    }
    if (self->mSubObj38 != 0) {
        // Redundant re-check on the reloaded pointer: MWCC shares the compare
        // between the two tests and emits two beq targets (skip-call /
        // skip-block), matching retail.
        if (self->mSubObj38 != 0) {
            // +0x08 with r4=1 = CObjectState_setStateBitMask (same as UVF6 above).
            reinterpret_cast<cf::CfObject*>(self->mSubObj38)->CObjectState_setStateBitMask(1);
        }
        self->mSubObj38 = 0;
    }
    self->mSubObj38 = newObj;
    if (newObj != 0) {
        // +0x2C with r4=self, r5 untouched = CScnItemModel::attachModelParent
        // (Nw4r scnImN4GetHidByNm; us-800BB550: mr r3,newObj / mr r4,self /
        // lwz r12,0x2C(r12)/bctrl). A CObjectState_setStateBitMask0(self)
        // spelling does NOT compile: the (int,int) decl rejects a 1-arg call
        // (MWCC 10248) and a 2-arg call emits an r5 setup with zero split
        // room. The (u32) cast is free.
        reinterpret_cast<CScnItemModel*>(newObj)->attachModelParent((u32)self);
        // +0xA0 with no FP setup = CScnItemModel::notifyModelAttached
        // (Nw4r scnImN4ResetAnims; us-800BB568: lwz r3,0x38 / lwz r12,0xA0(r12)
        // / bctrl). The CfObject-family slot here (UVF20(float,float)) would
        // emit f1/f2 setups with zero split room, so the arity-exact scene
        // slot is used.
        reinterpret_cast<CScnItemModel*>(self->mSubObj38)->notifyModelAttached();
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
    void* p = self->CfObjectModel_getModelName();  // vtable +0x180
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

// Call the cf-chain vtable slot +0x14C (retail CfObject_readKindFlagWord
// returns a flag word) and return bit 1 of it.
u32 func_800BAD98(cf::CfObject* obj) {
    return (obj->CfObject_readKindFlagWord() >> 1) & 1;
}

// Same as func_800BAD98 but returns bit 2 of the flag word.
u32 func_800BADC8(cf::CfObject* obj) {
    return (obj->CfObject_readKindFlagWord() >> 2) & 1;
}

// Same as func_800BAD98 but returns bit 3 of the flag word.
u32 func_800BADF8(cf::CfObject* obj) {
    return (obj->CfObject_readKindFlagWord() >> 3) & 1;
}

// Call the cf-chain vtable slot +0x14C and return bit 7 of the flag word.
u32 func_800BAE28(cf::CfObject* obj) {
    return (obj->CfObject_readKindFlagWord() >> 7) & 1;
}

// Copies the vector into the sub-object transform's position and refreshes
// it, then mirrors the vector into this object's +0x3C position (retail
// symbol is Fv even though the body reads r4).
void CfObject_setMoveTargetVec__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, const ml::CVec3* vec) {
    if (self->mSubObj98 != 0) {
        ml::CAttrTransform* transform = static_cast<ml::CAttrTransform*>(simGetLeafActData(self->mSubObj98));
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
        ml::CAttrTransform* transform = static_cast<ml::CAttrTransform*>(simGetLeafActData(self->mSubObj98));
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
    // Filter word: default mid variant; bit30 SET picks the narrow one
    // (retail pairs 0x4a05 with the taken path: the beq at +0x38 falls
    // through to the li, and the final beq takes the if-path for the same
    // bit). Bit29 (when bit30 clear) picks the wide one. Each unk64 bit
    // test is normalized through the double-cntlzw booleanize idiom (retail
    // booleanizes twice).
    u32 filter = 0x44a09;
    if ((((u32)__cntlzw((u32)__cntlzw(flags & 0x2) >> 5)) >> 5) != 0) {
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
        // Same call shape with the selected filter (retail reuses the filter
        // word here: r5 is not reloaded on this path, so the else probe uses
        // the default 0x44a09, not a fresh 0x4a05 immediate).
        func_800A7094(pos, &probe, filter, scale, lbl_eu_80666A68);
    }
    self->CfObject_setMoveTargetVec(&probe);
}

// Retail symbol is Fv (no params) but the body consumes two floats in f1/f2 -
// forced-name form; packs them with a constant into a stack vector and calls
// the cf-chain vtable slot +0xB4 (CfObject_snapMoveTarget, hidden args).
void CfObject_UnkVirtualFunc20__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, float a, float b) {
    float arr[3];
    arr[1] = lbl_eu_80666A80;
    arr[0] = a;
    arr[2] = b;
    self->CfObject_snapMoveTarget(reinterpret_cast<ml::CVec3*>(arr), lbl_eu_80666A84);
}

// Return a pointer-typed word: the sub-object's derived value +0xB8, or
// this +0x3C when there is no sub-object.
ml::CVec3* cf::CfObjectModel::CfObject_getPosVector() {
    if (mSubObj98 != 0) {
        return reinterpret_cast<ml::CVec3*>(reinterpret_cast<uintptr_t>(simGetLeafActData(mSubObj98)) + 0xB8);
    }
    return reinterpret_cast<ml::CVec3*>(reinterpret_cast<uintptr_t>(this) + 0x3C);
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
void CfObject_setModelRotVec__Q22cf13CfObjectModelFPv(cf::CfObjectModel* self, const ml::CVec3* vec) {
    if (self->mSubObj98 != 0) {
        ml::CAttrTransform* transform = static_cast<ml::CAttrTransform*>(simGetLeafActData(self->mSubObj98));
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
        return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(simGetLeafActData(ptr)) + 0xC4);
    } else {
        return data->field_48;
    }
}

// Stores the value into the sub-object transform's rotation-Y and refreshes
// it, then stores it at +0x4C (retail symbol is Fv even though the body
// reads f1).
void CfObject_setMoveHeadAngle__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, float value) {
    if (self->mSubObj98 != 0) {
        ml::CAttrTransform* transform = static_cast<ml::CAttrTransform*>(simGetLeafActData(self->mSubObj98));
        transform->mRot.y = value;
        transform->update();
    }
    self->mField4C = value;
}

float CfObject_getMoveHeadAngle__Q22cf13CfObjectModelFv(void* self) {
    void* ptr = *(void**)((unsigned char*)self + 0x98);
    if (ptr != 0) {
        void* result = simGetLeafActData(ptr);
        return *(float*)((unsigned char*)result + 0xc8);
    } else {
        return *(float*)((unsigned char*)self + 0x4c);
    }
}

extern "C" void CfObject_setScaledHeadAngle__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, float f) {
    // Scale f by the global factor, then tail-call the virtual
    // CfObject_setMoveHeadAngle (vtable+0xC4) with the scaled value. The
    // upcast to CfObject resolves the base (float) overload that CfObjectModel
    // hides with its no-arg override.
    ((cf::CfObject*)self)->CfObject_setMoveHeadAngle(f * lbl_eu_8066A210);
}

// Scale the CfObject_getMoveHeadAngle result by the global scale factor.
float cf::CfObjectModel::CfObject_UnkVirtualFunc34() {
    float value = CfObject_getMoveHeadAngle();
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
        // its value; Nw4r owner is scnImN4GetEffAct (getEffectActOwner alias).
        return reinterpret_cast<CScnItemModel*>(sub)->getEffectActOwner();
    }
    return 0;
}

// Height difference between the two positions the sub-object helper computes;
// a fixed fallback constant when there is no sub-object.
// const self (forced-name free function): MWCC hoists the mSubObj98 load
// (lwz r3,0x98(r3)) above the LR save (stw r0,0x24(r1)) — the load-hoist
// family lever (CfObjectMove_queryNpcAdvance / CScnEffectActNw4r getters);
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

// Forwards the arg through the sub-object's vtable+0xA8 result to the
// effect-act id-table lookup at +0x44 (retail symbol is Fv even though the
// body reads r4). The +0xA8 call takes no args: on the Nw4r scene model it
// dispatches scnImN4GetEffAct, which ignores r4 and returns the embedded
// CScnEffectActNw4r (+0x14C4); retail keeps r4 live across that bctrl and
// restores it into the +0x44 bctrl, whose callee is func_8049C18C
// (CScnEffectAct* (u32 idx)). Both dispatches go through the real owning
// classes - no proxy.
u32 CfObject_UnkVirtualFunc55__Q22cf13CfObjectModelFv(const cf::CfObjectModel* self, int arg) {
    cf::CfObjectModelSub98* sub = self->mSubObj98;
    if (sub == 0) {
        return 0;
    }
    CScnEffectActNw4r* acts = reinterpret_cast<CScnEffectActNw4r*>(
        reinterpret_cast<CScnItemModel*>(sub)->getEffectActOwner());
    return (u32)acts->func_8049C18C((u32)arg);
}

// Returns this+0x10 when the vtable+0x44 flag is set, else the vtable+0x180
// (CfObjectModel_getModelName) result.
void* cf::CfObjectModel::CObjectParam_getActiveParam() {
    if (CObjectParam_hasObjectName() != 0) {
        return reinterpret_cast<void*>(&mPtr10);
    }
    return CfObjectModel_getModelName();
}

// Return the sub-object's vtable+0x18 result, or a static null placeholder
// when there is no sub-object (retail tail-calls the sub-object's slot).
extern "C" void* CfObjectModel_UnkVirtualFunc3__Q22cf13CfObjectModelFv(cf::CfObjectModel* self) {
    if (self->mSubObj98 != 0) {
        return (void*)reinterpret_cast<CScnItemModel*>(self->mSubObj98)->getModelNameStr();
    }
    return lbl_eu_804FC548;
}

// If the sub-object's +0x7A4 bit-1 flag differs from the requested flag,
// forward (sub, flag) to the model visibility helper as a tail call.
void func_800BB618(cf::CfObjectModel* self, u32 flag) {
    cf::CfObjectModelSub98* sub = self->mSubObj98;
    if (sub != 0 && ((sub->field_7A4 >> 1) & 1) != flag) {
        simSetFlag2OnTree(sub, flag);
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
        simSetFlag2OnTree(sub, flag);
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
        simSetFlag2OnTree(sub, (flag != 0) && (bit20 != 0));
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
    reinterpret_cast<CScnItemModel*>(sub)->setModelFloatRate(value);
}

void CfObject_UnkVirtualFunc72__Q22cf13CfObjectModelFv(void* self, float val) { *(float*)((u8*)self + 0xa4) = val; }

void CfObjectModel_UnkVirtualFunc10__Q22cf13CfObjectModelFv(void* self, float val) { *(float*)((u8*)self + 0xa8) = val; }

void CfObjectModel_UnkVirtualFunc12__Q22cf13CfObjectModelFv(void* self, float val) { *(float*)((u8*)self + 0xac) = val; }

// Retail symbol is Fv (no params) but the body reads r4 and forwards it to
// the sub-object's vtable slot +0x6C; written with the verbatim mangled
// name. The sub-object is read fresh from +0x98 for each call (retail
// reloads it after the simSetFlags7A8_12 call).
void CfObjectModel_UnkVirtualFunc13__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, u32 arg) {
    if (self->mSubObj98 != 0) {
        simSetFlags7A8_12(self->mSubObj98, 1, self->field_BD);
        reinterpret_cast<CScnItemModel*>(self->mSubObj98)->setModelFlag6C(arg);
    }
}

// Retail symbol is Fv (no params) but the body reads r4/r5; written with the
// verbatim mangled name. Copies another object's named node position into
// this object via the sub-object, remembering the source and name.
void CfObjectModel_UnkVirtualFunc14__Q22cf13CfObjectModelFv(
    cf::CfObjectModel* self, cf::CfObjectModel* other, const char* name) {
    // Hoisted: retail caches the name in r31 first (prologue or r31,r5),
    // then other in r30 and self in r29.
    const char* target = name;
    if (self->field_BC == 0) {
        self->field_B4 = 0;
        self->field_B8 = 0;
        self->field_BD = 1;
    }
    if (self->mSubObj98 == 0) {
        return;
    }
    if (other != 0) {
        // Two separate null checks (retail beq's straight to the epilogue
        // for each; an || pair emits an extra branch).
        if (other->mSubObj98 == 0) {
            return;
        }
        if (name == 0) {
            return;
        }

        // Resolve the name through the source object; fall back to the retail
        // placeholder label (+1) when the lookup reports failure. The lookup
        // takes the cached target (r31), not the incoming r5.
        if (static_cast<cf::CfObject*>(other)->CfObject_findNodeMatrix(target) == nullptr) {
            target = reinterpret_cast<const char*>(&lbl_eu_804FC548[1]);
        }

        ml::CVec3 pos;
        cf::CfObjectModelSub98* srcSub = other->mSubObj98;
        if (srcSub != 0) {
            float* mtx = (float*)reinterpret_cast<CScnItemModel*>(srcSub)->getNamedNodeMtx(target);
            pos = ml::CVec3(mtx[3], mtx[7], mtx[11]);
        } else {
            pos = zero__Q22ml5CVec3;
        }
        self->CfObjectModel_aimAtPosition(&pos);
        self->field_B4 = other;
        self->field_B8 = target;
        return;
    }
    // Release the current binding when inactive.
    if (self->field_BC == 0) {
        simSetFlags7A8_12(self->mSubObj98, 0, 1);
    }
}

extern "C" void CfObjectModel_UnkVirtualFunc15__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, cf::CfObject* other, const char* name) {
    // Store (other != 0) at field_BC, then tail-call the virtual
    // CfObjectModel_bindModelTo (vtable+0x1AC; UnkVirtualFunc14 is the Fv alias)
    // with the same args.
    self->field_BC = (u8)(other != nullptr);
    self->CfObjectModel_bindModelTo(other, name);
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
// reached through the real CfObject_getProgressLow/High virtuals. The
// sub-object check reproduces the retail's redundant second li r31,0.
extern "C" int CfObject_UnkVirtualFunc68__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, const ml::CVec3* vec) {
    (void)vec;
    int result = 0;
    cf::CfObjectModelSub98* sub = self->mSubObj98;
    if (sub != 0) {
        result = 0;
        if ((sub->field_7A4 & 0x2) != 0) {
            if (self->CfObject_getProgressLow() < lbl_eu_80666A6C &&
                self->CfObject_getProgressHigh() > lbl_eu_80666A68) {
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
        reinterpret_cast<CScnItemModel*>(self->mSubObj98)->notifyModelReady();
        float vec[3];
        vec[0] = lbl_eu_80666A6C;
        vec[1] = lbl_eu_80666A6C;
        vec[2] = lbl_eu_80666A6C;
        reinterpret_cast<CScnItemModel*>(self->mSubObj98)->probeModelVec(vec, 0);
    }
}

// Tail-call the sub-object's vtable slot 0x8C (CfObject_getMoveSpeedRate
// in the base vtable layout); the 2nd argument rides along untouched in r4.
void func_800BBA7C(cf::CfObjectModel* self, f32* vec) {
    if (self->mSubObj98 != 0) {
        ((cf::CfObject*)self->mSubObj98)->CfObject_getMoveSpeedRate();
    }
}

// Return bit 4 of the sub-object's +0x7A4 flag word (0 when no sub-object).
// Free-function form keeps the Unk Fv linker name the hand-built vtables
// reference; the header virtual is CfObjectModel_getSubFlag04 (Wave-60).
extern "C" u32 CfObjectModel_UnkVirtualFunc8__Q22cf13CfObjectModelFv(cf::CfObjectModel* self) {
    if (self->mSubObj98 != 0) {
        return (self->mSubObj98->field_7A4 >> 4) & 1;
    }
    return 0;
}

// Forwards the caller's (pos, scale) to the sub-object's vtable slot +0xB4
// (CfObject_snapMoveTarget) as a tail call: retail leaves r4/f1 live with
// no setup (bctr, 0x20 bytes), so this slot really takes (pos, scale) even
// though the Fv linker name claims no params. Free-function form (same as
// CfObject_UnkVirtualFunc55/70/72 below): the vtable's bare-Fv reference
// resolves to this definition; the widened member decl in the header serves
// call-site codegen only.
void CfObjectModel_UnkVirtualFunc7__Q22cf13CfObjectModelFv(
    cf::CfObjectModel* self, ml::CVec3* pos, float scale) {
    if (self->mSubObj98 != 0) {
        // Sub-object is itself a CfObject; forward r4/f1 untouched.
        cf::CfObject* sub = reinterpret_cast<cf::CfObject*>(self->mSubObj98);
        sub->CfObject_snapMoveTarget(pos, scale);
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
        u32 v = (arg != 0) ? reinterpret_cast<CScnItemModel*>(arg)->getEffectActOwner() : 0;
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
    u32 v = (sub != 0) ? reinterpret_cast<CScnItemModel*>(sub)->getEffectActOwner() : 0;
    if (v != 0) {
        u32 c0 = reinterpret_cast<u32>(obj->field_C0);
        if (c0 == 0 && c0 != v) {
            cf::CfObjectModelSub98* sub2 = self->mSubObj98;
            u32 v2 = (sub2 != 0) ? reinterpret_cast<CScnItemModel*>(sub2)->getEffectActOwner() : 0;
            obj->field_C0 = reinterpret_cast<void*>(v2);
            detachTrgPrt_(reinterpret_cast<u8*>(v2));
        }
    }
}

void* CfObjectModel_checkTargetNode__Q22cf13CfObjectModelFv(void* self) { return 0; }

// CfObject.hpp declares this with (float, float) params but the retail symbol is Fv (no params),
// so we emit the exact mangled name via extern "C" to match the retail .o symbol table.
extern "C" void CfObject_setPosXZ__Q22cf8CfObjectFv(cf::CfObject* self, float a, float b) {
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

// --- typified .data (layout from build/us/asm/kyoshin/cf/object/CfObjectModel.s) ---
// Two full vtables + RTTI locator list + assert strings. Slots carrying
// relocs emit zero bytes + ADDR32 relocs in the .o, matching retail (the
// gate compares bytes; reloc names are ignored once bytes match). Targets
// owned by other TUs are extern (UNDEF here, resolved at link via the
// global symbol map). The two vtable bodies use stand-in names:
// CfObjectModel.hpp pins lbl_eu_80529318/805294E0 as u8[] for other TUs,
// so pointer tables cannot take those spellings in this TU; UNIT_RULES
// exact_renames maps them onto the retail labels for the link.
extern "C" {
void CObjectState_setStateBitMask__Q22cf12CObjectStateFv();
void CObjectState_checkStateFlags__Q22cf12CObjectStateFv();
void CObjectState_setStateBitFlag__Q22cf12CObjectStateFv();
void CObjectState_clearStateWord4__Q22cf12CObjectStateFv();
void CObjectState_applyStateFlags__Q22cf12CObjectStateFv();
void CObjectState_clearStateWord8__Q22cf12CObjectStateFv();
void CObjectState_clearStateFlags8__Q22cf12CObjectStateFv();
void CObjectState_UnkVirtualFunc8__Q22cf12CObjectStateFv();
void CObjectState_UnkVirtualFunc9__Q22cf12CObjectStateFv();
void CObjectState_setStateBitMask0__Q22cf12CObjectStateFv();
void CObjectState_getStateData__Q22cf12CObjectStateFv();
void CObjectState_setStateBitMask2__Q22cf12CObjectStateFv();
void CObjectState_setStateBitMask3__Q22cf12CObjectStateFv();
void CObjectParam_UnkVirtualFunc1__Q22cf12CObjectParamFv();
void CObjectParam_getActiveParam__Q22cf13CfObjectModelFv();
void CObjectParam_hasObjectName__Q22cf12CObjectParamFv();
void CObjectParam_UnkVirtualFunc4__Q22cf12CObjectParamFv();
void CObjectParam_getSelfObjectId__Q22cf12CObjectParamFv();
void CObjectParam_signalActionEnd__Q22cf12CObjectParamFv();
void __dt__Q22cf11CfObjectMapFv();
void checkTarget__Q22cf11CfObjectMapFv();
void clearStatus__Q22cf11CfObjectMapFv();
void dispatchMapState__Q22cf11CfObjectMapFv();
void cleanupMap__Q22cf11CfObjectMapFv();
void nopMap__Q22cf11CfObjectMapFv();
void isObjectMapReady__Q22cf11CfObjectMapFv();
void setMapScale__Q22cf11CfObjectMapFv();
void setMapVisibility__Q22cf11CfObjectMapFv();
void setMapId__Q22cf11CfObjectMapFv();
void setMapEffectFlag__Q22cf11CfObjectMapFv();
void __dt__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc3__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc5__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc6__Q22cf8CfObjectFv();
void CfObject_isMoveActiveNow__Q22cf8CfObjectFv();
void CfObject_forwardSubObject__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc11__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc12__Q22cf8CfObjectFv();
void CfObject_queryTargetState__Q22cf8CfObjectFv();
void CfObject_pushRefreshValue__Q22cf8CfObjectFf();
void CfObject_getMoveSpeedRate__Q22cf8CfObjectFv();
void CfObject_pushRefreshExtra__Q22cf8CfObjectFv();
void CfObject_readRefreshValue__Q22cf8CfObjectFv();
void CfObject_checkTargetState__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc21__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc26__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc37__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc38__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc39__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc40__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc42__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc43__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc45__Q22cf8CfObjectFv();
void CfObject_setMoveTargetPtr__Q22cf8CfObjectFv();
void CfObject_createMoveTarget__Q22cf8CfObjectFv();
void CfObject_getCurrentTarget__Q22cf8CfObjectFv();
void CfObject_forwardNpcAction__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc50__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc51__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc57__Q22cf8CfObjectFv();
void CfObject_getMoveRateScale__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc59__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc60__Q22cf8CfObjectFv();
void CfObject_setAnimSlotEntry__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc62__Q22cf8CfObjectFv();
void CfObject_setMoveBusyState__Q22cf8CfObjectFi();
void CfObject_setMoveReadyFlag__Q22cf8CfObjectFv();
void CObjectParam_UnkVirtualFunc2__Q22cf12CObjectParamFv();
void CfObject_setMoveTargetVec__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc22__Q22cf8CfObjectFv();
void CfObject_getPosVector__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc24__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc27__Q22cf8CfObjectFPv();
void CfObject_UnkVirtualFunc28__Q22cf8CfObjectFv();
void CfObject_setMoveHeadAngle__Q22cf8CfObjectFf();
void CfObject_UnkVirtualFunc30__Q22cf8CfObjectFv();
void CfObject_getMoveHeadAngle__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc32__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc33__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc34__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc35__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc36__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc52__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc53__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc54__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc55__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc56__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc67__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc69__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc70__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc71__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc72__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc73__Q22cf8CfObjectFv();
void CfObjectModel_teardownModel__Q22cf13CfObjectModelFv();
void CfObject_getPosVector__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc30__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc33__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc34__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc71__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc73__Q22cf13CfObjectModelFv();
void CfObjectModel_UnkVirtualFunc1__Q22cf13CfObjectModelFv();
void CfObjectModel_UnkVirtualFunc4__Q22cf13CfObjectModelFv();
void CfObjectModel_UnkVirtualFunc5__Q22cf13CfObjectModelFv();
void CfObjectModel_UnkVirtualFunc9__Q22cf13CfObjectModelFv();
void CfObjectModel_UnkVirtualFunc11__Q22cf13CfObjectModelFv();
void CfObjectModel_UnkVirtualFunc16__Q22cf13CfObjectModelFv();
void CfObjectModel_UnkVirtualFunc17__Q22cf13CfObjectModelFv();
void CfObjectModel_UnkVirtualFunc18__Q22cf13CfObjectModelFv();
void CfObjectModel_UnkVirtualFunc19__Q22cf13CfObjectModelFv();
void CfObjectModel_UnkVirtualFunc20__Q22cf13CfObjectModelFv();
}
extern char lbl_eu_80661D08[];
extern char lbl_eu_806618E8[];
extern char lbl_eu_806618F0[];
extern char lbl_eu_806618F8[];
extern char lbl_eu_80661900[];
extern const void* lbl_eu_80529128[114];
extern const void* lbl_eu_805292F0[10];
extern const void* modelVtable29318[114];
extern const void* modelVtable294E0[94];

// CfObjectMap-side vtable (0x1C8): sdata locator + CObjectState/Param slots,
// CfObjectMap/CfObject slots, then the CfObjectModel tail.
__declspec(section ".data") __attribute__((used, aligned(8))) const void* lbl_eu_80529128[114] = {
    lbl_eu_80661D08, 0,
    (const void*)CObjectState_setStateBitMask__Q22cf12CObjectStateFv,
    (const void*)CObjectState_checkStateFlags__Q22cf12CObjectStateFv,
    (const void*)CObjectState_setStateBitFlag__Q22cf12CObjectStateFv,
    (const void*)CObjectState_clearStateWord4__Q22cf12CObjectStateFv,
    (const void*)CObjectState_applyStateFlags__Q22cf12CObjectStateFv,
    (const void*)CObjectState_clearStateWord8__Q22cf12CObjectStateFv,
    (const void*)CObjectState_clearStateFlags8__Q22cf12CObjectStateFv,
    (const void*)CObjectState_UnkVirtualFunc8__Q22cf12CObjectStateFv,
    (const void*)CObjectState_UnkVirtualFunc9__Q22cf12CObjectStateFv,
    (const void*)CObjectState_setStateBitMask0__Q22cf12CObjectStateFv,
    (const void*)CObjectState_getStateData__Q22cf12CObjectStateFv,
    (const void*)CObjectState_setStateBitMask2__Q22cf12CObjectStateFv,
    (const void*)CObjectState_setStateBitMask3__Q22cf12CObjectStateFv,
    (const void*)CObjectParam_UnkVirtualFunc1__Q22cf12CObjectParamFv,
    (const void*)CObjectParam_getActiveParam__Q22cf13CfObjectModelFv,
    (const void*)CObjectParam_hasObjectName__Q22cf12CObjectParamFv,
    (const void*)CObjectParam_UnkVirtualFunc4__Q22cf12CObjectParamFv,
    (const void*)CObjectParam_getSelfObjectId__Q22cf12CObjectParamFv,
    (const void*)CObjectParam_signalActionEnd__Q22cf12CObjectParamFv,
    (const void*)__dt__Q22cf11CfObjectMapFv,
    (const void*)checkTarget__Q22cf11CfObjectMapFv,
    (const void*)clearStatus__Q22cf11CfObjectMapFv,
    (const void*)dispatchMapState__Q22cf11CfObjectMapFv,
    (const void*)CfObject_UnkVirtualFunc5__Q22cf8CfObjectFv,
    (const void*)cleanupMap__Q22cf11CfObjectMapFv,
    (const void*)nopMap__Q22cf11CfObjectMapFv,
    (const void*)CfObject_notifyEventDone__Q22cf13CfObjectModelFv,
    (const void*)isObjectMapReady__Q22cf11CfObjectMapFv,
    (const void*)CfObject_forwardSubObject__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc11__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc12__Q22cf8CfObjectFv,
    (const void*)CfObject_queryTargetState__Q22cf8CfObjectFv,
    (const void*)setMapScale__Q22cf11CfObjectMapFv,
    (const void*)CfObject_getMoveSpeedRate__Q22cf8CfObjectFv,
    (const void*)CfObject_pushRefreshExtra__Q22cf8CfObjectFv,
    (const void*)CfObject_readRefreshValue__Q22cf8CfObjectFv,
    (const void*)CfObject_checkTargetState__Q22cf8CfObjectFv,
    (const void*)CfObject_setMoveTargetVec__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc20__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc21__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc22__Q22cf13CfObjectModelFv,
    (const void*)CfObject_getPosVector__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc24__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc25__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc26__Q22cf8CfObjectFv,
    (const void*)CfObject_setModelRotVec__Q22cf13CfObjectModelFPv,
    (const void*)CfObject_UnkVirtualFunc28__Q22cf13CfObjectModelFv,
    (const void*)CfObject_setMoveHeadAngle__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc30__Q22cf13CfObjectModelFv,
    (const void*)CfObject_getMoveHeadAngle__Q22cf13CfObjectModelFv,
    (const void*)CfObject_setScaledHeadAngle__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc33__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc34__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc35__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc36__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc37__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc38__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc39__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc40__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc42__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc43__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc45__Q22cf8CfObjectFv,
    (const void*)CfObject_setMoveTargetPtr__Q22cf8CfObjectFv,
    (const void*)CfObject_createMoveTarget__Q22cf8CfObjectFv,
    (const void*)CfObject_getCurrentTarget__Q22cf8CfObjectFv,
    (const void*)CfObject_forwardNpcAction__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc50__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc51__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc52__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc53__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc54__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc55__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc56__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc57__Q22cf8CfObjectFv,
    (const void*)CfObject_getMoveRateScale__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc59__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc60__Q22cf8CfObjectFv,
    (const void*)CfObject_setAnimSlotEntry__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc62__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc63__Q22cf13CfObjectModelFv,
    (const void*)CfObject_setMoveBusyState__Q22cf8CfObjectFi,
    (const void*)CfObject_setMoveReadyFlag__Q22cf8CfObjectFv,
    (const void*)setMapVisibility__Q22cf11CfObjectMapFv,
    (const void*)CfObject_UnkVirtualFunc67__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc68__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc69__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc70__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc71__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc72__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc73__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc1__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_releaseModelList__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc3__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc4__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc5__Q22cf13CfObjectModelFv,
    (const void*)setMapId__Q22cf11CfObjectMapFv,
    (const void*)setMapEffectFlag__Q22cf11CfObjectMapFv,
    (const void*)CfObjectModel_UnkVirtualFunc8__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc9__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc10__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc11__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc12__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc13__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc14__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc15__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc16__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc17__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc18__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc19__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc20__Q22cf13CfObjectModelFv,
};
// RTTI locator list (0x28): sdata pairs + nulls.
__declspec(section ".data") __attribute__((used)) const void* lbl_eu_805292F0[10] = {
    lbl_eu_80661900, 0, lbl_eu_806618F8, 0, lbl_eu_806618F0, 0,
    lbl_eu_806618E8, 0, 0, 0,
};
// CfObjectModel vtable (0x1C8) -> lbl_eu_80529318 via UNIT_RULES rename.
__declspec(section ".data") __attribute__((used)) const void* modelVtable29318[114] = {
    lbl_eu_806618E8, 0,
    (const void*)CObjectState_setStateBitMask__Q22cf12CObjectStateFv,
    (const void*)CObjectState_checkStateFlags__Q22cf12CObjectStateFv,
    (const void*)CObjectState_setStateBitFlag__Q22cf12CObjectStateFv,
    (const void*)CObjectState_clearStateWord4__Q22cf12CObjectStateFv,
    (const void*)CObjectState_applyStateFlags__Q22cf12CObjectStateFv,
    (const void*)CObjectState_clearStateWord8__Q22cf12CObjectStateFv,
    (const void*)CObjectState_clearStateFlags8__Q22cf12CObjectStateFv,
    (const void*)CObjectState_UnkVirtualFunc8__Q22cf12CObjectStateFv,
    (const void*)CObjectState_UnkVirtualFunc9__Q22cf12CObjectStateFv,
    (const void*)CObjectState_setStateBitMask0__Q22cf12CObjectStateFv,
    (const void*)CObjectState_getStateData__Q22cf12CObjectStateFv,
    (const void*)CObjectState_setStateBitMask2__Q22cf12CObjectStateFv,
    (const void*)CObjectState_setStateBitMask3__Q22cf12CObjectStateFv,
    (const void*)CObjectParam_UnkVirtualFunc1__Q22cf12CObjectParamFv,
    (const void*)CObjectParam_getActiveParam__Q22cf13CfObjectModelFv,
    (const void*)CObjectParam_hasObjectName__Q22cf12CObjectParamFv,
    (const void*)CObjectParam_UnkVirtualFunc4__Q22cf12CObjectParamFv,
    (const void*)CObjectParam_getSelfObjectId__Q22cf12CObjectParamFv,
    (const void*)CObjectParam_signalActionEnd__Q22cf12CObjectParamFv,
    (const void*)__dt__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc2__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc3__Q22cf8CfObjectFv,
    0,
    (const void*)CfObject_UnkVirtualFunc5__Q22cf8CfObjectFv,
    (const void*)CfObjectModel_teardownModel__Q22cf13CfObjectModelFv,
    0,
    (const void*)CfObject_notifyEventDone__Q22cf13CfObjectModelFv,
    (const void*)CfObject_isMoveActiveNow__Q22cf8CfObjectFv,
    (const void*)CfObject_forwardSubObject__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc11__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc12__Q22cf8CfObjectFv,
    (const void*)CfObject_queryTargetState__Q22cf8CfObjectFv,
    (const void*)CfObject_pushRefreshValue__Q22cf8CfObjectFf,
    (const void*)CfObject_getMoveSpeedRate__Q22cf8CfObjectFv,
    (const void*)CfObject_pushRefreshExtra__Q22cf8CfObjectFv,
    (const void*)CfObject_readRefreshValue__Q22cf8CfObjectFv,
    (const void*)CfObject_checkTargetState__Q22cf8CfObjectFv,
    (const void*)CfObject_setMoveTargetVec__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc20__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc21__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc22__Q22cf13CfObjectModelFv,
    (const void*)CfObject_getPosVector__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc24__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc25__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc26__Q22cf8CfObjectFv,
    (const void*)CfObject_setModelRotVec__Q22cf13CfObjectModelFPv,
    (const void*)CfObject_UnkVirtualFunc28__Q22cf13CfObjectModelFv,
    (const void*)CfObject_setMoveHeadAngle__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc30__Q22cf13CfObjectModelFv,
    (const void*)CfObject_getMoveHeadAngle__Q22cf13CfObjectModelFv,
    (const void*)CfObject_setScaledHeadAngle__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc33__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc34__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc35__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc36__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc37__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc38__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc39__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc40__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc42__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc43__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc45__Q22cf8CfObjectFv,
    (const void*)CfObject_setMoveTargetPtr__Q22cf8CfObjectFv,
    (const void*)CfObject_createMoveTarget__Q22cf8CfObjectFv,
    (const void*)CfObject_getCurrentTarget__Q22cf8CfObjectFv,
    (const void*)CfObject_forwardNpcAction__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc50__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc51__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc52__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc53__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc54__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc55__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc56__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc57__Q22cf8CfObjectFv,
    (const void*)CfObject_getMoveRateScale__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc59__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc60__Q22cf8CfObjectFv,
    (const void*)CfObject_setAnimSlotEntry__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc62__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc63__Q22cf13CfObjectModelFv,
    (const void*)CfObject_setMoveBusyState__Q22cf8CfObjectFi,
    (const void*)CfObject_setMoveReadyFlag__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc66__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc67__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc68__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc69__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc70__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc71__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc72__Q22cf13CfObjectModelFv,
    (const void*)CfObject_UnkVirtualFunc73__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc1__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_releaseModelList__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc3__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc4__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc5__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_checkTargetNode__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc7__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc8__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc9__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc10__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc11__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc12__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc13__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc14__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc15__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc16__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc17__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc18__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc19__Q22cf13CfObjectModelFv,
    (const void*)CfObjectModel_UnkVirtualFunc20__Q22cf13CfObjectModelFv,
};
// CfObject vtable (0x178) -> lbl_eu_805294E0 via UNIT_RULES rename.
__declspec(section ".data") __attribute__((used)) const void* modelVtable294E0[94] = {
    lbl_eu_806618F0, 0,
    (const void*)CObjectState_setStateBitMask__Q22cf12CObjectStateFv,
    (const void*)CObjectState_checkStateFlags__Q22cf12CObjectStateFv,
    (const void*)CObjectState_setStateBitFlag__Q22cf12CObjectStateFv,
    (const void*)CObjectState_clearStateWord4__Q22cf12CObjectStateFv,
    (const void*)CObjectState_applyStateFlags__Q22cf12CObjectStateFv,
    (const void*)CObjectState_clearStateWord8__Q22cf12CObjectStateFv,
    (const void*)CObjectState_clearStateFlags8__Q22cf12CObjectStateFv,
    (const void*)CObjectState_UnkVirtualFunc8__Q22cf12CObjectStateFv,
    (const void*)CObjectState_UnkVirtualFunc9__Q22cf12CObjectStateFv,
    (const void*)CObjectState_setStateBitMask0__Q22cf12CObjectStateFv,
    (const void*)CObjectState_getStateData__Q22cf12CObjectStateFv,
    (const void*)CObjectState_setStateBitMask2__Q22cf12CObjectStateFv,
    (const void*)CObjectState_setStateBitMask3__Q22cf12CObjectStateFv,
    (const void*)CObjectParam_UnkVirtualFunc1__Q22cf12CObjectParamFv,
    (const void*)CObjectParam_UnkVirtualFunc2__Q22cf12CObjectParamFv,
    (const void*)CObjectParam_hasObjectName__Q22cf12CObjectParamFv,
    (const void*)CObjectParam_UnkVirtualFunc4__Q22cf12CObjectParamFv,
    (const void*)CObjectParam_getSelfObjectId__Q22cf12CObjectParamFv,
    (const void*)CObjectParam_signalActionEnd__Q22cf12CObjectParamFv,
    (const void*)__dt__Q22cf8CfObjectFv,
    0,
    (const void*)CfObject_UnkVirtualFunc3__Q22cf8CfObjectFv,
    0,
    (const void*)CfObject_UnkVirtualFunc5__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc6__Q22cf8CfObjectFv,
    0, 0,
    (const void*)CfObject_isMoveActiveNow__Q22cf8CfObjectFv,
    (const void*)CfObject_forwardSubObject__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc11__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc12__Q22cf8CfObjectFv,
    (const void*)CfObject_queryTargetState__Q22cf8CfObjectFv,
    (const void*)CfObject_pushRefreshValue__Q22cf8CfObjectFf,
    (const void*)CfObject_getMoveSpeedRate__Q22cf8CfObjectFv,
    (const void*)CfObject_pushRefreshExtra__Q22cf8CfObjectFv,
    (const void*)CfObject_readRefreshValue__Q22cf8CfObjectFv,
    (const void*)CfObject_checkTargetState__Q22cf8CfObjectFv,
    (const void*)CfObject_setMoveTargetVec__Q22cf8CfObjectFv,
    (const void*)CfObject_setPosXZ__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc21__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc22__Q22cf8CfObjectFv,
    (const void*)CfObject_getPosVector__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc24__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc25__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc26__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc27__Q22cf8CfObjectFPv,
    (const void*)CfObject_UnkVirtualFunc28__Q22cf8CfObjectFv,
    (const void*)CfObject_setMoveHeadAngle__Q22cf8CfObjectFf,
    (const void*)CfObject_UnkVirtualFunc30__Q22cf8CfObjectFv,
    (const void*)CfObject_getMoveHeadAngle__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc32__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc33__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc34__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc35__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc36__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc37__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc38__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc39__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc40__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc42__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc43__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc45__Q22cf8CfObjectFv,
    (const void*)CfObject_setMoveTargetPtr__Q22cf8CfObjectFv,
    (const void*)CfObject_createMoveTarget__Q22cf8CfObjectFv,
    (const void*)CfObject_getCurrentTarget__Q22cf8CfObjectFv,
    (const void*)CfObject_forwardNpcAction__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc50__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc51__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc52__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc53__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc54__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc55__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc56__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc57__Q22cf8CfObjectFv,
    (const void*)CfObject_getMoveRateScale__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc59__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc60__Q22cf8CfObjectFv,
    (const void*)CfObject_setAnimSlotEntry__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc62__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc63__Q22cf8CfObjectFv,
    (const void*)CfObject_setMoveBusyState__Q22cf8CfObjectFi,
    (const void*)CfObject_setMoveReadyFlag__Q22cf8CfObjectFv,
    0,
    (const void*)CfObject_UnkVirtualFunc67__Q22cf8CfObjectFv,
    0,
    (const void*)CfObject_UnkVirtualFunc69__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc70__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc71__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc72__Q22cf8CfObjectFv,
    (const void*)CfObject_UnkVirtualFunc73__Q22cf8CfObjectFv,
};
// Assert strings live in .data in retail (not .rodata). The 7-byte padding
// after g3d_resnode_ac.h is folded into the second array so the all-zero
// tail cannot migrate to .bss.
__declspec(section ".data") __attribute__((used)) const char lbl_eu_80529658[0x20] = "NW4R:Failed assertion IsValid()";
__declspec(section ".data") __attribute__((used)) const char lbl_eu_80529678[0x18] = "g3d_resnode_ac.h";

// Typified retail data (replaces the old generated byte blobs). Layout from
// build/us/asm/kyoshin/cf/object/CfObjectModel.s.
// .rodata 0x8: null-prefixed "move" name; element [1] is read as the node
// name string by CfObjectModel_UnkVirtualFunc7 (retail lbl_eu_804FC548).
__declspec(section ".rodata") __attribute__((aligned(8))) u8 lbl_eu_804FC548[0x8] __attribute__((used)) = {
    0x00, 'm', 'o', 'v', 'e', 0x00, 0x00, 0x00
};

// .sdata2 0x20: position/probe constants in retail order. Scalar consts are
// out: MWCC -ipa file folds their values into code (regressing Unk56/Unk7)
// and volatile reorders the loads (regressing Unk56 harder); the aggregate
// keeps the pool bytes ordered while its member loads stay opaque like the
// old UNDEF externs. Hpp keeps plain `extern const float` decls (ignored by
// codegen once the use sites rewrite to members below).
struct Sdata2_ModelPool { float f68, f6C, f70, f74, f78, f7C, f80, f84; };
__declspec(section ".sdata2") __attribute__((used, aligned(8)))
const Sdata2_ModelPool sdata2_ModelPool = {0.0f, 1.0f, -0.5f, 0.7071f, 0.19f, -1.3900001f, 0.5f, 1000.0f};
#define lbl_eu_80666A68 (sdata2_ModelPool.f68)
#define lbl_eu_80666A6C (sdata2_ModelPool.f6C)
#define lbl_eu_80666A70 (sdata2_ModelPool.f70)
#define lbl_eu_80666A74 (sdata2_ModelPool.f74)
#define lbl_eu_80666A78 (sdata2_ModelPool.f78)
#define lbl_eu_80666A7C (sdata2_ModelPool.f7C)
#define lbl_eu_80666A80 (sdata2_ModelPool.f80)
#define lbl_eu_80666A84 (sdata2_ModelPool.f84)

