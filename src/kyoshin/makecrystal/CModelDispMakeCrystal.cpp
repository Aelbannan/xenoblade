// Auto-scaffolded catalog TU for kyoshin/makecrystal/CModelDispMakeCrystal
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/makecrystal/CModelDispMakeCrystal.hpp"

#include "kyoshin/makecrystal/CMCEffStart.hpp"
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
}
extern const f32 lbl_eu_806684D4;
extern const f32 lbl_eu_806684A0;
extern const f32 lbl_eu_806684A4;
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

void __ct__CModelDispMakeCrystal(){}

void CModelDispMakeCrystal::initCrystalSubStruct() {
    *(unsigned long*)this = 0;
    *(unsigned long*)((char*)this + 4) = 0;
    ((unsigned char*)this)[8] = 0;
}

void func_8021C4F0(){}

void __dt__8021C540(){}

CModelDispMakeCrystal::~CModelDispMakeCrystal() {}

void func_8021C6E4(){}

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

void func_8021CE4C(){}

void func_8021CEF0(){}

void func_8021CFC0(){}

void func_8021D168(){}

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

void func_8021DEF8(){}

void func_8021DF84(){}

void func_8021E014(){}

void func_8021E5C0(){}

void func_8021E840(){}

void func_8021E888(){}

void func_8021E8E4(){}

void func_8021EAB8(){}

void func_8021EB00(){}

void func_8021EC04(){}

void func_8021ECD4(){}

void func_8021EF30(){}

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

void func_8021F828(){}

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

void func_802200A8(){}

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

void func_80221CD0(){}

void func_80221D58(void* self){}

// (initCrystalData is a retail hard-symbol; not defined here so it stays a call)

void CModelDispMakeCrystal::destroyCrystalDispThunk4() { ((void(*)(void*))__dt__21CModelDispMakeCrystalFv)((char*)this - 0x4); }

void CModelDispMakeCrystal::releaseCrystalDispThunk() { ((void(*)(void*))func_80221D58)((char*)this - 0x8); }

void CModelDispMakeCrystal::destroyCrystalDispThunk8() { ((void(*)(void*))__dt__21CModelDispMakeCrystalFv)((char*)this - 0x8); }

void sinit_80221DDC() {
    initCrystalData(&lbl_eu_80664718);
    func_8004B60C(lbl_eu_80576658, lbl_eu_80668508, lbl_eu_806684AC, lbl_eu_8066850C);
    func_8004B60C(lbl_eu_80576664, lbl_eu_806684A4, scaleByGlobal(lbl_eu_80668510), lbl_eu_806684A4);
}
