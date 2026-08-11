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
#include <string.h>

extern "C" void __dt__17CMenuBattleDamageFv(void*, int);

// Defined below in this TU (damage-slot enqueue; also called by func_80109734).
extern "C" void func_801098B0(CMenuBattleDamage* self, int actorId,
                              int value, u32 flags);

// Unmangled retail ctor symbol; takes the owning scene (stored at +0x60).
// Zero-fills the entries twice: first directly, then by copying a
// zero-initialized stack local into each entry (retail order).
void __ct__CMenuBattleDamage(CMenuBattleDamage* obj, CScn* scene) {
    char* vtFinal;
    char* vtWork;
    char* vtRender;
    u32 ptmfWord0;
    u32 ptmfWord1;
    u32 ptmfWord2;
    u32 z;

    __ct__8CProcessFv(reinterpret_cast<CProcess*>(obj));

    // Interim CProcess vtable, then final MI vtable + interface pieces.
    // Retail: lwzu of [0], then stw [1]@+0x40 before [0]@+0x3C.
    obj->mProcessVt = reinterpret_cast<u32>(lbl_eu_8052C1C0);
    vtFinal = lbl_eu_8052C230;
    ptmfWord0 = __ptmf_null[0];
    vtWork = vtFinal + 0x24;
    vtRender = vtFinal + 0xac;
    z = 0;
    ptmfWord1 = __ptmf_null[1];
    obj->ptmfMove[1] = ptmfWord1;
    obj->ptmfMove[0] = ptmfWord0;
    ptmfWord2 = __ptmf_null[2];
    obj->ptmfMove[2] = ptmfWord2;
    ptmfWord0 = __ptmf_null[0];
    ptmfWord1 = __ptmf_null[1];
    obj->ptmfDraw[1] = ptmfWord1;
    obj->ptmfDraw[0] = ptmfWord0;
    ptmfWord2 = __ptmf_null[2];
    obj->ptmfDraw[2] = ptmfWord2;
    obj->mField_54 = (u8)z;
    obj->mField_55 = (u8)z;
    obj->mProcessVt = reinterpret_cast<u32>(vtFinal);
    obj->mIWorkEventVt = reinterpret_cast<u32>(vtWork);
    obj->mIScnRenderVt = reinterpret_cast<u32>(vtRender);
    obj->mScn = scene;
    __ct__17UnkClass_8045F564Fv(&obj->mMemRegion);

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
        e.field_28 = lbl_eu_80666F68;
        e.field_2C = lbl_eu_80666F68;
        e.field_30 = 0;
        e.field_31 = 0;
        e.field_32 = 0;
        e.field_34 = lbl_eu_80666F68;
    }
    obj->mDamageType = (u8)z;
    obj->mDamageDir = (u8)z;
    obj->_pad776[0] = (u8)z;
    obj->_pad776[1] = (u8)z;
    obj->_pad776[2] = (u8)z;
    obj->field_0x779 = (u8)z;

    for (u8 i = 0; i < 0x20; i++) {
        CMenuBattleDamageEntry e = {};
        obj->mEntries[i] = e;
    }
}

CMenuBattleDamage::~CMenuBattleDamage() {}

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
                    func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
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

void CMenuBattleDamage::Move() {}

void CMenuBattleDamage::cbRenderBefore() {
    CTaskGame::getInstance();
    // Retail: guard 1 folds to `bne done`; guard 2 (rlwinm. bit test) is the
    // branch-over-branch `beq body; b done` - reproduced by the &&-chain
    // gate (first condition folds, second becomes the gate).
    if (CTaskGame::func_800426F0() == 0 &&
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
    u32 mem = CWorkThreadSystem::getWorkMem();
    CMenuBattleDamage* obj =
        (CMenuBattleDamage*)mtl::MemManager::allocate(0x77c, mem);
    if (obj != 0) {
        __ct__CMenuBattleDamage(obj, scene);
    }
    lbl_eu_80663F28 = obj;
    reinterpret_cast<CProcess*>(obj)->Regist(parent, false);
    return lbl_eu_80663F28;
}

extern "C" void func_80109734() {}
extern "C" void func_8010975C() {}
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

extern "C" void func_8010A8E4() {}
extern "C" void func_8010A940() {}
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
