#include "kyoshin/menu/CMenuFade.hpp"

#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include <revolution/gx/GXPixel.h>

// Forward-declare CTaskGame to avoid pulling in the real IScnRender
// (CMenuFade.hpp declares a local IScnRender without a destructor to
// control codegen; the real one would conflict).
class CTaskGame {
public:
    static CTaskGame* getInstance();
    bool func_800426F0();
};

// Layout helper from code_80135FDC (not yet declared in the header).
void func_80137B44(nw4r::lyt::Layout* layout, const char* paneName, int value);

extern "C" {
extern u32 lbl_eu_80663E28;
extern CMenuFade* lbl_eu_80663FA0;
extern const u8 lbl_eu_8052BF70[];   // CTTask<IUICf> vtable
extern const u8 lbl_eu_8052C540[];   // CMenuFade vtable
extern const u8 __ptmf_null[12];     // null pointer-to-member-function
extern void __ct__8CProcessFv(void*);
extern void __ct__17UnkClass_8045F564Fv(void*);

extern const f32 lbl_eu_80667058;    // 0.0f
extern const f32 lbl_eu_8066705C;    // 1.0f
extern const f64 lbl_eu_80667068;    // 4503599627370496.0 (u32→f32 magic)
}

CMenuFade::CMenuFade(CScn* pScn, int p5, int p6, float f1, float f2, float f3) {
    // Manual construction following the retail codegen — vtable/PTMF
    // initialization must match the exact sequence for byte-identical output.
    u8* self = reinterpret_cast<u8*>(this);

    // CProcess base constructor
    __ct__8CProcessFv(this);

    // Intermediate vtable (CTTask<IUICf> level)
    reinterpret_cast<u32*>(self)[0x10 / 4] = reinterpret_cast<u32>(lbl_eu_8052BF70);

    // Copy __ptmf_null to PTMF slots at 0x3C and 0x48 (12 bytes each)
    u32 ptmf0 = reinterpret_cast<const u32*>(__ptmf_null)[0];
    u32 ptmf1 = reinterpret_cast<const u32*>(__ptmf_null)[1];
    u32 ptmf2 = reinterpret_cast<const u32*>(__ptmf_null)[2];
    reinterpret_cast<u32*>(self)[0x3C / 4] = ptmf0;
    reinterpret_cast<u32*>(self)[0x40 / 4] = ptmf1;
    reinterpret_cast<u32*>(self)[0x44 / 4] = ptmf2;
    reinterpret_cast<u32*>(self)[0x48 / 4] = ptmf0;
    reinterpret_cast<u32*>(self)[0x4C / 4] = ptmf1;
    reinterpret_cast<u32*>(self)[0x50 / 4] = ptmf2;

    // Zero the CMenuFadeBase fields
    field_0x54 = 0;
    pad55[0] = 0;

    // Final vtable (CMenuFade)
    u32 vtFinal = reinterpret_cast<u32>(lbl_eu_8052C540);
    reinterpret_cast<u32*>(self)[0x10 / 4] = vtFinal;

    // IWorkEvent vtable at 0x58 (offset +0x24 within CMenuFade vtable)
    mIWorkEventVtbl = vtFinal + 0x24;

    // IScnRender vtable at 0x5c (offset +0xAC within CMenuFade vtable)
    reinterpret_cast<u32*>(self)[0x5C / 4] = vtFinal + 0xAC;

    // Store constructor parameters
    mScn = pScn;

    // Construct mLayoutMem in-place (at this+0x64)
    __ct__17UnkClass_8045F564Fv(&mLayoutMem);

    // Initialize remaining fields
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
    if (CTaskGame::getInstance()->func_800426F0()) {
        return;
    }
    // __builtin_expect(..., 0) tells MWCC the render path is unlikely,
    // which pushes it out-of-line (beq + b pattern instead of bne).
    if (__builtin_expect((lbl_eu_80663E28 & 0x00200000) == 0, 0)) {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        nw4r::lyt::DrawInfo drawInfo;
        func_80137250(&drawInfo);
        func_80137038(mLayout, &drawInfo, 0, 1);
    }
}

CMenuFade* func_80113C84(CProcess* parent, CScn* pScn, int p5, int p6, float f1, float f2, float f3) {
    extern char lbl_eu_804FDEA8[];

    CMenuFade* fade = lbl_eu_80663FA0;
    if (fade != nullptr) {
        // Already exists — update the existing instance
        u16 frameSize = fade->field_0x78->GetFrameSize();
        f32 frameSizeF = (f32)frameSize;
        fade->field_0x88 = (f3 >= lbl_eu_80667058) ? (frameSizeF / f3) : lbl_eu_8066705C;
        fade->field_0x8C = 2;
        fade->field_0x94 = 1;
        fade->field_0x98 = p6;
        switch (p6) {
        case 0:
            func_80137B44(fade->mLayout, lbl_eu_804FDEA8 + 0x26, 0xFF);
            break;
        case 1:
            func_80137B44(fade->mLayout, lbl_eu_804FDEA8 + 0x26, -1);
            break;
        }
        return nullptr;
    }

    // Allocate and construct a new CMenuFade
    void* mem = mtl::MemManager::allocate(sizeof(CMenuFade), CWorkThreadSystem::getWorkMem());
    if (mem != nullptr) {
        fade = new (mem) CMenuFade(pScn, p5, p6, f1, f2, f3);
    } else {
        fade = nullptr;
    }
    lbl_eu_80663FA0 = fade;
    fade->Regist(parent, false);
    return lbl_eu_80663FA0;
}
