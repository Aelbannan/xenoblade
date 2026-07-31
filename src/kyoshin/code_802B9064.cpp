#include <types.h>
#include "kyoshin/cf/CBattleManager.hpp"

extern "C" bool func_802A330C(u32, u32);
extern "C" bool func_802A34E4(u32);
extern "C" void func_802A3D54(void*, u32, u32);

struct BattleIf {
    virtual void vf8();
    virtual void vfC();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual void vf30();
    virtual void vf34();
    virtual void vf38();
    virtual void vf3C();
    virtual void vf40();
    virtual void vf44();
    virtual void vf48();
    virtual void vf4C();
    virtual void vf50();
    virtual void vf54();
    virtual void vf58();
    virtual void vf5C();
    virtual void vf60();
    virtual void vf64();
    virtual void vf68();
    virtual void vf6C();
    virtual void vf70();
    virtual void vf74();
    virtual void vf78();
    virtual void vf7C();
    virtual void vf80();
    virtual void vf84();
    virtual void vf88();
    virtual void vf8C();
    virtual void vf90();
    virtual void vf94();
    virtual void vf98();
    virtual void vf9C();
    virtual void vfA0();
    virtual void vfA4();
    virtual void vfA8();
    virtual void vfAC();
    virtual void vfB0();
    virtual void vfB4();
    virtual void vfB8();
    virtual void vfBC();
    virtual void vfC0();
    virtual void vfC4();
    virtual void vfC8();
    virtual void vfCC();
    virtual void vfD0();
    virtual void vfD4();
    virtual void vfD8();
    virtual void vfDC();
    virtual void vfE0();
    virtual void vfE4();
    virtual void vfE8();
    virtual void vfEC();
    virtual void vfF0();
    virtual void vfF4();
    virtual void vfF8();
    virtual void vfFC();
    virtual void vf100();
    virtual void vf104();
    virtual void vf108();
    virtual void vf10C();
    virtual void vf110();
    virtual void vf114();
    virtual void vf118();
    virtual void vf11C();
    virtual void vf120();
    virtual void vf124();
    virtual void vf128();
    virtual void vf12C();
    virtual void vf130();
    virtual void vf134();
    virtual void vf138();
    virtual void vf13C();
    virtual void vf140();
    virtual void vf144();
    virtual void vf148();
    virtual void vf14C();
    virtual void vf150();
    virtual void vf154();
    virtual void vf158();
    virtual f32 vf15C();
    virtual void vf160();
    virtual void vf164();
    virtual void vf168();
    virtual void vf16C();
    virtual void vf170();
    virtual void vf174();
    virtual void vf178();
    virtual void vf17C();
    virtual void vf180();
    virtual void vf184();
    virtual void vf188();
    virtual void vf18C();
    virtual void vf190();
    virtual void vf194();
    virtual void vf198();
    virtual void vf19C();
    virtual void vf1A0();
    virtual void vf1A4();
    virtual void vf1A8();
    virtual void vf1AC();
    virtual void vf1B0();
    virtual void vf1B4();
    virtual void vf1B8();
    virtual void vf1BC();
    virtual void vf1C0();
    virtual void vf1C4();
    virtual void vf1C8();
    virtual void vf1CC();
    virtual void vf1D0();
    virtual void vf1D4();
    virtual void vf1D8();
    virtual void vf1DC();
    virtual void vf1E0();
    virtual void vf1E4();
    virtual void vf1E8();
    virtual void vf1EC();
    virtual void vf1F0();
    virtual void vf1F4();
    virtual void vf1F8();
    virtual void vf1FC();
    virtual void vf200();
    virtual void vf204();
    virtual void vf208();
    virtual void vf20C();
    virtual void vf210();
    virtual void vf214();
    virtual void vf218();
    virtual void vf21C();
    virtual void vf220();
    virtual void vf224();
    virtual void vf228();
    virtual void vf22C();
    virtual void vf230();
    virtual void vf234();
    virtual void vf238();
    virtual void vf23C();
    virtual void vf240();
    virtual void vf244();
    virtual void vf248();
    virtual void vf24C();
    virtual void vf250();
    virtual void vf254();
    virtual void vf258();
    virtual void vf25C();
    virtual void vf260();
    virtual void vf264();
    virtual void vf268();
    virtual void vf26C();
    virtual void vf270();
    virtual void vf274();
    virtual void vf278();
    virtual void vf27C();
    virtual void vf280();
    virtual void vf284();
    virtual void vf288();
    virtual void vf28C();
    virtual void vf290();
    virtual void vf294();
    virtual void vf298();
    virtual void vf29C();
    virtual void vf2A0();
    virtual void vf2A4();
    virtual void vf2A8();
    virtual void vf2AC();
    virtual void vf2B0();
    virtual void vf2B4();
    virtual void vf2B8();
    virtual bool vf2BC();
};

extern "C" bool func_802B9064(void* obj, f32 f1, f32 f2) {
    u32* obj32 = (u32*)obj;
    
    if (!(obj32[0x3F00 / 4] & 2)) {
        return false;
    }
    
    if (f1 <= f2) {
        return false;
    }
    
    if (reinterpret_cast<BattleIf*>(obj)->vf2BC()) {
        return false;
    }
    
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    if (*(s16*)((u8*)bm + 0x20C8) != 0) {
        return false;
    }
    
    f32 a = f1 / reinterpret_cast<BattleIf*>(obj)->vf15C();
    f32 b = f2 / reinterpret_cast<BattleIf*>(obj)->vf15C();
    
    u32 voiceID;
    if (b < 1.0f && 1.0f <= a) {
        voiceID = 0xA2A;
    } else if (b < 0.5f && 0.5f <= a) {
        voiceID = 0xA29;
    } else {
        return false;
    }
    
    if (!func_802A330C(0x7D, 1)) {
        return false;
    }
    if (!func_802A34E4(0x20)) {
        return false;
    }
    if (obj != NULL) {
        obj = (u8*)obj + 0x3E9C;
    }
    func_802A3D54(obj, voiceID, 0x7D);
    return false;
}
