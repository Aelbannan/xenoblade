#pragma once

#include <types.h>

// Single winning declaration for func_8049603C (body in CScn.cpp:
// loads CScn+0x80 and tail-calls func_8049C7A8, returning the scene
// float block; floats live at +0/+4/+8/+C).
//
// Retail calls this BOTH ways: with the scene pointer (most kyoshin
// call sites) and with no source argument at all (cf::CfCam and
// menu::CMenuUpdate::func_801443E4 rely on whatever r3 holds). An
// ellipsis prototype is the only single signature that accepts both
// arities; PPC call codegen is identical either way (plain bl).
//
// NOTE: deliberately NOT included by CScn.cpp - the provisional
// u32(u8*) definition there must stay verbatim for its match.
extern "C" void* func_8049603C(...);
