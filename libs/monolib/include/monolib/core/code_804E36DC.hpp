#pragma once

#include <types.h>

// Forward declarations needed for the C-linkage import signatures
struct CSchedule;
struct CScheduleItem;
struct CEffectObj;
struct ScheduleEntry;
struct CItemEntry;

struct UnkStruct_804E36DC {
    u16 field_0x0;
    u16 pad0x2;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xC;
    u8 pad0x10[0x80];
    float field_0x90;
    float field_0x94;
    u8 pad0x98[0x44];
    u32 field_0xDC[4];
};

// C-linkage imports
// ---------------------------------------------------------------------------

// Scene/view helpers (other TUs)
extern "C" f32 func_80496288(void* view);
extern "C" void* func_80496264(void* view, s32 idx);

// Effect system helpers (monolib/src/core/code_804DEDA8, effect TUs)
extern "C" void func_804DCA88(void* mtx, const void* params);
extern "C" void* func_804DFBF4(s16 handle);   // item pool lookup
extern "C" void func_804DFE20(s16 handle);    // release object handle
extern "C" CEffectObj* func_804DFEAC(s16 handle); // object pool lookup
extern "C" s16 func_804DFCC4(ScheduleEntry* entries, CItemEntry* entry,
                             CSchedule* sched, s32 slotVal, u32 index);
extern "C" void* func_804CB5FC(CEffectObj* obj);
extern "C" void func_804DF808(CSchedule* sched, void* arg, CEffectObj* obj, f32 delta);
extern "C" void func_804C8D90(CEffectObj* obj, f32 delta);
extern "C" void func_804C9D24(CEffectObj* obj);
extern "C" void func_804C9D30(CEffectObj* obj);
extern "C" void func_804CAA94(CEffectObj* obj);

// ScheduleList callbacks / lookup
extern "C" void func_804E536C(void* arg);
extern "C" void* func_804DF2F0(void* table, void* key);

extern "C" void __dl__FPv(void* ptr);
extern "C" f64 lbl_eu_8066B2F0;
extern "C" f64 lbl_eu_8066B2E8;

// Functions defined in this TU (forward declarations)
extern "C" void func_804E39E8(CSchedule* self, f32 delta);
extern "C" void func_804E41B4(CScheduleItem* item);
