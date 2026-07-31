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

extern "C" void __dt__17UnkClass_8045F564Fv(void* self, int deleteFlag);
struct PTMF;
extern "C" long __ptmf_test(PTMF* ptmf);
extern "C" void __ptmf_scall(...);

// ---------- CMenuSymbolMark destructor ----------
CMenuSymbolMark::~CMenuSymbolMark() {
    if (this) {
        __dt__17UnkClass_8045F564Fv(&mUnkClass, -1);
        this->CProcess::~CProcess();
    }
}

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
CArrow3D::~CArrow3D() {
    if (this) {
        this->CTTask<CArrow3D>::~CTTask();
    }
}

// ---------- CTTask<CArrow3D>::Move ----------
template<>
void CTTask<CArrow3D>::Move() {
    if (__ptmf_test(&mMoveFunc)) {
        __ptmf_scall(this, &mMoveFunc);
    }
}

// ---------- CTTask<CArrow3D>::Draw ----------
template<>
void CTTask<CArrow3D>::Draw() {
    if (__ptmf_test(&mDrawFunc)) {
        __ptmf_scall(this, &mDrawFunc);
    }
}

// ---------- CArrow3D::cbRenderBefore ----------
void CArrow3D::cbRenderBefore() {
}

// ---------- CArrow3D::Move ----------
void CArrow3D::Move() {
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
// Using non-virtual scoped calls to avoid vtable dispatch in the thunk.

extern "C" void func_8012213C(void* self) {
    CMenuSymbolMark* obj = (CMenuSymbolMark*)((char*)self - 0x58);
    obj->CMenuSymbolMark::~CMenuSymbolMark();
}

extern "C" void func_80122144(void* self) {
    CMenuSymbolMark* obj = (CMenuSymbolMark*)((char*)self - 0x5C);
    obj->CMenuSymbolMark::cbRenderBefore();
}

extern "C" void func_8012214C(void* self) {
    CMenuSymbolMark* obj = (CMenuSymbolMark*)((char*)self - 0x5C);
    obj->CMenuSymbolMark::~CMenuSymbolMark();
}

extern "C" void func_80122154(void* self) {
    CArrow3D* obj = (CArrow3D*)((char*)self - 0x54);
    obj->CArrow3D::cbRenderBefore();
}

extern "C" void func_8012215C(void* self) {
    CArrow3D* obj = (CArrow3D*)((char*)self - 0x54);
    obj->CArrow3D::~CArrow3D();
}