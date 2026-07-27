#include "kyoshin/cf/voice/CCharVoiceMan.hpp"
#include "kyoshin/UnkClass_805764CC.hpp"

namespace cf{
    CCharVoiceMan::CCharVoiceMan(){
        unk204 = 0;
        unk208 = 0;
        unk20C = 0;
        unk210 = 0;
        unk214 = 1;
        unk218 = 0;
        unk21C = 0;
        unk220 = 0;
        unk221 = 0;
        unk222 = 0;
        unk223 = 0;
        unk224 = 0;
        unk229 = 0;
        unk22A = 0;
        unk22C = 0;
        unk230 = 0;
        UnkClass_805764CC* classPtr = UnkClass_805764CC::func_800B07E8();
        classPtr->func_800B8804(this);
    }

    CCharVoiceMan::~CCharVoiceMan(){
        UnkClass_805764CC* classPtr = UnkClass_805764CC::func_800B07E8();
        classPtr->func_800B88E0(this);
    }
}

extern "C" bool func_802A1EA0() { return true; }
extern "C" unsigned char* lbl_eu_80664A58;
extern "C" void func_802A1F9C() {
    unsigned char* voice_man = lbl_eu_80664A58;
    if (voice_man != 0)
        voice_man[0x222] = 0;
}
void func_802A2CF0(){}
void func_802A34E4(){}
extern "C" unsigned int func_802A35A0(unsigned int value) { extern unsigned int lbl_eu_80664A5C; unsigned int counter = lbl_eu_80664A5C; lbl_eu_80664A5C = counter + 1; return (counter << 16) | (value & 0xFFFF); }
extern "C" int func_802A3740(void* self) { return 0; }

void __ct__CCharVoiceMan(){}
void func_802A14B8(){}
void func_802A1500(){}
void func_802A1610(){}
void func_802A1C68(){}
void func_802A1D04(){}
void func_802A1DA8(){}
void func_802A1DF0(){}
void func_802A1EA8(){}
void func_802A1FB4(){}
void func_802A201C(){}
void func_802A2078(){}
void func_802A216C(){}
void func_802A2210(){}
void func_802A2250(){}
void func_802A232C(){}
void func_802A2424(){}
void func_802A24B4(){}
void func_802A2558(){}
void func_802A25EC(){}
void func_802A2648(){}
void func_802A26D8(){}
void func_802A27F4(){}
void func_802A285C(){}
void func_802A28C4(){}
void func_802A293C(){}
void func_802A29A4(){}
void func_802A2A0C(){}
void func_802A2A74(){}
void func_802A2ADC(){}
void func_802A2B44(){}
void func_802A2BB0(){}
void func_802A2C1C(){}
void func_802A2C88(){}
void func_802A2D0C(){}
void func_802A2D84(){}
void func_802A2E08(){}
void func_802A2E68(){}
void func_802A2EEC(){}
void func_802A2F54(){}
void func_802A300C(){}
void func_802A3074(){}
void func_802A30DC(){}
void func_802A3144(){}
void func_802A31AC(){}
void func_802A3214(){}
void func_802A3290(){}
void func_802A330C(){}
void func_802A35B8(){}
void func_802A3680(){}
void func_802A3748(){}
void CCharVoiceMan_FactoryEvent2(){}
void func_802A38C8(){}
