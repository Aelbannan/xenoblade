#pragma once

#include <types.h>

#include "kyoshin/cf/CfGimmick.hpp"

class UnkClass_8003AA34;

// ---------------------------------------------------------------------------
// C-linkage imports (unmangled retail symbols defined in other TUs).
// ---------------------------------------------------------------------------

// Gimmick work-area registration helpers (register three column blocks with
// the bdat manager returned by Bdat_GetTable_AA34).
extern "C" void CfGimmick_LoadBdatAreaPos(cf::CfGimmick* self, float* out, void* unused, void* holder);
extern "C" void CfGimmick_LoadBdatAreaExtents(cf::CfGimmick* self, cf::CfGimmick* out, void* unused, void* holder);
extern "C" void CfGimmick_LoadBdatAreaRotation(cf::CfGimmick* self, f32* out, void* bdat, void* table);

// State-machine side effects shared with CfGimmickObject.
extern "C" void CfGimmick_SetGlobalFlag200000();
extern "C" void CfGimmick_SetGlobalFlag8();

// Poke the object registered at result+0x84 (save-point activation).
extern "C" void CtrlMoveEne_MarkActiveSteady(u8* target);

// ---------------------------------------------------------------------------
// External data (retail linker symbols; global-scope variables keep their
// plain names without `extern "C"`).
// ---------------------------------------------------------------------------

// gGimmickBdatColumns is declared in CfGimmick.hpp (const void*[]).
// Bdat file holder seed handed to the registration helpers (.sdata pointer).

namespace cf {

// Save-point gimmick.  Base fields used: field_04 (area position), field_10
// (area rotation), field_1C (collider view), field_60 (collider kind / checker
// index), field_64 (bdat row), field_6C/field_6E (player-count gate bounds),
// field_82 (gimmick type id = 8).
class CfGimmickSaveOff : public CfGimmick {
public:
    CfGimmickSaveOff(s32 row);
    virtual ~CfGimmickSaveOff();
    virtual void CfGimmick_Update();

    /* 0x84 */ u32 mType;       // completion behaviour selector (byte-extended)
};

// State-machine tick: optional range gate on the game-manager counter, then
// dispatch through the state jump table, then act on mType.

} // namespace cf
