// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectEff
// Replace stubs with high-level C/C++ during decomp.
#include "kyoshin/cf/CfCamSymbols.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectEff.hpp"
#include "kyoshin/CUIWindowManagerApi.hpp"
#include "kyoshin/cf/code_800F42AC.hpp"
#include "kyoshin/realtimeevt/CREvtEffect.hpp"
#include "kyoshin/cf/object/CfObjectModel.hpp"
#include "kyoshin/cf/IResInfo.hpp"
#include "libs/monolib/src/scn/CScnItemModel.hpp"

// Global camera-flag query (code_800F42AC.cpp; TU-local import).




namespace cf {
// TU-local view of the unmodeled base-class bytes written by the ctor
// (exact layout; member names are erased at codegen, same offsets/widths
// as the raw typed stores they replace).
struct CfEffCtorBaseView {
    u8 _pad00[0x04];
    u32 f04;                 // 0x04
    u32 f08;                 // 0x08
    u32 f0C;                 // 0x0C
    u8 f10;                  // 0x10
    u8 _pad11[0x30 - 0x11];
    u32 f30;                 // 0x30
    u32 f34;                 // 0x34
    u8 _pad38[0x48 - 0x38];
    float f48;               // 0x48
    u8 _pad4C[0x50 - 0x4C];
    float f50;               // 0x50
    float f54;               // 0x54
    float f58;               // 0x58
    float f5C;               // 0x5C
    u8 _pad60[0x6C - 0x60];
    u32 f6C;                 // 0x6C
    u8 _pad70[0x74 - 0x70];
    u32 f74;                 // 0x74
    u8 f78;                  // 0x78
    u8 _pad79[0x88 - 0x79];
    u32 f88;                 // 0x88
    u16 f8C;                 // 0x8C
    u16 f8E;                 // 0x8E
};
CfObjectEff::CfObjectEff() {
    // The retail base-ctor chain (CObjectState/CObjectParam/CfObject) is
    // inlined here: MWCC keeps the base vtable store and devirtualizes the
    // slot-0x5C call against the stored base vtable before the derived
    // vtable replaces it. The 0x70-0x8E tail is cleared in the same block.
    // Unmodeled base bytes are written as raw typed stores (same order and
    // widths as retail); modeled fields use their real members.
    CfEffCtorBaseView* v = reinterpret_cast<CfEffCtorBaseView*>(this);
    v->f04 = 0;
    v->f08 = 0;
    v->f0C = 0;
    v->f10 = 0;
    v->f30 = 0;
    v->f34 = 0;
    *(void**)this = (void*)&__vt__Q22cf8CfObject;
    mSubObj38 = nullptr;
    mPos3C = 0.0f;
    mPos40 = 0.0f;
    mPos44 = 0.0f;
    v->f48 = 0.0f;
    mField4C = 0.0f;
    v->f50 = 0.0f;
    v->f54 = 0.0f;
    v->f58 = 0.0f;
    v->f5C = 0.0f;
    mFloat60 = 1.0f;
    mFlags68 = 0;
    v->f6C = 0;
    mField70 = 0;
    v->f74 = 0;
    v->f78 = 0;
    v->f88 = 0;
    v->f8C = 0;
    v->f8E = 0;
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
    if (isGlobalCamFlagSet(0x04000000)) {
        mFlagsA4 |= 0x2;
    }
}
} // namespace cf

void cf::CfObject::CfObject_UnkVirtualFunc3() {}

namespace cf {
CfObject::~CfObject() {
    // Reset the vtable and run the slot-0x68 cleanup; MWCC auto-emits the
    // null guard, the vtable store and the delete-flag guard around it.
    // Slot +0x68 is CfObject_releaseMoveTargets (forwards to
    // CfObject_UnkVirtualFunc6, void() - matches the retail
    // devirtualized call through the just-stored base vtable).
    this->CfObject_releaseMoveTargets();
}
} // namespace cf

void cf::CfObject::CfObject_releaseMoveTargets() {}

namespace cf {
bool CfObjectEff::initEffState_() {
    CfObject_notifyEventDone();
    return true;
}
u32 CfObjectEff::hasChildEffs_() { return mChildEff != nullptr; }
void CfObjectEff::detachChildEf() {
    if (mChildEff != nullptr) {
        // Parent for the effect detach is this+0x90; retail null-guards the
        // offset so a null this passes 0 to schedDetachChildSlot.
        u8* parent = reinterpret_cast<u8*>(this);
        if (this != nullptr) {
            parent = reinterpret_cast<u8*>(&this->mSubObj90);
        }
        schedDetachChildSlot(mChildEff, parent);
        schedClearFlag15Update(mChildEff);
        mChildEff = nullptr;
    }
}
} // namespace cf

void cf::CfObjectModel::CfObjectModel_UnkVirtualFunc18() {}

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
    // Forwards to the sub-object's vtable slot +0xA8 (tail virtual call).
    // No-arg CScnItemModel::getEffectActOwner matches retail's leave-r4-live
    // bctr (same shape as CfObjectMove UVF7, FULL_MATCH).
    cf::CfObject* sub = (cf::CfObject*)mSubObj38;
    if (sub != 0) {
        reinterpret_cast<CScnItemModel*>(sub)->getEffectActOwner();
    }
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
        return schedSetStepIntervals(eff->mChildEff, first, second);
}

// TU-local typed view over the remaining raw base-tail offsets.
struct EffRotTailView { u8 _pad[0x48]; u32 w0; u32 w1; u32 w2; };
namespace cf {
extern "C" void setChildF50____Q22cf11CfObjectEffFv(cf::CfObjectEff* self, float v) {
    cf::CfObjectEffChild* child = self->mChildEff;
    if (child != nullptr) child->field_50 = v;
}
} // namespace cf

extern "C" void setChildF50G_(void* self, float v) {
    cf::CfObjectEffChild* child = static_cast<cf::CfObjectEff*>(self)->mChildEff;
    if (child != nullptr) child->field_50 = v;
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

// snapEffTarget is Eff's UVF25 override: forward to the base implementation with the
// same (pos, scale) ABI, then copy into the child.
void snapEffTarget__Q22cf11CfObjectEffFv(cf::CfObjectEff* self,
                                         cf::CfObjectEffU32Vec3* src,
                                         float scale) {
    self->CfObject::CfObject_snapMoveTarget(reinterpret_cast<ml::CVec3*>(src), scale);
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
    arr[1] = 0.5f;
    arr[2] = b;
    self->CfObject_snapMoveTarget(reinterpret_cast<ml::CVec3*>(arr),
                                    1000.0f);
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
    EffRotTailView* tail = reinterpret_cast<EffRotTailView*>(self);
    u32* dst48 = reinterpret_cast<u32*>(&tail->w0);
    dst48[0] = src->x;
    dst48[1] = src->y;
    dst48[2] = src->z;
}
u8* CfObjectEff::getEffPosPtr_() {
    if (mChildEff != nullptr)
        return reinterpret_cast<u8*>(&mChildEff->field_28);
    return reinterpret_cast<u8*>(&reinterpret_cast<EffRotTailView*>(this)->w0);
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
    // slot 0xC4 (CfObject_setMoveHeadAngle - float scale, matches the
    // retail occupant ACDFC(float) at that slot).
    self->CfObject_setMoveHeadAngle(value * ml::deg2rad);
}
float CfObjectEff::getScaledVal_() {
    float result = CfObject_getMoveHeadAngle();
    return result * ml::rad2deg;
}
} // namespace cf

void cf::CfObjectEff::setEffScale__(float value) {
    if (!(this->mFlagsA4 & 0x4)) {
        cf::CfObjectEffChild* child = this->mChildEff;
        if (child != nullptr) {
            // Word-copy the float bits into the child's three 0x34 slots; the
            // local's address spills it to the stack (stfs/lwz/stw) like retail.
            cf::CfObjectEffVec3 v;
            v.x = value;
            v.y = value;
            v.z = value;
            child->unk34[0] = *reinterpret_cast<u32*>(&v.x);
            child->unk34[1] = *reinterpret_cast<u32*>(&v.y);
            child->unk34[2] = *reinterpret_cast<u32*>(&v.z);
        }
        this->mFloat60 = value;
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
// CfModel_GetEffOwner (a lookup) before writing it to the child.
void bindPartnerO_(cf::CfObjectEff* self, u8* object, u8* partner) {
    self->mFieldAC = partner;
    self->mField9C = object;
    if (self->mChildEff != nullptr) {
        if (object != nullptr && partner == nullptr) {
            partner = ((u8* (*)(u8*))CfModel_GetEffOwner)(object);
        }
        self->mChildEff->field_14 = partner;
    }
}
void setTargetObj_(cf::CfObjectEff* obj, u8* target) {
    if (obj->mChildEff != nullptr) {
        if (target != nullptr)
            obj->mChildEff->field_18 = ((u8* (*)(u8*))CfModel_GetEffOwner)(target);
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

// TU-local view anchoring the rotation vector at +0x48 (CfObject pad48 /
// mField4C / +0x50 tail); used where the address alone is taken.
struct EffRotVecView { u8 _pad48[0x48]; ml::CVec3 rot; };
void createEffect_(cf::CfObjectEff* self) {
    // Gated by the game-manager flag; the effect type is packed in the top
    // 5 bits of mField70. The 0x10/0x11 types also require the source's +0xC
    // word to be clear, otherwise the whole update is skipped.
    if (reinterpret_cast<cf::CfGameManager*>(self)->getEffectFlagState() == 0) return;
    if (self->mFlags68 & 0x40) return;
    u32 type = self->mField70 >> 27;
    u8* source = nullptr;
    if (type == 0xe) {
        source = static_cast<u8*>(CfRes_getInstPtr224());
    } else if (type == 0xc) {
        source = static_cast<u8*>(CfRes_getInstPtr170());
    } else if (type == 0xf) {
        source = self->mFieldA8;
    } else if (type >= 0x10 && type <= 0x11) {
        source = self->mFieldA8;
        if (source != 0 &&
            reinterpret_cast<ResInfoEntry*>(source)->field_0x0C != 0) {
            return;
        }
    }
    if (source == 0) {
        self->mFlags68 |= 0x40;
        return;
    }
    bool valid = true;
    if (reinterpret_cast<ResInfoEntry*>(source)->field_0x2C->isInUse(source) == 0) {
        valid = false;
    }
    if (!valid) return;
    void* bdat = CfRes_findEntryById(source, self->mField70);
    if (bdat == 0) bdat = CfRes_findKypEntryA(source);
    if (bdat != 0 && lbl_eu_8065FC18 != 0) {
        // Drop dead partner objects before creating the effect.
        if (self->mField9C != 0 && lookupWorkAtAddr(self->mField9C) == 0) {
            self->mField9C = 0;
            self->mFlags68 |= 0x40;
            self->mCount8E++;
            return;
        }
        if (self->mFieldA0 != 0 && lookupWorkAtAddr(self->mFieldA0) == 0) {
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
                EffRoot_AddSchedReq(lbl_eu_8065FC18, bdat, mgr, self->mCount8C - 1, work, 0));
        }
        self->mField98 = static_cast<u8*>(bdat);
        if (self->mChildEff != 0) {
            self->CfObject_setMoveTargetVec(
                reinterpret_cast<const ml::CVec3*>(&self->mPos3C));
            self->CfObject_setRotVec(&reinterpret_cast<const EffRotVecView*>(self)->rot);
            self->CfObject_setObjScale(self->mFloat60);
            // The child may have been torn down by the slot calls above;
            // retail re-checks it before each partner write. The partner and
            // source-object loads are hoisted above the guard so MWCC emits
            // them before the branch, matching retail's schedule.
            u8* partner = self->mFieldAC;
            u8* f9c = self->mField9C;
            if (self->mChildEff != 0) {
                if (f9c != 0 && partner == 0) {
                    partner = static_cast<u8*>(CfModel_GetEffOwner(f9c));
                }
                self->mChildEff->field_14 = partner;
            }
            work = reinterpret_cast<u32>(self->mFieldA0);
            if (self->mChildEff != 0) {
                if (work != 0) {
                    self->mChildEff->field_18 = static_cast<u8*>(CfModel_GetEffOwner(reinterpret_cast<void*>(work)));
                } else {
                    self->mChildEff->field_18 = 0;
                }
            }
            self->mFieldA0 = reinterpret_cast<u8*>(work);
            void* result = schedFindEntryByKey(self->mChildEff);
            self->CObjectParam_setObjectName(
                reinterpret_cast<const char*>(result));
            self->mFieldB4 = 1;
            u8* parent = reinterpret_cast<u8*>(self);
            if (self != 0) parent = reinterpret_cast<u8*>(&self->mSubObj90);
            schedAttachChildSlot(self->mChildEff, parent);
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
        reinterpret_cast<cf::CfObjectSub38*>(mSubObj38)->updateSubEffect();
    }
    if (mChildEff != nullptr) {
        // Sync the child's flag bit 14 (0x4000) with slot-0x160's status
        // word. That slot's occupant in the complete-object table is
        // CfObjectEff::checkStsFlag_ (no args); the shared decl carries a
        // defaulted vec arg for the CtrlNpc/Model callers, so this TU reaches
        // it through the same-arity CfObject_checkSubReady alias (virtual
        // dispatch to +0x160, identical to retail's bctrl). Retail leaves r4
        // live across both calls (no li); the alias's default nullptr arg
        // materializes li r4,0 here (+8 bytes, the whole split overage) -
        // a known non-pad residual until the slot's arity is split per TU.
        // `active` is materialized before the opaque virtual call, so MWCC
        // keeps the neg/or/srwi booleanize idiom and compares with xor.
        // Bitfield read converted to bool: MWCC emits the extrwi +
        // neg/or/srwi booleanize for this shape.
        bool flagSet = mChildEff->flag4000;
        int status = this->CfObject_checkSubReady();
        if (flagSet ^ status) {
            status = this->CfObject_checkSubReady();
            // Re-read mChildEff after the opaque virtual calls (retail reloads
            // the pointer), then insert the status flag into flag bit 14.
            u16* dst = reinterpret_cast<u16*>(mChildEff);
            // rlwimi merge of the status flag into bit 14 (retail shape).
            dst[0] = __rlwimi(dst[0], status, 14, 17, 17);
        }
    }
    if (mFieldA0 != nullptr && lookupWorkAtAddr(mFieldA0) == 0) {
        mFieldA0 = nullptr;
        if (mChildEff != nullptr) {
            mChildEff->field_18 = nullptr;
        }
    }
}
int CfObjectEff::checkStsFlag_() {
    return ((mFlags68 >> 20) & 1);
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

// Iterates the effect chain, downcasting each node's CfObject to CfObjectEff
// and calling detachPrtnrA_ (slot 0x198) with `self`; the call's side effects
// are the point (both branches advance to the same node).
void notifyDetach_(u8* self) {
    cf::CfGameManager* gm = cf::CfGameManager::getInstance();
    EffChainMgr* mgr = clearGimmickFlags__Q22cf13CfGameManagerFv(gm);
    EffChainNode* node = mgr->sentinel->next;
    while (node != mgr->sentinel) {
        EffChainNode* next = node->next;
        cf::CfObjectEff* obj = dynamic_cast<cf::CfObjectEff*>(reinterpret_cast<cf::CfObject*>(node->object));
        if (obj->detachPrtnrA_(self) != 0) {
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
// TU-local view of the +0x14 validity word teardownEff__ reads off its
// u8* argument (exact layout; replaces the raw offset deref). Valid
// objects carry a non-zero word here (cf. getEffOwner__'s +0x64 flag word).
struct EffTeardownArgView {
    u8 _pad[0x14];
    u32 valid;  // 0x14
};
extern "C" void teardownEff____Q22cf11CfObjectEffFv(cf::CfObjectEff* self, u8* arg) {
    // Parent for the effect detach is this+0x90; retail null-guards the
    // offset so a null this passes 0 to schedDetachChildSlot.
    u8* parent = reinterpret_cast<u8*>(self);
    if (self != nullptr) {
        parent = reinterpret_cast<u8*>(&self->mSubObj90);
    }
    schedDetachChildSlot(self->mChildEff, parent);
    if (self->mFieldB0 != nullptr) {
        reinterpret_cast<cf::CPcEffectBank*>(self->mFieldB0)->release(self);
        self->mFieldB0 = nullptr;
    }
    if (self->mField9C != nullptr &&
        reinterpret_cast<EffTeardownArgView*>(arg)->valid != 0 &&
        lookupWorkAtAddr(self->mField9C) != 0) {
        reinterpret_cast<cf::CfObjectModel*>(self->mField9C)->CfObjectModel_detachEffectSlot(self);
    }
    if (self->mFieldA0 != nullptr && lookupWorkAtAddr(self->mFieldA0) != 0) {
        reinterpret_cast<cf::CfObjectModel*>(self->mFieldA0)->CfObjectModel_detachEffectSlot(self);
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
    this->CfObject_releaseMoveTargets();
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
    if (obj != 0 && (static_cast<cf::CfObject*>(obj)->unk64 & 0x4) != 0) {
        if (obj == 0) return obj;
        return static_cast<u8*>(obj) - 0x3E9C;
    }
    return 0;
}

// Eff UVF22 (+0xA8): pure forward to UVF19. MWCC keeps r4 live -> 0x10-byte
// bctr thunk (retail callVirt19).
extern "C" void callVirt19_____Q22cf11CfObjectEffFv(cf::CfObjectEff* self,
                                                    const ml::CVec3* vec) {
    self->CfObject_setMoveTargetVec(vec);
}

// Eff UVF26 (+0xB8): pure forward to UVF25. MWCC keeps r4/f1 live -> 0x10-byte
// bctr thunk (retail callVirt25).
extern "C" void callVirt25_____Q22cf11CfObjectEffFv(cf::CfObjectEff* self,
                                                    ml::CVec3* pos,
                                                    float scale) {
    self->CfObject_snapMoveTarget(pos, scale);
}

extern "C" void callVirt32_____Q22cf11CfObjectEffFv(cf::CfObjectEff* self, float deg) { self->CfObject_syncMoveHead(deg); }

extern "C" void callVirtC4_____Q22cf11CfObjectEffFv(cf::CfObjectEff* self, float value) { self->CfObject_setMoveHeadAngle(value); }


namespace cf {
void CfObjectEff::cleanupEffct_() {
    if (mFieldB0 != nullptr) {
        reinterpret_cast<cf::CPcEffectBank*>(mFieldB0)->release(this);
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
            reinterpret_cast<cf::CfObjectModel*>(mField9C)->CfObjectModel_detachEffectSlot(this);
        }
        if (mFieldA0 != nullptr) {
            reinterpret_cast<cf::CfObjectModel*>(mFieldA0)->CfObjectModel_detachEffectSlot(this);
        }
        detachChildEf();
    }
    mField70 = 0;
    mFlags68 &= 0x40000000;
}
} // namespace cf

// absorb: split1 retail data sections
// generated from retail object bytes (reloc-zeroed)

