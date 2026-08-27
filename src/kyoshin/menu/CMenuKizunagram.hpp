#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>

#include "kyoshin/CFade.hpp"
#include "kyoshin/CTitleAHelp.hpp"

// Shield CLoad.hpp's C++-linkage helper declarations: this TU needs the
// CLoad class, but declares the func_802AExxxx helpers itself below as
// extern "C" so call relocs carry the retail UNMANGLED symbol names
// (identical plain decls emit mangled __FP5CLoad refs). MWCC 10505.
#define func_802AE508 func_802AE508_CLoadHdr
#define func_802AE560 func_802AE560_CLoadHdr
#define func_802AE5F0 func_802AE5F0_CLoadHdr
#define func_802AE62C func_802AE62C_CLoadHdr
#define func_802AE6AC func_802AE6AC_CLoadHdr
#define func_802AE6B4 func_802AE6B4_CLoadHdr
#define func_802AE6BC func_802AE6BC_CLoadHdr
#define func_802AE6C4 func_802AE6C4_CLoadHdr
#define func_802AE758 func_802AE758_CLoadHdr
#define func_802AE7EC func_802AE7EC_CLoadHdr
#define func_802AE894 func_802AE894_CLoadHdr
#include "kyoshin/CLoad.hpp"
#undef func_802AE508
#undef func_802AE560
#undef func_802AE5F0
#undef func_802AE62C
#undef func_802AE6AC
#undef func_802AE6B4
#undef func_802AE6BC
#undef func_802AE6C4
#undef func_802AE758
#undef func_802AE7EC
#undef func_802AE894

// Opaque kizuna-chart sub-object at +0x98 (spans 0x98..0x178). Driven by
// func_8025C78C (CKizunagram unit); detailed layout TBD.
struct UnkKizunaMenuSub98 {
    u8 data[0xE0];
};

// Opaque slot for the CPcKizunagram sub-object at +0x178 (sizeof 0x4C).
// CPcKizunagram.hpp is not included here: its extern "C" func_8025DA48
// declares a u8 return, which makes MWCC emit an rlwinm byte mask at the
// call site; retail compares r3 directly, so the call-site declaration must
// return int.
struct UnkKizunaMenuPcSub {
    u8 data[0x4C];
};

// Kizuna gram (affinity chart) screen shown on the World Map.
// Inherits from CProcess (task system) and owns an IScnRender render-callback
// subobject at offset +0x58 (requires this-adjusting thunks).
//
// Layout (mirrors CMenuMapSelectSC; recovered from retail member offsets):
//   0x00: CProcess
//   0x3C-0x53: null PMF callback slots (2 groups of 3 words)
//   0x54/0x55: u8 flags
//   0x58: IScnRender (member subobject, vptr)
//   0x5C: CProcess* parent reference
//   0x60: CTitleAHelp
//   0x98: kizuna chart sub-object (opaque, CKizunagram 0xE0)
//  0x178: CPcKizunagram
//  0x1C4: CLoad
//  0x1F4: CFade
//  0x21C: u8 state
class CMenuKizunagram : public CProcess {
public:
    CMenuKizunagram();
    virtual ~CMenuKizunagram();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    void cbRenderBefore();

    // IScnRender vtable this-adjusting thunks
    void func_80257A7C();
    void func_80257A84();

    u32 ptmf0[3];                    // 0x3C-0x47: null PMF callback slot group 1
    u32 ptmf1[3];                    // 0x48-0x53: null PMF callback slot group 2
    u8 field_0x54;                   // 0x54
    u8 field_0x55;                   // 0x55
    u8 _pad56[2];                    // 0x56-0x57
    IScnRender mIScnRender;          // 0x58: render-callback subobject (vptr)
    CProcess* mParentRef;            // 0x5C
    CTitleAHelp mTitleAHelp;         // 0x60 (sizeof 0x38)
    UnkKizunaMenuSub98 mSub98;       // 0x98 (opaque CKizunagram slot, 0xE0)
    UnkKizunaMenuPcSub mPcKizunagram; // 0x178 (CPcKizunagram slot)
    CLoad mLoad;                     // 0x1C4 (sizeof 0x30)
    CFade mFade;                     // 0x1F4 (sizeof 0x28)
    u8 field_0x21C;                  // 0x21C
    u8 field_0x21D;                  // 0x21D
    u8 field_0x21E;                  // 0x21E
};

// CScn / CTaskGame forward declarations (their full headers pull broken
// monolib umbrella headers; keep the pointers opaque, same scheme as
// CMenuBattleEnd / CMenuMapSelectSC).
class CScn;
class CTaskGame;

// Retail-unmangled callee names (US strips mangling for these func_ helpers);
// declared extern "C" so call sites emit the literal retail reloc names.
// Return types are int (not u8) so callers compare with cmpwi directly
// (no byte mask), matching retail.
extern "C" int isIdle__11CTitleAHelpFv(CTitleAHelp* h);
extern "C" int func_8025C78C(UnkKizunaMenuSub98* self);
extern "C" int func_8025DA48(UnkKizunaMenuPcSub* self);
extern "C" int func_80244508(CFade* self);
extern "C" void func_80244518(CFade* self);

// Additional retail-unmangled callee names for the kizuna-gram screen state
// helpers (same scheme as CMenuMapSelectSC / CMakeCrystalWin). Return types
// are int where retail compares r3 with cmpwi directly (no byte mask);
// func_8025CBCC is u8 because retail masks its result with clrlwi.
// Retail-unmangled views of the CLoad helpers (see include shield above).
extern "C" u8 func_802AE6AC(CLoad* self);
extern "C" void func_802AE6C4(CLoad* self);
extern "C" void func_802AE758(CLoad* self);
extern "C" int func_80244510(CFade* self);
extern "C" void func_80244538(CFade* self);
extern "C" int func_8025C770(UnkKizunaMenuSub98* self);
extern "C" u8 func_8025CBCC(UnkKizunaMenuSub98* self);
extern "C" void func_8025C7D0(UnkKizunaMenuSub98* self);
extern "C" int func_801C4114(CTitleAHelp* self);
extern "C" void func_801C412C(CTitleAHelp* self);
extern "C" void func_801C416C(CTitleAHelp* self);
extern "C" void func_801C41C0(CTitleAHelp* self, char* name);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 arg);
extern "C" char* func_80136190(char* a, char* b, int id);

// D2-form subobject/base destructor helpers (explicit delete flags), used by
// the free-function form of ~CMenuKizunagram. __dt__800FED0C is the CProcess
// base dtor retail symbol (the game-side D2 wrapper, distinct from the
// library D1 __dt__8CProcessFv). __dt__13CPcKizunagramFv / __dt__11CKizunagramFv
// are the retail dtor names for the opaque sub-objects at +0x178 / +0x98.
extern "C" void __dt__5CFadeFv(CFade* _this, int flags);
extern "C" void __dt__5CLoadFv(CLoad* _this, int flags);
extern "C" void __dt__13CPcKizunagramFv(UnkKizunaMenuPcSub* _this, int flags);
extern "C" void __dt__11CKizunagramFv(UnkKizunaMenuSub98* _this, int flags);
extern "C" void __dt__11CTitleAHelpFv(CTitleAHelp* _this, int flags);
extern "C" void __dt__800FED0C(CProcess* _this, int flags);

// Retail constructor (unmangled global, ctor with two explicit args) and the
// D2-form destructor (keeps its C++ mangling in US). The IScnRender dtor
// thunk references __dt__15CMenuKizunagramFv by this symbol.
extern "C" CMenuKizunagram* __ct__CMenuKizunagram(CMenuKizunagram* self, int a, int b);
extern "C" CMenuKizunagram* __dt__15CMenuKizunagramFv(CMenuKizunagram* _this, int flags);

// C++-linkage helper - retail emits the mangled form func_80138078__FUl.
void func_80138078(unsigned long op);

// Scene/task-system helpers (retail-unmangled callee names).
extern "C" void waitForDrawDone__9CDeviceVIFv();
extern "C" void removeRenderCB__4CScnFP10IScnRender(CScn*, IScnRender*);
extern "C" int func_804962A0(CScn* scn, int flag);
extern "C" CTaskGame* getInstance__9CTaskGameFv();
extern "C" int isFlag01Set__9CTaskGameFv();
extern "C" void setPresentationFlag__Q22cf13CfGameManagerFv(bool enable);

// code_80135FDC helpers (retail-unmangled).
extern "C" u8 func_8013B980();
extern "C" u8 code80135FDC_getByte_64080();
extern "C" void func_801338C8();
extern "C" int func_8013BE50();

// CTitleAHelp / CFade helpers used by Term / cbRenderBefore (retail strips
// the mangling for these member helpers).
extern "C" void func_801C40A0(CTitleAHelp* self);
extern "C" void func_801C4080(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801C4198(CTitleAHelp* self);
extern "C" void func_80244460(CFade* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_8024448C(CFade* self);

// CLoad draw/reset helpers.
extern "C" void func_802AE62C(CLoad* self);
extern "C" void func_802AE5F0(CLoad* self, nw4r::lyt::DrawInfo* drawInfo);

// Kizuna-chart sub-object helpers (retail-unmangled; CKizunagram unit).
extern "C" void func_8025C6F0(UnkKizunaMenuSub98* self);
extern "C" void func_8025C61C(UnkKizunaMenuSub98* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_8025C870(UnkKizunaMenuSub98* self);
extern "C" int func_8025CBBC(UnkKizunaMenuSub98* self);
extern "C" int func_8025CBC4(UnkKizunaMenuSub98* self);
extern "C" void func_8025C7FC(UnkKizunaMenuSub98* self, int arg);
extern "C" void func_8025CC70(UnkKizunaMenuSub98* self);
extern "C" void func_8025CAE4(UnkKizunaMenuSub98* self);
extern "C" void func_8025CB50(UnkKizunaMenuSub98* self);
extern "C" void func_8025C874(UnkKizunaMenuSub98* self);
extern "C" void func_8025C904(UnkKizunaMenuSub98* self);
extern "C" void func_8025C994(UnkKizunaMenuSub98* self);
extern "C" void func_8025CA24(UnkKizunaMenuSub98* self);
extern "C" void func_8025CAB4(UnkKizunaMenuSub98* self);
extern "C" void func_801C414C(CTitleAHelp* self);
// CPcKizunagram helpers. Return types are int (not u8) so callers compare
// with cmpwi directly (no byte mask), matching retail.
extern "C" int func_8025DA40(UnkKizunaMenuPcSub* self);
extern "C" void func_8025D9C4(UnkKizunaMenuPcSub* self);
extern "C" void func_8025D954(UnkKizunaMenuPcSub* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_8025DA50(UnkKizunaMenuPcSub* self);
extern "C" void func_8025DA78(UnkKizunaMenuPcSub* self);
extern "C" void func_8025DAE8(UnkKizunaMenuPcSub* self);
extern "C" void func_8025DB30(UnkKizunaMenuPcSub* self);
extern "C" void func_8025DB78(UnkKizunaMenuPcSub* self);
extern "C" void func_8025DBC0(UnkKizunaMenuPcSub* self);

// Move() state-dispatch helpers (retail-unmangled; defined in this TU).
// Declared extern "C" so the switch-dispatch call sites in Move() emit the
// literal retail reloc names (the definitions below inherit C linkage).
extern "C" void func_80257318(CMenuKizunagram* self);
extern "C" void func_80257360(CMenuKizunagram* self);
extern "C" void func_802573B8(CMenuKizunagram* self);
extern "C" void func_80257448(CMenuKizunagram* self);
extern "C" void func_80257498(CMenuKizunagram* self);
extern "C" void func_80257704(CMenuKizunagram* self);
extern "C" void func_80257754(CMenuKizunagram* self);
extern "C" void func_802577F0(CMenuKizunagram* self);
extern "C" void func_80257840(CMenuKizunagram* self);
extern "C" void func_80257994(CMenuKizunagram* self);
extern "C" void func_80257A2C(CMenuKizunagram* self);

// Move() per-frame sub-object refresh helpers (retail-unmangled).
extern "C" void func_801C3FF0(CTitleAHelp* self);
extern "C" void func_8025C580(UnkKizunaMenuSub98* self);
extern "C" void func_8025D8C4(UnkKizunaMenuPcSub* self);
extern "C" void func_802AE560(CLoad* self);
extern "C" void func_802443E8(CFade* self);

// Sound helpers used by Move() (retail-unmangled).
extern "C" int func_800FEDF8();
extern "C" void func_800FF914();

// Sub-object / base constructors (retail-unmangled ctor names).
extern "C" void __ct__8CProcessFv(CProcess* self);
extern "C" void __ct__CTitleAHelp(CTitleAHelp* self, char* name, u8 arg);
extern "C" void __ct__CKizunagram(UnkKizunaMenuSub98* self, int arg);
extern "C" void __ct__CPcKizunagram(UnkKizunaMenuPcSub* self);
extern "C" void __ct__CLoad(CLoad* self, u8 arg);
extern "C" void __ct__CFade(CFade* self);

// Ctor tail helpers (retail-unmangled).
extern "C" void func_80042874();
extern "C" void func_8011C400();

// C++-linkage helper - retail emits the mangled form
// func_80137250__FPQ34nw4r3lyt8DrawInfo.
void func_80137250(nw4r::lyt::DrawInfo* pDrawInfo);
// nw4r DrawInfo pre-mangled ct/dt names (retail emits the direct calls; a
// C++ local would virtual-dispatch its scope-exit destructor and bloat the
// body).
extern "C" void __ct__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* drawInfo);
extern "C" void __dt__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* drawInfo, int flags);

// Flat pad-data view exposing only the flag words func_80257840 reads:
// CPad::mPressedButtonFlags at +0x4 and CfPadData::mTurboPressButtonFlags at
// +0x104 (see kyoshin/cf/CfPadData.hpp for the full layout).
struct KizunaPadData {
    u32 mPadHeldFlags;          // 0x00 - CPad::mHeldButtonFlags
    u32 mPadPressedFlags;       // 0x04 - CPad::mPressedButtonFlags
    u8 _pad[0xFC];              // 0x08-0x103 - rest of the pad data
    u32 mTurboPressButtonFlags; // 0x104
};
extern "C" KizunaPadData* getCfPadData__Q22cf13CfGameManagerFv();
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int arg);

// Data imports (MWCC does not mangle global-scope data names).
extern u32 lbl_eu_806647E0;    // CMenuKizunagram singleton pointer (sbss)
extern char lbl_eu_8050CAB8[]; // kizuna-gram string pool base
extern u32 lbl_eu_80663E28;    // global mode bitfield (bit 0x200000 = kizuna gate)
extern u8 lbl_eu_8052BF70[];   // temporary vtable (pre-PMF copy)
extern u8 lbl_eu_805374B0[];   // final vtable; IScnRender vtable at +0x24
extern u32 __ptmf_null[3];     // null pointer-to-member-function constant
