#pragma once

#include <types.h>

// Thin owner-API header for kyoshin/CQuestWindow (declarations mirror the
// canonical copies in CQuestWindow.hpp) for TUs whose include closure cannot
// take the full owner header.
u32 getQuestWindow();
void resetQuestWindow();
bool isQuestWindowOpen();
