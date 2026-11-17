#pragma once

#include <types.h>

#include "kyoshin/IUICf.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"

class CScn;
class CItemBoxGrid;

// The 4-argument CItemBoxGrid constructor (retail unmangled name).
extern "C" void __ct__CItemBoxGrid(CItemBoxGrid* self, u32 arg1, u32 arg2,
                                   u32 arg3, u32 arg4);

/*
 * Menu item-screen process (in-game item menu singleton).
 *
 *   0x00: CProcess head, 0x3C/0x48: CTTask Move/Draw pmf slots
 *   0x58: IScnRender subobject (secondary base)
 *   0x60: CBgTex, 0x80: CTitleAHelp, 0xB8: CItemBoxGrid (opaque)
 *   0x4AC4-0x4AC6: state / config flag bytes.
 */
class CMenuItem : public IUICf, public IScnRender {
public:
    CMenuItem(CScn* scene, u8 flag5, u8 flag6)
        : mScene(scene), mBgTex(0), mTitleAHelp(0, 0) {
        // mItemBoxGrid is opaque storage; its constructor is called by the
        // C-ABI name (the 4-arg form has no C++ declaration).
        __ct__CItemBoxGrid(grid(), 0, 0, 0, 1);
        mState = 0;
        mField4AC5 = flag5;
        mField4AC6 = flag6;
    }
    virtual ~CMenuItem();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    void cbRenderBefore();

    static CMenuItem* create(CProcess* parent, CScn* scene, u8 flag5, u8 flag6);

    static CMenuItem* sInstance;

    // CItemBoxGrid is not a complete class yet (kyoshin/CItemBoxGrid.cpp is
    // still built from C-style pieces), so the grid is opaque storage here.
    CItemBoxGrid* grid() { return reinterpret_cast<CItemBoxGrid*>(mItemBoxGrid); }

    CScn* mScene;                  // 0x5C - owning scene
    CBgTex mBgTex;                 // 0x60 - background layout widget
    CTitleAHelp mTitleAHelp;       // 0x80 - title/help bar
    u8 mItemBoxGrid[0x4AC4 - 0xB8]; // 0xB8..0x4AC3 - item grid (opaque storage)
    u8 mState;                     // 0x4AC4
    u8 mField4AC5;                 // 0x4AC5
    u8 mField4AC6;                 // 0x4AC6
    u8 _pad4AC7[1];                // 0x4AC7
};
