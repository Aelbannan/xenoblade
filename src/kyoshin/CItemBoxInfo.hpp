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

