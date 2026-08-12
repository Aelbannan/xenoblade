// Auto-scaffolded catalog TU for kyoshin/cf/CfGimmickObject
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CfGimmickObject.hpp"

using namespace cf;

void __ct__cf_CfGimmickObject(){}

// Rebuild both collider matrices from the object's basis (+0x04) and the
// reference point (+0x10); collider A lives at +0x1C, collider B at +0xF4.
void func_801F7930(cf::CfGimmickObject* self) {
    func_802089BC(self->field_1C, self->field_04, &self->field_10);
    func_802089BC(self->field_F4, self->field_04, &self->field_10);
}

cf::CfGimmickObject::~CfGimmickObject() {
    this->vtable = (void*)lbl_eu_80534F70;
    func_80208EE4((void*)this);
    func_8020A434(&this->field_7C);
    __dt__Q22cf9CfGimmickFv((void*)this, 0);
    // MWCC appends the deleting-dtor prologue (null guard) and epilogue
    // (delete-flag ? operator delete(this) : skip) automatically.
}

// func_801F5B00 - step-machine driver. While the step index stays below 6,
// promote the 0x80000 busy flag to 0x40000000, then dispatch through the
// field_74 0x400 gate: the lbl_eu_80534EB8 pmf table when set, otherwise the
// func_801F634C helper (which may clear work via func_8020A0CC) or the
// lbl_eu_80534E70 pmf table. Stop on a 0 return and run the cleanup hook.
void func_801F5B00(cf::CfGimmickObject* self) {
    while (self->field_188 < 6) {
        if ((self->field_74 & 0x80000) != 0)
            // volatile read so the RMW re-loads field_74 (retail does not
            // reuse the branch-test value).
            *(volatile u32*)&self->field_74 |= 0x40000000;
        if ((self->field_74 & 0x400) != 0) {
            if ((self->*lbl_eu_80534EB8[self->field_188])() == 0)
                break;
        } else {
            if (func_801F634C(self) != 0) {
                if ((self->field_74 & 0x200) != 0)
                    func_8020A0CC();
                break;
            }
            if ((self->*lbl_eu_80534E70[self->field_188])() != 0)
                self->field_74 |= 0x400;
            else
                break;
        }
    }
    func_801F76A8(self);
}

void func_801F5BF8(void* self) {
    *(unsigned long*)((char*)self + 0x74) |= 0x10000;
}

extern "C" void func_801F5C08(u8* self) {
    *(unsigned long*)((char*)self + 0x74) &= ~0x18000;
}

extern "C" void func_801F5C18(u8* self) {
    *(unsigned short*)((char*)self + 0x188) = 6;
    *(unsigned long*)((char*)self + 0x74) = 0;
}

// Same-TU stub -- __declspec(noinline) keeps -ipa from folding the empty
// body into the retail caller (must emit a direct `bl`).
__declspec(noinline) void func_801F5C2C(cf::CfGimmickObject* self, int a, int b) {}

// Refresh the per-LOD task registrations and the linked map object.
void func_801F61B0(cf::CfGimmickObject* self, int mode) {
    for (int i = 0; i < 2; i++) {
        if (self->field_70[i] != 0) {
            func_80462DB4__8CTaskLODFv(self->field_70[i], mode);
            if (mode != 0) {
                func_804BCC30(func_804BC9EC__Fv(), self->field_70[i]);
            } else {
                func_804BCC3C(func_804BC9EC__Fv(), self->field_70[i]);
            }
        }
    }

    if (self->field_68 != 0) {
        ::CfGimmickObject* obj = (::CfGimmickObject*)func_80186BC8(self->field_68);
        if (obj != 0) {
            ((void (*)(::CfGimmickObject*, int))obj->vtable[0x158 >> 2])(obj, mode);
            func_804B1DC0((char*)obj + 0x60c, mode);
        }
    }
}

// func_801F627C - LOD timer refresh. While the object is busy (flag
// 0x10000000) or the step is 2/3 behind the field_66 0x20 gate, update the
// +0x178 LOD diff (mode != 0: countdown from the second LOD timer; mode == 0:
// snapshot the first) and raise the 0x10 working bit. Separate if-statements
// keep MWCC from folding the ==2/==3 tests into a subi range check.
void func_801F627C(cf::CfGimmickObject* self, u8 lod, int mode) {
    if ((self->field_74 & 0x10000000) != 0)
        goto refresh;
    if ((self->field_66 & 0x20) == 0)
        return;
    if (self->field_188 == 2)
        goto refresh;
    if (self->field_188 != 3)
        return;
refresh:
    if ((self->field_74 & 0x10) != 0)
        return;
    if (mode != 0) {
        f32 base = func_80462F2C__8CTaskLODFv(lod);
        f32 v = func_80462FF4__8CTaskLODFv(lod);
        u32 w = self->field_74;
        self->field_178 = v - base;
        // volatile read so MWCC re-loads field_74 here instead of reusing w
        // (retail emits a fresh lwz after the branch).
        if ((w & 0x20000000) != 0)
            *(volatile u32*)&self->field_74 |= 0x40000000;
    } else {
        self->field_178 = func_80462F2C__8CTaskLODFv(lod);
    }
    self->field_74 |= 0x10;
}

// func_801F634C - step-machine helper (stub; returns "not done").
__declspec(noinline) int func_801F634C(cf::CfGimmickObject* self) { return 0; }

// Same-TU stub -- __declspec(noinline) keeps -ipa from folding the empty
// body into the retail caller (must emit a direct `bl`).
__declspec(noinline) void func_801F6780(cf::CfGimmickObject* self) {}

// func_801F6B98 - per-LOD frame update. Seeded from a +0x0C frame count,
// then dispatched on the +0x05 flag byte: 0x4/0x2 re-register the LOD task,
// 0x1/0x2 raise a LOD timer, 0x30 rebuilds the LOD2 range and 0x8 flips the
// LOD register mode. A +0x06 id byte change refreshes the +0x194 slot.
void func_801F6B98(cf::CfGimmickObject* self, u8 lod,
                   const CfGimmickLodFrame* frame) {
    if (frame->field_0C != 0)
        func_80462FD8__8CTaskLODFv(lod, (f32)frame->field_0C);
    u8 flags = frame->field_05;
    if ((flags & ~0x40) != 0) {
        if ((flags & 0x4) != 0) {
            if ((flags & 0x2) != 0)
                func_80462EF4__8CTaskLODFv(lod, lbl_eu_806681A0);
            else
                func_80462F10__8CTaskLODFv(lod);
            func_80462F4C__8CTaskLODFv(lod, 0);
        } else if ((flags & 0x1) != 0) {
            func_80462F70__8CTaskLODFv(lod, 0);
            func_80462F4C__8CTaskLODFv(lod, 1);
            func_801F627C(self, lod, 1);
        } else if ((flags & 0x2) != 0) {
            func_80462F70__8CTaskLODFv(lod, 1);
            func_80462F4C__8CTaskLODFv(lod, 1);
            func_801F627C(self, lod, 0);
        } else if ((flags & 0x30) != 0) {
            func_80462F4C__8CTaskLODFv(lod, 0);
            if ((flags & 0x20) != 0) {
                self->field_74 |= 0x8;
                self->field_174 = lbl_eu_806681B4;
                for (int i = 0; i < 2; i++) {
                    if (self->field_70[i] != 0) {
                        f32 a = func_80462F2C__8CTaskLODFv(self->field_70[i]);
                        f32 b = func_80462FF4__8CTaskLODFv(self->field_70[i]);
                        f32 c = a + lbl_eu_806681A4;
                        if (b <= c)
                            c -= b;
                        func_80462EF4__8CTaskLODFv(self->field_70[i], c);
                    }
                }
            }
        }
    }
    if ((flags & 0x8) != 0)
        func_80462ED0__8CTaskLODFv(lod, 0);
    else
        func_80462ED0__8CTaskLODFv(lod, 1);
    if (self->field_194 != frame->field_06) {
        self->field_194 = frame->field_06;
        func_80462F94__8CTaskLODFv(lod, frame->field_06);
    }
}

// func_801F6D8C - party-membership gate. With the 0x8000 flag of +0x152 set,
// returns 1 when any flagged character slot (bits 0..14) IS registered in the
// party (func_8009E284 != 0); otherwise returns 1 when any flagged slot is
// NOT registered. field_152 is re-read every iteration because the party
// queries may mutate it.
int func_801F6D8C(cf::CfGimmickObject* self) {
    u16 flags = self->field_152;
    if (flags != 0) {
        if ((flags & 0x8000) != 0) {
            for (int i = 0; i < 15; i++) {
                if ((self->field_152 & (1 << i)) != 0 &&
                    func_8009E284(func_8009ECB0(), i + 1) != 0)
                    return 1;
            }
        } else {
            for (int i = 0; i < 15; i++) {
                if ((self->field_152 & (1 << i)) != 0 &&
                    func_8009E284(func_8009ECB0(), i + 1) == 0)
                    return 1;
            }
        }
    }
    return 0;
}

// Same-TU stub -- __declspec(noinline) keeps -ipa from folding the empty
// body into the retail caller (must emit a direct `bl`).
__declspec(noinline) void func_801F6E60(cf::CfGimmickObject* self, u8 arg) {}

// Scan the 3-entry u16 table at +0x14A from the index in +0x18C (stub).
// __declspec(noinline) keeps -ipa from folding the stub into func_801F856C.
__declspec(noinline) int func_801F72A4(cf::CfGimmickObject* self, u16* table) { return 0; }

// func_801F75CC - countdown/activation step. While flag 0x4000 is set,
// decrement +0x170 by the scene allocator's frame delta and keep returning 1
// until it drops to lbl_eu_806681A0 (then clear 0x4000 and return 0).
// Otherwise (re)start the countdown from lbl_eu_806681B8, play the entry
// sound for the old handle, refresh the handle via func_80208C48 and return 1.
int func_801F75CC(cf::CfGimmickObject* self) {
    if (self->field_158 != 0) {
        func_8020A010();
        u32 flags = self->field_74;
        if ((flags & 0x4000) != 0) {
            f32 delta = func_80496288(lbl_eu_80663E14);
            f32 cur = self->field_170 - delta;
            f32 limit = lbl_eu_806681A0;
            self->field_170 = cur;
            if (cur > limit)
                return 1;
            self->field_74 &= ~0x4000;
        } else {
            // volatile reads keep the else-path RMW a fresh load (retail does
            // not reuse the branch-test value of field_74); load order v, f74,
            // const matches retail's schedule.
            u16 v = *(volatile u16*)&self->field_80;
            u32 f = *(volatile u32*)&self->field_74;
            f32 c = lbl_eu_806681B8;
            self->field_74 = f | 0x4000;
            self->field_170 = c;
            if (v != 0xffff)
                func_801BFED0(1, v, 0xa);
            self->field_80 = func_80208C48(self->field_158, self->field_04);
            if ((lbl_eu_806646BC & 0x8) != 0)
                self->field_74 |= 0x00800000;
            return 1;
        }
    }
    return 0;
}

// func_801F76A8 - step-machine cleanup (stub).
__declspec(noinline) void func_801F76A8(cf::CfGimmickObject* self) {}

// func_801F7978 - area availability check. While the 0x80000 busy flag is
// set for the +0x6A area id, a registered/ready party (func_8020A5DC / the
// +0x7C slot) keeps the 0x20000 ready flag; a failed check clears it and the
// caller must not run. Otherwise the +0x6C..0x6E / +0x13C..0x142 sequence
// windows, the +0x15F/0x160 random window and the +0x161 mode bits gate the
// per-frame effect.
int func_801F7978(cf::CfGimmickObject* self) {
    int result = 0;
    u16 areaId = self->field_6A;
    if (areaId != 0 && (self->field_74 & 0x80000) != 0) {
        if ((self->field_74 & 0x20000) != 0) {
            if (func_8020A5DC() != 0)
                result = 1;
            else
                self->field_74 &= ~0x20000;
        } else {
            if (func_8020A87C(self, (u32)self->field_7C.field_00) != 0) {
                func_8020A484(areaId);
                self->field_74 |= 0x20000;
                result = 1;
            }
        }
    }
    if (result != 0)
        return 0;
    if (self->field_6E != 0) {
        u32 seq = func_800822F4__Q22cf13CfGameManagerFv();
        if (self->field_6C > seq || seq > self->field_6E)
            return 0;
    }
    if (self->field_13C != 0 &&
        func_80082354__Q22cf13CfGameManagerFv(self->field_13C) != self->field_13E)
        return 0;
    if (self->field_140 != 0 &&
        func_8020971C((void*)(u32)self->field_140) == 0)
        return 0;
    if (self->field_142 != 0 &&
        func_802096EC((void*)(u32)self->field_142) == 0)
        return 0;
    if (self->field_15F != 0 || self->field_160 != 0) {
        u16 rnd = (u16)func_8006A33C();
        u8 lo = self->field_15F;
        u8 hi = self->field_160;
        if (lo > hi) {
            if (hi < rnd && rnd < lo)
                return 0;
        } else {
            if (lo > rnd && rnd > hi)
                return 0;
        }
    }
    if ((self->field_161 & 0x10) != 0 && (self->field_161 & 0x4) == 0)
        self->field_74 |= 0x80000;
    return 1;
}

// func_801F7B44 - availability gate for the +0x156 area id (mirrors
// func_801F7978's first block), then scans both +0x84 area tables against
// the scenario/sequence windows, and finally the +0x74 party flags: while
// the 0x8000/0x10000 bits are raised and the global 0x4 bit is clear, the
// step machine is (re)initialized via func_801F5C2C.
int func_801F7B44(cf::CfGimmickObject* self) {
    int result = 0;
    u16 areaId = self->field_156;
    if (areaId != 0 && (self->field_74 & 0x80000) != 0) {
        if ((self->field_74 & 0x20000) != 0) {
            if (func_8020A5DC() != 0)
                result = 1;
            else
                self->field_74 &= ~0x20000;
        } else {
            if (func_8020A87C(self, (u32)self->field_7C.field_00) != 0) {
                func_8020A484(areaId);
                self->field_74 |= 0x20000;
                result = 1;
            }
        }
    }
    if (result != 0)
        return 0;
    for (int i = 0; i < 2; i++) {
        if (self->field_84[i].field_06 != 0) {
            u32 seq = func_800822F4__Q22cf13CfGameManagerFv();
            if (self->field_84[i].field_04 > seq || seq > self->field_84[i].field_06)
                return 0;
        }
        if (self->field_84[i].field_00 != 0 &&
            func_80082354__Q22cf13CfGameManagerFv(self->field_84[i].field_00) !=
                self->field_84[i].field_02)
            return 0;
        if (self->field_84[i].field_08 != 0 &&
            func_8020971C((void*)(u32)self->field_84[i].field_08) == 0)
            return 0;
        if (self->field_84[i].field_0A != 0 &&
            func_802096EC((void*)(u32)self->field_84[i].field_0A) == 0)
            return 0;
        if (self->field_84[i].field_0C != 0 &&
            func_801587E8(self->field_84[i].field_0C) == 0)
            return 0;
    }
    if (func_801F6D8C(self) != 0)
        return 0;
    if ((self->field_74 & 0x8000) != 0) {
        if ((self->field_74 & 0x10000) != 0 && (lbl_eu_806646BC & 0x4) == 0) {
            func_801F5C2C(self, 0, 5);
            self->field_74 &= ~0x18000;
            func_8020974C(self->field_64, 1);
        }
        return 0;
    }
    return 1;
}

// func_801F7D38 - per-frame update. Party membership (func_801F6D8C) puts
// the step machine into state 1; while the +0x15E mode byte is active the
// +0x6C..0x6E and per-area +0x04/0x06 sequence windows must contain the
// current scenario sequence. The +0x66 flags then drive the spawn/effect
// gate (func_802098EC) and, on the fail path, the +0x6A area registration
// retry (func_8020A5DC / func_8020A87C).
int func_801F7D38(cf::CfGimmickObject* self) {
    if (func_801F6D8C(self) != 0) {
        self->field_188 = 1;
        return 0;
    }
    if (self->field_15E == 1) {
        if (self->field_6E != 0) {
            u32 seq = func_800822F4__Q22cf13CfGameManagerFv();
            if (self->field_6C > seq || seq > self->field_6E) {
                self->field_188 = 0;
                return 0;
            }
        }
        for (int i = 0; i < 2; i++) {
            if (self->field_84[i].field_06 != 0) {
                u32 seq = func_800822F4__Q22cf13CfGameManagerFv();
                if (self->field_84[i].field_04 > seq ||
                    seq > self->field_84[i].field_06) {
                    self->field_188 = 0;
                    return 0;
                }
            }
        }
    }
    if (func_802098EC(self->field_66, (cf::CfGimmick*)&self->field_1C,
                      (const CfGimmickVec3*)self->field_04,
                      (const f32*)&self->field_10, self->field_7C.field_00) != 0) {
        for (int i = 0; i < 2; i++) {
            if (self->field_84[i].field_0C != 0 && self->field_84[i].field_0E != 0)
                func_80159C04(self->field_84[i].field_0C, 1);
        }
        if ((self->field_161 & 0x8) != 0)
            self->field_74 &= ~0xC0000;
        if ((self->field_66 & 0x20) != 0)
            func_8020974C(self->field_64, 1);
        return 1;
    }
    if ((self->field_66 & 0x4) != 0) {
        u16 areaId = self->field_6A;
        if (areaId != 0 && (self->field_74 & 0x80000) != 0) {
            if ((self->field_74 & 0x20000) != 0) {
                if (func_8020A5DC() == 0)
                    self->field_74 &= ~0x20000;
            } else {
                if (func_8020A87C(self, (u32)self->field_7C.field_00) != 0) {
                    func_8020A484(areaId);
                    self->field_74 |= 0x20000;
                }
            }
        }
    }
    return 0;
}

void func_801F7F24(){}

int func_801F8564() { return 1; }

// Busy-gate: an object flagged busy in field_74 (bit 12) with the global
// busy bit (lbl_eu_80663E28 bit 5) set, or whose +0x14A table fails
// func_801F72A4, is NOT available (returns 0).
int func_801F856C(cf::CfGimmickObject* self) {
    if (self->field_74 & 0x1000) {
        if (lbl_eu_80663E28 & 0x20)
            return 0;
        if (func_801F72A4(self, self->field_14A) == 0)
            return 0;
    }
    return 1;
}

// Advance the +0x188 step counter (down while flagged busy, up otherwise,
// clamped to 0..6), clearing the working bits on the way out.
int func_801F85C4(cf::CfGimmickObject* self) {
    func_801F6780(self);
    if (self->field_74 & 0x08000000) {
        s16 v = (s16)(self->field_188 - 1);
        self->field_188 = v;
        if (v < 0)
            self->field_188 = 0;
        self->field_74 &= ~0x10;
    } else {
        s16 v = (s16)(self->field_188 + 1);
        self->field_188 = v;
        if (v > 6)
            self->field_188 = 6;
    }
    self->field_74 &= ~0x200000;
    return 1;
}

// func_801F8658 - per-frame step/activation driver (continuation of
// func_801F85C4's step-machine). Runs the countdown via func_801F75CC; when
// it expires, re-seeds the +0x17C timer from +0x15A, refreshes the area
// checks (func_801F6E60 / func_801F6780), sets the busy bits and advances
// the +0x188 step counter (down while busy, up otherwise, clamped 0..6).
int func_801F8658(cf::CfGimmickObject* self) {
    if (func_801F75CC(self) != 0) {
        if (self->field_C4 != 0)
            func_80209F5C();
        return 0;
    }
    if (self->field_15A != 0) {
        self->field_17C = (f32)self->field_15A;
        self->field_74 |= 0x20;
    }
    func_801F6E60(self, self->field_162);
    func_801F6780(self);
    if ((self->field_74 & 0x100) != 0) {
        func_8020A068((int)self, 1, 1);
        self->field_74 |= 0x500000;
    }
    if ((self->field_74 & 0x200) != 0 && (self->field_74 & 0x08000000) == 0 &&
        (self->field_74 & 0x3F) != 0)
        func_8020A0CC();
    if ((self->field_74 & 0x08000000) != 0) {
        s16 v = (s16)(self->field_188 - 1);
        self->field_188 = v;
        self->field_74 &= ~0x08000000;
        if (v < 0)
            self->field_188 = 0;
    } else {
        s16 v = (s16)(self->field_188 + 1);
        self->field_188 = v;
        if (v > 6)
            self->field_188 = 6;
    }
    self->field_74 &= ~0x400;
    return 1;
}

void func_801F879C(){}

void func_801F89B8(){}

// func_801F8BB8 - state refresh. Set the +0x188 step from the +0x15E mode
// byte (2 when +0x161 bit 7 is set, else 0; 6 when not active), fire the
// id-table effects (func_80193678) for the +0x14A / +0x144 entries when the
// matching field_74 bits are raised, then clear the 0x80000000/0x400 work
// flags and report "not busy".
int func_801F8BB8(cf::CfGimmickObject* self) {
    if (self->field_15E == 1) {
        if ((self->field_161 & 0x80) != 0)
            self->field_188 = 2;
        else
            self->field_188 = 0;
        if ((self->field_74 & 0x1000) != 0) {
            for (int i = 0; i < 3; i++) {
                u16 id = self->field_14A[i];
                if (id != 0)
                    func_80193678(id);
            }
        }
        if ((self->field_74 & 0x800) != 0) {
            if (self->field_164 == 1) {
                for (int i = 0; i < 3; i++) {
                    u16 id = self->field_144[i];
                    if (id != 0)
                        func_80193678(id);
                }
            }
        }
    } else {
        self->field_188 = 6;
    }
    self->field_74 &= 0x7FFFFBFF;
    return 0;
}
