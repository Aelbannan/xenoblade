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
extern "C" void CTitle_update(void*);

// CSysWin advance helper (retail C-linkage; same declaration as CSystemWindow.hpp).
extern "C" void func_8022B748(void*);

// CSysWin content setters / advance-close helpers (retail unmangled names,
// defined in CSysWin.cpp). func_8022BFC8 is declared by CSysWin.hpp.
extern "C" void func_8022B9B4(void* syswin, const char* msg, int flag);
extern "C" void func_8022B8B8(void* syswin);
extern "C" void func_8022B8E4(void* syswin);

// Pane-name format helper (code_80135FDC.cpp, retail unmangled name).
extern "C" char* BdatTouchStringCell(char* fmt, char* base, u32 id);

// String pool used by func_802B4F40's error message.
extern char lbl_eu_805135E0[];

// cf::CfGameManager current-pad accessor (retail pre-mangled static).
void* getCurrentPad__Q22cf13CfGameManagerFv();

// Pad view for func_802B4FE8: button word at +4.
struct CErrMesPad {
    u8 _00[4];
    u32 field_0x4;
};


extern "C" void func_8022B7F4(void*);

// CMenuTitle +0x60 sub-object advance/release helpers (retail C-linkage, CTitle.cpp).
extern "C" void CTitle_startLoad(void*);
extern "C" void CTitle_teardown(void*);

// cf::CfGameManager helper. Retail names it with the no-arg Fv suffix, but the
// call site passes three zero args; declare the retail symbol verbatim under
// C linkage so calls bind to it (same pattern as isClassicController in CSysWin.hpp).
extern "C" void lookupEffectForResource__Q22cf13CfGameManagerFv(int, int, int);

// cf::CfGameManager error-message helper (func_802B48E4 dispatches 0x29/0x24).
// Retail ships the no-arg-Fv mangled name verbatim; keep it under C linkage so
// the call site binds to the retail symbol (same pattern as lookupEffectForResource).
extern "C" void createBattleEffect__Q22cf13CfGameManagerFv(u32, u8*);

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
extern "C" void CTitle_draw(u8* self, nw4r::lyt::DrawInfo* drawInfo);

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
// returned by getListB28__Fv / getReslistC08 share the same shape: the
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

// Voice/actor-manager list accessor (retail mangled name getListB28__Fv -
// plain C++ linkage reproduces the Fv suffix).
CErrMesList* getListB28();

// Enemy-manager list accessor (retail unmangled C symbol).
extern "C" CErrMesList* getReslistC08();

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
// CfObjectMove at +0x3E9C (its address is passed to createBattleEffect), a flag
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
// (CObjectParam_getSelfObjectId) returns the actor/action id fed to
// findObjectById.
struct CErrMesObjView {
    u8 _00[0x3E9C];
    cf::CObjectParam objectParam;  // +0x3E9C
};

// Voice/actor record base (func_8016FE34 result, e.g. func_802B5970's actor
// sources and func_802B5254's candidate): vtable at +0 is the real actor
// table (cf::CActorParam lineage; the voice-idle check is slot 138 at 0x2BC
// via CActorParam_isBattleLocked, same as CVS_THREAD_HAGE / CBattleManager),
// +0x4 is the actor-state sub-object (slot 0x30 via cf::CHelpBattleSub4::vf30
// from CTitle.hpp), and +0x3E9C is the embedded voice-owner interface
// (getPosition at 0xAC). Plain data - the pad vtables are gone.
struct CErrMesVoiceHandle {
    void* vtable;               // 0x00 - real actor vtable
    cf::CHelpBattleSub4* unk4;  // 0x04 - actor-state sub-object
    u8 _08[0x3E9C - 0x08];      // 0x08..0x3E9B
    CVoiceOwnerIntf sub;        // 0x3E9C - embedded voice-owner interface
};

// d80 scene-object view
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

// Actor-source view (func_8016FE34(findObjectById(...)) result): signed word
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
void* Scn_FindCamItem(void* scene, int index);
void* func_8016FE34(void* source);
int func_802A77E8(void* handle);
void __ct__8CProcessFv(CProcess* self);
void __ct__CSysWin(void* syswin, int arg);
void* func_802A330C(int size, int align);
int func_802A3D54(CCharVoice* voicePtr, int voiceId, int arg);
}

// C++-mangled retail helper (actor id -> action source).
void* findObjectById(int id);

// Player accessor (retail pre-mangled cf::CfGameManager static). Canonical
// void* form (matches CfObjectMove.hpp / CfObjectImplMove.hpp / CtrlEnemy.hpp
// and every other TU); the CfObjectMove* spelling is an illegal overload
// when both are visible in one TU.
extern "C" void* getPlayer__Q22cf13CfGameManagerFi(int index);

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
    /* 0x60 */ u8 field_0x60[0x88];  // +0x60..+0xE8 - opaque region (CTitle_update target)
    /* 0xE8 */ u8 field_0xE8;        // state byte (ptmf table index)
    /* 0xE9 */ u8 field_0xE9;        // cbRenderBefore gate byte (non-zero = draw title)
};
