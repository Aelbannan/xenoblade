// Auto-scaffolded catalog TU for kyoshin/makecrystal/CModelDispMakeCrystal
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__CModelDispMakeCrystal() {}

extern "C" void func_8021C4DC(void* self) {
    *(unsigned long*)self = 0;
    *(unsigned long*)((char*)self + 4) = 0;
    ((unsigned char*)self)[8] = 0;
}

extern "C" void func_8021C4F0() {}

extern "C" void __dt__8021C540() {}

extern "C" void __dt__21CModelDispMakeCrystalFv(void* self) {}

extern "C" void func_8021C6E4() {}

extern "C" void func_8021C7A0() {}

extern "C" void func_8021C8B0() {}

extern "C" void func_8021C928() {}

extern "C" u8 func_8021CA34(void* self) { return ((u8*)self)[0xBE8]; }

extern "C" void func_8021CA3C() {}

extern "C" u8 func_8021CB10(void* self) { return ((u8*)self)[0x2DC3]; }

extern "C" u8 func_8021CB18(void* self) { return ((u8*)self)[0x2DD2]; }

extern "C" void func_8021CB20() {}

extern "C" void func_8021CC60() {}

extern "C" void func_8021CD8C() {}

extern "C" void func_8021CE4C() {}

extern "C" void func_8021CEF0() {}

extern "C" void func_8021CFC0() {}

extern "C" void func_8021D168() {}

extern "C" void func_8021D200() {}

extern "C" void func_8021D3E4() {}

extern "C" void func_8021D564() {}

extern "C" void func_8021D6B4() {}

extern "C" void func_8021D99C(void* self, unsigned short index, short a, short b)
{
    struct Entry {
        char _pad0[4];
        short x;
        short y;
        char _pad8;
        unsigned char flag;
    };
    Entry* entry = (Entry*)((char*)self + ((unsigned int)index << 3));
    entry->x = a;
    entry->y = b;
    entry->flag = 1;
}

extern "C" void func_8021D9B8() {}

extern "C" void func_8021DAF4() {}

extern "C" void func_8021DB3C() {}

extern "C" void func_8021DB9C() {}

extern "C" void func_8021DC1C() {}

extern "C" void func_8021DD0C() {}

extern "C" void func_8021DE20() {}

extern "C" void func_8021DE7C() {}

extern "C" void func_8021DEF8() {}

extern "C" void func_8021DF84() {}

extern "C" void func_8021E014() {}

extern "C" void func_8021E5C0() {}

extern "C" void func_8021E840() {}

extern "C" void func_8021E888() {}

extern "C" void func_8021E8E4() {}

extern "C" void func_8021EAB8() {}

extern "C" void func_8021EB00() {}

extern "C" void func_8021EC04() {}

extern "C" void func_8021ECD4() {}

extern "C" void func_8021EF30() {}

extern "C" void func_8021EFE4() {}

extern "C" void func_8021F058() {}

extern "C" void func_8021F214() {}

extern "C" void func_8021F290() {}

extern "C" void func_8021F2D8() {}

extern "C" void func_8021F534() {}

extern "C" void func_8021F5A8() {}

extern "C" void func_8021F764() {}

extern "C" void func_8021F7E0() {}

extern "C" void func_8021F828() {}

extern "C" void func_8021F8E4() {}

extern "C" void func_8021F958() {}

extern "C" void func_8021FB14() {}

extern "C" void func_8021FB68() {}

extern "C" void func_8021FC28() {}

extern "C" void func_8021FD44() {}

extern "C" void func_8021FEDC() {}

extern "C" void func_802200A8() {}

extern "C" void func_80220128() {}

extern "C" void func_802203D8() {}

extern "C" void func_8022077C() {}

extern "C" void func_80220954() {}

extern "C" void func_80220C34() {}

extern "C" void func_80220E14() {}

extern "C" void func_802211CC() {}

extern "C" void OnFileEvent__21CModelDispMakeCrystalFP10CEventFile() {}

extern "C" void func_80221B90() {}

extern "C" void func_80221CC8(void* self, u32 val) { *(u32*)((u8*)self + 0xE1C) = val; }

extern "C" void func_80221CD0() {}

extern "C" void func_80221D58(void* self) {}

extern "C" void func_80221E38(unsigned char* p) {
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[5] = 0;
}

extern "C" void func_80221E54(void* self) { ((void(*)(void*))__dt__21CModelDispMakeCrystalFv)((char*)self - 0x4); }

extern "C" void func_80221E5C(void* self) { ((void(*)(void*))func_80221D58)((char*)self - 0x8); }

extern "C" void func_80221E64(void* self) { ((void(*)(void*))__dt__21CModelDispMakeCrystalFv)((char*)self - 0x8); }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_80221DDC() {}
