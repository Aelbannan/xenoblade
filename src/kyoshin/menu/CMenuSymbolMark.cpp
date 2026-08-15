// Decompiled: CMenuSymbolMark + CArrow3D
// High-level C++ reconstruction

#include "kyoshin/menu/CMenuSymbolMark.hpp"
#include <nw4r/lyt/lyt_drawInfo.h>   // nw4r::lyt::DrawInfo
#include <nw4r/lyt/lyt_layout.h>    // nw4r::lyt::Layout

// Zero-body stubs for matching
// Each function will be filled in iteratively

// ---------- func_8011D338 ----------
extern "C" u32 func_8011D338() {
    return 0;
}

// Render-gate globals/callees (declared locally to avoid header conflicts).
extern "C" void* getInstance__9CTaskGameFv();
extern "C" void* func_800426F0__9CTaskGameFv(void* self);
extern "C" int func_8013BE50();
struct CMenuGimmickGlobal { u8 mPad[0x214]; u32 field_214; };
extern "C" CMenuGimmickGlobal* getUnk80664658();
extern u32 lbl_eu_80663E28;
extern u32 lbl_eu_80663E24;
extern "C" void func_80137250(void* drawInfo);
extern "C" void func_80137038(void* layout, void* drawInfo, int a, int b);
extern "C" void __dt__Q34nw4r3lyt8DrawInfoFv(void* self, int flags);
extern "C" void __dt__Q34nw4r3lyt6LayoutFv(void* self, int flags);

// Retail singletons used by the menu create helpers.
extern u32 lbl_eu_80663FC8;                    // installed CMenuSymbolMark pointer
extern "C" void* getWorkMem__17CWorkThreadSystemFv();
extern "C" CMenuSymbolMark* __ct__CMenuSymbolMark(CMenuSymbolMark* self, CProcess* parent);

extern "C" void __dt__17UnkClass_8045F564Fv(void* self, int flags);
extern "C" void __dt__8CProcessFv(void* self, int flags);
extern "C" void __dl__FPv(void* p);

// ---------- CMenuSymbolMark constructor ----------
CMenuSymbolMark::CMenuSymbolMark(CScn* scn) {
}

// ---------- CMenuSymbolMark destructor ----------
// Retail: if (self == 0) goto end; __dt__17UnkClass_8045F564Fv(&self->mUnkClass, -1);
// if (self != 0) { if (self != 0) { __dt__8CProcessFv(self, 0); } }
// if (flags > 0) __dl__FPv(self); end: return self; — the redundant
// `if (self != 0)` re-check reproduces the retail's dead double-beq
// (CMenuBattleChain/CMenuArtsSelect idiom).
#pragma push
#pragma auto_inline off
extern "C" void* __dt__15CMenuSymbolMarkFv(CMenuSymbolMark* self, int flags) {
    if (self == 0) goto end;
    __dt__17UnkClass_8045F564Fv(&self->mUnkClass, -1);
    if (self != 0) {
        if (self != 0) {
            __dt__8CProcessFv(self, 0);
        }
    }
    if (flags > 0) __dl__FPv(self);
end:
    return self;
}
#pragma pop

// ---------- CMenuSymbolMark::Init ----------
void CMenuSymbolMark::Init() {
}

// ---------- CMenuSymbolMark::Term ----------
void CMenuSymbolMark::Term() {
}

// ---------- CMenuSymbolMark::Move ----------
void CMenuSymbolMark::Move() {
}

// ---------- CMenuSymbolMark::cbRenderBefore ----------
// Render gate chain: task instance check, window flag (bit 0x4000), archive
// pointer, scene gate (func_8013BE50), view flag (bit 0x800), syswin flag
// (0xAFA4 mask), entry count, then render all visible entries through a
// DrawInfo.
extern "C" void cbRenderBefore__15CMenuSymbolMarkFv(CMenuSymbolMark* self) {
    if (func_800426F0__9CTaskGameFv(getInstance__9CTaskGameFv()) != 0) return;
    if (lbl_eu_80663E28 & 0x200000) goto cont;
    return;
cont:
    if (self->mArchiveFP == 0) return;
    if (func_8013BE50() == 0) return;
    if (!(getUnk80664658()->field_214 & 0x100000)) return;
    if (lbl_eu_80663E24 & 0xAFA40000) return;
    if (self->mEntryCount == 0) return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    func_80137250(&drawInfo);
    for (int i = 0; i < 16; i++) {
        if (self->mEntries[i].flag0) {
            func_80137038(self->mEntries[i].layout, &drawInfo, self->mEntries[i].flag2, 1);
        }
    }
    drawInfo.~DrawInfo();
}

// ---------- func_8011E4C4 ----------
extern "C" void* func_8011E4C4(CProcess* scn, CProcess* parent) {
    if (lbl_eu_80663FC8 != 0) {
        return 0;
    }
    CMenuSymbolMark* obj = (CMenuSymbolMark*)mtl::MemManager::allocate(
        2228, (u32)getWorkMem__17CWorkThreadSystemFv());
    if (obj != 0) {
        obj = __ct__CMenuSymbolMark(obj, parent);
    }
    lbl_eu_80663FC8 = (u32)obj;
    reinterpret_cast<CProcess*>(obj)->Regist(scn, false);
    return (void*)lbl_eu_80663FC8;
}

// ---------- func_8011E540 ----------
void func_8011E540(CMenuSymbolMark* self, u32 a, void* b, void* c, u32 d, u32 e) {
}

// ---------- func_8011E778 ----------
void func_8011E778(CMenuSymbolMark* self, SymbolMarkEntry* entry, void* pos, void* c, u32 d) {
}

// ---------- func_8011EA98 ----------
void func_8011EA98(CMenuSymbolMark* self) {
}

// ---------- func_8011EBA8 ----------
void func_8011EBA8(CMenuSymbolMark* self) {
}

// ---------- func_8011EC94 ----------
void func_8011EC94(CMenuSymbolMark* self) {
}

// ---------- func_8011EDDC ----------
void func_8011EDDC(CMenuSymbolMark* self) {
}

// ---------- func_8011EFB0 ----------
void func_8011EFB0(CMenuSymbolMark* self) {
}

// ---------- func_8011F8F8 ----------
void func_8011F8F8(CMenuSymbolMark* self) {
}

// ---------- func_8011FB68 ----------
void func_8011FB68(CMenuSymbolMark* self) {
}

// ---------- func_801209BC ----------
void func_801209BC(CMenuSymbolMark* self) {
}

// ---------- CArrow3D destructor ----------
// Retail: if (self == 0) goto end; if (self != 0) { if (self != 0) {
// __dt__8CProcessFv(self, 0); } } if (flags > 0) __dl__FPv(self);
// end: return self; — the redundant `if (self != 0)` re-check reproduces the
// retail's dead double-beq (CMenuBattleChain/CMenuArtsSelect idiom).
#pragma push
#pragma auto_inline off
extern "C" void* __dt__8CArrow3DFv(CArrow3D* self, int flags) {
    if (self == 0) goto end;
    if (self != 0) {
        __dt__8CProcessFv(self, 0);
    }
    if (flags > 0) __dl__FPv(self);
end:
    return self;
}
#pragma pop

// ---------- CTTask<CArrow3D>::Move ----------
// MWCC lowers the PTMF dispatch to the retail __ptmf_test/__ptmf_scall calls
// (r3=this, r12=&ptmf) - same shape as CTaskGameEff/CMainMenu. (The old
// explicit __ptmf_test/__ptmf_scall calls passed the ptmf in r4 and never
// matched retail.) static_cast mirrors CTTask.hpp's inline body.
template<>
void CTTask<CArrow3D>::Move() {
    if (mMoveFunc) {
        (static_cast<CArrow3D*>(this)->*mMoveFunc)();
    }
}

// ---------- CTTask<CArrow3D>::Draw ----------
template<>
void CTTask<CArrow3D>::Draw() {
    if (mDrawFunc) {
        (static_cast<CArrow3D*>(this)->*mDrawFunc)();
    }
}

// ---------- CTTask<CArrow3D>::~CTTask ----------
// Referenced by the explicit base-dtor call in CArrow3D::~CArrow3D; emits the
// retail __dt__17CTTask<8CArrow3D>Fv body.
template<>
CTTask<CArrow3D>::~CTTask() {}

// ---------- CArrow3D::cbRenderBefore ----------
#pragma push
#pragma auto_inline off
void CArrow3D::cbRenderBefore() {
}
#pragma pop

// ---------- CArrow3D::Move (retail empty override, kept as extern "C" stub;
// the class deliberately declares no Move() so the vtable keeps the
// CTTask<CArrow3D>::Move dispatch - see CMenuSymbolMark.hpp) ----------
extern "C" void Move__8CArrow3DFv() {
}

// ---------- CArrow3D::Term ----------
// Wait for the draw to finish, release the layout (vtable+0x8 with flags=1),
// then remove the IScnRender callback at +0x54 from the current scene.
namespace CDeviceVI {
extern "C" void waitForDrawDone();
}
extern "C" void removeRenderCB__4CScnFP10IScnRender(void* self, void* cb);
extern u8* lbl_eu_80663E14;
void CArrow3D::Term() {
    CDeviceVI::waitForDrawDone();
    // delete through the Layout type: the retail dispatches vtable slot 2
    // with the deleting flag 1 (polymorphic delete), and the delete's own
    // null-check is the dead second beq (func_80285ABC pattern).
    nw4r::lyt::Layout* layout = (nw4r::lyt::Layout*)mLayout;
    if (layout) {
        delete layout;
        mLayout = 0;
    }
    if (this) {
        void* cb = (u8*)this + 0x54;
        removeRenderCB__4CScnFP10IScnRender(lbl_eu_80663E14, cb);
    }
}

// ---------- CArrow3D::Init ----------
void CArrow3D::Init() {
}

// ---------- Thunk functions ----------
// These are vtable thunks for multiple inheritance.
// When called through the IWorkEvent vtable (offset 0x58) or IScnRender vtable (offset 0x5C),
// the this pointer needs adjustment before calling the actual implementation.
// Retail emits 8-byte tail calls (addi this-adjust + branch). The C++ member-scoped
// call form would inline the tiny bodies, so call the retail symbols directly as
// extern "C" functions (same pattern as the CMenuMapSelect/CMapSel dtors).
// Note: the dtors are declared 1-arg here (matching the retail thunk ABI - only
// r3 is set before the tail branch; the flags arg is caller-leftover, as retail).
extern "C" void* __dt__15CMenuSymbolMarkFv(CMenuSymbolMark* self, int flags);
extern "C" void cbRenderBefore__15CMenuSymbolMarkFv(CMenuSymbolMark* self);
extern "C" void cbRenderBefore__8CArrow3DFv(CArrow3D* self);

extern "C" void func_8012213C(void* self) {
    __dt__15CMenuSymbolMarkFv((CMenuSymbolMark*)((char*)self - 0x58), 0);
}

extern "C" void func_80122144(void* self) {
    cbRenderBefore__15CMenuSymbolMarkFv((CMenuSymbolMark*)((char*)self - 0x5C));
}

extern "C" void func_8012214C(void* self) {
    __dt__15CMenuSymbolMarkFv((CMenuSymbolMark*)((char*)self - 0x5C), 0);
}

extern "C" void func_80122154(void* self) {
    cbRenderBefore__8CArrow3DFv((CArrow3D*)((char*)self - 0x54));
}

extern "C" void func_8012215C(void* self) {
    __dt__8CArrow3DFv((CArrow3D*)((char*)self - 0x54), 0);
}