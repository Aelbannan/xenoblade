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

// Named .sdata2 conversion magics: defining them lets MWCC's constant pool
// reuse the retail symbols for the (f32) casts on s16/u8 map coordinates
// instead of emitting TU-local @N labels (CMiniMap.cpp idiom).
extern const double lbl_eu_80668770 = 0x4330000080000000ll;
extern const double lbl_eu_80668788 = 0x4330000000000000ll;

extern u32 lbl_eu_8066479C;
extern u32 func_8003B1EC(u32);
extern u32 lbl_eu_8050BDF8[];
extern u8 lbl_eu_80664798;

extern void func_80137C1C(void*, void*);
extern void* createPicture__10CLibLayoutFv();
extern void SetName__Q34nw4r3lyt4PaneFPCc(void*, const char*);

// Draw helpers used by CFloorMap::Draw and related functions
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
void func_801F35B0(void* scrollBar, void* drawInfo);
void func_8022B7C8(void* sysWin, void* drawInfo);
void func_801D20B0(void* cursor, void* drawInfo);

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

u8 func_8024CE60(void* self) { return static_cast<CFloorMapFull*>(self)->field_40; }


// Move the map cursor down one row on the current page, wrapping through
// pages and resetting to the first row once the last visible row is passed.
void func_80245450(void* self) {
    CFloorMapCursor* cur = (CFloorMapCursor*)self;
    CFloorMapFloorEntry* entries = reinterpret_cast<CFloorMapFloorEntry*>(cur->_0C);
    u8 count = reinterpret_cast<CFloorMapFloorEntryView*>(&entries[cur->field_09])->count;
    if (count >= 5) {
        u8 na = (u8)(cur->field_0A + 1);
        cur->field_0A = na;
        if ((s8)na >= 5) {
            u8 nb = (u8)(cur->field_0B + 1);
            cur->field_0A = 4;
            cur->field_0B = nb;
            if ((s8)nb > (s32)(count - 5)) {
                cur->field_0A = 0;
                cur->field_0B = 0;
            }
        }
    } else {
        u8 na = (u8)(cur->field_0A + 1);
        cur->field_0A = na;
        if ((s8)na >= count) {
            cur->field_0A = 0;
            cur->field_0B = 0;
        }
    }
    func_80246200(cur);

    // Refresh the cursor position: format the pane name for the current page
    // index, scale its translate by the zoom pane, and move the target pane.
    // pos/dest are POD _VEC3 so the struct copy uses lwz/stw (retail).
    nw4r::math::_VEC3 dest;
    nw4r::math::_VEC3 pos;
    char buf[0x20];
    sprintf(buf, &lbl_eu_8050BEA8[0x14D], (s8)cur->field_0A);

    nw4r::lyt::Pane* pane = cur->mData->GetRootPane()->FindPaneByName(buf, 1);
    func_801375A0((nw4r::math::VEC3*)&pos, pane);

    nw4r::lyt::Pane* scalePane =
        cur->mData->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);
    pos.x *= scalePane->GetScale().x;

    dest = pos;
    if (cur->field_3108) {
        CFloorMapCursorTarget* target = (CFloorMapCursorTarget*)cur->field_3108;
        target->pane->SetTranslate(*(nw4r::math::VEC3*)&dest);
    }

    func_801F3850(cur->field_3134, (u16)(s8)cur->field_0B);
}

void func_802455F0(void* self) {
    CFloorMapCursor* cur = (CFloorMapCursor*)self;
    CFloorMapFloorEntry* entries = reinterpret_cast<CFloorMapFloorEntry*>(cur->_0C);
    u8 count =
        reinterpret_cast<CFloorMapFloorEntryView*>(&entries[cur->field_09])->count;
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
    nw4r::math::VEC3 pos;
    nw4r::math::VEC3 dest;
    char buf[0x20];
    sprintf(buf, &lbl_eu_8050BEA8[0x14D], (s8)cur->field_0A);

    nw4r::lyt::Pane* pane = cur->mData->GetRootPane()->FindPaneByName(buf, 1);
    func_801375A0(&pos, pane);

    nw4r::lyt::Pane* scalePane =
        cur->mData->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);
    pos.x *= scalePane->GetScale().x;

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
    s8 oldA = (s8)cur->field_0A;
    s8 oldB = (s8)cur->field_0B;
    CFloorMapFloorEntry* entries = reinterpret_cast<CFloorMapFloorEntry*>(cur->_0C);
    u8 count = reinterpret_cast<CFloorMapFloorEntryView*>(&entries[cur->field_09])->count;
    for (u8 i = 0; i < count; i++) {
        if (*(u16*)((u8*)&entries[cur->field_09] + 0xC + i * 0x18) == val) {
            if (i >= 5) {
                cur->field_0A = 4;
                cur->field_0B = (u8)(i - 4);
            } else {
                cur->field_0A = i;
                cur->field_0B = 0;
            }
            func_80246200(cur);

            // Refresh the cursor position: format the pane name for the
            // current page index, scale its translate by the zoom pane, and
            // move the target pane.
            nw4r::math::VEC3 pos;
            nw4r::math::VEC3 dest;
            char buf[0x20];
            sprintf(buf, &lbl_eu_8050BEA8[0x14D], (s8)cur->field_0A);

            nw4r::lyt::Pane* pane = cur->mData->GetRootPane()->FindPaneByName(buf, 1);
            func_801375A0(&pos, pane);

            nw4r::lyt::Pane* scalePane =
                cur->mData->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);
            pos.x *= scalePane->GetScale().x;

            dest = pos;
            if (cur->field_3108) {
                CFloorMapCursorTarget* target = (CFloorMapCursorTarget*)cur->field_3108;
                target->pane->SetTranslate(dest);
            }

            func_801F3850(cur->field_3134, (u16)(s8)cur->field_0B);
            break;
        }
    }
    if ((s8)cur->field_0A != oldA || (s8)cur->field_0B != oldB)
        func_80138078(1);
}

// Rebuild the floor-name entries from the current floor table, set the
// per-floor row panes' visibility/color (found floors use one color set),
// position the map panes under the current floor, and update the two status
// panes' flags for the current map.
void func_80245950(void* self) {
    CFloorMapCursor* cur = (CFloorMapCursor*)self;
    u8 count = (u8)func_8003B1EC(lbl_eu_8066479C);
    cur->field_30D0 = (u8)count;

    // Fill the floor entries from the table in reverse order.
    s8 fi = (s8)(count - 1);
    for (u8 i = 1; i <= count; i++) {
        CFloorMapFloorNameEntry* e =
            (CFloorMapFloorNameEntry*)((u8*)self + (s8)fi * 0x30C);
        e->field_10 = i;
        e->field_12 = (u16)func_80136330(lbl_eu_8066479C, &lbl_eu_8050BEA8[0x15A], i);
        e->field_14 = (u32)func_8013639C((const void*)lbl_eu_8066479C, (const void*)&lbl_eu_8050BEA8[0x161], (int)i);
        e->field_318 = 0;
        fi--;
    }

    // Name the floor panes bottom-up; floors present in the current map use
    // the found-color table, the rest the default table.
    char buf2[0x20];
    for (u8 k = count; k >= 1; k--) {
        u8 found = 0;
        u16 idx = 0;
        u8 floorId = (u8)(count - ((u8)k - 1));
        do {
            u16 j = (u16)(idx + 1);
            if (func_8013AC3C(lbl_eu_80664798, floorId, j)) {
                found = 1;
                break;
            }
            idx++;
        } while (idx < 0x240);
        sprintf(buf2, &lbl_eu_8050BEA8[0x12A], k);
        if (found)
            func_80139A18(cur->mData, buf2, lbl_eu_806647B0, lbl_eu_806647B8);
        else
            func_80139A18(cur->mData, buf2, lbl_eu_806647A0, lbl_eu_806647A8);
    }

    // Show the first `count` row panes; each pane's low flag bit is set when
    // its index is below the floor count.
    char buf3[0x20];
    for (u8 k = 0; k < 10; k++) {
        sprintf(buf3, &lbl_eu_8050BEA8[0x12A], (u8)(k + 1));
        nw4r::lyt::Pane* pane = cur->mData->GetRootPane()->FindPaneByName(buf3, 1);
        CFloorMapPaneFlag* pf = (CFloorMapPaneFlag*)pane;
        pf->mFlag = (pf->mFlag & 0x7F) | (((u32)k - (u32)count) >> 31 & 1);
    }

    // Pick the current floor entry: the first entry whose floor id matches
    // the current-floor flag (or id 1 when the flag is unset).
    for (u8 i = 0; i < count; i++) {
        CFloorMapFloorNameEntry* e =
            (CFloorMapFloorNameEntry*)((u8*)self + (u8)i * 0x30C);
        if (cur->field_08 == 0) {
            if (e->field_10 == 1) {
                cur->field_09 = (s8)i;
                break;
            }
        } else if (e->field_10 == cur->field_08) {
            cur->field_09 = (s8)i;
            break;
        }
    }

    // Position the map pane under the current floor.
    char buf4[0x20];
    sprintf(buf4, &lbl_eu_8050BEA8[0x12A], (s8)cur->field_09 + 1);
    nw4r::lyt::Pane* pane = cur->mData->GetRootPane()->FindPaneByName(buf4, 1);
    nw4r::math::VEC3 pos;
    func_801375A0(&pos, pane);
    nw4r::lyt::Pane* scalePane =
        cur->mData->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);
    pos.x *= scalePane->GetScale().x;
    nw4r::math::_VEC3 dest = pos;
    if (cur->field_30D8)
        CFloorMapSetPanePos(cur->field_30D8->GetRootPane(), dest);

    func_80136B4C(cur->field_30D8, &lbl_eu_8050BEA8[0xF8],
                  (char*)((u8*)self + (s8)cur->field_09 * 0x30C)[0x14], 0);
    func_80136B4C(cur->mData, &lbl_eu_8050BEA8[0x145],
                  func_8013639C((const void*)lbl_eu_806640A8, (const void*)&lbl_eu_8050BEA8[0x1F], (int)lbl_eu_80664798), 0);

    // Second position target (cursor-position pane) when the current floor
    // flag is set.
    if (cur->field_08 != 0) {
        char buf5[0x20];
        sprintf(buf5, &lbl_eu_8050BEA8[0x12A], (s8)cur->field_09 + 1);
        nw4r::lyt::Pane* pane2 = cur->mData->GetRootPane()->FindPaneByName(buf5, 1);
        nw4r::math::VEC3 pos2;
        func_801375A0(&pos2, pane2);
        nw4r::lyt::Pane* scalePane2 =
            cur->mData->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);
        pos2.x *= scalePane2->GetScale().x;
        nw4r::math::_VEC3 dest2 = pos2;
        if (cur->field_30F0)
            CFloorMapSetPanePos(cur->field_30F0->GetRootPane(), dest2);
    }

    if (lbl_eu_80664798 != (s32)lbl_eu_80664184)
        cur->field_3101 = 0;

    // Flag the panes when the current map is one of the special tutorial-map
    // ids (0x13-0x15, 0x17-0x18, 0x1B).
    u8 flag = 0;
    if ((u32)(lbl_eu_80664798 - 0x13) <= 2 || (u32)(lbl_eu_80664798 - 0x17) <= 1 ||
        lbl_eu_80664798 == 0x1B)
        flag = 1;

    nw4r::lyt::Pane* paneA =
        cur->mData->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x16B], 1);
    CFloorMapPaneFlag* pfa = (CFloorMapPaneFlag*)paneA;
    pfa->mFlag = (pfa->mFlag & 0x7F) | ((__cntlzw(flag) & 0x10) != 0);
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
    func_8003AA34();

    u32 fp = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x17F]);
    u16 count = (u16)func_8003B1EC(fp);

    CFloorMapMarkerEntry entries[32];
    u16 n = 0;
    for (u16 i = 1; i <= count; i++) {
        if (!func_8009CF8C(0x20C8 + i)) continue;
        if (func_801361E8(fp, &lbl_eu_8050BEA8[0x18C], i) != lbl_eu_80664798) continue;
        if (func_801361E8(fp, &lbl_eu_8050BEA8[0x192], i) == 2) continue;

        CFloorMapMarkerEntry* e = &entries[n];
        e->id = i;
        n++;
        e->type = func_801361E8(fp, &lbl_eu_8050BEA8[0x192], i);
        e->x = (f32)(s16)func_80136330(fp, &lbl_eu_8050BEA8[0x19B], i);
        e->y = (f32)(s16)func_80136330(fp, &lbl_eu_8050BEA8[0x1A0], i);
        e->z = (f32)(s16)func_80136330(fp, &lbl_eu_8050BEA8[0x1F], i);
        e->unk = (u32)func_8013639C((const void*)fp, (const void*)&lbl_eu_8050BEA8[0x1F], i);

        if (i == 0) {
            e->flag = 1;
        } else if (getUnk80664658()->field_214 & 0x48000) {
            e->flag = 1;
        } else {
            u16 left = func_80136254((const void*)fp, (const void*)&lbl_eu_8050BEA8[0x1AA], i);
            u16 right = func_80136254((const void*)fp, (const void*)&lbl_eu_8050BEA8[0x1B5], i);
            u16 pos = (u16)func_8009CF8C(0x20);
            e->flag = (left <= pos && pos <= right) ? 1 : 0;
        }
    }

    // Merge: walk the floors from the top down; the first floor whose height
    // is above the marker's y receives the marker appended to its list.
    CFloorMapCursor* cur = (CFloorMapCursor*)self;
    for (u8 j = 0; j < (u16)n; j++) {
        CFloorMapMarkerEntry* e = &entries[j];
        f32 y = e->y;
        for (s8 k = (s8)(cur->field_30D0 - 1); k >= 0; k--) {
            CFloorMapMarkerBlock* blk =
                (CFloorMapMarkerBlock*)((u8*)cur + k * 0x30C + 0x10);
            if (y >= (f32)blk->height) continue;

            CFloorMapMarkerEntry* rec = &blk->records[blk->count];
            rec->id = e->id;
            rec->type = e->type;
            rec->x = e->x;
            rec->y = e->y;
            rec->z = e->z;
            rec->unk = e->unk;
            rec->flag = e->flag;
            blk->count++;
            break;
        }
    }

    func_80246200(cur);

    // Refresh the cursor position: format the pane name for the current page
    // index, scale its translate by the zoom pane, and move the target pane.
    char buf[0x20];
    sprintf(buf, &lbl_eu_8050BEA8[0x14D], (s8)cur->field_0A);

    nw4r::lyt::Pane* pane = cur->mData->GetRootPane()->FindPaneByName(buf, 1);
    nw4r::math::VEC3 pos;
    func_801375A0(&pos, pane);

    nw4r::lyt::Pane* scalePane =
        cur->mData->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);
    pos.x *= scalePane->GetScale().x;

    nw4r::math::_VEC3 dest = pos;
    if (cur->field_3108) {
        CFloorMapCursorTarget* target = (CFloorMapCursorTarget*)cur->field_3108;
        target->pane->SetTranslate(*(nw4r::math::VEC3*)&dest);
    }

    if (cur->field_3134) {
        f32 vec[3] = {lbl_eu_80668768, lbl_eu_8066876C, lbl_eu_80668764};
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

void func_80246330(void* self){}

void* __dt__802468C8(void* self, int mode) {
    extern void* __dl__FPv(void*);
    if (self && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

void func_80246908(){}

void func_80247490(void* self, u8 arg2, u32 arg3, f32 arg4){}

void func_8024808C(void* self, void* arg2, u8 arg3) {
    extern void* func_8003AA34();
    extern void* getFP__FPCc(const char*);
    extern u32 func_801361E8(const char*, const char*, u32);
    extern u32 func_8009CF8C(u32);
    extern void func_80141DC4(f32*);
    extern s16 func_80136330(u32, const char*, u32);
    extern void* getPlayer__Q22cf13CfGameManagerFi(int);
    extern u32 func_8003B1EC(u32);
    extern f32 lbl_eu_80668764;
    extern f32 lbl_eu_80668778;
    extern const f64 lbl_eu_80668788;
    u8* p = (u8*)self;
    func_8003AA34();
    u32 fp = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x17F]);
    u8 map = func_801361E8((const char*)fp, &lbl_eu_8050BEA8[0x18C], *(u32*)((u8*)arg2 + 0x10));
    if (map != lbl_eu_80664798) return;
    if (!func_8009CF8C(0x20C8)) return;
    f32 buf[3];
    func_80141DC4(buf);
    u8 count = func_8003B1EC(fp);
    u8 r26 = 0;
    for (u8 i = 1; i <= count; i++) {
        s16 val = func_80136330(fp, &lbl_eu_8050BEA8[0x15A], i);
        if ((f32)(s16)val > buf[1]) {
            if (i == p[0x0C]) { r26 = 1; break; }
        }
    }
    if (r26) {
        p[0x0A]++;
    }
}

void func_8024830C(void* self, void* arg2) {
    extern void* getPlayer__Q22cf13CfGameManagerFi(int);
    extern s16 func_80136330(u32, const char*, u32);
    extern u32 func_8009CF8C(u32);
    extern void func_80141DC4(f32*);
    extern u32 func_801361E8(const char*, const char*, u32);
    extern u32 lbl_eu_80664184;
    extern u32 lbl_eu_806640A8;
    extern u16 lbl_eu_8050B798;
    extern f32 lbl_eu_80668764;
    extern f32 lbl_eu_80668778;
    extern f32 lbl_eu_8066877C;
    extern const f64 lbl_eu_80668788;
    u8* p = (u8*)self;
    f32* result = (f32*)p;
    result[0] = result[1] = result[2] = lbl_eu_80668764;
    void* slot = *(void**)((u8*)arg2 + 0x00);
    if (!slot) return;
    if ((lbl_eu_80664184 & 0xFF) == lbl_eu_80664798) {
        void* player = getPlayer__Q22cf13CfGameManagerFi(0);
        if (!player) return;
        void** vt = *(void***)player;
        f32* pos = (f32*)((void*(*)(void*))vt[0xAC])(player);
        result[0] = pos[0]; result[1] = pos[1]; result[2] = pos[2];
    } else {
        u16 idx = *(u16*)lbl_eu_8050B798;
        if (!idx) return;
        f32 buf[3];
        func_80141DC4(buf);
        result[0] = buf[0]; result[1] = buf[1]; result[2] = buf[2];
    }
    s16 val1 = func_80136330(*(u32*)lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1E2], lbl_eu_80664798);
    s16 val2 = func_80136330(*(u32*)lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1F0], lbl_eu_80664798);
    u8 region = func_801361E8(&lbl_eu_8050BEA8[0x1FE], &lbl_eu_8050BEA8[0x1F], lbl_eu_80664798);
    result[0] += (f32)(s16)val1 / ((f32)(s32)region * lbl_eu_80668778);
    result[1] -= (f32)(s16)val2 / ((f32)(s32)region * lbl_eu_80668778);
    result[2] = lbl_eu_80668764;
    if (*(void**)(p + 0x3108)) {
        void* obj = *(void**)((u8*)*(void**)(p + 0x3108) + 0x10);
        *(f32*)((u8*)obj + 0x2C) = result[0];
        *(f32*)((u8*)obj + 0x30) = result[1];
        *(f32*)((u8*)obj + 0x34) = result[2];
    }
    func_801F3850(*(void**)(p + 0x3134), (u16)(s16)p[0x0B]);
}

// Find the map row nearest the current map position: walk the row-marker
// panes (children of the pane found by the 0x26C name, linked through their
// raw ut::LinkListNode fields), accumulate each marker pane's parent-chain
// translate up to the layout root, and pick the first pane inside the hit
// radius. The trailing digits of that pane's name are the row id.
u32 func_80248558(void* self) {
    CFloorMapLayoutSlots* slots = (CFloorMapLayoutSlots*)self;
    nw4r::lyt::Pane* pane = slots->pane->FindPaneByName(&lbl_eu_8050BEA8[0x26C], 1);

    CFloorMapChildList* list = (CFloorMapChildList*)((u8*)pane + 0x14);
    CFloorMapChildNode* node = list->mpNext;
    CFloorMapChildNode* end = (CFloorMapChildNode*)list;

    f32 d = lbl_eu_80668790 + lbl_eu_80668790;
    f32 threshold = d * d;

    while (node != end) {
        if (node == NULL)
            Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052CB40, 0x23D, (const char*)lbl_eu_8052CB1C);

        nw4r::lyt::Pane* pane2 = slots->pane->FindPaneByName(node->name, 1);

        nw4r::lyt::Pane* target = slots->layout->GetRootPane();
        nw4r::math::VEC3 out;
        out.x = lbl_eu_80668764;
        out.y = lbl_eu_80668764;
        out.z = lbl_eu_80668764;
        if (target != pane2 && pane2 != NULL) {
            out.x = pane2->GetTranslate().x;
            out.y = pane2->GetTranslate().y;
            out.z = pane2->GetTranslate().z;

            nw4r::lyt::Pane* parent = pane2->GetParent();
            nw4r::math::VEC3 acc;
            acc.x = lbl_eu_80668764;
            acc.y = lbl_eu_80668764;
            acc.z = lbl_eu_80668764;
            if (target != parent && parent != NULL) {
                acc.x = parent->GetTranslate().x;
                acc.y = parent->GetTranslate().y;
                acc.z = parent->GetTranslate().z;

                nw4r::lyt::Pane* gp = parent->GetParent();
                nw4r::math::VEC3 acc2;
                acc2.x = lbl_eu_80668764;
                acc2.y = lbl_eu_80668764;
                acc2.z = lbl_eu_80668764;
                if (target != gp && gp != NULL) {
                    acc2.x = gp->GetTranslate().x;
                    acc2.y = gp->GetTranslate().y;
                    acc2.z = gp->GetTranslate().z;

                    nw4r::lyt::Pane* ggp = gp->GetParent();
                    nw4r::math::VEC3 acc3;
                    acc3.x = lbl_eu_80668764;
                    acc3.y = lbl_eu_80668764;
                    acc3.z = lbl_eu_80668764;
                    if (target != ggp && ggp != NULL) {
                        acc3.x = ggp->GetTranslate().x;
                        acc3.y = ggp->GetTranslate().y;
                        acc3.z = ggp->GetTranslate().z;
                        nw4r::math::VEC3 rec;
                        func_8024B4CC(&rec, self, ggp->GetParent());
                        nw4r::math::VEC3Add(&acc3, &acc3, &rec);
                    }
                    nw4r::math::VEC3Add(&acc2, &acc2, &acc3);
                }
                nw4r::math::VEC3Add(&acc, &acc, &acc2);
            }
            nw4r::math::VEC3Add(&out, &out, &acc);
        }

        f32 dy = -out.y;
        f32 dx = -out.x;
        f32 dist2 = dy * dy + dx * dx;
        if (dist2 <= threshold) {
            if (node == NULL)
                Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052CB40, 0x23D, (const char*)lbl_eu_8052CB1C);

            char buf2[0x40];
            sprintf(buf2, &lbl_eu_8050BEA8[0x223], node->name);

            // Keep only the digits of the formatted name (max 4) and parse them.
            char digits[4] = {0};
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

extern "C" void* func_80248920(void* self, const char* name, float x, float y, void* arg5, const char* paneName) {
    if (!name) return NULL;
    if (!paneName) return NULL;

    void* result = (char*)func_80136190(&lbl_eu_8050BEA8[0x2f6], &lbl_eu_8050BEA8[0x303], (u32)name);

    char buf[48];
    sprintf(buf, &lbl_eu_8050BEA8[0x30e], result);

    void* accessor = func_801355F4();
    typedef void* (*VFuncPtr4)(void*, u32, void*, u32);
    VFuncPtr4* vt = *(VFuncPtr4**)accessor;
    void* picture = vt[3](accessor, 0x74696d67, buf, 0);

    if (!picture) return NULL;

    void* pic = createPicture__10CLibLayoutFv();
    SetName__Q34nw4r3lyt4PaneFPCc(pic, paneName);

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

void func_80248A6C(){}

void func_80248ED8(){}

void func_80249344(){}

// Scan the marker table of the current map file and place one picture pane
// per eligible marker: each marker is accepted only after its map-id, random
// x-range, encounter-count, flag and position checks pass and the marker is
// not already placed; the picture is appended to the map pane whose name is
// formatted from the marker index.
void func_802497B0(void* self) {
    CFloorMapLayoutSlots* slots = (CFloorMapLayoutSlots*)self;

    u32 fp = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x315]);
    int count = (int)func_8003B1EC(fp);

    // Two s16/u8 -> f32 conversion slots (retail keeps both 0x4330 prefix
    // words stored from the prologue and alternates the slots).
    CFloorMapF64Conv convA;
    CFloorMapF64Conv convB;
    convA.w[0] = 0x43300000;
    convB.w[0] = 0x43300000;

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
        f32 fx = (f32)(convA.d - lbl_eu_80668770);
        convB.w[1] = (u32)(s16)y ^ 0x80000000;
        f32 fy = (f32)(convB.d - lbl_eu_80668770);
        convA.w[1] = (u32)(s16)z ^ 0x80000000;
        f32 fz = (f32)(convA.d - lbl_eu_80668770);
        nw4r::math::_VEC3 pos = { fx, fy, fz };

        u8 found = 0;
        u8 count2 = (u8)func_8003B1EC(lbl_eu_8066479C);
        for (u8 j = 1; j <= count2; j++) {
            s16 h = func_80136330(lbl_eu_8066479C, &lbl_eu_8050BEA8[0x15A], j);
            convB.w[1] = (u32)(s16)h ^ 0x80000000;
            if ((f32)(convB.d - lbl_eu_80668770) > fy) {
                if (j == slots->field_0C) found = 1;
                break;
            }
        }
        if (!found) continue;

        if (func_801361E8(fp, &lbl_eu_8050BEA8[0x37D], i) != 6) continue;

        char buf[0x20];
        sprintf(buf, &lbl_eu_8050BEA8[0x3A6], i);
        if (slots->layout->GetRootPane()->FindPaneByName(buf, 1)) continue;

        s16 vx = func_80136330(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1E2], lbl_eu_80664798);
        s16 vy = func_80136330(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1F0], lbl_eu_80664798);

        convA.w[1] = (u32)(s16)vx ^ 0x80000000;
        f32 rvx = (f32)(convA.d - lbl_eu_80668770);
        convB.w[1] = (u32)(s16)vy ^ 0x80000000;
        f32 rvy = (f32)(convB.d - lbl_eu_80668770);
        f32 coords[3] = { rvx, rvy, lbl_eu_80668764 };
        u8 region = (u8)func_801361E8(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1FE], lbl_eu_80664798);

        convA.w[1] = (u32)region;
        f32 scale = (f32)(convA.d - lbl_eu_80668788) * lbl_eu_80668778;
        nw4r::math::_VEC3 result = pos;
        result.y = coords[1] - (result.z / scale) * lbl_eu_8066877C;
        result.x = coords[0] + (result.x / scale) * lbl_eu_8066877C;
        result.z = lbl_eu_80668764;

        void* picture = func_80248920(self, (const char*)0x18, result.x, result.y, (void*)-1, buf);

        nw4r::lyt::Pane* pane2 = slots->pane->FindPaneByName(&lbl_eu_8050BEA8[0x293], 1);
        pane2->AppendChild((nw4r::lyt::Pane*)picture);
    }
}

void func_80249C1C(){}

void func_8024A448(){}

// Place one landmark marker on the current floor map: pick a random landmark
// row from the world table, format its display name by type (enemy lists,
// landmark table, save points, camps), then check the marker's floor against
// the current map row and append a positioned picture pane to the map layout.
void func_8024A748(void* self) {
    CFloorMapFull* p = (CFloorMapFull*)self;

    // Two s16/u8 -> f32 conversion slots (retail stores the 0x4330 prefix
    // words in the prologue, before the world-id check).
    CFloorMapF64Conv convA;
    CFloorMapF64Conv convB;
    convB.w[0] = 0x43300000;
    convA.w[0] = 0x43300000;
    u32 worldId = lbl_eu_80664184 & 0xFF;
    if (worldId != lbl_eu_80664798) return;

    u32 rnd = func_8009CF8C(0x20);
    if ((s32)rnd <= 0) return;
    if ((u8)func_8013600C(&lbl_eu_8050BEA8[0x44A], &lbl_eu_8050BEA8[0x457], rnd) != worldId) return;

    u32 id = func_8013600C(&lbl_eu_8050BEA8[0x44A], &lbl_eu_8050BEA8[0x37D], rnd);
    u16 type = func_8013606C(&lbl_eu_8050BEA8[0x44A], &lbl_eu_8050BEA8[0x3E6], rnd);

    nw4r::math::_VEC3 vec = { lbl_eu_80668764, lbl_eu_80668764, lbl_eu_80668764 };
    char buf[0x20];
    buf[0] = 0;

    if (type == 0) {
        if ((s32)(u8)id == 1) {
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
                sprintf(buf, &lbl_eu_8050BEA8[0x45B], g->m74);
                break;
            }
        }
    } else {
        switch ((s32)(u8)id) {
        case 2: {
            // Landmark-table entry: scan the 0x4C-byte rows for the id.
            CFloorMapCase2Elem* arr = (CFloorMapCase2Elem*)func_80193804();
            s32 count = *(s32*)((u8*)arr + 0x9800);
            for (CFloorMapCase2Elem* el = arr; el < arr + count; el++) {
                if (el->m1C != type) continue;
                vec.x = el->x;
                vec.y = el->y;
                vec.z = el->z;
                sprintf(buf, &lbl_eu_8050BEA8[0x3F9], el->m1C);
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
                if ((g->m64 & 0x4000) != 0 || (g->m64 & 0x8000) != 0) {
                    if (g->m8C == type) {
                        CFloorMapVec3* pos = g->GetPos();
                        vec.x = pos->x;
                        vec.y = pos->y;
                        vec.z = pos->z;
                        sprintf(buf, &lbl_eu_8050BEA8[0x3ED], g->m74);
                        break;
                    }
                }
            }
            break;
        }
        case 4: {
            u8 found = 0;
            CFloorMapObjList* glist = (CFloorMapObjList*)func_800B6BEC();
            for (CFloorMapObjNode* node = glist->head->next; node != glist->head;
                 node = node->next) {
                CFloorMapObj* g = (CFloorMapObj*)node->object;
                if (g->m8C == type) {
                    CFloorMapVec3* pos = g->GetPos();
                    vec.x = pos->x;
                    vec.y = pos->y;
                    vec.z = pos->z;
                    sprintf(buf, &lbl_eu_8050BEA8[0x3BC], g->m74);
                    found = 1;
                    break;
                }
            }
            if (!found) {
                // Fallback: resolve the landmark file entry and check whether
                // it is flagged for display on the current world.
                u32 fp = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x17F]);
                u16 id2 = func_8013606C(&lbl_eu_8050BEA8[0x3C6], &lbl_eu_8050BEA8[0x3D2], type);
                if ((u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x18C], id2) != lbl_eu_80664184) break;
                if (func_8009CF8C((u32)id2 + 0x20C8) == 0) break;

                u32 ok = 0;
                if (id2 == 0) {
                    ok = 1;
                } else if (getUnk80664658()->field_214 & 0x48000) {
                    ok = 1;
                } else {
                    u16 left = func_80136254((const void*)fp, &lbl_eu_8050BEA8[0x1AA], id2);
                    u16 right = func_80136254((const void*)fp, &lbl_eu_8050BEA8[0x1B5], id2);
                    u16 rnd2 = (u16)func_8009CF8C(0x20);
                    if (left <= rnd2 && rnd2 <= right) ok = 1;
                }
                if (ok) break;

                f32 tmp[3];
                func_80141DC4(tmp, id2);
                vec.x = tmp[0];
                vec.y = tmp[1];
                vec.z = tmp[2];
                sprintf(buf, &lbl_eu_8050BEA8[0x3BC], type);
            }
            break;
        }
        case 5: {
            u8 found = 0;
            CFloorMapObjList* glist = (CFloorMapObjList*)func_800B6BEC();
            for (CFloorMapObjNode* node = glist->head->next; node != glist->head;
                 node = node->next) {
                CFloorMapObj* g = (CFloorMapObj*)node->object;
                if (g->m8C == type) {
                    CFloorMapVec3* pos = g->GetPos();
                    vec.x = pos->x;
                    vec.y = pos->y;
                    vec.z = pos->z;
                    sprintf(buf, &lbl_eu_8050BEA8[0x3BC], g->m74);
                    found = 1;
                    break;
                }
            }
            if (!found) {
                u32 fp = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x17F]);
                u16 id2 = func_8013606C(&lbl_eu_8050BEA8[0x3C6], &lbl_eu_8050BEA8[0x3D2], type);
                if ((u8)func_801361E8(fp, &lbl_eu_8050BEA8[0x18C], id2) != lbl_eu_80664184) break;
                if (func_8009CF8C((u32)id2 + 0x20C8) == 0) break;

                u32 ok = 0;
                if (id2 == 0) {
                    ok = 1;
                } else if (getUnk80664658()->field_214 & 0x48000) {
                    ok = 1;
                } else {
                    u16 left = func_80136254((const void*)fp, &lbl_eu_8050BEA8[0x1AA], id2);
                    u16 right = func_80136254((const void*)fp, &lbl_eu_8050BEA8[0x1B5], id2);
                    u16 rnd2 = (u16)func_8009CF8C(0x20);
                    if (left <= rnd2 && rnd2 <= right) ok = 1;
                }
                if (ok) break;

                f32 tmp[3];
                func_80141DC4(tmp, id2);
                vec.x = tmp[0];
                vec.y = tmp[1];
                vec.z = tmp[2];
                sprintf(buf, &lbl_eu_8050BEA8[0x3BC], type);
            }
            break;
        }
        }
    }

    // The marker is only placed when its name was formatted and its floor is
    // the current map row (first floor above the marker's height).
    if (buf[0] != 0) {
        f32 y = vec.y;
        u8 count = (u8)func_8003B1EC(lbl_eu_8066479C);
        u8 found = 0;
        for (u8 j = 1; j <= count; j++) {
            s16 h = func_80136330(lbl_eu_8066479C, &lbl_eu_8050BEA8[0x15A], j);
            convB.w[0] = 0x43300000;
            convB.w[1] = (u32)(s16)h ^ 0x80000000;
            if ((f32)(convB.d - lbl_eu_80668770) > y) {
                if (j == p->field_0C) found = 1;
                break;
            }
        }
        if (found) {
            // Map the marker position onto the floor-map pane coordinates.
            s16 vx = func_80136330(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1E2], lbl_eu_80664798);
            s16 vy = func_80136330(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1F0], lbl_eu_80664798);
            convA.w[0] = 0x43300000;
            convA.w[1] = (u32)(s16)vx ^ 0x80000000;
            convB.w[0] = 0x43300000;
            convB.w[1] = (u32)(s16)vy ^ 0x80000000;
            f32 fvx = (f32)(convA.d - lbl_eu_80668770);
            f32 fvy = (f32)(convB.d - lbl_eu_80668770);
            u8 region = (u8)func_801361E8(lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1FE], lbl_eu_80664798);
            convA.w[0] = 0x43300000;
            convA.w[1] = (u32)region;
            f32 scale = (f32)(convA.d - lbl_eu_80668788) * lbl_eu_80668778;
            f32 rx = fvx + (vec.x / scale) * lbl_eu_8066877C;
            f32 ry = fvy - (vec.z / scale) * lbl_eu_8066877C;

            void* pic = func_80248920(p, (const char*)7, rx, ry, (void*)-1, &lbl_eu_8050BEA8[0x466]);
            nw4r::lyt::Pane* pane = p->field_08->FindPaneByName(&lbl_eu_8050BEA8[0x2BD], true);
            pane->AppendChild((nw4r::lyt::Pane*)pic);
        }
    }
}

void func_8024AEEC(){}

void func_8024B234(){}

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
            pane->SetVisible(__cntlzw(arg3 - i) & 0x20);
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
            pane->SetVisible(!(func_8009CF8C(0x20) < 0x171));
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

void __ct__CFloorMap(){}

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

void func_8024BE1C(){}


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
            pf->mFlag = (pf->mFlag & 0x7F) | found;
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
    if (*(void**)((u8*)p + 0x150)) {
        u8 i = 0;
        do {
            void* s = *(void**)((u8*)p + 0x150 + i * 8);
            func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(s), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
            i++;
        } while (i < p->field_1F0);
    }
    if (p->field_208) {
        if (p->mLayout1FC)
            func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(p->mLayout1FC), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
        if (p->mLayout32D4 && p->field_32E5)
            func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(p->mLayout32D4), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
        if (p->field_204 && p->mLayout32EC && p->field_32FD)
            func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(p->mLayout32EC), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
        if ((s8)p->field_206 >= 0) {
            u8 cond;
            if (p->mLayout32D4) {
                void** vtable = *(void***)(*(void**)((u8*)p->mLayout32D4 + 0x10));
                void* result = ((void*(*)(void*, const char*, u32))vtable[15])((void*)p->mLayout32D4, &lbl_eu_8050BEA8[0xEE], 1);
                cond = (*(u8*)((u8*)result + 0xBB)) & 1;
            } else {
                cond = 0;
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

void func_8024CB94(){}

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
void func_8024CE68(void* self) {
    extern int sprintf(char*, const char*, ...);
    CFloorMapFull* p = (CFloorMapFull*)self;
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
    char buf[0x20];
    sprintf(buf, &lbl_eu_8050BEA8[0x12A], (s8)p->field_205 + 1);

    nw4r::lyt::Layout* layout = (nw4r::lyt::Layout*)p->mLayout1FC;
    nw4r::lyt::Pane* pane = layout->GetRootPane()->FindPaneByName(buf, 1);
    nw4r::math::VEC3 pos;
    func_801375A0(&pos, pane);

    nw4r::lyt::Pane* scalePane = layout->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);
    pos.x *= scalePane->GetScale().x;
    if (p->mLayout32D4) {
        CFloorMapSetPanePos(((nw4r::lyt::Layout*)p->mLayout32D4)->GetRootPane(), pos);
    }

    s8 fi = (s8)p->field_205;
    CFloorMapFloorBlock* floor = (CFloorMapFloorBlock*)((u8*)p + fi * 0x30C);
    func_80136B4C((nw4r::lyt::Layout*)p->mLayout32D4, &lbl_eu_8050BEA8[0xF8], floor->name_210, 0);
    func_80136B4C((nw4r::lyt::Layout*)p->mLayout1FC, &lbl_eu_8050BEA8[0x145], func_8013639C((const void*)lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1F], lbl_eu_80664798), 0);
    func_80246200(&p->mLayout1FC);

    if (p->field_3330) {
        f32 vec[3] = {lbl_eu_80668768, lbl_eu_8066876C, lbl_eu_80668764};
        func_801F3670((void*)p->field_3330, vec);
        func_801F36BC((void*)p->field_3330, 5, floor->field_514);
        func_801F3850((void*)p->field_3330, 0);
    }

    ((nw4r::lyt::Layout*)p->mLayout32D4)->SetAnimationEnable((nw4r::lyt::AnimTransform*)p->mLayout32DC, false);
    ((nw4r::lyt::Layout*)p->mLayout32D4)->SetAnimationEnable((nw4r::lyt::AnimTransform*)p->mLayout32E0, true);
    p->field_32E4 = 1;
    func_801390E0__FPP11CFileHandle((CFileHandle**)&p->field_2C);

    u8 v26 = floor->field_20C;
    if (v26 == 0) v26 = 1;

    u16 v1 = func_80136254((const void*)lbl_eu_8066479C, &lbl_eu_8050BEA8[0x4AF], v26);
    u16 v2 = func_80136254((const void*)lbl_eu_8066479C, &lbl_eu_8050BEA8[0x4B5], v26);
    u8 v3 = func_801361E8(lbl_eu_8066479C, &lbl_eu_8050BEA8[0x4BC], v26);

    // Pick which map variant to load: weighted by the floor table's three
    // rates (v1/v2/v3) against two random draws.
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

    const char* key = NULL;
    switch (mode) {
    case 0: key = &lbl_eu_8050BEA8[0x4C1]; break;
    case 1: key = &lbl_eu_8050BEA8[0x4C8]; break;
    case 2: key = &lbl_eu_8050BEA8[0x4D0]; break;
    }

    char fname[0x20];
    sprintf(fname, &lbl_eu_8050BEA8[0x4D8], func_80138F78((u16)func_80136254((const void*)lbl_eu_8066479C, key, v26)));
    p->field_2C = (u32)readFile__11CDeviceFileFUlPCcP10IWorkEventii((u32)getHandleMEM2__Q23mtl10MemManagerFv(), fname, p, 0, 0);
    func_80138078(0xA);
}

// Move the map cursor to the next floor: increment the floor index (wrapping
// to the first floor), then refresh the floor name panes / scrollbar and
// (re)load the floor's map file.
void func_8024D23C(void* self) {
    extern int sprintf(char*, const char*, ...);
    CFloorMapFull* p = (CFloorMapFull*)self;
    if (!p->field_208) return;
    if (p->field_58) return;
    if (CSysWin_getUnk34(&p->mSysWinB8)) return;
    if (CSysWin_getUnk34(&p->mSysWinF4)) return;

    u8 nf = (u8)(p->field_205 + 1);
    p->field_205 = nf;
    if ((s8)nf >= p->field_32CC) {
        p->field_205 = 0;
    }

    p->field_206 = -1;
    p->field_207 = 0;
    char buf[0x20];
    sprintf(buf, &lbl_eu_8050BEA8[0x12A], (s8)p->field_205 + 1);

    nw4r::lyt::Layout* layout = (nw4r::lyt::Layout*)p->mLayout1FC;
    nw4r::lyt::Pane* pane = layout->GetRootPane()->FindPaneByName(buf, 1);
    nw4r::math::VEC3 pos;
    func_801375A0(&pos, pane);

    nw4r::lyt::Pane* scalePane = layout->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);
    pos.x *= scalePane->GetScale().x;
    if (p->mLayout32D4) {
        CFloorMapSetPanePos(((nw4r::lyt::Layout*)p->mLayout32D4)->GetRootPane(), pos);
    }

    s8 fi = (s8)p->field_205;
    CFloorMapFloorBlock* floor = (CFloorMapFloorBlock*)((u8*)p + fi * 0x30C);
    func_80136B4C((nw4r::lyt::Layout*)p->mLayout32D4, &lbl_eu_8050BEA8[0xF8], floor->name_210, 0);
    func_80136B4C((nw4r::lyt::Layout*)p->mLayout1FC, &lbl_eu_8050BEA8[0x145], func_8013639C((const void*)lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1F], lbl_eu_80664798), 0);
    func_80246200(&p->mLayout1FC);

    if (p->field_3330) {
        f32 vec[3] = {lbl_eu_80668768, lbl_eu_8066876C, lbl_eu_80668764};
        func_801F3670((void*)p->field_3330, vec);
        func_801F36BC((void*)p->field_3330, 5, floor->field_514);
        func_801F3850((void*)p->field_3330, 0);
    }

    ((nw4r::lyt::Layout*)p->mLayout32D4)->SetAnimationEnable((nw4r::lyt::AnimTransform*)p->mLayout32DC, false);
    ((nw4r::lyt::Layout*)p->mLayout32D4)->SetAnimationEnable((nw4r::lyt::AnimTransform*)p->mLayout32E0, true);
    p->field_32E4 = 1;
    func_801390E0__FPP11CFileHandle((CFileHandle**)&p->field_2C);

    u8 v26 = floor->field_20C;
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

    const char* key = NULL;
    switch (mode) {
    case 0: key = &lbl_eu_8050BEA8[0x4C1]; break;
    case 1: key = &lbl_eu_8050BEA8[0x4C8]; break;
    case 2: key = &lbl_eu_8050BEA8[0x4D0]; break;
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
    if (p->field_2C) return;
    if (!p->field_208) return;
    if (CSysWin_getUnk34(&p->mSysWinF4)) return;

    if (p->field_58) {
        if (!CSysWin_isActive(&p->mSysWinB8)) return;

        u8 nc = (u8)(p->field_5C - 1);
        p->field_5C = nc;
        if ((s8)nc < 0) p->field_5C = 1;

        nw4r::math::VEC3 out;
        func_8022C1B4(&out, &p->mSysWinB8, p->field_5C);
        ((CCur18View*)&p->mCursor)->v02(&out);
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

    nw4r::math::VEC3 pos;
    pos.x = lbl_eu_80668764;
    pos.y = lbl_eu_80668764;
    pos.z = lbl_eu_80668764;
    char buf[0x20];
    sprintf(buf, &lbl_eu_8050BEA8[0x2EB]);

    CFloorMapLayoutSlots* slots = (CFloorMapLayoutSlots*)&p->mLayout140;
    nw4r::lyt::Pane* pane = slots->layout->GetRootPane()->FindPaneByName(buf, 1);
    if (pane) {
        nw4r::lyt::Pane* target = slots->layout->GetRootPane();
        nw4r::math::VEC3 out;
        out.x = lbl_eu_80668764;
        out.y = lbl_eu_80668764;
        out.z = lbl_eu_80668764;
        if (target != pane && pane != NULL) {
            out.x = pane->GetTranslate().x;
            out.y = pane->GetTranslate().y;
            out.z = pane->GetTranslate().z;

            nw4r::lyt::Pane* parent = pane->GetParent();
            nw4r::math::VEC3 acc;
            acc.x = lbl_eu_80668764;
            acc.y = lbl_eu_80668764;
            acc.z = lbl_eu_80668764;
            if (target != parent && parent != NULL) {
                acc.x = parent->GetTranslate().x;
                acc.y = parent->GetTranslate().y;
                acc.z = parent->GetTranslate().z;

                nw4r::lyt::Pane* gp = parent->GetParent();
                nw4r::math::VEC3 acc2;
                acc2.x = lbl_eu_80668764;
                acc2.y = lbl_eu_80668764;
                acc2.z = lbl_eu_80668764;
                if (target != gp && gp != NULL) {
                    acc2.x = gp->GetTranslate().x;
                    acc2.y = gp->GetTranslate().y;
                    acc2.z = gp->GetTranslate().z;

                    nw4r::lyt::Pane* ggp = gp->GetParent();
                    nw4r::math::VEC3 acc3;
                    acc3.x = lbl_eu_80668764;
                    acc3.y = lbl_eu_80668764;
                    acc3.z = lbl_eu_80668764;
                    if (target != ggp && ggp != NULL) {
                        acc3.x = ggp->GetTranslate().x;
                        acc3.y = ggp->GetTranslate().y;
                        acc3.z = ggp->GetTranslate().z;
                        nw4r::math::VEC3 rec;
                        func_8024B4CC(&rec, slots, ggp->GetParent());
                        nw4r::math::VEC3Add(&acc3, &acc3, &rec);
                    }
                    nw4r::math::VEC3Add(&acc2, &acc2, &acc3);
                }
                nw4r::math::VEC3Add(&acc, &acc, &acc2);
            }
            nw4r::math::VEC3Add(&out, &out, &acc);
        }
        pos = out;

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

// Per-frame map update for the world-map view: when the syswin cursor is
// active, step the cursor index (wrapping to 0); otherwise advance the map
// rows and move the map view to the row under the cursor (same position
// computation as func_8024F7CC).
void func_8024DA0C(void* self) {
    char buf[0x20];
    nw4r::math::VEC3 pos;
    nw4r::math::VEC3 cursorOut;
    if (((CFloorMapFull*)self)->field_2C) return;
    if (!((CFloorMapFull*)self)->field_208) return;
    if (CSysWin_getUnk34(&((CFloorMapFull*)self)->mSysWinF4)) return;

    CFloorMapFull* p = (CFloorMapFull*)self;
    if (p->field_58) {
        if (!CSysWin_isActive(&p->mSysWinB8)) return;

        // Wrap the cursor index 0 -> 1 -> 0 (syswin cursor mode).
        u8 nc = (u8)(p->field_5C + 1);
        p->field_5C = nc;
        if ((s8)nc >= 2) p->field_5C = 0;

        func_8022C1B4(&cursorOut, &p->mSysWinB8, p->field_5C);
        ((CCur18View*)&p->mCursor)->v02(&cursorOut);
        func_80138078(1);
        return;
    }

    s8 fi = (s8)p->field_205;
    CFloorMapFloorBlock* floor = (CFloorMapFloorBlock*)((u8*)p + fi * 0x30C);
    if (!floor->field_514) return;

    func_80245450(&p->mLayout1FC);

    u16 rowId;
    s8 rowIdx = (s8)p->field_206;
    if (rowIdx >= 0) {
        rowId = ((CFloorMapFloorBlock*)((u8*)p + (s8)p->field_205 * 0x30C))->rows[(s8)p->field_207 + rowIdx].field_00;
    } else {
        rowId = 0;
    }

    pos.x = lbl_eu_80668764;
    pos.y = lbl_eu_80668764;
    pos.z = lbl_eu_80668764;
    CFloorMapLayoutSlots* slots = (CFloorMapLayoutSlots*)&p->mLayout140;
    sprintf(buf, &lbl_eu_8050BEA8[0x2EB], rowId);

    nw4r::lyt::Pane* pane = slots->layout->GetRootPane()->FindPaneByName(buf, 1);
    if (pane) {
        // Accumulate the pane's parent-chain translate (excluding the layout
        // root); the early exits keep the lower accumulators at {0,0,0}.
        nw4r::lyt::Pane* target = slots->layout->GetRootPane();
        nw4r::lyt::Pane* parent;
        nw4r::lyt::Pane* gp;
        nw4r::lyt::Pane* ggp;
        nw4r::math::VEC3 acc;
        nw4r::math::VEC3 acc2;
        nw4r::math::VEC3 acc3;
        nw4r::math::VEC3 rec;
        nw4r::math::VEC3 out;
        out.x = lbl_eu_80668764;
        out.y = lbl_eu_80668764;
        out.z = lbl_eu_80668764;
        if (target == pane || pane == NULL) goto set_pos;
        out.x = pane->GetTranslate().x;
        out.y = pane->GetTranslate().y;
        out.z = pane->GetTranslate().z;

        parent = pane->GetParent();
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
        pos = out;

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

    // Refresh the cursor position: format the pane name for the current page
    // index, scale its translate by the zoom pane, and move the target pane.
    char buf[0x20];
    sprintf(buf, &lbl_eu_8050BEA8[0x14D], (s8)p->field_206);

    nw4r::lyt::Layout* layout = (nw4r::lyt::Layout*)p->mLayout1FC;
    nw4r::lyt::Pane* pane = layout->GetRootPane()->FindPaneByName(buf, 1);
    nw4r::math::VEC3 pos;
    func_801375A0(&pos, pane);
    nw4r::lyt::Pane* scalePane =
        layout->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);
    pos.x *= scalePane->GetScale().x;
    nw4r::math::_VEC3 dest = pos;
    if (p->mLayout3304)
        CFloorMapSetPanePos(((CFloorMapLayoutHolder*)p->mLayout3304)->pane, dest);

    func_801F3850((void*)p->field_3330, (u16)(s8)p->field_207);

    u16 rowId = 0;
    if ((s8)p->field_206 >= 0) {
        rowId = ((CFloorMapFloorBlock*)((u8*)p + (s8)p->field_205 * 0x30C))
                    ->rows[(s8)p->field_207 + (s8)p->field_206].field_00;
    }

    // Move the map view to the row under the cursor: accumulate the row
    // pane's parent-chain translate (excluding the layout root), scale by the
    // zoom pane and add the root offset.
    nw4r::math::VEC3 vec;
    vec.x = lbl_eu_80668764;
    vec.y = lbl_eu_80668764;
    vec.z = lbl_eu_80668764;
    CFloorMapLayoutSlots* slots = (CFloorMapLayoutSlots*)&p->mLayout140;
    char buf2[0x20];
    sprintf(buf2, &lbl_eu_8050BEA8[0x2EB]);

    nw4r::lyt::Pane* pane2 = slots->layout->GetRootPane()->FindPaneByName(buf2, 1);
    if (pane2) {
        nw4r::lyt::Pane* target = slots->layout->GetRootPane();
        nw4r::lyt::Pane* parent;
        nw4r::lyt::Pane* gp;
        nw4r::lyt::Pane* ggp;
        nw4r::math::VEC3 acc;
        nw4r::math::VEC3 acc2;
        nw4r::math::VEC3 acc3;
        nw4r::math::VEC3 rec;
        nw4r::math::VEC3 out;
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

        nw4r::lyt::Pane* scalePane2 =
            slots->layout->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);
        vec.x *= scalePane2->GetScale().x;
        vec += slots->layout->GetRootPane()->GetTranslate();
    }

    p->field_54 = lbl_eu_806687A0;
    p->field_41 = 2;
    p->field_4C = vec.x / lbl_eu_806687A0;
    p->field_50 = vec.y / lbl_eu_806687A0;
    func_80138078(1);
}

void func_8024E2BC(void* self) {
    extern void func_802455F0(void*);
    extern int sprintf(char*, const char*, ...);
    extern void func_8024B4CC(void*, void*, void*);
    extern f32 lbl_eu_80668764;
    extern f32 lbl_eu_806687BC;
    u8* p = (u8*)self;
    if (*(u32*)(p + 0x2C)) return;
    if (!p[0x208]) return;
    if (CSysWin_getUnk34(p + 0xF4)) return;
    if (p[0x58]) return;
    s8 idx0 = (s8)p[0x205];
    u32 base = idx0 * 0x30C;
    if (!*(u8*)(p + base + 0x514)) return;
    func_802455F0(p + 0x1FC);
    s8 idx1 = (s8)p[0x206];
    u16 val = 0;
    if (idx1 >= 0) {
        s8 idx2 = (s8)p[0x207];
        val = *(u16*)(p + base + (idx2 + idx1) * 0x18 + 0x214);
    }
    if (!val) return;
    f32 pos[3] = {lbl_eu_80668764, lbl_eu_80668764, lbl_eu_80668764};
    char buf[0x20];
    sprintf(buf, &lbl_eu_8050BEA8[0x2EB]);
    void* data = *(void**)(p + 0x140);
    void* obj = *(void**)((u8*)data + 0x10);
    VFuncPtr* vtable = *(VFuncPtr**)obj;
    void* result = vtable[15](obj, buf, 1);
    if (result) {
        void* target = *(void**)((u8*)data + 0x10);
        void* node = result;
        if (node && *(void**)((u8*)node + 0x10) != target) {
            void* next = *(void**)((u8*)node + 0x0C);
            if (next && *(void**)((u8*)next + 0x10) != target) {
                void* next2 = *(void**)((u8*)next + 0x0C);
                if (next2 && *(void**)((u8*)next2 + 0x10) != target) {
                    f32 p1[3];
                    func_8024B4CC(p1, data, next2);
                    pos[0] += p1[0]; pos[1] += p1[1]; pos[2] += p1[2];
                }
                pos[0] += *(f32*)((u8*)next2 + 0x2C);
                pos[1] += *(f32*)((u8*)next2 + 0x30);
                pos[2] += *(f32*)((u8*)next2 + 0x34);
            }
            pos[0] += *(f32*)((u8*)next + 0x2C);
            pos[1] += *(f32*)((u8*)next + 0x30);
            pos[2] += *(f32*)((u8*)next + 0x34);
        }
        void* result2 = vtable[15](obj, &lbl_eu_8050BEA8[0x136], 1);
        if (result2) {
            f32 scale = *(f32*)((u8*)result2 + 0x44);
            pos[0] *= scale;
            pos[0] += *(f32*)((u8*)data + 0x2C);
            pos[1] += *(f32*)((u8*)data + 0x30);
            pos[2] += *(f32*)((u8*)data + 0x34);
        }
    }
    *(f32*)(p + 0x54) = lbl_eu_806687BC;
    p[0x41] = 2;
    *(f32*)(p + 0x4C) = pos[0] / lbl_eu_806687BC;
    *(f32*)(p + 0x50) = pos[1] / lbl_eu_806687BC;
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
    nw4r::lyt::Pane* mapPane =
        (fm->mLayout130 ? (CFloorMapLayoutHolder*)fm->mLayout130 : NULL)->pane;
    nw4r::lyt::Pane* scalePane =
        mapPane->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);

    f32 right = lbl_eu_806687B0 * scalePane->GetScale().x - lbl_eu_806687B4;
    f32 x = fm->pos_x_44 + lbl_eu_806687A8;
    f32 limit = lbl_eu_80668798 * right;
    fm->pos_x_44 = x;
    if (x >= limit) fm->pos_x_44 = limit;

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


void func_8024EC24(void* self) {
    extern f32 lbl_eu_80668798;
    extern f32 lbl_eu_806687A8;
    extern f32 lbl_eu_806687B0;
    extern f32 lbl_eu_806687B4;
    extern f32 lbl_eu_806687B8;
    extern f32 lbl_eu_80668764;
    u8* p = (u8*)self;
    if (p[0x58] || CSysWin_getUnk34(p + 0xB8) || CSysWin_getUnk34(p + 0xF4)) return;
    void* slot = *(void**)(p + 0x130);
    if (!slot) slot = 0;
    void* obj = *(void**)((u8*)slot + 0x10);
    VFuncPtr* vt = *(VFuncPtr**)obj;
    void* result = vt[15](obj, &lbl_eu_8050BEA8[0x136], 1);
    f32 f4 = lbl_eu_806687B0 * *(f32*)((u8*)result + 0x44) - lbl_eu_806687B4;
    f32 f3 = lbl_eu_80668798 * f4;
    f32* pos = (f32*)(p + 0x44);
    *pos -= lbl_eu_806687A8;
    f32 limit = lbl_eu_806687B8 * f3;
    if (*pos < limit) *pos = limit;
    for (int i = 0; i < 3; i++) {
        void* s = *(void**)(p + 0x130 + i * 8);
        if (s) {
            void* o = *(void**)((u8*)s + 0x10);
            *(f32*)((u8*)o + 0x2C) = pos[0];
            *(f32*)((u8*)o + 0x30) = pos[1];
            *(f32*)((u8*)o + 0x34) = lbl_eu_80668764;
        }
    }
    void* s = *(void**)(p + 0x150);
    if (s) {
        void* o = *(void**)((u8*)s + 0x10);
        *(f32*)((u8*)o + 0x2C) = pos[0];
        *(f32*)((u8*)o + 0x30) = pos[1];
        *(f32*)((u8*)o + 0x34) = lbl_eu_80668764;
    }
    for (u8 i = 0; i < p[0x1F0]; i++) {
        void* s2 = *(void**)(p + 0x150 + i * 8);
        if (s2) {
            void* o = *(void**)((u8*)s2 + 0x10);
            *(f32*)((u8*)o + 0x2C) = pos[0];
            *(f32*)((u8*)o + 0x30) = pos[1];
            *(f32*)((u8*)o + 0x34) = lbl_eu_80668764;
        }
    }
    func_80248558(p + 0x140);
    func_8024577C(p + 0x1FC, *(u16*)(p + 0x5A));
    p[0x5D] = 1;
}

void func_8024EE50(){}

void func_8024F1FC(void* self, u32 arg2) {
    extern void* func_8003AA34();
    extern void* getFP__FPCc(const char*);
    extern u8 lbl_eu_80664798;
    lbl_eu_80664798 = (u8)arg2;
    if (arg2 > 0x1C) return;
    func_8003AA34();
    u32 strs[] = {
        0x524, 0x534, 0x544, 0x554, 0x564, 0x574, 0x584, 0x594,
        0x5A4, 0x5B4, 0x5C4, 0x5D4, 0x5E4, 0x5F4, 0x604, 0x614,
        0x624, 0x634, 0x634, 0x644, 0x654, 0x664, 0x674, 0x684,
        0x634, 0x644, 0x654, 0x664
    };
    lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[strs[arg2]]);
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
    extern "C" void func_801D216C(void*, int);
    extern "C" void func_8022B8E4(void*);
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
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, char const*, void*, int, int);
void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, char const*, void*, int, int);
int func_800A9D90();
void func_801F34F4(void*);

void func_8024C104(void* self) {
    u8* p = (u8*)self;
    u32 handle = (u32)getHandleMEM2__Q23mtl10MemManagerFv();
    *(void**)(p + 0x24) = readFile__11CDeviceFileFUlPCcP10IWorkEventii(handle, &lbl_eu_8050BEA8[0x4e7], self, 0, 0);
    u32 handle2 = func_800A9D90();
    *(void**)(p + 0x30) = readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(handle2, &lbl_eu_8050BEA8[0x4fc], self, 0, 0);
    u32 buffer[29];
    u32* dst = &buffer[1];
    u32* src = lbl_eu_8050BDF8;
    buffer[0] = 0;
    int count = 14;
    do {
        *dst++ = *src++;
        *dst++ = *src++;
    } while (--count);
    handle = (u32)getHandleMEM2__Q23mtl10MemManagerFv();
    u8 idx = lbl_eu_80664798;
    *(void**)(p + 0x28) = readFile__11CDeviceFileFUlPCcP10IWorkEventii(handle, (char*)buffer[idx], self, 0, 0);
    func_801F34F4(p + 0x60);
    typedef void (*VoidVFuncPtr)(void*);
    VoidVFuncPtr* vt = *(VoidVFuncPtr**)(p + 0xB8);
    vt[0x20](p + 0xB8);
    vt = *(VoidVFuncPtr**)(p + 0xF4);
    vt[0x20](p + 0xF4);
}


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
    u32 result = __cntlzw(p[0x208]);
    int shifted = result >> 5;
    p[0x208] = shifted;
}

u8 func_8024F6BC(void* self) {
    CFloorMapFull* full = static_cast<CFloorMapFull*>(self);
    if (full->field_58) return 0;
    return full->field_208;
}

extern "C" u8 func_8024F6D8(CFloorMapFull* self) { return self->field_208; }

u32 func_8024F6E0(void* self) {
    s8 idx1 = *(s8*)((u8*)self + 0x206);
    u16 val;
    if (idx1 < 0) {
        val = 0;
    } else {
        s8 idx0 = *(s8*)((u8*)self + 0x205);
        s8 idx2 = *(s8*)((u8*)self + 0x207);
        u32 offset = idx0 * 0x30C + (idx2 + idx1) * 0x18;
        val = *(u16*)((u8*)self + offset + 0x214);
    }
    return val != 0 ? 1 : 0;
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

    nw4r::math::VEC3 pos;
    pos.x = lbl_eu_80668764;
    pos.y = lbl_eu_80668764;
    pos.z = lbl_eu_80668764;
    char buf[0x20];
    sprintf(buf, &lbl_eu_8050BEA8[0x2EB]);

    CFloorMapLayoutSlots* slots = (CFloorMapLayoutSlots*)&p->mLayout140;
    nw4r::lyt::Pane* pane = slots->layout->GetRootPane()->FindPaneByName(buf, 1);
    if (pane) {
        // Accumulate the pane's parent-chain translate (excluding the layout
        // root); the early exits keep the lower accumulators at {0,0,0}.
        nw4r::lyt::Pane* target = slots->layout->GetRootPane();
        nw4r::lyt::Pane* parent;
        nw4r::lyt::Pane* gp;
        nw4r::lyt::Pane* ggp;
        nw4r::math::VEC3 out;
        out.x = lbl_eu_80668764;
        out.y = lbl_eu_80668764;
        out.z = lbl_eu_80668764;
        if (target == pane || pane == NULL) goto set_pos;
        out.x = pane->GetTranslate().x;
        out.y = pane->GetTranslate().y;
        out.z = pane->GetTranslate().z;

        parent = pane->GetParent();
        nw4r::math::VEC3 acc;
        acc.x = lbl_eu_80668764;
        acc.y = lbl_eu_80668764;
        acc.z = lbl_eu_80668764;
        if (target == parent || parent == NULL) goto add_out;
        acc.x = parent->GetTranslate().x;
        acc.y = parent->GetTranslate().y;
        acc.z = parent->GetTranslate().z;

        gp = parent->GetParent();
        nw4r::math::VEC3 acc2;
        acc2.x = lbl_eu_80668764;
        acc2.y = lbl_eu_80668764;
        acc2.z = lbl_eu_80668764;
        if (target == gp || gp == NULL) goto add_acc;
        acc2.x = gp->GetTranslate().x;
        acc2.y = gp->GetTranslate().y;
        acc2.z = gp->GetTranslate().z;

        ggp = gp->GetParent();
        nw4r::math::VEC3 acc3;
        acc3.x = lbl_eu_80668764;
        acc3.y = lbl_eu_80668764;
        acc3.z = lbl_eu_80668764;
        if (target == ggp || ggp == NULL) goto add_acc2;
        acc3.x = ggp->GetTranslate().x;
        acc3.y = ggp->GetTranslate().y;
        acc3.z = ggp->GetTranslate().z;
        nw4r::math::VEC3 rec;
        func_8024B4CC(&rec, slots, ggp->GetParent());
        nw4r::math::VEC3Add(&acc3, &acc3, &rec);
    add_acc2:
        nw4r::math::VEC3Add(&acc2, &acc2, &acc3);
    add_acc:
        nw4r::math::VEC3Add(&acc, &acc, &acc2);
    add_out:
        nw4r::math::VEC3Add(&out, &out, &acc);
    set_pos:
        pos = out;

        nw4r::lyt::Pane* scalePane = slots->layout->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);
        pos.x *= scalePane->GetScale().x;
        pos += slots->layout->GetRootPane()->GetTranslate();
    }

    p->field_54 = lbl_eu_806687BC;
    p->field_41 = 2;
    p->field_4C = pos.x / lbl_eu_806687BC;
    p->field_50 = pos.y / lbl_eu_806687BC;
}

u32 func_8024FB78(void* self) {
    extern u32 lbl_eu_80664184;
    extern u8 lbl_eu_80664798;
    extern u32 lbl_eu_8066479C;
    extern void* getPlayer__Q22cf13CfGameManagerFi(int);
    extern u32 func_8003B1EC(u32);
    extern s16 func_80136330(u32, const char*, u32);
    extern void func_80141DC4(f32*);
    extern const f64 lbl_eu_80668770;
    u32 result = 0;
    if ((lbl_eu_80664184 & 0xFF) == lbl_eu_80664798) {
        if (!lbl_eu_8066479C) return 0;
        void* player = getPlayer__Q22cf13CfGameManagerFi(0);
        if (!player) return 0;
        void** vt = *(void***)player;
        f32* pos = (f32*)((void*(*)(void*))vt[0xAC])(player);
        f32 y = pos[1];
        u32 count = func_8003B1EC(lbl_eu_8066479C);
        for (u32 i = 1; i <= count; i++) {
            s16 val = func_80136330(lbl_eu_8066479C, &lbl_eu_8050BEA8[0x15A], i);
            if ((f32)(s16)val > y) return i;
        }
    } else {
        if (!lbl_eu_8066479C) return 0;
        f32 buf[3];
        func_80141DC4(buf);
        u32 count = func_8003B1EC(lbl_eu_8066479C);
        for (u32 i = 1; i <= count; i++) {
            s16 val = func_80136330(lbl_eu_8066479C, &lbl_eu_8050BEA8[0x15A], i);
            if ((f32)(s16)val > buf[1]) return i;
        }
    }
    return 0;
}

// File-load completion handler: dispatch on the four file handles. The first
// two build the map layouts and row-marker panes, the third swaps the map
// buffer and rebuilds the current floor's marker rows, the fourth parses the
// auxiliary world-map file. Returns 1 when the event was consumed, 0 otherwise.
u32 CFloorMap::OnFileEvent(CEventFile* event) {
    CFileHandle* handle = event->mFileHandle;

    // Event for the floor-list file (field_24): open a scratch region, load
    // the two map layouts, build the row-marker pane list and the embedded
    // cursor, then push the cursor position into every map layout.
    if ((CFileHandle*)this->field_24 == handle) {
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
        func_80246330(&this->layouts_150[0]);

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
    if ((CFileHandle*)this->field_28 == handle) {
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
    if ((CFileHandle*)this->field_2C == handle) {
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
            // The row id is computed but only drives the (retail) flow;
            // func_8024808C actually receives the row's type byte and body.
            u16 rowId;
            if (i == 0xFF) {
                if ((s8)this->field_206 >= 0) {
                    rowId = fl->rows[(s8)this->field_207 + (s8)this->field_206].field_00;
                } else {
                    rowId = 0;
                }
            } else {
                rowId = fl->rows[i].field_00;
            }
            func_8024808C(&this->layout_140, (u8*)&fl->rows[i].field_04,
                          fl->rows[i].field_02);
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
    if ((CFileHandle*)this->field_30 == handle) {
        CFileHandle* file = (CFileHandle*)this->field_30;
        u32 zero = 0;
        CBdat::func_8003AA78(2, file->getData());
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
extern "C" void func_80244AE8() {}
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
    CFloorMapFloorEntry* entries = reinterpret_cast<CFloorMapFloorEntry*>(cur->_0C);
    u8 count =
        reinterpret_cast<CFloorMapFloorEntryView*>(&entries[cur->field_09])->count;
    // Page up by one row; wrap to the previous page once the row index
    // underflows, then to the last page/row of the floor.
    u8 a1 = (u8)(cur->field_0A - 1);
    cur->field_0A = a1;
    if ((s8)a1 < 0) {
        u8 b1 = (u8)(cur->field_0B - 1);
        cur->field_0A = 0;
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
    nw4r::math::VEC3 pos;
    nw4r::math::VEC3 dest;
    char buf[0x20];
    sprintf(buf, &lbl_eu_8050BEA8[0x14D], (s8)cur->field_0A);

    nw4r::lyt::Pane* pane = cur->mData->GetRootPane()->FindPaneByName(buf, 1);
    func_801375A0(&pos, pane);

    nw4r::lyt::Pane* scalePane =
        cur->mData->GetRootPane()->FindPaneByName(&lbl_eu_8050BEA8[0x136], 1);
    pos.x *= scalePane->GetScale().x;

    dest = pos;
    if (cur->field_3108) {
        CFloorMapCursorTarget* target = (CFloorMapCursorTarget*)cur->field_3108;
        target->pane->SetTranslate(dest);
    }

    func_801F3850(cur->field_3134, (u16)(s8)cur->field_0B);
}
