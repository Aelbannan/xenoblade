// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectEff
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectEff.hpp"
#include "kyoshin/cf/code_800F42AC.hpp"
#include "kyoshin/realtimeevt/CREvtEffect.hpp"

struct CfObjIf {
    virtual void _v0008(int flag);
    virtual void _v000C();
    virtual void _v0010();
    virtual void _v0014();
    virtual void _v0018();
    virtual void _v001C();
    virtual void _v0020();
    virtual void _v0024();
    virtual void _v0028();
    virtual void _v002C(cf::CfObjectEff* parent);
    virtual void _v0030();
    virtual void _v0034();
    virtual void _v0038();
    virtual void _v003C();
    virtual void _v0040();
    virtual void _v0044();
    virtual void _v0048();
    virtual void _v004C();
    virtual void _v0050();
    virtual void _v0054();
    virtual void _v0058();
    virtual void _v005C();
    virtual void _v0060();
    virtual void _v0064();
    virtual void _v0068();
    virtual void _v006C();
    virtual void _v0070();
    virtual void _v0074();
    virtual void _v0078();
    virtual void _v007C();
    virtual void _v0080();
    virtual void _v0084();
    virtual void _v0088();
    virtual void _v008C();
    virtual void _v0090();
    virtual void _v0094();
    virtual void _v0098();
    virtual void vf009C();
    virtual void _v00A0();
    virtual void _v00A4();
    virtual void _v00A8();
    virtual void _v00AC();
    virtual void _v00B0();
    virtual void vf00B4();
    virtual void _v00B8();
    virtual void _v00BC();
    virtual void _v00C0();
    virtual void vf00C4();
    virtual void _v00C8();
    virtual void _v00CC();
    virtual void vf00D0();
};

namespace cf {
CfObjectEff::CfObjectEff() {
    // The retail base-ctor chain (CObjectState/CObjectParam/CfObject) is
    // inlined here: MWCC keeps the base vtable store and devirtualizes the
    // slot-0x5C call against the stored base vtable before the derived
    // vtable replaces it. The 0x70-0x8E tail is cleared in the same block.
    CfObjectEffCtorView* v = reinterpret_cast<CfObjectEffCtorView*>(this);
    v->field_04 = 0;
    v->field_08 = 0;
    v->field_0C = 0;
    v->field_10 = 0;
    v->field_30 = 0;
    v->field_34 = 0;
    *(void**)this = (void*)&lbl_eu_805294E0;
    v->field_38 = 0;
    v->field_3C = lbl_eu_80666958;
    v->field_40 = lbl_eu_80666958;
    v->field_44 = lbl_eu_80666958;
    v->field_48 = lbl_eu_80666958;
    v->field_4C = lbl_eu_80666958;
    v->field_50 = lbl_eu_80666958;
    v->field_54 = lbl_eu_80666958;
    v->field_58 = lbl_eu_80666958;
    v->field_5C = lbl_eu_80666958;
    v->field_60 = lbl_eu_8066695C;
    v->field_68 = 0;
    v->field_6C = 0;
    v->field_70 = 0;
    v->field_74 = 0;
    v->field_78 = 0;
    v->field_88 = 0;
    v->field_8C = 0;
    v->field_8E = 0;
    reinterpret_cast<CfObjectEffIf*>(this)->_f5C();
    // Derived ctor: switch to the CfObjectEff vtable and initialize the
    // effect slots, then register with the game manager's flag check.
    mFlags68 |= 0x100000;
    *(void**)this = lbl_eu_80528870;
    mSubObj90 = &lbl_eu_80528870[0x178];
    mChildEff = nullptr;
    mField98 = nullptr;
    mField9C = nullptr;
    mFieldA0 = nullptr;
    mFlagsA4 = 1;
    mCountA6 = 0;
    mFieldA8 = nullptr;
    mFieldAC = nullptr;
    mFieldB0 = nullptr;
    mFieldB4 = 0;
    mFieldB8 = nullptr;
    mFieldBC = nullptr;
    CfGameManager::getInstance();
    if (func_8006EF04__Fi(0x04000000)) {
        mFlagsA4 |= 0x2;
    }
}
} // namespace cf

void CfObject_UnkVirtualFunc3__Q22cf8CfObjectFv() {}

namespace cf {
CfObject::~CfObject() {
    // Reset the vtable and run the slot-0x68 cleanup; MWCC auto-emits the
    // null guard, the vtable store and the delete-flag guard around it.
    reinterpret_cast<CfObjectEffIf*>(this)->_f68();
}
} // namespace cf

void CfObject_UnkVirtualFunc6__Q22cf8CfObjectFv() {}

namespace cf {
bool CfObjectEff::func_800AC7CC() {
    CfObject_UnkVirtualFunc8();
    return true;
}
extern "C" u32 func_800AC7FC__Q22cf11CfObjectEffFv(cf::CfObjectEff* self) { return *(u32*)((u8*)self + 0x94) != 0; }
void CfObjectEff::func_800AC810() {
    if (mChildEff != nullptr) {
        // Parent for the effect detach is this+0x90; retail null-guards the
        // offset so a null this passes 0 to func_804E3D48.
        u8* parent = reinterpret_cast<u8*>(this);
        if (this != nullptr) {
            parent = reinterpret_cast<u8*>(this) + 0x90;
        }
        func_804E3D48(mChildEff, parent);
        func_804E3CCC(mChildEff);
        mChildEff = nullptr;
    }
}
} // namespace cf

void CfObjectModel_UnkVirtualFunc18__Q22cf13CfObjectModelFv() {}

namespace cf {
// Retail symbol func_800AC990__Q22cf11CfObjectEffFv (void params in the name)
// but the body consumes an object pointer in r4 - forced-name form.
// Detaches `obj` from either the 0x9C or 0xA0 slot when the active sub-object
// is set: tears the sub-object down, clears the slot and sets flag 0x40.
extern "C" bool func_800AC990__Q22cf11CfObjectEffFv(cf::CfObjectEff* self, u8* obj) {
    if (self->getSub188() != 0) {
        if (self->mField9C == obj) {
            self->destroy190();
            self->mField9C = 0;
            u32 flags = self->mFlags68;
            self->mFlags68 = flags | 0x40;
            return true;
        }
        if (self->mFieldA0 == obj) {
            self->destroy190();
            self->mFieldA0 = 0;
            u32 flags = self->mFlags68;
            self->mFlags68 = flags | 0x40;
            return true;
        }
    }
    return false;
}
// Retail symbol func_800ACA58__Q22cf11CfObjectEffFv (void params in the name)
// but the body consumes an object pointer in r4 - forced-name form.
extern "C" bool func_800ACA58__Q22cf11CfObjectEffFv(cf::CfObjectEff* self, u8* arg) {
    if (self->getSub188() != 0 && arg == self->mField98) {
        self->destroy190();
        u32 flags = self->mFlags68;
        self->mField9C = 0;
        self->mField98 = 0;
        self->mFlags68 = flags | 0x40;
        return true;
    }
    return false;
}
void CfObjectEff::func_800ACAE8() {
    if (mSubObj38 != nullptr)
        reinterpret_cast<CfObjIf*>(mSubObj38)->_v00A8();
}
// Retail symbol func_800ACB08__Q22cf11CfObjectEffFv (void params in the name)
// but the body consumes an object pointer in r4 - forced-name form.
extern "C" void func_800ACB08__Q22cf11CfObjectEffFv(cf::CfObjectEff* self, u8* arg) {
    if (self->mSubObj38 != 0) {
        // Redundant nested check on the same loaded value mirrors retail's two
        // beq targets (MWCC keeps both branches).
        if (self->mSubObj38 != 0) {
            reinterpret_cast<CfObjIf*>(self->mSubObj38)->_v0008(1);
        }
        self->mSubObj38 = 0;
    }
    self->mSubObj38 = arg;
    if (arg != 0) {
        reinterpret_cast<CfObjIf*>(arg)->_v002C(self);
        reinterpret_cast<CfObjIf*>(self->mSubObj38)->_v00A0();
    }
}
// Retail symbol func_800ACBA4__Q22cf11CfObjectEffFv (void params in the name)
// but the body consumes a bool flag in r4 - forced-name form.
extern "C" void func_800ACBA4__Q22cf11CfObjectEffFv(cf::CfObjectEff* self, bool flag) {
    if (flag)
        self->mFlags68 |= 0x100000;
    else
        self->mFlags68 &= ~0x100000;
}
// Retail symbol func_800ACBCC__Q22cf11CfObjectEffFv (void params in the name)
// but the body consumes a flag in r4 - forced-name form.
extern "C" void func_800ACBCC__Q22cf11CfObjectEffFv(cf::CfObjectEff* self, int flag) {
    if (self->mChildEff != nullptr) {
        // Bit 10 (0x400) of the child's u16 at +0x0 doubles as an enabled flag;
        // the != comparison makes MWCC form the neg/or truth-mask and insert
        // its sign bit via rlwimi, matching retail.
        self->mChildEff->flag400 = flag != 0;
    }
    if (flag)
        self->mFlagsA4 |= 0x1;
    else
        self->mFlagsA4 &= ~0x1;
}
} // namespace cf

void func_800ACC14(void* self, unsigned char val) {
    cf::CfObjectEff* obj = static_cast<cf::CfObjectEff*>(self);
    if (obj->mChildEff != nullptr)
        obj->mChildEff->unk59 = val;
}

void func_800ACC28(cf::CfObject* self, float first, float second) {
    cf::CfObjectEff* eff = static_cast<cf::CfObjectEff*>(self);
    if (eff->mChildEff != nullptr)
        return func_804E3CDC(eff->mChildEff, first, second);
}

namespace cf {
extern "C" void func_800ACC3C__Q22cf11CfObjectEffFv(cf::CfObjectEff* self, float v) {
    void* o = *(void**)((u8*)self + 0x94);
    if (o) *(float*)((u8*)o + 0x50) = v;
}
} // namespace cf

extern "C" void func_800ACC50(void* self, float v) {
    void* o = *(void**)((u8*)self + 0x94);
    if (o) *(float*)((u8*)o + 0x50) = v;
}

void func_800ACC64(void* obj, const void* src) {
    cf::CfObjectEff* eff = static_cast<cf::CfObjectEff*>(obj);
    cf::CfObjectEffChild* child = eff->mChildEff;
    if (child == nullptr) return;
    const u32* s = static_cast<const u32*>(src);
    child->unk40[0] = s[0];
    child->unk40[1] = s[1];
    child->unk40[2] = s[2];
    child->unk40[3] = s[3];
}

namespace cf {
// Retail symbol func_800ACC94__Q22cf11CfObjectEffFv (void params in the name)
// but the body consumes a position-vector pointer in r4 - forced-name form.
extern "C" void func_800ACC94__Q22cf11CfObjectEffFv(cf::CfObjectEff* self, const cf::CfObjectEffU32Vec3* src) {
    cf::CfObjectEffChild* child = self->mChildEff;
    if (child != nullptr) {
        child->field_1C[0] = src->x;
        child->field_1C[1] = src->y;
        child->field_1C[2] = src->z;
    }
    // Whole-vector word copy into the position floats (batched lwz/stw).
    *reinterpret_cast<cf::CfObjectEffU32Vec3*>(&self->mPos3C) = *src;
}


} // namespace cf

// Retail symbol func_800ACCE4__Q22cf11CfObjectEffFv is Fv-mangled but the
// body consumes a position-vector pointer in r4 - defined at global scope so
// MWCC's mangled name still contains the retail symbol as a prefix.
void func_800ACCE4__Q22cf11CfObjectEffFv(cf::CfObjectEff* self, const cf::CfObjectEffU32Vec3* src) {
    self->CfObject::CfObject_UnkVirtualFunc25();
    cf::CfObjectEffChild* child = self->mChildEff;
    self->mFlags68 |= 0x100;
    if (child != nullptr) {
        child->field_1C[0] = src->x;
        child->field_1C[1] = src->y;
        child->field_1C[2] = src->z;
    }
}

namespace cf {
// Retail symbol func_800ACD5C__Q22cf11CfObjectEffFv (void params in the name)
// but the body consumes two floats in f1/f2 - forced-name form.
extern "C" void func_800ACD5C__Q22cf11CfObjectEffFv(cf::CfObjectEff* self, float a, float b) {
    float arr[3];
    arr[0] = a;
    arr[1] = lbl_eu_80666960;
    arr[2] = b;
    reinterpret_cast<cf::CfObjectEffMoveIf*>(self)->moveB4(arr, lbl_eu_80666964);
}

// Retail symbol func_800ACDA0__Q22cf11CfObjectEffFv (void params in the name)
// but the body consumes a rotation-vector pointer in r4 - forced-name form.
extern "C" void func_800ACDA0__Q22cf11CfObjectEffFv(cf::CfObjectEff* self, const cf::CfObjectEffU32Vec3* src) {
    cf::CfObjectEffChild* child = self->mChildEff;
    if (child != nullptr) {
        *reinterpret_cast<u32*>(&child->field_28) = src->x;
        *reinterpret_cast<u32*>(&child->unk2C) = src->y;
        *reinterpret_cast<u32*>(&child->field_30) = src->z;
    }
    // Whole-vector word copy into the 0x48 tail (spanning mField4C).
    reinterpret_cast<cf::CfObjectEffVec48View*>(self)->vec48 = *src;
}
u8* CfObjectEff::func_800ACDE0() {
    if (mChildEff != nullptr)
        return reinterpret_cast<u8*>(mChildEff) + 0x28;
    return reinterpret_cast<u8*>(this) + 0x48;
}
// Retail symbol func_800ACDFC__Q22cf11CfObjectEffFv (void params in the name)
// but the body consumes a float in f1 - forced-name form.
extern "C" void func_800ACDFC__Q22cf11CfObjectEffFv(cf::CfObjectEff* self, float a) {
    cf::CfObjectEffChild* child = self->mChildEff;
    if (child != nullptr) {
        // Rebuild the child vector with a fresh Y. Taking the local's address
        // spills it to the stack, and the word copies back (lwz/stw) match retail.
        cf::CfObjectEffVec3 v;
        v.x = child->field_28;
        v.y = a;
        v.z = child->field_30;
        *reinterpret_cast<u32*>(&child->field_28) = *reinterpret_cast<u32*>(&v.x);
        *reinterpret_cast<u32*>(&child->unk2C) = *reinterpret_cast<u32*>(&v.y);
        *reinterpret_cast<u32*>(&child->field_30) = *reinterpret_cast<u32*>(&v.z);
    }
    self->mField4C = a;
}
} // namespace cf

float func_800ACE44__Q22cf11CfObjectEffFv(void* self) {
    cf::CfObjectEff* obj = static_cast<cf::CfObjectEff*>(self);
    if (obj->mChildEff != nullptr) {
        return obj->mChildEff->unk2C;
    }
    return reinterpret_cast<float&>(static_cast<cf::CfObject*>(obj)->mField4C);
}

namespace cf {
// Retail symbol name is func_800ACE60__Q22cf11CfObjectEffFv (void params in the
// name) but the body consumes a float scale argument in f1 - forced-name form.
extern "C" void func_800ACE60__Q22cf11CfObjectEffFv(cf::CfObjectEff* self, float value) {
    // Scale the value by the global factor, then dispatch through vtable
    // slot 0xC4 (CfObject_UnkVirtualFunc29).
    reinterpret_cast<CfObjectEffIf*>(self)->setScale(value * lbl_eu_8066A210);
}
float CfObjectEff::func_800ACE78() {
    float result = CfObject_UnkVirtualFunc31();
    return result * lbl_eu_8066A20C;
}
} // namespace cf

// Retail symbol func_800ACEAC__Q22cf11CfObjectEffFv is Fv-mangled but the
// body consumes a float scale in f1 - defined at global scope so MWCC's
// mangled name still contains the retail symbol as a prefix.
void func_800ACEAC__Q22cf11CfObjectEffFv(cf::CfObjectEff* self, float value) {
    if (!(self->mFlagsA4 & 0x4)) {
        cf::CfObjectEffChild* child = self->mChildEff;
        if (child != nullptr) {
            // Word-copy the float bits into the child's three 0x34 slots; the
            // local's address spills it to the stack (stfs/lwz/stw) like retail.
            cf::CfObjectEffVec3 v;
            v.x = value;
            v.y = value;
            v.z = value;
            cf::CfObjectEffChild34View* dst = reinterpret_cast<cf::CfObjectEffChild34View*>(child);
            dst->field_34[0] = *reinterpret_cast<u32*>(&v.x);
            dst->field_34[1] = *reinterpret_cast<u32*>(&v.y);
            dst->field_34[2] = *reinterpret_cast<u32*>(&v.z);
        }
        self->mFloat60 = value;
    }
}

void func_800ACEF8(void* obj, const void* src) {
    cf::CfObjectEff* eff = static_cast<cf::CfObjectEff*>(obj);
    if (eff->mFlagsA4 & 0x4) return;
    cf::CfObjectEffChild* child = eff->mChildEff;
    if (child != nullptr) {
        const u32* s = static_cast<const u32*>(src);
        child->unk34[0] = s[0];
        child->unk34[1] = s[1];
        child->unk34[2] = s[2];
    }
    float f = *static_cast<const float*>(src);
    static_cast<cf::CfObject*>(eff)->mFloat60 = f;
}

float func_800ACF34__Q22cf11CfObjectEffFv(char* self) {
    cf::CfObjectEff* obj = reinterpret_cast<cf::CfObjectEff*>(self);
    if (obj->mChildEff != nullptr) {
        return reinterpret_cast<float&>(obj->mChildEff->unk34[1]);
    }
    return static_cast<cf::CfObject*>(obj)->mFloat60;
}

namespace cf {
// Retail symbol func_800ACF50__Q22cf11CfObjectEffFv (void params in the name)
// but the body consumes a bool flag in r4 - forced-name form.
extern "C" void func_800ACF50__Q22cf11CfObjectEffFv(cf::CfObjectEff* self, bool flag) {
    if (flag)
        self->mFlagsA4 |= 0x4;
    else
        self->mFlagsA4 &= ~0x4;
}
} // namespace cf

// r4 = object, r5 = partner. Stores both on the owner, then when a child
// effect exists and only the object is set, resolves the partner via
// func_800BB340 (a lookup) before writing it to the child.
void func_800ACF78(cf::CfObjectEff* self, u8* object, u8* partner) {
    self->mFieldAC = partner;
    self->mField9C = object;
    if (self->mChildEff != nullptr) {
        if (object != nullptr && partner == nullptr) {
            partner = ((u8* (*)(u8*))func_800BB340)(object);
        }
        self->mChildEff->field_14 = partner;
    }
}
void func_800ACFD8(cf::CfObjectEff* obj, u8* target) {
    if (obj->mChildEff != nullptr) {
        if (target != nullptr)
            obj->mChildEff->field_18 = ((u8* (*)(u8*))func_800BB340)(target);
        else
            obj->mChildEff->field_18 = nullptr;
    }
    obj->mFieldA0 = target;
}

void func_800AD040(char* obj, int flag) {
    cf::CfObjectEff* eff = reinterpret_cast<cf::CfObjectEff*>(obj);
    if (eff->mChildEff == nullptr) return;
    eff->mChildEff->unk5C = (flag != 0);
}

void func_800AD060(cf::CfObjectEff* self) {
    // Gated by the game-manager flag; the effect type is packed in the top
    // 5 bits of mField70. The 0x10/0x11 types also require the source's +0xC
    // word to be clear, otherwise the whole update is skipped.
    if (reinterpret_cast<cf::CfGameManager*>(self)->func_80082900() == 0) return;
    if (self->mFlags68 & 0x40) return;
    u32 type = self->mField70 >> 27;
    u8* source = nullptr;
    if (type == 0xe) {
        source = static_cast<u8*>(func_800630C8());
    } else if (type == 0xc) {
        source = static_cast<u8*>(func_80063038());
    } else if (type == 0xf) {
        source = self->mFieldA8;
    } else if (type >= 0x10 && type <= 0x11) {
        source = self->mFieldA8;
        if (source != 0 &&
            reinterpret_cast<cf::CfObjectEffSourceView*>(source)->field_0C != 0) {
            return;
        }
    }
    if (source == 0) {
        self->mFlags68 |= 0x40;
        return;
    }
    bool valid = true;
    if (reinterpret_cast<cf::CfObjectEffSourceSubIf*>(
            reinterpret_cast<cf::CfObjectEffSourceView*>(source)->field_2C)->func40(source) == 0) {
        valid = false;
    }
    if (!valid) return;
    void* bdat = func_80066E7C(source, self->mField70);
    if (bdat == 0) bdat = func_80066CF8(source);
    if (bdat != 0 && lbl_eu_8065FC18 != 0) {
        // Drop dead partner objects before creating the effect.
        if (self->mField9C != 0 && func_800B8920(self->mField9C) == 0) {
            self->mField9C = 0;
            self->mFlags68 |= 0x40;
            self->mCount8E++;
            return;
        }
        if (self->mFieldA0 != 0 && func_800B8920(self->mFieldA0) == 0) {
            self->mFieldA0 = 0;
            self->mFlags68 |= 0x40;
            self->mCount8E++;
            return;
        }
        void* mgr = self->mFieldBC;
        // One u32 slot is reused for the bound flag and later for the
        // mFieldA0 value; MWCC keeps the same callee-saved register for a
        // single variable across both live ranges, matching retail's r30.
        u32 work = self->mFlagsA4 & 1;
        if (mgr == 0) mgr = (void*)CfRes_getD80Flag();
        if (self->mCount8C != 0) {
            self->mChildEff = static_cast<cf::CfObjectEffChild*>(
                func_804CC1F4(lbl_eu_8065FC18, bdat, mgr, self->mCount8C - 1, work, 0));
        }
        self->mField98 = static_cast<u8*>(bdat);
        if (self->mChildEff != 0) {
            reinterpret_cast<cf::CfObjectEffOwnerIf*>(self)->vf9C(&self->mPos3C);
            reinterpret_cast<cf::CfObjectEffOwnerIf*>(self)->vfBC(reinterpret_cast<u8*>(self) + 0x48);
            reinterpret_cast<cf::CfObjectEffOwnerIf*>(self)->vfDC(self->mFloat60);
            // The child may have been torn down by the slot calls above;
            // retail re-checks it before each partner write. The partner and
            // source-object loads are hoisted above the guard so MWCC emits
            // them before the branch, matching retail's schedule.
            u8* partner = self->mFieldAC;
            u8* f9c = self->mField9C;
            if (self->mChildEff != 0) {
                if (f9c != 0 && partner == 0) {
                    partner = static_cast<u8*>(func_800BB340(f9c));
                }
                self->mChildEff->field_14 = partner;
            }
            work = reinterpret_cast<u32>(self->mFieldA0);
            if (self->mChildEff != 0) {
                if (work != 0) {
                    self->mChildEff->field_18 = static_cast<u8*>(func_800BB340(reinterpret_cast<void*>(work)));
                } else {
                    self->mChildEff->field_18 = 0;
                }
            }
            self->mFieldA0 = reinterpret_cast<u8*>(work);
            void* result = func_804E3CFC(self->mChildEff);
            reinterpret_cast<cf::CfObjectEffOwnerIf*>(self)->vf3C(result);
            self->mFieldB4 = 1;
            u8* parent = reinterpret_cast<u8*>(self);
            if (self != 0) parent = reinterpret_cast<u8*>(self) + 0x90;
            func_804E3D0C(self->mChildEff, parent);
        } else {
            self->mFlags68 |= 0x40;
        }
    }
    self->mCount8E++;
}

void func_800AD378(void* obj) {
    cf::CfObjectEff* eff = static_cast<cf::CfObjectEff*>(obj);
    if (eff->mCountA6 == 0) return;
    eff->mCountA6--;
    if (eff->mCountA6 != 0) return;
    static_cast<cf::CfObject*>(eff)->mFlags68 |= 0x40;
}

namespace cf {
void CfObjectEff::func_800AD3A4() {
    // Dispatch through the ptmf table (0x18 bytes before the CfObjectEff
    // vtable) while the u16 counter at 0x8E stays below 2.
    if (mCount8E < 2) {
        (this->*lbl_eu_80528858[mCount8E])();
    }
    if (mSubObj38 != nullptr) {
        reinterpret_cast<CfObjIf*>(mSubObj38)->_v00A4();
    }
    if (mChildEff != nullptr) {
        // Sync a child flag bit with slot-0x160's status word: retail reads
        // bit 18 of the 16-bit flag word (a zero bit for a u16 - kept verbatim)
        // and on mismatch writes status bit 28 into flag bit 14 (0x4000). The
        // mask form stops MWCC from constant-folding the u16 range.
        u16 flags = reinterpret_cast<CfObjectEffChildFlagsView*>(mChildEff)->field_00;
        bool b = (flags & 0x40000) != 0;
        if (b != reinterpret_cast<CfObjectEffVtable160If*>(this)->func160()) {
            reinterpret_cast<CfObjectEffChildFlagsView*>(mChildEff)->field_00 =
                (reinterpret_cast<CfObjectEffChildFlagsView*>(mChildEff)->field_00 & ~0x4000) |
                ((reinterpret_cast<CfObjectEffVtable160If*>(this)->func160() >> 14) & 0x4000);
        }
    }
    if (mFieldA0 != nullptr && func_800B8920(mFieldA0) == 0) {
        mFieldA0 = nullptr;
        if (mChildEff != nullptr) {
            mChildEff->field_18 = nullptr;
        }
    }
}
int CfObjectEff::func_800AD4A4() {
    return ((*(u32*)((u8*)this + 0x68) >> 20) & 1);
}
} // namespace cf

// Chain node/mgr views for func_800AD558: the manager returned by
// func_80086B14 holds a circular list whose sentinel lives at +0x4; each
// node's +0x8 is a CfObjectEff.
struct EffChainNode {
    EffChainNode* next;   // 0x00
    u32 field_04;         // 0x04
    u8* object;           // 0x08
};
struct EffChainMgr {
    u32 field_00;              // 0x00
    EffChainNode* sentinel;    // 0x04 - circular-list sentinel
};

// func_80086B14 is declared void in CfGameManager.hpp but retail returns the
// effect chain manager in r3; declare the retail-named entry point here (the
// header is read-only and the member cannot be redeclared with a return type).
extern "C" EffChainMgr* func_80086B14__Q22cf13CfGameManagerFv(cf::CfGameManager* self);

// Iterates the effect chain, dynamic-casting each node's object to the base
// type at lbl_eu_806618F0 and poking its slot-0x198 method with `self`; the
// call's side effects are the point (both branches advance to the same node).
void func_800AD4B0(u8* self) {
    cf::CfGameManager* gm = cf::CfGameManager::getInstance();
    EffChainMgr* mgr = func_80086B14__Q22cf13CfGameManagerFv(gm);
    EffChainNode* node = mgr->sentinel->next;
    while (node != mgr->sentinel) {
        EffChainNode* next = node->next;
        void* obj = __dynamic_cast(node->object, 0, &lbl_eu_80661970, &lbl_eu_806618F0, 0);
        if (reinterpret_cast<cf::CfObjectEffVtable198If*>(obj)->func198(self) != 0) {
            node = next;
        } else {
            node = node->next;
        }
    }
}

void func_800AD558(u8* arg) {
    if (arg == 0) return;
    cf::CfGameManager* gm = cf::CfGameManager::getInstance();
    EffChainMgr* mgr = func_80086B14__Q22cf13CfGameManagerFv(gm);
    EffChainNode* node = mgr->sentinel->next;
    while (node != mgr->sentinel) {
        cf::CfObjectEff* obj = reinterpret_cast<cf::CfObjectEff*>(node->object);
        if (obj->mChildEff != nullptr) {
            if (obj->mChildEff->field_14 == arg) {
                obj->mChildEff->field_14 = nullptr;
                obj->mFieldB8 = arg;
            }
        } else {
            if (obj->mFieldAC == arg) {
                obj->mFieldAC = nullptr;
                obj->mFieldB8 = arg;
            }
        }
        node = node->next;
    }
}
// Reattaches a detached target: walks the effect chain and restores `arg1`
// into either the child's partner slot or the owner's 0xAC slot wherever
// `arg0` was parked in the 0xB8 detached-target slot, then clears 0xB8.
void func_800AD5EC(u8* arg0, u8* arg1) {
    if (arg0 == 0) return;
    cf::CfGameManager* gm = cf::CfGameManager::getInstance();
    EffChainMgr* mgr = func_80086B14__Q22cf13CfGameManagerFv(gm);
    EffChainNode* node = mgr->sentinel->next;
    while (node != mgr->sentinel) {
        cf::CfObjectEff* obj = reinterpret_cast<cf::CfObjectEff*>(node->object);
        if (obj->mChildEff != nullptr) {
            if (obj->mFieldB8 == arg0) {
                obj->mChildEff->field_14 = arg1;
                obj->mFieldB8 = nullptr;
            }
        } else {
            if (obj->mFieldB8 == arg0) {
                obj->mFieldAC = arg1;
                obj->mFieldB8 = nullptr;
            }
        }
        node = node->next;
    }
}

namespace cf {
// Retail symbol func_800AD68C__Q22cf11CfObjectEffFv (void params in the name)
// but the body consumes an object pointer in r4 - forced-name form.
// Release path: detaches the child effect, clears the 0xB0 object, pokes the
// 0x9C/0xA0 objects' slot-0x1BC (gated by the arg's 0x14 validity word and
// the live-object check), then drops the child and sets flag 0x40.
extern "C" void func_800AD68C__Q22cf11CfObjectEffFv(cf::CfObjectEff* self, u8* arg) {
    // Parent for the effect detach is this+0x90; retail null-guards the
    // offset so a null this passes 0 to func_804E3D48.
    u8* parent = reinterpret_cast<u8*>(self);
    if (self != nullptr) {
        parent = reinterpret_cast<u8*>(self) + 0x90;
    }
    func_804E3D48(self->mChildEff, parent);
    if (self->mFieldB0 != nullptr) {
        reinterpret_cast<cf::CfObjectEffVtable0CIf*>(self->mFieldB0)->func0C(self);
        self->mFieldB0 = nullptr;
    }
    if (self->mField9C != nullptr &&
        reinterpret_cast<cf::CfObjectEffArg14View*>(arg)->field_14 != 0 &&
        func_800B8920(self->mField9C) != 0) {
        reinterpret_cast<cf::CfObjectEffVtable1BCIf*>(self->mField9C)->func1BC(self);
    }
    if (self->mFieldA0 != nullptr && func_800B8920(self->mFieldA0) != 0) {
        reinterpret_cast<cf::CfObjectEffVtable1BCIf*>(self->mFieldA0)->func1BC(self);
    }
    self->mChildEff = nullptr;
    u32 flags = self->mFlags68;
    self->mFlags68 = flags | 0x40;
}
CfObjectEff::~CfObjectEff() {
    // novtable suppresses MWCC's implicit vtable store, so write both vtable
    // pointers explicitly (derived vtable at +0x0, its +0x178 secondary at
    // +0x90); MWCC then devirtualizes the slot-0x68 destroy against the stored
    // vtable (reusing the lis/addi register) and auto-inlines the base dtor
    // (null guard + CfObject vtable + destroy) plus the delete flag guard.
    *(void**)this = lbl_eu_80528870;
    mSubObj90 = &lbl_eu_80528870[0x178];
    reinterpret_cast<CfObjectEffIf*>(this)->_f68();
}
// Retail symbol name is func_800AD818__Q22cf11CfObjectEffFv (void params in the
// name) but the body consumes a mask argument in r4 - forced-name form.
extern "C" bool func_800AD818__Q22cf11CfObjectEffFv(cf::CfObjectEff* self, u16 flags) {
    return (self->mFlagsA4 & flags) != 0;
}


} // namespace cf

// Forward declarations for symbols used by thunks
void func_800AD68C__Q22cf11CfObjectEffFv();
void __dt__Q22cf11CfObjectEffFv();

void func_800AD850__Q22cf11CfObjectEffFv(void* self) {
    ((void(*)(void*))func_800AD68C__Q22cf11CfObjectEffFv)((char*)self - 0x90);
}

void func_800AD858__Q22cf11CfObjectEffFv(void* self) {
    ((void(*)(void*))__dt__Q22cf11CfObjectEffFv)((char*)self - 0x90);
}

// Container-of helper (retail symbol func_800AD860__FPv): obj points at a
// CfObject-family sub-object embedded at +0x3E9C inside its owner. Returns
// the owner when the +0x64 flag word has bit 4 set, else nullptr. The
// nested null guard returns `obj` (0 there) so MWCC tail-returns via
// `beqlr cr1`, matching retail.
void* func_800AD860(void* obj) {
    if (obj != 0 && (static_cast<cf::CfObjectEffSubView*>(obj)->field_0x64 & 0x4) != 0) {
        if (obj == 0) return obj;
        return static_cast<u8*>(obj) - 0x3E9C;
    }
    return 0;
}

extern "C" void func_800ACCD4__Q22cf11CfObjectEffFv(cf::CfObjectEff* self) { reinterpret_cast<CfObjIf*>(self)->vf009C(); }

extern "C" void func_800ACD4C__Q22cf11CfObjectEffFv(cf::CfObjectEff* self) { reinterpret_cast<CfObjIf*>(self)->vf00B4(); }

extern "C" void func_800AD830__Q22cf11CfObjectEffFv(cf::CfObjectEff* self) { reinterpret_cast<CfObjIf*>(self)->vf00D0(); }

extern "C" void func_800AD840__Q22cf11CfObjectEffFv(cf::CfObjectEff* self) { reinterpret_cast<CfObjIf*>(self)->vf00C4(); }


namespace cf {
void CfObjectEff::func_800AC86C() {
    if (mFieldB0 != nullptr) {
        reinterpret_cast<CfObjectEffVtable0CIf*>(mFieldB0)->func0C(this);
        mFieldB0 = nullptr;
    }
    if (mSubObj38 != nullptr) {
        reinterpret_cast<CfObjIf*>(mSubObj38)->_v00AC();
        if (mSubObj38 != nullptr) {
            // Redundant nested check on the reloaded value mirrors retail's
            // two beq targets (MWCC keeps both branches).
            if (mSubObj38 != nullptr) {
                reinterpret_cast<CfObjIf*>(mSubObj38)->_v0008(1);
            }
            mSubObj38 = nullptr;
        }
    }
    if (getSub188() != nullptr) {
        if (mField9C != nullptr) {
            reinterpret_cast<CfObjectEffVtable1BCIf*>(mField9C)->func1BC(this);
        }
        if (mFieldA0 != nullptr) {
            reinterpret_cast<CfObjectEffVtable1BCIf*>(mFieldA0)->func1BC(this);
        }
        destroy190();
    }
    mField70 = 0;
    mFlags68 &= 0x40000000;
}
} // namespace cf
