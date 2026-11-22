#pragma once

#include <types.h>
#include "kyoshin/IUICf.hpp"
#include "monolib/scn/IScnRender.hpp"

#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CItemBoxGrid.hpp"

class CProcess;
class CScn;

/*
 * Shop sell menu screen process (singleton, built by create()).
 *
 * Layout (retail offsets):
 *   0x00: IUICf task base (CTTask<IUICf>, mField54/mField55 at 0x54/0x55)
 *   0x58: IScnRender render-callback subobject (sub-vtable)
 *   0x5C: CScn* mScene -- owning scene (removeRenderCB target in Term)
 *   0x60: CBgTex mBgTex -- background layout widget (0x20 bytes)
 *   0x80: CTitleAHelp mTitleAHelp -- title/help bar (0x38 bytes)
 *   0xB8: CItemBoxGrid mItemBoxGrid -- item-box grid widget
 *  0x4AC4: u8 mState -- phase/state indicator (HandlePhase1)
 */
class CMenuShopSell : public IUICf, public IScnRender {
public:
    CMenuShopSell(CScn* scene);
    virtual ~CMenuShopSell();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    void cbRenderBefore(CScn* scene);

    static CMenuShopSell* create(CProcess* parent, CScn* scene);
    static CMenuShopSell* sInstance;

    void HandlePhase0();
    void HandlePhase1();
    void HandlePhase2();
    void HandlePhase3();

    CScn* mScene;                    // 0x5C
    CBgTex mBgTex;                   // 0x60
    CTitleAHelp mTitleAHelp;         // 0x80
    CItemBoxGrid mItemBoxGrid;       // 0xB8
    u8 mState;                       // 0x4AC4
};

extern "C" void MenuShopSellCopyData(void*, void*);
extern "C" bool MenuShopSellIsCreated();
