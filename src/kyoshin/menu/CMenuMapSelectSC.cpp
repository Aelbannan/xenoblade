// Auto-scaffolded catalog TU for kyoshin/menu/CMenuMapSelectSC
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuMapSelectSC.hpp"

#include <new>
#include <cstring>
#include <revolution/gx/GXPixel.h>

struct S {
    int dummy;
    int a;
    int b;
    int c;
    int d;
    unsigned char e;
    unsigned char f;
};

extern "C" void func_80251294(S* r3, const S* r4);

// Retail constructor (unmangled global) with an explicit arg, mirroring the
// CMenuOption ctor pattern: base CProcess ctor, temp vtable store, null PMF
// data copy (two 3-word slots at 0x3C/0x48), then the composite vtable +
// IScnRender sub-vtable at +0x58, then each embedded widget's ctor and the
// final state bytes.
extern "C" CMenuMapSelectSC* __ct__CMenuMapSelectSC(CMenuMapSelectSC* self, CProcess* arg) {
    __ct__8CProcessFv((CProcess*)self);

    // vtable fixups: temp (CProcess) vtable first, then the composite vtable
    // and the IScnRender sub-vtable at +0x58.
    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_8052BF70;
    // Post-increment walk folds the first load into lwzu (btm_sco_init
    // pattern); the second group re-reads the base with plain loads so MWCC
    // reuses the already-materialised base register.
    u32* src = __ptmf_null;
    u32 pmfV0 = *src++;
    u32 pmfV1 = *src++;
    self->ptmf0[1] = pmfV1;
    self->ptmf0[0] = pmfV0;
    u32 pmfV2 = *src++;
    self->ptmf0[2] = pmfV2;
    u32* src2 = __ptmf_null;
    u32 pmfU0 = src2[0];
    u32 pmfU1 = src2[1];
    self->ptmf1[1] = pmfU1;
    self->ptmf1[0] = pmfU0;
    u32 pmfU2 = src2[2];
    self->ptmf1[2] = pmfU2;
    self->mField54 = 0;
    self->mField55 = 0;

    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_80537130;
    *(u32*)((u8*)self + 0x58) = (u32)lbl_eu_80537130 + 0x24;
    self->mParentRef = arg;

    __ct__CTitleAHelp(&self->mTitleAHelp, 0, 0);
    __ct__CFade(&self->mFade);
    __ct__CFloorMap(&self->mFloorMap);

    self->mState = 0;
    self->mTimer = lbl_eu_806687C0;
    return self;
}

/* Destructor (retail D2 form, symbol keeps its C++ mangling in US).
 *
 * Written with an explicit flags parameter so the base-class destruction can
 * target the retail CProcess dtor symbol __dt__800FED0C (0x800FF7F4, the
 * game-side D2 wrapper). A real member destructor would make MWCC auto-emit
 * __dt__8CProcessFv (the library D1 at 0x804474D0) for the base call, which
 * cannot match the retail reloc name at the call site. Subobjects are
 * destroyed in reverse construction order, then conditional operator delete
 * when flags > 0 (same shape as CSysWinSave's dtor). */
extern "C" CMenuMapSelectSC* __dt__16CMenuMapSelectSCFv(CMenuMapSelectSC* _this, int flags) {
    if (_this != 0) {
        __dt__9CFloorMapFv(&_this->mFloorMap, -1);
        __dt__5CFadeFv(&_this->mFade, -1);
        __dt__11CTitleAHelpFv(&_this->mTitleAHelp, -1);
        __dt__800FED0C(_this, 0);
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

void CMenuMapSelectSC::Init() {
    func_8008294C__Q22cf13CfGameManagerFv(1);

    // --- Re-initialise the embedded CTitleAHelp via a temporary ---
    char* name = func_80136190(lbl_eu_8050C5A4, lbl_eu_8050C5A4 + 8, 1);

    u8 tempTitle[0x38];
    __ct__CTitleAHelp(reinterpret_cast<CTitleAHelp*>(tempTitle), name, 0x4a);

    __ct__UnkClass_8011C974((u8*)this + 0x64, tempTitle + 0x4);
    CTitleAHelp* tmpTitle = reinterpret_cast<CTitleAHelp*>(tempTitle);
    mTitleAHelp.mFileHandle = tmpTitle->mFileHandle;
    mTitleAHelp.mArcResourceAccessor = tmpTitle->mArcResourceAccessor;
    mTitleAHelp.mLayout = tmpTitle->mLayout;
    mTitleAHelp.mAnimTrans20 = tmpTitle->mAnimTrans20;
    mTitleAHelp.mAnimTrans24 = tmpTitle->mAnimTrans24;
    mTitleAHelp.unk28 = tmpTitle->unk28;
    mTitleAHelp.unk2c = tmpTitle->unk2c;
    mTitleAHelp.mName = tmpTitle->mName;
    mTitleAHelp.unk34 = tmpTitle->unk34;
    mTitleAHelp.unk35 = tmpTitle->unk35;
    mTitleAHelp.unk36 = tmpTitle->unk36;
    mTitleAHelp.unk37 = tmpTitle->unk37;
    __dt__11CTitleAHelpFv(tmpTitle, -1);

    CTitleAHelp_load(&mTitleAHelp);

    // --- Re-initialise the embedded CFade via a temporary ---
    u8 tempFade[0x28];
    __ct__CFade(reinterpret_cast<CFade*>(tempFade));

    __ct__UnkClass_8011C974((u8*)this + 0x9c, tempFade + 0x4);
    CFade* tmpFade = reinterpret_cast<CFade*>(tempFade);
    mFade.mFileHandle = tmpFade->mFileHandle;
    mFade.mArcResAcc = tmpFade->mArcResAcc;
    mFade.mLayout = tmpFade->mLayout;
    mFade.mAnimTrans = tmpFade->mAnimTrans;
    mFade.mIsLoaded = tmpFade->mIsLoaded;
    mFade.mFadeState = tmpFade->mFadeState;
    mFade.mReady = tmpFade->mReady;
    mFade.mVisible = tmpFade->mVisible;
    __dt__5CFadeFv(tmpFade, -1);

    func_8024439C(&mFade);

    // --- Re-initialise the embedded CFloorMap via a temporary ---
    u8 tempFloor[0x3348];
    __ct__CFloorMap(reinterpret_cast<CFloorMap*>(tempFloor));

    __ct__UnkClass_8011C974((u8*)this + 0xc4, tempFloor + 0x4);
    __ct__UnkClass_8011C974((u8*)this + 0xd4, tempFloor + 0x14);

    // File handles / buffers / visibility flags (member 0x24-0x5D).
    *(u32*)((u8*)this + 0xe4) = *(u32*)(tempFloor + 0x24);
    *(u32*)((u8*)this + 0xe8) = *(u32*)(tempFloor + 0x28);
    *(u32*)((u8*)this + 0xec) = *(u32*)(tempFloor + 0x2c);
    *(u32*)((u8*)this + 0xf0) = *(u32*)(tempFloor + 0x30);
    *(u32*)((u8*)this + 0xf4) = *(u32*)(tempFloor + 0x34);
    *(u32*)((u8*)this + 0xf8) = *(u32*)(tempFloor + 0x38);
    *(u32*)((u8*)this + 0xfc) = *(u32*)(tempFloor + 0x3c);
    *(u8*)((u8*)this + 0x100) = *(u8*)(tempFloor + 0x40);
    *(u8*)((u8*)this + 0x101) = *(u8*)(tempFloor + 0x41);
    *(u8*)((u8*)this + 0x102) = *(u8*)(tempFloor + 0x42);
    *(u8*)((u8*)this + 0x103) = *(u8*)(tempFloor + 0x43);
    *(f32*)((u8*)this + 0x104) = *(f32*)(tempFloor + 0x44);
    *(f32*)((u8*)this + 0x108) = *(f32*)(tempFloor + 0x48);
    *(f32*)((u8*)this + 0x10c) = *(f32*)(tempFloor + 0x4c);
    *(f32*)((u8*)this + 0x110) = *(f32*)(tempFloor + 0x50);
    *(f32*)((u8*)this + 0x114) = *(f32*)(tempFloor + 0x54);
    *(u8*)((u8*)this + 0x118) = *(u8*)(tempFloor + 0x58);
    *(u16*)((u8*)this + 0x11a) = *(u16*)(tempFloor + 0x5a);
    *(u8*)((u8*)this + 0x11c) = *(u8*)(tempFloor + 0x5c);
    *(u8*)((u8*)this + 0x11d) = *(u8*)(tempFloor + 0x5d);

    __ct__UnkClass_8011C974((u8*)this + 0x124, tempFloor + 0x64);

    // Cursor / scroll state (member 0x74-0xB5).
    *(u32*)((u8*)this + 0x134) = *(u32*)(tempFloor + 0x74);
    *(u32*)((u8*)this + 0x138) = *(u32*)(tempFloor + 0x78);
    *(u32*)((u8*)this + 0x13c) = *(u32*)(tempFloor + 0x7c);
    *(u32*)((u8*)this + 0x140) = *(u32*)(tempFloor + 0x80);
    *(u8*)((u8*)this + 0x144) = *(u8*)(tempFloor + 0x84);
    *(u8*)((u8*)this + 0x145) = *(u8*)(tempFloor + 0x85);
    *(u8*)((u8*)this + 0x146) = *(u8*)(tempFloor + 0x86);
    *(u8*)((u8*)this + 0x147) = *(u8*)(tempFloor + 0x87);
    *(f32*)((u8*)this + 0x148) = *(f32*)(tempFloor + 0x88);
    *(f32*)((u8*)this + 0x14c) = *(f32*)(tempFloor + 0x8c);
    *(f32*)((u8*)this + 0x150) = *(f32*)(tempFloor + 0x90);
    *(f32*)((u8*)this + 0x154) = *(f32*)(tempFloor + 0x94);
    *(f32*)((u8*)this + 0x158) = *(f32*)(tempFloor + 0x98);
    *(u8*)((u8*)this + 0x15c) = *(u8*)(tempFloor + 0x9c);
    *(u32*)((u8*)this + 0x164) = *(u32*)(tempFloor + 0xa4);
    *(u32*)((u8*)this + 0x168) = *(u32*)(tempFloor + 0xa8);
    *(u32*)((u8*)this + 0x16c) = *(u32*)(tempFloor + 0xac);
    *(u32*)((u8*)this + 0x170) = *(u32*)(tempFloor + 0xb0);
    *(u8*)((u8*)this + 0x174) = *(u8*)(tempFloor + 0xb4);
    *(u8*)((u8*)this + 0x175) = *(u8*)(tempFloor + 0xb5);

    func_8016742C((u8*)this + 0x178, tempFloor + 0xb8);

    // Layout pointer array (member 0x130-0x14C) + 20 8-byte entries.
    *(u32*)((u8*)this + 0x1f0) = *(u32*)(tempFloor + 0x130);
    *(u32*)((u8*)this + 0x1f4) = *(u32*)(tempFloor + 0x134);
    *(u32*)((u8*)this + 0x1f8) = *(u32*)(tempFloor + 0x138);
    *(u32*)((u8*)this + 0x1fc) = *(u32*)(tempFloor + 0x13c);
    *(u32*)((u8*)this + 0x200) = *(u32*)(tempFloor + 0x140);
    *(u32*)((u8*)this + 0x204) = *(u32*)(tempFloor + 0x144);
    *(u32*)((u8*)this + 0x208) = *(u32*)(tempFloor + 0x148);
    *(u8*)((u8*)this + 0x20c) = *(u8*)(tempFloor + 0x14c);
    memcpy((u8*)this + 0x210, tempFloor + 0x150, sizeof(UnkLayoutEntry) * 0x14);

    // Entry count / draw flags (member 0x1F0-0x208).
    *(u8*)((u8*)this + 0x2b0) = *(u8*)(tempFloor + 0x1f0);
    *(u32*)((u8*)this + 0x2b4) = *(u32*)(tempFloor + 0x1f4);
    *(u8*)((u8*)this + 0x2b8) = *(u8*)(tempFloor + 0x1f8);
    *(u32*)((u8*)this + 0x2bc) = *(u32*)(tempFloor + 0x1fc);
    *(u32*)((u8*)this + 0x2c0) = *(u32*)(tempFloor + 0x200);
    *(u8*)((u8*)this + 0x2c4) = *(u8*)(tempFloor + 0x204);
    *(u8*)((u8*)this + 0x2c5) = *(u8*)(tempFloor + 0x205);
    *(u8*)((u8*)this + 0x2c6) = *(u8*)(tempFloor + 0x206);
    *(u8*)((u8*)this + 0x2c7) = *(u8*)(tempFloor + 0x207);
    *(u8*)((u8*)this + 0x2c8) = *(u8*)(tempFloor + 0x208);

    // Map-data blob (member 0x20C, 0x30C0 bytes).
    memcpy((u8*)this + 0x2cc, tempFloor + 0x20c, 0x30c0);

    *(u8*)((u8*)this + 0x338c) = *(u8*)(tempFloor + 0x32cc);

    // Four 0x18-byte sub-object copies.
    func_80251294((S*)((u8*)this + 0x3390), (const S*)(tempFloor + 0x3338));
    func_80251294((S*)((u8*)this + 0x33a8), (const S*)(tempFloor + 0x3350));
    func_80251294((S*)((u8*)this + 0x33c0), (const S*)(tempFloor + 0x3368));
    func_80251294((S*)((u8*)this + 0x33d8), (const S*)(tempFloor + 0x3380));

    // Tail fields (member 0x3330-0x344).
    *(u32*)((u8*)this + 0x33f0) = *(u32*)(tempFloor + 0x3330);
    *(u32*)((u8*)this + 0x33f4) = *(u32*)(tempFloor + 0x3334);
    *(u32*)((u8*)this + 0x33f8) = *(u32*)(tempFloor + 0x3338);
    *(u8*)((u8*)this + 0x33fc) = *(u8*)(tempFloor + 0x333c);
    *(u32*)((u8*)this + 0x3400) = *(u32*)(tempFloor + 0x3340);
    *(u32*)((u8*)this + 0x3404) = *(u32*)(tempFloor + 0x3344);

    __dt__9CFloorMapFv(reinterpret_cast<CFloorMap*>(tempFloor), -1);

    // Register the widget as an IScnRender render callback on its parent scene.
    IScnRender* renderCB = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        renderCB = &mIScnRender;
    }
    reinterpret_cast<CScn*>(mParentRef)->addRenderCB(renderCB, 0xd, 1);
}

extern "C" void func_80251294(S* r3, const S* r4) {
    S* dst = r3;
    const S* src = r4;
    dst->a = src->a;
    dst->b = src->b;
    dst->c = src->c;
    dst->d = src->d;
    dst->e = src->e;
    dst->f = src->f;
}

void CMenuMapSelectSC::Term() {
    CDeviceVI::waitForDrawDone();
    func_804962A0(reinterpret_cast<CScn*>(mParentRef), 1);

    // The `if (this)` is the MWCC idiom that splits mr r4,r31 / beq / addi r4,+0x58.
    IScnRender* renderCB = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        renderCB = &mIScnRender;
    }
    reinterpret_cast<CScn*>(mParentRef)->removeRenderCB(renderCB);

    func_801C40A0(&mTitleAHelp);
    func_8024448C(&mFade);
    func_8024CB94(&mFloorMap);

    lbl_eu_806647C0 = 0;
    func_8008294C__Q22cf13CfGameManagerFv(0);
}

void CMenuMapSelectSC::Move() {
    getInstance__9CTaskGameFv();
    if (func_800426F0__9CTaskGameFv() != 0) {
        return;
    }
    // Gate: when the global mode bit (0x200000) is set, skip the world-map
    // update. The adjacent `goto end; goto body;` pair hits MWCC's
    // branch-over-branch peephole, emitting retail's beq-into-body / b-end.
    if ((lbl_eu_80663E28 & 0x200000) != 0) {
        goto end;
    }
    goto body;
body:
    switch (mState) {
    case 0:
        func_80251560(this);
        break;
    case 1:
        func_802515B8(this);
        break;
    case 2:
        func_80251628(this);
        break;
    case 3:
        if (func_80244510(&mFade) != 0) {
            mState = 4;
        }
        break;
    case 4:
        func_802516DC(this);
        break;
    case 5:
        func_80251D4C(this);
        break;
    case 6:
        func_80251D94(this);
        break;
    case 7:
        mField54 = 1;
        break;
    }
    func_801C3FF0(&mTitleAHelp);
    func_802443E8(&mFade);
    func_8024C1FC(&mFloorMap);
end:
    ;
}

void CMenuMapSelectSC::cbRenderBefore() {
    getInstance__9CTaskGameFv();
    if (func_800426F0__9CTaskGameFv() != 0) {
        return;
    }
    // Gate: when the global mode bit (0x200000) is set, skip the world-map
    // render. The adjacent `goto end; goto body;` pair hits MWCC's
    // branch-over-branch peephole, emitting retail's beq-into-body / b-end.
    if ((lbl_eu_80663E28 & 0x200000) != 0) {
        goto end;
    }
    goto body;
body:
    if (func_8013BE50() == 0) {
        return;
    }
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Raw-storage DrawInfo built/destroyed via C-ABI pre-mangled ct/dt
    // calls to match the retail direct calls (a C++ local would
    // virtual-dispatch its scope-exit destructor and bloat the body).
    u8 drawInfo[0x54];
    __ct__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_80137250((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_8024C8F8(&mFloorMap, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_801C4080(&mTitleAHelp, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_80244460(&mFade, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    __dt__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0], -1);
end:
    ;
}

// Create the world-map (SC) singleton: returns 0 when it already exists,
// otherwise allocates the object (0x3410 bytes), constructs it, stores the
// singleton and registers it as a CProcess child of `parent`.
CMenuMapSelectSC* func_802514D4(CProcess* parent, CProcess* arg) {
    if (lbl_eu_806647C0 != 0) {
        return 0;
    }

    func_80138078(4);
    CMenuMapSelectSC* obj = (CMenuMapSelectSC*)mtl::MemManager::allocate(
        0x3410, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        __ct__CMenuMapSelectSC(obj, arg);
    }
    lbl_eu_806647C0 = (u32)obj;
    obj->Regist(parent, false);
    return (CMenuMapSelectSC*)lbl_eu_806647C0;
}

void func_80251550(){}

// World-map phase 0: wait for the title/help bar and fade to be ready, then
// start the fade-in animation and advance to phase 1.
extern "C" void func_80251560(CMenuMapSelectSC* self) {
    if (func_801C4114(&self->mTitleAHelp) && func_80244508(&self->mFade)) {
        func_80244518(&self->mFade);
        self->mState = 1;
    }
}

// World-map phase 2: once the fade has finished hiding, prepare the floor map
// for the current map id, start its file loads, advance to phase 3 and play
// the confirm sound.
extern "C" void func_802515B8(CMenuMapSelectSC* self) {
    if (func_80244510(&self->mFade) != 0) {
        func_804962A0(reinterpret_cast<CScn*>(self->mParentRef), 0);
        func_8024F1FC(&self->mFloorMap, (u8)lbl_eu_80664184);
        func_8024C104(&self->mFloorMap);
        self->mState = 2;
        func_8013EC6C(1, 0);
    }
}

// World-map phase 2: once the scrollbar is visible, populate the title/help
// bar text for the current map, start its animation and advance to phase 3.
extern "C" void func_80251628(CMenuMapSelectSC* self) {
    if (func_8024CE1C(&self->mFloorMap) != 0) {
        func_801C4654(&self->mTitleAHelp, 0);
        char* name = func_80136190(lbl_eu_8050C5A4, lbl_eu_8050C5A4 + 8, 3);
        func_801C46B4(&self->mTitleAHelp, name);
        if (func_8009CF8C(0x3212) != 0) {
            func_801C46DC(&self->mTitleAHelp, 1);
        }
        func_801C41E8(&self->mTitleAHelp, 0);
        func_801C412C(&self->mTitleAHelp);
        func_8024BE1C(&self->mFloorMap);
        func_80138078(0x6d);
        func_80244538(&self->mFade);
        self->mState = 3;
    }
}

// World-map phase 4: floor-map input handler. Accumulates a per-frame timer,
// then dispatches the current pad state to the cursor/selection helpers. The
// pad layout differs between Classic (bits 21-23/25-26) and Wiimote/Nunchuk
// (bits 4-5/10/12) controllers.
extern "C" void func_802516DC(CMenuMapSelectSC* self) {
    if (func_8024F538(&self->mFloorMap) != 0) {
        return;
    }

    self->mTimer += lbl_eu_806687C4;
    if (self->mTimer > lbl_eu_806687C8) {
        self->mTimer = lbl_eu_806687C8;
    }

    if (cf::CfGameManager::func_80086F9C(-1) != 0) {
        // Classic controller layout.
        if (func_8024F6E0(&self->mFloorMap) != 0) {
            if (func_8024F630(&self->mFloorMap) > 1 && func_8024F6D8(&self->mFloorMap) != 0) {
                func_801C41E8(&self->mTitleAHelp, 0x4b);
            } else {
                func_801C41E8(&self->mTitleAHelp, 0x4c);
            }
        } else {
            if (func_8024F630(&self->mFloorMap) > 1 && func_8024F6D8(&self->mFloorMap) != 0) {
                func_801C41E8(&self->mTitleAHelp, 0x4d);
            } else {
                func_801C41E8(&self->mTitleAHelp, 0x4e);
            }
        }
        func_8024F5C4(&self->mFloorMap, 0);

        CPad* pad = cf::CfGameManager::getCurrentPad();
        if ((pad->mTurboPressButtonFlags & 0x02000000) != 0 &&
            func_8024F630(&self->mFloorMap) > 1 &&
            func_8024F6BC(&self->mFloorMap) != 0 &&
            func_8024F784(&self->mFloorMap) == 0) {
            func_8024CE68(&self->mFloorMap);
            return;
        }
        if ((pad->mTurboPressButtonFlags & 0x04000000) != 0 &&
            func_8024F630(&self->mFloorMap) > 1 &&
            func_8024F6BC(&self->mFloorMap) != 0 &&
            func_8024F784(&self->mFloorMap) == 0) {
            func_8024D23C(&self->mFloorMap);
            return;
        }

        u32 pressed = pad->mPressedButtonFlags;
        if ((pressed & 0x00200000) != 0) {
            func_8024EE50(&self->mFloorMap);
            if (func_8024F54C(&self->mFloorMap) != 0) {
                func_80244518(&self->mFade);
                self->mState = 6;
            }
        } else if ((pressed & 0x00400000) != 0) {
            if (func_8024F554(&self->mFloorMap) != 0) {
                func_8024F55C(&self->mFloorMap);
            } else if (func_8024F784(&self->mFloorMap) == 0) {
                func_80244518(&self->mFade);
                self->mState = 5;
                func_80138078(6);
            } else {
                func_8024EE50(&self->mFloorMap);
            }
        } else if ((pressed & 0x00800000) != 0) {
            if (func_8024F784(&self->mFloorMap) == 0) {
                func_80244518(&self->mFade);
                self->mState = 5;
                func_80138078(6);
            }
        } else if ((pad->mTurboPressButtonFlags & 0x4) != 0) {
            func_8024D614(&self->mFloorMap);
        } else if ((pad->mTurboPressButtonFlags & 0x8) != 0) {
            func_8024DA0C(&self->mFloorMap);
        } else if ((pad->mTurboPressButtonFlags & 0x80000000) != 0) {
            func_8024DE08(&self->mFloorMap);
        } else if ((pad->mTurboPressButtonFlags & 0x2) != 0) {
            func_8024E2BC(&self->mFloorMap);
        } else if ((pressed & 0x01000000) != 0) {
            func_8024F658(&self->mFloorMap);
        } else if ((pressed & 0x200) != 0) {
            func_8024F72C(&self->mFloorMap);
        } else {
            // D-pad held: each direction moves the cursor once per press
            // transition; when no direction is held, confirm the selection.
            u32 flag = 1;
            if ((pad->mHeldButtonFlags & 0x8000) != 0) {
                func_8024E650(&self->mFloorMap);
                flag = 0;
            }
            if ((pad->mHeldButtonFlags & 0x10000) != 0) {
                func_8024E828(&self->mFloorMap);
                flag = 0;
            }
            if ((pad->mHeldButtonFlags & 0x2000) != 0) {
                func_8024EA00(&self->mFloorMap);
                flag = 0;
            }
            if ((pad->mHeldButtonFlags & 0x4000) != 0) {
                func_8024EC24(&self->mFloorMap);
                flag = 0;
            }
            if (flag != 0) {
                func_8024F7CC(&self->mFloorMap);
            }
        }
    } else {
        // Wiimote/Nunchuk layout.
        if (func_8024F6E0(&self->mFloorMap) != 0) {
            if (func_8024F630(&self->mFloorMap) > 1 && func_8024F6D8(&self->mFloorMap) != 0) {
                func_801C41E8(&self->mTitleAHelp, 0x4b);
            } else {
                func_801C41E8(&self->mTitleAHelp, 0x4c);
            }
        } else {
            if (func_8024F630(&self->mFloorMap) > 1 && func_8024F6D8(&self->mFloorMap) != 0) {
                func_801C41E8(&self->mTitleAHelp, 0x4d);
            } else {
                func_801C41E8(&self->mTitleAHelp, 0x4e);
            }
        }
        func_8024F5C4(&self->mFloorMap, 0);

        CPad* pad = cf::CfGameManager::getCurrentPad();
        if ((pad->mHeldButtonFlags & 0x1000) != 0 &&
            func_8024F630(&self->mFloorMap) > 1 &&
            func_8024F6BC(&self->mFloorMap) != 0 &&
            func_8024F784(&self->mFloorMap) == 0) {
            func_801C41E8(&self->mTitleAHelp, 0x4f);
            if (self->mTimer > lbl_eu_806687C4) {
                func_80138078(2);
            }
            self->mTimer = lbl_eu_806687C0;
            func_8024F5C4(&self->mFloorMap, 1);
            if ((pad->mTurboPressButtonFlags & 0x4) != 0) {
                func_8024CE68(&self->mFloorMap);
            } else if ((pad->mTurboPressButtonFlags & 0x8) != 0) {
                func_8024D23C(&self->mFloorMap);
            }
        } else {
            u32 pressed = pad->mPressedButtonFlags;
            if ((pressed & 0x10) != 0) {
                func_8024EE50(&self->mFloorMap);
                if (func_8024F54C(&self->mFloorMap) != 0) {
                    func_80244518(&self->mFade);
                    self->mState = 6;
                }
            } else if ((pressed & 0x20) != 0) {
                if (func_8024F554(&self->mFloorMap) != 0) {
                    func_8024F55C(&self->mFloorMap);
                } else if (func_8024F784(&self->mFloorMap) == 0) {
                    func_80244518(&self->mFade);
                    self->mState = 5;
                    func_80138078(6);
                } else {
                    func_8024EE50(&self->mFloorMap);
                }
            } else if ((pressed & 0x400) != 0) {
                if (func_8024F784(&self->mFloorMap) == 0) {
                    func_80244518(&self->mFade);
                    self->mState = 5;
                    func_80138078(6);
                }
            } else if ((pad->mTurboPressButtonFlags & 0x4) != 0) {
                func_8024D614(&self->mFloorMap);
            } else if ((pad->mTurboPressButtonFlags & 0x8) != 0) {
                func_8024DA0C(&self->mFloorMap);
            } else if ((pad->mTurboPressButtonFlags & 0x80000000) != 0) {
                func_8024DE08(&self->mFloorMap);
            } else if ((pad->mTurboPressButtonFlags & 0x2) != 0) {
                func_8024E2BC(&self->mFloorMap);
            } else if ((pressed & 0x800) != 0) {
                func_8024F658(&self->mFloorMap);
            } else if ((pressed & 0x200) != 0) {
                func_8024F72C(&self->mFloorMap);
            } else {
                u32 flag = 1;
                if ((pad->mHeldButtonFlags & 0x8000) != 0) {
                    func_8024E650(&self->mFloorMap);
                    flag = 0;
                }
                if ((pad->mHeldButtonFlags & 0x10000) != 0) {
                    func_8024E828(&self->mFloorMap);
                    flag = 0;
                }
                if ((pad->mHeldButtonFlags & 0x2000) != 0) {
                    func_8024EA00(&self->mFloorMap);
                    flag = 0;
                }
                if ((pad->mHeldButtonFlags & 0x4000) != 0) {
                    func_8024EC24(&self->mFloorMap);
                    flag = 0;
                }
                if (flag != 0) {
                    func_8024F7CC(&self->mFloorMap);
                }
            }
        }
    }
}

// World-map phase 6: wait for the fade to finish hiding, then reset the
// floor map and move to phase 7 (idle).
extern "C" void func_80251D4C(CMenuMapSelectSC* self) {
    if (func_80244510(&self->mFade)) {
        func_8024CB94(&self->mFloorMap);
        self->mState = 7;
    }
}

// World-map phase 6 variant: after the fade completes, hand the selected
// floor-map cursor index to the game manager and move to phase 7 (idle).
extern "C" void func_80251D94(CMenuMapSelectSC* self) {
    if (func_80244510(&self->mFade)) {
        func_8008413C__Q22cf13CfGameManagerFv(func_8024F54C(&self->mFloorMap), 0);
        self->mState = 7;
    }
}

// IScnRender vtable this-adjusting thunk for cbRenderBefore.
// IScnRender is a non-primary base at offset 0x58 within CMenuMapSelectSC;
// vtable dispatch passes 'this' pointing at +0x58, so the thunk subtracts
// it back and tail-calls the real implementation.
// Retail: subi r3, r3, 0x58; b cbRenderBefore__16CMenuMapSelectSCFv
void func_80251DE8(IScnRender* self) {
    ((void(*)(char*))cbRenderBefore__16CMenuMapSelectSCFv)((char*)self - 0x58);
}

// IScnRender vtable this-adjusting thunk for ~CMenuMapSelectSC.
// Same adjustment as func_80251DE8 but forwards to the destructor, leaving
// r4 (delete flag) as caller leftover.
// Retail: subi r3, r3, 0x58; b __dt__16CMenuMapSelectSCFv
void func_80251DF0(IScnRender* self) {
    ((void(*)(char*))__dt__16CMenuMapSelectSCFv)((char*)self - 0x58);
}
