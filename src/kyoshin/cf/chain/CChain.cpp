#include "kyoshin/cf/chain/CChain.hpp"

namespace cf {
    CChain::CChain(){
        
    }
}

void func_80277154(){}
void func_80277A7C(){}
void func_80277B34(void){}
extern "C" void func_80278E00() {}
extern "C" int func_80278E04(void* self) { return 0; }
void func_80278F5C(){}
void func_80278F70(){}
extern "C" void func_802795D0() {}
extern "C" bool func_8027B770(void*);

extern "C" bool func_8027968C(void* self) {
    return func_8027B770((char*)self + 0x18);
}
extern "C" void func_80279768() {}
extern "C" u16 func_8027976C(void** arg0) {
    return *(u16*)((char*)*arg0 + 0x3f28);
}
extern "C" bool func_802798B0(unsigned char* a1, unsigned char* a2, int a3) {
    unsigned char v = a1[2];
    if (v < 1 || v > 0x18) return false;
    if (v == 5) {
        if (*(unsigned short*)(a2 + 0x3f28) != 5) return false;
        if (a3 == 0) return false;
    }
    return true;
}
extern "C" void func_80279B24(void* _this, int val) {
    *(int*)_this = val;
    *(unsigned short*)((char*)_this + 0x6C) = 0;
}
void func_8027A324(){}
extern "C" int func_8027A584(void* self) { return 0; }
extern "C" int func_8027A9D0(void* self) { return 0; }
extern "C" int func_8027A9D8(void* self) { return 0; }
extern "C" int func_8027A9E0(void* self) { return 0; }
extern "C" void func_8027A9E8() {}
extern "C" int func_8027A9EC(void* self) { return 0; }
extern "C" int func_8027A9F4(void* self) { return 0; }
extern "C" int func_8027A9FC(void* self) { return 0; }
extern "C" int func_8027AA04(void* self) { return 0; }
extern "C" void func_8027AA0C() {}

void func_80276C30(){}
void func_80276C58(){}
void func_80276CAC(){}
void func_80276D30(){}
void func_8027711C(){}
void func_8027728C(){}
void func_8027732C(){}
void func_802773EC(){}
void func_8027750C(){}
void func_80277B38(){}
void func_80278E0C(){}
void func_80278F84(){}
void func_80279214(){}
void func_8027936C(){}
void func_802795D4(){}
void func_80279694(){}
void func_80279778(){}
void func_8027990C(){}
void func_802799F0(){}
void func_80279A4C(){}
void func_80279B34(){}
void func_80279DC0(){}
void func_80279E48(){}
void func_80279F6C(){}
void func_8027A024(){}
void func_8027A338(){}
void func_8027A58C(){}
void func_8027A8C8(){}
