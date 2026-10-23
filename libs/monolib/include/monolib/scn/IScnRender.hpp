#pragma once

// Retail: IScnRender carries virtual cbRenderBefore (vtable lbl_eu_80525A00 slot 1)
// plus legacy non-virtual empty handleEmptyRender (0x800444C0, blr; defined in
// kyoshin/CTaskGame.cpp). Adjustor thunks take the overridden virtual's name.
//
// ISCN_RENDER_NO_DTOR: define it before including this header (at the top of the
// .cpp, like IWORK_EVENT_INLINE_DTOR) in TUs whose retail objects never odr-use
// ~IScnRender. Without the virtual dtor, MWCC neither emits a standalone
// __dt__10IScnRenderFv copy (0x40, retail keeps it only in CTaskGame.o) nor an
// extra dtor adjustor thunk, and derived dtors lose the base-dtor null check.
// Those TUs fill the dtor slot of the retail sub-vtable with a manual thunk.
class IScnRender {
public:
#ifndef ISCN_RENDER_NO_DTOR
    virtual ~IScnRender(){}
#endif
    virtual void cbRenderBefore();
    void handleEmptyRender(); // non-virtual legacy empty impl (retail 0x800444C0)
};
