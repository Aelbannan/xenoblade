#pragma once

#include <types.h>

#include "monolib/work/CProcess.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "kyoshin/IUIWindow.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

namespace cf {
class CfObjectMove;
}

namespace nw4r {
namespace lyt {
class Layout;
class AnimTransform;
class Pane;
class ArcResourceAccessor;
class DrawInfo;
}
}

class CScn;

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

// Battle-chain menu state shared by the trackChainPlayer..requestAnimReset helpers:
// the current player reference plus two pending toggle flags consumed when the
// arts-select menu becomes interactable.
struct CBattleChainMenuState {
    cf::CfObjectMove* mPlayer; // +0x00 current player (cf::CfGameManager::getPlayer(0))
    u8 mFlag4;                 // +0x04 pending flag
    u8 mFlag5;                 // +0x05 pending flag
};

/*
 * Battle-chain menu controller (size 0xA0).
 *
 * Real MI class, from the retail composite vtable __vt__16CMenuBattleChain
 * (0x80539E48): IUIWindow at +0x00, IWorkEvent at +0x6C and IScnRender at
 * +0x70; the +0x24 / +0xAC sub-vtables, the 0x40 base list and the
 * +0x6C/+0x70 adjusting thunks are compiler output.
 *
 *   0x00  IUIWindow base (CProcess header, Move/Draw pmf hooks at +0x3C/+0x48,
 *         layout/state fields at +0x54..+0x68)
 *   0x6C  IWorkEvent base
 *   0x70  IScnRender base (render callback registered with the scene)
 *   0x74  mScn     - owning scene (addRenderCB target)
 *   0x78  mRegion  - embedded layout-build scratch region (0x10)
 *   0x88  mAnim0..mAnim4 - the five layout animations
 *   0x9C  mField9C - state/mode byte
 *   0x9D  mField9D - chain-type byte (ctor arg)
 *   0x9E  mField9E - frame counter
 */
class CMenuBattleChain : public IUIWindow, public IWorkEvent, public IScnRender {
public:
    CMenuBattleChain(CScn* parent, u8 arg);
    virtual ~CMenuBattleChain();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();

    CScn* mScn;                       // 0x74 owning scene
    UnkClass_8045F564 mRegion;        // 0x78 scratch memory region
    nw4r::lyt::AnimTransform* mAnim0; // 0x88
    nw4r::lyt::AnimTransform* mAnim1; // 0x8C
    nw4r::lyt::AnimTransform* mAnim2; // 0x90
    nw4r::lyt::AnimTransform* mAnim3; // 0x94
    nw4r::lyt::AnimTransform* mAnim4; // 0x98
    u8 mField9C;                      // 0x9C state/mode byte
    u8 mField9D;                      // 0x9D chain-type byte (ctor arg)
    u8 mField9E;                      // 0x9E frame counter
    u8 _pad9F;                        // 0x9F
    void switchChainType(u8 arg);
}; // size: 0xA0

// Battle-chain singleton (retail SDA symbol, .sbss).
extern CMenuBattleChain* lbl_eu_80664A60;

// Battle-chain resource-name strings: the layout region name and the
// resource-name table (region at +0x00, layout at +0x11, anims at
// +0x2a/+0x46/+0x64/+0x87/+0xaa, panes at +0xdc..0x133).
extern const char lbl_eu_80510878[];
extern const char lbl_eu_8051088C[];
// Anim-frame reset constant (sdata2 float pool).
extern const f32 lbl_eu_80668CB0;
// Anim "finished" threshold passed to advanceAnimTransform / AnimRewindFrame.
extern const f32 lbl_eu_80668CB4;

// cbRenderBefore flag (.sbss); bit 0x200000 gates CMenuBattleChain::Move.
extern u32 lbl_eu_80663E28;

// Battle-chain pane colour records (4x u16; zero-init .sbss, filled at
// startup by sinit_802AAF24, passed as pairs to PaneMatSetTevColorsByName).
extern u16 lbl_eu_80664A68[4];
extern u16 lbl_eu_80664A70[4];
extern u16 lbl_eu_80664A78[4];
extern u16 lbl_eu_80664A80[4];
extern u16 lbl_eu_80664A88[4];
extern u16 lbl_eu_80664A90[4];
extern u16 lbl_eu_80664A98[4];
extern u16 lbl_eu_80664AA0[4];
extern u16 lbl_eu_80664AA8[4];
extern u16 lbl_eu_80664AB0[4];
extern u16 lbl_eu_80664AB8[4];
extern u16 lbl_eu_80664AC0[4];
extern u16 lbl_eu_80664AC8[4];
extern u16 lbl_eu_80664AD0[4];
extern u16 lbl_eu_80664AD8[4];
extern u16 lbl_eu_80664AE0[4];
extern u16 lbl_eu_80664AE8[4];
extern u16 lbl_eu_80664AF0[4];
extern u16 lbl_eu_80664AF8[4];
extern u16 lbl_eu_80664B00[4];
extern u16 lbl_eu_80664B08[4];
extern u16 lbl_eu_80664B10[4];
extern u16 lbl_eu_80664B18[4];
extern u16 lbl_eu_80664B20[4];
extern u16 lbl_eu_80664B28[4];
extern u16 lbl_eu_80664B30[4];
extern u16 lbl_eu_80664B38[4];
extern u16 lbl_eu_80664B40[4];
extern u16 lbl_eu_80664B48[4];
extern u16 lbl_eu_80664B50[4];
extern u16 lbl_eu_80664B58[4];
extern u16 lbl_eu_80664B60[4];
extern u16 lbl_eu_80664B68[4];
extern u16 lbl_eu_80664B70[4];
extern u16 lbl_eu_80664B78[4];
extern u16 lbl_eu_80664B80[4];
extern u16 lbl_eu_80664B88[4];
extern u16 lbl_eu_80664B90[4];
extern u16 lbl_eu_80664B98[4];
extern u16 lbl_eu_80664BA0[4];
extern u16 lbl_eu_80664BA8[4];
extern u16 lbl_eu_80664BB0[4];
extern u16 lbl_eu_80664BB8[4];
extern u16 lbl_eu_80664BC0[4];
extern u16 lbl_eu_80664BC8[4];
extern u16 lbl_eu_80664BD0[4];
extern u16 lbl_eu_80664BD8[4];
extern u16 lbl_eu_80664BE0[4];

// Chain-type colour/position constants read by applyChainColorScheme (.sdata).
extern u32 lbl_eu_80662D70;
extern u32 lbl_eu_80662D74;
extern u32 lbl_eu_80662D78;
extern u32 lbl_eu_80662D7C;
extern u32 lbl_eu_80662D80;
extern u32 lbl_eu_80662D84;
extern u32 lbl_eu_80662D88;
extern u32 lbl_eu_80662D8C;
extern u32 lbl_eu_80662D90;
extern u32 lbl_eu_80662D94;
extern u32 lbl_eu_80662D98;
extern u32 lbl_eu_80662D9C;
extern u32 lbl_eu_80662DA0;
extern u32 lbl_eu_80662DA4;
extern u32 lbl_eu_80662DA8;
extern u32 lbl_eu_80662DAC;
extern u32 lbl_eu_80662DB0;
extern u32 lbl_eu_80662DB4;
extern u32 lbl_eu_80662DB8;
extern u32 lbl_eu_80662DBC;
extern u32 lbl_eu_80662DC0;
extern u32 lbl_eu_80662DC4;
extern u32 lbl_eu_80662DC8;
extern u32 lbl_eu_80662DCC;

// Font-pane helper (retail mangled name getPackedFont__Fv).

// Result of CMenuArtsSelect_getSelectState: signed state bytes at +0x0/+0x1
// (see cf::chain CArtsSelectStateView).
struct CArtsSelectStateView {
    s8 field_0; // 0x0
    s8 field_1; // 0x1
};

// C-linkage imports (retail unmangled symbols - keep linkage verbatim).
extern "C" {
void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564* self);
void tickChainCounter(CMenuBattleChain* self);
void applyChainColorScheme(CMenuBattleChain* self);
void sinit_802AAF24();

// C-linkage imports from the CMenuArtsSelect unit (retail unmangled names).
// Proper home: kyoshin/menu/CMenuArtsSelect.hpp - move there when that unit
// exports them. Declared here because CMenuArtsSelect.hpp pulls the full
// monolib CScn, which conflicts with the minimal CScn from
// kyoshin/harness_catalog.hpp -> CTaskGameEff.hpp.
void CMenuArtsSelect_setDisabled();
int CMenuArtsSelect_isInteractable();
int CMenuArtsSelect_isNotReady();
void CMenuArtsSelect_CreateArtsRef();
void CMenuArtsSelect_ResetSlotAnims();
CArtsSelectStateView* CMenuArtsSelect_getSelectState();
}
