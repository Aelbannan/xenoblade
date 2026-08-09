// Skip-timer widget: shown during cutscene skips.
// Matches CMenuTutorial's pattern (CProcess + IScnRender MI, free adjuster thunks).

#include "kyoshin/menu/CMenuSkipTimer.hpp"

#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/scn/CScn.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/core/CPadManager.hpp"


// ---------- CMenuSkipTimer ctor/dtor ----------

/* Retail constructor symbol (unmangled global). Kept out-of-line so the
 * factory (func_8029EDE4) emits a real bl to it, and returns `this` in r3
 * like a real constructor (retail relies on it). */
__declspec(noinline) CMenuSkipTimer* __ct__CMenuSkipTimer(CMenuSkipTimer* self, CProcess* parent) {
    self->mParentRef = parent;
    return self;
}

CMenuSkipTimer::~CMenuSkipTimer() {}

/* Re-initialise each embedded widget via a temporary object + field copy,
 * following CMenuTutorial::Init / CMenuKizunaTalkList::Init. Each widget is
 * constructed on the stack, its unk4 sub-region rebuilt with its copy-ctor,
 * the remaining scalar fields copied out, the temp destroyed, then the member's
 * load/readFile helper is invoked. Finally the widget is registered as an
 * IScnRender render callback on its parent scene. */
void CMenuSkipTimer::Init() {
    func_8008294C__Q22cf13CfGameManagerFv(1);

    // --- Re-initialise the embedded CBgTex via a temporary ---
    u8 tempBgTex[0x20];
    __ct__CBgTex(tempBgTex, 0);

    __ct__UnkClass_8011C974((u8*)this + 0x64, tempBgTex + 0x4);
    *(u32*)((u8*)this + 0x74) = *(u32*)(tempBgTex + 0x14);
    *(u32*)((u8*)this + 0x78) = *(u32*)(tempBgTex + 0x18);
    *(u8*)((u8*)this + 0x7c)  = *(u8*)(tempBgTex + 0x1c);
    *(u8*)((u8*)this + 0x7d)  = *(u8*)(tempBgTex + 0x1d);
    *(u8*)((u8*)this + 0x7e)  = *(u8*)(tempBgTex + 0x1e);
    __dt__6CBgTexFv(tempBgTex, -1);

    func_801C3C14(&mBgTex);

    // --- Re-initialise the embedded CTitleAHelp via a temporary ---
    char* name = func_80136190(lbl_eu_80510540, lbl_eu_80510540 + 9, 0x35);

    u8 tempTitle[0x38];
    __ct__CTitleAHelp(tempTitle, name, 0x75);

    __ct__UnkClass_8011C974((u8*)this + 0x84, tempTitle + 0x4);
    *(u32*)((u8*)this + 0x94) = *(u32*)(tempTitle + 0x14);
    *(u32*)((u8*)this + 0x98) = *(u32*)(tempTitle + 0x18);
    *(u32*)((u8*)this + 0x9c) = *(u32*)(tempTitle + 0x1c);
    *(u32*)((u8*)this + 0xa0) = *(u32*)(tempTitle + 0x20);
    *(u32*)((u8*)this + 0xa4) = *(u32*)(tempTitle + 0x24);
    *(u8*)((u8*)this + 0xa8)  = *(u8*)(tempTitle + 0x28);
    *(u32*)((u8*)this + 0xac) = *(u32*)(tempTitle + 0x2c);
    *(u32*)((u8*)this + 0xb0) = *(u32*)(tempTitle + 0x30);
    *(u8*)((u8*)this + 0xb4)  = *(u8*)(tempTitle + 0x34);
    *(u8*)((u8*)this + 0xb5)  = *(u8*)(tempTitle + 0x35);
    *(u8*)((u8*)this + 0xb6)  = *(u8*)(tempTitle + 0x36);
    *(u8*)((u8*)this + 0xb7)  = *(u8*)(tempTitle + 0x37);
    __dt__11CTitleAHelpFv(tempTitle, -1);

    CTitleAHelp_load(&mTitleAHelp);

    // --- Re-initialise the embedded CSkipTimer via a temporary ---
    u8 tempSkipTimer[0x98];
    __ct__CSkipTimer(tempSkipTimer);

    __ct__UnkClass_8011C974((u8*)this + 0xbc, tempSkipTimer + 0x4);
    *(u32*)((u8*)this + 0xcc) = *(u32*)(tempSkipTimer + 0x14);
    *(u32*)((u8*)this + 0xd0) = *(u32*)(tempSkipTimer + 0x18);
    *(u32*)((u8*)this + 0xd4) = *(u32*)(tempSkipTimer + 0x1c);
    *(u32*)((u8*)this + 0xd8) = *(u32*)(tempSkipTimer + 0x20);
    *(u32*)((u8*)this + 0xdc) = *(u32*)(tempSkipTimer + 0x24);
    *(u8*)((u8*)this + 0xe0)  = *(u8*)(tempSkipTimer + 0x28);
    *(u8*)((u8*)this + 0xe1)  = *(u8*)(tempSkipTimer + 0x29);
    *(u8*)((u8*)this + 0xe2)  = *(u8*)(tempSkipTimer + 0x2a);
    *(u8*)((u8*)this + 0xe3)  = *(u8*)(tempSkipTimer + 0x2b);
    *(s16*)((u8*)this + 0xe4) = *(s16*)(tempSkipTimer + 0x2c);
    *(s16*)((u8*)this + 0xe6) = *(s16*)(tempSkipTimer + 0x2e);
    *(u8*)((u8*)this + 0xe8)  = *(u8*)(tempSkipTimer + 0x30);

    __ct__UnkClass_8011C974((u8*)this + 0xf0, tempSkipTimer + 0x38);
    *(u32*)((u8*)this + 0x100) = *(u32*)(tempSkipTimer + 0x48);
    *(u32*)((u8*)this + 0x104) = *(u32*)(tempSkipTimer + 0x4c);
    *(u32*)((u8*)this + 0x108) = *(u32*)(tempSkipTimer + 0x50);
    *(u32*)((u8*)this + 0x10c) = *(u32*)(tempSkipTimer + 0x54);
    *(u32*)((u8*)this + 0x110) = *(u32*)(tempSkipTimer + 0x58);
    *(u8*)((u8*)this + 0x114)  = *(u8*)(tempSkipTimer + 0x5c);
    *(u32*)((u8*)this + 0x118) = *(u32*)(tempSkipTimer + 0x60);
    *(u32*)((u8*)this + 0x11c) = *(u32*)(tempSkipTimer + 0x64);
    *(u8*)((u8*)this + 0x120)  = *(u8*)(tempSkipTimer + 0x68);
    *(u8*)((u8*)this + 0x121)  = *(u8*)(tempSkipTimer + 0x69);
    *(u8*)((u8*)this + 0x122)  = *(u8*)(tempSkipTimer + 0x6a);
    *(u8*)((u8*)this + 0x123)  = *(u8*)(tempSkipTimer + 0x6b);
    *(u8*)((u8*)this + 0x124)  = *(u8*)(tempSkipTimer + 0x6c);
    *(u8*)((u8*)this + 0x125)  = *(u8*)(tempSkipTimer + 0x6d);

    __ct__UnkClass_8011C974((u8*)this + 0x12c, tempSkipTimer + 0x74);
    *(u32*)((u8*)this + 0x13c) = *(u32*)(tempSkipTimer + 0x84);
    *(u32*)((u8*)this + 0x140) = *(u32*)(tempSkipTimer + 0x88);
    *(u32*)((u8*)this + 0x144) = *(u32*)(tempSkipTimer + 0x8c);
    *(u8*)((u8*)this + 0x148)  = *(u8*)(tempSkipTimer + 0x90);
    *(u8*)((u8*)this + 0x149)  = *(u8*)(tempSkipTimer + 0x91);
    *(u8*)((u8*)this + 0x14a)  = *(u8*)(tempSkipTimer + 0x92);
    *(u8*)((u8*)this + 0x14b)  = *(u8*)(tempSkipTimer + 0x93);
    *(u8*)((u8*)this + 0x14c)  = *(u8*)(tempSkipTimer + 0x94);
    __dt__10CSkipTimerFv(tempSkipTimer, -1);

    func_8029FBE0(&mTimerData);

    // Register the widget as an IScnRender render callback on its parent scene.
    IScnRender* renderCB = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        renderCB = reinterpret_cast<IScnRender*>((char*)this + 0x58);
    }
    reinterpret_cast<CScn*>(mParentRef)->addRenderCB(renderCB, 0xd, 0);
}

void CMenuSkipTimer::Term() {
    CDeviceVI::waitForDrawDone();

    IScnRender* renderCB = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        renderCB = reinterpret_cast<IScnRender*>((char*)this + 0x58);
    }
    reinterpret_cast<CScn*>(mParentRef)->removeRenderCB(renderCB);

    func_801C3D9C(&mBgTex);
    func_801C40A0(&mTitleAHelp);
    func_8029FE30(&mTimerData);

    lbl_eu_80664A48 = 0;
    func_8008294C__Q22cf13CfGameManagerFv(0);
}

/* Per-frame update of the skip-timer widget.
 * 1. Bail when CTaskGame is paused or the realtime event bit is busy.
 * 2. On the skip button (config-dependent bit), open the skip-timer panel panel
 *    if it is not already engaged, then play the open cue (sound 0x6).
 * 3. Drive the 5-state FSM (mFlag3) and refresh the bg/help bar/layout each
 *    frame. */
void CMenuSkipTimer::Move() {
    if (CTaskGame::getInstance()->func_800426F0() || (lbl_eu_80663E28 & 0x200000)) {
        return;
    }

    CPad* pad = cf::CfGameManager::getCurrentPad();
    bool pressed;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        pressed = (pad->mPressedButtonFlags & 0x800000) != 0;
    } else {
        pressed = (pad->mPressedButtonFlags & 0x400) != 0;
    }

    if (pressed) {
        if (func_802A01F0(&mTimerData) == 0) {
            if (func_800FEDF8() != 0) {
                func_800FF914();
                func_80138078__FUl(6);
            }
            mFlag3 = 4;
            mFlag1 = 1;
        }
    }

    switch (mFlag3) {
    case 0: func_8029EE68(this); break;
    case 1: func_8029EEE0(this); break;
    case 2: func_8029EF30(this); break;
    case 3: func_8029F048(this); break;
    }

    func_801C3D54(&mBgTex);
    func_801C3FF0(&mTitleAHelp);
    func_8029FCDC(&mTimerData);
}

void CMenuSkipTimer::cbRenderBefore() {}


// ---------- Factory: lazily allocate + construct + register the widget ----------

/* Retail ASM:
 *   if (lbl_eu_80664A48 != 0) return 0;
 *   obj = allocate(0x154, getWorkMem());
 *   if (obj != 0) obj = __ct__(obj, parent);
 *   lbl_eu_80664A48 = obj; obj->Regist(self, 0); return lbl_eu_80664A48;
 */
CMenuSkipTimer* func_8029EDE4(CProcess* self, CProcess* parent) {
    if (lbl_eu_80664A48 != 0) {
        return 0;
    }
    CMenuSkipTimer* obj = (CMenuSkipTimer*)mtl::MemManager::allocate(
        0x154, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        obj = __ct__CMenuSkipTimer(obj, parent);
    }
    lbl_eu_80664A48 = (unsigned long)obj;
    obj->Regist(self, 0x0);
    return (CMenuSkipTimer*)lbl_eu_80664A48;
}


// ---------- Idle + advance helpers ----------

// When the title/help bar is idle and the timer widget is mid-anim, mark the
// widget as having reached phase 2 (mFlag3 at 0x150).
extern "C" void func_8029EEE0(CMenuSkipTimer* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        func_8029FF00(&self->mTimerData) != 0) {
        self->mFlag3 = 2;
    }
}

// Same idle+advance check, but advances to phase 1 (writes mFlag1 at 0x54).
extern "C" void func_8029F048(CMenuSkipTimer* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        func_8029FF00(&self->mTimerData) != 0) {
        self->mFlag1 = 1;
    }
}

extern "C" int isIdle__11CTitleAHelpFv(CTitleAHelp* h);

// Retail-unmangled C-linkage callees in other TUs. They are emitted by retail
// under their bare func_ names (no Itanium mangling), so they are declared
// extern "C" here (same pattern as CMenuTutorial.cpp). Return types are int so
// the `!= 0` tests compile to cmpwi rather than an rlwinm byte-mask.
extern "C" {
int func_801C3E34(CBgTex* self);
int func_801C4114(CTitleAHelp* self);
int func_8029FEBC(CSkipTimer* self);
void func_801C412C(CTitleAHelp* self);
void func_802A0008(CSkipTimer* self);
}

// When the bg texture, title/help bar, and skip-timer panel are all ready,
// engage the skip timer (start anim, flag phase) and play the open sound.
extern "C" void func_8029EE68(CMenuSkipTimer* self) {
    if (func_801C3E34(&self->mBgTex) != 0 &&
        func_801C4114(&self->mTitleAHelp) != 0 &&
        func_8029FEBC(&self->mTimerData) != 0) {
        func_801C412C(&self->mTitleAHelp);
        func_802A0008(&self->mTimerData);
        self->mFlag3 = 1;
        func_80138078__FUl(0x6d);
    }
}


// ---------- IScnRender vtable this-adjusting thunks ----------

/**
 * IScnRender vtable this-adjusting thunk: subi r3, r3, 0x58; b cbRenderBefore.
 */
void func_8029F098(IScnRender* sub) {
    ((CMenuSkipTimer*)((char*)sub - 0x58))->cbRenderBefore();
}

/**
 * IScnRender vtable this-adjusting thunk: subi r3, r3, 0x58; b __dt__.
 * Tail-calls the destructor, leaving r4 (delete flag) as caller leftover.
 */
extern "C" void __dt__14CMenuSkipTimerFv(CMenuSkipTimer*);
void func_8029F0A0(IScnRender* sub) {
    __dt__14CMenuSkipTimerFv((CMenuSkipTimer*)((char*)sub - 0x58));
}

extern "C" unsigned long func_8029EE58(void) { return lbl_eu_80664A48 != 0; }