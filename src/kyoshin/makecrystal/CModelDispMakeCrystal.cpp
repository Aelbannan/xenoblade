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

// forward declarations for scaffold thunk references
void __dt__21CModelDispMakeCrystalFv(void*);

// --- external C-linkage helpers and global data for this TU ---
extern "C" {
void func_8004B60C(void* self, f32 x, f32 y, f32 z);
float scaleByGlobal(float val);
void initCrystalData(unsigned char* p);
int CSysWin_isActive(void* self);
void func_801D216C(void*, u8);
void func_8022C1B4(void* out, void* csyswin, u8);
void func_80297B68(void*);
void func_80297E18(void*);
void func_8022B7F4(void*);
int func_80222A58(void*);
// Retail member-return types are int (callers `cmpwi r3,0` the raw reg).
// Mangled-name refs let us call them as int-returning without re-mangling.
int isFinished__11CMCEffStartFv(void* self);
int isFinished__13CMCEffCrystalFv(void* self);
void func_802203D8(void*);
void func_8022077C(void*);
void func_802A1500(void);
void func_80189C88(void);
void func_80043C88(void);
void func_80133E58(u32, u32, u32);
void func_80138078(u32);
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
void func_801F34F4(void*);
void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
u32 getHandleMEM2__Q23mtl10MemManagerFv();
int isFinished__FPv(void*);
char lbl_eu_805090FC[];
}
extern const f32 lbl_eu_806684D4;
extern const f32 lbl_eu_806684A0;
extern const f32 lbl_eu_806684A4;
extern const f32 lbl_eu_806684DC;
extern const f32 lbl_eu_80668508;
extern const f32 lbl_eu_806684AC;
extern const f32 lbl_eu_8066850C;
extern const f32 lbl_eu_80668510;
extern "C" u8 lbl_eu_80664718;
extern u8 lbl_eu_80576658[0x20];
extern u8 lbl_eu_80576664[0x20];

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

void __ct__CModelDispMakeCrystal(){}

void CModelDispMakeCrystal::initCrystalSubStruct() {
    *(unsigned long*)this = 0;
    *(unsigned long*)((char*)this + 4) = 0;
    ((unsigned char*)this)[8] = 0;
}

void func_8021C4F0(){}

void __dt__8021C540(){}

CModelDispMakeCrystal::~CModelDispMakeCrystal() {}

void func_8021C6E4(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    u32 h1 = getHandleMEM2__Q23mtl10MemManagerFv();
    *reinterpret_cast<void**>(base + 0x24) =
        readFile__11CDeviceFileFUlPCcP10IWorkEventii(h1, &lbl_eu_805090FC[0], self, 0, 0);
    u32 h2 = getHandleMEM2__Q23mtl10MemManagerFv();
    *reinterpret_cast<void**>(base + 0x18) =
        readFile__11CDeviceFileFUlPCcP10IWorkEventii(h2, &lbl_eu_805090FC[0x11], self, 0, 0);
    u32 h3 = getHandleMEM2__Q23mtl10MemManagerFv();
    *reinterpret_cast<void**>(base + 0x3c) =
        readFile__11CDeviceFileFUlPCcP10IWorkEventii(h3, &lbl_eu_805090FC[0x25], self, 0, 0);
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

void func_8021CB20(){}

void func_8021CC60(){}

void func_8021CD8C(){}

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

void func_8021CEF0(){}

void func_8021CFC0(){}

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

void func_8021D3E4(){}

void func_8021D564(){}

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

void func_8021D9B8(){}

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
    u8* base = reinterpret_cast<u8*>(self);
    if (isFinished__FPv(base + 0xd5c)) {
        u8* entries = *reinterpret_cast<u8**>(base + 0xe1c);
        CMCCylinderGauge* gauge = reinterpret_cast<CMCCylinderGauge*>(base + 0xbec);
        CMCEffCylinder* cyl = reinterpret_cast<CMCEffCylinder*>(base + 0xdfc);
        if (entries[0]) {
            base[0xbdd] = 0x11;
            cyl->setNumber(gauge->func_80222258());
            gauge->func_80222118();
            cyl->startIn();
        } else {
            base[0xbdd] = 0x14;
            gauge->func_80222118();
        }
    }
}

void func_8021DF84(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    CMCEffCylinder* cyl = reinterpret_cast<CMCEffCylinder*>(base + 0xdfc);
    CMCCylinderGauge* gauge = reinterpret_cast<CMCCylinderGauge*>(base + 0xbec);
    if (!cyl->isFinished()) return;
    if (!gauge->func_802220F0()) return;
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

void func_8021ECD4(){}

void func_8021EF30(CModelDispMakeCrystal* self)
{
    u8* base = reinterpret_cast<u8*>(self);
    CMCGetItemBox* box = reinterpret_cast<CMCGetItemBox*>(base + 0xecc);
    if (!box->func_80297D1C()) return;
    u8 count = base[0x2dc0];
    int found = 0;
    for (u8 i = 0; i < count; i++) {
        u8* e = base + (u32)i * 0x34 + 0x13c0;
        if (e && (((*reinterpret_cast<u32*>(e) >> 12) & 0xf) == 3)) {
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


void func_8021F058(){}

void func_8021F214(){}

void func_8021F290(){}

void func_8021F2D8(){}

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


void func_8021F5A8(){}

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


void func_8021F958(){}

void func_8021FB14(){}

void func_8021FB68(){}

void func_8021FC28(){}

void func_8021FD44(){}

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
                ->setNumber(slot, *reinterpret_cast<u16*>(e + 6));
            slot++;
        }
    }
    reinterpret_cast<CMCEffUpPrm*>(base + 0xcec)->activateSlots(slot);
}

void func_80220128(){}

void func_802203D8(){}

void func_8022077C(){}

void func_80220954(){}

void func_80220C34(){}

void func_80220E14(){}

void func_802211CC(){}

void CModelDispMakeCrystal::OnFileEvent() {}

void func_80221B90(){}

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
    u8* base = reinterpret_cast<u8*>(obj);
    u32 e1 = *reinterpret_cast<u32*>(base + 0x10);
    if (id == e1) {
        func_804E3D48(reinterpret_cast<void*>(e1), obj ? base + 0x8 : base);
        *reinterpret_cast<u32*>(base + 0x10) = 0;
    } else {
        u32 e2 = *reinterpret_cast<u32*>(base + 0x14);
        if (id == e2) {
            func_804E3D48(reinterpret_cast<void*>(e2), obj ? base + 0x8 : base);
            *reinterpret_cast<u32*>(base + 0x14) = 0;
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
