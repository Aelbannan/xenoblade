#pragma once

#include <types.h>

#include "kyoshin/IUICf.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CItemBoxGrid.hpp"

class CScn;

/*
 * Menu item-screen process (in-game item menu singleton).
 *
 *   0x00: CProcess head, 0x3C/0x48: CTTask Move/Draw pmf slots
 *   0x58: IScnRender subobject (secondary base)
 *   0x60: CBgTex, 0x80: CTitleAHelp, 0xB8: CItemBoxGrid
 *   0x4AC4-0x4AC6: state / config flag bytes.
 */
class CMenuItem : public IUICf, public IScnRender {
public:
    CMenuItem(CScn* scene, u8 flag5, u8 flag6)
        : mScene(scene), mBgTex(0), mTitleAHelp(0, 0), mItemBoxGrid(0, 0, 0, 1) {
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

    void HandleInput();

    CScn* mScene;                  // 0x5C - owning scene
    CBgTex mBgTex;                 // 0x60 - background layout widget
    CTitleAHelp mTitleAHelp;       // 0x80 - title/help bar
    CItemBoxGrid mItemBoxGrid;     // 0xB8 - item grid
    u8 mState;                     // 0x4AC4
    u8 mField4AC5;                 // 0x4AC5
    u8 mField4AC6;                 // 0x4AC6
    u8 _pad4AC7[1];                // 0x4AC7
};
