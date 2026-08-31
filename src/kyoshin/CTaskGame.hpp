#pragma once

#include <types.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// Declared BEFORE the monolib includes: functions.hpp (pulled in via
// monolib/scn/CScnNw4r.hpp) declares func_8004302C as a plain C++ function,
// and a later extern "C" redeclaration is an MWCC error. Declaring the C
// linkage first makes the plain redeclaration inherit it, so the call sites
// in func_80040CD8 / func_80040EB4 emit the flat retail reloc.
extern "C" void func_8004302C(int a, int b);

#include "monolib/core.hpp"
#include "monolib/scn.hpp"
#include "monolib/util.hpp"
#include "monolib/work.hpp"
// Full HBM class (owner: monolib/lib/CLibHbm.hpp). The former static-only
// minimal copy here clashed with every TU whose include closure also pulls
// the full class (e.g. via monolib/lib.hpp); one winning decl lives on the
// owner header now.
#include "monolib/lib/CLibHbm.hpp"

// nw4r layout forward decls (DrawInfo raw-storage helpers below).
namespace nw4r {
namespace lyt {
class DrawInfo;
}
}

namespace cf {
class CfObject; // reslist<cf::CfObject*> instantiation in CTaskGame.cpp
class CfObjEnumList; // enum list holder target (CfObjectEnumList.hpp)
class CTaskGameCf; // task created by func_80042274 (create__Q22cf11CTaskGameCfFv)
}

// Object stored in the four virtual-delete slots at CTaskGame +0x174..+0x184
// (Term deletes each non-null entry through its vtable deleting-dtor slot).
// Declaration-only dtor: no vtable is emitted in this TU.
class CTaskGameSlotObj {
public:
    virtual ~CTaskGameSlotObj();
};

// CLibCri caller-shape imports: retail CLibCri.hpp declares these as no-arg
// members, but the retail call sites in this unit pass real arguments (the
// unkD8 CRI handle + volume), so declare the mangled symbols with the caller's
// shape (same scheme as cf/CTaskREvent.hpp). extern "C" keeps the retail
// verbatim symbol (C++ linkage would append an __F-arg mangling suffix).
extern "C" void setStreamVolume__7CLibCriFv(int handle, float volume);
extern "C" int getStreamPosition__7CLibCriFv(int handle);
extern "C" int stopStream__7CLibCriFv(int handle);
// CRI streaming-play volume/rate setter (4-arg form; same caller-shape scheme
// as the members above).
extern "C" void fadeStreamVolume__7CLibCriFv(s32 handle, float volume, float time, s32 mode);
// CRI handle-state query / active-set (caller-shape scheme as above; retail
// func_8004312C branches on the query result and forwards the booleanized OR
// of two gates as the active flag).
extern "C" int isStreamActive__7CLibCriFv(int handle);
extern "C" void setStreamPause__7CLibCriFv(int handle, bool active);
// Frame-time getter (CDeviceVI; retail verbatim name).
extern "C" f32 getSecPerFrame__9CDeviceVIFv();
// Target-frame-rate getter (CDeviceVI static; same flat name as
// CDeviceFileCri.hpp:108).
extern "C" u32 getTargetFramerate__9CDeviceVIFv();

// Mirror of the CTTask move-hook ptmf region (0x3C-0x48) for word-wise pool
// copies, so stores use named fields instead of raw offset arithmetic. The
// pad places the fields at the absolute offsets retail writes (self+0x3C...).
struct CTaskGamePtmfWords {
    u8 pad[0x3C];
    u32 field_0x3C;
    u32 field_0x40;
    u32 field_0x44;
};

class ITitleMenu{
public:
    virtual ~ITitleMenu(){}
    virtual void ITitleMenu__UnkVirtualFunc1() = 0;
};

class IErrMesWinSel{
public:
    virtual ~IErrMesWinSel(){}
    virtual void IErrMesWinSel__UnkVirtualFunc1() = 0;
};

struct UnkClass_8004041C{
    void func_8004041C(u8 r4, float f1, int r5, u32 r6, u8 r7, u32 r8, u32 r9);

    u8 unk0;
    float unk4;
    int unk8;
    u32 unkC;
    u8 unk10;
    u32 unk14;
    u32 unk18;
};

// Minimal CBattery view (members used by cbRenderBefore). The full
// CBattery.hpp pulls monolib/lib.hpp (revolution GX/HBM); only the used
// members are declared here.
class CBattery {
public:
    CBattery(u8 batteryLevel);
    void loadBatteryArchive();
    void updateLayout();
    void drawBattery(void*);
    void releaseLayout();
    void setBatteryLevel(u8 level);
    bool mLayoutReady; // 0x20
};


class CTaskGame : public CTTask<CTaskGame>,
                  public IWorkEvent,
                  public IScnRender,
                  public IGameException,
                  public ITitleMenu,
                  public IErrMesWinSel {
public:
    CTaskGame(CView* pView, CWorkThread* pThread, int r6);
    virtual ~CTaskGame();

    static CTaskGame* getInstance();
    static u32 isFlag2000Set(); // isUnk68Bit13Set: (spInstance->unk68 >> 13) & 1
    static int func_800426F0();
    virtual void Init();

    // Retail func_800436A8 compares the inherited CTTask move-hook ptmf against
    // a static ptmf pool entry (lbl_eu_80525850). Passed by value so MWCC
    // materializes the pool entry on the stack before __ptmf_cmpr.
    bool isMoveFunc(CTTask<CTaskGame>::MoveFunc f) const {
        return mMoveFunc == f;
    }

    // mMoveFunc is protected in CTTask; the pool-swap helpers assign the
    // whole 12-byte ptmf through this inline member.
    void setMoveFunc(CTTask<CTaskGame>::MoveFunc f) { mMoveFunc = f; }
    void setMoveFuncFrom(const CTTask<CTaskGame>::MoveFunc* src) { mMoveFunc = *src; }

    void setLoadingCaption(u16 r4, u16 r5, const char* r6, s16 r7);
    static bool isFlag01Set();
    void setInitFlag();
    void requestGameExit();
    static CTaskGame* create(CView* pView, CWorkThread* pThread, int r5);
    static s32 isMoveFuncActive();

    virtual void Term();
    void stub_80040A3C();
    void stub_80042720();
    void setFlag_200(bool enabled, unsigned int mode);
    void setFlag_400(int enabled, unsigned int mode, unsigned int value);
    void setFlag_100000(int enabled, int unused, unsigned int value);
    void setFlag_1000(int value);
    virtual void ITitleMenu__UnkVirtualFunc1();
    virtual void IErrMesWinSel__UnkVirtualFunc1();
    virtual bool gameExceptionCB(u32 r4);

    CScnNw4r* getScene() const {
        return unk74;
    }

    //0x0: vtable (CTTask)
    //0x4-3C: CProcess
    //0x3C-54: CTTask
    //0x54: vtable 2 (IWorkEvent)
    //0x58: vtable 3 (IScnRender)
    //0x5C: vtable 4 (IGameException)
    //0x60: vtable 5 (ITitleMenu)
    //0x64: vtable 6 (IErrMesWinSel)
    u32 unk68;
    CWorkThread* unk6C;
    CView* unk70;
    CScnNw4r* unk74;
    u32 unk78;
    u32 unk7C;
    u16 unk80;
    u16 unk82;
    s16 unk84;   // mission/save id; retail func_80042274 sign-extends it (lha)
    u16 unk86;
    u16 unk88;
    s16 unk8A;   // retail func_80042274 sign-extends it (lha) into the format vararg
    s16 unk8C;   // retail lha/sth/extsh (cbRenderBefore frame counter)
    s16 unk8E;
    u8 unk90;
    u8 unk91[0xA0 - 0x91];
    u32 unkA0;
    ml::FixStr<32> unkA4;
    u8 unkC8;
    u8 unkC9[0xCC - 0xC9];
    u32 unkCC;
    u32 unkD0;
    u32 unkD4;
    int unkD8;
    u32 unkDC;
    float unkE0;
    u32 unkE4;
    int unkE8;
    u32 unkEC;
    u32 unkF0;
    u32 unkF4;
    int unkF8;
    u32 unkFC;
    u32 unk100;
    u8 unk104;
    u8 unk105[0x124 - 0x105];
    u32 unk124;
    u32 unk128;
    u8 unk12C[0x130 - 0x12C];
    u8 unk130;
    u8 unk131[0x170 - 0x131];
    u32 unk170;
    CTaskGameSlotObj* unk174[5];
    u8 unk188;
    u8 unk189[0x18C - 0x189]; //padding?
    UnkClass_8004041C unk18C;

protected:
    CTaskGame();
    void cbRenderBefore();
    void OnFileEvent();
    void Draw();
};

// 12-byte static Move-hook ptmf pool entry compared against CTTask mMoveFunc by
// retail func_800436A8 (this unit's .data region 0x80525850).
extern CTTask<CTaskGame>::MoveFunc lbl_eu_80525850;

// Retail no-arg reset helper func_80043BC4 (DISCOVERY target us-80044160 in
// this unit); func_80043C88 calls it with the C-ABI flat name.
extern "C" void func_80043BC4();
// Retail per-mode cf::CTaskGameCf start helpers are FLAT C symbols
// (0x800448B8 / 0x800448D8, no member mangling); the instance returned by
// getInstance() flows through r3.
extern "C" void func_8004431C(cf::CTaskGameCf* instance);
extern "C" void func_8004433C(cf::CTaskGameCf* instance);
// Sound-state reset helper (flat retail name; defined in code_80187F14.cpp).
// Retail func_80043BC4 calls it behind the unk68 bit 0x80 gate.
extern "C" void func_80189C7C();

// NAND state-check helpers (flat retail names; defined in CfNandManager.cpp).
// Retail call sites (func_800417AC / func_80041658 / func_80041908) pass the
// address of the func_80043564 callback and branch on a non-zero result.
extern "C" int func_8023F690(void (*cb)(u32, u32, u32, u32));
extern "C" int func_8023FBA0(void (*cb)(u32, u32, u32, u32));
extern "C" int func_8023FCCC(void (*cb)(u32, u32, u32, u32));
// func_80041A48 passes just the callback; func_80041390 passes (unkFC, cb).
extern "C" int func_8023F5CC(void (*cb)(u32, u32, u32, u32));
extern "C" int func_8023F2F4(u32 value, void (*cb)(u32, u32, u32, u32));
// Address-taken callback passed to the NAND state checks above (flat retail
// name; stub body in this TU, DISCOVERY target us-80043adc). Retail
// dispatches (mode, a, b, c) to the per-mode helpers above by tail call.
extern "C" void func_80043564(u32 mode, u32 a, u32 b, u32 c);
// Window-state gate (flat retail name; stub body in this TU). Retail
// func_80040DE4 / func_80040C2C call it and branch on the result. Declared
// with the task pointer: retail func_80041BC0 explicitly re-passes self in
// r3 (mr r3, r30 before the call), which only a with-arg call emits.
extern "C" u32 func_80042784(CTaskGame* self);
// Menu-state gates (flat retail names; defined in CMenuSave.cpp /
// CMenuOption.cpp). func_80041F54 / func_80042048 branch on them.
extern "C" u32 func_8028E440();
extern "C" u32 func_8029BBA0();
// NAND state-check helpers (flat retail names; defined in CfNandManager.cpp).
// func_8023FC18 is called with the func_80043564 callback and branched on
// (func_800411A4); func_8023FD4C takes a single int mode.
extern "C" int func_8023FC18(void (*cb)(u32, u32, u32, u32));
extern "C" void func_8023FD4C(int mode);
// CScn empty-per-frame stub (flat retail name; 8-byte retail body). The
// retail call site in func_80040B38 passes (self, 0, vec4) with the flat
// verbatim reloc, so declare the caller's shape (CScnNw4r.hpp's member form
// would mangle an __FiP... suffix).
// NOTE: params are void* so this prototype is IDENTICAL to the one in
// cf/CTaskREvtSequence.hpp - two different extern "C" type lists for the same
// symbol make any TU that includes both headers fail with "illegal function
// overloading" (10197). Pointer params are ABI-identical either way.
extern "C" void func_8049602C(void* scene, int index, void* vec);
// Scene current-process query (flat retail name; defined in CfGameManager.cpp).
extern "C" void* func_80496034(CScn* scene);

// Hbm state toggle (flat retail names; defined in CLibHbm.cpp). Retail
// func_800411A4 disables via setHbmActiveFlag then re-enables via setHbmStopFlag.
extern "C" void func_eu_804521BC(int value);
// CTaskGameEvt event-helper import (retail flat name; stub body in
// CTaskGameEvt.cpp): setLoadingCaption passes the unkD4 object.
extern "C" void func_802956A8(void*);
// Per-mode dispatch helpers for the func_80043564 callback (flat retail
// names; stub bodies in this TU). Retail func_80043564 tail-calls them.
extern "C" void func_800433B0(CTaskGame* inst, u32 a, u32 b);
extern "C" void func_80043410(CTaskGame* inst, u32 a, u32 b, u32 c);
extern "C" void func_8004347C(CTaskGame* inst, u32 a, u32 b, u32 c);
extern "C" void func_800434AC(CTaskGame* inst, u32 a, u32 b, u32 c);
extern "C" void func_800434DC(CTaskGame* inst, u32 a, u32 b, u32 c);
extern "C" void func_80043538(CTaskGame* inst, u32 a);
// Default caption/title string data (.data); setLoadingCaption / func_8004256C
// index into it for the empty-caption fallback (offsets 0x6D / 0x8A).
extern char lbl_eu_804FA890[];

// Move-hook ptmf pool entries (.data): retail func_800419BC / func_80041B94
// copy the 12-byte CTTask mMoveFunc at +0x3C from these 3-word pools.
extern u32 lbl_eu_8052564C[3];
extern u32 lbl_eu_80525670[3];
extern u32 lbl_eu_8052567C[3];
extern u32 lbl_eu_805256A0[3];
extern u32 lbl_eu_805256AC[3];
extern u32 lbl_eu_805256B8[3];
extern u32 lbl_eu_805256C4[3];
extern u32 lbl_eu_805256E8[3];
extern u32 lbl_eu_805256F4[3];
extern u32 lbl_eu_80525700[3];
extern u32 lbl_eu_8052570C[3];
extern u32 lbl_eu_80525718[3];
extern u32 lbl_eu_80525724[3];
extern u32 lbl_eu_80525730[3];
extern u32 lbl_eu_8052573C[3];
extern u32 lbl_eu_80525778[3];
extern u32 lbl_eu_805257C0[3];
extern u32 lbl_eu_805257CC[3];
extern u32 lbl_eu_805257FC[3];
extern u32 lbl_eu_80525820[3];
extern u32 lbl_eu_80525598[3];
extern u32 lbl_eu_805255A4[3];
extern u32 lbl_eu_80525658[3];
extern u32 lbl_eu_80525664[3];
extern u32 lbl_eu_80525688[3];
extern u32 lbl_eu_80525694[3];
extern u32 lbl_eu_80525748[3];
extern u32 lbl_eu_80525754[3];
extern u32 lbl_eu_805256D0[3];
extern u32 lbl_eu_805256DC[3];
extern u32 lbl_eu_80525808[3];
extern u32 lbl_eu_80525814[3];
extern u32 lbl_eu_80525610[3];
extern u32 lbl_eu_8052561C[3];
extern u32 lbl_eu_80525838[3];
extern u32 lbl_eu_80525844[3];
extern u32 lbl_eu_8052558C[3];
extern u32 lbl_eu_805255B0[3];
extern u32 lbl_eu_805255BC[3];
extern u32 lbl_eu_805255C8[3];
// cf::CfGameManager controller-type query (flat retail name): the call site
// must load r3=-1 before the bl, so bind the caller's shape directly (the
// CfGameManager.hpp inline wrapper drops the argument).
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int arg);
extern u32 lbl_eu_80525760[3];
extern u32 lbl_eu_8052576C[3];
extern u32 lbl_eu_8052582C[3];
extern u32 lbl_eu_80525784[3];
extern u32 lbl_eu_805257D8[3];
extern u32 lbl_eu_805257E4[3];
extern u32 lbl_eu_805255F8[3];
extern u32 lbl_eu_80525604[3];
extern u32 lbl_eu_80525790[3];
extern u32 lbl_eu_805257F0[3];
// Base of the move-hook ptmf pool region holding pools at +0xC0/+0xCC/+0xD8
// (retail func_8004125C indexes it with addi from one materialized base).
extern u32 lbl_eu_80525568[];

// 8-byte enum-list holder (list pointer + MEM2 alloc handle) used by
// func_80043D90 / func_80043F18 / __dt__80043E88; other TUs declare their
// own local holder equivalents around these flat symbols.
struct CfEnumListHolder {
    cf::CfObjEnumList* list;   // +0x00
    u32 handle;                // +0x04
};

// Object-registry layout at CTaskGame +0x174: 4 object pointers (stride 4)
// each paired with a busy byte at +0x184 (stride 1); func_80043310 and
// func_8004335C scan it.
struct CTaskGameObjSlots {
    u8 pad[0x174];
    u32 objs[4];   // +0x174
    u8 busy[4];    // +0x184
};

// Field-offset view of the CfObjEnumList object. The shared
// CfObjectEnumList.hpp omits the polymorphic reslist base (vtable at +0x00),
// so its member offsets compile 4 low; this view matches the retail layout
// (mPtrCount at +0x620, mObjInfoCount at +0x3030).
struct CfObjEnumListFields {
    u8 pad[0x620];
    u32 mPtrCount;          // +0x620
    u8 pad2[0x3030 - 0x624];
    u32 mObjInfoCount;      // +0x3030
};

// CfObjEnumList ctor called with the raw allocated buffer; returns the object
// in r3 (retail reloc carries the recovered name __ct__cf_CfObjEnumList, not
// C++ mangling).
extern "C" cf::CfObjEnumList* __ct__cf_CfObjEnumList(cf::CfObjEnumList* self);
// Object-registry slot finder / releaser (retail flat names; defined in this TU).
extern "C" void* func_80043310();
extern "C" void func_8004335C(void* obj);

// Enum-list message-fill helper (flat retail name; defined in CUICfManager.cpp).
extern "C" void func_800F4A98(void*, u32, u32);

// Error-window factory ctor (retail stripped name; defined in CUIErrMesWin.cpp
// with the same C-ABI shape). extern "C": the call-site reloc must carry the
// verbatim retail name (a C++ declaration would mangle an __FP... suffix).
extern "C" void* __ct__802B4DF4(CProcess* parent, u32 a, u16 b, u8 c);

// Polymorphic view of the CfObjEnumList / reslist base for the virtual
// deleting-dtor dispatch in __dt__80043E88. MWCC lays vtables out as
// [typeinfo @0, toffset @4, virtuals @8..], so the dtor entry sits at +0x08
// and retail `delete list` dispatches through it with flag 1. A real virtual
// call forces MWCC's r12 dispatch (lwz r12,0(r3); lwz r12,8(r12); mtctr;
// bctrl) and the delete expansion supplies the redundant pointer test
// (retail emits two beq against one cmpwi). The shared CfObjectEnumList.hpp
// omits the polymorphic base, so the delete is expressed through this view;
// the dtor is never defined here (dispatched virtually at runtime).
struct CfObjEnumListVtView {
    virtual ~CfObjEnumListVtView();
};

// Error-message-window active flag (.sdata); same symbol as CUIErrMesWin.hpp.
extern u32 lbl_eu_80664C28;

// Unmangled callee imports for the func_80042630 reset sequence. The retail
// relocs carry these flat names; the defining TUs (CTaskGameEff.cpp /
// CfObjectSelectorObj.cpp / code_800B06A4.cpp) emit C-linkage symbols.
extern "C" u32 func_80044DF4();
extern "C" void func_800450C8();
extern "C" void* func_800FE68C();
extern "C" void __dt__800FDEF8(void* obj);
extern "C" void func_800B15A4(void* obj);
// Object-factory singleton accessor (C++ linkage -> retail getInstance__Fv).
extern void* getInstance();

// (definition moved above class CTaskGame; see CTaskGamePtmfWords)

// Three-word ptmf pool entry view (&lbl_eu_80525568 + offset); member access
// through this view makes MWCC materialize the entry address in a register
// and load w1/w2 relative to it (retail addi rX,base,off / lwz 4(rX) shape).
struct CTaskGamePtmfPool {
    u32 w0;
    u32 w1;
    u32 w2;
};

// C++ runtime delete / array-delete (flat retail names; used by the reslist
// base dtor).
extern "C" void __dl__FPv(void*);
extern "C" void __dla__FPv(void*);

// _reslist_base<cf::CfObject*> vtable (restored by its dtor
// __dt___reslist_base_cf_CfObject).
extern u8 lbl_eu_8052585C[];

// .sdata flag byte (func_80040C2C branches on it to pick the move-hook).
extern u8 lbl_eu_80663D2C;

// Object pointed to by CTaskGame::unkD0 / unkCC: a busy/active flag byte at
// +0x8C (func_80040C2C clears/sets it).
struct CTaskGameFlag8C {
    u8 pad[0x8C];
    u8 field_0x8C;   // +0x8C
};

// Raw view of the unk68 flag word for stores that must not CSE with earlier
// gate reads (func_80041448 reloads unk68 fresh before raising bit 0x2000).
struct CTaskGameFlags68 {
    u8 pad[0x68];
    volatile u32 flags;   // +0x68
};

// Float constant used by retail func_80040B38 as the 4th arg of the first
// vec4 setter call (lbl_eu_80665D6C, .sdata2; lbl_eu_80665D74 is declared in
// include/lbls_kyoshin.hpp).
extern const f32 lbl_eu_80665D6C;
extern const f32 lbl_eu_80665D74;

// Object pointed to by CTaskGame::unkD4: flag word at +0x60 (setLoadingCaption
// clears its bit 0x2).
struct CTaskGameUnkD4Obj {
    u8 pad[0x60];
    u32 field_0x60;   // +0x60
};

// FixStr<32> view of the caption buffer at CTaskGame +0x104 (mLength at
// +0x124); func_8004256C assigns the default caption through this view.
// The declared CTaskGame fields at 0x104/0x124 stay byte/u32 so the ctor
// init list is untouched.
struct CTaskGameCaptionStr {
    u8 pad[0x104];
    ml::FixStr<32> str;   // +0x104 (mLength at +0x124)
};

// FixStr<64> view of the caption buffer at CTaskGame +0x130 (mString at
// +0x130, mLength at +0x170); func_80043BC4 re-seeds it with the default
// caption (retail strlen+strcpy shape).
struct CTaskGameCaption130 {
    u8 pad[0x130];
    ml::FixStr<64> str;   // +0x130 (mLength at +0x170)
};

// Byte-flag view of the cf::CTaskGameCf object at +0x39 (inside the CProcess
// base; no named member in CTaskGameCf.hpp); func_8004256C sets it to 1.
struct CTaskGameCfFlag39 {
    u8 pad[0x39];
    u8 field_0x39;   // +0x39
};

// Byte-flag view of the objects pointed to by CTaskGame::unkCC / unkD0 /
// unkF0 (func_80041BC0 / func_8004213C set +0x39 to 1 when clearing/null-ing
// the pointers; the same busy-flag semantics as CTaskGameCfFlag39 but for the
// task's own object slots).
struct CTaskGameFlag39 {
    u8 pad[0x39];
    u8 field_0x39;   // +0x39
};

// Flat retail imports used by the func_80040CD8 / func_80041BC0 /
// func_8004213C / func_8004312C / func_80040EB4 call sites. extern "C" keeps
// the retail verbatim symbol (a plain C++ declaration would append an
// __F-arg mangling suffix at the call site, even for func_-prefixed names).
// Move-state gate (flat retail name; DISCOVERY stub in this TU): retail
// func_80040CD8 / func_80040EB4 call it with the task pointer and branch on
// the result.
extern "C" int func_8004362C(CTaskGame* self);
// Effect/particle reset helpers (flat retail names; defined in
// CfResPcImpl.cpp / code_804C8684.cpp). func_8004302C is declared before the
// monolib includes above (functions.hpp declares it plain C++).
extern "C" void func_804C8690(int a, int b);
// Battle/script-time gate (flat retail name; stub body in
// CTaskREvtSequence.cpp): retail func_8004312C ORs its result with
// isSceneLoading and forwards the boolean to the CRI active-setter.
extern "C" int func_8016C720();
// Vision resource-copy helper (flat retail name; defined in this TU). Retail
// func_8004312C passes the unk18C struct fields and branches on the result.
extern "C" int func_8004392C(u32 a, u32 b, u32 c, u32 d, u32 e, f32 f);
// Vision streaming-open helper (flat retail name; defined in this TU). Retail
// func_80041CC8 / func_8004392C call it out of line with the path/handle set.
extern "C" void func_80043738(u32 a1, const char* path, u32 a3, u32 a4, u32 a5, u32 a6, float volume);
// Play-time gate (flat retail name; defined in this TU).
extern "C" int func_80043D68();
// CRI volume/seek helper (flat retail name; defined in this TU).
extern "C" void func_80043B04(float volume);
// CTaskGameEvt event helper (flat retail name; stub body in CTaskGameEvt.cpp):
// func_80041BC0 passes the unkD4 object.
extern "C" void func_802956A4(void* obj);
// Window/error-message reset (flat retail name; declared in
// CMenuKizunagram.hpp): func_80041BC0 runs it before the title-menu ctor.
extern "C" void func_80042874();
// CRI stream volume/rate ramp (flat retail name; defined later in this TU
// with C linkage): retail func_8004213C calls it out of line with
// (framerate/2, volume constant) before the vec4 scene push.
extern "C" void func_80043CD8(int frames, float volume);
// Title-menu factory ctor (retail stripped name; stub body in CMenuTitle.cpp
// with the same C-ABI shape). extern "C": the call-site reloc must carry the
// verbatim retail name (a C++ declaration would mangle an __FP... suffix;
// same scheme as __ct__802B4DF4 above). The ITitleMenu* third parameter
// receives the +0x60 subobject via a null-checked static_cast at the call
// site (retail cmpwi/mr/beq/addi adjusted-this shape).
extern "C" void* __ct__CMenuTitle(CTaskGame* self, CScnNw4r* scene, ITitleMenu* titleMenu);

// Caller-shape imports for the func_80041CC8 / func_80043738 /
// func_8004392C / func_80042274 call sites (same scheme as the CLibCri
// caller-shape imports above). Flat retail names; the defining TUs emit
// C-linkage symbols.
extern "C" u32 func_80495FF0(CScn* scene);
extern "C" int func_801684F4();
extern "C" void func_80134C34();
extern "C" void func_80134E50(u32 value);
extern "C" u32 func_80496110(CScnNw4r* scene);
extern "C" int func_802A0CB8(char* buffer, int index, int value);
extern "C" int dispatchFilePlayback__7CLibCriFPCcUli(const char* filename, u32 allocHandle, int fileHandle);
extern "C" u32 calcStreamBufferSize__7CLibCriFv(int arg);
extern "C" int func_804DE010();
extern "C" int func_804DDD54(const char* ext, const char* path, u32* v0, u32* v1, u32* v2, u32* v3);
extern "C" int getFileSize__11CDeviceFileFPCc(const char* path, int arg1);
extern "C" void func_80189C70();
// Canonical C-linkage form, matching CDeviceFileCri.hpp's extern "C" block
// (int(const char*)); a single spelling so both headers coexist in one TU.
extern "C" int func_eu_804520D0(const char* str);
// cf::CTaskGameCf::create defined with the retail flat name (retail call
// sites pass the parent + 0; the defining TU emits the Fv symbol).
extern "C" cf::CTaskGameCf* create__Q22cf11CTaskGameCfFv(CProcess* pParent, int arg2);
// Scene object (alloc-handle source for func_80495FF0; same symbol as
// CTaskREvent.hpp).
// Global flag word raised to 0x10000000 by retail func_80042274 after the
// cf::CTaskGameCf create call.
extern u32 lbl_eu_80663E28;
// FixStr<32> append helpers (flat retail names; defined later in this TU
// with C linkage): retail func_80042274 appends the mission-caption suffix
// segments through them.
extern "C" char* func_80044070(ml::FixStr<32>* str, const char* s);
extern "C" char* func_800440C4(ml::FixStr<32>* str, const char* s);

// --- cbRenderBefore (IScnRender render-callback slot) imports ---
// The retail symbol cbRenderBefore__9CTaskGameFv is entered with r3 = this and
// r4 = scene (the CScn::Draw vt+0xC dispatch passes (cb, scn) and the retail
// vtable thunk subi r3,-0x58 / b cbRenderBefore__9CTaskGameFv preserves r4), so
// declare the caller's shape under extern "C" to keep the verbatim symbol.
extern "C" void cbRenderBefore__9CTaskGameFv(CTaskGame* self, CScn* scene);

// Camera/view object returned by func_8049603C (f32 at +0/+4/+8/+C).
struct CTaskGameCamView {
    f32 field_0;
    f32 field_4;
    f32 field_8;
    f32 field_C;
};
// func_8049603C is declared once in libs/monolib/src/scn/CScn_8049603C.hpp
// (owner: monolib CScn); include that header where the symbol is used.


// Loading-screen object (CLoad) helpers / gates (flat retail names; defined
// in CLoad.cpp / CfGameManager.cpp / menu TUs).
class CLoad;
class CBattery;
extern CLoad* lbl_eu_80663D1C;
extern CBattery* lbl_eu_80663D20;
extern u32 lbl_eu_80663D24;
extern u32 lbl_eu_80663D28;
extern u32 lbl_eu_80663D30;
extern u32 lbl_eu_80663D34;
extern u32 lbl_eu_806649F4;
extern const f32 lbl_eu_80665D78;

// CLoad helpers: declared by CLoad.hpp (proper C++ linkage).
// CLoad constructor with the stripped retail name (the C++ ctor emits a
// mangled symbol; retail call sites use the flat one). Declared returning
// CLoad* (PPC ctors return self in r3) so callers can chain the register
// value instead of keeping the local live across the call.
extern "C" CLoad* __ct__CLoad(CLoad* self, u8 arg);
// Scene render-callback removal + teardown helpers called by Term
// (retail verbatim symbols).
extern "C" void removeRenderCB__4CScnFP10IScnRender(CScn* scn, IScnRender* cb);
extern "C" void __dt__8009D72C();
extern "C" void __dt__8047BFFC();
extern "C" void releasePoolMemory__17UnkClass_8047CD0CFv();

// --- CTaskGame::Init factory imports (flat retail names; defined in the
// monolib / cf / kyoshin TUs with C linkage) ---
class CTaskGameEvt;
class CTaskGamePic;
// Scene async-load kick + resource-flag setter.
extern "C" void func_80496118(CScn* scn, CWorkThread* work, u32 flag);
// Scene factory (defined in monolib with the flat retail symbol).
extern "C" CScnNw4r* create__8CScnNw4rFv(CProcess* parent, char* name,
                                         u32 arg1, u32 handle, void* param);
extern "C" void CfRes_setD80Flag(void* scene);
// Global actor-param work-buffer builder (defined in CtrlObjectParam.cpp).
extern "C" void __ct__8009D604();
// Effect-task factory (defined in CTaskGameEff.cpp with the flat name).
extern "C" void func_800450CC(CProcess* parent, CScn* scene);
extern "C" void* create__8CTaskLODFv(void* parent, void* p1, void* p2, u32 handle,
                                     u32 size);
extern "C" void* create__16CTaskColiManagerFv(CProcess* parent, void* scene,
                                               void* view);
extern "C" void __dt__8047BFA8();
extern "C" void refreshPoolMemory__17UnkClass_8047CD0CFv();
extern "C" void* create__Q22cf13CfNandManagerFv(CProcess* parent, void* scene);
extern "C" void addRenderCB__4CScnFP10IScnRenderUlUl(void* scn, void* cb, u32 a,
                                                      u32 b);
extern "C" u8 getLanguage__9CDeviceSCFv();
extern "C" void func_80294EC0(CTaskGamePic* self, const char* path);
extern "C" CTaskGamePic* create__12CTaskGamePicFv(CProcess* parent, int arg);
extern "C" CTaskGameEvt* create__12CTaskGameEvtFv(CProcess* parent, int arg);
// Scene-create name pointer (.sdata; points into .sdata2 string space).
extern char* lbl_eu_80661908;
extern const f32 lbl_eu_80665D70;
// Move-hook ptmf pool entry used by Init (lbl_eu_80525568 + 0xC).
extern u32 lbl_eu_80525574[3];
extern "C" int func_802B0D10();
extern "C" int func_800FF738();
extern "C" bool CMenuArtsSelect_isCreated();
// cf::CfPadTask::getWiimoteBattery() static (retail verbatim mangle).
// Signed return: retail call sites compare with cmpwi/ble (battery > 1),
// which MWCC only emits for a signed type.
extern "C" int getWiimoteBattery__Q22cf9CfPadTaskFv();

// nw4r DrawInfo raw-storage helpers (retail pre-mangled names; same scheme
// as CQuestWindow.hpp).
void __ct__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* self);
void __dt__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* self, int flags);
void func_80137250(nw4r::lyt::DrawInfo* drawInfo);

// Virtual-delete views for the loading screen / battery objects (vtable dtor
// at +8, flag 1); same scheme as CfObjEnumListVtView.
struct CLoadVtView {
    virtual ~CLoadVtView();
};
struct CBatteryVtView {
    virtual ~CBatteryVtView();
};
