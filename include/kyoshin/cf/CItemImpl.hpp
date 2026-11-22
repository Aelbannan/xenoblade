#pragma once

// cf::ItemImpl (retail RTTI __RTTI__Q22cf8ItemImpl) and its eight concrete
// item kinds (retail vtables __vt__Q22cf21ItemPurifyKessyouImpl ..
// __vt__Q22cf13ItemDummyImpl, 0xA0 bytes = RTTI + 0 + 38 virtuals, base lists
// from the retail hierarchy objects). Shared by the item owner
// (kyoshin/cf/CItem.hpp) and its UI callers. Slot names are offsets until the
// per-kind overrides are recovered.
#include <types.h>

struct CItemData;
struct CItemRec;

namespace cf {

class ItemImpl {
public:
    virtual u32 vf08(CItemData* p);              // 0x08
    virtual void vf0C(CItemData* p, u32 x);      // 0x0C
    virtual void vf10(CItemData* p);             // 0x10
    virtual void vf14();                         // 0x14
    virtual void vf18();                         // 0x18
    virtual u32 vf1C(CItemData* p);                              // 0x1C
    virtual const char* vf20(CItemData* p);            // 0x20 (name string; retail passes the item in r4, no post-mask)
    virtual void vf24();                                         // 0x24
    virtual u16 vf28(CItemData* p, const char* col);              // 0x28
    virtual CItemRec* vf2C(CItemData* p, u32 x);                 // 0x2C (returns the 8-byte sub-record)
    virtual u16 vf30(CItemData* p);                              // 0x30 (slot count; u16 per getItemCountForGroup pass-through)
    virtual void vf34(CItemData* p, u32 x);      // 0x34
    virtual void vf38();                         // 0x38
    virtual void vf3C(CItemData* p, u32 x);      // 0x3C
    virtual u32 vf40(CItemData* p, u32 x);       // 0x40
    virtual void vf44(CItemData* p, u32 x, u32 y); // 0x44
    virtual void vf48(CItemData* p);             // 0x48
    virtual u32 vf4C(CItemData* p, u32 x);       // 0x4C
    virtual u32 vf50(CItemData* p, u32 x, u16 y); // 0x50
    virtual u32 vf54(CItemData* p);              // 0x54 (retail keeps the full width)
    virtual void vf58(CItemData* p, u32 x);      // 0x58
    virtual void vf5C();                         // 0x5C
    virtual void vf60();                         // 0x60
    virtual u32 vf64(CItemData* p, u32 x);            // 0x64 (slot value; retail passes (item,(u8)i), stb's r3)
    virtual u32 vf68(CItemData* p, u32 x, u16 y); // 0x68
    virtual void vf6C();                         // 0x6C
    virtual void vf70();                         // 0x70
    virtual void vf74();                         // 0x74
    virtual u32 vf78(CItemData* p);              // 0x78
    virtual void vf7C(CItemData* p, u16 x);      // 0x7C
    virtual u32 vf80(CItemData* p);              // 0x80
    virtual u32 vf84(CItemData* p, u32 x);       // 0x84
    virtual void vf88();                         // 0x88
    virtual void vf8C(CItemData* p, u32 x);      // 0x8C
    virtual int vf90(CItemData* p);              // 0x90 (value; full-width int)
    virtual u32 vf94(CItemData* p, u32 x);       // 0x94
    virtual void vf98();                         // 0x98
    virtual void vf9C();                         // 0x9C
};

class ItemDummyImpl : public ItemImpl {};
class ItemConsImpl : public ItemImpl {};
class ItemEquipImpl : public ItemImpl {};
class ItemCrystalImpl : public ItemImpl {};
class ItemDropCrystalImpl : public ItemCrystalImpl {};
class ItemPurifyCrystalImpl : public ItemCrystalImpl {};
class ItemKessyouImpl : public ItemImpl {};
class ItemPurifyKessyouImpl : public ItemKessyouImpl {};

} // namespace cf

// Pre-namespace spelling still used by the item UI callers.
typedef cf::ItemImpl CItemImpl;
