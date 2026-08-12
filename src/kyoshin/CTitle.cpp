// Auto-scaffolded catalog TU for kyoshin/CTitle
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CTitle.hpp"
#include "kyoshin/code_80135FDC.hpp" // func_80136E84 / func_80136F08 layout+anim builders

#include "kyoshin/cf/CfGameManager.hpp" // cf::CfGameManager::getPlayer
#include "kyoshin/help/CHelp_Talk.hpp"  // func_80174C98
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/lib/CLibLayout.hpp"   // CLibLayout::createArcResourceAccessor
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CEventFile.hpp"  // CEventFile::mFileHandle

#include <nw4r/math.h>

// TU-local plain functions (defined below in address order) called by the
// state-machine helpers in this file. noinline keeps real bl branches
// (retail calls them, so the call must survive).
void __declspec(noinline) func_802B6724(CTitleLogo* self);
void __declspec(noinline) func_802B67BC(CTitleLogo* self);
void __declspec(noinline) func_802B6854(CTitleLogo* self);
void __declspec(noinline) func_802B6F64(CTitleMenu* self);
void __declspec(noinline) func_802B71C4(CTitleMenu* self);
void __declspec(noinline) func_802B7094(CTitleMenu* self);
void __declspec(noinline) func_802B725C(CTitleMenu* self);

// ---------------------------------------------------------------------------
// CTitleLogo: vtable + six zero fields; field_0x19 starts at 1.
// ---------------------------------------------------------------------------
extern "C" __declspec(noinline) CTitleLogo* __ct__CTitleLogo(CTitleLogo* self) {
    *(void**)self = lbl_eu_8053B368;
    self->mAccessor = 0;
    self->mLayout = 0;
    self->mAnimTrans0 = 0;
    self->mAnimTrans1 = 0;
    self->mAnimTrans2 = 0;
    self->field_0x18 = 0;
    self->field_0x19 = 1;
    self->field_0x1A = 0;
    return self;
}

CTitleLogo::~CTitleLogo() {}

// Build the logo layout from the freshly-attached arc: init the layout and
// its three animation transforms, then unbind all (they get rebound by
// func_802B6724 when the logo becomes active).
void func_802B63A4(CTitleLogo* self, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    self->mAccessor = arcResAcc;
    func_80136E84(&self->mLayout, self->mAccessor, &lbl_eu_80513628[0]);
    func_80136F08(self->mLayout, &self->mAnimTrans0, self->mAccessor, &lbl_eu_80513628[0x13]);
    func_80136F08(self->mLayout, &self->mAnimTrans1, self->mAccessor, &lbl_eu_80513628[0x29]);
    func_80136F08(self->mLayout, &self->mAnimTrans2, self->mAccessor, &lbl_eu_80513628[0x41]);
    self->mLayout->UnbindAllAnimation();
}

// CTitleLogo per-frame update: run the state-machine entry selected by
// field_0x1A, then advance the layout animation. The exit-before-body label
// shape reproduces the retail branch-over-branch gate (MWCC_REFERENCE
// sjrbf_PutChunk pattern).
void func_802B6434(CTitleLogo* self) {
    if (self->mLayout != 0 && self->field_0x18 != 0) {
        goto body;
    }
    goto exit;
exit:
    return;
body:
    (self->*lbl_eu_8053B1F0[self->field_0x1A])();
    self->mLayout->Animate(0);
}

extern "C" void func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(void* layout, void* info, int a, int b);

extern "C" void func_802B64AC(void* self, void* drawInfo) {
    CTitleLogo* logo = (CTitleLogo*)self;
    if (logo->mLayout != 0 && logo->field_0x18 != 0) {
        goto call;
    }
    return;
call:
    func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(
        logo->mLayout, drawInfo, 0, 1);
}

// Target: us-802b8d9c - CTitleLogo ctor: implicit vptr (retail
// lbl_eu_8053B368), nulled pointers, phase flag 1.
CTitleLogo::CTitleLogo() {
    mAccessor = nullptr;
    mLayout = nullptr;
    mAnimTrans0 = nullptr;
    mAnimTrans1 = nullptr;
    mAnimTrans2 = nullptr;
    field_0x18 = 0;
    field_0x19 = 1;
    field_0x1A = 0;
}

void func_802B64DC(){}

void func_802B6534(CTitleLogo* self) {
    if (self->field_0x1A == 0) {
        self->field_0x1A = 1;
        func_802B6724(self);
        self->field_0x18 = 1;
        self->field_0x19 = 0;
    }
}

void func_802B6580(CTitleLogo* self) {
    if (self->field_0x1A == 2) {
        self->field_0x1A = 3;
        func_802B6854(self);
        self->field_0x19 = 0;
    }
}

// Logo state 1: park the first animation on its final frame (frame count minus
// the per-frame delta), advance the layout, then step into state 2 and mark
// the phase flag so the caller can proceed.
extern "C" __declspec(noinline) void func_802B65C8(CTitleLogo* self) {
    if (self->field_0x1A == 1) {
        self->mAnimTrans0->SetFrame((float)self->mAnimTrans0->GetFrameSize() - lbl_eu_80668FE0);
        self->mLayout->Animate(0);
        self->field_0x1A = 2;
        func_802B67BC(self);
        self->field_0x19 = 1;
    }
}

void func_802B6660() {}

void func_802B6664(){}

extern float lbl_eu_80668FE0;
extern "C" void func_80137444__FPQ34nw4r3lyt13AnimTransformf(void* a, float f);
extern "C" void func_802B66B8(void* self) { func_80137444__FPQ34nw4r3lyt13AnimTransformf(*(void**)((u8*)self + 0x10), lbl_eu_80668FE0); }

void func_802B66C4(){}

// Logo intro: bind the logo "in" animation, reset it to frame 0, and advance
// the layout once so it is ready to play.
void __declspec(noinline) func_802B6724(CTitleLogo* self) {
    self->mLayout->UnbindAnimation(self->mAnimTrans2);
    self->mLayout->BindAnimation(self->mAnimTrans0);
    self->mLayout->SetAnimationEnable(self->mAnimTrans0, true);
    self->mAnimTrans0->SetFrame(lbl_eu_80668FF0);
    self->mLayout->Animate(0);
}

// Logo phase 2: unbind the intro animation, bind animation 1, reset it to
// frame 0, and advance the layout (mirrors func_802B6724 with the next slot).
void __declspec(noinline) func_802B67BC(CTitleLogo* self) {
    self->mLayout->UnbindAnimation(self->mAnimTrans0);
    self->mLayout->BindAnimation(self->mAnimTrans1);
    self->mLayout->SetAnimationEnable(self->mAnimTrans1, true);
    self->mAnimTrans1->SetFrame(lbl_eu_80668FF0);
    self->mLayout->Animate(0);
}

// Logo phase 3: same switch pattern, moving from animation 1 to animation 2.
void __declspec(noinline) func_802B6854(CTitleLogo* self) {
    self->mLayout->UnbindAnimation(self->mAnimTrans1);
    self->mLayout->BindAnimation(self->mAnimTrans2);
    self->mLayout->SetAnimationEnable(self->mAnimTrans2, true);
    self->mAnimTrans2->SetFrame(lbl_eu_80668FF0);
    self->mLayout->Animate(0);
}

// ---------------------------------------------------------------------------
// CTitleMenu: vtable + nine zero fields; field_0x25 starts at 1.
// ---------------------------------------------------------------------------
extern "C" __declspec(noinline) CTitleMenu* __ct__CTitleMenu(CTitleMenu* self) {
    *(void**)self = lbl_eu_8053B35C;
    self->mAccessor = 0;
    self->mLayout = 0;
    self->mAnimTrans0 = 0;
    self->mAnimTrans1 = 0;
    self->mAnimTrans2 = 0;
    self->mAnimTrans3 = 0;
    self->mAnimTrans4 = 0;
    self->mAnimTrans5 = 0;
    self->field_0x24 = 0;
    self->field_0x25 = 1;
    self->field_0x26 = 0;
    return self;
}

CTitleMenu::~CTitleMenu() {}

// Build the title menu layout: attach the accessor, load the layout and all
// six animation transforms, then unbind all (rebound when the menu becomes
// active). On the bit-30 gate, tint the title text via func_80139A18 with two
// grey colors (opaque + transparent).
void func_802B6970(CTitleMenu* self, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    GXColorS10 colorA;
    GXColorS10 colorB;
    self->mAccessor = arcResAcc;
    func_80136E84(&self->mLayout, self->mAccessor, &lbl_eu_80513628[0x58]);
    func_80136F08(self->mLayout, &self->mAnimTrans0, self->mAccessor, &lbl_eu_80513628[0x6b]);
    func_80136F08(self->mLayout, &self->mAnimTrans1, self->mAccessor, &lbl_eu_80513628[0x88]);
    func_80136F08(self->mLayout, &self->mAnimTrans2, self->mAccessor, &lbl_eu_80513628[0xa7]);
    func_80136F08(self->mLayout, &self->mAnimTrans3, self->mAccessor, &lbl_eu_80513628[0xc4]);
    func_80136F08(self->mLayout, &self->mAnimTrans4, self->mAccessor, &lbl_eu_80513628[0xda]);
    func_80136F08(self->mLayout, &self->mAnimTrans5, self->mAccessor, &lbl_eu_80513628[0xf2]);
    self->mLayout->UnbindAllAnimation();
    if ((lbl_eu_80663E28 & 0x40000000) != 0) {
        GXColorS10* pColorA = func_801C4B60(&colorA, 0x80, 0x80, 0x80, 0xff);
        GXColorS10* pColorB = func_801C4B60(&colorB, 0x80, 0x80, 0x80, 0x00);
        char* pText = &lbl_eu_80513628[0x109];
        func_80139A18(self->mLayout, pText, pColorB, pColorA);
    }
}

// CTitleMenu per-frame update: run the state-machine entry selected by
// field_0x26, then advance the layout animation (same gate shape as
// func_802B6434).
void func_802B6A90(CTitleMenu* self) {
    if (self->mLayout != 0 && self->field_0x24 != 0) {
        goto body;
    }
    goto exit;
exit:
    return;
body:
    (self->*lbl_eu_8053B220[self->field_0x26])();
    self->mLayout->Animate(0);
}

extern "C" void func_802B6B08(void* self, void* drawInfo) {
    CTitleMenu* menu = (CTitleMenu*)self;
    if (menu->mLayout != 0 && menu->field_0x24 != 0) {
        goto call;
    }
    return;
call:
    func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(
        menu->mLayout, drawInfo, 0, 1);
}

void func_802B6B38(){}

extern "C" __declspec(noinline) void func_802B6B90(CTitleMenu* self) {
    if (self->field_0x26 == 0) {
        self->field_0x26 = 1;
        func_802B6F64(self);
        self->field_0x24 = 1;
        self->field_0x25 = 0;
    }
}

extern "C" __declspec(noinline) void func_802B6BDC(CTitleMenu* self) {
    if (self->field_0x26 == 2) {
        self->field_0x26 = 3;
        func_802B7094(self);
        self->field_0x25 = 0;
    }
}

void func_802B6C24(){}

void func_802B6C74(CTitleMenu* self) {
    if (self->field_0x26 == 5) {
        self->field_0x26 = 6;
        func_802B725C(self);
        self->field_0x25 = 0;
    }
}

extern "C" void __declspec(noinline) func_802B6CBC(nw4r::math::VEC3* dest, CTitleMenu* menu, u8 val) {}

// Menu state 4: park animation 3 on its final frame (frame count minus the
// per-frame delta), advance the layout, step into state 5, and mark the
// phase flag so the caller can proceed.
extern "C" __declspec(noinline) void func_802B6D5C(CTitleMenu* self) {
    if (self->field_0x26 == 4) {
        self->mAnimTrans3->SetFrame((float)self->mAnimTrans3->GetFrameSize() - lbl_eu_80668FE0);
        self->mLayout->Animate(0);
        self->field_0x26 = 5;
        func_802B71C4(self);
        self->field_0x25 = 1;
    }
}

void func_802B6DF4() {}

void func_802B6DF8(){}

extern "C" void func_802B6E4C(void* self) { func_80137444__FPQ34nw4r3lyt13AnimTransformf(*(void**)((u8*)self + 0x10), lbl_eu_80668FE0); }

void func_802B6E58(){}

void func_802B6EA4(){}

extern "C" void func_802B6EF8(void* self) { func_80137444__FPQ34nw4r3lyt13AnimTransformf(*(void**)((u8*)self + 0x1C), lbl_eu_80668FE0); }

void func_802B6F04(){}

// Menu intro: unbind the last animation, bind animation 0, reset it to
// frame 0, and advance the layout once so it is ready to play.
void __declspec(noinline) func_802B6F64(CTitleMenu* self) {
    self->mLayout->UnbindAnimation(self->mAnimTrans5);
    self->mLayout->BindAnimation(self->mAnimTrans0);
    self->mLayout->SetAnimationEnable(self->mAnimTrans0, true);
    self->mAnimTrans0->SetFrame(lbl_eu_80668FF0);
    self->mLayout->Animate(0);
}

// Menu phase 2: unbind the intro animation, bind animation 1, reset it to
// frame 0, and advance the layout.
void func_802B6FFC(CTitleMenu* self) {
    self->mLayout->UnbindAnimation(self->mAnimTrans0);
    self->mLayout->BindAnimation(self->mAnimTrans1);
    self->mLayout->SetAnimationEnable(self->mAnimTrans1, true);
    self->mAnimTrans1->SetFrame(lbl_eu_80668FF0);
    self->mLayout->Animate(0);
}

void __declspec(noinline) func_802B7094(CTitleMenu* self){}

void func_802B712C(){}

void __declspec(noinline) func_802B71C4(CTitleMenu* self) {}

void __declspec(noinline) func_802B725C(CTitleMenu* self){}

// ---------------------------------------------------------------------------
// CTitle: IWorkEvent vtable, mem region, flag fields, then the embedded
// logo/menu/cursor sub-objects (retail order).
// ---------------------------------------------------------------------------
extern "C" CTitle* __ct__CTitle(CTitle* self) {
    *(void**)self = lbl_eu_8053B2C8;
    __ct__17UnkClass_8045F564Fv(&self->mMemRegion);
    self->mFileHandle = 0;
    self->mAccessor = 0;
    self->field_0x1C = 1;
    self->field_0x20 = 0;
    self->field_0x24 = 0;
    self->field_0x25 = -1;
    __ct__CTitleLogo(&self->mLogo);
    __ct__CTitleMenu(&self->mMenu);
    __ct__CCur18(reinterpret_cast<CBaseCur*>(&self->mCur[0]), 0);
    return self;
}

// Member dtors and operator delete (retail names).
extern "C" void __dt__6CCur18Fv(void* self, int flags);
extern "C" void __dt__17UnkClass_8045F564Fv(void* self, int flags);
extern "C" void __dl__FPv(void* p);

// Retail dtor is a plain free function: destroys the +0x70 CCur18 member and
// the +0x04 UnkClass_8045F564 member, then frees the object when the delete
// flag is positive.
#pragma optimize_for_size on
extern "C" void* __dt__6CTitleFv(void* self, int flags) {
    if (self != 0) {
        __dt__6CCur18Fv((u8*)self + 0x70, -1);
        __dt__17UnkClass_8045F564Fv((u8*)self + 0x04, -1);
        if (flags > 0)
            __dl__FPv(self);
    }
    return self;
}
#pragma optimize_for_size off

// Kick off the async load of the title layout arc, register the file-event
// callback, and expose this instance to the callback via the sbss singleton.
void func_802B73D4(CTitle* self) {
    self->mFileHandle = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(),
                                              &lbl_eu_80513628[0x133], self, 0, 0);
    CDeviceFile::setHandleFlag1(self->mFileHandle);
    if (func_8023FEDC(func_802B7948) == 0) {
        self->field_0x25 = 0;
    }
    self->field_0x1C = 0;
    lbl_eu_80664C38 = self;
}

void func_802B744C(){}

void func_802B74A8(){}

void func_802B74F4(){}

extern "C" int func_802B7564(CTitle* self) {
    int r = 0;
    if (self->field_0x1C != 0 && (s8)self->field_0x25 != -1) {
        r = 1;
    }
    return r;
}

extern "C" int func_802B7590(CTitle* self) {
    int r = 0;
    if (self->mLogo.field_0x19 != 0 && self->mMenu.field_0x25 != 0) {
        r = 1;
    }
    return r;
}

extern "C" void func_802B75B8(CTitle* self) {
    if (self->field_0x24 != 0) return;
    self->field_0x24 = 1;
    func_802B6B90(&self->mMenu);
}

void func_802B75D8(){}

extern "C" void func_802B7630(CTitle* self) {
    if (self->field_0x24 != 2) return;
    self->field_0x24 = 3;
    func_802B6BDC(&self->mMenu);
}

// Selection cursor up: while in the selection phase, step the menu index back
// (wrapping from -1 to 2), reposition the cursor over the new entry, and cue
// the menu SFX.
void func_802B7650(CTitle* self) {
    if (self->field_0x24 == 5) {
        self->field_0x25 = self->field_0x25 - 1;
        if (self->field_0x25 < 0) {
            self->field_0x25 = 2;
        }
        nw4r::math::VEC3 pos;
        func_802B6CBC(&pos, &self->mMenu, self->field_0x25);
        reinterpret_cast<CCur18View*>(&self->mCur[0])->vf04(&pos);
        func_80138078(1);
    }
}

// Selection cursor down: step the menu index forward (wrapping 3 -> 0),
// reposition the cursor over the new entry, and cue the menu SFX.
void func_802B76D4(CTitle* self) {
    if (self->field_0x24 == 5) {
        self->field_0x25 = self->field_0x25 + 1;
        if (self->field_0x25 >= 3) {
            self->field_0x25 = 0;
        }
        nw4r::math::VEC3 pos;
        func_802B6CBC(&pos, &self->mMenu, self->field_0x25);
        reinterpret_cast<CCur18View*>(&self->mCur[0])->vf04(&pos);
        func_80138078(1);
    }
}

// Selection confirm (phase 5): dispatch the selection virtual on the +0x20
// controller, cue the confirm/cancel SFX by the current index, and return
// whether the selection index is unset (0).
int func_802B775C(CTitle* self) {
    if (self->field_0x24 != 5) {
        return 0;
    }
    self->field_0x20->vf01(self->field_0x25);
    int result = 0;
    if (self->field_0x25 == 1 && (lbl_eu_80663E28 & 0x40000000) != 0) {
        result = 1;
    }
    if (result != 0) {
        func_80138078(5);
    } else {
        func_80138078(4);
    }
    return self->field_0x25 == 0;
}

// Target: us-802ba270 - init the embedded +0x2C and +0x48 sub-objects.
extern "C" void func_802B7800(void* self) {
    func_802B65C8((CTitleLogo*)((u8*)self + 0x2C));
    func_802B6D5C((CTitleMenu*)((u8*)self + 0x48));
}

void func_802B7838() {}

void func_802B783C(u8* thisPtr) {
    if (thisPtr[0x6d] == 0) {
        return;
    }
    thisPtr[0x24] = 2;
}

void func_802B7854() {}

void func_802B7858(){}

// Advance to the selection phase: mark phase 5, show the cursor, build the
// cursor target from the current menu entry, and move the cursor to it.
void func_802B78A4(CTitle* self) {
    if (self->mLogo.field_0x19 != 0 && self->mMenu.field_0x25 != 0) {
        self->field_0x24 = 5;
        func_801D216C(&self->mCur[0], 1);
        nw4r::math::VEC3 pos;
        func_802B6CBC(&pos, &self->mMenu, self->field_0x25);
        reinterpret_cast<CCur18View*>(&self->mCur[0])->vf04(&pos);
    }
}

void func_802B7920() {}

extern "C" void func_802B7924(CTitle* self) {
    if (self->mLogo.field_0x19 == 0) return;
    if (self->mMenu.field_0x25 == 0) return;
    self->field_0x24 = 0;
}

extern "C" void func_802B7948(void* a, unsigned int b, unsigned char v) {
    extern void* lbl_eu_80664C38;
    void* p = lbl_eu_80664C38;
    if (p) *(unsigned char*)((u8*)p + 0x25) = v;
}

// File-event callback: the title layout arc finished loading. Open a scratch
// region (RAII Class_8045F858 guard), detach the file buffer, attach it to a
// fresh ArcResourceAccessor, and build the logo + menu layouts plus the
// embedded CCur18 cursor (construct on stack, copy the body, destroy).
bool CTitle::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle != pEventFile->mFileHandle) {
        return false;
    }
    mMemRegion.createRegion(mtl::MemManager::getHandleMEM2(), 0x10000,
                            &lbl_eu_80513628[0x145], 1);
    Class_8045F858 regionGuard(&mMemRegion);
    void* fileData = mFileHandle->getData();
    mtl::MemManager::func_80434A4C(false);
    mAccessor = CLibLayout::createArcResourceAccessor();
    mAccessor->Attach(fileData, &lbl_eu_80513628[0x14c]);
    func_802B63A4(&mLogo, mAccessor);
    func_802B6970(&mMenu, mAccessor);
    u8 cur18Temp[0x18];
    __ct__CCur18(reinterpret_cast<CBaseCur*>(cur18Temp), mAccessor);
    CBaseCur* curDst = reinterpret_cast<CBaseCur*>(&mCur[0]);
    CBaseCur* curSrc = reinterpret_cast<CBaseCur*>(cur18Temp);
    curDst->mArcResAcc = curSrc->mArcResAcc;
    curDst->mpLayout = curSrc->mpLayout;
    curDst->mpAnimTrans0 = curSrc->mpAnimTrans0;
    curDst->mpAnimTrans1 = curSrc->mpAnimTrans1;
    curDst->mActive = curSrc->mActive;
    curDst->mVisible = curSrc->mVisible;
    __dt__6CCur18Fv(curSrc, -1);
    reinterpret_cast<CCur18View*>(&mCur[0])->vf02();
    field_0x1C = 1;
    mFileHandle = 0;
    mMemRegion.func_8045F810();
    return true;
}

// Refresh the "to attack" button caches: resolve the current player's battle
// object, and if either cached button state says "held" while the live query
// (func_80174C98) reports otherwise, return 1 (state changed). Otherwise
// re-query both buttons (A = 0x7, Z = 0x800), store the results, return 0.
int cf::CHelp_ToAttack::func_802B7A90() {
    cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
    cf::CHelpBattleObj* battleObj = func_800BFC68(player);
    if (battleObj == 0) {
        return 0;
    }
    u32 valA;
    u32 valB;
    u32 valC;
    u32 valD;
    if (field_0xC != 0) {
        valA = *battleObj->field_0x4->vf30();
        if (func_80174C98(battleObj, &valA, 0x7) == 0) {
            return 1;
        }
    }
    if (field_0xD != 0) {
        valB = *battleObj->field_0x4->vf30();
        if (func_80174C98(battleObj, &valB, 0x800) == 0) {
            return 1;
        }
    }
    valC = *battleObj->field_0x4->vf30();
    field_0xC = func_80174C98(battleObj, &valC, 0x7);
    valD = *battleObj->field_0x4->vf30();
    field_0xD = func_80174C98(battleObj, &valD, 0x800);
    return 0;
}
