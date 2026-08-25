#pragma once

/**
 * Script / OC instance helpers (unit kyoshin/code_801862C0).
 * Names are still placeholder ``func_*`` until symbol recovery; signatures
 * match observed call sites in plugin TUs (VMThread in, OC object out).
 */

#include <types.h>
#include "monolib/vm/yvm2.h"
#include "kyoshin/plugin/ocBdat.hpp"

#ifdef __cplusplus
extern "C" {
#endif

void* func_801862C0(void);
void* func_801862E0(void* p);
void* func_801863F4(void* p);
void* func_80186460(void* dst, void* src);
void* func_80186474(void* p);
void* func_801864DC(void* pObj, int slot);
void func_80186664(u8* self);
void func_801866F0(struct MapObjVt** objects, int row);
void* func_80186A70(void* p);
void* func_80186BC8(int p);
void* func_80186C7C(void* p);
void* func_80186D20(void* p);

#ifdef __cplusplus
}
#endif

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" u32 lbl_eu_806642D0; // SDA singleton pointer for the arts select object (opaque pointer)
extern "C" void* memset(void*, int, unsigned long);

// Imported data symbols (other splits) referenced by func_801862C0
// One-time init guard flag in SDA.
extern s8 lbl_eu_806642C8;

// Column-name pool for the map-object spawn table (.rodata, other split).
extern u8 lbl_eu_805038C8[];
// Bdat table pointer singleton (.sbss, set up by the bdat manager).
extern void* lbl_eu_806640B0;

// Spawned map-object entry: only the vtable slots exercised by
// func_801866F0 are named; the rest are padding so the named slots land on
// the retail offsets (first declared virtual sits at vtable+0x08 due to the
// RTTI header, matching the CfCollCircleVTableIf pattern).
struct Vec3f {
    f32 x;
    f32 y;
    f32 z;
};

class MapObjVt {
public:
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
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual void _v058();
    virtual void _v05C();
    virtual void _v060();
    virtual void _v064();
    virtual void _v068();
    virtual void _v06C();
    virtual void _v070();
    virtual void _v074();
    virtual void _v078();
    virtual void _v07C();
    virtual void _v080();
    virtual void _v084();
    virtual void _v088();
    virtual void _v08C();
    virtual void _v090();
    virtual void _v094();
    virtual void _v098();
    virtual void _v09C();
    virtual void _v0A0();
    virtual void _v0A4();
    virtual void placeOnGround(void* pos, float arg); // 0xA8
    virtual Vec3f* getPos();                          // 0xAC
    virtual void _v0B0();
    virtual void _v0B4();
    virtual void placeInAir(void* pos, float arg);    // 0xB8
    virtual void applyRot(void* rot);                 // 0xBC
    virtual void _v0C0();
    virtual void _v0C4();
    virtual void _v0C8();
    virtual void _v0CC();
    virtual void _v0D0();
    virtual void _v0D4();
    virtual void _v0D8();
    virtual void setScale(float scale);               // 0xDC
    virtual void _v0E0();
    virtual void _v0E4();
    virtual void _v0E8();
    virtual void _v0EC();
    virtual void _v0F0();
    virtual void _v0F4();
    virtual void _v0F8();
    virtual void _v0FC();
    virtual void _v100();
    virtual void _v104();
    virtual void _v108();
    virtual void _v10C();
    virtual void _v110();
    virtual void _v114();
    virtual void _v118();
    virtual void _v11C();
    virtual void _v120();
    virtual void _v124();
    virtual void _v128();
    virtual void _v12C();
    virtual void _v130();
    virtual void _v134();
    virtual void _v138();
    virtual void _v13C();
    virtual void _v140();
    virtual void _v144();
    virtual void _v148();
    virtual void _v14C();
    virtual void setVisible(int visible);             // 0x150
    virtual void setVisible2(int visible);            // 0x154

    u8 pad_04[0x68]; // 0x04..0x6B
    u32 field_6C;    // 0x6C: state flag bits (disp bit 0x1000 / bit 0x8)
};

// .sdata2 constants used by func_801866F0 (defined in port/data_defs.cpp)
extern const f32 lbl_eu_806679C0;  // 0.0001 position scale
extern const f32 lbl_eu_806679C4;  // 0.01 rotation scale
extern const f32 lbl_eu_806679C8;  // 0.1 scale-column scale
extern const f32 lbl_eu_806679CC;  // 1000.0 place-helper arg
extern const f64 lbl_eu_806679D0;  // signed int-to-double magic
extern const f64 lbl_eu_806679D8;  // unsigned int-to-double magic
extern const f32 lbl_eu_8066A210;  // degrees-to-radians

// cf::CfGameManager resource-pair lookup: spawns the map object for
// (modelId, motionId) and returns it (retail mangled symbol).
class Unk80EE4Data;
extern "C" Unk80EE4Data* func_80081694__Q22cf13CfGameManagerFv(u32 modelId, u32 motionId);

// bdat column reader: canonical decl comes from plugin/ocBdat.hpp (above).
// NOTE: do NOT declare func_8003AA34 here - member TUs
// declare it with conflicting signatures (void() vs void(void*)) and an
// extern-"C" overload here breaks every TU that includes both chains.

// Matches the CfGimmick.hpp declaration verbatim so TUs including both see
// one consistent signature.
extern "C" void* func_8003AA34();
