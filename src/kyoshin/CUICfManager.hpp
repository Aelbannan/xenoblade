#pragma once

#include "kyoshin/cf/IFlagEvent.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/work.hpp"

#include <nw4r/lyt.h>
#include <types.h>

namespace nw4r {
namespace ut {

// Stub: retail size 0x3C; Destroy returns buffer for MemManager::deallocate.
class PackedFont {
public:
    void* Destroy();

private:
    u8 unk[0x3C];
};

} // namespace ut
} // namespace nw4r

struct CUICfUnk144 {
    u8 unk00[0x39];
    u8 unk39; // 0x39
};

// Queue item type for CUICfManager::Move list walks (CProcess-sized prefix).
struct CUICfMenuItem {
    u8 unk00[0x39];
    u8 unk39; // 0x39 - remove / SetRemove
    u8 unk3A[0x54 - 0x3A];
    u8 unk54; // 0x54
    u8 unk55; // 0x55
};

struct CUICfInitBlock {
    u32 unk00;
    u16 unk04;
    u8 unk06[0x34 - 6];
};

struct CUICfInitState {
    u8 mode;
    u8 state;
    u8 unk02[2];
};

// Retail copy: lwz +0; paired +8/+4; lhz +0xC; lone lwz +0x0E; paired words from +0x12.
// Trailing bytes split so MWCC pair-unrolls (one big u8[] → lwzu). Zeros via u16* overlay.
#pragma pack(push, 1)
struct CUICfInitTailChunk8 {
    u8 b[8];
};
struct CUICfInitTailChunk40 {
    u8 b[0x40];
};
struct CUICfInitTailChunk3E {
    u8 b[0x3E];
};
struct CUICfInitTailChunk40View {
    u8 b[0x40]; // assign view: 2-byte overhang past Tail (retail last lwz pair)
};
struct CUICfInitTail {
    u32 unk00;                  // +0x00
    CUICfInitTailChunk8 mid;    // +0x04..+0x0B (memcpy pair → +8/+4 loads)
    u16 unk0C;                  // +0x0C
    u32 unk0E;                  // +0x0E
    CUICfInitTailChunk40 rest0; // +0x12
    CUICfInitTailChunk3E rest1; // +0x52
}; // size = 0x90
#pragma pack(pop)

// Forces retail stack contiguity: state, block0, blocks[3], tail.
struct CUICfInitTemplates {
    CUICfInitState state;
    CUICfInitBlock block0;
    CUICfInitBlock blocks[3];
    CUICfInitTail tail;
};

// 0xC-byte pool node for func_80133324's event queue - same layout as
// `_reslist_node<u32>` (mNext@0, mPrev@4, mItem@8). Empty slots have mNext==0.
struct CUICfListNode {
    CUICfListNode* next; // 0x0
    CUICfListNode* prev; // 0x4
    u32 item;            // 0x8
};

// 27-entry, 0-terminated id table copied onto the stack by func_80133324
// (retail: sp+0x28..0x5D, matches lbl_eu_804FFFDC minus its trailing entry).
struct CUICfIdTable {
    u16 ids[27];
};

class CUICfManager;

// Retail mangles this as a no-arg CUICfManager member (`Fv`) but the body
// reads r4/r5/r6 as real event-dispatch arguments; declared extern "C" here
// (before the class, so the in-class friend declaration below binds to this
// same linkage) so its ABI is r3=<unused self>, r4=id, r5=a1, r6=a2.
extern "C" void func_80133324__12CUICfManagerFv(CUICfManager* self, int id, int a1, int a2);

struct CUICfInitSlot {
    u8 unk00[4];
    u8 unk04;
    u8 unk05;
    u8 unk06[2];
    CUICfInitBlock unk08;
    CUICfInitBlock unk3C;
    CUICfInitBlock unk70;
    CUICfInitBlock unkA4;
    CUICfInitTail unkD8;
}; // size = 0x168

class CUICfManager : public CTTask<CUICfManager>, public IWorkEvent, public cf::IFlagEvent {
public:
    static CUICfManager* getInstance() {
        return spInstance;
    }
    static CUICfManager* create(CProcess* pParent, CScnNw4r* pScene, mtl::ALLOC_HANDLE mHandle);
    static nw4r::lyt::ArcResourceAccessor* func_801355F4();
    static int func_80135FDC();

    void Init();
    void Term();
    void Move();
    // func_80133324__12CUICfManagerFv is a free function below (retail Fv-mangled
    // member that actually reads r4/r5/r6 event args; see CUICfManager.cpp).
    friend void func_80133324__12CUICfManagerFv(CUICfManager* self, int id, int a1, int a2);

    // Fork helper for presentation gating (coop::ShouldRenderSplitScreen).
    u16 getFlags() const {
        return mFlags;
    }

private:
    // 0x000-0x054 CTTask
    // 0x054-0x058 IWorkEvent
    // 0x058-0x05C cf::IFlagEvent
    nw4r::lyt::ArcResourceAccessor* mArcResourceAccessor; // 0x05C
    nw4r::ut::PackedFont mPackedFont60;                  // 0x060
    nw4r::ut::PackedFont mPackedFont9C;                  // 0x09C
    nw4r::ut::PackedFont mPackedFontD8;                  // 0x0D8
    CFileHandle* mFileHandle;                            // 0x114
    int unk118;                                          // 0x118
    u32 unk11C;                                          // 0x11C
    u32 unk120;                                          // 0x120 - Move countdown
    u8 unk124[0x128 - 0x124];                            // 0x124
    CUICfListNode* unk128;                               // 0x128 (event queue head)
    u8 unk12C[0x138 - 0x12C];                            // 0x12C
    CUICfListNode* unk138;                               // 0x138 (event node array)
    int unk13C;                                          // 0x13C (event node array count)
    u8 unk140[0x144 - 0x140];                            // 0x140
    CUICfUnk144* unk144;                                 // 0x144
    CUICfInitSlot mInitSlots[8];                         // 0x148
    // Slot0.unk00[0]/[1] are manager-wide clear/mark flags (0x148/0x149).
    u8 unkC88[8];                                        // 0xC88
    u16 mFlags;                                          // 0xC90 - Move bitflags
    u8 unkC92[2];                                        // 0xC92

    static CUICfManager* spInstance;
}; // size = 0xC94
