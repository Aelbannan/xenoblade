#pragma once

#include <types.h>
#include "kyoshin/IUICf.hpp"
#include "monolib/scn/IScnRender.hpp"

#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"

class CProcess;
class CScn;
class CItemBoxGrid;

/*
 * Shop sell menu screen process (singleton, built by create()).
 *
 * Layout (retail offsets):
 *   0x00: IUICf task base (CTTask<IUICf>, mField54/mField55 at 0x54/0x55)
 *   0x58: IScnRender render-callback subobject (sub-vtable)
 *   0x5C: CScn* mScene -- owning scene (removeRenderCB target in Term)
 *   0x60: CBgTex mBgTex -- background layout widget (0x20 bytes)
 *   0x80: CTitleAHelp mTitleAHelp -- title/help bar (0x38 bytes)
 *   0xB8: CItemBoxGrid storage (raw; built and destroyed through its C-ABI ctor/dtor)
 *  0x4AC4: u8 mState -- phase/state indicator (MenuShopSellHandlePhase1)
 */
class CMenuShopSell : public IUICf, public IScnRender {
public:
    CMenuShopSell(CScn* scene);
    virtual ~CMenuShopSell();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    void cbRenderBefore();

    static CMenuShopSell* create(CProcess* parent, CScn* scene);
    static CMenuShopSell* sInstance;

    CItemBoxGrid* itemBoxGrid() { return reinterpret_cast<CItemBoxGrid*>(mItemBoxGrid); }

    CScn* mScene;                    // 0x5C
    CBgTex mBgTex;                   // 0x60
    CTitleAHelp mTitleAHelp;         // 0x80
    u8 mItemBoxGrid[0x4AC4 - 0xB8];  // 0xB8: CItemBoxGrid storage
    u8 mState;                       // 0x4AC4
};

extern "C" void MenuShopSellCopyCursor(void*, void*);
extern "C" void MenuShopSellCopyData(void*, void*);
extern "C" bool MenuShopSellIsCreated();
extern "C" __declspec(noinline) void MenuShopSellHandlePhase0(CMenuShopSell* self);
extern "C" __declspec(noinline) void MenuShopSellHandlePhase1(CMenuShopSell* self);
extern "C" __declspec(noinline) void MenuShopSellHandlePhase2(CMenuShopSell* self);
extern "C" __declspec(noinline) void MenuShopSellHandlePhase3(CMenuShopSell* self);
