#include "kyoshin/cf/chain/CChain.hpp"

struct ChIf {
    virtual void _v0008();
    virtual void _v000C();
    virtual void _v0010();
    virtual void _v0014();
    virtual void _v0018();
    virtual void _v001C();
    virtual void _v0020();
    virtual void _v0024();
    virtual void _v0028();
    virtual void _v002C();
    virtual void _v0030();
    virtual void _v0034();
    virtual void _v0038();
    virtual void _v003C();
    virtual void _v0040();
    virtual void _v0044();
    virtual void _v0048();
    virtual void vf004C();
    virtual void vf0050();
};
struct ChIf2 {
    virtual void _v0008();
    virtual void _v000C();
    virtual void _v0010();
    virtual void _v0014();
    virtual void _v0018();
    virtual void _v001C();
    virtual void _v0020();
    virtual void _v0024();
    virtual void _v0028();
    virtual void _v002C();
    virtual void _v0030();
    virtual void _v0034();
    virtual void _v0038();
    virtual void _v003C();
    virtual void _v0040();
    virtual void _v0044();
    virtual void _v0048();
    virtual void _v004C();
    virtual void _v0050();
    virtual void _v0054();
    virtual void _v0058();
    virtual void _v005C();
    virtual void _v0060();
    virtual void _v0064();
    virtual void _v0068();
    virtual void _v006C();
    virtual void _v0070();
    virtual void _v0074();
    virtual void _v0078();
    virtual void _v007C();
    virtual void _v0080();
    virtual void _v0084();
    virtual void _v0088();
    virtual void _v008C();
    virtual void _v0090();
    virtual void _v0094();
    virtual void _v0098();
    virtual void _v009C();
    virtual void _v00A0();
    virtual void _v00A4();
    virtual void _v00A8();
    virtual void _v00AC();
    virtual void _v00B0();
    virtual void _v00B4();
    virtual void _v00B8();
    virtual void _v00BC();
    virtual void _v00C0();
    virtual void _v00C4();
    virtual void _v00C8();
    virtual void _v00CC();
    virtual void _v00D0();
    virtual void _v00D4();
    virtual void _v00D8();
    virtual void _v00DC();
    virtual void _v00E0();
    virtual void _v00E4();
    virtual void _v00E8();
    virtual void _v00EC();
    virtual void _v00F0();
    virtual void _v00F4();
    virtual void _v00F8();
    virtual void _v00FC();
    virtual void _v0100();
    virtual void _v0104();
    virtual void _v0108();
    virtual void _v010C();
    virtual void _v0110();
    virtual void _v0114();
    virtual void _v0118();
    virtual void _v011C();
    virtual void _v0120();
    virtual void _v0124();
    virtual void _v0128();
    virtual void _v012C();
    virtual void _v0130();
    virtual void _v0134();
    virtual void _v0138();
    virtual void _v013C();
    virtual void _v0140();
    virtual void _v0144();
    virtual void _v0148();
    virtual void _v014C();
    virtual void _v0150();
    virtual void _v0154();
    virtual void _v0158();
    virtual void _v015C();
    virtual void _v0160();
    virtual void _v0164();
    virtual void _v0168();
    virtual void _v016C();
    virtual void _v0170();
    virtual void _v0174();
    virtual void _v0178();
    virtual void _v017C();
    virtual void _v0180();
    virtual void _v0184();
    virtual void _v0188();
    virtual void _v018C();
    virtual void _v0190();
    virtual void _v0194();
    virtual void _v0198();
    virtual void _v019C();
    virtual void _v01A0();
    virtual void _v01A4();
    virtual void _v01A8();
    virtual void _v01AC();
    virtual void _v01B0();
    virtual void _v01B4();
    virtual void _v01B8();
    virtual void _v01BC();
    virtual void _v01C0();
    virtual void _v01C4();
    virtual void _v01C8();
    virtual void _v01CC();
    virtual void _v01D0();
    virtual void _v01D4();
    virtual void _v01D8();
    virtual void _v01DC();
    virtual void _v01E0();
    virtual void _v01E4();
    virtual void _v01E8();
    virtual void _v01EC();
    virtual void _v01F0();
    virtual void _v01F4();
    virtual void _v01F8();
    virtual void _v01FC();
    virtual void _v0200();
    virtual void _v0204();
    virtual void _v0208();
    virtual void _v020C();
    virtual void _v0210();
    virtual void _v0214();
    virtual void _v0218();
    virtual void _v021C();
    virtual void _v0220();
    virtual void _v0224();
    virtual void _v0228();
    virtual void _v022C();
    virtual void _v0230();
    virtual void _v0234();
    virtual void _v0238();
    virtual void _v023C();
    virtual void _v0240();
    virtual void _v0244();
    virtual void _v0248();
    virtual void _v024C();
    virtual void _v0250();
    virtual void _v0254();
    virtual void _v0258();
    virtual void _v025C();
    virtual void _v0260();
    virtual void _v0264();
    virtual void _v0268();
    virtual void _v026C();
    virtual void _v0270();
    virtual void _v0274();
    virtual void _v0278();
    virtual void _v027C();
    virtual void _v0280();
    virtual void _v0284();
    virtual void _v0288();
    virtual void _v028C();
    virtual void _v0290();
    virtual void _v0294();
    virtual void _v0298();
    virtual void _v029C();
    virtual void _v02A0();
    virtual void _v02A4();
    virtual void _v02A8();
    virtual void _v02AC();
    virtual void _v02B0();
    virtual void _v02B4();
    virtual void _v02B8();
    virtual void vf02BC();
};

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
extern "C" u16 CChain_getChainCount(u8* self) { return *(u16*)((u8*)*(void**)self + 0x3f28); }

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
extern "C" void func_8027711C(void* self);
extern "C" void func_80277B34(void* self) { func_8027711C(self); }
extern "C" void CChain_noop_78E00() {}
extern "C" void func_80278F5C(u8* self) { reinterpret_cast<ChIf*>((u8*)*(void**)self + 0x3e9c)->vf0050(); }
extern "C" void func_80278F70(u8* self) { reinterpret_cast<ChIf*>((u8*)*(void**)self + 0x3e9c)->vf004C(); }
extern "C" void CChain_noop_795D0() {}
extern "C" void CChain_noop_79768() {}
extern "C" void func_8027A324(u8* self) { reinterpret_cast<ChIf2*>(*(void**)self)->vf02BC(); }
extern "C" void CChain_noop_A9E8() {}
extern "C" void CChain_noop_AA0C() {}

void func_80276C30(){}
void func_80276C58(){}
void func_80276CAC(){}
void func_80276D30(){}
#pragma push
#pragma auto_inline off
extern "C" void func_8027711C(void* self){}
#pragma pop
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
