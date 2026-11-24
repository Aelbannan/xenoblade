// Auto-scaffolded catalog TU for kyoshin/CItemBoxLine
// High-level C/C++ reconstruction from retail PPC.

#define IWORK_EVENT_INLINE_DTOR // ~CItemBoxLine elides the empty base dtor (retail)

#include "kyoshin/cf/CfBdat.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/code_8025FB10.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CItemBoxLine.hpp"
#include "kyoshin/CTagProcessor.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"
#include "kyoshin/CUICfManager_Res.hpp"
#include "kyoshin/cf/CtrlRemoteApi.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_pane.h>
#include <nw4r/lyt/lyt_material.h>
#include <nw4r/ut/ut_TagProcessorBase.h>
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/device/CDeviceSC.hpp"

#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/code_80135FDC_State.hpp"

/* Tab holder with a trailing FixStr<32> name buffer at +0x94. */
struct CIBLTabFormat {
    CIBLTabEntry entries[12]; // +0x00..0x8F
    u16 count;                // +0x90
    u8 pad_92[2];             // +0x92..0x93
    ml::FixStr<32> str94;     // +0x94
};

/* Full tab-format object used by the init/alloc/format helpers. */
struct CIBLTabFull {
    CIBLTabEntry entries[12]; // +0x00..0x8F
    u16 count;                // +0x90
    u8 field92;               // +0x92 - selector byte
    u8 field93;               // +0x93
    ml::FixStr<32> str94;     // +0x94 (mString[32] + mLength at +0xB4)
    ml::FixStr<128> strB8;    // +0xB8 (mString[128] + mLength at +0x138)
};

// Tab entry accessors are declared in kyoshin/CItemBoxLine.hpp.
extern "C" void ItemBoxLine_CopyTabEntry(CIBLTabEntry* dest, const CIBLTabEntry* src);
extern "C" CIBLTabEntry* ItemBoxLine_InitTabEntry(CIBLTabEntry* self, u16 item, u32 value, u8 lock, u8 ready);

// Retail data labels owned by this TU / sibling merged units.
extern const float lbl_eu_80668114;
extern const float lbl_eu_806680F8;
extern u32 lbl_eu_80664184;

// Plain C-ABI helpers used by this TU.
extern "C" void* Bdat_GetTable_AA34();
extern "C" void CItem_thunkAllocRecord(u32, u32);
extern "C" void* CItemBlock_getPtr20E8(void);
extern "C" void CItemBlock_setCount(s32);
extern "C" int KyoshinHeap_GetField44(void);
extern "C" u32 CtrlObjectParam_GetArtsDataWord(void*);
extern "C" void* CtrlObjectParam_GetCharEntry(u32);
extern "C" u32 CtrlObjectParam_GetCurrentRowKey(void*);
extern "C" u32 CItemBlock_countEmpty(u8);
extern "C" u32 CItem_sumFamilyByte6(unsigned int);
extern "C" u32 ItemBoxLine_CalcCountKey(void*, unsigned int);
extern "C" u8 ItemBoxLine_ProbePageSlot(void*, unsigned int);
extern "C" char* getErrMesText16(void);
extern "C" char* getErrMesText15(void);
extern "C" void ItemBoxLine_BuildTabEntries(CIBLTab*, unsigned char, unsigned short, unsigned char);
extern "C" void CopyTabSlotVec(CIBLVec3*, const u16*, u32);
extern "C" void copyVEC3(void*, const void*);
extern "C" u16 ArrayGet12(const u16*, u8);
extern "C" char* ItemBoxLine_FormatDisplayName(CIBLTabFull*, u16);
extern "C" void SplitU32ToS16s(void*, u32);

// Item-box guard words and the fourteen static colour objects defined at the
// foot of this TU (retail .sbss 0x806645D8..0x80664640).
extern u32 gItemBoxLineMnuItemMesATable;
extern u32 gItemBoxLineMnuItemMesBTable;
extern u32 gItemBoxLineMnuSkillTable;
extern GXColorS10 itemBoxLine_tabLabelNeutralTop;
extern GXColorS10 itemBoxLine_tabLabelNeutralBottom;
extern GXColorS10 itemBoxLine_tabLabelSelectedTop;
extern GXColorS10 itemBoxLine_tabLabelSelectedBottom;
extern GXColorS10 itemBoxLine_tabValueNeutralTop;
extern GXColorS10 itemBoxLine_tabValueNeutralBottom;
extern GXColorS10 itemBoxLine_tabValueSelectedTop;
extern GXColorS10 itemBoxLine_tabValueSelectedBottom;
extern GXColorS10 itemBoxLine_pageLabelNeutralTop;
extern GXColorS10 itemBoxLine_pageLabelNeutralBottom;
extern GXColorS10 itemBoxLine_pageLabelSelectedTop;
extern GXColorS10 itemBoxLine_pageLabelSelectedBottom;
extern GXColorS10 itemBoxLine_pageValueSelectedTop;
extern GXColorS10 itemBoxLine_pageValueSelectedBottom;

int sprintf(char*, const char*, ...);

static const char* const sItemBoxLineTabFormats[14] = { // name-guess: retail .rodata table at 0x80506D90+0
    NULL, NULL, "wpn", NULL, "head", "body", "arm", "waist", "legg", NULL, NULL, NULL, NULL, "arts",
};
static const char* const sItemBoxLineTabNames[27] = { // name-guess: retail .rodata table at 0x80506D90+0x240
    "exchangelist0000", "exchangelist0101", "exchangelist0201", "exchangelist0301",
    "exchangelist0401", "exchangelist0402", "exchangelist0501", "exchangelist0601",
    "exchangelist0701", "exchangelist0801", "exchangelist0901", "exchangelist1001",
    "exchangelist1101", "exchangelist1201", "exchangelist1202", "exchangelist1301",
    "exchangelist1401", "exchangelist1501", "exchangelist1601", "exchangelist1601",
    "exchangelist1701", "exchangelist1801", "exchangelist1901", "exchangelist2001",
    "exchangelist2101", "exchangelist2201", "exchangelist2301",
};
static const char* const sItemBoxLineTabKinds[28] = { // name-guess: retail .rodata table at 0x80506D90+0x2F8
    NULL, NULL, NULL, NULL, "wpn1", NULL, "kessyou1", "kessyou2", "head1", NULL,
    "body1", NULL, "arm1", NULL, "waist1", NULL, "legg1", NULL, NULL, NULL,
    "collect1", "collect2", "materia1", "materia2", NULL, NULL, NULL, NULL,
};
static const char* const sItemBoxLinePercentColumns[7] = { // name-guess: retail .rodata table at 0x80507140
    "", "percent_E", "percent_D", "percent_C", "percent_B", "percent_A", "percent_S",
};

// ============================================================================
// ItemBoxLine_GetSelectReady: busy-guard chain - only read the selector byte when every
// sub-system (info2 state, num-select, scrollbar, syswin) reports active/ready.
// ============================================================================
int CItemBoxLine::GetSelectReady() {
    if (mInfo2.GetState() == 0) return 0;
    if (mNumSel.IsLoaded() == 0) return 0;
    if (mScrollBar.isVisible() == 0) return 0;
    if (reinterpret_cast<CSysWin*>(&mSysWin)->isReady() != 0) return field58;
    return 0;
}

// ============================================================================
// ItemBoxLine_LeaveState3: leave state-3 - store 4, reset byte flag, quiet cursors,
// advance the item-box state machine, refresh scrollbar, and beep if armed.
// ============================================================================
void CItemBoxLine::LeaveState3() {
    if (field50 != 3) return;
    field50 = 4;
    unk59 = 0;
    mCurB8.SetVisible(0);
    mCur70.SetVisible(0);
    mCurA0.SetVisible(0);
    mInfo2.Close();
    mScrollBar.requestScrollOut();
    if (!unk39E) playUISound(6);
}

// ============================================================================
// ItemBoxLine_RefreshTabSelect: invoke the tab-left selection refresh if nothing is busy.
// ============================================================================
void CItemBoxLine::RefreshTabSelect() {
    if (unk38C == -1) return;
    if (mNumSel.IsOpen()) return;
    if (reinterpret_cast<CSysWin*>(&mSysWin)->isOpened()) return;
    if (field3A0) return;
    unk38C = -1;
    unk38E = 0;
    RefreshLineRows();
    RefreshTabPanes();
    RefreshPageList();
    mScrollBar.PlaceThumb((u16)unk38E);
    playUISound(2);
}

int CItemBoxLine::IsReadyFlag() { return unk59; }

// ============================================================================
// ItemBoxLine_CopyTabEntry: copy a 12-byte tab entry. extern "C": the retail
// name is unmangled and ItemBoxLine_InitTabFormat's calls are external relocs (see the
// ItemBoxLine_InitTabEntry stub note).
// ============================================================================
extern "C" void ItemBoxLine_CopyTabEntry(CIBLTabEntry* dest, const CIBLTabEntry* src) {
    dest->f0 = src->f0;
    dest->f4 = src->f4;
    dest->f8 = src->f8;
    dest->f9 = src->f9;
}

// us-801ed7bc: CIBLTab has an empty out-of-line destructor (CItemBoxLine
// ctor/dtor extab DESTROYMEMBER +0x3A4).
CIBLTab::~CIBLTab() {}

// ============================================================================
// ItemBoxLine_BuildTabEntries: (re)build the tab list. Clears the entries, then either probes
// the item-name tables per tab (field93 != 1) or walks the two-slot name tables
// (field93 == 1), appending each hit as a tab entry with its availability flags
// (f8 from the page-count overflow, f9 from the kind-owner lookups). The
// definition inherits C linkage from the header's extern "C" declaration.
// ============================================================================
void ItemBoxLine_BuildTabEntries(CIBLTab* self, u8 arg2, u16 arg3, u8 arg4) {
    ml::FixStr<32> str;
    self->count = 0;
    self->field92 = arg2;
    CIBLTabEntry tmp;
    for (u16 i = 0; i < 12; i++) {
        ItemBoxLine_CopyTabEntry(&self->entries[i], ItemBoxLine_InitTabEntry(&tmp, 0, 0, 0, 0));
    }
    if (self->field93 != 1) {
        // Item-name probe path: per-tab format table, then a 12-entry scan.
        const char* const* fmt = sItemBoxLineTabFormats;
        u32 cur = (u32)CItemBlock_getPtr20E8();
        for (u16 i = 1; i <= 12; i++) {
            str.format("%s%d", fmt[self->field92], i);
            u16 result = BdatGetU16ByTableKey("shoplist", str.mString, arg3);
            if (result == 0) return;
            CIBLTabEntry* entry = &self->entries[self->count++];
            entry->f0 = result;
            LookupItemTypeTable(result);
            BdatGetItemType(result);
            BdatGetItemId(result);
            entry->f4 = ItemBoxLine_CalcCountKey(self, result);
            if (entry->f4 > cur) entry->f8 = 1;
            if (arg4 != 0) {
                if (self->field92 == 0xd) {
                    if (BdatGetU8Direct((const void*)::cf::CfBdat::spItmArtsListFileData, "pc_type",
                                      (u16)BdatGetItemId(result)) != 0) {
                        entry->f9 = 1;
                    }
                } else {
                    u32 t = 0;
                    switch (self->field92) {
                    case 2: t = (u32)::cf::CfBdat::spItmWpnListFileData; break;
                    case 4: case 5: case 6: case 7: case 8: t = (u32)::cf::CfBdat::spItmEquipListFileData; break;
                    }
                    if (self->field92 == 2) {
                        str.format("equip_pc%d", arg4);
                    } else {
                        str.format("pc%d", arg4);
                    }
                    if (BdatGetU8Direct((const void*)t, (const char*)str.mString, (u16)BdatGetItemId(result)) != 0) {
                        entry->f9 = 1;
                    }
                    if (self->field92 != 2) {
                        u8 v = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spItmEquipListFileData, "arm_type",
                                                 BdatGetItemId(result));
                        u32 base = (u32)CtrlObjectParam_GetCharEntry(arg4) + 0x3534;
                        if (v == 3) {
                            if (Counter_TestBit((void*)base, 0x85) == 0) entry->f9 = 0;
                        } else if (v == 2) {
                            if (Counter_TestBit((void*)base, 0x84) == 0) entry->f9 = 0;
                        }
                    }
                }
            }
        }
    } else {
        // Name-table path: copy the name list, resolve the file via the name
        // index byte, then walk the two kind-owner pairs.
        const char* const* names = sItemBoxLineTabNames;
        u8 n = (u8)lbl_eu_80664184;
        Bdat_GetTable_AA34();
        void* fp = getFP(names[n - 1]);
        u8 v = (u8)MapValueToRank6(n);
        u16 id = CtrlRemote_TouchBitByArg(v + 0x21);
        u8 rowIdx;
        if (id >= 0x1f40) {
            rowIdx = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spFldNpcListFileData, "exc_id5", arg3);
        } else if (id >= 0x1770) {
            rowIdx = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spFldNpcListFileData, "exc_id4", arg3);
        } else if (id >= 0xfa0) {
            rowIdx = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spFldNpcListFileData, "exc_id3", arg3);
        } else if (id >= 0x7d0) {
            rowIdx = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spFldNpcListFileData, "exc_id2", arg3);
        } else {
            rowIdx = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spFldNpcListFileData, "exc_id1", arg3);
        }
        const char* const* kinds = sItemBoxLineTabKinds;
        CItemBlock_getPtr20E8();
        for (int j = 0; j < 2; j++) {
            if (kinds[self->field92 * 2 + j] == 0) return;
            u16 result = BdatGetU16Direct((const void*)fp, kinds[self->field92 * 2 + j], rowIdx);
            if (result == 0) return;
            CIBLTabEntry* entry = &self->entries[self->count++];
            entry->f0 = result;
            LookupItemTypeTable(result);
            BdatGetItemType(result);
            BdatGetItemId(result);
            entry->f4 = ItemBoxLine_CalcCountKey(self, result);
            if (arg4 != 0) {
                if (self->field92 == 3) {
                    entry->f9 = 1;
                } else if (self->field92 <= 8) {
                    u32 t = 0;
                    switch (self->field92) {
                    case 2: t = (u32)::cf::CfBdat::spItmWpnListFileData; break;
                    case 4: case 5: case 6: case 7: case 8: t = (u32)::cf::CfBdat::spItmEquipListFileData; break;
                    }
                    if (self->field92 == 2) {
                        str.format("equip_pc%d", arg4);
                    } else {
                        str.format("pc%d", arg4);
                    }
                    if (BdatGetU8Direct((const void*)t, (const char*)str.mString, (u16)BdatGetItemId(result)) != 0) {
                        entry->f9 = 1;
                    }
                    if (self->field92 != 2) {
                        u8 w = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spItmEquipListFileData, "arm_type",
                                                 BdatGetItemId(result));
                        u32 base = (u32)CtrlObjectParam_GetCharEntry(arg4) + 0x3534;
                        if (w == 3) {
                            if (Counter_TestBit((void*)base, 0x85) == 0) entry->f9 = 0;
                        } else if (w == 2) {
                            if (Counter_TestBit((void*)base, 0x84) == 0) entry->f9 = 0;
                        }
                    }
                } else {
                    entry->f9 = 1;
                }
            }
        }
    }
}


// ============================================================================
// Tab entry accessors (array of 12-byte CIBLTabEntry behind a halfword count)
// ============================================================================

// ItemBoxLine_GetTabEntryLock: u8 at +0x8
u8 ItemBoxLine_GetTabEntryLock(const CIBLTab* self, unsigned int index) {
    if (index < self->count) {
        return self->entries[index].f8;
    }
    return 0;
}

// ItemBoxLine_GetTabEntryValue: u32 at +0x4
u32 ItemBoxLine_GetTabEntryValue(const CIBLTab* self, unsigned int index) {
    if (index < self->count) {
        return self->entries[index].f4;
    }
    return 0;
}

// ItemBoxLine_GetTabEntryItem: u16 at +0x0
u16 ItemBoxLine_GetTabEntryItem(const CIBLTab* self, unsigned int index) {
    if (index < self->count) {
        return self->entries[index].f0;
    }
    return 0;
}

// ItemBoxLine_GetTabEntryReady: u8 at +0x9 (default 1)
// Retail calls this accessor out-of-line from ItemBoxLine_OpenLine/ItemBoxLine_RefreshPageList.
u8 ItemBoxLine_GetTabEntryReady(const CIBLTab* self, unsigned int index) {
    if (index < self->count) {
        return self->entries[index].f9;
    }
    return 1;
}

// ============================================================================
// CIBLTabCur vtable plumbing (retail ctor/dtor symbols are plain names)
// ============================================================================

CIBLTabCur::CIBLTabCur(nw4r::lyt::ArcResourceAccessor* arcResAcc) : CBaseCur(arcResAcc) {}

CIBLTabCur::~CIBLTabCur() {}

// ============================================================================
// ItemBoxGetTab: byte at this+0xA8, index bounded to a 12-entry table
// ============================================================================
u8 ItemBoxGetTab(const CItemBoxPage* page, unsigned int index) {
    if (index < 12u) {
        return page->mBytesA8[index];
    }
    return 0;
}

// ============================================================================
// ItemBoxLine_ResetTabBytes: reset tab byte entries + two counters
// ============================================================================
void CItemBoxLine::ResetTabBytes() {
    memset(tabEntries, 0, 9);
    tabCount = 0;
    field6D = 0;
}

// ============================================================================
// ItemBoxLine_FormatTabEntryName: format the entry index's name into the trailing FixStr<32>
// buffer (pool+0x57 "%s"); null if the index is out of range.
// ============================================================================
ml::FixStr<32>* ItemBoxLine_FormatTabEntryName(CIBLTabFormat* self, unsigned int index) {
    if (index < self->count) {
        self->str94.format("%s", BuildItemNameStr(self->entries[index].f0));
        return &self->str94;
    }
    return 0;
}

// ============================================================================
// ItemBoxLine_FormatDisplayName
// ============================================================================
// ============================================================================
// ItemBoxLine_FormatDisplayName: build the tab entry's display name into the trailing
// FixStr<128> buffer (+0xB8) and return it. Selector 3 composes a base name
// from the kind-owner tables and then rewrites "$1"/"$2" placeholders in
// place from per-kind name parts (Shift-JIS aware: lead bytes 0x81-0x9F /
// 0xE0-0xEF advance by 2); any other selector resolves the display name
// through the kind-owner tables directly.
// ============================================================================
extern "C" char* ItemBoxLine_FormatDisplayName(CIBLTabFull* self, u16 index) {
    if (index >= self->count) return NULL;
    u16 f0 = self->entries[index].f0;
    if (f0 == 0) return NULL;
    u8 sel = self->field92;
    if (sel == 3) {
        BdatGetItemType(f0);
        u16 kind = BdatGetItemId(f0);
        u8 n = BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData, "rankType", f0);
        u8 pct = BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, sItemBoxLinePercentColumns[n], kind);
        ml::FixStr<32> partName(true);
        partName.format("%s", BdatTouchStringCell("MNU_shop", "name", n));
        u8 sub = BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, "rvs_type", kind);
        char* base = BdatGetPtrDirect((const void*)::cf::CfBdat::spBtlSkillListFileData, "rvs_caption", kind);
        self->strB8.format("%s", base);
        // Only some languages keep the full-width decoration around $-parts.
        int full = 1;
        if (CDeviceSC::getLanguage() == 3 || CDeviceSC::getLanguage() == 2) {
            full = 0;
        }
        // Placeholder rewrite pass over a stack copy of the composed name.
        // NOTE: the exact per-sub-case sprintf format offsets are reconstructed;
        // the retail loop shuffles in-buffer pointers instead of concatenating.
        char work[0x20];
        strcpy(work, self->strB8.mString);
        char out[0x20];
        char tmp[0x20];
        out[0] = '\0';
        char* w = work;
        while (*w != '\0') {
            signed char c = *w;
            if (c == '$') {
                w++;
                signed char d = *w++;
                tmp[0] = '\0';
                if (d == '1') {
                    switch (sub) {
                    case 1:
                    case 2:
                        sprintf(tmp, "<col=red>%s<col=def2>", partName.mString);
                        break;
                    case 3:
                    case 4:
                        if (full) {
                            sprintf(tmp, "<col=red>%s %%<col=def2>", partName.mString);
                        } else {
                            sprintf(tmp, "<col=red>%s%%<col=def2>", partName.mString);
                        }
                        break;
                    }
                } else if (d == '2') {
                    if (full) {
                        sprintf(tmp, "<col=red>%d %%<col=def2>", pct);
                    } else {
                        sprintf(tmp, "<col=red>%d%%<col=def2>", pct);
                    }
                }
                strcat(out, tmp);
            } else {
                // Shift-JIS lead-byte pair advance.
                int adv = (((u8)c >= 0x81 && (u8)c <= 0x9f) || ((u8)c >= 0xe0 && (u8)c <= 0xef)) ? 2 : 1;
                strncat(out, w, adv);
                w += adv;
            }
        }
        strcpy(self->strB8.mString, out);
        self->strB8.mLength = strlen(out);
    } else {
        // Non-composed selectors: resolve the name through the kind-owner
        // tables (a guarded alternate table pair when the selector exceeds 5).
        u32 table = gItemBoxLineMnuItemMesBTable;
        if ((u8)(sel + 0xfc) > 5 && sel != 2) {
            table = gItemBoxLineMnuItemMesATable;
        }
        u16 v = BdatGetU16Direct((const void*)::cf::CfBdat::spItmItemListFileData, "comment", f0);
        char* name = BdatGetPtrDirect((const void*)table, "comment", v);
        self->strB8.format("%s", name);
    }
    return &self->strB8.mString[0];
}

void ItemBoxLine_AllocEntryStock(CIBLTabFull* self, u32 index, u32 count) {
    if (index >= self->count) return;
    CIBLTabEntry* entry = &self->entries[index];
    LookupItemTypeTable(entry->f0);
    BdatGetItemType(entry->f0);
    BdatGetItemId(entry->f0);
    if (self->field92 != 0xd) {
        for (u8 i = 0; i < count; i++) {
            CItem_thunkAllocRecord(entry->f0, 1);
        }
    } else {
        CItem_thunkAllocRecord(entry->f0, count);
    }
    CItemBlock_setCount((s32)((u32)CItemBlock_getPtr20E8() - entry->f4 * count));
}

// ============================================================================
// ItemBoxLine_ProbeNameSlot: name-dispatch lookup. Bounds-check the tab entry, then by the
// selector byte search the kind-owner table (::cf::CfBdat::spItmWpnListFileData for selector 2,
// ::cf::CfBdat::spItmEquipListFileData otherwise) for the entry's category key: probe slot names
// "%s%u" until BdatGetU16Direct finds a match (0xFD), exhaust the count (0xFE),
// or bail with 0. Selector 0xD (name-format mode) returns 0 via its own tail.
// The goto chain reproduces retail's test ladder: the search body sits in the
// fall-through after the tests, with both ret-0 tails kept separate.
// ============================================================================
int ItemBoxLine_ProbeNameSlot(CIBLTabFull* self, u32 index) {
    u16 f0;
    u16 v;
    u8 sel;
    if (index >= self->count) goto ret0;
    f0 = self->entries[index].f0;
    if (f0 == 0) goto ret0;
    v = BdatGetItemId(f0);
    sel = self->field92;
    if ((u32)(sel - 4) <= 4) goto search;
    if ((int)sel == 2) goto search;
    if ((int)sel == 0xd) goto retSel13;
    goto ret0;
search:
    {
        u32 obj;
        if ((u32)sel == 2) {
            obj = (u32)::cf::CfBdat::spItmWpnListFileData;
        } else {
            obj = (u32)::cf::CfBdat::spItmEquipListFileData;
        }
        u8 n = BdatGetU8Direct((const void*)obj, "jwl_slot", (u16)v);
        if (n == 0) goto ret0;
        char buf[0x20];
        for (u8 i = 0; i < n; i++) {
            sprintf(buf, "jwl_skill%d", (u8)i + 1);
            if ((u16)BdatGetU16Direct((const void*)obj, (const char*)buf, (u16)v) != 0) return 0xfd;
        }
        return 0xfe;
    }
retSel13:
    return 0;
ret0:
    return 0;
}

// ============================================================================
// ItemBoxLine_CalcCountKey: per-item display-count key (same shape as the equip item
// box's calcEIBCountKey). Category band selects the computation - cat 3 scales
// the bdat count by a per-item float record, cats 2-8 accumulate a per-count
// increment from two sdata2 floats, everything else passes the raw count
// through. Result is returned *10.
// ============================================================================
extern "C" u32 __cvt_fp2unsigned(double);
/* RankType-indexed float multipliers from retail .rodata:0x8050715C; CalcCountKey
   indexes them with the owner lookup byte (mask allows past the end on
   purpose, same as the equip item box's F32Record idiom). */
struct CIBLF32Rec {
    float f[7];
};
static const CIBLF32Rec sItemBoxLineRankMultipliers = {{ 0.0f, 1.0f, 1.2f, 1.5f, 1.8f, 2.0f, 2.5f }}; // name-guess: rankType-indexed multipliers from retail .rodata:0x8050715C
extern "C" u32 ItemBoxLine_CalcCountKey(void* self, unsigned int itemData) {
    u32 itemId = LookupItemTypeTable(itemData);
    u32 kind = BdatGetItemType(itemData);
    u16 cat = BdatGetItemId(itemData);
    u32 result = 0;
    if ((u16)kind == 3) {
        u8 v = BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData, "rankType", itemData);
        u16 count = BdatGetU16Direct((const void*)itemId, "money", cat);
        CIBLF32Rec rec = sItemBoxLineRankMultipliers;
        result = __cvt_fp2unsigned((double)((float)count * rec.f[v & 0xFF]));
    } else if ((u16)kind == 9 || (u16)kind < 2 || (u16)kind > 8) {
        u16 count = BdatGetU16Direct((const void*)itemId, "money", cat);
        result = count;
    } else {
        u16 count = BdatGetU16Direct((const void*)itemId, "money", cat);
        u8 n = BdatGetU8Direct((const void*)itemId, "jwl_slot", cat);
        float inc = lbl_eu_80668114;
        float step = 0.2f;
        do {
            inc += step;
        } while (n-- != 0);
        result = __cvt_fp2unsigned((double)((float)count * inc));
    }
    return result * 10;
}

// ============================================================================
// ItemBoxLine_BuildLineLayout: build the item-box line layout - load the layout arc, bind
// the two animation transforms, stop all running animations, then run the
// cursor deactivation tail.
// ============================================================================
void CIBLTabCur::initLayout() {
    buildLayout(&mpLayout, mArcResAcc, "mf00_reg00_curs07.brlyt");
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans0, mArcResAcc, "mf00_reg00_curs07_roop.brlan");
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans1, mArcResAcc, "mf00_reg00_curs07_on.brlan");
    mpLayout->UnbindAllAnimation();
    ((CBaseCur*)(this))->DeactivateTail();
}

void ItemBoxLine_InitTabFormat(CIBLTabFull* self);

// CItemBoxLine constructor (retail __ct__12CItemBoxLineFUsUc). After the member
// initialisation the retail body re-initialises mInfo2, mNumSel, mScrollBar,
// mSysWin and the tab table by assignment from temporaries.
CItemBoxLine::CItemBoxLine(u16 pageId, u8 mode)
    : field24(0), field28(0), field2C(0), field30(0), field34(0), field38(0), field3C(0),
      field40(0), field44(0), field48(0), field4C(0), field50(0), field54(0), field58(0),
      unk59(1), tabCount(0), field6D(0), mCur70(0), mCur88(0), mCurA0(0), mCurB8(0),
      mInfo2(0, 0), mNumSel(1), mScrollBar(0), mSysWin(0), unk38C(0), unk38E(0), field390(mode),
      field392(1), field394(pageId), unk39E(0), field39F(0), field3A0(0), field3A1(0),
      field3A2(0), field3A3(1) {
    ItemBoxLine_InitTabFormat(reinterpret_cast<CIBLTabFull*>(&unk3A4));
    memset(tabEntries, 0, 9);
    memset(unk64, 0, 9);
    u8 kind = 0;
    if (mode == 1) {
        kind = 2;
    } else if (mode == 2) {
        kind = 3;
    }
    mInfo2 = CItemBoxInfo2(kind, field394);
    mNumSel = CNumSelect(0);
    mScrollBar = CScrollBar(0);
    mSysWin = CSysWin(0);
    CIBLTabFull tab;
    ItemBoxLine_InitTabFormat(&tab);
    *reinterpret_cast<CIBLTabFull*>(&unk3A4) = tab;
    gItemBoxLineMnuItemMesATable = gItemBoxLineMnuItemMesBTable = gItemBoxLineMnuSkillTable = 0;
}

// ============================================================================
// CItemBoxLine destructor (retail __dt__12CItemBoxLineFv): destroy the
// embedded sub-objects in reverse construction order. The +0xA0 cursor is
// destroyed with the 0 delete-flag through a null guard (MWCC D2-into-D1
// artifact, same shape as ~COption's mSubCur2); the rest use the -1
// embedded-member flag. MWCC supplies the this-null check, the flags>0
// delete guard and the stmw/lmw frame from the dtor shape itself.
// ============================================================================
CItemBoxLine::~CItemBoxLine() {}

// ============================================================================
// ItemBoxLine_LoadFiles: load the item-box line's four files (MEM2 handle via
// getHandleMEM2, the third through the common archive) with this as the load
// event, then initialise the info2 state, num-select and scroll-bar members,
// and finally dispatch the syswin's last vtable slot (index 32) to finish.
// ============================================================================
void CItemBoxLine::LoadFiles() {
    field24 = (u32)CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), "menu/jp/ItemBoxLine.arc", this, 0, 0);
    field28 = (u32)CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), "menu/jp/tpl/ItemIcon.arc", this, 0, 0);
    field2C = (u32)CDeviceFile::readCommonArchiveFile(
        (u32)KyoshinHeap_GetField44(), "common/jp/bdat_menu_item.bin", this, 0, 0);
    field30 = (u32)CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), "common/jp/bdat_menu_mes.bin", this, 0, 0);
    mInfo2.LoadFiles();
    mNumSel.LoadFiles();
    mScrollBar.loadLayoutArc();
    mSysWin.loadSystemArc();
}

void CItemBoxLine::UpdateStates() {
    if (field4C == 0) return;
    switch (field50) {
    case 0: EnterState0(); break;
    case 1: EnterState1(); break;
    case 2: EnterState2(); break;
    case 3: EnterState3(); break;
    case 4:
        if (mNumSel.IsIdle()) {
            field50 = 7;
        }
        break;
    case 5:
        if (mNumSel.IsIdle()) {
            field50 = 3;
        }
        break;
    case 6: PushSyswinCursor(); break;
    // states 7..0xB share the syswin-commit body and then fall through into
    // the per-frame tail (the switch exit is the tail start in retail).
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        CommitOverlayState();
    }
    field40->Animate(0);
    mCur70.Update();
    mCur88.Update();
    mCurA0.Update();
    mCurB8.Update();
    mInfo2.UpdateAnims();
    mNumSel.UpdateStates();
    mScrollBar.UpdateDispatch();
    reinterpret_cast<CSysWin*>(&mSysWin)->dispatchPhase();
}

void CItemBoxLine::DrawLayout(nw4r::lyt::DrawInfo* drawInfo) {
    if (field4C == 0) return;
    mInfo2.DrawLayout(drawInfo);
    drawLayout(field40, drawInfo, 0, 1);    // "line not busy" flag: both the tab-name cursor and the num-select idle.
    int noBusy = 0;
    if (mCurB8.BothSubPanesOn() == 0 && mNumSel.IsOpen() == 0) noBusy = 1;
    if (noBusy != 0) {
        if (reinterpret_cast<CSysWin*>(&mSysWin)->isOpened() == 0) {
            mCur88.DrawLayout(drawInfo);
            mCurA0.DrawLayout(drawInfo);
        }
    }
    mCurB8.DrawLayout(drawInfo);
    mScrollBar.draw(drawInfo);
    mNumSel.DrawLayout(drawInfo);
    reinterpret_cast<CSysWin*>(&mSysWin)->drawLayout(reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo));
    // "syswin overlay armed" flag: overlay busy and nav state before 9.
    int syswinArmed = 0;
    if (reinterpret_cast<CSysWin*>(&mSysWin)->isOpened() != 0 && field50 < 9) syswinArmed = 1;
    if (noBusy != 0 && syswinArmed == 0) {
        mCur70.DrawLayout(drawInfo);
    }
}

// ============================================================================
// ItemBoxLine_UnloadFiles: teardown of the loaded item-box line. Stop the two UI sounds,
// release the four file handles, free the scratch buffer and the two heap
// objects (virtual deleting-dtor, double-null-checked), destroy the two arc
// resource accessors and the two 16-byte mem regions at +0x04/+0x14, silence
// the four cursors, then destroy the embedded sub-systems.
// ============================================================================
void CItemBoxLine::UnloadFiles() {
    CBdat::getEntry(2);
    CBdat::getEntry(5);
    closeFileHandle(reinterpret_cast<CFileHandle**>(&field24));
    closeFileHandle(reinterpret_cast<CFileHandle**>(&field28));
    closeFileHandle(reinterpret_cast<CFileHandle**>(&field2C));
    closeFileHandle(reinterpret_cast<CFileHandle**>(&field30));
    field4C = 0;
    if (field34 != 0) {
        mtl::MemManager::deallocate(field34);
        field34 = 0;
    }
    nw4r::lyt::Layout* layout40 = field40;
    if (layout40 != 0) {
        delete layout40;
        field40 = 0;
    }
    releaseArcResourceAccessor(field38);
    releaseArcResourceAccessor(field3C);
    if (field54 != 0) {
        delete field54;
        field54 = 0;
    }
    (&mRegion04)->deleteRegion();
    (&mRegion14)->deleteRegion();
    mCur70.cleanup();
    mCur88.cleanup();
    ((CBaseCur*)&mCurA0)->cleanup();
    ((CBaseCur*)&mCurB8)->cleanup();
    mInfo2.UnloadFiles();
    mNumSel.UnloadFiles();
    mScrollBar.Teardown();
    reinterpret_cast<CSysWin*>(&mSysWin)->termLayout();
}


// ============================================================================
// ItemBoxLine_IsBusy: busy-check - a CSysWin or num-select member is active, else return byte.
// ============================================================================
int CItemBoxLine::IsBusy() {
    if (reinterpret_cast<CSysWin*>(&mSysWin)->isOpened()) {
        return 1;
    }
    if (mNumSel.IsOpen()) {
        return 1;
    }
    return field3A0;
}

void CItemBoxLine::OpenLine() {
    if (field50 != 0) return;
    field50 = 1;
    field4C = 1;
    unk59 = 0;
    unk39E = 0;
    mInfo2.Open();
    // Four values stay live across the calls below (this / tabs / idx / f9):
    // retail keeps them in r28..r31 behind a _savegpr_28 prologue.
    CIBLTab* tabs = &unk3A4;
    u8 idx = (u8)(unk38C + unk38E);
    int f9 = ItemBoxLine_GetTabEntryReady(tabs, idx);
    mInfo2.SetItem((u16)ItemBoxLine_GetTabEntryItem(tabs, idx), 0, field39F, f9);
    mInfo2.SetNamedText(0, (char*)ItemBoxLine_FormatTabEntryName((CIBLTabFormat*)tabs, idx));
    SetInfo2Item((u16)ItemBoxLine_GetTabEntryItem(tabs, idx));
    float vec[3];
    mScrollBar.InitRootPane(reinterpret_cast<const nw4r::math::VEC3*>(code80135FDC_setVec3((nw4r::math::VEC3*)vec, 276.0f, 53.0f, lbl_eu_806680F8)));
    mScrollBar.UpdateThumb(7, tabs->count);
    mScrollBar.requestScrollIn();
}

void CItemBoxLine::PushTabByte(unsigned char val) {
    unsigned char n = reinterpret_cast<unsigned char*>(this)[0x63];
    if (n >= 9) {
        return;
    }
    tabEntries[n] = val;
    reinterpret_cast<unsigned char*>(this)[0x63] = n + 1;
}

void CItemBoxLine::TabNext() {
    if (mNumSel.IsOpen()) return;
    field6D = field6D + 1;
    if ((s8)field6D >= (int)tabCount) {
        field6D = 0;
    }
    if (unk64[(s8)field6D] == 0) {
        u8 count = tabCount;
        int j = (s8)(field6D + 1);
        if ((s8)j >= (int)count) j = 0;
        for (u8 i = 0; i < count; i++) {
            if (unk64[(s8)j] != 0) {
                field6D = (u8)j;
                break;
            }
            j = j + 1;
            if ((s8)j >= (int)count) j = 0;
        }
    }
    unk38E = 0;
    RefreshTabLabels(0);
    RefreshLineState();
    RefreshLineRows();
    playUISound(0x70);
    mScrollBar.UpdateThumb(7, unk3A4.count);
}

void CItemBoxLine::TabPrev() {
    if (mNumSel.IsOpen()) return;
    field6D = field6D - 1;
    if ((s8)field6D < 0) {
        field6D = tabCount - 1;
    }
    if (unk64[(s8)field6D] == 0) {
        // j-init (with its own tabCount reload for the wrap) precedes the count
        // load, matching retail scheduling; 'last' is only referenced inside
        // the loop.
        int j = (s8)(field6D - 1);
        if ((s8)j < 0) j = (s8)(tabCount - 1);
        u8 count = tabCount;
        int last = count - 1;
        for (u8 i = 0; i < count; i++) {
            if (unk64[(s8)j] != 0) {
                field6D = (u8)j;
                break;
            }
            j = j - 1;
            if ((s8)j < 0) j = (s8)last;
        }
    }
    unk38E = 0;
    RefreshTabLabels(1);
    RefreshLineState();
    RefreshLineRows();
    playUISound(0x70);
    mScrollBar.UpdateThumb(7, unk3A4.count);
}

// ============================================================================
// ItemBoxLine_CursorPageBack: tab page-up / cursor-back interaction - the mirror of
// ItemBoxLine_CursorPageDown. Syswin overlay open (and nav state 9, active): step the
// overlay selection byte down (wrapping 0 -> 1) and push the syswin selection
// buffer through the line cursor's vtable[2]; tab cursor active: scan the
// page list backward for the previous occupied slot (wrap at 4) and push its
// VEC3 through mCur88; num-select busy: advance field392 (resetting to 1 past
// the entry's capacity) and repush value*step; otherwise page the tab line up
// by one row (wrapping through the tab count) and refresh the line.
// ============================================================================
void CItemBoxLine::CursorPageBack() {
    if (reinterpret_cast<CSysWin*>(&mSysWin)->isOpened() != 0) {
        if (field50 >= 9) {
            if (reinterpret_cast<CSysWin*>(&mSysWin)->isActive() != 0) {
                u8 v = field3A3 - 1;
                field3A3 = v;
                if ((s8)v < 0) {
                    field3A3 = 1;
                }
                u8 tmp[12];
                CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(tmp), reinterpret_cast<CSysWin*>(&mSysWin), field3A3);
                mCur70.setRootPaneTranslate((const nw4r::math::VEC3*)(tmp));
                playUISound(1);
            }
        }
        return;
    }
    if (field3A0 != 0) {
        CItemBoxPage* page = &mInfo2.mPage;
        s8 a = (s8)(field3A2 - 1);
        while (a != (s8)field3A2) {
            if (a < 0) a = 3;
            if (ArrayGet12(page->mItemIds, (u8)((s8)field3A1 * 4 + a)) != 0) {
                field3A2 = (u8)a;
                break;
            }
            a--;
        }
        CIBLVec3 vec;
        CopyTabSlotVec(&vec, page->mItemIds, (u8)((s8)field3A1 * 4 + field3A2));
        mCur88.setRootPaneTranslate((const nw4r::math::VEC3*)(&vec));
    } else if (mNumSel.IsOpen() != 0) {
        if (mNumSel.IsIdle() == 0) return;
        u8 idx = (u8)(unk38C + unk38E);
        CIBLTab* tabs = &unk3A4;
        u32 lim = ItemBoxLine_ProbePageSlot((void*)tabs, idx);
        field392 += 1;
        if ((u32)field392 > lim) field392 = 1;
        mNumSel.CountUp();
        mNumSel.SetCount((u8)field392);
        mNumSel.SetTotal(field392 * ItemBoxLine_GetTabEntryValue(tabs, idx));
    } else {
        if (unk38C == 0 && unk38E == 0) {
            unk38C = -1;
        } else {
            unk38C--;
            if ((s16)unk38C < 0) {
                unk38C = 0;
                unk38E--;
                if ((s16)unk38E < 0) {
                    u16 count = unk3A4.count;
                    if (count >= 7) {
                        unk38C = 6;
                        unk38E = count - 7;
                    } else {
                        unk38C = count - 1;
                        unk38E = 0;
                        if ((s16)unk38C < 0) {
                            unk38C = 0;
                        }
                    }
                }
            }
        }
        RefreshLineRows();
        RefreshTabPanes();
        RefreshPageList();
        mScrollBar.PlaceThumb((u16)unk38E);
    }
    playUISound(1);
}

// ============================================================================
// ItemBoxLine_CursorPageDown: tab page-down / tab-cursor interaction. When the syswin
// overlay is open and the nav state is in the overlay region (>= 9), advance
// the overlay selection byte (wrapping at 2) and push the syswin selection
// buffer through the cursor's vtable[2]; when the tab cursor is active, scan
// the page list for the previous occupied slot (wrap at 4) and push its VEC3;
// when the num-select is busy, step the page position down by 1 clamped to
// the entry's capacity; otherwise page the tab line down by 7 (wrap at the
// tab count) and refresh the line.
// ============================================================================
void CItemBoxLine::CursorPageDown() {
    if (reinterpret_cast<CSysWin*>(&mSysWin)->isOpened() != 0) {
        if (field50 >= 9) {
            if (reinterpret_cast<CSysWin*>(&mSysWin)->isActive() != 0) {
                field3A3++;
                if ((s8)field3A3 > 1) {
                    field3A3 = 0;
                }
                u8 tmp[12];
                CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(tmp), reinterpret_cast<CSysWin*>(&mSysWin), field3A3);
                mCur70.setRootPaneTranslate((const nw4r::math::VEC3*)(tmp));
                playUISound(1);
            }
        }
        return;
    }
    if (field3A0 != 0) {
        CItemBoxPage* page = &mInfo2.mPage;
        s8 a = (s8)(field3A2 + 1);
        while (a != (s8)field3A2) {
            if (a >= 4) a = 0;
            if (ArrayGet12(page->mItemIds, (u8)((s8)field3A1 * 4 + a)) != 0) {
                field3A2 = (u8)a;
                break;
            }
            a++;
        }
        CIBLVec3 vec;
        CopyTabSlotVec(&vec, page->mItemIds, (u8)((s8)field3A1 * 4 + field3A2));
        mCur88.setRootPaneTranslate((const nw4r::math::VEC3*)(&vec));
    } else if (mNumSel.IsOpen() != 0) {
        if (mNumSel.IsIdle() == 0) return;
        u8 idx = (u8)(unk38C + unk38E);
        CIBLTab* tabs = &unk3A4;
        s16 lim = (s16)ItemBoxLine_ProbePageSlot((void*)tabs, idx);
        field392 -= 1;
        if ((s16)field392 < 1) field392 = lim;
        mNumSel.CountDown();
        mNumSel.SetCount((u8)field392);
        mNumSel.SetTotal(field392 * ItemBoxLine_GetTabEntryValue(tabs, idx));
    } else {
        u16 count = unk3A4.count;
        if (count >= 7) {
            if (unk38C == 6 && unk38E == count - 7) {
                unk38C = -1;
                unk38E = 0;
            } else {
                unk38C++;
                if (unk38C >= 7) {
                    unk38C = 6;
                    unk38E++;
                    if (unk38E > count - 7) unk38E = count - 7;
                }
            }
        } else {
            int last = count - 1;
            if (unk38C == last) {
                unk38C = -1;
                unk38E = 0;
            } else {
                unk38C++;
                if (unk38C >= count) {
                    unk38C = last;
                    unk38E = 0;
                }
            }
        }
        RefreshLineRows();
        RefreshTabPanes();
        RefreshPageList();
        mScrollBar.PlaceThumb((u16)unk38E);
    }
    playUISound(1);
}

// ============================================================================
// ItemBoxLine_CursorPageUp: per-frame tab/cursor interaction. When the tab cursor is
// active, scan the page list for the next non-empty slot and push its VEC3
// to the cursor; when the num-select is busy, step its page position;
// otherwise handle the tab page up/down wrap and refresh the line.
// ============================================================================
void CItemBoxLine::CursorPageUp() {
    if (reinterpret_cast<CSysWin*>(&mSysWin)->isOpened() != 0) return;
    if (field3A0 != 0) {
        CItemBoxPage* page = &mInfo2.mPage;
        s8 a = (s8)(field3A1 - 1);
        while (a != (s8)field3A1) {
            if (a < 0) a = 2;
            if (ArrayGet12(page->mItemIds, (u8)(field3A2 + (s8)a * 4)) != 0) {
                field3A1 = (u8)a;
                break;
            }
            a--;
        }
        CIBLVec3 vec;
        CopyTabSlotVec(&vec, page->mItemIds, (u8)(field3A2 + (s8)field3A1 * 4));
        mCur88.setRootPaneTranslate((const nw4r::math::VEC3*)(&vec));
        playUISound(1);
        return;
    }
    if (mNumSel.IsOpen() != 0) {
        if (mNumSel.IsIdle() == 0) return;
        u8 idx = (u8)(unk38C + unk38E);
        CIBLTab* tabs = &unk3A4;
        if (ItemBoxLine_ProbePageSlot((void*)tabs, idx) != 0) {
            field392 -= 10;
            if (field392 < 1) field392 = 1;
        }
        mNumSel.SetCount((u8)field392);
        mNumSel.SetTotal(field392 * ItemBoxLine_GetTabEntryValue(tabs, idx));
        playUISound(1);
    } else {
        if (unk38C == -1) {
            TabPrev();
            return;
        }
        if (unk3A4.count >= 7) {
            unk38E -= 7;
            if (unk38E < 0) {
                unk38C = unk38E + 6;
                unk38E = 0;
                if (unk38C < 0) unk38C = 0;
            }
        } else {
            unk38C = 0;
            unk38E = 0;
        }
        RefreshLineRows();
        RefreshTabPanes();
        RefreshPageList();
        mScrollBar.PlaceThumb((u16)unk38E);
        playUISound(1);
    }
}

// ============================================================================
// ItemBoxLine_CursorPageDownRow: page-up mirror of ItemBoxLine_CursorPageUp. When the tab cursor is
// active, scan the page list for the next non-empty slot in the up direction
// (wrap >= 3 back to 0) and push its VEC3 to the cursor; when the num-select
// is busy, step its page position up by 10 clamped to the page's capacity;
// otherwise handle the tab page-up wrap (unk38E += 7) and refresh the line.
// ============================================================================
void CItemBoxLine::CursorPageDownRow() {
    if (reinterpret_cast<CSysWin*>(&mSysWin)->isOpened() != 0) return;
    if (field3A0 != 0) {
        CItemBoxPage* page = &mInfo2.mPage;
        s8 a = (s8)(field3A1 + 1);
        while (a != (s8)field3A1) {
            if (a >= 3) a = 0;
            if (ArrayGet12(page->mItemIds, (u8)(field3A2 + (s8)a * 4)) != 0) {
                field3A1 = (u8)a;
                break;
            }
            a++;
        }
        CIBLVec3 vec;
        CopyTabSlotVec(&vec, page->mItemIds, (u8)(field3A2 + (s8)field3A1 * 4));
        mCur88.setRootPaneTranslate((const nw4r::math::VEC3*)(&vec));
        playUISound(1);
        return;
    }
    if (mNumSel.IsOpen() != 0) {
        if (mNumSel.IsIdle() == 0) return;
        u8 idx = (u8)(unk38C + unk38E);
        CIBLTab* tabs = &unk3A4;
        u8 lim = ItemBoxLine_ProbePageSlot((void*)tabs, idx);
        if (lim != 0) {
            field392 += 10;
            if ((u32)field392 > (u32)lim) field392 = (s16)lim;
        }
        mNumSel.SetCount((u8)field392);
        mNumSel.SetTotal(field392 * ItemBoxLine_GetTabEntryValue(tabs, idx));
        playUISound(1);
        return;
    }
    if (unk38C == -1) {
        TabNext();
        return;
    }
    u16 count = unk3A4.count;
    if (count >= 7) {
        int t = (int)count - 7;
        unk38E += 7;
        if (unk38E > t) {
            unk38C = unk38E - t;
            unk38E = t;
            if (unk38C >= 7) unk38C = 6;
        }
    } else {
        unk38C = (s16)(count - 1);
        unk38E = 0;
        if ((s16)(count - 1) < 0) unk38C = 0;
    }
    RefreshLineRows();
    RefreshTabPanes();
    RefreshPageList();
    mScrollBar.PlaceThumb((u16)unk38E);
    playUISound(1);
}

// ============================================================================
// ItemBoxLine_ConfirmOverlayOrHint: item-box line update. When the syswin overlay is armed the
// active tab is committed; otherwise a tab/cursor hint is advanced.
// ============================================================================
void CItemBoxLine::ConfirmOverlayOrHint() {
    if (reinterpret_cast<CSysWin*>(&mSysWin)->isOpened()) {
        if (reinterpret_cast<CSysWin*>(&mSysWin)->isActive()) {
            reinterpret_cast<CSysWin*>(&mSysWin)->advancePhase3();
            if (field50 >= 9) {
                field50 = 0xb;
                field3A3 = 1;
                RefreshLineRows();
            }
        }
    } else {
        if (field3A0 != 0) {
            field3A0 = 0;
            if (unk38C == -1) {
                mCurA0.SetVisible(1);
            } else {
                mCur70.SetVisible(1);
            }
            mCur88.SetVisible(0);
            playUISound(6);
        } else {
            if (mNumSel.IsOpen() == 0) return;
            if (mNumSel.IsIdle() == 0) return;
            mNumSel.Close();
            field50 = 8;
            playUISound(6);
        }
    }
}

// ============================================================================
// ItemBoxLine_ResolveEntry: item-box line input/nav state machine. With the syswin
// overlay open: commit the overlay selection once state reaches 9+. Otherwise
// ignore while no tab is selected (-1) or the cursor hint is armed; when the
// num-select is confirming (field390==1 + busy + state 7) rebuild the tab list
// for the confirmed entry and settle into state 8. Otherwise resolve the
// highlighted entry: occupied slots beep, entries with page data open the
// syswin detail pane with kind-range-dependent text, armed pages either
// dispatch likewise or raise the "unavailable" flag (state 3 beep).
// ============================================================================
void CItemBoxLine::ResolveEntry() {
    if (reinterpret_cast<CSysWin*>(&mSysWin)->isOpened()) {
        if (reinterpret_cast<CSysWin*>(&mSysWin)->isActive()) {
            reinterpret_cast<CSysWin*>(&mSysWin)->advancePhase3();
            if (field50 < 9) return;
            field50 = 0xb;
            RefreshLineRows();
        }
        return;
    }
    if (unk38C == -1) return;
    if (field3A0 != 0) return;
    CIBLTab* tabs = &unk3A4;
    u32 key = (u8)(unk38C + unk38E);
    if (field390 == 1 && mNumSel.IsOpen()) {
        // Num-select confirm: rebuild the tab list for the confirmed entry.
        if (mNumSel.IsIdle() == 0) return;
        if (field50 != 7) return;
        ItemBoxLine_AllocEntryStock(reinterpret_cast<CIBLTabFull*>(tabs), key, (u8)field392);
        u8 v = GetCollectedFlagByte(field39F);
        ItemBoxLine_BuildTabEntries(tabs, tabEntries[(s8)field6D], field394, (u8)v);
        RefreshLineState();
        RebuildTabList();
        RefreshTabPanes();
        RefreshPageList();
        mNumSel.Close();
        field50 = 8;
        playUISound(0x2f);
        return;
    }
    if (ItemBoxLine_GetTabEntryItem(tabs, key) == 0) return;
    if (ItemBoxLine_GetTabEntryLock(tabs, key) != 0) {
        playUISound(5);
        return;
    }
    if (ItemBoxLine_ProbePageSlot(reinterpret_cast<CIBLTabFull*>(tabs), key) == 0) {
        // No page data: open the syswin detail pane with kind-range text.
        char* text = NULL;
        u16 f0 = ItemBoxLine_GetTabEntryItem(tabs, key);
        u16 kv = (u16)BdatGetItemType(f0);
        BdatGetItemId(f0);
        if ((u32)(kv - 2) <= 7) {
            text = BdatTouchStringCell("MNU_shop", "name", 0x11);
        } else if ((u32)(kv - 0xa) <= 3) {
            text = (CItem_sumFamilyByte6(f0) >= 1)
                       ? BdatTouchStringCell("MNU_sysmes", "name", 3)
                       : BdatTouchStringCell("MNU_shop", "name", 0x11);
        }
        reinterpret_cast<CSysWin*>(&mSysWin)->switchKindPane(0);
        reinterpret_cast<CSysWin*>(&mSysWin)->setKindContent(reinterpret_cast<char*>((u32)text), 0);
        reinterpret_cast<CSysWin*>(&mSysWin)->applyKind(1);
        reinterpret_cast<CSysWin*>(&mSysWin)->openPhase1();
        playUISound(5);
        return;
    }
    // Armed page: name-format tabs probe availability through ItemBoxLine_ProbeAvailability,
    // other kinds go through the character-data lookups below.
    if (tabEntries[(s8)field6D] == 0xd) {
        u16 f0d = ItemBoxLine_GetTabEntryItem(tabs, key);
        if (ProbeAvailability(f0d) != 0) {
            char* shopText = BdatTouchStringCell("MNU_shop", "name", 24);
            char* partyLow = BdatTouchStringCell("MNU_party", "name", 13);
            char* partyHigh = BdatTouchStringCell("MNU_party", "name", 14);
            mSysWin.switchKindPane(2);
            mSysWin.setKindContent(shopText, 0);
            mSysWin.setTwoTextValues(partyLow, partyHigh);
            mSysWin.applyKind(0);
            mSysWin.openPhase1();
            unk39E = 1;
            mCur70.SetVisible(0);
            field50 = 9;
            playUISound(3);
            return;
        }
    }
    int unavailable = 0;
    char* text = NULL;
    u16 f0 = ItemBoxLine_GetTabEntryItem(tabs, key);
    u16 kv = (u16)BdatGetItemType(f0);
    BdatGetItemId(key);
    if ((u32)(kv - 2) <= 7) {
        if (CItemBlock_countEmpty((u8)kv) != 0) {
            unavailable = 1;
        } else {
            text = BdatTouchStringCell("MNU_shop", "name", 0x11);
        }
    } else if ((u32)(kv - 0xa) <= 3) {
        u32 t = CItem_sumFamilyByte6(kv);
        if (t >= 1) {
            if (t >= 0x63) {
                text = BdatTouchStringCell("MNU_sysmes", "name", 3);
            } else {
                unavailable = 1;
            }
        } else if (CItemBlock_countEmpty((u8)kv) != 0) {
            unavailable = 1;
        } else {
            text = BdatTouchStringCell("MNU_shop", "name", 0x11);
        }
    }
    if (unavailable) {
        unk39E = 1;
        playUISound(3);
    } else {
        reinterpret_cast<CSysWin*>(&mSysWin)->switchKindPane(0);
        reinterpret_cast<CSysWin*>(&mSysWin)->setKindContent(reinterpret_cast<char*>((u32)text), 0);
        reinterpret_cast<CSysWin*>(&mSysWin)->applyKind(1);
        reinterpret_cast<CSysWin*>(&mSysWin)->openPhase1();
        playUISound(5);
    }
}

// ============================================================================
// ItemBoxLine_ShowDetailWin: item-box line syswin/tab interaction. When the tab cursor is
// active and the syswin overlay is open, advance the syswin; otherwise open
// the detail window for the current page slot (name via the slot flag: kind-3
// slots use the item-name provider, others the shared table). When the tab
// cursor is idle, scan the page list for the first occupied slot (preferring
// slot 8), decompose it into page/column (field3A1 = slot/4, field3A2 =
// slot%4 via a float multiply), prime the cursors and push the slot VEC3.
// ============================================================================
void CItemBoxLine::ShowDetailWin() {
    if (mNumSel.IsOpen() != 0) return;
    if (field3A0 != 0) {
        if (mSysWin.isOpened() != 0) {
            if (mSysWin.isActive() != 0) {
                mSysWin.advancePhase3();
            }
            return;
        }
        CItemBoxPage* page = &mInfo2.mPage;
        u8 idx = (u8)(field3A2 + (s8)field3A1 * 4);
        u16 v = ArrayGet12(page->mItemIds, idx);
        if (v != 0) {
            u8 bt = ItemBoxGetTab(page, idx);
            char* obj;
            if (bt == 3) {
                obj = page->GetItemName(idx);
            } else {
                obj = BdatTouchStringCell("MNU_skill", "help", (u16)v);
            }
            mSysWin.switchKindPane(0);
            mSysWin.setKindContent(obj, 0);
            mSysWin.applyKind(1);
            mSysWin.openPhase1();
        }
        return;
    }
    if (mSysWin.isOpened() != 0) return;
    CItemBoxPage* page2 = &mInfo2.mPage;
    u8 found = 0;
    for (u8 i = 0; i < 12; i++) {
        if (page2->mItemIds[i] != 0) {
            page2->mSelSlot = i;
            if (page2->mItemIds[8] != 0) page2->mSelSlot = 8;
            found = 1;
            break;
        }
    }
    if (found != 0) {
        u8 sel = page2->mSelSlot;
        field3A0 = 1;
        field3A1 = (u8)((float)sel * 0.25f);
        field3A2 = (u8)(sel - (s8)field3A1 * 4);
        mCurA0.SetVisible(0);
        mCur70.SetVisible(0);
        mCur88.SetVisible(1);
        CIBLVec3 vec;
        CopyTabSlotVec(&vec, page2->mItemIds, sel);
        mCur88.setRootPaneTranslate((const nw4r::math::VEC3*)(&vec));
        playUISound(2);
    } else {
        playUISound(5);
    }
}

// ============================================================================
// ItemBoxLine_DrawTabRowText:
// its texture name via the shared table (::cf::CfBdat::spItmItemListFileData), try object A
// (+0x3C) first and fall back to object B (+0x38) with the default name; with
// no key id, B is used directly. If a resource was found, format the pane
// name from arg3 and bind the resource to the active layout.
// ============================================================================
void CItemBoxLine::DrawTabRowText(u32 arg2, u32 arg3) {
    u32 result;
    if (arg2 != 0) {
        u32 v = BdatGetU16Direct((const void*)::cf::CfBdat::spItmItemListFileData, "icon_base", arg2);
        result = (u32)((nw4r::lyt::ArcResourceAccessor*)field3C)->GetResource(0x74696d67, MakeTplNameSysFile((u16)v), 0);
        if (result == 0) {
            result = (u32)((nw4r::lyt::ArcResourceAccessor*)field38)->GetResource(0x74696d67, "mf00_com00_dmy.tpl", 0);
        }
    } else {
        result = (u32)((nw4r::lyt::ArcResourceAccessor*)field38)->GetResource(0x74696d67, "mf00_com00_dmy.tpl", 0);
    }
    if (result != 0) {
        char buf[0x20];
        sprintf(buf, "pic_btnbs%02d", arg3 + 1);
        PaneSetTexPaletteByName(field40, (const char*)buf, (TPLPalette*)result);
    }
}

void CItemBoxLine::DrawTabRowIcon(u32 arg2, u32 arg3) {
    u32 result;
    if (arg2 != 0) {
        u32 v = BdatGetU16Direct((const void*)::cf::CfBdat::spItmItemListFileData, "icon", arg2);
        result = (u32)((nw4r::lyt::ArcResourceAccessor*)field3C)->GetResource(0x74696d67, MakeTplNameSysFile((u16)v), 0);
        if (result == 0) {
            result = (u32)((nw4r::lyt::ArcResourceAccessor*)field38)->GetResource(0x74696d67, "mf00_com00_dmy.tpl", 0);
        }
    } else {
        result = (u32)((nw4r::lyt::ArcResourceAccessor*)field38)->GetResource(0x74696d67, "mf00_com00_dmy.tpl", 0);
    }
    if (result != 0) {
        char buf[0x20];
        sprintf(buf, "pic_icon%02d", arg3 + 1);
        PaneSetTexPaletteByName(field40, (const char*)buf, (TPLPalette*)result);
    }
}

// ============================================================================
// ItemBoxLine_FormatName: format the item-slot name into the line. The kind selector
// (BdatGetItemType) picks the item-name path (kinds 3/9) which resolves the
// name via the owner table; otherwise a signed hint byte picks one of the
// fixed strings / name providers.
// ============================================================================
void CItemBoxLine::FormatName(u32 arg4, s8 arg5, u32 arg6) {
    char buf[0x20];
    sprintf(buf, "txt_num%02d", arg6 + 1);
    ml::FixStr<32> str;
    str.clear();
    u8 sel = (u8)BdatGetItemType(arg4);
    if (sel == 3 || sel == 9) {
        u8 v = BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData, "rankType", arg4);
        char* name = BdatTouchStringCell("MNU_item", "name", 0x1e - (v - 1));
        str.format("%s", name);
        PaneMatSetTevColorsByName(field40, buf, &itemBoxLine_pageLabelNeutralTop, &itemBoxLine_pageLabelNeutralBottom);
    } else if (arg5 > 0) {
        str.format("%d");
        PaneMatSetTevColorsByName(field40, buf, &itemBoxLine_pageLabelNeutralTop, &itemBoxLine_pageLabelNeutralBottom);
    } else if (arg5 == 0) {
        const char* s = "";
        str = s;
    } else if (arg5 == -2) {
        str = getErrMesText16();
        PaneMatSetTevColorsByName(field40, buf, &itemBoxLine_pageLabelSelectedTop, &itemBoxLine_pageLabelSelectedBottom);
    } else if (arg5 == -3) {
        str = getErrMesText15();
        PaneMatSetTevColorsByName(field40, buf, &itemBoxLine_pageValueSelectedTop, &itemBoxLine_pageValueSelectedBottom);
    } else {
        const char* s = "";
        str = s;
    }
    LayoutSetTextBoxFmtValue(field40, buf, str.mString, 0);
}

// ============================================================================
// ItemBoxLine_BindSlotTexture: bind the per-slot tab texture and nameplate colour. Resolve
// the texture name by slot kind (kinds 1..12 format a fixed pool string,
// kind 0 uses the shared "timeg" fallback name directly), fetch the texture
// through resource object A (falling back to B), bind it to the pane named
// "%d"-style by index+1, and - unless the slot's occupancy byte is set -
// push the default nameplate colour through the pane's colour-source virtual.
// ============================================================================
void CItemBoxLine::BindSlotTexture(u8 kind, u32 index) {
    const u32 tag = 0x74696d67;
    char buf[0x20];
    u32 tex;
    if (kind == 0) {
        tex = (u32)((nw4r::lyt::ArcResourceAccessor*)field38)->GetResource(tag, "mf00_com00_dmy.tpl", 0);
    } else {
        switch (kind) {
        case 2:  sprintf(buf, "mf00_reg20_cat00.tpl"); break;
        case 3:  sprintf(buf, "mf00_reg20_cat07.tpl"); break;
        case 4:  sprintf(buf, "mf00_reg20_cat01.tpl"); break;
        case 5:  sprintf(buf, "mf00_reg20_cat02.tpl"); break;
        case 6:  sprintf(buf, "mf00_reg20_cat03.tpl"); break;
        case 7:  sprintf(buf, "mf00_reg20_cat04.tpl"); break;
        case 8:  sprintf(buf, "mf00_reg20_cat05.tpl"); break;
        case 9:  sprintf(buf, "mf00_reg20_cat06.tpl"); break;
        case 10:  sprintf(buf, "mf00_reg20_cat09.tpl"); break;
        case 11: sprintf(buf, "mf00_reg20_cat10.tpl"); break;
        case 12: sprintf(buf, "mf00_reg20_cat11.tpl"); break;
        case 13: sprintf(buf, "mf00_reg20_cat08.tpl"); break;
        }
        tex = (u32)((nw4r::lyt::ArcResourceAccessor*)field3C)->GetResource(tag, buf, 0);
            if (tex == 0) {
                tex = (u32)((nw4r::lyt::ArcResourceAccessor*)field38)->GetResource(tag, "mf00_com00_dmy.tpl", 0);
        }
    }
    if (tex != 0) {
        char nameB[0x20];
        sprintf(nameB, "pic_cat%02d", index + 1);
        PaneSetTexPaletteByName(field40, (const char*)nameB, (TPLPalette*)tex);
        if (unk64[index] == 0) {
            GXColorS10 col;
            col.b = 0x14;
            col.g = 0x14;
            col.r = 0x14;
            col.a = field396.a;
            nw4r::lyt::Pane* pane = field40->GetRootPane()->FindPaneByName(nameB, true);
            if (pane != NULL) {
                // +0x68 is Pane::GetMaterial; the material's mTevCols[0]
                // lives at +0x10 (see CIBLColorSrc).
                nw4r::lyt::Material* mat = pane->GetMaterial();
                if (mat != NULL) {
                    mat->SetTevColor(0, col);
                }
            }
        }
    }
}

// ============================================================================
// ItemBoxLine_SetPaneVisible: format the pane name (skipping the leading marker char of the
// caller's string) into a temp buffer, look the pane up on the layout root,
// and toggle its visibility from the caller's flag.
// ============================================================================
void CItemBoxLine::SetPaneVisible(u32 index, bool visible) {
    char buf[0x20];
    sprintf(buf, "pic_off%02d", index + 1);
    nw4r::lyt::Pane* root = field40->GetRootPane();
    nw4r::lyt::Pane* pane = root->FindPaneByName(buf, true);
    setPaneVisible(pane, !visible);
}

// ============================================================================
// ItemBoxLine_RebuildTabList: refresh the seven tab-slot panes (same shape as
// ItemBoxLine_RefreshTabLabels), then, when the slot is unoccupied, bind a texture resource
// (object A fallback path) onto the slot's pane name; finally dispatch the
// slot entry through ItemBoxLine_BindSlotTexture. r25 keeps tabEntries[i] across the calls.
// ============================================================================
void CItemBoxLine::RebuildTabList() {
    for (u8 i = 0; i < 7; i++) {
        char nameA[0x20];
        char nameB[0x20];
        sprintf(nameA, "pic_tbbcon%02d", (u8)i + 1);
        sprintf(nameB, "pic_tbbcof%02d", (u8)i + 1);
        bool visA = false;
        bool visB = false;
        u8 entry = tabEntries[i];
        if (entry != 0) {
            if ((u8)i == (s8)field6D) {
                visA = true;
                visB = false;
            } else {
                visA = false;
                visB = true;
            }
        }
        setPaneVisible(field40->GetRootPane()->FindPaneByName(nameA, true), visA);
        setPaneVisible(field40->GetRootPane()->FindPaneByName(nameB, true), visB);
        if (unk64[i] == 0) {
            u32 tex = (u32)((nw4r::lyt::ArcResourceAccessor*)field38)->GetResource(0x74696d67, "mf02_box00_tabemp.tpl", 0);
            if (tex != 0) {
                PaneSetTexPaletteByName(field40, (const char*)nameB, (TPLPalette*)tex);
            }
        }
        BindSlotTexture(entry, i);
    }
}

// ============================================================================
// ItemBoxLine_RefreshTabPanes: refresh the seven tab-slot panes.
// palette entries, then for each slot: pick the lit/unlit color pair from the
// palette by slot occupancy, refresh the pane texture/name/visibility, and for
// named tabs bind the tab name + per-pane animation colors (alpha from the
// PaneMatGetS16QuadByName animation vector) to the layout panes.
// ============================================================================
void CItemBoxLine::RefreshTabPanes() {
    CIBLTab* tabs = &unk3A4;
    GXColorS10 colors[12];
    setGXColorS10(&colors[0], 0xff, 0xff, 0xfa, 0);
    setGXColorS10(&colors[1], 0xff, 0xff, 0xfa, 0);
    setGXColorS10(&colors[2], 0x48, 0x3a, 0x21, 0);
    setGXColorS10(&colors[3], 0xff, 0xff, 0xfa, 0);
    setGXColorS10(&colors[4], 0xb7, 0xe, 0xe, 0);
    setGXColorS10(&colors[5], 0xb7, 0xe, 0xe, 0);
    setGXColorS10(&colors[6], 0x28, 0x5, 0x5, 0);
    setGXColorS10(&colors[7], 0xf7, 0x8e, 0x8e, 0);
    setGXColorS10(&colors[8], 0, 0, 0, 0);
    setGXColorS10(&colors[9], 0, 0, 0, 0);
    setGXColorS10(&colors[10], 0, 0, 0, 0);
    setGXColorS10(&colors[11], 0, 0, 0, 0);
    for (u8 i = 0; i < 7; i++) {
        u16 idx = (u16)(unk38E + i);
        if (ItemBoxLine_GetTabEntryLock(tabs, idx) != 0) {
            colors[8] = colors[4];
            colors[9] = colors[5];
            colors[10] = colors[6];
            colors[11] = colors[7];
        } else {
            colors[8] = colors[0];
            colors[9] = colors[1];
            colors[10] = colors[2];
            colors[11] = colors[3];
        }
        u16 key = ItemBoxLine_GetTabEntryItem(tabs, idx);
        DrawTabRowText(key, i);
        DrawTabRowIcon(key, i);
        u8 vis = ItemBoxLine_GetTabEntryReady(tabs, idx);
        SetPaneVisible(i, vis);
        FormatName(key, (s8)ItemBoxLine_ProbeNameSlot((CIBLTabFull*)tabs, idx), i);
        if (key == 0) {
            char nameBuf2[0x20];
            sprintf(nameBuf2, "txt_name%02d", i + 1);
            LayoutSetTextBoxFmtValue(field40, nameBuf2, "", 0);
            sprintf(nameBuf2, "txt_buyvalue%02d", i + 1);
            LayoutSetTextBoxFmtValue(field40, nameBuf2, "", 0);
        } else {
            char nameBuf1[0x20];
            sprintf(nameBuf1, "txt_name%02d", i + 1);
            LayoutSetTextBoxFmtValue(field40, nameBuf1,
                          (char*)ItemBoxLine_FormatTabEntryName((CIBLTabFormat*)tabs, idx), 0);
            GXColorS10 fs0 = PaneMatGetS16QuadByName(field40, nameBuf1, 0);
            GXColorS10 fs1 = fs0;
            GXColorS10 fs2 = PaneMatGetS16QuadByName(field40, nameBuf1, 1);
            GXColorS10 fs3 = fs2;
            colors[8].a = fs1.a;
            colors[9].a = fs2.a;
            PaneMatSetTevColorsByName(field40, nameBuf1, &colors[8], &colors[9]);
            char* name = BdatTouchStringCell("MNU_item", "name", 3);
            u32 count = ItemBoxLine_GetTabEntryValue(tabs, idx);
            char buf3[0x20];
            sprintf(buf3, "%d%s", count, name);
            sprintf(nameBuf1, "txt_buyvalue%02d", i + 1);
            LayoutSetTextBoxText(field40, nameBuf1, buf3, 0);
            GXColorS10 fs4 = PaneMatGetS16QuadByName(field40, nameBuf1, 0);
            GXColorS10 fs5 = fs4;
            GXColorS10 fs6 = PaneMatGetS16QuadByName(field40, nameBuf1, 1);
            GXColorS10 fs7 = fs6;
            colors[10].a = fs5.a;
            colors[11].a = fs6.a;
            PaneMatSetTevColorsByName(field40, nameBuf1, &colors[10], &colors[11]);
        }
    }
}

int CItemBoxLine::GetArmedFlag() { return unk39E; }

u16 CItemBoxLine::TouchTabEntryItem() {
    return ItemBoxLine_GetTabEntryItem(&unk3A4, (unsigned char)(unk38C + unk38E));
}

void CItemBoxLine::ResetCursorB8(bool show) { mCurB8.ShowTwoSubPanes(show); }

// ============================================================================
// ItemBoxLine_Info2SelectPrev: step the tab cursor down one slot (wrap to count-1 when it
// underflows), re-format the tab name overlay, then refresh the whole line.
// ============================================================================
void CItemBoxLine::Info2SelectPrev() {
    u8 b = code80135FDC_getByte_64077();
    u8 n = field39F - 1;
    field39F = n;
    if ((s8)n < 0) {
        field39F = b - 1;
    }
    nw4r::math::VEC3 tmp = mInfo2.GetSlotPos(field39F);
    ((CBaseCur*)&mCurB8)->setRootPaneTranslate(&tmp);
    RefreshLineState();
    playUISound(0xa);
}

// ============================================================================
// ItemBoxLine_Info2SelectNext: bump a cursor-position counter, format the tab name into a
// temp buffer, push it to the item-box-info2 overlay (via its vtable[4]), then
// refresh the whole line and beep.
// ============================================================================
void CItemBoxLine::Info2SelectNext() {
    unsigned char maxPos = code80135FDC_getByte_64077();
    unsigned char n = (unsigned char)(field39F + 1);
    field39F = n;
    if ((signed char)n >= (int)maxPos) {
        field39F = 0;
    }
    nw4r::math::VEC3 tmp = mInfo2.GetSlotPos(field39F);
    ((CBaseCur*)&mCurB8)->setRootPaneTranslate(&tmp);
    RefreshLineState();
    playUISound(0xa);
}


// ItemBoxLine_ResolveFocusId: item-box focus resolver. Returns a cursor/focus id based on
// the current navigation state and tab position.
// ============================================================================
u8 CItemBoxLine::ResolveFocusId() {
    if (field50 >= 6) return 0;
    if (mNumSel.IsOpen()) return 0;
    if (reinterpret_cast<CSysWin*>(&mSysWin)->isOpened()) return 0;
    if (mCurB8.BothSubPanesOn()) return 0xa;
    bool flag = code80135FDC_getByte_64077() > 1;
    if (field3A0 != 0) return 0xb;
    if (unk38C == -1) {
        if (tabCount > 1) {
            if (flag) return 0xc;
            return 0xe;
        }
        if (flag) return 0xd;
        return 0xf;
    }
    s32 result = 9;
    if (flag) result = 8;
    return result;
}

// ============================================================================
// ItemBoxLine_EnterState0: when the animation transform at +0x44 has finished, disable
// animation on it and enable it on +0x48 (intro -> loop), entering state 2.
// ============================================================================
void CItemBoxLine::EnterState0() {
    if (advanceAnimTransform(field44, lbl_eu_80668114) != 0) {
        field40->SetAnimationEnable(field44, false);
        field40->SetAnimationEnable(field48, true);
        field50 = 2;
    }
}

// ============================================================================
// ItemBoxLine_EnterState1: animate the item-box intro (0x48), store state 3, prime cursors
// and re-format/push the tab name overlay.
// ============================================================================
void CItemBoxLine::EnterState1() {
    if (advanceAnimTransform(field48, lbl_eu_80668114) == 0) return;
    field50 = 3;
    unk59 = 1;
    RefreshPageList();
    mCur70.SetVisible(1);
    mCurB8.SetVisible(1);
    nw4r::math::VEC3 tmp = mInfo2.GetSlotPos(field39F);
    ((CBaseCur*)&mCurB8)->setRootPaneTranslate(&tmp);
    RefreshLineRows();
}

// ============================================================================
// ItemBoxLine_EnterState2: when the animation transform at +0x48 has finished, disable
// animation on it and re-enable it on +0x44 (outro -> intro), entering state 5.
// ============================================================================
void CItemBoxLine::EnterState2() {
    if (AnimRewindFrame(field48, lbl_eu_80668114) != 0) {
        field40->SetAnimationEnable(field48, false);
        field40->SetAnimationEnable(field44, true);
        field50 = 5;
    }
}

// ============================================================================
// ItemBoxLine_EnterState3: on animation (+0x44) completion, set the busy byte, clear the
// navigation state, silence both cursors (+0x70/+0xA0) and clear the visible
// flag so the line stops rendering.
// ============================================================================
void CItemBoxLine::EnterState3() {
    if (AnimRewindFrame(field44, lbl_eu_80668114) != 0) {
        unk59 = 1;
        field50 = 0;
        mCur70.SetVisible(0);
        mCurA0.SetVisible(0);
        field4C = 0;
    }
}

// ============================================================================
// ItemBoxLine_PushSyswinCursor: when the syswin overlay is active, enter state 10, prime the
// cursor at +0x70, build the syswin selection buffer and push it through the
// cursor's vtable[2] virtual.
// ============================================================================
void CItemBoxLine::PushSyswinCursor() {
    if (reinterpret_cast<CSysWin*>(&mSysWin)->isActive()) {
        field50 = 0xa;
        mCur70.SetVisible(1);
        u8 tmp[0x10];
        CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(tmp), reinterpret_cast<CSysWin*>(&mSysWin), field3A3);
        mCur70.setRootPaneTranslate((const nw4r::math::VEC3*)(tmp));
    }
}

// ============================================================================
// ItemBoxLine_CommitOverlayState: refresh the active tab - select from num-select, set the name,
// and advance to state 6. csvtab index = (0x38c + 0x38e) & 0xFF.
// ============================================================================
void CItemBoxLine::CommitOverlayState() {
    if (!reinterpret_cast<CSysWin*>(&mSysWin)->isActive()) return;
    field50 = 3;
    if ((s8)field3A3 != 0) return;
    u8 tabidx = (u8)(unk38C + unk38E);
    field392 = 1;
    // tabs pointer is CSE'd into one callee-saved register (retail r30). The
    // (u8*) arg type routes both calls through the extern "C" (void*,
    // unsigned) overloads so MWCC emits external relocs to ItemBoxLine_FormatTabEntryName /
    // ItemBoxLine_GetTabEntryValue (retail object boundary) instead of inlining the same-TU
    // definitions.
    u8* tabs = (u8*)&unk3A4;
    mNumSel.SetItemName((char*)ItemBoxLine_FormatTabEntryName((CIBLTabFormat*)tabs, tabidx));
    mNumSel.SetCount((u8)field392);
    mNumSel.SetTotal(field392 * ItemBoxLine_GetTabEntryValue((const CIBLTab*)tabs, tabidx));
    mNumSel.Open();
    field50 = 6;
}

// ============================================================================
// ItemBoxLine_CompactTabList: refresh the tab list - re-register each tab entry with the
// layout, compact empty slots (bubble-shift), recount, re-register, then
// refresh the line and its slot panes.
// ============================================================================
void CItemBoxLine::CompactTabList() {
    if (field40 == 0) return;
    if (field3C == 0) return;
    if (gItemBoxLineMnuItemMesATable == 0) return;
    if (gItemBoxLineMnuItemMesBTable == 0) return;
    if (gItemBoxLineMnuSkillTable == 0) return;
    BuildSlotItemLists(0);
    if (field390 == 2) unk3A4.field93 = 1;
    u8 i = 0;
    while (i < tabCount) {
        ItemBoxLine_BuildTabEntries(&unk3A4, tabEntries[i], field394, 0);
        unk64[i] = (unk3A4.count != 0);
        i++;
    }
    for (u8 j = 0; j < tabCount; j++) {
        if (unk64[j] == 0) tabEntries[j] = 0;
    }
    for (u8 k = 0; k < tabCount - 1; k++) {
        u8 did = 0;
        for (u8 m = 0; m < (tabCount - 1) - k; m++) {
            if (tabEntries[m] == 0) {
                tabEntries[m] = tabEntries[m + 1];
                tabEntries[m + 1] = 0;
                did = 1;
            }
        }
        if (!did) break;
    }
    tabCount = 0;
    for (u8 n = 0; n < 9; n++) {
        if (tabEntries[n] == 0) break;
        tabCount++;
    }
    i = 0;
    while (i < tabCount) {
        ItemBoxLine_BuildTabEntries(&unk3A4, tabEntries[i], field394, 0);
        unk64[i] = (unk3A4.count != 0);
        i++;
    }
    RefreshLineState();
    RebuildTabList();
    field58 = 1;
}


// ============================================================================
// ItemBoxLine_RefreshLineState: refresh the item-box line - push the current tab entry (tab
// type byte, page id, selection cursor) into the tab layout, re-run the
// focus/page refresh, then poll the info2 overlay state. The tab-buffer,
// focus and page helpers are external retail symbols (same merged unit,
// different retail objects), so they are called through their C-linkage
// declarations to keep the relocs external.
// ============================================================================
u32 CItemBoxLine::RefreshLineState() {
    u8 sel = GetCollectedFlagByte(field39F);
    ItemBoxLine_BuildTabEntries(&unk3A4, tabEntries[(s8)field6D],
                  field394, (u8)sel);
    RefreshTabPanes();
    RefreshPageList();
    return mInfo2.GetState();
}

// ============================================================================
// ItemBoxLine_RefreshPageList: refresh the tab-line page list. Set the tab name overlay,
// then (when the info2 overlay is open) push the active tab entry into the
// info2 state and rebuild the 12 page slots: copy the per-page word, flag and
// position data from the page tables into the +0x180 page region.
// ============================================================================
void CItemBoxLine::RefreshPageList() {
    CIBLTab* tabs = &unk3A4;
    u8 idx = (u8)(unk38C + unk38E);
    char* name = ItemBoxLine_FormatDisplayName(reinterpret_cast<CIBLTabFull*>(tabs), idx);
    LayoutSetTextBoxFmtValue(field40, "txt_comment", name,
                  reinterpret_cast<u32>(field54));
    if (mInfo2.GetState() != 0) {
        u8 f9 = ItemBoxLine_GetTabEntryReady(tabs, idx);
        u16 f0 = ItemBoxLine_GetTabEntryItem(tabs, idx);
        mInfo2.SetItem(f0, 0, field39F, f9);
        char* tabName = reinterpret_cast<char*>(
            ItemBoxLine_FormatTabEntryName((CIBLTabFormat*)tabs, idx));
        mInfo2.SetNamedText(0, tabName);
        SetInfo2Item(
                      ItemBoxLine_GetTabEntryItem(tabs, idx));
        CItemBoxPage* page = &mInfo2.mPage;
        u16 i = 0;
        do {
            u16 v = ArrayGet12(mPage.mItemIds, (u8)i);
            if (v != 0) {
                page->mItemIds[(u8)i] = v;
                page->mBytesA8[(u8)i] = ItemBoxGetTab(
                    &mPage, (u8)i);
                u8 b594 = 0;
                if ((u8)i < 12) {
                    b594 = mPage.mBytesB4[(u8)i];
                }
                page->mBytesB4[(u8)i] = b594;
                s16 s5A0 = 0;
                if ((u8)i < 12) {
                    s5A0 = mPage.mValues[(u8)i];
                }
                page->mText.mValues2[(u8)i] = s5A0;
                CIBLVec3 vec;
                CopyTabSlotVec(&vec, mPage.mItemIds, (u8)i);
                copyVEC3(&page->mPos[(u8)i], &vec);
            }
            i++;
        } while (i < 12);
    }
}
// ============================================================================
// ItemBoxLine_RefreshTabLabels: refresh the seven tab-slot panes - format each slot's two
// pane names, then show/hide them: the slot matching the current tab position
// is lit when its entry is active, otherwise the paired pane is lit instead.
// ============================================================================
void CItemBoxLine::RefreshTabLabels(unsigned int) {
    for (u8 i = 0; i < 7; i++) {
        char nameA[0x20];
        char nameB[0x20];
        sprintf(nameA, "pic_tbbcon%02d", (u8)i + 1);
        sprintf(nameB, "pic_tbbcof%02d", (u8)i + 1);
        bool visA = false;
        bool visB = false;
        if (tabEntries[i] != 0) {
            if (i == (s8)field6D) {
                visA = true;
                visB = false;
            } else {
                visA = false;
                visB = true;
            }
        }
        setPaneVisible(field40->GetRootPane()->FindPaneByName(nameA, true), visA);
        setPaneVisible(field40->GetRootPane()->FindPaneByName(nameB, true), visB);
    }
}

// ============================================================================
// ItemBoxLine_RefreshLineRows: move the active cursor onto its pane. When no tab entry is
// selected (unk38C == -1) the tab cursor (mCurA0) tracks the pane named from
// the current tab position and the line cursor (mCur70) is hidden; otherwise
// the line cursor tracks the slot pane and the tab cursor is hidden. The
// cursor Move virtual receives the pane's accumulated translate with x scaled
// by the fixed reference pane's +0x44 scale.
// ============================================================================
void CItemBoxLine::RefreshLineRows() {
    s16 sel = unk38C;
    if (sel == -1) {
        char name[0x20];
        sprintf(name, "pic_cat%02d", (s8)field6D + 1);
        nw4r::lyt::Pane* pane =
            field40->GetRootPane()->FindPaneByName(name, true);
        nw4r::math::VEC3 pos;
        AccumPaneWorldPos(&pos, pane);
        nw4r::lyt::Pane* refPane =
            field40->GetRootPane()->FindPaneByName("nul_proportion", true);
        pos.x *= refPane->GetScale().x;
        ((CBaseCur*)&mCurA0)->setRootPaneTranslate((const nw4r::math::VEC3*)(&pos));
        mCurA0.SetVisible(1);
        mCur70.SetVisible(0);
    } else {
        char name[0x20];
        sprintf(name, "nulbtn%02d", sel + 1);
        nw4r::lyt::Pane* pane =
            field40->GetRootPane()->FindPaneByName(name, true);
        nw4r::math::VEC3 pos;
        AccumPaneWorldPos(&pos, pane);
        nw4r::lyt::Pane* refPane =
            field40->GetRootPane()->FindPaneByName("nul_proportion", true);
        pos.x *= refPane->GetScale().x;
        mCur70.setRootPaneTranslate((const nw4r::math::VEC3*)(&pos));
        mCur70.SetVisible(1);
        mCurA0.SetVisible(0);
    }
}

// ============================================================================
// ItemBoxLine_SetInfo2Item: dispatch on the active tab (tab entry type) and the item pick
// kind to route to a tab-specific layout refresh / page-navigation helper.
// ============================================================================
void CItemBoxLine::SetInfo2Item(u32 itemData) {
    ResetPageTables(itemData);
    setLayoutTextBoxNumber(field40, "txt_havenum", CItem_sumFamilyByte6(itemData));
    switch ((int)tabEntries[(s8)field6D]) {
    case 2:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 0xd:
        setPaneVisible(field40->GetRootPane()->FindPaneByName("nul_bcln01", true), 1);
        break;
    case 3:
        setPaneVisible(field40->GetRootPane()->FindPaneByName("nul_bcln50", true), 1);
        break;
    case 0xa:
    case 0xb:
        setPaneVisible(field40->GetRootPane()->FindPaneByName("nul_bcln60", true), 1);
        break;
    }
    switch ((int)(BdatGetItemType(itemData) & 0xff)) {
    case 2:
        ShowSlotCounts(itemData);
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        ShowSlotCounters(itemData);
        break;
    case 3:
        FormatGemName(itemData);
        break;
    case 0xa:
        ShowPageTabs(itemData);
        break;
    case 0xd:
        FormatKindDName(itemData);
        break;
    }
}

// ============================================================================
// ItemBoxLine_ResetPageTables: reset the page word/vector tables and hide every line pane.
// ============================================================================
void CItemBoxLine::ResetPageTables(u32 itemData) {
    for (u8 i = 0; i < 12; i++) {
        mPage.mItemIds[i] = 0;
        CIBLVec3 vec;
        copyVEC3(&mPage.mPos[i],
                 reinterpret_cast<const CIBLVec3*>(code80135FDC_setVec3(
                     (nw4r::math::VEC3*)((float*)&vec), lbl_eu_806680F8, lbl_eu_806680F8, lbl_eu_806680F8)));
    }
    // Hide every line pane (pane names from the shared pool; all hidden).
    setPaneVisible(field40->GetRootPane()->FindPaneByName("nul_para_01", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("nul_para_50", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("nul_para_60", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("nul_para_80", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("nul_bcln01", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("nul_bcln50", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("nul_bcln60", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para01", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para04", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para08", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para10", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para11", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para22", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para23", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para24", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para25", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_value02", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_value04", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_value07", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_value08", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_value09", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_value22", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("pic_ethcol01", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("pic_ethcol02", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("pic_ethcol03", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_eth01", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_eth02", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_eth03", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para50", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para51", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_value50", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_value51", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para60", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para61", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_value60", true), 0);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_value61", true), 0);
}

// ============================================================================
// ItemBoxLine_ShowSlotCounts: show the item-slot panes, bind per-kind slot data (counts,
// names, per-slot bytes) into the layout, accumulate a ratio from the slot-
// entry lookup loop and refresh the pane colors.
// ============================================================================
void CItemBoxLine::ShowSlotCounts(u32 itemData) {
    setPaneVisible(field40->GetRootPane()->FindPaneByName("nul_para_01", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para01", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para04", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para08", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para10", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para11", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para22", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para23", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para24", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para25", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_value02", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_value04", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_value07", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_value08", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_value09", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("pic_ethcol01", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("pic_ethcol02", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("pic_ethcol03", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_eth01", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_eth02", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_eth03", true), 1);

    // Per-kind slot lookups from the kind-owner table (::cf::CfBdat::spItmWpnListFileData).
    u32 table = (u32)::cf::CfBdat::spItmWpnListFileData;
    BdatGetItemType(itemData);
    u16 kind = BdatGetItemId(itemData);
    u16 a = BdatGetU16Direct((const void*)table, "dmg_low", kind);
    u16 b = BdatGetU16Direct((const void*)table, "dmg_hi", kind);
    u8 c = BdatGetU8Direct((const void*)table, "arm_phy", kind);
    u8 d = BdatGetU8Direct((const void*)table, "arm_eth", kind);
    u8 e = BdatGetU8Direct((const void*)table, "att_lev", kind);
    u8 f = BdatGetU8Direct((const void*)table, "speed", kind);
    float total = (float)f / 10.0f;
    u8 g = BdatGetU8Direct((const void*)table, "grd_rate", kind);
    int v23;
    int v22;
    if (BdatGetU8Direct((const void*)table, "flag", kind) & 4) {
        // retail reuses CtrlObjectParam_GetCharEntry's r3 as the arg to CtrlObjectParam_GetArtsDataWord
        u16 c1 = CtrlObjectParam_GetArtsDataWord(CtrlObjectParam_GetCharEntry(1));
        v23 = (int)(1.4f * (float)(a * c1));
        u16 c2 = CtrlObjectParam_GetArtsDataWord(CtrlObjectParam_GetCharEntry(1));
        v22 = (int)(1.5f * (float)(b * c2));
        if ((u16)v23 >= 0x3e7) v23 = 0x3e7;
        if ((u16)v22 >= 0x3e7) v22 = 0x3e7;
    }

    // Slot-entry ratio loop: probe slot names "%d" until an entry exists,
    // adding the per-slot bonus; then refresh the item counts.
    char buf[0x20];
    u8 i = 1;
    do {
        sprintf(buf, "equip_pc%d", i);
        if (BdatGetU8Direct((const void*)table, (const char*)buf, kind) != 0) {
            total += GetFloatTableEntry(i);
            break;
        }
        i++;
    } while (i <= 10);
    BlendFloatAvgScale((u16)v23, (u16)v22, 0, total);

    // Format the slot-count text and bind it plus the per-slot bytes.
    ml::FixStr<32> str;
    char* nameB = BdatTouchStringCell("MNU_item", "name", 0xb);
    str.format("%d%s%d", (u16)v23, nameB, (u16)v22);
    LayoutSetTextBoxFmtValue(field40, "txt_value02", str.mString, 0);
    setLayoutTextBoxNumber(field40, "txt_value08", c);
    setLayoutTextBoxNumber(field40, "txt_value09", d);
    char* n7f = BdatTouchStringCell("MNU_item", "name", 0x7f);
    char* n80 = BdatTouchStringCell("MNU_item", "name", 0x80);
    if (e != 0) {
        str.format("%s%d%s", n7f, n80);
    } else {
        str.format("%d%s", e, n80);
    }
    LayoutSetTextBoxFmtValue(field40, "txt_value04", str.mString, 0);
    char* m7f = BdatTouchStringCell("MNU_item", "name", 0x7f);
    char* m80 = BdatTouchStringCell("MNU_item", "name", 0x80);
    if (g != 0) {
        str.format("%s%d%s", m7f, m80);
    } else {
        str.format("%d%s", g, m80);
    }
    LayoutSetTextBoxFmtValue(field40, "txt_value07", str.mString, 0);
    LayoutSetTextBoxFmtValue(field40, "txt_value22", "", 0);
    LayoutSetTextBoxFmtValue(field40, "txt_para24", "", 0);
    LayoutSetTextBoxFmtValue(field40, "txt_para25", "", 0);
    PaneMatSetTevColorsByName(field40, "txt_para01", &itemBoxLine_tabLabelNeutralTop, &itemBoxLine_tabLabelNeutralBottom);
    PaneMatSetTevColorsByName(field40, "txt_para10", &itemBoxLine_tabLabelNeutralTop, &itemBoxLine_tabLabelNeutralBottom);
    PaneMatSetTevColorsByName(field40, "txt_para11", &itemBoxLine_tabLabelNeutralTop, &itemBoxLine_tabLabelNeutralBottom);
    PaneMatSetTevColorsByName(field40, "txt_para04", &itemBoxLine_tabLabelNeutralTop, &itemBoxLine_tabLabelNeutralBottom);
    PaneMatSetTevColorsByName(field40, "txt_para08", &itemBoxLine_tabLabelNeutralTop, &itemBoxLine_tabLabelNeutralBottom);
    PaneMatSetTevColorsByName(field40, "txt_para22", &itemBoxLine_tabLabelSelectedTop, &itemBoxLine_tabLabelSelectedBottom);
    PaneMatSetTevColorsByName(field40, "txt_para23", &itemBoxLine_tabLabelSelectedTop, &itemBoxLine_tabLabelSelectedBottom);
    BuildPageData(itemData);
}


// ============================================================================
// ItemBoxLine_ShowSlotCounters: show the item-slot panes, bind the per-kind counter text into
// the three slots, then pick the slot-name format strings via the kind selector
// (a compare ladder plus a jump-table switch) and refresh the pane colors.
// ============================================================================
void CItemBoxLine::ShowSlotCounters(u32 itemData) {
    setPaneVisible(field40->GetRootPane()->FindPaneByName("nul_para_01", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para01", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para04", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para08", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para10", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para11", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para22", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para23", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para24", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para25", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_value08", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_value09", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_value22", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("pic_ethcol01", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("pic_ethcol02", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("pic_ethcol03", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_eth01", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_eth02", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_eth03", true), 1);

    // Kind-based counter text into the three slots.
    u32 table = (u32)::cf::CfBdat::spItmEquipListFileData;
    BdatGetItemType(itemData);
    u16 kind = BdatGetItemId(itemData);
    u8 a = BdatGetU8Direct((const void*)table, "arm_phy", kind);
    u8 b = BdatGetU8Direct((const void*)table, "arm_eth", kind);
    u8 c = BdatGetU8Direct((const void*)table, "eva_rate", kind);
    setLayoutTextBoxNumber(field40, "txt_value08", a);
    setLayoutTextBoxNumber(field40, "txt_value09", b);
    setLayoutTextBoxNumber(field40, "txt_value22", c);

    // Slot-name format ladder (kind selector). The range check and the three
    // equality tests each branch to their case body; the bodies sit after the
    // tests in retail order (0x32, 0x31, 0x30, 0x2e) with the default last.
    u32 selTable = (u32)::cf::CfBdat::spItmEquipListFileData;
    u8 sel = BdatGetU8Direct((const void*)selTable, "arm_type",
                           BdatGetItemId(itemData));
    char* name;
    if ((u32)(sel - 4) <= 9) goto sel2e;
    if ((int)sel == 3) goto sel32;
    if ((int)sel == 2) goto sel31;
    if ((int)sel == 1) goto sel30;
    goto selElse;
sel32:
    name = BdatTouchStringCell("MNU_item", "name", 0x32);
    goto selDone;
sel31:
    name = BdatTouchStringCell("MNU_item", "name", 0x31);
    goto selDone;
sel30:
    name = BdatTouchStringCell("MNU_item", "name", 0x30);
    goto selDone;
sel2e:
    name = BdatTouchStringCell("MNU_item", "name", 0x2e);
    goto selDone;
selElse:
    name = 0;
selDone:
    LayoutSetTextBoxFmtValue(field40, "txt_para24", name, 0);

    // Secondary slot-name switch (selectors 4..12 via jump table).
    char* name2 = 0;
    switch (sel) {
    case 4: name2 = BdatTouchStringCell("pc_arts", "name", 0x77); break;
    case 5: name2 = BdatTouchStringCell("pc_arts", "name", 0x78); break;
    case 6: name2 = BdatTouchStringCell("pc_arts", "name", 0x79); break;
    case 7: name2 = BdatTouchStringCell("pc_arts", "name", 0x7a); break;
    case 8: name2 = BdatTouchStringCell("pc_arts", "name", 0x7b); break;
    case 9: name2 = BdatTouchStringCell("pc_arts", "name", 0x7c); break;
    case 10: name2 = BdatTouchStringCell("pc_arts", "name", 0x7d); break;
    case 11: name2 = BdatTouchStringCell("pc_arts", "name", 0x7e); break;
    case 12: name2 = BdatTouchStringCell("pc_arts", "name", 0x7f); break;
    }
    LayoutSetTextBoxFmtValue(field40, "txt_para25", name2, 0);

    // Refresh the slot pane colors.
    PaneMatSetTevColorsByName(field40, "txt_para01", &itemBoxLine_tabLabelSelectedTop, &itemBoxLine_tabLabelSelectedBottom);
    PaneMatSetTevColorsByName(field40, "txt_para10", &itemBoxLine_tabLabelNeutralTop, &itemBoxLine_tabLabelNeutralBottom);
    PaneMatSetTevColorsByName(field40, "txt_para11", &itemBoxLine_tabLabelNeutralTop, &itemBoxLine_tabLabelNeutralBottom);
    PaneMatSetTevColorsByName(field40, "txt_para04", &itemBoxLine_tabLabelSelectedTop, &itemBoxLine_tabLabelSelectedBottom);
    PaneMatSetTevColorsByName(field40, "txt_para08", &itemBoxLine_tabLabelSelectedTop, &itemBoxLine_tabLabelSelectedBottom);
    PaneMatSetTevColorsByName(field40, "txt_para22", &itemBoxLine_tabLabelNeutralTop, &itemBoxLine_tabLabelNeutralBottom);
    PaneMatSetTevColorsByName(field40, "txt_para23", &itemBoxLine_tabLabelNeutralTop, &itemBoxLine_tabLabelNeutralBottom);
    BuildPageData(itemData);
}



// ============================================================================
// ItemBoxLine_FormatGemName: kind-3 item slot. Shows the five slot panes, resolves the
// kind-3 display name via the EC owner table (index 30-(v-1)), looks up the
// gem id (BdatGetU16Direct) and per-gem strings: id==1 pins the fixed "none"
// string, otherwise the two D8-owner lookups fill the gem name / +21-indexed
// level name. Finally stores the category halfword + state 9 and copies the
// pane-anchor position into the +0x558 vec.
// ============================================================================
void CItemBoxLine::FormatGemName(u32 itemData) {
    setPaneVisible(field40->GetRootPane()->FindPaneByName("nul_para_50", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para50", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para51", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_value50", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_value51", true), 1);
    BdatGetItemType(itemData);
    u16 cat = BdatGetItemId(itemData);
    u32 vEC = BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData, "rankType", itemData);
    char* name = BdatTouchStringCell("MNU_item", "name", 30 - ((u8)vEC - 1));
    LayoutSetTextBoxFmtValue(field40, "txt_value50", name, 0);
    u16 id = BdatGetU16Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, "max", cat);
    char* txt = BdatTouchStringCell("MNU_shop", "name", 15);
    if (id == 1) {
        txt = "";
        LayoutSetTextBoxFmtValue(field40, "txt_value51", txt, 0);
    }
    char* gemName = BdatGetPtrDirect((const void*)::cf::CfBdat::spBtlSkillListFileData, "name", cat);
    LayoutSetTextBoxFmtValue(field40, "txt_para51", gemName, 0);
    char* lvlName = BdatTouchStringCell(
        "MNU_item", "name",
        (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, "attach", cat) + 21);
    LayoutSetTextBoxFmtValue(field40, "txt_para52", lvlName, 0);
    nw4r::lyt::Pane* anchor =
        field40->GetRootPane()->FindPaneByName("txt_para51", true);
    nw4r::lyt::Pane* ref =
        field40->GetRootPane()->FindPaneByName("nul_proportion", true);
    nw4r::math::VEC3 pos;
    ScalePaneWorldPos(&pos, anchor, ref, field40->GetRootPane());
    mPage.mItemIds[8] = cat;
    mPage.mBytesA8[8] = 9;
    nw4r::math::VEC3 pos2 = pos;
    copyVEC3((nw4r::math::VEC3*)&mPage.mPos[8], &pos2);
}

// ============================================================================
// ItemBoxLine_ShowPageTabs: show the page tab panes and fill the active item's name /
// category text into the syswin layout panes.
// ============================================================================
void CItemBoxLine::ShowPageTabs(u32 itemData) {
    setPaneVisible(field40->GetRootPane()->FindPaneByName("nul_para_60", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para60", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_para61", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_value60", true), 1);
    setPaneVisible(field40->GetRootPane()->FindPaneByName("txt_value61", true), 1);
    u32 owner = (u32)::cf::CfBdat::spItmCollectListFileData;
    BdatGetItemType(itemData);
    u16 cat = BdatGetItemId(itemData);
    u8 v = BdatGetU8Direct((const void*)owner, "type", cat);
    char* name = BdatTouchStringCell("MNU_collect", "name", v);
    LayoutSetTextBoxFmtValue(field40, "txt_value60", name, 0);
    u8 v2 = BdatGetU8Direct((const void*)owner, "mapID", cat);
    char* name2;
    if (0x1a == v2) {        name2 = BdatTouchStringCell("MNU_collect", "name", 0x14);
    } else {
        name2 = BdatGetPtrDirect((const void*)::cf::CfBdat::spFldMapListFileData, "name", v2);
    }
    LayoutSetTextBoxFmtValue(field40, "txt_value61", name2, 0);
}



// ============================================================================
// ItemBoxLine_FormatKindDName: kind-0xD item slot name. Shows the slot pane, resolves the
// gem/ferron entry via the owner tables (entry = table + 73*kindIndex +
// 2*nameIndex) and picks the display string: kind 1 requires byte +0xE8 set,
// kinds 2/3 require bit 6 / bit 5 of byte +0xE9, each falling back to the
// kind-agnostic default (index 44 vs 43), then binds it to the text pane.
// ============================================================================
void CItemBoxLine::FormatKindDName(u32 itemData) {
    setPaneVisible(field40->GetRootPane()->FindPaneByName("nul_para_80", true), 1);
    u32 owner = (u32)::cf::CfBdat::spItmArtsListFileData;
    BdatGetItemType(itemData);
    u16 cat = BdatGetItemId(itemData);
    u8 vA = BdatGetU8Direct((const void*)owner, "memory_type", cat);
    u8 vB = BdatGetU8Direct((const void*)owner, "pc_type", cat);
    u8 vC = BdatGetU8Direct((const void*)owner, "get_arts", cat);
    u8 defName = BdatGetU8ByTableKey("pc_arts", "idx", vC);
    void* tbl = CtrlObjectParam_GetCharEntry(vB);
    u8 idx = (u8)CtrlObjectParam_GetCurrentRowKey(tbl);
    const u8* entry = (const u8*)tbl + idx * 73 + (defName << 1);
    char* str = BdatTouchStringCell("MNU_item", "name", 44);
    switch (vA) {
    case 1:
        if (entry[232] != 0) {
            str = BdatTouchStringCell("MNU_item", "name", 43);
        }
        break;
    case 2:
        if ((entry[233] >> 7) & 1) {
            str = BdatTouchStringCell("MNU_item", "name", 43);
        }
        break;
    case 3:
        if ((entry[233] >> 6) & 1) {
            str = BdatTouchStringCell("MNU_item", "name", 43);
        }
        break;
    }
    LayoutSetTextBoxFmtValue(field40, "txt_para80", str, 0);
}

// ============================================================================
// ItemBoxLine_BuildPageData: build the item-box page data.
// the pane, resolve the slot entry (itemId+kind via the owner tables) and bind
// a texture/name; then push the slot's kind/counters/position into the page
// arrays at index i+8 (guarded to 12) and refresh the pane hierarchy.
// ============================================================================
void CItemBoxLine::BuildPageData(u32 itemData) {
    u32 itemId = LookupItemTypeTable(itemData);
    u16 kind = BdatGetItemId(itemData);
    u8 n = BdatGetU8Direct((const void*)itemId, "jwl_slot", (u16)kind);
    for (u8 i = 0; i < 3; i++) {
        char nameBuf[0x20];
        ml::FixStr<32> buf;
        buf.clear();
        sprintf(nameBuf, "pic_ethcol%02d", i + 1);
        nw4r::lyt::Pane* pane = field40->GetRootPane()->FindPaneByName(nameBuf, true);
        u16 kindV = 0;
        u8 b = 0;
        u8 a = 0;
        if (i < n) {
            setPaneVisible(pane, 1);
            u32 tex = 0;
            buf.format("jwl_skill%d", i + 1);
            u16 v = BdatGetU16Direct((const void*)itemId, (const char*)buf.mString, (u16)kind);
            if (v != 0) {
                a = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData, "percent", (u16)v);
                b = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData, "rankType", (u16)v);
                u8 sel = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, "atr_type",
                                          BdatGetItemId(v));
                if (sel == 0) {
                    tex = (u32)((nw4r::lyt::ArcResourceAccessor*)field3C)->GetResource(0x74696d67, "mf00_reg30_crys00.tpl", 0);
                } else if (sel == 4) {
                    tex = (u32)((nw4r::lyt::ArcResourceAccessor*)field3C)->GetResource(0x74696d67, "mf00_reg30_crys01.tpl", 0);
                } else if (sel == 5) {
                    tex = (u32)((nw4r::lyt::ArcResourceAccessor*)field3C)->GetResource(0x74696d67, "mf00_reg30_crys02.tpl", 0);
                } else if (sel == 6) {
                    tex = (u32)((nw4r::lyt::ArcResourceAccessor*)field3C)->GetResource(0x74696d67, "mf00_reg30_crys03.tpl", 0);
                } else if (sel == 7) {
                    tex = (u32)((nw4r::lyt::ArcResourceAccessor*)field3C)->GetResource(0x74696d67, "mf00_reg30_crys04.tpl", 0);
                } else if (sel == 8) {
                    tex = (u32)((nw4r::lyt::ArcResourceAccessor*)field3C)->GetResource(0x74696d67, "mf00_reg30_crys05.tpl", 0);
                } else if (sel == 9) {
                    tex = (u32)((nw4r::lyt::ArcResourceAccessor*)field3C)->GetResource(0x74696d67, "mf00_reg30_crys06.tpl", 0);
                }
                buf.format("%s", BuildItemNameStr(v));
                kindV = BdatGetItemId(v);
            } else {
                tex = (u32)((nw4r::lyt::ArcResourceAccessor*)field3C)->GetResource(0x74696d67, "mf00_reg30_crys00.tpl", 0);
                buf.format("%s",
                           BdatTouchStringCell("MNU_item", "name", 0x2a));
            }
            sprintf(nameBuf, "txt_eth%02d", i + 1);
            LayoutSetTextBoxFmtValue(field40, nameBuf, buf.mString, 0);
            if (tex != 0) {
                PaneSetTexPalette(pane, (TPLPalette*)tex);
            }
        } else {
            sprintf(nameBuf, "txt_eth%02d", i + 1);
            LayoutSetTextBoxFmtValue(field40, nameBuf, "", 0);
            setPaneVisible(pane, 0);
        }
        nw4r::lyt::Pane* find = field40->GetRootPane()->FindPaneByName("nul_proportion", true);
        CIBLVec3 pos;
        ScalePaneWorldPos((nw4r::math::VEC3*)&pos, pane, find, field40->GetRootPane());
        u8 idx = (u8)(i + 8);
        CItemBoxPage* page = &mPage;
        if (idx < 12) {
            page->mItemIds[idx] = kindV;
        }
        if (idx < 12) {
            page->mBytesA8[idx] = 3;
        }
        if (idx < 12) {
            page->mBytesB4[idx] = b;
        }
        if (idx < 12) {
            page->mValues[idx] = a;
        }
        CIBLVec3 tmp;
        tmp.x = pos.x;
        tmp.y = pos.y;
        tmp.z = pos.z;
        if (idx < 12) {
            copyVEC3(&page->mPos[idx], &tmp);
        }
    }
}

// ============================================================================
// ItemBoxLine_ProbeAvailability: kind/availability probe. Resolve the item's kind via the
// owner tables, index into the 0x49-byte character-data rows with the page
// lookup, and report whether the flagged slot (kind-dependent bit in +0xE8/
// +0xE9 of the row entry) is set. The first parameter is unused (retail keeps
// it in r3 and works from r4).
// ============================================================================
int CItemBoxLine::ProbeAvailability(u32 key) {
    u32 table = (u32)::cf::CfBdat::spItmArtsListFileData;
    BdatGetItemType(key);
    // Retail keeps the string-pool base in a temp register that is later
    // reused for the BdatGetU8ByTableKey result.
    u16 v = BdatGetItemId(key);
    u8 a = BdatGetU8Direct((const void*)table, "memory_type", v);
    u8 b = BdatGetU8Direct((const void*)table, "pc_type", v);
    u8 c = BdatGetU8Direct((const void*)table, "get_arts", v);
    u8 d = BdatGetU8ByTableKey("pc_arts", "idx", c);
    void* lookup = CtrlObjectParam_GetCharEntry((u8)b);
    u8 cat = (u8)CtrlObjectParam_GetCurrentRowKey(lookup);
    // Same typed art-row walk as CItemBoxInfo::BuildSelRec.
    CItemBoxArtSlot* entry = &((CItemBoxCharEntry*)lookup)->mArtRows[cat].mArts[d];

    switch (a) {
    case 1:
        if (entry->mLevel != 0) return 1;
        break;
    case 2:
        if ((entry->mFlags >> 7) & 1) return 1;
        break;
    case 3:
        if ((entry->mFlags >> 6) & 1) return 1;
        break;
    }
    return 0;
}

// ============================================================================
// CItemBoxLine::OnFileEvent - file-load completion dispatch. The event's file
// handle is matched against the four load slots (+0x24 line arc, +0x28 common
// arc A, +0x2C bdat, +0x30 scratch): the line-arc branch builds the whole
// layout (scratch region, tag processor, arc accessor, layout + two animation
// transforms, font bind, pane text/colour seeding, the four embedded cursors);
// the common-arc branch re-attaches its accessor and refreshes the tab list;
// the bdat/scratch branches register their buffers with the CBdat system.
bool CItemBoxLine::OnFileEvent(CEventFile* evt) {
    if (this->field24 == (u32)evt->mFileHandle) {
        (&mRegion04)->createRegion(
            mtl::MemManager::getHandleMEM2(), 0x18000, "CItemBoxLine", 0);
        Class_8045F858 host((&mRegion04));
        u8* data = (u8*)((CFileHandle*)this->field24)->getData();
        mtl::MemManager::setMemInitFlag(false);
        void* tpMem = mtl::MemManager::allocate(0x858,
                                                CLibLayout::getAllocHandle());
        // The ctor returns self in r3, so the store can reuse r3 after the call
        // (retail: cmpwi/beq/bl __ct__CTagProcessor; stw r3).
        this->field54 = (nw4r::ut::WideTagProcessor*)(tpMem != 0 ? __ct__CTagProcessor(tpMem) : tpMem);
        nw4r::lyt::ArcResourceAccessor* acc =
            CLibLayout::createArcResourceAccessor();
        this->field38 = acc;
        acc->Attach(data, "arc");
        buildLayout(&this->field40, (nw4r::lyt::ArcResourceAccessor*)this->field38,
                      "mf02_box01_boxsh.brlyt");
        bindLayoutAnimTransform(this->field40, &this->field44,
                      (nw4r::lyt::ArcResourceAccessor*)this->field38,
                      "mf02_box01_boxsh_in.brlan");
        bindLayoutAnimTransform(this->field40, &this->field48,
                      (nw4r::lyt::ArcResourceAccessor*)this->field38,
                      "mf02_box01_boxsh_info_in.brlan");
        nw4r::lyt::Pane* root = this->field40->GetRootPane();
        void* fontObj = CDeviceFont::getFontInfo(1, this->field40);
        ApplyFontToTxtPanes(root, (const nw4r::ut::Font*)(((IDeviceFontInfo*)fontObj)->getFont()));

        // Seed the seven slot-name textboxes with the line text.
        char* text = (char*)CUICfManager_getPackedFont9C();
        for (u8 i = 1; i <= 7; i++) {
            char buf[0x20];
            sprintf(buf, "txt_buyvalue%02d", i);
            setLayoutTextBoxFont(this->field40, buf, (u32)text);
        }
        this->field40->SetAnimationEnable(this->field48, false);
        this->field40->SetAnimationEnable(this->field44, true);
        this->field40->Animate(0);

        // Bind the tag processor to the text pane, seed the nameplate colour.
    nw4r::lyt::TextBox* tagPane =
        (nw4r::lyt::TextBox*)this->field40->GetRootPane()->
            FindPaneByName("txt_comment", true);
        if (tagPane != 0) {
            tagPane->SetTagProcessor(this->field54);
        }
        nw4r::lyt::Pane* namePane = this->field40->GetRootPane()->
                                        FindPaneByName("pic_cat01", true);
        if (namePane != 0) {
            // +0x68 is Pane::GetMaterial; seed the nameplate colour from
            // the material's mTevCols[0].
            nw4r::lyt::Material* mat = namePane->GetMaterial();
            GXColorS10 colorTmp = mat->GetTevColor(0);
            this->field396 = colorTmp;
        }

        // Bind the line text into the sixteen label panes.
        char* text2 = (char*)CUICfManager_getPackedFont9C();
        setLayoutTextBoxFont(this->field40, "txt_value02", (u32)text2);
        setLayoutTextBoxFont(this->field40, "txt_value08", (u32)text2);
        setLayoutTextBoxFont(this->field40, "txt_value09", (u32)text2);
        setLayoutTextBoxFont(this->field40, "txt_value04", (u32)text2);
        setLayoutTextBoxFont(this->field40, "txt_value07", (u32)text2);
        setLayoutTextBoxFont(this->field40, "txt_value22", (u32)text2);
        setLayoutTextBoxFont(this->field40, "txt_value50", (u32)text2);
        setLayoutTextBoxFont(this->field40, "txt_value51", (u32)text2);
        setLayoutTextBoxFont(this->field40, "txt_havenum", (u32)text2);
        setLayoutTextBoxFont(this->field40, "txt_num01", (u32)text2);
        setLayoutTextBoxFont(this->field40, "txt_num02", (u32)text2);
        setLayoutTextBoxFont(this->field40, "txt_num03", (u32)text2);
        setLayoutTextBoxFont(this->field40, "txt_num04", (u32)text2);
        setLayoutTextBoxFont(this->field40, "txt_num05", (u32)text2);
        setLayoutTextBoxFont(this->field40, "txt_num06", (u32)text2);
        setLayoutTextBoxFont(this->field40, "txt_num07", (u32)text2);

        // Per-kind slot name providers (index into the shared name table).
        LayoutSetTextBoxFmtValue(this->field40, "txt_para01",
                      BdatTouchStringCell("MNU_item", "name", 0xa), 0);
        LayoutSetTextBoxFmtValue(this->field40, "txt_para04",
                      BdatTouchStringCell("MNU_item", "name", 0xd), 0);
        LayoutSetTextBoxFmtValue(this->field40, "txt_para08",
                      BdatTouchStringCell("MNU_item", "name", 0x11), 0);
        LayoutSetTextBoxFmtValue(this->field40, "txt_para10",
                      BdatTouchStringCell("MNU_item", "name", 0x12), 0);
        LayoutSetTextBoxFmtValue(this->field40, "txt_para11",
                      BdatTouchStringCell("MNU_item", "name", 0xc), 0);
        LayoutSetTextBoxFmtValue(this->field40, "txt_para22",
                      BdatTouchStringCell("MNU_item", "name", 0x13), 0);
        LayoutSetTextBoxFmtValue(this->field40, "txt_para50",
                      BdatTouchStringCell("MNU_item", "name", 0x18), 0);
        LayoutSetTextBoxFmtValue(this->field40, "txt_para60",
                      BdatTouchStringCell("MNU_item", "name", 0x24), 0);
        LayoutSetTextBoxFmtValue(this->field40, "txt_para61",
                      BdatTouchStringCell("MNU_item", "name", 0x25), 0);
        LayoutSetTextBoxFmtValue(this->field40, "txt_para23",
                      BdatTouchStringCell("MNU_item", "name", 0x2f), 0);
        LayoutSetTextBoxFmtValue(this->field40, "txt_have",
                      BdatTouchStringCell("MNU_collect", "name", 0x10), 0);

        // Clear three auxiliary labels.
        LayoutSetTextBoxFmtValue(this->field40, "txt_eth01", "", 0);
        LayoutSetTextBoxFmtValue(this->field40, "txt_eth02", "", 0);
        LayoutSetTextBoxFmtValue(this->field40, "txt_eth03", "", 0);

        // Seed the .sbss colour tables from the pane material colours and copy
        // the alpha shorts into the paired highlight entries.
        nw4r::lyt::Pane* pane443 = this->field40->GetRootPane()->
                                       FindPaneByName("txt_para01", true);
        GXColorS10 c0 = PaneMatGetS16Quad((nw4r::lyt::Pane*)pane443, 0);
        itemBoxLine_tabLabelNeutralTop = c0;
        GXColorS10 c1 = PaneMatGetS16Quad((nw4r::lyt::Pane*)pane443, 1);
        itemBoxLine_tabLabelNeutralBottom = c1;
        itemBoxLine_tabLabelSelectedTop.a = itemBoxLine_tabLabelNeutralTop.a;
        itemBoxLine_tabLabelSelectedBottom.a = itemBoxLine_tabLabelNeutralBottom.a;

        nw4r::lyt::Pane* pane4A6 = this->field40->GetRootPane()->
                                       FindPaneByName("txt_value02", true);
        GXColorS10 c2 = PaneMatGetS16Quad((nw4r::lyt::Pane*)pane4A6, 0);
        itemBoxLine_tabValueNeutralTop = c2;
        GXColorS10 c3 = PaneMatGetS16Quad((nw4r::lyt::Pane*)pane4A6, 1);
        itemBoxLine_tabValueNeutralBottom = c3;
        itemBoxLine_tabValueSelectedTop.a = itemBoxLine_tabValueNeutralTop.a;
        itemBoxLine_tabValueSelectedBottom.a = itemBoxLine_tabValueNeutralBottom.a;

        nw4r::lyt::Pane* pane76B = this->field40->GetRootPane()->
                                       FindPaneByName("txt_num01", true);
        GXColorS10 c4 = PaneMatGetS16Quad((nw4r::lyt::Pane*)pane76B, 0);
        itemBoxLine_pageLabelNeutralTop = c4;
        GXColorS10 c5 = PaneMatGetS16Quad((nw4r::lyt::Pane*)pane76B, 1);
        itemBoxLine_pageLabelNeutralBottom = c5;
        itemBoxLine_pageLabelSelectedTop.a = itemBoxLine_pageLabelNeutralTop.a;
        itemBoxLine_pageValueSelectedTop.a = itemBoxLine_pageLabelNeutralTop.a;
        itemBoxLine_pageLabelSelectedBottom.a = itemBoxLine_pageLabelNeutralBottom.a;
        itemBoxLine_pageValueSelectedBottom.a = itemBoxLine_pageLabelNeutralBottom.a;

        // Build the four embedded cursors from stack temps (copy + teardown).
        this->mCur70 = CCur18(CUICfManager_getArcResourceAccessor());
        this->mCur70.initLayout();

        this->mCur88 = CCur18(CUICfManager_getArcResourceAccessor());
        this->mCur88.initLayout();

        this->mCurA0 = CIBLTabCur((nw4r::lyt::ArcResourceAccessor*)this->field38);
        this->mCurA0.initLayout();

        this->mCurB8 = CSubCur((nw4r::lyt::ArcResourceAccessor*)this->field38);
        ((CBaseCur*)&this->mCurB8)->initLayout();

        this->mCurB8.ShowTwoSubPanes(0);
        CompactTabList();
        this->field24 = 0;
        (&mRegion04)->validateHeap();
        return true;
    }
    if (this->field28 == (u32)evt->mFileHandle) {
        (&mRegion14)->createRegion(
            mtl::MemManager::getHandleMEM2(), 0x100, "CItemBoxLineTex", 0);
        Class_8045F858 host2((&mRegion14));
        CFileHandle* fh28 = (CFileHandle*)this->field28;
        u8* data28 = (u8*)fh28->getData();
        mtl::MemManager::setMemInitFlag(false);
        nw4r::lyt::ArcResourceAccessor* acc2 =
            CLibLayout::createArcResourceAccessor();
        this->field3C = acc2;
        acc2->Attach(data28, "arc");
        CompactTabList();
        this->field28 = 0;
        (&mRegion14)->validateHeap();
        return true;
    }
    if (this->field2C == (u32)evt->mFileHandle) {
        CFileHandle* fh2C = (CFileHandle*)this->field2C;
        u8* data2C = (u8*)fh2C->getData();
        Bdat_GetTable_AA34();
        void* fpA = getFP("MNU_item_mes_a");
        if (fpA == 0) {
            CBdat::setBdatEntry(2, data2C);
        }
        Bdat_GetTable_AA34();
        gItemBoxLineMnuItemMesATable = (u32)getFP("MNU_item_mes_a");
        Bdat_GetTable_AA34();
        gItemBoxLineMnuItemMesBTable = (u32)getFP("MNU_item_mes_b");
        CompactTabList();
        this->field2C = 0;
        return true;
    }
    if (this->field30 == (u32)evt->mFileHandle) {
        CFileHandle* fh30 = (CFileHandle*)this->field30;
        u8* data30 = (u8*)fh30->getData();
        this->field34 = data30;
        Bdat_GetTable_AA34();
        void* fpB = getFP("MNU_skill");
        if (fpB == 0) {
            CBdat::setBdatEntry(5, this->field34);
        }
        gItemBoxLineMnuSkillTable = (u32)getFP("MNU_skill");
        CompactTabList();
        this->field30 = 0;
        return true;
    }
    return false;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// ============================================================================
// sinit_801F32EC: static initializer for the fourteen 8-byte .sbss color
// objects at 0x806645D8..0x80664640 - reset via SplitU32ToS16s(ptr, 0) or set
// the RGBA via setGXColorS10(ptr, r, g, b, 0), in retail call order.
// ============================================================================
void sinit_801F32EC() {
    SplitU32ToS16s(&itemBoxLine_tabLabelNeutralTop, 0);
    SplitU32ToS16s(&itemBoxLine_tabLabelNeutralBottom, 0);
    setGXColorS10(&itemBoxLine_tabLabelSelectedTop, 0x80, 0x80, 0x80, 0);
    setGXColorS10(&itemBoxLine_tabLabelSelectedBottom, 0x80, 0x80, 0x80, 0);
    SplitU32ToS16s(&itemBoxLine_tabValueNeutralTop, 0);
    SplitU32ToS16s(&itemBoxLine_tabValueNeutralBottom, 0);
    setGXColorS10(&itemBoxLine_tabValueSelectedTop, 0xff, 0xff, 0xfa, 0);
    setGXColorS10(&itemBoxLine_tabValueSelectedBottom, 0x80, 0x80, 0x80, 0);
    SplitU32ToS16s(&itemBoxLine_pageLabelNeutralTop, 0);
    SplitU32ToS16s(&itemBoxLine_pageLabelNeutralBottom, 0);
    setGXColorS10(&itemBoxLine_pageLabelSelectedTop, 0x12, 0xa3, 0xe7, 0);
    setGXColorS10(&itemBoxLine_pageLabelSelectedBottom, 0xff, 0xff, 0xff, 0);
    setGXColorS10(&itemBoxLine_pageValueSelectedTop, 0xb3, 0x9, 0xc0, 0);
    setGXColorS10(&itemBoxLine_pageValueSelectedBottom, 0xff, 0xff, 0xff, 0);
}

// Retail ItemBoxLine_InitTabFormat calls these as external relocs.
extern "C" CIBLTabEntry* ItemBoxLine_InitTabEntry(CIBLTabEntry* self, u16 item, u32 value, u8 lock, u8 ready) {
    self->f0 = item;
    self->f4 = value;
    self->f8 = lock;
    self->f9 = ready;
    return self;
}

// ============================================================================
// ItemBoxLine_InitTabFormat: initialise a tab-format object - zero every entry via the
// entry writer, clear the count/selector/name/counter fields, then re-copy
// zero entries over the table through a stack temp (retail keeps the r3
// reuse from ItemBoxLine_InitTabEntry for the copy source).
// ============================================================================
void ItemBoxLine_InitTabFormat(CIBLTabFull* self) {
    CIBLTabEntry* e = self->entries;
    do {
        ItemBoxLine_InitTabEntry(e, 0, 0, 0, 0);
        e++;
    } while (e < &self->entries[12]);
    self->count = 0;
    self->field92 = 0;
    self->field93 = 0;
    self->str94.mString[0] = 0;   // +0x94 (name-buffer first byte)
    self->str94.mLength = 0;      // +0xB4
    self->strB8.mString[0] = 0;   // +0xB8 (name-buffer first byte)
    self->strB8.mLength = 0;      // +0x138
    CIBLTabEntry tmp;
    for (u16 i = 0; i < 12; i++) {
        CIBLTabEntry* src = ItemBoxLine_InitTabEntry(&tmp, 0, 0, 0, 0);
        ItemBoxLine_CopyTabEntry(&self->entries[i], src);
    }
}
// ============================================================================
// ItemBoxLine_ProbePageSlot: page-slot probe on a tab-format entry. Start from 99, then
// scan the entry's per-page capacity (f4) for the smallest page whose
// cumulative total exceeds the current count (CItemBlock_getPtr20E8), returning the
// page's item offset rounded via the float path; finally clamp against the
// kind-owner count (field92 == 0xD uses the item-name owner table instead).
// ============================================================================
u8 ItemBoxLine_ProbePageSlot(CIBLTabFull* self, u32 index) {
    int result = 0x63;
    if (index < self->count) {
        u16 f0 = self->entries[index].f0;
        CIBLTabEntry* entry = &self->entries[index];
        u32 cur = (u32)CItemBlock_getPtr20E8();
        for (u8 i = 1; i < 100; i++) {
            u32 prod = entry->f4 * i;
            if (cur < prod) {
                u32 diff = prod - cur;
                float ratio = (float)(s32)diff / (float)entry->f4;
                int n = (int)ratio;
                if (ratio - (float)n != lbl_eu_806680F8) n++;
                result = (u8)i - n;
                break;
            }
        }
        if (self->field92 == 0xd) {
            int v2 = 0x63 - (int)CItem_sumFamilyByte6(f0);
            if (v2 < result) result = v2;
        } else {
            int v = (int)CItemBlock_countEmpty(self->field92);
            if (v < result) result = v;
        }
    }
    return (u8)result;
}

// .sbss UI colour objects owned by this TU (retail 0x806645D8..0x80664640),
// declared in retail address order at the top of this TU.
GXColorS10 itemBoxLine_tabLabelNeutralTop;
GXColorS10 itemBoxLine_tabLabelNeutralBottom;
GXColorS10 itemBoxLine_tabLabelSelectedTop;
GXColorS10 itemBoxLine_tabLabelSelectedBottom;
GXColorS10 itemBoxLine_tabValueNeutralTop;
GXColorS10 itemBoxLine_tabValueNeutralBottom;
GXColorS10 itemBoxLine_tabValueSelectedTop;
GXColorS10 itemBoxLine_tabValueSelectedBottom;
GXColorS10 itemBoxLine_pageLabelNeutralTop;
GXColorS10 itemBoxLine_pageLabelNeutralBottom;
GXColorS10 itemBoxLine_pageLabelSelectedTop;
GXColorS10 itemBoxLine_pageLabelSelectedBottom;
GXColorS10 itemBoxLine_pageValueSelectedTop;
GXColorS10 itemBoxLine_pageValueSelectedBottom;
