// CTaskGame.hpp must come first: its line-10 `extern "C"` declaration of
// func_8004302C must precede the plain C++ declaration pulled in by
// CMenuArtsSelect.hpp (monolib/scn.hpp -> CScnNw4r.hpp -> functions.hpp);
// MWCC rejects the reverse order (10505 illegal overloading).
//
// CTaskGame.hpp:571 also declares lbl_eu_80663E24 non-volatile, which
// conflicts with the volatile declaration in CfObjectMove.hpp (reached via
// the CBattleManager.hpp include below). Rename that CTaskGame.hpp copy out
// of the way (same scheme as CMenuKizunaTalk.hpp's func_801361E8 rename).
// (func_8004392C now has a single unified decl.)
#define lbl_eu_80663E24 artsSelectCtaskGameE24Unused
// The battle-manager singleton getter has ONE canonical declaration in
// kyoshin/cf/CBattleManagerApi.hpp; no rename guards are needed anymore.
#include "kyoshin/CTaskGame.hpp"
#undef lbl_eu_80663E24

#include "kyoshin/menu/CMenuArtsSelect.hpp"

#include "monolib/device/CDeviceVI.hpp"
#include "monolib/core/CPadManager.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CTTask.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

// CUIBattleManager.hpp declares func_8012FD60/func_8012FC74 as C++-linkage
// friends (mangled), but the retail symbols at these call sites are unmangled
// (texture lookup helpers); rename the friend decls out of the way and declare
// C-linkage copies in the extern "C" block below.
#define func_8012FD60 artsSelectUiBmFD60Unused
#define func_8012FC74 artsSelectUiBmFC74Unused
#include "kyoshin/CUIBattleManager.hpp"
#undef func_8012FC74
#undef func_8012FD60
#include <nw4r/math.h>

// CVision.hpp's lbl_eu_80663E24 copy is volatile, which clashes with
// CfObjectMove.hpp's non-volatile extern (recently aligned with
// CSystemWindow.hpp); this TU reads E24 via the CfObjectMove copy, so rename
// CVision's copy out of the way. (CVision.hpp's func_80496288 copy is gone;
// the symbol now has a single unified decl.)
#define lbl_eu_80663E24 artsSelectVisionE24Unused
#include "kyoshin/cf/CVision.hpp"
#undef lbl_eu_80663E24
// CSuddenCommu.hpp's import signatures now match the chain / CfObjectActor
// headers (see CSuddenCommu.hpp); its battle-manager singleton copy is now
// unified in kyoshin/cf/CBattleManagerApi.hpp.
// CSuddenCommu.hpp:285 also declares func_8017FD44 with a void* arg; this TU
// uses the C-linkage no-arg version (unmangled retail symbol).
#define func_8017FD44 artsSelectSuddenCommuFd44Unused
#include "kyoshin/cf/CSuddenCommu.hpp"
#undef func_8017FD44
// CChain.hpp:737 declares func_80107C54 with (void*, int), conflicting with
// this TU's (CMenuArtsSelect*, s32) extern "C" import used by the matched
// func_80104454; rename the CChain.hpp copy out of the way. Its volatile
// lbl_eu_80663E24 also clashes with CfObjectMove.hpp's non-volatile extern.
#define func_80107C54 artsSelectChainSlotProbeUnused
// CChainActorList.hpp (via CChain.hpp) declares func_8017FD44(void*);
// this TU uses the C-linkage no-arg version.
#define func_8017FD44 artsSelectChainFd44Unused
#define lbl_eu_80663E24 artsSelectChainE24Unused
#include "kyoshin/cf/chain/CChain.hpp"
#undef lbl_eu_80663E24
#undef func_80107C54
#undef func_8017FD44
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
// code_80135FDC.hpp:168 declares lbl_eu_8066A208 as u32 (conflicting with
// CfObjectMove.hpp:97's const float), :172 declares lbl_eu_80663E24 as
// non-volatile u32 (conflicting with CfObjectMove.hpp:71's volatile u32).
// This TU uses none of these copies.
#define lbl_eu_8066A208 artsSelectCode35FDCepsilonUnused
#define lbl_eu_80663E24 artsSelectCode35FDCe24Unused
#include "kyoshin/code_80135FDC.hpp"
#undef lbl_eu_80663E24
#undef lbl_eu_8066A208

#include "decomp.h"
#include <revolution/GX.h>
#include <stdio.h>

// sdata2 conversion-magic doubles (2^52 family) are defined globally in
// port/data_defs.cpp (lbl_eu_80666F30 / lbl_eu_80666F38); declaring them here
// lets conversions reference the shared .sdata2 pool entries by retail name.
extern const double lbl_eu_80666F30;  // s16 -> f32 magic (2^52 + 2^31)
extern const double lbl_eu_80666F38;  // u16 -> f32 magic (2^52)


extern "C" {
// Batch 2026-07-14g: menu-arts-cbrender owns cbRenderBefore exclusively.
// Batch 2026-07-14j: menu-arts-move owns Move exclusively.
// Batch 2026-07-14k: menu-arts-ctor owns __ct__CMenuArtsSelect exclusively.

// Arts parameter info layout (case 10 in Move).
struct ArtsParamInfo {
    u8 _pad00[0x24];
    void* mNamePtr;  // +0x24: arts name string
    u16 mField28;    // +0x28: art id (func_801086D0 compares vs 0x16 / 0x53)
    u8 _pad2A[0x48 - 0x2A];
    u16 mField48;    // +0x48: gauge/action id (func_801086D0 compares vs actor+0x1530)
    u8 _pad4A[0x72 - 0x4A];
    u16 mField72;    // +0x72: per-slot stock/count (func_801080F8)
    u16 mCheckFlag;  // +0x74: non-zero when skill has gauge
    u8 _pad76[0x80 - 0x76];
    f32 mRatioNum;   // +0x80: current gauge value
    void* mTablePtr; // +0x84: pointer to function table
};

// Function table for arts param info (+0x14 = getMax vtable slot).
struct ArtsParamTable {
    u8 _pad00[0x14];
    void* mGetMaxFn; // +0x14: vtable entry for getMax()
};

// Battle actor container: CfObjectMove lives at offset 0x3e9c.
// The +0x04 field is a secondary MI vtable pointer.
struct BattleActor {
    void* mVtable0;          // +0x00: primary vtable
    void* mSecondaryVtable;  // +0x04: secondary MI vtable
    u8 mArtsList[0x1530 - 0x08]; // +0x08: arts container (func_80148778 operates on &+0x8)
    u32 mField1530;          // +0x1530
    u8 _pad1534[0x3388 - 0x1534];
    u16 mField3388;          // +0x3388
    u8 _pad338a[0x3e9c - 0x338a];
    u8 mMoveStart;           // +0x3e9c: CfObjectMove starts here
    u8 _pad3e9d[0x3f28 - 0x3e9d];
    u16 mField3F28;          // +0x3f28: same u16 as CfObjectMove+0x8C
    u8 _pad3f2a[0x3f60 - 0x3f2a];
    void* mField3F60;        // +0x3f60: sub-object pointer (move wrapper)
};

// BattleManager window (func_801086D0 reads s16@0x20C8, u8@0x1AA).
struct ArtsBmWindow {
    u8 _pad00[0x1AA];
    u8 mField1AA;            // +0x1AA: battle mode byte (1..24 = in battle)
    u8 _pad1AB[0x20C8 - 0x1AB];
    s16 mField20C8;          // +0x20C8: battle flag
};

// Sub-object at BattleActor::mField3F60 (+0x3f60); u16@+0x530 bit0.
struct ArtsSubObj {
    u8 _pad00[0x530];
    u16 mFlag530;            // +0x530 - bit0 gates arts-select availability
};

// Result of func_800B708C (battle action source); u32@+0x64 bit2.
struct ArtsActionSource {
    u8 _pad00[0x64];
    u32 mFlags;              // +0x64 - bit2 gates arts-select availability
};

// Fake SI interfaces: REAL virtuals (never defined/constructed) so MWCC emits
// the retail lwz r12 / lwz r12,off(r12) / mtctr / bcctrl dispatch chain
// (manual casts color the vtable base into a scratch reg instead). This TU
// reserves 2 vtable slots: vtable offset = (declared index + 2) * 4.
struct ArtsSubVtbl {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09();
    virtual void* mFn30();  // #10 => +0x30: party-status getter
};

struct ArtsMoveVtbl {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m0A(); virtual void m0B();
    virtual void m0C(); virtual void m0D(); virtual void m0E(); virtual void m0F();
    virtual void m10();
    virtual void* mFn4C();  // #17 => +0x4C: CfObjectMove status getter
    // fillers #18..#40
    virtual void m11(); virtual void m12(); virtual void m13(); virtual void m14();
    virtual void m15(); virtual void m16(); virtual void m17(); virtual void m18();
    virtual void m19(); virtual void m1A(); virtual void m1B(); virtual void m1C();
    virtual void m1D(); virtual void m1E(); virtual void m1F(); virtual void m20();
    virtual void m21(); virtual void m22(); virtual void m23(); virtual void m24();
    virtual void m25(); virtual void m26(); virtual void m27(); virtual void m28();
    virtual nw4r::math::VEC3* mFnAC();  // #41 => +0xAC: position getter
};

// Battle action source (func_800B708C result) vtable view.
struct ArtsActionSrcVtbl {
    // fillers #0..#40
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m0A(); virtual void m0B();
    virtual void m0C(); virtual void m0D(); virtual void m0E(); virtual void m0F();
    virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13();
    virtual void m14(); virtual void m15(); virtual void m16(); virtual void m17();
    virtual void m18(); virtual void m19(); virtual void m1A(); virtual void m1B();
    virtual void m1C(); virtual void m1D(); virtual void m1E(); virtual void m1F();
    virtual void m20(); virtual void m21(); virtual void m22(); virtual void m23();
    virtual void m24(); virtual void m25(); virtual void m26(); virtual void m27();
    virtual nw4r::math::VEC3* mFnAC();  // #41 => +0xAC: position getter
};

struct ArtsActorVtbl {
    // fillers #0..#71 (m00..m47)
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m0A(); virtual void m0B();
    virtual void m0C(); virtual void m0D(); virtual void m0E(); virtual void m0F();
    virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13();
    virtual void m14(); virtual void m15(); virtual void m16(); virtual void m17();
    virtual void m18(); virtual void m19(); virtual void m1A(); virtual void m1B();
    virtual void m1C(); virtual void m1D(); virtual void m1E(); virtual void m1F();
    virtual void m20(); virtual void m21(); virtual void m22(); virtual void m23();
    virtual void m24(); virtual void m25(); virtual void m26(); virtual void m27();
    virtual void m28(); virtual void m29(); virtual void m2A(); virtual void m2B();
    virtual void m2C(); virtual void m2D(); virtual void m2E(); virtual void m2F();
    virtual void m30(); virtual void m31(); virtual void m32(); virtual void m33();
    virtual void m34(); virtual void m35(); virtual void m36(); virtual void m37();
    virtual void m38(); virtual void m39(); virtual void m3A(); virtual void m3B();
    virtual void m3C(); virtual void m3D(); virtual void m3E(); virtual void m3F();
    virtual void m40(); virtual void m41(); virtual void m42(); virtual void m43();
    virtual void m44(); virtual void m45(); virtual void m46(); virtual void m47();
    virtual f32 mFn128();  // #72 => +0x128: party gauge
    // fillers #73..#155 (m48..m9A)
    virtual void m48(); virtual void m49(); virtual void m4A(); virtual void m4B();
    virtual void m4C(); virtual void m4D(); virtual void m4E(); virtual void m4F();
    virtual void m50(); virtual void m51(); virtual void m52(); virtual void m53();
    virtual void m54(); virtual f32 mFn160(); // #86 => +0x160: party arts gauge ratio
    virtual void m56(); virtual void m57();
    virtual void m58(); virtual void m59(); virtual void m5A(); virtual void m5B();
    virtual void m5C(); virtual void m5D(); virtual void m5E(); virtual void m5F();
    virtual void m60(); virtual void m61(); virtual void m62(); virtual void m63();
    virtual void m64(); virtual void m65(); virtual void m66(); virtual void m67();
    virtual void m68(); virtual void m69(); virtual void m6A(); virtual void m6B();
    virtual void m6C(); virtual void m6D(); virtual void m6E(); virtual void m6F();
    virtual void m70(); virtual void m71(); virtual void m72(); virtual void m73();
    virtual void m74(); virtual void m75(); virtual void m76(); virtual void m77();
    virtual void m78(); virtual void m79(); virtual void m7A(); virtual void m7B();
    virtual void m7C(); virtual void m7D(); virtual void m7E(); virtual void m7F();
    virtual void m80(); virtual void m81(); virtual void m82(); virtual void m83();
    virtual void m84(); virtual void m85(); virtual void m86(); virtual void m87();
    virtual void m88(); virtual void m89(); virtual void m8A(); virtual void m8B();
    virtual void m8C(); virtual void m8D(); virtual void m8E(); virtual void m8F();
    virtual void m90(); virtual void m91(); virtual void m92(); virtual void m93();
    virtual void m94(); virtual void m95(); virtual void m96(); virtual void m97();
    virtual void m98(); virtual void m99(); virtual void m9A();
    virtual void* mFn278();  // #156 => +0x278: arts list
};

struct ArtsTableVtbl {
    virtual void m00(); virtual void m01(); virtual void m02();
    virtual f32 mFn14();  // #3 => +0x14: getMax
};

// func_80105D54 support types.

// Opaque scene pose object (func_80496264 result; passed to func_8049B59C).
struct ArtsSelPose {
    u8 _pad00[0x10];
};

// 12-byte rodata id table copied by value into func_80105D54 locals.
struct ArtsSelEnumIds {
    u32 id[3];
};

// 8-byte CfObjEnumList holder (func_80043D90 ctor / __dt__80043E88 dtor).
struct ArtsEnumHolder {
    u8 _data[8];
};

// Enum-list handle returned by func_80043F18 (element count at +0x620).
struct ArtsEnumList {
    u8 _pad00[0x620];
    u32 mCount620;  // +0x620
};

// One enum-list slot (func_800F6EC0 result): +0x4 object, compared by +0x8C.
struct ArtsEnumSlot {
    u8 _pad00[4];
    CfObjectMoveArtsView* mObj;  // +0x04
};

// 5-byte mode table assembled on stack in func_80105D54 (word copy + byte).
struct ArtsSelModeTbl {
    u32 data;
    u8 extra;
};



// lbl_eu_80663E24 / lbl_eu_80663E28 are declared by CfObjectMove.hpp, which
// the CBattleManager.hpp include pulls in (C++ linkage, volatile for E24).
// Declaring them again inside this extern "C" block would conflict.
// Unmangled retail names; int (not u8) avoids clrlwi before cmpwi.
int func_8018A608();
int func_80122448();

u8 func_8013BEB8();

// func_801072E0 / func_80107580 imports (unmangled retail names)
u32 func_800A32BC(void*); // character-data category
u16 func_80139358(u32 value);
void* func_80157C4C(u32 index);
// CUIBattleManager.hpp only declares this as a friend; re-declare at file scope.
void* func_8012FD04(const char* name);

int func_8012FA5C();
void func_80138078__FUl(u32);
nw4r::lyt::ArcResourceAccessor* func_801355F4();
int func_80174C98(void* actor, int* outVal, int flags);
int func_8010EDD4(void*);
int func_8010A840(void*);

// func_8010433C arts-ref allocator: CfGameManager creates the ref object.
extern "C" void* func_8008187C__Q22cf13CfGameManagerFv(u32 index);
// func_801088CC player gate: converts a CfObjectMove to its actor container.
extern "C" BattleActor* func_800BFC68__FPQ22cf12CfObjectMove(cf::CfObjectMove* objMove);
extern "C" int func_801B2084();
extern "C" bool func_80060290(void*);


extern char lbl_eu_804FD1E0[];
extern u8 lbl_eu_804FD0D0[];

// func_80104454 (retail symbol is UNMANGLED; header member decl is a
// decompiler guess). Retail table: posX s16[9] @+0x00, posY @+0x14,
// scale f32[9] @+0x28, selTab s16[5] @+0x4C of lbl_eu_804FD0D0.
extern "C" int func_8029A658();
extern "C" int func_8010784C(CMenuArtsSelect*);
extern "C" int func_801086D0(CMenuArtsSelect*);
extern "C" int func_801088CC(CMenuArtsSelect*);
extern "C" void func_80104454(CMenuArtsSelect* self);
extern "C" void func_80107580(CMenuArtsSelect*);
extern "C" void func_801072E0(CMenuArtsSelect*);
extern "C" int func_80107970(CMenuArtsSelect*, s32);
extern "C" int func_80107C54(CMenuArtsSelect*, s32);
// Move calls these helpers through their UNMANGLED retail symbols (retail
// reloc targets are the C-linkage names, not the mangled member names).
extern "C" void func_80105A34(CMenuArtsSelect* self);
extern "C" void func_80105D54(CMenuArtsSelect* self);
extern "C" void func_80106450(CMenuArtsSelect* self);
extern "C" void func_801065E4(CMenuArtsSelect* self);
extern "C" void func_80106900(CMenuArtsSelect* self);
extern "C" void func_80106C30(CMenuArtsSelect*, s32);
extern "C" void func_80106EC8(CMenuArtsSelect*, s32);
extern "C" void func_801071B8(CMenuArtsSelect*, s32);
extern "C" void func_801080F8(CMenuArtsSelect*);
extern "C" int func_80154168(const u8*);
extern "C" int func_8015419C(u8*);
// func_80105D54 imports (unmangled retail names)
void func_8010ED38(void*);
void func_8010ED58(void*);
void func_8010A710(void*);
void func_8010A7A8(void*);
int func_8017FD44();
void* func_800B8B94(s32 id);                 // actor-container lookup by id
void* func_80496264(void* scn, int index);   // matches CfCam.hpp decl
void func_8049B59C(f32* out, void* pose, nw4r::math::VEC3* pos);
void func_80043D90(void* holder);
void* func_80043F18(void* holder);
void __dt__80043E88(void* holder, int flag);
void func_800F4A98(void* list, u32 type, u32 filter);
void* func_800F6EC0(void* list, u32 index);
extern const u32 lbl_eu_80666F54;  // mode-table word
extern const u8 lbl_eu_80666F58;   // mode-table extra byte
extern const ArtsSelEnumIds lbl_eu_804FD128;
// Unmangled imports used by the batch-14o targets (no header declares these in
// this TU's include closure; retail symbols are C-linkage names).
extern "C" FourShorts func_80139658(nw4r::lyt::Layout*, const char*, int);
extern "C" void func_801398A4(nw4r::lyt::Layout*, const char*, s16*, int);
extern "C" int CChain_isValidChain(u8* a1, u8* a2, int a3);
extern "C" int func_8027DF38(u8* self, u8* actor, int flag, int index);
extern "C" int func_802795D4(u8* self, int param);
extern "C" u8* func_8012FD60(const char* name);
extern "C" u8* func_8012FC74(const char* name);
extern "C" void func_80137F88(nw4r::lyt::Pane* pane, u8* tex);
extern s16 lbl_eu_804FD11C[];  // rodata selTab (s16[5]) - same table as lbl_eu_804FD0D0+0x4c
extern "C" int func_800DA06C(void*, void*);
extern "C" void func_8010EDDC(void*, u8);
extern "C" void func_8010ED18(void*);
extern "C" void func_8010A848(void*, u8);
extern "C" void func_8010A6F0(void*);
extern "C" int func_80187710();
extern "C" void func_80187718();
extern "C" int* func_8009ECB0();
extern "C" char lbl_eu_80661E08[8];  // sdata: empty-name fallback (fixed size keeps @sda21)
extern "C" u32 lbl_eu_80666F48;  // 4-byte talent table header
extern "C" u8 lbl_eu_80666F4C;   // 5th byte of talent table
// (lbl_eu_80666F50 is 100.0f -- declared below with the other floats)

// Init-only declarations
u32 getAllocHandle__10CLibLayoutFv();
void* allocate__Q23mtl10MemManagerFUlUl(u32, u32);
void __ct__CTagProcessor(void*);
u8* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);

void func_80139198(u32);
u32 func_801392C0();
extern const f32 lbl_eu_80666F28; // 0.0f
extern const f32 lbl_eu_80666F2C; // 1.0f
extern const f32 lbl_eu_80666F40; // -80.0f
extern const f32 lbl_eu_80666F44; // -1.0f
extern const f32 lbl_eu_80666F60; // talent-target distance limit A
extern const f32 lbl_eu_80666F64; // talent-target distance limit B
extern const f32 lbl_eu_80666F50; // 100.0f (talent-gauge gate)
extern const f32 lbl_eu_80666F5C; // frame-count limit (func_80106450 unk344 cap)

// Process + MI vtable / PTMF labels for ctor (retail __ct__CMenuArtsSelect).
// Extern refs to retail rodata/sdata symbols (NOT local definitions:
// tentative definitions here would emit duplicate data and skew reloc
// addends - see hexdiff reloc-drift notes).
extern char lbl_eu_8052C1C0[];
extern char lbl_eu_8052C084[];
// Retail .data label holding the null PTMF triple (same idiom as
// CMenuBattleDamage's matched ctor; referencing __ptmf_null directly drifts
// both the reloc name and the @ha/@l split).
extern u32 lbl_eu_80535AD0[3];
void __ct__8CProcessFv(CProcess*);
void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564*);
}

// Retail linker name is unlengthened `__ct__CMenuArtsSelect` (not __ct__15...).
// Statement order mirrors the FULL_MATCH CMenuBattleDamage ctor in this TU
// family: base ctor, interim vtable, final MI vtable + interface pieces, PTMF
// null copies, then the field clears and the 9-iteration array clear loop.
extern "C" CMenuArtsSelect* __ct__CMenuArtsSelect(CMenuArtsSelect* self, CScn* scn) {
    CMenuArtsSelect* p;
    char* vtFinal;
    char* vtWork;
    char* vtScn;
    char* vtInfo;
    u32 ptmfWord0;
    u32 ptmfWord1;
    u32 ptmfWord2;
    u32 z;

    p = self;
    __ct__8CProcessFv(reinterpret_cast<CProcess*>(p));
    // Interim CProcess vtable, then final MI vtable + interface pieces.
    p->vtable = reinterpret_cast<void*>(lbl_eu_8052C1C0);
    vtFinal = lbl_eu_8052C084;
    // Retail: lwzu of [0], then stw [1]@+0x40 before [0]@+0x3C.
    vtWork = vtFinal + 0x24;
    vtScn = vtFinal + 0xAC;
    ptmfWord0 = lbl_eu_80535AD0[0];
    z = 0;
    ptmfWord1 = lbl_eu_80535AD0[1];
    p->ptmfMove[1] = ptmfWord1;
    vtInfo = vtFinal + 0xBC;
    p->ptmfMove[0] = ptmfWord0;
    ptmfWord2 = lbl_eu_80535AD0[2];
    p->ptmfMove[2] = ptmfWord2;
    ptmfWord0 = lbl_eu_80535AD0[0];
    ptmfWord1 = lbl_eu_80535AD0[1];
    p->ptmfDraw[1] = ptmfWord1;
    p->ptmfDraw[0] = ptmfWord0;
    ptmfWord2 = lbl_eu_80535AD0[2];
    p->ptmfDraw[2] = ptmfWord2;
    p->unk54 = (u8)z;
    p->unk55 = (u8)z;
    p->vtable = reinterpret_cast<void*>(vtFinal);
    p->vtWorkEvent = reinterpret_cast<void*>(vtWork);
    p->vtScnRender = reinterpret_cast<void*>(vtScn);
    p->vtObjectInfo = reinterpret_cast<void*>(vtInfo);
    p->mScn = scn;
    p->unk68 = z;
    __ct__17UnkClass_8045F564Fv(&p->unk6C);

    // Loop-only locals declared at first assignment: MWCC colors callee-saved
    // registers by VR birth order, and the retail prologue only saves
    // r29-r31, so these must not be born at function entry.
    s32 mode9 = 9;
    f32 zeroF = lbl_eu_80666F28;
    s32 mode4 = 4;
    s8 neg1 = -1;
    s32 modeB = 0xb;
    s32 modeC = 0xc;
    s32 mode10 = 0x10;
    // Field clears must use z (r31). Setting i=0 before those stores makes
    // MWCC CSE the zeros onto the loop index (r8) and breaks ~20 stw encodings.
    p->unk7C = neg1;
    p->unk7D = neg1;
    p->unk7E = neg1;
    p->unk80 = (nw4r::lyt::Layout*)z;
    p->unk84 = (nw4r::lyt::AnimTransform*)z;
    p->unk88 = (nw4r::lyt::AnimTransform*)z;
    p->unk8C = (nw4r::lyt::Layout*)z;
    *reinterpret_cast<u32*>(&p->unk90[0]) = z;
    *reinterpret_cast<u32*>(&p->unk90[4]) = z;
    p->unk98 = (nw4r::lyt::Layout*)z;
    *reinterpret_cast<u32*>(&p->unk9C[0]) = z;
    p->unkA0 = (nw4r::lyt::AnimTransform*)z;
    p->unk288 = (u8)z;
    p->unk291 = (u8)z;
    p->unk294 = (nw4r::lyt::Layout*)z;
    p->unk298 = (s32)z;
    p->unk29C = mode9;
    p->unk324 = mode4;
    p->unk328 = (s32)z;
    p->unk32C = z;
    p->unk330 = z;
    p->unk334 = (u8)z;
    p->unk335 = (u8)z;
    p->unk336 = (u8)z;
    p->unk340 = (u16)z;
    p->unk344 = zeroF;
    p->unk348 = (u8)z;

    // for(left!=0;left--) -> mtctr/bdnz (MWCC_CASES 8c13).
    u32 i = 0;
    u8* base = reinterpret_cast<u8*>(p);
    for (u32 left = mode9; left != 0; left--) {
        if ((u8)i < 8) {
            *reinterpret_cast<u32*>(base + 0xa4 + ((u8)i << 2)) = z;
        }
        if ((u8)i < 8) {
            *reinterpret_cast<u32*>(base + 0xc4 + ((u8)i << 2)) = z;
        }
        if ((u8)i < 8) {
            *reinterpret_cast<u32*>(base + 0xe4 + ((u8)i << 2)) = z;
        }
        if ((u8)i < 8) {
            *reinterpret_cast<u32*>(base + 0x2a0 + ((u8)i << 2)) = (u32)modeB;
        }
        if ((u8)i < 8) {
            *(base + 0x289 + (u8)i) = (u8)z;
        }
        *reinterpret_cast<u32*>(base + 0x104 + ((u8)i << 2)) = z;
        *reinterpret_cast<u32*>(base + 0x128 + ((u8)i << 2)) = z;
        *reinterpret_cast<u32*>(base + 0x14c + ((u8)i << 2)) = z;
        *reinterpret_cast<u32*>(base + 0x2c0 + ((u8)i << 2)) = (u32)modeC;
        *reinterpret_cast<u32*>(base + 0x170 + ((u8)i << 2)) = z;
        *reinterpret_cast<u32*>(base + 0x194 + ((u8)i << 2)) = z;
        *reinterpret_cast<u32*>(base + 0x2e4 + ((u8)i << 2)) = (u32)mode10;
        *reinterpret_cast<u32*>(base + 0x1b8 + ((u8)i << 2)) = z;
        *reinterpret_cast<u32*>(base + 0x1dc + ((u8)i << 2)) = z;
        i++;
    }

    // Fresh clear=0 (r4). Trailing 0x7c..0x7e = -1 rewrite is scheduled
    // before the ptr clears by MWCC.
    {
        u32 clear = 0;
        p->unk200[0] = (UnkArtsSelectEntry*)clear;
        p->unk224[0] = (UnkArtsSelectEntry*)clear;
        p->unk248[0] = (UnkArtsSelectEntry*)clear;
        p->unk200[1] = (UnkArtsSelectEntry*)clear;
        p->unk224[1] = (UnkArtsSelectEntry*)clear;
        p->unk248[1] = (UnkArtsSelectEntry*)clear;
        p->unk200[2] = (UnkArtsSelectEntry*)clear;
        p->unk224[2] = (UnkArtsSelectEntry*)clear;
        p->unk248[2] = (UnkArtsSelectEntry*)clear;
        p->unk200[3] = (UnkArtsSelectEntry*)clear;
        p->unk224[3] = (UnkArtsSelectEntry*)clear;
        p->unk248[3] = (UnkArtsSelectEntry*)clear;
        p->unk200[4] = (UnkArtsSelectEntry*)clear;
        p->unk224[4] = (UnkArtsSelectEntry*)clear;
        p->unk248[4] = (UnkArtsSelectEntry*)clear;
        p->unk200[5] = (UnkArtsSelectEntry*)clear;
        p->unk224[5] = (UnkArtsSelectEntry*)clear;
        p->unk248[5] = (UnkArtsSelectEntry*)clear;
        p->unk200[6] = (UnkArtsSelectEntry*)clear;
        p->unk224[6] = (UnkArtsSelectEntry*)clear;
        p->unk248[6] = (UnkArtsSelectEntry*)clear;
        p->unk200[7] = (UnkArtsSelectEntry*)clear;
        p->unk224[7] = (UnkArtsSelectEntry*)clear;
        p->unk248[7] = (UnkArtsSelectEntry*)clear;
        p->unk200[8] = (UnkArtsSelectEntry*)clear;
        p->unk224[8] = (UnkArtsSelectEntry*)clear;
        p->unk248[8] = (UnkArtsSelectEntry*)clear;
        neg1 = -1;
        p->unk7C = neg1;
        p->unk7D = neg1;
        p->unk7E = neg1;
        lbl_eu_80663F24 = (UnkArtsSelectRef*)clear;
        p->unk31C = clear;
        p->unk320 = (u8)clear;
    }
    return p;
}

// ---------------------------------------------------------------------------
// Destructor (us-80102e44). Retail symbol __dt__15CMenuArtsSelectFv has the
// MSL "D2-inlined-into-D1" shape: the embedded UnkClass_8045F564 member dtor
// (flag -1), then the CProcess base dtor guarded by the double null-check,
// then a conditional operator delete when flags > 0. Written as a global
// function carrying the retail symbol name (CMapSel/CSysWinSelect idiom):
// MWCC's member-dtor codegen cannot express the double guard artifact.
// ---------------------------------------------------------------------------
extern "C" void __dt__17UnkClass_8045F564Fv(void* self, int flags);
extern "C" void __dt__8CProcessFv(void* self, int flags);

// ---------------------------------------------------------------------------
// __dt__80102300 (us-80102de8) -- standalone CTTask<IUIBattle> destructor.
// Null-guarded CProcess base teardown (flags forced to 0), then conditional
// operator delete when the caller passes flags > 0. The doubled null test is
// MWCC's nested-guard artifact (both branches reuse the same cr0 compare).
// ---------------------------------------------------------------------------
extern "C" CMenuArtsSelect* __dt__80102300(CMenuArtsSelect* _this, int flags) {
    if (_this != 0) {
        if (_this != 0) {
            __dt__8CProcessFv(_this, 0);
        }
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}
extern "C" CMenuArtsSelect* __dt__15CMenuArtsSelectFv(CMenuArtsSelect* _this,
                                                       int flags) {
    if (_this != 0) {
        __dt__17UnkClass_8045F564Fv(&_this->unk6C, -1);
        if (_this != 0) {
            if (_this != 0) {
                __dt__8CProcessFv(_this, 0);
            }
        }
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

// ---------------------------------------------------------------------------
// CTTask<IUIBattle> out-of-line specializations (retail standalone symbols).
// Move/Draw test the ptmf hook at +0x3C / +0x48 through __ptmf_test and
// dispatch via __ptmf_scall; ~CTTask emits the null check, CProcess base dtor
// call, and conditional operator delete.
// ---------------------------------------------------------------------------
template<>
void CTTask<IUIBattle>::Move() {
    if (mMoveFunc) {
        (reinterpret_cast<IUIBattle*>(this)->*mMoveFunc)();
    }
}

template<>
void CTTask<IUIBattle>::Draw() {
    if (mDrawFunc) {
        (reinterpret_cast<IUIBattle*>(this)->*mDrawFunc)();
    }
}

template<>
CTTask<IUIBattle>::~CTTask() {}

// Retail rematerializes the arc base per region rather than caching one copy;
// spell it inline via macro so MWCC sees each reference directly.
#define arc lbl_eu_804FD1E0
void CMenuArtsSelect::Init() {
    // Table pointers declared once: MWCC pins them in persistent registers
    // (retail r26/r29/r28) for the whole function.
    // Position/scale tables live for the whole function (retail pins them in
    // r26/r29/r28); every helper block derives its lookups from them.
    unk6C.createRegion(mtl::MemManager::getHandleMEM2(), 0x17a00, lbl_eu_804FD1E0, 0);
    Class_8045F858 regionGuard(&unk6C);

    mtl::MemManager::func_80434A4C(0);
    void* tagProc = allocate__Q23mtl10MemManagerFUlUl(0x858, getAllocHandle__10CLibLayoutFv());
    if (tagProc != NULL) {
        __ct__CTagProcessor(tagProc);
    }
    unk294 = static_cast<nw4r::lyt::Layout*>(tagProc);

    unk68 = reinterpret_cast<u32>(func_801355F4());

    func_80136E84(&unk80, reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(unk68), arc + 0x10);
    func_80136F08(unk80, &unk84, reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(unk68), arc + 0x27);
    func_80136F08(unk80, &unk88, reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(unk68), arc + 0x41);

    {
        nw4r::lyt::Pane* rootPane = unk80->GetRootPane();
        u8* fontObj = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, unk80);
        typedef u32 (*FontVFn)(void*);
        u32 lineSpace = (*reinterpret_cast<FontVFn**>(fontObj))[0x24 / 4](fontObj);
        func_8013676C(rootPane, lineSpace);
    }

    {
        nw4r::lyt::Pane* pane = unk80->GetRootPane()->FindPaneByName(arc + 0x5c, true);
        if (pane != NULL) {
            // TextBox::mpTagProcessor at offset 0xF8
            struct TextBoxLayout { u8 _[0xF8]; void* mpTagProcessor; };
            reinterpret_cast<TextBoxLayout*>(pane)->mpTagProcessor = unk294;
        }
    }

    func_80136B4C(unk80, arc + 0x69, NULL, 0);
    func_80136B4C(unk80, arc + 0x5c, NULL, reinterpret_cast<u32>(unk294));

    unk80->SetAnimationEnable(unk88, false);
    unk80->SetAnimationEnable(unk84, true);
    unk80->Animate(0);

    // Table pointers stay live from here to the last layout group (retail
    // pins them in r26/r29/r28).
    s16* posX = reinterpret_cast<s16*>(lbl_eu_804FD0D0);         // +0x00: s16[9]
    s16* posY = reinterpret_cast<s16*>(lbl_eu_804FD0D0 + 0x14);  // +0x14: s16[9]
    f32* scale = reinterpret_cast<f32*>(lbl_eu_804FD0D0 + 0x28); // +0x28: f32[9]

    func_80136E84(&unk8C, reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(unk68), arc + 0x74);
    func_80136F08(unk8C, &unk90, reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(unk68), arc + 0x8d);
    func_80136F08(unk8C, &unk94, reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(unk68), arc + 0xa9);

    {
        nw4r::lyt::Pane* pane = unk8C->GetRootPane()->FindPaneByName(arc + 0xc7, true);
        nw4r::math::VEC3 trans = pane->GetTranslate();
        trans.x = static_cast<f32>(posX[unk324]);
        trans.y = static_cast<f32>(posY[unk324]);
        pane->SetTranslate(trans);
        f32 s = scale[unk324];
        pane->SetScale(nw4r::math::VEC2(s, s));
    }
    unk8C->GetRootPane()->SetVisible(false);

    unk8C->SetAnimationEnable(unk90, false);
    unk8C->SetAnimationEnable(unk94, true);
    unk8C->Animate(0);

    func_80136E84(&unk98, reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(unk68), arc + 0xd2);
    func_80136F08(unk98, &unk9C, reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(unk68), arc + 0xf0);
    func_80136F08(unk98, &unkA0, reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(unk68), arc + 0x113);

    {
        nw4r::lyt::Pane* pane = unk98->GetRootPane()->FindPaneByName(arc + 0x139, true);
        nw4r::math::VEC3 trans = pane->GetTranslate();
        trans.x = static_cast<f32>(0);
        trans.y = static_cast<f32>(-146);
        pane->SetTranslate(trans);
        f32 s = scale[4];
        pane->SetScale(nw4r::math::VEC2(s, s));
    }

    unk98->SetAnimationEnable(unkA0, false);
    unk98->SetAnimationEnable(unk9C, true);
    unk9C->SetFrame(0.0f);
    unk98->Animate(0);

    // Per-slot layouts: retail re-finds the pane for each of
    // Get/SetTranslate and SetScale (three virtual FindPaneByName calls).
    for (s32 i = 0; i < 8; i++) {
        func_80136E84(&unkA4[i], reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(unk68), arc + 0xd2);
        func_80136F08(unkA4[i], &unkC4[i], reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(unk68), arc + 0xf0);
        func_80136F08(unkA4[i], &unkE4[i], reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(unk68), arc + 0x113);

        nw4r::math::VEC3 trans =
            unkA4[i]->GetRootPane()->FindPaneByName(arc + 0x139, true)->GetTranslate();
        s32 lookupIdx = (i < 4) ? i : (i + 1);
        trans.x = static_cast<f32>(posX[lookupIdx]);
        trans.y = static_cast<f32>(posY[lookupIdx]);
        unkA4[i]->GetRootPane()->FindPaneByName(arc + 0x139, true)->SetTranslate(trans);
        f32 s = scale[lookupIdx];
        unkA4[i]->GetRootPane()->FindPaneByName(arc + 0x139, true)->SetScale(nw4r::math::VEC2(s, s));

        unkA4[i]->SetAnimationEnable(unkE4[i], false);
        unkA4[i]->SetAnimationEnable(unkC4[i], true);

        f32 frame = static_cast<f32>(unkC4[i]->GetFrameSize()) - 1.0f;
        unkC4[i]->SetFrame(frame);

        unkA4[i]->Animate(0);

        unk337[i] = 1;
    }

    unk318 = 0;

    for (s32 i = 0; i < 9; i++) {
        func_80136E84(&unk104[i], reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(unk68), arc + 0x142);
        func_80136F08(unk104[i], &unk128[i], reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(unk68), arc + 0x160);
        func_80136F08(unk104[i], &unk14C[i], reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(unk68), arc + 0x181);

        nw4r::math::VEC3 trans =
            unk104[i]->GetRootPane()->FindPaneByName(arc + 0x1a3, true)->GetTranslate();
        s32 lookupIdx = (i == 8) ? 4 : ((i < 4) ? i : (i + 1));
        trans.x = static_cast<f32>(posX[lookupIdx]);
        trans.y = static_cast<f32>(posY[lookupIdx]);
        unk104[i]->GetRootPane()->FindPaneByName(arc + 0x1a3, true)->SetTranslate(trans);
        f32 s = scale[lookupIdx];
        unk104[i]->GetRootPane()->FindPaneByName(arc + 0x1a3, true)->SetScale(nw4r::math::VEC2(s, s));

        unk104[i]->SetAnimationEnable(unk14C[i], false);
        unk104[i]->SetAnimationEnable(unk128[i], true);
        unk128[i]->SetFrame(0.0f);
        unk104[i]->Animate(0);
    }

    unk310 = 0;

    for (s32 i = 0; i < 9; i++) {
        func_80136E84(&unk170[i], reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(unk68), arc + 0x1ac);
        func_80136F08(unk170[i], &unk194[i], reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(unk68), arc + 0x1cb);

        unk170[i]->SetAnimationEnable(unk194[i], true);
        unk194[i]->SetFrame(0.0f);
        unk170[i]->Animate(0);

        nw4r::math::VEC3 trans =
            unk170[i]->GetRootPane()->FindPaneByName(arc + 0x1ed, true)->GetTranslate();
        s32 lookupIdx = (i == 8) ? 4 : ((i < 4) ? i : (i + 1));
        trans.x = static_cast<f32>(posX[lookupIdx]);
        trans.y = static_cast<f32>(posY[lookupIdx]);
        unk170[i]->GetRootPane()->FindPaneByName(arc + 0x1ed, true)->SetTranslate(trans);
        f32 s = scale[lookupIdx];
        unk170[i]->GetRootPane()->FindPaneByName(arc + 0x1ed, true)->SetScale(nw4r::math::VEC2(s, s));
    }

    unk314 = 0;

    for (s32 i = 0; i < 9; i++) {
        func_80136E84(&unk1B8[i], reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(unk68), arc + 0x1f7);
        func_80136F08(unk1B8[i], &unk1DC[i], reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(unk68), arc + 0x217);

        nw4r::math::VEC3 trans =
            unk1B8[i]->GetRootPane()->FindPaneByName(arc + 0x23e, true)->GetTranslate();
        s32 lookupIdx = (i == 8) ? 4 : ((i < 4) ? i : (i + 1));
        trans.x = static_cast<f32>(posX[lookupIdx]);
        trans.y = static_cast<f32>(posY[lookupIdx]);
        unk1B8[i]->GetRootPane()->FindPaneByName(arc + 0x23e, true)->SetTranslate(trans);
        f32 s = scale[lookupIdx];
        unk1B8[i]->GetRootPane()->FindPaneByName(arc + 0x23e, true)->SetScale(nw4r::math::VEC2(s, s));

        unk1B8[i]->SetAnimationEnable(unk1DC[i], true);

        f32 frame = static_cast<f32>(unk1DC[i]->GetFrameSize()) - 1.0f;
        unk1DC[i]->SetFrame(frame);

        unk1B8[i]->Animate(0);
    }

    unk30C = 0;
    unk308 = 4;

    func_80108994();
    func_80139198(0);
    unk31C = code80135FDC_getByte_64077();
#undef arc


    {
        IScnRender* cb = reinterpret_cast<IScnRender*>(this);
        if (this != NULL) {
            cb = reinterpret_cast<IScnRender*>(&vtScnRender);
        }
        mScn->addRenderCB(cb, 0xa, 0);
    }

    unk6C.func_8045F810();
}

template <typename Fn>
static inline Fn artsVslot(void* obj, u32 offset) {
    return reinterpret_cast<Fn>((*reinterpret_cast<void***>(obj))[offset / 4]);
}

void CMenuArtsSelect::Term() {
    // Retail clears lbl_eu_80663F24 (not a mangled spInstance).
    if (lbl_eu_80663F24 != NULL) {
        lbl_eu_80663F24->unkB0 = NULL;
        lbl_eu_80663F24->unk68 |= 0x40u;
        lbl_eu_80663F24 = NULL;
    }

    CDeviceVI::waitForDrawDone();

    // MI adjust: IScnRender at +0x5c (null-this safe).
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        cb = reinterpret_cast<IScnRender*>(&this->vtScnRender);
    }
    mScn->removeRenderCB(cb);

    // Sliding base matches retail r30 += 4 over 9 iterations.
    u8* base = reinterpret_cast<u8*>(this);
    s32 i = 0;
    do {
        nw4r::lyt::Layout*& a = *reinterpret_cast<nw4r::lyt::Layout**>(base + 0x1b8);
        if (a != NULL) {
            delete a;
            a = NULL;
        }
        nw4r::lyt::Layout*& b = *reinterpret_cast<nw4r::lyt::Layout**>(base + 0x170);
        if (b != NULL) {
            delete b;
            b = NULL;
        }
        nw4r::lyt::Layout*& c = *reinterpret_cast<nw4r::lyt::Layout**>(base + 0x104);
        if (c != NULL) {
            delete c;
            c = NULL;
        }
        if (i < 8) {
            nw4r::lyt::Layout*& d = *reinterpret_cast<nw4r::lyt::Layout**>(base + 0xa4);
            if (d != NULL) {
                delete d;
                d = NULL;
            }
        }
        i++;
        base += 4;
    } while (i < 9);

    if (unk98 != NULL) {
        delete unk98;
        unk98 = NULL;
    }
    if (unk8C != NULL) {
        delete unk8C;
        unk8C = NULL;
    }
    if (unk80 != NULL) {
        delete unk80;
        unk80 = NULL;
    }
    if (unk294 != NULL) {
        delete unk294;
        unk294 = NULL;
    }

    unk6C.func_8045F778();

    lbl_eu_80663F20 = NULL;

    // Retail calls each getter twice when non-null.
    if (func_80110A70() != NULL) {
        func_8010EDE4(reinterpret_cast<u8*>(func_80110A70()) + 0x7e4);
    }
    if (func_8010CE48() != NULL) {
        func_8010A8E4(reinterpret_cast<u8*>(func_8010CE48()) + 0x7cc);
    }
    if (unk334 != 0) {
        func_80133770();
    }
}

void CMenuArtsSelect::Move() {
    // NV shift base kept live across the function; occupies the one
    // callee-saved slot below `this` so savegpr starts at r20 like retail.
    u32 one = 1;

    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0()) {
        goto done;
    }
    // Retail: rlwinm.; beq +8; b done. MWCC collapses if->goto to bne; keep beq
    // via fallthrough asm b (PLAN.md section 17.6). See MWCC_CASES 8c9.
    if ((lbl_eu_80663E28 & (1u << 21)) == 0) {
        goto after_bit21;
    }
    DECOMP_ASM_INSN_BEGIN
    asm {
        b done
    }
    DECOMP_ASM_INSN_END
after_bit21:
    if (!func_8013BE50()) {
        goto done;
    }

    unk7C = -1;
    unk7D = -1;
    unk7E = -1;

    if (lbl_eu_80663E24 & 0xAFA40000u) {
        goto done;
    }
    if (cf::CfGameManager::func_800829B8()) {
        goto done;
    }
    if (func_8018A608()) {
        goto done;
    }
    if (func_80122448()) {
        goto done;
    }
    if (getInstance__11CSysWinBuffFv() != NULL) {
        goto done;
    }
    if (func_80110A70() == NULL) {
        goto done;
    }
    // Same beq+8; b done shape as bit21 (PLAN.md section 17.6).
    if (func_8010CE48() != NULL) {
        goto after_ce48;
    }
    DECOMP_ASM_INSN_BEGIN
    asm {
        b done
    }
    DECOMP_ASM_INSN_END
after_ce48:

    ::func_801080F8(this);

    {
        u8 flag = func_8013BEB8();
        u8* b;
        b = &unk26C[0]->unkBB;
        *b = static_cast<u8>((*b & 0xFEu) | flag);
        b = &unk26C[1]->unkBB;
        *b = static_cast<u8>((*b & 0xFEu) | flag);
        b = &unk26C[2]->unkBB;
        *b = static_cast<u8>((*b & 0xFEu) | flag);
        b = &unk26C[3]->unkBB;
        *b = static_cast<u8>((*b & 0xFEu) | flag);
        b = &unk26C[4]->unkBB;
        *b = static_cast<u8>((*b & 0xFEu) | flag);
        b = &unk26C[5]->unkBB;
        *b = static_cast<u8>((*b & 0xFEu) | flag);
        b = &unk26C[6]->unkBB;
        *b = static_cast<u8>((*b & 0xFEu) | flag);
    }

    if (unk298 >= 2) {
        // Single-register idiom: retail does mr rN,r3 / subi rN,rN,0x3e9c.
        BattleActor* actor =
            reinterpret_cast<BattleActor*>(cf::CfGameManager::getPlayer(0));
        if (actor != NULL) {
            actor = (BattleActor*)((char*)actor - 0x3e9c);
        }
        if (actor != NULL) {
            typedef void* (*GetPtrFn)(void*);
            void* sub = actor->mSecondaryVtable;
            u32* pVal = reinterpret_cast<u32*>(artsVslot<GetPtrFn>(sub, 0x30)(sub));
            int localVal = pVal[0];
            if (func_80174C98(actor, &localVal, 0x803) != 0) {
                if (unk348 == 0) {
                    unk348 = 1;
                    unk328 = 4;
                    ::func_80107580(this);
                    if (unk324 == 4) {
                        ::func_801072E0(this);
                    }
                }
            } else {
                unk348 = 0;
            }
        }
    }

    switch (static_cast<s32>(unk298)) {
    case 0:
        unk308 |= 0x10u;
        if (func_8012FA5C() != 0) {
            ::func_80107580(this);
            func_80138078__FUl(0x42);
            unk298 = 1;
        }
        break;
    case 1:
        unk308 |= 0x10u;
        if (func_80137444(unk84, lbl_eu_80666F2C) != 0) {
            nw4r::lyt::Pane* rootPane8C = unk8C->GetRootPane();
            rootPane8C->SetVisible(true);
            char* nameStr =
                func_80136190(lbl_eu_804FD1E0 + 0x249, lbl_eu_804FD1E0 + 0x254, 1);
            char* helpStr =
                func_80136190(lbl_eu_804FD1E0 + 0x249, lbl_eu_804FD1E0 + 0x259, 1);
            func_80136B4C(unk80, lbl_eu_804FD1E0 + 0x69, nameStr, 0);
            func_80136B4C(unk80, lbl_eu_804FD1E0 + 0x5c, helpStr,
                          reinterpret_cast<u32>(unk294));
            unk298 = 2;
        }
        break;
    case 2:
        ::func_80104454(this);
        break;
    case 3:
        unk308 |= 0x90u;
        if (func_80137444(unk88, lbl_eu_80666F2C) != 0) {
            unk54 = 1;
        }
        if (unk308 & 0x2u) {
            // MWCC inlines GetRootPane / GetFrame / SetTranslate here.
            f32 frame = unk88->GetFrame();
            unk98->GetRootPane()->SetTranslate(nw4r::math::VEC3(
                lbl_eu_80666F28, lbl_eu_80666F40 * frame, lbl_eu_80666F28));
            unk308 |= 0x1u;
        }
        break;
    case 4:
        ::func_80105A34(this);
        break;
    case 5:
        if (func_80110A70() != NULL) {
            if (func_8010CE48() != NULL) {
                if (func_8010EDD4(reinterpret_cast<u8*>(func_80110A70()) + 0x7e4) !=
                    0) {
                    if (func_8010A840(reinterpret_cast<u8*>(func_8010CE48()) +
                                      0x7cc) != 0) {
                        unk298 = 6;
                    }
                }
            }
        }
        break;
    case 6:
        ::func_80105D54(this);
        break;
    case 7:
        if (func_80110A70() != NULL) {
            if (func_8010CE48() != NULL) {
                if (func_8010EDD4(reinterpret_cast<u8*>(func_80110A70()) + 0x7e4) !=
                    0) {
                    if (func_8010A840(reinterpret_cast<u8*>(func_8010CE48()) +
                                      0x7cc) != 0) {
                        unk8C->GetRootPane()->SetVisible(true);
                        unk298 = 2;
                    }
                }
            }
        }
        break;
    case 8:
        ::func_80106450(this);
        break;
    default:
        break;
    }

    if (unk298 != 0) {
        // Single-register idiom: retail does mr rN,r3 / subi rN,rN,0x3e9c.
        BattleActor* actor =
            reinterpret_cast<BattleActor*>(cf::CfGameManager::getPlayer(0));
        if (actor != NULL) {
            actor = (BattleActor*)((char*)actor - 0x3e9c);
        }
        if (actor != NULL) {
            if (unk298 >= 2 && !(unk308 & 0x80u)) {
                if (unk328 == 4) {
                    switch (static_cast<s32>(unk29C)) {
                    case 9:
                        ::func_801065E4(this);
                        break;
                    case 10:
                        unk308 |= 0x3u;
                        if (func_80137444(unkA0, lbl_eu_80666F2C) != 0) {
                            unk29C = 0xb;
                        }
                        break;
                    case 11:
                        ::func_80106900(this);
                        break;
                    default:
                        break;
                    }
                }

                f32 zeroF = lbl_eu_80666F28;
                for (s32 i = 0; i < 8; i++) {
                    switch (static_cast<s32>(unk2A0[i])) {
                    case 9:
                        ::func_80106C30(this, i);
                        break;
                    case 10: {
                        unk318 |= (one << i) | (one << (i + 9));
                        cf::CfGameManager::getPlayer(0);
                        void* skillSrc = func_8016FE34(NULL);
                        int ready = 0;
                        if (skillSrc != NULL) {
                            typedef u16* (*GetU16Fn)(void*);
                            u16* p =
                                artsVslot<GetU16Fn>(skillSrc, 0x27c)(skillSrc);
                            if (p[0] != 0) {
                                ready = 1;
                            }
                        }
                        typedef void* (*GetPtrFn)(void*);
                        void* skill =
                            artsVslot<GetPtrFn>(skillSrc, 0x278)(skillSrc);
                        ArtsParamInfo* infoRaw = reinterpret_cast<ArtsParamInfo*>(getArtsParamAtCnt(skill, i));
                        if (ready == 0) {
                            ArtsParamInfo* info = infoRaw;
                            if (info->mCheckFlag != 0) {
                                typedef f32 (*GetF32Fn)(void*);
                                ArtsParamTable* table = static_cast<ArtsParamTable*>(info->mTablePtr);
                                GetF32Fn getMax = reinterpret_cast<GetF32Fn>(table->mGetMaxFn);
                                f32 denom = getMax(infoRaw);
                                f32 ratio;
                                if (denom != zeroF) {
                                    denom = getMax(infoRaw);
                                    ratio = info->mRatioNum / denom;
                                } else {
                                    ratio = lbl_eu_80666F44;
                                }
                                if (ratio > zeroF) {
                                    ready = 1;
                                }
                            }
                        }
                        if (func_80137444(unkE4[i], lbl_eu_80666F2C) != 0 ||
                            ready != 0) {
                            unk2A0[i] = 0xb;
                            unkA4[i]->SetAnimationEnable(unkE4[i], false);
                            unkA4[i]->SetAnimationEnable(unkC4[i], true);
                        }
                        break;
                    }
                    case 11:
                        ::func_80106EC8(this, i);
                        break;
                    default:
                        break;
                    }
                }
            }

            {
                cf::CBattleManager* bm = cf::CBattleManager::getInstance();
                if (bm->mActorList1.size() == 0) {
                    CPad* pad = cf::CfGameManager::getCurrentPad();
                    u32 bit;
                    if (cf::CfGameManager::func_80086F9C(-1) != 0) {
                        bit = (pad->mPressedButtonFlags >> 23) & 1;
                    } else {
                        bit = (pad->mPressedButtonFlags >> 10) & 1;
                    }
                    if (bit != 0) {
                        typedef f32 (*GetF32Fn)(void*);
                        f32 v = artsVslot<GetF32Fn>(actor, 0x128)(actor);
                        if (v > lbl_eu_80666F28) {
                            unk334 = 1;
                        }
                    }
                }
            }
        }

        if (unk298 >= 2 && !(unk308 & 0x80u)) {
            for (s32 i = 0; i < 9; i++) {
                switch (static_cast<s32>(unk2C0[i])) {
                case 0xc: {
                    s32 nextIdx = (i >= 8) ? 0 : (i + 1);
                    u32 mask = (one << i) | (one << (i + 9));
                    unk310 &= ~mask;
                    if (unk200[nextIdx]->unkBB & 1) {
                        if (::func_80107C54(this, i) != 0) {
                            unk104[i]->SetAnimationEnable(unk14C[i], false);
                            unk104[i]->SetAnimationEnable(unk128[i], true);
                            unk128[i]->SetFrame(lbl_eu_80666F28);
                            unk2C0[i] = 0xd;
                            unk310 |= mask;
                        }
                    }
                    break;
                }
                case 0xd: {
                    s32 nextIdx = (i >= 8) ? 0 : (i + 1);
                    if (unk200[nextIdx]->unkBB & 1) {
                        unk310 |= (one << i) | (one << (i + 9));
                        if (func_80137444(unk128[i], lbl_eu_80666F2C) != 0) {
                            unk2C0[i] = 0xe;
                        }
                    } else {
                        unk310 &= ~((one << i) | (one << (i + 9)));
                        unk2C0[i] = 0xc;
                    }
                    break;
                }
                case 0xe:
                    ::func_801071B8(this, i);
                    break;
                case 0xf:
                    unk310 |= (one << i) | (one << (i + 9));
                    if (func_80137444(unk14C[i], lbl_eu_80666F2C) != 0) {
                        unk2C0[i] = 0xc;
                    }
                    break;
                default:
                    break;
                }

                switch (static_cast<s32>(unk2E4[i])) {
                case 0x10:
                    unk314 &= ~((one << i) | (one << (i + 9)));
                    if (::func_80107970(this, i) != 0) {
                        unk2E4[i] = 0x11;
                    }
                    break;
                case 0x11:
                    unk314 |= (one << i) | (one << (i + 9));
                    if (func_80137444(unk194[i], lbl_eu_80666F2C) != 0) {
                        unk2E4[i] = 0x12;
                    }
                    break;
                case 0x12:
                    unk314 = (unk314 | (one << i)) & ~(one << (i + 9));
                    if (::func_80107970(this, i) == 0) {
                        unk2E4[i] = 0x13;
                    }
                    break;
                case 0x13:
                    unk314 |= (one << i) | (one << (i + 9));
                    if (func_80137510(unk194[i],
                                      lbl_eu_80666F2C) != 0) {
                        unk2E4[i] = 0x10;
                    }
                    break;
                default:
                    break;
                }
            }
        }

        for (s32 i = 0; i < 9; i++) {
            if (func_80137444(unk1DC[i], lbl_eu_80666F2C) != 0) {
                unk30C = static_cast<u16>(unk30C & ~static_cast<u16>(one << i));
            } else {
                unk30C = static_cast<u16>(unk30C | static_cast<u16>(one << i));
            }
        }
    }

    if (unk308 & 0x30u) {
        unk80->Animate(0);
    }
    unk8C->Animate(0);
    if (unk308 & 0x1u) {
        unk98->Animate(0);
    }

    // Sliding this-base (+4 / iter) matches retail r20 += 4; also keeps a
    // second NV so this stays in r21 (savegpr_20).
    {
        u8* base = reinterpret_cast<u8*>(this);
        s32 i = 0;
        do {
            if (i < 8) {
                if (unk318 & (one << (i + 9))) {
                    (*reinterpret_cast<nw4r::lyt::Layout**>(base + 0xa4))
                        ->Animate(0);
                }
            }
            u32 bit9 = one << (i + 9);
            if (unk310 & bit9) {
                (*reinterpret_cast<nw4r::lyt::Layout**>(base + 0x104))
                    ->Animate(0);
            }
            if (unk314 & bit9) {
                (*reinterpret_cast<nw4r::lyt::Layout**>(base + 0x170))
                    ->Animate(0);
            }
            if (unk30C & (one << i)) {
                (*reinterpret_cast<nw4r::lyt::Layout**>(base + 0x1b8))
                    ->Animate(0);
            }
            i++;
            base += 4;
        } while (i < 9);
    }
done:
    ;
}

void CMenuArtsSelect::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0()) {
        goto done;
    }
    // Retail: rlwinm.; beq +8; b done. MWCC collapses if->goto to bne; keep beq
    // via fallthrough asm b (PLAN.md section 17.6). See MWCC_CASES 8c9.
    if ((lbl_eu_80663E28 & (1u << 21)) == 0) {
        goto after_bit21;
    }
    DECOMP_ASM_INSN_BEGIN
    asm {
        b done
    }
    DECOMP_ASM_INSN_END
after_bit21:
    if (!func_8013BE50()) {
        goto done;
    }
    if (lbl_eu_80663E24 & 0xAFA40000u) {
        goto done;
    }
    if (func_8018A608()) {
        goto done;
    }
    if (func_80122448()) {
        goto done;
    }

    {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        nw4r::lyt::DrawInfo drawInfo;
        func_80137250(&drawInfo);

        if (unk298 > 1 && !(unk308 & 0x80u)) {
            for (s32 i = 0; i < 9; i++) {
                if (unk30C & (1u << i)) {
                    func_80137038(unk1B8[i], &drawInfo, 0, 1);
                }
            }
        }

        {
            u32 flags = unk308;
            u32 notBit25 = !(flags & 0x40u);
            u32 merged = (flags & 0x30u) | notBit25;
            int visible = merged != 0;
            func_80137038(unk80, &drawInfo, 0, visible);
        }

        unk308 |= 0x40u;

        if (unk298 > 1) {
            if (unk328 == 4) {
                if (unk308 & 0x2u) {
                    u32 bit0 = unk308 & 0x1u;
                    u32 notBit28 = !(unk308 & 0x8u);
                    u32 merged = bit0 | notBit28;
                    int visible = merged != 0;
                    func_80137038(unk98, &drawInfo, 0, visible);
                    unk308 |= 0x8u;
                } else {
                    unk308 &= ~0x8u;
                }
            } else {
                unk308 &= ~0x8u;
            }

            if (!(unk308 & 0x80u)) {
                for (s32 i = 0; i < 8; i++) {
                    u32 v = unk318;
                    if (v & (1u << i)) {
                        u32 bit18 = 1u << (i + 18);
                        u32 notBit18 = !(v & bit18);
                        u32 merged = notBit18 | (v & (1u << (i + 9)));
                        int visible = merged != 0;
                        if (unk340 == 0) {
                            func_80137038(unkA4[i], &drawInfo, 0, visible);
                        }
                        unk318 |= bit18;
                    } else {
                        unk318 &= ~(1u << (i + 18));
                    }
                }

                for (s32 j = 0; j < 9; j++) {
                    s32 nextIdx = (j == 8) ? 0 : (j + 1);
                    if (unk200[nextIdx]->unkBB & 1) {
                        u32 v = unk310;
                        u32 bitI = 1u << j;
                        u32 bit18;
                        if (v & bitI) {
                            bit18 = 1u << (j + 18);
                            u32 notBit18 = !(v & bit18);
                            u32 merged = notBit18 | (v & (1u << (j + 9)));
                            int visible = merged != 0;
                            func_80137038(unk104[j], &drawInfo, 0, visible);
                            unk310 |= bit18;
                        } else {
                            bit18 = 1u << (j + 18);
                            unk310 &= ~bit18;
                        }

                        if (unk2C0[j] == 0xC) {
                            u32 w = unk314;
                            if (w & bitI) {
                                u32 notBit18 = !(w & bit18);
                                u32 merged = notBit18 | (w & (1u << (j + 9)));
                                int visible = merged != 0;
                                func_80137038(unk170[j], &drawInfo, 0, visible);
                                unk314 |= bit18;
                            } else {
                                unk314 &= ~bit18;
                            }
                        } else {
                            unk314 &= ~bit18;
                        }
                    } else {
                            unk310 &= ~(1u << (j + 18));
                            unk314 &= ~(1u << (j + 18));
                    }
                }
            }
        }

        func_80137038(unk8C, &drawInfo, 0, 1);
    }
done:
    ;
}

extern "C" void CMenuArtsSelect_clearArtsRef() { lbl_eu_80663F24 = 0; }
extern "C" void CMenuArtsSelect_releaseArtsRef(void* self, UnkArtsSelectRef* ref) { if (lbl_eu_80663F24 == ref) { ref->unkB0 = NULL; lbl_eu_80663F24 = 0; } }
extern "C" void CMenuArtsSelect_setDisabled() { if (lbl_eu_80663F20 != 0) lbl_eu_80663F20->unk336 = 1; }
extern "C" bool CMenuArtsSelect_isFinished() { return false; }
extern "C" CMenuArtsSelect* CMenuArtsSelect_getInstance() { return lbl_eu_80663F20; }
extern "C" bool CMenuArtsSelect_isCreated() { return lbl_eu_80663F20 != 0; }
// func_801042C8 (us-80104db0): arts-select availability gate.
// Returns 1 once the menu instance exists and its main FSM (unk298) has
// advanced past the pre-open states (< 5), else 0.
int func_801042C8() {
    if (lbl_eu_80663F20 != NULL && lbl_eu_80663F20->unk298 >= 5) {
        return 1;
    }
    return 0;
}
extern "C" int CMenuArtsSelect_isInteractable() { CMenuArtsSelect* menu = lbl_eu_80663F20; if (menu != 0) { int value = menu->unk298; if (value != 1 && value != 3) return 1; } return 0; }
extern "C" CMenuArtsSelect* CMenuArtsSelect_getSelectState() { if (lbl_eu_80663F20 == 0) return 0; return reinterpret_cast<CMenuArtsSelect*>(&lbl_eu_80663F20->unk7C); }
extern "C" int CMenuArtsSelect_isNotReady() { return lbl_eu_80663F20 == 0 ? 1 : lbl_eu_80663F24 == 0; }
void __dt__15CMenuArtsSelectFv(void*);
// IWorkEvent dtor this-adjusting thunk (retail: subi r3,-0x58; b __dt__)
extern "C" void func_80108C30(void* self){
    __dt__15CMenuArtsSelectFv((char*)self - 0x58);
}
extern "C" void CMenuArtsSelect_workEventDraw(void* self) {
    ((CMenuArtsSelect*)((char*)self - 0x5c))->cbRenderBefore();
}
extern "C" void CMenuArtsSelect_workEventDtor(void* p) {
    extern void __dt__15CMenuArtsSelectFv(void*);
    __dt__15CMenuArtsSelectFv((u8*)p - 0x5c);
}
extern "C" void CMenuArtsSelect_scnRenderRelease(void* ptr) { ((void (*)(char*))CMenuArtsSelect_releaseArtsRef)((char*)ptr - 0x60); }
extern "C" void CMenuArtsSelect_scnRenderDtor(CMenuArtsSelect* self) {
    extern void __dt__15CMenuArtsSelectFv(void*);
    __dt__15CMenuArtsSelectFv((CMenuArtsSelect*)((char*)self - 0x60));
}

// ---------------------------------------------------------------------------
// func_80104210 (us-80104cf8) -- lazy factory: create + register the menu.
// Returns NULL if the singleton already exists. Regist(NULL, parent, false)
// is called even when allocation fails (retail has no guard).
// ---------------------------------------------------------------------------
extern "C" CMenuArtsSelect* func_80104210(CProcess* parent, CScn* scn) {
    if (lbl_eu_80663F20 != NULL) {
        return NULL;
    }
    CMenuArtsSelect* obj = (CMenuArtsSelect*)mtl::MemManager::allocate(
        0x34c, CWorkThreadSystem::getWorkMem());
    if (obj != NULL) {
        obj = __ct__CMenuArtsSelect(obj, scn);
    }
    lbl_eu_80663F20 = obj;
    reinterpret_cast<CProcess*>(obj)->Regist(parent, false);
    return lbl_eu_80663F20;
}

// ---------------------------------------------------------------------------
// func_8010433C (us-80104e24) -- create the arts-select ref object.
// Only runs while the menu exists but the ref does not; the ref's +0xb0 is
// bound to the menu's IObjectInfo slot (+0x60), NULL-safe.
// ---------------------------------------------------------------------------
extern "C" void func_8010433C() {
    if (lbl_eu_80663F20 != NULL && lbl_eu_80663F24 == NULL) {
        UnkArtsSelectRef* ref =
            (UnkArtsSelectRef*)func_8008187C__Q22cf13CfGameManagerFv(0xb5);
        lbl_eu_80663F24 = ref;
        if (ref != NULL) {
            // Local first: retail re-reads the global after the call and uses
            // the loaded pointer itself as the NULL case (no separate li 0).
            void* p = lbl_eu_80663F20;
            if (p != NULL) {
                p = reinterpret_cast<void*>(reinterpret_cast<char*>(p) + 0x60);
            }
            ref->unkB0 = p;
        }
    }
}

// ---------------------------------------------------------------------------
// func_801043BC (us-80104ea4) -- reset all art-slot anim frames to 0.0f.
// Each slot whose availability probe (func_80107970 / func_80107C54) passes
// gets its unk1DC anim rewound; then a 0x9e SE plays.
// ---------------------------------------------------------------------------
extern "C" void func_801043BC() {
    if (lbl_eu_80663F20 == NULL) return;
    f32 zero = lbl_eu_80666F28;
    for (u32 i = 0; i < 9; i++) {
        if (func_80107970(lbl_eu_80663F20, (u8)i) != 0) continue;
        if (func_80107C54(lbl_eu_80663F20, (u8)i) != 0) continue;
        lbl_eu_80663F20->unk1DC[(u8)i]->SetFrame(zero);
    }
    func_80138078__FUl(0x9e);
}

// ---------------------------------------------------------------------------
// func_80104454 (us-80104f3c) -- arts-select main per-frame update.
// Called from Move() case 2 (unk298 == 2). Retail symbol is the UNMANGLED
// free function; the header's member declaration is kept for Move's call.
// ---------------------------------------------------------------------------
extern "C" void func_80104454(CMenuArtsSelect* self) {
    typedef void* (*GetPtrFn)(void*);
    typedef f32 (*GetF32Fn)(void*);
    typedef void* (*AtFn)(void*, s32);
    u8* tbl = lbl_eu_804FD0D0;  // retail hoists this to r30 in the prologue

    self->unk308 &= ~0x10u;

    if (func_8029A658() != 0) return;

    if (func_8010784C(self) != 0) {
        self->unk80->SetAnimationEnable(self->unk84, false);
        self->unk80->SetAnimationEnable(self->unk88, true);
        self->unk8C->GetRootPane()->SetVisible(false);
        func_80136B4C(self->unk80, lbl_eu_804FD1E0 + 0x69, NULL, 0);
        func_80136B4C(self->unk80, lbl_eu_804FD1E0 + 0x5c, NULL,
                      reinterpret_cast<u32>(self->unk294));
        func_80138078__FUl(67);
        self->unk298 = 3;
    }

    // battleCheck copy 1: (s16)+0x20C8 != 0, or (u8)+0x1AA in [1,24], or
    // player-0 gauge <= 0.0f. Retail has three full copies (do not factor).
    bool battle = false;
    {
        cf::CBattleManager* bm = cf::CBattleManager::getInstance();
        if (*(s16*)((char*)bm + 0x20C8) != 0) {
            battle = true;
        } else {
            u32 bmv = *(u8*)((char*)cf::CBattleManager::getInstance() + 0x1AA);
            bool b2 = false;
            if (bmv >= 1) {
                if (bmv <= 24) b2 = true;
            }
            if (b2) {
                battle = true;
            } else {
                void* pl = cf::CfGameManager::getPlayer(0);
                void* adj = pl;
                if (pl != NULL) adj = (char*)pl - 0x3e9c;
                if (adj != NULL) {
                    f32 g = artsVslot<GetF32Fn>(adj, 0x128)(adj);
                    if (g <= lbl_eu_80666F28) {
                        battle = true;
                    } else {
                        battle = false;
                    }
                } else {
                    battle = false;
                }
            }
        }
    }
    if (battle) {
        self->unk328 = 4;
        func_80107580(self);
        if (self->unk324 == 4) func_801072E0(self);
    }

    // unk335 latch -- talent art (id 0xEA) presence, duplicated per retail.
    if (self->unk335 == 0) {
        bool found = false;
        void* pl = cf::CfGameManager::getPlayer(0);
        void* adj = pl;
        if (pl != NULL) adj = (char*)pl - 0x3e9c;
        if (*(u16*)((char*)adj + 0x3F28) == 1) {
            for (s32 i = 0; i < 104; i++) {
                void* el = artsVslot<AtFn>((char*)adj + 8, 0x54)((char*)adj + 8, i);
                if (*(u16*)((char*)el + 0xC) == 0xEA) {
                    found = true;
                    break;
                }
            }
        }
        if (found) self->unk335 = 1;
    } else {
        bool found = false;
        void* pl = cf::CfGameManager::getPlayer(0);
        void* adj = pl;
        if (pl != NULL) adj = (char*)pl - 0x3e9c;
        if (*(u16*)((char*)adj + 0x3F28) == 1) {
            for (s32 i = 0; i < 104; i++) {
                void* el = artsVslot<AtFn>((char*)adj + 8, 0x54)((char*)adj + 8, i);
                if (*(u16*)((char*)el + 0xC) == 0xEA) {
                    found = true;
                    break;
                }
            }
        }
        if (!found) self->unk335 = 0;
    }

    cf::CfObjectMove* pl0 = cf::CfGameManager::getPlayer(0);
    BattleActor* actor = reinterpret_cast<BattleActor*>(pl0);
    if (pl0 != NULL) actor = (BattleActor*)((char*)pl0 - 0x3e9c);
    if (actor == NULL) return;

    if (self->unk320 != 0) {
        void* sub = actor->mSecondaryVtable;
        int v = *static_cast<int*>(artsVslot<GetPtrFn>(sub, 0x30)(sub));
        if (func_80174C98(actor, &v, 31) == 0) self->unk320 = 0;
    }

    cf::CfObjectMove* pl1 = cf::CfGameManager::getPlayer(0);
    BattleActor* a2 = reinterpret_cast<BattleActor*>(pl1);
    if (pl1 != NULL) a2 = (BattleActor*)((char*)pl1 - 0x3e9c);
    if (a2 != NULL && self->unk320 == 0) {
        void* sub = a2->mSecondaryVtable;
        int v = *static_cast<int*>(artsVslot<GetPtrFn>(sub, 0x30)(sub));
        if (func_80174C98(a2, &v, 0x803) != 0) {
            if (self->unk324 == 4 && self->unk328 == 0) {
                self->unk328 = 4;
                func_80107580(self);
                func_801072E0(self);
            } else if (self->unk328 == 2) {
                self->unk328 = 4;
                func_80107580(self);
                func_801072E0(self);
            }
        }
    }

    CPad* pad = cf::CfGameManager::getCurrentPad();
    bool b1;
    bool b2;
    bool b31;
    bool b0;
    bool bX;
    if (cf::CfGameManager::func_80086F9C(-1) != 0) {
        u32 held = pad->mHeldButtonFlags;
        if ((held & (1u << 27)) != 0) return;
        if ((held & (1u << 28)) != 0) return;
        if ((held & (1u << 25)) != 0) return;
        if ((held & (1u << 26)) == 0) return;
        // Retail rlwinm SH: page1=bit2 (SH30), page2=bit3 (SH29),
        // prev=bit0 (SH0), next=bit1 (SH31), use=bit21 (SH11).
        b1 = (pad->mTurboPressButtonFlags >> 2) & 1;
        b2 = (pad->mTurboPressButtonFlags >> 3) & 1;
        b31 = pad->mTurboPressButtonFlags & 1;
        b0 = (pad->mTurboPressButtonFlags >> 1) & 1;
        bX = (pad->mPressedButtonFlags >> 21) & 1;
        if ((pad->mHeldButtonFlags & (1u << 22)) == 0) bX = 0;
    } else {
        u32 held = pad->mHeldButtonFlags;
        if ((held & (1u << 11)) != 0) return;
        if ((held & (1u << 12)) == 0) return;
        b1 = (pad->mTurboPressButtonFlags >> 2) & 1;
        b2 = (pad->mTurboPressButtonFlags >> 3) & 1;
        b31 = pad->mTurboPressButtonFlags & 1;
        b0 = (pad->mTurboPressButtonFlags >> 1) & 1;
        bX = (pad->mPressedButtonFlags >> 4) & 1;
        if ((pad->mHeldButtonFlags & (1u << 5)) == 0) bX = 0;
    }

    if (b1) {
        // battleCheck copy 2
        bool battle2 = false;
        {
            cf::CBattleManager* bm = cf::CBattleManager::getInstance();
            if (*(s16*)((char*)bm + 0x20C8) != 0) {
                battle2 = true;
            } else {
                u32 bmv = *(u8*)((char*)cf::CBattleManager::getInstance() + 0x1AA);
                bool b22 = false;
                if (bmv >= 1) {
                    if (bmv <= 24) b22 = true;
                    }
                if (b22) {
                    battle2 = true;
                } else {
                    void* pl = cf::CfGameManager::getPlayer(0);
                    void* adj = pl;
                    if (pl != NULL) adj = (char*)pl - 0x3e9c;
                    if (adj != NULL) {
                        f32 g = artsVslot<GetF32Fn>(adj, 0x128)(adj);
                        if (g <= lbl_eu_80666F28) {
                            battle2 = true;
                        } else {
                            battle2 = false;
                        }
                    } else {
                        battle2 = false;
                    }
                }
            }
        }
        if (!battle2) {
            if (self->unk324 == 4) {
                void* sub = actor->mSecondaryVtable;
                int v11 = *static_cast<int*>(artsVslot<GetPtrFn>(sub, 0x30)(sub));
                if (func_80174C98(actor, &v11, 11) != 0) {
                    func_80138078__FUl(5);
                    goto end_body;
                }
                if (func_800DA06C(cf::CBattleManager::getInstance(), actor) == 0) {
                    void* sub2 = actor->mSecondaryVtable;
                    int v18 = *static_cast<int*>(artsVslot<GetPtrFn>(sub2, 0x30)(sub2));
                    if (func_80174C98(actor, &v18, 18) != 0) {
                        func_80138078__FUl(5);
                        goto end_body;
                    }
                }
                switch (self->unk328) {
                case 0:
                    self->unk328 = 4;
                    break;
                case 1:
                    self->unk328 = 4;
                    break;
                case 2:
                    self->unk328 = 0;
                    break;
                case 3:
                    self->unk328 = 1;
                    break;
                case 4: {
                    self->unk328 = 2;
                    void* sub4 = actor->mSecondaryVtable;
                    int v29 = *static_cast<int*>(artsVslot<GetPtrFn>(sub4, 0x30)(sub4));
                    if (func_800DA06C(cf::CBattleManager::getInstance(), actor) != 0 ||
                        func_80174C98(actor, &v29, 29) != 0) {
                        if (self->unk320 == 0) self->unk328 = 3;
                    }
                    break;
                }
                default:
                    break;
                }
                func_80107580(self);
                if (self->unk328 == 4) {
                    func_801072E0(self);
                } else {
                s16* selTab = reinterpret_cast<s16*>(tbl + 0x4C);
                    s16 v = reinterpret_cast<s16*>(tbl + 0x4C)[self->unk328];
                    char* nameStr = func_80136190(lbl_eu_804FD1E0 + 0x249,
                                                  lbl_eu_804FD1E0 + 0x254, v);
                    char* helpStr = func_80136190(lbl_eu_804FD1E0 + 0x249,
                                                  lbl_eu_804FD1E0 + 0x259, v);
                    func_80136B4C(self->unk80, lbl_eu_804FD1E0 + 0x69, nameStr, 0);
                    func_80136B4C(self->unk80, lbl_eu_804FD1E0 + 0x5c, helpStr,
                                  reinterpret_cast<u32>(self->unk294));
                }
                func_80138078__FUl(85);
                goto end_body;
            }
            // adopt slot 4
            self->unk324 = 4;
            nw4r::lyt::Pane* pane =
                self->unk8C->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0xC7, true);
            {
            s16* posX = reinterpret_cast<s16*>(tbl + 0x00);
            s16* posY = reinterpret_cast<s16*>(tbl + 0x14);
            f32* scale = reinterpret_cast<f32*>(tbl + 0x28);
                nw4r::math::VEC3 trans = pane->GetTranslate();
                trans.x = static_cast<f32>(posX[self->unk324]);
                trans.y = static_cast<f32>(posY[self->unk324]);
                pane->SetTranslate(trans);
                f32 s = scale[self->unk324];
                pane->SetScale(nw4r::math::VEC2(s, s));
            }
            if (self->unk328 == 4) {
                func_801072E0(self);
            } else {
                s16* selTab = reinterpret_cast<s16*>(tbl + 0x4C);
                s16 v = reinterpret_cast<s16*>(tbl + 0x4C)[self->unk328];
                char* nameStr = func_80136190(lbl_eu_804FD1E0 + 0x249,
                                              lbl_eu_804FD1E0 + 0x254, v);
                char* helpStr = func_80136190(lbl_eu_804FD1E0 + 0x249,
                                              lbl_eu_804FD1E0 + 0x259, v);
                func_80136B4C(self->unk80, lbl_eu_804FD1E0 + 0x69, nameStr, 0);
                func_80136B4C(self->unk80, lbl_eu_804FD1E0 + 0x5c, helpStr,
                              reinterpret_cast<u32>(self->unk294));
            }
            self->unk8C->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0x25E, true)
                ->SetVisible(true);
            self->unk8C->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0x26C, true)
                ->SetVisible(true);
            goto end_body;
        }
    }
    if (b2) {
        // battleCheck copy 3
        bool battle3 = false;
        {
            cf::CBattleManager* bm = cf::CBattleManager::getInstance();
            if (*(s16*)((char*)bm + 0x20C8) != 0) {
                battle3 = true;
            } else {
                u32 bmv = *(u8*)((char*)cf::CBattleManager::getInstance() + 0x1AA);
                bool b23 = false;
                if (bmv >= 1) {
                    if (bmv <= 24) b23 = true;
                    }
                if (b23) {
                    battle3 = true;
                } else {
                    void* pl = cf::CfGameManager::getPlayer(0);
                    void* adj = pl;
                    if (pl != NULL) adj = (char*)pl - 0x3e9c;
                    if (adj != NULL) {
                        f32 g = artsVslot<GetF32Fn>(adj, 0x128)(adj);
                        if (g <= lbl_eu_80666F28) {
                            battle3 = true;
                        } else {
                            battle3 = false;
                        }
                    } else {
                        battle3 = false;
                    }
                }
            }
        }
        if (!battle3) {
            if (self->unk324 == 4) {
                void* sub = actor->mSecondaryVtable;
                int v11 = *static_cast<int*>(artsVslot<GetPtrFn>(sub, 0x30)(sub));
                if (func_80174C98(actor, &v11, 11) != 0) {
                    func_80138078__FUl(5);
                    goto end_body;
                }
                if (func_800DA06C(cf::CBattleManager::getInstance(), actor) == 0) {
                    void* sub2 = actor->mSecondaryVtable;
                    int v18 = *static_cast<int*>(artsVslot<GetPtrFn>(sub2, 0x30)(sub2));
                    if (func_80174C98(actor, &v18, 18) != 0) {
                        func_80138078__FUl(5);
                        goto end_body;
                    }
                }
                switch (self->unk328) {
                case 0:
                    self->unk328 = 2;
                    break;
                case 1:
                    self->unk328 = 3;
                    break;
                case 2:
                    self->unk328 = 4;
                    break;
                case 3:
                    self->unk328 = 4;
                    break;
                case 4: {
                    self->unk328 = 0;
                    void* sub4 = actor->mSecondaryVtable;
                    int v29 = *static_cast<int*>(artsVslot<GetPtrFn>(sub4, 0x30)(sub4));
                    if (func_800DA06C(cf::CBattleManager::getInstance(), actor) != 0 ||
                        func_80174C98(actor, &v29, 29) != 0) {
                        if (self->unk320 == 0) self->unk328 = 1;
                    }
                    break;
                }
                default:
                    break;
                }
                func_80107580(self);
                if (self->unk328 == 4) {
                    func_801072E0(self);
                } else {
                s16* selTab = reinterpret_cast<s16*>(tbl + 0x4C);
                    s16 v = reinterpret_cast<s16*>(tbl + 0x4C)[self->unk328];
                    char* nameStr = func_80136190(lbl_eu_804FD1E0 + 0x249,
                                                  lbl_eu_804FD1E0 + 0x254, v);
                    char* helpStr = func_80136190(lbl_eu_804FD1E0 + 0x249,
                                                  lbl_eu_804FD1E0 + 0x259, v);
                    func_80136B4C(self->unk80, lbl_eu_804FD1E0 + 0x69, nameStr, 0);
                    func_80136B4C(self->unk80, lbl_eu_804FD1E0 + 0x5c, helpStr,
                                  reinterpret_cast<u32>(self->unk294));
                }
                func_80138078__FUl(85);
                goto end_body;
            }
            self->unk324 = 4;
            nw4r::lyt::Pane* pane =
                self->unk8C->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0xC7, true);
            {
            s16* posX = reinterpret_cast<s16*>(tbl + 0x00);
            s16* posY = reinterpret_cast<s16*>(tbl + 0x14);
            f32* scale = reinterpret_cast<f32*>(tbl + 0x28);
                nw4r::math::VEC3 trans = pane->GetTranslate();
                trans.x = static_cast<f32>(posX[self->unk324]);
                trans.y = static_cast<f32>(posY[self->unk324]);
                pane->SetTranslate(trans);
                f32 s = scale[self->unk324];
                pane->SetScale(nw4r::math::VEC2(s, s));
            }
            if (self->unk328 == 4) {
                func_801072E0(self);
            } else {
                s16* selTab = reinterpret_cast<s16*>(tbl + 0x4C);
                s16 v = reinterpret_cast<s16*>(tbl + 0x4C)[self->unk328];
                char* nameStr = func_80136190(lbl_eu_804FD1E0 + 0x249,
                                              lbl_eu_804FD1E0 + 0x254, v);
                char* helpStr = func_80136190(lbl_eu_804FD1E0 + 0x249,
                                              lbl_eu_804FD1E0 + 0x259, v);
                func_80136B4C(self->unk80, lbl_eu_804FD1E0 + 0x69, nameStr, 0);
                func_80136B4C(self->unk80, lbl_eu_804FD1E0 + 0x5c, helpStr,
                              reinterpret_cast<u32>(self->unk294));
            }
            self->unk8C->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0x25E, true)
                ->SetVisible(true);
            self->unk8C->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0x26C, true)
                ->SetVisible(true);
            goto end_body;
        }
    }
    if (b31) {
        // prev-art scan
        void* arts = artsVslot<GetPtrFn>(actor, 0x278)(actor);
        s8 idx = (s8)(self->unk324 - 1);
        while ((s8)idx != self->unk324) {
            if (idx < 0) idx = 8;
            if (idx == 4) {
                self->unk324 = 4;
                break;
            }
            s32 q = idx;
            if (q >= 0) q--;
            ArtsParamInfo* p =
                reinterpret_cast<ArtsParamInfo*>(getArtsParamAtCnt(arts, q));
            if (p->mCheckFlag != 0) {
                self->unk324 = (s8)idx;
                break;
            }
            idx--;
        }
        nw4r::lyt::Pane* pane =
            self->unk8C->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0xC7, true);
        {
            s16* posX = reinterpret_cast<s16*>(tbl + 0x00);
            s16* posY = reinterpret_cast<s16*>(tbl + 0x14);
            f32* scale = reinterpret_cast<f32*>(tbl + 0x28);
            nw4r::math::VEC3 trans = pane->GetTranslate();
            trans.x = static_cast<f32>(posX[self->unk324]);
            trans.y = static_cast<f32>(posY[self->unk324]);
            pane->SetTranslate(trans);
            f32 s = scale[self->unk324];
            pane->SetScale(nw4r::math::VEC2(s, s));
        }
        if (self->unk324 == 4) {
            if (self->unk328 == 4) {
                func_801072E0(self);
            } else {
                s16* selTab = reinterpret_cast<s16*>(tbl + 0x4C);
                s16 v = reinterpret_cast<s16*>(tbl + 0x4C)[self->unk328];
                char* nameStr = func_80136190(lbl_eu_804FD1E0 + 0x249,
                                              lbl_eu_804FD1E0 + 0x254, v);
                char* helpStr = func_80136190(lbl_eu_804FD1E0 + 0x249,
                                              lbl_eu_804FD1E0 + 0x259, v);
                func_80136B4C(self->unk80, lbl_eu_804FD1E0 + 0x69, nameStr, 0);
                func_80136B4C(self->unk80, lbl_eu_804FD1E0 + 0x5c, helpStr,
                              reinterpret_cast<u32>(self->unk294));
            }
            self->unk8C->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0x25E, true)
                ->SetVisible(true);
            self->unk8C->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0x26C, true)
                ->SetVisible(true);
        } else {
            void* pl = cf::CfGameManager::getPlayer(0);
            void* adj = pl;
            if (pl != NULL) adj = (char*)pl - 0x3e9c;
            if (adj != NULL) {
                void* arts2 = artsVslot<GetPtrFn>(adj, 0x278)(adj);
                s32 q = self->unk324;
                if (q > 4) q--;
                ArtsParamInfo* p =
                    reinterpret_cast<ArtsParamInfo*>(getArtsParamAtCnt(arts2, q));
                char* name = (char*)p->mNamePtr;
                if (name == NULL) name = lbl_eu_80661E08;
                func_80136B4C(self->unk80, lbl_eu_804FD1E0 + 0x69, (char*)p, 0);
                func_80136B4C(self->unk80, lbl_eu_804FD1E0 + 0x5c, name,
                              reinterpret_cast<u32>(self->unk294));
            }
            self->unk8C->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0x25E, true)
                ->SetVisible(false);
            self->unk8C->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0x26C, true)
                ->SetVisible(false);
        }
        func_80138078__FUl(84);
        goto end_body;
    }
    if (b0) {
        // next-art scan
        void* arts = artsVslot<GetPtrFn>(actor, 0x278)(actor);
        s8 idx = (s8)(self->unk324 + 1);
        while ((s8)idx != self->unk324) {
            if (idx > 8) idx = 0;
            if (idx == 4) {
                self->unk324 = 4;
                break;
            }
            s32 q = idx;
            if (q >= 0) q--;
            ArtsParamInfo* p =
                reinterpret_cast<ArtsParamInfo*>(getArtsParamAtCnt(arts, q));
            if (p->mCheckFlag != 0) {
                self->unk324 = (s8)idx;
                break;
            }
            idx++;
        }
        nw4r::lyt::Pane* pane =
            self->unk8C->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0xC7, true);
        {
            s16* posX = reinterpret_cast<s16*>(tbl + 0x00);
            s16* posY = reinterpret_cast<s16*>(tbl + 0x14);
            f32* scale = reinterpret_cast<f32*>(tbl + 0x28);
            nw4r::math::VEC3 trans = pane->GetTranslate();
            trans.x = static_cast<f32>(posX[self->unk324]);
            trans.y = static_cast<f32>(posY[self->unk324]);
            pane->SetTranslate(trans);
            f32 s = scale[self->unk324];
            pane->SetScale(nw4r::math::VEC2(s, s));
        }
        if (self->unk324 == 4) {
            if (self->unk328 == 4) {
                func_801072E0(self);
            } else {
                s16* selTab = reinterpret_cast<s16*>(tbl + 0x4C);
                s16 v = reinterpret_cast<s16*>(tbl + 0x4C)[self->unk328];
                char* nameStr = func_80136190(lbl_eu_804FD1E0 + 0x249,
                                              lbl_eu_804FD1E0 + 0x254, v);
                char* helpStr = func_80136190(lbl_eu_804FD1E0 + 0x249,
                                              lbl_eu_804FD1E0 + 0x259, v);
                func_80136B4C(self->unk80, lbl_eu_804FD1E0 + 0x69, nameStr, 0);
                func_80136B4C(self->unk80, lbl_eu_804FD1E0 + 0x5c, helpStr,
                              reinterpret_cast<u32>(self->unk294));
            }
            self->unk8C->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0x25E, true)
                ->SetVisible(true);
            self->unk8C->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0x26C, true)
                ->SetVisible(true);
        } else {
            void* pl = cf::CfGameManager::getPlayer(0);
            void* adj = pl;
            if (pl != NULL) adj = (char*)pl - 0x3e9c;
            if (adj != NULL) {
                void* arts2 = artsVslot<GetPtrFn>(adj, 0x278)(adj);
                s32 q = self->unk324;
                if (q > 4) q--;
                ArtsParamInfo* p =
                    reinterpret_cast<ArtsParamInfo*>(getArtsParamAtCnt(arts2, q));
                char* name = (char*)p->mNamePtr;
                if (name == NULL) name = lbl_eu_80661E08;
                func_80136B4C(self->unk80, lbl_eu_804FD1E0 + 0x69, (char*)p, 0);
                func_80136B4C(self->unk80, lbl_eu_804FD1E0 + 0x5c, name,
                              reinterpret_cast<u32>(self->unk294));
            }
            self->unk8C->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0x25E, true)
                ->SetVisible(false);
            self->unk8C->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0x26C, true)
                ->SetVisible(false);
        }
        func_80138078__FUl(84);
        goto end_body;
    }
    if (bX) {
        // use/confirm flow
        s32 listIdx = self->unk324;
        if (listIdx == 4) {
            listIdx = 8;
        } else if (listIdx >= 4) {
            listIdx--;
        }
        if (func_80107970(self, listIdx) != 0) goto useFail;
        if (func_80107C54(self, listIdx) != 0) goto useFail;
        if (func_801086D0(self) != 0) goto useFail;

        if (self->unk324 == 4 && self->unk328 == 4) {
            void* pl = cf::CfGameManager::getPlayer(0);
            if (pl != NULL) {
                u8 v = (u8)(*(u16*)((char*)pl + 0x8C));
                if (v == 5 || v == 7) {
                    /* ok */
                } else {
                    void* mgr = func_8009EC9C(v);
                    void* sub = (char*)mgr + 0x17C;
                    f32 val = artsVslot<GetF32Fn>(sub, 0x158)(sub);
                    if (val == lbl_eu_80666F50) {
                        /* ok */
                    } else {
                        goto useFail;
                    }
                }
            } else {
                goto useFail;
            }
        }
        if (func_801088CC(self) != 0) goto useFail;

        void* arts = artsVslot<GetPtrFn>(actor, 0x278)(actor);
        s32 q3 = self->unk324;
        if (q3 > 4) q3--;
        ArtsParamInfo* p =
            reinterpret_cast<ArtsParamInfo*>(getArtsParamAtCnt(arts, q3));
        if (self->unk324 != 4 && func_80154168((const u8*)p) != 0) {
            self->unk330 = 0;
            if (func_8015419C((u8*)p) != 0) {
                int* cfg = func_8009ECB0();
                for (s32 i = 0; i < 3; i++) {
                    if (cfg[i + 1] == (s32)actor->mField3F28) continue;
                    void* obj = func_800B8B94(cfg[i + 1]);
                    if (artsVslot<GetF32Fn>(obj, 0x128)(obj) > lbl_eu_80666F28) {
                        self->unk330 = i;
                        break;
                    }
                }
            }
            self->unk8C->GetRootPane()->SetVisible(false);
            func_80138078__FUl(95);
            self->unk298 = 5;
            if (func_80110A70() != NULL) {
                func_8010EDDC((u8*)func_80110A70() + 0x7E4, (u8)self->unk330);
                func_8010ED18((u8*)func_80110A70() + 0x7E4);
            }
            if (func_8010CE48() != NULL) {
                func_8010A848((u8*)func_8010CE48() + 0x7CC, (u8)self->unk330);
                func_8010A6F0((u8*)func_8010CE48() + 0x7CC);
            }
            goto end_body;
        }

        if (self->unk324 == 4) {
            u8 t[5];
            *(u32*)&t[0] = lbl_eu_80666F48;
            t[4] = lbl_eu_80666F4C;
            s8 sv = (s8)t[self->unk328];
            self->unk7C = 0;
            self->unk7D = sv;
            self->unk7E = -1;
            self->unk320 = 0;
            if (sv == 6) {
                if (func_80187710() != 0) func_80187718();
            }
            if (self->unk328 == 2) self->unk348 = 1;
            if (self->unk328 == 4) {
                bool atEnd = true;
                if (lbl_eu_80663F20 != NULL) {
                    for (s32 i = 0; i < 9; i++) {
                        nw4r::lyt::AnimTransform* a = lbl_eu_80663F20->unk1DC[i];
                        if (a->GetFrame() !=
                            (f32)(u16)a->GetFrameSize() - 1.0f) {
                            atEnd = false;
                            break;
                        }
                    }
                }
                if (!atEnd) {
                    for (s32 i = 0; i < 9; i++) {
                        nw4r::lyt::AnimTransform* a = lbl_eu_80663F20->unk1DC[i];
                        a->SetFrame((f32)(u16)a->GetFrameSize() - 1.0f);
                    }
                }
            }
        } else {
            s32 q4 = self->unk324;
            if (q4 > 4) q4--;
            self->unk7C = 1;
            self->unk7D = (s8)q4;
            self->unk7E = -1;
        }
        func_80138078__FUl(95);
        self->unk298 = 4;
        self->unk8C->SetAnimationEnable(self->unk94, false);
        self->unk8C->SetAnimationEnable(self->unk90, true);
        self->unk90->SetFrame(lbl_eu_80666F28);
        self->unk8C->Animate(0);
        goto end_body;
    }
    goto end_body;

useFail:
    func_80138078__FUl(5);
end_body:
    func_80137444(self->unk94, lbl_eu_80666F2C);
}

void CMenuArtsSelect::func_80105A34() {
    // Case-4 page-mode driver: when the party-target latch fires, adopt the
    // talent page (unk328 = 4) or revert to the main arts page, refreshing
    // the name/help text; the unk90 anim completion rewinds the sub-anims.
    BattleActor* actor =
        reinterpret_cast<BattleActor*>(cf::CfGameManager::getPlayer(0));
    if (actor != NULL) {
        actor = reinterpret_cast<BattleActor*>(reinterpret_cast<char*>(actor) - 0x3e9c);
    }
    if (actor != NULL) {
        u32* pVal = reinterpret_cast<u32*>(
            reinterpret_cast<ArtsSubVtbl*>(actor->mSecondaryVtable)->mFn30());
        int localVal = pVal[0];
        if (func_80174C98(actor, &localVal, 0x803) != 0) {
            if (unk324 == 4) {
                if (unk320 == 0) {
                    if (unk328 == 0) {
                        unk328 = 4;
                        ::func_80107580(this);
                        ::func_801072E0(this);
                    } else if (unk328 == 2) {
                        unk320 = 1;
                        unk328 = 0;
                        ::func_80107580(this);
                        s16 v = lbl_eu_804FD11C[unk328];
                        char* name = func_80136190(lbl_eu_804FD1E0 + 0x249,
                                                    lbl_eu_804FD1E0 + 0x254, v);
                        char* help = func_80136190(lbl_eu_804FD1E0 + 0x249,
                                                    lbl_eu_804FD1E0 + 0x259, v);
                        func_80136B4C(unk80, lbl_eu_804FD1E0 + 0x69, name, 0);
                        func_80136B4C(unk80, lbl_eu_804FD1E0 + 0x5c, help,
                                      reinterpret_cast<u32>(unk294));
                    }
                }
            } else {
                s32 mode = unk328;
                if (mode == 0 || mode == 2) {
                    unk328 = 4;
                    ::func_80107580(this);
                    if (mode == 0) {
                        cf::CfObjectMove* pl = cf::CfGameManager::getPlayer(0);
                        BattleActor* a2 = reinterpret_cast<BattleActor*>(pl);
                        if (pl != NULL) {
                            a2 = reinterpret_cast<BattleActor*>(reinterpret_cast<char*>(pl) - 0x3e9c);
                        }
                        if (a2 != NULL) {
                            u8* arts2 = static_cast<u8*>(
                                reinterpret_cast<ArtsActorVtbl*>(a2)->mFn278());
                            s32 q = unk324;
                            if (q > 4) q--;
                            ArtsParamInfo* p2 = reinterpret_cast<ArtsParamInfo*>(
                                getArtsParamAtCnt(arts2, q));
                            char* name = static_cast<char*>(p2->mNamePtr);
                            name = (name != NULL) ? name : lbl_eu_80661E08;
                            func_80136B4C(unk80, lbl_eu_804FD1E0 + 0x69,
                                          reinterpret_cast<char*>(p2), 0);
                            func_80136B4C(unk80, lbl_eu_804FD1E0 + 0x5c, name,
                                          reinterpret_cast<u32>(unk294));
                        }
                    } else {
                        ::func_801072E0(this);
                    }
                }
            }
        } else if (unk328 == 1) {
            unk328 = 0;
            unk320 = 0;
            ::func_80107580(this);
            s16 v = lbl_eu_804FD11C[unk328];
            char* name = func_80136190(lbl_eu_804FD1E0 + 0x249,
                                        lbl_eu_804FD1E0 + 0x254, v);
            char* help = func_80136190(lbl_eu_804FD1E0 + 0x249,
                                        lbl_eu_804FD1E0 + 0x259, v);
            func_80136B4C(unk80, lbl_eu_804FD1E0 + 0x69, name, 0);
            func_80136B4C(unk80, lbl_eu_804FD1E0 + 0x5c, help,
                          reinterpret_cast<u32>(unk294));
        }
    }

    if (func_80137444(unk90, lbl_eu_80666F2C) != 0) {
        unk8C->SetAnimationEnable(unk90, false);
        unk8C->SetAnimationEnable(unk94, true);
        unk94->SetFrame(lbl_eu_80666F28);
        unk8C->Animate(0);
        unk298 = 2;
    }
}
// ---------------------------------------------------------------------------
// func_80105D54 (us-8010683c) -- arts-select confirm/target handler.
// First hides the current entry (twice, gated by the open-gate probe and the
// slot probe), then reads the pad: turbo page bits drive a candidate scan of
// the three party slots whose results are distance-sorted; direction bits
// rotate unk330 through the sorted list, and the confirm bit switches the
// page/mode (unk7C/unk7D/unk7E) and moves to state 8.
// ---------------------------------------------------------------------------
extern "C" void func_80105D54(CMenuArtsSelect* self) {
    typedef void* (*GetPtrFn)(void*);
    typedef f32 (*GetF32Fn)(void*);

    // Stack locals laid out to mirror retail frame slots
    // (0x08 / 0x10 / 0x18 / 0x20 / 0x2c / 0x38).
    u8 order[3];            // candidate slot ids (retail +0x08)
    ArtsEnumHolder holder;  // enum-list holder (retail +0x10)
    ArtsSelModeTbl tbl;     // mode-table copy (retail +0x18)
    ArtsSelEnumIds ids;     // enum id table copy (retail +0x20)
    f32 d;                  // distance temp (retail +0x2c)
    f32 dist[3];            // candidate distances (retail +0x38)

    if (func_8010784C(self) != 0) {
        UnkArtsSelectEntry* e =
            reinterpret_cast<UnkArtsSelectEntry*>(self->unk8C->GetRootPane());
        e->unkBB = static_cast<u8>((e->unkBB & 0xFEu) | 1);
        func_80138078__FUl(6);
        self->unk298 = 7;
        if (func_80110A70() != NULL) {
            func_8010ED38(reinterpret_cast<u8*>(func_80110A70()) + 0x7e4);
        }
        if (func_8010CE48() != NULL) {
            func_8010A710(reinterpret_cast<u8*>(func_8010CE48()) + 0x7cc);
        }
    }

    s32 probeIdx = self->unk324;
    if (probeIdx == 4) {
        probeIdx = 8;
    } else if (probeIdx > 4) {
        probeIdx = probeIdx - 1;
    }
    if (func_80107C54(self, probeIdx) != 0) {
        UnkArtsSelectEntry* e =
            reinterpret_cast<UnkArtsSelectEntry*>(self->unk8C->GetRootPane());
        e->unkBB = static_cast<u8>((e->unkBB & 0xFEu) | 1);
        func_80138078__FUl(6);
        self->unk298 = 7;
        if (func_80110A70() != NULL) {
            func_8010ED38(reinterpret_cast<u8*>(func_80110A70()) + 0x7e4);
        }
        if (func_8010CE48() != NULL) {
            func_8010A710(reinterpret_cast<u8*>(func_8010CE48()) + 0x7cc);
        }
    }

    CPad* pad = cf::CfGameManager::getCurrentPad();
    bool bA;
    bool bB;
    bool bC;
    bool bD;
    int selDir;
    if (cf::CfGameManager::func_80086F9C(-1) != 0) {
        if ((pad->mHeldButtonFlags & (1u << 27)) != 0) return;
        if ((pad->mHeldButtonFlags & (1u << 28)) != 0) return;
        if ((pad->mHeldButtonFlags & (1u << 25)) != 0) return;
        if ((pad->mHeldButtonFlags & (1u << 26)) != 0) return;
        bA = (pad->mTurboPressButtonFlags >> 2) & 1;
        bB = (pad->mTurboPressButtonFlags >> 3) & 1;
        bC = pad->mTurboPressButtonFlags & 1;
        bD = (pad->mTurboPressButtonFlags >> 1) & 1;
        selDir = (pad->mPressedButtonFlags >> 21) & 1;
        if ((pad->mHeldButtonFlags & (1u << 22)) != 0) selDir = 0;
    } else {
        if ((pad->mHeldButtonFlags & (1u << 11)) != 0) return;
        if ((pad->mHeldButtonFlags & (1u << 12)) != 0) return;
        bA = (pad->mTurboPressButtonFlags >> 2) & 1;
        bB = (pad->mTurboPressButtonFlags >> 3) & 1;
        bC = pad->mTurboPressButtonFlags & 1;
        bD = (pad->mTurboPressButtonFlags >> 1) & 1;
        selDir = (pad->mPressedButtonFlags >> 4) & 1;
        if ((pad->mHeldButtonFlags & (1u << 5)) != 0) selDir = 0;
    }

    if (!bA && !bB && !bC && !bD) {
        // No page-direction input.
        if (selDir == 0) {
            if (self->unk324 == 0) return;
            if (func_8017FD44() != 0) return;
            func_80138078__FUl(6);
            self->unk298 = 7;
            if (func_80110A70() != NULL) {
                func_8010ED38(reinterpret_cast<u8*>(func_80110A70()) + 0x7e4);
            }
            if (func_8010CE48() != NULL) {
                func_8010A710(reinterpret_cast<u8*>(func_8010CE48()) + 0x7cc);
            }
            return;
        }

        // Confirm: record page/mode switch into unk7C/unk7D/unk7E.
        if (self->unk324 == 4) {
            tbl.data = lbl_eu_80666F54;
            tbl.extra = lbl_eu_80666F58;
            self->unk7C = 0;
            self->unk7D = reinterpret_cast<const u8*>(&tbl)[self->unk328];
            self->unk7E = static_cast<u8>(self->unk330);
        } else {
            func_80496264(self->mScn, -1);
            int* cfg = func_8009ECB0();            u32 id = cfg[self->unk330 + 1];
            BattleActor* cand =
                static_cast<BattleActor*>(func_800B8B94(id));
            CfObjectMoveArtsView* mv = reinterpret_cast<CfObjectMoveArtsView*>(cand);
            if (mv != NULL) {
                mv = reinterpret_cast<CfObjectMoveArtsView*>(
                    reinterpret_cast<char*>(mv) + 0x3e9c);
            }
            s32 matchIdx = 0;
            if (mv != NULL) {
                func_80043D90(&holder);
                ids.id[0] = lbl_eu_804FD128.id[0];
                ids.id[1] = lbl_eu_804FD128.id[1];
                ids.id[2] = lbl_eu_804FD128.id[2];
                // Find which enum type matches the target's +0x8C object id.
                for (s32 k = 0; k < 3; k++) {
                    ArtsEnumList* list =
                        static_cast<ArtsEnumList*>(func_80043F18(&holder));
                    func_800F4A98(list, ids.id[k], 0);
                    list = static_cast<ArtsEnumList*>(func_80043F18(&holder));
                    if (list->mCount620 == 0) continue;
                    list = static_cast<ArtsEnumList*>(func_80043F18(&holder));
                    ArtsEnumSlot* slot =
                        static_cast<ArtsEnumSlot*>(func_800F6EC0(list, 0));
                    if (slot->mObj == NULL) continue;
                    if (mv->field_8C == slot->mObj->field_8C) {
                        matchIdx = k;
                        break;
                    }
                }
                __dt__80043E88(&holder, -1);
            }
            s32 v = self->unk324;
            if (v > 4) v--;
            self->unk7C = 1;
            self->unk7D = v;
            self->unk7E = matchIdx;
        }

        bool skipState = false;
        if (self->unk324 == 4 && self->unk328 == 7) {
            if (func_802795D4(
                    reinterpret_cast<u8*>(cf::CBattleManager::getInstance()) + 0x1a8,
                    0) == 0) {
                skipState = true;
            }
        }
        if (!skipState) {
            self->unk298 = 8;
            if (func_80110A70() != NULL) {
                func_8010ED58(reinterpret_cast<u8*>(func_80110A70()) + 0x7e4);
            }
            if (func_8010CE48() != NULL) {
                func_8010A7A8(reinterpret_cast<u8*>(func_8010CE48()) + 0x7cc);
            }
            func_80138078__FUl(0x5f);
        }
        return;
    }

    // Page direction held: scan the three party candidates.
    BattleActor* actor =
        reinterpret_cast<BattleActor*>(cf::CfGameManager::getPlayer(0));
    if (actor != NULL) {
        actor = reinterpret_cast<BattleActor*>(reinterpret_cast<char*>(actor) - 0x3e9c);
    }
    if (self->unk31C <= 1) return;
    if (actor == NULL) return;

    void* pose = func_80496264(self->mScn, -1);
    int* cfg = func_8009ECB0();

    s32 listIdx = self->unk324;
    if (listIdx == 4) {
        listIdx = 8;
    } else if (listIdx > 4) {
        listIdx = listIdx - 1;
    }

    bool hasGauge = false;
    if (listIdx < 8) {
        void* arts = artsVslot<GetPtrFn>(actor, 0x278)(actor);
        ArtsParamInfo* p =
            reinterpret_cast<ArtsParamInfo*>(getArtsParamAtCnt(arts, listIdx));
        if (func_8015419C(reinterpret_cast<u8*>(p)) != 0) hasGauge = true;
    }

    f32 zeroF = lbl_eu_80666F28;
    s32 count = 0;
    for (s32 i = 0; i < 3; i++) {
        u32 entryId = cfg[i + 1];
        BattleActor* cand = static_cast<BattleActor*>(func_800B8B94(entryId));
        CfObjectMoveArtsView* mv = reinterpret_cast<CfObjectMoveArtsView*>(cand);
        if (mv != NULL) {
            mv = reinterpret_cast<CfObjectMoveArtsView*>(
                reinterpret_cast<char*>(mv) + 0x3e9c);
        }
        if (mv == NULL) continue;
        f32 g = artsVslot<GetF32Fn>(cand, 0x128)(cand);
        if (g <= zeroF) continue;
        if (hasGauge) {
            if (entryId == actor->mField3F28) continue;
        }
        nw4r::math::VEC3* pos =
            artsVslot<nw4r::math::VEC3* (*)(void*)>(mv, 0xAC)(mv);
        f32 d;
        func_8049B59C(&d, pose, pos);
        dist[count] = d;
        order[count] = static_cast<u8>(i);
        count++;
    }

    u8 changed = 0;
    if (count >= 2) {
        if (bC || bD) {
            // Bubble sort descending by distance (retail pass loop).
            for (s32 pass = 0; pass < count;) {
                bool sorted = true;
                for (s32 j = 0; j < count - 1; j++) {
                    u8 a = order[j];
                    u8 b = order[j + 1];
                    if (dist[a] > dist[b]) {
                        order[j] = b;
                        order[j + 1] = a;
                        sorted = false;
                    }
                }
                if (sorted) break;
                pass++;
            }
        }
        s32 sel = 0;
        for (s32 j = 0; j < count; j++) {
            if (order[j] == self->unk330) {
                sel = j;
                break;
            }
        }
        if (bC || bA) {
            sel--;
            if (sel < 0) sel = count - 1;
        } else {
            sel++;
            if (sel >= count) sel = 0;
        }
        self->unk330 = order[sel];
        changed = 1;
    } else {
        if (count != 0) {
            if (self->unk330 != order[0]) {
                self->unk330 = order[0];
                changed = 1;
            }
        }
    }
    if (changed) {
        if (func_80110A70() != NULL) {
            func_8010EDDC((u8*)func_80110A70() + 0x7e4, (u8)self->unk330);
        }
        if (func_8010CE48() != NULL) {
            func_8010A848((u8*)func_8010CE48() + 0x7cc, (u8)self->unk330);
        }
        func_80138078__FUl(0x54);
    }
}
// ---------------------------------------------------------------------------
// func_80106450 (us-80106f38) -- arts-select open frame timer + auto-open.
// unk344 counts up by 1.0f per frame; when it passes the cap the menu jumps
// to state 7 and the frame resets. While the party is battle-capable the
// menu re-opens: page mode is forced to 4 and the target-arts name/help text
// (or the full layout, for mode 2) is refreshed.
// ---------------------------------------------------------------------------
void CMenuArtsSelect::func_80106450() {
    typedef void* (*GetPtrFn)(void*);

    f32 v = unk344 + lbl_eu_80666F2C;
    unk344 = v;
    if (v >= lbl_eu_80666F5C) {
        unk298 = 7;
        unk344 = lbl_eu_80666F28;
    }

    cf::CfObjectMove* move = cf::CfGameManager::getPlayer(0);
    BattleActor* actor = reinterpret_cast<BattleActor*>(move);
    if (move != NULL) actor = (BattleActor*)((char*)move - 0x3e9c);
    if (actor != NULL) {
        void* sub = actor->mSecondaryVtable;
        u32* pVal = reinterpret_cast<u32*>(
            reinterpret_cast<ArtsSubVtbl*>(sub)->mFn30());
        int localVal = pVal[0];
        if (func_80174C98(actor, &localVal, 0x803) != 0) {
            if (unk324 != 4) {
                s32 mode = unk328;
                if (mode == 0 || mode == 2) {
                    unk328 = 4;
                    ::func_80107580(this);
                    if (mode == 0) {
                        cf::CfObjectMove* pl = cf::CfGameManager::getPlayer(0);
                        BattleActor* a2 = reinterpret_cast<BattleActor*>(pl);
                        if (pl != NULL) a2 = (BattleActor*)((char*)pl - 0x3e9c);
                        if (a2 != NULL) {
                            void* arts = reinterpret_cast<ArtsActorVtbl*>(a2)->mFn278();
                            s32 q = unk324;
                            if (q > 4) q--;
                            ArtsParamInfo* p = reinterpret_cast<ArtsParamInfo*>(
                                getArtsParamAtCnt(arts, q));
                            char* name = static_cast<char*>(p->mNamePtr);
                            // Retail: cmpwi; beq .fb; b .cont; .fb:
                            // li name, lbl_eu_80661E08@sda21; .cont: -- the
                            // select keeps name in r30; only this ternary
                            // shape emits the branch-over-branch.
                            name = (name != NULL) ? name : lbl_eu_80661E08;
                            func_80136B4C(unk80, lbl_eu_804FD1E0 + 0x69,
                                          reinterpret_cast<char*>(p), 0);
                            func_80136B4C(unk80, lbl_eu_804FD1E0 + 0x5c, name,
                                          reinterpret_cast<u32>(unk294));
                        }
                    } else {
                        ::func_801072E0(this);
                    }
                    unk298 = 7;
                    unk344 = lbl_eu_80666F28;
                }
            }
        }
    }
}

void CMenuArtsSelect::func_801065E4() {
    // Case-9 gauge driver: animate the parent arts gauge toward the party
    // gauge ratio; when it maxes out, either advance to the idle case 0xb or
    // rewind the anims and set the window text for the selected art.
    unk308 |= 0x2;

    // Self-adjust: null test reads the pre-adjust value (retail: mr/beq/subi).
    BattleActor* actor =
        reinterpret_cast<BattleActor*>(cf::CfGameManager::getPlayer(0));
    if (actor != NULL) {
        actor = reinterpret_cast<BattleActor*>(reinterpret_cast<char*>(actor) - 0x3e9c);
    }
    if (actor != NULL) {
        // NOTE: retail names the shared .sdata2 2^52 magic (lbl_eu_80666F38)
        // for this u16->f32 conversion; MWCC will not merge the implicit
        // conversion literal against the named constant in this function's
        // schedule, so the @N pool entry here stays a known residual.
        f32 frameLimit = static_cast<f32>(unk9C->GetFrameSize()) - lbl_eu_80666F2C;
        f32 v = frameLimit * reinterpret_cast<ArtsActorVtbl*>(actor)->mFn160();
        if (v != unk9C->GetFrame() || (unk308 & 0x4) != 0) {
            unk308 |= 0x1;
        } else {
            unk308 &= ~0x1;
        }
        unk9C->SetFrame(v);
        if (v >= frameLimit) {
            if (unk308 & 0x4) {
                unk29C = 0xb;
                unk308 &= ~0x3;
            } else {
                unk98->SetAnimationEnable(unk9C, false);
                unk98->SetAnimationEnable(unkA0, true);
                func_80138078__FUl(0x9f);
                unkA0->SetFrame(lbl_eu_80666F28);
                unk29C = 0xa;
                unk308 |= 0x3;
            }
        }

        // Talent-art readiness latch: the RC art param's gauge flag, the
        // sub-state flag bit 8 and the talent gate (art id 7) clear it.
        s32 flag = 0;
        ArtsParamInfo* rc = reinterpret_cast<ArtsParamInfo*>(getArtsParamRC(
            static_cast<u8*>(
                reinterpret_cast<ArtsActorVtbl*>(actor)->mFn278()),
            2, 0));
        if (rc->mCheckFlag != 0) {
            if (func_80154280(rc, actor, 0) & 0x80) flag = 1;
            if (actor->mField3F28 == 7 && flag != 0 && func_801088CC(this) == 0) {
                flag = 0;
            }
        }

        // Window rect: slot defaults to {0,0,0,fs.c}; when the talent latch
        // survives, use the {0x14,0x58,0xc6} highlight rect and unk291 = 1.
        // MWCC lowers the ArtsGaugeBox initializer to a word-pair copy of the
        // FourShorts plus an lha readback for .w (see header note).
        FourShorts fs = func_80139658(unk98, lbl_eu_804FD1E0 + 0x27a, 1);
        s16 rect[6] = {0, 0, 0, fs.c, fs.a, fs.b};
        unk291 = 0;
        if (unk289[0] != 0) flag = 0;
        if (unk289[1] != 0) flag = 0;
        if (unk289[2] != 0) flag = 0;
        if (unk289[3] != 0) flag = 0;
        if (unk289[4] != 0) flag = 0;
        if (unk289[5] != 0) flag = 0;
        if (unk289[6] != 0) flag = 0;
        if (unk289[7] != 0) flag = 0;
        if (flag != 0) {
            rect[0] = 0x14;
            rect[1] = 0x58;
            rect[2] = 0xc6;
            rect[3] = fs.c;
            unk291 = 1;
        }
        char* winNames = lbl_eu_804FD1E0;  // retail hoists the table base
        func_801398A4(unk98, winNames + 0x27a, rect, 1);
        func_801398A4(unk98, winNames + 0x283, rect, 1);
        unk308 &= ~0x4;
    }
}
void CMenuArtsSelect::func_80106900() {
    // Case-0xb gauge driver (inverse of func_801065E4): while the gauge is
    // below the limit, rewind the anims and set the window text; once the
    // talent latch survives, set the highlight rect and replay the anims.
    unk308 &= ~0x3;

    // Self-adjust: null test reads the pre-adjust value (retail: mr/beq/subi).
    BattleActor* actor =
        reinterpret_cast<BattleActor*>(cf::CfGameManager::getPlayer(0));
    if (actor != NULL) {
        actor = reinterpret_cast<BattleActor*>(reinterpret_cast<char*>(actor) - 0x3e9c);
    }
    if (actor != NULL) {
        // u16 -> f32 goes through double; the [0x43300000, v] bit-pattern
        // scratch is declared first so its stack slot lands at the top of the
        // frame like retail's conversion scratch (0x38/0x3c(sp)).
        f32 frameLimit;
        f32 v;
        {
            union {
                double d;
                u32 w[2];
            } conv;
            conv.w[1] = static_cast<u16>(unk9C->GetFrameSize());
            double magic = lbl_eu_80666F38;  // blocks the idiom fold
            conv.w[0] = 0x43300000;
            frameLimit = static_cast<f32>(conv.d - magic) - lbl_eu_80666F2C;
        }
        v = frameLimit * reinterpret_cast<ArtsActorVtbl*>(actor)->mFn160();
        if (v < frameLimit) {
            unk98->SetAnimationEnable(unkA0, false);
            unk98->SetAnimationEnable(unk9C, true);
            unk29C = 0x9;
            FourShorts fs = func_80139658(unk98, lbl_eu_804FD1E0 + 0x27a, 1);
            // Aggregate init lowers to a word-pair quad copy plus an lha
            // readback for .w (see header note on ArtsGaugeBox).
            ArtsGaugeBox box = {0, 0, 0, fs.c, fs};
            unk291 = 0;
            func_801398A4(unk98, lbl_eu_804FD1E0 + 0x27a,
                          reinterpret_cast<s16*>(&box), 1);
            func_801398A4(unk98, lbl_eu_804FD1E0 + 0x283,
                          reinterpret_cast<s16*>(&box), 1);
        } else {
            s32 flag = 0;
            u8* arts = static_cast<u8*>(
                reinterpret_cast<ArtsActorVtbl*>(actor)->mFn278());
            ArtsParamInfo* rc = reinterpret_cast<ArtsParamInfo*>(getArtsParamRC(arts, 2, 0));
            if (rc->mCheckFlag != 0) {
                if (func_80154280(rc, actor, 0) & 0x80) flag = 1;
                if (actor->mField3F28 == 7 && flag != 0 && func_801088CC(this) == 0) {
                    flag = 0;
                }
            }
            if (unk289[0] != 0) flag = 0;
            if (unk289[1] != 0) flag = 0;
            if (unk289[2] != 0) flag = 0;
            if (unk289[3] != 0) flag = 0;
            if (unk289[4] != 0) flag = 0;
            if (unk289[5] != 0) flag = 0;
            if (unk289[6] != 0) flag = 0;
            if (unk289[7] != 0) flag = 0;
            if (flag != 0) {
                unk308 |= 0x3;
                FourShorts fs = func_80139658(unk98, lbl_eu_804FD1E0 + 0x27a, 1);
                ArtsGaugeBox box = {0x14, 0x58, 0xc6, fs.c, fs};
                unk291 = 1;
                func_801398A4(unk98, lbl_eu_804FD1E0 + 0x27a,
                              reinterpret_cast<s16*>(&box), 1);
                func_801398A4(unk98, lbl_eu_804FD1E0 + 0x283,
                              reinterpret_cast<s16*>(&box), 1);
                unk98->SetAnimationEnable(unkA0, false);
                unk98->SetAnimationEnable(unk9C, true);
                unk9C->SetFrame(lbl_eu_80666F28);
            }
        }
    }
}
// ---------------------------------------------------------------------------
// func_80106C30 (us-80107718) -- per-slot arts gauge animation driver.
// Clears the slot's latch bits, then when the slot's art has a gauge
// (mCheckFlag) computes the target frame from the gauge ratio: ratio > 0
// rewinds the anim (stopping it if it was playing), ratio <= 0 starts the
// anim (playing the 0x9f SE once) with the frame frozen at 0. The final
// frame is written back to unkC4[index] and the latch bits are re-set
// depending on where the target frame lands vs the frame size / current
// frame.
// ---------------------------------------------------------------------------
void CMenuArtsSelect::func_80106C30(s32 index) {
    u32 bitB = 1u << (index + 9);
    u32 bitA = 1u << index;
    u32 mask = bitA | bitB;
    unk318 &= ~mask;

    cf::CfObjectMove* move = cf::CfGameManager::getPlayer(0);
    BattleActor* actor = reinterpret_cast<BattleActor*>(move);
    if (move != NULL) actor = (BattleActor*)((char*)move - 0x3e9c);
    if (actor != NULL) {
        void* arts = reinterpret_cast<ArtsActorVtbl*>(actor)->mFn278();
        ArtsParamLocal* info =
            reinterpret_cast<ArtsParamLocal*>(getArtsParamAtCnt(arts, index));
        if (info->mCheckFlag != 0) {
            // getMax is a real virtual at the +0x84 vptr's slot 0x14 (retail
            // CAttackParam_UnkVirtualFunc4) - a real virtual call reproduces
            // the retail r12 ABI dispatch (lwz r12,0x84(r3); lwz r12,0x14(r12);
            // mtctr; bcctrl) that a manual fn-ptr fetch cannot.
            f32 max = info->mFn14();
            f32 ratio;
            if (max != lbl_eu_80666F28) {
                max = info->mFn14();
                ratio = info->mRatioNum / max;
            } else {
                ratio = lbl_eu_80666F44;
            }

            f32 f30 = static_cast<f32>(unkC4[index]->GetFrameSize()) -
                      lbl_eu_80666F2C;
            bool wasRunning = true;
            if (ratio > lbl_eu_80666F28) {
                f32 fs2 = static_cast<f32>(unkC4[index]->GetFrameSize());
                f30 = f30 - ratio * fs2;
                if (f30 < lbl_eu_80666F28) f30 = lbl_eu_80666F28;
                if (unk337[index] != 0) {
                    unk337[index] = 0;
                }
            } else {
                if (unk337[index] == 0) {
                    if (unk340 == 0) {
                        func_80138078__FUl(0x9f);
                    }
                    unk337[index] = 1;
                    unkA4[index]->SetAnimationEnable(unkC4[index], false);
                    unkA4[index]->SetAnimationEnable(unkE4[index], true);
                    unkE4[index]->SetFrame(lbl_eu_80666F28);
                    wasRunning = false;
                    unk2A0[index] = 0xa;
                    unk318 |= mask;
                }
            }

            f32 fs3 = static_cast<f32>(unkC4[index]->GetFrameSize());
            if (fs3 - lbl_eu_80666F2C > f30) {
                unk318 |= bitA;
            }
            if (f30 != unkC4[index]->GetFrame()) {
                unk318 |= bitB;
            } else if (wasRunning) {
                unk318 &= ~bitB;
            }
            unkC4[index]->SetFrame(f30);
        }
    }
}
void CMenuArtsSelect::func_80106EC8(s32 index) {
    // Per-slot arts gauge highlight driver: when the partner art is charged
    // (flag bit8) light the slot up; otherwise fade it back out and latch the
    // gauge ratio into the slot's anim state.
    cf::CfObjectMove* move = cf::CfGameManager::getPlayer(0);
    BattleActor* actor = reinterpret_cast<BattleActor*>(move);
    if (move != NULL) {
        actor = reinterpret_cast<BattleActor*>(reinterpret_cast<char*>(move) - 0x3e9c);
    }
    if (actor == NULL) return;
    void* arts = reinterpret_cast<ArtsActorVtbl*>(actor)->mFn278();

    s32 flag = 0;
    if (index < 8) {
        // Partner readiness: same probe chain against the second player.
        cf::CfObjectMove* pl = cf::CfGameManager::getPlayer(0);
        if (pl != NULL) {
            void* other = func_8016FE34(pl);
            void* arts2 = reinterpret_cast<ArtsActorVtbl*>(other)->mFn278();
            ArtsParamInfo* p = reinterpret_cast<ArtsParamInfo*>(
                getArtsParamAtCnt(arts2, index));
            if (p->mCheckFlag != 0) {
                if (func_80154280(p, other, 0) & 0x100) flag = 1;
            }
        }
    }

    // Both branches' locals live in one switch-body scope so MWCC gives them
    // disjoint stack slots (retail keeps two fs/box sets).
    switch (flag) {
    case 0:
        if ((unk318 & (1 << index)) != 0) {
            FourShorts fs = func_80139658(unkA4[index], lbl_eu_804FD1E0 + 0x27a, 1);
            ArtsGaugeBox box = {0, 0, 0, fs.d, fs};
            unk289[index] = 0;
            func_801398A4(unkA4[index], lbl_eu_804FD1E0 + 0x27a,
                          reinterpret_cast<s16*>(&box), 1);
            func_801398A4(unkA4[index], lbl_eu_804FD1E0 + 0x283,
                          reinterpret_cast<s16*>(&box), 1);
        }
        unk318 &= ~((1 << index) | (1 << (index + 9)));

        // Latch the current/max gauge ratio into the slot's anim frame.
        ArtsParamLocal* p = reinterpret_cast<ArtsParamLocal*>(getArtsParamAtCnt(arts, index));
        f32 ratio;
        if (p->mFn14() == lbl_eu_80666F28) {
            ratio = lbl_eu_80666F44;
        } else {
            ratio = p->mRatioNum / p->mFn14();
        }
        // u16 frame-size -> f32 (MWCC magic), then trim 2 frames.
        f32 frame = static_cast<f32>(static_cast<u16>(unkC4[index]->GetFrameSize())) -
                    lbl_eu_80666F2C;
        if (ratio > lbl_eu_80666F28) {
            unk318 |= (1 << index) | (1 << (index + 9));
            unk2A0[index] = 9;
        } else {
            unkC4[index]->SetFrame(frame);
        }
        break;

    default:
        unk318 |= (1 << index) | (1 << (index + 9));
        FourShorts fs2 = func_80139658(unkA4[index], lbl_eu_804FD1E0 + 0x27a, 1);
        ArtsGaugeBox box2 = {0x14, 0x58, 0xc6, fs2.d, fs2};
        unk289[index] = 1;
        func_801398A4(unkA4[index], lbl_eu_804FD1E0 + 0x27a,
                      reinterpret_cast<s16*>(&box2), 1);
        func_801398A4(unkA4[index], lbl_eu_804FD1E0 + 0x283,
                      reinterpret_cast<s16*>(&box2), 1);
        unkC4[index]->SetFrame(lbl_eu_80666F28);
        break;
    }
}
// ---------------------------------------------------------------------------
// func_801071B8 (us-80107ca0) -- per-slot art-availability update.
// nextIdx wraps (i+1)%9; when the next slot's pane is available the slot's
// anims are rewound (SetAnimationEnable + frame reset) and unk310 bits are
// latched; otherwise the slot is disabled back to mode 0xc.
// ---------------------------------------------------------------------------
void CMenuArtsSelect::func_801071B8(s32 index) {
    s32 nextIdx = (index == 8) ? 0 : (index + 1);
    if (unk200[nextIdx]->unkBB & 1) {
        // Compound form makes MWCC reuse r0 for the final andc + store.
        unk310 |= 1u << index;
        unk310 &= ~(1u << (index + 9));
        if (::func_80107C54(this, index) == 0) {
            unk104[index]->SetAnimationEnable(unk128[index], false);
            unk104[index]->SetAnimationEnable(unk14C[index], true);
            unk14C[index]->SetFrame(lbl_eu_80666F28);
            unk2C0[index] = 0xf;
            unk310 |= (1u << index) | (1u << (index + 9));
        }
    } else {
        unk310 &= ~((1u << index) | (1u << (index + 9)));
        unk2C0[index] = 0xc;
    }
}

// ---------------------------------------------------------------------------
// func_801072E0 (us-80107dc8) -- refresh the arts name/help text panes.
// The player actor's mode u16 (mField3F28) selects a result code (0 = clear
// text, small codes = fixed ids, 8 probes the item record table for a
// category byte); non-zero results are formatted into the two text panes.
// ---------------------------------------------------------------------------
void CMenuArtsSelect::func_801072E0() {
    char* base = lbl_eu_804FD1E0;

    cf::CfObjectMove* move = cf::CfGameManager::getPlayer(0);
    BattleActor* actor = reinterpret_cast<BattleActor*>(move);
    if (move != NULL) {
        actor = reinterpret_cast<BattleActor*>(reinterpret_cast<char*>(move) - 0x3e9c);
    }

    u32 code = 0;
    if (actor != NULL) {
        u16 mode = actor->mField3F28;
        switch (mode) {
        case 1:
            if (func_800A32BC(func_8009EC9C(1)) == 0) {
                code = 1;
            } else {
                code = 2;
            }
            break;
        case 2:
            code = 0x13;
            break;
        case 3:
            code = 0x24;
            break;
        case 4:
        case 0xc:
            // arg is still the dispatch value here (retail emits no li)
            if (func_800A32BC(func_8009EC9C(mode)) == 0) {
                code = 0x29;
            } else {
                code = 0x98;
            }
            break;
        case 5:
            code = 0x3d;
            break;
        case 6:
            code = 0x4f;
            break;
        case 7:
            code = 0x60;
            break;
        case 8: {
            void* rec = func_80157C4C(8);
            if (rec == NULL || *reinterpret_cast<u32*>(rec) == 0) {
                code = 0x77;
                break;
            }
            u16 cat = func_80139358(*reinterpret_cast<u32*>(rec) >> 20);
            switch (func_801361E8(lbl_eu_806640F8, lbl_eu_804FD1E0 + 0x28e, cat)) {
            case 4:  code = 0x77; break;
            case 5:  code = 0x78; break;
            case 6:  code = 0x79; break;
            case 7:  code = 0x7a; break;
            case 8:  code = 0x7b; break;
            case 9:  code = 0x7c; break;
            case 10: code = 0x7d; break;
            case 11: code = 0x7e; break;
            case 12: code = 0x7f; break;
            default: code = 0x77; break;
            }
            break;
        }
        default:
            code = 0;
            break;
        }
    }

    if (actor != NULL && code != 0) {
        func_8013606C(base + 0x297, base + 0x254, code);
        char* nameStr = func_80136190(base + 0x297, base + 0x254, code);
        char* helpStr = func_80136190(base + 0x297, base + 0x259, code);
        func_80136B4C(unk80, base + 0x69, nameStr, 0);
        func_80136B4C(unk80, base + 0x5c, helpStr,
                      reinterpret_cast<u32>(unk294));
    } else {
        func_80136B4C(unk80, base + 0x69, NULL, 0);
        func_80136B4C(unk80, base + 0x5c, NULL,
                      reinterpret_cast<u32>(unk294));
    }
}

// ---------------------------------------------------------------------------
// func_80107580 (us-80108068) -- set the arts/gauge texture panes.
// Name and gauge ids come either from the rodata per-mode tables (indexed by
// unk328) or, in render-mode 4, from the same switch as func_801072E0 plus two
// table lookups. Each id resolves through func_80138F78 to a texture name.
// ---------------------------------------------------------------------------
void CMenuArtsSelect::func_80107580() {
    char* base = lbl_eu_804FD1E0;

    cf::CfObjectMove* move = cf::CfGameManager::getPlayer(0);
    if (move == NULL) {
        return;
    }

    ArtsModeIds tabA = lbl_eu_804FD138;
    ArtsModeIds tabB = lbl_eu_804FD148;
    u16 nameId = tabA.id[unk328];
    u16 gaugeId = tabB.id[unk328];

    if (unk328 == 4) {
        u16 mode = reinterpret_cast<CfObjectMoveArtsView*>(move)->field_8C;
        u32 code = 0;
        switch (mode) {
        case 1:
            if (func_800A32BC(func_8009EC9C(1)) == 0) {
                code = 1;
            } else {
                code = 2;
            }
            break;
        case 2:
            code = 0x13;
            break;
        case 3:
            code = 0x24;
            break;
        case 4:
        case 0xc:
            if (func_800A32BC(func_8009EC9C(mode)) == 0) {
                code = 0x29;
            } else {
                code = 0x98;
            }
            break;
        case 5:
            code = 0x3d;
            break;
        case 6:
            code = 0x4f;
            break;
        case 7:
            code = 0x60;
            break;
        case 8: {
            void* rec = func_80157C4C(8);
            if (rec == NULL || *reinterpret_cast<u32*>(rec) == 0) {
                code = 0x77;
                break;
            }
            u16 cat = func_80139358(*reinterpret_cast<u32*>(rec) >> 20);
            switch (func_801361E8(lbl_eu_806640F8, lbl_eu_804FD1E0 + 0x28e, cat)) {
            case 4:  code = 0x77; break;
            case 5:  code = 0x78; break;
            case 6:  code = 0x79; break;
            case 7:  code = 0x7a; break;
            case 8:  code = 0x7b; break;
            case 9:  code = 0x7c; break;
            case 10: code = 0x7d; break;
            case 11: code = 0x7e; break;
            case 12: code = 0x7f; break;
            default: code = 0x77; break;
            }
            break;
        }
        default:
            code = 0;
            break;
        }
        nameId = func_8013606C(base + 0x297, base + 0x29f, code);
        gaugeId = func_8013606C(base + 0x297, base + 0x2a9, code);
    }

    if (nameId != 0) {
        void* tex = func_8012FD60(func_80138F78(nameId));
        if (tex != NULL) {
            func_80137E7C(unk80, lbl_eu_804FD1E0 + 0x2ae, tex);
        }
    }
    if (gaugeId != 0) {
        void* tex;
        if (unk328 == 4) {
            tex = func_8012FC74(func_80138F78(gaugeId));
        } else {
            tex = func_8012FD04(func_80138F78(gaugeId));
        }
        if (tex != NULL) {
            func_80137E7C(unk80, lbl_eu_804FD1E0 + 0x2b9, tex);
        }
    }
}
// ---------------------------------------------------------------------------
// func_8010784C (us-80108334) -- arts-select open gate.
// Returns 1 while the menu may stay open: the event-flag probe returns 0, the
// party is not in a battle-commit state, and the current actor's arts/move
// sub-state has not finished animating out. Otherwise falls back to the
// unk334/unk336 latch bytes.
// ---------------------------------------------------------------------------
extern "C" int func_8010784C(CMenuArtsSelect* self) {
    typedef void* (*GetPtrFn)(void*);

    cf::CfGameManager::getInstance();
    if (func_8006EF04__Fi(0x10000000) != 0) return 1;

    cf::CfObjectMove* move = cf::CfGameManager::getPlayer(0);
    BattleActor* actor = reinterpret_cast<BattleActor*>(move);
    if (move != NULL) actor = (BattleActor*)((char*)move - 0x3e9c);
    if (actor != NULL) {
        void* sub = actor->mSecondaryVtable;
        void* pv = reinterpret_cast<ArtsSubVtbl*>(sub)->mFn30();
        u32* pVal = reinterpret_cast<u32*>(pv);
        int localVal = pVal[0];
        if (func_80174C98(actor, &localVal, 0x803) == 0) {
            ArtsSubObj* subObj = static_cast<ArtsSubObj*>(actor->mField3F60);
            if (subObj != NULL && (subObj->mFlag530 & 1)) return 1;
            // CfObjectMove sub-object virtual call: getMax-ish slot 0x4c.
            void* moveObj = &actor->mMoveStart;
            void* ret = reinterpret_cast<ArtsMoveVtbl*>(moveObj)->mFn4C();
            if (ret == NULL) return 1;
            ArtsActionSource* src =
                static_cast<ArtsActionSource*>(func_800B708C((int)ret));
            if (src == NULL) return 1;
            if ((src->mFlags & 0x4) == 0) return 1;
        }
    }
    if (self->unk334 != 0) return 1;
    return self->unk336;
}

int CMenuArtsSelect::func_80107970(s32 index) {
    // Per-slot arts availability gate shared by cbRenderBefore / Move.
    cf::CfObjectMove* move = cf::CfGameManager::getPlayer(0);
    BattleActor* actor = reinterpret_cast<BattleActor*>(move);
    if (move != NULL) {
        actor = reinterpret_cast<BattleActor*>(reinterpret_cast<char*>(move) - 0x3e9c);
    }
    if (actor == NULL) return 0;

    // Sub-object ready flag blocks everything.
    if (actor->mField3F60 != NULL) {
        if (*reinterpret_cast<u32*>(reinterpret_cast<u8*>(actor->mField3F60) + 0x4c0) != 0) {
            return 1;
        }
    }
    // Pending battle-manager action + party-move bit also block.
    if (func_801BA2C8(reinterpret_cast<u8*>(cf::CBattleManager::getInstance()) + 0x216c) != 0 &&
        (actor->mField3388 & 0x2) != 0) {
        return 1;
    }

    if (index < 8) {
        void* mv = reinterpret_cast<ArtsMoveVtbl*>(&actor->mMoveStart)->mFn4C();
        if (mv == NULL) {
            // Party-status probe through the secondary MI vtable.
            u32* pVal = reinterpret_cast<u32*>(
                reinterpret_cast<ArtsSubVtbl*>(actor->mSecondaryVtable)->mFn30());
            int localVal = pVal[0];
            return func_80174C98(actor, &localVal, 0x803);
        }
        u8* arts = static_cast<u8*>(
            reinterpret_cast<ArtsActorVtbl*>(actor)->mFn278());
        ArtsParamInfo* p = reinterpret_cast<ArtsParamInfo*>(getArtsParamAtCnt(arts, index));
        if (p->mCheckFlag == 0) return 0;
        if ((func_80154280(p, actor, 0) & 0x20) == 0) return 0;
        return 1;
    }

    if (unk328 == 2) {
        void* mv = reinterpret_cast<ArtsMoveVtbl*>(&actor->mMoveStart)->mFn4C();
        if (mv == NULL) return 0;
        ArtsActionSource* srcRaw =
            static_cast<ArtsActionSource*>(func_800B708C(index));
        if (srcRaw == NULL) return 0;
        // Squared distance between the action source position and the player;
        // threshold constant depends on srcFlags bit 3.
        nw4r::math::VEC3* srcPos =
            reinterpret_cast<ArtsActionSrcVtbl*>(srcRaw)->mFnAC();
        nw4r::math::VEC3* myPos =
            reinterpret_cast<ArtsMoveVtbl*>(&actor->mMoveStart)->mFnAC();
        nw4r::math::VEC3 d;
        d.x = myPos->x - srcPos->x;
        d.y = myPos->y - srcPos->y;
        d.z = myPos->z - srcPos->z;
        nw4r::math::VEC3 e = d;
        f32 dist = e.x * e.x + e.y * e.y + e.z * e.z;
        f32 limit;
        if (srcRaw->mFlags & 0x8) {
            limit = lbl_eu_80666F60;
        } else {
            limit = lbl_eu_80666F64;
        }
        if (dist > limit) return 1;
        return 0;
    }

    if (unk328 == 4) {
        void* mv = reinterpret_cast<ArtsMoveVtbl*>(&actor->mMoveStart)->mFn4C();
        if (mv == NULL) return 0;
        u8* arts = static_cast<u8*>(
            reinterpret_cast<ArtsActorVtbl*>(actor)->mFn278());
        ArtsParamInfo* rc = reinterpret_cast<ArtsParamInfo*>(getArtsParamRC(arts, 2, 0));
        if (rc->mCheckFlag == 0) return 0;
        if ((func_80154280(rc, actor, 0) & 0x20) == 0) {
            if (actor->mField3F28 == 7 && func_801088CC(this) == 0) return 1;
            return 0;
        }
        return 1;
    }
    return 0;
}
int CMenuArtsSelect::func_80107C54(s32 index) {
    // Per-slot arts usability gate. Bit16 of the global settings flag word
    // blocks everything; each arts mode has its own set of rejection probes
    // (sub-state flags, chain validity, skill availability).
    // Hoisted so MWCC colors it into r31 ahead of `this` (retail prologue).
    ArtsParamInfo* p;
    if (getUnk80664658()->field_214 & 0x10000) return 1;

    if (index < 8) {
        BattleActor* actor = reinterpret_cast<BattleActor*>(cf::CfGameManager::getPlayer(0));
        if (actor != NULL) {
            actor = reinterpret_cast<BattleActor*>(reinterpret_cast<char*>(actor) - 0x3e9c);
        }
        if (actor == NULL) return 0;
        u8* arts = static_cast<u8*>(
            reinterpret_cast<ArtsActorVtbl*>(actor)->mFn278());
        p = reinterpret_cast<ArtsParamInfo*>(getArtsParamAtCnt(arts, index));
        if (p->mCheckFlag == 0) return 1;
        if (func_80154280(p, actor, 0) & 0xd0) return 1;
        if (actor->mField3F60 != NULL) {
            u32 f = *reinterpret_cast<u32*>(reinterpret_cast<u8*>(actor->mField3F60) + 0x4ec);
            if ((f & 0x2) != 0 || (f & 0x100000) != 0) return 1;
        }
        if (unk31C == 1 && func_8015419C(reinterpret_cast<u8*>(p)) != 0) return 1;
        if (CChain_isValidChain(
                reinterpret_cast<u8*>(cf::CBattleManager::getInstance()) + 0x1a8,
                reinterpret_cast<u8*>(actor), 0) != 0) {
            return 1;
        }
        if (func_8027DF38(
                reinterpret_cast<u8*>(cf::CBattleManager::getInstance()) + 0x20c8,
                reinterpret_cast<u8*>(actor), 0, index) != 0) {
            return 1;
        }
        // Talent-skill availability: the move sub-object's status getter, the
        // action source and the skill object's flag bit 23 must all be set.
        void* mv = reinterpret_cast<ArtsMoveVtbl*>(&actor->mMoveStart)->mFn4C();
        if (mv != NULL) {
            void* src = func_800B708C((int)mv);
            if (src != NULL) {
                void* skill = func_8016FE34(src);
                if (skill != NULL) {
                    if (*reinterpret_cast<u32*>(reinterpret_cast<u8*>(skill) + 0x3374) & 0x800000) {
                        return 1;
                    }
                }
            }
        }
        return 0;
    }

    // Talent page (index >= 8): page-mode dependent probes.
    BattleActor* actor = reinterpret_cast<BattleActor*>(cf::CfGameManager::getPlayer(0));
    if (actor != NULL) {
        actor = reinterpret_cast<BattleActor*>(reinterpret_cast<char*>(actor) - 0x3e9c);
    }
    if (actor != NULL) {
        if (unk328 == 4) {
            u8* arts = static_cast<u8*>(
                reinterpret_cast<ArtsActorVtbl*>(actor)->mFn278());
            ArtsParamInfo* rc = reinterpret_cast<ArtsParamInfo*>(getArtsParamRC(arts, 2, 0));
            if (rc->mCheckFlag == 0) return 1;
            if (func_80154280(rc, actor, 0) & 0xc0) return 1;
        }
        if (actor->mField3F60 != NULL) {
            u32 f = *reinterpret_cast<u32*>(reinterpret_cast<u8*>(actor->mField3F60) + 0x4ec);
            if ((f & 0x2) != 0 || (f & 0x100000) != 0) return 1;
        }
        if (CChain_isValidChain(
                reinterpret_cast<u8*>(cf::CBattleManager::getInstance()) + 0x1a8,
                reinterpret_cast<u8*>(actor), 1) != 0) {
            return 1;
        }
        u8* chainWin = reinterpret_cast<u8*>(cf::CBattleManager::getInstance()) + 0x20c8;
        if (func_8027DF38(chainWin, reinterpret_cast<u8*>(actor), 1, 0) != 0) return 1;

        if (unk328 == 1) {
            if (*reinterpret_cast<s16*>(chainWin) != 0) return 1;
            if (func_80148778(&actor->mArtsList, 0x9) != 0) return 1;
            if (func_80148778(&actor->mArtsList, 0xf) != 0) return 1;
            if (func_80148778(&actor->mArtsList, 0x10) != 0) return 1;
            if (func_80148778(&actor->mArtsList, 0xb) != 0) return 1;
            if (func_80148778(&actor->mArtsList, 0xeb) != 0) return 1;
            if (func_80148778(&actor->mArtsList, 0xf8) != 0) return 1;
            if (func_80148778(&actor->mArtsList, 0xef) != 0) return 1;
            if (func_80148778(&actor->mArtsList, 0xf0) != 0) return 1;
            if (func_80148778(&actor->mArtsList, 0xf1) != 0) return 1;
        } else {
            void* mv = reinterpret_cast<ArtsMoveVtbl*>(&actor->mMoveStart)->mFn4C();
            if (mv != NULL) {
                void* src = func_800B708C((int)mv);
                if (src != NULL) {
                    void* skill = func_8016FE34(src);
                    if (skill != NULL) {
                        if (*reinterpret_cast<u32*>(reinterpret_cast<u8*>(skill) + 0x3374) & 0x800000) {
                            return 1;
                        }
                    }
                }
            }
            if (unk328 == 2) {
                if (func_8009CF8C(0x3359) == 0) return 1;
            } else if (unk328 == 3) {
                if (func_8009CF8C(0x3357) == 0) return 1;
                u32* pVal = reinterpret_cast<u32*>(
                    reinterpret_cast<ArtsSubVtbl*>(actor->mSecondaryVtable)->mFn30());
                int localVal = pVal[0];
                if (func_80174C98(actor, &localVal, 0xb) != 0) return 1;
            }
        }
    }

    if (unk328 == 3) {
        // Retail evaluates the chain pointer before re-fetching the manager
        // for the 0x194 counter compare.
        u8* chain = reinterpret_cast<u8*>(cf::CBattleManager::getInstance()) + 0x1a8;
        cf::CBattleManager* bm = cf::CBattleManager::getInstance();
        if (*reinterpret_cast<s32*>(reinterpret_cast<u8*>(bm) + 0x194) != 0x12c ||
            func_802795D4(chain, 0) == 0) {
            return 1;
        }
    }
    return 0;
}
void CMenuArtsSelect::func_801080F8() {
    // Arts-select per-frame availability sweep. For each of the 8 regular art
    // slots: when the menu is open (unk298 >= 2) and the player's arts list
    // matches (art id in [3,10] for the first adopt, then any slot), adopt the
    // slot (set unk340, refresh name/help, rewind the anims) or the talent
    // page (unk324 = 4). The loop tail syncs the three per-slot panes' flags
    // from the art's stock count (u16 at +0x72) and latches unk308 bit 0x20.
    unk308 &= ~0x20;

    BattleActor* actor = reinterpret_cast<BattleActor*>(
        func_8016FE34(cf::CfGameManager::getPlayer(0)));
    u8* arts = static_cast<u8*>(reinterpret_cast<ArtsActorVtbl*>(actor)->mFn278());

    f32 zeroF = lbl_eu_80666F28;
    s16* posX = reinterpret_cast<s16*>(lbl_eu_804FD0D0 + 0x00);
    s16* posY = reinterpret_cast<s16*>(lbl_eu_804FD0D0 + 0x14);
    f32* scale = reinterpret_cast<f32*>(lbl_eu_804FD0D0 + 0x28);
    s16* selTab = reinterpret_cast<s16*>(lbl_eu_804FD0D0 + 0x4c);

    for (u8 i = 0; i < 8; i++) {
        ArtsParamInfo* param = reinterpret_cast<ArtsParamInfo*>(
            getArtsParamAtCnt(arts, i));
        u32 artId = param->mField28;
        if (unk298 >= 2 && actor->mField3F28 == 1 && artId != 0) {
            if (unk340 == 0) {
                if (artId >= 3 && artId <= 10) {
                    unk340 = static_cast<u16>(artId);
                    if (unk324 == 4) {
                        if (unk328 == 4) {
                            func_801072E0();
                        } else {
                            s16 v = selTab[unk328];
                            char* name = func_80136190(lbl_eu_804FD1E0 + 0x249,
                                                        lbl_eu_804FD1E0 + 0x254, v);
                            char* help = func_80136190(lbl_eu_804FD1E0 + 0x249,
                                                        lbl_eu_804FD1E0 + 0x259, v);
                            func_80136B4C(unk80, lbl_eu_804FD1E0 + 0x69, name, 0);
                            func_80136B4C(unk80, lbl_eu_804FD1E0 + 0x5c, help,
                                          reinterpret_cast<u32>(unk294));
                        }
                    } else {
                        cf::CfObjectMove* pl = cf::CfGameManager::getPlayer(0);
                        BattleActor* a2 = reinterpret_cast<BattleActor*>(pl);
                        if (pl != NULL) {
                            a2 = reinterpret_cast<BattleActor*>(reinterpret_cast<char*>(pl) - 0x3e9c);
                        }
                        if (a2 != NULL) {
                            u8* arts2 = static_cast<u8*>(
                                reinterpret_cast<ArtsActorVtbl*>(a2)->mFn278());
                            s32 q = unk324;
                            if (q > 4) q--;
                            ArtsParamInfo* p2 = reinterpret_cast<ArtsParamInfo*>(
                                getArtsParamAtCnt(arts2, q));
                            char* name = static_cast<char*>(p2->mNamePtr);
                            name = (name != NULL) ? name : lbl_eu_80661E08;
                            func_80136B4C(unk80, lbl_eu_804FD1E0 + 0x69,
                                          reinterpret_cast<char*>(p2), 0);
                            func_80136B4C(unk80, lbl_eu_804FD1E0 + 0x5c, name,
                                          reinterpret_cast<u32>(unk294));
                        }
                    }
                    if (func_80110A70() != NULL) {
                        func_8010EDE4(reinterpret_cast<u8*>(func_80110A70()) + 0x7e4);
                    }
                    if (func_8010CE48() != NULL) {
                        func_8010A8E4(reinterpret_cast<u8*>(func_8010CE48()) + 0x7cc);
                    }
                    reinterpret_cast<UnkArtsSelectEntry*>(unk8C->GetRootPane())->unkBB =
                        static_cast<u8>((reinterpret_cast<UnkArtsSelectEntry*>(unk8C->GetRootPane())->unkBB & 0xFEu) | 1);
                    unk298 = 2;
                    unk8C->SetAnimationEnable(unk90, false);
                    unk8C->SetAnimationEnable(unk94, true);
                    unk94->SetFrame(zeroF);
                    unk8C->Animate(0);
                }
            } else if (artId < 3 || artId > 10) {
                if (func_80110A70() != NULL) {
                    func_8010EDE4(reinterpret_cast<u8*>(func_80110A70()) + 0x7e4);
                }
                if (func_8010CE48() != NULL) {
                    func_8010A8E4(reinterpret_cast<u8*>(func_8010CE48()) + 0x7cc);
                }
                reinterpret_cast<UnkArtsSelectEntry*>(unk8C->GetRootPane())->unkBB =
                    static_cast<u8>((reinterpret_cast<UnkArtsSelectEntry*>(unk8C->GetRootPane())->unkBB & 0xFEu) | 1);
                unk8C->SetAnimationEnable(unk90, false);
                unk8C->SetAnimationEnable(unk94, true);
                unk94->SetFrame(zeroF);
                unk8C->Animate(0);
                unk340 = 0;
                unk324 = 4;
                nw4r::lyt::Pane* pane =
                    unk8C->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0xc7, true);
                {
                    nw4r::math::VEC3 trans = pane->GetTranslate();
                    trans.x = static_cast<f32>(posX[4]);
                    trans.y = static_cast<f32>(posY[4]);
                    pane->SetTranslate(trans);
                }
                {
                    f32 s = scale[4];
                    pane->SetScale(nw4r::math::VEC2(s, s));
                }
                if (unk328 == 4) {
                    func_801072E0();
                } else {
                    s16 v = selTab[unk328];
                    char* name = func_80136190(lbl_eu_804FD1E0 + 0x249,
                                                lbl_eu_804FD1E0 + 0x254, v);
                    char* help = func_80136190(lbl_eu_804FD1E0 + 0x249,
                                                lbl_eu_804FD1E0 + 0x259, v);
                    func_80136B4C(unk80, lbl_eu_804FD1E0 + 0x69, name, 0);
                    func_80136B4C(unk80, lbl_eu_804FD1E0 + 0x5c, help,
                                  reinterpret_cast<u32>(unk294));
                }
                reinterpret_cast<UnkArtsSelectEntry*>(
                    unk8C->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0x25e, true))->unkBB =
                    static_cast<u8>((reinterpret_cast<UnkArtsSelectEntry*>(
                                         unk8C->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0x25e, true))->unkBB & 0xFEu) | 1);
                reinterpret_cast<UnkArtsSelectEntry*>(
                    unk8C->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0x26c, true))->unkBB =
                    static_cast<u8>((reinterpret_cast<UnkArtsSelectEntry*>(
                                         unk8C->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0x26c, true))->unkBB & 0xFEu) | 1);
            }
        }

        // Loop tail: sync the three slot panes' flag bytes with the stock
        // count; then latch/clear unk308 bit (i+8) and swap in the per-slot
        // textures from the arts-elem / arts-pc arcs.
        u32 t = (u32)-(s32)param->mField72 | param->mField72;
        u32 b = t >> 31;
        UnkArtsSelectEntry* e1 = unk200[i];
        e1->unkBB = static_cast<u8>((e1->unkBB & 0xFEu) | ((t >> 30) & 1));
        UnkArtsSelectEntry* e2 = unk224[i];
        e2->unkBB = static_cast<u8>(b | ((e2->unkBB & 0x7Fu) << 24));
        UnkArtsSelectEntry* e3 = unk248[i];
        e3->unkBB = static_cast<u8>(b | ((e3->unkBB & 0x7Fu) << 24));
        u32 mask = 1u << (i + 8);
        if (b != 0) {
            if (!(unk308 & mask)) {
                unk308 = (unk308 | mask) | 0x20;
            }
            char* tex = func_80138F78(param->mCheckFlag);
            u8* res = func_8012FD60(tex);
            if (res != NULL) {
                func_80137F88(reinterpret_cast<nw4r::lyt::Pane*>(unk200[i]), res);
            }
            tex = func_80138F78(param->mField72);
            res = func_8012FC74(tex);
            if (res != NULL) {
                func_80137F88(reinterpret_cast<nw4r::lyt::Pane*>(unk224[i]), res);
            }
        } else {
            if (unk308 & mask) {
                unk308 = (unk308 & ~mask) | 0x20;
            }
        }
    }
}
// ---------------------------------------------------------------------------
// func_801086D0 (us-801091b8) -- arts-use gate for non-talent slots.
// Rejects the talent slot (unk324==4), the talent art id (0xeb + id 0x16),
// and any art while the battle/commit conditions are mid-flight (battle mode
// 1..24, party gauge drained, or a pending skill action); then requires the
// art's gauge ratio (current/max) to be positive.
// ---------------------------------------------------------------------------
extern "C" int func_801086D0(CMenuArtsSelect* self) {
    s32 idx = self->unk324;
    if (idx == 4) return 0;
    if (idx >= 4) idx--;  // retail: cmpwi; blt skip; subi (== 4 unreachable)

    // Self-adjust: the null test reads the pre-adjust value (retail:
    // mr r30, r3; beq; subi r30, r3, 0x3e9c) - no separate `move` local.
    BattleActor* actor =
        reinterpret_cast<BattleActor*>(cf::CfGameManager::getPlayer(0));
    if (actor != NULL) actor = (BattleActor*)((char*)actor - 0x3e9c);
    // Retail: actor==NULL branches to the shared return-0 block at the end
    // (.L_80109394), so the whole body is wrapped in `if (actor != NULL)`.
    if (actor != NULL) {
        ArtsParamInfo* p = reinterpret_cast<ArtsParamInfo*>(
            getArtsParamAtCnt(reinterpret_cast<ArtsActorVtbl*>(actor)->mFn278(),
                              idx));
        if (func_80148778(&actor->mArtsList, 0xeb) != 0 && p->mField28 == 0x16) {
            return 0;
        }

        // First instance is kept (r31) for the final commit check; the retail
        // re-fetches getInstance() for the mode/flag probes.
        ArtsBmWindow* bm =
            reinterpret_cast<ArtsBmWindow*>(cf::CBattleManager::getInstance());
        if (p->mField48 != 0 && actor->mField1530 != 0) {
            s32 battle;
            if (reinterpret_cast<ArtsBmWindow*>(
                    cf::CBattleManager::getInstance())->mField20C8 != 0) {
                battle = 1;
            } else {
                u8 mode = reinterpret_cast<ArtsBmWindow*>(
                    cf::CBattleManager::getInstance())->mField1AA;
                s32 inRange = 0;
                if (mode >= 1) {
                    if (mode <= 24) inRange = 1;
                }
                if (inRange) {
                    battle = 1;
                } else {
                    cf::CfObjectMove* pl = cf::CfGameManager::getPlayer(0);
                    BattleActor* a2 = reinterpret_cast<BattleActor*>(pl);
                    if (pl != NULL) a2 = (BattleActor*)((char*)pl - 0x3e9c);
                    // Flat: one shared battle=0 else (retail .L_80109300).
                    if (a2 != NULL &&
                        reinterpret_cast<ArtsActorVtbl*>(a2)->mFn128() <=
                            lbl_eu_80666F28) {
                        battle = 1;
                    } else {
                        battle = 0;
                    }
                }
            }
            if (!battle && bm->mField20C8 == 0 && p->mField28 != 0x53 &&
                p->mField48 == actor->mField1530) {
                return 0;
            }
        }

        // Gauge ratio: getMax is virtual on the table; retail calls it twice
        // and divides current (mRatioNum) by max when non-zero, else -1.0f.
        // Compare against the named symbols, not literals, so MWCC emits
        // lbl_eu_80666F28/44 relocs (literals pool into TU-local @N labels).
        // The division arm is the fall-through; the -1.0f fallback is the
        // branch target placed after it (retail .L_8010937C layout).
        f32 max = reinterpret_cast<ArtsTableVtbl*>(p->mTablePtr)->mFn14();
        f32 ratio;
        if (max != lbl_eu_80666F28) {
            max = reinterpret_cast<ArtsTableVtbl*>(p->mTablePtr)->mFn14();
            ratio = p->mRatioNum / max;
        } else {
            ratio = lbl_eu_80666F44;
        }
        if (ratio > lbl_eu_80666F28) return 1;
    }
    return 0;
}

// ---------------------------------------------------------------------------
// func_801088CC (us-801093b4) -- use/confirm gate for the talent-art slot.
// Allowed only when both the main FSM (unk324) and the talent page (unk328)
// select slot 4, the player's art-id is 7, and its sub-object state passes.
// ---------------------------------------------------------------------------
extern "C" int func_801088CC(CMenuArtsSelect* self) {
    // Retail shares one return-1 block for the two opening guards (bne into
    // it, beq over it) and one return-0 block (.L_80109460) for the NULL and
    // sub-state paths; the sub==NULL and !=7 returns stay inline.
    if (self->unk324 != 4) goto ret1;
    if (self->unk328 == 4) goto body;
ret1:
    return 1;
body:
    BattleActor* actor =
        func_800BFC68__FPQ22cf12CfObjectMove(cf::CfGameManager::getPlayer(0));
    if (actor == NULL) goto ret0;
    if ((u8)actor->mField3F28 != 7) return 1;
    void* sub = actor->mField3F60;
    if (sub == NULL) return 0;
    int v = func_801B2084();
    if (func_80060290(sub) != 0) {
        if (v <= 1) goto ret0;
        return 1;
    } else {
        if (v <= 0) goto ret0;
        return 1;
    }
ret0:
    return 0;
}
// ---------------------------------------------------------------------------
// func_80108994 (us-8010947c) -- bind per-slot panes.
// The 9 slot triples (unk200/unk224/unk248) come from printf-formatted pane
// names (formats at +0x2c4/+0x2d1/+0x2de); the 7 visibility-flag group panes
// (unk26C) use the fixed names at +0x2ea..+0x322 and the two text panes.
// ---------------------------------------------------------------------------
void CMenuArtsSelect::func_80108994() {
    char buf[0x28];
    for (s32 i = 0; i < 9; i++) {
        sprintf(buf, lbl_eu_804FD1E0 + 0x2c4, i);
        unk200[i] = reinterpret_cast<UnkArtsSelectEntry*>(
            unk80->GetRootPane()->FindPaneByName(buf, true));
        sprintf(buf, lbl_eu_804FD1E0 + 0x2d1, i);
        unk224[i] = reinterpret_cast<UnkArtsSelectEntry*>(
            unk80->GetRootPane()->FindPaneByName(buf, true));
        sprintf(buf, lbl_eu_804FD1E0 + 0x2de, i);
        unk248[i] = reinterpret_cast<UnkArtsSelectEntry*>(
            unk80->GetRootPane()->FindPaneByName(buf, true));
    }
    unk26C[0] = reinterpret_cast<UnkArtsSelectEntry*>(
        unk80->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0x2ea, true));
    unk26C[1] = reinterpret_cast<UnkArtsSelectEntry*>(
        unk80->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0x2f8, true));
    unk26C[2] = reinterpret_cast<UnkArtsSelectEntry*>(
        unk80->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0x306, true));
    unk26C[3] = reinterpret_cast<UnkArtsSelectEntry*>(
        unk80->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0x314, true));
    unk26C[4] = reinterpret_cast<UnkArtsSelectEntry*>(
        unk80->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0x322, true));
    unk26C[5] = reinterpret_cast<UnkArtsSelectEntry*>(
        unk80->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0x69, true));
    unk26C[6] = reinterpret_cast<UnkArtsSelectEntry*>(
        unk80->GetRootPane()->FindPaneByName(lbl_eu_804FD1E0 + 0x5c, true));
}


extern "C" void func_801041F4() {}
extern "C" void func_801042F0() {}




