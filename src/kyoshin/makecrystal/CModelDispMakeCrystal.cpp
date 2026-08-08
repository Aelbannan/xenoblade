// Auto-scaffolded catalog TU for kyoshin/makecrystal/CModelDispMakeCrystal
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/makecrystal/CModelDispMakeCrystal.hpp"

#include "kyoshin/makecrystal/CMCEffStart.hpp"
#include "kyoshin/makecrystal/CMCCylinderGauge.hpp"
#include "kyoshin/makecrystal/CMCGetItemBox.hpp"
#include "kyoshin/makecrystal/CMCCrystalList.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/core/CPadManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfPadData.hpp"
#include "monolib/util/MemManager.hpp"

// forward declarations for scaffold thunk references
void __dt__21CModelDispMakeCrystalFv(void*);
void func_8021FC28(CModelDispMakeCrystal*, u8);
void func_8021FD44(CModelDispMakeCrystal*);
void func_80220E14(CModelDispMakeCrystal*, CMCrystalDispSub*);
void func_802211CC(CModelDispMakeCrystal*, u8*);

// --- external C-linkage helpers and global data for this TU ---
extern "C" {
void* func_8004B60C(void*, f32, f32, f32);
float scaleByGlobal(float val);
void initCrystalData(unsigned char* p);
int CSysWin_isActive(void* self);
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
void* func_804CC1F4(void*, void*, void*, int, int, int);
// Retail member-return types are int (callers `cmpwi r3,0` the raw reg).
// Mangled-name refs let us call them as int-returning without re-mangling.
int isFinished__11CMCEffStartFv(void* self);
int isFinished__13CMCEffCrystalFv(void* self);
int isFinished__14CMCEffCylinderFv(void*);
int func_802220F0__16CMCCylinderGaugeFv(void*);
void func_802203D8(void*);
void func_8022077C(void*);
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
extern "C" u32 lbl_eu_805090D8[]; // 3 state filter values for func_800F4A98
int sprintf(char*, const char*, ...);
void func_80043D90(CMCryListHolder*);
void* func_80043F18(CMCryListHolder*); // returns holder->list
void __dt__80043E88(CMCryListHolder*, int);
void func_800F4A98(void*, unsigned int, unsigned int);
void* func_800F6EC0(void*, unsigned int); // &slot -> +0x4 holds the move ptr
void* func_800BFC68__FPQ22cf12CfObjectMove(void*);
void* func_80062C28(short, int);
short func_800BE954(void*);
void* func_800584B8(void*, unsigned int, const char*);
void func_8004B624(void*, void*, void*, unsigned int);
void func_80200388(void*, void*);
void func_8021E8E4(void*);
void func_80222D9C(void*, u8);
void func_80222B14(void*, u8, u16, u16);
void func_80223004(void*, u8, u16, u8);
void func_80222A60(void*);
void func_80222A84(void*);
void func_80297928(void*);
void* func_80496264(void*, int);
void func_8049EFF8(void*, void*, void*);
void func_801F3670(void*, void*);
void func_801F36BC(void*, u8, u8);
void func_801F367C(void*);
s32 getInstance__Q22ml6MTRandFv();
u32 rand31__Q22ml6MTRandFv();
u16 func_8013A7D0(u8, u8);
void* func_8009EC9C(u8);
int func_8026178C(void*, int);
u8 func_8025FB10(void*, int);
void func_80220128(void*);
void func_8009D018(int, int);
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
extern const f32 lbl_eu_806684F8;
extern const f32 lbl_eu_806684FC;
extern const f32 lbl_eu_80668504;
extern const f32 lbl_eu_806684C4;
extern const f32 lbl_eu_806684C8;
extern "C" u8 lbl_eu_80664718;
extern "C" u8 lbl_eu_80535D90[];
extern u8 lbl_eu_80576658[0x20];
extern u8 lbl_eu_80576664[0x20];
extern u8 lbl_eu_8065FC18[0x10];

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

// Virtual dispatch to a sub-object whose vtable method sits at slot 4 (+0x10)
// taking one argument (used on this+0xe20 in func_8021DF84).
struct CMCE20Fn {
    virtual void m0();
    virtual void m1();
    virtual void m2();
    virtual void m3();
    virtual void m4(const void* arg);
};

// Same slot-4 virtual dispatch but no-arg (used on this+0xc8c in func_8021CE4C).
struct CMCC8CFn {
    virtual void m0();
    virtual void m1();
    virtual void m2();
    virtual void m3();
    virtual void m4();
};

// Virtual dispatch at vtable offset +0x88 (index 34): the CSysWin-dispatch
// call in func_8021C6E4 on this+0xe78.
struct CMCSysWinFn {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m0A(); virtual void m0B();
    virtual void m0C(); virtual void m0D(); virtual void m0E(); virtual void m0F();
    virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13();
    virtual void m14(); virtual void m15(); virtual void m16(); virtual void m17();
    virtual void m18(); virtual void m19(); virtual void m1A(); virtual void m1B();
    virtual void m1C(); virtual void m1D(); virtual void m1E(); virtual void m1F();
    virtual void m20(); virtual void m21(); virtual void m22();  // index 34 => +0x88
};

// Entry at this+0x13c0 (stride 0x34); a 4-bit type at bits 12-15.
struct CMCStep {
    u32 _pad0 : 12;
    u32 type : 4;
    u32 _pad16 : 16;
};

// Virtual dispatch at vtable offset +0xE0 (index 0x38): the flush call in
// func_8021FB68 on the inline object at the sub-struct +0x8.
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
    virtual void m34(); virtual void m35(); virtual void m36(); virtual void m37();
    virtual void m38();  // index 0x38 => +0xE0
};

void __ct__CModelDispMakeCrystal(){}

// Virtual dispatch at vtable offset +0x48 (index 18), taking one float arg.
// Used on this+0x20 and on the per-slot objects at this+0x44 (stride 0x5cc).
struct CMCVt48 {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m0A(); virtual void m0B();
    virtual void m0C(); virtual void m0D(); virtual void m0E(); virtual void m0F();
    virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13();
    virtual void m14(); virtual void m15(); virtual void m16(); virtual void m17();
    virtual void m18(float f1);  // index 18 => +0x48
};

void CModelDispMakeCrystal::initCrystalSubStruct() {
    *(unsigned long*)this = 0;
    *(unsigned long*)((char*)this + 4) = 0;
    ((unsigned char*)this)[8] = 0;
}

void func_8021C4F0(){}

void __dt__8021C540(void*, int) {}

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
    reinterpret_cast<CMCSysWinFn*>(base + 0xe78)->m22();
    base[0xbe9] = 0;
    base[0xbdd] = 0;
    base[0x2dd2] = 0;
    base[0x2dd4] = 0;
}

void func_8021C7A0(){}

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


void func_8021C928(){}

u8 CModelDispMakeCrystal::getCrystalStateA() { return reinterpret_cast<CModelDispMakeCrystalFull*>(this)->field_BE8; }

void func_8021CA3C(){}

u8 CModelDispMakeCrystal::getCrystalStateB() { return reinterpret_cast<CModelDispMakeCrystalFull*>(this)->field_2DC3; }

u8 CModelDispMakeCrystal::getCrystalStateC() { return reinterpret_cast<CModelDispMakeCrystalFull*>(this)->field_2DD2; }

void func_8021CB20(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    if (base[0xbdc]) return;
    base[0xbdc] = 1;
    base[0xbe8] = 0;
    u8 r0 = func_801392B4(base[0x5f0]);
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
    func_8021FC28(self, 0);
    u8 r30 = base[0xbbc];
    func_801392B4(base[0x5f0]);
    u8 bbc = func_801392B4(r30);
    u8* gl = &lbl_eu_80664718;
    gl[3] = 0;
    u8* row = &lbl_eu_80535D90[(bbc - 1) * 3];
    gl[0] = row[0];
    gl[1] = row[1];
    gl[2] = row[2];
    gl[5] = 1;
    func_8021FD44(self);
}

void func_8021CC60(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    f32* fbe4 = reinterpret_cast<f32*>(base + 0xbe4);
    f32* fbe0 = reinterpret_cast<f32*>(base + 0xbe0);
    f32 v4 = *fbe4 + lbl_eu_806684A0;
    *fbe4 = v4;
    if (v4 < lbl_eu_806684CC) return;
    f32 v0 = *fbe0 - lbl_eu_806684D0;
    *fbe0 = v0;
    if (v0 >= lbl_eu_806684A4) goto L_EB70;
    *fbe0 = lbl_eu_806684A4;
    base[0xbdc] = 2;
    base[0xbe8] = 1;
    for (u8 i = 0; i < 8; i++) {
        nw4r::math::VEC3 v;
        func_80222F64(&v, base + 0xc18, i);
        reinterpret_cast<CMCEffUpPrm*>(base + 0xcec)->setTranslate(i, v);
    }
    func_80138078__FUl(0x3d);
L_EB70:
    void* p20 = *reinterpret_cast<void**>(base + 0x20);
    if (p20) {
        reinterpret_cast<CMCVt48*>(p20)->m18(*fbe0);
    }
    for (u8 i = 0; i < 2; i++) {
        void* p = *reinterpret_cast<void**>(base + (u32)i * 0x5cc + 0x44);
        if (p) {
            reinterpret_cast<CMCVt48*>(p)->m18(*fbe0);
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

void func_8021CE4C(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    for (u8 i = 0; i < 2; i++) {
        u32 off = (u32)i * 0x5cc;
        if (*reinterpret_cast<u32*>(base + off + 0x44)) {
            func_8004B9D4(base + off + 0x4c, 0x22, 0, -1, 0);
        }
    }
    reinterpret_cast<CMCC8CFn*>(base + 0xc8c)->m4();
    base[0xbdd] = 0x1;
    func_80220954(self, 0, func_801392B4(base[0xbbc]));
}

void func_8021CEF0(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    f32* vel = reinterpret_cast<f32*>(base + 0x2dcc);
    *vel += lbl_eu_806684A0;
    if (isFinished__11CMCEffStartFv(base + 0xc8c)) {
        if (!(*vel < lbl_eu_806684D8)) {
            *vel = lbl_eu_806684A4;
            for (u8 i = 0; i < 2; i++) {
                u32 off = (u32)i * 0x5cc;
                if (*reinterpret_cast<u32*>(base + off + 0x44)) {
                    func_8004B9D4(base + off + 0x4c, 0x21, 0, -1, 0);
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
    if (*reinterpret_cast<u32*>(base + 0x610)) {
        func_8004B9D4(base + 0x618, 0x23, 0, -1, 0);
    }
    u8* gl = &lbl_eu_80664718;
    if (gl[5]) {
        u8 n31 = gl[3];
        u8 n29 = gl[1];
        u8 n28 = gl[0];
        s8 roll = (s8)(rand31__Q22ml6MTRandFv() % 100);
        s8 new3;
        if (roll < (s8)n28) {
            new3 = 0;
        } else {
            // bin the roll by cumulative thresholds; yields 1 or 2
            new3 = (s8)(1 + (roll >= (s8)((s8)n28 + (s8)n29)));
        }
        gl[3] = (u8)new3;        if ((u8)new3 == n31) {
            gl[4]++;
        } else {
            gl[4] = 0;
        }
    }
    if (*reinterpret_cast<u32*>(base + 0x10)) {
        u8* p = reinterpret_cast<u8*>(*reinterpret_cast<u32*>(base + 0x10));
        switch (gl[3]) {
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
    if (v >= lbl_eu_806684DC) {
        u32 f44 = *reinterpret_cast<u32*>(base + 0x44);
        *vel = lbl_eu_806684A4;
        if (f44) {
            func_8004B9D4(base + 0x4c, 0x24, 0, -1, 0);
        }
        base[0xbdd] = 0x4;
        func_80220954(self, 1, func_801392B4(base[0x5f0]));
    }
}

void func_8021D200(){}

void func_8021D3E4(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    f32* f2dcc = reinterpret_cast<f32*>(base + 0x2dcc);
    f32 v = *f2dcc + lbl_eu_806684A0;
    *f2dcc = v;
    if (v < lbl_eu_806684DC) return;
    *f2dcc = lbl_eu_806684A4;
    base[0x13bf]++;
    (&lbl_eu_80664718)[4]++;
    func_80220128(self);
    u8 r0 = base[0x13bf];
    if (r0 >= 0xa) {
        // Tuning/failback step exhausted: jump to failure cleanup.
        u32 p = *reinterpret_cast<u32*>(base + 0x14);
        base[0xbdd] = 6;
        base[0x13bf] = 0;
        if (p) reinterpret_cast<u8*>(p)[0x59] = 1;
        for (u8 i = 0; i < 2; i++) {
            if (*reinterpret_cast<u32*>(base + (u32)i * 0x5cc + 0x44)) {
                func_8004B9D4(base + (u32)i * 0x5cc + 0x4c, 0x21, 0, -1, 0);
            }
        }
    } else if (r0 >= 3) {
        // Random (1/3) chance to fail the tuning step.
        s8 rem = (s8)(rand31__Q22ml6MTRandFv() % 10);
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
        if ((s8)(rand31__Q22ml6MTRandFv() % 5) == 0) r30 = 1;
    }
    if (r30 == 0) {
        u8 r56 = func_801392B4(base[0x5f0]);
        s8 delta = (s8)(rand31__Q22ml6MTRandFv() % 11) + 0x14;
        if (r56 == 6) delta = (s8)(delta * 2);
        s16 nv = *reinterpret_cast<s16*>(base + 0x2dc4) - delta;
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

void func_8021D6B4(){}

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
            if (*reinterpret_cast<u32*>(base + (u32)i * 0x5cc + 0x44)) {
                func_8004B9D4(base + (u32)i * 0x5cc + 0x4c, 0x26, 0, -1, 0);
            }
        }
        func_80138078__FUl(0x91);
        if (r31 < 0xc8) {
            func_80220954(self, 5, 0);
        } else {
            func_80220954(self, 6, 0);
        }
    } else {
        // Failure: crystal count too low.
        base[0xbdd] = 0xa;
        reinterpret_cast<CMCC8CFn*>(base + 0xd78)->m4();
        for (u8 i = 0; i < 2; i++) {
            if (*reinterpret_cast<u32*>(base + (u32)i * 0x5cc + 0x44)) {
                func_8004B9D4(base + (u32)i * 0x5cc + 0x4c, 0x27, 0, -1, 0);
            }
        }
        func_80138078__FUl(0xb2);
    }
}

void func_8021DAF4(){}

void func_8021DB3C(CModelDispMakeCrystal* self) {
    u8* base = reinterpret_cast<u8*>(self);
    *(reinterpret_cast<f32*>(base + 0x2dcc)) += lbl_eu_806684A0;
    if (isFinished__11CMCEffStartFv(base + 0xd78)) {
        *(reinterpret_cast<f32*>(base + 0x2dcc)) = lbl_eu_806684A4;
        base[0xbdd] = 0x10;
        reinterpret_cast<CMCEffSuccess*>(base + 0xd5c)->startOut();
    }
}


void func_8021DB9C(){}

void func_8021DC1C(){}

void func_8021DD0C(){}

void func_8021DE20(){}

void func_8021DE7C(){}

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
    if (isFinished__14CMCEffCylinderFv(base + 0xdfc) == 0) return;
    if (func_802220F0__16CMCCylinderGaugeFv(base + 0xbec) != 0) goto L_block;
    return;
L_block:
    base[0xbdd] = 0x12;
    func_801D216C(base + 0xe20, 1);
    u8 buf[16];
    func_80222F64(buf, base + 0xc18, base[0x2dc1]);
    reinterpret_cast<CMCE20Fn*>(base + 0xe20)->m4(buf);
    func_80223334(base + 0xc18);
}

void func_8021E014(){}

void func_8021E5C0(){}

void func_8021E840(){}

void func_8021E888(){}

void func_8021E8E4(){}

void func_8021EAB8(){}

void func_8021EB00(){}

void func_8021EC04(){}

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
                char* a = func_80136190(&lbl_eu_805090FC[0x47], &lbl_eu_805090FC[0x52], 2);
                char* b = func_80136190(&lbl_eu_805090FC[0x47], &lbl_eu_805090FC[0x52], 4);
                char* c = func_80136190(&lbl_eu_805090FC[0x47], &lbl_eu_805090FC[0x52], 5);
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
    for (u8 i = 0; i < count; i++) {
        CMCStep* p =
            reinterpret_cast<CMCStep*>(base + (u32)i * 0x34 + 0x13c0);
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


void func_8021F058(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();
    // Cursor step / cancel booleans derived from turbo and pressed button flags.
    // Bit positions differ between Classic controller and Wiimote/Nunchuk.
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
        u8 buf[16];
        func_8022C1B4(buf, base + 0xe78, param);
        reinterpret_cast<CMCWinFn*>(base + 0xeb4)->m2(buf);
        func_80138078__FUl(1);
    } else if (dir) {
        // Cursor forward: increment step (resetting to 0).
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

void func_8021F214(){}

void func_8021F290(){}

void func_8021F2D8(CModelDispMakeCrystal* self)
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
                base[0xbdd] = 0x20;
                func_802985B4(base + 0xecc);
                func_80297E18(base + 0xecc);
                func_80138078__FUl(3);
            } else if (r == 2) {
                // Jump to gauge-tuning state: rebuild the confirmation UI.
                base[0xbdd] = 0x21;
                base[0x2dd1] = 0;
                char* a = func_80136190(&lbl_eu_805090FC[0x47], &lbl_eu_805090FC[0x52], 2);
                char* b = func_80136190(&lbl_eu_805090FC[0x47], &lbl_eu_805090FC[0x52], 4);
                char* c = func_80136190(&lbl_eu_805090FC[0x47], &lbl_eu_805090FC[0x52], 5);
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


void func_8021F5A8(CModelDispMakeCrystal* self)
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

void func_8021F764(){}

void func_8021F7E0(){}

void func_8021F828(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    base[0xbdd] = 0x26;
    base[0x2dd1] = 0x0;
    char* a = func_80136190(&lbl_eu_805090FC[0x57], &lbl_eu_805090FC[0x63], 0x3d);
    char* b = func_80136190(&lbl_eu_805090FC[0x57], &lbl_eu_805090FC[0x63], 0x3e);
    char* c = func_80136190(&lbl_eu_805090FC[0x57], &lbl_eu_805090FC[0x63], 0x3f);
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

void func_8021FB14(){}

void func_8021FB68(CModelDispMakeCrystal* self, u8* obj)
{
    CDeviceVI::waitForDrawDone();
    if (*reinterpret_cast<u32*>(obj) == 0) return;
    func_8004B6BC(obj + 0x8, *reinterpret_cast<void**>(obj + 0x4));
    reinterpret_cast<CMCVtE0*>(obj + 0x8)->m38();
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

void func_8021FC28(CModelDispMakeCrystal* self, u8 arg4)
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
        if (v8 == 0) {
            if (v4 >= 0xc8) {
                entries[(((int)i + (s8)base[0x2dc2]) & 0xffff) * 8 + 8] = 1;
            }
        } else if (v8 == 1) {
            if (v4 >= 0x12c) {
                entries[(((int)i + (s8)base[0x2dc2]) & 0xffff) * 8 + 8] = 2;
            }
        }
    }
}

void func_8021FD44(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    u8 r31 = func_801392B4(base[0x5f0]);
    u8 r30 = func_801392B4(base[0xbbc]);
    u16 val = func_8013A7D0(r31, r30);
    s16 iv = (s16)(lbl_eu_806684FC * val + lbl_eu_806684F8);
    *reinterpret_cast<f32*>(base + 0x2dcc) = lbl_eu_806684A4;
    base[0x2dc8] = 0;
    base[0x2dc4] = iv;
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

void func_8021FEDC(){}

void func_802200A8(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    u8 slot = 0;
    u8 i = 0;
    u8* entries = *reinterpret_cast<u8**>(base + 0xe1c);
    u8 entryCount = entries[0];
    for (; i < entryCount; i++) {
        u8* e = entries + (i << 3);
        if (e[9]) {
            reinterpret_cast<CMCEffUpPrm*>(base + 0xcec)
                ->setNumber(slot, (u8)*reinterpret_cast<u16*>(e + 6));
            slot++;
        }
    }
    reinterpret_cast<CMCEffUpPrm*>(base + 0xcec)->activateSlots(slot);
}

void func_80220128(){}

void func_802203D8(){}

void func_8022077C(){}

void func_80220954(){}

// Virtual dispatch at vtable offset +0xA8 (index 0x2a), returns a pointer.
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
    virtual void* m28(); virtual void* m29(); virtual void* m2A();  // +0xA8
};

void func_80220C34(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    // One-time setup: allocate the crystal display buffers and, if the file
    // reference is present, spin up the crystal support object.
    if (base[0xbe9] == 0 &&
        *reinterpret_cast<u32*>(base + 0x24) == 0 &&
        *reinterpret_cast<u32*>(base + 0x18) == 0 &&
        *reinterpret_cast<u32*>(base + 0x28) != 0 &&
        *reinterpret_cast<u32*>(base + 0x1c) != 0 &&
        *reinterpret_cast<u32*>(base + 0x20) == 0 &&
        *reinterpret_cast<u32*>(base + 0x10) == 0) {
        void* obj = func_80495E8C(*reinterpret_cast<void**>(base + 0xc), -1, 1);
        *reinterpret_cast<void**>(base + 0x20) = obj;
        func_80485684(obj, 1);
        func_80482DF4(obj, 1);
        reinterpret_cast<CMCVt48*>(obj)->m18(*reinterpret_cast<f32*>(base + 0xbe0));
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
                void* r = reinterpret_cast<CMCVtA8*>(obj)->m2A();
                *reinterpret_cast<u32*>(reinterpret_cast<u8*>(*reinterpret_cast<void**>(base + 0x10)) + 0x14) =
                    reinterpret_cast<u32>(r);
            }
        }
        base[0xbe9] = 1;
    }

    // Refresh each of the two crystal substructs.
    for (u8 i = 0; i < 2; i++) {
        u8* s = base + (u32)i * 0x5cc;
        if (base[(u32)i * 0x5cc + 0x60c] == 0) {
            s8 v = reinterpret_cast<s8*>(s + 0x44)[0x5ac];
            if (v >= 0 && v < 3) {
                func_80220E14(self, reinterpret_cast<CMCrystalDispSub*>(s + 0x44));
            } else {
                func_802211CC(self, s + 0x44);
            }
        }
    }
}

void func_80220E14(CModelDispMakeCrystal* self, CMCrystalDispSub* sub)
{
    CMCCrySelfFields* objs = reinterpret_cast<CMCCrySelfFields*>(self);
    // The 3 state-specific enum filter types (keyed by sub->field_5ac state).
    u32 src[3];
    src[0] = reinterpret_cast<u32*>(lbl_eu_805090D8)[0];
    src[1] = reinterpret_cast<u32*>(lbl_eu_805090D8)[1];
    src[2] = reinterpret_cast<u32*>(lbl_eu_805090D8)[2];

    CMCryListHolder holder;
    func_80043D90(&holder);

    // Select the crystal list for the current char state and check it's loaded.
    func_800F4A98(func_80043F18(&holder), src[(s8)sub->field_5ac], 0);
    if (*reinterpret_cast<u32*>(reinterpret_cast<u8*>(func_80043F18(&holder)) + 0x620) == 0) {
        func_8021FB68(self, reinterpret_cast<u8*>(sub));
        __dt__80043E88(&holder, -1);
        return;
    }

    // Grab the first crystal object from the enum list.
    void* slot = func_800F6EC0(func_80043F18(&holder), 0);
    void* cfMove = *reinterpret_cast<void**>(reinterpret_cast<u8*>(slot) + 4);
    if (cfMove == nullptr) {
        func_8021FB68(self, reinterpret_cast<u8*>(sub));
        __dt__80043E88(&holder, -1);
        return;
    }

    CMCCryChgActor* actor =
        reinterpret_cast<CMCCryChgActor*>(func_800BFC68__FPQ22cf12CfObjectMove(cfMove));
    int ready = actor != nullptr ? 1 : 0;
    int m = 0;
    if (actor != nullptr) {
        m = actor->field_3f2c;
        if (m == 0) ready = 0;
        if (reinterpret_cast<CMCryMoveVt*>(&actor->move)->m74() == 0) ready = 0;
        if (sub->field_5a4 == 0) {
            if (sub->field_5a0 == 0) {
                // Load the crystal model file for this actor.
                char buf[0x20];
                sprintf(buf, &lbl_eu_805090FC[0xd1],
                        actor->field_3f28 == 3 ? 8 : (int)actor->field_3f28);
                u32 h = getHandleMEM2__Q23mtl10MemManagerFv();
                sub->field_5a0 =
                    reinterpret_cast<u32>(readFile__11CDeviceFileFUlPCcP10IWorkEventii(
                        h, buf, self, 0, 0));
            }
            ready = 0;
        }
    }

    if (sub->field_00 == nullptr && ready != 0) {
        CMCryMoveVt* mv = reinterpret_cast<CMCryMoveVt*>(&actor->move);
        // Build the crystal display model for this slot.
        sub->field_00 = func_80495E8C(objs->field_0c, m, -1, 1);
        sub->mCrystalVals[1] = (static_cast<u32>(mv->m82(1)) >> 12) & 0x3ff;
        s16 be = func_800BE954(&actor->move);
        CMCCryParamSlot* param =
            reinterpret_cast<CMCCryParamSlot*>(func_80062C28(be, 0));
        for (int idx = 2; idx <= 5; idx++) {
            if (mv->m82(idx) != 0) {
                void* obj = param[idx].field_2c;
                func_804831C4(sub->field_00, reinterpret_cast<CMCCryParamObjVt*>(obj)->m02());
                sub->mCrystalVals[idx] = (static_cast<u32>(mv->m82(idx)) >> 12) & 0x3ff;
            }
        }
        m = sub->field_5a4;
        sub->field_04 = func_800584B8(objs->field_0c, m, &lbl_eu_805090FC[0xef]);
        sub->mAnim.m38();
        func_8004B624(&sub->mAnim, sub->field_00, sub->field_04, m);
        sub->field_14 |= 0x160;
        func_80200388(&sub->mAnim,
                      self ? reinterpret_cast<void*>(reinterpret_cast<u8*>(self) + 4) : self);
        u32* group0 = reinterpret_cast<u32*>(func_8048315C(sub->field_00));
        group0[0] = sub->field_5b0;
        group0[1] = sub->field_5b4;
        group0[2] = sub->field_5b8;
        u32* group1 = reinterpret_cast<u32*>(func_8048315C(sub->field_00));
        group1[3] = sub->field_5bc;
        group1[4] = sub->field_5c0;
        group1[5] = sub->field_5c4;
        reinterpret_cast<CMCModelVt*>(sub->field_00)->m12(objs->field_be0);
        reinterpret_cast<CMCModelVt*>(sub->field_00)->m27(3, 0);
        if (sub->field_00 != nullptr) {
            func_8004B9D4(&sub->mAnim, 0x21, 0, -1, 0);
        }
        sub->field_5c8 = 1;
    } else {
        if (sub->field_00 != nullptr && ready == 0) {
            func_8021FB68(self, reinterpret_cast<u8*>(sub));
        }
    }

    __dt__80043E88(&holder, -1);
}

void func_802211CC(CModelDispMakeCrystal* self, u8* sub){}

void CModelDispMakeCrystal::OnFileEvent() {}

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

void func_80221D58(void* obj, u32 id)
{
    u32 e1 = *reinterpret_cast<u32*>((u8*)obj + 0x10);
    if (id == e1) {
        func_804E3D48(reinterpret_cast<void*>(e1),
                      obj ? reinterpret_cast<void*>((u8*)obj + 8) : obj);
        *reinterpret_cast<u32*>((u8*)obj + 0x10) = 0;
    } else {
        u32 e2 = *reinterpret_cast<u32*>((u8*)obj + 0x14);
        if (id == e2) {
            func_804E3D48(reinterpret_cast<void*>(e2),
                          obj ? reinterpret_cast<void*>((u8*)obj + 8) : obj);
            *reinterpret_cast<u32*>((u8*)obj + 0x14) = 0;
        }
    }
}

// (initCrystalData is a retail hard-symbol; not defined here so it stays a call)

void CModelDispMakeCrystal::destroyCrystalDispThunk4() { ((void(*)(void*))__dt__21CModelDispMakeCrystalFv)((char*)this - 0x4); }

void CModelDispMakeCrystal::releaseCrystalDispThunk() { ((void(*)(void*))func_80221D58)((char*)this - 0x8); }

void CModelDispMakeCrystal::destroyCrystalDispThunk8() { ((void(*)(void*))__dt__21CModelDispMakeCrystalFv)((char*)this - 0x8); }

void sinit_80221DDC() {
    initCrystalData(&lbl_eu_80664718);
    func_8004B60C(lbl_eu_80576658, lbl_eu_80668508, lbl_eu_806684AC, lbl_eu_8066850C);
    func_8004B60C(lbl_eu_80576664, lbl_eu_806684A4, scaleByGlobal(lbl_eu_80668510), lbl_eu_806684A4);
}
