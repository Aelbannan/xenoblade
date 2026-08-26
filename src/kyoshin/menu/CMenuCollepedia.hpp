#pragma once

#include <types.h>
#include <monolib/scn/IScnRender.hpp>
#include <monolib/work/CProcess.hpp>

#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CCollepedia.hpp"

// Forward declaration for the owning scene (CScn.hpp pulls broken monolib
// umbrella headers; keep the pointer opaque, same scheme as CMenuBattleEnd).
class CScn;

/*
 * Collepedia (encyclopedia) menu screen process.
 *
 * Layout (constructor + Term):
 *   0x00: CProcess        -- task-system base (0x3C) + vtable PMF data
 *   0x54: u8              -- phase/state flag (written by func_8025306C etc.)
 *   0x58: IScnRender      -- render-callback subobject (member, vptr)
 *   0x5C: CScn*           -- owning scene (removeRenderCB target in Term)
 *   0x60: CBgTex          -- background layout widget
 *   0x80: CTitleAHelp     -- title/help bar
 *   0xB8: CCollepedia     -- core collepedia data/logic (0x28FC bytes)
 *  0x29B4: u8 mState      -- phase/state indicator
 *  0x29B8: f32 mTimer     -- animation/open timer
 *  0x29BC: total size     -- sizeof(CMenuCollepedia) = 0x29BC
 */
class CMenuCollepedia : public CProcess {
public:
    // CProcess overrides
    virtual ~CMenuCollepedia();
    virtual void Init();
    virtual void Term();
    virtual void Move();

    // Render callback (dispatched through the IScnRender subobject vtable)
    void cbRenderBefore();

    // IScnRender vtable this-adjusting thunks
    void func_80253188();
    void func_80253190();

    u32 ptmf0[3];                 // 0x3C-0x47: null PMF callback slot group 1
    u32 ptmf1[3];                 // 0x48-0x53: null PMF callback slot group 2
    u8 mField54;                  // 0x54: phase flag
    u8 mField55;                  // 0x55
    u8 _pad56[2];                 // 0x56-0x57
    IScnRender mIScnRender;       // 0x58: render-callback subobject (vptr)
    CScn* mScene;                 // 0x5C: owning scene
    CBgTex mBgTex;                // 0x60
    CTitleAHelp mTitleAHelp;      // 0x80
    CCollepedia mCollepedia;      // 0xB8 (0x28FC bytes -> 0x29B4)
    u8 mState;                    // 0x29B4
    f32 mTimer;                   // 0x29B8
    CMenuCollepedia();
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim).
extern "C" void waitForDrawDone__9CDeviceVIFv();
extern "C" void removeRenderCB__4CScnFP10IScnRender(CScn*, IScnRender*);
extern "C" void addRenderCB__4CScnFP10IScnRenderUlUl(CScn*, IScnRender*, u32, u32);
extern "C" void func_8008294C__Q22cf13CfGameManagerFv(u32 enable);
extern "C" int func_800FEDF8();
extern "C" void func_800FF914();
extern "C" void func_80138078__FUl(u32 op);
extern "C" void func_801C3D9C(CBgTex* self);
extern "C" void func_801C40A0(CTitleAHelp* self);
extern "C" void func_801C414C(CTitleAHelp* self);
// isIdle declared int (not u8) so callers compare with cmpwi directly (no byte mask).
extern "C" int isIdle__11CTitleAHelpFv(CTitleAHelp* h);

// CCollepedia callees (retail-unmangled func_ names).
extern "C" int func_80254D50(CCollepedia* self);
extern "C" void func_80254C04(CCollepedia* self);
extern "C" void func_80254D8C(CCollepedia* self);
extern "C" void func_802552B4(CCollepedia* self);
extern "C" int func_80255698(CCollepedia* self);
extern "C" u32 func_802556DC(CCollepedia* self);

// Widget ctor/dtor and Init helper calls (US retail strips these names).
extern "C" void __ct__8CProcessFv(CProcess* self);
extern "C" void __ct__CBgTex(CBgTex* self, u8 arg);
extern "C" void __ct__CTitleAHelp(CTitleAHelp* self, char* name, u8 arg);
extern "C" void __ct__CCollepedia(CCollepedia* self);
extern "C" void __dt__6CBgTexFv(CBgTex* self, int dealloc);
extern "C" void __dt__11CTitleAHelpFv(CTitleAHelp* self, int dealloc);
extern "C" void __dt__11CCollepediaFv(CCollepedia* self, int dealloc);
extern "C" void __ct__UnkClass_8011C974(void* dst, void* src);
extern "C" void CTitleAHelp_load(CTitleAHelp* self);
extern "C" int func_801C3C14(CBgTex* self);
extern "C" char* func_80136190(const void* a, const void* b, int id);
extern "C" void func_8018B0FC(void* dst, void* src);
extern "C" void func_8025492C(CCollepedia* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 mode);

// func_80252DD8 callees (retail-unmangled func_ names).
extern "C" int func_80255688(CCollepedia* self);
extern "C" void func_80255210(CCollepedia* self);
extern "C" void func_8025516C(CCollepedia* self);
extern "C" void func_802553AC(CCollepedia* self);
extern "C" void func_80254E64(CCollepedia* self);
extern "C" void func_80254F2C(CCollepedia* self);
extern "C" void func_80255000(CCollepedia* self);
extern "C" void func_802550B4(CCollepedia* self);
// CfGameManager controller-type query: retail loads r3=-1 into the symbol
// (the no-arg member wrapper drops it), so call through the C name directly.
extern "C" int func_80086F9C__Q22cf13CfGameManagerFv(int arg);

// Flat pad-data view exposing only the flag words func_80252DD8 reads:
// CPad::mPressedButtonFlags at +0x4 and CfPadData::mTurboPressButtonFlags at
// +0x104 (see kyoshin/cf/CfPadData.hpp for the full layout).
struct MenuCollepediaPadData {
    u32 mPadHeldFlags;          // 0x00 - CPad::mHeldButtonFlags
    u32 mPadPressedFlags;       // 0x04 - CPad::mPressedButtonFlags
    u8 _pad[0xFC];              // 0x08-0x103 - rest of the pad data
    u32 mTurboPressButtonFlags; // 0x104
};

// One 8-byte element of the CCollepedia sub-array copied by Init's loop
// (0x500 entries at CCollepedia+0xE8, stride 8).
struct CollepediaSubEntry {
    u32 field_0;
    u32 field_4;
};

// vtable / PMF / shared data (MWCC does not mangle global-scope data names).
extern u32 __ptmf_null[3];
extern char lbl_eu_8052BF70[];   // CProcess primary vtable (pre-PMF copy)
extern char lbl_eu_805372A0[];   // CMenuCollepedia composite vtable
extern char lbl_eu_8050C688[];   // message-name string pool (Init title lookup)
// Menu timer constants (sdata2 pool): init 0.0f, per-frame +1.0f, clamp 10.0f.
extern const f32 lbl_eu_806687E0;
extern const f32 lbl_eu_806687E4;
extern const f32 lbl_eu_806687E8;

// Shared singleton (retail name; global-scope plain extern keeps unmangled symbol).
extern u32 lbl_eu_806647D0;
