// Auto-scaffolded catalog TU for kyoshin/CItemBoxGrid
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CItemBoxGrid.hpp"

u8 GetField61(u8* self) { return ((u8*)self)[0x61]; }










void __ct__801C5514(){}

void SetEntry9Bytes(unsigned char* p, unsigned short a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f, unsigned char g, unsigned char h) {
    unsigned char* buf = (unsigned char*)p;
    *((unsigned short*)(buf + 0)) = a;
    buf[2] = b;
    buf[3] = c;
    buf[4] = d;
    buf[5] = e;
    buf[6] = f;
    buf[7] = g;
    buf[8] = h;
}

void func_801C562C(){}

void __dt__801C5670(){}

void func_801C56D8(){}

void func_801C5E5C(){}

void func_801C5EF4(){}

void func_801C5F20(){}

void func_801C5F48(){}

void func_801C5FC0(){}

void func_801C6158(){}

void func_801C618C(){}

void func_801C62AC(){}

void func_801C631C(){}

void func_801C6388(){}

void func_801C6528(){}

void func_801C65A0(){}

void func_801C6618(){}

void func_801C6690(){}

void func_801C6708(){}

void func_801C673C(){}

void func_801C6770(){}

void func_801C67F8(){}

void func_801C6840(){}

int LookupIndexedByte(char* obj) {
    char off = *(signed char*)((char*)obj + 0x2804);
    if (off >= 0x400) return 0;
    return *(unsigned char*)((char*)obj + off + 0x28a5);
}

void func_801C68A0(){}

void func_801C6938(){}

void func_801C6A44(){}

void func_801C6E90(){}

void func_801C6EC0(){}

void func_801C7730(){}

void CopyEntry9Bytes(char* dst, const char* src) {
    *(short*)((char*)dst + 0) = *(short*)((char*)src + 0);
    *(char*)((char*)dst + 2) = *(char*)((char*)src + 2);
    *(char*)((char*)dst + 3) = *(char*)((char*)src + 3);
    *(char*)((char*)dst + 4) = *(char*)((char*)src + 4);
    *(char*)((char*)dst + 5) = *(char*)((char*)src + 5);
    *(char*)((char*)dst + 6) = *(char*)((char*)src + 6);
    *(char*)((char*)dst + 7) = *(char*)((char*)src + 7);
    *(char*)((char*)dst + 8) = *(char*)((char*)src + 8);
}

void func_801C7958(){}

void func_801C7C7C(){}

void func_801C7EF0(){}

void func_801C81D0(){}

void func_801C82D0(){}

void func_801C83E4(){}

void func_801C8534(){}

void func_801C8634(){}

void func_801C87CC(){}

void func_801C88B0(){}

void func_801C8994(){}

void func_801C8ACC(){}

void func_801C8C58(){}

void func_801C8DE4(){}

void func_801C8F04(){}

void func_801C9040(){}

void func_801C9158(){}

void func_801C9270(){}

void func_801C9390(){}

void func_801C94E0(){}

void func_801C9630(){}

void func_801C9780(){}

void func_801C98D0(){}

void func_801C9A3C(){}

void func_801C9B8C(){}

void func_801C9CCC(){}

void func_801C9E1C(){}

void func_801C9F88(){}

void func_801CA070(){}

void func_801CA110(){}

void __ct__CItemBoxGrid(){}

void CItemBoxGrid::~CItemBoxGrid() {}

void func_801CAA6C(){}

void func_801CABC8(){}

void func_801CAD8C(){}

void func_801CAE9C(){}

void func_801CB038(){}


void func_801CB0FC(){}

void func_801CB184(){}

u8 GetField549(u8* self) { return ((u8*)self)[0x549]; }

void func_801CB1E4(){}

void func_801CB28C(){}

void func_801CB38C(){}

void func_801CB480(){}

void PushToList(unsigned char *self, unsigned char val) {
    unsigned char count = self[0x6e];
    if (count >= 0xc) {
        return;
    }
    self[0x62 + count] = val;
    self[0x6e] = count + 1;
}

void func_801CB4E4(){}

void func_801CB56C(){}

void func_801CB5F0(){}

unsigned short ArrayGet12(const unsigned short* p, unsigned char i) {
    if (i < 12) {
        return p[i];
    }
    return 0;
}

void func_801CB9D8(){}

void func_801CBA04(){}

void func_801CBDE8(){}

void func_801CC0EC(){}

void func_801CC3F4(){}

void func_801CC4E8(){}

void func_801CC5DC(){}

void func_801CC7B0(){}

void func_801CCAF0(){}

void func_801CDB94(){}

void func_801CDBE0(){}

u8 GetField52D(u8* self) { return ((u8*)self)[0x52D]; }

void func_801CDC40(){}

void func_801CDEE8(){}

void func_801CDFB4(){}

void func_801CE108(){}

void func_801CE1A0(){}

float lbl_eu_80667F78;

namespace nw4r { namespace lyt { class AnimTransform; } }
void func_80137444(nw4r::lyt::AnimTransform*, float);

void CheckState4_Animate(char* self) {
    if (*(unsigned char*)((char*)self + 0x527) != 4) {
        return;
    }
    func_80137444(*(nw4r::lyt::AnimTransform**)((char*)self + 0x50), lbl_eu_80667F78);
}

void func_801CE2F8(){}

void func_801CE390(){}

void func_801CE3E8(){}

void func_801CE4B4(){}

void func_801CE524(){}

void func_801CE974(){}

void func_801CE9E8(){}

void func_801CEA30(){}

void func_801CEAA0(){}

void func_801CEAE8(){}

void func_801CEB3C(){}

void func_801CEBF0(){}

void func_801CEC80(){}

void func_801CECD0(){}

void func_801CF240(){}

void func_801CF71C(){}

void func_801CF900(){}

void func_801CFA58(){}

void func_801CFCBC(){}

void func_801CFD2C(){}

void func_801CFF28(){}

void func_801CFFEC(){}

void func_801D0328(){}

void func_801D05D4(){}

void func_801D0950(){}

void func_801D0BD8(){}

void func_801D0E88(){}

void func_801D11B8(){}

void func_801D1220(){}

void func_801D12D4(){}

void CItemBoxGrid::OnFileEvent() {}

void CopyVec4s(short* dst, const short* src) { dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2]; dst[3] = src[3]; }

void func_801D1F9C(){}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_801D1E30(){}

extern u8 lbl_eu_805347F8[];

