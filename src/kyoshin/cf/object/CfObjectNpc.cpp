// Decompiled TU for kyoshin/cf/object/CfObjectNpc
// NPC object implementation.
//
// CfObjectNpc represents an NPC in the game world. It extends CfObjectMove
// and adds NPC-specific fields and behavior: icon type (from BDAT icon_type),
// relationship meet value (rlt_meet), and a timer for dialogue triggers.

#include "kyoshin/cf/object/CfObjectNpc.hpp"

namespace cf {

// 0x800BFD48
CfObjectNpc::CfObjectNpc() {}

// 0x800BFE00
CfObjectNpc::~CfObjectNpc() {}

// 0x800BFE74
bool CfObjectNpc::func_800BF46C() { return false; }

// 0x800BFEE4
void CfObjectNpc::func_800BF4DC() {}

// 0x800C016C
void CfObjectNpc::func_800BF764() {}

// 0x800C0314
void CfObjectNpc::func_800BF8CC() {}

// 0x800C0368
s16 CfObjectNpc::func_800BF920() { return 0; }

/**
 * Returns the NPC icon type byte.
 * This value comes from the icon_type BDAT column and is stored at offset 0x71C.
 */
u8 CfObjectNpc::func_800BF984() {
    return mIconType;
}

} // namespace cf
