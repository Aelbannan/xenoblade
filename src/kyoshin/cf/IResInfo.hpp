#pragma once

#include <types.h>
#include "monolib/util/FixStr.hpp"

struct CFileHandle; // full definition in monolib/device/CFileHandle.hpp

struct ResEntry {
    u8 _00[4];
    u8 data[4];      // 0x04 - returned by lookup functions
    u32 id;           // 0x08
    u8 _0C[0x2A];    // 0x0C - 0x35
    u8 value;         // 0x36
    u8 _37[5];        // 0x37 - 0x3B
};

struct ResContainer {
    u8 _00[0x14DC];
    ResEntry table[0x28]; // 0x14DC - 40 entries

    u8* findResEntry(u32 id, u32* outIndex, u32* outValue);
};

// Object referenced by ResInfoEntry::field_0x2C. Exact class identity unknown
// (set externally by the resource manager); only the vtable slots this unit
// calls are declared. With -RTTI on, MWCC adds 2 leading vtable slots, so the
// first declared virtual lands at vtable offset +8 (matching retail).
class CResLookup {
public:
    virtual void* getResourceBase(void* entry, int arg); // vtable +8
    virtual u32 getFlags();                              // vtable +0xC - state flags (masked with 0x18E / 0x10000)
    virtual ~CResLookup();                               // vtable +0x10 (unused)
    virtual void* vfunc01(void* entry);                  // vtable +0x14
    virtual u8* getResHandle18(void* entry);             // vtable +0x18
    virtual u8* getResHandle1C(void* entry);             // vtable +0x1C
    virtual void* vfunc04(void* entry);                  // vtable +0x20
    virtual void* vfunc05(void* entry);                  // vtable +0x24
    virtual int isResActive28(void* entry);              // vtable +0x28
    virtual void* vfunc07(void* entry);                  // vtable +0x2C
    virtual u8* getResHandle30(void* entry);             // vtable +0x30
    virtual void* vfunc09(void* entry);                // vtable +0x34 (outer dispatch)
    virtual void* vfunc0A(void* entry);                // vtable +0x38 (outer dispatch, widened)
    virtual void* vfunc0B(void* entry);                  // vtable +0x3C - resolution probe (1 arg)
    virtual int isInUse(void* entry);                      // vtable +0x40
    virtual int cmpField4Eq(void* entry, u32 val);       // vtable +0x44 - was m17 (field_04 compare)
    virtual int checkFlag48(void* entry);                // vtable +0x48 - was m16 / func_80068998
    virtual int checkFlag4C(void* entry);                // vtable +0x4C - was isResFlag0 / func_800661A8
    virtual void cleanup50(void* entry);                 // vtable +0x50 - was func_800665A0
    // Wrappers keep call sites stable after _v rename
    inline u8* getHandle18(void* e) { return getResHandle18(e); }
    inline u8* getHandle1C(void* e) { return getResHandle1C(e); }
    inline int isActive28(void* e) { return isResActive28(e); }
    inline u8* getHandle30(void* e) { return getResHandle30(e); }
};

// 0x3C-byte resource record operated on by the IResInfo accessors.
// Only the fields the accessors touch are declared.
struct ResInfoEntry {
    u32 field_0x00;             // 0x00 - flags
    u32 field_0x04;             // 0x04
    u32* data;                  // 0x08 - data pointer (passed to func_80062430)
    u32 field_0x0C;             // 0x0C - 0x0F
    u32 field_0x10;             // 0x10
    u8 field_0x14[4];           // 0x14 - 0x17
    u32 field_0x18;             // 0x18 - data word (returned by lookup accessors)
    u32 field_0x1C;             // 0x1C
    u32 field_0x20;             // 0x20
    u32 field_0x24;             // 0x24
    CFileHandle* field_0x28;    // 0x28 - device file handle (CDeviceFile::cancel)
    CResLookup* field_0x2C;     // 0x2C - resource object (virtual lookup)
    u16 field_0x30;             // 0x30 - logical id (lhz)
    u8 field_0x32;              // 0x32 - subtype
    u8 field_0x33;              // 0x33 - type
    s16 field_0x34;             // 0x34 - index
    s16 field_0x36;             // 0x36 - fade counter (lha/sth)
    u16 field_0x38;             // 0x38
    u16 field_0x3A;             // 0x3A
};

// Doubly-linked list node used by the reslist-style container at 0x1EB0
// (next at +0, prev at +4, 4-byte item at +8 -> node stride 0xC).
struct ResInfoListNode {
    ResInfoListNode* next;
    ResInfoListNode* prev;
    u32 item;
};

// Embedded reslist-style container (mirrors _reslist_base<T> layout; the
// vtable slot at +0x1EB0 is not touched by this unit).
struct ResInfoStorage {
    u8 _00[0x1EB0];
    void* vtable;               // 0x1EB0 - reslist vtable (unused here)
    ResInfoListNode* mStartNodePtr; // 0x1EB4 - &mStartNode
    ResInfoListNode mStartNode;     // 0x1EB8 - sentinel node
    ResInfoListNode* mList;         // 0x1EC4 - slot array (freed on destroy)
    int mCapacity;                  // 0x1EC8
    u8 mOwnsList;                   // 0x1ECC - 0 => slot array is owned
};

// 0x3C-byte record initialized by func_800676F8 (the tables at 0x2D0,
// 0x14DC and 0x16BC share this shape).
struct ResInitEntry {
    u8 _00[0x30];
    ResInfoListNode* field_0x30; // reslist sentinel node or scratch lookup slot
    s16 field_0x34;              // logical index
    u8 field_0x36;               // column (n % 11 for the 0x2D0 grid)
    u8 field_0x37;               // row (n / 11 for the 0x2D0 grid)
    u8 _38[4];
};

// Object layout driven by func_800676F8.
struct ResInfoWork {
    u32 flags;                    // 0x00
    u8 _04[0x74];
    ResInitEntry preTable[10];    // 0x78 - sound categories 2..11
    ResInitEntry entries[0x4D];   // 0x2D0 - logical indices 0xC..0x58
    ResInitEntry gridLow[8];      // 0x14DC - ids 0x59..0x60
    ResInitEntry gridHigh[0x20];  // 0x16BC - ids 0x61..0x80
    u8 _1E3C[0x40];
    u32 slots[14];                // 0x1E7C - scratch lookup slots
    ResInfoListNode* mStartNodePtr; // 0x1EB4
    ResInfoListNode mStartNode;     // 0x1EB8
    u8 _1EC4[0xC];
    float counter;                // 0x1ED0
};

// 0x3C-byte record cleared by the container constructor (__ct__80066F9C).
// Only the fields the constructor resets are declared; the gaps keep the
// 0x3C stride.
struct ResCtorEntry {
    u32 field_0x00;
    u32 field_0x04;
    u32 field_0x08;
    u8 _0C[4];
    u32 field_0x10;
    u8 _14[4];
    u32 field_0x18;
    u32 field_0x1C;
    u8 _20[4];
    u32 field_0x24;
    u32 field_0x28;
    u8 _2C[8];
    s16 field_0x34;
    u8 _36[2];
    u16 field_0x38;
    u16 field_0x3A;
};

// Constructor view of the IResInfo container - see ResCtorLayout below.
struct ResCtorLayout;

// reslist<unsigned short> / _reslist_base<unsigned short> layout mirror
// (monolib util/reslist.hpp). The retail dtor symbols use the old flat
// template mangling, so the deleting destructors are plain global functions
// over this layout (twin of ResListIUIBattle in CUIBattleManager.hpp).
struct ResListUSNode {
    ResListUSNode* next;    // 0x0
    ResListUSNode* prev;    // 0x4
    u16 item;               // 0x8
};

struct ResListUS {
    void* vtable;               // 0x0 - lbl_eu_80526938
    ResListUSNode* mHead;       // 0x4 - &mStartNode
    ResListUSNode mStartNode;   // 0x8 - sentinel node
    ResListUSNode* mList;       // 0x14 - slot array (freed on destroy)
    int mCapacity;              // 0x18
    u8 mOwnsList;               // 0x1C - 0 => slot array is owned
};

// Constructor view of the IResInfo container: flag word, then 130
// 0x3C-stride records from +0x04 up to the scratch slot table.
struct ResCtorLayout {
    u32 flags;                    // 0x00
    ResCtorEntry entries[0x82];   // 0x04 - 0x1E7C
    void* slots[13];              // 0x1E7C - 0x1EB0 - per-category vtables
    void* vtable;                 // 0x1EB0 - embedded reslist vtable
    ResInfoListNode* mStartNodePtr; // 0x1EB4
    ResInfoListNode mStartNode;   // 0x1EB8 - sentinel
    ResListUSNode* mList;         // 0x1EC4 - node pool (allocated here)
    int mCapacity;                // 0x1EC8
    u8 mOwnsList;                 // 0x1ECC
};

// 0x3C-byte entry in the resource grid tables at 0x14DC / 0x16BC (indices
// 0x59..0x80). The resource object (virtual getResourceBase) sits at +0x30;
// the cancel handle at +0x2C.
struct ResGridEntry {
    u32 field_0x00;         // 0x00
    u32 field_0x04;         // 0x04 - data word (may carry flag bits)
    u32 id;                 // 0x08
    u8 _0C[4];              // 0x0C - 0x0F
    u32 field_0x10;         // 0x10
    u8 _14[0x18];           // 0x14 - 0x2B
    CFileHandle* handle;    // 0x2C - cancel handle (CDeviceFile::cancel)
    CResLookup* lookup;     // 0x30 - resource object (virtual lookup)
    u8 _34[2];              // 0x34 - 0x35
    u8 value;               // 0x36
    u8 _37[3];              // 0x37 - 0x39
    u16 field_0x3A;         // 0x3A - 0x3B
};

// 0x40-byte view of a 0x3C-stride grid record: the s16 state flags at
// +0x3C/+0x3E (dirty/availability bits read by the update walks) physically
// overlap the following record's first 4 bytes. Only used for field access;
// never iterated (the grids stride by 0x3C).
struct ResGridEntryEx {
    ResGridEntry entry;
    s16 field_0x3C;         // 0x3C
    s16 field_0x3E;         // 0x3E
};

// The IResInfo resource container: two overlapping 0x3C-stride grids at
// 0x14DC (indices 0x59..0x80, 40 records) and 0x16BC (indices 0x61..0x80,
// 32 records sharing grid[8..]), plus a per-container float state at 0x1ED0.
struct ResInfoContainer {
    u8 _00[0x14DC];
    ResGridEntry grid[0x28];  // 0x14DC - 0x1E3C (logical indices 0x59..0x80)
    u8 _1E3C[0x94];           // 0x1E3C - 0x1ED0
    float field_0x1ED0;       // 0x1ED0 - fade/state counter
};

// Global cf flag words owned by CfGameManagerData / CUICfManager.
extern u32 lbl_eu_80663E30;

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" char* getEntryPtrGrid(char* self, int a, int b);
extern "C" void clearWordFlag(u32* self, u32 mask);
extern "C" void* func_80068494(void*, u32, u32*, u32*, int, int);
extern "C" bool func_800686E4(ResInfoContainer*, bool);

extern "C" bool testResInfoFlag(u32 flags);

// Packed-token formatter (kyoshin/code_800AA008.cpp) and sound-manager
// slot helpers (kyoshin/cf/CfSoundMan.cpp) - retail C symbol names.
extern "C" int func_800AA33C(ml::FixStr<64>& buf, u32 packed, int prefixFlag, int suffixFlag);
extern "C" int func_801BFA64(int);
extern "C" void func_801BFA08(int, void*, int, int);
extern "C" void func_801BFA88(int, int, int, int);