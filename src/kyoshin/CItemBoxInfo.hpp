#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"

class CEventFile;

namespace nw4r { namespace lyt { class DrawInfo; } }

struct CItemBoxInfoState {
    u8 _00[0x04];
    UnkClass_8045F564 memRegion1;   // 0x04
    UnkClass_8045F564 memRegion2;   // 0x14
    void* fileHandle1;              // 0x24 - CFileHandle
    void* fileHandle2;              // 0x28 - CFileHandle
    void* arcResourceAccessor;     // 0x2C - nw4r::lyt::ArcResourceAccessor
    void* resource;
    void* layout;
    void* animTransform1;   // 0x38 - nw4r::lyt::AnimTransform
    void* animTransform2;   // 0x3C - nw4r::lyt::AnimTransform
    u8 _40[0x50];
    u8 active;
    u8 _91[3];
    s32 state;
    u8 current;
    u8 visible;
    u8 _9A[0x26];
    s16 values[12];
};

struct CItemBoxInfo {
    CItemBoxInfoState state;
    ~CItemBoxInfo();
    bool OnFileEvent(CEventFile*);

    u8 getItemBoxState();
    void advanceItemBoxState();
    void setItemBoxIndex(unsigned char index, short value);
};

// Free-function helper (retail: tryActivateItemBox__FP12CItemBoxInfo)
void tryActivateItemBox(CItemBoxInfo* info);

// Recovered free functions (retail: getItemBoxState__FP12CItemBoxInfo)
u8 getItemBoxState(CItemBoxInfo* self);

struct CItemBoxInfo2 {
    CItemBoxInfoState state;
    ~CItemBoxInfo2();
    bool OnFileEvent(CEventFile*);

    void drawItemBox2Layout(nw4r::lyt::DrawInfo* drawInfo);
    void advanceItemBox2State();
};

u8 getItemBox2State(CItemBoxInfo2* self);

struct CItemBoxInfoEntry {
    u16 itemId;
    u16 _02;
    u32 value;
    u8 state;

};

// 8-byte color (4 s16 channels) used by the item-box layout renderer.
struct CItemBoxQuadColor {
    s16 r, g, b, a;
    CItemBoxQuadColor() : r(0), g(0), b(0), a(0) {}
    CItemBoxQuadColor(s16 _r, s16 _g, s16 _b, s16 _a) : r(_r), g(_g), b(_b), a(_a) {}
};

// 4 selection/vertex colors (0x20 bytes) built per-slot in func_801E2928.
struct CItemBoxQuad {
    CItemBoxQuadColor col[4];
};

// 6-byte slot table: a u32 (bytes 0..3) + u16 (bytes 4..5), read from two
// consecutive .sdata2 constants (lbl_eu_8066805C + 80668060, etc.).
union CItemBoxSlotBytes {
    struct { u32 a; u16 b; } ab;
    u8 bytes[6];
};

