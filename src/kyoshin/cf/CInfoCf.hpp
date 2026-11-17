#pragma once

/**
 * cf::CInfoCf - Information/settings singleton.
 *
 * One global instance is created at boot; the constructor stores the
 * instance pointer in the global lbl_eu_80664250 so any code in this TU
 * can reach it without passing ``this`` around.  Used by the menu / info
 * stack to query and toggle UI state flags.
 *
 * Field layout (MWCC big-endian 32-bit):
 *   0x00  void* vtable     - set to externally-defined lbl_eu_805309B0
 *   0x04  u32   mFlags     - bitfield of state flags
 *   0x08  u32   mField08   - cached setting value (set by InfoCfRefreshSettings)
 *   0x0C  u32   mField0C   - runtime value 1 (set by InfoCfRefreshSettings)
 *   0x10  u32   mField10   - runtime value 2 (set by InfoCfRefreshSettings)
 */

#include <types.h>
#include "monolib/scn/IScnRender.hpp"
#include "monolib/work/CProcess.hpp"


namespace cf {

// The retail ctor symbol __ct__cf_CInfoCf is a C-linkage name (no C++
// mangling markers), so the constructor is emitted as a global function in
// the cpp (same convention as __ct__cf_CfTFile). The dtor keeps its regular
// member mangling (__dt__Q22cf7CInfoCfFv).
struct CInfoCf {
    ~CInfoCf();

    void* vtable;   // 0x00 - externally-defined vtable (lbl_eu_805309B0)
    u32 mFlags;     // 0x04
    u32 mField08;   // 0x08
    u32 mField0C;   // 0x0C
    u32 mField10;   // 0x10
};

} // namespace cf

// CInfoCf vtable (retail .data:0x805309B0) and the UI-state singleton pointer
// (sbss; instance stored by __ct__cf_CInfoCf, cleared by the dtor).
extern u8 lbl_eu_805309B0[];
extern cf::CInfoCf* lbl_eu_80664250;

extern "C" {
u32 InfoCfGetUiFlags();
void CfRes_getInstPtrBC();
int KyoshinHeap_GetActive54();
u32 KyoshinHeap_GetSize500000();
}
