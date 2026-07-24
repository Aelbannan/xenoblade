#include "kyoshin/menu/CMenuBattleMode.hpp"

#include "kyoshin/code_80135FDC.hpp"
#include "monolib/util/MemManager.hpp"

// CLEANUP: renamed all unk fields to descriptive names (see header).

extern "C" {
extern char lbl_eu_80503D80[];       // String table base; string offsets 0x10/0x29/0x45 select ARC resource names
extern const f32 lbl_eu_80667C80;    // Initial frame value for the default animation
// Retail calls this unmangled (matches CUIBattleManager.cpp / CMenuPTGauge.cpp
// usage) rather than through the CUICfManager::func_801355F4 mangled name.
nw4r::lyt::ArcResourceAccessor* func_801355F4();  // Shared ARC resource accessor
}

/**
 * Initialises the battle mode layout.
 *
 * Allocates a 0xd00-byte MEM2 region via mLayoutMem, then builds the primary
 * layout (mLayout) and two animation layers (mAnimDefault, mAnimLabel) from the
 * shared battle ARC resource at string offsets 0x10, 0x29, 0x45 respectively.
 * mAnimLabel is disabled at init; mAnimDefault is enabled and its frame set to
 * the initial value. Registers this object as an IScnRender render callback at
 * layer 7 on the owning scene.
 */
void CMenuBattleMode::Init() {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    mLayoutMem.createRegion(handle, 0xd00, lbl_eu_80503D80, 0);
    // Scoped region guard — destructor releases the region when Init finishes
    Class_8045F858 regionGuard(&mLayoutMem);

    nw4r::lyt::ArcResourceAccessor* accessor = func_801355F4();
    func_80136E84(&mLayout, accessor, lbl_eu_80503D80 + 0x10);

    accessor = func_801355F4();
    func_80136F08(mLayout, &mAnimDefault, accessor, lbl_eu_80503D80 + 0x29);

    accessor = func_801355F4();
    func_80136F08(mLayout, &mAnimLabel, accessor, lbl_eu_80503D80 + 0x45);

    mLayout->SetAnimationEnable(mAnimLabel, false);
    mLayout->SetAnimationEnable(mAnimDefault, true);
    mAnimDefault->SetFrame(lbl_eu_80667C80);
    mLayout->Animate(0);

    IScnRender* cb = this;
    mScn->addRenderCB(cb, 7, 0);
    mLayoutMem.func_8045F810();
}

// Thunk for ~CMenuBattleMode accessed via IWorkEvent vtable (offset +0x58).
// Adjusts 'this' from the IWorkEvent subobject back to CMenuBattleMode,
// then tail-calls the real destructor.
extern "C" void func_801A048C(IWorkEvent* self) {
    extern void __dt__15CMenuBattleModeFv(CMenuBattleMode*);
    return __dt__15CMenuBattleModeFv(
        reinterpret_cast<CMenuBattleMode*>(
            reinterpret_cast<char*>(self) - 0x58));
}
// LLM-HARNESS-BEGIN: us-801a1bb0
extern void cbRenderBefore__15CMenuBattleModeFv();
extern "C" void func_801A0494(void* self) { ((void(*)(void*))cbRenderBefore__15CMenuBattleModeFv)((char*)self - 0x5c); }
// LLM-HARNESS-END: us-801a1bb0
// LLM-HARNESS-BEGIN: us-801a1bb8
extern void __dt__15CMenuBattleModeFv();
extern "C" void func_801A049C(void* self) { ((void(*)(void*))__dt__15CMenuBattleModeFv)((char*)self - 0x5c); }
// LLM-HARNESS-END: us-801a1bb8

// LLM-HARNESS-BEGIN: us-801a1a90
extern "C" void __ct__CMenuBattleMode() {}
// LLM-HARNESS-END: us-801a1a90
