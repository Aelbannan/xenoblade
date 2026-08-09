// Auto-scaffolded catalog TU for kyoshin/cf/CfResReloadImpl
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

namespace cf { struct CfResReloadImpl; }

void __ct__cf_CfResReloadImpl(){}

// EQUIVALENT_MATCH: checks if type field at offset +8 is >= 3
int checkTypeIsValid(cf::CfResReloadImpl* arg) {
    unsigned int v = *(unsigned short*)((char*)arg + 8);
    return (int)(((v | 0xFFFFFFFCu) - ((v - 3u) >> 1)) >> 31);
}

void func_8016CE5C(){}

// FULL_MATCH: vtable slot 6 - returns constant resource type identifier
int getTypeId() { return 12; }

void func_8016CF24(){}

void func_8016CFBC(){}

void func_8016CFDC(){}

void func_8016D0C0(){}

void func_8016D144(){}

void func_8016D1D8(){}

void func_8016D240(){}

void func_8016D2FC(){}

void func_8016D390(){}

void func_8016D3F8(){}

void func_8016D688(){}

void func_8016DAF8(){}

// Cast-only SI for CfResReloadImpl sub-object virtual calls
struct ResReloadIf {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void* vf3C(void* a);
    virtual void _v040(); virtual int vf44();
    virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
    virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
    virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
    virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
    virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
    virtual void _v098(); virtual void _v09C(); virtual void _v0A0(); virtual void _v0A4();
    virtual void _v0A8(); virtual void _v0AC(); virtual void vfB0();
    virtual void _v0B4(); virtual void _v0B8(); virtual void _v0BC(); virtual void _v0C0();
    virtual void _v0C4(); virtual void _v0C8(); virtual void _v0CC(); virtual void _v0D0();
    virtual void _v0D4(); virtual void _v0D8(); virtual void _v0DC(); virtual float vfE0();
    virtual void _v0E4(); virtual void _v0E8(); virtual void _v0EC(); virtual float vfF0();
    virtual void _v0F4(); virtual void _v0F8(); virtual void _v0FC(); virtual void _v100();
    virtual void _v104(); virtual void _v108(); virtual void _v10C(); virtual void _v110();
    virtual void _v114(); virtual void _v118(); virtual void _v11C(); virtual void _v120();
    virtual void _v124(); virtual void _v128(); virtual void _v12C(); virtual void _v130();
    virtual void _v134(); virtual void _v138(); virtual void _v13C(); virtual void _v140();
    virtual void _v144(); virtual void _v148(); virtual void _v14C(); virtual void _v150();
    virtual void _v154(); virtual void _v158(); virtual void _v15C(); virtual void _v160();
    virtual void _v164(); virtual void _v168(); virtual void _v16C(); virtual void _v170();
    virtual void _v174(); virtual void _v178(); virtual void _v17C(); virtual void* vf180();
    virtual void _v184(); virtual void _v188(); virtual void _v18C(); virtual void _v190();
    virtual void _v194(); virtual void _v198(); virtual void _v19C(); virtual void _v1A0();
    virtual void _v1A4(); virtual void _v1A8(); virtual void _v1AC(); virtual void _v1B0();
    virtual void _v1B4(); virtual void _v1B8(); virtual void _v1BC(); virtual void _v1C0();
    virtual void _v1C4(); virtual void _v1C8(); virtual void _v1CC(); virtual void _v1D0();
    virtual void _v1D4(); virtual void _v1D8(); virtual void _v1DC(); virtual void _v1E0();
    virtual void _v1E4(); virtual void _v1E8(); virtual void _v1EC(); virtual void _v1F0();
    virtual void _v1F4(); virtual void _v1F8(); virtual void _v1FC(); virtual int vf200();
};

extern "C" void func_8016DCE4(u8* self) {
    if (!((ResReloadIf*)*(void**)self)->vf44()) {
        void* r = ((ResReloadIf*)*(void**)self)->vf180();
        ((ResReloadIf*)*(void**)self)->vf3C(r);
    }
    if (*(void**)((u8*)*(void**)self + 0x38))
        ((ResReloadIf*)*(void**)((u8*)*(void**)self + 0x38))->vfB0();
    if (*(void**)((u8*)*(void**)self + 0x98)) {
        float v;
        if (((ResReloadIf*)*(void**)self)->vf200()) {
            void* b = *(void**)self;
            if (b) b = (u8*)b - 0x3e9c;
            v = ((ResReloadIf*)b)->vfF0();
        } else {
            v = ((ResReloadIf*)*(void**)self)->vfE0();
        }
        void* obj = *(void**)((u8*)*(void**)self + 0x98);
        *(float*)((u8*)obj + 0x304) = v;
        *(float*)((u8*)obj + 0x308) = v;
        *(float*)((u8*)obj + 0x30c) = v;
    }
}

void func_8016DDE8(){}

void func_eu_8016F1C4(){}

void func_8016DE68(){}

void func_8016DE8C(){}

// FULL_MATCH: vtable slot 23 - returns constant resource sub-type
int getSubTypeId() { return 1; }

void setWorkValue(cf::CfResReloadImpl* thisPtr, int index, unsigned int value) {
    if (index < 2) {
        ((unsigned int*)((char*)thisPtr + 0x14))[index] = value;
    }
}

void* __dt__Q22cf15CfResReloadImplFv(void* this_, int deletingFlag) { if (this_ && deletingFlag > 0) ::operator delete(this_); return this_; }

extern u16 lbl_eu_80664278;
u16 getReloadParam0() { return lbl_eu_80664278; }

float lbl_eu_806676B0;
float lbl_eu_80666210;
float lbl_eu_80664284;

void updateReloadFloat(short param_1)
{
    lbl_eu_80664278 = param_1;
    lbl_eu_80664284 = lbl_eu_806676B0 * lbl_eu_80666210;
}

void func_8016DF4C(){}

extern u16 lbl_eu_80664280;
u16 getReloadParam2() { return lbl_eu_80664280; }

extern u16 lbl_eu_8066427E;
u16 getReloadParam1() { return lbl_eu_8066427E; }

unsigned short lbl_eu_8066427A;
unsigned short lbl_eu_8066427C;
void* memset(void*, int, unsigned long);

// Initializes a 0x10-byte struct from global reload params
void initReloadInfoStruct(void* self) {
    unsigned char* p = static_cast<unsigned char*>(self);
    memset(self, 0, 0x10);
    *reinterpret_cast<float*>(p + 0x0) = lbl_eu_80664284;
    *reinterpret_cast<unsigned short*>(p + 0x4) = lbl_eu_80664278;
    *reinterpret_cast<unsigned short*>(p + 0x6) = lbl_eu_8066427E;
    *reinterpret_cast<unsigned short*>(p + 0xC) = lbl_eu_80664280;
    *reinterpret_cast<unsigned short*>(p + 0x8) = lbl_eu_8066427A;
    *reinterpret_cast<unsigned short*>(p + 0xA) = lbl_eu_8066427C;
}

void func_8016E100(){}

void func_8016E13C(){}

void updateReloadTypeState(u16 r3, u16 r4) {
    extern u16 lbl_eu_8066427A;
    extern u16 lbl_eu_8066427C;
    extern u16 lbl_eu_80664288;
    extern float lbl_eu_806676C8;
    if (lbl_eu_8066427A == r3 && lbl_eu_8066427C == r4) {
        lbl_eu_80664288 &= 0xFFFE;
    } else {
        lbl_eu_80664288 |= 0x1;
    }
    lbl_eu_80664284 = lbl_eu_806676C8;
    lbl_eu_8066427A = r3;
    lbl_eu_8066427C = r4;
}

void func_8016E1AC(){}

void func_8016E430(){}

void func_8016E578(){}

void func_8016E654(){}

void func_8016E854(){}

void func_8016E9CC(){}

void func_8016EA68(){}

void func_8016EC58(){}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_8016ED44() {
    extern unsigned short lbl_eu_80664288;
    lbl_eu_80664288 = 0;
}
