#define IWORK_EVENT_INLINE_DTOR
// Auto-scaffolded catalog TU for kyoshin/CItemBoxGrid
// Replace stubs with high-level C/C++ during decomp.

// Real item-impl owner (kyoshin/cf/CItem.hpp): the harness chain declares a
// stale extern "C" void* CItem_initItemImplInstances that conflicts with the
// real CItemImpl*(CItemData*) form, so it is renamed away for this TU (same
// workaround as CEquipChange.cpp).
#define CItem_initItemImplInstances CItem_initItemImplInstances_staleVoid
#include "kyoshin/cf/CfBdat.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/code_8025FB10.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "kyoshin/CItemBoxGrid.hpp"
#include "kyoshin/makecrystal/CMenuMakeCrystal.hpp"
#include "kyoshin/CPresentWinApi.hpp"
#include "kyoshin/cf/CtrlObjectParam.hpp"
#include "kyoshin/CNumSelect.hpp"
#include "kyoshin/CItemBoxInfo.hpp"
#include "kyoshin/CSortMenu.hpp"
#include <new>
#include "kyoshin/CUIWindowManagerApi.hpp"
#include "kyoshin/CExchangeWin.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"
#undef CItem_initItemImplInstances
// CItemBoxGrid.hpp declares these two accessors with a stale void*/u32 view
// that CItem.hpp (the owner) declares as CItemExt*/s32; the owning header is
// not canonicalised yet, so CItem.hpp's copies are renamed for this include.
#define CItem_slotCnt CItem_slotCnt_grid_stale
#include "kyoshin/cf/CItem.hpp"
#undef CItem_slotCnt
#include "kyoshin/CSysWin.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/device/CDeviceSC.hpp"

#include <stdio.h>
#include <string.h>
#include <nw4r/lyt.h>
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/CfEnumListHolder.hpp"
#include "monolib/util/MemManager.hpp"  // canonical mtl::MemManager::allocate
#include "kyoshin/CTitleAHelp.hpp"  // setGXColorS10 (defined in CTitleAHelp.cpp)

// Retail helper forms used by this TU (kept out of CItemBoxGrid.hpp: owners
// that also include CItemBoxInfo.hpp declare them with other parameter types).


// Forward declarations for external functions
void* __dt__13CArtsBookItemFv(void* self, int mode);
void* __dt__11CVisionItemFv(void* self, int mode);
void* __dt__10CQuestItemFv(void* self, int mode);


// Forward declarations for functions defined later in this file
extern "C" long RoundHalfAway0(double f);
extern "C" {
void CopyTabSlotVec(u32*, void*, u32);
}
void CopyTabSlotVec(u32*, void*, u32);
void deleteRegion__17UnkClass_8045F564Fv(void*);
// .sdata2 conversion magics referenced by ((CItemBoxGridFull*)(retail pool
// constants; F48 = signed-int magic 0x4330000080000000))->GetItemValueFloat(F40 = plain 2^52).
extern const double lbl_eu_80667F40;
extern const double lbl_eu_80667F48;
extern const float lbl_eu_80667F34;
extern const float lbl_eu_80667F88;
extern const float lbl_eu_80667F8C;
extern const float lbl_eu_80667F98;
extern const double lbl_eu_80667F60;
extern const double lbl_eu_80667F68;

extern u32 itemBoxGrid_slotLabelNeutralTop;
extern u32 itemBoxGrid_slotLabelNeutralBottom;
extern u32 itemBoxGrid_slotLabelUpTop;
extern u32 itemBoxGrid_slotLabelUpBottom;
extern u32 itemBoxGrid_slotLabelDownTop;
extern u32 itemBoxGrid_slotLabelDownBottom;
extern u32 itemBoxGrid_slotValueNeutralTop;
extern u32 itemBoxGrid_slotValueNeutralBottom;
extern u32 itemBoxGrid_slotValueUpTop;
extern u32 itemBoxGrid_slotValueUpBottom;
extern u32 itemBoxGrid_shopLabelNeutralTop;
extern u32 itemBoxGrid_shopLabelNeutralBottom;
extern u32 itemBoxGrid_shopLabelSelectedTop;
extern u32 itemBoxGrid_shopLabelSelectedBottom;
extern u32 itemBoxGrid_shopValueSelectedTop;
extern u32 itemBoxGrid_shopValueSelectedBottom;
extern u32 itemBoxGrid_shopItemTypeTable;
extern u32 itemBoxGrid_shopSkillTable;
extern u32 itemBoxGrid_shopItemIdTable;

extern const char itemBoxGridCategoryFilterWords[];
extern const char itemBoxGridLanguageSlotWords[];
extern const char itemBoxGridPriceScaleRow[];
extern const char itemBoxGridSlotWordTable[];
extern const char lbl_eu_80573D18[];
extern const char lbl_eu_8050566C[];
extern u32 itemBoxGridCurrentOwner;
// 4-byte .sbss pointer (symbols.txt size:0x4). A large u8[] decl forces
// lis/addi; void* keeps it SDA-eligible (EMB_SDA21 lwz like retail).
// Forward declarations for later-defined functions
void CopyTabSlotVec(u32*, void*, u32);


// Externs for LoadItemBoxFiles and UnloadItemBox

// Externs for state dispatch functions

// TU-local cast-only views over grid-family objects (CItemBoxGridFull in the
// header covers the 0x2800 cell array + count/category/row/page fields):
// - CibgIdList: the u16 id pool shared by the CVisionItem/CQuestItem/
//   CArtsBookItem objects (ids at +0x04, count at +0x804).
// - CibgGridTail: grid words past CItemBoxGridFull (page index at +0x2804,
//   lookup window at +0x28A5 indexed by that page value).
// - CibgOuterSlots: outer-object member slots destroyed by
//   __dt__12CItemBoxGridFv (each slot starts at its dtor offset and runs to
//   the next slot's start; only the starts are load-bearing).
// - CibgTabSlot12: 12-byte tab-slot payload copied by CopyTabSlotVec.
struct CibgIdList {
    u8 _00[4];
    u16 ids[0x400]; // +0x04, ends +0x804
    u16 count;      // +0x804
};
struct CibgGridTail {
    u8 _00[0x2804];
    s8 page2804;              // +0x2804 page index
    u8 _2805[0x28A5 - 0x2805];
    u8 win28A5[0x80];         // +0x28A5 indexed by page2804
};
struct CibgOuterSlots {
    u8 _00[0x08];
    u8 m08[0x10];   // +0x08
    u8 m18[0x58];   // +0x18
    u8 m70[0x18];   // +0x70
    u8 m88[0x18];   // +0x88
    CCur18 mA0;     // +0xA0
    u8 mB8[0x18];   // +0xB8
    u8 mD0[0x18];   // +0xD0
    CSortMenu mSortMenu; // +0xE8
    u8 m1D8[0x20C]; // +0x1D8
    u8 m3E4[0x34];  // +0x3E4
    u8 m418[0x28];  // +0x418
    u8 m440[0x28];  // +0x440
    u8 m468[0x44];  // +0x468
    u8 m4AC[0x3C];  // +0x4AC
    u8 m4E8[0x64];  // +0x4E8
    u8 m54C[4];     // +0x54C grid storage follows; address-taken only
};
struct CibgTabSlot12 {
    u32 w[3]; // 12-byte tab-slot payload
};

int CItemBoxGrid::GetField61() { return field_61; }

// Id-pool lists (CQuestItem base, CVisionItem/CArtsBookItem leaves).
// Scan item ids 0x220+ grouped by the category ranges returned by
// GetU16PairEntry and register teachable entries: an item qualifies when its
// type-specific name row (base-name key, or the +0xc8 teach key, or a free
// ability slot) equals 2 and the paired look-up key is present, in which
// case PushUniqueItemId pushes the id.
// Direct global references (no pointer locals): retail keeps two webs for
// lbl_eu_8050566C - an @ha base copy and a full-pointer copy - which only
// appears when the pool is referenced directly instead of through a local.
#define XB_K ((const char*)lbl_eu_8050566C)
#define XB_TABLES ((const void* const*)lbl_eu_80573D18)
void CQuestItem::Rebuild() {
    // C89-style declarations: MWCC births virtual registers in declaration
    // order when locals are hoisted, which is the shape retail claims.
    void* tbl;
    u32 cat;
    u32 id;
    u16 end;
    u8 tp;
    u16 v;
    mCount = 0;
    for (cat = 1; cat < 0x1b; cat++) {
        u32 start = GetU16PairEntry((u16)cat);
        if ((u16)start == 0) continue;
        end = GetU16PairEntry((u16)(cat + 1));
        for (id = start; (u16)id < end; id++) {
            tp = CtrlRemote_TouchBitByArg((u16)id + 0x220);
            if (tp >= 0xFE || tp == 0 || tp == 0xC8) continue;
            tbl = (void*)XB_TABLES[SearchThresholdIdx((u16)id)];
            if ((u8)BdatGetU8Direct((const void*)tbl, "type_succ_B1", (u16)id)) {
                // Retail pushes the masked lookup value, not the raw item id:
                // each `clrlwi. r4, r3, 16` result feeds PushUniqueItemId's r4.
                #define XB_PUSH(tblArg, kOff) \
                do { \
                    v = BdatGetU16Direct((const void*)(tblArg), &XB_K[kOff], (u16)id); \
                    if (v) PushUniqueItemId(v); \
                } while (0)
                if (tp == 1) {
                    // Both lookups are issued before either result is tested.
                    u8 c1 = BdatGetU8Direct((const void*)tbl, "type_succ_A1", (u16)id);
                    u8 c2 = BdatGetU8Direct((const void*)tbl, "type_succ_B1", (u16)id);
                    if (c1 == 2) XB_PUSH(tbl, 0x1a);
                    if (c2 == 2) XB_PUSH(tbl, 0x26);
                } else if (tp == 0x6f) {
                    if ((u8)BdatGetU8Direct((const void*)tbl, "type_succ_A2", (u16)id) == 2) XB_PUSH(tbl, 0x3f);
                } else if (tp == 0x79) {
                    if ((u8)BdatGetU8Direct((const void*)tbl, "type_succ_A3", (u16)id) == 2) XB_PUSH(tbl, 0x58);
                } else if (tp == 0x83) {
                    if ((u8)BdatGetU8Direct((const void*)tbl, "type_succ_A4", (u16)id) == 2) XB_PUSH(tbl, 0x71);
                } else if (tp == 0x70) {
                    if ((u8)BdatGetU8Direct((const void*)tbl, "type_succ_B2", (u16)id) == 2) XB_PUSH(tbl, 0x8a);
                } else if (tp == 0x7a) {
                    if ((u8)BdatGetU8Direct((const void*)tbl, "type_succ_B3", (u16)id) == 2) XB_PUSH(tbl, 0xa3);
                } else if (tp == 0x84) {
                    if ((u8)BdatGetU8Direct((const void*)tbl, "type_succ_B4", (u16)id) == 2) XB_PUSH(tbl, 0xbc);
                }
            } else if ((u8)BdatGetU8Direct((const void*)tbl, "order_succ_A", (u16)id)) {
                // Teach-key dispatch (subset of types).
                if (tp == 1) {
                    if ((u8)BdatGetU8Direct((const void*)tbl, "type_succ_A1", (u16)id) == 2) XB_PUSH(tbl, 0x1a);
                } else if (tp == 0x6f) {
                    if ((u8)BdatGetU8Direct((const void*)tbl, "type_succ_A2", (u16)id) == 2) XB_PUSH(tbl, 0x3f);
                } else if (tp == 0x79) {
                    if ((u8)BdatGetU8Direct((const void*)tbl, "type_succ_A3", (u16)id) == 2) XB_PUSH(tbl, 0x58);
                } else if (tp == 0x83) {
                    if ((u8)BdatGetU8Direct((const void*)tbl, "type_succ_A4", (u16)id) == 2) XB_PUSH(tbl, 0x71);
                }
            } else {
                // Fallback: no base name and no teach key - check ability slots
                u32 obj = (u32)UIWin_BuildFlagBuf((u16)id);
                if (!UIWin_QueryItemAvail((CItemQuery*)obj, 0, 0)) {
                    if ((u8)BdatGetU8Direct((const void*)tbl, "type_succ_A1", (u16)id) == 2) XB_PUSH(tbl, 0x1a);
                }
                if (!UIWin_QueryItemAvail((CItemQuery*)obj, 0, 1)) {
                    if ((u8)BdatGetU8Direct((const void*)tbl, "type_succ_A2", (u16)id) == 2) XB_PUSH(tbl, 0x3f);
                }
                if (!UIWin_QueryItemAvail((CItemQuery*)obj, 0, 2)) {
                    if ((u8)BdatGetU8Direct((const void*)tbl, "type_succ_A3", (u16)id) == 2) XB_PUSH(tbl, 0x58);
                }
                if (!UIWin_QueryItemAvail((CItemQuery*)obj, 0, 3)) {
                    if ((u8)BdatGetU8Direct((const void*)tbl, "type_succ_A4", (u16)id) == 2) XB_PUSH(tbl, 0x71);
                }
            }
        }
    }
}
// Add id to list if not already present and capacity check passes.
// noinline: RegisterLearnedSkills (and the other callers in this TU) must emit `bl`
// to the retail symbol - the unit builds with -inline and would otherwise
// fold this body into every call site (tripling RegisterLearnedSkills).
void CQuestItem::PushUniqueItemId(u32 id) {
    if (HasSlotIdMatch(id)) return;
    if ((BdatGetItemType(id) & 0xFFFF) == 12) return;
    u16 count = mCount;
    mIds[count] = (u16)id;
    mCount = count + 1;
}

// Search for a matching short id in an array, return 1 if found.
// noinline: retail PushUniqueItemId keeps a `bl HasSlotIdMatch` call; u32 id
// (retail compares the full register, no call-site mask).
int CQuestItem::HasSlotIdMatch(u32 id) {
    u16 count = mCount;
    u16 i = 0;
    while (i < count) {
        if (mIds[i] == id) return 1;
        i++;
    }
    return 0;
}

CVisionItem::CVisionItem() {
}

CVisionItem::~CVisionItem() {
}

void CVisionItem::Rebuild() {
    void* bdat = getFP("MMU_questeve");
    if (!bdat) return;

    u16 count = (u16)Bdat_GetMaxRow_B1EC(bdat);
    u16 i;
    for (i = 1; i <= count; i++) {
        if (!CtrlRemote_TouchBitByArg((u32)(i + 0x2596))) continue;

        u32 flag1 = BdatGetU16Direct(bdat, (const char*)"questID", (u32)i);
        if (flag1 & 0xFFFF) {
            u8 val = (u8)CtrlRemote_TouchBitByArg((flag1 & 0xFFFF) + 0x220);
            if (val >= 0xFE) continue;
            if (val > (u8)BdatGetU8Direct((const void*)bdat, (const char*)"questval", (u32)i)) continue;
        } else {
            u32 hasVal = BdatGetU8Direct((const void*)bdat, (const char*)"col6ID", (u32)i);
            if (hasVal & 0xFF) {
                if ((u8)BdatGetU8Direct((const void*)bdat, (const char*)"col6val", (u32)i)
                    <= (u8)CtrlRemote_TouchBitByArg((hasVal & 0xFF) + 0x7fc)) continue;
            }
        }

        u32 type = BdatGetU16Direct(bdat, (const char*)"itemID", (u32)i);
        if (type & 0xFFFF) {
            PushUniqueItemId(type & 0xFFFF);
        }
    }
}

// Scan the skill-learn table (::cf::CfBdat::spItmArtsListFileData) for entries 1..count and
// push each one into the list when its per-party table cell has the
// "learned" flag bit set. The cell address is charData + party * 0x49 +
// dialogType * 2; bits at +0xE8/+0xE9 select the check by category.

CArtsBookItem::CArtsBookItem() {
}

CArtsBookItem::~CArtsBookItem() {
}

void CArtsBookItem::Rebuild() {
    u32 bdat= (u32)::cf::CfBdat::spItmArtsListFileData;
    s32 count = (s32)Bdat_GetMaxRow_B1EC((void*)bdat);
    u8 i;
    for (i = 1; i <= count; i++) {
        u32 flag = BdatGetU8Direct((const void*)bdat, (const char*)"memory_type", (u32)i);
        u32 charId = BdatGetU8Direct((const void*)bdat, (const char*)"pc_type", (u32)i);
        u32 unk = BdatGetU8Direct((const void*)bdat, (const char*)"get_arts", (u32)i);
        u32 sub = BdatGetU8ByTableKey((const char*)"pc_arts", (const char*)"idx",
                            (u8)unk);
        // Reuse the charId slot for the character-data pointer (retail
        // recycles its register); then add the party stride and sub offset.
        charId = (u32)CtrlObjectParam_GetCharEntry((u8)charId);
        u8* cell = (u8*)charId + (u8)CtrlObjectParam_GetCurrentRowKey((void*)charId) * 0x49;
        cell = cell + (u8)sub * 2;
        switch ((u8)flag) {
        case 1:
            if (cell[0xE8] != 0) {
                PushUniqueItemId((u32)i);
            }
            break;
        case 2:
            if ((cell[0xE9] >> 7) & 1) {
                PushUniqueItemId((u32)i);
            }
            break;
        case 3:
            if ((cell[0xE9] >> 6) & 1) {
                PushUniqueItemId((u32)i);
            }
            break;
        }
    }
}

// Constructor for the cell grid: the cell array is built through the
// (out-of-line) cell ctor with its default arguments, the metadata bytes and
// the three id-pool lists follow in declaration order, then a blank cell
// temporary is stamped over every cell via CopyGridEntry9 (retail keeps
// both helper calls as `bl` and copies from the ctor's returned pointer).
CItemBoxGridFull::CItemBoxGridFull()
    : field_2800(0), field_2802(0), field_2803(0), field_2804(0) {
    itemBoxGridCurrentOwner = (u32)this;
    for (u16 i = 0; i < 0x400; i++) {
        CopyGridEntry9(&entries[i], (void*)&(const CItemBoxGridEntry&)CItemBoxGridEntry(-1, 0, 0, 0, 0, 0, 0, 0));
    }
}

// Cell ctor (retail SetEntry9Bytes).
CItemBoxGridEntry::CItemBoxGridEntry(s16 a, u8 b, u8 c, u8 d, u8 e, u8 f, u8 g, bool h) {
    id = a;
    flags[0] = b;
    flags[1] = c;
    flags[2] = d;
    flags[3] = e;
    flags[4] = f;
    flags[5] = g;
    mMarked = h;
}

// Copy a 9-byte entry from src to dst. noinline: retail callers emit `bl`
// (the unit builds with -inline and would otherwise fold this body in).
extern "C" void CopyGridEntry9(void* dst, void* src) {
    volatile u8* d = (volatile u8*)dst;
    volatile u8* s = (volatile u8*)src;
    short h = *(volatile short*)(s + 0);
    u8 b0 = s[2];
    u8 b1 = s[3];
    u8 b2 = s[4];
    u8 b3 = s[5];
    u8 b4 = s[6];
    u8 b5 = s[7];
    u8 b6 = s[8];
    *(volatile short*)(d + 0) = h;
    d[2] = b0;
    d[3] = b1;
    d[4] = b2;
    d[5] = b3;
    d[6] = b4;
    d[7] = b5;
    d[8] = b6;
}

// Destructor for the cell grid: clears the active-grid global; the
// CArtsBookItem/CVisionItem members are destroyed after the body.
// -O4,s: retail uses stmw r30/lmw r30 block save (not individual stw).
CItemBoxGridFull::~CItemBoxGridFull() {
    itemBoxGridCurrentOwner = 0;
}


// C linkage: retail exports this helper under its plain address-name symbol.
// C linkage: retail exports this helper under its plain address-name symbol.
void CItemBoxGridFull::PopulateGridEntries(u8 cat, int ownerIdx, int refItemId, int valueMode) {
    u8* p = (u8*)this;
    u32 i;
    ((CItemBoxGridFull*)p)->field_2800 = 0;
    // Reset the three sub-objects through their vtables (+0x0C method).
    ((CVisionItem*)(p + 0x34b0))->Rebuild();
    ((CQuestItem*)(p + 0x2ca8))->Rebuild();
    ((CArtsBookItem*)(p + 0x3cb8))->Rebuild();
    // Clear all 0x400 cells to blank entries and stamp the hidden-flag row.
    for (u16 j = 0; j < 0x400; j++) {
        CopyGridEntry9(&entries[j], (void*)&(const CItemBoxGridEntry&)CItemBoxGridEntry(-1, 0, 0, 0, 0, 0, 0, 0));
        field_28A5[j] = 3;
    }
    u32 total = CItemBlock_countKindSlots(cat);
    // Read 24 bytes from itemBoxGridCategoryFilterWords (6 category filter entries)
    GridFilter24 filterBuf = *(const GridFilter24*)&itemBoxGridCategoryFilterWords;
    u16 id;
    for (id = 0; (u32)id < total; id++) {
        void* item = CItem_kindPtr(cat, id);
        if (!item) continue;
        if (!*(u32*)item) continue;
        if (refItemId) {
            u32 v1 = ((CItemBoxGridFull*)((CItemBoxGridFull*)this))->GetScaledItemValue(refItemId, 0, (int)valueMode);
            u32 kind = (*(u32*)item) >> 20;
            u32 v2 = ((CItemBoxGridFull*)((CItemBoxGridFull*)this))->GetScaledItemValue(kind, item, (int)valueMode);
            if (v1 > v2) continue;
        }
        u16 count = ((CItemBoxGridFull*)p)->field_2800;
        u32 off = count * 10;
        ((CItemBoxGridFull*)p)->field_2800 = count + 1;
        *(s16*)(p + off) = id;
        u8* entry = p + off;
        if (cat == 3) {
            GridFilter24 filterCopy = filterBuf;
            u8 subCatIdx;
            for (subCatIdx = 0; subCatIdx < 6; subCatIdx++) {
                u8 subCat = (u8)filterCopy.w[subCatIdx];
                u32 sc = CItemBlock_countKindSlots(subCat);
                u16 subId;
                for (subId = 0; (u32)subId < sc; subId++) {
                    void* subItem = CItem_kindPtr(subCat, subId);
                    if (!subItem) continue;
                    if (!*(u32*)subItem) continue;
                    CItemImpl* inst = CItem_initItemImplInstances((CItemData*)subItem);
                    u16 numSlots = (u16)inst->vf30((CItemData*)subItem);
                    u32 slot;
                    u8 matched = 0;
                    u16 keptSubId = 0;
                    for (slot = 0; slot < (u32)numSlots; slot++) {
                        CItemImpl* inst2 = CItem_initItemImplInstances((CItemData*)subItem);
                        s16 equipId = (s16)inst2->vf40((CItemData*)subItem, slot);
                        if (equipId == -1) continue;
                        if (equipId == (s16)id) {
                            entry[2] = 2;
                            keptSubId = (u16)subId;
                            matched = 1;
                            break;
                        }
                    }
                    if (matched) {
                        u8 partyIdx;
                        for (partyIdx = 0; partyIdx < (u8)code80135FDC_getByte_64077(); partyIdx++) {
                            u32 pn = GetCollectedFlagByte(partyIdx);
                            void* obj2 = (void*)CtrlObjectParam_GetCharEntry(pn & 0xFF);
                            s16 equipCheck = 0;
                            switch (subCat) {
                                case 2: equipCheck = ((CItemBoxCharEntry*)obj2)->slotIds[5]; break;
                                case 4: equipCheck = ((CItemBoxCharEntry*)obj2)->slotIds[0]; break;
                                case 5: equipCheck = ((CItemBoxCharEntry*)obj2)->slotIds[1]; break;
                                case 6: equipCheck = ((CItemBoxCharEntry*)obj2)->slotIds[2]; break;
                                case 7: equipCheck = ((CItemBoxCharEntry*)obj2)->slotIds[3]; break;
                                case 8: equipCheck = ((CItemBoxCharEntry*)obj2)->slotIds[4]; break;
                            }
                            if (equipCheck == (s16)keptSubId) {
                                entry[2] = 1;
                                break;
                            }
                        }
                        goto entry_done;
                    }
                }
            }
            goto entry_done;
        }
        if (cat >= 2 && cat <= 8) {
            if (ownerIdx) {
                u32 ownerTbl;
                switch (cat) {
                    case 2: ownerTbl = (u32)::cf::CfBdat::spItmWpnListFileData; break;
                    default: ownerTbl = (u32)::cf::CfBdat::spItmEquipListFileData; break;
                }
                u32 kindId = (*(u32*)item) >> 20;
                u16 shortKind = BdatGetItemId(kindId);
                char nameBuf[64];
                const char* fmt = (cat == 2) ? "equip_pc%d" : "pc%d";
                sprintf(nameBuf, fmt, ownerIdx);
                if (!BdatGetU8Direct((const void*)ownerTbl, (const char*)nameBuf, shortKind & 0xFFFF)) {
                    *(s16*)entry = -1;
                    count = ((CItemBoxGridFull*)p)->field_2800;
                    ((CItemBoxGridFull*)p)->field_2800 = count - 1;
                    goto check_next;
                }
            }
            u8 partyIdx;
            for (partyIdx = 0; partyIdx < (u8)code80135FDC_getByte_64077(); partyIdx++) {
                u32 pn = GetCollectedFlagByte(partyIdx);
                void* obj3 = (void*)CtrlObjectParam_GetCharEntry(pn & 0xFF);
                s16 equipCheck = 0;
                switch (cat) {
                    case 2: equipCheck = ((CItemBoxCharEntry*)obj3)->slotIds[5]; break;
                    case 4: equipCheck = ((CItemBoxCharEntry*)obj3)->slotIds[0]; break;
                    case 5: equipCheck = ((CItemBoxCharEntry*)obj3)->slotIds[1]; break;
                    case 6: equipCheck = ((CItemBoxCharEntry*)obj3)->slotIds[2]; break;
                    case 7: equipCheck = ((CItemBoxCharEntry*)obj3)->slotIds[3]; break;
                    case 8: equipCheck = ((CItemBoxCharEntry*)obj3)->slotIds[4]; break;
                }
                if (equipCheck == (s16)*(s16*)entry) {
                    if ((pn & 0xFF) > 8) entry[3] = 1;
                    entry[2] = 1;
                    entry[8] = 1;
                    goto entry_done;
                }
            }
        }
entry_done:
        if (cat != 3 && cat != 9) {
            if (!entry[3]) {
                u32 kind2 = (*(u32*)item) >> 20;
                u32 check = BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData, "item_keep", kind2);
                u32 result = check;
                u32 oneIfNonZero = (result != 0) ? 1 : 0;
                entry[3] = (u8)oneIfNonZero;
            }
            u32 kind3 = (*(u32*)item) >> 20;
            entry[5] = (u8)this->mVisionItems.HasSlotIdMatch(kind3);
            entry[4] = (u8)this->mQuestItems.HasSlotIdMatch(kind3);
            if (cat == 0xd) {
                u16 shortKind = BdatGetItemId(kind3);
                entry[6] = (u8)this->mArtsBookItems.HasSlotIdMatch(shortKind & 0xFFFF);
            }
        }
check_next:;
    }
    // Update grid metadata
    if (((CItemBoxGridFull*)p)->field_2802 != cat) {
        u32 cnt = ((CItemBoxGridFull*)p)->field_2800;
        ((CItemBoxGridFull*)p)->field_2802 = cat;
        // Compute row count: cnt / 30, rounded up with tie-breaking
        float fcnt = (float)(s32)cnt;
        float f30 = 30.0f;
        float fdiv = fcnt / f30;
        int rows = (int)fdiv;
        float frac = fdiv - (float)rows;
        if (0.0f != frac) rows++;
        if (!(u8)rows) rows = 1;
        ((CItemBoxGridFull*)p)->field_2803 = (u8)rows;
        ((CItemBoxGridFull*)p)->field_2804 = 0;
    } else {
        u32 cnt = ((CItemBoxGridFull*)p)->field_2800;
        float fcnt = (float)(s32)cnt;
        float f30 = 30.0f;
        float fdiv = fcnt / f30;
        int rows = (int)fdiv;
        float frac = fdiv - (float)rows;
        if (0.0f != frac) rows++;
        if (!(u8)rows) rows = 1;
        ((CItemBoxGridFull*)p)->field_2803 = (u8)rows;
        if ((s8)((CItemBoxGridFull*)p)->field_2804 >= (s8)((CItemBoxGridFull*)p)->field_2803) {
            ((CItemBoxGridFull*)p)->field_2804 = ((CItemBoxGridFull*)p)->field_2803 - 1;
        }
    }
    // For equipment/armor categories (2-8) or category 11: set overlay byte
    u8 adjusted = (u8)(cat + 0xfe);
    if (adjusted <= 6 || cat == 0xb) {
        u32 cnt = CItemBlock_countKindSlots(cat);
        u32 base = CItem_slotCnt(cat);
        s32 last = (s32)cnt - 1;
        s32 diff = (s32)cnt - (s32)base;
        s32 k;
        if (last >= diff) {
            for (k = last; k >= diff; k--) {
                ((CItemBoxGridFull*)p)->entries[k].flags[5] = 1;
            }
        }
        // Compute some threshold
        float fcnt2 = (float)(s32)cnt;
        float fmul = 0.1f * fcnt2;
        int threshold = (int)fmul;
        u32 j = 0;
        for (;;) {
            int diff2 = threshold - (j + 1) * 3;
            if (diff2 < 0) {
                ((CItemBoxGridFull*)p)->field_28A5[j] = (u8)(diff2 + 3);
                break;
            }
            j++;
        }
    }
    ((CItemBoxGridFull*)((CItemBoxGridFull*)this))->ResortGridEntries(0);
}


// Convert category to timing value - complex float math
// Compute grid row count.
u32 CItemBoxGridFull::CalcGridRowCount() {
    u8* p = (u8*)this;
    u32 val = CItemBlock_countKindSlots(p[0x2802]);
    float f = (float)(s32)val;
    if (f >= 200.0f) return 10;
    float div = f / 30.0f;
    s32 result = (s32)div;
    float frac = div - (float)result;
    if (0.0f != frac) result++;
    return (u8)result;
}

// Increment a sub-index counter; wrap to 0 when reaching the limit.
// noinline: retail callers emit `bl` (the unit builds with -inline).
void CItemBoxGridFull::IncGridPageIndex() {
    u8 idx = this->field_2804 + 1;
    this->field_2804 = idx;
    if ((s8)idx >= this->field_2803) {
        this->field_2804 = 0;
    }
}

// Decrement a sub-index counter; borrow from the limit field when underflowing.
// noinline: retail callers emit `bl` (the unit builds with -inline).
void CItemBoxGridFull::DecGridPageIndex() {
    u8 idx = this->field_2804 - 1;
    this->field_2804 = idx;
    if ((s8)idx < 0) {
        this->field_2804 = this->field_2803 - 1;
    }
}

// Lookup the item id stored in a grid cell; -1 when the cell is out of
// range or the referenced item object is invalid.
s16 CItemBoxGridFull::GetCellItemId(u16 idx) {
    s8 base = (s8)this->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset < this->field_2800) {
        u8* obj = (u8*)CItem_kindPtr(this->field_2802, this->entries[offset].id);
        if (obj && *(u32*)obj) {
            return this->entries[offset].id;
        }
    }
    return -1;
}

// Item sell price for the cell at page*0x1e+idx: 0 when the cell is out of
// range or the referenced item object is invalid. Otherwise the item's base
// price (GetItemBasePrice) scaled by kind type (0xB unchanged, 0xD half, else
// tenth), then multiplied by 100 + the party's skill-0x93 bonus and rounded.
u32 CItemBoxGridFull::GetCellSellPrice(u16 idx) {
    s8 base = (s8)this->field_2804;
    u32 offset = (u16)(base * 0x1e + idx);
    if (offset >= (u32)this->field_2800) return 0;

    void* obj = CItem_kindPtr(this->field_2802, this->entries[offset].id);
    if (!obj || !*(u32*)obj) return 0;

    u32 kind = *(u32*)obj >> 20;
    LookupItemTypeTable(kind);
    u16 type = BdatGetItemType(kind);
    BdatGetItemId(kind);
    u32 price = ((CItemBoxGridFull*)(this))->GetItemBasePrice(obj);

    if ((u32)(u16)type == 0xB) {
        // kind 0xB sells at the base price unchanged
    } else if ((u32)(u16)type == 0xD) {
        price = (u32)((double)price * 0.5f);
    } else {
        price = (u32)((double)price * 0.1f);
    }

    u32 basePrice = 0x64;
    u8 partyCount = code80135FDC_getByte_64077();
    u32 i;
    for (i = 0; (u32)(u8)i < (u32)partyCount; i++) {
        u32 member = GetCollectedFlagByte((u8)i);
        void* charData = (void*)CtrlObjectParam_GetCharEntry((u32)(u8)member);
        void* ptr = (void*)(((CItemBoxCharEntry*)charData)->counterArea);
        if (Counter_TestBit(ptr, 0x93)) {
            basePrice += IdTable_SumValues((CUnkObj8025FB10*)ptr, 0x93);
        }
    }

    return (u32)RoundHalfAway0(0.01f * (double)(price * basePrice));
}

// Round a double to nearest integer with .5 tie-breaking biased away from zero.
extern "C" long RoundHalfAway0(double f) {
    return (long)(f + (f > lbl_eu_80667F34 ? lbl_eu_80667F60 : lbl_eu_80667F68));
}

// Item value: base price from GetShopItemValue, then scaled by the party's
// equipment-skill bonus when a concrete item (not a bare kind) is passed.
// The category is a conditional dereference: retail keeps `obj = item ?
// item : 0` and re-tests item for the load, so both branches stay separate.
u32 CItemBoxGridFull::GetScaledItemValue(u32 id, void* item, int reserved) {
    void* obj = item ? item : 0;
    u32 cat;
    if (item) {
        cat = *(u32*)obj >> 20;
    } else {
        cat = id;
    }
    LookupItemTypeTable(cat & 0xFFFF);
    BdatGetItemType(cat & 0xFFFF);
    BdatGetItemId(cat & 0xFFFF);
    u32 result = ((CItemBoxGridFull*)(this))->GetShopItemValue(cat & 0xFFFF, item);

    if (item) {
        u32 base = 0x64;
        u8 partyCount = code80135FDC_getByte_64077();
        u8 i;
        for (i = 0; i < partyCount; i++) {
            u8 member = (u8)GetCollectedFlagByte(i);
            void* charData = (void*)CtrlObjectParam_GetCharEntry(member);
            void* ptr = (void*)(((CItemBoxCharEntry*)charData)->counterArea);
            if (Counter_TestBit(ptr, 0x8f)) {
                base += IdTable_SumValues((CUnkObj8025FB10*)ptr, 0x8f);
            }
        }
        result = (u32)RoundHalfAway0((float)(result * base) * 0.01f);
    }

    return result;
}

// Lookup entry, check category; return word >> 20 or 0.
u32 CItemBoxGridFull::GetCellKind(u16 idx) {
    s8 base = (s8)this->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset < this->field_2800) {
        s16 val = this->entries[offset].id;
        void* obj = CItem_kindPtr(this->field_2802, val);
        if (obj) {
            u32 w = *(u32*)obj;
            if (w) return w >> 20;
        }
    }
    return 0;
}

// Lookup entry, check category; return obj ptr or 0.
void* CItemBoxGridFull::GetCellItemPtr(u16 idx) {
    s8 base = (s8)this->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset < this->field_2800) {
        s16 val = this->entries[offset].id;
        void* obj = CItem_kindPtr(this->field_2802, val);
        if (obj && *(u32*)obj) {
            return obj;
        }
    }
    return 0;
}

// Check item entry validity based on category. Categories 4-8 and 2 share
// the equipment-slot scan (slot count via vt+0x30, free-slot probe via
// vt+0x40, equipped-state fetch via vt+0x2C); category 3 collapses the cell's
// state byte to a boolean. -O4,s merges the r29-r31 saves into
// retail's stmw prologue; all failure paths share one return-0 block at end.
s32 CItemBoxGridFull::CheckCellState(u16 idx) {
    s8 base = (s8)this->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset >= this->field_2800) goto fail;

    CItemBoxGridEntry* entry = &this->entries[offset];
    void* obj = CItem_kindPtr(this->field_2802, entry->id);
    if (!obj || !*(u32*)obj) goto fail;

    // Category dispatch: the equipment scan is shared by cats 2 and 4-8;
    // cats 9 and default share the plain-fail value.
    switch (this->field_2802) {
    case 2:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        // Equipment categories: count slots; if none are visible report the
        // cell state flag, otherwise scan for empty-but-equipped slots.
        {
            u8 n = (u8)CItem_initItemImplInstances((CItemData*)obj)->vf30((CItemData*)obj);
            if (n != 0) {
                // Scan slots for ones that are free (-1) but whose equipped
                // state object reports the equipped flag.
                for (u8 j = 0; j < n; j++) {
                    if ((s16)CItem_initItemImplInstances((CItemData*)obj)->vf40((CItemData*)obj, j) == -1) {
                        void* res =
                            CItem_initItemImplInstances((CItemData*)obj)->vf2C((CItemData*)obj, j);
                        if (res && (((CItemBoxSlotState*)res)->state & 1)) return -3;
                    }
                }
                return -2;
            }
            // marked cell: -1, otherwise 0.
            return (s8)(entry->flags[0] ? -1 : 0);
        }
    case 3:
        // marked cell: -1, otherwise 0.
        return (s8)(entry->flags[0] ? -1 : 0);
    case 9:
        return 0;
    case 10:
    case 11:
    case 12:
    case 13:
        return (s8)((CItemBoxItem*)obj)->mTier;
    default:
        goto fail;
    }
fail:
    return 0;
}

// Lookup entry, check category; return flag byte 2 (equip kind) or 0.
u8 CItemBoxGridFull::GetCellFlag0(u16 idx) {
    s8 base = (s8)this->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset < this->field_2800) {
        CItemBoxGridEntry* entry = &this->entries[offset];
        u8* obj = (u8*)CItem_kindPtr(this->field_2802, entry->id);
        if (obj && *(u32*)obj) {
            return entry->flags[0];
        }
    }
    return 0;
}

// Lookup entry, check category; return flag byte 4 or 0.
u8 CItemBoxGridFull::GetCellFlag2(u16 idx) {
    s8 base = (s8)this->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset < this->field_2800) {
        CItemBoxGridEntry* entry = &this->entries[offset];
        u8* obj = (u8*)CItem_kindPtr(this->field_2802, entry->id);
        if (obj && *(u32*)obj) {
            return entry->flags[2];
        }
    }
    return 0;
}

// Lookup entry, check category; return flag byte 5 or 0.
u8 CItemBoxGridFull::GetCellFlag3(u16 idx) {
    s8 base = (s8)this->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset < this->field_2800) {
        CItemBoxGridEntry* entry = &this->entries[offset];
        u8* obj = (u8*)CItem_kindPtr(this->field_2802, entry->id);
        if (obj && *(u32*)obj) {
            return entry->flags[3];
        }
    }
    return 0;
}

// Lookup entry, check category; return flag byte 3 or 0.
int CItemBoxGridFull::GetCellFlag1(u16 idx) {
    s8 base = (s8)this->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset < this->field_2800) {
        CItemBoxGridEntry* entry = &this->entries[offset];
        u8* obj = (u8*)CItem_kindPtr(this->field_2802, entry->id);
        if (obj && *(u32*)obj) {
            return entry->flags[1];
        }
    }
    return 0;
}

// Lookup a byte from a 10-byte-entry table indexed by (field_2804 * 0x1e + idx).
// Returns byte at offset 7 within the entry, or 0 if out of bounds.
u8 CItemBoxGridFull::GetCellByte7(u16 idx) {
    s32 tmp = (s8)this->field_2804 * 0x1e + idx;
    u32 offset = (u32)(u16)tmp;
    if (offset < 0x400) {
        return this->entries[offset].flags[5];
    }
    return 0;
}

bool CItemBoxGridFull::GetCellByte8(u16 idx) {
    s32 tmp = (s8)this->field_2804 * 0x1e + idx;
    u32 offset = (u32)(u16)tmp;
    if (offset < 0x400) {
        return this->entries[offset].mMarked;
    }
    return 0;
}


// Toggle an entry's flag based on category cap.
void CItemBoxGridFull::ToggleCellMark(u16 idx) {
    u16 offset = (u16)((s8)this->field_2804 * 0x1e + idx);
    if (offset >= 0x400) return;
    u8 cap;
    CItemBoxGridEntry* entry = &this->entries[offset];
    if (entry->mMarked == 0) {
        cap = (u8)((CItemBoxGridFull*)(this))->GetCategoryCap();
        if (((CItemBoxGridFull*)(this))->CountMarkedCells() >= cap) return;
    }
    entry->mMarked ^= 1;
}

// Count entries with non-zero byte at offset 8 in a 10-byte stride array.
u8 CItemBoxGridFull::CountMarkedCells() {
    u16 count = this->field_2800;
    u16 result = 0;
    u16 i;
    for (i = 0; i < count; i++) {
        CItemBoxGridEntry* entry = &this->entries[i];
        if (entry->mMarked != 0) {
            result++;
        }
    }
    return (u8)result;
}

// Return a duration/stride value based on the category byte at offset 0x2802.
u8 CItemBoxGridFull::GetCategoryCap() {
    int cat = this->field_2802;
    switch (cat) {
    case 2:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        return 0x1e;
    case 11:
        return 0x3c;
    default:
        return 0;
    }
}

int CItemBoxGridFull::LookupIndexedByte() {
    char* obj = (char*)this;
    char off = ((CibgGridTail*)obj)->page2804;
    if (off >= 0x400) return 0;
    return ((CibgGridTail*)obj)->win28A5[off];
}

// Iterate entries and init item instances.
void CItemBoxGridFull::InitGridItems() {
    u16 i;
    for (i = 0; i < this->field_2800; i++) {
        CItemBoxGridEntry* entry = &this->entries[i];
        if (entry->mMarked != 0) continue;
        void* obj = CItem_kindPtr(this->field_2802, entry->id);
        if (!obj || !*(u32*)obj) continue;
        CItemImpl* inst = CItem_initItemImplInstances((CItemData*)obj);
        inst->vf10((CItemData*)obj);
    }
}

// Format the item name of the cell at page*0x1e+idx into the buffer at
// +0x2805 and return it (0 when the cell is empty/invalid). -O4,s
// merges the r29-r31 saves into retail's stmw prologue; the shared fail
// block reproduces retail's single return-0 fall-through into the epilogue.
u32 CItemBoxGridFull::FormatCellName(u32 idx) {
    u8* p = (u8*)this;
    s8 base = (s8)((CItemBoxGridFull*)p)->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset >= ((CItemBoxGridFull*)p)->field_2800) goto fail;

    u8 cat = ((CItemBoxGridFull*)p)->field_2802;
    s16 val = ((CItemBoxGridFull*)p)->entries[offset].id;
    void* obj = CItem_kindPtr(cat, val);
    if (!obj || !*(u32*)obj) goto fail;

    CItemImpl* inst = CItem_initItemImplInstances((CItemData*)obj);
    const char* name = inst->vf20((CItemData*)obj);

    sprintf((char*)field_2805, (const char*)&lbl_eu_8050566C[0x14c], name);

    if (((CItemBoxGridFull*)p)->field_2802 == 3) {
        CItemImpl* inst2 = CItem_initItemImplInstances((CItemData*)obj);
        u32 count = inst2->vf08((CItemData*)obj);
        u8 b = (u8)count;
        u32 r = (u32)BdatTouchStringCell((const char*)&lbl_eu_8050566C[0x14f], &lbl_eu_8050566C[0x158], (u32)(0x1e - (b - 1)));
        sprintf((char*)field_2805, (const char*)&lbl_eu_8050566C[0x15d], (char*)field_2805, r);
    }

    return (u32)(p + 0x2805);
fail:
    return 0;
}

// Format the name of the item at page*0x1e+idx into the buffer at +0x2825
// and return it (0 when the cell is empty/invalid). The category-3 path
// post-processes the name string in place: `$1`/`$2` tags are replaced with
// per-language slot text (a 7-word table from itemBoxGridLanguageSlotWords is copied to
// the stack and indexed by the item's vtable+0x08 byte).
char* CItemBoxGridFull::FormatCellItemName(u16 idx) {
    // Stack slots follow retail: word table @0x08, tag buffers @0x28/0x48/0x68.
    u32 wordTbl[7];
    char buf28[32];
    char buf48[32];
    char buf68[32];
    u8* p = (u8*)this;
    s8 base = (s8)p[0x2804];
    u16 offset = (u16)(base * 0x1e + idx);
    void* item;

    if (offset >= ((CItemBoxGridFull*)p)->field_2800) goto fail;

    item = CItem_kindPtr(p[0x2802], *(s16*)(p + offset * 0xa));
    if (!item) goto fail;
    if (!*(u32*)item) goto fail;

    if (IsSkillItem(item)) {
        CItemImpl* inst = CItem_initItemImplInstances((CItemData*)item);
        u32 v = inst->vf80((CItemData*)item);
        int sel = 0x9d;
        if ((u16)v != 0) sel = 0x9e;
        char* msg = BdatTouchStringCell((const char*)"MNU_item", (const char*)"name", sel);
        sprintf((char*)field_2825, (const char*)"%s", msg);
    } else if (((*(u32*)item >> 12) & 0xF) == 9 && ((((u8*)item)[7] & 3) == 3)) {
        char* msg = BdatTouchStringCell((const char*)"MNU_item", (const char*)"name", 0x9c);
        sprintf((char*)field_2825, (const char*)"%s", msg);
    } else if (p[0x2802] == 3) {
        u32 tbl= (u32)::cf::CfBdat::spBtlSkillListFileData;
        CItemImpl* inst1 = CItem_initItemImplInstances((CItemData*)item);
        u16 slotStatus = CItem_initItemImplInstances((CItemData*)item)->vf54((CItemData*)item);
        CItemImpl* inst2 = CItem_initItemImplInstances((CItemData*)item);
        u8 slotByte = (u8)inst2->vf08((CItemData*)item);
        CItemImpl* inst3 = CItem_initItemImplInstances((CItemData*)item);
        char* name90 = (char*)inst3->vf90((CItemData*)item);
        char buf48[32];
        sprintf(buf48, (const char*)"%d", name90);
        u32 nameKey = BdatGetU8Direct((const void*)tbl, (const char*)"rvs_type", (u16)slotStatus);
        char* desc = BdatGetPtrDirect((const void*)tbl, "rvs_caption", (u16)slotStatus);
        sprintf((char*)field_2825, (const char*)"%s", desc);

        int langFlag = 1;
        if (CDeviceSC::getLanguage() != 3) {
            if (CDeviceSC::getLanguage() != 2) langFlag = 0;
        }

        // Retail keeps the slot-byte word offset and both pool bases in
        // non-volatile registers across the whole scan loop.
        {
        u32 langIdx = slotByte << 2;
        char* cur = (char*)field_2825;
        while (*cur != '\0') {
            s8 ch = *cur;
            if (ch == '$') {
                u32 len;
                char* q;
                memset(buf28, 0, 0x20);
                ch = *++cur;
                if (ch == '1') {
                    cur++;
                    switch (nameKey) {
                    case 1:
                        sprintf(buf28, (const char*)"<col=red>%s<col=def2>", buf48);
                        break;
                    case 2:
                        sprintf(buf28, (const char*)"<col=red>%s<col=def2>", buf48);
                        break;
                    case 3:
                        if (langFlag != 0) sprintf(buf28, (const char*)"<col=red>%s %%<col=def2>", buf48);
                        else sprintf(buf28, (const char*)"<col=red>%s%%<col=def2>", buf48);
                        break;
                    case 4:
                        if (langFlag != 0) sprintf(buf28, (const char*)"<col=red>%s %%<col=def2>", buf48);
                        else sprintf(buf28, (const char*)"<col=red>%s%%<col=def2>", buf48);
                        break;
                    }
                } else if (ch == '2') {
                    u32 key;
                    u32 nKey;
                    cur++;
                    // Retail keeps the 7-word language table copy in the '$2'
                    // arm (lowered to a 3x8-byte loop plus tail word), indexed
                    // by the item's slot byte.
                    u32 ti;
                    for (ti = 0; ti < 7; ti++) wordTbl[ti] = ((const u32*)itemBoxGridLanguageSlotWords)[ti];
                    key = *(u32*)((char*)wordTbl + langIdx);
                    nKey = BdatGetU8Direct((const void*)tbl, (const char*)key, (u16)slotStatus);
                    if (langFlag != 0) sprintf(buf28, (const char*)"<col=red>%d %%<col=def2>", (u8)nKey);
                    else sprintf(buf28, (const char*)"<col=red>%d%%<col=def2>", (u8)nKey);
                } else {
                    goto postTag;  // unknown tag char: splice the empty buffer
                }
                // Splice buf28 over the '$n' tag: save the remainder, shift in
                // the replacement, then continue just past the inserted text.
                len = 0;
                q = buf28;
                do {
                    q++;
                    len++;
                } while (*(q - 1) != '\0');
                sprintf(buf68, (const char*)"%s", cur);
                cur -= 2;
                sprintf(cur, (const char*)"%s", buf28);
                cur += len - 1;
                sprintf(cur, (const char*)"%s", buf68);
            postTag:;
            } else if ((ch >= 0x81 && ch <= 0x9f) || (ch >= (s8)0xe0 && ch <= (s8)0xef)) {
                cur += 2;
            } else {
                cur += 1;
            }
        }
        }
    } else {
        // itemBoxGrid_shopItemTypeTable/0C are single pointer variables (not arrays):
        // retail loads 0C first, then overrides with 08 when the category is
        // in range or exactly 2.
        u32 table = itemBoxGrid_shopSkillTable;
        if ((((u32)p[0x2802] + 0xFC) & 0xFF) <= 5 || p[0x2802] == 2) table = itemBoxGrid_shopItemTypeTable;
        u16 v = BdatGetU16Direct((const void*)::cf::CfBdat::spItmItemListFileData, "comment",
                              (*(u32*)item) >> 20);
        char* s = BdatGetPtrDirect((const void*)table, "comment", v);
        sprintf((char*)field_2825, (const char*)"%s", s);
    }
    return (char*)field_2825;
fail:
    return NULL;
}

// Check if an object has type 9 (extracted from vtable bits) and subtype 2.
// noinline: retail callers emit `bl` (the unit builds with -inline and would
// otherwise fold this body into every call site).
extern "C" int IsSkillItem(void* obj) {
    u32 w = *(u32*)obj;
    u32 type = (w >> 16) & 0xF;
    int result = 0;
    if (type == 9) {
        u8 sub = ((CItemBoxItem*)obj)->mState & 3;
        if (sub == 2) {
            result = 1;
        }
    }
    return result;
}

// Forward declarations for sort helpers defined later in this file.
// (memberized: SortByItemU16 -> CItemBoxGridFull::SortByItemU16; declaration moved into class CItemBoxGridFull)
// (memberized: SortByPriceDesc -> CItemBoxGridFull::SortByPriceDesc; declaration moved into class CItemBoxGridFull)
// (memberized: SortByKindNameD -> CItemBoxGridFull::SortByKindNameD; declaration moved into class CItemBoxGridFull)
// (memberized: SortByItemByte6 -> CItemBoxGridFull::SortByItemByte6; declaration moved into class CItemBoxGridFull)
// (memberized: SortByCellFlags -> CItemBoxGridFull::SortByCellFlags; declaration moved into class CItemBoxGridFull)
// (memberized: SortByFlag6Desc -> CItemBoxGridFull::SortByFlag6Desc; declaration moved into class CItemBoxGridFull)
// (memberized: SortByFlag6Asc -> CItemBoxGridFull::SortByFlag6Asc; declaration moved into class CItemBoxGridFull)
// (memberized: SortByItemVf08 -> CItemBoxGridFull::SortByItemVf08; declaration moved into class CItemBoxGridFull)
// (memberized: SortByItemVf30 -> CItemBoxGridFull::SortByItemVf30; declaration moved into class CItemBoxGridFull)
// (memberized: SortByEquipAsc -> CItemBoxGridFull::SortByEquipAsc; declaration moved into class CItemBoxGridFull)
// (memberized: SortByEquipDesc -> CItemBoxGridFull::SortByEquipDesc; declaration moved into class CItemBoxGridFull)
// (memberized: SortByCapKeyAsc -> CItemBoxGridFull::SortByCapKeyAsc; declaration moved into class CItemBoxGridFull)
// (memberized: SortByCapKey2Asc -> CItemBoxGridFull::SortByCapKey2Asc; declaration moved into class CItemBoxGridFull)
// (memberized: SortByCapKeyDesc -> CItemBoxGridFull::SortByCapKeyDesc; declaration moved into class CItemBoxGridFull)
// (memberized: SortByItemVf90 -> CItemBoxGridFull::SortByItemVf90; declaration moved into class CItemBoxGridFull)

// Re-sort the grid after its contents changed: dispatch on the current
// category (sorting first with SortGridEntries and masking the per-category
// CItemBlock_getFlag120EC result into an 8-bit value v), then apply the category's
// inner sort sequence. Categories 8-13 do nothing (default). -O4,s
// merges the r30/r31 saves into retail's stmw prologue. The empty cases
// 8-13 must be written with an explicit `return;` body: grouped `break;`
// empties (with or without default) collapse and MWCC emits only an 8-slot
// table (`cmpli 7`); the return-form placeholder extends it to retail's
// 14-entry `cmplwi 0xd` table. Remaining residuals: (a) MWCC colors the
// outer switch operand r0 where retail keeps r4 (a named u8 local recolors
// it but also reorders the SortGridEntries/CItemBlock_getFlag120EC call scheduling --
// worse); (b) ~40-byte block-order divergence starting at the case-2 jump
// table (decomp places case-1 subcase-8/9 body where retail puts the
// jumptable_eu_80534598 setup).
void CItemBoxGridFull::ResortGridEntries(u16 idx) {
    ((CItemBoxGridFull*)(this))->SortGridEntries(this->field_2802);
    u8 v = (u8)CItemBlock_getFlag120EC(this->field_2802);
    switch (this->field_2802) {
    case 0:
        this->SortByItemU16();
        switch (v) {
        case 1:
            this->SortByEquipAsc();
            this->SortByItemVf30();
            ((CItemBoxGridFull*)(this))->SortGridByValue();
            ((CItemBoxGridFull*)(this))->SortGridBySlotStatus();
            break;
        case 2:
            this->SortByEquipAsc();
            this->SortByItemVf30();
            ((CItemBoxGridFull*)(this))->SortGridByValue();
            ((CItemBoxGridFull*)(this))->SortGridByKindName();
            ((CItemBoxGridFull*)(this))->SortGridBySlotStatus();
            break;
        case 3:
            this->SortByEquipAsc();
            this->SortByItemVf30();
            ((CItemBoxGridFull*)(this))->SortGridByValue();
            ((CItemBoxGridFull*)(this))->SortGridByKindNameAlt();
            ((CItemBoxGridFull*)(this))->SortGridBySlotStatus();
            break;
        case 4:
            ((CItemBoxGridFull*)(this))->SortGridByValue();
            this->SortByItemVf30();
            this->SortByEquipDesc();
            ((CItemBoxGridFull*)(this))->SortGridBySlotStatus();
            break;
        case 5:
            ((CItemBoxGridFull*)(this))->SortGridByValue();
            this->SortByItemVf30();
            this->SortByEquipAsc();
            ((CItemBoxGridFull*)(this))->SortGridBySlotStatus();
            break;
        case 6:
            this->SortByPriceDesc();
            break;
        }
        break;
    case 1:
        this->SortByItemU16();
        switch (v) {
        case 0:
            ((CItemBoxGridFull*)(this))->GroupByCapKey(1);
            ((CItemBoxGridFull*)(this))->GroupByCapKey(2);
            ((CItemBoxGridFull*)(this))->GroupByCapKey(3);
            this->SortByEquipAsc();
            this->SortByItemVf30();
            this->SortByCapKeyAsc();
            break;
        case 1:
            ((CItemBoxGridFull*)(this))->GroupByCapKey(1);
            ((CItemBoxGridFull*)(this))->GroupByCapKey(2);
            ((CItemBoxGridFull*)(this))->GroupByCapKey(3);
            this->SortByEquipAsc();
            this->SortByItemVf30();
            this->SortByCapKey2Asc();
            break;
        case 2:
            this->SortByEquipAsc();
            this->SortByItemVf30();
            this->SortByCapKeyAsc();
            ((CItemBoxGridFull*)(this))->GroupByCapKey(3);
            ((CItemBoxGridFull*)(this))->GroupByCapKey(2);
            ((CItemBoxGridFull*)(this))->GroupByCapKey(1);
            break;
        case 3:
            this->SortByEquipAsc();
            this->SortByItemVf30();
            this->SortByCapKeyAsc();
            ((CItemBoxGridFull*)(this))->GroupByCapKey(3);
            ((CItemBoxGridFull*)(this))->GroupByCapKey(1);
            ((CItemBoxGridFull*)(this))->GroupByCapKey(2);
            break;
        case 4:
            this->SortByEquipAsc();
            this->SortByItemVf30();
            this->SortByCapKeyAsc();
            ((CItemBoxGridFull*)(this))->GroupByCapKey(2);
            ((CItemBoxGridFull*)(this))->GroupByCapKey(1);
            ((CItemBoxGridFull*)(this))->GroupByCapKey(3);
            break;
        case 5:
            ((CItemBoxGridFull*)(this))->GroupByCapKey(1);
            ((CItemBoxGridFull*)(this))->GroupByCapKey(2);
            ((CItemBoxGridFull*)(this))->GroupByCapKey(3);
            this->SortByCapKeyAsc();
            this->SortByEquipAsc();
            this->SortByItemVf30();
            this->SortByCapKeyDesc();
            break;
        case 6:
            ((CItemBoxGridFull*)(this))->GroupByCapKey(1);
            ((CItemBoxGridFull*)(this))->GroupByCapKey(2);
            ((CItemBoxGridFull*)(this))->GroupByCapKey(3);
            this->SortByCapKeyAsc();
            this->SortByItemVf30();
            this->SortByEquipDesc();
            break;
        case 7:
            ((CItemBoxGridFull*)(this))->GroupByCapKey(1);
            ((CItemBoxGridFull*)(this))->GroupByCapKey(2);
            ((CItemBoxGridFull*)(this))->GroupByCapKey(3);
            this->SortByCapKeyAsc();
            this->SortByItemVf30();
            this->SortByEquipAsc();
            break;
        case 8:
            this->SortByPriceDesc();
            break;
        case 9:
            this->SortByPriceDesc();
            break;
        }
        break;
    case 2:
        this->SortByItemU16();
        switch (v) {
        case 0:
            this->SortByItemU16();
            break;
        case 1:
            this->SortByItemVf90();
            ((CItemBoxGridFull*)(this))->SortGridEntries(this->field_2802);
            this->SortByItemVf08();
            break;
        case 2:
            ((CItemBoxGridFull*)(this))->SortGridEntries(this->field_2802);
            this->SortByItemVf90();
            this->SortByItemVf08();
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(2);
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(0);
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(1);
            break;
        case 3:
            ((CItemBoxGridFull*)(this))->SortGridEntries(this->field_2802);
            this->SortByItemVf90();
            this->SortByItemVf08();
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(1);
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(0);
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(2);
            break;
        case 4:
            ((CItemBoxGridFull*)(this))->SortGridEntries(this->field_2802);
            this->SortByItemVf90();
            this->SortByItemVf08();
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(2);
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(1);
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(0);
            break;
        case 5:
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(2);
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(1);
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(0);
            this->SortByItemVf90();
            this->SortByItemVf08();
            ((CItemBoxGridFull*)(this))->SortGridEntries(this->field_2802);
            ((CItemBoxGridFull*)(this))->GroupBySlotStat(1);
            break;
        case 6:
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(2);
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(1);
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(0);
            this->SortByItemVf90();
            this->SortByItemVf08();
            ((CItemBoxGridFull*)(this))->SortGridEntries(this->field_2802);
            ((CItemBoxGridFull*)(this))->GroupBySlotStat(2);
            break;
        case 7:
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(2);
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(1);
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(0);
            this->SortByItemVf90();
            this->SortByItemVf08();
            ((CItemBoxGridFull*)(this))->SortGridEntries(this->field_2802);
            ((CItemBoxGridFull*)(this))->GroupBySlotStat(3);
            break;
        case 8:
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(2);
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(1);
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(0);
            this->SortByItemVf90();
            this->SortByItemVf08();
            ((CItemBoxGridFull*)(this))->SortGridEntries(this->field_2802);
            ((CItemBoxGridFull*)(this))->GroupBySlotStat(4);
            break;
        case 9:
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(2);
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(1);
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(0);
            this->SortByItemVf90();
            this->SortByItemVf08();
            ((CItemBoxGridFull*)(this))->SortGridEntries(this->field_2802);
            ((CItemBoxGridFull*)(this))->GroupBySlotStat(5);
            break;
        case 10:
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(2);
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(1);
            ((CItemBoxGridFull*)(this))->GroupBySlotAlt(0);
            this->SortByItemVf90();
            this->SortByItemVf08();
            ((CItemBoxGridFull*)(this))->SortGridEntries(this->field_2802);
            ((CItemBoxGridFull*)(this))->GroupBySlotStat(6);
            break;
        case 11:
            this->SortByPriceDesc();
            break;
        }
        break;
    case 3:
        this->SortByItemU16();
        switch (v) {
        case 0:
            this->SortByItemU16();
            break;
        case 1:
            this->SortByItemVf08();
            break;
        case 2:
            this->SortByItemVf08();
            ((CItemBoxGridFull*)(this))->SortGridEntries(this->field_2802);
            break;
        case 3:
            this->SortByItemU16();
            this->SortByPriceDesc();
            break;
        }
        break;
    case 4:
        this->SortByItemU16();
        switch (v) {
        case 1:
            this->SortByKindNameD();
            break;
        case 2:
            this->SortByItemByte6();
            break;
        case 3:
            this->SortByCellFlags();
            break;
        case 4:
            this->SortByPriceDesc();
            break;
        }
        break;
    case 5:
        this->SortByItemU16();
        switch (v) {
        case 1:
            this->SortByItemByte6();
            break;
        case 2:
            this->SortByCellFlags();
            break;
        case 3:
            this->SortByPriceDesc();
            break;
        }
        break;
    case 6:
        if (v == 0) {
            this->SortByItemU16();
        }
        break;
    case 7:
        switch (v) {
        case 0:
            this->SortByItemU16();
            break;
        case 1:
            ((CItemBoxGridFull*)(this))->GroupByKindName(3);
            ((CItemBoxGridFull*)(this))->GroupByKindName(2);
            ((CItemBoxGridFull*)(this))->GroupByKindName(1);
            ((CItemBoxGridFull*)(this))->SortGridByMsgKeyDesc();
            this->SortByFlag6Desc();
            break;
        case 2:
            ((CItemBoxGridFull*)(this))->GroupByKindName(3);
            ((CItemBoxGridFull*)(this))->GroupByKindName(2);
            ((CItemBoxGridFull*)(this))->GroupByKindName(1);
            ((CItemBoxGridFull*)(this))->SortGridByMsgKeyDesc();
            this->SortByFlag6Asc();
            break;
        case 3:
            this->SortByItemU16();
            this->SortByPriceDesc();
            break;
        }
        break;
    // Explicit `return;` bodies force MWCC to emit the full 14-entry jump
    // table with retail's cmplwi 0xd bound (empty break-cases collapse).
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        return;
    }
}

// Clear inactive entries (flag byte 8 set) and compact the grid: bubble
// passes swap every still-empty (-1 id) cell toward the front. Afterwards,
// when the entry count changed, recompute the row count (entries per row /
// rounded up) and reset the page index.
void CItemBoxGridFull::CompactGridEntries() {
    u8* p = (u8*)this;
    u16 count = this->field_2800;
    u16 i;
    for (i = 0; i < count; i++) {
        u8* e = p + i * 10;
        if (e[8]) {
            CopyGridEntry9(e, (void*)&(const CItemBoxGridEntry&)CItemBoxGridEntry(-1, 0, 0, 0, 0, 0, 0, 0));
            this->field_2800--;
        }
    }
    u16 pass = 0;
    do {
        u32 stay = 0x3FF - pass;
        u32 swapped = 0;
        u16 j;
        for (j = 0; j < stay; j++) {
            u8* e1 = p + j * 10;
            u8* e2 = p + (j + 1) * 10;
            s16 v1 = *(s16*)e1;
            s16 v2 = *(s16*)e2;
            // Only an empty (-1) cell followed by a non-empty one swaps.
            if ((v1 + 1 == 0) >= (v2 + 1 == 0)) continue;
            char t1[9];
            char t2[9];
            char t3[9];
            CopyEntry9Bytes(t1, (const char*)e1);
            CopyGridEntry9(e1, CopyEntry9Bytes(t2, (const char*)e2));
            CopyGridEntry9(e2, CopyEntry9Bytes(t3, (const char*)t1));
            swapped = 1;
        }
        if (swapped == 0) break;
        pass++;
    } while (pass < 0x3FF);
    if (count != this->field_2800) {
        // Row count = entries/row rounded up; page index resets.
        u32 cnt = this->field_2800;
        float fdiv = (float)cnt / 30.0f;
        int rows = (int)fdiv;
        float frac = fdiv - (float)rows;
        if (0.0f != frac) rows++;
        if (!(u8)rows) rows = 1;
        this->field_2803 = (u8)rows;
        this->field_2804 = 0;
    }
}

// 9-byte copy used by the sort routines' temp shuffle. noinline keeps the
// call sites' `bl` (retail calls it; -inline would fold the body in).
// Returns dst: retail sorters reuse the caller-preserved r3 (the CopyEntry9Bytes
// result) for the following CopyGridEntry9 src argument instead of recomputing.
char* CopyEntry9Bytes(char* dst, const char* src) {
    *(short*)((char*)dst + 0) = *(short*)((char*)src + 0);
    *(char*)((char*)dst + 2) = *(char*)((char*)src + 2);
    *(char*)((char*)dst + 3) = *(char*)((char*)src + 3);
    *(char*)((char*)dst + 4) = *(char*)((char*)src + 4);
    *(char*)((char*)dst + 5) = *(char*)((char*)src + 5);
    *(char*)((char*)dst + 6) = *(char*)((char*)src + 6);
    *(char*)((char*)dst + 7) = *(char*)((char*)src + 7);
    *(char*)((char*)dst + 8) = *(char*)((char*)src + 8);
    return dst;
}

// Base price of an item, scaled by its kind type:
// - type 3: price row keyed by the +0x54 slot value, scaled by the
//   itemBoxGridPriceScaleRow row indexed by the +0x08 slot value.
// - type 9: per-slot price rows are accumulated over up to 4 slots (each
//   scaled by a fresh copy of the table row), then the sum is scaled by
//   0x67F38; the common tail applies an extra x10.
// - types 2..8: price row scaled by 0x67F78 + 0x67F7C per slot count.
// - anything else: plain price row lookup.
u32 CItemBoxGridFull::GetItemBasePrice(void* item) {
    u32 word = *(u32*)item;
    u32 kind = word >> 20;
    u32 type = (word >> 16) & 0xF;
    u16 shortKind = BdatGetItemId(kind);
    u32 result = 0;

    if (type == 3) {
        u16 key = CItem_initItemImplInstances((CItemData*)item)->vf54((CItemData*)item);
        u16 slot = (u16)CItem_initItemImplInstances((CItemData*)item)->vf08((CItemData*)item);
        u16 lookup = BdatGetU16Direct((void*)::cf::CfBdat::spBtlSkillListFileData, "money", (u16)key);
        CItemPriceScaleRow row;
        row = *(const CItemPriceScaleRow*)&itemBoxGridPriceScaleRow;
        result = (u32)((float)lookup * row.v[slot & 0x3F]);
    } else if (type == 9) {
        u16 slot = (u16)CItem_initItemImplInstances((CItemData*)item)->vf08((CItemData*)item);
        CItemPriceScaleRow rowA;
        rowA = *(const CItemPriceScaleRow*)&itemBoxGridPriceScaleRow;
        u32 i;
        for (i = 0; i < 4; i++) {
            u16 v = (u16)CItem_initItemImplInstances((CItemData*)item)->vf4C((CItemData*)item, (u8)i);
            if (v == 0) continue;
            u16 lookup = BdatGetU16Direct((void*)::cf::CfBdat::spBtlSkillListFileData, "money", v);
            // Retail re-copies the row inside the loop before each scale.
            CItemPriceScaleRow rowB;
            rowB = rowA;
            result = (u32)(0.25f * ((float)lookup * rowB.v[slot & 0x3F]) + (double)result);
        }
        result = (u32)((float)result * 0.1f);
    } else if (type >= 2 && type <= 8) {
        LookupItemTypeTable(kind);
        u16 lookup = BdatGetU16Direct((void*)::cf::CfBdat::spBtlSkillListFileData, "money", (u16)shortKind);
        u8 count = (u8)CItem_initItemImplInstances((CItemData*)item)->vf30((CItemData*)item);
        float f = 1.0f;
        while (count-- != 0) f += 0.2f;
        result = (u32)((float)lookup * f);
    } else {
        LookupItemTypeTable(kind);
        result = BdatGetU16Direct((void*)::cf::CfBdat::spBtlSkillListFileData, "money", (u16)shortKind);
    }
    return result * 10;
}

// Item value used for shop buy/sell comparisons. Kind/type split mirrors
// GetItemBasePrice: type 3 prices off the +0x54 slot row scaled by the
// itemBoxGridPriceScaleRow row indexed by the +0x08 slot (or, for bare kinds, the
// name-table lookups); type 9 values at 0; types 2-8 price off the row
// scaled by 0x67F78 + 0x67F7C per slot; everything else is the plain row.
// Result is always x10.
s32 CItemBoxGridFull::GetShopItemValue(u32 id, void* item) {
    void* obj;
    u32 type;
    u32 kind;
    u32 result = 0;
    u32 shortKind;
    obj = item ? item : 0;
    if (item) kind = *(u32*)obj >> 20;
    else kind = id;
    if (item) type = (*(u32*)obj >> 12) & 0xF;
    else type = (u16)BdatGetItemType(kind & 0xFFFF);
    shortKind = BdatGetItemId(kind & 0xFFFF);
    if ((u32)(u16)type == 3) {
        u32 key;
        u32 slot;
        if (item) {
            key = (u32)CItem_initItemImplInstances((CItemData*)obj)->vf54((CItemData*)obj);
            slot = (u16)CItem_initItemImplInstances((CItemData*)obj)->vf08((CItemData*)obj);
        } else {
            key = shortKind;
            slot = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData, "rankType", shortKind & 0xFFFF);
        }
        u16 lookup = BdatGetU16Direct((void*)::cf::CfBdat::spBtlSkillListFileData, "money", key & 0xFFFF);
        u32 scale[7];
        u32 i;
        for (i = 0; i < 7; i++) scale[i] = ((const u32*)itemBoxGridPriceScaleRow)[i];
        result = (u32)((float)lookup * ((float*)scale)[slot & 0x3F]);
    } else if ((u32)(u16)type == 9) {
        // result stays 0
    } else if ((u32)(u16)type >= 2 && (u32)(u16)type <= 8) {
        u32 tbl = LookupItemTypeTable(kind & 0xFFFF);
        u16 lookup = BdatGetU16Direct((void*)::cf::CfBdat::spBtlSkillListFileData, "money", shortKind & 0xFFFF);
        u32 count;
        if (item) count = (u16)CItem_initItemImplInstances((CItemData*)obj)->vf30((CItemData*)obj);
        else count = (u8)BdatGetU8Direct((const void*)tbl, "jwl_slot", shortKind & 0xFFFF);
        float f = 1.0f;
        while (count-- != 0) f += 0.2f;
        result = (u32)((float)lookup * f);
    } else {
        LookupItemTypeTable(kind & 0xFFFF);
        result = BdatGetU16Direct((void*)::cf::CfBdat::spBtlSkillListFileData, "money", shortKind & 0xFFFF);
    }
    return result * 10;
}

// Bubble-sort grid entries ascending under the selected comparison mode.
// A pass that performs no swap ends the sort early (i only advances on swap).
void CItemBoxGridFull::SortGridEntries(u32 mode) {
    u32 i;
    for (i = 0; (u16)i < this->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        for (j = 0; (u16)j < this->field_2800 - 1 - (u16)i; j++) {
            void* obj1 = CItem_kindPtr(this->field_2802, this->entries[j].id);
            void* obj2 = CItem_kindPtr(this->field_2802, this->entries[j + 1].id);
            CItemBoxGridEntry* e1 = &this->entries[j];
            CItemBoxGridEntry* e2 = &this->entries[j + 1];
            if (mode == 3) {
                // Compare (u16) inst->vf54(item) keys; obj2's chain runs first.
                u16 key2 = (u16)CItem_initItemImplInstances((CItemData*)obj2)->vf54((CItemData*)obj2);
                u16 key1 = (u16)CItem_initItemImplInstances((CItemData*)obj1)->vf54((CItemData*)obj1);
                if (key1 > key2) {
                    char tmp1[9];
                    char tmp2[9];
                    char tmp3[9];
                    CopyEntry9Bytes(tmp1, (const char*)e1);
                    CopyGridEntry9(e1, CopyEntry9Bytes(tmp2, (const char*)e2));
                    CopyGridEntry9(e2, CopyEntry9Bytes(tmp3, (const char*)tmp1));
                    swapped = 1;
                }
            } else if (mode == 9) {
                int a = IsSkillItem(obj1);
                int b = IsSkillItem(obj2);
                // A non-zero vf80 result zeroes that side's key.
                if (CItem_initItemImplInstances((CItemData*)obj1)->vf80((CItemData*)obj1) != 0) a = 0;
                if (CItem_initItemImplInstances((CItemData*)obj2)->vf80((CItemData*)obj2) != 0) b = 0;
                if (a != 0 || b != 0) {
                    if (a > b) {
                        char tmp1[9];
                        char tmp2[9];
                        char tmp3[9];
                        CopyEntry9Bytes(tmp1, (const char*)e1);
                        CopyGridEntry9(e1, CopyEntry9Bytes(tmp2, (const char*)e2));
                        CopyGridEntry9(e2, CopyEntry9Bytes(tmp3, (const char*)tmp1));
                        swapped = 1;
                    }
                } else {
                    // Both keys zeroed: tie-break on flag-byte-7 bits 2..7.
                    CItemBoxItem* v1 = (CItemBoxItem*)obj1;
                    CItemBoxItem* v2 = (CItemBoxItem*)obj2;
                    if ((v1->mState >> 2 & 0x3f) > (v2->mState >> 2 & 0x3f)) {
                        char tmp1[9];
                        char tmp2[9];
                        char tmp3[9];
                        CopyEntry9Bytes(tmp1, (const char*)e1);
                        CopyGridEntry9(e1, CopyEntry9Bytes(tmp2, (const char*)e2));
                        CopyGridEntry9(e2, CopyEntry9Bytes(tmp3, (const char*)tmp1));
                        swapped = 1;
                    }
                }
            } else {
                CItemBoxItem* v1 = (CItemBoxItem*)obj1;
                CItemBoxItem* v2 = (CItemBoxItem*)obj2;
                u32 key1 = (u16)BdatGetItemId(v1->mWord >> 20);
                u32 key2 = (u16)BdatGetItemId(v2->mWord >> 20);
                if (key1 > key2) {
                    char tmp1[9];
                    char tmp2[9];
                    char tmp3[9];
                    CopyEntry9Bytes(tmp1, (const char*)e1);
                    CopyGridEntry9(e1, CopyEntry9Bytes(tmp2, (const char*)e2));
                    CopyGridEntry9(e2, CopyEntry9Bytes(tmp3, (const char*)tmp1));
                    swapped = 1;
                }
            }
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries ascending by the item object's +4 value. The two
// item lookups (CItem_kindPtr) gate the swap; the 3-slot temp shuffle keeps
// the retail bl sequence (CopyEntry9Bytes x3 / CopyGridEntry9 x2).
void CItemBoxGridFull::SortByItemU16() {
    u16 i;
    for (i = 0; i < this->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < this->field_2800 - 1 - i; j++) {
            CItemBoxGridEntry* e1 = &this->entries[j];
            CItemBoxGridEntry* e2 = &this->entries[j + 1];
            void* obj1 = CItem_kindPtr(this->field_2802, e1->id);
            void* obj2 = CItem_kindPtr(this->field_2802, e2->id);
            if (((CItemBoxItem*)obj1)->mFlags <= ((CItemBoxItem*)obj2)->mFlags) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries by item price (GetItemBasePrice). The value calls
// evaluate item j+1 before item j (locals force the call order), matching
// retail's sequence; the 3-slot temp shuffle is the same shape as
// SortByItemU16. Swap when the first item's price is strictly lower.
void CItemBoxGridFull::SortByPriceDesc() {
    u16 i;
    for (i = 0; i < this->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < this->field_2800 - 1 - i; j++) {
            CItemBoxGridEntry* e1 = &this->entries[j];
            CItemBoxGridEntry* e2 = &this->entries[j + 1];
            void* obj1 = CItem_kindPtr(this->field_2802, e1->id);
            void* obj2 = CItem_kindPtr(this->field_2802, e2->id);
            u32 v2 = this->GetItemBasePrice(obj2);
            u32 v1 = this->GetItemBasePrice(obj1);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries by kind-name sort key: each item's kind
// (word>>20) is mapped through BdatGetItemId to a short id, which is
// looked up in the message table (::cf::CfBdat::spItmCollectListFileData) with the "name" key
// string; swap when the byte keys differ (descending). Same 3-slot temp
// shuffle shape as SortByItemU16.
void CItemBoxGridFull::SortByKindNameD() {
    u16 i;
    for (i = 0; i < this->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < this->field_2800 - 1 - i; j++) {
            CItemBoxGridEntry* e1 = &this->entries[j];
            CItemBoxGridEntry* e2 = &this->entries[j + 1];
            void* obj1 = CItem_kindPtr(this->field_2802, e1->id);
            void* obj2 = CItem_kindPtr(this->field_2802, e2->id);
            u32 w1 = ((CItemBoxItem*)obj1)->mWord;
            u16 k1 = BdatGetItemId(w1 >> 20);
            u32 w2 = ((CItemBoxItem*)obj2)->mWord;
            u16 k2 = BdatGetItemId(w2 >> 20);
            u8 c1 = BdatGetU8Direct((const void*)::cf::CfBdat::spItmCollectListFileData, (const char*)&lbl_eu_8050566C[0x212], k1 & 0xFFFF);
            u8 c2 = BdatGetU8Direct((const void*)::cf::CfBdat::spItmCollectListFileData, (const char*)&lbl_eu_8050566C[0x212], k2 & 0xFFFF);
            if ((u32)c1 <= (u32)c2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries descending by the item object's +6 byte (swap when
// the first is strictly smaller). Same temp-shuffle shape as SortByItemU16.
void CItemBoxGridFull::SortByItemByte6() {
    u16 i;
    for (i = 0; i < this->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < this->field_2800 - 1 - i; j++) {
            CItemBoxGridEntry* e1 = &this->entries[j];
            CItemBoxGridEntry* e2 = &this->entries[j + 1];
            void* obj1 = CItem_kindPtr(this->field_2802, e1->id);
            void* obj2 = CItem_kindPtr(this->field_2802, e2->id);
            if (((CItemBoxItem*)obj1)->mTier >= ((CItemBoxItem*)obj2)->mTier) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Two-pass bubble sort: pass 1 orders entries ascending by flag byte 5
// (flags[3]), pass 2 by flag byte 4 (flags[2]). Each pass swaps adjacent
// pairs while the earlier entry's byte is strictly smaller, stopping early
// when a pass makes no swap. The 3-slot temp shuffle (12-byte stride) keeps
// the retail CopyEntry9Bytes / CopyGridEntry9 call sequence.
void CItemBoxGridFull::SortByCellFlags() {
    u32 i;
    for (i = 0; (u16)i < this->field_2800 - 1; i++) {
        u32 i16 = (u16)i;
        int swapped = 0;
        u32 j;
        char tmp[36];
        for (j = 0; (u16)j < this->field_2800 - 1 - (s32)i16; j++) {
            CItemBoxGridEntry* e1 = &this->entries[(u16)j];
            CItemBoxGridEntry* e2 = &this->entries[(u16)j + 1];
            if ((u32)e1->flags[3] < (u32)e2->flags[3]) {
                CopyEntry9Bytes(tmp, (const char*)e1);
                CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
                CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
    for (u32 i = 0; (u16)i < this->field_2800 - 1; i++) {
        u32 i16 = (u16)i;
        int swapped = 0;
        u32 j;
        char tmp2[36];
        for (j = 0; (u16)j < this->field_2800 - 1 - (s32)i16; j++) {
            CItemBoxGridEntry* e1 = &this->entries[(u16)j];
            CItemBoxGridEntry* e2 = &this->entries[(u16)j + 1];
            if ((u32)e1->flags[2] < (u32)e2->flags[2]) {
                CopyEntry9Bytes(tmp2, (const char*)e1);
                CopyGridEntry9(e1, CopyEntry9Bytes(tmp2 + 12, (const char*)e2));
                CopyGridEntry9(e2, CopyEntry9Bytes(tmp2 + 24, (const char*)tmp2));
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

// Bubble sort entries descending by byte at offset 6. Three 12-byte-strided
// stack scratch buffers reproduce retail's sp+8/0x14/0x20 copy shuffle.
// -O4,s merges the r26-r31 saves into retail's stmw prologue.
void CItemBoxGridFull::SortByFlag6Desc() {
    /* grid base viewed as entries/field_2800 (see CItemBoxGridFull). */
    char tmp[36];
    u32 i;
    for (i = 0; (u16)i < this->field_2800 - 1; i++) {
        int swapped = 0;
        u32 j;
        for (j = 0; (u16)j < this->field_2800 - 1 - (u16)i; j++) {
            u16 jj = (u16)j;
            CItemBoxGridEntry* e1 = &this->entries[jj];
            CItemBoxGridEntry* e2 = &this->entries[jj + 1];
            if (e1->flags[4] > e2->flags[4]) {
                CopyEntry9Bytes(tmp, (const char*)e1);
                CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
                CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, tmp));
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

// Bubble sort entries ascending by byte at offset 6 (mirror of
// SortByFlag6Desc with the comparison inverted).
void CItemBoxGridFull::SortByFlag6Asc() {
    /* grid base viewed as entries/field_2800 (see CItemBoxGridFull). */
    char tmp[36];
    u32 i;
    for (i = 0; (u16)i < this->field_2800 - 1; i++) {
        int swapped = 0;
        u32 j;
        for (j = 0; (u16)j < this->field_2800 - 1 - (u16)i; j++) {
            u16 jj = (u16)j;
            CItemBoxGridEntry* e1 = &this->entries[jj];
            CItemBoxGridEntry* e2 = &this->entries[jj + 1];
            if (e1->flags[4] < e2->flags[4]) {
                CopyEntry9Bytes(tmp, (const char*)e1);
                CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
                CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, tmp));
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries ascending by the item instance's vtable+0x08
// slot value (u16). v2 (item j+1) is evaluated before v1 to match retail's
// call order; the 3-slot temp shuffle is the same shape as SortByItemU16.
void CItemBoxGridFull::SortByItemVf08() {
    void* obj2;
    u16 v2;
    u16 i;
    for (i = 0; i < this->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < this->field_2800 - 1 - i; j++) {
            CItemBoxGridEntry* e1 = &this->entries[j];
            CItemBoxGridEntry* e2 = &this->entries[j + 1];
            void* obj1 = CItem_kindPtr(this->field_2802, e1->id);
            obj2 = CItem_kindPtr(this->field_2802, e2->id);
            v2 = (u16)CItem_initItemImplInstances((CItemData*)obj2)->vf08((CItemData*)obj2);
            u16 v1;
            v1 = (u16)CItem_initItemImplInstances((CItemData*)obj1)->vf08((CItemData*)obj1);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries so items whose slot-status name lookup matches
// `target` group at the front: an adjacent pair (j, j+1) is swapped when
// entry j+1's key byte equals target but entry j's does not. Key = each
// item's vtable+0x54 slot status, looked up in the message table
// (::cf::CfBdat::spBtlSkillListFileData) with the "name" key at +0x217.
void CItemBoxGridFull::GroupBySlotStat(u32 target) {
    u32 i;
    for (i = 0; (u16)i < this->field_2800 - 1; i++) {
        int swapped = 0;
        u32 j;
        char tmp[36];
        for (j = 0; (u16)j < this->field_2800 - 1 - (u16)i; j++) {
            CItemBoxGridEntry* e1 = &this->entries[(u16)j];
            CItemBoxGridEntry* e2 = &this->entries[(u16)j + 1];
            void* obj1 = CItem_kindPtr(this->field_2802, e1->id);
            void* obj2 = CItem_kindPtr(this->field_2802, e2->id);
            u32 v1 = CItem_initItemImplInstances((CItemData*)obj1)->vf54((CItemData*)obj1);
            u32 v2 = CItem_initItemImplInstances((CItemData*)obj2)->vf54((CItemData*)obj2);
            u8 c1 = BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, (const char*)&lbl_eu_8050566C[0x217], (u16)v1);
            u8 c2 = BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, (const char*)&lbl_eu_8050566C[0x217], (u16)v2);
            u32 m1 = c1 == target;
            u32 m2 = c2 == target;
            if (m1 < m2) {
                CopyEntry9Bytes(tmp, (const char*)e1);
                CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
                CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries so items whose slot-status name lookup matches
// `target` group at the front (same shape as GroupBySlotStat, key string at
// +0x220).
void CItemBoxGridFull::GroupBySlotAlt(u32 target) {
    u32 i;
    for (i = 0; (u16)i < this->field_2800 - 1; i++) {
        int swapped = 0;
        u32 j;
        char tmp[36];
        for (j = 0; (u16)j < this->field_2800 - 1 - (u16)i; j++) {
            CItemBoxGridEntry* e1 = &this->entries[(u16)j];
            CItemBoxGridEntry* e2 = &this->entries[(u16)j + 1];
            void* obj1 = CItem_kindPtr(this->field_2802, e1->id);
            void* obj2 = CItem_kindPtr(this->field_2802, e2->id);
            u32 v1 = CItem_initItemImplInstances((CItemData*)obj1)->vf54((CItemData*)obj1);
            u32 v2 = CItem_initItemImplInstances((CItemData*)obj2)->vf54((CItemData*)obj2);
            u8 c1 = BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, (const char*)&lbl_eu_8050566C[0x220], (u16)v1);
            u8 c2 = BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, (const char*)&lbl_eu_8050566C[0x220], (u16)v2);
            u32 m1 = c1 == target;
            u32 m2 = c2 == target;
            if (m1 < m2) {
                CopyEntry9Bytes(tmp, (const char*)e1);
                CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
                CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

// Bubble sort grid entries ascending by item value (GetItemValueFloat), with
// early exit when a pass makes no swaps. The 3-slot temp shuffle at a
// 12-byte stride reproduces retail's sp+8/sp+0x14/sp+0x20 copy sequence;
// v1 must survive the second value call, so it lives in saved f31.
void CItemBoxGridFull::SortGridByValue() {
    u16 i;
    for (i = 0; i < this->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        for (j = 0; j < this->field_2800 - 1 - i; j++) {
            CItemBoxGridEntry* e1 = &this->entries[j];
            CItemBoxGridEntry* e2 = &this->entries[j + 1];
            void* obj1 = CItem_kindPtr(this->field_2802, e1->id);
            void* obj2 = CItem_kindPtr(this->field_2802, e2->id);
            float v1 = ((CItemBoxGridFull*)(this))->GetItemValueFloat(obj1);
            float v2 = ((CItemBoxGridFull*)(this))->GetItemValueFloat(obj2);
            if (v1 >= v2) continue;
            char tmp[36];
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 12, (const char*)e2);
            CopyGridEntry9(e1, tmp + 12);
            CopyEntry9Bytes(tmp + 24, (const char*)tmp);
            CopyGridEntry9(e2, tmp + 24);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries ascending by the item instance's vtable+0x30
// slot value (u16). v2 is evaluated first, matching retail's call order;
// same 3-slot temp shuffle shape as SortByItemU16.
void CItemBoxGridFull::SortByItemVf30() {
    u16 i;
    for (i = 0; i < this->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < this->field_2800 - 1 - i; j++) {
            CItemBoxGridEntry* e1 = &this->entries[j];
            CItemBoxGridEntry* e2 = &this->entries[j + 1];
            void* obj1 = CItem_kindPtr(this->field_2802, e1->id);
            void* obj2 = CItem_kindPtr(this->field_2802, e2->id);
            u32 v2 = CItem_initItemImplInstances((CItemData*)obj2)->vf30((CItemData*)obj2);
            u32 v1 = CItem_initItemImplInstances((CItemData*)obj1)->vf30((CItemData*)obj1);
            if ((u16)v1 >= (u16)v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries ascending by HasEquippedSlot slot status (swap when
// item j's value is strictly lower). Same 3-slot temp shuffle shape as
// SortByItemU16.
void CItemBoxGridFull::SortByEquipAsc() {
    u16 i;
    for (i = 0; i < this->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < this->field_2800 - 1 - i; j++) {
            CItemBoxGridEntry* e1 = &this->entries[j];
            CItemBoxGridEntry* e2 = &this->entries[j + 1];
            void* obj1 = CItem_kindPtr(this->field_2802, e1->id);
            void* obj2 = CItem_kindPtr(this->field_2802, e2->id);
            u32 v1 = this->HasEquippedSlot(obj1);
            u32 v2 = this->HasEquippedSlot(obj2);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries descending by HasEquippedSlot slot status (swap when
// item j's value is strictly higher). Same 3-slot temp shuffle shape as
// SortByItemU16.
void CItemBoxGridFull::SortByEquipDesc() {
    u16 i;
    for (i = 0; i < this->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < this->field_2800 - 1 - i; j++) {
            CItemBoxGridEntry* e1 = &this->entries[j];
            CItemBoxGridEntry* e2 = &this->entries[j + 1];
            void* obj1 = CItem_kindPtr(this->field_2802, e1->id);
            void* obj2 = CItem_kindPtr(this->field_2802, e2->id);
            u32 v1 = this->HasEquippedSlot(obj1);
            u32 v2 = this->HasEquippedSlot(obj2);
            if (v1 <= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort grid entries by FindKindPoolIdx slot status, ascending (swap
// when entry j's status is strictly higher). Same 3-slot temp shuffle as
// SortGridByValue; both value calls pass (self, item).
void CItemBoxGridFull::SortGridBySlotStatus() {
    u16 i;
    for (i = 0; i < this->field_2800 - 1; i++) {
        u16 j;
        int swapped = 0;
        for (j = 0; j < this->field_2800 - 1 - i; j++) {
            CItemBoxGridEntry* e1 = &this->entries[j];
            CItemBoxGridEntry* e2 = &this->entries[j + 1];
            void* obj1 = CItem_kindPtr(this->field_2802, e1->id);
            void* obj2 = CItem_kindPtr(this->field_2802, e2->id);
            u8 v1 = (u8)((CItemBoxGridFull*)(this))->FindKindPoolIdx(obj1);
            u8 v2 = (u8)((CItemBoxGridFull*)(this))->FindKindPoolIdx(obj2);
            if (v1 <= v2) continue;
            char tmp[36];
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries ascending by the byte value of each item's kind
// name lookup: the item's kind (word>>20) is mapped through BdatGetItemId to
// a short id, which is looked up in the owner table (::cf::CfBdat::spItmWpnListFileData) with
// the sort-key string; swap when item j's key byte is strictly lower. Same
// 3-slot temp shuffle shape as SortByItemU16.
void CItemBoxGridFull::SortGridByKindName() {
    u16 i;
    for (i = 0; i < this->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < this->field_2800 - 1 - i; j++) {
            CItemBoxGridEntry* e1 = &this->entries[j];
            CItemBoxGridEntry* e2 = &this->entries[j + 1];
            void* obj1 = CItem_kindPtr(this->field_2802, e1->id);
            void* obj2 = CItem_kindPtr(this->field_2802, e2->id);
            u32 w1 = ((CItemBoxItem*)obj1)->mWord;
            u32 t1 = (u32)(size_t)::cf::CfBdat::spItmWpnListFileData;
            u32 k1 = BdatGetItemId(w1 >> 20);
            u8 c1 = BdatGetU8Direct((const void*)t1, (const char*)"att_lev", k1 & 0xFFFF);
            u32 w2 = ((CItemBoxItem*)obj2)->mWord;
            u32 t2 = (u32)(size_t)::cf::CfBdat::spItmWpnListFileData;
            u32 k2 = BdatGetItemId(w2 >> 20);
            u8 c2 = BdatGetU8Direct((const void*)t2, (const char*)"att_lev", k2 & 0xFFFF);
            if ((u32)c1 >= (u32)c2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries ascending by the byte value of each item's kind
// name lookup, using the alternate owner table key string (offset 0x22f).
// Same per-item kind->lookup interleave and temp shuffle as SortGridByKindName.
void CItemBoxGridFull::SortGridByKindNameAlt() {
    u16 i;
    for (i = 0; i < this->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < this->field_2800 - 1 - i; j++) {
            CItemBoxGridEntry* e1 = &this->entries[j];
            CItemBoxGridEntry* e2 = &this->entries[j + 1];
            void* obj1 = CItem_kindPtr(this->field_2802, e1->id);
            void* obj2 = CItem_kindPtr(this->field_2802, e2->id);
            // Owner-table row is selected by the raw item word used as a
            // byte offset into the table data.
            u32 w1 = ((CItemBoxItem*)obj1)->mWord;
            u8 c1 = BdatGetU8Direct((const void*)(*(u32*)((u8*)::cf::CfBdat::spItmWpnListFileData + w1)),
                                   (const char*)"grd_rate",
                                   BdatGetItemId(w1 >> 20));
            u32 w2 = ((CItemBoxItem*)obj2)->mWord;
            u8 c2 = BdatGetU8Direct((const void*)(*(u32*)((u8*)::cf::CfBdat::spItmWpnListFileData + w2)),
                                   (const char*)"grd_rate",
                                   BdatGetItemId(w2 >> 20));
            if (c1 >= c2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries ascending by the byte value of each item's kind
// name lookup in the capacity table (::cf::CfBdat::spItmEquipListFileData). Both kinds are
// computed first, then both lookups - the SortByKindNameD ordering. Same
// 3-slot temp shuffle shape as SortByItemU16.
void CItemBoxGridFull::SortByCapKeyAsc() {
    u16 i;
    for (i = 0; i < this->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < this->field_2800 - 1 - i; j++) {
            CItemBoxGridEntry* e1 = &this->entries[j];
            CItemBoxGridEntry* e2 = &this->entries[j + 1];
            void* obj1 = CItem_kindPtr(this->field_2802, e1->id);
            void* obj2 = CItem_kindPtr(this->field_2802, e2->id);
            u32 w1 = ((CItemBoxItem*)obj1)->mWord;
            u16 k1 = BdatGetItemId(w1 >> 20);
            u32 w2 = ((CItemBoxItem*)obj2)->mWord;
            u16 k2 = BdatGetItemId(w2 >> 20);
            u8 c1 = BdatGetU8Direct((const void*)::cf::CfBdat::spItmEquipListFileData, (const char*)&lbl_eu_8050566C[0x238], k1 & 0xFFFF);
            u8 c2 = BdatGetU8Direct((const void*)::cf::CfBdat::spItmEquipListFileData, (const char*)&lbl_eu_8050566C[0x238], k2 & 0xFFFF);
            if ((u32)c1 >= (u32)c2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries ascending by the byte value of each item's kind
// name lookup in the capacity table (::cf::CfBdat::spItmEquipListFileData), key string offset
// 0x240. Same ordering and temp shuffle shape as SortByCapKeyAsc.
void CItemBoxGridFull::SortByCapKey2Asc() {
    u16 i;
    for (i = 0; i < this->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < this->field_2800 - 1 - i; j++) {
            CItemBoxGridEntry* e1 = &this->entries[j];
            CItemBoxGridEntry* e2 = &this->entries[j + 1];
            void* obj1 = CItem_kindPtr(this->field_2802, e1->id);
            void* obj2 = CItem_kindPtr(this->field_2802, e2->id);
            u32 w1 = ((CItemBoxItem*)obj1)->mWord;
            u16 k1 = BdatGetItemId(w1 >> 20);
            u32 w2 = ((CItemBoxItem*)obj2)->mWord;
            u16 k2 = BdatGetItemId(w2 >> 20);
            u8 c1 = BdatGetU8Direct((const void*)::cf::CfBdat::spItmEquipListFileData, (const char*)&lbl_eu_8050566C[0x240], k1 & 0xFFFF);
            u8 c2 = BdatGetU8Direct((const void*)::cf::CfBdat::spItmEquipListFileData, (const char*)&lbl_eu_8050566C[0x240], k2 & 0xFFFF);
            if ((u32)c1 >= (u32)c2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Partition-style bubble pass: move every entry whose kind-name lookup key
// (kind mapped through BdatGetItemId then looked up in table ::cf::CfBdat::spItmEquipListFileData
// with the string at lbl_eu_8050566C+0x248) equals `target` toward the front.
// Swap fires when the earlier entry mismatches and the later one matches
// (retail compares the two "== target" booleans); the whole sort aborts as
// soon as one full sweep makes no swap.
void CItemBoxGridFull::GroupByCapKey(u32 target) {
    // Hoisted string-pool base: retail keeps this pointer live in a
    // callee-saved register for the whole function.
    const char* strBase = lbl_eu_8050566C;
    u32 i;
    for (i = 0; (u16)i < this->field_2800 - 1; i++) {
        u32 swapped = 0;
        u32 j;
        char tmp[36];
        for (j = 0; (u16)j < this->field_2800 - 1 - (s32)(u16)i; j++) {
            CItemBoxGridEntry* e1 = &this->entries[(u16)j];
            CItemBoxGridEntry* e2 = &this->entries[(u16)j + 1];
            void* obj1 = CItem_kindPtr(this->field_2802, e1->id);
            void* obj2 = CItem_kindPtr(this->field_2802, e2->id);
            // Both kind keys are looked up before either byte is truncated:
            // retail keeps the first lookup result live across the second call.
            u16 k1 = BdatGetItemId(*(u32*)obj1 >> 20);
            u16 k2 = BdatGetItemId(*(u32*)obj2 >> 20);
            u8 c1 = BdatGetU8Direct((const void*)::cf::CfBdat::spItmEquipListFileData, strBase + 0x248, k1 & 0xFFFF);
            u8 c2 = BdatGetU8Direct((const void*)::cf::CfBdat::spItmEquipListFileData, strBase + 0x248, k2 & 0xFFFF);
            // Swap only when entry j+1 matches the target and entry j doesn't:
            // this drives all target-matching entries to the front.
            u32 m2 = (c2 == target);
            u32 m1 = (c1 == target);
            if (m1 < m2) {
                CopyEntry9Bytes(tmp, (const char*)e1);
                CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
                CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries DESCENDING by the byte value of each item's kind
// name lookup in the capacity table (::cf::CfBdat::spItmEquipListFileData), key string offset
// 0x251 (swap when item j's key byte is strictly higher). Same ordering
// and temp shuffle shape as SortByCapKeyAsc.
void CItemBoxGridFull::SortByCapKeyDesc() {
    u16 i;
    for (i = 0; i < this->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < this->field_2800 - 1 - i; j++) {
            CItemBoxGridEntry* e1 = &this->entries[j];
            CItemBoxGridEntry* e2 = &this->entries[j + 1];
            void* obj1 = CItem_kindPtr(this->field_2802, e1->id);
            void* obj2 = CItem_kindPtr(this->field_2802, e2->id);
            u32 w1 = ((CItemBoxItem*)obj1)->mWord;
            u16 k1 = BdatGetItemId(w1 >> 20);
            u32 w2 = ((CItemBoxItem*)obj2)->mWord;
            u16 k2 = BdatGetItemId(w2 >> 20);
            u8 c1 = BdatGetU8Direct((const void*)::cf::CfBdat::spItmEquipListFileData, (const char*)&lbl_eu_8050566C[0x251], k1 & 0xFFFF);
            u8 c2 = BdatGetU8Direct((const void*)::cf::CfBdat::spItmEquipListFileData, (const char*)&lbl_eu_8050566C[0x251], k2 & 0xFFFF);
            if ((u32)c1 <= (u32)c2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries ascending by the item instance's vtable+0x90
// slot value (byte). v1 is evaluated first, matching retail's call order;
// same 3-slot temp shuffle shape as SortByItemU16.
void CItemBoxGridFull::SortByItemVf90() {
    u16 i;
    for (i = 0; i < this->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < this->field_2800 - 1 - i; j++) {
            CItemBoxGridEntry* e1 = &this->entries[j];
            CItemBoxGridEntry* e2 = &this->entries[j + 1];
            void* obj1 = CItem_kindPtr(this->field_2802, e1->id);
            void* obj2 = CItem_kindPtr(this->field_2802, e2->id);
            u32 v1 = (u32)CItem_initItemImplInstances((CItemData*)obj1)->vf90((CItemData*)obj1);
            u32 v2 = (u32)CItem_initItemImplInstances((CItemData*)obj2)->vf90((CItemData*)obj2);
            if ((u8)v1 >= (u8)v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries in DESCENDING order of the kind's message-table
// lookup key (string at lbl_eu_8050566C+0x115): adjacent pair (j, j+1) is
// swapped when entry j's key byte exceeds entry j+1's. Lookup is inlined
// (same chain as the 100% sibling GroupByKindName) so the SDA load of
// ::cf::CfBdat::spItmArtsListFileData and the 39358/361E8 calls stay in this function.
void CItemBoxGridFull::SortGridByMsgKeyDesc() {
    u32 i;
    for (i = 0; (u16)i < this->field_2800 - 1; i++) {
        int swapped = 0;
        u32 j;
        char tmp[36];
        for (j = 0; (u16)j < this->field_2800 - 1 - (u16)i; j++) {
            // tbl declared first, assigned after the 57C4C calls so the SDA
            // load stays at retail +0x70 (early assign moved it and added
// structural). Residual: e1<->tbl r23/r24.
            u32 tbl;
            CItemBoxGridEntry* e1 = &this->entries[(u16)j];
            CItemBoxGridEntry* e2 = &this->entries[(u16)j + 1];
            void* obj1 = CItem_kindPtr(this->field_2802, *(s16*)((u8*)this + (u16)j * 10));
            void* obj2 = CItem_kindPtr(this->field_2802, e2->id);
            u32 w1 = ((CItemBoxItem*)obj1)->mWord;
            tbl= (u32)::cf::CfBdat::spItmArtsListFileData;
            u8 c1 = BdatGetU8Direct((const void*)tbl, (const char*)"pc_type", (u16)BdatGetItemId(w1 >> 20));
            u32 w2 = ((CItemBoxItem*)obj2)->mWord;
            u32 tbl2= (u32)::cf::CfBdat::spItmArtsListFileData;
            u8 c2 = BdatGetU8Direct((const void*)tbl2, (const char*)"pc_type", (u16)BdatGetItemId(w2 >> 20));
            if (c1 <= c2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries so items whose kind-name lookup matches `target`
// group at the front: an adjacent pair (j, j+1) is swapped when entry j+1's
// key byte equals target but entry j's does not. Key = message-table
// (::cf::CfBdat::spItmArtsListFileData) lookup of the kind's short id ("name" key at +0x109).
void CItemBoxGridFull::GroupByKindName(u32 target) {
    u32 i;
    for (i = 0; (u16)i < this->field_2800 - 1; i++) {
        int swapped = 0;
        u32 j;
        char tmp[36];
        for (j = 0; (u16)j < this->field_2800 - 1 - (u16)i; j++) {
            CItemBoxGridEntry* e1 = &this->entries[(u16)j];
            CItemBoxGridEntry* e2 = &this->entries[(u16)j + 1];
            void* obj1 = CItem_kindPtr(this->field_2802, e1->id);
            void* obj2 = CItem_kindPtr(this->field_2802, e2->id);
            u32 w1 = ((CItemBoxItem*)obj1)->mWord;
            u32 tbl= (u32)::cf::CfBdat::spItmArtsListFileData;
            u8 c1 = BdatGetU8Direct((const void*)tbl, (const char*)&lbl_eu_8050566C[0x109], (u16)BdatGetItemId(w1 >> 20));
            u32 w2 = ((CItemBoxItem*)obj2)->mWord;
            u32 tbl2= (u32)::cf::CfBdat::spItmArtsListFileData;
            u8 c2 = BdatGetU8Direct((const void*)tbl2, (const char*)&lbl_eu_8050566C[0x109], (u16)BdatGetItemId(w2 >> 20));
            u32 m1 = c1 == target;
            u32 m2 = c2 == target;
            if (m1 < m2) {
                CopyEntry9Bytes(tmp, (const char*)e1);
                CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
                CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

// Item sort value for an entry: look up the kind's index in the owner table
// (::cf::CfBdat::spItmWpnListFileData); when the kind's flag entry has bit 2 set, scale the
// index by party member 1's CtrlObjectParam_GetArtsDataWord value, then scale by the 0x67F80
// factor, clamp to 999 and return as a float.
// -O4,s: retail uses the bl _savegpr_28/_restgpr_28 helper-call
// prologue, which only size-opt emits on this unit.
float CItemBoxGridFull::GetItemValueFloat(void* entry) {
    u32 word = *(u32*)entry;
    u32 tbl = (u32)(size_t)::cf::CfBdat::spItmWpnListFileData;
    u16 kind = BdatGetItemId(word >> 20);
    u32 idx = BdatGetU16Direct((void*)tbl, (const char*)"dmg_hi", (u16)kind);
    // Bit 2 of the kind flag entry gates a party-bonus multiplier.
    if ((BdatGetU8Direct((const void*)tbl, (const char*)"flag", (u16)kind) & 4)) {
        void* charData = CtrlObjectParam_GetCharEntry(1);
        // Reading the product back through a signed union member blocks
        // MWCC's non-negative range fold, so the conversion uses the signed
        // xoris magic (lbl_eu_80667F48).
        union {
            u32 u;
            int i;
        } prod;
        prod.u = (idx & 0xFFFF) * (u16)CtrlObjectParam_GetArtsDataWord(charData);
        idx = (u32)(u16)(s32)((float)prod.i * 1.5f);
    }
    if ((u32)(u16)idx > 999) idx = 999;
    // Truncated result converted back through the 2^52 magic (lbl_eu_80667F40).
    return (float)((double)(u16)idx - lbl_eu_80667F40);
}

u32 CItemBoxGridFull::HasEquippedSlot(void* item) {
    CItemImpl* inst = CItem_initItemImplInstances((CItemData*)item);
    u16 count = (u16)inst->vf30((CItemData*)item);
    u8 i;
    for (i = 0; i < count; i++) {
        CItemImpl* inst2 = CItem_initItemImplInstances((CItemData*)item);
        void* obj = inst2->vf2C((CItemData*)item, i);
        if (obj && (reinterpret_cast<CItemBoxSlotState*>(obj)->state & 1)) return 1;
    }
    return 0;
}

// Check if entry kind exists in pool.
u32 CItemBoxGridFull::FindKindPoolIdx(void* entry) {
    u32 val = *(u32*)entry;
    u32 obj = (u32)::cf::CfBdat::spItmWpnListFileData;
    u16 kind = BdatGetItemId(val >> 20);
    u32 i;
    for (i = 1; i <= 10; i++) {
        char buf[40];
        sprintf(buf, (const char*)&lbl_eu_8050566C[0x132], (u8)i);
        if ((u8)BdatGetU8Direct((const void*)obj, (const char*)buf, (u16)kind)) return i;
    }
    return 0;
}

// CItemBoxGrid constructor: the members are built in declaration order
// (mode/unk params land in the header and +0x527..+0x543 bytes), then the
// default configurations are copied in from temporaries: sort menu, item
// info (mode-derived kind), num select, exchange window (mode 2 only), the
// two sys-wins (copied through the out-of-line InfoCfCopySysWin) and the
// cell grid.
CItemBoxGrid::CItemBoxGrid(u8 mode, u16 unk5, u32 unk6, u8 unk7)
    : field_04(unk6),
      field_28(0), field_2C(0), field_30(0), field_34(0), field_38(0),
      field_3C(0), field_40(0), field_44(0), field_48(0), field_4C(0), field_50(0),
      field_54(0), field_58(0), field_5C(0), field_60(0), field_61(1),
      field_6E(0), field_6F(0),
      mInfo(0, 0),
      mNumSelect(1),
      mSysWin4AC(0),
      mSysWin4E8(0),
      field_524(0), field_525(0), field_526(0), field_527(mode), field_528(0),
      field_529(1), field_52A(unk5), field_52C(0), field_52D(0), field_52E(0),
      field_530(0), field_540(1), field_541(0), field_542(0), field_543(unk7),
      field_544(0), field_545(0), field_546(0), field_547(0), field_548(0),
      field_549(0) {
    memset(field_62, 0, sizeof(field_62));

    mSortMenu = CSortMenu();

    int infoKind = 0;
    if (mode == 1) infoKind = 1;
    else if (mode == 2) infoKind = 3;
    mInfo = CItemBoxInfo(infoKind, field_52A);

    mNumSelect = CNumSelect(1);

    if (mode == 2) {
        mExchangeWin = CExchangeWin();
    }

    mSysWin4AC = CSysWin(2);
    mSysWin4E8 = CSysWin(0);

    itemBoxGrid_shopItemTypeTable = 0;
    itemBoxGrid_shopSkillTable = 0;
    itemBoxGrid_shopItemIdTable = 0;
    ItemBlockSetDefaultFlags();

    mGrid = CItemBoxGridFull();

    CItemBlock_setFlag120EC(2, 0);
    CItemBlock_setFlag120EC(4, 0);
    CItemBlock_setFlag120EC(5, 0);
    CItemBlock_setFlag120EC(6, 0);
    CItemBlock_setFlag120EC(7, 0);
    CItemBlock_setFlag120EC(8, 0);
    CItemBlock_setFlag120EC(3, 0);
    CItemBlock_setFlag120EC(9, 0);
    CItemBlock_setFlag120EC(0xa, 0);
    CItemBlock_setFlag120EC(0xb, 0);
    CItemBlock_setFlag120EC(0xc, 0);
    CItemBlock_setFlag120EC(0xd, 0);
}

// CItemBoxGrid destructor: the members are destroyed in reverse order and
// the block is freed when the deleting-dtor flag is set.
CItemBoxGrid::~CItemBoxGrid() {
}

// Load the item-box resource files. When MEM1 can hold the biggest archive,
// the first file is read into MEM1 instead of MEM2. The string-table base is
// cached in a local (retail keeps it in r31 across every call).
// -O4,s: retail uses stmw r29/lmw r29 block saves.
void CItemBoxGrid::LoadItemBoxFiles() {
    const char* strs;
    void* allocHandle = getHandleMEM2__Q23mtl10MemManagerFv();

    if (MakeCrystalIsCreated()) {
        if (getMaxAllocSize__Q23mtl10MemManagerFUl((u32)getHandleMEM1__Q23mtl10MemManagerFv()) >= 0x38000) {
            allocHandle = (void*)getHandleMEM1__Q23mtl10MemManagerFv();
        }
    }

    // Retail caches the string-table base here (r31), after the MEM1 choice.
    strs = lbl_eu_8050566C;

    field_28 = (CFileHandle*)readFile__11CDeviceFileFUlPCcP10IWorkEventii((u32)allocHandle, strs + 0x266, this, 0, 0);

    allocHandle = getHandleMEM2__Q23mtl10MemManagerFv();
    field_2C = (CFileHandle*)readFile__11CDeviceFileFUlPCcP10IWorkEventii((u32)allocHandle, strs + 0x27e, this, 0, 0);

    if (field_543) {
        allocHandle = (void*)KyoshinHeap_GetField44();
        field_30 = (CFileHandle*)readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii((u32)allocHandle, strs + 0x297, this, 0, 0);

        allocHandle = getHandleMEM2__Q23mtl10MemManagerFv();
        field_34 = (CFileHandle*)readFile__11CDeviceFileFUlPCcP10IWorkEventii((u32)allocHandle, strs + 0x2b4, this, 0, 0);
    } else {
        itemBoxGrid_shopItemTypeTable = (u32)getFP(strs + 0x2d0);
        itemBoxGrid_shopSkillTable = (u32)getFP(strs + 0x2df);
        itemBoxGrid_shopItemIdTable = (u32)getFP(strs + 0x2ee);
    }

    mSortMenu.Init();
    mInfo.LoadFiles();
    mNumSelect.LoadFiles();

    if (field_527 == 2) {
        mExchangeWin.ExWin_LoadFile();
    }

    mSysWin4AC.loadSystemArc();
    mSysWin4E8.loadSystemArc();
}

// Per-frame state machine update for the item-box grid: gate on the ready
// byte at +0x54 and the state word at +0x58, dispatch through MWCC's
// computed-jump switch, then run the common per-frame update tail.
void CItemBoxGrid::UpdateItemBox() {
    u8* p = (u8*)this;
    if (!this->field_54) return;
    u32 state = this->field_58;
    if (!state) return;

    // Bare switch: MWCC emits the single cmplwi 0x1a/bgt range check plus
    // the computed-jump table (gaps fall to the common tail) -- an explicit
    // `if (state <= 0x1a)` wrapper would add a second range branch.
    switch (state) {
        case 1:
            AdvanceOpenAnim();
            break;
        case 2:
            FinishOpenAnim();
            break;
        case 3:
            CheckState4_Animate();
            break;
        case 4:
            AdvanceCloseAnim();
            break;
        case 5:
            FinishCloseAnim();
            break;
        case 6:
            if (reinterpret_cast<CNumSelect*>(p + 0x3e4)->IsIdle()) {
                this->field_58 = 0x19;
            }
            break;
        case 8:
            FinishNumSelect();
            break;
        case 9:
            ShowSubMenuCur();
            break;
        case 11:
            DispatchSubmenuConfirm();
            break;
        case 12:
            if (reinterpret_cast<CNumSelect*>(p + 0x3e4)->IsIdle()) {
                this->field_58 = 0xA;
            }
            break;
        case 14:
            if (reinterpret_cast<CNumSelect*>(p + 0x3e4)->IsIdle()) {
                this->field_58 = 0x3;
            }
            break;
        case 15:
            ShowExchangeCur();
            break;
        case 17:
            FinishExchange();
            break;
        case 18:
            ShowPresentCur();
            break;
        case 20:
            FinishPresent();
            break;
        case 21:
            OnSysWinOpen();
            break;
        case 23:
            HandleSysWinOK();
            break;
        case 24:
            if (reinterpret_cast<CSysWin*>(p + 0x4e8)->isActive()) {
                this->field_58 = 0x13;
            }
            break;
        case 26:
            FinishSysWin();
            break;
    }

    // Virtual dispatch through the +0x44 layout object (vtable+0x38 slot);
    // method takes only the flag - this comes in r3, arg in r4.
    nw4r::lyt::Layout* layout = this->field_44;
    layout->Animate(0);

    ((CBaseCur*)(p + 0x70))->Update();
    ((CBaseCur*)(p + 0x88))->Update();
    ((CBaseCur*)(p + 0xa0))->Update();
    ((CBaseCur*)(p + 0xb8))->Update();
    ((CBaseCur*)(p + 0xd0))->Update();
    ((CSortMenu*)(p + 0xe8))->Move();
    ((CItemBoxInfo*)(p + 0x1d8))->UpdateAnims();
    reinterpret_cast<CNumSelect*>(p + 0x3e4)->UpdateStates();
    ((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_Move();
    ((CExchangeWin*)(p + 0x440))->ExWin_TickUpdate();
    updatePresentWin(reinterpret_cast<CPresentWin*>(p + 0x468));
    reinterpret_cast<CSysWin*>(p + 0x4ac)->dispatchPhase();
    reinterpret_cast<CSysWin*>(p + 0x4e8)->dispatchPhase();
}

// Draw callback: update the exchange/num-select widgets, then draw the
// layout and all sub-panels. -O4,s merges the r30/r31 saves
// into retail's stmw prologue.
void CItemBoxGrid::DrawItemBoxGrid(nw4r::lyt::DrawInfo* drawInfo) {
    u8* p = (u8*)this;
    if (!p[0x54]) return;
    if (!this->field_58) return;

    ((CItemBoxInfo*)(p + 0x1d8))->DrawLayout((nw4r::lyt::DrawInfo*)drawInfo);
    drawLayout(this->field_44, (nw4r::lyt::DrawInfo*)drawInfo, 0, 1);

    u8 r = p[0x2d4f];
    u8 v = (r != 0) ? r : 1;
    if ((u32)v > 1) {
        ((CBaseCur*)(p + 0x88))->DrawLayout((nw4r::lyt::DrawInfo*)drawInfo);
    }

    ((CSortMenu*)(p + 0xe8))->Draw((nw4r::lyt::DrawInfo*)drawInfo);
    ((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_DrawLayoutGated(drawInfo);
    ((CExchangeWin*)(p + 0x440))->ExWin_DrawLayout(drawInfo);
    reinterpret_cast<CSysWin*>(p + 0x4ac)->drawLayout(reinterpret_cast<nw4r::lyt::DrawInfo*>((void*)drawInfo));
    reinterpret_cast<CSysWin*>(p + 0x4e8)->drawLayout(reinterpret_cast<nw4r::lyt::DrawInfo*>((void*)drawInfo));
    drawPresentWin(reinterpret_cast<CPresentWin*>(p + 0x468), drawInfo);
    ((CBaseCur*)(p + 0xd0))->DrawLayout((nw4r::lyt::DrawInfo*)drawInfo);
    ((CBaseCur*)(p + 0x70))->DrawLayout((nw4r::lyt::DrawInfo*)drawInfo);
    ((CBaseCur*)(p + 0xa0))->DrawLayout((nw4r::lyt::DrawInfo*)drawInfo);
    ((CBaseCur*)(p + 0xb8))->DrawLayout((nw4r::lyt::DrawInfo*)drawInfo);
    reinterpret_cast<CNumSelect*>(p + 0x3e4)->DrawLayout((nw4r::lyt::DrawInfo*)drawInfo);
}

// Teardown: release the CBdat banners, file handles, heap block, layout
// sub-objects and all UI panes, in retail's exact order. -O4,s
// merges the r30/r31 saves into retail's stmw prologue, and the delete-style
// releases reproduce the double null test (if + delete's own guard).
void CItemBoxGrid::UnloadItemBox() {
    u8* p = (u8*)this;

    if (this->field_543) {
        CBdat::getEntry(2);
        CBdat::getEntry(5);
    }

    closeFileHandle((CFileHandle**)&this->field_28);
    closeFileHandle((CFileHandle**)&this->field_2C);
    closeFileHandle((CFileHandle**)&this->field_30);
    closeFileHandle((CFileHandle**)&this->field_34);

    void* ptr38 = this->field_38;
    this->field_54 = 0;
    if (ptr38) {
        deallocate__Q23mtl10MemManagerFPv(ptr38);
        this->field_38 = 0;
    }

    void* obj44 = this->field_44;
    if (obj44) {
        delete (nw4r::lyt::Layout*)obj44;
        this->field_44 = 0;
    }

    releaseArcResourceAccessor(this->field_3C);
    releaseArcResourceAccessor(this->field_40);

    void* obj5c = this->field_5C;
    if (obj5c) {
        // CTagProcessor (built by __ct__CTagProcessor above) derives from
        // CTagProcessorBase : nw4r::ut::TagProcessorBase<wchar_t> (virtual
        // dtor at +0x08); deleting through the nw4r base keeps the retail
        // deleting-dtor dispatch without pulling CTagProcessor.hpp (whose
        // C++ AccumPaneWorldPos decl clashes with this TU's extern "C" one).
        delete (nw4r::ut::TagProcessorBase<wchar_t>*)obj5c;
        this->field_5C = 0;
    }

    deleteRegion__17UnkClass_8045F564Fv(p + 0x8);
    deleteRegion__17UnkClass_8045F564Fv(p + 0x18);

    ((CBaseCur*)(p + 0x70))->cleanup();
    ((CBaseCur*)(p + 0x88))->cleanup();
    ((CBaseCur*)(p + 0xa0))->cleanup();
    ((CBaseCur*)(p + 0xb8))->cleanup();
    ((CBaseCur*)(p + 0xd0))->cleanup();

    ((CSortMenu*)(p + 0xe8))->Term();
    ((CItemBoxInfo*)(p + 0x1d8))->UnloadFiles();
    reinterpret_cast<CNumSelect*>(p + 0x3e4)->UnloadFiles();
    ((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_TermLayout();
    ((CExchangeWin*)(p + 0x440))->ExWin_Teardown();
    termPresentWin(p + 0x468);
    reinterpret_cast<CSysWin*>(p + 0x4ac)->termLayout();
    reinterpret_cast<CSysWin*>(p + 0x4e8)->termLayout();
}

// Check if item grid is fully ready.
u32 CItemBoxGrid::IsItemBoxReady() {
    u8* p = (u8*)this;
    if (!((CSortMenu*)(p + 0xe8))->IsReady()) return 0;
    if (!((CItemBoxInfo*)(p + 0x1d8))->GetState()) return 0;
    if (!reinterpret_cast<CNumSelect*>(p + 0x3e4)->IsLoaded()) return 0;
    if (!((CExchangeWin*)(p + 0x440))->getField25()) return 0;
    if (!reinterpret_cast<CSysWin*>(p + 0x4ac)->isReady()) return 0;
    if (reinterpret_cast<CSysWin*>(p + 0x4e8)->isReady()) return p[0x60];
    return 0;
}


// Check if item grid is active.
u32 CItemBoxGrid::IsItemBoxActive() {
    u8* p = (u8*)this;
    if (p[0x528]) return 1;
    if (reinterpret_cast<CSysWin*>(p + 0x4ac)->isOpened()) return 1;
    if (reinterpret_cast<CSysWin*>(p + 0x4e8)->isOpened()) return 1;
    if (p[0x544]) return 1;
    return ((CSortMenu*)(p + 0xe8))->IsOpen();
}

// Get field depending on window state.
int CItemBoxGrid::GetIdleFlag542() {
    if (reinterpret_cast<CSysWin*>(((CibgOuterSlots*)this)->m4E8)->isOpened()) return 0;
    s32 state = (s32)this->field_58;
    if (state != 3) return 0;
    return this->field_542;
}

u8 CItemBoxGrid::GetField549() { return field_549; }

// Check if any sub-system is active.
u32 CItemBoxGrid::IsSubWinActive() {
    u8* p = (u8*)this;
    if (reinterpret_cast<CNumSelect*>(p + 0x3e4)->IsOpen()) return 1;
    if (((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_IsVisibleFlag()) return 1;
    if (((CExchangeWin*)(p + 0x440))->getField24()) return 1;
    if (isPresentActive(reinterpret_cast<CPresentWin*>(p + 0x468))) return 1;
    if (reinterpret_cast<CSysWin*>(p + 0x4ac)->isOpened()) return 1;
    return reinterpret_cast<CSysWin*>(p + 0x4e8)->isOpened();
}

// Initialize item display state. `item` (GetCellItemPtr result) is declared
// first so MWCC colors it to r31 (first-declared -> highest saved reg).
void CItemBoxGrid::InitItemDisplay() {
    u32 item;   // GetCellItemPtr result
    u8* p = (u8*)this;
    if (*(u32*)(p + 0x58)) return;
    *(u32*)(p + 0x58) = 1;
    p[0x61] = 0;
    RefreshCategoryTabs();
    func_801D0BD8();
    ((CItemBoxInfo*)(p + 0x1D8))->Open();
    ((CItemBoxInfo*)(p + 0x1D8))->HidePanes((p + (s8)p[0x6F])[0x62]);
    u8 idx0 = p[0x525];
    u8 idx1 = p[0x524];
    u32 entry_idx = (idx1 + idx0 * 10) & 0xFF;
    u8* sub = p + 0x54C;
    item = (u32)((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellItemPtr((u16)entry_idx);
    u32 r2 = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellKind((u16)entry_idx);
    ((CItemBoxInfo*)(p + 0x1D8))->SetItem(r2 & 0xFFFF, (void*)item, 1);
    u32 v = ((CItemBoxGridFull*)(sub))->FormatCellName(entry_idx);
    ((CItemBoxInfo*)((void*)(p + 0x1D8)))->SetNamedText(1, (char*)v);
    ((CBaseCur*)(p + 0xB8))->SetVisible(0);
    p[0x549] = 0;
    if (p[0x527] == 4) ((CItemBoxInfo*)(p + 0x1D8))->SetResultPane(1);
}

// Advance item box state.
void CItemBoxGrid::AdvanceBoxState() {
    u8* p = (u8*)this;
    if ((s32)this->field_58 != 3) return;
    if (((CSortMenu*)(p + 0xe8))->IsOpen()) return;
    this->field_58 = 4;
    this->field_44->SetAnimationEnable(this->field_50, 0);
    this->field_44->SetAnimationEnable(this->field_48, 0);
    this->field_44->SetAnimationEnable(this->field_4C, 1);
    this->field_61 = 0;
    ((CBaseCur*)(p + 0x70))->SetVisible(0);
    ((CBaseCur*)(p + 0x88))->SetVisible(0);
    ((CBaseCur*)(p + 0xd0))->SetVisible(0);
    ((CBaseCur*)(p + 0xb8))->SetVisible(0);
    ((CItemBoxInfo*)(p + 0x1d8))->Close();
    if (!this->field_52C) playUISound(6);
}

// Clear a 14-byte region (list/array init). Retail uses memset for the
// 12-byte block at +0x62, then two byte clears.
void CItemBoxGrid::ClearListSlots() {
    u8* p = (u8*)this;
    memset(p + 0x62, 0, 12);
    p[0x6e] = 0;
    this->field_6F = 0;
}

void CItemBoxGrid::PushToList(unsigned char val) {
    unsigned char count = reinterpret_cast<unsigned char*>(this)[0x6e];
    if (count >= 0xc) {
        return;
    }
    unsigned char* slot = reinterpret_cast<unsigned char*>(this) + count;
    slot[0x62] = val;
    reinterpret_cast<unsigned char*>(this)[0x6e] = count + 1;
}

// Increment list counter with wrap.
void CItemBoxGrid::IncListIndex() {
    u8* p = (u8*)this;
    if (((CSortMenu*)(p + 0xe8))->IsOpen()) return;
    if (p[0x528]) return;
    u8 idx = this->field_6F + 1;
    this->field_6F = idx;
    if ((s8)idx >= p[0x6e]) {
        this->field_6F = 0;
    }
    MoveTabPage(0);
    RebuildGridCat();
    playUISound(0x70);
}

// Decrement a list counter with wrap.
void CItemBoxGrid::DecListIndex() {
    u8* p = (u8*)this;
    if (((CSortMenu*)(p + 0xe8))->IsOpen()) return;
    if (p[0x528]) return;
    u8 idx = this->field_6F - 1;
    this->field_6F = idx;
    if ((s8)idx < 0) {
        this->field_6F = p[0x6e] - 1;
    }
    MoveTabPage(1);
    RebuildGridCat();
    playUISound(0x70);
}

// Item box advance handler. -O4,s reproduces retail's stmw r29
// prologue and 0x50 frame; the vt+0x10 dispatches use CBaseCur::setRootPaneTranslate
// cast class so MWCC emits lwz r12,0x10(r12); mtctr; bctrl.
void CItemBoxGrid::GridMoveCursorLeft() {
    u8* p = (u8*)this;
    if (p[0x542]) return;
    if (reinterpret_cast<CSysWin*>(p + 0x4ac)->isOpened()) {
        if (reinterpret_cast<CSysWin*>(p + 0x4ac)->isActive()) {
            p[0x540] -= 1;
            if ((s8)p[0x540] < 0) p[0x540] = 1;
            UpdateCursors();
            playUISound(1);
        }
        return;
    }
    if (reinterpret_cast<CSysWin*>(p + 0x4e8)->isOpened()) return;
    if (((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_IsVisibleFlag()) {
        if (((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_IsOpenedFlag()) {
            ((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_MoveSelectionPrev();
            u8 temp[12];
            GridSubMenu_GetCursorPos(temp, (CItemBoxGridSubMenu*)(p + 0x418));
            ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate((const nw4r::math::VEC3*)(temp));
            playUISound(1);
        }
        return;
    }
    if (isPresentActive(reinterpret_cast<CPresentWin*>(p + 0x468))) return;
    // Negated so the p544/p528/p527 chain nests in the false arm (retail
    // branches forward past the D3328 block instead of jumping to a shared end).
    if (!((CSortMenu*)(p + 0xe8))->IsOpen()) {
    if (p[0x544]) {
        void* arr = p + 0x288;
        // Retail scans candidate indices downward from p546-1 (wrapping to 3)
        // until one matches the starting value; the array index is
        // (s8)p545 * 4 + candidate for both probes and the final copy.
        s8 idx = (s8)p[0x546] - 1;
        while (idx != (s8)p[0x546]) {
            if (idx < 0) idx = 3;
            if (ArrayGet12((const unsigned short*)arr, (u8)((s8)p[0x545] * 4 + idx))) {
                p[0x546] = (u8)idx;
                break;
            }
            idx--;
        }
        u32 tmp[3];
        CopyTabSlotVec(tmp, arr, (u8)((s8)p[0x545] * 4 + (u8)p[0x546]));
        ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate((const nw4r::math::VEC3*)(tmp));
        goto sound_and_return;
    }
    if (p[0x528]) {
        if (p[0x527] == 1) {
            if (reinterpret_cast<CNumSelect*>(p + 0x3e4)->IsIdle()) {
                u8* sub = p + 0x54c;
                u8 entry = (u8)(p[0x525] * 10 + p[0x524]);
                u32 kind = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellKind((u16)entry);
                BdatGetItemType(kind & 0xFFFF);
                s32 count = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->CheckCellState(entry);
                if ((s8)count > 0) {
                    p[0x529] += 1;
                    if ((s8)p[0x529] > (s8)count) p[0x529] = 1;
                }
                reinterpret_cast<CNumSelect*>(p + 0x3e4)->CountUp();
                reinterpret_cast<CNumSelect*>(p + 0x3e4)->SetCount((u32)p[0x529]);
                u32 cost = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellSellPrice(entry);
                u32 total = (s8)p[0x529] * cost;
                reinterpret_cast<CNumSelect*>(p + 0x3e4)->SetTotal(total);
                goto sound_and_return;
            }
            return;
        } else if (p[0x527] == 2) {
            if (((CExchangeWin*)(p + 0x440))->getField27()) {
                p[0x529] -= 1;
                if ((s8)p[0x529] < 0) p[0x529] = 1;
                u8 temp[12];
                ExWin_CopyPanePair(temp, &mExchangeWin, p[0x529]);
                ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate((const nw4r::math::VEC3*)(temp));
                goto sound_and_return;
            }
            return;
        }
        goto sound_and_return;
    }
    if (p[0x527] == 4) {
        p[0x525] -= 1;
        if ((s8)p[0x525] < -2) {
            p[0x525] = (u8)(((CItemBoxGridFull*)((char*)this))->LookupIndexedByte() - 1);
        } else if ((s8)p[0x525] < 0) {
            *(s8*)&p[0x525] = -2;
        }
        UpdateCursors();
        RefreshSelectedCellText();
        goto sound_and_return;
    }
    {
        p[0x525] -= 1;
        if ((s8)p[0x525] < -1) {
            p[0x525] = (u8)(((CItemBoxGridFull*)((char*)this))->LookupIndexedByte() - 1);
        }
        UpdateCursors();
        RefreshSelectedCellText();
    }
    } else {
        if (((CSortMenu*)(p + 0xe8))->IsIdle()) {
            ((CSortMenu*)(p + 0xe8))->CursorUp();
            nw4r::math::VEC3 pos = ((CSortMenu*)(p + 0xe8))->GetCursorPos();
            ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate(&pos);
            goto sound_and_return;
        }
        return;
    }
sound_and_return:
    playUISound(1);
}

unsigned short ArrayGet12(const unsigned short* p, unsigned char i) {
    if (i < 12) {
        return p[i];
    }
    return 0;
}

// Open item: entry-copy base/value register clash. Retail keeps the base in r5
// (value loads into freed r4); MWCC always bases in r4 (value in r5), same
// ABI-boundary r4<->r5 class as reslistCount. Decl-order and word-indexed
// variants invariant; witness rejects (no bijection in [3,11)).
// Open item: entry-copy base/value register clash. Retail keeps the base in r5
// (value loads into freed r4); MWCC always bases in r4 (value in r5), same
// ABI-boundary r4<->r5 class as reslistCount. Decl-order and word-indexed
// variants invariant; witness rejects (no bijection in [3,11)).
void CopyTabSlotVec(u32* dst, void* src, u32 idx) {
    if (idx >= 12) return;
    u32 v24, v28;
    CibgTabSlot12* slots = (CibgTabSlot12*)src;
    u32* e = (u32*)&slots[idx];
    v24 = e[6];
    v28 = e[7];
    dst[1] = v28;
    dst[0] = v24;
    dst[2] = e[8];
}

// Right navigation. -O4,s: retail saves r29-r31 as one stmw
// block (-0x50 frame); sub/entry stay live across calls so MWCC colors them
// into the saved regs. Stack buffers mirror retail slots: 0x08/0x14/0x20 are
// 12-byte payloads, 0x2c is a 24-byte buffer filled by GridSubMenu_GetCursorPos. The
// do/while(0) gives every sound path one shared tail (retail branches to a
// single li r3,1/bl site) without goto-forced spills.
void CItemBoxGrid::GridMoveCursorRight() {
    u8* sub;
    u32 entry;
    u8* p = (u8*)this;
    if (p[0x542]) return;
    if (reinterpret_cast<CSysWin*>(p + 0x4ac)->isOpened()) {
        if (!reinterpret_cast<CSysWin*>(p + 0x4ac)->isActive()) return;
        // blink counter wraps to 0 after reaching 1
        u8 val = p[0x540] + 1;
        p[0x540] = val;
        if ((s8)val > 1) p[0x540] = 0;
        UpdateCursors();
        playUISound(1);
        return;
    }
    if (reinterpret_cast<CSysWin*>(p + 0x4e8)->isOpened()) return;
    if (((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_IsVisibleFlag()) {
        if (((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_IsOpenedFlag()) {
            ((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_MoveSelectionNext();
            u32 buf2c[3];
            GridSubMenu_GetCursorPos(buf2c, (CItemBoxGridSubMenu*)(p + 0x418));
            ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate((const nw4r::math::VEC3*)(buf2c));
            playUISound(1);
        }
        return;
    }
    if (isPresentActive(reinterpret_cast<CPresentWin*>(p + 0x468))) return;
    if (((CSortMenu*)(p + 0xe8))->IsOpen()) {
        if (((CSortMenu*)(p + 0xe8))->IsIdle()) {
            ((CSortMenu*)(p + 0xe8))->CursorDown();
            nw4r::math::VEC3 pos = ((CSortMenu*)(p + 0xe8))->GetCursorPos();
            ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate(&pos);
            goto sound_and_return2;
        }
        return;
    }
    if (p[0x544]) {
            // Tab scan: walk right from the current column, wrapping every
            // 4, until a non-empty slot is found or we come back to start.
            // idx assigned before sub so MWCC colors idx=r29, sub=r30.
            s8 idx = p[0x546] + 1;
            u8* sub = p + 0x288;
            do {
                if (idx >= 4) idx = 0;
                if (ArrayGet12((const unsigned short*)sub, (u8)(idx + (s8)p[0x545] * 4))) {
                    p[0x546] = (u8)idx;
                    break;
                }
                idx++;
            } while (idx != (s8)p[0x546]);
            u32 buf14[3];
            CopyTabSlotVec(buf14, sub, (u32)(u8)((s8)p[0x545] * 4 + p[0x546]));
            ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate((const nw4r::math::VEC3*)(buf14));
            goto sound_and_return2;
        }
        if (p[0x528]) {
            if (p[0x527] == 1) {
                if (!reinterpret_cast<CNumSelect*>(p + 0x3e4)->IsIdle()) return;
                sub = p + 0x54c;
                entry = (u8)(p[0x525] * 10 + p[0x524]);
                BdatGetItemType(((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellKind((u16)entry) & 0xFFFF);
                s32 count = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->CheckCellState((u16)entry);
                if ((s8)count > 0) {
                    u8 val = p[0x529] - 1;
                    p[0x529] = val;
                    if ((s8)val < 1) p[0x529] = (u8)count;
                }
                reinterpret_cast<CNumSelect*>(p + 0x3e4)->CountDown();
                reinterpret_cast<CNumSelect*>(p + 0x3e4)->SetCount(p[0x529]);
                reinterpret_cast<CNumSelect*>(p + 0x3e4)->SetTotal(((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellSellPrice((u16)entry) * (s8)p[0x529]);
                goto sound_and_return2;
            } else if (p[0x527] == 2) {
                if (!((CExchangeWin*)(p + 0x440))->getField27()) return;
                u8 val = p[0x529] + 1;
                p[0x529] = val;
                if ((s8)val > 1) p[0x529] = 0;
                u32 buf08[3];
                ExWin_CopyPanePair(buf08, &mExchangeWin, p[0x529]);
                ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate((const nw4r::math::VEC3*)(buf08));
                goto sound_and_return2;
            }
            goto sound_and_return2;
        }
        if (p[0x527] == 4) {
            // Cursor left with wraparound through the -2 sentinel
            u8 val = p[0x525] + 1;
            p[0x525] = val;
            if ((s8)val == -1) {
                p[0x525] = 0;
            } else {
                u8 max = (u8)mGrid.LookupIndexedByte();
                if ((s8)p[0x525] >= max) *(s8*)&p[0x525] = -2;
            }
            UpdateCursors();
            RefreshSelectedCellText();
            goto sound_and_return2;
        }
        {
            p[0x525] = p[0x525] + 1;
            u8 max = (u8)mGrid.LookupIndexedByte();
            if ((s8)p[0x525] >= max) *(s8*)&p[0x525] = -1;
            UpdateCursors();
            RefreshSelectedCellText();
        }
sound_and_return2:
    playUISound(1);
}

// Down navigation. -O4,s: retail saves r29-r31 as one stmw block
// (-0x30 frame); sub (r29) and entry (r30) are declared at function scope so
// their live ranges span the calls and MWCC colors them to the saved regs.
void CItemBoxGrid::GridMoveCursorDown() {
    // sub-array pointer and grid index stay live across calls -> MWCC colors
    // them into the stmw-saved r29/r30; buf/tmp mirror retail's stack slots.
    u32 entry;
    u8* sub;
    u8* p = (u8*)this;
    if (p[0x542]) return;
    if (reinterpret_cast<CSysWin*>(p + 0x4ac)->isOpened()) return;
    if (reinterpret_cast<CSysWin*>(p + 0x4e8)->isOpened()) return;
    if (((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_IsVisibleFlag()) return;
    if (isPresentActive(reinterpret_cast<CPresentWin*>(p + 0x468))) {
        if (!isPresentSettled(reinterpret_cast<CPresentWin*>(p + 0x468))) return;
        if (isPresentBusy(reinterpret_cast<CPresentWin*>(p + 0x468))) return;
        dispatchCounterStepBack(reinterpret_cast<CPresentWin*>(p + 0x468));
        playUISound(1);
        return;
    }
    if (p[0x528]) {
        if (p[0x527] != 1) return;
        if (!reinterpret_cast<CNumSelect*>(p + 0x3e4)->IsIdle()) return;
        sub = p + 0x54c;
        entry = (u8)(p[0x524] + p[0x525] * 10);
        BdatGetItemType(((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellKind((u16)entry) & 0xFFFF);
        s32 chk = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->CheckCellState(entry);
        if ((s8)chk > 0) {
            // stack-count step of 10, clamped to a minimum of 1
            u8 cur = p[0x529];
            int cnt = cur - 10;
            p[0x529] = (u8)cnt;
            if ((s8)(u8)cnt < 1) p[0x529] = 1;
        }
        reinterpret_cast<CNumSelect*>(p + 0x3e4)->SetCount(p[0x529]);
        u32 v = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellSellPrice(entry);
        s32 total = (s8)p[0x529] * v;
        reinterpret_cast<CNumSelect*>(p + 0x3e4)->SetTotal(total);
        playUISound(1);
        return;
    }
    if (((CSortMenu*)(p + 0xe8))->IsOpen()) {
        if (!((CSortMenu*)(p + 0xe8))->IsIdle()) return;
        ((CSortMenu*)(p + 0xe8))->PageUp();
        nw4r::math::VEC3 pos = ((CSortMenu*)(p + 0xe8))->GetCursorPos();
        ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate(&pos);
        playUISound(1);
        return;
    }
    if (p[0x544]) {
        s8 idx;
        sub = p + 0x288;
        // scan backwards for a non-empty tab slot, wrapping at 2
        for (idx = p[0x545] - 1; idx != p[0x545]; idx--) {
            if (idx < 0) idx = 2;
            if (ArrayGet12((const u16*)sub, (u8)(p[0x546] + idx * 4))) {
                p[0x545] = (u8)idx;
                break;
            }
        }
        u32 sbuf[3];
        CopyTabSlotVec(sbuf, sub, (u8)(p[0x546] + (s8)p[0x545] * 4));
        ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate((const nw4r::math::VEC3*)(sbuf));
        playUISound(1);
        return;
    }
    s8 row = (s8)p[0x525];
    if (row == -2) return;
    if (row == -1) {
        DecListIndex();
        UpdateCursors();
        return;
    }
    if ((s8)p[0x524] == 0) {
        p[0x524] = 9;
        PrevGridPage();
        UpdateCursors();
    } else {
        s8 col = (s8)p[0x524] - 1;
        p[0x524] = (u8)col;
        if ((s8)p[0x524] < 0) p[0x524] = 9;
        UpdateCursors();
        RefreshSelectedCellText();
    }
    playUISound(1);
}

// Up navigation. Same shape as GridMoveCursorDown (see notes there).
void CItemBoxGrid::GridMoveCursorUp() {
    // sub-array pointer and grid index stay live across calls -> MWCC colors
    // them into the stmw-saved r29/r30; buf/tmp mirror retail's stack slots.
    u32 entry;
    u8* sub;
    u8* p = (u8*)this;
    if (p[0x542]) return;
    if (reinterpret_cast<CSysWin*>(p + 0x4ac)->isOpened()) return;
    if (reinterpret_cast<CSysWin*>(p + 0x4e8)->isOpened()) return;
    if (((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_IsVisibleFlag()) return;
    if (isPresentActive(reinterpret_cast<CPresentWin*>(p + 0x468))) {
        if (!isPresentSettled(reinterpret_cast<CPresentWin*>(p + 0x468))) return;
        if (isPresentBusy(reinterpret_cast<CPresentWin*>(p + 0x468))) return;
        stepPresentCounter(reinterpret_cast<CPresentWin*>(p + 0x468));
        playUISound(1);
        return;
    }
    if (p[0x528]) {
        if (p[0x527] != 1u) return;
        if (!reinterpret_cast<CNumSelect*>(p + 0x3e4)->IsIdle()) return;
        entry = (u8)(p[0x524] + p[0x525] * 10);
        sub = p + 0x54c;
        BdatGetItemType(((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellKind((u16)entry) & 0xFFFF);
        s32 chk = (s8)((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->CheckCellState(entry);
        if (chk > 0) {
            // stack-count step of 10, clamped to the item's max stack
            u8 cur = p[0x529];
            int cnt = cur + 10;
            p[0x529] = (u8)cnt;
            if ((s8)(u8)cnt > chk) p[0x529] = (u8)chk;
        }
        reinterpret_cast<CNumSelect*>(p + 0x3e4)->SetCount(p[0x529]);
        s32 total = (s8)p[0x529] * ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellSellPrice(entry);
        reinterpret_cast<CNumSelect*>(p + 0x3e4)->SetTotal(total);
        playUISound(1);
        return;
    }
    if (((CSortMenu*)(p + 0xe8))->IsOpen()) {
        if (!((CSortMenu*)(p + 0xe8))->IsIdle()) return;
        ((CSortMenu*)(p + 0xe8))->PageDown();
        nw4r::math::VEC3 pos = ((CSortMenu*)(p + 0xe8))->GetCursorPos();
        ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate(&pos);
        playUISound(1);
        return;
    }
    if (p[0x544]) {
        // idx born before sub so MWCC colors them like retail (idx=r30, sub=r29)
        s8 idx = (s8)p[0x545] + 1;
        sub = p + 0x288;
        // scan forwards for a non-empty tab slot, wrapping at 3; retail
        // reloads p[0x545] for the wrap bound every iteration
        for (; idx != (s8)p[0x545]; idx++) {
            if (idx >= 3) idx = 0;
            u16 val = ArrayGet12((const u16*)sub, (u8)(p[0x546] + (u8)idx * 4));
            if (val) { p[0x545] = (u8)idx; break; }
        }
        u32 buf[3];
        CopyTabSlotVec(buf, sub, (u8)(p[0x546] + (u8)p[0x545] * 4));
        ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate((const nw4r::math::VEC3*)buf);
        playUISound(1);
        return;
    }
    s8 row = (s8)p[0x525];
    if (row == -2) return;
    if (row == -1) {
        IncListIndex();
        UpdateCursors();
        return;
    }
    if ((s8)p[0x524] == 9) {
        p[0x524] = 0;
        NextGridPage();
        UpdateCursors();
    } else {
        p[0x524]++;
        if ((s8)p[0x524] >= 10) p[0x524] = 0;
        UpdateCursors();
        RefreshSelectedCellText();
    }
    playUISound(1);
}

// Grid state update with conditions.
// Grid state update.
// -O4,s: retail uses stmw r30/lmw r30; sub (r31) is declared
// before p (r30) so MWCC colors it to the higher saved register.
void CItemBoxGrid::NextGridPage() {
    u8* sub;
    u8* p = (u8*)this;
    if (((CSortMenu*)(p + 0xe8))->IsOpen()) return;
    if (p[0x528]) return;
    if (reinterpret_cast<CSysWin*>(p + 0x4ac)->isOpened()) return;
    if (reinterpret_cast<CSysWin*>(p + 0x4e8)->isOpened()) return;
    if (((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_IsVisibleFlag()) return;
    if (isPresentActive(reinterpret_cast<CPresentWin*>(p + 0x468))) return;
    sub = p + 0x54c;
    ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->IncGridPageIndex();
    // Retail calls LookupIndexedByte twice: once for the compare, once for
    // the store (subi from the fresh call result).
    // retail branches over the store when a < b (bc 12,0), so negate
    if (!((s8)p[0x525] < (u8)((CItemBoxGridFull*)((char*)sub))->LookupIndexedByte()))
        p[0x525] = (u8)(((CItemBoxGridFull*)((char*)sub))->LookupIndexedByte() - 1);
    RebuildGridUI();
    RefreshSelectedCellText();
    u8 rows = sub[0x2803];
    u8 f = rows == 0 ? 1 : rows;
    if (f != 1) playUISound(0xa);
}

// Grid state update (decrement variant).
void CItemBoxGrid::PrevGridPage() {
    u8* sub;
    u8* p = (u8*)this;
    if (((CSortMenu*)(p + 0xe8))->IsOpen()) return;
    if (p[0x528]) return;
    if (reinterpret_cast<CSysWin*>(p + 0x4ac)->isOpened()) return;
    if (reinterpret_cast<CSysWin*>(p + 0x4e8)->isOpened()) return;
    if (((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_IsVisibleFlag()) return;
    if (isPresentActive(reinterpret_cast<CPresentWin*>(p + 0x468))) return;
    sub = p + 0x54c;
    ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->DecGridPageIndex();
    // retail branches over the store when a < b (bc 12,0), so negate
    if (!((s8)p[0x525] < (u8)((CItemBoxGridFull*)((char*)sub))->LookupIndexedByte()))
        p[0x525] = (u8)(((CItemBoxGridFull*)((char*)sub))->LookupIndexedByte() - 1);
    RebuildGridUI();
    RefreshSelectedCellText();
    u8 rows = sub[0x2803];
    u8 f = rows == 0 ? 1 : rows;
    if (f != 1) playUISound(0xa);
}

// Guard chain shares a single exit label in retail (every check branches to
// the epilogue), so express the checks as one && chain.
void CItemBoxGrid::OpenSortMenu() {
    u8* p = (u8*)this;
    if (!this->field_542 && !this->field_528 && !reinterpret_cast<CSysWin*>(p + 0x4ac)->isOpened() &&
        !reinterpret_cast<CSysWin*>(p + 0x4e8)->isOpened() && !((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_IsVisibleFlag() &&
        !isPresentActive(reinterpret_cast<CPresentWin*>(p + 0x468)) && !this->field_544) {
        if (((CSortMenu*)(p + 0xe8))->IsOpen()) {
            if (!((CSortMenu*)(p + 0xe8))->IsIdle()) return;
            UpdateCursors();
            ((CBaseCur*)(p + 0xa0))->SetVisible(0);
            ((CSortMenu*)(p + 0xe8))->Close();
            playUISound(6);
            return;
        }
        // state word is compared signed (retail cmpwi)
        if ((s32)this->field_58 != 3) return;
        if (!((CSortMenu*)(p + 0xe8))->IsIdle()) return;
        // fetch the layout pane and resolve two tag strings through its
        // vtable+0x3C accessor, then merge them back into the window text.
        nw4r::lyt::Layout* obj = this->field_44;
        const char* strs = lbl_eu_8050566C;
        nw4r::lyt::Pane* pane = obj->GetRootPane();
        // MWCC evaluates args right-to-left, so listing the 0x301 fetch as
        // the third argument makes it execute first, matching retail.
        nw4r::math::VEC3 basePos;
        ScalePaneWorldPos(&basePos, pane->FindPaneByName((char*)&lbl_eu_8050566C[0x2f8], 1),
                      pane->FindPaneByName((char*)&lbl_eu_8050566C[0x301], 1), pane);
        ((CSortMenu*)(p + 0xe8))->SetPos(&basePos);
        ((CSortMenu*)(p + 0xe8))->SetSelect((u8)(this->field_547 + this->field_548));
        nw4r::math::VEC3 pos = ((CSortMenu*)(p + 0xe8))->GetCursorPos();
        ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate(&pos);
        ((CBaseCur*)(p + 0xa0))->SetVisible(1);
        ((CBaseCur*)(p + 0x70))->SetVisible(0);
        ((CBaseCur*)(p + 0xb8))->SetVisible(0);
        ((CSortMenu*)(p + 0xe8))->Open();
        this->field_526 = 0;
        playUISound(2);
    }
}

// -O4,s: retail keeps the stmw r30 / lmw r30 block save.
// Mirrors retail's label structure: guard failures branch to the bare exit,
// handled arms fall into the single shared r4-check tail (keeps MWCC from
// duplicating the tail per return path).
void CItemBoxGrid::HandleCancelBtn(int confirmed) {
    u8* p = (u8*)this;
    if (reinterpret_cast<CSysWin*>(p + 0x4ac)->isOpened()) {
        if (!reinterpret_cast<CSysWin*>(p + 0x4ac)->isActive()) goto exit;
        reinterpret_cast<CSysWin*>(p + 0x4ac)->advancePhase3();
        ((CBaseCur*)(p + 0xa0))->SetVisible(0);
        this->field_540 = 1;
        this->field_58 = 0x17;
        goto exit;
    }
    if (reinterpret_cast<CSysWin*>(p + 0x4e8)->isOpened()) {
        if (!reinterpret_cast<CSysWin*>(p + 0x4e8)->isActive()) goto exit;
        reinterpret_cast<CSysWin*>(p + 0x4e8)->advancePhase3();
        ((CBaseCur*)(p + 0x70))->SetVisible(0);
        this->field_58 = 0x14;
        goto exit;
    }
    if (((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_IsVisibleFlag()) {
        if (!((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_IsOpenedFlag()) goto exit;
        UpdateCursors();
        ((CBaseCur*)(p + 0x70))->SetVisible(1);
        ((CBaseCur*)(p + 0xa0))->SetVisible(0);
        ((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_ClearSelection();
        ((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_AdvanceState2To3();
        this->field_58 = 8;
        this->field_528 = 0;
        // retail inlines the r4-check tail here (copy 1 of 3)
        if (!confirmed) playUISound(6);
        goto exit;
    }
    if (isPresentActive(reinterpret_cast<CPresentWin*>(p + 0x468))) {
        if (!isPresentSettled(reinterpret_cast<CPresentWin*>(p + 0x468))) goto exit;
        if (isPresentBusy(reinterpret_cast<CPresentWin*>(p + 0x468))) {
            advancePresentState(reinterpret_cast<CPresentWin*>(p + 0x468));
            this->field_58 = 0x11;
            UpdateCursors();
            ((CBaseCur*)(p + 0x70))->SetVisible(1);
            ((CBaseCur*)(p + 0xd0))->SetVisible(0);
        } else if (getPresentPath(reinterpret_cast<CPresentWin*>(p + 0x468))) {
            togglePresentPath(reinterpret_cast<CPresentWin*>(p + 0x468));
            u32 buf[4];
            getPresentPaneWorldPos((nw4r::math::VEC3*)buf, reinterpret_cast<CPresentWin*>(p + 0x468));
            ((CBaseCur*)(p + 0xd0))->setRootPaneTranslate((const nw4r::math::VEC3*)(buf));
        } else {
            advancePresentState(reinterpret_cast<CPresentWin*>(p + 0x468));
            this->field_58 = 0x11;
            UpdateCursors();
            ((CBaseCur*)(p + 0x70))->SetVisible(1);
            ((CBaseCur*)(p + 0xd0))->SetVisible(0);
        }
        // tail copy 2
        if (!confirmed) playUISound(6);
        goto exit;
    }
    if (this->field_528) {
        if (this->field_527 == 1) {
            if (!reinterpret_cast<CNumSelect*>(p + 0x3e4)->IsIdle()) goto exit;
            if ((s32)this->field_58 != 0x19) goto exit;
            reinterpret_cast<CNumSelect*>(p + 0x3e4)->Close();
            this->field_58 = 0x1a;
        } else if (this->field_527 == 2) {
            if (!((CExchangeWin*)(p + 0x440))->getField27()) goto exit;
            ((CExchangeWin*)(p + 0x440))->ExWin_RequestClose();
            this->field_58 = 0x0e;
            ((CBaseCur*)(p + 0x70))->SetVisible(1);
            ((CBaseCur*)(p + 0xa0))->SetVisible(0);
            UpdateCursors();
        }
        goto check_tail;
    }
    if (this->field_544) {
        this->field_544 = 0;
        ((CBaseCur*)(p + 0x70))->SetVisible(1);
        ((CBaseCur*)(p + 0xa0))->SetVisible(0);
        goto check_tail;
    }
    if (((CSortMenu*)(p + 0xe8))->IsOpen()) {
        if (!((CSortMenu*)(p + 0xe8))->IsIdle()) goto exit;
        UpdateCursors();
        ((CBaseCur*)(p + 0xa0))->SetVisible(0);
        ((CSortMenu*)(p + 0xe8))->Close();
    } else if (this->field_527 == 4) {
        this->field_525 = -2;
        UpdateCursors();
    }
check_tail:
    // tail copy 3 (shared by the p[0x528]/p[0x544]/final arms)
    if (!confirmed) playUISound(6);
exit:;
}



// A-confirm / window-result handler. -O4,s reproduces retail's
// stmw r25 / 96-byte frame; entry index is the usual col + row*10 cell.
void CItemBoxGrid::func_801CCAF0() {
    void* item;
    CItemBoxGridFull* sub;
    u8 entry;
    u8* p = (u8*)this;
    if (reinterpret_cast<CSysWin*>(p + 0x4ac)->isOpened()) {
        if (reinterpret_cast<CSysWin*>(p + 0x4ac)->isActive()) {
            reinterpret_cast<CSysWin*>(p + 0x4ac)->advancePhase3();
            ((CBaseCur*)(p + 0xa0))->SetVisible(0);
            *(u32*)(p + 0x58) = 0x17;
        }
        return;
    }
    if (reinterpret_cast<CSysWin*>(p + 0x4e8)->isOpened()) {
        if (reinterpret_cast<CSysWin*>(p + 0x4e8)->isActive()) {
            reinterpret_cast<CSysWin*>(p + 0x4e8)->advancePhase3();
            ((CBaseCur*)(p + 0x70))->SetVisible(0);
            *(u32*)(p + 0x58) = 0x14;
        }
        return;
    }
    if (((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_IsVisibleFlag()) {
        if (!((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_IsOpenedFlag()) return;
        if ((GridSubMenu_GetOptionCount(p + 0x418) & 0xFF) == 1) {
            sub = (CItemBoxGridFull*)(p + 0x54c);
            entry = (u8)(p[0x524] + p[0x525] * 10);
            item = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellItemPtr((u16)entry);
            u32 type = (*(u32*)item >> 16) & 0xF;
            if (type == 2 || (u8)(type + 0xfc) <= 4) {
                u8 nslots = (u8)CItem_initItemImplInstances((CItemData*)item)->vf30((CItemData*)item);
                for (u8 j = 0; j < nslots; j++) {
                    if ((s16)CItem_initItemImplInstances((CItemData*)item)->vf40((CItemData*)item, j) != -1) {
                        p[0x541] = 1;
                        break;
                    }
                }
            }
            if (item) {
                CItem_initItemImplInstances((CItemData*)item)->vf10((CItemData*)item);
            }
            RebuildGridCat();
            playUISound(0x7a);
            UpdateCursors();
        } else {
            u8* listRow = p + (s8)this->field_6F;
            if (listRow[0x62] == 0xa) {
                s32 count2 = 0;
                void* ec = CtrlObjectParam_GetSlotTableBase();
                u32* ptr = (u32*)((u8*)ec + 4);
                for (u8 k = 0; k < 7; k++) {
                    u32 val;
                    if (k < 3) {
                        val = *(u32*)((u8*)ec + 4 + (k << 2));
                    } else {
                        val = ptr[k];
                    }
                    if ((s32)val > 0) {
                        void* obj = (void*)CtrlObjectParam_GetCharEntry(val & 0xFFFF);
                        if (((CItemBoxCharEntry*)obj)->mParam.unk15F0 != 1) count2++;
                    }
                }
                if (count2 <= 1) {
                    playUISound(5);
                    return;
                }
            }
            playUISound(3);
        }
        ((CBaseCur*)(p + 0xa0))->SetVisible(0);
        ((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_AdvanceState2To3();
        *(u32*)(p + 0x58) = 8;
        return;
    }
    if (isPresentActive(reinterpret_cast<CPresentWin*>(p + 0x468))) {
        if (!isPresentSettled(reinterpret_cast<CPresentWin*>(p + 0x468))) return;
        if (getPresentPath(reinterpret_cast<CPresentWin*>(p + 0x468))) {
            if (isPresentBusy(reinterpret_cast<CPresentWin*>(p + 0x468))) {
                advancePresentState(reinterpret_cast<CPresentWin*>(p + 0x468));
                *(u32*)(p + 0x58) = 0x11;
                UpdateCursors();
                ((CBaseCur*)(p + 0x70))->SetVisible(1);
                ((CBaseCur*)(p + 0xd0))->SetVisible(0);
            } else {
                sub = (CItemBoxGridFull*)(p + 0x54c);
                entry = (u8)(p[0x524] + p[0x525] * 10);
                item = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellItemPtr((u16)entry);
                u32 kind = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellKind((u16)entry);
                s32 count = ((CItemBoxGridFull*)(sub))->CheckCellState(entry);
                showPresentRank(p + 0x468);
                ((CBaseCur*)(p + 0xd0))->SetVisible(0);
                u32 teachType = getPresentRankA(p + 0x468);
                u32 teachState = getPresentRankB(p + 0x468);
                if ((u8)teachState == 1) {
                    u32 table= (u32)::cf::CfBdat::spItmCollectListFileData;
                    if ((BdatGetU8Direct((const void*)table, (const char*)"type",
                                       BdatGetItemId(kind & 0xFFFF) & 0xFFFF) & 0xFF) == 7) {
                        incrementEventCounter(0x87);
                    }
                } else if ((u8)teachState == 2) {
                    if ((*(u32*)item >> 20) == 0x75a) incrementEventCounter(0x88);
                } else if ((u8)teachState == 4) {
                    if ((*(u32*)item >> 20) == 0x775) incrementEventCounter(0x8a);
                } else if ((u8)teachState == 5) {
                    u32 table= (u32)::cf::CfBdat::spItmCollectListFileData;
                    if ((BdatGetU8Direct((const void*)table, (const char*)"type",
                                       BdatGetItemId(kind & 0xFFFF) & 0xFFFF) & 0xFF) == 2) {
                        incrementEventCounter(0x8b);
                    }
                }
                if ((u8)teachType == 3 || (u8)teachType == 8) {
                    if ((u8)teachState == 1) incrementEventCounter(0x89);
                } else if ((u8)teachType == 6) {
                    u32 table= (u32)::cf::CfBdat::spItmCollectListFileData;
                    if ((BdatGetU8Direct((const void*)table, (const char*)"type",
                                       BdatGetItemId(kind & 0xFFFF) & 0xFFFF) & 0xFF) == 5) {
                        incrementEventCounter(0x8d);
                    }
                } else if ((u8)teachType == 7) {
                    incrementEventCounter(0x8c);
                }
                if ((*(u32*)item >> 20) == 0x867) incrementEventCounter(0x8e);
                DispatchItemEvt(item, (s8)count - 1);
                RebuildGridCat();
            }
        } else {
            togglePresentPath(reinterpret_cast<CPresentWin*>(p + 0x468));
            u8 temp[16];
            getPresentPaneWorldPos((nw4r::math::VEC3*)temp, reinterpret_cast<CPresentWin*>(p + 0x468));
            ((CBaseCur*)(p + 0xd0))->setRootPaneTranslate((const nw4r::math::VEC3*)temp);
        }
        playUISound(3);
        return;
    }
    if (((CSortMenu*)(p + 0xe8))->IsOpen()) {
        if (((CSortMenu*)(p + 0xe8))->IsIdle()) {
            u32 val = ((CSortMenu*)(p + 0xe8))->GetSelect();
            CItemBlock_setFlag120EC((p + (s8)this->field_6F)[0x62], val & 0xFF);
            p[0x547] = (u8)((CSortMenu*)(p + 0xe8))->GetCursor();
            p[0x548] = (u8)((CSortMenu*)(p + 0xe8))->GetTop();
            ((CItemBoxGrid*)p)->HandleCancelBtn(1);
            p[0x524] = 0;
            if ((s8)p[0x525] > 0) p[0x525] = 0;
            UpdateCursors();
            p[0x2d50] = 0;
            RebuildGridCat();
            playUISound(3);
        }
        return;
    }
    if (p[0x544]) return;
    s8 cursor = (s8)p[0x525];
    if (cursor == -1) return;
    u8 mode = p[0x527];
    if (mode == 4) {
        if (cursor == -2) {
            p[0x549] = 1;
            ((CItemBoxGridFull*)((CItemBoxGridFull*)(p + 0x54c)))->InitGridItems();
            playUISound(4);
            return;
        }
        sub = (CItemBoxGridFull*)(p + 0x54c);
        entry = (u8)(p[0x524] + cursor * 10);
        u32 cap = ((CItemBoxGridFull*)(sub))->GetCategoryCap() & 0xFF;
        if ((((CItemBoxGridFull*)(sub))->CountMarkedCells() & 0xFF) >= cap) {
            if (sub->GetCellByte8(entry)) {
                ((CItemBoxGridFull*)(sub))->ToggleCellMark(entry);
                RebuildGridUI();
                RefreshSelectedCellText();
                func_801D0BD8();
                playUISound(6);
                return;
            }
            if (!(((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellKind((u16)entry) & 0xFFFF)) return;
            *(u32*)(p + 0x58) = 0x12;
            u32 msg = (u32)BdatTouchStringCell((const char*)"MNU_item", "name", 0xa1);
            reinterpret_cast<CSysWin*>(p + 0x4e8)->switchKindPane(0);
            reinterpret_cast<CSysWin*>(p + 0x4e8)->setKindContent(reinterpret_cast<char*>(msg), 0);
            reinterpret_cast<CSysWin*>(p + 0x4e8)->applyKind(1);
            reinterpret_cast<CSysWin*>(p + 0x4e8)->openPhase1();
            ((CBaseCur*)(p + 0x70))->SetVisible(0);
            playUISound(5);
            return;
        }
        if ((u8)((CItemBoxGridFull*)(sub))->GetCellFlag0(entry)) return;
        if (!(((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellKind((u16)entry) & 0xFFFF)) return;
        ((CItemBoxGridFull*)(sub))->ToggleCellMark(entry);
        RebuildGridUI();
        RefreshSelectedCellText();
        func_801D0BD8();
        if (sub->GetCellByte8(entry)) {
            playUISound(3);
        } else {
            playUISound(6);
        }
        return;
    }
    if (mode == 1) {
        if (p[0x528]) {
            if (!reinterpret_cast<CNumSelect*>(p + 0x3e4)->IsIdle()) return;
            if (*(s32*)(p + 0x58) != 0x19) return;
            sub = (CItemBoxGridFull*)(p + 0x54c);
            entry = (u8)(p[0x524] + p[0x525] * 10);
            item = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellItemPtr((u16)entry);
            ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellKind((u16)entry);
            s32 count = ((CItemBoxGridFull*)(sub))->CheckCellState(entry);
            u32 type = (*(u32*)item >> 16) & 0xF;
            if ((u8)((CItemBoxGridFull*)(sub))->GetCellFlag0(entry)) {
                s16 sid = ((CItemBoxGridFull*)(sub))->GetCellItemId(entry);
                UnequipPartyItem((int)type, (int)sid);
            }
            if (type == 2 || (u8)(type + 0xfc) <= 4) {
                u8 numSlots = (u8)CItem_initItemImplInstances((CItemData*)item)->vf30((CItemData*)item);
                for (u8 j = 0; j < numSlots; j++) {
                    if ((s16)CItem_initItemImplInstances((CItemData*)item)->vf40((CItemData*)item, j) != -1) {
                        p[0x541] = 1;
                        break;
                    }
                }
            }
            s32 add = (s8)p[0x529] * (s32)((CItemBoxGridFull*)(sub))->GetCellSellPrice(entry);
            CItemBlock_setCount((s32)((u32)CItemBlock_getPtr20E8() + add));
            DispatchItemEvt(item, (s8)count - (s8)p[0x529]);
            RebuildGridCat();
            reinterpret_cast<CNumSelect*>(p + 0x3e4)->Close();
            *(u32*)(p + 0x58) = 0x1a;
            playUISound(0x2f);
            return;
        }
        if (!reinterpret_cast<CNumSelect*>(p + 0x3e4)->IsIdle()) return;
        if (*(s32*)(p + 0x58) != 3) return;
        {
            sub = (CItemBoxGridFull*)(p + 0x54c);
            entry = (u8)((s8)p[0x524] + (s8)p[0x525] * 10);
            u32 kindSell = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellKind((u16)entry);
            if (!(kindSell & 0xFFFF)) return;
            p[0x529] = 1;
            reinterpret_cast<CNumSelect*>(p + 0x3e4)->SetItemName((char*)((CItemBoxGridFull*)(sub))->FormatCellName(entry));
            reinterpret_cast<CNumSelect*>(p + 0x3e4)->SetCount(p[0x529]);
            reinterpret_cast<CNumSelect*>(p + 0x3e4)->SetTotal((s8)p[0x529] * ((CItemBoxGridFull*)(sub))->GetCellSellPrice(entry));
            if (sub->GetCellFlag1(entry)) {
                *(u32*)(p + 0x58) = 0x12;
                u32 msg2 = (u32)BdatTouchStringCell((const char*)"MNU_shop", "name", 0x17);
                reinterpret_cast<CSysWin*>(p + 0x4e8)->switchKindPane(0);
                reinterpret_cast<CSysWin*>(p + 0x4e8)->setKindContent(reinterpret_cast<char*>(msg2), 0);
                reinterpret_cast<CSysWin*>(p + 0x4e8)->applyKind(1);
                reinterpret_cast<CSysWin*>(p + 0x4e8)->openPhase1();
                ((CBaseCur*)(p + 0x70))->SetVisible(0);
            } else if ((u8)((CItemBoxGridFull*)(sub))->GetCellFlag0(entry)) {
                void* item2 = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellItemPtr((u16)entry);
                if (((*(u32*)item2 >> 16) & 0xF) == 2) {
                    *(u32*)(p + 0x58) = 0x12;
                    u32 msg3 = (u32)BdatTouchStringCell((const char*)"MNU_shop", "name", 0x16);
                    reinterpret_cast<CSysWin*>(p + 0x4e8)->switchKindPane(0);
                    reinterpret_cast<CSysWin*>(p + 0x4e8)->setKindContent(reinterpret_cast<char*>(msg3), 0);
                    reinterpret_cast<CSysWin*>(p + 0x4e8)->applyKind(1);
                    reinterpret_cast<CSysWin*>(p + 0x4e8)->openPhase1();
                    ((CBaseCur*)(p + 0x70))->SetVisible(0);
                } else {
                    *(u32*)(p + 0x58) = 0x15;
                    u32 msg4 = (u32)BdatTouchStringCell((const char*)"MNU_shop", "name", 0x13);
                    u32 msg5 = (u32)BdatTouchStringCell((const char*)"MNU_shop", "name", 0x14);
                    u32 msg6 = (u32)BdatTouchStringCell((const char*)"MNU_shop", "name", 0x15);
                    reinterpret_cast<CSysWin*>(p + 0x4ac)->setKindContent(reinterpret_cast<char*>(msg4), 0);
                    reinterpret_cast<CSysWin*>(p + 0x4ac)->setTwoTextValues(reinterpret_cast<char*>(msg5), reinterpret_cast<char*>(msg6));
                    reinterpret_cast<CSysWin*>(p + 0x4ac)->applyKind(0);
                    reinterpret_cast<CSysWin*>(p + 0x4ac)->openPhase1();
                    ((CBaseCur*)(p + 0x70))->SetVisible(0);
                    p[0x540] = 1;
                }
            } else {
                u8 listVal2 = p[0x62 + (s8)this->field_6F];
                if (listVal2 == 0xd) {
                    u32 k12 = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellKind((u16)entry);
                    if (IsItemTeachable(k12 & 0xFFFF)) {
                        *(u32*)(p + 0x58) = 0x15;
                        u32 msg7 = (u32)BdatTouchStringCell((const char*)"MNU_shop", "name", 0x19);
                        u32 msg8 = (u32)BdatTouchStringCell((const char*)"MNU_shop", "name", 0x14);
                        u32 msg9 = (u32)BdatTouchStringCell((const char*)"MNU_shop", "name", 0x15);
                        reinterpret_cast<CSysWin*>(p + 0x4ac)->setKindContent(reinterpret_cast<char*>(msg7), 0);
                        reinterpret_cast<CSysWin*>(p + 0x4ac)->setTwoTextValues(reinterpret_cast<char*>(msg8), reinterpret_cast<char*>(msg9));
                        reinterpret_cast<CSysWin*>(p + 0x4ac)->applyKind(0);
                        reinterpret_cast<CSysWin*>(p + 0x4ac)->openPhase1();
                        ((CBaseCur*)(p + 0x70))->SetVisible(0);
                        p[0x540] = 1;
                    } else {
                        goto sell_qty;
                    }
                } else {
                sell_qty:
                    u32 lv = (p + (s8)this->field_6F)[0x62];
                    if (lv - 2 <= 7) {
                        reinterpret_cast<CNumSelect*>(p + 0x3e4)->SetArrowVisible(0);
                    } else {
                        reinterpret_cast<CNumSelect*>(p + 0x3e4)->SetArrowVisible(1);
                    }
                    reinterpret_cast<CNumSelect*>(p + 0x3e4)->Open();
                    *(u32*)(p + 0x58) = 0x18;
                    p[0x528] = 1;
                    ((CBaseCur*)(p + 0x70))->SetVisible(0);
                    playUISound(3);
                    return;
                }
            }
        }
    }
    if (mode == 2) {
        if (p[0x542]) return;
        if (p[0x52c]) return;
        if (p[0x528]) {
            if (!((CExchangeWin*)(p + 0x440))->getField27()) return;
            if (*(s32*)(p + 0x58) != 0xd) return;
            if ((s8)p[0x529] == 0) {
                sub = (CItemBoxGridFull*)(p + 0x54c);
                entry = (u8)(p[0x524] + p[0x525] * 10);
                item = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellItemPtr((u16)entry);
                u32 kind = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellKind((u16)entry);
                s32 count = ((CItemBoxGridFull*)(sub))->CheckCellState(entry);
                if (!(kind & 0xFFFF)) return;
                u32 v1 = ((CItemBoxGridFull*)(sub))->GetScaledItemValue(*(u16*)(p + 0x52e), 0, *(u16*)(p + 0x52a));
                u32 v2 = ((CItemBoxGridFull*)(sub))->GetScaledItemValue((u16)kind, item, *(u16*)(p + 0x52a));
                if (v1 > v2) return;
                u16 nVal = BdatGetU16Direct((void*)::cf::CfBdat::spFldNpcListFileData, (const char*)"present", *(u16*)(p + 0x52a));
                u32 test = ((CItemBoxGridFull*)(sub))->GetScaledItemValue(nVal, 0, 0);
                s8 leftover = (s8)count;
                s32 neg = -((s32)(v2 - v1) < (s32)test);
                p[0x52d] = (u8)neg;
                DispatchItemEvt(item, leftover - 1);
                p[0x52c] = 1;
                LookupItemTypeTable(*(u16*)(p + 0x52e));
                BdatGetItemType(*(u16*)(p + 0x52e));
                BdatGetItemId(*(u16*)(p + 0x52e));
                CItem_thunkAllocRecord(*(u16*)(p + 0x52e), 1);
                UIWin_BuildFlagBufRec(2, *(u16*)(p + 0x52e), 0);
                incrementEventCounter(0xbb);
                incrementEventCounter(0xbc);
                incrementEventCounter(0xbd);
                switch ((s32)*(u16*)(p + 0x52e)) {
                }
            }
            ((CExchangeWin*)(p + 0x440))->ExWin_RequestClose();
            *(u32*)(p + 0x58) = 0xe;
            ((CBaseCur*)(p + 0x70))->SetVisible(1);
            ((CBaseCur*)(p + 0xa0))->SetVisible(0);
            UpdateCursors();
            if ((s8)p[0x529] == 0) playUISound(0x31);
            else playUISound(3);
            return;
        }
        if (!((CExchangeWin*)(p + 0x440))->getField27()) return;
        if (*(s32*)(p + 0x58) != 3) return;
        {
            sub = (CItemBoxGridFull*)(p + 0x54c);
            u32 kind = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellKind((u16)(u8)(p[0x524] + p[0x525] * 10));
            if (!(kind & 0xFFFF)) return;
            char fmtBuf[0x28];
            sprintf(fmtBuf, (const char*)"%s", BuildItemNameStr(*(u16*)(p + 0x52e)));
            ((CExchangeWin*)(p + 0x440))->ExWin_SetTwoTexts(fmtBuf, (char*)BuildItemNameStr(kind & 0xFFFF));
            ((CExchangeWin*)(p + 0x440))->ExWin_RequestOpen();
            p[0x528] = 1;
            *(u32*)(p + 0x58) = 0xc;
            p[0x529] = 1;
            ((CBaseCur*)(p + 0x70))->SetVisible(0);
            playUISound(3);
            return;
        }
    }
    if (p[0x528]) return;
    if (!((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_IsOpenedFlag()) return;
    if (!isPresentSettled(reinterpret_cast<CPresentWin*>(p + 0x468))) return;
    {
        sub = (CItemBoxGridFull*)(p + 0x54c);
        entry = (u8)((s8)p[0x524] + (s8)p[0x525] * 10);
        if ((u8)((CItemBoxGridFull*)(sub))->GetCellFlag1(entry)) {
            playUISound(5);
            return;
        }
        if (!(((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellKind((u16)entry) & 0xFFFF)) {
            playUISound(5);
            return;
        }
        if ((u8)((CItemBoxGridFull*)(sub))->GetCellFlag0(entry)) {
            playUISound(5);
            return;
        }
        u8 listVal3 = p[0x62 + (s8)this->field_6F];
        if (listVal3 == 0xc) {
            playUISound(5);
            return;
        }
        u32 sendVal = 0xFF;
        if (p[0x527] != 3) sendVal = listVal3;
        ((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_SetupOptions(sendVal & 0xFF);
        ((CBaseCur*)(p + 0x70))->SetVisible(0);
        ((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_BeginOpenAnim();
        *(u32*)(p + 0x58) = 6;
        p[0x528] = 1;
        playUISound(3);
    }
}

// Store value and call helpers.
void CItemBoxGrid::SetInfoMsgId(u16 val) {
    field_52E = val;
    mInfo.SetNamedText(0, (char*)BuildItemNameStr((u16)val));
    RebuildGridCat();
}

// Get field depending on obj state.
int CItemBoxGrid::GetExchFlag52C() {
    if (!((CExchangeWin*)(((CibgOuterSlots*)this)->m440))->getField27()) return 0;
    s32 state = (s32)this->field_58;
    if (state != 3) return 0;
    return this->field_52C;
}

int CItemBoxGrid::GetField52D() { return field_52D; }

// Periodic grid tick: when the page-detail window (0x544) is active, either
// drive it (if the +0x4e8 win is up) or populate it from the current list
// selection; otherwise, when in state 3, scan for the first non-empty cell
// and page to it. -O4,s merges the r29-r31 saves into retail's
// _savegpr_29 prologue.
void CItemBoxGrid::TickItemBoxGrid() {
    u8* p = (u8*)this;
    if (p[0x528]) return;
    if (reinterpret_cast<CSysWin*>(p + 0x4ac)->isOpened()) return;
    if (((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_IsVisibleFlag()) return;
    if (isPresentActive(reinterpret_cast<CPresentWin*>(p + 0x468))) return;
    if (((CSortMenu*)(p + 0xe8))->IsOpen()) return;
    if (p[0x544]) {
        if (reinterpret_cast<CSysWin*>(p + 0x4e8)->isOpened()) {
            if (!reinterpret_cast<CSysWin*>(p + 0x4e8)->isActive()) return;
            reinterpret_cast<CSysWin*>(p + 0x4e8)->advancePhase3();
            ((CBaseCur*)(p + 0x70))->SetVisible(0);
            *(u32*)(p + 0x58) = 0x14;
            return;
        }
        u16* list = (u16*)(p + 0x288);
        u8 idx = (u8)(p[0x546] + (s8)p[0x545] * 4);
        u16 val = ArrayGet12(list, idx);
        if (!val) return;
        *(u32*)(p + 0x58) = 0x12;
        u8 bt = (idx < 12) ? p[0x288 + idx + 0xa8] : 0;
        void* obj;
        if (bt == 3) {
            obj = (void*)((CItemBoxPage*)(list))->GetItemName(idx);
        } else {
            obj = (void*)BdatGetPtrDirect((const void*)*(u32*)(itemBoxGrid_shopItemIdTable), "help", val & 0xFFFF);
        }
        reinterpret_cast<CSysWin*>(p + 0x4e8)->switchKindPane(0);
        reinterpret_cast<CSysWin*>(p + 0x4e8)->setKindContent(reinterpret_cast<char*>((u32)obj), 0);
        reinterpret_cast<CSysWin*>(p + 0x4e8)->applyKind(1);
        reinterpret_cast<CSysWin*>(p + 0x4e8)->openPhase1();
        ((CBaseCur*)(p + 0xa0))->SetVisible(0);
        return;
    }
    if (*(u32*)(p + 0x58) != 3) return;
    if (reinterpret_cast<CSysWin*>(p + 0x4e8)->isOpened()) return;
    u16* list = (u16*)(p + 0x288);
    int i;
    u8 found = 0;
    for (i = 0; i < 12; i++) {
        if (list[i] != 0) {
            p[0x288 + 0xd8] = (u8)i;
            if (list[8] != 0) p[0x288 + 0xd8] = 8;
            found = 1;
            break;
        }
    }
    if (found) {
        u8 v = p[0x288 + 0xd8];
        p[0x544] = 1;
        // Page = v / 4 rounded to nearest via the 0x67F74 scale; the
        // remainder selects the cell within the page (both stored as bytes).
        int n = (int)(0.25f * (float)v);
        p[0x545] = (u8)n;
        p[0x546] = (u8)(v - (s8)(u8)n * 4);
        ((CBaseCur*)(p + 0x70))->SetVisible(0);
        ((CBaseCur*)(p + 0xa0))->SetVisible(1);
        u32 buf[3];
        CopyTabSlotVec(buf, list, (u32)v);
        ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate((const nw4r::math::VEC3*)(buf));
        playUISound(2);
    } else {
        playUISound(5);
    }
}

// Check conditions and update state.
void CItemBoxGrid::SelectCatRow() {
    u8* p = (u8*)this;
    if ((s32)this->field_58 != 3) return;
    if (this->field_528) return;
    if (this->field_525 == -1) return;
    if (reinterpret_cast<CSysWin*>(p + 0x4ac)->isOpened()) return;
    if (reinterpret_cast<CSysWin*>(p + 0x4e8)->isOpened()) return;
    if (((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_IsVisibleFlag()) return;
    if (isPresentActive(reinterpret_cast<CPresentWin*>(p + 0x468))) return;
    if (((CSortMenu*)(p + 0xe8))->IsOpen()) return;
    if (this->field_544) return;
    this->field_525 = -1;
    UpdateCursors();
    RefreshSelectedCellText();
    playUISound(2);
}

// Check prerequisites and return a state code: each sub-window must be
// idle (nonzero -> 0); then 2 while the item box is animating, 5 while a
// confirm prompt is up, otherwise derive from the selected category byte
// (0x527) and the pending-selection byte (0x525).
u8 CItemBoxGrid::GetPromptState() {
    u8* p = (u8*)this;
    if ((s32)this->field_58 != 3) return 0;
    if (reinterpret_cast<CNumSelect*>(p + 0x3e4)->IsOpen()) return 0;
    if (((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_IsVisibleFlag()) return 0;
    if (((CExchangeWin*)(p + 0x440))->getField24()) return 0;
    if (isPresentActive(reinterpret_cast<CPresentWin*>(p + 0x468))) return 0;
    if (reinterpret_cast<CSysWin*>(p + 0x4ac)->isOpened()) return 0;
    if (reinterpret_cast<CSysWin*>(p + 0x4e8)->isOpened()) return 0;
    if (((CSortMenu*)(p + 0xe8))->IsOpen()) return 2;
    if (p[0x544]) return 5;
    u32 cat = p[0x527];
    if (cat == 4) return 0x76;
    if ((s8)p[0x525] == -1) {
        u32 r = 4;
        if ((u32)p[0x6e] > 1) r = 3;
        return r;
    }
    if (cat == 1) return 7;
    if (cat == 2) return 6;
    return 1;
}

// Animate grid panels (alt variant).
void CItemBoxGrid::AdvanceOpenAnim() {
    u8* p = (u8*)this;
    if (!advanceAnimTransform(this->field_48, 1.0f)) return;
    this->field_44->SetAnimationEnable(this->field_50, 0);
    this->field_44->SetAnimationEnable(this->field_48, 0);
    this->field_44->SetAnimationEnable(this->field_4C, 1);
    this->field_58 = 2;
}

void CItemBoxGrid::FinishOpenAnim() {
    u8* p = (u8*)this;
    if (advanceAnimTransform(this->field_4C, 1.0f)) {
        this->field_58 = 3;
        this->field_44->SetAnimationEnable(this->field_48, 0);
        this->field_44->SetAnimationEnable(this->field_4C, 0);
        this->field_44->SetAnimationEnable(this->field_50, 1);
        p[0x61] = 1;
        RefreshSelectedCellText();
        ((CBaseCur*)((p + 0x70)))->SetVisible(1);
        ((CBaseCur*)((p + 0x88)))->SetVisible(1);
        UpdateCursors();
        if (p[0x542]) {
            this->field_58 = 0x12;
            {
                u32 msg = (u32)BdatTouchStringCell((const char*)&lbl_eu_8050566C[0x310], &lbl_eu_8050566C[0x158], 0x27);
                reinterpret_cast<CSysWin*>((void*)(p + 0x4e8))->switchKindPane(0);
                reinterpret_cast<CSysWin*>((void*)(p + 0x4e8))->setKindContent(reinterpret_cast<char*>(msg), 0);
                reinterpret_cast<CSysWin*>((void*)(p + 0x4e8))->applyKind(1);
                reinterpret_cast<CSysWin*>((void*)(p + 0x4e8))->openPhase1();
                ((CBaseCur*)((p + 0x70)))->SetVisible(0);
            }
        }
    }
}


namespace nw4r { namespace lyt { class AnimTransform; } }

void CItemBoxGrid::CheckState4_Animate() {
    char* self = (char*)this; // noinline: dispatch table calls keep `bl`
    if (this->field_527 != 4) {
        return;
    }
    advanceAnimTransform(this->field_50, 1.0f);
}


// Animate grid panels.
void CItemBoxGrid::AdvanceCloseAnim() {
    u8* p = (u8*)this;
    if (!AnimRewindFrame(this->field_4C, 1.0f)) return;
    this->field_44->SetAnimationEnable(this->field_4C, 0);
    this->field_44->SetAnimationEnable(this->field_50, 0);
    this->field_44->SetAnimationEnable(this->field_48, 1);
    this->field_58 = 5;
}

void CItemBoxGrid::FinishCloseAnim() {
    void* self = this; // noinline: dispatch table calls keep `bl`
    float f = 1.0f;
    u8* p = (u8*)self;
    nw4r::lyt::AnimTransform* obj = this->field_48;
    if (AnimRewindFrame(obj, f)) {
        p[0x61] = 1;
        this->field_58 = 0;
        ((CBaseCur*)(p + 0x70))->SetVisible(0);
    }
}
void CItemBoxGrid::FinishNumSelect() {
    u8* p = (u8*)this;
    if (reinterpret_cast<CNumSelect*>(p + 0x3e4)->IsIdle()) {
        this->field_58 = 3;
        p[0x528] = 0;
        ((CBaseCur*)((p + 0x70)))->SetVisible(1);
        UpdateCursors();
        if (p[0x541]) {
            p[0x541] = 0;
            this->field_58 = 0x12;
            {
                u32 msg = (u32)BdatTouchStringCell((const char*)&lbl_eu_8050566C[0x310], &lbl_eu_8050566C[0x158], 0x10);
                reinterpret_cast<CSysWin*>((void*)(p + 0x4e8))->switchKindPane(0);
                reinterpret_cast<CSysWin*>((void*)(p + 0x4e8))->setKindContent(reinterpret_cast<char*>(msg), 0);
                reinterpret_cast<CSysWin*>((void*)(p + 0x4e8))->applyKind(1);
                reinterpret_cast<CSysWin*>((void*)(p + 0x4e8))->openPhase1();
                ((CBaseCur*)((p + 0x70)))->SetVisible(0);
            }
        }
    }
}
// Handle sub-object activation.
void CItemBoxGrid::ShowSubMenuCur() {
    u8* p = (u8*)this;
    if (!((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_IsOpenedFlag()) return;
    this->field_58 = 7;
    u8 temp[16];
    GridSubMenu_GetCursorPos(temp, (CItemBoxGridSubMenu*)(p + 0x418));
    ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate((const nw4r::math::VEC3*)(temp));
    ((CBaseCur*)(p + 0xa0))->SetVisible(1);
}


void CItemBoxGrid::DispatchSubmenuConfirm() {
    void* self = this; // noinline: dispatch table calls keep `bl`
    u8* p = (u8*)self;
    if (!((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_IsOpenedFlag()) return;
    *(u32*)(p + 0x58) = 3;
    if ((GridSubMenu_GetOptionCount(p + 0x418) & 0xFF) == 2) {
        // Confirmed: dispatch on the selected cell's category byte.
        s32 val = (s8)this->field_6F;
        if (p[val + 0x62] == 0xa) {
        // Category 0xa cell: feed the picked item kind into the exchange view.
        u8* sub = p + 0x54c;
        u8 entry = p[0x525] * 10 + p[0x524];
        u32 kind = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellKind((u16)entry);
        setPresentAnimText(p + 0x468, kind & 0xFFFF);
        openPresentWin(p + 0x468);
        *(u32*)(p + 0x58) = 0xf;
        p[0x528] = 1;
        return;
    }
    if (p[val + 0x62] == 0xd) {
        // Category 0xd cell: collect-per-character confirmation dialog.
        u8* sub = p + 0x54c;
        u8 entry = p[0x525] * 10 + p[0x524];
        u32 kind = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellKind((u16)entry);
        BdatGetItemType(kind & 0xFFFF);
        u32 kindShort = BdatGetItemId(kind & 0xFFFF);

        u32 tbl= (u32)::cf::CfBdat::spItmArtsListFileData;
        u32 catVal = BdatGetU8Direct((const void*)tbl, "memory_type", kindShort & 0xFFFF);
        u32 nameIdx = BdatGetU8Direct((const void*)tbl, "pc_type", kindShort & 0xFFFF);
        u32 paramVal = BdatGetU8Direct((const void*)tbl, "get_arts", kindShort & 0xFFFF);

        tbl = BdatGetU8ByTableKey((const char*)"pc_arts", "idx", paramVal & 0xFF);
        char* msg = (char*)BdatTouchStringCell("pc_arts", "name", paramVal & 0xFF);
        char* subMsg = 0;   // optional second dialog line
        u32 showDetail = 0;

        if (cf::CfGameManager::isResourceFlagSet(nameIdx & 0xFF)) {
            u8* save = (u8*)CtrlObjectParam_GetCharEntry(nameIdx & 0xFF);
            u32 cnt = CtrlObjectParam_GetCurrentRowKey(save) & 0xFF;
            // Per-(character, slot) record in the collection-save area.
            u8* rec = save + cnt * 0x49 + (tbl & 0xFF) * 2;
            u32 seenNew = 0;

            switch (catVal & 0xFF) {
            case 1:
                if (rec[0xe8]) {
                    msg = (char*)BdatTouchStringCell("MNU_battle", "name", 0x40);
                } else {
                    subMsg = (char*)BdatTouchStringCell("MNU_battle", "name", 0x39);
                    showDetail = 1;
                    seenNew = 1;
                    CtrlObjectParam_ClaimArtsSlotRow(save, paramVal & 0xFF);
                }
                break;
            case 2:
                if (rec[0xe8]) {
                    if (!(rec[0xe9] & 1)) {
                        rec[0xe9] |= 0x80;
                        subMsg = (char*)BdatTouchStringCell("MNU_battle", "name", 0x3a);
                        showDetail = 1;
                        seenNew = 1;
                        incrementEventCounter(0x46);
                    } else {
                        msg = (char*)BdatTouchStringCell("MNU_battle", "name", 0x40);
                    }
                } else {
                    msg = (char*)BdatTouchStringCell("MNU_battle", "name", 0x41);
                }
                break;
            case 3:
                if (rec[0xe8]) {
                    if (!(rec[0xe9] & 2)) {
                        rec[0xe9] |= 0xc0;
                        subMsg = (char*)BdatTouchStringCell("MNU_battle", "name", 0x3b);
                        showDetail = 1;
                        seenNew = 1;
                        incrementEventCounter(0x46);
                        incrementEventCounter(0x47);
                    } else {
                        msg = (char*)BdatTouchStringCell("MNU_battle", "name", 0x40);
                    }
                } else {
                    msg = (char*)BdatTouchStringCell("MNU_battle", "name", 0x41);
                }
                break;
            }

            if (seenNew) {
                void* itemObj = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellItemPtr((u16)entry);
                s32 state = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->CheckCellState(entry);
                DispatchItemEvt(itemObj, (s8)state - 1);
                RebuildGridCat();
            }
        } else {
            msg = (char*)BdatTouchStringCell("MNU_battle", "name", 0x42);
        }

        reinterpret_cast<CSysWin*>(p + 0x4e8)->switchKindPane(showDetail);
        reinterpret_cast<CSysWin*>(p + 0x4e8)->setKindContent(reinterpret_cast<char*>((u32)msg), reinterpret_cast<char*>((u32)subMsg));
        reinterpret_cast<CSysWin*>(p + 0x4e8)->applyKind(1);
        reinterpret_cast<CSysWin*>(p + 0x4e8)->openPhase1();
        *(u32*)(p + 0x58) = 0x12;
        p[0x528] = 0;
        return;
        }
        // Other categories: nothing to confirm.
        ((CBaseCur*)(p + 0x70))->SetVisible(1);
        p[0x528] = 0;
        return;
    }
    // Confirmation pending/not given: pop the notice dialog once.
    if (p[0x541]) {
        p[0x541] = 0;
        *(u32*)(p + 0x58) = 0x12;
        void* msg = BdatTouchStringCell("MNU_shop", "name", 0x10);
        reinterpret_cast<CSysWin*>(p + 0x4e8)->switchKindPane(0);
        reinterpret_cast<CSysWin*>(p + 0x4e8)->setKindContent(reinterpret_cast<char*>((u32)msg), 0);
        reinterpret_cast<CSysWin*>(p + 0x4e8)->applyKind(1);
        reinterpret_cast<CSysWin*>(p + 0x4e8)->openPhase1();
        ((CBaseCur*)(p + 0x70))->SetVisible(0);
    } else {
        ((CBaseCur*)(p + 0x70))->SetVisible(1);
    }
    p[0x528] = 0;
}

// Handle sub-object activation with copy.
void CItemBoxGrid::ShowExchangeCur() {
    u8* p = (u8*)this;
    if (!((CExchangeWin*)(p + 0x440))->getField27()) return;
    this->field_58 = 0xd;
    ((CBaseCur*)(p + 0xa0))->SetVisible(1);
    u8 temp[16];
    u8 val529 = p[0x529];
    ExWin_CopyPanePair(temp, &mExchangeWin, val529);
    reinterpret_cast<CBaseCur*>(p + 0xa0)->setRootPaneTranslate((const nw4r::math::VEC3*)(temp));
}

// Check if sub-obj is active; set state to 3 and clear flag.
void CItemBoxGrid::FinishExchange() {
    u8* p = (u8*)this;
    if (((CExchangeWin*)(p + 0x440))->getField27()) {
        this->field_58 = 3;
        p[0x528] = 0;
    }
}

// Handle sub-object activation (variant).
void CItemBoxGrid::ShowPresentCur() {
    u8* p = (u8*)this;
    if (!isPresentSettled(reinterpret_cast<CPresentWin*>(p + 0x468))) return;
    this->field_58 = 0x10;
    u8 temp[16];
    getPresentPaneWorldPos((nw4r::math::VEC3*)temp, reinterpret_cast<CPresentWin*>(p + 0x468));
    ((CBaseCur*)(p + 0xd0))->setRootPaneTranslate((const nw4r::math::VEC3*)(temp));
    ((CBaseCur*)(p + 0xd0))->SetVisible(1);
}

// Check if sub-obj is active; set state to 3 and clear flag.
void CItemBoxGrid::FinishPresent() {
    u8* p = (u8*)this;
    if (isPresentSettled(reinterpret_cast<CPresentWin*>(p + 0x468))) {
        this->field_58 = 3;
        p[0x528] = 0;
    }
}

// Check system window; if active, set state and call helpers.
void CItemBoxGrid::OnSysWinOpen() {
    u8* p = (u8*)this;
    if (reinterpret_cast<CSysWin*>(p + 0x4ac)->isActive()) {
        this->field_58 = 0x16;
        ((CBaseCur*)(p + 0xa0))->SetVisible(1);
        UpdateCursors();
    }
}

// Handle system window state change.
void CItemBoxGrid::HandleSysWinOK() {
    u8* p = (u8*)this;
    if (!reinterpret_cast<CSysWin*>(p + 0x4ac)->isActive()) return;
    if (!this->field_540) {
        u8 idx = this->field_6F;
        u8 cat = this->field_62[(s8)idx];
        u32 diff = cat - 2;
        if (diff <= 7) {
            reinterpret_cast<CNumSelect*>(p + 0x3e4)->SetArrowVisible(0);
        } else {
            reinterpret_cast<CNumSelect*>(p + 0x3e4)->SetArrowVisible(1);
        }
        reinterpret_cast<CNumSelect*>(p + 0x3e4)->Open();
        this->field_58 = 0x18;
        this->field_528 = 1;
    } else {
        this->field_58 = 3;
        ((CBaseCur*)(p + 0x70))->SetVisible(1);
        UpdateCursors();
    }
}

// Handle system window activation.
void CItemBoxGrid::FinishSysWin() {
    u8* p = (u8*)this;
    if (!reinterpret_cast<CSysWin*>(p + 0x4e8)->isActive()) return;
    this->field_58 = 3;
    if (!p[0x542] && !p[0x544]) {
        ((CBaseCur*)(p + 0x70))->SetVisible(1);
    }
    if (!p[0x544]) {
        UpdateCursors();
    }
    if (p[0x544]) {
        ((CBaseCur*)(p + 0xa0))->SetVisible(1);
    }
}

// Check conditions; set flags.
void CItemBoxGrid::SetGridReady() {
    // Guard chain: the message object at +0x44 and the list at +0x40 must be
    // live, and the three global table pointers must all be non-null, before
    // the busy/dirty flags are set.  The exit label keeps retail's
    // `bne store; blr` gate layout (see MWCC_CASES goto-gate pattern).
    if (this->field_44 == 0) goto exit;
    if (this->field_40 == 0) goto exit;
    if (itemBoxGrid_shopItemTypeTable == 0) goto exit;
    if (itemBoxGrid_shopSkillTable == 0) goto exit;
    if (itemBoxGrid_shopItemIdTable != 0) goto store;
    goto exit;
exit:
    return;
store:
    this->field_60 = 1;
    this->field_54 = 1;
    return;
}

// Resolve the cell's detail-message id and show it: per-item-type message
// lookups (equipment by the +0x54 slot-status name row 0x144..0x149, arts by
// the byte7>>2 subtype, skill items 0x155), each falling back to the +0x3C
// accessor's default label, then a kind name-table fallback, the list-tab
// override when bt is set, and finally the shared sprintf + PaneSetTexPaletteByName
// show.
void CItemBoxGrid::ShowCellIcon(u32 kind, void* item, u16 idx, u32 bt) {
    u8* p = (u8*)this;
    void* obj = item ? item : 0;
    u32 msgId = 0;
    if (obj) {
        u32 type = (((CItemBoxItem*)obj)->mWord >> 16) & 0xF;
        if (type == 3) {
            msgId = 0;
            CItemImpl* inst = CItem_initItemImplInstances((CItemData*)obj);
            u32 st = inst->vf54((CItemData*)obj);
            u8 c = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, (const char*)&lbl_eu_8050566C[0x331], (u16)st);
            switch (c) {
            case 4: msgId = (u32)MakeTplNameSysFile(0x144); break;
            case 5: msgId = (u32)MakeTplNameSysFile(0x145); break;
            case 6: msgId = (u32)MakeTplNameSysFile(0x146); break;
            case 7: msgId = (u32)MakeTplNameSysFile(0x147); break;
            case 8: msgId = (u32)MakeTplNameSysFile(0x148); break;
            case 9: msgId = (u32)MakeTplNameSysFile(0x149); break;
            }
            if (msgId) {
                msgId = (u32)this->field_40->GetResource(
                    0x74696d67, (char*)msgId, 0);
            } else {
                msgId = (u32)this->field_3C->GetResource(
                    0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
            }
        } else if (type == 9) {
            CItemImpl* inst = CItem_initItemImplInstances((CItemData*)obj);
            u16 hasArts = (u16)inst->vf80((CItemData*)obj);
            if (hasArts == 0) {
                if (IsSkillItem(obj)) {
                    msgId = (u32)MakeTplNameSysFile(0x155);
                    msgId = (u32)this->field_40->GetResource(
                        0x74696d67, (char*)msgId, 0);
                } else {
                    u32 sub = 0;
                    int v = (((CItemBoxItem*)obj)->mState >> 2) & 0x3F;
                    switch (v) {
                    case 4: sub = (u32)MakeTplNameSysFile(0x144); break;
                    case 5: sub = (u32)MakeTplNameSysFile(0x145); break;
                    case 6: sub = (u32)MakeTplNameSysFile(0x146); break;
                    case 7: sub = (u32)MakeTplNameSysFile(0x147); break;
                    case 8: sub = (u32)MakeTplNameSysFile(0x148); break;
                    case 9: sub = (u32)MakeTplNameSysFile(0x149); break;
                    }
                    if (sub) {
                        msgId = (u32)this->field_40->GetResource(
                            0x74696d67, (char*)sub, 0);
                    } else {
                        msgId = (u32)this->field_3C->GetResource(
                            0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
                    }
                }
            } else {
                // origin arts - same byte7>>2 subtype mapping
                u32 sub2 = 0;
                int v = (((CItemBoxItem*)obj)->mState >> 2) & 0x3F;
                switch (v) {
                case 4: sub2 = (u32)MakeTplNameSysFile(0x144); break;
                case 5: sub2 = (u32)MakeTplNameSysFile(0x145); break;
                case 6: sub2 = (u32)MakeTplNameSysFile(0x146); break;
                case 7: sub2 = (u32)MakeTplNameSysFile(0x147); break;
                case 8: sub2 = (u32)MakeTplNameSysFile(0x148); break;
                case 9: sub2 = (u32)MakeTplNameSysFile(0x149); break;
                }
                if (sub2) {
                    msgId = (u32)this->field_40->GetResource(
                        0x74696d67, (char*)sub2, 0);
                } else {
                    msgId = (u32)this->field_3C->GetResource(
                        0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
                }
            }
        } else {
            goto done;
        }
    }
done:
    if (!msgId) {
        if (kind) {
            u16 v = BdatGetU16Direct((const void*)::cf::CfBdat::spItmItemListFileData, &lbl_eu_8050566C[0x34d], kind);
            msgId = (u32)MakeTplNameSysFile(v);
            msgId = (u32)this->field_40->GetResource(
                0x74696d67, (char*)msgId, 0);
            if (!msgId) {
                msgId = (u32)this->field_3C->GetResource(
                    0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
            }
        } else {
            msgId = (u32)this->field_3C->GetResource(
                0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
        }
    }
    if (bt) {
        u8 cat = this->field_62[(s8)this->field_6F];
        if (cat == 3) {
            msgId = (u32)this->field_40->GetResource(
                0x74696d67, (char*)&lbl_eu_8050566C[0x357], 0);
        } else {
            msgId = (u32)this->field_40->GetResource(
                0x74696d67, (char*)&lbl_eu_8050566C[0x36d], 0);
        }
    }
    if (msgId) {
        char buf[32];
        sprintf(buf, (const char*)&lbl_eu_8050566C[0x383], idx + 1);
        PaneSetTexPaletteByName(this->field_44, (const char*)buf, (TPLPalette*)((void*)msgId));
    }
}

// Resolve the cell's detail-message id and show it: per-item-type message
// lookups (equipment 0x197..0x192 by the +0x08 slot value, arts 0x191..0x18d
// for skill items, 0x19c..0x198 for origin arts), each falling back to the
// +0x3C accessor's default label when the lookup yields nothing, then a kind
// name-table fallback, and finally the shared sprintf + PaneSetTexPaletteByName show.
void CItemBoxGrid::ShowCellBadge(u32 kind, void* item, u16 idx) {
    u8* p = (u8*)this;
    void* obj = item ? item : 0;
    u32 msgId = 0;
    if (obj) {
        u32 type = (((CItemBoxItem*)obj)->mWord >> 16) & 0xF;
        if (type == 3) {
            msgId = 0;
            CItemImpl* inst = CItem_initItemImplInstances((CItemData*)obj);
            u16 v = (u16)inst->vf08((CItemData*)obj);
            switch (v) {
            case 1: msgId = (u32)MakeTplNameSysFile(0x197); break;
            case 2: msgId = (u32)MakeTplNameSysFile(0x196); break;
            case 3: msgId = (u32)MakeTplNameSysFile(0x195); break;
            case 4: msgId = (u32)MakeTplNameSysFile(0x194); break;
            case 5: msgId = (u32)MakeTplNameSysFile(0x193); break;
            case 6: msgId = (u32)MakeTplNameSysFile(0x192); break;
            }
            if (msgId) {
                msgId = (u32)this->field_40->GetResource(
                    0x74696d67, (char*)msgId, 0);
            } else {
                msgId = (u32)this->field_3C->GetResource(
                    0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
            }
        } else if (type == 9) {
            void* inst = CItem_initItemImplInstances((CItemData*)obj);
            u16 hasArts = (u16)((CItemImpl*)inst)->vf80((CItemData*)obj);
            if (hasArts == 0) {
                if (IsSkillItem(obj)) {
                    // skill item - use the 0x191..0x18d mapping below
                } else {
                    u32 w = ((CItemBoxItem*)obj)->mWord;
                    int isArts = 0;
                    u32 t = (w >> 16) & 0xF;
                    if (t == 9 && (u8)(((CItemBoxItem*)obj)->mState & 3) == 1) isArts = 1;
                    if (!isArts && t != 9) goto done_item;
                }
                msgId = 0;
                CItemImpl* inst2 = CItem_initItemImplInstances((CItemData*)obj);
                u16 v = (u16)inst2->vf08((CItemData*)obj);
                switch (v) {
                case 1: msgId = (u32)MakeTplNameSysFile(0x191); break;
                case 2: msgId = (u32)MakeTplNameSysFile(0x190); break;
                case 3: msgId = (u32)MakeTplNameSysFile(0x18f); break;
                case 4: msgId = (u32)MakeTplNameSysFile(0x18e); break;
                case 5: msgId = (u32)MakeTplNameSysFile(0x18d); break;
                }
                if (msgId) {
                    msgId = (u32)this->field_40->GetResource(
                        0x74696d67, (char*)msgId, 0);
                } else {
                    msgId = (u32)this->field_3C->GetResource(
                        0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
                }
            } else {
                // origin arts - use the 0x19c..0x198 mapping
                msgId = 0;
                CItemImpl* inst3 = CItem_initItemImplInstances((CItemData*)obj);
                u16 v = (u16)inst3->vf08((CItemData*)obj);
                switch (v) {
                case 1: msgId = (u32)MakeTplNameSysFile(0x19c); break;
                case 2: msgId = (u32)MakeTplNameSysFile(0x19b); break;
                case 3: msgId = (u32)MakeTplNameSysFile(0x19a); break;
                case 4: msgId = (u32)MakeTplNameSysFile(0x199); break;
                case 5: msgId = (u32)MakeTplNameSysFile(0x198); break;
                }
                if (msgId) {
                    msgId = (u32)this->field_40->GetResource(
                        0x74696d67, (char*)msgId, 0);
                } else {
                    msgId = (u32)this->field_3C->GetResource(
                        0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
                }
            }
        } else {
            goto done_item;
        }
    }
done_item:
    if (!msgId) {
        if (kind) {
            u16 v = BdatGetU16Direct((const void*)::cf::CfBdat::spItmItemListFileData, &lbl_eu_8050566C[0x391], kind);
            msgId = (u32)MakeTplNameSysFile(v);
            msgId = (u32)this->field_40->GetResource(
                0x74696d67, (char*)msgId, 0);
            if (!msgId) {
                msgId = (u32)this->field_3C->GetResource(
                    0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
            }
        } else {
            msgId = (u32)this->field_3C->GetResource(
                0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
        }
    }
    if (msgId) {
        char buf[32];
        sprintf(buf, (const char*)&lbl_eu_8050566C[0x396], idx + 1);
        PaneSetTexPaletteByName(this->field_44, (const char*)buf, (TPLPalette*)((void*)msgId));
    }
}

// Set the item-box cell caption: format the cell number (idx+1) into one
// buffer and the state message into another, then pass both to the layout.
// The message depends on the item's kind nibble and the signed category byte:
// - kind nibble 3/9 (stackable count items): slot-count text resolved via
//   BdatTouchStringCell between tags 0x14f/0x158 (30 minus the slot count).
// - category > 0: sprintf with the raw category value.
// - category == 0 / unlisted negatives: plain default string.
// - category -2/-3: language-specific strings returned by
//   getErrMesText16/getErrMesText15 are used AS the format string, and the
//   matching pane-colour tag pair goes through PaneMatSetTevColorsByName.
// -O4,s: retail keeps a stmw r29 block save.
void CItemBoxGrid::SetCellCaption(s8 kind, void* item, u16 idx) {
    void* it = (item != 0) ? item : 0;
    u8* p = (u8*)this;
    char nameBuf[0x20];
    char msgBuf[0x20];
    sprintf(nameBuf, (const char*)&lbl_eu_8050566C[0x3a3], idx + 1);

    // Kind nibble of the item's descriptor word; 0 when there is no item.
    u32 sel;
    if (it != 0) {
        sel = ((((CItemBoxItem*)it)->mWord >> 16) & 0xF);
    } else {
        sel = 0;
    }

    if ((u8)sel == 3 || (u8)sel == 9) {
        CItemImpl* inst = CItem_initItemImplInstances((CItemData*)it);
        u32 cnt = (u32)inst->vf08((CItemData*)it);
        char* msg = BdatTouchStringCell((const char*)&lbl_eu_8050566C[0x14f], (const char*)&lbl_eu_8050566C[0x158],
                                  (u32)(0x1e - ((cnt & 0xFF) - 1)));
        sprintf(msgBuf, (const char*)&lbl_eu_8050566C[0x14c], msg);
        PaneMatSetTevColorsByName(this->field_44, nameBuf, (const GXColorS10*)&itemBoxGrid_shopLabelNeutralTop, (const GXColorS10*)&itemBoxGrid_shopLabelNeutralBottom);
    } else if (kind > 0) {
        sprintf(msgBuf, (const char*)&lbl_eu_8050566C[0x162], kind);
        PaneMatSetTevColorsByName(this->field_44, nameBuf, (const GXColorS10*)&itemBoxGrid_shopLabelNeutralTop, (const GXColorS10*)&itemBoxGrid_shopLabelNeutralBottom);
    } else if (kind == 0) {
        sprintf(msgBuf, (const char*)&lbl_eu_8050566C[0x3af]);
    } else if (kind == -2) {
        sprintf(msgBuf, getErrMesText16());
        PaneMatSetTevColorsByName(this->field_44, nameBuf, (const GXColorS10*)&itemBoxGrid_shopLabelSelectedTop, (const GXColorS10*)&itemBoxGrid_shopLabelSelectedBottom);
    } else if (kind == -3) {
        sprintf(msgBuf, getErrMesText15());
        PaneMatSetTevColorsByName(this->field_44, nameBuf, (const GXColorS10*)&itemBoxGrid_shopValueSelectedTop, (const GXColorS10*)&itemBoxGrid_shopValueSelectedBottom);
    } else {
        sprintf(msgBuf, (const char*)&lbl_eu_8050566C[0x3af]);
    }
    LayoutSetTextBoxText(this->field_44, nameBuf, msgBuf, 0);
}

// Show the cell-count label on the item box: format "x N" into a stack
// buffer, resolve the image pane through the +0x3C accessor's default label
// (overridden by the category-specific label when kind is 1 or 2, and by the
// flag-driven overlay labels), then hand the buffer to the layout via
// PaneSetTexPaletteByName.
void CItemBoxGrid::ShowCellCount(u32 kind, void* flag2, void* flag3, u32 idx) {
    u8* p = (u8*)this;
    char buf[0x20];
    sprintf(buf, (const char*)&lbl_eu_8050566C[0x3b0], idx + 1);

    void* acc;
    void* pane;
    acc = this->field_3C;
    pane = ((nw4r::lyt::ArcResourceAccessor*)acc)->GetResource(0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
    if (kind == 1) {
        acc = this->field_3C;
        pane = ((nw4r::lyt::ArcResourceAccessor*)acc)->GetResource(0x74696d67, (char*)&lbl_eu_8050566C[0x3bb], 0);
    } else if (kind == 2) {
        acc = this->field_3C;
        pane = ((nw4r::lyt::ArcResourceAccessor*)acc)->GetResource(0x74696d67, (char*)&lbl_eu_8050566C[0x3cf], 0);
    }
    if (flag3) {
        acc = this->field_3C;
        pane = ((nw4r::lyt::ArcResourceAccessor*)acc)->GetResource(0x74696d67, (char*)&lbl_eu_8050566C[0x3e3], 0);
    }
    if (flag2) {
        acc = this->field_3C;
        pane = ((nw4r::lyt::ArcResourceAccessor*)acc)->GetResource(0x74696d67, (char*)&lbl_eu_8050566C[0x3f7], 0);
    }

    if (pane) {
        // pane rides in r5 straight into the call (retail never reloads r5)
        PaneSetTexPaletteByName(this->field_44, (const char*)buf, (TPLPalette*)((void*)pane));
    }
}

// Format the current tab's header text: build the category label via the
// switch-driven sprintf (jump table), resolve the pane through the +0x40
// accessor (falling back to +0x3C's default label), then write the
// "<label> <idx+1>" string into the layout. -O4,s gives the
// retail stmw r28 prologue (4 saved regs).
// Case layout per retail jump table jumptable_eu_80534694: slots 0/1 share
// the exit (cases 0 and 1 skip the label lookup buffer), slots 2..13 map to
// the twelve label formats 0x40b..0x4f2.
void CItemBoxGrid::SetTabHeader(int cat, int idx) {
    u8* p = (u8*)this;
    char buf[32];
    char tmp[32];
    int result;
    if (cat != 0) {
        switch (cat) {
        case 1: break;
        case 2: sprintf(buf, (const char*)&lbl_eu_8050566C[0x40b]); break;
        case 3: sprintf(buf, (const char*)&lbl_eu_8050566C[0x420]); break;
        case 4: sprintf(buf, (const char*)&lbl_eu_8050566C[0x435]); break;
        case 5: sprintf(buf, (const char*)&lbl_eu_8050566C[0x44a]); break;
        case 6: sprintf(buf, (const char*)&lbl_eu_8050566C[0x45f]); break;
        case 7: sprintf(buf, (const char*)&lbl_eu_8050566C[0x474]); break;
        case 8: sprintf(buf, (const char*)&lbl_eu_8050566C[0x489]); break;
        case 9: sprintf(buf, (const char*)&lbl_eu_8050566C[0x49e]); break;
        case 10: sprintf(buf, (const char*)&lbl_eu_8050566C[0x4b3]); break;
        case 11: sprintf(buf, (const char*)&lbl_eu_8050566C[0x4c8]); break;
        case 12: sprintf(buf, (const char*)&lbl_eu_8050566C[0x4dd]); break;
        case 13: sprintf(buf, (const char*)&lbl_eu_8050566C[0x4f2]); break;
        }
        result = (int)this->field_40->GetResource(0x74696d67, buf, 0);
        if (!result) {
            result = (int)this->field_3C->GetResource(0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
        }
    } else {
        result = (int)this->field_3C->GetResource(0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
    }
    if (result) {
        sprintf(tmp, (const char*)&lbl_eu_8050566C[0x507], idx + 1);
        PaneSetTexPaletteByName(this->field_44, (const char*)tmp, (TPLPalette*)((void*)result));
    }
}

// Format text and set on layout pane.
void CItemBoxGrid::SetCellMarkVis(bool val, u32 idx) {
    u8* p = (u8*)this;
    char buf[32];
    sprintf(buf, (const char*)&lbl_eu_8050566C[0x513], idx + 1);
    nw4r::lyt::Layout* obj = this->field_44;
    nw4r::lyt::Pane* sub = obj->GetRootPane();
    nw4r::lyt::Pane* ret = sub->FindPaneByName(buf, 1);
    setPaneVisible(ret, val);
}

// Refresh the 12 category tabs: format each tab label, tint it per the
// current page (cat-4/2/0xB use the "active" palette, others the plain one),
// then update the two tab text panes. Afterwards rebuild the sub-grid if in
// category mode (0x527 == 2). No pragma: retail's stmw r26 prologue is the
// plain -O4 shape, so keep exactly six long-lived values (cat, isOtherTab,
// i, isCurTab, string base, self).
void CItemBoxGrid::RefreshCategoryTabs() {
    u8* p = (u8*)this;
    bool isCurTab;
    u32 i;
    for (i = 0; i < 12; i++) {
        char buf1[32];
        char buf2[32];
        int tabNum = (u8)i + 1;
        sprintf(buf1, (const char*)"pic_tbbcon%02d", tabNum);
        sprintf(buf2, (const char*)"pic_tbbcof%02d", tabNum);
        isCurTab = false;
        bool isOtherTab = false;
        u8 cat = p[(u8)i + 0x62];        if (cat != 0) {
            if ((u8)i == (s8)this->field_6F) {
                isCurTab = true;
                isOtherTab = false;
            } else {
                isCurTab = false;
                isOtherTab = true;
            }
            if ((u32)(cat - 4) <= 4 || (s32)cat == 2 || (s32)cat == 0xb) {
                PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(void**)(p + 0x44)), buf1, (const GXColorS10*)&itemBoxGrid_slotLabelDownTop, (const GXColorS10*)&itemBoxGrid_slotLabelDownBottom);
                PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(void**)(p + 0x44)), buf2, (const GXColorS10*)&itemBoxGrid_slotLabelDownTop, (const GXColorS10*)&itemBoxGrid_slotLabelDownBottom);
            } else {
                PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(void**)(p + 0x44)), buf1, (const GXColorS10*)&itemBoxGrid_slotLabelUpTop, (const GXColorS10*)&itemBoxGrid_slotLabelUpBottom);
                PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(void**)(p + 0x44)), buf2, (const GXColorS10*)&itemBoxGrid_slotLabelUpTop, (const GXColorS10*)&itemBoxGrid_slotLabelUpBottom);
            }
        }
        void* pane = *(void**)(*(u32*)(p + 0x44) + 0x10);
        nw4r::lyt::Pane* t1 = ((nw4r::lyt::Pane*)pane)->FindPaneByName(buf1, 1);
        setPaneVisible(t1, isCurTab);
        pane = *(void**)(*(u32*)(p + 0x44) + 0x10);
        nw4r::lyt::Pane* t2 = ((nw4r::lyt::Pane*)pane)->FindPaneByName(buf2, 1);
        setPaneVisible(t2, isOtherTab);
        SetTabHeader(cat, (u8)i);
    }
    MoveTabPage(0);
    if (p[0x527] == 2) {
        CItemBoxGridFull* subGrid = (CItemBoxGridFull*)(p + 0x54c);
        p[0x542] = 0;
        ((CItemBoxGridFull*)(subGrid))->PopulateGridEntries(p[0x62], 0, *(u16*)(p + 0x52e), *(u16*)(p + 0x52a));
        u16 count1 = subGrid->field_2800;
        ((CItemBoxGridFull*)(subGrid))->PopulateGridEntries(p[0x63], 0, *(u16*)(p + 0x52e), *(u16*)(p + 0x52a));
        if (!(count1 | subGrid->field_2800)) {
            p[0x542] = 1;
        }
    }
    RebuildGridCat();
}

void CItemBoxGrid::RebuildGridCat() {
    u8* p = (u8*)this;
    u8* sub = p + 0x54c;
    ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->PopulateGridEntries(this->field_62[(s8)this->field_6F], 0, this->field_52E, this->field_52A);
    if (this->field_527 == 4) {
        ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->CompactGridEntries();
    }
    setLayoutTextBoxNumber(this->field_44,
                  const_cast<char*>(&lbl_eu_8050566C[0x53e]),
                  (u8)((CItemBoxGridFull*)((void*)sub))->CalcGridRowCount());
    RebuildGridUI();
    RefreshSelectedCellText();
    func_801D0BD8();
    if (((CItemBoxInfo*)(p + 0x1d8))->GetState() != 0) {
        ((CItemBoxInfo*)(p + 0x1d8))->HidePanes(this->field_62[(s8)this->field_6F]);
    }
}

// Rebuilds the item-box grid UI: shows/hides the row panes for the current
// row count/page, then pushes all 30 entries' state back into the widgets.
// Keep `rows` full-width: retail converts (10 - rows) through the u8 truncate
// and holds the raw return in a callee-saved register across both loops.
void CItemBoxGrid::RebuildGridUI() {
    // Declaration order nudges MWCC's callee-saved allocation toward retail
    // (rows gets the highest register, sub/this the middle ones).
    u32 rows;
    CItemBoxGridFull* sub;
    u8* p = (u8*)this;
    sub = (CItemBoxGridFull*)(p + 0x54c);
    rows = ((CItemBoxGridFull*)(sub))->CalcGridRowCount();
    if ((u8)rows > 1) {
        void* pane = *(void**)(*(u32**)(p + 0x44) + 4);
        nw4r::lyt::Pane* ret = ((nw4r::lyt::Pane*)pane)->FindPaneByName((char*)"nul_pg", 1);
        setPaneVisible(ret, 1);
        u32 visRows = rows & 0xFF;
        u8 i;
        for (i = 0; i < 10; i++) {
            char buf[32];
            sprintf(buf, (char*)"pic_pghole%02d", i + 1);
            void* pane2 = *(void**)(*(u32**)(p + 0x44) + 4);
            nw4r::lyt::Pane* ret2 = ((nw4r::lyt::Pane*)pane2)->FindPaneByName(buf, 1);
            // visible while i < visRows (borrow trick)
            setPaneVisible(ret2, ((u32)i - visRows) >> 31);
        }
        u8 pageDisp = (u8)(sub->field_2804 + 1);
        setLayoutTextBoxNumber(*(nw4r::lyt::Layout**)(p + 0x44), (char*)"txt_pg00", pageDisp);
        void* pane3 = *(void**)(*(u32**)(p + 0x44) + 4);
        void* ret3 = ((nw4r::lyt::Pane*)pane3)->FindPaneByName((char*)"pic_pghole_on", 1);
        float v[3];
        v[0] = *(float*)(p + 0x534);
        v[1] = *(float*)(p + 0x538);
        v[2] = *(float*)(p + 0x53c);
        float step = 16.0f;
        // Hand-built 0x43300000 conversions (signed page term via xoris,
        // unsigned row deficit direct) so the only pool refs are retail's
        // named magics F48/F40 - compiler cast magics would land in an
        // anonymous pool and fail the reloc gate.
        union {
            u32 w[2];
            double d;
        } convRows, convPage;
        convRows.w[0] = 0x43300000;
        convPage.w[0] = 0x43300000;
        convRows.w[1] = (u32)(u8)(10 - rows);
        convPage.w[1] = (u32)(pageDisp - 1) ^ 0x80000000;
        float tPage = (float)(convPage.d - lbl_eu_80667F48);
        float tRows = (float)(convRows.d - lbl_eu_80667F40);
        float offset = step * tPage + step * tRows;
        v[0] += offset;
        copyVEC3((u8*)&((nw4r::lyt::Pane*)ret3)->GetTranslate(), v);
    } else {
        void* pane = *(void**)(*(u32**)(p + 0x44) + 4);
        nw4r::lyt::Pane* ret = ((nw4r::lyt::Pane*)pane)->FindPaneByName((char*)"nul_pg", 1);
        setPaneVisible(ret, 0);
    }
    u8 i;
    for (i = 0; i < 3; i++) {
        char buf[32];
        sprintf(buf, (char*)"pic_hole%02d", i + 1);
        void* pane = *(void**)(*(u32**)(p + 0x44) + 4);
        nw4r::lyt::Pane* ret = ((nw4r::lyt::Pane*)pane)->FindPaneByName(buf, 1);
        if (ret) {
            u8 max = (u8)((CItemBoxGridFull*)((char*)sub))->LookupIndexedByte();
            setPaneVisible(ret, i < max);
        }
    }
    u8 idx;
    for (idx = 0; idx < 0x1e; idx++) {
        u32 kind = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellKind((u16)idx);
        void* obj = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellItemPtr((u16)idx);
        u32 bt = ((CItemBoxGridFull*)(sub))->GetCellByte7(idx);
        ShowCellIcon(kind & 0xFFFF, obj, idx, bt);
        ShowCellBadge(kind & 0xFFFF, obj, idx);
        s32 chk = ((CItemBoxGridFull*)(sub))->CheckCellState(idx);
        SetCellCaption((s8)chk, obj, idx);
        u32 b5 = ((CItemBoxGridFull*)(sub))->GetCellFlag3(idx);
        u32 b4 = ((CItemBoxGridFull*)(sub))->GetCellFlag2(idx);
        u32 b2 = ((CItemBoxGridFull*)(sub))->GetCellFlag0(idx);
        ShowCellCount(b2 & 0xFF, (void*)b4, (void*)b5, idx);
        bool b7 = ((CItemBoxGridFull*)(sub))->GetCellByte8(idx);
        SetCellMarkVis(b7, idx);
    }
}

// Refreshes the grid UI text for the selected cell. Retail computes the
// selected index unconditionally from the raw bytes, then special-cases the
// "no selection" rows (-1/-2).
void CItemBoxGrid::RefreshSelectedCellText() {
    u8* p = (u8*)this;
    CItemBoxGridFull* sub = (CItemBoxGridFull*)(p + 0x54c);
    s8 row = (s8)p[0x525];
    s8 idx = (s8)(p[0x524] + p[0x525] * 10);
    if (row == -1) {
        char* val = (char*)GetCatLabelMsg();
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)*(u32**)(p + 0x44), (char*)"txt_name", val, 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)*(u32**)(p + 0x44), (char*)"txt_comment", (char*)"", 0);
    } else if (row == -2) {
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)*(u32**)(p + 0x44), (char*)"txt_name", (char*)"", 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)*(u32**)(p + 0x44), (char*)"txt_comment", (char*)"", 0);
    } else {
        char* name = (char*)((CItemBoxGridFull*)(sub))->FormatCellName((u16)(u8)idx);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)*(u32**)(p + 0x44), (char*)"txt_name", name, 0);
        char* str = ((CItemBoxGridFull*)(sub))->FormatCellItemName((u16)(u8)idx);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)*(u32**)(p + 0x44), (char*)"txt_comment", str, *(u32*)(p + 0x5c));
    }
    u32 qty;
    if (p[0x527] == 2) {
        void* obj = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellItemPtr((u16)(u8)idx);
        u32 kind = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellKind((u16)(u8)idx);
        ((CItemBoxGridFull*)(sub))->CheckCellState((u16)(u8)idx);  // result unused in retail
        if ((s8)p[0x525] > -1)
            qty = ((CItemBoxGridFull*)(sub))->GetScaledItemValue(kind & 0xFFFF, (void*)obj, *(u16*)(p + 0x52a));
        else
            qty = 0;
    } else {
        if ((s8)p[0x525] > -1)
            qty = ((CItemBoxGridFull*)(sub))->GetCellSellPrice((u16)(u8)idx);
        else
            qty = 0;
    }
    char buf[0x30];
    sprintf(buf, (const char*)"%d%s", qty, (char*)BdatTouchStringCell("MNU_item", "name", 3));
    LayoutSetTextBoxText((nw4r::lyt::Layout*)*(u32**)(p + 0x44), (char*)"txt_buyvalue02", buf, 0);
    if (((CItemBoxInfo*)(p + 0x1d8))->GetState()) {
        u32 kind2;
        if ((s8)p[0x525] > -1) {
            void* obj2 = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellItemPtr((u16)(u8)idx);
            kind2 = ((CItemBoxGridFull*)((CItemBoxGridFull*)sub))->GetCellKind((u16)(u8)idx) & 0xFFFF;
            ((CItemBoxInfo*)((void*)(p + 0x1d8)))->SetItem(kind2, obj2, 1);
        } else {
            ((CItemBoxInfo*)((void*)(p + 0x1d8)))->SetItem(0, 0, 1);
        }
        u32 val2;
        if ((s8)p[0x525] > -1)
            val2 = ((CItemBoxGridFull*)(sub))->FormatCellName((u16)(u8)idx);
        else
            val2 = 0;
        ((CItemBoxInfo*)(p + 0x1d8))->SetNamedText(1, (char*)val2);
        // Right-to-left evaluation: retail calls C6840 first.
        ((CItemBoxInfo*)(p + 0x1d8))->SetCounts(((CItemBoxGridFull*)(sub))->CountMarkedCells() & 0xFF, ((CItemBoxGridFull*)(sub))->GetCategoryCap() & 0xFF);
    }
}

// Tab-page mover: rewrites the "N/total" tab labels for the new page
// (wrapping around the page count at +0x6e) and refreshes the 12 overlay
// panes with the colour pair matching the current category.
void CItemBoxGrid::MoveTabPage(int val) {
    u8* p = (u8*)this;
    u8 buf[2];
    buf[0] = this->field_6F;
    s8 v = (s8)this->field_6F;
    buf[1] = (u8)((val != 0) ? v + 1 : v - 1);
    if ((s8)buf[1] < 0) {
        buf[1] = p[0x6e] - 1;
    } else if ((s8)buf[1] >= p[0x6e]) {
        buf[1] = 0;
    }
    for (u32 i = 0; i < 2; i++) {
        char pane1[32];
        char pane2[32];
        sprintf(pane1, XB_K + 0x520, (s8)buf[(u8)i] + 1);
        sprintf(pane2, XB_K + 0x52f, (s8)buf[(u8)i] + 1);
        bool isCurTab = false;
        s32 idx = (s8)buf[(u8)i];
        bool isOtherTab = false;
        if (p[0x62 + idx] != 0) {
            if (idx == (s8)this->field_6F) {
                isCurTab = true;
                isOtherTab = false;
            } else {
                isCurTab = false;
                isOtherTab = true;
            }
        }
        setPaneVisible(((nw4r::lyt::Pane*)*(void**)(*(u32*)(p + 0x44) + 0x10))->FindPaneByName(pane1, 1), isCurTab);
        setPaneVisible(((nw4r::lyt::Pane*)*(void**)(*(u32*)(p + 0x44) + 0x10))->FindPaneByName(pane2, 1), isOtherTab);
    }
    {
        u8 cat = this->field_62[(s8)this->field_6F];
        if ((u32)(cat - 4) <= 4 || (s32)cat == 2 || (s32)cat == 0xB) {
            // Layout pointer re-derived per call: PaneMatSetTevColorsByName clobbers
            // memory, so MWCC reloads *(p+0x44) before every call.
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(u32*)(p + 0x44)), XB_K + 0x5aa, (const GXColorS10*)&itemBoxGrid_slotLabelDownTop, (const GXColorS10*)&itemBoxGrid_slotLabelDownBottom);
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(u32*)(p + 0x44)), XB_K + 0x5b6, (const GXColorS10*)&itemBoxGrid_slotLabelDownTop, (const GXColorS10*)&itemBoxGrid_slotLabelDownBottom);
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(u32*)(p + 0x44)), XB_K + 0x5c2, (const GXColorS10*)&itemBoxGrid_slotLabelDownTop, (const GXColorS10*)&itemBoxGrid_slotLabelDownBottom);
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(u32*)(p + 0x44)), XB_K + 0x5ce, (const GXColorS10*)&itemBoxGrid_slotLabelDownTop, (const GXColorS10*)&itemBoxGrid_slotLabelDownBottom);
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(u32*)(p + 0x44)), XB_K + 0x5da, (const GXColorS10*)&itemBoxGrid_slotLabelDownTop, (const GXColorS10*)&itemBoxGrid_slotLabelDownBottom);
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(u32*)(p + 0x44)), XB_K + 0x5e6, (const GXColorS10*)&itemBoxGrid_slotLabelDownTop, (const GXColorS10*)&itemBoxGrid_slotLabelDownBottom);
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(u32*)(p + 0x44)), XB_K + 0x5f2, (const GXColorS10*)&itemBoxGrid_slotLabelDownTop, (const GXColorS10*)&itemBoxGrid_slotLabelDownBottom);
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(u32*)(p + 0x44)), XB_K + 0x5fe, (const GXColorS10*)&itemBoxGrid_slotLabelDownTop, (const GXColorS10*)&itemBoxGrid_slotLabelDownBottom);
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(u32*)(p + 0x44)), XB_K + 0x60a, (const GXColorS10*)&itemBoxGrid_slotLabelDownTop, (const GXColorS10*)&itemBoxGrid_slotLabelDownBottom);
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(u32*)(p + 0x44)), XB_K + 0x616, (const GXColorS10*)&itemBoxGrid_slotLabelDownTop, (const GXColorS10*)&itemBoxGrid_slotLabelDownBottom);
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(u32*)(p + 0x44)), XB_K + 0x621, (const GXColorS10*)&itemBoxGrid_slotLabelDownTop, (const GXColorS10*)&itemBoxGrid_slotLabelDownBottom);
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(u32*)(p + 0x44)), XB_K + 0x62c, (const GXColorS10*)&itemBoxGrid_slotLabelDownTop, (const GXColorS10*)&itemBoxGrid_slotLabelDownBottom);
        } else {
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(u32*)(p + 0x44)), XB_K + 0x5aa, (const GXColorS10*)&itemBoxGrid_slotLabelUpTop, (const GXColorS10*)&itemBoxGrid_slotLabelUpBottom);
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(u32*)(p + 0x44)), XB_K + 0x5b6, (const GXColorS10*)&itemBoxGrid_slotLabelUpTop, (const GXColorS10*)&itemBoxGrid_slotLabelUpBottom);
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(u32*)(p + 0x44)), XB_K + 0x5c2, (const GXColorS10*)&itemBoxGrid_slotLabelUpTop, (const GXColorS10*)&itemBoxGrid_slotLabelUpBottom);
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(u32*)(p + 0x44)), XB_K + 0x5ce, (const GXColorS10*)&itemBoxGrid_slotLabelUpTop, (const GXColorS10*)&itemBoxGrid_slotLabelUpBottom);
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(u32*)(p + 0x44)), XB_K + 0x5da, (const GXColorS10*)&itemBoxGrid_slotLabelUpTop, (const GXColorS10*)&itemBoxGrid_slotLabelUpBottom);
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(u32*)(p + 0x44)), XB_K + 0x5e6, (const GXColorS10*)&itemBoxGrid_slotLabelUpTop, (const GXColorS10*)&itemBoxGrid_slotLabelUpBottom);
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(u32*)(p + 0x44)), XB_K + 0x5f2, (const GXColorS10*)&itemBoxGrid_slotLabelUpTop, (const GXColorS10*)&itemBoxGrid_slotLabelUpBottom);
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(u32*)(p + 0x44)), XB_K + 0x5fe, (const GXColorS10*)&itemBoxGrid_slotLabelUpTop, (const GXColorS10*)&itemBoxGrid_slotLabelUpBottom);
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(u32*)(p + 0x44)), XB_K + 0x60a, (const GXColorS10*)&itemBoxGrid_slotLabelUpTop, (const GXColorS10*)&itemBoxGrid_slotLabelUpBottom);
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(u32*)(p + 0x44)), XB_K + 0x616, (const GXColorS10*)&itemBoxGrid_slotLabelUpTop, (const GXColorS10*)&itemBoxGrid_slotLabelUpBottom);
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(u32*)(p + 0x44)), XB_K + 0x621, (const GXColorS10*)&itemBoxGrid_slotLabelUpTop, (const GXColorS10*)&itemBoxGrid_slotLabelUpBottom);
            PaneMatSetTevColorsByName((nw4r::lyt::Layout*)(*(u32*)(p + 0x44)), XB_K + 0x62c, (const GXColorS10*)&itemBoxGrid_slotLabelUpTop, (const GXColorS10*)&itemBoxGrid_slotLabelUpBottom);
        }
    }
}

// Refresh the cursor-position readouts: when the +0x4ac window is up, feed
// its cursor position into the +0xa0 cursor; otherwise compute the selected
// cell's pane position (AccumPaneWorldPos), scale its x by the label pane's
// +0x44 factor, and drive the +0x70/+0xb8 cursors with it.
void CItemBoxGrid::UpdateCursors() {
    u8* p = (u8*)this;
    nw4r::math::VEC3 posT2;
    nw4r::math::VEC3 posT1;
    nw4r::math::VEC3 posDef;
    nw4r::math::VEC3 posWin;
    if (reinterpret_cast<CSysWin*>(p + 0x4ac)->isOpened()) {
        CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(&posWin), reinterpret_cast<CSysWin*>((void*)(p + 0x4ac)), p[0x540]);
        ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate((const nw4r::math::VEC3*)(&posWin));
        return;
    }
    s8 idx = (s8)p[0x525];
    if (idx == -2) {
        nw4r::lyt::Pane* t1 = this->field_44->GetRootPane()->FindPaneByName((char*)&lbl_eu_8050566C[0x637], 1);
        AccumPaneWorldPos(&posT2, (nw4r::lyt::Pane*)((void*)t1));
        nw4r::lyt::Pane* t2 = this->field_44->GetRootPane()->FindPaneByName((char*)&lbl_eu_8050566C[0x301], 1);
        posT2.x = posT2.x * ((const nw4r::lyt::Pane*)t2)->GetScale().x;
        ((CBaseCur*)(p + 0xb8))->setRootPaneTranslate((const nw4r::math::VEC3*)(&posT2));
        ((CBaseCur*)(p + 0x70))->SetVisible(0);
        ((CBaseCur*)(p + 0xb8))->SetVisible(1);
        return;
    }
    if (idx == -1) {
        char str[32];
        sprintf(str, (const char*)&lbl_eu_8050566C[0x507], (s8)this->field_6F + 1);
        nw4r::lyt::Pane* t1 = this->field_44->GetRootPane()->FindPaneByName(str, 1);
        AccumPaneWorldPos(&posT1, (nw4r::lyt::Pane*)((void*)t1));
        nw4r::lyt::Pane* t2 = this->field_44->GetRootPane()->FindPaneByName((char*)&lbl_eu_8050566C[0x301], 1);
        posT1.x = posT1.x * ((const nw4r::lyt::Pane*)t2)->GetScale().x;
        ((CBaseCur*)(p + 0x70))->setRootPaneTranslate((const nw4r::math::VEC3*)(&posT1));
        ((CBaseCur*)(p + 0x70))->SetVisible(1);
        ((CBaseCur*)(p + 0xb8))->SetVisible(0);
        return;
    }
    {
        char str[32];
        sprintf(str, (const char*)&lbl_eu_8050566C[0x396], (s8)p[0x524] + idx * 10 + 1);
        nw4r::lyt::Pane* t1 = this->field_44->GetRootPane()->FindPaneByName(str, 1);
        AccumPaneWorldPos(&posDef, (nw4r::lyt::Pane*)((void*)t1));
        nw4r::lyt::Pane* t2 = this->field_44->GetRootPane()->FindPaneByName((char*)&lbl_eu_8050566C[0x301], 1);
        posDef.x = posDef.x * ((const nw4r::lyt::Pane*)t2)->GetScale().x;
        ((CBaseCur*)(p + 0x70))->setRootPaneTranslate((const nw4r::math::VEC3*)(&posDef));
        ((CBaseCur*)(p + 0x70))->SetVisible(1);
        ((CBaseCur*)(p + 0xb8))->SetVisible(0);
    }
}

__declspec(noinline) void CItemBoxGrid::func_801D0BD8() {
    void* self = this; }

// Unequip helper: scans the 8 party member slots for a member holding item
// `id`. Category 3 searches every item of the 6 sub-categories (the scan list
// is re-copied from an 8-byte sdata2 seed each pass) and clears the owning
// slot via the item's vtable+0x44 method; equipment categories (2, 4-8)
// compare the member's slot field and clear it with the dedicated setter.
// A match (or any non-equipment category) then runs one camera/enum-list
// pass that finds the on-screen move object for that player and detaches it;
// category 3 without a match just continues to the next pass.
#pragma push
void CItemBoxGrid::UnequipPartyItem(int kind, int id) {
    u8 cats[6];
    // Aggregate seed: MWCC stores both halves to the stack in the prologue
    // and the category-3 arm re-copies them into the scan list every pass.
    struct GridCatsSeed {
        u32 w;
        u16 h;
    } seed = {*(const u32*)(const void*)&lbl_eu_80667F88,
              *(const u16*)(const void*)&lbl_eu_80667F8C};
    int found;  // only assigned in the category-3 arm; stale elsewhere
    u8 pass;
    for (pass = 1; pass <= 8; pass++) {
        void* obj = (void*)CtrlObjectParam_GetCharEntry(pass);
        if (kind == 3) {
            found = 0;
            *(u32*)cats = seed.w;
            *(u16*)(cats + 4) = seed.h;
            u8 i;
            for (i = 0; i < 6; i++) {
                u8 cat = cats[i];
                u16 count = CItemBlock_countKindSlots(cat);
                u16 j;
                for (j = 0; j < count; j++) {
                    void* item = CItem_kindPtr(cat, j);
                    if (!item) continue;
                    if (!*(u32*)item) continue;
                    CItemImpl* inst = CItem_initItemImplInstances((CItemData*)item);
                    u8 num = (u8)inst->vf30((CItemData*)item);
                    u8 k;
                    for (k = 0; k < num; k++) {
                        int sv = (s16)CItem_initItemImplInstances((CItemData*)item)->vf40(
                            (CItemData*)item, k);
                        if (sv == -1) continue;
                        if (sv != id) continue;
                        CItem_initItemImplInstances((CItemData*)item)->vf44(
                            (CItemData*)item, k, -1);
                        found = 1;
                        break;
                    }
                    if (found) break;
                }
                if (found) break;
            }
            if (!found) continue;
        } else {
            // Equipment slots: only a member actually wearing `id` proceeds.
            switch (kind) {
            case 2: {
                int v = ((CItemBoxCharEntry*)obj)->slotIds[5];
                if (v == -1 || v != id) continue;
                CtrlObjectParam_SetEquipSlot5(obj, -1);
                break;
            }
            case 4: {
                int v = ((CItemBoxCharEntry*)obj)->slotIds[0];
                if (v == -1 || v != id) continue;
                CtrlObjectParam_SetEquipSlot0(obj, -1);
                break;
            }
            case 5: {
                int v = ((CItemBoxCharEntry*)obj)->slotIds[1];
                if (v == -1 || v != id) continue;
                CtrlObjectParam_SetEquipSlot1(obj, -1);
                break;
            }
            case 6: {
                int v = ((CItemBoxCharEntry*)obj)->slotIds[2];
                if (v == -1 || v != id) continue;
                reinterpret_cast<cf::CtrlObjectParamData*>(obj)->setArgType2((void*)-1);
                break;
            }
            case 7: {
                int v = ((CItemBoxCharEntry*)obj)->slotIds[3];
                if (v == -1 || v != id) continue;
                reinterpret_cast<cf::CtrlObjectParamData*>(obj)->setArgType3((void*)-1);
                break;
            }
            case 8: {
                int v = ((CItemBoxCharEntry*)obj)->slotIds[4];
                if (v == -1 || v != id) continue;
                CtrlObjectParam_SetEquipSlot4(obj, -1);
                break;
            }
            }
        }
        // Camera pass: filter the scene enum list by this player and detach
        // its move object when the player index matches.
        CtrlObjectParam_SyncParamFromActor((void*)found);
        u32 names[3];
        const u32* src = (const u32*)itemBoxGridSlotWordTable;
        names[0] = *src++;
        names[1] = *src++;
        names[2] = *src++;
        CfEnumListHolder holder;
        u8 i;
        for (i = 0; i < 3; i++) {
            startEnumObjects(holder.get(), names[i], 0);
            CItemBoxGridEnumList* list = (CItemBoxGridEnumList*)holder.get();
            if (list->field_620 >= 1) {
                list = (CItemBoxGridEnumList*)holder.get();
                CItemBoxGridEnumSlot* slot =
                    (CItemBoxGridEnumSlot*)getEntryAt(list, 0);
                if (slot->field_04) {
                    CItemBoxGridMoveObj* mv = (CItemBoxGridMoveObj*)
                        getCfObjectPc__FPQ22cf12CfObjectMove(slot->field_04);
                    if ((u8)pass == mv->field_3f28) {
                        CfObjectPc_setupFrame(reinterpret_cast<cf::CfObjectPc*>(mv));
                        break;
                    }
                }
            }
        }
        return;
    }
}

// Handle item event dispatch.
void CItemBoxGrid::DispatchItemEvt(void* item, int eventType) {
    if (!item) return;
    if (eventType < 1) {
        CItem_initItemImplInstances((CItemData*)item)->vf10((CItemData*)item);
    } else {
        u32 w = ((CItemBoxItem*)item)->mWord;
        CItemData_initFromFamily(item, w >> 20);
    }
}

// Cast-only vtable interface for the object returned by
// CItem_initItemImplInstances: with -RTTI on, MWCC prepends 2 hidden RTTI
// header entries, so the Nth declared virtual sits at 4*(N+2). Real virtual
// dispatch reproduces the retail `lwz r12,0(r3); lwz r12,<off>(r12)`
// sequence; manual `(*(void***)x)[N]` casts color a scratch r5 instead.
// Dispatch based on entry category.
u32 CItemBoxGrid::GetCatLabelMsg() {
    u8* p = (u8*)this;
    u8* entry = p + (s8)this->field_6F;
    u8 cat = entry[0x62];
    u32 result = 0;
    switch (cat) {
        case 2:  result = 0x33; break;
        case 3:  result = 0x3C; break;
        case 4:  result = 0x34; break;
        case 5:  result = 0x35; break;
        case 6:  result = 0x36; break;
        case 7:  result = 0x37; break;
        case 8:  result = 0x38; break;
        case 9:  result = 0x3D; break;
        case 10: result = 0x3E; break;
        case 11: result = 0x39; break;
        case 12: result = 0x3B; break;
        case 13: result = 0x3A; break;
    }
    if (result) {
        return (u32)BdatTouchStringCell(&lbl_eu_8050566C[0x14f], &lbl_eu_8050566C[0x158], result);
    }
    return 0;
}

// Check item teachability: look up the short-kind row in the arts table
// (::cf::CfBdat::spItmArtsListFileData) and test the character's learned-arts flags at
// charData + count*0x49 + (table byte << 1). Returns 1 when the flag test
// fails (item not yet learned); 0 when it is already learned.
u32 CItemBoxGrid::IsItemTeachable(u32 kind) {
    u32 bdat= (u32)::cf::CfBdat::spItmArtsListFileData;
    BdatGetItemType(kind);
    u16 shortKind = BdatGetItemId(kind);
    u8 flag = BdatGetU8Direct((const void*)bdat, (const char*)"memory_type", (u16)shortKind);
    u8 v;
    u8 charId = BdatGetU8Direct((const void*)bdat, (const char*)"pc_type", (u16)shortKind);
    u8 unk = BdatGetU8Direct((const void*)bdat, (const char*)"get_arts", (u16)shortKind);
    v = BdatGetU8ByTableKey((const char*)"pc_arts", (const char*)"idx", unk);
    void* charData = (void*)CtrlObjectParam_GetCharEntry(charId);
    u8* ptr = (u8*)charData + (CtrlObjectParam_GetCurrentRowKey(charData) & 0xFF) * 0x49 + (v << 1);
    switch (flag) {
    case 1:
        if (ptr[0xe8]) return 0;
        break;
    case 2:
        if (ptr[0xe9] & 0x80) return 0;
        break;
    case 3:
        if (ptr[0xe9] & 0x40) return 0;
        break;
    }
    return 1;
}


extern "C" void CopyVec4s(GXColorS10* dst, const GXColorS10* src) {
    dst->r = src->r;
    dst->g = src->g;
    dst->b = src->b;
    dst->a = src->a;
}

// Split a u32 into its four bytes, stored as shorts (big-endian order).
extern "C" void SplitU32ToS16s(short* dst, unsigned long val) {
    dst[3] = val & 0xFF;
    dst[2] = (val >> 8) & 0xFF;
    dst[1] = (val >> 16) & 0xFF;
    dst[0] = (val >> 24) & 0xFF;
}



// CItemBoxGrid::OnFileEvent - file-load completion dispatch. Four pending
// load slots are keyed by the event's file handle: +0x28 builds the whole
// item-box layout (fonts, animations, cursors, colour tables), +0x2c loads
// the secondary arc, +0x30/+0x34 register two BDAT tables.
bool CItemBoxGrid::OnFileEvent(CEventFile* pEventFile) {
    void* event = pEventFile;
    u8* p = (u8*)this;
    // Keep the handle comparison operandless so MWCC holds it in r4 like retail.
    if (*(u32*)(p + 0x28) == *(u32*)((u8*)event + 4)) {
    void* memHandle = getHandleMEM2__Q23mtl10MemManagerFv();
    createRegion__17UnkClass_8045F564FiiPCci(p + 8, memHandle, 0x28000, "CItemBoxGrid", 0);
    u8 regionBuf[0x14];
    __ct__14Class_8045F858FP17UnkClass_8045F564(regionBuf, p + 8);
    u32* fileData = *(u32**)(p + 0x28);
    void* fileBuf = (void*)fileData[1];
    fileData[1] = 0;
    setMemInitFlag__Q23mtl10MemManagerFb(0);
    mtl::ALLOC_HANDLE allocHandle = CLibLayout::getAllocHandle();
    void* tpMem = mtl::MemManager::allocate(0x858, allocHandle);
    if (tpMem != NULL) __ct__CTagProcessor(tpMem);
    *(void**)(p + 0x5c) = tpMem;
    nw4r::lyt::ArcResourceAccessor* arcRes = CLibLayout::createArcResourceAccessor();
    *(nw4r::lyt::ArcResourceAccessor**)(p + 0x3c) = arcRes;
    Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(arcRes, fileBuf, "arc");
    buildLayout((nw4r::lyt::Layout**)(p + 0x44), arcRes, "mf02_box01_box.brlyt");
    nw4r::lyt::Layout* layout = *(nw4r::lyt::Layout**)(p + 0x44);
    bindLayoutAnimTransform(layout, (nw4r::lyt::AnimTransform**)(p + 0x48), arcRes, (char*)"mf02_box01_box_in.brlan");
    bindLayoutAnimTransform(layout, (nw4r::lyt::AnimTransform**)(p + 0x4c), arcRes, (char*)"mf02_box01_box_in_info.brlan");
    bindLayoutAnimTransform(layout, (nw4r::lyt::AnimTransform**)(p + 0x50), arcRes, (char*)"mf02_box01_box_slct.brlan");
    // Pane lookups go through the layout's root pane.
    nw4r::lyt::Pane* paneHost = layout->GetRootPane();
    void* font = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, layout);
    void* fontData = ((IDeviceFontInfo*)font)->getFont();
    ApplyFontToTxtPanes(paneHost, (const nw4r::ut::Font*)((u32)fontData));
    u32 screenW = (u32)getPackedFont();
    setLayoutTextBoxFont(layout, (char*)"txt_pg00", screenW);
    setLayoutTextBoxFont(layout, (char*)"txt_pg01", screenW);
    u32 screenH = (u32)CUICfManager_getPackedFont9C();
    u8 i;
    for (i = 1; i <= 0x1e; i++) {
        char nameBuf[0x20];
        sprintf(nameBuf, (const char*)"txt_num%02d", i);
        setLayoutTextBoxFont(layout, nameBuf, screenH);
    }
    setLayoutTextBoxFont(layout, (char*)"txt_buyvalue02", screenH);
    // Initial animation visibility: only the 0x48 transform starts shown.
    (layout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)*(void**)(p + 0x4c), 0);
    (layout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)*(void**)(p + 0x50), 0);
    (layout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)*(void**)(p + 0x48), 1);
    layout->Animate(0);
    BuildSlotItemLists(0);
    // Bind the tag processor to the message pane.
    void* tpPane = paneHost->FindPaneByName((char*)"txt_comment", 1);
    if (tpPane != NULL) ((nw4r::lyt::TextBox*)tpPane)->SetTagProcessor((nw4r::ut::WideTagProcessor*)field_5C);
    // Window title / caption strings.
    char* msgA = BdatTouchStringCell("MNU_item", "name", 5);
    LayoutSetTextBoxFmtValue(layout, (char*)"txt_buyvalue01", msgA, 0);
    char* msgB = BdatTouchStringCell("MNU_item", "name", 4);
    LayoutSetTextBoxFmtValue(layout, (char*)"txt_soattit", msgB, 0);
    LayoutSetTextBoxFmtValue(layout, (char*)"txt_soat01", (char*)"", 0);
    // Mode-dependent help texture.
    const char* modeStr = "fileID_1";
    if (cf::CfGameManager::isClassicController(-1)) modeStr = "fileID_2";
    u16 msgKey = BdatGetU16ByTableKey("MNU_kyeassign", modeStr, 0x49);
    u32 msgId = (u32)MakeTplNameSysFile(msgKey);
    void* sysWin = CUICfManager_getArcResourceAccessor();
    void* texRes = ((nw4r::lyt::ArcResourceAccessor*)sysWin)->GetResource(0x74696D67, (const char*)msgId, 0);
    if (texRes != NULL) {
        PaneSetTexPaletteByName(layout, (const char*)"pic_soatbtn", (TPLPalette*)texRes);
        void* pane2 = paneHost->FindPaneByName((char*)"pic_soatbtn", 1);
        if (pane2 != NULL) {
            CibgTexMat* mat = *(CibgTexMat**)((u8*)texRes + 8);
            CibgTexData* tex = *(CibgTexData**)mat->field_00;
            u16 w = tex->field_02;
            u16 h = tex->field_00;
            ((nw4r::lyt::Pane*)pane2)->SetSize(nw4r::lyt::Size((float)(s32)w, (float)(s32)h));
        }
    }
    // Hide the icons that do not apply to this box mode.
    if (p[0x527] != 1) {
        nw4r::lyt::Pane* iconPane = paneHost->FindPaneByName((char*)"txt_buyvalue01", 1);
        setPaneVisible(iconPane, 0);
        if (p[0x527] != 2) {
            nw4r::lyt::Pane* iconPane2 = paneHost->FindPaneByName((char*)"txt_buyvalue02", 1);
            setPaneVisible(iconPane2, 0);
        }
    }
    // Snapshot the cursor pane's initial translation.
    void* posPane = paneHost->FindPaneByName((char*)"pic_pghole10", 1);
    copyVEC3(p + 0x534, (u8*)&((nw4r::lyt::Pane*)posPane)->GetTranslate());
    // Snapshot both cursor colour states into the global colour tables.
    GXColorS10 colA = PaneMatGetS16QuadByName(layout, "pic_tbbcof01", 0);
    CopyVec4s((GXColorS10*)&itemBoxGrid_slotLabelNeutralTop, &colA);
    GXColorS10 colB = PaneMatGetS16QuadByName(layout, "pic_tbbcof01", 1);
    CopyVec4s((GXColorS10*)&itemBoxGrid_slotLabelNeutralBottom, &colB);
    // Fan the two base colours' kind indices out over the eight table rows.
    s16 colAW = *(s16*)((u8*)&itemBoxGrid_slotLabelNeutralTop + 6);
    s16 colBW = *(s16*)((u8*)&itemBoxGrid_slotLabelNeutralBottom + 6);
    *(s16*)((u8*)&itemBoxGrid_slotLabelUpTop + 6) = colAW;
    *(s16*)((u8*)&itemBoxGrid_slotLabelUpBottom + 6) = colBW;
    *(s16*)((u8*)&itemBoxGrid_slotLabelDownTop + 6) = colAW;
    *(s16*)((u8*)&itemBoxGrid_slotLabelDownBottom + 6) = colBW;
    *(s16*)((u8*)&itemBoxGrid_slotValueNeutralTop + 6) = colAW;
    *(s16*)((u8*)&itemBoxGrid_slotValueNeutralBottom + 6) = colBW;
    *(s16*)((u8*)&itemBoxGrid_slotValueUpTop + 6) = colAW;
    *(s16*)((u8*)&itemBoxGrid_slotValueUpBottom + 6) = colBW;
    // Build each cursor from a stack temporary handed to the attach helper.
    *reinterpret_cast<CCur07*>(p + 0x70) = CCur07((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x3c));
    ((CBaseCur*)(p + 0x70))->initLayout();
    *reinterpret_cast<CCur09*>(p + 0x88) = CCur09((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x3c));
    ((CBaseCur*)(p + 0x88))->initLayout();
    // Cursor-09 move bounds.
    CibgVec3 vecMin;
    code80135FDC_setVec3((nw4r::math::VEC3*)((float*)&vecMin), -56.0f, -68.0f, 0.0f);
    CibgVec3 vecMax;
    code80135FDC_setVec3((nw4r::math::VEC3*)((float*)&vecMax), lbl_eu_80667F98, -68.0f, 0.0f);
    ((CCur09*)(p + 0x88))->SetTwoPanes(reinterpret_cast<const nw4r::math::VEC3*>(&vecMax),
                                       reinterpret_cast<const nw4r::math::VEC3*>(&vecMin));
    *reinterpret_cast<CCur18*>(p + 0xa0) = CCur18(CUICfManager_getArcResourceAccessor());
    ((CBaseCur*)(p + 0xa0))->initLayout();
    *reinterpret_cast<CCur16*>(p + 0xb8) = CCur16((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x3c));
    ((CBaseCur*)(p + 0xb8))->initLayout();
    *reinterpret_cast<CCur11*>(p + 0xd0) = CCur11((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x3c));
    ((CBaseCur*)(p + 0xd0))->initLayout();
    initPresentWin(p + 0x468, *(void**)(p + 0x3c));
    ((CItemBoxGridSubMenu*)(p + 0x418))->GridSubMenu_InitLayout(*(nw4r::lyt::ArcResourceAccessor**)(p + 0x3c));
    // Group colour snapshot for the grid cells.
    void* grpPane = paneHost->FindPaneByName((char*)"txt_num01", 1);
    if (grpPane != NULL) {
        GXColorS10 gcolA = PaneMatGetS16Quad((nw4r::lyt::Pane*)grpPane, 0);
        CopyVec4s((GXColorS10*)&itemBoxGrid_shopLabelNeutralTop, &gcolA);
        GXColorS10 gcolB = PaneMatGetS16Quad((nw4r::lyt::Pane*)grpPane, 1);
        CopyVec4s((GXColorS10*)&itemBoxGrid_shopLabelNeutralBottom, &gcolB);
        s16 gAW = *(s16*)((u8*)&itemBoxGrid_shopLabelNeutralTop + 6);
        s16 gBW = *(s16*)((u8*)&itemBoxGrid_shopLabelNeutralBottom + 6);
        *(s16*)((u8*)&itemBoxGrid_shopLabelSelectedTop + 6) = gAW;
        *(s16*)((u8*)&itemBoxGrid_shopLabelSelectedBottom + 6) = gBW;
        *(s16*)((u8*)&itemBoxGrid_shopValueSelectedTop + 6) = gAW;
        *(s16*)((u8*)&itemBoxGrid_shopValueSelectedBottom + 6) = gBW;
    }
    char* msgD = BdatTouchStringCell("MNU_item", "name", 7);
    LayoutSetTextBoxFmtValue(layout, (char*)"txt_button00", msgD, 0);
    if (p[0x527] != 4) {
        nw4r::lyt::Pane* iconPane3 = paneHost->FindPaneByName((char*)"nul_button", 1);
        setPaneVisible(iconPane3, 0);
        LayoutSetTextBoxFmtValue(layout, (char*)"txt_button00", (char*)"", 0);
    }
    // Mode 2 (party members): the first member with the relevant skill gets
    // a highlight fade-in driven by PaneMatSetTevColorsByName with inline colour blocks.
    if (p[0x527] == 2) {
        u8 cnt = code80135FDC_getByte_64077();
        u8 j;
        for (j = 0; j < cnt; j++) {
            void* chp = (u8*)CtrlObjectParam_GetCharEntry(GetCollectedFlagByte(j)) + 0x3534;
            if (!Counter_TestBit(chp, 0x8f) && !Counter_TestBit(chp, 0x93)) continue;
            GXColorS10 cA = PaneMatGetS16QuadByName(layout, "txt_buyvalue02", 0);
            GXColorS10 cB = PaneMatGetS16QuadByName(layout, "txt_buyvalue02", 1);
            struct FadeRefBlk {
                void* layout;
                const char* name;
            };
            struct ColPairBlk {
                u32 lo;
                u32 hi;
            };
            FadeRefBlk blk1 = { layout, "txt_buyvalue02" };
            FadeRefBlk blk2 = { layout, "txt_buyvalue02" };
            ColPairBlk cbCol;
            cbCol.lo = 0x0046B400;
            cbCol.hi = (u32)(u16)cB.a << 16;
            ColPairBlk caCol;
            caCol.lo = 0;
            caCol.hi = (u32)(u16)cA.a << 16;
            PaneMatSetTevColorsByName(layout, (char*)"txt_buyvalue02", (const GXColorS10*)&caCol, (const GXColorS10*)&cbCol);
            break;
        }
    }
    *(u32*)(p + 0x28) = 0;
    validateHeap__17UnkClass_8045F564Fv(p + 8);
    __dt__14Class_8045F858Fv(regionBuf, -1);
    return 1;
    }
    // Secondary arc resource load (+0x2c slot).
    if (*(u32*)(p + 0x2c) == *(u32*)((u8*)event + 4)) {
        void* memHandle = getHandleMEM2__Q23mtl10MemManagerFv();
        createRegion__17UnkClass_8045F564FiiPCci(p + 0x18, memHandle, 0x100, "CItemBoxGridTex", 0);
        u8 regionBuf[0x14];
        __ct__14Class_8045F858FP17UnkClass_8045F564(regionBuf, p + 0x18);
        u32* fileData = *(u32**)(p + 0x2c);
        void* fileBuf = (void*)fileData[1];
        fileData[1] = 0;
        setMemInitFlag__Q23mtl10MemManagerFb(0);
        nw4r::lyt::ArcResourceAccessor* arcRes = CLibLayout::createArcResourceAccessor();
        *(nw4r::lyt::ArcResourceAccessor**)(p + 0x40) = arcRes;
        Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(arcRes, fileBuf, "arc");
        *(u32*)(p + 0x2c) = 0;
        validateHeap__17UnkClass_8045F564Fv(p + 0x18);
        __dt__14Class_8045F858Fv(regionBuf, -1);
        return 1;
    }
    // BDAT table loads: register the fetched tables in their globals so the
    // item-box screens can look them up later.
    if (*(u32*)(p + 0x30) == *(u32*)((u8*)event + 4)) {
        u32* fileData = *(u32**)(p + 0x30);
        void* data = (void*)fileData[1];
        fileData[1] = 0;
        Bdat_GetTable_AA34();
        if (getFP("MNU_item_mes_a") == NULL)
            CBdat::setBdatEntry(2, data);
        itemBoxGrid_shopItemTypeTable = (u32)getFP("MNU_item_mes_a");
        itemBoxGrid_shopSkillTable = (u32)getFP("MNU_item_mes_b");
        *(u32*)(p + 0x30) = 0;
        return 1;
    }
    if (*(u32*)(p + 0x34) == *(u32*)((u8*)event + 4)) {
        u32* fileData = *(u32**)(p + 0x34);
        void* data = (void*)fileData[1];
        fileData[1] = 0;
        *(void**)(p + 0x38) = data;
        Bdat_GetTable_AA34();
        if (getFP("MNU_skill") == NULL)
            CBdat::setBdatEntry(5, *(void**)(p + 0x38));
        itemBoxGrid_shopItemIdTable = (u32)getFP("MNU_skill");
        *(u32*)(p + 0x34) = 0;
        return 1;
    }
    return 0;
}
// Static initialization of color table entries.
void sinit_801D1E30(void) {
    SplitU32ToS16s((short*)&itemBoxGrid_slotLabelNeutralTop, 0);
    SplitU32ToS16s((short*)&itemBoxGrid_slotLabelNeutralBottom, 0);
    setGXColorS10((GXColorS10*)&itemBoxGrid_slotLabelUpTop, 0x79, 0x49, 0x07, 0x00);
    setGXColorS10((GXColorS10*)&itemBoxGrid_slotLabelUpBottom, 0xED, 0xCD, 0x83, 0x00);
    setGXColorS10((GXColorS10*)&itemBoxGrid_slotLabelDownTop, 0x1A, 0x43, 0x53, 0x00);
    setGXColorS10((GXColorS10*)&itemBoxGrid_slotLabelDownBottom, 0xC4, 0xE8, 0xEB, 0x00);
    setGXColorS10((GXColorS10*)&itemBoxGrid_slotValueNeutralTop, 0x74, 0x54, 0x1D, 0x00);
    setGXColorS10((GXColorS10*)&itemBoxGrid_slotValueNeutralBottom, 0xD5, 0xB9, 0x78, 0x00);
    setGXColorS10((GXColorS10*)&itemBoxGrid_slotValueUpTop, 0x3D, 0x68, 0x78, 0x00);
    setGXColorS10((GXColorS10*)&itemBoxGrid_slotValueUpBottom, 0xC4, 0xE8, 0xEB, 0x00);
    SplitU32ToS16s((short*)&itemBoxGrid_shopLabelNeutralTop, 0);
    SplitU32ToS16s((short*)&itemBoxGrid_shopLabelNeutralBottom, 0);
    setGXColorS10((GXColorS10*)&itemBoxGrid_shopLabelSelectedTop, 0x12, 0xA3, 0xE7, 0x00);
    setGXColorS10((GXColorS10*)&itemBoxGrid_shopLabelSelectedBottom, 0xFF, 0xFF, 0xFF, 0x00);
    setGXColorS10((GXColorS10*)&itemBoxGrid_shopValueSelectedTop, 0xB3, 0x09, 0xC0, 0x00);
    setGXColorS10((GXColorS10*)&itemBoxGrid_shopValueSelectedBottom, 0xFF, 0xFF, 0xFF, 0x00);
}


