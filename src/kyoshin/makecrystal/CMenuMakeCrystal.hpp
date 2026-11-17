#pragma once

#include <types.h>

#include "kyoshin/IUICf.hpp"
#include "kyoshin/CBgTex.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/work/CProcess.hpp"

class CScn;

// Opaque view of the crafting window (the full class lives in
// makecrystal/CMakeCrystalWin.hpp, which cannot be co-included here yet
// because of its flat import declarations). Only the size and the
// out-of-line destructor matter to the menu.
class CMakeCrystalWin {
public:
    ~CMakeCrystalWin();
    int mkCrystalIsBusyState();

private:
    u32 mStorage[0x4364 / 4];
};

// Main menu UI for the crystal crafting system.
// Common IUICf task base at 0x00; IScnRender subobject at 0x58 (requires this-adjusting thunks).
class CMenuMakeCrystal : public IUICf, public IScnRender {
public:
    CMenuMakeCrystal(CScn* scene);
    virtual ~CMenuMakeCrystal();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();

    static CMenuMakeCrystal* create(CProcess* parent, CScn* scene);

    static CMenuMakeCrystal* sInstance;

    /* 0x5C */ CScn* mScene;
    /* 0x60 */ CBgTex mBgTex;
    /* 0x80 */ CMakeCrystalWin mCrystalWin;
    /* 0x43E4 */ u8 mField43E4;
    /* 0x43E5 */ u8 mField43E5;
}; // size: 0x43E8
