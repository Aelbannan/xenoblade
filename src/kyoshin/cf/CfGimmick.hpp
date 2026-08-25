#pragma once

#include <types.h>
#include "kyoshin/plugin/ocBdat.hpp"

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
struct CfGimmickObject {
    void** vtable;              // 0x00
    u8 gap04[0x90 - 0x04];      // 0x04..0x8F
    u8 field_90;                // 0x90
};

// Sparse virtual interface over CfGimmickObject's vtable (slots +0x9C =
// setPos, +0x158 = activate).  MWCC vtable slots are shifted +2 for the
// RTTI/dtor headers, so a method declared at index N dispatches at (N+2)*4:
// declared 37 -> +0x9C, declared 84 -> +0x158 (same scheme as CfGimmickMgr88).
class CfGimmickSpawnIf {
public:
    virtual ~CfGimmickSpawnIf();                    // declared 0 -> +0x08
    virtual void d01();  virtual void d02();  virtual void d03();  virtual void d04();
    virtual void d05();  virtual void d06();  virtual void d07();  virtual void d08();
    virtual void d09();  virtual void d10();  virtual void d11();  virtual void d12();
    virtual void d13();  virtual void d14();  virtual void d15();  virtual void d16();
    virtual void d17();  virtual void d18();  virtual void d19();  virtual void d20();
    virtual void d21();  virtual void d22();  virtual void d23();  virtual void d24();
    virtual void d25();  virtual void d26();  virtual void d27();  virtual void d28();
    virtual void d29();  virtual void d30();  virtual void d31();  virtual void d32();
    virtual void d33();  virtual void d34();  virtual void d35();  virtual void d36();
    virtual void setPos(const CfGimmickVec3* pos);  // declared 37 -> +0x9C
    virtual void d38();  virtual void d39();  virtual void d40();  virtual void d41();
    virtual void d42();  virtual void d43();  virtual void d44();  virtual void d45();
    virtual void d46();  virtual void d47();  virtual void d48();  virtual void d49();
    virtual void d50();  virtual void d51();  virtual void d52();  virtual void d53();
    virtual void d54();  virtual void d55();  virtual void d56();  virtual void d57();
    virtual void d58();  virtual void d59();  virtual void d60();  virtual void d61();
    virtual void d62();  virtual void d63();  virtual void d64();  virtual void d65();
    virtual void d66();  virtual void d67();  virtual void d68();  virtual void d69();
    virtual void d70();  virtual void d71();  virtual void d72();  virtual void d73();
    virtual void d74();  virtual void d75();  virtual void d76();  virtual void d77();
    virtual void d78();  virtual void d79();  virtual void d80();  virtual void d81();
    virtual void d82();  virtual void d83();
    virtual void activate(int flag);                // declared 84 -> +0x158
};

// Vtable interface over the getPlayer(0) object used by func_80208CC0.
// Mirrors the CfObject vtable layout, typing the four used slots: +0x74
// state check (bool), +0xAC target-map position, +0xCC heading angle (f32),
// +0x160 extra state check (int), plus the raw s32 at +0xC4.  MWCC slots
// are shifted +2 (RTTI headers), so declared N dispatches at (N+2)*4:
// 27 -> +0x74, 41 -> +0xAC, 49 -> +0xCC, 86 -> +0x160.
class CfGimmickPlayerFace {
public:
    virtual ~CfGimmickPlayerFace();                 // declared 0 -> +0x08
    virtual void d01();  virtual void d02();  virtual void d03();  virtual void d04();
    virtual void d05();  virtual void d06();  virtual void d07();  virtual void d08();
    virtual void d09();  virtual void d10();  virtual void d11();  virtual void d12();
    virtual void d13();  virtual void d14();  virtual void d15();  virtual void d16();
    virtual void d17();  virtual void d18();  virtual void d19();  virtual void d20();
    virtual void d21();  virtual void d22();  virtual void d23();  virtual void d24();
    virtual void d25();  virtual void d26();
    virtual bool d27();                             // declared 27 -> +0x74
    virtual void d28();  virtual void d29();  virtual void d30();  virtual void d31();
    virtual void d32();  virtual void d33();  virtual void d34();  virtual void d35();
    virtual void d36();  virtual void d37();  virtual void d38();  virtual void d39();
    virtual void d40();
    virtual CfGimmickVec3* d41();                   // declared 41 -> +0xAC
    virtual void d42();  virtual void d43();  virtual void d44();  virtual void d45();
    virtual void d46();  virtual void d47();  virtual void d48();
    virtual float d49();                            // declared 49 -> +0xCC
    virtual void d50();  virtual void d51();  virtual void d52();  virtual void d53();
    virtual void d54();  virtual void d55();  virtual void d56();  virtual void d57();
    virtual void d58();  virtual void d59();  virtual void d60();  virtual void d61();
    virtual void d62();  virtual void d63();  virtual void d64();  virtual void d65();
    virtual void d66();  virtual void d67();  virtual void d68();  virtual void d69();
    virtual void d70();  virtual void d71();  virtual void d72();  virtual void d73();
    virtual void d74();  virtual void d75();  virtual void d76();  virtual void d77();
    virtual void d78();  virtual void d79();  virtual void d80();  virtual void d81();
    virtual void d82();  virtual void d83();  virtual void d84();  virtual void d85();
    virtual int d86();                              // declared 86 -> +0x160

    u8 pad04[0xC4 - 0x04];          // 0x04..0xC3
    s32 field_C4;                   // 0xC4
};

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
// Player base object scanned by func_8020A294.  The HP read is a genuine
// C++ virtual dispatch (vtable slot +0x128); MWCC shifts declared slots +2
// for the RTTI/dtor headers, so getHP sits at declared 72.
class CfPlayerBase {
public:
    virtual ~CfPlayerBase();                    // declared 0
    virtual void d01();  virtual void d02();  virtual void d03();  virtual void d04();
    virtual void d05();  virtual void d06();  virtual void d07();  virtual void d08();
    virtual void d09();  virtual void d10();  virtual void d11();  virtual void d12();
    virtual void d13();  virtual void d14();  virtual void d15();  virtual void d16();
    virtual void d17();  virtual void d18();  virtual void d19();  virtual void d20();
    virtual void d21();  virtual void d22();  virtual void d23();  virtual void d24();
    virtual void d25();  virtual void d26();  virtual void d27();  virtual void d28();
    virtual void d29();  virtual void d30();  virtual void d31();  virtual void d32();
    virtual void d33();  virtual void d34();  virtual void d35();  virtual void d36();
    virtual void d37();  virtual void d38();  virtual void d39();  virtual void d40();
    virtual void d41();  virtual void d42();  virtual void d43();  virtual void d44();
    virtual void d45();  virtual void d46();  virtual void d47();  virtual void d48();
    virtual void d49();  virtual void d50();  virtual void d51();  virtual void d52();
    virtual void d53();  virtual void d54();  virtual void d55();  virtual void d56();
    virtual void d57();  virtual void d58();  virtual void d59();  virtual void d60();
    virtual void d61();  virtual void d62();  virtual void d63();  virtual void d64();
    virtual void d65();  virtual void d66();  virtual void d67();  virtual void d68();
    virtual void d69();  virtual void d70();  virtual void d71();
    virtual float getHP();                      // declared 72 -> +0x128

    u8 pad[0x3E9C - 0x04];
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

// Cast-only interface for the CfGimmick::field_78 game-manager object:
// func_8020899C tail-dispatches its vtable slot +0x88.  MWCC vtable slots
// are shifted +2 (RTTI headers), so a method at declared slot N dispatches
// at (N+2)*4; declared 32 -> vtable +0x88 (same scheme as CfGimmickLockObj).
class CfGimmickMgr88 {
public:
    virtual ~CfGimmickMgr88();      // declared 0 -> 0x08
    virtual void d01();  virtual void d02();  virtual void d03();  virtual void d04();
    virtual void d05();  virtual void d06();  virtual void d07();  virtual void d08();
    virtual void d09();  virtual void d10();  virtual void d11();  virtual void d12();
    virtual void d13();  virtual void d14();  virtual void d15();  virtual void d16();
    virtual void d17();  virtual void d18();  virtual void d19();  virtual void d20();
    virtual void d21();  virtual void d22();  virtual void d23();  virtual void d24();
    virtual void d25();  virtual void d26();  virtual void d27();  virtual void d28();
    virtual void d29();  virtual void d30();  virtual void d31();
    virtual void m88();             // declared 32 -> vtable +0x88
};

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// Relocated from CfGimmick.cpp; do NOT "fix" the mangled names.
// ---------------------------------------------------------------------------

// Cross-TU callees (resolved via the retail symbol map).
extern "C" void func_800B3A88(UnkClass_805764CC* self, void* target);
extern "C" void func_801BFED0(int a, u16 b, int c);
extern "C" int func_8009CF8C(void*);

// Small-data globals accessed by CfGimmick-region helpers (@sda21 loads/stores).
extern "C" u32 lbl_eu_806646B4;
extern "C" u32 lbl_eu_806646BC;
extern "C" float lbl_eu_80662784;
extern "C" u16 lbl_eu_806646C0;
extern "C" int lbl_eu_805765B0[10];
// CfGimmick vtable (stored at +0x00 by the constructor).
extern "C" u8 lbl_eu_80535844[];

// Reference point the gimmick range checks are measured from.
extern "C" CfGimmickVec3 lbl_eu_805765A0;

// Downward drop offset recorded by func_80208CC0 when the stage ground is
// above zero (.sdata).
extern "C" f32 lbl_eu_80662780;
// 1.0f fill constant used when no player is present (.sdata).
extern "C" f32 lbl_eu_80668360;

// Shared singleton accessor; refs resolve to the unmangled retail name.
extern "C" CfGimmickGlobal* getUnk80664658();

// Bdat table data used by func_8020A608 / func_80208F34 columns.
extern "C" u8 lbl_eu_805357E8[];
extern "C" void* lbl_eu_80664148;   // .sbss - current bdat file pointer
// Returned when func_8020A608 cannot fetch a column row.
extern "C" void* lbl_eu_80662788;

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
extern "C" void func_801BFDE8(unsigned int mode, unsigned int value,
                               unsigned int playerValue, float first, float second);
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
extern "C" CfGimmickObject* func_800B20B4(UnkClass_805764CC* mgr, int a, int b, int c);

// Message-posting helpers (CUICfManager.cpp / CUIWindowManager.cpp).
extern "C" u32 func_8013C54C();
extern "C" void func_8013D55C(char* msg, int a, int b);
// Shared message format buffer (retail .data, absolute-address access).
extern "C" char lbl_eu_805765D8[0x20];