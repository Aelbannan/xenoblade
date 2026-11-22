#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
// size: 0xF
class __declspec(novtable) CHelp_LearnArts : public CHelp {
public:
    bool isLearnArtsReady();
    void isHelpAvailable();

    s16 mWpnTypeCount; // 0xC
    u8 mLearnArtsFlag; // 0xE
};

} // namespace cf


// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// CHelpManager singleton is declared (with mLearnArtsFlag @0x16) in
// kyoshin/cf/CHelpManager.hpp; see that header.
extern "C" UNKWORD hasQuestWindow();
extern "C" u32 GetItemMulti_IsActiveFlag();
extern "C" void* CtrlObjectParam_GetCharEntry(u32 index);
extern "C" bool ResTbox_IsCellBitSet(unsigned char* p, int i, int j);

// "wpn_type" BDAT column-name string (retail .rodata label, owning TU or extern).
extern u8 lbl_eu_805138AC[];
