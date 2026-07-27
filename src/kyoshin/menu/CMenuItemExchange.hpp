#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>

/**
 * OC (object-component) interface embedded at offset 0x6c within
 * CMenuItemExchange. Used for virtual dispatch through the OC vtable;
 * thunks adjust back to the containing object before delegating.
 */
struct CMenuItemExchangeOC {
    virtual ~CMenuItemExchangeOC() {}
};

/**
 * Menu item exchange / equipment screen controller.
 *
 * MI layout:
 *   0x00: CProcess
 *   0x58: IScnRender
 *   0x6c: CMenuItemExchangeOC (secondary base for OC dispatch)
 *
 * Full class layout TBD - members will be filled in during decomp.
 */
class CMenuItemExchange : public CProcess, public IScnRender, public CMenuItemExchangeOC {
public:
    CMenuItemExchange();
    virtual ~CMenuItemExchange();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    void cbRenderBefore();
};
