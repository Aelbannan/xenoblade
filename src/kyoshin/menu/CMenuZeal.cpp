// kyoshin/menu/CMenuZeal - Zeal (collective) menu screen controller.

#include "kyoshin/menu/CMenuZeal.hpp"

#include "kyoshin/cf/CfGameManager.hpp"
// (pluginUi.hpp not included: its extern "C" func_80136190 declaration
// clashes with code_80135FDC.hpp's; only func_800451D8 was needed and it is
// declared in CMenuZeal.hpp.)
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "kyoshin/code_80135FDC.hpp"

#include <revolution/GX.h>

// Global-view declarations so the catalog thunks can address the member
// lifecycle functions by their retail symbol names (the definitions below
// emit those symbols).
void __dt__9CMenuZealFv(void* self, int flags);
void cbRenderBefore__9CMenuZealFv(void* self);

// ---------------------------------------------------------------------------
// Allocating factory constructor (retail keeps the unmangled global name).
// Runs CProcess's ctor, installs the temp then composite vtables, copies the
// null PMF triple into the Move/Draw callback slots, then constructs the
// UnkClass_8045F564 scratch region, zeroes the layout/anim pointers, and
// initialises the tail fields (menu state 1, position timers).
// ---------------------------------------------------------------------------
extern "C" __declspec(noinline) CMenuZeal* __ct__CMenuZeal(
    CMenuZeal* obj, void* a2, int a3, float f1) {
    __ct__8CProcessFv(reinterpret_cast<CProcess*>(obj));

    // vtable fixups: temp (CProcess) vtable first, then the composite vtable
    // and the IWorkEvent (+0x24) / IScnRender (+0xac) sub-vtables. The pmf
    // copy reads the null member-function-pointer triple through one base
    // register; post-increment derefs of a const local pointer fold the first
    // access into `lwzu` (btm_sco_init shape). Stores are [1],[0],[2] per
    // group (retail order 0x40,0x3C,0x44 / 0x4C,0x48,0x50).
    obj->mProcessVt = (u32)lbl_eu_8052BF70;
    const u32* src = __ptmf_null;
    u32 w0 = *src++;
    u32 w1 = *src++;
    obj->ptmfMove[1] = w1;
    obj->ptmfMove[0] = w0;
    u32 w2 = *src++;
    obj->ptmfMove[2] = w2;
    src = __ptmf_null;
    w1 = *src++;
    w0 = *src++;
    obj->ptmfDraw[1] = w0;
    obj->ptmfDraw[0] = w1;
    w2 = *src++;
    obj->ptmfDraw[2] = w2;
    obj->mField_54 = 0;
    obj->mField_55 = 0;

    obj->mProcessVt = (u32)lbl_eu_80531BE8;
    obj->mIWorkEventVt = (u32)lbl_eu_80531BE8 + 0x24;
    obj->mIScnRenderVt = (u32)lbl_eu_80531BE8 + 0xac;
    obj->mScn = (CScn*)a2;

    // Zero the layout/anim pointer region (0x64..0x90), then the scalar tail.
    obj->mLayout1 = 0;
    obj->mAnim1 = 0;
    obj->mAnim2 = 0;
    obj->mAnim3 = 0;
    obj->mAnim4 = 0;
    obj->mLayout2 = 0;
    obj->mAnim5 = 0;
    obj->mAnim6 = 0;
    obj->mAnim7 = 0;
    obj->mAnim8 = 0;
    obj->mLayout3 = 0;
    obj->mAnim9 = 0;
    obj->mField_94 = lbl_eu_80667870;
    obj->mField_98 = lbl_eu_80667874;
    obj->mField_9C = lbl_eu_80667874;
    obj->mField_A0 = 0;
    obj->mField_A1 = 0;

    __ct__17UnkClass_8045F564Fv(&obj->mLayoutMem[0]);

    obj->mField_B8 = (u32)a3;
    obj->mField_BC = 1;
    obj->mField_BD = 1;
    obj->mField_BE = 0;
    obj->mField_BF = 0;
    obj->mField_C0 = lbl_eu_80667874;
    obj->mField_C4 = 1;
    obj->mField_B4 = lbl_eu_80667878 * f1;
    return obj;
}

CMenuZeal::~CMenuZeal() {
    // Scratch region + CProcess base destruction are driven by hand (retail
    // shape: member dtor with r4=-1, then the base dtor guarded by the nested
    // double null-check, an MWCC D2-inlined-into-D1 artifact). The conditional
    // operator delete is auto-emitted from the dtor flags.
    reinterpret_cast<UnkClass_8045F564*>(&mLayoutMem)->~UnkClass_8045F564();
    if (this != 0) {
        if (this != 0) {
            __dt__8CProcessFv(reinterpret_cast<CProcess*>(this), 0);
        }
    }
}

void CMenuZeal::Init() {}

void CMenuZeal::Move() {}

// ---------------------------------------------------------------------------
// func_8017FEF0: position the collective menu relative to the player.
// Gets the player object; if present, stores the sdata2 constant into
// mField_9C and routes the effect lookup through the 0xc1 (or 0xc2 when
// arg2 is zero) effect table entry.
// ---------------------------------------------------------------------------
void func_8017FEF0(CMenuZeal* self, int arg2) {
    cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
    if (player != 0) {
        self->mField_9C = lbl_eu_80667890;
        if (arg2 != 0) {
            func_800451D8(0xc1, player);
        } else {
            func_800451D8(0xc2, player);
        }
    }
}

// ---------------------------------------------------------------------------
// func_8017FC88: allocate and register the Zeal singleton.
// If a singleton already exists and none of its gate flags (0xBE/0x54/0xA1)
// are set, returns null; otherwise marks the existing singleton's 0xBF flag
// and (re)builds a fresh instance: allocate 0xc8 bytes from work memory,
// construct it, store the singleton, and register it under `parent`.
// ---------------------------------------------------------------------------
CMenuZeal* func_8017FC88(CProcess* parent, void* a2, int a3, float f1) {
    CMenuZeal* existing = lbl_eu_806642A0;
    if (existing != 0) {
        // Bitwise-OR the three gate flags into a local so MWCC keeps the
        // retail `lbz/or/or.` shape instead of short-circuiting to branches.
        u8 flags = existing->mField_BE | existing->mField_54 | existing->mField_A1;
        if (flags == 0) {
            return 0;
        }
        existing->mField_BF = 1;
    }

    CMenuZeal* obj = (CMenuZeal*)mtl::MemManager::allocate(
        0xc8, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        // The retail ctor returns `this` in r3, so threading the return value
        // back into obj keeps it live in r3 across the call (no spill).
        obj = __ct__CMenuZeal(obj, a2, a3, f1);
    }

    lbl_eu_806642A0 = obj;
    Regist__8CProcessFP8CProcessb(reinterpret_cast<CProcess*>(obj), parent,
                                  false);
    return lbl_eu_806642A0;
}

// ---------------------------------------------------------------------------
// Term: tear down the render callback, the three layouts, and the scratch
// region, then clear the singleton unless the 0xBF flag is still set.
// ---------------------------------------------------------------------------
void CMenuZeal::Term() {
    CDeviceVI::waitForDrawDone();

    // IScnRender subobject lives at +0x5C; the null-checked upcast keeps the
    // retail `mr r4,this; beq; addi r4,this,0x5c` shape.
    IScnRender* renderCB = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        renderCB = reinterpret_cast<IScnRender*>(&mIScnRenderVt);
    }
    mScn->removeRenderCB(renderCB);

    if (mLayout1 != 0) {
        delete mLayout1;
        mLayout1 = 0;
    }
    if (mLayout2 != 0) {
        delete mLayout2;
        mLayout2 = 0;
    }
    if (mLayout3 != 0) {
        delete mLayout3;
        mLayout3 = 0;
    }

    reinterpret_cast<UnkClass_8045F564*>(&mLayoutMem)->func_8045F778();
    if (mField_BF == 0) {
        lbl_eu_806642A0 = 0;
    }
}

// ---------------------------------------------------------------------------
// cbRenderBefore: gate prefix (task busy / global bit 21 / scene active /
// draw-enabled flag), then draw the layouts through a stack DrawInfo. The
// third layout only draws when mField_A0 is set.
// ---------------------------------------------------------------------------
void CMenuZeal::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() != 0) goto exit;
    if (lbl_eu_80663E28 & (1u << 21)) goto exit;
    // Branch-over-branch guard: `goto body` with the `exit` label + return
    // placed BEFORE `body` keeps MWCC from folding the bit test to a single
    // `bne` -- it emits retail's `beq body; b exit`.
    goto body;
exit:
    return;
body:
    if (!func_8013BE50()) goto exit;
    if (mField_BD == 0) goto exit;

    // Inner block: the DrawInfo's scope-exit dtor is auto-emitted as the
    // retail direct `bl __dt__Q34nw4r3lyt8DrawInfoFv(drawInfo, -1)`; an
    // explicit `drawInfo.~DrawInfo()` would make MWCC ALSO emit a
    // virtual-dispatched scope-exit dtor, so none is written here.
    {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        nw4r::lyt::DrawInfo drawInfo;
        func_80137250(&drawInfo);
        drawLayout(mLayout1, &drawInfo, 0, 1);
        drawLayout(mLayout2, &drawInfo, 0, 1);
        if (mField_A0 != 0) {
            drawLayout(mLayout3, &drawInfo, 0, 1);
        }
    }
}

// ---------------------------------------------------------------------------
// VTable-compatible thunks adjacent to the CMenuZeal subobjects (retail emits
// these as offset-adjusted dispatch stubs: only r3 is re-based, then a tail
// jump into the right member). Preserved from the scaffold.
// ---------------------------------------------------------------------------
void func_8017FF60(void* self) { ((void(*)(void*))__dt__9CMenuZealFv)((char*)self - 0x58); }

void func_8017FF68(void* self) { ((void(*)(void*))cbRenderBefore__9CMenuZealFv)((char*)self - 0x5c); }

void func_8017FF70(void* self) { ((void(*)(void*))__dt__9CMenuZealFv)((char*)self - 0x5c); }

u32 func_8017FD44() { return (u32)lbl_eu_806642A0; }

// ---------------------------------------------------------------------------
// func_8017FD4C: advance the Zeal menu state when the collective-gauge intro
// has played. Sets the 0xA0 draw flag, binds the second layout's anim colours
// (two 0xff s16 quads), then when the 0x94 timer passes the threshold binds
// the layout-1 texture (name depends on classic-controller detection) and
// picks the menu state based on the 0xC4 stage: stage 3 moves to stage 6
// with result 1/2 (2 when 0x98 is inside the b4-scaled window), otherwise
// the fallback path plays sound 0xaa and returns 0.
// ---------------------------------------------------------------------------
int func_8017FD4C(CMenuZeal* self) {
    if (self->mField_A1 != 0) {
        return 0;
    }
    self->mField_A0 = 1;

    // Bind the second layout's animation colours: src1 = {ff,ff,ff,0} and
    // src2 = {ff,ff,ff,ff} s16 quads, filled descending (retail store order
    // 0xe..0x8 then 0x16..0x10). Individual stores -- MWCC pools a
    // brace-initialised local array into sdata2 (lwz/stw copies) where retail
    // emits per-element sth stores.
    s16 quadP[4];  // {ff,ff,ff,0}  (src1, passed third)
    s16 quadQ[4];  // {ff,ff,ff,ff} (src2, passed fourth)
    quadQ[3] = 0xff;
    quadQ[2] = 0xff;
    quadQ[1] = 0xff;
    quadQ[0] = 0xff;
    quadP[3] = 0;
    quadP[2] = 0xff;
    quadP[1] = 0xff;
    quadP[0] = 0xff;
    func_80139A18(self->mLayout2, &lbl_eu_80503454[0x25e], quadP, quadQ);

    if (self->mField_94 >= lbl_eu_80667870) {
        // Texture name depends on the detected pad type.
        const char* texName = isClassicController__Q22cf13CfGameManagerFv(-1)
                                  ? &lbl_eu_80503454[0x1c7]
                                  : &lbl_eu_80503454[0x1e2];
        void* tex = func_801355F4()->GetResource(0x74696D67, texName, 0);
        if (tex != 0) {
            func_80137E7C(self->mLayout1, &lbl_eu_80503454[0x1fd], tex);
        }

        self->mField_94 = lbl_eu_80667874;
        if (self->mField_C4 == 3) {
            // Stage-3 timing: result 2 while 0x98 sits in
            // [80667870, 8066788C * 0xB4 + 80667870], else 1. The sound id is
            // Named float temps keep MWCC's retail FPR allocation.
            f32 minGauge = lbl_eu_80667870;
            f32 gauge = self->mField_98;
            int result;
            u32 sound;
            if (minGauge <= gauge &&
                gauge <=
                    self->mField_B4 * lbl_eu_8066788C + minGauge) {
                result = 2;
                sound = 0xa9;
            } else {
                result = 1;
                sound = 0xa9;
            }
            playUISound(sound);
            self->mField_A1 = 1;
            self->mField_BC = 0;
            self->mField_C4 = 6;
            return result;
        }
    }

    playUISound(0xaa);
    self->mField_A1 = 1;
    self->mField_BC = 0;
    self->mField_C4 = 6;
    return 0;
}
