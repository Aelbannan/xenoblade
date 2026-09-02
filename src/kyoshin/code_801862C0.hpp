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

// CArtsSelectSlot: one (arts id, source ptr) entry.
struct CArtsSelectSlot {
    u32 unk00;  // +0x00: arts id (0 = free slot)
    u32 unk04;  // +0x04: source pointer value
};

// One hash bucket: 46 entries x 8 bytes = 0x170 stride.
class CArtsSelectBucket {
public:
    CArtsSelectSlot slots[46];
};

// Arts-select container: 16 hash buckets of 46 (id, ptr) slots plus a
// fast-path cache pair at 0x1700/0x1704.
class CArtsSelectContainer {
public:
    CArtsSelectBucket buckets[16]; // 0x000..0x16FF
    u32 field_1700;                // cached id
    u32 field_1704;                // cached source pointer
};
class MapProxy;
void* func_801863F4(void* self, void* src);
void* func_80186460(void* dst, void* src);
void* func_80186474(void* self, void* src);
void* func_801864DC(void* pObj, int slot);
void func_80186664(u8* self);
void func_801866F0(MapProxy** objects, int row);
void* func_80186BC8(int p);
void func_80186C7C(void* p);

// cf::CfGameManager sequence/resource counters (retail mangled symbols).
u32 getQueuedFileEventCount__Q22cf13CfGameManagerFv(void);
u32 getResourceFromTable__Q22cf13CfGameManagerFv(u32 resourceId);

#ifdef __cplusplus
}
#endif

// Sync loop (C++ linkage; defined in code_801862C0.cpp).
void func_80186D20(void* p);

// Arts-availability check (retail keeps the unmangled C symbol even though
// the 9-argument form spills the last column pointer to the stack).
extern "C" int func_80186A70(void* p, s32 row, const char* c1, const char* c2,
                             const char* c3, const char* c4, const char* c5,
                             const char* c6, const char* c7);

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" u32 lbl_eu_806642D0; // SDA singleton pointer for the arts select object (opaque pointer)
extern "C" void* memset(void*, int, unsigned long);

// Imported data symbols (other splits) referenced by func_801862C0
// One-time init guard flag in SDA.
extern s8 lbl_eu_806642C8;

// Game-state flag words (CUICfManager .sbss) read by func_80186D20.
extern u32 lbl_eu_80663E24;
extern u32 lbl_eu_80663E28;

// Column-name pool for the map-object spawn table (.rodata, other split).
extern u8 lbl_eu_805038C8[];
// Bdat table pointer singleton (.sbss, set up by the bdat manager).
extern void* lbl_eu_806640B0;

struct Vec3f {
    f32 x;
    f32 y;
    f32 z;
};

#include "monolib/math/CVec3.hpp"

// Map object proxy: real vtable at lbl_eu_8052AC98 (size 0xF0+). Slots at
// 0xA8/0xAC/0xB8/0xBC/0xDC/0x150/0x154 are the map helpers used by
// func_801866F0. Dummy slots are named unkXX to avoid pad pattern
// but the layout matches retail exactly (84 virtuals through 0x154).
class __declspec(novtable) MapProxy {
public:
    virtual void unk08(); virtual void unk0C(); virtual void unk10(); virtual void unk14();
    virtual void unk18(); virtual void unk1C(); virtual void unk20(); virtual void unk24();
    virtual void unk28(); virtual void unk2C(); virtual void unk30(); virtual void unk34();
    virtual void unk38(); virtual void unk3C(); virtual void unk40(); virtual void unk44();
    virtual void unk48(); virtual void unk4C(); virtual void unk50(); virtual void unk54();
    virtual void unk58(); virtual void unk5C(); virtual void unk60(); virtual void unk64();
    virtual void unk68(); virtual void unk6C(); virtual void unk70(); virtual void unk74();
    virtual void unk78(); virtual void unk7C(); virtual void unk80(); virtual void unk84();
    virtual void unk88(); virtual void unk8C(); virtual void unk90(); virtual void unk94();
    virtual void unk98(); virtual void unk9C(); virtual void unkA0(); virtual void unkA4();
    virtual void placeOnGround(const ml::CVec3* pos, float scale); // 0xA8
    virtual ml::CVec3* getPos(); // 0xAC
    virtual void unkB0(); virtual void unkB4();
    virtual void placeInAir(const ml::CVec3* pos, float scale); // 0xB8
    virtual void applyRot(const ml::CVec3* rot); // 0xBC
    virtual void unkC0(); virtual void unkC4(); virtual void unkC8(); virtual void unkCC();
    virtual void unkD0(); virtual void unkD4(); virtual void unkD8();
    virtual void setScale(float scale); // 0xDC
    virtual void unkE0(); virtual void unkE4(); virtual void unkE8(); virtual void unkEC();
    virtual void unkF0(); virtual void unkF4(); virtual void unkF8(); virtual void unkFC();
    virtual void unk100(); virtual void unk104(); virtual void unk108(); virtual void unk10C();
    virtual void unk110(); virtual void unk114(); virtual void unk118(); virtual void unk11C();
    virtual void unk120(); virtual void unk124(); virtual void unk128(); virtual void unk12C();
    virtual void unk130(); virtual void unk134(); virtual void unk138(); virtual void unk13C();
    virtual void unk140(); virtual void unk144(); virtual void unk148(); virtual void unk14C();
    virtual void setVisible(int visible); // 0x150
    virtual void setVisible2(int visible); // 0x154
    virtual void unk158(int flag); // 0x158 - arts entry state

    u8 pad04[0x64]; // 0x04..0x67
    u32 field_68; // 0x68
    u32 field_6C; // 0x6C
};

class CScnItemModel;

// .sdata2 constants used by func_801866F0 (defined in port/data_defs.cpp)
extern const f32 lbl_eu_806679C0;  // 0.0001 position scale
extern const f32 lbl_eu_806679C4;  // 0.01 rotation scale
extern const f32 lbl_eu_806679C8;  // 0.1 scale-column scale
extern const f32 lbl_eu_806679CC;  // 1000.0 place-helper arg
extern const f64 lbl_eu_806679D0;  // signed int-to-double magic
extern const f64 lbl_eu_806679D8;  // unsigned int-to-double magic
extern const f32 lbl_eu_8066A210;  // degrees-to-radians

// Notifier at widget+0x98 is a CScnItemModel (retail vfunc88(int) at
// vtable+0x88). See libs/monolib/src/scn/CScnItemModel.hpp.

// Entry at 0x158 is cf::CfObject::CfObject_UnkVirtualFunc66(int) (0x158).
// Real class is cf::CfObject; entry objects are CfObject instances.
namespace cf { class CfObject; }

// Widget slot viewed via raw offsets to avoid a local pad type.
struct ArtsWidgetView {
    u8 pad_00[0x6C];
    u32 field_6C;
    u8 pad_70[0x28];
    CScnItemModel* field_98;
};

// cf::CfGameManager resource-pair lookup: spawns the map object for
// (modelId, motionId) and returns it (retail mangled symbol).
class Unk80EE4Data;
extern "C" Unk80EE4Data* createPlayerEffectInstance__Q22cf13CfGameManagerFv(u32 modelId, u32 motionId);

// bdat column reader: canonical decl comes from plugin/ocBdat.hpp (above).
// NOTE: do NOT declare func_8003AA34 here - member TUs
// declare it with conflicting signatures (void() vs void(void*)) and an
// extern-"C" overload here breaks every TU that includes both chains.

// Matches the CfGimmick.hpp declaration verbatim so TUs including both see
// one consistent signature.
extern "C" void* func_8003AA34();

// BDAT row base / row count helpers (retail unmangled C symbols; canonical
// definitions live in plugin/ocBdat.cpp).
extern "C" u32 func_8003B41C(void* bdat);
extern "C" u32 func_8003B1EC(void* bdat);

// C++-linkage import: retail symbol is the mangled findObjectById__Fi.
void* findObjectById(int id);
