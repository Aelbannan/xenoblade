#pragma once

#include <types.h>

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" u32 func_8009D1F8(u32* buffer, s32 index);
extern "C" u32 func_80061FE8();

// Player-object state/event probe (retail unmangled C-ABI name; arg1 is the
// actor object + 0x8 state region, arg2 a probe id).
// NOTE: declared with void* to match CfObjectImplMove.hpp (both headers are
// included together in some TUs; extern "C" funcs cannot be overloaded).
extern "C" int func_80148778(void* obj, int id);

// Global flag-bit probe on the game-manager state word. Retail symbol is the
// C++-mangled func_8006EF04__Fi, so this is a plain global C++ declaration
// (CfCam.cpp defines it under C linkage with the same linker name).
bool func_8006EF04(int mask);

namespace cf {
class CtrlRemote;
class CtrlPc;
}

// Full retail layout view of cf::CtrlRemote (the CtrlPc.hpp declaration only
// exposes the implicit vptr; __ct__CtrlRemote initializes the fields below).
// The ctor stores the retail vtable manually, so this view carries the
// vptr slot for the manual store but relies on the caller's novtable class.
struct CtrlRemoteLayout {
    void* vtbl;                  // 0x00
    u8 _pad_04[0x1DC - 0x04];    // 0x04-0x1DB (CtrlPc-compatible base region)
    f32 mFloats1DC[32];          // 0x1DC-0x25B
    u32 mField25C;               // 0x25C
    u32 mField260;               // 0x260
};

// Retail CtrlPc sub-ctor called by __ct__CtrlRemote (defined in CtrlPc.cpp).
extern "C" void __ct__cf_CtrlPc(cf::CtrlPc* obj, void* posObj, void* arg5);

// Retail CtrlRemote ctor (0x800995BC). Returns `this` in r3; the CtrlPc.hpp
// scaffold declares it `void` (its caller discards the result), so this TU
// declares the real signature here and hides the scaffold decl via #define.
cf::CtrlRemote* __ct__CtrlRemote(cf::CtrlRemote* obj, void* posObj, int arg5);

// Serialized control-data buffer parsed by func_8009CE14:
// [0x00 u32 kind][0x04 u32 payload size][0x08 payload bytes].
struct CtrlRemoteBuf {
    u32 field_00;   // 0x00
    u32 mSize;      // 0x04
    u8 mData[4];    // 0x08 (payload follows the 8-byte header)
};

// Typed-parse helpers dispatched by func_8009CE14. Retail bodies live in the
// retail binary (not decompiled in this TU) - declare only so the call sites
// emit real `bl` relocs instead of inlining. C linkage keeps the retail
// unmangled reloc names.
extern "C" u32 func_8009C9B8(cf::CtrlRemote* self, CtrlRemoteBuf* buf, u32 arg);
extern "C" u32 func_8009CAAC(cf::CtrlRemote* self, CtrlRemoteBuf* buf, u32 arg);
extern "C" u32 func_8009CB80(cf::CtrlRemote* self, CtrlRemoteBuf* buf, u32 arg);

// Heap alloc/free callbacks installed into the UnkClass_80460C34Ctx below;
// defined in this TU (C linkage so the address-of relocs match retail).
extern "C" void* func_8009C8F4(cf::CtrlRemote* self, u32 a, u32 b);
extern "C" void func_8009C980(cf::CtrlRemote* self, u8* ptr);

// Object stored in the shared control-data slots (buf + 0x1234 + 4*i);
// vtable slot 3 (offset 0x0C) receives (index, clamped value, extracted bits)
// when func_8009D2C8 writes a control-data field.
class CtrlDataSlotObj {
public:
    virtual void vf00();
    virtual void vf03(u32 index, u32 value, u32 bits);
};

// Retail bit-setter over the control-data buffer; defined in this TU.
void func_8009D2C8(u32* buffer, u32 index, u32 value);

// Retail bit-setter over the control-data buffer; defined in this TU.
void func_8009D2C8(u32* buffer, u32 index, u32 value);

// Control-index -> (word pointer, type, sub-index) mapper; defined in this TU.
extern "C" u32* func_8009D12C(u32* buffer, s32 index, s32* typeOut, s32* idxOut);

// External data (retail linker symbols)
extern "C" u32 lbl_eu_80571848[];
extern "C" u8 lbl_eu_80663E80;
extern "C" u32 lbl_eu_80661C70[2];
extern "C" u32 lbl_eu_80528048[4];
extern "C" u32 lbl_eu_80528058[32];

// Global flag word read by func_8009D5FC (retail: lwz r3, lbl_eu_80663E88@sda21).
extern u32 lbl_eu_80663E88;

// 0x38-byte stack context handed to the retail UnkClass_80460C34 methods by
// func_8009CAAC (fields at the offsets the retail prologue stores).
struct UnkClass_80460C34Ctx {
    u8* mPayload;    // 0x00 (buf + 8)
    u32 mArg;        // 0x04
    u8 _08[0x0C - 0x08];
    void* mSelf;     // 0x0C
    u32 mSize;       // 0x10
    u8 _14[0x20 - 0x14];
    void* mAlloc;    // 0x20 (func_8009C8F4)
    void* mFree;     // 0x24 (func_8009C980)
    u32 mZero;       // 0x28
    u8 _2C[0x38 - 0x2C];
};

// Retail UnkClass_80460C34 stream methods (retail binary only - declared so
// the call sites emit real `bl` relocs; C linkage keeps the retail names).
extern "C" int func_80460DCC__17UnkClass_80460C34Fv(void* self, const void* a, int b);
extern "C" int func_80460F58__17UnkClass_80460C34Fv(void* self, int a);
extern "C" int func_80461FE0__17UnkClass_80460C34Fv(void* self);

// Data pointer passed as the second arg of func_80460DCC (retail .data).
extern u8 lbl_eu_804FBC20[];

// Retail CtrlRemote vtable data (.data at 0x80527F88). Stored manually by
// __ct__CtrlRemote because the class is __declspec(novtable).
extern u8 lbl_eu_80527F88[];

// sdata2 float constant stored to the 32 floats at +0x1DC by __ct__CtrlRemote
// (const declaration so MWCC references the retail pool slot directly).
extern const f32 lbl_eu_80666730;

// ---------------------------------------------------------------------------
// Imports / view types used by the func_8009BD14 pad-handler sweep.
// ---------------------------------------------------------------------------

// Forward decls (full layouts live in CtrlPc.hpp, included after this header).
class CtrlPcSub37;
class CtrlPlayerObj;
struct CfMoveEnumList;

// C++-mangled retail helper func_800B708C__Fi (actor id -> action source).
void* func_800B708C(int id);

// Voice/battle-list sweep helpers (retail unmangled C-ABI names).
// Signature matches CfObjectImplMove.hpp (both headers appear together).
extern "C" void* func_800F6EAC(CfMoveEnumList* list, u32 idx);
extern "C" void* func_8016FE34(void* r3);
class Fd44State;
// Retail call site passes no argument (r3 left over from the previous call);
// matches the CtrlPc.hpp declaration.
extern "C" Fd44State* func_8017FD44();
extern "C" void* func_800451D8(u32 cls, void* param);

// Actor-state gate probe (retail unmangled C-ABI name; see CfCam.hpp).

// Voice-manager sweep helpers and the sudden-commu active check. Retail
// relocs are the unmangled C names, so these are extern "C" imports.
extern "C" void func_802A2BB0();
extern "C" void func_802A2CF0();
extern "C" void func_802A2B44();
extern "C" void func_802A2C1C();
extern "C" int func_801B0F8C();

// CtrlPc view exposing vf38 (vtable slot 0xA0) for func_8009BD14's menu-state
// word reads (CtrlPc.hpp stops at vf37 = 0x9C). The Nth declared virtual sits
// at (N+2)*4, so vf38 lands at 0xA0; layout is CtrlPc-compatible.
class CtrlPcVf38 {
public:
    virtual ~CtrlPcVf38();  // vtable slot 0x08 (CtrlPc's leading dtor slot)
    virtual void vf01();  virtual void vf02();  virtual void vf03();
    virtual void vf04();  virtual void vf05();  virtual void vf06();
    virtual void vf07();  virtual void vf08();  virtual void vf09();
    virtual void vf10();  virtual void vf11();  virtual void vf12();
    virtual void vf13();  virtual void vf14();  virtual void vf15();
    virtual void vf16();  virtual void vf17();  virtual void vf18();
    virtual void vf19();
    virtual void vf20();  virtual void vf21();  virtual void vf22();  // 0x60
    virtual void vf23();  virtual void vf24();
    virtual int vf25(float* out);  // 0x6C (writes a 3-float vector)
    virtual void vf26();  virtual void vf27();  virtual void vf28();
    virtual void vf29();  virtual void vf30();  virtual void vf31();
    virtual void vf32();  virtual void vf33();  virtual void vf34();
    virtual void vf35();  virtual void vf36();
    virtual CtrlPcSub37* vf37();  // 0x9C
    virtual CtrlPcSub37* vf38();  // 0xA0

    u32 mField4;                 // 0x04 (pad/battle flags)
    f32 mField8;                 // 0x08
    f32 mFieldC;                 // 0x0C (target angle/position state)
    f32 mField10;                // 0x10 (computed facing angle)
    f32 mField14;                // 0x14 (aim/fx state flag value)
    char _pad_18[0x5C - 0x18];   // 0x18-0x5B
    CtrlPlayerObj* mField5C;     // 0x5C
};

// func_800B708C result view for func_8009BD14: combo/target id at +0x74.
struct CtrlVoiceSweepView {
    u8 _00[0x74];
    u32 mField74;               // 0x74
};

// Result view of self->vf38() (vtable slot 0xA0) for func_8009A4AC: flag
// words at 0x00 / 0x0C / 0x10 (the CtrlPcSub37 view in CtrlPc.hpp does not
// expose the 0x0C word this sweep reads).
struct CtrlRemoteSubA0 {
    u32 mField0;                // 0x00
    u8 _04[0x0C - 0x04];
    u32 mFieldC;                // 0x0C
    u32 mField10;               // 0x10
};

// Extended vf37() (vtable slot 0x9C) result view for func_80098EF8: signed
// stick bytes at 0x58-0x5B and a state byte at 0xED (CtrlPcSub37 in
// CtrlPc.hpp stops at 0x18).
struct CtrlPcSub37Ext {
    u8 _00[0x58];
    s8 mField58;                // 0x58
    s8 mField59;                // 0x59
    s8 mField5A;                // 0x5A
    s8 mField5B;                // 0x5B
    u8 _5C[0xED - 0x5C];
    u8 mFieldED;                // 0xED
};

// Result of player->vf164() (vtable slot 0x298): actor id word at +0x04.
struct CtrlPlayerSub298Vf4 {
    u32 mField4;                // 0x04
};

// Sub-object pointed to by the voice-owner interface data field at +0xC4
// (func_80098EF8): flag words at 0x270/0x274 and a u16 at 0x530.
struct CtrlVoiceSubC4 {
    u8 _00[0x270];
    u32 mField270;              // 0x270
    u32 mField274;              // 0x274
    u8 _278[0x530 - 0x278];
    u16 mField530;              // 0x530
};

// Embedded voice-owner interface at player+0x3E9C: data pointer at +0xC4
// (the vptr at +0x00 is handled by CVoiceOwnerIntfPc in CtrlPc.hpp).
struct CtrlVoiceOwnerC4View {
    u8 _00[0xC4];
    CtrlVoiceSubC4* mFieldC4;   // 0xC4
};

// getUnk80664658 result view for func_80098EF8: flag word at +0x214.
struct CtrlUnk64658View {
    u8 _00[0x214];
    u32 mField214;              // 0x214
};

// Result of the game-manager sub-object's vf05() (vtable slot 0x1C): float
// at +0x04.
struct CtrlGm8Vf1CRes {
    u8 _00[0x04];
    f32 mField4;                // 0x04
};

// Game-manager sub-object view exposing vf05 at slot 0x1C with a result
// pointer (the UnkClass_800821F8View decl in CtrlPc.hpp declares it void).
class CtrlGm8Vf1C {
public:
    virtual void vf00();
    virtual void vf01();
    virtual void vf02();
    virtual void vf03();
    virtual void vf04();
    virtual CtrlGm8Vf1CRes* vf05();  // vtable slot 0x1C
    virtual void vf06();
    virtual void vf07();
    virtual void vf08();
    virtual void vf09(void* arg);    // 0x2C
    u32 mField4;                     // 0x04
};

// Player-actor view exposing vf141 (vtable slot 0x23C) as a float-returning
// virtual (the CtrlPlayerObj decl in CtrlPc.hpp names it as void padding).
class CtrlPlayerVf23C {
public:
    virtual void vf00();  virtual void vf01();  virtual void vf02();
    virtual void vf03();  virtual void vf04();  virtual void vf05();
    virtual void vf06();  virtual void vf07();  virtual void vf08();
    virtual void vf09();  virtual void vf10();  virtual void vf11();
    virtual void vf12();  virtual void vf13();  virtual void vf14();
    virtual void vf15();  virtual void vf16();  virtual void vf17();
    virtual void vf18();  virtual void vf19();  virtual void vf20();
    virtual void vf21();  virtual void vf22();  virtual void vf23();
    virtual void vf24();  virtual void vf25();  virtual void vf26();
    virtual void vf27();  virtual void vf28();  virtual void vf29();
    virtual void vf30();  virtual void vf31();  virtual void vf32();
    virtual void vf33();  virtual void vf34();  virtual void vf35();
    virtual void vf36();  virtual void vf37();  virtual void vf38();
    virtual void vf39();  virtual void vf40();  virtual void vf41();
    virtual void vf42();  virtual void vf43();  virtual void vf44();
    virtual void vf45();  virtual void vf46();  virtual void vf47();
    virtual void vf48();  virtual void vf49();  virtual void vf50();
    virtual void vf51();  virtual void vf52();  virtual void vf53();
    virtual void vf54();  virtual void vf55();  virtual void vf56();
    virtual void vf57();  virtual void vf58();  virtual void vf59();
    virtual void vf60();  virtual void vf61();  virtual void vf62();
    virtual void vf63();  virtual void vf64();  virtual void vf65();
    virtual void vf66();  virtual void vf67();  virtual void vf68();
    virtual void vf69();  virtual void vf70();  virtual void vf71();
    virtual void vf72();  virtual void vf73();  virtual void vf74();
    virtual void vf75();  virtual void vf76();  virtual void vf77();
    virtual void vf78();  virtual void vf79();  virtual void vf80();
    virtual void vf81();  virtual void vf82();  virtual void vf83();
    virtual void vf84();  virtual void vf85();  virtual void vf86();
    virtual void vf87();  virtual void vf88();  virtual void vf89();
    virtual void vf90();  virtual void vf91();  virtual void vf92();
    virtual void vf93();  virtual void vf94();  virtual void vf95();
    virtual void vf96();  virtual void vf97();  virtual void vf98();
    virtual void vf99();  virtual void vf100(); virtual void vf101();
    virtual void vf102(); virtual void vf103(); virtual void vf104();
    virtual void vf105(); virtual void vf106(); virtual void vf107();
    virtual void vf108(); virtual void vf109(); virtual void vf110();
    virtual void vf111(); virtual void vf112(); virtual void vf113();
    virtual void vf114(); virtual void vf115(); virtual void vf116();
    virtual void vf117(); virtual void vf118(); virtual void vf119();
    virtual void vf120(); virtual void vf121(); virtual void vf122();
    virtual void vf123(); virtual void vf124(); virtual void vf125();
    virtual void vf126(); virtual void vf127(); virtual void vf128();
    virtual void vf129(); virtual void vf130(); virtual void vf131();
    virtual void vf132(); virtual void vf133(); virtual void vf134();
    virtual void vf135(); virtual void vf136(); virtual void vf137();
    virtual void vf138(); virtual void vf139(); virtual void vf140();
    virtual float vf141();  // vtable slot 0x23C
};

// nw4r math helpers and battle-manager chain probe (retail C-ABI names).
extern "C" f32 Atan2FIdx__Q24nw4r4mathFff(f32 y, f32 x);
extern "C" f32 FrSqrt__Q24nw4r4mathFf(f32 value);
extern "C" void Warning__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
extern "C" CtrlUnk64658View* getUnk80664658();
extern "C" int func_802799F0(void* a, void* b);
extern "C" u32 func_8029EE58();

// Debug-assert strings pooled by the Warning call in func_80098EF8 (retail
// .data at 0x80526324 / 0x80526300).
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];

// sdata2 float constants used by func_80098EF8. const declarations so MWCC
// references the retail pool slots directly (see CtrlPc.hpp notes).
extern const f32 lbl_eu_80666734;
extern const f32 lbl_eu_80666738;
extern const f64 lbl_eu_80666740;
extern const f32 lbl_eu_8066674C;
extern const f32 lbl_eu_80666750;
extern const f32 lbl_eu_80666754;
extern const f32 lbl_eu_80666758;
extern const f32 lbl_eu_8066675C;
extern const f32 lbl_eu_80666760;
extern const f32 lbl_eu_80666764;
extern const f32 lbl_eu_80666768;
extern const f32 lbl_eu_8066676C;
extern const f32 lbl_eu_8066A1F8;   // pi
extern const f32 lbl_eu_8066A1FC;   // 2*pi
extern const f32 lbl_eu_8066A20C;
extern const f32 lbl_eu_80666748;

// ---------------------------------------------------------------------------
// Imports / views used by func_8009AE80.
// ---------------------------------------------------------------------------

// func_800FE68C object view covering both the actor-id word (+0x90E4) and the
// demo/idle state word (+0xC180) probed by func_8009AE80.
struct CfObjAe80 {
    u8 _00[0x90E4];
    u32 mField90E4;                  // 0x90E4
    u8 _90E8[0xC180 - 0x90E8];
    u32 mFieldC180;                  // 0xC180
};

// Selector-system helpers (retail C-ABI names; CfObjectSelectorObj.cpp).
extern "C" void func_800FE860(void* obj, u32 arg);
extern "C" unsigned long func_800FE910(void* obj);
extern "C" void func_800FE950(void* obj, u32 a, u32 b, u32 c);

// vf37() (vtable slot 0x9C) result view for func_8009AE80: menu-state flag
// words at 0x00 / 0x04 / 0x08 / 0x10 / 0x14.
struct CtrlPcSub37Ae80 {
    u32 mField0;                 // 0x00
    u32 mField4;                 // 0x04
    u32 mField8;                 // 0x08
    u8 _0C[0x10 - 0x0C];
    u32 mField10;                // 0x10
    u32 mField14;                // 0x14
};

// CtrlPc-compatible view exposing the +0x2C state word and vf37 (slot 0x9C)
// for func_8009AE80.
class CtrlPcVf37State {
public:
    virtual ~CtrlPcVf37State();  // vtable slot 0x00 (leading dtor slot)
    virtual void vf01();  virtual void vf02();  virtual void vf03();
    virtual void vf04();  virtual void vf05();  virtual void vf06();
    virtual void vf07();  virtual void vf08();  virtual void vf09();
    virtual void vf10();  virtual void vf11();  virtual void vf12();
    virtual void vf13();  virtual void vf14();  virtual void vf15();
    virtual void vf16();  virtual void vf17();  virtual void vf18();
    virtual void vf19();
    virtual void vf20();  virtual void vf21();  virtual void vf22();
    virtual void vf23();  virtual void vf24();  virtual void vf25();
    virtual void vf26();  virtual void vf27();  virtual void vf28();
    virtual void vf29();  virtual void vf30();  virtual void vf31();
    virtual void vf32();  virtual void vf33();  virtual void vf34();
    virtual void vf35();  virtual void vf36();
    virtual CtrlPcSub37Ae80* vf37();  // 0x9C

    u32 mField4;                     // 0x04
    f32 mField8;                     // 0x08
    f32 mFieldC;                     // 0x0C
    f32 mField10;                    // 0x10
    f32 mField14;                    // 0x14
    char _pad_18[0x2C - 0x18];       // 0x18-0x2B
    u32 mField2C;                    // 0x2C (aim/menu mirror word)
    char _pad_30[0x5C - 0x30];       // 0x30-0x5B
    CtrlPlayerObj* mField5C;         // 0x5C
};

// CtrlPc-compatible view exposing both vf37/vf38 (slots 0x9C/0xA0) and the
// +0x2C state word for func_8009C6BC.
class CtrlPcVf38State {
public:
    virtual ~CtrlPcVf38State();  // vtable slot 0x08
    virtual void vf01();  virtual void vf02();  virtual void vf03();
    virtual void vf04();  virtual void vf05();  virtual void vf06();
    virtual void vf07();  virtual void vf08();  virtual void vf09();
    virtual void vf10();  virtual void vf11();  virtual void vf12();
    virtual void vf13();  virtual void vf14();  virtual void vf15();
    virtual void vf16();  virtual void vf17();  virtual void vf18();
    virtual void vf19();
    virtual void vf20();  virtual void vf21();  virtual void vf22();
    virtual void vf23();  virtual void vf24();  virtual void vf25();
    virtual void vf26();  virtual void vf27();  virtual void vf28();
    virtual void vf29();  virtual void vf30();  virtual void vf31();
    virtual void vf32();  virtual void vf33();  virtual void vf34();
    virtual void vf35();  virtual void vf36();
    virtual CtrlPcSub37* vf37();  // 0x9C
    virtual CtrlPcSub37* vf38();  // 0xA0

    u32 mField4;                     // 0x04
    f32 mField8;                     // 0x08
    f32 mFieldC;                     // 0x0C
    f32 mField10;                    // 0x10
    f32 mField14;                    // 0x14
    char _pad_18[0x2C - 0x18];       // 0x18-0x2B
    u32 mField2C;                    // 0x2C (aim/menu mirror word)
    char _pad_30[0x5C - 0x30];       // 0x30-0x5B
    CtrlPlayerObj* mField5C;         // 0x5C
};

// Second global flag word probed by func_80098EF8 (retail sda21 access).
extern u32 lbl_eu_80663E28;

// CtrlPc-compatible view exposing the virtuals func_80098CB8 pokes (up to
// slot 0xB8), the +0x2C state word, the 32-float ring buffer at 0x1DC, and
// the ring index at 0x25C.
class CtrlPcVf44 {
public:
    virtual ~CtrlPcVf44();      // vtable slot 0x08
    virtual void vf01();  virtual void vf02();  virtual void vf03();
    virtual void vf04();  virtual void vf05();  virtual void vf06();
    virtual void vf07();  virtual void vf08();  virtual void vf09();
    virtual void vf10();  virtual void vf11();  virtual void vf12();
    virtual void vf13();  virtual void vf14();  virtual void vf15();
    virtual void vf16();  virtual void vf17();  virtual void vf18();
    virtual void vf19();
    virtual void vf20();  virtual void vf21();  virtual void vf22();
    virtual void vf23();  virtual void vf24();  virtual void vf25();
    virtual void vf26();  virtual void vf27();  virtual void vf28();
    virtual void vf29();  virtual void vf30();  virtual void vf31();
    virtual void vf32();  virtual void vf33();  virtual void vf34();
    virtual void vf35();  virtual void vf36();
    virtual CtrlPcSub37Ae80* vf37();  // 0x9C
    virtual void vf38();
    virtual void vf39();  // 0xA4
    virtual void vf40();  // 0xA8
    virtual void vf41();  // 0xAC
    virtual void vf42();  // 0xB0
    virtual void vf43();  // 0xB4
    virtual void vf44();  // 0xB8

    u32 mField4;                     // 0x04
    f32 mField8;                     // 0x08
    f32 mFieldC;                     // 0x0C
    f32 mField10;                    // 0x10
    f32 mField14;                    // 0x14
    char _pad_18[0x2C - 0x18];       // 0x18-0x2B
    u32 mField2C;                    // 0x2C
    char _pad_30[0x5C - 0x30];       // 0x30-0x5B
    CtrlPlayerObj* mField5C;         // 0x5C
    char _pad_60[0x1DC - 0x60];      // 0x60-0x1DB
    f32 mFloats1DC[32];              // 0x1DC ring buffer
    u32 mField25C;                   // 0x25C ring index
};

// Per-frame remote-control update (target us-80099690).
void func_80098CB8(CtrlPcVf44* self);

// Menu base teardown/recreate (menu/CMenuArtsSelect.cpp; retail exports the
// unmangled C-ABI name).
extern "C" void func_80133770();

// Circular list node used by the +0x28 head (next at +0x00).
struct CtrlSweepNode {
    CtrlSweepNode* mNext;       // 0x00
};

// Battle-manager view for func_8009BD14's list sweep: +0x28 circular list head
// (next link at +0x00), +0x1AA battle-state byte, +0x20C8 signed timer word.
struct CtrlBmSweepView {
    u8 _00[0x28];
    CtrlSweepNode* mField28;    // 0x28 list head
    u8 _2C[0x1AA - 0x2C];
    u8 mField1AA;               // 0x1AA
    u8 _1AB[0x20C8 - 0x1AB];
    s16 mField20C8;             // 0x20C8
};

// func_8016FE34 result view for func_8009BD14's sweep: u16 state word at
// +0x3388 (bit 0x10) and u32 combo word at +0x3E98, with the opaque voice-
// owner region at +0x3E9C.
struct CtrlAccSweepView {
    u8 _00[0x3388];
    u16 mField3388;             // 0x3388
    u8 _338A[0x3E98 - 0x338A];
    u32 mField3E98;             // 0x3E98
    u8 mOwner3E9C[0x10];        // 0x3E9C voice-owner region (opaque)
};

// CtrlPlayerObj view exposing the +0x3E9C voice-owner region as a u8 array
// (func_8009BD14's func_800451D8 argument, matching the acc-view shape).
struct CtrlPlayerSweepView {
    u8 _00[0x3E9C];
    u8 mOwner3E9C[0x10];       // 0x3E9C voice-owner region (opaque)
};

// Enum-list view with func_8009BD14's counter word at +0x3030.
struct CtrlEnumListSweep {
    u8 _00[0x620];
    u32 count;                  // 0x620
    u8 _624[0x3030 - 0x624];
    u32 mField3030;             // 0x3030
};

// ---------------------------------------------------------------------------
// Imports / views used by func_8009C1BC.
// ---------------------------------------------------------------------------

extern "C" int func_80190940(void* res, void* actor, int mode, void* filter);
extern "C" unsigned long func_801BA2C8(void* self);
extern "C" void func_80280ADC();
extern "C" void func_8017FEF0(void* obj, int arg);
extern "C" void* func_800EA444(void* bm);
extern "C" u32 func_8004C5EC(void* obj);
extern "C" void func_800BE12C(void* obj, int a, int b, int c, int d);
extern "C" void func_802A201C(void* a, void* b);
extern "C" void func_801B0E88();
extern "C" void func_8018C820(void* obj, int value);

namespace cf {
class CChainState;
class CChainBattleObj;
}  // namespace cf

// Chain start (chain/CChainTimer.cpp; C++ linkage - keep the mangling).
int func_8027DE44(cf::CChainState* self, cf::CChainBattleObj* p1,
                  cf::CChainBattleObj* p2, u32 p3);

// Object pointed to by sub3ED4+0x374 in func_8009C1BC; its +0x3F10 word
// seeds the command block.
struct CtrlRemoteSub374View {
    u8 _00[0x3F10];
    u32 mField3F10;               // 0x3F10
};

// Extended player->mSub3ED4 view for func_8009C1BC: slots 0xF0 (vf58) /
// 0xF4 (vf59) plus the fields it reads (+0x18 actor ref, +0x368 result
// record, +0x370/+0x374 words).
class CtrlSub3ED4Remote {
public:
    virtual ~CtrlSub3ED4Remote();  // vtable slot 0x08
    virtual void vf01();
    virtual void vf02();
    virtual void vf03();
    virtual void vf04();
    virtual void vf05();
    virtual void vf06();
    virtual void vf07();
    virtual void vf08();
    virtual void vf09();
    virtual void vf10();
    virtual void vf11();
    virtual void vf12();
    virtual void vf13();
    virtual void vf14();
    virtual void vf15();
    virtual void vf16();
    virtual void vf17();
    virtual void vf18();
    virtual void vf19();
    virtual void vf20();
    virtual void vf21();
    virtual void vf22();
    virtual void vf23();
    virtual void vf24();
    virtual void vf25();
    virtual void vf26();
    virtual void vf27();
    virtual void vf28();
    virtual void vf29();
    virtual void vf30();
    virtual void vf31();
    virtual void vf32();
    virtual void vf33();
    virtual void vf34();
    virtual void vf35();
    virtual void vf36();
    virtual void vf37();
    virtual void vf38();
    virtual void vf39();
    virtual void vf40();
    virtual void vf41();
    virtual void vf42();
    virtual void vf43();
    virtual void vf44();
    virtual void vf45();
    virtual void vf46();
    virtual void vf47();
    virtual void vf48();
    virtual void vf49();
    virtual void vf50();
    virtual void vf51();
    virtual void vf52();
    virtual void vf53();
    virtual void vf54();
    virtual void vf55();
    virtual void vf56();
    virtual void vf57();
    virtual void vf58(int arg);   // vtable slot 0xF0
    virtual void vf59();          // vtable slot 0xF4

    u8 _04[0x14];
    void* mActor18;               // 0x18 actor ref handed to func_80190940
    u8 _1C[0x368 - 0x1C];
    u32 mResult368;               // 0x368 func_80190940 result record base
    u32 mField36C;                // 0x36C
    u32 mField370;                // 0x370
    CtrlRemoteSub374View* mField374;  // 0x374
};

// Object reached through func_800EA444 whose +0x824 flag word gains bit
// 0x00080000 in func_8009C1BC.
struct CtrlRemoteEaView {
    u8 _00[0x824];
    u32 mField824;                // 0x824
};

// 0x20-byte voice/battle command block filled by func_8009C1BC and passed
// to func_8014AC38 (same layout as CVisionFxParam).
struct CtrlRemoteCmd20 {
    u32 w00;                      // 0x00
    u8 _04[2];                    // 0x04
    u8 b06;                       // 0x06
    u8 _07[0x0D - 0x07];
    u8 b0D;                       // 0x0D
    u8 b0E;                       // 0x0E
    u8 _0F;                       // 0x0F
    u16 h10;                      // 0x10
    u16 h12;                      // 0x12
    f32 f14;                      // 0x14
    u8 _18[0x20 - 0x18];
};
