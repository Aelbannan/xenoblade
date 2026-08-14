#pragma once

#include <types.h>
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CNumSelect.hpp"
#include "kyoshin/CScrollBar.hpp"
#include <monolib/util.hpp>
#include <nw4r/lyt.h>

// CItemBoxInfo2 layout lives in CItemBoxInfo.hpp (not included: it redeclares
// __dt__17UnkClass_8045F564Fv with a void* arg, clashing with CSysWin.hpp's
// typed form). Only the pointer is used here (dtor dispatch).
struct CItemBoxInfo2;

/* 12-byte item-box tab entry: halves/words/bytes as stored per slot. */
struct CIBLTabEntry {
    u16 f0;   // +0x0
    u32 f4;   // +0x4
    u8 f8;    // +0x8
    u8 f9;    // +0x9
};

/* Tab holder embedded in CItemBoxLine (+0x3A4): 12 entries then a halfword count. */
struct CIBLTab {
    CIBLTabEntry entries[12]; // +0x00..0x8F
    u16 count;                // +0x90
};

/* Tab holder with a trailing FixStr<32> name buffer at +0x94 (func_801EC3D0). */
struct CIBLTabFormat {
    CIBLTabEntry entries[12]; // +0x00..0x8F
    u16 count;                // +0x90
    u8 pad_92[2];             // +0x92..0x93
    ml::FixStr<32> str94;     // +0x94
};

/* Full tab-format object used by func_801EBAD4 (init) and func_801EC808
 * (name dispatch): 12 entries + halfword count + selector byte + name buffer
 * + counters up to +0x138. */
struct CIBLTabFull {
    CIBLTabEntry entries[12]; // +0x00..0x8F
    u16 count;                // +0x90
    u8 field92;               // +0x92 - selector byte (0xD = name-format mode)
    u8 field93;               // +0x93
    u8 field94;               // +0x94 - name-buffer first byte
    u8 pad_95[0xB4 - 0x95];   // 0x95..0xB3
    u32 fieldB4;              // +0xB4
    u8 fieldB8;               // +0xB8
    u8 pad_B9[0x138 - 0xB9];  // 0xB9..0x137
    u32 field138;             // +0x138
};

/* Minimal vtable view of the embedded cursor object at +0x70. MWCC's vtable
 * prefix (offset-to-top + RTTI) shifts virtual index N to vtable+(N+2)*4, so
 * the 3rd virtual (index 2) sits at vtable+0x10 - the slot func_801EF3E8
 * dispatches through with the syswin-selection buffer. */
class CIBLCur70View {
public:
    virtual void vf00() = 0;        // index 0 -> +0x08
    virtual void vf01() = 0;        // index 1 -> +0x0C
    virtual void vf02(void* data) = 0; // index 2 -> +0x10 - func_801EF3E8
};

/* Vtable view of the CSysWin sub-object at +0x350. MWCC prefixes its vtables
 * with offset-to-top + RTTI, so virtual index N sits at vtable+(N+2)*4; the
 * file-load init (func_801ED31C) dispatches through the LAST CSysWin virtual
 * (index 32 -> vtable+0x88) with the syswin buffer. */
class CSysWinVtblView {
public:
    virtual void vf00() = 0;        // index 0 -> +0x08
    virtual void vf01() = 0;        // index 1 -> +0x0C
    virtual void vf02() = 0;        // index 2 -> +0x10
    virtual void vf03() = 0;        // index 3 -> +0x14
    virtual void vf04() = 0;        // index 4 -> +0x18
    virtual void vf05() = 0;        // index 5 -> +0x1C
    virtual void vf06() = 0;        // index 6 -> +0x20
    virtual void vf07() = 0;        // index 7 -> +0x24
    virtual void vf08() = 0;        // index 8 -> +0x28
    virtual void vf09() = 0;        // index 9 -> +0x2C
    virtual void vf10() = 0;        // index 10 -> +0x30
    virtual void vf11() = 0;        // index 11 -> +0x34
    virtual void vf12() = 0;        // index 12 -> +0x38
    virtual void vf13() = 0;        // index 13 -> +0x3C
    virtual void vf14() = 0;        // index 14 -> +0x40
    virtual void vf15() = 0;        // index 15 -> +0x44
    virtual void vf16() = 0;        // index 16 -> +0x48
    virtual void vf17() = 0;        // index 17 -> +0x4C
    virtual void vf18() = 0;        // index 18 -> +0x50
    virtual void vf19() = 0;        // index 19 -> +0x54
    virtual void vf20() = 0;        // index 20 -> +0x58
    virtual void vf21() = 0;        // index 21 -> +0x5C
    virtual void vf22() = 0;        // index 22 -> +0x60
    virtual void vf23() = 0;        // index 23 -> +0x64
    virtual void vf24() = 0;        // index 24 -> +0x68
    virtual void vf25() = 0;        // index 25 -> +0x6C
    virtual void vf26() = 0;        // index 26 -> +0x70
    virtual void vf27() = 0;        // index 27 -> +0x74
    virtual void vf28() = 0;        // index 28 -> +0x78
    virtual void vf29() = 0;        // index 29 -> +0x7C
    virtual void vf30() = 0;        // index 30 -> +0x80
    virtual void vf31() = 0;        // index 31 -> +0x84
    virtual void vf32() = 0;        // index 32 -> +0x88 - func_801ED31C tail dispatch
};

/* Vtable view for the texture-resource objects at +0x38/+0x3C: the tab-pane
 * refresh (func_801EF734) dispatches through virtual index 1 (vtable+0x0C)
 * with a 'timg' fourcc, a texture name/handle and a flag. */
class CItemBoxLineResView {
public:
    virtual void vf00() = 0;                       // index 0 -> +0x08
    virtual u32 vf01(u32 fourcc, const char* name, u32 flag) = 0; // index 1 -> +0x0C
};

class CIBLTabCur : public CBaseCur {
public:
    CIBLTabCur();

    // CBaseCur occupies sizeof(CBaseCur)=0x18 bytes (4-aligned); byte table at +0xA8.
    u8 pad_18[0xA8 - 0x18]; // 0x18..0xA7
    u8 unkA8[12];           // +0xA8
};

class CItemBoxLine {
public:
    CItemBoxLine();
    ~CItemBoxLine();
    void OnFileEvent();

    void func_801EDA4C(unsigned char val);
    void func_801EECC8();

    // Tab storage (byte entries + count) + unknown byte fields.
    // The retail class stores its vtable manually (like CSysWin): the dtor
    // (__dt__12CItemBoxLineFv) does NOT re-store the vptr, so the dtor is
    // declared non-virtual and the vtable slot is an explicit member at +0x00
    // (MWCC would otherwise emit a __vt__12CItemBoxLine store in the dtor).
    u8* mVtbl;                          // +0x00: retail vtable (set by ctor)
    nw4r::lyt::ArcResourceAccessor* field04;    // +0x04: layout arc resource accessor
    nw4r::lyt::Layout* field08;                 // +0x08: item-box line layout
    nw4r::lyt::AnimTransform* field0C;          // +0x0C: animation transform A
    nw4r::lyt::AnimTransform* field10;          // +0x10: animation transform B
    u8 pad_14[0x24 - 0x14];                     // 0x14..0x23
    u32 field24;                                // +0x24: loaded file handle (item-box line arc)
    u32 field28;                                // +0x28: loaded file handle
    u32 field2C;                                // +0x2C: common-archive file handle
    u32 field30;                                // +0x30: loaded file handle
    u8 pad_34[0x38 - 0x34];                     // 0x34..0x37
    CItemBoxLineResView* field38;       // +0x38: texture resource object B (fallback)
    CItemBoxLineResView* field3C;       // +0x3C: texture resource object A
    nw4r::lyt::Layout* field40;         // +0x40: active layout
    nw4r::lyt::AnimTransform* field44;  // +0x44: animation transform 1
    nw4r::lyt::AnimTransform* field48;  // +0x48: animation transform 2
    u8 field4C;                         // +0x4C
    u8 pad_4D[0x50 - 0x4D];             // 0x4D..0x4F
    int field50;                        // +0x50: navigation state machine
    u8 pad_54[0x59 - 0x54]; // 0x54..0x58
    u8 unk59;               // +0x59: read by func_801ED800
    u8 tabEntries[9];           // 0x5A..0x62: entry bytes, cleared by func_801EDA08
    u8 tabCount;                // +0x63: count
    u8 unk64[9];                // 0x64..0x6C: per-slot occupancy bytes (func_801EDA6C/DB80 scan)
    u8 field6D;                 // +0x6D
    u8 pad_6E[0x70 - 0x6E];             // 0x6E..0x6F
    CBaseCur mCur70;                    // +0x70: embedded cursor (CBaseCur-sized)
    CBaseCur mCur88;                    // +0x88: embedded cursor (0x18 stride)
    CBaseCur mCurA0;                    // +0xA0: embedded cursor
    CBaseCur mCurB8;                    // +0xB8: embedded cursor (vtable[2] tab-name dispatcher)
    u8 mInfo2D0[0x2DC - 0xD0];          // +0xD0..0x2DB: item-box info2 state region
    CNumSelectFull mNumSel;     // +0x2DC: member of func_801ED808
    u8 pad_30B[0x310 - 0x30B];  // 0x30B..0x30F (CNumSelectFull is 0x2F bytes)
    u8 mScrollBar310[0x350 - 0x310]; // +0x310: scroll-bar object region (0x40)
    CSysWinFull mSysWin;        // +0x350: member of func_801ED808
    u8 pad_387[0x38C - 0x387];  // 0x387..0x38B
    s16 unk38C;                 // +0x38C: read by func_801EECC8
    s16 unk38E;                 // +0x38E: read by func_801EECC8
    u8 pad_390[2];              // 0x390..0x391
    s16 field392;               // +0x392: tab page counter (written by func_801EF45C)
    u16 field394;               // +0x394: page id read by func_801EFFC4
    u8 pad_396[8];              // 0x396..0x39D
    u8 unk39E;                  // +0x39E: read by func_801EECC0
    u8 field39F;                // +0x39F: current cursor position (func_801EED6C)
    u8 field3A0;                // +0x3A0: returned by func_801ED808
    u8 pad_3A1[0x3A3 - 0x3A1];  // 0x3A1..0x3A2
    u8 field3A3;                // +0x3A3: wrote by func_801EE684
    CIBLTab unk3A4;             // +0x3A4: target of func_801EC3B0 call
};

// ---------------------------------------------------------------------------
// C-linkage imports - moved verbatim from CItemBoxLine.cpp. The names ARE the
// retail linker symbols; do NOT rename or "fix" the mangled forms.
// ---------------------------------------------------------------------------

// CIBLTabCur vtable (.data)
extern "C" void* lbl_eu_80534D80[];

// CBaseCur ctor/dtor + operator delete (defined in CCur unit).
extern "C" void __ct__8CBaseCurFv(CBaseCur* _this);
extern "C" void* __dt__8CBaseCurFv(CBaseCur* _this, int flags);
extern "C" void __dl__FPv(void*);

// Sub-object destructors invoked by ~CItemBoxLine (retail C-ABI names; the
// member-class dtors live in their own TUs as extern "C" free functions).
extern "C" void __dt__7CSysWinFv(CSysWin*, int);
extern "C" void __dt__10CScrollBarFv(CScrollBar*, int);
extern "C" void __dt__10CNumSelectFv(CNumSelect*, int);
extern "C" void __dt__13CItemBoxInfo2Fv(CItemBoxInfo2*, int);
extern "C" void __dt__7CSubCurFv(CBaseCur*, int);
extern "C" void __dt__6CCur18Fv(CBaseCur*, int);

// --- external retail helpers (C-linkage or literal mangled symbols) ---
extern "C" u32 getItemBox2State__FP13CItemBoxInfo2(void*);
extern "C" u32 func_801EB020(void*);          // num-select busy flag (+0x2C)
extern "C" u32 func_801EB018(void*);          // num-select busy flag (+0x2D)
extern "C" int CSysWin_getUnk34(void*);
extern "C" void advanceItemBox2State__FP13CItemBoxInfo2(void*);
extern "C" int CScrollBar_isVisible(void*);
extern "C" u32 CSysWin_isReady(void*);
extern "C" int CSysWin_isActive(void*);
extern "C" void func_801D216C(void*, u8);
extern "C" void func_8022C1B4(void*, void*, u8);
extern "C" void func_801F369C(void*);
extern "C" void func_80138078__FUl(unsigned int);
extern "C" u8 code80135FDC_getByte_64077();
extern "C" void func_801E174C(void*, void*, unsigned int);
extern "C" u32 func_80137444__FPQ34nw4r3lyt13AnimTransformf(void*, float);
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);
extern "C" u32 func_801393CC(u32);
extern "C" u32 func_801392E4(u32);
extern "C" u16 func_80139358(u32);
extern "C" void func_801586D4(u32, u32);
extern "C" void* func_801571FC(void);
extern "C" void func_80157184(s32);
extern "C" void func_801EB030(void*, void*);
extern "C" void func_801EB04C(void*, u8);
extern "C" void func_801EB064(void*, unsigned int);
extern "C" void func_801EB0D4(void*);
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv(void);
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(unsigned long, const char*, void*, int, int);
extern "C" void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(unsigned long, const char*, void*, int, int);
extern "C" int func_800A9D90(void);
extern "C" void func_801E12E0(void*);
extern "C" void func_801EAE8C(void*);
extern "C" void func_801F34F4(void*);
extern "C" void func_801F3850(void*, u32);
// per-frame update helpers (external retail symbols, same merged unit)
extern "C" void func_801D202C(void*);            // cursor per-frame update (CCur)
extern "C" void func_801E1348(void*);            // item-box info2 per-frame update
extern "C" void func_801EAED4(void*);            // num-select per-frame update
extern "C" void func_801F3540(void*);            // scrollbar per-frame update
extern "C" void func_8022B748(void*);            // syswin per-frame update
extern "C" void func_801F061C(void*, unsigned int); // tab-pane refresh (2-arg caller overload)
extern "C" void func_801F36BC(void*, int, int);      // scrollbar range setup
extern "C" void func_801F3670(void*, const float*);  // scrollbar init (3-float vec)
extern "C" void func_801F367C(void*);                // scrollbar show
extern "C" void func_801E1498(void*);                // info2 state open
extern "C" void func_801E14DC(void*, unsigned short, void*, unsigned short, unsigned int); // info2 tab-data push
extern "C" void func_801E16F0(void*, char*, char*);  // info2 tab-name set
extern "C" void func_801F08B4(void*, unsigned int);  // active-tab dispatch
extern "C" u8 func_801EC8B4(void*, unsigned int);    // tab entry byte accessor (external overload)
// nav-state dispatch helpers (external overloads of the C++ bodies in this TU)
extern "C" void func_801EF1E4(void*);
extern "C" void func_801EF260(void*);
extern "C" void func_801EF2FC(void*);
extern "C" void func_801EF378(void*);
extern "C" void func_801EF3E8(void*);
extern "C" void func_801EF45C(void*);
extern "C" u8 func_801361E8(u32, const char*, u32);
extern "C" void func_80124270(void*, u32);
extern "C" void* lbl_eu_80664104;
extern "C" void* lbl_eu_806640A8;
extern "C" void* lbl_eu_806640EC;
extern "C" void* lbl_eu_806640D8;
extern "C" void drawItemBox2Layout__FP13CItemBoxInfo2PQ34nw4r3lyt8DrawInfo(void*, void*);
extern "C" void func_801D20B0(void*, void*);
extern "C" void func_801F35B0(void*, void*);
extern "C" void func_801EAF7C(void*, void*);
extern "C" void func_8022B7C8(void*, void*);
extern "C" int func_801D2ED8(CBaseCur*);

// Forward declarations for sibling functions resolved at link via the symbol map
// (retail treats these as external relocs, not same-TU symbols)
extern "C" void func_801EFFC4(void*);
extern "C" void func_801F0030(void*);
extern "C" void func_801F0488(void*);
extern "C" void func_801F071C(void*);

// Layout/anim builders (code_80135FDC unit) used by func_801ECC10. The
// unmangled C++ forms mangle to the retail symbols
// (func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc /
// func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc).
void func_80136E84(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
void func_80136F08(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**, nw4r::lyt::ArcResourceAccessor*, char*);
// Cursor deactivation tail handler (CCur unit) - genuine C-ABI helper.
extern "C" void func_801D21CC(void*);

extern "C" void* func_801394D4(u32);
extern "C" char lbl_eu_805071B0[]; // rodata string pool
extern const float lbl_eu_80668114; // .sdata2 animation-completion threshold (lfs-hoist: const routes it to readonly sdata2)
extern const float lbl_eu_806680F8; // .sdata2 tab-pane placement z (setVec3)
extern const float lbl_eu_8066811C; // .sdata2 tab-pane placement x
extern const float lbl_eu_80668120; // .sdata2 tab-pane placement y

// Tab-accessor C-linkage overloads: the typed C++ bodies live in this TU
// (their mangled names map to the retail symbols); these C-linkage
// declarations keep the cross-.o calls (retail external relocs) external
// instead of inlining the local body.
extern "C" void* func_801EC3D0(void*, unsigned int);
extern "C" u32 func_801EC260(void*, unsigned int);
extern "C" u16 func_801EC3B0(CIBLTab*, unsigned int);

// Item-box-line refresh helpers (external retail symbols).
extern "C" void func_801EBC00(CIBLTab*, unsigned char, unsigned short, unsigned char);
extern "C" u32 func_801392B4(u32);

extern "C" void func_8022B8E4(void*);
extern "C" int func_801EB028(void*);
extern "C" void func_801EB178(void*);

// code_80135FDC string/table helpers (retail unmangled C-ABI names).
extern "C" u16 func_80136254(u32, const char*, u32);
extern "C" char* func_80138F78(u32);
extern "C" void func_80137E7C(nw4r::lyt::Layout*, const char*, u32);

// .sbss item-kind owner tables (u32 values; SDA-accessed).
extern u32 lbl_eu_806640F4;
extern u32 lbl_eu_806640F8;