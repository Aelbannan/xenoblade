#define IWORK_EVENT_INLINE_DTOR
// kyoshin/menu/CMenuZeal - Zeal (collective) menu screen controller.

#include "kyoshin/menu/CMenuZeal.hpp"

#include "kyoshin/cf/CfGameManager.hpp"
// (pluginUi.hpp not included: its extern "C" BdatTouchStringCell declaration
// clashes with code_80135FDC.hpp's; only bindIndexedEffect was needed.)
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/CTaskGameEffApi.hpp"
#include "kyoshin/code_80135FDC.hpp"

#include <revolution/GX.h>

// Retail data label: cbRenderBefore gate flag (bit 21 skips the draw).
extern u32 lbl_eu_80663E28;
// Zeal menu string table (layout/anim names, texture names, callbacks).
extern char lbl_eu_80503454[];

// C-ABI imports (retail unmangled names; these identifiers must keep C linkage
// so MWCC emits the literal reloc names -- see CSysWinSave.hpp).
extern "C" {
// Shared ARC resource accessor getter (retail unmangled name).
nw4r::lyt::ArcResourceAccessor* CUICfManager_getArcResourceAccessor();
// Classic-controller detection; the retail callee ignores the arg, but the
// call site still materialises `li r3,-1` (the CfGameManager.hpp inline
// wrapper drops it when inlined, so bind the pre-mangled name directly).
int isClassicController__Q22cf13CfGameManagerFv(int arg);
}
// UI sound effect (mangles to playUISound__FUl).
void playUISound(u32 op);

// ---------------------------------------------------------------------------
// Ctor: retail keeps this as a separate out-of-line function (create() calls
// it), so it is defined here rather than inlined into the class body. The
// IUICf base (CTTask, null PMF slots, fields 0x54/0x55) and the vtables are
// set up by the compiler; the members initialise in declaration order.
// ---------------------------------------------------------------------------
CMenuZeal::CMenuZeal(CScn* scene, int a3, float f1)
    : mScn(scene), mLayout1(nullptr), mAnim1(nullptr), mAnim2(nullptr),
      mAnim3(nullptr), mAnim4(nullptr), mLayout2(nullptr), mAnim5(nullptr),
      mAnim6(nullptr), mAnim7(nullptr), mAnim8(nullptr), mLayout3(nullptr),
      mAnim9(nullptr), mField_94(100.0f), mField_98(0.0f), mField_9C(0.0f),
      mField_A0(0), mField_A1(0), mField_B8(a3), mField_BC(1), mField_BD(1),
      mField_BE(0), mField_BF(0), mField_C0(0.0f), mField_C4(1) {
    mField_B4 = 5.0f * f1;
}

// ---------------------------------------------------------------------------
// Dtor: the UnkClass_8045F564 scratch member and the IUICf/IWorkEvent/IScnRender
// bases are destroyed by the compiler.
// ---------------------------------------------------------------------------
CMenuZeal::~CMenuZeal() {}

void CMenuZeal::Init() {}

// ---------------------------------------------------------------------------
// Term: tear down the render callback, the three layouts, and the scratch
// region, then clear the singleton unless the 0xBF flag is still set.
// ---------------------------------------------------------------------------
void CMenuZeal::Term() {
    CDeviceVI::waitForDrawDone();

    mScn->removeRenderCB(this);

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

    mLayoutMem.deleteRegion();
    if (mField_BF == 0) {
        sInstance = 0;
    }
}

void CMenuZeal::Move() {}

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
    if (!IsMenuState621F0()) goto exit;
    if (mField_BD == 0) goto exit;

    // Inner block: the DrawInfo's scope-exit dtor is auto-emitted as the
    // retail direct `bl __dt__Q34nw4r3lyt8DrawInfoFv(drawInfo, -1)`; an
    // explicit `drawInfo.~DrawInfo()` would make MWCC ALSO emit a
    // virtual-dispatched scope-exit dtor, so none is written here.
    {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        nw4r::lyt::DrawInfo drawInfo;
        SetWideLocAdjust(&drawInfo);
        drawLayout(mLayout1, &drawInfo, 0, 1);
        drawLayout(mLayout2, &drawInfo, 0, 1);
        if (mField_A0 != 0) {
            drawLayout(mLayout3, &drawInfo, 0, 1);
        }
    }
}

// ---------------------------------------------------------------------------
// create: allocate and register the Zeal singleton.
// If a singleton already exists and none of its gate flags (0xBE/0x54/0xA1)
// are set, returns null; otherwise marks the existing singleton's 0xBF flag
// and (re)builds a fresh instance in work memory, stores the singleton, and
// registers it under `parent`.
// ---------------------------------------------------------------------------
CMenuZeal* CMenuZeal::sInstance;

CMenuZeal* CMenuZeal::create(CProcess* parent, CScn* scene, int a3, float f1) {
    CMenuZeal* existing = sInstance;
    if (existing != 0) {
        // Bitwise-OR the three gate flags into a local so MWCC keeps the
        // retail `lbz/or/or.` shape instead of short-circuiting to branches.
        u8 flags = existing->mField_BE | existing->mField54 | existing->mField_A1;
        if (flags == 0) {
            return 0;
        }
        existing->mField_BF = 1;
    }

    CMenuZeal* menu = new (CWorkThreadSystem::getWorkMem()) CMenuZeal(scene, a3, f1);
    sInstance = menu;
    menu->Regist(parent, false);
    return sInstance;
}

extern "C" CMenuZeal* GetZealObjPtr() { return CMenuZeal::sInstance; }

// ---------------------------------------------------------------------------
// GetZealResult: advance the Zeal menu state when the collective-gauge intro
// has played. Sets the 0xA0 draw flag, binds the second layout's anim colours
// (two 0xff s16 quads), then when the 0x94 timer passes the threshold binds
// the layout-1 texture (name depends on classic-controller detection) and
// picks the menu state based on the 0xC4 stage: stage 3 moves to stage 6
// with result 1/2 (2 when 0x98 is inside the b4-scaled window), otherwise
// the fallback path plays sound 0xaa and returns 0.
// ---------------------------------------------------------------------------
extern "C" int GetZealResult(CMenuZeal* self) {
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
    PaneMatSetTevColorsByName(self->mLayout2, &lbl_eu_80503454[0x25e], quadP, quadQ);

    if (self->mField_94 >= 100.0f) {
        // Texture name depends on the detected pad type.
        const char* texName = isClassicController__Q22cf13CfGameManagerFv(-1)
                                  ? &lbl_eu_80503454[0x1c7]
                                  : &lbl_eu_80503454[0x1e2];
        void* tex = CUICfManager_getArcResourceAccessor()->GetResource(0x74696D67, texName, 0);
        if (tex != 0) {
            PaneSetTexPaletteByName(self->mLayout1, &lbl_eu_80503454[0x1fd], tex);
        }

        self->mField_94 = 0.0f;
        if (self->mField_C4 == 3) {
            // Stage-3 timing: result 2 while 0x98 sits in
            // [100.0, 1.5 * 0xB4 + 100.0], else 1. The sound id is
            // Named float temps keep MWCC's retail FPR allocation.
            f32 minGauge = 100.0f;
            f32 gauge = self->mField_98;
            int result;
            u32 sound;
            if (minGauge <= gauge &&
                gauge <=
                    self->mField_B4 * 1.5f + minGauge) {
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

// ---------------------------------------------------------------------------
// AddZealEffect: position the collective menu relative to the player.
// Gets the player object; if present, stores the sdata2 constant into
// mField_9C and routes the effect lookup through the 0xc1 (or 0xc2 when
// arg2 is zero) effect table entry.
// ---------------------------------------------------------------------------
extern "C" void AddZealEffect(CMenuZeal* self, int arg2) {
    cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
    if (player != 0) {
        self->mField_9C = 34.0f;
        if (arg2 != 0) {
            bindIndexedEffect(0xc1, player);
        } else {
            bindIndexedEffect(0xc2, player);
        }
    }
}
