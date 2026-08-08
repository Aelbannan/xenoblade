#pragma once

#include <types.h>
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CNumSelect.hpp"
#include <monolib/util.hpp>

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
    virtual ~CItemBoxLine();
    void OnFileEvent();

    void func_801EDA4C(unsigned char val);
    void func_801EECC8();

    // Tab storage (byte entries + count) + unknown byte fields.
    // CItemBoxLine has an implicit vtable pointer at +0x00, so the first
    // explicit member starts at +0x04.
    u8 pad_04[0x50 - 0x04]; // 0x04..0x4F
    int field50;            // +0x50: navigation state machine
    u8 pad_54[0x59 - 0x54]; // 0x54..0x58
    u8 unk59;               // +0x59: read by func_801ED800
    u8 tabEntries[9];           // 0x5A..0x62: entry bytes, cleared by func_801EDA08
    u8 tabCount;                // +0x63: count
    u8 pad_64[0x6D - 0x64];     // 0x64..0x6C
    u8 field6D;                 // +0x6D
    u8 pad_6E[0x2DC - 0x6E];    // 0x6E..0x2DB
    CNumSelectFull mNumSel;     // +0x2DC: member of func_801ED808
    u8 pad_30C[0x350 - 0x30B];  // 0x30B..0x34F (CNumSelectFull is 0x2F bytes)
    CSysWinFull mSysWin;        // +0x350: member of func_801ED808
    u8 pad_387[0x38C - 0x387];  // 0x387..0x38B
    s16 unk38C;                 // +0x38C: read by func_801EECC8
    s16 unk38E;                 // +0x38E: read by func_801EECC8
    u8 pad_390[0x0E];           // 0x390..0x39D
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

// --- external retail helpers (C-linkage or literal mangled symbols) ---
extern "C" u32 getItemBox2State__FP13CItemBoxInfo2(void*);
extern "C" u32 func_801EB020(void*);          // num-select busy flag (+0x2C)
extern "C" u32 func_801EB018(void*);          // num-select busy flag (+0x2D)
extern "C" int CSysWin_getUnk34(void*);
extern "C" void advanceItemBox2State__FP13CItemBoxInfo2(void*);
extern "C" u32 CScrollBar_isVisible(void*);
extern "C" u32 CSysWin_isReady(void*);
extern "C" u32 CSysWin_isActive(void*);
extern "C" void func_801D216C(void*, int);
extern "C" void func_801F369C(void*);
extern "C" void func_80138078__FUl(unsigned int);
extern "C" u8 code80135FDC_getByte_64077();
extern "C" void func_801E174C(void*, void*, unsigned int);
extern "C" u32 func_80137444__FPQ34nw4r3lyt13AnimTransformf(void*, float);
extern "C" u32 func_801393CC(u32);
extern "C" u32 func_801392E4(u32);
extern "C" u32 func_80139358(u32);
extern "C" void func_801586D4(u32, u32);
extern "C" s32 func_801571FC(void);
extern "C" void func_80157184(s32);
extern "C" void func_801EB030(void*, void*);
extern "C" void func_801EB04C(void*, unsigned int);
extern "C" void func_801EB064(void*, unsigned int);
extern "C" void func_801EB0D4(void*);
extern "C" u32 getHandleMEM2__Q23mtl10MemManagerFv(void);
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(unsigned long, const char*, void*, int, int);
extern "C" void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(unsigned long, const char*, void*, int, int);
extern "C" void func_800A9D90(void);
extern "C" void func_801E12E0(void*);
extern "C" void func_801EAE8C(void*);
extern "C" void func_801F34F4(void*);
extern "C" void func_801F3850(void*, unsigned short);
extern "C" u32 func_801361E8(void*, char*, u32);
extern "C" void func_80124270(nw4r::lyt::Pane*, u32);
extern "C" void* lbl_eu_80664104;
extern "C" void* lbl_eu_806640A8;
extern "C" void* lbl_eu_806640EC;
extern "C" void* lbl_eu_806640D8;
extern "C" void drawItemBox2Layout__FP13CItemBoxInfo2PQ34nw4r3lyt8DrawInfo(void*, void*);
extern "C" void func_801D20B0(void*, void*);
extern "C" void func_801F35B0(void*, void*);
extern "C" void func_801EAF7C(void*, void*);
extern "C" void func_8022B7C8(void*, void*);
extern "C" int func_801D2ED8(void*);

// Forward declarations for sibling functions resolved at link via the symbol map
// (retail treats these as external relocs, not same-TU symbols)
extern "C" void func_801EFFC4(void*);
extern "C" void func_801F0030(void*);
extern "C" void func_801F0488(void*);
extern "C" void func_801F071C(void*);

extern "C" void* func_801394D4(u32);
extern "C" char lbl_eu_805071B0[]; // rodata string pool

// These tab accessors are external retail symbols (owned by other TUs); the
// calls from this unit must be C-linkage relocs, so they are declared, not
// defined, here. The bodies live in their owning unit and the symbol map
// resolves the addresses.
extern "C" void* func_801EC3D0(void*, unsigned int);
extern "C" u32 func_801EC260(void*, unsigned int);
extern "C" u16 func_801EC3B0(void*, unsigned int);

extern "C" void func_8022B8E4(void*);
extern "C" int func_801EB028(void*);
extern "C" void func_801EB178(void*);