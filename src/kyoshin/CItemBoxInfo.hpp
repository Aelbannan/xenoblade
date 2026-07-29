#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"

namespace nw4r { namespace lyt { class DrawInfo; } }

struct CItemBoxInfoState {
    u8 _00[0x04];
    UnkClass_8045F564 memRegion1;   // 0x04
    UnkClass_8045F564 memRegion2;   // 0x14
    void* fileHandle1;              // 0x24 - CFileHandle
    void* fileHandle2;              // 0x28 - CFileHandle
    u8 _2C[0x04];
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
    void OnFileEvent();

    u8 getItemBoxState();
    void advanceItemBoxState();
    void setItemBoxIndex(unsigned char index, short value);
    void tryActivateItemBox();
};

struct CItemBoxInfo2 {
    CItemBoxInfoState state;
    ~CItemBoxInfo2();
    void OnFileEvent();

    void drawItemBox2Layout(nw4r::lyt::DrawInfo* drawInfo);
    u8 getItemBox2State();
    void advanceItemBox2State();
};

struct CItemBoxInfoEntry {
    u16 itemId;
    u16 _02;
    u32 value;
    u8 state;

    void setItemBoxEntry(u16 r4, u32 r5, u8 r6);
    void setItemBoxCopy(unsigned short a, unsigned int b, unsigned char c);
};

typedef CItemBoxInfoEntry ItemBoxInfoCopy;
