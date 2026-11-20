#pragma once

#include <types.h>

class CfMapItemLoader;

// Real C++ entry points defined in this TU (retail-mangled names).
void MapItemStoreBytes567(void* self, u8 a, u8 b, u8 c);
void requestItemLoad(CfMapItemLoader* self, u32 id);

// Owner module header for kyoshin/cf/CfMapItemManager.cpp
// flag/status helpers). These are the single unified declarations; TUs that
// call them include this header.

// Battle-status word probe: tests the actor's status flags against `id`
// (raw bitmask window outside [0x800,0x807], per-category dispatch inside).
// `outFlags` points at the actor's u32 flag word.
extern "C" s32 testActorStatus(void* obj, void* outFlags, u32 id);

// Battle-status flag setter (clear/mask write on the actor's status word).
extern "C" void dispatchItemLoad(void* actor, u32 flags);

namespace ml {
struct CVec3;
}

struct CfMapItemPos {
    f32 x, y, z;
};

// One map-item record; the manager's item array is 0x1C-byte strided.
// Layout from the retail ctor: each record gets stws for handle/position,
// an stfs for the +0x10 scale and an sth for the +0x14 kind.
struct CfMapItem {
    union {
        u32 handle;                // 0x00: hikari scene-object handle
        struct {
            s16 field_00;          // 0x00 low half (timer notch funcs)
            s16 field_02;          // 0x02 type selector (1..3), indexes data[]
        };
    };
    union {
        u8 data[12];               // 0x04 raw bytes over the position
        CfMapItemPos pos;          // 0x04 world position
        u32 field_04;              // 0x04 raw word view
    };
    f32 field_10_f;                // 0x10 scale / respawn timer
    u16 field_14;                  // 0x14 kind (0 = timer-only slot)
    s16 field_16;                  // 0x16 respawn countdown
    u32 field_18;                  // 0x18 state flags (row in top 12 bits)

    CfMapItem() : field_10_f(0.0f), field_14(0) {}
};

namespace cf {

// Map-item manager (RTTI "cf::CfMapItemManager", no bases). Two instances
// live on the game-manager singleton: the map-item manager (+0xCA4) and the
// cf::CfValueItemManager subclass (+0xCA8). The destructor is not virtual
// (the vtables have no destructor slot); the singleton deletes both through
// CfMapItemManager pointers. Retail allocates 0x3808 bytes per manager.
class CfMapItemManager {
public:
    CfMapItemManager();
    ~CfMapItemManager();

    virtual void collectItems();                    // +0x08
    virtual int isEventActive();                    // +0x0C
    virtual int placeItem(u32 row, void* item);     // +0x10 lottery + place
    virtual u32 getItemTable();                     // +0x14 bdat table handle

    void clearMapItems();
    int placeMapItemPick(u32 row, void* item, int pick);
    void setItemEnable(void* arg);
    int updateMapItems(ml::CVec3* pos);
    void tryPickupMapItem();

    /* 0x0004 */ CfMapItem mItems[512];
    /* 0x3804 */ u16 mCount;        // 1-based
    /* 0x3806 */ u16 field_3806;    // active-item count
};

// Value-item manager (RTTI "cf::CfValueItemManager", base cf::CfMapItemManager):
// overrides all four virtuals. Its ctor is out of line in code_800B06A4.cpp,
// right after buildSingleton, the only place that constructs one.
class CfValueItemManager : public CfMapItemManager {
public:
    CfValueItemManager();

    virtual void collectItems();
    virtual int isEventActive();
    virtual int placeItem(u32 row, void* item);
    virtual u32 getItemTable();
};

} // namespace cf
