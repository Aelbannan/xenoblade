#pragma once

/**
 * Object-container / unit plugin + CfObject virtuals that live in this TU.
 * Class layouts and virtuals come from the recovered cf object headers.
 */

#include <cstring>

#include "monolib/vm/yvm2.h"
#include "kyoshin/code_801862C0.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"
#include "kyoshin/cf/object/CObjectParam.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

// Retail data symbols (other splits): .sdata2 float/double pool constants
// used by the OC plugin functions. Declared here (header) per repo
// convention - never in the .cpp.
extern const float lbl_eu_8066A210;  // degrees-to-radians scale

// 6-entry u32 event-arg table (.rodata, other split) copied into a local
// buffer by func_8003CC9C before indexing.
extern const u32 lbl_eu_804FA4C0[6];

// Vtable-view structs for virtual slots the OC plugins dispatch with
// signatures that differ from the CfObject.hpp base declarations (the slot
// offsets are fixed by the recovered vtable layout).
struct CfObjVt_C8 {
    u32 _pad[0xC8 / 4];
    void (*fn)(cf::CfObject* self, float value); // vtable slot 0xC8 (float arg)
};
struct CfObjVt_128 {
    u32 _pad[0x128 / 4];
    float (*fn)(cf::CfObject* self);             // vtable slot 0x128 (float ret)
};
struct CfObjVt_1C0 {
    u32 _pad[0x1C0 / 4];
    void (*fn)(cf::CfObject* self, int value);   // vtable slot 0x1C0 (int arg)
};
struct CfObjVt_1EC {
    u32 _pad[0x1EC / 4];
    void (*fn)(cf::CfObject* self, float value); // vtable slot 0x1EC (float arg)
};
struct CfObjVt_114 {
    u32 _pad[0x114 / 4];
    void (*fn)(cf::CfObject* self, u32 value);   // vtable slot 0x114 (u32 arg)
};
struct CfObjVt_30 {
    u32 _pad[0x30 / 4];
    void* (*fn)(void* self);                     // vtable slot 0x30 (ptr ret)
};
struct CfObjVt_28 {
    u32 _pad[0x28 / 4];
    int (*fn)(void* self);                       // vtable slot 0x28 (int ret)
};

#ifdef __cplusplus
extern "C" {
#endif

void ocUnitRegist();
void func_800BC3F0(cf::CfObject* obj);
void* func_800BF324(void* objParam);
void func_8013D07C(void* obj, const char* str, int mode);
void func_8013D448(void* obj, const char* str);
unsigned int func_8013EC58();
int func_80174C98(void* player, u32* val, int mode);
u8 code80135FDC_getByte_64058();
unsigned int func_8013EC58();

#ifdef __cplusplus
}
#endif



