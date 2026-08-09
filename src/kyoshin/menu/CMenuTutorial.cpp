// Decomp of kyoshin/menu/CMenuTutorial.
// Tutorial menu screen process (CProcess + IScnRender MI).

#include "kyoshin/menu/CMenuTutorial.hpp"

#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"

extern unsigned long lbl_eu_80664A28;

/* Retail constructor symbol (unmangled global). Not yet fully matched.
 * Kept out-of-line so the factory (func_8029A5DC) emits a real bl to it, and
 * returns `this` in r3 like a real constructor (retail relies on it). */
__declspec(noinline) CMenuTutorial* __ct__CMenuTutorial(CMenuTutorial* self, CProcess* parent, u32 arg2) {
    return self;
}

CMenuTutorial::~CMenuTutorial() {}

void CMenuTutorial::Init() {}

void CMenuTutorial::Term() {}

void CMenuTutorial::Move() {}

void CMenuTutorial::Draw() {}

void CMenuTutorial::cbRenderBefore() {}

/*
 * When the title/help bar is idle and the tutorial widget has finished its
 * current animation, mark the tutorial as having reached phase 2.
 */
// Retail callee symbols are unmangled globals; declared as int so the result
// is compared with cmpwi (no byte mask) like retail.
extern int func_8029ACB4(CTutorial* c);
extern int isIdle__11CTitleAHelpFv(CTitleAHelp* h);
void func_8029A764(CMenuTutorial* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        func_8029ACB4(&self->mTutorial) != 0) {
        self->mIsInitialised = 2;
    }
}

/*
 * Factory: lazily allocate + construct the single tutorial menu instance and
 * register it as a child of `self`. Returns the stored instance (or 0 if it
 * already exists).
 */
CMenuTutorial* func_8029A5DC(CProcess* self, CProcess* parent, u32 arg2) {
    if (lbl_eu_80664A28 != 0) {
        return 0;
    }
    CMenuTutorial* obj = (CMenuTutorial*)mtl::MemManager::allocate(
        0xf4, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        obj = __ct__CMenuTutorial(obj, parent, arg2);
    }
    lbl_eu_80664A28 = (unsigned long)obj;
    obj->Regist(self, 0x0);
    return (CMenuTutorial*)lbl_eu_80664A28;
}

void func_8029A668() {}

void func_8029A7B4() {}

/*
 * Same idle+advance check as func_8029A764, but advances the tutorial to
 * phase 1 (writes the state byte at offset 0x54).
 */
void func_8029A8D4(CMenuTutorial* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        func_8029ACB4(&self->mTutorial) != 0) {
        self->field_54 = 1;
    }
}

/**
 * IScnRender vtable this-adjusting thunk: subi r3, r3, 0x58; b cbRenderBefore.
 */
void func_8029A924(IScnRender* sub) {
    ((CMenuTutorial*)((char*)sub - 0x58))->cbRenderBefore();
}

/**
 * IScnRender vtable this-adjusting thunk: subi r3, r3, 0x58; b __dt__.
 * Tail-calls the destructor, leaving r4 (delete flag) as caller leftover.
 */
extern void __dt__13CMenuTutorialFv(CMenuTutorial*);
void func_8029A92C(IScnRender* sub) {
    __dt__13CMenuTutorialFv((CMenuTutorial*)((char*)sub - 0x58));
}

extern "C" unsigned long func_8029A658(void) { return lbl_eu_80664A28 != 0; }