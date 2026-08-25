// Auto-scaffolded catalog TU for kyoshin/makecrystal/CModelDispMakeCrystal
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>

#include "kyoshin/CTaskGameApi.hpp"
#include "kyoshin/makecrystal/CModelDispMakeCrystal.hpp"


#include "kyoshin/makecrystal/CMCEffStart.hpp"
#include "kyoshin/makecrystal/CMCCylinderGauge.hpp"
#include "kyoshin/makecrystal/CMCCrystalList.hpp"
// CModelDispMakeCrystal.hpp declares CItem_initItemImplInstances(void*)
// (retail passes the item pointer), while CfGameManager.hpp declares a
// zero-arg form - hide the conflicting decl for this include. RESIDUAL:
// the symbol genuinely has two call shapes in retail (pointer-arg here,
// no-arg in CfMapMineManager/pluginCfs); unifying them would change r3
// setup at the no-arg sites, so both decls stay.
#define CItem_initItemImplInstances makeCrystalCItemInitItemImplInstancesUnused
// code_80135FDC.hpp declares func_8049603C as CTaskGameCamView*, while
// CfGameManager.hpp declares it as UnkScnResult* - rename one away here
// (this TU never calls it).
#define func_8049603C makeCrystalCode35FDC9603CUnused
#include "kyoshin/code_80135FDC.hpp"
#undef func_8049603C
// code_80135FDC.hpp:254 declares lbl_eu_8066A208 as extern u32 while
// monolib/math/FloatUtils.hpp (via CDeviceVI.hpp) declares it const float -
// MWCC 10563. This TU never references the symbol; rename the later copies.
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/core/CPadManager.hpp"
#define CItem_initItemImplInstances makeCrystalCItemInitItemImplInstancesUnused
// CfGameManager.hpp:710 re-declares getInstance__Q22cf14CBattleManagerFv
// with a CBattleManagerView* return while CBattleManagerApi.hpp (included at
// its top) already carries the shared extern "C" void* copy - MWCC 10505.
#define getInstance__Q22cf14CBattleManagerFv makeCrystalBmGetInstanceUnused
#include "kyoshin/cf/CfGameManager.hpp"
#undef getInstance__Q22cf14CBattleManagerFv
#undef CItem_initItemImplInstances
#include "kyoshin/cf/CfPadData.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/math/MTRand.hpp"
#include "monolib/work/CEventFile.hpp"

// forward declarations for scaffold thunk references
void __dt__21CModelDispMakeCrystalFv(void*);
void func_80220E14(CModelDispMakeCrystal*, CMCrystalDispSub*);
void func_802211CC(CModelDispMakeCrystal*, u8*);
// state-machine helpers defined later in this TU
void func_8021CC60(CModelDispMakeCrystal*);
void func_8021CD8C(CModelDispMakeCrystal*);
void func_80220C34(CModelDispMakeCrystal*);
// crystal-slot array element ctor/dtor (defined later in this TU; the ctor
// passes their addresses to __construct_array). C linkage keeps the
// __construct_array relocs bound to the bare retail names.
extern "C" CModelDispMakeCrystal* func_8021C4F0(CModelDispMakeCrystal* self);
extern "C" void* __dt__8021C540(void* obj, int flag);

// --- external C-linkage helpers and global data for this TU ---
extern "C" {
void* func_8004B60C(void*, f32, f32, f32);
float scaleByGlobal(float val);
void initCrystalData(unsigned char* p);
int CSysWin_isActive(void* self);
int CSysWin_isReady(void* self);
int CScrollBar_isVisible(void* self);
int func_80297CC0(void* self);
int func_80222A50(void* self);
void func_80222AF0(void* self);
void func_801F369C(void* self);
void func_80222964(void*, void*);
void func_802228B8(void* self);
void func_8022E988(void* self);
void func_8022EA04(void* self);
void func_8022E9E4(void*, void*);
void func_801D20B0(void*, void*);
void func_801F35B0(void*, void*);
void func_80297AAC(void*, void*);
void func_8022B7C8(void*, void*);
void func_802979E4(void* self);
void func_801D202C(void* self);
void func_801F3540(void* self);
void func_801F35DC(void* self);
void func_8022B748(void* self);
void func_801D216C(void*, u8);
void func_8022C1B4(void* out, void* csyswin, u8);
void func_80297B68(void*);
void func_80297E18(void*);
void func_8022B7F4(void*);
int func_80222A58(void*);
void func_8004CF00(void*);
void func_8021FEDC(void*);
void func_80220C34(void*);
int func_80086F9C__Q22cf13CfGameManagerFv(int arg);
void func_8004B6BC(void*, void*);
void func_80495E60(void*);
void* func_80495E8C(void*, int, int, ...);
void func_80485684(void*, int);
void func_80482DF4(void*, int);
void* func_8048315C(void*);
void func_804831C4(void*, void*);
void func_804E3CCC(void*);
void func_804E3D0C(void*, void*);
void func_804CC1D8(void*, void*);
void* func_804CC1F4(void*, void*, void*, int, int, int);
// Retail member-return types are int (callers `cmpwi r3,0` the raw reg).
// Mangled-name refs let us call them as int-returning without re-mangling.
int isFinished__11CMCEffStartFv(void* self);
int isFinished__13CMCEffCrystalFv(void* self);
int isFinished__14CMCEffCylinderFv(void*);
int func_802220F0__16CMCCylinderGaugeFv(void*);
void func_802203D8(void*);
void func_8022077C(void*);
void func_8022EA88(void*, u8);
void func_8022EA64(void*);
void func_8013B428__FUl(u32);
void func_802A1500(void);
void func_80189C88(void);
void func_80043C88(void);
void func_80133E58(u32, u32, u32);
void func_80138078__FUl(u32);
void func_8045F778__17UnkClass_8045F564Fv(void* self);
// --- target callees (retail C-linkage / mangled-name symbols) ---
void func_804E3D48(void*, void*);
int CSysWin_getUnk34(void*);
int func_80298850(void*);
void func_8004B9D4(void*, int, int, int, int);
void func_80220954(void*, int, u8);
void func_80297D2C(void*, int, void*, u8);
void func_80222F64(void*, void*, u8);
void func_80223334(void*);
void func_80222848(void*);
void func_8022B90C(void*, int);
void func_8022B9B4(void*, void*, int);
void func_8022BF6C(void*, void*, void*);
void func_8022BFC8(void*, int);
void func_8022B8B8(void*);
void func_8022B8E4(void*);
void func_801F34F4(void*);
void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
u32 getHandleMEM2__Q23mtl10MemManagerFv();
int isFinished__FPv(void*);
int func_80297D1C(void*);
int func_80297D24(void*);
void func_80297E18(void*);
void func_80297E90(void*);
void func_80298450(void*);
void func_802984E4(void*);
int func_80298540(void*);
void func_802985B4(void*);
void func_80298228(void*);
void func_802980DC(void*);
void func_80297FB4(void*);
void func_8029860C(void*, int);
void func_80298614(void*);
char lbl_eu_805090FC[];
int sprintf(char*, const char*, ...);
void func_80043D90(CMCryListHolder*);
void* func_80043F18(CMCryListHolder*); // returns holder->list
void __dt__80043E88(CMCryListHolder*, int);
void func_800F4A98(void*, unsigned int, unsigned int);
void* func_800F6EC0(void*, unsigned int); // &slot -> +0x4 holds the move ptr
void* func_800BFC68__FPQ22cf12CfObjectMove(void*);
void* func_80062C28(short, int);
int func_80062A00();
void* func_80062AD8(u32, void*);
short func_800BE954(void*);
void func_8009ECB0();
u32 func_80141E90(u32, s16, u32, u32);
int func_800AA33C(void*, u32, int, int);
void func_8007E038__Q22cf13CfGameManagerFv(u32, bool);
int func_8003B1EC(void*);
void func_80159F6C(void*, u16, u16, u8);
void func_802232E4(void*);
// OnFileEvent constructor/helper imports (retail reloc names)
void __ct__16CMCCylinderGaugeFPQ34nw4r3lyt19ArcResourceAccessor(void*, nw4r::lyt::ArcResourceAccessor*);
void __ct__CMCCrystalSupport(void*, nw4r::lyt::ArcResourceAccessor*);
void __ct__CMCEffStart(void*, nw4r::lyt::ArcResourceAccessor*);
void __ct__CMCEffUpRed(void*, nw4r::lyt::ArcResourceAccessor*);
void __ct__CMCEffUpBlue(void*, nw4r::lyt::ArcResourceAccessor*);
void __ct__CMCEffUpGreen(void*, nw4r::lyt::ArcResourceAccessor*);
void __ct__CMCEffUpPrm(void*, nw4r::lyt::ArcResourceAccessor*);
void __ct__CMCEffSuccess(void*, nw4r::lyt::ArcResourceAccessor*);
void __ct__CMCEffFailure(void*, nw4r::lyt::ArcResourceAccessor*);
void __ct__CMCEffCrystal(void*, nw4r::lyt::ArcResourceAccessor*);
void __ct__CMCEffUpRank(void*, nw4r::lyt::ArcResourceAccessor*);
void __ct__CMCEffDivide(void*, nw4r::lyt::ArcResourceAccessor*);
void __ct__CMCEffCylinder(void*, nw4r::lyt::ArcResourceAccessor*);
void __ct__6CCur18Fv(void*, void*);
void createRegion__17UnkClass_8045F564FiiPCci(void*, u32, int, const char*, int);
void __ct__14Class_8045F858FP17UnkClass_8045F564(void*, void*);
void func_80434A4C__Q23mtl10MemManagerFb(bool);
void* createArcResourceAccessor__10CLibLayoutFv();
void Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(void*, void*, const char*);
void func_80221EF4__16CMCCylinderGaugeFv(void*);
void func_8022E8F8(void*);
void func_80211CB8(void*, void*);
void init__11CMCEffUpPrmFv(void*);
void* func_801355F4();
void func_8018B0FC(void*, void*);
void func_8045F810__17UnkClass_8045F564Fv(void*);
void __dt__14Class_8045F858Fv(void*, int);
void func_804CC1BC(void*);
void* func_800584B8(void*, unsigned int, const char*);
void func_8004B624(void*, void*, void*, unsigned int);
void func_80200388(void*, void*);
void func_8021E8E4(void*);
void func_80213E20(void*, void*);
void* func_802165E8(void*, const void*); // param copy helper (returns dst)
void func_80222D9C(void*, u8);
void __dt__80222984(void*);
void func_80222B14(void*, u8, u16, u16);
void func_80223004(void*, u8, u16, u8);
void func_80222A60(void*);
void func_80222A84(void*);
void func_80222AA8(void*);
void func_80222ACC(void*);
// CMCEffUpPrm::setNumber called with a u16 second arg: retail passes the raw
// halfword as the u8 param (callee reads only the low byte), so declaring the
// param as u16 here avoids MWCC's truncation mask at the call site.
void setNumber__11CMCEffUpPrmFUlUc(CMCEffUpPrm*, u32, u16);
void func_80297928(void*);
void* func_80496264(void*, int);
void func_8049EFF8(void*, void*, void*);
void func_801F3670(void*, void*);
void func_801F36BC(void*, u8, u8);
void func_801F367C(void*);
s32 getInstance__Q22ml6MTRandFv();
s32 rand31__Q22ml6MTRandFv();
u16 func_8013A7D0(u8, u8);
void* func_8009EC9C(u32);
int func_8026178C(void*, int);
u8 func_8025FB10(void*, int);
void func_80220128(void*);
// func_8009D018 comes from CfGameManager.hpp (u32,u32) - no local decl.
void __destroy_arr(void*, void*, int, int);
void __dt__13CMCGetItemBoxFv(void*, int);
void __dt__6CCur18Fv(void*, int);
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
void __dl__FPv(void*);
}
extern const f32 lbl_eu_806684D4;
extern const f32 lbl_eu_806684A0;
extern const f32 lbl_eu_806684A4;
extern const f32 lbl_eu_806684DC;
extern const f32 lbl_eu_806684D8;
extern const f32 lbl_eu_80668508;
extern const f32 lbl_eu_806684AC;
extern const f32 lbl_eu_8066850C;
extern const f32 lbl_eu_80668510;
extern const f32 lbl_eu_806684CC;
extern const f32 lbl_eu_806684D0;
extern const f64 lbl_eu_806684F0;
extern const u16 lbl_eu_806684E0;
extern const f32 lbl_eu_806684EC;
extern const f64 lbl_eu_806684F0;
extern const f32 lbl_eu_806684F8;
extern const f32 lbl_eu_806684FC;
extern const f32 lbl_eu_80668500;
extern const f32 lbl_eu_80668504;
extern const f32 lbl_eu_806684C4;
extern const f32 lbl_eu_806684C8;
extern void* lbl_eu_806640EC;   // sdata: character/enum table pointer (func_802203D8)
extern void* lbl_eu_806640D8;   // sdata: item-source pointer (func_802203D8)
extern const u8 lbl_eu_806684E2;
extern const u32 lbl_eu_806684E4;
extern const u16 lbl_eu_806684E8;
extern u8 lbl_eu_80576658[0x20];
extern u8 lbl_eu_80576664[0x20];
extern u32 lbl_eu_8065FC18[];

// Virtual-call helper: represents the UI sub-object at this+0xEB4 whose
// vtable has methods at slots 3 (offset 0x0c, no-arg) and 4 (offset 0x10,
// void* arg).
struct CMCWinFn {
    virtual void m0();
    virtual void m1();
    virtual void m2(void* arg);
    virtual void m3();
    virtual void m4();
};

// Virtual dispatch to a sub-object whose vtable method sits at slot +0x10
// (declared index 2, MWCC adds 2 hidden RTTI slots) taking one argument
// (used on this+0xe20 in func_8021DF84).
struct CMCE20Fn {
    virtual void m0();
    virtual void m1();
    virtual void m2(const void* arg);
};

// Same slot-4 virtual dispatch but no-arg (used on this+0xc8c in func_8021CE4C).
// MWCC reserves 2 hidden vtable slots (RTTI), so declared index 2 => +0x10.
struct CMCC8CFn {
    virtual void m0();
    virtual void m1();
    virtual void m2();
};

// Virtual dispatch at vtable offset +0x88 (index 34): the CSysWin-dispatch
// call in func_8021C6E4 on this+0xe78. MWCC reserves 2 hidden vtable slots
// (RTTI), so the declared list is 2 shorter than the real method count.
struct CMCSysWinFn {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m0A(); virtual void m0B();
    virtual void m0C(); virtual void m0D(); virtual void m0E(); virtual void m0F();
    virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13();
    virtual void m14(); virtual void m15(); virtual void m16(); virtual void m17();
    virtual void m18(); virtual void m19(); virtual void m1A(); virtual void m1B();
    virtual void m1C(); virtual void m1D(); virtual void m1E(); virtual void m1F();
    virtual void m20();  // declared index 32 => slot +0x88
};

// Entry at this+0x13c0 (stride 0x34); a 4-bit type at bits 12-15.
struct CMCStep {
    u32 _pad0 : 12;
    u32 type : 4;
    u32 _pad16 : 16;
};

// Virtual dispatch at vtable offset +0xE0 (index 0x38): the flush call in
// func_8021FB68 on the inline object at the sub-struct +0x8.
// NOTE: MWCC adds 2 hidden vtable slots (RTTI), so the declared method list
// must be 2 shorter than the real vtable to index the retail slot.
struct CMCVtE0 {
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
    virtual void m34(); virtual void m35(); virtual void m36();  // index 0x38 => +0xE0
};

void __ct__CModelDispMakeCrystal(CModelDispMakeCrystal* self, CScn* scene)
{
    u8* base = reinterpret_cast<u8*>(self);

    // Stack buffers (declaration order fixes the retail stack layout:
    // sub@0x8, group2@0x18, group1@0x30, swTmp@0x48, sbTmp@0x88,
    // listTmp@0xc8, boxTmp@0x120).
    u8 sub[0xc];       // initCrystalSubStruct scratch
    CMCryVec3 group2[2];
    CMCryVec3 group1[2];
    u8 swTmp[0x40];    // temp CSysWin
    u8 sbTmp[0x40];    // temp CScrollBar
    u8 listTmp[0x58];  // temp CMCCrystalList
    u8 boxTmp[0x4ec];  // temp CMCGetItemBox

    // --- vtable pointers, scene, zeroed scalar head ---
    *reinterpret_cast<void**>(base + 0x00) = lbl_eu_80535E70;
    *reinterpret_cast<void**>(base + 0x04) = lbl_eu_80535E70 + 0x88;
    *reinterpret_cast<void**>(base + 0x08) = lbl_eu_80535E70 + 0xb4;
    *reinterpret_cast<void**>(base + 0x0c) = scene;
    *reinterpret_cast<u32*>(base + 0x10) = 0;
    *reinterpret_cast<u32*>(base + 0x14) = 0;
    *reinterpret_cast<u32*>(base + 0x18) = 0;
    *reinterpret_cast<u32*>(base + 0x1c) = 0;
    *reinterpret_cast<u32*>(base + 0x20) = 0;
    *reinterpret_cast<u32*>(base + 0x24) = 0;
    *reinterpret_cast<u32*>(base + 0x28) = 0;
    __ct__17UnkClass_8045F564Fv(base + 0x2c);
    *reinterpret_cast<u32*>(base + 0x3c) = 0;
    *reinterpret_cast<u32*>(base + 0x40) = 0;
    __construct_array(base + 0x44, (void*)func_8021C4F0, (void*)__dt__8021C540, 0x5cc, 2);

    // --- gauge fields + embedded member construction ---
    base[0xbdc] = 0;
    base[0xbdd] = 0;
    *reinterpret_cast<f32*>(base + 0xbe0) = lbl_eu_806684A0;
    *reinterpret_cast<f32*>(base + 0xbe4) = lbl_eu_806684A4;
    base[0xbe8] = 1;
    base[0xbe9] = 1;
    __ct__16CMCCylinderGaugeFPQ34nw4r3lyt19ArcResourceAccessor(base + 0xbec, 0);
    __ct__CMCCrystalList(base + 0xc18);
    __ct__CMCCrystalSupport(base + 0xc70, 0);
    __ct__CMCEffStart(base + 0xc8c, 0);
    __ct__CMCEffUpRed(base + 0xca4, 0);
    __ct__CMCEffUpBlue(base + 0xcbc, 0);
    __ct__CMCEffUpGreen(base + 0xcd4, 0);
    __ct__CMCEffUpPrm(base + 0xcec, 0);
    __ct__CMCEffSuccess(base + 0xd5c, 0);
    __ct__CMCEffFailure(base + 0xd78, 0);
    __ct__CMCEffCrystal(base + 0xd90, 0);
    __ct__CMCEffUpRank(base + 0xdcc, 0);
    __ct__CMCEffDivide(base + 0xde4, 0);
    __ct__CMCEffCylinder(base + 0xdfc, 0);
    *reinterpret_cast<u32*>(base + 0xe1c) = 0;
    __ct__CCur18(base + 0xe20, 0);
    __ct__CScrollBar(base + 0xe38, 0);
    __ct__CSysWin(base + 0xe78, 0);
    __ct__CCur18(base + 0xeb4, 0);
    __ct__CMCGetItemBox(base + 0xecc);
    base[0x13bf] = 0;

    // --- zero the 128 step slots (stride 0x34 at +0x13c0) ---
    // Pointer-walk do-while with the increment inside the condition: MWCC
    // keeps this rolled (cmplw/blt) instead of unrolling it.
    u8* p = base + 0x13c0;
    do {
        *(u32*)p = 0;
        *(u16*)(p + 4) = 0;
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
    *reinterpret_cast<f32*>(base + 0x2dcc) = lbl_eu_806684A4;
    base[0x2dd0] = zero2;
    base[0x2dd1] = zero2;
    base[0x2dd2] = zero2;
    base[0x2dd3] = zero2;
    base[0x2dd4] = zero2;
    base[0x2dd5] = zero2;

    // --- crystal-position groups (two VEC3 pairs) ---
    func_8004B60C(&group1[0], lbl_eu_806684A8, lbl_eu_806684AC, lbl_eu_806684B0);
    f32 g1y = lbl_eu_806684BC * lbl_eu_8066A210;
    f32 g2y = lbl_eu_806684C0 * lbl_eu_8066A210;
    group1[1].x = lbl_eu_806684B4;
    group1[1].y = lbl_eu_806684AC;
    group1[1].z = lbl_eu_806684B8;
    group2[0].x = lbl_eu_806684A4;
    group2[0].y = g1y;
    group2[0].z = lbl_eu_806684A4;
    func_8004B60C(&group2[1], lbl_eu_806684A4, g2y, lbl_eu_806684A4);

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
            initCrystalSubStruct(sub);
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

    // --- re-init the embedded crystal list from a default temp ---
    __ct__CMCCrystalList(listTmp);
    __ct__UnkClass_8011C974(base + 0xc1c, listTmp + 0x4);
    __ct__UnkClass_8011C974(base + 0xc2c, listTmp + 0x14);
    *reinterpret_cast<void**>(base + 0xc3c) = *reinterpret_cast<void**>(listTmp + 0x24);
    *reinterpret_cast<void**>(base + 0xc40) = *reinterpret_cast<void**>(listTmp + 0x28);
    *reinterpret_cast<void**>(base + 0xc44) = *reinterpret_cast<void**>(listTmp + 0x2c);
    *reinterpret_cast<void**>(base + 0xc48) = *reinterpret_cast<void**>(listTmp + 0x30);
    *reinterpret_cast<void**>(base + 0xc4c) = *reinterpret_cast<void**>(listTmp + 0x34);
    *reinterpret_cast<void**>(base + 0xc50) = *reinterpret_cast<void**>(listTmp + 0x38);
    *reinterpret_cast<void**>(base + 0xc54) = *reinterpret_cast<void**>(listTmp + 0x3c);
    *reinterpret_cast<void**>(base + 0xc58) = *reinterpret_cast<void**>(listTmp + 0x40);
    *reinterpret_cast<void**>(base + 0xc5c) = *reinterpret_cast<void**>(listTmp + 0x44);
    *reinterpret_cast<void**>(base + 0xc60) = *reinterpret_cast<void**>(listTmp + 0x48);
    base[0xc64] = listTmp[0x4c];
    base[0xc65] = listTmp[0x4d];
    // Load both words, store high word first (retail schedule).
    {
        u32 lo = *reinterpret_cast<u32*>(listTmp + 0x4e);
        u32 hi = *reinterpret_cast<u32*>(listTmp + 0x52);
        *reinterpret_cast<u32*>(base + 0xc6a) = hi;
        *reinterpret_cast<u32*>(base + 0xc66) = lo;
    }
    base[0xc6e] = listTmp[0x56];
    base[0xc6f] = listTmp[0x57];
    __dt__14CMCCrystalListFv(listTmp, -1);

    // --- re-init the embedded scroll bar from a default temp ---
    __ct__CScrollBar(sbTmp, 0);
    __ct__UnkClass_8011C974(base + 0xe3c, sbTmp + 0x4);
    *reinterpret_cast<void**>(base + 0xe4c) = *reinterpret_cast<void**>(sbTmp + 0x14);
    *reinterpret_cast<void**>(base + 0xe50) = *reinterpret_cast<void**>(sbTmp + 0x18);
    *reinterpret_cast<void**>(base + 0xe54) = *reinterpret_cast<void**>(sbTmp + 0x1c);
    *reinterpret_cast<void**>(base + 0xe58) = *reinterpret_cast<void**>(sbTmp + 0x20);
    base[0xe5c] = sbTmp[0x24];
    base[0xe5d] = sbTmp[0x25];
    base[0xe5e] = sbTmp[0x26];
    base[0xe5f] = sbTmp[0x27];
    *reinterpret_cast<f32*>(base + 0xe60) = *reinterpret_cast<f32*>(sbTmp + 0x28);
    *reinterpret_cast<f32*>(base + 0xe64) = *reinterpret_cast<f32*>(sbTmp + 0x2c);
    *reinterpret_cast<f32*>(base + 0xe68) = *reinterpret_cast<f32*>(sbTmp + 0x30);
    *reinterpret_cast<f32*>(base + 0xe6c) = *reinterpret_cast<f32*>(sbTmp + 0x34);
    *reinterpret_cast<f32*>(base + 0xe70) = *reinterpret_cast<f32*>(sbTmp + 0x38);
    base[0xe74] = sbTmp[0x3c];
    __dt__10CScrollBarFv(sbTmp, -1);

    // --- re-init the embedded system window from an arg-2 temp ---
    __ct__CSysWin(swTmp, 2);
    func_8016742C(base + 0xe78, swTmp);
    __dt__7CSysWinFv(swTmp, -1);

    // --- re-init the embedded item box from a default temp ---
    __ct__CMCGetItemBox(boxTmp);
    __ct__UnkClass_8011C974(base + 0xed0, boxTmp + 0x4);
    __ct__UnkClass_8011C974(base + 0xee0, boxTmp + 0x14);
    *reinterpret_cast<void**>(base + 0xef0) = *reinterpret_cast<void**>(boxTmp + 0x24);
    *reinterpret_cast<void**>(base + 0xef4) = *reinterpret_cast<void**>(boxTmp + 0x28);
    *reinterpret_cast<void**>(base + 0xef8) = *reinterpret_cast<void**>(boxTmp + 0x2c);
    *reinterpret_cast<void**>(base + 0xefc) = *reinterpret_cast<void**>(boxTmp + 0x30);
    *reinterpret_cast<void**>(base + 0xf00) = *reinterpret_cast<void**>(boxTmp + 0x34);
    *reinterpret_cast<void**>(base + 0xf04) = *reinterpret_cast<void**>(boxTmp + 0x38);
    *reinterpret_cast<void**>(base + 0xf08) = *reinterpret_cast<void**>(boxTmp + 0x3c);
    *reinterpret_cast<void**>(base + 0xf0c) = *reinterpret_cast<void**>(boxTmp + 0x40);
    *reinterpret_cast<void**>(base + 0xf10) = *reinterpret_cast<void**>(boxTmp + 0x44);
    *reinterpret_cast<void**>(base + 0xf14) = *reinterpret_cast<void**>(boxTmp + 0x48);
    base[0xf18] = boxTmp[0x4c];
    base[0xf19] = boxTmp[0x4d];
    *reinterpret_cast<void**>(base + 0xf1c) = *reinterpret_cast<void**>(boxTmp + 0x50);
    base[0xf20] = boxTmp[0x54];
    base[0xf21] = boxTmp[0x55];
    func_8018B0FC(base + 0xf24, boxTmp + 0x58);
    func_8018B0FC(base + 0xf3c, boxTmp + 0x70);
    func_8018B0FC(base + 0xf54, boxTmp + 0x88);
    func_8018B0FC(base + 0xf6c, boxTmp + 0xa0);
    func_8016742C(base + 0xf84, boxTmp + 0xb8);
    __ct__UnkClass_8011C974(base + 0xfc4, boxTmp + 0xf8);
    __ct__UnkClass_8011C974(base + 0xfd4, boxTmp + 0x108);
    // itemBox tail members (memberwise words/bytes).
    *reinterpret_cast<u32*>(base + 0xfe4) = *reinterpret_cast<u32*>(boxTmp + 0x118);
    *reinterpret_cast<u32*>(base + 0xfe8) = *reinterpret_cast<u32*>(boxTmp + 0x11c);
    *reinterpret_cast<u32*>(base + 0xfec) = *reinterpret_cast<u32*>(boxTmp + 0x120);
    *reinterpret_cast<u32*>(base + 0xff0) = *reinterpret_cast<u32*>(boxTmp + 0x124);
    *reinterpret_cast<u32*>(base + 0xff4) = *reinterpret_cast<u32*>(boxTmp + 0x128);
    *reinterpret_cast<u32*>(base + 0xff8) = *reinterpret_cast<u32*>(boxTmp + 0x12c);
    *reinterpret_cast<u32*>(base + 0xffc) = *reinterpret_cast<u32*>(boxTmp + 0x130);
    *reinterpret_cast<u32*>(base + 0x1000) = *reinterpret_cast<u32*>(boxTmp + 0x134);
    *reinterpret_cast<u32*>(base + 0x1004) = *reinterpret_cast<u32*>(boxTmp + 0x138);
    *reinterpret_cast<u32*>(base + 0x1008) = *reinterpret_cast<u32*>(boxTmp + 0x13c);
    *reinterpret_cast<u32*>(base + 0x100c) = *reinterpret_cast<u32*>(boxTmp + 0x140);
    *reinterpret_cast<u32*>(base + 0x1010) = *reinterpret_cast<u32*>(boxTmp + 0x144);
    *reinterpret_cast<u32*>(base + 0x1014) = *reinterpret_cast<u32*>(boxTmp + 0x148);
    *reinterpret_cast<u32*>(base + 0x1018) = *reinterpret_cast<u32*>(boxTmp + 0x14c);
    *reinterpret_cast<u32*>(base + 0x101c) = *reinterpret_cast<u32*>(boxTmp + 0x150);
    *reinterpret_cast<u32*>(base + 0x1020) = *reinterpret_cast<u32*>(boxTmp + 0x154);
    *reinterpret_cast<u32*>(base + 0x1024) = *reinterpret_cast<u32*>(boxTmp + 0x158);
    *reinterpret_cast<u32*>(base + 0x1028) = *reinterpret_cast<u32*>(boxTmp + 0x15c);
    *reinterpret_cast<u32*>(base + 0x102c) = *reinterpret_cast<u32*>(boxTmp + 0x160);
    *reinterpret_cast<u32*>(base + 0x1030) = *reinterpret_cast<u32*>(boxTmp + 0x164);
    *reinterpret_cast<u32*>(base + 0x1034) = *reinterpret_cast<u32*>(boxTmp + 0x168);
    *reinterpret_cast<u32*>(base + 0x1038) = *reinterpret_cast<u32*>(boxTmp + 0x16c);
    *reinterpret_cast<u32*>(base + 0x103c) = *reinterpret_cast<u32*>(boxTmp + 0x170);
    *reinterpret_cast<u32*>(base + 0x1040) = *reinterpret_cast<u32*>(boxTmp + 0x174);
    *reinterpret_cast<u32*>(base + 0x1044) = *reinterpret_cast<u32*>(boxTmp + 0x178);
    *reinterpret_cast<u32*>(base + 0x1048) = *reinterpret_cast<u32*>(boxTmp + 0x17c);
    *reinterpret_cast<u32*>(base + 0x104c) = *reinterpret_cast<u32*>(boxTmp + 0x180);
    base[0x1050] = boxTmp[0x184];
    *reinterpret_cast<u32*>(base + 0x1054) = *reinterpret_cast<u32*>(boxTmp + 0x188);
    base[0x1058] = boxTmp[0x18c];
    base[0x1059] = boxTmp[0x18d];
    base[0x105a] = boxTmp[0x18e];
    {
        u32 lo = *reinterpret_cast<u32*>(boxTmp + 0x190);
        u32 hi = *reinterpret_cast<u32*>(boxTmp + 0x194);
        *reinterpret_cast<u32*>(base + 0x1060) = hi;
        *reinterpret_cast<u32*>(base + 0x105c) = lo;
        u32 lo2 = *reinterpret_cast<u32*>(boxTmp + 0x198);
        u32 hi2 = *reinterpret_cast<u32*>(boxTmp + 0x19c);
        *reinterpret_cast<u32*>(base + 0x1068) = hi2;
        *reinterpret_cast<u32*>(base + 0x1064) = lo2;
    }
    *reinterpret_cast<u16*>(base + 0x106c) = *reinterpret_cast<u16*>(boxTmp + 0x1a0);
    base[0x106e] = boxTmp[0x1a2];
    // cursor table/flags/marker copy - pair-copy loops with -1-biased word
    // pointers reproduce retail's `lwz 4(r4); lwzu 8(r4); stw 4(r5);
    // stwu 8(r5); bdnz` counted loops (CBattleState pair-copy shape).
    // table part 1: 0xc u16s (24B = 3 pairs)
    {
        u32 n = 3;
        u32* s = reinterpret_cast<u32*>(boxTmp + 0x1a4) - 1;
        u32* d = reinterpret_cast<u32*>(base + 0x1070) - 1;
        do {
            u32 a = *(s + 1);
            u32 b = *(s += 2);
            *(d + 1) = a;
            *(d += 2) = b;
        } while (--n);
    }
    // table part 2: 0x48 u16s (144B = 18 pairs)
    {
        u32 n = 0x12;
        u32* s = reinterpret_cast<u32*>(boxTmp + 0x1bc) - 1;
        u32* d = reinterpret_cast<u32*>(base + 0x1088) - 1;
        do {
            u32 a = *(s + 1);
            u32 b = *(s += 2);
            *(d + 1) = a;
            *(d += 2) = b;
        } while (--n);
    }
    // flags part 1: 6 u32s (24B); retail interleaves the loads/stores.
    {
        u32 w0 = *reinterpret_cast<u32*>(boxTmp + 0x24c);
        u32 w1 = *reinterpret_cast<u32*>(boxTmp + 0x250);
        *reinterpret_cast<u32*>(base + 0x111c) = w1;
        *reinterpret_cast<u32*>(base + 0x1118) = w0;
        *reinterpret_cast<u32*>(base + 0x1120) = *reinterpret_cast<u32*>(boxTmp + 0x254);
        u32 w3 = *reinterpret_cast<u32*>(boxTmp + 0x258);
        u32 w4 = *reinterpret_cast<u32*>(boxTmp + 0x25c);
        *reinterpret_cast<u32*>(base + 0x1128) = w4;
        *reinterpret_cast<u32*>(base + 0x1124) = w3;
        *reinterpret_cast<u32*>(base + 0x112c) = *reinterpret_cast<u32*>(boxTmp + 0x260);
    }
    // flags part 2: 0x18 u8s (24B = 3 pairs)
    {
        u32 n = 3;
        u32* s = reinterpret_cast<u32*>(boxTmp + 0x264) - 1;
        u32* d = reinterpret_cast<u32*>(base + 0x1130) - 1;
        do {
            u32 a = *(s + 1);
            u32 b = *(s += 2);
            *(d + 1) = a;
            *(d += 2) = b;
        } while (--n);
    }
    *(u8*)(base + 0x1148) = *(u8*)(boxTmp + 0x27c);   // selection marker
    // pad: 0x80 u8s (128B = 16 pairs)
    {
        u32 n = 0x10;
        u32* s = reinterpret_cast<u32*>(boxTmp + 0x27d) - 1;
        u32* d = reinterpret_cast<u32*>(base + 0x1149) - 1;
        do {
            u32 a = *(s + 1);
            u32 b = *(s += 2);
            *(d + 1) = a;
            *(d += 2) = b;
        } while (--n);
    }
    // Selection-flag bytes + word pair (reversed store order), then the
    // trailing 256-byte table (retail n=0x20 counted pair loop).
    base[0x11cc] = boxTmp[0x300];
    base[0x11cd] = boxTmp[0x301];
    base[0x11ce] = boxTmp[0x302];
    base[0x11cf] = boxTmp[0x303];
    base[0x11d0] = boxTmp[0x304];
    base[0x11d1] = boxTmp[0x305];
    {
        u32 lo = *reinterpret_cast<u32*>(boxTmp + 0x308);
        u32 hi = *reinterpret_cast<u32*>(boxTmp + 0x30c);
        *reinterpret_cast<u32*>(base + 0x11d8) = hi;
        *reinterpret_cast<u32*>(base + 0x11d4) = lo;
    }
    {
        u32 n = 0x20;
        u32* s = reinterpret_cast<u32*>(boxTmp + 0x310) - 1;
        u32* d = reinterpret_cast<u32*>(base + 0x11dc) - 1;
        do {
            u32 a = *(s + 1);
            u32 b = *(s += 2);
            *(d + 1) = a;
            *(d += 2) = b;
        } while (--n);
    }
    // string-name fields.
    *reinterpret_cast<u16*>(base + 0x12e0) = *reinterpret_cast<u16*>(boxTmp + 0x414);
    base[0x12e2] = boxTmp[0x416];
    base[0x12e3] = boxTmp[0x417];
    base[0x12e4] = boxTmp[0x418];
    *reinterpret_cast<u32*>(base + 0x1328) =
        (u32)strlen(reinterpret_cast<const char*>(boxTmp + 0x41c));
    strcpy(reinterpret_cast<char*>(base + 0x12e8),
           reinterpret_cast<const char*>(boxTmp + 0x41c));
    *reinterpret_cast<u32*>(base + 0x13ac) =
        (u32)strlen(reinterpret_cast<const char*>(boxTmp + 0x460));
    strcpy(reinterpret_cast<char*>(base + 0x132c),
           reinterpret_cast<const char*>(boxTmp + 0x460));
    *reinterpret_cast<u32*>(base + 0x13b0) = *reinterpret_cast<u32*>(boxTmp + 0x4e4);
    base[0x13b4] = boxTmp[0x4e8];
    __dt__13CMCGetItemBoxFv(boxTmp, -1);
}

// Virtual dispatch at vtable offset +0x48 (index 18), taking one float arg.
// Used on this+0x20 and on the per-slot objects at this+0x44 (stride 0x5cc).
// MWCC reserves 2 hidden vtable slots (RTTI): declared index 16 => +0x48.
struct CMCVt48 {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m0A(); virtual void m0B();
    virtual void m0C(); virtual void m0D(); virtual void m0E(); virtual void m0F();
    virtual void m16(float f1);  // declared index 16 => +0x48
};

void CModelDispMakeCrystal::initCrystalSubStruct() {
    *(unsigned long*)this = 0;
    *(unsigned long*)((char*)this + 4) = 0;
    ((unsigned char*)this)[8] = 0;
}

// Retail 0x8021E348: constructor - builds the inline CActParamAnimGame at
// +0x8, then zeroes the six 0xC-byte crystal sub-buffers at +0x564..+0x5ac.
// NOTE: -O4,p static cap - retail's `stmw r29` (3-reg save) needs -O4,s
// (MWCC_CASES §16); body/loop otherwise byte-identical.
#pragma optimize_for_size on  // -O4,s stmw frame
CModelDispMakeCrystal* func_8021C4F0(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    __ct__Q22cf17CActParamAnimGameFv(base + 0x8);
    u8* p = base + 0x564;
    do {
        initCrystalSubStruct(p);
        p += 0xc;
    } while (p < base + 0x5ac);
    return self;
}
#pragma optimize_for_size off

// Retail 0x8021E398: sub-object destructor (array-element dtor for the
// 0x5cc-stride crystal display sub-structs) - destroys the inline
// CActParamAnimGame at +0x8 and frees the object when the flag is positive.
// Returns the object.
void* __dt__8021C540(void* obj, int flag)
{
    if (obj != nullptr) {
        __dt__Q22cf17CActParamAnimGameFv(reinterpret_cast<u8*>(obj) + 8, -1);
        if (flag > 0) {
            __dl__FPv(obj);
        }
    }
    return obj;
}

// Retail frame is stmw r30 (-O4,s shape); body otherwise identical.
#pragma optimize_for_size on
CModelDispMakeCrystal::~CModelDispMakeCrystal()
{
    u8* base = reinterpret_cast<u8*>(this);
    __dt__13CMCGetItemBoxFv(base + 0xecc, -1);
    __dt__6CCur18Fv(base + 0xeb4, -1);
    __dt__7CSysWinFv(base + 0xe78, -1);
    __dt__10CScrollBarFv(base + 0xe38, -1);
    __dt__6CCur18Fv(base + 0xe20, -1);
    __dt__14CMCEffCylinderFv(base + 0xdfc, -1);
    __dt__12CMCEffDivideFv(base + 0xde4, -1);
    __dt__12CMCEffUpRankFv(base + 0xdcc, -1);
    __dt__13CMCEffCrystalFv(base + 0xd90, -1);
    __dt__13CMCEffFailureFv(base + 0xd78, -1);
    __dt__13CMCEffSuccessFv(base + 0xd5c, -1);
    __dt__11CMCEffUpPrmFv(base + 0xcec, -1);
    __dt__13CMCEffUpGreenFv(base + 0xcd4, -1);
    __dt__12CMCEffUpBlueFv(base + 0xcbc, -1);
    __dt__11CMCEffUpRedFv(base + 0xca4, -1);
    __dt__11CMCEffStartFv(base + 0xc8c, -1);
    __dt__17CMCCrystalSupportFv(base + 0xc70, -1);
    __dt__14CMCCrystalListFv(base + 0xc18, -1);
    __dt__16CMCCylinderGaugeFv(base + 0xbec, -1);
    __destroy_arr(base + 0x44, (void*)__dt__8021C540, 0x5cc, 2);
    __dt__17UnkClass_8045F564Fv(base + 0x2c, -1);
}
#pragma optimize_for_size off

void func_8021C6E4(CModelDispMakeCrystal* self)
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
    func_80222848(base + 0xc18);
    func_801F34F4(base + 0xe38);
    reinterpret_cast<CMCSysWinFn*>(base + 0xe78)->m20();
    base[0xbe9] = 0;
    base[0xbdd] = 0;
    base[0x2dd2] = 0;
    base[0x2dd4] = 0;
}

// Retail 0x8021E5F8: per-frame state-machine driver - dispatch the crystal
// charge state, then tick every makecrystal UI sub-object.
void func_8021C7A0(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    switch (base[0xbdc]) {
    case 0:
        func_80220C34(self);
        break;
    case 1:
        func_8021CC60(self);
        break;
    case 2:
        func_8021CD8C(self);
        break;
    case 3:
        base[0xbdc] = 0;
        base[0x2dc3] = 1;
        break;
    }
    reinterpret_cast<CMCCylinderGauge*>(base + 0xbec)->func_80221FE0();
    func_802228B8(base + 0xc18);
    func_8022E988(base + 0xc70);
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
    func_801D202C(base + 0xe20);
    func_801F3540(base + 0xe38);
    func_8022B748(base + 0xe78);
    func_801D202C(base + 0xeb4);
    func_802979E4(base + 0xecc);
}

void func_8021C8B0(CModelDispMakeCrystal* self) {
    u8* base = reinterpret_cast<u8*>(self);
    CDeviceVI::waitForDrawDone();
    func_8022077C(self);
    func_8022B7F4(base + 0xe78);
    reinterpret_cast<CMCWinFn*>(base + 0xeb4)->m1();
    func_80297B68(base + 0xecc);
    func_801390E0(reinterpret_cast<CFileHandle**>(base + 0x3c));
    func_80139124(*reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40));
    *reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40) = nullptr;
    func_8045F778__17UnkClass_8045F564Fv(base + 0x2c);
}


// Retail 0x8021E780: draw every makecrystal UI sub-object.
// [stmw] r30 frame (see func_8021DC1C note).
#pragma optimize_for_size on
void func_8021C928(CModelDispMakeCrystal* self, nw4r::lyt::DrawInfo* drawInfo)
{
    u8* base = reinterpret_cast<u8*>(self);
    func_80222964(base + 0xc18, drawInfo);
    reinterpret_cast<CMCCylinderGauge*>(base + 0xbec)->func_80222070(drawInfo);
    func_8022E9E4(base + 0xc70, drawInfo);
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
    func_801D20B0(base + 0xe20, drawInfo);
    func_801F35B0(base + 0xe38, drawInfo);
    func_80297AAC(base + 0xecc, drawInfo);
    func_8022B7C8(base + 0xe78, drawInfo);
    func_801D20B0(base + 0xeb4, drawInfo);
}
#pragma optimize_for_size off

u8 CModelDispMakeCrystal::getCrystalStateA() { return reinterpret_cast<CModelDispMakeCrystalFull*>(this)->field_BE8; }

// Retail 0x8021E894: "ready to advance" gate - every crystal display slot
// must be built, plus the crystal-list / scrollbar / syswin gates, then the
// crystal-box file state machine answers.
int func_8021CA3C(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    // Two-entry gate: counted loop so MWCC emits the ctr form.
    u8 i = 0;
    u32 n = 2;
    do {
        if (base[(u32)i * 0x5cc + 0x60c] == 0) return 0;
        i++;
    } while (--n);
    if (base[0xbe9] == 0) return 0;
    if (*reinterpret_cast<u32*>(base + 0x40) == 0) return 0;
    if (func_80222A50(base + 0xc18) == 0) return 0;
    if (CScrollBar_isVisible(base + 0xe38) == 0) return 0;
    if (CSysWin_isReady(base + 0xe78) == 0) return 0;
    return func_80297CC0(base + 0xecc);
}

u8 CModelDispMakeCrystal::getCrystalStateB() { return reinterpret_cast<CModelDispMakeCrystalFull*>(this)->field_2DC3; }

u8 CModelDispMakeCrystal::getCrystalStateC() { return reinterpret_cast<CModelDispMakeCrystalFull*>(this)->field_2DD2; }

#pragma optimize_for_size on
void func_8021CB20(CModelDispMakeCrystal* self)
{
    u8* base;
    if (self->field_BDC) return;
    self->field_BDC = 1;
    self->field_BE8 = 0;
    base = reinterpret_cast<u8*>(self);
    int r0 = func_801392B4(self->field_5F0);
    if (r0 == 3 || r0 == 8) {
        reinterpret_cast<CMCCylinderGauge*>(base + 0xbec)->func_802222A4(3);
    } else {
        reinterpret_cast<CMCCylinderGauge*>(base + 0xbec)->func_802222A4(1);
    }
    reinterpret_cast<CMCCylinderGauge*>(base + 0xbec)->func_802220F8();
    func_80222A60(base + 0xc18);
    u8* entries = *reinterpret_cast<u8**>(base + 0xe1c);
    nw4r::math::VEC3 v;
    v.x = lbl_eu_806684C4;
    v.y = lbl_eu_806684C8;
    v.z = lbl_eu_806684A4;
    func_801F3670(base + 0xe38, &v);
    func_801F36BC(base + 0xe38, 8, entries[0]);
    func_801F367C(base + 0xe38);
    func_8021FC28(reinterpret_cast<CModelDispMakeCrystal*>(base), 0);
    u8 bbc = base[0xbbc];
    func_801392B4(base[0x5f0]);
    // Table lookup: byte-array indexing through a named offset so MWCC
    // keeps the scaled offset in a register (mulli) for both loads.
    const u8* tbl = lbl_eu_80535D90->c;
    int off = ((int)func_801392B4(bbc) - 1) * 3;
    lbl_eu_80664718[3] = 0;
    u8 c0 = tbl[off];
    u8 c1 = tbl[off + 1];
    u8 c2 = tbl[off + 2];
    lbl_eu_80664718[0] = c0;
    lbl_eu_80664718[1] = c1;
    lbl_eu_80664718[2] = c2;
    lbl_eu_80664718[5] = 1;
    func_8021FD44(reinterpret_cast<CModelDispMakeCrystal*>(base));
}
#pragma optimize_for_size off

void func_8021CC60(CModelDispMakeCrystal* self)
{
    nw4r::math::VEC3 v;
    u8* base = reinterpret_cast<u8*>(self);
    f32* fbe4 = reinterpret_cast<f32*>(base + 0xbe4);
    f32* fbe0 = reinterpret_cast<f32*>(base + 0xbe0);
    f32 v4 = *fbe4 + lbl_eu_806684A0;
    *fbe4 = v4;
    if (v4 >= lbl_eu_806684CC) {
        f32 v0 = *fbe0 - lbl_eu_806684D0;
        *fbe0 = v0;
        if (v0 < lbl_eu_806684A4) {
            *fbe0 = lbl_eu_806684A4;
            base[0xbdc] = 2;
            base[0xbe8] = 1;
            for (u8 i = 0; i < 8; i++) {
                func_80222F64(&v, base + 0xc18, i);
                reinterpret_cast<CMCEffUpPrm*>(base + 0xcec)->setTranslate(i, v);
            }
            func_80138078__FUl(0x3d);
        }
        void* p20 = *reinterpret_cast<void**>(base + 0x20);
        if (p20) {
            reinterpret_cast<CMCVt48*>(p20)->m16(*fbe0);
        }
        for (u8 i = 0; i < 2; i++) {
            void* p = *reinterpret_cast<void**>(base + (u32)i * 0x5cc + 0x44);
            if (p) {
                reinterpret_cast<CMCVt48*>(p)->m16(*fbe0);
            }
        }
    }
}

void func_8021CD8C(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (base[0xbe9] == 0) {
        func_80220C34(self);
    } else {
        for (u8 i = 0; i < 2; i++) {
            u8* p = base + (u32)i * 0x5cc;
            if (*reinterpret_cast<u32*>(p + 0x44)) {
                func_8004CF00(p + 0x4c);
            }
        }
    }
    // Input-trigger check: the bit differs between classic controller and
    // Wiimote/Nunchuk pad layouts.
    CPad* pad = cf::CfGameManager::getCurrentPad();
    u32 bit;
    // Classic-controller vs Wiimote/Nunchuk trigger bit.
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0)
        bit = (pad->mTurboPressButtonFlags >> 21) & 1;
    else
        bit = (pad->mTurboPressButtonFlags >> 4) & 1;
    if (bit) {
        *reinterpret_cast<f32*>(base + 0x2dcc) = lbl_eu_806684D4;
        base[0x2dd5] = 1;
    }
    func_8021FEDC(self);
}

// -O4,s frame: retail saves r30/r31 with stmw/lmw.
#pragma optimize_for_size on
void func_8021CE4C(CModelDispMakeCrystal* self)
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
    reinterpret_cast<CMCC8CFn*>(base + 0xc8c)->m2();
    base[0xbdd] = 0x1;
    func_80220954(self, 0, func_801392B4(base[0xbbc]));
}
#pragma optimize_for_size off

void func_8021CEF0(CModelDispMakeCrystal* self)
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
            func_80220954(self, 0, func_801392B4(base[0x5f0]));
        }
    }
}

void func_8021CFC0(CModelDispMakeCrystal* self)
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
    // Crystal-probability state (sbss, shared with func_8021CB20). Referenced
    // directly so MWCC re-materializes the @sda21 address at each use instead
    // of holding it in a callee-saved register (retail reloads it too).
    if (lbl_eu_80664718[5]) {
        u8 n31 = lbl_eu_80664718[3];
        u8 n29 = lbl_eu_80664718[1];
        u8 n28 = lbl_eu_80664718[0];
        s8 roll = (s8)((s32)ml::MTRand::getInstance()->rand31() % 100);
        s8 new3;
        if (roll < (s8)n28) {
            new3 = 0;
        } else {
            // bin the roll by cumulative thresholds; yields 2 or 3
            new3 = (s8)(2 + (roll >= (s8)((s8)n28 + (s8)n29)));
        }
        lbl_eu_80664718[3] = (u8)new3;
        if ((u8)new3 == n31) {
            lbl_eu_80664718[4]++;
        } else {
            lbl_eu_80664718[4] = 0;
        }
    }
    if (*reinterpret_cast<u32*>(base + 0x10)) {
        u8* p = reinterpret_cast<u8*>(*reinterpret_cast<u32*>(base + 0x10));
        switch (lbl_eu_80664718[3]) {
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
    func_80220954(self, 1, func_801392B4(base[0xbbc]));
}

void func_8021D168(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    f32* vel = reinterpret_cast<f32*>(base + 0x2dcc);
    f32 v = *vel + lbl_eu_806684A0;
    *vel = v;
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
    func_80220954(self, 1, func_801392B4(base[0x5f0]));
}

void func_8021D200(){}

// -O4,s frame: retail keeps the plain divw for the %10 (no magic-multiply).
#pragma optimize_for_size on
void func_8021D3E4(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    f32* f2dcc = reinterpret_cast<f32*>(base + 0x2dcc);
    f32 v = *f2dcc + lbl_eu_806684A0;
    *f2dcc = v;
    if (v < lbl_eu_806684DC) return;
    *f2dcc = lbl_eu_806684A4;
    base[0x13bf]++;
    lbl_eu_80664718[4]++;
    func_80220128(self);
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
            if (*reinterpret_cast<u32*>(base + (u32)i * 0x5cc + 0x44)) {
                void* m = *reinterpret_cast<void**>(base + (u32)i * 0x5cc + 0x44);
                if (m != 0) {
                    func_8004B9D4(base + (u32)i * 0x5cc + 0x4c, 0x21, 0, -1, 0);
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
                if (*reinterpret_cast<u32*>(base + (u32)i * 0x5cc + 0x44)) {
                    func_8004B9D4(base + (u32)i * 0x5cc + 0x4c, 0x21, 0, -1, 0);
                }
            }
        }
    }
}
#pragma optimize_for_size off

#pragma optimize_for_size on
void func_8021D564(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    f32* f2dcc = reinterpret_cast<f32*>(base + 0x2dcc);
    f32 v = *f2dcc + lbl_eu_806684A0;
    *f2dcc = v;
    if (v < lbl_eu_806684DC) return;
    *f2dcc = lbl_eu_806684A4;
    u8 n = base[0x2dc8] + 1;
    int r30 = 0;
    base[0x2dc8] = n;
    if (n >= base[0x2dc7]) {
        r30 = 1;
    } else if (n >= base[0x2dc6]) {
        // Retail calls getInstance() before each rand31(); divisor math stays
        // on signed ints so -O4,s keeps the literal divw/mullw/subf sequence.
        if ((s8)((int)ml::MTRand::getInstance()->rand31() % 5) == 0) r30 = 1;
    }
    if (r30 == 0) {
        // Delta drawn first, then the char-state lookup (retail order).
        s8 delta = (s8)((int)ml::MTRand::getInstance()->rand31() % 11);
        delta += 0x14;
        int r56 = (u8)func_801392B4(base[0x5f0]);
        if (r56 == 6) delta = (s8)((s8)delta * 2);
        s16 nv = *reinterpret_cast<s16*>(base + 0x2dc4) - (s8)delta;
        *reinterpret_cast<s16*>(base + 0x2dc4) = nv;
        if (nv > 0) {
            base[0xbdd] = 7;
            return;
        }
    }
    base[0xbdd] = 8;
    func_8021FC28(self, 0);
    func_80222A84(base + 0xc18);
    if (*reinterpret_cast<u32*>(base + 0x10)) {
        reinterpret_cast<u8*>(*reinterpret_cast<u32*>(base + 0x10))[0x59] = 7;
    }
    func_80138078__FUl(0x3f);
}
#pragma optimize_for_size off

// Retail 0x8021F50C: slot-trial state driver - once the random success
// window opens, pick the slot whose stored value beats the random threshold
// and place crystals in it. On success the state advances to 2 and the
// function ends (break out of the slot loop); otherwise the next slot is
// tried.
extern "C" void func_8021D6B4(void* selfp)
{
    u8* base = reinterpret_cast<u8*>(selfp);
    *reinterpret_cast<f32*>(base + 0x2dcc) = lbl_eu_806684D8;
    if ((u8)code80135FDC_getByte_64077() > 2) {
        u8 charId = (u8)func_801392B4(base[0xbbc]);
        for (u8 i = 0; i < 7; i++) {
            if ((s8)base[0x13b8 + i] <= 0) continue;
            u16 val = func_8013A7D0(charId, base[0x13b8 + i]);
            s32 iv = (s32)((float)(u32)val * lbl_eu_806684EC);
            ml::MTRand::getInstance();
            if ((s8)(rand31__Q22ml6MTRandFv() % 100) >= (u16)iv) continue;
            // Success: reset the charge, place the crystals, and stop.
            *reinterpret_cast<f32*>(base + 0x2dcc) = lbl_eu_806684A4;
            ml::MTRand::getInstance();
            u8* entries = *reinterpret_cast<u8**>(base + 0xe1c);
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
                    ((CModelDispMakeCrystal*)entries)->setCrystalPosEntry(
                        (u16)(s8)buf8[m],
                        (u16)(*(s16*)(buf12 + (m << 1)) +
                              *(u16*)(entries + (((u16)(s8)buf8[m]) << 3) + 4)),
                        (u16)(s8)buf8[m]);
                }
                m++;
            } while (m < (u8)n);
            func_802200A8(selfp);
            func_8021FC28((CModelDispMakeCrystal*)selfp, 0);
            func_8022EA88(base + 0xc70, base[0x13b8 + i]);
            func_8022EA64(base + 0xc70);
            func_8013B428__FUl(0x7e);
            func_8013B428__FUl(0x7f);
            func_80138078__FUl(0x8e);
            func_80220954(selfp, 4, base[0x13b8 + i]);
            base[0x13b8 + i] = 0;
            break;
        }
    }
    base[0xbdd] = 2;
}

void CModelDispMakeCrystal::setCrystalPosEntry(unsigned short index, short a, short b)
{
    struct Entry {
        char _pad0[4];
        short x;
        short y;
        char _pad8;
        unsigned char flag;
    };
    Entry* entry = (Entry*)((char*)this + ((unsigned int)index << 3));
    entry->x = a;
    entry->y = b;
    entry->flag = 1;
}

#pragma optimize_for_size on
void func_8021D9B8(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (!func_80222A58(base + 0xc18)) return;
    u16 r31 = *reinterpret_cast<u16*>(*reinterpret_cast<u8**>(base + 0xe1c) + 4);
    if (r31 >= 0x64) {
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
        func_80138078__FUl(0x91);
        // Then-block laid out first: >= 0xc8 falls through into rank 6.
        if (r31 >= 0xc8) {
            func_80220954(self, 6, 0);
        } else {
            func_80220954(self, 5, 0);
        }
    } else {
        // Failure: crystal count too low.
        base[0xbdd] = 0xa;
        reinterpret_cast<CMCC8CFn*>(base + 0xd78)->m2();
        for (u8 i = 0; i < 2; i++) {
            // Same dead-duplicate-beq shape as the success loop above.
            if (*reinterpret_cast<u32*>(base + (u32)i * 0x5cc + 0x44)) {
                if (*reinterpret_cast<void**>(base + (u32)i * 0x5cc + 0x44) != 0) {
                    func_8004B9D4(base + (u32)i * 0x5cc + 0x4c, 0x27, 0, -1, 0);
                }
            }
        }
        func_80138078__FUl(0xb2);
    }
}
#pragma optimize_for_size off

void func_8021DAF4(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    // Once the success effect finishes, record the "crystal placed" state
    // and switch the crystal list to its final state.
    if (isFinished__FPv(base + 0xd5c)) {
        base[0xbdd] = 0xb;
        func_80222ACC(base + 0xc18);
    }
}

void func_8021DB3C(CModelDispMakeCrystal* self) {
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
void func_8021DB9C(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (func_80222A58(base + 0xc18)) {
        if (lbl_eu_806684D4 == *reinterpret_cast<f32*>(base + 0x2dcc)) {
            base[0xbdd] = 0xf;
        } else {
            base[0xbdd] = 0xc;
            u8* entries = *reinterpret_cast<u8**>(base + 0xe1c);
            reinterpret_cast<CMCEffCrystal*>(base + 0xd90)->setCrystalInfo(
                *reinterpret_cast<u16*>(entries + 2), entries[1]);
            reinterpret_cast<CMCEffCrystal*>(base + 0xd90)->startIn();
            func_80138078__FUl(0x8c);
        }
    }
}

// Retail 0x8021FA74: crystal-list state driver - when the crystal effect
// finishes, either start the change-in anim (count >= 200) or trace-out.
// NOTE: retail saves r30/r31 as one [stmw] block; -O4,p emits separate stw
// (MWCC_CASES S16), so force the size-optimal frame like func_8021C4F0.
#pragma optimize_for_size on
void func_8021DC1C(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (isFinished__13CMCEffCrystalFv(base + 0xd90)) {
        u8* entries = *reinterpret_cast<u8**>(base + 0xe1c);
        if (*reinterpret_cast<u16*>(entries + 4) >= 0xc8) {
            base[0xbdd] = 0xd;
            reinterpret_cast<CMCEffCrystal*>(base + 0xd90)->startChange();
            reinterpret_cast<CMCC8CFn*>(base + 0xdcc)->m2();
            for (u8 i = 0; i < 2; i++) {
                u8* s = base + (u32)i * 0x5cc;
                if (*reinterpret_cast<u32*>(s + 0x44)) {
                    void* m = *reinterpret_cast<void**>(s + 0x44);
                    if (m != 0) {
                        func_8004B9D4(s + 0x4c, 0x22, 0, -1, 0);
                    }
                }
            }
            func_80138078__FUl(0x8d);
            func_80220954(self, 7, 0);
        } else {
            base[0xbdd] = 0xf;
            reinterpret_cast<CMCEffCrystal*>(base + 0xd90)->setTraceVisible(0);
            reinterpret_cast<CMCEffCrystal*>(base + 0xd90)->startOut();
            func_80138078__FUl(0x8c);
        }
    }
}
#pragma optimize_for_size off

// Retail 0x8021FB64: crystal-list state driver - when the start effect
// finishes, either run the success/special path (crystal count >= 300) or
// the trace-out path.
void func_8021DD0C(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (isFinished__11CMCEffStartFv(base + 0xdcc) == 0) return;
    f32* vel = reinterpret_cast<f32*>(base + 0x2dcc);
    f32 v = *vel + lbl_eu_806684A0;
    *vel = v;
    if (v < lbl_eu_806684D8) return;
    *vel = lbl_eu_806684A4;
    u16 count = *reinterpret_cast<u16*>(*reinterpret_cast<u8**>(base + 0xe1c) + 4);
    if (count >= 0x12c) {
        // Success path: full crystal count reached.
        base[0xbdd] = 0xe;
        reinterpret_cast<CMCC8CFn*>(base + 0xde4)->m2();
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
        func_80138078__FUl(0x8d);
        func_80220954(self, 7, 0);
    } else {
        base[0xbdd] = 0xf;
        reinterpret_cast<CMCEffCrystal*>(base + 0xd90)->setTraceVisible(0);
        reinterpret_cast<CMCEffCrystal*>(base + 0xd90)->startOut();
    }
}

// Retail 0x8021FC78: once the divide-in effect finishes, snap the gauge
// position, switch state, and begin the crystal effect's out animation.
void func_8021DE20(CModelDispMakeCrystal* self)
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
void func_8021DE7C(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (isFinished__13CMCEffCrystalFv(base + 0xd90)) {
        func_802203D8(self);
        u16 count = *reinterpret_cast<u16*>(*reinterpret_cast<u8**>(base + 0xe1c) + 4);
        if (count >= 0x64) {
            base[0xbdd] = 0xb;
            func_80222ACC(base + 0xc18);
        } else {
            *reinterpret_cast<f32*>(base + 0x2dcc) = lbl_eu_806684A4;
            base[0xbdd] = 0x10;
            reinterpret_cast<CMCEffSuccess*>(base + 0xd5c)->startOut();
        }
    }
}

void func_8021DEF8(CModelDispMakeCrystal* self)
{
    if (isFinished__FPv((u8*)self + 0xd5c)) {
        u8* base = reinterpret_cast<u8*>(self);
        u8* entries = *reinterpret_cast<u8**>(base + 0xe1c);
        if (entries[0]) {
            base[0xbdd] = 0x11;
            reinterpret_cast<CMCEffCylinder*>(base + 0xdfc)
                ->setNumber(reinterpret_cast<CMCCylinderGauge*>(base + 0xbec)->func_80222258());
            reinterpret_cast<CMCCylinderGauge*>(base + 0xbec)->func_80222118();
            reinterpret_cast<CMCEffCylinder*>(base + 0xdfc)->startIn();
        } else {
            base[0xbdd] = 0x14;
            reinterpret_cast<CMCCylinderGauge*>(base + 0xbec)->func_80222118();
        }
    }
}

void func_8021DF84(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    // Once the cylinder effect finishes and the gauge is full, start the
    // cylinder-count anim: rebuild the step list and flush the buffered entry.
    if (isFinished__14CMCEffCylinderFv(base + 0xdfc) == 0) return;
    if (func_802220F0__16CMCCylinderGaugeFv(base + 0xbec) == 0) return;
    base[0xbdd] = 0x12;
    func_801D216C(base + 0xe20, 1);
    u8 buf[16];
    func_80222F64(buf, base + 0xc18, base[0x2dc1]);
    reinterpret_cast<CMCE20Fn*>(base + 0xe20)->m2(buf);
    func_80223334(base + 0xc18);
}

// Retail 0x8021FE6C: input-driven cursor/trigger dispatch for the cylinder
// gauge-tune dialog. Classify the pad flags (Classic vs Wiimote/Nunchuk),
// then route trigger presses to cursor stepping (cancel/dir/confirm/menu)
// or to the CSysWin dialog paths.
// Retail frame is stmw r30 (2-reg save); decomp emits separate stw saves -
// per-function -O4,s pragma was tried and made the body diverge further
// (MWCC_CASES §16/§13 static cap).
void func_8021E014(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();
    // Button masks are kept as raw mask words and converted to bools right
    // after decode; trigger bits are extracted as 0/1 ints (retail extrwi).
    u32 cancel, dir, confirm, menu;
    int trigger1, trigger2, trigger3;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
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
        if (CSysWin_getUnk34(base + 0xe78) != 0) {
            if (CSysWin_isActive(base + 0xe78)) {
                func_8022B8E4(base + 0xe78);
                func_801D216C(base + 0xe20, 1);
            }
        } else {
            base[0xbdd] = 0x13;
            reinterpret_cast<CMCEffCylinder*>(base + 0xdfc)->startChange();
            func_80138078__FUl(3);
        }
    } else if (trigger2) {
        if (CSysWin_getUnk34(base + 0xe78) != 0) {
            if (CSysWin_isActive(base + 0xe78)) {
                func_8022B8E4(base + 0xe78);
                func_801D216C(base + 0xe20, 1);
            }
        }
    } else if (bCancel) {
        // Cursor back: decrement the row cursor with page wrap-around.
        if (CSysWin_getUnk34(base + 0xe78) == 0) {
            u8 c1 = (u8)(base[0x2dc1] - 1);
            u8* entries = *reinterpret_cast<u8**>(base + 0xe1c);
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
            func_80222F64(local, base + 0xc18, base[0x2dc1]);
            reinterpret_cast<CMCE20Fn*>(base + 0xe20)->m2(local);
            func_8021FC28(self, 1);
            func_801F3850(base + 0xe38, (u16)(s8)base[0x2dc2]);
            func_80138078__FUl(1);
        }
    } else if (bDir) {
        // Cursor forward: increment the row cursor with page wrap-around.
        if (CSysWin_getUnk34(base + 0xe78) == 0) {
            u8* entries = *reinterpret_cast<u8**>(base + 0xe1c);
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
            func_80222F64(local, base + 0xc18, base[0x2dc1]);
            reinterpret_cast<CMCE20Fn*>(base + 0xe20)->m2(local);
            func_8021FC28(self, 1);
            func_801F3850(base + 0xe38, (u16)(s8)base[0x2dc2]);
            func_80138078__FUl(1);
        }
    } else if (bConfirm) {
        // Page back by 8 rows.
        if (CSysWin_getUnk34(base + 0xe78) == 0) {
            u8* entries = *reinterpret_cast<u8**>(base + 0xe1c);
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
            func_80222F64(local, base + 0xc18, base[0x2dc1]);
            reinterpret_cast<CMCE20Fn*>(base + 0xe20)->m2(local);
            func_8021FC28(self, 1);
            func_801F3850(base + 0xe38, (u16)(s8)base[0x2dc2]);
            func_80138078__FUl(1);
        }
    } else if (bMenu) {
        // Page forward by 8 rows.
        if (CSysWin_getUnk34(base + 0xe78) == 0) {
            u8* entries = *reinterpret_cast<u8**>(base + 0xe1c);
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
            func_80222F64(local, base + 0xc18, base[0x2dc1]);
            reinterpret_cast<CMCE20Fn*>(base + 0xe20)->m2(local);
            func_8021FC28(self, 1);
            func_801F3850(base + 0xe38, (u16)(s8)base[0x2dc2]);
            func_80138078__FUl(1);
        }
    } else if (trigger3) {
        if (CSysWin_getUnk34(base + 0xe78) != 0) {
            if (CSysWin_isActive(base + 0xe78)) {
                func_8022B8E4(base + 0xe78);
                func_801D216C(base + 0xe20, 1);
            }
        } else {
            // Selection key: resolve the cursor-sum index to an entry's
            // u16 param and set the dialog text.
            s32 idx = (s8)base[0x2dc1] + (s8)base[0x2dc2];
            u8* entries = *reinterpret_cast<u8**>(base + 0xe1c);
            u16 ev = *(u16*)(entries + ((u32)((u16)idx << 3)) + 2);
            char* s = func_80136190(&lbl_eu_805090FC[0x38], &lbl_eu_805090FC[0x42], ev);
            func_8022B90C(base + 0xe78, 0);
            func_8022B9B4(base + 0xe78, s, 0);
            func_8022BFC8(base + 0xe78, 1);
            func_8022B8B8(base + 0xe78);
            func_801D216C(base + 0xe20, 0);
        }
    }
}

// Item-implementation vtable for the local CItemData in func_8021E5C0.
// Declared index i => byte offset +0x08+4i (MWCC reserves 2 hidden RTTI
// slots): index 5 => +0x1C, 18 => +0x50, 24 => +0x68, 31 => +0x84.
struct CMCItemVt {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(void* p);   // +0x1C
    virtual void m06(); virtual void m07(); virtual void m08(); virtual void m09();
    virtual void m0A(); virtual void m0B(); virtual void m0C(); virtual void m0D();
    virtual void m0E(); virtual void m0F(); virtual void m10(); virtual void m11();
    virtual void m12(); virtual void m13(); virtual void m14(); virtual void m15();
    virtual void m16(); virtual void m17(); virtual void m18(void* p, u32 x, u16 y);  // +0x50
    virtual void m19(); virtual void m1A(); virtual void m1B(); virtual void m1C();
    virtual void m1D(); virtual void m1E(); virtual void m1F(); virtual void m20();
    virtual void m21(); virtual void m22(); virtual void m23(); virtual void m24(void* p, u32 x, u16 y);  // +0x68
    virtual void m25(); virtual void m26(); virtual void m27(); virtual void m28();
    virtual void m29(); virtual void m2A(); virtual void m2B(); virtual void m2C();
    virtual void m2D(); virtual void m2E(); virtual void m2F(); virtual void m30();
    virtual void m31(void* p, u32 x);  // +0x84
};

// Local mirror of the item record used by func_8021E5C0: 0x34 bytes, head
// zeroed as a u32 + u16, then copied into the 0x34-stride step slots at
// +0x13c0 by func_8015704C.
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
void func_8021E5C0(CModelDispMakeCrystal* self)
{
    if (isFinished__14CMCEffCylinderFv((u8*)self + 0xdfc) == 0) return;
    reinterpret_cast<CMCCylinderGauge*>((u8*)self + 0xbec)->func_80222234();
    u8 v = reinterpret_cast<CMCCylinderGauge*>((u8*)self + 0xbec)->func_80222258();
    reinterpret_cast<CMCEffCylinder*>((u8*)self + 0xdfc)->setNumber(v);
    u8* entries = *reinterpret_cast<u8**>((u8*)self + 0xe1c);
    CMCItemData item;
    item.field_00 = 0;
    *reinterpret_cast<u16*>(&item.field_04) = 0;
    func_80158300(&item, 1);
    CMCItemVt* inst = (CMCItemVt*)CItem_initItemImplInstances(&item);
    inst->m05(&item);
    inst = (CMCItemVt*)CItem_initItemImplInstances(&item);
    inst->m31(&item, 1);
    func_801570A0(&item, entries[1]);
    for (u8 i = 0; i < 4; i++) {
        inst = (CMCItemVt*)CItem_initItemImplInstances(&item);
        inst->m18(&item, i, 0);
        inst = (CMCItemVt*)CItem_initItemImplInstances(&item);
        inst->m24(&item, i, 0);
    }
    u16 idx = (u16)((s8)((u8*)self)[0x2dc1] + (s8)((u8*)self)[0x2dc2]);
    inst = (CMCItemVt*)CItem_initItemImplInstances(&item);
    inst->m18(&item, 0, *(u16*)(entries + ((u16)idx << 3) + 2));
    inst = (CMCItemVt*)CItem_initItemImplInstances(&item);
    inst->m24(&item, 0, *(u16*)(entries + ((u16)idx << 3) + 4));
    u8 n = ((u8*)self)[0x2dc0];
    ((u8*)self)[0x2dc0] = n + 1;
    func_8015704C((u8*)self + n * 0x34 + 0x13c0, &item);
    func_8021E840(entries, idx);
    func_8021E888(entries);
    ((u8*)self)[0x2dc1] = 0;
    ((u8*)self)[0x2dc2] = 0;
    u8 buf[0xc];
    func_80222F64(buf, (u8*)self + 0xc18, 0);
    reinterpret_cast<CMCE20Fn*>((u8*)self + 0xe20)->m2(buf);
    func_8021FC28(self, 1);
    func_801F36BC((u8*)self + 0xe38, 8, entries[0]);
    func_801F3850((u8*)self + 0xe38, (u16)(s8)((u8*)self)[0x2dc2]);
    func_80138078__FUl(0xb1);
    if (entries[0] != 0 &&
        reinterpret_cast<CMCCylinderGauge*>((u8*)self + 0xbec)->func_80222258() != 0) {
        ((u8*)self)[0xbdd] = 0x12;
    } else {
        ((u8*)self)[0xbdd] = 0x14;
        reinterpret_cast<CMCEffCylinder*>((u8*)self + 0xdfc)->startOut();
    }
}

void func_8021E840(CModelDispMakeCrystal* self, u16 idx)
{
    // Clear the 8-byte param record for the crystal slot and copy it into
    // the slot entry (entry layout: 8-byte stride, param at +2).
    u8* base = reinterpret_cast<u8*>(self);
    CMCrySlotParam param;
    param.m0 = 0;
    param.m2 = 0;
    param.m4 = 0;
    param.m6 = 0;
    param.m7 = 0;
    func_80213E20(base + ((u32)idx << 3) + 2, &param);
}

// Retail 0x802206E0: refresh the crystal slot entry list, then find the first
// entry whose param u16 (at +2) is zero and record its index in byte 0.
// NOTE: -O4,p static cap - retail keeps this as an mtctr/bdnz countdown loop
// (-O4,s shape); plain -O4,p unrolls it (MWCC_CASES §856).
void func_8021E888(CModelDispMakeCrystal* self)
{
    CMCrySlotEntry* entries = reinterpret_cast<CMCrySlotEntry*>(self);
    func_8021E8E4(self);
    u8 i = 0;
    s32 n = 0x20;
    do {
        if (*reinterpret_cast<u16*>(reinterpret_cast<u8*>(self) + (i << 3) + 2) == 0) {
            reinterpret_cast<u8*>(self)[0] = i;
            break;
        }
        i++;
    } while (--n);
}

// Retail 0x8022073C: two-pass bubble-sort of the crystal-slot entries,
// then clear their +9 flag bytes. Pass 1 sorts by the u16 key at entry+4,
// pass 2 by the byte at entry+9 (both ascending, early-exit when a pass
// makes no swap). Swaps move the 8-byte param record at entry+2 through
// the retail copy helpers (func_802165E8 reads, func_80213E20 writes).
void func_8021E8E4(CModelDispMakeCrystal* self)
{
    u8 i;                          // pass-1 outer / pass-2 inner
    u8* base = reinterpret_cast<u8*>(self);
    u8 j;                          // pass-1 inner / pass-2 outer
    // Pass 1: sort by the u16 key at entry+4 (bubble sort, early exit).
    for (i = 0; i < base[0] - 1; i++) {
        int swapped = 0;
        for (j = 0; j < base[0] - 1 - i; j++) {
            u8* e1 = base + ((u8)j << 3);
            u8* e2 = base + ((u16)(j + 1) << 3);
            u8* p_cur = e1 + 2;
            u8* p_next = e2 + 2;
            u16 vi = *(u16*)(e1 + 4);
            u16 vi1 = *(u16*)(p_next + 2);
            if ((u32)vi < (u32)vi1) {
                CMCrySlotParam tA, tB, tC;
                func_802165E8(&tA, p_cur);
                func_80213E20(p_cur, func_802165E8(&tB, p_next));
                func_80213E20(p_next, func_802165E8(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
    // Pass 2: sort by the byte at entry+9 (outer/inner roles swapped).
    for (j = 0; j < base[0] - 1; j++) {
        int swapped = 0;
        for (i = 0; i < base[0] - 1 - j; i++) {
            u8* e1 = base + ((u8)i << 3);
            u8* e2 = base + ((u16)(i + 1) << 3);
            u8* p_cur = e1 + 2;
            u8* p_next = e2 + 2;
            if ((u32)e1[9] < (u32)p_next[7]) {
                CMCrySlotParam tA, tB, tC;
                func_802165E8(&tA, p_cur);
                func_80213E20(p_cur, func_802165E8(&tB, p_next));
                func_80213E20(p_next, func_802165E8(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
    // Clear the +9 flag byte of every entry.
    for (u8 m = 0; m < base[0]; m++) {
        base[((u8)m << 3) + 9] = 0;
    }
}

void func_8021EAB8(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    // Once the cylinder effect finishes, mark the cylinder phase done and
    // advance the crystal list state.
    if (isFinished__14CMCEffCylinderFv(base + 0xdfc)) {
        base[0xbdd] = 0x15;
        func_80222AA8(base + 0xc18);
    }
}

// Retail 0x80220958: crystal charge float tick - accumulate the charge,
// clamp and flag the charged state, then push the value into the model and
// slot effects; once charged, advance the crystal list state.
void func_8021EB00(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    f32* fbe0 = reinterpret_cast<f32*>(base + 0xbe0);
    f32 v = *fbe0 + lbl_eu_806684D0;
    *fbe0 = v;
    if (v > lbl_eu_806684A0) {
        *fbe0 = lbl_eu_806684A0;
        base[0xbdc] = 0;
        base[0xbe8] = 1;
        base[0x2dc3] = 1;
    }
    void* p20 = *reinterpret_cast<void**>(base + 0x20);
    if (p20) {
        reinterpret_cast<CMCVt48*>(p20)->m16(*fbe0);
    }
    for (u8 i = 0; i < 2; i++) {
        void* p = *reinterpret_cast<void**>(base + (u32)i * 0x5cc + 0x44);
        if (p) {
            reinterpret_cast<CMCVt48*>(p)->m16(*fbe0);
        }
    }
    if (!(*fbe0 < lbl_eu_806684A0)) {
        if (func_80222A58(base + 0xc18)) {
            base[0xbdd] = 0x16;
            func_80222AF0(base + 0xc18);
            func_801F369C(base + 0xe38);
            func_801D216C(base + 0xe20, 0);
        }
    }
}

// Retail 0x80220A5C: after the crystal list is active, scan the step list
// (stride 0x34 at +0x13c0) for a step of type 9; the result picks the
// pass/fail UI state (0x17 or 0x1e) and re-flushes the step list.
void func_8021EC04(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (func_80222A58(base + 0xc18) == 0) return;
    func_8022077C(self);
    u8 count = base[0x2dc0];
    int found = 0;
    for (u8 i = 0; i < count; i++) {
        CMCStep* p = reinterpret_cast<CMCStep*>(base + (u32)i * 0x34 + 0x13c0);
        if (p && p->type == 9) {
            found = 1;
            break;
        }
    }
    if (found) {
        base[0xbdd] = 0x17;
        func_80297D2C(base + 0xecc, 0x9, base + 0x13c0, base[0x2dc0]);
    } else {
        base[0xbdd] = 0x1e;
        func_80297D2C(base + 0xecc, 0x3, base + 0x13c0, base[0x2dc0]);
    }
}

void func_8021ECD4(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();
    u32 trigger1, trigger2, trigger3, cancel, confirm, menu, dir;
    // The bit positions differ between Classic Controller and Wiimote/Nunchuk.
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
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
        if (!func_80297D24(base + 0xecc)) {
            int r = func_80298540(base + 0xecc);
            if (r == 1) {
                // Proceed to the pass/fail tuning state.
                base[0xbdd] = 0x19;
                func_802985B4(base + 0xecc);
                func_80297E18(base + 0xecc);
                func_80138078__FUl(3);
            } else if (r == 2) {
                // Jump to gauge-tuning state: rebuild the confirmation UI.
                base[0xbdd] = 0x1a;
                base[0x2dd1] = 0;
                char* a = (char*)func_80136190(&lbl_eu_805090FC[0x47], &lbl_eu_805090FC[0x52], 2);
                char* b = (char*)func_80136190(&lbl_eu_805090FC[0x47], &lbl_eu_805090FC[0x52], 4);
                char* c = (char*)func_80136190(&lbl_eu_805090FC[0x47], &lbl_eu_805090FC[0x52], 5);
                func_8022B90C(base + 0xe78, 2);
                func_8022B9B4(base + 0xe78, a, 0);
                func_8022BF6C(base + 0xe78, b, c);
                func_8022BFC8(base + 0xe78, 0);
                func_8022B8B8(base + 0xe78);
                func_8029860C(base + 0xecc, 0);
                func_80138078__FUl(5);
            }
        } else {
            func_802984E4(base + 0xecc);
        }
    } else if (trigger2) {
        func_80298450(base + 0xecc);
    } else if (cancel) {
        func_80297E90(base + 0xecc);
    } else if (dir) {
        func_80297FB4(base + 0xecc);
    } else if (confirm) {
        func_802980DC(base + 0xecc);
    } else if (menu) {
        func_80298228(base + 0xecc);
    } else if (trigger3) {
        func_80298614(base + 0xecc);
    }
}

void func_8021EF30(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (!func_80297D1C(base + 0xecc)) return;
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
        func_80297D2C(base + 0xecc, 3, base + 0x13c0, base[0x2dc0]);
    } else {
        base[0xbdd] = 0x25;
    }
}

void func_8021EFE4(CModelDispMakeCrystal* self) {
    u8* base = reinterpret_cast<u8*>(self);
    if (!CSysWin_isActive(base + 0xe78)) return;
    base[0xbdd] = 0x1b;
    func_801D216C(base + 0xeb4, 1);
    u8 param = base[0x2dd1];
    u8 buf[16];
    func_8022C1B4(buf, base + 0xe78, param);
    reinterpret_cast<CMCWinFn*>(base + 0xeb4)->m2(buf);
}


// [stmw] r30 frame (see func_8021DC1C note): -O4,s.
#pragma optimize_for_size on
void func_8021F058(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();
    // Cursor step / cancel booleans derived from turbo and pressed button flags.
    // Bit positions differ between Classic controller and Wiimote/Nunchuk.
    u32 trigger1, trigger2, cancel, dir;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
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
        func_801D216C(base + 0xeb4, 0);
        func_8022B8E4(base + 0xe78);
        func_80138078__FUl(3);
    } else if (trigger2) {
        // Forward cursor button: reset step and play forward sound.
        base[0xbdd] = 0x1c;
        base[0x2dd1] = 1;
        func_801D216C(base + 0xeb4, 0);
        func_8022B8E4(base + 0xe78);
        func_80138078__FUl(6);
    } else if (cancel) {
        // Cursor back: decrement step (wrapping to 1).
        u8 v = base[0x2dd1] - 1;
        base[0x2dd1] = v;
        if ((s8)v < 0) base[0x2dd1] = 1;
        u8 param = base[0x2dd1];
        u8 buf[12];
        func_8022C1B4(buf, base + 0xe78, param);
        reinterpret_cast<CMCWinFn*>(base + 0xeb4)->m2(buf);
        func_80138078__FUl(1);
    } else if (dir) {
        // Cursor forward: increment step (resetting to 0).
        u8 v = base[0x2dd1] + 1;
        base[0x2dd1] = v;
        if ((s8)v > 1) base[0x2dd1] = 0;
        u8 param = base[0x2dd1];
        u8 buf[12];
        func_8022C1B4(buf, base + 0xe78, param);
        reinterpret_cast<CMCWinFn*>(base + 0xeb4)->m2(buf);
        func_80138078__FUl(1);
    }
}
#pragma optimize_for_size off

// Retail 0x80221104: while the gauge-tune dialog is open, branch on the cursor
// step: zero runs the confirm-side cleanup, non-zero moves the cursor forward.
void func_8021F214(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (CSysWin_isActive(base + 0xe78)) {
        if ((s8)base[0x2dd1] == 0) {
            base[0xbdd] = 0x1d;
            func_802A1500();
            func_80189C88();
            func_80043C88();
            func_80133E58(0x9, *reinterpret_cast<u32*>(base + 0xc), 0x0);
        } else {
            base[0xbdd] = 0x19;
            func_80297E18(base + 0xecc);
        }
    }
}

void func_8021F290(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    // While the item menu is not open, move to the pass/fail tuning state
    // and reset the crystal box file state machine.
    if (func_80167A18() == 0) {
        base[0xbdd] = 0x18;
        func_8029860C(base + 0xecc, 1);
    }
}

void func_8021F2D8(CModelDispMakeCrystal* self)
{
    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();
    int trigger1, trigger2, trigger3, cancel, confirm, menu, dir;
    // The bit positions differ between Classic Controller and Wiimote/Nunchuk.
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
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
        if (func_80297D24((u8*)self + 0xecc) != 0) {
            func_802984E4((u8*)self + 0xecc);
        } else {
            u8 r = (u8)func_80298540((u8*)self + 0xecc);
            if (r == 1) {
                // Proceed to the pass/fail tuning state.
                ((u8*)self)[0xbdd] = 0x20;
                func_802985B4((u8*)self + 0xecc);
                func_80297E18((u8*)self + 0xecc);
                func_80138078__FUl(3);
            } else if (r == 2) {
                // Jump to gauge-tuning state: rebuild the confirmation UI.
                ((u8*)self)[0xbdd] = 0x21;
                ((u8*)self)[0x2dd1] = 0;
                char* a = func_80136190(&lbl_eu_805090FC[0x47], &lbl_eu_805090FC[0x52], 2);
                char* b = func_80136190(&lbl_eu_805090FC[0x47], &lbl_eu_805090FC[0x52], 4);
                char* c = func_80136190(&lbl_eu_805090FC[0x47], &lbl_eu_805090FC[0x52], 5);
                func_8022B90C((u8*)self + 0xe78, 2);
                func_8022B9B4((u8*)self + 0xe78, a, 0);
                func_8022BF6C((u8*)self + 0xe78, b, c);
                func_8022BFC8((u8*)self + 0xe78, 0);
                func_8022B8B8((u8*)self + 0xe78);
                func_8029860C((u8*)self + 0xecc, 0);
                func_80138078__FUl(5);
            }
        }
    } else if (trigger2) {
        func_80298450((u8*)self + 0xecc);
    } else if (cancel) {
        func_80297E90((u8*)self + 0xecc);
    } else if (dir) {
        func_80297FB4((u8*)self + 0xecc);
    } else if (confirm) {
        func_802980DC((u8*)self + 0xecc);
    } else if (menu) {
        func_80298228((u8*)self + 0xecc);
    } else if (trigger3) {
        func_80298614((u8*)self + 0xecc);
    }
}

void func_8021F534(CModelDispMakeCrystal* self) {
    u8* base = reinterpret_cast<u8*>(self);
    if (!CSysWin_isActive(base + 0xe78)) return;
    base[0xbdd] = 0x22;
    func_801D216C(base + 0xeb4, 1);
    u8 param = base[0x2dd1];
    u8 buf[16];
    func_8022C1B4(buf, base + 0xe78, param);
    reinterpret_cast<CMCWinFn*>(base + 0xeb4)->m2(buf);
}


#pragma optimize_for_size on
void func_8021F5A8(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();
    u32 trigger1, trigger2, cancel, dir;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
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
        func_801D216C(base + 0xeb4, 0);
        func_8022B8E4(base + 0xe78);
        func_80138078__FUl(3);
    } else if (trigger2) {
        base[0xbdd] = 0x23;
        base[0x2dd1] = 1;
        func_801D216C(base + 0xeb4, 0);
        func_8022B8E4(base + 0xe78);
        func_80138078__FUl(6);
    } else if (cancel) {
        u8 v = base[0x2dd1] - 1;
        base[0x2dd1] = v;
        if ((s8)v < 0) base[0x2dd1] = 1;
        u8 param = base[0x2dd1];
        u8 buf[12];
        func_8022C1B4(buf, base + 0xe78, param);
        reinterpret_cast<CMCWinFn*>(base + 0xeb4)->m2(buf);
        func_80138078__FUl(1);
    } else if (dir) {
        u8 v = base[0x2dd1] + 1;
        base[0x2dd1] = v;
        if ((s8)v > 1) base[0x2dd1] = 0;
        u8 param = base[0x2dd1];
        u8 buf[12];
        func_8022C1B4(buf, base + 0xe78, param);
        reinterpret_cast<CMCWinFn*>(base + 0xeb4)->m2(buf);
        func_80138078__FUl(1);
    }
}
#pragma optimize_for_size off

// Retail 0x80221654: same gauge-tune dialog branch as func_8021F214, but for
// the crystal-success menu (confirm path 3 / cursor-forward path 0x20).
void func_8021F764(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (CSysWin_isActive(base + 0xe78)) {
        if ((s8)base[0x2dd1] == 0) {
            base[0xbdd] = 0x24;
            func_802A1500();
            func_80189C88();
            func_80043C88();
            func_80133E58(0x3, *reinterpret_cast<u32*>(base + 0xc), 0x0);
        } else {
            base[0xbdd] = 0x20;
            func_80297E18(base + 0xecc);
        }
    }
}

void func_8021F7E0(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    // Same item-menu gate as func_8021F290, but for the success state (0x1f).
    if (func_80167A18() == 0) {
        base[0xbdd] = 0x1f;
        func_8029860C(base + 0xecc, 1);
    }
}

void func_8021F828(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    base[0xbdd] = 0x26;
    base[0x2dd1] = 0x0;
    char* a = (char*)func_80136190(&lbl_eu_805090FC[0x57], &lbl_eu_805090FC[0x63], 0x3d);
    char* b = (char*)func_80136190(&lbl_eu_805090FC[0x57], &lbl_eu_805090FC[0x63], 0x3e);
    char* c = (char*)func_80136190(&lbl_eu_805090FC[0x57], &lbl_eu_805090FC[0x63], 0x3f);
    func_8022B90C(base + 0xe78, 2);
    func_8022B9B4(base + 0xe78, a, 0);
    func_8022BF6C(base + 0xe78, b, c);
    func_8022BFC8(base + 0xe78, 0);
    func_8022B8B8(base + 0xe78);
}

void func_8021F8E4(CModelDispMakeCrystal* self) {
    u8* base = reinterpret_cast<u8*>(self);
    if (!CSysWin_isActive(base + 0xe78)) return;
    base[0xbdd] = 0x27;
    func_801D216C(base + 0xeb4, 1);
    u8 param = base[0x2dd1];
    u8 buf[16];
    func_8022C1B4(buf, base + 0xe78, param);
    reinterpret_cast<CMCWinFn*>(base + 0xeb4)->m2(buf);
}


void func_8021F958(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();
    u32 trigger1, trigger2, cancel, dir;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
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
        func_801D216C(base + 0xeb4, 0);
        func_8022B8E4(base + 0xe78);
        func_80138078__FUl(3);
    } else if (trigger2) {
        base[0xbdd] = 0x28;
        base[0x2dd1] = 1;
        func_801D216C(base + 0xeb4, 0);
        func_8022B8E4(base + 0xe78);
        func_80138078__FUl(6);
    } else if (cancel) {
        u8 v = base[0x2dd1] - 1;
        base[0x2dd1] = v;
        if ((s8)v < 0) base[0x2dd1] = 1;
        u8 param = base[0x2dd1];
        u8 buf[16];
        func_8022C1B4(buf, base + 0xe78, param);
        reinterpret_cast<CMCWinFn*>(base + 0xeb4)->m2(buf);
        func_80138078__FUl(1);
    } else if (dir) {
        u8 v = base[0x2dd1] + 1;
        base[0x2dd1] = v;
        if ((s8)v > 1) base[0x2dd1] = 0;
        u8 param = base[0x2dd1];
        u8 buf[16];
        func_8022C1B4(buf, base + 0xe78, param);
        reinterpret_cast<CMCWinFn*>(base + 0xeb4)->m2(buf);
        func_80138078__FUl(1);
    }
}

// Retail 0x8022196C: while the CSysWin dialog is open, mark the confirmation
// step active; if the cursor step is zero, flag the confirm UI as initialised.
void func_8021FB14(CModelDispMakeCrystal* self)
{
    CModelDispMakeCrystalFull* full = reinterpret_cast<CModelDispMakeCrystalFull*>(self);
    if (CSysWin_isActive(reinterpret_cast<u8*>(self) + 0xe78)) {
        full->field_BDC = 3;
        if (full->field_2DD1 == 0) {
            full->field_2DD2 = 1;
        }
    }
}

// Retail symbol is the bare C-linkage name (defined below), so intra-TU
// calls emit the unmangled reloc.
extern "C" void func_8021FB68(CModelDispMakeCrystal* self, u8* obj)
{
    CDeviceVI::waitForDrawDone();
    if (*reinterpret_cast<u32*>(obj) == 0) return;
    func_8004B6BC(obj + 0x8, *reinterpret_cast<void**>(obj + 0x4));
    reinterpret_cast<CMCVtE0*>(obj + 0x8)->m36();
    func_80495E60(*reinterpret_cast<void**>(obj + 0x4));
    func_80495E60(reinterpret_cast<void*>(*reinterpret_cast<u32*>(obj + 0x0)));
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

extern "C" void func_8021FC28(CModelDispMakeCrystal* self, u8 arg4)
{
    u8* base = reinterpret_cast<u8*>(self);
    u8* entries = *reinterpret_cast<u8**>(base + 0xe1c);
    if (entries == nullptr) return;
    func_8021E8E4(entries);
    func_80222D9C(base + 0xc18, arg4);
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
            func_80223004(base + 0xc18, i, v4, v8);
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

extern "C" void func_8021FD44(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    u8 r31 = func_801392B4(base[0x5f0]);
    u8 r30 = func_801392B4(base[0xbbc]);
    u16 val = func_8013A7D0(r31, r30);
    s16 iv = (s16)(lbl_eu_806684FC * val + lbl_eu_806684F8);
    *reinterpret_cast<f32*>(base + 0x2dcc) = lbl_eu_806684A4;
    base[0x2dc8] = 0;
    *reinterpret_cast<u16*>(base + 0x2dc4) = iv;
    base[0x2dc6] = (u8)(iv / 30);
    base[0x2dc7] = (u8)(iv / 20);
    for (u8 k = 0; k < 7; k++) {
        if (k >= code80135FDC_getByte_64077()) {
            base[0x13b8 + k] = 0;
        } else {
            base[0x13b8 + k] = func_801392B4(k);
        }
    }
    base[0x13b8 + (s8)base[0x5f0]] = 0;
    base[0x13b8 + (s8)base[0xbbc]] = 0;
    base[0x2dd3] = 0;
    u8* a = reinterpret_cast<u8*>(func_8009EC9C(r31));
    u8* b = a + 0x3534;
    if (func_8026178C(b, 0x90)) {
        base[0x2dd3] = (u8)(base[0x2dd3] + func_8025FB10(b, 0x90));
    }
    u8* a2 = reinterpret_cast<u8*>(func_8009EC9C(r30));
    u8* b2 = a2 + 0x3534;
    if (func_8026178C(b2, 0x90)) {
        base[0x2dd3] = (u8)(base[0x2dd3] + func_8025FB10(b2, 0x90));
    }
}

// Retail 0x80221D34: makecrystal state-machine dispatcher - dispatch the
// crystal-charge state byte (+0xbdd) through a dense jump table. States
// 0x16/0x1e/0x20 inline the file-state check (func_80297D1C) and advance
// the state on success; every other state delegates to a state-runner.
void func_8021FEDC(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    switch (base[0xbdd]) {
    case 0x00: func_8021CE4C(self); break;
    case 0x01: func_8021CEF0(self); break;
    case 0x02: func_8021CFC0(self); break;
    case 0x03: func_8021D168(self); break;
    case 0x04: func_8021D200(); break;
    case 0x05: func_8021D3E4(self); break;
    case 0x06: func_8021D564(self); break;
    case 0x07: func_8021D6B4(self); break;
    case 0x08: func_8021D9B8(self); break;
    case 0x09: func_8021DAF4(self); break;
    case 0x0a: func_8021DB3C(self); break;
    case 0x0b: func_8021DB9C(self); break;
    case 0x0c: func_8021DC1C(self); break;
    case 0x0d: func_8021DD0C(self); break;
    case 0x0e: func_8021DE20(self); break;
    case 0x0f: func_8021DE7C(self); break;
    case 0x10: func_8021DEF8(self); break;
    case 0x11: func_8021DF84(self); break;
    case 0x12: func_8021E014(self); break;
    case 0x13: func_8021E5C0(self); break;
    case 0x14: func_8021EAB8(self); break;
    case 0x15: func_8021EB00(self); break;
    case 0x16:
        // Charged state: wait for the crystal-box file state machine to be
        // ready, then move to the pass/fail tuning state (0x18).
        if (func_80297D1C(base + 0xecc)) base[0xbdd] = 0x18;
        break;
    case 0x17: func_8021ECD4(self); break;
    case 0x18: func_8021EF30(self); break;
    case 0x19: func_8021EFE4(self); break;
    case 0x1a: func_8021F058(self); break;
    case 0x1b: func_8021F214(self); break;
    case 0x1c: func_8021F290(self); break;
    case 0x1d: func_8021F2D8(self); break;
    case 0x1e:
        // Tuning-cancel path: wait for the file state machine, then jump to
        // the item-menu-open state (0x1f).
        if (func_80297D1C(base + 0xecc)) base[0xbdd] = 0x1f;
        break;
    case 0x1f: func_8021F534(self); break;
    case 0x20:
        // Success confirm path: wait for the file state machine, then jump to
        // the success state (0x25).
        if (func_80297D1C(base + 0xecc)) base[0xbdd] = 0x25;
        break;
    case 0x21: func_8021F5A8(self); break;
    case 0x22: func_8021F764(self); break;
    case 0x23: func_8021F7E0(self); break;
    case 0x24: func_8021F828(self); break;
    case 0x25: func_8021F8E4(self); break;
    case 0x26: func_8021F958(self); break;
    case 0x27: func_8021FB14(self); break;
    }
}

// Retail 0x80221F80: crystal placement state driver - dispatch on the
// buffered probability-state byte in sbss, place crystal entries for the
// state, and refresh the position/effect sub-objects. All three states end
// with func_80220954(self, 2, 0).
void func_80220128(CModelDispMakeCrystal* self)
{
    switch (lbl_eu_80664718[3]) {
    case 0: {
        // Single placement: pick a random slot, offset the crystal entry by
        // a random amount (extra +1 for char 2, or the streak count for
        // char 4), and write the entry.
        u8* entries = *reinterpret_cast<u8**>((u8*)self + 0xe1c);
        u8 count = entries[0];
        ml::MTRand::getInstance();
        s8 slot = (s8)(count != 0 ? rand31__Q22ml6MTRandFv() % count : 0);
        ml::MTRand::getInstance();
        int d = (s8)(rand31__Q22ml6MTRandFv() % 8) + 3;
        u8 c = func_801392B4(self->field_5F0);
        if (c == 2) {
            ml::MTRand::getInstance();
            d += (s8)((s8)(rand31__Q22ml6MTRandFv() % 5) + 1);
        } else if (c == 4) {
            d = (s8)(lbl_eu_80664718[4] + 1);
        }
        ((CModelDispMakeCrystal*)entries)->setCrystalPosEntry(
            (u16)slot,
            (u16)((s8)d + *(u16*)(entries + ((u16)slot << 3) + 4)),
            (u16)(s8)d);
        reinterpret_cast<CMCC8CFn*>((u8*)self + 0xca4)->m2();
        func_802200A8(self);
        func_8021FC28(self, 0);
        func_80138078__FUl(0x8d);
        break;
    }
    case 1: {
        // Multi placement: place one crystal per entry, each with a random
        // offset (+1 normally, extra +1 for char 7, streak count for char 4).
        u8* entries = *reinterpret_cast<u8**>((u8*)self + 0xe1c);
        u8 count = entries[0];
        for (u8 i = 0; i < count; i++) {
            ml::MTRand::getInstance();
            if (count != 0) {
                rand31__Q22ml6MTRandFv();
            }
            ml::MTRand::getInstance();
            int d = (s8)(rand31__Q22ml6MTRandFv() % 5) + 1;
            u8 c = func_801392B4(self->field_5F0);
            if (c == 7) {
                ml::MTRand::getInstance();
                d += (s8)((s8)(rand31__Q22ml6MTRandFv() % 5) + 1);
            } else if (c == 4) {
                d = (s8)(lbl_eu_80664718[4] + 1);
            }
            ((CModelDispMakeCrystal*)entries)->setCrystalPosEntry(
                i,
                (u16)((s8)d + *(u16*)(entries + ((u8)i << 3) + 4)),
                (u16)(s8)d);
        }
        reinterpret_cast<CMCC8CFn*>((u8*)self + 0xcbc)->m2();
        func_802200A8(self);
        func_8021FC28(self, 0);
        func_80138078__FUl(0x8c);
        break;
    }
    case 2: {
        // Gauge-tune: refresh the cylinder gauge fill with a char-dependent
        // constant, then flush the effect list.
        u8 c = func_801392B4(self->field_5F0);
        if (c == 5) {
            reinterpret_cast<CMCCylinderGauge*>((u8*)self + 0xbec)->func_802221A4(
                lbl_eu_80668500);
        } else {
            reinterpret_cast<CMCCylinderGauge*>((u8*)self + 0xbec)->func_802221A4(
                lbl_eu_806684A4);
        }
        reinterpret_cast<CMCC8CFn*>((u8*)self + 0xcd4)->m2();
        func_80138078__FUl(0xb0);
        break;
    }
    }
    func_80220954(self, 2, 0);
}

// Count the used crystal slots (entry byte +9 set) and number them.
// The halfword at entry+6 rides unmasked into the u8 setNumber param
// (retail passes the raw lhz result; MWCC emits no narrowing mask).
// Defined after func_80220128 so MWCC cannot inline it (retail calls it).
extern "C" void func_802200A8(void* selfp)
{
    u8* base = reinterpret_cast<u8*>(selfp);
    CModelDispMakeCrystal* self = reinterpret_cast<CModelDispMakeCrystal*>(selfp);
    u8 slot = 0;
    u8* entries = *reinterpret_cast<u8**>(base + 0xe1c);
    u8 entryCount = entries[0];
    if (entryCount > 0x20) entryCount = 0x20;
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

// Item-impl vtable used by func_802203D8: methods at +0x58 (3 args incl
// this) and +0x94 (3 args). MWCC reserves 2 hidden RTTI slots, so declared
// indices 20 and 35 hit those slots.
struct CMCItemImplVt {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m0A(); virtual void m0B();
    virtual void m0C(); virtual void m0D(); virtual void m0E(); virtual void m0F();
    virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13();
    virtual void m58(void* item, u32 n);   // declared index 20 => +0x58
    virtual void m14(); virtual void m15(); virtual void m16(); virtual void m17();
    virtual void m18(); virtual void m19(); virtual void m1A(); virtual void m1B();
    virtual void m1C(); virtual void m1D(); virtual void m1E(); virtual void m1F();
    virtual void m20(); virtual void m21(); virtual void m22(); virtual void m23();
    virtual void m94(void* item, u32 v);   // declared index 35 => +0x94
};

// Retail 0x80222230: cylinder-count finalize - resolve the item-source row
// for the current char, build the item record, compute the display count
// from the bdat columns, and append the step(s) to the step list.
void func_802203D8(void* selfp)
{
    u8* base = reinterpret_cast<u8*>(selfp);
    u8* entries = *reinterpret_cast<u8**>(base + 0xe1c);
    if (entries == nullptr) return;
    void* g1 = lbl_eu_806640EC;
    u16 n = *(u16*)(entries + 2);
    u16 count = *(u16*)(entries + 4);
    u8 ch = entries[1];
    // Walk the bdat enum rows backwards to find the row whose column 3
    // value is 3 (the row key for this item source).
    u16 idx = (u16)(func_8003B1EC(g1) - 1);
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
    func_80159F6C(&item, idx, n, tier);
    reinterpret_cast<CMCItemImplVt*>(CItem_initItemImplInstances(&item))->m58(&item, n);
    void* g2 = lbl_eu_806640D8;
    u32 first = 0;
    u32 second = 0;
    // Per-arm string-table base: one address materialization shared by both
    // column lookups (retail keeps the base in r28 across the pair).
    switch (tier) {
    case 1: {
        const char* t = &lbl_eu_805090FC[0];
        first = func_801361E8(reinterpret_cast<u32>(g2), t + 0x71, n);
        second = func_801361E8(reinterpret_cast<u32>(g2), t + 0x79, n);
        break;
    }
    case 2: {
        const char* t = &lbl_eu_805090FC[0];
        first = func_801361E8(reinterpret_cast<u32>(g2), t + 0x81, n);
        second = func_801361E8(reinterpret_cast<u32>(g2), t + 0x89, n);
        break;
    }
    case 3: {
        const char* t = &lbl_eu_805090FC[0];
        first = func_801361E8(reinterpret_cast<u32>(g2), t + 0x91, n);
        second = func_801361E8(reinterpret_cast<u32>(g2), t + 0x99, n);
        break;
    }
    case 4: {
        const char* t = &lbl_eu_805090FC[0];
        first = func_801361E8(reinterpret_cast<u32>(g2), t + 0xa1, n);
        second = func_801361E8(reinterpret_cast<u32>(g2), t + 0xa9, n);
        break;
    }
    case 5: {
        const char* t = &lbl_eu_805090FC[0];
        first = func_801361E8(reinterpret_cast<u32>(g2), t + 0xb1, n);
        second = func_801361E8(reinterpret_cast<u32>(g2), t + 0xb9, n);
        break;
    }
    case 6: {
        const char* t = &lbl_eu_805090FC[0];
        first = func_801361E8(reinterpret_cast<u32>(g2), t + 0xc1, n);
        second = func_801361E8(reinterpret_cast<u32>(g2), t + 0xc9, n);
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
    reinterpret_cast<CMCItemImplVt*>(CItem_initItemImplInstances(&item))->m94(&item, result);
    // Append the step record (stride 0x34 at +0x13c0) the requested times.
    for (u8 k = 0; k < copyCount; k++) {
        u8 stepN = base[0x2dc0];
        base[0x2dc0] = (u8)(stepN + 1);
        func_8015704C(base + 0x13c0 + stepN * 0x34, &item);
    }
    func_8021E840(entries, 0);
    func_8021E888(entries);
    func_8021FC28(reinterpret_cast<CModelDispMakeCrystal*>(selfp), 0);
    func_801F36BC(base + 0xe38, 8, entries[0]);
    func_802232E4(base + 0xc18);
}

// Retail 0x802225D4: teardown of the make-crystal UI state. Waits for the
// draw engine, releases the file handles, frees the crystal-box file state
// and display buffers, then destroys the per-slot crystal display substructs
// and every effect sub-object.
void func_8022077C(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (base[0x2dd4] == 0) {
        CDeviceVI::waitForDrawDone();
        func_801390E0(reinterpret_cast<CFileHandle**>(base + 0x18));
        func_801390E0(reinterpret_cast<CFileHandle**>(base + 0x24));
        void* arc = &lbl_eu_8065FC18[0];
        if (arc != nullptr) {
            // Release the file-state machine's object references.
            if (*reinterpret_cast<u32*>(base + 0x10) != 0) {
                func_804E3CCC(*reinterpret_cast<void**>(base + 0x10));
            }
            if (*reinterpret_cast<u32*>(base + 0x14) != 0) {
                func_804E3CCC(*reinterpret_cast<void**>(base + 0x14));
            }
            if (*reinterpret_cast<u32*>(base + 0x1c) != 0) {
                func_804CC1D8(arc, *reinterpret_cast<void**>(base + 0x1c));
            }
        }
        base[0x10] = 0;
        base[0x14] = 0;
        if (*reinterpret_cast<u32*>(base + 0x1c) != 0) {
            mtl::MemManager::deallocate(*reinterpret_cast<void**>(base + 0x1c));
            base[0x1c] = 0;
        }
        if (*reinterpret_cast<u32*>(base + 0x20) != 0) {
            func_80495E60(*reinterpret_cast<void**>(base + 0x20));
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
                func_801390E0(reinterpret_cast<CFileHandle**>(sub + (u32)j * 0xc + 0x564));
            }
        }
        // Release the built display models for both slots.
        for (u8 i = 0; i < 2; i++) {
            func_8021FB68(self, base + (u32)i * 0x5cc + 0x44);
        }
        // Destroy the gauge, list, support, and every effect sub-object.
        reinterpret_cast<CMCCylinderGauge*>(base + 0xbec)->func_80222090();
        __dt__80222984(base + 0xc18);
        func_8022EA04(base + 0xc70);
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
        reinterpret_cast<CMCE20Fn*>(base + 0xe20)->m1();
        func_801F35DC(base + 0xe38);
        base[0x2dd4] = 1;
    }
}

// Retail 0x802227AC: crystal-item spawn dispatcher - picks a crystal object
// by the selection code (jumptable on sel), computes a position offset
// (random sign for the random cases, char-dependent slot for others), and
// spawns it through the vision helper func_8004392C.
extern "C" void func_80220954(void* selfp, int sel, u8 chIn)
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
        ch = func_801392B4(base[0x5f0 + s * 0x5cc]);
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
        ch = func_801392B4(base[0x5f0 + s * 0x5cc]);
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
        ch = func_801392B4(base[0x5f0 + s * 0x5cc]);
        ml::MTRand::getInstance();
        r = rand31__Q22ml6MTRandFv();
        s = (s8)(((r & 1) ^ (r >> 31)) - (r >> 31));
        off = (u16)(0xe1e - (s == 1));
        break;
    case 7:
        ml::MTRand::getInstance();
        r = rand31__Q22ml6MTRandFv();
        s = (s8)(((r & 1) ^ (r >> 31)) - (r >> 31));
        ch = func_801392B4(base[0x5f0 + s * 0x5cc]);
        ml::MTRand::getInstance();
        r = rand31__Q22ml6MTRandFv();
        s = (s8)(((r & 1) ^ (r >> 31)) - (r >> 31));
        off = (u16)(0xe16 - (s == 1));
        break;
    }
    func_8004392C((u8)ch, off, (u32)getHandleMEM2__Q23mtl10MemManagerFv(), 2, 1,
                  lbl_eu_806684A0);
}

// Virtual dispatch at vtable offset +0xA8 (index 0x2a), returns a pointer.
// MWCC reserves 2 hidden vtable slots (RTTI): declared index 0x28 => +0xA8.
struct CMCVtA8 {
    virtual void* m00(); virtual void* m01(); virtual void* m02(); virtual void* m03();
    virtual void* m04(); virtual void* m05(); virtual void* m06(); virtual void* m07();
    virtual void* m08(); virtual void* m09(); virtual void* m0A(); virtual void* m0B();
    virtual void* m0C(); virtual void* m0D(); virtual void* m0E(); virtual void* m0F();
    virtual void* m10(); virtual void* m11(); virtual void* m12(); virtual void* m13();
    virtual void* m14(); virtual void* m15(); virtual void* m16(); virtual void* m17();
    virtual void* m18(); virtual void* m19(); virtual void* m1A(); virtual void* m1B();
    virtual void* m1C(); virtual void* m1D(); virtual void* m1E(); virtual void* m1F();
    virtual void* m20(); virtual void* m21(); virtual void* m22(); virtual void* m23();
    virtual void* m24(); virtual void* m25(); virtual void* m26(); virtual void* m27();
    virtual void* m28();  // declared index 0x28 => +0xA8
};

void func_80220C34(CModelDispMakeCrystal* self)
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
            func_80495E8C(*reinterpret_cast<void**>(base + 0xc),
                          *reinterpret_cast<u32*>(base + 0x28), -1, 1);
        func_80485684(*reinterpret_cast<void**>(base + 0x20), 1);
        func_80482DF4(*reinterpret_cast<void**>(base + 0x20), 1);
        reinterpret_cast<CMCVt48*>(*reinterpret_cast<void**>(base + 0x20))->m16(
            *reinterpret_cast<f32*>(base + 0xbe0));
        void* p1 = func_8048315C(*reinterpret_cast<void**>(base + 0x20));
        u8* s1 = reinterpret_cast<u8*>(lbl_eu_80576658);
        u8* o1 = reinterpret_cast<u8*>(p1);
        *reinterpret_cast<u32*>(o1 + 0x0) = *reinterpret_cast<u32*>(s1 + 0x0);
        *reinterpret_cast<u32*>(o1 + 0x4) = *reinterpret_cast<u32*>(s1 + 0x4);
        *reinterpret_cast<u32*>(o1 + 0x8) = *reinterpret_cast<u32*>(s1 + 0x8);
        void* p2 = func_8048315C(*reinterpret_cast<void**>(base + 0x20));
        u8* s2 = reinterpret_cast<u8*>(lbl_eu_80576664);
        u8* o2 = reinterpret_cast<u8*>(p2);
        *reinterpret_cast<u32*>(o2 + 0xc) = *reinterpret_cast<u32*>(s2 + 0x0);
        *reinterpret_cast<u32*>(o2 + 0x10) = *reinterpret_cast<u32*>(s2 + 0x4);
        *reinterpret_cast<u32*>(o2 + 0x14) = *reinterpret_cast<u32*>(s2 + 0x8);
        void* arc = &lbl_eu_8065FC18[0];
        if (arc != nullptr) {
            void* e = func_804CC1F4(arc, *reinterpret_cast<void**>(base + 0x1c),
                                    *reinterpret_cast<void**>(base + 0xc), 0, 1, 1);
            *reinterpret_cast<void**>(base + 0x10) = e;
            if (e != nullptr) {
                func_804E3D0C(e, self ? reinterpret_cast<void*>(base + 0x8) : nullptr);
                void* r = reinterpret_cast<CMCVtA8*>(*reinterpret_cast<void**>(base + 0x20))->m28();
                *reinterpret_cast<u32*>(reinterpret_cast<u8*>(*reinterpret_cast<void**>(base + 0x10)) + 0x14) =
                    reinterpret_cast<u32>(r);
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
                    func_80220E14(self, reinterpret_cast<CMCrystalDispSub*>(s + 0x44));
                } else {
                    func_802211CC(self, s + 0x44);
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

void func_80220E14(CModelDispMakeCrystal* self, CMCrystalDispSub* sub)
{
    // Copy the 3 state-specific enum filter types first (retail statement order).
    const CMCryFilters* filt = reinterpret_cast<const CMCryFilters*>(lbl_eu_805090D8);
    u32 filters[3];
    filters[0] = filt->v0;
    filters[1] = filt->v1;
    filters[2] = filt->v2;

    CMCryListHolder holder;
    func_80043D90(&holder);

    // Select the crystal list for the current char state and check it's loaded.
    func_800F4A98(func_80043F18(&holder), filters[(s8)sub->field_5ac], 0);
    if (*reinterpret_cast<u32*>(reinterpret_cast<u8*>(func_80043F18(&holder)) + 0x620) == 0) {
        func_8021FB68(self, reinterpret_cast<u8*>(sub));
        __dt__80043E88(&holder, -1);
        return;
    }

    // Grab the first crystal object from the enum list.
    void* cfMove = *reinterpret_cast<void**>(
        reinterpret_cast<u8*>(func_800F6EC0(func_80043F18(&holder), 0)) + 4);
    if (cfMove == nullptr) {
        func_8021FB68(self, reinterpret_cast<u8*>(sub));
        __dt__80043E88(&holder, -1);
        return;
    }

    CMCCryChgActor* actor =
        reinterpret_cast<CMCCryChgActor*>(func_800BFC68__FPQ22cf12CfObjectMove(cfMove));
    int ready = actor != nullptr ? 1 : 0;
    u32 crystalCount = 0;
    if (actor != nullptr) {
        crystalCount = actor->field_3f2c;
        if (crystalCount == 0) ready = 0;
        if (reinterpret_cast<CMCryMoveVt*>(&actor->move)->m74() == 0) ready = 0;
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
        sub->field_00 = func_80495E8C(objs->field_0c, crystalCount, -1, 1);
        sub->mCrystalVals[1] =
            (static_cast<u32>(reinterpret_cast<CMCryMoveVt*>(&actor->move)->m82(1)) >> 12) &
            0x3ff;
        s16 be = func_800BE954(&actor->move);
        CMCCryParamSlot* params =
            reinterpret_cast<CMCCryParamSlot*>(func_80062C28(be, 0));
        // Crystal attachment points 2..5; do-while keeps the retail loop shape.
        u8 idx = 2;
        do {
            if (reinterpret_cast<CMCryMoveVt*>(&actor->move)->m82(idx) != 0) {
                void* obj = params[idx].field_2c->m02();
                func_804831C4(sub->field_00, obj);
                sub->mCrystalVals[idx] =
                    (static_cast<u32>(reinterpret_cast<CMCryMoveVt*>(&actor->move)->m82(idx)) >>
                     12) &
                    0x3ff;
            }
        } while (++idx <= 5);
        u32 handle = sub->field_5a4;
        sub->field_04 = func_800584B8(objs->field_0c, handle, &lbl_eu_805090FC[0xef]);
        sub->mAnim.m36();
        func_8004B624(&sub->mAnim, sub->field_00, sub->field_04, handle);
        sub->field_14 |= 0x160;
        func_80200388(&sub->mAnim,
                      self ? reinterpret_cast<void*>(reinterpret_cast<u8*>(self) + 4)
                           : reinterpret_cast<void*>(self));
        u32* posA = reinterpret_cast<u32*>(func_8048315C(sub->field_00));
        posA[0] = sub->field_5b0;
        posA[1] = sub->field_5b4;
        posA[2] = sub->field_5b8;
        u32* posB = reinterpret_cast<u32*>(func_8048315C(sub->field_00));
        posB[3] = sub->field_5bc;
        posB[4] = sub->field_5c0;
        posB[5] = sub->field_5c4;
        reinterpret_cast<CMCModelVt*>(sub->field_00)->m12(objs->field_be0);
        reinterpret_cast<CMCModelVt*>(sub->field_00)->m27(3, 0);
        if (sub->field_00 != nullptr) {
            func_8004B9D4(&sub->mAnim, 0x21, 0, -1, 0);
        }
        sub->field_5c8 = 1;
    } else if (sub->field_00 != nullptr && ready == 0) {
        func_8021FB68(self, reinterpret_cast<u8*>(sub));
    }

    __dt__80043E88(&holder, -1);
}

// Retail 0x80223018: crystal display sub-struct release/rebuild path (state
// >= 3). Reload the crystal values from the char data, re-arm any missing
// crystal buffers, then rebuild the display model exactly like the initial
// build path (shared tail with func_80220E14).
void func_802211CC(CModelDispMakeCrystal* self, u8* subp)
{
    CMCrystalDispSub* sub = reinterpret_cast<CMCrystalDispSub*>(subp);
    if (sub->field_00 != 0) return;
    func_8009ECB0();
    u8 ch = (u8)func_801392B4((u8)sub->field_5ac);
    if (ch == 0) return;
    u8* data = reinterpret_cast<u8*>(func_8009EC9C(ch));
    sub->mCrystalVals[1] = func_80141E90(ch, (s16)(u16)*(u16*)(data + 0xe), 1, 0);
    sub->mCrystalVals[2] = func_80141E90(ch, (s16)(u16)*(u16*)(data + 0x10), 2, 0);
    sub->mCrystalVals[3] = func_80141E90(ch, (s16)(u16)*(u16*)(data + 0x12), 3, 0);
    sub->mCrystalVals[4] = func_80141E90(ch, (s16)(u16)*(u16*)(data + 0x14), 4, 0);
    sub->mCrystalVals[5] = func_80141E90(ch, (s16)(u16)*(u16*)(data + 0x16), 5, 0);

    // Re-arm the 5 crystal buffers (slot stride 0xc at +0x568): each value
    // still set and not already loaded is re-requested; if any buffer was
    // (re)loaded, the model must be rebuilt afterwards.
    int anyLoaded = 1;
    for (u32 i = 1; i <= 5; i++) {
        if (*(u32*)(subp + 0x544 + i * 4) != 0) {
            char pathBuf[96];
            u32 outTag = 0;
            pathBuf[0] = 0;
            if (func_800AA33C(pathBuf, *(u32*)(subp + 0x544 + i * 4), 1, 1) != 0) {
                u8* slot = subp + 0x568 + (i - 1) * 0xc;
                if (*reinterpret_cast<u32*>(slot) == 0) {
                    anyLoaded = 0;
                    if (func_80062A00() != 0) {
                        slot[4] = 1;
                        *reinterpret_cast<u32*>(slot) =
                            reinterpret_cast<u32>(func_80062AD8(*(u32*)(subp + 0x544 + i * 4), &outTag));
                        if (outTag == 0xffffffff) {
                            func_8007E038__Q22cf13CfGameManagerFv(ch, false);
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
        // with func_80220E14's initial build).
        sub->field_00 = func_80495E8C(
            *reinterpret_cast<void**>(reinterpret_cast<u8*>(self) + 0xc),
            *reinterpret_cast<u32*>(subp + 0x568), -1, 1);
        func_80485684(sub->field_00, 1);
        func_80482DF4(sub->field_00, 1);
        if (sub->field_00 == 0) return;
        func_804831C4(sub->field_00, *reinterpret_cast<void**>(subp + 0x574));
        func_804831C4(sub->field_00, *reinterpret_cast<void**>(subp + 0x580));
        func_804831C4(sub->field_00, *reinterpret_cast<void**>(subp + 0x58c));
        func_804831C4(sub->field_00, *reinterpret_cast<void**>(subp + 0x598));
        u32 f5a4 = sub->field_5a4;
        sub->field_04 = func_800584B8(
            *reinterpret_cast<void**>(reinterpret_cast<u8*>(self) + 0xc), f5a4,
            &lbl_eu_805090FC[0xef]);
        reinterpret_cast<CMCVtE0*>(subp + 0x8)->m36();
        func_8004B624(subp + 0x8, sub->field_00, sub->field_04, f5a4);
        sub->field_14 |= 0x160;
        func_80200388(
            subp + 0x8,
            self ? reinterpret_cast<void*>(reinterpret_cast<u8*>(self) + 4)
                 : reinterpret_cast<void*>(self));
        u32* g0 = reinterpret_cast<u32*>(func_8048315C(sub->field_00));
        g0[0] = sub->field_5b0;
        g0[1] = sub->field_5b4;
        g0[2] = sub->field_5b8;
        u32* g1 = reinterpret_cast<u32*>(func_8048315C(sub->field_00));
        g1[3] = sub->field_5bc;
        g1[4] = sub->field_5c0;
        g1[5] = sub->field_5c4;
        reinterpret_cast<CMCModelVt*>(sub->field_00)->m12(
            *reinterpret_cast<f32*>(reinterpret_cast<u8*>(self) + 0xbe0));
        reinterpret_cast<CMCModelVt*>(sub->field_00)->m27(3, 0);
        if (sub->field_00 != 0) {
            func_8004B9D4(subp + 0x8, 0x21, 0, -1, 0);
        }
        sub->field_5c8 = 1;
    }
}

// Virtual dispatch at vtable offset +0xC on the embedded effect objects
// (declared index 1, MWCC adds 2 hidden RTTI slots).
struct CMCEffInitVt {
    virtual void m0();
    virtual void m1();  // declared index 1 => +0xC
};

// Virtual dispatch at vtable offset +0x8 (declared index 0).
struct CMCCur18Vt {
    virtual void m0();  // declared index 0 => +0x8
};

// Retail 0x8022338C: async file-event handler for the make-crystal UI load.
// Dispatch on which pending readFile handle completed: the crystal-list file
// (this+0x24), the item-source file (this+0x18), or the layout archive
// (this+0x3c) which drives the whole sub-object construction; anything else
// is a per-slot crystal model file.
int CModelDispMakeCrystal::OnFileEvent(CEventFile* ev)
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
        if (arc != nullptr) func_804CC1BC(arc);
        *reinterpret_cast<void**>(base + 0x18) = 0;
        return 1;
    }
    // Case 3: the layout archive completed - build every sub-object.
    void* f3c = *reinterpret_cast<void**>(base + 0x3c);
    if (f3c == ev->mFileHandle) {
        // Stack scratch, declaration order matches the retail frame
        // (cls858@0x8 .. effUpPrm@0x1b0).
        u8 cls858[8];
        u8 cur2[0x18];
        u8 cur1[0x18];
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
        func_80434A4C__Q23mtl10MemManagerFb(false);
        func_80434A4C__Q23mtl10MemManagerFb(false);
        *reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40) =
            reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                createArcResourceAccessor__10CLibLayoutFv());
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
        func_80221EF4__16CMCCylinderGaugeFv(base + 0xbec);
        // Crystal support: same build/copy/destroy pattern.
        __ct__CMCCrystalSupport(
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
        func_8022E8F8(base + 0xc70);
        // Effect objects: build on stack, install via func_80211CB8, destroy.
        __ct__CMCEffStart(
            effStart,
            *reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40));
        func_80211CB8(base + 0xc8c, effStart);
        __dt__11CMCEffStartFv(effStart, -1);
        reinterpret_cast<CMCEffInitVt*>(base + 0xc8c)->m1();
        __ct__CMCEffUpRed(
            effUpRed,
            *reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40));
        func_80211CB8(base + 0xca4, effUpRed);
        __dt__11CMCEffUpRedFv(effUpRed, -1);
        reinterpret_cast<CMCEffInitVt*>(base + 0xca4)->m1();
        __ct__CMCEffUpBlue(
            effUpBlue,
            *reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40));
        func_80211CB8(base + 0xcbc, effUpBlue);
        __dt__12CMCEffUpBlueFv(effUpBlue, -1);
        reinterpret_cast<CMCEffInitVt*>(base + 0xcbc)->m1();
        __ct__CMCEffUpGreen(
            effUpGreen,
            *reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40));
        func_80211CB8(base + 0xcd4, effUpGreen);
        __dt__13CMCEffUpGreenFv(effUpGreen, -1);
        reinterpret_cast<CMCEffInitVt*>(base + 0xcd4)->m1();
        // UpPrm: larger copy with an 8-byte-per-iteration counted loop
        // (same lwz/lwzu + stw/stwu pair idiom as the item-box tables).
        __ct__CMCEffUpPrm(
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
        __ct__CMCEffSuccess(
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
        reinterpret_cast<CMCEffInitVt*>(base + 0xd5c)->m1();
        __ct__CMCEffFailure(
            effFailure,
            *reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40));
        func_80211CB8(base + 0xd78, effFailure);
        __dt__13CMCEffFailureFv(effFailure, -1);
        reinterpret_cast<CMCEffInitVt*>(base + 0xd78)->m1();
        __ct__CMCEffCrystal(
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
        reinterpret_cast<CMCEffInitVt*>(base + 0xd90)->m1();
        __ct__CMCEffUpRank(
            effUpRank,
            *reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40));
        func_80211CB8(base + 0xdcc, effUpRank);
        __dt__12CMCEffUpRankFv(effUpRank, -1);
        reinterpret_cast<CMCEffInitVt*>(base + 0xdcc)->m1();
        __ct__CMCEffDivide(
            effDivide,
            *reinterpret_cast<nw4r::lyt::ArcResourceAccessor**>(base + 0x40));
        func_80211CB8(base + 0xde4, effDivide);
        __dt__12CMCEffDivideFv(effDivide, -1);
        reinterpret_cast<CMCEffInitVt*>(base + 0xde4)->m1();
        __ct__CMCEffCylinder(
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
        reinterpret_cast<CMCEffInitVt*>(base + 0xdfc)->m1();
        // Cursor objects: build on stack, install via func_8018B0FC, destroy.
        __ct__6CCur18Fv(cur1, func_801355F4());
        func_8018B0FC(base + 0xe20, cur1);
        __dt__6CCur18Fv(cur1, -1);
        reinterpret_cast<CMCCur18Vt*>(base + 0xe20)->m0();
        __ct__6CCur18Fv(cur2, func_801355F4());
        func_8018B0FC(base + 0xeb4, cur2);
        __dt__6CCur18Fv(cur2, -1);
        reinterpret_cast<CMCCur18Vt*>(base + 0xeb4)->m0();
        *reinterpret_cast<void**>(base + 0x3c) = 0;
        func_8045F810__17UnkClass_8045F564Fv(base + 0x2c);
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

#pragma optimize_for_size on
void func_80221B90(CModelDispMakeCrystal* self, u8 r4, u8 r5)
{
    u8* base = reinterpret_cast<u8*>(self);
    func_80297928(base + 0xecc);
    void* m = func_80496264(*reinterpret_cast<void**>(base + 0xc), -1);
    nw4r::math::VEC3 v1;
    func_8004B60C(&v1, lbl_eu_806684A4, lbl_eu_806684A0, lbl_eu_806684A4);
    nw4r::math::VEC3 v2;
    func_8004B60C(&v2, lbl_eu_806684A4, lbl_eu_80668504, lbl_eu_806684CC);
    func_8049EFF8(m, &v2, &v1);
    for (u8 j = 0; j < 2; j++) {
        for (u8 i = 0; i < 6; i++) {
            func_801390E0(reinterpret_cast<CFileHandle**>(
                base + (u32)j * 0x5cc + 0x44 + (u32)i * 0xc + 0x564));
        }
    }
    for (u8 k = 0; k < 2; k++) {
        func_8021FB68(self, base + (u32)k * 0x5cc + 0x44);
    }
    base[0x5f0] = r4;
    base[0xbbc] = r5;
    base[0x60c] = 0;
    base[0xbd8] = 0;
    func_801392B4(r4);
    func_801392B4(r5);
    func_8009D018(0xbd, 0);
    func_8009D018(0xbf, 0);
}
#pragma optimize_for_size off

void CModelDispMakeCrystal::setCrystalCursor(u32 val) { *(u32*)((u8*)this + 0xE1C) = val; }

int func_80221CD0(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (CSysWin_getUnk34(base + 0xe78)) return 0;
    u8 state = base[0xbdd];
    if ((u8)(state + 0xee) <= 1) return 0x38;
    if (state == 0x18 || state == 0x1f)
        return func_80298850(base + 0xecc);
    return (state < 0x12) ? 0x3b : 0;
}

// Release a registered anim callback by id: if the id matches the buffered
// object (+0x10 / +0x14), flush it with the anim wrapper (+0x8, or the object
// itself when null) and clear the slot.
void func_80221D58(void* obj, u32 id)
{
    u32 e1 = *reinterpret_cast<u32*>((u8*)obj + 0x10);
    if (id == e1) {
        void* arg = obj;
        if (obj) arg = (u8*)obj + 8;
        func_804E3D48(reinterpret_cast<void*>(e1), arg);
        *reinterpret_cast<u32*>((u8*)obj + 0x10) = 0;
    } else {
        u32 e2 = *reinterpret_cast<u32*>((u8*)obj + 0x14);
        if (id == e2) {
            void* arg = obj;
            if (obj) arg = (u8*)obj + 8;
            func_804E3D48(reinterpret_cast<void*>(e2), arg);
            *reinterpret_cast<u32*>((u8*)obj + 0x14) = 0;
        }
    }
}

// (initCrystalData is a retail hard-symbol; not defined here so it stays a call)

void CModelDispMakeCrystal::destroyCrystalDispThunk4() { ((void(*)(void*))__dt__21CModelDispMakeCrystalFv)((char*)this - 0x4); }

void CModelDispMakeCrystal::releaseCrystalDispThunk() { ((void(*)(void*))func_80221D58)((char*)this - 0x8); }

void CModelDispMakeCrystal::destroyCrystalDispThunk8() { ((void(*)(void*))__dt__21CModelDispMakeCrystalFv)((char*)this - 0x8); }

void sinit_80221DDC() {
    initCrystalData(lbl_eu_80664718);
    func_8004B60C(lbl_eu_80576658, lbl_eu_80668508, lbl_eu_806684AC, lbl_eu_8066850C);
    func_8004B60C(lbl_eu_80576664, lbl_eu_806684A4, scaleByGlobal(lbl_eu_80668510), lbl_eu_806684A4);
}
