// Auto-scaffolded catalog TU for kyoshin/makecrystal/CModelDispMakeCrystal
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/makecrystal/CModelDispMakeCrystal.hpp"

// forward declarations for scaffold thunk references
void __dt__21CModelDispMakeCrystalFv(void*);

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

void func_8021C8B0(){}

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

void func_8021DB3C(){}

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

void func_8021EFE4(){}

void func_8021F058(){}

void func_8021F214(){}

void func_8021F290(){}

void func_8021F2D8(){}

void func_8021F534(){}

void func_8021F5A8(){}

void func_8021F764(){}

void func_8021F7E0(){}

void func_8021F828(){}

void func_8021F8E4(){}

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

void initCrystalData(unsigned char* p) {
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[5] = 0;
}

void CModelDispMakeCrystal::destroyCrystalDispThunk4() { ((void(*)(void*))__dt__21CModelDispMakeCrystalFv)((char*)this - 0x4); }

void CModelDispMakeCrystal::releaseCrystalDispThunk() { ((void(*)(void*))func_80221D58)((char*)this - 0x8); }

void CModelDispMakeCrystal::destroyCrystalDispThunk8() { ((void(*)(void*))__dt__21CModelDispMakeCrystalFv)((char*)this - 0x8); }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_80221DDC(){}
