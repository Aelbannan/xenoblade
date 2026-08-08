// Decompiled: CMenuSymbolMark + CArrow3D
// High-level C++ reconstruction

#include "kyoshin/menu/CMenuSymbolMark.hpp"

// Zero-body stubs for matching
// Each function will be filled in iteratively

// ---------- func_8011D338 ----------
extern "C" u32 func_8011D338() {
    return 0;
}

// ---------- CMenuSymbolMark constructor ----------
CMenuSymbolMark::CMenuSymbolMark(CScn* scn) {
}

// ---------- CMenuSymbolMark destructor ----------
// auto_inline off: the IScnRender vtable thunks (func_8012213C/4C) must
// tail-call the retail symbol, not inline the dtor body (which would add a
// vtable-set prologue the retail thunk doesn't have).
#pragma push
#pragma auto_inline off
CMenuSymbolMark::~CMenuSymbolMark() {
    if (this) {
        __dt__17UnkClass_8045F564Fv(&mUnkClass, -1);
        this->CProcess::~CProcess();
    }
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
void CMenuSymbolMark::cbRenderBefore() {
}

// ---------- func_8011E4C4 ----------
extern "C" void* func_8011E4C4(CScn* scn) {
    return 0;
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
#pragma push
#pragma auto_inline off
CArrow3D::~CArrow3D() {
    if (this) {
        this->CTTask<CArrow3D>::~CTTask();
    }
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
void CArrow3D::Term() {
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
extern "C" void __dt__15CMenuSymbolMarkFv(CMenuSymbolMark* self);
extern "C" void cbRenderBefore__15CMenuSymbolMarkFv(CMenuSymbolMark* self);
extern "C" void __dt__8CArrow3DFv(CArrow3D* self);
extern "C" void cbRenderBefore__8CArrow3DFv(CArrow3D* self);

extern "C" void func_8012213C(void* self) {
    __dt__15CMenuSymbolMarkFv((CMenuSymbolMark*)((char*)self - 0x58));
}

extern "C" void func_80122144(void* self) {
    cbRenderBefore__15CMenuSymbolMarkFv((CMenuSymbolMark*)((char*)self - 0x5C));
}

extern "C" void func_8012214C(void* self) {
    __dt__15CMenuSymbolMarkFv((CMenuSymbolMark*)((char*)self - 0x5C));
}

extern "C" void func_80122154(void* self) {
    cbRenderBefore__8CArrow3DFv((CArrow3D*)((char*)self - 0x54));
}

extern "C" void func_8012215C(void* self) {
    __dt__8CArrow3DFv((CArrow3D*)((char*)self - 0x54));
}