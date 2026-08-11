// kyoshin/menu/CMenuLandTelop - land-telop (area-name popup) screen process.
//
// Matching session: Term / cbRenderBefore / func_801453B8 / func_80144EE4 /
// constructor (the retail ctor is the unmangled global __ct__CMenuLandTelop).

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/CMenuLandTelop.hpp"

#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include <nw4r/lyt.h>
#include <nw4r/lyt/lyt_drawInfo.h>
#include <revolution/gx/GXPixel.h>

#include <new>

// cf::CBattleManager::getInstance() - declared in CMenuLandTelop.hpp
// (extern "C" so the pre-mangled retail symbol is emitted verbatim).

// Constructor (unmangled retail symbol). Runs CProcess's ctor then fills the
// CProcess-style region by hand (vtable, run-time ptmf hooks, state fields),
// constructs the embedded MEM2 layout region and clears the entry table.
extern "C" CMenuLandTelop* __ct__CMenuLandTelop(CMenuLandTelop* self,
                                                CScn* scene, u16 opt,
                                                u8 param) {
    __ct__8CProcessFv((CProcess*)self);

    self->mVtab = (u32)lbl_eu_8052D238;

    // Post-increment pointer form: MWCC folds the base @l into a single
    // `lwzu` for the first load and keeps the base register for the rest;
    // the second triple re-reads the same elements through the folded
    // negative offsets (retail re-loads ptmf[0..2] after the first stores).
    const u32* ptmf = __ptmf_null;
    u32 p0, p1, p2, q0, q1, q2;
    p0 = *ptmf++;
    p1 = *ptmf++;
    self->ptmf0[1] = p1;
    self->ptmf0[0] = p0;
    p2 = *ptmf++;
    self->ptmf0[2] = p2;
    q0 = ptmf[-3];
    q1 = ptmf[-2];
    self->ptmf1[1] = q1;
    self->ptmf1[0] = q0;
    q2 = ptmf[-1];
    self->ptmf1[2] = q2;

    self->field_54 = 0;
    self->field_58 = 0;
    self->field_5C = 0;
    self->field_60 = -1;
    self->field_64 = 0;
    self->field_65 = 0;
    self->field_66 = 0;
    self->field_67 = 1;
    self->field_68 = 0;
    // Retail re-stores +0x10 with the dispatch vtable cluster base after the
    // class vtable (same double-store scheme as CMenuBattleEnd); +0x24/+0xac
    // are the IWorkEvent / IScnRender dispatch slots inside that cluster.
    self->mVtab = (u32)lbl_eu_8052E8B0;
    self->mWorkEvent = (u32)lbl_eu_8052E8B0 + 0x24;
    self->mScnRender = (u32)lbl_eu_8052E8B0 + 0xac;
    self->mScene = scene;

    __ct__17UnkClass_8045F564Fv(&self->mMemRegion);

    self->field_88 = 0;
    self->field_8C = opt;
    self->field_8E = param;
    self->field_90 = 0;
    self->field_94 = lbl_eu_806673C8;
    self->field_98 = 0;
    self->mEntries[0].f0 = 0;
    self->mEntries[0].f2 = 0;
    // Clear the remaining 15 entries: MWCC unrolls the pointer loop into an
    // 8-entry block loop plus a scalar remainder (retail trip-count math
    // computes both from the end/start addresses with a redundant cmplw/bge
    // guard - same shape as the CMenuBattleEnd ctor's entry loop).
    for (CLandTelopEntry* e = &self->mEntries[1]; e < &self->mEntries[16];
         e++) {
        e->f0 = 0;
        e->f2 = 0;
    }
    self->field_DA = 0;
    self->field_DC = lbl_eu_806673C8;
    self->field_E0 = 0;
    return self;
}

// Destructor (retail __dt__14CMenuLandTelopFv). Destroy the embedded
// UnkClass_8045F564 region, then the CProcess base (guarded by the nested
// double null-check, an MWCC D2-inlined-into-D1 artifact), then conditional
// operator delete when flags > 0. CSysWinSave idiom: the hpp member dtor
// declaration is satisfied at link time by this global definition.
CMenuLandTelop* __dt__14CMenuLandTelopFv(CMenuLandTelop* _this, int flags) {
    if (_this != 0) {
        __dt__17UnkClass_8045F564Fv(&_this->mMemRegion, -1);
        if (_this != 0) {
            if (_this != 0) {
                __dt__8CProcessFv((CProcess*)_this, 0);
            }
        }
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

void CMenuLandTelop::Init() {
    func_8014548C(this);
    field_E0 = func_80226B94();
    field_67 = 0;
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        cb = reinterpret_cast<IScnRender*>(&mScnRender);
    }
    mScene->addRenderCB(cb, 0xa, 0);
}

void CMenuLandTelop::Term() {
    CDeviceVI::waitForDrawDone();
    // Retail guards a dead null-check on this before picking the +0x70
    // IScnRender subobject (same shape as Init / CMenuQstCnt::Term).
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        cb = reinterpret_cast<IScnRender*>((u8*)this + 0x70);
    }
    mScene->removeRenderCB(cb);
    // Release the layout (deleting-dtor dispatch at vtable slot +8). The
    // extra inner null-check is the D2-inlined-into-D1 artifact; field_54 is
    // cleared regardless.
    if (field_54) {
        delete field_54;
        field_54 = 0;
    }
    mMemRegion.func_8045F778();
    lbl_eu_806641A0 = 0;
}

void CMenuLandTelop::Move() {}

void CMenuLandTelop::cbRenderBefore() {
    CTaskGame::getInstance();
    // Single short-circuit OR so MWCC emits: func test -> bne exit;
    // bit test -> beq continue / b exit (CSystemWindow::Move shape).
    if (CTaskGame::func_800426F0() || (lbl_eu_80663E28 & 0x200000)) return;
    if (func_8013BE50() == 0) return;
    if (lbl_eu_80663E24 & 0xBFE40000u) return;
    if (cf::CfGameManager::func_800829B8()) return;
    if (field_DA != 0) return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Raw-storage DrawInfo built/destroyed via C-ABI pre-mangled ct/dt calls
    // to match the retail direct calls (a C++ local would virtual-dispatch
    // its scope-exit destructor and bloat the body).
    u8 drawInfo[0x54];
    __ct__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_80137250((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_80137038(field_54, (nw4r::lyt::DrawInfo*)&drawInfo[0], 0, 1);
    __dt__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0], -1);
}

// Create the land-telop singleton under `parent`. While a battle is active
// (param == 1) the battle manager's battle list must be empty; if the
// singleton already exists, func_8014504C re-targets it instead. Returns the
// singleton (0 on failure).
CMenuLandTelop* func_80144EE4(CProcess* parent, CScn* scene, u16 opt,
                              u8 param) {
    if (param == 1) {
        CBattleListNode* head =
            ((CBattleListHead*)getInstance__Q22cf14CBattleManagerFv())->list;
        s32 count = 0;
        for (CBattleListNode* p = head->next; p != head; p = p->next) {
            count++;
        }
        if (count != 0) return 0;
    }
    if (lbl_eu_806641A0 != 0) {
        func_8014504C(lbl_eu_806641A0, opt, param);
        return 0;
    }
    mtl::ALLOC_HANDLE workMem = CWorkThreadSystem::getWorkMem();
    CMenuLandTelop* obj =
        (CMenuLandTelop*)mtl::MemManager::allocate(0xe4, workMem);
    if (obj != 0) {
        obj = __ct__CMenuLandTelop(obj, scene, opt, param);
    }
    lbl_eu_806641A0 = reinterpret_cast<CLandTelopGlobal*>(obj);
    reinterpret_cast<CProcess*>(obj)->Regist(parent, 0);
    return reinterpret_cast<CMenuLandTelop*>(lbl_eu_806641A0);
}

// Quest-log gating helper: true while the land-telop state flag at +0x8E
// of the global object is 1.
int func_80144FC8() {
    CLandTelopGlobal* p = lbl_eu_806641A0;
    if (p != 0 && p->field_0x8E == 1) {
        return 1;
    }
    return 0;
}

// Quest-log gating helper: true while the land-telop state flag at +0x8E
// of the global object is 0.
int func_80144FF0() {
    CLandTelopGlobal* p = lbl_eu_806641A0;
    if (p != 0 && p->field_0x8E == 0) {
        return 1;
    }
    return 0;
}

void func_80145018() {
    CLandTelopGlobal* p = lbl_eu_806641A0;
    if (p != 0) {
        p->field_0x64 = 1;
    }
}

unsigned char func_80145030(void) {
    if (lbl_eu_806641A0 != 0) {
        return lbl_eu_806641A0->field_0xE0;
    }
    return 0;
}

// Empty stub: guard so MWCC -inline auto does not fold the call in
// func_80144EE4 away (MWCC_REFERENCE sec 834: empty same-TU stubs inline to
// nothing).
#pragma push
#pragma auto_inline off
void func_8014504C(CLandTelopGlobal* obj, u32 opt, u32 param) {}
#pragma pop

// Scan the entry table for the first pending entry (nonzero f0): move it to
// the display fields, reset the slot, then reset the layout/animation state
// and rebuild the screen via the Init helper. Returns 1 when an entry was
// consumed, 0 otherwise.
int func_801453B8(CMenuLandTelop* self) {
    int found = 0;
    for (u8 i = 0; i < 16; i++) {
        u16 v = self->mEntries[i].f0;
        if (v != 0) {
            self->field_8C = v;
            self->field_8E = self->mEntries[i].f2;
            self->mEntries[i].f0 = 0;
            self->mEntries[i].f2 = 0;
            found = 1;
            break;
        }
    }
    if (found == 0) return 0;
    // Same delete shape as Term() (vtable-slot +8 dispatch + D2 artifact).
    if (self->field_54) {
        delete self->field_54;
        self->field_54 = 0;
    }
    // Load the reset-frame constant into a local first so the lfs (sda21
    // reloc) is emitted before the field stores like retail.
    f32 v = lbl_eu_806673C8;
    self->field_88 = 0;
    self->field_94 = v;
    self->mMemRegion.func_8045F778();
    func_8014548C(self);
    return 1;
}

// Empty stub: guard so MWCC -inline auto does not fold the call in Init
// away (MWCC_REFERENCE sec 834: empty same-TU stubs inline to nothing).
#pragma push
#pragma auto_inline off
void func_8014548C(CMenuLandTelop* self) {}
#pragma pop

void func_80145A90(void* self) { ((void(*)(void*))__dt__14CMenuLandTelopFv)((char*)self - 0x6c); }

void func_80145A98(void* self) { ((void(*)(void*))cbRenderBefore__14CMenuLandTelopFv)((char*)self - 0x70); }

void func_80145AA0(void* self) { ((void(*)(void*))__dt__14CMenuLandTelopFv)((char*)self - 0x70); }

// Resolve the bdat string for the given row: read the column named by the
// string at lbl_eu_805018A8[9]; when it is missing or identical to the
// fallback name at [0xE], return the fallback name.
const char* func_80145AA8(int index) {
    const char* s = getBdatStringColumnValue(lbl_eu_806640E0, &lbl_eu_805018A8[9], index);
    if (s != 0 && std::strcmp(s, &lbl_eu_805018A8[0xe]) != 0) {
        return s;
    }
    return &lbl_eu_805018A8[0xe];
}
