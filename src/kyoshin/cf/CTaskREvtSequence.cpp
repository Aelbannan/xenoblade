// Auto-scaffolded catalog TU for kyoshin/cf/CTaskREvtSequence
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CTaskREvtSequence.hpp"

namespace cf {
    u32 CTaskREvtSequence::func_8016A354() { return field_0x20; }

    void CTaskREvtSequence::OnFileEvent() {
        extern void func_8016ABA8(void*);
        ((void(*)(void*))func_8016ABA8)((char*)this - 0x54);
    }
}

void __ct__cf_CTaskREvtSequence(){}

void __dt__801682AC(){}

cf::CTaskREvtSequence::~CTaskREvtSequence() {}

unsigned long func_801683FC(){
    extern unsigned long lbl_eu_80664268;
    unsigned long* ptr = (unsigned long*)lbl_eu_80664268;
    return (ptr[0x5c/4] >> 17) & 1;
}

unsigned long func_8016840C(){
    extern unsigned long lbl_eu_80664268;
    unsigned long* ptr = (unsigned long*)lbl_eu_80664268;
    return (ptr[0x5c/4] >> 19) & 1;
}

void func_8016841C(){}

void func_8016846C(){}

bool func_8016847C() { return false; }

void func_80168484(){}

void func_801684F4(){}

void func_80168514(){}

void func_80168610(){}

void func_801686B0(){}

void func_8016872C(){}

void func_80168800(){}

void func_80168F38(){}

int func_80169048(void* self) { return 1; }

void func_80169050(){}

void func_8016925C(){}

int func_801696C4(void* self) { return 0; }

void func_801696CC(){}

void func_80169A34() {}

void func_80169A38(){}

void func_80169CD0(){}

void func_80169DD0(){}

void func_80169F24() {}

void func_80169F28(){}

void func_8016A24C(){}

void func_8016A27C(){}

void func_8016A35C(){}

void func_8016A378(){}

void func_8016A3A8(){}

void func_8016A3C4(){}

void cf::CTaskREvtSequence::Init() {}

void cf::CTaskREvtSequence::Term() {}

void func_8016A480(){}



void func_8016ABA8(void* self){}

void func_8016AD44(){}

void func_8016ADF8(){}

void func_8016AED4(){}

void func_8016AF4C(){}

void func_8016B164(){}

void func_8016B384(){}

void func_8016B5A4(){}

void func_8016B788(){}

void func_8016B860(){}

void func_8016BB38(){}

void func_8016BC1C(){}

void func_8016BDA8(){}

void func_8016C118(){}

void cf::CTaskREvtSequence::cbRenderBefore() {}

void func_8016C2C8(){}

void func_8016C2E4(){}

void func_8016C300(){}

void func_8016C3DC(){}

void func_8016C410(){}

void func_8016C450(){}

void func_8016C6EC(){}

void func_8016C720(){}

void func_eu_8016DA48(){}

void func_8016C7D8(void* self) { reinterpret_cast<cf::CTaskREvtSequence*>((char*)self - 0x54)->~CTaskREvtSequence(); }

void func_8016C7E0(void* self) { reinterpret_cast<cf::CTaskREvtSequence*>((char*)self - 0x58)->cbRenderBefore(); }

extern "C" void func_8016C7E8(void* self) { reinterpret_cast<cf::CTaskREvtSequence*>((char*)self - 0x58)->~CTaskREvtSequence(); }

// CTTask<cf::CTaskREvtSequence> specializations provided by header
