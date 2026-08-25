#pragma once

#include <types.h>

class CScn;

// Single winning declaration for func_8049603C (body in CScn.cpp:
// loads CScn+0x80 and tail-calls func_8049C7A8, returning the scene
// float block; floats live at +0/+4/+8/+C).
//
// NOTE: two retail call sites (cf::CfCam, menu::CMenuUpdate::func_801443E4)
// branch here WITHOUT setting up r3 (stale-register trick); those TUs keep
// a private no-arg declaration and must NOT include this header - a single
// prototype cannot express both arities, and an ellipsis one would make
// MWCC emit crxor cr1 before every call.
//
// NOTE: deliberately NOT included by CScn.cpp - the provisional
// u32(u8*) definition there must stay verbatim for its match.
extern "C" void* func_8049603C(CScn* scene);
