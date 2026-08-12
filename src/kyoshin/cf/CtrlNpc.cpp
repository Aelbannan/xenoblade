// Auto-scaffolded catalog TU for kyoshin/cf/CtrlNpc
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include <monolib/math/CVec3.hpp>
#include <monolib/math/Random.hpp>
#include <nw4r/math/math_arithmetic.h>
#include <nw4r/math/math_triangular.h>

#include "kyoshin/cf/CtrlNpc.hpp"

// Retail ctor is a free function (MWCC_REFERENCE §4193): stores the base
// vtable, initializes the base fields, creates the character object, then
// stores the final vtable and constructs the CCtrlMoveNpc sub-object at +0x30.
cf::CtrlNpc* __ct__CtrlNpc(cf::CtrlNpc* self, cf::CfObject* param) {
    self->field_04 = 0;
    self->field_08 = lbl_eu_80666698;
    self->field_0C = lbl_eu_80666698;
    self->field_10 = lbl_eu_80666698;
    self->field_14 = lbl_eu_80666698;
    self->field_18 = 0;
    self->field_1C = 2;
    self->field_24 = -1;
    *(void**)self = (void*)lbl_eu_80527BB0;   // base vtable (retail position)
    self->field_28 = (cf::CCtrlNpcChar*)func_800BBC0C(param);
    self->field_2C = 0;
    *(void**)self = (void*)lbl_eu_80527B38;   // final vtable
    __ct__cf_CtrlMoveNpc((cf::CCtrlMoveNpc*)self->_sub30, self);
    self->field_AC.f = lbl_eu_80666698;
    self->field_B0.f = lbl_eu_80666698;
    self->field_B4.f = lbl_eu_80666698;
    self->field_B8 = 0;
    self->field_BC = 0;
    self->field_BE = 0;
    self->field_C0 = 0;
    self->field_C2 = 0;
    self->field_C3 = 0;
    self->field_C4 = 0;
    self->field_D4 = lbl_eu_8066669C;
    self->field_DC = 0;
    self->field_DE = 0x1e;
    self->field_172 = 0;
    self->field_174 = 0;
    self->field_178 = lbl_eu_80666698;
    return self;
}

// 0x80094154: set the action id. When switching away from a movement action
// (4/5), report the current target/position offset to the character object
// and (on success) bump the page counter / set the done flags.
void func_8009377C(cf::CtrlNpc* self, u32 param) {
    self->field_28->_v40();
    u16 c0 = self->field_C0;
    self->field_DC = 0;
    if (param != c0) {
        if (c0 == 4 || c0 == 5) {
            const ml::CVec3* posf =
                reinterpret_cast<const ml::CVec3*>(self->field_28->_vAC());
            const ml::CVec3* tgt =
                reinterpret_cast<const ml::CVec3*>(&self->field_AC);
            ml::CVec3 diff;
            diff.x = tgt->x - posf->x;
            diff.y = tgt->y - posf->y;
            diff.z = tgt->z - posf->z;
            ml::CVec3 v = diff;
            if (self->field_28->_v160(&diff) != 0 &&
                (lbl_eu_80663E28 & 0x04000000) == 0) {
                f32 len2 = v.x * v.x + v.y * v.y + v.z * v.z;
                if (len2 < lbl_eu_806666A0) {
                    self->field_DC += 1;
                    if (self->field_C0 == 4) {
                        self->field_28->_v1DC(0);
                        self->field_2C |= 0x10;
                    } else if (self->field_C0 == 5) {
                        self->field_2C |= 0x8;
                    }
                }
            }
        }
    }
    if (param <= 3) {
        func_800BE12C(self->field_28, 1, 1, -1, 1);
    }
    self->field_174 = 0;
    self->field_172 = 0;
    self->field_C0 = (u16)param;
    self->field_14 = lbl_eu_80666698;
    self->field_BE = 0;
}

void CfObjectMove_UnkVirtualFunc6__Q22cf12CfObjectMoveFv(void* self, unsigned long val) {
    void* ptr = *(void**)((char*)self + 0xc4);
    if (ptr != 0)
        *(unsigned long*)((char*)ptr + 0x37c) = val;
}

// 0x80094310: clear field_04, then run the char-object virtuals at vtable
// slots +0x58/+0x5C.
void func_80093938(cf::CCtrlNpcChar* self) {
    self->field_04 = 0;
    self->_v58();
    self->_v5C();
}

void func_8009398C(){}

void func_80093F28(){}

// 0x80095274: cf::CObjectState::CObjectState_UnkVirtualFunc5. The retail
// symbol keeps the Fv suffix, but the virtual is invoked with an int (see
// object/CObjectState.hpp and CfObjectNpc.cpp callers), so the body is emitted
// under the literal retail name with the caller's real arg shape.
extern "C" void CObjectState_UnkVirtualFunc5__Q22cf12CObjectStateFv(cf::CObjectState* self, int arg) {
    if ((u32)arg < 0x3f)
        self->CObjectState_UnkVirtualFunc7(0x3f);
    self->unk8 |= (u32)arg;
}

// 0x800952D0: record the movement range (field_160/164) and duration, then
// copy `count` 12-byte elements from src into the +0xE0 target array. Bulk
// copies run in 8-element (96-byte) groups.
struct NpcTargetRow {
    u32 w[24];   // 96 bytes = 8 x 12-byte elements
};

void func_800948F8(cf::CtrlNpc* self, u32 a, u32 b, int count,
                   const u32* src, f32 f) {
    self->field_160 = a;
    self->field_168 = (s16)((b - a) * 60);
    self->field_164 = b;
    self->field_BE = 0;
    self->field_C0 = 3;
    self->field_C4 = 1;
    self->field_158 = f;
    self->field_15C = (s16)count;
    if (count <= 0)
        return;
    int i = 0;
    int n = count - 8;
    if (count > 8) {
        int ok = 0;
        if (count < 0)
            goto ok_test;
        if (count > 0x7FFFFFFE)
            goto ok_test;
        ok = 1;
    ok_test:
        if (ok == 0)
            goto tail;
        u32* d = &self->field_E0;
        const u32* s = src;
        for (; i < n; i += 8) {
            *(NpcTargetRow*)d = *(const NpcTargetRow*)s;
            d += 24;
            s += 24;
        }
    }
tail:
    u32* d2 = &self->field_E0 + i * 3;
    const u32* s2 = src + i * 3;
    for (; i < count; i++) {
        *(ml::CVec3*)d2 = *(const ml::CVec3*)s2;
        d2 += 3;
        s2 += 3;
    }
}

// Retail func_80094A9C is a 0x24C-byte body (not yet recovered); the stub is
// kept noinline so func_80094CE8 can tail-call it instead of inlining it.
extern "C" void __declspec(noinline) func_80094A9C(cf::CtrlNpc* self) {}

// 0x800956C0: store the action-setup fields, then dispatch: a zero action id
// arms the movement sub-object (CCtrlMoveNpc at +0x30), anything else runs
// the generic action setup.
void func_80094CE8(cf::CtrlNpc* self, const ml::CVec3* vec, int val, f32 scale, f32 paramB) {
    u16 v = self->field_C6;
    v = (u16)__rlwimi(v, (u32)val, 8, 16, 23);
    self->field_C0 = 6;
    self->field_C6 = v;
    self->field_158 = paramB;
    self->field_BE = 1;
    if (val != 0)
        func_80094A9C(self);
    else
        func_8019F6E8((cf::CCtrlMoveNpc*)self->_sub30, vec, scale, paramB);
}

// 0x800956F4: action-advance query - whether the NPC may advance from its
// current busy state (field_BE) given the action kind (field_C0).
int func_80094D1C(const cf::CtrlNpc* self) {
    int result = 0;
    u32 c0 = self->field_C0;
    if (c0 == 3) {
        if ((lbl_eu_80663E28 & 0x04000000) != 0) {
            if (self->field_BE == 2)
                result = 1;
        } else {
            if (self->field_BE == 2) {
                if (self->field_28->_v24(1) != 0 || self->field_28->_v0C(1) != 0)
                    result = 0;
                else
                    result = 1;
            }
        }
    } else if (c0 == 6) {
        if (self->field_BE == 4)
            result = 1;
    } else {
        result = 1;
    }
    return result;
}

// 0x800957CC: store the target position words and flags, then activate the
// character object if it is in one of the "in action" states (0x21..0x2A).
void func_80094DF4(cf::CtrlNpc* self, const cf::CtrlNpcVec3W* vec) {
    self->field_14 = lbl_eu_80666698;
    self->field_C8 = vec->x;
    self->field_CC = vec->y;
    self->field_D0 = vec->z;
    self->field_BE = 1;
    self->field_174 = 0;
    cf::CCtrlNpcChar* obj = self->field_28;
    if (obj->field_6C4 < 0x21)
        return;
    if (obj->field_6C4 > 0x2a)
        return;
    obj->field_6C4 = 1;
}

// 0x8009581C: reset the movement target to the character's current position
// and arm the action fields (kind 4) with the given id.
void func_80094E44(cf::CtrlNpc* self, u32 val) {
    self->field_14 = lbl_eu_80666698;
    self->field_BE = 0;
    self->field_C0 = 4;
    self->field_16C = val;
    self->field_16E = val;
    self->field_170 = val;
    cf::CtrlNpcVec3W* pos = self->field_28->_vAC();
    u32 x = pos->x;
    u32 y = pos->y;
    self->field_B0.u = y;
    self->field_AC.u = x;
    self->field_B4.u = pos->z;
    self->field_174 = 0;
    self->field_28->field_6C4 = val;
}

// 0x800958B4: reset the movement target to the character's current position
// and arm the action fields (kind 5); the 0x16E/0x170 ids are clamped into
// [0, 0x68) (out-of-range ids become 0).
void func_80094EDC(cf::CtrlNpc* self, int r4, int r5, int r6) {
    self->field_14 = lbl_eu_80666698;
    self->field_BE = 0;
    self->field_C0 = 5;
    self->field_16C = r4;
    int ok5 = (r5 >= 0) && !(r5 >= 0x68);
    self->field_16E = ok5 ? r5 : 0;
    int ok6 = (r6 >= 0) && !(r6 >= 0x68);
    self->field_170 = ok6 ? r6 : 0;
    cf::CtrlNpcVec3W* pos = self->field_28->_vAC();
    u32 x = pos->x;
    u32 y = pos->y;
    self->field_B0.u = y;
    self->field_AC.u = x;
    self->field_B4.u = pos->z;
    self->field_174 = 0;
    s16 state = self->field_16C != 0 ? self->field_16C : self->field_16E;
    self->field_28->field_6C4 = state;
}

// 0x800959A0: record the flag byte, then activate the character object if it
// is in one of the "in action" states (0x21..0x2A).
void func_80094FC8(cf::CtrlNpc* self, u8 val) {
    self->field_C2 = val;
    cf::CCtrlNpcChar* obj = self->field_28;
    if (obj->field_6C4 < 0x21)
        return;
    if (obj->field_6C4 > 0x2a)
        return;
    obj->field_6C4 = 1;
}

float CfObject_UnkVirtualFunc73__Q22cf13CfObjectModelFv(void* self) { return *(float*)((u8*)self + 0xa4); }

float CfObject_UnkVirtualFunc71__Q22cf13CfObjectModelFv(void* self) { return *(float*)((u8*)self + 0xa0); }

// 0x800959D8: when the character object's field_C4 flag is set, feed the
// halfword field_16C into the battle-status helper and bump the busy counter.
void func_80095000(cf::CtrlNpc* self) {
    cf::CCtrlNpcChar* obj = self->field_28;
    if (obj->field_C4 != 0) {
        func_800BE12C(obj, self->field_16C, 0, -1, 1);
        self->field_BE += 1;
    }
}

// 0x80095A34: when field_C4 is set, dispatch the battle-status helper with
// field_16C (or field_16E when it is zero); the zero branch bumps the busy
// counter one extra time.
void func_8009505C(cf::CtrlNpc* self) {
    cf::CCtrlNpcChar* obj = self->field_28;
    if (obj->field_C4 != 0) {
        if (self->field_16C != 0) {
            func_800BE12C(obj, self->field_16C, 1, -1, 1);
        } else {
            func_800BE12C(obj, self->field_16E, 1, -1, 1);
            self->field_BE += 1;
        }
        self->field_BE += 1;
    }
}

// 0x80095AC0: advance the NPC action - when the character object's field_C4
// flag is set and it is idle (or the page helper says so), feed field_16E
// into the battle-status helper and bump the busy counter.
void func_800950E8(cf::CtrlNpc* self) {
    cf::CCtrlNpcChar* obj = self->field_28;
    u32 flag = obj->field_C4;
    if (flag != 0 && (obj->_v84() != 0 || func_8004C5EC(flag) == 1)) {
        if (self->field_16E != 0) {
            int zero = (self->field_28->_v11C() == 0);
            func_800BE12C(self->field_28, self->field_16E, zero, -1, 1);
        }
        self->field_BE += 1;
    }
}

void func_8009519C() {}

// 0x80095B78: pull the movement target from the character object's position
// getter (vtable +0xac), record it, and either bump the busy counter or reset
// the movement flag.
void func_800951A0(cf::CtrlNpc* self) {
    cf::CtrlNpcVec3W* pos = self->field_28->_vAC();
    u32 x = pos->x;
    self->field_CC = pos->y;
    self->field_C8 = x;
    self->field_D0 = pos->z;
    self->field_BC = 0;
    if (self->field_28->field_C4 != 0)
        self->field_BE += 1;
    else
        self->field_14 = lbl_eu_80666698;
}

// 0x80095BFC: turn the NPC toward its current movement target (or wander when
// close enough). The heading is stored into field_0C; when far away it aims at
// the target with a random jitter, otherwise it picks a random turn (continuing
// the previous turn while the wander timer field_BC is still running).
void func_80095224(cf::CtrlNpc* self) {
    cf::CCtrlNpcChar* obj = self->field_28;
    if (obj->field_C4 != 0 && obj->_v24(0x1000) != 0)
        return;
    const ml::CVec3* posf =
        reinterpret_cast<const ml::CVec3*>(self->field_28->_vAC());
    const ml::CVec3* tgt = reinterpret_cast<const ml::CVec3*>(&self->field_C8);
    f32 d4 = self->field_D4;
    ml::CVec3 diff = *tgt - *posf;   // operator- emits the paired-single diff
    ml::CVec3 v = diff;
    f32 f1;
    f32 thresh = lbl_eu_806666B4 * (d4 * d4);
    if (diff.x * diff.x + diff.z * diff.z >= thresh) {
        // Far: aim at the target, jittered by a random amount in [-30, 30].
        f32 ang = lbl_eu_806666AC * nw4r::math::Atan2FIdx(diff.x, diff.z);
        f1 = ang +
             lbl_eu_806666B8 * (lbl_eu_8066A1F8 * (f32)ml::math::mtRand(-30, 30));
    } else {
        // Close: wander. Continue the current turn while the timer runs.
        if (self->field_BC != 0) {
            f32 rnd = (f32)ml::math::mtRand(60);
            f1 = (lbl_eu_806666BC + self->field_28->_vD8() + rnd) *
                 lbl_eu_8066A210;
        } else {
            f1 = (f32)ml::math::mtRand(360) * lbl_eu_8066A210;
        }
        self->field_BA = 0;
    }
    self->field_0C = f1;
    if (self->field_28 != 0) {
        self->field_28->_vC4();
        if (self->field_28->_v74() != 0)
            func_800BE12C(self->field_28, 3, 0, -1, 1);
    }
    self->field_BE = 2;
    self->field_B8 = (s16)(ml::math::mtRand(128) + 64);
}

// 0x80095E28: per-frame movement/action update - check the distance to the
// target, tick the action timers, and advance the busy state.
void func_80095450(cf::CtrlNpc* self) {
    const ml::CVec3* posf =
        reinterpret_cast<const ml::CVec3*>(self->field_28->_vAC());
    const ml::CVec3* tgt = reinterpret_cast<const ml::CVec3*>(&self->field_C8);
    ml::CVec3 diff;
    diff.x = tgt->x - posf->x;
    diff.y = tgt->y - posf->y;
    diff.z = tgt->z - posf->z;
    ml::CVec3 v = diff;
    f32 d4 = self->field_D4;
    if (diff.x * diff.x + diff.z * diff.z > d4 * d4) {
        if (self->field_BA < 0)
            self->field_B8 = 0;
    }
    cf::CCtrlNpcChar* obj = self->field_28;
    if (obj->field_C4 != 0 && obj->_v24(0x1000) != 0) {
        self->field_14 = lbl_eu_80666698;
    } else {
        if (self->field_BC > 0)
            self->field_BC -= 1;
        if (obj->field_C4 != 0) {
            if ((obj->field_4EC & 0x02000000) != 0) {
                if (self->field_BC > 0) {
                    self->field_B8 -= 1;
                    self->field_BA -= 1;
                } else {
                    self->field_B8 = 0;
                    self->field_BA = 0;
                    self->field_BC = 15;
                }
            } else {
                self->field_B8 -= 1;
                self->field_BA -= 1;
            }
        } else {
            self->field_B8 -= 1;
            self->field_BA -= 1;
        }
        self->field_14 = self->field_28->_v140();
        if (self->field_B8 <= 0) {
            self->field_BA = self->field_DE;
            self->field_B8 = 0;
            self->field_BE += 1;
            self->field_14 = lbl_eu_80666698;
            if (self->field_C0 == 1 || self->field_DE == 0) {
                self->field_BA = 0;
                self->field_BE = 1;
            }
        }
    }
}

struct CtrlNpcData {
    u8 _pad00[0xBA];
    short field_BA;
    u8 _padBC[2];
    short field_BE;
};

void func_8009563C(char* p) {
    CtrlNpcData* data = reinterpret_cast<CtrlNpcData*>(p);
    short v = data->field_BA - 1;
    data->field_BA = v;
    if (v <= 0) {
        data->field_BE = 1;
    }
}

// 0x80096034: movement-timer body (not yet recovered); the stub keeps the
// retail arg shape and C linkage (from the header declaration) so
// func_80096488's call reloc matches retail. noinline so the call stays a bl.
void __declspec(noinline) func_8009565C(cf::CtrlNpc* self) {}

void func_80095F44(){}

// 0x80096E60: query the game manager's current id halfword; when it falls
// outside [field_160, field_164), clear the busy flag and restart the
// movement timer.
void func_80096488(cf::CtrlNpc* self) {
    u16 id;
    u16 dummy;
    func_80086D98__Q22cf13CfGameManagerFv(&id, &dummy);
    func_80086DB0__Q22cf13CfGameManagerFv();
    if (id < self->field_160 || id >= self->field_164) {
        self->field_BE = 0;
        func_8009565C(self);
    }
}

// 0x80096EC4: movement-approach update. When the horizontal distance to the
// movement target is large enough, steer the heading toward it and drive the
// move helper; otherwise decay the remaining-duration timer and advance the
// action state.
int func_800964EC(cf::CtrlNpc* self) {
    const ml::CVec3* posf =
        reinterpret_cast<const ml::CVec3*>(self->field_28->_vAC());
    const ml::CVec3* tgt =
        reinterpret_cast<const ml::CVec3*>(self->field_EC);
    ml::CVec3 diff;
    diff.x = tgt->x - posf->x;
    diff.y = tgt->y - posf->y;
    diff.z = tgt->z - posf->z;
    ml::CVec3 v = diff;
    f32 len2 = diff.x * diff.x + diff.z * diff.z;
    if (len2 > lbl_eu_80666708) {
        f32 target = self->field_D8;
        if (len2 < lbl_eu_806666A0) {
            // nw4r FSqrt: warn on negative input, else sqrt via FrSqrt.
            if (!(len2 >= 0.0f)) {
                nw4r::db::Warning(lbl_eu_80526324, 0x273,
                                  lbl_eu_80526300);
            }
            f32 len = (len2 <= 0.0f)
                          ? 0.0f
                          : len2 * nw4r::math::FrSqrt(len2);
            target = (self->field_D8 - lbl_eu_806666A8) *
                         ((len - lbl_eu_8066670C) * lbl_eu_80666710) +
                     lbl_eu_806666A8;
        }
        self->field_14 = target;
        self->field_0C =
            lbl_eu_806666AC * nw4r::math::Atan2FIdx(v.x, v.z);
        if (func_800A5038(posf, reinterpret_cast<const ml::CVec3*>(self->field_EC),
                          self->field_D8, lbl_eu_8066670C) == 0)
            return 1;
    }
    self->field_14 = lbl_eu_80666698;
    self->field_D4 = lbl_eu_80666698;
    if (self->field_158 < lbl_eu_80666714) {
        self->field_0C = self->field_158 * lbl_eu_8066A210;
        if (self->field_28 != 0) {
            self->field_28->_vC4();
            if (self->field_28->_v74() != 0)
                func_800BE12C(self->field_28, 3, 0, -1, 1);
        }
    }
    return 0;
}

// 0x800970C0: per-frame NPC action update. Dispatches on the busy counter
// (field_BE): state 1 arms the movement/character action (depending on the
// packed action byte in field_C6), state 2 waits for the character to become
// idle, state 3 counts the movement timer down to 0.
void func_800966E8(cf::CtrlNpc* self) {
    if (self->field_BE == 1) {
        int armed;
        if (((self->field_C6 >> 8) & 0xFF) != 0)
            armed = func_800964EC(self);
        else
            armed = func_8019F8E0((cf::CCtrlMoveNpc*)self->_sub30);
        if (armed != 0) {
            self->field_BE = 1;
        } else if (self->field_158 < lbl_eu_80666714) {
            self->field_BE = 2;
        } else {
            self->field_D4 = lbl_eu_80666700;
            self->field_BE = 3;
        }
    } else if (self->field_BE == 2) {
        if (self->field_28->field_C4 == 0 ||
            self->field_28->_v24(0x1000) == 0) {
            self->field_D4 = lbl_eu_80666700;
            self->field_BE = 3;
        }
    } else if (self->field_BE == 3) {
        // CfGameManager.hpp declares func_80069EA0 void; it really returns
        // the frame delta (see CfTFile.cpp), so call it through a cast.
        f32 v = self->field_D4 - ((float (*)())func_80069EA0)();
        self->field_D4 = v;
        if (v <= lbl_eu_80666698) {
            self->field_D4 = lbl_eu_80666698;
            self->field_BE = 4;
        }
    }
}

// 0x800971D0: returns whether the NPC is active - either the "busy" flag is
// set, or the character object's state query (vtable +0x24) reports active.
int func_800967F8(cf::CtrlNpc* self) {
    if (self->field_174 != 0)
        goto ret1;
    if (self->field_28->_v24(0x1000) == 0)
        goto ret0;
ret1:
    return 1;
ret0:
    return 0;
}

extern "C" int func_8009684C(u8* self) { return 1; }

extern "C" int func_80096854(u8* self) { return 0; }

cf::CtrlNpc::~CtrlNpc() {}
