// Auto-scaffolded catalog TU for kyoshin/menu/CMenuQuestLog
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/CMenuQuestLog.hpp"

#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

extern "C" void __dt__13CMenuQuestLogFv(void*, int);

// Retail constructor symbol (unmangled global). Out-of-line stub so the
// factory (func_8011CCE0) emits a real `bl` to it; returns `this` in r3 like
// a real constructor (retail relies on it). C linkage inherited from the
// header declaration.
__declspec(noinline) CMenuQuestLog* __ct__CMenuQuestLog(CMenuQuestLog* _this, CProcess* parent, u32 arg2) {
    return _this;
}

CMenuQuestLog::~CMenuQuestLog() {}

void CMenuQuestLog::Init() {}

void __ct__UnkClass_8011C974(){}

void func_8011C998(){}

void CMenuQuestLog::Term() {}

void CMenuQuestLog::Move() {}

__declspec(noinline) void CMenuQuestLog::cbRenderBefore() {}

// ---------------------------------------------------------------------------
// ---- func_8011CCE0 (us-8011d7bc) ------------------------------------------
// Factory: lazily allocate + construct the single quest-log menu instance and
// register it as a child of `self`. Returns the stored singleton (or 0 if it
// already exists). Regist is called even when the allocation failed, matching
// retail.
// ---------------------------------------------------------------------------
extern "C" void Regist__8CProcessFP8CProcessb(void* self, CProcess* parent, bool b);

CMenuQuestLog* func_8011CCE0(CProcess* self, CProcess* parent, u32 arg2) {
    if (lbl_eu_80663FC0 != 0) {
        return 0;
    }
    CMenuQuestLog* obj = (CMenuQuestLog*)mtl::MemManager::allocate(
        0x2288, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        obj = __ct__CMenuQuestLog(obj, parent, arg2);
    }
    lbl_eu_80663FC0 = obj;
    Regist__8CProcessFP8CProcessb(obj, self, false);
    return lbl_eu_80663FC0;
}

// (lbl_eu_80663FC0 != 0) - retail lwz sda21; subic; subfe
extern "C" bool func_8011CD5C() { return lbl_eu_80663FC0 != 0; }

void func_8011CD6C(){}

void func_8011CDF4(){}

void func_8011CE44(){}

void func_8011D03C(){}

void func_8011D08C(unsigned char* self)
{
    unsigned char* base = self;
    if (isIdle__11CTitleAHelpFv(base + 0x80) && func_80227CCC(base + 0xB8)) {
        unsigned int value = func_80228394(base + 0xB8);
        func_80229768(base + 0x2240, static_cast<unsigned short>(value));
        func_80229510(base + 0x2240);
        base[0x2284] = 5;
    }
}

// ---------------------------------------------------------------------------
// ---- func_8011D0FC (us-8011dbd8) ------------------------------------------
// When the quest log info is done, set the title-help mode to 0x3d, close the
// title help, finish the log info and advance to state 6.
// ---------------------------------------------------------------------------
void func_8011D0FC(CMenuQuestLog* self) {
    if (func_802296D0(&self->mQstLogInfo)) {
        func_801C41E8(&self->mTitleAHelp, 0x3d);
        func_801C416C(&self->mTitleAHelp);
        func_802296E0(&self->mQstLogInfo);
        self->mState = 6;
    }
}

// ---------------------------------------------------------------------------
// ---- func_8011D158 (us-8011dc34) ------------------------------------------
// When the title help is idle and the quest log info is done, advance to
// state 7.
// ---------------------------------------------------------------------------
void func_8011D158(CMenuQuestLog* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) && func_802296D8(&self->mQstLogInfo)) {
        self->mState = 7;
    }
}

void func_8011D1A8(){}

// ---------------------------------------------------------------------------
// ---- func_8011D22C (us-8011dd08) ------------------------------------------
// When the title help is idle and the quest log info is done, set the
// title-help mode to 0x3c, close the title help, re-sort the list and advance
// to state 9.
// ---------------------------------------------------------------------------
void func_8011D22C(CMenuQuestLog* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) && func_802296D8(&self->mQstLogInfo)) {
        func_801C41E8(&self->mTitleAHelp, 0x3c);
        func_801C416C(&self->mTitleAHelp);
        func_80227CDC(&self->mQstLogList);
        self->mState = 9;
    }
}

// ---------------------------------------------------------------------------
// ---- func_8011D298 (us-8011dd74) ------------------------------------------
// When the title help is idle and sorting is enabled, advance to state 2.
// ---------------------------------------------------------------------------
void func_8011D298(CMenuQuestLog* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) && func_80227CCC(&self->mQstLogList)) {
        self->mState = 2;
    }
}

// Adjusting thunk: upcasts from a base sub-object (at offset +0x58 within CMenuQuestLog)
// to the full CMenuQuestLog, then tail-calls cbRenderBefore.
extern "C" void func_8011D2E8(void* self) {
    reinterpret_cast<CMenuQuestLog*>(static_cast<char*>(self) - 0x58)->cbRenderBefore();
}

// Adjusting thunk: upcasts from a base sub-object (at offset +0x58 within CMenuQuestLog)
// to the full CMenuQuestLog, then tail-calls the destructor.
extern "C" void func_8011D2F0(void* self) {
    ((void(*)(void*))__dt__13CMenuQuestLogFv)((char*)self - 0x58);
}

void __dt__8011D2F8(){}
