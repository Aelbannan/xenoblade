#pragma once

#include <types.h>

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

// Minimal CfGameManager view for the direct member calls used by this unit:
// the parent object in field_00 is a CfGameManager-derived instance (its
// vtable dispatches go through CfResParentVtIf). Declared here (instead of
// including the shared CfGameManager.hpp) to avoid that header's wrong
// `void func_80069EA0()` import conflicting with the real float return.
class CfGameManager {
public:
    static u32 func_800822F4();
    static bool func_800829B8();
    u32 func_80082900();
    bool func_80085840();
};

// Object behind CfResReloadParent::field_98: its vtable slots +0x64 and
// +0x88 take a single int arg (called with 0 by func_8016D688).
struct CfResParentObjIf {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
    virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
    virtual void _v058(); virtual void _v05C(); virtual void _v060();
    virtual void _v064(int arg);  // vtable offset 0x64
    virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
    virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
    virtual void _v088(int arg);  // vtable offset 0x88
};

// field_2C view used by func_8016D688: the +0x18/+0x1C/+0x28/+0x30/+0x40
// slots each take the owning entry as r4 (retail `mr r4, r31` before each
// dispatch) and the +0x18/+0x1C results are stored to parent +0x90/+0x94.
struct CfResEntryIf2 {
    virtual void _v000();
    virtual void _v004();
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual int _v018(CfResLookupEntry* entry);  // vtable offset 0x18
    virtual int _v01C(CfResLookupEntry* entry);  // vtable offset 0x1C
    virtual void _v020();
    virtual void _v024();
    virtual int _v028(CfResLookupEntry* entry);  // vtable offset 0x28
    virtual void _v02C();
    virtual int _v030(CfResLookupEntry* entry);  // vtable offset 0x30
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual int _v040(CfResLookupEntry* entry);  // vtable offset 0x40
};

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
    /* 0x98 */ CfResParentObjIf* field_98;
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

// Polymorphic view of the parent object (a CfObject-derived instance) for the
// two virtual dispatches in func_8016DAF8: vtable slot +0x98 (returns nonzero
// to stop the reload) and slot +0x168 (takes a float arg). The vptr sits at
// +0x00 of the parent object; dummy slots pin the offsets (same pattern as
// CfResReloadVtIf).
struct CfResParentVtIf {
    virtual void _v008(); virtual void _v00C();
    virtual void _v010(); virtual void _v014(); virtual void _v018(); virtual void _v01C();
    virtual void _v020(); virtual void _v024(); virtual void _v028(); virtual void _v02C();
    virtual void _v030(); virtual void _v034(); virtual void _v038(); virtual void _v03C();
    virtual void _v040(); virtual void _v044(); virtual void _v048(); virtual void _v04C();
    virtual void _v050(); virtual void _v054(); virtual void _v058(); virtual void _v05C();
    virtual void _v060(); virtual void _v064(); virtual void _v068(); virtual void _v06C();
    virtual void _v070(); virtual void _v074(); virtual void _v078(); virtual void _v07C();
    virtual void _v080(); virtual void _v084(); virtual void _v088(); virtual void _v08C();
    virtual void _v090(); virtual void _v094();
    virtual int _v098();       // vtable offset 0x98 - nonzero stops the reload
    virtual void _v09C(); virtual void _v0A0(); virtual void _v0A4(); virtual void _v0A8();
    virtual void _v0AC(); virtual void _v0B0(); virtual void _v0B4(); virtual void _v0B8();
    virtual void _v0BC(); virtual void _v0C0(); virtual void _v0C4(); virtual void _v0C8();
    virtual void _v0CC(); virtual void _v0D0(); virtual void _v0D4(); virtual void _v0D8();
    virtual void _v0DC(); virtual void _v0E0(); virtual void _v0E4(); virtual void _v0E8();
    virtual void _v0EC(); virtual void _v0F0(); virtual void _v0F4(); virtual void _v0F8();
    virtual void _v0FC(); virtual void _v100(); virtual void _v104(); virtual void _v108();
    virtual void _v10C(); virtual void _v110(); virtual void _v114(); virtual void _v118();
    virtual void _v11C(); virtual void _v120(); virtual void _v124(); virtual void _v128();
    virtual void _v12C(); virtual void _v130(); virtual void _v134(); virtual void _v138();
    virtual void _v13C(); virtual void _v140(); virtual void _v144(); virtual void _v148();
    virtual void _v14C(); virtual void _v150(); virtual void _v154(); virtual void _v158();
    virtual void _v15C(); virtual void _v160(); virtual void _v164();
    virtual void _v168(float value);  // vtable offset 0x168
    virtual void _v16C(); virtual void _v170(); virtual void _v174();
    virtual void _v178();  // vtable offset 0x178 - dispatched by func_8016D3F8
    virtual void _v17C();  // vtable offset 0x17C - dispatched by func_8016D3F8
};

// Cast-only fake SI interface for the secondary vtable at +0x10 (see
// MWCC_REFERENCE "double-hop thunks"): the non-polymorphic Shift base puts the
// vptr at object+0x10, so a virtual call emits `lwz r12,0x10(r3);
// lwz r12,slot(r12); mtctr; bctr` with `this` staying at the object base.
struct CfResReloadShift {
    u8 field_00[0x10];
};

struct CfResReloadVtIf : CfResReloadShift {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();  // vtable offset 0x28 - dispatched by func_8016DE68
    virtual void _v02C();
    virtual void _v030();
    virtual int _v034(int);  // vtable offset 0x34 - called with (this, 1) by func_8016CF24
};

// Resource reload implementation.
// Manages reloading of resources with a timer and state tracking.
// Inherits from CfResImpl (base class defines fields 0x00-0x13).
struct CfResReloadImpl {
    /* 0x00 */ CfResReloadParent* field_00;   // parent/reference pointer
    /* 0x04 */ f32 field_04;                  // timer or delay float
    /* 0x08 */ u16 field_08;                  // type/category (also PMTF index in func_8016DE8C)
    /* 0x0A */ s16 field_0A;                  // state/param (-1 = invalid; lha by callers)
    /* 0x0C */ u16 field_0C;                  // counter
    /* 0x0E */ u16 field_0E;                  // state/param (-1 = invalid)
    /* 0x10 */ void* field_10;  // secondary vtable pointer (via CfResReloadVtIf cast)
    /* 0x14 */ u32 field_14[2];               // work buffer (2 words)
    /* 0x1C */ s16 field_1C;                  // reload count/state (lha by func_8016D3F8)
    /* 0x1E */ u8 field_1E;                   // flags
    /* 0x1F */ u8 field_1F;                   // flags

    // vtable slot 6: returns a resource type identifier (12 for reload impl)
    int func_8016CF1C();

    // vtable slot 23: returns a resource sub-type identifier (1 for reload impl)
    int func_8016DECC();
};

// Interface for the object behind CfResLookupEntry::field_2C: its vtable slot
// at +0x40 reports whether the resource is currently in use (func_8016CE5C).
struct CfResEntryObjIf {
    virtual void _v000();
    virtual void _v004();
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual int _v040();  // vtable offset 0x40 - in-use test
};

// Entry returned by the CfRes table lookup func_80062EC4 (indexed by the
// state field); +0x00 (flags), +0x32 (attribute id), +0x04 (resource ptr) and
// +0x2C (resource object) are read by this unit.
struct CfResLookupEntry {
    /* 0x00 */ u32 field_00;  // flags (bit 0x800 = present, bit 0x1000 = sound)
    /* 0x04 */ u8* field_04;  // resource pointer (compared against parent +0x70)
    u8 field_08[0x24];        // 0x08..0x2B
    /* 0x2C */ CfResEntryObjIf* field_2C;  // resource object (virtual +0x40 in-use test)
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
// incompatible casts (see MWCC_REFERENCE "in-progress stub" pattern) so the
// -ipa file inliner cannot fold them into the call sites. All are C-ABI-style
// retail symbols (plain names), so extern "C" keeps the call relocs at the
// plain retail names.
extern "C" int func_8016E654(u32 type, u32 arg2, int arg3, u16* out1, u16* out2);
extern "C" void func_8016E1AC();
extern "C" int func_8016E430(u32 type);
extern "C" u16 func_8016E854(cf::CfResReloadImpl* self, u16* out1, u16* counter,
                             u16* out2, u16* out3);
extern "C" int func_8016E578(u32 type, int sub);
extern "C" void func_8016EA68(cf::CfResReloadImpl* self);

// func_800AD860 (retail mangled C++ symbol func_800AD860__FPv, single void*
// parameter): a plain C++ declaration (not extern "C") yields the matching
// linker name.
extern cf::CfResEneObj* func_800AD860(void* obj);

// Forward decl for the C-ABI player helpers declared above.
namespace cf { class CfObjectMove; }

// Free-function imports used by this unit (defined in CfRes.cpp /
// CfObjectModel.cpp). extern "C" keeps the call-site reloc at the plain
// retail name - a C++ declaration makes MWCC emit a mangled __F<params>
// name (same convention as CfObjectModel.hpp).
// C-ABI imports used by func_8016D3F8 (defined in CfRes.cpp / IResInfo.cpp /
// code_800B06A4.cpp / object/CfObjectModel.cpp).
extern "C" int func_80062B3C(u32 handle, u32 state);
extern "C" int func_80062BAC(int handle);
extern "C" cf::ResReloadFindEntry* findResEntry(u8* self, u32 id, u32* outIndex, u32* outValue);
extern "C" cf::ResReloadFindEntry* func_80068564(u8* self, u32 id, u32* outIndex, u32* outValue);
extern "C" int func_800B4A24(cf::CfResReloadParent* parent);
extern "C" void func_800B1BBC(int arg);
extern "C" void func_800BBB50(cf::CfObjectModel* self);
// Imports used by func_8016D688 (defined in CfTFile.cpp / CfRes.cpp /
// CfGameManager.cpp / object/CfObjectModel.cpp).
extern "C" float func_80069EA0();
extern "C" void func_80434A4C__Q23mtl10MemManagerFb(bool value);
extern "C" u8* func_80489A60(u8* global, u8* handle, int a, int b, int c, int d);
extern "C" void func_800BBADC(cf::CfResReloadParent* parent, u8* handle);
extern "C" int func_800AA33C(u8* buf, u32 packed, int prefixFlag, int suffixFlag);
extern "C" u8* func_800584B8(u32 handle, u8* buf);
extern "C" int CfRes_getD80Flag();
extern "C" void CfRes_stub_63990();
extern "C" void func_800BB618(cf::CfResReloadParent* parent, int arg);
extern "C" void func_800BCFA0(cf::CfResReloadParent* parent);
extern "C" void func_800BE824(cf::CfResReloadParent* parent, int flag);
extern "C" void func_804B0A6C(u8* subObj, u8* handle);
extern "C" void func_800BE12C(u32 handle, int a, int b, int c, int d);

// Scene/manager globals read by func_8016D688.
extern u8* lbl_eu_80663E14;
// .sdata2 floats used by func_8016D688 / func_8016E1AC.
extern float lbl_eu_8066769C;
extern float lbl_eu_806676A0;

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
// More C-ABI imports used by func_8016D688.
extern "C" void func_800AD4B0(cf::CfResReloadParent* parent);
// More C-ABI imports used by this unit's functions.
extern "C" cf::DeviceSearchEntry* func_80068928(u8* self, u32 id, int start, int end);
extern "C" int func_801BFE20(int a, int b, u8* c, float f1, float f2);
extern "C" cf::SoundSlotEntry* func_801BFAE4(u16 handle);
extern "C" u32 getBdatStringColumnValue(void* bdat, const char* column, int index);
extern "C" u16 func_8006A6D0();
extern "C" void* CfRes_getInstanceField();
// More C-ABI imports used by func_8016DF4C / func_8016DAF8 / func_8016EA68.
extern "C" void func_8018896C(int index, unsigned int type, float f1, float f2);
extern "C" float func_80069EE4();
extern "C" void func_800BC3B0(cf::CfObjectMove* player, float value);
extern "C" void func_800BC4A0(cf::CfObjectMove* player);

// Flag words defined in CUICfManager.cpp (.sbss); read by func_8016D2FC /
// func_8016E9CC.
extern u32 lbl_eu_80663E24;
extern u32 lbl_eu_80663E28;
// Delay/timer floats (.sdata2) selected by func_8016E9CC.
extern float lbl_eu_806676B4;
extern float lbl_eu_806676B8;
// More .sdata2 constants used by func_8016DAF8 / func_8016DF4C / func_8016EA68.
extern float lbl_eu_806676A4;    // vtable-slot-0x168 float arg
extern float lbl_eu_806676A8;    // func_800BC3B0 restore-heal float
// 2^52 conversion constant: u16 -> float via the double-magic trick (retail
// references the named .sdata2 double; a direct (f32) cast would pool a
// TU-local constant instead).
extern double lbl_eu_806676C0;
extern float lbl_eu_806676BC;    // func_8018896C second float arg

// Secondary-interface vtable stored at +0x10 by the constructor (.data).
extern void* lbl_eu_80530FF0[];
// Delay/timer float seeded by the constructor (.sdata2).
extern float lbl_eu_80667698;

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
