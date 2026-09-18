// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectMove
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"
#define CfObjectMove_setRegionAttached CfObjectMove_setRegionAttached_hidden
#include "kyoshin/cf/CfResObjImpl.hpp"
#undef CfObjectMove_setRegionAttached
// Owning-class virtuals for the +0xC4 target (CActParamAnim family: this TU's
// mTargetC4 is the +0xD0 CActParamAnimGame). Five of this header's canonical
// decls clash with TU-visible forms (same clash family as
// CModelDispEquip.hpp's hidden form): 5-arg func_8004B9D4 vs the TU-local
// 4-arg form, s32-id Scn_FindCamItem vs the TU-local int-index form, int
// ScnRes_VertRayForward_E398 vs harness_catalog's void* form, void* simGetLeafActData vs the
// harness form, and float lbl_eu_8066A200 vs the const-f32 form. None is used
// by inline code in that header (decls only), so hide them for the include;
// all TU call sites resolve after the #undefs and are unaffected.
#define func_8004B9D4 func_8004B9D4_hidden
#define Scn_FindCamItem Scn_FindCamItem_hidden
#define ScnRes_VertRayForward_E398 ScnRes_VertRayForward_E398_hidden
#define simGetLeafActData simGetLeafActData_hidden
#define lbl_eu_8066A200 lbl_eu_8066A200_hidden
#include "kyoshin/action/CActParamAnim.hpp"
#undef func_8004B9D4
#undef Scn_FindCamItem
#undef ScnRes_VertRayForward_E398
#undef simGetLeafActData
#undef lbl_eu_8066A200
#include "monolib/math.hpp"
#include <nw4r/math.h>
#include <math.h>

// Novtable dispatch view of cf::CfCam through slot +0x60 (cfCam_loadUnk164).
// Full CfCam.hpp cannot be included in this TU: its extern "C" imports clash
// with CActParamAnim.hpp (copyVec3Words / func_804BE4E0 return types). Slot
// names and arity match the owning class; never constructed here.
namespace cf {
class __declspec(novtable) CfCam {
public:
    virtual ~CfCam();                               // 0x08
    virtual void func_8006CC68(int arg);            // 0x0C
    virtual void* cfCam_getPlus1C();                  // 0x10
    virtual void cfCam_copyToPlus28(void* src);          // 0x14
    virtual void cfCam_updFollowPos(void* arg, float f); // 0x18
    virtual void* cfCam_nestedPlus118();                  // 0x1C
    virtual void func_8006E884(float f);            // 0x20
    virtual void cfCam_nopVirt24();                   // 0x24
    virtual void func_8006BFDC();                   // 0x28
    virtual void* cfCam_copyToPlus10(void* src);         // 0x2C
    virtual void* cfCam_getPlus10();                  // 0x30
    virtual void* cfCam_getPlus40();                  // 0x34
    virtual void cfCam_initFollowS(void* src);          // 0x38
    virtual void cfCam_setFollowD(float f);            // 0x3C
    virtual void cfCam_storeUnk08(int arg);            // 0x40
    virtual void* cfCam_returnZero();                  // 0x44
    virtual void cfCam_copyCamState(CfObject* src);      // 0x48
    virtual void camPluginNoopV4C();                   // 0x4C
    virtual void camPluginNoopV50();                   // 0x50
    virtual void camPluginNoopV54();                   // 0x54
    virtual float cfCam_get1E0Again();                  // 0x58
    virtual void cfCam_setFollowTg(void* target);       // 0x5C
    virtual void* cfCam_loadUnk164();                  // 0x60
};
} // namespace cf

// Shared Ctrl* movement-target prefix (CtrlEnemy/Npc/Pc/Pad/Remote):
// dtor at +0x08, reset at +0x0C (CtrlEnemy_UnkFunc_8008772C family). Two
// real virtuals only - no dummy pad slots. Used for mTarget6C0 +0x0C.
namespace cf {
class __declspec(novtable) CtrlMoveTarget {
public:
    virtual ~CtrlMoveTarget();
    virtual void resetMoveTarget();  // +0x0C
};
} // namespace cf


// toggle bits there; same offset as the CActParamAnim flag view).
struct CfObjectMoveC4Flags {
    u8 _pad[0xC];
    u32 flags;  // 0xC
};

// One-arg call form of the CActParamAnim translation helper. Declared in
// CfObjectMove.hpp with C linkage (retail unmangled name).


namespace cf {

CfObjectMove::CfObjectMove() {
    // Base CfObjectModel ctor runs first; novtable (header) suppresses the
    // implicit vptr store, so the retail label is assigned explicitly. Then
    // the +0xD0 CActParamAnimGame sub-object and the +0x60C region are
    // constructed, the flags/fields are initialized (mFlags6C9 becomes
    // 0x14: top3 cleared, bits 4-1 = 0b1010, bit 0 cleared), the
    // 0x70C/0x710/0x6F8 arrays are zeroed, and the base vtable slot +0x18
    // (CObjectState_applyStateFlags) is invoked with flag 3.
    *(void**)this = (void*)lbl_eu_80529690;
    mTargetC4 = 0;
    __ct__Q22cf17CActParamAnimGameFv((cf::CActParamAnimGame*)((u8*)this + 0xD0));
    ColiNodeInit(_60C_region);
    mTarget6C0 = 0;
    mField6C4 = 1;
    mField6C8 = -1;
    u8 flags = mFlags6C9;
    ((cf::CfFlags6C9*)&flags)->top3 = 0;   // rlwinm r0,r0,0,27,23
    ((cf::CfFlags6C9*)&flags)->mid4 = 5;   // li r3,5; rlwimi r0,r3,1,27,30
    ((cf::CfFlags6C9*)&flags)->b0 = 0;     // clrrwi r0,r0,1
    mFlags6C9 = flags;
    _6CA = -1;
    field_6CC = 1;
    field_6CE = -1;
    field_6CF = -1;
    _6D0 = 0;
    mField6D4 = 0;
    mField6D8 = 0;
    mField6DC = 0;
    float f = lbl_eu_80666A88;
    mField6E4 = f;
    mField6E8 = f;
    mField6EC = f;
    mMoveSpeed = f;
    _6F4 = 0;
    mField700 = 0;
    mField704 = 0;
    mField708 = 0;
    _714 = 0;
    mField718 = f;
    mTargetCC = 0;
    mTargetC8 = 0;
    memset(field_70C, 0, sizeof(field_70C));
    memset(field_710, 0, sizeof(field_710));
    memset(mField6F8, 0, sizeof(mField6F8));
    CObjectState_applyStateFlags(3);
}

cf::CfObjectMove::~CfObjectMove() {
    // Explicit vtable restore, virtual teardown (slot +0x68), region detach,
    // then the hand-rolled member/base destructor chain (region at +0x60C,
    // CActParamAnimGame at +0xD0, CfObjectModel base) with the retail flag
    // literals; MWCC appends the delete-flag wrapper itself.
    *(void**)this = (void*)lbl_eu_80529690;
    CfObject_releaseMoveTargets();
    if (lbl_eu_80665958 != 0) {
        Coli_ListRemoveNode(lbl_eu_80665958, (void*)this->_60C_region);
    }
    __dt__804B095C((void*)this->_60C_region, -1);
    __dt__Q22cf17CActParamAnimGameFv((void*)((u8*)this + 0xD0), -1);
}

// Wave-63: override of CfObject_runMoveUpdate (+0x60). Free-function Fv
// form keeps the Unk linker name that hand-built tables reference.
extern "C" void CfObject_UnkVirtualFunc4__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    // Per-frame movement update: run the base position/state pair, then (when
    // no menu/battle state is active) refresh the movement (func_800BCD04) and
    // release the two +0xC8/+0xCC targets. With a model sub-object present the
    // +0x1AC slot forwards the +0xB4/+0xB8 words, and the +0x38 sub-object
    // receives the mode-dependent advance notification before the movement
    // value is fed to the CCharEffect slot helper.
    self->CfObject_refreshSubB0();
    self->CfObject_updateMoveRate();
    if (func_800829B8__Q22cf13CfGameManagerFv() == 0) {
        func_800BCD04(self);
        // Pointer-induction release walk (retail steps the slot pointer by 4
        // with a separate counter).
        void** slot;
        int i = 0;
        slot = &self->mTargetC8;
        while (i < 2) {
            if (*slot != 0) {
                tickAnimFrame(*slot);
            }
            i++;
            slot++;
        }
    }
    if (CfModel_GetFlag7A8(self) != 0) {
        u32 b4 = ((cf::CfObjectMoveB4View*)self)->field_B4;
        if (b4 != 0) {
            self->CfObjectModel_bindModelTo((cf::CfObject*)b4, (const char*)((cf::CfObjectMoveB4View*)self)->field_B8);
        }
    }
    int r30 = 0;
    int cond = 0;
    int r28 = 0;
    int r27 = 0;
    if (self->mSubObj38 != 0) {
        // Global-mode gate: the two flag reads are kept separate (volatile
        // casts; the extern itself is non-volatile to match CSystemWindow.hpp
        // and the CUICfManager.cpp definition) so MWCC emits the retail double
        // lwz of lbl_eu_80663E24 (r0 for the mode test, r5 for the +0x40000
        // advance gate).
        u32 g0 = *(volatile u32*)&lbl_eu_80663E24;
        u32 g1 = *(volatile u32*)&lbl_eu_80663E24;
        cond = (g0 & 0x02040000) != 0;
        if ((g0 & 0x10000000) != 0) {
            cond = 0;
        }
        if (cond == 0) {
            ((cf::CfObject*)self->mSubObj38)->CfObject_setPosXYZ();
        } else if (((self->unk64 & 0x8) != 0 || (self->unk64 & 0x4) != 0) && (g1 & 0x40000) != 0) {
            ((cf::CfObject*)self->mSubObj38)->CfObject_setPosXYZ();
        }
        if ((self->unk64 & 0x2) != 0 || (self->unk64 & 0x4) != 0) {
            if (self->mSubObj98 != 0) {
                r28 = (self->mSubObj98->field_7A4 & 0x04000000) == 0;
            }
            if (((cf::CfObject*)self->mSubObj38)->CfObject_getCharEffect() != 0) {
                if (self->mSubObj98 != 0) {
                    if (((CScnItemModel*)self->mSubObj98)->vfunc4C() > lbl_eu_80666A8C) {
                        r27 = 1;
                    }
                }
if (cond == 0 && self->CfObject_checkSubReady() != 0 && r28 != 0 && r27 == 0) {
                    r30 = 1;
                }
                int q = ((cf::CfObject*)self->mSubObj38)->CfObject_getCharEffect();
                func_8015BD24((void*)q, r30);
            }
        }
    }
}


// Wave-63: override of CfObject_notifySubEffect (+0x6C). Free-function Fv
// form keeps the Unk linker name that hand-built tables reference.
extern "C" void CfObject_UnkVirtualFunc7__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    // Forward to the sub-object's vtable slot +0xA8 (tail virtual call).
    // No-arg CScnItemModel::getEffectActOwner matches retail's leave-r4-live
    // bctr (CfObject::syncMoveTarget would load r4).
    cf::CfObject* sub = (cf::CfObject*)self->mSubObj38;
    if (sub != 0) {
        reinterpret_cast<CScnItemModel*>(sub)->getEffectActOwner();
    }
}

// Wave-62: override of CfObject_releaseMoveTargets (+0x68). Free-function
// Fv form keeps the Unk linker name that hand-built tables / CREvtModel
// reference; the renamed member decl serves call-site codegen.
extern "C" void CfObject_UnkVirtualFunc6__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    // Teardown pair (vtable +0x17C then +0x178), then release the C4 target
    // (vtable +0xE0) and the NPC movement target (vtable +0x8, flag 1),
    // clearing both pointers.
    self->CfObjectModel_releaseModelList();  // vtable +0x17C
    self->CfObjectModel_releaseModelSub();  // vtable +0x178
    if (self->mTargetC4 != 0) {
        // C4 target is the +0xD0 CActParamAnimGame: +0xE0 is CActParamAnim::
        // func_8004B114 (retail table lbl_eu_80526458, method 54).
        reinterpret_cast< ::CActParamAnim*>(self->mTargetC4)->func_8004B114();
        self->mTargetC4 = 0;
    }
    if (self->mTarget6C0 != 0) {
        // Redundant nested check mirrors retail's two beq targets.
        if (self->mTarget6C0 != 0) {
            reinterpret_cast<cf::CObjectState*>(self->mTarget6C0)->CObjectState_setStateBitMask(1);
        }
        self->mTarget6C0 = 0;
    }
}

// Wave-63/64: override of CfObjectModel_releaseModelSub (+0x178). Free-
// function Fv keeps the Unk linker name that hand-built tables / CREvtModel
// reference. Base impl is the non-virtual UnkVirtualFunc1 out-of-line body
// on CfObjectModel (novtable; virtual primary is releaseModelSub).
extern "C" void CfObjectModel_UnkVirtualFunc1__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    self->CfObjectMove_releaseSlotById(0);
    self->CfObjectMove_releaseSlotById(1);
    for (int i = 0; i < 2; i++) {
        void* t = (&self->mTargetC8)[i];
        // Redundant nested checks mirror retail's three beq targets (MWCC
        // keeps them; the final clear only runs when the first two pass).
        if (t != 0) {
            if (t != 0) {
                if (t != 0) {
                    reinterpret_cast<cf::CObjectState*>(t)->CObjectState_setStateBitMask(1);
                }
                (&self->mTargetC8)[i] = 0;
            }
        }
    }
    cf::CfObject* sub = (cf::CfObject*)self->mSubObj38;
    if (sub != 0 && self->mFieldC0 != 0) {
        if (sub->CfObject_getCharEffect() != 0) {
            int r = ((cf::CfObject*)self->mSubObj38)->CfObject_getCharEffect();
            CharEffDetachBySubId((void*)r, self->mSubObj98);
        }
    }
    self->CfObjectModel::CfObjectModel_UnkVirtualFunc1();
    self->mFlags68 &= ~0x4;
    ColiNodeReleaseBuffer(self->_60C_region);
    if (self->mTargetC4 != 0) {
        // Same owning-class slot as above (CActParamAnim +0xE0).
        reinterpret_cast< ::CActParamAnim*>(self->mTargetC4)->func_8004B114();
        self->mTargetC4 = 0;
    }
}


// Wave-61: override of CfObjectModel_releaseModelList (+0x17C). Free-function
// Fv form keeps the Unk linker name that hand-built tables / CREvtModel
// reference; the widened member decl in the header serves call-site codegen.
extern "C" void CfObjectModel_UnkVirtualFunc2__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    // Refresh the two animation slots (+0x1CC before the release work,
    // +0x1C8 after), releasing both model lists (+0x9C from the base class
    // and +0x6D4) through the C4 target, then hop the +0xB0 sub-object's
    // vtable slot +0x24.
    self->CfObjectMove_detachModelList();
    // field_0x9C is the u8[4] base-class slot; access it as the pointer it
    // holds (model list released through the C4 target).
    void*& list9C = *reinterpret_cast<void**>(self->field_0x9C);
    if (list9C != 0) {
        if (self->mTargetC4 != 0) {
            releaseAnimObj(self->mTargetC4, list9C);
        }
        Scn_IsAnimActiveOrNull(list9C);
        list9C = 0;
    }
    if (self->mField6D4 != 0) {
        if (self->mTargetC4 != 0) {
            releaseAnimObj(self->mTargetC4, self->mField6D4);
        }
        Scn_IsAnimActiveOrNull(self->mField6D4);
        self->mField6D4 = 0;
    }
    // Base +0x17C: retail reloc spells UnkVirtualFunc2 (Model's empty stub);
    // call the forced-name Fv so the reloc name matches.
    CfObjectModel_UnkVirtualFunc2__Q22cf13CfObjectModelFv(self);
    self->CfObjectMove_dropMoveRequest();
    reinterpret_cast<cf::CfResObjImpl*>(self->mSubObjB0)->CfResObj_noop24();
}


void cf::CfObjectMove::CfObjectMove_detachModelList() {
    // When the model list (0x6D8) is present, notify each of the two
    // +0xC8/+0xCC targets, release the list, and clear the list fields.
    if (mField6D8 != 0) {
        for (int i = 0; i < 2; i++) {
            void* t = (&mTargetC8)[i];
            if (t != 0) {
                releaseAnimObj(t, mField6D8);
            }
        }
        Scn_IsAnimActiveOrNull(mField6D8);
        mField6D8 = 0;
        mField704 = 0;
    }
}

void cf::CfObjectMove::CfObjectMove_dropMoveRequest() {
    // Forward to the +0xB0 sub-object's vtable slot +0x44, then clear the
    // two resource-request fields (retail stores 0 to +0x6DC / +0x6E0).
    reinterpret_cast<cf::CfResObjImpl*>(*(void**)((u8*)this + 0xb0))->CfResObj_noop44();
    mField6DC = 0;
    mField6E0 = 0;
}



// Retail CfObjectMove_resetMoveSpeed: clear bits 0xC00 of mFlags68 (rlwinm r0,r0,0,22,19)
// and reset the movement speed to the shared constant. (The old
// setMoveSpeed/resetMoveSpeed method bodies duplicated the already-FULL
// CfObjectMove_setMoveSpeed twin and this retail body; retail keeps one unmangled copy.)
extern "C" void CfObjectMove_resetMoveSpeed(cf::CfObjectMove* self) {
    u32 flags = self->mFlags68;
    f32 value = lbl_eu_80666A88;
    self->mMoveSpeed = value;
    self->mFlags68 = flags & ~0xC00;
}




// Wave-62: override of CfObject_updateMoveRate (+0x64). Free-function Fv
// form keeps the Unk linker name that hand-built tables / CREvtModel
// reference; the renamed member decl serves call-site codegen.
extern "C" void CfObject_UnkVirtualFunc5__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    float v = Scn_GetFrameDelta(lbl_eu_80663E14);
    cf::CfObjectMoveA8View* view = (cf::CfObjectMoveA8View*)self;
    // The flag word is read once for the direction tests (kept in a local)
    // while the clamp-path clears re-read the member - retail reloads
    // mFlags68 (lwz r0,0x68) at each clear, so keep the test value separate.
    // :: on the constants avoids the namespace-cf copy (mangled
    // lbl_eu_80666A88__2cf) so the SDA relocs keep the retail names.
    u32 flags = self->mFlags68;
    if ((flags & 0x4000) != 0) {
        // Decreasing state: step the +0xA4 rate down toward the low clamp.
        // The clamp-path clears go through a separate view type so MWCC
        // reloads mFlags68 (retail lwz r0,0x68) instead of reusing the test
        // register. :: on the constants avoids the namespace-cf copy.
        float f = view->field_A4 - lbl_eu_80666AB4 * v;
        view->field_A4 = f;
        if (f <= ::lbl_eu_80666A88) {
            view->field_A4 = ::lbl_eu_80666A88;
            ((cf::CfObjectMoveFlags68View*)self)->flags68 &= ~0x4000;
            if (lbl_eu_80665958 != 0) {
                Coli_ListRemoveNode(lbl_eu_80665958, self->_60C_region);
            }
        }
    } else if ((flags & 0x2000) != 0) {
        // Increasing state: step the +0xA4 rate up toward the high clamp.
        float f = view->field_A4 + lbl_eu_80666AB4 * v;
        view->field_A4 = f;
        if (f >= lbl_eu_80666A94) {
            view->field_A4 = lbl_eu_80666A94;
            ((cf::CfObjectMoveFlags68View*)self)->flags68 &= ~0x2000;
            if (lbl_eu_80665958 != 0) {
                func_804B4BDC(lbl_eu_80665958, self->_60C_region);
            }
        }
    }
    if (self->mSubObj98 != 0) {
        float spd = self->mMoveSpeed;
        if (spd == ::lbl_eu_80666A88) {
            spd = lbl_eu_80666A94;
        }
        float factor = lbl_eu_80666A94 / spd;
        u32 flags2 = self->mFlags68;
        if ((flags2 & 0x800) != 0) {
            // Move the +0xA0 position toward the high clamp.
            float f = factor * v + self->field_A0;
            self->field_A0 = f;
            if (f >= lbl_eu_80666A94) {
                self->field_A0 = lbl_eu_80666A94;
                self->mFlags68 &= ~0x800;
            }
        } else if ((flags2 & 0x400) != 0) {
            // Move the +0xA0 position toward the low clamp.
            float f = self->field_A0 - factor * v;
            self->field_A0 = f;
            if (f <= ::lbl_eu_80666A88) {
                self->field_A0 = ::lbl_eu_80666A88;
                self->mFlags68 &= ~0x400;
            }
        }
        func_800BC8D8(self);
    }
}







// Retail symbol is Fv but the body consumes r4 (a target pointer) -
// forced-name form carrying the hidden arg (same scheme as
// CfObject_setMoveReadyFlag). When a 6C0 target exists and differs from the
// incoming target, release the old target (vtable +0x8, flag 1) and clear
// the slot; the redundant nested checks mirror retail's two beq targets.
// Then, when a C4 target exists, set the 6C0 slot to the incoming target,
// run the position query (vtable +0x138) and write its x component into the
// C4 target's scale fields (0x430/0x43C, then 0x434 scaled by
// lbl_eu_80666AD0).
void CfObject_setMoveTargetPtr__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, void* arg) {
    if (self->mTarget6C0 != 0) {
        if (arg != self->mTarget6C0) {
            if (self->mTarget6C0 != 0) {
                if (self->mTarget6C0 != 0) {
                    reinterpret_cast<cf::CObjectState*>(self->mTarget6C0)->CObjectState_setStateBitMask(1);
                }
                self->mTarget6C0 = 0;
            }
            self->mTarget6C0 = 0;
        }
    }
    if (self->mTargetC4 != 0) {
        setAnimOwner(self->mTargetC4, arg);
        self->mTarget6C0 = arg;
        float x = *(float*)self->CfObject_getMoveRateScale();
        cf::CfObjectMoveTargetC4* c4 = (cf::CfObjectMoveTargetC4*)self->mTargetC4;
        c4->field_430 = x;
        c4->field_43C = c4->field_438 * x;
        float y = *(float*)self->CfObject_getMoveRateScale();
        cf::CfObjectMoveTargetC4* c4b = (cf::CfObjectMoveTargetC4*)self->mTargetC4;
        c4b->field_434 = lbl_eu_80666AD0 * y;
    } else {
        self->mTarget6C0 = arg;
    }
}

// Retail symbol is Fv but the body consumes r4 (the action/kind id) -
// forced-name form carrying the hidden arg (same scheme as
// CfObject_setMoveTargetPtr/49). extern "C" so the definition symbol is the
// exact retail name. Allocates and constructs the movement-target object for
// the id, then dispatches it through the vtable +0x108 slot.
extern "C" void CfObject_createMoveTarget__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, int arg) {
    // Two separate guards (retail emits two beq targets to the epilogue; a
    // single ||-condition adds an extra branch).
    if (self->_6CA == arg) {
        return;
    }
    if (arg == -1) {
        return;
    }
    self->_6CA = (s16)arg;
    // obj declared (and zeroed) before the heap query so MWCC colours it
    // into r31 and the heap handle into r30, as in retail.
    void* obj = 0;
    u32 heap = CfRes_getHeapHandle();
    if (arg == 8) {
        // Single target variable: retail assigns the allocation result
        // straight into the tracked slot (no intermediate temporary).
        obj = allocate__Q23mtl10MemManagerFUlUl(0x224, CfRes_getAllocHandle());
        if (obj != 0) {
            __ct__cf_CtrlEnemy(obj, self);
        }
    } else if (arg == 9) {
        obj = allocate__Q23mtl10MemManagerFUlUl(0x180, CfRes_getAllocHandle());
        if (obj != 0) {
            __ct__CtrlNpc(obj, self);
        }
    } else if (arg == 10) {
        obj = allocate__Q23mtl10MemManagerFUlUl(0x1DC, CfRes_getHeapHandle());
        if (obj != 0) {
            __ct__cf_CtrlPc(obj, self, 0);
        }
    } else {
        float f = self->CfObject_getMoveHeadAngle();
        if (arg >= 0 && arg <= 3) {
            obj = allocate__Q23mtl10MemManagerFUlUl(0x264, heap);
            if (obj != 0) {
                __ct__CtrlRemote(obj, self, arg);
            }
        } else if (arg >= 4 && arg <= 7) {
            obj = allocate__Q23mtl10MemManagerFUlUl(0x264, heap);
            if (obj != 0) {
                __ct__cf_CtrlPad(obj, self, arg);
            }
        }
        if (self->mTarget6C0 != 0) {
            *(float*)((char*)self->mTarget6C0 + 0xC) = f;
        }
    }
    self->CfObject_setMoveTargetPtr(obj);
}

// Retail symbol is Fv but the body consumes r4 (an action id forwarded to
// the NPC movement target's action setter) - forced-name form carrying the
// hidden arg. When a 6C0 target exists, record the target's +0xC0 action id
// and the incoming id in the +0x6CF/+0x6CE bytes, then dispatch the id to
// func_8009377C (ids 1,2,3,5,4 map to themselves; anything else maps to 0).
void CfObject_forwardNpcAction__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, int arg) {
    cf::CtrlNpc* target = (cf::CtrlNpc*)self->mTarget6C0;
    if (target != 0) {
        self->field_6CF = (u8)((cf::CfObjectMoveNpcView*)target)->field_C0;
        self->field_6CE = (u8)arg;
        if (arg == 1) {
            func_8009377C(target, 1);
        } else if (arg == 2) {
            func_8009377C(target, 2);
        } else if (arg == 3) {
            func_8009377C(target, 3);
        } else if (arg == 5) {
            func_8009377C(target, 5);
        } else if (arg == 4) {
            func_8009377C(target, 4);
        } else {
            func_8009377C(target, 0);
        }
    }
}

void cf::CfObjectMove::CfObject_setMoveBusyState(int flag) {
    if (flag != 0) {
        mFlags68 |= 0x01000000;
    } else {
        mFlags68 &= ~0x01000000u;
    }
    void* target = mTargetC4;
    if (target != 0) {
        CfObjectMoveC4Flags* view = (CfObjectMoveC4Flags*)target;
        if (flag != 0) {
            view->flags |= 0x40;
        } else {
            view->flags &= ~0x40u;
        }
    }
}

// Retail symbol is Fv but the body reads r4 (a flag) - forced-name form
// (same scheme as CfObject_setMoveBusyState, whose symbol carries the Fi).
void CfObject_setMoveReadyFlag__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, int flag) {
    if (flag != 0) {
        self->mFlags68 |= 0x02000000;
    } else {
        self->mFlags68 &= ~0x02000000;
    }
    void* target = self->mTargetC4;
    if (target != 0) {
        CfObjectMoveC4Flags* view = (CfObjectMoveC4Flags*)target;
        if (flag != 0) {
            view->flags |= 0x100;
        } else {
            view->flags &= ~0x100;
        }
    }
}

// Retail CfObjectMove_syncRegionToPos: when unk64 bit 3 (0x8) is set, reattach the position
// region (+0x60C) to the move position (+0x3C). (Old updatePos method body
// moved verbatim onto the retail unmangled name.)
extern "C" void CfObjectMove_syncRegionToPos(cf::CfObjectMove* self) {
    if ((self->unk64 & 8) != 0) {
        ColiSetAxisBlockInverse(self->_60C_region, &self->mPos3C);
    }
}

// Retail symbol is Fv but the body reads r4 (a position vector forwarded to
// the base +0x9C implementation) - forced-name form, same scheme as the
// CfObjectModel sibling defined in CfObjectModel.cpp.
void CfObject_setMoveTargetVec__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, const ml::CVec3* vec) {
    void* target = self->mTargetC4;
    if (target != 0) {
        // One-arg call: retail leaves r4 (the vec) untouched for the base
        // call below, so the source passes only the C4 target.
        setAnimSubPos(target);
    }
    CfObject_setMoveTargetVec__Q22cf13CfObjectModelFv(self, vec);
}

// Retail symbol is Fv but the body reads r4 (a position vector forwarded to
// the base +0x9C implementation) - forced-name form, same scheme as the
// CfObject_setMoveTargetVec sibling. The C4 target and +0x98 sub-object are
// processed when present before the base call.
void CfObject_UnkVirtualFunc22__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, const ml::CVec3* vec) {
    cf::CfObjectMoveTargetC4* target = (cf::CfObjectMoveTargetC4*)self->mTargetC4;
    if (target != 0) {
        setAnimLastPos(target);
        if (self->mSubObj98 != 0) {
            scnImN4DynStart(self->mSubObj98);
        }
    }
    CfObjectModel_syncMoveTarget(self, vec);
}

// Retail symbol is Fv but the body consumes r4 (a position vector) and f1
// (an amount) - forced-name form carrying the hidden args (same scheme as
// CfObject_setMoveTargetVec/22/26). Each unk64 bit test is normalized through
// the double-cntlzw booleanize idiom (retail booleanizes twice).
extern "C" void CfObject_UnkVirtualFunc25__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, const ml::CVec3* vec, float amount) {
    u32 flags = self->unk64;
    if ((((u32)__cntlzw((u32)__cntlzw(flags & 0x08000000) >> 5)) >> 5) == 0) {
        // Positive &&-chain: each guard emits its own compare-and-branch
        // (the negated ||-chain made MWCC add a cror to the fcmpo).
        if (((((u32)__cntlzw((u32)__cntlzw(flags & 0x10000000) >> 5)) >> 5) != 0) &&
            ((((u32)__cntlzw((u32)__cntlzw(flags & 0x8) >> 5)) >> 5) != 0) &&
            amount > lbl_eu_80666AD4 && lbl_eu_80663E42 == 4u &&
            lbl_eu_80663E44 == 1u) {
            // Sum the incoming vector with the shared offset through
            // CVec3::operator+ (nw4r VEC3Add -> paired-single adds); the
            // copy-ctor init reproduces retail's second stack copy.
            ml::CVec3 off(::lbl_eu_80666A88, lbl_eu_80666AD8[0],
                          ::lbl_eu_80666A88);
            ml::CVec3 arg;
            arg = *vec + off;
            CfObject_snapMoveTargetBase(self, &arg);
        } else {
            CfObject_snapMoveTargetBase(self, vec);
        }
        // Both dispatch paths raise busy bit 8.
        self->mFlags68 |= 0x100;
    }
    if (self->mTargetC4 != 0) {
        // Two-arg call form: retail sets r4 = this+0x3C explicitly.
        ((bool (*)(void*, void*))setAnimSubPos)(self->mTargetC4, &self->mPos3C);
        if (self->mSubObj98 != 0) {
            scnImN4DynStart(self->mSubObj98);
        }
    }
}

// Retail symbol is Fv but the body consumes r4 (a position vector) and f1
// (an amount) - forced-name form carrying the hidden args (same scheme as
// CfObject_setMoveTargetVec/22). extern "C" so the definition symbol is the
// exact retail name.
extern "C" void CfObject_UnkVirtualFunc26__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, const ml::CVec3* vec, float amount) {
    // Each unk64 bit test is normalized through the double-cntlzw booleanize
    // idiom (retail booleanizes each test twice).
    u32 flags = self->unk64;
    if ((((u32)__cntlzw((u32)__cntlzw(flags & 0x08000000) >> 5)) >> 5) == 0) {
        // Same positive &&-chain shape as Func25.
        if (((((u32)__cntlzw((u32)__cntlzw(flags & 0x10000000) >> 5)) >> 5) != 0) &&
            ((((u32)__cntlzw((u32)__cntlzw(flags & 0x8) >> 5)) >> 5) != 0) &&
            amount > lbl_eu_80666AD4 && lbl_eu_80663E42 == 4u &&
            lbl_eu_80663E44 == 1u) {
            ml::CVec3 off(::lbl_eu_80666A88, lbl_eu_80666AD8[0],
                          ::lbl_eu_80666A88);
            ml::CVec3 arg;
            arg = *vec + off;
            CfObject_snapMoveTargetBase(self, &arg);
        } else {
            CfObject_snapMoveTargetBase(self, vec);
        }
        // Both dispatch paths raise busy bit 8 (retail converges here from
        // the base-call branch target before falling into the tail).
        self->mFlags68 |= 0x100;
    }
    if (self->mTargetC4 != 0) {
        // Two-arg call form: retail sets r4 = this+0x3C (position) explicitly
        // before the call, so pass the position through the function-pointer
        // cast (the header declares the one-arg form used by Func22).
        ((bool (*)(void*, void*))setAnimLastPos)(self->mTargetC4, &self->mPos3C);
        if (self->mSubObj98 != 0) {
            scnImN4DynStart(self->mSubObj98);
        }
    }
}

ml::CVec3* cf::CfObjectMove::CfObject_getPosVector() {
    // Both sub-objects present + busy flag bit 2 (mFlags68 & 4) -> return a
    // pointer into the C4 target; otherwise defer to the base implementation.
    if (mSubObj98 != 0 && *(const u32*)field_0x9C != 0 && (mFlags68 & 0x4) != 0) {
        cf::CfObjectMoveTargetC4* target = (cf::CfObjectMoveTargetC4*)mTargetC4;
        return (ml::CVec3*)&target->field_3A8;
    }
    return CfObjectModel::CfObject_getPosVector();
}


// Wave-62: override of CfObject_setRotVec (+0xBC). Free-function FPv form
// keeps the Unk linker name that hand-built tables / CREvtModel reference.
extern "C" void CfObject_UnkVirtualFunc27__Q22cf12CfObjectMoveFPv(cf::CfObjectMove* self, void* src) {
    // Z-only movement (X within epsilon but Z outside it) marks the target
    // with the 0x20 flag so its next update re-aims instead of sliding.
    const ml::CVec3* vec = (const ml::CVec3*)src;
    if ((self->unk64 & 0x80) != 0) {
        if (ml::math::abs(vec->x) > lbl_eu_8066A208 ||
            ml::math::abs(vec->z) > lbl_eu_8066A208) {
            CfObjectMoveC4FlagsView* c4 = (CfObjectMoveC4FlagsView*)self->mTargetC4;
            if (c4 != 0) {
                c4->flags |= 0x20;
            }
        }
    }
    CfObjectMoveTargetC4* target = (cf::CfObjectMoveTargetC4*)self->mTargetC4;
    if (target != 0 && (self->mFlags68 & 0x4) != 0) {
        setTurnAngle(target, vec->y);
    }
    self->CfObject_setModelRotVec(src);
}

// Retail symbol is Fv but the body consumes f1 (base call, C4-gated
// setTurnAngle, 6C0 +0xC store) - forced-name form carrying the hidden
// float arg (same scheme as CfObject_UnkVirtualFunc30). (Old method body
// moved verbatim onto the retail Fv name.)
void CfObject_setMoveHeadAngle__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, float value) {
    CfObject_setMoveHeadAngle__Q22cf13CfObjectModelFv(self);
    void* target = self->mTargetC4;
    if (target != 0 && ((self->mFlags68 & 0x4) != 0)) {
        setTurnAngle(target, value);
    }
    target = self->mTarget6C0;
    if (target != 0) {
        *(float*)((char*)target + 0xC) = value;
    }
}

// Wave-64: override of CfObject_setMoveYaw (+0xC8). Fv keeps Unk linker
// name (hand-built tables / CREvtModel). Retail symbol is Fv but the body
// consumes f1 - forced-name form carrying the hidden float arg (same scheme
// as CfObject_setMoveHeadAngle). Base call, then the C4 target's busy flag
// (bit 2) gates a setTurnScale call plus a +0x400 flag set (mTargetC4 is
// re-read after the call), and the 6C0 target's +0xC word receives the value.
void CfObject_UnkVirtualFunc30__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, float value) {
    CfObject_setMoveHeadAngle__Q22cf13CfObjectModelFv(self);
    void* target = self->mTargetC4;
    if (target != 0 && ((self->mFlags68 & 0x4) != 0)) {
        setTurnScale(target, value);
        CfObjectMoveC4Flags* view = (CfObjectMoveC4Flags*)self->mTargetC4;
        view->flags |= 0x400;
    }
    target = self->mTarget6C0;
    if (target != 0) {
        *(float*)((char*)target + 0xC) = value;
    }
}

// Empty Ff stub removed (Wave-62): retail only exports the Fv scaled
// applyMoveYaw body below; the prior member def emitted a dead Ff symbol.

// Retail symbol is Fv but the body returns 1 on the null path and tail-calls
// the C4 target's vtable +0x80 with a 0 arg (the base header declares the
// override void) - forced-name form carrying the int return. The C4 target
// is the +0xD0 CActParamAnimGame, so +0x80 is CActParamAnim::
// calcAnimBlendWeight (retail table lbl_eu_80526458, method 30) - called on
// the owning type instead of the old CfC4Target single-slot proxy (which
// emitted +0x08: retail reads 128(r12)).
extern "C" int CfObject_queryTargetState__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    ::CActParamAnim* target = (::CActParamAnim*)self->mTargetC4;
    if (target != 0) {
        return target->calcAnimBlendWeight(0);
    }
    return 1;
}

extern "C" void* CfObjectMove_getSubObj54(CfObjectMove* self) { return &self->mSubObj54; }

// Retail symbol is Fv but the body consumes f1 (store to self+0x6E4 and to
// the C4 target) - forced-name form with the hidden float arg (same scheme as
// CfObjectMove_recordMoveValue).
extern "C" void CfObject_UnkVirtualFunc57__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, float value) {
    cf::CfObjectMoveTargetC4* target = (cf::CfObjectMoveTargetC4*)self->mTargetC4;
    self->mField6E4 = value;
    if (target != 0) {
        target->field_430 = value;
        target->field_43C = target->field_438 * value;
    }
}

// Retail symbol is Fv but the body consumes f1 (store to self and to the
// C4 target) - forced-name form with the hidden float arg (same scheme as
// CfObject_UnkVirtualFunc32/33 below).
extern "C" void CfObjectMove_recordMoveValue__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, float value) {
    void* target = self->mTargetC4;
    self->mField6E8 = value;
    if (target != 0) {
        *(float*)((char*)target + 0x434) = value;
    }
}

// Wave-63: override of CObjectParam_getActiveParam (+0x40). Fv keeps Unk
// linker name from lbl_eu_80529690.
extern "C" void* CObjectParam_UnkVirtualFunc2__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    return self->CfObjectModel::CObjectParam_getActiveParam();
}




// Open item: three null-checked stores of `value` at +0x388. MWCC reuses r3
// (this) for the THIRD target load/store (this is dead after); retail keeps r4
// for all three (3 reg_swap, 0 structural). Witness rejects the r4<->r3 ABI
// permutation. Separated locals + (void)this invariant.
void* cf::CfObjectMove::CfObject_pushRefreshValue(float value) {
    void* target = this->mTargetC4;
    if (target != 0) {
        *(float*)((char*)target + 0x388) = value;
    }
    target = this->mTargetC8;
    if (target != 0) {
        *(float*)((char*)target + 0x388) = value;
    }
    target = this->mTargetCC;
    if (target != 0) {
        *(float*)((char*)target + 0x388) = value;
    }
    return this;
}

// Retail CfObjectMove_setNpcMoveRange (0x800BEC44, slot 0x1EC): store the
// incoming float to the +0x6C0 movement target's +0xD4 arrival range
// (ocUnit walkR feeds it the walk range). Forced-name Fv form keeps the
// Unk linker name CfObjectMove_UnkVirtualFunc10__Q22cf12CfObjectMoveFv with
// the hidden float arg (same scheme as the writeTargetField sibling below).
extern "C" void CfObjectMove_UnkVirtualFunc10__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, float value) {
    void* o = *(void**)((u8*)self + 0x6c0);
    if (o != 0) {
        *(float*)((u8*)o + 0xd4) = value;
    }
}

extern "C" void CfObjectMove_writeTargetField__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u16 v) {
    void* o = *(void**)((u8*)self + 0x6c0);
    if (o) *(u16*)((u8*)o + 0xde) = v;
}

// Retail symbols are Fv but both bodies return a float (load from the C4
// target, else the shared time source) - forced-name forms carrying the
// float return (the base header declares the overrides void).
extern "C" float CfObject_getMoveSpeedRate__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    // Getter for the value CfObject_pushRefreshValue's target writes at
    // +0x388; falls back to the shared time source when no target. The
    // shared header declares CfT_PlayRateGet void; it really returns float,
    // so call it through a cast (same as CtrlNpc.cpp).
    void* target = self->mTargetC4;
    if (target != 0) {
        return *(float*)((char*)target + 0x388);
    }
    return ((float (*)())CfT_PlayRateGet)();
}

extern "C" float CfObject_readRefreshValue__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    void* target = self->mTargetC4;
    if (target != 0) {
        return *(float*)((char*)target + 0x38C);
    }
    return ((float (*)())CfT_PlayRateGet)();
}

// Retail symbol is Fv but ocUnit setNpcPath leaves r4-r7/f1 live into
// the bctr (ignored here; tail call takes r3 only) - forced-name form with
// hidden args (recordMoveValue precedent). Converted from a method so the
// widened class decl keeps compiling; same Fv symbol, same body.
extern "C" void CfObjectMove_refreshNpcState__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, int modeA, int modeB, int count, const ml::CVec3* start, double angle) {
    (void)modeA; (void)modeB; (void)count; (void)start; (void)angle;
    // Dispatch to the NPC movement-target handler (tail call, r3 only).
    cf::CtrlNpc* target = (cf::CtrlNpc*)self->mTarget6C0;
    if (target != 0) {
        CtrlNpcInitMoveWindow(target);
    }
}

// Retail symbol is Fv but ocUnit moveTo leaves r4/f1/r5 live into
// the bctr (ignored here; tail call takes r3 only) - forced-name form with
// hidden args (refreshNpcState precedent). Converted from a method so the
// widened class decl keeps compiling; same Fv symbol, same body.
extern "C" void CfObjectMove_updateNpcTarget__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, const ml::CVec3* vec, float step, int flag, int angle) {
    (void)vec;
    (void)step;
    (void)flag;
    (void)angle;
    cf::CtrlNpc* target = (cf::CtrlNpc*)self->mTarget6C0;
    if (target != 0) {
        CtrlNpcSetupAction(target);
    }
}

extern "C" void CfObjectMove_advanceNpcTarget__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, const ml::CVec3* vec) {
    (void)vec;
    cf::CtrlNpc* target = (cf::CtrlNpc*)self->mTarget6C0;
    if (target != 0) {
        CtrlNpcStoreTargetActivate(target);
    }
}

extern "C" void CfObjectMove_commandNpcTarget__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, int arg) {
    (void)arg;
    cf::CtrlNpc* target = (cf::CtrlNpc*)self->mTarget6C0;
    if (target != 0) {
        func_80094E44(target);
    }
}

extern "C" void CfObjectMove_restartNpcTarget__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, int a, int b, int c) {
    (void)a; (void)b; (void)c;
    // Tail-call the NPC movement-target handler (r3 only); see Func7/8/12/13.
    cf::CtrlNpc* target = (cf::CtrlNpc*)self->mTarget6C0;
    if (target != 0) {
        CtrlNpcArmMoveTarget5(target);
    }
}

// Retail symbol is Fv and the body genuinely returns int (1, or the CtrlNpc
// action-advance query result). The header declares the vtable slot with the
// int return so the vtable entry keeps the retail name.
// const self: MWCC hoists the first member load (lwz r3,0x6c0(r3)) above the
// LR save (stw r0,0x14(r1)) - the documented plateau closed by the const-self
// lever (CScnEffectActNw4r getters family; non-const emits stw-first).
// extern "C" + the mangled name keeps the vtable reference verbatim.
extern "C" int CfObjectMove_queryNpcAdvance__Q22cf12CfObjectMoveFv(const cf::CfObjectMove* self) {
    cf::CtrlNpc* target = (cf::CtrlNpc*)self->mTarget6C0;
    int result = 1;
    if (target != 0) {
        result = CtrlNpcCanAdvanceAction(target);
    }
    return result;
}



// Retail symbol is Fv but the body consumes f1 (three null-checked stores at
// +0x38C of the C4/C8/CC targets) - forced-name form with the hidden float
// arg (same scheme as CfObjectMove_recordMoveValue).
extern "C" void* CfObject_pushRefreshExtra__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, float value) {
    void* target = self->mTargetC4;
    if (target != 0) {
        *(float*)((char*)target + 0x38C) = value;
    }
    target = self->mTargetC8;
    if (target != 0) {
        *(float*)((char*)target + 0x38C) = value;
    }
    target = self->mTargetCC;
    if (target != 0) {
        *(float*)((char*)target + 0x38C) = value;
    }
    return self;
}

// Retail symbols are Fv but the bodies consume r4 (an index forwarded to
// the CCharEffect slot helpers) - forced-name forms carrying the hidden
// arg (same scheme as CfObjectMove_flushEffectSlots, which passes the
// vtable query result leftover in r3). Query the +0x38 sub-object's vtable
// slot +0xE4; on a non-zero result re-run the query (retail reloads the
// sub pointer) and forward the query result plus the incoming index.
void CfObjectMove_setEffectSlotBit__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u32 arg) {
    cf::CfObject* sub = (cf::CfObject*)self->mSubObj38;
    if (sub != 0) {
        if (sub->CfObject_getCharEffect() != 0) {
            int r = ((cf::CfObject*)self->mSubObj38)->CfObject_getCharEffect();
            func_8015C074((void*)r, arg);
        }
    }
}

void CfObjectMove_transferSlotBits__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u32 arg) {
    cf::CfObject* sub = (cf::CfObject*)self->mSubObj38;
    if (sub != 0) {
        if (sub->CfObject_getCharEffect() != 0) {
            int r = ((cf::CfObject*)self->mSubObj38)->CfObject_getCharEffect();
            CharEffMoveBit((void*)r, arg);
        }
    }
}

// Retail symbol is Fv but the body consumes r4-r8 (five args forwarded to
// the CCharEffect slot helper) - forced-name form carrying the hidden args
// (same scheme as CfObjectMove_setEffectSlotBit/18). The +0x38 sub-object's
// vtable +0xE4 is queried twice; on success the query result plus the five
// args go to CharEffRegisterSlot (the s16 arg emits the retail extsh).
void CfObjectMove_attachEffectSlot__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u32 a, u32 b, u32 c, u32 d, u32 e) {
    cf::CfObject* sub = (cf::CfObject*)self->mSubObj38;
    if (sub != 0) {
        if (sub->CfObject_getCharEffect() != 0) {
            int r = ((cf::CfObject*)self->mSubObj38)->CfObject_getCharEffect();
            // c is widened here so the int->short conversion emits the retail
            // extsh at the call site (MWCC defers it after the plain moves).
            CharEffRegisterSlot((void*)r, a, b, (s16)c, d, e);
        }
    }
}

void cf::CfObjectMove::CfObjectMove_flushEffectSlots() {
    // Query the +0x38 sub-object's vtable slot +0xE4; on a non-zero result
    // re-run the query (retail reloads the sub pointer) and flush the
    // character effect slots.
    cf::CfObject* sub = (cf::CfObject*)mSubObj38;
    if (sub != 0) {
        if (sub->CfObject_getCharEffect() != 0) {
            ((cf::CfObject*)mSubObj38)->CfObject_getCharEffect();
            CharEffReleaseSlots0A10();
        }
    }
}

void cf::CfObjectMove::CfObjectMove_setAllEffectBits() {
    // Loop the counter 0..0x2B through the vtable slot +0x208 (the int-taking
    // form of CfObjectMove_setEffectSlotBit, declared with retail arity on
    // CfObjectMove, so the call dispatches directly).
    for (int i = 0; i < 0x2C; i++) {
        this->CfObjectMove_setEffectSlotBit(i);
    }
}

// Retail symbol is Fv but the body consumes r4 (forwarded to the CCharEffect
// slot query) and returns the helper's bool result (0 on the null path) -
// forced-name form carrying the hidden arg and the return. Same sub-object
// +0xE4 double-query scheme as CfObjectModel_UnkVirtualFunc18.
bool CfObjectMove_isEffectSlotUsed__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u32 arg) {
    cf::CfObject* sub = (cf::CfObject*)self->mSubObj38;
    if (sub != 0) {
        if (sub->CfObject_getCharEffect() != 0) {
            int r = ((cf::CfObject*)self->mSubObj38)->CfObject_getCharEffect();
            return func_8015C294((unsigned int*)r, arg);
        }
    }
    return 0;
}

void CfObjectMove_signalEffectSlot__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u32 arg) {
    cf::CfObject* sub = (cf::CfObject*)self->mSubObj38;
    if (sub != 0) {
        if (sub->CfObject_getCharEffect() != 0) {
            int r = ((cf::CfObject*)self->mSubObj38)->CfObject_getCharEffect();
            func_8015BD24((void*)r, arg);
        }
    }
}

// Wave-64: override of CfObjectModel_detachEffectSlot (+0x1BC). Fv keeps
// Unk linker name (hand-built tables / CREvtModel). Retail symbol is Fv
// but the body consumes r4 (an index forwarded to the CCharEffect slot
// helper) - forced-name form carrying the hidden arg (same scheme as
// CfObjectMove_setEffectSlotBit). Query the +0x38 sub-object's vtable
// slot +0xE4; on a non-zero result re-run the query (retail reloads the
// sub pointer) and forward the query result plus the incoming arg to
// CharEffRemoveByPtr.
void CfObjectModel_UnkVirtualFunc18__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u32 arg) {
    cf::CfObject* sub = (cf::CfObject*)self->mSubObj38;
    if (sub != 0) {
        if (sub->CfObject_getCharEffect() != 0) {
            int r = ((cf::CfObject*)self->mSubObj38)->CfObject_getCharEffect();
            CharEffRemoveByPtr((void*)r, (void*)arg);
        }
    }
}


// Retail symbol is Fv but the body takes a hidden arg (r4) and returns a
// pointer (the CfGameManager loader result, or 0) - forced-name form.
extern "C" void* CfObjectMove_loadResourceById__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, void* param) {
    void* a = self->mField6DC;
    bool cond = (a != 0 && self->mField6E0 != 0);
    if (cond) {
        return func_80081900__Q22cf13CfGameManagerFv((u32)param, (u32)a, (u32)self->mField6E0);
    }
    return 0;
}

// Retail requestVoice__Q22cf8CfObjectFiUlff (0x800BF204): forwards the +0x38
// sub-object (+0x28) to the voice dispatcher, or returns false when absent.
// (Old func_800BE898 forced-name label renamed onto the true retail method
// name; CfObject.hpp decl return fixed void -> bool.)
bool cf::CfObject::requestVoice(int a, u32 b, float c, float d) {
    void* sub = mSubObj38;
    if (sub != 0) {
        return playGated((char*)sub + 0x28);
    }
    return false;
}


// Retail func_800BE8F4: sub-object state query - null sub-object or negative
// +0x2C counter yields -1, else the +0x30 word. (Old getSubState method body
// moved verbatim onto the retail unmangled name.)
extern "C" int func_800BE8F4(cf::CfObjectMove* self) {
    struct SubObj { int dummy0[11]; int field_2c; int field_30; };
    SubObj* sub = (SubObj*)self->mSubObj38;
    if (sub == 0) {
        return -1;
    } else if (sub->field_2c < 0) {
        return -1;
    }
    return sub->field_30;
}

// Retail CfObjectMove_releaseVoiceHandle: release the +0x38 sub-object's voice handle.
// (Old freeSub method body moved verbatim onto the retail unmangled name.)
extern "C" void CfObjectMove_releaseVoiceHandle(cf::CfObjectMove* self)
{
    extern void stopVoiceB(void*);
    char* sub = (char*)self->mSubObj38;
    if (sub != 0) {
        stopVoiceB(sub + 0x28);
    }
}


// Retail CfObjectMove_setSubB0FieldC: store to the +0xB0 sub-object's +0xC halfword.
// (Old setSubFieldC method body moved verbatim onto the retail name.)
extern "C" void CfObjectMove_setSubB0FieldC(cf::CfObjectMove* self, unsigned short val) {
    void* ptr = self->mSubObjB0;
    *(unsigned short*)((char*)ptr + 0xc) = val;
}

// Retail CfObjectMove_getSubB0FieldA: sign-extended load of the +0xB0 sub-object's +0xA
// halfword. (Old getSubFieldA method body moved onto the retail name.)
extern "C" int CfObjectMove_getSubB0FieldA(cf::CfObjectMove* self) { return *(s16*)((char*)self->mSubObjB0 + 10); }

// Retail CfObjectMove_setSubB0FieldA: store to the +0xB0 sub-object's +0xA halfword.
// (Old setSubFieldA method body moved onto the retail name.)
extern "C" void CfObjectMove_setSubB0FieldA(cf::CfObjectMove* self, unsigned short val) {
    void* ptr = self->mSubObjB0;
    *(unsigned short*)((char*)ptr + 0xa) = val;
}

// Retail CfObjectMove_getSubB0FieldE: sign-extended load of the +0xB0 sub-object's +0xE
// halfword. (Old getSubFieldE method body moved onto the retail name.)
extern "C" int CfObjectMove_getSubB0FieldE(cf::CfObjectMove* self) { return *(s16*)((char*)self->mSubObjB0 + 14); }

// Retail CfObjectMove_setSubB0FieldE: store to the +0xB0 sub-object's +0xE halfword.
// (Old setSubFieldE method body moved onto the retail name.)
extern "C" void CfObjectMove_setSubB0FieldE(cf::CfObjectMove* self, unsigned short val) {
    void* ptr = self->mSubObjB0;
    *(unsigned short*)((char*)ptr + 0xe) = val;
}

extern "C" void CfObject_isMoveActiveNow__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    reinterpret_cast<cf::CfResObjImpl*>(*(void**)((u8*)self + 0xb0))->ResObj_IsInUse_C860();
}

extern "C" void CfObject_forwardSubObject__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    reinterpret_cast<cf::CfResObjImpl*>(*(void**)((u8*)self + 0xb0))->CfObjectMove_relaySubB0Slot14();
}

void cf::CfObjectMove::virtCall10(){
    void** inner = (void**)mPtr10;
    void (*func)(void*) = (void (*)(void*))inner[0x14 / 4];
    func(this);
}

// Retail symbol is Fv but the body is a bcctr thunk that leaves the caller's
// r4/r5 live into CfResObjImpl::ResObj_SetWork_CD68 (slot +0x30).
extern "C" void CfObject_setAnimSlotEntry__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, int idx, int value) {
    reinterpret_cast<cf::CfResObjImpl*>(*(void**)((u8*)self + 0xb0))->ResObj_SetWork_CD68(idx, value);
}

// Retail symbol is Fv but the body is a bcctr thunk that leaves the caller's
// r4 live into CfResObjImpl::ResObj_GetWork_CCBC (slot +0x34).
extern "C" void CfObject_UnkVirtualFunc62__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, int idx) {
    reinterpret_cast<cf::CfResObjImpl*>(*(void**)((u8*)self + 0xb0))->ResObj_GetWork_CCBC(idx);
}

// Wave-62: override of CfObject_refreshSubB0 (+0x80). Free-function Fv form
// keeps the Unk linker name that hand-built tables / CREvtModel reference.
extern "C" void CfObject_UnkVirtualFunc12__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    // Double-hop through the +0xB0 sub-object's vtable slots +0x0C and
    // +0x10 (Shift-padded view: the vptr sits at object+0x10). Retail
    // reloads mSubObjB0 for each call, so keep the two calls as separate
    // member accesses rather than a cached pointer.
    reinterpret_cast<cf::CfResObjImpl*>(self->mSubObjB0)->ResObj_DispatchState_CCE0();
    reinterpret_cast<cf::CfResObjImpl*>(self->mSubObjB0)->CfResObj_noop10();
}


// Retail symbol is Fv but the body consumes r4 (a slot index) - forced-name
// form carrying the hidden arg (same scheme as CfObject_setMoveReadyFlag).
// Releases the indexed +0x6F8 slot: stop its animation model through the
// +0x98 sub-object (vtable +0xC8), detach its character-effect slots via
// the +0x38 sub-object's +0xE4 query, release the model list, and clear the
// slot. Then clears the matching +0x6C flag bit (18 for slot 1, 17 for
// slot 0).
void CfObjectMove_releaseSlotById__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, int index) {
    if (index < 2) {
        if (self->mField6F8[index] != 0) {
            if (self->mSubObj98 != 0) {
                reinterpret_cast<CScnItemModel*>(self->mSubObj98)->vfuncC8(self->mField6F8[index]);
            }
            cf::CfObject* sub38 = (cf::CfObject*)self->mSubObj38;
            if (sub38 != 0) {
                if (sub38->CfObject_getCharEffect() != 0) {
                    int q = ((cf::CfObject*)self->mSubObj38)->CfObject_getCharEffect();
                    CharEffDetachBySubId((void*)q, self->mField6F8[index]);
                }
            }
            Scn_IsAnimActiveOrNull(self->mField6F8[index]);
            self->mField6F8[index] = 0;
        }
        if (index == 1) {
            ((cf::CfObjectMoveFlags6C*)self)->field_6C &= ~0x00002000;
        } else {
            ((cf::CfObjectMoveFlags6C*)self)->field_6C &= ~0x00001000;
        }
    }
}

// Retail symbol is Fv but the body reads r4 (an int flag forwarded to the
// base implementation, which the base header declares with an int param) -
// forced-name form carrying the hidden arg, same scheme as
// CfObject_setMoveReadyFlag.
void CfObject_UnkVirtualFunc66__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, int flag) {
    CfObjectModel_setPointEnabled(self, flag);
    if ((self->unk64 & 8) != 0 && flag == 0 && lbl_eu_80665958 != 0) {
        // flag is provably 0 here, so the attach path never runs; it is
        // retained because retail emits both sides of the inner flag test
        // (one cmpwi in cr1 serves both tests, with attach as the
        // fall-through).
        if (flag != 0) {
            func_804B4BDC(lbl_eu_80665958, self->_60C_region);
        } else {
            Coli_ListRemoveNode(lbl_eu_80665958, self->_60C_region);
        }
    }
}

// Retail symbol is Fv but the body consumes r4 (a flag) - forced-name form
// (same scheme as CfObject_setMoveReadyFlag). Raises bit 21 of mFlags68 (or
// clears bit 21 when the flag is 0), then walks the two +0x6F8 slots,
// forwarding the flag to each whose +0x7A4 bit 1 differs.
void CfObjectModel_UnkVirtualFunc19__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, int flag) {
    if (flag != 0) {
        self->mFlags68 |= 0x00200000;
    } else {
        self->mFlags68 &= ~0x00200000;
    }
    for (int i = 0; i < 2; i++) {
        cf::CfObjectModelSub98* s = (cf::CfObjectModelSub98*)self->mField6F8[i];
        if (s != 0 && (((s->field_7A4 >> 1) & 1) != flag)) {
            simSetFlag2OnTree(s, flag);
        }
    }
}

extern "C" void CfObjectModel_UnkVirtualFunc6__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    reinterpret_cast<cf::CfResObjImpl*>(*(void**)((u8*)self + 0xb0))->CfResObj_unk48();
}


// Retail symbol is Fv but the body consumes r4 (forwarded to the vtable
// +0x18C slot) - forced-name form carrying the hidden arg. Teardown/init
// sequence: base model funcs 2 and 1 (vtable +0x17C/+0x178), then the
// +0x1D0 slot (CfObjectMove_releaseSlotById) with flags 0 and 1, the
// +0x90/+0x94 word pair is cleared, and the arg goes to the +0x18C slot
// (CfObjectModel_checkTargetNode).
void CfObject_UnkVirtualFunc37__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u32 arg) {
    cf::CfObjectMove90View* view = reinterpret_cast<cf::CfObjectMove90View*>(self);
    self->CfObjectModel_releaseModelList();  // vtable +0x17C
    self->CfObjectModel_releaseModelSub();  // vtable +0x178
    self->CfObjectMove_releaseSlotById(0);
    self->CfObjectMove_releaseSlotById(1);
    view->field_90 = 0;
    view->field_94 = 0;
    self->CfObjectModel_checkTargetNode((void*)arg);  // vtable +0x18C
}

// Retail symbol is Fv but the body reads r4 (forwarded to the +0xB0
// sub-object's vtable slot +0x4C) - forced-name form. Also calls the vtable
// slot +0x17C (CfObjectModel_releaseModelList) and clears the +0x94 word.
void CfObject_UnkVirtualFunc38__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u32 arg) {
    self->CfObjectModel_releaseModelList();
    reinterpret_cast<cf::CfObjectMove90View*>(self)->field_94 = 0;
    reinterpret_cast<cf::CfResObjImpl*>(self->mSubObjB0)->CfResObj_unk4C(arg);
}









// Wave-64: override of CfObject_armAnimSlot0 (+0xEC). Fv keeps Unk linker
// name. Retail symbols are Fv but the bodies consume r4/r5 (a bdat id and a
// slot index) - forced-name forms carrying the hidden args (same scheme as
// CfObject_setMoveReadyFlag). Both look up the id via func_801422A8, record
// the id and the column-0x7 result in the +0x710/+0x70C u16 arrays, then
// either clear the slot (vtable +0x1D0 with a 0/1 flag) or arm it (vtable
// +0x144 with the bdat result) plus the +0xB0 sub-object's vtable +0x3C.
// func_800BF7A0 uses slot 0 / flags 6,0; func_800BF980 slot 1 / flags 7,1.
int CfObject_UnkVirtualFunc39__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u32 arg1, u32 arg2) {
    int r = (int)cf::CfBdat::func_801422A8(arg1);
    self->field_710[0] = (u16)arg1;
    self->field_70C[0] = (u16)func_8014235C(arg1, &lbl_eu_804FC550[0x7], 0);
    if (r == 0) {
        self->CfObjectMove_releaseSlotById(0);
    } else {
        self->field_70C[0] = (u16)((int (*)(u32))func_80142428)(arg2);
        self->CfObject_setAnimSlotEntry(6, (u32)r);
        if (self->mSubObj98 != 0) {
            reinterpret_cast<cf::CfResObjImpl*>(self->mSubObjB0)->CfResObj_unk3C((u32)r, 0);
        }
    }
    return 1;
}

// Wave-64: override of CfObject_getActorScale / armAnimSlot0N (+0xF0). Fv
// keeps Unk linker name. Retail symbol is Fv but the body consumes r4/r5
// (a bdat id and a slot index) - forced-name form carrying the hidden args
// (same scheme as CfObject_armAnimSlot0/42). Unlike the 39/42 pair the slot
// index is first converted to a name (CfBdat::getBdatStringEntry) and the arm
// path feeds that name back through func_80142428. Slot 0 / flags 6,0.
int CfObject_UnkVirtualFunc40__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u32 arg1, u32 arg2) {
    int r;
    u32 name;
    name = (u32)cf::CfBdat::getBdatStringEntry((u16)arg2);
    r = (int)cf::CfBdat::func_801422A8(arg1);
    self->field_710[0] = (u16)arg1;
    self->field_70C[0] = (u16)func_8014235C(arg1, &lbl_eu_804FC550[0x7], 0);
    if (r == 0) {
        self->CfObjectMove_releaseSlotById(0);
    } else {
        self->field_70C[0] = (u16)((int (*)(u32))func_80142428)(name);
        self->CfObject_setAnimSlotEntry(6, (u32)r);
        if (self->mSubObj98 != 0) {
            reinterpret_cast<cf::CfResObjImpl*>(self->mSubObjB0)->CfResObj_unk3C((u32)r, 0);
        }
    }
    return 1;
}

int CfObject_UnkVirtualFunc42__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u32 arg1, u32 arg2) {
    int r = (int)cf::CfBdat::func_801422A8(arg1);
    self->field_710[1] = (u16)arg1;
    self->field_70C[1] = (u16)func_8014235C(arg1, &lbl_eu_804FC550[0x7], 1);
    if (r == 0) {
        self->CfObjectMove_releaseSlotById(1);
    } else {
        self->field_70C[1] = (u16)((int (*)(u32))func_80142428)(arg2);
        self->CfObject_setAnimSlotEntry(7, (u32)r);
        if (self->mSubObj98 != 0) {
            reinterpret_cast<cf::CfResObjImpl*>(self->mSubObjB0)->CfResObj_unk3C((u32)r, 1);
        }
    }
    return 1;
}

// Retail symbol is Fv but the body consumes r4/r5 (a bdat id and a slot
// index) - forced-name form carrying the hidden args (same scheme as
// CfObject_UnkVirtualFunc39/42). Same name round-trip as
// CfObject_UnkVirtualFunc40 but slot 1 / flags 7,1. r is declared first so
// MWCC colours it into r31 (retail keeps r in r31, the func_801424A8
// result in r30).
int CfObject_UnkVirtualFunc43__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u32 arg1, u32 arg2) {
    int r;
    u32 name;
    name = (u32)cf::CfBdat::getBdatStringEntry((u16)arg2);
    r = (int)cf::CfBdat::func_801422A8(arg1);
    self->field_710[1] = (u16)arg1;
    self->field_70C[1] = (u16)func_8014235C(arg1, &lbl_eu_804FC550[0x7], 1);
    if (r == 0) {
        self->CfObjectMove_releaseSlotById(1);
    } else {
        self->field_70C[1] = (u16)((int (*)(u32))func_80142428)(name);
        self->CfObject_setAnimSlotEntry(7, (u32)r);
        if (self->mSubObj98 != 0) {
            reinterpret_cast<cf::CfResObjImpl*>(self->mSubObjB0)->CfResObj_unk3C((u32)r, 1);
        }
    }
    return 1;
}

// Retail symbol is Fv but the body consumes r4 (a bdat name/id) and r5 (a
// slot index) - forced-name form carrying the hidden args. Look up the id via
// func_80142428; when positive and the +0x98 sub-object and the indexed
// +0x6F8 slot both exist, stop the slot's animation model (vtable +0xC8) and
// re-arm it with the name (vtable +0xC4, flag 0). Retail re-tests the same
// two conditions around the first call (MWCC reuses the cr1/cr0 results), and
// reloads both sub-object pointers from memory after the call (no callee-saved
// registers free), so the members are re-read at each use site. The id is
// finally stored as a u16 in the +0x70C slot array.
void CfObject_UnkVirtualFunc45__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u32 value, u32 index) {
    int result = ((int (*)(u32))func_80142428)(value);
    if (result > 0) {
        if (self->mSubObj98 != 0 && self->mField6F8[index] != 0) {
            if (self->mSubObj98 != 0 && self->mField6F8[index] != 0) {
                reinterpret_cast<CScnItemModel*>(self->mSubObj98)->vfuncC8(self->mField6F8[index]);
            }
            reinterpret_cast<CScnItemModel*>(self->mSubObj98)->vfuncC4(self->mField6F8[index], value, 0);
        }
    }
    self->field_70C[index] = (u16)result;
}









void CfResObj_noop2C() {}

extern "C" void CfObjectMove_relaySubB0Slot40(void* self) {
    reinterpret_cast<cf::CfResObjImpl*>(*(void**)((u8*)self + 0xb0))->CfResObj_noop40();
}
// Retail symbol is a bcctr thunk that leaves the caller's r4/r5/r6/f1/f2 live
// into CfResObjImpl::ResObj_NotifySound_C888 (slot +0x54).
extern "C" void CfObjectMove_relaySubB0Slot54(void* self, int a, int b, int c, float f1, float f2) {
    reinterpret_cast<cf::CfResObjImpl*>(*(void**)((u8*)self + 0xb0))->ResObj_NotifySound_C888(a, b, c, f1, f2);
}
extern "C" void CfObjectMove_relaySubB0Slot58(void* self) {
    reinterpret_cast<cf::CfResObjImpl*>(*(void**)((u8*)self + 0xb0))->CfResObj_noop58();
}
extern "C" void CfObjectMove_relaySubB0Slot60(void* self) {
    reinterpret_cast<cf::CfResObjImpl*>(*(void**)((u8*)self + 0xb0))->ResObj_PollActive_C950();
}
extern "C" void CfObjectMove_relaySubB0Slot28(void* self) {
    reinterpret_cast<cf::CfResObjImpl*>(*(void**)((u8*)self + 0xb0))->CfResObj_noop28();
}
extern "C" void CfObjectMove_relaySubB0Slot2C(void* self) {
    reinterpret_cast<cf::CfResObjImpl*>(*(void**)((u8*)self + 0xb0))->CfResObj_noop2C();
}
extern "C" void CfObjectMove_relaySubB0Slot68(void* self) {
    reinterpret_cast<cf::CfResObjImpl*>(*(void**)((u8*)self + 0xb0))->CfResObj_true68();
}
extern "C" void CfObjectMove_setSubB0ScaledRate(void* self, float v) {
    extern float lbl_eu_80666A90;
    void* o = *(void**)((u8*)self + 0xb0);
    *(float*)((u8*)o + 4) = lbl_eu_80666A90 * v;
}


// (isActive's `return 1` body is the already-FULL CfResObj_true68 twin; no
// separate method body - retail keeps one copy.)

// Retail CfObjectMove_setFlag6C9Bit0: insert the incoming bit into b0 of mFlags6C9
// (rlwimi r0,r4,0,31,31). (Old setBit6c9 method body moved verbatim onto
// the retail unmangled name.)
extern "C" void CfObjectMove_setFlag6C9Bit0(cf::CfObjectMove* self, unsigned long bit) {
    unsigned char val = self->mFlags6C9;
    self->mFlags6C9 = __rlwimi(val, bit, 0, 31, 31);
}

// Wave-64: override of CfObject_syncModelRate (+0x168). Fv keeps Unk linker
// name. Retail symbol is Fv but the body consumes f1 (forwarded to the base
// implementation, then subtracted from the shared constant) - forced-name
// form carrying the hidden float arg (same scheme as
// CfObject_setMoveHeadAngle/30). Base call first, then the usual +0x38
// sub-object +0xE4 double-query; on success call the camera-time helper
// with (query result, lbl_eu_80666A94 - value).
void CfObject_UnkVirtualFunc70__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, float value) {
    CfObjectModel_syncModelRate(self, value);
    cf::CfObject* sub = (cf::CfObject*)self->mSubObj38;
    if (sub != 0) {
        if (sub->CfObject_getCharEffect() != 0) {
            int r = ((cf::CfObject*)self->mSubObj38)->CfObject_getCharEffect();
            func_eu_8015D258((void*)r, lbl_eu_80666A94 - value);
        }
    }
}

// Retail CfObjectMove_getSelfIfActive: return this when set (non-null and unk64 bit 3),
// else null. (Old testFlag8 method body moved verbatim onto the retail
// unmangled name.)
extern "C" cf::CfObjectMove* CfObjectMove_getSelfIfActive(cf::CfObjectMove* self) {
    if (self != NULL && (self->unk64 & 8) != 0) {
        return self;
    }
    return NULL;
}

} // namespace cf

// Retail: vptr at +0x10, bcctr to slot +0x14 (CfResObjImpl::ResObj_IsInUse_C860).
extern "C" void CfObjectMove_relaySubB0Slot14(void* self) {
    reinterpret_cast<cf::CfResObjImpl*>(self)->ResObj_IsInUse_C860();
}

// Wave-64: override of CfObject_syncMoveHead (+0xD0). Fv keeps Unk linker
// name. Retail symbol is Fv but the body tail-calls vtable+0xC4 with a scaled
// float argument - forced-name form (same as the CfObjectModel sibling).
extern "C" void CfObject_UnkVirtualFunc32__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, float f) {
    self->CfObject_setMoveHeadAngle(f * lbl_eu_8066A210);
}

// Retail symbol is Fv but the body tail-calls vtable+0xC8 with a scaled
// float argument (the +0xC8 slot consumes a float in retail despite the
// base header's no-arg decl) - forced-name form.
extern "C" void CfObject_UnkVirtualFunc33__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, float f) {
    self->CfObject_setMoveYaw(f * lbl_eu_8066A210);
}

extern "C" void CfResObj_noop24() {}

extern "C" void CfResObj_noop44() {}

extern "C" void CfObjectMove_setMoveSpeed(cf::CfObjectMove* self, float value) {
    // Unconditional speed set (sibling of CfObjectMove_setMoveSpeedGated without the busy
    // gate): store f1 to mMoveSpeed (+0x6F0), clear bit 0x400 of mFlags68
    // (rlwinm r0,r0,0,22,20) and set bit 0x800 (ori).
    self->mMoveSpeed = value;
    self->mFlags68 = (self->mFlags68 & ~0x400u) | 0x800u;
}

extern "C" void CfObjectMove_setMoveSpeedGated(cf::CfObjectMove* self, float value) {
    // Busy flag (bit 28 of mFlags68) gates the speed set; otherwise clear
    // bit 0x800 and set bits 0x100000+0x400.
    if ((self->mFlags68 & 0x10000000) != 0) {
        return;
    }
    self->mMoveSpeed = value;
    self->mFlags68 = (*(volatile u32*)&self->mFlags68 & ~0x800u) | 0x100400u;
}
extern "C" void CfObjectMove_attachMoveRegion(cf::CfObjectMove* self) {
    // Set bits 20+13 of mFlags68 (clearing bit 14 first), then notify
    // the +0x98 sub-object and attach the position region.
    u32 flags = self->mFlags68;
    self->mFlags68 = (flags & ~0x4000u) | 0x00102000u;
    if (self->mSubObj98 != 0) {
        simSetFlag2OnTree(self->mSubObj98, 1);
    }
    if (lbl_eu_80665958 != 0) {
        func_804B4BDC(lbl_eu_80665958, self->_60C_region);
    }
}
void CfObjectMove_detachMoveRegion(cf::CfObjectMove* self) {
    // Set bit 14 of mFlags68 (clearing bit 13 first; retail keeps both ops).
    // When the global flag bit 26 is set, also clear bit 14 and copy the
    // shared speed constant to +0xA4. Finally detach the position region.
    u32 flags = self->mFlags68;
    flags = (flags & ~0x2000u) | 0x4000u;
    self->mFlags68 = flags;
    if (lbl_eu_80663E28 & 0x04000000u) {
        // const-declared SDA global: MWCC hoists the lfs above the flag
        // store and keeps the stw/stfs order, matching retail exactly.
        float v = lbl_eu_80666A88;
        self->mFlags68 = flags & ~0x4000u;
        *(float*)self->field_0xA4 = v;
    }
    if (lbl_eu_80665958 != 0) {
        Coli_ListRemoveNode(lbl_eu_80665958, self->_60C_region);
    }
}
extern "C" int CfObjectMove_testMoveProximity(cf::CfObjectMove* self) {
    // Distance/state check for the movement update (called by func_800BC8D8):
    // resolves the scene object, then depending on the mFlags68 bits either
    // attaches the position region to the scene's target vector (bit 2), or
    // compares the squared distance between the position-query result and the
    // scene target against a threshold (bit 8), or attaches with a fallback
    // distance that grows when the camera/player differs from self. The flag
    // tests are normalized through the double-cntlzw booleanize idiom (the
    // retail booleanizes each bit test twice) - written with the __cntlzw
    // builtin exactly as CfObjectImplWalker.cpp does.
    void* res;
    int result = 0;
    res = Scn_FindCamItem((void*)CfRes_getD80Flag(), -1);
    if (res != 0) {
        cf::CfResScene10C* scene = (cf::CfResScene10C*)res;
        u32 flags = self->unk64;
        if (((u32)__cntlzw((u32)__cntlzw(flags & 0x4) >> 5) >> 5) != 0) {
            cf::CfObjectMove6B4View* v = (cf::CfObjectMove6B4View*)self;
            u32 w = v->field_6B4;
            v->field_6B4 = w | 0x1000;
            result = ColiCheckMoveRadius(self->_60C_region, &scene->x, 0, 0, lbl_eu_80666A94);
            v->field_6B4 &= ~0x1000;
        } else {
            if (((u32)__cntlzw((u32)__cntlzw(flags & 0x100) >> 5) >> 5) != 0) {
                const nw4r::math::VEC3* pos =
                    (const nw4r::math::VEC3*)self->CfObject_getPosVector();
                nw4r::math::VEC3 scratch;
                nw4r::math::VEC3 delta;
                nw4r::math::VEC3* pDelta = &delta;
                nw4r::math::VEC3* pScratch = &scratch;
                nw4r::math::VEC3Sub(pDelta, (const nw4r::math::VEC3*)&scene->x, pos);
                // Retail materializes the diff to a second stack slot before
                // the length-sq (MWCC emits the lfs/stfs round-trip for the
                // inline-asm output hand-off; component-wise so the copy
                // stays in the float domain).
                scratch.x = delta.x;
                scratch.y = delta.y;
                scratch.z = delta.z;
                f32 distSq = nw4r::math::VEC3LenSq(pScratch);
                result = distSq < lbl_eu_80666A98;
            } else {
                f32 dist = lbl_eu_80666A9C;
                if (func_8007560C() != 0) {
                    // Retail calls getCameraDataBlock (0x80082B7C; camera
                    // block at CfGameManager+0xB0). The old func_800821F8
                    // placeholder name was a misattribution: 0x800821F8 is
                    // retail CfGameManager::setObjectFlags64 (a void setter).
                    void* mgr = getCameraDataBlock__Q22cf13CfGameManagerFv();
                    if (mgr == 0 || (void*)self != reinterpret_cast<cf::CfCam*>(mgr)->cfCam_loadUnk164()) {
                        dist += lbl_eu_80666AA0;
                    }
                } else if ((void*)self != getPlayer__Q22cf13CfGameManagerFi(0)) {
                    dist += lbl_eu_80666AA0;
                }
                result = ColiCheckMoveRadius(self->_60C_region, &scene->x, 1, 0, dist);
            }
        }
    }
    return result;
}

// Movement-speed update on the +0xA8 float. With a non-zero arg the speed is
// decreased by lbl_eu_80666AA4 and clamped below at lbl_eu_80666A88. With a
// zero arg, flag bit 23 of mFlags68 selects either an approach toward
// lbl_eu_80666AA8 (epsilon-snap via ml::epsilon, then step down by
// lbl_eu_80666AAC or up by lbl_eu_80666AB0, clamped at the target) or a
// plain increase clamped at lbl_eu_80666A94.
void CfObjectMove_updateSpeedA8(cf::CfObjectMove* self, u32 arg) {
    cf::CfObjectMoveA8View* view = (cf::CfObjectMoveA8View*)self;
    if (arg != 0) {
        float v = view->field_A8 - lbl_eu_80666AA4;
        view->field_A8 = v;
        if (v < lbl_eu_80666A88) {
            view->field_A8 = lbl_eu_80666A88;
        }
        return;
    }
    if ((self->mFlags68 & 0x00800000) != 0) {
        if (ml::math::abs(view->field_A8 - lbl_eu_80666AA8) <= lbl_eu_8066A208) {
            view->field_A8 = lbl_eu_80666AA8;
        }
        if (view->field_A8 > lbl_eu_80666AA8) {
            float v = view->field_A8 - lbl_eu_80666AAC;
            view->field_A8 = v;
            if (v < lbl_eu_80666AA8) {
                view->field_A8 = lbl_eu_80666AA8;
            }
        } else {
            float v = view->field_A8 + lbl_eu_80666AB0;
            view->field_A8 = v;
            if (v > lbl_eu_80666AA8) {
                view->field_A8 = lbl_eu_80666AA8;
            }
        }
    } else {
        float v = view->field_A8 + lbl_eu_80666AB0;
        view->field_A8 = v;
        if (v > lbl_eu_80666A94) {
            view->field_A8 = lbl_eu_80666A94;
        }
    }
}
extern "C" void func_800BC8D8(cf::CfObjectMove* self) {
    if (self->mSubObj98 == 0) {
        return;
    }
    if ((*(volatile u32*)&lbl_eu_80663E24 & 0x02040000) == 0) {
        u32 flags = self->unk64;
        if ((flags & 0x2) != 0 || (flags & 0x4) != 0 || (flags & 0x8) != 0 ||
            (flags & 0x80000000u) != 0 || (flags & 0x100) != 0) {
            int r = 0;
            if (self->_6F4 != 0) {
                r = 1;
            } else if (CfObjectMove_testMoveProximity(self) != 0) {
                r = 1;
            }
            CfObjectMove_updateSpeedA8(self, r);
        }
    }
    cf::CfObjectMoveA8View* view = (cf::CfObjectMoveA8View*)self;
    float v = lbl_eu_80666A94 - view->field_AC * (view->field_A8 *
              ((lbl_eu_80666A94 - self->field_A0) * view->field_A4));
    if (v > lbl_eu_80666A88) {
        if ((self->mFlags68 & 0x100000) != 0) {
            CfModel_SyncVisFlag(self, 1);
        }
    }
    self->mField718 = v;
    reinterpret_cast<CScnItemModel*>(self->mSubObj98)->vfunc48(v);
    cf::CfObject* sub = (cf::CfObject*)self->mSubObj38;
    if (sub != 0) {
        if (sub->CfObject_getCharEffect() != 0) {
            int r = ((cf::CfObject*)self->mSubObj38)->CfObject_getCharEffect();
            func_eu_8015D258((void*)r, lbl_eu_80666A94 - v);
        }
    }
}
extern "C" void func_800BCD04(cf::CfObjectMove* self) {
    // Position/state refresh. With a model sub-object, a +0x9C word and the
    // busy bit (bit 2) present, snapshot the C4 target's position; then either
    // reset the movement state (bit 3 path: func_800BC9EC + region attach) or
    // re-sync the position region against the model sub-object's movement
    // query. Without the model sub-object the +0x6C0 movement target drives
    // the position difference instead.
    if (self->mSubObj98 != 0 && ((cf::CfObjectMove94View*)self)->field_9C != 0 && (self->mFlags68 & 0x4) != 0) {
        // Snapshot the C4 position once (kept on the stack; retail reloads
        // self->mTargetC4 at every later use instead of caching the pointer).
        cf::CfObjectMoveTargetC4* c4 = (cf::CfObjectMoveTargetC4*)self->mTargetC4;
        ml::CVec3 targetPos;
        targetPos.x = c4->field_3A8;
        targetPos.y = c4->field_3AC;
        targetPos.z = c4->field_3B0;
        // The bit-3 test is normalized through the double-cntlzw idiom (the
        // retail booleanizes it; the top-level tests stay direct).
        if (((u32)__cntlzw((u32)__cntlzw(self->unk64 & 0x8) >> 5) >> 5) != 0) {
            if ((self->mSubObj98->field_7A4 & 0x10000) != 0 && (*(volatile u32*)&lbl_eu_80663E24 & 0x01040000) == 0) {
                if (self->CObjectState_checkStateFlags8(0x1000) == 0) {
                    func_800BC9EC(self);
                    ((bool (*)(void*, void*))setAnimLastPos)(self->mTargetC4, &self->mPos3C);
                    goto done_flags;
                }
            }
        }
if (self->CfObject_checkSubReady() != 0) {
            tickAnimFrame(self->mTargetC4);
            // Copy the C4 position into the move position (integer copy) and
            // store the difference against the snapshot into the +0x54 area
            // (PS sub via the nw4r inline helper, then the float->int bridge).
            c4 = (cf::CfObjectMoveTargetC4*)self->mTargetC4;
            *reinterpret_cast<ml::CVec3*>(&self->mPos3C) =
                *reinterpret_cast<const ml::CVec3*>(&c4->field_3A8);
            // Struct-returning operator- keeps MWCC's return-slot + copy
            // sequence (retail routes the delta through a second stack copy).
            ml::CVec3 delta = *reinterpret_cast<ml::CVec3*>(&self->mPos3C) - targetPos;
            *reinterpret_cast<ml::CVec3*>(&((cf::CfObjectMove54View*)self)->field_54) = delta;
            self->mField4C = ((cf::CfObjectMoveTargetC4*)self->mTargetC4)->field_444;
        } else {
            func_800BC9EC(self);
            ((bool (*)(void*, void*))setAnimLastPos)(self->mTargetC4, &self->mPos3C);
        }
done_flags:
        // Toggle the C4 target's busy bit 6 from the mFlags68 bit 0 / bit 24
        // states (retail keeps both ops and clears bit 0 on the first path).
        u32 f = self->mFlags68;
        if ((f & 0x1) != 0) {
            ((CfObjectMoveC4Flags*)self->mTargetC4)->flags |= 0x40;
            self->mFlags68 = f & ~0x1;
        } else if ((f & 0x01000000) != 0) {
            ((CfObjectMoveC4Flags*)self->mTargetC4)->flags |= 0x40;
        } else {
            ((CfObjectMoveC4Flags*)self->mTargetC4)->flags &= ~0x20;
        }
    } else {
        // No model sub-object: the +0x6C0 target drives the position delta.
        if (self->mTarget6C0 != 0) {
            ml::CVec3 saved;
            saved.x = self->mPos3C;
            saved.y = self->mPos40;
            saved.z = self->mPos44;
            func_800BC9EC(self);
            ml::CVec3 delta = *reinterpret_cast<ml::CVec3*>(&self->mPos3C) - saved;
            *reinterpret_cast<ml::CVec3*>(&((cf::CfObjectMove54View*)self)->field_54) = delta;
            self->mField4C = ((cf::CfObjectMove6C0View*)self->mTarget6C0)->field_C;
        } else {
            // Zero the +0x54 area and reattach the position region.
            *reinterpret_cast<ml::CVec3*>(&((cf::CfObjectMove54View*)self)->field_54) = ml::CVec3::zero;
            ColiSetAxisBlockInverse(self->_60C_region, &self->mPos3C);
        }
    }
}

extern "C" void func_800BC9EC(cf::CfObjectMove* self) {
    if (self->mTarget6C0 == 0) {
        return;
    }
    // Snapshot the position query result and the movement target's state.
    const ml::CVec3* pos = (const ml::CVec3*)self->CfObject_getPosVector();
    ml::CVec3 origPos;
    origPos.x = pos->x;
    origPos.y = pos->y;
    origPos.z = pos->z;
    cf::CfObjectMove6C0View* tgt = (cf::CfObjectMove6C0View*)self->mTarget6C0;
    tgt->field_14 = lbl_eu_80666A88;
    ((cf::CtrlMoveTarget*)tgt)->resetMoveTarget();
    // Declared angle-first so MWCC colours rate into f30 (retail order).
    f32 angle = tgt->field_C;
    f32 rate = tgt->field_14;
    if (getPlayer__Q22cf13CfGameManagerFi(0) == (void*)self && self->CfObject_isMoveActiveNow() == 0) {
        rate = lbl_eu_80666A88;
        ColiSetAxisBlockInverse(self->_60C_region, &self->mPos3C);
    }
    int force = 0;
    void* enemy = func_800AD860((void*)self);
    if (enemy != 0) {
        if (((cf::CfObjectMoveAD86View*)enemy)->field_45CA & 1) {
            force = 1;
        }
    }
    if (force == 0 && ((u32)__cntlzw((u32)__cntlzw(self->unk64 & 0x8) >> 5) >> 5) != 0 && self->field_6CE == 3) {
        force = 1;
    }
    if (ml::math::abs(rate) > lbl_eu_8066A208 || force != 0) {
        // Circular sweep: scale the position X by the rate, sweep the angle
        // through Sin/Cos, then step the +0x60C region toward the swept point.
        float sx = rate * *(const float*)self->CfObject_getMoveRateScale();
        rate = lbl_eu_80666AB8 * sx;
        f32 ang = lbl_eu_80666ABC * angle;
        ml::CVec3 step;
        step.x = rate * SinFIdx__Q24nw4r4mathFf(ang);
        step.y = lbl_eu_80666A88;
        step.z = rate * CosFIdx__Q24nw4r4mathFf(ang);
        const ml::CVec3* base = (const ml::CVec3*)self->CfObject_getPosVector();
        ml::CVec3 tmp;
        func_804B1164(self->_60C_region, &tmp, base, &step);
        step.x += tmp.x;
        step.y += tmp.y;
        step.z += tmp.z;
        if (((u32)__cntlzw((u32)__cntlzw(self->unk64 & 0x08000000) >> 5) >> 5) == 0) {
            step.y -= lbl_eu_80666AA4;
        }
        u32 id = 0x44A09;
        if (((u32)__cntlzw((u32)__cntlzw(self->unk64 & 0x2) >> 5) >> 5) != 0) {
            id = 0x44A05;
        } else if (((u32)__cntlzw((u32)__cntlzw(self->unk64 & 0x4) >> 5) >> 5) != 0) {
            id = 0x44A11;
        }
        float f3 = ((u32)__cntlzw((u32)__cntlzw(self->unk64 & 0x08000000) >> 5) >> 5) != 0 ? lbl_eu_80666A88 : lbl_eu_80666AC0;
        func_804BD94C(&self->mPos3C, &step, id, 0, 0, 0,
                      lbl_eu_80666AA8, lbl_eu_80666AA0, f3, lbl_eu_8066AF20, lbl_eu_80666AC4);
    }
    // Store the position difference against the snapshot into the +0x54 area.
    ml::CVec3 diff;
    diff.x = self->mPos3C - origPos.x;
    diff.y = self->mPos40 - origPos.y;
    diff.z = self->mPos44 - origPos.z;
    *reinterpret_cast<ml::CVec3*>(&((cf::CfObjectMove54View*)self)->field_54) = diff;
}

extern "C" void func_800BCFA0(cf::CfObjectMove* self) {
    if (self->mSubObj98 == 0) {
        return;
    }
    int wasC4Null = self->mTargetC4 == 0;
    ColiNodeSetWord8Rebuild(self->_60C_region);
    if (((cf::CfObjectMove94View*)self)->field_9C != 0 && self->mTargetC4 == 0) {
        // Construct the +0xD0 CActParamAnimGame sub-object as the C4 target.
        self->mTargetC4 = self->_D0;
        // Fresh C4 target is the +0xD0 CActParamAnimGame (see above).
        ((::CActParamAnim*)self->mTargetC4)->func_8004B114();
        ((cf::CfObjectMoveD0View*)self->mTargetC4)->field_4E8 = self;
        u32 flags = self->unk64;
        if ((flags & 0x2) != 0) {
            void* c4 = self->mTargetC4;
            if (getPlayer__Q22cf13CfGameManagerFi(0) == (void*)self) {
                startAnimModeA(c4);
                ((cf::CfObjectMoveD0View*)self->mTargetC4)->field_530 |= 0x400;
            } else {
                startAnimModeA2(c4);
            }
            ((cf::CfObjectMoveD0View*)self->mTargetC4)->field_4EC |= 0x4000;
            void* c4b = self->mTargetC4;
            ((cf::CfObjectMoveD0View*)c4b)->field_4EC =
                (((cf::CfObjectMoveD0View*)c4b)->field_4EC & 0xFFF5FFFF) | 0x00040000;
        } else if ((flags & 0x4) != 0) {
            startAnimModeC(self->mTargetC4);
        } else if ((flags & 0x8) != 0 || (flags & 0x1) != 0) {
            startAnimModeB(self->mTargetC4);
        } else if ((flags & 0x100) != 0) {
            startAnimModeA(self->mTargetC4);
        }
    }
    if (self->mTargetC4 != 0) {
        u32 w6C = ((cf::CfObjectMoveFlags6C*)self)->field_6C;
        self->mFlags68 |= 0x4;
        if ((w6C & 0x10000) != 0 && (self->unk64 & 0x2) != 0) {
            // Both model pointers are loaded before the virtual dispatch and
            // stay live across it (retail colours them into r28/r29).
            void* field6D4 = self->mField6D4;
            void* sub98 = self->mSubObj98;
            void* anim = ((cf::CfObjectModel*)self)->CfObjectModel_getAnimState();
            attachAnimObj(self->mTargetC4, sub98, field6D4, (u32)anim);
            pushAnimNode(self->mTargetC4, (void*)((cf::CfObjectMove94View*)self)->field_9C, (void*)((cf::CfObjectMove94View*)self)->field_94);
        } else {
            attachAnimObj(self->mTargetC4, self->mSubObj98, (void*)((cf::CfObjectMove94View*)self)->field_9C, ((cf::CfObjectMove94View*)self)->field_94);
        }
    }
    u32 f68 = self->mFlags68;
    if ((f68 & 0x10) != 0) {
        self->mFlags68 &= ~0x111;
        static_cast<cf::CfObject*>(self)->CfObject_syncMoveTarget((const ml::CVec3*)&self->mPos3C);
    } else if ((f68 & 0x00800000) != 0) {
        u32 flags2 = self->unk64;
        self->mFlags68 &= ~0x00800000u;
        f32 rate2 = (flags2 & 0x2) != 0 ? lbl_eu_80666AC8 : lbl_eu_80666ACC;
        self->CfObject_applyMoveOffset((const ml::CVec3*)&self->mPos3C, rate2);
    } else {
        static_cast<cf::CfObject*>(self)->CfObject_syncMoveTarget((const ml::CVec3*)&self->mPos3C);
    }
    self->CfObject_setMoveYaw(self->mField4C);
    self->CfObject_setRotVec((void*)self->_pad48);
    if (self->mTargetC4 != 0) {
        if (wasC4Null) {
            if (self->mTarget6C0 != 0) {
                setAnimOwner(self->mTargetC4, self->mTarget6C0);
            }
            float x = *(float*)self->CfObject_getMoveRateScale();
            cf::CfObjectMoveTargetC4* c4 = (cf::CfObjectMoveTargetC4*)self->mTargetC4;
            c4->field_430 = x;
            c4->field_43C = c4->field_438 * x;
            float y = *(float*)self->CfObject_getMoveRateScale();
            ((cf::CfObjectMoveTargetC4*)self->mTargetC4)->field_434 = lbl_eu_80666AD0 * y;
            void* c4b = self->mTargetC4;
            ((cf::CfObjectMoveC8View*)c4b)->field_4 = self->mSubObj38;
            ((cf::CfObjectMoveC8View*)c4b)->field_34 = self->mSubObj38;
            if ((self->unk64 & 0x100) != 0) {
                ((cf::CfObjectMoveD0View*)self->mTargetC4)->field_504 = lbl_eu_80666AA4;
                ((cf::CfObjectMoveD0View*)self->mTargetC4)->field_50C = lbl_eu_80666A9C;
            }
        }
        // Push the mode bytes to the CActParamAnim helper, then reset them
        // unless the busy bit is set.
        int top3 = (self->mFlags6C9 >> 5) & 7;
        ((void (*)(void*, u32, u32, u32, u32))func_8004B9D4)(
            self->mTargetC4, self->mField6C4, top3 != 0, (s8)self->mField6C8, 0);
        if ((self->unk64 & 0x8) == 0) {
            self->mField6C4 = 1;
            self->mField6C8 = -1;
            self->mFlags6C9 &= ~0xE0;
        }
    }
    if ((self->mFlags68 & 0x1000000) != 0) {
        self->CfObject_setMoveBusyState(1);
    }
    if ((self->mFlags68 & 0x2000000) != 0) {
        ((cf::CfObject*)self)->CfObject_setMoveReadyFlag(1);
    }
    simSetLeafDist7B0(self->mSubObj98, lbl_eu_80666A94);
    // Real spelling for slot 0x168 (inline alias in CfObject.hpp;
    // identical virtual dispatch to CfObject_UnkVirtualFunc70).
    self->CfObject_syncModelRate(self->field_A0);
    ((cf::CfObjectModel*)self)->CfObjectModel_setModelRate(lbl_eu_80666A94);
    u32 f = self->mFlags68;
    if ((f & 0x800) != 0) {
        int bit14 = (f >> 14) & 1;
        if (bit14 != 0) {
            self->mFlags68 |= 0x20000;
        } else {
            self->mFlags68 &= ~0x20000;
        }
        if (self->mSubObj98 != 0) {
            if (bit14 != 0) {
                ((CScnItemModel*)self->mSubObj98)->vfunc50(1);
                cf::CfObjectModelSub98* sub = self->mSubObj98;
                if (sub != 0) {
                    void* p = ((cf::CfObjectMoveSub98View7EC*)sub)->field_7EC;
                    if (p != 0) {
                        ((cf::CfObjectMove7ECView*)p)->field_2A = (u8)self->field_6CC;
                    }
                }
            } else {
                ((CScnItemModel*)self->mSubObj98)->vfunc50(0);
            }
        }
    }
    if ((self->mFlags68 & 0x20000) == 0) {
        if (self->mSubObj98 != 0) {
            if (((CScnItemModel*)self->mSubObj98)->vfunc54() != 0) {
                self->mFlags68 |= 0x20000;
                if (self->mSubObj98 != 0) {
                    ((CScnItemModel*)self->mSubObj98)->vfunc50(1);
                    cf::CfObjectModelSub98* sub = self->mSubObj98;
                    if (sub != 0) {
                        void* p = ((cf::CfObjectMoveSub98View7EC*)sub)->field_7EC;
                        if (p != 0) {
                            ((cf::CfObjectMove7ECView*)p)->field_2A = (u8)self->field_6CC;
                        }
                    }
                }
            }
        }
    }
    if ((self->unk64 & 0x4) != 0) {
        cf::CfObjectModelSub98* sub = self->mSubObj98;
        void* p = ((cf::CfObjectMoveSub98View7EC*)sub)->field_7EC;
        if (p != 0) {
            ((CActParam7ECTarget*)p)->func08(1, lbl_eu_8066AF20);
        }
    }
    if (((self->mFlags6C9 >> 3) & 0xF) != 5) {
        reinterpret_cast<CScnItemModel*>(self->mSubObj98)->vfunc64(0);
    }
    self->setPointEnabled((self->mFlags68 >> 11) & 1);
    if (CfObj_KindBit1(self) != 0 || CfObj_KindBit3(self) != 0) {
        ((CScnItemModel*)self->mSubObj98)->vfunc9C(0, 0);
    } else if (CfObj_KindBit2(self) != 0) {
        ((CScnItemModel*)self->mSubObj98)->vfunc9C(1, 0);
    } else if (CfObj_KindBit7(self) != 0) {
        ((CScnItemModel*)self->mSubObj98)->vfunc9C(2, 0);
        const u8* s = (const u8*)getBdatStringColumnValue(lbl_eu_806640A8, lbl_eu_804FC550, lbl_eu_80664184);
        u8 b = *s;
        if ((b & 0x2) != 0) {
            ((cf::CfObjectMoveSub98View7A0*)self->mSubObj98)->field_7A0 &= ~0x10000000;
        }
        if ((b & 0x8) != 0) {
            ScnEnvLgt_EnableFlag100(&((cf::CfObjectMoveSub98View31C*)self->mSubObj98)->field_31C, 1);
        }
    }
    if (self->mSubObj38 != 0) {
        ((cf::CfObject*)self->mSubObj38)->CfObject_isMoveActiveNow();
    }
}
// retail: lhz r0,0x1678(r3); extrwi r3,r0,1,28 = ((u16 at +0x1678)>>3)&1
extern "C" u32 CfObjectMove_getStatusBit1678(void* self) { return (*(const u16*)((const char*)self + 0x1678) >> 3) & 1; }
// Walk the two +0x6F8 animation slots; when the matching +0xC8/+0xCC
// target is missing, allocate and construct it (field_378 = slot index,
// +0x4/+0x34 = the +0x38 sub-object, +0x4F4 = self), then advance the
// animation model (AnimGame_SetStateFlags), attach the model list via the vtable
// +0x188 query and attachAnimObj, and reset the C4 target's pages via
// getAnimModelId + the 5-arg func_8004B9D4 form. Retail re-reads the
// member target for the object field writes (no register reuse), so the
// writes go through the member re-derivation.
void CfObjectMove_ensureAnimTargets(cf::CfObjectMove* self) {
    // Walk the two +0x6F8 animation slots; both per-slot reads (the slot
    // array and the +0xC8/+0xCC target pair) index the same i so MWCC
    // strength-reduces them onto one stepped cursor.
    for (int i = 0; i < 2; i++) {
        void* slot = self->mField6F8[i];
        if (slot != 0 && self->mField6D8 != 0) {
            if ((&self->mTargetC8)[i] == 0) {
                // obj declared/zeroed before the allocation sequence (same
                // scheme as CfObject_createMoveTarget) so MWCC colours the
                // alloc result deterministically.
                void* obj = 0;
                obj = allocate__Q23mtl10MemManagerFUlUl(0x53C, CfRes_getHeapHandle());
                if (obj != 0) {
                    __ct__8005A3FC(obj, self);
                }
                // Retail stores the (possibly null) result to the slot
                // BEFORE the field writes, and writes field_378 straight
                // from the alloc result; the remaining fields re-read the
                // member (no register reuse).
                (&self->mTargetC8)[i] = obj;
                ((cf::CfObjectMoveC8View*)obj)->field_378 = i;
                void* ta = (&self->mTargetC8)[i];
                ((cf::CfObjectMoveC8View*)ta)->field_4 = self->mSubObj38;
                ta = (&self->mTargetC8)[i];
                ((cf::CfObjectMoveC8View*)ta)->field_34 = self->mSubObj38;
                ta = (&self->mTargetC8)[i];
                ((cf::CfObjectMoveC8View*)ta)->field_4F4 = self;
            }
            if ((&self->mTargetC8)[i] != 0) {
                AnimGame_SetStateFlags((&self->mTargetC8)[i]);
                u32 anim = (u32)reinterpret_cast<cf::CfObjectModel*>(self)->CfObjectModel_getAnimFlags();
                attachAnimObj((&self->mTargetC8)[i], slot, self->mField6D8, anim);
                if (self->mTargetC4 != 0) {
                    // Retail: bl getAnimModelId; mr r4,r3; lwz r3,<target>;
                    // li r5/r6/r7; bl func_8004B9D4.
                    ((void (*)(void*, void*, int, int, int))func_8004B9D4)(
                        (&self->mTargetC8)[i], (void*)getAnimModelId(self->mTargetC4), 0, -1, 0);
                }
            }
        }
    }
    if (self->mSubObj98 != 0) {
        func_80482918(self->mSubObj98, 1);
    }
}

extern "C" void CfObjectMove_setMoveModeField(cf::CfObjectMove* self, u32 value) {
    // Insert (value << 1) into bits 27-30 of mFlags6C9, then forward the
    // inserted nibble to the +0x98 sub-object's vtable +0x64 (tail call).
    cf::CfObjectModelSub98* target = self->mSubObj98;
    u32 flags = __rlwimi(self->mFlags6C9, value, 1, 27, 30);
    self->mFlags6C9 = (u8)flags;
    if (target != 0) {
        // Retail: rlwinm r4,r0,31,28,31 = (flags >> 1) & 0xF - the flag-word
        // nibble at bits 1-4 (untouched by the 27-30 insertion), NOT the
        // inserted field nibble ((flags >> 27) & 0xF is a different value).
        reinterpret_cast<CScnItemModel*>(target)->vfunc64((flags >> 1) & 0xF);
    }
}
void CfObjectMove_setAnimModeArgs(u8* obj, int a, int b, int c, int d) {
    // Store the two 6C4/6C8 fields and merge `b` into the top bits of
    // mFlags6C9 (rlwimi), then when a C4 target exists forward the incoming
    // args to the CActParamAnim reset helper and, unless the +0x64 flag bit
    // 3 is set, reset 6C4/6C8 to 1/-1 and clear the merged bits.
    cf::CfObjectMove* self = (cf::CfObjectMove*)obj;
    void* target = self->mTargetC4;
    self->mField6C4 = a;
    self->mField6C8 = c;
    self->mFlags6C9 = (u8)__rlwimi(self->mFlags6C9, b, 5, 24, 26);
    if (target != 0) {
        ((void (*)(void*, int, int, int, int))func_8004B9D4)(target, a, b, c, d);
        if ((self->unk64 & 0x8) == 0) {
            self->mField6C4 = 1;
            self->mField6C8 = -1;
            // Bitfield clear: MWCC emits the retail rlwinm r0,r0,0,27,23
            // directly (a plain `& ~0xE0` on the byte field folds the mask
            // to 27,31 via byte-range analysis).
            ((cf::CfFlags6C9*)&self->mFlags6C9)->top3 = 0;
        }
    }
}

// retail: lwz r3,0xB0(r3); lhz r3,0xC(r3); blr
extern "C" u16 CfObjectMove_getSubB0FieldC(void* self) { return *(u16*)((u8*)*(void**)((u8*)self + 0xB0) + 0xC); }
extern "C" void CfObjectMove_resetAnimModeArgs(cf::CfObjectMove* self) {
    // Store 1 at +0x6C4, -1 at +0x6C8, and clear bits 24-26 of mFlags6C9.
    // Bitfield clear: MWCC emits the retail rlwinm r0,r0,0,27,23 directly
    // (a plain AND on the byte field folds the mask to 27,31).
    self->mField6C4 = 1;
    self->mField6C8 = -1;
    ((cf::CfFlags6C9*)&self->mFlags6C9)->top3 = 0;
}
void CfObjectMove_forwardNpcSignal(cf::CfObjectMove* self) {
    cf::CtrlNpc* target = (cf::CtrlNpc*)self->mTarget6C0;
    if (target != 0) {
        CtrlNpcSetFlagActivate(target);
    }
}
// Toggles bit 17 of mFlags68 from the incoming flag, then when a +0x98
// sub-object exists forwards the flag to its vtable slot +0x50. On the
// set path, the sub-object is re-read (retail reloads +0x98 after the call)
// and its +0x7EC pointer's +0x2A byte receives the low byte of the +0x6CC
// u16 field.
void CfObjectMove_setModelDisplayFlag(cf::CfObjectMove* self, int flag) {
    if (flag != 0) {
        self->mFlags68 |= 0x00020000;
    } else {
        self->mFlags68 &= ~0x00020000;
    }
    if (self->mSubObj98 != 0) {
        if (flag != 0) {
            reinterpret_cast<CScnItemModel*>(self->mSubObj98)->vfunc50(1);
            cf::CfObjectModelSub98* sub = self->mSubObj98;
            if (sub != 0) {
                struct Sub98Field7EC {
                    u8 _pad[0x7EC];
                    void* field_7EC;  // 0x7EC
                };
                struct Field2A {
                    u8 _pad[0x2A];
                    u8 field_2A;  // 0x2A
                };
                void* p = ((Sub98Field7EC*)sub)->field_7EC;
                if (p != 0) {
                    ((Field2A*)p)->field_2A = (u8)self->field_6CC;
                }
            }
        } else {
            reinterpret_cast<CScnItemModel*>(self->mSubObj98)->vfunc50(0);
        }
    }
}
// Retail symbol is Fv but the body consumes r4 (a flag): with a flag set the
// +0x6C bit 16 is raised and the +0xB0 sub-object's vtable +0x38 is invoked;
// with it clear the model list (0x6D4) is released through the C4 target and
// the list destructor, then bit 16 is lowered.
void CfObjectMove_setModelListLock(cf::CfObjectMove* self, int flag) {
    if (flag != 0) {
        ((cf::CfObjectMoveFlags6C*)self)->field_6C |= 0x10000;
        reinterpret_cast<cf::CfResObjImpl*>(self->mSubObjB0)->CfResObj_false38();
    } else {
        if (self->mField6D4 != 0) {
            if (self->mTargetC4 != 0) {
                releaseAnimObj(self->mTargetC4, self->mField6D4);
            }
            Scn_IsAnimActiveOrNull(self->mField6D4);
            self->mField6D4 = 0;
        }
        ((cf::CfObjectMoveFlags6C*)self)->field_6C &= ~0x10000;
    }
}
// Forward a position region (self+0x60C) to the region-library manager
// (lbl_eu_80665958); flag selects the attach (func_804B4BDC) vs detach
// (Coli_ListRemoveNode) helper.
extern "C" void CfObjectMove_setRegionAttached(void* obj, int flag) {
    cf::CfObjectMove* self = (cf::CfObjectMove*)obj;
    if (lbl_eu_80665958 != 0) {
        if (flag != 0) {
            func_804B4BDC(lbl_eu_80665958, self->_60C_region);
        } else {
            Coli_ListRemoveNode(lbl_eu_80665958, self->_60C_region);
        }
    }
}
int CfObjectMove_checkSubCounter(cf::CfObjectMove* self, int value) {
    // Sub-object state query: returns whether the sub's +0x2C counter equals
    // `value`, or (for negative values) whether the counter is non-negative.
    struct SubObj { int dummy0[11]; int field_2c; };
    SubObj* sub = (SubObj*)self->mSubObj38;
    if (sub != 0) {
        if (value >= 0) {
            return (sub->field_2c - value) == 0;
        }
        return ((u32)sub->field_2c >> 31) ^ 1;
    }
    return 0;
}
extern "C" void CfResObj_noop10() {}
extern "C" bool CfResObj_false38() { return false; }
extern "C" void CfResObj_noop58() {}
extern "C" int CfResObj_true68() { return 1; }
// Look up a bdat column (offset 0x7 into the column-name table) for the
// given id/flag; tail-calls the shared bdat reader.
extern "C" u32 CfObjectMove_lookupBdatCol7(u32 id, u32 flag) {
    return func_8014235C(id, &lbl_eu_804FC550[0x7], flag);
}
// Column-name lookup through the bdat reader: index self->field_710[flag],
// resolve column offset 0x7 of the name table, then map the u16 result to a
// name string via CfBdat::getBdatStringEntry.
const char* CfObjectMove_getBdatNameCol7(cf::CfObjectMove* self, u32 flag) {
    u32 result = func_8014235C(self->field_710[flag], &lbl_eu_804FC550[0x7], flag);
    return cf::CfBdat::getBdatStringEntry((u16)result);
}

// Same as CfObjectMove_getBdatNameCol7 but column offset 0x11.
const char* CfObjectMove_getBdatNameCol11(cf::CfObjectMove* self, u32 flag) {
    u32 result = func_8014235C(self->field_710[flag], &lbl_eu_804FC550[0x11], flag);
    return cf::CfBdat::getBdatStringEntry((u16)result);
}
// Same as CfObjectMove_lookupBdatCol7 but column offset 0x11.
extern "C" u32 CfObjectMove_lookupBdatCol11(u32 id, u32 flag) {
    return func_8014235C(id, &lbl_eu_804FC550[0x11], flag);
}
extern "C" void CfResObj_noop40() {}
extern "C" void CfResObj_noop28() {}

// typed data sections - source shape for retail bytes (replaces absorb blobs)
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_804FC550[0x20] = "mapATR\0def_mount\0bat_mount";

__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80666A88 = 0.0f;
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80666A8C = 0.9f;
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80666A90 = 30.0f;
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80666A94 = 1.0f;
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80666A98 = 3.2399998f;
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80666A9C = 0.4f;
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80666AA0 = 1.4f;
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80666AA4 = 0.1f;
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80666AA8 = 0.7f;
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80666AAC = 0.03f;
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80666AB0 = 0.06f;
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80666AB4 = 0.05f;
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80666AB8 = 0.009259259f;
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80666ABC = 40.743664f;
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80666AC0 = -0.1f;
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80666AC4 = 0.3f;
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80666AC8 = 10.0f;
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80666ACC = 10000.0f;
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80666AD0 = 0.5f;
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80666AD4 = 4.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(8))) const float lbl_eu_80666AD8[2] = {2.4f, 0.0f};
