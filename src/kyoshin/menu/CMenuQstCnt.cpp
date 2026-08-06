// Auto-scaffolded catalog TU for kyoshin/menu/CMenuQstCnt
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/code_80135FDC.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

#include "kyoshin/menu/CMenuQstCnt.hpp"

struct QstEntry {
    unsigned short f0;
    unsigned short f2;
    unsigned char f4;
    unsigned char f5;
    unsigned char f6;

    void setQstEntry(short a, short b, unsigned char c, unsigned char d, unsigned char e);
};

void QstEntry::setQstEntry(short a, short b, unsigned char c, unsigned char d, unsigned char e) {
    f0 = a;
    f2 = b;
    f4 = c;
    f5 = d;
    f6 = e;
}

struct QstInfo {
    unsigned short f0;
    unsigned char f2;
    unsigned char f3;
    unsigned char f4;
    unsigned char f5;
    unsigned char f6;
    unsigned char f7;
};

// The quest-status menu object. Fields are read/written by the address-named
// update helpers below; only the tail (0x88 onward) is referenced here.
struct QstMenuData {
    u8 mPad88[0x88];                            // 0x00
    nw4r::lyt::AnimTransform* mAnim88;          // 0x88
    u32 mState8C;                               // 0x8C
    f32 mValue90;                               // 0x90
};

extern "C" int func_80144FF0();
extern "C" int func_80145030(void);
extern void func_80138078(u32);

extern "C" {
void __dt__11CMenuQstCntFv();
void cbRenderBefore__11CMenuQstCntFv();
}

void __ct__CMenuQstCnt(){}

// Converted to inline member function in struct

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

struct Unk80226BA4 {
    unsigned char pad[0x64];
    unsigned char flag;
};

Unk80226BA4* lbl_eu_80664720;

int func_80226B94() {
    // subic/subfe is-zero idiom: returns whether the global menu object is unallocated
    return lbl_eu_80664720 == 0;
}

void invalidateQstFlag() {
    if (lbl_eu_80664720 != 0) {
        lbl_eu_80664720->flag = 1;
    }
}

// Quest-log gating: when the CF-game resource gate reports the scene is open
// (not mid-load) and the input state allows it, play the sound and set state 1.
void func_80226BBC(QstMenuData* self) {
    if (func_80144FF0() == 0 || cf::CfGameManager::func_800829B8() || func_80145030()) {
        func_80138078(0x1f);
        self->mState8C = 1;
    }
}

// Advance the open/close animation at 0x88 by one frame; when it finishes the
// animation (func_80137444 returns 1), move to state 2.
void func_80226C18(QstMenuData* self) {
    if (func_80137444(self->mAnim88, 1.0f) != 0) {
        self->mState8C = 2;
    }
}

// Per-frame timer: 0x90 counts frames (1 per call); once it reaches 90 frames
// it clamps to state 3. Uses >= (cror eq,gt,eq; bnelr returns while below).
void func_80226C5C(QstMenuData* self) {
    self->mValue90 += 1.0f;
    if (self->mValue90 >= 90.0f) {
        self->mState8C = 3;
    }
}

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
    if (v < 0)
        return data->field_2000;
    return (unsigned short)v;
}

QstInfo* func_802276F4(QstInfo* base, unsigned short idx) {
    // clrlslwi 16,3 = (idx & 0xFFFF) << 3 -> pointer into an 8-byte QstInfo array
    if (idx >= 0x400) return 0;
    return base + idx;
}

void func_80227710(){}
