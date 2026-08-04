#include "kyoshin/cf/chain/CChain.hpp"

namespace cf {
    CChain::CChain(){
        
    }

    bool CChain::chkActorList() {
        extern bool func_8027B770(void*);
        return func_8027B770((char*)this + 0x18);
    }



}

extern "C" int CChain_getZero_78E04() { return 0; }
extern "C" int CChain_getZero_A584() { return 0; }
extern "C" int CChain_getZero_A9D0() { return 0; }
extern "C" int CChain_getZero_A9D8() { return 0; }
extern "C" int CChain_getZero_A9E0() { return 0; }
extern "C" int CChain_getZero_A9EC() { return 0; }
extern "C" int CChain_getZero_A9F4() { return 0; }
extern "C" int CChain_getZero_A9FC() { return 0; }
extern "C" int CChain_getZero_AA04() { return 0; }
extern "C" u16 CChain_getChainCount(void* self) { return *(u16*)((u8*)*(void**)self + 0x3f28); }

bool CChain_isValidChain(unsigned char* a1, unsigned char* a2, int a3) {
    unsigned char v = a1[2];
    if (v < 1 || v > 0x18) return false;
    if (v == 5) {
        if (*(unsigned short*)(a2 + 0x3f28) != 5) return false;
        if (a3 == 0) return false;
    }
    return true;
}

void func_80277154(){}
void func_80277A7C(){}
void func_80277B34(void){}
extern "C" void CChain_noop_78E00() {}
void func_80278F5C(){}
void func_80278F70(){}
extern "C" void CChain_noop_795D0() {}
extern "C" void CChain_noop_79768() {}
void func_8027A324(){}
extern "C" void CChain_noop_A9E8() {}
extern "C" void CChain_noop_AA0C() {}

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

extern "C" void CChain_setFieldAndClear(void* self, int val) {
    *(int*)self = val;
    *(u16*)((u8*)self + 0x6c) = 0;
}
