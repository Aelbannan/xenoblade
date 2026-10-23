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
extern "C" f32 Scn_GetFrameDelta(void* view);
extern "C" void* Scn_FindCamItem(void* view, s32 idx);

// Effect system helpers (monolib/src/core/code_804DEDA8, effect TUs)
extern "C" void CERotEulerXYZ(void* mtx, const void* params);
extern "C" void* SchedItem_FetchByHandle(s16 handle);   // item pool lookup
extern "C" void SchedFx_ReleaseHandle(s16 handle);    // release object handle
extern "C" CEffectObj* SchedFx_FetchByHandle(s16 handle); // object pool lookup
extern "C" s16 SchedFx_ClaimConstruct(ScheduleEntry* entries, CItemEntry* entry,
                             CSchedule* sched, s32 slotVal, u32 index);
extern "C" void* EffObj_GetPriority(CEffectObj* obj);
extern "C" void SchedElem_ClaimNode(CSchedule* sched, void* arg, CEffectObj* obj, f32 delta);
extern "C" void EffObj_Update(CEffectObj* obj, f32 delta);
extern "C" void EffSys_AdvanceChain(CEffectObj* obj);
extern "C" void EffFrame_Update(CEffectObj* obj);
extern "C" void EffObj_StepNode(CEffectObj* obj);

// ScheduleList callbacks / lookup
extern "C" void SchedList_DropBoth(void* arg);
extern "C" void* SchedRes_FindValueByKey(void* table, void* key);

extern "C" f64 lbl_eu_8066B2F0;
extern "C" f64 lbl_eu_8066B2E8;

// Functions defined in this TU (forward declarations)
// CSchedule::StepEntries is a member now; declared in monolib/core/CSchedule.hpp.
// (schedReleaseItemObjects is now CScheduleItem::ReleaseObjects; stale decl retired.)
