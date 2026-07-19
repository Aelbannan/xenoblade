#include "kyoshin/menu/CMenuBattleMode.hpp"

#include "kyoshin/code_80135FDC.hpp"
#include "monolib/util/MemManager.hpp"

// Batch 2026-07-14l: menu-battlemode-init owns Init exclusively.

extern "C" {
extern char lbl_eu_80503D80[];
extern const f32 lbl_eu_80667C80;
// Retail calls this unmangled (matches CUIBattleManager.cpp / CMenuPTGauge.cpp
// usage) rather than through the CUICfManager::func_801355F4 mangled name.
nw4r::lyt::ArcResourceAccessor* func_801355F4();
}

void CMenuBattleMode::Init() {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    unk60.createRegion(handle, 0xd00, lbl_eu_80503D80, 0);
    Class_8045F858 unkClass8045F858(&unk60);

    nw4r::lyt::ArcResourceAccessor* accessor = func_801355F4();
    func_80136E84(&unk74, accessor, lbl_eu_80503D80 + 0x10);

    accessor = func_801355F4();
    func_80136F08(unk74, &unk78, accessor, lbl_eu_80503D80 + 0x29);

    accessor = func_801355F4();
    func_80136F08(unk74, &unk7C, accessor, lbl_eu_80503D80 + 0x45);

    unk74->SetAnimationEnable(unk7C, false);
    unk74->SetAnimationEnable(unk78, true);
    unk78->SetFrame(lbl_eu_80667C80);
    unk74->Animate(0);

    IScnRender* cb = this;
    unk70->addRenderCB(cb, 7, 0);
    unk60.func_8045F810();
}

// LLM-HARNESS-BEGIN: us-801a1ba8
extern "C" bool func_801A048C() { return false; }
// LLM-HARNESS-END: us-801a1ba8
// LLM-HARNESS-BEGIN: us-801a1bb0
extern "C" bool func_801A0494() { return false; }
// LLM-HARNESS-END: us-801a1bb0
// LLM-HARNESS-BEGIN: us-801a1bb8
extern "C" bool func_801A049C() { return false; }
// LLM-HARNESS-END: us-801a1bb8

// LLM-HARNESS-BEGIN: us-801a1a90
extern "C" void ct_CMenuBattleMode() {}
// LLM-HARNESS-END: us-801a1a90
