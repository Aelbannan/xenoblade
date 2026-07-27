// Auto-scaffolded catalog TU for kyoshin/menu/CMenuQstCnt
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/CMenuQstCnt.hpp"

struct QstEntry {
    unsigned short f0;
    unsigned short f2;
    unsigned char f4;
    unsigned char f5;
    unsigned char f6;
};

struct QstInfo {
    unsigned short f0;
    unsigned char f2;
    unsigned char f3;
    unsigned char f4;
    unsigned char f5;
    unsigned char f6;
    unsigned char f7;
};

extern "C" {
void __dt__11CMenuQstCntFv();
void cbRenderBefore__11CMenuQstCntFv();
}

void __ct__CMenuQstCnt(){}

void setQstEntry(QstEntry* self, short a, short b, unsigned char c, unsigned char d, unsigned char e)
{
    self->f0 = a;
    self->f2 = b;
    self->f4 = c;
    self->f5 = d;
    self->f6 = e;
}

void copyQstEntry(QstEntry* dst, const QstEntry* src) {
    dst->f0 = src->f0;
    dst->f2 = src->f2;
    dst->f4 = src->f4;
    dst->f5 = src->f5;
    dst->f6 = src->f6;
}

CMenuQstCnt::~CMenuQstCnt() {}

void CMenuQstCnt::Init() {}

void CMenuQstCnt::Term() {}

void CMenuQstCnt::Move() {}

void CMenuQstCnt::cbRenderBefore() {}

void func_802269D8(){}

void func_80226B94(){}

struct Unk80226BA4 {
    unsigned char pad[0x64];
    unsigned char flag;
};

Unk80226BA4* lbl_eu_80664720;

void invalidateQstFlag() {
    if (lbl_eu_80664720 != 0) {
        lbl_eu_80664720->flag = 1;
    }
}

void func_80226BBC(){}

void func_80226C18(){}

void func_80226C5C(){}

void func_80226C88(){}

void copyQstEntry2(QstEntry* dest, const QstEntry* src) {
    dest->f0 = src->f0;
    dest->f2 = src->f2;
    dest->f4 = src->f4;
    dest->f5 = src->f5;
    dest->f6 = src->f6;
}

void func_80226E54(){}

void dtorQstBase_6C(void* self) { ((void(*)(void*))__dt__11CMenuQstCntFv)((char*)self - 0x6c); }

void renderQstBase_70(void* self) { ((void(*)(void*))cbRenderBefore__11CMenuQstCntFv)((char*)self - 0x70); }

void dtorQstBase_70(void* self) { ((void(*)(void*))__dt__11CMenuQstCntFv)((char*)self - 0x70); }

void func_80226FAC(){}

void initQstInfo(QstInfo* ptr) {
    ptr->f0 = 0;
    ptr->f2 = 0;
    ptr->f3 = 0;
    ptr->f4 = 0;
    ptr->f5 = 0;
    ptr->f6 = 0;
    ptr->f7 = 0;
}

void __dt__80227030(){}

void __dt__80227070(){}

void func_802270CC(){}

void copyQstInfo(QstInfo* dst, const QstInfo* src) {
    dst->f0 = src->f0;
    dst->f2 = src->f2;
    dst->f3 = src->f3;
    dst->f4 = src->f4;
    dst->f5 = src->f5;
    dst->f6 = src->f6;
    dst->f7 = src->f7;
}

void func_80227260(){}

void func_80227660(){}

void func_8022769C(){}

struct QstData {
    u8 _pad[0x2000];
    u16 field_2000;  // 0x2000
    s16 field_2002;  // 0x2002
};

unsigned short selectQstIndex(unsigned char* p) {
    QstData* data = reinterpret_cast<QstData*>(p);
    short v = data->field_2002;
    if (v < 0) {
        v = data->field_2000;
    }
    return (unsigned short)v;
}

void func_802276F4(){}

void func_80227710(){}
