#pragma once

// Owner header for monolib/src/effect/code_804D854C.cpp.  The concrete
// CDrawCtx / TexSrc layouts live in the defining and calling TUs; these
// helpers only pass pointers to them.
struct CDrawCtx;
struct TexSrc;

// Real C++ entry points defined in code_804D854C.cpp (retail mangles them).
void pushEffectFog(int type, int flag);
void releaseEffectDrawTex(CDrawCtx* self);
int setupEffectDrawCtx(CDrawCtx* draw, TexSrc* desktop, void* material);
