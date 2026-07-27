// Auto-scaffolded catalog TU for kyoshin/CItemBoxInfo
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

struct CItemBoxInfoState {
    u8 _00[0x30];
    void* resource;
    void* layout;
    u8 _38[0x52];
    u8 active;
    u8 _8B[5];
    u32 state;
    u8 current;
    u8 visible;
    u8 _9A[0x26];
    s16 values[12];
};

struct CItemBoxInfo {
    CItemBoxInfoState state;
    void OnFileEvent();
};

struct CItemBoxInfo2 {
    CItemBoxInfoState state;
    ~CItemBoxInfo2();
    void OnFileEvent();
};

void resetCItemBox() {}








u8 getItemBoxState(CItemBoxInfo* self) {
    return self->state.current;
}


void advanceItemBoxState(CItemBoxInfo* self) {
    if (self->state.state == 3) {
        self->state.state = 4;
        self->state.visible = 0;
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

void setItemBoxIndex(CItemBoxInfo* self, unsigned char index, short value) {
    if (index >= 12) return;
    self->state.values[index] = value;
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

void tryActivateItemBox(CItemBoxInfo* arg) {
    if (arg->state.layout == 0) return;
    if (arg->state.resource == 0) return;
    arg->state.current = 1;
    arg->state.active = 1;
}

void func_801D8E34(){}

struct CItemBoxInfoEntry {
    u16 itemId;
    u16 _02;
    u32 value;
    u8 state;
};

void copyItemBoxEntry(CItemBoxInfoEntry* dst, const CItemBoxInfoEntry* src) {
    *dst = *src;
}

void setItemBoxEntry(CItemBoxInfoEntry* self, u16 r4, u32 r5, u8 r6) {
    self->itemId = r4;
    self->value = r5;
    self->state = r6;
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

CItemBoxInfo2::~CItemBoxInfo2() {}

void func_801E12E0(){}

void func_801E1348(){}

namespace nw4r { namespace lyt { class Layout; class DrawInfo; } }
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

void drawItemBox2Layout(CItemBoxInfo2* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->state.active != 0) {
        func_80137038((nw4r::lyt::Layout*)self->state.layout, drawInfo, 0, 1);
    }
}

void func_801E13F8(){}

u8 getItemBox2State(CItemBoxInfo2* self) {
    return self->state.current;
}

void func_801E1498(){}

void advanceItemBox2State(CItemBoxInfo2* self) {
    if (self->state.state == 3) {
        self->state.state = 4;
        self->state.visible = 0;
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

typedef CItemBoxInfoEntry ItemBoxInfoCopy;

void copyItemBoxCopy(ItemBoxInfoCopy* dst, const ItemBoxInfoCopy* src) {
    *dst = *src;
}

void setItemBoxCopy(ItemBoxInfoCopy* self, unsigned short a, unsigned int b, unsigned char c) {
    self->itemId = a;
    self->value = b;
    self->state = c;
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
