#pragma once

// Retail IScnRender vtable (__vt__10IScnRender, 0x10 bytes, owned by
// kyoshin/CTaskGame.cpp): [RTTI, 0, ~IScnRender, cbRenderBefore]. Its only
// virtuals are the destructor and cbRenderBefore; slot 1's out-of-line empty
// body is the retail 4-byte `blr` at 0x800444C0.
//
// The destructor has three spellings, all used by real retail TUs:
//   default                inline-empty, so a derived destructor elides the
//                          empty base-dtor call (retail shape everywhere);
//                          MWCC also emits the usual link-deduped weak copy.
//   ISCN_RENDER_NO_DTOR    no virtual destructor at all. TUs whose retail
//                          object fills the IScnRender dtor slot of a
//                          hand-built sub-vtable use this (CTaskGame,
//                          CTaskGameEff, CMenuFade, CMiniMap).
//   ISCN_RENDER_OUT_OF_LINE_DTOR
//                          declaration only: the derived destructor calls
//                          __dt__10IScnRenderFv out of line, which is what
//                          retail's CMenuUpdate destructor does.
class IScnRender {
public:
#if defined(ISCN_RENDER_NO_DTOR)
    // no destructor declared on purpose (see above)
#elif defined(ISCN_RENDER_OUT_OF_LINE_DTOR)
    virtual ~IScnRender();
#else
    virtual ~IScnRender() {}
#endif
    virtual void cbRenderBefore();
};
