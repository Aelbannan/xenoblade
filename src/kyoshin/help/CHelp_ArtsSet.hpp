#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
// size: 0x10
class __declspec(novtable) CHelp_ArtsSet : public CHelp {
public:
    // Help-availability predicate (CHelpManager help-list check): false until
    // the sequence counter (CfGameManager::getQueuedFileEventCount) reaches unkC, then
    // true iff the Arts-Set menu instance exists (func_8022F530).
    bool isHelpAvailable();

    s32 unkC; // 0xC
};
} // namespace cf

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" bool func_8022F530();
