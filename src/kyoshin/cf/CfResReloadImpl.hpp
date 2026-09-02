#pragma once

#include <types.h>
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
#include "monolib/math/FloatUtils.hpp"  // H3 label-owner decl (lbl_eu_8066A208)

// Minimal nw4r sound-object interface used by func_8016CFDC (slot entry +0x00
// dereferenced to call SetPlayerPriority). Declared here rather than via
// snd_BasicSound.h to keep this catalog TU's include graph light; MWCC mangles
// the member call to the retail name
// SetPlayerPriority__Q44nw4r3snd6detail10BasicSoundFi.
namespace nw4r {
namespace snd {
namespace detail {
class BasicSound {
public:
    void SetPlayerPriority(int priority);
};
} // namespace detail
} // namespace snd
} // namespace nw4r

class CResLookup;

namespace cf {

// Forward decl (needed by the vtable struct below).
struct CfResReloadImpl;
struct CfResLookupEntry;

// Sub-object at parent +0xC4 (CfObject-derived); the flag words at +0x0C and
// +0x4EC are read by func_8016DAF8.
struct CfResReloadParentSub {
    u8 field_00[0x0C];
    /* 0x0C */ u32 field_0C;    // flags (bits 1 / 6 / 16 tested)
    u8 field_10[0x4DC];         // 0x10..0x4EB
    /* 0x4EC */ u32 field_4EC;  // flags (bit 1 tested)
};

// Minimal CfGameManager view note: the shared CfGameManager.hpp is already
// pulled in transitively (harness_catalog.hpp -> CTaskGameEff.hpp), so the
// cf::CfGameManager methods used by this unit come from there. func_80069EA0
// is declared void by that header but really returns float - call it through
// a cast at the use site.
// Parent object referenced at +0x00 of CfResReloadImpl; the flag words at
// +0x68/+0x6C and the resource pointer at +0x70 are read/written here.
struct CfResReloadParent {
    u8 field_00[0x64];
    /* 0x64 */ u32 field_64;  // flags (bit 16 = 0x10000 tested by func_8016D2FC)
    /* 0x68 */ u32 field_68;  // flags (bit 26 = 0x04000000 tested by func_8016D390)
    /* 0x6C */ u32 field_6C;  // flags (bit 2 tested by func_8016DE68)
    /* 0x70 */ u8* field_70;  // resource pointer passed to func_eu_80063174
    /* 0x74 */ u8* field_74;  // sound-related pointer passed to func_801BFE20
    u8 field_78[0x18];        // 0x78..0x8F
    /* 0x90 */ u8* field_90;  // slot +0x18 result (cleared by func_8016D3F8)
    /* 0x94 */ u8* field_94;  // slot +0x1C result (cleared by func_8016D3F8)
    /* 0x98 */ void* field_98;
    /* 0x9C */ u8* field_9C;
    u8 field_A0[0x24];        // 0xA0..0xC3
    /* 0xC4 */ CfResReloadParentSub* field_C4;  // sub-object (flag words at +0x0C/+0x4EC)
    u8 field_C8[0x544];       // 0xC8..0x60B
    /* 0x60C */ u8 field_60C[0xB8];  // sub-object passed to func_804B0A6C
    /* 0x6C4 */ u32 field_6C4;
    u8 field_6C8[0x14];       // 0x6C8..0x6DB
    /* 0x6DC */ CfResLookupEntry* field_6DC;
    /* 0x6E0 */ u32 field_6E0;
    /* 0x6E4 */ u8 field_6E4[0x20];   // tail padding (struct ends at 0x704)
};

// CfResParent deleted: use cf::CfObject / CfObjectModel real virtuals
// slot98 -> CfObject_UnkVirtualFunc18 (0x98, int)
// slot168 -> CfObject_UnkVirtualFunc70 (0x168, float)
// slot178 -> CfObjectModel_UnkVirtualFunc1 (0x178)
// slot17C -> CfObjectModel_UnkVirtualFunc2 (0x17C)

// Prefix pushing vptr to +0x10 (CHelp pattern). Base CfResImpl defines the
// 0x00-0x0E prefix; the vptr follows at +0x10, then the 0x14 tail.
struct CfResReloadImplPrefix {
    /* 0x00 */ CfResReloadParent* field_00; // parent/reference pointer
    /* 0x04 */ f32 field_04;                // timer or delay float
    /* 0x08 */ u16 field_08;                // type/category (also PMTF index)
    /* 0x0A */ u16 field_0A;                // state/param (-1 = invalid)
    /* 0x0C */ u16 field_0C;                // counter
    /* 0x0E */ s16 field_0E;                // state (-1 = invalid)
};

struct CfResReloadImplVtbl {
    void* slots[27]; // RTTI + 0 + 25 virtuals (0x6C)
};

// Real class tree for cf::CfResReloadImpl (retail lbl_eu_80530FF0, JP
// __vt__Q22cf15CfResReloadImpl is not emitted; US/EU lbl_eu_80530FF0 is
// 0x6C: RTTI + 0 + 25 slots). novtable: TU has no .data vtable, ctor
// writes lbl_eu_80530FF0 at +0x10 like CToken / CHelp.
class __declspec(novtable) CfResReloadImpl : public CfResReloadImplPrefix {
public:
    // 25 virtuals in retail order (offset 0x08 .. 0x68)
    virtual ~CfResReloadImpl();                 // 0x08
    virtual void func_8016DE8C();               // 0x0C - PMTF dispatch
    virtual void func_8016D240();               // 0x10
    virtual void func_8016CE3C();               // 0x14
    virtual int func_8016CE5C();                // 0x18 - in-use test
    virtual void func_8016DCE4();               // 0x1C
    virtual int func_8016CF1C();                // 0x20 - type id (12)
    virtual void func_800BC2DC();               // 0x24 - base CfResImpl slot
    virtual void func_8016DDE8();               // 0x28 - reset
    virtual void func_eu_8016F1C4();            // 0x2C
    virtual void func_8016DED4();               // 0x30
    virtual u32 func_8016CFBC(int index);       // 0x34 - work word getter
    virtual void func_800BEA38();               // 0x38
    virtual void func_800BED64();               // 0x3C
    virtual void func_800BEE30();               // 0x40
    virtual void func_800BC3AC();               // 0x44
    virtual int func_8016D2FC(int arg2);        // 0x48
    virtual int func_8016D390(int arg2);        // 0x4C
    virtual void func_8016CD5C();               // 0x50
    virtual void func_8016CFDC(int arg2, int arg3, float f1, float f2, int priority); // 0x54
    virtual void func_8016D0C0(int arg2, int arg3); // 0x58
    virtual void func_8016D144(int arg2, int arg3, int arg4); // 0x5C
    virtual int func_8016D1D8();                // 0x60
    virtual int func_8016DECC();                // 0x64 - sub-type id (1)
    virtual int func_8016CF24();                // 0x68 - random byte

    void*& vtbl() {
        return *reinterpret_cast<void**>(reinterpret_cast<u8*>(this) + 0x10);
    }

    /* 0x14 */ u32 field_14[2]; // work buffer (2 words)
    /* 0x1C */ s16 field_1C;    // reload count/state
    /* 0x1E */ u8 field_1E;     // flags
    /* 0x1F */ u8 field_1F;     // flags
};

// Entry returned by the CfRes table lookup func_80062EC4 (indexed by the
// state field); +0x00 (flags), +0x32 (attribute id), +0x04 (resource ptr) and
// +0x2C (resource object) are read by this unit.
struct CfResLookupEntry {
    /* 0x00 */ u32 field_00;  // flags (bit 0x800 = present, bit 0x1000 = sound)
    /* 0x04 */ u8* field_04;  // resource pointer (compared against parent +0x70)
    u8 field_08[0x24];        // 0x08..0x2B
    /* 0x2C */ ::CResLookup* field_2C;  // resource object (virtual +0x40 in-use test)
    u8 field_30[0x2];         // 0x30..0x31
    /* 0x32 */ u8 field_32;   // attribute id
};

// Enemy-object flag view returned by func_800AD860: the u16 flag word at
// +0x45CA and the u32 at +0x3374 are read/written by func_8016DAF8.
struct CfResEneObj {
    u8 field_00[0x3374];
    /* 0x3374 */ u32 field_3374;
    u8 field_3378[0x1252];    // 0x3378..0x45C9
    /* 0x45CA */ u16 field_45CA;
};

// Entry returned by func_80068928 (device search); func_8016D240 increments
// the s16 counters at +0x38/+0x3A.
struct DeviceSearchEntry {
    u8 field_00[0x38];
    /* 0x38 */ s16 field_38;
    /* 0x3A */ s16 field_3A;
};

// Slot entry returned by func_801BFAE4 (CfSoundMan slot lookup); +0x00 holds
// the active sound object, +0x2A a u16 flag word (cf. pluginSnd.cpp).
struct SoundSlotEntry {
    /* 0x00 */ nw4r::snd::detail::BasicSound* field_00;
    u8 field_04[0x26];
    /* 0x2A */ u16 field_2A;
};

// 0x10-byte reload-info struct written by initReloadInfoStruct and read back
// by func_8016E100.
struct ReloadInfo {
    /* 0x00 */ f32 field_00;
    /* 0x04 */ u16 field_04;
    /* 0x06 */ u16 field_06;
    /* 0x08 */ u16 field_08;
    /* 0x0A */ u16 field_0A;
    /* 0x0C */ u16 field_0C;
    /* 0x0E */ u16 field_0E;
};

// Entry returned by findResEntry / func_80068564 (reload-table search used by
// func_8016D3F8); the +0x0C word is cleared on a successful reload.
struct ResReloadFindEntry {
    u8 field_00[0x0C];
    /* 0x0C */ u32 field_0C;
};

} // namespace cf

// PMTF dispatch table (4 x 12-byte member pointers) selected by field_08 in
// func_8016DE8C. Declared at global scope so MWCC keeps the retail name
// unmangled; the (this->*table[idx])() call lowers to `bl __ptmf_scall`.
typedef void (cf::CfResReloadImpl::*CfResReloadImplPMF)();
extern CfResReloadImplPMF lbl_eu_80530FC0[4];

// Unit-internal helpers (defined in CfResReloadImpl.cpp). func_8016E430 /
// func_8016E854 / func_8016E578 / func_8016EA68 have real bodies; the
// not-yet-decompiled func_8016E654 / func_8016E1AC stubs are called through
// incompatible casts (see MWCC_CASES "in-progress stub" pattern) so the
// -ipa file inliner cannot fold them into the call sites. All are C-ABI-style
// retail symbols (plain names), so extern "C" keeps the call relocs at the
// plain retail names.
extern "C" int func_8016E654(u32 type, u32 arg2, int arg3, u16* out1, u16* out2);
extern "C" int func_8016E1AC(cf::CfResReloadImpl* self, u32 arg2, int arg3);
extern "C" int func_8016E430(u32 type);
extern "C" u16 func_8016E854(cf::CfResReloadImpl* self, u16* out1, u16* counter,
                             u16* out2, u16* out3);
extern "C" int func_8016E578(u32 type, int sub);
extern "C" void func_8016EA68(cf::CfResReloadImpl* self);

// func_800AD860 (retail mangled C++ symbol getEffOwner____FPv, single void*
// parameter): a plain C++ declaration (not extern "C") yields the matching
// linker name. Return type is void* to match CfObjectPc.hpp (the same
// function is declared there first; C++ rejects differing return types).
extern void* func_800AD860(void* obj);

// Forward decl for the C-ABI player helpers declared above.
namespace cf { class CfObjectMove; }

// Free-function imports used by this unit (defined in CfRes.cpp /
// CfObjectModel.cpp). extern "C" keeps the call-site reloc at the plain
// retail name - a C++ declaration makes MWCC emit a mangled __F<params>
// name (same convention as CfObjectModel.hpp).
// C-ABI imports used by func_8016D3F8 (defined in CfRes.cpp / IResInfo.cpp /
// code_800B06A4.cpp / object/CfObjectModel.cpp).
extern "C" int func_80062B3C(u32 handle, u32 state);
extern "C" int func_80062BAC(int handle, int state);
extern "C" cf::ResReloadFindEntry* findResEntry(u8* self, u32 id, u32* outIndex, u32* outValue);
extern "C" cf::ResReloadFindEntry* func_80068564(u8* self, u32 id, u32* outIndex, u32* outValue);
extern "C" int func_800B4A24(cf::CfResReloadParent* parent);
extern "C" void func_800B1BBC(int arg);
extern "C" void func_800BBB50(cf::CfObjectModel* self);
// Imports used by func_8016D688 (defined in CfTFile.cpp / CfRes.cpp /
// CfGameManager.cpp / object/CfObjectModel.cpp).
extern "C" void setMemInitFlag__Q23mtl10MemManagerFb(bool value);
extern "C" u8* func_80489A60(u8* global, u8* handle, int a, int b, int c, int d);
extern "C" void func_800BBADC(cf::CfResReloadParent* parent, u8* handle);
// func_800AA33C is declared in IResInfo.hpp (ml::FixStr version)
extern "C" u8* func_800584B8(u32 global, u32 id, const char* name);
extern "C" int CfRes_getD80Flag();
extern "C" void CfRes_stub_63990();
// func_800BB618/func_800BCFA0 are owned by kyoshin/cf/object/CfObjectMove.hpp
// (single extern "C" decl; overloading C-ABI functions is illegal).
extern "C" void func_800BE824(cf::CfResReloadParent* parent, int flag);
extern "C" void func_804B0A6C(u8* subObj, u8* handle);
// func_800BE12C is owned by kyoshin/cf/object/CfObjectMove.hpp.

// Scene/manager globals read by func_8016D688.
// .sdata2 floats used by func_8016D688 / func_8016E1AC.
extern float lbl_eu_8066769C;
extern float lbl_eu_806676A0;
extern float lbl_eu_806676CC;
extern const double lbl_eu_806676D0;

extern "C" cf::CfResLookupEntry* func_80062EC4(int);
extern "C" int func_80062998(int, int, int);
extern "C" void func_eu_80063174(int, u8*);
extern "C" void func_800BAB64(cf::CfResReloadParent*);
// More C-ABI imports (defined in CfSoundMan.cpp / CfRes.cpp /
// code_801A929C.cpp); same extern "C" convention as above.
extern "C" void func_801BFE8C(u32 a, u32 b, u32 c);
extern "C" void func_801BFF04(int a, int b, int c, int d);
extern "C" int func_80063A60(int a);
extern "C" int func_801AAAA0(int a);
// More C-ABI imports used by this unit's functions.
extern "C" cf::DeviceSearchEntry* func_80068928(u8* self, u32 id, int start, int end);
extern "C" int func_801BFE20(int a, int b, u8* c, float f1, float f2);
extern "C" cf::SoundSlotEntry* func_801BFAE4(u16 handle);
extern "C" u16 func_8006A6D0();
extern "C" void* CfRes_getInstanceField();
// More C-ABI imports used by func_8016DF4C / func_8016DAF8 / func_8016EA68.
extern "C" void func_8018896C(int index, unsigned int type, float f1, float f2);
extern "C" float func_80069EE4();
extern "C" void func_800BC3B0(cf::CfObjectMove* player, float value);
extern "C" void func_800BC4A0(cf::CfObjectMove* player);

// Flag words defined in CUICfManager.cpp (.sbss); read by func_8016D2FC /
// func_8016E9CC.
extern u32 lbl_eu_80663E28;
// Delay/timer floats (.sdata2) selected by func_8016E9CC.
extern float lbl_eu_806676B4;
extern float lbl_eu_806676B8;
// More .sdata2 constants used by func_8016DAF8 / func_8016DF4C / func_8016EA68.
extern float lbl_eu_806676A4;    // vtable-slot-0x168 float arg
extern float lbl_eu_806676A8;    // func_800BC3B0 restore-heal float
// 2^52 conversion constant: u16 -> float via the double-magic trick (retail
// references the named .sdata2 double; a direct (f32) cast would pool a
// TU-local constant instead). const -> readonly sdata2 pool.
extern const double lbl_eu_806676C0;
extern float lbl_eu_806676BC;    // func_8018896C second float arg

// Secondary-interface vtable stored at +0x10 by the constructor (.data).
// Typed-object declaration (CHelp pattern): the ctor stores &symbol.
extern cf::CfResReloadImplVtbl lbl_eu_80530FF0;
// Delay/timer float seeded by the constructor (.sdata2). const -> readonly
// sdata2 pool: lets MWCC hoist the lfs above the member stores (CArtsInfo
// pattern).
extern const float lbl_eu_80667698;

// BDAT table pointer and cached row index read by func_8016E578 / func_8016EC58.
extern void* lbl_eu_806640A8;   // BDAT table pointer (.sbss)
extern u32 lbl_eu_80664184;     // cached BDAT row index (read as u16)
// Column-name string block base and pointer table used with getBdatStringColumnValue.
extern char lbl_eu_80503140[];
extern char* lbl_eu_80531068[];

// Reload-state globals owned by this unit (.sbss / .sdata2). Global-scope
// declarations keep the retail symbol names unmangled.
extern u16 lbl_eu_80664278;
extern u16 lbl_eu_8066427A;
extern u16 lbl_eu_8066427C;
extern u16 lbl_eu_8066427E;
extern u16 lbl_eu_80664280;
extern float lbl_eu_80664284;
extern u16 lbl_eu_80664288;
extern float lbl_eu_806676C8;
