#pragma once

// Unit-private import declarations and helper views split out of
// CMenuArtsSet.hpp; included only by CMenuArtsSet.cpp.
#include "kyoshin/menu/CMenuArtsSet.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
// Sub-object ctor/dtor/body-copy imports for CArtsList::CArtsList's temp
// copy-init (retail emits the unmangled names at the call sites).
extern "C" void __ct__10CScrollBarFUc(CScrollBar* self, int arg);
extern "C" void __dt__10CScrollBarFv(void* self, int flags);
extern "C" void __dt__9CArtsInfoFv(void* self, int flags);
extern "C" void __ct__7CSysWinFUc(void* self, int arg);
extern "C" void __dt__7CSysWinFv(void* self, int flags);
extern "C" void __as__17UnkClass_8045F564FRC17UnkClass_8045F564(void* dest, void* src);
// OnFileEvent imports: mem-region resize / font / layout-text helpers
// (retail emits the unmangled names at the call sites).
extern "C" void* createRegion__17UnkClass_8045F564FiiPCci(void*, int, int, const char*, int);
extern "C" void setMemInitFlag__Q23mtl10MemManagerFb(bool);
extern "C" bool Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(nw4r::lyt::ArcResourceAccessor*, void*, const char*);
extern "C" void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
extern "C" u32 CSysWin_isReady(void*);
extern "C" void* CtrlObjectParam_GetCharEntry(u32);
extern "C" u32 CtrlObjectParam_GetCurrentRowKey(void*);
// Arts-slot re-arm ctor (CtrlObjectParam.cpp; plain-C retail symbol).
extern "C" void __ct__8009F8B8(void* self);
// Actor-param refresh + player resolution tail of artsClaimSlot.
extern "C" void CtrlObjectParam_SyncParamFromActorEx(void* self, u32 flag);
extern "C" void* getPlayer__Q22cf13CfGameManagerFi(int index);
extern "C" void* getCfObjectPc__FPQ22cf12CfObjectMove(void* objMove);

// Player object returned by getCfObjectPc in artsClaimSlot's tail: the u16
// player id at +0x3F28 must equal the sub-object's 0x26 character id, then
// the refresh virtual at vtable +0x32C runs. With the 2-entry RTTI header,
// vtable +0x32C is declared index 201.
class SArtsPlayerRefresh {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075();
    virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091();
    virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111();
    virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115();
    virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123();
    virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127();
    virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131();
    virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135();
    virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147();
    virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151();
    virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155();
    virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159();
    virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163();
    virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167();
    virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171();
    virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175();
    virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179();
    virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183();
    virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187();
    virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191();
    virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195();
    virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199();
    virtual void v200();
    virtual void vf32C();   // index 201 -> vtable +0x32C

    u8 _pad04[0x3F28 - 0x4];
    u16 field_3F28;         // +0x3F28 player id
};

// CArtsInfo helpers defined in CArtsInfo.cpp (plain-C retail symbols).
extern "C" int CArtsInfo_GetFlag48(CArtsInfo* self);
extern "C" void CArtsInfo_ReqState4(CArtsInfo* self);
extern "C" void CArtsInfo_LoadFiles(CArtsInfo* self);

// Table lookup helper (defined later in this TU). Plain-C retail symbol;
// kept out-of-line so callers emit a real bl instead of inlining.
extern "C" __declspec(noinline) u8 ArtsCharList_getId(SArtsSub8022FA58* self, u32 idx);

// BDAT message-table / texture-name helpers used by ArtsCharList_refreshDisplay (imports).
extern void* lbl_eu_80664090;
// Mangled retail symbol declared directly so the third argument stays a
// full register (retail passes the raw manager value without truncation).

extern "C" void ArtsCharList_refreshDisplay(SArtsSub8022FA58* self);
extern "C" void ArtsSlotCursor_refresh(SArtsSub8022FA58* self);
// Scrollbar cursor refresh (defined in CScrollBar.cpp; C-linkage retail name).
// Layout-out animation driver (same TU, C-linkage retail name).
extern "C" void ArtsBusyPane_refreshCursor(SArts313E0* self);

// Character-slot record view for artsReadSelId's mode-4/id-8 arm: the
// signed halfword at +0x24 is the current entry id (-1 = none).
struct SArtsCharData24 {
    char _pad[0x24];
    s16 field_0x24;
};
// Entry object returned by CItem_kindPtr; artsReadSelId only reads the
// leading word (top 12 bits feed the category lookup).
struct SArtsEntryWord {
    u32 field_0x00;
};
// Mode-4/id-8 imports (code_80135FDC.cpp / CItem.cpp; C symbols).
extern "C" SArtsEntryWord* CItem_kindPtr(u32 kind);
// Current BDAT text-table handle consumed by BdatGetU8Direct (.sdata word).
extern u32 lbl_eu_806640F8;

// Same-TU cursor/state helpers (retail unmangled names; definitions below).
extern "C" u8 artsReadSelId(SArtsSub8022FA58* self);
extern "C" u8 artsAvailFlag(SArtsSub8022FA58* self);
extern "C" u8 ArtsTable_getSelectedId(SArts322BC* self);
extern "C" u8 ArtsTable_getRowValue(SArts322BC* self, int key);
extern "C" u16 artsGetRowPct(SArts322BC* self, int key);
extern "C" void ArtsTable_bumpUsage(SArts322BC* self, int key);
extern "C" void ArtsTableCursor_refresh(SArts327B0* self);
extern "C" void artsRenderRow(SArts327B0* self);
extern "C" void copyVEC2(float* dst, const float* src);
extern "C" __declspec(noinline) int CMenuArtsSet_isListBusy(SArts34C84* self);
extern "C" __declspec(noinline) int CMenuArtsSet_isLockedArtOpen(SArts34D14* self);
extern "C" __declspec(noinline) int CMenuArtsSet_isDemoLockedArt(SArts34D14* self);
extern "C" u32 CMenuArtsSet_getHelpMode(CArtsList* self);

// Cross-unit CArtsInfo helpers (C-linkage, defined in CArtsInfo.cpp).
// Declared u32 so the ==0/!=0 tests compare the raw register (no rlwinm).
extern "C" u32 CArtsInfo_GetFlag49(CArtsInfo* self);
extern "C" void CArtsInfo_ReqState1(CArtsInfo* self);
extern "C" u32 CArtsInfo_IsStateGE6(CArtsInfo* self);
// CArtsInfo cursor-step helpers (CMenuArtsSet_cursorUp / CMenuArtsSet_cursorDown busy paths).
extern "C" void CArtsInfo_CursorPrev(CArtsInfo* self);
extern "C" void CArtsInfo_CursorNext(CArtsInfo* self);
// CArtsInfo state-machine advance (defined in CArtsInfo.cpp; C symbol).
extern "C" void CArtsInfo_ToStateA(CArtsInfo* self);
// CArtsInfo advance helpers used by artsDriveMenu (CArtsInfo.cpp; C symbols).
extern "C" u32 CArtsInfo_IsCursorZero(CArtsInfo* self);
extern "C" void CArtsInfo_ToStateC(CArtsInfo* self);
extern "C" void CArtsInfo_OpenDetail6(CArtsInfo* self);
// Locked-entry refresh driven by artsDriveMenu when the 0x168 mode is 4.
extern "C" void CMenuArtsSet_confirmLockedArt(CArtsList* self);
// CSysWin active/advance helpers (defined in CSysWin.cpp; C symbols).
extern "C" int CSysWin_isActive(void* self);
extern "C" void sysWinAdvancePhase3(void* self);
// CArtsInfo draw (defined in CArtsInfo.cpp; C-linkage retail name).
extern "C" void CArtsInfo_Draw(CArtsInfo* self, void* drawInfo);
// CSysWin draw (defined in CSysWin.cpp; C-linkage retail name).
extern "C" void sysWinDrawLayout(CSysWin* self, nw4r::lyt::DrawInfo* drawInfo);
// Pane-visible check (defined in CTitleAHelp.cpp; C-linkage retail name).
extern "C" bool isPaneVisible(nw4r::lyt::Pane* pane);
// C-linkage CSysWin state query (defined in CSysWin.cpp).
extern "C" u32 CSysWin_getUnk34(void* self);
// Drive/refresh helper (same TU, defined below; C-name for reloc parity).
extern "C" void CMenuArtsSet_refreshArtsInfo(CArtsList* self);

// Scroll-bar scroll-out request (CMenuArtsSet_completeLayoutOut state-4 tail).
// Scroll-bar scroll-in with a 3-float init vector, and the tick that
// consumes it (CMenuArtsSet_completeLayoutIn). Declared here rather than including
// CMenuPlayAward.hpp (same signature-clash reason as that header documents).
// Scroll-bar teardown (CMenuArtsSet_teardown).
extern "C" void CArtsInfo_UpdateAnims(CArtsInfo* self);
extern "C" void sysWinDispatchPhase(CSysWin* self);
extern "C" void ArtsLayoutPair_advancePhases(SArts306F0* obj148);
extern "C" void ArtsLayout_runTwinLayoutIn(SArts31648* self);
extern "C" void ArtsTableLayout_runLayoutIn(SArts3150C* self);
extern "C" void ArtsTableLayout_runTwinLayoutIn(SArts31648* self);
extern "C" void ArtsLayout_runLayoutOut(SArts31480* self);
extern "C" void ArtsLayout_runTwinLayoutOut(SArts315BC* self);
extern "C" void ArtsTableLayout_advancePhases(SArts306F0* obj148);
extern "C" void CMenuArtsSet_completeLayoutIn(CArtsList* self);
extern "C" void CMenuArtsSet_resetSubList(CArtsList* self);
// Arts-table row-check (defined in this TU): returns 1 when the row for key
// (-1 = current cursor row) has state byte 4 with a clear +8 flag, or state
// byte 7 with a clear +9 flag.
extern "C" int ArtsTable_isRowSelectable(SArts322BC* self, int key);
extern "C" void CMenuArtsSet_completeLayoutOut(CArtsList* self);
extern "C" void CMenuArtsSet_enterInteract(CArtsList* self);
extern "C" void CMenuArtsSet_finishCloseAnim(SArts3506C* self);
extern "C" void CMenuArtsSet_returnFromInfo(CArtsList* self);
// CMenuArtsSet sub-object release (CMenuArtsSet_teardown).
extern "C" void ArtsLayoutPair_releaseSlots(SArts3066C* self);
// SArtsSub8022FA58 cursor-store helper (CMenuArtsSet_completeLayoutOut). Same-TU definition;
// noinline so callers emit a real bl (retail shape).
extern "C" __declspec(noinline) void ArtsSubCursor_setVisible(SArtsSub8022FA58* self, bool val);
// Same-TU release helpers kept out-of-line for retail bl parity.
extern "C" __declspec(noinline) void ArtsLayoutPair_release(SArts2FDF4* self);
extern "C" __declspec(noinline) void ArtsTableLayout_release(SArts3066C* self);

// CMenuArtsSet state-machine / teardown / driver entry points (defined in
// this TU; C linkage matches the retail unmangled symbols).
extern "C" void ArtsCharList_build(SArtsSub8022FA58* self);
extern "C" void ArtsTable_pageUp(SArts322BC* self);
extern "C" void CMenuArtsSet_teardown(CArtsList* self);
extern "C" void ArtsLayout_runLayoutIn(SArts3150C* self);

// code_80135FDC helpers (retail C symbols; local decls because
// code_80135FDC.hpp cannot be co-included - BdatTouchStringCell signature clash).
// File-handle / arc-accessor teardown; retail symbols are the mangled C++
// names (closeFileHandle__FPP11CFileHandle etc.) so declare C++-linkage.
// Layout/anim binding helpers (code_80135FDC.cpp). C++ linkage: MWCC mangles
// buildLayout/bindLayoutAnimTransform to the retail names (buildLayout__FPPQ...).
// CArtsInfo teardown (defined in CArtsInfo.cpp, C symbol).
extern "C" void CArtsInfo_Cleanup(CArtsInfo* self);
// CSysWin teardown (defined in CSysWin.cpp, C symbol).
extern "C" void sysWinTermLayout(CSysWin* self);

// .sdata2 floats for the layout-animation drivers (ArtsLayout_runLayoutOut etc.).
// const is load-bearing: it lets MWCC treat the pool load as a constant and
// hoist the lfs above the frame stores (retail position 3) - plain extern
// float places it after the stores (4-byte shift, MWCC_CASES).
extern const float lbl_eu_80668648;
extern const float lbl_eu_8066864C;
// .sdata2 floats for ArtsSlotCursor_refresh's 9-entry cursor position table.
extern const float lbl_eu_80668650;
extern const float lbl_eu_80668654;
// .sdata2 zero float for ArtsTableLayout_load's pane-translate reset (SArtsVec2).
extern const float lbl_eu_80668658;
// .sdata2 floats for CMenuArtsSet_completeLayoutIn's scroll-in vector (stack temp fed to
// CScrollBar::InitRootPane).
extern const float lbl_eu_80668678;
extern const float lbl_eu_8066867C;

// Word blocks copied by the arts-table cursor drivers (ArtsTableCursor_refresh copies
// the 5-word lbl_eu_8050AC4C, ArtsSlotCursor_refresh the 9-word lbl_eu_8050ABB4) via
// MWCC's counted block-copy path (lwzu/stwu loop).
struct SArtsB88Block {
    u32 w[5];   // 0x14 bytes
};
extern const SArtsB88Block lbl_eu_8050AC4C;
struct SArts316F8Block {
    u32 w[9];   // 0x24 bytes
};
extern const SArts316F8Block lbl_eu_8050ABB4;
// 9-word label block for the ArtsBusyPane_refreshCursor cursor refresh (field_0x21 twin
// of ArtsSlotCursor_refresh's lbl_eu_8050ABB4).
extern const SArts316F8Block lbl_eu_8050ABD8;

// Per-character arts-list block copied onto the stack by artsFillTable
// (18 bytes: nine {id,count} pairs, indexed by character; declared as
// words+tail so MWCC emits the counted block-copy loop).
struct SArts320C0Entry {
    u8 id;
    u8 count;
};
struct SArts320C0Block {
    u16 w[9];   // nine {id,count} pairs; flat array so MWCC emits the
                // counted block-copy loop (2x8 bytes + lhz tail)
};
extern const SArts320C0Block lbl_eu_8050ABFC;
// Scrollbar list rebuild (artsFillTable tail).

// String-table lookup (artsAvailFlag) and learn-arts flag-grid query
// (artsAvailFlag); C-linkage retail symbols.
extern "C" bool ResTbox_IsCellBitSet(unsigned char*, int, int);
// Accumulated pane translate (AccumPaneWorldPos, defined in code_80135FDC.cpp)
// and pane translate setter (Cur_SetPaneTranslate, defined in CSysWin.cpp).

// Window-arm chain used by CMenuArtsSet_armSysWin: build a label string pair then
// drive the CSysWin sub-object. sysWinApplyKind is already declared in
// CSysWin.hpp as (CSysWin*, u8).
extern "C" void sysWinSetKindContent(CSysWin* self, char* str, u32 flag);
extern "C" void sysWinOpenPhase1(CSysWin* self);

// AnimRewindFrame is a flat C symbol (anim-frame check helper).

// CArtsInfo field setters / refresh (defined in CArtsInfo.cpp; C symbols).
extern "C" void CArtsInfo_SetCharId(CArtsInfo* self, u8 val);
extern "C" void CArtsInfo_SetField55(CArtsInfo* self, u8 val);
extern "C" void CArtsInfo_SetField56(CArtsInfo* self, u8 val);
extern "C" void CArtsInfo_SetLevel58(CArtsInfo* self, u16 val);
extern "C" void CArtsInfo_RefreshIfReady(CArtsInfo* self);
// Arts-element scan / character-data refresh (C symbols).
extern "C" void SysWinLog_ScanGrid(u8* self);
extern "C" void CtrlObjectParam_SyncParamFromActor(void*);
// BDAT string -> u16 (code_80135FDC.cpp). u32 3rd arg: retail passes the
// raw int (no clrlwi at the call site).
// sprintf (varargs): C declaration mirrors CArtsInfo.hpp (crclr cr1eq at
// the call site comes from the varargs ABI).
extern "C" int sprintf(char*, const char*, ...);

// ---------------------------------------------------------------------------
// artsMenuInput (per-frame input dispatch) callees
// ---------------------------------------------------------------------------
// Party-change notice gate (CUICfManager.cpp).
extern "C" int MenuTutorialIsCreated();
// Same-TU dispatch helpers (definitions below; C linkage binds the calls to
// the retail unmangled symbols).
extern "C" u8 CMenuArtsSet_isBusy(SArts33888* self);
extern "C" void CMenuArtsSet_armSysWin(CArtsList* self);
extern "C" void CMenuArtsSet_selectPrevChar(CArtsList* self);
extern "C" void artsStepAhead(CArtsList* self);
extern "C" void artsDriveMenu(CArtsList* self);
extern "C" void CMenuArtsSet_advanceIdle(CArtsList* self);
extern "C" void CMenuArtsSet_beginClose(CArtsList* self);
extern "C" void CMenuArtsSet_cursorUp(CArtsList* self);
extern "C" void CMenuArtsSet_cursorDown(CArtsList* self);
extern "C" void CMenuArtsSet_slotCursorUp(CArtsList* self);
extern "C" void CMenuArtsSet_slotCursorDown(CArtsList* self);
extern "C" void CMenuArtsSet_confirmSelect(CArtsList* self);
extern "C" u8 CMenuArtsSet_isWindowBusy(CArtsList* self);
// Scene/fade helpers (imports).
extern "C" int CMainMenu_GetInstancePtr();
extern "C" void ArtsInfo_SetReadyFlag();
// Title/help bar close + mode-set (CTitleAHelp.cpp; free C symbols - the
// member declarations in CTitleAHelp.hpp mangle differently).
extern "C" void beginClose(CTitleAHelp* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 mode);
// Member thunks called through their retail unmangled free-symbol forms by
// artsMenuInput (the member declarations mangle differently).
extern "C" void CMenuArtsSet_setArmedFlag(CArtsList* self, u8 val);
extern "C" u32 CMenuArtsSet_isSlotWindowEmpty(CArtsList* self);
extern "C" u32 CMenuArtsSet_isArtsInfoActive(CArtsList* self);