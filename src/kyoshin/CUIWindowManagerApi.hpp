#pragma once

#include <types.h>

// Thin owner-API header for kyoshin/CUIWindowManager (definition lives in
// CUIWindowManager.cpp, canonical declaration in CUIWindowManager.hpp). TUs
// whose include closure cannot take the full owner header include this
// instead; the declaration here is identical to the owner copy.

// Flag-buffer builder (retail UIWin_BuildFlagBuf).
extern "C" u8* UIWin_BuildFlagBuf(u32 target);

// Item-availability query (retail UIWin_QueryItemAvail); same signature as the
// owner copy in CUIWindowManager.hpp.
class CItemQuery;
extern "C" int UIWin_QueryItemAvail(CItemQuery* self, u32 arg1, u32 arg2);

struct CFlagBuffer;

// C++-linkage helpers owned by CUIWindowManager.cpp (canonical copies in
// CUIWindowManager.hpp).
int UIWin_CreateExtraWin();
int UIWin_CreateCol6Check();
void UIWin_FlagBufCommit(CFlagBuffer* self);
