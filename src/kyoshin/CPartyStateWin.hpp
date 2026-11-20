#pragma once

#include <types.h>
#include "kyoshin/CTitleAHelp.hpp"
#include "monolib/core/CViewFrame.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)

// Verbatim-mangled import: the real retail CfGameManager::isClassicController
// takes the selector argument (call sites pass -1). The inline s16 overload
// in include/kyoshin/cf/CfGameManager.hpp drops it, so callers that need the
// argument use this declaration instead.

// Forward decls only. The full sub-object headers (CTitleAHelp.hpp,
// CModelDisp.hpp, CPartyState.hpp, CEquipChange.hpp) cannot be included here:
// e.g. CPartyState.hpp's `extern CScn* gmScene;` clashes with
// CMenuPTState.cpp's own `extern void* gmScene;`, and
// CModelDispEquip.hpp's `struct CEventFile` clashes with
// monolib/work/CEventFile.hpp in TUs that include CBgTex.hpp / CScn.hpp.
// The sub-objects are embedded at fixed offsets, so sized pads + casts suffice.
class CTitleAHelp;
class CModelDisp;
class CPartyState;
class CEquipChange;
class CSysWin;
class CScn;
class CWorkThread;
class IScnRender;
class CProc;

// nw4r layout DrawInfo (full definition in <nw4r/lyt/lyt_drawInfo.h>;
// only a pointer is used in declarations here).
namespace nw4r { namespace lyt { class DrawInfo; } }

// Opaque view of the CModelDispEquip sub-object embedded at +0x3090 (retail
// size 0x10C0). The real class lives in
// kyoshin/menu/parts/CModelDispEquip.hpp, which cannot be included here (its
// `struct CEventFile` clashes with monolib/work/CEventFile.hpp). Only the
// embedded region and the C-linkage helper relocs are needed, so a sized
// opaque view suffices.
struct CModelDispEquipView {
    u8 data[0x10C0];
};

// Memory-accounting object used by PartyStateWin_PickAllocHandle: two signed counters at
// +0x00/+0x04 that are decremented by the freed size plus the 0x80 block
// header, after pinging the MEM1/MEM2 alloc handles.
struct CPartyStateWinMem {
    s32 field_0x0;   // 0x00
    s32 field_0x4;   // 0x04
};

// Byte view of the u32 window-state word (lbl_eu_806681E4) used by
// PartyStateWin_RefreshTitleHelp: the word is copied to a stack local and indexed by the
// party-state selection value (retail: lwz/stw copy + lbzx index).
union CPartyStateWinWord {
    u32 word;     // 0x00
    u8 bytes[4];  // 0x00
};

// Copy blob used by PartyStateWin_CopyWinBlob: three 0xFF0-byte blocks at +0x08/+0xFF8/
// +0x1FE8 (copied by PartyStateWin_CopyBlockFF0) plus trailing scalar fields.
struct CPartyStateWinCopy {
    u8 _pad00[0x4];              // 0x00
    u32 field_0x4;               // 0x04
    u8 block0[0xff0];            // 0x08..0xFF8
    u8 block1[0xff0];            // 0xFF8..0x1FE8
    u8 block2[0xff0];            // 0x1FE8..0x2FD8
    u8 field_0x2FD8;             // 0x2FD8
    u8 _pad2FD9[0x2fdc - 0x2fd9]; // 0x2FD9..0x2FDC
    f32 field_0x2FDC;            // 0x2FDC
    f32 field_0x2FE0;            // 0x2FE0
    u8 field_0x2FE4;             // 0x2FE4
};

// 8-byte pair (two u32) used for the tail of the PartyStateWin_CopyBlockFF0 block; the
// copy loop iterates these (retail mtctr/bdnz lwzu+stwu shape, same as the
// SCopyPair loops in CopyPTSubBlob).
struct CPartyStateWinWordPair {
    u32 lo;  // 0x00
    u32 hi;  // 0x04
};

// 0xFF0-byte state block copied by PartyStateWin_CopyBlockFF0: three leading u32, three
// SCopy_80191C88 blobs (0x53C each), a trailing u32, a single word copied
// before the loop and four word-pairs (words[1..8]) copied by the loop.
struct CPartyStateWinBlock {
    u32 field_0x0;       // 0x000
    u32 field_0x4;       // 0x004
    u32 field_0x8;       // 0x008
    u8 copy0[0x53c];     // 0x00C..0x548 (SCopy_80191C88 blob)
    u32 field_0x548;     // 0x548
    u32 field_0x54c;     // 0x54C
    u8 copy1[0x53c];     // 0x550..0xA8C (SCopy_80191C88 blob)
    u8 copy2[0x53c];     // 0xA8C..0xFC8 (SCopy_80191C88 blob)
    u32 field_0xfc8;     // 0xFC8
    u32 word0;           // 0xFCC (copied separately before the loop)
    CPartyStateWinWordPair pairs[4];  // 0xFD0..0xFEC (words[1..8])
    u32 field_0xfec;     // 0xFEC..0xFF0 (not copied)
};

// 0x24-byte ring-buffer record pushed by PartyStateWin_PushRingRecord into the view object
// at CPartyStateWin +0x14 (type word, 7 words, u16, u8, u8 - the trailing
// fields sit at 0x20/0x22/0x23, not packed after 0x18).
struct CPartyStateWinRec {
    u32 field_0x0;       // 0x00 type tag
    u32 field_0x4;       // 0x04
    u32 field_0x8;       // 0x08
    u32 field_0xc;       // 0x0C
    u32 field_0x10;      // 0x10
    u32 field_0x14;      // 0x14
    u32 field_0x18;      // 0x18
    u32 field_0x1c;      // 0x1C
    u16 field_0x20;      // 0x20
    u8 field_0x22;       // 0x22
    u8 field_0x23;       // 0x23
};

// Ring-buffer view of the object PartyStateWin_PushRingRecord pushes records into (the
// CView created by the ctor at CPartyStateWin +0x14).
struct CPartyStateWinRing {
    u8 _pad00[0x3ec];    // 0x000..0x3EC
    u8* field_0x3ec;     // 0x3EC record buffer base
    s32 field_0x3f0;     // 0x3F0
    s32 field_0x3f4;     // 0x3F4 write offset
    s32 field_0x3f8;     // 0x3F8 ring size (divisor)
    s32 field_0x3fc;     // 0x3FC last written offset
};

/*
 * Party-state window object (US retail).
 *
 * Deliberately NON-polymorphic in this TU: the retail object carries two
 * vtable-like pointers at +0x00/+0x04 (composite vtable lbl_eu_805352DC
 * written by the factory ctor, owned by another .data unit), and the -4
 * interface thunks PartyStateWin_ThunkRenderBefore / PartyStateWin_ThunkDtor are dispatched through the
 * +0x04 vtable slot (they receive obj+4 and rewind to the object base).
 * The dtor is a plain (non-virtual) member so those thunks forward to the
 * retail symbol directly; it is not defined in this TU yet.
 *
 * Layout (from the ctor / dtor ASM):
 *   0x00  composite vtable pointer (lbl_eu_805352DC)
 *   0x04  secondary vtable pointer (lbl_eu_805352DC + 0x88)
 *   0x08  ctor arg / 0x0C ctor arg / 0x10 0x14 zeroed words
 *   0x18  CTitleAHelp
 *   0x50  CModelDisp (region 0x444..0x454 copied by PartyStateWin_StoreVec4Quad)
 *   0x3038 CPartyState
 *   0x3090 CModelDispEquip
 *   0x4150 CEquipChange
 *   0x6BA8 CSysWin
 *   0x6BE4 / 0x6BE5 byte flags (ctor zero-fill)
 *   0x6BE8 float (ctor init)
 */
class CPartyStateWin {
public:
    CPartyStateWin(u32 arg1, u32 arg2);
    ~CPartyStateWin();
    void cbRenderBefore();

    u32 mVtbl0;                            // 0x00 composite vtable (lbl_eu_805352DC)
    u32 mVtbl4;                            // 0x04 secondary vtable (lbl_eu_805352DC + 0x88)
    u32 mArg1;                             // 0x08 ctor arg 1
    u32 mArg2;                             // 0x0C ctor arg 2
    CScn* mScene;                          // 0x10 owning scene (0 until assigned)
    CWorkThread* mWork14;                  // 0x14 child work thread (0 until assigned)
    u8 _pad18[0x38];                       // 0x18..0x50 (CTitleAHelp)
    u8 _pad50[0x444 - 0x50];               // 0x50..0x444 (CModelDisp head)
    u32 mQuad444[4];                       // 0x444..0x453 (PartyStateWin_StoreVec4Quad target)
    u8 _pad454[0x3038 - 0x454];            // 0x454..0x3038 (CModelDisp tail)
    u8 _pad3038[0x58];                     // 0x3038..0x3090 (CPartyState)
    CModelDispEquipView mModelDispEquip;   // 0x3090 (0x10C0)
    u8 _pad4150[0x2A58];                   // 0x4150..0x6BA8 (CEquipChange)
    u8 _pad6BA8[0x3C];                     // 0x6BA8..0x6BE4 (CSysWin)
    u8 field_6BE4;                         // 0x6BE4
    u8 field_6BE5;                         // 0x6BE5 (PartyStateWin_GetLatchFlag getter)
    u8 _pad6BE6[2];                        // 0x6BE6..0x6BE8
    f32 field_6BE8;                        // 0x6BE8 (ctor init)
};

void PartyStateWin_TeardownWindow(CPartyStateWin* self);
int PartyStateWin_GetLatchFlag(CPartyStateWin* self);
int PartyStateWin_QueryMenuClose(CPartyStateWin* self);
void PartyStateWin_ThunkDtor(void* self);

// ---- imports folded from the former CPartyStateWinImports.hpp ----
// Unit-private imports, copy views and helper structs of the CPartyStateWin
// translation unit (kept out of CPartyStateWin.hpp so that including the class
// does not pull these flat declarations into unrelated units).

#include "monolib/util/MemManager.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
namespace cf { class CfObjectMove; }
#include "kyoshin/CPartyStateWin.hpp"
#include "kyoshin/code_80135FDC_State.hpp"

// C-linkage imports for the embedded CModelDispEquip region (retail names are
// unmangled; the definitions live in kyoshin/menu/parts/CModelDispEquip.cpp
// with the same extern "C" symbols). Return types are int where the retail
// call sites cmpwi the result directly (an u8 return would make MWCC emit a
// normalizing rlwinm before the compare).
extern "C" int ModelDispEquip_GetState20(CModelDispEquipView*);
extern "C" void ModelDispEquip_StartFadeOut(CModelDispEquipView*);
extern "C" u32 ModelDispEquip_GetEquipSlot(CModelDispEquipView*);
extern "C" void ModelDispEquip_NextEquipSlot(CModelDispEquipView*);
extern "C" void ModelDispEquip_PrevEquipSlot(CModelDispEquipView*);
class CModelDispEquip;
void ModelDispEquip_ResetColorStep(CModelDispEquip* self);
void ModelDispEquip_ResetColorWhite(CModelDispEquip* self);
extern "C" void ModelDispEquip_ResetDisplay(CModelDispEquipView*);

// C-linkage import for the embedded CEquipChange sub-object used by
// PartyStateWin_StateRebuild (retail unmangled name; int return so the retail cmpwi
// compares r3 directly).

// C-linkage imports for the embedded CPartyState / CModelDisp / CEquipChange
// sub-objects (retail unmangled names; CModelDisp.cpp / CEquipChange.cpp
// define the same symbols). int returns where the retail cmpwi's the call
// result directly (an u8 return would add a normalizing rlwinm).
extern "C" void setupPartySelect(CPartyState*);
extern "C" void ModelDispStartFadeOut(CModelDisp*);

// Per-frame party-state input handlers used by PartyStateWin_StatePartyInput's pad chain
// (retail unmangled; CPartyState.hpp cannot be included here). int returns
// where the retail cmpwi's the call result directly.
extern "C" void cancelPartySelect(CPartyState*);
extern "C" void func_801FD594(CPartyState*);
extern "C" void stepPartySelectBack(CPartyState*);
extern "C" void stepPartySelectFwd(CPartyState*);
extern "C" void pageDownPartySelect(CPartyState*);
extern "C" void pageUpPartySelect(CPartyState*);
extern "C" void confirmPartySelect(CPartyState*);
extern "C" void startPartyPanelClose(CPartyState*);
extern "C" void ModelDispStartFadeIn(CModelDisp*);

// C-linkage imports for the embedded CTitleAHelp sub-object (retail unmangled
// names; CTitleAHelp.cpp defines the same symbols). isInitialized is a real
// member (see CTitleAHelp.hpp).

// String-pool lookup helper (retail unmangled; also declared in
// CMenuKizunagram.hpp) and the string pool it indexes.
extern char lbl_eu_80507C94[];

// C-linkage CSysWin helper imports (retail unmangled names, defined in
// CSysWin.cpp / CEquipChange-adjacent units).

// cf::CfGameManager helper (retail pre-mangled Fv name with a vestigial arg;
// same convention as CfGameManagerUnityHelpers.hpp).
extern "C" bool isPlayerReadyForEvent__Q22cf13CfGameManagerFv(s32 playerIndex,
                                                       bool requireFlag);

// CfGameManager controller-type query. Retail materializes r3=-1 into the
// pre-mangled Fv symbol; the inline s16 wrapper in CfGameManager.hpp would
// discard the arg when inlined, so bind the verbatim retail name with the
// int arg (same scheme as CSysWin.hpp / CMenuSkipTimer.hpp).

// UI sound effect (playUISound__FUl). C++ linkage so MWCC emits the mangled
// retail name.

// Sibling in this TU (retail unmangled name; extern "C" so the call site in
// PartyStateWin_StateOpen binds to the retail symbol).
extern "C" void PartyStateWin_RefreshTitleHelp(CPartyStateWin*);

// Sibling copy helper in this TU (retail unmangled name; copies a 0xFF0-byte
// block and returns dst, matching retail's mr r3,r30; extern "C" so the call
// sites in PartyStateWin_CopyWinBlob emit the retail name).
extern "C" u8* PartyStateWin_CopyBlockFF0(u8* dst, u8* src);

// State-blob copy helper called by PartyStateWin_CopyBlockFF0 (retail unmangled; defined
// in code_8018F8D8.cpp). Opaque byte pointers because SCopy_80191C88's
// header cannot be included here.
extern "C" void CopyPTSubBlob(u8* dst, const u8* src);

// C-linkage imports for the embedded CPartyState sub-object used by
// PartyStateWin_StateSettle / PartyStateWin_RefreshTitleHelp. func_801FD5C4 returns u8 here (the caller
// masks at use sites, retail has no post-call rlwinm); isPartyReady returns
// int (retail cmpwi's the call result directly).
extern "C" u8 func_801FD5C4(CPartyState*);

// Party-state selection getter used by PartyStateWin_StateRefresh's equip-display refresh
// (retail unmangled; distinct from func_801FD5C4 above). Returns u8 here
// (the caller masks the result with clrlwi at the use site).

// UI sound/announce helper (retail unmangled; declared in
// CfGameManagerUnityHelpers.hpp, which is not included here).
extern "C" void UIBattleReleaseAll();

// CTitleAHelp byte-setter (retail unmangled; CTitleAHelp.hpp cannot be
// included here).

// Party-size byte / config-word lookups (retail unmangled; also declared in
// CPartyState.hpp / CMapSel.hpp, which cannot be included here).
#include "kyoshin/cf/CtrlRemoteApi.hpp"

// Scene object behind the common-archive alloc handle; its alloc-handle
// getter (retail unmangled, same declarations as CPartyState.hpp).

// Window-state word read by PartyStateWin_RefreshTitleHelp (.sdata, retail unmangled). const
// lets MWCC hoist the sda21 load above the frame stores (retail shape -
// MWCC_CASES "extern const float" pattern).
extern const u32 lbl_eu_806681E4;

// Composite vtable written by the ctor at +0x00/+0x04 (owned by another
// .data unit; +0x88 is the secondary interface vtable offset).
extern u8 lbl_eu_805352DC[];

// The ctor's initial field_6BE8 value and the equip-input timer step/clamp are
// plain 0.0f / 1.0f / 10.0f literals in the .cpp (compiler-emitted .sdata2 pool).

// Global mode bitfield (bit 0x400 gates the party-window render).
extern u32 gmPauseFlags;

// CTaskGame / render-gate helpers (retail pre-mangled names; same scheme as
// CMenuKizunagram.hpp - the full CTaskGame.hpp pulls broken monolib umbrella
// headers, so only a forward decl is used here).
class CTaskGame;

// Sub-object render helpers taking a DrawInfo (retail unmangled).
extern "C" void drawPartyState(CPartyState*, nw4r::lyt::DrawInfo*);

// Embedded sub-object destructors (retail C++-mangled names, defined in the
// owning TUs: CSysWin.cpp / CEquipChange.cpp / CModelDispEquip.cpp /
// CPartyState.cpp / CModelDisp.cpp / CTitleAHelp.cpp). Declared under C
// linkage with the literal retail names so CPartyStateWin's dtor emits the
// exact call relocs (same pattern as __dt__6CCur22Fv in CPartyState.hpp).
extern "C" void __dt__7CSysWinFv(void*, int);
extern "C" CModelDispEquipView* __dt__15CModelDispEquipFv(CModelDispEquipView*, int);
extern "C" CPartyState* __dt__11CPartyStateFv(CPartyState*, int);
extern "C" CModelDisp* __dt__10CModelDispFv(CModelDisp*, int);
extern "C" void __dt__11CTitleAHelpFv(CTitleAHelp*, int);

// Embedded sub-object constructors (retail unmangled ctor names, defined in
// the owning TUs; same scheme as the dtor declarations above).
extern "C" void __ct__11CTitleAHelpFPcUc(CTitleAHelp*, char* name, u8 arg);
extern "C" void __ct__CModelDisp(CModelDisp*, u32 arg);
extern "C" void __ct__11CPartyStateFv(CPartyState*);
extern "C" void __ct__CModelDispEquip(CModelDispEquipView*, u32 arg1, u32 arg2);
extern "C" void __ct__12CEquipChangeFv(CEquipChange*);
extern "C" void __ct__7CSysWinFUc(void*, int arg);

// This TU's own dtor symbol under C linkage (the member ~CPartyStateWin emits
// the same name). PartyStateWin_ThunkDtor's r4-passthrough thunk calls it via a 1-arg
// function-pointer cast so the incoming delete flag survives.
extern "C" CPartyStateWin* __dt__14CPartyStateWinFv(CPartyStateWin*, int);

// 2x s16 pair / 4x s16 quad views for PartyStateWin_BuildS16Quad (retail lha/sth copy).
struct CPartyStateS16Pair { s16 x, y; };
struct CPartyStateS16Quad { s16 x, y, z, w; };

// C-linkage sibling copy helper (retail unmangled name). Returns dst so the
// PartyStateWin_FrameStep call site can pass the copied quad straight to setRect
// (retail: mr r4, r3 after the bl). The body keeps dst in r3 throughout, so
// `return dst` adds no instructions (byte-identical to the void form).
extern "C" CPartyStateS16Quad* PartyStateWin_BuildS16Quad(CPartyStateS16Quad* dst,
                                               const CPartyStateS16Pair* srcA,
                                               const CPartyStateS16Pair* srcB);

// Source view of PartyStateWin_BuildRectQuad's second argument: an embedded CViewFrame at
// 0x1DC whose content origin (mContentX/Y at frame +0x54/0x56) doubles as the
// s16 pair at 0x230/0x232 read by the offset computation.
struct CPartyStateWinRectSrc {
    u8 _pad00[0x1c8];              // 0x000..0x1C8
    CPartyStateS16Pair mPair1C8;   // 0x1C8
    u8 _pad1CC[0x1dc - 0x1cc];     // 0x1CC..0x1DC
    CViewFrame mFrame1DC;          // 0x1DC (mContentX@0x230, mContentY@0x232)
};

// Title-help idle check. The retail symbol is the C++-mangled member name, so
// it is referenced under C linkage with that literal name (repo-wide pattern
// used by CMenuOption/CMenuShopBuy/...); int return avoids the u8-normalizing
// rlwinm at the call sites (retail cmpwi's the result directly).


// Sub-object teardown helpers used by PartyStateWin_TeardownWindow (retail unmangled; the
// embedded CTitleAHelp/CModelDisp/CPartyState/CEquipChange/CSysWin headers
// cannot be included here). ModelDispEquip_ResetDisplay is declared above.
extern "C" void ModelDispTeardownAll(CModelDisp*);
extern "C" void termPartyState(CPartyState*);

// Effect-task scene unregister helper (retail unmangled; defined in
// CTaskGameEff.cpp). PartyStateWin_TeardownWindow calls it unconditionally with the owning
// scene before removing the render callback.
extern "C" void unregisterEffectScene(CScn*);

// Equip-change / sys-win settle checks used by PartyStateWin_StateRefresh's gate (retail
// unmangled; int returns so the retail cmpwi compares r3 directly).

// Per-frame sub-object update helpers used by PartyStateWin_FrameStep's tail (retail
// unmangled; the embedded sub-object headers cannot be included here).
extern "C" void ModelDispTickState(CModelDisp*);
extern "C" void advancePartyPanel(CPartyState*);
extern "C" void ModelDispEquip_StepStateDispatch(CModelDispEquipView*);

// C-linkage imports for the embedded CEquipChange / CSysWin / CModelDispEquip
// sub-object handlers used by PartyStateWin_StateEquipInput's pad-dispatch chains (retail
// unmangled; the embedded sub-object headers cannot be included here).
// u8/u16 returns where the retail masks the call result (clrlwi) before
// comparing; int returns where the retail cmpwi's the call result directly.
extern "C" int ModelDispEquip_GetState21(CModelDispEquipView*);
extern "C" void ModelDispEquip_StartFadeIn(CModelDispEquipView*);

// Shared window object pointer value (.sdata, retail unmangled; PartyStateWin_GetSharedWinObj
// returns it, PartyStateWin_FrameStep reads its stored s16 pair at +0x1C8).
extern u32 gmCurrentView;

// CModelDispEquip select-slot setter used by PartyStateWin_StateRefresh (retail unmangled;
// the value is masked to u8 at the call site, but the parameter is int: the
// PartyStateWin_StateEquipRefresh call site passes -1 as a full word).
extern "C" void ModelDispEquip_SetEquipSlot(CModelDispEquipView*, int);

// Enum list/slot views used by PartyStateWin_StateEquipRefresh's item-filter block
// (CfEnumListHolder::get() results).
struct CPartyStateWinList {
    u8 _00[0x620];
    u32 field_0x620;   // +0x620
};
struct CPartyStateWinListSlot {
    u8 _00[0x4];
    cf::CfObjectMove* field_0x4;   // +0x4 (null-checked, passed to getCfObjectPc)
};

#include "monolib/util/MemManager.hpp"
namespace cf { class CfObjectMove; }

// The CfObjEnumList helper family is declared with opaque void* parameters
// repo-wide (CAIAction.hpp / CItemBoxGrid.hpp): typed parameters would make
// extern "C" re-declarations clash (10197 illegal function overloading) in
// TUs that include both headers. ABI is unchanged.
extern "C" void startEnumObjects(void*, u32, u32);
extern "C" void* getEntryAt(void*, u32);
extern "C" void* getCfObjectPc__FPQ22cf12CfObjectMove(void* objMove);
namespace cf { class CfObjectPc; }
void CfObjectPc_setupFrame(cf::CfObjectPc* obj);
typedef void (*CfObjectPcSetupFrameFn)(cf::CfObjectPc*, u32);  // DEBT(callcast): retail passes live r4 to 1-arg CfObjectPc_setupFrame (mangled arity 1; LOSTFIX proof)

// 3-word name-table read by PartyStateWin_StateEquipRefresh's item-filter block (.rodata,
// retail unmangled). const so MWCC can keep one base register for the three
// loads (retail: lis + lwzu + +4/+8 displacements).
extern const u32 lbl_eu_80507C78[3];

// 0x18-byte cursor-like sub-object embedded at +0x34 of the PartyStateWin_CopyStateBlob58
// blob. MenuShopSellCopyCursor copies its +0x4..+0x15 region (4 words + 2 bytes); the
// +0x0 word is not copied (same shape as ShopSellCursor18 in
// CMenuShopSell.hpp, which cannot be included here).
struct CPartyStateWinCursor {
    u32 field_0x0;    // +0x0 (not copied)
    u32 field_0x4;    // +0x4
    u32 field_0x8;    // +0x8
    u32 field_0xC;    // +0xC
    u32 field_0x10;   // +0x10
    u8 field_0x14;    // +0x14
    u8 field_0x15;    // +0x15
};

// 0x58-byte menu-state blob copied by PartyStateWin_CopyStateBlob58: a 4-word mem region at
// +0x04 (copied by the __as__17UnkClass_8045F564FRC17UnkClass_8045F564 helper), scalar fields
// +0x14..+0x31, a cursor sub-object at +0x34 (copied by MenuShopSellCopyCursor) and a
// packed tail. The tail's +0x4F u32 and +0x53 u16 sit at odd offsets, so the
// struct is 1-packed to reproduce the retail misaligned lwz/stw + lhz/sth
// (same pack pattern as CInfoCf.hpp).
#pragma pack(push, 1)
struct CPartyStateWinBlob58 {
    u32 field_0x0;              // 0x00 (not copied)
    u32 field_0x4[4];           // 0x04..0x14 (copied by __as__17UnkClass_8045F564FRC17UnkClass_8045F564)
    u32 field_0x14;             // 0x14
    u32 field_0x18;             // 0x18
    u32 field_0x1C;             // 0x1C
    u32 field_0x20;             // 0x20
    u32 field_0x24;             // 0x24
    u8 field_0x28;              // 0x28
    u8 _pad29[3];               // 0x29..0x2C
    u32 field_0x2C;             // 0x2C
    u8 field_0x30;              // 0x30
    u8 field_0x31;              // 0x31
    u8 _pad32[2];               // 0x32..0x34
    CPartyStateWinCursor field_0x34;  // 0x34..0x4C (copied by MenuShopSellCopyCursor)
    u8 field_0x4C;              // 0x4C
    u8 field_0x4D;              // 0x4D
    u8 field_0x4E;              // 0x4E
    u32 field_0x4F;             // 0x4F (misaligned)
    u16 field_0x53;             // 0x53 (misaligned)
    u8 field_0x55;              // 0x55
    u8 field_0x56;              // 0x56
    u8 field_0x57;              // 0x57
};
#pragma pack(pop)

// 0x10C0-byte state blob copied by PartyStateWin_CopyBlob10C0 (same family as
// CPartyStateWinBlock: three 0x53C SCopy_80191C88 regions with word tails,
// then counted 8-byte pair runs). Adjacent word pairs that retail copies as a
// 2-load/2-reverse-store unit (0xFD8/0xFDC, 0x1004/0x1008, 0x1098/0x109C,
// 0x10A8/0x10AC, 0x10B4/0x10B8) are CPartyStateWinWordPair members; the rest
// are plain scalars copied field-by-field.
struct CPartyStateWinBlob10C0 {
    u8 _pad00[0xC];               // 0x000..0x00C
    u32 field_0xC;                // 0x00C
    u32 field_0x10;               // 0x010
    u32 field_0x14;               // 0x014
    u32 field_0x18;               // 0x018
    u8 copy0[0x53c];              // 0x01C..0x558 (SCopy_80191C88 blob)
    u32 field_0x558;              // 0x558
    u32 field_0x55c;              // 0x55C
    u8 copy1[0x53c];              // 0x560..0xA9C (SCopy_80191C88 blob)
    u8 copy2[0x53c];              // 0xA9C..0xFD8 (SCopy_80191C88 blob)
    CPartyStateWinWordPair pairFd8;    // 0xFD8..0xFE0
    CPartyStateWinWordPair pairs[4];   // 0xFE0..0x1000 (counted loop)
    u32 field_0x1000;             // 0x1000
    CPartyStateWinWordPair pair1004;   // 0x1004..0x100C
    u32 field_0x100c;             // 0x100C
    u32 field_0x1010;             // 0x1010
    u8 field_0x1014;              // 0x1014
    u8 _pad1015[0x1018 - 0x1015]; // 0x1015..0x1018
    f32 field_0x1018;             // 0x1018
    f32 field_0x101c;             // 0x101C
    u8 field_0x1020;              // 0x1020
    u8 field_0x1021;              // 0x1021
    u8 _pad1022[0x1024 - 0x1022]; // 0x1022..0x1024
    CPartyStateWinWordPair pairs2[13];  // 0x1024..0x108C (counted loop)
    u32 field_0x108c;             // 0x108C
    u32 field_0x1090;             // 0x1090
    u32 field_0x1094;             // 0x1094
    CPartyStateWinWordPair pair1098;   // 0x1098..0x10A0
    u32 field_0x10a0;             // 0x10A0
    f32 field_0x10a4;             // 0x10A4
    CPartyStateWinWordPair pair10A8;   // 0x10A8..0x10B0
    u32 field_0x10b0;             // 0x10B0
    CPartyStateWinWordPair pair10B4;   // 0x10B4..0x10BC
    u32 field_0x10bc;             // 0x10BC
};

// 4-word mem-region copy helper (retail unmangled; also declared in
// CSkipTimer.hpp / CQstLogList.hpp / COption.hpp). PartyStateWin_CopyStateBlob58 uses it to
// copy the +0x04 region of its blob.
extern "C" void __as__17UnkClass_8045F564FRC17UnkClass_8045F564(void* dest, void* src);

// Cursor sub-object copy helper (retail unmangled; also declared in
// COption.hpp / CItemBoxGrid.hpp). Copies the +0x4..+0x15 region of a
// 0x18-byte cursor object; PartyStateWin_CopyStateBlob58 passes its +0x34 sub-object.

// Copy helpers used by PartyStateWin_CopyEquipChange / PartyStateWin_OpenWindow (retail unmangled names;
// owning TU headers cannot be included here).
extern "C" void __as__12CItemBoxPageFRC12CItemBoxPage(u8* dst, const u8* src); // weak CItemBoxPage::operator=

// Byte-offset view of the embedded CEquipChange sub-object (retail size
// 0x2A58) as copied by PartyStateWin_CopyEquipChange. Regions handled by copy helpers are
// opaque byte blobs; every individually-copied scalar keeps field-style
// naming. The three counted loops copy 8-byte records through the shared
// wd[1]/wd[2] pointer-walk form, so their base pointers sit 4 bytes before
// each copied run.
struct CEquipChangeCopyView {
    u8 f04[0x10];        // 0x004 (__as__17UnkClass_8045F564FRC17UnkClass_8045F564)
    u8 f14[0x10];        // 0x014 (__as__17UnkClass_8045F564FRC17UnkClass_8045F564)
    u32 f24[8];          // 0x024..0x044
    u8 f44;              // 0x044
    u8 _pad45[3];
    u32 f48;             // 0x048
    u8 f4c;              // 0x04c
    u8 f4d;              // 0x04d
    u8 _pad4e[2];
    u8 f50[0x18];        // 0x050 (MenuShopSellCopyCursor)
    u8 f68[0x18];        // 0x068 (MenuShopSellCopyCursor)
    u8 f80[0x18];        // 0x080 (MenuShopSellCopyCursor)
    u8 f98;              // 0x098
    u8 f99;              // 0x099
    u32 f9a;             // 0x09a
    u32 f9e;             // 0x09e
    u8 _pada2[6];
    u8 fa8[0x10];        // 0x0a8 (__as__17UnkClass_8045F564FRC17UnkClass_8045F564)
    u8 fb8[0x10];        // 0x0b8 (__as__17UnkClass_8045F564FRC17UnkClass_8045F564)
    u32 fc8[27];         // 0x0c8..0x134
    u8 f134;             // 0x134
    u8 _pad135[3];
    u32 f138;            // 0x138
    u8 f13c;             // 0x13c
    u8 f13d;             // 0x13d
    u8 f13e;             // 0x13e
    u8 _pad13f;
    u32 f140;            // 0x140
    u32 f144;            // 0x144
    u32 f148;            // 0x148
    u32 f14c;            // 0x14c
    u16 f150;            // 0x150
    u8 f152;             // 0x152
    u8 _pad153;
    u8 f154[0x160];      // 0x154..0x2b4 (ShopBuy_CopyData)
    u8 f2b4[0x10];       // 0x2b4 (__as__17UnkClass_8045F564FRC17UnkClass_8045F564)
    u8 f2c4[0x10];       // 0x2c4 (__as__17UnkClass_8045F564FRC17UnkClass_8045F564)
    u32 f2d4[7];         // 0x2d4..0x2f0
    u8 f2f0[4];          // 0x2f0..0x2f4
    u8 f2f4[0x18];       // 0x2f4 (PartyStateWin_CopySlotRec)
    u8 f30c[0x18];       // 0x30c (PartyStateWin_CopySlotRec)
    u8 f324[0x1c];       // 0x324 (MenuShopSellCopyCursor)
    u8 f340[0x10];       // 0x340 (__as__17UnkClass_8045F564FRC17UnkClass_8045F564)
    u32 f350[5];         // 0x350..0x364
    u8 f364[4];          // 0x364..0x368
    u8 _pad368[4];
    u8 f36c[0x10];       // 0x36c (__as__17UnkClass_8045F564FRC17UnkClass_8045F564)
    u32 f37c[4];         // 0x37c..0x38c
    u8 f38c[4];          // 0x38c..0x390
    f32 f390[5];         // 0x390..0x3a4
    u8 f3a4;             // 0x3a4 (loop base for pairs16)
    u8 _pad3a5[3];
    u8 pairs16[0x80];    // 0x3a8..0x428 (counted loop, 16 records)
    u8 f428[3];          // 0x428..0x42b
    u8 _pad42b;
    u8 f42c[0x3c];       // 0x42c (InfoCfCopySysWin)
    u8 f468[0x3c];       // 0x468 (InfoCfCopySysWin)
    u8 f4a4[3];          // 0x4a4..0x4a7
    u8 _pad4a7;
    u16 f4a8;            // 0x4a8
    u16 f4aa;            // 0x4aa
    u16 f4ac;            // 0x4ac
    u8 f4ae;             // 0x4ae
    u8 _pad4af;
    u32 f4b0;            // 0x4b0
    u32 f4b4;            // 0x4b4
    u32 f4b8;            // 0x4b8
    u32 f4bc;            // 0x4bc
    u8 f4c0[0x15c];      // 0x4c0..0x61c (ShopBuy_CopyData)
    u32 f61c;            // 0x61c
    u16 f620;            // 0x620
    u8 f622[0xe];        // 0x622..0x630 (&f622[8] is the loop base at 0x62a)
    u8 f62e[0x2000];     // 0x62e..0x262e (counted loop, 0x400 records)
    u16 f262e;           // 0x262e
    u8 f2630[4];         // 0x2630..0x2634 (loop base for f2634)
    u8 f2634[0x20];      // 0x2634..0x2654 (counted loop, 4 records)
    u8 f2650[4];         // 0x2650..0x2654 (loop base for f2654)
    u8 f2654[0x400];     // 0x2654..0x2a54 (counted loop, 0x80 records)
    u32 f2a54;           // 0x2a54
};

// vtable-slot call on the embedded CSysWin during factory construction:
// retail loads the object's vtable, fetches slot +0x88 and calls it with the
// CSysWin sub-object as receiver.
struct CSysWinSlot88 {
    u8 _pad00[0x88];
    void (*slot88)(CSysWin*);
};

// 6-argument party-window gauge record filled by PartyStateWin_InitGaugeRecord and handed to
// CScnNw4r::create by the factory.
struct PartyGaugeRecord {
    u32 field_0;   // 0x00
    u32 field_4;   // 0x04
    f32 field_8;   // 0x08
    u16 field_C;   // 0x0C
    u16 gauge[10]; // 0x0E..0x22
    u16 field_22;  // 0x22
};

// Factory imports (retail unmangled names; the embedded sub-object headers
// cannot be included here - see the forward-decl note at the top).
// CTitleAHelp::CTitleAHelp_load() is declared in kyoshin/CTitleAHelp.hpp.
extern "C" void __as__11CTitleAHelpFRC11CTitleAHelp(CTitleAHelp* dest, CTitleAHelp* src); // weak CTitleAHelp::operator= (interim until CPartyStateWin is a real class)
extern "C" void ModelDispInitPoseSlots(CModelDisp* self);
extern "C" void loadPartyLayoutArc(CPartyState* self);
extern "C" void ModelDispEquip_SyncScalePose(CModelDispEquipView* self);
extern "C" void Scn_GetCamWorkInt(CScn* scn, CWorkThread* work, u32 flag);
extern "C" void PartyStateWin_BuildRectQuad(CPartyStateS16Quad* dst, CPartyStateWinRectSrc* obj);
extern "C" void PartyStateWin_StoreVec4Quad(CPartyStateWin* self, const u32* src);
// Defined with C++ linkage in CTaskGameEff.cpp; declared here to match.
void registerEffScene(CScn* scn);
extern "C" u8* getField5C(CScn* scn);
extern "C" void scnVlInitLights2(u8* unk5c);
class CProcess;
class CScnNw4r;
// Canonical form (definition in monolib/scn/CScn.cpp); the gauge slot is the
// opaque 5th argument.
extern "C" CScnNw4r* create__8CScnNw4rFv(CProcess* parent, char* name,
                                         u32 arg1, u32 handle, void* param);

// Task-game field readers (retail unmangled; defined in this TU).
extern "C" u32 PartyStateWin_GetGameProc(u8* self);
extern "C" u32 PartyStateWin_GetGameWorkThread(u8* self);
extern "C" u32 PartyStateWin_GetSharedWinObj(void);

// Sibling helpers defined later in this TU (retail unmangled names).
extern "C" void PartyStateWin_PushRingRecord(CPartyStateWinRing* self, u32 flag);
extern "C" CPartyStateWinMem* PartyStateWin_InitMemCounters(CPartyStateWinMem* self);
extern "C" void PartyStateWin_InitGaugeRecord(PartyGaugeRecord* rec, u32 a, f32 f, u16 b, u16 c,
                              u16 d);
extern "C" mtl::ALLOC_HANDLE PartyStateWin_PickAllocHandle(CPartyStateWinMem* self, s32 size);
extern "C" CPartyStateWinCopy* PartyStateWin_CopyWinBlob(CPartyStateWinCopy* dst,
                                             CPartyStateWinCopy* src);
extern "C" CPartyStateWinBlob58* PartyStateWin_CopyStateBlob58(CPartyStateWinBlob58* self,
                                               CPartyStateWinBlob58* src);
extern "C" CPartyStateWinBlob10C0* PartyStateWin_CopyBlob10C0(CPartyStateWinBlob10C0* self,
                                                 CPartyStateWinBlob10C0* src);
extern "C" void PartyStateWin_CopyEquipChange(CEquipChange* dst, CEquipChange* src);
extern "C" void PartyStateWin_OpenWindow(CPartyStateWin* self);

// Equip-display pad-chain targets used by PartyStateWin_StateEquipDisplay (retail unmangled;
// the CModelDispEquip header cannot be included here).
extern "C" void ModelDispEquip_ScaleDepthDown(CModelDispEquipView*);
extern "C" void ModelDispEquip_ScaleDepthUp(CModelDispEquipView*);
extern "C" void ModelDispEquip_FadeAlphaDown(CModelDispEquipView*);
extern "C" void ModelDispEquip_FadeAlphaUp(CModelDispEquipView*);
extern "C" void ModelDispEquip_ScaleHeightUp(CModelDispEquipView*);
extern "C" void ModelDispEquip_ScaleHeightDown(CModelDispEquipView*);

// CTitleAHelp idle check distinct from (retail
// unmangled; int return so the caller cntlzw/srwi-normalizes it)->func_801C4124().
