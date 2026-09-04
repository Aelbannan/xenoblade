#pragma once

#include "kyoshin/cf/object/CActorParam.hpp"
// Include CActParamData.hpp BEFORE the rename macros so its typed
// declarations keep the real names; only CActParamAnim.hpp's legacy
// void*-first declarations get renamed (this TU never calls any of them).
#include "kyoshin/action/CActParamData.hpp"
#define func_80053F7C func_80053F7C_animsig
#define func_80053B24 func_80053B24_animsig
#define func_80055B88 func_80055B88_animsig
#include "kyoshin/action/CActParamAnim.hpp"
#undef func_80053F7C
#undef func_80053B24
#undef func_80055B88
#include "monolib/math/CVec3.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
#include "monolib/math/FloatUtils.hpp"  // H3 label-owner decl (lbl_eu_8066A208)

namespace cf {

// Region/link object referenced by the +0x4E8 / +0x4F4 slots; only the
// +0x714 active byte is consumed by this TU (func_8005DC30).
struct CActParamAnimGameLink {
    u8 _00[0x64];
    u32 f64;                       // +0x64 (func_8005EEB4 flag)
    u8 _68[0x60C - 0x68];
    u8 f60C[0x6BE - 0x60C];        // +0x60C func_804B1164 sub-object
    u16 f6BE;                      // +0x6BE (func_8005EEB4 save/restore)
    u8 _6C0[0x714 - 0x6C0];
    u8 active;                     // +0x714
};

// Sub-object reachable via +0x3A0 then +0x7EC; the +0x7EC slot points to a
// CActParam7ECTarget (see kyoshin/action/CActParamAnim.hpp) notified through
// +0x08 (int gate, func_8005D2C4), +0x14 (position) and +0x18 (flag).
struct CActParamAnimGameObj3A0 {
    u8 _00[0x7CC];
    ml::CVec3 pos7CC;  // +0x7CC airborne drift accumulator
    u8 _7D8[0x7EC - 0x7D8];
    void* sub7EC;   // +0x7EC
};

// Retail hierarchy: cf::CActParamAnimGame : CActParamAnim (base ctor/dtor are
// __ct__13CActParamAnimFv / __dt__13CActParamAnimFv). The retail vtable
// (lbl_eu_80526458, .data split1) is stored manually; __declspec(novtable)
// suppresses MWCC's own __vt__ so the ctor/dtor reloc name stays
// byte-identical. Object size 0x53C.
class __declspec(novtable) CActParamAnimGame : public ::CActParamAnim {
public:
    CActParamAnimGame();
    virtual ~CActParamAnimGame();
    void func_8005A524();
    void func_8005D2C4();
    bool isActionReady(u32 type);
    bool checkHeightThreshold();
    bool checkHeightBelow();
    bool clearFlag80000();
    bool func_8005D76C(u32 type, u32 state);
    bool func_8005D84C(u32 type, u32 state);
    bool tryGroundStep(u32 type, u32 state);
    bool func_8005DA44(u32 type);
    bool func_8005DB1C(u32 type);
    bool func_8005DC30(u32 type);
    void func_8005DCA8();
    int func_8005EEB4(ml::CVec3* pos, ml::CVec3* move);
    void func_80060110();
    void syncYawToLink();
    void checkNotFalling();
    void checkFlag40000();

    // base ::CActParamAnim vptr occupies 0x00-0x03; retail base payload pads
    // to 0x4E8 before this class's own fields.
    u8 _04[0x0C - 0x04];                  // 0x04-0x0B base-class payload
    volatile u32 field_0C;                // +0x0C flags (volatile: retail reloads it in func_8005D2C4's fall branch)
    u8 _10[0x270 - 0x10];                 // 0x10-0x26F base-class payload
    u32 field_270;                        // +0x270 base-class flags (func_8005A5B0)
    u8 _274[0x374 - 0x274];               // 0x274-0x373 base-class payload
    int field_374;                        // +0x374 (signed: retail compares it with cmpwi)
    u8 _378[0x390 - 0x378];               // 0x378-0x38F base-class payload
    f32 field_390;                        // +0x390 speed scale (func_8005A5B0)
    f32 field_394;                        // +0x394
    f32 field_398;                        // +0x398
    u8 _39C[0x3A0 - 0x39C];               // 0x39C-0x39F base-class payload
    void* field_3A0;                      // +0x3A0
    u8 _3A4[0x3A8 - 0x3A4];               // 0x3A4-0x3A7 base-class payload
    ml::CVec3 field_3A8;                  // +0x3A8 position
    ml::CVec3 field_3B4;                  // +0x3B4 ground-probe ref (func_8005A5B0)
    ml::CVec3 field_3C0;                  // +0x3C0 velocity (func_8005A5B0)
    ml::CVec3 field_3CC;                  // +0x3CC facing dir (func_8005A5B0)
    ml::CVec3 field_3D8;                  // +0x3D8 hit pos (func_8005A5B0)
    ml::CVec3 field_3E4;                  // +0x3E4 target pos (func_8005A5B0)
    ml::CVec3 field_3F0;                  // +0x3F0 move vec (func_8005A5B0)
    f32 field_3FC;                        // +0x3FC (func_8005EEB4 normal x)
    f32 field_400;                        // +0x400 (func_8005EEB4 normal y)
    f32 field_404;                        // +0x404 (func_8005EEB4 normal z)
    u8 _408[0x444 - 0x408];               // 0x408-0x443 base-class payload
    f32 field_444;                        // +0x444
    u8 _448[0x464 - 0x448];               // 0x448-0x463 base-class payload
    f32 field_464;                        // +0x464 (func_8005EEB4 height)
    u8 _468[0x47C - 0x468];               // 0x468-0x47B base-class payload
    f32 field_47C;                        // +0x47C
    u8 _480[0x484 - 0x480];               // 0x480-0x483 base-class payload
    f32 field_484;                        // +0x484 (func_8005EEB4 probe arg)
    f32 field_488;                        // +0x488
    u8 _48C[0x490 - 0x48C];               // 0x48C-0x48F base-class payload
    f32 field_490;                        // +0x490 (func_8005EEB4 probe arg)
    f32 field_494;                        // +0x494 (func_8005EEB4 probe arg)
    u8 _498[0x4A8 - 0x498];               // 0x498-0x4A7 base-class payload
    u32 field_4A8;                        // +0x4A8 filter
    s32 field_4AC;                        // +0x4AC
    u32 field_4B0;                        // +0x4B0 (func_8005A5B0)
    u8 _4B4[0x4E8 - 0x4B4];               // 0x4B4-0x4E7 base-class payload
    CActParamAnimGameLink* field_4E8;     // +0x4E8 region/link pointer
    u32 field_4EC;                         // +0x4EC flags
    u32 field_4F0;                         // +0x4F0
    CActParamAnimGameLink* field_4F4;      // +0x4F4
    f32 field_4F8;                         // +0x4F8
    f32 field_4FC;                         // +0x4FC
    f32 field_500;                         // +0x500
    f32 field_504;                         // +0x504
    f32 field_508;                         // +0x508
    f32 field_50C;                         // +0x50C
    f32 field_510;                         // +0x510 (func_8005EEB4 y-replace)
    f32 field_514;                         // +0x514
    f32 field_518;                         // +0x518
    f32 field_51C;                         // +0x51C
    ml::CVec3 field_520;                  // +0x520 (func_8005A5B0 step vec)
    int field_52C;                         // +0x52C
    volatile u16 field_530;                // +0x530 (volatile: retail reloads it across the flag-check branch in func_8005D84C)
    u16 field_532;                         // +0x532
    u16 field_534;                         // +0x534
    u16 field_536;                         // +0x536
    u16 field_538;                         // +0x538
};

// Absolute-offset view of the whole actor object. The ::CActParamAnim base
// compiles larger than the retail layout, so direct class-member access emits
// displacements shifted +0x36C vs retail; the state-machine routines below go
// through this view so every load/store uses the retail byte offset.
struct CActParamAnimGameView {
    u8 _00[0x0C];
    volatile u32 flags0C;            // 0x0C
    u8 _10[0x270 - 0x10];            // 0x10 embedded CActParamData (child data)
    u32 flags270;                    // 0x270
    u8 _274[0x374 - 0x274];
    s32 state374;                    // 0x374 (signed: cmpwi)
    u8 _378[0x380 - 0x378];
    f32 f380;                        // 0x380
    u8 _384[0x390 - 0x384];
    f32 f390;                        // 0x390
    f32 f394;                        // 0x394
    f32 f398;                        // 0x398
    u8 _39C[0x3A0 - 0x39C];
    void* obj3A0;                    // 0x3A0
    u8 _3A4[0x3A8 - 0x3A4];
    ml::CVec3 pos3A8;                // 0x3A8 position
    ml::CVec3 ground3B4;             // 0x3B4 ground-probe ref
    ml::CVec3 vel3C0;                // 0x3C0 velocity
    ml::CVec3 face3CC;               // 0x3CC facing dir
    ml::CVec3 vec3D4;                // 0x3D4 accel dir
    ml::CVec3 hit3D8;                // 0x3D8 hit pos
    ml::CVec3 tgt3E4;                // 0x3E4 target pos
    ml::CVec3 move3F0;               // 0x3F0 move vec
    u8 _3FC[0x444 - 0x3FC];
    f32 f444;                        // 0x444 facing fidx
    f32 f448;                        // 0x448 normalized speed
    u8 _44C[0x47C - 0x44C];
    f32 f47C;                        // 0x47C
    u8 _480[0x488 - 0x480];
    f32 f488;                        // 0x488 ground height accum
    u8 _48C[0x4A8 - 0x48C];
    u32 filter4A8;                   // 0x4A8 collision filter
    s32 field4AC;                    // 0x4AC
    u32 id4B0;                       // 0x4B0 probe result tag
    u8 _4B4[0x4E8 - 0x4B4];
    CActParamAnimGameLink* link4E8;  // 0x4E8 region/link pointer
    u32 flags4EC;                    // 0x4EC
    u32 flags4F0;                    // 0x4F0 snapshot
    u8 _4F4[0x4F8 - 0x4F4];
    f32 f4F8;                        // 0x4F8 air timer
    f32 f4FC;                        // 0x4FC ground height
    f32 f500;                        // 0x500
    f32 f504;                        // 0x504
    f32 f508;                        // 0x508 fall timer
    f32 f50C;                        // 0x50C
    u8 _510[0x514 - 0x510];
    f32 f514;                        // 0x514 saved ground point x
    f32 f518;                        // 0x518 saved ground point y
    f32 f51C;                        // 0x51C saved ground point z
    ml::CVec3 probe520;              // 0x520 probe step vec
    s32 state52C;                    // 0x52C move state
    volatile u16 flags530;           // 0x530
};

} // namespace cf

// Opaque object stored at the actor's +0x8 slot; its +0x4 word holds gate
// flag bits consumed by isActionReady.
struct CActParamAnimGameOwner {
    u32 _00;
    u32 flags04;
};

// Game-manager list container returned by getGimmickListHead; the +0x4 word is
// the list head/sentinel walked by func_8005DB1C.
struct CActParamAnimGameMgr {
    u8 _00[4];
    u32 head;  // +0x4
};

// Node of the manager list: +0x0 next pointer, +0x8 payload id fed to
// func_8016FE34.
struct CActParamAnimGameListNode {
    u32 next;      // +0x0
    u8 _04[4];
    void* f08;     // +0x8
};

// Retail data symbols (global scope: variable names are not mangled).
// const-qualified so MWCC treats the sdata2 loads as constants and schedules
// them at retail's hoisted position (see MWCC_CASES SDA-const hoist).
extern const float lbl_eu_80666040;   // sdata2: 0.0f (field_4F8 initializer)
extern const float lbl_eu_80666044;   // sdata2: 1.0f (field_500 / field_508 initializer)
extern const float lbl_eu_80666048;   // sdata2: field_50C / field_504 initializer
extern const float lbl_eu_8066604C;   // sdata2: field_47C initializer (func_8005A5B0)
extern const float lbl_eu_80666050;   // sdata2: ground-probe threshold (func_8005E60C)
extern const float lbl_eu_80666054;   // sdata2: func_804BE470 probe arg (func_8005A5B0)
extern const float lbl_eu_80666058;   // sdata2: height-diff threshold (func_8005A5B0)
extern const float lbl_eu_8066605C;   // sdata2: move-vec scale (func_8005A5B0)
extern const float lbl_eu_80666060;   // sdata2: step vec / probe arg (func_8005A5B0)
extern const float lbl_eu_80666064;   // sdata2: step vec (func_8005A5B0)
extern const float lbl_eu_80666068;   // sdata2: sin/cos offset scale (func_8005D2C4)
extern const float lbl_eu_8066606C;   // sdata2: loop bound (func_8005A5B0)
extern const float lbl_eu_80666070;   // sdata2: loop bound (func_8005A5B0)
extern const float lbl_eu_80666074;   // sdata2: loop bound (func_8005A5B0)
extern const float lbl_eu_80666078;   // sdata2: loop bound (func_8005A5B0)
extern const float lbl_eu_8066607C;   // sdata2: loop bound (func_8005A5B0)
extern const float lbl_eu_80666084;   // sdata2: probe scale (func_8005A5B0)
extern const float lbl_eu_80666088;   // sdata2: len threshold (func_8005A5B0)
extern const float lbl_eu_8066608C;   // sdata2: normalize scale (func_8005A5B0)
extern const float lbl_eu_80666090;   // sdata2: pos.y threshold (func_8005A5B0)
extern const float lbl_eu_80666094;   // sdata2: -1.0f (func_8005A5B0)
extern const float lbl_eu_80666098;   // sdata2: dot scale (func_8005A5B0)
extern const float lbl_eu_8066609C;   // sdata2: move-vec scale (func_8005A5B0)
extern const float lbl_eu_806660A0;   // sdata2: len threshold (func_8005A5B0)
extern const float lbl_eu_80666080;   // sdata2: ground-probe threshold (func_8005E7C4)
extern const float lbl_eu_806660B0;   // sdata2: fall scale x (func_8005B820)
extern const float lbl_eu_806660B4;   // sdata2: fall scale z (func_8005B820)
extern const float lbl_eu_806660BC;   // sdata2: pos.y offset / probe arg (func_8005E60C)
extern const float lbl_eu_806660B8;   // sdata2: fall-penalty scale (func_8005B820)
extern const float lbl_eu_806660C0;   // sdata2: fall-penalty cap (func_8005B820)
extern const float lbl_eu_806660CC;   // sdata2: sin/cos fidx multiplier (40.743663f)
extern const float lbl_eu_806619FC;   // sdata2: normal-refine scale (func_8005E28C)
extern const float lbl_eu_806619F8;   // sdata2: step-contact radius (func_8005DE68)
extern const float lbl_eu_80666120;   // sdata2: landing radius squared (func_8005E28C)
extern const float lbl_eu_806660D4;   // sdata2: lateral offset (func_8005E7C4)
extern const float lbl_eu_806660D8;   // sdata2: probe arg (func_8005E7C4)
extern const float lbl_eu_806660E0;   // sdata2: angle scale (func_80060110)
extern const float lbl_eu_806660C4;   // sdata2: f3D8.y bound (func_8005EEB4)
extern const float lbl_eu_806660C8;   // sdata2: height bound (func_8005EEB4)
extern const float lbl_eu_80666104;   // sdata2: dir scale (func_8005DCA8)
extern const float lbl_eu_80666100;   // sdata2: commit scale (func_8005E990)
extern const float lbl_eu_80666128;   // sdata2: 3C0.y bound (func_8005EEB4)
extern const float lbl_eu_8066612C;   // sdata2: B840 scale (func_8005EEB4)
extern const float lbl_eu_80666130;   // sdata2: probe arg (func_8005EEB4)
extern const float lbl_eu_80666134;   // sdata2: scan bound (func_8005EEB4)
extern const float lbl_eu_80666138;   // sdata2: scan bound (func_8005EEB4)
extern const float lbl_eu_8066613C;   // sdata2: ground scale (func_8005EEB4)
extern const float lbl_eu_80666140;   // sdata2: ground scale (func_8005EEB4)
extern const float lbl_eu_80666144;   // sdata2: ground scale (func_8005EEB4)
extern const float lbl_eu_80666148;   // sdata2: height bound (func_8005EEB4)
extern const float lbl_eu_8066614C;   // sdata2: len bound (func_8005EEB4)
extern const float lbl_eu_80666150;   // sdata2: fixed len (func_8005EEB4)
extern const float lbl_eu_80666154;   // sdata2: height bound (func_8005EEB4)
extern const float lbl_eu_80666158;   // sdata2: height bound (func_8005EEB4)
extern const float lbl_eu_8066615C;   // sdata2: probe offset (func_8005EEB4)
extern const float lbl_eu_80666160;   // sdata2: height bound (func_8005EEB4)
extern u32 lbl_eu_80665958;   // sdata: object pointer passed to func_804B5658 (func_8005EEB4)
extern const float lbl_eu_8066610C;   // sdata2: ground-probe constant (func_8005D2C4)
extern const float lbl_eu_80666110;   // sdata2: drop threshold (func_8005D2C4)
extern const float lbl_eu_80666114;   // sdata2: ground-probe constant (func_8005D2C4)
extern const float lbl_eu_80666118;   // sdata2: ground-probe constant (func_8005D2C4)
extern const float lbl_eu_8066611C;   // sdata2: matrix row init (func_8005DCA8)
extern const float lbl_eu_80666124;   // sdata2: probe arg (func_8005E60C)
extern f32 lbl_eu_8066AF20;   // sdata2: probe arg (func_8005E60C); f32 form to match CtrlMoveBase.hpp
extern u8 lbl_eu_80526458[];  // retail CActParamAnimGame vtable (.data split1)

// C++-mangled retail helper (actor id -> action source), used by
// func_80060110. Global-scope C++ declaration mangles to findObjectById__Fi.
void* findObjectById(int id);

// C-linkage imports (retail names, defined in kyoshin/action/CActParamAnim.cpp).
extern "C" int func_80051AD0(CActParamAnim* self);
extern "C" int func_80051B38(CActParamAnim* self);
extern "C" void func_8004CEF8(void* self, u32 param);

// C-linkage imports from kyoshin/action/CActParamData.cpp: float getters on
// the +0x10 child-data region, and a global state query (func_8005D2C4).
extern "C" float func_80055DD4(const void* data);
extern "C" float func_80055DB8(const void* data);
extern "C" int func_804BCC10(void);

// C-linkage imports from libs/monolib/src/scn/code_804BC9EC.cpp (sound
// system global state / query) and kyoshin/CUIBattleManager.cpp (link
// resolution), used by func_8005D2C4.
extern "C" void* getScnHandle__Fv(void);
extern "C" void* func_8016FE34(void* r3);

// C-linkage import: the retail base-class ctor, called directly by the
// __ct__8005A3FC free function (a mangled ctor symbol cannot be invoked
// from C++ except via placement new, which MWCC null-guards).
extern "C" void __ct__13CActParamAnimFv(void* self);
