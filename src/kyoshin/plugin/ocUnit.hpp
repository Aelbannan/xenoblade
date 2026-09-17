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
// buffer by execNpcAction before indexing.
extern const u32 lbl_eu_804FA4C0[6];

// .sdata2 float constant passed by chkEventRange to func_800AB580.
extern const float lbl_eu_80665D44;

// Defined in kyoshin/cf/object/CfObjectColl.cpp (declaration kept here so
// plugin call sites see one consistent signature).
extern "C" int func_800AB580(cf::CfObjectColl* self, cf::CfObject* obj, ml::CVec3* out, float f1);

// Moves an object to pos with the given angle, relative to its move object.
// Also defined in CfObjectColl.cpp.
extern "C" void func_800ABFC4(cf::CfObject* obj, void* moveObj, ml::CVec3* pos, float angle);

// Fake vtables removed - all slots folded onto owning classes
// cf::CfObject / cf::CfObjectMove / cf::CActorParam etc. See headers for real virtuals.

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
// func_8003BD7C (bdat-name dispatch): CfObject::unk64 (0x64) and
// CfObject::mFlags68 (0x68), spelled through the real class now.

// OC handle words read straight off the lookup result (+0x04) and the
// object bone-target word (+0xC4): raw offsets now, no view structs.
// The actor word at +0x3F10 is likewise read raw at the use site.

// Battle-event work buffer passed to CBattleMan_RunBattleEvent (0x34 bytes, memset to 0
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
void CfObjectMove_attachMoveRegion(cf::CfObject* obj);
void* CfObjectMove_getSelfIfActive(void* objParam);
void UIWin_CreateTalkWin(void* obj, const char* str, int mode);
void UIWin_CreateEveTalkWin(void* obj, const char* str);
unsigned int UIWin_GetTimer();
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
unsigned int UIWin_GetTimer();
void* getPlayerContainerForCam__Q22cf13CfGameManagerFv();
bool isPlayerInEventRange__Q22cf13CfGameManagerFv(int arg);

#ifdef __cplusplus
}
#endif



