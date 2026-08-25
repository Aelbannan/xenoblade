// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectMove
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "monolib/scn/CScnTimeApi.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "monolib/math.hpp"
#include <nw4r/math.h>
#include <math.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
#include "monolib/math/FloatUtils.hpp"  // H3 label-owner decl (lbl_eu_8066A208)

struct OMIfShift { char pad[0x10]; };
struct OMIf : OMIfShift {
    virtual void _v0008();
    virtual void _v000C();
    virtual void _v0010();
    virtual void vf0014();
};

typedef void (*VFn)(void*);
// Cast-only SI iface for the +0xB0/+0x10 double-hop thunks.
// Shift base puts the vptr at object+0x10 (retail lwz r12, 0x10(r3));
// RTTI omit keeps slots at vtable+0x08, +0x04 each.
struct Shift { char pad[0x10]; };
struct ObjVtIf : Shift {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void vf14();
    virtual void vf18(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void vf28(); virtual void vf2C(); virtual void vf30(); virtual void vf34();
    virtual void _v038(); virtual void _v03C(); virtual void vf40(); virtual void _v044();
    virtual void vf48(); virtual void _v04C(); virtual void _v050(); virtual void vf54();
    virtual void vf58(); virtual void _v05C(); virtual void vf60(); virtual void _v064();
    virtual void vf68();
};

// Flags word at +0xC of the mTargetC4 object (CfObject_UnkVirtualFunc64/65
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
    // (CObjectState_UnkVirtualFunc5) is invoked with flag 3.
    *(void**)this = (void*)lbl_eu_80529690;
    mTargetC4 = 0;
    __ct__Q22cf17CActParamAnimGameFv((cf::CActParamAnimGame*)((u8*)this + 0xD0));
    func_804B0924(_60C_region);
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
    CObjectState_UnkVirtualFunc5(3);
}

cf::CfObjectMove::~CfObjectMove() {
    // Explicit vtable restore, virtual teardown (slot +0x68), region detach,
    // then the hand-rolled member/base destructor chain (region at +0x60C,
    // CActParamAnimGame at +0xD0, CfObjectModel base) with the retail flag
    // literals; MWCC appends the delete-flag wrapper itself.
    *(void**)this = (void*)lbl_eu_80529690;
    CfObject_UnkVirtualFunc6();
    if (lbl_eu_80665958 != 0) {
        func_804B4C7C(lbl_eu_80665958, (void*)this->_60C_region);
    }
    __dt__804B095C((void*)this->_60C_region, -1);
    __dt__Q22cf17CActParamAnimGameFv((void*)((u8*)this + 0xD0), -1);
    __dt__Q22cf13CfObjectModelFv((void*)this, 0);
}

void cf::CfObjectMove::CfObject_UnkVirtualFunc4() {
    // Per-frame movement update: run the base position/state pair, then (when
    // no menu/battle state is active) refresh the movement (func_800BCD04) and
    // release the two +0xC8/+0xCC targets. With a model sub-object present the
    // +0x1AC slot forwards the +0xB4/+0xB8 words, and the +0x38 sub-object
    // receives the mode-dependent advance notification before the movement
    // value is fed to the CCharEffect slot helper.
    this->CfObject_UnkVirtualFunc12();
    this->CfObject_UnkVirtualFunc5();
    if (func_800829B8__Q22cf13CfGameManagerFv() == 0) {
        func_800BCD04(this);
        // Pointer-induction release walk: the base pointer starts at this and
        // the fixed +0xC8 member offset stays folded into the load (retail
        // steps the pointer by 4 with a separate counter).
        void** slot = (void**)this;
        int i = 0;
        while (i < 2) {
            if (slot[50] != 0) {
                func_8004CF00(slot[50]);
            }
            i++;
            slot++;
        }
    }
    if (func_800BB934(this) != 0) {
        u32 b4 = ((cf::CfObjectMoveB4View*)this)->field_B4;
        if (b4 != 0) {
            ((cf::CfObjectMoveVt1AC*)this)->m1AC(b4, ((cf::CfObjectMoveB4View*)this)->field_B8);
        }
    }
    if (mSubObj38 != 0) {
        // Global-mode gate: the two flag reads are kept separate (volatile
        // casts; the extern itself is non-volatile to match CSystemWindow.hpp
        // and the CUICfManager.cpp definition) so MWCC emits the retail double
        // lwz of lbl_eu_80663E24 (r0 for the mode test, r5 for the +0x40000
        // advance gate).
        u32 g0 = *(volatile u32*)&lbl_eu_80663E24;
        u32 g1 = *(volatile u32*)&lbl_eu_80663E24;
        int cond = (g0 & 0x02040000) != 0;
        if ((g0 & 0x10000000) != 0) {
            cond = 0;
        }
        if (cond == 0) {
            ((cf::CfObjectSub38If*)mSubObj38)->_fA4();
        } else if (((this->unk64 & 0x8) != 0 || (this->unk64 & 0x4) != 0) && (g1 & 0x40000) != 0) {
            ((cf::CfObjectSub38If*)mSubObj38)->_fA4();
        }
        if ((this->unk64 & 0x2) != 0 || (this->unk64 & 0x4) != 0) {
            int r28 = 0;
            if (mSubObj98 != 0) {
                r28 = (mSubObj98->field_7A4 & 0x04000000) == 0;
            }
            if (((cf::CfObjectSub38VtE4*)mSubObj38)->_fE4() != 0) {
                int r27 = 0;
                if (mSubObj98 != 0) {
                    if (((cf::CfObjectMoveSub98Vt4C*)mSubObj98)->m4C() > lbl_eu_80666A8C) {
                        r27 = 1;
                    }
                }
                int r30 = 0;
                if (cond == 0 && ((cf::CfObjectMoveVt160*)this)->m160() != 0 && r28 != 0 && r27 == 0) {
                    r30 = 1;
                }
                int q = ((cf::CfObjectSub38VtE4*)mSubObj38)->_fE4();
                func_8015BD24((void*)q, r30);
            }
        }
    }
}

void cf::CfObjectMove::CfObject_UnkVirtualFunc7() {
    // Forward to the sub-object's vtable slot +0xA8 (tail virtual call).
    CfObjectSub38If* sub = (CfObjectSub38If*)mSubObj38;
    if (sub != 0) {
        sub->_fA8();
    }
}

void cf::CfObjectMove::CfObject_UnkVirtualFunc6() {
    // Teardown pair (vtable +0x17C then +0x178), then release the C4 target
    // (vtable +0xE0) and the NPC movement target (vtable +0x8, flag 1),
    // clearing both pointers.
    CfObjectModel_UnkVirtualFunc2();  // vtable +0x17C
    CfObjectModel_UnkVirtualFunc1();  // vtable +0x178
    if (mTargetC4 != 0) {
        reinterpret_cast<cf::CfObjectSub38VtE4*>(mTargetC4)->_fE0();
        mTargetC4 = 0;
    }
    if (mTarget6C0 != 0) {
        // Redundant nested check mirrors retail's two beq targets.
        if (mTarget6C0 != 0) {
            reinterpret_cast<cf::CfObjectSub38If*>(mTarget6C0)->m08(1);
        }
        mTarget6C0 = 0;
    }
}

void cf::CfObjectMove::CfObjectModel_UnkVirtualFunc1() {
    reinterpret_cast<cf::CfObjectMoveVt1D0*>(this)->m1D0(0);
    reinterpret_cast<cf::CfObjectMoveVt1D0*>(this)->m1D0(1);
    for (int i = 0; i < 2; i++) {
        void* t = (&mTargetC8)[i];
        // Redundant nested checks mirror retail's three beq targets (MWCC
        // keeps them; the final clear only runs when the first two pass).
        if (t != 0) {
            if (t != 0) {
                if (t != 0) {
                    reinterpret_cast<cf::CfObjectSub38If*>(t)->m08(1);
                }
                (&mTargetC8)[i] = 0;
            }
        }
    }
    cf::CfObjectSub38VtE4* sub = (cf::CfObjectSub38VtE4*)mSubObj38;
    if (sub != 0 && mFieldC0 != 0) {
        if (sub->_fE4() != 0) {
            int r = ((cf::CfObjectSub38VtE4*)mSubObj38)->_fE4();
            func_8015C8F4((void*)r, mSubObj98);
        }
    }
    CfObjectModel_UnkVirtualFunc1__Q22cf13CfObjectModelFv(this);
    mFlags68 &= ~0x4;
    func_804B0A7C(_60C_region);
    if (mTargetC4 != 0) {
        reinterpret_cast<cf::CfObjectSub38VtE4*>(mTargetC4)->_fE0();
        mTargetC4 = 0;
    }
}

void cf::CfObjectMove::CfObjectModel_UnkVirtualFunc2() {
    // Refresh the two animation slots (+0x1CC before the release work,
    // +0x1C8 after), releasing both model lists (+0x9C from the base class
    // and +0x6D4) through the C4 target, then hop the +0xB0 sub-object's
    // vtable slot +0x24.
    reinterpret_cast<cf::CfObjectMoveVt208*>(this)->m1CC();
    // field_0x9C is the u8[4] base-class slot; access it as the pointer it
    // holds (model list released through the C4 target).
    void*& list9C = *reinterpret_cast<void**>(field_0x9C);
    if (list9C != 0) {
        if (mTargetC4 != 0) {
            func_8004B6BC(mTargetC4, list9C);
        }
        func_80495E60(list9C);
        list9C = 0;
    }
    if (mField6D4 != 0) {
        if (mTargetC4 != 0) {
            func_8004B6BC(mTargetC4, mField6D4);
        }
        func_80495E60(mField6D4);
        mField6D4 = 0;
    }
    CfObjectModel::CfObjectModel_UnkVirtualFunc2();
    reinterpret_cast<cf::CfObjectMoveVt208*>(this)->m1C8();
    reinterpret_cast<ObjVtIf*>(mSubObjB0)->_v024();
}

void CfObjectMove_nullsub_1() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc2() {
    // When the model list (0x6D8) is present, notify each of the two
    // +0xC8/+0xCC targets, release the list, and clear the list fields.
    if (mField6D8 != 0) {
        for (int i = 0; i < 2; i++) {
            void* t = (&mTargetC8)[i];
            if (t != 0) {
                func_8004B6BC(t, mField6D8);
            }
        }
        func_80495E60(mField6D8);
        mField6D8 = 0;
        mField704 = 0;
    }
}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc1() {
    // Forward to the +0xB0 sub-object's vtable slot +0x44, then clear the
    // two resource-request fields (retail stores 0 to +0x6DC / +0x6E0).
    reinterpret_cast<ObjVtIf*>(*(void**)((u8*)this + 0xb0))->_v044();
    mField6DC = 0;
    mField6E0 = 0;
}

void CfObjectMove_nullsub_2() {}

void CfObjectMove_nullsub_3(){}

void cf::CfObjectMove::setMoveSpeed(float value)
{
    mMoveSpeed = value;
    unsigned int flags = mFlags68;
    flags = (flags & ~0x400u) | 0x800u;
    mFlags68 = flags;
}

void CfObjectMove_nullsub_4(){}

void CfObjectMove_nullsub_5(){}

// (the shared speed constant lbl_eu_80666A88 is only declared in the
// header as extern const float; retail keeps the single .sdata2 copy in
// split1.s, so this TU must not define a namespace-cf storage copy)

void cf::CfObjectMove::resetMoveSpeed() {
    u32 flags = mFlags68;
    f32 value = lbl_eu_80666A88;
    mMoveSpeed = value;
    mFlags68 = flags & ~0xC00;
}

void CfObjectMove_nullsub_6(){}

void CfObjectMove_nullsub_7(){}

void CfObjectMove_nullsub_8(){}

void cf::CfObjectMove::CfObject_UnkVirtualFunc5() {
    float v = func_80496288(lbl_eu_80663E14);
    cf::CfObjectMoveA8View* view = (cf::CfObjectMoveA8View*)this;
    // The flag word is read once for the direction tests (kept in a local)
    // while the clamp-path clears re-read the member - retail reloads
    // mFlags68 (lwz r0,0x68) at each clear, so keep the test value separate.
    // :: on the constants avoids the namespace-cf copy (mangled
    // lbl_eu_80666A88__2cf) so the SDA relocs keep the retail names.
    u32 flags = mFlags68;
    if ((flags & 0x4000) != 0) {
        // Decreasing state: step the +0xA4 rate down toward the low clamp.
        // The clamp-path clears go through a separate view type so MWCC
        // reloads mFlags68 (retail lwz r0,0x68) instead of reusing the test
        // register. :: on the constants avoids the namespace-cf copy.
        float f = view->field_A4 - lbl_eu_80666AB4 * v;
        view->field_A4 = f;
        if (f <= ::lbl_eu_80666A88) {
            // Retail order at each clamp: reload the flag word FIRST, then
            // store the clamp constant, then mask+store the flags.
            // Volatile cast: blocks MWCC from CSE-ing this load with the
            // direction-test read above (retail emits a fresh lwz here).
            u32 cur = *(volatile u32*)&mFlags68;
            view->field_A4 = ::lbl_eu_80666A88;
            ((cf::CfObjectMoveFlags68View*)this)->flags68 = cur & ~0x4000;
            if (lbl_eu_80665958 != 0) {
                func_804B4C7C(lbl_eu_80665958, _60C_region);
            }
        }
    } else if ((flags & 0x2000) != 0) {
        // Increasing state: step the +0xA4 rate up toward the high clamp.
        float f = view->field_A4 + lbl_eu_80666AB4 * v;
        view->field_A4 = f;
        if (f >= lbl_eu_80666A94) {
            u32 cur = *(volatile u32*)&mFlags68;
            view->field_A4 = lbl_eu_80666A94;
            ((cf::CfObjectMoveFlags68View*)this)->flags68 = cur & ~0x2000;
            if (lbl_eu_80665958 != 0) {
                func_804B4BDC(lbl_eu_80665958, _60C_region);
            }
        }
    }
    if (mSubObj98 != 0) {
        float spd = mMoveSpeed;
        // Constant-first form: retail emits fcmpu cr0, const, spd.
        if (::lbl_eu_80666A88 == spd) {
            spd = lbl_eu_80666A94;
        }
        float factor = lbl_eu_80666A94 / spd;
        u32 flags2 = mFlags68;
        if ((flags2 & 0x800) != 0) {
            // Move the +0xA0 position toward the high clamp.
            float f = factor * v + field_A0;
            field_A0 = f;
            if (f >= lbl_eu_80666A94) {
                u32 cur = *(volatile u32*)&mFlags68;
                field_A0 = lbl_eu_80666A94;
                mFlags68 = cur & ~0x800;
            }
        } else if ((flags2 & 0x400) != 0) {
            // Move the +0xA0 position toward the low clamp.
            float f = field_A0 - factor * v;
            field_A0 = f;
            if (f <= ::lbl_eu_80666A88) {
                u32 cur = *(volatile u32*)&mFlags68;
                field_A0 = ::lbl_eu_80666A88;
                mFlags68 = cur & ~0x400;
            }
        }
        func_800BC8D8(this);
    }
}

void CfObjectMove_nullsub_9(){}

void CfObjectMove_nullsub_10(){}

void CfObjectMove_nullsub_11(){}

void CfObjectMove_nullsub_12(){}

void CfObjectMove_nullsub_13(){}

void CfObjectMove_nullsub_14(){}

// Retail symbol is Fv but the body consumes r4 (a target pointer) -
// forced-name form carrying the hidden arg (same scheme as
// CfObject_UnkVirtualFunc65). When a 6C0 target exists and differs from the
// incoming target, release the old target (vtable +0x8, flag 1) and clear
// the slot; the redundant nested checks mirror retail's two beq targets.
// Then, when a C4 target exists, set the 6C0 slot to the incoming target,
// run the position query (vtable +0x138) and write its x component into the
// C4 target's scale fields (0x430/0x43C, then 0x434 scaled by
// lbl_eu_80666AD0).
void CfObject_UnkVirtualFunc46__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, void* arg) {
    if (self->mTarget6C0 != 0) {
        if (arg != self->mTarget6C0) {
            if (self->mTarget6C0 != 0) {
                if (self->mTarget6C0 != 0) {
                    reinterpret_cast<cf::CfObjectSub38If*>(self->mTarget6C0)->m08(1);
                }
                self->mTarget6C0 = 0;
            }
            self->mTarget6C0 = 0;
        }
    }
    if (self->mTargetC4 != 0) {
        func_8004B730(self->mTargetC4, arg);
        self->mTarget6C0 = arg;
        float x = *(float*)self->CfObject_UnkVirtualFunc58();
        cf::CfObjectMoveTargetC4* c4 = (cf::CfObjectMoveTargetC4*)self->mTargetC4;
        c4->field_430 = x;
        c4->field_43C = c4->field_438 * x;
        float y = *(float*)self->CfObject_UnkVirtualFunc58();
        cf::CfObjectMoveTargetC4* c4b = (cf::CfObjectMoveTargetC4*)self->mTargetC4;
        c4b->field_434 = lbl_eu_80666AD0 * y;
    } else {
        self->mTarget6C0 = arg;
    }
}

// Retail symbol is Fv but the body consumes r4 (the action/kind id) -
// forced-name form carrying the hidden arg (same scheme as
// CfObject_UnkVirtualFunc46/49). extern "C" so the definition symbol is the
// exact retail name. Allocates and constructs the movement-target object for
// the id, then dispatches it through the vtable +0x108 slot.

// Per-kind construct helpers: allocation + conditional constructor. Written
// as real functions so MWCC's inliner pins the allocation result in r3
// across the null-check (retail only copies it into the tracked slot at the
// helper boundary).
namespace {

inline void* newMoveEnemy(cf::CfObjectMove* self) {
    void* mem = allocate__Q23mtl10MemManagerFUlUl(0x224, func_80061FFC());
    if (mem != 0) {
        __ct__cf_CtrlEnemy(mem, self);
    }
    return mem;
}

inline void* newMoveNpc(cf::CfObjectMove* self) {
    void* mem = allocate__Q23mtl10MemManagerFUlUl(0x180, func_80061FFC());
    if (mem != 0) {
        __ct__CtrlNpc(mem, self);
    }
    return mem;
}

inline void* newMovePc(cf::CfObjectMove* self) {
    void* mem = allocate__Q23mtl10MemManagerFUlUl(0x1DC, func_80061FE8());
    if (mem != 0) {
        __ct__cf_CtrlPc(mem, self, 0);
    }
    return mem;
}

inline void* newMoveRemote(cf::CfObjectMove* self, u32 heap, int arg) {
    void* mem = allocate__Q23mtl10MemManagerFUlUl(0x264, heap);
    if (mem != 0) {
        __ct__CtrlRemote(mem, self, arg);
    }
    return mem;
}

inline void* newMovePad(cf::CfObjectMove* self, u32 heap, int arg) {
    void* mem = allocate__Q23mtl10MemManagerFUlUl(0x264, heap);
    if (mem != 0) {
        __ct__cf_CtrlPad(mem, self, arg);
    }
    return mem;
}

}  // namespace

extern "C" void CfObject_UnkVirtualFunc47__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, int arg) {
    // Two separate guards (retail emits two beq targets to the epilogue; a
    // single ||-condition adds an extra branch).
    if (self->_6CA == arg) {
        return;
    }
    if (arg == -1) {
        return;
    }
    self->_6CA = (s16)arg;
    // Per-kind helpers keep the allocation result in r3 across the
    // null-check/ctor block; obj only receives it at the helper boundary.
    void* obj = 0;
    u32 heap = func_80061FE8();
    if (arg == 8) {
        obj = newMoveEnemy(self);
    } else if (arg == 9) {
        obj = newMoveNpc(self);
    } else if (arg == 10) {
        obj = newMovePc(self);
    } else {
        float f = self->CfObject_UnkVirtualFunc31();
        if (arg >= 0 && arg <= 3) {
            obj = newMoveRemote(self, heap, arg);
        } else if (arg >= 4 && arg <= 7) {
            obj = newMovePad(self, heap, arg);
        }
        if (self->mTarget6C0 != 0) {
            *(float*)((char*)self->mTarget6C0 + 0xC) = f;
        }
    }
    reinterpret_cast<CfObjectMoveVt108*>(self)->m108(obj);
}

// Retail symbol is Fv but the body consumes r4 (an action id forwarded to
// the NPC movement target's action setter) - forced-name form carrying the
// hidden arg. When a 6C0 target exists, record the target's +0xC0 action id
// and the incoming id in the +0x6CF/+0x6CE bytes, then dispatch the id to
// func_8009377C (ids 1,2,3,5,4 map to themselves; anything else maps to 0).
void CfObject_UnkVirtualFunc49__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, int arg) {
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

void cf::CfObjectMove::CfObject_UnkVirtualFunc64(int flag) {
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
// (same scheme as CfObject_UnkVirtualFunc64, whose symbol carries the Fi).
void CfObject_UnkVirtualFunc65__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, int flag) {
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

void cf::CfObjectMove::updatePos() {
    if ((unk64 & 8) != 0) {
        func_804B0B54(_60C_region, &mPos3C);
    }
}

// Retail symbol is Fv but the body reads r4 (a position vector forwarded to
// the base +0x9C implementation) - forced-name form, same scheme as the
// CfObjectModel sibling defined in CfObjectModel.cpp.
void CfObject_UnkVirtualFunc19__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, const ml::CVec3* vec) {
    void* target = self->mTargetC4;
    if (target != 0) {
        // One-arg call: retail leaves r4 (the vec) untouched for the base
        // call below, so the source passes only the C4 target.
        func_8004B354(target);
    }
    CfObject_UnkVirtualFunc19__Q22cf13CfObjectModelFv(self, vec);
}

// Retail symbol is Fv but the body reads r4 (a position vector forwarded to
// the base +0x9C implementation) - forced-name form, same scheme as the
// CfObject_UnkVirtualFunc19 sibling. The C4 target and +0x98 sub-object are
// processed when present before the base call.
void CfObject_UnkVirtualFunc22__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, const ml::CVec3* vec) {
    cf::CfObjectMoveTargetC4* target = (cf::CfObjectMoveTargetC4*)self->mTargetC4;
    if (target != 0) {
        func_8004B40C(target);
        if (self->mSubObj98 != 0) {
            func_804876DC(self->mSubObj98);
        }
    }
    CfObject_UnkVirtualFunc22__Q22cf13CfObjectModelFv(self, vec);
}

// Retail symbol is Fv but the body consumes r4 (a position vector) and f1
// (an amount) - forced-name form carrying the hidden args (same scheme as
// CfObject_UnkVirtualFunc19/22/26). Each unk64 bit test is normalized through
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
            // Slot order matters: retail stacks the offset temp at sp+0x14,
            // the operator+ result at sp+8 and the assigned target at
            // sp+0x20.
            ml::CVec3 arg;
            ml::CVec3 off(::lbl_eu_80666A88, lbl_eu_80666AD8,
                          ::lbl_eu_80666A88);
            ml::CVec3 sum = *vec + off;
            // Volatile member writes keep the sp+0x20 copy observable so
            // MWCC cannot merge arg with the operator+ return temp.
            ((volatile ml::CVec3*)&arg)->x = sum.x;
            ((volatile ml::CVec3*)&arg)->y = sum.y;
            ((volatile ml::CVec3*)&arg)->z = sum.z;
        } else {
            CfObject_UnkVirtualFunc25__Q22cf8CfObjectFv(self, vec);
        }
        // Both dispatch paths raise busy bit 8.
        self->mFlags68 |= 0x100;
    }
    if (self->mTargetC4 != 0) {
        // Two-arg call form: retail sets r4 = this+0x3C explicitly.
        ((bool (*)(void*, void*))func_8004B354)(self->mTargetC4, &self->mPos3C);
        if (self->mSubObj98 != 0) {
            func_804876DC(self->mSubObj98);
        }
    }
}

// Retail symbol is Fv but the body consumes r4 (a position vector) and f1
// (an amount) - forced-name form carrying the hidden args (same scheme as
// CfObject_UnkVirtualFunc19/22). extern "C" so the definition symbol is the
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
            // Retail stacks the offset temp at sp+0x14, the operator+
            // result at sp+8 and a separate sp+0x20 buffer passed to the
            // base call via addi r0/mr r4. The inline helper above forces
            // the early address materialization.
            // Retail stacks the offset temp at sp+0x14, the operator+
            // result at sp+8 and a separate sp+0x20 buffer passed to the
            // base call via addi r0/mr r4. OPEN ITEM (plateau): every
            // source shape tried - operator= merge, named-sum copy-ctor,
            // pointer local used for store+call, inline helper taking the
            // destination by reference (2 spellings) - canonicalizes to
            // identical 304B output: MWCC forwards the copy into the call
            // argument and folds &arg into the bl, landing exactly one mr
            // short of retail's 308B schedule (uncontrollable hoist class,
            // cf. code_800B06A4.cpp "byte-identical output, MWCC
            // normalizes"). Next lever: match sibling UnkVirtualFunc25 or
            // find the original expression tree boundary.
            ml::CVec3 arg;
            ml::CVec3 off(::lbl_eu_80666A88, lbl_eu_80666AD8,
                          ::lbl_eu_80666A88);
            ml::CVec3 sum = *vec + off;
            arg = sum;
            CfObject_UnkVirtualFunc25__Q22cf8CfObjectFv(self, &arg);
        } else {
            CfObject_UnkVirtualFunc25__Q22cf8CfObjectFv(self, vec);
        }
        // Both dispatch paths raise busy bit 8 (retail converges here from
        // the base-call branch target before falling into the tail).
        self->mFlags68 |= 0x100;
    }
    if (self->mTargetC4 != 0) {
        // Two-arg call form: retail sets r4 = this+0x3C (position) explicitly
        // before the call, so pass the position through the function-pointer
        // cast (the header declares the one-arg form used by Func22).
        ((bool (*)(void*, void*))func_8004B40C)(self->mTargetC4, &self->mPos3C);
        if (self->mSubObj98 != 0) {
            func_804876DC(self->mSubObj98);
        }
    }
}

u32 cf::CfObjectMove::CfObject_UnkVirtualFunc23() {
    // Both sub-objects present + busy flag bit 2 (mFlags68 & 4) -> return a
    // pointer into the C4 target; otherwise defer to the base implementation.
    if (mSubObj98 != 0 && *(const u32*)field_0x9C != 0 && (mFlags68 & 0x4) != 0) {
        cf::CfObjectMoveTargetC4* target = (cf::CfObjectMoveTargetC4*)mTargetC4;
        return (u32)&target->field_3A8;
    }
    return CfObjectModel::CfObject_UnkVirtualFunc23();
}


void cf::CfObjectMove::CfObject_UnkVirtualFunc27(void* src) {
    // Z-only movement (X within epsilon but Z outside it) marks the target
    // with the 0x20 flag so its next update re-aims instead of sliding.
    const ml::CVec3* vec = (const ml::CVec3*)src;
    if ((unk64 & 0x80) != 0) {
        if (ml::math::abs(vec->x) > lbl_eu_8066A208 ||
            ml::math::abs(vec->z) > lbl_eu_8066A208) {
            CfObjectMoveC4FlagsView* c4 = (CfObjectMoveC4FlagsView*)mTargetC4;
            if (c4 != 0) {
                c4->flags |= 0x20;
            }
        }
    }
    CfObjectMoveTargetC4* target = (cf::CfObjectMoveTargetC4*)mTargetC4;
    if (target != 0 && (mFlags68 & 0x4) != 0) {
        func_8004B4A4(target, vec->y);
    }
    cf::CfObjectModel::CfObject_UnkVirtualFunc27(src);
}

void cf::CfObjectMove::CfObject_UnkVirtualFunc29(float value) {
    CfObject_UnkVirtualFunc29__Q22cf13CfObjectModelFv(this);
    void* target = mTargetC4;
    if (target != 0 && ((mFlags68 & 0x4) != 0)) {
        func_8004B4A4(target, value);
    }
    target = mTarget6C0;
    if (target != 0) {
        *(float*)((char*)target + 0xC) = value;
    }
}

// Retail symbol is Fv but the body consumes f1 - forced-name form carrying
// the hidden float arg (same scheme as CfObject_UnkVirtualFunc29). Base
// call, then the C4 target's busy flag (bit 2) gates a func_8004B52C call
// plus a +0x400 flag set (mTargetC4 is re-read after the call), and the
// 6C0 target's +0xC word receives the value.
void CfObject_UnkVirtualFunc30__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, float value) {
    CfObject_UnkVirtualFunc29__Q22cf13CfObjectModelFv(self);
    void* target = self->mTargetC4;
    if (target != 0 && ((self->mFlags68 & 0x4) != 0)) {
        func_8004B52C(target, value);
        CfObjectMoveC4Flags* view = (CfObjectMoveC4Flags*)self->mTargetC4;
        view->flags |= 0x400;
    }
    target = self->mTarget6C0;
    if (target != 0) {
        *(float*)((char*)target + 0xC) = value;
    }
}

void cf::CfObjectMove::CfObject_UnkVirtualFunc33(float amount) {}

// Retail symbol is Fv but the body returns 1 on the null path and tail-calls
// the C4 target's vtable +0x80 with a 0 arg (the base header declares the
// override void) - forced-name form carrying the int return.
extern "C" int CfObject_UnkVirtualFunc13__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    cf::CfObjectMoveC4Vt80* target = (cf::CfObjectMoveC4Vt80*)self->mTargetC4;
    if (target != 0) {
        return target->m80(0);
    }
    return 1;
}

extern "C" void* func_800BE0B0(CfObjectMove* self) { return &self->mSubObj54; }

// Retail symbol is Fv but the body consumes f1 (store to self+0x6E4 and to
// the C4 target) - forced-name form with the hidden float arg (same scheme as
// CfObjectMove_UnkVirtualFunc4).
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
extern "C" void CfObjectMove_UnkVirtualFunc4__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, float value) {
    void* target = self->mTargetC4;
    self->mField6E8 = value;
    if (target != 0) {
        *(float*)((char*)target + 0x434) = value;
    }
}

void* cf::CfObjectMove::CObjectParam_UnkVirtualFunc2() { return cf::CfObjectModel::CObjectParam_UnkVirtualFunc2(); }

void CfObjectMove_nullsub_15(){}

void CfObjectMove_nullsub_16(){}

void CfObjectMove_nullsub_17(){}

// Open item: three null-checked stores of `value` at +0x388. MWCC reuses r3
// (this) for the THIRD target load/store (this is dead after); retail keeps r4
// for all three (3 reg_swap, 0 structural). Witness rejects the r4<->r3 ABI
// permutation. Separated locals + (void)this invariant.
void* cf::CfObjectMove::CfObject_UnkVirtualFunc14(float value) {
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

extern "C" void CfObjectMove_UnkVirtualFunc11__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u16 v) {
    void* o = *(void**)((u8*)self + 0x6c0);
    if (o) *(u16*)((u8*)o + 0xde) = v;
}

// Retail symbols are Fv but both bodies return a float (load from the C4
// target, else the shared time source) - forced-name forms carrying the
// float return (the base header declares the overrides void).
extern "C" float CfObject_UnkVirtualFunc15__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    // Getter for the value CfObject_UnkVirtualFunc14's target writes at
    // +0x388; falls back to the shared time source when no target. The
    // shared header declares func_80069EA0 void; it really returns float,
    // so call it through a cast (same as CtrlNpc.cpp).
    void* target = self->mTargetC4;
    if (target != 0) {
        return *(float*)((char*)target + 0x388);
    }
    return ((float (*)())func_80069EA0)();
}

extern "C" float CfObject_UnkVirtualFunc17__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    void* target = self->mTargetC4;
    if (target != 0) {
        return *(float*)((char*)target + 0x38C);
    }
    return ((float (*)())func_80069EA0)();
}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc7() {
    // Dispatch to the NPC movement-target handler (tail call, r3 only).
    cf::CtrlNpc* target = (cf::CtrlNpc*)mTarget6C0;
    if (target != 0) {
        func_800948F8(target);
    }
}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc8() {
    cf::CtrlNpc* target = (cf::CtrlNpc*)mTarget6C0;
    if (target != 0) {
        func_80094CE8(target);
    }
}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc12() {
    cf::CtrlNpc* target = (cf::CtrlNpc*)mTarget6C0;
    if (target != 0) {
        func_80094DF4(target);
    }
}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc13() {
    cf::CtrlNpc* target = (cf::CtrlNpc*)mTarget6C0;
    if (target != 0) {
        func_80094E44(target);
    }
}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc14() {
    // Tail-call the NPC movement-target handler (r3 only); see Func7/8/12/13.
    cf::CtrlNpc* target = (cf::CtrlNpc*)mTarget6C0;
    if (target != 0) {
        func_80094EDC(target);
    }
}

// Retail symbol is Fv and the body genuinely returns int (1, or the CtrlNpc
// action-advance query result). The header declares the vtable slot with the
// int return so the vtable entry keeps the retail name.
// const self: MWCC hoists the first member load (lwz r3,0x6c0(r3)) above the
// LR save (stw r0,0x14(r1)) -- the documented plateau closed by the const-self
// lever (CScnEffectActNw4r getters family; non-const emits stw-first).
// extern "C" + the mangled name keeps the vtable reference verbatim.
extern "C" int CfObjectMove_UnkVirtualFunc9__Q22cf12CfObjectMoveFv(const cf::CfObjectMove* self) {
    cf::CtrlNpc* target = (cf::CtrlNpc*)self->mTarget6C0;
    int result = 1;
    if (target != 0) {
        result = func_80094D1C(target);
    }
    return result;
}

void CfObjectMove_nullsub_19(){}

void CfObjectMove_nullsub_20(){}

// Retail symbol is Fv but the body consumes f1 (three null-checked stores at
// +0x38C of the C4/C8/CC targets) - forced-name form with the hidden float
// arg (same scheme as CfObjectMove_UnkVirtualFunc4).
extern "C" void* CfObject_UnkVirtualFunc16__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, float value) {
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
// arg (same scheme as CfObjectMove_UnkVirtualFunc21, which passes the
// vtable query result leftover in r3). Query the +0x38 sub-object's vtable
// slot +0xE4; on a non-zero result re-run the query (retail reloads the
// sub pointer) and forward the query result plus the incoming index.
void CfObjectMove_UnkVirtualFunc17__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u32 arg) {
    cf::CfObjectSub38VtE4* sub = (cf::CfObjectSub38VtE4*)self->mSubObj38;
    if (sub != 0) {
        if (sub->_fE4() != 0) {
            int r = ((cf::CfObjectSub38VtE4*)self->mSubObj38)->_fE4();
            func_8015C074((void*)r, arg);
        }
    }
}

void CfObjectMove_UnkVirtualFunc18__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u32 arg) {
    cf::CfObjectSub38VtE4* sub = (cf::CfObjectSub38VtE4*)self->mSubObj38;
    if (sub != 0) {
        if (sub->_fE4() != 0) {
            int r = ((cf::CfObjectSub38VtE4*)self->mSubObj38)->_fE4();
            func_8015C0B0((void*)r, arg);
        }
    }
}

// Retail symbol is Fv but the body consumes r4-r8 (five args forwarded to
// the CCharEffect slot helper) - forced-name form carrying the hidden args
// (same scheme as CfObjectMove_UnkVirtualFunc17/18). The +0x38 sub-object's
// vtable +0xE4 is queried twice; on success the query result plus the five
// args go to func_8015BFCC (the s16 arg emits the retail extsh).
void CfObjectMove_UnkVirtualFunc16__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u32 a, u32 b, u32 c, u32 d, u32 e) {
    cf::CfObjectSub38VtE4* sub = (cf::CfObjectSub38VtE4*)self->mSubObj38;
    if (sub != 0) {
        if (sub->_fE4() != 0) {
            int r = ((cf::CfObjectSub38VtE4*)self->mSubObj38)->_fE4();
            // c is widened here so the int->short conversion emits the retail
            // extsh at the call site (MWCC defers it after the plain moves).
            func_8015BFCC((void*)r, a, b, (s16)c, d, e);
        }
    }
}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc21() {
    // Query the +0x38 sub-object's vtable slot +0xE4; on a non-zero result
    // re-run the query (retail reloads the sub pointer) and flush the
    // character effect slots.
    cf::CfObjectSub38VtE4* sub = (cf::CfObjectSub38VtE4*)mSubObj38;
    if (sub != 0) {
        if (sub->_fE4() != 0) {
            ((cf::CfObjectSub38VtE4*)mSubObj38)->_fE4();
            func_8015C100();
        }
    }
}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc22() {
    // Loop the counter 0..0x2B through the vtable slot +0x208 (the int-taking
    // form of CfObjectMove_UnkVirtualFunc17; the base header declares the
    // slot Fv, so the call goes through the CfObjectMoveVt208 proxy).
    for (int i = 0; i < 0x2C; i++) {
        reinterpret_cast<CfObjectMoveVt208*>(this)->m208(i);
    }
}

// Retail symbol is Fv but the body consumes r4 (forwarded to the CCharEffect
// slot query) and returns the helper's bool result (0 on the null path) -
// forced-name form carrying the hidden arg and the return. Same sub-object
// +0xE4 double-query scheme as CfObjectModel_UnkVirtualFunc18.
bool CfObjectMove_UnkVirtualFunc19__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u32 arg) {
    cf::CfObjectSub38VtE4* sub = (cf::CfObjectSub38VtE4*)self->mSubObj38;
    if (sub != 0) {
        if (sub->_fE4() != 0) {
            int r = ((cf::CfObjectSub38VtE4*)self->mSubObj38)->_fE4();
            return func_8015C294((unsigned int*)r, arg);
        }
    }
    return 0;
}

void CfObjectMove_UnkVirtualFunc20__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u32 arg) {
    cf::CfObjectSub38VtE4* sub = (cf::CfObjectSub38VtE4*)self->mSubObj38;
    if (sub != 0) {
        if (sub->_fE4() != 0) {
            int r = ((cf::CfObjectSub38VtE4*)self->mSubObj38)->_fE4();
            func_8015BD24((void*)r, arg);
        }
    }
}

// Retail symbol is Fv but the body consumes r4 (an index forwarded to the
// CCharEffect slot helper) - forced-name form carrying the hidden arg (same
// scheme as CfObjectMove_UnkVirtualFunc17/18/20). Query the +0x38
// sub-object's vtable slot +0xE4; on a non-zero result re-run the query
// (retail reloads the sub pointer) and forward the query result plus the
// incoming arg to func_8015C214.
void CfObjectModel_UnkVirtualFunc18__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u32 arg) {
    cf::CfObjectSub38VtE4* sub = (cf::CfObjectSub38VtE4*)self->mSubObj38;
    if (sub != 0) {
        if (sub->_fE4() != 0) {
            int r = ((cf::CfObjectSub38VtE4*)self->mSubObj38)->_fE4();
            func_8015C214((void*)r, (void*)arg);
        }
    }
}

void CfObjectMove_nullsub_21(){}

// Retail symbol is Fv but the body takes a hidden arg (r4) and returns a
// pointer (the CfGameManager loader result, or 0) - forced-name form.
extern "C" void* CfObjectMove_UnkVirtualFunc23__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, void* param) {
    void* a = self->mField6DC;
    bool cond = (a != 0 && self->mField6E0 != 0);
    if (cond) {
        return func_80081900__Q22cf13CfGameManagerFv((u32)param, (u32)a, (u32)self->mField6E0);
    }
    return 0;
}

// Retail returns the sub-object voice-request result (r3) or 0; the base
// header declares void, so use the forced-name form to carry the bool
// return. Only `this` (+0x28) is forwarded to func_802A109C in retail.
extern "C" bool func_800BE898__Q22cf8CfObjectFiUlff(cf::CfObject* self, int a, u32 b, float c, float d) {
    void* sub = self->mSubObj38;
    if (sub != 0) {
        return func_802A109C((char*)sub + 0x28);
    }
    return false;
}

void CfObjectMove_nullsub_22(){}

int cf::CfObjectMove::getSubState() {
    struct SubObj { int dummy0[11]; int field_2c; int field_30; };
    SubObj* sub = (SubObj*)mSubObj38;
    if (!sub) return -1;
    if (sub->field_2c < 0) return -1;
    return sub->field_30;
}

void cf::CfObjectMove::freeSub()
{
    extern void func_802A1304(void*);
    char* sub = (char*)mSubObj38;
    if (sub != 0) {
        func_802A1304(sub + 0x28);
    }
}

void CfObjectMove_nullsub_23(){}

void cf::CfObjectMove::setSubFieldC(unsigned short val) {
    void* ptr = mSubObjB0;
    *(unsigned short*)((char*)ptr + 0xc) = val;
}

int cf::CfObjectMove::getSubFieldA() { return *(s16*)((char*)mSubObjB0 + 10); }

void cf::CfObjectMove::setSubFieldA(unsigned short val) {
    void* ptr = mSubObjB0;
    *(unsigned short*)((char*)ptr + 0xa) = val;
}

int cf::CfObjectMove::getSubFieldE() { return *(s16*)((char*)mSubObjB0 + 14); }

void cf::CfObjectMove::setSubFieldE(unsigned short val) {
    void* ptr = mSubObjB0;
    *(unsigned short*)((char*)ptr + 0xe) = val;
}

extern "C" void CfObject_UnkVirtualFunc9__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    reinterpret_cast<ObjVtIf*>(*(void**)((u8*)self + 0xb0))->vf14();
}

extern "C" void CfObject_UnkVirtualFunc10__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    reinterpret_cast<ObjVtIf*>(*(void**)((u8*)self + 0xb0))->vf18();
}

void cf::CfObjectMove::virtCall10(){
    void** inner = (void**)mPtr10;
    void (*func)(void*) = (void (*)(void*))inner[0x14 / 4];
    func(this);
}

extern "C" void CfObject_UnkVirtualFunc61__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    reinterpret_cast<ObjVtIf*>(*(void**)((u8*)self + 0xb0))->vf30();
}

extern "C" void CfObject_UnkVirtualFunc62__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    reinterpret_cast<ObjVtIf*>(*(void**)((u8*)self + 0xb0))->vf34();
}

void cf::CfObjectMove::CfObject_UnkVirtualFunc12() {
    // Double-hop through the +0xB0 sub-object's vtable slots +0x0C and
    // +0x10 (Shift-padded view: the vptr sits at object+0x10). Retail
    // reloads mSubObjB0 for each call, so keep the two calls as separate
    // member accesses rather than a cached pointer.
    reinterpret_cast<ObjVtIf*>(mSubObjB0)->_v00C();
    reinterpret_cast<ObjVtIf*>(mSubObjB0)->_v010();
}

void CfObjectMove_nullsub_24() {}

int cf::CfObjectMove::nullsub_25() { return 0; }

// Retail symbol is Fv but the body consumes r4 (a slot index) - forced-name
// form carrying the hidden arg (same scheme as CfObject_UnkVirtualFunc65).
// Releases the indexed +0x6F8 slot: stop its animation model through the
// +0x98 sub-object (vtable +0xC8), detach its character-effect slots via
// the +0x38 sub-object's +0xE4 query, release the model list, and clear the
// slot. Then clears the matching +0x6C flag bit (18 for slot 1, 17 for
// slot 0).
void CfObjectMove_UnkVirtualFunc3__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, int index) {
    if (index < 2) {
        if (self->mField6F8[index] != 0) {
            if (self->mSubObj98 != 0) {
                reinterpret_cast<cf::CfObjectMoveSub98VtC4*>(self->mSubObj98)->mC8(self->mField6F8[index]);
            }
            cf::CfObjectSub38VtE4* sub38 = (cf::CfObjectSub38VtE4*)self->mSubObj38;
            if (sub38 != 0) {
                if (sub38->_fE4() != 0) {
                    int q = ((cf::CfObjectSub38VtE4*)self->mSubObj38)->_fE4();
                    func_8015C8F4((void*)q, self->mField6F8[index]);
                }
            }
            func_80495E60(self->mField6F8[index]);
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
// CfObject_UnkVirtualFunc65.
void CfObject_UnkVirtualFunc66__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, int flag) {
    CfObject_UnkVirtualFunc66__Q22cf13CfObjectModelFv(self, flag);
    if ((self->unk64 & 8) != 0 && flag == 0 && lbl_eu_80665958 != 0) {
        // flag is provably 0 here, so the attach path never runs; it is
        // retained because retail emits both sides of the inner flag test
        // (one cmpwi in cr1 serves both tests, with attach as the
        // fall-through).
        if (flag != 0) {
            func_804B4BDC(lbl_eu_80665958, self->_60C_region);
        } else {
            func_804B4C7C(lbl_eu_80665958, self->_60C_region);
        }
    }
}

// Retail symbol is Fv but the body consumes r4 (a flag) - forced-name form
// (same scheme as CfObject_UnkVirtualFunc65). Raises bit 21 of mFlags68 (or
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
            func_804838DC(s, flag);
        }
    }
}

extern "C" void* CfObjectModel_UnkVirtualFunc6__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    reinterpret_cast<ObjVtIf*>(*(void**)((u8*)self + 0xb0))->vf48();
    return NULL;
}

int cf::CfObjectMove::nullsub_26() { return 0; }

// Retail symbol is Fv but the body consumes r4 (forwarded to the vtable
// +0x18C slot) - forced-name form carrying the hidden arg. Teardown/init
// sequence: base model funcs 2 and 1 (vtable +0x17C/+0x178), then the
// +0x1D0 slot (CfObjectMove_UnkVirtualFunc3) with flags 0 and 1, the
// +0x90/+0x94 word pair is cleared, and the arg goes to the +0x18C slot
// (CfObjectModel_UnkVirtualFunc6).
void CfObject_UnkVirtualFunc37__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u32 arg) {
    cf::CfObjectMove90View* view = reinterpret_cast<cf::CfObjectMove90View*>(self);
    self->CfObjectModel_UnkVirtualFunc2();  // vtable +0x17C
    self->CfObjectModel_UnkVirtualFunc1();  // vtable +0x178
    reinterpret_cast<cf::CfObjectMoveVt1D0*>(self)->m1D0(0);
    reinterpret_cast<cf::CfObjectMoveVt1D0*>(self)->m1D0(1);
    view->field_90 = 0;
    view->field_94 = 0;
    self->CfObjectModel_UnkVirtualFunc6((void*)arg);  // vtable +0x18C
}

// Retail symbol is Fv but the body reads r4 (forwarded to the +0xB0
// sub-object's vtable slot +0x4C) - forced-name form. Also calls the vtable
// slot +0x17C (CfObjectModel_UnkVirtualFunc2) and clears the +0x94 word.
void CfObject_UnkVirtualFunc38__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u32 arg) {
    self->CfObjectModel_UnkVirtualFunc2();
    reinterpret_cast<cf::CfObjectMove90View*>(self)->field_94 = 0;
    reinterpret_cast<cf::CfObjectMoveSubB0Vt4C*>(self->mSubObjB0)->_v04C(arg);
}

int cf::CfObjectMove::nullsub_27() { return 0; }

int cf::CfObjectMove::nullsub_28() { return 0; }

void CfObjectMove_nullsub_29(){}

void CfObjectMove_nullsub_30(){}

void CfObjectMove_nullsub_31(){}

void CfObjectMove_nullsub_32(){}

void CfObjectMove_nullsub_33(){}

void CfObjectMove_nullsub_34() {}

// Retail symbols are Fv but the bodies consume r4/r5 (a bdat id and a slot
// index) - forced-name forms carrying the hidden args (same scheme as
// CfObject_UnkVirtualFunc65). Both look up the id via func_801422A8, record
// the id and the column-0x7 result in the +0x710/+0x70C u16 arrays, then
// either clear the slot (vtable +0x1D0 with a 0/1 flag) or arm it (vtable
// +0x144 with the bdat result) plus the +0xB0 sub-object's vtable +0x3C.
// func_800BF7A0 uses slot 0 / flags 6,0; func_800BF980 slot 1 / flags 7,1.
int CfObject_UnkVirtualFunc39__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u32 arg1, u32 arg2) {
    int r = (int)cf::CfBdat::func_801422A8(arg1);
    self->field_710[0] = (u16)arg1;
    self->field_70C[0] = (u16)func_8014235C(arg1, &lbl_eu_804FC550[0x7], 0);
    if (r == 0) {
        reinterpret_cast<cf::CfObjectMoveVt1D0*>(self)->m1D0(0);
    } else {
        self->field_70C[0] = (u16)((int (*)(u32))func_80142428)(arg2);
        reinterpret_cast<cf::CfObjectMoveVt144*>(self)->m144(6, (u32)r);
        if (self->mSubObj98 != 0) {
            reinterpret_cast<cf::CfObjectMoveSubB0Vt3C*>(self->mSubObjB0)->m3C((u32)r, 0);
        }
    }
    return 1;
}

// Retail symbol is Fv but the body consumes r4/r5 (a bdat id and a slot
// index) - forced-name form carrying the hidden args (same scheme as
// CfObject_UnkVirtualFunc39/42). Unlike the 39/42 pair the slot index is
// first converted to a name (CfBdat::func_801424A8) and the arm path feeds
// that name back through func_80142428. Slot 0 / flags 6,0.
int CfObject_UnkVirtualFunc40__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u32 arg1, u32 arg2) {
    int r;
    u32 name;
    name = (u32)cf::CfBdat::func_801424A8((u16)arg2);
    r = (int)cf::CfBdat::func_801422A8(arg1);
    self->field_710[0] = (u16)arg1;
    self->field_70C[0] = (u16)func_8014235C(arg1, &lbl_eu_804FC550[0x7], 0);
    if (r == 0) {
        reinterpret_cast<cf::CfObjectMoveVt1D0*>(self)->m1D0(0);
    } else {
        self->field_70C[0] = (u16)((int (*)(u32))func_80142428)(name);
        reinterpret_cast<cf::CfObjectMoveVt144*>(self)->m144(6, (u32)r);
        if (self->mSubObj98 != 0) {
            reinterpret_cast<cf::CfObjectMoveSubB0Vt3C*>(self->mSubObjB0)->m3C((u32)r, 0);
        }
    }
    return 1;
}

int CfObject_UnkVirtualFunc42__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u32 arg1, u32 arg2) {
    int r = (int)cf::CfBdat::func_801422A8(arg1);
    self->field_710[1] = (u16)arg1;
    self->field_70C[1] = (u16)func_8014235C(arg1, &lbl_eu_804FC550[0x7], 1);
    if (r == 0) {
        reinterpret_cast<cf::CfObjectMoveVt1D0*>(self)->m1D0(1);
    } else {
        self->field_70C[1] = (u16)((int (*)(u32))func_80142428)(arg2);
        reinterpret_cast<cf::CfObjectMoveVt144*>(self)->m144(7, (u32)r);
        if (self->mSubObj98 != 0) {
            reinterpret_cast<cf::CfObjectMoveSubB0Vt3C*>(self->mSubObjB0)->m3C((u32)r, 1);
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
    name = (u32)cf::CfBdat::func_801424A8((u16)arg2);
    r = (int)cf::CfBdat::func_801422A8(arg1);
    self->field_710[1] = (u16)arg1;
    self->field_70C[1] = (u16)func_8014235C(arg1, &lbl_eu_804FC550[0x7], 1);
    if (r == 0) {
        reinterpret_cast<cf::CfObjectMoveVt1D0*>(self)->m1D0(1);
    } else {
        self->field_70C[1] = (u16)((int (*)(u32))func_80142428)(name);
        reinterpret_cast<cf::CfObjectMoveVt144*>(self)->m144(7, (u32)r);
        if (self->mSubObj98 != 0) {
            reinterpret_cast<cf::CfObjectMoveSubB0Vt3C*>(self->mSubObjB0)->m3C((u32)r, 1);
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
                reinterpret_cast<cf::CfObjectMoveSub98VtC4*>(self->mSubObj98)->mC8(self->mField6F8[index]);
            }
            reinterpret_cast<cf::CfObjectMoveSub98VtC4*>(self->mSubObj98)->mC4(self->mField6F8[index], value, 0);
        }
    }
    self->field_70C[index] = (u16)result;
}

void CfObjectMove_nullsub_35(){}

void CfObjectMove_nullsub_36(){}

void CfObjectMove_nullsub_37() {}

void CfObjectMove_nullsub_38() {}

void CfObjectMove_nullsub_39(){}

void CfObjectMove_nullsub_40(){}

void CfObjectMove_nullsub_41() {}

void CfObjectMove_nullsub_42(){}

void func_eu_800BFC78() {}

extern "C" void func_800BEE1C(void* self) {
    reinterpret_cast<ObjVtIf*>(*(void**)((u8*)self + 0xb0))->vf40();
}
extern "C" void func_800BF29C(void* self) {
    reinterpret_cast<ObjVtIf*>(*(void**)((u8*)self + 0xb0))->vf54();
}
extern "C" void func_800BF2B0(void* self) {
    reinterpret_cast<ObjVtIf*>(*(void**)((u8*)self + 0xb0))->vf58();
}
extern "C" void func_800BF2CC(void* self) {
    reinterpret_cast<ObjVtIf*>(*(void**)((u8*)self + 0xb0))->vf60();
}
extern "C" void func_800BF2E0(void* self) {
    reinterpret_cast<ObjVtIf*>(*(void**)((u8*)self + 0xb0))->vf28();
}
extern "C" void func_800BF2F8(void* self) {
    reinterpret_cast<ObjVtIf*>(*(void**)((u8*)self + 0xb0))->vf2C();
}
extern "C" void func_eu_800BFC7C(void* self) {
    reinterpret_cast<ObjVtIf*>(*(void**)((u8*)self + 0xb0))->vf68();
}
extern "C" void func_800BC4B8(void* self, float v) {
    extern float lbl_eu_80666A90;
    void* o = *(void**)((u8*)self + 0xb0);
    *(float*)((u8*)o + 4) = lbl_eu_80666A90 * v;
}


int cf::CfObjectMove::isActive() { return 1; }

void cf::CfObjectMove::setBit6c9(unsigned long bit) {
    unsigned char val = mFlags6C9;
    mFlags6C9 = __rlwimi(val, bit, 0, 31, 31);
}

// Retail symbol is Fv but the body consumes f1 (forwarded to the base
// implementation, then subtracted from the shared constant) - forced-name
// form carrying the hidden float arg (same scheme as
// CfObject_UnkVirtualFunc29/30). Base call first, then the usual +0x38
// sub-object +0xE4 double-query; on success call the camera-time helper
// with (query result, lbl_eu_80666A94 - value).
void CfObject_UnkVirtualFunc70__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, float value) {
    CfObject_UnkVirtualFunc70__Q22cf13CfObjectModelFv(self, value);
    cf::CfObjectSub38VtE4* sub = (cf::CfObjectSub38VtE4*)self->mSubObj38;
    if (sub != 0) {
        if (sub->_fE4() != 0) {
            int r = ((cf::CfObjectSub38VtE4*)self->mSubObj38)->_fE4();
            func_eu_8015D258((void*)r, lbl_eu_80666A94 - value);
        }
    }
}

cf::CfObjectMove* cf::CfObjectMove::testFlag8() {
    if (this != NULL && (unk64 & 8) != 0) {
        return this;
    }
    return NULL;
}

} // namespace cf

extern "C" void func_800BE9AC(void* self) { reinterpret_cast<OMIf*>(self)->vf0014(); }

// Retail symbol is Fv but the body tail-calls vtable+0xC4 with a scaled
// float argument - forced-name form (same as the CfObjectModel sibling).
extern "C" void CfObject_UnkVirtualFunc32__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, float f) {
    self->CfObject_UnkVirtualFunc29(f * lbl_eu_8066A210);
}

// Retail symbol is Fv but the body tail-calls vtable+0xC8 with a scaled
// float argument (the +0xC8 slot consumes a float in retail despite the
// base header's no-arg decl) - forced-name form.
extern "C" void CfObject_UnkVirtualFunc33__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, float f) {
    reinterpret_cast<cf::CfObjectMoveVtC8*>(self)->mC8(f * lbl_eu_8066A210);
}

extern "C" void func_800BC3B0(cf::CfObjectMove* self, float value) {
    // Busy flag (bit 28 of mFlags68) gates the speed set; otherwise clear
    // bit 0x800 and set bits 0x100000+0x400.
    if ((self->mFlags68 & 0x10000000) != 0) {
        return;
    }
    self->mMoveSpeed = value;
    self->mFlags68 = (*(volatile u32*)&self->mFlags68 & ~0x800u) | 0x100400u;
}
extern "C" void func_800BC3F0(cf::CfObjectMove* self) {
    // Set bits 20+13 of mFlags68 (clearing bit 14 first), then notify
    // the +0x98 sub-object and attach the position region.
    u32 flags = self->mFlags68;
    self->mFlags68 = (flags & ~0x4000u) | 0x00102000u;
    if (self->mSubObj98 != 0) {
        func_804838DC(self->mSubObj98, 1);
    }
    if (lbl_eu_80665958 != 0) {
        func_804B4BDC(lbl_eu_80665958, self->_60C_region);
    }
}
void func_800BC458(cf::CfObjectMove* self) {
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
        func_804B4C7C(lbl_eu_80665958, self->_60C_region);
    }
}
extern "C" int func_800BC4CC(cf::CfObjectMove* self) {
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
    res = func_80496264((void*)CfRes_getD80Flag(), -1);
    if (res != 0) {
        cf::CfResScene10C* scene = (cf::CfResScene10C*)res;
        u32 flags = self->unk64;
        if (((u32)__cntlzw((u32)__cntlzw(flags & 0x4) >> 5) >> 5) != 0) {
            cf::CfObjectMove6B4View* v = (cf::CfObjectMove6B4View*)self;
            u32 w = v->field_6B4;
            v->field_6B4 = w | 0x1000;
            result = func_804B192C(self->_60C_region, &scene->x, 0, 0, lbl_eu_80666A94);
            v->field_6B4 &= ~0x1000;
        } else {
            if (((u32)__cntlzw((u32)__cntlzw(flags & 0x100) >> 5) >> 5) != 0) {
                const nw4r::math::VEC3* pos =
                    (const nw4r::math::VEC3*)self->CfObject_UnkVirtualFunc23();
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
                    void* mgr = func_800821F8__Q22cf13CfGameManagerFv();
                    if (mgr == 0 || (void*)self != ((cf::CfDynMgrVt60*)mgr)->m60()) {
                        dist += lbl_eu_80666AA0;
                    }
                } else if ((void*)self != getPlayer__Q22cf13CfGameManagerFi(0)) {
                    dist += lbl_eu_80666AA0;
                }
                result = func_804B192C(self->_60C_region, &scene->x, 1, 0, dist);
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
void func_800BC68C(cf::CfObjectMove* self, u32 arg) {
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
        // Short-circuit OR of the five movement-request bits; the result
        // selects whether func_800BC68C decays or raises the move speed.
        if ((flags & 0x2) != 0 || (flags & 0x4) != 0 || (flags & 0x8) != 0 ||
            (flags & 0x80000000u) != 0 || (flags & 0x100) != 0) {
            int r = self->_6F4 != 0 || func_800BC4CC(self) != 0;
            func_800BC68C(self, r);
        }
    }
    cf::CfObjectMoveA8View* view = (cf::CfObjectMoveA8View*)self;
    // v = lbl_eu_80666A94 - AC * (A8 * ((lbl_eu_80666A94 - A0) * A4)).
    // Residual: retail emits the inner fmuls with the difference as frA
    // (fmuls f3,f4,f3); this MWCC version canonicalizes every tried source
    // shape (~16 variants incl. named temps, sign-flipped difference,
    // reordered leaves) to the load-first form (fmuls f3,f3,f4).
    float v = lbl_eu_80666A94 - view->field_AC * (view->field_A8 *
              ((lbl_eu_80666A94 - self->field_A0) * view->field_A4));
    if (v > lbl_eu_80666A88) {
        if ((self->mFlags68 & 0x100000) != 0) {
            func_800BB618(self, 1);
        }
    }
    self->mField718 = v;
    reinterpret_cast<cf::CfObjectMoveSub98Vt48*>(self->mSubObj98)->m48(v);
    // Retail re-loads mSubObj38 from memory for the second (argument) call.
    if (((cf::CfObjectSub38VtE4*)self->mSubObj38) != 0 &&
        ((cf::CfObjectSub38VtE4*)self->mSubObj38)->_fE4() != 0) {
        int r = ((cf::CfObjectSub38VtE4*)self->mSubObj38)->_fE4();
        func_eu_8015D258((void*)r, lbl_eu_80666A94 - v);
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
                if (self->CObjectState_UnkVirtualFunc8(0x1000) == 0) {
                    func_800BC9EC(self);
                    ((bool (*)(void*, void*))func_8004B40C)(self->mTargetC4, &self->mPos3C);
                    goto done_flags;
                }
            }
        }
        if (((cf::CfObjectMoveVt160*)self)->m160() != 0) {
            func_8004CF00(self->mTargetC4);
            // Copy the C4 position into the move position (integer copy) and
            // store the difference against the snapshot into the +0x54 area
            // (PS sub via the nw4r inline helper, then the float->int bridge).
            cf::CfObjectMoveTargetC4* tgt = (cf::CfObjectMoveTargetC4*)self->mTargetC4;
            *reinterpret_cast<ml::CVec3*>(&self->mPos3C) =
                *reinterpret_cast<const ml::CVec3*>(&tgt->field_3A8);
            // Struct-returning operator- keeps MWCC's return-slot + copy
            // sequence (retail routes the delta through a second stack copy).
            ml::CVec3 delta = *reinterpret_cast<ml::CVec3*>(&self->mPos3C) - targetPos;
            *reinterpret_cast<ml::CVec3*>(&((cf::CfObjectMove54View*)self)->field_54) = delta;
            self->mField4C = tgt->field_444;
        } else {
            func_800BC9EC(self);
            ((bool (*)(void*, void*))func_8004B40C)(self->mTargetC4, &self->mPos3C);
        }
done_flags:
        // Toggle the C4 target's busy bit 6 from the mFlags68 sign bit /
        // bit 24 states (the sign-bit path also clears the sign bit).
        u32 f = self->mFlags68;
        if ((f & 0x80000000) != 0) {
            ((CfObjectMoveC4Flags*)self->mTargetC4)->flags |= 0x40;
            self->mFlags68 = self->mFlags68 & 0x7FFFFFFFu;
        } else {
            CfObjectMoveC4Flags* c4f = (CfObjectMoveC4Flags*)self->mTargetC4;
            if ((f & 0x01000000) != 0) {
                c4f->flags |= 0x40;
            } else {
                c4f->flags &= ~0x40u;
            }
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
            func_804B0B54(self->_60C_region, &self->mPos3C);
        }
    }
}

// Store the position difference into the +0x54 area. The by-value parameter
// makes MWCC materialize the subtraction result and copy it word-wise to the
// destination (retail double-copy shape).
static inline void cfomPutPosDiff(cf::CfObjectMove* self, ml::CVec3 d) {
    ((cf::CfObjectMove54View*)self)->field_54 = *(u32*)&d.x;
    ((cf::CfObjectMove54View*)self)->field_58 = *(u32*)&d.y;
    ((cf::CfObjectMove54View*)self)->field_5C = *(u32*)&d.z;
}

extern "C" void func_800BC9EC(cf::CfObjectMove* self) {
    if (self->mTarget6C0 == 0) {
        return;
    }
    // Snapshot the position query result and the movement target's state.
    const ml::CVec3* pos = (const ml::CVec3*)self->CfObject_UnkVirtualFunc23();
    ml::CVec3 origPos;
    origPos.x = pos->x;
    origPos.y = pos->y;
    origPos.z = pos->z;
    cf::CfObjectMove6C0View* tgt = (cf::CfObjectMove6C0View*)self->mTarget6C0;
    tgt->field_14 = lbl_eu_80666A88;
    ((cf::CfObjectSub38If*)tgt)->_f0C();
    f32 rate = tgt->field_14;
    f32 angle = tgt->field_C;
    if (getPlayer__Q22cf13CfGameManagerFi(0) == (void*)self && self->CfObject_UnkVirtualFunc9() == 0) {
        rate = lbl_eu_80666A88;
        func_804B0B54(self->_60C_region, &self->mPos3C);
    }
    int force = 0;
    void* enemy = func_800AD860((void*)self);
    // Nested else-if keeps retail's branch shape (no force retest).
    if (enemy != 0) {
        if (((cf::CfObjectMoveAD86View*)enemy)->field_45CA & 1) {
            force = 1;
        }
    } else if (((u32)__cntlzw((u32)__cntlzw(self->unk64 & 0x8) >> 5) >> 5) != 0 && self->field_6CE == 3) {
        force = 1;
    }
    if (ml::math::abs(rate) > lbl_eu_8066A208 || force != 0) {
        // Circular sweep: scale the position X by the rate, sweep the angle
        // through Sin/Cos, then step the +0x60C region toward the swept point.
        // Retail recomputes ABC*angle for the Cos call instead of CSE-ing it.
        float sx = rate * *(const float*)self->CfObject_UnkVirtualFunc58();
        rate = lbl_eu_80666AB8 * sx;
        ml::CVec3 step;
        step.x = rate * SinFIdx__Q24nw4r4mathFf(lbl_eu_80666ABC * angle);
        step.z = rate * CosFIdx__Q24nw4r4mathFf(lbl_eu_80666ABC * angle);
        step.y = lbl_eu_80666A88;
        const ml::CVec3* base = (const ml::CVec3*)self->CfObject_UnkVirtualFunc23();
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
    cfomPutPosDiff(self, *reinterpret_cast<const ml::CVec3*>(&self->mPos3C) - origPos);
}

extern "C" void func_800BCFA0(cf::CfObjectMove* self) {
    if (self->mSubObj98 == 0) {
        return;
    }
    int wasC4Null = self->mTargetC4 == 0;
    func_804B0A74(self->_60C_region);
    if (((cf::CfObjectMove94View*)self)->field_9C != 0 && self->mTargetC4 == 0) {
        // Construct the +0xD0 CActParamAnimGame sub-object as the C4 target.
        self->mTargetC4 = self->_D0;
        ((cf::CfObjectSub38VtE4*)self->mTargetC4)->_fE0();
        ((cf::CfObjectMoveD0View*)self->mTargetC4)->field_4E8 = self;
        u32 flags = self->unk64;
        if ((flags & 0x2) != 0) {
            void* c4 = self->mTargetC4;
            if (getPlayer__Q22cf13CfGameManagerFi(0) == (void*)self) {
                func_80051B84(c4);
                ((cf::CfObjectMoveD0View*)self->mTargetC4)->field_530 |= 0x400;
            } else {
                func_80051BA0(c4);
            }
            ((cf::CfObjectMoveD0View*)self->mTargetC4)->field_4EC |= 0x4000;
            void* c4b = self->mTargetC4;
            ((cf::CfObjectMoveD0View*)c4b)->field_4EC =
                (((cf::CfObjectMoveD0View*)c4b)->field_4EC & 0xFFF5FFFF) | 0x00040000;
        } else if ((flags & 0x4) != 0) {
            func_80051BDC(self->mTargetC4);
        } else if ((flags & 0x8) != 0 || (flags & 0x1) != 0) {
            func_80051BC4(self->mTargetC4);
        } else if ((flags & 0x100) != 0) {
            func_80051B84(self->mTargetC4);
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
            void* anim = ((cf::CfObjectMoveVt184*)self)->m184();
            func_8004B624(self->mTargetC4, sub98, field6D4, (u32)anim);
            func_8004B6A4(self->mTargetC4, (void*)((cf::CfObjectMove94View*)self)->field_9C, (void*)((cf::CfObjectMove94View*)self)->field_94);
        } else {
            func_8004B624(self->mTargetC4, self->mSubObj98, (void*)((cf::CfObjectMove94View*)self)->field_9C, ((cf::CfObjectMove94View*)self)->field_94);
        }
    }
    u32 f68 = self->mFlags68;
    if ((f68 & 0x10) != 0) {
        self->mFlags68 &= ~0x111;
        ((cf::CfObjectMoveVtA8*)self)->mA8((const ml::CVec3*)&self->mPos3C);
    } else if ((f68 & 0x00800000) != 0) {
        u32 flags2 = self->unk64;
        self->mFlags68 &= ~0x00800000u;
        f32 rate2 = (flags2 & 0x2) != 0 ? lbl_eu_80666AC8 : lbl_eu_80666ACC;
        self->CfObject_UnkVirtualFunc26((u32)(&self->mPos3C), rate2);
    } else {
        ((cf::CfObjectMoveVtA8*)self)->mA8((const ml::CVec3*)&self->mPos3C);
    }
    ((cf::CfObjectMoveVtC8*)self)->mC8(self->mField4C);
    self->CfObject_UnkVirtualFunc27(self->_pad48);
    if (self->mTargetC4 != 0) {
        if (wasC4Null) {
            if (self->mTarget6C0 != 0) {
                func_8004B730(self->mTargetC4, self->mTarget6C0);
            }
            float x = *(float*)self->CfObject_UnkVirtualFunc58();
            cf::CfObjectMoveTargetC4* c4 = (cf::CfObjectMoveTargetC4*)self->mTargetC4;
            c4->field_430 = x;
            c4->field_43C = c4->field_438 * x;
            float y = *(float*)self->CfObject_UnkVirtualFunc58();
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
        self->CfObject_UnkVirtualFunc64(1);
    }
    if ((self->mFlags68 & 0x2000000) != 0) {
        ((cf::CfObjectMoveVt154*)self)->m154(1);
    }
    func_80484E5C(self->mSubObj98, lbl_eu_80666A94);
    self->CfObject_UnkVirtualFunc70(self->field_A0);
    ((cf::CfObjectMoveVt1A4*)self)->m1A4(lbl_eu_80666A94);
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
                ((cf::CfObjectMoveSub98Vt50*)self->mSubObj98)->m50(1);
                cf::CfObjectModelSub98* sub = self->mSubObj98;
                if (sub != 0) {
                    void* p = ((cf::CfObjectMoveSub98View7EC*)sub)->field_7EC;
                    if (p != 0) {
                        ((cf::CfObjectMove7ECView*)p)->field_2A = (u8)self->field_6CC;
                    }
                }
            } else {
                ((cf::CfObjectMoveSub98Vt50*)self->mSubObj98)->m50(0);
            }
        }
    }
    if ((self->mFlags68 & 0x20000) == 0) {
        if (self->mSubObj98 != 0) {
            if (((cf::CfObjectMoveSub98Vt54*)self->mSubObj98)->m54() != 0) {
                self->mFlags68 |= 0x20000;
                if (self->mSubObj98 != 0) {
                    ((cf::CfObjectMoveSub98Vt50*)self->mSubObj98)->m50(1);
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
            ((cf::CfObjectMoveVt1C*)p)->m1C(1, lbl_eu_8066AF20);
        }
    }
    if (((self->mFlags6C9 >> 3) & 0xF) != 5) {
        ((cf::CfObjectModelSub98Vt*)self->mSubObj98)->m64();
    }
    self->CfObject_UnkVirtualFunc66((self->mFlags68 >> 11) & 1);
    if (func_800BAD98(self) != 0 || func_800BADF8(self) != 0) {
        ((cf::CfObjectMoveSub98Vt9C*)self->mSubObj98)->m9C(0, 0);
    } else if (func_800BADC8(self) != 0) {
        ((cf::CfObjectMoveSub98Vt9C*)self->mSubObj98)->m9C(1, 0);
    } else if (func_800BAE28(self) != 0) {
        ((cf::CfObjectMoveSub98Vt9C*)self->mSubObj98)->m9C(2, 0);
        const u8* s = (const u8*)getBdatStringColumnValue(lbl_eu_806640A8, lbl_eu_804FC550, lbl_eu_80664184);
        u8 b = *s;
        if ((b & 0x2) != 0) {
            ((cf::CfObjectMoveSub98View7A0*)self->mSubObj98)->field_7A0 &= ~0x10000000;
        }
        if ((b & 0x8) != 0) {
            func_804C0254(&((cf::CfObjectMoveSub98View31C*)self->mSubObj98)->field_31C, 1);
        }
    }
    if (self->mSubObj38 != 0) {
        ((cf::CfObjectSub38If*)self->mSubObj38)->_f74();
    }
}
// retail: lhz r0,0x1678(r3); extrwi r3,r0,1,28 = ((u16 at +0x1678)>>3)&1
extern "C" u32 func_800BD638(void* self) { return (*(const u16*)((const char*)self + 0x1678) >> 3) & 1; }
// Walk the two +0x6F8 animation slots; when the matching +0xC8/+0xCC
// target is missing, allocate and construct it (field_378 = slot index,
// +0x4/+0x34 = the +0x38 sub-object, +0x4F4 = self), then advance the
// animation model (func_8005A594), attach the model list via the vtable
// +0x188 query and func_8004B624, and reset the C4 target's pages via
// func_8004C5EC + the 5-arg func_8004B9D4 form. Retail re-reads the
// member target for the object field writes (no register reuse), so the
// writes go through the member re-derivation.
void func_800BD644(cf::CfObjectMove* self) {
    // Walk the two +0x6F8 animation slots; both per-slot reads (the slot
    // array and the +0xC8/+0xCC target pair) index the same i so MWCC
    // strength-reduces them onto one stepped cursor.
    void* slot;
    char* cur = (char*)self;
    int i = 0;
    while (i < 2) {
        slot = *(void**)(cur + 0x6F8);
        if (slot != 0 && self->mField6D8 != 0) {
            if (*(void**)(cur + 0xC8) == 0) {
                void* obj = allocate__Q23mtl10MemManagerFUlUl(0x53C, func_80061FE8());
                if (obj != 0) {
                    // Ctor returns the object in r3 (MWCC ctor ABI), so the
                    // store below can reuse r3 directly - no callee-saved
                    // register is needed to carry obj across the call.
                    obj = __ct__8005A3FC(obj, self);
                }
                // Retail stores the (possibly null) result to the slot
                // BEFORE the field writes, and writes field_378 straight
                // from the alloc result; the remaining fields re-read the
                // member (no register reuse).
                *(void**)(cur + 0xC8) = obj;
                ((cf::CfObjectMoveC8View*)obj)->field_378 = i;
                void* ta = *(void**)(cur + 0xC8);
                void* sub38 = self->mSubObj38;
                ((cf::CfObjectMoveC8View*)ta)->field_4 = sub38;
                ((cf::CfObjectMoveC8View*)ta)->field_34 = sub38;
                ta = *(void**)(cur + 0xC8);
                ((cf::CfObjectMoveC8View*)ta)->field_4F4 = self;
            }
            if (*(void**)(cur + 0xC8) != 0) {
                func_8005A594(*(void**)(cur + 0xC8));
                u32 anim = (u32)reinterpret_cast<cf::CfObjectMoveVt188*>(self)->m188();
                func_8004B624(*(void**)(cur + 0xC8), slot, self->mField6D8, anim);
                if (self->mTargetC4 != 0) {
                    // Retail: bl func_8004C5EC; mr r4,r3; lwz r3,<target>;
                    // li r5/r6/r7; bl func_8004B9D4.
                    void* page = (void*)func_8004C5EC(self->mTargetC4);
                    ((void (*)(void*, void*, int, int, int))func_8004B9D4)(
                        *(void**)(cur + 0xC8), page, 0, -1, 0);
                }
            }
        }
        i++;
        cur += 4;
    }
    if (self->mSubObj98 != 0) {
        func_80482918(self->mSubObj98, 1);
    }
}

extern "C" void func_800BE0F8(cf::CfObjectMove* self, u32 value) {
    // Insert (value << 1) into bits 27-30 of mFlags6C9, then forward the
    // inserted nibble to the +0x98 sub-object's vtable +0x64 (tail call).
    cf::CfObjectModelSub98* target = self->mSubObj98;
    u32 flags = __rlwimi(self->mFlags6C9, value, 1, 27, 30);
    self->mFlags6C9 = (u8)flags;
    if (target != 0) {
        // Retail: rlwinm r4,r0,31,28,31 = (flags >> 1) & 0xF -- the flag-word
        // nibble at bits 1-4 (untouched by the 27-30 insertion), NOT the
        // inserted field nibble ((flags >> 27) & 0xF is a different value).
        ((cf::CfObjectMoveSub98Vt64*)target)->m64((flags >> 1) & 0xF);
    }
}
void func_800BE12C(u8* obj, int a, int b, int c, int d) {
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
extern "C" u16 func_800BE93C(void* self) { return *(u16*)((u8*)*(void**)((u8*)self + 0xB0) + 0xC); }
extern "C" void func_800BE1A4(cf::CfObjectMove* self) {
    // Store 1 at +0x6C4, -1 at +0x6C8, and clear bits 24-26 of mFlags6C9.
    // Bitfield clear: MWCC emits the retail rlwinm r0,r0,0,27,23 directly
    // (a plain AND on the byte field folds the mask to 27,31).
    self->mField6C4 = 1;
    self->mField6C8 = -1;
    ((cf::CfFlags6C9*)&self->mFlags6C9)->top3 = 0;
}
void func_800BE28C(cf::CfObjectMove* self) {
    cf::CtrlNpc* target = (cf::CtrlNpc*)self->mTarget6C0;
    if (target != 0) {
        func_80094FC8(target);
    }
}
// Toggles bit 17 of mFlags68 from the incoming flag, then when a +0x98
// sub-object exists forwards the flag to its vtable slot +0x50. On the
// set path, the sub-object is re-read (retail reloads +0x98 after the call)
// and its +0x7EC pointer's +0x2A byte receives the low byte of the +0x6CC
// u16 field.
void func_800BE33C(cf::CfObjectMove* self, int flag) {
    if (flag != 0) {
        self->mFlags68 |= 0x00020000;
    } else {
        self->mFlags68 &= ~0x00020000;
    }
    if (self->mSubObj98 != 0) {
        if (flag != 0) {
            reinterpret_cast<cf::CfObjectMoveSub98Vt50*>(self->mSubObj98)->m50(1);
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
            reinterpret_cast<cf::CfObjectMoveSub98Vt50*>(self->mSubObj98)->m50(0);
        }
    }
}
// Retail symbol is Fv but the body consumes r4 (a flag): with a flag set the
// +0x6C bit 16 is raised and the +0xB0 sub-object's vtable +0x38 is invoked;
// with it clear the model list (0x6D4) is released through the C4 target and
// the list destructor, then bit 16 is lowered.
void func_800BE3E8(cf::CfObjectMove* self, int flag) {
    if (flag != 0) {
        ((cf::CfObjectMoveFlags6C*)self)->field_6C |= 0x10000;
        reinterpret_cast<ObjVtIf*>(self->mSubObjB0)->_v038();
    } else {
        if (self->mField6D4 != 0) {
            if (self->mTargetC4 != 0) {
                func_8004B6BC(self->mTargetC4, self->mField6D4);
            }
            func_80495E60(self->mField6D4);
            self->mField6D4 = 0;
        }
        ((cf::CfObjectMoveFlags6C*)self)->field_6C &= ~0x10000;
    }
}
// Forward a position region (self+0x60C) to the region-library manager
// (lbl_eu_80665958); flag selects the attach (func_804B4BDC) vs detach
// (func_804B4C7C) helper.
extern "C" void func_800BE824(cf::CfObjectMove* self, int flag) {
    if (lbl_eu_80665958 != 0) {
        if (flag != 0) {
            func_804B4BDC(lbl_eu_80665958, self->_60C_region);
        } else {
            func_804B4C7C(lbl_eu_80665958, self->_60C_region);
        }
    }
}
int func_800BE8B4(cf::CfObjectMove* self, int value) {
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
extern "C" void func_800BEA34() {}
extern "C" bool func_800BEA38() { return false; }
// Look up a bdat column (offset 0x7 into the column-name table) for the
// given id/flag; tail-calls the shared bdat reader.
extern "C" u32 func_800BED6C(u32 id, u32 flag) {
    return func_8014235C(id, &lbl_eu_804FC550[0x7], flag);
}
// Column-name lookup through the bdat reader: index self->field_710[flag],
// resolve column offset 0x7 of the name table, then map the u16 result to a
// name string via CfBdat::func_801424A8.
const char* func_800BED80(cf::CfObjectMove* self, u32 flag) {
    u32 result = func_8014235C(self->field_710[flag], &lbl_eu_804FC550[0x7], flag);
    return cf::CfBdat::func_801424A8((u16)result);
}

// Same as func_800BED80 but column offset 0x11.
const char* func_800BEDC4(cf::CfObjectMove* self, u32 flag) {
    u32 result = func_8014235C(self->field_710[flag], &lbl_eu_804FC550[0x11], flag);
    return cf::CfBdat::func_801424A8((u16)result);
}
// Same as func_800BED6C but column offset 0x11.
extern "C" u32 func_800BEE08(u32 id, u32 flag) {
    return func_8014235C(id, &lbl_eu_804FC550[0x11], flag);
}
extern "C" void func_800BEE30() {}
extern "C" void func_800BF2F4() {}
