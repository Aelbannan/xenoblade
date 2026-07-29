#pragma once

#include <types.h>

class CQuestItem {
public:
    virtual ~CQuestItem();

    // TODO: add fields
};

class CVisionItem {
public:
    virtual ~CVisionItem();

    // TODO: add fields
};

class CArtsBookItem {
public:
    CArtsBookItem();
    virtual ~CArtsBookItem();

    // TODO: add fields
};

// Full object layout for CItemBoxGrid (used by C-linkage accessors)
struct CItemBoxGridFull {
    u8 _00[0x61];
    u8 field_61;
    u8 _62[0x52D - 0x62];
    u8 field_52D;
    u8 _52E[0x549 - 0x52E];
    u8 field_549;
    u8 _54A[0x2800 - 0x54A];
    u16 field_2800;
    u8 field_2802;
    u8 field_2803;
    u8 field_2804;
};

class CItemBoxGrid {
public:
    CItemBoxGrid();
    virtual ~CItemBoxGrid();
    void OnFileEvent();

    u8 GetField61();
    u8 GetField549();
    u8 GetField52D();
    void PushToList(unsigned char val);

    // TODO: add fields
};

