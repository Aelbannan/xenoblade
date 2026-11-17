#pragma once

// Unit-private import declarations split out of CSkipTimer.hpp so the class header
// stays includable next to other headers.
#include "kyoshin/CSkipTimer.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"


// C-linkage imports - retail symbol names; extern "C" so calls bind to the
// literal (un)mangled identifiers. (Retail names are the true symbol names;
// do not rename.)
// CSkipTimer2 vtable pointer stored at +0x00.
// CSkipTimer vtable pointer stored at +0x00.
// String pool used by CSkipTimerStampSlotText (pane name + sprintf format).
extern "C" char lbl_eu_80510568[];
// CSkipTimer2 ctor: retail symbol is the short form (no length/sig suffix),
// so it must be a C-linkage function carrying the exact retail name
// (MWCC_CASES sec. 3824); the dtor keeps the mangled member form.
extern "C" CSkipTimer2* __ct__CSkipTimer2(CSkipTimer2* self, void* parent);
// CSkipTimer ctor: same retail short-form treatment; returns the object.
extern "C" CSkipTimer* __ct__CSkipTimer(CSkipTimer* self);

// External callees.
extern "C" void __dt__7CSysWinFv(void*, int);
extern "C" void __ct__7CSysWinFUc(void* self, int arg);
extern "C" UnkClass_8045F564* __ct__17UnkClass_8045F564Fv(UnkClass_8045F564* self);
// Mem-region copy initializer used by the temp-copy widget-rebuild pattern
// (CMenuSave / CMenuCollepedia convention; retail symbol is unmangled).
extern "C" void __as__17UnkClass_8045F564FRC17UnkClass_8045F564(void* dest, void* src);

extern "C" int CSysWin_getUnk34(void*);
extern "C" u32 CSysWin_isReady(void*);

// .sdata2 float constants used by the sinit_802A07D8 static initializer.
extern "C" float lbl_eu_80664A50;
extern "C" const float lbl_eu_80668C4C;
extern "C" const float lbl_eu_80662C78;
extern "C" const float lbl_eu_80668C48;
extern "C" void sysWinDrawLayout(void*, void*);
extern "C" void sysWinTermLayout(void*);
// C-exported retail symbol: unmangled reloc name must be emitted at call sites.
extern "C" int CSysWin_isActive(void*);
extern "C" void sysWinAdvancePhase3(void*);
extern "C" u32 CMainMenu_GetInstancePtr();
extern "C" void ArtsInfo_SetReadyFlag();

// Shared-arc font/text value feeding setLayoutTextBoxFont (unmangled retail symbol).
// Sub-controller layout rebind helper (unmangled retail symbol).
extern "C" void CSkipTimer2RebindAnim(CSkipTimer2* self);
// Post-build flag setter on the sub-controller (unmangled retail symbol).
extern "C" void CSkipTimer2MarkActive(u8* self);

// Unmatched same-unit siblings referenced as extern (linker resolves to retail
// address) so in-unit callers emit a direct `bl` instead of inlining a stub.
// Retail strips mangling for these func_ names in US, hence extern "C".
extern "C" void CSkipTimer2SelectSlotPane(CSkipTimer2* self, u8 arg);
extern "C" void func_8029F6EC(CSkipTimer2* self);
extern "C" void CSkipTimer2OnAdvanceEnd(CSkipTimer2* self);
// CSkipTimer_BeginLoad: System.arc load kickoff (retail strips mangling on US func_
// names), so the definition binds to the literal retail symbol.
extern "C" void CSkipTimer_BeginLoad(CSkipTimer* self);
// CSkipTimer2RebuildLayout: post-build hook on the sub-controller (body lives in retail).
extern "C" void CSkipTimer2RebuildLayout(CSkipTimer2* self);
extern "C" int CSkipTimerMapKeyToAction(CSkipTimer* self);
extern "C" void CSkipTimer2Start(CSkipTimer2* self, u8 arg);
// CSkipTimer2Reset: reset the sub-controller (body lives in retail).
extern "C" void CSkipTimer2Reset(CSkipTimer2* self);

// Same-unit siblings DEFINED in this TU whose retail callers emit a direct
// `bl` (retail keeps them out-of-line). The definitions in CSkipTimer.cpp are
// guarded with #pragma auto_inline off (MWCC_CASES sec. hbm/seq) so
// -inline auto does not inline them into CSkipTimer2DriveState / CSkipTimerActivateTimer /
// CSkipTimerNextKey; the extern "C" form binds both the definitions and the call
// sites to the literal (un)mangled retail names.
extern "C" void CSkipTimer2OnFwdDone(CSkipTimer2* self);
extern "C" void CSkipTimer2OnRewindDone(CSkipTimer2* self);
extern "C" void CSkipTimer_UpdateSlotFrame(CSkipTimer* self);
extern "C" void CSkipTimerBindAnim24(CSkipTimer* self);
extern "C" void CSkipTimerBindAnim20(CSkipTimer* self);
extern "C" void CSkipTimerActivateTimer(CSkipTimer* self);

class CSysWin;
// CSysWin per-frame update (retail symbol is unmangled; defined extern "C" in CSysWin.cpp).
extern "C" void sysWinDispatchPhase(CSysWin* sysWin);

// cf::CfGameManager static (retail symbol keeps the Fv suffix from the
// decompiler guess; the real signature is 3 u32s, see CREvtCamera.cpp).
extern "C" void func_80086B5C__Q22cf13CfGameManagerFv(u32, u32, u32);

// Window content setters from CSysWin.cpp (retail-unmangled names).
extern "C" void sysWinSetKindContent(void* syswin, void* str1, void* str2);
extern "C" void sysWinOpenPhase1(void* syswin);
