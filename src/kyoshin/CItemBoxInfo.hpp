#pragma once

#include <types.h>

namespace nw4r { namespace lyt { class DrawInfo; } }

struct CItemBoxInfoState {
    u8 _00[0x30];
    void* resource;
    void* layout;
    u8 _38[0x52];
    u8 active;
    u8 _8B[5];
    u32 state;
    u8 current;
    u8 visible;
    u8 _9A[0x26];
    s16 values[12];
};

struct CItemBoxInfo {
    CItemBoxInfoState state;
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
