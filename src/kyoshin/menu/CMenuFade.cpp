#include "kyoshin/menu/CMenuFade.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"

#include <revolution/gx/GXPixel.h>

extern "C" {
extern u32 lbl_eu_80663E28;
extern CMenuFade* lbl_eu_80663FA0;
}

CMenuFade::CMenuFade(CScn* pScn, int p5, int p6, float f1, float f2, float f3) {
    mScn = pScn;
    field_0x54 = 0;
    pad55[0] = 0;
    mLayoutMem.~UnkClass_8045F564();
    new (&mLayoutMem) UnkClass_8045F564();
    mLayout = nullptr;
    field_0x78 = 0;
    field_0x7C = 0.0f;
    field_0x80 = f1;
    field_0x84 = f2;
    field_0x88 = f3;
    field_0x8C = 0;
    field_0x90 = p5;
    field_0x94 = 1;
    field_0x98 = p6;
}

CMenuFade::~CMenuFade() {

}

void CMenuFade::Draw() {

}

void CMenuFade::Term() {
    CDeviceVI::waitForDrawDone();
    mScn->removeRenderCB(this);
    if (mLayout) {
        delete mLayout;
        mLayout = 0;
    }
    mLayoutMem.func_8045F778();
    lbl_eu_80663FA0 = 0;
}

int func_80113E1C() {
    return reinterpret_cast<int>(lbl_eu_80663FA0);
}
u8 func_80113E24(void* pthis) {
    return reinterpret_cast<CMenuFade*>(pthis)->field_0x94;
}

// Thunks for IWorkEvent/IScnRender subobject adjustment
void __dt__9CMenuFadeFv(CMenuFade*);
void func_80113E38(CMenuFade* p) {
    __dt__9CMenuFadeFv(reinterpret_cast<CMenuFade*>(reinterpret_cast<u8*>(p) - 0x58));
}
void cbRenderBefore__9CMenuFadeFv(CMenuFade*);
void func_80113E40(void* self) {
    cbRenderBefore__9CMenuFadeFv(reinterpret_cast<CMenuFade*>(reinterpret_cast<u8*>(self) - 0x5c));
}
void func_80113E48(void* arg0) {
    __dt__9CMenuFadeFv(reinterpret_cast<CMenuFade*>(reinterpret_cast<u8*>(arg0) - 0x5C));
}

void CMenuFade::cbRenderBefore() {
    // Skip rendering when game is paused or menu fade is suppressed
    if (CTaskGame::getInstance()->func_800426F0()) return;
    if (lbl_eu_80663E28 & 0x00200000) return;

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    func_80137250(&drawInfo);
    func_80137038(mLayout, &drawInfo, 0, 1);
}

CMenuFade* func_80113C84(CProcess* parent, CScn* pScn, int p5, int p6, float f1, float f2, float f3) {
    CMenuFade* fade = lbl_eu_80663FA0;
    if (fade != nullptr) {
        // Already exists — configure and return
        f32 frameSize = (f32)fade->mLayout->GetFrameSize();
        fade->field_0x88 = (f3 > 0.0f) ? (frameSize / f3) : 0.0f;
        fade->field_0x8C = 2;
        fade->field_0x94 = 1;
        fade->field_0x98 = p6;
        if (p6 == 0) {
            fade->mLayout->func_80137B44("in_fade", 0xFF);
        } else if (p6 == 1) {
            fade->mLayout->func_80137B44("in_fade", -1);
        }
        return nullptr;
    }

    // Allocate new CMenuFade
    void* mem = CWorkThreadSystem::getWorkMem()->allocate(sizeof(CMenuFade), 0x9c);
    fade = new (mem) CMenuFade(pScn, p5, p6, f1, f2, f3);
    lbl_eu_80663FA0 = fade;
    fade->Regist(parent, false);
    return fade;
}
