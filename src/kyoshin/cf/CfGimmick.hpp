#pragma once

#include "kyoshin/CSystemWindowApi.hpp"
#include <types.h>
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/object/IObjectInfo.hpp"

// Forward declarations for the C-linkage imports below.
class UnkClass_805764CC;
namespace cf { class CfObjectMove; }

// Minimal 3-component vector used by the CfGimmick collision helpers.
struct CfGimmickVec3 {
    f32 x;  // 0x00
    f32 y;  // 0x04
    f32 z;  // 0x08
};

// Container whose first member (0x00) is a registered-object pointer used by
// CfGimmick_UnregisterSpawnedObject to unregister from the global resource manager.
struct CfGimmickObject;  // defined below
struct CfGimmickReg {
    CfGimmickObject* field_00;
};

namespace cf {

class CfGameManager;

class CfGimmick : public IObjectInfo {
public:
    CfGimmick();
    virtual ~CfGimmick();                                        // +0x08
    virtual void CfGimmick_DetachManager(CfGameManager* other);  // +0x0C
    virtual void CfGimmick_Update();                             // +0x10 (per-frame update; base no-op)
    virtual void GimNoopB();                                     // +0x14
    virtual void GimNoopC();                                     // +0x18
    virtual void GimNoopD();                                     // +0x1C
    virtual void CfGimmick_UpdateColliderMatrix();               // +0x20

    // vptr at 0x00 (compiler-managed)
    /* 0x04 */ CfGimmickVec3 field_04;  // bdat area position (CfGimmick_LoadBdatAreaPos)
    /* 0x10 */ CfGimmickVec3 field_10;  // bdat area rotation (CfGimmick_LoadBdatAreaRotation)
    // 0x1C: collider sub-object (CfGimmick-typed view: matrix at +0x00,
    // extents at +0x30.., placement kind at +0x44 = absolute 0x60).
    /* 0x1C */ u8 field_1C[0x30 - 0x1C];
    /* 0x30 */ f32 field_30;   // 0x30 - horizontal radius / half-extent
    /* 0x34 */ f32 field_34;   // 0x34 - vertical extent (low)
    /* 0x38 */ f32 field_38;   // 0x38 - vertical extent (high)
    /* 0x3C */ f32 field_3C;   // 0x3C - depth half-extent
    /* 0x40 */ f32 field_40;   // 0x40 - interaction distance
    /* 0x44 */ s32 field_44;   // 0x44 - rotation/placement kind (0..4); switch index for jumptable_eu_80535830
    // 0x48..0x5F: tail of the collider view (its extents at +0x30.. = absolute 0x4C..)
    /* 0x48 */ f32 field_48;
    /* 0x4C */ f32 field_4C;   // collider extent (+0x30)
    /* 0x50 */ f32 field_50;   // collider extent (+0x34)
    /* 0x54 */ f32 field_54;   // collider extent (+0x38)
    /* 0x58 */ f32 field_58;   // collider extent (+0x3C)
    /* 0x5C */ f32 field_5C;   // collider interaction distance (+0x40)
    /* 0x60 */ u32 field_60;   // collider placement kind (field_44 of the +0x1C view)
    /* 0x64 */ u16 field_64;
    /* 0x66 */ u16 field_66;
    /* 0x68 */ u16 field_68;
    /* 0x6A */ u16 field_6A;
    /* 0x6C */ u16 field_6C;
    /* 0x6E */ u16 field_6E;
    /* 0x70 */ u8 field_70;
    /* 0x71 */ u8 field_71;
    /* 0x72 */ u8 field_72;
    /* 0x73 */ u8 field_73;
    /* 0x74 */ u32 field_74;
    /* 0x78 */ CfGameManager* field_78;
    /* 0x7C */ CfGimmickReg field_7C;  // spawned gimmick-object registration slot
    /* 0x80 */ u16 field_80;
    /* 0x82 */ u16 field_82;   // gimmick type id (set by the derived ctors)

    int CfGimmick_CheckerKind0Always();
};

} // namespace cf

#include "kyoshin/code_801F3BE0.hpp"  // CGimmickGlobal, getUnk80664658 (owner)
#include "kyoshin/cf/CfSoundMan.hpp"


// u32-view of the 12-byte position block: CfGimmick_UpdatePartyAnchorState copies the player
// target vector with integer lwz/stw moves (no float conversion).
struct CfGimmickVec3u {
    u32 x;  // 0x00
    u32 y;  // 0x04
    u32 z;  // 0x08
};


// Object spawned by CfGimmick_UpdateProximity via spawnMaskedObject.  The vtable is used to
// invoke slots 0x158 (activate) and 0x9C (set position); field_90 is the
// spawned/active flag.
struct CfGimmickObject : public cf::CfObject {
    // CfObject base at 0x00 (vtable + fields to 0x70); tail holds gimmick flag
    u8 gap70[0x90 - 0x70];      // 0x70..0x8F
    u8 field_90;                // 0x90
    int CfObject_UnkVirtualFunc2() override { return 1; }
    void CfObject_UnkVirtualFunc4() override {}
    void CfObject_UnkVirtualFunc7() override {}
    void CfObject_notifyEventDone() override {}
    void CfObject_setMoveTargetVec(const ml::CVec3* vec) override { (void)vec; }
    // Slot 0x158 (base setPointEnabled): records the spawned/active flag.
    void setPointEnabled(int flag) override { field_90 = (u8)flag; }
    void setPos(const CfGimmickVec3* pos) { CfObject_setMoveTargetVec((const ml::CVec3*)pos); }
    // Retail CfGimmick_SpawnNamedObject/8020A6B0 reach slot 0x158 through a virtual
    // dispatch (lwz 0x158 + bctrl); spelling that here as setPointEnabled()
    // grows this TU by +0x14 per spawner and blows the +0x24 split budget,
    // so activate keeps the baseline direct store (same bytes as before)
    // while the real override above fills the vtable slot. The missing
    // virtual dispatch in the two spawners is a recorded non-pad residual.
    void activate(int flag) { field_90 = (u8)flag; }
};

// Player object helpers are provided by cf::CfObject / cf::CfObjectMove
// (vtable slots +0x74, +0xAC, +0xCC, +0x160 and raw field +0xC4).

// Circular object list returned by getReslistB48 / getReslistB68.  The head
// node stored at +0x04 is the sentinel; real nodes are reached from head->next
// and terminate when they wrap back to head.
struct CfGimmickListNode {
    CfGimmickListNode* next;    // 0x00
    u8 gap04[0x08 - 0x04];      // 0x04..0x07
    cf::CfObject* object;       // 0x08
};
struct CfGimmickList {
    u32* field_00;              // 0x00
    CfGimmickListNode* head;    // 0x04
};

// Player sub-object (base at getPlayer(i)-0x3E9C) used for the per-player
// eligibility checks in CfGimmick_QueryTrigger.
struct CfPlayerSub3F60 {
    u8 pad[0x4EC];
    u32 field_4EC;              // 0x4EC
};
struct CfPlayerSpot {
    void** vtable;              // 0x00 (object at player+0x3E9C; slot 0xAC yields the target)
};
// Player base object scanned by CfGimmick_CheckFightListPlayerAlive.  HP is CActorParam::CActorParam_getHp at +0x128.
struct CfPlayerBase {
    u8 pad[0x3E9C];
    CfPlayerSpot spot;          // 0x3E9C - object handed to the jumptable checkers
    u8 pad2[0x3F60 - 0x3EA0];
    void* subField3F60;         // 0x3F60
};

// View of the player base object scanned by CfGimmick_CheckFightListPlayerAlive: the u16 id at
// +0x456C is compared ((id >> 4) == playerId) against the caller's id.
struct CfPlayerIdView {
    u8 pad[0x456C];
    u16 id456C;   // 0x456C
};

// CfGimmick::field_78 dispatch at +0x88 is driven through the CfObject view
// (CfObject_pushRefreshValue slot, float arity) at the CfGimmick_PushRefreshValue
// definition in CfGimmick.cpp.

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// Relocated from CfGimmick.cpp; do NOT "fix" the mangled names.
// ---------------------------------------------------------------------------

// Cross-TU callees (resolved via the retail symbol map).
class UnkClass_805764CC;
extern "C" void removeManagedObject(UnkClass_805764CC* self, void* target);
#include "kyoshin/cf/CtrlRemoteApi.hpp"

// Small-data globals accessed by CfGimmick-region helpers (@sda21 loads/stores).
extern "C" u32 gGimmickPartyId;
extern "C" u32 gGimmickPartyFlags;
// Cached value written by CfGimmick_SetGlobalB8Value (retail: single stw + blr).
extern "C" u32 gGimmickSelectedRow;
extern "C" float gGimmickMoveRateScale;
extern "C" u16 gGimmickInitFlags;
extern "C" int gGimmickPartySlotState[10];

// Reference point the gimmick range checks are measured from.
extern "C" CfGimmickVec3 gGimmickAnchorPoint;

// Downward drop offset recorded by CfGimmick_UpdatePartyAnchorState when the stage ground is
// above zero (.sdata).
extern "C" f32 lbl_eu_80662780;


// Bdat table data used by CfGimmick_LookupBdatGimmickName / CfGimmick_LoadBdatAreaPos columns.
extern "C" const void* gGimmickBdatColumns[];
extern "C" void* gGimmickBdatFile;   // .sbss - current bdat file pointer
// Returned when CfGimmick_LookupBdatGimmickName cannot fetch a column row.
extern "C" const void* gGimmickFallbackName[2];

// Column-capacity helpers (CBdat row begin/count).
extern "C" void* Bdat_GetTable_AA34();
extern "C" u32 Bdat_GetRowBase_B41C(void* bdat);
extern "C" u32 Bdat_GetMaxRow_B1EC(void* bdat);

// Player per-heal helpers paired with CfObject_syncModelRate (+0x168).
extern "C" void CfObjectMove_setMoveSpeedGated(cf::CfObjectMove* player, float value);
extern "C" void CfObjectMove_setMoveSpeed(cf::CfObjectMove* player, float value);

// CfSoundMan_PlayLinkedParam(u32 mode, u32 value, u32 playerValue, float first, float second)
// u32 spellings (not unsigned int): must match CfObjectImplMove.hpp/UnityHelpers exactly
extern "C" void CfGimmick_TriggerSound2CC8(u32 id, u32 value);

// Checker functions dispatched by gimmick->field_44 (jumptable_eu_80535830).
typedef int (*CfGimmickChecker)(cf::CfGimmick*, const CfGimmickVec3*, const CfGimmickVec3*);
extern "C" CfGimmickChecker jumptable_eu_80535830[];
extern "C" CfGimmickList* getReslistB48();
extern "C" CfGimmickList* getReslistB68();
// Fixed rotation angle used by CfGimmick_QueryTrigger's mask-0x4 occlusion test.
extern "C" f32 gGimmickFixedRotationAngle;

extern "C" void ObjPoint_SetName_13FC(void* obj, const char* name, int arg);
// Create/attach a gimmick object (C-ABI, unmangled): manager first, then flags.
extern "C" CfGimmickObject* spawnMaskedObject(void* mgr, int a, int b, int c);

// Message-posting helpers (CUICfManager.cpp / CUIWindowManager.cpp).
extern "C" u32 UIWin_GetInstance();
// Shared message format buffer (retail .data, absolute-address access).
extern "C" char gGimmickMessageBuffer[128];
