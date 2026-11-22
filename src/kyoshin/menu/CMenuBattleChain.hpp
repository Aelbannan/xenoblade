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
    virtual void cbRenderBefore(CScn* scene);

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

    static CMenuBattleChain* sInstance;
}; // size: 0xA0

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
extern u32 gmPauseFlags;

// Battle-chain pane colour records (4x u16; zero-init .sbss, filled at
// startup by sinit_802AAF24, passed as pairs to PaneMatSetTevColorsByName).
extern GXColorS10 chainIconColorT1A;
extern GXColorS10 chainIconColorT1B;
extern GXColorS10 chainIconColorT2A;
extern GXColorS10 chainIconColorT2B;
extern GXColorS10 chainIconColorT3A;
extern GXColorS10 chainIconColorT3B;
extern GXColorS10 chainIconColorT4A;
extern GXColorS10 chainIconColorT4B;
extern GXColorS10 chainIconColorT5A;
extern GXColorS10 chainIconColorT5B;
extern GXColorS10 chainIconColorT6A;
extern GXColorS10 chainIconColorT6B;
extern GXColorS10 chainIconColorT7A;
extern GXColorS10 chainIconColorT7B;
extern GXColorS10 chainIconColorT8A;
extern GXColorS10 chainIconColorT8B;
extern GXColorS10 chainIconGlossColorT1A;
extern GXColorS10 chainIconGlossColorT1B;
extern GXColorS10 chainIconGlossColorT2A;
extern GXColorS10 chainIconGlossColorT2B;
extern GXColorS10 chainIconGlossColorT3A;
extern GXColorS10 chainIconGlossColorT3B;
extern GXColorS10 chainIconGlossColorT4A;
extern GXColorS10 chainIconGlossColorT4B;
extern GXColorS10 chainIconGlossColorT5A;
extern GXColorS10 chainIconGlossColorT5B;
extern GXColorS10 chainIconGlossColorT6A;
extern GXColorS10 chainIconGlossColorT6B;
extern GXColorS10 chainIconGlossColorT7A;
extern GXColorS10 chainIconGlossColorT7B;
extern GXColorS10 chainIconGlossColorT8A;
extern GXColorS10 chainIconGlossColorT8B;
extern GXColorS10 chainBarColorT1A;
extern GXColorS10 chainBarColorT1B;
extern GXColorS10 chainBarColorT2A;
extern GXColorS10 chainBarColorT2B;
extern GXColorS10 chainBarColorT3A;
extern GXColorS10 chainBarColorT3B;
extern GXColorS10 chainBarColorT4A;
extern GXColorS10 chainBarColorT4B;
extern GXColorS10 chainBarColorT5A;
extern GXColorS10 chainBarColorT5B;
extern GXColorS10 chainBarColorT6A;
extern GXColorS10 chainBarColorT6B;
extern GXColorS10 chainBarColorT7A;
extern GXColorS10 chainBarColorT7B;
extern GXColorS10 chainBarColorT8A;
extern GXColorS10 chainBarColorT8B;

// Chain-type colour/position constants read by applyChainColorScheme (.sdata).
extern u32 chainVtxColorT1;
extern u32 chainVtxColorT2;
extern u32 chainVtxColorT3;
extern u32 chainVtxColorT4;
extern u32 chainVtxColorT5;
extern u32 chainVtxColorT6;
extern u32 chainVtxColorT7;
extern u32 chainVtxColorT8;
extern u32 chainTextWhiteT1;
extern u32 chainTextColorT1;
extern u32 chainTextWhiteT2;
extern u32 chainTextColorT2;
extern u32 chainTextWhiteT3;
extern u32 chainTextColorT3;
extern u32 chainTextWhiteT4;
extern u32 chainTextColorT4;
extern u32 chainTextWhiteT5;
extern u32 chainTextColorT5;
extern u32 chainTextWhiteT6;
extern u32 chainTextColorT6;
extern u32 chainTextWhiteT7;
extern u32 chainTextColorT7;
extern u32 chainTextWhiteT8;
extern u32 chainTextColorT8;

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
