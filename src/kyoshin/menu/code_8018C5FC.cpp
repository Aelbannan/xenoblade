#include <types.h>
#include <monolib/device/CDeviceVI.hpp>
#include <functions.hpp>

namespace cf {

class UnkClass_8018C5FC {
public:
    UnkClass_8018C5FC();
    s32 unk0;
    f32 unk4;
};

extern f32 lbl_eu_80667A30;
extern f32 lbl_eu_80667A34;
extern f32 lbl_eu_80667A38;
extern u32 lbl_eu_80663E14;
extern u32 lbl_eu_80663E24;

extern "C" {
    bool func_800829B8__Q22cf13CfGameManagerFv();
    f32 func_80496288(u32*);
    void* getInstance__Q22cf14CBattleManagerFv();
    void* func_800B6BA4__Fv();
    u32 func_8026178C(void*, u32);
    void func_802A293C(s32, s32);
    void func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(u32, u32, u32, u32, f32);
}

UnkClass_8018C5FC::UnkClass_8018C5FC() {
    unk0 = 0;
    unk4 = lbl_eu_80667A30;
}

// Timer-based counter decrement: decreases the party gauge over time.
// When the timer elapses and there are no active battles, the counter
// decrements by 1 (if a qualifying party member is present) or by 2.
void func_8018C610(UnkClass_8018C5FC* _this) {
    if (!func_8009CF8C(0x3357)) return;
    if (func_800829B8__Q22cf13CfGameManagerFv()) return;
    if (lbl_eu_80663E24 & 0xafa40000) return;

    f32 scale = func_80496288(&lbl_eu_80663E14);
    _this->unk4 += CDeviceVI::getSecPerFrame() * scale;

    if (_this->unk4 >= lbl_eu_80667A34) {
        _this->unk4 = 0.0f;

        // Count active battles from the battle manager's actor list
        void* bm = getInstance__Q22cf14CBattleManagerFv();
        void** sentinel = (void**)((u8*)bm + 8);
        s32 battleCount = 0;
        for (void* node = *sentinel; node != sentinel; node = *(void**)node) {
            battleCount++;
        }

        s32 adjust;
        if (battleCount == 0) {
            // Search the party member list for a qualifying character
            void* list = func_800B6BA4__Fv();
            void** startNodePtr = (void**)((u8*)list + 4);
            void* curr = *startNodePtr;
            s32 found = 0;

            while (curr != startNodePtr) {
                void* cfObj = *(void**)((u8*)curr + 8);
                if (cfObj != 0) {
                    cfObj = (void*)((u8*)cfObj - 0x3e9c);
                }

                if (cfObj != 0) {
                    void* vtable = *(void**)cfObj;
                    typedef u32 (*VirtualFunc)(void*);
                    VirtualFunc vf = (VirtualFunc)((void**)vtable)[0x290 / 4];
                    u32 ret = vf(cfObj);

                    if (ret != 0) {
                        ret = vf(cfObj);
                        ret = func_8026178C((void*)ret, 0x69);
                    }

                    if (ret != 0) {
                        found = 1;
                        break;
                    }
                }
                curr = *(void**)curr;
            }

            adjust = found ? -1 : -2;
        } else {
            adjust = 0;
        }

        if (adjust != 0) {
            if (!func_8009CF8C(0x3357)) return;

            s32 oldVal = _this->unk0;
            s32 newVal = oldVal + adjust;
            _this->unk0 = newVal;

            if (newVal < 0) {
                _this->unk0 = 0;
            } else if (newVal > 0x12c) {
                _this->unk0 = 0x12c;
            }

            func_802A293C(_this->unk0, oldVal);

            s32 clamped = _this->unk0;
            if ((oldVal < 0x64 && clamped >= 0x64) ||
                (oldVal < 0xc8 && clamped >= 0xc8))
            {
                func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, 0x64, 0, 0, lbl_eu_80667A38);
            }
        }
    }
}

// Add a delta to the party gauge counter, clamp to [0, 300], and return the new value.
s32 func_8018C820(UnkClass_8018C5FC* _this, s32 delta) {
    s32 newVal = _this->unk0 + delta;

    if (func_8009CF8C(0x3357)) {
        s32 oldVal = _this->unk0;
        _this->unk0 = newVal;

        if (newVal < 0) {
            _this->unk0 = 0;
        } else if (newVal > 0x12c) {
            _this->unk0 = 0x12c;
        }

        func_802A293C(_this->unk0, oldVal);

        s32 clamped = _this->unk0;
        if ((oldVal < 0x64 && clamped >= 0x64) ||
            (oldVal < 0xc8 && clamped >= 0xc8))
        {
            func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, 0x64, 0, 0, lbl_eu_80667A38);
        }
    }

    return _this->unk0;
}

// Set the party gauge counter to a value, clamp to [0, 300], and return the new value.
s32 func_8018C8F4(UnkClass_8018C5FC* _this, s32 val) {
    if (func_8009CF8C(0x3357)) {
        s32 oldVal = _this->unk0;
        _this->unk0 = val;

        if (val < 0) {
            _this->unk0 = 0;
        } else if (val > 0x12c) {
            _this->unk0 = 0x12c;
        }

        func_802A293C(_this->unk0, oldVal);

        s32 clamped = _this->unk0;
        if ((oldVal < 0x64 && clamped >= 0x64) ||
            (oldVal < 0xc8 && clamped >= 0xc8))
        {
            func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, 0x64, 0, 0, lbl_eu_80667A38);
        }
    }

    return _this->unk0;
}

} // namespace cf