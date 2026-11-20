#pragma once

#include <types.h>
#include "kyoshin/CTitleAHelp.hpp"
#include "monolib/core/CViewFrame.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)

// Verbatim-mangled import: the real retail CfGameManager::isClassicController
// takes the selector argument (call sites pass -1). The inline s16 overload
// in include/kyoshin/cf/CfGameManager.hpp drops it, so callers that need the
// argument use this declaration instead.

// Forward decls only. The full sub-object headers (CTitleAHelp.hpp,
// CModelDisp.hpp, CPartyState.hpp, CEquipChange.hpp) cannot be included here:
// e.g. CPartyState.hpp's `extern CScn* gmScene;` clashes with
// CMenuPTState.cpp's own `extern void* gmScene;`, and
// CModelDispEquip.hpp's `struct CEventFile` clashes with
// monolib/work/CEventFile.hpp in TUs that include CBgTex.hpp / CScn.hpp.
// The sub-objects are embedded at fixed offsets, so sized pads + casts suffice.
class CTitleAHelp;
class CModelDisp;
class CPartyState;
class CEquipChange;
class CSysWin;
class CScn;
class CWorkThread;
class IScnRender;
class CProc;

// nw4r layout DrawInfo (full definition in <nw4r/lyt/lyt_drawInfo.h>;
// only a pointer is used in declarations here).
namespace nw4r { namespace lyt { class DrawInfo; } }

// Opaque view of the CModelDispEquip sub-object embedded at +0x3090 (retail
// size 0x10C0). The real class lives in
// kyoshin/menu/parts/CModelDispEquip.hpp, which cannot be included here (its
// `struct CEventFile` clashes with monolib/work/CEventFile.hpp). Only the
// embedded region and the C-linkage helper relocs are needed, so a sized
// opaque view suffices.
struct CModelDispEquipView {
    u8 data[0x10C0];
};

// Memory-accounting object used by PartyStateWin_PickAllocHandle: two signed counters at
// +0x00/+0x04 that are decremented by the freed size plus the 0x80 block
// header, after pinging the MEM1/MEM2 alloc handles.
struct CPartyStateWinMem {
    s32 field_0x0;   // 0x00
    s32 field_0x4;   // 0x04
};

// Byte view of the u32 window-state word (lbl_eu_806681E4) used by
// PartyStateWin_RefreshTitleHelp: the word is copied to a stack local and indexed by the
// party-state selection value (retail: lwz/stw copy + lbzx index).
union CPartyStateWinWord {
    u32 word;     // 0x00
    u8 bytes[4];  // 0x00
};

// Copy blob used by PartyStateWin_CopyWinBlob: three 0xFF0-byte blocks at +0x08/+0xFF8/
// +0x1FE8 (copied by PartyStateWin_CopyBlockFF0) plus trailing scalar fields.
struct CPartyStateWinCopy {
    u8 _pad00[0x4];              // 0x00
    u32 field_0x4;               // 0x04
    u8 block0[0xff0];            // 0x08..0xFF8
    u8 block1[0xff0];            // 0xFF8..0x1FE8
    u8 block2[0xff0];            // 0x1FE8..0x2FD8
    u8 field_0x2FD8;             // 0x2FD8
    u8 _pad2FD9[0x2fdc - 0x2fd9]; // 0x2FD9..0x2FDC
    f32 field_0x2FDC;            // 0x2FDC
    f32 field_0x2FE0;            // 0x2FE0
    u8 field_0x2FE4;             // 0x2FE4
};

// 8-byte pair (two u32) used for the tail of the PartyStateWin_CopyBlockFF0 block; the
// copy loop iterates these (retail mtctr/bdnz lwzu+stwu shape, same as the
// SCopyPair loops in CopyPTSubBlob).
struct CPartyStateWinWordPair {
    u32 lo;  // 0x00
    u32 hi;  // 0x04
};

// 0xFF0-byte state block copied by PartyStateWin_CopyBlockFF0: three leading u32, three
// SCopy_80191C88 blobs (0x53C each), a trailing u32, a single word copied
// before the loop and four word-pairs (words[1..8]) copied by the loop.
struct CPartyStateWinBlock {
    u32 field_0x0;       // 0x000
    u32 field_0x4;       // 0x004
    u32 field_0x8;       // 0x008
    u8 copy0[0x53c];     // 0x00C..0x548 (SCopy_80191C88 blob)
    u32 field_0x548;     // 0x548
    u32 field_0x54c;     // 0x54C
    u8 copy1[0x53c];     // 0x550..0xA8C (SCopy_80191C88 blob)
    u8 copy2[0x53c];     // 0xA8C..0xFC8 (SCopy_80191C88 blob)
    u32 field_0xfc8;     // 0xFC8
    u32 word0;           // 0xFCC (copied separately before the loop)
    CPartyStateWinWordPair pairs[4];  // 0xFD0..0xFEC (words[1..8])
    u32 field_0xfec;     // 0xFEC..0xFF0 (not copied)
};

// 0x24-byte ring-buffer record pushed by PartyStateWin_PushRingRecord into the view object
// at CPartyStateWin +0x14 (type word, 7 words, u16, u8, u8 - the trailing
// fields sit at 0x20/0x22/0x23, not packed after 0x18).
struct CPartyStateWinRec {
    u32 field_0x0;       // 0x00 type tag
    u32 field_0x4;       // 0x04
    u32 field_0x8;       // 0x08
    u32 field_0xc;       // 0x0C
    u32 field_0x10;      // 0x10
    u32 field_0x14;      // 0x14
    u32 field_0x18;      // 0x18
    u32 field_0x1c;      // 0x1C
    u16 field_0x20;      // 0x20
    u8 field_0x22;       // 0x22
    u8 field_0x23;       // 0x23
};

// Ring-buffer view of the object PartyStateWin_PushRingRecord pushes records into (the
// CView created by the ctor at CPartyStateWin +0x14).
struct CPartyStateWinRing {
    u8 _pad00[0x3ec];    // 0x000..0x3EC
    u8* field_0x3ec;     // 0x3EC record buffer base
    s32 field_0x3f0;     // 0x3F0
    s32 field_0x3f4;     // 0x3F4 write offset
    s32 field_0x3f8;     // 0x3F8 ring size (divisor)
    s32 field_0x3fc;     // 0x3FC last written offset
};

/*
 * Party-state window object (US retail).
 *
 * Deliberately NON-polymorphic in this TU: the retail object carries two
 * vtable-like pointers at +0x00/+0x04 (composite vtable lbl_eu_805352DC
 * written by the factory ctor, owned by another .data unit), and the -4
 * interface thunks PartyStateWin_ThunkRenderBefore / PartyStateWin_ThunkDtor are dispatched through the
 * +0x04 vtable slot (they receive obj+4 and rewind to the object base).
 * The dtor is a plain (non-virtual) member so those thunks forward to the
 * retail symbol directly; it is not defined in this TU yet.
 *
 * Layout (from the ctor / dtor ASM):
 *   0x00  composite vtable pointer (lbl_eu_805352DC)
 *   0x04  secondary vtable pointer (lbl_eu_805352DC + 0x88)
 *   0x08  ctor arg / 0x0C ctor arg / 0x10 0x14 zeroed words
 *   0x18  CTitleAHelp
 *   0x50  CModelDisp (region 0x444..0x454 copied by PartyStateWin_StoreVec4Quad)
 *   0x3038 CPartyState
 *   0x3090 CModelDispEquip
 *   0x4150 CEquipChange
 *   0x6BA8 CSysWin
 *   0x6BE4 / 0x6BE5 byte flags (ctor zero-fill)
 *   0x6BE8 float (ctor init)
 */
class CPartyStateWin {
public:
    CPartyStateWin(u32 arg1, u32 arg2);
    ~CPartyStateWin();
    void cbRenderBefore();

    u32 mVtbl0;                            // 0x00 composite vtable (lbl_eu_805352DC)
    u32 mVtbl4;                            // 0x04 secondary vtable (lbl_eu_805352DC + 0x88)
    u32 mArg1;                             // 0x08 ctor arg 1
    u32 mArg2;                             // 0x0C ctor arg 2
    CScn* mScene;                          // 0x10 owning scene (0 until assigned)
    CWorkThread* mWork14;                  // 0x14 child work thread (0 until assigned)
    u8 _pad18[0x38];                       // 0x18..0x50 (CTitleAHelp)
    u8 _pad50[0x444 - 0x50];               // 0x50..0x444 (CModelDisp head)
    u32 mQuad444[4];                       // 0x444..0x453 (PartyStateWin_StoreVec4Quad target)
    u8 _pad454[0x3038 - 0x454];            // 0x454..0x3038 (CModelDisp tail)
    u8 _pad3038[0x58];                     // 0x3038..0x3090 (CPartyState)
    CModelDispEquipView mModelDispEquip;   // 0x3090 (0x10C0)
    u8 _pad4150[0x2A58];                   // 0x4150..0x6BA8 (CEquipChange)
    u8 _pad6BA8[0x3C];                     // 0x6BA8..0x6BE4 (CSysWin)
    u8 field_6BE4;                         // 0x6BE4
    u8 field_6BE5;                         // 0x6BE5 (PartyStateWin_GetLatchFlag getter)
    u8 _pad6BE6[2];                        // 0x6BE6..0x6BE8
    f32 field_6BE8;                        // 0x6BE8 (ctor init)
};

void PartyStateWin_TeardownWindow(CPartyStateWin* self);
int PartyStateWin_GetLatchFlag(CPartyStateWin* self);
int PartyStateWin_QueryMenuClose(CPartyStateWin* self);
void PartyStateWin_ThunkDtor(void* self);
