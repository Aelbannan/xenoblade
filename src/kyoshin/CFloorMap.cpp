// Auto-scaffolded catalog TU for kyoshin/CFloorMap
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CFloorMap.hpp"
#include <cstdio>
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

// Named .sdata2 conversion magics: defining them lets MWCC's constant pool
// reuse the retail symbols for the (f32) casts on s16/u8 map coordinates
// instead of emitting TU-local @N labels (CMiniMap.cpp idiom).
extern const double lbl_eu_80668770 = 0x4330000080000000ll;
extern const double lbl_eu_80668788 = 0x4330000000000000ll;

extern u32 lbl_eu_8066479C;
extern u32 lbl_eu_80664184;
extern u16 lbl_eu_8050B798[];
extern u32 func_8003B1EC(u32);
extern u32 lbl_eu_8050BDF8[];
extern s16 func_80136330(u32, const char*, u32);
extern u8 lbl_eu_80664798;

extern void func_80137C1C(void*, void*);
extern void* createPicture__10CLibLayoutFv();
extern void SetName__Q34nw4r3lyt4PaneFPCc(void*, const char*);

// Draw helpers used by CFloorMap::Draw and related functions
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
extern "C" void func_801F35B0(void* scrollBar, void* drawInfo);
extern "C" void func_8022B7C8(void* sysWin, void* drawInfo);
extern "C" void func_801D20B0(void* cursor, void* drawInfo);

extern float lbl_eu_80668764;
extern float lbl_eu_80668794;
extern float lbl_eu_80668798;
extern float lbl_eu_806687A4;
extern float lbl_eu_806687A8;
extern float lbl_eu_806687AC;
extern float lbl_eu_806687B0;
extern float lbl_eu_806687B4;
extern float lbl_eu_806687B8;

typedef void* (*VFuncPtr)(void*, const char*, u32);

// Plain-C prototype so MWCC recognizes memset as an intrinsic (the <cstring>
// header declaration defeats the inline expansion).
void* memset(void*, int, unsigned long);

void* func_80248920(void* self, const char* name, f32 x, f32 y, void* arg5, const char* paneName);

u8 func_8024CE60(void* self) { return static_cast<CFloorMapFull*>(self)->field_40; }


// Move the map cursor down one row on the current page, wrapping through
// pages and resetting to the first row once the last visible row is passed.
// Refresh the 5 visible row panes for the current page: each row's text pane
// (name built from the page index) gets its string from the merged marker
// list, and its color dimmed when the marker is flagged or the region flag
// at 0x3212 is set.
void func_80246200(void* self) {
    CFloorMapCursor* cur = (CFloorMapCursor*)self;
    u8* base = (u8*)self + cur->field_09 * 0x30C;
    char buf[0x20];
    u32 color;
    u8* rec;
    char* tbl = lbl_eu_8050BEA8;
    u8* markers = base + 0x10;
    u8 count = base[0x318];
    u32 i;
    for (i = 0; i < 5; i++) {
        sprintf(buf, tbl + 0x14D, (u8)i);
        s32 row = (s8)cur->field_0B + (u8)i;
        if (row >= count) {
            // No marker on this visible row: show the default blank label.
            func_80136B4C(cur->mData, buf, tbl + 0x1C0, 0);
        } else {
            rec = markers + row * 0x18;
            func_80136B4C(cur->mData, buf, *(char**)(rec + 0x18), 0);
            color = -1;
            if (rec[0x1C] != 0 || func_8009CF8C(0x3212) != 0)
                color = 0xC85050FF;
            func_80137B44(cur->mData, buf, color);
        }
    }
}

void func_80245450(void* self) {
    CFloorMapCursor* cur = (CFloorMapCursor*)self;
    u8 count = *(u8*)((u8*)self + cur->field_09 * 0x30C + 0x318);
    if (count >= 5) {
        cur->field_0A = cur->field_0A + 1;
        if ((s8)cur->field_0A >= 5) {
            cur->field_0A = 4;
            cur->field_0B = cur->field_0B + 1;
            if ((s8)cur->field_0B > count - 5) {
                cur->field_0A = 0;
                cur->field_0B = 0;
            }
        }
    } else {
        cur->field_0A = cur->field_0A + 1;
        if ((s8)cur->field_0A >= count) {
            cur->field_0A = 0;
            cur->field_0B = 0;
        }
    }
    func_80246200(cur);

    // Refresh the cursor position: format the pane name for the current page
    // index, scale its translate by the zoom pane, and move the target pane.
    const char* tbl = lbl_eu_8050BEA8;
    nw4r::math::_VEC3 dest;
    nw4r::math::_VEC3 pos;
    char buf[0x20];
    sprintf(buf, tbl + 0x14D, (s8)cur->field_0A);

    nw4r::lyt::Pane* pane =
        cur->mData->GetRootPane()->FindPaneByName(buf, true);
    func_801375A0((nw4r::math::VEC3*)&pos, pane);

    nw4r::lyt::Pane* scalePane =
        cur->mData->GetRootPane()->FindPaneByName(tbl + 0x136, true);
    pos.x = pos.x * scalePane->GetScale().x;

    // Word-wise copy so MWCC uses integer registers like retail.
    u32* src = (u32*)&pos;
    u32* dst = (u32*)&dest;
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];

    if (cur->field_3108 != NULL) {
        ((CFloorMapLayoutHolder*)cur->field_3108)
            ->pane->SetTranslate(*(nw4r::math::VEC3*)&dest);
    }

    func_801F3850(cur->field_3134, cur->field_0B);
}

// Retail keeps this helper's symbol unmangled.
extern "C" void func_802455F0(void* self) {
    CFloorMapCursor* cur = (CFloorMapCursor*)self;
    CFloorMapFloorEntry* entries = reinterpret_cast<CFloorMapFloorEntry*>(cur->_0C);
    u8 count = *(u8*)((u8*)self + cur->field_09 * 0x30C + 0x318);
    if (count >= 5) {
        // Page down by one row; wrap to the next page once the row index
        // passes the last visible row on this page.
        u8 nb = (u8)(cur->field_0B + 5);
        cur->field_0B = nb;
        if ((s8)nb > (s32)(count - 5)) {
            u8 na = (u8)(nb - (count - 5));
            cur->field_0A = na;
            cur->field_0B = (u8)(count - 5);
            if ((s8)na >= 5)
                cur->field_0A = 4;
        }
    } else {
        u8 c1 = (u8)(count - 1);
        cur->field_0A = c1;
        cur->field_0B = 0;
        if ((s8)c1 < 0)
            cur->field_0A = 0;
    }
    func_80246200(cur);

    // Refresh the cursor position: format the pane name for the current page
    // index, scale its translate by the zoom pane, and move the target pane.
    nw4r::math::VEC3 dest;
    nw4r::math::VEC3 pos;
    char buf[0x20];
    sprintf(buf, &lbl_eu_8050BEA8[0x14D], (s8)cur->field_0A);

    nw4r::lyt::Pane* pane = cur->mData->GetRootPane()->FindPaneByName(buf, 1);
    func_801375A0(&pos, pane);

    nw4r::lyt::Pane* scalePane =
        cur->mData->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);
    pos.x = pos.x * scalePane->GetScale().x;

    dest = pos;
    if (cur->field_3108) {
        CFloorMapCursorTarget* target = (CFloorMapCursorTarget*)cur->field_3108;
        target->pane->SetTranslate(dest);
    }

    func_801F3850(cur->field_3134, (u16)(s8)cur->field_0B);
}

// Select the floor-map row whose map-row id matches val: move the cursor to
// that row (clamped to a page of 5), reposition the target pane, and notify
// the UI when the cursor actually moved.
void func_8024577C(void* self, u16 val) {
    CFloorMapCursor* cur = (CFloorMapCursor*)self;
    if (!val) {
        cur->field_0A = -1;
        cur->field_0B = 0;
        return;
    }
    s8 fi = cur->field_09;
    // Hoisted floor block: row-marker record i sits at base + i*0x18,
    // its row/map id in the record's +0x18 slot.
    u8* base;
    u8 count;
    u8 i;
    u8 oldA;
    u8 oldB;
    int off;
    i = 0;
    oldA = cur->field_0A;
    oldB = cur->field_0B;
    off = fi * 0x30C;
    base = (u8*)self + off;
    count = base[0x318];

    while (i < count) {
        if (*(const u16*)(i * 0x18 + 0x18 + base) == val) {
            if (i >= 5) {
                cur->field_0A = 4;
                cur->field_0B = i - 4;
            } else {
                cur->field_0A = i;
                cur->field_0B = 0;
            }
            func_80246200(cur);

            // Refresh the cursor position: format the pane name for the
            // current page index, scale its translate by the zoom pane, and
            // move the target pane.
            nw4r::math::VEC3 dest;
            nw4r::math::VEC3 pos;
            char buf[0x20];
            const char* tbl = lbl_eu_8050BEA8;
            sprintf(buf, &tbl[0x14D], (s8)cur->field_0A);

            nw4r::lyt::Pane* pane = cur->mData->GetRootPane()->FindPaneByName(buf, 1);
            func_801375A0(&pos, pane);

            nw4r::lyt::Pane* scalePane =
                cur->mData->GetRootPane()->FindPaneByName(&tbl[0x136], 1);
            pos.x = scalePane->GetScale().x * pos.x;

            // Word-wise copy so MWCC keeps the retail integer-register copy
            // instead of forwarding pos into the inlined SetTranslate.
            ((u32*)&dest)[0] = ((u32*)&pos)[0];
            ((u32*)&dest)[1] = ((u32*)&pos)[1];
            ((u32*)&dest)[2] = ((u32*)&pos)[2];
            if (cur->field_3108) {
                CFloorMapCursorTarget* target = (CFloorMapCursorTarget*)cur->field_3108;
                target->pane->SetTranslate(dest);
            }

            func_801F3850(cur->field_3134, (u16)cur->field_0B);
            break;
        }
        i++;
    }
    if (oldA != (s8)cur->field_0A || oldB != (s8)cur->field_0B)
        func_80138078(1);
}

// Rebuild the floor-name entries from the current floor table, set the
// per-floor row panes' visibility/color (found floors use one color set),
// position the map panes under the current floor, and update the two status
// panes' flags for the current map.
void func_80245950(void* self) {
    CFloorMapCursor* cur = (CFloorMapCursor*)self;
    u8 count = func_8003B1EC(lbl_eu_8066479C);
    cur->field_30D0 = count;

    char buf2[0x20];
    char buf3[0x20];
    char buf4[0x20];

    // Fill the floor entries from the table in reverse order.
    // fi is an int walking below -128 for large counts, so the (s8) cast
    // cannot be folded and MWCC re-emits extsb+mulli every iteration.
    // Declared e-then-fi-then-i so MWCC colors them onto retail's regs.
    CFloorMapFloorNameEntry* e;
    u8 i;
    int fi;
    fi = (s8)(count - 1);
    i = 1;
    while (i <= count) {
        e = (CFloorMapFloorNameEntry*)((u8*)self + (u32)((s8)fi * 0x30C));
        fi--;
        e->field_10 = i;
        e->field_12 = func_80136330(lbl_eu_8066479C, &lbl_eu_8050BEA8[0x15A], i);
        e->field_14 = (u32)func_8013639C((const void*)lbl_eu_8066479C, (const void*)&lbl_eu_8050BEA8[0x161], i);
        e->field_318 = 0;
        i++;
    }

    // Name the floor panes bottom-up; floors present in the current map use
    // the found-color table, the rest the default table.
    for (u8 k = count; k >= 1; k--) {
        int found = 0;
        u8 floorId = count - (k - 1);
        for (u32 idx = 0; idx < 0x240; idx++) {
            if (func_8013AC3C(lbl_eu_80664798, floorId, idx + 1)) {
                found = 1;
                break;
            }
        }
        sprintf(buf2, &lbl_eu_8050BEA8[0x12A], k);
        if (found)
            func_80139A18(cur->mData, buf2, lbl_eu_806647B0, lbl_eu_806647B8);
        else
            func_80139A18(cur->mData, buf2, lbl_eu_806647A0, lbl_eu_806647A8);
    }

    // Show the first `count` row panes; each pane's low flag bit is set when
    // its index is below the floor count.
    for (u8 k = 0; k < 10; k++) {
        sprintf(buf3, &lbl_eu_8050BEA8[0x12A], k + 1);
        nw4r::lyt::Pane* pane = cur->mData->GetRootPane()->FindPaneByName(buf3, 1);
        CFloorMapPaneFlag* pf = (CFloorMapPaneFlag*)pane;
        pf->mFlag = (pf->mFlag & 0x7F) | (k < count);
    }

    // Pick the current floor entry: the first entry whose floor id matches
    // the current-floor flag (or id 1 when the flag is unset).
    for (u8 i = 0; i < count; i++) {
        s8 fl = cur->field_08;
        CFloorMapFloorNameEntry* e =
            (CFloorMapFloorNameEntry*)((u8*)self + i * 0x30C);
        if (fl == 0) {
            if (e->field_10 == 1) {
                cur->field_09 = i;
                break;
            }
        } else if ((s8)e->field_10 == fl) {
            cur->field_09 = i;
            break;
        }
    }

    // Position the map pane under the current floor.
    sprintf(buf4, &lbl_eu_8050BEA8[0x12A], cur->field_09 + 1);
    nw4r::lyt::Pane* pane = cur->mData->GetRootPane()->FindPaneByName(buf4, 1);
    nw4r::math::VEC3 pos;
    func_801375A0(&pos, pane);
    nw4r::lyt::Pane* scalePane =
        cur->mData->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);
    pos.x = pos.x * scalePane->GetScale().x;
    if (cur->field_30D8)
        CFloorMapSetSlotPos((CFloorMapLayoutHolder*)cur->field_30D8, *(nw4r::math::_VEC3*)&pos);

    CFloorMapFloorNameEntry* eSel =
        (CFloorMapFloorNameEntry*)((u8*)self + cur->field_09 * 0x30C);
    func_80136B4C(cur->field_30D8, &lbl_eu_8050BEA8[0xF8], *(char**)&eSel->field_14, 0);
    func_80136B4C((nw4r::lyt::Layout*)lbl_eu_806640A8, &lbl_eu_8050BEA8[0x145],
                  func_8013639C((const void*)lbl_eu_806640A8, (const void*)&lbl_eu_8050BEA8[0x1F], lbl_eu_80664798), 0);

    // Second position target (cursor-position pane) when the current floor
    // flag is set.
    if (cur->field_08 != 0) {
        char buf5[0x20];
        sprintf(buf5, &lbl_eu_8050BEA8[0x12A], cur->field_09 + 1);
        nw4r::lyt::Pane* pane2 = cur->mData->GetRootPane()->FindPaneByName(buf5, 1);
        nw4r::math::VEC3 pos2;
        func_801375A0(&pos2, pane2);
        nw4r::lyt::Pane* scalePane2 =
            cur->mData->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);
        pos2.x = pos2.x * scalePane2->GetScale().x;
        if (cur->field_30F0)
            CFloorMapSetSlotPos((CFloorMapLayoutHolder*)cur->field_30F0, *(nw4r::math::_VEC3*)&pos2);
    }

    if (lbl_eu_80664798 != lbl_eu_80664184)
        cur->field_3101 = 0;

    // Flag the panes when the current map is one of the special tutorial-map
    // ids (0x13-0x15, 0x17-0x18, 0x1B).
    int flag = 0;
    if ((u32)(lbl_eu_80664798 - 0x13) <= 2 || (u32)(lbl_eu_80664798 - 0x17) <= 1 ||
        lbl_eu_80664798 == 0x1B)
        flag = 1;

    nw4r::lyt::Pane* paneA =
        cur->mData->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x16B], 1);
    CFloorMapPaneFlag* pfa = (CFloorMapPaneFlag*)paneA;
    pfa->mFlag = (pfa->mFlag & 0x7F) | !flag;
    nw4r::lyt::Pane* paneB =
        cur->mData->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x175], 1);
    CFloorMapPaneFlag* pfb = (CFloorMapPaneFlag*)paneB;
    pfb->mFlag = (pfb->mFlag & 0x7F) | flag;
}

// Rebuild the floor-map marker lists: read the marker table from the current
// file (filtered by map-id/type/region checks), then merge each accepted
// marker into the highest floor whose height is above the marker, and finally
// refresh the cursor pane position and scrollbar.
void func_80245DF8(void* self) {
    // Locals in retail frame-slot order; the s16->f32 conversions below use
    // plain casts - MWCC materializes its own stack temps and reuses the
    // TU-defined lbl_eu_80668770 magic for them.
    f32 vec[3];
    nw4r::math::VEC3 pos;
    nw4r::math::_VEC3 dest;
    char buf[0x20];
    CFloorMapMarkerEntry entries[32];

    func_8003AA34();

    u32 fp = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x17F]);
    u16 count = (u16)func_8003B1EC(fp);

    u16 n = 0;
    for (u16 i = 1; i <= count; i++) {
        if (func_8009CF8C(0x20C8 + i) == 0) continue;
        if (lbl_eu_80664798 != (u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x18C], i)) continue;
        if ((u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x192], i) == 2) continue;

        CFloorMapMarkerEntry* e = &entries[n];
        e->id = i;
        n++;
        e->type = func_801361E8(fp, &lbl_eu_8050BEA8[0x192], i);
        e->x = (f32)func_80136330(fp, &lbl_eu_8050BEA8[0x19B], i);
        e->y = (f32)func_80136330(fp, &lbl_eu_8050BEA8[0x1A0], i);
        e->z = (f32)func_80136330(fp, &lbl_eu_8050BEA8[0x1A5], i);
        e->unk = (u32)func_8013639C((const void*)fp, (const void*)&lbl_eu_8050BEA8[0x1F], i);

        // Marker is always shown for entry 0 or when either global gimmick
        // flag is set; otherwise only while the current scroll position lies
        // inside the marker's [left,right] range.
        u8 flag;
        if (i == 0) {
            flag = 1;
        } else if (getUnk80664658()->field_214 & 0x48000) {
            flag = 1;
        } else {
            u16 left = func_80136254((const void*)fp, (const void*)&lbl_eu_8050BEA8[0x1AA], i);
            u16 right = func_80136254((const void*)fp, (const void*)&lbl_eu_8050BEA8[0x1B5], i);
            u16 scrollPos = (u16)func_8009CF8C(0x20);
            flag = (left > scrollPos || scrollPos > right) ? 0 : 1;
        }
        e->flag = flag;
    }

    // Merge: walk the floors from the top down; the first floor whose height
    // is above the marker's y receives the marker appended to its list.
    CFloorMapCursor* cur = (CFloorMapCursor*)self;
    for (u8 j = 0; j < n; j++) {
        CFloorMapMarkerEntry* e = &entries[j];
        f32 y = e->y;
        for (s8 k = (s8)(cur->field_30D0 - 1); k >= 0; k--) {
            CFloorMapMarkerBlock* blk =
                (CFloorMapMarkerBlock*)((u8*)cur + k * 0x30C + 0x10);
            if (!((f32)blk->height > y)) continue;

            CFloorMapMarkerEntry* rec = &blk->records[blk->count];
            blk->count++;
            rec->id = e->id;
            rec->type = e->type;
            rec->x = e->x;
            rec->y = e->y;
            rec->z = e->z;
            rec->unk = e->unk;
            rec->flag = e->flag;
            break;
        }
    }

    func_80246200(cur);

    // Refresh the cursor position: format the pane name for the current page
    // index, scale its translate by the zoom pane, and move the target pane.
    sprintf(buf, &lbl_eu_8050BEA8[0x14D], (s8)cur->field_0A);

    nw4r::lyt::Pane* pane = cur->mData->GetRootPane()->FindPaneByName(buf, 1);
    func_801375A0(&pos, pane);

    nw4r::lyt::Pane* scalePane =
        cur->mData->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);
    pos.x *= scalePane->GetScale().x;

    dest = *(nw4r::math::_VEC3*)&pos;
    if (cur->field_3108) {
        CFloorMapCursorTarget* target = (CFloorMapCursorTarget*)cur->field_3108;
        target->pane->SetTranslate(*(nw4r::math::VEC3*)&dest);
    }

    if (cur->field_3134) {
        vec[0] = lbl_eu_80668768;
        vec[1] = lbl_eu_8066876C;
        vec[2] = lbl_eu_80668764;
        func_801F3670(cur->field_3134, vec);
        CFloorMapFloorEntry* entries2 = (CFloorMapFloorEntry*)cur->_0C;
        func_801F36BC(cur->field_3134, 5,
                      reinterpret_cast<CFloorMapFloorEntryView*>(&entries2[cur->field_09])->count);
        func_801F3850(cur->field_3134, 0);
    }
    func_801F367C(cur->field_3134);
}

// Stub for the floor-map cursor refresh (retail 0x802483B8, not a target).
// Declared in CFloorMap.hpp (extern "C") but deliberately NOT defined here:
// a same-TU empty definition lets MWCC prove the call is a no-op and elide
// it, dropping the retail `bl` from every caller. The undefined extern
// resolves to the retail address via config/<region>/symbols.txt at link
// time (same mechanism as the func_80136E84 family).

// Deleting dtors (retail names): free self when mode > 0, return self.
extern "C" void* __dt__80244724(void* self, int mode) {
    extern void* __dl__FPv(void*);
    if (self && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

extern "C" void* __dt__8024503C(void* self, int mode) {
    extern void* __dl__FPv(void*);
    if (self && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

void* __dt__802462F0(void* self, int mode) {
    extern void* __dl__FPv(void*);
    if (self && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

// Rebuild the 20-slot layout block at CFloorMap+0x150: load every floor-row
// layout, then walk the current map file's marker table. Each accepted marker
// updates its row pane's text/color (dimmed while outside the scroll range
// unless the region gimmick flag is set), projects the player position onto
// that row's map plane, and lights exactly one of the three status panes
// based on the measured text height.
void func_80246330(CFloorMapLayoutBlock* self) {
    char* tbl = lbl_eu_8050BEA8;

    // Stack locals declared in reverse of retail frame-slot order
    // (convB@0xA8, convA@0xA0, utf16@0x60, buf@0x40, scalars@0x28-0x3C,
    // pos@0x1C, snap@0x10, out@0x08).
    CFloorMapF64Conv convB;
    CFloorMapF64Conv convA;
    u16 utf16[0x10];
    char buf[0x20];
    f32 ry;
    f32 rx;
    f32 fvy;
    f32 fvx;
    nw4r::math::VEC3 pos;
    nw4r::math::VEC3 out;

    // s16/u8 -> f32 conversion slots; retail stores both 0x4330 prefix words
    // before the layout-init loop runs.
    convA.w[0] = 0x43300000;
    convB.w[0] = 0x43300000;

    // Dimmed-color base kept in a register across the whole walk (retail
    // materializes lis r30, 0xC850 once and adds 0x50FF per row).
    u32 colorHi = 0xC8500000;

    // Dedicated base pointer for the UTF-16 conversion buffer (retail keeps
    // it in r31 across every call).
    u16* utf16p = utf16;

    // Load all 20 layouts and bind each row pane's device font.
    for (u32 i = 0; i < 20; i++) {
        self->slots[i].layout = NULL;
        self->slots[i].posY = 0;
        func_80136E84(&self->slots[i].layout, self->accessor, tbl + 0x1C1);
        nw4r::lyt::Layout* lay = self->slots[i].layout;
        CFloorMapFontView* font =
            (CFloorMapFontView*)func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, lay);
        func_8013676C(lay->GetRootPane(), font->getFontHandle());
    }

    func_8003AA34();
    u32 fp = (u32)getFP__FPCc(&tbl[0x17F]);
    u32 count = func_8003B1EC(fp);

    self->count = 0;

    // Constants kept live across the whole marker walk (retail f25-f31).
    const f64 convBias = lbl_eu_80668770;
    const f32 zero = lbl_eu_80668764;
    const f32 regionBase = lbl_eu_80668788;
    const f32 regionScale = lbl_eu_80668778;
    const f32 projScale = lbl_eu_8066877C;
    const f32 thrHi = lbl_eu_80668780;
    const f32 thrLo = lbl_eu_80668784;

    for (u32 i = 1; i <= count; i++) {
        if (func_8009CF8C(i + 0x20C8) == 0)
            continue;
        if (lbl_eu_80664798 != func_801361E8(fp, tbl + 0x18C, i))
            continue;
        if (func_801361E8(fp, tbl + 0x192, i) == 2)
            continue;

        u8 n = self->count;
        self->count = n + 1;
        CFloorMapLayoutSlot* e = &self->slots[n];

        // Row world position: y is truncated into the slot, x/z stay live for
        // the projection below.
        nw4r::math::VEC3 pos;
        func_80141DC4(&pos.x);
        e->posY = (s32)pos.y;

        char* name = func_8013639C((const void*)fp, tbl + 0x1F, i);
        func_80136B4C(e->layout, tbl + 0x1D7, name, 0);

        // Dimmed color when the row is not yet visible on the scrollbar but
        // the region gimmick flag is set.
        u32 color = 0xFFFFFFFF;
        int vis;
        if (i == 0) {
            vis = 1;
        } else if (getUnk80664658()->field_214 & 0x12000) {
            vis = 1;
        } else {
            u16 left = func_80136254((const void*)fp, tbl + 0x1AA, i);
            u16 right = func_80136254((const void*)fp, tbl + 0x1B5, i);
            u16 scrollPos = (u16)func_8009CF8C(0x20);
            vis = (left > scrollPos || scrollPos > right) ? 0 : 1;
        }
        if (vis || func_8009CF8C(0x3212) != 0)
            color = colorHi + 0x50FF;
        func_80137B44(e->layout, tbl + 0x1D7, color);

        // Project this row's scroll range onto the map plane for the current
        // region.
        s16 vx = func_80136330(lbl_eu_806640A8, tbl + 0x1E2, lbl_eu_80664798);
        s16 vy = func_80136330(lbl_eu_806640A8, tbl + 0x1F0, lbl_eu_80664798);
        convA.w[1] = (u32)(s16)vx ^ 0x80000000;
        fvx = (f32)(convA.d - convBias);
        convB.w[1] = (u32)(s16)vy ^ 0x80000000;
        fvy = (f32)(convB.d - convBias);

        u8 region = func_801361E8(lbl_eu_806640A8, tbl + 0x1FE, lbl_eu_80664798);
        convA.w[1] = region;
        f32 denom = ((f32)(convA.d - regionBase)) * regionScale;

        rx = fvx + (pos.x / denom) * projScale;
        ry = fvy - (pos.z / denom) * projScale;

        // Per-row pixel offsets from the map table nudge the projected point.
        convB.w[1] = (u32)(s8)func_801362C0((const void*)fp, tbl + 0x20B, i) ^ 0x80000000;
        ry = ry + (f32)(convB.d - convBias);
        convA.w[1] = (u32)(s8)func_801362C0((const void*)fp, tbl + 0x213, i) ^ 0x80000000;
        rx = rx + (f32)(convA.d - convBias);

        nw4r::lyt::Pane* mark = e->layout->GetRootPane()->FindPaneByName(tbl + 0x21B, 1);
        CFloorMapPaneTransXYZ* mt = (CFloorMapPaneTransXYZ*)mark;
        mt->trans2C = ry;
        mt->trans30 = rx;
        mt->trans34 = zero;

        // Convert the marker name to UTF-16 for the text measurement helper.
        sprintf(buf, tbl + 0x223, name);
        func_80136400(buf, utf16p, 0x20);

        u32 len = 0;
        while (utf16p[len] != 0)
            len++;

        nw4r::lyt::Pane* textBox = e->layout->GetRootPane()->FindPaneByName(tbl + 0x1D7, 1);
        func_80125D00((f32*)&out, textBox, utf16p);

        // Clear all three status-pane visible bits, then light the one whose
        // height band contains the measured text position.
        nw4r::lyt::Pane* pA = e->layout->GetRootPane()->FindPaneByName(tbl + 0x226, 1);
        ((CFloorMapPaneFlag*)pA)->mFlag &= 0xFE;
        nw4r::lyt::Pane* pB = e->layout->GetRootPane()->FindPaneByName(tbl + 0x233, 1);
        ((CFloorMapPaneFlag*)pB)->mFlag &= 0xFE;
        nw4r::lyt::Pane* pC = e->layout->GetRootPane()->FindPaneByName(tbl + 0x240, 1);
        ((CFloorMapPaneFlag*)pC)->mFlag &= 0xFE;

        f32 z = out.x;
        if (z >= thrHi) {
            ((CFloorMapPaneFlag*)pA)->mFlag |= 1;
        } else if (z >= thrLo) {
            ((CFloorMapPaneFlag*)pB)->mFlag |= 1;
        } else {
            ((CFloorMapPaneFlag*)pC)->mFlag |= 1;
        }
    }
}

void* __dt__802468C8(void* self, int mode) {
    extern void* __dl__FPv(void*);
    if (self && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

// ---------------------------------------------------------------------------
// func_80246908 - release every marker textbox/picture attached to the map.
// For each named holder pane under slots->pane (the holder is looked up by
// name only when the string-table entry is non-NULL), detach and delete the
// matching marker pane found by child-name under the main layout root;
// afterwards walk the holder pane's direct children, drop the one extra named
// pane, and finally release the map layout itself.
// ---------------------------------------------------------------------------
#define FLOORMAP_RELEASE_MARKERS(groupPane)                                    \
    do {                                                                       \
        CFloorMapHolderChildren* kids;                                         \
        if ((groupPane) != NULL)                                               \
            kids = (CFloorMapHolderChildren*)((u8*)(groupPane) + 0x10);        \
        else                                                                   \
            kids = (CFloorMapHolderChildren*)((u8*)slots->pane + 0x10);        \
        CFloorMapChildNode* node = kids->first;                                \
        CFloorMapChildNode* end = (CFloorMapChildNode*)&kids->first;           \
        while (node != end) {                                                  \
            if (node == NULL)                                                  \
                Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052CB40,       \
                                           0x23D,                              \
                                           (const char*)lbl_eu_8052CB1C);      \
            CFloorMapChildNode* next = node->mpNext;                           \
            nw4r::lyt::Pane* marker =                                          \
                ((nw4r::lyt::Layout*)slots->layout)                            \
                    ->GetRootPane()                                            \
                    ->FindPaneByName(node->name, 1);                           \
            if (marker != NULL && marker->GetParent() != NULL) {               \
                marker->GetParent()->RemoveChild(marker);                      \
                ((CFloorMapLayoutDtorVt*)marker)->destroy(-1);                 \
                ((CLibLayout*)marker)->deleteTextboxOrPicture();               \
            }                                                                  \
            node = next;                                                       \
        }                                                                      \
    } while (0)

void func_80246908(void* self) {
    CFloorMapLayoutSlots* slots = (CFloorMapLayoutSlots*)self;

    do {
        nw4r::lyt::Pane* group = NULL;
        if (&lbl_eu_8050BEA8[0x26C] != NULL) {
            group = slots->pane->FindPaneByName(&lbl_eu_8050BEA8[0x26C], true);
            if (group == NULL)
                break;
        }
        FLOORMAP_RELEASE_MARKERS((group != NULL) ? group : slots->pane);
    } while (0);

    do {
        nw4r::lyt::Pane* group = NULL;
        if (&lbl_eu_8050BEA8[0x275] != NULL) {
            group = slots->pane->FindPaneByName(&lbl_eu_8050BEA8[0x275], true);
            if (group == NULL)
                break;
        }
        FLOORMAP_RELEASE_MARKERS((group != NULL) ? group : slots->pane);
    } while (0);

    do {
        nw4r::lyt::Pane* group = NULL;
        if (&lbl_eu_8050BEA8[0x281] != NULL) {
            group = slots->pane->FindPaneByName(&lbl_eu_8050BEA8[0x281], true);
            if (group == NULL)
                break;
        }
        FLOORMAP_RELEASE_MARKERS((group != NULL) ? group : slots->pane);
    } while (0);

    do {
        nw4r::lyt::Pane* group = NULL;
        if (&lbl_eu_8050BEA8[0x28A] != NULL) {
            group = slots->pane->FindPaneByName(&lbl_eu_8050BEA8[0x28A], true);
            if (group == NULL)
                break;
        }
        FLOORMAP_RELEASE_MARKERS((group != NULL) ? group : slots->pane);
    } while (0);

    do {
        nw4r::lyt::Pane* group = NULL;
        if (&lbl_eu_8050BEA8[0x293] != NULL) {
            group = slots->pane->FindPaneByName(&lbl_eu_8050BEA8[0x293], true);
            if (group == NULL)
                break;
        }
        FLOORMAP_RELEASE_MARKERS((group != NULL) ? group : slots->pane);
    } while (0);

    do {
        nw4r::lyt::Pane* group = NULL;
        if (&lbl_eu_8050BEA8[0x2AB] != NULL) {
            group = slots->pane->FindPaneByName(&lbl_eu_8050BEA8[0x2AB], true);
            if (group == NULL)
                break;
        }
        FLOORMAP_RELEASE_MARKERS((group != NULL) ? group : slots->pane);
    } while (0);

    do {
        nw4r::lyt::Pane* group = NULL;
        if (&lbl_eu_8050BEA8[0x2B4] != NULL) {
            group = slots->pane->FindPaneByName(&lbl_eu_8050BEA8[0x2B4], true);
            if (group == NULL)
                break;
        }
        FLOORMAP_RELEASE_MARKERS((group != NULL) ? group : slots->pane);
    } while (0);

    do {
        nw4r::lyt::Pane* group = NULL;
        if (&lbl_eu_8050BEA8[0x2A0] != NULL) {
            group = slots->pane->FindPaneByName(&lbl_eu_8050BEA8[0x2A0], true);
            if (group == NULL)
                break;
        }
        FLOORMAP_RELEASE_MARKERS((group != NULL) ? group : slots->pane);
    } while (0);

    do {
        nw4r::lyt::Pane* group = NULL;
        if (&lbl_eu_8050BEA8[0x2BD] != NULL) {
            group = slots->pane->FindPaneByName(&lbl_eu_8050BEA8[0x2BD], true);
            if (group == NULL)
                break;
        }
        FLOORMAP_RELEASE_MARKERS((group != NULL) ? group : slots->pane);
    } while (0);

    do {
        nw4r::lyt::Pane* group = NULL;
        if (&lbl_eu_8050BEA8[0x2C7] != NULL) {
            group = slots->pane->FindPaneByName(&lbl_eu_8050BEA8[0x2C7], true);
            if (group == NULL)
                break;
        }
        FLOORMAP_RELEASE_MARKERS((group != NULL) ? group : slots->pane);
    } while (0);

    do {
        nw4r::lyt::Pane* group = NULL;
        if (&lbl_eu_8050BEA8[0x2CF] != NULL) {
            group = slots->pane->FindPaneByName(&lbl_eu_8050BEA8[0x2CF], true);
            if (group == NULL)
                break;
        }
        FLOORMAP_RELEASE_MARKERS((group != NULL) ? group : slots->pane);
    } while (0);

    // Direct walk of the holder pane's own children (row-marker panes; no
    // holder lookup here).
    {
        CFloorMapChildNode* node =
            *(CFloorMapChildNode**)((u8*)slots->pane + 0x14);
        CFloorMapChildNode* end =
            (CFloorMapChildNode*)((u8*)slots->pane + 0x14);
        while (node != end) {
            if (node == NULL)
                Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052CB40, 0x23D,
                                           (const char*)lbl_eu_8052CB1C);
            CFloorMapChildNode* next = node->mpNext;
            nw4r::lyt::Pane* marker =
                ((nw4r::lyt::Layout*)slots->layout)
                    ->GetRootPane()
                    ->FindPaneByName(node->name, 1);
            if (marker != NULL && marker->GetParent() != NULL) {
                marker->GetParent()->RemoveChild(marker);
                ((CFloorMapLayoutDtorVt*)marker)->destroy(-1);
                ((CLibLayout*)marker)->deleteTextboxOrPicture();
            }
            node = next;
        }
    }

    // One extra named pane outside the holder groups.
    nw4r::lyt::Pane* extra =
        ((nw4r::lyt::Layout*)slots->layout)
            ->GetRootPane()
            ->FindPaneByName(&lbl_eu_8050BEA8[0x263], 1);
    if (extra != NULL && extra->GetParent() != NULL) {
        extra->GetParent()->RemoveChild(extra);
        ((CFloorMapLayoutDtorVt*)extra)->destroy(-1);
        ((CLibLayout*)extra)->deleteTextboxOrPicture();
    }

    // Release the map layout itself (double null check is the retail
    // deleting-dtor shape).
    if (slots->layout != 0) {
        if (slots->layout != 0)
            ((CFloorMapLayoutDtorVt*)slots->layout)->destroy(1);
        slots->layout = 0;
    }
}

// Per-group marker release (inlined at each call site): resolve the holder
// pane by name, then detach/delete every marker pane in its child list
// (link node at pane+0x14).
#define FLOORMAP_RELEASE_GROUP(slots_, name_, label_)                          \
    {                                                                          \
        nw4r::lyt::Pane* group = NULL;                                         \
        if ((name_) != NULL) {                                                 \
            group = (slots_)->pane->FindPaneByName((name_), true);             \
            if (group == NULL)                                                 \
                goto label_;                                                   \
        }                                                                      \
        nw4r::lyt::Pane* holder = (group != NULL) ? group : (slots_)->pane;    \
        CFloorMapGroupAnchor* anchor =                                         \
            (CFloorMapGroupAnchor*)((u8*)holder + 0x10);                       \
        CFloorMapChildNode* node = anchor->kids.mpNext;                        \
        CFloorMapChildNode* end = (CFloorMapChildNode*)&anchor->kids;                   \
        while (node != end) {                                                  \
            if (node == NULL)                                                  \
                Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052CB40,       \
                                           0x23D,                              \
                                           (const char*)lbl_eu_8052CB1C);      \
            CFloorMapChildNode* next = node->mpNext;                           \
            nw4r::lyt::Pane* marker =                                          \
                ((nw4r::lyt::Layout*)(slots_)->layout)                         \
                    ->GetRootPane()                                            \
                    ->FindPaneByName(node->name, 1);                           \
            if (marker != NULL && marker->GetParent() != NULL) {               \
                marker->GetParent()->RemoveChild(marker);                      \
                ((CFloorMapLayoutDtorVt*)marker)->destroy(-1);                 \
                ((CLibLayout*)marker)->deleteTextboxOrPicture();               \
            }                                                                  \
            node = next;                                                       \
        }                                                                      \
    }

// Release the row-marker panes attached to each of the 12 marker-holder
// groups (tbl offsets below), then rebuild every per-marker pane flag: for
// idx 1..0x240 the pane named tbl+0x2DE gets its visible bit set when the
// map-table lookup func_8013AC3C reports a valid (< 0x10000) result, and
// finally all placement/refresh passes are re-run.
void func_80247490(void* self, u8 arg2, u32 arg3, f32 arg4) {
    CFloorMapLayoutSlots* slots = (CFloorMapLayoutSlots*)self;
    slots->field_0C = arg2;
    slots->field_0C = arg2;

    FLOORMAP_RELEASE_GROUP(slots, &lbl_eu_8050BEA8[0x26C], next01)
next01:
    FLOORMAP_RELEASE_GROUP(slots, &lbl_eu_8050BEA8[0x275], next02)
next02:
    FLOORMAP_RELEASE_GROUP(slots, &lbl_eu_8050BEA8[0x281], next03)
next03:
    FLOORMAP_RELEASE_GROUP(slots, &lbl_eu_8050BEA8[0x28A], next04)
next04:
    FLOORMAP_RELEASE_GROUP(slots, &lbl_eu_8050BEA8[0x293], next05)
next05:
    FLOORMAP_RELEASE_GROUP(slots, &lbl_eu_8050BEA8[0x2AB], next06)
next06:
    FLOORMAP_RELEASE_GROUP(slots, &lbl_eu_8050BEA8[0x2B4], next07)
next07:
    FLOORMAP_RELEASE_GROUP(slots, &lbl_eu_8050BEA8[0x2A0], next08)
next08:
    FLOORMAP_RELEASE_GROUP(slots, &lbl_eu_8050BEA8[0x2BD], next09)
next09:
    FLOORMAP_RELEASE_GROUP(slots, &lbl_eu_8050BEA8[0x2D6], next10)
next10:
    FLOORMAP_RELEASE_GROUP(slots, &lbl_eu_8050BEA8[0x2C7], next11)
next11:
    FLOORMAP_RELEASE_GROUP(slots, &lbl_eu_8050BEA8[0x2CF], next12)
next12:

    // Rebuild each marker pane's visible bit from the map table lookup.
    // Retail stores bit0 as bit 4 of cntlzw(result), i.e. result < 0x10000.
    for (u32 idx = 1; idx <= 0x240; idx++) {
        int found = func_8013AC3C(lbl_eu_80664798, slots->field_0C, (u16)idx);
        char buf[0x20];
        sprintf(buf, &lbl_eu_8050BEA8[0x2DE], (u16)idx);
        nw4r::lyt::Pane* pane =
            ((nw4r::lyt::Layout*)slots->layout)->GetRootPane()->FindPaneByName(buf, 1);
        CFloorMapPaneFlag* pf = (CFloorMapPaneFlag*)pane;
        pf->mFlag = (pf->mFlag & 0xFE) | ((__cntlzw((u32)found) >> 4) & 1);
    }

    func_8024B234((CFloorMapFull*)self);
    func_8024AEEC(self);
    func_8024A748(self);
    func_8024A448(self);
    func_80249C1C(slots);
    func_802497B0(self);
    func_80249344(slots);
    func_80248ED8(slots);
    func_80248A6C(slots);
}

// Place a map marker for floor row `idx`: after confirming the row belongs
// to the current map and its unlock flag is set, find the first floor whose
// height is above the row's y (and which is the current map row), project the
// position onto the map pane coordinates, build a uniquely-named pane from the
// projected coordinates, create the picture pane and append it to the map root.
void func_8024808C(CFloorMapFull* self, u32 idx) {
    // Locals in retail frame-slot order (tmp@0x08, work@0x14, fy/fx@0x20/0x24,
    // pos@0x2C, name buf@0x38, conv slots@0x58/0x60).
    nw4r::math::VEC3 tmp;
    nw4r::math::VEC3 work;
    f32 fy;
    f32 fx;
    nw4r::math::VEC3 pos;
    char buf[0x20];
    CFloorMapF64Conv convA;
    CFloorMapF64Conv convB;
    // Seeded before any early-out; MWCC sinks these stores into the prologue.
    convA.w[0] = 0x43300000;
    convB.w[0] = 0x43300000;

    func_8003AA34();
    u32 fp = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x17F]);
    if (lbl_eu_80664798 != (u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x18C], idx)) return;
    if (func_8009CF8C(idx + 0x20C8) == 0) return;

    func_80141DC4(&tmp.x);
    pos = tmp;
    f32 py = pos.y;

    u8 count = (u8)func_8003B1EC(lbl_eu_8066479C);
    u8 found = 0;
    for (u8 i = 1; i <= count; i++) {
        s16 h = func_80136330(lbl_eu_8066479C, &lbl_eu_8050BEA8[0x15A], i);
        convA.w[1] = (u32)(s16)h ^ 0x80000000;
        if ((f32)(convA.d - lbl_eu_80668770) > py) {
            if (i == self->field_0C) found = 1;
            break;
        }
    }
    if (!found) return;

    // Project onto the map pane coordinate system for the current world.
    s16 vx = func_80136330(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1E2], lbl_eu_80664798);
    s16 vy = func_80136330(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1F0], lbl_eu_80664798);
    convA.w[1] = (u32)(s16)vy ^ 0x80000000;
    fy = (f32)(convA.d - lbl_eu_80668770);
    convB.w[1] = (u32)(s16)vx ^ 0x80000000;
    fx = (f32)(convB.d - lbl_eu_80668770);
    u8 region = (u8)func_801361E8(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1FE], lbl_eu_80664798);
    convB.w[1] = region;
    f32 denom = ((f32)(convB.d - lbl_eu_80668788)) * lbl_eu_80668778;

    work = pos;
    work.x = fy + (work.x / denom) * lbl_eu_8066877C;
    work.y = fx - (work.z / denom) * lbl_eu_8066877C;
    work.z = lbl_eu_80668764;

    sprintf(buf, &lbl_eu_8050BEA8[0x2EB], fx, work.x);

    void* pic = func_80248920(self, (const char*)0xE, work.x, work.y, (void*)(-1), buf);
    nw4r::lyt::Pane* pane = self->field_08->FindPaneByName(&lbl_eu_8050BEA8[0x26C], true);
    pane->AppendChild((nw4r::lyt::Pane*)pic);
}

void func_8024830C(void* self, void* arg2) {
    f32* result = (f32*)self;
    u32* slotFlag = (u32*)arg2;

    f32 zero = lbl_eu_80668764;
    result[0] = zero;
    result[1] = zero;
    result[2] = zero;

    if (*slotFlag == 0)
        goto done;

    if ((lbl_eu_80664184 & 0xFF) == lbl_eu_80664798) {
        // Current world: take the live player position.
        void* player = cf::CfGameManager::getPlayer(0);
        if (player == NULL)
            goto done;
        CFloorMapVec3* pos = ((CFloorMapPlayerObj*)player)->GetPos();
        result[0] = pos->x;
        result[1] = pos->y;
        result[2] = pos->z;
    } else {
        // Other world: use this map's spawn point, when one exists.
        u16 idx = lbl_eu_8050B798[lbl_eu_80664798];
        if (idx != 0) {
            f32 buf[3];
            func_80141DC4(buf);
            result[0] = buf[0];
            result[1] = buf[1];
            result[2] = buf[2];
        }
    }

    s16 val1 = func_80136330(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1E2], lbl_eu_80664798);
    s16 val2 = func_80136330(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1F0], lbl_eu_80664798);

    // s16 -> f32 conversions via the shared 0x4330 double-magic idiom; the
    // projected values are kept in an array local so they spill to the stack
    // across the region lookup, as in retail.
    CFloorMapF64Conv convA;
    CFloorMapF64Conv convB;
    f32 vals[3];
    convA.w[0] = 0x43300000;
    convB.w[0] = 0x43300000;
    convA.w[1] = (u32)(s16)val1 ^ 0x80000000;
    vals[0] = (f32)(convA.d - lbl_eu_80668770);
    convB.w[1] = (u32)(s16)val2 ^ 0x80000000;
    vals[1] = (f32)(convB.d - lbl_eu_80668770);
    vals[2] = lbl_eu_80668764;

    u8 region = (u8)func_801361E8(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1FE], lbl_eu_80664798);
    convA.w[0] = 0x43300000;
    convA.w[1] = region;
    f32 denom = ((f32)(convA.d - lbl_eu_80668788)) * lbl_eu_80668778;

    // Word-wise snapshot of the current position so the divisions read the
    // pre-projection values like retail.
    nw4r::math::VEC3 work = *(nw4r::math::VEC3*)result;
    result[0] = vals[0] + (work.x / denom) * lbl_eu_8066877C;
    result[1] = vals[1] - (work.z / denom) * lbl_eu_8066877C;

    nw4r::lyt::Pane* found =
        ((nw4r::lyt::Pane*)*(u32*)((u8*)*(u32**)slotFlag + 0x10))
            ->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);
    result[0] *= found->GetScale().x;
    *(nw4r::math::VEC3*)result +=
        ((nw4r::lyt::Pane*)*(u32*)((u8*)*(u32**)slotFlag + 0x10))->GetTranslate();
done:;
}

// Find the map row nearest the current map position: walk the row-marker
// panes (children of the pane found by the 0x26C name, linked through their
// raw ut::LinkListNode fields), accumulate each marker pane's parent-chain
// translate up to the layout root, and pick the first pane inside the hit
// radius. The trailing digits of that pane's name are the row id.
u32 func_80248558(void* self) {
    CFloorMapLayoutSlots* slots = (CFloorMapLayoutSlots*)self;

    // Declared in retail frame-slot order (buf2@0x48, out@0x3c, rec@0x30,
    // acc3@0x24, acc2@0x18, acc@0xc, digits@0x8).
    char buf2[0x40];
    nw4r::math::VEC3 out;
    nw4r::math::VEC3 rec;
    nw4r::math::VEC3 acc3;
    nw4r::math::VEC3 acc2;
    nw4r::math::VEC3 acc;
    char digits[4];

    nw4r::lyt::Pane* pane = slots->pane->FindPaneByName(&lbl_eu_8050BEA8[0x26C], 1);

    CFloorMapChildNode* node = *(CFloorMapChildNode**)((u8*)pane + 0x14);
    CFloorMapChildNode* end = (CFloorMapChildNode*)((u8*)pane + 0x14);

    // Cached float locals: retail keeps these in f31/f30 across every call
    // in the loop (hence the nonvolatile-float prologue).
    f32 zero = lbl_eu_80668764;
    f32 d = lbl_eu_80668790 + lbl_eu_80668790;
    f32 threshold = d * d;

    while (node != end) {
        if (node == NULL)
            Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052CB40, 0x23D, (const char*)lbl_eu_8052CB1C);

        nw4r::lyt::Pane* pane2 = slots->pane->FindPaneByName(node->name, 1);

        nw4r::lyt::Pane* target = slots->layout->GetRootPane();
        out.x = zero;
        out.y = zero;
        out.z = zero;
        if (target != pane2 && pane2 != NULL) {
            nw4r::lyt::Pane* parent = pane2->GetParent();
            out.x = pane2->GetTranslate().x;
            out.y = pane2->GetTranslate().y;
            out.z = pane2->GetTranslate().z;
            acc.x = zero;
            acc.y = zero;
            acc.z = zero;
            // Shared add tails: when a level's check fails the remaining
            // additions still run over zeroed accumulators (as in retail).
            if (target == parent || parent == NULL)
                goto add_out;
            nw4r::lyt::Pane* gp = parent->GetParent();
            acc.x = parent->GetTranslate().x;
            acc.y = parent->GetTranslate().y;
            acc.z = parent->GetTranslate().z;
            acc2.x = zero;
            acc2.y = zero;
            acc2.z = zero;
            if (target == gp || gp == NULL)
                goto add_acc;
            nw4r::lyt::Pane* ggp = gp->GetParent();
            acc2.x = gp->GetTranslate().x;
            acc2.y = gp->GetTranslate().y;
            acc2.z = gp->GetTranslate().z;
            acc3.x = zero;
            acc3.y = zero;
            acc3.z = zero;
            if (target == ggp || ggp == NULL)
                goto add_acc2;
            acc3.x = ggp->GetTranslate().x;
            acc3.y = ggp->GetTranslate().y;
            acc3.z = ggp->GetTranslate().z;
            func_8024B4CC(&rec, self, ggp->GetParent());
            nw4r::math::VEC3Add(&acc3, &acc3, &rec);
        add_acc2:
            nw4r::math::VEC3Add(&acc2, &acc2, &acc3);
        add_acc:
            nw4r::math::VEC3Add(&acc, &acc, &acc2);
        add_out:
            nw4r::math::VEC3Add(&out, &out, &acc);
        }

        // Scale the accumulated position into map coordinates using the
        // zoom pane: x is multiplied by its scale, then the pane's own
        // translate (including z) is added on top.
        nw4r::lyt::Pane* scalePane =
            slots->layout->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);
        out.x *= scalePane->GetScale().x;
        nw4r::math::VEC3Add(&out, &out,
                            (nw4r::math::VEC3*)&scalePane->GetTranslate());

        f32 dy = -out.y;
        f32 dx = -out.x;
        f32 dist2 = dy * dy + dx * dx;
        if (dist2 <= threshold) {
            if (node == NULL)
                Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052CB40, 0x23D, (const char*)lbl_eu_8052CB1C);

            sprintf(buf2, &lbl_eu_8050BEA8[0x223], node->name);

            // Keep only the digits of the formatted name (max 4) and parse them.
            digits[0] = 0;
            digits[1] = 0;
            digits[2] = 0;
            digits[3] = 0;
            u8 outIdx = 0;
            for (u8 in = 0; buf2[in] != 0; in++) {
                u8 c = (u8)buf2[in];
                if (c >= '0' && c <= '9') {
                    digits[outIdx++] = (char)c;
                    if (outIdx >= 4) break;
                }
            }
            return (u16)atoi(digits);
        }
        node = node->mpNext;
    }
    return 0;
}

void* func_80248920(void* self, const char* name, f32 x, f32 y, void* arg5, const char* paneName) {
    if (!name) return NULL;
    if (!paneName) return NULL;

    void* result = (char*)func_80136190(&lbl_eu_8050BEA8[0x2f6], &lbl_eu_8050BEA8[0x303], (u32)name);

    char buf[20];
    sprintf(buf, &lbl_eu_8050BEA8[0x30e], result);

    void* accessor = func_801355F4();
    typedef void* (*VFuncPtr4)(void*, u32, void*, u32);
    VFuncPtr4* vt = *(VFuncPtr4**)accessor;
    void* picture = vt[3](accessor, 0x74696d67, buf, 0);

    if (!picture) return NULL;

    void* pic = createPicture__10CLibLayoutFv();
    SetName__Q34nw4r3lyt4PaneFPCc(pic, paneName);

    nw4r::math::_VEC3 vec = { x, y, lbl_eu_80668764 };

    *(float*)((u8*)pic + 0x2C) = x;
    *(float*)((u8*)pic + 0x30) = y;
    *(float*)((u8*)pic + 0x34) = lbl_eu_80668764;

    func_80137C1C(pic, arg5);

    u8* byte = (u8*)pic + 0xBB;
    *byte = (*byte & 0x7F) | 0x01;

    *(float*)((u8*)pic + 0x44) = lbl_eu_80668794;
    *(float*)((u8*)pic + 0x48) = lbl_eu_80668794;

    return pic;
}

// Variant of the marker-placement scan (same shape as func_802497B0) for a
// different map table section: picture panes keyed by tbl+0x382 names are
// appended under the tbl+0x275 row pane when every eligibility check passes.
void func_80248A6C(CFloorMapLayoutSlots* slots) {
    const char* tbl = lbl_eu_8050BEA8;

    // Two s16/u8 -> f32 conversion slots; retail stores their 0x4330 prefix
    // words from the prologue, before opening the map file.
    CFloorMapF64Conv convA;
    CFloorMapF64Conv convB;
    convA.w[0] = 0x43300000;
    convB.w[0] = 0x43300000;

    u32 fp = (u32)getFP__FPCc(&tbl[0x315]);
    int count = (int)func_8003B1EC(fp);

    // Constants hoisted before the walk so MWCC keeps them in f26-f31.
    const f64 convBias = lbl_eu_80668770;
    const f32 zero = lbl_eu_80668764;
    const f32 regionBase = lbl_eu_80668788;
    const f32 regionScale = lbl_eu_80668778;
    const f32 projScale = lbl_eu_8066877C;

    for (u16 i = 1; i <= count; i++) {
        if (func_801361E8(fp, &lbl_eu_8050BEA8[0x322], i) != lbl_eu_80664798) continue;

        u16 rnd = (u16)func_8009CF8C(0x20);
        u16 left = (u16)func_80136254((const void*)fp, &lbl_eu_8050BEA8[0x329], (int)i);
        if (rnd < left) continue;
        u16 right = (u16)func_80136254((const void*)fp, &lbl_eu_8050BEA8[0x32F], (int)i);
        if (right < rnd) continue;

        u8 c1 = (u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x335], i);
        u8 c2 = (u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x343], i);
        if (func_8009CF8C((u32)c1 + 0x7FC) < c2) continue;

        u8 c3 = (u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x34D], i);
        u8 c4 = (u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x35B], i);
        if (func_8009CF8C((u32)c3 + 0x7FC) < c4) continue;

        u8 v365 = (u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x365], i);
        if (v365 != 0 && func_8009CF8C((u32)v365 + 0x2578) == 0) continue;

        u16 v373 = (u16)func_80136254((const void*)fp, &lbl_eu_8050BEA8[0x373], (int)i);
        if (v373 != 0 && func_8009CF8C((u32)v373 + 0x220) != 0xFE) continue;

        s16 x = func_80136330(fp, &lbl_eu_8050BEA8[0x19B], i);
        s16 y = func_80136330(fp, &lbl_eu_8050BEA8[0x1A0], i);
        s16 z = func_80136330(fp, &lbl_eu_8050BEA8[0x1A5], i);

        convA.w[1] = (u32)(s16)x ^ 0x80000000;
        f32 fx = (f32)(convA.d - convBias);
        convB.w[1] = (u32)(s16)y ^ 0x80000000;
        f32 fy = (f32)(convB.d - convBias);
        convA.w[1] = (u32)(s16)z ^ 0x80000000;
        f32 fz = (f32)(convA.d - convBias);
        nw4r::math::_VEC3 pos = { fx, fy, fz };

        // Skip markers whose world y sits below the first map row taller than
        // it, unless that row is exactly the row currently displayed.
        u8 found = 0;
        u8 count2 = (u8)func_8003B1EC(lbl_eu_8066479C);
        for (u8 j = 1; j <= count2; j++) {
            s16 h = func_80136330(lbl_eu_8066479C, &lbl_eu_8050BEA8[0x15A], j);
            convB.w[1] = (u32)(s16)h ^ 0x80000000;
            if ((f32)(convB.d - convBias) > fy) {
                if (j == slots->field_0C) found = 1;
                break;
            }
        }
        if (!found) continue;

        if (func_801361E8(fp, &lbl_eu_8050BEA8[0x37D], i) != 5) continue;

        char buf[0x20];
        sprintf(buf, &lbl_eu_8050BEA8[0x382], i);
        if (slots->layout->GetRootPane()->FindPaneByName(buf, 1)) continue;

        s16 vx = func_80136330(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1E2], lbl_eu_80664798);
        s16 vy = func_80136330(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1F0], lbl_eu_80664798);

        convA.w[1] = (u32)(s16)vx ^ 0x80000000;
        f32 rvx = (f32)(convA.d - convBias);
        convB.w[1] = (u32)(s16)vy ^ 0x80000000;
        f32 rvy = (f32)(convB.d - convBias);
        f32 coords[3] = { rvx, rvy, zero };
        u8 region = (u8)func_801361E8(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1FE], lbl_eu_80664798);

        convA.w[1] = (u32)region;
        f32 scale = (f32)(convA.d - regionBase) * regionScale;
        nw4r::math::_VEC3 result = pos;
        result.y = coords[1] - (result.z / scale) * projScale;
        result.x = coords[0] + (result.x / scale) * projScale;
        result.z = zero;

        void* picture = func_80248920(slots, (const char*)0x3, result.x, result.y, (void*)-1, buf);

        nw4r::lyt::Pane* pane2 = slots->pane->FindPaneByName(&lbl_eu_8050BEA8[0x275], 1);
        pane2->AppendChild((nw4r::lyt::Pane*)picture);
    }
}

// Variant of the marker-placement scan (same shape as func_80248A6C) for a
// third map table section: picture panes keyed by tbl+0x390 names are appended
// under the tbl+0x281 row pane when every eligibility check passes.
void func_80248ED8(CFloorMapLayoutSlots* slots) {
    // Stack locals: MWCC assigns frame slots in reverse declaration order,
    // so declare highest-offset first (conv@0x50/0x58 ... result@0x08).
    const char* tbl = lbl_eu_8050BEA8;

    // Two s16/u8 -> f32 conversion slots; retail stores their 0x4330 prefix
    // words from the prologue, before opening the map file.
    CFloorMapF64Conv convA;
    CFloorMapF64Conv convB;
    char buf[0x20];
    nw4r::math::_VEC3 pos;
    f32 coords[3];
    nw4r::math::_VEC3 result;
    convA.w[0] = 0x43300000;
    convB.w[0] = 0x43300000;

    u32 fp = (u32)getFP__FPCc(&tbl[0x315]);
    int count = (int)func_8003B1EC(fp);

    // Constants hoisted before the walk so MWCC keeps them in f26-f31.
    const f64 convBias = lbl_eu_80668770;
    const f32 zero = lbl_eu_80668764;
    const f32 regionBase = lbl_eu_80668788;
    const f32 regionScale = lbl_eu_80668778;
    const f32 projScale = lbl_eu_8066877C;

    for (u16 i = 1; i <= count; i++) {
        if (func_801361E8(fp, &lbl_eu_8050BEA8[0x322], i) != lbl_eu_80664798) continue;

        u16 rnd = (u16)func_8009CF8C(0x20);
        u16 left = (u16)func_80136254((const void*)fp, &lbl_eu_8050BEA8[0x329], (int)i);
        if (rnd < left) continue;
        u16 right = (u16)func_80136254((const void*)fp, &lbl_eu_8050BEA8[0x32F], (int)i);
        if (right < rnd) continue;

        u8 c1 = (u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x335], i);
        u8 c2 = (u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x343], i);
        if (func_8009CF8C((u32)c1 + 0x7FC) < c2) continue;

        u8 c3 = (u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x34D], i);
        u8 c4 = (u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x35B], i);
        if (func_8009CF8C((u32)c3 + 0x7FC) < c4) continue;

        u8 v365 = (u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x365], i);
        if (v365 != 0 && func_8009CF8C((u32)v365 + 0x2578) == 0) continue;

        u16 v373 = (u16)func_80136254((const void*)fp, &lbl_eu_8050BEA8[0x373], (int)i);
        if (v373 != 0 && func_8009CF8C((u32)v373 + 0x220) != 0xFE) continue;

        s16 x = func_80136330(fp, &lbl_eu_8050BEA8[0x19B], i);
        s16 y = func_80136330(fp, &lbl_eu_8050BEA8[0x1A0], i);
        s16 z = func_80136330(fp, &lbl_eu_8050BEA8[0x1A5], i);

        // s16 -> f32 via the shared 0x4330 double-magic idiom.
        convA.w[1] = (u32)(s16)x ^ 0x80000000;
        pos.x = (f32)(convA.d - convBias);
        convB.w[1] = (u32)(s16)y ^ 0x80000000;
        pos.y = (f32)(convB.d - convBias);
        convA.w[1] = (u32)(s16)z ^ 0x80000000;
        pos.z = (f32)(convA.d - convBias);
        f32 fy = pos.y;

        // Skip markers whose world y sits below the first map row taller than
        // it, unless that row is exactly the row currently displayed.
        u8 found = 0;
        u8 count2 = (u8)func_8003B1EC(lbl_eu_8066479C);
        for (u8 j = 1; j <= count2; j++) {
            s16 h = func_80136330(lbl_eu_8066479C, &lbl_eu_8050BEA8[0x15A], j);
            convB.w[1] = (u32)(s16)h ^ 0x80000000;
            if ((f32)(convB.d - convBias) > fy) {
                if (j == slots->field_0C) found = 1;
                break;
            }
        }
        if (!found) continue;

        if (func_801361E8(fp, &lbl_eu_8050BEA8[0x37D], i) != 3) continue;

        sprintf(buf, &lbl_eu_8050BEA8[0x390], i);
        if (slots->layout->GetRootPane()->FindPaneByName(buf, 1)) continue;

        s16 vx = func_80136330(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1E2], lbl_eu_80664798);
        s16 vy = func_80136330(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1F0], lbl_eu_80664798);

        coords[2] = zero;
        convA.w[1] = (u32)(s16)vx ^ 0x80000000;
        coords[0] = (f32)(convA.d - convBias);
        convB.w[1] = (u32)(s16)vy ^ 0x80000000;
        coords[1] = (f32)(convB.d - convBias);
        u8 region = (u8)func_801361E8(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1FE], lbl_eu_80664798);

        convA.w[1] = (u32)region;
        f32 scale = (f32)(convA.d - regionBase) * regionScale;
        result = pos;
        result.y = coords[1] - (result.z / scale) * projScale;
        result.x = coords[0] + (result.x / scale) * projScale;
        result.z = zero;

        void* picture = func_80248920(slots, (const char*)0x10, result.x, result.y, (void*)-1, buf);

        nw4r::lyt::Pane* pane2 = slots->pane->FindPaneByName(&lbl_eu_8050BEA8[0x281], 1);
        pane2->AppendChild((nw4r::lyt::Pane*)picture);
    }
}

// Variant of the marker-placement scan (same shape as func_80248A6C) for a
// fourth map table section: picture panes keyed by tbl+0x39B names are
// appended under the tbl+0x28A row pane when every eligibility check passes.
void func_80249344(CFloorMapLayoutSlots* slots) {
    const char* tbl = lbl_eu_8050BEA8;

    // Stack locals declared in reverse of retail frame-slot order
    // (MWCC assigns first-declared the highest slot).
    CFloorMapF64Conv convB;
    CFloorMapF64Conv convA;
    char buf[0x20];
    nw4r::math::_VEC3 pos;
    f32 coords[3];
    nw4r::math::_VEC3 result;
    convA.w[0] = 0x43300000;
    convB.w[0] = 0x43300000;

    u32 fp = (u32)getFP__FPCc(&tbl[0x315]);
    int count = (int)func_8003B1EC(fp);

    // Constants hoisted before the walk so MWCC keeps them in f26-f31
    // (declared in reverse of retail's load order).
    const f32 projScale = lbl_eu_8066877C;
    const f32 regionScale = lbl_eu_80668778;
    const f32 regionBase = lbl_eu_80668788;
    const f32 zero = lbl_eu_80668764;
    const f64 convBias = lbl_eu_80668770;

    for (u16 i = 1; (s32)i <= count; i++) {
        if ((u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x322], i) != lbl_eu_80664798) continue;

        u16 rnd = (u16)func_8009CF8C(0x20);
        u16 left = (u16)func_80136254((const void*)fp, &lbl_eu_8050BEA8[0x329], (int)i);
        if (rnd < left) continue;
        u16 right = (u16)func_80136254((const void*)fp, &lbl_eu_8050BEA8[0x32F], (int)i);
        if (right < rnd) continue;

        u8 c1 = (u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x335], i);
        u8 c2 = (u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x343], i);
        if (func_8009CF8C((u32)c1 + 0x7FC) < c2) continue;

        u8 c3 = (u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x34D], i);
        u8 c4 = (u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x35B], i);
        if (func_8009CF8C((u32)c3 + 0x7FC) < c4) continue;

        u8 v365 = (u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x365], i);
        if (v365 != 0 && func_8009CF8C((u32)v365 + 0x2578) == 0) continue;

        u16 v373 = (u16)func_80136254((const void*)fp, &lbl_eu_8050BEA8[0x373], (int)i);
        if (v373 != 0 && func_8009CF8C((u32)v373 + 0x220) != 0xFE) continue;

        // s16 -> f32 via the shared 0x4330 double-magic idiom; y is cached in
        // a scalar so MWCC dedicates a callee-saved FPR to it across the row
        // loop (retail f26).
        s16 x = func_80136330(fp, &lbl_eu_8050BEA8[0x19B], i);
        s16 y = func_80136330(fp, &lbl_eu_8050BEA8[0x1A0], i);
        s16 z = func_80136330(fp, &lbl_eu_8050BEA8[0x1A5], i);

        convA.w[1] = (u32)(s16)x ^ 0x80000000;
        pos.x = (f32)(convA.d - convBias);
        convB.w[1] = (u32)(s16)y ^ 0x80000000;
        f32 fy = (f32)(convB.d - convBias);
        convA.w[1] = (u32)(s16)z ^ 0x80000000;
        pos.y = fy;
        pos.z = (f32)(convA.d - convBias);

        // Skip markers whose world y sits below the first map row taller than
        // it, unless that row is exactly the row currently displayed.
        u8 found = 0;
        u8 count2 = (u8)func_8003B1EC(lbl_eu_8066479C);
        for (u8 j = 1; j <= count2; j++) {
            s16 h = func_80136330(lbl_eu_8066479C, &lbl_eu_8050BEA8[0x15A], j);
            convB.w[1] = (u32)(s16)h ^ 0x80000000;
            if ((f32)(convB.d - convBias) > fy) {
                if (j == slots->field_0C) found = 1;
                break;
            }
        }
        if (!found) continue;

        if ((u8)func_801361E8(fp, &tbl[0x37D], i) != 4) continue;

        sprintf(buf, &tbl[0x39B], i);

        // Duplicate check against the root pane of the map layout reached
        // through slots+0x00 (pane pointer at layout+0x10).
        if (((nw4r::lyt::Pane*)*(u32*)((u8*)*(u32**)slots + 0x10))
                ->FindPaneByName(buf, 1))
            continue;

        s16 vx = func_80136330(lbl_eu_806640A8, &tbl[0x1E2], lbl_eu_80664798);
        s16 vy = func_80136330(lbl_eu_806640A8, &tbl[0x1F0], lbl_eu_80664798);

        // Region lookup happens between the coordinate conversions and
        // reuses the first conversion slot, as in retail.
        f32 coords[3];
        convB.w[1] = (u32)(s16)vy ^ 0x80000000;
        convA.w[1] = (u32)(s16)vx ^ 0x80000000;
        coords[2] = zero;
        coords[0] = (f32)(convA.d - convBias);
        coords[1] = (f32)(convB.d - convBias);
        u8 region = (u8)func_801361E8(lbl_eu_806640A8, &tbl[0x1FE], lbl_eu_80664798);

        convA.w[1] = region;
        f32 scale = (f32)(convA.d - regionBase) * regionScale;

        // Word-wise copy of the position into a scratch vector; retail reads
        // z back before overwriting it with zero mid-computation.
        nw4r::math::_VEC3 result;
        ((u32*)&result)[0] = ((u32*)&pos)[0];
        ((u32*)&result)[1] = ((u32*)&pos)[1];
        ((u32*)&result)[2] = ((u32*)&pos)[2];

        f32 zTerm = (result.z / scale) * projScale;
        result.z = zero;
        result.x = coords[0] + (result.x / scale) * projScale;
        result.y = coords[1] - zTerm;

        void* picture = func_80248920(slots, (const char*)0xC, result.x, result.y, (void*)-1, buf);

        nw4r::lyt::Pane* pane2 = slots->pane->FindPaneByName(&tbl[0x28A], 1);
        pane2->AppendChild((nw4r::lyt::Pane*)picture);
    }
}

// Scan the marker table of the current map file and place one picture pane
// per eligible marker: each marker is accepted only after its map-id, random
// x-range, encounter-count, flag and position checks pass and the marker is
// not already placed; the picture is appended to the map pane whose name is
// formatted from the marker index.
void func_802497B0(void* self) {
    CFloorMapLayoutSlots* slots = (CFloorMapLayoutSlots*)self;

    // Stack locals declared so MWCC lands each at its retail frame slot
    // (convA@0x50, convB@0x58, buf@0x30, pos@0x20, coords@0x14, result@0x08).
    nw4r::math::_VEC3 result;
    f32 coords[3];
    nw4r::math::_VEC3 pos;
    char buf[0x20];
    CFloorMapF64Conv convB;
    CFloorMapF64Conv convA;
    f32 fy; // marker world y, live across the row loop (retail f26)

    // Both 0x4330 prefix words are stored before the map file is opened.
    convA.w[0] = 0x43300000;
    convB.w[0] = 0x43300000;

    u32 fp = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x315]);
    int count = (int)func_8003B1EC(fp);

    // Constants hoisted before the walk so MWCC keeps them in f26-f31
    // (declared in reverse of retail's load order).
    const f32 projScale = lbl_eu_8066877C;
    const f32 regionScale = lbl_eu_80668778;
    const f32 regionBase = lbl_eu_80668788;
    const f32 zero = lbl_eu_80668764;
    const f64 convBias = lbl_eu_80668770;

    for (u16 i = 1; i <= count; i++) {
        if ((u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x322], i) != lbl_eu_80664798) continue;

        u16 rnd = (u16)func_8009CF8C(0x20);
        u16 left = (u16)func_80136254((const void*)fp, &lbl_eu_8050BEA8[0x329], (int)i);
        if (rnd < left) continue;
        u16 right = (u16)func_80136254((const void*)fp, &lbl_eu_8050BEA8[0x32F], (int)i);
        if (right < rnd) continue;

        u8 c1 = (u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x335], i);
        u8 c2 = (u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x343], i);
        if (func_8009CF8C((u32)c1 + 0x7FC) < c2) continue;

        u8 c3 = (u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x34D], i);
        u8 c4 = (u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x35B], i);
        if (func_8009CF8C((u32)c3 + 0x7FC) < c4) continue;

        u8 v365 = (u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x365], i);
        if (v365 != 0 && func_8009CF8C((u32)v365 + 0x2578) == 0) continue;

        u16 v373 = (u16)func_80136254((const void*)fp, &lbl_eu_8050BEA8[0x373], (int)i);
        if (v373 != 0 && func_8009CF8C((u32)v373 + 0x220) != 0xFE) continue;

        s16 x = func_80136330(fp, &lbl_eu_8050BEA8[0x19B], i);
        s16 y = func_80136330(fp, &lbl_eu_8050BEA8[0x1A0], i);
        s16 z = func_80136330(fp, &lbl_eu_8050BEA8[0x1A5], i);

        // s16 -> f32 via the shared 0x4330 double-magic idiom; slot A carries
        // x then z, slot B carries y.
        convA.w[1] = (u32)(s16)x ^ 0x80000000;
        pos.x = (f32)(convA.d - convBias);
        convB.w[1] = (u32)(s16)y ^ 0x80000000;
        fy = (f32)(convB.d - convBias);
        pos.y = fy;
        convA.w[1] = (u32)(s16)z ^ 0x80000000;
        pos.z = (f32)(convA.d - convBias);

        // Skip markers whose world y sits below the first map row taller than
        // it, unless that row is exactly the row currently displayed.
        u8 found = 0;
        u8 count2 = (u8)func_8003B1EC(lbl_eu_8066479C);
        for (u8 j = 1; j <= count2; j++) {
            s16 h = func_80136330(lbl_eu_8066479C, &lbl_eu_8050BEA8[0x15A], j);
            convB.w[1] = (u32)(s16)h ^ 0x80000000;
            if ((f32)(convB.d - convBias) > fy) {
                if (j == slots->field_0C) found = 1;
                break;
            }
        }
        if (!found) continue;

        if ((u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x37D], i) != 6) continue;

        sprintf(buf, &lbl_eu_8050BEA8[0x3A6], i);
        if (slots->layout->GetRootPane()->FindPaneByName(buf, 1)) continue;

        s16 vx = func_80136330(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1E2], lbl_eu_80664798);
        s16 vy = func_80136330(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1F0], lbl_eu_80664798);

        // Region lookup happens between the coordinate conversions; slot A is
        // reused unsigned for the region id afterwards.
        convA.w[1] = (u32)(s16)vx ^ 0x80000000;
        convB.w[1] = (u32)(s16)vy ^ 0x80000000;
        coords[2] = zero;
        coords[0] = (f32)(convA.d - convBias);
        coords[1] = (f32)(convB.d - convBias);
        u8 region = (u8)func_801361E8(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1FE], lbl_eu_80664798);

        convA.w[1] = region;
        f32 scale = (f32)(convA.d - regionBase) * regionScale;

        result = pos;
        f32 zTerm = (result.z / scale) * projScale;
        result.z = zero;
        result.x = coords[0] + (result.x / scale) * projScale;
        result.y = coords[1] - zTerm;

        void* picture = func_80248920(self, (const char*)0x18, result.x, result.y, (void*)-1, buf);

        nw4r::lyt::Pane* pane2 = slots->pane->FindPaneByName(&lbl_eu_8050BEA8[0x293], 1);
        pane2->AppendChild((nw4r::lyt::Pane*)picture);
    }
}

// Walk the current map's marker table and place one picture pane per entry
// whose display type (tbl+0x37D) is 1-6: each type resolves a world position
// and marker label differently, then the position is projected onto the map
// plane for the current region and the picture appended under the tbl+0x2B4
// row pane. The tbl+0x2BD highlight pane's visible bit is set while placing.
void func_80249C1C(CFloorMapLayoutSlots* slots) {
    // Frame-slot order matters: MWCC allocates locals by declaration order,
    // and retail places the conversion slots at the very top of the local
    // block (0x70/0x78), i.e. last.
    char buf[0x20];
    CFloorMapF64Conv convA;
    CFloorMapF64Conv convB;

    if ((lbl_eu_80664184 & 0xFF) != lbl_eu_80664798) return;
    convA.w[0] = 0x43300000;
    convB.w[0] = 0x43300000;

    const char* tbl = lbl_eu_8050BEA8;

    // Light the map highlight pane while markers are being placed.
    nw4r::lyt::Pane* hl = slots->pane->FindPaneByName(&tbl[0x2BD], true);
    ((CFloorMapPaneFlag*)hl)->mFlag |= 1;

    // Current world id, read fresh after the pane lookup and kept live across
    // the whole marker walk.
    const u8 world = lbl_eu_80664184;
    func_8003AA34();
    u32 fp = (u32)getFP__FPCc(&tbl[0x17F]);
    u32 count = func_8003B1EC(fp);

    // Constants kept live across the whole walk (retail f27-f31).
    const f64 convBias = lbl_eu_80668770;
    const f32 zero = lbl_eu_80668764;
    const f32 regionBase = lbl_eu_80668788;
    const f32 regionScale = lbl_eu_80668778;
    const f32 projScale = lbl_eu_8066877C;

    for (u32 i = 0; i < count; i++) {
        if (!func_80138574((const char*)fp, i + 1)) continue;
        u8 type = func_801361E8(fp, &tbl[0x37D], i + 1);
        if (!type) continue;
        u16 rowId = func_80136254((const void*)fp, &tbl[0x3E6], i + 1);

        nw4r::math::VEC3 foundPos;
        foundPos.x = zero;
        foundPos.y = zero;
        foundPos.z = zero;
        u8 placed = 0;

        switch (type) {
        case 1: {
            // Gimmick-object list: first object whose map-row id matches.
            CFloorMapObjList* glist = (CFloorMapObjList*)func_800B6BEC();
            u8 found = 0;
            for (CFloorMapObjNode* node = glist->head->next; node != glist->head;
                 node = node->next) {
                CFloorMapObj* obj = (CFloorMapObj*)node->object;
                if (obj->m8C != rowId) continue;
                foundPos = *(nw4r::math::VEC3*)obj->GetPos();
                sprintf(buf, &tbl[0x3BC], obj->m74);
                placed = 1;
                found = 1;
                break;
            }
            if (found) break;

            // No live object: fall back to the save-point table. The marker
            // shows only when the save point's own visibility checks pass.
            u32 fp2 = (u32)getFP__FPCc(&tbl[0x17F]);
            u16 id2 = func_8013606C(&tbl[0x3C6], &tbl[0x3D2], rowId);
            if ((u8)func_801361E8(fp2, &tbl[0x18C], id2) != world) break;
            if (func_8009CF8C(id2 + 0x20C8) == 0) break;

            u8 hide;
            if (id2 == 0) {
                hide = 1;
            } else if (getUnk80664658()->field_214 & 0x48000) {
                hide = 1;
            } else {
                u16 left = func_80136254((const void*)fp2, &tbl[0x1AA], id2);
                u16 right = func_80136254((const void*)fp2, &tbl[0x1B5], id2);
                u16 scrollPos = (u16)func_8009CF8C(0x20);
                hide = (left > scrollPos || scrollPos > right) ? 1 : 0;
            }
            if (hide || func_8009CF8C(0x3212) != 0) break;

            f32 tmp[3];
            func_80141DC4(tmp, id2);
            foundPos.x = tmp[0];
            foundPos.y = tmp[1];
            foundPos.z = tmp[2];
            sprintf(buf, &tbl[0x3BC], id2);
            placed = 1;
            break;
        }
        case 2: {
            // Second gimmick list: only objects flagged 0x4000/0x8000.
            CFloorMapObjList* glist = (CFloorMapObjList*)func_800B6C58();
            for (CFloorMapObjNode* node = glist->head->next; node != glist->head;
                 node = node->next) {
                CFloorMapObj* obj = (CFloorMapObj*)node->object;
                if (!((obj->m64 & 0x4000) || (obj->m64 & 0x8000))) continue;
                if (obj->m8C != rowId) continue;
                foundPos = *(nw4r::math::VEC3*)obj->GetPos();
                sprintf(buf, &tbl[0x3ED], obj->m74);
                placed = 1;
                break;
            }
            break;
        }
        case 3: {
            // Landmark table: scan the 0x4C-byte rows for the row id.
            CFloorMapCase2Elem* arr = (CFloorMapCase2Elem*)func_80193804();
            s32 cnt = *(s32*)((u8*)arr + 0x9800);
            CFloorMapCase2Elem* end = arr + cnt;
            for (CFloorMapCase2Elem* e = arr; e != end; e++) {
                if (e->m1C != rowId) continue;
                foundPos.x = e->x;
                foundPos.y = e->y;
                foundPos.z = e->z;
                sprintf(buf, &tbl[0x3F9]);
                placed = 1;
                break;
            }
            break;
        }
        case 4: {
            // Current player position.
            f32 tmp[3];
            func_80141DC4(tmp, rowId);
            foundPos.x = tmp[0];
            foundPos.y = tmp[1];
            foundPos.z = tmp[2];
            sprintf(buf, &tbl[0x2EB], i + 1);
            placed = 1;
            break;
        }
        case 5: {
            // Map-table coordinates: three s16 columns converted via the
            // shared double-magic idiom into a scratch vector.
            u32 fp2 = (u32)getFP__FPCc(&tbl[0x403]);
            func_80136254((const void*)fp2, &tbl[0x412], rowId);
            if (func_8009CF8C(rowId + 0x2B9C) != 0) break;
            s16 a = func_80136330(fp2, &tbl[0x19B], rowId);
            s16 b = func_80136330(fp2, &tbl[0x1A0], rowId);
            s16 c = func_80136330(fp2, &tbl[0x1A5], rowId);
            // Reuse the function-level conversion slots (retail: a,c share
            // slot A, b uses slot B).
            nw4r::math::VEC3 work;
            convA.w[1] = (u32)(s16)a ^ 0x80000000;
            work.x = (f32)(convA.d - convBias);
            convB.w[1] = (u32)(s16)b ^ 0x80000000;
            work.y = (f32)(convB.d - convBias);
            convA.w[1] = (u32)(s16)c ^ 0x80000000;
            work.z = (f32)(convA.d - convBias);
            foundPos.x = work.x;
            foundPos.y = work.y;
            foundPos.z = work.z;
            sprintf(buf, &tbl[0x419], i + 1);
            placed = 1;
            break;
        }
        case 6: {
            if (func_8013C038(rowId)) continue;
            foundPos = *(nw4r::math::VEC3*)func_801F4E68(getUnk80664658(), rowId);
            sprintf(buf, &tbl[0x423], i + 1);
            placed = 1;
            break;
        }
        }

        if (!placed) continue;

        // Keep only rows whose floor height sits above the marker y AND that
        // is exactly the currently displayed map row.
        f32 foundY = foundPos.y;
        u8 hit = 0;
        u8 cntB = (u8)func_8003B1EC(lbl_eu_8066479C);
        for (u8 k = 1; k <= cntB; k++) {
            s16 h = func_80136330(lbl_eu_8066479C, &tbl[0x15A], k);
            convB.w[1] = (u32)(s16)h ^ 0x80000000;
            if ((f32)(convB.d - convBias) > foundY) {
                if (k == slots->field_0C) hit = 1;
                break;
            }
        }
        if (!hit) continue;

        // Skip when a marker pane with this name already exists.
        if (slots->layout->GetRootPane()->FindPaneByName(buf, true)) continue;

        // Region projection onto the map plane. The converted coordinates
        // live in a contiguous array so they spill to stack slots across the
        // region lookup, as in retail.
        s16 vx = func_80136330(lbl_eu_806640A8, &tbl[0x1E2], lbl_eu_80664798);
        s16 vy = func_80136330(lbl_eu_806640A8, &tbl[0x1F0], lbl_eu_80664798);
        f32 fv[3];
        convA.w[1] = (u32)(s16)vx ^ 0x80000000;
        fv[0] = (f32)(convA.d - convBias);
        convB.w[1] = (u32)(s16)vy ^ 0x80000000;
        fv[1] = (f32)(convB.d - convBias);
        fv[2] = zero;
        u8 region = func_801361E8(lbl_eu_806640A8, &tbl[0x1FE], lbl_eu_80664798);
        convA.w[1] = region;
        f32 denom = ((f32)(convA.d - regionBase)) * regionScale;

        // Word-wise copy of the found position; retail reads z back before
        // zeroing it mid-computation.
        nw4r::math::VEC3 res;
        res.x = foundPos.x;
        res.z = foundPos.z;
        f32 xterm = (res.x / denom) * projScale;
        f32 zterm = (res.z / denom) * projScale;
        res.y = foundPos.y;
        res.z = fv[2];
        res.x = fv[0] + xterm;
        res.y = fv[1] - zterm;

        // Pick the picture kind from the selected row's BDAT entry: kind 1
        // directly, otherwise re-check when the tbl+0x440 column is nonzero.
        u16 sel = func_80136254((const void*)fp, &tbl[0x42D], i + 1);
        u32 entry = (u32)lbl_eu_80573D18[func_80138138(sel)];
        u8 ok = func_801361E8(entry, &tbl[0x436], sel) == 1;
        if (!ok) {
            u16 v440 = func_80136254((const void*)fp, &tbl[0x440], i + 1);
            if (v440 != 0) ok = func_801361E8(entry, &tbl[0x436], sel) == 1;
        }
        if (!ok) continue;

        void* pic = func_80248920(slots, (const char*)0x5, res.x, res.y, (void*)-1, buf);

        nw4r::lyt::Pane* rowPane = slots->pane->FindPaneByName(&tbl[0x2B4], true);
        rowPane->AppendChild((nw4r::lyt::Pane*)pic);

        nw4r::lyt::Pane* hl2 = slots->pane->FindPaneByName(&tbl[0x2BD], true);
        ((CFloorMapPaneFlag*)hl2)->mFlag &= 0xFE;
    }
}

// Walk the special-object list and place one landmark picture per eligible
// entry: each object must pass the pane/flag/kind checks, its floor row (first
// row whose height exceeds the object's y) must equal the current map row, and
// the projected position is appended under the formatted marker pane name.
void func_8024A448(void* self) {
    CFloorMapFull* fm = (CFloorMapFull*)self;

    // s16/u8 -> f32 conversion slots (retail stores both 0x4330 prefix words
    // in the prologue, before the random/world-id checks).
    CFloorMapF64Conv convA;
    CFloorMapF64Conv convB;
    convA.w[0] = 0x43300000;
    convB.w[0] = 0x43300000;

    if (!func_8009CF8C(0x3354)) return;
    if ((lbl_eu_80664184 & 0xFF) != lbl_eu_80664798) return;

    CFloorMapObjList* glist = (CFloorMapObjList*)func_800B6C58();

    // Constants hoisted before the walk so MWCC keeps them in f26-f31.
    const f64 convBias = lbl_eu_80668770;
    const f32 zero = lbl_eu_80668764;
    const f32 regionBase = lbl_eu_80668788;
    const f32 regionScale = lbl_eu_80668778;
    const f32 projScale = lbl_eu_8066877C;
    for (CFloorMapObjNode* node = glist->head->next; node != glist->head;
         node = node->next) {
        CFloorMapObj* g = (CFloorMapObj*)node->object;
        if (g->fn0x160() == NULL) continue;
        if ((g->m64 & 0x8000) == 0 && (g->m64 & 0x4000) == 0) continue;
        if (g->m91 != 6) continue;

        // Word-wise snapshot of the position; the y is cached in a scalar so
        // MWCC dedicates a callee-saved FPR to it across the row loop
        // (retail f26).
        CFloorMapVec3 pv = *g->GetPos();
        f32 py = pv.y;

        u8 found = 0;
        u8 count = (u8)func_8003B1EC(lbl_eu_8066479C);
        for (u8 j = 1; j <= count; j++) {
            s16 h = func_80136330(lbl_eu_8066479C, &lbl_eu_8050BEA8[0x15A], j);
            convA.w[1] = (u32)(s16)h ^ 0x80000000;
            if ((f32)(convA.d - convBias) > py) {
                if (j == fm->field_0C) found = 1;
                break;
            }
        }
        if (!found) continue;

        char buf[0x20];
        sprintf(buf, &lbl_eu_8050BEA8[0x3ED], g->m74);

        s16 vx = func_80136330(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1E2], lbl_eu_80664798);
        s16 vy = func_80136330(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1F0], lbl_eu_80664798);

        convB.w[1] = (u32)(s16)vx ^ 0x80000000;
        convA.w[1] = (u32)(s16)vy ^ 0x80000000;
        f32 fvx = (f32)(convB.d - convBias);
        f32 fvy = (f32)(convA.d - convBias);
        u8 region = (u8)func_801361E8(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1FE], lbl_eu_80664798);

        // Word-wise copy of the position into a scratch vector; retail reads
        // x/z back through the copy, zeroes z mid-computation, then stores the
        // projected coordinates back over it.
        nw4r::math::_VEC3 result = *(nw4r::math::_VEC3*)&pv;

        convB.w[1] = region;
        f32 denom = (f32)(convB.d - regionBase) * regionScale;

        f32 zTerm = (result.z / denom) * projScale;
        result.z = zero;
        f32 xTerm = (result.x / denom) * projScale;
        result.x = fvx + xTerm;
        result.y = fvy - zTerm;

        void* picture =
            func_80248920(self, (const char*)0xF, result.x, result.y, (void*)-1, buf);

        nw4r::lyt::Pane* pane = fm->field_08->FindPaneByName(&lbl_eu_8050BEA8[0x2A0], 1);
        pane->AppendChild((nw4r::lyt::Pane*)picture);
    }
}

// Place one landmark marker on the current floor map: pick a random landmark
// row from the world table, format its display name by type (enemy lists,
// landmark table, save points, camps), then check the marker's floor against
// the current map row and append a positioned picture pane to the map layout.
void func_8024A748(void* self) {
    CFloorMapFull* p = (CFloorMapFull*)self;
    const char* tbl = lbl_eu_8050BEA8;

    // Two s16/u8 -> f32 conversion slots (retail stores the 0x4330 prefix
    // words around the world-id check, before any call).
    CFloorMapF64Conv convA;
    CFloorMapF64Conv convB;
    convA.w[0] = 0x43300000;
    u32 worldId = lbl_eu_80664184 & 0xFF;
    convB.w[0] = 0x43300000;
    if (worldId != lbl_eu_80664798) return;

    u32 rnd = func_8009CF8C(0x20);
    if ((s32)rnd <= 0) return;
    if ((func_8013600C(tbl + 0x44A, tbl + 0x457, rnd) & 0xFF) != worldId) return;

    u32 id = func_8013600C(tbl + 0x44A, tbl + 0x37D, rnd);
    u32 type = func_8013606C(tbl + 0x44A, tbl + 0x3E6, rnd);

    nw4r::math::VEC3 vec;
    char buf[0x20];
    vec.x = lbl_eu_80668764;
    vec.y = lbl_eu_80668764;
    vec.z = lbl_eu_80668764;
    buf[0] = 0;

    if ((u16)type == 0) {
        if ((u8)id == 1) {
            // Player-related landmark: walk the enemy list and use the first
            // entry's position.
            CFloorMapObjList* glist = (CFloorMapObjList*)func_800B6CF8(1);
            for (CFloorMapObjNode* node = glist->head->next; node != glist->head;
                 node = node->next) {
                CFloorMapObj* g = (CFloorMapObj*)node->object;
                CFloorMapVec3* pos = g->GetPos();
                vec.x = pos->x;
                vec.y = pos->y;
                vec.z = pos->z;
                sprintf(buf, tbl + 0x45B, g->m74);
                break;
            }
        }
    } else {
        switch ((u8)id) {
        case 2: {
            // Landmark-table entry: scan the 0x4C-byte rows for the id.
            CFloorMapCase2Elem* arr = (CFloorMapCase2Elem*)func_80193804();
            s32 count = *(s32*)((u8*)arr + 0x9800);
            CFloorMapCase2Elem* end = arr + count;
            for (CFloorMapCase2Elem* el = arr; el != end; el++) {
                if (el->m1C != (u16)type) continue;
                vec.x = el->x;
                vec.y = el->y;
                vec.z = el->z;
                sprintf(buf, tbl + 0x3F9, el->m1C);
                break;
            }
            break;
        }
        case 3: {
            // Special-object list: keep entries with either flag bit set.
            CFloorMapObjList* glist = (CFloorMapObjList*)func_800B6C58();
            for (CFloorMapObjNode* node = glist->head->next; node != glist->head;
                 node = node->next) {
                CFloorMapObj* g = (CFloorMapObj*)node->object;
                if ((g->m64 & 0x8000) != 0 || (g->m64 & 0x10000) != 0) {
                    if (g->m8C == (u16)type) {
                        CFloorMapVec3* pos = g->GetPos();
                        vec.x = pos->x;
                        vec.y = pos->y;
                        vec.z = pos->z;
                        sprintf(buf, tbl + 0x3ED, g->m74);
                        break;
                    }
                }
            }
            break;
        }
        case 4: {
            u32 found = 0;
            CFloorMapObjList* glist = (CFloorMapObjList*)func_800B6BEC();
            for (CFloorMapObjNode* node = glist->head->next; node != glist->head;
                 node = node->next) {
                CFloorMapObj* g = (CFloorMapObj*)node->object;
                if (g->m8C == (u16)type) {
                    CFloorMapVec3* pos = g->GetPos();
                    vec.x = pos->x;
                    vec.y = pos->y;
                    vec.z = pos->z;
                    sprintf(buf, tbl + 0x3BC, g->m74);
                    found = 1;
                    break;
                }
            }
            if (!found) {
                // Fallback: resolve the landmark file entry and check whether
                // it is flagged for display on the current world.
                u32 fp = (u32)getFP__FPCc(tbl + 0x17F);
                u32 id2 = func_8013606C(tbl + 0x3C6, tbl + 0x3D2, (u16)type);
                u32 w = func_801361E8(fp, tbl + 0x18C, (u16)id2);
                if (w != lbl_eu_80664184) break;
                if (func_8009CF8C((u16)id2 + 0x20C8) == 0) break;

                u32 ok;
                if ((u16)id2 == 0) {
                    ok = 1;
                } else if (getUnk80664658()->field_214 & 0x110000) {
                    ok = 1;
                } else {
                    u32 left = func_80136254((const void*)fp, tbl + 0x1AA, (u16)id2);
                    u32 right = func_80136254((const void*)fp, tbl + 0x1B5, (u16)id2);
                    u32 scroll = (u16)func_8009CF8C(0x20);
                    if (left > scroll || scroll > right)
                        ok = 0;
                    else
                        ok = 1;
                }
                if (ok) break;

                f32 tmp[3];
                func_80141DC4(tmp, (u16)id2);
                vec.x = tmp[0];
                vec.y = tmp[1];
                vec.z = tmp[2];
                sprintf(buf, tbl + 0x3BC, (u16)type);
            }
            break;
        }
        case 5: {
            u32 found = 0;
            CFloorMapObjList* glist = (CFloorMapObjList*)func_800B6BEC();
            for (CFloorMapObjNode* node = glist->head->next; node != glist->head;
                 node = node->next) {
                CFloorMapObj* g = (CFloorMapObj*)node->object;
                if (g->m8C == (u16)type) {
                    CFloorMapVec3* pos = g->GetPos();
                    vec.x = pos->x;
                    vec.y = pos->y;
                    vec.z = pos->z;
                    sprintf(buf, tbl + 0x3BC, g->m74);
                    found = 1;
                    break;
                }
            }
            if (!found) {
                u32 fp = (u32)getFP__FPCc(tbl + 0x17F);
                u32 id2 = func_8013606C(tbl + 0x3C6, tbl + 0x3D2, (u16)type);
                u32 w = func_801361E8(fp, tbl + 0x18C, (u16)id2);
                if (w != lbl_eu_80664184) break;
                if (func_8009CF8C((u16)id2 + 0x20C8) == 0) break;

                u32 ok;
                if ((u16)id2 == 0) {
                    ok = 1;
                } else if (getUnk80664658()->field_214 & 0x110000) {
                    ok = 1;
                } else {
                    u32 left = func_80136254((const void*)fp, tbl + 0x1AA, (u16)id2);
                    u32 right = func_80136254((const void*)fp, tbl + 0x1B5, (u16)id2);
                    u32 scroll = (u16)func_8009CF8C(0x20);
                    if (left > scroll || scroll > right)
                        ok = 0;
                    else
                        ok = 1;
                }
                if (ok) break;

                f32 tmp[3];
                func_80141DC4(tmp, (u16)id2);
                vec.x = tmp[0];
                vec.y = tmp[1];
                vec.z = tmp[2];
                sprintf(buf, tbl + 0x3BC, (u16)type);
            }
            break;
        }
        }
    }

    // The marker is only placed when its name was formatted and its floor is
    // the current map row (first floor above the marker's height).
    if (buf[0] != 0) {
        const f64 bias = lbl_eu_80668770;
        f32 y = vec.y;
        u8 count = (u8)func_8003B1EC(lbl_eu_8066479C);
        u32 found = 0;
        for (u32 j = 1; j <= count; j++) {
            s16 h = func_80136330(lbl_eu_8066479C, tbl + 0x15A, j);
            convA.w[1] = (u32)(s16)h ^ 0x80000000;
            if ((f32)(convA.d - bias) > y) {
                if (j == p->field_0C) found = 1;
                break;
            }
        }
        if (found) {
            // Map the marker position onto the floor-map pane coordinates.
            // Retail divides work.z (the pre-projection z snapshot) here even
            // though a projected-z of 0 is also computed afterwards.
            s16 vx = func_80136330(lbl_eu_806640A8, tbl + 0x1E2, lbl_eu_80664798);
            s16 vy = func_80136330(lbl_eu_806640A8, tbl + 0x1F0, lbl_eu_80664798);
            convB.w[1] = (u32)(s16)vx ^ 0x80000000;
            convA.w[1] = (u32)(s16)vy ^ 0x80000000;
            nw4r::math::VEC3 pos;
            pos.z = lbl_eu_80668764;
            pos.x = (f32)(convB.d - lbl_eu_80668770);
            pos.y = (f32)(convA.d - lbl_eu_80668770);
            u32 region = func_801361E8(lbl_eu_806640A8, tbl + 0x1FE, lbl_eu_80664798);
            convB.w[1] = region;
            f32 denom = ((f32)(convB.d - lbl_eu_80668788)) * lbl_eu_80668778;

            // Word-wise copy of the marker position so the projection reads
            // it back through frame slots like retail.
            nw4r::math::VEC3 work = *(nw4r::math::VEC3*)&vec;
            work.x = pos.x + (work.x / denom) * lbl_eu_8066877C;
            work.y = pos.y - (work.z / denom) * lbl_eu_8066877C;
            work.z = lbl_eu_80668764;

            void* pic = func_80248920(p, (const char*)7, work.x, work.y, (void*)-1, tbl + 0x466);
            nw4r::lyt::Pane* pane = p->field_08->FindPaneByName(tbl + 0x2BD, true);
            pane->AppendChild((nw4r::lyt::Pane*)pic);
        }
    }
}

// Build the current-position marker: find the floor row whose height is
// above the player's y (and which is the current map row), project the
// player position onto the map pane coordinates, then allocate a wrapper
// pane holding the marker picture and append it to the map root pane.
void func_8024AEEC(void* self) {
    CFloorMapFull* p = (CFloorMapFull*)self;
    // Retail seeds both conversion-slot prefix words before the early-outs
    // and keeps the height-loop bias/count/index in nonvolatile f31/r28/r30.
    CFloorMapF64Conv convA;
    CFloorMapF64Conv convB;
    convA.w[0] = 0x43300000;
    convB.w[0] = 0x43300000;

    if ((u8)lbl_eu_80664184 != lbl_eu_80664798) return;

    CFloorMapPlayerObj* player =
        (CFloorMapPlayerObj*)cf::CfGameManager::getPlayer(0);
    if (player == NULL) return;
    CFloorMapVec3* pos = player->GetPos();
    u8 found = 0;
    // Word-wise snapshot so the position lives in frame slots (retail
    // 0x40-0x4B) instead of nonvolatile FPRs across the table walk.
    nw4r::math::_VEC3 ppos;
    ppos.y = pos->y;
    ppos.x = pos->x;
    ppos.z = pos->z;
    f32 py = ppos.y;

    const char* tbl = lbl_eu_8050BEA8;
    u8 count = (u8)func_8003B1EC(lbl_eu_8066479C);
    const f64 bias = lbl_eu_80668770;
    for (u8 i = 1; i <= count; i++) {
        s16 h = func_80136330(lbl_eu_8066479C, &tbl[0x15A], (u32)i);
        convA.w[1] = (u32)(s16)h ^ 0x80000000;
        if ((f32)(convA.d - bias) > py) {
            if (i == p->field_0C) found = 1;
            break;
        }
    }
    if (!found) return;

    // Project onto the current world's map pane coordinate system.
    u16 vx = (u16)func_80136330(lbl_eu_806640A8, &tbl[0x1E2], (u32)lbl_eu_80664798);
    u16 vy = (u16)func_80136330(lbl_eu_806640A8, &tbl[0x1F0], (u32)lbl_eu_80664798);
    convB.w[1] = (u32)(s16)vx ^ 0x80000000;
    convA.w[1] = (u32)(s16)vy ^ 0x80000000;
    // Second vector local so MWCC spills the projected bases to frame
    // slots (retail 0x34/0x38/0x3c) instead of burning a nonvolatile FPR.
    nw4r::math::_VEC3 work;
    work.z = lbl_eu_80668764;
    work.x = (f32)(convB.d - lbl_eu_80668788);
    work.y = (f32)(convA.d - lbl_eu_80668788);
    u8 region = (u8)func_801361E8(lbl_eu_806640A8, &tbl[0x1FE], (u32)lbl_eu_80664798);
    convB.w[1] = region;
    f32 denom = (f32)(convB.d - lbl_eu_80668788) * lbl_eu_80668778;

    // Snapshot through a vector local so the pre-projection z feeds the
    // second term while the stored z is replaced by zero.
    nw4r::math::_VEC3 result;
    result.x = ppos.x;
    result.y = py;
    result.z = ppos.z;
    result.x = work.x + (result.x / denom) * lbl_eu_8066877C;
    result.y = work.y - (result.z / denom) * lbl_eu_8066877C;
    result.z = lbl_eu_80668764;

    nw4r::lyt::Pane* picPane = (nw4r::lyt::Pane*)func_80248920(
        self, (const char*)2, lbl_eu_80668764, lbl_eu_80668764, (void*)(-1), &tbl[0x470]);

    // Re-apply the picture translate: x cleared and y scaled by the layout
    // constant (z left untouched).
    f32 picH = picPane->GetSize().height;
    CFloorMapPaneTransXY* picXY = (CFloorMapPaneTransXY*)picPane;
    picXY->transX = lbl_eu_80668764;
    picXY->transY = picH * lbl_eu_80668798;

    // Wrapper pane that owns the picture and carries the projected position.
    nw4r::lyt::Pane* holder =
        new (getAllocHandle__10CLibLayoutFv()) nw4r::lyt::Pane();
    holder->SetName(&tbl[0x2D6]);

    CFloorMapPaneFlag* pf = (CFloorMapPaneFlag*)holder;
    pf->mFlag = (pf->mFlag & 0xFE) | 1;

    holder->SetTranslate(*(nw4r::math::VEC3*)&result);

    UnkClass_800821F8* mgr = cf::CfGameManager::getInstance()->func_800821F8();
    if (mgr != NULL) {
        // Scale the marker by the game's map zoom numerator; retail stores it
        // into the wrapper pane's rotate vector.
        f32 scale =
            lbl_eu_8066879C * ((CFloorMapMgrView*)mgr)->getScaleSrc()->scale04 /
            lbl_eu_8066A1F8;
        CFloorMapPaneRotate* rot = (CFloorMapPaneRotate*)holder;
        rot->rotX = lbl_eu_80668764;
        rot->rotY = lbl_eu_80668764;
        rot->rotZ = scale;
    }

    holder->AppendChild(picPane);

    nw4r::lyt::Pane* target = p->field_08->FindPaneByName(&tbl[0x2C7], true);
    target->AppendChild(holder);
}

// Place the current-position marker on the floor map: after confirming the
// current world matches and a live player exists, find the floor row whose
// height is above the player's y, project the player position onto the map
// pane coordinates, build the marker picture pane and append it to the map
// root; finally scale it by the player object's zoom factor (vtable+0xCC).
void func_8024B234(CFloorMapFull* self) {
    // Locals declared so MWCC packs them into the retail frame slots
    // (MWCC allocates in reverse declaration order): pre-projection pos
    // @0x08, projected work @0x14, player pos snapshot @0x20, conversion
    // scratch @0x30/0x38.
    CFloorMapF64Conv convB;
    CFloorMapF64Conv convA;
    nw4r::math::_VEC3 ppos;
    nw4r::math::_VEC3 work;
    nw4r::math::_VEC3 tmp;
    // Two s16/u8 -> f32 conversion slots (retail stores both 0x4330 prefix
    // words from the prologue, before the world-id check).
    convA.w[0] = 0x43300000;
    convB.w[0] = 0x43300000;

    if ((u8)lbl_eu_80664184 != lbl_eu_80664798) return;

    CFloorMapPlayerObj* player = (CFloorMapPlayerObj*)cf::CfGameManager::getPlayer(0);
    if (player == NULL) return;

    CFloorMapVec3* pos = player->GetPos();
    // Word-wise snapshot so the loop compares a cached float (retail f30)
    // and the projection reads pre-projection x/z.
    ((u32*)&ppos)[0] = ((u32*)pos)[0];
    ((u32*)&ppos)[1] = ((u32*)pos)[1];
    ((u32*)&ppos)[2] = ((u32*)pos)[2];
    f32 py = ppos.y;

    u32 count = (u32)(u8)func_8003B1EC(lbl_eu_8066479C);
    u32 found = 0;
    for (u32 i = 1; i <= count; i++) {
        s16 h = func_80136330(lbl_eu_8066479C, &lbl_eu_8050BEA8[0x15A], (u32)i);
        convA.w[1] = (u32)(s16)h ^ 0x80000000;
        if ((f32)(convA.d - lbl_eu_80668770) > py) {
            if ((u8)i == self->field_0C) found = 1;
            break;
        }
    }
    if (!found) return;

    // Project the world position onto the floor-map pane coordinates.
    s16 vx = func_80136330(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1E2], lbl_eu_80664798);
    s16 vy = func_80136330(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1F0], lbl_eu_80664798);
    convB.w[1] = (u32)(s16)vx ^ 0x80000000;
    f32 fx = (f32)(convB.d - lbl_eu_80668770);
    convA.w[1] = (u32)(s16)vy ^ 0x80000000;
    f32 fy = (f32)(convA.d - lbl_eu_80668770);

    work.x = fx;
    work.y = fy;
    work.z = lbl_eu_80668764;

    u8 region = (u8)func_801361E8(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1FE], lbl_eu_80664798);
    convB.w[1] = region;
    f32 denom = (f32)(convB.d - lbl_eu_80668788) * lbl_eu_80668778;

    tmp = *(nw4r::math::_VEC3*)&ppos;
    work.x = fx + (tmp.x / denom) * lbl_eu_8066877C;
    work.y = fy - (tmp.z / denom) * lbl_eu_8066877C;

    void* pic = func_80248920(self, (const char*)1, work.x, work.y, (void*)-1,
                              &lbl_eu_8050BEA8[0x478]);
    nw4r::lyt::Pane* root =
        self->field_08->FindPaneByName(&lbl_eu_8050BEA8[0x2cf], true);
    root->AppendChild((nw4r::lyt::Pane*)pic);

    // Scale the marker by the player object's zoom factor.
    f32 zoom = player->fn0xCC();
    // Reset of the position snapshot: dead in retail but kept by MWCC since
    // ppos's address was taken for the word-wise copy above.
    ppos.y = lbl_eu_80668764;
    ppos.x = lbl_eu_80668764;
    *(f32*)((u8*)pic + 0x38) = lbl_eu_80668764;
    *(f32*)((u8*)pic + 0x3C) = lbl_eu_80668764;
    *(f32*)((u8*)pic + 0x40) =
        lbl_eu_8066879C + lbl_eu_8066879C * zoom / lbl_eu_8066A1F8;
}

// Accumulate the parent-chain translate of `node` up to (excluding) the
// layout's root pane into `output`. `data` points at a CFloorMap layout slot
// whose +0x00 holds the Layout* (root pane at layout+0x10); the walk stops at
// that root (or a NULL parent) and recurses past the fourth level. The callers
// (func_8024F7CC / func_8024D614 / func_80248558) hand-inline this walk with
// their own stack arrays; only the recursion stays out-of-line (as in retail).
void func_8024B4CC(nw4r::math::VEC3* output, void* data, nw4r::lyt::Pane* node) {
    output->x = lbl_eu_80668764;
    output->y = lbl_eu_80668764;
    output->z = lbl_eu_80668764;

    if (((nw4r::lyt::Layout*)*(void**)data)->GetRootPane() == node || node == NULL) return;

    output->x = node->GetTranslate().x;
    output->y = node->GetTranslate().y;
    output->z = node->GetTranslate().z;

    nw4r::lyt::Pane* parent = node->GetParent();
    nw4r::math::VEC3 accum;
    accum.x = lbl_eu_80668764;
    accum.y = lbl_eu_80668764;
    accum.z = lbl_eu_80668764;
    if (((nw4r::lyt::Layout*)*(void**)data)->GetRootPane() != parent && parent != NULL) {
        accum.x = parent->GetTranslate().x;
        accum.y = parent->GetTranslate().y;
        accum.z = parent->GetTranslate().z;

        nw4r::lyt::Pane* grandparent = parent->GetParent();
        nw4r::math::VEC3 temp;
        temp.x = lbl_eu_80668764;
        temp.y = lbl_eu_80668764;
        temp.z = lbl_eu_80668764;
        if (((nw4r::lyt::Layout*)*(void**)data)->GetRootPane() != grandparent && grandparent != NULL) {
            temp.x = grandparent->GetTranslate().x;
            temp.y = grandparent->GetTranslate().y;
            temp.z = grandparent->GetTranslate().z;

            nw4r::lyt::Pane* ggp = grandparent->GetParent();
            nw4r::math::VEC3 tmp2;
            code80135FDC_setVec3((float*)&tmp2, lbl_eu_80668764, lbl_eu_80668764, lbl_eu_80668764);
            if (((nw4r::lyt::Layout*)*(void**)data)->GetRootPane() != ggp && ggp != NULL) {
                copyVEC3(&tmp2, &ggp->GetTranslate());
                nw4r::math::VEC3 recurse;
                func_8024B4CC(&recurse, data, ggp->GetParent());
                func_80137738(&tmp2, &recurse);
            }
            nw4r::math::VEC3Add(&temp, &temp, &tmp2);
        }
        nw4r::math::VEC3Add(&accum, &accum, &temp);
    }
    nw4r::math::VEC3Add(output, output, &accum);
}

void* __dt__8024B6B8(void* self, int mode) {
    extern void* __dl__FPv(void*);
    if (self && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

void func_8024B6F8(CFloorMapRowList* self, void* arg2, u32 arg3, u32 arg4, u32 arg5) {
    if (arg2 == 0)
        return;
    if (self->mData == 0)
        return;

    func_80137E7C(self->mData, &lbl_eu_8050BEA8[0x47F], arg2);

    // Tag each row pane: panes within the 8 rows above the current row are
    // shown, the rest are hidden.
    for (u8 i = 1; i <= arg4; i++) {
        char buf[0x20];
        sprintf(buf, &lbl_eu_8050BEA8[0x487], i);
        nw4r::lyt::Pane* pane =
            self->mData->GetRootPane()->FindPaneByName(buf, 1);
        if (pane)
            pane->SetVisible(__cntlzw(arg3 - (u8)i) & 0x20);
    }

    // Random visibility for the two special row kinds.
    if (arg5 == 0xC) {
        nw4r::lyt::Pane* pane =
            self->mData->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x491], 1);
        if (pane) {
            u32 v = func_8009CF8C(0x20) ^ 0x166;
            pane->SetVisible(((0x166 << __cntlzw(v)) & 0x80000000ul) != 0);
        }
    } else if (arg5 == 5) {
        nw4r::lyt::Pane* pane =
            self->mData->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x491], 1);
        if (pane) {
            pane->SetVisible(func_8009CF8C(0x20) >= 0x171);
        }
    }
}

void* __dt__8024B894(void* self, int mode) {
    extern void* __dl__FPv(void*);
    if (self && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

// Staging-image ctor (defined below in this TU; retail keeps the plain name).
extern "C" void* __ct__80244F50(void* self, int a, int b, u8 c);

// Layout-compatible body view of CSysWin (CFloorMap.hpp's fwd decl keeps
// CSysWin incomplete here; CSysWin.hpp's dtor decl collides with this TU's).
struct CSysWinBody {
    void* mVtbl;
    UnkClass_8045F564 mMemRegion;
    void* mFileHandle;
    void* mTagProcessor;
    void* mArcAccessor;
    void* mLayout;
    void* mAnimTrans;
    u8 field_28;
    u8 _29[3];
    u32 field_2C;
    u32 field_30;
    u8 field_34;
    u8 field_35;
    u8 field_36;
    u8 field_37;
    u8 field_38;
    u8 field_39;
};

// Complete-object constructor (retail __ct__CFloorMap). Order: vtable, the
// two UnkClass_8045F564 memory regions, the scalar/POD block, the four
// embedded widgets (scrollbar / cursor / 2 syswins), the layout-slot block
// (zeroed twice - a whole-array memset plus an explicit per-entry pass,
// both present in retail), the cursor staging ctor at +0x1FC, then three
// stack temps whose bodies are memberwise-copied over the embedded widgets
// (skipping each temp's vptr) before being destroyed.
CFloorMap::CFloorMap() {
    mVtbl = (void*)lbl_eu_80537028;
    __ct__17UnkClass_8045F564Fv(reinterpret_cast<UnkClass_8045F564*>(&mMemRegion04[0]));
    __ct__17UnkClass_8045F564Fv(reinterpret_cast<UnkClass_8045F564*>(&mMemRegion14[0]));
    field_24 = 0;
    field_28 = 0;
    field_2C = 0;
    field_30 = 0;
    field_34 = 0;
    field_38 = 0;
    allocated_buf_3C = 0;
    field_40 = 0;
    field_41 = 0;
    field_42 = 0;
    field_43 = 1;
    pos_x_44 = lbl_eu_80668764;
    pos_y_48 = lbl_eu_80668764;
    field_4C = lbl_eu_80668764;
    field_50 = lbl_eu_80668764;
    field_54 = lbl_eu_80668764;
    cursor_active_58 = 0;
    field_5A = 0;
    cursor_idx_5C = 0;
    field_5D = 0;
    __ct__CScrollBar(&mScrollBar[0], 0);
    __ct__CCur18(&mCursorA0[0], 0);
    __ct__CSysWin(reinterpret_cast<CSysWin*>(&mSysWinB8[0]), 0);
    __ct__CSysWin(reinterpret_cast<CSysWin*>(&mSysWinF4[0]), 0);
    layout_130 = 0;
    field_134 = 0;
    layout_138 = 0;
    field_13C = 0;
    layout_140 = 0;
    field_144 = 0;
    field_148 = 0;
    field_14C = 0;
    layouts_150[0].mPtr = 0;
    layouts_150[0].mUnk = 0;
    {
        // Retail expands this zero-fill inline (chunked word-store template);
        // this compiler never expands memset, so spell the fill out.
        u32* p = &layouts_150[1].mPtr;
        u32* end = (u32*)(layouts_150 + 0x14);
        if ((u32)p < (u32)end) {
            u32* last = end - 16;
            u32 chunks = (u32)(((u8*)end - 1 - (u8*)p) >> 6);
            if (p < last) {
                do {
                    p[0] = 0; p[1] = 0; p[2] = 0; p[3] = 0;
                    p[4] = 0; p[5] = 0; p[6] = 0; p[7] = 0;
                    p[8] = 0; p[9] = 0; p[10] = 0; p[11] = 0;
                    p[12] = 0; p[13] = 0; p[14] = 0; p[15] = 0;
                    p += 16;
                } while (--chunks);
            }
            u32 pairs = (u32)(((u8*)end + 7 - (u8*)p) >> 3);
            if (p < end) {
                do {
                    p[0] = 0;
                    p[1] = 0;
                    p += 2;
                } while (--pairs);
            }
        }
    }
    layout_count_1F0 = 0;
    field_1F4 = 0;
    field_1F8 = 0;
    for (int i = 0; i < 0x14; i++) {
        layouts_150[i].mPtr = 0;
        layouts_150[i].mUnk = 0;
    }
    // Build the floor-map cursor staging image in place at +0x1FC.
    __ct__80244F50(&layout_1FC, 0, 0, 0);
    field_3330 = 0;
    layout_3334 = 0;
    field_3338 = 0;
    field_333C = 0;
    field_3340 = NULL;
    field_3344 = lbl_eu_80668760;

    // Rebuild the embedded scrollbar from a stack temp (direction 3).
    {
        u8 sbStorage[0x40];
        CScrollBar* sb = reinterpret_cast<CScrollBar*>(sbStorage);
        __ct__CScrollBar(sb, 3);
        CScrollBar* dst = reinterpret_cast<CScrollBar*>(&mScrollBar[0]);
        dst->mMemRegion = sb->mMemRegion;
        dst->mFileHandle = sb->mFileHandle;
        dst->mAccessor = sb->mAccessor;
        dst->mLayout = sb->mLayout;
        dst->mAnimTransform = sb->mAnimTransform;
        dst->mReady = sb->mReady;
        dst->mVisible = sb->mVisible;
        dst->mState = sb->mState;
        dst->mActive = sb->mActive;
        dst->mAnimOffset = sb->mAnimOffset;
        dst->mScrollPosY = sb->mScrollPosY;
        dst->mScrollRatio = sb->mScrollRatio;
        dst->mThumbHeight = sb->mThumbHeight;
        dst->mContentHeight = sb->mContentHeight;
        dst->mDirection = sb->mDirection;
        sb->~CScrollBar();
    }

    // Rebuild the +0xB8 system window from a stack temp (kind 3).
    {
        u8 winStorage[0x3C];
        CSysWinBody* win = reinterpret_cast<CSysWinBody*>(winStorage);
        __ct__CSysWin(reinterpret_cast<CSysWin*>(win), 3);
        CSysWinBody* dst = reinterpret_cast<CSysWinBody*>(&mSysWinB8[0]);
        dst->mMemRegion = win->mMemRegion;
        dst->mFileHandle = win->mFileHandle;
        dst->mTagProcessor = win->mTagProcessor;
        dst->mArcAccessor = win->mArcAccessor;
        dst->mLayout = win->mLayout;
        dst->mAnimTrans = win->mAnimTrans;
        dst->field_28 = win->field_28;
        dst->field_2C = win->field_2C;
        dst->field_30 = win->field_30;
        dst->field_34 = win->field_34;
        dst->field_35 = win->field_35;
        dst->field_36 = win->field_36;
        dst->field_37 = win->field_37;
        dst->field_38 = win->field_38;
        dst->field_39 = win->field_39;
        win->~CSysWinBody();
    }

    // Rebuild the +0xF4 system window from a stack temp (kind 0).
    {
        u8 winStorage[0x3C];
        CSysWinBody* win = reinterpret_cast<CSysWinBody*>(winStorage);
        __ct__CSysWin(reinterpret_cast<CSysWin*>(win), 0);
        CSysWinBody* dst = reinterpret_cast<CSysWinBody*>(&mSysWinF4[0]);
        dst->mMemRegion = win->mMemRegion;
        dst->mFileHandle = win->mFileHandle;
        dst->mTagProcessor = win->mTagProcessor;
        dst->mArcAccessor = win->mArcAccessor;
        dst->mLayout = win->mLayout;
        dst->mAnimTrans = win->mAnimTrans;
        dst->field_28 = win->field_28;
        dst->field_2C = win->field_2C;
        dst->field_30 = win->field_30;
        dst->field_34 = win->field_34;
        dst->field_35 = win->field_35;
        dst->field_36 = win->field_36;
        dst->field_37 = win->field_37;
        dst->field_38 = win->field_38;
        dst->field_39 = win->field_39;
        win->~CSysWinBody();
    }
}

// Complete-object destructor.  Sub-objects are opaque byte arrays, so their
// retail destructors are invoked explicitly in reverse construction order
// (+0xF4, +0xB8, +0xA0, +0x60, +0x14, +0x04); MWCC emits the null-check on
// this and the flags-conditional operator delete automatically for the member
// destructor form.
CFloorMap::~CFloorMap() {
    __dt__7CSysWinFv(mSysWinF4, -1);
    __dt__7CSysWinFv(mSysWinB8, -1);
    __dt__6CCur18Fv(mCursorA0, -1);
    __dt__10CScrollBarFv(mScrollBar, -1);
    __dt__17UnkClass_8045F564Fv(mMemRegion14, -1);
    __dt__17UnkClass_8045F564Fv(mMemRegion04, -1);
}

// Loads the floor-map archive: builds the cursor staging image from the
// scrollbar sub-object plus the map resource accessor, installs it at
// this+0x1FC, initialises the embedded widgets (__ct__8024507C), then resolves
// the language-specific data-file path from the message tables and starts the
// asynchronous read on the MEM2 device file (result lands in field_2C).
// Staging-image ctor (defined below) + resident-cursor widget init ctor.
extern "C" void* __ct__80244F50(void* self, int a, int b, u8 c);
extern "C" void __ct__8024507C(void* self);

void func_8024BE1C(CFloorMap* self) {
    CFloorMapStage stage;
    char path[0x80];
    u32 lang;
    u16 v1;
    u16 v2;
    const char* tbl;
    u32 sel;
    const char* msgName;

    lang = func_8024FB78(self);
    __ct__80244F50(&stage, (int)self->mScrollBar, (int)self->field_34, (u8)lang);

    // Install the staged image into the resident cursor at this+0x1FC. The
    // 5-byte state run is copied byte-wise; its last byte aliases the first
    // byte of the 0x618 entry-block chunk that follows it.
    #define FM_DST (((CFloorMapStage*)((u8*)self + 0x1FC)))
    FM_DST->mData = stage.mData;
    FM_DST->accessor = stage.accessor;
    *(CFloorMapHeadFlags*)&FM_DST->headFlags = *(CFloorMapHeadFlags*)&stage.headFlags;
    FM_DST->blocks = stage.blocks;
    FM_DST->floorCount = stage.floorCount;
    for (int i = 0; i < 4; i++) {
        FM_DST->slots[i].body = stage.slots[i].body;
    }
    FM_DST->scrollBarPtr = stage.scrollBarPtr;
    __ct__8024507C(FM_DST);
    #undef FM_DST

    // Pick the data file name from the message tables based on the unlocked-
    // region / map flags; falls back to a default name when checks fail.
    lang = func_8024FB78(self);
    if ((u8)lang == 0) {
        lang = 1;
    }
    tbl = lbl_eu_8050BEA8;
    v1 = func_80136254((const void*)lbl_eu_8066479C, tbl + 0x4af, lang & 0xFF);
    v2 = func_80136254((const void*)lbl_eu_8066479C, tbl + 0x4b5, lang & 0xFF);
    // Reuses the table-pointer slot for the third lookup, matching retail.
    tbl = (const char*)(u32)func_801361E8((u32)lbl_eu_8066479C, tbl + 0x4bc, lang & 0xFF);

    if (v1 != 0 && func_8009CF8C(0x20) >= v1) {
        if (v2 == 0) {
            sel = 1;
        } else {
            sel = (func_8009CF8C(v2 + 0x220) < (u8)(u32)tbl) ? 1 : 2;
        }
    } else if (v2 == 0) {
        sel = 0;
    } else {
        sel = (func_8009CF8C(v2 + 0x220) < (u8)(u32)tbl) ? 0 : 1;
    }

    switch (sel) {
    case 0:
        msgName = tbl + 0x4c1;
        break;
    case 1:
        msgName = tbl + 0x4c8;
        break;
    case 2:
        msgName = tbl + 0x4d0;
        break;
    default:
        msgName = 0;
        break;
    }

    // Build "<dir>/<file>" path and kick off the async read on this as the
    // IWorkEvent callback target.
    sprintf(path, tbl + 0x4d8,
            func_80138F78(func_80136254((const void*)lbl_eu_8066479C, msgName, lang & 0xFF)));
    self->field_2C = (u32)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (u32)getHandleMEM2__Q23mtl10MemManagerFv(), path, self, 0, 0);
}


// Per-frame update: count down the map-buffer free timer (freeing the buffer
// when it expires), and while scrolling (field_41 == 2) ease the map cursor
// position toward the target and push it into every map layout's pane; then
// update the sub-widgets, advance every bound layout animation and drive the
// per-layout animation state machines.
void func_8024C1FC(void* self) {
    CFloorMapFull* p = (CFloorMapFull*)self;
    if (!p->field_40) return;
    if (!p->field_41) return;

    // Timer-driven deallocation of the loaded-map buffer: count down once per
    // frame and free the buffer once the timer drops below zero.
    u8* buf = p->field_3340;
    if (buf != 0) {
        f32 t = p->field_3344 - lbl_eu_80668760;
        p->field_3344 = t;
        if (t < lbl_eu_80668764) {
            if (buf != 0) {
                mtl::MemManager::deallocate(buf);
                p->field_3340 = 0;
            }
        }
    }

    if ((s32)p->field_41 == 2) {
        // Ease the cursor position toward the stored target and push the new
        // position into every map layout's target pane.
        nw4r::math::_VEC3 vec = { p->pos_x_44 - p->field_4C, p->pos_y_48 - p->field_50,
                                  lbl_eu_80668764 };
        p->pos_x_44 = vec.x;
        p->pos_y_48 = vec.y;
        CFloorMapSetSlotPos((CFloorMapLayoutHolder*)p->mLayout130, vec);
        CFloorMapSetSlotPos((CFloorMapLayoutHolder*)p->mLayout138, vec);
        CFloorMapSetSlotPos((CFloorMapLayoutHolder*)p->mLayout140, vec);
        if (p->mLayouts150[0].mPtr != 0) {
            for (u8 i = 0; i < p->field_1F0; i++) {
                CFloorMapSetPanePos(((CFloorMapLayoutHolder*)p->mLayouts150[i].mPtr)->pane, vec);
            }
        }

        f32 t = p->field_54 - lbl_eu_80668760;
        p->field_54 = t;
        if (t <= lbl_eu_80668764) p->field_41 = 1;
    }

    func_801F3540(&p->mScrollBar);
    func_801D202C(&p->mCursor);
    func_8022B748(&p->mSysWinB8);
    func_8022B748(&p->mSysWinF4);

    if (p->mLayout130 != 0) ((nw4r::lyt::Layout*)p->mLayout130)->Animate(0);
    if (p->mLayout138 != 0) ((nw4r::lyt::Layout*)p->mLayout138)->Animate(0);
    if (p->mLayout140 != 0) ((nw4r::lyt::Layout*)p->mLayout140)->Animate(0);

    // Flag each 0x150 map layout's pane when the current floor is the first
    // floor whose height is above the layout's stored marker height.
    if (p->mLayouts150[0].mPtr != 0) {
        for (u8 i = 0; i < p->field_1F0; i++) {
            CFloorMapF64Conv conv;
            conv.w[0] = 0x43300000;
            conv.w[1] = p->mLayouts150[i].mUnk ^ 0x80000000;
            f32 y = (f32)(conv.d - lbl_eu_80668770);

            u8 count = (u8)func_8003B1EC(lbl_eu_8066479C);
            u8 found = 0;
            for (u8 j = 1; j <= count; j++) {
                s16 hv = func_80136330(lbl_eu_8066479C, &lbl_eu_8050BEA8[0x15A], j);
                conv.w[0] = 0x43300000;
                conv.w[1] = (u32)hv ^ 0x80000000;
                if ((f32)(conv.d - lbl_eu_80668770) > y) {
                    if (j == p->field_1F8) found = 1;
                    break;
                }
            }

            CFloorMapPaneFlag* pf = (CFloorMapPaneFlag*)((CFloorMapLayoutHolder*)p->mLayouts150[i].mPtr)->pane;
            pf->mFlag = (pf->mFlag & 0xFE) | found;
            ((nw4r::lyt::Layout*)p->mLayouts150[i].mPtr)->Animate(0);
        }
    }

    // The remaining layout animation groups only run while the floor-map
    // layout (0x1FC) is built.
    if (p->mLayout1FC != 0) {
        ((nw4r::lyt::Layout*)p->mLayout1FC)->Animate(0);

        if (p->mLayout32D4 != 0 && p->field_32E5 != 0) {
            switch (p->field_32E4) {
            case 0:
                func_80137444((nw4r::lyt::AnimTransform*)p->mLayout32DC, lbl_eu_80668760);
                break;
            case 1:
                if (func_80137444((nw4r::lyt::AnimTransform*)p->mLayout32E0, lbl_eu_80668760)) {
                    ((nw4r::lyt::Layout*)p->mLayout32D4)->SetAnimationEnable((nw4r::lyt::AnimTransform*)p->mLayout32E0, false);
                    ((nw4r::lyt::Layout*)p->mLayout32D4)->SetAnimationEnable((nw4r::lyt::AnimTransform*)p->mLayout32DC, true);
                    ((nw4r::lyt::AnimTransform*)p->mLayout32DC)->SetFrame(lbl_eu_80668764);
                    p->field_32E4 = 0;
                }
                break;
            }
            ((nw4r::lyt::Layout*)p->mLayout32D4)->Animate(0);
        }

        if (p->field_204 != 0 && p->mLayout32EC != 0 && p->field_32FD != 0) {
            switch (p->field_32FC) {
            case 0:
                func_80137444((nw4r::lyt::AnimTransform*)p->mLayout32F4, lbl_eu_80668760);
                break;
            case 1:
                if (func_80137444((nw4r::lyt::AnimTransform*)p->mLayout32F8, lbl_eu_80668760)) {
                    ((nw4r::lyt::Layout*)p->mLayout32EC)->SetAnimationEnable((nw4r::lyt::AnimTransform*)p->mLayout32F8, false);
                    ((nw4r::lyt::Layout*)p->mLayout32EC)->SetAnimationEnable((nw4r::lyt::AnimTransform*)p->mLayout32F4, true);
                    ((nw4r::lyt::AnimTransform*)p->mLayout32F4)->SetFrame(lbl_eu_80668764);
                    p->field_32FC = 0;
                }
                break;
            }
            ((nw4r::lyt::Layout*)p->mLayout32EC)->Animate(0);
        }

        s8 fi = (s8)p->field_205;
        CFloorMapFloorBlock* floor = (CFloorMapFloorBlock*)((u8*)p + fi * 0x30C);
        if (floor->field_514 != 0 && p->mLayout3304 != 0 && p->field_3315 != 0) {
            switch (p->field_3314) {
            case 0:
                func_80137444((nw4r::lyt::AnimTransform*)p->mLayout330C, lbl_eu_80668760);
                break;
            case 1:
                if (func_80137444((nw4r::lyt::AnimTransform*)p->mLayout3310, lbl_eu_80668760)) {
                    ((nw4r::lyt::Layout*)p->mLayout3304)->SetAnimationEnable((nw4r::lyt::AnimTransform*)p->mLayout3310, false);
                    ((nw4r::lyt::Layout*)p->mLayout3304)->SetAnimationEnable((nw4r::lyt::AnimTransform*)p->mLayout330C, true);
                    ((nw4r::lyt::AnimTransform*)p->mLayout330C)->SetFrame(lbl_eu_80668764);
                    p->field_3314 = 0;
                }
                break;
            }
            ((nw4r::lyt::Layout*)p->mLayout3304)->Animate(0);
        }

        if (p->mLayout331C != 0 && p->field_332D != 0) {
            switch (p->field_332C) {
            case 0:
                func_80137444((nw4r::lyt::AnimTransform*)p->mLayout3324, lbl_eu_80668760);
                break;
            case 1:
                if (func_80137444((nw4r::lyt::AnimTransform*)p->mLayout3328, lbl_eu_80668760)) {
                    ((nw4r::lyt::Layout*)p->mLayout331C)->SetAnimationEnable((nw4r::lyt::AnimTransform*)p->mLayout3328, false);
                    ((nw4r::lyt::Layout*)p->mLayout331C)->SetAnimationEnable((nw4r::lyt::AnimTransform*)p->mLayout3324, true);
                    ((nw4r::lyt::AnimTransform*)p->mLayout3324)->SetFrame(lbl_eu_80668764);
                    p->field_332C = 0;
                }
                break;
            }
            ((nw4r::lyt::Layout*)p->mLayout331C)->Animate(0);
        }
    }

    if (p->field_333C != 0 && p->mLayout3334 != 0) {
        ((nw4r::lyt::Layout*)p->mLayout3334)->Animate(0);
    }
}

void func_8024C8F8(void* self, void* drawInfo) {
    CFloorMapFull* p = (CFloorMapFull*)self;
    if (!p->field_40) return;
    if (!p->field_41) return;
    if (p->mLayout130)
        func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(p->mLayout130), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
    if (p->mLayout138)
        func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(p->mLayout138), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
    if (p->mLayout140)
        func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(p->mLayout140), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
    if (p->mLayouts150[0].mPtr) {
        for (u8 i = 0; i < p->field_1F0; i++) {
            void* s = *(void**)((u8*)p + i * 8 + 0x150);
            func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(s), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
        }
    }
    if (p->field_208) {
        if (p->mLayout1FC)
            func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(p->mLayout1FC), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
        if (p->mLayout32D4 && p->field_32E5)
            func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(p->mLayout32D4), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
        if (p->field_204 && p->mLayout32EC && p->field_32FD)
            func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(p->mLayout32EC), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
        if ((s8)p->field_206 >= 0) {
            int cond;
            if (!p->mLayout32D4) {
                cond = 0;
            } else {
                void** vtable = *(void***)(*(void**)((u8*)p->mLayout32D4 + 0x10));
                void* result = ((void*(*)(void*, const char*, u32))vtable[15])((void*)p->mLayout32D4, &lbl_eu_8050BEA8[0xEE], 1);
                cond = (*(u8*)((u8*)result + 0xBB)) & 1;
            }
            if (!cond) {
                s8 idx = (s8)p->field_205;
                if (*(u8*)((u8*)p + idx * 0x30C + 0x514)) {
                    if (p->mLayout3304 && p->field_3315)
                        func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(p->mLayout3304), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
                }
            }
        }
        if (p->mLayout331C && p->field_332D)
            func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(p->mLayout331C), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
    }
    if (p->field_208)
        func_801F35B0((void*)p->mScrollBar, drawInfo);
    if (p->field_333C && p->mLayout3334)
        func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(p->mLayout3334), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
    func_8022B7C8((void*)p->mSysWinB8, drawInfo);
    func_8022B7C8((void*)p->mSysWinF4, drawInfo);
    if ((s8)p->field_5C >= 0)
        func_801D20B0((void*)p->mCursor, drawInfo);
}

// Teardown of the floor-map screen (paired with the ctor-time resource
// loads). Frees the three file handles; when the draw flag is set it also
// releases every layout/resource/widget created for the screen and clears
// each owning pointer.
void func_8024CB94(CFloorMapFull* p) {
    func_8003AA8C__5CBdatFUl(2);
    func_801390E0(reinterpret_cast<CFileHandle**>(&p->field_24));
    func_801390E0(reinterpret_cast<CFileHandle**>(&p->field_28));
    func_801390E0(reinterpret_cast<CFileHandle**>(&p->field_2C));

    if (p->field_40) {
        p->field_40 = 0;
        p->field_42 = 0;
        waitForDrawDone__9CDeviceVIFv();

        // Free the marker-name buffer and the layout arc buffer.
        if (p->field_3340 != NULL) {
            mtl::MemManager::deallocate(p->field_3340);
            p->field_3340 = NULL;
        }
        if (p->field_3C != 0) {
            mtl::MemManager::deallocate((void*)p->field_3C);
            p->field_3C = 0;
        }

        // Release both arc resource accessors and the two scratch regions.
        func_80139124(reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(p->field_34));
        func_80139124(reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(p->field_38));
        reinterpret_cast<UnkClass_8045F564*>((u8*)p + 0x04)->func_8045F778();
        reinterpret_cast<UnkClass_8045F564*>((u8*)p + 0x14)->func_8045F778();

        func_801F35DC((void*)p->mScrollBar);
        ((CCur18View*)&p->mCursor)->v01();
        func_8022B7F4((void*)&p->mSysWinB8);
        func_8022B7F4((void*)&p->mSysWinF4);

        // Destroy each owned layout (double null check is the retail D2-
        // inlined deleting-dtor shape) and clear the slot.
        if (p->mLayout130 != 0) {
            if (p->mLayout130 != 0)
                ((CFloorMapLayoutDtorVt*)p->mLayout130)->destroy(1);
            p->mLayout130 = 0;
        }
        if (p->mLayout138 != 0) {
            if (p->mLayout138 != 0)
                ((CFloorMapLayoutDtorVt*)p->mLayout138)->destroy(1);
            p->mLayout138 = 0;
        }

        func_80246908(&p->mLayout140);

        for (u32 i = 0; i < 0x14; i++) {
            u32* slot = &p->mLayouts150[i].mPtr;
            if (*slot != 0) {
                if (*slot != 0)
                    ((CFloorMapLayoutDtorVt*)*slot)->destroy(1);
                *slot = 0;
            }
        }

        if (p->mLayout1FC != 0) {
            if (p->mLayout1FC != 0)
                ((CFloorMapLayoutDtorVt*)p->mLayout1FC)->destroy(1);
            p->mLayout1FC = 0;
        }
        if (p->mLayout32D4 != 0) {
            if (p->mLayout32D4 != 0)
                ((CFloorMapLayoutDtorVt*)p->mLayout32D4)->destroy(1);
            p->mLayout32D4 = 0;
        }
        if (p->mLayout32EC != 0) {
            if (p->mLayout32EC != 0)
                ((CFloorMapLayoutDtorVt*)p->mLayout32EC)->destroy(1);
            p->mLayout32EC = 0;
        }
        if (p->mLayout3304 != 0) {
            if (p->mLayout3304 != 0)
                ((CFloorMapLayoutDtorVt*)p->mLayout3304)->destroy(1);
            p->mLayout3304 = 0;
        }
        if (p->mLayout331C != 0) {
            if (p->mLayout331C != 0)
                ((CFloorMapLayoutDtorVt*)p->mLayout331C)->destroy(1);
            p->mLayout331C = 0;
        }
        if (p->mLayout3334 != 0) {
            if (p->mLayout3334 != 0)
                ((CFloorMapLayoutDtorVt*)p->mLayout3334)->destroy(1);
            p->mLayout3334 = 0;
        }
    }
}

u8 func_8024CE1C(void* self) {
    extern int CScrollBar_isVisible(void*);
    if (CScrollBar_isVisible((u8*)self + 0x60)) {
        return *(u8*)((u8*)self + 0x42);
    }
    return 0;
}


// Move the map cursor to the previous floor: decrement the floor index
// (wrapping to the last floor), rebuild the floor-name panes, set the
// scrollbar range, then (re)load the floor's map file.
// NOTE: the per-floor record address is deliberately recomputed at each use
// (retail never keeps it in a callee-saved register across the calls).
#define FM_FLOOR(p_) ((CFloorMapFloorBlock*)((u8*)(p_) + (s8)(p_)->field_205 * 0x30C))
void func_8024CE68(void* self) {
    extern int sprintf(char*, const char*, ...);
    CFloorMapFull* p = (CFloorMapFull*)self;
    nw4r::math::VEC3 pos;
    f32 scl[3];
    char nameBuf[0x20];
    char fname[0x20];
    if (!p->field_208) return;
    if (p->field_58) return;
    if (CSysWin_getUnk34(&p->mSysWinB8)) return;
    if (CSysWin_getUnk34(&p->mSysWinF4)) return;

    u8 nf = (u8)(p->field_205 - 1);
    p->field_205 = nf;
    if ((s8)nf < 0) {
        p->field_205 = (u8)(p->field_32CC - 1);
    }

    p->field_206 = -1;
    p->field_207 = 0;
    sprintf(nameBuf, &lbl_eu_8050BEA8[0x12A], (s8)p->field_205 + 1);

    func_801375A0(&pos, ((nw4r::lyt::Layout*)p->mLayout1FC)->GetRootPane()->FindPaneByName(nameBuf, 1));

    pos.x *= ((nw4r::lyt::Layout*)p->mLayout1FC)->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1)->GetScale().x;
    if (p->mLayout32D4) {
        CFloorMapSetPanePos(((nw4r::lyt::Layout*)p->mLayout32D4)->GetRootPane(), pos);
    }

    func_80136B4C((nw4r::lyt::Layout*)p->mLayout32D4, &lbl_eu_8050BEA8[0xF8], FM_FLOOR(p)->name_210, 0);
    func_80136B4C((nw4r::lyt::Layout*)p->mLayout1FC, &lbl_eu_8050BEA8[0x145], func_8013639C((const void*)lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1F], lbl_eu_80664798), 0);
    func_80246200(&p->mLayout1FC);

    if (p->field_3330) {
        scl[0] = lbl_eu_80668768;
        scl[1] = lbl_eu_8066876C;
        scl[2] = lbl_eu_80668764;
        func_801F3670((void*)p->field_3330, scl);
        func_801F36BC((void*)p->field_3330, 5, FM_FLOOR(p)->field_514);
        func_801F3850((void*)p->field_3330, 0);
    }

    ((nw4r::lyt::Layout*)p->mLayout32D4)->SetAnimationEnable((nw4r::lyt::AnimTransform*)p->mLayout32DC, false);
    ((nw4r::lyt::Layout*)p->mLayout32D4)->SetAnimationEnable((nw4r::lyt::AnimTransform*)p->mLayout32E0, true);
    p->field_32E4 = 1;
    func_801390E0__FPP11CFileHandle((CFileHandle**)&p->field_2C);

    u8 lang = FM_FLOOR(p)->field_20C;
    if (lang == 0) lang = 1;

    const char* msgTbl = lbl_eu_8050BEA8;
    u16 rate1 = func_80136254((const void*)lbl_eu_8066479C, msgTbl + 0x4AF, lang);
    u16 rate2 = func_80136254((const void*)lbl_eu_8066479C, msgTbl + 0x4B5, lang);
    u8 rate3 = func_801361E8(lbl_eu_8066479C, msgTbl + 0x4BC, lang);

    // Pick which map variant to load: weighted by the floor table's three
    // rates (rate1/rate2/rate3) against two random draws.
    int mode;
    if (rate1 != 0 && func_8009CF8C(0x20) >= rate1) {
        if (rate2 == 0) {
            mode = 1;
        } else if (func_8009CF8C((u32)rate2 + 0x220) < rate3) {
            mode = 1;
        } else {
            mode = 2;
        }
    } else {
        if (rate2 == 0) {
            mode = 0;
        } else if (func_8009CF8C((u32)rate2 + 0x220) < rate3) {
            mode = 0;
        } else {
            mode = 1;
        }
    }

    const char* key = NULL;
    switch (mode) {
    case 0: key = msgTbl + 0x4C1; break;
    case 1: key = msgTbl + 0x4C8; break;
    case 2: key = msgTbl + 0x4D0; break;
    }

    sprintf(fname, msgTbl + 0x4D8, func_80138F78((u16)func_80136254((const void*)lbl_eu_8066479C, key, lang)));
    p->field_2C = (u32)readFile__11CDeviceFileFUlPCcP10IWorkEventii((u32)getHandleMEM2__Q23mtl10MemManagerFv(), fname, p, 0, 0);
    func_80138078(0xC);
}
#undef FM_FLOOR

// Move the map cursor to the next floor: increment the floor index (wrapping
// to the first floor), then refresh the floor name panes / scrollbar and
// (re)load the floor's map file.
void func_8024D23C(void* self) {
    CFloorMapFull* p = (CFloorMapFull*)self;
    if (!p->field_208) return;
    if (p->field_58) return;
    if (CSysWin_getUnk34(&p->mSysWinB8)) return;
    if (CSysWin_getUnk34(&p->mSysWinF4)) return;

    // Advance the floor index, wrapping back to the first floor.
    u8 nf = (u8)(p->field_205 + 1);
    p->field_205 = nf;
    if ((s32)nf >= (s32)p->field_32CC) {
        p->field_205 = 0;
    }

    p->field_206 = -1;
    p->field_207 = 0;

    char* tbl = lbl_eu_8050BEA8;
    p->field_206 = -1;
    p->field_207 = 0;
    char buf[0x20];
    sprintf(buf, &tbl[0x12A], (s8)p->field_205 + 1);

    nw4r::lyt::Pane* pane =
        ((nw4r::lyt::Layout*)p->mLayout1FC)->GetRootPane()->FindPaneByName(buf, 1);
    nw4r::math::_VEC3 pos;
    func_801375A0((nw4r::math::VEC3*)&pos, pane);

    nw4r::lyt::Pane* scalePane =
        ((nw4r::lyt::Layout*)p->mLayout1FC)->GetRootPane()->FindPaneByName(&tbl[0x136], 1);
    // Word-wise move of y/z into the by-value argument slots so MWCC keeps
    // the retail integer-register copies while x is rescaled.
    u32 py = ((u32*)&pos)[1];
    u32 pz = ((u32*)&pos)[2];
    pos.x = pos.x * scalePane->GetScale().x;
    nw4r::math::_VEC3 dest;
    ((u32*)&dest)[1] = py;
    if (p->mLayout32D4) {
        ((u32*)&dest)[0] = ((u32*)&pos)[0];
        ((u32*)&dest)[2] = pz;
        CFloorMapSetPanePos(((nw4r::lyt::Layout*)p->mLayout32D4)->GetRootPane(), dest);
    }

    // The floor block is never cached; retail recomputes self + fi*0x30C
    // for each field access while keeping only the field_20C byte live.
    func_80136B4C((nw4r::lyt::Layout*)p->mLayout32D4, &tbl[0xF8],
                  ((CFloorMapFloorBlock*)((u8*)p + (s8)p->field_205 * 0x30C))->name_210, 0);
    func_80136B4C((nw4r::lyt::Layout*)p->mLayout1FC, &tbl[0x145],
                  func_8013639C((const void*)lbl_eu_806640A8, &tbl[0x1F], lbl_eu_80664798), 0);
    func_80246200(&p->mLayout1FC);

    if (p->field_3330) {
        f32 vec[3] = {lbl_eu_80668768, lbl_eu_8066876C, lbl_eu_80668764};
        func_801F3670((void*)p->field_3330, vec);
        func_801F36BC((void*)p->field_3330, 5,
                      ((CFloorMapFloorBlock*)((u8*)p + (s8)p->field_205 * 0x30C))->field_514);
        func_801F3850((void*)p->field_3330, 0);
    }

    ((nw4r::lyt::Layout*)p->mLayout32D4)->SetAnimationEnable((nw4r::lyt::AnimTransform*)p->mLayout32DC, false);
    ((nw4r::lyt::Layout*)p->mLayout32D4)->SetAnimationEnable((nw4r::lyt::AnimTransform*)p->mLayout32E0, true);
    p->field_32E4 = 1;
    func_801390E0__FPP11CFileHandle((CFileHandle**)&p->field_2C);

    u8 v26 = ((CFloorMapFloorBlock*)((u8*)p + (s8)p->field_205 * 0x30C))->field_20C;
    if (v26 == 0) v26 = 1;

    u16 v1 = func_80136254((const void*)lbl_eu_8066479C, &lbl_eu_8050BEA8[0x4AF], v26);
    u16 v2 = func_80136254((const void*)lbl_eu_8066479C, &lbl_eu_8050BEA8[0x4B5], v26);
    u8 v3 = func_801361E8(lbl_eu_8066479C, &lbl_eu_8050BEA8[0x4BC], v26);

    int mode;
    if (v1 != 0 && func_8009CF8C(0x20) >= v1) {
        if (v2 != 0 && func_8009CF8C((u32)v2 + 0x220) >= v3) {
            mode = 2;
        } else {
            mode = 1;
        }
    } else {
        if (v2 != 0 && func_8009CF8C((u32)v2 + 0x220) >= v3) {
            mode = 1;
        } else {
            mode = 0;
        }
    }

    char* key;
    if (mode == 0) {
        key = &lbl_eu_8050BEA8[0x4C1];
    } else if (mode == 1) {
        key = &lbl_eu_8050BEA8[0x4C8];
    } else {
        key = &lbl_eu_8050BEA8[0x4D0];
    }

    char fname[0x20];
    sprintf(fname, &lbl_eu_8050BEA8[0x4D8], func_80138F78((u16)func_80136254((const void*)lbl_eu_8066479C, key, v26)));
    p->field_2C = (u32)readFile__11CDeviceFileFUlPCcP10IWorkEventii((u32)getHandleMEM2__Q23mtl10MemManagerFv(), fname, p, 0, 0);
    func_80138078(0xA);
}

// Per-frame map update: when the syswin cursor is active, step the cursor
// index (wrapping to 1); otherwise move the map view to the row id under the
// cursor (same position computation as func_8024F7CC).
void func_8024D614(void* self) {
    extern int sprintf(char*, const char*, ...);
    CFloorMapFull* p = (CFloorMapFull*)self;
    // Stack temps mirroring retail's frame layout; pos is declared first
    // after self so its address colors into r30 like retail.
    nw4r::math::VEC3 pos;
    nw4r::math::VEC3 out;
    nw4r::math::VEC3 rec;
    nw4r::math::VEC3 acc3;
    nw4r::math::VEC3 acc2;
    nw4r::math::VEC3 acc;
    char buf[0x20];
    // Hoisted above every goto target so no jump crosses an initializer.
    CFloorMapLayoutSlots* slots;
    nw4r::lyt::Pane* root;
    nw4r::lyt::Pane* pane;
    nw4r::lyt::Pane* parent;
    nw4r::lyt::Pane* gp;
    nw4r::lyt::Pane* ggp;
    if (p->field_2C) return;
    if (!p->field_208) return;
    if (CSysWin_getUnk34(&p->mSysWinF4)) return;

    if (p->field_58) {
        if (!CSysWin_isActive(&p->mSysWinB8)) return;

        // Step down, clamping at 1 (never below).
        u8 nc = (u8)(p->field_5C - 1);
        p->field_5C = nc;
        if ((s8)nc < 0) p->field_5C = 1;

        nw4r::math::VEC3 curOut;
        func_8022C1B4(&curOut, &p->mSysWinB8, p->field_5C);
        ((CCur18View*)&p->mCursor)->v02(&curOut);
        func_80138078(1);
        return;
    }

    s8 fi = (s8)p->field_205;
    CFloorMapFloorBlock* floor = (CFloorMapFloorBlock*)((u8*)p + fi * 0x30C);
    if (!floor->field_514) return;

    func_802452C4(&p->mLayout1FC);

    u16 rowId;
    s8 rowIdx = (s8)p->field_206;
    if (rowIdx >= 0) {
        rowId = ((CFloorMapFloorBlock*)((u8*)p + (s8)p->field_205 * 0x30C))->rows[(s8)p->field_207 + rowIdx].field_00;
    } else {
        rowId = 0;
    }
    if (!rowId) return;

    pos.x = lbl_eu_80668764;
    pos.y = lbl_eu_80668764;
    pos.z = lbl_eu_80668764;
    // Retail keeps the layout-slot pointer and &pos in nonvolatiles across
    // the sprintf call, so slot is assigned before it.
    slots = (CFloorMapLayoutSlots*)&p->mLayout140;
    sprintf(buf, &lbl_eu_8050BEA8[0x2EB]);

    root = slots->layout->GetRootPane();
    pane = root->FindPaneByName(buf, 1);
    // Retail tests the FindPaneByName result on cr1 at the call return and
    // wraps the whole tail (zero-init, walk, pos copy, scale lookup) inside
    // the non-NULL branch; a NULL pane jumps straight to the divides block.
    if (pane != NULL) {
        out.x = lbl_eu_80668764;
        out.y = lbl_eu_80668764;
        out.z = lbl_eu_80668764;
        if (pane != root) {
            out.x = pane->GetTranslate().x;
            out.y = pane->GetTranslate().y;
            out.z = pane->GetTranslate().z;

            parent = pane->GetParent();
            acc.x = lbl_eu_80668764;
            acc.y = lbl_eu_80668764;
            acc.z = lbl_eu_80668764;
            if (root != parent && parent != NULL) {
                acc.x = parent->GetTranslate().x;
                acc.y = parent->GetTranslate().y;
                acc.z = parent->GetTranslate().z;

                gp = parent->GetParent();
                acc2.x = lbl_eu_80668764;
                acc2.y = lbl_eu_80668764;
                acc2.z = lbl_eu_80668764;
                if (root != gp && gp != NULL) {
                    acc2.x = gp->GetTranslate().x;
                    acc2.y = gp->GetTranslate().y;
                    acc2.z = gp->GetTranslate().z;

                    ggp = gp->GetParent();
                    acc3.x = lbl_eu_80668764;
                    acc3.y = lbl_eu_80668764;
                    acc3.z = lbl_eu_80668764;
                    if (root != ggp && ggp != NULL) {
                        acc3.x = ggp->GetTranslate().x;
                        acc3.y = ggp->GetTranslate().y;
                        acc3.z = ggp->GetTranslate().z;
                        // Deepest level: no null check on the fourth ancestor;
                        // func_8024B4CC handles it internally.
                        func_8024B4CC(&rec, slots, ggp->GetParent());
                        nw4r::math::VEC3Add(&acc3, &acc3, &rec);
                    }
                    goto add_acc2;
                }
                goto add_acc;
            }
            goto add_out;
        add_acc2:
            nw4r::math::VEC3Add(&acc2, &acc2, &acc3);
        add_acc:
            nw4r::math::VEC3Add(&acc, &acc, &acc2);
        add_out:
            nw4r::math::VEC3Add(&out, &out, &acc);
        }
        pos.x = out.x;
        pos.y = out.y;
        pos.z = out.z;

        nw4r::lyt::Pane* scalePane =
            slots->layout->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);
        pos.x *= scalePane->GetScale().x;
        pos += slots->layout->GetRootPane()->GetTranslate();
    }
divides:

    p->field_54 = lbl_eu_806687A0;
    p->field_41 = 2;
    p->field_4C = pos.x / lbl_eu_806687A0;
    p->field_50 = pos.y / lbl_eu_806687A0;
    func_80138078(1);
}

// Per-frame map update for the world-map view: when the syswin cursor is
// active, step the cursor index (wrapping to 0); otherwise advance the map
// rows and move the map view to the row under the cursor (same position
// computation as func_8024F7CC).
void func_8024DA0C(void* self) {
    extern int sprintf(char*, const char*, ...);
    CFloorMapFull* p = (CFloorMapFull*)self;
    char buf[0x20];
    nw4r::math::VEC3 pos;
    nw4r::math::VEC3 cursorOut;
    if (p->field_2C) return;
    if (!p->field_208) return;
    if (CSysWin_getUnk34(&p->mSysWinF4)) return;

    if (p->field_58) {
        if (!CSysWin_isActive(&p->mSysWinB8)) return;

        // Wrap the cursor index 0 -> 1 -> 0 (syswin cursor mode); the wrap
        // test reads the freshly stored byte back.
        p->field_5C = p->field_5C + 1;
        if ((s8)p->field_5C >= 2) p->field_5C = 0;

        func_8022C1B4(&cursorOut, &p->mSysWinB8, p->field_5C);
        ((CCur18View*)&p->mCursor)->v02(&cursorOut);
        func_80138078(1);
        return;
    }

    CFloorMapFloorBlock* floor = (CFloorMapFloorBlock*)((u8*)p + (s8)p->field_205 * 0x30C);
    if (!floor->field_514) return;

    func_80245450(&p->mLayout1FC);

    u16 rowId = 0;
    if ((s8)p->field_206 >= 0) {
        rowId = ((CFloorMapFloorBlock*)((u8*)p + (s8)p->field_205 * 0x30C))
                    ->rows[p->field_207 + (s8)p->field_206].field_00;
    }

    pos.x = lbl_eu_80668764;
    pos.y = lbl_eu_80668764;
    pos.z = lbl_eu_80668764;
    // Retail keeps this slot pointer and &pos in nonvolatiles across the
    // sprintf call below.
    CFloorMapLayoutSlots* slots = (CFloorMapLayoutSlots*)&p->mLayout140;
    sprintf(buf, &lbl_eu_8050BEA8[0x2EB], rowId);

    // The layout root is read once and kept in a register for the ancestor
    // comparisons; the layout pointer itself is re-read per call.
    nw4r::lyt::Pane* root = slots->layout->GetRootPane();
    nw4r::lyt::Pane* pane = root->FindPaneByName(buf, 1);
    if (pane != NULL) {
        // Accumulate the pane's parent-chain translates (excluding the layout
        // root); each level that terminates early leaves its accumulator at
        // {0,0,0}.
        nw4r::math::VEC3 out;
        out.x = lbl_eu_80668764;
        out.y = lbl_eu_80668764;
        out.z = lbl_eu_80668764;
        if (pane != root) {
            nw4r::lyt::Pane* parent = pane->GetParent();
            out.x = pane->GetTranslate().x;
            out.y = pane->GetTranslate().y;
            out.z = pane->GetTranslate().z;

            nw4r::math::VEC3 acc;
            acc.x = lbl_eu_80668764;
            acc.y = lbl_eu_80668764;
            acc.z = lbl_eu_80668764;
            if (parent != root && parent != NULL) {
                nw4r::lyt::Pane* gp = parent->GetParent();
                acc.x = parent->GetTranslate().x;
                acc.y = parent->GetTranslate().y;
                acc.z = parent->GetTranslate().z;

                nw4r::math::VEC3 acc2;
                acc2.x = lbl_eu_80668764;
                acc2.y = lbl_eu_80668764;
                acc2.z = lbl_eu_80668764;
                if (gp != root && gp != NULL) {
                    nw4r::lyt::Pane* ggp = gp->GetParent();
                    acc2.x = gp->GetTranslate().x;
                    acc2.y = gp->GetTranslate().y;
                    acc2.z = gp->GetTranslate().z;

                    nw4r::math::VEC3 acc3;
                    acc3.x = lbl_eu_80668764;
                    acc3.y = lbl_eu_80668764;
                    acc3.z = lbl_eu_80668764;
                    if (ggp != root && ggp != NULL) {
                        nw4r::math::VEC3 rec;
                        acc3.x = ggp->GetTranslate().x;
                        acc3.y = ggp->GetTranslate().y;
                        acc3.z = ggp->GetTranslate().z;
                        // Deepest level: no null check on the fourth
                        // ancestor; func_8024B4CC handles it internally.
                        func_8024B4CC(&rec, slots, ggp->GetParent());
                        nw4r::math::VEC3Add(&acc3, &acc3, &rec);
                    }
                    nw4r::math::VEC3Add(&acc2, &acc2, &acc3);
                }
                nw4r::math::VEC3Add(&acc, &acc, &acc2);
            }
            nw4r::math::VEC3Add(&out, &out, &acc);
        }
        pos.x = out.x;
        pos.y = out.y;
        pos.z = out.z;

        nw4r::lyt::Pane* scalePane = slots->layout->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);
        pos.x *= scalePane->GetScale().x;
        pos += slots->layout->GetRootPane()->GetTranslate();
    }

    p->field_54 = lbl_eu_806687A0;
    p->field_41 = 2;
    p->field_4C = pos.x / lbl_eu_806687A0;
    p->field_50 = pos.y / lbl_eu_806687A0;
    func_80138078(1);
}

// Per-frame map-down handler: when the cursor is not active, step the row
// index down by one row (wrapping the page), refresh the cursor pane, move
// the map view to the row under the cursor, and normalize the new position.
void func_8024DE08(CFloorMapFull* p) {
    // Declared in retail frame-slot order (MWCC assigns first-declared the
    // highest address): buf@0x88, buf2@0x68, vec@0x5C, pos@0x50, dest@0x44,
    // acc@0x38, acc2@0x2C, acc3@0x20, rec@0x14, out@0x08.
    char buf[0x20];
    char buf2[0x20];
    nw4r::math::VEC3 vec;
    nw4r::math::_VEC3 pos;
    nw4r::math::_VEC3 dest;
    nw4r::math::VEC3 acc;
    nw4r::math::VEC3 acc2;
    nw4r::math::VEC3 acc3;
    nw4r::math::VEC3 rec;
    nw4r::math::VEC3 out;

    if (p->field_2C) return;
    if (!p->field_208) return;
    if (CSysWin_getUnk34(&p->mSysWinF4)) return;
    if (p->field_58) return;

    s8 fi = (s8)p->field_205;
    CFloorMapFloorBlock* floor = (CFloorMapFloorBlock*)((u8*)p + fi * 0x30C);
    u8 rowCount = floor->field_514;
    if (!rowCount) return;

    // Step the row index down; when it wraps below zero, move to the next
    // page (up to four pages) and clear the row index.
    if (rowCount >= 5) {
        u8 v = (u8)(p->field_207 - 5);
        p->field_207 = v;
        if ((s8)(u8)v < 0) {
            u8 w = (u8)((u8)v + 4);
            p->field_206 = (s8)w;
            p->field_207 = 0;
            if ((s8)(u8)w < 0) p->field_206 = 0;
        }
    } else {
        p->field_206 = 0;
        p->field_207 = 0;
    }

    func_80246200(&p->mLayout1FC);

    const char* tbl = lbl_eu_8050BEA8;
    // Refresh the cursor position: format the pane name for the current page
    // index, scale its translate by the zoom pane, and move the target pane.
    sprintf(buf, &tbl[0x14D], (s8)p->field_206);

    nw4r::lyt::Pane* pane =
        ((nw4r::lyt::Layout*)p->mLayout1FC)->GetRootPane()->FindPaneByName(buf, 1);
    func_801375A0((nw4r::math::VEC3*)&pos, pane);
    nw4r::lyt::Pane* scalePane =
        ((nw4r::lyt::Layout*)p->mLayout1FC)->GetRootPane()->FindPaneByName(&tbl[0x136], 1);
    pos.x = pos.x * scalePane->GetScale().x;

    // Word-wise y/z copy so MWCC keeps the retail integer-register moves;
    // the scaled x lands in dest last.
    ((u32*)&dest)[1] = ((u32*)&pos)[1];
    ((u32*)&dest)[2] = ((u32*)&pos)[2];
    dest.x = pos.x;

    if (p->mLayout3304)
        ((CFloorMapLayoutHolder*)p->mLayout3304)->pane->SetTranslate(
            *(nw4r::math::VEC3*)&dest);

    func_801F3850((void*)p->field_3330, (u16)(s8)p->field_207);

    // Resolve the map-row id under the cursor (0 when the page index wrapped).
    u16 rowId = 0;
    if ((s8)p->field_206 >= 0) {
        rowId = ((CFloorMapFloorBlock*)((u8*)p + (s8)p->field_205 * 0x30C))
                    ->rows[(s8)p->field_207 + (s8)p->field_206].field_00;
    }

    // Move the map view to the row under the cursor: accumulate the row
    // pane's parent-chain translate (excluding the layout root), scale by the
    // zoom pane and add the root offset.
    vec.x = lbl_eu_80668764;
    vec.y = lbl_eu_80668764;
    vec.z = lbl_eu_80668764;
    CFloorMapLayoutSlots* slots = (CFloorMapLayoutSlots*)&p->mLayout140;
    sprintf(buf2, &tbl[0x2EB], rowId);

    nw4r::lyt::Pane* pane2 = slots->layout->GetRootPane()->FindPaneByName(buf2, 1);
    nw4r::lyt::Pane* parent;
    nw4r::lyt::Pane* gp;
    nw4r::lyt::Pane* ggp;
    nw4r::lyt::Pane* scalePane2;
    if (pane2 == NULL) goto tail;

    nw4r::lyt::Pane* target = slots->layout->GetRootPane();
    out.x = lbl_eu_80668764;
        out.y = lbl_eu_80668764;
        out.z = lbl_eu_80668764;
        if (target == pane2 || pane2 == NULL) goto set_pos;
        out.x = pane2->GetTranslate().x;
        out.y = pane2->GetTranslate().y;
        out.z = pane2->GetTranslate().z;

        parent = pane2->GetParent();
        acc.x = lbl_eu_80668764;
        acc.y = lbl_eu_80668764;
        acc.z = lbl_eu_80668764;
        if (target == parent || parent == NULL) goto add_out;
        acc.x = parent->GetTranslate().x;
        acc.y = parent->GetTranslate().y;
        acc.z = parent->GetTranslate().z;

        gp = parent->GetParent();
        acc2.x = lbl_eu_80668764;
        acc2.y = lbl_eu_80668764;
        acc2.z = lbl_eu_80668764;
        if (target == gp || gp == NULL) goto add_acc;
        acc2.x = gp->GetTranslate().x;
        acc2.y = gp->GetTranslate().y;
        acc2.z = gp->GetTranslate().z;

        ggp = gp->GetParent();
        acc3.x = lbl_eu_80668764;
        acc3.y = lbl_eu_80668764;
        acc3.z = lbl_eu_80668764;
        if (target == ggp || ggp == NULL) goto add_acc2;
        acc3.x = ggp->GetTranslate().x;
        acc3.y = ggp->GetTranslate().y;
        acc3.z = ggp->GetTranslate().z;
        func_8024B4CC(&rec, slots, ggp->GetParent());
        nw4r::math::VEC3Add(&acc3, &acc3, &rec);
    add_acc2:
        nw4r::math::VEC3Add(&acc2, &acc2, &acc3);
    add_acc:
        nw4r::math::VEC3Add(&acc, &acc, &acc2);
    add_out:
        nw4r::math::VEC3Add(&out, &out, &acc);
    set_pos:
        vec = out;

        scalePane2 =
            slots->layout->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);
        vec.x *= scalePane2->GetScale().x;
        nw4r::math::VEC3Add(&vec, &vec,
                            (nw4r::math::VEC3*)&slots->layout->GetRootPane()->GetTranslate());
    tail:
    p->field_54 = lbl_eu_806687A0;
    p->field_41 = 2;
    p->field_4C = vec.x / lbl_eu_806687A0;
    p->field_50 = vec.y / lbl_eu_806687A0;
    func_80138078(1);
}

// Scroll the floor-map cursor view down to the selected row: reset the
// cursor, resolve the selected row's pane by name, accumulate its parent-chain
// translate (scaled by the map zoom pane) and store the normalized position.
// The tail normalization always runs; when the pane is missing, pos simply
// stays at the zero it was seeded with before the name lookup.
void func_8024E2BC(void* self) {
    CFloorMapFull* p = (CFloorMapFull*)self;
    if (p->field_2C != 0) return;
    if (p->field_208 == 0) return;
    if (CSysWin_getUnk34(&p->mSysWinF4)) return;
    if (p->field_58 != 0) return;

    if (*(u8*)((u8*)p + (s8)p->field_205 * 0x30C + 0x514) == 0) return;
    func_802455F0(&p->mLayout1FC);

    // Locals in retail frame-slot order (buf@0x50, pos@0x44, acc@0x38,
    // acc2@0x2C, acc3@0x20, rec@0x14, out@0x08).
    char buf[0x20];
    nw4r::math::VEC3 pos;
    nw4r::math::VEC3 acc;
    nw4r::math::VEC3 acc2;
    nw4r::math::VEC3 acc3;
    nw4r::math::VEC3 rec;
    nw4r::math::VEC3 out;
    // Address locals seeded before the calls: the translate adds consume
    // these through the ps-pair helpers, and retail keeps the pointers in
    // callee-saved registers across the lookups.
    nw4r::math::VEC3* posP = &pos;
    nw4r::math::VEC3* outP = &out;

    // Row id of the selected marker row; 0 while no row is selected.
    u16 rowId = 0;
    if ((s8)p->field_206 >= 0)
        rowId = *(u16*)(
            (u8*)p + (s8)p->field_205 * 0x30C +
            ((s8)p->field_207 + (s8)p->field_206) * 0x18 + 0x214);

    // pos is zeroed before the sprintf: its frame slots back the skipped-path
    // value consumed by the tail below.
    pos.x = lbl_eu_80668764;
    pos.y = lbl_eu_80668764;
    pos.z = lbl_eu_80668764;
    sprintf(buf, &lbl_eu_8050BEA8[0x2EB], rowId);

    CFloorMapLayoutSlots* slots = (CFloorMapLayoutSlots*)&p->mLayout140;
    nw4r::lyt::Pane* pane = slots->layout->GetRootPane()->FindPaneByName(buf, 1);
    if (pane != NULL) {
        // Accumulate the pane's parent-chain translate (up to the layout
        // root); each level starts from zero so the early exits keep the sum
        // correct.
        nw4r::lyt::Pane* target = slots->layout->GetRootPane();
        out.x = lbl_eu_80668764;
        out.y = lbl_eu_80668764;
        out.z = lbl_eu_80668764;
        if (target != pane) {
            out.x = pane->GetTranslate().x;
            out.y = pane->GetTranslate().y;
            out.z = pane->GetTranslate().z;

            nw4r::lyt::Pane* parent = pane->GetParent();
            acc.x = lbl_eu_80668764;
            acc.y = lbl_eu_80668764;
            acc.z = lbl_eu_80668764;
            if (target != parent && parent != NULL) {
                acc.x = parent->GetTranslate().x;
                acc.y = parent->GetTranslate().y;
                acc.z = parent->GetTranslate().z;

                nw4r::lyt::Pane* gp = parent->GetParent();
                acc2.x = lbl_eu_80668764;
                acc2.y = lbl_eu_80668764;
                acc2.z = lbl_eu_80668764;
                if (target != gp && gp != NULL) {
                    acc2.x = gp->GetTranslate().x;
                    acc2.y = gp->GetTranslate().y;
                    acc2.z = gp->GetTranslate().z;

                    nw4r::lyt::Pane* ggp = gp->GetParent();
                    acc3.x = lbl_eu_80668764;
                    acc3.y = lbl_eu_80668764;
                    acc3.z = lbl_eu_80668764;
                    if (target != ggp && ggp != NULL) {
                        acc3.x = ggp->GetTranslate().x;
                        acc3.y = ggp->GetTranslate().y;
                        acc3.z = ggp->GetTranslate().z;
                        func_8024B4CC(&rec, slots, ggp->GetParent());
                        nw4r::math::VEC3Add(&acc3, &acc3, &rec);
                    }
                    nw4r::math::VEC3Add(&acc2, &acc2, &acc3);
                }
                nw4r::math::VEC3Add(&acc, &acc, &acc2);
            }
            nw4r::math::VEC3Add(outP, outP, &acc);
        }

        pos = out;
        nw4r::lyt::Pane* scalePane =
            slots->layout->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);
        pos.x *= scalePane->GetScale().x;
        nw4r::math::VEC3Add(
            posP, posP,
            (nw4r::math::VEC3*)&slots->layout->GetRootPane()->GetTranslate());
    }

    ((CFloorMapFull*)self)->field_54 = lbl_eu_806687A0;
    ((CFloorMapFull*)self)->field_41 = 2;
    ((CFloorMapFull*)self)->field_4C = pos.x / lbl_eu_806687A0;
    ((CFloorMapFull*)self)->field_50 = pos.y / lbl_eu_806687A0;
    func_80138078(1);
}

// Scroll the map view down: step the cursor position toward the lower bound
// and push the new position into every map layout's target pane.
void func_8024E650(void* self) {
    CFloorMapFull* fm = (CFloorMapFull*)self;
    if (fm->field_58) return;
    if (CSysWin_getUnk34(&fm->mSysWinB8)) return;
    if (CSysWin_getUnk34(&fm->mSysWinF4)) return;
    f32 min = lbl_eu_806687A4;
    f32 y = fm->pos_y_48 - lbl_eu_806687A8;
    fm->pos_y_48 = y;
    if (y <= min) fm->pos_y_48 = min;

    nw4r::math::_VEC3 vec = { fm->pos_x_44, fm->pos_y_48, lbl_eu_80668764 };

    CFloorMapSetSlotPos((CFloorMapLayoutHolder*)fm->mLayout130, vec);
    CFloorMapSetSlotPos((CFloorMapLayoutHolder*)fm->mLayout138, vec);
    CFloorMapSetSlotPos((CFloorMapLayoutHolder*)fm->mLayout140, vec);
    if (fm->mLayouts150[0].mPtr) {
        for (u8 i = 0; i < fm->field_1F0; i++) {
            CFloorMapSetPanePos(((CFloorMapLayoutHolder*)fm->mLayouts150[i].mPtr)->pane, vec);
        }
    }

    func_8024577C(&fm->mLayout1FC, (u16)func_80248558(&fm->mLayout140));
    fm->field_5D = 1;
}

// Scroll the map view up: step the cursor position toward the upper bound
// and push the new position into every map layout's target pane.
void func_8024E828(void* self) {
    CFloorMapFull* fm = (CFloorMapFull*)self;
    if (fm->field_58) return;
    if (CSysWin_getUnk34(&fm->mSysWinB8)) return;
    if (CSysWin_getUnk34(&fm->mSysWinF4)) return;

    // Scroll up: step y toward the upper bound.
    f32 max = lbl_eu_806687AC;
    f32 y = fm->pos_y_48 + lbl_eu_806687A8;
    fm->pos_y_48 = y;
    if (y >= max) fm->pos_y_48 = max;

    nw4r::math::_VEC3 vec = { fm->pos_x_44, fm->pos_y_48, lbl_eu_80668764 };

    CFloorMapSetSlotPos((CFloorMapLayoutHolder*)fm->mLayout130, vec);
    CFloorMapSetSlotPos((CFloorMapLayoutHolder*)fm->mLayout138, vec);
    CFloorMapSetSlotPos((CFloorMapLayoutHolder*)fm->mLayout140, vec);
    if (fm->mLayouts150[0].mPtr) {
        for (u8 i = 0; i < fm->field_1F0; i++) {
            CFloorMapSetPanePos(((CFloorMapLayoutHolder*)fm->mLayouts150[i].mPtr)->pane, vec);
        }
    }

    func_8024577C(&fm->mLayout1FC, (u16)func_80248558(&fm->mLayout140));
    fm->field_5D = 1;
}

// Scroll the map view right: step the cursor position toward the right edge
// of the map and push the new position into every map layout's target pane.
void func_8024EA00(CFloorMapFull* fm) {
    if (fm->field_58) return;
    if (CSysWin_getUnk34(&fm->mSysWinB8)) return;
    if (CSysWin_getUnk34(&fm->mSysWinF4)) return;

    // The map pane's scale factor sets the right-edge bound for the step.
    // Retail uses an explicit if/else here (not a ?: ), which MWCC compiles
    // to the branch-over-zero form instead of a conditional move.
    // Both branches re-read mLayout130, so MWCC keeps the tested value in
    // r3, leaves the then-block empty and emits the branch-over-zero form
    // before the shared pane load.
    CFloorMapLayoutHolder* holder;
    if (fm->mLayout130 != 0)
        holder = (CFloorMapLayoutHolder*)fm->mLayout130;
    else
        holder = (CFloorMapLayoutHolder*)NULL;
    nw4r::lyt::Pane* mapPane = holder->pane;
    nw4r::lyt::Pane* zoomPane =
        mapPane->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);
    f32 sx = zoomPane->GetScale().x;
    f32 right = lbl_eu_806687B0 * sx - lbl_eu_806687B4;
    f32 x;
    f32 limit = lbl_eu_80668798 * right;
    x = fm->pos_x_44 + lbl_eu_806687A8;
    fm->pos_x_44 = x;
    if (x >= limit) fm->pos_x_44 = limit;

    f32 py = fm->pos_y_48;
    nw4r::math::_VEC3 vec = { fm->pos_x_44, py, lbl_eu_80668764 };

    CFloorMapSetSlotPos((CFloorMapLayoutHolder*)fm->mLayout130, vec);
    CFloorMapSetSlotPos((CFloorMapLayoutHolder*)fm->mLayout138, vec);
    CFloorMapSetSlotPos((CFloorMapLayoutHolder*)fm->mLayout140, vec);
    if (fm->mLayouts150[0].mPtr) {
        for (u8 i = 0; i < fm->field_1F0; i++) {
            CFloorMapSetPanePos(
                ((CFloorMapLayoutHolder*)fm->mLayouts150[i].mPtr)->pane, vec);
        }
    }

    u32 v = func_80248558(&fm->mLayout140);
    func_8024577C(&fm->mLayout1FC, (u16)v);
    fm->field_5D = 1;
}


// Scroll the map view left: step the cursor position toward the left edge
// of the map and push the new position into every map layout's target pane.
// Mirrors func_8024EA00 with negated step and an extra B8 scale on the bound.
void func_8024EC24(CFloorMapFull* fm) {
    if (fm->field_58) return;
    if (CSysWin_getUnk34(&fm->mSysWinB8)) return;
    if (CSysWin_getUnk34(&fm->mSysWinF4)) return;

    // The map pane's scale factor sets the left-edge bound for the step.
    nw4r::lyt::Pane* mapPane =
        (fm->mLayout130 ? (CFloorMapLayoutHolder*)fm->mLayout130 : NULL)->pane;
    f32 right =
        mapPane->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1)->GetScale().x *
            lbl_eu_806687B0 -
        lbl_eu_806687B4;
    f32 mid = lbl_eu_80668798 * right;
    f32 x = fm->pos_x_44 - lbl_eu_806687A8;
    f32 limit = lbl_eu_806687B8 * mid;
    fm->pos_x_44 = x;
    if (x <= limit) fm->pos_x_44 = limit;

    nw4r::math::_VEC3 vec = { fm->pos_x_44, fm->pos_y_48, lbl_eu_80668764 };

    CFloorMapSetSlotPos((CFloorMapLayoutHolder*)fm->mLayout130, vec);
    CFloorMapSetSlotPos((CFloorMapLayoutHolder*)fm->mLayout138, vec);
    CFloorMapSetSlotPos((CFloorMapLayoutHolder*)fm->mLayout140, vec);
    if (fm->mLayouts150[0].mPtr) {
        for (u8 i = 0; i < fm->field_1F0; i++) {
            CFloorMapSetPanePos(
                ((CFloorMapLayoutHolder*)fm->mLayouts150[i].mPtr)->pane, vec);
        }
    }

    u32 v = func_80248558(&fm->mLayout140);
    func_8024577C(&fm->mLayout1FC, (u16)v);
    fm->field_5D = 1;
}

// Map input handler: drive the two system windows. While the detail window
// (0xF4) is up, close it once finished; while the cursor window (0xB8) is
// closing, refresh the row readout; otherwise open either the row-detail
// window or the map-cursor window for the selected row.
void func_8024EE50(void* self) {
    CFloorMapFull* p = (CFloorMapFull*)self;
    // Retail recomputes the current-floor block address at every access
    // instead of caching it in a register across the UI calls.
#define FLOORBLK ((CFloorMapFloorBlock*)((u8*)p + (s8)p->field_205 * 0x30C))
    if (!FLOORBLK->field_514) return;
    if (!CSysWin_isActive(&p->mSysWinB8)) return;

    if (CSysWin_getUnk34(&p->mSysWinF4)) {
        // Detail window is up: close it once its own animation finished.
        if (!CSysWin_isActive(&p->mSysWinF4)) return;
        func_8022B8E4(&p->mSysWinF4);
        return;
    }

    if (p->field_58) {
        // Cursor window closing animation: refresh the row-id readout only on
        // the first frame (cursor_idx_5C == 0).
        if ((s8)p->field_5C == 0) {
            u16 rows;
            if ((s8)p->field_206 >= 0) {
                rows = FLOORBLK->rows[(s8)p->field_207 + (s8)p->field_206].field_00;
            } else {
                rows = 0;
            }
            p->field_5A = rows;
            func_80138078(0x1D);
        } else {
            func_80138078(6);
        }
        func_801D216C(&p->mCursor, 0);
        func_8022B8E4(&p->mSysWinB8);
        p->field_58 = 0;
        return;
    }

    // Resolve the currently selected row id and its visited flag. Retail
    // evaluates the guarded record lookup twice (once for the id, once for
    // the visited byte), keeping both under the same field_206 sign guard.
    u16 rowId;
    if ((s8)p->field_206 >= 0)
        rowId = FLOORBLK->rows[(s8)p->field_207 + (s8)p->field_206].field_00;
    else
        rowId = 0;
    if (!rowId) return;
    u8 visited = 0;
    if ((s8)p->field_206 >= 0)
        visited = FLOORBLK->rows[(s8)p->field_207 + (s8)p->field_206].field_02;

    if (visited != 0) {
        // Visited row: pick the detail-text variant; maps 14/24/25 with a
        // lucky roll and maps with the extra-info flags use the shorter one.
        // Maps 24 and 25 are separate clauses so each rolls its own random.
        u16 nameIdx = 0x16;
        if (lbl_eu_80664798 == 0xD) {
            u16 rnd = (u16)func_8009CF8C(0x20);
            if (rnd >= 0x171 && rnd <= 0x194) nameIdx = 0x15;
        } else if (lbl_eu_80664798 == 0x17) {
            u16 rnd = (u16)func_8009CF8C(0x20);
            if (rnd >= 0x15F && rnd <= 0x16A) nameIdx = 0x15;
        } else if (lbl_eu_80664798 == 0x18) {
            u16 rnd = (u16)func_8009CF8C(0x20);
            if (rnd >= 0x15F && rnd <= 0x16A) nameIdx = 0x15;
        }
        u32 flags = getUnk80664658()->field_214;
        if ((flags & 0x2000) != 0 || (flags & 0x10000) != 0) nameIdx = 0x15;


        char* name = func_80136190(&lbl_eu_8050BEA8[0x519], &lbl_eu_8050BEA8[0x1F], nameIdx);
        func_8022B9B4(&p->mSysWinF4, name, 0);
        func_8022BFC8(&p->mSysWinF4, 1);
        func_8022B8B8(&p->mSysWinF4);
        func_80138078(5);
        return;
    }

    if (func_8009CF8C(0x3212)) {
        // Unvisited but already-seen marker id: show the "no data" text.
        char* name = func_80136190(&lbl_eu_8050BEA8[0x519], &lbl_eu_8050BEA8[0x1F], 0x15);
        func_8022B9B4(&p->mSysWinF4, name, 0);
        func_8022BFC8(&p->mSysWinF4, 1);
        func_8022B8B8(&p->mSysWinF4);
        func_80138078(5);
        return;
    }

    // Fresh unvisited row: open the map cursor window with the row/floor
    // captions and place the cursor at the window's anchor point.
    char* nRow = func_80136190(&lbl_eu_8050BEA8[0x17F], &lbl_eu_8050BEA8[0x1F], rowId);
    char* nCap1 = func_80136190(&lbl_eu_8050BEA8[0x17], &lbl_eu_8050BEA8[0x1F], 7);
    char* nCap2 = func_80136190(&lbl_eu_8050BEA8[0x17], &lbl_eu_8050BEA8[0x1F], 8);
    char* nCap3 = func_80136190(&lbl_eu_8050BEA8[0x17], &lbl_eu_8050BEA8[0x1F], 9);
    func_8022B9B4(&p->mSysWinB8, nCap1, nRow);
    func_8022BF6C(&p->mSysWinB8, nCap2, nCap3);
    func_8022BFC8(&p->mSysWinB8, 0);
    func_8022B8B8(&p->mSysWinB8);
    p->field_5C = 0;
    nw4r::math::VEC3 pos;
    func_8022C1B4(&pos, &p->mSysWinB8, 0);
    ((CCur18View*)&p->mCursor)->v02(&pos);
    func_801D216C(&p->mCursor, 1);
    p->field_58 = 1;
    p->field_208 = 1;
    func_80138078(3);
}

// Select the floor-table file pointer for map id `id` (29 ids, string offsets
// stride 0x10). Map 18 aliases map 17's string and maps 26-28 alias map 25's,
// so the last strings are reused; unknown ids clear the pointer. MWCC emits
// this as a jump table.
void func_8024F1FC(CFloorMap* self, u8 id) {
    (void)self;
    lbl_eu_80664798 = id;
    switch (id) {
    case 0:  func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x524]); break;
    case 1:  func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x534]); break;
    case 2:  func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x544]); break;
    case 3:  func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x554]); break;
    case 4:  func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x564]); break;
    case 5:  func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x574]); break;
    case 6:  func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x584]); break;
    case 7:  func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x594]); break;
    case 8:  func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x5A4]); break;
    case 9:  func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x5B4]); break;
    case 10: func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x5C4]); break;
    case 11: func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x5D4]); break;
    case 12: func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x5E4]); break;
    case 13: func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x5F4]); break;
    case 14: func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x604]); break;
    case 15: func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x614]); break;
    case 16: func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x624]); break;
    // Maps 17 and 18 are written as two clauses sharing one string, so MWCC
    // emits the body twice instead of merging the jump-table entries.
    case 17: func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x634]); break;
    case 18: func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x634]); break;
    case 19: func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x644]); break;
    case 20: func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x654]); break;
    case 21: func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x664]); break;
    case 22: func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x674]); break;
    case 23: func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x684]); break;
    case 24: func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x694]); break;
    case 25: func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x6A4]); break;
    case 26:
    case 27:
    case 28: func_8003AA34(); lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x6B4]); break;
    default:
        lbl_eu_8066479C = 0;
        break;
    }
}

extern "C" u32 func_8024F538(u8* self) {
    u8 val = *(u8*)(self + 0x41);
    u32 result = __cntlzw(val - 2);
    return result >> 5;
}

extern "C" u16 func_8024F54C(u8* self) { return *(u16*)(self + 0x5A); }

extern "C" u8 func_8024F554(CFloorMapFull* self) { return self->field_58; }

void func_8024F55C(void* self) {
    extern int CSysWin_isActive(void*);
    extern void func_801D216C(void*, int);
    extern void func_8022B8E4(void*);
    extern void func_80138078(unsigned long);
    u8* p = (u8*)self;
    if (p[0x58] && CSysWin_isActive(p + 0xB8)) {
        func_801D216C(p + 0xA0, 0);
        func_8022B8E4(p + 0xB8);
        p[0x58] = 0;
        func_80138078(6);
    }
}

typedef void* (*VFuncPtr)(void*, const char*, u32);
extern "C" void func_801D216C(void*, int);
extern "C" void func_8022B8E4(void*);
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, char const*, void*, int, int);
extern "C" void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, char const*, void*, int, int);
extern "C" int func_800A9D90();
extern "C" void func_801F34F4(void*);

// Size-optimized region: the shared string-table copy below must keep its
// rolled mtctr/bdnz + lwzu/stwu form (under -O4,p it unrolls).
#pragma optimize_for_size on
#pragma use_lmw_stmw off
void func_8024C104(void* self) {
    u8* p = (u8*)self;
    char* namebuf[28];
    u32 handle = (u32)getHandleMEM2__Q23mtl10MemManagerFv();
    *(void**)(p + 0x24) = readFile__11CDeviceFileFUlPCcP10IWorkEventii(handle, &lbl_eu_8050BEA8[0x4e7], self, 0, 0);
    u32 handle2 = func_800A9D90();
    *(void**)(p + 0x30) = readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(handle2, &lbl_eu_8050BEA8[0x4fc], self, 0, 0);
    // Build the local map-file name table from the shared string table
    // (entries 1..28), copied two pointers at a time.
    // Counted mtctr/bdnz copy: temp-first declaration order colors the
    // registers like retail; s/d are biased one element back so the
    // loads/stores fuse into lwzu/stwu update forms.
    {
        u32 v1, v0;
        u32* s;
        u32* d = (u32*)(namebuf)-1;
        u32 n;
        s = (u32*)lbl_eu_8050BDF8 - 1;
        for (n = 14; n != 0; n--) {
            *(d + 1) = *(s + 1);
            *(d + 2) = *(s + 2);
            s += 2;
            d += 2;
        }
    }
    handle = (u32)getHandleMEM2__Q23mtl10MemManagerFv();
    u8 idx = lbl_eu_80664798;
    *(void**)(p + 0x28) = readFile__11CDeviceFileFUlPCcP10IWorkEventii(handle, namebuf[idx - 1], self, 0, 0);
    func_801F34F4(p + 0x60);
    // Invoke vtable slot 0x88 on each embedded sys-win subobject
    ((CFloorMapWinVf88*)(p + 0xB8))->vf88();
    ((CFloorMapWinVf88*)(p + 0xF4))->vf88();
}
#pragma optimize_for_size off


void func_8024F5C4(void* self, u32 arg2) {
    void* ptr = *(void**)((u8*)self + 0x32D4);
    if (!ptr) return;
    void* obj = *(void**)((u8*)ptr + 0x10);
    VFuncPtr* vt = *(VFuncPtr**)obj;
    void* result = vt[15](obj, (char*)&lbl_eu_8050BEA8 + 0xEE, 1);
    *(u8*)((u8*)result + 0xBB) = (*(u8*)((u8*)result + 0xBB) & 0x7F) | (u8)arg2;
}

extern "C" unsigned char func_8024F630(void) {
    return (unsigned char)func_8003B1EC(lbl_eu_8066479C);
}

void func_8024F658(void* self) {
    u8* p = (u8*)self;
    if (p[0x58]) return;
    if (CSysWin_getUnk34(p + 0xB8)) return;
    if (CSysWin_getUnk34(p + 0xF4)) return;
    u8 val = p[0x208];
    u32 result = __cntlzw(val);
    p[0x208] = __cntlzw(val) >> 5;
}

u8 func_8024F6BC(void* self) {
    CFloorMapFull* full = static_cast<CFloorMapFull*>(self);
    if (full->field_58) return 0;
    return full->field_208;
}

extern "C" u8 func_8024F6D8(CFloorMapFull* self) { return self->field_208; }

u32 func_8024F6E0(CFloorMapFull* p) {
    // Row id -1 (no selection) reads as "not visited".
    u16 val;
    if ((s8)p->field_206 < 0) {
        val = 0;
    } else {
        CFloorMapFloorBlock* floor =
            (CFloorMapFloorBlock*)((u8*)p + (s8)p->field_205 * 0x30C);
        val = floor->rows[(s8)p->field_207 + (s8)p->field_206].field_00;
    }
    return val != 0;
}

void func_8024F72C(void* self) {
    u8* p = (u8*)self;
    if (CSysWin_getUnk34(p + 0xB8)) return;
    if (CSysWin_getUnk34(p + 0xF4)) return;
    p[0x333C] = (u32)__cntlzw(p[0x333C]) >> 5;
}

u32 func_8024F784(void* self) {
    u8* p = (u8*)self;
    if (CSysWin_getUnk34(p + 0xB8)) return 1;
    return CSysWin_getUnk34(p + 0xF4);
}

// Move the map cursor to the current row position: rebuild the pane name for
// the row id, accumulate the pane's parent-chain translate up to the layout
// root, scale by the map zoom pane, add the root offset and store the
// normalized position back into the map state.
void func_8024F7CC(void* self) {
    extern int sprintf(char*, const char*, ...);
    CFloorMapFull* p = (CFloorMapFull*)self;
    if (!p->field_5D) return;
    p->field_5D = 0;
    if (p->field_2C) return;
    if (p->field_41 != 1) return;
    if (!p->field_208) return;
    if (CSysWin_getUnk34(&p->mSysWinB8)) return;
    if (CSysWin_getUnk34(&p->mSysWinF4)) return;
    if (p->field_58) return;

    s8 fi = (s8)p->field_205;
    CFloorMapFloorBlock* floor = (CFloorMapFloorBlock*)((u8*)p + fi * 0x30C);
    if (!floor->field_514) return;

    u16 rowId;
    s8 rowIdx = (s8)p->field_206;
    if (rowIdx >= 0) {
        rowId = floor->rows[(s8)p->field_207 + rowIdx].field_00;
    } else {
        rowId = 0;
    }
    if (!rowId) return;

    nw4r::lyt::Pane* pane;
    nw4r::math::VEC3 out;
    nw4r::math::VEC3 rec;
    nw4r::math::VEC3 acc3;
    nw4r::math::VEC3 acc2;
    nw4r::math::VEC3 acc;
    nw4r::math::VEC3 pos;
    CFloorMapLayoutSlots* slots;
    char buf[0x20];

    pos.x = lbl_eu_80668764;
    pos.y = lbl_eu_80668764;
    pos.z = lbl_eu_80668764;
    // Retail forms the layout-slot pointer and the pos address before the
    // sprintf call, keeping both in nonvolatile registers across it.
    slots = (CFloorMapLayoutSlots*)&p->mLayout140;
    sprintf(buf, &lbl_eu_8050BEA8[0x2EB]);

    // Holding the root pane live across the FindPaneByName call mirrors
    // retail's cross-call register pressure (root + slots + &pos + self).
    nw4r::lyt::Pane* root = slots->layout->GetRootPane();
    pane = root->FindPaneByName(buf, 1);
    // Retail tests the pane pointer in cr1 and branches straight to the final
    // divide block when it is NULL; the walk itself is guarded by a separate
    // pane-vs-root compare with shared add tails.
    if (!pane) goto divides;
    {
        out.x = lbl_eu_80668764;
        out.y = lbl_eu_80668764;
        out.z = lbl_eu_80668764;
        if (pane == root) goto do_pos;

        nw4r::lyt::Pane* parent = pane->GetParent();
        out.x = pane->GetTranslate().x;
        out.y = pane->GetTranslate().y;
        out.z = pane->GetTranslate().z;
        acc.x = lbl_eu_80668764;
        acc.y = lbl_eu_80668764;
        acc.z = lbl_eu_80668764;
        if (root != parent && parent != NULL) {
            nw4r::lyt::Pane* gp = parent->GetParent();
            acc.x = parent->GetTranslate().x;
            acc.y = parent->GetTranslate().y;
            acc.z = parent->GetTranslate().z;
            acc2.x = lbl_eu_80668764;
            acc2.y = lbl_eu_80668764;
            acc2.z = lbl_eu_80668764;
            if (root != gp && gp != NULL) {
                nw4r::lyt::Pane* ggp = gp->GetParent();
                acc2.x = gp->GetTranslate().x;
                acc2.y = gp->GetTranslate().y;
                acc2.z = gp->GetTranslate().z;
                acc3.x = lbl_eu_80668764;
                acc3.y = lbl_eu_80668764;
                acc3.z = lbl_eu_80668764;
                if (root != ggp && ggp != NULL) {
                    acc3.x = ggp->GetTranslate().x;
                    acc3.y = ggp->GetTranslate().y;
                    acc3.z = ggp->GetTranslate().z;
                    func_8024B4CC(&rec, slots, ggp->GetParent());
                    nw4r::math::VEC3Add(&acc3, &acc3, &rec);
                }
                goto add_acc2;
            }
            goto add_acc;
        }
        goto add_out;
    add_acc2:
        nw4r::math::VEC3Add(&acc2, &acc2, &acc3);
    add_acc:
        nw4r::math::VEC3Add(&acc, &acc, &acc2);
    add_out:
        nw4r::math::VEC3Add(&out, &out, &acc);
        goto do_pos;
    do_pos:
        pos.x = out.x;
        pos.y = out.y;
        pos.z = out.z;
        nw4r::lyt::Pane* scalePane =
            slots->layout->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);
        pos.x *= scalePane->GetScale().x;
        nw4r::math::VEC3Add(&pos, &pos, &slots->layout->GetRootPane()->GetTranslate());
    }
divides:
    p->field_54 = lbl_eu_806687BC;
    p->field_41 = 2;
    p->field_4C = pos.x / lbl_eu_806687BC;
    p->field_50 = pos.y / lbl_eu_806687BC;
}

// Current-floor lookup: when the current world is the map being shown, walk
// the floor table against the player's y position; otherwise use the saved
// landmark position. Returns the 1-based floor row whose height is above the
// reference point, or 0.
u32 func_8024FB78(void*) {
    u8 curMap = lbl_eu_80664798;
    u32 result = 0;
    // s16 -> f64 conversion slot shared by both branches.
    CFloorMapF64Conv conv;
    if ((lbl_eu_80664184 & 0xFF) == curMap) {
        if (lbl_eu_8066479C != 0) {
            void* player = cf::CfGameManager::getPlayer(0);
            if (player != NULL) {
                int count = func_8003B1EC(lbl_eu_8066479C);
                for (u8 i = 1; i <= count; i++) {
                    // Compare floor height against player y in double precision.
                    s16 val = func_80136330(lbl_eu_8066479C, &lbl_eu_8050BEA8[0x15A], i);
                    f32 y = ((CFloorMapPlayerObj*)player)->GetPos()->y;
                    conv.w[1] = (u32)(s16)val ^ 0x80000000;
                    conv.w[0] = 0x43300000;
                    if ((f32)(conv.d - lbl_eu_80668770) > y) {
                        result = i;
                        break;
                    }
                }
            }
        }
    } else {
        if (lbl_eu_8066479C != 0) {
            u16 rowId = ((u16*)&lbl_eu_8050B798)[curMap];
            if (rowId != 0) {
                f32 tmp[3];
                func_80141DC4(tmp, rowId);
                nw4r::math::VEC3 pos = *(nw4r::math::VEC3*)tmp;
                int count = func_8003B1EC(lbl_eu_8066479C);
                for (u8 i = 1; i <= count; i++) {
                    s16 val = func_80136330(lbl_eu_8066479C, &lbl_eu_8050BEA8[0x15A], i);
                    conv.w[1] = (u32)(s16)val ^ 0x80000000;
                    conv.w[0] = 0x43300000;
                    if (conv.d - lbl_eu_80668770 > pos.y) {
                        result = i;
                        break;
                    }
                }
            }
        }
    }
    return result;
}

// File-load completion handler: dispatch on the four file handles. The first
// two build the map layouts and row-marker panes, the third swaps the map
// buffer and rebuilds the current floor's marker rows, the fourth parses the
// auxiliary world-map file. Returns 1 when the event was consumed, 0 otherwise.
u32 CFloorMap::OnFileEvent(CEventFile* event) {
    // Note: the file handle is re-read from the event at each dispatch (kept
    // in a volatile register by MWCC, like retail) - caching it in a local
    // would force an extra nonvolatile save across every branch body.

    // Event for the floor-list file (field_24): open a scratch region, load
    // the two map layouts, build the row-marker pane list and the embedded
    // cursor, then push the cursor position into every map layout.
    if ((CFileHandle*)this->field_24 == event->mFileHandle) {
        int mem2 = (int)mtl::MemManager::getHandleMEM2();
        ((UnkClass_8045F564*)&this->mMemRegion04[0])
            ->createRegion(mem2, 0x80000, &lbl_eu_8050BEA8[0x6c4], 0);
        Class_8045F858 guard((UnkClass_8045F564*)&this->mMemRegion04[0]);

        CFileHandle* file = (CFileHandle*)this->field_24;
        void* data = file->getData();
        mtl::MemManager::func_80434A4C(false);

        this->field_34 = (u32)CLibLayout::createArcResourceAccessor();
        ((nw4r::lyt::ArcResourceAccessor*)this->field_34)->Attach(data, &lbl_eu_8050BEA8[0x6d1]);

        this->layout_130 = 0;
        this->field_134 = this->field_34;
        func_80136E84((nw4r::lyt::Layout**)&this->layout_130,
                      (nw4r::lyt::ArcResourceAccessor*)this->field_34, &lbl_eu_8050BEA8[0x49b]);

        this->layout_140 = 0;
        this->field_144 = this->field_34;
        this->field_148 = 0;
        this->field_14C = 0;
        func_80136E84((nw4r::lyt::Layout**)&this->layout_140,
                      (nw4r::lyt::ArcResourceAccessor*)this->field_34, &lbl_eu_8050BEA8[0x24d]);

        // Row-marker panes: a base pane appended to the map layout's zoom
        // pane (0x136), then eleven more panes appended to the base pane.
        nw4r::lyt::Pane* pane = new (getAllocHandle__10CLibLayoutFv()) nw4r::lyt::Pane();
        pane->SetName(&lbl_eu_8050BEA8[0x263]);
        pane->SetVisible(true);
        this->field_148 = (u32)pane;
        ((nw4r::lyt::Layout*)this->layout_140)->GetRootPane()
            ->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1)
            ->AppendChild((nw4r::lyt::Pane*)this->field_148);

        nw4r::lyt::Pane* child = new (getAllocHandle__10CLibLayoutFv()) nw4r::lyt::Pane();
        child->SetName(&lbl_eu_8050BEA8[0x26c]);
        child->SetVisible(true);
        ((nw4r::lyt::Pane*)this->field_148)->AppendChild(child);

        child = new (getAllocHandle__10CLibLayoutFv()) nw4r::lyt::Pane();
        child->SetName(&lbl_eu_8050BEA8[0x275]);
        child->SetVisible(true);
        ((nw4r::lyt::Pane*)this->field_148)->AppendChild(child);

        child = new (getAllocHandle__10CLibLayoutFv()) nw4r::lyt::Pane();
        child->SetName(&lbl_eu_8050BEA8[0x281]);
        child->SetVisible(true);
        ((nw4r::lyt::Pane*)this->field_148)->AppendChild(child);

        child = new (getAllocHandle__10CLibLayoutFv()) nw4r::lyt::Pane();
        child->SetName(&lbl_eu_8050BEA8[0x28a]);
        child->SetVisible(true);
        ((nw4r::lyt::Pane*)this->field_148)->AppendChild(child);

        child = new (getAllocHandle__10CLibLayoutFv()) nw4r::lyt::Pane();
        child->SetName(&lbl_eu_8050BEA8[0x293]);
        child->SetVisible(true);
        ((nw4r::lyt::Pane*)this->field_148)->AppendChild(child);

        child = new (getAllocHandle__10CLibLayoutFv()) nw4r::lyt::Pane();
        child->SetName(&lbl_eu_8050BEA8[0x2a0]);
        child->SetVisible(true);
        ((nw4r::lyt::Pane*)this->field_148)->AppendChild(child);

        child = new (getAllocHandle__10CLibLayoutFv()) nw4r::lyt::Pane();
        child->SetName(&lbl_eu_8050BEA8[0x2ab]);
        child->SetVisible(true);
        ((nw4r::lyt::Pane*)this->field_148)->AppendChild(child);

        child = new (getAllocHandle__10CLibLayoutFv()) nw4r::lyt::Pane();
        child->SetName(&lbl_eu_8050BEA8[0x2b4]);
        child->SetVisible(true);
        ((nw4r::lyt::Pane*)this->field_148)->AppendChild(child);

        child = new (getAllocHandle__10CLibLayoutFv()) nw4r::lyt::Pane();
        child->SetName(&lbl_eu_8050BEA8[0x2bd]);
        child->SetVisible(true);
        ((nw4r::lyt::Pane*)this->field_148)->AppendChild(child);

        child = new (getAllocHandle__10CLibLayoutFv()) nw4r::lyt::Pane();
        child->SetName(&lbl_eu_8050BEA8[0x2c7]);
        child->SetVisible(true);
        ((nw4r::lyt::Pane*)this->field_148)->AppendChild(child);

        child = new (getAllocHandle__10CLibLayoutFv()) nw4r::lyt::Pane();
        child->SetName(&lbl_eu_8050BEA8[0x2cf]);
        child->SetVisible(true);
        ((nw4r::lyt::Pane*)this->field_148)->AppendChild(child);

        // Per-map layout table: zeroed local copied into the member array at
        // 0x150, with the table count/accessor/floor-id stored at 0x1F0-0x1F8.
        UnkLayoutEntry layouts[0x14] = {0};
        u8 count = 0;
        u32 acc = this->field_34;
        u8 floorId = 0;
        for (u8 i = 0; i < 0x14; i++) {
            layouts[i].mPtr = 0;
            layouts[i].mUnk = 0;
        }

        for (u8 i = 0; i < 0x14; i++) {
            this->layouts_150[i] = layouts[i];
        }
        this->layout_count_1F0 = count;
        this->field_1F4 = acc;
        this->field_1F8 = floorId;
        func_80246330((CFloorMapLayoutBlock*)&this->layouts_150[0]);

        // Bind the 0x3334 layout slot and rebuild the embedded cursor from a
        // stack temp (body copied without the vtable pointer).
        this->layout_3334 = 0;
        this->field_3338 = this->field_34;
        this->field_333C = 0;
        func_80244764((CFloorMapLayoutData0*)&this->layout_3334);

        u8 tmpCur[0x18];
        __ct__CCur18(tmpCur, func_801355F4());
        CCur18Data* curDst = reinterpret_cast<CCur18Data*>(&this->mCursorA0[0]);
        CCur18Data* curSrc = reinterpret_cast<CCur18Data*>(tmpCur);
        curDst->field_4 = curSrc->field_4;
        curDst->field_8 = curSrc->field_8;
        curDst->field_C = curSrc->field_C;
        curDst->field_10 = curSrc->field_10;
        curDst->field_14 = curSrc->field_14;
        curDst->field_15 = curSrc->field_15;
        __dt__6CCur18Fv(tmpCur, -1);
        reinterpret_cast<CCur18View*>(&this->mCursorA0[0])->v00();

        // Reposition every map layout under the current map cursor.
        if (this->field_34 != 0 && this->field_38 != 0 && this->field_30 == 0) {
            if ((u8)func_8024FB78(this)) {
                nw4r::math::_VEC3 cur;
                func_8024830C(&cur, &this->layout_140);
                this->pos_x_44 -= cur.x;
                this->pos_y_48 -= cur.y;
            }

            nw4r::math::_VEC3 vec = { this->pos_x_44, this->pos_y_48, lbl_eu_80668764 };
            CFloorMapSetSlotPos((CFloorMapLayoutHolder*)this->layout_130, vec);
            CFloorMapSetSlotPos((CFloorMapLayoutHolder*)this->layout_138, vec);
            CFloorMapSetSlotPos((CFloorMapLayoutHolder*)this->layout_140, vec);
            if (this->layouts_150[0].mPtr != 0) {
                for (u8 i = 0; i < this->layout_count_1F0; i++) {
                    CFloorMapSetPanePos(((CFloorMapLayoutHolder*)this->layouts_150[i].mPtr)->pane, vec);
                }
            }
            func_8024577C(&this->layout_1FC, (u16)func_80248558(&this->layout_140));
            this->field_42 = 1;
            this->field_40 = 1;
            this->field_41 = 1;
        }

        this->field_24 = 0;
        ((UnkClass_8045F564*)&this->mMemRegion04[0])->func_8045F810();
        return 1;
    }

    // Event for the current-world file (field_28): load the per-map layout
    // for the current map id into the 0x138 slot.
    if ((CFileHandle*)this->field_28 == event->mFileHandle) {
        int mem2 = (int)mtl::MemManager::getHandleMEM2();
        ((UnkClass_8045F564*)&this->mMemRegion14[0])
            ->createRegion(mem2, 0x14000, &lbl_eu_8050BEA8[0x6d5], 0);
        Class_8045F858 guard((UnkClass_8045F564*)&this->mMemRegion14[0]);

        CFileHandle* file = (CFileHandle*)this->field_28;
        void* data = file->getData();
        mtl::MemManager::func_80434A4C(false);

        this->field_38 = (u32)CLibLayout::createArcResourceAccessor();
        ((nw4r::lyt::ArcResourceAccessor*)this->field_38)->Attach(data, &lbl_eu_8050BEA8[0x6d1]);

        // Copy the per-map layout-name table (28 entries) and load the layout
        // whose name is selected by the current map id.
        u32 mapNames[0x1C];
        const u32* src = lbl_eu_8050BAB0;
        u32* dst = mapNames;
        for (int i = 0xE; i > 0; i--) {
            *dst++ = *src++;
            *dst++ = *src++;
        }

        this->layout_138 = 0;
        this->field_13C = this->field_38;
        func_80136E84((nw4r::lyt::Layout**)&this->layout_138,
                      (nw4r::lyt::ArcResourceAccessor*)this->field_13C,
                      (const char*)mapNames[lbl_eu_80664798 - 1]);

        // Reposition every map layout under the current map cursor.
        if (this->field_34 != 0 && this->field_38 != 0 && this->field_30 == 0) {
            if ((u8)func_8024FB78(this)) {
                nw4r::math::_VEC3 cur;
                func_8024830C(&cur, &this->layout_140);
                this->pos_x_44 -= cur.x;
                this->pos_y_48 -= cur.y;
            }

            nw4r::math::_VEC3 vec = { this->pos_x_44, this->pos_y_48, lbl_eu_80668764 };
            CFloorMapSetSlotPos((CFloorMapLayoutHolder*)this->layout_130, vec);
            CFloorMapSetSlotPos((CFloorMapLayoutHolder*)this->layout_138, vec);
            CFloorMapSetSlotPos((CFloorMapLayoutHolder*)this->layout_140, vec);
            if (this->layouts_150[0].mPtr != 0) {
                for (u8 i = 0; i < this->layout_count_1F0; i++) {
                    CFloorMapSetPanePos(((CFloorMapLayoutHolder*)this->layouts_150[i].mPtr)->pane, vec);
                }
            }
            func_8024577C(&this->layout_1FC, (u16)func_80248558(&this->layout_140));
            this->field_42 = 1;
            this->field_40 = 1;
            this->field_41 = 1;
        }

        this->field_28 = 0;
        ((UnkClass_8045F564*)&this->mMemRegion14[0])->func_8045F810();
        return 1;
    }

    // Event for the per-floor map file (field_2C): park the old buffer with
    // a one-frame free countdown, swap in the new map buffer and rebuild the
    // current floor's marker rows.
    if ((CFileHandle*)this->field_2C == event->mFileHandle) {
        void* oldBuf = (void*)this->allocated_buf_3C;
        this->field_3340 = (u8*)oldBuf;
        this->field_3344 = lbl_eu_80668760;
        CFileHandle* file = (CFileHandle*)this->field_2C;
        this->allocated_buf_3C = (u32)file->getData();

        func_80247490(&this->layout_140,
                      ((CFloorMapFloorBlock*)((u8*)this + (s8)this->field_205 * 0x30C))->field_20C,
                      (u32)oldBuf, lbl_eu_80668760);

        this->field_1F8 = ((CFloorMapFloorBlock*)((u8*)this + (s8)this->field_205 * 0x30C))->field_20C;
        u8 rowCount = ((CFloorMapFloorBlock*)((u8*)this + (s8)this->field_205 * 0x30C))->field_514;
        for (u8 i = 0; i < rowCount; i++) {
            CFloorMapFloorBlock* fl = (CFloorMapFloorBlock*)((u8*)this + (s8)this->field_205 * 0x30C);
            // The row id comes from the row record at +0x214. When the index
            // wraps past 0xFF, fall back to row (field_206 + field_207), or
            // row 0 when field_206 is negative. Retail passes &layout_140
            // (the layout slot block), not the CFloorMap itself.
            u16 rowId;
            if ((u8)i == 0xFF) {
                if ((s8)this->field_206 < 0) {
                    rowId = 0;
                } else {
                    rowId = fl->rows[(u8)((s8)this->field_207 + (s8)this->field_206)].field_00;
                }
            } else {
                rowId = fl->rows[(u8)i].field_00;
            }
            func_8024808C((CFloorMapFull*)&this->layout_140, rowId);
        }

        func_8024577C(&this->layout_1FC, (u16)func_80248558(&this->layout_140));

        func_8024B6F8((CFloorMapRowList*)&this->layout_138, (void*)this->allocated_buf_3C,
                      ((CFloorMapFloorBlock*)((u8*)this + (s8)this->field_205 * 0x30C))->field_20C,
                      this->field_32CC, lbl_eu_80664798);
        this->field_2C = 0;
        return 1;
    }

    // Event for the auxiliary world-map file (field_30): hand the buffer to
    // the BDAT parser and refresh the map positions.
    if ((CFileHandle*)this->field_30 == event->mFileHandle) {
        CFileHandle* file = (CFileHandle*)this->field_30;
        u32 zero = 0;
        void* data = file->getData();
        CBdat::func_8003AA78(2, data);
        this->field_30 = zero;

        if (this->field_34 != 0 && this->field_38 != 0 && zero == 0) {
            if ((u8)func_8024FB78(this)) {
                nw4r::math::_VEC3 cur;
                func_8024830C(&cur, &this->layout_140);
                this->pos_x_44 -= cur.x;
                this->pos_y_48 -= cur.y;
            }

            nw4r::math::_VEC3 vec = { this->pos_x_44, this->pos_y_48, lbl_eu_80668764 };
            CFloorMapSetSlotPos((CFloorMapLayoutHolder*)this->layout_130, vec);
            CFloorMapSetSlotPos((CFloorMapLayoutHolder*)this->layout_138, vec);
            CFloorMapSetSlotPos((CFloorMapLayoutHolder*)this->layout_140, vec);
            if (this->layouts_150[0].mPtr != 0) {
                for (u8 i = 0; i < this->layout_count_1F0; i++) {
                    CFloorMapSetPanePos(((CFloorMapLayoutHolder*)this->layouts_150[i].mPtr)->pane, vec);
                }
            }
            func_8024577C(&this->layout_1FC, (u16)func_80248558(&this->layout_140));
            this->field_42 = 1;
            this->field_40 = 1;
            this->field_41 = 1;
        }
        return 1;
    }

    return 0;
}

extern "C" void* __ct__80244F50(void* self, int a, int b, u8 c) {
    u8* p = (u8*)self;

    *(u32*)(p + 0x00) = 0;
    *(u32*)(p + 0x04) = b;
    *(u8*)(p + 0x08) = c;
    *(u8*)(p + 0x09) = 0;
    *(s8*)(p + 0x0A) = -1;
    *(u8*)(p + 0x0B) = 0;
    *(u8*)(p + 0x0C) = 1;

    // Tautological range test over the object's own storage (retail emits
    // both compare results; the flag ends up 1 either way).
    const u8* lo = p + 0x31C;
    const u8* hi = p + 0x30D0;
    u8 flag = 1;
    if (lo < hi) {
        flag = (lo <= hi);
    }

    const void* n0 = lbl_eu_80537104;
    const void* n1 = lbl_eu_805370EC;
    const void* n2 = lbl_eu_805370D4;
    const void* n3 = lbl_eu_805370BC;

    // Four consecutive 0x18-byte row records: default name string, five
    // zero words, a zero byte and the shared flag byte.
    *(u8*)(p + 0x30D0) = 0;
    *(const void**)(p + 0x30D4) = n0;
    *(u32*)(p + 0x30D8) = 0;
    *(u32*)(p + 0x30DC) = 0;
    *(u32*)(p + 0x30E0) = 0;
    *(u32*)(p + 0x30E4) = 0;
    *(u8*)(p + 0x30E8) = 0;
    *(u8*)(p + 0x30E9) = flag;

    *(const void**)(p + 0x30EC) = n1;
    *(u32*)(p + 0x30F0) = 0;
    *(u32*)(p + 0x30F4) = 0;
    *(u32*)(p + 0x30F8) = 0;
    *(u32*)(p + 0x30FC) = 0;
    *(u8*)(p + 0x3100) = 0;
    *(u8*)(p + 0x3101) = flag;

    *(const void**)(p + 0x3104) = n2;
    *(u32*)(p + 0x3108) = 0;
    *(u32*)(p + 0x310C) = 0;
    *(u32*)(p + 0x3110) = 0;
    *(u32*)(p + 0x3114) = 0;
    *(u8*)(p + 0x3118) = 0;
    *(u8*)(p + 0x3119) = flag;

    *(const void**)(p + 0x311C) = n3;
    *(u32*)(p + 0x3120) = 0;
    *(u32*)(p + 0x3124) = 0;
    *(u32*)(p + 0x3128) = 0;
    *(u32*)(p + 0x312C) = 0;
    *(u8*)(p + 0x3130) = 0;
    *(u8*)(p + 0x3131) = flag;

    *(u32*)(p + 0x3134) = (u32)a;
    return self;
}
// Constructor decoded from retail __ct__8024507C (us-80247234): layout load +
// font bind + holder/s16-table initialization. Implementation in progress.
extern "C" void __ct__8024507C(void* self) {
    // Typed view over the cursor object: layout/accessor head plus the four
    // embedded layout widgets at +0x30D4.
    CFloorMapCtorView* cv = (CFloorMapCtorView*)self;
    CFloorMapWidgets* wv = (CFloorMapWidgets*)self;

    // Layout load: func_80136E84(&this->layout@+0, accessor=this->[+4], path=&lbl[267])
    func_80136E84(&cv->layout, cv->accessor, &lbl_eu_8050BEA8[267]);

    // Font bind onto the loaded layout's root pane.
    nw4r::lyt::Layout* layout = cv->layout;
    nw4r::lyt::Pane* rootPane = layout->GetRootPane();
    void* fontObj = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, layout);
    u32 fontResult = ((CFloorMapFontView*)fontObj)->getFontHandle();
    func_8013676C(rootPane, fontResult);

    // Fill the four .sbss colour tables from two layout group queries: table
    // A0 gets the index-0 colours, A8 the index-1 colours, and both B0/B8
    // share their last entry with A0's.
    s16* c0 = func_80139658(layout, &lbl_eu_8050BEA8[0x120], 0);
    s16* c1 = func_80139658(layout, &lbl_eu_8050BEA8[0x120], 1);
    lbl_eu_806647A0[0] = c0[0];
    lbl_eu_806647A0[1] = c0[1];
    lbl_eu_806647A0[2] = c0[2];
    lbl_eu_806647A0[3] = c0[3];
    lbl_eu_806647A8[0] = c1[0];
    lbl_eu_806647A8[1] = c1[1];
    lbl_eu_806647A8[2] = c1[2];
    lbl_eu_806647A8[3] = c1[3];
    lbl_eu_806647B0[3] = lbl_eu_806647A0[3];
    lbl_eu_806647B8[3] = lbl_eu_806647A0[3];

    // Reset each embedded layout widget in turn: clear everything but the
    // vtable, install the shared arc resource accessor, then call the
    // widget's virtual init entry with its colour-table / name argument.
    CFloorMapWidgetArg arg;

    arg.nameOrTable = lbl_eu_806647B8;
    wv->widgets[0].field_04 = NULL;
    wv->widgets[0].accessor = cv->accessor;
    wv->widgets[0].field_0C = NULL;
    wv->widgets[0].field_10 = NULL;
    wv->widgets[0].field_14 = 0;
    wv->widgets[0].field_15 = 1;
    ((CFloorMapWidget*)&wv->widgets[0])->Init(&arg);

    arg.nameOrTable = lbl_eu_805370EC;
    wv->widgets[1].field_04 = NULL;
    wv->widgets[1].accessor = cv->accessor;
    wv->widgets[1].field_0C = NULL;
    wv->widgets[1].field_10 = NULL;
    wv->widgets[1].field_14 = 0;
    wv->widgets[1].field_15 = 1;
    ((CFloorMapWidget*)&wv->widgets[1])->Init(&arg);

    arg.nameOrTable = lbl_eu_805370D4;
    wv->widgets[2].field_04 = NULL;
    wv->widgets[2].accessor = cv->accessor;
    wv->widgets[2].field_0C = NULL;
    wv->widgets[2].field_10 = NULL;
    wv->widgets[2].field_14 = 0;
    wv->widgets[2].field_15 = 1;
    ((CFloorMapWidget*)&wv->widgets[2])->Init(&arg);

    arg.nameOrTable = lbl_eu_805370BC;
    wv->widgets[3].field_04 = NULL;
    wv->widgets[3].accessor = cv->accessor;
    wv->widgets[3].field_0C = NULL;
    wv->widgets[3].field_10 = NULL;
    wv->widgets[3].field_14 = 0;
    wv->widgets[3].field_15 = 1;
    ((CFloorMapWidget*)&wv->widgets[3])->Init(&arg);

    // Rebuild the derived views.
    func_80245950(self);
    func_80245DF8(self);
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_80250CB4() {
    lbl_eu_806647A0[3] = 0xFF;
    lbl_eu_806647A0[2] = 0xFF;
    lbl_eu_806647A0[1] = 0xFF;
    lbl_eu_806647A0[0] = 0xFF;
    lbl_eu_806647A8[3] = 0xFF;
    lbl_eu_806647A8[2] = 0xFF;
    lbl_eu_806647A8[1] = 0xFF;
    lbl_eu_806647A8[0] = 0xFF;
    lbl_eu_806647B0[3] = 0xFF;
    lbl_eu_806647B0[2] = 0x23;
    lbl_eu_806647B0[1] = 0x8C;
    lbl_eu_806647B0[0] = 0xA0;
    lbl_eu_806647B8[3] = 0xFF;
    lbl_eu_806647B8[2] = 0x43;
    lbl_eu_806647B8[1] = 0xC0;
    lbl_eu_806647B8[0] = 0xD9;
}

// Load the floor layout, bind the font to its root pane, then hide the ten
// row panes: each pane name is formatted from the shared prefix string at
// 0x17/0x1F and the row index (0xB..0x14), and func_80136B4C updates the
// pane addressed by the string at 0x24 + 9*(row-0xB).
void func_80244764(CFloorMapLayoutData0* self) {
    func_80136E84(&self->layout, self->accessor, &lbl_eu_8050BEA8[0]);

    nw4r::lyt::Pane* rootPane = self->layout->GetRootPane();
    void* fontObj = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, self->layout);
    u32 fontResult = ((CFloorMapFontView*)fontObj)->getFontHandle();
    func_8013676C(rootPane, fontResult);

    func_80136B4C(self->layout, &lbl_eu_8050BEA8[0x24],
                  func_80136190(&lbl_eu_8050BEA8[0x17], &lbl_eu_8050BEA8[0x1F], 0xB), 0);
    func_80136B4C(self->layout, &lbl_eu_8050BEA8[0x2D],
                  func_80136190(&lbl_eu_8050BEA8[0x17], &lbl_eu_8050BEA8[0x1F], 0xC), 0);
    func_80136B4C(self->layout, &lbl_eu_8050BEA8[0x36],
                  func_80136190(&lbl_eu_8050BEA8[0x17], &lbl_eu_8050BEA8[0x1F], 0xD), 0);
    func_80136B4C(self->layout, &lbl_eu_8050BEA8[0x3F],
                  func_80136190(&lbl_eu_8050BEA8[0x17], &lbl_eu_8050BEA8[0x1F], 0xE), 0);
    func_80136B4C(self->layout, &lbl_eu_8050BEA8[0x48],
                  func_80136190(&lbl_eu_8050BEA8[0x17], &lbl_eu_8050BEA8[0x1F], 0xF), 0);
    func_80136B4C(self->layout, &lbl_eu_8050BEA8[0x51],
                  func_80136190(&lbl_eu_8050BEA8[0x17], &lbl_eu_8050BEA8[0x1F], 0x10), 0);
    func_80136B4C(self->layout, &lbl_eu_8050BEA8[0x5A],
                  func_80136190(&lbl_eu_8050BEA8[0x17], &lbl_eu_8050BEA8[0x1F], 0x11), 0);
    func_80136B4C(self->layout, &lbl_eu_8050BEA8[0x63],
                  func_80136190(&lbl_eu_8050BEA8[0x17], &lbl_eu_8050BEA8[0x1F], 0x12), 0);
    func_80136B4C(self->layout, &lbl_eu_8050BEA8[0x6C],
                  func_80136190(&lbl_eu_8050BEA8[0x17], &lbl_eu_8050BEA8[0x1F], 0x13), 0);
    func_80136B4C(self->layout, &lbl_eu_8050BEA8[0x75],
                  func_80136190(&lbl_eu_8050BEA8[0x17], &lbl_eu_8050BEA8[0x1F], 0x14), 0);
}

// Load the map layout + animation transforms, bind the font, hide the four
// decorative panes, and start the two animation transforms.
void func_80244944(void* self) {
    CFloorMapLayoutData* obj = (CFloorMapLayoutData*)self;
    func_80136E84(&obj->layout, obj->accessor, &lbl_eu_8050BEA8[0x7E]);
    func_80136F08(obj->layout, &obj->anim0C, obj->accessor, &lbl_eu_8050BEA8[0x96]);
    func_80136F08(obj->layout, &obj->anim10, obj->accessor, &lbl_eu_8050BEA8[0xB3]);

    nw4r::lyt::Pane* rootPane = obj->layout->GetRootPane();
    void* fontObj = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, obj->layout);
    u32 fontResult = ((CFloorMapFontView*)fontObj)->getFontHandle();
    func_8013676C(rootPane, fontResult);

    obj->layout->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0xCE], 1)->SetVisible(false);
    obj->layout->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0xD9], 1)->SetVisible(false);
    obj->layout->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0xE4], 1)->SetVisible(false);
    obj->layout->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0xEE], 1)->SetVisible(false);

    obj->layout->SetAnimationEnable(obj->anim10, false);
    obj->layout->SetAnimationEnable(obj->anim0C, true);
    obj->layout->Animate(0);
}
// Load the map layout + animation transforms, bind the font, hide the three
// decorative panes (0x100 / 0xCE / 0xD9), and start the two animations.
void func_80244AE8(void* self) {
    CFloorMapLayoutData* obj = (CFloorMapLayoutData*)self;
    func_80136E84(&obj->layout, obj->accessor, &lbl_eu_8050BEA8[0x7E]);
    func_80136F08(obj->layout, &obj->anim0C, obj->accessor, &lbl_eu_8050BEA8[0x96]);
    func_80136F08(obj->layout, &obj->anim10, obj->accessor, &lbl_eu_8050BEA8[0xB3]);

    nw4r::lyt::Pane* rootPane = obj->layout->GetRootPane();
    void* fontObj = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, obj->layout);
    u32 fontResult = ((CFloorMapFontView*)fontObj)->getFontHandle();
    func_8013676C(rootPane, fontResult);

    obj->layout->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x100], 1)->SetVisible(false);
    obj->layout->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0xCE], 1)->SetVisible(false);
    obj->layout->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0xD9], 1)->SetVisible(false);

    obj->layout->SetAnimationEnable(obj->anim10, false);
    obj->layout->SetAnimationEnable(obj->anim0C, true);
    obj->layout->Animate(0);
}
// Load the map layout + animation transforms, bind the font, hide the three
// decorative panes, and start the two animation transforms.
void func_80244C60(void* self) {
    CFloorMapLayoutData* obj = (CFloorMapLayoutData*)self;
    func_80136E84(&obj->layout, obj->accessor, &lbl_eu_8050BEA8[0x7E]);
    func_80136F08(obj->layout, &obj->anim0C, obj->accessor, &lbl_eu_8050BEA8[0x96]);
    func_80136F08(obj->layout, &obj->anim10, obj->accessor, &lbl_eu_8050BEA8[0xB3]);

    nw4r::lyt::Pane* rootPane = obj->layout->GetRootPane();
    void* fontObj = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, obj->layout);
    u32 fontResult = ((CFloorMapFontView*)fontObj)->getFontHandle();
    func_8013676C(rootPane, fontResult);

    obj->layout->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x100], 1)->SetVisible(false);
    obj->layout->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0xD9], 1)->SetVisible(false);
    obj->layout->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0xE4], 1)->SetVisible(false);

    obj->layout->SetAnimationEnable(obj->anim10, false);
    obj->layout->SetAnimationEnable(obj->anim0C, true);
    obj->layout->Animate(0);
}

void func_80244DD8(void* self) {
    CFloorMapLayoutData* obj = (CFloorMapLayoutData*)self;
    func_80136E84(&obj->layout, obj->accessor, &lbl_eu_8050BEA8[0x7E]);
    func_80136F08(obj->layout, &obj->anim0C, obj->accessor, &lbl_eu_8050BEA8[0x96]);
    func_80136F08(obj->layout, &obj->anim10, obj->accessor, &lbl_eu_8050BEA8[0xB3]);

    nw4r::lyt::Pane* rootPane = obj->layout->GetRootPane();
    void* fontObj = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, obj->layout);
    u32 fontResult = ((CFloorMapFontView*)fontObj)->getFontHandle();
    func_8013676C(rootPane, fontResult);

    obj->layout->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x100], 1)->SetVisible(false);
    obj->layout->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0xCE], 1)->SetVisible(false);
    obj->layout->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0xE4], 1)->SetVisible(false);

    obj->layout->SetAnimationEnable(obj->anim10, false);
    obj->layout->SetAnimationEnable(obj->anim0C, true);
    obj->layout->Animate(0);
}

void func_802452C4(void* self) {
    CFloorMapCursor* cur = (CFloorMapCursor*)self;
    u8 count = *(u8*)((u8*)self + cur->field_09 * 0x30C + 0x318);
    // Page up by one row; wrap to the previous page once the row index
    // underflows, then to the last page/row of the floor.
    u8 a1 = (u8)(cur->field_0A - 1);
    cur->field_0A = a1;
    if ((s8)a1 < 0) {
        cur->field_0A = 0;
        u8 b1 = (u8)(cur->field_0B - 1);
        cur->field_0B = b1;
        if ((s8)b1 < 0) {
            if (count >= 5) {
                cur->field_0A = 4;
                cur->field_0B = (u8)(count - 5);
            } else {
                u8 c1 = (u8)(count - 1);
                cur->field_0A = c1;
                cur->field_0B = 0;
                if ((s8)c1 < 0)
                    cur->field_0A = 0;
            }
        }
    }
    func_80246200(cur);

    // Refresh the cursor position: format the pane name for the current page
    // index, scale its translate by the zoom pane, and move the target pane.
    nw4r::math::VEC3 dest;
    nw4r::math::VEC3 apos;
    char buf[0x20];
    sprintf(buf, &lbl_eu_8050BEA8[0x14D], (s8)cur->field_0A);

    nw4r::lyt::Pane* pane = cur->mData->GetRootPane()->FindPaneByName(buf, 1);
    func_801375A0(&apos, pane);

    nw4r::lyt::Pane* scalePane =
        cur->mData->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);
    apos.x = apos.x * scalePane->GetScale().x;

    dest = apos;
    if (cur->field_3108) {
        CFloorMapCursorTarget* target = (CFloorMapCursorTarget*)cur->field_3108;
        target->pane->SetTranslate(dest);
    }

    func_801F3850(cur->field_3134, (u16)(s8)cur->field_0B);
}
