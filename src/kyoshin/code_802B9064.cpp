#include <types.h>
#include "kyoshin/cf/CBattleManager.hpp"

extern "C" bool func_802A330C(u32, u32);
extern "C" bool func_802A34E4(u32);
extern "C" void func_802A3D54(void*, u32, u32);

static inline bool callBoolVFunc(void* obj, u32 offset) {
    void* vtable = *(void**)obj;
    return ((bool (*)(void*))(((void**)vtable)[offset / 4]))(obj);
}

static inline f32 callFloatVFunc(void* obj, u32 offset) {
    void* vtable = *(void**)obj;
    return ((f32 (*)(void*))(((void**)vtable)[offset / 4]))(obj);
}

extern "C" bool func_802B9064(void* obj, f32 f1, f32 f2) {
    u32* obj32 = (u32*)obj;
    
    if (!(obj32[0x3F00 / 4] & 2)) {
        return false;
    }
    
    if (f1 <= f2) {
        return false;
    }
    
    if (callBoolVFunc(obj, 0x2BC)) {
        return false;
    }
    
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    if (*(s16*)((u8*)bm + 0x20C8) != 0) {
        return false;
    }
    
    f32 a = f1 / callFloatVFunc(obj, 0x15C);
    f32 b = f2 / callFloatVFunc(obj, 0x15C);
    
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
