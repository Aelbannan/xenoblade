#pragma once

// Retail: IScnRender carries virtual cbRenderBefore (vtable lbl_eu_80525A00 slot 1)
// plus legacy non-virtual empty handleEmptyRender (0x800444C0, blr; defined in
// kyoshin/CTaskGame.cpp). Adjustor thunks take the overridden virtual's name.
class IScnRender {
public:
    virtual ~IScnRender(){}
    virtual void cbRenderBefore();
    void handleEmptyRender(); // non-virtual legacy empty impl (retail 0x800444C0)
};
