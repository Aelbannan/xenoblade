// Auto-scaffolded catalog TU for kyoshin/makecrystal/CModelDispMakeCrystal
// Replace stubs with high-level C/C++ during decomp.
#define IWORK_EVENT_INLINE_DTOR

#include "kyoshin/cf/CfBdat.hpp"
#include <types.h>
#include "kyoshin/code_8025FB10.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "monolib/lib/CLibLayout.hpp"

#include "kyoshin/CTaskGameApi.hpp"
#include "kyoshin/makecrystal/CModelDispMakeCrystal.hpp"
#include "kyoshin/makecrystal/CMenuMakeCrystal.hpp"
#include "kyoshin/makecrystal/CMCGetItemBox.hpp"
#include "kyoshin/makecrystal/CMCCrystalParamTable.hpp"


#include "kyoshin/makecrystal/CMCEffStart.hpp"
#include "kyoshin/makecrystal/CMCCylinderGauge.hpp"
#include "kyoshin/makecrystal/CMCCrystalList.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"
// code_80135FDC.hpp:254 declares ml::epsilon as extern u32 while
// monolib/math/FloatUtils.hpp (via CDeviceVI.hpp) declares it const float -
// MWCC 10563. This TU never references the symbol; rename the later copies.
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/core/CPadManager.hpp"
// CItem_initItemImplInstances has ONE abi in retail (item pointer in r3,
// dispatched on by the definition in src/kyoshin/cf/CItem.cpp). All headers
// now declare the unified pointer-arg form; TUs whose retail sites leave r3
// carrying the item from preceding code simply pass that live value (MWCC
// emits nothing when it is already in r3). No guard needed here anymore.
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CItemImpl.hpp"
#include "kyoshin/cf/CfPadData.hpp"

#include "monolib/util/MemManager.hpp"
#include "monolib/math/MTRand.hpp"
#include "monolib/work/CEventFile.hpp"
// Owning classes for the folded vtable slots (cf. CModelDispEquip.hpp):
// +0x48/+0x9C/+0xA8 model slots -> CScnItemModel::vfunc48/vfunc9C/vfuncA8
// (retail lbl_eu_8056DD70 words 18/39/42); +0xE0 act-param flush ->
// CActParamAnim::initAnimState; +0x74/+0x148 move slots ->
// cf::CfObject UVF9/UVF62; +0x08 param slot -> CResLookup::getResourceBase;
// +0x88 syswin slot -> CSysWin::loadSystemArc (retail lbl_eu_80536510).
// The canonical headers declare these symbols with a different arity/type
// than this TU's byte-exact retail decls (same scheme as
// CModelDispEquip.hpp): hide the header copies so the class definitions
// (CActParamAnim, CScnItemModel, CfObject, CResLookup, CSysWin) come through
// intact while this TU keeps its own prototypes.
// writeVec3f: unified to `void*` (decl matches the CActParamAnim.cpp definition)

#include "kyoshin/action/CActParamAnim.hpp"
#include "libs/monolib/src/scn/CScnItemModel.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/IResInfo.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CfEnumListHolder.hpp"

// Typified wave7: retail .data color table (8 rows of 3 bytes; struct + use
// site already exist in the hpp: `sCrystalColorTable->c`).
extern "C" {
__declspec(section ".data") __attribute__((used))
extern const CMCryColorRow sCrystalColorTable[8] = { // name-guess: 8 rows of per-character crystal-draw colors indexed by collected-flag byte
    {{0x19, 0x19, 0x32}}, {{0x50, 0x0A, 0x0A}},
    {{0x05, 0x5A, 0x05}}, {{0x3C, 0x0F, 0x19}},
    {{0x0F, 0x3C, 0x19}}, {{0x0A, 0x0A, 0x50}},
    {{0x0A, 0x50, 0x0A}}, {{0x05, 0x5A, 0x05}},
};
} // extern "C"

// forward declarations for scaffold thunk references
// Retail calls every state runner out-of-line via its unmangled C-linkage
// symbol; declare them extern "C" here so the dispatcher's bl targets match.
extern "C" {
void MakeCrystal_PickPassFail(CModelDispMakeCrystal*);
void MakeCrystal_StateSlotCheck(CModelDispMakeCrystal*);
void MakeCrystal_StateStartFx(CModelDispMakeCrystal*);
void MakeCrystal_TickRise(CModelDispMakeCrystal*);
void MakeCrystal_StateChargeStep(CModelDispMakeCrystal*);
void MakeCrystal_FinishRound(CModelDispMakeCrystal*);
void MakeCrystal_NopD3E4(CModelDispMakeCrystal*);
void MakeCrystal_StateChargeLoop(CModelDispMakeCrystal*);
void MakeCrystal_StateCrystalFull(CModelDispMakeCrystal*);
void MakeCrystal_StatePlacedDone(CModelDispMakeCrystal*);
void MakeCrystal_StateFailWait(CModelDispMakeCrystal*);
void MakeCrystal_StateDivideSel(CModelDispMakeCrystal*);
void MakeCrystal_StateCrystalChk(CModelDispMakeCrystal*);
void MakeCrystal_StateRankUp(CModelDispMakeCrystal*);
void MakeCrystal_StateDivideDone(CModelDispMakeCrystal*);
void MakeCrystal_StateCrystalDone(CModelDispMakeCrystal*);
void MakeCrystal_StateSuccessChk(CModelDispMakeCrystal*);
void MakeCrystal_NopDF84(CModelDispMakeCrystal*);
void MakeCrystal_DispatchTuneInput(CModelDispMakeCrystal*);
void MakeCrystal_FinalizeCylCount(CModelDispMakeCrystal*);
void MakeCrystal_StateCylDone(CModelDispMakeCrystal*);
void MakeCrystal_StateChargeFinal(CModelDispMakeCrystal*);
void MakeCrystal_MoveCursorPane(CModelDispMakeCrystal*);
void func_8021EF30(CModelDispMakeCrystal*);
void MakeCrystal_StateSysWinPrompt(CModelDispMakeCrystal*);
void MakeCrystal_ShowCursorPane(CModelDispMakeCrystal*);
void MakeCrystal_StateConfirmStep(CModelDispMakeCrystal*);
void MakeCrystal_StateItemMenuWait(CModelDispMakeCrystal*);
void func_8021F2D8(CModelDispMakeCrystal*);
void MakeCrystal_StateConfirmPrompt(CModelDispMakeCrystal*);
void MakeCrystal_CheckItemMenuDone(CModelDispMakeCrystal*);
void MakeCrystal_StateSuccessMenu(CModelDispMakeCrystal*);
void MakeCrystal_StateSuccessWait(CModelDispMakeCrystal*);
void MakeCrystal_OpenResultDialog(CModelDispMakeCrystal*);
void MakeCrystal_StateResultPrompt(CModelDispMakeCrystal*);
void MakeCrystal_ReadResultInput(CModelDispMakeCrystal*);
void MakeCrystal_StateConfirmDone(CModelDispMakeCrystal*);
}

void __dt__21CModelDispMakeCrystalFv(void*);
void MakeCrystal_BuildCrystalList(CModelDispMakeCrystal*, CMCrystalDispSub*);
void MakeCrystal_RebuildSubStruct(CModelDispMakeCrystal*, u8*);
// state-machine helpers defined later in this TU (retail C-linkage plain
// names; the extern "C" decl makes the later definitions inherit it, so
// call-site relocs bind to the bare retail names).
extern "C" void MakeCrystal_TickCharge(CModelDispMakeCrystal*);
extern "C" void MakeCrystal_StateChargeWait(CModelDispMakeCrystal*);
extern "C" void MakeCrystal_SetupDispBuffers(CModelDispMakeCrystal*);
// crystal-slot array element ctor/dtor (defined later in this TU; the ctor
// passes their addresses to __construct_array). C linkage keeps the
// __construct_array relocs bound to the bare retail names.

// --- external C-linkage helpers and global data for this TU ---
extern "C" {
void* writeVec3f(void*, f32, f32, f32);
float scaleByGlobal(float val);
void initCrystalData(unsigned char* p);
void crystalListDispatch(void* self);
void CrySupTickAnim(void* self);
void CrySupUnloadLayout(void* self);
void CrySupDrawLayout(void*, void*);
void tickAnimFrame(void*);
void MakeCrystal_DispatchState(CModelDispMakeCrystal*);
void releaseAnimObj(void*, void*);
bool Scn_IsAnimActiveOrNull(void*);
void* Scn_SetupAnim(void*, int, int, ...);
void simSetLeafFlag4000(void*, int);
extern "C" void simAttachModel(void*, void*);
void simMarkTree100000(void*, int);

void schedClearFlag15Update(void*);
void schedAttachChildSlot(void*, void*);
void EffSched_LookupB(void*, void*);
void* EffRoot_AddSchedReq(void*, void*, void*, int, int, int);
// Retail member-return types are int (callers `cmpwi r3,0` the raw reg).
// Mangled-name refs let us call them as int-returning without re-mangling.
int isFinished__11CMCEffStartFv(void* self);
int isFinished__13CMCEffCrystalFv(void* self);
int isFinished__14CMCEffCylinderFv(void*);
int isReady__16CMCCylinderGaugeFv(void*);
void MakeCrystal_AppendItemSteps(void*);
void MakeCrystal_TeardownUI(void*);
void CrySupSetTexByIndex(void*, u8);
void CrySupStartState(void*);
void CCharVoiceMan_FlushAll(void);
void MenuSnd_StopAllSlots_89C88(void);
void CTaskGame_stopVision(void);
void CUICfManager_queuePauseItemMenu(u32, u32, u32);
void deleteRegion__17UnkClass_8045F564Fv(void* self);
// --- target callees (retail C-linkage / mangled-name symbols) ---
void schedDetachChildSlot(void*, void*);
// func_8004B9D4: canonical CActParamAnim.hpp declaration.
void MakeCrystal_SpawnCrystalItem(void*, int, u8);
void openMCGetItemBox(void*, int, void*, u8);
void crystalListAlignSlot(void*, void*, u8);
void crystalListResetAnim3(void*);
void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
u32 getHandleMEM2__Q23mtl10MemManagerFv();
int isFinished__FPv(void*);
void moveMCGetItemBoxLeft(void*);
void forwardMCSubObj88(void*, int);
extern "C" void __dt__Q22cf17CActParamAnimGameFv(void* self, int flag);
void advanceMCGetItemBox(void*);
char lbl_eu_805090FC[];
int sprintf(char*, const char*, ...);
void startEnumObjects(void*, unsigned int, unsigned int);
void* getEntryAt(void*, unsigned int); // &slot -> +0x4 holds the move ptr
void* getCfObjectPc__FPQ22cf12CfObjectMove(void*);
void* CfRes_getPcGridEntry(short, int);
extern "C" int CfRes_isGridLoadIdle();
extern "C" void* CfRes_tryResolveToken(u32, void*);
short CfObjectMove_getSubB0FieldA(void*);
void CtrlObjectParam_GetSlotTableBase();
u32 bdatPcPartsRes(u32, s16, u32, u32);

void syncFieldData__Q22cf13CfGameManagerFv(u32, bool);
int Bdat_GetMaxRow_B1EC(void*);
extern "C" void CItem_initSlot(void* self, u32 family, u32 row, u16 kind);
void crystalListResetAnim4Play(void*);
// OnFileEvent constructor/helper imports (retail reloc names)
void __ct__16CMCCylinderGaugeFPQ34nw4r3lyt19ArcResourceAccessor(void*, nw4r::lyt::ArcResourceAccessor*);
void __ct__17CMCCrystalSupportFPQ34nw4r3lyt19ArcResourceAccessor(void*, nw4r::lyt::ArcResourceAccessor*);
void __ct__11CMCEffStartFPQ34nw4r3lyt19ArcResourceAccessor(void*, nw4r::lyt::ArcResourceAccessor*);
void __ct__11CMCEffUpRedFPQ34nw4r3lyt19ArcResourceAccessor(void*, nw4r::lyt::ArcResourceAccessor*);
void __ct__12CMCEffUpBlueFPQ34nw4r3lyt19ArcResourceAccessor(void*, nw4r::lyt::ArcResourceAccessor*);
void __ct__13CMCEffUpGreenFPQ34nw4r3lyt19ArcResourceAccessor(void*, nw4r::lyt::ArcResourceAccessor*);
void __ct__11CMCEffUpPrmFPQ34nw4r3lyt19ArcResourceAccessor(void*, nw4r::lyt::ArcResourceAccessor*);
void __ct__13CMCEffSuccessFPQ34nw4r3lyt19ArcResourceAccessor(void*, nw4r::lyt::ArcResourceAccessor*);
void __ct__13CMCEffFailureFPQ34nw4r3lyt19ArcResourceAccessor(void*, nw4r::lyt::ArcResourceAccessor*);
void __ct__13CMCEffCrystalFPQ34nw4r3lyt19ArcResourceAccessor(void*, nw4r::lyt::ArcResourceAccessor*);
void __ct__12CMCEffUpRankFPQ34nw4r3lyt19ArcResourceAccessor(void*, nw4r::lyt::ArcResourceAccessor*);
void __ct__12CMCEffDivideFPQ34nw4r3lyt19ArcResourceAccessor(void*, nw4r::lyt::ArcResourceAccessor*);
void __ct__14CMCEffCylinderFPQ34nw4r3lyt19ArcResourceAccessor(void*, nw4r::lyt::ArcResourceAccessor*);
void createRegion__17UnkClass_8045F564FiiPCci(void*, u32, int, const char*, int);
void __ct__14Class_8045F858FP17UnkClass_8045F564(void*, void*);
void setMemInitFlag__Q23mtl10MemManagerFb(bool);
void Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(void*, void*, const char*);
void init__16CMCCylinderGaugeFv(void*);
void CrySupBuildLayout(void*);
void init__11CMCEffUpPrmFv(void*);
void validateHeap__17UnkClass_8045F564Fv(void*);
void __dt__14Class_8045F858Fv(void*, int);
void EffSched_LookupA(void*);
void* initMcaFile(void*, unsigned int, const char*);
void attachAnimObj(void*, void*, void*, unsigned int);
void ModelDispEquip_SetActParamParent(void*, void*);
void crystalListClearSlots(void*, u8);
void __dt__80222984(void*);
void func_80222B14(void*, u8, u16, u16);
void crystalListUpdateSlot(void*, u8, u16, u8);
// CMCEffUpPrm::setNumber called with a u16 second arg: retail passes the raw
// halfword as the u8 param (callee reads only the low byte), so declaring the
// param as u16 here avoids MWCC's truncation mask at the call site.
void setNumber__11CMCEffUpPrmFUlUc(CMCEffUpPrm*, u32, u16);
void* Scn_FindCamItem(void*, int);
void ScnCamPointAt(void*, void*, void*);
s32 getInstance__Q22ml6MTRandFv();
s32 rand31__Q22ml6MTRandFv();
void* CtrlObjectParam_GetCharEntry(u32);
void MakeCrystal_PlaceCrystals(void*);
// CtrlRemote_SetSharedBit: retail flat name (cf. code_80135FDC.cpp:180).
extern "C" void CtrlRemote_SetSharedBit(u32 destination, u32 value);

void __dt__7CSysWinFv(void*, int);
void __dt__10CScrollBarFv(void*, int);
void __dt__14CMCEffCylinderFv(void*, int);
void __dt__12CMCEffDivideFv(void*, int);
void __dt__12CMCEffUpRankFv(void*, int);
void __dt__13CMCEffCrystalFv(void*, int);
void __dt__13CMCEffFailureFv(void*, int);
void __dt__13CMCEffSuccessFv(void*, int);
void __dt__11CMCEffUpPrmFv(void*, int);
void __dt__13CMCEffUpGreenFv(void*, int);
void __dt__12CMCEffUpBlueFv(void*, int);
void __dt__11CMCEffUpRedFv(void*, int);
void __dt__11CMCEffStartFv(void*, int);
void __dt__17CMCCrystalSupportFv(void*, int);
void __dt__14CMCCrystalListFv(void*, int);
void __dt__16CMCCylinderGaugeFv(void*, int);
void __dt__17UnkClass_8045F564Fv(void*, int);
}

extern u32 lbl_eu_8065FC18[];

// ===== typified split1 retail data (wave6) =====
// .rodata: filter triple + class name in ONE struct (MWCC 8-packs separate
// .rodata objects, which would pad the string to 0x10; retail packs it at
// 0xC). The [0x18] class field absorbs retail's 2-byte gap at 0x22.
// Only this TU references these labels, so member macros are link-safe.
struct Rodata_MCMCHead {
    u32 filt[3];
    char cls[0x18];
};
__declspec(section ".rodata") __attribute__((aligned(8))) __attribute__((used)) const struct Rodata_MCMCHead rodata_MCMCHead = {
    { 0x10, 0x100000, 0x200000 }, "CModelDispMakeCrystal"
};
#define lbl_eu_805090D8 (rodata_MCMCHead.filt)
#define lbl_eu_805090E4 (rodata_MCMCHead.cls)
// code offsets: +0 .chr, +0x11 .rec, +0x25 .arc, +0x38 MNU_skill, +0x42 help,
// +0x47 MNU_sysmes, +0x52 name, +0x57 MNU_crystal, +0x63 info, +0x68 itemType,
// +0xd1 format, +0xef ANM, +0xf3 class, +0x109 arc.
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_805090FC[0x114] =
    "obj/oj820002.chr\000eff/rec/eu/eu01.rec\000menu/jp/McData.arc\000MNU_skill\000help\000MNU_sysmes\000name\000MNU_crystal\000info\000itemType\000lower_E\000upper_E\000lower_D\000upper_D\000lower_C\000upper_C\000lower_B\000upper_B\000lower_A\000upper_A\000lower_S\000upper_S\000/chr/jp/pc/mu%02d%02d%02d.mca\000ANM\000CModelDispMakeCrystal\000arc\000";

// RTTI is emitted by MWCC from the real CModelDispMakeCrystal class.

// MWCC 4.3 (-ipa file) rules, verified by micro-probes against this TU's flags:
//  - scalar const-float/double reads fold into an anonymous first-use pool
//    (named defs dropped, pool order diverges) and const-int reads fold into
//    immediates; struct/array member reads at nonzero offset cost an extra
//    address instruction. So every code-referenced constant is its own
//    NON-CONST global: uses stay single SDA21 loads, emission follows
//    declaration order (hpp decls for A8..C0 updated to match).
//  - any all-zero object exiles to .sbss2 despite section+used (plain,
//    const, volatile, scalar and aggregate alike), and every aggregate
//    (struct or array) is 8-aligned in .sdata2, so no aggregate can own the
//    0x04 word. Zero words therefore live where MWCC leaves them: A4's 0.0
//    is the orphan pad MWCC emits before the 8-aligned arr08, the 0x43 gap
//    hides in E2's u8[2], and the unreferenced 0.0 tails ride in wider
//    const arrays. A4 itself is the pad, named by its retail label rather
//    than a second object in the pool tail (which landed in List's slice).
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_806684A0[1] = {1.0f};
#define lbl_eu_806684A0 (lbl_eu_806684A0[0])
__declspec(section ".sdata2") __attribute__((used)) const float arr08[1] __attribute__((aligned(8))) = {0.063f};
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_806684AC[1] = {-0.15f};
#define lbl_eu_806684AC (lbl_eu_806684AC[0])
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_806684B0[1] = {-1.659f};
#define lbl_eu_806684B0 (lbl_eu_806684B0[0])
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_806684B4[1] = {2.037f};
#define lbl_eu_806684B4 (lbl_eu_806684B4[0])
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_806684B8[1] = {-0.941f};
#define lbl_eu_806684B8 (lbl_eu_806684B8[0])
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_806684BC[1] = {70.0f};
#define lbl_eu_806684BC (lbl_eu_806684BC[0])
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_806684C0[1] = {-110.0f};
#define lbl_eu_806684C0 (lbl_eu_806684C0[0])
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_806684C4[1] = {-86.0f};
#define lbl_eu_806684C4 (lbl_eu_806684C4[0])
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_806684C8[1] = {72.0f};
#define lbl_eu_806684C8 (lbl_eu_806684C8[0])
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_806684CC[1] = {5.0f};
#define lbl_eu_806684CC (lbl_eu_806684CC[0])
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_806684D0[1] = {0.2f};
#define lbl_eu_806684D0 (lbl_eu_806684D0[0])
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_806684D4[1] = {50.0f};
#define lbl_eu_806684D4 (lbl_eu_806684D4[0])
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_806684D8[1] = {30.0f};
#define lbl_eu_806684D8 (lbl_eu_806684D8[0])
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_806684DC[1] = {20.0f};
#define lbl_eu_806684DC (lbl_eu_806684DC[0])
// E2's 0xFF + the 0x43 gap ride here: u8 aggregates will not pack at 0x42
// (4-aligned minimum), but this 4-byte struct lands at 0x40 with exact C
// packing e0@+0/e2@+2, so only E2's single red-function use costs extra.
__declspec(section ".sdata2") __attribute__((used)) struct Tag42 { u16 e0; u8 e2[2]; } tag42 = {0xFFFF, 0xFF, 0x00};
#define lbl_eu_806684E0 (tag42.e0)
#define lbl_eu_806684E2 (tag42.e2[0])
__declspec(section ".sdata2") __attribute__((used)) u32 lbl_eu_806684E4 = 0xFFFFFFFF;
__declspec(section ".sdata2") __attribute__((used)) u16 lbl_eu_806684E8 = 0xFFFF;
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_806684EC[1] = {0.01f};
#define lbl_eu_806684EC (lbl_eu_806684EC[0])
__declspec(section ".sdata2") __attribute__((aligned(8))) __attribute__((used)) double lbl_eu_806684F0 = 4503599627370496.0;
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_806684F8[1] = {100.0f};
#define lbl_eu_806684F8 (lbl_eu_806684F8[0])
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_806684FC[1] = {0.04f};
#define lbl_eu_806684FC (lbl_eu_806684FC[0])
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80668500[1] = {25.0f};
#define lbl_eu_80668500 (lbl_eu_80668500[0])
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80668504[1] = {1.5f};
#define lbl_eu_80668504 (lbl_eu_80668504[0])
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80668508[1] = {1.05f};
#define lbl_eu_80668508 (lbl_eu_80668508[0])
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_8066850C[1] = {-1.3f};
#define lbl_eu_8066850C (lbl_eu_8066850C[0])
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80668510[3] = {-20.0f, 0.0f, 0.0f};
__declspec(section ".sdata2") __attribute__((aligned(8))) __attribute__((used)) double lbl_eu_80668520 = 4503599627370496.0;
__declspec(section ".sdata2") __attribute__((used)) float lbl_eu_80668528 = 30.0f;
__declspec(section ".sdata2") __attribute__((used)) float lbl_eu_8066852C = 0.01f;
__declspec(section ".sdata2") __attribute__((aligned(8))) __attribute__((used)) double lbl_eu_80668530 = 4503601774854144.0;
__declspec(section ".sdata2") __attribute__((used)) const float arr98[3] = {1.0f, 0.0f, 0.0f};
// 0x806684A4 is the alignment pad before the 8-aligned arr08, emitted by
// this TU already; no second object may own it (the old tail copy landed in
// CMCCrystalList's slice at 0x80668548). Name the pad, don't duplicate it.
extern const f32 lbl_eu_806684A4;
#define lbl_eu_806684A8 (arr08[0])
#define lbl_eu_80668518 (lbl_eu_80668510[2])
#define lbl_eu_80668538 (arr98)
#define lbl_eu_80668540 (arr98[2])

// .bss 2x0xC (crystal vec work) + .sbss crystal-state bytes (hpp declares u8[8]).
// Plain zero-init: MWCC sorts by size into .bss/.sbss like the old absorbs.
// name-guess: sinit writes two Vec3f; RebuildSubStruct copies them into the
// sim leaf act data at +0x0 and +0xC (no retail name in any region).
__attribute__((aligned(8))) __attribute__((used)) u8 sCrystalLeafVecA[0xC];
__attribute__((used)) u8 sCrystalLeafVecB[0xC];
__attribute__((aligned(8))) __attribute__((used)) u8 sCrystalProbState[8]; // name-guess: crystal-probability state bytes at sbss 0x80664718





// (was CMCC8CFn: no-arg virtual dispatch at vtable offset +0x10 on the
// CMCEff family (this+0xc8c/0xca4/0xcbc/0xcd4/0xd78/0xdcc/0xde4). Retail
// tables (lbl_eu_80536114 et al.) carry startInAnim (Start/Failure) or play
// (UpRed/UpBlue/UpGreen/UpRank/Divide) at the +0x10 word; startInAnim is now
// a real virtual on CMCEffStart, so call sites dispatch through it and land
// on the retail word for every leaf.)

// (was CMCSysWinFn: virtual dispatch at vtable offset +0x88, the CSysWin call
// in MakeCrystal_InitFiles on this+0xe78. Retail table lbl_eu_80536510 word 34 is
// CSysWin::loadSystemArc (def: sysWinInitFileRead); the call site now uses the
// owning class directly.)

// Entry at this+0x13c0 (stride 0x34); a 4-bit type at bits 12-15.
struct CMCStep {
    u32 _pad0 : 12;
    u32 type : 4;
    u32 _pad16 : 16;
};

// (was CMCVtE0: virtual dispatch at vtable offset +0xE0, the flush call in
// MakeCrystal_ReleaseAnimObj on the inline object at the sub-struct +0x8. Retail vtable
// word 54 is CActParamAnim::initAnimState (cf. CModelDispEquip.hpp); call
// sites now use the owning class directly.)

// Real constructor: every member up to +0x13BF is a typed member, so MWCC
// emits the member ctors from this init list in declaration (= retail store)
// order, and the exception table records them like retail does.
CModelDispMakeCrystal::CModelDispMakeCrystal(CScn* scene)
    : mScene(scene),
      mField10(0),
      mField14(0),
      mFileHandle18(0),
      mField1C(0),
      mField20(0),
      mFileHandle24(0),
      mField28(0),
      mFileHandle3C(0),
      mField40(0),
      field_BDC(0),
      field_BDD(0),
      field_BE0(1.0f),
      field_BE4(0.0f),
      field_BE8(1),
      field_BE9(1),
      mGauge(0),
      mCrystalList(),
      mSupport(0),
      mEffStart(0),
      mEffUpRed(0),
      mEffUpBlue(0),
      mEffUpGreen(0),
      mEffUpPrm(0),
      mEffSuccess(0),
      mEffFailure(0),
      mEffCrystal(0),
      mEffUpRank(0),
      mEffDivide(0),
      mEffCylinder(0),
      mEntries(nullptr),
      mCurE20(0),
      mScrollBar(0),
      mSysWin(0),
      mCurEB4(0),
      mItemBox(),
      mCrystalNum(0) {
    u8* base = reinterpret_cast<u8*>(this);

    // Stack scratch (declaration order fixes the retail stack layout:
    // sub@0x8, group2@0x18, group1@0x30); the widget re-inits below use
    // compiler-managed temporaries.
    u8 sub[0xc];       // initCrystalSubStruct scratch
    CMCryVec3 group1[2];
    CMCryVec3 group2[2];

    // --- zero the 128 step slots (stride 0x34 at +0x13c0) ---
    // Pointer-walk do-while with the increment inside the condition: MWCC
    // keeps this rolled (cmplw/blt) instead of unrolling it.
    u8* p = base + 0x13c0;
    do {
        *(u16*)(p + 4) = 0;
        *(u32*)p = 0;
    } while ((p += 0x34) < base + 0x2dc0);

    // --- cursor-state fields (second zero constant: retail r30) ---
    u32 zero2 = 0;
    base[0x2dc0] = zero2;
    base[0x2dc1] = zero2;
    base[0x2dc2] = zero2;
    base[0x2dc3] = zero2;
    *reinterpret_cast<u16*>(base + 0x2dc4) = zero2;
    base[0x2dc6] = zero2;
    base[0x2dc7] = zero2;
    base[0x2dc8] = zero2;
    *reinterpret_cast<f32*>(base + 0x2dcc) = 0.0f;
    base[0x2dd0] = zero2;
    base[0x2dd1] = zero2;
    base[0x2dd2] = zero2;
    base[0x2dd3] = zero2;
    base[0x2dd4] = zero2;
    base[0x2dd5] = zero2;

    // --- crystal-position groups (two VEC3 pairs) ---
    writeVec3f(&group1[0], lbl_eu_806684A8, lbl_eu_806684AC, lbl_eu_806684B0);
    f32 g1y = lbl_eu_806684BC * ml::deg2rad;
    f32 g2y = lbl_eu_806684C0 * ml::deg2rad;
    group1[1].x = lbl_eu_806684B4;
    group1[1].y = lbl_eu_806684AC;
    group1[1].z = lbl_eu_806684B8;
    group2[0].x = 0.0f;
    group2[0].y = g1y;
    group2[0].z = 0.0f;
    writeVec3f(&group2[1], 0.0f, g2y, 0.0f);

    // --- per-slot init (2 slots, stride 0x5cc at +0x44) ---
    // Retail colors: gp1=r23, gp2=r24, one=r25, eight=r26, sb=r28,
    // slot=r29, zero2=r30, minus1=r31, j=r22. Declaration/creation order
    // tuned to steer MWCC's coloring toward that assignment.
    s8 initState = -1;
    u32 nVals = 8;
    CMCryVec3* gp2 = group2;
    u8 builtFlag = 1;
    CMCryVec3* gp1 = group1;
    for (u8 slot = 0; slot < 2; slot++) {
        u8* s = base + (u32)(u8)slot * 0x5cc;
        u8* sb = s + 0x44;   // CMCrystalDispSub base (retail r28)
        *reinterpret_cast<u32*>(s + 0x44) = zero2;
        *reinterpret_cast<u32*>(s + 0x48) = zero2;
        for (u32 i = 0; i < nVals; i++) {
            *reinterpret_cast<u32*>(sb + 0x544 + ((u32)(u8)i << 2)) = zero2;
        }
        for (u8 j = 0; j < 6; j++) {
            reinterpret_cast<CModelDispMakeCrystal*>(sub)->initCrystalSubStruct();
            u8* d = sb + 0x564 + (u32)(u8)j * 0xc;
            *reinterpret_cast<u32*>(d + 0) = *reinterpret_cast<u32*>(sub + 0);
            *reinterpret_cast<u32*>(d + 4) = *reinterpret_cast<u32*>(sub + 4);
            d[8] = sub[8];
        }
        *reinterpret_cast<s8*>(sb + 0x5ac) = initState;
        *reinterpret_cast<u32*>(sb + 0x5b0) = *reinterpret_cast<u32*>(&gp1[slot].x);
        *reinterpret_cast<u32*>(sb + 0x5b4) = *reinterpret_cast<u32*>(&gp1[slot].y);
        *reinterpret_cast<u32*>(sb + 0x5b8) = *reinterpret_cast<u32*>(&gp1[slot].z);
        *reinterpret_cast<u32*>(sb + 0x5bc) = *reinterpret_cast<u32*>(&gp2[slot].x);
        *reinterpret_cast<u32*>(sb + 0x5c0) = *reinterpret_cast<u32*>(&gp2[slot].y);
        *reinterpret_cast<u32*>(sb + 0x5c4) = *reinterpret_cast<u32*>(&gp2[slot].z);
        sb[0x5c8] = builtFlag;
    }

    // --- re-init the embedded crystal list from a default temporary ---
    mCrystalList = CMCCrystalList();

    // --- re-init the embedded scroll bar from a direction-0 temporary ---
    mScrollBar = CScrollBar(0);

    // --- re-init the embedded system window from a kind-2 temporary ---
    mSysWin = CSysWin(2);

    // --- re-init the embedded item box from a default temporary ---
    mItemBox = CMCGetItemBox();
}

// (was CMCVt48: virtual dispatch at vtable offset +0x48 taking one float
// arg, on this+0x20 and the per-slot objects. Retail table lbl_eu_8056DD70
// word 18 is CScnItemModel::vfunc48 (cf. CModelDispEquip.hpp); call sites
// now use the owning class directly.)

void CModelDispMakeCrystal::initCrystalSubStruct() {
    ((CMCrySubBuf*)this)->field_00 = 0;
    ((CMCrySubBuf*)this)->field_04 = 0;
    ((CMCrySubBuf*)this)->field_08 = 0;
}

// Retail 0x8021E348: element constructor of the two 0x5CC-stride crystal
// display slots (the owner's `CMCrystalDispSub mSlots[2]` member makes MWCC
// emit __construct_array over these). Builds the inline CActParamAnimGame at
// +0x8, then zeroes the six 0xC-byte crystal sub-buffers at +0x564..+0x5ac.
// NOTE: -O4,p static cap - retail's `stmw r29` (3-reg save) needs -O4,s
// (MWCC_CASES §16); body/loop otherwise byte-identical.
CMCrystalDispSub::CMCrystalDispSub()
{
    u8* base = reinterpret_cast<u8*>(this);
    __ct__Q22cf17CActParamAnimGameFv(base + 0x8);
    u8* p = base + 0x564;
    do {
        reinterpret_cast<CModelDispMakeCrystal*>(p)->initCrystalSubStruct();
        p += 0xc;
    } while (p < base + 0x5ac);
}

// Retail 0x8021E398: element destructor of the two crystal display slots
// (row of __destroy_arr) - destroys the inline CActParamAnimGame at +0x8 and
// frees the object when the flag is positive. Retail frame is stmw/lmw
// r30+r31 (-O4,s shape).
CMCrystalDispSub::~CMCrystalDispSub()
{
    __dt__Q22cf17CActParamAnimGameFv(reinterpret_cast<u8*>(this) + 8, -1);
}

// Retail frame is stmw r30 (-O4,s shape); body otherwise identical.
// Every member is a real typed member, so MWCC emits the member destructors
// (reverse declaration order == retail's destruction order) and the
// __destroy_arr row for the two crystal display slots.
CModelDispMakeCrystal::~CModelDispMakeCrystal() {}

void MakeCrystal_InitFiles(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    *reinterpret_cast<void**>(base + 0x24) =
        readFile__11CDeviceFileFUlPCcP10IWorkEventii(
            getHandleMEM2__Q23mtl10MemManagerFv(), &lbl_eu_805090FC[0], self, 0, 0);
    *reinterpret_cast<void**>(base + 0x18) =
        readFile__11CDeviceFileFUlPCcP10IWorkEventii(
            getHandleMEM2__Q23mtl10MemManagerFv(), &lbl_eu_805090FC[0x11], self, 0, 0);
    *reinterpret_cast<void**>(base + 0x3c) =
        readFile__11CDeviceFileFUlPCcP10IWorkEventii(
            getHandleMEM2__Q23mtl10MemManagerFv(), &lbl_eu_805090FC[0x25], self, 0, 0);
    crystalListLoadFiles(reinterpret_cast<CMCCrystalList*>(base + 0xc18));
    reinterpret_cast<CScrollBar*>(base + 0xe38)->loadLayoutArc();
    reinterpret_cast<CSysWin*>(base + 0xe78)->loadSystemArc();
    base[0xbe9] = 0;
    base[0xbdd] = 0;
    base[0x2dd2] = 0;
    base[0x2dd4] = 0;
}

// Retail 0x8021E5F8: per-frame state-machine driver - dispatch the crystal
// charge state, then tick every makecrystal UI sub-object.
void MakeCrystal_TickAll(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    switch (base[0xbdc]) {
    case 0:
        MakeCrystal_SetupDispBuffers(self);
        break;
    case 1:
        MakeCrystal_TickCharge(self);
        break;
    case 2:
        MakeCrystal_StateChargeWait(self);
        break;
    case 3:
        base[0xbdc] = 0;
        base[0x2dc3] = 1;
        break;
    }
    reinterpret_cast<CMCCylinderGauge*>(base + 0xbec)->update();
    crystalListDispatch(base + 0xc18);
    CrySupTickAnim(base + 0xc70);
    reinterpret_cast<CMCEffStart*>(base + 0xc8c)->update();
    reinterpret_cast<CMCEffStart*>(base + 0xca4)->update();
    reinterpret_cast<CMCEffStart*>(base + 0xcbc)->update();
    reinterpret_cast<CMCEffStart*>(base + 0xcd4)->update();
    reinterpret_cast<CMCEffUpPrm*>(base + 0xcec)->update();
    reinterpret_cast<CMCEffSuccess*>(base + 0xd5c)->update();
    reinterpret_cast<CMCEffStart*>(base + 0xd78)->update();
    reinterpret_cast<CMCEffCrystal*>(base + 0xd90)->update();
    reinterpret_cast<CMCEffStart*>(base + 0xdcc)->update();
    reinterpret_cast<CMCEffStart*>(base + 0xde4)->update();
    reinterpret_cast<CMCEffCylinder*>(base + 0xdfc)->update();
    ((CBaseCur*)(base + 0xe20))->Update();
    reinterpret_cast<CScrollBar*>(base + 0xe38)->UpdateDispatch();
    reinterpret_cast<CSysWin*>(base + 0xe78)->dispatchPhase();
    ((CBaseCur*)(base + 0xeb4))->Update();
    updateMCGetItemBox((CMCGetItemBox*)(base + 0xecc));
}

void MakeCrystal_CleanupFiles(CModelDispMakeCrystal* self) {
    u8* base = reinterpret_cast<u8*>(self);
    CDeviceVI::waitForDrawDone();
    MakeCrystal_TeardownUI(self);
    reinterpret_cast<CSysWin*>(base + 0xe78)->termLayout();
    reinterpret_cast<CBaseCur*>(base + 0xeb4)->cleanup();
    teardownMCGetItemBox((CMCGetItemBox*)(base + 0xecc));
    closeFileHandle(reinterpret_cast<CFileHandle**>(base + 0x3c));
    releaseArcResourceAccessor(*reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40));
    *reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40) = nullptr;
    deleteRegion__17UnkClass_8045F564Fv(base + 0x2c);
}


// Retail 0x8021E780: draw every makecrystal UI sub-object.
// [stmw] r30 frame (see MakeCrystal_StateCrystalChk note).
void MakeCrystal_DrawAll(CModelDispMakeCrystal* self, nw4r::lyt::DrawInfo* drawInfo)
{
    u8* base = reinterpret_cast<u8*>(self);
    reinterpret_cast<CMCCrystalList*>(base + 0xc18)->crystalListDrawIfActive(drawInfo);
    reinterpret_cast<CMCCylinderGauge*>(base + 0xbec)->draw(drawInfo);
    CrySupDrawLayout(base + 0xc70, drawInfo);
    draw(reinterpret_cast<CMCEffStart*>(base + 0xc8c), drawInfo);
    draw(reinterpret_cast<CMCEffStart*>(base + 0xca4), drawInfo);
    draw(reinterpret_cast<CMCEffStart*>(base + 0xcbc), drawInfo);
    draw(reinterpret_cast<CMCEffStart*>(base + 0xcd4), drawInfo);
    reinterpret_cast<CMCEffUpPrm*>(base + 0xcec)->draw(drawInfo);
    reinterpret_cast<CMCEffSuccess*>(base + 0xd5c)->draw(drawInfo);
    draw(reinterpret_cast<CMCEffStart*>(base + 0xd78), drawInfo);
    reinterpret_cast<CMCEffCrystal*>(base + 0xd90)->draw(drawInfo);
    draw(reinterpret_cast<CMCEffStart*>(base + 0xdcc), drawInfo);
    draw(reinterpret_cast<CMCEffStart*>(base + 0xde4), drawInfo);
    reinterpret_cast<CMCEffCylinder*>(base + 0xdfc)->draw(drawInfo);
    ((CBaseCur*)(base + 0xe20))->DrawLayout(drawInfo);
    reinterpret_cast<CScrollBar*>(base + 0xe38)->draw(drawInfo);
    drawMCGetItemBox((CMCGetItemBox*)(base + 0xecc), drawInfo);
    reinterpret_cast<CSysWin*>(base + 0xe78)->drawLayout(reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo));
    ((CBaseCur*)(base + 0xeb4))->DrawLayout(drawInfo);
}

u32 CModelDispMakeCrystal::getCrystalStateA() { return reinterpret_cast<CModelDispMakeCrystalFull*>(this)->field_BE8; }

// Retail 0x8021E894: "ready to advance" gate - every crystal display slot
// must be built, plus the crystal-list / scrollbar / syswin gates, then the
// crystal-box file state machine answers.
int MakeCrystal_CheckReady(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    // Single-counter constant-trip loop so -O4,s uses the CTR (bdnz) form.
    for (u8 i = 0; i < 2; i++) {
        if (base[(u32)i * 0x5cc + 0x60c] == 0) return 0;
    }
    if (base[0xbe9] == 0) return 0;
    if (*reinterpret_cast<u32*>(base + 0x40) == 0) return 0;
    if (reinterpret_cast<CMCCrystalList*>(base + 0xc18)->crystalListGetDataFlag() == 0) return 0;
    if (reinterpret_cast<CScrollBar*>(base + 0xe38)->isVisible() == 0) return 0;
    if (reinterpret_cast<CSysWin*>(base + 0xe78)->isReady() == 0) return 0;
    return isMCGetItemBoxActive((CMCGetItemBox*)(base + 0xecc));
}

u32 CModelDispMakeCrystal::getCrystalStateB() { return reinterpret_cast<CModelDispMakeCrystalFull*>(this)->field_2DC3; }

u32 CModelDispMakeCrystal::getCrystalStateC() { return reinterpret_cast<CModelDispMakeCrystalFull*>(this)->field_2DD2; }

void MakeCrystal_StartCharge(CModelDispMakeCrystal* self)
{
    // Single live pointer (base) so MWCC keeps the small -O4,s frame with
    // two saved regs like retail; the table base materializes after the
    // second char-state call so the lis/addi pair lands after it.
    u8* base = reinterpret_cast<u8*>(self);
    if (base[0xbdc]) return;
    base[0xbdc] = 1;
    base[0xbe8] = 0;
    int r0 = GetCollectedFlagByte(base[0x5f0]);
    if (r0 == 3 || r0 == 8) {
        reinterpret_cast<CMCCylinderGauge*>(base + 0xbec)->setLevel(3);
    } else {
        reinterpret_cast<CMCCylinderGauge*>(base + 0xbec)->setLevel(1);
    }
    reinterpret_cast<CMCCylinderGauge*>(base + 0xbec)->start();
    crystalListState0To1(reinterpret_cast<CMCCrystalList*>(base + 0xc18));
    u8* entries = reinterpret_cast<CModelDispMakeCrystal*>(base)->mEntries;
    nw4r::math::VEC3 v;
    v.x = lbl_eu_806684C4;
    v.y = lbl_eu_806684C8;
    v.z = 0.0f;
    reinterpret_cast<CScrollBar*>(base + 0xe38)->InitRootPane(reinterpret_cast<const nw4r::math::VEC3*>(&v));
    reinterpret_cast<CScrollBar*>(base + 0xe38)->UpdateThumb(8, entries[0]);
    reinterpret_cast<CScrollBar*>(base + 0xe38)->requestScrollIn();
    MakeCrystal_ResetSlotList(reinterpret_cast<CModelDispMakeCrystal*>(base), 0);
    u8 bbc = base[0xbbc];
    GetCollectedFlagByte(base[0x5f0]);
    // Table lookup: byte-array indexing through a named offset so MWCC
    // keeps the scaled offset in a register (mulli) for both loads.
    int off = ((int)GetCollectedFlagByte(bbc) - 1) * 3;
    const u8* tbl = sCrystalColorTable->c;
    u8 c0 = tbl[off];
    u8 c1 = tbl[off + 1];
    u8 c2 = tbl[off + 2];
    sCrystalProbState[3] = 0;
    sCrystalProbState[0] = c0;
    sCrystalProbState[1] = c1;
    sCrystalProbState[2] = c2;
    sCrystalProbState[5] = 1;
    MakeCrystal_RefreshSlotEntries(reinterpret_cast<CModelDispMakeCrystal*>(base));
}

void MakeCrystal_TickCharge(CModelDispMakeCrystal* self)
{
    nw4r::math::VEC3 v;
    u8* base = reinterpret_cast<u8*>(self);
    f32* fbe4 = reinterpret_cast<f32*>(base + 0xbe4);
    f32* fbe0 = reinterpret_cast<f32*>(base + 0xbe0);
    f32 v4 = (*fbe4 += 1.0f);
    if (v4 >= lbl_eu_806684CC) {
        f32 v0 = *fbe0 - lbl_eu_806684D0;
        *fbe0 = v0;
        if (v0 < 0.0f) {
            *fbe0 = 0.0f;
            base[0xbdc] = 2;
            base[0xbe8] = 1;
            for (u8 i = 0; i < 8; i++) {
                crystalListAlignSlot(&v, base + 0xc18, i);
                reinterpret_cast<CMCEffUpPrm*>(base + 0xcec)->setTranslate(i, v);
            }
            playUISound(0x3d);
        }
        void* p20 = *reinterpret_cast<void**>(base + 0x20);
        if (p20) {
            reinterpret_cast<CScnItemModel*>(p20)->vfunc48(*fbe0);
        }
        for (u8 i = 0; i < 2; i++) {
            void* p = *reinterpret_cast<void**>(base + (u32)i * 0x5cc + 0x44);
            if (p) {
                reinterpret_cast<CScnItemModel*>(p)->vfunc48(*fbe0);
            }
        }
    }
}

void MakeCrystal_StateChargeWait(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (base[0xbe9] == 0) {
        MakeCrystal_SetupDispBuffers(self);
    } else {
        for (u8 i = 0; i < 2; i++) {
            u8* p = base + (u32)i * 0x5cc;
            if (*reinterpret_cast<u32*>(p + 0x44)) {
                tickAnimFrame(p + 0x4c);
            }
        }
    }
    // Input-trigger check: the bit differs between classic controller and
    // Wiimote/Nunchuk pad layouts.
    CPad* pad = cf::CfGameManager::getCurrentPad();
    u32 bit;
    // Classic-controller vs Wiimote/Nunchuk trigger bit.
    if (cf::CfGameManager::isClassicController(-1) != 0)
        bit = (pad->mTurboPressButtonFlags >> 21) & 1;
    else
        bit = (pad->mTurboPressButtonFlags >> 4) & 1;
    if (bit) {
        *reinterpret_cast<f32*>(base + 0x2dcc) = lbl_eu_806684D4;
        base[0x2dd5] = 1;
    }
    MakeCrystal_DispatchState(self);
}

// -O4,s frame: retail saves r30/r31 with stmw/lmw.
void MakeCrystal_StateSlotCheck(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    for (u32 i = 0; i < 2; i++) {
        u32 off = (u8)i * 0x5cc;
        if (*reinterpret_cast<u32*>(base + off + 0x44)) {
            // Reload the slot pointer (CSE'd with the test load) so MWCC
            // emits retail's dead duplicate beq before the call setup.
            void* m = *reinterpret_cast<void**>(base + off + 0x44);
            if (m != 0) {
                func_8004B9D4(base + off + 0x4c, 0x22, 0, -1, 0);
            }
        }
    }
    reinterpret_cast<CMCEffStart*>(base + 0xc8c)->startInAnim();
    base[0xbdd] = 0x1;
    MakeCrystal_SpawnCrystalItem(self, 0, GetCollectedFlagByte(base[0xbbc]));
}

void MakeCrystal_StateStartFx(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    f32* vel = reinterpret_cast<f32*>(base + 0x2dcc);
    *vel += lbl_eu_806684A0;
    if (isFinished__11CMCEffStartFv(base + 0xc8c)) {
        if (!(*vel < lbl_eu_806684D8)) {
            *vel = lbl_eu_806684A4;
            for (u8 i = 0; i < 2; i++) {
                u8* s = base + (u32)i * 0x5cc;
                if (*reinterpret_cast<u32*>(s + 0x44)) {
                    // Reloading the slot pointer here (CSE'd with the test
                    // load) makes MWCC emit retail's dead duplicate beq
                    // before the 5-arg call setup.
                    void* m = *reinterpret_cast<void**>(s + 0x44);
                    if (m != 0) {
                        func_8004B9D4(s + 0x4c, 0x21, 0, -1, 0);
                    }
                }
            }
            base[0xbdd] = 0x2;
            MakeCrystal_SpawnCrystalItem(self, 0, GetCollectedFlagByte(base[0x5f0]));
        }
    }
}

void MakeCrystal_TickRise(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    f32* f2dcc = reinterpret_cast<f32*>(base + 0x2dcc);
    f32 v = *f2dcc + lbl_eu_806684A0;
    *f2dcc = v;
    if (v < lbl_eu_806684D8) return;
    *f2dcc = lbl_eu_806684A4;
    // Reload the 0x610 flag (CSE'd with the test load) to make MWCC emit
    // retail's dead duplicate beq before the 5-arg call setup.
    if (*reinterpret_cast<u32*>(base + 0x610)) {
        void* m = *reinterpret_cast<void**>(base + 0x610);
        if (m != 0) {
            func_8004B9D4(base + 0x618, 0x23, 0, -1, 0);
        }
    }
    // Crystal-probability state (sbss, shared with MakeCrystal_StartCharge). Referenced
    // directly so MWCC re-materializes the @sda21 address at each use instead
    // of holding it in a callee-saved register (retail reloads it too).
    if (sCrystalProbState[5]) {
        u8 n31 = sCrystalProbState[3];
        u8 n29 = sCrystalProbState[1];
        u8 n28 = sCrystalProbState[0];
        s8 roll = (s8)((s32)ml::MTRand::getInstance()->rand31() % 100);
        s8 new3;
        if (roll < (s8)n28) {
            new3 = 0;
        } else {
            // bin the roll by cumulative thresholds; yields 2 or 3
            new3 = (s8)(2 + (roll >= (s8)((s8)n28 + (s8)n29)));
        }
        sCrystalProbState[3] = (u8)new3;
        if ((u8)new3 == n31) {
            sCrystalProbState[4]++;
        } else {
            sCrystalProbState[4] = 0;
        }
    }
    if (*reinterpret_cast<u32*>(base + 0x10)) {
        u8* p = reinterpret_cast<u8*>(*reinterpret_cast<u32*>(base + 0x10));
        switch (sCrystalProbState[3]) {
        case 0:
            if (p[0x59] != 1) p[0x59] = 1;
            break;
        case 1:
            if (p[0x59] != 3) p[0x59] = 3;
            break;
        case 2:
            if (p[0x59] != 2) p[0x59] = 2;
            break;
        }
    }
    base[0xbdd] = 3;
    MakeCrystal_SpawnCrystalItem(self, 1, GetCollectedFlagByte(base[0xbbc]));
}

void MakeCrystal_StateChargeStep(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    f32* vel = reinterpret_cast<f32*>(base + 0x2dcc);
    f32 v = (*vel += lbl_eu_806684A0);
    if (v < lbl_eu_806684DC) return;
    u32 f44 = *reinterpret_cast<u32*>(base + 0x44);
    *vel = lbl_eu_806684A4;
    // Nested reload reproduces retail's dead duplicate beq before the call.
    if (f44) {
        void* m = *reinterpret_cast<void**>(base + 0x44);
        if (m != 0) {
            func_8004B9D4(base + 0x4c, 0x24, 0, -1, 0);
        }
    }
    base[0xbdd] = 0x4;
    MakeCrystal_SpawnCrystalItem(self, 1, GetCollectedFlagByte(base[0x5f0]));
}

// (was CMCVtA8: virtual dispatch at vtable offset +0xA8 returning a word.
// Retail table lbl_eu_8056DD70 word 42 is CScnItemModel::vfuncA8 (cf.
// CModelDispEquip.hpp); call sites now use the owning class directly.)

// Retail symbol is the unmangled C-linkage "setCrystalPosEntry".
extern "C" void setCrystalPosEntry(
    void* entries, unsigned short index, short a, short b);

// Retail 0x8021F058: crystal-result state ticker.
// on expiry resets it, refreshes the list (MakeCrystal_PlaceCrystals), plays the result
// jingle (0x79/0x7a/0x7b) and either latches "awaiting confirm" (+0x2dd0 = 1)
// or rolls the RNG gate to start the next round: phase flag from the battle
// mode check, rand31()%128 vs threshold at +0x2dd3, then swaps in the success
// arc entry (lbl_eu_8065FC18), stops the old sound object and starts a new
// one through the file-state machine, releases both slot substructs
// (0x25 release), plays 0x7c/0x7d and re-enters the state machine at 3.
void MakeCrystal_FinishRound(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    f32* vel = reinterpret_cast<f32*>(base + 0x2dcc);
    *vel += 1.0f;
    if (*vel < lbl_eu_806684DC) return;
    *vel = 0.0f;
    MakeCrystal_PlaceCrystals(self);
    incrementEventCounter(0x79);
    incrementEventCounter(0x7a);
    incrementEventCounter(0x7b);
    if (base[0x2dd0] == 0 && (GetCollectedFlagByte(base[0x5f0]) & 0xFF) == 6) {
        base[0x2dd0] = 1;
        return;
    }
    base[0x2dd0] = 0;
    u8 chArg = base[0x5f0];
    base[0xbdd] = 6;
    // Named call-result temp so the phase init lands after the extract.
    int cres = (GetCollectedFlagByte(chArg) & 0xFF);
    u8 phase = 0;
    if (cres == 1) phase = 1;
    s32 rv = ml::MTRand::getInstance()->rand31();
    int q = rv / 128;
    if ((s8)(rv - q * 128) > (int)(phase + base[0x2dd3])) return;
        base[0xbdd] = 5;
        playUISound(0x96);
        void* arc = &lbl_eu_8065FC18[0];
        if (arc != nullptr) {
            if (*reinterpret_cast<u32*>(base + 0x14) != 0) {
                schedClearFlag15Update(*reinterpret_cast<void**>(base + 0x14));
            }
            void* e = EffRoot_AddSchedReq(arc, *reinterpret_cast<void**>(base + 0x1c),
                                    *reinterpret_cast<void**>(base + 0xc), 1, 1, 1);
            *reinterpret_cast<void**>(base + 0x14) = e;
            if (e != nullptr) {
                void* arg = reinterpret_cast<void*>(self);
                if (self != nullptr) {
                    arg = reinterpret_cast<void*>(base + 0x8);
                }
                schedAttachChildSlot(e, arg);
                u32 r = reinterpret_cast<CScnItemModel*>(*reinterpret_cast<void**>(base + 0x20))->vfuncA8();
                *reinterpret_cast<u32*>(reinterpret_cast<u8*>(*reinterpret_cast<void**>(base + 0x14)) + 0x14) = r;
            }
        }
        for (u8 i = 0; i < 2; i++) {
            u8* s = base + (u32)i * 0x5cc;
            // Reload the slot pointer (CSE'd with the test load) so MWCC
            // emits retail's dead duplicate beq before the call setup.
            if (*reinterpret_cast<u32*>(s + 0x44)) {
                void* m = *reinterpret_cast<void**>(s + 0x44);
                if (m != 0) {
                    func_8004B9D4(s + 0x4c, 0x25, 0, -1, 0);
                }
            }
        }
        incrementEventCounter(0x7c);
        incrementEventCounter(0x7d);
        MakeCrystal_SpawnCrystalItem(self, 3, 0);
}

// -O4,s frame: retail keeps the plain divw for the %10 (no magic-multiply).
void MakeCrystal_NopD3E4(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    f32* f2dcc = reinterpret_cast<f32*>(base + 0x2dcc);
    f32 v = (*f2dcc += lbl_eu_806684A0);
    if (v < lbl_eu_806684DC) return;
    *f2dcc = lbl_eu_806684A4;
    base[0x13bf]++;
    sCrystalProbState[4]++;
    MakeCrystal_PlaceCrystals(self);
    u8 r0 = base[0x13bf];
    if (r0 >= 0xa) {
        // Tuning/failback step exhausted: jump to failure cleanup.
        u32 p = *reinterpret_cast<u32*>(base + 0x14);
        base[0xbdd] = 6;
        base[0x13bf] = 0;
        if (p) reinterpret_cast<u8*>(p)[0x59] = 1;
        for (u8 i = 0; i < 2; i++) {
            // Reload the slot pointer (CSE'd with the test load) so MWCC emits
            // retail's dead duplicate beq before the call setup.
            u8* s = base + (u32)i * 0x5cc;
            if (*reinterpret_cast<u32*>(s + 0x44)) {
                void* m = *reinterpret_cast<void**>(s + 0x44);
                if (m != 0) {
                    func_8004B9D4(s + 0x4c, 0x21, 0, -1, 0);
                }
            }
        }
    } else if (r0 >= 3) {
        // Random (1/3) chance to fail the tuning step.
        s8 rem = (s8)((s32)ml::MTRand::getInstance()->rand31() % 10);
        if (rem < 3) {
            u32 p = *reinterpret_cast<u32*>(base + 0x14);
            base[0xbdd] = 6;
            base[0x13bf] = 0;
            if (p) reinterpret_cast<u8*>(p)[0x59] = 1;
            for (u8 i = 0; i < 2; i++) {
                // Reload the slot pointer (CSE'd with the test load) so MWCC
                // emits retail's dead duplicate beq before the call setup.
                u8* s = base + (u32)i * 0x5cc;
                if (*reinterpret_cast<u32*>(s + 0x44)) {
                    void* m = *reinterpret_cast<void**>(s + 0x44);
                    if (m != 0) {
                        func_8004B9D4(s + 0x4c, 0x21, 0, -1, 0);
                    }
                }
            }
        }
    }
}

void MakeCrystal_StateChargeLoop(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    f32* f2dcc = reinterpret_cast<f32*>(base + 0x2dcc);
    f32 v = (*f2dcc += lbl_eu_806684A0);
    if (v < lbl_eu_806684DC) return;
    *f2dcc = lbl_eu_806684A4;
    u8 n = base[0x2dc8] + 1;
    int charged = 0;
    base[0x2dc8] = n;
    if (n >= base[0x2dc7]) {
        charged = 1;
    } else if (n >= base[0x2dc6]) {
        // Retail calls getInstance() before each rand31(); divisor math stays
        // on signed ints so -O4,s keeps the literal divw/mullw/subf sequence.
        if ((s8)((int)ml::MTRand::getInstance()->rand31() % 5) == 0) charged = 1;
    }
    if (charged == 0) {
        // Delta drawn first, then the char-state lookup (retail order).
        s8 delta = (s8)((int)ml::MTRand::getInstance()->rand31() % 11);
        delta += 0x14;
        int collectVal = (u8)GetCollectedFlagByte(base[0x5f0]);
        if (collectVal == 6) delta = (s8)((s8)delta * 2);
        s16 nv = *reinterpret_cast<s16*>(base + 0x2dc4) - (s8)delta;
        *reinterpret_cast<s16*>(base + 0x2dc4) = nv;
        if (nv > 0) {
            base[0xbdd] = 7;
            return;
        }
    }
    base[0xbdd] = 8;
    MakeCrystal_ResetSlotList(self, 0);
    crystalListState3To6(reinterpret_cast<CMCCrystalList*>(base + 0xc18));
    if (*reinterpret_cast<u32*>(base + 0x10)) {
        reinterpret_cast<u8*>(*reinterpret_cast<u32*>(base + 0x10))[0x59] = 7;
    }
    playUISound(0x3f);
}

// Retail 0x8021F50C: slot-trial state driver - once the random success
// window opens, pick the slot whose stored value beats the random threshold
// and place crystals in it. On success the state advances to 2 and the
// function ends (break out of the slot loop); otherwise the next slot is
// tried.
extern "C" void MakeCrystal_TrySlots(void* selfp)
{
    u8* base = reinterpret_cast<u8*>(selfp);
    *reinterpret_cast<f32*>(base + 0x2dcc) = lbl_eu_806684D8;
    if ((u8)code80135FDC_getByte_64077() > 2) {
        u8 charId = (u8)GetCollectedFlagByte(base[0xbbc]);
        for (u8 i = 0; i < 7; i++) {
            if ((s8)base[0x13b8 + i] <= 0) continue;
            u16 val = LookupRelateValue(charId, base[0x13b8 + i]);
            s32 iv = (s32)((float)(u32)val * lbl_eu_806684EC);
            ml::MTRand::getInstance();
            if ((s8)(rand31__Q22ml6MTRandFv() % 100) >= (u16)iv) continue;
            // Success: reset the charge, place the crystals, and stop.
            *reinterpret_cast<f32*>(base + 0x2dcc) = 0.0f;
            ml::MTRand::getInstance();
            u8* entries = reinterpret_cast<CModelDispMakeCrystal*>(base)->mEntries;
            s8 n = (s8)(rand31__Q22ml6MTRandFv() % 3) + 1;
            u8 buf8[4];
            u8 buf12[8];
            buf8[0] = (u8)lbl_eu_806684E0;
            buf8[2] = lbl_eu_806684E2;
            *(u32*)(buf12 + 0) = lbl_eu_806684E4;
            *(u16*)(buf12 + 4) = lbl_eu_806684E8;
            u8 count = entries[0];
            u8 j = 0;
            do {
                ml::MTRand::getInstance();
                s8 pick = (s8)(count != 0 ? rand31__Q22ml6MTRandFv() % count : 0);
                ml::MTRand::getInstance();
                s8 delta = (s8)(rand31__Q22ml6MTRandFv() % 11) + 10;
                u8 found = 0;
                u8 k = 0;
                do {
                    if ((s8)buf8[k] == pick) {
                        *(s16*)(buf12 + (k << 1)) =
                            (s16)(*(s16*)(buf12 + (k << 1)) + delta);
                        found = 1;
                    }
                    k++;
                } while (k < (u8)n);
                if (found == 0) {
                    buf8[j] = (u8)pick;
                    *(s16*)(buf12 + (j << 1)) = delta;
                }
                j++;
            } while (j < (u8)n);
            u8 m = 0;
            do {
                if ((s8)buf8[m] != -1) {
                    setCrystalPosEntry(
                        entries,
                        (u16)(s8)buf8[m],
                        (u16)(*(s16*)(buf12 + (m << 1)) +
                              *(u16*)(entries + (((u16)(s8)buf8[m]) << 3) + 4)),
                        (u16)(s8)buf8[m]);
                }
                m++;
            } while (m < (u8)n);
            MakeCrystal_AdvanceSlotEntries(selfp);
            MakeCrystal_ResetSlotList((CModelDispMakeCrystal*)selfp, 0);
            CrySupSetTexByIndex(base + 0xc70, base[0x13b8 + i]);
            CrySupStartState(base + 0xc70);
            incrementEventCounter(0x7e);
            incrementEventCounter(0x7f);
            playUISound(0x8e);
            MakeCrystal_SpawnCrystalItem(selfp, 4, base[0x13b8 + i]);
            base[0x13b8 + i] = 0;
            break;
        }
    }
    base[0xbdd] = 2;
}

extern "C" void setCrystalPosEntry(
    void* entries, unsigned short index, short a, short b)
{
    struct Entry {
        char _pad0[4];
        short x;
        short y;
        char _pad8;
        unsigned char flag;
    };
    Entry* entry = (Entry*)((char*)entries + ((unsigned int)index << 3));
    entry->x = a;
    entry->y = b;
    entry->flag = 1;
}

// View of the crystal-list head pointer at +0xE1C: active byte at +0,
// u16 live count at +4 (read by the State* gating checks).
struct MCCountHead {
    u8 _pad00[4];
    u16 count04;
};

void MakeCrystal_StateCrystalFull(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (!reinterpret_cast<CMCCrystalList*>(base + 0xc18)->crystalListIsActive()) return;
    u16 crystalCount = ((MCCountHead*)reinterpret_cast<CModelDispMakeCrystal*>(base)->mEntries)->count04;
    if (crystalCount >= 0x64) {
        // Success: full crystal count reached.
        base[0xbdd] = 9;
        reinterpret_cast<CMCEffSuccess*>(base + 0xd5c)->startIn();
        for (u8 i = 0; i < 2; i++) {
            // Reload the slot pointer (CSE'd with the test load) so MWCC
            // emits retail's dead duplicate beq before the call setup.
            if (*reinterpret_cast<u32*>(base + (u32)i * 0x5cc + 0x44)) {
                if (*reinterpret_cast<void**>(base + (u32)i * 0x5cc + 0x44) != 0) {
                    func_8004B9D4(base + (u32)i * 0x5cc + 0x4c, 0x26, 0, -1, 0);
                }
            }
        }
        playUISound(0x91);
        // Then-block laid out first: >= 0xc8 falls through into rank 6.
        if (crystalCount >= 0xc8) {
            MakeCrystal_SpawnCrystalItem(self, 6, 0);
        } else {
            MakeCrystal_SpawnCrystalItem(self, 5, 0);
        }
    } else {
        // Failure: crystal count too low.
        base[0xbdd] = 0xa;
        reinterpret_cast<CMCEffStart*>(base + 0xd78)->startInAnim();
        for (u8 i = 0; i < 2; i++) {
            // Same dead-duplicate-beq shape as the success loop above.
            if (*reinterpret_cast<u32*>(base + (u32)i * 0x5cc + 0x44)) {
                if (*reinterpret_cast<void**>(base + (u32)i * 0x5cc + 0x44) != 0) {
                    func_8004B9D4(base + (u32)i * 0x5cc + 0x4c, 0x27, 0, -1, 0);
                }
            }
        }
        playUISound(0xb2);
    }
}

void MakeCrystal_StatePlacedDone(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    // Once the success effect finishes, record the "crystal placed" state
    // and switch the crystal list to its final state.
    if (isFinished__FPv(base + 0xd5c)) {
        base[0xbdd] = 0xb;
        crystalListState3To8(reinterpret_cast<CMCCrystalList*>(base + 0xc18));
    }
}

void MakeCrystal_StateFailWait(CModelDispMakeCrystal* self) {
    u8* base = reinterpret_cast<u8*>(self);
    *(reinterpret_cast<f32*>(base + 0x2dcc)) += lbl_eu_806684A0;
    if (isFinished__11CMCEffStartFv(base + 0xd78)) {
        *(reinterpret_cast<f32*>(base + 0x2dcc)) = lbl_eu_806684A4;
        base[0xbdd] = 0x10;
        reinterpret_cast<CMCEffSuccess*>(base + 0xd5c)->startOut();
    }
}


// Retail 0x8021FA00: crystal list active - if the gauge has settled to its
// base value, mark the divide-out state; otherwise push the next crystal info
// into the crystal effect and start its in-anim.
void MakeCrystal_StateDivideSel(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (reinterpret_cast<CMCCrystalList*>(base + 0xc18)->crystalListIsActive()) {
        if (lbl_eu_806684D4 == *reinterpret_cast<f32*>(base + 0x2dcc)) {
            base[0xbdd] = 0xf;
        } else {
            base[0xbdd] = 0xc;
            u8* entries = reinterpret_cast<CModelDispMakeCrystal*>(base)->mEntries;
            reinterpret_cast<CMCEffCrystal*>(base + 0xd90)->setCrystalInfo(
                *reinterpret_cast<u16*>(entries + 2), entries[1]);
            reinterpret_cast<CMCEffCrystal*>(base + 0xd90)->startIn();
            playUISound(0x8c);
        }
    }
}

// Retail 0x8021FA74: crystal-list state driver - when the crystal effect
// finishes, either start the change-in anim (count >= 200) or trace-out.
// NOTE: retail saves r30/r31 as one [stmw] block; -O4,p emits separate stw
// (MWCC_CASES S16), so force the size-optimal frame like MakeCrystal_InitSlotArray.
void MakeCrystal_StateCrystalChk(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (isFinished__13CMCEffCrystalFv(base + 0xd90)) {
        u8* entries = reinterpret_cast<CModelDispMakeCrystal*>(base)->mEntries;
        if (*reinterpret_cast<u16*>(entries + 4) >= 0xc8) {
            base[0xbdd] = 0xd;
            reinterpret_cast<CMCEffCrystal*>(base + 0xd90)->startChange();
            reinterpret_cast<CMCEffStart*>(base + 0xdcc)->startInAnim();
            for (u8 i = 0; i < 2; i++) {
                u8* s = base + (u32)i * 0x5cc;
                if (*reinterpret_cast<u32*>(s + 0x44)) {
                    void* m = *reinterpret_cast<void**>(s + 0x44);
                    if (m != 0) {
                        func_8004B9D4(s + 0x4c, 0x22, 0, -1, 0);
                    }
                }
            }
            playUISound(0x8d);
            MakeCrystal_SpawnCrystalItem(self, 7, 0);
        } else {
            base[0xbdd] = 0xf;
            reinterpret_cast<CMCEffCrystal*>(base + 0xd90)->setTraceVisible(0);
            reinterpret_cast<CMCEffCrystal*>(base + 0xd90)->startOut();
            playUISound(0x8c);
        }
    }
}

// Retail 0x8021FB64: crystal-list state driver - when the start effect
// finishes, either run the success/special path (crystal count >= 300) or
// the trace-out path.
void MakeCrystal_StateRankUp(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (isFinished__11CMCEffStartFv(base + 0xdcc) == 0) return;
    f32* vel = reinterpret_cast<f32*>(base + 0x2dcc);
    f32 v = (*vel += lbl_eu_806684A0);
    if (v < lbl_eu_806684D8) return;
    *vel = lbl_eu_806684A4;
    u16 count = ((MCCountHead*)reinterpret_cast<CModelDispMakeCrystal*>(base)->mEntries)->count04;
    if (count >= 0x12c) {
        // Success path: full crystal count reached.
        base[0xbdd] = 0xe;
        reinterpret_cast<CMCEffStart*>(base + 0xde4)->startInAnim();
        reinterpret_cast<CMCEffCrystal*>(base + 0xd90)->startSpecial();
        for (u8 i = 0; i < 2; i++) {
            u8* s = base + (u32)i * 0x5cc;
            if (*reinterpret_cast<u32*>(s + 0x44)) {
                void* m = *reinterpret_cast<void**>(s + 0x44);
                if (m != 0) {
                    func_8004B9D4(s + 0x4c, 0x22, 0, -1, 0);
                }
            }
        }
        playUISound(0x8d);
        MakeCrystal_SpawnCrystalItem(self, 7, 0);
    } else {
        base[0xbdd] = 0xf;
        reinterpret_cast<CMCEffCrystal*>(base + 0xd90)->setTraceVisible(0);
        reinterpret_cast<CMCEffCrystal*>(base + 0xd90)->startOut();
    }
}

// Retail 0x8021FC78: once the divide-in effect finishes, snap the gauge
// position, switch state, and begin the crystal effect's out animation.
void MakeCrystal_StateDivideDone(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (isFinished__11CMCEffStartFv(base + 0xde4)) {
        *reinterpret_cast<f32*>(base + 0x2dcc) = lbl_eu_806684A4;
        base[0xbdd] = 0xf;
        reinterpret_cast<CMCEffCrystal*>(base + 0xd90)->setTraceVisible(1);
        reinterpret_cast<CMCEffCrystal*>(base + 0xd90)->startOut();
    }
}

// Retail 0x8021FD64: once the crystal-out effect finishes, either lock in the
// success state (crystal count >= 100) or start the success-effect out anim.
void MakeCrystal_StateCrystalDone(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (isFinished__13CMCEffCrystalFv(base + 0xd90)) {
        MakeCrystal_AppendItemSteps(self);
        u16 count = ((MCCountHead*)reinterpret_cast<CModelDispMakeCrystal*>(base)->mEntries)->count04;
        if (count >= 0x64) {
            base[0xbdd] = 0xb;
            crystalListState3To8(reinterpret_cast<CMCCrystalList*>(base + 0xc18));
        } else {
            *reinterpret_cast<f32*>(base + 0x2dcc) = lbl_eu_806684A4;
            base[0xbdd] = 0x10;
            reinterpret_cast<CMCEffSuccess*>(base + 0xd5c)->startOut();
        }
    }
}

void MakeCrystal_StateSuccessChk(CModelDispMakeCrystal* self)
{
    if (isFinished__FPv(&self->mEffSuccess)) {
        u8* base = reinterpret_cast<u8*>(self);
        u8* entries = reinterpret_cast<CModelDispMakeCrystal*>(base)->mEntries;
        if (entries[0]) {
            base[0xbdd] = 0x11;
            reinterpret_cast<CMCEffCylinder*>(base + 0xdfc)
->setNumber(reinterpret_cast<CMCCylinderGauge*>(base + 0xbec)->getLevel());
            reinterpret_cast<CMCCylinderGauge*>(base + 0xbec)->startOut();
            reinterpret_cast<CMCEffCylinder*>(base + 0xdfc)->startIn();
        } else {
            base[0xbdd] = 0x14;
            reinterpret_cast<CMCCylinderGauge*>(base + 0xbec)->startOut();
        }
    }
}

void MakeCrystal_NopDF84(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    // Once the cylinder effect finishes and the gauge is full, start the
    // cylinder-count anim: rebuild the step list and flush the buffered entry.
    if (isFinished__14CMCEffCylinderFv(base + 0xdfc) == 0) return;
    // Branch/else-return (not early-return): retail branches to the body with
    // bne when the gauge is ready and skips it with b otherwise.
    if (reinterpret_cast<CMCCylinderGauge*>(base + 0xbec)->isReady() != 0) {
        base[0xbdd] = 0x12;
        ((CBaseCur*)(base + 0xe20))->SetVisible(1);
        u8 buf[16];
        crystalListAlignSlot(buf, base + 0xc18, base[0x2dc1]);
        reinterpret_cast<CBaseCur*>(base + 0xe20)->setRootPaneTranslate(reinterpret_cast<const nw4r::math::VEC3*>(buf));
        crystalListResetAnim3(base + 0xc18);
    } else {
        return;
    }
}

// Retail 0x8021FE6C: input-driven cursor/trigger dispatch for the cylinder
// gauge-tune dialog. Classify the pad flags (Classic vs Wiimote/Nunchuk),
// then route trigger presses to cursor stepping (cancel/dir/confirm/menu)
// or to the CSysWin dialog paths.
// Retail frame is stmw r30 (2-reg save); decomp emits separate stw saves -
// per-function -O4,s pragma was tried and made the body diverge further
// (MWCC_CASES §16/§13 static cap).
void MakeCrystal_DispatchTuneInput(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();
    // Button masks are kept as raw mask words and converted to bools right
    // after decode; trigger bits are extracted as 0/1 ints (retail extrwi).
    u32 cancel, dir, confirm, menu;
    int trigger1, trigger2, trigger3;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        u32 f = *reinterpret_cast<u32*>(reinterpret_cast<u8*>(pad) + 0x104);
        u32 p = *reinterpret_cast<u32*>(reinterpret_cast<u8*>(pad) + 4);
        cancel = f & 0x8004;
        dir = (f & 0x10000000) | (f & 0x8000);
        confirm = f & 0x2001;
        menu = f & 0x4002;
        trigger1 = (p >> 10) & 1;
        trigger2 = (p >> 9) & 1;
        trigger3 = (p >> 22) & 1;
    } else {
        u32 f = *reinterpret_cast<u32*>(reinterpret_cast<u8*>(pad) + 0x104);
        u32 p = *reinterpret_cast<u32*>(reinterpret_cast<u8*>(pad) + 4);
        cancel = f & 0x8004;
        dir = (f & 0x10000000) | (f & 0x8000);
        confirm = f & 0x2001;
        menu = f & 0x4002;
        trigger1 = (p >> 27) & 1;
        trigger2 = (p >> 26) & 1;
        trigger3 = (p >> 22) & 1;
    }
    bool bCancel = cancel != 0;
    bool bDir = dir != 0;
    bool bConfirm = confirm != 0;
    bool bMenu = menu != 0;

    if (trigger1) {
        if (reinterpret_cast<CSysWin*>(base + 0xe78)->isOpened() != 0) {
            if (reinterpret_cast<CSysWin*>(base + 0xe78)->isActive()) {
                reinterpret_cast<CSysWin*>(base + 0xe78)->advancePhase3();
                ((CBaseCur*)(base + 0xe20))->SetVisible(1);
            }
        } else {
            base[0xbdd] = 0x13;
            reinterpret_cast<CMCEffCylinder*>(base + 0xdfc)->startChange();
            playUISound(3);
        }
    } else if (trigger2) {
        if (reinterpret_cast<CSysWin*>(base + 0xe78)->isOpened() != 0) {
            if (reinterpret_cast<CSysWin*>(base + 0xe78)->isActive()) {
                reinterpret_cast<CSysWin*>(base + 0xe78)->advancePhase3();
                ((CBaseCur*)(base + 0xe20))->SetVisible(1);
            }
        }
    } else if (bCancel) {
        // Cursor back: decrement the row cursor with page wrap-around.
        if (reinterpret_cast<CSysWin*>(base + 0xe78)->isOpened() == 0) {
            u8 c1 = (u8)(base[0x2dc1] - 1);
            u8* entries = reinterpret_cast<CModelDispMakeCrystal*>(base)->mEntries;
            u8 entryCount = entries[0];
            base[0x2dc1] = c1;
            if ((s8)c1 < 0) {
                u8 c2 = (u8)(base[0x2dc2] - 1);
                base[0x2dc1] = 0;
                base[0x2dc2] = c2;
                if ((s8)c2 < 0) {
                    if (entryCount >= 8) {
                        base[0x2dc1] = 7;
                        base[0x2dc2] = (u8)(entryCount - 8);
                    } else {
                        u8 nc1 = (u8)(entryCount - 1);
                        base[0x2dc1] = nc1;
                        base[0x2dc2] = 0;
                        if ((s8)nc1 < 0) base[0x2dc1] = 0;
                    }
                }
            }
            char local[0xc];
            crystalListAlignSlot(local, base + 0xc18, base[0x2dc1]);
            reinterpret_cast<CBaseCur*>(base + 0xe20)->setRootPaneTranslate(reinterpret_cast<const nw4r::math::VEC3*>(local));
            MakeCrystal_ResetSlotList(self, 1);
            reinterpret_cast<CScrollBar*>(base + 0xe38)->PlaceThumb((u16)(s8)base[0x2dc2]);
            playUISound(1);
        }
    } else if (bDir) {
        // Cursor forward: increment the row cursor with page wrap-around.
        if (reinterpret_cast<CSysWin*>(base + 0xe78)->isOpened() == 0) {
            u8* entries = reinterpret_cast<CModelDispMakeCrystal*>(base)->mEntries;
            u8 entryCount = entries[0];
            if (entryCount >= 8) {
                u8 c1 = (u8)(base[0x2dc1] + 1);
                base[0x2dc1] = c1;
                if ((s8)c1 >= 8) {
                    u8 c2 = (u8)(base[0x2dc2] + 1);
                    base[0x2dc1] = 7;
                    base[0x2dc2] = c2;
                    if ((s8)c2 > (s32)(entryCount - 8)) {
                        base[0x2dc1] = 0;
                        base[0x2dc2] = 0;
                    }
                }
            } else {
                u8 c1 = (u8)(base[0x2dc1] + 1);
                base[0x2dc1] = c1;
                if ((s8)c1 >= (s8)entryCount) {
                    base[0x2dc1] = 0;
                    base[0x2dc2] = 0;
                }
            }
            char local[0xc];
            crystalListAlignSlot(local, base + 0xc18, base[0x2dc1]);
            reinterpret_cast<CBaseCur*>(base + 0xe20)->setRootPaneTranslate(reinterpret_cast<const nw4r::math::VEC3*>(local));
            MakeCrystal_ResetSlotList(self, 1);
            reinterpret_cast<CScrollBar*>(base + 0xe38)->PlaceThumb((u16)(s8)base[0x2dc2]);
            playUISound(1);
        }
    } else if (bConfirm) {
        // Page back by 8 rows.
        if (reinterpret_cast<CSysWin*>(base + 0xe78)->isOpened() == 0) {
            u8* entries = reinterpret_cast<CModelDispMakeCrystal*>(base)->mEntries;
            u8 entryCount = entries[0];
            if (entryCount >= 8) {
                u8 nc2 = (u8)(base[0x2dc2] - 8);
                base[0x2dc2] = nc2;
                if ((s8)nc2 < 0) {
                    u8 nc1 = (u8)(nc2 + 7);
                    base[0x2dc1] = nc1;
                    base[0x2dc2] = 0;
                    if ((s8)nc1 < 0) base[0x2dc1] = 0;
                }
            } else {
                base[0x2dc1] = 0;
                base[0x2dc2] = 0;
            }
            char local[0xc];
            crystalListAlignSlot(local, base + 0xc18, base[0x2dc1]);
            reinterpret_cast<CBaseCur*>(base + 0xe20)->setRootPaneTranslate(reinterpret_cast<const nw4r::math::VEC3*>(local));
            MakeCrystal_ResetSlotList(self, 1);
            reinterpret_cast<CScrollBar*>(base + 0xe38)->PlaceThumb((u16)(s8)base[0x2dc2]);
            playUISound(1);
        }
    } else if (bMenu) {
        // Page forward by 8 rows.
        if (reinterpret_cast<CSysWin*>(base + 0xe78)->isOpened() == 0) {
            u8* entries = reinterpret_cast<CModelDispMakeCrystal*>(base)->mEntries;
            u8 entryCount = entries[0];
            if (entryCount >= 8) {
                u8 nc2 = (u8)(base[0x2dc2] + 8);
                base[0x2dc2] = nc2;
                if ((s8)nc2 > (s32)(entryCount - 8)) {
                    u8 nc1 = (u8)(nc2 - (entryCount - 8));
                    base[0x2dc1] = nc1;
                    base[0x2dc2] = (u8)(entryCount - 8);
                    if ((s8)nc1 >= 8) base[0x2dc1] = 7;
                }
            } else {
                u8 nc1 = (u8)(entryCount - 1);
                base[0x2dc1] = nc1;
                base[0x2dc2] = 0;
                if ((s8)nc1 < 0) base[0x2dc1] = 0;
            }
            char local[0xc];
            crystalListAlignSlot(local, base + 0xc18, base[0x2dc1]);
            reinterpret_cast<CBaseCur*>(base + 0xe20)->setRootPaneTranslate(reinterpret_cast<const nw4r::math::VEC3*>(local));
            MakeCrystal_ResetSlotList(self, 1);
            reinterpret_cast<CScrollBar*>(base + 0xe38)->PlaceThumb((u16)(s8)base[0x2dc2]);
            playUISound(1);
        }
    } else if (trigger3) {
        if (reinterpret_cast<CSysWin*>(base + 0xe78)->isOpened() != 0) {
            if (reinterpret_cast<CSysWin*>(base + 0xe78)->isActive()) {
                reinterpret_cast<CSysWin*>(base + 0xe78)->advancePhase3();
                ((CBaseCur*)(base + 0xe20))->SetVisible(1);
            }
        } else {
            // Selection key: resolve the cursor-sum index to an entry's
            // u16 param and set the dialog text.
            s32 idx = (s8)base[0x2dc1] + (s8)base[0x2dc2];
            u8* entries = reinterpret_cast<CModelDispMakeCrystal*>(base)->mEntries;
            u16 ev = *(u16*)(entries + ((u32)((u16)idx << 3)) + 2);
            char* s = BdatTouchStringCell(&lbl_eu_805090FC[0x38], &lbl_eu_805090FC[0x42], ev);
            reinterpret_cast<CSysWin*>(base + 0xe78)->switchKindPane(0);
            reinterpret_cast<CSysWin*>(base + 0xe78)->setKindContent(reinterpret_cast<char*>(s), 0);
            reinterpret_cast<CSysWin*>(base + 0xe78)->applyKind(1);
            reinterpret_cast<CSysWin*>(base + 0xe78)->openPhase1();
            ((CBaseCur*)(base + 0xe20))->SetVisible(0);
        }
    }
}

// Local mirror of the item record used by MakeCrystal_FinalizeCylCount: 0x34 bytes, head
// zeroed as a u32 + u16, then copied into the 0x34-stride step slots at
// +0x13c0 by CItem_copyRecMasked.
struct CMCItemData {
    u32 field_00;
    u8 field_04;
    u8 field_05;
    u8 field_06;
    u8 field_07;
    u8 field_08[0x20];
    s16 mEntries[4];
    u8 mCount;
    u8 field_31;
    u8 field_32;
    u8 field_33;
};

// Retail 0x80220418: cylinder-count finalize. Once the cylinder effect
// finishes, rebuild the item step list from the gauge value and the current
// crystal-slot entries, then hand the next step off to the cylinder UI.
void MakeCrystal_FinalizeCylCount(CModelDispMakeCrystal* self)
{
    if (isFinished__14CMCEffCylinderFv(&self->mEffCylinder) == 0) return;
    self->mGauge.decrementLevel();
    u8 v = self->mGauge.getLevel();
    self->mEffCylinder.setNumber(v);
    u8* entries = self->mEntries;
    CMCItemData item;
    item.field_00 = 0;
    *reinterpret_cast<u16*>(&item.field_04) = 0;
    CItem_initKind9Rec(&item, 1);
    ((CItemImpl*)CItem_initItemImplInstances((CItemData*)&item))->vf1C((CItemData*)&item);
    ((CItemImpl*)CItem_initItemImplInstances((CItemData*)&item))->vf84((CItemData*)&item, 1);
    CItemData_callVf0C(&item, entries[1]);
    for (u8 i = 0; i < 4; i++) {
        ((CItemImpl*)CItem_initItemImplInstances((CItemData*)&item))->vf50((CItemData*)&item, i, 0);
        ((CItemImpl*)CItem_initItemImplInstances((CItemData*)&item))->vf68((CItemData*)&item, i, 0);
    }
    u16 idx = (u16)((s8)((u8*)self)[0x2dc1] + (s8)((u8*)self)[0x2dc2]);
    ((CItemImpl*)CItem_initItemImplInstances((CItemData*)&item))->vf50((CItemData*)&item, 0, *(u16*)(entries + ((u16)idx << 3) + 2));
    ((CItemImpl*)CItem_initItemImplInstances((CItemData*)&item))->vf68((CItemData*)&item, 0, *(u16*)(entries + ((u16)idx << 3) + 4));
    u8 n = ((u8*)self)[0x2dc0];
    ((u8*)self)[0x2dc0] = n + 1;
    CItem_copyRecMasked((u8*)self + n * 0x34 + 0x13c0, &item);
    reinterpret_cast<CMCCrystalParamTable*>(entries)->clearSlot(idx);
    reinterpret_cast<CMCCrystalParamTable*>(entries)->refreshCount();
    ((u8*)self)[0x2dc1] = 0;
    ((u8*)self)[0x2dc2] = 0;
    u8 buf[0xc];
    crystalListAlignSlot(buf, &self->mCrystalList, 0);
    self->mCurE20.setRootPaneTranslate(reinterpret_cast<const nw4r::math::VEC3*>(buf));
    MakeCrystal_ResetSlotList(self, 1);
    self->mScrollBar.UpdateThumb(8, entries[0]);
    self->mScrollBar.PlaceThumb((u16)(s8)((u8*)self)[0x2dc2]);
    playUISound(0xb1);
    if (entries[0] != 0 &&
        self->mGauge.getLevel() != 0) {
        ((u8*)self)[0xbdd] = 0x12;
    } else {
        ((u8*)self)[0xbdd] = 0x14;
        self->mEffCylinder.startOut();
    }
}

void CMCCrystalParamTable::clearSlot(u16 idx)
{
    mSlots[idx].set(CMCCrystalBoxParam());
}

// Retail 0x802206E0: sort the slots, then record the index of the first empty
// slot as the used count.
void CMCCrystalParamTable::refreshCount()
{
    sortSlots();
    for (u8 i = 0; i < 0x20; i++) {
        if (mSlots[i].m0 == 0) {
            mCount = i;
            break;
        }
    }
}

// Retail 0x8022073C: two-pass bubble sort of the used slots (by quantity,
// then by the m7 flag), then clear every used slot's m7 flag.
void CMCCrystalParamTable::sortSlots()
{
    u8 i;                          // pass-1 outer / pass-2 inner
    u8 j;                          // pass-1 inner / pass-2 outer
    for (i = 0; i < mCount - 1; i++) {
        int swapped = 0;
        for (j = 0; j < mCount - 1 - i; j++) {
            if (mSlots[j].m2 < mSlots[j + 1].m2) {
                CMCCrystalBoxParam t(mSlots[j]);
                mSlots[j].set(CMCCrystalBoxParam(mSlots[j + 1]));
                mSlots[j + 1].set(CMCCrystalBoxParam(t));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
    for (j = 0; j < mCount - 1; j++) {
        int swapped = 0;
        for (i = 0; i < mCount - 1 - j; i++) {
            if (mSlots[i].m7 < mSlots[i + 1].m7) {
                CMCCrystalBoxParam t(mSlots[i]);
                mSlots[i].set(CMCCrystalBoxParam(mSlots[i + 1]));
                mSlots[i + 1].set(CMCCrystalBoxParam(t));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
    for (u8 m = 0; m < mCount; m++) {
        mSlots[m].m7 = 0;
    }
}

void MakeCrystal_StateCylDone(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    // Once the cylinder effect finishes, mark the cylinder phase done and
    // advance the crystal list state.
    if (isFinished__14CMCEffCylinderFv(base + 0xdfc)) {
        base[0xbdd] = 0x15;
        crystalListState3To7(reinterpret_cast<CMCCrystalList*>(base + 0xc18));
    }
}

// Retail 0x80220958: crystal charge float tick - accumulate the charge,
// clamp and flag the charged state, then push the value into the model and
// slot effects; once charged, advance the crystal list state.
void MakeCrystal_StateChargeFinal(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    f32* fbe0 = reinterpret_cast<f32*>(base + 0xbe0);
    f32 v = (*fbe0 += lbl_eu_806684D0);
    if (v > lbl_eu_806684A0) {
        *fbe0 = lbl_eu_806684A0;
        base[0xbdc] = 0;
        base[0xbe8] = 1;
        base[0x2dc3] = 1;
    }
    void* p20 = *reinterpret_cast<void**>(base + 0x20);
    if (p20) {
        reinterpret_cast<CScnItemModel*>(p20)->vfunc48(*fbe0);
    }
    for (u8 i = 0; i < 2; i++) {
        void* p = *reinterpret_cast<void**>(base + (u32)i * 0x5cc + 0x44);
        if (p) {
            reinterpret_cast<CScnItemModel*>(p)->vfunc48(*fbe0);
        }
    }
    if (!(*fbe0 < lbl_eu_806684A0)) {
        if (reinterpret_cast<CMCCrystalList*>(base + 0xc18)->crystalListIsActive()) {
            base[0xbdd] = 0x16;
            crystalListState3To4(reinterpret_cast<CMCCrystalList*>(base + 0xc18));
            reinterpret_cast<CScrollBar*>(base + 0xe38)->requestScrollOut();
            ((CBaseCur*)(base + 0xe20))->SetVisible(0);
        }
    }
}

// Retail 0x80220A5C: after the crystal list is active, scan the step list
// (stride 0x34 at +0x13c0) for a step of type 9; the result picks the
// pass/fail UI state (0x17 or 0x1e) and re-flushes the step list.
void MakeCrystal_PickPassFail(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (reinterpret_cast<CMCCrystalList*>(base + 0xc18)->crystalListIsActive() == 0) return;
    MakeCrystal_TeardownUI(self);
    u8 count = base[0x2dc0];
    int found = 0;
    for (u8 i = 0; i < count; i++) {
        u32 off = reinterpret_cast<u32>(base) + (u32)i * 0x34;
        CMCStep* p = reinterpret_cast<CMCStep*>(off + 0x13c0);
        if (p && p->type == 9) {
            found = 1;
            break;
        }
    }
    if (found) {
        base[0xbdd] = 0x17;
        openMCGetItemBox(base + 0xecc, 0x9, base + 0x13c0, base[0x2dc0]);
    } else {
        base[0xbdd] = 0x1e;
        openMCGetItemBox(base + 0xecc, 0x3, base + 0x13c0, base[0x2dc0]);
    }
}

void MakeCrystal_MoveCursorPane(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();
    u32 trigger1, trigger2, trigger3, cancel, confirm, menu, dir;
    // The bit positions differ between Classic Controller and Wiimote/Nunchuk.
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        u32 f = pad->mTurboPressButtonFlags;
        u32 p = pad->mPad.mPressedButtonFlags;
        cancel = (f & 0x8004) != 0;
        dir = (f & 0x10008) != 0;
        confirm = (f & 0x2001) != 0;
        menu = (f & 0x4002) != 0;
        trigger1 = (p >> 21) & 1;
        trigger2 = (p >> 22) & 1;
        trigger3 = (p >> 9) & 1;
    } else {
        u32 f = pad->mTurboPressButtonFlags;
        u32 p = pad->mPad.mPressedButtonFlags;
        cancel = (f & 0x8004) != 0;
        dir = (f & 0x10008) != 0;
        confirm = (f & 0x2001) != 0;
        menu = (f & 0x4002) != 0;
        trigger1 = (p >> 4) & 1;
        trigger2 = (p >> 5) & 1;
        trigger3 = (p >> 9) & 1;
    }

    if (trigger1) {
        // Confirm/advance trigger: drive the makecrystal file-state machine.
        if (!((CMCGetItemBox*)(base + 0xecc))->isMCGetItemBoxGridMode()) {
            int r = getMCGetItemBoxPageState((CMCGetItemBox*)(base + 0xecc));
            if (r == 1) {
                // Proceed to the pass/fail tuning state.
                base[0xbdd] = 0x19;
                cacheMCGetItemBoxEntries((CMCGetItemBox*)(base + 0xecc));
                confirmMCGetItemBox((CMCGetItemBox*)(base + 0xecc));
                playUISound(3);
            } else if (r == 2) {
                // Jump to gauge-tuning state: rebuild the confirmation UI.
                base[0xbdd] = 0x1a;
                base[0x2dd1] = 0;
                char* a = (char*)BdatTouchStringCell(&lbl_eu_805090FC[0x47], &lbl_eu_805090FC[0x52], 2);
                char* b = (char*)BdatTouchStringCell(&lbl_eu_805090FC[0x47], &lbl_eu_805090FC[0x52], 4);
                char* c = (char*)BdatTouchStringCell(&lbl_eu_805090FC[0x47], &lbl_eu_805090FC[0x52], 5);
                reinterpret_cast<CSysWin*>(base + 0xe78)->switchKindPane(2);
                reinterpret_cast<CSysWin*>(base + 0xe78)->setKindContent(reinterpret_cast<char*>(a), 0);
                reinterpret_cast<CSysWin*>(base + 0xe78)->setTwoTextValues(reinterpret_cast<char*>(b), reinterpret_cast<char*>(c));
                reinterpret_cast<CSysWin*>(base + 0xe78)->applyKind(0);
                reinterpret_cast<CSysWin*>(base + 0xe78)->openPhase1();
                forwardMCSubObj88(base + 0xecc, 0);
                playUISound(5);
            }
        } else {
            openMCGetItemBoxSysWin((CMCGetItemBox*)(base + 0xecc));
        }
    } else if (trigger2) {
        toggleMCGetItemBoxSysWin((CMCGetItemBox*)(base + 0xecc));
    } else if (cancel) {
        moveMCGetItemBoxLeft(base + 0xecc);
    } else if (dir) {
        moveMCGetItemBoxRight((CMCGetItemBox*)(base + 0xecc));
    } else if (confirm) {
        moveMCGetItemBoxUp((CMCGetItemBox*)(base + 0xecc));
    } else if (menu) {
        moveMCGetItemBoxDown((CMCGetItemBox*)(base + 0xecc));
    } else if (trigger3) {
        advanceMCGetItemBox(base + 0xecc);
    }
}

void func_8021EF30(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (!((CMCGetItemBox*)(base + 0xecc))->isMCGetItemBoxReady()) return;
    u8 count = base[0x2dc0];
    int found = 0;
    // While-form loop: retail jumps to the condition before the first body
    // iteration; element-pointer formation matches retail's add/addic. split.
    for (u8 i = 0; i < count; i++) {
        u32 off = reinterpret_cast<u32>(base) + (u32)i * 0x34;
        CMCStep* p = reinterpret_cast<CMCStep*>(off + 0x13c0);
        if (p && p->type == 3) {
            found = 1;
            break;
        }
    }
    if (found) {
        base[0xbdd] = 0x1e;
        openMCGetItemBox(base + 0xecc, 3, base + 0x13c0, base[0x2dc0]);
    } else {
        base[0xbdd] = 0x25;
    }
}

void MakeCrystal_StateSysWinPrompt(CModelDispMakeCrystal* self) {
    u8* base = reinterpret_cast<u8*>(self);
    if (!reinterpret_cast<CSysWin*>(base + 0xe78)->isActive()) return;
    base[0xbdd] = 0x1b;
    ((CBaseCur*)(base + 0xeb4))->SetVisible(1);
    u8 param = base[0x2dd1];
    u8 buf[16];
    CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(buf), reinterpret_cast<CSysWin*>(base + 0xe78), param);
    reinterpret_cast<CBaseCur*>(base + 0xeb4)->setRootPaneTranslate(reinterpret_cast<const nw4r::math::VEC3*>(buf));
}


// [stmw] r30 frame (see MakeCrystal_StateCrystalChk note): -O4,s.
void MakeCrystal_ShowCursorPane(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();
    // Cursor step / cancel booleans derived from turbo and pressed button flags.
    // Bit positions differ between Classic controller and Wiimote/Nunchuk.
    u32 trigger1, trigger2, cancel, dir;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        u32 p = pad->mPad.mPressedButtonFlags;
        u32 f = pad->mTurboPressButtonFlags;
        cancel = (f & 0x8004) != 0;
        dir = ((f & 0x10000) | (f & 0x8)) != 0;
        trigger1 = (p >> 21) & 1;
        trigger2 = (p >> 22) & 1;
    } else {
        u32 p = pad->mPad.mPressedButtonFlags;
        u32 f = pad->mTurboPressButtonFlags;
        cancel = (f & 0x8004) != 0;
        dir = ((f & 0x10000) | (f & 0x8)) != 0;
        trigger1 = (p >> 4) & 1;
        trigger2 = (p >> 5) & 1;
    }

    if (trigger1) {
        // Fast cursor advance button.
        base[0xbdd] = 0x1c;
        ((CBaseCur*)(base + 0xeb4))->SetVisible(0);
        reinterpret_cast<CSysWin*>(base + 0xe78)->advancePhase3();
        playUISound(3);
    } else if (trigger2) {
        // Forward cursor button: reset step and play forward sound.
        base[0xbdd] = 0x1c;
        base[0x2dd1] = 1;
        ((CBaseCur*)(base + 0xeb4))->SetVisible(0);
        reinterpret_cast<CSysWin*>(base + 0xe78)->advancePhase3();
        playUISound(6);
    } else if (cancel) {
        // Cursor back: decrement step (wrapping to 1).
        u8 v = base[0x2dd1] - 1;
        base[0x2dd1] = v;
        if ((s8)v < 0) base[0x2dd1] = 1;
        u8 param = base[0x2dd1];
        u8 buf[12];
        CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(buf), reinterpret_cast<CSysWin*>(base + 0xe78), param);
        reinterpret_cast<CBaseCur*>(base + 0xeb4)->setRootPaneTranslate(reinterpret_cast<const nw4r::math::VEC3*>(buf));
        playUISound(1);
    } else if (dir) {
        // Cursor forward: increment step (resetting to 0).
        u8 v = base[0x2dd1] + 1;
        base[0x2dd1] = v;
        if ((s8)v > 1) base[0x2dd1] = 0;
        u8 param = base[0x2dd1];
        u8 buf[12];
        CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(buf), reinterpret_cast<CSysWin*>(base + 0xe78), param);
        reinterpret_cast<CBaseCur*>(base + 0xeb4)->setRootPaneTranslate(reinterpret_cast<const nw4r::math::VEC3*>(buf));
        playUISound(1);
    }
}

// Retail 0x80221104
// step: zero runs the confirm-side cleanup, non-zero moves the cursor forward.
void MakeCrystal_StateConfirmStep(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (reinterpret_cast<CSysWin*>(base + 0xe78)->isActive()) {
        if ((s8)base[0x2dd1] == 0) {
            base[0xbdd] = 0x1d;
            CCharVoiceMan_FlushAll();
            MenuSnd_StopAllSlots_89C88();
            CTaskGame_stopVision();
            CUICfManager_queuePauseItemMenu(0x9, *reinterpret_cast<u32*>(base + 0xc), 0x0);
        } else {
            base[0xbdd] = 0x19;
            confirmMCGetItemBox((CMCGetItemBox*)(base + 0xecc));
        }
    }
}

void MakeCrystal_StateItemMenuWait(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    // While the item menu is not open, move to the pass/fail tuning state
    // and reset the crystal box file state machine.
    if (ItemMenu_IsPresent() == 0) {
        base[0xbdd] = 0x18;
        forwardMCSubObj88(base + 0xecc, 1);
    }
}

void func_8021F2D8(CModelDispMakeCrystal* self)
{
    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();
    int trigger1, trigger2, trigger3, cancel, confirm, menu, dir;
    // The bit positions differ between Classic Controller and Wiimote/Nunchuk.
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        u32 f = pad->mTurboPressButtonFlags;
        u32 p = pad->mPad.mPressedButtonFlags;
        u32 c = f & 0x8004;
        u32 d = (f & 0x10000) | (f & 0x8);
        u32 k = f & 0x2001;
        u32 m = f & 0x4002;
        trigger1 = (p >> 21) & 1;
        trigger2 = (p >> 22) & 1;
        trigger3 = (p >> 9) & 1;
        cancel = c != 0;
        dir = d != 0;
        confirm = k != 0;
        menu = m != 0;
    } else {
        u32 f = pad->mTurboPressButtonFlags;
        u32 p = pad->mPad.mPressedButtonFlags;
        u32 c = f & 0x8004;
        u32 d = (f & 0x10000) | (f & 0x8);
        u32 k = f & 0x2001;
        u32 m = f & 0x4002;
        trigger1 = (p >> 4) & 1;
        trigger2 = (p >> 5) & 1;
        trigger3 = (p >> 9) & 1;
        cancel = c != 0;
        dir = d != 0;
        confirm = k != 0;
        menu = m != 0;
    }

    if (trigger1) {
        // Confirm/advance trigger: drive the makecrystal file-state machine.
        if ((&self->mItemBox)->isMCGetItemBoxGridMode() != 0) {
            openMCGetItemBoxSysWin(&self->mItemBox);
        } else {
            u8 r = (u8)getMCGetItemBoxPageState(&self->mItemBox);
            if (r == 1) {
                // Proceed to the pass/fail tuning state.
                ((u8*)self)[0xbdd] = 0x20;
                cacheMCGetItemBoxEntries(&self->mItemBox);
                confirmMCGetItemBox(&self->mItemBox);
                playUISound(3);
            } else if (r == 2) {
                // Jump to gauge-tuning state: rebuild the confirmation UI.
                ((u8*)self)[0xbdd] = 0x21;
                ((u8*)self)[0x2dd1] = 0;
                char* a = BdatTouchStringCell(&lbl_eu_805090FC[0x47], &lbl_eu_805090FC[0x52], 2);
                char* b = BdatTouchStringCell(&lbl_eu_805090FC[0x47], &lbl_eu_805090FC[0x52], 4);
                char* c = BdatTouchStringCell(&lbl_eu_805090FC[0x47], &lbl_eu_805090FC[0x52], 5);
                self->mSysWin.switchKindPane(2);
                self->mSysWin.setKindContent(reinterpret_cast<char*>(a), 0);
                self->mSysWin.setTwoTextValues(reinterpret_cast<char*>(b), reinterpret_cast<char*>(c));
                self->mSysWin.applyKind(0);
                self->mSysWin.openPhase1();
                forwardMCSubObj88(&self->mItemBox, 0);
                playUISound(5);
            }
        }
    } else if (trigger2) {
        toggleMCGetItemBoxSysWin(&self->mItemBox);
    } else if (cancel) {
        moveMCGetItemBoxLeft(&self->mItemBox);
    } else if (dir) {
        moveMCGetItemBoxRight(&self->mItemBox);
    } else if (confirm) {
        moveMCGetItemBoxUp(&self->mItemBox);
    } else if (menu) {
        moveMCGetItemBoxDown(&self->mItemBox);
    } else if (trigger3) {
        advanceMCGetItemBox(&self->mItemBox);
    }
}

void MakeCrystal_StateConfirmPrompt(CModelDispMakeCrystal* self) {
    u8* base = reinterpret_cast<u8*>(self);
    if (!reinterpret_cast<CSysWin*>(base + 0xe78)->isActive()) return;
    base[0xbdd] = 0x22;
    ((CBaseCur*)(base + 0xeb4))->SetVisible(1);
    u8 param = base[0x2dd1];
    u8 buf[16];
    CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(buf), reinterpret_cast<CSysWin*>(base + 0xe78), param);
    reinterpret_cast<CBaseCur*>(base + 0xeb4)->setRootPaneTranslate(reinterpret_cast<const nw4r::math::VEC3*>(buf));
}


void MakeCrystal_CheckItemMenuDone(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();
    u32 trigger1, trigger2, cancel, dir;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        u32 p = pad->mPad.mPressedButtonFlags;
        u32 f = pad->mTurboPressButtonFlags;
        cancel = (f & 0x8004) != 0;
        dir = ((f & 0x10000) | (f & 0x8)) != 0;
        trigger1 = (p >> 21) & 1;
        trigger2 = (p >> 22) & 1;
    } else {
        u32 p = pad->mPad.mPressedButtonFlags;
        u32 f = pad->mTurboPressButtonFlags;
        cancel = (f & 0x8004) != 0;
        dir = ((f & 0x10000) | (f & 0x8)) != 0;
        trigger1 = (p >> 4) & 1;
        trigger2 = (p >> 5) & 1;
    }

    if (trigger1) {
        base[0xbdd] = 0x23;
        ((CBaseCur*)(base + 0xeb4))->SetVisible(0);
        reinterpret_cast<CSysWin*>(base + 0xe78)->advancePhase3();
        playUISound(3);
    } else if (trigger2) {
        base[0xbdd] = 0x23;
        base[0x2dd1] = 1;
        ((CBaseCur*)(base + 0xeb4))->SetVisible(0);
        reinterpret_cast<CSysWin*>(base + 0xe78)->advancePhase3();
        playUISound(6);
    } else if (cancel) {
        u8 v = base[0x2dd1] - 1;
        base[0x2dd1] = v;
        if ((s8)v < 0) base[0x2dd1] = 1;
        u8 param = base[0x2dd1];
        u8 buf[12];
        CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(buf), reinterpret_cast<CSysWin*>(base + 0xe78), param);
        reinterpret_cast<CBaseCur*>(base + 0xeb4)->setRootPaneTranslate(reinterpret_cast<const nw4r::math::VEC3*>(buf));
        playUISound(1);
    } else if (dir) {
        u8 v = base[0x2dd1] + 1;
        base[0x2dd1] = v;
        if ((s8)v > 1) base[0x2dd1] = 0;
        u8 param = base[0x2dd1];
        u8 buf[12];
        CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(buf), reinterpret_cast<CSysWin*>(base + 0xe78), param);
        reinterpret_cast<CBaseCur*>(base + 0xeb4)->setRootPaneTranslate(reinterpret_cast<const nw4r::math::VEC3*>(buf));
        playUISound(1);
    }
}

// Retail 0x80221654
// the crystal-success menu (confirm path 3 / cursor-forward path 0x20).
void MakeCrystal_StateSuccessMenu(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (reinterpret_cast<CSysWin*>(base + 0xe78)->isActive()) {
        if ((s8)base[0x2dd1] == 0) {
            base[0xbdd] = 0x24;
            CCharVoiceMan_FlushAll();
            MenuSnd_StopAllSlots_89C88();
            CTaskGame_stopVision();
            CUICfManager_queuePauseItemMenu(0x3, *reinterpret_cast<u32*>(base + 0xc), 0x0);
        } else {
            base[0xbdd] = 0x20;
            confirmMCGetItemBox((CMCGetItemBox*)(base + 0xecc));
        }
    }
}

void MakeCrystal_StateSuccessWait(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    // Same item-menu gate as MakeCrystal_StateItemMenuWait, but for the success state (0x1f).
    if (ItemMenu_IsPresent() == 0) {
        base[0xbdd] = 0x1f;
        forwardMCSubObj88(base + 0xecc, 1);
    }
}

void MakeCrystal_OpenResultDialog(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    base[0xbdd] = 0x26;
    base[0x2dd1] = 0x0;
    char* a = (char*)BdatTouchStringCell(&lbl_eu_805090FC[0x57], &lbl_eu_805090FC[0x63], 0x3d);
    char* b = (char*)BdatTouchStringCell(&lbl_eu_805090FC[0x57], &lbl_eu_805090FC[0x63], 0x3e);
    char* c = (char*)BdatTouchStringCell(&lbl_eu_805090FC[0x57], &lbl_eu_805090FC[0x63], 0x3f);
    reinterpret_cast<CSysWin*>(base + 0xe78)->switchKindPane(2);
    reinterpret_cast<CSysWin*>(base + 0xe78)->setKindContent(reinterpret_cast<char*>(a), 0);
    reinterpret_cast<CSysWin*>(base + 0xe78)->setTwoTextValues(reinterpret_cast<char*>(b), reinterpret_cast<char*>(c));
    reinterpret_cast<CSysWin*>(base + 0xe78)->applyKind(0);
    reinterpret_cast<CSysWin*>(base + 0xe78)->openPhase1();
}

void MakeCrystal_StateResultPrompt(CModelDispMakeCrystal* self) {
    u8* base = reinterpret_cast<u8*>(self);
    if (!reinterpret_cast<CSysWin*>(base + 0xe78)->isActive()) return;
    base[0xbdd] = 0x27;
    ((CBaseCur*)(base + 0xeb4))->SetVisible(1);
    u8 param = base[0x2dd1];
    u8 buf[16];
    CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(buf), reinterpret_cast<CSysWin*>(base + 0xe78), param);
    reinterpret_cast<CBaseCur*>(base + 0xeb4)->setRootPaneTranslate(reinterpret_cast<const nw4r::math::VEC3*>(buf));
}


void MakeCrystal_ReadResultInput(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();
    u32 trigger1, trigger2, cancel, dir;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        u32 f = pad->mTurboPressButtonFlags;
        u32 p = pad->mPad.mPressedButtonFlags;
        cancel = (f & 0x8004) != 0;
        dir = (f & 0x10008) != 0;
        trigger1 = (p >> 21) & 1;
        trigger2 = (p >> 22) & 1;
    } else {
        u32 f = pad->mTurboPressButtonFlags;
        u32 p = pad->mPad.mPressedButtonFlags;
        cancel = (f & 0x8004) != 0;
        dir = (f & 0x10008) != 0;
        trigger1 = (p >> 4) & 1;
        trigger2 = (p >> 5) & 1;
    }

    if (trigger1) {
        base[0xbdd] = 0x28;
        ((CBaseCur*)(base + 0xeb4))->SetVisible(0);
        reinterpret_cast<CSysWin*>(base + 0xe78)->advancePhase3();
        playUISound(3);
    } else if (trigger2) {
        base[0xbdd] = 0x28;
        base[0x2dd1] = 1;
        ((CBaseCur*)(base + 0xeb4))->SetVisible(0);
        reinterpret_cast<CSysWin*>(base + 0xe78)->advancePhase3();
        playUISound(6);
    } else if (cancel) {
        u8 v = base[0x2dd1] - 1;
        base[0x2dd1] = v;
        if ((s8)v < 0) base[0x2dd1] = 1;
        u8 param = base[0x2dd1];
        u8 buf[16];
        CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(buf), reinterpret_cast<CSysWin*>(base + 0xe78), param);
        reinterpret_cast<CBaseCur*>(base + 0xeb4)->setRootPaneTranslate(reinterpret_cast<const nw4r::math::VEC3*>(buf));
        playUISound(1);
    } else if (dir) {
        u8 v = base[0x2dd1] + 1;
        base[0x2dd1] = v;
        if ((s8)v > 1) base[0x2dd1] = 0;
        u8 param = base[0x2dd1];
        u8 buf[16];
        CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(buf), reinterpret_cast<CSysWin*>(base + 0xe78), param);
        reinterpret_cast<CBaseCur*>(base + 0xeb4)->setRootPaneTranslate(reinterpret_cast<const nw4r::math::VEC3*>(buf));
        playUISound(1);
    }
}

// Retail 0x8022196C: while the CSysWin dialog is open, mark the confirmation
// step active; if the cursor step is zero, flag the confirm UI as initialised.
void MakeCrystal_StateConfirmDone(CModelDispMakeCrystal* self)
{
    CModelDispMakeCrystalFull* full = reinterpret_cast<CModelDispMakeCrystalFull*>(self);
    if (reinterpret_cast<CSysWin*>(reinterpret_cast<u8*>(self) + 0xe78)->isActive()) {
        full->field_BDC = 3;
        if (full->field_2DD1 == 0) {
            full->field_2DD2 = 1;
        }
    }
}

// Retail symbol is the bare C-linkage name (defined below), so intra-TU
// calls emit the unmangled reloc.
extern "C" void MakeCrystal_ReleaseAnimObj(CModelDispMakeCrystal* self, u8* obj)
{
    CDeviceVI::waitForDrawDone();
    if (*reinterpret_cast<u32*>(obj) == 0) return;
    releaseAnimObj(obj + 0x8, *reinterpret_cast<void**>(obj + 0x4));
    reinterpret_cast<CActParamAnim*>(obj + 0x8)->initAnimState();
    Scn_IsAnimActiveOrNull(*reinterpret_cast<void**>(obj + 0x4));
    Scn_IsAnimActiveOrNull(reinterpret_cast<void*>(*reinterpret_cast<u32*>(obj + 0x0)));
    *reinterpret_cast<u32*>(obj + 0x0) = 0;
    // Release the 6 sub-buffers (stride 0xC stores pointer+flag pairs).
    for (u8 i = 0; i < 6; i++) {
        u8* e = obj + 0x568 + (u32)i * 0xC;
        if (e[4] != 0) {
            *reinterpret_cast<u32*>(e + 0) = 0;
            e[4] = 0;
        } else {
            void* p = *reinterpret_cast<void**>(e);
            if (p == 0) continue;
            mtl::MemManager::deallocate(p);
            *reinterpret_cast<u32*>(e) = 0;
        }
    }
    obj[0x5AC] = 0xFF;
}

extern "C" void MakeCrystal_ResetSlotList(CModelDispMakeCrystal* self, u8 arg4)
{
    u8* base = reinterpret_cast<u8*>(self);
    u8* entries = reinterpret_cast<CModelDispMakeCrystal*>(base)->mEntries;
    if (entries == nullptr) return;
    reinterpret_cast<CMCCrystalParamTable*>(entries)->sortSlots();
    crystalListClearSlots(base + 0xc18, arg4);
    u8 count = entries[0];
    u8 i;
    // Iterate the 8 crystal slot entries, wall-clock-offset by base[0x2dc2].
    for (i = 0; i < 8; i++) {
        int idx = (int)i + (s8)base[0x2dc2];
        if (idx >= count) return;
        u16 v4 = *reinterpret_cast<u16*>(entries + ((idx & 0xffff) << 3) + 4);
        u8 v8 = entries[((idx & 0xffff) << 3) + 8];
        u16 v2 = *reinterpret_cast<u16*>(entries + ((idx & 0xffff) << 3) + 2);
        func_80222B14(base + 0xc18, i, v2, v4);
        if (arg4 == 0) {
            crystalListUpdateSlot(base + 0xc18, i, v4, v8);
        }
        switch (v8) {
        case 0:
            if (v4 >= 0xc8) {
                entries[(((int)i + (s8)base[0x2dc2]) & 0xffff) * 8 + 8] = 1;
            }
            break;
        case 1:
            if (v4 >= 0x12c) {
                entries[(((int)i + (s8)base[0x2dc2]) & 0xffff) * 8 + 8] = 2;
            }
            break;
        }
    }
}

extern "C" void MakeCrystal_RefreshSlotEntries(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    u8 flagA = GetCollectedFlagByte(base[0x5f0]);
    u8 flagB = GetCollectedFlagByte(base[0xbbc]);
    u16 val = LookupRelateValue(flagA, flagB);
    s16 iv = (s16)(lbl_eu_806684FC * val + lbl_eu_806684F8);
    *reinterpret_cast<f32*>(base + 0x2dcc) = 0.0f;
    base[0x2dc8] = 0;
    *reinterpret_cast<u16*>(base + 0x2dc4) = iv;
    base[0x2dc6] = (u8)(iv / 30);
    base[0x2dc7] = (u8)(iv / 20);
    for (u8 k = 0; k < 7; k++) {
        if (k >= code80135FDC_getByte_64077()) {
            base[0x13b8 + k] = 0;
        } else {
            base[0x13b8 + k] = GetCollectedFlagByte(k);
        }
    }
    base[0x13b8 + (s8)base[0x5f0]] = 0;
    base[0x13b8 + (s8)base[0xbbc]] = 0;
    base[0x2dd3] = 0;
    u8* a = reinterpret_cast<u8*>(CtrlObjectParam_GetCharEntry(flagA));
    u8* b = a + 0x3534;
    if (Counter_TestBit(b, 0x90)) {
        base[0x2dd3] = (u8)(base[0x2dd3] + IdTable_SumValues((CUnkObj8025FB10*)b, 0x90));
    }
    u8* a2 = reinterpret_cast<u8*>(CtrlObjectParam_GetCharEntry(flagB));
    u8* b2 = a2 + 0x3534;
    if (Counter_TestBit(b2, 0x90)) {
        base[0x2dd3] = (u8)(base[0x2dd3] + IdTable_SumValues((CUnkObj8025FB10*)b2, 0x90));
    }
}

// Retail 0x80221D34: makecrystal state-machine dispatcher - dispatch the
// crystal-charge state byte (+0xbdd) through a dense jump table. States
// 0x16/0x1e/0x20 inline the file-state check (isMCGetItemBoxReady) and advance
// the state on success; every other state delegates to a state-runner.
void MakeCrystal_DispatchState(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    switch (base[0xbdd]) {
    case 0x00: MakeCrystal_StateSlotCheck(self); break;
    case 0x01: MakeCrystal_StateStartFx(self); break;
    case 0x02: MakeCrystal_TickRise(self); break;
    case 0x03: MakeCrystal_StateChargeStep(self); break;
    case 0x04: MakeCrystal_FinishRound(self); break;
    case 0x05: MakeCrystal_NopD3E4(self); break;
    case 0x06: MakeCrystal_StateChargeLoop(self); break;
    case 0x07: MakeCrystal_TrySlots(self); break;
    case 0x08: MakeCrystal_StateCrystalFull(self); break;
    case 0x09: MakeCrystal_StatePlacedDone(self); break;
    case 0x0a: MakeCrystal_StateFailWait(self); break;
    case 0x0b: MakeCrystal_StateDivideSel(self); break;
    case 0x0c: MakeCrystal_StateCrystalChk(self); break;
    case 0x0d: MakeCrystal_StateRankUp(self); break;
    case 0x0e: MakeCrystal_StateDivideDone(self); break;
    case 0x0f: MakeCrystal_StateCrystalDone(self); break;
    case 0x10: MakeCrystal_StateSuccessChk(self); break;
    case 0x11: MakeCrystal_NopDF84(self); break;
    case 0x12: MakeCrystal_DispatchTuneInput(self); break;
    case 0x13: MakeCrystal_FinalizeCylCount(self); break;
    case 0x14: MakeCrystal_StateCylDone(self); break;
    case 0x15: MakeCrystal_StateChargeFinal(self); break;
    case 0x16: MakeCrystal_PickPassFail(self); break;
    case 0x17:
        // Charged state: wait for the crystal-box file state machine to be
        // ready, then move to the pass/fail tuning state (0x18).
        if (((CMCGetItemBox*)(base + 0xecc))->isMCGetItemBoxReady()) base[0xbdd] = 0x18;
        break;
    case 0x18: MakeCrystal_MoveCursorPane(self); break;
    case 0x19: func_8021EF30(self); break;
    case 0x1a: MakeCrystal_StateSysWinPrompt(self); break;
    case 0x1b: MakeCrystal_ShowCursorPane(self); break;
    case 0x1c: MakeCrystal_StateConfirmStep(self); break;
    case 0x1d: MakeCrystal_StateItemMenuWait(self); break;
    case 0x1e:
        // Tuning-cancel path: wait for the file state machine, then jump to
        // the item-menu-open state (0x1f).
        if (((CMCGetItemBox*)(base + 0xecc))->isMCGetItemBoxReady()) base[0xbdd] = 0x1f;
        break;
    case 0x1f: func_8021F2D8(self); break;
    case 0x20:
        // Success confirm path: wait for the file state machine, then jump to
        // the success state (0x25).
        if (((CMCGetItemBox*)(base + 0xecc))->isMCGetItemBoxReady()) base[0xbdd] = 0x25;
        break;
    case 0x21: MakeCrystal_StateConfirmPrompt(self); break;
    case 0x22: MakeCrystal_CheckItemMenuDone(self); break;
    case 0x23: MakeCrystal_StateSuccessMenu(self); break;
    case 0x24: MakeCrystal_StateSuccessWait(self); break;
    case 0x25: MakeCrystal_OpenResultDialog(self); break;
    case 0x26: MakeCrystal_StateResultPrompt(self); break;
    case 0x27: MakeCrystal_ReadResultInput(self); break;
    case 0x28: MakeCrystal_StateConfirmDone(self); break;
    }
}

// Retail 0x80221F80: crystal placement state driver - dispatch on the
// buffered probability-state byte in sbss, place crystal entries for the
// state, and refresh the position/effect sub-objects. All three states end
// with MakeCrystal_SpawnCrystalItem(self, 2, 0).
void MakeCrystal_PlaceCrystals(CModelDispMakeCrystal* self)
{
    switch (sCrystalProbState[3]) {
    case 0: {
        // Single placement: pick a random slot, offset the crystal entry by
        // a random amount (extra +1 for char 2, or the streak count for
        // char 4), and write the entry.
        u8* entries = self->mEntries;
        u8 count = entries[0];
        ml::MTRand::getInstance();
        s8 slot = (s8)(count != 0 ? rand31__Q22ml6MTRandFv() % count : 0);
        ml::MTRand::getInstance();
        int rv8 = rand31__Q22ml6MTRandFv();
        int d = (s8)(rv8 - (rv8 / 8) * 8) + 3;
        u8 c = GetCollectedFlagByte(self->mSlots[0].field_5ac);
        if (c == 2) {
            ml::MTRand::getInstance();
            d += (s8)((s8)(rand31__Q22ml6MTRandFv() % 5) + 1);
        } else if (c == 4) {
            d = (s8)(sCrystalProbState[4] + 1);
        }
        setCrystalPosEntry(
            entries,
            (u16)slot,
            (u16)((s8)d + *(u16*)(entries + ((u16)slot << 3) + 4)),
            (u16)(s8)d);
        self->mEffUpRed.startInAnim();
        MakeCrystal_AdvanceSlotEntries(self);
        MakeCrystal_ResetSlotList(self, 0);
        playUISound(0x8d);
        break;
    }
    case 1: {
        // Multi placement: place one crystal per entry, each with a random
        // offset (+1 normally, extra +1 for char 7, streak count for char 4).
        u8* entries = self->mEntries;
        u8 count = entries[0];
        for (u8 i = 0; i < count; i++) {
            ml::MTRand::getInstance();
            if (count != 0) {
                rand31__Q22ml6MTRandFv();
            }
            ml::MTRand::getInstance();
            int d = (s8)(rand31__Q22ml6MTRandFv() % 5) + 1;
            u8 c = GetCollectedFlagByte(self->mSlots[0].field_5ac);
            if (c == 7) {
                ml::MTRand::getInstance();
                d += (s8)((s8)(rand31__Q22ml6MTRandFv() % 5) + 1);
            } else if (c == 4) {
                d = (s8)(sCrystalProbState[4] + 1);
            }
            setCrystalPosEntry(
                entries,
                i,
                (u16)((s8)d + *(u16*)(entries + ((u8)i << 3) + 4)),
                (u16)(s8)d);
        }
        self->mEffUpBlue.startInAnim();
        MakeCrystal_AdvanceSlotEntries(self);
        MakeCrystal_ResetSlotList(self, 0);
        playUISound(0x8c);
        break;
    }
    case 2: {
        // Gauge-tune: refresh the cylinder gauge fill with a char-dependent
        // constant, then flush the effect list.
        u8 c = GetCollectedFlagByte(self->mSlots[0].field_5ac);
        if (c == 5) {
            self->mGauge.addFillValue(
                lbl_eu_80668500);
        } else {
            self->mGauge.addFillValue(
                0.0f);
        }
        self->mEffUpGreen.startInAnim();
        playUISound(0xb0);
        break;
    }
    }
    MakeCrystal_SpawnCrystalItem(self, 2, 0);
}

// Count the used crystal slots (entry byte +9 set) and number them.
// The halfword at entry+6 rides unmasked into the u8 setNumber param
// (retail passes the raw lhz result; MWCC emits no narrowing mask).
// Defined after MakeCrystal_PlaceCrystals so MWCC cannot inline it (retail calls it).
extern "C" void MakeCrystal_AdvanceSlotEntries(void* selfp)
{
    u8* base = reinterpret_cast<u8*>(selfp);
    CModelDispMakeCrystal* self = reinterpret_cast<CModelDispMakeCrystal*>(selfp);
    u8 slot = 0;
    u8* entries = reinterpret_cast<CModelDispMakeCrystal*>(base)->mEntries;
    u8 entryCount = entries[0];
    u8 i = 0;
    while (i < entryCount) {
        u8* e = entries + ((u32)i << 3);
        if (e[9] != 0) {
            // slot++ as an argument: retail masks the pre-increment value
            // into r4 and schedules the increment before the call.
            setNumber__11CMCEffUpPrmFUlUc(
                reinterpret_cast<CMCEffUpPrm*>(base + 0xcec), slot++,
                *reinterpret_cast<u16*>(e + 6));
        }
        i++;
    }
    reinterpret_cast<CMCEffUpPrm*>(base + 0xcec)->activateSlots(slot);
}

// Second item-impl pad folded onto the same CItemImpl owner above:
// +0x58=vf58, +0x94=vf94. Call sites in MakeCrystal_AppendItemSteps use
// ((CItemImpl*)CItem_initItemImplInstances(...))->vfXX directly.

// Retail 0x80222230: cylinder-count finalize - resolve the item-source row
// for the current char, build the item record, compute the display count
// from the bdat columns, and append the step(s) to the step list.
void MakeCrystal_AppendItemSteps(void* selfp)
{
    u8* base = reinterpret_cast<u8*>(selfp);
    u8* entries = reinterpret_cast<CModelDispMakeCrystal*>(base)->mEntries;
    if (entries == nullptr) return;
    void* g1 = ::cf::CfBdat::spItmItemListFileData;
    u16 n = *(u16*)(entries + 2);
    u16 count = *(u16*)(entries + 4);
    u8 ch = entries[1];
    // Walk the bdat enum rows backwards to find the row whose column 3
    // value is 3 (the row key for this item source).
    u16 idx = (u16)(Bdat_GetMaxRow_B1EC(g1) - 1);
    while (idx != 0) {
        // Row-index variant of the bdat column lookup (CUIWindowManager.hpp
        // declares the int third arg; the const char* variant is another TU's).
        u32 word = (u32)getBdatStringColumnValue(
            g1, &lbl_eu_805090FC[0x68], (int)idx);
        if (*reinterpret_cast<u16*>(&word) == 3) break;
        idx--;
    }
    if (count >= 0xc8) ch = (u8)(ch + 1);
    CMCItemData item;
    item.field_00 = 0;
    *reinterpret_cast<u16*>(&item.field_04) = 0;
    u8 tier;
    if (ch < 1) {
        tier = 1;
    } else if (ch > 6) {
        tier = 6;
    } else {
        tier = ch;
    }
    CItem_initSlot(&item, idx, n, tier);
    ((CItemImpl*)CItem_initItemImplInstances((CItemData*)&item))->vf58((CItemData*)&item, n);
    void* g2 = ::cf::CfBdat::spBtlSkillListFileData;
    u32 first = 0;
    u32 second = 0;
    // Per-arm string-table base: one address materialization shared by both
    // column lookups (retail keeps the base in r28 across the pair).
    switch (tier) {
    case 1: {
        const char* t = &lbl_eu_805090FC[0];
        first = BdatGetU8Direct((const void*)(reinterpret_cast<u32>(g2)), t + 0x71, n);
        second = BdatGetU8Direct((const void*)(reinterpret_cast<u32>(g2)), t + 0x79, n);
        break;
    }
    case 2: {
        const char* t = &lbl_eu_805090FC[0];
        first = BdatGetU8Direct((const void*)(reinterpret_cast<u32>(g2)), t + 0x81, n);
        second = BdatGetU8Direct((const void*)(reinterpret_cast<u32>(g2)), t + 0x89, n);
        break;
    }
    case 3: {
        const char* t = &lbl_eu_805090FC[0];
        first = BdatGetU8Direct((const void*)(reinterpret_cast<u32>(g2)), t + 0x91, n);
        second = BdatGetU8Direct((const void*)(reinterpret_cast<u32>(g2)), t + 0x99, n);
        break;
    }
    case 4: {
        const char* t = &lbl_eu_805090FC[0];
        first = BdatGetU8Direct((const void*)(reinterpret_cast<u32>(g2)), t + 0xa1, n);
        second = BdatGetU8Direct((const void*)(reinterpret_cast<u32>(g2)), t + 0xa9, n);
        break;
    }
    case 5: {
        const char* t = &lbl_eu_805090FC[0];
        first = BdatGetU8Direct((const void*)(reinterpret_cast<u32>(g2)), t + 0xb1, n);
        second = BdatGetU8Direct((const void*)(reinterpret_cast<u32>(g2)), t + 0xb9, n);
        break;
    }
    case 6: {
        const char* t = &lbl_eu_805090FC[0];
        first = BdatGetU8Direct((const void*)(reinterpret_cast<u32>(g2)), t + 0xc1, n);
        second = BdatGetU8Direct((const void*)(reinterpret_cast<u32>(g2)), t + 0xc9, n);
        break;
    }
    }
    u8 result;
    u8 copyCount = 1;
    if (count >= 0x12c) {
        result = (u8)second;
        copyCount = 2;
    } else {
        // Percent-based split of the display count between the two columns:
        // A = count * scale, B = A * scale2, countB = count - B, then
        // result = diff * countB * scale + first (all truncating).
        u16 a16 = (u16)((s32)((float)count * lbl_eu_806684EC));
        u8 diff = (u8)(second - first);
        s32 b = (s32)((float)a16 * lbl_eu_806684F8);
        u16 countB = (u16)(count - b);
        result = (u8)((s32)((float)diff * ((float)countB * lbl_eu_806684EC)) + first);
    }
    ((CItemImpl*)CItem_initItemImplInstances((CItemData*)&item))->vf94((CItemData*)&item, result);
    // Append the step record (stride 0x34 at +0x13c0) the requested times.
    for (u8 k = 0; k < copyCount; k++) {
        u8 stepN = base[0x2dc0];
        base[0x2dc0] = (u8)(stepN + 1);
        CItem_copyRecMasked(base + 0x13c0 + stepN * 0x34, &item);
    }
    reinterpret_cast<CMCCrystalParamTable*>(entries)->clearSlot(0);
    reinterpret_cast<CMCCrystalParamTable*>(entries)->refreshCount();
    MakeCrystal_ResetSlotList(reinterpret_cast<CModelDispMakeCrystal*>(selfp), 0);
    reinterpret_cast<CScrollBar*>(base + 0xe38)->UpdateThumb(8, entries[0]);
    crystalListResetAnim4Play(base + 0xc18);
}

// Retail 0x802225D4: teardown of the make-crystal UI state. Waits for the
// draw engine, releases the file handles, frees the crystal-box file state
// and display buffers, then destroys the per-slot crystal display substructs
// and every effect sub-object.
void MakeCrystal_TeardownUI(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (base[0x2dd4] == 0) {
        CDeviceVI::waitForDrawDone();
        closeFileHandle(reinterpret_cast<CFileHandle**>(base + 0x18));
        closeFileHandle(reinterpret_cast<CFileHandle**>(base + 0x24));
        void* arc = &lbl_eu_8065FC18[0];
        if (arc != nullptr) {
            // Release the file-state machine's object references.
            if (*reinterpret_cast<u32*>(base + 0x10) != 0) {
                schedClearFlag15Update(*reinterpret_cast<void**>(base + 0x10));
            }
            if (*reinterpret_cast<u32*>(base + 0x14) != 0) {
                schedClearFlag15Update(*reinterpret_cast<void**>(base + 0x14));
            }
            if (*reinterpret_cast<u32*>(base + 0x1c) != 0) {
                EffSched_LookupB(arc, *reinterpret_cast<void**>(base + 0x1c));
            }
        }
        base[0x10] = 0;
        base[0x14] = 0;
        if (*reinterpret_cast<u32*>(base + 0x1c) != 0) {
            mtl::MemManager::deallocate(*reinterpret_cast<void**>(base + 0x1c));
            base[0x1c] = 0;
        }
        if (*reinterpret_cast<u32*>(base + 0x20) != 0) {
            Scn_IsAnimActiveOrNull(*reinterpret_cast<void**>(base + 0x20));
            base[0x20] = 0;
        }
        if (*reinterpret_cast<u32*>(base + 0x28) != 0) {
            mtl::MemManager::deallocate(*reinterpret_cast<void**>(base + 0x28));
            base[0x28] = 0;
        }
        // Release the per-slot file handles (2 slots x 6 handles each).
        for (u8 i = 0; i < 2; i++) {
            u8* sub = base + (u32)i * 0x5cc + 0x44;
            for (u8 j = 0; j < 6; j++) {
                closeFileHandle(reinterpret_cast<CFileHandle**>(sub + (u32)j * 0xc + 0x564));
            }
        }
        // Release the built display models for both slots.
        for (u8 i = 0; i < 2; i++) {
            MakeCrystal_ReleaseAnimObj(self, base + (u32)i * 0x5cc + 0x44);
        }
        // Destroy the gauge, list, support, and every effect sub-object.
        reinterpret_cast<CMCCylinderGauge*>(base + 0xbec)->destroy();
        __dt__80222984(base + 0xc18);
        CrySupUnloadLayout(base + 0xc70);
        reinterpret_cast<CMCEffStart*>(base + 0xc8c)->destroy();
        reinterpret_cast<CMCEffStart*>(base + 0xca4)->destroy();
        reinterpret_cast<CMCEffStart*>(base + 0xcbc)->destroy();
        reinterpret_cast<CMCEffStart*>(base + 0xcd4)->destroy();
        reinterpret_cast<CMCEffUpPrm*>(base + 0xcec)->destroy();
        reinterpret_cast<CMCEffSuccess*>(base + 0xd5c)->destroy();
        reinterpret_cast<CMCEffStart*>(base + 0xd78)->destroy();
        reinterpret_cast<CMCEffCrystal*>(base + 0xd90)->destroy();
        reinterpret_cast<CMCEffStart*>(base + 0xdcc)->destroy();
        reinterpret_cast<CMCEffStart*>(base + 0xde4)->destroy();
        reinterpret_cast<CMCEffCylinder*>(base + 0xdfc)->destroy();
        reinterpret_cast<CBaseCur*>(base + 0xe20)->cleanup();
        reinterpret_cast<CScrollBar*>(base + 0xe38)->Teardown();
        base[0x2dd4] = 1;
    }
}

// Retail 0x802227AC: crystal-item spawn dispatcher - picks a crystal object
// by the selection code (jumptable on sel), computes a position offset
// (random sign for the random cases, char-dependent slot for others), and
// spawns it through the vision helper CTaskGame_openVision.
extern "C" void MakeCrystal_SpawnCrystalItem(void* selfp, int sel, u8 chIn)
{
    u8* base = reinterpret_cast<u8*>(selfp);
    if (base[0x2dd5] != 0) {
        base[0x2dd5] = 0;
        return;
    }
    u8 ch = 0;
    u16 off = 0;
    // Random ±1 sign from the low bit of a random value (signed LSB).
    u32 r;
    s8 s;
    switch ((u32)sel) {
    case 0:
        ch = chIn;
        off = 0xe1f;
        break;
    case 1:
        ch = chIn;
        ml::MTRand::getInstance();
        r = rand31__Q22ml6MTRandFv();
        s = (s8)(((r & 1) ^ (r >> 31)) - (r >> 31));
        off = (u16)(0xe12 - (s == 1));
        break;
    case 2:
        ml::MTRand::getInstance();
        r = rand31__Q22ml6MTRandFv();
        s = (s8)(((r & 1) ^ (r >> 31)) - (r >> 31));
        ch = GetCollectedFlagByte(base[0x5f0 + s * 0x5cc]);
        ml::MTRand::getInstance();
        r = rand31__Q22ml6MTRandFv();
        s = (s8)(((r & 1) ^ (r >> 31)) - (r >> 31));
        off = (u16)(0xe14 - (s == 1));
        break;
    case 3:
        ch = chIn;
        ml::MTRand::getInstance();
        r = rand31__Q22ml6MTRandFv();
        s = (s8)(((r & 1) ^ (r >> 31)) - (r >> 31));
        off = (u16)(0xe1a - (s == 1));
        break;
    case 4:
        ml::MTRand::getInstance();
        r = rand31__Q22ml6MTRandFv();
        s = (s8)(((r & 1) ^ (r >> 31)) - (r >> 31));
        ch = GetCollectedFlagByte(base[0x5f0 + s * 0x5cc]);
        ml::MTRand::getInstance();
        r = rand31__Q22ml6MTRandFv();
        s = (s8)(((r & 1) ^ (r >> 31)) - (r >> 31));
        off = (u16)(0xe18 - (s == 1));
        break;
    case 5:
        ch = chIn;
        ml::MTRand::getInstance();
        r = rand31__Q22ml6MTRandFv();
        s = (s8)(((r & 1) ^ (r >> 31)) - (r >> 31));
        off = (u16)(0xe1c - (s == 1));
        break;
    case 6:
        ml::MTRand::getInstance();
        r = rand31__Q22ml6MTRandFv();
        s = (s8)(((r & 1) ^ (r >> 31)) - (r >> 31));
        ch = GetCollectedFlagByte(base[0x5f0 + s * 0x5cc]);
        ml::MTRand::getInstance();
        r = rand31__Q22ml6MTRandFv();
        s = (s8)(((r & 1) ^ (r >> 31)) - (r >> 31));
        off = (u16)(0xe1e - (s == 1));
        break;
    case 7:
        ml::MTRand::getInstance();
        r = rand31__Q22ml6MTRandFv();
        s = (s8)(((r & 1) ^ (r >> 31)) - (r >> 31));
        ch = GetCollectedFlagByte(base[0x5f0 + s * 0x5cc]);
        ml::MTRand::getInstance();
        r = rand31__Q22ml6MTRandFv();
        s = (s8)(((r & 1) ^ (r >> 31)) - (r >> 31));
        off = (u16)(0xe16 - (s == 1));
        break;
    }
    CTaskGame_openVision((u8)ch, off, (u32)getHandleMEM2__Q23mtl10MemManagerFv(), 2, 1,
                  1.0f);
}

// Virtual dispatch at vtable offset +0xA8 (index 0x2a), returns a pointer.
// MWCC reserves 2 hidden vtable slots (RTTI): declared index 0x28 => +0xA8.
void MakeCrystal_SetupDispBuffers(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    // One-time setup: allocate the crystal display buffers and, if the file
    // reference is present, spin up the crystal support object. The object is
    // kept in base+0x20 and reloaded before every use (retail never hoists it).
    if (base[0xbe9] == 0 &&
        *reinterpret_cast<u32*>(base + 0x24) == 0 &&
        *reinterpret_cast<u32*>(base + 0x18) == 0 &&
        *reinterpret_cast<u32*>(base + 0x28) != 0 &&
        *reinterpret_cast<u32*>(base + 0x1c) != 0 &&
        *reinterpret_cast<u32*>(base + 0x20) == 0 &&
        *reinterpret_cast<u32*>(base + 0x10) == 0) {
        // Retail reuses the base+0x28 value (still in r4 from the check) as
        // the allocator's second argument.
        *reinterpret_cast<void**>(base + 0x20) =
            Scn_SetupAnim(*reinterpret_cast<void**>(base + 0xc),
                          *reinterpret_cast<u32*>(base + 0x28), -1, 1);
        simSetLeafFlag4000(*reinterpret_cast<void**>(base + 0x20), 1);
        simMarkTree100000(*reinterpret_cast<void**>(base + 0x20), 1);
        reinterpret_cast<CScnItemModel*>(*reinterpret_cast<void**>(base + 0x20))->vfunc48(
            *reinterpret_cast<f32*>(base + 0xbe0));
        void* p1 = simGetLeafActData(*reinterpret_cast<void**>(base + 0x20));
        u8* s1 = reinterpret_cast<u8*>(sCrystalLeafVecA);
        u8* o1 = reinterpret_cast<u8*>(p1);
        *reinterpret_cast<u32*>(o1 + 0x0) = *reinterpret_cast<u32*>(s1 + 0x0);
        *reinterpret_cast<u32*>(o1 + 0x4) = *reinterpret_cast<u32*>(s1 + 0x4);
        *reinterpret_cast<u32*>(o1 + 0x8) = *reinterpret_cast<u32*>(s1 + 0x8);
        void* p2 = simGetLeafActData(*reinterpret_cast<void**>(base + 0x20));
        u8* s2 = reinterpret_cast<u8*>(sCrystalLeafVecB);
        u8* o2 = reinterpret_cast<u8*>(p2);
        *reinterpret_cast<u32*>(o2 + 0xc) = *reinterpret_cast<u32*>(s2 + 0x0);
        *reinterpret_cast<u32*>(o2 + 0x10) = *reinterpret_cast<u32*>(s2 + 0x4);
        *reinterpret_cast<u32*>(o2 + 0x14) = *reinterpret_cast<u32*>(s2 + 0x8);
        void* arc = &lbl_eu_8065FC18[0];
        if (arc != nullptr) {
            void* e = EffRoot_AddSchedReq(arc, *reinterpret_cast<void**>(base + 0x1c),
                                    *reinterpret_cast<void**>(base + 0xc), 0, 1, 1);
            *reinterpret_cast<void**>(base + 0x10) = e;
            if (e != nullptr) {
                schedAttachChildSlot(e, self ? reinterpret_cast<void*>(base + 0x8) : nullptr);
                u32 r = reinterpret_cast<CScnItemModel*>(*reinterpret_cast<void**>(base + 0x20))->vfuncA8();
                *reinterpret_cast<u32*>(reinterpret_cast<u8*>(*reinterpret_cast<void**>(base + 0x10)) + 0x14) = r;
            }
        }
        base[0xbe9] = 1;
    }

    // Refresh each of the two crystal substructs. A negative state skips the
    // slot entirely; 0..2 runs the buffer fill, >= 3 runs the release path.
    for (u8 i = 0; i < 2; i++) {
        u8* s = base + (u32)i * 0x5cc;
        if (base[(u32)i * 0x5cc + 0x60c] == 0) {
            s8 v = reinterpret_cast<s8*>(s + 0x44)[0x5ac];
            if (v >= 0) {
                if (v < 3) {
                    MakeCrystal_BuildCrystalList(self, reinterpret_cast<CMCrystalDispSub*>(s + 0x44));
                } else {
                    MakeCrystal_RebuildSubStruct(self, s + 0x44);
                }
            }
        }
    }
}

// Filter-value triple at lbl_eu_805090D8; accessed through a struct pointer
// so MWCC materializes the address with a single lis + lwzu pair (retail).
struct CMCryFilters {
    u32 v0, v1, v2;
};

void MakeCrystal_BuildCrystalList(CModelDispMakeCrystal* self, CMCrystalDispSub* sub)
{
    // Copy the 3 state-specific enum filter types first (retail statement order).
    const CMCryFilters* filt = reinterpret_cast<const CMCryFilters*>(lbl_eu_805090D8);
    u32 filters[3];
    filters[0] = filt->v0;
    filters[1] = filt->v1;
    filters[2] = filt->v2;

    CfEnumListHolder holder;

    // Select the crystal list for the current char state and check it's loaded.
    startEnumObjects(holder.get(), filters[(s8)sub->field_5ac], 0);
    if (*reinterpret_cast<u32*>(reinterpret_cast<u8*>(holder.get()) + 0x620) == 0) {
        MakeCrystal_ReleaseAnimObj(self, reinterpret_cast<u8*>(sub));
        return;
    }

    // Grab the first crystal object from the enum list.
    void* cfMove = *reinterpret_cast<void**>(
        reinterpret_cast<u8*>(getEntryAt(holder.get(), 0)) + 4);
    if (cfMove == nullptr) {
        MakeCrystal_ReleaseAnimObj(self, reinterpret_cast<u8*>(sub));
        return;
    }

    CMCCryChgActor* actor =
        reinterpret_cast<CMCCryChgActor*>(getCfObjectPc__FPQ22cf12CfObjectMove(cfMove));
    int ready = actor != nullptr ? 1 : 0;
    u32 crystalCount = 0;
    if (actor != nullptr) {
        crystalCount = actor->field_3f2c;
        if (crystalCount == 0) ready = 0;
        if (reinterpret_cast<cf::CfObject*>(&actor->move)->CfObject_isMoveActiveNow() == 0) ready = 0;
        if (sub->field_5a4 == 0) {
            if (sub->field_5a0 == 0) {
                // Load the crystal model file for this actor.
                char buf[0x20];
                sprintf(buf, &lbl_eu_805090FC[0xd1],
                        actor->field_3f28 == 3 ? 8 : (int)actor->field_3f28,
                        0, 10);
                sub->field_5a0 =
                    reinterpret_cast<u32>(readFile__11CDeviceFileFUlPCcP10IWorkEventii(
                        getHandleMEM2__Q23mtl10MemManagerFv(), buf,
                        reinterpret_cast<void*>(self), 0, 0));
            }
            ready = 0;
        }
    }

    CMCCrySelfFields* objs = reinterpret_cast<CMCCrySelfFields*>(self);
    if (sub->field_00 == nullptr && ready != 0) {
        // Build the crystal display model for this slot.
        sub->field_00 = Scn_SetupAnim(objs->field_0c, crystalCount, -1, 1);
        sub->mCrystalVals[1] =
            (reinterpret_cast<cf::CfObject*>(&actor->move)->CfObject_getSlotBits(1) >> 12) &
            0x3ff;
        s16 be = CfObjectMove_getSubB0FieldA(&actor->move);
        CMCCryParamSlot* params =
            reinterpret_cast<CMCCryParamSlot*>(CfRes_getPcGridEntry(be, 0));
        // Crystal attachment points 2..5; do-while keeps the retail loop shape.
        u8 idx = 2;
        do {
            if (reinterpret_cast<cf::CfObject*>(&actor->move)->CfObject_getSlotBits(idx) != 0) {
                void* obj = params[idx].field_2c->getResourceBase(&params[idx], actor->field_3f28);
                simAttachModel(sub->field_00, obj);
                sub->mCrystalVals[idx] =
                    (reinterpret_cast<cf::CfObject*>(&actor->move)->CfObject_getSlotBits(idx) >>
                     12) &
                    0x3ff;
            }
        } while (++idx <= 5);
        u32 handle = sub->field_5a4;
        sub->field_04 = initMcaFile(objs->field_0c, handle, &lbl_eu_805090FC[0xef]);
        reinterpret_cast<CActParamAnim*>(reinterpret_cast<u8*>(sub) + 0x8)->initAnimState();
        attachAnimObj(reinterpret_cast<u8*>(sub) + 0x8, sub->field_00, sub->field_04, handle);
        sub->field_14 |= 0x160;
        ModelDispEquip_SetActParamParent(reinterpret_cast<u8*>(sub) + 0x8,
                      self ? reinterpret_cast<void*>(reinterpret_cast<u8*>(self) + 4)
                           : reinterpret_cast<void*>(self));
        u32* posA = reinterpret_cast<u32*>(simGetLeafActData(sub->field_00));
        posA[0] = sub->field_5b0;
        posA[1] = sub->field_5b4;
        posA[2] = sub->field_5b8;
        u32* posB = reinterpret_cast<u32*>(simGetLeafActData(sub->field_00));
        posB[3] = sub->field_5bc;
        posB[4] = sub->field_5c0;
        posB[5] = sub->field_5c4;
        reinterpret_cast<CScnItemModel*>(sub->field_00)->vfunc48(objs->field_be0);
        reinterpret_cast<CScnItemModel*>(sub->field_00)->vfunc9C(3, 0);
        if (sub->field_00 != nullptr) {
            func_8004B9D4(reinterpret_cast<u8*>(sub) + 0x8, 0x21, 0, -1, 0);
        }
        sub->field_5c8 = 1;
    } else if (sub->field_00 != nullptr && ready == 0) {
        MakeCrystal_ReleaseAnimObj(self, reinterpret_cast<u8*>(sub));
    }

}

// Retail 0x80223018: crystal display sub-struct release/rebuild path (state
// >= 3). Reload the crystal values from the char data, re-arm any missing
// crystal buffers, then rebuild the display model exactly like the initial
// build path (shared tail with MakeCrystal_BuildCrystalList).
void MakeCrystal_RebuildSubStruct(CModelDispMakeCrystal* self, u8* subp)
{
    CMCrystalDispSub* sub = reinterpret_cast<CMCrystalDispSub*>(subp);
    if (sub->field_00 != 0) return;
    CtrlObjectParam_GetSlotTableBase();
    u8 ch = (u8)GetCollectedFlagByte((u8)sub->field_5ac);
    if (ch == 0) return;
    u8* data = reinterpret_cast<u8*>(CtrlObjectParam_GetCharEntry(ch));
    sub->mCrystalVals[1] = bdatPcPartsRes(ch, (s16)(u16)*(u16*)(data + 0xe), 1, 0);
    sub->mCrystalVals[2] = bdatPcPartsRes(ch, (s16)(u16)*(u16*)(data + 0x10), 2, 0);
    sub->mCrystalVals[3] = bdatPcPartsRes(ch, (s16)(u16)*(u16*)(data + 0x12), 3, 0);
    sub->mCrystalVals[4] = bdatPcPartsRes(ch, (s16)(u16)*(u16*)(data + 0x14), 4, 0);
    sub->mCrystalVals[5] = bdatPcPartsRes(ch, (s16)(u16)*(u16*)(data + 0x16), 5, 0);

    // Re-arm the 5 crystal buffers (slot stride 0xc at +0x568): each value
    // still set and not already loaded is re-requested; if any buffer was
    // (re)loaded, the model must be rebuilt afterwards.
    int anyLoaded = 1;
    for (u32 i = 1; i <= 5; i++) {
        if (*(u32*)(subp + 0x544 + i * 4) != 0) {
            char pathBuf[96];
            u32 outTag = 0;
            pathBuf[0] = 0;
            if (Tok_FormatStr(*reinterpret_cast<ml::FixStr<64>*>(pathBuf), *(u32*)(subp + 0x544 + i * 4), 1, 1) != 0) {
                u8* slot = subp + 0x568 + (i - 1) * 0xc;
                if (*reinterpret_cast<u32*>(slot) == 0) {
                    anyLoaded = 0;
                    if (CfRes_isGridLoadIdle() != 0) {
                        slot[4] = 1;
                        *reinterpret_cast<u32*>(slot) =
                            reinterpret_cast<u32>(CfRes_tryResolveToken(*(u32*)(subp + 0x544 + i * 4), &outTag));
                        if (outTag == 0xffffffff) {
                            syncFieldData__Q22cf13CfGameManagerFv(ch, false);
                        }
                    }
                }
            }
        }
    }

    // Ensure the model file is loaded before building; loading (or having a
    // stale load pending) forces a rebuild below.
    if (sub->field_5a4 == 0) {
        if (sub->field_5a0 == 0) {
            if (ch == 3) ch = 8;
            char buf[0x10];
            sprintf(buf, &lbl_eu_805090FC[0xd1], ch, 0, 10);
            sub->field_5a0 = reinterpret_cast<u32>(
                readFile__11CDeviceFileFUlPCcP10IWorkEventii(
                    getHandleMEM2__Q23mtl10MemManagerFv(), buf, self, 0, 0));
        }
        anyLoaded = 0;
    }
    if (anyLoaded == 0) {
        // Build the display model from the re-armed buffers (shared tail
        // with MakeCrystal_BuildCrystalList's initial build).
        sub->field_00 = Scn_SetupAnim(
            *reinterpret_cast<void**>(reinterpret_cast<u8*>(self) + 0xc),
            *reinterpret_cast<u32*>(subp + 0x568), -1, 1);
        simSetLeafFlag4000(sub->field_00, 1);
        simMarkTree100000(sub->field_00, 1);
        if (sub->field_00 == 0) return;
        simAttachModel(sub->field_00, *reinterpret_cast<void**>(subp + 0x574));
        simAttachModel(sub->field_00, *reinterpret_cast<void**>(subp + 0x580));
        simAttachModel(sub->field_00, *reinterpret_cast<void**>(subp + 0x58c));
        simAttachModel(sub->field_00, *reinterpret_cast<void**>(subp + 0x598));
        u32 f5a4 = sub->field_5a4;
        sub->field_04 = initMcaFile(
            *reinterpret_cast<void**>(reinterpret_cast<u8*>(self) + 0xc), f5a4,
            &lbl_eu_805090FC[0xef]);
        reinterpret_cast<CActParamAnim*>(subp + 0x8)->initAnimState();
        attachAnimObj(subp + 0x8, sub->field_00, sub->field_04, f5a4);
        sub->field_14 |= 0x160;
        ModelDispEquip_SetActParamParent(
            subp + 0x8,
            self ? reinterpret_cast<void*>(reinterpret_cast<u8*>(self) + 4)
                 : reinterpret_cast<void*>(self));
        u32* g0 = reinterpret_cast<u32*>(simGetLeafActData(sub->field_00));
        g0[0] = sub->field_5b0;
        g0[1] = sub->field_5b4;
        g0[2] = sub->field_5b8;
        u32* g1 = reinterpret_cast<u32*>(simGetLeafActData(sub->field_00));
        g1[3] = sub->field_5bc;
        g1[4] = sub->field_5c0;
        g1[5] = sub->field_5c4;
        reinterpret_cast<CScnItemModel*>(sub->field_00)->vfunc48(
            *reinterpret_cast<f32*>(reinterpret_cast<u8*>(self) + 0xbe0));
        reinterpret_cast<CScnItemModel*>(sub->field_00)->vfunc9C(3, 0);
        if (sub->field_00 != 0) {
            func_8004B9D4(subp + 0x8, 0x21, 0, -1, 0);
        }
        sub->field_5c8 = 1;
    }
}





// Retail 0x8022338C: async file-event handler for the make-crystal UI load.
// Dispatch on which pending readFile handle completed: the crystal-list file
// (this+0x24), the item-source file (this+0x18), or the layout archive
// (this+0x3c) which drives the whole sub-object construction; anything else
// is a per-slot crystal model file.
bool CModelDispMakeCrystal::OnFileEvent(CEventFile* ev)
{
    u8* base = reinterpret_cast<u8*>(this);
    // Case 1: crystal-list file completed - move the record link to +0x28.
    void* f24 = *reinterpret_cast<void**>(base + 0x24);
    if (f24 == ev->mFileHandle) {
        void* nxt = *reinterpret_cast<void**>(reinterpret_cast<u8*>(f24) + 4);
        *reinterpret_cast<void**>(reinterpret_cast<u8*>(f24) + 4) = 0;
        *reinterpret_cast<void**>(base + 0x28) = nxt;
        *reinterpret_cast<void**>(base + 0x24) = 0;
        return 1;
    }
    // Case 2: item-source file completed - release the layout-state object.
    void* f18 = *reinterpret_cast<void**>(base + 0x18);
    if (f18 == ev->mFileHandle) {
        void* nxt = *reinterpret_cast<void**>(reinterpret_cast<u8*>(f18) + 4);
        // Single named pointer introduced early: MWCC splits the @ha/@l
        // pair around the pending stores and folds the null test into
        // the @l addic.
        void* arc = &lbl_eu_8065FC18[0];
        *reinterpret_cast<void**>(reinterpret_cast<u8*>(f18) + 4) = 0;
        *reinterpret_cast<void**>(base + 0x1c) = nxt;
        if (arc != nullptr) EffSched_LookupA(arc);
        *reinterpret_cast<void**>(base + 0x18) = 0;
        return 1;
    }
    // Case 3: the layout archive completed - build every sub-object.
    void* f3c = *reinterpret_cast<void**>(base + 0x3c);
    if (f3c == ev->mFileHandle) {
        // Stack scratch, declaration order matches the retail frame
        // (cls858@0x8 .. effUpPrm@0x1b0).
        u8 cls858[8];
        CCur18 cur2;
        CCur18 cur1;
        u8 effCylinder[0x20];
        u8 effDivide[0x18];
        u8 effUpRank[0x18];
        u8 effFailure[0x18];
        u8 effSuccess[0x20];
        u8 effUpGreen[0x18];
        u8 effUpBlue[0x18];
        u8 effUpRed[0x18];
        u8 effStart[0x18];
        u8 crySupport[0x1c];
        u8 effCrystal[0x3c];
        u8 cylGauge[0x30];
        u8 effUpPrm[0x70];
        // Create the layout region and archive accessor.
        createRegion__17UnkClass_8045F564FiiPCci(
            base + 0x2c, getHandleMEM2__Q23mtl10MemManagerFv(), 0x2ff000,
            &lbl_eu_805090FC[0xf3], 0);
        __ct__14Class_8045F858FP17UnkClass_8045F564(cls858, base + 0x2c);
        void* fileData =
            *reinterpret_cast<void**>(reinterpret_cast<u8*>(f3c) + 4);
        *reinterpret_cast<void**>(reinterpret_cast<u8*>(f3c) + 4) = 0;
        setMemInitFlag__Q23mtl10MemManagerFb(false);
        setMemInitFlag__Q23mtl10MemManagerFb(false);
        *reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40) =
            reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                CLibLayout::createArcResourceAccessor());
        Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(
            *reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40),
            fileData, &lbl_eu_805090FC[0x109]);
        // Cylinder gauge: build on the stack, copy, destroy. The accessor
        // is re-loaded from this+0x40 before every ctor (retail shape).
        __ct__16CMCCylinderGaugeFPQ34nw4r3lyt19ArcResourceAccessor(
            cylGauge,
            *reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40));
        *(u32*)(base + 0xbf0) = *(u32*)(cylGauge + 4);
        *(u32*)(base + 0xbf4) = *(u32*)(cylGauge + 8);
        *(u32*)(base + 0xbf8) = *(u32*)(cylGauge + 0xc);
        *(u32*)(base + 0xbfc) = *(u32*)(cylGauge + 0x10);
        *(u32*)(base + 0xc00) = *(u32*)(cylGauge + 0x14);
        *(u32*)(base + 0xc04) = *(u32*)(cylGauge + 0x18);
        base[0xc08] = cylGauge[0x1c];
        *(u32*)(base + 0xc0c) = *(u32*)(cylGauge + 0x20);
        base[0xc10] = cylGauge[0x24];
        base[0xc11] = cylGauge[0x25];
        *(f32*)(base + 0xc14) = *(f32*)(cylGauge + 0x28);
        __dt__16CMCCylinderGaugeFv(cylGauge, -1);
        init__16CMCCylinderGaugeFv(base + 0xbec);
        // Crystal support: same build/copy/destroy pattern.
        __ct__17CMCCrystalSupportFPQ34nw4r3lyt19ArcResourceAccessor(
            crySupport,
            *reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40));
        *(u32*)(base + 0xc74) = *(u32*)(crySupport + 4);
        *(u32*)(base + 0xc78) = *(u32*)(crySupport + 8);
        *(u32*)(base + 0xc7c) = *(u32*)(crySupport + 0xc);
        base[0xc80] = crySupport[0x10];
        *(u32*)(base + 0xc84) = *(u32*)(crySupport + 0x14);
        base[0xc88] = crySupport[0x18];
        base[0xc89] = crySupport[0x19];
        __dt__17CMCCrystalSupportFv(crySupport, -1);
        CrySupBuildLayout(base + 0xc70);
        // Effect objects: build on stack, install via MakeCrystalCopyParamBlock, destroy.
        __ct__11CMCEffStartFPQ34nw4r3lyt19ArcResourceAccessor(
            effStart,
            *reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40));
        MakeCrystalCopyParamBlock((CMakeCrystalCopyBlock*)(base + 0xc8c), (CMakeCrystalCopyBlock*)effStart);
        __dt__11CMCEffStartFv(effStart, -1);
        reinterpret_cast<CMCEffStart*>(base + 0xc8c)->init();
        __ct__11CMCEffUpRedFPQ34nw4r3lyt19ArcResourceAccessor(
            effUpRed,
            *reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40));
        MakeCrystalCopyParamBlock((CMakeCrystalCopyBlock*)(base + 0xca4), (CMakeCrystalCopyBlock*)effUpRed);
        __dt__11CMCEffUpRedFv(effUpRed, -1);
        reinterpret_cast<CMCEffUpRed*>(base + 0xca4)->init();
        __ct__12CMCEffUpBlueFPQ34nw4r3lyt19ArcResourceAccessor(
            effUpBlue,
            *reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40));
        MakeCrystalCopyParamBlock((CMakeCrystalCopyBlock*)(base + 0xcbc), (CMakeCrystalCopyBlock*)effUpBlue);
        __dt__12CMCEffUpBlueFv(effUpBlue, -1);
        reinterpret_cast<CMCEffUpBlue*>(base + 0xcbc)->init();
        __ct__13CMCEffUpGreenFPQ34nw4r3lyt19ArcResourceAccessor(
            effUpGreen,
            *reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40));
        MakeCrystalCopyParamBlock((CMakeCrystalCopyBlock*)(base + 0xcd4), (CMakeCrystalCopyBlock*)effUpGreen);
        __dt__13CMCEffUpGreenFv(effUpGreen, -1);
        reinterpret_cast<CMCEffUpGreen*>(base + 0xcd4)->init();
        // UpPrm: larger copy with an 8-byte-per-iteration counted loop
        // (same lwz/lwzu + stw/stwu pair idiom as the item-box tables).
        __ct__11CMCEffUpPrmFPQ34nw4r3lyt19ArcResourceAccessor(
            effUpPrm,
            *reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40));
        base[0xcf0] = effUpPrm[4];
        *(u32*)(base + 0xcf4) = *(u32*)(effUpPrm + 8);
        {
            u32 n = 0xc;
            u32* s = reinterpret_cast<u32*>(effUpPrm + 0xc);
            u32* d = reinterpret_cast<u32*>(base + 0xcf8);
            u32* ss = s - 1;
            u32* dd = d - 1;
            do {
                u32 a = *(ss + 1);
                u32 b = *(ss += 2);
                *(dd + 1) = a;
                *(dd += 2) = b;
            } while (--n);
        }
        base[0xd58] = effUpPrm[0x6c];
        __dt__11CMCEffUpPrmFv(effUpPrm, -1);
        init__11CMCEffUpPrmFv(base + 0xcec);
        __ct__13CMCEffSuccessFPQ34nw4r3lyt19ArcResourceAccessor(
            effSuccess,
            *reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40));
        base[0xd60] = effSuccess[4];
        base[0xd61] = effSuccess[5];
        *(u32*)(base + 0xd64) = *(u32*)(effSuccess + 8);
        *(u32*)(base + 0xd68) = *(u32*)(effSuccess + 0xc);
        *(u32*)(base + 0xd6c) = *(u32*)(effSuccess + 0x10);
        *(u32*)(base + 0xd70) = *(u32*)(effSuccess + 0x14);
        base[0xd74] = effSuccess[0x18];
        __dt__13CMCEffSuccessFv(effSuccess, -1);
        reinterpret_cast<CMCEffSuccess*>(base + 0xd5c)->init();
        __ct__13CMCEffFailureFPQ34nw4r3lyt19ArcResourceAccessor(
            effFailure,
            *reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40));
        MakeCrystalCopyParamBlock((CMakeCrystalCopyBlock*)(base + 0xd78), (CMakeCrystalCopyBlock*)effFailure);
        __dt__13CMCEffFailureFv(effFailure, -1);
        reinterpret_cast<CMCEffFailure*>(base + 0xd78)->init();
        __ct__13CMCEffCrystalFPQ34nw4r3lyt19ArcResourceAccessor(
            effCrystal,
            *reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40));
        base[0xd94] = effCrystal[4];
        base[0xd95] = effCrystal[5];
        *(u32*)(base + 0xd98) = *(u32*)(effCrystal + 8);
        *(u32*)(base + 0xd9c) = *(u32*)(effCrystal + 0xc);
        *(u32*)(base + 0xda0) = *(u32*)(effCrystal + 0x10);
        *(u32*)(base + 0xda4) = *(u32*)(effCrystal + 0x14);
        *(u32*)(base + 0xda8) = *(u32*)(effCrystal + 0x18);
        *(u32*)(base + 0xdac) = *(u32*)(effCrystal + 0x1c);
        *(u32*)(base + 0xdb0) = *(u32*)(effCrystal + 0x20);
        *(u32*)(base + 0xdb4) = *(u32*)(effCrystal + 0x24);
        *(u32*)(base + 0xdb8) = *(u32*)(effCrystal + 0x28);
        *(u32*)(base + 0xdbc) = *(u32*)(effCrystal + 0x2c);
        *(u32*)(base + 0xdc0) = *(u32*)(effCrystal + 0x30);
        base[0xdc4] = effCrystal[0x34];
        *(u16*)(base + 0xdc6) = *(u16*)(effCrystal + 0x36);
        base[0xdc8] = effCrystal[0x38];
        __dt__13CMCEffCrystalFv(effCrystal, -1);
        reinterpret_cast<CMCEffCrystal*>(base + 0xd90)->MCCrystal_BuildLayouts();
        __ct__12CMCEffUpRankFPQ34nw4r3lyt19ArcResourceAccessor(
            effUpRank,
            *reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40));
        MakeCrystalCopyParamBlock((CMakeCrystalCopyBlock*)(base + 0xdcc), (CMakeCrystalCopyBlock*)effUpRank);
        __dt__12CMCEffUpRankFv(effUpRank, -1);
        reinterpret_cast<CMCEffUpRank*>(base + 0xdcc)->init();
        __ct__12CMCEffDivideFPQ34nw4r3lyt19ArcResourceAccessor(
            effDivide,
            *reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40));
        MakeCrystalCopyParamBlock((CMakeCrystalCopyBlock*)(base + 0xde4), (CMakeCrystalCopyBlock*)effDivide);
        __dt__12CMCEffDivideFv(effDivide, -1);
        reinterpret_cast<CMCEffDivide*>(base + 0xde4)->init();
        __ct__14CMCEffCylinderFPQ34nw4r3lyt19ArcResourceAccessor(
            effCylinder,
            *reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40));
        base[0xe00] = effCylinder[4];
        base[0xe01] = effCylinder[5];
        *(u32*)(base + 0xe04) = *(u32*)(effCylinder + 8);
        *(u32*)(base + 0xe08) = *(u32*)(effCylinder + 0xc);
        *(u32*)(base + 0xe0c) = *(u32*)(effCylinder + 0x10);
        *(u32*)(base + 0xe10) = *(u32*)(effCylinder + 0x14);
        *(u32*)(base + 0xe14) = *(u32*)(effCylinder + 0x18);
        base[0xe18] = effCylinder[0x1c];
        __dt__14CMCEffCylinderFv(effCylinder, -1);
        reinterpret_cast<CMCEffCylinder*>(base + 0xdfc)->init();
        // Cursor objects: build on stack, install via MenuShopSellCopyCursor, destroy.
        __ct__6CCur18FPQ34nw4r3lyt19ArcResourceAccessor(&cur1, CUICfManager_getArcResourceAccessor());
        *reinterpret_cast<CBaseCur*>(base + 0xe20) = *reinterpret_cast<const CBaseCur*>(&cur1);
        __dt__6CCur18Fv(&cur1, -1);
        reinterpret_cast<CBaseCur*>(base + 0xe20)->initLayout();
        __ct__6CCur18FPQ34nw4r3lyt19ArcResourceAccessor(&cur2, CUICfManager_getArcResourceAccessor());
        *reinterpret_cast<CBaseCur*>(base + 0xeb4) = *reinterpret_cast<const CBaseCur*>(&cur2);
        __dt__6CCur18Fv(&cur2, -1);
        reinterpret_cast<CBaseCur*>(base + 0xeb4)->initLayout();
        *reinterpret_cast<void**>(base + 0x3c) = 0;
        validateHeap__17UnkClass_8045F564Fv(base + 0x2c);
        __dt__14Class_8045F858Fv(cls858, -1);
        return 1;
    }
    // Case 4: a per-slot crystal model file completed - clear the slot's
    // record and link the next handle. The event handle is re-read inside
    // the inner loop (retail keeps the reload live across iterations).
    u8 s = 0;
    do {
        u8* slotBase = base + (u32)s * 0x5cc + 0x44;
        u32 n = 6;
        u8 j = 0;
        do {
            u8* e = slotBase + (u32)j * 0xc;
            if (*(u32*)(e + 0x564) == reinterpret_cast<u32>(ev->mFileHandle)) {
                void* rec = reinterpret_cast<void*>(*(u32*)(e + 0x564));
                void* nxt = *reinterpret_cast<void**>(reinterpret_cast<u8*>(rec) + 4);
                *reinterpret_cast<void**>(reinterpret_cast<u8*>(rec) + 4) = 0;
                *(u32*)(e + 0x568) = reinterpret_cast<u32>(nxt);
                *(u32*)(e + 0x564) = 0;
                return 1;
            }
            j++;
        } while (--n);
        s++;
    } while (s < 2);
    return 0;
}

void MakeCrystal_UnlinkFileSlot(CModelDispMakeCrystal* self, u8 slotA, u8 slotB)
{
    u8* base = reinterpret_cast<u8*>(self);
    loadMCGetItemBoxFiles((CMCGetItemBox*)(base + 0xecc));
    void* m = Scn_FindCamItem(*reinterpret_cast<void**>(base + 0xc), -1);
    nw4r::math::VEC3 v1;
    writeVec3f(&v1, 0.0f, 1.0f, 0.0f);
    nw4r::math::VEC3 v2;
    writeVec3f(&v2, 0.0f, lbl_eu_80668504, lbl_eu_806684CC);
    ScnCamPointAt(m, &v2, &v1);
    for (u8 j = 0; j < 2; j++) {
        for (u8 i = 0; i < 6; i++) {
            closeFileHandle(reinterpret_cast<CFileHandle**>(
                base + (u32)j * 0x5cc + 0x44 + (u32)i * 0xc + 0x564));
        }
    }
    for (u8 k = 0; k < 2; k++) {
        MakeCrystal_ReleaseAnimObj(self, base + (u32)k * 0x5cc + 0x44);
    }
    base[0x5f0] = slotA;
    base[0xbbc] = slotB;
    base[0x60c] = 0;
    base[0xbd8] = 0;
    GetCollectedFlagByte(slotA);
    GetCollectedFlagByte(slotB);
    CtrlRemote_SetSharedBit(0xbd, 0);
    CtrlRemote_SetSharedBit(0xbf, 0);
}

void CModelDispMakeCrystal::setCrystalCursor(u32 val) { mEntries = reinterpret_cast<u8*>(val); }

int MakeCrystal_GetPromptState(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (reinterpret_cast<CSysWin*>(base + 0xe78)->isOpened()) return 0;
    u8 state = base[0xbdd];
    if ((u8)(state + 0xee) <= 1) return 0x38;
    if (state == 0x18 || state == 0x1f)
        return getMCGetItemBoxKeyCode((CMCGetItemBox*)(base + 0xecc));
    return (state < 0x12) ? 0x3b : 0;
}

// Anim-callback slot: buffered ids at +0x10/+0x14 (+0x08 is address-taken
// embedded data, still formed raw). Per-statement use only.
// Slot record: the detachable anim-wrapper subobject at +0x08 (forwarded to
// schedDetachChildSlot) and the registered anim ids at +0x10/+0x14.
struct MCCryAnimSlot { u8 _00[8]; void* anim08; u8 _0C[4]; u32 id10; u32 id14; };

// Release a registered anim callback by id: if the id matches the buffered
// object (+0x10 / +0x14), flush it with the anim wrapper (+0x8, or the object
// itself when null) and clear the slot.
extern "C" void MakeCrystal_ClearAnimSlot(void* obj, u32 id)
{
    u32 e1 = ((MCCryAnimSlot*)obj)->id10;
    if (id == e1) {
        void* arg = obj;
        if (obj) arg = &((MCCryAnimSlot*)obj)->anim08;
        schedDetachChildSlot(reinterpret_cast<void*>(e1), arg);
        ((MCCryAnimSlot*)obj)->id10 = 0;
    } else {
    u32 e2 = ((MCCryAnimSlot*)obj)->id14;
    if (id == e2) {
        void* arg = obj;
        if (obj) arg = &((MCCryAnimSlot*)obj)->anim08;
            schedDetachChildSlot(reinterpret_cast<void*>(e2), arg);
            ((MCCryAnimSlot*)obj)->id14 = 0;
        }
    }
}

// (initCrystalData is a retail hard-symbol; not defined here so it stays a call)

extern "C" void releaseCrystalDispThunk(CModelDispMakeCrystal* self) { ((void(*)(void*))MakeCrystal_ClearAnimSlot)((char*)self - 0x8); }

void sinit_80221DDC() {
    initCrystalData(sCrystalProbState);
    writeVec3f(sCrystalLeafVecA, lbl_eu_80668508, lbl_eu_806684AC, lbl_eu_8066850C);
    writeVec3f(sCrystalLeafVecB, lbl_eu_806684A4, scaleByGlobal(lbl_eu_80668510[0]), lbl_eu_806684A4);
}


// Retail vtable slot spellings (now unused by the emitted vtables).
extern "C" {

extern void OnFileEvent__21CModelDispMakeCrystalFP10CEventFile(int);
extern void WorkEvent3__10IWorkEventFPv();
extern void WorkEvent4__10IWorkEventFv();
extern void OnPauseTrigger__10IWorkEventFb();
extern void WorkEvent6__10IWorkEventFv();
extern void WorkEvent7__10IWorkEventFv();
extern void WorkEvent8__10IWorkEventFv();
extern void WorkEvent9__10IWorkEventFv();
extern void WorkEvent10__10IWorkEventFv();
extern void WorkEvent11__10IWorkEventFv();
extern void WorkEvent12__10IWorkEventFv();
extern void WorkEvent13__10IWorkEventFv();
extern void WorkEvent14__10IWorkEventFv();
extern void WorkEvent15__10IWorkEventFv();
extern void WorkEvent16__10IWorkEventFv();
extern void WorkEvent17__10IWorkEventFv();
extern void WorkEvent18__10IWorkEventFv();
extern void WorkEvent19__10IWorkEventFv();
extern void WorkEvent20__10IWorkEventFv();
extern void WorkEvent21__10IWorkEventFv();
extern void WorkEvent22__10IWorkEventFv();
extern void WorkEvent23__10IWorkEventFv();
extern void WorkEvent24__10IWorkEventFv();
extern void WorkEvent25__10IWorkEventFv();
extern void WorkEvent26__10IWorkEventFv();
extern void WorkEvent27__10IWorkEventFv();
extern void WorkEvent28__10IWorkEventFv();
extern void WorkEvent29__10IWorkEventFv();
extern void WorkEvent30__10IWorkEventFv();
extern void WorkEvent31__10IWorkEventFv();
extern "C" void releaseCrystalDispThunk(CModelDispMakeCrystal* self);
extern "C" void MakeCrystal_ClearAnimSlot(void*, u32);
} // extern "C"

// The three vtables, RTTI and base lists are emitted by MWCC from the real
// CModelDispMakeCrystal class and its base classes.
// zero hole at 0xDC that IS the retail gap_07_80535E6C_data word.


// --- restored from git history (base:gone repo-search) ---
// from commit 4116edb5dec8 path=src/kyoshin/makecrystal/CModelDispMakeCrystal.cpp needle=initCrystalData

void initCrystalData(unsigned char* p) {
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[5] = 0;
}

