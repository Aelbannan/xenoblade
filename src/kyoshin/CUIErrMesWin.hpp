#pragma once

#include <types.h>
#include "monolib/work/CTTask.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CTitle.hpp"
#include "kyoshin/cf/object/CObjectParam.hpp"
#include "monolib/math/Random.hpp"
#include "kyoshin/cf/voice/CCharVoice.hpp"
#include <nw4r/math/math_types.h>

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void cbRenderBefore__12CUIErrMesWinFv();
extern "C" void __dt__12CUIErrMesWinFv();

// CSysWin helper imports (retail C-linkage; same declarations as CSaveLoad.hpp).
extern "C" u32 CSysWin_isReady(void*);
extern "C" int CSysWin_isActive(void*);

// +0x60 sub-object advance helper (retail C-linkage, defined in CTitle.cpp).
extern "C" void func_802B744C(void*);

// CSysWin advance helper (retail C-linkage; same declaration as CSystemWindow.hpp).
extern "C" void func_8022B748(void*);

// CSysWin content setters / advance-close helpers (retail unmangled names,
// defined in CSysWin.cpp). func_8022BFC8 is declared by CSysWin.hpp.
extern "C" void func_8022B9B4(void* syswin, const char* msg, int flag);
extern "C" void func_8022B8B8(void* syswin);
extern "C" void func_8022B8E4(void* syswin);

// Pane-name format helper (code_80135FDC.cpp, retail unmangled name).
extern "C" char* func_80136190(char* fmt, char* base, u32 id);

// String pool used by func_802B4F40's error message.
extern char lbl_eu_805135E0[];

// cf::CfGameManager current-pad accessor (retail pre-mangled static).
void* getCurrentPad__Q22cf13CfGameManagerFv();

// Pad view for func_802B4FE8: button word at +4.
struct CErrMesPad {
    u8 _00[4];
    u32 field_0x4;
};

// Abstract view over the embedded CSysWin so Init can dispatch the layout-build
// virtual at vtable offset 0x88 (declared index 32 after MWCC's RTTI prefix
// slots). Pure virtuals only - no vtable emitted.
class CErrMesSysWinView {
public:
    virtual void v00() = 0;
    virtual void v01() = 0;
    virtual void v02() = 0;
    virtual void v03() = 0;
    virtual void v04() = 0;
    virtual void v05() = 0;
    virtual void v06() = 0;
    virtual void v07() = 0;
    virtual void v08() = 0;
    virtual void v09() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual void v12() = 0;
    virtual void v13() = 0;
    virtual void v14() = 0;
    virtual void v15() = 0;
    virtual void v16() = 0;
    virtual void v17() = 0;
    virtual void v18() = 0;
    virtual void v19() = 0;
    virtual void v20() = 0;
    virtual void v21() = 0;
    virtual void v22() = 0;
    virtual void v23() = 0;
    virtual void v24() = 0;
    virtual void v25() = 0;
    virtual void v26() = 0;
    virtual void v27() = 0;
    virtual void v28() = 0;
    virtual void v29() = 0;
    virtual void v30() = 0;
    virtual void v31() = 0;
    virtual void v32() = 0;  // offset 0x88 - layout build
};

// CSysWin release helper (retail C-linkage; same declaration as CSystemWindow.hpp).
extern "C" void func_8022B7F4(void*);

// CMenuTitle +0x60 sub-object advance/release helpers (retail C-linkage, CTitle.cpp).
extern "C" void func_802B73D4(void*);
extern "C" void func_802B74F4(void*);

// cf::CfGameManager helper. Retail names it with the no-arg Fv suffix, but the
// call site passes three zero args; declare the retail symbol verbatim under
// C linkage so calls bind to it (same pattern as func_80086F9C in CSysWin.hpp).
extern "C" void func_80081E90__Q22cf13CfGameManagerFv(int, int, int);

// cf::CfGameManager error-message helper (func_802B48E4 dispatches 0x29/0x24).
// Retail ships the no-arg-Fv mangled name verbatim; keep it under C linkage so
// the call site binds to the retail symbol (same pattern as func_80081E90).
extern "C" void func_80081F28__Q22cf13CfGameManagerFv(u32, u8*);

// Capsule geometry types used by func_802B5254. nw4r defines these in
// math_geometry.cpp (not in the shipped headers); identical layout here so the
// IntersectionCapsule call binds to the retail mangled symbol.
namespace nw4r {
namespace math {
struct SEGMENT3 {
    VEC3 a;  // 0x00
    VEC3 b;  // 0x0C
};
struct CAPSULE {
    SEGMENT3 segment;  // 0x00
    f32 radius;        // 0x18
};
bool IntersectionCapsule(const CAPSULE* pCapsule0, const CAPSULE* pCapsule1);
}  // namespace math
}  // namespace nw4r

// Capsule parameters used by func_802B5254's three sweeps (.sdata floats).
extern f32 lbl_eu_80668FB8;
extern f32 lbl_eu_80668FBC;
extern f32 lbl_eu_80668FC0;
extern f32 lbl_eu_80668FC4;
extern f32 lbl_eu_80668FC8;

// Global window-active flags cleared by the Term functions (.sbss words).
extern u32 lbl_eu_80664C28;
extern u32 lbl_eu_80664C30;

// Sub-object embedded at CErrMesEntry +0x09, managed by the func_802B5148 /
// func_802B515C / func_802B58A4 family. Its first byte is an active flag;
// the rest of the layout is opaque to this TU.
struct CErrMesSub {
    u8 field_0;  // +0x00 - active flag
};

// Error-message record (func_802B48A0 / func_802B48B8 family).
struct CErrMesEntry {
    /* 0x00 */ u32 field_0;
    /* 0x04 */ s32 field_4;   // state word (retail compares it signed: cmpwi)
    /* 0x08 */ u8 field_8;
    /* 0x09 */ CErrMesSub sub;  // embedded sub-object
};

// Error-message sub-object update (defined in this TU; declared extern "C" +
// noinline so func_802B4B84's call stays a direct `bl func_802B5148` instead
// of an inlined stub body).
extern "C" __declspec(noinline) void func_802B5148(CErrMesSub* self);

// CMenuTitle +0x60 sub-object render helper (retail C-linkage, CTitle.cpp):
// draws the title sub-object with a layout DrawInfo.
extern "C" void func_802B74A8(u8* self, nw4r::lyt::DrawInfo* drawInfo);

// CTitle sub-object destructor (retail C-linkage, CTitle.cpp).
extern "C" void __dt__6CTitleFv(u8* self, int flags);

// CSysWin sub-object destructor (retail C-linkage, CSysWin.cpp).
extern "C" void __dt__7CSysWinFv(u8* self, int flags);

// CUIErrMesWin dtor helper import: the layout DrawInfo constructor/destructor.
// Retail emits direct bl __ct__Q34nw4r3lyt8DrawInfoFv / bl
// __dt__Q34nw4r3lyt8DrawInfoFv (flag -1). A class-typed local would make MWCC
// virtual-dispatch the explicit dtor or add a scope-exit call, so the storage
// is a plain byte buffer with C-linkage ctor/dtor calls (CSysWinSave idiom).
extern "C" void __ct__Q34nw4r3lyt8DrawInfoFv(u8* self);
extern "C" void __dt__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* self, int flags);

// Layout DrawInfo setup helper - retail emits the mangled name
// func_80137250__FPQ34nw4r3lyt8DrawInfo, so it is a plain C++ declaration.
void func_80137250(nw4r::lyt::DrawInfo* drawInfo);

// Circular-list views used by func_802B58A4's active-flag sweeps. The lists
// returned by func_800B6BA4__Fv / func_800B6C7C share the same shape: the
// sentinel head lives at +0x04 and each node carries its item at +0x08.
struct CErrMesListNode {
    CErrMesListNode* next;  // 0x00
    u8 _04[4];              // 0x04..0x07
    u8* object;             // 0x08 - item (points at the +0x3E9C sub-object for the actor lists)
};
struct CErrMesList {
    u8* field_0x0;          // 0x00
    CErrMesListNode* sentinel;  // 0x04
};

// Voice/actor-manager list accessor (retail mangled name func_800B6BA4__Fv -
// plain C++ linkage reproduces the Fv suffix).
CErrMesList* func_800B6BA4();

// Enemy-manager list accessor (retail unmangled C symbol).
extern "C" CErrMesList* func_800B6C7C();

// Record base recovered from a list item by subtracting 0x3E9C (the item
// points at the embedded CfObjectMove sub-object). func_802B58A4 clears the
// +0x4590 active flag on every live record.
struct CErrMesRecord {
    u8 _00[0x4590];
    u8 field_0x4590;  // active flag
};

// Enemy record view for func_802B58A4's third sweep (byte at +0x6F4).
struct CErrMesEnemyObj {
    u8 _00[0x6F4];
    u8 field_0x6F4;  // active flag
};

// Owner object for func_802B48E4: battle-object-like layout with the embedded
// CfObjectMove at +0x3E9C (its address is passed to func_80081F28), a flag
// word at +0x3F00 (bit 1 = battle active) and a comparison word at +0x3F10.
struct CErrMesOwner {
    u8 _00[0x3E9C];             // 0x00..0x3E9B
    u8 field_0x3E9C;            // 0x3E9C - embedded CfObjectMove sub-object
    u8 _3E9D[0x3F00 - 0x3E9D];  // 0x3E9D..0x3EFF
    u32 field_0x3F00;           // 0x3F00 - flag word (bit 1 = battle active)
    u8 _3F04[0x3F10 - 0x3F04];  // 0x3F04..0x3F0F
    u32 field_0x3F10;           // 0x3F10 - comparison word
};

// CfObjectMove-like view (func_8016FE34 result / error-message owner): the
// embedded CObjectParam sub-object sits at +0x3E9C and its +0x4C virtual
// (CObjectParam_UnkVirtualFunc5) returns the actor/action id fed to
// func_800B708C.
struct CErrMesObjView {
    u8 _00[0x3E9C];
    cf::CObjectParam objectParam;  // +0x3E9C
};

// Phantom vtable view over the voice-handle vtable (vtable pointer at +0) so
// the voice-idle check at byte offset 0x2BC is dispatched as a real
// r12-chained virtual call (lwz r12,0(r3); lwz r12,0x2BC(r12); ...) matching
// retail. Retail data vtable, never constructed from C++ - only cast + call a
// slot, so no vtable is emitted and no method needs a definition.
struct CErrMesVoiceVtbl {
    virtual void f0();  virtual void f1();  virtual void f2();  virtual void f3();
    virtual void f4();  virtual void f5();  virtual void f6();  virtual void f7();
    virtual void f8();  virtual void f9();  virtual void f10(); virtual void f11();
    virtual void f12(); virtual void f13(); virtual void f14(); virtual void f15();
    virtual void f16(); virtual void f17(); virtual void f18(); virtual void f19();
    virtual void f20(); virtual void f21(); virtual void f22(); virtual void f23();
    virtual void f24(); virtual void f25(); virtual void f26(); virtual void f27();
    virtual void f28(); virtual void f29(); virtual void f30(); virtual void f31();
    virtual void f32(); virtual void f33(); virtual void f34(); virtual void f35();
    virtual void f36(); virtual void f37(); virtual void f38(); virtual void f39();
    virtual void f40(); virtual void f41(); virtual void f42(); virtual void f43();
    virtual void f44(); virtual void f45(); virtual void f46(); virtual void f47();
    virtual void f48(); virtual void f49(); virtual void f50(); virtual void f51();
    virtual void f52(); virtual void f53(); virtual void f54(); virtual void f55();
    virtual void f56(); virtual void f57(); virtual void f58(); virtual void f59();
    virtual void f60(); virtual void f61(); virtual void f62(); virtual void f63();
    virtual void f64(); virtual void f65(); virtual void f66(); virtual void f67();
    virtual void f68(); virtual void f69(); virtual void f70(); virtual void f71();
    virtual void f72(); virtual void f73(); virtual void f74(); virtual void f75();
    virtual void f76(); virtual void f77(); virtual void f78(); virtual void f79();
    virtual void f80(); virtual void f81(); virtual void f82(); virtual void f83();
    virtual void f84(); virtual void f85(); virtual void f86(); virtual void f87();
    virtual void f88(); virtual void f89(); virtual void f90(); virtual void f91();
    virtual void f92(); virtual void f93(); virtual void f94(); virtual void f95();
    virtual void f96(); virtual void f97(); virtual void f98(); virtual void f99();
    virtual void f100(); virtual void f101(); virtual void f102(); virtual void f103();
    virtual void f104(); virtual void f105(); virtual void f106(); virtual void f107();
    virtual void f108(); virtual void f109(); virtual void f110(); virtual void f111();
    virtual void f112(); virtual void f113(); virtual void f114(); virtual void f115();
    virtual void f116(); virtual void f117(); virtual void f118(); virtual void f119();
    virtual void f120(); virtual void f121(); virtual void f122(); virtual void f123();
    virtual void f124(); virtual void f125(); virtual void f126(); virtual void f127();
    virtual void f128(); virtual void f129(); virtual void f130(); virtual void f131();
    virtual void f132(); virtual void f133(); virtual void f134(); virtual void f135();
    virtual void f136(); virtual void f137(); virtual void f138(); virtual void f139();
    virtual void f140(); virtual void f141(); virtual void f142(); virtual void f143();
    virtual void f144(); virtual void f145(); virtual void f146(); virtual void f147();
    virtual void f148(); virtual void f149(); virtual void f150(); virtual void f151();
    virtual void f152(); virtual void f153(); virtual void f154(); virtual void f155();
    virtual void f156(); virtual void f157(); virtual void f158(); virtual void f159();
    virtual void f160(); virtual void f161(); virtual void f162(); virtual void f163();
    virtual void f164(); virtual void f165(); virtual void f166(); virtual void f167();
    virtual void f168(); virtual void f169(); virtual void f170(); virtual void f171();
    virtual void f172();
    virtual int isActive();  // slot 173 = 0x2BC - voice-idle check
};

// Voice/actor record base (func_8016FE34 result, e.g. func_802B5970's actor
// sources and func_802B5254's candidate): vtable at +0 (voice-idle check at
// 0x2BC via CErrMesVoiceVtbl::isActive), a sub-object at +4 (virtual at 0x30),
// and the embedded voice-owner interface at +0x3E9C (getPosition at 0xAC).
struct CErrMesArtsSubVtbl {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09();
    virtual void* v10();  // slot 10 = 0x30 - returns pointer read at [0]
};
struct CErrMesVoiceHandle : CErrMesVoiceVtbl {
    CErrMesArtsSubVtbl* unk4;   // 0x04
    u8 _08[0x3E9C - 0x08];      // 0x08..0x3E9B
    CVoiceOwnerIntf sub;        // 0x3E9C - embedded voice-owner interface
};

// d80 scene-object view (func_802B5254 arg4): position triple at +0x10C.
struct CErrMesD80 {
    u8 _00[0x10C];
    f32 field_0x10C;  // +0x10C x
    f32 field_0x110;  // +0x110 y
    f32 field_0x114;  // +0x114 z
};

// Voice-list record (func_802B5254 sweep 1): the list item points at the
// embedded +0x3E9C sub-object, so the record base is item - 0x3E9C; the
// active flag lives at +0x4590.
struct CErrMesVoiceRecord {
    u8 _00[0x3E9C];
    CVoiceOwnerIntf sub;         // +0x3E9C - embedded voice-owner interface
    u8 _3E9D[0x4590 - 0x3E9D];   // 0x3E9D..0x458F
    u8 field_0x4590;             // +0x4590 active flag
};

// Gimmick-record sub-object (func_802B5254 sweep 2): the +0x3E9C sub-object
// carries per-record capsule parameters at +0x63C / +0x640.
struct CErrMesGimmickSub : CVoiceOwnerIntf {
    u8 _04[0x63C - 0x04];
    f32 field_0x63C;  // +0x63C capsule radius
    f32 field_0x640;  // +0x640 capsule offset-y
};

// Gimmick record (func_802B5254 sweep 2): same embedding as the voice
// record; item points at the +0x3E9C sub-object.
struct CErrMesGimmickRecord {
    u8 _00[0x3E9C];
    CErrMesGimmickSub sub;       // +0x3E9C
    u8 _3E9D[0x4590 - 0x3E9D];
    u8 field_0x4590;             // +0x4590 active flag
};

// Enemy record (func_802B5254 sweep 3): the list item points at the record
// base; vtable at +0 (getPosition at 0xAC); active flag at +0x6F4.
struct CErrMesEnemyRec {
    CVoiceOwnerIntf sub;     // +0x00
    u8 _04[0x6F4 - 0x04];
    u8 field_0x6F4;          // +0x6F4 active flag
};

// Actor-source view (func_8016FE34(func_800B708C(...)) result): signed word
// at +0x15E4 (battle-state counter compared against 4).
struct CErrMesActor15E4 {
    u8 _00[0x15E4];
    s32 field_0x15E4;  // +0x15E4
};

// Ctor-view over the retail CUIErrMesWin region (0x00..0xA0): exposes the
// vtable pointer, the two null-ptmf callback slots and the scalar tail so the
// factory ctor can fill them without raw byte offsets (CSystemWindow idiom).
struct CErrMesWinCtorView {
    u8 _00[0x10];
    u32 mProcessVt;        // +0x10 - vtable pointer (interim, then final)
    u8 _14[0x3C - 0x14];   // +0x14..+0x3B - CProcess fields
    u32 ptmfMove[3];       // +0x3C - mMoveFunc (null ptmf)
    u32 ptmfDraw[3];       // +0x48 - mDrawFunc (null ptmf)
    u32 field_0x54;        // +0x54 - sub-vtable (final + 0x24)
    u32 field_0x58;        // +0x58 - sub-vtable (final + 0xAC)
    u32 field_0x5C;        // +0x5C - ctor arg1 (owning scene)
    u8 mSysWin[0x3C];      // +0x60 - CSysWin storage
    u8 field_0x9C;         // +0x9C - window state byte
    u8 _9D[1];             // +0x9D
    u16 field_0x9E;        // +0x9E - ctor arg2
    u8 field_0xA0;         // +0xA0 - ctor arg3
};

// C-ABI imports used by the error-window state machine (retail-unmangled
// names; C linkage stops MWCC appending a __F<argtypes> suffix).
extern "C" {
int CfRes_getD80Flag();
void* func_80496264(void* scene, int index);
void* func_8016FE34(void* source);
int func_802A77E8(void* handle);
void __ct__8CProcessFv(CProcess* self);
void __ct__CSysWin(void* syswin, int arg);
void* func_802A330C(int size, int align);
int func_802A3D54(CCharVoice* voicePtr, int voiceId, int arg);
}

// C++-mangled retail helper (actor id -> action source).
void* func_800B708C(int id);

// Player accessor (retail pre-mangled cf::CfGameManager static).
namespace cf { class CfObjectMove; }
extern "C" cf::CfObjectMove* getPlayer__Q22cf13CfGameManagerFi(int index);

// Null pointer-to-member-function constant + the CUIErrMesWin vtable regions
// (retail .data, written by the factory ctor).
extern u32 __ptmf_null[3];
extern char lbl_eu_8053AFFC[];   // interim (CTTask<CUIErrMesWin>) vtable
extern char lbl_eu_8053AF08[];   // final CUIErrMesWin composite vtable

// State-dispatch ptmf tables (retail .data). Indexed by the window state byte
// (field_0x9C / field_0xE8); each entry is a 12-byte ptmf.
class CUIErrMesWin;
class CMenuTitle;
extern void (CUIErrMesWin::*const lbl_eu_8053AEB0[])();
extern void (CMenuTitle::*const lbl_eu_8053B040[])();

// Minimal CScn declaration (same idiom as CTaskGameEff.hpp): only the
// render-callback members used by Init/Term. Guarded so the kyoshin headers
// can be included together in one TU. Member calls emit the retail mangled
// symbols addRenderCB__4CScnFP10IScnRenderUlUl / removeRenderCB__4CScnFP10IScnRender.
#ifndef KYOSHIN_MINIMAL_CSCN_DECLARED
#define KYOSHIN_MINIMAL_CSCN_DECLARED
class IScnRender;
class CScn {
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
};
#endif

// CProcess/CTask base (0x00-0x53): CDoubleListNode, vtable at +0x10, Move/Draw
// ptmf hooks at +0x3C/+0x48. The retail object is CTTask<CUIErrMesWin>-derived
// (retail emits Move__22CTTask<12CUIErrMesWin>Fv / Draw over the +0x3C/+0x48
// hooks). The render-callback subobject sits at +0x58 (IScnRender vtable slot)
// with the owning scene pointer at +0x5C (same arrangement as CSystemWindow).
class __declspec(novtable) CUIErrMesWin : public CTTask<CUIErrMesWin> {
public:
    virtual ~CUIErrMesWin();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    /* 0x54 */ u8 field_0x54[0x04];  // +0x54..+0x58 - unknown region
    /* 0x58 */ u32 mScnRender;       // +0x58 - IScnRender vtable slot (render-callback subobject)
    /* 0x5C */ CScn* mScene;         // +0x5C - owning scene
    /* 0x60 */ u8 mSysWin[0x3C];     // +0x60 - CSysWin storage. Raw buffer (CSystemWindow
                                    //          idiom): retail drives the ctor/dtor via the
                                    //          C-linkage __ct__CSysWin / __dt__7CSysWinFv,
                                    //          so a CSysWin member would add an implicit
                                    //          member-destruction chain to the dtor.
    /* 0x9C */ u8 field_0x9C;        // window state byte (2=ready, 4=active)
    /* 0x9D */ u8 _9D;
    /* 0x9E */ u16 field_0x9E;       // message id (string-pool index)
    /* 0xA0 */ u8 field_0xA0;        // window kind
};

class __declspec(novtable) CMenuTitle : public CTTask<CMenuTitle> {
public:
    virtual ~CMenuTitle();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    /* 0x54 */ u8 field_0x54[0x04];  // +0x54..+0x58 - unknown region
    /* 0x58 */ u32 mScnRender;       // +0x58 - IScnRender vtable slot (render-callback subobject)
    /* 0x5C */ CScn* mScene;         // +0x5C - owning scene
    /* 0x60 */ u8 field_0x60[0x88];  // +0x60..+0xE8 - opaque region (func_802B744C target)
    /* 0xE8 */ u8 field_0xE8;        // state byte (ptmf table index)
    /* 0xE9 */ u8 field_0xE9;        // cbRenderBefore gate byte (non-zero = draw title)
};
