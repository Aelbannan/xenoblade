#pragma once

/// Menu save/load screen process.
/// Full class layout TBD - forward declaration sufficient for adjusting thunks.
class CMenuSave;

class CMenuSave {
public:
    CMenuSave();
    virtual ~CMenuSave();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

