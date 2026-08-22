#pragma once

#include <types.h>

namespace cf {

class CfObject; // forward decl (effect object array elements)

// Scene object referenced by CfMapEffectManager::field_0x134 (released via
// func_800B9404 once func_800B8920 validates it).
struct CfMapEffectHandle {
    u8 field_0x0[0xB0];
    u32 field_0xB0;
};

// Effect-object view exposing the release flag at 0xB0 used by
// func_8016EF2C / func_8016FC98 (the concrete effect subclasses are opaque
// here).
struct CfMapEffectObject {
    u8 field_0x0[0xB0];
    u32 field_0xB0; // 0xB0
};

// Layout recovered from __ct__cf_CfMapEffectManager / func_8016F9D4 /
// func_8016FC0C / func_8016FBA8. The vtable pointer at 0x00 is stored
// manually (lbl_eu_80531080) so the ctor emits the exact retail reloc name
// instead of an MWCC auto-generated __vt__ label.
class CfMapEffectManager {
public:
    ~CfMapEffectManager();

    u8* field_0x0;                    // 0x00 vtable pointer (lbl_eu_80531080)
    s16 field_0x4;                    // 0x04
    s16 field_0x6;                    // 0x06
    s16 field_0x8;                    // 0x08
    s16 field_0xA;                    // 0x0A
    CfObject* field_0x0C[0x24];       // 0x0C effect objects (36 entries)
    u16 field_0x9C[0x24];             // 0x9C per-slot u16 counter bank 1
    u16 field_0xE4[0x24];             // 0xE4 per-slot u16 counter bank 2
    s16 field_0x12C;                  // 0x12C
    s16 field_0x12E;                  // 0x12E
    s16 field_0x130;                  // 0x130
    CfMapEffectHandle* field_0x134;   // 0x134 scene object
    u8 field_0x138;                   // 0x138 active flag
};

// u16/u32 -> float conversion helper: the retail code builds the
// 0x43300000-prefixed bit pattern in memory and subtracts the shared sdata2
// magic double (CfTFile.cpp convention) so the pool reloc matches the retail
// constant instead of an MWCC-synthesised @N entry.
union F64Conv {
    u32 w[2];
    double d;
};

class CfObjectEff; // concrete effect object (defined in CfObjectEff.hpp)

// Vtable-view structs for virtual slots whose signatures differ from the
// base CfObject.hpp declarations (code_8027513C.cpp convention).
struct CfObjectEff_VTable39 {
    u32 _pad[39];                      // vtable words 0x00..0x98
    void (*fn)(CfObjectEff*, float*);  // slot 0x9C
};
struct CfObjectEff_VTable47 {
    u32 _pad[47];                      // vtable words 0x00..0xB8
    void (*fn)(CfObjectEff*, float*);  // slot 0xBC
};
struct CfGameManager_VTable101 {
    u32 _pad[0x65];                    // vtable words 0x00..0x190
    int (*fn)(void*);                  // slot 0x194
};

} // namespace cf

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------
extern "C" {
void func_800AD040(char* obj, int flag); // CfObjectEff flag setter
u16 func_8016DF2C();                     // play-time seconds getter
// Sibling functions of this unit (retail 0x801702E0 / 0x801706A0). Kept as
// declared-only externs so calls from matched functions emit a real bl - a
// same-TU stub definition would be inlined by MWCC (-O4).
void func_8016EF2C(cf::CfMapEffectManager* pSelf);
void func_8016F2A4(cf::CfMapEffectManager* pSelf, int index, u8* bdat);
void func_800ACC28(cf::CfObject* object, float first, float second);
// CfGameManager phase/counter getters (same signatures as CfGimmickItem.hpp;
// CfGimmickItem.hpp itself cannot be included here - its CfGimmick.hpp
// re-declares func_8009CF8C with a conflicting signature).
unsigned int func_800822F4__Q22cf13CfGameManagerFv(void);
unsigned int func_80082354__Q22cf13CfGameManagerFv(unsigned int a);
int func_8016EFD8(int unused, int index);
void func_8016FC98(cf::CfMapEffectManager* pSelf, cf::CfObject* target);
void func_8016FD84(float first, float second);
int func_8016FA68(int unused, int b, int c, int d); // same-TU definition below
void func_8006A6D0();
void* func_800817BC__Q22cf13CfGameManagerFv(u32 value, u32 unused);
}

// Retail data symbols (linker-provided)
extern u8 lbl_eu_80531080[];                  // data: vtable
extern cf::CfMapEffectManager* lbl_eu_80664290; // sbss: singleton instance
extern u8* lbl_eu_806640AC;                   // sbss: bdat file pointer
extern float lbl_eu_806623E8;                 // sdata: global float (set by func_8016FE2C)
extern const float lbl_eu_80667730;           // sdata2: clamp threshold
// rodata: BDAT column-name string table (7 strings at 0x00/0x08/0x11/0x1A/0x20/0x26/0x2C)
extern char lbl_eu_80503248[];
extern u16 lbl_eu_80661AF4;                   // sdata: play-clock / BGM frame counter
extern u8* lbl_eu_806623EC;                   // sdata: scratch pointer (byte at +0x06)
// sdata2 float/double pool constants (func_8016F2A4)
extern const float lbl_eu_80667710;           // default flash value (func_800ACC28 arg 2)
extern const float lbl_eu_80667714;           // u32 column -> float scale
extern const float lbl_eu_80667718;           // s16 column -> float scale
extern const double lbl_eu_80667720;          // 2^52 + 2^31 (u32/s16 -> float magic)
extern const double lbl_eu_80667728;          // 2^52 (u16 -> float magic)
extern const float lbl_eu_8066A210;           // final position scale
