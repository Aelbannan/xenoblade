#pragma once

// Retail vtable (lbl_eu_80525A00): [~IScnRender, cbRenderBefore]. The old
// handleEmptyRender spelling was a misnomer for the render-callback slot:
// MWCC names adjustor thunks after the overridden virtual, and only an
// exact name match overrides (a differently-named member gets no thunk).
// See the thunk-recovery pilot note (docs/evidence/decomp, 2026-10-18).
class IScnRender {
public:
    virtual ~IScnRender(){}
    virtual void cbRenderBefore();
};
