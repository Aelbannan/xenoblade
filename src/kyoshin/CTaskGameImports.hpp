#pragma once

// CTaskGame TU-private imports: retail flat-name callees, .data/.sdata labels and
// layout views used by CTaskGame.cpp. The class itself is in kyoshin/CTaskGame.hpp.

#include <types.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)
#include "kyoshin/code_80135FDC_Lyt.hpp"

// Declared BEFORE the monolib includes: functions.hpp (pulled in via
// monolib/scn/CScnNw4r.hpp) declares CTaskGame_setLoadScreen as a plain C++ function,
// and a later extern "C" redeclaration is an MWCC error. Declaring the C
// linkage first makes the plain redeclaration inherit it, so the call sites
// in CTaskGame_moveFadeTick / CTaskGame_moveWaitPad emit the flat retail reloc.
extern "C" void CTaskGame_setLoadScreen(int a, int b);

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
class CTaskGameCf; // task created by CTaskGame_moveStartMission (create__Q22cf11CTaskGameCfFv)
}


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
// CTaskGame_updateStream branches on the query result and forwards the booleanized OR
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
    u8 pad[0x3C]; /* 0x00 */
    u32 field_0x3C; /* 0x3C */
    u32 field_0x40; /* 0x40 */
    u32 field_0x44; /* 0x44 */
};


#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/CUICfManager.hpp"


// 12-byte static Move-hook ptmf pool entry compared against CTTask mMoveFunc by
// retail CTaskGame_isMoveHook (this unit's .data region 0x80525850).
extern CTTask<CTaskGame>::MoveFunc lbl_eu_80525850;

// Retail no-arg reset helper CTaskGame_resetStream (DISCOVERY target us-80044160 in
// this unit); CTaskGame_stopVision calls it with the C-ABI flat name.
extern "C" void CTaskGame_resetStream();
// cf::CTaskGameCf::startMode1New is a real member (declared in
// kyoshin/cf/CTaskGameCf.hpp).
extern "C" void startMode2Con(cf::CTaskGameCf* instance);
// Sound-state reset helper (flat retail name; defined in code_80187F14.cpp).
// Retail CTaskGame_resetStream calls it behind the unk68 bit 0x80 gate.

// NAND state-check helpers (flat retail names; defined in CfNandManager.cpp).
// Retail call sites (CTaskGame_moveNandChk690 / CTaskGame_moveNandChkBA0 / CTaskGame_moveNandChkCCC) pass the
// address of the CTaskGame_nandCallback callback and branch on a non-zero result.
extern "C" int NandQueueChk690(void (*cb)(u32, u32, u32, u32));
extern "C" int NandQueueChkBA0(void (*cb)(u32, u32, u32, u32));
extern "C" int NandQueueChkCCC(void (*cb)(u32, u32, u32, u32));
// CTaskGame_moveNandChk5CC passes just the callback; CTaskGame_moveNandChkFc passes (unkFC, cb).
extern "C" int NandQueueChk5CC(void (*cb)(u32, u32, u32, u32));
extern "C" int NandQueueChkFc(u32 value, void (*cb)(u32, u32, u32, u32));
// Address-taken callback passed to the NAND state checks above (flat retail
// name; stub body in this TU, DISCOVERY target us-80043adc). Retail
// dispatches (mode, a, b, c) to the per-mode helpers above by tail call.
extern "C" void CTaskGame_nandCallback(u32 mode, u32 a, u32 b, u32 c);
// Window-state gate (flat retail name; stub body in this TU). Retail
// CTaskGame_movePrepDual / CTaskGame_movePrepFade call it and branch on the result. Declared
// with the task pointer: retail CTaskGame_moveShowTitle explicitly re-passes self in
// r3 (mr r3, r30 before the call), which only a with-arg call emits.
extern "C" u32 CTaskGame_windowGate(CTaskGame* self);
// Menu-state gates (flat retail names; defined in CMenuSave.cpp /
// CMenuOption.cpp). CTaskGame_moveAfterSave / CTaskGame_moveAfterOpt branch on them.
extern "C" u32 isSaveMenuActive();
extern "C" u32 hasOptionMenu();
// NAND state-check helpers (flat retail names; defined in CfNandManager.cpp).
// NandQueueReset is called with the CTaskGame_nandCallback callback and branched on
// (CTaskGame_moveNandReset); NandResetMode takes a single int mode.
extern "C" int NandQueueReset(void (*cb)(u32, u32, u32, u32));
extern "C" void NandResetMode(int mode);
// CScn empty-per-frame stub (flat retail name; 8-byte retail body). The
// retail call site in CTaskGame_moveFadeOut passes (self, 0, vec4) with the flat
// verbatim reloc, so declare the caller's shape (CScnNw4r.hpp's member form
// would mangle an __FiP... suffix).
// NOTE: params are void* so this prototype is IDENTICAL to the one in
// cf/CTaskREvtSequence.hpp - two different extern "C" type lists for the same
// symbol make any TU that includes both headers fail with "illegal function
// overloading" (10197). Pointer params are ABI-identical either way.
extern "C" void Scn_ReleaseUnk80(void* scene, int index, void* vec);
// Scene current-process query (flat retail name; defined in CfGameManager.cpp).
extern "C" void* Scn_GetUnk80Handle(CScn* scene);

// Hbm state toggle (flat retail names; defined in CLibHbm.cpp). Retail
// CTaskGame_moveNandReset disables via setHbmActiveFlag then re-enables via setHbmStopFlag.
extern "C" void DevFile_SetByteA9(int value);
// Per-mode dispatch helpers for the CTaskGame_nandCallback callback (flat retail
// names; stub bodies in this TU). Retail CTaskGame_nandCallback tail-calls them.
extern "C" void CTaskGame_setFlag200(CTaskGame* inst, u32 a, u32 b);
extern "C" void CTaskGame_setFlag400(CTaskGame* inst, u32 a, u32 b, u32 c);
extern "C" void CTaskGame_setNandFlag80000(CTaskGame* inst, u32 a, u32 b, u32 c);
extern "C" void CTaskGame_setNandFlag800(CTaskGame* inst, u32 a, u32 b, u32 c);
extern "C" void CTaskGame_setFlag100000(CTaskGame* inst, u32 a, u32 b, u32 c);
extern "C" void CTaskGame_setFlag1000000(CTaskGame* inst, u32 a);
// Default caption/title string data (.data); setLoadingCaption / CTaskGame_moveSetCaption
// index into it for the empty-caption fallback (offsets 0x6D / 0x8A).
extern char lbl_eu_804FA890[];

// Move-hook ptmf pool entries (.data): retail CTaskGame_moveHook8M / CTaskGame_moveClrErrWin2
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
// (retail CTaskGame_moveSelectErrWin indexes it with addi from one materialized base).
extern u32 lbl_eu_80525568[];

#include "kyoshin/CfEnumListHolder.hpp"

// Object-registry layout at CTaskGame +0x174: 4 object pointers (stride 4)
// each paired with a busy byte at +0x184 (stride 1); CTaskGame_allocObjSlot and
// CTaskGame_freeObjSlot scan it.
struct CTaskGameObjSlots {
    u8 pad[0x174]; /* 0x00 */
    u32 objs[4];   // +0x174 /* 0x174 */
    u8 busy[4];    // +0x184 /* 0x184 */
};

// Field-offset view of the CfObjEnumList object. The shared
// CfObjectEnumList.hpp omits the polymorphic reslist base (vtable at +0x00),
// so its member offsets compile 4 low; this view matches the retail layout
// (mPtrCount at +0x620, mObjInfoCount at +0x3030).
struct CfObjEnumListFields {
    u8 pad[0x620]; /* 0x00 */
    u32 mPtrCount;          // +0x620 /* 0x620 */
    u8 pad2[0x3030 - 0x624]; /* 0x624 */
    u32 mObjInfoCount;      // +0x3030 /* 0x3030 */
};

// CfObjEnumList ctor called with the raw allocated buffer; returns the object
// in r3 (retail reloc carries the recovered name __ct__cf_CfObjEnumList, not
// C++ mangling).
extern "C" cf::CfObjEnumList* __ct__cf_CfObjEnumList(cf::CfObjEnumList* self);
// Object-registry slot finder / releaser (retail flat names; defined in this TU).
extern "C" void* CTaskGame_allocObjSlot();
extern "C" void CTaskGame_freeObjSlot(void* obj);

// Enum-list message-fill helper (flat retail name; defined in CUICfManager.cpp).
extern "C" void startEnumObjects(void*, u32, u32);

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

// Unmangled callee imports for the CTaskGame_moveResetFx reset sequence. The retail
// relocs carry these flat names; the defining TUs (CTaskGameEff.cpp /
// CfObjectSelectorObj.cpp / code_800B06A4.cpp) emit C-linkage symbols.
extern "C" u32 getEffectTask();
extern "C" void noopGameEffTask();
extern "C" void __dt__800FDEF8(void* obj);
extern "C" void teardownGameMgr(void* obj);
// Object-factory singleton accessor (C++ linkage -> retail getInstance__Fv).
class UnkClass_805764CC;
extern UnkClass_805764CC* getInstance();

// (definition moved above class CTaskGame; see CTaskGamePtmfWords)

// Three-word ptmf pool entry view (&lbl_eu_80525568 + offset); member access
// through this view makes MWCC materialize the entry address in a register
// and load w1/w2 relative to it (retail addi rX,base,off / lwz 4(rX) shape).
struct CTaskGamePtmfPool {
    u32 w0; /* 0x00 */
    u32 w1; /* 0x04 */
    u32 w2; /* 0x08 */
};

// C++ runtime delete / array-delete (flat retail names; used by the reslist
// base dtor).

// _reslist_base<cf::CfObject*> vtable (restored by its dtor
// __dt___reslist_base_cf_CfObject).
extern u8 lbl_eu_8052585C[];

// .sdata flag byte (CTaskGame_movePrepFade branches on it to pick the move-hook).
extern u8 gMoveHookFlag;

// Object pointed to by CTaskGame::unkD0 / unkCC: a busy/active flag byte at
// +0x8C (CTaskGame_movePrepFade clears/sets it).
struct CTaskGameFlag8C {
    u8 pad[0x8C]; /* 0x00 */
    u8 field_0x8C;   // +0x8C /* 0x8C */
};

// Raw view of the unk68 flag word for stores that must not CSE with earlier
// gate reads (CTaskGame_moveErrWin56 reloads unk68 fresh before raising bit 0x2000).
struct CTaskGameFlags68 {
    u8 pad[0x68]; /* 0x00 */
    volatile u32 flags;   // +0x68 /* 0x68 */
};

// Float constant used by retail CTaskGame_moveFadeOut as the 4th arg of the first
// vec4 setter call (lbl_eu_80665D6C, .sdata2); the shared zero argument is a
// source 0.0f literal.
extern const f32 lbl_eu_80665D6C;

// FixStr<32> view of the caption buffer at CTaskGame +0x104 (mLength at
// +0x124); CTaskGame_moveSetCaption assigns the default caption through this view.
// The declared CTaskGame fields at 0x104/0x124 stay byte/u32 so the ctor
// init list is untouched.
struct CTaskGameCaptionStr {
    u8 pad[0x104]; /* 0x00 */
    ml::FixStr<32> str;   // +0x104 (mLength at +0x124) /* 0x104 */
};

// FixStr<64> view of the caption buffer at CTaskGame +0x130 (mString at
// +0x130, mLength at +0x170); CTaskGame_resetStream re-seeds it with the default
// caption (retail strlen+strcpy shape).
struct CTaskGameCaption130 {
    u8 pad[0x130]; /* 0x00 */
    ml::FixStr<64> str;   // +0x130 (mLength at +0x170) /* 0x130 */
};

// Byte-flag view of the cf::CTaskGameCf object at +0x39 (inside the CProcess
// base; no named member in CTaskGameCf.hpp); CTaskGame_moveSetCaption sets it to 1.
struct CTaskGameCfFlag39 {
    u8 pad[0x39]; /* 0x00 */
    u8 field_0x39;   // +0x39 /* 0x39 */
};

// Byte-flag view of the objects pointed to by CTaskGame::unkCC / unkD0 /
// unkF0 (CTaskGame_moveShowTitle / CTaskGame_moveExitFade set +0x39 to 1 when clearing/null-ing
// the pointers; the same busy-flag semantics as CTaskGameCfFlag39 but for the
// task's own object slots).
struct CTaskGameFlag39 {
    u8 pad[0x39]; /* 0x00 */
    u8 field_0x39;   // +0x39 /* 0x39 */
};

// Flat retail imports used by the CTaskGame_moveFadeTick / CTaskGame_moveShowTitle /
// CTaskGame_moveExitFade / CTaskGame_updateStream / CTaskGame_moveWaitPad call sites. extern "C" keeps
// the retail verbatim symbol (a plain C++ declaration would append an
// __F-arg mangling suffix at the call site, even for func_-prefixed names).
// Move-state gate (flat retail name; DISCOVERY stub in this TU): retail
// CTaskGame_moveFadeTick / CTaskGame_moveWaitPad call it with the task pointer and branch on
// the result.
extern "C" int CTaskGame_padConfirm(CTaskGame* self);
// Effect/particle reset helpers (flat retail names; defined in
// CfResPcImpl.cpp / code_804C8684.cpp). CTaskGame_setLoadScreen is declared before the
// monolib includes above (functions.hpp declares it plain C++).
extern "C" void EffCtl_StoreFlagAndParams(int a, int b);
// Battle/script-time gate (flat retail name; stub body in
// CTaskREvtSequence.cpp): retail CTaskGame_updateStream ORs its result with
// isSceneLoading and forwards the boolean to the CRI active-setter.
// Vision resource-copy helper (flat retail name; defined in this TU). Retail
// CTaskGame_updateStream passes the unk18C struct fields and branches on the result.
extern "C" int CTaskGame_openVision(u32 a, u32 b, u32 c, u32 d, u32 e, f32 f);
// Vision streaming-open helper (flat retail name; defined in this TU). Retail
// CTaskGame_moveTitleTick / CTaskGame_openVision call it out of line with the path/handle set.
extern "C" void CTaskGame_openStream(u32 a1, const char* path, u32 a3, u32 a4, u32 a5, u32 a6, float volume);
// Play-time gate (flat retail name; defined in this TU).
extern "C" int CTaskGame_playTimeGate();
// CRI volume/seek helper (flat retail name; defined in this TU).
extern "C" void CTaskGame_setStreamVol(float volume);
// Window/error-message reset (flat retail name; declared in
// CMenuKizunagram.hpp): CTaskGame_moveShowTitle runs it before the title-menu ctor.
extern "C" void CTaskGame_deleteLoad();
// CRI stream volume/rate ramp (flat retail name; defined later in this TU
// with C linkage): retail CTaskGame_moveExitFade calls it out of line with
// (framerate/2, volume constant) before the vec4 scene push.
extern "C" void CTaskGame_fadeStream(int frames, float volume);
// Caller-shape imports for the CTaskGame_moveTitleTick / CTaskGame_openStream /
// CTaskGame_openVision / CTaskGame_moveStartMission call sites (same scheme as the CLibCri
// caller-shape imports above). Flat retail names; the defining TUs emit
// C-linkage symbols.
extern "C" void CUICfManager_openSaveMenu();
extern "C" u32 Scn_GetUnk80Resource(CScnNw4r* scene);
extern "C" int formatVoiceFilePath(char* buffer, int index, int value);
extern "C" int dispatchFilePlayback__7CLibCriFPCcUli(const char* filename, u32 allocHandle, int fileHandle);
extern "C" u32 calcStreamBufferSize__7CLibCriFv(int arg);
// Defined in monolib/src/work/CWorkSystemPack.cpp with C++ linkage.
int hasPackFile(const char* pName);
extern "C" int func_804DDD54(const char* ext, const char* path, u32* v0, u32* v1, u32* v2, u32* v3);
extern "C" int getFileSize__11CDeviceFileFPCc(const char* path, int arg1);
// Defined in monolib/src/device/CDeviceFile.cpp with C++ linkage.
void DevFile_SubstLangPath(char* str);
// cf::CTaskGameCf::create defined with the retail flat name (retail call
// sites pass the parent + 0; the defining TU emits the Fv symbol).
extern "C" cf::CTaskGameCf* create__Q22cf11CTaskGameCfFv(CProcess* pParent, int arg2);
// Scene object (alloc-handle source for CScn::getAllocHandle; same symbol as
// CTaskREvent.hpp).
// Global flag word raised to 0x10000000 by retail CTaskGame_moveStartMission after the
// cf::CTaskGameCf create call.
extern u32 gmPauseFlags;
// FixStr<32> append helpers (flat retail names; defined later in this TU
// with C linkage): retail CTaskGame_moveStartMission appends the mission-caption suffix
// segments through them.
extern "C" char* CTaskGame_strAppend(ml::FixStr<32>* str, const char* s);
extern "C" char* CTaskGame_strAppend2(ml::FixStr<32>* str, const char* s);

// --- cbRenderBefore (IScnRender render-callback slot) imports ---
// The retail symbol cbRenderBefore__9CTaskGameFv is entered with r3 = this and
// r4 = scene (the CScn::Draw vt+0xC dispatch passes (cb, scn) and the retail
// vtable thunk subi r3,-0x58 / b cbRenderBefore__9CTaskGameFv preserves r4), so
// declare the caller's shape under extern "C" to keep the verbatim symbol.
extern "C" void cbRenderBefore__9CTaskGameFv(CTaskGame* self, CScn* scene);

// Camera/view object returned by Scn_QueryUnk80State (f32 at +0/+4/+8/+C).
struct CTaskGameCamView {
    f32 field_0; /* 0x00 */
    f32 field_4; /* 0x04 */
    f32 field_8; /* 0x08 */
    f32 field_C; /* 0x0C */
};
// Scn_QueryUnk80State is declared once in libs/monolib/src/scn/CScn_8049603C.hpp
// (owner: monolib CScn); include that header where the symbol is used.


// Loading-screen object (CLoad) helpers / gates (flat retail names; defined
// in CLoad.cpp / CfGameManager.cpp / menu TUs).
class CLoad;
class CBattery;
extern CLoad* gLoadScreen;
extern CBattery* gBatteryDisplay;
extern u32 gLoadStep;
extern u32 gLoadIdleCounter;
extern u32 gLoadTimeoutCounter;
extern u32 gLoadDelayCounter;
extern u32 gCSaveLoadInstance;
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
extern "C" void Scn_GetCamWorkInt(CScn* scn, CWorkThread* work, u32 flag);
// Scene factory (defined in monolib with the flat retail symbol).
extern "C" CScnNw4r* create__8CScnNw4rFv(CProcess* parent, char* name,
                                         u32 arg1, u32 handle, void* param);
extern "C" u32 CfRes_setD80Flag(u32 val);
// Global actor-param work-buffer builder (defined in CtrlObjectParam.cpp).
extern "C" void __ct__8009D604();
// Effect-task factory (defined in CTaskGameEff.cpp with the flat name).
extern "C" void createGameEffTask(CProcess* parent, CScn* scene);
extern "C" void* create__8CTaskLODFv(void* parent, void* p1, void* p2, u32 handle,
                                     u32 size);
extern "C" void* create__16CTaskColiManagerFv(CProcess* parent, void* scene,
                                               void* view);
extern "C" void __dt__8047BFA8();
extern "C" void refreshPoolMemory__17UnkClass_8047CD0CFv();
extern "C" void* create__Q22cf13CfNandManagerFv(CProcess* parent, void* scene);
extern "C" void addRenderCB__4CScnFP10IScnRenderUlUl(void* scn, void* cb, u32 a,
                                                      u32 b);
extern "C" void GamePicStartFileLoad(CTaskGamePic* self, const char* path);
extern "C" CTaskGamePic* create__12CTaskGamePicFv(CProcess* parent, int arg);
// Scene-create name pointer (.sdata; points into .sdata2 string space).
extern char* lbl_eu_80661908;
extern const f32 lbl_eu_80665D70;
// Move-hook ptmf pool entry used by Init (lbl_eu_80525568 + 0xC).
extern u32 lbl_eu_80525574[3];
extern "C" int CMenuGCItem_IsActive();
extern "C" int CMainMenu_IsOpen();
extern "C" bool CMenuArtsSelect_isCreated();
// cf::CfPadTask::getWiimoteBattery() static (retail verbatim mangle).
// Signed return: retail call sites compare with cmpwi/ble (battery > 1),
// which MWCC only emits for a signed type.
extern "C" int getWiimoteBattery__Q22cf9CfPadTaskFv();

// nw4r DrawInfo raw-storage helpers (retail pre-mangled names; same scheme
// as CQuestWindow.hpp).

// CLoad derives from IWorkEvent with a real virtual destructor; its deletes
// dispatch directly through the compiler-emitted __vt__5CLoad.
