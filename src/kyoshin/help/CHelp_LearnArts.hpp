#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
// size: 0xF
class __declspec(novtable) CHelp_LearnArts : public CHelp {
public:
    bool func_802B8398();
    void isHelpAvailable();

    s16 mWpnTypeCount; // 0xC
    u8 mLearnArtsFlag; // 0xE
};

// Character-data object returned by func_8009EC9C. Fields beyond +0xE8
// are the learn-arts flag grid read by ResTbox_IsCellBitSet.
struct CHelpLearnArtsCharData {
    u8 pad[0xC];            // 0x0..0xB
    u16 mWpnType;           // +0xC
    u8 pad2[0xE8 - 0xE];    // 0xE..0xE7
    u8 mArts[1];            // +0xE8
};

} // namespace cf


// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// CHelpManager singleton is declared (with mLearnArtsFlag @0x16) in
// kyoshin/cf/CHelpManager.hpp; see that header.
extern "C" u8* lbl_eu_806640F4;
extern "C" UNKWORD hasQuestWindow();
extern "C" u8 isQuestWindowOpen();
extern "C" u32 GetItemMulti_IsActiveFlag();
extern "C" void* func_8009EC9C(u32 index);
extern "C" bool ResTbox_IsCellBitSet(unsigned char* p, int i, int j);

// "wpn_type" BDAT column-name string (retail .rodata label, owning TU or extern).
extern u8 lbl_eu_805138AC[];
