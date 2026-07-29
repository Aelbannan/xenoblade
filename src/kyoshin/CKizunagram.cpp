// Auto-scaffolded catalog TU for kyoshin/CKizunagram
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CKizunagram.hpp"

void func_8025C870() {}










void func_80257F9C(){}

CKizunaLine::CKizunaLine() {}

CKizunaLine::~CKizunaLine() {}

void func_802580CC(){}

void func_80258F5C(){}

void func_80258F80(){}

void func_80258F9C(){}

void func_80259098(){}

#pragma push
#pragma auto_inline off
void func_80259228(void* subobj) {
    u8* b = static_cast<u8*>(subobj);
    b[0x14] = 1;
    b[0x15] = 0;
}
#pragma pop

void func_80259280(){}

void func_802592D8(){}

void func_80259344(){}

void func_80259394(){}

void func_8025949C(){}

void func_80259820(){}

void CKizunagram_copyShortString(char* dest, const char* src) { dest[0] = src[0]; dest[1] = src[1]; dest[2] = src[2]; dest[3] = 0; }

void func_80259B18(){}

void func_80259C5C(){}

void func_80259D44(){}

void func_80259DE8(){}

void func_8025A11C(){}

void func_8025AA38(){}

void func_8025AAE0(){}

void func_8025AB04(){}

void func_8025AB84(){}

extern float lbl_eu_80668828[];
void CKizunagram_resetFields(void* self){
    *(u8*)((u8*)self + 0x34) = 0;
    *(u16*)((u8*)self + 0x36) = 0;
    *(float*)((u8*)self + 0x38) = lbl_eu_80668828[0];
}

void func_8025AC1C(){}

void CKizunagram_copyString(unsigned char* dst, const unsigned char* src) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = 0;
}

CKizunaInfo::CKizunaInfo() {}

CKizunaInfo::~CKizunaInfo() {}

void func_8025B670(){}

void func_8025B870(){}

void func_8025B900(){}

void func_8025B958(){}

void func_8025B9C8(){}

void func_8025BA38(){}

void func_8025C16C(){}

void func_8025C21C(){}

void func_8025C298(){}

void func_8025C348(){}

CKizunagram::CKizunagram() {}

CKizunagram::~CKizunagram() {}

void func_8025C510(){}

void func_8025C580(){}

void func_8025C61C(){}

void func_8025C6F0(){}

unsigned char CKizunagram_getField7E(void* this_ptr) {
    if (*(unsigned char*)((unsigned char*)this_ptr + 0x7e) != 0) {
        return *(unsigned char*)((unsigned char*)this_ptr + 0x3b);
    } else {
        return 0;
    }
}

unsigned char CKizunagram_checkFields(void* arg1)
{
    if (*(unsigned char*)((char*)arg1 + 0x61) == 0)
        return 0;
    if (*(unsigned char*)((char*)arg1 + 0x7d) == 0)
        return 0;
    if (*(unsigned char*)((char*)arg1 + 0xbd) != 0)
        return *(unsigned char*)((char*)arg1 + 0x3c);
    return 0;
}

void func_8025C7D0(){}

void func_8025C7FC(){}


void func_8025C874(){}

void func_8025C904(){}

void func_8025C994(){}

void func_8025CA24(){}

void func_8025CAB4(){}

void func_8025CAE4(){}

void func_8025CB50(){}

int CKizunagram_stub(void* self) { return 0; }

struct CKizunagramState {
    u8 _00[0x8C];
    u8 field8C;
};

u8 CKizunagram_getField8C(void* self) {
    return ((CKizunagramState*)self)->field8C;
}

void func_8025CBCC(){}

void func_8025CC70(){}

void func_8025CC88(CKizunagram* self) {
    if (self->field_0x62 == 0) return;
    self->_38[1] = 2;
    func_80259228(reinterpret_cast<u8*>(self) + 0x68);
}

void func_8025CCA8(){}

void CKizunagram_setField39(void* arg) {
    unsigned char* ptr = (unsigned char*)arg;
    if (ptr[0x62] != 0) {
        ptr[0x39] = 5;
    }
}

void CKizunagram_resetState(void* self) {
    uint8_t* b = static_cast<uint8_t*>(self);
    if (!b[0x61]) return;
    if (!b[0x7d]) return;
    b[0x39] = 0;
    b[0x3c] = 1;
    b[0x38] = 0;
}

void func_8025CD40(){}

void func_8025CE00(){}

void func_8025CE78(){}

void func_8025CF1C(){}

void func_8025CF40(){}

void CKizunagram::OnFileEvent() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_8025D304(){}
