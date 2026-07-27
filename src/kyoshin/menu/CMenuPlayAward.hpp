#pragma once

#include <types.h>

class CMenuPlayAward {
public:
    CMenuPlayAward();
    virtual ~CMenuPlayAward();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // Partial field layout (recovered from accessor thunks)
    u8 _pad00[0x8B];               // 0x00-0x8A
    u8 mField_8B;                  // 0x8B: some state/flag byte
    u8 _pad8C[0x100A - 0x8C];      // 0x8C-0x1009
    u8 mPageIndex;                 // 0x100A: page index for entry lookup (shifted << 11)
    // TODO: more fields beyond 0x100B
};

class CPlayAwardList {
public:
    CPlayAwardList();
    virtual ~CPlayAwardList();
    void OnFileEvent();

    // TODO: add fields
};

