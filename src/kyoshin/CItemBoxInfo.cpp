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



void func_801D4A2C(){}

void func_801D4AB0(){}

void func_801D4AE0(){}

void func_801D4B3C(){}

void func_801D4BDC(){}

void func_801D4C3C(){}

void func_801D4C9C(){}

void func_801D4D18(){}

void func_801D4D64(){}

void func_801D4DE0(){}

void func_801D4E2C(){}

void func_801D5274(){}

void func_801D5564(){}

void func_801D59C0(){}

void func_801D5AA0(){}

void func_801D5C38(){}

void func_801D5DA4(){}

void func_801D62F8(){}

void __as__11_GXColorS10FRC11_GXColorS10(){}

void func_801D6394(){}

void func_801D69FC(){}

extern "C" void func_801D77A4(void* self, unsigned char index, short value) {
    if (index >= 12) return;
    *(short*)((char*)self + index * 2 + 0xC0) = value;
}

void func_801D77BC(){}

void func_801D79F8(){}

void func_801D8058(){}

void func_801D80EC(){}

void func_801D8318(){}

void func_801D85D8(){}

void func_801D885C(){}

void func_801D8930(){}

void func_801D8A88(){}

void func_801D8B08(){}

void func_801D8B60(){}

void func_801D8C0C(){}

extern "C" void func_801D8E08(void* arg) {
    if (*(unsigned int*)((char*)arg + 0x34) == 0) return;
    if (*(unsigned int*)((char*)arg + 0x30) == 0) return;
    *((char*)arg + 0x98) = 1;
    *((char*)arg + 0x90) = 1;
}

void func_801D8E34(){}

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

void func_801DF4E0(){}

void func_801DF578(){}

void func_801DF610(){}

void func_801DF988(){}

void func_801DFD60(){}

void func_801DFDC0(){}

void func_801DFE48(){}

void func_801DFFB8(){}

void CItemBoxInfo::OnFileEvent() {}

void __ct__CItemBoxInfo2(){}

void CItemBoxInfo2::~CItemBoxInfo2() {}

void func_801E12E0(){}

void func_801E1348(){}

namespace nw4r { namespace lyt { class Layout; class DrawInfo; } }
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

extern "C" void func_801E13D8(void* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (*(unsigned char*)((char*)self + 0x90) != 0) {
        func_80137038(*(nw4r::lyt::Layout**)((char*)self + 0x34), drawInfo, 0, 1);
    }
}

void func_801E13F8(){}

extern "C" u8 func_801E1490(void* self) {
    return *(u8*)((char*)self + 0x98);
}

void func_801E1498(){}

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

void func_801E14DC(){}

void func_801E16F0(){}

void func_801E174C(){}

void func_801E17EC(){}

void func_801E1868(){}

void func_801E18B4(){}

void func_801E1930(){}

void func_801E197C(){}

void func_801E1E0C(){}

void func_801E20FC(){}

void func_801E2558(){}

void func_801E2638(){}

void func_801E27D0(){}

void func_801E2928(){}

void func_801E2C5C(){}

void func_801E2FEC(){}

void func_801E3228(){}

void func_801E3730(){}

void func_801E37C4(){}

void func_801E3918(){}

void func_801E3B9C(){}

void func_801E3DE4(){}

void func_801E3EB8(){}

void func_801E4010(){}

void func_801E4090(){}

void func_801E40E8(){}

void func_801E4194(){}

void func_801E4390(){}

void func_801E43BC(){}

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

void func_801E9190(){}

void func_801E9224(){}

void func_801E92B8(){}

void func_801E9310(){}

void func_801E9690(){}

void func_801E96F0(){}

void func_801E9774(){}

void func_801E98E4(){}

void CItemBoxInfo2::OnFileEvent() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_801EABC4(){}
