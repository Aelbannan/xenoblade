#pragma once

#include <types.h>
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/cf/object/CfObject.hpp"

// Forward declarations for the C-linkage imports below.
class UnkClass_805764CC;
namespace cf { class CfObjectMove; }

namespace cf {

class CfGameManager;

class CfGimmick {
public:
    ~CfGimmick();

    /* 0x00 */ void* vtable;   // set to lbl_eu_80535844 by __ct__cf_CfGimmick
    /* 0x04 */ u8 gap04[0x30 - 0x04];
    /* 0x30 */ f32 field_30;   // 0x30 - horizontal radius / half-extent
    /* 0x34 */ f32 field_34;   // 0x34 - vertical extent (low)
    /* 0x38 */ f32 field_38;   // 0x38 - vertical extent (high)
    /* 0x3C */ f32 field_3C;   // 0x3C - depth half-extent
    /* 0x40 */ f32 field_40;   // 0x40 - interaction distance
    /* 0x44 */ s32 field_44;   // 0x44 - rotation/placement kind (0..4); switch index for jumptable_eu_80535830
    /* 0x48 */ u8 gap48[0x64 - 0x48];
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
    /* 0x7C */ u32 field_7C;
    /* 0x80 */ u16 field_80;

    void func_8020896C(void* other);
    void func_80208988();
    int func_8020A8AC();
};

} // namespace cf

// Global settings object returned by getUnk80664658 (field_214 flag word).
struct CfGimmickGlobal {
    u8 pad[0x210];
    u32 field_210;   // 0x210 - value set by func_8020A068
    u32 field_214;   // 0x214 - flag bits set by several setters
};

// Minimal 3-component vector used by the CfGimmick collision helpers.
struct CfGimmickVec3 {
    f32 x;  // 0x00
    f32 y;  // 0x04
    f32 z;  // 0x08
};

// u32-view of the 12-byte position block: func_80208CC0 copies the player
// target vector with integer lwz/stw moves (no float conversion).
struct CfGimmickVec3u {
    u32 x;  // 0x00
    u32 y;  // 0x04
    u32 z;  // 0x08
};

// Container whose first member (0x00) is a registered-object pointer used by
// func_8020A434 to unregister from the global resource manager.
struct CfGimmickReg {
    void* field_00;
};

// Object spawned by func_8020A6B0 via func_800B20B4.  The vtable is used to
// invoke slots 0x158 (activate) and 0x9C (set position); field_90 is the
// spawned/active flag.
struct CfGimmickObject : public cf::CfObject {
    // CfObject base at 0x00 (vtable + fields to 0x70); tail holds gimmick flag
    u8 gap70[0x90 - 0x70];      // 0x70..0x8F
    u8 field_90;                // 0x90
    void CfObject_UnkVirtualFunc2() override {}
    void CfObject_UnkVirtualFunc4() override {}
    void CfObject_UnkVirtualFunc7() override {}
    void CfObject_UnkVirtualFunc8() override {}
    void CfObject_UnkVirtualFunc19(const ml::CVec3* vec) override { (void)vec; }
    void CfObject_UnkVirtualFunc66(int flag) override { field_90 = (u8)flag; }
    void setPos(const CfGimmickVec3* pos) { CfObject_UnkVirtualFunc19((const ml::CVec3*)pos); }
    void activate(int flag) { CfObject_UnkVirtualFunc66(flag); }
};

// Player object helpers are provided by cf::CfObject / cf::CfObjectMove
// (vtable slots +0x74, +0xAC, +0xCC, +0x160 and raw field +0xC4).

// Circular object list returned by func_800B6BC8 / func_800B6BEC.  The head
// node stored at +0x04 is the sentinel; real nodes are reached from head->next
// and terminate when they wrap back to head.
struct CfGimmickListNode {
    CfGimmickListNode* next;    // 0x00
    u8 gap04[0x08 - 0x04];      // 0x04..0x07
    void* object;               // 0x08
};
struct CfGimmickList {
    void* field_00;             // 0x00
    CfGimmickListNode* head;    // 0x04
};

// Player sub-object (base at getPlayer(i)-0x3E9C) used for the per-player
// eligibility checks in func_802098EC.
struct CfPlayerSub3F60 {
    u8 pad[0x4EC];
    u32 field_4EC;              // 0x4EC
};
struct CfPlayerSpot {
    void** vtable;              // 0x00 (object at player+0x3E9C; slot 0xAC yields the target)
};
// Player base object scanned by func_8020A294.  HP is CActorParam::CActorParam_UnkVirtualFunc37 at +0x128.
struct CfPlayerBase {
    u8 pad[0x3E9C];
    CfPlayerSpot spot;          // 0x3E9C - object handed to the jumptable checkers
    u8 pad2[0x3F60 - 0x3EA0];
    void* subField3F60;         // 0x3F60
};

// View of the player base object scanned by func_8020A294: the u16 id at
// +0x456C is compared ((id >> 4) == playerId) against the caller's id.
struct CfPlayerIdView {
    u8 pad[0x456C];
    u16 id456C;   // 0x456C
};

// CfGimmick::field_78 dispatch at +0x88 is recovered on cf::CfGameManager
// (see CfGimmick.cpp local CfGameManager definition).

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// Relocated from CfGimmick.cpp; do NOT "fix" the mangled names.
// ---------------------------------------------------------------------------

// Cross-TU callees (resolved via the retail symbol map).
extern "C" void func_800B3A88(void* self, void* target);
extern "C" void func_801BFED0(int a, u16 b, int c);
extern "C" u32 func_8009CF8C(u32 resourceId);

// Small-data globals accessed by CfGimmick-region helpers (@sda21 loads/stores).
extern "C" u32 lbl_eu_806646B4;
extern "C" u32 lbl_eu_806646BC;
// Cached value written by func_80208EDC (retail: single stw + blr).
extern "C" u32 lbl_eu_806646B8;
extern "C" float lbl_eu_80662784;
extern "C" u16 lbl_eu_806646C0;
extern "C" int lbl_eu_805765B0[10];
// CfGimmick vtable (stored at +0x00 by the constructor).
extern "C" u8 lbl_eu_80535844[];

// Reference point the gimmick range checks are measured from.
extern "C" CfGimmickVec3 lbl_eu_805765A0;
extern "C" char _bss_pad_805765A0[4];

// Downward drop offset recorded by func_80208CC0 when the stage ground is
// above zero (.sdata).
extern "C" f32 lbl_eu_80662780;
// 1.0f fill constant used when no player is present (.sdata).
extern "C" f32 lbl_eu_80668360;

// Shared singleton accessor; refs resolve to the unmangled retail name.
extern "C" CfGimmickGlobal* getUnk80664658();

// Bdat table data used by func_8020A608 / func_80208F34 columns.
extern "C" const void* lbl_eu_805357E8[];
extern "C" void* lbl_eu_80664148;   // .sbss - current bdat file pointer
// Returned when func_8020A608 cannot fetch a column row.
extern "C" char lbl_eu_80662788[8];

// Column-capacity helpers (CBdat row begin/count).
extern "C" void* func_8003AA34();
extern "C" u32 func_8003B41C(void* bdat);
extern "C" u32 func_8003B1EC(void* bdat);

// Player per-heal helpers paired with CfObject_UnkVirtualFunc70.
extern "C" void func_800BC3B0(cf::CfObjectMove* player, float value);
extern "C" void func_800BC3D8(cf::CfObjectMove* player, float value);

// Scale factor for bdat int -> float position conversion.
extern "C" float lbl_eu_80668364;
// Alternative scale factor used by the +0x1C-family column readers.  Const
// declaration matches CfObjectModel.hpp (which also references it).
extern const float lbl_eu_8066A210;
// 2^52 magic constant subtracted in the u16->f32 double-trick conversions
// (lfd + fsubs per bdat-column reader block).
extern "C" double lbl_eu_80668370;
// Base of the +0, +0xA, +0x15, +0x20 column-name string block.
extern "C" char lbl_eu_80508634[];
// Sentinel used by the player loops (func_8020A124 / func_8020A1DC) and the
// gimmick extent checks.  Const so MWCC treats the SDA load as read-only and
// schedules it at retail's position (CfObjectMove.hpp pattern).
extern "C" const float lbl_eu_80668350;

// func_801BFDE8(u32 mode, u32 value, u32 playerValue, float first, float second)
// u32 spellings (not unsigned int): must match CfObjectImplMove.hpp/UnityHelpers exactly
extern "C" void func_801BFDE8(u32 mode, u32 value,
                               u32 playerValue, float first, float second);
// Sound constants loaded in func_80208C48 / func_80208C60 (retail .sda21 loads)
extern "C" float lbl_eu_80668358;
extern "C" float lbl_eu_8066835C;
extern "C" unsigned int func_80124B78();
// FIdx scale applied to the rotation point before MTX34RotXYZFIdx.
extern "C" const f32 lbl_eu_80668354;

// Checker functions dispatched by gimmick->field_44 (jumptable_eu_80535830).
typedef int (*CfGimmickChecker)(cf::CfGimmick*, void*, const CfGimmickVec3*);
extern "C" CfGimmickChecker jumptable_eu_80535830[];
extern "C" CfGimmickList* func_800B6BC8();
extern "C" CfGimmickList* func_800B6BEC();
// Fixed rotation angle used by func_802098EC's mask-0x4 occlusion test.
extern "C" f32 lbl_eu_806646B0;

// Height offset added to a spawned object's Y position.  Const so MWCC
// hoists the SDA load to retail's position (CfObjectMove.hpp pattern).
extern "C" const f32 lbl_eu_80668378;
extern "C" void func_800C13FC(void* obj, const char* name, int arg);
// Create/attach a gimmick object (C-ABI, unmangled): manager first, then flags.
extern "C" CfGimmickObject* func_800B20B4(void* mgr, int a, int b, int c);

// Message-posting helpers (CUICfManager.cpp / CUIWindowManager.cpp).
extern "C" u32 func_8013C54C();
extern "C" void func_8013D55C(char* msg, int a, int b);
// Shared message format buffer (retail .data, absolute-address access).
extern "C" char lbl_eu_805765D8[128];