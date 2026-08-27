#pragma once

/**
 * Object-container / unit plugin + CfObject virtuals that live in this TU.
 * Class layouts and virtuals come from the recovered cf object headers.
 */

#include <cstring>

#include "monolib/vm/yvm2.h"
#include "kyoshin/cf/object/CActorParam.hpp"
#include "kyoshin/cf/object/CObjectParam.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

// Forward decls for func_800AB580's signature (defined in
// kyoshin/cf/object/CfObjectColl.cpp); that header cannot be included here
// because it carries its own cf::CfObject declaration.
namespace cf {
class CfObjectColl;
}
namespace ml {
class CVec3;
}

// Retail data symbols (other splits): .sdata2 float/double pool constants
// used by the OC plugin functions. Declared here (header) per repo
// convention - never in the .cpp.
// .sdata2 float/double pool constants shared by the OC plugin functions.
extern const float lbl_eu_8066A210;  // degrees-to-radians scale
extern const float lbl_eu_80665C30;  // fixed-point scale (2048.0)
extern const double lbl_eu_80665C38; // sdata2: int->double magic (2^52 + 2^31)

// 6-entry u32 event-arg table (.rodata, other split) copied into a local
// buffer by func_8003CC9C before indexing.
extern const u32 lbl_eu_804FA4C0[6];

// .sdata2 float constant passed by func_8003FFF4 to func_800AB580.
extern const float lbl_eu_80665D44;

// Defined in kyoshin/cf/object/CfObjectColl.cpp (declaration kept here so
// plugin call sites see one consistent signature).
extern "C" int func_800AB580(cf::CfObjectColl* self, cf::CfObject* obj, ml::CVec3* out, float f1);

// Moves an object to pos with the given angle, relative to its move object.
// Also defined in CfObjectColl.cpp.
extern "C" void func_800ABFC4(cf::CfObject* obj, void* moveObj, ml::CVec3* pos, float angle);

// Vtable-view structs for virtual slots the OC plugins dispatch with
// signatures that differ from the CfObject.hpp base declarations (the slot
// offsets are fixed by the recovered vtable layout).
struct CfObjVt_AC {
    u32 _pad[0xAC / 4];
    void* (*fn)(cf::CfObject* self);             // vtable slot 0xAC (ptr ret)
};
// Interface view of vtable slot 0xAC (returns the object's move object).
// Declared as real virtuals so call sites use the r12/r12 virtual-call idiom.
struct CfObjIf_AC {
    virtual void _v0008();
    virtual void _v000C();
    virtual void _v0010();
    virtual void _v0014();
    virtual void _v0018();
    virtual void _v001C();
    virtual void _v0020();
    virtual void _v0024();
    virtual void _v0028();
    virtual void _v002C();
    virtual void _v0030();
    virtual void _v0034();
    virtual void _v0038();
    virtual void _v003C();
    virtual void _v0040();
    virtual void _v0044();
    virtual void _v0048();
    virtual void _v004C();
    virtual void _v0050();
    virtual void _v0054();
    virtual void _v0058();
    virtual void _v005C();
    virtual void _v0060();
    virtual void _v0064();
    virtual void _v0068();
    virtual void _v006C();
    virtual void _v0070();
    virtual void _v0074();
    virtual void _v0078();
    virtual void _v007C();
    virtual void _v0080();
    virtual void _v0084();
    virtual void _v0088();
    virtual void _v008C();
    virtual void _v0090();
    virtual void _v0094();
    virtual void _v0098();
    virtual void _v009C();
    virtual void _v00A0();
    virtual void _v00A4();
    virtual void _v00A8();
    virtual void* getMoveObj(); // vtable slot 0xAC
};

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

// Interface view of the talk-state sub-object embedded 0x3E9C bytes before
// the CfObject (isTalk): vtable slot 0x30 returns a pointer to its state word.
struct TalkSubIf {
    virtual void _v0008();
    virtual void _v000C();
    virtual void _v0010();
    virtual void _v0014();
    virtual void _v0018();
    virtual void _v001C();
    virtual void _v0020();
    virtual void _v0024();
    virtual void _v0028();
    virtual void _v002C();
    virtual u32* _v0030();
};

// Script/OC instance helpers (defined in code_801862C0.cpp). Declared here
// instead of including code_801862C0.hpp to keep this header light;
// getBdatStringColumnValue has a single canonical declaration on
// kyoshin/plugin/ocBdat.hpp.
extern "C" void* func_801862C0(void);
extern "C" void* func_801862E0(void* p);
extern "C" void* func_801863F4(void* p, void* obj);
extern "C" void* func_80186460(void* dst, void* src);
extern "C" void* func_801864DC(void* pObj, int slot);

struct Vec3f {
    f32 x;
    f32 y;
    f32 z;
};

// Flag words of the objects returned by the OC spawn/lookup plugin
// func_8003BD7C (bdat-name dispatch).
struct OcSpawnObjView {
    u8 _0000[0x64];
    u32 field_0x64;
    u32 field_0x68;
};

// View of the OC handle word lookAt reads from the target lookup result.
struct OcTargetView {
    u8 _0000[0x04];
    u32 field_0x04; // OC object handle
};

// View of the CfObject bone-target word the lookAt plugin tests.
struct CfObjBoneView {
    u8 _0000[0xC4];
    void* field_0xC4; // current bone target (NULL = none)
};

// View of the CfObjectActor fields func_8003E974 reads after dynamic_cast.
struct CfObjectActorView {
    u8 _0000[0x3F10];
    u32 field_0x3F10; // actor/battle id
};

// Battle-event work buffer passed to func_800EC8FC (0x34 bytes, memset to 0
// then partially filled by the plugin).
struct BattleEventWork {
    u32 field_00;     // copied from actor +0x3F10
    u32 field_04;
    u16 field_08;
    u16 field_0A;
    u16 field_0C;     // event id (arg 1)
    u16 field_0E;
    u32 field_10;     // arg 3
    u16 field_14;     // arg 4
    u16 field_16;
    u32 field_18;
    u32 field_1C;
    float field_20;   // arg 2 / 2048.0f
    float field_24;   // arg 5 / 2048.0f
    u32 field_28;
    u32 field_2C;
    u32 field_30;
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
// Retail caller compares the raw register (cmpwi), so declare as int, not u8.
int code80135FDC_getByte_64058();

// VM array element accessor (defined in libs/monolib/src/vm/yvm2.c).
VMArg* vmArrayGet(VMThread* pThread, VMArg* pArray, int index);

// CfGameManager spawn/lookup helpers called by func_800BD7C-style plugins
// (retail mangled symbols; member declarations in CfGameManager.hpp carry
// placeholder signatures).
void* createItemObjectWrapper__Q22cf13CfGameManagerFv(const char* name, u16 index);
void* func_80081A40__Q22cf13CfGameManagerFv(const char* name, u16 index, int flag1, int flag2);
void* createMapObjectInstance__Q22cf13CfGameManagerFv(const char* name, u16 index, int flag1, int flag2);
void* createPlayerEffectInstance__Q22cf13CfGameManagerFv(u32 modelId, u32 motionId);
void* func_8003AA34(void);
const char* getFP__FPCc(const char* name);
unsigned int func_8013EC58();
void* getPlayerContainerForCam__Q22cf13CfGameManagerFv();
bool isPlayerInEventRange__Q22cf13CfGameManagerFv(int arg);

#ifdef __cplusplus
}
#endif



