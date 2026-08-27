#include <types.h>
#include "monolib/scn/CScnTimeApi.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include <monolib/device/CDeviceVI.hpp>
#include <functions.hpp>

// __cntlzw is a compiler builtin available as an intrinsic; drop the macro
// wrapper so we can call it directly to reproduce retail's cntlzw+srwi guard.
#undef __cntlzw

// Global-scope lbl_* (data imports); declaring them outside namespace cf keeps
// their global (unmangled) symbol name instead of cf::-mangling them.
extern const f32 lbl_eu_80667A30;
extern f32 lbl_eu_80667A34;
extern f32 lbl_eu_80667A38;

namespace cf {

// Party-gauge controller: holds the gauge counter (clamped to [0, 300])
// plus a frame-time accumulator used to tick it down during play.
class UnkClass_8018C5FC {
public:
    UnkClass_8018C5FC();
    s32 gaugeValue;   // +0x00: party gauge counter, clamped to [0, 0x12c]
    f32 tickAccum;    // +0x04: accumulated scaled frame time until next tick
};

// --- linked-list / battle-object types used by func_8018C610 ---

struct CMB_CfObjEnt_8018C5FC; // fwd

// Intrusive singly-linked node: next @ +0, data @ +8.
struct CMB_ListNode_8018C5FC {
    CMB_ListNode_8018C5FC* next;    // +0x00
    u32 field_04;                   // +0x04
    CMB_CfObjEnt_8018C5FC* data;    // +0x08
};

// Battle manager getInstance() result: only +0x08 (a node ptr) is used here.
struct CMB_Bm_8018C5FC {
    u8 pad_00[0x8];
    CMB_ListNode_8018C5FC* listHead; // +0x08
};

// Party list returned by getListB28__Fv: head node ptr at +0x04.
struct CMB_PartyList_8018C5FC {
    u8 pad_00[0x4];
    CMB_ListNode_8018C5FC* headNode; // +0x04
};

// Canonical cf object: vtable with a callable at +0x290.
struct CMB_CfObj_8018C5FC;
struct CMB_CfObjVtable_8018C5FC {
    u8 pad_00[0x290];
    s32 (*func_290)(CMB_CfObj_8018C5FC*); // +0x290
};
struct CMB_CfObj_8018C5FC {
    CMB_CfObjVtable_8018C5FC* vtable;    // +0x00
};

// The object referenced at node->data; the low-priority object lives at
// +0x3e9c within it (so “obj = data - 0x3e9c” is expressed as a member).
struct CMB_CfObjEnt_8018C5FC {
    u8 pad_00[0x3e9c];
    CMB_CfObj_8018C5FC obj;    // +0x3e9c
};

extern "C" {
    // C-ABI retail symbol (functions.hpp declares the C++-mangled bool f(int)
    // which would emit func_8009CF8C__Fi; the cf-namespace extern "C" decl
    // below shadows it so the unmangled func_8009CF8C reloc is emitted.
    u32 func_8009CF8C(u32);
    CMB_PartyList_8018C5FC* getListB28__Fv();
    s32 func_8026178C(s32, u32);
    void func_802A293C(s32, s32);
    extern "C" u16 playActorSound__Q22cf10CfSoundManFUlUlUlUlf(u32, u32, u32, u32, f32);
}

UnkClass_8018C5FC::UnkClass_8018C5FC() : gaugeValue(0) {
    tickAccum = lbl_eu_80667A30;
}

// Timer-based counter decrement: decreases the party gauge over time.
// When the timer elapses and there are no active battles, the counter
// decrements by 1 (if a qualifying party member is present) or by 2.
void func_8018C610(UnkClass_8018C5FC* _this) {
    u32 cf8cVal = func_8009CF8C(0x3357);
    u32 cntlz = (u32)__cntlzw(cf8cVal);
    if ((cntlz >> 5) != 0) return;
    if (cf::CfGameManager::isSceneLoading()) return;
    if (lbl_eu_80663E24 & 0xafa40000) return;

    f32 scale = func_80496288(&lbl_eu_80663E14);
    if ((_this->tickAccum += CDeviceVI::getSecPerFrame() * scale) >= lbl_eu_80667A34) {
        _this->tickAccum = lbl_eu_80667A30;

        // Count active battles by walking the battle manager's actor list.
        s32 adjust = 0;
        CMB_Bm_8018C5FC* bm = (CMB_Bm_8018C5FC*)getInstance__Q22cf14CBattleManagerFv();
        CMB_ListNode_8018C5FC* head = bm->listHead;
        s32 battleCount = 0;
        for (CMB_ListNode_8018C5FC* node = head->next;
             node != head; node = node->next)
        {
            battleCount++;
        }
        if (battleCount == 0) {
            // No active battle: scan the party member list for a qualifying
            // character (virtual call via vtable+0x290 on the low-priority
            // subobject) to decide whether to drop the gauge by 1 or 2. The
            // loop reloads list->headNode at its bottom like retail.
            CMB_PartyList_8018C5FC* list = getListB28__Fv();
            s32 found = 0;
            for (CMB_ListNode_8018C5FC* node = list->headNode->next;
                 node != list->headNode; node = node->next)
            {
                s32 ret = 0;
                if (node->data != 0) {
                    ret = node->data->obj.vtable->func_290(&node->data->obj);
                }

                ret = ret != 0
                          ? func_8026178C(node->data->obj.vtable->func_290(&node->data->obj),
                                          0x69)
                          : 0;

                if (ret == 0) {
                    continue;
                }
                found = 1;
                break;
            }
            adjust = -2;
            if (found != 0) {
                adjust = -1;
            }
        }

        if (adjust != 0) {
            // newVal is computed before the re-check guard (retail loads
            // gaugeValue+adjust into a reg ahead of the call); oldVal is reloaded
            // only after the guard passes.
            s32 newVal = _this->gaugeValue + adjust;
            u32 cf8cVal2 = func_8009CF8C(0x3357);
            u32 cntlz2 = (u32)__cntlzw(cf8cVal2);
            if ((cntlz2 >> 5) != 0) return;

            s32 oldVal = _this->gaugeValue;
            _this->gaugeValue = newVal;

            if (newVal < 0) {
                _this->gaugeValue = 0;
            } else if (newVal > 0x12c) {
                _this->gaugeValue = 0x12c;
            }

            func_802A293C(_this->gaugeValue, oldVal);

            s32 clamped = _this->gaugeValue;
            if ((oldVal < 0x64 && clamped >= 0x64) ||
                (oldVal < 0xc8 && clamped >= 0xc8))
            {
                playActorSound__Q22cf10CfSoundManFUlUlUlUlf(0, 0x64, 0, 0, lbl_eu_80667A38);
            }
        }
    }
}

// Add a delta to the party gauge counter, clamp to [0, 300], and return the new value.
s32 func_8018C820(UnkClass_8018C5FC* _this, s32 delta) {
    s32 newVal = _this->gaugeValue + delta;

    u32 cf8cVal = func_8009CF8C(0x3357);
    u32 cntlz = (u32)__cntlzw(cf8cVal);
    if ((cntlz >> 5) != 0) return _this->gaugeValue;

    s32 oldVal = _this->gaugeValue;
    _this->gaugeValue = newVal;

    if (newVal < 0) {
        _this->gaugeValue = 0;
    } else if (newVal > 0x12c) {
        _this->gaugeValue = 0x12c;
    }

    func_802A293C(_this->gaugeValue, oldVal);

    s32 clamped = _this->gaugeValue;
    if ((oldVal < 0x64 && clamped >= 0x64) ||
        (oldVal < 0xc8 && clamped >= 0xc8))
    {
        playActorSound__Q22cf10CfSoundManFUlUlUlUlf(0, 0x64, 0, 0, lbl_eu_80667A38);
    }

    return _this->gaugeValue;
}

// Set the party gauge counter to a value, clamp to [0, 300], and return the new value.
s32 func_8018C8F4(UnkClass_8018C5FC* _this, s32 val) {
    u32 cf8cVal = func_8009CF8C(0x3357);
    u32 cntlz = (u32)__cntlzw(cf8cVal);
    if ((cntlz >> 5) != 0) return _this->gaugeValue;

    s32 oldVal = _this->gaugeValue;
    _this->gaugeValue = val;

    if (val < 0) {
        _this->gaugeValue = 0;
    } else if (val > 0x12c) {
        _this->gaugeValue = 0x12c;
    }

    func_802A293C(_this->gaugeValue, oldVal);

    s32 clamped = _this->gaugeValue;
    if ((oldVal < 0x64 && clamped >= 0x64) ||
        (oldVal < 0xc8 && clamped >= 0xc8))
    {
        playActorSound__Q22cf10CfSoundManFUlUlUlUlf(0, 0x64, 0, 0, lbl_eu_80667A38);
    }

    return _this->gaugeValue;
}

} // namespace cf