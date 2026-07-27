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

class CItemBoxGrid {
public:
    CItemBoxGrid();
    virtual ~CItemBoxGrid();
    void OnFileEvent();

    // TODO: add fields
};

