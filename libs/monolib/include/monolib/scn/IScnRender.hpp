#pragma once

// Retail vtable (lbl_eu_80525A00): [~IScnRender, handleEmptyRender].
// handleEmptyRender is the base (empty) implementation of the render-callback
// slot; derived classes override it with their own cbRenderBefore-style
// functions (named cbRenderBefore__<Class>Fv in retail), declared there as
// plain members + extern "C" vtable thunks.
class IScnRender {
public:
    virtual ~IScnRender(){}
    virtual void handleEmptyRender();
};
