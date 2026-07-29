// Auto-scaffolded catalog TU for kyoshin/CItemBoxInfo
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CItemBoxInfo.hpp"

// --- Forward declarations ---
namespace nw4r { namespace lyt { class Layout; class DrawInfo; } }
extern "C" char lbl_eu_805063BC[];
void func_80136B4C(nw4r::lyt::Layout*, char*, char*, u32);
void func_80136910(nw4r::lyt::Layout*, const char*, u8);
int sprintf(char*, const char*, ...);
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

void resetCItemBox() {}

// --- CItemBoxInfo methods ---

u8 CItemBoxInfo::getItemBoxState() {
    return state.current;
}

void CItemBoxInfo::advanceItemBoxState() {
    if (state.state == 3) {
        state.state = 4;
        state.visible = 0;
    }
}

void CItemBoxInfo::tryActivateItemBox() {
    if (state.layout == 0) return;
    if (state.resource) {
        state.current = 1;
        state.active = 1;
    }
}

u32 func_801D4AB0(void* arg) {
    u32 v = *(u32*)arg;
    u32 result = 0;
    if (((v >> 16) & 0xF) == 9) {
        u32 flags = ((u8*)arg)[7];
        if ((flags & 3) == 3u) {
            result = 1;
        }
    }
    return result;
}

void func_801D4AE0(CItemBoxInfo* info, char* arg1, char* arg2) {
    char buf[0x20];
    sprintf(buf, &lbl_eu_805063BC[0x151], arg1);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf, arg2, 0);
}

void func_801D4BDC(CItemBoxInfo* info, u8 arg2, u8 arg3) {
    if (info->state.layout != 0) {
        func_80136910((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x17d], arg2);
        func_80136910((nw4r::lyt::Layout*)info->state.layout, &lbl_eu_805063BC[0x188], arg3);
    }
}

// --- Remaining CItemBoxInfo stubs ---

void func_801D8C0C(CItemBoxInfo*);
void func_801E4194(CItemBoxInfo2*);

void func_801D421C(CItemBoxInfo* info) {
    if (info->state.state != 0) return;
    info->state.state = 1;
    info->state.visible = 0;
    func_801D8C0C(info);
    return;
}

void func_801D4154(CItemBoxInfo* info, nw4r::lyt::DrawInfo* drawInfo) {
    if (info->state.active != 0) {
        func_80137038((nw4r::lyt::Layout*)info->state.layout, drawInfo, 0, 1);
    }
}

void func_801D4A2C(){}
void func_801D4B3C(){}
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

void CItemBoxInfo::setItemBoxIndex(unsigned char index, short value) {
    if (index >= 12) return;
    state.values[index] = value;
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
void func_801D8B60(){}
void func_801D8B08(CItemBoxInfo* info) {
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x48f],
                  &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x49b],
                  &lbl_eu_805063BC[0x2aa], 0);
}

void func_801D8C0C(){}
void func_801D8E34(){}

void copyItemBoxEntry(CItemBoxInfoEntry* dst, const CItemBoxInfoEntry* src) {
    dst->itemId = src->itemId;
    dst->value = src->value;
    dst->state = src->state;
}

void CItemBoxInfoEntry::setItemBoxEntry(u16 r4, u32 r5, u8 r6) {
    itemId = r4;
    value = r5;
    state = r6;
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

void CItemBoxInfo2::drawItemBox2Layout(nw4r::lyt::DrawInfo* drawInfo) {
    if (state.active != 0) {
        func_80137038((nw4r::lyt::Layout*)state.layout, drawInfo, 0, 1);
    }
}

void func_801E13F8(){}

u8 CItemBoxInfo2::getItemBox2State() {
    return state.current;
}

void func_801E1498(CItemBoxInfo2* info) {
    if (info->state.state != 0) return;
    info->state.state = 1;
    info->state.visible = 0;
    func_801E4194(info);
}

void CItemBoxInfo2::advanceItemBox2State() {
    if (state.state == 3) {
        state.state = 4;
        state.visible = 0;
    }
}

void func_801E14DC(){}
void func_801E16F0(CItemBoxInfo2* info, char* arg1, char* arg2) {
    char buf[0x20];
    sprintf(buf, &lbl_eu_805063BC[0x151], arg1);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout, buf, arg2, 0);
}
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
void func_801E4090(CItemBoxInfo2* info) {
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x48f],
                  &lbl_eu_805063BC[0x2aa], 0);
    func_80136B4C((nw4r::lyt::Layout*)info->state.layout,
                  &lbl_eu_805063BC[0x49b],
                  &lbl_eu_805063BC[0x2aa], 0);
}
void func_801E40E8(){}
void func_801E4194(){}
void func_801E4390(CItemBoxInfo2* info) {
    if (info->state.layout == 0) return;
    if (info->state.resource) {
        info->state.current = 1;
        info->state.active = 1;
    }
}
void func_801E43BC(){}

void copyItemBoxCopy(ItemBoxInfoCopy* dst, const ItemBoxInfoCopy* src) {
    dst->itemId = src->itemId;
    dst->value = src->value;
    dst->state = src->state;
}

void CItemBoxInfoEntry::setItemBoxCopy(unsigned short a, unsigned int b, unsigned char c) {
    itemId = a;
    value = b;
    state = c;
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
