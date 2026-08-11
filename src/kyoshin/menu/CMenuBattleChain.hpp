#pragma once

#include <types.h>

#include "monolib/lib/UnkClass_8045F564.hpp"

namespace cf {
class CfObjectMove;
}

namespace nw4r {
namespace lyt {
class Layout;
class AnimTransform;
class Pane;
class ArcResourceAccessor;
}
}

class CScn;

// Minimal CTaskGame decl (retail getInstance__9CTaskGameFv /
// func_800426F0__9CTaskGameFv); the full header would pull the monolib CScn,
// conflicting with the minimal CScn from harness_catalog.hpp -> CTaskGameEff.hpp.
class CTaskGame {
public:
    static CTaskGame* getInstance();
    static bool func_800426F0();
};

// Battle-chain menu state shared by the func_802AB3D0..func_802AB5E4 helpers:
// the current player reference plus two pending toggle flags consumed when the
// arts-select menu becomes interactable.
struct CBattleChainMenuState {
    cf::CfObjectMove* mPlayer; // +0x00 current player (cf::CfGameManager::getPlayer(0))
    u8 mFlag4;                 // +0x04 pending flag
    u8 mFlag5;                 // +0x05 pending flag
};

/*
 * Battle-chain menu controller.
 *
 * Deliberately NON-polymorphic (the CMenuGetItem idiom): the CProcess base
 * region is embedded at +0x00 and the composite / IWorkEvent / IScnRender
 * vtables are stored as raw u32 slots, so the retail unmangled ctor
 * `__ct__CMenuBattleChain` can be written as an extern "C" free function that
 * stores the retail vtable addresses directly.
 */
class CMenuBattleChain {
public:
    CMenuBattleChain();
    ~CMenuBattleChain();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    u8 mProcess[0x10];          // 0x00 CProcess storage head
    u32 mProcessVt;             // 0x10 CProcess vtable slot (temp, then composite)
    u8 mProcRest[0x3C - 0x14];  // 0x14..0x3B CProcess tail
    u32 ptmfMove[3];            // 0x3C move-callback PMF (null triple)
    u32 ptmfDraw[3];            // 0x48 draw-callback PMF (null triple)
    nw4r::lyt::Layout* mLayout; // 0x54 main layout
    u32 mField58;               // 0x58
    u32 mField5C;               // 0x5C
    s32 mField60;               // 0x60 current anim index (-1 = none)
    u8 mField64;                // 0x64
    u8 mField65;                // 0x65
    u8 mField66;                // 0x66
    u8 mField67;                // 0x67 ready flag (1 after ctor, cleared by Init)
    u32 mField68;               // 0x68
    u32 mIWorkEventVt;          // 0x6C IWorkEvent subobject vtable slot
    u32 mIScnRenderVt;          // 0x70 IScnRender subobject vtable slot
    CScn* mScn;                 // 0x74 owning scene (addRenderCB target)
    UnkClass_8045F564 mRegion;  // 0x78 scratch memory region (0x10 bytes)
    nw4r::lyt::AnimTransform* mAnim0; // 0x88
    nw4r::lyt::AnimTransform* mAnim1; // 0x8C
    nw4r::lyt::AnimTransform* mAnim2; // 0x90
    nw4r::lyt::AnimTransform* mAnim3; // 0x94
    nw4r::lyt::AnimTransform* mAnim4; // 0x98
    u8 mField9C;                // 0x9C state/mode byte
    u8 mField9D;                // 0x9D chain-type byte (ctor arg)
    u8 mField9E;                // 0x9E frame counter
    u8 _pad9F;                  // 0x9F
};

// Battle-chain singleton (retail SDA symbol, .sbss).
extern CMenuBattleChain* lbl_eu_80664A60;

// Temp (CProcess) vtable written before the PMF copy; composite vtable with
// the IWorkEvent sub-vtable at +0x24 and IScnRender sub-vtable at +0xac.
extern char lbl_eu_8052D238[];
extern char lbl_eu_80539E48[];
// Null pointer-to-member-function constant (3 words, copied into +0x3C..0x53).
extern u32 __ptmf_null[3];

// Battle-chain resource-name table: region name at +0x00, layout name at
// +0x11, anim names at +0x2a/+0x46/+0x64/+0x87/+0xaa, pane names at
// +0xdc..0x133.
extern char lbl_eu_8051088C[];
// Anim-frame reset constant (sdata2 float pool).
extern const f32 lbl_eu_80668CB0;
// Anim "finished" threshold passed to func_80137444 / func_80137510.
extern const f32 lbl_eu_80668CB4;

// cbRenderBefore flag (.sbss); bit 0x200000 gates CMenuBattleChain::Move.
extern u32 lbl_eu_80663E28;

// Battle-chain pane colour records (4x u16; zero-init .sbss, filled at
// startup by sinit_802AAF24, passed as pairs to func_80139A18).
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

// Chain-type colour/position constants read by func_802AA588 (.sdata).
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

// Font-pane helper (retail mangled name func_801355A0__Fv).
u32 func_801355A0();

// C-linkage imports (retail unmangled symbols - keep linkage verbatim).
extern "C" {
nw4r::lyt::ArcResourceAccessor* func_801355F4();
void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564* self);
void func_80137CD4(void*, const char*, int, u32);
void func_80139BF4(void*, const char*, u32, u32);
void func_802AAC78(CMenuBattleChain* self);
void func_802AA588(CMenuBattleChain* self);
void sinit_802AAF24();

// C-linkage imports from the CMenuArtsSelect unit (retail unmangled names).
// Proper home: kyoshin/menu/CMenuArtsSelect.hpp - move there when that unit
// exports them. Declared here because CMenuArtsSelect.hpp pulls the full
// monolib CScn, which conflicts with the minimal CScn from
// kyoshin/harness_catalog.hpp -> CTaskGameEff.hpp.
void CMenuArtsSelect_setDisabled();
int CMenuArtsSelect_isInteractable();
int CMenuArtsSelect_isNotReady();
void func_8010433C();
void func_801043BC();
}
