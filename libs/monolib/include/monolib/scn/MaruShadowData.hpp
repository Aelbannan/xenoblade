#pragma once

#include <types.h>

// Shared shadow-map scratch emitted by the scene data TUs.
// name-guess: world-space centre written by MaruShadowDrawQuad from the camera
// render record and read by MaruShadowStreamVerts as the UV origin.
extern f32 sMaruShadowCenter[4];
// name-guess: reciprocal shadow-map extent written by MaruShadowDrawQuad and
// consumed as the GX texcoord scale by MaruShadowStreamVerts.
extern f32 sMaruShadowDepthScale[2];
