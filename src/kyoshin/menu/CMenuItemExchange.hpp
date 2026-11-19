#pragma once

#include "kyoshin/CItemBoxLine.hpp"

#include <types.h>

#include <monolib/util/FixStr.hpp>

#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CItemBoxGrid.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

class CScn;
class IScnRender;

/**
 * OC (object-component) interface embedded at offset 0x6c within
 * CMenuItemExchange. Used for virtual dispatch through the OC vtable;
 * thunks adjust back to the containing object before delegating.
 */
struct CMenuItemExchangeOC {
    virtual ~CMenuItemExchangeOC() {}
};

// IUIWindow region (0x6C): CProcess base (0x00-0x3B) + vtable + Move/Draw
// Window bases: the canonical IUIWindow (kyoshin/IUIWindow.hpp) plus the
// monolib IScnRender interface at +0x6C. Both sub-vtables are compiler-emitted
// here (this TU owns the key function ~CMenuItemExchange).
#include "kyoshin/IUIWindow.hpp"
#include "monolib/scn/IScnRender.hpp"

/**
 * Menu item exchange / equipment screen controller.
 *
 *   0x00 IUIWindow base (0x6C)
 *   0x6C IScnRender sub-object vptr (composite vtable + 0x24)
 *   0x70 CScn* mScene (owning scene, addRenderCB target in Init)
 *   0x74 u32 mField74 (u16 arg passed to the item-box ctors)
 *   0x78 CBgTex, 0x98 CTitleAHelp, 0xD0 CItemBoxLine, 0x70C CItemBoxGrid
 *   0x5118 u8 phase, 0x511C/0x5120 timers
 */
class CMenuItemExchange : public IUIWindow, public IScnRender {
public:
    CMenuItemExchange(CScn* scene, u32 arg);
    virtual ~CMenuItemExchange();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();

    void Phase0Load();
    void GateFrom1();
    void Phase2Idle();
    void GateFrom3();
    void Phase4Fade();
    void GateFrom5();
    void Phase6Input();
    void Phase7Entry();
    void GateFrom8();

    CScn* mScene;                    // 0x70 owning scene
    u32 mField74;                    // 0x74 (u16 arg passed to item-box ctors)
    CBgTex mBgTex;                   // 0x78
    CTitleAHelp mTitleAHelp;         // 0x98
    CItemBoxLine mItemBoxLine;       // 0xD0
    CItemBoxGrid mItemBoxGrid;       // 0x70C
    u8  field_5118;                  // 0x5118 phase
    f32 mFloat511C;                  // 0x511C (input-handler timer)
    f32 mFloat5120;                  // 0x5120 (state-4 fade float)
};

#include "monolib/scn/CScn.hpp"

// cf::CfPadData view: only the flag words the menu input handlers read
// (+0x00 held, +0x04 pressed, +0x104 turbo/short-press).
struct CMIExPadData {
    u32 mHeldFlags;                  // +0x00
    u32 mPressedFlags;               // +0x04
    u8 _pad08[0x104 - 0x08];         // 0x08..0x103
    u32 mTurboFlags;                 // +0x104
};

// Retail-unmangled callee names (US strips the member manglings for the
// func_ helpers; isIdle keeps its C++ mangling). int returns keep the caller's
// `!= 0` as a plain cmpwi (no u8 mask) like retail. These names ARE the
// retail linker symbols - do not rename or "fix" the mangled forms.

// Widget ctors used by this TU (the widget classes' ctors are still C-ABI).
extern "C" void __ct__6CBgTexFUc(CBgTex* self, u8 arg);
extern "C" void __ct__11CTitleAHelpFPcUc(CTitleAHelp* self, char* name, u8 arg);
extern "C" void __ct__12CItemBoxGridFUcUsUlUc(CItemBoxGrid* self, u32 a, u32 b, u32 c, u32 d);
extern "C" bool BgTex_Acquire_3C14(CBgTex* self);
// CTitleAHelp::CTitleAHelp_load() is declared in kyoshin/CTitleAHelp.hpp.
extern "C" void markReplayClose(CTitleAHelp* self);
extern "C" void beginClose(CTitleAHelp* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 mode);

extern "C" void func_801C412C(CTitleAHelp* self);      // hide prompt
extern "C" void reopenFromClose(CTitleAHelp* self);      // stop help-bar animation
// Load/anim hooks used by the phase gates (retail unmangled names).
extern "C" int BgTex_IsLoaded_3E34(CBgTex* self);            // bg layout load-done query

// Factory support (retail unmangled symbols; sibling-menu-TU scheme).
class CProcess;
extern "C" void* getWorkMem__17CWorkThreadSystemFv();
extern "C" void Regist__8CProcessFP8CProcessb(void* _this, void* parent, bool insertTop);
extern "C" void drawHelp(CTitleAHelp* self, nw4r::lyt::DrawInfo* di);

// cbRenderBefore gates / draw pipeline (retail verbatim names).
namespace nw4r { namespace lyt { class DrawInfo; } }
extern "C" void BgTex_Draw_3D7C(CBgTex* self, nw4r::lyt::DrawInfo* di);
extern u32 lbl_eu_80663E28;   // cbRenderBefore mode-flag word (.sbss)

// Input-handler callees (retail unmangled names). isClassicController keeps the
// u32 return declared by CItemBoxGrid.hpp (included above) - do not redeclare.
extern "C" CMIExPadData* getCfPadData__Q22cf13CfGameManagerFv();
extern "C" void updateHelp(CTitleAHelp* self);     // help-bar per-frame update
// setPresentationFlag: canonical setPresentationFlag__Q22cf13CfGameManagerFv(bool).
extern "C" int MenuTutorialIsCreated();

// Move()/phase-handler callees (retail unmangled names).
extern "C" void BgTex_Tick_3D54(CBgTex* self);          // bg per-frame update

// Shared data imports (MWCC does not mangle global-scope data names).
extern char lbl_eu_80505324[];   // split1 .rodata string pool (Init title name)
extern u8 lbl_eu_8066442C;       // .sbss (item-box line tab mode latch)
extern const f32 lbl_eu_80667E78; // .sdata2 (fade-out/state float)
extern const f32 lbl_eu_80667E7C; // .sdata2 (timer increment)
extern const f32 lbl_eu_80667E80; // .sdata2 (timer clamp)
