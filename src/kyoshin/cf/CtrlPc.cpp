// Auto-scaffolded catalog TU for kyoshin/cf/CtrlPc
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CtrlPc.hpp"
#include "kyoshin/cf/CArtsSet.hpp"
#include "kyoshin/cf/CtrlMoveBase.hpp"
#include "kyoshin/cf/CtrlMovePC.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/code_800F42AC.hpp"
#include "kyoshin/cf/voice/cvsys/CVS_THREAD_REVIVE.hpp"
#include "monolib/core/CPadManager.hpp"
// Retail __ct__cf_CtrlPad (0x80097274): base-constructs CtrlRemote (passing
// posObj through, arg5 = 0), stores the CtrlPad vtable manually (novtable),
// caches the current pad, then copies the 30-entry pad config table via
// func_80098BD0.
void __ct__cf_CtrlPad(cf::CtrlPad* obj, void* posObj) {
    // CtrlPad's retail layout is CtrlRemote's (retail ctor chain
    // __ct__cf_CtrlPad -> __ct__CtrlRemote -> __ct__cf_CtrlPc).
    __ct__CtrlRemote(reinterpret_cast<cf::CtrlRemote*>(obj), posObj, 0);
    *(void**)obj = (void*)lbl_eu_80527C80;
    reinterpret_cast<cf::CtrlPc*>(obj)->mField1D4 =
        (u32)cf::CfGameManager::getCurrentPad();
    for (int i = 0; i < 0x1E; i++) {
        func_80098BD0(i, lbl_eu_80527C08[i]);
    }
}

cf::CtrlRemote::~CtrlRemote() {
    // Destroy the CtrlPc sub-object with a direct call (retail: li r4,0;
    // bl __dt__Q22cf6CtrlPcFv). Extern "C" name prevents MWCC inlining the
    // in-TU ~CtrlPc stub body.
    __dt__Q22cf6CtrlPcFv(reinterpret_cast<cf::CtrlPc*>(this), 0);
}

// Retail func_80096974 (0x8009734C): player auto-pad/aiming handler.
// Routes the pad state through the sub-objects, applies target-lock flags,
// and computes the facing angle toward the locked target.
void func_80096974(cf::CtrlPc* self) {
    cf::CfGameManager::getInstance();
    UnkClass_800821F8View* gmSub = (UnkClass_800821F8View*)func_800821F8__Q22cf13CfGameManagerFv();
    CfObj90E4* g = func_800FE68C();
    CtrlVoiceHandle* vh = (CtrlVoiceHandle*)(void*)func_800B708C__Fi((int)g->mField90E4);
    {
        CtrlPlayerObj* p = self->mField5C;
        u32 v = *p->mField4->vf30();
        if (func_80174C98(p, &v, 1) != 0) return;
    }
    {
        CtrlPlayerObj* p = self->mField5C;
        u32 v = *p->mField4->vf30();
        if (func_80174C98(p, &v, 2) != 0) return;
    }
    int lock = 0;
    int flag28 = 0;
    int flag27 = 0;
    int flag26 = 0;
    self->mField5C->mSub3ED4->vf10(0x400, 1);
    u32 m12 = func_80098B74(0x12);
    if ((self->vf37()->mField0 & m12) != 0) {
        if (vh == 0) {
            flag28 = 1;
        } else if ((self->vf37()->mField0 & 3) != 0) {
            flag28 = 1;
        } else if ((self->vf37()->mField14 & func_80098B74(0x12)) != 0) {
            lock = 1;
        }
    } else if ((gmSub->mField4 & 0x10) != 0) {
        flag28 = 1;
    }
    if ((self->vf37()->mField0 & func_80098B74(0x10)) == 0) {
        self->mField5C->mSub3ED4->vf11(0x2000);
    }
    if (flag28 != 0) {
        self->mField5C->mSub3ED4->vf10(0x2000, 1);
        if ((gmSub->mField4 & 0x10) != 0) {
            func_8006BC1C(gmSub, 0x10);
            if (self->mField5C->mSub3ED4->vf14(0x400) == 0) {
                flag26 = 1;
            }
        }
    }
    if (lock != 0) {
        if ((gmSub->mField4 & 0x10) == 0) {
            if (self->mField5C->mSub3ED4->vf14(0x2000) == 0) {
                func_8006BBF4(gmSub, 0x10, 1);
                flag27 = 1;
            }
        }
    }
    if ((gmSub->mField4 & 0x10) != 0) {
        gmSub->vf09(vh->vf41());
    }
    if ((self->vf37()->mField10 & func_80098B74(0x13)) != 0) {
        func_8006BBF4(gmSub, 0x20, 1);
    }
    func_8006BBF4(gmSub, 3, self->mField5C->mSub3ED4->vf14(0x400) == 0);
    if ((self->vf37()->mField4 & func_80098B74(0x14)) != 0) {
        u32 v14 = self->mField5C->mSub3ED4->vf14(0x1000);
        if (v14 != 0) flag26 = 1;
        if (v14 == 0) flag27 = 1;
    }
    if (self->mField5C->mSub3ED4->vf14(0x1000) != 0) {
        if (vh != 0) {
            CVoicePos* p1 = self->mField5C->mSub3E9C.getPosition();
            CVoicePos* p2 = (CVoicePos*)vh->vf41();
            ml::CVec3 diff;
            diff.x = p2->f[0] - p1->f[0];
            diff.y = p2->f[1] - p1->f[1];
            diff.z = p2->f[2] - p1->f[2];
            ml::CVec3 d = diff;
            self->mField10 = lbl_eu_80666718 * nw4r::math::Atan2FIdx(d.x, d.z);
        } else {
            flag26 = 1;
        }
    }
    {
        CtrlPlayerObj* p = self->mField5C;
        u32 v = *p->mField4->vf30();
        if (func_80174C98(p, &v, 3) != 0) {
            flag26 = 1;
            flag27 = 0;
        }
    }
    if (flag27 != 0) {
        if (self->mField5C->mSub3ED4->vf14(0x1000) == 0) {
            self->mField5C->mSub3ED4->vf10(0x1000, 1);
        }
    }
    if (flag26 != 0) {
        if (self->mField5C->mSub3ED4->vf14(0x1000) != 0) {
            self->mField5C->mSub3ED4->vf11(0x1000);
        }
    }
}

// bit extracts from word at +0x2C (retail: lwz r0,0x2c(r3); extrwi r3,r0,1,b -> (x>>(32-1-b))&1)
extern "C" u32 func_80096EE8(void* self) { return (*(u32*)((char*)self + 0x2C) >> 23) & 1; }

extern "C" u32 func_80096EF4(void* self) { return (*(u32*)((char*)self + 0x2C) >> 22) & 1; }

extern "C" u32 func_80096F00(void* self) { return (*(u32*)((char*)self + 0x2C) >> 21) & 1; }

u32 cf::CtrlPc::testBit20() { return (mPadFlags >> 20) & 0x1u; }
u32 cf::CtrlPc::testBit16() { return (mPadFlags >> 16) & 0x1u; }
u32 cf::CtrlPc::testBit15() { return (mPadFlags >> 15) & 0x1u; }
u32 cf::CtrlPc::testBit14() { return (mPadFlags >> 14) & 0x1u; }
u32 cf::CtrlPc::testBit13() { return (mPadFlags >> 13) & 0x1u; }
u32 cf::CtrlPc::testBit12() { return (mPadFlags >> 12) & 0x1u; }
u32 cf::CtrlPc::testBit11() { return (mPadFlags >> 11) & 0x1u; }
u32 cf::CtrlPc::testBit10() { return (mPadFlags >> 10) & 0x1u; }
u32 cf::CtrlPc::testBit9() { return (mPadFlags >> 9) & 0x1u; }
u32 cf::CtrlPc::testBit8() { return (mPadFlags >> 8) & 0x1u; }

cf::CtrlPad::~CtrlPad() {
    // Nested null-guard: reproduces the retail double-beq (D2-inlined-into-D1).
    if (this != 0) {
        __dt__Q22cf6CtrlPcFv(reinterpret_cast<cf::CtrlPc*>(this), 0);
    }
}

// Retail __ct__cf_CtrlPc (0x800979DC): base-constructs the CtrlRemote base
// (__ct__800D10DC) passing posObj/arg5 through, stores the retail vtable
// manually, constructs the CCtrlMovePC sub-object at +0x8C, then snapshots
// the pad position from posObj (vtable slot 0xAC) or ml::CVec3::zero.
void __ct__cf_CtrlPc(cf::CtrlPc* obj, void* posObj, void* arg5) {
    __ct__800D10DC(obj, posObj, arg5);
    *(void**)obj = (void*)lbl_eu_80527DB0;
    __ct__801993C4(reinterpret_cast<cf::CCtrlMovePC*>(obj->mSubObj8C), obj,
                   posObj);
    obj->mField1D4 = 0;
    obj->mField1D8 = (u32)arg5;
    obj->mField1D4 = (u32)cf::CfGameManager::getCurrentPad();
    if (posObj != 0) {
        ml::CVec3* v = reinterpret_cast<cf::CCtrlMovePosObj*>(posObj)->getPosition();
        obj->mPos7C.x = v->x;
        obj->mPos7C.y = v->y;
        obj->mPos7C.z = v->z;
    } else {
        obj->mPos7C = ml::CVec3::zero;
    }
}

cf::CtrlPc::~CtrlPc() {
    // novtable class: store the retail vtable manually (MWCC would emit its
    // own __vt__Q22cf6CtrlPc otherwise -> reloc name drift vs lbl_eu_80527DB0).
    *(void**)this = (void*)lbl_eu_80527DB0;
    // Destroy the CCtrlMovePC sub-object at +0x8C (retail func_80199618).
    func_80199618(reinterpret_cast<cf::CCtrlMovePC*>(mSubObj8C));
}

// Retail func_80097134 (0x80097B0C): per-frame pad handler for the player.
// Drives the move sub-object, syncs the battle target voice state, and calls
// the pad-handler virtuals.
void func_80097134(cf::CtrlPc* self) {
    self->mField4 = 0;
    CtrlPlayerObj* obj = (CtrlPlayerObj*)func_800BFC68(cf::CfGameManager::getPlayer(0));
    u32 flags = lbl_eu_80663E24;
    if ((flags & 0x2000400) != 0) {
        return;
    }
    CtrlPlayerObj* player = self->mField5C;
    int doMove = 0;
    if (player != 0) {
        CtrlPlayerObj* p1 = self->mField5C;
        u32 v1 = *p1->mField4->vf30();
        int c4 = func_80174C98(p1, &v1, 4);
        CtrlPlayerObj* p2 = self->mField5C;
        u32 v2 = *p2->mField4->vf30();
        int c3 = func_80174C98(p2, &v2, 3);
        if ((c3 | c4) != 0) {
            CtrlPlayerObj* p3 = self->mField5C;
            u32 v3 = *p3->mField4->vf30();
            if (func_80174C98(p3, &v3, 0x4000) == 0) {
                if (func_80148778(&self->mField5C->mField8, 6) == 0 &&
                    func_80148778(&self->mField5C->mField8, 0xCD) == 0) {
                    CtrlPlayerObj* p4 = obj;
                    u32 v4 = *p4->mField4->vf30();
                    if (func_80174C98(p4, &v4, 0x803) != 0) {
                        CtrlPlayerObj* p5 = obj;
                        u32 v5 = *p5->mField4->vf30();
                        if (func_80174C98(p5, &v5, 0x800000) == 0) {
                            if (func_800DA06C(
                                    getInstance__Q22cf14CBattleManagerFv(), obj) != 0) {
                                self->mField5C->mSub3E9C.v00(0x100);
                            }
                        }
                    }
                }
            }
        }
    }
    if (self->mField5C->mSub3E9C.v01(0x100) != 0) {
        CtrlPlayerObj* p = obj;
        u32 v = *p->mField4->vf30();
        if (func_80174C98(p, &v, 0x803) != 0) {
            CtrlPlayerObj* p2 = obj;
            u32 v2 = *p2->mField4->vf30();
            if (func_80174C98(p2, &v2, 0x1F) == 0) {
                if (self->mField5C->vf173() == 0) {
                    self->mField4 |= 4;
                }
            }
        }
    }
    {
        CtrlPlayerObj* p = self->mField5C;
        if (p->mField3F60 != 0) {
            CtrlPlayerObj* p2 = self->mField5C;
            u32 v = *p2->mField4->vf30();
            if (func_80174C98(p2, &v, 0x1A) != 0) {
                if ((self->mField5C->mField3F60->mField4EC & 0x10) == 0) {
                    self->mField5C->mField3F60->mField4EC |= 0x10;
                    self->mField5C->mField3F60->mField510 =
                        self->mField5C->mSub3E9C.getPosition()->f[1];
                }
            } else {
                cf::CfGameManager::getInstance();
                if (func_8006EF04__Fi(0x4000000) == 0) {
                    CtrlPlayerSub3F60* t2 = self->mField5C->mField3F60;
                    u32 f4ec = t2->mField4EC;
                    if ((f4ec & 0x10) != 0) {
                        u32 fC = t2->mFieldC;
                        if ((fC & 0x2) != 0 || (f4ec & 0x2) != 0) {
                            t2->mField4EC &= ~0x10;
                        }
                    }
                }
            }
        }
    }
    if (self->mField5C != 0) {
        CtrlPlayerObj* p = self->mField5C;
        u32 v = *p->mField4->vf30();
        if (func_80174C98(p, &v, 0x803) != 0) {
            self->vf33();
            self->vf20();
            self->vf34();
            self->vf21();
            doMove = 1;
        }
    } else {
        func_8019956C((cf::CCtrlMovePC*)self->mSubObj8C);
        self->mField10 = self->mFieldC;
        if (func_80148778(&self->mField5C->mField8, 6) != 0 ||
            func_8006EF04__Fi(0x4000000) != 0) {
            self->mField14 = lbl_eu_80666720;
        }
    }
    self->vf35();
    self->vf36();
    if (doMove != 0) {
        func_8019A9C4((cf::CCtrlMovePC*)self->mSubObj8C);
    }
}

// Retail func_80097598 (0x80097F70): pad action dispatch for the player.
// Validates the pad-state, installs AI actions, and syncs the voice owner.
void func_80097598(cf::CtrlPc* self) {
    cf::CfGameManager::getInstance();
    if (func_8006EF04__Fi(0x1000000) != 0) goto fail;
    {
        CtrlPlayerObj* p = self->mField5C;
        u32 v = *p->mField4->vf30();
        if (func_80174C98(p, &v, 1) != 0) goto fail;
    }
    {
        CtrlPlayerObj* p = self->mField5C;
        u32 v = *p->mField4->vf30();
        if (func_80174C98(p, &v, 8) != 0) goto fail;
    }
    {
        CtrlPlayerObj* p = self->mField5C;
        u32 v = *p->mField4->vf30();
        if (func_80174C98(p, &v, 0x4000) != 0) goto fail;
    }
    {
        CtrlPlayerObj* p = self->mField5C;
        u32 v = *p->mField4->vf30();
        if (func_80174C98(p, &v, 0x1D) != 0) goto fail;
    }
    if (func_80148778(&self->mField5C->mField8, 0xF) != 0) goto fail;
    if (func_80148778(&self->mField5C->mField8, 9) == 0) goto cont;
fail:
    self->mField14 = lbl_eu_80666720;
    return;
cont:
    {
        CtrlPlayerObj* obj = (CtrlPlayerObj*)func_800BFC68(cf::CfGameManager::getPlayer(0));
        if (self->mField5C->mSub3E9C.v01(0x200) != 0) {
            self->mField4 |= 8;
        } else if (self->mField5C->mSub3E9C.v01(0x100) != 0 &&
                   self->mField5C->vf173() == 0) {
            self->mField4 |= 4;
        }
        if (self->mField5C->mSub3E9C.v17() == 0) {
            void* src = func_80098694(self);
            if (src != 0) {
                self->mField5C->mSub3E9C.v18(src);
            }
        }
        if (self->vf32() != 0) {
            return;
        }
        {
            CtrlPlayerObj* p = self->mField5C;
            u32 v = *p->mField4->vf30();
            if (func_80174C98(p, &v, 5) != 0) {
                self->mField14 = lbl_eu_80666720;
            } else {
                CtrlPlayerObj* p2 = self->mField5C;
                u32 v2 = *p2->mField4->vf30();
                if (func_80174C98(p2, &v2, 0x800) == 0) {
                    CtrlPlayerObj* p3 = obj;
                    u32 v3 = *p3->mField4->vf30();
                    if (func_80174C98(p3, &v3, 0x802) != 0) {
                        self->mField5C->mSub3E9C.v00(0x200);
                        return;
                    }
                    self->vf22();
                }
                {
                    CtrlPlayerObj* p4 = self->mField5C;
                    u32 v4 = *p4->mField4->vf30();
                    if (func_80174C98(p4, &v4, 0x4000) != 0 ||
                        func_80148778(&self->mField5C->mField8, 6) != 0 ||
                        func_80148778(&self->mField5C->mField8, 0xCD) != 0) {
                        self->mField14 = lbl_eu_80666720;
                    }
                }
                {
                    CtrlPlayerObj* p5 = self->mField5C;
                    u32 v5 = *p5->mField4->vf30();
                    if (func_80174C98(p5, &v5, 3) != 0 ||
                        func_80174C98(p5, &v5, 6) != 0 ||
                        func_80174C98(p5, &v5, 0x806) != 0) {
                        if (self->mField5C->mSub3E9C.v01(4) != 0) {
                            self->mField14 = lbl_eu_80666720;
                            self->mField5C->mSub3E9C.v47(self->mFieldC);
                            func_800BE12C(&self->mField5C->mSub3E9C, 3, 0, -1, 1);
                            self->mField5C->mSub3E9C.v02(4);
                        }
                    }
                }
            }
        }
    }
    return;
}

void func_80097A5C(){}

// Retail func_80097C74 (0x8009864C): battle-state maintenance. Sets/clears
// bit 2 of the player's target word based on the battle-manager state, then
// sets/clears bit 3 depending on the actor-id / art queries via func_80174C98.
void func_80097C74(cf::CtrlPc* self) {
    if (self->mField5C->mField3F60 != 0) {
        u32 v = getInstance__Q22cf14CBattleManagerFv()->mField1AA;
        if (v >= 1 && v <= 0x18) {
            self->mField5C->mField3F60->mField4EC |= 4;
        } else if (getInstance__Q22cf14CBattleManagerFv()->mField20C8 != 0) {
            self->mField5C->mField3F60->mField4EC |= 4;
        } else {
            self->mField5C->mField3F60->mField4EC &= ~4;
        }

        u32 val1 = *self->mField5C->mField4->vf30();
        if (func_80174C98(self->mField5C, &val1, 0x803) != 0 &&
            (self->mField5C->mField3F60->mFieldC & 4) != 0) {
            u32 val2 = *self->mField5C->mField4->vf30();
            if (func_80174C98(self->mField5C, &val2, 0x19) == 0) {
                u32 val3 = *self->mField5C->mField4->vf30();
                if (func_80174C98(self->mField5C, &val3, 0x1A) == 0) {
                    self->mField5C->mField3F60->mField4EC |= 8;
                    goto clearDone;
                }
            }
        }
        self->mField5C->mField3F60->mField4EC &= ~8;
    clearDone:;
    }
}

void CtrlPc_emptyStub_97DFC() {}

// Retail func_80097E00 (0x800987D8): idle/busy probe for the player pad
// handler. Returns 1 when the player is in a usable (non-busy) state, else 0.
int func_80097E00(cf::CtrlPc* self) {
    u32 battleState = getInstance__Q22cf14CBattleManagerFv()->mField1AA;
    u32 chainActive;
    int inBattle = 0;
    if (battleState >= 1 && 0x18 >= battleState) {
        inBattle = 1;
    }
    chainActive = (getInstance__Q22cf14CBattleManagerFv()->mField20C8 != 0);
    cf::CfGameManager::getInstance();
    if (func_8006EF04__Fi(0x4000000) != 0) {
        return 0;
    }
    void* owner = self->mField5C;
    if (owner != 0) {
        owner = &((CtrlPlayerObj*)owner)->mSub3E9C;
    }
    if (owner != (void*)cf::CfGameManager::getPlayer(0)) {
        return 0;
    }
    if (func_800FEDF8() != 0) {
        return 0;
    }
    if (self->mField5C->mSub3ED4->vf14(0x400) != 0 && inBattle == 0 &&
        chainActive == 0) {
        return 0;
    }
    if (self->mField5C->mSub3ED4->vf14(0x40000) != 0) {
        return 0;
    }

    // All actor-id flags must be clear for the player to be idle; each probe
    // uses its own stack word (retail keeps them at distinct offsets).
    {
        CtrlPlayerObj* p = self->mField5C;
        u32 v = *p->mField4->vf30();
        if (func_80174C98(p, &v, 3) != 0) goto busy;
    }
    {
        CtrlPlayerObj* p = self->mField5C;
        u32 v = *p->mField4->vf30();
        if (func_80174C98(p, &v, 0xA) != 0) goto busy;
    }
    {
        CtrlPlayerObj* p = self->mField5C;
        u32 v = *p->mField4->vf30();
        if (func_80174C98(p, &v, 0xB) != 0) goto busy;
    }
    {
        CtrlPlayerObj* p = self->mField5C;
        u32 v = *p->mField4->vf30();
        if (func_80174C98(p, &v, 9) != 0) goto busy;
    }
    {
        CtrlPlayerObj* p = self->mField5C;
        u32 v = *p->mField4->vf30();
        if (func_80174C98(p, &v, 6) != 0) goto busy;
    }
    {
        CtrlPlayerObj* p = self->mField5C;
        u32 v = *p->mField4->vf30();
        if (func_80174C98(p, &v, 0x14) != 0) goto busy;
    }
    {
        CtrlPlayerObj* p = self->mField5C;
        u32 v = *p->mField4->vf30();
        if (func_80174C98(p, &v, 0x1F) != 0) goto busy;
    }
    {
        CtrlPlayerObj* p = self->mField5C;
        u32 v = *p->mField4->vf30();
        if (func_80174C98(p, &v, 0x12) != 0) goto busy;
    }
    return 0;

busy:
    if (func_80148778(&self->mField5C->mField8, 0xF) != 0) {
        return 0;
    }
    {
        CtrlPlayerObj* p = self->mField5C;
        u32 v = *p->mField4->vf30();
        if (func_80174C98(p, &v, 1) != 0) {
            return 0;
        }
    }
    {
        CtrlPlayerObj* p = self->mField5C;
        u32 v = *p->mField4->vf30();
        return func_80174C98(p, &v, 2) == 0;
    }
}

void func_80098194(){}


// Retail func_800983B8 (0x80098D90): pad-action dispatcher. arg selects the
// action family: 5 = install an AI-action slot on the player (combo-range
// dependent), 6 = clear a chain-state flag via the battle manager, 1 = push
// a voice/battle status on the player's owner interface.
void func_800983B8(cf::CtrlPc* self, char arg) {
    int mode;
    if (arg == 5) {
        CVisionFxParam p;
        u32 val;
        // Two separate zeroing calls on the slot: an 0xE-byte prefix write
        // (bytes 4..0x11) followed by the full 0x20-byte clear.
        memset(&p.unk0[4], 0, 0xE);
        memset(&p, 0, sizeof(p));
        mode = (getInstance__Q22cf14CBattleManagerFv()->mField20C8 != 0) ? 0x1000 : 4;
        if (self->mField5C->mField3F28 == 4) {
            if (self->mField5C->vf164()->mField48 >= 7 && self->mField5C->vf164()->mField48 <= 9) {
                val = self->mField5C->vf164()->mField48;
                if (self->mField5C->mField3F60 != 0) {
                    func_8004DACC(self->mField5C->mField3F60);
                }
                p.b_0D = (u8)(val - 1);
                p.h_12 = 0;
                p.f_14 = lbl_eu_80666720;
                p.h_10 = mode;
                if ((mode & 0x200) == 0) {
                    p.b_06 = 6;
                } else {
                    p.b_06 = 0x25;
                }
                func_8014AC38(&self->mField5C->mField3380, &p);
            } else {
                p.b_0D = 5;
                p.h_12 = 0;
                p.f_14 = lbl_eu_80666720;
                p.h_10 = mode;
                if ((mode & 0x200) == 0) {
                    p.b_06 = 6;
                } else {
                    p.b_06 = 0x25;
                }
                func_8014AC38(&self->mField5C->mField3380, &p);
            }
        } else if (self->mField5C->mField3F28 == 7) {
            CtrlPlayerSub298* sub = self->mField5C->vf164();
            sub->mField7C = 0;
            p.b_0D = 5;
            p.h_12 = 0;
            p.f_14 = lbl_eu_80666720;
            p.h_10 = mode;
            if ((mode & 0x200) == 0) {
                p.b_06 = 6;
            } else {
                p.b_06 = 0x25;
            }
            func_8014AC38(&self->mField5C->mField3380, &p);
        } else {
            p.b_0D = 5;
            p.h_12 = 0;
            p.f_14 = lbl_eu_80666720;
            p.h_10 = mode;
            if ((mode & 0x200) == 0) {
                p.b_06 = 6;
            } else {
                p.b_06 = 0x25;
            }
            func_8014AC38(&self->mField5C->mField3380, &p);
        }
        return;
    }
    if (arg == 6) {
        func_8027936C(&getInstance__Q22cf14CBattleManagerFv()->mField1A8, 0);
        return;
    }
    if (arg == 1) {
        CtrlPlayerSub3ED4Cast* casted = (CtrlPlayerSub3ED4Cast*)__dynamic_cast(
            self->mField5C->mSub3ED4, 0, &lbl_eu_80661C60, &lbl_eu_80661BE8, 0);
        if (casted != 0) {
            casted->vf60(self->mField5C->mSub3E9C.v17());
            func_800BE12C(&self->mField5C->mSub3E9C, 0x1B, 0, 0x63, 1);
        }
    }
}

// Retail func_80098694 (0x8009906C): build an enum list of talk-source ids
// filtered to 0x80000002, then scan for the first source whose voice handle
// is idle; returns that source (or 0).
void* func_80098694(cf::CtrlPc* self) {
    const f32 radius = lbl_eu_80666724;
    CfEnumListHolder holder;
    func_80043D90(&holder);
    func_800F4A98(func_80043F18(&holder), 0x80000002, 0);
    CVoicePos* pos = self->mField5C->mSub3E9C.getPosition();
    __ct__800FB044(func_80043F18(&holder), radius, pos, 0);
    void* elem;
    u32 i;
    for (i = 0; i < func_80043F18(&holder)->count; i++) {
        elem = func_800F6E98(func_80043F18(&holder), i);
        CVoiceHandleVTV* src = (CVoiceHandleVTV*)func_8016FE34(
            (void*)func_800B708C__Fi((int)(intptr_t)elem));
        if (src->isActive() == 0) {
            __dt__80043E88(&holder, -1);
            return elem;
        }
    }
    __dt__80043E88(&holder, -1);
    return 0;
}

int func_800987A0(cf::CtrlPc* self, ml::CVec3* out, u8* outFlag,
                  const ml::CVec3* src, int flag, int cond) {
    if (cond != 0) {
        *outFlag = 0;
        func_80089398(reinterpret_cast<cf::CCtrlMoveBase*>(self->mSubObj8C), out, src, flag);
        return 1;
    }
    return func_800890A8(reinterpret_cast<cf::CCtrlMoveBase*>(self->mSubObj8C), out, outFlag, src, flag);
}

void cf::CtrlPc::delegateTo899C0() {
    extern void func_800899C0(void*, float);
    extern float lbl_eu_80666728;
    func_800899C0(mSubObj8C, lbl_eu_80666728);
}

void func_80089E88(void* self);
void cf::CtrlPc::delegateTo89E88() { func_80089E88(mSubObj8C); }

void cf::CtrlPc::delegateTo89F68() { func_80089F68(mSubObj8C); }

void func_80098810(){}

void cf::CAttackParam::CAttackParam_UnkVirtualFunc3(u8 val) { unk2A = val; }
u8 cf::CAttackParam::CAttackParam_UnkVirtualFunc2() { return unk2A; }

// Retail func_80098A04 (0x800993DC): pad-state maintenance. Reads the
// player's combo sub-object (vtable slot 0x2A4) and the pad-config id at
// +0x3F28; writes mField24 (-1/1) and the sub-object's +0x4C state word.
void func_80098A04(cf::CtrlPc* self) {
    CtrlPlayerSub2A4* sub = self->mField5C->vf167();
    CtrlPlayerSub50* sub50 = sub->mField50;
    u32 id = self->mField5C->mField3F28;
    if (id == 2) {
        self->mField24 = -1;
        if ((sub->mField78 & 0x800) != 0 && sub50->mField77 == 0x10 &&
            self->mField5C->mField1530 != 0) {
            self->mField24 = 1;
            sub->mField4C = 1;
        }
    } else if (id == 5) {
        u32 f5 = sub->mField78;
        if ((f5 & 0x800) == 0 && (f5 & 0x400) != 0) {
            u8 b = sub50->mField77;
            if (b == 0 || (b - 2) <= 1u) {
                if (sub->mField4 == self->mField5C->mField3F10) {
                    self->mField24 = 1;
                    sub->mField4C = 1;
                } else {
                    self->mField24 = -1;
                    sub->mField4C = -1;
                }
            }
        }
    }
}

// Retail func_80098AF0 (0x800994C8): returns the pad object to use - the
// dummy pad when the player's +0x3ED4 sub-object vtable[0x40] probe succeeds
// or the 0x10000000 mode flag is set, otherwise the cached pad at +0x1D4.
CPad* func_80098AF0(cf::CtrlPc* self) {
    CtrlPlayerObj* player = self->mField5C;
    int flag = 0x800;
    if (player->mSub3ED4->vf14(flag) != 0) {
        return CPadManager::getDummyPad();
    }
    cf::CfGameManager::getInstance();
    if (func_8006EF04__Fi(0x10000000) != 0) {
        return CPadManager::getDummyPad();
    }
    return (CPad*)self->mField1D4;
}

u32 cf::CtrlPc::getField1D4() { return mField1D4; }
int cf::CtrlPc::returnTrue() { return 1; }

u32 func_80098B74(int index) {
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        return lbl_eu_80527F10[index];
    }
    return lbl_eu_80527E98[index];
}

extern "C" void CtrlPc_setArrayVal(int idx, int val)
{
    lbl_eu_80527E98[idx] = val;
}

// Bit-test helpers: lwz r0,0x2c(r3); extrwi r3,r0,1,b (b = 12/13/14 -> bits 19/18/17).
u32 func_80096F18(cf::CtrlPc* self) { return (self->mPadFlags >> 19) & 0x1u; }
u32 func_80096F24(cf::CtrlPc* self) { return (self->mPadFlags >> 18) & 0x1u; }
u32 func_80096F30(cf::CtrlPc* self) { return (self->mPadFlags >> 17) & 0x1u; }
// Bit-test helpers (series continues func_80096EE8..80096F30): retail is
// lwz r0,0x2c(r3); extrwi r3,r0,1,b -> (mPadFlags>>(31-b))&1.
u32 func_80096F48(cf::CtrlPc* self) { return (self->mPadFlags >> 15) & 0x1u; } // extrwi 1,16

u32 func_80096F54(cf::CtrlPc* self) { return (self->mPadFlags >> 14) & 0x1u; } // extrwi 1,17

u32 func_80096F60(cf::CtrlPc* self) { return (self->mPadFlags >> 13) & 0x1u; } // extrwi 1,18

u32 func_80096F78(cf::CtrlPc* self) { return (self->mPadFlags >> 11) & 0x1u; } // extrwi 1,20

u32 func_80096F84(cf::CtrlPc* self) { return (self->mPadFlags >> 10) & 0x1u; } // extrwi 1,21

u32 func_80096F90(cf::CtrlPc* self) { return (self->mPadFlags >> 9) & 0x1u; }

// 0x380 field setter: stw r4,0x380(r3)
void func_8009868C(cf::CtrlPc* self, u32 val) { self->mField380 = val; }

// Tail-call thunk into the move sub-object at +0x8C: addi r3,r3,0x8c; b func_80089B24
int func_800987F8(cf::CtrlPc* self, ml::CVec3* out) {
    return func_80089B24(reinterpret_cast<cf::CCtrlMoveBase*>(self->mSubObj8C), out);
}
