// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectEff
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectEff.hpp"
#include "kyoshin/CUIWindowManagerApi.hpp"
#include "kyoshin/cf/code_800F42AC.hpp"
#include "kyoshin/realtimeevt/CREvtEffect.hpp"
#include "kyoshin/cf/object/CfObjectModel.hpp"



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
    this->CfObject_UnkVirtualFunc3();
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
    if (isGlobalCamFlagSet__Fi(0x04000000)) {
        mFlagsA4 |= 0x2;
    }
}
} // namespace cf

void CfObject_UnkVirtualFunc3__Q22cf8CfObjectFv() {}

namespace cf {
CfObject::~CfObject() {
    // Reset the vtable and run the slot-0x68 cleanup; MWCC auto-emits the
    // null guard, the vtable store and the delete-flag guard around it.
    // Slot +0x68 is CfObject_UnkVirtualFunc6 (void() - matches the retail
    // devirtualized call through the just-stored base vtable).
    this->CfObject_UnkVirtualFunc6();
}
} // namespace cf

void CfObject_UnkVirtualFunc6__Q22cf8CfObjectFv() {}

namespace cf {
bool CfObjectEff::initEffState_() {
    CfObject_UnkVirtualFunc8();
    return true;
}
extern "C" u32 hasChildEffs___Q22cf11CfObjectEffFv(cf::CfObjectEff* self) { return *(u32*)((u8*)self + 0x94) != 0; }
void CfObjectEff::detachChildEf() {
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
// Retail symbol detachPrtnrA___Q22cf11CfObjectEffFv (void params in the name)
// but the body consumes an object pointer in r4 - forced-name form.
// Detaches `obj` from either the 0x9C or 0xA0 slot when the active sub-object
// is set: tears the sub-object down, clears the slot and sets flag 0x40.
extern "C" bool detachPrtnrA___Q22cf11CfObjectEffFv(cf::CfObjectEff* self, u8* obj) {
    if (self->hasChildEffs_() != 0) {
        if (self->mField9C == obj) {
            self->detachChildEf();
            self->mField9C = 0;
            u32 flags = self->mFlags68;
            self->mFlags68 = flags | 0x40;
            return true;
        }
        if (self->mFieldA0 == obj) {
            self->detachChildEf();
            self->mFieldA0 = 0;
            u32 flags = self->mFlags68;
            self->mFlags68 = flags | 0x40;
            return true;
        }
    }
    return false;
}
// Retail symbol detachBoundO___Q22cf11CfObjectEffFv (void params in the name)
// but the body consumes an object pointer in r4 - forced-name form.
extern "C" bool detachBoundO___Q22cf11CfObjectEffFv(cf::CfObjectEff* self, u8* arg) {
    if (self->hasChildEffs_() != 0 && arg == self->mField98) {
        self->detachChildEf();
        u32 flags = self->mFlags68;
        self->mField9C = 0;
        self->mField98 = 0;
        self->mFlags68 = flags | 0x40;
        return true;
    }
    return false;
}
void CfObjectEff::notifySubA08_() {
    if (mSubObj38 != nullptr)
        reinterpret_cast<cf::CfObjectSub38*>(mSubObj38)->mA8();
}
// Retail symbol setSubObject___Q22cf11CfObjectEffFv (void params in the name)
// but the body consumes an object pointer in r4 - forced-name form.
extern "C" void setSubObject___Q22cf11CfObjectEffFv(cf::CfObjectEff* self, u8* arg) {
    if (self->mSubObj38 != 0) {
        // Redundant nested check on the same loaded value mirrors retail's two
        // beq targets (MWCC keeps both branches).
        if (self->mSubObj38 != 0) {
            reinterpret_cast<cf::CfObjectSub38*>(self->mSubObj38)->m08(1);
        }
        self->mSubObj38 = 0;
    }
    self->mSubObj38 = arg;
    if (arg != 0) {
        reinterpret_cast<cf::CfObjectSub38*>(arg)->m2C(reinterpret_cast<cf::CfObjectModel*>(self));
        reinterpret_cast<cf::CfObjectSub38*>(self->mSubObj38)->mA0();
    }
}
// Retail symbol setEffEnable___Q22cf11CfObjectEffFv (void params in the name)
// but the body consumes a bool flag in r4 - forced-name form.
extern "C" void setEffEnable___Q22cf11CfObjectEffFv(cf::CfObjectEff* self, bool flag) {
    if (flag)
        self->mFlags68 |= 0x100000;
    else
        self->mFlags68 &= ~0x100000;
}
// Retail symbol setChildFlag___Q22cf11CfObjectEffFv (void params in the name)
// but the body consumes a flag in r4 - forced-name form.
extern "C" void setChildFlag___Q22cf11CfObjectEffFv(cf::CfObjectEff* self, int flag) {
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

void setChildB59__(void* self, unsigned char val) {
    cf::CfObjectEff* obj = static_cast<cf::CfObjectEff*>(self);
    if (obj->mChildEff != nullptr)
        obj->mChildEff->unk59 = val;
}

void setChildScl__(cf::CfObject* self, float first, float second) {
    cf::CfObjectEff* eff = static_cast<cf::CfObjectEff*>(self);
    if (eff->mChildEff != nullptr)
        return func_804E3CDC(eff->mChildEff, first, second);
}

namespace cf {
extern "C" void setChildF50____Q22cf11CfObjectEffFv(cf::CfObjectEff* self, float v) {
    void* o = *(void**)((u8*)self + 0x94);
    if (o) *(float*)((u8*)o + 0x50) = v;
}
} // namespace cf

extern "C" void setChildF50G_(void* self, float v) {
    void* o = *(void**)((u8*)self + 0x94);
    if (o) *(float*)((u8*)o + 0x50) = v;
}

void setChildV40__(void* obj, const void* src) {
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
// Retail symbol setEffPosVec___Q22cf11CfObjectEffFv (void params in the name)
// but the body consumes a position-vector pointer in r4 - forced-name form.
extern "C" void setEffPosVec___Q22cf11CfObjectEffFv(cf::CfObjectEff* self, const cf::CfObjectEffU32Vec3* src) {
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

// ACCE4 is Eff's UVF25 override: forward to the base implementation with the
// same (pos, scale) ABI, then copy into the child.
extern "C" void CfObject_UnkVirtualFunc25__Q22cf8CfObjectFv(cf::CfObject* self,
                                                           ml::CVec3* pos,
                                                           float scale);
void func_800ACCE4__Q22cf11CfObjectEffFv(cf::CfObjectEff* self,
                                         cf::CfObjectEffU32Vec3* src,
                                         float scale) {
    CfObject_UnkVirtualFunc25__Q22cf8CfObjectFv(
        self, reinterpret_cast<ml::CVec3*>(src), scale);
    // x's zero-init is dead (overwritten below) but fixes its slot as the
    // first-created local; the visible loads all stay inside the copy block.
    u32 x = 0;
    cf::CfObjectEffChild* child = self->mChildEff;
    self->mFlags68 |= 0x100;
    // Early-return shape: the retail branch targets the shared epilogue.
    if (child == nullptr)
        return;
    x = src->x;
    u32 y = src->y;
    child->field_1C[0] = x;
    child->field_1C[1] = y;
    child->field_1C[2] = src->z;
}

namespace cf {
// Retail symbol moveEffOfs_____Q22cf11CfObjectEffFv (void params in the name)
// but the body consumes two floats in f1/f2 - forced-name form.
extern "C" void moveEffOfs_____Q22cf11CfObjectEffFv(cf::CfObjectEff* self, float a, float b) {
    float arr[3];
    arr[0] = a;
    arr[1] = lbl_eu_80666960;
    arr[2] = b;
    self->CfObject_UnkVirtualFunc25(reinterpret_cast<ml::CVec3*>(arr),
                                    lbl_eu_80666964);
}

// Retail symbol setEffRotVec___Q22cf11CfObjectEffFv (void params in the name)
// but the body consumes a rotation-vector pointer in r4 - forced-name form.
extern "C" void setEffRotVec___Q22cf11CfObjectEffFv(cf::CfObjectEff* self, const cf::CfObjectEffU32Vec3* src) {
    cf::CfObjectEffChild* child = self->mChildEff;
    if (child != nullptr) {
        *reinterpret_cast<u32*>(&child->field_28) = src->x;
        *reinterpret_cast<u32*>(&child->unk2C) = src->y;
        *reinterpret_cast<u32*>(&child->field_30) = src->z;
    }
    // Whole-vector word copy into the 0x48 tail (spanning mField4C).
    reinterpret_cast<cf::CfObjectEffVec48View*>(self)->vec48 = *src;
}
u8* CfObjectEff::getEffPosPtr_() {
    if (mChildEff != nullptr)
        return reinterpret_cast<u8*>(mChildEff) + 0x28;
    return reinterpret_cast<u8*>(this) + 0x48;
}
// Retail symbol setEffYPos_____Q22cf11CfObjectEffFv (void params in the name)
// but the body consumes a float in f1 - forced-name form.
extern "C" void setEffYPos_____Q22cf11CfObjectEffFv(cf::CfObjectEff* self, float a) {
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

float getEffYPos_____Q22cf11CfObjectEffFv(void* self) {
    cf::CfObjectEff* obj = static_cast<cf::CfObjectEff*>(self);
    if (obj->mChildEff != nullptr) {
        return obj->mChildEff->unk2C;
    }
    return reinterpret_cast<float&>(static_cast<cf::CfObject*>(obj)->mField4C);
}

namespace cf {
// Retail symbol name is scaleVirt29____Q22cf11CfObjectEffFv (void params in the
// name) but the body consumes a float scale argument in f1 - forced-name form.
extern "C" void scaleVirt29____Q22cf11CfObjectEffFv(cf::CfObjectEff* self, float value) {
    // Scale the value by the global factor, then dispatch through vtable
    // slot 0xC4 (CfObject_UnkVirtualFunc29 - float scale, matches the
    // retail occupant ACDFC(float) at that slot).
    self->CfObject_UnkVirtualFunc29(value * lbl_eu_8066A210);
}
float CfObjectEff::getScaledVal_() {
    float result = CfObject_UnkVirtualFunc31();
    return result * lbl_eu_8066A20C;
}
} // namespace cf

// Retail symbol setEffScale____Q22cf11CfObjectEffFv is Fv-mangled but the
// body consumes a float scale in f1 - defined at global scope so MWCC's
// mangled name still contains the retail symbol as a prefix.
void setEffScale____Q22cf11CfObjectEffFv(cf::CfObjectEff* self, float value) {
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

void setChild34Sc_(void* obj, const void* src) {
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

float getScaleComp___Q22cf11CfObjectEffFv(char* self) {
    cf::CfObjectEff* obj = reinterpret_cast<cf::CfObjectEff*>(self);
    if (obj->mChildEff != nullptr) {
        return reinterpret_cast<float&>(obj->mChildEff->unk34[1]);
    }
    return static_cast<cf::CfObject*>(obj)->mFloat60;
}

namespace cf {
// Retail symbol setEffLockFg___Q22cf11CfObjectEffFv (void params in the name)
// but the body consumes a bool flag in r4 - forced-name form.
extern "C" void setEffLockFg___Q22cf11CfObjectEffFv(cf::CfObjectEff* self, bool flag) {
    if (flag)
        self->mFlagsA4 |= 0x4;
    else
        self->mFlagsA4 &= ~0x4;
}
} // namespace cf

// r4 = object, r5 = partner. Stores both on the owner, then when a child
// effect exists and only the object is set, resolves the partner via
// func_800BB340 (a lookup) before writing it to the child.
void bindPartnerO_(cf::CfObjectEff* self, u8* object, u8* partner) {
    self->mFieldAC = partner;
    self->mField9C = object;
    if (self->mChildEff != nullptr) {
        if (object != nullptr && partner == nullptr) {
            partner = ((u8* (*)(u8*))func_800BB340)(object);
        }
        self->mChildEff->field_14 = partner;
    }
}
void setTargetObj_(cf::CfObjectEff* obj, u8* target) {
    if (obj->mChildEff != nullptr) {
        if (target != nullptr)
            obj->mChildEff->field_18 = ((u8* (*)(u8*))func_800BB340)(target);
        else
            obj->mChildEff->field_18 = nullptr;
    }
    obj->mFieldA0 = target;
}

void setChild5CFl_(char* obj, int flag) {
    cf::CfObjectEff* eff = reinterpret_cast<cf::CfObjectEff*>(obj);
    if (eff->mChildEff == nullptr) return;
    eff->mChildEff->unk5C = (flag != 0);
}

void createEffect_(cf::CfObjectEff* self) {
    // Gated by the game-manager flag; the effect type is packed in the top
    // 5 bits of mField70. The 0x10/0x11 types also require the source's +0xC
    // word to be clear, otherwise the whole update is skipped.
    if (reinterpret_cast<cf::CfGameManager*>(self)->getEffectFlagState() == 0) return;
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
    if (reinterpret_cast<cf::CfSourceHelper*>(
            reinterpret_cast<cf::CfObjectEffSourceView*>(source)->field_2C)->validate(source) == 0) {
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
            self->CfObject_UnkVirtualFunc19(
                reinterpret_cast<const ml::CVec3*>(&self->mPos3C));
            self->CfObject_UnkVirtualFunc27(reinterpret_cast<u8*>(self) + 0x48);
            self->CfObject_UnkVirtualFunc35(self->mFloat60);
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
            self->CObjectParam_UnkVirtualFunc1(
                reinterpret_cast<const char*>(result));
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

void decCounterA6_(void* obj) {
    cf::CfObjectEff* eff = static_cast<cf::CfObjectEff*>(obj);
    if (eff->mCountA6 == 0) return;
    eff->mCountA6--;
    if (eff->mCountA6 != 0) return;
    static_cast<cf::CfObject*>(eff)->mFlags68 |= 0x40;
}

namespace cf {
void CfObjectEff::updateEffect_() {
    // Dispatch through the ptmf table (0x18 bytes before the CfObjectEff
    // vtable) while the u16 counter at 0x8E stays below 2.
    if (mCount8E < 2) {
        (this->*lbl_eu_80528858[mCount8E])();
    }
    if (mSubObj38 != nullptr) {
        reinterpret_cast<cf::CfObjectSub38*>(mSubObj38)->mA4();
    }
    if (mChildEff != nullptr) {
        // Sync the child's flag bit 14 (0x4000) with slot-0x160's status
        // word. That slot (CfObject_UnkVirtualFunc68's occupant at +0x160 in
        // the complete-object table) is CfObjectEff::checkStsFlag_'s
        // address, but CfObject.hpp declares it with a placeholder void()
        // signature, so the view stays until that header is corrected.
        // `active` is materialized before the opaque virtual call, so MWCC
        // keeps the neg/or/srwi booleanize idiom and compares with xor.
        // Bitfield read converted to bool: MWCC emits the extrwi +
        // neg/or/srwi booleanize for this shape.
        bool flagSet = mChildEff->flag4000;
        int status = this->CfObject_UnkVirtualFunc68();
        if (flagSet ^ status) {
            status = this->CfObject_UnkVirtualFunc68();
            // Re-read mChildEff after the opaque virtual calls (retail reloads
            // the pointer), then insert the status flag into flag bit 14.
            CfObjectEffChildFlagsView* dst =
                reinterpret_cast<CfObjectEffChildFlagsView*>(mChildEff);
            // rlwimi merge of the status flag into bit 14 (retail shape).
            dst->field_00 = __rlwimi(dst->field_00, status, 14, 17, 17);
        }
    }
    if (mFieldA0 != nullptr && func_800B8920(mFieldA0) == 0) {
        mFieldA0 = nullptr;
        if (mChildEff != nullptr) {
            mChildEff->field_18 = nullptr;
        }
    }
}
int CfObjectEff::checkStsFlag_() {
    return ((*(u32*)((u8*)this + 0x68) >> 20) & 1);
}
} // namespace cf

// Chain node/mgr views for detachTrgPrt_: the manager returned by
// clearGimmickFlags holds a circular list whose sentinel lives at +0x4; each
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

// clearGimmickFlags is declared void in CfGameManager.hpp but retail returns the
// effect chain manager in r3; declare the retail-named entry point here (the
// header is read-only and the member cannot be redeclared with a return type).
extern "C" EffChainMgr* clearGimmickFlags__Q22cf13CfGameManagerFv(cf::CfGameManager* self);

// Iterates the effect chain, dynamic-casting each node's object to the base
// type at lbl_eu_806618F0 and poking its slot-0x198 method with `self`; the
// call's side effects are the point (both branches advance to the same node).
void notifyDetach_(u8* self) {
    cf::CfGameManager* gm = cf::CfGameManager::getInstance();
    EffChainMgr* mgr = clearGimmickFlags__Q22cf13CfGameManagerFv(gm);
    EffChainNode* node = mgr->sentinel->next;
    while (node != mgr->sentinel) {
        EffChainNode* next = node->next;
        // The dynamic_cast target is the CfObject base (lbl_eu_806618F0) at
        // offset 0, so the result pointer is the complete CfObjectEff*.
        // Slot +0x198 is CfObjectEff's own appended virtual
        // detachPrtnrA_ (+0x198) - call it directly on the derived type
        // (composite offset via the primary vptr, unadjusted this).
        void* obj = __dynamic_cast(node->object, 0, &lbl_eu_80661970, &lbl_eu_806618F0, 0);
        if (static_cast<cf::CfObjectEff*>(obj)->detachPrtnrA_(self) != 0) {
            node = next;
        } else {
            node = node->next;
        }
    }
}

void detachTrgPrt_(u8* arg) {
    if (arg == 0) return;
    cf::CfGameManager* gm = cf::CfGameManager::getInstance();
    EffChainMgr* mgr = clearGimmickFlags__Q22cf13CfGameManagerFv(gm);
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
void reattachTrg__(u8* arg0, u8* arg1) {
    if (arg0 == 0) return;
    cf::CfGameManager* gm = cf::CfGameManager::getInstance();
    EffChainMgr* mgr = clearGimmickFlags__Q22cf13CfGameManagerFv(gm);
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
// Retail symbol teardownEff____Q22cf11CfObjectEffFv (void params in the name)
// but the body consumes an object pointer in r4 - forced-name form.
// Release path: detaches the child effect, clears the 0xB0 object, pokes the
// 0x9C/0xA0 objects' slot-0x1BC (gated by the arg's 0x14 validity word and
// the live-object check), then drops the child and sets flag 0x40.
extern "C" void teardownEff____Q22cf11CfObjectEffFv(cf::CfObjectEff* self, u8* arg) {
    // Parent for the effect detach is this+0x90; retail null-guards the
    // offset so a null this passes 0 to func_804E3D48.
    u8* parent = reinterpret_cast<u8*>(self);
    if (self != nullptr) {
        parent = reinterpret_cast<u8*>(self) + 0x90;
    }
    func_804E3D48(self->mChildEff, parent);
    if (self->mFieldB0 != nullptr) {
        reinterpret_cast<cf::CfB0Helper*>(self->mFieldB0)->func0C(self);
        self->mFieldB0 = nullptr;
    }
    if (self->mField9C != nullptr &&
        reinterpret_cast<cf::CfObjectEffArg14View*>(arg)->field_14 != 0 &&
        func_800B8920(self->mField9C) != 0) {
        reinterpret_cast<cf::CfObjectModel*>(self->mField9C)->CfObjectModel_UnkVirtualFunc18(self);
    }
    if (self->mFieldA0 != nullptr && func_800B8920(self->mFieldA0) != 0) {
        reinterpret_cast<cf::CfObjectModel*>(self->mFieldA0)->CfObjectModel_UnkVirtualFunc18(self);
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
    this->CfObject_UnkVirtualFunc6();
}
// Retail symbol name is testFlagA4_____Q22cf11CfObjectEffFv (void params in the
// name) but the body consumes a mask argument in r4 - forced-name form.
extern "C" bool testFlagA4_____Q22cf11CfObjectEffFv(cf::CfObjectEff* self, u16 flags) {
    return (self->mFlagsA4 & flags) != 0;
}


} // namespace cf

// Forward declarations for symbols used by thunks
void teardownEff____Q22cf11CfObjectEffFv();
void __dt__Q22cf11CfObjectEffFv();

void thunkTear90____Q22cf11CfObjectEffFv(void* self) {
    ((void(*)(void*))teardownEff____Q22cf11CfObjectEffFv)((char*)self - 0x90);
}

void thunkDtor90____Q22cf11CfObjectEffFv(void* self) {
    ((void(*)(void*))__dt__Q22cf11CfObjectEffFv)((char*)self - 0x90);
}

// Container-of helper (retail symbol getEffOwner____FPv): obj points at a
// CfObject-family sub-object embedded at +0x3E9C inside its owner. Returns
// the owner when the +0x64 flag word has bit 4 set, else nullptr. The
// nested null guard returns `obj` (0 there) so MWCC tail-returns via
// `beqlr cr1`, matching retail.
void* getEffOwner__(void* obj) {
    if (obj != 0 && (static_cast<cf::CfObjectEffSubView*>(obj)->field_0x64 & 0x4) != 0) {
        if (obj == 0) return obj;
        return static_cast<u8*>(obj) - 0x3E9C;
    }
    return 0;
}

// Eff UVF22 (+0xA8): pure forward to UVF19. MWCC keeps r4 live → 0x10-byte
// bctr thunk (retail callVirt19).
extern "C" void callVirt19_____Q22cf11CfObjectEffFv(cf::CfObjectEff* self,
                                                    const ml::CVec3* vec) {
    self->CfObject_UnkVirtualFunc19(vec);
}

// Eff UVF26 (+0xB8): pure forward to UVF25. MWCC keeps r4/f1 live → 0x10-byte
// bctr thunk (retail callVirt25).
extern "C" void callVirt25_____Q22cf11CfObjectEffFv(cf::CfObjectEff* self,
                                                    ml::CVec3* pos,
                                                    float scale) {
    self->CfObject_UnkVirtualFunc25(pos, scale);
}

extern "C" void callVirt32_____Q22cf11CfObjectEffFv(cf::CfObjectEff* self) { self->CfObject_UnkVirtualFunc32(); }

extern "C" void callVirtC4_____Q22cf11CfObjectEffFv(cf::CfObjectEff* self, float value) { self->CfObject_UnkVirtualFunc29(value); }


namespace cf {
void CfObjectEff::cleanupEffct_() {
    if (mFieldB0 != nullptr) {
        reinterpret_cast<cf::CfB0Helper*>(mFieldB0)->func0C(this);
        mFieldB0 = nullptr;
    }
    if (mSubObj38 != nullptr) {
        reinterpret_cast<cf::CfObjectSub38*>(mSubObj38)->mAC();
        if (mSubObj38 != nullptr) {
            // Redundant nested check on the reloaded value mirrors retail's
            // two beq targets (MWCC keeps both branches).
            if (mSubObj38 != nullptr) {
                reinterpret_cast<cf::CfObjectSub38*>(mSubObj38)->m08(1);
            }
            mSubObj38 = nullptr;
        }
    }
    if (hasChildEffs_() != 0) {
        if (mField9C != nullptr) {
            reinterpret_cast<cf::CfObjectModel*>(mField9C)->CfObjectModel_UnkVirtualFunc18(this);
        }
        if (mFieldA0 != nullptr) {
            reinterpret_cast<cf::CfObjectModel*>(mFieldA0)->CfObjectModel_UnkVirtualFunc18(this);
        }
        detachChildEf();
    }
    mField70 = 0;
    mFlags68 &= 0x40000000;
}
} // namespace cf

// absorb: split1 retail data sections
// generated from retail object bytes (reloc-zeroed)

