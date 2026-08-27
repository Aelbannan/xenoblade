// Auto-scaffolded catalog TU for kyoshin/menu/CMenuBattleDamage
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuBattleDamage.hpp"

#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include <nw4r/lyt.h>
#include <nw4r/math.h>

#include "kyoshin/menu/CMenuBattleDamage.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include <nw4r/lyt.h>
#include <nw4r/math.h>
#include <revolution/GX.h>
#include <stdio.h>
#include <string.h>

#include "monolib/util/FixStr.hpp"

extern "C" void* __dt__17CMenuBattleDamageFv(void*, int);

// Defined below in this TU (damage-slot enqueue; also called by func_80109734).
extern "C" void func_801098B0(CMenuBattleDamage* self, int actorId,
                              int value, u32 flags);

// Unmangled retail ctor symbol; takes the owning scene (stored at +0x60).
// Zero-fills the entries twice: first directly, then by copying a
// zero-initialized stack local into each entry (retail order).
extern "C" CMenuBattleDamage* __ct__CMenuBattleDamage(
    CMenuBattleDamage* obj, CScn* scene) {
    char* vtFinal;
    char* vtWork;
    char* vtRender;
    u32 ptmfWord0;
    u32 ptmfWord1;
    u32 ptmfWord2;
    u32 z;

    __ct__8CProcessFv(reinterpret_cast<CProcess*>(obj));

    obj->mProcessVt = reinterpret_cast<u32>(lbl_eu_8052C1C0);
    vtFinal = lbl_eu_8052C230;
    ptmfWord0 = lbl_eu_80535AD0[0];
    vtWork = vtFinal + 0x24;
    vtRender = vtFinal + 0xac;
    z = 0;
    ptmfWord1 = lbl_eu_80535AD0[1];
    obj->ptmfMove[1] = ptmfWord1;
    obj->ptmfMove[0] = ptmfWord0;
    ptmfWord2 = lbl_eu_80535AD0[2];
    obj->ptmfMove[2] = ptmfWord2;
    ptmfWord0 = lbl_eu_80535AD0[0];
    ptmfWord1 = lbl_eu_80535AD0[1];
    obj->ptmfDraw[1] = ptmfWord1;
    obj->ptmfDraw[0] = ptmfWord0;
    ptmfWord2 = lbl_eu_80535AD0[2];
    obj->ptmfDraw[2] = ptmfWord2;
    obj->mField_54 = (u8)z;
    obj->mField_55 = (u8)z;
    obj->mProcessVt = reinterpret_cast<u32>(vtFinal);
    obj->mIWorkEventVt = reinterpret_cast<u32>(vtWork);
    obj->mIScnRenderVt = reinterpret_cast<u32>(vtRender);
    obj->mScn = scene;
    __ct__17UnkClass_8045F564Fv(&obj->mMemRegion);

    const f32 zeroF = lbl_eu_80666F68;
    for (u8 i = 0; i < 0x20; i++) {
        CMenuBattleDamageEntry& e = obj->mEntries[i];
        e.mLayout0 = NULL;
        e.field_04 = 0;
        e.mAnim0 = NULL;
        e.mAnim1 = NULL;
        e.mAnim2 = NULL;
        e.mLayout1 = NULL;
        e.mAnim3 = NULL;
        e.mActive = 0;
        e.field_20 = 0;
        e.field_24 = 0;
        e.field_28 = zeroF;
        e.field_2C = zeroF;
        e.field_30 = 0;
        e.field_31 = 0;
        e.field_32 = 0;
        e.field_34 = zeroF;
    }
    obj->mDamageType = (u8)z;
    obj->mDamageDir = (u8)z;
    obj->_pad776[0] = (u8)z;
    obj->_pad776[1] = (u8)z;
    obj->_pad776[2] = (u8)z;
    obj->field_0x779 = (u8)z;

    // Pass 2: retail assigns an explicit stack local, then copies it into
    // each entry (MWCC keeps the dead local stores alongside the copies).
    for (u8 i = 0; i < 0x20; i++) {
        CMenuBattleDamageEntry e;
        e.mLayout0 = NULL;
        e.field_04 = 0;
        e.mAnim0 = NULL;
        e.mAnim1 = NULL;
        e.mAnim2 = NULL;
        e.mLayout1 = NULL;
        e.mAnim3 = NULL;
        e.mActive = 0;
        e.field_20 = 0;
        e.field_24 = 0;
        e.field_28 = lbl_eu_80666F68;
        e.field_2C = lbl_eu_80666F68;
        e.field_30 = 0;
        e.field_31 = 0;
        e.field_32 = 0;
        e.field_34 = lbl_eu_80666F68;
        obj->mEntries[i] = e;
    }
    return obj;
}

// The retail dtor is the full implicit D1 of the real class (CProcess +
// IWorkEvent/IScnRender bases + mMemRegion member). The header models the
// bases as storage, so the symbol is defined as a freestanding extern "C"
// function with the exact retail structure: member (flag -1) first, then the
// base with a NON-deleting flag 0 behind the D2-inlined-into-D1 double
// null-check (retail cmpi r30,0 x2), then delete-on-flag, returning self.
// The C++ dtor stays declared (header) so the `this->~CMenuBattleDamage()`
// thunk calls resolve to this symbol at link time.
extern "C" void* __dt__17CMenuBattleDamageFv(void* self, int flag) {
    CMenuBattleDamage* this_ = reinterpret_cast<CMenuBattleDamage*>(self);
    if (this_ != 0) {
        __dt__17UnkClass_8045F564Fv(&this_->mMemRegion, -1);
        if (this_ != 0) {
            if (this_ != 0) {
                __dt__8CProcessFv(reinterpret_cast<CProcess*>(this_), 0);
            }
        }
        if (flag > 0) {
            ::operator delete(this_);
        }
    }
    return self;
}

void CMenuBattleDamage::Init() {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    mMemRegion.createRegion(handle, 0x53000, lbl_eu_804FD524, 0);
    Class_8045F858 scoped(&mMemRegion);

    for (u8 i = 0; i < 0x20; i++) {
        CMenuBattleDamageEntry& e = mEntries[i];
        nw4r::lyt::ArcResourceAccessor* acc;

        acc = func_801355F4();
        func_80136E84(&e.mLayout0, acc, lbl_eu_804FD524 + 0x12);
        acc = func_801355F4();
        func_80136F08(e.mLayout0, &e.mAnim0, acc, lbl_eu_804FD524 + 0x2d);
        acc = func_801355F4();
        func_80136F08(e.mLayout0, &e.mAnim1, acc, lbl_eu_804FD524 + 0x4c);
        acc = func_801355F4();
        func_80136F08(e.mLayout0, &e.mAnim2, acc, lbl_eu_804FD524 + 0x6b);

        {
            nw4r::lyt::Pane* root = e.mLayout0->GetRootPane();
            u32 font = func_801355A0();
            func_8013676C(root, font);
        }
        e.mLayout0->UnbindAllAnimation();

        acc = func_801355F4();
        func_80136E84(&e.mLayout1, acc, lbl_eu_804FD524 + 0x8a);
        acc = func_801355F4();
        func_80136F08(e.mLayout1, &e.mAnim3, acc, lbl_eu_804FD524 + 0xa2);

        {
            nw4r::lyt::Pane* root = e.mLayout1->GetRootPane();
            CMenuBattleDamageFontView* fontObj =
                static_cast<CMenuBattleDamageFontView*>(
                    getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
                        1, e.mLayout1));
            func_8013676C(root, fontObj->sf7());
        }

        e.mLayout1->SetAnimationEnable(e.mAnim3, true);
        e.mAnim3->SetFrame(lbl_eu_80666F68);
        e.mLayout1->Animate(0);
    }

    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        cb = reinterpret_cast<IScnRender*>(&mIScnRenderVt);
    }
    mScn->addRenderCB(cb, 0x9, 0x0);
    mMemRegion.func_8045F810();
}

// Tear down the battle-damage singleton: detach the render callback, destroy
// both per-entry layouts (32 entries), release the scratch region and clear
// the singleton pointer.
void CMenuBattleDamage::Term() {
    CDeviceVI::waitForDrawDone();
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        render = reinterpret_cast<IScnRender*>(&mIScnRenderVt);
    }
    mScn->removeRenderCB(render);
    for (u8 i = 0; i < 0x20; i++) {
        if (mEntries[i].mLayout0 != NULL) {
            delete mEntries[i].mLayout0;
            mEntries[i].mLayout0 = NULL;
        }
        if (mEntries[i].mLayout1 != NULL) {
            delete mEntries[i].mLayout1;
            mEntries[i].mLayout1 = NULL;
        }
    }
    mMemRegion.func_8045F778();
    lbl_eu_80663F28 = NULL;
}

// Per-frame damage-popup update: for each active entry, project the actor's
// position to screen space, move the popup and advance/expire its animation.
void CMenuBattleDamage::Move() {
    // Retail gate: `bne done` (guard 1) then the branch-over-branch
    // `beq body; b done` (guard 2) - reproduced by the ||-chain early return
    // (same shape as the matched CMenuLvUp::Move).
    if (CTaskGame::getInstance()->isFlag01Set() ||
        (lbl_eu_80663E28 & 0x200000))
        return;
    if (!func_8013BE50()) return;
    if (_pad776[0] == 0) return;

    // Constant pool values cached in callee-saved FPRs (retail f29..f31,
    // f26..f28 load order).
    const f32 xoff = lbl_eu_80666F70;
    const f32 one = lbl_eu_80666F6C;
    const f32 zero = lbl_eu_80666F68;
    const f32 dirmod = lbl_eu_80666F7C;
    const f32 yscale = lbl_eu_80666F78;
    const f32 yoff = lbl_eu_80666F74;

    for (u8 i = 0; i < 0x20; i++) {
        CMenuBattleDamageEntry& e = mEntries[i];
        if (e.mActive == 0) {
            continue;
        }

        // MWCC allocates locals in reverse declaration order to ascending
        // slots: screen +0x20, world +0x14, tmp +0x08.
        nw4r::math::VEC3 screen; // 0x20
        nw4r::math::VEC3 world;  // 0x14
        nw4r::math::VEC3 tmp;    // 0x08 (vfn12C strided components)
        screen.y = e.field_2C;
        screen.x = e.field_28;
        screen.z = zero;

        CMenuBattleDamageActor* actor = findObjectById((int)e.field_24);
        if (actor != 0) {
            const nw4r::math::VEC3* src;
            CMenuBattleDamageObjPos* obj = actor->vfn12C(0xfa);
            if (obj != 0) {
                // Load the strided components in retail's z,y,x order.
                tmp.x = obj->field_0x0c;
                tmp.y = obj->field_0x1c;
                tmp.z = obj->field_0x2c;
                src = &tmp;
            } else {
                src = actor->vfnAC();
            }
            world.x = src->x;
            world.y = src->y;
            world.z = src->z;
            if (obj == 0) {
                world.y += one;
            }

            CMenuBattleDamagePose* pose = func_80496264(mScn, -1);
            func_8049B59C(&screen, pose, &world);

            screen.z = zero;
            screen.y = (screen.y - yoff) * yscale;
            screen.x = screen.x - xoff;

            if (e.field_30 == 1) {
                screen.x -= dirmod;
            } else if (e.field_30 == 2) {
                screen.x += dirmod;
            }
            e.field_28 = screen.x;
            // Retail quirk: the y used for the pane translate is re-read
            // from the entry, discarding the projected value.
            screen.y = e.field_2C;
        }

        nw4r::lyt::AnimTransform* anim;
        nw4r::lyt::Layout* layout;
        if (e.field_32 != 0) {
            layout = e.mLayout0;
            anim = e.field_04;
        } else {
            layout = e.mLayout1;
            anim = e.mAnim3;
        }

        layout->GetRootPane()->SetTranslate(screen);

        if (e.field_34 >= zero) {
            // Count down the popup lifetime.
            e.field_34 -= one;
        } else {
            // Lifetime expired: show the pane, and free the slot once the
            // anim finishes. Retail re-derives the root pane here.
            layout->GetRootPane()->SetVisible(true);
            if (func_80137444(anim, lbl_eu_80666F6C) != 0) {
                e.mActive = 0;
                _pad776[0] -= 1;
            }
        }
        layout->Animate(0);
    }

    if (mField_55 != 0) {
        mField_54 = 1;
    }
}

void CMenuBattleDamage::cbRenderBefore() {
    CTaskGame::getInstance();
    // Retail: guard 1 folds to `bne done`; guard 2 (rlwinm. bit test) is the
    // branch-over-branch `beq body; b done` - reproduced by the &&-chain
    // gate (first condition folds, second becomes the gate).
    if (CTaskGame::isFlag01Set() == 0 &&
        (lbl_eu_80663E28 & 0x200000) == 0) {
        goto body;
    }
    goto done;
done:
    return;
body:
    if (!func_8013BE50()) {
        goto done;
    }
    if (_pad776[0] == 0) {
        goto done;
    }

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    func_80137250(&drawInfo);

    // Draw order: start with 0..31, then bubble-sort active entries to
    // the front (by ascending field_31), leaving inactive entries last.
    u8 order[0x20];
    for (u8 i = 0; i < 0x20; i++) {
        order[i] = i;
    }
    for (u8 i = 0; i < 0x1f; i++) {
        u8 swapped = 0;
        for (u8 j = 0; j < 0x1f - i; j++) {
            u8 a = order[j];
            u8 b = order[j + 1];
            CMenuBattleDamageEntry& ea = mEntries[a];
            CMenuBattleDamageEntry& eb = mEntries[b];
            if (ea.mActive != 0) {
                if (eb.mActive != 0) {
                    if (ea.field_31 > eb.field_31) {
                        // u8 temp swap: MWCC emits the retail xor-swap form
                        u8 tmp = order[j];
                        order[j] = order[j + 1];
                        order[j + 1] = tmp;
                        swapped = 1;
                    }
                }
            } else if (eb.mActive != 0) {
                u8 tmp = order[j];
                order[j] = order[j + 1];
                order[j + 1] = tmp;
                swapped = 1;
            }
        }
        if (swapped == 0) {
            break;
        }
    }

    for (u8 i = 0; i < 0x20; i++) {
        CMenuBattleDamageEntry& e = mEntries[order[i]];
        if (e.mActive == 0) {
            break;
        }
        nw4r::lyt::Layout* layout =
            (e.field_32 != 0) ? e.mLayout0 : e.mLayout1;
        func_80137038(layout, &drawInfo, 0, 1);
    }
}

void createBattleDamage() {}

void addBattleDamage() {}

void setDamageType(unsigned char val) {
    extern CMenuBattleDamage* lbl_eu_80663F28;
    CMenuBattleDamage* p = lbl_eu_80663F28;
    if (p != 0) p->mDamageType = val;
}

void setDamageDir(unsigned char val) {
    extern CMenuBattleDamage* lbl_eu_80663F28;
    CMenuBattleDamage* p = lbl_eu_80663F28;
    if (p != 0) p->mDamageDir = val;
}

void addBattleDamageEx() {}

void setDamageStyle() {}

void setDamageFlag1() {}

void setDamageFlag2() {}

void addDamageSlot() {}

void CMenuBattleDamage::thunk_IWorkEvent_dtor() {
    this->~CMenuBattleDamage();
}

void CMenuBattleDamage::thunk_IScnRender_cbRenderBefore() {
    this->cbRenderBefore();
}

void CMenuBattleDamage::thunk_IScnRender_dtor() {
    this->~CMenuBattleDamage();
}

CPcSelectCursor01::~CPcSelectCursor01() {}

void CPcSelectCursor01::cursorInit() {
    if (this->mState != 0) {
        return;
    }
    this->mState = 1;
    this->mAnimFrame = 0;
}

void cursorMoveRight() {}

void cursorMoveLeft() {}

u8 CPcSelectCursor01::cursorGetAnimFrame() { return this->mAnimFrame; }

void cursorShow() {}

void cursorHide() {}

void cursorAnimate() {}

void cursorFinalize() {}

extern CMenuBattleDamage* lbl_eu_80663F28;

extern "C" void func_80109874(u8 val) { CMenuBattleDamage* g = lbl_eu_80663F28; if (g) *(u8*)((u8*)g + 0x777) = val; }
extern "C" void func_80109888(u8 val) { CMenuBattleDamage* g = lbl_eu_80663F28; if (g) *(u8*)((u8*)g + 0x778) = val; }
extern "C" void func_8010989C(u8 val) { CMenuBattleDamage* g = lbl_eu_80663F28; if (g) *(u8*)((u8*)g + 0x779) = val; }

// Battle-damage singleton factory. Returns null when one already exists;
// otherwise allocates the 0x77c-byte object from the work-thread region,
// constructs it with the owning scene, registers it under `parent` and
// returns the singleton.
CMenuBattleDamage* func_801096B8(CProcess* parent, CScn* scene) {
    if (lbl_eu_80663F28 != 0) {
        return 0;
    }
    CMenuBattleDamage* obj = (CMenuBattleDamage*)mtl::MemManager::allocate(
        0x77c, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        // The retail ctor returns `this` in r3, so threading the return value
        // back into obj keeps it live in r3 across the call (no spill).
        obj = __ct__CMenuBattleDamage(obj, scene);
    }
    lbl_eu_80663F28 = obj;
    Regist__8CProcessFP8CProcessb(reinterpret_cast<CProcess*>(obj), parent,
                                  false);
    return lbl_eu_80663F28;
}

extern "C" void func_80109734(int actorId, int value) {
    CMenuBattleDamage* g = lbl_eu_80663F28;
    if (g) {
        func_801098B0(g, actorId, value, 0);
    }
}
extern "C" void func_8010975C(u8 val) { CMenuBattleDamage* g = lbl_eu_80663F28; if (g) *(u8*)((u8*)g + 0x774) = val; }
extern "C" void func_80109784(int actorId, int value, u32 flags) {
    if (lbl_eu_80663F28 == NULL) {
        return;
    }
    if (!func_8013BFD8()) {
        return;
    }
    if (flags == 0) {
        return;
    }
    if (flags & 0x8) {
        char* s = func_8013639C(reinterpret_cast<const void*>(lbl_eu_80664160),
                                lbl_eu_804FD524 + 0xc4, value);
        if (strcmp(s, lbl_eu_804FD524 + 0xc9) == 0) {
            return;
        }
    } else if ((flags & 1) != 0 || (flags & 2) != 0 ||
               (flags & 0x20) != 0 || (flags & 4) != 0) {
        const char* s = func_80145AA8(value);
        if (strcmp(s, lbl_eu_804FD524 + 0xc9) == 0) {
            return;
        }
    }
    func_801098B0(lbl_eu_80663F28, actorId, value, flags);
}

// Cursor state machine: state 2 -> 3 (damage-up animation), enabling the
// third anim and disabling the first two.
void func_8010A710(CPcSelectCursor01* self) {
    if (self->field_0x2C != 2) {
        return;
    }
    self->field_0x2C = 3;
    self->field_0x28 = 0;
    self->mLayout->SetAnimationEnable(self->field_0x20, false);
    self->mLayout->SetAnimationEnable(self->field_0x24, false);
    self->mLayout->SetAnimationEnable(self->mAnim1, true);
}

// Cursor state machine: state 2 -> 4, enabling the second anim and
// disabling the others.
void func_8010A7A8(CPcSelectCursor01* self) {
    if (self->field_0x2C != 2) {
        return;
    }
    self->field_0x2C = 4;
    self->field_0x28 = 0;
    self->mLayout->SetAnimationEnable(self->field_0x24, false);
    self->mLayout->SetAnimationEnable(self->mAnim1, false);
    self->mLayout->SetAnimationEnable(self->field_0x20, true);
}

extern "C" void func_8010A67C(void* self) { ((void(*)(void*))__dt__17CMenuBattleDamageFv)((char*)self - 0x58); }

extern "C" void func_8010A68C(void* self) { ((void(*)(void*))__dt__17CMenuBattleDamageFv)((char*)self - 0x5c); }

// Damage-number text: store the value, look up the "N_Damage" pane on the
// layout's root and place it at (-178, 110 - 86*value, 0).
void func_8010A848(CPcSelectCursor01* self, u8 value) {
    self->field_0x29 = value;
    nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(
        lbl_eu_804FD720 + 0x95, true);
    // Inline VEC3 args so MWCC matches retail lfs/fnmsubs/stfs order.
    pane->SetTranslate(nw4r::math::VEC3(
        lbl_eu_80666FA0,
        lbl_eu_80666F98 - lbl_eu_80666F9C * static_cast<f32>(self->field_0x29),
        lbl_eu_80666F94));
}

extern "C" void func_8010A8E4(CPcSelectCursor01* self) {
    self->field_0x2C = 0;
    self->mAnim1->SetFrame(lbl_eu_80666F94);
    self->mLayout->Animate();
    self->field_0x28 = 1;
}
// Damage-number enqueue: compact the non-empty digit slots of the queue to
// the front (via a stack buffer), then append the new digit triple and mark
// the queue filled.
void func_8010A940(CMenuBattleDamageQueue* self, u32 v0, u32 v1, u32 v2) {
    CMenuBattleDamageSlot stack[0x20] = {};
    u8 dst = 0;
    for (u8 i = 0; i < 0x20; i++) {
        CMenuBattleDamageSlot& s = self->mSlots[i];
        if (s.mVal0 != 0 || s.mVal1 != 0 || s.mVal2 != 0) {
            stack[dst] = s;
            s.mVal0 = 0;
            s.mVal1 = 0;
            s.mVal2 = 0;
            dst++;
        }
    }
    if (dst < 0x20) {
        // Retail compiles this copy loop into the unrolled 8-slot bulk +
        // 1-slot tail shape (no memcpy call).
        for (int j = 0; j < dst; j++) {
            self->mSlots[j] = stack[j];
        }
        self->mSlots[dst].mVal0 = v0;
        self->mSlots[dst].mVal1 = v1;
        self->mSlots[dst].mVal2 = v2;
        if (self->mFlag == 0) {
            self->mFlag = 2;
        }
    }
}
// Draw one queued damage number: find the first non-empty digit slot, write
// its three digits into the layout panes, play the hit sound and clear it.
// If every slot is empty, mark the queue idle.
extern "C" void func_8010ACC4(CMenuBattleDamageQueue* self) {
    for (u8 i = 0; i < 0x20; i++) {
        CMenuBattleDamageSlot& s = self->mSlots[i];
        if (s.mVal0 == 0 && s.mVal1 == 0 && s.mVal2 == 0) {
            continue;
        }
        self->mFlag = 1;
        func_80136910__FPQ34nw4r3lyt6LayoutPcUc(self->mLayout,
                                                lbl_eu_804FD720 + 0xe1,
                                                s.mVal0);
        func_80136910__FPQ34nw4r3lyt6LayoutPcUc(self->mLayout,
                                                lbl_eu_804FD720 + 0xe9,
                                                s.mVal1);
        func_80136910__FPQ34nw4r3lyt6LayoutPcUc(self->mLayout,
                                                lbl_eu_804FD720 + 0xf0,
                                                s.mVal2);
        s.mVal0 = 0;
        s.mVal1 = 0;
        s.mVal2 = 0;
        func_80138078(0xad);
        return;
    }
    self->mFlag = 0;
}

// Spawn a battle-damage popup. Finds a free slot (first 8 only), picks the
// next z-order, projects the actor's position to screen space, then builds
// either the big damage layout (flags == 0) or the small label layout
// (flags != 0), chaining the animation start frame to a previous entry of
// the same actor so popups don't overlap.
void func_801098B0(CMenuBattleDamage* self, int actorId, int value,
                    u32 flags) {
    s8 found = -1;
    for (u8 i = 0; i < 8; i++) {
        if (self->mEntries[i].mActive == 0) {
            found = (s8)i;
            break;
        }
    }
    if (found < 0) {
        return;
    }

    // Count live same-action actors and the highest z-order in use.
    u32 aliveCount = 0;
    u8 maxZ = 0;
    for (u8 i = 0; i < 0x20; i++) {
        CMenuBattleDamageEntry& o = self->mEntries[i];
        if (o.mActive != 0) {
            CMenuBattleDamageActor* a = findObjectById((int)o.field_24);
            if (a != 0 && (a->field_0x64 & 2) != 0) {
                aliveCount++;
            }
        }
        if (o.field_31 > maxZ) {
            maxZ = o.field_31;
        }
    }

    CMenuBattleDamageEntry& e = self->mEntries[found];
    e.mActive = 1;
    e.field_20 = value;
    e.field_24 = actorId;
    e.field_30 = self->mDamageDir;
    e.field_31 = maxZ + 1;

    nw4r::math::VEC3 tmp;    // 0x10 (vfn12C strided components)
    nw4r::math::VEC3 world;  // 0x1c
    nw4r::math::VEC3 screen; // 0x28
    screen.x = lbl_eu_80666F68;
    screen.y = lbl_eu_80666F68;
    screen.z = lbl_eu_80666F68;

    CMenuBattleDamageActor* actor = findObjectById((int)e.field_24);
    u32 dirFlag = 0;
    if (actor != 0) {
        const nw4r::math::VEC3* src;
        CMenuBattleDamageObjPos* obj = actor->vfn12C(0xfa);
        if (obj != 0) {
            // Load the strided components in retail's z,y,x order.
            f32 az = obj->field_0x2c;
            f32 ay = obj->field_0x1c;
            f32 ax = obj->field_0x0c;
            tmp.x = ax;
            tmp.y = ay;
            tmp.z = az;
            src = &tmp;
        } else {
            src = actor->vfnAC();
        }
        world.x = src->x;
        world.y = src->y;
        world.z = src->z;
        if (obj == 0) {
            world.y += lbl_eu_80666F6C;
        }

        CMenuBattleDamagePose* pose = func_80496264(self->mScn, -1);
        func_8049B59C(&screen, pose, &world);

        screen.y = (screen.y - lbl_eu_80666F74) * lbl_eu_80666F78;
        screen.z = lbl_eu_80666F68;
        screen.x = screen.x - lbl_eu_80666F70;

        // Direction: actors with the "action source" bit cycle the popup
        // direction through the live-actor count modulo 3.
        dirFlag = (actor->field_0x64 >> 1) & 1;
        if (dirFlag != 0) {
            e.field_30 = aliveCount % 3;
        }
        if (e.field_30 == 1) {
            screen.x -= lbl_eu_80666F7C;
        } else if (e.field_30 == 2) {
            screen.x += lbl_eu_80666F7C;
        }
    }

    e.field_28 = screen.x;
    e.field_2C = screen.y;

    if (flags == 0) {
        char buf8[0x8]; // 0x08 sprintf scratch

        nw4r::lyt::Pane* pane = e.mLayout0->GetRootPane();
        pane->SetTranslate(screen);

        switch (self->mDamageType) {
        case 0:
            sprintf(buf8, lbl_eu_804FD524 + 0xc9);
            break;
        case 1:
            sprintf(buf8, lbl_eu_804FD524 + 0xca);
            break;
        case 2:
            sprintf(buf8, lbl_eu_804FD524 + 0xcc);
            break;
        }

        ml::FixStr<32> buf; // 0x34
        switch (self->mDamageType) {
        case 0:
            buf.format(lbl_eu_804FD524 + 0xcf, buf8, e.field_20);
            break;
        case 1:
            buf.format(lbl_eu_804FD524 + 0xcf, buf8, e.field_20);
            break;
        case 2:
            buf.format(lbl_eu_804FD524 + 0xcf, buf8, e.field_20);
            break;
        case 3:
            buf.format(lbl_eu_804FD524 + 0xcf, buf8, e.field_20);
            break;
        case 4:
            buf.format(lbl_eu_804FD524 + 0xd4,
                       func_80136190(lbl_eu_804FD524 + 0xd7,
                                     lbl_eu_804FD524 + 0xc4, 0xb));
            break;
        case 5:
            buf.format(lbl_eu_804FD524 + 0xe2, buf8, e.field_20,
                       func_80136190(lbl_eu_804FD524 + 0xd7,
                                     lbl_eu_804FD524 + 0xc4, 0xc));
            break;
        case 6:
            buf.format(lbl_eu_804FD524 + 0xe2, buf8, e.field_20,
                       func_80136190(lbl_eu_804FD524 + 0xd7,
                                     lbl_eu_804FD524 + 0xc4, 0xe));
            break;
        case 7:
            buf.format(lbl_eu_804FD524 + 0xe2, buf8, e.field_20,
                       func_80136190(lbl_eu_804FD524 + 0xd7,
                                     lbl_eu_804FD524 + 0xc4, 0xd));
            break;
        case 8:
            buf.format(lbl_eu_804FD524 + 0xd4,
                       func_80136190(lbl_eu_804FD524 + 0xd7,
                                     lbl_eu_804FD524 + 0xc4, 0xe));
            break;
        case 9:
            buf.format(lbl_eu_804FD524 + 0xd4,
                       func_80136190(lbl_eu_804FD524 + 0xd7,
                                     lbl_eu_804FD524 + 0xc4, 0xd));
            break;
        case 0xa:
            buf.format(lbl_eu_804FD524 + 0xcf, buf8, e.field_20);
            break;
        }

        // Hide every damage-type pane, then reveal the one selected below.
        e.mLayout0->GetRootPane()->FindPaneByName(lbl_eu_804FD524 + 0xea,
                                                  true)
            ->SetVisible(false);
        e.mLayout0->GetRootPane()->FindPaneByName(lbl_eu_804FD524 + 0xf5,
                                                  true)
            ->SetVisible(false);
        e.mLayout0->GetRootPane()->FindPaneByName(lbl_eu_804FD524 + 0xfd,
                                                  true)
            ->SetVisible(false);
        e.mLayout0->GetRootPane()->FindPaneByName(lbl_eu_804FD524 + 0x10a,
                                                  true)
            ->SetVisible(false);
        e.mLayout0->GetRootPane()->FindPaneByName(lbl_eu_804FD524 + 0x112,
                                                  true)
            ->SetVisible(false);
        e.mLayout0->GetRootPane()->FindPaneByName(lbl_eu_804FD524 + 0x11a,
                                                  true)
            ->SetVisible(false);
        e.mLayout0->GetRootPane()->FindPaneByName(lbl_eu_804FD524 + 0x124,
                                                  true)
            ->SetVisible(false);

        const char* animName = lbl_eu_804FD524 + 0xf5;
        if (self->mDamageType == 3) {
            animName = lbl_eu_804FD524 + 0xfd;
        } else if (self->mDamageType == 1) {
            animName = lbl_eu_804FD524 + 0x112;
        } else if (self->mDamageType == 2) {
            animName = lbl_eu_804FD524 + 0x11a;
        } else if (self->mDamageType == 0xa) {
            animName = lbl_eu_804FD524 + 0x10a;
        } else {
            animName = lbl_eu_804FD524 + 0xf5;
            if (dirFlag != 0) {
                animName = lbl_eu_804FD524 + 0xea;
            }
        }
        e.mLayout0->GetRootPane()
            ->FindPaneByName(animName, true)
            ->SetVisible(true);
        func_80136B4C(e.mLayout0, animName, buf.c_str(), 0);

        if (self->_pad776[1] != 0) {
            e.mLayout0->GetRootPane()
                ->FindPaneByName(lbl_eu_804FD524 + 0x124, true)
                ->SetVisible(true);
        }

        // Select the damage anim: mAnim1 by default, mAnim0 for the plain
        // numeric types, mAnim2 when the extra style flag is set.
        e.field_32 = 1;
        e.field_04 = e.mAnim1;
        if (self->mDamageType == 2 || self->mDamageType == 1 ||
            self->mDamageType == 0xa) {
            e.field_04 = e.mAnim0;
        }
        if (self->field_0x779 != 0) {
            e.field_04 = e.mAnim2;
        }
        e.mLayout0->UnbindAllAnimation();
        e.mLayout0->BindAnimation(e.field_04);
        e.mLayout0->SetAnimationEnable(e.field_04, true);
        e.field_04->SetFrame(lbl_eu_80666F68);
        e.mLayout0->Animate(0);

        e.field_34 = lbl_eu_80666F68;
        for (s8 j = found - 1; j >= 0; j--) {
            CMenuBattleDamageEntry& p = self->mEntries[j];
            if (p.mActive == 0) {
                continue;
            }
            if (p.field_32 == 0) {
                continue;
            }
            if (p.field_24 != e.field_24) {
                continue;
            }
            // Chain the start frame to the previous popup of this actor so
            // they don't overlap: prev + (frameSize - 1)*scale - frame.
            f32 mf = p.field_04->GetFrame();
            u16 fs = p.field_04->GetFrameSize();
            e.field_34 = p.field_34 +
                         ((static_cast<f64>(fs) - lbl_eu_80666F6C) *
                              lbl_eu_80666F80 -
                          mf);
            if (e.field_34 < lbl_eu_80666F68) {
                e.field_34 = lbl_eu_80666F68;
            }
            e.mLayout0->GetRootPane()->SetVisible(false);
            break;
        }
    } else {
        nw4r::lyt::Pane* pane = e.mLayout1->GetRootPane();
        pane->SetTranslate(screen);

        // Pick the label text for the small layout by flag bits.
        const char* text = func_80145AA8(value);
        if (flags & 0x8) {
            text = func_8013639C(
                reinterpret_cast<const void*>(lbl_eu_806640E0),
                lbl_eu_804FD524 + 0xc4, value);
        } else if (flags & 0x10) {
            text = func_8013639C(
                reinterpret_cast<const void*>(lbl_eu_80664160),
                lbl_eu_804FD524 + 0xc4, value);
        } else if (flags & 0x20) {
            switch (value) {
            case 1:
                text = func_eu_802B142C();
                break;
            case 2:
                text = func_eu_802B1444();
                break;
            case 3:
                text = func_eu_802B145C();
                break;
            }
        }

        e.mLayout1->GetRootPane()->FindPaneByName(lbl_eu_804FD524 + 0x12d,
                                                  true)
            ->SetVisible(false);
        e.mLayout1->GetRootPane()->FindPaneByName(lbl_eu_804FD524 + 0x133,
                                                  true)
            ->SetVisible(false);
        if (flags & 0x22) {
            e.mLayout1->GetRootPane()
                ->FindPaneByName(lbl_eu_804FD524 + 0x133, true)
                ->SetVisible(true);
            func_80136B4C(e.mLayout1, lbl_eu_804FD524 + 0x139, text, 0);
        } else {
            e.mLayout1->GetRootPane()
                ->FindPaneByName(lbl_eu_804FD524 + 0x12d, true)
                ->SetVisible(true);
            func_80136B4C(e.mLayout1, lbl_eu_804FD524 + 0x148, text, 0);
        }

        // Bind a "timg" resource to the small layout (name depends on the
        // flag bits), then set the fixed label string if one was found.
        u8* res = static_cast<u8*>(func_801355F4()->GetResource(
            0x74696D67, lbl_eu_804FD524 + 0x157, 0));
        if (flags & 0x8) {
            res = static_cast<u8*>(func_801355F4()->GetResource(
                0x74696D67, lbl_eu_804FD524 + 0x16e, 0));
        } else if (flags & 0x10) {
            res = static_cast<u8*>(func_801355F4()->GetResource(
                0x74696D67, lbl_eu_804FD524 + 0x185, 0));
        } else if (flags & 0x20) {
            res = static_cast<u8*>(func_801355F4()->GetResource(
                0x74696D67, lbl_eu_804FD524 + 0x19c, 0));
        } else if (flags & 0x40) {
            res = static_cast<u8*>(func_801355F4()->GetResource(
                0x74696D67, lbl_eu_804FD524 + 0x19c, 0));
        }
        if (res != 0) {
            func_80137E7C(e.mLayout1, lbl_eu_804FD524 + 0x1af);
        }

        e.field_32 = 0;
        e.mAnim3->SetFrame(lbl_eu_80666F68);
        e.mLayout1->Animate(0);

        e.field_34 = lbl_eu_80666F68;
        for (s8 j = found - 1; j >= 0; j--) {
            CMenuBattleDamageEntry& p = self->mEntries[j];
            if (p.mActive == 0) {
                continue;
            }
            if (p.field_32 != 0) {
                continue;
            }
            if (p.field_24 != e.field_24) {
                continue;
            }
            f32 mf = p.mAnim3->GetFrame();
            u16 fs = p.mAnim3->GetFrameSize();
            e.field_34 = p.field_34 +
                         ((static_cast<f64>(fs) - lbl_eu_80666F6C) *
                              lbl_eu_80666F80 -
                          mf);
            if (e.field_34 < lbl_eu_80666F68) {
                e.field_34 = lbl_eu_80666F68;
            }
            e.mLayout1->GetRootPane()->SetVisible(false);
            break;
        }
    }

    self->_pad776[0] += 1;
}
