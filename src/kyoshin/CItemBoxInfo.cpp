// Auto-scaffolded catalog TU for kyoshin/CItemBoxInfo
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void func_801D3FF0() {}








extern "C" u8 func_801D4214(void* self) {
    return *(u8*)((char*)self + 0x98);
}


extern "C" void func_801D4240(void* self) {
    struct Layout {
        unsigned char _pad[0x94];
        int field_94;
        unsigned char _pad98[1];
        unsigned char field_99;
    };
    Layout* p = (Layout*)self;
    if (p->field_94 == 3) {
        p->field_94 = 4;
        p->field_99 = 0;
    }
}



extern "C" void func_801D4A2C() {}

extern "C" void func_801D4AB0() {}

extern "C" void func_801D4AE0() {}

extern "C" void func_801D4B3C() {}

extern "C" void func_801D4BDC() {}

extern "C" void func_801D4C3C() {}

extern "C" void func_801D4C9C() {}

extern "C" void func_801D4D18() {}

extern "C" void func_801D4D64() {}

extern "C" void func_801D4DE0() {}

extern "C" void func_801D4E2C() {}

extern "C" void func_801D5274() {}

extern "C" void func_801D5564() {}

extern "C" void func_801D59C0() {}

extern "C" void func_801D5AA0() {}

extern "C" void func_801D5C38() {}

extern "C" void func_801D5DA4() {}

extern "C" void func_801D62F8() {}

extern "C" void __as__11_GXColorS10FRC11_GXColorS10() {}

extern "C" void func_801D6394() {}

extern "C" void func_801D69FC() {}

extern "C" void func_801D77A4(void* self, unsigned char index, short value) {
    if (index >= 12) return;
    *(short*)((char*)self + index * 2 + 0xC0) = value;
}

extern "C" void func_801D77BC() {}

extern "C" void func_801D79F8() {}

extern "C" void func_801D8058() {}

extern "C" void func_801D80EC() {}

extern "C" void func_801D8318() {}

extern "C" void func_801D85D8() {}

extern "C" void func_801D885C() {}

extern "C" void func_801D8930() {}

extern "C" void func_801D8A88() {}

extern "C" void func_801D8B08() {}

extern "C" void func_801D8B60() {}

extern "C" void func_801D8C0C() {}

extern "C" void func_801D8E08(void* arg) {
    if (*(unsigned int*)((char*)arg + 0x34) == 0) return;
    if (*(unsigned int*)((char*)arg + 0x30) == 0) return;
    *((char*)arg + 0x98) = 1;
    *((char*)arg + 0x90) = 1;
}

extern "C" void func_801D8E34() {}

struct CItemBoxInfoEntry {
    unsigned short unk0;
    unsigned int unk4;
    unsigned char unk8;
};

extern "C" void func_801DF4B4(CItemBoxInfoEntry* dst, const CItemBoxInfoEntry* src) {
    dst->unk0 = src->unk0;
    dst->unk4 = src->unk4;
    dst->unk8 = src->unk8;
}

extern "C" void func_801DF4D0(void* self, u16 r4, u32 r5, u8 r6) {
    *(u16*)self = r4;
    *((u32*)self + 1) = r5;
    *((u8*)self + 8) = r6;
}

extern "C" void func_801DF4E0() {}

extern "C" void func_801DF578() {}

extern "C" void func_801DF610() {}

extern "C" void func_801DF988() {}

extern "C" void func_801DFD60() {}

extern "C" void func_801DFDC0() {}

extern "C" void func_801DFE48() {}

extern "C" void func_801DFFB8() {}

extern "C" void OnFileEvent__12CItemBoxInfoFP10CEventFile() {}

extern "C" void __ct__CItemBoxInfo2() {}

extern "C" void __dt__13CItemBoxInfo2Fv() {}

extern "C" void func_801E12E0() {}

extern "C" void func_801E1348() {}

namespace nw4r { namespace lyt { class Layout; class DrawInfo; } }
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

extern "C" void func_801E13D8(void* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (*(unsigned char*)((char*)self + 0x90) != 0) {
        func_80137038(*(nw4r::lyt::Layout**)((char*)self + 0x34), drawInfo, 0, 1);
    }
}

extern "C" void func_801E13F8() {}

extern "C" u8 func_801E1490(void* self) {
    return *(u8*)((char*)self + 0x98);
}

extern "C" void func_801E1498() {}

extern "C" void func_801E14BC(void* param_1) {
    struct Fields {
        char unk_0x00[0x94];
        int field_0x94;
        char unk_0x98;
        unsigned char field_0x99;
    };
    Fields* p = reinterpret_cast<Fields*>(param_1);
    if (p->field_0x94 == 3) {
        p->field_0x94 = 4;
        p->field_0x99 = 0;
    }
}

extern "C" void func_801E14DC() {}

extern "C" void func_801E16F0() {}

extern "C" void func_801E174C() {}

extern "C" void func_801E17EC() {}

extern "C" void func_801E1868() {}

extern "C" void func_801E18B4() {}

extern "C" void func_801E1930() {}

extern "C" void func_801E197C() {}

extern "C" void func_801E1E0C() {}

extern "C" void func_801E20FC() {}

extern "C" void func_801E2558() {}

extern "C" void func_801E2638() {}

extern "C" void func_801E27D0() {}

extern "C" void func_801E2928() {}

extern "C" void func_801E2C5C() {}

extern "C" void func_801E2FEC() {}

extern "C" void func_801E3228() {}

extern "C" void func_801E3730() {}

extern "C" void func_801E37C4() {}

extern "C" void func_801E3918() {}

extern "C" void func_801E3B9C() {}

extern "C" void func_801E3DE4() {}

extern "C" void func_801E3EB8() {}

extern "C" void func_801E4010() {}

extern "C" void func_801E4090() {}

extern "C" void func_801E40E8() {}

extern "C" void func_801E4194() {}

extern "C" void func_801E4390() {}

extern "C" void func_801E43BC() {}

struct ItemBoxInfoCopy {
    unsigned short unk0;
    unsigned int unk4;
    unsigned char unk8;
};

extern "C" void func_801E9164(ItemBoxInfoCopy* dst, const ItemBoxInfoCopy* src) {
    *dst = *src;
}

extern "C" void func_801E9180(void* self, unsigned short a, unsigned long b, unsigned char c) {
    *(unsigned short*)self = a;
    *(unsigned long*)((char*)self + 4) = b;
    *(unsigned char*)((char*)self + 8) = c;
}

extern "C" void func_801E9190() {}

extern "C" void func_801E9224() {}

extern "C" void func_801E92B8() {}

extern "C" void func_801E9310() {}

extern "C" void func_801E9690() {}

extern "C" void func_801E96F0() {}

extern "C" void func_801E9774() {}

extern "C" void func_801E98E4() {}

extern "C" void OnFileEvent__13CItemBoxInfo2FP10CEventFile() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_801EABC4() {}
