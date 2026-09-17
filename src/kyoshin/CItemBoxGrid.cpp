// Auto-scaffolded catalog TU for kyoshin/CItemBoxGrid
// Replace stubs with high-level C/C++ during decomp.

// Real item-impl owner (kyoshin/cf/CItem.hpp): the harness chain declares a
// stale extern "C" void* CItem_initItemImplInstances that conflicts with the
// real CItemImpl*(CItemData*) form, so it is renamed away for this TU (same
// workaround as CEquipChange.cpp).
#define CItem_initItemImplInstances CItem_initItemImplInstances_staleVoid
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CItemBoxGrid.hpp"
#include "kyoshin/CUIWindowManagerApi.hpp"
#include "kyoshin/CExchangeWin.hpp"
#include "kyoshin/CBaseCur.hpp"
#undef CItem_initItemImplInstances
// This TU declares six table handles + two helpers with stale types that
// clash with cf/CItem.hpp's decls; rename CItem.hpp's copies so the TU decls
// keep the retail names (same #define-rename pattern as CEquipChange.cpp).
#define lbl_eu_806640D8 lbl_eu_806640D8_citem_dup
#define lbl_eu_806640EC lbl_eu_806640EC_citem_dup
#define lbl_eu_806640F4 lbl_eu_806640F4_citem_dup
#define lbl_eu_806640F8 lbl_eu_806640F8_citem_dup
#define lbl_eu_80664104 lbl_eu_80664104_citem_dup
#define lbl_eu_80664110 lbl_eu_80664110_citem_dup
#define func_80157C4C func_80157C4C_grid_stale
#define func_801576C8 func_801576C8_grid_stale
#include "kyoshin/cf/CItem.hpp"
#undef lbl_eu_806640D8
#undef lbl_eu_806640EC
#undef lbl_eu_806640F4
#undef lbl_eu_806640F8
#undef lbl_eu_80664104
#undef lbl_eu_80664110
#undef func_80157C4C
#undef func_801576C8
// Real CSysWin owner for the +0x88 loadSystemArc dispatch: rename the five
// decls whose signatures clash with this TU's stale void*/int forms so the
// TU decls keep the retail names (identical decls need no guard).
#define __dt__17UnkClass_8045F564Fv __dt__17UnkClass_8045F564Fv_csys_dup
#define __ct__17UnkClass_8045F564Fv __ct__17UnkClass_8045F564Fv_csys_dup
#define func_80137924 func_80137924_csys_dup
#define code80135FDC_setVec3 code80135FDC_setVec3_csys_dup
#define func_8022BFC8 func_8022BFC8_csys_dup
#define func_8022B90C func_8022B90C_csys_dup
#include "kyoshin/CSysWin.hpp"
#undef __dt__17UnkClass_8045F564Fv
#undef __ct__17UnkClass_8045F564Fv
#undef func_80137924
#undef code80135FDC_setVec3
#undef func_8022BFC8
#undef func_8022B90C
#include "monolib/device/CDeviceFont.hpp"
#include <nw4r/ut/ut_TagProcessorBase.h>
#include <stdio.h>
#include <string.h>
#include <nw4r/lyt.h>

// Forward declarations for external functions
void* __dt__13CArtsBookItemFv(void* self, int mode);
void* __dt__11CVisionItemFv(void* self, int mode);
void* __dt__10CQuestItemFv(void* self, int mode);

void setLayoutTextBoxNumber(nw4r::lyt::Layout*, char*, u8);

// Forward declarations for functions defined later in this file
u32 func_801C7958(void* self, void* item);
u32 HasEquippedSlot(void* self, void* item);
extern "C" u32 FindKindPoolIdx(void* self, void* entry);
s32 func_801C7C7C(void* self, u32 id, void* item);
extern "C" u32 FormatCellName(void* self, u32 idx);
extern "C" long RoundHalfAway0(double f);
extern "C" u8 GetCellFlag0(CItemBoxGridFull* self, u16 idx);
extern "C" u8 GetCellFlag2(CItemBoxGridFull* self, u16 idx);
extern "C" u8 GetCellFlag3(CItemBoxGridFull* self, u16 idx);
extern "C" u8 GetCellByte7(CItemBoxGridFull* self, u16 idx);
extern "C" u8 GetCellByte8(CItemBoxGridFull* self, u16 idx);
extern "C" __declspec(noinline) u8 GetCategoryCap(CItemBoxGridFull* self);
extern "C" __declspec(noinline) u8 CountMarkedCells(CItemBoxGridFull* self);
extern "C" u32 func_801C5FC0(CItemBoxGridFull* self, u16 idx);  // retail symbols are unmangled
extern "C" s32 CheckCellState(CItemBoxGridFull* self, u16 idx);
u32 func_801C618C(void* self, u32 id, void* item, int r6);
void func_801C6EC0(CItemBoxGridFull* self, u16 idx);
int LookupIndexedByte(char* obj);
extern "C" {
void NextGridPage(void*);
void PrevGridPage(void*);
void OpenSortMenu(void*);
void HandleCancelBtn(void*, int);
void CopyTabSlotVec(u32*, void*, u32);
void AdvanceOpenAnim(void*);
void FinishOpenAnim(void*);
void CheckState4_Animate(char*);
void AdvanceCloseAnim(void*);
void FinishNumSelect(void*);
void ShowSubMenuCur(void*);
void func_801CE524(void*);
void ShowExchangeCur(void*);
void FinishExchange(void*);
void ShowPresentCur(void*);
void FinishPresent(void*);
void OnSysWinOpen(void*);
void HandleSysWinOK(void*);
void FinishSysWin(void*);
void SetGridReady(void*);
void SetInfoMsgId(void*, u32);
}
void CopyTabSlotVec(u32*, void*, u32);
void func_801CBA04(void*);
void getEntry__5CBdatFUl(u32);
void closeFileHandle__FPP11CFileHandle(void**);
void releaseArcResourceAccessor__FPQ34nw4r3lyt19ArcResourceAccessor(void*);
void deleteRegion__17UnkClass_8045F564Fv(void*);
void func_8022B7F4(void*);
extern "C" void ShowCellIcon(CItemBoxGridFull* self, u32 kind, void* item, u16 idx, u32 bt);
extern "C" void ShowCellBadge(CItemBoxGridFull* self, u32 kind, void* item, u16 idx);
extern "C" void SetCellCaption(void* self, s8 kind, void* item, u16 idx);
extern "C" void ShowCellCount(void* self, u32 r4, void* r5, void* r6, u32 r7);
extern "C" void SetCellMarkVis(void* self, u32 val, u32 idx);
extern "C" u32 CalcGridRowCount(void* self);
char* func_801C6A44(void* self, u16 idx);
// .sdata2 conversion magics referenced by func_801C9F88 (retail pool
// constants; F48 = signed-int magic 0x4330000080000000, F40 = plain 2^52).
extern const double lbl_eu_80667F40;
extern const double lbl_eu_80667F48;
extern const float lbl_eu_80667F80;
extern const float lbl_eu_80667F78;
extern const float lbl_eu_80667F30;
extern const float lbl_eu_80667F34;
extern const float lbl_eu_80667F38;
extern const float lbl_eu_80667F50;
extern const float lbl_eu_80667F54;
extern const float lbl_eu_80667F58;
extern const float lbl_eu_80667F74;
extern const float lbl_eu_80667F7C;
extern const float lbl_eu_80667F88;
extern const float lbl_eu_80667F8C;
extern const float lbl_eu_80667F90;
extern const float lbl_eu_80667F94;
extern const float lbl_eu_80667F98;
extern const float lbl_eu_80667F84;
extern const double lbl_eu_80667F60;
extern const double lbl_eu_80667F68;

extern u32 lbl_eu_80664098;
extern u32 lbl_eu_806640D8;
extern u32 lbl_eu_806640EC;

extern u32 lbl_eu_806640F8;
extern u32 lbl_eu_80664104;
extern u32 lbl_eu_80664110;
extern u32 lbl_eu_80664488;
extern u32 lbl_eu_80664490;
extern u32 lbl_eu_80664498;
extern u32 lbl_eu_806644A0;
extern u32 lbl_eu_806644A8;
extern u32 lbl_eu_806644B0;
extern u32 lbl_eu_806644B8;
extern u32 lbl_eu_806644C0;
extern u32 lbl_eu_806644C8;
extern u32 lbl_eu_806644D0;
extern u32 lbl_eu_806644D8;
extern u32 lbl_eu_806644E0;
extern u32 lbl_eu_806644E8;
extern u32 lbl_eu_806644F0;
extern u32 lbl_eu_806644F8;
extern u32 lbl_eu_80664500;
extern u32 lbl_eu_80664508;
extern u32 lbl_eu_8066450C;
extern u32 lbl_eu_80664510;

extern const char lbl_eu_80505590[];
extern const char lbl_eu_805055F0[];
extern const char lbl_eu_8050560C[];
extern const char lbl_eu_80505628[];
extern const char lbl_eu_80534740[];
extern const char lbl_eu_805347D8[];
extern const char lbl_eu_80534818[];
extern const char lbl_eu_80573D18[];
extern const char lbl_eu_8050566C[];
extern u32 lbl_eu_80664514;
// 4-byte .sbss pointer (symbols.txt size:0x4). A large u8[] decl forces
// lis/addi; void* keeps it SDA-eligible (EMB_SDA21 lwz like retail).
extern void* lbl_eu_806640F4;
// Forward declarations for later-defined functions
void NextGridPage(void*);
void PrevGridPage(void*);
void OpenSortMenu(void*);
void HandleCancelBtn(void*, int);
void CopyTabSlotVec(u32*, void*, u32);
void CheckState4_Animate(char*);
void AdvanceOpenAnim(void*);
void FinishOpenAnim(void*);
void AdvanceCloseAnim(void*);
void FinishNumSelect(void*);
void ShowSubMenuCur(void*);
void func_801CE524(void*);
void ShowExchangeCur(void*);
void FinishExchange(void*);
void ShowPresentCur(void*);
void FinishPresent(void*);
void OnSysWinOpen(void*);
void HandleSysWinOK(void*);
void FinishSysWin(void*);
void SetGridReady(void*);
void SetInfoMsgId(void*, u32);
void func_801D0E88(void*, int, int);


// Externs for LoadItemBoxFiles and UnloadItemBox

// Externs for state dispatch functions

u8 CItemBoxGrid::GetField61() { return reinterpret_cast<CItemBoxGridFull*>(this)->bytes.field_61; }

// Constructor for sub-item grid: blank every cell, set metadata and the
// vtable at +0x2CA8, then stamp the pre-cleared 0x400-cell template over the
// grid via CopyGridEntry9 (retail keeps both helper calls as `bl`).
#pragma optimize_for_size on
extern "C" __declspec(noinline) void* __ct__801C5514(void* self) {
    u8* end = (u8*)self + 0x2800;
    u8* cur = (u8*)self;
    do {
        SetEntry9Bytes(cur, -1, 0, 0, 0, 0, 0, 0, 0);
        cur += 10;
    } while (cur < end);
    u16 z = 0;   // separate vreg: retail reuses end's r31 for the post-loop zeros
    *(u16*)((u8*)self + 0x2800) = z;
    ((u8*)self)[0x2802] = (u8)z;
    ((u8*)self)[0x2803] = (u8)z;
    ((u8*)self)[0x2804] = (u8)z;
    *(u32*)((u8*)self + 0x2CA8) = (u32)&lbl_eu_80534818;
    *(u16*)((u8*)self + 0x34AC) = z;
    __ct__CVisionItem((u8*)self + 0x34B0);
    __ct__CArtsBookItem((u8*)self + 0x3CB8);
    lbl_eu_80664514 = (u32)self;
    {
        u16 i;
        for (i = 0; i < 0x400; i++) {
            u8 tmp[9];
            CopyGridEntry9((u8*)self + i * 10, (char*)SetEntry9Bytes(tmp, -1, 0, 0, 0, 0, 0, 0, 0));
        }
    }
    return self;
}
#pragma optimize_for_size off

__declspec(noinline) unsigned char* SetEntry9Bytes(unsigned char* p, short a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f, unsigned char g, unsigned char h) {
    unsigned char* buf = (unsigned char*)p;
    *((unsigned short*)(buf + 0)) = a;
    buf[2] = b;
    buf[3] = c;
    buf[4] = d;
    buf[5] = e;
    buf[6] = f;
    buf[7] = g;
    buf[8] = h;
    return buf;
}

// Copy a 9-byte entry from src to dst. noinline: retail callers emit `bl`
// (the unit builds with -inline and would otherwise fold this body in).
extern "C" __declspec(noinline) void CopyGridEntry9(void* dst, void* src) {
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

// Destructor for the main grid container.
// optimize_for_size: retail uses stmw r30/lmw r30 block save (not individual stw).
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void* __dt__801C5670(void* self, int mode) {
    u8* p = (u8*)self;
    if (!self) goto exit;
    lbl_eu_80664514 = 0;
    __dt__13CArtsBookItemFv(p + 0x3cb8, 0xFFFFFFFF);
    __dt__11CVisionItemFv(p + 0x34b0, 0xFFFFFFFF);
    if (mode > 0) __dl__FPv(self);
exit:
    return self;
}
#pragma pop


// C linkage: retail exports this helper under its plain address-name symbol.
// C linkage: retail exports this helper under its plain address-name symbol.
extern "C" void func_801C56D8(CItemBoxGridFull* self, u8 cat, int r5, int r6, int r7) {
    u8* p = (u8*)self;
    u32 i;
    *(u16*)(p + 0x2800) = 0;
    // Reset the three sub-objects through their vtables (+0x0C method).
    ((CVisionItem*)(p + 0x34b0))->Rebuild();
    ((CQuestItem*)(p + 0x2ca8))->Rebuild();
    ((CArtsBookItem*)(p + 0x3cb8))->Rebuild();
    // Clear all 0x400 cells to blank entries and stamp the hidden-flag row.
    struct { u32 pad; u8 buf[9]; } tmp;
    for (i = 0; i < 0x400; i++) {
        tmp.pad = 0;
        SetEntry9Bytes(tmp.buf, -1, 0, 0, 0, 0, 0, 0, 0);
        CopyGridEntry9(p + i * 10, tmp.buf);
        p[i * 10 + 0x28a5] = 3;
    }
    u32 total = CItemBlock_countKindSlots(cat);
    // Read 24 bytes from lbl_eu_80505590 (6 category filter entries)
    GridFilter24 filterBuf = *(const GridFilter24*)&lbl_eu_80505590;
    u16 id;
    for (id = 0; (u32)id < total; id++) {
        void* item = func_80157C4C(cat, id);
        if (!item) continue;
        if (!*(u32*)item) continue;
        if (r6) {
            u32 v1 = func_801C618C((CItemBoxGridFull*)self, r6, 0, (int)r7);
            u32 kind = (*(u32*)item) >> 20;
            u32 v2 = func_801C618C((CItemBoxGridFull*)self, kind, item, (int)r7);
            if (v1 > v2) continue;
        }
        u16 count = *(u16*)(p + 0x2800);
        u32 off = count * 10;
        *(u16*)(p + 0x2800) = count + 1;
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
                    void* subItem = func_80157C4C(subCat, subId);
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
                            void* obj2 = (void*)func_8009EC9C(pn & 0xFF);
                            s16 equipCheck = 0;
                            switch (subCat) {
                                case 2: equipCheck = *(s16*)((u8*)obj2 + 0x26); break;
                                case 4: equipCheck = *(s16*)((u8*)obj2 + 0x1c); break;
                                case 5: equipCheck = *(s16*)((u8*)obj2 + 0x1e); break;
                                case 6: equipCheck = *(s16*)((u8*)obj2 + 0x20); break;
                                case 7: equipCheck = *(s16*)((u8*)obj2 + 0x22); break;
                                case 8: equipCheck = *(s16*)((u8*)obj2 + 0x24); break;
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
            if (r5) {
                u32 ownerTbl;
                switch (cat) {
                    case 2: ownerTbl = (u32)lbl_eu_806640F4; break;
                    default: ownerTbl = (u32)lbl_eu_806640F8; break;
                }
                u32 kindId = (*(u32*)item) >> 20;
                u32 shortKind = BdatGetItemId(kindId);
                char nameBuf[64];
                const char* fmt = (cat == 2) ? &lbl_eu_8050566C[0x132] : &lbl_eu_8050566C[0x13d];
                sprintf(nameBuf, fmt, r5);
                if (!BdatGetU8Direct(ownerTbl, nameBuf, shortKind & 0xFFFF)) {
                    *(s16*)entry = -1;
                    count = *(u16*)(p + 0x2800);
                    *(u16*)(p + 0x2800) = count - 1;
                    goto check_next;
                }
            }
            u8 partyIdx;
            for (partyIdx = 0; partyIdx < (u8)code80135FDC_getByte_64077(); partyIdx++) {
                u32 pn = GetCollectedFlagByte(partyIdx);
                void* obj3 = (void*)func_8009EC9C(pn & 0xFF);
                s16 equipCheck = 0;
                switch (cat) {
                    case 2: equipCheck = *(s16*)((u8*)obj3 + 0x26); break;
                    case 4: equipCheck = *(s16*)((u8*)obj3 + 0x1c); break;
                    case 5: equipCheck = *(s16*)((u8*)obj3 + 0x1e); break;
                    case 6: equipCheck = *(s16*)((u8*)obj3 + 0x20); break;
                    case 7: equipCheck = *(s16*)((u8*)obj3 + 0x22); break;
                    case 8: equipCheck = *(s16*)((u8*)obj3 + 0x24); break;
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
                u32 check = BdatGetU8Direct(lbl_eu_806640EC, &lbl_eu_8050566C[0x142], kind2);
                u32 result = check;
                u32 oneIfNonZero = (result != 0) ? 1 : 0;
                entry[3] = (u8)oneIfNonZero;
            }
            u32 kind3 = (*(u32*)item) >> 20;
            entry[5] = (u8)HasSlotIdMatch(p + 0x34b0, kind3);
            entry[4] = (u8)HasSlotIdMatch(p + 0x2ca8, kind3);
            if (cat == 0xd) {
                u32 shortKind = BdatGetItemId(kind3);
                entry[6] = (u8)HasSlotIdMatch(p + 0x3cb8, shortKind & 0xFFFF);
            }
        }
check_next:;
    }
    // Update grid metadata
    if (*(u8*)(p + 0x2802) != cat) {
        u32 cnt = *(u16*)(p + 0x2800);
        *(u8*)(p + 0x2802) = cat;
        // Compute row count: cnt / 30, rounded up with tie-breaking
        float fcnt = (float)(s32)cnt;
        float f30 = lbl_eu_80667F30;
        float fdiv = fcnt / f30;
        int rows = (int)fdiv;
        float frac = fdiv - (float)rows;
        if (lbl_eu_80667F34 != frac) rows++;
        if (!(u8)rows) rows = 1;
        *(u8*)(p + 0x2803) = (u8)rows;
        *(u8*)(p + 0x2804) = 0;
    } else {
        u32 cnt = *(u16*)(p + 0x2800);
        float fcnt = (float)(s32)cnt;
        float f30 = lbl_eu_80667F30;
        float fdiv = fcnt / f30;
        int rows = (int)fdiv;
        float frac = fdiv - (float)rows;
        if (lbl_eu_80667F34 != frac) rows++;
        if (!(u8)rows) rows = 1;
        *(u8*)(p + 0x2803) = (u8)rows;
        if ((s8)*(u8*)(p + 0x2804) >= (s8)*(u8*)(p + 0x2803)) {
            *(u8*)(p + 0x2804) = *(u8*)(p + 0x2803) - 1;
        }
    }
    // For equipment/armor categories (2-8) or category 11: set overlay byte
    u8 adjusted = (u8)(cat + 0xfe);
    if (adjusted <= 6 || cat == 0xb) {
        u32 cnt = CItemBlock_countKindSlots(cat);
        u32 base = func_801576C8(cat);
        s32 last = (s32)cnt - 1;
        s32 diff = (s32)cnt - (s32)base;
        s32 k;
        if (last >= diff) {
            for (k = last; k >= diff; k--) {
                p[k * 10 + 7] = 1;
            }
        }
        // Compute some threshold
        float fcnt2 = (float)(s32)cnt;
        float fmul = lbl_eu_80667F38 * fcnt2;
        int threshold = (int)fmul;
        u32 j = 0;
        for (;;) {
            int diff2 = threshold - (j + 1) * 3;
            if (diff2 < 0) {
                p[j + 0x28a5] = (u8)(diff2 + 3);
                break;
            }
            j++;
        }
    }
    func_801C6EC0((CItemBoxGridFull*)self, 0);
}

// Search for a matching short id in an array, return 1 if found.
// noinline: retail func_801C5158 keeps a `bl HasSlotIdMatch` call; u32 id
// (retail compares the full register, no call-site mask).
extern "C" __declspec(noinline) int HasSlotIdMatch(void* obj, u32 id) {
    u16 count = *(u16*)((u8*)obj + 0x804);
    u16 i = 0;
    while (i < count) {
        if (*(u16*)((u8*)obj + i * 2 + 4) == id) return 1;
        i++;
    }
    return 0;
}

// Convert category to timing value - complex float math
// Compute grid row count.
extern "C" __declspec(noinline) u32 CalcGridRowCount(void* self) {
    u8* p = (u8*)self;
    u32 val = CItemBlock_countKindSlots(p[0x2802]);
    float f = (float)(s32)val;
    if (f >= lbl_eu_80667F50) return 10;
    float div = f / lbl_eu_80667F30;
    s32 result = (s32)div;
    float frac = div - (float)result;
    if (lbl_eu_80667F34 != frac) result++;
    return (u8)result;
}

// Increment a sub-index counter; wrap to 0 when reaching the limit.
// noinline: retail callers emit `bl` (the unit builds with -inline).
extern "C" __declspec(noinline) void IncGridPageIndex(CItemBoxGridFull* self) {
    u8 idx = self->field_2804 + 1;
    self->field_2804 = idx;
    if ((s8)idx >= self->field_2803) {
        self->field_2804 = 0;
    }
}

// Decrement a sub-index counter; borrow from the limit field when underflowing.
// noinline: retail callers emit `bl` (the unit builds with -inline).
extern "C" __declspec(noinline) void DecGridPageIndex(CItemBoxGridFull* self) {
    u8 idx = self->field_2804 - 1;
    self->field_2804 = idx;
    if ((s8)idx < 0) {
        self->field_2804 = self->field_2803 - 1;
    }
}

// Lookup the item id stored in a grid cell; -1 when the cell is out of
// range or the referenced item object is invalid.
#pragma push
#pragma optimize_for_size on
extern "C" s16 GetCellItemId(CItemBoxGridFull* self, u16 idx) {
    s8 base = (s8)self->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset < self->field_2800) {
        u8* obj = (u8*)func_80157C4C(self->field_2802, self->entries[offset].id);
        if (obj && *(u32*)obj) {
            return self->entries[offset].id;
        }
    }
    return -1;
}
#pragma pop

// Item sell price for the cell at page*0x1e+idx: 0 when the cell is out of
// range or the referenced item object is invalid. Otherwise the item's base
// price (func_801C7958) scaled by kind type (0xB unchanged, 0xD half, else
// tenth), then multiplied by 100 + the party's skill-0x93 bonus and rounded.
extern "C" __declspec(noinline) u32 func_801C5FC0(CItemBoxGridFull* self, u16 idx) {
    s8 base = (s8)self->field_2804;
    u32 offset = (u16)(base * 0x1e + idx);
    if (offset >= (u32)self->field_2800) return 0;

    void* obj = func_80157C4C(self->field_2802, self->entries[offset].id);
    if (!obj || !*(u32*)obj) return 0;

    u32 kind = *(u32*)obj >> 20;
    func_801393CC(kind);
    u32 type = BdatGetItemType(kind);
    BdatGetItemId(kind);
    u32 price = func_801C7958(self, obj);

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
        void* charData = (void*)func_8009EC9C((u32)(u8)member);
        void* ptr = (void*)((u8*)charData + 0x3534);
        if (func_8026178C(ptr, 0x93)) {
            basePrice += func_8025FB10(ptr, 0x93);
        }
    }

    return (u32)RoundHalfAway0(0.01f * (double)(price * basePrice));
}

// Round a double to nearest integer with .5 tie-breaking biased away from zero.
extern "C" __declspec(noinline) long RoundHalfAway0(double f) {
    return (long)(f + (f > lbl_eu_80667F34 ? lbl_eu_80667F60 : lbl_eu_80667F68));
}

// Item value: base price from func_801C7C7C, then scaled by the party's
// equipment-skill bonus when a concrete item (not a bare kind) is passed.
// The category is a conditional dereference: retail keeps `obj = item ?
// item : 0` and re-tests item for the load, so both branches stay separate.
u32 func_801C618C(void* self, u32 id, void* item, int r6) {
    void* obj = item ? item : 0;
    u32 cat;
    if (item) {
        cat = *(u32*)obj >> 20;
    } else {
        cat = id;
    }
    func_801393CC(cat & 0xFFFF);
    BdatGetItemType(cat & 0xFFFF);
    BdatGetItemId(cat & 0xFFFF);
    u32 result = func_801C7C7C(self, cat & 0xFFFF, item);

    if (item) {
        u32 base = 0x64;
        u8 partyCount = code80135FDC_getByte_64077();
        u8 i;
        for (i = 0; i < partyCount; i++) {
            u8 member = (u8)GetCollectedFlagByte(i);
            void* charData = (void*)func_8009EC9C(member);
            void* ptr = (void*)((u8*)charData + 0x3534);
            if (func_8026178C(ptr, 0x8f)) {
                base += func_8025FB10(ptr, 0x8f);
            }
        }
        result = (u32)RoundHalfAway0((float)(result * base) * 0.01f);
    }

    return result;
}

// Lookup entry, check category; return word >> 20 or 0.
extern "C" __declspec(noinline) u32 GetCellKind(CItemBoxGridFull* self, u16 idx) {
    s8 base = (s8)self->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset < self->field_2800) {
        s16 val = *(s16*)((u8*)self + offset * 0xa);
        void* obj = func_80157C4C(self->field_2802, val);
        if (obj) {
            u32 w = *(u32*)obj;
            if (w) return w >> 20;
        }
    }
    return 0;
}

// Lookup entry, check category; return obj ptr or 0.
extern "C" __declspec(noinline) void* GetCellItemPtr(CItemBoxGridFull* self, u16 idx) {
    s8 base = (s8)self->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset < self->field_2800) {
        s16 val = *(s16*)((u8*)self + offset * 0xa);
        void* obj = func_80157C4C(self->field_2802, val);
        if (obj && *(u32*)obj) {
            return obj;
        }
    }
    return 0;
}

// Check item entry validity based on category. Categories 4-8 and 2 share
// the equipment-slot scan (slot count via vt+0x30, free-slot probe via
// vt+0x40, equipped-state fetch via vt+0x2C); category 3 collapses the cell's
// state byte to a boolean. optimize_for_size merges the r29-r31 saves into
// retail's stmw prologue; all failure paths share one return-0 block at end.
#pragma push
#pragma optimize_for_size on
extern "C" __declspec(noinline) s32 CheckCellState(CItemBoxGridFull* self, u16 idx) {
    s8 base = (s8)self->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset >= self->field_2800) goto fail;

    CItemBoxGridEntry* entry = &self->entries[offset];
    void* obj = func_80157C4C(self->field_2802, entry->id);
    if (!obj || !*(u32*)obj) goto fail;

    // Category dispatch: the equipment scan is shared by cats 2 and 4-8;
    // cats 9 and default share the plain-fail value.
    switch (self->field_2802) {
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
        return (s8)((u8*)obj)[6];
    default:
        goto fail;
    }
fail:
    return 0;
}
#pragma pop

// Lookup entry, check category; return flag byte 2 (equip kind) or 0.
extern "C" __declspec(noinline) u8 GetCellFlag0(CItemBoxGridFull* self, u16 idx) {
    s8 base = (s8)self->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset < self->field_2800) {
        CItemBoxGridEntry* entry = &self->entries[offset];
        u8* obj = (u8*)func_80157C4C(self->field_2802, entry->id);
        if (obj && *(u32*)obj) {
            return entry->flags[0];
        }
    }
    return 0;
}

// Lookup entry, check category; return flag byte 4 or 0.
extern "C" __declspec(noinline) u8 GetCellFlag2(CItemBoxGridFull* self, u16 idx) {
    s8 base = (s8)self->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset < self->field_2800) {
        CItemBoxGridEntry* entry = &self->entries[offset];
        u8* obj = (u8*)func_80157C4C(self->field_2802, entry->id);
        if (obj && *(u32*)obj) {
            return entry->flags[2];
        }
    }
    return 0;
}

// Lookup entry, check category; return flag byte 5 or 0.
extern "C" __declspec(noinline) u8 GetCellFlag3(CItemBoxGridFull* self, u16 idx) {
    s8 base = (s8)self->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset < self->field_2800) {
        CItemBoxGridEntry* entry = &self->entries[offset];
        u8* obj = (u8*)func_80157C4C(self->field_2802, entry->id);
        if (obj && *(u32*)obj) {
            return entry->flags[3];
        }
    }
    return 0;
}

// Lookup entry, check category; return flag byte 3 or 0.
extern "C" u8 GetCellFlag1(CItemBoxGridFull* self, u16 idx) {
    s8 base = (s8)self->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset < self->field_2800) {
        CItemBoxGridEntry* entry = &self->entries[offset];
        u8* obj = (u8*)func_80157C4C(self->field_2802, entry->id);
        if (obj && *(u32*)obj) {
            return entry->flags[1];
        }
    }
    return 0;
}

// Lookup a byte from a 10-byte-entry table indexed by (field_2804 * 0x1e + idx).
// Returns byte at offset 7 within the entry, or 0 if out of bounds.
extern "C" __declspec(noinline) u8 GetCellByte7(CItemBoxGridFull* self, u16 idx) {
    s32 tmp = (s8)self->field_2804 * 0x1e + idx;
    u32 offset = (u32)(u16)tmp;
    if (offset < 0x400) {
        return ((u8*)self)[offset * 0xa + 7];
    }
    return 0;
}

extern "C" __declspec(noinline) u8 GetCellByte8(CItemBoxGridFull* self, u16 idx) {
    s32 tmp = (s8)self->field_2804 * 0x1e + idx;
    u32 offset = (u32)(u16)tmp;
    if (offset < 0x400) {
        return ((u8*)self)[offset * 0xa + 8];
    }
    return 0;
}


// Toggle an entry's flag based on category cap.
#pragma optimize_for_size on
extern "C" void ToggleCellMark(CItemBoxGridFull* self, u16 idx) {
    u16 offset = (u16)((s8)self->field_2804 * 0x1e + idx);
    if (offset >= 0x400) return;
    u8 cap;
    u8* entry = (u8*)self + offset * 0xa;
    if (entry[8] == 0) {
        cap = (u8)GetCategoryCap(self);
        if (CountMarkedCells(self) >= cap) return;
    }
    entry[8] = (u8)((entry[8] ^ 1) != 0);
}
#pragma optimize_for_size off

// Count entries with non-zero byte at offset 8 in a 10-byte stride array.
extern "C" __declspec(noinline) u8 CountMarkedCells(CItemBoxGridFull* self) {
    u16 count = self->field_2800;
    u16 result = 0;
    u16 i;
    for (i = 0; i < count; i++) {
        u8* entry = (u8*)self + i * 0xa;
        if (entry[8] != 0) {
            result++;
        }
    }
    return (u8)result;
}

// Return a duration/stride value based on the category byte at offset 0x2802.
extern "C" __declspec(noinline) u8 GetCategoryCap(CItemBoxGridFull* self) {
    int cat = self->field_2802;
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

__declspec(noinline) int LookupIndexedByte(char* obj) {
    char off = *(signed char*)((char*)obj + 0x2804);
    if (off >= 0x400) return 0;
    return *(unsigned char*)((char*)obj + off + 0x28a5);
}

// Iterate entries and init item instances.
#pragma optimize_for_size on
extern "C" void InitGridItems(CItemBoxGridFull* self) {
    u16 i;
    for (i = 0; i < self->field_2800; i++) {
        CItemBoxGridEntry* entry = &self->entries[i];
        if (entry->flags[6] != 0) continue;
        void* obj = func_80157C4C(self->field_2802, entry->id);
        if (!obj || !*(u32*)obj) continue;
        CItemImpl* inst = CItem_initItemImplInstances((CItemData*)obj);
        inst->vf10((CItemData*)obj);
    }
}
#pragma optimize_for_size off

// Format the item name of the cell at page*0x1e+idx into the buffer at
// +0x2805 and return it (0 when the cell is empty/invalid). optimize_for_size
// merges the r29-r31 saves into retail's stmw prologue; the shared fail
// block reproduces retail's single return-0 fall-through into the epilogue.
#pragma optimize_for_size on
extern "C" u32 FormatCellName(void* self, u32 idx) {
    u8* p = (u8*)self;
    s8 base = (s8)p[0x2804];
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset >= *(u16*)(p + 0x2800)) goto fail;

    u8 cat = p[0x2802];
    s16 val = *(s16*)(p + offset * 0xa);
    void* obj = func_80157C4C(cat, val);
    if (!obj || !*(u32*)obj) goto fail;

    CItemImpl* inst = CItem_initItemImplInstances((CItemData*)obj);
    const char* name = inst->vf20((CItemData*)obj);

    sprintf((char*)(p + 0x2805), (const char*)&lbl_eu_8050566C[0x14c], name);

    if (p[0x2802] == 3) {
        CItemImpl* inst2 = CItem_initItemImplInstances((CItemData*)obj);
        u32 count = inst2->vf08((CItemData*)obj);
        u8 b = (u8)count;
        u32 r = (u32)BdatTouchStringCell((void*)&lbl_eu_8050566C[0x14f], &lbl_eu_8050566C[0x158], (u32)(0x1e - (b - 1)));
        sprintf((char*)(p + 0x2805), (const char*)&lbl_eu_8050566C[0x15d], (char*)(p + 0x2805), r);
    }

    return (u32)(p + 0x2805);
fail:
    return 0;
}
#pragma optimize_for_size off

// Format the name of the item at page*0x1e+idx into the buffer at +0x2825
// and return it (0 when the cell is empty/invalid). The category-3 path
// post-processes the name string in place: `$1`/`$2` tags are replaced with
// per-language slot text (a 7-word table from lbl_eu_805055F0 is copied to
// the stack and indexed by the item's vtable+0x08 byte).
#pragma optimize_for_size on
char* func_801C6A44(CItemBoxGridFull* self, u16 idx) {
    // Stack slots follow retail: word table @0x08, tag buffers @0x28/0x48/0x68.
    u32 wordTbl[7];
    char buf28[32];
    char buf48[32];
    char buf68[32];
    u8* p = (u8*)self;
    s8 base = (s8)p[0x2804];
    u16 offset = (u16)(base * 0x1e + idx);
    void* item;

    if (offset >= *(u16*)(p + 0x2800)) goto fail;

    item = func_80157C4C(p[0x2802], *(s16*)(p + offset * 0xa));
    if (!item) goto fail;
    if (!*(u32*)item) goto fail;

    if (IsSkillItem(item)) {
        CItemImpl* inst = CItem_initItemImplInstances((CItemData*)item);
        u32 v = inst->vf80((CItemData*)item);
        int sel = 0x9d;
        if ((u16)v != 0) sel = 0x9e;
        char* msg = BdatTouchStringCell((void*)&lbl_eu_8050566C[0x14f], (void*)&lbl_eu_8050566C[0x158], sel);
        sprintf((char*)(p + 0x2825), (const char*)&lbl_eu_8050566C[0x14c], msg);
    } else if (((*(u32*)item >> 12) & 0xF) == 9 && ((((u8*)item)[7] & 3) == 3)) {
        char* msg = BdatTouchStringCell((void*)&lbl_eu_8050566C[0x14f], (void*)&lbl_eu_8050566C[0x158], 0x9c);
        sprintf((char*)(p + 0x2825), (const char*)&lbl_eu_8050566C[0x14c], msg);
    } else if (p[0x2802] == 3) {
        u32 tbl = lbl_eu_806640D8;
        CItemImpl* inst1 = CItem_initItemImplInstances((CItemData*)item);
        u16 slotStatus = CItem_initItemImplInstances((CItemData*)item)->vf54((CItemData*)item);
        CItemImpl* inst2 = CItem_initItemImplInstances((CItemData*)item);
        u8 slotByte = (u8)inst2->vf08((CItemData*)item);
        CItemImpl* inst3 = CItem_initItemImplInstances((CItemData*)item);
        char* name90 = (char*)inst3->vf90((CItemData*)item);
        char buf48[32];
        sprintf(buf48, (const char*)&lbl_eu_8050566C[0x162], name90);
        u32 nameKey = BdatGetU8Direct(tbl, (const char*)&lbl_eu_8050566C[0x165], (u16)slotStatus);
        char* desc = BdatGetPtrDirect((const void*)tbl, &lbl_eu_8050566C[0x16e], (u16)slotStatus);
        sprintf((char*)(p + 0x2825), (const char*)&lbl_eu_8050566C[0x14c], desc);

        int langFlag = 1;
        if ((u8)getLanguage__9CDeviceSCFv() != 3) {
            if ((u8)getLanguage__9CDeviceSCFv() != 2) langFlag = 0;
        }

        // Retail keeps the slot-byte word offset and both pool bases in
        // non-volatile registers across the whole scan loop.
        {
        u32 langIdx = slotByte << 2;
        char* cur = (char*)(p + 0x2825);
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
                        sprintf(buf28, (const char*)&lbl_eu_8050566C[0x17a], buf48);
                        break;
                    case 2:
                        sprintf(buf28, (const char*)&lbl_eu_8050566C[0x17a], buf48);
                        break;
                    case 3:
                        if (langFlag != 0) sprintf(buf28, (const char*)&lbl_eu_8050566C[0x190], buf48);
                        else sprintf(buf28, (const char*)&lbl_eu_8050566C[0x1a9], buf48);
                        break;
                    case 4:
                        if (langFlag != 0) sprintf(buf28, (const char*)&lbl_eu_8050566C[0x190], buf48);
                        else sprintf(buf28, (const char*)&lbl_eu_8050566C[0x1a9], buf48);
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
                    for (ti = 0; ti < 7; ti++) wordTbl[ti] = ((const u32*)lbl_eu_805055F0)[ti];
                    key = *(u32*)((char*)wordTbl + langIdx);
                    nKey = BdatGetU8Direct(tbl, (const char*)key, (u16)slotStatus);
                    if (langFlag != 0) sprintf(buf28, (const char*)&lbl_eu_8050566C[0x1c1], (u8)nKey);
                    else sprintf(buf28, (const char*)&lbl_eu_8050566C[0x1da], (u8)nKey);
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
                sprintf(buf68, (const char*)&lbl_eu_8050566C[0x14c], cur);
                cur -= 2;
                sprintf(cur, (const char*)&lbl_eu_8050566C[0x14c], buf28);
                cur += len - 1;
                sprintf(cur, (const char*)&lbl_eu_8050566C[0x14c], buf68);
            postTag:;
            } else if ((ch >= 0x81 && ch <= 0x9f) || (ch >= (s8)0xe0 && ch <= (s8)0xef)) {
                cur += 2;
            } else {
                cur += 1;
            }
        }
        }
    } else {
        // lbl_eu_80664508/0C are single pointer variables (not arrays):
        // retail loads 0C first, then overrides with 08 when the category is
        // in range or exactly 2.
        u32 table = lbl_eu_8066450C;
        if ((((u32)p[0x2802] + 0xFC) & 0xFF) <= 5 || p[0x2802] == 2) table = lbl_eu_80664508;
        u16 v = BdatGetU16Direct((const void*)lbl_eu_806640EC, &lbl_eu_8050566C[0x1f2],
                              (*(u32*)item) >> 20);
        char* s = BdatGetPtrDirect((const void*)table, &lbl_eu_8050566C[0x1f2], v);
        sprintf((char*)(p + 0x2825), (const char*)&lbl_eu_8050566C[0x14c], s);
    }
    return (char*)(p + 0x2825);
fail:
    return NULL;
}
#pragma optimize_for_size off

// Check if an object has type 9 (extracted from vtable bits) and subtype 2.
// noinline: retail callers emit `bl` (the unit builds with -inline and would
// otherwise fold this body into every call site).
extern "C" __declspec(noinline) int IsSkillItem(void* obj) {
    u32 w = *(u32*)obj;
    u32 type = (w >> 16) & 0xF;
    int result = 0;
    if (type == 9) {
        u8 sub = *(u8*)((u8*)obj + 7) & 3;
        if (sub == 2) {
            result = 1;
        }
    }
    return result;
}

// Forward declarations for sort helpers defined later in this file.
void func_801C7EF0(CItemBoxGridFull* self, u32 mode);
extern "C" void SortByItemU16(CItemBoxGridFull* self);
extern "C" void SortByPriceDesc(CItemBoxGridFull* self);
extern "C" void SortByKindNameD(CItemBoxGridFull* self);
extern "C" void SortByItemByte6(CItemBoxGridFull* self);
extern "C" void SortByCellFlags(CItemBoxGridFull* self);
extern "C" void SortByFlag6Desc(CItemBoxGridFull* self);
extern "C" void SortByFlag6Asc(CItemBoxGridFull* self);
extern "C" void SortByItemVf08(CItemBoxGridFull* self);
void GroupBySlotStat(CItemBoxGridFull* self, u32 target);
void GroupBySlotAlt(CItemBoxGridFull* self, u32 target);
void func_801C8DE4(CItemBoxGridFull* self);
extern "C" void SortByItemVf30(CItemBoxGridFull* self);
extern "C" void SortByEquipAsc(CItemBoxGridFull* self);
extern "C" void SortByEquipDesc(CItemBoxGridFull* self);
void func_801C9270(CItemBoxGridFull* self);
void func_801C9390(CItemBoxGridFull* self);
void func_801C94E0(CItemBoxGridFull* self);
extern "C" void SortByCapKeyAsc(CItemBoxGridFull* self);
extern "C" void SortByCapKey2Asc(CItemBoxGridFull* self);
void GroupByCapKey(CItemBoxGridFull* self, u32 target);
extern "C" void SortByCapKeyDesc(CItemBoxGridFull* self);
extern "C" void SortByItemVf90(CItemBoxGridFull* self);
void func_801C9CCC(CItemBoxGridFull* self);
void GroupByKindName(CItemBoxGridFull* self, u32 target);

// Re-sort the grid after its contents changed: dispatch on the current
// category (sorting first with func_801C7EF0 and masking the per-category
// CItemBlock_getFlag120EC result into an 8-bit value v), then apply the category's
// inner sort sequence. Categories 8-13 do nothing (default). optimize_for_size
// merges the r30/r31 saves into retail's stmw prologue. The empty cases
// 8-13 must be written with an explicit `return;` body: grouped `break;`
// empties (with or without default) collapse and MWCC emits only an 8-slot
// table (`cmpli 7`); the return-form placeholder extends it to retail's
// 14-entry `cmplwi 0xd` table. Remaining residuals: (a) MWCC colors the
// outer switch operand r0 where retail keeps r4 (a named u8 local recolors
// it but also reorders the func_801C7EF0/CItemBlock_getFlag120EC call scheduling --
// worse); (b) ~40-byte block-order divergence starting at the case-2 jump
// table (decomp places case-1 subcase-8/9 body where retail puts the
// jumptable_eu_80534598 setup).
#pragma optimize_for_size on
void func_801C6EC0(CItemBoxGridFull* self, u16 idx) {
    func_801C7EF0(self, self->field_2802);
    u8 v = (u8)CItemBlock_getFlag120EC(self->field_2802);
    switch (self->field_2802) {
    case 0:
        SortByItemU16(self);
        switch (v) {
        case 1:
            SortByEquipAsc(self);
            SortByItemVf30(self);
            func_801C8DE4(self);
            func_801C9270(self);
            break;
        case 2:
            SortByEquipAsc(self);
            SortByItemVf30(self);
            func_801C8DE4(self);
            func_801C9390(self);
            func_801C9270(self);
            break;
        case 3:
            SortByEquipAsc(self);
            SortByItemVf30(self);
            func_801C8DE4(self);
            func_801C94E0(self);
            func_801C9270(self);
            break;
        case 4:
            func_801C8DE4(self);
            SortByItemVf30(self);
            SortByEquipDesc(self);
            func_801C9270(self);
            break;
        case 5:
            func_801C8DE4(self);
            SortByItemVf30(self);
            SortByEquipAsc(self);
            func_801C9270(self);
            break;
        case 6:
            SortByPriceDesc(self);
            break;
        }
        break;
    case 1:
        SortByItemU16(self);
        switch (v) {
        case 0:
            GroupByCapKey(self, 1);
            GroupByCapKey(self, 2);
            GroupByCapKey(self, 3);
            SortByEquipAsc(self);
            SortByItemVf30(self);
            SortByCapKeyAsc(self);
            break;
        case 1:
            GroupByCapKey(self, 1);
            GroupByCapKey(self, 2);
            GroupByCapKey(self, 3);
            SortByEquipAsc(self);
            SortByItemVf30(self);
            SortByCapKey2Asc(self);
            break;
        case 2:
            SortByEquipAsc(self);
            SortByItemVf30(self);
            SortByCapKeyAsc(self);
            GroupByCapKey(self, 3);
            GroupByCapKey(self, 2);
            GroupByCapKey(self, 1);
            break;
        case 3:
            SortByEquipAsc(self);
            SortByItemVf30(self);
            SortByCapKeyAsc(self);
            GroupByCapKey(self, 3);
            GroupByCapKey(self, 1);
            GroupByCapKey(self, 2);
            break;
        case 4:
            SortByEquipAsc(self);
            SortByItemVf30(self);
            SortByCapKeyAsc(self);
            GroupByCapKey(self, 2);
            GroupByCapKey(self, 1);
            GroupByCapKey(self, 3);
            break;
        case 5:
            GroupByCapKey(self, 1);
            GroupByCapKey(self, 2);
            GroupByCapKey(self, 3);
            SortByCapKeyAsc(self);
            SortByEquipAsc(self);
            SortByItemVf30(self);
            SortByCapKeyDesc(self);
            break;
        case 6:
            GroupByCapKey(self, 1);
            GroupByCapKey(self, 2);
            GroupByCapKey(self, 3);
            SortByCapKeyAsc(self);
            SortByItemVf30(self);
            SortByEquipDesc(self);
            break;
        case 7:
            GroupByCapKey(self, 1);
            GroupByCapKey(self, 2);
            GroupByCapKey(self, 3);
            SortByCapKeyAsc(self);
            SortByItemVf30(self);
            SortByEquipAsc(self);
            break;
        case 8:
            SortByPriceDesc(self);
            break;
        case 9:
            SortByPriceDesc(self);
            break;
        }
        break;
    case 2:
        SortByItemU16(self);
        switch (v) {
        case 0:
            SortByItemU16(self);
            break;
        case 1:
            SortByItemVf90(self);
            func_801C7EF0(self, self->field_2802);
            SortByItemVf08(self);
            break;
        case 2:
            func_801C7EF0(self, self->field_2802);
            SortByItemVf90(self);
            SortByItemVf08(self);
            GroupBySlotAlt(self, 2);
            GroupBySlotAlt(self, 0);
            GroupBySlotAlt(self, 1);
            break;
        case 3:
            func_801C7EF0(self, self->field_2802);
            SortByItemVf90(self);
            SortByItemVf08(self);
            GroupBySlotAlt(self, 1);
            GroupBySlotAlt(self, 0);
            GroupBySlotAlt(self, 2);
            break;
        case 4:
            func_801C7EF0(self, self->field_2802);
            SortByItemVf90(self);
            SortByItemVf08(self);
            GroupBySlotAlt(self, 2);
            GroupBySlotAlt(self, 1);
            GroupBySlotAlt(self, 0);
            break;
        case 5:
            GroupBySlotAlt(self, 2);
            GroupBySlotAlt(self, 1);
            GroupBySlotAlt(self, 0);
            SortByItemVf90(self);
            SortByItemVf08(self);
            func_801C7EF0(self, self->field_2802);
            GroupBySlotStat(self, 1);
            break;
        case 6:
            GroupBySlotAlt(self, 2);
            GroupBySlotAlt(self, 1);
            GroupBySlotAlt(self, 0);
            SortByItemVf90(self);
            SortByItemVf08(self);
            func_801C7EF0(self, self->field_2802);
            GroupBySlotStat(self, 2);
            break;
        case 7:
            GroupBySlotAlt(self, 2);
            GroupBySlotAlt(self, 1);
            GroupBySlotAlt(self, 0);
            SortByItemVf90(self);
            SortByItemVf08(self);
            func_801C7EF0(self, self->field_2802);
            GroupBySlotStat(self, 3);
            break;
        case 8:
            GroupBySlotAlt(self, 2);
            GroupBySlotAlt(self, 1);
            GroupBySlotAlt(self, 0);
            SortByItemVf90(self);
            SortByItemVf08(self);
            func_801C7EF0(self, self->field_2802);
            GroupBySlotStat(self, 4);
            break;
        case 9:
            GroupBySlotAlt(self, 2);
            GroupBySlotAlt(self, 1);
            GroupBySlotAlt(self, 0);
            SortByItemVf90(self);
            SortByItemVf08(self);
            func_801C7EF0(self, self->field_2802);
            GroupBySlotStat(self, 5);
            break;
        case 10:
            GroupBySlotAlt(self, 2);
            GroupBySlotAlt(self, 1);
            GroupBySlotAlt(self, 0);
            SortByItemVf90(self);
            SortByItemVf08(self);
            func_801C7EF0(self, self->field_2802);
            GroupBySlotStat(self, 6);
            break;
        case 11:
            SortByPriceDesc(self);
            break;
        }
        break;
    case 3:
        SortByItemU16(self);
        switch (v) {
        case 0:
            SortByItemU16(self);
            break;
        case 1:
            SortByItemVf08(self);
            break;
        case 2:
            SortByItemVf08(self);
            func_801C7EF0(self, self->field_2802);
            break;
        case 3:
            SortByItemU16(self);
            SortByPriceDesc(self);
            break;
        }
        break;
    case 4:
        SortByItemU16(self);
        switch (v) {
        case 1:
            SortByKindNameD(self);
            break;
        case 2:
            SortByItemByte6(self);
            break;
        case 3:
            SortByCellFlags(self);
            break;
        case 4:
            SortByPriceDesc(self);
            break;
        }
        break;
    case 5:
        SortByItemU16(self);
        switch (v) {
        case 1:
            SortByItemByte6(self);
            break;
        case 2:
            SortByCellFlags(self);
            break;
        case 3:
            SortByPriceDesc(self);
            break;
        }
        break;
    case 6:
        if (v == 0) {
            SortByItemU16(self);
        }
        break;
    case 7:
        switch (v) {
        case 0:
            SortByItemU16(self);
            break;
        case 1:
            GroupByKindName(self, 3);
            GroupByKindName(self, 2);
            GroupByKindName(self, 1);
            func_801C9CCC(self);
            SortByFlag6Desc(self);
            break;
        case 2:
            GroupByKindName(self, 3);
            GroupByKindName(self, 2);
            GroupByKindName(self, 1);
            func_801C9CCC(self);
            SortByFlag6Asc(self);
            break;
        case 3:
            SortByItemU16(self);
            SortByPriceDesc(self);
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
#pragma optimize_for_size off

// Clear inactive entries (flag byte 8 set) and compact the grid: bubble
// passes swap every still-empty (-1 id) cell toward the front. Afterwards,
// when the entry count changed, recompute the row count (entries per row /
// rounded up) and reset the page index.
void func_801C7730(CItemBoxGridFull* self) {
    u8* p = (u8*)self;
    u16 count = self->field_2800;
    u16 i;
    for (i = 0; i < count; i++) {
        u8* e = p + i * 10;
        if (e[8]) {
            // Retail reuses the ctor's return as the move src (nested call).
            CopyGridEntry9(e, SetEntry9Bytes(e, -1, 0, 0, 0, 0, 0, 0, 0));
            self->field_2800--;
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
    if (count != self->field_2800) {
        // Row count = entries/row rounded up; page index resets.
        u32 cnt = self->field_2800;
        float fdiv = (float)cnt / lbl_eu_80667F30;
        int rows = (int)fdiv;
        float frac = fdiv - (float)rows;
        if (lbl_eu_80667F34 != frac) rows++;
        if (!(u8)rows) rows = 1;
        self->field_2803 = (u8)rows;
        self->field_2804 = 0;
    }
}

// 9-byte copy used by the sort routines' temp shuffle. noinline keeps the
// call sites' `bl` (retail calls it; -inline would fold the body in).
// Returns dst: retail sorters reuse the caller-preserved r3 (the CopyEntry9Bytes
// result) for the following CopyGridEntry9 src argument instead of recomputing.
__declspec(noinline) char* CopyEntry9Bytes(char* dst, const char* src) {
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
//   lbl_eu_8050560C row indexed by the +0x08 slot value.
// - type 9: per-slot price rows are accumulated over up to 4 slots (each
//   scaled by a fresh copy of the table row), then the sum is scaled by
//   0x67F38; the common tail applies an extra x10.
// - types 2..8: price row scaled by 0x67F78 + 0x67F7C per slot count.
// - anything else: plain price row lookup.
__declspec(noinline) u32 func_801C7958(void* self, void* item) {
    u32 word = *(u32*)item;
    u32 kind = word >> 20;
    u32 type = (word >> 16) & 0xF;
    u32 shortKind = BdatGetItemId(kind);
    u32 result = 0;

    if (type == 3) {
        u16 key = CItem_initItemImplInstances((CItemData*)item)->vf54((CItemData*)item);
        u16 slot = (u16)CItem_initItemImplInstances((CItemData*)item)->vf08((CItemData*)item);
        u16 lookup = BdatGetU16Direct((void*)lbl_eu_806640D8, &lbl_eu_8050566C[0x1fa], (u16)key);
        CItemPriceScaleRow row;
        row = *(const CItemPriceScaleRow*)&lbl_eu_8050560C;
        result = (u32)((float)lookup * row.v[slot & 0x3F]);
    } else if (type == 9) {
        u16 slot = (u16)CItem_initItemImplInstances((CItemData*)item)->vf08((CItemData*)item);
        CItemPriceScaleRow rowA;
        rowA = *(const CItemPriceScaleRow*)&lbl_eu_8050560C;
        u32 i;
        for (i = 0; i < 4; i++) {
            u16 v = (u16)CItem_initItemImplInstances((CItemData*)item)->vf4C((CItemData*)item, (u8)i);
            if (v == 0) continue;
            u16 lookup = BdatGetU16Direct((void*)lbl_eu_806640D8, &lbl_eu_8050566C[0x1fa], v);
            // Retail re-copies the row inside the loop before each scale.
            CItemPriceScaleRow rowB;
            rowB = rowA;
            result = (u32)(lbl_eu_80667F74 * ((float)lookup * rowB.v[slot & 0x3F]) + (double)result);
        }
        result = (u32)((float)result * lbl_eu_80667F38);
    } else if (type >= 2 && type <= 8) {
        func_801393CC(kind);
        u16 lookup = BdatGetU16Direct((void*)lbl_eu_806640D8, &lbl_eu_8050566C[0x1fa], (u16)shortKind);
        u8 count = (u8)CItem_initItemImplInstances((CItemData*)item)->vf30((CItemData*)item);
        float f = lbl_eu_80667F78;
        while (count-- != 0) f += lbl_eu_80667F7C;
        result = (u32)((float)lookup * f);
    } else {
        func_801393CC(kind);
        result = BdatGetU16Direct((void*)lbl_eu_806640D8, &lbl_eu_8050566C[0x1fa], (u16)shortKind);
    }
    return result * 10;
}

// Item value used for shop buy/sell comparisons. Kind/type split mirrors
// func_801C7958: type 3 prices off the +0x54 slot row scaled by the
// lbl_eu_8050560C row indexed by the +0x08 slot (or, for bare kinds, the
// name-table lookups); type 9 values at 0; types 2-8 price off the row
// scaled by 0x67F78 + 0x67F7C per slot; everything else is the plain row.
// Result is always x10.
#pragma optimize_for_size on
__declspec(noinline) s32 func_801C7C7C(void* self, u32 id, void* item) {
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
            slot = (u8)BdatGetU8Direct((u32)lbl_eu_806640EC, &lbl_eu_8050566C[0x200], shortKind & 0xFFFF);
        }
        u16 lookup = BdatGetU16Direct((void*)lbl_eu_806640D8, &lbl_eu_8050566C[0x1fa], key & 0xFFFF);
        u32 scale[7];
        u32 i;
        for (i = 0; i < 7; i++) scale[i] = ((const u32*)lbl_eu_8050560C)[i];
        result = (u32)((float)lookup * ((float*)scale)[slot & 0x3F]);
    } else if ((u32)(u16)type == 9) {
        // result stays 0
    } else if ((u32)(u16)type >= 2 && (u32)(u16)type <= 8) {
        u32 tbl = func_801393CC(kind & 0xFFFF);
        u16 lookup = BdatGetU16Direct((void*)lbl_eu_806640D8, &lbl_eu_8050566C[0x1fa], shortKind & 0xFFFF);
        u32 count;
        if (item) count = (u16)CItem_initItemImplInstances((CItemData*)obj)->vf30((CItemData*)obj);
        else count = (u8)BdatGetU8Direct(tbl, &lbl_eu_8050566C[0x209], shortKind & 0xFFFF);
        float f = lbl_eu_80667F78;
        while (count-- != 0) f += lbl_eu_80667F7C;
        result = (u32)((float)lookup * f);
    } else {
        func_801393CC(kind & 0xFFFF);
        result = BdatGetU16Direct((void*)lbl_eu_806640D8, &lbl_eu_8050566C[0x1fa], shortKind & 0xFFFF);
    }
    return result * 10;
}
#pragma optimize_for_size off

// Bubble-sort grid entries ascending under the selected comparison mode.
// A pass that performs no swap ends the sort early (i only advances on swap).
void func_801C7EF0(CItemBoxGridFull* self, u32 mode) {
    u32 i;
    for (i = 0; (u16)i < self->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        for (j = 0; (u16)j < self->field_2800 - 1 - (u16)i; j++) {
            void* obj1 = func_80157C4C(self->field_2802, self->entries[j].id);
            void* obj2 = func_80157C4C(self->field_2802, self->entries[j + 1].id);
            CItemBoxGridEntry* e1 = &self->entries[j];
            CItemBoxGridEntry* e2 = &self->entries[j + 1];
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
                    CItemSortView* v1 = (CItemSortView*)obj1;
                    CItemSortView* v2 = (CItemSortView*)obj2;
                    if ((v1->flags7 >> 2 & 0x3f) > (v2->flags7 >> 2 & 0x3f)) {
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
                CItemSortView* v1 = (CItemSortView*)obj1;
                CItemSortView* v2 = (CItemSortView*)obj2;
                u32 key1 = (u16)BdatGetItemId(v1->word00 >> 20);
                u32 key2 = (u16)BdatGetItemId(v2->word00 >> 20);
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
// item lookups (func_80157C4C) gate the swap; the 3-slot temp shuffle keeps
// the retail bl sequence (CopyEntry9Bytes x3 / CopyGridEntry9 x2).
extern "C" void SortByItemU16(CItemBoxGridFull* self) {
    u16 i;
    for (i = 0; i < self->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < self->field_2800 - 1 - i; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            if (*(u16*)((u8*)obj1 + 4) <= *(u16*)((u8*)obj2 + 4)) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries by item price (func_801C7958). The value calls
// evaluate item j+1 before item j (locals force the call order), matching
// retail's sequence; the 3-slot temp shuffle is the same shape as
// SortByItemU16. Swap when the first item's price is strictly lower.
extern "C" void SortByPriceDesc(CItemBoxGridFull* self) {
    u16 i;
    for (i = 0; i < self->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < self->field_2800 - 1 - i; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            u32 v2 = func_801C7958(self, obj2);
            u32 v1 = func_801C7958(self, obj1);
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
// looked up in the message table (lbl_eu_80664104) with the "name" key
// string; swap when the byte keys differ (descending). Same 3-slot temp
// shuffle shape as SortByItemU16.
extern "C" void SortByKindNameD(CItemBoxGridFull* self) {
    u16 i;
    for (i = 0; i < self->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < self->field_2800 - 1 - i; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            u32 w1 = *(u32*)obj1;
            u32 k1 = BdatGetItemId(w1 >> 20);
            u32 w2 = *(u32*)obj2;
            u32 k2 = BdatGetItemId(w2 >> 20);
            u32 c1 = BdatGetU8Direct(lbl_eu_80664104, (const char*)&lbl_eu_8050566C[0x212], k1 & 0xFFFF);
            u32 c2 = BdatGetU8Direct(lbl_eu_80664104, (const char*)&lbl_eu_8050566C[0x212], k2 & 0xFFFF);
            if ((c1 & 0xFF) <= (c2 & 0xFF)) continue;
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
extern "C" void SortByItemByte6(CItemBoxGridFull* self) {
    u16 i;
    for (i = 0; i < self->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < self->field_2800 - 1 - i; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            if (*(u8*)((u8*)obj1 + 6) >= *(u8*)((u8*)obj2 + 6)) continue;
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
extern "C" void SortByCellFlags(CItemBoxGridFull* self) {
    u32 i;
    for (i = 0; (u16)i < self->field_2800 - 1; i++) {
        u32 i16 = (u16)i;
        int swapped = 0;
        u32 j;
        char tmp[36];
        for (j = 0; (u16)j < self->field_2800 - 1 - (s32)i16; j++) {
            CItemBoxGridEntry* e1 = &self->entries[(u16)j];
            CItemBoxGridEntry* e2 = &self->entries[(u16)j + 1];
            if ((u32)e1->flags[3] < (u32)e2->flags[3]) {
                CopyEntry9Bytes(tmp, (const char*)e1);
                CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
                CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
    for (u32 i = 0; (u16)i < self->field_2800 - 1; i++) {
        u32 i16 = (u16)i;
        int swapped = 0;
        u32 j;
        char tmp2[36];
        for (j = 0; (u16)j < self->field_2800 - 1 - (s32)i16; j++) {
            CItemBoxGridEntry* e1 = &self->entries[(u16)j];
            CItemBoxGridEntry* e2 = &self->entries[(u16)j + 1];
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
// optimize_for_size merges the r26-r31 saves into retail's stmw prologue.
#pragma push
#pragma optimize_for_size on
extern "C" void SortByFlag6Desc(CItemBoxGridFull* self) {
    u8* p = (u8*)self;
    char tmp[36];
    u32 i;
    for (i = 0; (u16)i < *(u16*)(p + 0x2800) - 1; i++) {
        int swapped = 0;
        u32 j;
        for (j = 0; (u16)j < *(u16*)(p + 0x2800) - 1 - (u16)i; j++) {
            u16 jj = (u16)j;
            u8* e1 = p + jj * 10;
            u8* e2 = p + (jj + 1) * 10;
            if (e1[6] > e2[6]) {
                CopyEntry9Bytes(tmp, (const char*)e1);
                CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
                CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, tmp));
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}
#pragma pop

// Bubble sort entries ascending by byte at offset 6 (mirror of
// SortByFlag6Desc with the comparison inverted).
#pragma push
#pragma optimize_for_size on
extern "C" void SortByFlag6Asc(CItemBoxGridFull* self) {
    u8* p = (u8*)self;
    char tmp[36];
    u32 i;
    for (i = 0; (u16)i < *(u16*)(p + 0x2800) - 1; i++) {
        int swapped = 0;
        u32 j;
        for (j = 0; (u16)j < *(u16*)(p + 0x2800) - 1 - (u16)i; j++) {
            u16 jj = (u16)j;
            u8* e1 = p + jj * 10;
            u8* e2 = p + (jj + 1) * 10;
            if (e1[6] < e2[6]) {
                CopyEntry9Bytes(tmp, (const char*)e1);
                CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
                CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, tmp));
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}
#pragma pop

// Bubble-sort grid entries ascending by the item instance's vtable+0x08
// slot value (u16). v2 (item j+1) is evaluated before v1 to match retail's
// call order; the 3-slot temp shuffle is the same shape as SortByItemU16.
extern "C" void SortByItemVf08(CItemBoxGridFull* self) {
    void* obj2;
    u16 v2;
    u16 i;
    for (i = 0; i < self->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < self->field_2800 - 1 - i; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
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
// (lbl_eu_806640D8) with the "name" key at +0x217.
void GroupBySlotStat(CItemBoxGridFull* self, u32 target) {
    u32 i;
    for (i = 0; (u16)i < self->field_2800 - 1; i++) {
        int swapped = 0;
        u32 j;
        char tmp[36];
        for (j = 0; (u16)j < self->field_2800 - 1 - (u16)i; j++) {
            CItemBoxGridEntry* e1 = &self->entries[(u16)j];
            CItemBoxGridEntry* e2 = &self->entries[(u16)j + 1];
            void* obj1 = func_80157C4C(self->field_2802, e1->id);
            void* obj2 = func_80157C4C(self->field_2802, e2->id);
            u16 v1 = CItem_initItemImplInstances((CItemData*)obj1)->vf54((CItemData*)obj1);
            u16 v2 = CItem_initItemImplInstances((CItemData*)obj2)->vf54((CItemData*)obj2);
            u32 c1 = BdatGetU8Direct(lbl_eu_806640D8, (const char*)&lbl_eu_8050566C[0x217], (u16)v1);
            u32 c2 = BdatGetU8Direct(lbl_eu_806640D8, (const char*)&lbl_eu_8050566C[0x217], (u16)v2);
            u32 m1 = (u8)c1 == target;
            u32 m2 = (u8)c2 == target;
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
void GroupBySlotAlt(CItemBoxGridFull* self, u32 target) {
    u32 i;
    for (i = 0; (u16)i < self->field_2800 - 1; i++) {
        int swapped = 0;
        u32 j;
        char tmp[36];
        for (j = 0; (u16)j < self->field_2800 - 1 - (u16)i; j++) {
            CItemBoxGridEntry* e1 = &self->entries[(u16)j];
            CItemBoxGridEntry* e2 = &self->entries[(u16)j + 1];
            void* obj1 = func_80157C4C(self->field_2802, e1->id);
            void* obj2 = func_80157C4C(self->field_2802, e2->id);
            u16 v1 = CItem_initItemImplInstances((CItemData*)obj1)->vf54((CItemData*)obj1);
            u16 v2 = CItem_initItemImplInstances((CItemData*)obj2)->vf54((CItemData*)obj2);
            u32 c1 = BdatGetU8Direct(lbl_eu_806640D8, (const char*)&lbl_eu_8050566C[0x220], (u16)v1);
            u32 c2 = BdatGetU8Direct(lbl_eu_806640D8, (const char*)&lbl_eu_8050566C[0x220], (u16)v2);
            u32 m1 = (u8)c1 == target;
            u32 m2 = (u8)c2 == target;
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

// Bubble sort grid entries ascending by item value (func_801C9F88), with
// early exit when a pass makes no swaps. The 3-slot temp shuffle at a
// 12-byte stride reproduces retail's sp+8/sp+0x14/sp+0x20 copy sequence;
// v1 must survive the second value call, so it lives in saved f31.
void func_801C8DE4(CItemBoxGridFull* self) {
    u16 i;
    for (i = 0; i < self->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        for (j = 0; j < self->field_2800 - 1 - i; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            float v1 = func_801C9F88(self, obj1);
            float v2 = func_801C9F88(self, obj2);
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
extern "C" void SortByItemVf30(CItemBoxGridFull* self) {
    u16 i;
    for (i = 0; i < self->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < self->field_2800 - 1 - i; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
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
extern "C" void SortByEquipAsc(CItemBoxGridFull* self) {
    u16 i;
    for (i = 0; i < self->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < self->field_2800 - 1 - i; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            u32 v1 = HasEquippedSlot(self, obj1);
            u32 v2 = HasEquippedSlot(self, obj2);
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
extern "C" void SortByEquipDesc(CItemBoxGridFull* self) {
    u16 i;
    for (i = 0; i < self->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < self->field_2800 - 1 - i; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            u32 v1 = HasEquippedSlot(self, obj1);
            u32 v2 = HasEquippedSlot(self, obj2);
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
// func_801C8DE4; both value calls pass (self, item).
void func_801C9270(CItemBoxGridFull* self) {
    u16 i;
    for (i = 0; i < self->field_2800 - 1; i++) {
        u16 j;
        int swapped = 0;
        for (j = 0; j < self->field_2800 - 1 - i; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            u8 v1 = (u8)FindKindPoolIdx(self, obj1);
            u8 v2 = (u8)FindKindPoolIdx(self, obj2);
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
// a short id, which is looked up in the owner table (lbl_eu_806640F4) with
// the sort-key string; swap when item j's key byte is strictly lower. Same
// 3-slot temp shuffle shape as SortByItemU16.
void func_801C9390(CItemBoxGridFull* self) {
    u16 i;
    for (i = 0; i < self->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < self->field_2800 - 1 - i; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            u32 w1 = *(u32*)obj1;
            u32 t1 = (u32)(size_t)lbl_eu_806640F4;
            u32 k1 = BdatGetItemId(w1 >> 20);
            u32 c1 = BdatGetU8Direct(t1, (const char*)&lbl_eu_8050566C[0x227], k1 & 0xFFFF);
            u32 w2 = *(u32*)obj2;
            u32 t2 = (u32)(size_t)lbl_eu_806640F4;
            u32 k2 = BdatGetItemId(w2 >> 20);
            u32 c2 = BdatGetU8Direct(t2, (const char*)&lbl_eu_8050566C[0x227], k2 & 0xFFFF);
            if ((c1 & 0xFF) >= (c2 & 0xFF)) continue;
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
// Same per-item kind->lookup interleave and temp shuffle as func_801C9390.
void func_801C94E0(CItemBoxGridFull* self) {
    u16 i;
    for (i = 0; i < self->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < self->field_2800 - 1 - i; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            // Owner-table row is selected by the raw item word used as a
            // byte offset into the table data.
            u32 w1 = *(u32*)obj1;
            u32 c1 = BdatGetU8Direct(*(u32*)((u8*)lbl_eu_806640F4 + w1),
                                   (const char*)&lbl_eu_8050566C[0x22f],
                                   (u16)BdatGetItemId(w1 >> 20));
            u32 w2 = *(u32*)obj2;
            u32 c2 = BdatGetU8Direct(*(u32*)((u8*)lbl_eu_806640F4 + w2),
                                   (const char*)&lbl_eu_8050566C[0x22f],
                                   (u16)BdatGetItemId(w2 >> 20));
            if ((u8)c1 >= (u8)c2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries ascending by the byte value of each item's kind
// name lookup in the capacity table (lbl_eu_806640F8). Both kinds are
// computed first, then both lookups - the SortByKindNameD ordering. Same
// 3-slot temp shuffle shape as SortByItemU16.
extern "C" void SortByCapKeyAsc(CItemBoxGridFull* self) {
    u16 i;
    for (i = 0; i < self->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < self->field_2800 - 1 - i; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            u32 w1 = *(u32*)obj1;
            u32 k1 = BdatGetItemId(w1 >> 20);
            u32 w2 = *(u32*)obj2;
            u32 k2 = BdatGetItemId(w2 >> 20);
            u32 c1 = BdatGetU8Direct(lbl_eu_806640F8, (const char*)&lbl_eu_8050566C[0x238], k1 & 0xFFFF);
            u32 c2 = BdatGetU8Direct(lbl_eu_806640F8, (const char*)&lbl_eu_8050566C[0x238], k2 & 0xFFFF);
            if ((c1 & 0xFF) >= (c2 & 0xFF)) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries ascending by the byte value of each item's kind
// name lookup in the capacity table (lbl_eu_806640F8), key string offset
// 0x240. Same ordering and temp shuffle shape as SortByCapKeyAsc.
extern "C" void SortByCapKey2Asc(CItemBoxGridFull* self) {
    u16 i;
    for (i = 0; i < self->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < self->field_2800 - 1 - i; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            u32 w1 = *(u32*)obj1;
            u32 k1 = BdatGetItemId(w1 >> 20);
            u32 w2 = *(u32*)obj2;
            u32 k2 = BdatGetItemId(w2 >> 20);
            u32 c1 = BdatGetU8Direct(lbl_eu_806640F8, (const char*)&lbl_eu_8050566C[0x240], k1 & 0xFFFF);
            u32 c2 = BdatGetU8Direct(lbl_eu_806640F8, (const char*)&lbl_eu_8050566C[0x240], k2 & 0xFFFF);
            if ((c1 & 0xFF) >= (c2 & 0xFF)) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyGridEntry9(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            CopyGridEntry9(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Partition-style bubble pass: move every entry whose kind-name lookup key
// (kind mapped through BdatGetItemId then looked up in table lbl_eu_806640F8
// with the string at lbl_eu_8050566C+0x248) equals `target` toward the front.
// Swap fires when the earlier entry mismatches and the later one matches
// (retail compares the two "== target" booleans); the whole sort aborts as
// soon as one full sweep makes no swap.
void GroupByCapKey(CItemBoxGridFull* self, u32 target) {
    // Hoisted string-pool base: retail keeps this pointer live in a
    // callee-saved register for the whole function.
    const char* strBase = lbl_eu_8050566C;
    u32 i;
    for (i = 0; (u16)i < self->field_2800 - 1; i++) {
        u32 swapped = 0;
        u32 j;
        char tmp[36];
        for (j = 0; (u16)j < self->field_2800 - 1 - (s32)(u16)i; j++) {
            u8* e1 = (u8*)self + (u16)j * 10;
            u8* e2 = (u8*)self + ((u16)j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            // Both kind keys are looked up before either byte is truncated:
            // retail keeps the first lookup result live across the second call.
            u32 k1 = BdatGetItemId(*(u32*)obj1 >> 20);
            u32 k2 = BdatGetItemId(*(u32*)obj2 >> 20);
            u32 c1 = BdatGetU8Direct(lbl_eu_806640F8, strBase + 0x248, k1 & 0xFFFF);
            u32 c2 = BdatGetU8Direct(lbl_eu_806640F8, strBase + 0x248, k2 & 0xFFFF);
            // Swap only when entry j+1 matches the target and entry j doesn't:
            // this drives all target-matching entries to the front.
            u32 m2 = ((u8)c2 == target);
            u32 m1 = ((u8)c1 == target);
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
// name lookup in the capacity table (lbl_eu_806640F8), key string offset
// 0x251 (swap when item j's key byte is strictly higher). Same ordering
// and temp shuffle shape as SortByCapKeyAsc.
extern "C" void SortByCapKeyDesc(CItemBoxGridFull* self) {
    u16 i;
    for (i = 0; i < self->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < self->field_2800 - 1 - i; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            u32 w1 = *(u32*)obj1;
            u32 k1 = BdatGetItemId(w1 >> 20);
            u32 w2 = *(u32*)obj2;
            u32 k2 = BdatGetItemId(w2 >> 20);
            u32 c1 = BdatGetU8Direct(lbl_eu_806640F8, (const char*)&lbl_eu_8050566C[0x251], k1 & 0xFFFF);
            u32 c2 = BdatGetU8Direct(lbl_eu_806640F8, (const char*)&lbl_eu_8050566C[0x251], k2 & 0xFFFF);
            if ((c1 & 0xFF) <= (c2 & 0xFF)) continue;
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
extern "C" void SortByItemVf90(CItemBoxGridFull* self) {
    u16 i;
    for (i = 0; i < self->field_2800 - 1; i++) {
        int swapped = 0;
        u16 j;
        char tmp[36];
        for (j = 0; j < self->field_2800 - 1 - i; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
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
// lbl_eu_80664110 and the 39358/361E8 calls stay in this function.
void func_801C9CCC(CItemBoxGridFull* self) {
    u32 i;
    for (i = 0; (u16)i < self->field_2800 - 1; i++) {
        int swapped = 0;
        u32 j;
        char tmp[36];
        for (j = 0; (u16)j < self->field_2800 - 1 - (u16)i; j++) {
            // tbl declared first, assigned after the 57C4C calls so the SDA
            // load stays at retail +0x70 (early assign moved it and added
            // structural). Residual: e1↔tbl r23/r24.
            u32 tbl;
            u8* e1 = (u8*)self + (u16)j * 10;
            u8* e2 = (u8*)self + ((u16)j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)((u8*)self + (u16)j * 10));
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            u32 w1 = *(u32*)obj1;
            tbl = lbl_eu_80664110;
            u32 c1 = BdatGetU8Direct(tbl, (const char*)&lbl_eu_8050566C[0x115], (u16)BdatGetItemId(w1 >> 20));
            u32 w2 = *(u32*)obj2;
            u32 tbl2 = lbl_eu_80664110;
            u32 c2 = BdatGetU8Direct(tbl2, (const char*)&lbl_eu_8050566C[0x115], (u16)BdatGetItemId(w2 >> 20));
            if ((u8)c1 <= (u8)c2) continue;
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
// (lbl_eu_80664110) lookup of the kind's short id ("name" key at +0x109).
void GroupByKindName(CItemBoxGridFull* self, u32 target) {
    u32 i;
    for (i = 0; (u16)i < self->field_2800 - 1; i++) {
        int swapped = 0;
        u32 j;
        char tmp[36];
        for (j = 0; (u16)j < self->field_2800 - 1 - (u16)i; j++) {
            CItemBoxGridEntry* e1 = &self->entries[(u16)j];
            CItemBoxGridEntry* e2 = &self->entries[(u16)j + 1];
            void* obj1 = func_80157C4C(self->field_2802, e1->id);
            void* obj2 = func_80157C4C(self->field_2802, e2->id);
            u32 w1 = *(u32*)obj1;
            u32 tbl = lbl_eu_80664110;
            u32 c1 = BdatGetU8Direct(tbl, (const char*)&lbl_eu_8050566C[0x109], (u16)BdatGetItemId(w1 >> 20));
            u32 w2 = *(u32*)obj2;
            u32 tbl2 = lbl_eu_80664110;
            u32 c2 = BdatGetU8Direct(tbl2, (const char*)&lbl_eu_8050566C[0x109], (u16)BdatGetItemId(w2 >> 20));
            u32 m1 = (u8)c1 == target;
            u32 m2 = (u8)c2 == target;
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
// (lbl_eu_806640F4); when the kind's flag entry has bit 2 set, scale the
// index by party member 1's CtrlObjectParam_GetArtsDataWord value, then scale by the 0x67F80
// factor, clamp to 999 and return as a float.
// optimize_for_size: retail uses the bl _savegpr_28/_restgpr_28 helper-call
// prologue, which only size-opt emits on this unit.
#pragma push
#pragma optimize_for_size on
float func_801C9F88(void* self, void* entry) {
    u32 word = *(u32*)entry;
    u32 tbl = (u32)(size_t)lbl_eu_806640F4;
    u32 kind = BdatGetItemId(word >> 20);
    u32 idx = BdatGetU16Direct((void*)tbl, (const char*)&lbl_eu_8050566C[0x25a], (u16)kind);
    // Bit 2 of the kind flag entry gates a party-bonus multiplier.
    if ((BdatGetU8Direct(tbl, (const char*)&lbl_eu_8050566C[0x261], (u16)kind) & 4)) {
        void* charData = func_8009EC9C(1);
        // Reading the product back through a signed union member blocks
        // MWCC's non-negative range fold, so the conversion uses the signed
        // xoris magic (lbl_eu_80667F48).
        union {
            u32 u;
            int i;
        } prod;
        prod.u = (idx & 0xFFFF) * (u16)CtrlObjectParam_GetArtsDataWord(charData);
        idx = (u32)(u16)(s32)((float)prod.i * lbl_eu_80667F80);
    }
    if ((u32)(u16)idx > 999) idx = 999;
    // Truncated result converted back through the 2^52 magic (lbl_eu_80667F40).
    return (float)((double)(u16)idx - lbl_eu_80667F40);
}
#pragma pop

#pragma optimize_for_size on
__declspec(noinline) u32 HasEquippedSlot(void* self, void* item) {
    CItemImpl* inst = CItem_initItemImplInstances((CItemData*)item);
    u16 count = (u16)inst->vf30((CItemData*)item);
    u8 i;
    for (i = 0; i < count; i++) {
        CItemImpl* inst2 = CItem_initItemImplInstances((CItemData*)item);
        void* obj = inst2->vf2C((CItemData*)item, i);
        if (obj && (*(u16*)((u8*)obj + 4) & 1)) return 1;
    }
    return 0;
}
#pragma optimize_for_size off

// Check if entry kind exists in pool.
#pragma optimize_for_size on
extern "C" __declspec(noinline) u32 FindKindPoolIdx(void* self, void* entry) {
    u32 val = *(u32*)entry;
    u32 obj = (u32)lbl_eu_806640F4;
    u32 kind = BdatGetItemId(val >> 20);
    u32 i;
    for (i = 1; i <= 10; i++) {
        char buf[40];
        sprintf(buf, (const char*)&lbl_eu_8050566C[0x132], (u8)i);
        if ((u8)BdatGetU8Direct(obj, buf, (u16)kind)) return i;
    }
    return 0;
}
#pragma optimize_for_size off

// CItemBoxGrid constructor: stamp the vtable, save the mode/unk params into
// the header fields, build every sub-object in place, then copy the default
// configurations out of stack temporaries (the sort-menu, item-info,
// num-select, exchange and sys-win temp objects are constructed, their
// fields copied to the members, and the temps destroyed).
void* __ct__CItemBoxGrid(void* self, u8 mode, u16 unk5, u32 unk6, u8 unk7) {
    // Stack temporaries; sizes/regions match retail's 0x48c0 frame layout.
    u8 tmpWin2[0x3c];
    u8 tmpWin1[0x3c];
    u8 tmpEx[0x28];
    u8 tmpNum[0x38];
    u8 tmpSort[0xf0];
    u8 tmpGrid[0x44c0];
    u8 tmpInfo[0x20c];

    u8* p = (u8*)self;
    *(u32*)(p + 0) = (u32)&lbl_eu_80534740;
    *(u32*)(p + 4) = unk6;

    __ct__17UnkClass_8045F564Fv((UnkClass_8045F564*)(p + 0x8));
    __ct__17UnkClass_8045F564Fv((UnkClass_8045F564*)(p + 0x18));

    *(u32*)(p + 0x28) = 0;
    *(u32*)(p + 0x2c) = 0;
    *(u32*)(p + 0x30) = 0;
    *(u32*)(p + 0x34) = 0;
    *(u32*)(p + 0x38) = 0;
    *(u32*)(p + 0x3c) = 0;
    *(u32*)(p + 0x40) = 0;
    *(u32*)(p + 0x44) = 0;
    *(u32*)(p + 0x48) = 0;
    *(u32*)(p + 0x4c) = 0;
    *(u32*)(p + 0x50) = 0;
    p[0x54] = 0;
    *(u32*)(p + 0x58) = 0;
    *(u32*)(p + 0x5c) = 0;
    p[0x60] = 0;
    p[0x61] = 1;
    p[0x6e] = 0;
    p[0x6f] = 0;

    __ct__CCur07(p + 0x70, 0);
    __ct__CCur09(p + 0x88, 0);
    __ct__CCur18(p + 0xa0, 0);
    __ct__CCur16(p + 0xb8, 0);
    __ct__CCur11(p + 0xd0, 0);
    __ct__CSortMenu(p + 0xe8);
    __ct__CItemBoxInfo(p + 0x1d8, 0, 0);
    __ct__CNumSelect(p + 0x3e4, 1);
    __ct__CItemBoxGridSubMenu(p + 0x418);
    __ct__CExchangeWin(p + 0x440);
    __ct__CPresentWin(p + 0x468);
    __ct__CSysWin(p + 0x4ac, 0);
    __ct__CSysWin(p + 0x4e8, 0);

    p[0x524] = 0;
    p[0x525] = 0;
    p[0x526] = 0;
    p[0x527] = mode;
    p[0x528] = 0;
    p[0x529] = 1;
    *(u16*)(p + 0x52a) = unk5;
    p[0x52c] = 0;
    p[0x52d] = 0;
    *(u16*)(p + 0x52e) = 0;
    p[0x530] = 0;
    p[0x540] = 1;
    p[0x541] = 0;
    p[0x542] = 0;
    p[0x543] = unk7;
    p[0x544] = 0;
    p[0x545] = 0;
    p[0x546] = 0;
    p[0x547] = 0;
    p[0x548] = 0;
    p[0x549] = 0;

    __ct__801C5514(p + 0x54c);
    memset(p + 0x62, 0, 0xc);

    // sort-menu temp: copy its config sub-blocks by value into the members.
    // Scalar fields are copied load-store interleaved; trailing word runs use
    // rolled pointer-walk loops (retail lwzu/stwu bdnz shape, pointers
    // biased -4 from the array start).
    __ct__CSortMenu(tmpSort);
    *(CibgSortMember*)(p + 0xec) = *(CibgSortMember*)(tmpSort + 4);
    __dt__9CSortMenuFv(tmpSort, -1);

    // item-info temp (mode-derived init)
    int modeId = 0;
    if (mode == 1) modeId = 1;
    else if (mode == 2) modeId = 3;
    __ct__CItemBoxInfo(tmpInfo, modeId, *(u16*)(p + 0x52a));
    *(CibgInfoMember*)(p + 0x1dc) = *(CibgInfoMember*)(tmpInfo + 4);
    __dt__12CItemBoxInfoFv(tmpInfo, -1);

    // num-select temp
    __ct__CNumSelect(tmpNum, 1);
    *(CibgNumSelMember*)(p + 0x3e8) = *(CibgNumSelMember*)(tmpNum + 4);
    __dt__10CNumSelectFv(tmpNum, -1);

    // exchange-win temp (mode 2 only)
    if (mode == 2) {
        __ct__CExchangeWin(tmpEx);
        *(CibgExchMember*)(p + 0x444) = *(CibgExchMember*)(tmpEx + 4);
        __dt__12CExchangeWinFv(tmpEx, -1);
    }

    // sys-win temps copied via func_8016742C
    __ct__CSysWin(tmpWin1, 2);
    func_8016742C(p + 0x4ac, tmpWin1);
    __dt__7CSysWinFv(tmpWin1, -1);
    __ct__CSysWin(tmpWin2, 0);
    func_8016742C(p + 0x4e8, tmpWin2);
    __dt__7CSysWinFv(tmpWin2, -1);

    lbl_eu_80664508 = 0;
    lbl_eu_8066450C = 0;
    lbl_eu_80664510 = 0;
    ItemBlockSetDefaultFlags();

    // sub-grid temp: init and bulk-copy cell table plus metadata blocks.
    // Each whole-array run is a struct assignment (MWCC lowers it as the
    // paired lwzu/stwu bdnz loop with pointers biased -4).
    __ct__801C5514(tmpGrid);
    *(CibgSubGridCells*)(p + 0x54c) = *(CibgSubGridCells*)tmpGrid;
    {
        CibgSubGridMeta* d = (CibgSubGridMeta*)(p + 0x2d4c);
        CibgSubGridMeta* s = (CibgSubGridMeta*)(tmpGrid + 0x800);
        d->a = s->a;
        d->b = s->b;
        d->c = s->c;
        d->d = s->d;
        d->names = s->names;
    }
    *(CibgWords16*)(p + 0x2d71) = *(CibgWords16*)(tmpGrid + 0x825);
    *(CibgByteBlock200*)(p + 0x2df1) = *(CibgByteBlock200*)(tmpGrid + 0x8a5);
    func_8018B130(p + 0x31f4, tmpGrid + 0xca8);
    func_8018B130(p + 0x39fc, tmpGrid + 0x34b0);
    func_8018B130(p + 0x4204, tmpGrid + 0x3cb8);
    __dt__801C5670(tmpGrid, -1);

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
    return self;
}

// CItemBoxGrid destructor: destroy each sub-object in reverse construction
// order, then free the block when the deleting-dtor flag is set. The
// optimize_for_size pair merges the r30/r31 saves into the retail stmw
// prologue (same pattern as the CArtsInfo/CKizunaTalkList dtors).
#pragma optimize_for_size on
extern "C" void* __dt__12CItemBoxGridFv(void* self, int mode) {
    if (self != 0) {
        __dt__801C5670((u8*)self + 0x54C, -1);
        __dt__7CSysWinFv((u8*)self + 0x4E8, -1);
        __dt__7CSysWinFv((u8*)self + 0x4AC, -1);
        __dt__11CPresentWinFv((u8*)self + 0x468, -1);
        __dt__12CExchangeWinFv((u8*)self + 0x440, -1);
        __dt__19CItemBoxGridSubMenuFv((u8*)self + 0x418, -1);
        __dt__10CNumSelectFv((u8*)self + 0x3E4, -1);
        __dt__12CItemBoxInfoFv((u8*)self + 0x1D8, -1);
        __dt__9CSortMenuFv((u8*)self + 0xE8, -1);
        __dt__6CCur11Fv((u8*)self + 0xD0, -1);
        __dt__6CCur16Fv((u8*)self + 0xB8, -1);
        __dt__6CCur18Fv((u8*)self + 0xA0, -1);
        __dt__6CCur09Fv((u8*)self + 0x88, -1);
        __dt__6CCur07Fv((u8*)self + 0x70, -1);
        __dt__17UnkClass_8045F564Fv((u8*)self + 0x18, -1);
        __dt__17UnkClass_8045F564Fv((u8*)self + 0x8, -1);
        if (mode > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}
#pragma optimize_for_size off

// Load the item-box resource files. When MEM1 can hold the biggest archive,
// the first file is read into MEM1 instead of MEM2. The string-table base is
// cached in a local (retail keeps it in r31 across every call).
// optimize_for_size: retail uses stmw r29/lmw r29 block saves.
#pragma push
#pragma optimize_for_size on
extern "C" void LoadItemBoxFiles(void* self, int r4) {
    u8* p = (u8*)self;
    const char* strs;
    void* allocHandle = getHandleMEM2__Q23mtl10MemManagerFv();

    if (func_80212480()) {
        if (getMaxAllocSize__Q23mtl10MemManagerFUl((u32)getHandleMEM1__Q23mtl10MemManagerFv()) >= 0x38000) {
            allocHandle = (void*)getHandleMEM1__Q23mtl10MemManagerFv();
        }
    }

    // Retail caches the string-table base here (r31), after the MEM1 choice.
    strs = lbl_eu_8050566C;

    *(u32*)(p + 0x28) = (u32)readFile__11CDeviceFileFUlPCcP10IWorkEventii((u32)allocHandle, strs + 0x266, self, 0, 0);

    allocHandle = getHandleMEM2__Q23mtl10MemManagerFv();
    *(u32*)(p + 0x2c) = (u32)readFile__11CDeviceFileFUlPCcP10IWorkEventii((u32)allocHandle, strs + 0x27e, self, 0, 0);

    if (p[0x543]) {
        allocHandle = (void*)KyoshinHeap_GetField44();
        *(u32*)(p + 0x30) = (u32)readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii((u32)allocHandle, strs + 0x297, self, 0, 0);

        allocHandle = getHandleMEM2__Q23mtl10MemManagerFv();
        *(u32*)(p + 0x34) = (u32)readFile__11CDeviceFileFUlPCcP10IWorkEventii((u32)allocHandle, strs + 0x2b4, self, 0, 0);
    } else {
        lbl_eu_80664508 = (u32)getFP__FPCc(strs + 0x2d0);
        lbl_eu_8066450C = (u32)getFP__FPCc(strs + 0x2df);
        lbl_eu_80664510 = (u32)getFP__FPCc(strs + 0x2ee);
    }

    func_801D3064(p + 0xe8);
    loadItemBoxFiles(p + 0x1d8);
    func_801EAE8C(p + 0x3e4);

    if (p[0x527] == 2) {
        func_8022CF2C(p + 0x440);
    }

    ((CSysWin*)(p + 0x4ac))->loadSystemArc();
    ((CSysWin*)(p + 0x4e8))->loadSystemArc();
}
#pragma pop

// Per-frame state machine update for the item-box grid: gate on the ready
// byte at +0x54 and the state word at +0x58, dispatch through MWCC's
// computed-jump switch, then run the common per-frame update tail.
extern "C" void UpdateItemBox(void* self, int r4) {
    u8* p = (u8*)self;
    if (!p[0x54]) return;
    u32 state = *(u32*)(p + 0x58);
    if (!state) return;

    // Bare switch: MWCC emits the single cmplwi 0x1a/bgt range check plus
    // the computed-jump table (gaps fall to the common tail) -- an explicit
    // `if (state <= 0x1a)` wrapper would add a second range branch.
    switch (state) {
        case 1:
            AdvanceOpenAnim(self);
            break;
        case 2:
            FinishOpenAnim(self);
            break;
        case 3:
            CheckState4_Animate((char*)self);
            break;
        case 4:
            AdvanceCloseAnim(self);
            break;
        case 5:
            FinishCloseAnim(self);
            break;
        case 6:
            if (func_801EB028(p + 0x3e4)) {
                *(u32*)(p + 0x58) = 0x19;
            }
            break;
        case 8:
            FinishNumSelect(self);
            break;
        case 9:
            ShowSubMenuCur(self);
            break;
        case 11:
            func_801CE524(self);
            break;
        case 12:
            if (func_801EB028(p + 0x3e4)) {
                *(u32*)(p + 0x58) = 0xA;
            }
            break;
        case 14:
            if (func_801EB028(p + 0x3e4)) {
                *(u32*)(p + 0x58) = 0x3;
            }
            break;
        case 15:
            ShowExchangeCur(self);
            break;
        case 17:
            FinishExchange(self);
            break;
        case 18:
            ShowPresentCur(self);
            break;
        case 20:
            FinishPresent(self);
            break;
        case 21:
            OnSysWinOpen(self);
            break;
        case 23:
            HandleSysWinOK(self);
            break;
        case 24:
            if (CSysWin_isActive(p + 0x4e8)) {
                *(u32*)(p + 0x58) = 0x13;
            }
            break;
        case 26:
            FinishSysWin(self);
            break;
    }

    // Virtual dispatch through the +0x44 layout object (vtable+0x38 slot);
    // method takes only the flag - this comes in r3, arg in r4.
    nw4r::lyt::Layout* layout = *(nw4r::lyt::Layout**)(p + 0x44);
    layout->Animate(0);

    func_801D202C(p + 0x70);
    func_801D202C(p + 0x88);
    func_801D202C(p + 0xa0);
    func_801D202C(p + 0xb8);
    func_801D202C(p + 0xd0);
    func_801D3160(p + 0xe8);
    updateItemBoxAnims(p + 0x1d8);
    func_801EAED4(p + 0x3e4);
    func_80208260(p + 0x418);
    func_8022CF7C(p + 0x440);
    func_8022DA58(p + 0x468);
    func_8022B748(p + 0x4ac);
    func_8022B748(p + 0x4e8);
}

// Draw callback: update the exchange/num-select widgets, then draw the
// layout and all sub-panels. optimize_for_size merges the r30/r31 saves
// into retail's stmw prologue.
#pragma optimize_for_size on
extern "C" void DrawItemBoxGrid(void* self, int r4) {
    u8* p = (u8*)self;
    if (!p[0x54]) return;
    if (!*(u32*)(p + 0x58)) return;

    renderItemBox(p + 0x1d8);
    drawLayout__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii((nw4r::lyt::Layout*)*(void**)(p + 0x44), (nw4r::lyt::DrawInfo*)r4, 0, 1);

    u8 r = p[0x2d4f];
    u8 v = (r != 0) ? r : 1;
    if ((u32)v > 1) {
        func_801D20B0(p + 0x88, (void*)r4);
    }

    func_801D31F8(p + 0xe8, (void*)r4);
    func_802082D0(p + 0x418, r4);
    func_8022CFEC(p + 0x440, r4);
    func_8022B7C8(p + 0x4ac, (void*)r4);
    func_8022B7C8(p + 0x4e8, (void*)r4);
    func_8022DAD8(p + 0x468, r4);
    func_801D20B0(p + 0xd0, (void*)r4);
    func_801D20B0(p + 0x70, (void*)r4);
    func_801D20B0(p + 0xa0, (void*)r4);
    func_801D20B0(p + 0xb8, (void*)r4);
    func_801EAF7C(p + 0x3e4, (void*)r4);
}
#pragma optimize_for_size off

// Teardown: release the CBdat banners, file handles, heap block, layout
// sub-objects and all UI panes, in retail's exact order. optimize_for_size
// merges the r30/r31 saves into retail's stmw prologue, and the delete-style
// releases reproduce the double null test (if + delete's own guard).
#pragma optimize_for_size on
extern "C" void UnloadItemBox(void* self, int r4) {
    u8* p = (u8*)self;

    if (p[0x543]) {
        getEntry__5CBdatFUl(2);
        getEntry__5CBdatFUl(5);
    }

    closeFileHandle__FPP11CFileHandle((void**)(p + 0x28));
    closeFileHandle__FPP11CFileHandle((void**)(p + 0x2c));
    closeFileHandle__FPP11CFileHandle((void**)(p + 0x30));
    closeFileHandle__FPP11CFileHandle((void**)(p + 0x34));

    void* ptr38 = *(void**)(p + 0x38);
    p[0x54] = 0;
    if (ptr38) {
        deallocate__Q23mtl10MemManagerFPv(ptr38);
        *(u32*)(p + 0x38) = 0;
    }

    void* obj44 = *(void**)(p + 0x44);
    if (obj44) {
        delete (nw4r::lyt::Layout*)obj44;
        *(u32*)(p + 0x44) = 0;
    }

    releaseArcResourceAccessor__FPQ34nw4r3lyt19ArcResourceAccessor(*(void**)(p + 0x3c));
    releaseArcResourceAccessor__FPQ34nw4r3lyt19ArcResourceAccessor(*(void**)(p + 0x40));

    void* obj5c = *(void**)(p + 0x5c);
    if (obj5c) {
        // CTagProcessor (built by __ct__CTagProcessor above) derives from
        // CTagProcessorBase : nw4r::ut::TagProcessorBase<wchar_t> (virtual
        // dtor at +0x08); deleting through the nw4r base keeps the retail
        // deleting-dtor dispatch without pulling CTagProcessor.hpp (whose
        // C++ func_801375A0 decl clashes with this TU's extern "C" one).
        delete (nw4r::ut::TagProcessorBase<wchar_t>*)obj5c;
        *(u32*)(p + 0x5c) = 0;
    }

    deleteRegion__17UnkClass_8045F564Fv(p + 0x8);
    deleteRegion__17UnkClass_8045F564Fv(p + 0x18);

    ((CBaseCur*)(p + 0x70))->cleanup();
    ((CBaseCur*)(p + 0x88))->cleanup();
    ((CBaseCur*)(p + 0xa0))->cleanup();
    ((CBaseCur*)(p + 0xb8))->cleanup();
    ((CBaseCur*)(p + 0xd0))->cleanup();

    func_801D3258(p + 0xe8);
    func_801D4174(p + 0x1d8);
    func_801EAF9C(p + 0x3e4);
    func_802082F0(p + 0x418);
    func_8022D018(p + 0x440);
    func_8022DB04(p + 0x468);
    func_8022B7F4(p + 0x4ac);
    func_8022B7F4(p + 0x4e8);
}
#pragma optimize_for_size off

// Check if item grid is fully ready.
extern "C" u32 IsItemBoxReady(void* self) {
    u8* p = (u8*)self;
    if (!func_801D32DC(p + 0xe8)) return 0;
    if (!getItemBoxState__FP12CItemBoxInfo(p + 0x1d8)) return 0;
    if (!func_801EB018(p + 0x3e4)) return 0;
    if (!((CExchangeWin*)(p + 0x440))->getField25()) return 0;
    if (!CSysWin_isReady(p + 0x4ac)) return 0;
    if (CSysWin_isReady(p + 0x4e8)) return p[0x60];
    return 0;
}


// Check if item grid is active.
extern "C" u32 IsItemBoxActive(void* self) {
    u8* p = (u8*)self;
    if (p[0x528]) return 1;
    if (CSysWin_getUnk34(p + 0x4ac)) return 1;
    if (CSysWin_getUnk34(p + 0x4e8)) return 1;
    if (p[0x544]) return 1;
    return func_801D3320(p + 0xe8);
}

// Get field depending on window state.
extern "C" u8 GetIdleFlag542(void* self) {
    if (CSysWin_getUnk34((u8*)self + 0x4e8)) return 0;
    s32 state = *(s32*)((u8*)self + 0x58);
    if (state != 3) return 0;
    return ((u8*)self)[0x542];
}

u8 CItemBoxGrid::GetField549() { return reinterpret_cast<CItemBoxGridFull*>(this)->bytes.field_549; }

// Check if any sub-system is active.
extern "C" u32 IsSubWinActive(void* self) {
    u8* p = (u8*)self;
    if (func_801EB020(p + 0x3e4)) return 1;
    if (func_80208358(p + 0x418)) return 1;
    if (((CExchangeWin*)(p + 0x440))->getField24()) return 1;
    if (func_8022DB6C(p + 0x468)) return 1;
    if (CSysWin_getUnk34(p + 0x4ac)) return 1;
    return CSysWin_getUnk34(p + 0x4e8);
}

// Initialize item display state. `item` (GetCellItemPtr result) is declared
// first so MWCC colors it to r31 (first-declared -> highest saved reg).
void func_801CB28C(void* self) {
    u32 item;   // GetCellItemPtr result
    u8* p = (u8*)self;
    if (*(u32*)(p + 0x58)) return;
    *(u32*)(p + 0x58) = 1;
    p[0x61] = 0;
    func_801CFD2C(self);
    func_801D0BD8(self);
    startItemBoxOpen(p + 0x1D8);
    func_801D4260(p + 0x1D8, (p + (s8)p[0x6F])[0x62]);
    u8 idx0 = p[0x525];
    u8 idx1 = p[0x524];
    u32 entry_idx = (idx1 + idx0 * 10) & 0xFF;
    u8* sub = p + 0x54C;
    item = (u32)GetCellItemPtr((CItemBoxGridFull*)sub, (u16)entry_idx);
    u32 r2 = GetCellKind((CItemBoxGridFull*)sub, (u16)entry_idx);
    func_801D47D4(p + 0x1D8, r2 & 0xFFFF, item, 1);
    u32 v = FormatCellName(sub, entry_idx);
    setItemBoxNamedText((void*)(p + 0x1D8), 1, (void*)v);
    func_801D216C(p + 0xB8, 0);
    p[0x549] = 0;
    if (p[0x527] == 4) setItemBoxResultPane(p + 0x1D8, 1);
}

// Advance item box state.
extern "C" void AdvanceBoxState(void* self) {
    u8* p = (u8*)self;
    if (*(s32*)(p + 0x58) != 3) return;
    if (func_801D3320(p + 0xe8)) return;
    *(u32*)(p + 0x58) = 4;
    ((nw4r::lyt::Layout*)(void*)*(u32*)(p + 0x44))->SetAnimationEnable((nw4r::lyt::AnimTransform*)(void*)*(u32*)(p + 0x50), 0);
    ((nw4r::lyt::Layout*)(void*)*(u32*)(p + 0x44))->SetAnimationEnable((nw4r::lyt::AnimTransform*)(void*)*(u32*)(p + 0x48), 0);
    ((nw4r::lyt::Layout*)(void*)*(u32*)(p + 0x44))->SetAnimationEnable((nw4r::lyt::AnimTransform*)(void*)*(u32*)(p + 0x4c), 1);
    p[0x61] = 0;
    func_801D216C(p + 0x70, 0);
    func_801D216C(p + 0x88, 0);
    func_801D216C(p + 0xd0, 0);
    func_801D216C(p + 0xb8, 0);
    advanceItemBoxState__FP12CItemBoxInfo(p + 0x1d8);
    if (!p[0x52c]) playUISound__FUl(6);
}

// Clear a 14-byte region (list/array init). Retail uses memset for the
// 12-byte block at +0x62, then two byte clears.
extern "C" void ClearListSlots(void* self) {
    u8* p = (u8*)self;
    memset(p + 0x62, 0, 12);
    p[0x6e] = 0;
    p[0x6f] = 0;
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
extern "C" __declspec(noinline) void IncListIndex(void* self) {
    u8* p = (u8*)self;
    if (func_801D3320(p + 0xe8)) return;
    if (p[0x528]) return;
    u8 idx = p[0x6f] + 1;
    p[0x6f] = idx;
    if ((s8)idx >= p[0x6e]) {
        p[0x6f] = 0;
    }
    func_801D05D4(self, 0);
    RebuildGridCat(self);
    playUISound__FUl(0x70);
}

// Decrement a list counter with wrap.
extern "C" __declspec(noinline) void DecListIndex(void* self) {
    u8* p = (u8*)self;
    if (func_801D3320(p + 0xe8)) return;
    if (p[0x528]) return;
    u8 idx = p[0x6f] - 1;
    p[0x6f] = idx;
    if ((s8)idx < 0) {
        p[0x6f] = p[0x6e] - 1;
    }
    func_801D05D4(self, 1);
    RebuildGridCat(self);
    playUISound__FUl(0x70);
}

// Item box advance handler. optimize_for_size reproduces retail's stmw r29
// prologue and 0x50 frame; the vt+0x10 dispatches use CBaseCur::setRootPaneTranslate
// cast class so MWCC emits lwz r12,0x10(r12); mtctr; bctrl.
#pragma push
#pragma optimize_for_size on
void func_801CB5F0(void* self) {
    u8* p = (u8*)self;
    if (p[0x542]) return;
    if (CSysWin_getUnk34(p + 0x4ac)) {
        if (CSysWin_isActive(p + 0x4ac)) {
            p[0x540] -= 1;
            if ((s8)p[0x540] < 0) p[0x540] = 1;
            UpdateCursors(self);
            playUISound__FUl(1);
        }
        return;
    }
    if (CSysWin_getUnk34(p + 0x4e8)) return;
    if (func_80208358(p + 0x418)) {
        if (func_80208360(p + 0x418)) {
            func_802083CC(p + 0x418);
            u8 temp[12];
            func_80208760(temp, p + 0x418);
            ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate((const nw4r::math::VEC3*)(temp));
            playUISound__FUl(1);
        }
        return;
    }
    if (func_8022DB6C(p + 0x468)) return;
    // Negated so the p544/p528/p527 chain nests in the false arm (retail
    // branches forward past the D3328 block instead of jumping to a shared end).
    if (!func_801D3320(p + 0xe8)) {
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
            if (func_801EB028(p + 0x3e4)) {
                u8* sub = p + 0x54c;
                u8 entry = (u8)(p[0x525] * 10 + p[0x524]);
                u32 kind = GetCellKind((CItemBoxGridFull*)sub, (u16)entry);
                BdatGetItemType(kind & 0xFFFF);
                s32 count = CheckCellState((CItemBoxGridFull*)sub, entry);
                if ((s8)count > 0) {
                    p[0x529] += 1;
                    if ((s8)p[0x529] > (s8)count) p[0x529] = 1;
                }
                func_801EB218(p + 0x3e4);
                func_801EB04C((void*)(p + 0x3e4), (u32)p[0x529]);
                u32 cost = func_801C5FC0((CItemBoxGridFull*)sub, entry);
                u32 total = (s8)p[0x529] * cost;
                func_801EB064((void*)(p + 0x3e4), total);
                goto sound_and_return;
            }
            return;
        } else if (p[0x527] == 2) {
            if (((CExchangeWin*)(p + 0x440))->getField27()) {
                p[0x529] -= 1;
                if ((s8)p[0x529] < 0) p[0x529] = 1;
                u8 temp[12];
                func_8022D0F8(temp, p + 0x440, p[0x529]);
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
            p[0x525] = (u8)(LookupIndexedByte((char*)self) - 1);
        } else if ((s8)p[0x525] < 0) {
            *(s8*)&p[0x525] = -2;
        }
        UpdateCursors(self);
        func_801D0328(self);
        goto sound_and_return;
    }
    {
        p[0x525] -= 1;
        if ((s8)p[0x525] < -1) {
            p[0x525] = (u8)(LookupIndexedByte((char*)self) - 1);
        }
        UpdateCursors(self);
        func_801D0328(self);
    }
    } else {
        if (func_801D3328(p + 0xe8)) {
            func_801D3620(p + 0xe8);
            u8 temp[12];
            func_801D3454(temp, p + 0xe8);
            ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate((const nw4r::math::VEC3*)(temp));
            goto sound_and_return;
        }
        return;
    }
sound_and_return:
    playUISound__FUl(1);
}
#pragma pop

__declspec(noinline) unsigned short ArrayGet12(const unsigned short* p, unsigned char i) {
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
__declspec(noinline) void CopyTabSlotVec(u32* dst, void* src, u32 idx) {
    if (idx >= 12) return;
    u32 v24, v28;
    u8* e = (u8*)src + idx * 12;
    v24 = *(u32*)(e + 0x18);
    v28 = *(u32*)(e + 0x1c);
    *(u32*)((u8*)dst + 4) = v28;
    *(u32*)((u8*)dst + 0) = v24;
    *(u32*)((u8*)dst + 8) = *(u32*)(e + 0x20);
}

// Right navigation. optimize_for_size: retail saves r29-r31 as one stmw
// block (-0x50 frame); sub/entry stay live across calls so MWCC colors them
// into the saved regs. Stack buffers mirror retail slots: 0x08/0x14/0x20 are
// 12-byte payloads, 0x2c is a 24-byte buffer filled by func_80208760. The
// do/while(0) gives every sound path one shared tail (retail branches to a
// single li r3,1/bl site) without goto-forced spills.
#pragma push
#pragma optimize_for_size on
void func_801CBA04(void* self) {
    u8* sub;
    u32 entry;
    u8* p = (u8*)self;
    if (p[0x542]) return;
    if (CSysWin_getUnk34(p + 0x4ac)) {
        if (!CSysWin_isActive(p + 0x4ac)) return;
        // blink counter wraps to 0 after reaching 1
        u8 val = p[0x540] + 1;
        p[0x540] = val;
        if ((s8)val > 1) p[0x540] = 0;
        UpdateCursors(self);
        playUISound__FUl(1);
        return;
    }
    if (CSysWin_getUnk34(p + 0x4e8)) return;
    if (func_80208358(p + 0x418)) {
        if (func_80208360(p + 0x418)) {
            func_8020844C(p + 0x418);
            u32 buf2c[3];
            func_80208760(buf2c, p + 0x418);
            ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate((const nw4r::math::VEC3*)(buf2c));
            playUISound__FUl(1);
        }
        return;
    }
    if (func_8022DB6C(p + 0x468)) return;
    if (func_801D3320(p + 0xe8)) {
        if (func_801D3328(p + 0xe8)) {
            func_801D3698(p + 0xe8);
            u32 buf20[3];
            func_801D3454(buf20, p + 0xe8);
            ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate((const nw4r::math::VEC3*)(buf20));
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
                if (!func_801EB028(p + 0x3e4)) return;
                sub = p + 0x54c;
                entry = (u8)(p[0x525] * 10 + p[0x524]);
                BdatGetItemType(GetCellKind((CItemBoxGridFull*)sub, (u16)entry) & 0xFFFF);
                s32 count = CheckCellState((CItemBoxGridFull*)sub, (u16)entry);
                if ((s8)count > 0) {
                    u8 val = p[0x529] - 1;
                    p[0x529] = val;
                    if ((s8)val < 1) p[0x529] = (u8)count;
                }
                func_801EB314(p + 0x3e4);
                func_801EB04C(p + 0x3e4, p[0x529]);
                func_801EB064(p + 0x3e4,
                              func_801C5FC0((CItemBoxGridFull*)sub, (u16)entry) * (s8)p[0x529]);
                goto sound_and_return2;
            } else if (p[0x527] == 2) {
                if (!((CExchangeWin*)(p + 0x440))->getField27()) return;
                u8 val = p[0x529] + 1;
                p[0x529] = val;
                if ((s8)val > 1) p[0x529] = 0;
                u32 buf08[3];
                func_8022D0F8(buf08, p + 0x440, p[0x529]);
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
                u8 max = (u8)LookupIndexedByte((char*)(p + 0x54c));
                if ((s8)p[0x525] >= max) *(s8*)&p[0x525] = -2;
            }
            UpdateCursors(self);
            func_801D0328(self);
            goto sound_and_return2;
        }
        {
            p[0x525] = p[0x525] + 1;
            u8 max = (u8)LookupIndexedByte((char*)(p + 0x54c));
            if ((s8)p[0x525] >= max) *(s8*)&p[0x525] = -1;
            UpdateCursors(self);
            func_801D0328(self);
        }
sound_and_return2:
    playUISound__FUl(1);
}
#pragma pop

// Down navigation. optimize_for_size: retail saves r29-r31 as one stmw block
// (-0x30 frame); sub (r29) and entry (r30) are declared at function scope so
// their live ranges span the calls and MWCC colors them to the saved regs.
#pragma optimize_for_size on
void func_801CBDE8(void* self) {
    // sub-array pointer and grid index stay live across calls -> MWCC colors
    // them into the stmw-saved r29/r30; buf/tmp mirror retail's stack slots.
    u32 entry;
    u8* sub;
    // One contiguous 24-byte region: retail uses sp+0x08 (buf) and sp+0x14 (tmp),
    // i.e. two adjacent 12-byte slots of a single stack block.
    u32 sbuf[6];
    u8* p = (u8*)self;
    if (p[0x542]) return;
    if (CSysWin_getUnk34(p + 0x4ac)) return;
    if (CSysWin_getUnk34(p + 0x4e8)) return;
    if (func_80208358(p + 0x418)) return;
    if (func_8022DB6C(p + 0x468)) {
        if (!func_8022DB74(p + 0x468)) return;
        if (func_8022E490(p + 0x468)) return;
        func_8022E3A8(p + 0x468);
        playUISound__FUl(1);
        return;
    }
    if (p[0x528]) {
        if (p[0x527] != 1) return;
        if (!func_801EB028(p + 0x3e4)) return;
        sub = p + 0x54c;
        entry = (u8)(p[0x524] + p[0x525] * 10);
        BdatGetItemType(GetCellKind((CItemBoxGridFull*)sub, (u16)entry) & 0xFFFF);
        s32 chk = CheckCellState((CItemBoxGridFull*)sub, entry);
        if ((s8)chk > 0) {
            // stack-count step of 10, clamped to a minimum of 1
            u8 cur = p[0x529];
            int cnt = cur - 10;
            p[0x529] = (u8)cnt;
            if ((s8)(u8)cnt < 1) p[0x529] = 1;
        }
        func_801EB04C(p + 0x3e4, p[0x529]);
        u32 v = func_801C5FC0((CItemBoxGridFull*)sub, entry);
        s32 total = (s8)p[0x529] * v;
        func_801EB064(p + 0x3e4, total);
        playUISound__FUl(1);
        return;
    }
    if (func_801D3320(p + 0xe8)) {
        if (!func_801D3328(p + 0xe8)) return;
        func_801D3724(p + 0xe8);
        func_801D3454(sbuf + 3, p + 0xe8);
        ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate((const nw4r::math::VEC3*)(sbuf + 3));
        playUISound__FUl(1);
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
        CopyTabSlotVec(sbuf, sub, (u8)(p[0x546] + (s8)p[0x545] * 4));
        ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate((const nw4r::math::VEC3*)(sbuf));
        playUISound__FUl(1);
        return;
    }
    s8 row = (s8)p[0x525];
    if (row == -2) return;
    if (row == -1) {
        DecListIndex(self);
        UpdateCursors(self);
        return;
    }
    if ((s8)p[0x524] == 0) {
        p[0x524] = 9;
        PrevGridPage(self);
        UpdateCursors(self);
    } else {
        s8 col = (s8)p[0x524] - 1;
        p[0x524] = (u8)col;
        if ((s8)p[0x524] < 0) p[0x524] = 9;
        UpdateCursors(self);
        func_801D0328(self);
    }
    playUISound__FUl(1);
}
#pragma optimize_for_size off

// Up navigation. Same shape as func_801CBDE8 (see notes there).
#pragma optimize_for_size on
void func_801CC0EC(void* self) {
    // sub-array pointer and grid index stay live across calls -> MWCC colors
    // them into the stmw-saved r29/r30; buf/tmp mirror retail's stack slots.
    u32 entry;
    u8* sub;
    u32 tmp[3];
    u32 buf[3];
    u8* p = (u8*)self;
    if (p[0x542]) return;
    if (CSysWin_getUnk34(p + 0x4ac)) return;
    if (CSysWin_getUnk34(p + 0x4e8)) return;
    if (func_80208358(p + 0x418)) return;
    if (func_8022DB6C(p + 0x468)) {
        if (!func_8022DB74(p + 0x468)) return;
        if (func_8022E490(p + 0x468)) return;
        func_8022E2F8(p + 0x468);
        playUISound__FUl(1);
        return;
    }
    if (p[0x528]) {
        if (p[0x527] != 1u) return;
        if (!func_801EB028(p + 0x3e4)) return;
        entry = (u8)(p[0x524] + p[0x525] * 10);
        sub = p + 0x54c;
        BdatGetItemType(GetCellKind((CItemBoxGridFull*)sub, (u16)entry) & 0xFFFF);
        s32 chk = (s8)CheckCellState((CItemBoxGridFull*)sub, entry);
        if (chk > 0) {
            // stack-count step of 10, clamped to the item's max stack
            u8 cur = p[0x529];
            int cnt = cur + 10;
            p[0x529] = (u8)cnt;
            if ((s8)(u8)cnt > chk) p[0x529] = (u8)chk;
        }
        func_801EB04C(p + 0x3e4, p[0x529]);
        s32 total = (s8)p[0x529] * func_801C5FC0((CItemBoxGridFull*)sub, entry);
        func_801EB064(p + 0x3e4, total);
        playUISound__FUl(1);
        return;
    }
    if (func_801D3320(p + 0xe8)) {
        if (!func_801D3328(p + 0xe8)) return;
        func_801D377C(p + 0xe8);
        func_801D3454(tmp, p + 0xe8);
        ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate((const nw4r::math::VEC3*)tmp);
        playUISound__FUl(1);
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
        CopyTabSlotVec(buf, sub, (u8)(p[0x546] + (u8)p[0x545] * 4));
        ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate((const nw4r::math::VEC3*)buf);
        playUISound__FUl(1);
        return;
    }
    s8 row = (s8)p[0x525];
    if (row == -2) return;
    if (row == -1) {
        IncListIndex(self);
        UpdateCursors(self);
        return;
    }
    if ((s8)p[0x524] == 9) {
        p[0x524] = 0;
        NextGridPage(self);
        UpdateCursors(self);
    } else {
        p[0x524]++;
        if ((s8)p[0x524] >= 10) p[0x524] = 0;
        UpdateCursors(self);
        func_801D0328(self);
    }
    playUISound__FUl(1);
}

// Grid state update with conditions.
// Grid state update.
// optimize_for_size: retail uses stmw r30/lmw r30; sub (r31) is declared
// before p (r30) so MWCC colors it to the higher saved register.
#pragma optimize_for_size on
__declspec(noinline) void NextGridPage(void* self) {
    u8* sub;
    u8* p = (u8*)self;
    if (func_801D3320(p + 0xe8)) return;
    if (p[0x528]) return;
    if (CSysWin_getUnk34(p + 0x4ac)) return;
    if (CSysWin_getUnk34(p + 0x4e8)) return;
    if (func_80208358(p + 0x418)) return;
    if (func_8022DB6C(p + 0x468)) return;
    sub = p + 0x54c;
    IncGridPageIndex((CItemBoxGridFull*)sub);
    // Retail calls LookupIndexedByte twice: once for the compare, once for
    // the store (subi from the fresh call result).
    // retail branches over the store when a < b (bc 12,0), so negate
    if (!((s8)p[0x525] < (u8)LookupIndexedByte((char*)sub)))
        p[0x525] = (u8)(LookupIndexedByte((char*)sub) - 1);
    func_801CFFEC(self);
    func_801D0328(self);
    u8 rows = sub[0x2803];
    u8 f = rows == 0 ? 1 : rows;
    if (f != 1) playUISound__FUl(0xa);
}
#pragma optimize_for_size off

// Grid state update (decrement variant).
#pragma optimize_for_size on
__declspec(noinline) void PrevGridPage(void* self) {
    u8* sub;
    u8* p = (u8*)self;
    if (func_801D3320(p + 0xe8)) return;
    if (p[0x528]) return;
    if (CSysWin_getUnk34(p + 0x4ac)) return;
    if (CSysWin_getUnk34(p + 0x4e8)) return;
    if (func_80208358(p + 0x418)) return;
    if (func_8022DB6C(p + 0x468)) return;
    sub = p + 0x54c;
    DecGridPageIndex((CItemBoxGridFull*)sub);
    // retail branches over the store when a < b (bc 12,0), so negate
    if (!((s8)p[0x525] < (u8)LookupIndexedByte((char*)sub)))
        p[0x525] = (u8)(LookupIndexedByte((char*)sub) - 1);
    func_801CFFEC(self);
    func_801D0328(self);
    u8 rows = sub[0x2803];
    u8 f = rows == 0 ? 1 : rows;
    if (f != 1) playUISound__FUl(0xa);
}
#pragma optimize_for_size off

// Guard chain shares a single exit label in retail (every check branches to
// the epilogue), so express the checks as one && chain.
#pragma optimize_for_size on
void OpenSortMenu(void* self) {
    u8* p = (u8*)self;
    // one shared stack buffer: the merge result lands in words [3] and is
    // consumed before func_801D3454 overwrites the whole array (retail
    // reuses the same frame slots).
    u32 buf[5];
    if (!p[0x542] && !p[0x528] && !CSysWin_getUnk34(p + 0x4ac) &&
        !CSysWin_getUnk34(p + 0x4e8) && !func_80208358(p + 0x418) &&
        !func_8022DB6C(p + 0x468) && !p[0x544]) {
        if (func_801D3320(p + 0xe8)) {
            if (!func_801D3328(p + 0xe8)) return;
            UpdateCursors(self);
            func_801D216C(p + 0xa0, 0);
            func_801D3408(p + 0xe8);
            playUISound__FUl(6);
            return;
        }
        // state word is compared signed (retail cmpwi)
        if (*(s32*)(p + 0x58) != 3) return;
        if (!func_801D3328(p + 0xe8)) return;
        // fetch the layout pane and resolve two tag strings through its
        // vtable+0x3C accessor, then merge them back into the window text.
        void* obj = *(void**)(p + 0x44);
        const char* strs = lbl_eu_8050566C;
        void* pane = *(void**)((char*)obj + 0x10);
        // MWCC evaluates args right-to-left, so listing the 0x301 fetch as
        // the third argument makes it execute first, matching retail.
        func_80137924(&buf[3], ((nw4r::lyt::Pane*)pane)->FindPaneByName((char*)strs + 0x2f8, 1),
                      ((nw4r::lyt::Pane*)pane)->FindPaneByName((char*)strs + 0x301, 1), pane);
        func_801D3430(p + 0xe8, &buf[3]);
        func_801D353C(p + 0xe8, (u8)(p[0x547] + p[0x548]));
        func_801D3454(buf, p + 0xe8);
        ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate((const nw4r::math::VEC3*)(buf));
        func_801D216C(p + 0xa0, 1);
        func_801D216C(p + 0x70, 0);
        func_801D216C(p + 0xb8, 0);
        func_801D3330(p + 0xe8);
        p[0x526] = 0;
        playUISound__FUl(2);
    }
}
#pragma optimize_for_size off

// optimize_for_size: retail keeps the stmw r30 / lmw r30 block save.
#pragma optimize_for_size on
// Mirrors retail's label structure: guard failures branch to the bare exit,
// handled arms fall into the single shared r4-check tail (keeps MWCC from
// duplicating the tail per return path).
#pragma optimize_for_size on
void HandleCancelBtn(void* self, int r4) {
    u8* p = (u8*)self;
    if (CSysWin_getUnk34(p + 0x4ac)) {
        if (!CSysWin_isActive(p + 0x4ac)) goto exit;
        func_8022B8E4(p + 0x4ac);
        func_801D216C(p + 0xa0, 0);
        p[0x540] = 1;
        *(u32*)(p + 0x58) = 0x17;
        goto exit;
    }
    if (CSysWin_getUnk34(p + 0x4e8)) {
        if (!CSysWin_isActive(p + 0x4e8)) goto exit;
        func_8022B8E4(p + 0x4e8);
        func_801D216C(p + 0x70, 0);
        *(u32*)(p + 0x58) = 0x14;
        goto exit;
    }
    if (func_80208358(p + 0x418)) {
        if (!func_80208360(p + 0x418)) goto exit;
        UpdateCursors(self);
        func_801D216C(p + 0x70, 1);
        func_801D216C(p + 0xa0, 0);
        func_80208838(p + 0x418);
        func_802083A4(p + 0x418);
        *(u32*)(p + 0x58) = 8;
        p[0x528] = 0;
        // retail inlines the r4-check tail here (copy 1 of 3)
        if (!r4) playUISound__FUl(6);
        goto exit;
    }
    if (func_8022DB6C(p + 0x468)) {
        if (!func_8022DB74(p + 0x468)) goto exit;
        if (func_8022E490(p + 0x468)) {
            func_8022DD68(p + 0x468);
            *(u32*)(p + 0x58) = 0x11;
            UpdateCursors(self);
            func_801D216C(p + 0x70, 1);
            func_801D216C(p + 0xd0, 0);
        } else if (func_8022E488(p + 0x468)) {
            func_8022E3AC(p + 0x468);
            u32 buf[4];
            func_8022E498(buf, p + 0x468);
            ((CBaseCur*)(p + 0xd0))->setRootPaneTranslate((const nw4r::math::VEC3*)(buf));
        } else {
            func_8022DD68(p + 0x468);
            *(u32*)(p + 0x58) = 0x11;
            UpdateCursors(self);
            func_801D216C(p + 0x70, 1);
            func_801D216C(p + 0xd0, 0);
        }
        // tail copy 2
        if (!r4) playUISound__FUl(6);
        goto exit;
    }
    if (p[0x528]) {
        if (p[0x527] == 1) {
            if (!func_801EB028(p + 0x3e4)) goto exit;
            if (*(s32*)(p + 0x58) != 0x19) goto exit;
            func_801EB178(p + 0x3e4);
            *(u32*)(p + 0x58) = 0x1a;
        } else if (p[0x527] == 2) {
            if (!((CExchangeWin*)(p + 0x440))->getField27()) goto exit;
            func_8022D0D0(p + 0x440);
            *(u32*)(p + 0x58) = 0x0e;
            func_801D216C(p + 0x70, 1);
            func_801D216C(p + 0xa0, 0);
            UpdateCursors(self);
        }
        goto check_tail;
    }
    if (p[0x544]) {
        p[0x544] = 0;
        func_801D216C(p + 0x70, 1);
        func_801D216C(p + 0xa0, 0);
        goto check_tail;
    }
    if (func_801D3320(p + 0xe8)) {
        if (!func_801D3328(p + 0xe8)) goto exit;
        UpdateCursors(self);
        func_801D216C(p + 0xa0, 0);
        func_801D3408(p + 0xe8);
    } else if (p[0x527] == 4) {
        ((s8*)p)[0x525] = -2;
        UpdateCursors(self);
    }
check_tail:
    // tail copy 3 (shared by the p[0x528]/p[0x544]/final arms)
    if (!r4) playUISound__FUl(6);
exit:;
}
#pragma optimize_for_size off



// A-confirm / window-result handler. optimize_for_size reproduces retail's
// stmw r25 / 96-byte frame; entry index is the usual col + row*10 cell.
#pragma optimize_for_size on
void func_801CCAF0(void* self) {
    void* item;
    CItemBoxGridFull* sub;
    u8 entry;
    u8* p = (u8*)self;
    if (CSysWin_getUnk34(p + 0x4ac)) {
        if (CSysWin_isActive(p + 0x4ac)) {
            func_8022B8E4(p + 0x4ac);
            func_801D216C(p + 0xa0, 0);
            *(u32*)(p + 0x58) = 0x17;
        }
        return;
    }
    if (CSysWin_getUnk34(p + 0x4e8)) {
        if (CSysWin_isActive(p + 0x4e8)) {
            func_8022B8E4(p + 0x4e8);
            func_801D216C(p + 0x70, 0);
            *(u32*)(p + 0x58) = 0x14;
        }
        return;
    }
    if (func_80208358(p + 0x418)) {
        if (!func_80208360(p + 0x418)) return;
        if ((func_802087B8(p + 0x418) & 0xFF) == 1) {
            sub = (CItemBoxGridFull*)(p + 0x54c);
            entry = (u8)(p[0x524] + p[0x525] * 10);
            item = GetCellItemPtr((CItemBoxGridFull*)sub, (u16)entry);
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
            RebuildGridCat(p);
            playUISound__FUl(0x7a);
            UpdateCursors(p);
        } else {
            u8* listRow = p + (s8)p[0x6f];
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
                        void* obj = (void*)func_8009EC9C(val & 0xFFFF);
                        if (*(s32*)((u8*)obj + 0x176c) != 1) count2++;
                    }
                }
                if (count2 <= 1) {
                    playUISound__FUl(5);
                    return;
                }
            }
            playUISound__FUl(3);
        }
        func_801D216C(p + 0xa0, 0);
        func_802083A4(p + 0x418);
        *(u32*)(p + 0x58) = 8;
        return;
    }
    if (func_8022DB6C(p + 0x468)) {
        if (!func_8022DB74(p + 0x468)) return;
        if (func_8022E488(p + 0x468)) {
            if (func_8022E490(p + 0x468)) {
                func_8022DD68(p + 0x468);
                *(u32*)(p + 0x58) = 0x11;
                UpdateCursors(p);
                func_801D216C(p + 0x70, 1);
                func_801D216C(p + 0xd0, 0);
            } else {
                sub = (CItemBoxGridFull*)(p + 0x54c);
                entry = (u8)(p[0x524] + p[0x525] * 10);
                item = GetCellItemPtr((CItemBoxGridFull*)sub, (u16)entry);
                u32 kind = GetCellKind((CItemBoxGridFull*)sub, (u16)entry);
                s32 count = CheckCellState(sub, entry);
                func_8022DD90(p + 0x468);
                func_801D216C(p + 0xd0, 0);
                u32 teachType = func_8022E4FC(p + 0x468);
                u32 teachState = func_8022E504(p + 0x468);
                if ((u8)teachState == 1) {
                    u32 table = lbl_eu_80664104;
                    if ((BdatGetU8Direct(table, (const char*)&lbl_eu_8050566C[0x212],
                                       BdatGetItemId(kind & 0xFFFF) & 0xFFFF) & 0xFF) == 7) {
                        incrementEventCounter__FUl(0x87);
                    }
                } else if ((u8)teachState == 2) {
                    if ((*(u32*)item >> 20) == 0x75a) incrementEventCounter__FUl(0x88);
                } else if ((u8)teachState == 4) {
                    if ((*(u32*)item >> 20) == 0x775) incrementEventCounter__FUl(0x8a);
                } else if ((u8)teachState == 5) {
                    u32 table = lbl_eu_80664104;
                    if ((BdatGetU8Direct(table, (const char*)&lbl_eu_8050566C[0x212],
                                       BdatGetItemId(kind & 0xFFFF) & 0xFFFF) & 0xFF) == 2) {
                        incrementEventCounter__FUl(0x8b);
                    }
                }
                if ((u8)teachType == 3 || (u8)teachType == 8) {
                    if ((u8)teachState == 1) incrementEventCounter__FUl(0x89);
                } else if ((u8)teachType == 6) {
                    u32 table = lbl_eu_80664104;
                    if ((BdatGetU8Direct(table, (const char*)&lbl_eu_8050566C[0x212],
                                       BdatGetItemId(kind & 0xFFFF) & 0xFFFF) & 0xFF) == 5) {
                        incrementEventCounter__FUl(0x8d);
                    }
                } else if ((u8)teachType == 7) {
                    incrementEventCounter__FUl(0x8c);
                }
                if ((*(u32*)item >> 20) == 0x867) incrementEventCounter__FUl(0x8e);
                DispatchItemEvt(p, item, (s8)count - 1);
                RebuildGridCat(p);
            }
        } else {
            func_8022E3AC(p + 0x468);
            u8 temp[16];
            func_8022E498(temp, p + 0x468);
            ((CBaseCur*)(p + 0xd0))->setRootPaneTranslate((const nw4r::math::VEC3*)temp);
        }
        playUISound__FUl(3);
        return;
    }
    if (func_801D3320(p + 0xe8)) {
        if (func_801D3328(p + 0xe8)) {
            u32 val = func_801D37F4(p + 0xe8);
            CItemBlock_setFlag120EC((p + (s8)p[0x6f])[0x62], val & 0xFF);
            p[0x547] = (u8)func_801D3808(p + 0xe8);
            p[0x548] = (u8)func_801D3810(p + 0xe8);
            HandleCancelBtn(p, 1);
            p[0x524] = 0;
            if ((s8)p[0x525] > 0) p[0x525] = 0;
            UpdateCursors(p);
            p[0x2d50] = 0;
            RebuildGridCat(p);
            playUISound__FUl(3);
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
            InitGridItems((CItemBoxGridFull*)(p + 0x54c));
            playUISound__FUl(4);
            return;
        }
        sub = (CItemBoxGridFull*)(p + 0x54c);
        entry = (u8)(p[0x524] + cursor * 10);
        u32 cap = GetCategoryCap(sub) & 0xFF;
        if ((CountMarkedCells(sub) & 0xFF) >= cap) {
            if (((int (*)(CItemBoxGridFull*, u16))GetCellByte8)(sub, entry)) {
                ToggleCellMark(sub, entry);
                func_801CFFEC(p);
                func_801D0328(p);
                func_801D0BD8(p);
                playUISound__FUl(6);
                return;
            }
            if (!(GetCellKind((CItemBoxGridFull*)sub, (u16)entry) & 0xFFFF)) return;
            *(u32*)(p + 0x58) = 0x12;
            u32 msg = (u32)BdatTouchStringCell((void*)&lbl_eu_8050566C[0x14f], &lbl_eu_8050566C[0x158], 0xa1);
            func_8022B90C(p + 0x4e8, 0);
            func_8022B9B4(p + 0x4e8, msg, 0);
            func_8022BFC8(p + 0x4e8, 1);
            func_8022B8B8(p + 0x4e8);
            func_801D216C(p + 0x70, 0);
            playUISound__FUl(5);
            return;
        }
        if ((u8)GetCellFlag0(sub, entry)) return;
        if (!(GetCellKind((CItemBoxGridFull*)sub, (u16)entry) & 0xFFFF)) return;
        ToggleCellMark(sub, entry);
        func_801CFFEC(p);
        func_801D0328(p);
        func_801D0BD8(p);
        if (((int (*)(CItemBoxGridFull*, u16))GetCellByte8)(sub, entry)) {
            playUISound__FUl(3);
        } else {
            playUISound__FUl(6);
        }
        return;
    }
    if (mode == 1) {
        if (p[0x528]) {
            if (!func_801EB028(p + 0x3e4)) return;
            if (*(s32*)(p + 0x58) != 0x19) return;
            sub = (CItemBoxGridFull*)(p + 0x54c);
            entry = (u8)(p[0x524] + p[0x525] * 10);
            item = GetCellItemPtr((CItemBoxGridFull*)sub, (u16)entry);
            GetCellKind((CItemBoxGridFull*)sub, (u16)entry);
            s32 count = CheckCellState(sub, entry);
            u32 type = (*(u32*)item >> 16) & 0xF;
            if ((u8)GetCellFlag0(sub, entry)) {
                s16 sid = GetCellItemId(sub, entry);
                func_801D0E88(p, (int)type, (int)sid);
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
            s32 add = (s8)p[0x529] * (s32)func_801C5FC0(sub, entry);
            CItemBlock_setCount((s32)((u32)CItemBlock_getPtr20E8() + add));
            DispatchItemEvt(p, item, (s8)count - (s8)p[0x529]);
            RebuildGridCat(p);
            func_801EB178(p + 0x3e4);
            *(u32*)(p + 0x58) = 0x1a;
            playUISound__FUl(0x2f);
            return;
        }
        if (!func_801EB028(p + 0x3e4)) return;
        if (*(s32*)(p + 0x58) != 3) return;
        {
            sub = (CItemBoxGridFull*)(p + 0x54c);
            entry = (u8)((s8)p[0x524] + (s8)p[0x525] * 10);
            u32 kindSell = GetCellKind((CItemBoxGridFull*)sub, (u16)entry);
            if (!(kindSell & 0xFFFF)) return;
            p[0x529] = 1;
            func_801EB030(p + 0x3e4, (void*)FormatCellName(sub, entry));
            func_801EB04C(p + 0x3e4, p[0x529]);
            func_801EB064(p + 0x3e4, (s8)p[0x529] * func_801C5FC0(sub, entry));
            if (((int (*)(CItemBoxGridFull*, u16))GetCellFlag1)(sub, entry)) {
                *(u32*)(p + 0x58) = 0x12;
                u32 msg2 = (u32)BdatTouchStringCell((void*)&lbl_eu_8050566C[0x310], &lbl_eu_8050566C[0x158], 0x17);
                func_8022B90C(p + 0x4e8, 0);
                func_8022B9B4(p + 0x4e8, msg2, 0);
                func_8022BFC8(p + 0x4e8, 1);
                func_8022B8B8(p + 0x4e8);
                func_801D216C(p + 0x70, 0);
            } else if ((u8)GetCellFlag0(sub, entry)) {
                void* item2 = GetCellItemPtr((CItemBoxGridFull*)sub, (u16)entry);
                if (((*(u32*)item2 >> 16) & 0xF) == 2) {
                    *(u32*)(p + 0x58) = 0x12;
                    u32 msg3 = (u32)BdatTouchStringCell((void*)&lbl_eu_8050566C[0x310], &lbl_eu_8050566C[0x158], 0x16);
                    func_8022B90C(p + 0x4e8, 0);
                    func_8022B9B4(p + 0x4e8, msg3, 0);
                    func_8022BFC8(p + 0x4e8, 1);
                    func_8022B8B8(p + 0x4e8);
                    func_801D216C(p + 0x70, 0);
                } else {
                    *(u32*)(p + 0x58) = 0x15;
                    u32 msg4 = (u32)BdatTouchStringCell((void*)&lbl_eu_8050566C[0x310], &lbl_eu_8050566C[0x158], 0x13);
                    u32 msg5 = (u32)BdatTouchStringCell((void*)&lbl_eu_8050566C[0x310], &lbl_eu_8050566C[0x158], 0x14);
                    u32 msg6 = (u32)BdatTouchStringCell((void*)&lbl_eu_8050566C[0x310], &lbl_eu_8050566C[0x158], 0x15);
                    func_8022B9B4(p + 0x4ac, msg4, 0);
                    func_8022BF6C(p + 0x4ac, msg5, msg6);
                    func_8022BFC8(p + 0x4ac, 0);
                    func_8022B8B8(p + 0x4ac);
                    func_801D216C(p + 0x70, 0);
                    p[0x540] = 1;
                }
            } else {
                u8 listVal2 = p[0x62 + (s8)p[0x6f]];
                if (listVal2 == 0xd) {
                    u32 k12 = GetCellKind((CItemBoxGridFull*)sub, (u16)entry);
                    if (func_801D12D4(p, k12 & 0xFFFF)) {
                        *(u32*)(p + 0x58) = 0x15;
                        u32 msg7 = (u32)BdatTouchStringCell((void*)&lbl_eu_8050566C[0x310], &lbl_eu_8050566C[0x158], 0x19);
                        u32 msg8 = (u32)BdatTouchStringCell((void*)&lbl_eu_8050566C[0x310], &lbl_eu_8050566C[0x158], 0x14);
                        u32 msg9 = (u32)BdatTouchStringCell((void*)&lbl_eu_8050566C[0x310], &lbl_eu_8050566C[0x158], 0x15);
                        func_8022B9B4(p + 0x4ac, msg7, 0);
                        func_8022BF6C(p + 0x4ac, msg8, msg9);
                        func_8022BFC8(p + 0x4ac, 0);
                        func_8022B8B8(p + 0x4ac);
                        func_801D216C(p + 0x70, 0);
                        p[0x540] = 1;
                    } else {
                        goto sell_qty;
                    }
                } else {
                sell_qty:
                    u32 lv = (p + (s8)p[0x6f])[0x62];
                    if (lv - 2 <= 7) {
                        func_801EB410(p + 0x3e4, 0);
                    } else {
                        func_801EB410(p + 0x3e4, 1);
                    }
                    func_801EB0D4(p + 0x3e4);
                    *(u32*)(p + 0x58) = 0x18;
                    p[0x528] = 1;
                    func_801D216C(p + 0x70, 0);
                    playUISound__FUl(3);
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
                item = GetCellItemPtr((CItemBoxGridFull*)sub, (u16)entry);
                u32 kind = GetCellKind((CItemBoxGridFull*)sub, (u16)entry);
                s32 count = CheckCellState(sub, entry);
                if (!(kind & 0xFFFF)) return;
                u32 v1 = func_801C618C(sub, *(u16*)(p + 0x52e), 0, *(u16*)(p + 0x52a));
                u32 v2 = func_801C618C(sub, (u16)kind, item, *(u16*)(p + 0x52a));
                if (v1 > v2) return;
                u16 nVal = BdatGetU16Direct((void*)lbl_eu_80664098, (const char*)&lbl_eu_8050566C[0x319], *(u16*)(p + 0x52a));
                u32 test = func_801C618C(sub, nVal, 0, 0);
                s8 leftover = (s8)count;
                s32 neg = -((s32)(v2 - v1) < (s32)test);
                p[0x52d] = (u8)neg;
                DispatchItemEvt(p, item, leftover - 1);
                p[0x52c] = 1;
                func_801393CC(*(u16*)(p + 0x52e));
                BdatGetItemType(*(u16*)(p + 0x52e));
                BdatGetItemId(*(u16*)(p + 0x52e));
                CItem_thunkAllocRecord(*(u16*)(p + 0x52e), 1);
                func_80140E00(2, *(u16*)(p + 0x52e), 0);
                incrementEventCounter__FUl(0xbb);
                incrementEventCounter__FUl(0xbc);
                incrementEventCounter__FUl(0xbd);
                switch ((s32)*(u16*)(p + 0x52e)) {
                case 0x7ee: incrementEventCounter__FUl(0xbe); break;
                case 0x835: incrementEventCounter__FUl(0xbf); break;
                case 0x7ef: incrementEventCounter__FUl(0xc0); break;
                case 0x837: incrementEventCounter__FUl(0xc1); break;
                case 0x836: incrementEventCounter__FUl(0xc2); break;
                }
            }
            func_8022D0D0(p + 0x440);
            *(u32*)(p + 0x58) = 0xe;
            func_801D216C(p + 0x70, 1);
            func_801D216C(p + 0xa0, 0);
            UpdateCursors(p);
            if ((s8)p[0x529] == 0) playUISound__FUl(0x31);
            else playUISound__FUl(3);
            return;
        }
        if (!((CExchangeWin*)(p + 0x440))->getField27()) return;
        if (*(s32*)(p + 0x58) != 3) return;
        {
            sub = (CItemBoxGridFull*)(p + 0x54c);
            u32 kind = GetCellKind((CItemBoxGridFull*)sub, (u16)(u8)(p[0x524] + p[0x525] * 10));
            if (!(kind & 0xFFFF)) return;
            char fmtBuf[0x28];
            sprintf(fmtBuf, (const char*)&lbl_eu_8050566C[0x14c], func_801394D4(*(u16*)(p + 0x52e)));
            func_8022D19C(p + 0x440, fmtBuf, (u32)func_801394D4(kind & 0xFFFF));
            func_8022D0A4(p + 0x440);
            p[0x528] = 1;
            *(u32*)(p + 0x58) = 0xc;
            p[0x529] = 1;
            func_801D216C(p + 0x70, 0);
            playUISound__FUl(3);
            return;
        }
    }
    if (p[0x528]) return;
    if (!func_80208360(p + 0x418)) return;
    if (!func_8022DB74(p + 0x468)) return;
    {
        sub = (CItemBoxGridFull*)(p + 0x54c);
        entry = (u8)((s8)p[0x524] + (s8)p[0x525] * 10);
        if ((u8)GetCellFlag1(sub, entry)) {
            playUISound__FUl(5);
            return;
        }
        if (!(GetCellKind((CItemBoxGridFull*)sub, (u16)entry) & 0xFFFF)) {
            playUISound__FUl(5);
            return;
        }
        if ((u8)GetCellFlag0(sub, entry)) {
            playUISound__FUl(5);
            return;
        }
        u8 listVal3 = p[0x62 + (s8)p[0x6f]];
        if (listVal3 == 0xc) {
            playUISound__FUl(5);
            return;
        }
        u32 sendVal = 0xFF;
        if (p[0x527] != 3) sendVal = listVal3;
        func_802084D4(p + 0x418, sendVal & 0xFF);
        func_801D216C(p + 0x70, 0);
        func_80208368(p + 0x418);
        *(u32*)(p + 0x58) = 6;
        p[0x528] = 1;
        playUISound__FUl(3);
    }
}
#pragma optimize_for_size off

// Store value and call helpers.
void SetInfoMsgId(void* self, u32 val) {
    *(u16*)((u8*)self + 0x52e) = (u16)val;
    void* res = func_801394D4(val & 0xFFFF);
    setItemBoxNamedText((void*)((u8*)self + 0x1d8), 0, res);
    RebuildGridCat(self);
}

// Get field depending on obj state.
extern "C" u8 GetExchFlag52C(void* self) {
    if (!((CExchangeWin*)((u8*)self + 0x440))->getField27()) return 0;
    s32 state = *(s32*)((u8*)self + 0x58);
    if (state != 3) return 0;
    return ((u8*)self)[0x52c];
}

u8 CItemBoxGrid::GetField52D() { return reinterpret_cast<CItemBoxGridFull*>(this)->bytes.field_52D; }

// Periodic grid tick: when the page-detail window (0x544) is active, either
// drive it (if the +0x4e8 win is up) or populate it from the current list
// selection; otherwise, when in state 3, scan for the first non-empty cell
// and page to it. optimize_for_size merges the r29-r31 saves into retail's
// _savegpr_29 prologue.
#pragma optimize_for_size on
void func_801CDC40(void* self) {
    u8* p = (u8*)self;
    if (p[0x528]) return;
    if (CSysWin_getUnk34(p + 0x4ac)) return;
    if (func_80208358(p + 0x418)) return;
    if (func_8022DB6C(p + 0x468)) return;
    if (func_801D3320(p + 0xe8)) return;
    if (p[0x544]) {
        if (CSysWin_getUnk34(p + 0x4e8)) {
            if (!CSysWin_isActive(p + 0x4e8)) return;
            func_8022B8E4(p + 0x4e8);
            func_801D216C(p + 0x70, 0);
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
            obj = (void*)func_801D3C74(list, idx);
        } else {
            obj = (void*)BdatGetPtrDirect((const void*)*(u32*)(lbl_eu_80664510), &lbl_eu_8050566C[0x321], val & 0xFFFF);
        }
        func_8022B90C(p + 0x4e8, 0);
        func_8022B9B4(p + 0x4e8, (u32)obj, 0);
        func_8022BFC8(p + 0x4e8, 1);
        func_8022B8B8(p + 0x4e8);
        func_801D216C(p + 0xa0, 0);
        return;
    }
    if (*(u32*)(p + 0x58) != 3) return;
    if (CSysWin_getUnk34(p + 0x4e8)) return;
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
        int n = (int)(lbl_eu_80667F74 * (float)v);
        p[0x545] = (u8)n;
        p[0x546] = (u8)(v - (s8)(u8)n * 4);
        func_801D216C(p + 0x70, 0);
        func_801D216C(p + 0xa0, 1);
        u32 buf[3];
        CopyTabSlotVec(buf, list, (u32)v);
        ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate((const nw4r::math::VEC3*)(buf));
        playUISound__FUl(2);
    } else {
        playUISound__FUl(5);
    }
}
#pragma optimize_for_size off

// Check conditions and update state.
extern "C" void SelectCatRow(void* self) {
    u8* p = (u8*)self;
    if (*(s32*)(p + 0x58) != 3) return;
    if (p[0x528]) return;
    if (((s8*)p)[0x525] == -1) return;
    if (CSysWin_getUnk34(p + 0x4ac)) return;
    if (CSysWin_getUnk34(p + 0x4e8)) return;
    if (func_80208358(p + 0x418)) return;
    if (func_8022DB6C(p + 0x468)) return;
    if (func_801D3320(p + 0xe8)) return;
    if (p[0x544]) return;
    ((s8*)p)[0x525] = -1;
    UpdateCursors(self);
    func_801D0328(self);
    playUISound__FUl(2);
}

// Check prerequisites and return a state code: each sub-window must be
// idle (nonzero -> 0); then 2 while the item box is animating, 5 while a
// confirm prompt is up, otherwise derive from the selected category byte
// (0x527) and the pending-selection byte (0x525).
extern "C" u32 GetPromptState(void* self) {
    u8* p = (u8*)self;
    if (*(s32*)(p + 0x58) != 3) return 0;
    if (func_801EB020(p + 0x3e4)) return 0;
    if (func_80208358(p + 0x418)) return 0;
    if (((CExchangeWin*)(p + 0x440))->getField24()) return 0;
    if (func_8022DB6C(p + 0x468)) return 0;
    if (CSysWin_getUnk34(p + 0x4ac)) return 0;
    if (CSysWin_getUnk34(p + 0x4e8)) return 0;
    if (func_801D3320(p + 0xe8)) return 2;
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
__declspec(noinline) void AdvanceOpenAnim(void* self) { // noinline: dispatch table calls keep `bl`
    u8* p = (u8*)self;
    if (!advanceAnimTransform__FPQ34nw4r3lyt13AnimTransformf((void*)*(u32*)(p + 0x48), lbl_eu_80667F78)) return;
    ((nw4r::lyt::Layout*)(void*)*(u32*)(p + 0x44))->SetAnimationEnable((nw4r::lyt::AnimTransform*)(void*)*(u32*)(p + 0x50), 0);
    ((nw4r::lyt::Layout*)(void*)*(u32*)(p + 0x44))->SetAnimationEnable((nw4r::lyt::AnimTransform*)(void*)*(u32*)(p + 0x48), 0);
    ((nw4r::lyt::Layout*)(void*)*(u32*)(p + 0x44))->SetAnimationEnable((nw4r::lyt::AnimTransform*)(void*)*(u32*)(p + 0x4C), 1);
    *(u32*)(p + 0x58) = 2;
}

extern "C" __declspec(noinline) void FinishOpenAnim(void* self) { // noinline: dispatch table calls keep `bl`
    u8* p = (u8*)self;
    if (advanceAnimTransform__FPQ34nw4r3lyt13AnimTransformf((void*)*(u32*)(p + 0x4c), lbl_eu_80667F78)) {
        *(u32*)(p + 0x58) = 3;
        ((nw4r::lyt::Layout*)(void*)*(u32*)(p + 0x44))->SetAnimationEnable((nw4r::lyt::AnimTransform*)(void*)*(u32*)(p + 0x48), 0);
        ((nw4r::lyt::Layout*)(void*)*(u32*)(p + 0x44))->SetAnimationEnable((nw4r::lyt::AnimTransform*)(void*)*(u32*)(p + 0x4c), 0);
        ((nw4r::lyt::Layout*)(void*)*(u32*)(p + 0x44))->SetAnimationEnable((nw4r::lyt::AnimTransform*)(void*)*(u32*)(p + 0x50), 1);
        p[0x61] = 1;
        func_801D0328(self);
        func_801D216C((void*)(p + 0x70), 1);
        func_801D216C((void*)(p + 0x88), 1);
        UpdateCursors(self);
        if (p[0x542]) {
            *(u32*)(p + 0x58) = 0x12;
            {
                u32 msg = (u32)BdatTouchStringCell((void*)&lbl_eu_8050566C[0x310], &lbl_eu_8050566C[0x158], 0x27);
                func_8022B90C((void*)(p + 0x4e8), 0);
                func_8022B9B4((void*)(p + 0x4e8), msg, 0);
                func_8022BFC8((void*)(p + 0x4e8), 1);
                func_8022B8B8((void*)(p + 0x4e8));
                func_801D216C((void*)(p + 0x70), 0);
            }
        }
    }
}


namespace nw4r { namespace lyt { class AnimTransform; } }
void advanceAnimTransform(nw4r::lyt::AnimTransform*, float);

__declspec(noinline) void CheckState4_Animate(char* self) { // noinline: dispatch table calls keep `bl`
    if (*(unsigned char*)((char*)self + 0x527) != 4) {
        return;
    }
    advanceAnimTransform(*(nw4r::lyt::AnimTransform**)((char*)self + 0x50), lbl_eu_80667F78);
}


// Animate grid panels.
__declspec(noinline) void AdvanceCloseAnim(void* self) { // noinline: dispatch table calls keep `bl`
    u8* p = (u8*)self;
    if (!AnimRewindFrame((nw4r::lyt::AnimTransform*)*(u32*)(p + 0x4c), lbl_eu_80667F78)) return;
    ((nw4r::lyt::Layout*)(void*)*(u32*)(p + 0x44))->SetAnimationEnable((nw4r::lyt::AnimTransform*)(void*)*(u32*)(p + 0x4c), 0);
    ((nw4r::lyt::Layout*)(void*)*(u32*)(p + 0x44))->SetAnimationEnable((nw4r::lyt::AnimTransform*)(void*)*(u32*)(p + 0x50), 0);
    ((nw4r::lyt::Layout*)(void*)*(u32*)(p + 0x44))->SetAnimationEnable((nw4r::lyt::AnimTransform*)(void*)*(u32*)(p + 0x48), 1);
    *(u32*)(p + 0x58) = 5;
}

__declspec(noinline) void FinishCloseAnim(void* self) { // noinline: dispatch table calls keep `bl`
    float f = lbl_eu_80667F78;
    u8* p = (u8*)self;
    u32 obj = *(u32*)(p + 0x48);
    if (AnimRewindFrame((nw4r::lyt::AnimTransform*)obj, f)) {
        p[0x61] = 1;
        *(u32*)(p + 0x58) = 0;
        func_801D216C(p + 0x70, 0);
    }
}
#pragma optimize_for_size on
extern "C" __declspec(noinline) void FinishNumSelect(void* self) { // noinline: dispatch table calls keep `bl`
    u8* p = (u8*)self;
    if (func_801EB028((void*)(p + 0x3e4))) {
        *(u32*)(p + 0x58) = 3;
        p[0x528] = 0;
        func_801D216C((void*)(p + 0x70), 1);
        UpdateCursors(self);
        if (p[0x541]) {
            p[0x541] = 0;
            *(u32*)(p + 0x58) = 0x12;
            {
                u32 msg = (u32)BdatTouchStringCell((void*)&lbl_eu_8050566C[0x310], &lbl_eu_8050566C[0x158], 0x10);
                func_8022B90C((void*)(p + 0x4e8), 0);
                func_8022B9B4((void*)(p + 0x4e8), msg, 0);
                func_8022BFC8((void*)(p + 0x4e8), 1);
                func_8022B8B8((void*)(p + 0x4e8));
                func_801D216C((void*)(p + 0x70), 0);
            }
        }
    }
}
#pragma optimize_for_size off
// Handle sub-object activation.
__declspec(noinline) void ShowSubMenuCur(void* self) { // noinline: dispatch table calls keep `bl`
    u8* p = (u8*)self;
    if (!func_80208360(p + 0x418)) return;
    *(u32*)(p + 0x58) = 7;
    u8 temp[16];
    func_80208760(temp, p + 0x418);
    ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate((const nw4r::math::VEC3*)(temp));
    func_801D216C(p + 0xa0, 1);
}


__declspec(noinline) void func_801CE524(void* self) { // noinline: dispatch table calls keep `bl`
    u8* p = (u8*)self;
    if (!func_80208360(p + 0x418)) return;
    *(u32*)(p + 0x58) = 3;
    if ((func_802087B8(p + 0x418) & 0xFF) == 2) {
        // Confirmed: dispatch on the selected cell's category byte.
        s32 val = (s8)p[0x6f];
        if (p[val + 0x62] == 0xa) {
        // Category 0xa cell: feed the picked item kind into the exchange view.
        u8* sub = p + 0x54c;
        u8 entry = p[0x525] * 10 + p[0x524];
        u32 kind = GetCellKind((CItemBoxGridFull*)sub, (u16)entry);
        func_8022E204(p + 0x468, kind & 0xFFFF);
        func_8022DB7C(p + 0x468);
        *(u32*)(p + 0x58) = 0xf;
        p[0x528] = 1;
        return;
    }
    if (p[val + 0x62] == 0xd) {
        // Category 0xd cell: collect-per-character confirmation dialog.
        u8* sub = p + 0x54c;
        u8 entry = p[0x525] * 10 + p[0x524];
        u32 kind = GetCellKind((CItemBoxGridFull*)sub, (u16)entry);
        BdatGetItemType(kind & 0xFFFF);
        u32 kindShort = BdatGetItemId(kind & 0xFFFF);

        u32 tbl = lbl_eu_80664110;
        u32 catVal = BdatGetU8Direct(tbl, &lbl_eu_8050566C[0x109], kindShort & 0xFFFF);
        u32 nameIdx = BdatGetU8Direct(tbl, &lbl_eu_8050566C[0x115], kindShort & 0xFFFF);
        u32 paramVal = BdatGetU8Direct(tbl, &lbl_eu_8050566C[0x11d], kindShort & 0xFFFF);

        tbl = BdatGetU8ByTableKey((void*)&lbl_eu_8050566C[0x126], &lbl_eu_8050566C[0x12e], paramVal & 0xFF);
        char* msg = (char*)BdatTouchStringCell(&lbl_eu_8050566C[0x126], &lbl_eu_8050566C[0x158], paramVal & 0xFF);
        char* subMsg = 0;   // optional second dialog line
        u32 showDetail = 0;

        if (isResourceFlagSet__Q22cf13CfGameManagerFv(nameIdx & 0xFF)) {
            u8* save = (u8*)func_8009EC9C(nameIdx & 0xFF);
            u32 cnt = CtrlObjectParam_GetCurrentRowKey() & 0xFF;
            // Per-(character, slot) record in the collection-save area.
            u8* rec = save + cnt * 0x49 + (tbl & 0xFF) * 2;
            u32 seenNew = 0;

            switch (catVal & 0xFF) {
            case 1:
                if (rec[0xe8]) {
                    msg = (char*)BdatTouchStringCell(&lbl_eu_8050566C[0x326], &lbl_eu_8050566C[0x158], 0x40);
                } else {
                    subMsg = (char*)BdatTouchStringCell(&lbl_eu_8050566C[0x326], &lbl_eu_8050566C[0x158], 0x39);
                    showDetail = 1;
                    seenNew = 1;
                    func_800A18A4(save, paramVal & 0xFF);
                }
                break;
            case 2:
                if (rec[0xe8]) {
                    if (!(rec[0xe9] & 1)) {
                        rec[0xe9] |= 0x80;
                        subMsg = (char*)BdatTouchStringCell(&lbl_eu_8050566C[0x326], &lbl_eu_8050566C[0x158], 0x3a);
                        showDetail = 1;
                        seenNew = 1;
                        incrementEventCounter__FUl(0x46);
                    } else {
                        msg = (char*)BdatTouchStringCell(&lbl_eu_8050566C[0x326], &lbl_eu_8050566C[0x158], 0x40);
                    }
                } else {
                    msg = (char*)BdatTouchStringCell(&lbl_eu_8050566C[0x326], &lbl_eu_8050566C[0x158], 0x41);
                }
                break;
            case 3:
                if (rec[0xe8]) {
                    if (!(rec[0xe9] & 2)) {
                        rec[0xe9] |= 0xc0;
                        subMsg = (char*)BdatTouchStringCell(&lbl_eu_8050566C[0x326], &lbl_eu_8050566C[0x158], 0x3b);
                        showDetail = 1;
                        seenNew = 1;
                        incrementEventCounter__FUl(0x46);
                        incrementEventCounter__FUl(0x47);
                    } else {
                        msg = (char*)BdatTouchStringCell(&lbl_eu_8050566C[0x326], &lbl_eu_8050566C[0x158], 0x40);
                    }
                } else {
                    msg = (char*)BdatTouchStringCell(&lbl_eu_8050566C[0x326], &lbl_eu_8050566C[0x158], 0x41);
                }
                break;
            }

            if (seenNew) {
                void* itemObj = GetCellItemPtr((CItemBoxGridFull*)sub, (u16)entry);
                s32 state = CheckCellState((CItemBoxGridFull*)sub, entry);
                DispatchItemEvt(p, itemObj, (s8)state - 1);
                RebuildGridCat(p);
            }
        } else {
            msg = (char*)BdatTouchStringCell(&lbl_eu_8050566C[0x326], &lbl_eu_8050566C[0x158], 0x42);
        }

        func_8022B90C(p + 0x4e8, showDetail);
        func_8022B9B4(p + 0x4e8, (u32)msg, (u32)subMsg);
        func_8022BFC8(p + 0x4e8, 1);
        func_8022B8B8(p + 0x4e8);
        *(u32*)(p + 0x58) = 0x12;
        p[0x528] = 0;
        return;
        }
        // Other categories: nothing to confirm.
        func_801D216C(p + 0x70, 1);
        p[0x528] = 0;
        return;
    }
    // Confirmation pending/not given: pop the notice dialog once.
    if (p[0x541]) {
        p[0x541] = 0;
        *(u32*)(p + 0x58) = 0x12;
        void* msg = BdatTouchStringCell(&lbl_eu_8050566C[0x310], &lbl_eu_8050566C[0x158], 0x10);
        func_8022B90C(p + 0x4e8, 0);
        func_8022B9B4(p + 0x4e8, (u32)msg, 0);
        func_8022BFC8(p + 0x4e8, 1);
        func_8022B8B8(p + 0x4e8);
        func_801D216C(p + 0x70, 0);
    } else {
        func_801D216C(p + 0x70, 1);
    }
    p[0x528] = 0;
}

// Handle sub-object activation with copy.
__declspec(noinline) void ShowExchangeCur(void* self) { // noinline: dispatch table calls keep `bl`
    u8* p = (u8*)self;
    if (!((CExchangeWin*)(p + 0x440))->getField27()) return;
    *(u32*)(p + 0x58) = 0xd;
    func_801D216C(p + 0xa0, 1);
    u8 temp[16];
    u8 val529 = p[0x529];
    func_8022D0F8(temp, p + 0x440, val529);
    reinterpret_cast<CBaseCur*>(p + 0xa0)->setRootPaneTranslate((const nw4r::math::VEC3*)(temp));
}

// Check if sub-obj is active; set state to 3 and clear flag.
__declspec(noinline) void FinishExchange(void* self) { // noinline: dispatch table calls keep `bl`
    u8* p = (u8*)self;
    if (((CExchangeWin*)(p + 0x440))->getField27()) {
        *(u32*)(p + 0x58) = 3;
        p[0x528] = 0;
    }
}

// Handle sub-object activation (variant).
__declspec(noinline) void ShowPresentCur(void* self) { // noinline: dispatch table calls keep `bl`
    u8* p = (u8*)self;
    if (!func_8022DB74(p + 0x468)) return;
    *(u32*)(p + 0x58) = 0x10;
    u8 temp[16];
    func_8022E498(temp, p + 0x468);
    ((CBaseCur*)(p + 0xd0))->setRootPaneTranslate((const nw4r::math::VEC3*)(temp));
    func_801D216C(p + 0xd0, 1);
}

// Check if sub-obj is active; set state to 3 and clear flag.
__declspec(noinline) void FinishPresent(void* self) { // noinline: dispatch table calls keep `bl`
    u8* p = (u8*)self;
    if (func_8022DB74(p + 0x468)) {
        *(u32*)(p + 0x58) = 3;
        p[0x528] = 0;
    }
}

// Check system window; if active, set state and call helpers.
__declspec(noinline) void OnSysWinOpen(void* self) { // noinline: dispatch table calls keep `bl`
    u8* p = (u8*)self;
    if (CSysWin_isActive(p + 0x4ac)) {
        *(u32*)(p + 0x58) = 0x16;
        func_801D216C(p + 0xa0, 1);
        UpdateCursors(self);
    }
}

// Handle system window state change.
void HandleSysWinOK(void* self) {
    u8* p = (u8*)self;
    if (!CSysWin_isActive(p + 0x4ac)) return;
    if (!((s8*)p)[0x540]) {
        u8 idx = p[0x6f];
        u8 cat = *(u8*)((u8*)self + (s8)idx + 0x62);
        u32 diff = cat - 2;
        if (diff <= 7) {
            func_801EB410(p + 0x3e4, 0);
        } else {
            func_801EB410(p + 0x3e4, 1);
        }
        func_801EB0D4(p + 0x3e4);
        *(u32*)(p + 0x58) = 0x18;
        p[0x528] = 1;
    } else {
        *(u32*)(p + 0x58) = 3;
        func_801D216C(p + 0x70, 1);
        UpdateCursors(self);
    }
}

// Handle system window activation.
__declspec(noinline) void FinishSysWin(void* self) { // noinline: dispatch table calls keep `bl`
    u8* p = (u8*)self;
    if (!CSysWin_isActive(p + 0x4e8)) return;
    *(u32*)(p + 0x58) = 3;
    if (!p[0x542] && !p[0x544]) {
        func_801D216C(p + 0x70, 1);
    }
    if (!p[0x544]) {
        UpdateCursors(self);
    }
    if (p[0x544]) {
        func_801D216C(p + 0xa0, 1);
    }
}

// Check conditions; set flags.
void SetGridReady(void* self) {
    // Guard chain: the message object at +0x44 and the list at +0x40 must be
    // live, and the three global table pointers must all be non-null, before
    // the busy/dirty flags are set.  The exit label keeps retail's
    // `bne store; blr` gate layout (see MWCC_CASES goto-gate pattern).
    if (*(u32*)((u8*)self + 0x44) == 0) goto exit;
    if (*(u32*)((u8*)self + 0x40) == 0) goto exit;
    if (lbl_eu_80664508 == 0) goto exit;
    if (lbl_eu_8066450C == 0) goto exit;
    if (lbl_eu_80664510 != 0) goto store;
    goto exit;
exit:
    return;
store:
    ((u8*)self)[0x60] = 1;
    ((u8*)self)[0x54] = 1;
    return;
}

// Resolve the cell's detail-message id and show it: per-item-type message
// lookups (equipment by the +0x54 slot-status name row 0x144..0x149, arts by
// the byte7>>2 subtype, skill items 0x155), each falling back to the +0x3C
// accessor's default label, then a kind name-table fallback, the list-tab
// override when bt is set, and finally the shared sprintf + PaneSetTexPaletteByName
// show.
extern "C" void ShowCellIcon(CItemBoxGridFull* self, u32 kind, void* item, u16 idx, u32 bt) {
    u8* p = (u8*)self;
    void* obj = item ? item : 0;
    u32 msgId = 0;
    if (obj) {
        u32 type = (*(u32*)obj >> 16) & 0xF;
        if (type == 3) {
            msgId = 0;
            CItemImpl* inst = CItem_initItemImplInstances((CItemData*)obj);
            u16 st = inst->vf54((CItemData*)obj);
            u8 c = (u8)BdatGetU8Direct(lbl_eu_806640D8, (const char*)&lbl_eu_8050566C[0x331], (u16)st);
            switch (c) {
            case 4: msgId = (u32)MakeTplNameSysFile(0x144); break;
            case 5: msgId = (u32)MakeTplNameSysFile(0x145); break;
            case 6: msgId = (u32)MakeTplNameSysFile(0x146); break;
            case 7: msgId = (u32)MakeTplNameSysFile(0x147); break;
            case 8: msgId = (u32)MakeTplNameSysFile(0x148); break;
            case 9: msgId = (u32)MakeTplNameSysFile(0x149); break;
            }
            if (msgId) {
                msgId = (u32)((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x40))->GetResource(
                    0x74696d67, (char*)msgId, 0);
            } else {
                msgId = (u32)((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x3c))->GetResource(
                    0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
            }
        } else if (type == 9) {
            CItemImpl* inst = CItem_initItemImplInstances((CItemData*)obj);
            u16 hasArts = (u16)inst->vf80((CItemData*)obj);
            if (hasArts == 0) {
                if (IsSkillItem(obj)) {
                    msgId = (u32)MakeTplNameSysFile(0x155);
                    msgId = (u32)((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x40))->GetResource(
                        0x74696d67, (char*)msgId, 0);
                } else {
                    u32 sub = 0;
                    int v = (((u8*)obj)[7] >> 2) & 0x3F;
                    switch (v) {
                    case 4: sub = (u32)MakeTplNameSysFile(0x144); break;
                    case 5: sub = (u32)MakeTplNameSysFile(0x145); break;
                    case 6: sub = (u32)MakeTplNameSysFile(0x146); break;
                    case 7: sub = (u32)MakeTplNameSysFile(0x147); break;
                    case 8: sub = (u32)MakeTplNameSysFile(0x148); break;
                    case 9: sub = (u32)MakeTplNameSysFile(0x149); break;
                    }
                    if (sub) {
                        msgId = (u32)((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x40))->GetResource(
                            0x74696d67, (char*)sub, 0);
                    } else {
                        msgId = (u32)((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x3c))->GetResource(
                            0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
                    }
                }
            } else {
                // origin arts - same byte7>>2 subtype mapping
                u32 sub2 = 0;
                int v = (((u8*)obj)[7] >> 2) & 0x3F;
                switch (v) {
                case 4: sub2 = (u32)MakeTplNameSysFile(0x144); break;
                case 5: sub2 = (u32)MakeTplNameSysFile(0x145); break;
                case 6: sub2 = (u32)MakeTplNameSysFile(0x146); break;
                case 7: sub2 = (u32)MakeTplNameSysFile(0x147); break;
                case 8: sub2 = (u32)MakeTplNameSysFile(0x148); break;
                case 9: sub2 = (u32)MakeTplNameSysFile(0x149); break;
                }
                if (sub2) {
                    msgId = (u32)((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x40))->GetResource(
                        0x74696d67, (char*)sub2, 0);
                } else {
                    msgId = (u32)((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x3c))->GetResource(
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
            u16 v = BdatGetU16Direct((const void*)lbl_eu_806640EC, &lbl_eu_8050566C[0x34d], kind);
            msgId = (u32)MakeTplNameSysFile(v);
            msgId = (u32)((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x40))->GetResource(
                0x74696d67, (char*)msgId, 0);
            if (!msgId) {
                msgId = (u32)((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x3c))->GetResource(
                    0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
            }
        } else {
            msgId = (u32)((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x3c))->GetResource(
                0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
        }
    }
    if (bt) {
        u8* q = p + (s8)p[0x6f];
        u8 cat = q[0x62];
        if (cat == 3) {
            msgId = (u32)((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x40))->GetResource(
                0x74696d67, (char*)&lbl_eu_8050566C[0x357], 0);
        } else {
            msgId = (u32)((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x40))->GetResource(
                0x74696d67, (char*)&lbl_eu_8050566C[0x36d], 0);
        }
    }
    if (msgId) {
        char buf[32];
        sprintf(buf, (const char*)&lbl_eu_8050566C[0x383], idx + 1);
        PaneSetTexPaletteByName(*(void**)(p + 0x44), buf, (void*)msgId);
    }
}

// Resolve the cell's detail-message id and show it: per-item-type message
// lookups (equipment 0x197..0x192 by the +0x08 slot value, arts 0x191..0x18d
// for skill items, 0x19c..0x198 for origin arts), each falling back to the
// +0x3C accessor's default label when the lookup yields nothing, then a kind
// name-table fallback, and finally the shared sprintf + PaneSetTexPaletteByName show.
extern "C" void ShowCellBadge(CItemBoxGridFull* self, u32 kind, void* item, u16 idx) {
    u8* p = (u8*)self;
    void* obj = item ? item : 0;
    u32 msgId = 0;
    if (obj) {
        u32 type = (*(u32*)obj >> 16) & 0xF;
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
                msgId = (u32)((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x40))->GetResource(
                    0x74696d67, (char*)msgId, 0);
            } else {
                msgId = (u32)((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x3c))->GetResource(
                    0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
            }
        } else if (type == 9) {
            void* inst = CItem_initItemImplInstances((CItemData*)obj);
            u16 hasArts = (u16)((CItemImpl*)inst)->vf80((CItemData*)obj);
            if (hasArts == 0) {
                if (IsSkillItem(obj)) {
                    // skill item - use the 0x191..0x18d mapping below
                } else {
                    u32 w = *(u32*)obj;
                    int isArts = 0;
                    u32 t = (w >> 16) & 0xF;
                    if (t == 9 && (u8)(((u8*)obj)[7] & 3) == 1) isArts = 1;
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
                    msgId = (u32)((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x40))->GetResource(
                        0x74696d67, (char*)msgId, 0);
                } else {
                    msgId = (u32)((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x3c))->GetResource(
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
                    msgId = (u32)((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x40))->GetResource(
                        0x74696d67, (char*)msgId, 0);
                } else {
                    msgId = (u32)((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x3c))->GetResource(
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
            u16 v = BdatGetU16Direct((const void*)lbl_eu_806640EC, &lbl_eu_8050566C[0x391], kind);
            msgId = (u32)MakeTplNameSysFile(v);
            msgId = (u32)((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x40))->GetResource(
                0x74696d67, (char*)msgId, 0);
            if (!msgId) {
                msgId = (u32)((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x3c))->GetResource(
                    0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
            }
        } else {
            msgId = (u32)((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x3c))->GetResource(
                0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
        }
    }
    if (msgId) {
        char buf[32];
        sprintf(buf, (const char*)&lbl_eu_8050566C[0x396], idx + 1);
        PaneSetTexPaletteByName(*(void**)(p + 0x44), buf, (void*)msgId);
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
//   func_eu_802B148C/func_eu_802B1474 are used AS the format string, and the
//   matching pane-colour tag pair goes through PaneMatSetTevColorsByName.
// optimize_for_size: retail keeps a stmw r29 block save.
#pragma push
#pragma optimize_for_size on
extern "C" void SetCellCaption(void* self, s8 kind, void* item, u16 idx) {
    void* it = (item != 0) ? item : 0;
    u8* p = (u8*)self;
    char nameBuf[0x20];
    char msgBuf[0x20];
    sprintf(nameBuf, (const char*)&lbl_eu_8050566C[0x3a3], idx + 1);

    // Kind nibble of the item's descriptor word; 0 when there is no item.
    u32 sel;
    if (it != 0) {
        sel = (*(u32*)it >> 16) & 0xF;
    } else {
        sel = 0;
    }

    if ((u8)sel == 3 || (u8)sel == 9) {
        CItemImpl* inst = CItem_initItemImplInstances((CItemData*)it);
        u32 cnt = (u32)inst->vf08((CItemData*)it);
        char* msg = BdatTouchStringCell((void*)&lbl_eu_8050566C[0x14f], (void*)&lbl_eu_8050566C[0x158],
                                  (u32)(0x1e - ((cnt & 0xFF) - 1)));
        sprintf(msgBuf, (const char*)&lbl_eu_8050566C[0x14c], msg);
        PaneMatSetTevColorsByName(*(void**)(p + 0x44), nameBuf, (void*)&lbl_eu_806644D8, (void*)&lbl_eu_806644E0);
    } else if (kind > 0) {
        sprintf(msgBuf, (const char*)&lbl_eu_8050566C[0x162], kind);
        PaneMatSetTevColorsByName(*(void**)(p + 0x44), nameBuf, (void*)&lbl_eu_806644D8, (void*)&lbl_eu_806644E0);
    } else if (kind == 0) {
        sprintf(msgBuf, (const char*)&lbl_eu_8050566C[0x3af]);
    } else if (kind == -2) {
        sprintf(msgBuf, func_eu_802B148C());
        PaneMatSetTevColorsByName(*(void**)(p + 0x44), nameBuf, (void*)&lbl_eu_806644E8, (void*)&lbl_eu_806644F0);
    } else if (kind == -3) {
        sprintf(msgBuf, func_eu_802B1474());
        PaneMatSetTevColorsByName(*(void**)(p + 0x44), nameBuf, (void*)&lbl_eu_806644F8, (void*)&lbl_eu_80664500);
    } else {
        sprintf(msgBuf, (const char*)&lbl_eu_8050566C[0x3af]);
    }
    func_80136A1C((nw4r::lyt::Layout*)*(void**)(p + 0x44), nameBuf, msgBuf, 0);
}
#pragma pop

// Show the cell-count label on the item box: format "x N" into a stack
// buffer, resolve the image pane through the +0x3C accessor's default label
// (overridden by the category-specific label when kind is 1 or 2, and by the
// flag-driven overlay labels), then hand the buffer to the layout via
// PaneSetTexPaletteByName.
extern "C" void ShowCellCount(void* self, u32 r4, void* r5, void* r6, u32 r7) {
    u8* p = (u8*)self;
    char buf[0x20];
    sprintf(buf, (const char*)&lbl_eu_8050566C[0x3b0], r7 + 1);

    void* acc;
    void* pane;
    acc = *(void**)(p + 0x3c);
    pane = ((nw4r::lyt::ArcResourceAccessor*)acc)->GetResource(0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
    if (r4 == 1) {
        acc = *(void**)(p + 0x3c);
        pane = ((nw4r::lyt::ArcResourceAccessor*)acc)->GetResource(0x74696d67, (char*)&lbl_eu_8050566C[0x3bb], 0);
    } else if (r4 == 2) {
        acc = *(void**)(p + 0x3c);
        pane = ((nw4r::lyt::ArcResourceAccessor*)acc)->GetResource(0x74696d67, (char*)&lbl_eu_8050566C[0x3cf], 0);
    }
    if (r6) {
        acc = *(void**)(p + 0x3c);
        pane = ((nw4r::lyt::ArcResourceAccessor*)acc)->GetResource(0x74696d67, (char*)&lbl_eu_8050566C[0x3e3], 0);
    }
    if (r5) {
        acc = *(void**)(p + 0x3c);
        pane = ((nw4r::lyt::ArcResourceAccessor*)acc)->GetResource(0x74696d67, (char*)&lbl_eu_8050566C[0x3f7], 0);
    }

    if (pane) {
        // pane rides in r5 straight into the call (retail never reloads r5)
        PaneSetTexPaletteByName(*(void**)(p + 0x44), (void*)buf, (void*)pane);
    }
}

// Format the current tab's header text: build the category label via the
// switch-driven sprintf (jump table), resolve the pane through the +0x40
// accessor (falling back to +0x3C's default label), then write the
// "<label> <idx+1>" string into the layout. optimize_for_size gives the
// retail stmw r28 prologue (4 saved regs).
// Case layout per retail jump table jumptable_eu_80534694: slots 0/1 share
// the exit (cases 0 and 1 skip the label lookup buffer), slots 2..13 map to
// the twelve label formats 0x40b..0x4f2.
#pragma optimize_for_size on
extern "C" void SetTabHeader(void* self, int r4, int r5) {
    u8* p = (u8*)self;
    char buf[32];
    char tmp[32];
    int result;
    if (r4 != 0) {
        switch (r4) {
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
        result = (int)((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x40))->GetResource(0x74696d67, buf, 0);
        if (!result) {
            result = (int)((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x3c))->GetResource(0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
        }
    } else {
        result = (int)((nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x3c))->GetResource(0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
    }
    if (result) {
        sprintf(tmp, (const char*)&lbl_eu_8050566C[0x507], r5 + 1);
        PaneSetTexPaletteByName(*(void**)(p + 0x44), tmp, (void*)result);
    }
}
#pragma optimize_for_size off

// Format text and set on layout pane.
#pragma optimize_for_size on
extern "C" void SetCellMarkVis(void* self, u32 val, u32 idx) {
    u8* p = (u8*)self;
    char buf[32];
    sprintf(buf, (const char*)&lbl_eu_8050566C[0x513], idx + 1);
    u32 obj = *(u32*)(p + 0x44);
    u32 sub = *(u32*)(obj + 0x10);
    void* ret = ((nw4r::lyt::Pane*)sub)->FindPaneByName(buf, 1);
    func_80124270(ret, val);
}
#pragma optimize_for_size off

// Refresh the 12 category tabs: format each tab label, tint it per the
// current page (cat-4/2/0xB use the "active" palette, others the plain one),
// then update the two tab text panes. Afterwards rebuild the sub-grid if in
// category mode (0x527 == 2). No pragma: retail's stmw r26 prologue is the
// plain -O4 shape, so keep exactly six long-lived values (cat, isOtherTab,
// i, isCurTab, string base, self).
extern "C" void func_801CFD2C(void* self) {
    u8* p = (u8*)self;
    int isCurTab;
    u32 i;
    for (i = 0; i < 12; i++) {
        char buf1[32];
        char buf2[32];
        int tabNum = (u8)i + 1;
        sprintf(buf1, (const char*)&lbl_eu_8050566C[0x520], tabNum);
        sprintf(buf2, (const char*)&lbl_eu_8050566C[0x52f], tabNum);
        isCurTab = 0;
        int isOtherTab = 0;
        u8 cat = p[(u8)i + 0x62];        if (cat != 0) {
            if ((u8)i == (s8)p[0x6f]) {
                isCurTab = 1;
                isOtherTab = 0;
            } else {
                isCurTab = 0;
                isOtherTab = 1;
            }
            if ((u32)(cat - 4) <= 4 || (s32)cat == 2 || (s32)cat == 0xb) {
                PaneMatSetTevColorsByName(*(void**)(p + 0x44), buf1, &lbl_eu_806644A8, &lbl_eu_806644B0);
                PaneMatSetTevColorsByName(*(void**)(p + 0x44), buf2, &lbl_eu_806644A8, &lbl_eu_806644B0);
            } else {
                PaneMatSetTevColorsByName(*(void**)(p + 0x44), buf1, &lbl_eu_80664498, &lbl_eu_806644A0);
                PaneMatSetTevColorsByName(*(void**)(p + 0x44), buf2, &lbl_eu_80664498, &lbl_eu_806644A0);
            }
        }
        void* pane = *(void**)(*(u32*)(p + 0x44) + 0x10);
        void* t1 = ((nw4r::lyt::Pane*)pane)->FindPaneByName(buf1, 1);
        func_80124270(t1, isCurTab);
        pane = *(void**)(*(u32*)(p + 0x44) + 0x10);
        void* t2 = ((nw4r::lyt::Pane*)pane)->FindPaneByName(buf2, 1);
        func_80124270(t2, isOtherTab);
        SetTabHeader(self, cat, (u8)i);
    }
    func_801D05D4(self, 0);
    if (p[0x527] == 2) {
        CItemBoxGridFull* subGrid = (CItemBoxGridFull*)(p + 0x54c);
        p[0x542] = 0;
        func_801C56D8(subGrid, p[0x62], 0, *(u16*)(p + 0x52e), *(u16*)(p + 0x52a));
        u16 count1 = subGrid->field_2800;
        func_801C56D8(subGrid, p[0x63], 0, *(u16*)(p + 0x52e), *(u16*)(p + 0x52a));
        if (!(count1 | subGrid->field_2800)) {
            p[0x542] = 1;
        }
    }
    RebuildGridCat(self);
}

#pragma push
#pragma optimize_for_size on
void RebuildGridCat(void* self) {
    u8* p = (u8*)self;
    u8* sub = p + 0x54c;
    func_801C56D8((CItemBoxGridFull*)sub, (p + 0x62)[(s8)p[0x6f]], 0,
                  *(u16*)(p + 0x52e), *(u16*)(p + 0x52a));
    if (p[0x527] == 4) {
        func_801C7730((CItemBoxGridFull*)sub);
    }
    setLayoutTextBoxNumber(reinterpret_cast<nw4r::lyt::Layout*>(*(u32*)(p + 0x44)),
                  const_cast<char*>(&lbl_eu_8050566C[0x53e]),
                  (u8)CalcGridRowCount((void*)sub));
    func_801CFFEC(self);
    func_801D0328(self);
    func_801D0BD8(self);
    if (getItemBoxState__FP12CItemBoxInfo(p + 0x1d8) != 0) {
        func_801D4260(p + 0x1d8, (p + 0x62)[(s8)p[0x6f]]);
    }
}
#pragma pop

// Rebuilds the item-box grid UI: shows/hides the row panes for the current
// row count/page, then pushes all 30 entries' state back into the widgets.
// Keep `rows` full-width: retail converts (10 - rows) through the u8 truncate
// and holds the raw return in a callee-saved register across both loops.
void func_801CFFEC(void* self) {
    // Declaration order nudges MWCC's callee-saved allocation toward retail
    // (rows gets the highest register, sub/self the middle ones).
    u32 rows;
    CItemBoxGridFull* sub;
    u8* p = (u8*)self;
    sub = (CItemBoxGridFull*)(p + 0x54c);
    rows = CalcGridRowCount(sub);
    if ((u8)rows > 1) {
        void* pane = *(void**)(*(u32**)(p + 0x44) + 4);
        void* ret = ((nw4r::lyt::Pane*)pane)->FindPaneByName((char*)&lbl_eu_8050566C[0x547], 1);
        func_80124270(ret, 1);
        u32 visRows = rows & 0xFF;
        u8 i;
        for (i = 0; i < 10; i++) {
            char buf[32];
            sprintf(buf, (char*)&lbl_eu_8050566C[0x54e], i + 1);
            void* pane2 = *(void**)(*(u32**)(p + 0x44) + 4);
            void* ret2 = ((nw4r::lyt::Pane*)pane2)->FindPaneByName(buf, 1);
            // visible while i < visRows (borrow trick)
            func_80124270(ret2, ((u32)i - visRows) >> 31);
        }
        u8 pageDisp = (u8)(sub->field_2804 + 1);
        setLayoutTextBoxNumber(*(nw4r::lyt::Layout**)(p + 0x44), (char*)&lbl_eu_8050566C[0x55d], pageDisp);
        void* pane3 = *(void**)(*(u32**)(p + 0x44) + 4);
        void* ret3 = ((nw4r::lyt::Pane*)pane3)->FindPaneByName((char*)&lbl_eu_8050566C[0x566], 1);
        float v[3];
        v[0] = *(float*)(p + 0x534);
        v[1] = *(float*)(p + 0x538);
        v[2] = *(float*)(p + 0x53c);
        float step = lbl_eu_80667F84;
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
        copyVEC3((u8*)ret3 + 0x2c, v);
    } else {
        void* pane = *(void**)(*(u32**)(p + 0x44) + 4);
        void* ret = ((nw4r::lyt::Pane*)pane)->FindPaneByName((char*)&lbl_eu_8050566C[0x547], 1);
        func_80124270(ret, 0);
    }
    u8 i;
    for (i = 0; i < 3; i++) {
        char buf[32];
        sprintf(buf, (char*)&lbl_eu_8050566C[0x574], i + 1);
        void* pane = *(void**)(*(u32**)(p + 0x44) + 4);
        void* ret = ((nw4r::lyt::Pane*)pane)->FindPaneByName(buf, 1);
        if (ret) {
            u8 max = (u8)LookupIndexedByte((char*)sub);
            func_80124270(ret, ((u32)i - (u32)max) >> 31);
        }
    }
    u8 idx;
    for (idx = 0; idx < 0x1e; idx++) {
        u32 kind = GetCellKind((CItemBoxGridFull*)sub, (u16)idx);
        void* obj = GetCellItemPtr((CItemBoxGridFull*)sub, (u16)idx);
        u32 bt = GetCellByte7(sub, idx);
        ShowCellIcon((CItemBoxGridFull*)self, kind & 0xFFFF, obj, idx, bt);
        ShowCellBadge((CItemBoxGridFull*)self, kind & 0xFFFF, obj, idx);
        s32 chk = CheckCellState(sub, idx);
        SetCellCaption(self, (s8)chk, obj, idx);
        u32 b5 = GetCellFlag3(sub, idx);
        u32 b4 = GetCellFlag2(sub, idx);
        u32 b2 = GetCellFlag0(sub, idx);
        ShowCellCount(self, b2 & 0xFF, (void*)b4, (void*)b5, idx);
        u32 b7 = GetCellByte8(sub, idx);
        SetCellMarkVis(self, b7, idx);
    }
}

// Refreshes the grid UI text for the selected cell. Retail computes the
// selected index unconditionally from the raw bytes, then special-cases the
// "no selection" rows (-1/-2).
__declspec(noinline) void func_801D0328(void* self) {
    u8* p = (u8*)self;
    CItemBoxGridFull* sub = (CItemBoxGridFull*)(p + 0x54c);
    s8 row = (s8)p[0x525];
    s8 idx = (s8)(p[0x524] + p[0x525] * 10);
    if (row == -1) {
        char* val = (char*)GetCatLabelMsg(self);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)*(u32**)(p + 0x44), (char*)&lbl_eu_8050566C[0x581], val, 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)*(u32**)(p + 0x44), (char*)&lbl_eu_8050566C[0x58a], (char*)&lbl_eu_8050566C[0x3af], 0);
    } else if (row == -2) {
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)*(u32**)(p + 0x44), (char*)&lbl_eu_8050566C[0x581], (char*)&lbl_eu_8050566C[0x3af], 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)*(u32**)(p + 0x44), (char*)&lbl_eu_8050566C[0x58a], (char*)&lbl_eu_8050566C[0x3af], 0);
    } else {
        char* name = (char*)FormatCellName(sub, (u16)(u8)idx);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)*(u32**)(p + 0x44), (char*)&lbl_eu_8050566C[0x581], name, 0);
        char* str = func_801C6A44(sub, (u16)(u8)idx);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)*(u32**)(p + 0x44), (char*)&lbl_eu_8050566C[0x58a], str, *(u32*)(p + 0x5c));
    }
    u32 qty;
    if (p[0x527] == 2) {
        void* obj = GetCellItemPtr((CItemBoxGridFull*)sub, (u16)(u8)idx);
        u32 kind = GetCellKind((CItemBoxGridFull*)sub, (u16)(u8)idx);
        CheckCellState(sub, (u16)(u8)idx);  // result unused in retail
        if ((s8)p[0x525] > -1)
            qty = func_801C618C(sub, kind & 0xFFFF, (void*)obj, *(u16*)(p + 0x52a));
        else
            qty = 0;
    } else {
        if ((s8)p[0x525] > -1)
            qty = func_801C5FC0(sub, (u16)(u8)idx);
        else
            qty = 0;
    }
    char buf[0x30];
    sprintf(buf, (const char*)&lbl_eu_8050566C[0x596], qty, (char*)BdatTouchStringCell(&lbl_eu_8050566C[0x14f], &lbl_eu_8050566C[0x158], 3));
    func_80136A1C((nw4r::lyt::Layout*)*(u32**)(p + 0x44), (char*)&lbl_eu_8050566C[0x59b], buf, 0);
    if (getItemBoxState__FP12CItemBoxInfo(p + 0x1d8)) {
        u32 kind2;
        if ((s8)p[0x525] > -1) {
            void* obj2 = GetCellItemPtr((CItemBoxGridFull*)sub, (u16)(u8)idx);
            kind2 = GetCellKind((CItemBoxGridFull*)sub, (u16)(u8)idx) & 0xFFFF;
            func_801D47D4((void*)(p + 0x1d8), kind2, (u32)obj2, 1);
        } else {
            func_801D47D4((void*)(p + 0x1d8), 0, 0, 1);
        }
        u32 val2;
        if ((s8)p[0x525] > -1)
            val2 = FormatCellName(sub, (u16)(u8)idx);
        else
            val2 = 0;
        setItemBoxNamedText(p + 0x1d8, 1, (void*)val2);
        // Right-to-left evaluation: retail calls C6840 first.
        func_801D4BDC(p + 0x1d8, CountMarkedCells(sub) & 0xFF, GetCategoryCap(sub) & 0xFF);
    }
}

// Tab-page mover: rewrites the "N/total" tab labels for the new page
// (wrapping around the page count at +0x6e) and refreshes the 12 overlay
// panes with the colour pair matching the current category.
extern "C" void func_801D05D4(void* self, int val) {
    u8* p = (u8*)self;
    u8 buf[2];
    u8 b = p[0x6f];
    s32 n;
    buf[0] = b;
    n = (s8)b - 1;
    if (val != 0) n = (s8)b + 1;
    buf[1] = (u8)n;
    if ((s8)(u8)n < 0) {
        buf[1] = p[0x6e] - 1;
    } else {
        if ((s8)(u8)n >= p[0x6e]) buf[1] = 0;
    }
    {
        const char* strbase = lbl_eu_8050566C;
        u8 i = 0;
        do {
            u8 idx = buf[i];
            char pane1[32];
            char pane2[32];
            int tabNum = (s8)idx + 1;
            sprintf(pane1, strbase + 0x520, tabNum);
            sprintf(pane2, strbase + 0x52f, tabNum);
            if (p[0x62 + (s8)idx] != 0) {
                // Flags are first assigned only after idx's last use, so
                // MWCC reuses the tabNum/idx registers for them.
                int isCurTab;
                int isOtherTab;
                if ((s8)idx == (s8)p[0x6f]) {
                    isCurTab = 1;
                    isOtherTab = 0;
                } else {
                    isCurTab = 0;
                    isOtherTab = 1;
                }
                func_80124270(((nw4r::lyt::Pane*)*(void**)(*(u32*)(p + 0x44) + 0x10))->FindPaneByName(pane1, 1), isCurTab);
                func_80124270(((nw4r::lyt::Pane*)*(void**)(*(u32*)(p + 0x44) + 0x10))->FindPaneByName(pane2, 1), isOtherTab);
            }
            i++;
        } while (i < 2);
    }
    {
        u8 cat = p[(s8)p[0x6f] + 0x62];
        u32 diff = cat - 4;
        if (diff <= 4 || cat == 2 || cat == 0xB) {
            char* strbase = (char*)lbl_eu_8050566C;
            // Layout pointer re-derived per call: PaneMatSetTevColorsByName clobbers
            // memory, so MWCC reloads *(p+0x44) before every call.
            PaneMatSetTevColorsByName((void*)*(u32*)(p + 0x44), strbase + 0x5aa, (void*)&lbl_eu_806644A8, (void*)&lbl_eu_806644B0);
            PaneMatSetTevColorsByName((void*)*(u32*)(p + 0x44), strbase + 0x5b6, (void*)&lbl_eu_806644A8, (void*)&lbl_eu_806644B0);
            PaneMatSetTevColorsByName((void*)*(u32*)(p + 0x44), strbase + 0x5c2, (void*)&lbl_eu_806644A8, (void*)&lbl_eu_806644B0);
            PaneMatSetTevColorsByName((void*)*(u32*)(p + 0x44), strbase + 0x5ce, (void*)&lbl_eu_806644A8, (void*)&lbl_eu_806644B0);
            PaneMatSetTevColorsByName((void*)*(u32*)(p + 0x44), strbase + 0x5da, (void*)&lbl_eu_806644A8, (void*)&lbl_eu_806644B0);
            PaneMatSetTevColorsByName((void*)*(u32*)(p + 0x44), strbase + 0x5e6, (void*)&lbl_eu_806644A8, (void*)&lbl_eu_806644B0);
            PaneMatSetTevColorsByName((void*)*(u32*)(p + 0x44), strbase + 0x5f2, (void*)&lbl_eu_806644A8, (void*)&lbl_eu_806644B0);
            PaneMatSetTevColorsByName((void*)*(u32*)(p + 0x44), strbase + 0x5fe, (void*)&lbl_eu_806644A8, (void*)&lbl_eu_806644B0);
            PaneMatSetTevColorsByName((void*)*(u32*)(p + 0x44), strbase + 0x60a, (void*)&lbl_eu_806644A8, (void*)&lbl_eu_806644B0);
            PaneMatSetTevColorsByName((void*)*(u32*)(p + 0x44), strbase + 0x616, (void*)&lbl_eu_806644A8, (void*)&lbl_eu_806644B0);
            PaneMatSetTevColorsByName((void*)*(u32*)(p + 0x44), strbase + 0x621, (void*)&lbl_eu_806644A8, (void*)&lbl_eu_806644B0);
            PaneMatSetTevColorsByName((void*)*(u32*)(p + 0x44), strbase + 0x62c, (void*)&lbl_eu_806644A8, (void*)&lbl_eu_806644B0);
        } else {
            char* strbase = (char*)lbl_eu_8050566C;
            PaneMatSetTevColorsByName((void*)*(u32*)(p + 0x44), strbase + 0x5aa, (void*)&lbl_eu_80664498, (void*)&lbl_eu_806644A0);
            PaneMatSetTevColorsByName((void*)*(u32*)(p + 0x44), strbase + 0x5b6, (void*)&lbl_eu_80664498, (void*)&lbl_eu_806644A0);
            PaneMatSetTevColorsByName((void*)*(u32*)(p + 0x44), strbase + 0x5c2, (void*)&lbl_eu_80664498, (void*)&lbl_eu_806644A0);
            PaneMatSetTevColorsByName((void*)*(u32*)(p + 0x44), strbase + 0x5ce, (void*)&lbl_eu_80664498, (void*)&lbl_eu_806644A0);
            PaneMatSetTevColorsByName((void*)*(u32*)(p + 0x44), strbase + 0x5da, (void*)&lbl_eu_80664498, (void*)&lbl_eu_806644A0);
            PaneMatSetTevColorsByName((void*)*(u32*)(p + 0x44), strbase + 0x5e6, (void*)&lbl_eu_80664498, (void*)&lbl_eu_806644A0);
            PaneMatSetTevColorsByName((void*)*(u32*)(p + 0x44), strbase + 0x5f2, (void*)&lbl_eu_80664498, (void*)&lbl_eu_806644A0);
            PaneMatSetTevColorsByName((void*)*(u32*)(p + 0x44), strbase + 0x5fe, (void*)&lbl_eu_80664498, (void*)&lbl_eu_806644A0);
            PaneMatSetTevColorsByName((void*)*(u32*)(p + 0x44), strbase + 0x60a, (void*)&lbl_eu_80664498, (void*)&lbl_eu_806644A0);
            PaneMatSetTevColorsByName((void*)*(u32*)(p + 0x44), strbase + 0x616, (void*)&lbl_eu_80664498, (void*)&lbl_eu_806644A0);
            PaneMatSetTevColorsByName((void*)*(u32*)(p + 0x44), strbase + 0x621, (void*)&lbl_eu_80664498, (void*)&lbl_eu_806644A0);
            PaneMatSetTevColorsByName((void*)*(u32*)(p + 0x44), strbase + 0x62c, (void*)&lbl_eu_80664498, (void*)&lbl_eu_806644A0);
        }
    }
}

// Refresh the cursor-position readouts: when the +0x4ac window is up, feed
// its cursor position into the +0xa0 cursor; otherwise compute the selected
// cell's pane position (func_801375A0), scale its x by the label pane's
// +0x44 factor, and drive the +0x70/+0xb8 cursors with it.
extern "C" __declspec(noinline) void UpdateCursors(void* self) {
    u8* p = (u8*)self;
    nw4r::math::VEC3 posT2;
    nw4r::math::VEC3 posT1;
    nw4r::math::VEC3 posDef;
    nw4r::math::VEC3 posWin;
    if (CSysWin_getUnk34(p + 0x4ac)) {
        func_8022C1B4(&posWin, (void*)(p + 0x4ac), p[0x540]);
        ((CBaseCur*)(p + 0xa0))->setRootPaneTranslate((const nw4r::math::VEC3*)(&posWin));
        return;
    }
    s8 idx = (s8)p[0x525];
    if (idx == -2) {
        void* t1 = ((nw4r::lyt::Pane*)*(void**)(*(u32*)(p + 0x44) + 0x10))->FindPaneByName((char*)&lbl_eu_8050566C[0x637], 1);
        func_801375A0(&posT2, (void*)t1);
        void* t2 = ((nw4r::lyt::Pane*)*(void**)(*(u32*)(p + 0x44) + 0x10))->FindPaneByName((char*)&lbl_eu_8050566C[0x301], 1);
        posT2.x = posT2.x * *(float*)((u8*)t2 + 0x44);
        ((CBaseCur*)(p + 0xb8))->setRootPaneTranslate((const nw4r::math::VEC3*)(&posT2));
        func_801D216C(p + 0x70, 0);
        func_801D216C(p + 0xb8, 1);
        return;
    }
    if (idx == -1) {
        char str[32];
        sprintf(str, (const char*)&lbl_eu_8050566C[0x507], (s8)p[0x6f] + 1);
        void* t1 = ((nw4r::lyt::Pane*)*(void**)(*(u32*)(p + 0x44) + 0x10))->FindPaneByName(str, 1);
        func_801375A0(&posT1, (void*)t1);
        void* t2 = ((nw4r::lyt::Pane*)*(void**)(*(u32*)(p + 0x44) + 0x10))->FindPaneByName((char*)&lbl_eu_8050566C[0x301], 1);
        posT1.x = posT1.x * *(float*)((u8*)t2 + 0x44);
        ((CBaseCur*)(p + 0x70))->setRootPaneTranslate((const nw4r::math::VEC3*)(&posT1));
        func_801D216C(p + 0x70, 1);
        func_801D216C(p + 0xb8, 0);
        return;
    }
    {
        char str[32];
        sprintf(str, (const char*)&lbl_eu_8050566C[0x396], (s8)p[0x524] + idx * 10 + 1);
        void* t1 = ((nw4r::lyt::Pane*)*(void**)(*(u32*)(p + 0x44) + 0x10))->FindPaneByName(str, 1);
        func_801375A0(&posDef, (void*)t1);
        void* t2 = ((nw4r::lyt::Pane*)*(void**)(*(u32*)(p + 0x44) + 0x10))->FindPaneByName((char*)&lbl_eu_8050566C[0x301], 1);
        posDef.x = posDef.x * *(float*)((u8*)t2 + 0x44);
        ((CBaseCur*)(p + 0x70))->setRootPaneTranslate((const nw4r::math::VEC3*)(&posDef));
        func_801D216C(p + 0x70, 1);
        func_801D216C(p + 0xb8, 0);
    }
}

extern "C" __declspec(noinline) void func_801D0BD8(void* self) { }

// Unequip helper: scans the 8 party member slots for a member holding item
// `id`. Category 3 searches every item of the 6 sub-categories (the scan list
// is re-copied from an 8-byte sdata2 seed each pass) and clears the owning
// slot via the item's vtable+0x44 method; equipment categories (2, 4-8)
// compare the member's slot field and clear it with the dedicated setter.
// A match (or any non-equipment category) then runs one camera/enum-list
// pass that finds the on-screen move object for that player and detaches it;
// category 3 without a match just continues to the next pass.
#pragma push
#pragma optimize_for_size on  // stmw r20..r31 block-save prologue
void func_801D0E88(void* self, int kind, int id) {
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
        void* obj = (void*)func_8009EC9C(pass);
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
                    void* item = func_80157C4C(cat, j);
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
                int v = *(s16*)((u8*)obj + 0x26);
                if (v == -1 || v != id) continue;
                CtrlObjectParam_SetEquipSlot5(obj, -1);
                break;
            }
            case 4: {
                int v = *(s16*)((u8*)obj + 0x1c);
                if (v == -1 || v != id) continue;
                CtrlObjectParam_SetEquipSlot0(obj, -1);
                break;
            }
            case 5: {
                int v = *(s16*)((u8*)obj + 0x1e);
                if (v == -1 || v != id) continue;
                CtrlObjectParam_SetEquipSlot1(obj, -1);
                break;
            }
            case 6: {
                int v = *(s16*)((u8*)obj + 0x20);
                if (v == -1 || v != id) continue;
                CtrlObjectParam_SetEquipSlot2(obj, -1);
                break;
            }
            case 7: {
                int v = *(s16*)((u8*)obj + 0x22);
                if (v == -1 || v != id) continue;
                CtrlObjectParam_SetEquipSlot3(obj, -1);
                break;
            }
            case 8: {
                int v = *(s16*)((u8*)obj + 0x24);
                if (v == -1 || v != id) continue;
                CtrlObjectParam_SetEquipSlot4(obj, -1);
                break;
            }
            }
        }
        // Camera pass: filter the scene enum list by this player and detach
        // its move object when the player index matches.
        CtrlObjectParam_SyncParamFromActor((void*)found);
        CItemBoxGridEnumHolder holder;
        const u32* src = (const u32*)lbl_eu_80505628;
        holder.names[0] = *src++;
        holder.names[1] = *src++;
        holder.names[2] = *src++;
        CTaskGame_enumListCtor(&holder);
        u8 i;
        for (i = 0; i < 3; i++) {
            func_800F4A98(CTaskGame_enumListGet(&holder), holder.names[i], 0);
            CItemBoxGridEnumList* list = (CItemBoxGridEnumList*)CTaskGame_enumListGet(&holder);
            if (list->field_620 >= 1) {
                list = (CItemBoxGridEnumList*)CTaskGame_enumListGet(&holder);
                CItemBoxGridEnumSlot* slot =
                    (CItemBoxGridEnumSlot*)func_800F6EC0(list, 0);
                if (slot->field_04) {
                    CItemBoxGridMoveObj* mv = (CItemBoxGridMoveObj*)
                        getCfObjectPc__FPQ22cf12CfObjectMove(slot->field_04);
                    if ((u8)pass == mv->field_3f28) {
                        func_800BFDE0(mv, 0);
                        break;
                    }
                }
            }
        }
        __dt__80043E88(&holder, -1);
        return;
    }
}

// Handle item event dispatch.
void DispatchItemEvt(void* self, void* item, int eventType) {
    if (!item) return;
    if (eventType < 1) {
        CItem_initItemImplInstances((CItemData*)item)->vf10((CItemData*)item);
    } else {
        u32 w = *(u32*)item;
        CItemData_initFromFamily(item, w >> 20);
    }
}

// Cast-only vtable interface for the object returned by
// CItem_initItemImplInstances: with -RTTI on, MWCC prepends 2 hidden RTTI
// header entries, so the Nth declared virtual sits at 4*(N+2). Real virtual
// dispatch reproduces the retail `lwz r12,0(r3); lwz r12,<off>(r12)`
// sequence; manual `(*(void***)x)[N]` casts color a scratch r5 instead.
// Dispatch based on entry category.
u32 GetCatLabelMsg(void* self) {
    u8* p = (u8*)self;
    u8* entry = p + (s8)p[0x6f];
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
// (lbl_eu_80664110) and test the character's learned-arts flags at
// charData + count*0x49 + (table byte << 1). Returns 1 when the flag test
// fails (item not yet learned); 0 when it is already learned.
u32 func_801D12D4(void* self, u32 kind) {
    u32 bdat = lbl_eu_80664110;
    BdatGetItemType(kind);
    u32 shortKind = BdatGetItemId(kind);
    u32 flag = BdatGetU8Direct(bdat, (const char*)&lbl_eu_8050566C[0x109], (u16)shortKind);
    u32 v;
    u32 charId = BdatGetU8Direct(bdat, (const char*)&lbl_eu_8050566C[0x115], (u16)shortKind);
    u32 unk = BdatGetU8Direct(bdat, (const char*)&lbl_eu_8050566C[0x11d], (u16)shortKind);
    v = BdatGetU8ByTableKey((void*)&lbl_eu_8050566C[0x126], (const char*)&lbl_eu_8050566C[0x12e], unk & 0xFF);
    void* charData = (void*)func_8009EC9C(charId & 0xFF);
    u8* ptr = (u8*)charData + (CtrlObjectParam_GetCurrentRowKey() & 0xFF) * 0x49 + ((v & 0xFF) << 1);
    switch (flag & 0xFF) {
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


void CopyVec4s(short* dst, const short* src) { dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2]; dst[3] = src[3]; }

// Split a u32 into its four bytes, stored as shorts (big-endian order).
extern "C" void SplitU32ToS16s(short* dst, unsigned long val) {
    dst[3] = val & 0xFF;
    dst[2] = (val >> 8) & 0xFF;
    dst[1] = (val >> 16) & 0xFF;
    dst[0] = (val >> 24) & 0xFF;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// CArtsBookItem constructor (hard-symbol stub). noinline: retail callers
// emit `bl`; -inline would fold this vtable-store body into the caller.
__declspec(noinline) void __ct__CArtsBookItem(void* self) {
    *(u16*)((u8*)self + 0x804) = 0;
    *(void**)((u8*)self) = (void*)&lbl_eu_805347D8;
}
// Standard MWCC virtual destructor
__declspec(noinline) void* __dt__10CQuestItemFv(void* self, int mode) {
    if (self && mode > 0) __dl__FPv(self);
    return self;
}
__declspec(noinline) void* __dt__11CVisionItemFv(void* self, int mode) {
    if (self && mode > 0) __dl__FPv(self);
    return self;
}
__declspec(noinline) void* __dt__13CArtsBookItemFv(void* self, int mode) {
    if (self && mode > 0) __dl__FPv(self);
    return self;
}


// Scan item ids 0x220+ grouped by the category ranges returned by
// func_801380A0 and register teachable entries: an item qualifies when its
// type-specific name row (base-name key, or the +0xc8 teach key, or a free
// ability slot) equals 2 and the paired look-up key is present, in which
// case func_801C5158 pushes the id.
// Direct global references (no pointer locals): retail keeps two webs for
// lbl_eu_8050566C - an @ha base copy and a full-pointer copy - which only
// appears when the pool is referenced directly instead of through a local.
#define XB_K ((const char*)lbl_eu_8050566C)
#define XB_TABLES ((const void* const*)lbl_eu_80573D18)
void func_801C4BB4(void* self) {
    // C89-style declarations: MWCC births virtual registers in declaration
    // order when locals are hoisted, which is the shape retail claims.
    void* tbl;
    u32 cat;
    u32 id;
    u16 end;
    u8 tp;
    u16 v;
    *(u16*)((u8*)self + 0x804) = 0;
    for (cat = 1; cat < 0x1b; cat++) {
        u32 start = func_801380A0((u16)cat);
        if ((u16)start == 0) continue;
        end = func_801380A0((u16)(cat + 1));
        for (id = start; (u16)id < end; id++) {
            tp = func_8009CF8C((u16)id + 0x220);
            if (tp >= 0xFE || tp == 0 || tp == 0xC8) continue;
            tbl = (void*)XB_TABLES[func_80138138((u16)id)];
            if ((u8)BdatGetU8Direct((u32)tbl, &XB_K[0], (u16)id)) {
                // Retail pushes the masked lookup value, not the raw item id:
                // each `clrlwi. r4, r3, 16` result feeds func_801C5158's r4.
                #define XB_PUSH(tblArg, kOff) \
                do { \
                    v = BdatGetU16Direct((const void*)(tblArg), &XB_K[kOff], (u16)id); \
                    if (v) func_801C5158(self, v); \
                } while (0)
                if (tp == 1) {
                    // Both lookups are issued before either result is tested.
                    u8 c1 = BdatGetU8Direct((u32)tbl, &XB_K[0xd], (u16)id);
                    u8 c2 = BdatGetU8Direct((u32)tbl, &XB_K[0], (u16)id);
                    if (c1 == 2) XB_PUSH(tbl, 0x1a);
                    if (c2 == 2) XB_PUSH(tbl, 0x26);
                } else if (tp == 0x6f) {
                    if ((u8)BdatGetU8Direct((u32)tbl, &XB_K[0x32], (u16)id) == 2) XB_PUSH(tbl, 0x3f);
                } else if (tp == 0x79) {
                    if ((u8)BdatGetU8Direct((u32)tbl, &XB_K[0x4b], (u16)id) == 2) XB_PUSH(tbl, 0x58);
                } else if (tp == 0x83) {
                    if ((u8)BdatGetU8Direct((u32)tbl, &XB_K[0x64], (u16)id) == 2) XB_PUSH(tbl, 0x71);
                } else if (tp == 0x70) {
                    if ((u8)BdatGetU8Direct((u32)tbl, &XB_K[0x7d], (u16)id) == 2) XB_PUSH(tbl, 0x8a);
                } else if (tp == 0x7a) {
                    if ((u8)BdatGetU8Direct((u32)tbl, &XB_K[0x96], (u16)id) == 2) XB_PUSH(tbl, 0xa3);
                } else if (tp == 0x84) {
                    if ((u8)BdatGetU8Direct((u32)tbl, &XB_K[0xaf], (u16)id) == 2) XB_PUSH(tbl, 0xbc);
                }
            } else if ((u8)BdatGetU8Direct((u32)tbl, &XB_K[0xc8], (u16)id)) {
                // Teach-key dispatch (subset of types).
                if (tp == 1) {
                    if ((u8)BdatGetU8Direct((u32)tbl, &XB_K[0xd], (u16)id) == 2) XB_PUSH(tbl, 0x1a);
                } else if (tp == 0x6f) {
                    if ((u8)BdatGetU8Direct((u32)tbl, &XB_K[0x32], (u16)id) == 2) XB_PUSH(tbl, 0x3f);
                } else if (tp == 0x79) {
                    if ((u8)BdatGetU8Direct((u32)tbl, &XB_K[0x4b], (u16)id) == 2) XB_PUSH(tbl, 0x58);
                } else if (tp == 0x83) {
                    if ((u8)BdatGetU8Direct((u32)tbl, &XB_K[0x64], (u16)id) == 2) XB_PUSH(tbl, 0x71);
                }
            } else {
                // Fallback: no base name and no teach key - check ability slots
                u32 obj = (u32)UIWin_BuildFlagBuf((u16)id);
                if (!func_80140854((CItemQuery*)obj, 0, 0)) {
                    if ((u8)BdatGetU8Direct((u32)tbl, &XB_K[0xd], (u16)id) == 2) XB_PUSH(tbl, 0x1a);
                }
                if (!func_80140854((CItemQuery*)obj, 0, 1)) {
                    if ((u8)BdatGetU8Direct((u32)tbl, &XB_K[0x32], (u16)id) == 2) XB_PUSH(tbl, 0x3f);
                }
                if (!func_80140854((CItemQuery*)obj, 0, 2)) {
                    if ((u8)BdatGetU8Direct((u32)tbl, &XB_K[0x4b], (u16)id) == 2) XB_PUSH(tbl, 0x58);
                }
                if (!func_80140854((CItemQuery*)obj, 0, 3)) {
                    if ((u8)BdatGetU8Direct((u32)tbl, &XB_K[0x64], (u16)id) == 2) XB_PUSH(tbl, 0x71);
                }
            }
        }
    }
}
// Add id to list if not already present and capacity check passes.
// noinline: func_801C53D8 (and the other callers in this TU) must emit `bl`
// to the retail symbol - the unit builds with -inline and would otherwise
// fold this body into every call site (tripling func_801C53D8).
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void func_801C5158(void* self, u32 id) {
    if (HasSlotIdMatch(self, id)) return;
    if ((BdatGetItemType(id) & 0xFFFF) == 12) return;
    u8* p = (u8*)self;
    u16 count = *(u16*)(p + 0x804);
    *(u16*)(p + count * 2 + 4) = (u16)id;
    *(u16*)(p + 0x804) = count + 1;
}
#pragma pop
void func_801C5254(void* self) {
    void* bdat = getFP__FPCc(&lbl_eu_8050566C[0xd5]);
    if (!bdat) return;

    u16 count = (u16)func_8003B1EC(bdat);
    u16 i;
    for (i = 1; i <= count; i++) {
        if (!func_8009CF8C((u32)(i + 0x2596))) continue;

        u32 flag1 = BdatGetU16Direct(bdat, (const char*)&lbl_eu_8050566C[0xe2], (u32)i);
        if (flag1 & 0xFFFF) {
            u8 val = (u8)func_8009CF8C((flag1 & 0xFFFF) + 0x220);
            if (val >= 0xFE) continue;
            if (val > (u8)BdatGetU8Direct((u32)bdat, (const char*)&lbl_eu_8050566C[0xea], (u32)i)) continue;
        } else {
            u32 hasVal = BdatGetU8Direct((u32)bdat, (const char*)&lbl_eu_8050566C[0xf3], (u32)i);
            if (hasVal & 0xFF) {
                if ((u8)BdatGetU8Direct((u32)bdat, (const char*)&lbl_eu_8050566C[0xfa], (u32)i)
                    <= (u8)func_8009CF8C((hasVal & 0xFF) + 0x7fc)) continue;
            }
        }

        u32 type = BdatGetU16Direct(bdat, (const char*)&lbl_eu_8050566C[0x102], (u32)i);
        if (type & 0xFFFF) {
            func_801C5158(self, type & 0xFFFF);
        }
    }
}

// Scan the skill-learn table (lbl_eu_80664110) for entries 1..count and
// push each one into the list when its per-party table cell has the
// "learned" flag bit set. The cell address is charData + party * 0x49 +
// dialogType * 2; bits at +0xE8/+0xE9 select the check by category.
void func_801C53D8(void* self) {
    u32 bdat = lbl_eu_80664110;
    s32 count = (s32)func_8003B1EC((void*)bdat);
    u8 i;
    for (i = 1; i <= count; i++) {
        u32 flag = BdatGetU8Direct(bdat, (const char*)&lbl_eu_8050566C[0x109], (u32)i);
        u32 charId = BdatGetU8Direct(bdat, (const char*)&lbl_eu_8050566C[0x115], (u32)i);
        u32 unk = BdatGetU8Direct(bdat, (const char*)&lbl_eu_8050566C[0x11d], (u32)i);
        u32 sub = BdatGetU8ByTableKey((void*)&lbl_eu_8050566C[0x126], (const char*)&lbl_eu_8050566C[0x12e],
                            (u8)unk);
        // Reuse the charId slot for the character-data pointer (retail
        // recycles its register); then add the party stride and sub offset.
        charId = (u32)func_8009EC9C((u8)charId);
        u8* cell = (u8*)charId + (u8)CtrlObjectParam_GetCurrentRowKey() * 0x49;
        cell = cell + (u8)sub * 2;
        switch ((u8)flag) {
        case 1:
            if (cell[0xE8] != 0) {
                func_801C5158(self, (u32)i);
            }
            break;
        case 2:
            if ((cell[0xE9] >> 7) & 1) {
                func_801C5158(self, (u32)i);
            }
            break;
        case 3:
            if ((cell[0xE9] >> 6) & 1) {
                func_801C5158(self, (u32)i);
            }
            break;
        }
    }
}

// CItemBoxGrid::OnFileEvent - file-load completion dispatch. Four pending
// load slots are keyed by the event's file handle: +0x28 builds the whole
// item-box layout (fonts, animations, cursors, colour tables), +0x2c loads
// the secondary arc, +0x30/+0x34 register two BDAT tables.
extern "C" int OnFileEvent__12CItemBoxGridFP10CEventFile(void* self, void* event) {
    u8* p = (u8*)self;
    // Keep the handle comparison operandless so MWCC holds it in r4 like retail.
    if (*(u32*)(p + 0x28) == *(u32*)((u8*)event + 4)) {
    void* memHandle = getHandleMEM2__Q23mtl10MemManagerFv();
    createRegion__17UnkClass_8045F564FiiPCci(p + 8, memHandle, 0x28000, &lbl_eu_8050566C[0x64d], 0);
    u8 regionBuf[0x14];
    __ct__14Class_8045F858FP17UnkClass_8045F564(regionBuf, p + 8);
    u32* fileData = *(u32**)(p + 0x28);
    void* fileBuf = (void*)fileData[1];
    fileData[1] = 0;
    setMemInitFlag__Q23mtl10MemManagerFb(0);
    void* allocHandle = getAllocHandle__10CLibLayoutFv();
    void* tpMem = allocate__Q23mtl10MemManagerFUlUl(0x858, (u32)allocHandle);
    if (tpMem != NULL) __ct__CTagProcessor(tpMem);
    *(void**)(p + 0x5c) = tpMem;
    nw4r::lyt::ArcResourceAccessor* arcRes = createArcResourceAccessor__10CLibLayoutFv();
    *(nw4r::lyt::ArcResourceAccessor**)(p + 0x3c) = arcRes;
    Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(arcRes, fileBuf, &lbl_eu_8050566C[0x65a]);
    buildLayout__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
        (nw4r::lyt::Layout**)(p + 0x44), arcRes, &lbl_eu_8050566C[0x65e]);
    nw4r::lyt::Layout* layout = *(nw4r::lyt::Layout**)(p + 0x44);
    bindLayoutAnimTransform__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        layout, (nw4r::lyt::AnimTransform**)(p + 0x48), arcRes, (char*)&lbl_eu_8050566C[0x673]);
    bindLayoutAnimTransform__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        layout, (nw4r::lyt::AnimTransform**)(p + 0x4c), arcRes, (char*)&lbl_eu_8050566C[0x68b]);
    bindLayoutAnimTransform__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        layout, (nw4r::lyt::AnimTransform**)(p + 0x50), arcRes, (char*)&lbl_eu_8050566C[0x6a8]);
    // Pane lookups go through the layout's root pane.
    nw4r::lyt::Pane* paneHost = layout->GetRootPane();
    void* font = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, layout);
    void* fontData = ((IDeviceFontInfo*)font)->getFont();
    func_8013676C(paneHost, (u32)fontData);
    u32 screenW = (u32)getPackedFont__Fv();
    setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(layout, (char*)&lbl_eu_8050566C[0x55d], screenW);
    setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(layout, (char*)&lbl_eu_8050566C[0x53e], screenW);
    u32 screenH = (u32)CUICfManager_getPackedFont9C();
    u8 i;
    for (i = 1; i <= 0x1e; i++) {
        char nameBuf[0x20];
        sprintf(nameBuf, (const char*)&lbl_eu_8050566C[0x3a3], i);
        setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(layout, nameBuf, screenH);
    }
    setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(layout, (char*)&lbl_eu_8050566C[0x59b], screenH);
    // Initial animation visibility: only the 0x48 transform starts shown.
    (layout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)*(void**)(p + 0x4c), 0);
    (layout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)*(void**)(p + 0x50), 0);
    (layout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)*(void**)(p + 0x48), 1);
    layout->Animate(0);
    func_80139198(0);
    // Bind the tag processor to the message pane.
    void* tpPane = paneHost->FindPaneByName((char*)&lbl_eu_8050566C[0x58a], 1);
    if (tpPane != NULL) *(u32*)((u8*)tpPane + 0xf8) = *(u32*)(p + 0x5c);
    // Window title / caption strings.
    char* msgA = BdatTouchStringCell(&lbl_eu_8050566C[0x14f], &lbl_eu_8050566C[0x158], 5);
    LayoutSetTextBoxFmtValue(layout, (char*)&lbl_eu_8050566C[0x6c2], msgA, 0);
    char* msgB = BdatTouchStringCell(&lbl_eu_8050566C[0x14f], &lbl_eu_8050566C[0x158], 4);
    LayoutSetTextBoxFmtValue(layout, (char*)&lbl_eu_8050566C[0x6d1], msgB, 0);
    LayoutSetTextBoxFmtValue(layout, (char*)&lbl_eu_8050566C[0x642], (char*)&lbl_eu_8050566C[0x3af], 0);
    // Mode-dependent help texture.
    const char* modeStr = &lbl_eu_8050566C[0x6e6];
    if (isClassicController__Q22cf13CfGameManagerFv(-1)) modeStr = &lbl_eu_8050566C[0x6dd];
    u16 msgKey = BdatGetU16ByTableKey(&lbl_eu_8050566C[0x6ef], modeStr, 0x49);
    u32 msgId = (u32)MakeTplNameSysFile(msgKey);
    void* sysWin = CUICfManager_getArcResourceAccessor();
    void* texRes = ((nw4r::lyt::ArcResourceAccessor*)sysWin)->GetResource(0x74696D67, (const char*)msgId, 0);
    if (texRes != NULL) {
        PaneSetTexPaletteByName(layout, (void*)&lbl_eu_8050566C[0x6fd], texRes);
        void* pane2 = paneHost->FindPaneByName((char*)&lbl_eu_8050566C[0x6fd], 1);
        if (pane2 != NULL) {
            CibgTexMat* mat = *(CibgTexMat**)((u8*)texRes + 8);
            CibgTexData* tex = *(CibgTexData**)mat->field_00;
            u16 w = tex->field_02;
            u16 h = tex->field_00;
            ((float*)((u8*)pane2 + 0x4c))[0] = (float)(s32)w;
            ((float*)((u8*)pane2 + 0x50))[0] = (float)(s32)h;
        }
    }
    // Hide the icons that do not apply to this box mode.
    if (p[0x527] != 1) {
        void* iconPane = paneHost->FindPaneByName((char*)&lbl_eu_8050566C[0x6c2], 1);
        func_80124270(iconPane, 0);
        if (p[0x527] != 2) {
            void* iconPane2 = paneHost->FindPaneByName((char*)&lbl_eu_8050566C[0x59b], 1);
            func_80124270(iconPane2, 0);
        }
    }
    // Snapshot the cursor pane's initial translation.
    void* posPane = paneHost->FindPaneByName((char*)&lbl_eu_8050566C[0x709], 1);
    copyVEC3(p + 0x534, (u8*)posPane + 0x2c);
    // Snapshot both cursor colour states into the global colour tables.
    CEquipBoxFourShorts colA = func_80139658(layout, &lbl_eu_8050566C[0x716], 0);
    CopyVec4s(&lbl_eu_80664488, &colA);
    CEquipBoxFourShorts colB = func_80139658(layout, &lbl_eu_8050566C[0x716], 1);
    CopyVec4s(&lbl_eu_80664490, &colB);
    // Fan the two base colours' kind indices out over the eight table rows.
    s16 colAW = *(s16*)((u8*)&lbl_eu_80664488 + 6);
    s16 colBW = *(s16*)((u8*)&lbl_eu_80664490 + 6);
    *(s16*)((u8*)&lbl_eu_80664498 + 6) = colAW;
    *(s16*)((u8*)&lbl_eu_806644A0 + 6) = colBW;
    *(s16*)((u8*)&lbl_eu_806644A8 + 6) = colAW;
    *(s16*)((u8*)&lbl_eu_806644B0 + 6) = colBW;
    *(s16*)((u8*)&lbl_eu_806644B8 + 6) = colAW;
    *(s16*)((u8*)&lbl_eu_806644C0 + 6) = colBW;
    *(s16*)((u8*)&lbl_eu_806644C8 + 6) = colAW;
    *(s16*)((u8*)&lbl_eu_806644D0 + 6) = colBW;
    // Build each cursor from a stack temporary handed to the attach helper.
    u8 cur07Buf[0x18];
    __ct__CCur07(cur07Buf, *(void**)(p + 0x3c));
    func_8018B0FC(p + 0x70, cur07Buf);
    __dt__6CCur07Fv(cur07Buf, -1);
    ((CBaseCur*)(p + 0x70))->initLayout();
    u8 cur09Buf[0x18];
    __ct__CCur09(cur09Buf, *(void**)(p + 0x3c));
    func_8018B0FC(p + 0x88, cur09Buf);
    __dt__6CCur09Fv(cur09Buf, -1);
    ((CBaseCur*)(p + 0x88))->initLayout();
    // Cursor-09 move bounds.
    CibgVec3 vecMin;
    code80135FDC_setVec3((float*)&vecMin, lbl_eu_80667F90, lbl_eu_80667F94, lbl_eu_80667F34);
    CibgVec3 vecMax;
    code80135FDC_setVec3((float*)&vecMax, lbl_eu_80667F98, lbl_eu_80667F94, lbl_eu_80667F34);
    func_801D24E8(p + 0x88, vecMax, vecMin);
    u8 cur18Buf[0x18];
    __ct__CCur18(cur18Buf, CUICfManager_getArcResourceAccessor());
    func_8018B0FC(p + 0xa0, cur18Buf);
    __dt__6CCur18Fv(cur18Buf, -1);
    ((CBaseCur*)(p + 0xa0))->initLayout();
    u8 cur16Buf[0x18];
    __ct__CCur16(cur16Buf, *(void**)(p + 0x3c));
    func_8018B0FC(p + 0xb8, cur16Buf);
    __dt__6CCur16Fv(cur16Buf, -1);
    ((CBaseCur*)(p + 0xb8))->initLayout();
    u8 cur11Buf[0x18];
    __ct__CCur11(cur11Buf, *(void**)(p + 0x3c));
    func_8018B0FC(p + 0xd0, cur11Buf);
    __dt__6CCur11Fv(cur11Buf, -1);
    ((CBaseCur*)(p + 0xd0))->initLayout();
    func_8022D614(p + 0x468, *(void**)(p + 0x3c));
    func_80207FC8(p + 0x418, *(void**)(p + 0x3c));
    // Group colour snapshot for the grid cells.
    void* grpPane = paneHost->FindPaneByName((char*)&lbl_eu_8050566C[0x723], 1);
    if (grpPane != NULL) {
        CEquipBoxFourShorts gcolA = func_801397AC(grpPane, 0);
        CopyVec4s(&lbl_eu_806644D8, &gcolA);
        CEquipBoxFourShorts gcolB = func_801397AC(grpPane, 1);
        CopyVec4s(&lbl_eu_806644E0, &gcolB);
        s16 gAW = *(s16*)((u8*)&lbl_eu_806644D8 + 6);
        s16 gBW = *(s16*)((u8*)&lbl_eu_806644E0 + 6);
        *(s16*)((u8*)&lbl_eu_806644E8 + 6) = gAW;
        *(s16*)((u8*)&lbl_eu_806644F0 + 6) = gBW;
        *(s16*)((u8*)&lbl_eu_806644F8 + 6) = gAW;
        *(s16*)((u8*)&lbl_eu_80664500 + 6) = gBW;
    }
    char* msgD = BdatTouchStringCell(&lbl_eu_8050566C[0x14f], &lbl_eu_8050566C[0x158], 7);
    LayoutSetTextBoxFmtValue(layout, (char*)&lbl_eu_8050566C[0x72d], msgD, 0);
    if (p[0x527] != 4) {
        void* iconPane3 = paneHost->FindPaneByName((char*)&lbl_eu_8050566C[0x637], 1);
        func_80124270(iconPane3, 0);
        LayoutSetTextBoxFmtValue(layout, (char*)&lbl_eu_8050566C[0x72d], (char*)&lbl_eu_8050566C[0x3af], 0);
    }
    // Mode 2 (party members): the first member with the relevant skill gets
    // a highlight fade-in driven by PaneMatSetTevColorsByName with inline colour blocks.
    if (p[0x527] == 2) {
        u8 cnt = code80135FDC_getByte_64077();
        u8 j;
        for (j = 0; j < cnt; j++) {
            void* chp = (u8*)func_8009EC9C(GetCollectedFlagByte(j)) + 0x3534;
            if (!func_8026178C(chp, 0x8f) && !func_8026178C(chp, 0x93)) continue;
            CEquipBoxFourShorts cA = func_80139658(layout, &lbl_eu_8050566C[0x59b], 0);
            CEquipBoxFourShorts cB = func_80139658(layout, &lbl_eu_8050566C[0x59b], 1);
            struct FadeRefBlk {
                void* layout;
                const char* name;
            };
            struct ColPairBlk {
                u32 lo;
                u32 hi;
            };
            FadeRefBlk blk1 = { layout, &lbl_eu_8050566C[0x59b] };
            FadeRefBlk blk2 = { layout, &lbl_eu_8050566C[0x59b] };
            ColPairBlk cbCol;
            cbCol.lo = 0x0046B400;
            cbCol.hi = (u32)(u16)cB.d << 16;
            ColPairBlk caCol;
            caCol.lo = 0;
            caCol.hi = (u32)(u16)cA.d << 16;
            PaneMatSetTevColorsByName(layout, (char*)&lbl_eu_8050566C[0x59b], &caCol, &cbCol);
            break;
        }
    }
    SetGridReady(self);
    *(u32*)(p + 0x28) = 0;
    validateHeap__17UnkClass_8045F564Fv(p + 8);
    __dt__14Class_8045F858Fv(regionBuf, -1);
    return 1;
    }
    // Secondary arc resource load (+0x2c slot).
    if (*(u32*)(p + 0x2c) == *(u32*)((u8*)event + 4)) {
        void* memHandle = getHandleMEM2__Q23mtl10MemManagerFv();
        createRegion__17UnkClass_8045F564FiiPCci(p + 0x18, memHandle, 0x100, &lbl_eu_8050566C[0x73a], 0);
        u8 regionBuf[0x14];
        __ct__14Class_8045F858FP17UnkClass_8045F564(regionBuf, p + 0x18);
        u32* fileData = *(u32**)(p + 0x2c);
        void* fileBuf = (void*)fileData[1];
        fileData[1] = 0;
        setMemInitFlag__Q23mtl10MemManagerFb(0);
        nw4r::lyt::ArcResourceAccessor* arcRes = createArcResourceAccessor__10CLibLayoutFv();
        *(nw4r::lyt::ArcResourceAccessor**)(p + 0x40) = arcRes;
        Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(arcRes, fileBuf, &lbl_eu_8050566C[0x65a]);
        SetGridReady(self);
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
        func_8003AA34();
        if (getFP__FPCc(&lbl_eu_8050566C[0x2d0]) == NULL)
            setBdatEntry__5CBdatFUlPv(2, data);
        lbl_eu_80664508 = (u32)getFP__FPCc(&lbl_eu_8050566C[0x2d0]);
        lbl_eu_8066450C = (u32)getFP__FPCc(&lbl_eu_8050566C[0x2df]);
        SetGridReady(self);
        *(u32*)(p + 0x30) = 0;
        return 1;
    }
    if (*(u32*)(p + 0x34) == *(u32*)((u8*)event + 4)) {
        u32* fileData = *(u32**)(p + 0x34);
        void* data = (void*)fileData[1];
        fileData[1] = 0;
        *(void**)(p + 0x38) = data;
        func_8003AA34();
        if (getFP__FPCc(&lbl_eu_8050566C[0x2ee]) == NULL)
            setBdatEntry__5CBdatFUlPv(5, *(void**)(p + 0x38));
        lbl_eu_80664510 = (u32)getFP__FPCc(&lbl_eu_8050566C[0x2ee]);
        SetGridReady(self);
        *(u32*)(p + 0x34) = 0;
        return 1;
    }
    return 0;
}
// Static initialization of color table entries.
void sinit_801D1E30(void) {
    SplitU32ToS16s((short*)&lbl_eu_80664488, 0);
    SplitU32ToS16s((short*)&lbl_eu_80664490, 0);
    func_801C4B60((void*)&lbl_eu_80664498, 0x79, 0x49, 0x07, 0x00);
    func_801C4B60((void*)&lbl_eu_806644A0, 0xED, 0xCD, 0x83, 0x00);
    func_801C4B60((void*)&lbl_eu_806644A8, 0x1A, 0x43, 0x53, 0x00);
    func_801C4B60((void*)&lbl_eu_806644B0, 0xC4, 0xE8, 0xEB, 0x00);
    func_801C4B60((void*)&lbl_eu_806644B8, 0x74, 0x54, 0x1D, 0x00);
    func_801C4B60((void*)&lbl_eu_806644C0, 0xD5, 0xB9, 0x78, 0x00);
    func_801C4B60((void*)&lbl_eu_806644C8, 0x3D, 0x68, 0x78, 0x00);
    func_801C4B60((void*)&lbl_eu_806644D0, 0xC4, 0xE8, 0xEB, 0x00);
    SplitU32ToS16s((short*)&lbl_eu_806644D8, 0);
    SplitU32ToS16s((short*)&lbl_eu_806644E0, 0);
    func_801C4B60((void*)&lbl_eu_806644E8, 0x12, 0xA3, 0xE7, 0x00);
    func_801C4B60((void*)&lbl_eu_806644F0, 0xFF, 0xFF, 0xFF, 0x00);
    func_801C4B60((void*)&lbl_eu_806644F8, 0xB3, 0x09, 0xC0, 0x00);
    func_801C4B60((void*)&lbl_eu_80664500, 0xFF, 0xFF, 0xFF, 0x00);
}

extern u8 lbl_eu_805347F8[];

