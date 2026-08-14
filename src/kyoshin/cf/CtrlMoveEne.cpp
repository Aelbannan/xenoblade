// Auto-scaffolded catalog TU for kyoshin/cf/CtrlMoveEne
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CtrlMoveEne.hpp"
#include "kyoshin/cf/CtrlNpc.hpp"
#include "monolib/math/Random.hpp"

// In-TU forward declarations (definitions below).
void func_8008D444(cf::CfObjectMove* self, cf::CFunc8008D444Obj* obj, int flag);
void func_8008BEEC(cf::CfObjectMove* self, void* obj, int arg2, u32 arg3);

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

void func_8008A2C8(){}

void* CActorParam_UnkVirtualFunc132__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x3298); }

void func_8008B580(cf::CfObjectMove* self) {
    cf::CfObjectMoveView9* v = (cf::CfObjectMoveView9*)self;
    func_800895A8(self);
    self->field_0x17C &= 0x7F613E7Fu;
    if ((self->field_0x17C & 0x1000u) != 0) return;
    if ((self->field_0x17C & 0x08000000u) == 0) {
        // Face the +0xCC heading: velocity = {sin, 0, cos} of the scaled index.
        f32 cz = CosFIdx__Q24nw4r4mathFf(
            lbl_eu_806665CC * self->field_0x34->field_0x28->_vCC());
        v->mVelocity.x = SinFIdx__Q24nw4r4mathFf(
            lbl_eu_806665CC * self->field_0x34->field_0x28->_vCC());
        v->mVelocity.y = lbl_eu_806665C0;
        v->mVelocity.z = cz;
    }
    s16 t = (s16)(self->field_0x198 + 1);
    self->field_0x198 = t;
    self->field_0x17C |= 0x08000000u;
    if (t > 300) {
        self->field_0x198 = 300;
    }
    cf::CNpcMoveSubView* sub2 = self->field_0x34->field_0x28;
    cf::CFunc8008B580Obj* obj =
        (cf::CFunc8008B580Obj*)(sub2 ? (u8*)sub2 - 0x3E9C : 0);
    cf::CfObj3F60View* f60 = obj->field_3F60;
    if (obj->_v2BC() != 0 && obj->_v128() <= lbl_eu_806665C0) {
        if (f60 != 0 && (self->field_0x180 & 0x8u) != 0) {
            self->field_0x17C &= ~0x8000u;
            func_8004B8B0(f60, 0, 1, lbl_eu_806665C0);
            ((cf::CFunc8008B580Sub150*)&obj->mSub)->_v150(0);
            return;
        }
    }
    if (f60 != 0) {
        func_8008BEEC(self, obj, 1, 0);
        if ((self->field_0x180 & 0x1C000000u) != 0) {
            if ((self->field_0x180 & 0x8u) != 0) {
                self->field_0x17C &= ~0x8000u;
                u32 w1 = obj->field_04->_v30()->field_0;
                if (func_80174C98(obj, &w1, 0x1C) != 0) {
                    func_8004B8B0(f60, 0, 1, lbl_eu_806665C0);
                    ((cf::CFunc8008B580Sub150*)&obj->mSub)->_v150(0);
                } else {
                    u32 w2 = obj->field_04->_v30()->field_0;
                    u32 w3;
                    int ok = func_80174C98(obj, &w2, 0x805) != 0;
                    if (!ok) {
                        w3 = obj->field_04->_v30()->field_0;
                        ok = func_80174C98(obj, &w3, 0x18) != 0;
                    }
                    if (ok) {
                        func_8004B8B0(f60, 0, 1, lbl_eu_806665C0);
                        ((cf::CFunc8008B580Sub150*)&obj->mSub)->_v150(0);
                    }
                }
            }
            if ((self->field_0x180 & 0x10u) != 0) {
                f60->field_C |= 0x2000;
            } else if ((self->field_0x180 & 0x20u) != 0) {
                f60->field_C |= 0x4000;
            }
        }
        if ((f60->field_4EC & 0x80u) != 0) {
            if ((f60->field_4EC & 0x40000u) != 0) {
                f32 hp = obj->_v128();
                obj->_v11C(-hp);
            }
        } else if ((f60->field_4EC & 0x100000u) != 0) {
            self->field_0x17C |= 0x40000000u;
            self->field_0x178 = lbl_eu_806665C0;
            if (((cf::CFunc8008B580F60*)f60)->field_532 >= 100) {
                f32 hp = obj->_v128();
                obj->_v11C(-hp);
            }
        }
        obj->field_4550 &= ~0xC00000u;
        self->field_0x3C = lbl_eu_806665F0;
    }
}

struct CCtrlMoveNpc;
extern "C" void func_8008962C(CCtrlMoveNpc* self);
extern "C" void func_8008B930(CCtrlMoveNpc* self) { func_8008962C(self); }

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

void func_8008BEEC(cf::CfObjectMove* self, void* obj, int arg2, u32 arg3) {}

// Enemy movement-controller per-frame update (retail func_8008C4F0): recovers
// the battle object from the move data's +0x28 sub-object, stores the +0x3C
// target constant, dispatches func_8008D444 while the +0x198 timer is at/over
// 90 with the +0x08000000 flag set and the +0x1000 / +0x2 bits clear, clears
// the +0x40000000/+0x10000000 flag bits, forwards the +0x1000 bit to
// func_8008BEEC and mirrors the +0x180 flag bits into the +0x3F60 sub-object.
// noinline keeps func_8008A23C's call site an opaque bl (retail has one).
__declspec(noinline) void func_8008C4F0(cf::CfObjectMove* self) {
    cf::CNpcMoveSubView* sub = self->field_0x34->field_0x28;
    cf::CFunc8008D444Obj* obj =
        (cf::CFunc8008D444Obj*)(sub ? (u8*)sub - 0x3E9C : 0);
    u32 flags = self->field_0x17C;
    self->field_0x3C = lbl_eu_806665F0;
    if ((flags & 0x08000000u) != 0 && (flags & 0x1000u) == 0) {
        if (self->field_0x198 >= 90 && (self->field_0x180 & 2u) == 0) {
            func_8008D444(self, obj, 0);
        }
    }
    flags &= 0xA3FFFFFFu;
    self->field_0x17C = flags;
    self->field_0x198 = 0;
    self->field_0x190 = 0;
    cf::CfObj3F60View* f60 = obj->field_3F60;
    if (f60 != 0) {
        func_8008BEEC(self, obj, 0, (flags >> 12) & 1);
        if ((self->field_0x180 & 0x1C000000u) != 0) {
            if ((self->field_0x180 & 0x10000000u) != 0) {
                if ((self->field_0x17C & 0x10000u) == 0) {
                    func_8004B8B0(f60, 1, 1, self->field_0x160);
                    func_800899AC(self, lbl_eu_806665E0);
                } else {
                    func_8004B8B0(f60, 0, 1, lbl_eu_806665C0);
                }
            }
            if ((self->field_0x180 & 0x08000000u) != 0) {
                f60->field_C |= 0x2000;
            } else if ((self->field_0x180 & 0x04000000u) != 0) {
                f60->field_C |= 0x4000;
            }
        }
        if ((self->field_0x180 & 0x02000000u) != 0) {
            f60->field_4EC |= 4;
        }
    }
}

void func_8008C660() {}

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
                func_8004B8B0(f60, 1, 0, view->field_0x148);
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
                func_8004B8B0(f60, 1, 1, view->field_0x148);
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
    cf::CfObjectMoveInitView* view = (cf::CfObjectMoveInitView*)self;
    cf::CNpcMoveSubView* sub = self->field_0x34->field_0x28;
    cf::CFunc8008D444Obj* obj =
        (cf::CFunc8008D444Obj*)(sub ? (u8*)sub - 0x3E9C : 0);
    func_80089684(self);
    self->mMoveHook =
        ((const CfMoveHookPtmfsE2D4*)&lbl_eu_80527830)->hook13C;
    union {
        u32 w;
        u16 h;
    } match;
    match.w = func_800AF7E4(obj, lbl_eu_804FB9E8 + 0x13);
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
    row.w = func_800AF7E4(obj, lbl_eu_804FB9E8);
    u8 state = row.b;
    if (state >= 0xF) {
        state = 0;
    }
    view->field_0x5C = state;
    view->field_0x60 = 0;
    view->field_0x72 = 0;
    f32 rate = lbl_eu_806665C0;
    if (*self->field_0x34->field_0x28->_v138() != lbl_eu_806665C0) {
        cf::CNpcMoveSubView* sub2 = self->field_0x34->field_0x28;
        f32 denom = *sub2->_v138();
        rate = *sub2->_v1D8() / denom;
    }
    view->field_0x60 = (s32)rate;
    int count = func_80086B44__Q22cf13CfGameManagerFv();
    int idx = 0;
    for (int i = 0; i < count; i++) {
        union {
            u32 w;
            u16 h;
        } c13;
        c13.w = func_800AF82C(obj, lbl_eu_804FB9E8 + 0x13, i);
        if (c13.h == matchId && idx < 8) {
            union {
                u32 w;
                u16 h;
            } c1b;
            c1b.w = func_800AF82C(obj, lbl_eu_804FB9E8 + 0x1b, i);
            u16 flags = c1b.h;
            if ((flags & 0x200u) != 0) {
                // Flag row: direct target fields instead of a move-list entry.
                union {
                    u32 w;
                    s16 h;
                } c22;
                c22.w = func_800AF82C(obj, lbl_eu_804FB9E8 + 0x22, i);
                view->field_0x4C = (f32)c22.h;
                union {
                    u32 w;
                    s16 h;
                } c27;
                c27.w = func_800AF82C(obj, lbl_eu_804FB9E8 + 0x27, i);
                view->field_0x50 = (f32)c27.h;
                union {
                    u32 w;
                    s16 h;
                } c2c;
                c2c.w = func_800AF82C(obj, lbl_eu_804FB9E8 + 0x2c, i);
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
                cf::CfMoveSubEntry* entry = &view->field_0x78[idx];
                entry->field_0xC = flags;
                union {
                    u32 w;
                    s16 h;
                } c22;
                c22.w = func_800AF82C(obj, lbl_eu_804FB9E8 + 0x22, i);
                entry->field_0x0 = (f32)c22.h;
                union {
                    u32 w;
                    s16 h;
                } c27;
                c27.w = func_800AF82C(obj, lbl_eu_804FB9E8 + 0x27, i);
                entry->field_0x4 = (f32)c27.h;
                union {
                    u32 w;
                    s16 h;
                } c2c;
                c2c.w = func_800AF82C(obj, lbl_eu_804FB9E8 + 0x2c, i);
                entry->field_0x8 = (f32)c2c.h;
                union {
                    u32 w;
                    s16 h;
                } c31;
                c31.w = func_800AF82C(obj, lbl_eu_804FB9E8 + 0x31, i);
                entry->field_0x10 = (f32)c31.h;
                union {
                    u32 w;
                    u8 b;
                } c3b;
                c3b.w = func_800AF82C(obj, lbl_eu_804FB9E8 + 0x3b, i);
                entry->field_0xE = c3b.b;
                union {
                    u32 w;
                    u8 b;
                } c45;
                c45.w = func_800AF82C(obj, lbl_eu_804FB9E8 + 0x45, i);
                entry->field_0xF = c45.b;
                union {
                    u32 w;
                    u8 b;
                } c4e;
                c4e.w = func_800AF82C(obj, lbl_eu_804FB9E8 + 0x4e, i);
                entry->field_0x10 = (f32)c4e.b;
                idx++;
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
        self->mMoveHook =
            ((const CfMoveHookPtmfsE2D4*)&lbl_eu_80527830)->hook148;
        view->field_0x4C = self->field_0x34->field_0x28->_vCC();
        self->field_0x180 |= 0x800;
    } else {
        // Dispatch the state record and commit the entry count / scaled timer.
        (self->*((const CfMoveDispatchPtmf*)&lbl_eu_80527830)
                    [view->field_0x5C])(&view->field_0x60, &view->field_0x6C);
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
    // whole function; member access emits the addi + base-load ptmf copy).
    const CfMoveHookPtmfsE760& hooks =
        *(const CfMoveHookPtmfsE760*)&lbl_eu_80527830;
    cf::CFunc8008E760View* view = (cf::CFunc8008E760View*)self;
    cf::CNpcMoveSubView* sub = self->field_0x34->field_0x28;
    cf::CFunc8008E760Obj* obj =
        (cf::CFunc8008E760Obj*)(sub ? (u8*)sub - 0x3E9C : 0);
    int r28;
    if ((obj->field_3F00 & 0x04000000u) != 0) {
        r28 = 1;
    } else {
        r28 = (func_800B8AFC(obj) == 0) ? 1 : 0;
    }
    view->field_0x17C = 0;
    view->field_0x18C = 0;
    if ((obj->field_3374 & 0x800u) != 0) {
        view->field_0x17C = 0x40;
    }
    union {
        u32 w;
        u8 b;
    } row53;
    row53.w = func_800AF7E4(obj, lbl_eu_804FB9E8 + 0x53);
    if (row53.b == 0) {
        self->field_0x180 |= 1;
    }
    if (r28 != 0) {
        // Battle state active: pick the hook / init path from the flags.
        if ((view->field_0x17C & 0x40u) != 0) {
            func_80089684(self);
            self->mMoveHook = hooks.hook154;
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
                self->mMoveHook = hooks.hook160;
                self->field_0x4C = self->field_0x34->field_0x28->_vCC();
                self->field_0x180 |= 0x800;
            }
        }
    } else {
        // Inactive: seed the counters and run the party-position probe.
        func_80089684(self);
        self->mMoveHook = hooks.hook16C;
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
        cf::CFunc8008EF04Sub* sub =
            (cf::CFunc8008EF04Sub*)self->field_0x34->field_0x28;
        if (sub->field_C4 != 0) {
            func_8004B8B0(sub->field_C4, 0, 1, lbl_eu_806665C0);
            sub->_v150(1);
        }
    }
    // Clear the +1 state flag when the target is gone / the +0x98 talk state
    // has its +0x8000 bit; a live +0x98 state makes the sub's +0xC4 slot take
    // over instead (returns nonzero -> skip the heading logic below).
    int done = 0;
    if ((self->field_0x17C & 1u) != 0) {
        cf::CFunc8008EF04Sub* sub =
            (cf::CFunc8008EF04Sub*)self->field_0x34->field_0x28;
        cf::CFunc8008EF04Sub98* p98 = (cf::CFunc8008EF04Sub98*)sub->field_98;
        if (p98 != 0 && (p98->field_7A4 & 0x8000u) != 0) {
            if (sub->field_C4 != 0) {
                func_8004B9D4(sub->field_C4, 1, 0, -1, 0);
            }
            self->field_0x17C &= ~1u;
        } else if (sub->field_C4 != 0 && sub->_v0C(4) != 0) {
            ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
                lbl_eu_806665C0;
            done = 1;
        } else {
            self->field_0x17C &= ~1u;
        }
    }
    if (done == 0) {
        // Heading logic: compare the sub's heading with the +0x4C target
        // heading through the cos of their difference.
        f32 h = self->field_0x34->field_0x28->_vCC();
        f32 cosA = CosFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * h);
        f32 cosB = CosFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * self->field_0x4C);
        f32 h2 = self->field_0x34->field_0x28->_vCC();
        f32 sinA = SinFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * h2);
        f32 sinB = SinFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * self->field_0x4C);
        if (sinA * sinB + cosA * cosB <= lbl_eu_806665DC) {
            // Facing the target: turn toward +0x4C and engage the +0x98 state
            // unless its +0x8000 bit is set.
            cf::CFunc8008EF04Sub* sub =
                (cf::CFunc8008EF04Sub*)self->field_0x34->field_0x28;
            sub->_vC4(self->field_0x4C);
            cf::CFunc8008EF04Sub98* p98 = (cf::CFunc8008EF04Sub98*)sub->field_98;
            if (sub->field_C4 != 0 && p98 != 0 &&
                (p98->field_7A4 & 0x8000u) == 0) {
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

void func_8008F9EC(cf::CfObjectMove* self) {
    cf::CfObjectMoveView9* v = (cf::CfObjectMoveView9*)self;
    cf::CFunc8008F9ECSub* sub =
        (cf::CFunc8008F9ECSub*)self->field_0x34->field_0x28;
    if (sub->field_C4 == 0) return;
    if (sub->_v98() == 0) return;
    if ((self->field_0x17C & 0x20u) == 0) {
        // Not fleeing: refresh the movement-rate divisor, then either pick a
        // random direction (when the +0x5C-scaled probe succeeds) or idle.
        if (self->field_0x50 != 0) {
            v->field_0x58 = lbl_eu_806665E4;
        } else {
            f32 rate = lbl_eu_806665C0;
            if (*self->field_0x34->field_0x28->_v138() != lbl_eu_806665C0) {
                cf::CNpcMoveSubView* sub2 = self->field_0x34->field_0x28;
                f32 denom = *sub2->_v138();
                rate = *sub2->_v1D8() / denom;
            }
            v->field_0x58 = rate;
        }
        ml::CVec3 out;
        if (func_8008B9C0(self, &out, self, v->field_0x5C,
                          lbl_eu_806665C0, 0) != 0) {
            f32 r = (f32)ml::math::mtRand(10);
            v->field_0x60 =
                v->field_0x5C * (lbl_eu_80666668 * r + lbl_eu_80666610);
            ml::CVec3 dir;
            dir.x = SinFIdx__Q24nw4r4mathFf(lbl_eu_806665CC *
                                            self->field_0x34->field_0x28->_vCC());
            dir.y = lbl_eu_806665C0;
            dir.z = CosFIdx__Q24nw4r4mathFf(lbl_eu_806665CC *
                                            self->field_0x34->field_0x28->_vCC());
            func_80089694(self, &dir, v->field_0x58);
        } else {
            ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
                lbl_eu_806665C0;
        }
        self->field_0x184 = 0;
        self->field_0x54 = 0;
        ((cf::CfObjectMove4CView*)self)->field_4C &= ~3;
        return;
    }
    // Flee path: head away from the +0x4C target while it is close, else
    // latch its position into mVec138.
    ml::CVec3 dir(lbl_eu_806665C0, lbl_eu_806665C0, lbl_eu_806665C0);
    int want = 1;
    if (((cf::CfObjectMove4CView*)self)->field_4C & 1u) {
        ml::CVec3* pos = ((cf::CFunc8008F9ECSub*)self->field_0x34->field_0x28)
                              ->_vAC();
        f32 dx = pos->x - v->mPosition.x;
        f32 dz = pos->z - v->mPosition.z;
        f32 ang =
            lbl_eu_80666638 * Atan2FIdx__Q24nw4r4mathFff(dx, dz);
        if (((cf::CfObjectMove4CView*)self)->field_4C & 2u) {
            ang += lbl_eu_8066A204;
        }
        dir.x = SinFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * ang);
        dir.y = lbl_eu_806665C0;
        dir.z = CosFIdx__Q24nw4r4mathFf(lbl_eu_806665CC * ang);
    }
    func_80089694(self, &dir, v->field_0x58);
    if (want != 0) {
        ml::CVec3* pos = ((cf::CFunc8008F9ECSub*)self->field_0x34->field_0x28)
                              ->_vAC();
        ml::CVec3 diff;
        nw4r::math::VEC3Sub(diff, self->mVec138, *pos);
        if (diff.x * diff.x + diff.z * diff.z <= lbl_eu_806665E4) {
            self->field_0x184 = self->field_0x184 + 1;
            if (self->field_0x184 > 60) {
                self->field_0x184 = 60;
                want = 0;
            }
        } else {
            self->mVec138 = *pos;
            self->field_0x184 = 0;
        }
        if (want != 0) return;
    }
    self->field_0x17C &= ~0x60020u;
    self->field_0x186 = 0;
    self->field_0x184 = 0;
}

void func_8008FE8C(){}

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

    cf::CfMoveSubEntry* entry =
        &view->field_0x78[(s16)view->field_0x5A];
    if (view->field_0x72 == 0) {
        // Gate clear: install the +0x1B4 hook, dispatch the state record and
        // step the +0x5A row timer.
        self->mMoveHook = hooks.hook1B4;
        view->field_0x70 &= 0xFFFB;
        if ((entry->field_0xC & 4u) != 0) {
            view->field_0x70 |= 1;
        }
        (self->*((const CfMoveDispatchPtmf*)&lbl_eu_80527830)
             [view->field_0x5C])(&view->field_0x60, &view->field_0x6C);
        view->field_0x60 = entry->field_0xE * 30;
        if ((view->field_0x70 & 1u) != 0) {
            s16 t = (s16)(view->field_0x5A - 1);
            view->field_0x5A = (s16)t;
            if (t < 0) {
                view->field_0x5A = 1;
                view->field_0x70 &= 0xFFFE;
            }
        } else {
            s16 t = (s16)(view->field_0x5A + 1);
            view->field_0x5A = (s16)t;
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

    // Gate set: step it and run the approach / pursuit logic.
    view->field_0x72 = view->field_0x72 - 1;
    u32 flags = view->field_0x17C;
    cf::CFunc80090DB4Sub* sub =
        (cf::CFunc80090DB4Sub*)self->field_0x34->field_0x28;
    if ((flags & 0x20u) == 0) {
        // Approach a candidate point around the entry (func_8008B9C0).
        u32 t = view->field_0x60;
        view->field_0x60 = t - 1;
        if (t > 0) return;
        view->field_0x60 = 0;
        ml::CVec3 dir44;
        if (func_8008B9C0(self, &dir44, (cf::CfObjectMove*)entry,
                          entry->field_0x10, lbl_eu_806665C0, 0) != 0) {
            if ((self->field_0x180 & 0x8u) == 0 &&
                func_80089E88(self, &view->mPosition, 1) == 0) {
                view->field_0x60 = 12;
                view->field_0x17C |= 0x100000;
            } else {
                // Commit the direction to the sub heading and the velocity.
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
                f32 ang =
                    Atan2FIdx__Q24nw4r4mathFff(dir.x, dir.z);
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
        ml::CVec3 dir38;
        func_80088974(self, &dir38, &view->mPosition, 0, 0);
        u32 t = view->field_0x60;
        view->field_0x60 = t - 1;
        if (t == 0) {
            view->field_0x60 = 0;
            cf::CFunc80090DB4Sub* sub2 =
                (cf::CFunc80090DB4Sub*)self->field_0x34->field_0x28;
            f32 ang = Atan2FIdx__Q24nw4r4mathFff(dir38.x, dir38.z);
            sub2->vtable->fn_0xC4(sub2, lbl_eu_80666638 * ang);
            if (sub2->field_C4 != 0 && sub2->field_98 != 0 &&
                (((cf::CFunc8008EF04Sub98*)sub2->field_98)->field_7A4 &
                 0x10000u) == 0) {
                func_800BE12C(sub2, 3, 0, -1, 1);
                sub2->vtable->fn_0x10(sub2, 4);
                view->field_0x17C |= 1;
            }
            *(u32*)&view->mVelocity.x = *(u32*)&dir38.x;
            *(u32*)&view->mVelocity.y = *(u32*)&dir38.y;
            *(u32*)&view->mVelocity.z = *(u32*)&dir38.z;
            view->field_0x17C &= ~0x100000u;
        }
        ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
            lbl_eu_806665C0;
        return;
    }

    int r0 = 0;
    if ((flags & 1u) != 0) {
        // Talk-state takeover: dismiss the +0xC4 icon or hand control to the
        // +0x98 state, else clear the +1 flag.
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

    // Close pursuit: keep the sub within reach of the controller.
    ml::CVec3* p = sub->vtable->fn_0xAC(sub);
    f32 dx = p->x - view->mPosition.x;
    f32 dz = p->z - view->mPosition.z;
    if (dx * dx + dz * dz > lbl_eu_806665E4) {
        view->field_0x186 = view->field_0x186 + 1;
        if (view->field_0x186 < 0x258) {
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
            if (r0 == 0) {
                func_80089694(self, &z, view->field_0x6C);
                return;
            }
        }
    }
    // Dispatch the state record and finalize the move-list fields.
    (self->*((const CfMoveDispatchPtmf*)&lbl_eu_80527830)
         [view->field_0x5C])(&view->field_0x60, &view->field_0x6C);
    view->field_0x184 = 0;
    view->field_0x186 = 0;
    view->field_0x60 = entry->field_0xE * 30;
    view->field_0x17C &= 0xFFE9FFDF;
    ((cf::CFunc8009DataView*)self->field_0x34)->field_0x14 =
        lbl_eu_806665C0;
}


// Enemy pursuit scan (retail func_8009156C): while the +0x8000 flag is set,
// either leap toward the move sub's position (when far) or, once close, run a
// two-tick gate before scanning the gimmick list for nearby targets to mark.
void func_8009156C(cf::CfObjectMove* self) {
    cf::CfObjectMoveView9* v = (cf::CfObjectMoveView9*)self;
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
            cf::CFunc8008F9ECSub* sub =
                (cf::CFunc8008F9ECSub*)self->field_0x34->field_0x28;
            cf::CFunc8009156CList* list = func_800B6BC8();
            for (cf::CFunc8009156CNode* node = list->head->next;
                 node != list->head; node = node->next) {
                cf::CFunc8009156CGimmick* gimmick = node->object;
                ml::CVec3* gp = gimmick->_vAC();
                ml::CVec3 diff;
                nw4r::math::VEC3Sub(diff, *sub->_vAC(), *gp);
                if (diff.x * diff.x + diff.z * diff.z <= lbl_eu_80666678) {
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

void func_80091864(){}

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
    if (*self->field_0x34->field_0x28->_v138() == lbl_eu_806665C0) {
    } else {
        cf::CNpcMoveSubView* sub = self->field_0x34->field_0x28;
        f32 denom = *sub->_v138();
        result = *sub->_v1D8() / denom;
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

extern "C" void func_80092FA8(void* u, u32* a, float* b) {
    *a = 0x12C;
    *b = lbl_eu_806665E4;
}

extern "C" void func_80092FBC(void* u, u32* a, float* b) {
    *a = 0x258;
    *b = lbl_eu_806665E4;
}

extern "C" void func_80092FD0(void* u, u32* a, float* b) {
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
        f32 rate = *self->field_0x34->field_0x28->_v138();
        if (lbl_eu_806665C0 != rate) {
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
        if (*self->field_0x34->field_0x28->_v138() == lbl_eu_806665C0) {
        } else {
            cf::CNpcMoveSubView* sub = self->field_0x34->field_0x28;
            f32 denom = *sub->_v138();
            result = *sub->_v1D8() / denom;
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
        if (*self->field_0x34->field_0x28->_v138() == lbl_eu_806665C0) {
        } else {
            cf::CNpcMoveSubView* sub = self->field_0x34->field_0x28;
            f32 denom = *sub->_v138();
            result = *sub->_v1D8() / denom;
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
        if (*self->field_0x34->field_0x28->_v138() == lbl_eu_806665C0) {
        } else {
            cf::CNpcMoveSubView* sub = self->field_0x34->field_0x28;
            f32 denom = *sub->_v138();
            result = *sub->_v1D8() / denom;
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
        if (*self->field_0x34->field_0x28->_v138() == lbl_eu_806665C0) {
        } else {
            cf::CNpcMoveSubView* sub = self->field_0x34->field_0x28;
            f32 denom = *sub->_v138();
            result = *sub->_v1D8() / denom;
        }
        *out2 = result;
        *out1 = 0x3C;
        self->field_0x17C |= 0x80;
    } else {
        f32 result;
        if (*self->field_0x34->field_0x28->_v138() == lbl_eu_806665C0) {
        } else {
            cf::CNpcMoveSubView* sub = self->field_0x34->field_0x28;
            f32 denom = *sub->_v138();
            result = *sub->_v1D8() / denom;
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
        if (*self->field_0x34->field_0x28->_v138() == lbl_eu_806665C0) {
        } else {
            cf::CNpcMoveSubView* sub = self->field_0x34->field_0x28;
            f32 denom = *sub->_v138();
            result = *sub->_v1D8() / denom;
        }
        *out2 = result;
        *out1 = 0x3C;
        self->field_0x17C |= 0x80;
    } else if (r >= 0x3C) {
        *out2 = lbl_eu_806665E4;
        *out1 = 0x12C;
    } else {
        f32 result;
        if (*self->field_0x34->field_0x28->_v138() == lbl_eu_806665C0) {
        } else {
            cf::CNpcMoveSubView* sub = self->field_0x34->field_0x28;
            f32 denom = *sub->_v138();
            result = *sub->_v1D8() / denom;
        }
        *out2 = result;
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
