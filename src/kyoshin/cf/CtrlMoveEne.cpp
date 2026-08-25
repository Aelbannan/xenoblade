// Auto-scaffolded catalog TU for kyoshin/cf/CtrlMoveEne
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CBattleManagerApi.hpp"
#include <stddef.h>
#include "monolib/scn/CScnTimeApi.hpp"

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CtrlMoveEne.hpp"
#include "kyoshin/cf/CtrlNpc.hpp"
#include "monolib/math/Random.hpp"

// In-TU forward declarations (definitions below).
void func_8008D444(cf::CfObjectMove* self, cf::CFunc8008D444Obj* obj, int flag);

// Enemy move-controller constructor (retail __ct__8008A104): runs the base
// ctor, installs the +0x48 vtable and the null move hook, zeroes the flags /
// halfword / vector fields, seeds field_0x18A from mtRand(60), installs the
// +0x12C dispatch hook, forwards the move sub-object's +0xCC result to
// field_0x4C and raises the +0x800 flag.
cf::CfObjectMove* __ct__8008A104(cf::CfObjectMove* self) {
    __ct__80088904(self);
    self->field_0x48 = (void*)lbl_eu_805279FC;
    self->mMoveHook = __ptmf_null;
    self->field_0x17C = 0;
    self->field_0x180 = 0;
    self->field_0x184 = 0;
    self->field_0x186 = 0;
    self->field_0x190 = 0;
    self->field_0x192 = 0;
    self->field_0x168 = lbl_eu_806665C0;
    self->field_0x178 = lbl_eu_806665C0;
    self->field_0x196 = 0;
    self->field_0x198 = 0;
    self->mVec138 = ml::CVec3::zero;
    self->mVec144 = ml::CVec3::zero;
    self->field_0x160 = lbl_eu_806665C4;
    self->field_0x164 = lbl_eu_806665C0;
    self->field_0x19C = lbl_eu_806665C0;
    self->field_0x18A = (u16)ml::math::mtRand(0x3c);
    func_80089684(self);
    self->mMoveHook = lbl_eu_805278E4;
    self->field_0x4C = self->field_0x34->field_0x28->_vCC();
    self->field_0x180 |= 0x800;
    self->field_0x38 = lbl_eu_806665C0;
    self->field_0x170 = lbl_eu_806665C8;
    self->field_0x174 = lbl_eu_806665C8;
    return self;
}

// Dispatches the +0x12C move hook when the movement sub-object's +0x8C slot
// reports a nonzero rate, the +0x12C hook is set, and func_8008D51C accepts.
void func_8008A23C(cf::CfObjectMove* self) {
    if (self->field_0x34->field_0x28->_v8C() != lbl_eu_806665C0) {
        func_800895A8(self);
        func_8008C4F0(self);
        if (__ptmf_test(&self->mMoveHook) != 0) {
            if (func_8008D51C(self) != 0) {
                (self->*self->mMoveHook)();
            }
        }
    }
}

// The +0x3E9C embedded sub-object of the enemy battle object, as
// func_8008A2C8 calls it (vtable slots 0x0C/0x4C/0x50/0x8C/0xAC/0x110/0x150).
static cf::CFunc8008A2C8Sub* A2C8Sub(cf::CFunc8008B580Obj* obj) {
    return (cf::CFunc8008A2C8Sub*)&obj->mSub2;
}
// func_8008A2C8's word copy of the base position (+0x0C) into mVec144
// (plus the move-sub height into +0x148, matching the retail store order).
static void A2C8CopyPos(cf::CFunc8008A2C8View* view,
                        cf::CFunc8008B580Obj* obj) {
    view->mVec144W.x = *(u32*)&view->mPos0.x;
    view->mVec144W.y = *(u32*)&view->mPos0.y;
    view->mVec144W.z = *(u32*)&view->mPos0.z;
    *(u32*)&view->mVec144W.y =
        *(u32*)&A2C8Sub(obj)->vtable->fn_0xAC(A2C8Sub(obj))->y;
}

// Word view of the base position (retail copies the three words with
// lwz/stw pairs).
static cf::CFunc8008E760Vec3W& A2C8PosW(cf::CFunc8008A2C8View* view) {
    return *(cf::CFunc8008E760Vec3W*)&view->mPos0;
}

// Enemy move-controller per-frame update (retail func_8008A2C8): the big
// battle-AI driver. It refreshes the controller state, orients the velocity
// toward the move-sub heading, runs the battle-object busy/HP gate, dispatches
// func_8008BEEC for the actor-word checks, runs the approach/ground-probe
// chain (actor lookup via func_800B708C__Fi / func_8016FE34, the collision
// height-band scan, the +0x192 timer), then the heading / +0x178 / +0x168
// pursuit gates, the player proximity scan and the mVec144 commit, the
// +0x8000 / +0x4 / +0x5 flag paths and finally the +0x31 battle event.
void func_8008A2C8(cf::CfObjectMove* self) {
    cf::CFunc8008A2C8View* view = (cf::CFunc8008A2C8View*)self;
    func_800895A8(self);
    view->field_0x17C &= 0x7F613E7F;
    if ((view->field_0x17C & 0x1000u) == 0) {
        if ((view->field_0x17C & 0x08000000u) == 0) {
            // Face the move-sub heading.
            cf::CFunc8008FE8CSub* sub =
                (cf::CFunc8008FE8CSub*)self->field_0x34->field_0x28;
            f32 h = sub->vtable->fn_0xCC(sub);
            view->mVelocity.z =
                CosFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * h);
            view->mVelocity.x =
                SinFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * h);
            view->mVelocity.y = lbl_eu_806665C0;
        }
        s16 t = (s16)(view->field_0x198 + 1);
        view->field_0x198 = t;
        view->field_0x17C |= 0x08000000;
        if (t > 0x12C) {
            view->field_0x198 = 0x12C;
        }
        cf::CFunc8008B580Obj* obj = (cf::CFunc8008B580Obj*)(
            self->field_0x34->field_0x28
                ? (u8*)self->field_0x34->field_0x28 - 0x3E9C
                : 0);
        cf::CFunc8008A2C8F60* f60 =
            (cf::CFunc8008A2C8F60*)obj->field_3F60;
        if (obj->_v2BC() != 0 && obj->_v128() <= lbl_eu_806665C0) {
            if (f60 != 0 && (self->field_0x180 & 0x8u) != 0) {
                view->field_0x17C &= ~0x8000u;
                func_8004B8B0(f60, 0, 1, lbl_eu_806665C0);
                A2C8Sub(obj)->vtable->fn_0x150(A2C8Sub(obj), 0);
            }
            return;
        }
        int r28 = 0;
        if (f60 != 0) {
            int r31 = 1;
            u32 w = obj->field_04->_v30()->field_0;
            if (func_80174C98(obj, &w, 0xA) == 0) {
                u32 w2 = obj->field_04->_v30()->field_0;
                if (func_80174C98(obj, &w2, 0x9) == 0) {
                    r31 = 0;
                }
            }
            func_8008BEEC(self, obj, 1, r31);
            u32 f180 = self->field_0x180;
            if ((f180 & 0x1C000000u) != 0) {
                if ((f180 & 0x8u) != 0) {
                    view->field_0x17C &= ~0x8000u;
                    u32 w3 = obj->field_04->_v30()->field_0;
                    if (func_80174C98(obj, &w3, 0x1C) != 0) {
                        func_8004B8B0(f60, 0, 1, lbl_eu_806665C0);
                        A2C8Sub(obj)->vtable->fn_0x150(A2C8Sub(obj), 0);
                    } else if ((view->field_0x17C & 0x800000u) != 0) {
                        func_8004B8B0(f60, 0, 1, lbl_eu_806665C0);
                        A2C8Sub(obj)->vtable->fn_0x150(A2C8Sub(obj), 0);
                    } else {
                        // Approach: resolve the action source and probe.
                        A2C8Sub(obj)->vtable->fn_0x150(A2C8Sub(obj), 1);
                        void* src = (void*)func_800B708C__Fi(
                            A2C8Sub(obj)->vtable->fn_0x4C(A2C8Sub(obj),
                                                          1));
                        cf::CFunc8008B580Obj* r26 =
                            (cf::CFunc8008B580Obj*)func_8016FE34(src);
                        if (r26 != 0) {
                            ml::CVec3* p =
                                A2C8Sub(r26)->vtable->fn_0xAC(A2C8Sub(r26));
                            f32 f31 = lbl_eu_806665D0 + p->y;
                            u16 d74 =
                                ((cf::CFunc8008E760Data*)self->field_0x34)
                                    ->field_74;
                            int r27 = 0;
                            if ((d74 & 1u) != 0) {
                                cf::CFunc8008A2C8F60* f3 =
                                    (cf::CFunc8008A2C8F60*)
                                        r26->field_3F60;
                                if (f3 != 0) {
                                    if ((f3->field_4EC & 0x2u) != 0) {
                                        f31 = lbl_eu_806665D4 +
                                              A2C8Sub(r26)
                                                  ->vtable
                                                  ->fn_0xAC(A2C8Sub(r26))
                                                  ->y;
                                    } else if ((f3->field_C & 0x2u) == 0) {
                                        r27 = 1;
                                    }
                                }
                                view->field_0x164 = f31;
                                view->field_0x192 = 0x3C;
                            } else {
                                // Collision height-band scan.
                                ml::CVec3* pa =
                                    A2C8Sub(r26)->vtable->fn_0xAC(
                                        A2C8Sub(r26));
                                ml::CVec3* pb =
                                    A2C8Sub(obj)->vtable->fn_0xAC(
                                        A2C8Sub(obj));
                                f32 f30 = pb->y - lbl_eu_806665E0;
                                f32 f28 = lbl_eu_806665D4 + pb->y;
                                s16 t192 = (s16)(view->field_0x192 - 1);
                                view->field_0x192 = t192;
                                if (t192 > 0) {
                                    f31 = view->field_0x164;
                                } else {
                                    ml::CVec3 vec(lbl_eu_806665C0,
                                                 lbl_eu_806665D4,
                                                 lbl_eu_806665C0);
                                    ml::CVec3 probe =
                                        *obj->mSub2.vtable
                                              ->fn_0xAC(&obj->mSub2) +
                                        vec;
                                    if (func_804BE398(
                                            &probe, 0x4A11, 0, 1,
                                            lbl_eu_806665DC,
                                            lbl_eu_806665DC) != 0) {
                                        int r24 = 0;
                                        for (int i = 0;
                                             i < func_804BE4AC(); i++) {
                                            if (((ml::CVec3*)func_804BE520(
                                                     i))
                                                    ->y <
                                                lbl_eu_806665C0) {
                                                continue;
                                            }
                                            ml::CVec3* c =
                                                (ml::CVec3*)func_804BE50C(i);
                                            f32 py = obj->mSub2.vtable
                                                         ->fn_0xAC(
                                                             &obj->mSub2)
                                                         ->y;
                                            if (c->y > py &&
                                                c->y < f28) {
                                                f28 = c->y;
                                            }
                                        }
                                        for (int i = 0;
                                             i < func_804BE4AC(); i++) {
                                            if (((ml::CVec3*)func_804BE520(
                                                     i))
                                                    ->y <=
                                                lbl_eu_806665C0) {
                                                continue;
                                            }
                                            f32 cy =
                                                ((ml::CVec3*)func_804BE50C(i))
                                                    ->y;
                                            if (cy > f30 && cy < f28) {
                                                r24 = 1;
                                                f30 = cy;
                                            }
                                        }
                                        if (r24 != 0) {
                                            view->field_0x164 =
                                                lbl_eu_806665E4 + f30;
                                        } else {
                                            view->field_0x164 = f31;
                                        }
                                    } else {
                                        view->field_0x164 = f31;
                                    }
                                    view->field_0x192 = 0x3C;
                                }
                            }
                            if (r31 != 0) r27 = 1;
                            if (r27 != 0) {
                                f31 = obj->mSub2.vtable
                                          ->fn_0xAC(&obj->mSub2)
                                          ->y;
                            }
                            func_8004B8B0(f60, 1, 0, f31);
                        }
                        func_800899AC(self, lbl_eu_806665E0);
                    }
                }
            }
        a2c8_b2c4:
            u32 f180b = self->field_0x180;
            if ((f180b & 0x08000000u) != 0) {
                f60->field_C |= 0x2000;
            } else if ((f180b & 0x04000000u) != 0) {
                f60->field_C |= 0x4000;
            }
            if ((self->field_0x180 & 0x40u) != 0) {
                f60->field_4EC |= 4;
            }
            // Heading / pursuit gates.
            f32 f29 = obj->mSub2.vtable->fn_0x8C(&obj->mSub2);
            f32 f31 = func_80496288(lbl_eu_80663E14) * f29;
            int r27 = 0;
            if (r31 != 0) {
                obj->field_4550 |= 0x200;
                void* sub2 = (void*)obj->_v2A4();
                if (sub2 != 0) {
                    u32* w50 = *(u32**)((u8*)sub2 + 0x50);
                    if (w50 != 0) {
                        u16 w = *(u16*)((u8*)w50 + 0x3C);
                        r27 = (w == 1 || w == 2) ? 1 : 0;
                    }
                }
            } else {
                obj->field_4550 &= ~0x200u;
            }
            if ((view->field_0x17C & 0x02000000u) != 0) {
                f32 f1;
                if ((f60->field_4EC & 1u) != 0) {
                    view->field_0x19C = lbl_eu_806665C0;
                    f1 = lbl_eu_806665C8;
                } else {
                    f1 = view->field_0x19C + f31;
                    view->field_0x19C = f1;
                    if (f1 >= lbl_eu_806665E8) {
                        if (f1 >= lbl_eu_806665EC) {
                            view->field_0x17C &= 0xFC7FFFFF;
                            view->field_0x19C = lbl_eu_806665C0;
                        }
                        f1 = view->field_0x170;
                    } else {
                        f1 = lbl_eu_806665C8;
                    }
                }
                view->field_0x174 =
                    lbl_eu_806665F0 * (f1 - view->field_0x174) +
                    view->field_0x174;
            } else {
                view->field_0x174 =
                    lbl_eu_806665F0 *
                        (view->field_0x170 - view->field_0x174) +
                    view->field_0x174;
                if (r31 != 0) {
                    view->field_0x19C = lbl_eu_806665C0;
                    ml::CVec3* pp =
                        obj->mSub2.vtable->fn_0xAC(&obj->mSub2);
                    view->mVec138W.x = *(u32*)&pp->x;
                    view->mVec138W.y = *(u32*)&pp->y;
                    view->mVec138W.z = *(u32*)&pp->z;
                } else if ((f60->field_4EC & 1u) != 0) {
                    ml::CVec3* pp =
                        obj->mSub2.vtable->fn_0xAC(&obj->mSub2);
                    ml::CVec3* me =
                        self->field_0x34->field_0x28
                            ? (ml::CVec3*)0
                            : (ml::CVec3*)0;
                    (void)me;
                    f32 dx = *(f32*)&view->mVec138W.x - pp->x;
                    f32 dz = *(f32*)&view->mVec138W.z - pp->z;
                    f32 d2 = dx * dx + dz * dz;
                    if (d2 <= lbl_eu_806665F4) {
                        f32 f1 = view->field_0x19C + f31;
                        view->field_0x19C = f1;
                        if (f1 > lbl_eu_806665F8) {
                            view->field_0x19C = lbl_eu_806665F8;
                            view->field_0x17C |= 0x02000000;
                        }
                    } else {
                        view->mVec138W.x = *(u32*)&pp->x;
                        view->mVec138W.y = *(u32*)&pp->y;
                        view->mVec138W.z = *(u32*)&pp->z;
                        view->field_0x19C = lbl_eu_806665C0;
                    }
                } else {
                    f32 f1 = view->field_0x19C - f31;
                    view->field_0x19C = f1;
                    if (f1 < lbl_eu_806665C0) {
                        view->field_0x19C = lbl_eu_806665C0;
                    }
                }
            }
            view->field_0x3C = lbl_eu_806665F0;
            if ((self->field_0x180 & 0x2u) != 0) {
                view->field_0x17C &= 0xF9FFFFFF;
                return;
            }
            getInstance__Q22cf13CfGameManagerFv();
            if (func_8006EF04(0x400) != 0) return;
            cf::CFunc8008A2C8BMan* bm = (cf::CFunc8008A2C8BMan*)
                getInstance__Q22cf14CBattleManagerFv();
            u8 b1aa = bm->field_1AA;
            if (b1aa >= 1 && b1aa <= 0x18) return;
            // Battle-state / damage gate.
            f32 f29b = obj->mSub2.vtable->fn_0x8C(&obj->mSub2);
            f32 f28 = func_80496288(lbl_eu_80663E14) * f29b;
            if (f28 == lbl_eu_806665C0) {
                f28 = lbl_eu_806665E4;
            }
            int r0 = 0;
            u32 f4ec = f60->field_4EC;
            if ((f4ec & 0x100u) != 0 && (f4ec & 0x40000u) != 0) {
                f32 hp = obj->_v128();
                obj->_v11C(-hp);
                r0 = 1;
            } else if ((f4ec & 0x100000u) != 0) {
                view->field_0x17C |= 0x40000000;
                view->field_0x178 = lbl_eu_806665C0;
                if (((cf::CFunc8008B580F60*)f60)->field_532 >= 100) {
                    f32 hp = obj->_v128();
                    obj->_v11C(-hp);
                }
                r0 = 1;
            } else {
                r0 = (f4ec & 0x40000u) != 0 ? 1 : 0;
            }
            if (r0 != 0) return;
            int r26 = 1;
            if ((view->field_0x17C & 0x04000000u) != 0) {
                view->field_0x17C &= 0xFC7FFFFF;
                r26 = 0;
                r28 = 1;
            } else if ((f60->field_4EC & 0x100u) == 0 ||
                       (obj->field_3374 & 0x100000u) != 0) {
                if ((view->field_0x17C & 0x4u) != 0) {
                    void* r23 = func_800B89CC(obj->field_45C0);
                    if (r23 != 0 && func_80198310() != 0) {
                        cf::CFunc8008B580Obj* ro =
                            (cf::CFunc8008B580Obj*)func_80198310();
                        if (ro->field_3F60 != 0) {
                            void* tgt = (void*)ro->mSub2.vtable
                                            ->fn_0x110(&ro->mSub2, 0);
                            if (tgt != 0 &&
                                (*(u32*)((u8*)tgt + 0x200) &
                                 0x08000000u) != 0) {
                                view->field_0x196 = 0;
                                int r0b = 0;
                                if ((self->field_0x180 & 0x40u) != 0) {
                                    if (f60->field_4F8 > lbl_eu_806665C0) {
                                        r0b = 1;
                                    } else {
                                        view->mVec144W =
                                            A2C8PosW(view);
                                        *(u32*)&view->mVec144W.y =
                                            *(u32*)&obj->mSub2.vtable
                                                    ->fn_0xAC(&obj->mSub2)
                                                    ->y;
                                        r0b = 0;
                                    }
                                } else if ((self->field_0x180 & 0x8u) !=
                                           0) {
                                    r0b = 1;
                                } else if ((f60->field_4EC & 0x2u) == 0) {
                                    r0b = 1;
                                } else {
                                    view->mVec144W =
                                        A2C8PosW(view);
                                    r0b = 0;
                                }
                                if (r0b != 0) return;
                                r26 = 0;
                            } else {
                                s16 t = (s16)(view->field_0x196 + 1);
                                view->field_0x196 = t;
                                if (t >= 0x1E) r26 = 0;
                            }
                        }
                    }
                }
                if ((view->field_0x17C & 0x40000000u) != 0) {
                    f32 f1 = view->field_0x178 + f28;
                    view->field_0x178 = f1;
                    if (f1 >= lbl_eu_806665E8) {
                        if (f1 <= lbl_eu_806665FC) {
                            view->field_0x178 = lbl_eu_806665C0;
                            view->field_0x17C &= 0xBFFFFFFF;
                            return;
                        }
                    } else {
                        return;
                    }
                }
                // Player proximity scan.
                void* pl = getPlayer__Q22cf13CfGameManagerFi(0);
                if (pl != 0) {
                    u32* c4 = *(u32**)((u8*)pl + 0xC4);
                    if (c4 != 0 &&
                        (*(u16*)((u8*)c4 + 0x530) & 1u) != 0) {
                        r26 = 0;
                    }
                }
                if ((self->field_0x180 & 0x40u) != 0) {
                    if (r27 != 0) {
                        view->field_0x168 = lbl_eu_806665C0;
                    } else {
                        cf::CFunc8008B580Obj* obj2 =
                            (cf::CFunc8008B580Obj*)(
                                self->field_0x34->field_0x28
                                    ? (u8*)self->field_0x34->field_0x28 -
                                          0x3E9C
                                    : 0);
                        if (func_80148778((u8*)obj2 + 8, 6) != 0 ||
                            func_80148778((u8*)obj2 + 8, 7) != 0 ||
                            func_80148778((u8*)obj2 + 8, 0xC) != 0) {
                            r27 = 1;
                        }
                        if (r27 == 0) {
                            f32 f1 = view->field_0x168 + f28;
                            view->field_0x168 = f1;
                            if (f1 >= lbl_eu_80666600) {
                                view->mVec144W = A2C8PosW(view);
                                *(u32*)&view->mVec144W.y =
                                    *(u32*)&obj->mSub2.vtable
                                            ->fn_0xAC(&obj->mSub2)
                                            ->y;
                                r26 = 0;
                            }
                        }
                    }
                } else {
                    if (r27 == 0 &&
                        (((cf::CFunc8008E760Data*)self->field_0x34)
                             ->field_74 &
                         0x20u) != 0) {
                        f32 f1 = view->field_0x168 + f28;
                        view->field_0x168 = f1;
                        if (f1 >= lbl_eu_806665FC) r26 = 0;
                    } else {
                        view->field_0x168 = lbl_eu_806665C0;
                    }
                }
            }
            if (((cf::CFunc8008A2C8Global*)getUnk80664658())
                    ->field_214 &
                0x10000u) {
                r26 = 0;
            }
            if (r26 != 0) {
                // Commit the +0x144 target / proximity decision.
                ml::CVec3* pos =
                    obj->mSub2.vtable->fn_0xAC(&obj->mSub2);
                f32 dx = *(f32*)&view->mVec144W.x - pos->x;
                f32 dz = *(f32*)&view->mVec144W.z - pos->z;
                f32 d2 = dx * dx + dz * dz;
                f32 th;
                int sel;
                if ((view->field_0x17C & 0x800u) != 0) {
                    if ((view->field_0x17C & 0x10u) != 0) {
                        th = _lbl_eu_80666604;
                        sel = 1;
                    } else {
                        th = _lbl_eu_80666608;
                        sel = 0;
                    }
                } else {
                    if ((view->field_0x17C & 0x10u) != 0) {
                        th = lbl_eu_8066660C;
                        sel = 1;
                    } else {
                        sel = 0;
                        th = view->field_0x16C;
                    }
                }
                if ((self->field_0x180 & 0x4u) != 0 || d2 >= th) {
                    r26 = 0;
                    if (sel != 1) {
                        if ((self->field_0x180 & 0x100u) != 0) {
                            sel = 2;
                        }
                        if ((self->field_0x180 & 0x400u) != 0) {
                            f28 = view->field_0x16C;
                        } else {
                            f28 = cf::lbl_eu_804FB9C8[sel];
                        }
                        f32 f29z = lbl_eu_806665C0;
                        for (int i = 0; i < 3; i++) {
                            void* p =
                                getPlayer__Q22cf13CfGameManagerFi(i);
                            if (p == 0) continue;
                            cf::CFunc8008B580Obj* pobj =
                                (cf::CFunc8008B580Obj*)((u8*)p - 0x3E9C);
                            if (pobj->_v128() <= f29z) continue;
                            ml::CVec3* pa =
                                pobj->mSub2.vtable->fn_0xAC(
                                    &pobj->mSub2);
                            ml::CVec3* pb =
                                obj->mSub2.vtable->fn_0xAC(
                                    &obj->mSub2);
                            f32 pdx = pa->x - pb->x;
                            f32 pdz = pa->z - pb->z;
                            if (pdx * pdx + pdz * pdz < f28) {
                                r26 = 1;
                                break;
                            }
                        }
                        if (r26 == 0) {
                            f32 f30 = cf::lbl_eu_804FB9C8[0];
                            f32 f31b = cf::lbl_eu_804FB9C8[1];
                            int r23 = 0;
                            for (int i = 1; i < 3; i++) {
                                void* p =
                                    getPlayer__Q22cf13CfGameManagerFi(i);
                                if (p == 0) continue;
                                cf::CFunc8008B580Obj* pobj =
                                    (cf::CFunc8008B580Obj*)((u8*)p -
                                                            0x3E9C);
                                if (pobj->_v128() <= lbl_eu_806665C0) {
                                    continue;
                                }
                                ml::CVec3* pa =
                                    pobj->mSub2.vtable->fn_0xAC(
                                        &pobj->mSub2);
                                ml::CVec3* pb =
                                    obj->mSub2.vtable->fn_0xAC(
                                        &obj->mSub2);
                                f32 pdx = pa->x - pb->x;
                                f32 pdz = pa->z - pb->z;
                                f32 pd2 = pdx * pdx + pdz * pdz;
                                if (pd2 < f30) {
                                    r26 = 1;
                                    r23 = 0;
                                    break;
                                }
                                if (pd2 < f31b) r26 = 1;
                            }
                            if (r23 != 0) {
                                view->field_0x190 =
                                    view->field_0x190 + 1;
                                if (view->field_0x190 >= 0x1C2) {
                                    r26 = 0;
                                }
                            } else {
                                view->field_0x190 = 0;
                            }
                        }
                    }
                }
            }
            if (r26 != 0) {
                int r0b = 0;
                if ((self->field_0x180 & 0x40u) != 0) {
                    if (f60->field_4F8 > lbl_eu_806665C0) {
                        r0b = 1;
                    } else {
                        view->mVec144W = A2C8PosW(view);
                        *(u32*)&view->mVec144W.y =
                            *(u32*)&obj->mSub2.vtable
                                    ->fn_0xAC(&obj->mSub2)
                                    ->y;
                        r0b = 0;
                    }
                } else if ((self->field_0x180 & 0x8u) != 0) {
                    r0b = 1;
                } else if ((f60->field_4EC & 0x2u) == 0) {
                    r0b = 1;
                } else {
                    view->mVec144W = A2C8PosW(view);
                    r0b = 0;
                }
                if (r0b != 0) return;
            }
            if (r28 == 0 && (view->field_0x17C & 0x4u) != 0) {
                // Mark the party targets with the +0x08000000 flag.
                void* r23 = func_800B89CC(obj->field_45C0);
                if (r23 != 0) {
                    s16 cnt = *(s16*)((u8*)r23 + 0xA2);
                    for (int i = 0; i < cnt; i++) {
                        void* r3 = (void*)func_801984F0(r23, i);
                        if (r3 != 0) {
                            cf::CFunc8008B580Obj* ro =
                                (cf::CFunc8008B580Obj*)r3;
                            void* tgt = (void*)ro->mSub2.vtable
                                            ->fn_0x110(&ro->mSub2, 0);
                            if (tgt != 0) {
                                u32 w = *(u32*)((u8*)tgt + 0x200);
                                if ((w & 0x08000000u) != 0) {
                                    *(u32*)((u8*)tgt + 0x200) =
                                        w | 0x40000000;
                                }
                            }
                        }
                    }
                }
            }
            func_8008D444(self, (cf::CFunc8008D444Obj*)obj, 1);
            if (f60 != 0) {
                u32 w = obj->field_04->_v30()->field_0;
                if (func_80174C98(obj, &w, 0x10) != 0 ||
                    func_80174C98(obj, &w, 0xA) != 0 ||
                    func_80174C98(obj, &w, 0x9) != 0) {
                    func_800BE12C(&obj->mSub2, 0x31, 0, -1, 1);
                }
            }
            void* bm2 = getInstance__Q22cf14CBattleManagerFv();
            func_800D9CA0(bm2, obj);
            return;
        }
        return;
    }
    if ((view->field_0x17C & 0x4000u) != 0) {
        view->field_0x17C &= ~0x6000u;
        cf::CFunc8008B580Obj* obj2 = (cf::CFunc8008B580Obj*)(
            self->field_0x34->field_0x28
                ? (u8*)self->field_0x34->field_0x28 - 0x3E9C
                : 0);
        if (obj2->mSub2.vtable->fn_0x0C(&obj2->mSub2, 0x200) == 0) {
            obj2->field_3374 &= 0xFBFFFFFF;
            view->field_0x17C &= 0xEFFFFFFF;
        }
    }
}


void* CActorParam_UnkVirtualFunc132__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x3298); }

void func_8008B580(cf::CfObjectMove* self) {
    cf::CfObjectMoveView9* v = (cf::CfObjectMoveView9*)self;
    func_800895A8(self);
    u32 flags = self->field_0x17C & 0x7F613E7Fu;
    self->field_0x17C = flags;
    if ((flags & 0x1000u) != 0) return;
    if ((flags & 0x08000000u) == 0) {
        // Face the +0xCC heading: velocity = {sin, 0, cos} of the scaled index.
        f32 cz = CosFIdx__Q24nw4r4mathFf(
            lbl_eu_806665CC * self->field_0x34->field_0x28->_vCC());
        v->mVelocity.x = SinFIdx__Q24nw4r4mathFf(
            lbl_eu_806665CC * self->field_0x34->field_0x28->_vCC());
        v->mVelocity.y = lbl_eu_806665C0;
        v->mVelocity.z = cz;
    }
    s16 t = self->field_0x198;
    u32 f17c = self->field_0x17C | 0x08000000u;
    self->field_0x198 = t + 1;
    self->field_0x17C = f17c;
    if (self->field_0x198 > 300) {
        self->field_0x198 = 300;
    }
    cf::CfObj3F60View* f60;
    cf::CFunc8008B580Obj* obj =
        (cf::CFunc8008B580Obj*)self->field_0x34->field_0x28;
    if (obj != 0) {
        obj = (cf::CFunc8008B580Obj*)((u8*)obj - 0x3E9C);
    }
    f60 = obj->field_3F60;
    if (obj->_v2BC() != 0 && obj->_v128() <= lbl_eu_806665C0) {
        if (f60 != 0 && (self->field_0x180 & 0x8u) != 0) {
            self->field_0x17C &= ~0x10000u;
            func_8004B8B0(f60, 0, 1, lbl_eu_806665C0);
            ((cf::CFunc8008B580Sub150*)&obj->mSub2)->_v150(0);
            return;
        }
    }
    if (f60 == 0) return;
    func_8008BEEC(self, obj, 1, 0);
    if ((self->field_0x180 & 0x38u) != 0) {
        if ((self->field_0x180 & 0x8u) != 0) {
            // Death/forced-stop states: clear the +0x10000 move flag, then
            // check the bdat action columns 0x1C / 0x805 / 0x18 (each with a
            // fresh state word); on a hit kill the enemy via the flag object
            // and the sub-object's +0x150 slot.
            self->field_0x17C &= ~0x10000u;
            u32 w1 = obj->field_04->_v30()->field_0;
            if (func_80174C98(obj, &w1, 0x1C) != 0) {
                func_8004B8B0(f60, 0, 1, lbl_eu_806665C0);
                ((cf::CFunc8008B580Sub150*)&obj->mSub2)->_v150(0);
            } else {
                u32 w2 = obj->field_04->_v30()->field_0;
                int hit = func_80174C98(obj, &w2, 0x805);
                if (hit == 0) {
                    u32 w3 = obj->field_04->_v30()->field_0;
                    hit = func_80174C98(obj, &w3, 0x18);
                }
                if (hit != 0) {
                    func_8004B8B0(f60, 0, 1, lbl_eu_806665C0);
                    ((cf::CFunc8008B580Sub150*)&obj->mSub2)->_v150(0);
                }
            }
        }
        // Mirror the +0x10 / +0x20 flags into the battle sub-object.
        if ((self->field_0x180 & 0x10u) != 0) {
            f60->field_C |= 0x2000;
        } else if ((self->field_0x180 & 0x20u) != 0) {
            f60->field_C |= 0x4000;
        }
    }
    u32 f4ec = f60->field_4EC;
    if ((f4ec & 0x80u) != 0 && (f4ec & 0x40000u) != 0) {
        // Poison-style drain: subtract the full HP.
        obj->_v11C(-obj->_v128());
    } else if ((f4ec & 0x100000u) != 0) {
        self->field_0x178 = lbl_eu_806665C0;
        self->field_0x17C |= 0x40000000u;
        if (((cf::CFunc8008B580F60*)f60)->field_532 >= 100) {
            obj->_v11C(-obj->_v128());
        }
    }
    obj->field_4550 &= ~0x200u;
    self->field_0x3C = lbl_eu_806665F0;
}

// Trampoline: func_8008B930 forwards to the base refresh helper (retail
// references the unmangled C-ABI name, hence the extern "C" decl).
struct CCtrlMoveNpc;
extern "C" void func_8008962C(CCtrlMoveNpc* self);
void func_8008B930(CCtrlMoveNpc* self) { func_8008962C(self); }

// Flag-guarded set of the +0x8000 flag bit and the +0x15C actor id; returns
// 0 when the +0x2 bit is set (1 otherwise).
int func_8008B934(cf::CCtrlMoveEneView* self, int arg) {
    u32 flags = self->field_0x17C;
    if (flags & 0x2u) {
        self->field_0x17C = flags | 0x8000u;
        self->field_0x15C = arg;
        return 0;
    }
    return 1;
}

// Sets the +0x04000000 flag bit when the +0x08000000 bit is already set.
void func_8008B95C(cf::CCtrlMoveEneView* self) {
    if (self->field_0x17C & 0x08000000u) {
        self->field_0x17C |= 0x04000000u;
    }
}

// Clears the +0x4000 flag bit and resolves the +0x15C actor id; returns 1
// only when the id resolves to a live action source.
int func_8008B974(cf::CCtrlMoveEneView* self) {
    u32 flags = self->field_0x17C;
    if (flags & 0x4000u) {
        self->field_0x17C = flags & ~0x4000u;
        if (func_800B708C(self->field_0x15C) != 0) {
            return 1;
        }
    }
    return 0;
}

// Enemy approach probe (retail func_8008B9C0): picks a candidate point around
// `other` at a random walk distance interpolated between f2 and f1 (or along
// the move sub's +0xCC heading when arg is set), and accepts it only when the
// XZ distance to the move sub stays at/above (lbl_80666610 * f1). With the
// +0x8 flag it adopts the point and returns the normalized direction to the
// move sub; otherwise it runs a ground probe and a height-band scan to pick
// the fall height, gates on the +0x40 flag and the target's +0x508 height,
// then re-probes before committing.
// noinline keeps func_8008F9EC's call site an opaque bl (retail has one): with
// a visible body -ipa file would transform the `!= 0` test at the call site.
__declspec(noinline) int func_8008B9C0(cf::CfObjectMove* self,
                                       ml::CVec3* out,
                                       cf::CfObjectMove* other, f32 f1,
                                       f32 f2, int arg) {
    cf::CFunc8008F9ECSub* sub =
        (cf::CFunc8008F9ECSub*)self->field_0x34->field_0x28;
    // Random walk distance interpolated between f2 and f1.
    f32 dist = f2 + f1 * (lbl_eu_80661C30 * (f32)ml::math::mtRand(0xC9));
    // Heading index: the move sub's own heading, or a random half-turn.
    f32 ang;
    if (arg != 0) {
        ang = sub->_vCC();
    } else {
        ang = (f32)ml::math::mtRand(0x168) * lbl_eu_8066A210;
    }
    // Candidate point around `other` at angle ang, radius dist.
    ml::CVec3 pos;
    pos.x = dist * SinFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * ang) +
            ((ml::CVec3*)other)->x;
    pos.y = ((ml::CVec3*)other)->y;
    pos.z = dist * CosFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * ang) +
            ((ml::CVec3*)other)->z;
    // Reject points inside the keep-away radius around the move sub.
    ml::CVec3 diff;
    nw4r::math::VEC3Sub(diff, pos, *sub->_vAC());
    f32 th = lbl_eu_80666610 * f1;
    if (diff.x * diff.x + diff.z * diff.z >= th * th) {
        cf::CfObjectMoveView9* v = (cf::CfObjectMoveView9*)self;
        if ((self->field_0x180 & 0x8u) != 0) {
            // Adopt the point; return the normalized direction to the sub.
            v->mPosition = pos;
            self->field_0x17C |= 0x20;
            *out = v->mPosition - *sub->_vAC();
            if (out->x == lbl_eu_806665C0 && out->y == lbl_eu_806665C0 &&
                out->z == lbl_eu_806665C0) {
                return 1;
            }
            if (out->x * out->x + out->y * out->y + out->z * out->z ==
                lbl_eu_806665C0) {
                *out = ml::CVec3::zero;
            } else {
                PSVECNormalize(*out, *out);
            }
            return 1;
        }
        // Ground probe at the candidate point.
        pos.y += lbl_eu_806665C4;
        if (func_804BE398(&pos, 0x4a11, 0, 1, lbl_eu_80666614,
                          lbl_eu_806665DC) == 0) {
            return 0;
        }
        // Height-band scan over the collision-list objects near the candidate.
        f32 lo = ((ml::CVec3*)other)->y - lbl_eu_80666618;
        f32 hi = lbl_eu_80666618 + ((ml::CVec3*)other)->y;
        int found = 0;
        int any = 0;
        f32 best = _lbl_eu_80666604;
        int sel;
        u32 i;
        for (i = 0; i < (u32)func_804BE4AC(); i++) {
            if (((ml::CVec3*)func_804BE520(i))->y >= lbl_eu_806665C0) {
                any = 1;
                if (func_804BE5A4(0x40000, (int)i) != 0) {
                    if (hi >= ((ml::CVec3*)func_804BE50C(i))->y) {
                        sel = (int)i;
                        found = 1;
                        hi = ((ml::CVec3*)func_804BE50C(i))->y;
                    }
                } else {
                    f32 y = ((ml::CVec3*)func_804BE50C(i))->y;
                    f32 d = ml::math::abs(y - ((ml::CVec3*)other)->y);
                    if (best >= d) {
                        best = d;
                        lo = ((ml::CVec3*)func_804BE50C(i))->y;
                    }
                }
            }
        }
        if (found && hi >= lo) {
            f32 low = hi - lbl_eu_80666618;
            for (int j = 0; j < func_804BE4AC(); j++) {
                if (sel != j && hi > ((ml::CVec3*)func_804BE50C((u32)j))->y
                    && low < ((ml::CVec3*)func_804BE50C((u32)j))->y) {
                    low = ((ml::CVec3*)func_804BE50C((u32)j))->y;
                }
            }
            f32 span = hi - low;
            f32 lim = lbl_eu_80666610;
            if ((self->field_0x180 & 0x40u) != 0) {
                if (sub->field_C4 != 0) {
                    lim = lbl_eu_8066661C *
                          ((cf::CFunc8008B9C0Target*)sub->field_C4)->field_0x508;
                }
                if (span <= lim) return 0;
            } else {
                if (sub->field_C4 != 0) {
                    lim = lbl_eu_806665D0 *
                          ((cf::CFunc8008B9C0Target*)sub->field_C4)->field_0x508;
                }
                if (span > lim) return 0;
            }
        } else {
            if ((self->field_0x180 & 0x40u) != 0) return 0;
        }
        if (any) {
            pos.y = lo;
            if (func_8008A01C(self, &pos) != 0) {
                if (func_80088974(self, out, &pos, 0, 0) != 0) {
                    v->mPosition = pos;
                    self->field_0x17C |= 0x20;
                    return 1;
                }
            }
        }
    }
    return 0;
}

// noinline keeps the retail bl opaque at every call site in this TU.
// noinline keeps the retail bl opaque at every call site in this TU.
extern "C" __declspec(noinline) void func_8008BEEC(
    cf::CfObjectMove* self, void* obj, int arg2, u32 arg3) {
}

// Enemy movement-controller per-frame update (retail func_8008C4F0): recovers
// the battle object from the move data's +0x28 sub-object, stores the +0x3C
// target constant, dispatches func_8008D444 while the +0x198 timer is at/over
// 90 with the +0x08000000 flag set and the +0x1000 / +0x2 bits clear, clears
// the +0x40000000/+0x10000000/+0x08000000/+0x04000000/+0x400 flag bits,
// forwards the +0x1000 bit to func_8008BEEC and mirrors the +0x180 flag bits
// into the +0x3F60 sub-object.
// noinline keeps func_8008A23C's call site an opaque bl (retail has one).
__declspec(noinline) void func_8008C4F0(cf::CfObjectMove* self) {
    cf::CFunc8008D444Obj* obj =
        (cf::CFunc8008D444Obj*)self->field_0x34->field_0x28;
    if (obj != 0) obj = (cf::CFunc8008D444Obj*)((u8*)obj - 0x3E9C);
    self->field_0x3C = lbl_eu_806665F0;
    if ((self->field_0x17C & 0x08000000u) != 0 &&
        (self->field_0x17C & 0x1000u) == 0 &&
        self->field_0x198 >= 90 && (self->field_0x180 & 0x2u) == 0) {
        func_8008D444(self, obj, 0);
    }
    // Reload after the opaque call: retail re-reads +0x17C from memory.
    u32 newFlags = self->field_0x17C & ~0x5C000000u & ~0x400u;
    self->field_0x17C = newFlags;
    self->field_0x198 = 0;
    self->field_0x190 = 0;
    if (obj->field_3F60 != 0) {
        cf::CfObj3F60View* f60 = obj->field_3F60;
        func_8008BEEC(self, obj, 0, (newFlags >> 12) & 1u);
        if ((self->field_0x180 & 0x38u) != 0) {
            if ((self->field_0x180 & 0x8u) != 0) {
                if ((self->field_0x17C & 0x10000u) == 0) {
                    func_8004B8B0(f60, 1, 1, self->field_0x160);
                    func_800899AC(self, lbl_eu_806665E0);
                } else {
                    func_8004B8B0(f60, 0, 1, lbl_eu_806665C0);
                }
            }
            if ((self->field_0x180 & 0x10u) != 0) {
                f60->field_C |= 0x2000;
            } else if ((self->field_0x180 & 0x20u) != 0) {
                f60->field_C |= 0x4000;
            }
        }
        if ((self->field_0x180 & 0x40u) != 0) {
            f60->field_4EC |= 4;
        }
    }
}

// Enemy move-controller pursuit helper (retail func_8008C660, stub): takes
// the controller, a direction vector and a scaled move distance; returns
// nonzero when the pursuit should stop.
int func_8008C660(cf::CfObjectMove* self, ml::CVec3* vec, f32 f);

// Enemy move-controller direction/turn helper (retail func_8008CDE8): while the
// +0x40000 flag is set, re-approaches the +0x150 target point (or resets the
// +0x18A timer and clears the flag once the move sub is within reach); then
// computes the direction to the move sub into `out`. With the +0x20000 flag it
// probes the collision system at the sub position plus a heading-scaled offset
// and, on success, latches the probe point as the new target; otherwise it
// steps the timer and retries after a delay.
void func_8008CDE8(cf::CfObjectMove* self, ml::CVec3* out, f32 f1) {
    cf::CFunc8008CDE8View* v = (cf::CFunc8008CDE8View*)self;
    cf::CFunc8008F9ECSub* sub =
        (cf::CFunc8008F9ECSub*)self->field_0x34->field_0x28;
    if ((v->field_0x17C & 0x40000u) != 0) {
        f32 dx = v->mVec150.x - sub->_vAC()->x;
        f32 dz = v->mVec150.z - sub->_vAC()->z;
        if (dx * dx + dz * dz <= lbl_eu_806665E4) {
            v->field_0x18A = 0x3c;
            v->field_0x17C &= ~0x60000u;
        } else {
            if (func_80088974(self, out, &v->mVec150, 0, 0) != 0) {
                u16 t = v->field_0x18A;
                v->field_0x18A = t - 1;
                if (t == 0) {
                    v->field_0x18A = 0x3c;
                    v->field_0x17C &= ~0x60000u;
                    return;
                }
            }
            v->field_0x18A = 0x3c;
            v->field_0x17C &= ~0x60000u;
            return;
        }
    }
    if (func_80089E88(self, &v->mPosition, 1) != 0) {
        // Direction from the move sub to the controller position (fresh access
        // to the move sub, matching the retail lwz/lwz reload here).
        *out = v->mPosition -
               *((cf::CFunc8008F9ECSub*)self->field_0x34->field_0x28)->_vAC();
        if (out->x != lbl_eu_806665C0 || out->y != lbl_eu_806665C0 ||
            out->z != lbl_eu_806665C0) {
            if (out->x * out->x + out->y * out->y + out->z * out->z ==
                lbl_eu_806665C0) {
                *out = ml::CVec3::zero;
            } else {
                PSVECNormalize(*out, *out);
            }
        }
    } else {
        func_80088974(self, out, &v->mPosition, 0, 0);
    }
    if ((v->field_0x17C & 0x20000u) != 0) {
        // Probe the ground ahead: sub position plus a heading/rand-scaled
        // offset, gated on the +0x20000 flag and the +0x18A timer parity.
        ml::CVec3 tgt;
        tgt.x = sub->_vAC()->x;
        tgt.y = lbl_eu_8066661C + sub->_vAC()->y;
        tgt.z = sub->_vAC()->z;
        f32 ang = Atan2FIdx__Q24nw4r4mathFff(out->x, out->z);
        f32 f4 = lbl_eu_80666638 * ang;
        f32 f5 = (v->field_0x18A & 1) ? lbl_eu_806665E4 : lbl_eu_8066663C;
        f32 r = (f32)((v->field_0x18A >> 1) & 0x7FFF);
        f32 f29 = lbl_eu_8066A200 * ((lbl_eu_806665E4 + r) * f5) + f4;
        // Heading direction: cosine first (survives the sine call in f30),
        // then the sine; the probe offset is the scaled direction.
        f32 cos = CosFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * f29);
        f32 sin = SinFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * f29);
        ml::CVec3 dir;
        dir.x = sin;
        dir.y = lbl_eu_806665C0;
        dir.z = cos;
        ml::CVec3 scaled = dir * (lbl_eu_806665D0 + f1);
        ml::CVec3 dest = tgt + scaled;
        if (func_804B526C(lbl_eu_80665958, (u8*)sub + 0x60c, &tgt, &dest, 2, 1,
                          lbl_eu_80571810) == 0) {
            ml::CVec3 dest2 = *sub->_vAC() + scaled;
            if (func_80089E88(self, &dest2, 1) != 0) {
                v->mVec150 = *sub->_vAC() + scaled;
                v->field_0x17C = (v->field_0x17C | 0x40000) & ~0x20000u;
                v->field_0x18A = 0x96;
                return;
            }
        }
        u16 t = v->field_0x18A + 1;
        v->field_0x18A = t;
        if (t >= 3) {
            v->field_0x18A = 0x3c;
            v->field_0x17C &= ~0x60000u;
        }
    } else {
        u16 t = v->field_0x18A;
        v->field_0x18A = t - 1;
        if (t == 0) {
            // Timer expired: probe the ground at the sub position plus the
            // out direction scaled by f1; latch the +0x20000 flag on success.
            ml::CVec3 tgt;
            tgt.x = sub->_vAC()->x;
            tgt.y = lbl_eu_8066661C + sub->_vAC()->y;
            tgt.z = sub->_vAC()->z;
            ml::CVec3 scaled = *out * f1;
            ml::CVec3 dest = tgt + scaled;
            if (func_804B526C(lbl_eu_80665958, (u8*)sub + 0x60c, &tgt, &dest,
                              2, 1, lbl_eu_80571810) != 0) {
                v->field_0x18A = 0;
                v->field_0x17C |= 0x20000;
            } else {
                v->field_0x18A = 0x1e;
            }
        }
    }
}

// Enemy movement-controller setup: zeroes the +0x168/+0x178/+0x19C floats and
// the +0x184/+0x190/+0x192/+0x196 halfwords, sets +0x1000 and clears the
// +0x04604810 bits of the +0x17C flags, forwards the enable flag to the battle
// object's embedded move sub-object (or sets +0x2000), then raises the
// +0x08000000 battle flag, clears +0x200 of the +0x4550 word and refreshes the
// movement data block.
void func_8008D444(cf::CfObjectMove* self, cf::CFunc8008D444Obj* obj, int flag) {
    f32 zero = lbl_eu_806665C0;
    u32 flags = self->field_0x17C;
    self->field_0x168 = zero;
    self->field_0x17C = (flags | 0x1000) & 0xB9FFB7EF;
    self->field_0x178 = zero;
    self->field_0x19C = zero;
    self->field_0x184 = 0;
    self->field_0x190 = 0;
    self->field_0x192 = 0;
    self->field_0x196 = 0;
    func_80089990(self);
    if (flag != 0) {
        obj->mSub.sv08(0x200);
    } else {
        self->field_0x17C |= 0x2000;
    }
    obj->field_3374 |= 0x08000000;
    obj->field_4550 &= ~0x200u;
    func_800D581C(self->field_0x34);
    func_800D59FC(self->field_0x34);
}

// Zeroes the +0x1928 move-data slots of the battle object (retail emits the
// 60-store block inline at each of the four call sites in func_8008D51C).
static void D51CZeroSlots(cf::CFunc8008D51CObj* obj) {
    for (int i = 0; i < 10; i++) {
        cf::CFunc8008D51CSlot& s = obj->slots[i];
        s.w14 = 0;
        s.f0 = lbl_eu_806665C0;
        s.f4 = lbl_eu_806665C0;
        s.f8 = lbl_eu_806665C0;
        s.fC = lbl_eu_806665C0;
        s.f10 = lbl_eu_806665C0;
    }
}

// Enemy move-controller hook gate (retail func_8008D51C, called by the +0x12C
// move hook dispatch): returns nonzero while the controller should keep
// running its move. The +0x1000 flag gates the whole body; with the +0x2 bit
// set it tracks the battle state via the +0x8C slot and resets the battle
// object when the +0x168 accumulator crosses the threshold; otherwise it runs
// the close-range / approach logic against the +0x144 target point and the
// +0x3F60 flag object, then handles the +0x100 / +0x200 timer flags.
__declspec(noinline) int func_8008D51C(cf::CfObjectMove* self) {
    cf::CFunc8008D51CView* view = (cf::CFunc8008D51CView*)self;
    cf::CFunc8008D51CObj* obj = (cf::CFunc8008D51CObj*)(
        self->field_0x34->field_0x28
            ? (u8*)self->field_0x34->field_0x28 - 0x3E9C
            : 0);
    cf::CfObj3F60View* f60 = obj->field_3F60;
    int r28 = 1;
    if ((view->field_0x17C & 0x8u) != 0 && f60 != 0) {
        if (func_8004C5EC((u32)f60) == 0x66) {
            r28 = 0;
        } else {
            view->field_0x17C &= ~0x8u;
        }
    }
    if ((view->field_0x17C & 0x1000u) == 0) goto d51c_938;
    if ((view->field_0x17C & 0x20000000u) != 0) {
        // Battle-state track: refresh the +0x168 accumulator, then reset the
        // battle object once it crosses the threshold.
        ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
            lbl_eu_806665C0;
        if (f60 != 0 && (self->field_0x180 & 0x8u) != 0) {
            if (view->field_0x18E != 0) {
                func_8004B8B0(f60, 0, 1, lbl_eu_806665C0);
                func_8004B7C0(f60, &ml::CVec3::zero);
                obj->mSub.vtable->fn_0x150(&obj->mSub, 1);
                obj->mSub.vtable->fn_0x9C(&obj->mSub, &view->mVec144);
            } else {
                func_8004B8B0(f60, 1, 0, view->mVec144.y);
            }
        }
        f32 f31 = obj->mSub.vtable->fn_0x8C(&obj->mSub);
        view->field_0x168 =
            func_80496288(lbl_eu_80663E14) * f31 + view->field_0x168;
        if (view->field_0x168 >= lbl_eu_806665EC) {
            view->field_0x17C &= 0xDDFFC7FF;
            D51CZeroSlots(obj);
            obj->mSub.vtable->fn_0x50(&obj->mSub, 0);
            if (((cf::CFunc8008B580Obj*)obj)->_v2BC() == 0) {
                func_80174B4C(obj, 3);
            }
            view->field_0x168 = lbl_eu_806665C0;
            func_80089990(self);
            view->field_0x19C = lbl_eu_806665C0;
            view->field_0x184 = 0;
            func_80193710(obj);
            ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
                lbl_eu_806665C0;
        }
        r28 = 0;
        goto d51c_97c;
    }
    // Bit-2 clear: run the approach / close-range logic.
    view->field_0x168 = lbl_eu_806665C0;
    if (f60 == 0) goto d51c_780;
    view->field_0x174 = lbl_eu_806665F0 *
                            (lbl_eu_806665C8 - view->field_0x174) +
                        view->field_0x174;
    view->field_0x17C &= ~0x8000u;
    obj->field_455A = 0xF9;
    obj->field_3374 |= 0x08000000;
    ml::CVec3 z(lbl_eu_806665C0, lbl_eu_806665C0, lbl_eu_806665C0);
    u16 f184 = view->field_0x184;
    view->field_0x184 = f184 + 1;
    if (f184 < 0x12C && (f60->field_4EC & 0x40000u) != 0) {
        // Close: turn toward the +0x144 target point and engage the move.
        cf::CFunc8008D51CSub* sub2 = &obj->mSub;
        ml::CVec3 diff = view->mVec144 - *sub2->vtable->fn_0xAC(sub2);
        f32 th = (view->field_0x18E != 0) ? lbl_eu_80666640
                                          : lbl_eu_806665E4;
        if (diff.x * diff.x + diff.y * diff.y + diff.z * diff.z <= th) {
            if (view->field_0x18E != 0) {
                sub2->vtable->fn_0x9C(sub2, &view->mVec144);
            }
            ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
                lbl_eu_806665C0;
            view->field_0x17C |= 0x20000000;
            D51CZeroSlots(obj);
            sub2->vtable->fn_0x50(sub2, 0);
            if (((cf::CFunc8008B580Obj*)obj)->_v2BC() == 0) {
                func_80174B4C(obj, 3);
            }
            view->field_0x168 = lbl_eu_806665C0;
            if ((self->field_0x180 & 0x8u) != 0 &&
                view->field_0x18E != 0) {
                cf::CfObj3F60View* w = obj->field_3F60;
                if (w != 0) {
                    func_8004B8B0(w, 0, 1, lbl_eu_806665C0);
                    func_8004B7C0(w, &ml::CVec3::zero);
                }
                sub2->vtable->fn_0x150(sub2, 1);
            }
        } else {
            // Far: flee / close in with the +0x180 & 8 flag set.
            if ((self->field_0x180 & 0x8u) != 0) {
                ml::CVec3 dir = view->mVec144 -
                                *sub2->vtable->fn_0xAC(sub2);
                if (dir.x != lbl_eu_806665C0 ||
                    dir.y != lbl_eu_806665C0 ||
                    dir.z != lbl_eu_806665C0) {
                    if (dir.x * dir.x + dir.y * dir.y +
                            dir.z * dir.z == lbl_eu_806665C0) {
                        dir = ml::CVec3::zero;
                    } else {
                        PSVECNormalize(dir, dir);
                    }
                }
                func_8004B8B0(f60, 1, 1, view->mVec144.y);
                if (dir.x * dir.x + dir.y * dir.y + dir.z * dir.z >
                    lbl_eu_80666640) {
                    f32 rate = lbl_eu_80666644 /
                               *sub2->vtable->fn_0x138(sub2);
                    func_80089694(self, &dir, rate);
                } else {
                    ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
                        lbl_eu_806665C0;
                }
            } else {
                ml::CVec3 dir;
                func_80088974(self, &dir, &view->mVec144, 1, 0);
                f32 rate = lbl_eu_80666644 /
                           *sub2->vtable->fn_0x138(sub2);
                func_80089694(self, &dir, rate);
            }
        }
    } else {
        // Timer / flag gate: reset the object and hand off to the +0x150 slot.
        obj->mSub.vtable->fn_0x9C(&obj->mSub, &view->mVec144);
        ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
            lbl_eu_806665C0;
        view->field_0x17C |= 0x20000000;
        D51CZeroSlots(obj);
        obj->mSub.vtable->fn_0x50(&obj->mSub, 0);
        if (((cf::CFunc8008B580Obj*)obj)->_v2BC() == 0) {
            func_80174B4C(obj, 3);
        }
        view->field_0x168 = lbl_eu_806665C0;
        if ((self->field_0x180 & 0x8u) != 0 && view->field_0x18E != 0) {
            cf::CfObj3F60View* w = obj->field_3F60;
            if (w != 0) {
                func_8004B8B0(w, 0, 1, lbl_eu_806665C0);
                func_8004B7C0(w, &ml::CVec3::zero);
            }
            obj->mSub.vtable->fn_0x150(&obj->mSub, 1);
        }
    }
    goto d51c_918;

d51c_780:
    // No flag object: reset and hand off.
    obj->mSub.vtable->fn_0x9C(&obj->mSub, &view->mVec144);
    ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
        lbl_eu_806665C0;
    view->field_0x17C |= 0x20000000;
    D51CZeroSlots(obj);
    obj->mSub.vtable->fn_0x50(&obj->mSub, 0);
    if (((cf::CFunc8008B580Obj*)obj)->_v2BC() == 0) {
        func_80174B4C(obj, 3);
    }
    view->field_0x168 = lbl_eu_806665C0;
    if ((self->field_0x180 & 0x8u) != 0 && view->field_0x18E != 0) {
        cf::CfObj3F60View* w = obj->field_3F60;
        if (w != 0) {
            func_8004B8B0(w, 0, 1, lbl_eu_806665C0);
            func_8004B7C0(w, &ml::CVec3::zero);
        }
        obj->mSub.vtable->fn_0x150(&obj->mSub, 1);
    }
    goto d51c_918;

d51c_918:
    obj->mSub.vtable->fn_0x50(&obj->mSub, 0);
    r28 = 0;
    goto d51c_97c;

d51c_938:
    // Bit-2 / +0x1000 clear: copy the sub position to +0x144 and ease the
    // +0x174 heading toward +0x170.
    {
        ml::CVec3* pos = obj->mSub.vtable->fn_0xAC(&obj->mSub);
        *(u32*)&view->mVec144.x = *(u32*)&pos->x;
        *(u32*)&view->mVec144.y = *(u32*)&pos->y;
        *(u32*)&view->mVec144.z = *(u32*)&pos->z;
        view->field_0x174 = lbl_eu_806665F0 *
                                (view->field_0x170 - view->field_0x174) +
                            view->field_0x174;
    }
    // fallthrough to the +0x100 / +0x200 timer flags

d51c_97c:
    if ((view->field_0x17C & 0x80u) != 0) {
        if ((view->field_0x17C & 0x100u) != 0) {
            s16 t = (s16)(view->field_0x188 - 1);
            view->field_0x188 = (s16)t;
            if (t <= 0) {
                view->field_0x17C &= ~0x180u;
                if (f60 != 0) {
                    func_8004B9D4(f60, 1, 0, 2, 0);
                }
            }
        } else {
            view->field_0x17C |= 0x100;
            view->field_0x188 =
                (s16)((ml::math::mtRand(0x3C) + 0x1E) * 0x1E);
            if (f60 != 0) {
                func_8004B9D4(f60, 1, 0, 0, 0);
            }
        }
        r28 = 0;
    }
    return r28;
}


void* CfObject_UnkVirtualFunc58__Q22cf12CfObjectMoveFv(void* self) { return (void*)((u8*)self + 0x6e4); }

// Enemy movement-controller (re)initialisation (retail func_8008E06C): picks a
// state from a bdat column, installs the matching move hook, seeds the +0x4C
// word via the embedded sub-object's vtable +0xC4 slot with a rand angle, then
// stores the movement-rate divisor and a level-derived value into +0x58/+0x5C.
void func_8008E06C(cf::CfObjectMove* self) {
    cf::CFunc8008D444Obj* obj =
        (cf::CFunc8008D444Obj*)(self->field_0x34->field_0x28
                                    ? (u8*)self->field_0x34->field_0x28 - 0x3E9C
                                    : 0);
    ((cf::CfObjectMove4CView*)self)->field_4C = 0;
    union {
        u32 w;
        u8 b;
    } row;
    row.w = func_800AF7E4(obj, lbl_eu_804FB9E8);
    u8 state = row.b;
    if (state >= 0xf) {
        state = state - 0xf;
        if (state >= 2) {
            state = 0;
        }
        self->field_0x50 = state;
        func_80089684(self);
        self->mMoveHook = lbl_eu_80527830.hook118;
        self->field_0x18C = 2;
    } else {
        func_80089684(self);
        self->mMoveHook = lbl_eu_80527830.hook124;
        self->field_0x50 = state;
        if (state != 0) {
            self->field_0x18C = 2;
            f32 angle = (f32)ml::math::mtRand(0x168) * lbl_eu_8066A210;
            obj->mSub._vC4(angle);
            ((cf::CfObjectMove4CView*)self)->field_4C |= 4;
        } else {
            func_80089684(self);
            self->mMoveHook = lbl_eu_80527830.hook130;
            self->field_0x4C = self->field_0x34->field_0x28->_vCC();
            self->field_0x180 |= 0x800;
        }
    }
    self->field_0x54 = 0;
    f32 rate;
    if (*self->field_0x34->field_0x28->_v138() == lbl_eu_806665C0) {
    } else {
        cf::CNpcMoveSubView* sub = self->field_0x34->field_0x28;
        f32 denom = *sub->_v138();
        rate = *sub->_v1D8() / denom;
    }
    self->field_0x58 = rate;
    union {
        u32 w;
        u16 h;
    } row2;
    row2.w = func_800AF7E4(obj, lbl_eu_804FB9E8 + 0xA);
    u16 level = row2.h;
    if (level != 0) {
        self->field_0x5C = (f32)(u32)level;
    } else {
        self->field_0x5C = lbl_eu_80666648;
    }
}

// Enemy move-controller (re)initialisation (retail func_8008E2D4): installs
// the +0x13C dispatch hook, reads a match id and a state byte from bdat
// columns, seeds the +0x5C state / +0x60 timer, scans the move-table rows for
// the match id and fills either the +0x4C..+0x70 fields (flag row) or one of
// the +0x78 move-list entries, then installs the +0x148 hook or dispatches the
// state record and commits the entry count / scaled timer.
void func_8008E2D4(cf::CfObjectMove* self) {
    const CfMoveHookPtmfsE2D4* hooks =
        (const CfMoveHookPtmfsE2D4*)&lbl_eu_80527830;
    cf::CfObjectMoveInitView* view = (cf::CfObjectMoveInitView*)self;
    cf::CNpcMoveSubView* subView = self->field_0x34->field_0x28;
    cf::CFunc8008D444Obj* obj = (cf::CFunc8008D444Obj*)subView;
    if (subView != 0) {
        obj = (cf::CFunc8008D444Obj*)((u8*)subView - 0x3E9C);
    }
    func_80089684(self);
    self->mMoveHook = hooks->hook13C;
    const char* table = lbl_eu_804FB9E8;
    union {
        u32 w;
        u16 h;
    } match;
    match.w = func_800AF7E4(obj, table + 0x13);
    u16 matchId = match.h;
    view->field_0x58 = 0;
    view->field_0x5A = 0;
    view->field_0x70 = 0;
    view->field_0x64 = lbl_eu_806665C0;
    view->field_0x68 = lbl_eu_806665C0;
    union {
        u32 w;
        u8 b;
    } row;
    row.w = func_800AF7E4(obj, table);
    u8 state = row.b;
    if (state >= 0xF) {
        state = 0;
    }
    view->field_0x5C = state;
    view->field_0x60 = 0;
    view->field_0x72 = 0;
    // Movement-rate divisor: *_v1D8() / *_v138() when the denominator probe
    // is nonzero (both slots re-probed fresh, matching the retail reloads).
    f32 rate = lbl_eu_806665C0;
    if (*self->field_0x34->field_0x28->_v138() != lbl_eu_806665C0) {
        cf::CNpcMoveSubView* sub2 = self->field_0x34->field_0x28;
        f32 denom = *sub2->_v138();
        rate = *sub2->_v1D8() / denom;
    }
    view->field_0x60 = (s32)rate;
    int count = func_80086B44__Q22cf13CfGameManagerFv();
    int idx = 0;
    cf::CfMoveSubEntry* entry = view->field_0x78;
    for (int i = 0; i < count; i++) {
        union {
            u32 w;
            u16 h;
        } c13;
        c13.w = func_800AF82C(obj, table + 0x13, i);
        if (c13.h == matchId && idx < 8) {
            union {
                u32 w;
                u16 h;
            } c1b;
            c1b.w = func_800AF82C(obj, table + 0x1b, i);
            u16 flags = c1b.h;
            if ((flags & 0x200u) != 0) {
                // Flag row: direct target fields instead of a move-list entry.
                union {
                    u32 w;
                    s16 h;
                } c22;
                c22.w = func_800AF82C(obj, table + 0x22, i);
                view->field_0x4C = (f32)c22.h;
                union {
                    u32 w;
                    s16 h;
                } c27;
                c27.w = func_800AF82C(obj, table + 0x27, i);
                view->field_0x50 = (f32)c27.h;
                union {
                    u32 w;
                    s16 h;
                } c2c;
                c2c.w = func_800AF82C(obj, table + 0x2c, i);
                view->field_0x54 = (f32)c2c.h;
                view->field_0x17C |= 0x2;
                view->field_0x70 |= 0x8;
                if ((flags & 0x8u) != 0) {
                    view->field_0x70 |= 0x10;
                }
                if ((flags & 0x10u) != 0) {
                    view->field_0x70 |= 0x20;
                }
            } else {
                // Move-list row: fill the next +0x78 entry (0x14 bytes).
                entry->field_0xC = flags;
                union {
                    u32 w;
                    s16 h;
                } c22;
                c22.w = func_800AF82C(obj, table + 0x22, i);
                entry->field_0x0 = (f32)c22.h;
                union {
                    u32 w;
                    s16 h;
                } c27;
                c27.w = func_800AF82C(obj, table + 0x27, i);
                entry->field_0x4 = (f32)c27.h;
                union {
                    u32 w;
                    s16 h;
                } c2c;
                c2c.w = func_800AF82C(obj, table + 0x2c, i);
                entry->field_0x8 = (f32)c2c.h;
                // Column +0x31 is a byte gate: any nonzero value marks the
                // entry's flags halfword with bit 0x8000.
                union {
                    u32 w;
                    u8 b;
                } c31;
                c31.w = func_800AF82C(obj, table + 0x31, i);
                if (c31.b != 0) {
                    entry->field_0xC |= 0x8000;
                }
                union {
                    u32 w;
                    u8 b;
                } c3b;
                c3b.w = func_800AF82C(obj, table + 0x3b, i);
                entry->field_0xE = c3b.b;
                union {
                    u32 w;
                    u8 b;
                } c45;
                c45.w = func_800AF82C(obj, table + 0x45, i);
                entry->field_0xF = c45.b;
                union {
                    u32 w;
                    u8 b;
                } c4e;
                c4e.w = func_800AF82C(obj, table + 0x4e, i);
                entry->field_0x10 = (f32)c4e.b;
                idx++;
                entry++;
            }
        }
    }
    if (view->field_0x5C == 0) {
        view->field_0x5C = 1;
    }
    if (idx == 0 || view->field_0x5C == 0) {
        // No matching row: fall back to the +0x148 hook and the +0xCC heading.
        view->field_0x6C = lbl_eu_806665C0;
        func_80089684(self);
        self->mMoveHook = hooks->hook148;
        view->field_0x4C = self->field_0x34->field_0x28->_vCC();
        self->field_0x180 |= 0x800;
    } else {
        // Dispatch the state record and commit the entry count / scaled timer.
        (self->*((const CfMoveDispatchPtmf*)hooks)[view->field_0x5C])(
            &view->field_0x60, &view->field_0x6C);
        view->field_0x58 = (u16)idx;
        view->field_0x18C = 1;
        view->field_0x60 = view->field_0x78[0].field_0xE * 30;
    }
}

// Enemy move-controller (re)initialisation (retail func_8008E760): the big
// setup split between the battle-state probe (r28) and the plain path. When
// the +0x3F00 bit-26 state is clear and func_800B8AFC reports the object
// inactive, the plain path installs the +0x16C hook, seeds the +0x58/+0x5A/
// +0x5C counters, runs the actor-id lookup / party-info probe, copies the
// battle position and computes the movement-rate divisor. The active path
// installs one of the +0x154 / +0x160 hooks or dispatches func_8008E06C /
// func_8008E2D4 by the +0x58 bdat column. The shared tail then resolves the
// +0x18E mode from the parts element, computes the +0x160/+0x16C scale
// values from the bdat columns and commits the position to mVec144.
void func_8008E760(cf::CfObjectMove* self) {
    // Hook-table base (retail keeps it in a callee-saved register across the
    // whole function).
    const CfMoveHookPtmfsE760* hooks =
        (const CfMoveHookPtmfsE760*)&lbl_eu_80527830;
    cf::CFunc8008E760View* view = (cf::CFunc8008E760View*)self;
    cf::CNpcMoveSubView* sub = self->field_0x34->field_0x28;
    // Retail branches around the subtraction only (no null-materialising
    // select): obj stays null when the move-sub pointer is null.
    cf::CFunc8008E760Obj* obj = (cf::CFunc8008E760Obj*)sub;
    if (obj != 0) {
        obj = (cf::CFunc8008E760Obj*)((u8*)obj - 0x3E9C);
    }
    int active;
    if ((obj->field_3F00 & 0x04000000u) != 0) {
        active = 1;
    } else {
        active = (func_800B8AFC(obj) == 0);
    }
    view->field_0x17C = 0;
    view->field_0x18C = 0;
    if ((obj->field_3374 & 0x800u) != 0) {
        // MWCC forwards the just-stored zero into this read-modify-write
        // (retail: ori r0, <zero reg>, 0x40).
        view->field_0x17C |= 0x40;
    }
    union {
        u32 w;
        u8 b;
    } row53;
    row53.w = func_800AF7E4(obj, lbl_eu_804FB9E8 + 0x53);
    if (row53.b == 0) {
        self->field_0x180 |= 1;
    }
    if (active != 0) {
        // Battle state active: pick the hook / init path from the flags.
        if ((view->field_0x17C & 0x40u) != 0) {
            func_80089684(self);
            self->mMoveHook = hooks->hook154;
            self->field_0x4C = self->field_0x34->field_0x28->_vCC();
            self->field_0x180 = (self->field_0x180 | 0x800) & ~0x80000u;
        } else {
            union {
                u32 w;
                u8 b;
            } row58;
            row58.w = func_800AF7E4(obj, lbl_eu_804FB9E8 + 0x58);
            u8 v58 = row58.b;
            if ((v58 & 1) != 0) {
                func_8008E06C(self);
            } else if ((v58 & 4) != 0) {
                func_8008E2D4(self);
            } else {
                func_80089684(self);
                self->mMoveHook = hooks->hook160;
                self->field_0x4C = self->field_0x34->field_0x28->_vCC();
                self->field_0x180 |= 0x800;
            }
        }
    } else {
        // Inactive: seed the counters and run the party-position probe.
        func_80089684(self);
        self->mMoveHook = hooks->hook16C;
        view->field_0x58 = 0;
        view->field_0x5A = (u16)(ml::math::mtRand() & 0xF);
        view->field_0x5C = (u32)(ml::math::mtRand() & 0xFFFF);
        void* r28b = func_800B89CC(obj->field_45C0);
        if (r28b != 0) {
            cf::CFunc8008E760BattleObj* r31b =
                (cf::CFunc8008E760BattleObj*)func_80198310();
            if (r31b != 0) {
                r31b->vtable->fn_0x5B4(r31b);
                obj->mSub.vtable->fn_0xC4(&obj->mSub);
                u16 r27 = obj->field_45C4;
                if (r27 == 0) r27 = 1;
                f32 f31 = ((cf::CFunc8008E760B89*)r28b)->field_8C;
                u32 r28v = obj->field_45C6;
                if (r27 == 1) {
                    r28v = view->field_0x5C;
                } else {
                    f31 += obj->field_44D8;
                }
                ml::CVec3 tmp20 = *r31b->mSub.vtable->fn_0xAC(&r31b->mSub);
                f32 f1 = r31b->vtable->fn_0x5B4(r31b);
                f32 sum = obj->field_44D8 + r31b->field_44D8;
                cf::CFunc8008E760PartyInfo info;
                func_80198710(&info, &tmp20, f1, r27, r28v, f31, sum);
                if ((self->field_0x180 & 1) != 0) info.field_2D = 0;
                int r = func_8019876C(&info, &view->field_0C);
                if ((obj->field_3374 & 0x100u) != 0) {
                    view->field_0x58 |= 4;
                    view->field_0x10 = obj->mSub.vtable->fn_0xAC(&obj->mSub)->y;
                    obj->mSub.vtable->fn_0x9C(&obj->mSub, &view->field_0C);
                } else if (r != 0) {
                    view->field_0x58 |= 4;
                    obj->mSub.vtable->fn_0x9C(&obj->mSub, &view->field_0C);
                }
                if ((self->field_0x180 & 1) != 0) view->field_0x17C |= 0x200;
            }
        }
        // Commit the battle position and the movement-rate divisor.
        const u32* pw = (const u32*)obj->mSub.vtable->fn_0xAC(&obj->mSub);
        view->field_4C = pw[0];
        view->field_50 = pw[1];
        view->field_54 = pw[2];
        f32 f1 = lbl_eu_806665C0;
        if (*self->field_0x34->field_0x28->_v138() != lbl_eu_806665C0) {
            cf::CNpcMoveSubView* sub2 = self->field_0x34->field_0x28;
            f32 denom = *sub2->_v138();
            f1 = *sub2->_v1D8() / denom;
        }
        view->field_0x60 = f1;
        view->field_0x64 = lbl_eu_806665C0;
        view->field_0x70 = 0;
        view->field_0x72 = 0;
        view->field_0x76 = 0;
        view->field_0x17C |= 0x80000004u;
        view->field_0x18C = 3;
    }
    // Shared tail: resolve the mode byte and the scale values.
    view->field_0x18E = 0;
    union {
        u32 w;
        u8 b;
    } row61;
    row61.w = func_800AF7E4(obj, lbl_eu_804FB9E8 + 0x61);
    u8 mode = row61.b;
    if ((obj->field_3374 & 0x100u) != 0) {
        self->field_0x180 |= 0x8;
        s16 m = (s16)view->field_0x18C;
        if (m != 1 && m != 2) {
            void* p = func_801974CC((void*)func_80193670(), obj);
            if (p != 0 && (*(u8*)((u8*)p + 0x24) & 0xB0) != 0) {
                self->field_0x17C |= 0x10000;
                u8 b24 = *(u8*)((u8*)p + 0x24);
                if ((b24 & 0x10) != 0) view->field_0x18E = 1;
                else if ((b24 & 0x20) != 0) view->field_0x18E = 2;
                else view->field_0x18E = 3;
            }
        }
        union {
            u32 w;
            s16 h;
        } conv160;
        conv160.w = func_800AF7E4(obj, lbl_eu_804FB9E8 + 0x27);
        f32 f160 = (f32)conv160.h;
        view->field_0x160 = f160;
        if (mode == 1) view->field_0x160 = f160 * lbl_eu_806665F0;
        else if (mode == 2) view->field_0x160 = f160 * lbl_eu_80666658;
        view->field_0x164 = view->field_0x160;
    }
    if (mode == 5) self->field_0x180 |= 0x200;
    if ((obj->field_3374 & 0x10000u) != 0) self->field_0x180 |= 0x40;
    void* fp = getFP__FPCc(obj->field_3F14);
    u32 bval = getBdatStringColumnValue(fp, lbl_eu_804FB9E8 + 0x68,
                                        obj->field_3F28);
    union {
        u32 w;
        u8 b;
    } rowC;
    rowC.w = bval;
    u8 v = rowC.b;
    view->field_0x16C = (f32)v;
    if (mode == 3 || v == 0xFF) self->field_0x180 |= 0x2;
    else if (v == 0xFE) self->field_0x180 |= 0x4;
    else if (v == 0xFD) self->field_0x180 |= 0x80;
    else if (v == 0xFC) {
        self->field_0x180 |= 0x100;
        view->field_0x16C = lbl_eu_8066665C;
    } else if (v == 0xFB) {
        self->field_0x180 |= 0x400;
        view->field_0x16C = lbl_eu_80666660;
    }
    f32 f16C = view->field_0x16C;
    if (f16C >= lbl_eu_80666664) view->field_0x16C = f16C * f16C;
    else view->field_0x16C = _lbl_eu_80666608;
    if ((obj->field_3374 & 0x200u) != 0) self->field_0x180 |= 0x10;
    if ((obj->field_3374 & 0x400u) != 0) self->field_0x180 |= 0x20;
    const u32* pw2 = (const u32*)obj->mSub.vtable->fn_0xAC(&obj->mSub);
    view->mPos0W.x = pw2[0];
    view->mPos0W.y = pw2[1];
    view->mPos0W.z = pw2[2];
    if ((self->field_0x180 & 0x40) != 0) {
        union {
            u32 w;
            s16 h;
        } conv4;
        conv4.w = func_800AF7E4(obj, lbl_eu_804FB9E8 + 0x27);
        f32 f4 = (f32)conv4.h;
        *(f32*)&view->mPos0W.y = f4;
        if (mode == 1) *(f32*)&view->mPos0W.y = f4 * lbl_eu_806665F0;
        else if (mode == 2) *(f32*)&view->mPos0W.y = f4 * lbl_eu_80666658;
    }
    view->mVec144W = view->mPos0W;
    if (mode == 4) {
        ((cf::CFunc8008E760Data*)self->field_0x34)->field_74 |= 0x2000;
    }
}


void func_8008EF04(cf::CfObjectMove* self) {
    cf::CfObjectMoveView9* v = (cf::CfObjectMoveView9*)self;
    // Battle-start pulse: with the +0x18E mode, +0x8 flag and no +0x10000 flag,
    // dismiss the +0xC4 target icon and refresh the move sub's +0x150 slot.
    if (v->field_0x18E != 0 && (self->field_0x180 & 0x8u) != 0 &&
        (self->field_0x17C & 0x10000u) == 0) {
        void* c4 =
            ((cf::CFunc8008EF04Sub*)self->field_0x34->field_0x28)->field_C4;
        if (c4 != 0) {
            func_8004B8B0(c4, 0, 1, lbl_eu_806665C0);
            ((cf::CFunc8008EF04Sub*)self->field_0x34->field_0x28)->_v150(1);
        }
    }
    // Clear the +1 state flag when the target is gone / the +0x10000 talk
    // state bit is set in the +0x98 object; a live talk state makes the sub's
    // +0xC4 slot take over instead (returns nonzero -> skip heading logic).
    int done;
    if ((self->field_0x17C & 1u) != 0) {
        cf::CFunc8008EF04Sub* sub =
            (cf::CFunc8008EF04Sub*)self->field_0x34->field_0x28;
        cf::CFunc8008EF04Sub98* p98 = (cf::CFunc8008EF04Sub98*)sub->field_98;
        if (p98 != 0 && (p98->field_7A4 & 0x10000u) != 0) {
            if (sub->field_C4 != 0) {
                func_8004B9D4(sub->field_C4, 1, 0, -1, 0);
            }
        } else if (sub->field_C4 != 0 && sub->_v0C(4) != 0) {
            ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
                lbl_eu_806665C0;
            done = 1;
            goto check;
        }
        self->field_0x17C &= ~1u;
        done = 0;
    } else {
        done = 0;
    }
check:
    if (done == 0) {
        // Heading logic: compare the sub's heading with the +0x4C target
        // heading through the cos of their difference.
        f32 cosA = CosFIdx__Q24nw4r4mathFf(
            lbl_eu_806665CC *
                self->field_0x34->field_0x28->_vCC());
        f32 cosB = CosFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * self->field_0x4C);
        f32 sinA = SinFIdx__Q24nw4r4mathFf(
            lbl_eu_806665CC *
                self->field_0x34->field_0x28->_vCC());
        f32 sinB = SinFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * self->field_0x4C);
        if (cosB * cosA + sinB * sinA <= lbl_eu_806665DC) {
            // Facing the target: turn toward +0x4C and engage the +0x98 state
            // unless its +0x8000 bit is set.
            cf::CNpcBaseDataView* data = self->field_0x34;
            f32 h4c = self->field_0x4C;
            cf::CFunc8008EF04Sub* sub =
                (cf::CFunc8008EF04Sub*)data->field_0x28;
            sub->_vC4(h4c);
            cf::CFunc8008EF04Sub98* p98 = (cf::CFunc8008EF04Sub98*)sub->field_98;
            if (sub->field_C4 != 0 && p98 != 0 &&
                (p98->field_7A4 & 0x10000u) == 0) {
                func_800BE12C(sub, 3, 0, -1, 1);
                sub->_v10(4);
                self->field_0x17C |= 1;
            }
        } else {
            // Not facing: with mode 2/3, raise the +0x10000 flag and issue the
            // matching battle-event / icon-dismiss sequence.
            s16 mode = v->field_0x18E;
            if (mode != 0 && (self->field_0x17C & 0x10000u) == 0 &&
                ((cf::CFunc8008EF04Sub*)self->field_0x34->field_0x28)
                        ->field_C4 != 0) {
                self->field_0x17C |= 0x10000;
                if (mode == 2) {
                    func_800BE12C(self->field_0x34->field_0x28, 1, 0, 0, 1);
                    func_8004B8B0(
                        ((cf::CFunc8008EF04Sub*)self->field_0x34->field_0x28)
                            ->field_C4,
                        0, 1, lbl_eu_806665C0);
                    if ((self->field_0x180 & 0x8u) != 0) {
                        ((cf::CFunc8008EF04Sub*)self->field_0x34->field_0x28)
                            ->_v150(1);
                    }
                } else if (mode == 3) {
                    func_800BE12C(self->field_0x34->field_0x28, 1, 0, 1, 1);
                    func_8004B8B0(
                        ((cf::CFunc8008EF04Sub*)self->field_0x34->field_0x28)
                            ->field_C4,
                        0, 1, lbl_eu_806665C0);
                    if ((self->field_0x180 & 0x8u) != 0) {
                        ((cf::CFunc8008EF04Sub*)self->field_0x34->field_0x28)
                            ->_v150(1);
                    }
                }
            }
        }
        // Commit the heading as the movement direction and zero the move data.
        f32 c = CosFIdx__Q24nw4r4mathFf(lbl_eu_806665CC *
                                        self->field_0x34->field_0x28->_vCC());
        f32 s = SinFIdx__Q24nw4r4mathFf(lbl_eu_806665CC *
                                        self->field_0x34->field_0x28->_vCC());
        v->mVelocity.x = s;
        v->mVelocity.z = c;
        v->mVelocity.y = lbl_eu_806665C0;
        ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
            lbl_eu_806665C0;
    }
}

void func_8008F2E0(){}

// Returns bit 8 of the +0x270 state word of the +0xC4 target object, or 1
// when the target is absent.
int cf::CfObjectMove::CfObject_UnkVirtualFunc18() {
    if (mTargetC4 != 0) {
        return (mTargetC4->field_0x270 >> 8) & 1;
    }
    return 1;
}

// Enemy flee/wander controller (retail func_8008F9EC): with the +0x20 flag
// clear the enemy wanders (refreshing the +0x58 rate divisor and picking a
// random heading via func_8008B9C0); with it set it flees the +0x4C-marked
// target until the +0x54 timer expires or it stays pinned near mVec138 for
// 60 frames, then clears the flee/wander flag bits.
void func_8008F9EC(cf::CfObjectMove* self) {
    cf::CfObjectMoveView9* v = (cf::CfObjectMoveView9*)self;
    cf::CFunc8008F9ECSub* sub =
        (cf::CFunc8008F9ECSub*)self->field_0x34->field_0x28;
    if (sub->field_C4 != 0) {
    if (sub->_v98() != 0) {
    if ((self->field_0x17C & 0x20u) == 0) {
        // Wander: refresh the movement-rate divisor, then either pick a
        // random direction (when the +0x5C-scaled probe succeeds) or idle.
        if (self->field_0x50 != 0) {
            v->field_0x58 = lbl_eu_806665E4;
        } else {
            f32 rate;
            if (*self->field_0x34->field_0x28->_v138() != lbl_eu_806665C0) {
                cf::CNpcMoveSubView* s =
                    (cf::CNpcMoveSubView*)self->field_0x34->field_0x28;
                f32 denom = *s->_v138();
                rate = *s->_v1D8() / denom;
            }
            v->field_0x58 = rate;
        }
        ml::CVec3 out;
        if (func_8008B9C0(self, &out, self, v->field_0x5C,
                          lbl_eu_806665C0, 0) != 0) {
            // Random heading change, scaled by the wander radius.
            v->field_0x60 = v->field_0x5C *
                (lbl_eu_80666668 * (f32)ml::math::mtRand(10) +
                 lbl_eu_80666610);
            // Cosine is evaluated first into a temp (live across the sine
            // call), then the direction vector is filled in x, y, z order.
            f32 cz = CosFIdx__Q24nw4r4mathFf(
                lbl_eu_806665CC * sub->_vCC());
            ml::CVec3 dir;
            dir.x = SinFIdx__Q24nw4r4mathFf(
                lbl_eu_806665CC * sub->_vCC());
            dir.y = lbl_eu_806665C0;
            dir.z = cz;
            func_80089694(self, &dir, v->field_0x58);
        } else {
            ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
                lbl_eu_806665C0;
        }
        self->field_0x184 = 0;
        self->field_0x54 = 0;
        ((cf::CfObjectMove4CView*)self)->field_4C &= ~3u;
        return;
    }

    // Flee path.
    ml::CVec3 dir(lbl_eu_806665C0, lbl_eu_806665C0, lbl_eu_806665C0);
    int cont = 1;
    if (((cf::CfObjectMove4CView*)self)->field_4C & 1u) {
        // Run from the marked target: heading opposite the target direction,
        // half-turn offset depending on the +0x4C bit 1 parity.
        f32 dx = ((cf::CFunc8008F9ECSub*)self->field_0x34->field_0x28)
                     ->_vAC()->x - v->mPosition.x;
        f32 dz = ((cf::CFunc8008F9ECSub*)self->field_0x34->field_0x28)
                     ->_vAC()->z - v->mPosition.z;
        f32 ang = lbl_eu_80666638 * Atan2FIdx__Q24nw4r4mathFff(dx, dz);
        if (((cf::CfObjectMove4CView*)self)->field_4C & 2u) {
            ang += lbl_eu_8066A204;
        } else {
            ang -= lbl_eu_8066A204;
        }
        ml::CVec3 tgt;
        tgt.x = v->field_0x60 *
                    SinFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * ang) +
                v->mPosition.x;
        tgt.z = v->field_0x60 *
                    CosFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * ang) +
                v->mPosition.z;
        tgt.y = v->mPosition.y;
        func_800896F4(self, &dir, &tgt);
        if ((s32)--self->field_0x54 <= 0) cont = 0;
    } else {
        func_800896F4(self, &dir, &v->mPosition);
        // Already next to the target: latch the flee state and a fresh
        // randomized timer (parity seeds the half-turn offset bit).
        f32 dx = v->mPosition.x -
                 ((cf::CFunc8008F9ECSub*)self->field_0x34->field_0x28)
                     ->_vAC()->x;
        f32 dz = v->mPosition.z -
                 ((cf::CFunc8008F9ECSub*)self->field_0x34->field_0x28)
                     ->_vAC()->z;
        if (dx * dx + dz * dz <= v->field_0x60 * v->field_0x60) {
            ((cf::CfObjectMove4CView*)self)->field_4C |= 1u;
            self->field_0x54 = ml::math::mtRand(0x12c) + 0x1c2;
            if ((self->field_0x54 & 1u) != 0) {
                ((cf::CfObjectMove4CView*)self)->field_4C |= 2u;
            }
        }
    }
    func_80089694(self, &dir, v->field_0x58);
    if (cont != 0) {
        // Pinned-proximity gate: count consecutive frames spent near mVec138.
        ml::CVec3* pos = ((cf::CFunc8008F9ECSub*)
                              self->field_0x34->field_0x28)->_vAC();
        ml::CVec3 diff;
        nw4r::math::VEC3Sub(diff, self->mVec138, *pos);
        int stop = 0;
        if (diff.x * diff.x + diff.y * diff.y + diff.z * diff.z <=
            lbl_eu_806665E4) {
            self->field_0x184 = self->field_0x184 + 1;
            if (self->field_0x184 > 60) {
                self->field_0x184 = 60;
                stop = 1;
            }
        } else {
            // Word copy of the target position into mVec138.
            *(u32*)&self->mVec138.x = *(u32*)&pos->x;
            *(u32*)&self->mVec138.y = *(u32*)&pos->y;
            *(u32*)&self->mVec138.z = *(u32*)&pos->z;
            self->field_0x184 = 0;
        }
        if (stop == 0) return;
    }
    // Flee over: clear the wander/flee flag bits.
    self->field_0x186 = 0;
    self->field_0x184 = 0;
    self->field_0x17C &= 0xFFF9FFDF;
    } else {
        return;
    }
    }
}

// Enemy move-controller per-frame update (retail func_8008FE8C): the +0x8000
// flag path reinstalls the +0x184 hook and resets the move-list state; the
// main path steps the +0x60 timer, latches the current move-list entry into
// the base position, and runs the direction / turn logic (the +0x70 & 0x40
// sub-heading branch, the +0x70 & 0x100 idle branch with the +0x74 timer, the
// +0x100000 pursuit branch), then the talk-state takeover, the +0x74 heading
// gate, the mVec138 pursuit-distance gate and finally the +0x5A row advance
// with the +0x190 / +0x19C / dispatch hooks.
void func_8008FE8C(cf::CfObjectMove* self) {
    cf::CFunc8008FE8CView* view = (cf::CFunc8008FE8CView*)self;
    // Kept live across the branchy region (retail parks it in a callee-saved
    // register); every assignment reads the same movement-data slot.
    cf::CFunc8008FE8CSub* sub;
    int flag;
    if ((view->field_0x17C & 0x8000u) != 0) {
        self->mMoveHook = lbl_eu_80527830.hook184;
        view->field_0x72 = 0;
        view->field_0x60 = 0;
        view->field_0x70 &= 0xFFFB;
        *(u32*)&view->mPosition.x = *(u32*)&view->field_0x4C;
        *(u32*)&view->mPosition.y = *(u32*)&view->field_0x50;
        *(u32*)&view->mPosition.z = *(u32*)&view->field_0x54;
        if ((self->field_0x180 & 0x8u) != 0) {
            view->field_0x160 = view->mPosition.y;
        }
        view->field_0x17C &= 0xFBFFFFFF;
        ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
            lbl_eu_806665C0;
        flag = 1;
    } else {
        flag = 0;
    }
    if (flag != 0) return;

    if ((view->field_0x17C & 0x20u) == 0) {
        u32 t = view->field_0x60;
        view->field_0x60 = t - 1;
        if (t > 0) return;
        view->field_0x60 = 0;
        cf::CfMoveSubEntry* entry =
            &view->field_0x78[(s16)view->field_0x5A];
        if ((entry->field_0xC & 0x400u) != 0) {
            view->field_0x70 |= 0x40;
        } else {
            view->field_0x70 &= ~0x40u;
        }
        cf::CfMoveSubEntry* entry2 =
            &view->field_0x78[(s16)view->field_0x5A];
        *(u32*)&view->mPosition.x = *(u32*)&entry2->field_0x0;
        *(u32*)&view->mPosition.y = *(u32*)&entry2->field_0x4;
        *(u32*)&view->mPosition.z = *(u32*)&entry2->field_0x8;
        view->field_0x17C |= 0x20;
        if ((self->field_0x180 & 0x8u) != 0) {
            view->field_0x160 = view->mPosition.y;
            view->field_0x74 = 0x3C;
            view->field_0x70 |= 0x80;
        } else {
            ml::CVec3 dir(lbl_eu_806665C0, lbl_eu_806665C0,
                          lbl_eu_806665C0);
            sub = (cf::CFunc8008FE8CSub*)self->field_0x34->field_0x28;
            if ((view->field_0x70 & 0x40u) != 0) {
                // Face the sub: commit the normalized direction.
                ml::CVec3 d = view->mPosition - *sub->vtable->fn_0xAC(sub);
                if (d.x != lbl_eu_806665C0 || d.y != lbl_eu_806665C0 ||
                    d.z != lbl_eu_806665C0) {
                    if (d.x * d.x + d.y * d.y + d.z * d.z ==
                        lbl_eu_806665C0) {
                        d = ml::CVec3::zero;
                    } else {
                        PSVECNormalize(d, d);
                    }
                }
                *(u32*)&view->mVelocity.x = *(u32*)&d.x;
                *(u32*)&view->mVelocity.y = *(u32*)&d.y;
                *(u32*)&view->mVelocity.z = *(u32*)&d.z;
                f32 ang = Atan2FIdx__Q24nw4r4mathFff(d.x, d.z);
                sub->vtable->fn_0xC4(sub, lbl_eu_80666638 * ang);
                if (sub->field_C4 != 0 && sub->field_98 != 0 &&
                    (((cf::CFunc8008EF04Sub98*)sub->field_98)->field_7A4 &
                     0x10000u) == 0) {
                    func_800BE12C(sub, 3, 0, -1, 1);
                    sub->vtable->fn_0x10(sub, 4);
                    view->field_0x17C |= 1;
                }
            } else if ((view->field_0x70 & 0x100u) != 0) {
                view->field_0x70 |= 0x80;
                view->field_0x74 = 0xA;
            } else if (sub->field_63C <= lbl_eu_806665C4) {
                func_80088974(self, &dir, &view->mPosition, 0, 0);
                view->field_0x60 = 12;
                view->field_0x17C |= 0x100000;
            } else {
                func_80088974(self, &dir, &view->mPosition, 0, 0);
                ml::CVec3 d = view->mPosition - *sub->vtable->fn_0xAC(sub);
                if (d.x != lbl_eu_806665C0 || d.y != lbl_eu_806665C0 ||
                    d.z != lbl_eu_806665C0) {
                    if (d.x * d.x + d.y * d.y + d.z * d.z ==
                        lbl_eu_806665C0) {
                        d = ml::CVec3::zero;
                    } else {
                        PSVECNormalize(d, d);
                    }
                }
                *(u32*)&view->mVelocity.x = *(u32*)&d.x;
                *(u32*)&view->mVelocity.y = *(u32*)&d.y;
                *(u32*)&view->mVelocity.z = *(u32*)&d.z;
                f32 ang = Atan2FIdx__Q24nw4r4mathFff(d.x, d.z);
                sub->vtable->fn_0xC4(sub, lbl_eu_80666638 * ang);
                if (sub->field_C4 != 0 && sub->field_98 != 0 &&
                    (((cf::CFunc8008EF04Sub98*)sub->field_98)->field_7A4 &
                     0x10000u) == 0) {
                    func_800BE12C(sub, 3, 0, -1, 1);
                    sub->vtable->fn_0x10(sub, 4);
                    view->field_0x17C |= 1;
                }
            }
            if ((view->field_0x70 & 0x80u) != 0) {
                ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
                    lbl_eu_806665C0;
                return;
            }
        }
    } else if ((view->field_0x17C & 0x100000u) != 0) {
        // Pursuit: turn toward the controller position, then hand off.
        ml::CVec3 dir;
        func_80088974(self, &dir, &view->mPosition, 0, 0);
        u32 t = view->field_0x60;
        view->field_0x60 = t - 1;
        if (t > 0) {
            ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
                lbl_eu_806665C0;
            return;
        }
        sub = (cf::CFunc8008FE8CSub*)self->field_0x34->field_0x28;
        f32 ang = Atan2FIdx__Q24nw4r4mathFff(dir.x, dir.z);
        sub->vtable->fn_0xC4(sub, lbl_eu_80666638 * ang);
        if (sub->field_C4 != 0 && sub->field_98 != 0 &&
            (((cf::CFunc8008EF04Sub98*)sub->field_98)->field_7A4 &
             0x10000u) == 0) {
            func_800BE12C(sub, 3, 0, -1, 1);
            sub->vtable->fn_0x10(sub, 4);
            view->field_0x17C |= 1;
        }
        *(u32*)&view->mVelocity.x = *(u32*)&dir.x;
        *(u32*)&view->mVelocity.y = *(u32*)&dir.y;
        *(u32*)&view->mVelocity.z = *(u32*)&dir.z;
        view->field_0x17C &= ~0x100000u;
        ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
            lbl_eu_806665C0;
        return;
    }

    // Talk-state takeover / +0x74 heading gate / pursuit distance.
    int r0 = 0;
    if ((view->field_0x17C & 1u) != 0) {
        sub = (cf::CFunc8008FE8CSub*)self->field_0x34->field_0x28;
        void* p98 = sub->field_98;
        if (p98 != 0 &&
            (((cf::CFunc8008EF04Sub98*)p98)->field_7A4 & 0x10000u) != 0) {
            if (sub->field_C4 != 0) {
                func_8004B9D4(sub->field_C4, 1, 0, -1, 0);
            }
        } else {
            if (sub->field_C4 != 0 &&
                sub->vtable->fn_0x0C(sub, 4) != 0) {
                ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
                    lbl_eu_806665C0;
                r0 = 1;
            }
        }
        if (r0 == 0) {
            view->field_0x17C &= ~1u;
        }
    }
    if (r0 != 0) return;
    view->field_0x17C |= 0x10000000;
    if ((view->field_0x70 & 0x80u) != 0) {
        s16 t74 = (s16)(view->field_0x74 - 1);
        view->field_0x74 = (s16)t74;
        view->field_0x3C = lbl_eu_8066666C;
        if (t74 <= 0) {
            view->field_0x70 &= ~0x80u;
        }
    }
    sub = (cf::CFunc8008FE8CSub*)self->field_0x34->field_0x28;
    ml::CVec3* p = sub->vtable->fn_0xAC(sub);
    f32 dx = p->x - view->mPosition.x;
    f32 dz = p->z - view->mPosition.z;
    f32 dist2 = dx * dx + dz * dz;
    f32 th = (sub->field_63C <= lbl_eu_806665C4) ? lbl_eu_80666670
                                                 : lbl_eu_806665E4;
    int r29 = (dist2 > th);
    ml::CVec3 dir74(lbl_eu_806665C0, lbl_eu_806665C0, lbl_eu_806665C0);
    if ((self->field_0x180 & 0x8u) != 0 ||
        (view->field_0x70 & 0x40u) != 0) {
        ml::CVec3 d = view->mPosition - *sub->vtable->fn_0xAC(sub);
        if (d.x != lbl_eu_806665C0 || d.y != lbl_eu_806665C0 ||
            d.z != lbl_eu_806665C0) {
            if (d.x * d.x + d.y * d.y + d.z * d.z == lbl_eu_806665C0) {
                d = ml::CVec3::zero;
            } else {
                PSVECNormalize(d, d);
            }
        }
        dir74 = d;
        func_800898D4(self, &dir74);
    } else if (sub->field_63C <= lbl_eu_806665C4) {
        ml::CVec3 vel = view->mVelocity;
        func_80088974(self, &dir74, &view->mPosition, 0, 0);
        view->mVelocity = vel;
        f32 h = sub->vtable->fn_0xCC(sub);
        f32 cos = CosFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * h);
        f32 sin = SinFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * h);
        f32 dot = dir74.x * sin + dir74.z * cos;
        if (dot <= lbl_eu_806665C0) {
            view->field_0x60 = 6;
            view->field_0x17C |= 0x100000;
            ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
                lbl_eu_806665C0;
            return;
        }
        if (dot <= lbl_eu_806665DC) {
            view->field_0x3C = lbl_eu_80666658;
        }
        func_800898D4(self, &dir74);
    } else {
        func_80088974(self, &dir74, &view->mPosition, 1, 0);
    }
    cf::CfMoveSubEntry* entry3 =
        &view->field_0x78[(s16)view->field_0x5A];
    if ((entry3->field_0xC & 0x8000u) != 0 && dist2 > lbl_eu_80666674) {
        if (view->field_0x72 == 0) {
            if ((view->field_0x70 & 0x2u) != 0) {
                view->field_0x70 &= ~0x2u;
                view->field_0x72 = 0xA;
                view->field_0x64 = lbl_eu_806665C0;
            } else {
                view->field_0x70 |= 0x2;
                view->field_0x72 = 0x28;
                view->field_0x64 =
                    (f32)((rand() % 160) - 80) * lbl_eu_8066A210;
            }
        }
        view->field_0x68 = lbl_eu_8066666C *
                               (view->field_0x64 - view->field_0x68) +
                           view->field_0x68;
        view->field_0x72 = view->field_0x72 - 1;
        f32 f30 = view->field_0x68 +
                  lbl_eu_80666638 *
                      Atan2FIdx__Q24nw4r4mathFff(dir74.x, dir74.z);
        dir74.x = SinFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * f30);
        dir74.z = CosFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * f30);
    }
    // mVec138 pursuit-distance gate.
    ml::CVec3* p2 = sub->vtable->fn_0xAC(sub);
    f32 mdx = *(f32*)&view->mVec138W.x - p2->x;
    f32 mdz = *(f32*)&view->mVec138W.z - p2->z;
    if (mdx * mdx + mdz * mdz <= lbl_eu_806665E4) {
        view->field_0x184 = view->field_0x184 + 1;
        if (view->field_0x184 > 0x3C) {
            view->field_0x184 = 0x3C;
            r0 = 1;
        } else {
            r0 = 0;
        }
    } else {
        view->mVec138W.x = *(u32*)&p2->x;
        view->mVec138W.y = *(u32*)&p2->y;
        view->mVec138W.z = *(u32*)&p2->z;
        view->field_0x184 = 0;
        r0 = 0;
    }
    if (r0 != 0) {
        func_80089694(self, &dir74, lbl_eu_806665E4);
    } else {
        func_80089694(self, &dir74, view->field_0x6C);
    }
    if (r29 != 0) {
        if ((self->field_0x180 & 0x8u) != 0) {
            // Keep the sub within reach of the controller.
            if (sub->field_C4 == 0 || sub->field_98 == 0) {
                ml::CVec3 stk = *sub->vtable->fn_0xAC(sub);
                f32 diff = view->field_0x160 - stk.y;
                if (fabsf(diff) > lbl_eu_806665E4) {
                    if (diff > lbl_eu_806665C0) {
                        stk.y += lbl_eu_80666640;
                    } else {
                        stk.y -= lbl_eu_80666640;
                    }
                }
                sub->vtable->fn_0x9C(sub, &stk);
            }
            return;
        }
        view->field_0x17C &= 0xFFEFFFDF;
        cf::CfMoveSubEntry* entry4 =
            &view->field_0x78[(s16)view->field_0x5A];
        if ((entry4->field_0xC & 0x8u) != 0) {
            if (sub->field_C4 != 0) {
                func_800BE12C(sub, 0x66, 0, -1, 1);
                view->field_0x17C |= 0x8;
            }
        } else if ((entry4->field_0xC & 0x10u) != 0) {
            view->field_0x17C |= 0x80;
        }
        cf::CfMoveSubEntry* entry5 =
            &view->field_0x78[(s16)view->field_0x5A];
        if ((entry5->field_0xC & 0x80u) != 0) {
            func_80089684(self);
            self->mMoveHook = lbl_eu_80527830.hook190;
            view->field_0x4C = self->field_0x34->field_0x28->_vCC();
            self->field_0x180 |= 0x800;
            ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
                lbl_eu_806665C0;
            return;
        }
        if ((entry5->field_0xC & 0x20u) != 0) {
            self->mMoveHook = lbl_eu_80527830.hook19C;
            view->field_0x70 |= 0x4;
            view->field_0x72 = entry5->field_0xF * 30;
            ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
                lbl_eu_806665C0;
            return;
        }
        if ((entry5->field_0xC & 0x4u) != 0) {
            view->field_0x70 |= 1;
        }
        (self->*((const CfMoveDispatchPtmf*)&lbl_eu_80527830)
             [view->field_0x5C])(&view->field_0x60, &view->field_0x6C);
        view->field_0x60 = entry5->field_0xE * 30;
        if ((view->field_0x70 & 1u) != 0) {
            s16 t5 = (s16)(view->field_0x5A - 1);
            view->field_0x5A = (s16)t5;
            if (t5 < 0) {
                view->field_0x5A = 1;
                view->field_0x70 &= ~1u;
            }
        } else {
            s16 t5 = (s16)(view->field_0x5A + 1);
            view->field_0x5A = (s16)t5;
            if (t5 >= view->field_0x58) {
                view->field_0x5A = 0;
            }
        }
        if ((f32)view->field_0x60 == lbl_eu_806665C0) {
            view->field_0x70 |= 0x100;
        } else {
            ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
                lbl_eu_806665C0;
            view->field_0x70 &= ~0x100u;
        }
    }
}


// Enemy move-controller per-frame update (retail func_80090DB4): the +0x8000
// flag path reinstalls the +0x1A8 hook, resets the move-list counters and
// copies the heading into the base position; otherwise the move-list entry at
// +0x78 + field_0x5A*0x14 drives the update: when the +0x72 gate is clear it
// installs the +0x1B4 hook, dispatches the +0x5C record, steps the +0x5A
// timer and clears the flags; when set it decrements the +0x72 gate and runs
// the approach/pursuit logic (func_8008B9C0 probe, direction commit via the
// +0xC4 heading, the +0x20 / +0x100000 / +1 flag branches, the mVec138
// pursuit-distance gate) before dispatching the state record again.
void func_80090DB4(cf::CfObjectMove* self) {
    const CfMoveHookPtmfsAll& hooks = lbl_eu_80527830;
    cf::CFunc80090DB4View* view = (cf::CFunc80090DB4View*)self;
    int flag;
    if ((view->field_0x17C & 0x8000u) != 0) {
        // Re-entry: reinstall the +0x1A8 hook and reset the move-list state.
        self->mMoveHook = hooks.hook1A8;
        view->field_0x72 = 0;
        view->field_0x60 = 0;
        view->field_0x70 &= 0xFFFB;
        *(u32*)&view->mPosition.x = *(u32*)&view->field_0x4C;
        *(u32*)&view->mPosition.y = *(u32*)&view->field_0x50;
        *(u32*)&view->mPosition.z = *(u32*)&view->field_0x54;
        if ((self->field_0x180 & 0x8u) != 0) {
            view->field_0x160 = view->mPosition.y;
        }
        view->field_0x17C &= 0xFBFFFFFF;
        ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
            lbl_eu_806665C0;
        flag = 1;
    } else {
        flag = 0;
    }
    if (flag != 0) return;

    // Active row of the +0x78 move list drives the update.
    cf::CfMoveSubEntry* entry =
        &view->field_0x78[(s16)view->field_0x5A];
    if (view->field_0x72 == 0) {
        // Gate clear: install the +0x1B4 hook, dispatch the state record and
        // step the +0x5A row timer.
        self->mMoveHook = hooks.hook1B4;
        u32 ev = view->field_0x70 & 0xFFFBu;
        view->field_0x70 = (u16)ev;
        if ((entry->field_0xC & 4u) != 0) {
            view->field_0x70 = (u16)(ev | 1);
        }
        (self->*((const CfMoveDispatchPtmf*)&lbl_eu_80527830)
             [view->field_0x5C])(&view->field_0x60, &view->field_0x6C);
        view->field_0x60 = entry->field_0xE * 30;
        if ((view->field_0x70 & 1u) != 0) {
            s16 t = (s16)(view->field_0x5A - 1);
            view->field_0x5A = t;
            if (t < 0) {
                view->field_0x5A = 1;
                view->field_0x70 &= 0xFFFE;
            }
        } else {
            s16 t = (s16)(view->field_0x5A + 1);
            view->field_0x5A = t;
            if (t >= view->field_0x58) {
                view->field_0x5A = 0;
            }
        }
        view->field_0x60 = 0;
        view->field_0x17C &= 0xFFE9FFDF;
        ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
            lbl_eu_806665C0;
        view->field_0x184 = 0;
        return;
    }

    // Gate set: step it and run the approach / pursuit logic. Retail keeps
    // the move sub-object in a callee-saved register across the branches.
    view->field_0x72 = view->field_0x72 - 1;
    u32 flags = view->field_0x17C;
    cf::CFunc80090DB4Sub* sub =
        (cf::CFunc80090DB4Sub*)self->field_0x34->field_0x28;
    if ((flags & 0x20u) == 0) {
        // Approach a candidate point around the entry (func_8008B9C0).
        u32 t = view->field_0x60;
        view->field_0x60 = t - 1;
        if ((int)t > 0) return;
        view->field_0x60 = 0;
        ml::CVec3 out;
        if (func_8008B9C0(self, &out, (cf::CfObjectMove*)entry,
                          entry->field_0x10, lbl_eu_806665C0, 0) != 0) {
            if ((self->field_0x180 & 0x8u) == 0 &&
                func_80089E88(self, &view->mPosition, 1) == 0) {
                // Probe rejected: retry after a short delay.
                view->field_0x60 = 12;
                view->field_0x17C |= 0x100000;
            } else {
                // Commit: turn the sub toward the adopted point and mirror
                // the normalized direction into the velocity.
                cf::CFunc80090DB4Sub* sub2 =
                    (cf::CFunc80090DB4Sub*)self->field_0x34->field_0x28;
                ml::CVec3 dir = view->mPosition - *sub2->vtable->fn_0xAC(sub2);
                if (dir.x != lbl_eu_806665C0 || dir.y != lbl_eu_806665C0 ||
                    dir.z != lbl_eu_806665C0) {
                    if (dir.x * dir.x + dir.y * dir.y + dir.z * dir.z ==
                        lbl_eu_806665C0) {
                        dir = ml::CVec3::zero;
                    } else {
                        PSVECNormalize(dir, dir);
                    }
                }
                f32 ang = Atan2FIdx__Q24nw4r4mathFff(dir.x, dir.z);
                sub2->vtable->fn_0xC4(sub2, lbl_eu_80666638 * ang);
                if (sub2->field_C4 != 0 && sub2->field_98 != 0 &&
                    (((cf::CFunc8008EF04Sub98*)sub2->field_98)->field_7A4 &
                     0x10000u) == 0) {
                    func_800BE12C(sub2, 3, 0, -1, 1);
                    sub2->vtable->fn_0x10(sub2, 4);
                    view->field_0x17C |= 1;
                }
                *(u32*)&view->mVelocity.x = *(u32*)&dir.x;
                *(u32*)&view->mVelocity.y = *(u32*)&dir.y;
                *(u32*)&view->mVelocity.z = *(u32*)&dir.z;
            }
        }
        ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
            lbl_eu_806665C0;
        view->field_0x184 = 0;
        return;
    }

    if ((flags & 0x100000u) != 0) {
        // Pursuit: head toward the controller position while the +0x60 timer
        // runs, then re-approach.
        ml::CVec3 dir;
        func_80088974(self, &dir, &view->mPosition, 0, 0);
        u32 t = view->field_0x60;
        view->field_0x60 = t - 1;
        if (t == 0) {
            view->field_0x60 = 0;
            cf::CFunc80090DB4Sub* sub2 =
                (cf::CFunc80090DB4Sub*)self->field_0x34->field_0x28;
            f32 ang = Atan2FIdx__Q24nw4r4mathFff(dir.x, dir.z);
            sub2->vtable->fn_0xC4(sub2, lbl_eu_80666638 * ang);
            if (sub2->field_C4 != 0 && sub2->field_98 != 0 &&
                (((cf::CFunc8008EF04Sub98*)sub2->field_98)->field_7A4 &
                 0x10000u) == 0) {
                func_800BE12C(sub2, 3, 0, -1, 1);
                sub2->vtable->fn_0x10(sub2, 4);
                view->field_0x17C |= 1;
            }
            *(u32*)&view->mVelocity.x = *(u32*)&dir.x;
            *(u32*)&view->mVelocity.y = *(u32*)&dir.y;
            *(u32*)&view->mVelocity.z = *(u32*)&dir.z;
            view->field_0x17C &= ~0x100000u;
        }
        ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
            lbl_eu_806665C0;
        return;
    }

    int done = 0;
    if ((flags & 1u) != 0) {
        // Talk-state takeover: dismiss the +0xC4 icon while the busy state
        // holds the +0x10000 bit, hand control to the icon query, else drop
        // the +1 flag.
        void* p98 = sub->field_98;
        if (p98 != 0 &&
            (((cf::CFunc8008EF04Sub98*)p98)->field_7A4 & 0x10000u) != 0) {
            if (sub->field_C4 != 0) {
                func_8004B9D4(sub->field_C4, 1, 0, -1, 0);
            }
        } else {
            if (sub->field_C4 != 0 &&
                sub->vtable->fn_0x0C(sub, 4) != 0) {
                ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
                    lbl_eu_806665C0;
                done = 1;
            }
        }
        if (done == 0) {
            view->field_0x17C &= ~1u;
        }
    }
    if (done != 0) return;

    // Close pursuit: keep the sub within reach of the controller. Retail
    // re-queries the position between the two axis loads (dx lives across
    // the second virtual call).
    ml::CVec3* pa = sub->vtable->fn_0xAC(sub);
    f32 dx = view->mPosition.x - pa->x;
    ml::CVec3* pb = sub->vtable->fn_0xAC(sub);
    f32 dz = view->mPosition.z - pb->z;
    if (dx * dx + dz * dz > lbl_eu_806665E4) {
        // Out of reach: count the escape frames, then pursue via
        // func_8008C660 / the mVec138 waypoint latch.
        u16 esc = view->field_0x186;
        view->field_0x186 = esc + 1;
        if (esc < 0x258) {
            ml::CVec3 z(lbl_eu_806665C0, lbl_eu_806665C0,
                        lbl_eu_806665C0);
            if (func_8008C660(self, &z,
                              lbl_eu_80666628 * sub->field_63C) != 0) {
                return;
            }
            ml::CVec3* p2 = sub->vtable->fn_0xAC(sub);
            f32 mdx = *(f32*)&view->mVec138W.x - p2->x;
            f32 mdz = *(f32*)&view->mVec138W.z - p2->z;
            if (mdx * mdx + mdz * mdz <= lbl_eu_806665E4) {
                // Reached the waypoint: count the idle frames, then give up.
                u16 idle = view->field_0x184 + 1;
                view->field_0x184 = idle;
                if ((u32)idle > 0x3C) {
                    view->field_0x184 = 0x3C;
                    done = 1;
                }
            } else {
                // Waypoint moved: relatch it from a fresh position query.
                ml::CVec3* p3 = sub->vtable->fn_0xAC(sub);
                view->mVec138W.x = *(u32*)&p3->x;
                view->mVec138W.y = *(u32*)&p3->y;
                view->mVec138W.z = *(u32*)&p3->z;
                view->field_0x184 = 0;
            }
            if (done == 0) {
                func_80089694(self, &z, view->field_0x6C);
                return;
            }
        }
    }
    // Dispatch the state record and finalize the move-list fields.
    (self->*((const CfMoveDispatchPtmf*)&lbl_eu_80527830)
         [view->field_0x5C])(&view->field_0x60, &view->field_0x6C);
    view->field_0x184 = 0;
    view->field_0x60 = entry->field_0xE * 30;
    view->field_0x186 = 0;
    view->field_0x17C &= 0xFFE9FFDF;
    ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
        lbl_eu_806665C0;
}


// Enemy pursuit scan (retail func_8009156C): while the +0x8000 flag is set,
// either leap toward the move sub's position (when far) or, once close, run a
// two-tick gate before scanning the gimmick list for nearby targets to mark.
void func_8009156C(cf::CfObjectMove* self) {
    cf::CfObjectMoveView9* v = (cf::CfObjectMoveView9*)self;
    cf::CFunc8008F9ECSub* sub;
    if ((v->field_0x17C & 0x8000u) != 0) {
        f32 dx = v->mPosition.x -
                 ((cf::CFunc8008F9ECSub*)self->field_0x34->field_0x28)
                     ->_vAC()->x;
        f32 dz = v->mPosition.z -
                 ((cf::CFunc8008F9ECSub*)self->field_0x34->field_0x28)
                     ->_vAC()->z;
        if (dx * dx + dz * dz > lbl_eu_806665E4) {
            ml::CVec3 zero(lbl_eu_806665C0, lbl_eu_806665C0, lbl_eu_806665C0);
            func_80088974(self, &zero, &v->mPosition, 1, 0);
            func_80089694(self, &zero, lbl_eu_806665E4);
            return;
        }
        // Close: honour the +0x70 event bits, then step the +0x72 gate.
        if (v->field_0x72 == 0) {
            if ((v->field_0x70 & 0x10u) != 0) {
                if (((cf::CFunc8008F9ECSub*)self->field_0x34->field_0x28)
                        ->field_C4 != 0) {
                    func_800BE12C(self->field_0x34->field_0x28, 0x66, 0, -1, 1);
                    v->field_0x17C |= 0x8;
                }
            } else if ((v->field_0x70 & 0x20u) != 0) {
                v->field_0x17C |= 0x80;
            }
        }
        v->field_0x72 = v->field_0x72 + 1;
        v->field_0x17C |= 0x10000000u;
        if (v->field_0x72 >= 2) {
            // Scan the gimmick list: mark any target whose XZ distance to the
            // move sub is at most the threshold.
            sub = (cf::CFunc8008F9ECSub*)self->field_0x34->field_0x28;
            cf::CFunc8009156CList* list = func_800B6BC8();
            f32 th = lbl_eu_80666678;
            for (cf::CFunc8009156CNode* node = list->head->next;
                 node != list->head; node = node->next) {
                cf::CFunc8009156CGimmick* gimmick = node->object;
                ml::CVec3* gp = gimmick->_vAC();
                ml::CVec3 diff;
                nw4r::math::VEC3Sub(diff, *sub->_vAC(), *gp);
                if (diff.x * diff.x + diff.z * diff.z <= th) {
                    cf::CFunc8009156CTarget* t =
                        (cf::CFunc8009156CTarget*)gimmick->_v110();
                    if (t != 0) {
                        int flag7 = (v->field_0x180 >> 7) & 1;
                        if (t->field_B8->field_28->field_C4 != 0) {
                            t->field_200 |= 0x4010;
                            t->field_1E0 = v->field_0x15C;
                            if (flag7 != 0) {
                                t->field_200 |= 0x800;
                            }
                        }
                    }
                }
            }
            if (((cf::CFunc8008F9ECSub*)self->field_0x34->field_0x28)
                    ->field_C4 != 0) {
                v->field_0x15C = v->field_0x15C;
                v->field_0x17C |= 0x4010;
            }
            v->field_0x17C &= ~0x8000u;
        } else {
            ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
                lbl_eu_806665C0;
            return;
        }
    }
    // Common tail: install the +0x12C hook, clear the +0x04000000 flag and
    // reset the +0x72 gate / +0x60 word / move-data float.
    self->mMoveHook = lbl_eu_805279F0;
    v->field_0x17C &= ~0x04000000u;
    v->field_0x72 = 0;
    v->field_0x60 = 0;
    ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
        lbl_eu_806665C0;
}

// Enemy movement state machine (retail func_80091864). Validates the enemy
// battle object (+0x3F60 flag object), resolves the player battle object,
// then either wanders toward a random nearby candidate or chases the player:
// seeds a random heading from field_0x5C, scans the actor list for candidates
// whose predicted position is within the summed proximity radius, and feeds
// the resulting direction into the party-info builder / movement commit.
// NOTE: the middle stretch of the retail routine was not available when this
// reconstruction was written; the tail state handling below is inferred from
// the surrounding branches.
void func_80091864(cf::CfObjectMove* selfRaw) {
    cf::CFunc80091864View* self = (cf::CFunc80091864View*)selfRaw;
    cf::CNpcBaseDataView* data = self->field_0x34;
    cf::CFunc8009DataView* data14 = (cf::CFunc8009DataView*)data;
    cf::CFunc8008EF04Sub* movesub = (cf::CFunc8008EF04Sub*)data->field_0x28;

    // Enemy battle object: the move-data +0x28 sub-object sits at +0x3E9C of
    // the containing battle object; recover the base pointer.
    cf::CFunc80091864Actor* ene =
        (cf::CFunc80091864Actor*)((u8*)data->field_0x28 -
                                  offsetof(cf::CFunc80091864Actor, mSub));

    if (ene->field_3F60 == 0) {
        data14->field_0x14 = lbl_eu_806665C0;
        return;
    }

    cf::CFunc8008E760B89* b89 =
        (cf::CFunc8008E760B89*)func_800B89CC(ene->field_45C0);
    if (b89 == 0) {
        data14->field_0x14 = lbl_eu_806665C0;
        return;
    }

    cf::CFunc80091864Actor* player =
        (cf::CFunc80091864Actor*)func_80198310();
    if (player == 0) {
        if ((self->field_0x180 & 0x8) != 0) {
            self->field_0x160 = self->field_0x4;
        }
        data14->field_0x14 = lbl_eu_806665C0;
        return;
    }

    // Latch the base position into mVec144 while the +0x400000 state bit is
    // up (retail copies the three words). The bit sits above the halfword
    // load's range, so the branch is effectively dead in retail too.
    if ((self->field_0x58 & 0x4000) != 0) {
        self->mVec144W = self->mPos0W;
    }

    // +0x17C bit 0 pending: re-check the move-sub target gate. When the
    // +0x98 object carries its flag, poke the icon helper instead; otherwise
    // query the move-sub's +0x0C slot - success ends the frame early without
    // clearing the pending bit.
    int blocked = 0;
    if ((self->field_0x17C & 1) != 0) {
        cf::CFunc8008EF04Sub98* s98 =
            (cf::CFunc8008EF04Sub98*)movesub->field_98;
        if (s98 != 0 && (s98->field_7A4 & 0x8000) != 0) {
            if (movesub->field_C4 != 0) {
                func_8004B9D4(movesub, 1, 0, -1, 0);
            }
        } else if (movesub->field_C4 != 0 && movesub->_v0C(4) != 0) {
            data14->field_0x14 = lbl_eu_806665C0;
            blocked = 1;
        }
        if (blocked == 0) {
            self->field_0x17C &= ~1u;
        }
    }
    if (blocked != 0) return;

    if (player->field_3F60 == 0) {
        data14->field_0x14 = lbl_eu_806665C0;
        return;
    }

    // Target activity: bit 3 of the player target's +0x200 flags word.
    int tgtActive = 0;
    {
        cf::CFunc80091864Target* t = (cf::CFunc80091864Target*)(
            player->mSub.vtable->fn_0x110(&player->mSub));
        if (t != 0) tgtActive = (int)((t->field_200 >> 3) & 1);
    }

    u16 hw58 = self->field_0x58;
    f32 radius = b89->field_8C;
    u32 seed = self->field_0x5C;

    if ((hw58 & 1) != 0) {
        // Step the entry gate; past 15 entries drop back out of the state.
        u16 gate = (u16)(self->field_0x5A + 1);
        self->field_0x5A = gate;
        if (gate > 0xF) {
            self->field_0x58 = (u16)(hw58 & ~1u);
            self->field_0x5A = 0;
            hw58 &= (u16)~1u;
        }
        if ((self->field_0x180 & 0x8) != 0) {
            if ((player->field_3374 & 0x100) != 0) {
                self->field_0x160 =
                    player->mSub.vtable->fn_0xAC(&player->mSub)->y;
            } else {
                self->field_0x160 =
                    lbl_eu_806665D0 +
                    player->mSub.vtable->fn_0xAC(&player->mSub)->y;
            }
            hw58 = self->field_0x58;
            seed = self->field_0x5C;
            radius = b89->field_8C;
        }
    }

    u16 esc = ene->field_45C4;
    if (esc == 0) esc = 1;

    if ((hw58 & 2) != 0) {
        // Wander countdown: when the timer lapses, reroll the seed and drop
        // the wander bit.
        s16 t = (s16)(self->field_0x6C - 1);
        self->field_0x6C = t;
        if (t < 0) {
            self->field_0x5C = (u32)(ml::math::mtRand() & 0xFFFF);
            self->field_0x58 = (u16)(hw58 & ~2u);
        }
    } else {
        // Pick a fresh random heading from the seed: seed mod 360 scaled to
        // radians plus the current player heading; walk distance scales
        // seed mod 100 by the proximity factor and adds both radii.
        s32 sv = (s32)seed;
        s32 rem360 = sv - sv / 360 * 360;
        s32 rem100 = sv - sv / 100 * 100;
        f32 ang = (f32)((f64)rem360 - lbl_eu_80666620) * lbl_eu_8066A210;
        f32 head = player->vtable->fn_0x5B4(player) + ang;
        f32 dist = lbl_eu_80666658 * (f32)((f64)rem100 - lbl_eu_80666620) *
                       radius +
                   ene->field_44D8 + player->field_44D8;

        ml::CVec3* pv = player->mSub.vtable->fn_0xAC(&player->mSub);
        f32 px = pv->x + dist * SinFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * head);
        f32 pz = pv->z + dist * CosFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * head);

        // Scan the actor list backwards for a candidate whose predicted
        // position lands inside ene->field_44D8 + cand->field_44D8.
        int found = 0;
        for (s32 idx = (s32)ene->field_45C6 - 1; idx >= 0; idx--) {
            cf::CFunc80091864Actor* cand =
                (cf::CFunc80091864Actor*)func_801984F0(b89, idx);
            if (cand == 0) continue;
            cf::CFunc80091864Target* t = (cf::CFunc80091864Target*)(
                cand->mSub.vtable->fn_0x110(&cand->mSub));
            if (t == 0) continue;
            s32 iv = (s32)t->field_E0;
            s32 irem360 = iv - iv / 360 * 360;
            s32 irem100 = iv - iv / 100 * 100;
            f32 ihead = lbl_eu_806665CC * head +
                        (f32)((f64)irem360 - lbl_eu_80666620) *
                            lbl_eu_8066A210;
            f32 idist =
                lbl_eu_80666658 * (f32)((f64)irem100 - lbl_eu_80666620) *
                    radius +
                cand->field_44D8 + player->field_44D8;
            ml::CVec3* cp = cand->mSub.vtable->fn_0xAC(&cand->mSub);
            f32 cx = cp->x + idist * SinFIdx__Q24nw4r4mathFf(ihead);
            f32 cz = cp->z + idist * CosFIdx__Q24nw4r4mathFf(ihead);
            f32 dx = px - cx;
            f32 dz = pz - cz;
            f32 th = lbl_eu_8066667C * (ene->field_44D8 + cand->field_44D8);
            if (dx * dx + dz * dz <= th) {
                found = 1;
                break;
            }
        }
        if (found != 0) {
            self->field_0x5C = (u32)(ml::math::mtRand() & 0xFFFF);
        } else {
            // Nothing close: enter the wander countdown.
            self->field_0x6C = 0x258;
            self->field_0x58 = (u16)(hw58 | 2);
        }
    }

    // Common tail: rebuild the party info block around the player's current
    // position / heading, then refresh the height latch under the flag bits.
    ml::CVec3 pos = *player->mSub.vtable->fn_0xAC(&player->mSub);
    f32 head2 = player->vtable->fn_0x5B4(player);
    cf::CFunc8008E760PartyInfo pi;
    func_80198710(&pi, &pos, head2, (int)esc, (int)seed, radius,
                  ene->field_44D8 + player->field_44D8);
    if ((self->field_0x180 & 1) != 0) {
        pi.field_2D = 0;
    }
    if ((self->field_0x180 & 0x8) != 0) {
        pi.field_2D = 0;
        if ((player->field_3374 & 0x100) != 0) {
            self->field_0x160 =
                player->mSub.vtable->fn_0xAC(&player->mSub)->y;
        } else {
            self->field_0x160 =
                lbl_eu_806665D0 +
                player->mSub.vtable->fn_0xAC(&player->mSub)->y;
        }
    }

    // Tail state machine: chase bookkeeping against the enemy's own sub
    // position, then either record it, count the lost frames or turn to face
    // the player and mark the move-sub target.
    ml::CVec3* ep = ene->mSub.vtable->fn_0xAC(&ene->mSub);
    f32 dxp = (f32)self->mPos0W.x - ep->x;
    f32 dzp = (f32)self->mPos0W.z - ep->z;
    if (dxp * dxp + dzp * dzp > lbl_eu_8066667C) {
        // Drifting away: step the lost counter, latch at 60 frames and raise
        // the give-up bits.
        s16 lost = (s16)(self->field_0x70 + 1);
        self->field_0x70 = lost;
        if (lost >= 0x3C) {
            self->field_0x70 = 0x3C;
            self->field_0x58 = (u16)(self->field_0x58 | 0x60);
        }
        return;
    }
    self->field_0x70 = 0;
    self->field_4C = *(u32*)&ep->x;
    self->field_50 = *(u32*)&ep->y;
    self->field_54 = *(u32*)&ep->z;

    if ((self->field_0x58 & 0x8) != 0) {
        data14->field_0x14 = lbl_eu_806665C0;
        return;
    }

    // Face the player: heading-alignment check between the two objects; when
    // aligned, poke the move-sub target through its event helpers.
    self->mVel18W.x = *(u32*)&lbl_eu_806665C0;
    f32 eh = ene->vtable->fn_0x5B4(ene);
    f32 ph = player->vtable->fn_0x5B4(player);
    f32 align = CosFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * eh) *
                    CosFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * ph) +
                SinFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * eh) *
                    SinFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * ph);
    if (align < lbl_eu_80666694) {
        data14->field_0x14 = lbl_eu_806665C0;
        return;
    }
    if (movesub->field_C4 != 0 && movesub->field_98 != 0 &&
        (((cf::CFunc8008EF04Sub98*)movesub->field_98)->field_7A4 & 0x8000) ==
            0) {
        func_800BE12C(data->field_0x28, 3, 0, -1, 1);
        movesub->_v10(4);
        self->field_0x17C |= 1;
    }
    f32 pvz = SinFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * ph);
    f32 pcos = CosFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * ph);
    self->mVel18W.z = *(u32*)&pvz;
    self->mVel18W.y = *(u32*)&lbl_eu_806665C0;
    self->mVel18W.x = *(u32*)&pcos;
    data14->field_0x14 = lbl_eu_806665C0;
}

void* CfObjectMove_UnkVirtualFunc5__Q22cf12CfObjectMoveFv(void* self) { return (void*)((u8*)self + 0x6e8); }

void func_80092CB0(void* a, void* b, void* c) {
    *(unsigned long*)b = 0;
    *(float*)c = lbl_eu_806665C0;
}

// Stores the movement-rate divisor into *out2: the +0x1D8 slot's value over
// the +0x138 slot's value, falling back to 0.0f when +0x138 reads 0. *out1
// receives 0.
void func_80092CC4(cf::CfObjectMove* self, u32* out1, f32* out2) {
    *out1 = 0;
    f32 result;
    if (*self->field_0x34->field_0x28->_v138() != lbl_eu_806665C0) {
        cf::CNpcMoveSubView* sub = self->field_0x34->field_0x28;
        f32 denom = *sub->_v138();
        result = *sub->_v1D8() / denom;
    } else {
        result = lbl_eu_806665C0;
    }
    *out2 = result;
}

void func_80092D78(cf::CfObjectMove* self, u32* out1, f32* out2) {
    *out1 = 0x12C;
    f32 result;
    if (*self->field_0x34->field_0x28->_v138() != lbl_eu_806665C0) {
        cf::CNpcMoveSubView* sub = self->field_0x34->field_0x28;
        f32 denom = *sub->_v138();
        result = *sub->_v1D8() / denom;
    } else {
        result = lbl_eu_806665C0;
    }
    *out2 = result;
}

void func_80092E2C(cf::CfObjectMove* self, u32* out1, f32* out2) {
    *out1 = 0x258;
    f32 result;
    if (*self->field_0x34->field_0x28->_v138() != lbl_eu_806665C0) {
        cf::CNpcMoveSubView* sub = self->field_0x34->field_0x28;
        f32 denom = *sub->_v138();
        result = *sub->_v1D8() / denom;
    } else {
        result = lbl_eu_806665C0;
    }
    *out2 = result;
}

void func_80092EE0(cf::CfObjectMove* self, u32* out1, f32* out2) {
    *out1 = 0x384;
    f32 result;
    if (*self->field_0x34->field_0x28->_v138() != lbl_eu_806665C0) {
        cf::CNpcMoveSubView* sub = self->field_0x34->field_0x28;
        f32 denom = *sub->_v138();
        result = *sub->_v1D8() / denom;
    } else {
        result = lbl_eu_806665C0;
    }
    *out2 = result;
}

void func_80092F94(void* a, void* b, void* c) {
    *(unsigned long*)b = 0;
    *(float*)c = lbl_eu_806665E4;
}

void func_80092FA8(void* u, u32* a, f32* b) {
    *a = 0x12C;
    *b = lbl_eu_806665E4;
}

void func_80092FBC(void* u, u32* a, f32* b) {
    *a = 0x258;
    *b = lbl_eu_806665E4;
}

void func_80092FD0(void* u, u32* a, f32* b) {
    *a = 0x384;
    *b = lbl_eu_806665E4;
}

// Rand-gated rate query: when mtRand(100) is at/above the threshold, writes
// the 1.0f fallback to *out2; otherwise writes the movement-rate divisor
// (*out2 = +0x1D8 rate over +0x138 divisor, 0.0f when the divisor reads 0).
// *out1 receives 0.
void func_80092FE4(cf::CfObjectMove* self, u32* out1, f32* out2) {
    if (ml::math::mtRand(100) >= 0x50) {
        *out2 = lbl_eu_806665E4;
    } else {
        f32 result;
        if (*self->field_0x34->field_0x28->_v138() != lbl_eu_806665C0) {
            cf::CNpcMoveSubView* sub = self->field_0x34->field_0x28;
            f32 denom = *sub->_v138();
            result = *sub->_v1D8() / denom;
        } else {
            result = lbl_eu_806665C0;
        }
        *out2 = result;
    }
    *out1 = 0;
}

// Same as func_80092FE4 with threshold 70 and *out1 = 0x12C.
void func_800930C0(cf::CfObjectMove* self, u32* out1, f32* out2) {
    if (ml::math::mtRand(100) >= 0x46) {
        *out2 = lbl_eu_806665E4;
    } else {
        f32 result;
        if (*self->field_0x34->field_0x28->_v138() != lbl_eu_806665C0) {
            cf::CNpcMoveSubView* sub = self->field_0x34->field_0x28;
            f32 denom = *sub->_v138();
            result = *sub->_v1D8() / denom;
        } else {
            result = lbl_eu_806665C0;
        }
        *out2 = result;
    }
    *out1 = 0x12C;
}

// Same as func_80092FE4 with threshold 60 and *out1 = 0x258.
void func_8009319C(cf::CfObjectMove* self, u32* out1, f32* out2) {
    if (ml::math::mtRand(100) >= 0x3C) {
        *out2 = lbl_eu_806665E4;
    } else {
        f32 result;
        if (*self->field_0x34->field_0x28->_v138() != lbl_eu_806665C0) {
            cf::CNpcMoveSubView* sub = self->field_0x34->field_0x28;
            f32 denom = *sub->_v138();
            result = *sub->_v1D8() / denom;
        } else {
            result = lbl_eu_806665C0;
        }
        *out2 = result;
    }
    *out1 = 0x258;
}

// Same as func_80092FE4 with threshold 50 and *out1 = 0x384.
void func_80093278(cf::CfObjectMove* self, u32* out1, f32* out2) {
    if (ml::math::mtRand(100) >= 0x32) {
        *out2 = lbl_eu_806665E4;
    } else {
        f32 result;
        if (*self->field_0x34->field_0x28->_v138() != lbl_eu_806665C0) {
            cf::CNpcMoveSubView* sub = self->field_0x34->field_0x28;
            f32 denom = *sub->_v138();
            result = *sub->_v1D8() / denom;
        } else {
            result = lbl_eu_806665C0;
        }
        *out2 = result;
    }
    *out1 = 0x384;
}

// Rand-gated movement-rate setter (retail func_80093354): when mtRand(100) is
// at/above 0x50, writes the movement-rate divisor into *out2, *out1 = 0x3C and
// raises the +0x80 flag; otherwise the same rate with *out1 = 0x12C. The rate
// is +0x1D8 over +0x138 (0.0f fallback when the divisor reads 0).
void func_80093354(cf::CfObjectMove* self, u32* out1, f32* out2) {
    if (ml::math::mtRand(100) >= 0x50) {
        f32 result;
        f32 div = *self->field_0x34->field_0x28->_v138();
        f32 zero = lbl_eu_806665C0;
        if (div != zero) {
            cf::CNpcMoveSubView* sub = self->field_0x34->field_0x28;
            f32 denom = *sub->_v138();
            result = *sub->_v1D8() / denom;
        } else {
            result = lbl_eu_806665C0;
        }
        *out2 = result;
        *out1 = 0x3C;
        self->field_0x17C |= 0x80;
    } else {
        f32 result;
        f32 div = *self->field_0x34->field_0x28->_v138();
        f32 zero = lbl_eu_806665C0;
        if (div != zero) {
            cf::CNpcMoveSubView* sub = self->field_0x34->field_0x28;
            f32 denom = *sub->_v138();
            result = *sub->_v1D8() / denom;
        } else {
            result = lbl_eu_806665C0;
        }
        *out2 = result;
        *out1 = 0x12C;
    }
}

// Three-way rand-gated movement-rate setter (retail func_800934AC): mtRand(100)
// at/above 0x5A behaves like func_80093354's high path (*out1 = 0x3C + flag);
// in [0x3C, 0x5A) writes the 1.0f fallback; below 0x3C writes the rate with
// *out1 = 0x12C.
void func_800934AC(cf::CfObjectMove* self, u32* out1, f32* out2) {
    int r = ml::math::mtRand(100);
    if (r >= 0x5A) {
        f32 result;
        f32 div = *self->field_0x34->field_0x28->_v138();
        f32 zero = lbl_eu_806665C0;
        if (div != zero) {
            cf::CNpcMoveSubView* sub = self->field_0x34->field_0x28;
            f32 denom = *sub->_v138();
            result = *sub->_v1D8() / denom;
        } else {
            result = lbl_eu_806665C0;
        }
        *out2 = result;
        *out1 = 0x3C;
        self->field_0x17C |= 0x80;
    } else {
        if (r >= 0x3C) {
            *out2 = lbl_eu_806665E4;
        } else {
            f32 result;
            f32 div = *self->field_0x34->field_0x28->_v138();
            f32 zero = lbl_eu_806665C0;
            if (div != zero) {
                cf::CNpcMoveSubView* sub = self->field_0x34->field_0x28;
                f32 denom = *sub->_v138();
                result = *sub->_v1D8() / denom;
            } else {
                result = lbl_eu_806665C0;
            }
            *out2 = result;
        }
        *out1 = 0x12C;
    }
}

// Stores a heading value, forwards it to the movement sub-object's vtable
// +0xC4 slot, and raises the battle-state event while the +0x74 slot reports
// active.
void func_80093618(cf::CNpcBaseDataView* self, f32 f) {
    self->field_0xC = f;
    cf::CNpcMoveSubView* sub = self->field_0x28;
    if (sub) {
        sub->_vC4();
        if (self->field_0x28->_v74() != 0) {
            func_800BE12C(self->field_0x28, 3, 0, -1, 1);
        }
    }
}
