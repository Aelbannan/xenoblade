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
extern "C" void func_802A2CF0() {}
extern "C" void func_802A34E4() {}
extern "C" unsigned int func_802A35A0(unsigned int value) { extern unsigned int lbl_eu_80664A5C; unsigned int counter = lbl_eu_80664A5C; lbl_eu_80664A5C = counter + 1; return (counter << 16) | (value & 0xFFFF); }
extern "C" int func_802A3740(void* self) { return 0; }

extern "C" void __ct__CCharVoiceMan() {}
extern "C" void func_802A14B8() {}
extern "C" void func_802A1500() {}
extern "C" void func_802A1610() {}
extern "C" void func_802A1C68() {}
extern "C" void func_802A1D04() {}
extern "C" void func_802A1DA8() {}
extern "C" void func_802A1DF0() {}
extern "C" void func_802A1EA8() {}
extern "C" void func_802A1FB4() {}
extern "C" void func_802A201C() {}
extern "C" void func_802A2078() {}
extern "C" void func_802A216C() {}
extern "C" void func_802A2210() {}
extern "C" void func_802A2250() {}
extern "C" void func_802A232C() {}
extern "C" void func_802A2424() {}
extern "C" void func_802A24B4() {}
extern "C" void func_802A2558() {}
extern "C" void func_802A25EC() {}
extern "C" void func_802A2648() {}
extern "C" void func_802A26D8() {}
extern "C" void func_802A27F4() {}
extern "C" void func_802A285C() {}
extern "C" void func_802A28C4() {}
extern "C" void func_802A293C() {}
extern "C" void func_802A29A4() {}
extern "C" void func_802A2A0C() {}
extern "C" void func_802A2A74() {}
extern "C" void func_802A2ADC() {}
extern "C" void func_802A2B44() {}
extern "C" void func_802A2BB0() {}
extern "C" void func_802A2C1C() {}
extern "C" void func_802A2C88() {}
extern "C" void func_802A2D0C() {}
extern "C" void func_802A2D84() {}
extern "C" void func_802A2E08() {}
extern "C" void func_802A2E68() {}
extern "C" void func_802A2EEC() {}
extern "C" void func_802A2F54() {}
extern "C" void func_802A300C() {}
extern "C" void func_802A3074() {}
extern "C" void func_802A30DC() {}
extern "C" void func_802A3144() {}
extern "C" void func_802A31AC() {}
extern "C" void func_802A3214() {}
extern "C" void func_802A3290() {}
extern "C" void func_802A330C() {}
extern "C" void func_802A35B8() {}
extern "C" void func_802A3680() {}
extern "C" void func_802A3748() {}
extern "C" void CCharVoiceMan_FactoryEvent2() {}
extern "C" void func_802A38C8() {}
