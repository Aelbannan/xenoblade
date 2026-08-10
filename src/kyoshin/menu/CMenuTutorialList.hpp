#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>

#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CTutorialList.hpp"

// Forward declarations: the owning scene (CScn.hpp pulls broken monolib
// umbrella headers; keep the pointer opaque, same scheme as CMenuCollepedia)
// and the tutorial widget type (only pointer params in this header).
class CScn;
class CTutorial;

/**
 * Tutorial list menu widget.
 *
 * MI layout: CProcess at 0x00, IScnRender subobject (member) at 0x58.
 * The thunk functions func_802ACBCC / func_802ACBD4 adjust this by -0x58
 * when entering through the IScnRender vtable.
 *
 * Member offsets recovered from the retail constructor (802AE4E8):
 *   0x00-0x3B: CProcess base
 *   0x3C-0x53: compiler PMF (pointer-to-member) callback slots
 *   0x54/0x55: u8 flags
 *   0x58:      IScnRender subobject (vtable ptr)
 *   0x5C:      CScn* mScene
 *   0x60:      CBgTex (size 0x20)
 *   0x80:      CTitleAHelp (size 0x38)
 *   0xB8:      CTutorialList (size 0x288)
 *   0x340:     CTutorial (size 0x54)
 *   0x394:     f32 mTimer
 *   0x398:     u8 mFlag
 *   0x39A:     u16 mChoice
 * Total size: 0x39C
 */
class CMenuTutorialList : public CProcess {
public:
    virtual ~CMenuTutorialList();
    virtual void Init();
    virtual void Term();
    virtual void Move();

    // Render callback (dispatched through the IScnRender subobject vtable)
    void cbRenderBefore();

    u32 ptmf0[3];             // 0x3C-0x47: null PMF callback slot group 1
    u32 ptmf1[3];             // 0x48-0x53: null PMF callback slot group 2
    u8 mField54;              // 0x54: phase/state flag (written by func_802AC8A4)
    u8 mField55;              // 0x55
    u8 _pad56[2];             // 0x56-0x57
    IScnRender mIScnRender;   // 0x58-0x5B: render-callback subobject (vptr)
    CScn* mScene;             // 0x5C: owning scene
    CBgTex mBgTex;            // 0x60-0x7F: background layout widget
    CTitleAHelp mTitleAHelp;  // 0x80-0xB7: title/help bar
    u8 mTutorialList[0x288];  // 0xB8-0x33F: CTutorialList (header type is layout-incomplete; opaque buffer)
    u8 mTutorial[0x54];       // 0x340-0x393: CTutorial
    f32 mTimer;               // 0x394
    u8 mFlag;                 // 0x398: phase byte (state machine in Move)
    u16 mChoice;              // 0x39A
    CMenuTutorialList();
};
// Total size: 0x39C

// Shared singleton / string pool (retail names; global-scope plain extern
// keeps the symbols unmangled).
extern unsigned long lbl_eu_80664BE8;  // menu-open singleton (0 = closed)
extern char lbl_eu_80510B44[];         // title/help label string pool

// IScnRender vtable this-adjusting thunks (retail: subi r3, r3, 0x58; b ...).
void func_802ACBCC(IScnRender* sub);
void func_802ACBD4(IScnRender* sub);

// Retail-unmangled phase handlers (US keeps the bare func_ names; C linkage
// so Move's dispatch calls and the definitions bind the literal symbols).
extern "C" void func_802AC520(CMenuTutorialList* self);
extern "C" void func_802AC598(CMenuTutorialList* self);
extern "C" void func_802AC5E8(CMenuTutorialList* self);
extern "C" void func_802AC8A4(CMenuTutorialList* self);
extern "C" void func_802AC8F4(CMenuTutorialList* self);
extern "C" void func_802AC970(CMenuTutorialList* self);
extern "C" void func_802ACA38(CMenuTutorialList* self);
extern "C" void func_802ACB50(CMenuTutorialList* self);

// Retail-unmangled callee names (US retail strips the mangling for these
// widget helpers) -- C linkage so calls bind the literal retail symbol.
// Return types are int (not u8) so callers compare with cmpwi directly.
extern "C" int isIdle__11CTitleAHelpFv(CTitleAHelp* h);
extern "C" int func_801C3E34(CBgTex* self);
extern "C" int func_801C4114(CTitleAHelp* self);
extern "C" void func_801C412C(CTitleAHelp* self);
extern "C" int func_802AD300(CTutorialList* self);
extern "C" int func_802AD2A4(CTutorialList* self);
extern "C" void func_802AD308(CTutorialList* self);
extern "C" void func_801C4760(CTitleAHelp* self);
extern "C" void func_801C4744(CTitleAHelp* self);
extern "C" void func_801C41C0(CTitleAHelp* self, char* name);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 mode);
extern "C" void func_801C3D9C(CBgTex* self);
extern "C" void func_801C3D54(CBgTex* self);
extern "C" void func_801C3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801C3FF0(CTitleAHelp* self);
extern "C" void func_801C4080(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801C40A0(CTitleAHelp* self);
extern "C" u16 func_802AD838(CTutorialList* self);
extern "C" void func_802AD854(CTutorialList* self);
extern "C" void func_802AD0E0(CTutorialList* self);
extern "C" void func_802AD188(CTutorialList* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_802AD1F4(CTutorialList* self);
extern "C" int func_8029ACAC(CTutorial* self);
extern "C" int func_8029ACB4(CTutorial* self);
extern "C" int func_8029ACBC(CTutorial* self);
extern "C" u8 func_8029AE5C(CTutorial* self);
extern "C" void func_8029AD88(CTutorial* self);
extern "C" void func_8029ACEC(CTutorial* self);
extern "C" void func_8029ADF8(CTutorial* self);
extern "C" void func_8029ABB8(CTutorial* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_8029AB28(CTutorial* self);
extern "C" void func_8029ACC4(CTutorial* self);
extern "C" void func_8029ABD8(CTutorial* self);
extern "C" u8 func_8013B980();
extern "C" u8 code80135FDC_getByte_64080();
extern "C" void func_8008294C__Q22cf13CfGameManagerFv(u8 enable);
// CfGameManager controller-type query: retail loads r3=-1 into the symbol (the
// no-arg member wrapper drops it), so call through the C name directly.
extern "C" int func_80086F9C__Q22cf13CfGameManagerFv(int arg);
// UI sound playback (retail unmangled C symbol; id + param).
extern "C" void func_8009D018(u32 soundId, u32 arg);
// Save/skip system busy queries (retail unmangled).
extern "C" int func_800FEDF8();
extern "C" void func_800FF914();
// IScnRender-vtable thunk targets: the C++ members are emitted under their
// mangled names; C linkage keeps the verbatim retail symbols and stops the
// compiler from inlining the (same-TU) member bodies into the thunks.
extern "C" void cbRenderBefore__17CMenuTutorialListFv(CMenuTutorialList* self);
extern "C" CMenuTutorialList* __dt__17CMenuTutorialListFv(CMenuTutorialList* self, int flags);

// Retail constructor symbol (unmangled global in US). Free-function form so
// the factory emits a real bl to the bare retail symbol; returns `this` in r3
// like retail.
extern "C" CMenuTutorialList* __ct__CMenuTutorialList(CMenuTutorialList* self, CProcess* parent, u32 arg2);

// Widget/base destructors called by the member dtor (US retail keeps the
// mangled __dt__ names; the first two are C-ABI wrappers for the opaque
// embedded widget storages).
extern "C" void __dt__9CTutorialFv(CTutorial* self, int dealloc);
extern "C" void __dt__13CTutorialListFv(CTutorialList* self, int dealloc);
extern "C" void __dt__11CTitleAHelpFv(CTitleAHelp* self, int dealloc);
extern "C" void __dt__6CBgTexFv(CBgTex* self, int dealloc);
extern "C" void __dt__800FED0C(CProcess* self, int flags);

// Embedded-widget ctors called by the member ctor (US retail strips these
// manglings; C-ABI wrappers for the opaque embedded storages).
extern "C" void __ct__8CProcessFv(CProcess* self);
extern "C" void __ct__CBgTex(CBgTex* self, u8 arg);
extern "C" void __ct__CTitleAHelp(CTitleAHelp* self, char* name, u8 arg);
extern "C" void __ct__CTutorialList(CTutorialList* self, int arg);
extern "C" void __ct__CTutorial(CTutorial* self, int arg, int arg2);

// vtable / PMF / shared data (MWCC does not mangle global-scope data names).
extern u32 __ptmf_null[3];
extern char lbl_eu_8052BF70[];    // CProcess primary vtable (pre-PMF copy)
extern char lbl_eu_8053A238[];    // CMenuTutorialList composite vtable
// Game-state flags word (bit 21 = menu blocked; shared with CMenuTutorial).
extern u32 lbl_eu_80663E28;
// Menu timer init constant (.sdata2 pool).
extern const f32 lbl_eu_80668DC8;
