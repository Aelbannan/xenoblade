#pragma once

// Unit-private import declarations split out of CMenuSkipTimer.hpp so the class header
// stays includable next to other headers.
#include "kyoshin/menu/CMenuSkipTimer.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"


extern "C" void SkipTimer_Phase0Init(CMenuSkipTimer* self);
extern "C" void SkipTimer_Phase1Mark2(CMenuSkipTimer* self);
extern "C" void SkipTimer_Phase3Handle(CMenuSkipTimer* self);

// Retail-unmangled callee names (US strips mangling for these func_ helpers).
// int returns so the caller's `!= 0` stays a plain cmpwi (retail emits no u8 mask).
extern "C" int CSkipTimerGetSkipButton(CSkipTimer* self);
extern "C" void CSkipTimerTeardown(CSkipTimer* self);
extern "C" void BgTex_Release_3D9C(CBgTex* self);
extern "C" void teardown(CTitleAHelp* self);
extern "C" void setPresentationFlag__Q22cf13CfGameManagerFv(u8 enable);

// Secondary mode bitfield (bit 21 / IBM bit 10 = realtime event busy).
extern u32 lbl_eu_80663E28;

// Retail-unmangled same-unit CSkipTimer / CMenuSkipTimer helpers referenced by
// Move/Init. Declared extern "C" so calls bind the literal (un)mangled retail
// symbol (US strips mangling for these func_ names). int returns keep the
// caller's `!= 0` a plain cmpwi (retail emits no u8 mask; the defining TU's u8
// return is ABI-compatible, value in r3).
extern "C" int CSkipTimerIsEngaged(CSkipTimer* self);
extern "C" void CSkipTimerTick(CSkipTimer* self);
extern "C" void CSkipTimer_BeginLoad(CSkipTimer* self);
extern "C" void SkipTimer_Phase2PollPad(CMenuSkipTimer* self);
extern "C" void BgTex_Tick_3D54(CBgTex* self);
extern "C" void updateHelp(CTitleAHelp* self);
// Retail-unmangled CW helpers used by Init()'s temporary-object re-init pattern.
extern "C" void __ct__6CBgTexFUc(void*, int);
extern "C" void __dt__6CBgTexFv(void*, int);
extern "C" void __as__17UnkClass_8045F564FRC17UnkClass_8045F564(void*, void*);
extern "C" void __ct__11CTitleAHelpFPcUc(void*, char*, int);
extern "C" void __dt__11CTitleAHelpFv(void*, int);
extern "C" void __dt__10CSkipTimerFv(void*, int);
extern "C" void BgTex_Acquire_3C14(CBgTex* self);
// CTitleAHelp::CTitleAHelp_load() is declared in kyoshin/CTitleAHelp.hpp.
// Draw/advance helpers for cbRenderBefore / SkipTimer_Phase2PollPad (retail keeps the
// bare func_ names at these call sites; int returns keep `!= 0` as cmpwi).
extern "C" int CSkipTimerIsReady(CSkipTimer* self);
extern "C" void CSkipTimerConfirmSkip(CSkipTimer* self);
extern "C" void CSkipTimerEngageSkip(CSkipTimer* self);
extern "C" void CSkipTimerNextKey(CSkipTimer* self);
extern "C" void CSkipTimerPrevKey(CSkipTimer* self);
extern "C" void CSkipTimerLeaveSkip(CSkipTimer* self);
extern "C" int isSkipActive(CSkipTimer* self);
extern "C" void beginClose(CTitleAHelp* self);
extern "C" void BgTex_Draw_3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void CSkipTimerDraw(CSkipTimer* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void drawHelp(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
// DrawInfo setup helper; the C++ declaration mangles to the retail name
// SetWideLocAdjust__FPQ34nw4r3lyt8DrawInfo.
// Scene-active gate for HUD draws (retail-unmangled; code_80135FDC.cpp).
// String-table base for the skip-timer title/help name lookup.
extern char lbl_eu_80510540[];
// Retail-mangled CfGameManager helper that accepts a pad-channel arg (keeps the
// `li r3, -1` materialisation that retail emits; cf::CfGameManager's inline s16
// overload discards it). Declared extern "C" so the call binds the verbatim
// retail symbol.
