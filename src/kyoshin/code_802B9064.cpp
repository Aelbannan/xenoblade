#include <types.h>
#include "kyoshin/cf/CBattleManager.hpp"

extern "C" bool func_802A330C(u32, u32);
extern "C" bool func_802A34E4(u32);
extern "C" void func_802A3D54(void*, u32, u32);

typedef bool (*BoolVFunc)(void*);
typedef float (*FloatVFunc)(void*);

extern "C" bool func_802B9064(void* obj, float f1, float f2) {
    if (!(*(u32*)((u8*)obj + 0x3F00) & 2)) return false;
    if (!(f1 > f2)) return false;

    if (reinterpret_cast<BoolVFunc>((*reinterpret_cast<void***>(obj))[0x2BC / 4])(obj)) return false;

    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    if (*(s16*)((u8*)bm + 0x20C8) != 0) return false;

    float ratio1 = f1 / reinterpret_cast<FloatVFunc>((*reinterpret_cast<void***>(obj))[0x15C / 4])(obj);
    float ratio2 = f2 / reinterpret_cast<FloatVFunc>((*reinterpret_cast<void***>(obj))[0x15C / 4])(obj);

    u32 voiceID;
    if (ratio2 < 1.0f && ratio1 >= 1.0f) {
        voiceID = 0xA2A;
    } else if (ratio2 < 0.5f && ratio1 >= 0.5f) {
        voiceID = 0xA29;
    } else {
        return false;
    }

    if (!func_802A330C(0x7D, 1)) return false;
    if (!func_802A34E4(0x20)) return false;
    if (obj != NULL) {
        obj = (u8*)obj + 0x3E9C;
    }
    func_802A3D54(obj, voiceID, 0x7D);
    return false;
}
