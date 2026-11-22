#pragma once

/**
 * @file CfGimmickItem.hpp
 * @brief cf::CfGimmickItem -- item gimmick entity class.
 *
 * CfGimmickItem extends CfGimmick to represent items that can be
 * picked up / collected in the game world (recovery items, key items,
 * etc.). Fields are populated from BDAT columns at construction.
 *
 * Base fields used: field_04 (placement point), field_10 (rotation),
 * field_1C (collider view), field_60 (checker index), field_7C (spawned
 * object registration), field_82 (type id = 6).
 */

#include "kyoshin/cf/CfGimmick.hpp"
#include "types.h"

namespace cf {

class CfGimmickItem : public CfGimmick {
public:
    CfGimmickItem(int rowId);
    virtual ~CfGimmickItem();
    virtual void CfGimmick_Update();   // PTMF state dispatch + spawn/cleanup

    /* 0x84 */ u16 field_84[3];          // per-slot item ids
    /* 0x8A */ u16 field_8A;
    /* 0x8C */ u16 field_8C;
    /* 0x8E */ u16 field_8E;             // effect id triggered while working (CfGimmick_PlaySoundAtPos)
    /* 0x90 */ u16 field_90;             // effect id (CfGimmick_PlaySoundAtPos, same as field_8E)
    /* 0x92 */ u16 field_92;
    /* 0x94 */ u16 field_94;             // resource id passed to getResourceFromTable
    /* 0x96 */ u8 field_96;              // lower bound of the respawn-count window
    /* 0x97 */ u8 field_97;              // upper bound of the respawn-count window
    /* 0x98 */ u8 field_98;
    /* 0x99 */ u8 field_99;
    /* 0x9A */ u8 field_9A;
    /* 0x9B */ u8 field_9B;
    /* 0x9C */ u8 field_9C;              // mode byte (2 or 3 => state 5)
    /* 0x9E */ u16 field_9E;             // current state (PTMF table index)
    /* 0xA0 */ u16 field_A0;             // countdown timer vs field_98
};

// Area manager object (createBattleActor result); +0xB0 back-pointer to the
// owning gimmick (same shape as CfGimmickObjectMgr in CfGimmickObject.hpp).
struct CfGimmickItemMgr {
    u32* vtable;                // +0x00
    u8 gap_B0[0xB0 - 0x04];
    CfGimmickItem* field_B0;    // +0xB0: owning gimmick
};

// Pointer-to-member state table indexed by field_9E (12-byte entries).
typedef void (CfGimmickItem::*CfGimmickItemState)();

} // namespace cf

// ---------------------------------------------------------------------------
// C-linkage imports (unmangled retail symbols used by this TU).  These are
// CfGimmick-family helpers / item engine calls defined in other units; they
// have C linkage in retail so extern "C" is required to emit the raw name.
// ---------------------------------------------------------------------------
extern "C" {
void CfGimmick_ClearManagerBinding(cf::CfGimmick* self);
void CfGimmick_UnregisterSpawnedObject(CfGimmickReg* self);                     // unregister field_7C object
void CfGimmick_PostGimmickName(unsigned short id);
unsigned int CfGimmick_IsMessageSystemBusy(void);
void CfGimmick_SetGlobalFlagC0042(void);
void CfGimmick_UpdateProximity(CfGimmickReg* reg, const ml::CVec3* point,
                   unsigned short c, float d, int e, int g);
void CItem_consumeFamilyCnt(unsigned int a, int b);
void CItem_thunkAllocRecord(unsigned int a, unsigned int b);

int CfGimmick_CheckTriggerGated(u32 mask, cf::CfGimmick* gimmick, const ml::CVec3* point,
                  const f32* ang, u32 partyId);
void CfGimmick_PlaySoundAtPos(u32 id, const ml::CVec3* pos);
u32 getQueuedFileEventCount__Q22cf13CfGameManagerFv(void);
u32 getResourceFromTable__Q22cf13CfGameManagerFv(u32 a);
unsigned int CItem_findRecByFamily(unsigned short id);
void* Bdat_GetTable_AA34(void);
extern "C" void CfGimmick_LoadBdatAreaPos(cf::CfGimmick* self, float* out, void* unused, void* holder);
extern "C" void CfGimmick_LoadBdatAreaExtents(cf::CfGimmick* self, cf::CfGimmick* out, void* unused, void* holder);
extern "C" void CfGimmick_LoadBdatAreaRotation(cf::CfGimmick* self, f32* out, void* bdat, void* table);
void CamEvtProgramEventCam(int idx);
int CfGimmick_CheckStateFlag2CC8(u8* obj);
}

// Area manager object factory (void* form matches CTaskGameEff.hpp).
extern "C" void* createBattleActor__Q22cf13CfGameManagerFv(u32 id, u32 mode);

// Data symbols (global-scope objects, not mangled by MWCC).
extern f32 lbl_eu_8066844C;            // spawn distance constant (sdata2)
// NOTE: retail's 6-entry PTMF state table. Declared as untyped bytes (the
// dispatch site casts to CfGimmickItemState*): a real PMF array cannot be
// constructed from free functions, so the table is a byte blob with the
// (0, -1, func) immediates spelled out. The cast is codegen-neutral.
extern char lbl_eu_80535A50[];         // 6-entry PTMF state table bytes
extern char* lbl_eu_806627B8[2];       // { "A_Item", "A_Lost" } name ptrs (sdata)
// (lbl_eu_805087AC retired: the column names are plain literals pooled by
// MWCC; no synthetic reference remains.)
extern f32 lbl_eu_80668448;            // LOD float constant (sdata2)