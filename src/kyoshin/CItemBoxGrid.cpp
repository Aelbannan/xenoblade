// Auto-scaffolded catalog TU for kyoshin/CItemBoxGrid
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CItemBoxGrid.hpp"
#include "kyoshin/CExchangeWin.hpp"
#include <stdio.h>
#include <string.h>
#include <nw4r/lyt.h>

// Forward declarations for external functions
void* __dt__13CArtsBookItemFv(void* self, int mode);
void* __dt__11CVisionItemFv(void* self, int mode);
void* __dt__10CQuestItemFv(void* self, int mode);

void func_80136910(nw4r::lyt::Layout*, char*, u8);

// Forward declarations for functions defined later in this file
u32 func_801C7958(void* self, void* item);
u32 func_801CA070(void* self, void* item);
u32 func_801CA110(void* self, void* entry);
s32 func_801C7C7C(void* self, u32 id, void* item);
u32 func_801C6938(void* self, u32 idx);
u32 func_801C62AC(CItemBoxGridFull* self, u16 idx);
void* func_801C631C(CItemBoxGridFull* self, u16 idx);
s32 func_801C6388(CItemBoxGridFull* self, u16 idx);
long func_801C6158(double f);
u8 func_801C6528(CItemBoxGridFull* self, u16 idx);
u8 func_801C65A0(CItemBoxGridFull* self, u16 idx);
u8 func_801C6618(CItemBoxGridFull* self, u16 idx);
u8 func_801C6708(CItemBoxGridFull* self, u16 idx);
u8 func_801C673C(CItemBoxGridFull* self, u16 idx);
extern "C" __declspec(noinline) u8 func_801C6840(CItemBoxGridFull* self);
extern "C" __declspec(noinline) u8 func_801C67F8(CItemBoxGridFull* self);
u32 func_801C5FC0(CItemBoxGridFull* self, u16 idx);
u32 func_801C618C(void* self, u32 id, void* item, int r6);
void func_801C6EC0(CItemBoxGridFull* self, u16 idx);
int LookupIndexedByte(char* obj);
extern "C" {
void func_801CC3F4(void*);
void func_801CC4E8(void*);
void func_801CC5DC(void*);
void func_801CC7B0(void*, int);
void func_801CB9D8(u32*, void*, u32);
void func_801CE108(void*);
void func_801CE1A0(void*);
void CheckState4_Animate(char*);
void func_801CE2F8(void*);
void func_801CE3E8(void*);
void func_801CE4B4(void*);
void func_801CE524(void*);
void func_801CE974(void*);
void func_801CE9E8(void*);
void func_801CEA30(void*);
void func_801CEAA0(void*);
void func_801CEAE8(void*);
void func_801CEB3C(void*);
void func_801CEBF0(void*);
void func_801CEC80(void*);
void func_801CDB94(void*, u32);
}
void func_801CB9D8(u32*, void*, u32);
void func_801CBA04(void*);
void func_8003AA8C__5CBdatFUl(u32);
void func_801390E0__FPP11CFileHandle(void**);
void func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(void*);
void func_8045F778__17UnkClass_8045F564Fv(void*);
void func_8022B7F4(void*);
void func_801CECD0(CItemBoxGridFull* self, u32 kind, void* item, u16 idx, u32 bt);
void func_801CF240(CItemBoxGridFull* self, u32 kind, void* item, u16 idx);
void func_801CF71C(void* self, int r4, void* r5, int r6);
void func_801CF900(void* self, u32 r4, void* r5, void* r6, u32 r7);
void func_801CFCBC(void* self, u32 val, u32 idx);
u32 func_801C5E5C(void* self);
char* func_801C6A44(void* self, u16 idx);
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
extern u32 lbl_eu_806640F4;
// Forward declarations for later-defined functions
void func_801CC3F4(void*);
void func_801CC4E8(void*);
void func_801CC5DC(void*);
void func_801CC7B0(void*, int);
void func_801CB9D8(u32*, void*, u32);
void CheckState4_Animate(char*);
void func_801CE108(void*);
void func_801CE1A0(void*);
void func_801CE2F8(void*);
void func_801CE3E8(void*);
void func_801CE4B4(void*);
void func_801CE524(void*);
void func_801CE974(void*);
void func_801CE9E8(void*);
void func_801CEA30(void*);
void func_801CEAA0(void*);
void func_801CEAE8(void*);
void func_801CEB3C(void*);
void func_801CEBF0(void*);
void func_801CEC80(void*);
void func_801CDB94(void*, u32);
void func_801D0E88(void*, int, int);


// Externs for func_801CAA6C and func_801CAE9C

// Externs for state dispatch functions

u8 CItemBoxGrid::GetField61() { return reinterpret_cast<CItemBoxGridFull*>(this)->bytes.field_61; }

// Constructor for sub-item grid: blank every cell, set metadata and the
// vtable at +0x2CA8, then stamp the pre-cleared 0x400-cell template over the
// grid via func_801C562C (retail keeps both helper calls as `bl`).
#pragma optimize_for_size on
extern "C" __declspec(noinline) void* __ct__801C5514(void* self) {
    u8* end = (u8*)self + 0x2800;
    u8* cur = (u8*)self;
    do {
        SetEntry9Bytes(cur, -1, 0, 0, 0, 0, 0, 0, 0);
        cur += 10;
    } while (cur < end);
    *(u16*)((u8*)self + 0x2800) = 0;
    ((u8*)self)[0x2802] = 0;
    ((u8*)self)[0x2803] = 0;
    ((u8*)self)[0x2804] = 0;
    *(u32*)((u8*)self + 0x2CA8) = (u32)&lbl_eu_80534818;
    *(u16*)((u8*)self + 0x34AC) = 0;
    __ct__CVisionItem((u8*)self + 0x34B0);
    __ct__CArtsBookItem((u8*)self + 0x3CB8);
    lbl_eu_80664514 = (u32)self;
    {
        u16 i;
        for (i = 0; i < 0x400; i++) {
            u8 tmp[9];
            func_801C562C((u8*)self + i * 10, (char*)SetEntry9Bytes(tmp, -1, 0, 0, 0, 0, 0, 0, 0));
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
__declspec(noinline) void func_801C562C(void* dst, void* src) {
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
extern "C" void func_801C56D8(CItemBoxGridFull* self, u8 cat, int r5, u16 r6, u16 r7) {
    u8* p = (u8*)self;
    u32 i;
    p[0x2800] = 0;
    *(u16*)(p + 0x2800) = 0;
    // Init sub-objects
    void** vtblVision = *(void***)(p + 0x34b0);
    ((void(*)(void*))vtblVision[3])(p + 0x34b0);
    void** vtblArts = *(void***)(p + 0x2ca8);
    ((void(*)(void*))vtblArts[3])(p + 0x2ca8);
    void** vtblBook = *(void***)(p + 0x3cb8);
    ((void(*)(void*))vtblBook[3])(p + 0x3cb8);
    // Init 0x400 entries with default values
    u8 tmpBuf[10];
    for (i = 0; i < 0x400; i++) {
        SetEntry9Bytes(tmpBuf, (unsigned short)0xFFFF, 0, 0, 0, 0, 0, 0, 0);
        func_801C562C(p + i * 10, tmpBuf);
        p[i + 0x28a5] = 3;
    }
    u32 total = func_80157C20(cat);
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
        *(u16*)(p + count * 10) = id;
        *(u16*)(p + 0x2800) = count + 1;
        u8* entry = p + count * 10;
        if (cat == 3) {
            GridFilter24 filterCopy = filterBuf;
            u32 subCatIdx;
            for (subCatIdx = 0; subCatIdx < 6; subCatIdx++) {
                u8 subCat = (u8)filterCopy.w[subCatIdx];
                u32 sc = func_80157C20(subCat);
                u16 subId;
                for (subId = 0; (u32)subId < sc; subId++) {
                    void* subItem = func_80157C4C(subCat, subId);
                    if (!subItem) continue;
                    if (!*(u32*)subItem) continue;
                    void* inst = CItem_initItemImplInstances(subItem);
                    void** vtbl = *(void***)inst;
                    u16 numSlots = (u16)((u32(*)(void*, void*))vtbl[0xc])(inst, subItem);
                    u32 slot;
                    u8 matched = 0;
                    u16 keptSubId = 0;
                    for (slot = 0; slot < (u32)numSlots; slot++) {
                        void* inst2 = CItem_initItemImplInstances(subItem);
                        void** vtbl2 = *(void***)inst2;
                        s16 equipId = (s16)((s32(*)(void*, void*, u32))vtbl2[0x10])(inst2, subItem, slot);
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
                            u32 pn = func_801392B4(partyIdx);
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
                u32 shortKind = func_80139358(kindId);
                char nameBuf[64];
                const char* fmt = (cat == 2) ? &lbl_eu_8050566C[0x132] : &lbl_eu_8050566C[0x13d];
                sprintf(nameBuf, fmt, r5);
                if (!func_801361E8(ownerTbl, nameBuf, shortKind & 0xFFFF)) {
                    *(s16*)entry = -1;
                    count = *(u16*)(p + 0x2800);
                    *(u16*)(p + 0x2800) = count - 1;
                    goto check_next;
                }
            }
            u8 partyIdx;
            for (partyIdx = 0; partyIdx < (u8)code80135FDC_getByte_64077(); partyIdx++) {
                u32 pn = func_801392B4(partyIdx);
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
                u32 check = func_801361E8(lbl_eu_806640EC, &lbl_eu_8050566C[0x142], kind2);
                u32 result = check;
                u32 oneIfNonZero = (result != 0) ? 1 : 0;
                entry[3] = (u8)oneIfNonZero;
            }
            u32 kind3 = (*(u32*)item) >> 20;
            entry[5] = (u8)func_801C51BC(p + 0x34b0, kind3);
            entry[4] = (u8)func_801C51BC(p + 0x2ca8, kind3);
            if (cat == 0xd) {
                u32 shortKind = func_80139358(kind3);
                entry[6] = (u8)func_801C51BC(p + 0x3cb8, shortKind & 0xFFFF);
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
        u32 cnt = func_80157C20(cat);
        u32 base = func_801576C8(cat);
        u32 last = cnt - 1;
        u32 diff = cnt - base;
        s32 start = (s32)(last);
        u32 i2;
        for (i2 = 0; i2 <= diff; i2++) {
            p[start * 10 + 7] = 1;
            start--;
        }
        // Compute some threshold
        float fcnt2 = (float)(s32)cnt;
        float fmul = lbl_eu_80667F38 * fcnt2;
        int threshold = (int)fmul;
        for (i2 = 0; ; i2++) {
            int val = (i2 + 1) * 3;
            int diff2 = threshold - val;
            if (diff2 >= 0) {
                p[i2 + 0x28a5] = (u8)diff2;
            } else {
                p[i2 + 0x28a5] = (u8)(diff2 + 3);
                break;
            }
        }
    }
    func_801C6EC0((CItemBoxGridFull*)self, 0);
}

// Search for a matching short id in an array, return 1 if found.
// noinline: retail func_801C5158 keeps a `bl func_801C51BC` call; u32 id
// (retail compares the full register, no call-site mask).
__declspec(noinline) int func_801C51BC(void* obj, u32 id) {
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
__declspec(noinline) u32 func_801C5E5C(void* self) {
    u8* p = (u8*)self;
    u32 val = func_80157C20(p[0x2802]);
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
__declspec(noinline) void func_801C5EF4(CItemBoxGridFull* self) {
    u8 idx = self->field_2804 + 1;
    self->field_2804 = idx;
    if ((s8)idx >= self->field_2803) {
        self->field_2804 = 0;
    }
}

// Decrement a sub-index counter; borrow from the limit field when underflowing.
// noinline: retail callers emit `bl` (the unit builds with -inline).
__declspec(noinline) void func_801C5F20(CItemBoxGridFull* self) {
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
s16 func_801C5F48(CItemBoxGridFull* self, u16 idx) {
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
__declspec(noinline) u32 func_801C5FC0(CItemBoxGridFull* self, u16 idx) {
    s8 base = (s8)self->field_2804;
    u32 offset = (u16)(base * 0x1e + idx);
    if (offset >= (u32)self->field_2800) return 0;

    void* obj = func_80157C4C(self->field_2802, self->entries[offset].id);
    if (!obj || !*(u32*)obj) return 0;

    u32 kind = *(u32*)obj >> 20;
    func_801393CC(kind);
    u32 type = func_801392E4(kind);
    func_80139358(kind);
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
        u32 member = func_801392B4((u8)i);
        void* charData = (void*)func_8009EC9C((u32)(u8)member);
        void* ptr = (void*)((u8*)charData + 0x3534);
        if (func_8026178C(ptr, 0x93)) {
            basePrice += func_8025FB10(ptr, 0x93);
        }
    }

    return (u32)func_801C6158(0.01f * (double)(price * basePrice));
}

// Round a double to nearest integer with .5 tie-breaking biased away from zero.
__declspec(noinline) long func_801C6158(double f) {
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
    func_801392E4(cat & 0xFFFF);
    func_80139358(cat & 0xFFFF);
    u32 result = func_801C7C7C(self, cat & 0xFFFF, item);

    if (item) {
        u32 base = 0x64;
        u8 partyCount = code80135FDC_getByte_64077();
        u8 i;
        for (i = 0; i < partyCount; i++) {
            u8 member = (u8)func_801392B4(i);
            void* charData = (void*)func_8009EC9C(member);
            void* ptr = (void*)((u8*)charData + 0x3534);
            if (func_8026178C(ptr, 0x8f)) {
                base += func_8025FB10(ptr, 0x8f);
            }
        }
        result = (u32)func_801C6158((float)(result * base) * 0.01f);
    }

    return result;
}

// Lookup entry, check category; return word >> 20 or 0.
__declspec(noinline) u32 func_801C62AC(CItemBoxGridFull* self, u16 idx) {
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
__declspec(noinline) void* func_801C631C(CItemBoxGridFull* self, u16 idx) {
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

// Check item entry validity based on category.
__declspec(noinline) s32 func_801C6388(CItemBoxGridFull* self, u16 idx) {
    u8* p = (u8*)self;
    s8 base = (s8)p[0x2804];
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset >= *(u16*)(p + 0x2800)) return 0;
    s16 val = *(s16*)(p + offset * 0xa);
    void* obj = func_80157C4C(p[0x2802], val);
    if (!obj || !*(u32*)obj) return 0;
    u8 cat = p[0x2802];
    if ((u32)(cat - 4) <= 4) {
        // category 4-8
        void* inst = CItem_initItemImplInstances(obj);
        void** vtbl = *(void***)inst;
        u8 result = (u8)((u32(*)(void*, void*))vtbl[12])(inst, obj);
        if (!result) {
            u8 v2 = ((u8*)p + offset * 0xa)[2];
            return (s8)((v2 - 1) | 1);
        }
        u32 j;
        for (j = 0; j < result; j++) {
            void* inst2 = CItem_initItemImplInstances(obj);
            void** vtbl2 = *(void***)inst2;
            s16 r = (s16)((s32(*)(void*, void*, u32))vtbl2[16])(inst2, obj, j);
            if (r == -1) {
                void* inst3 = CItem_initItemImplInstances(obj);
                void** vtbl3 = *(void***)inst3;
                void* res3 = ((void*(*)(void*, void*, u32))vtbl3[11])(inst3, obj, j);
                if (res3 && (*(u16*)((u8*)res3 + 4) & 1)) return -3;
            }
        }
        return -2;
    } else if ((u32)(cat - 10) <= 3) {
        // category 10-13
        u8 v6 = ((u8*)obj)[6];
        return (s8)v6;
    } else if (cat == 2) {
        // same as cat 4-8
        void* inst = CItem_initItemImplInstances(obj);
        void** vtbl = *(void***)inst;
        u8 result = (u8)((u32(*)(void*, void*))vtbl[12])(inst, obj);
        if (!result) {
            u8 v2 = ((u8*)p + offset * 0xa)[2];
            return (s8)((v2 - 1) | 1);
        }
        u32 j;
        for (j = 0; j < result; j++) {
            void* inst2 = CItem_initItemImplInstances(obj);
            void** vtbl2 = *(void***)inst2;
            s16 r = (s16)((s32(*)(void*, void*, u32))vtbl2[16])(inst2, obj, j);
            if (r == -1) {
                void* inst3 = CItem_initItemImplInstances(obj);
                void** vtbl3 = *(void***)inst3;
                void* res3 = ((void*(*)(void*, void*, u32))vtbl3[11])(inst3, obj, j);
                if (res3 && (*(u16*)((u8*)res3 + 4) & 1)) return -3;
            }
        }
        return -2;
    } else if (cat == 3) {
        u8 v2 = ((u8*)p + offset * 0xa)[2];
        return (s8)((v2 - 1) | 1);
    } else if (cat == 9) {
        return 0;
    }
    return 0;
}

// Lookup entry, check category; return flag byte 2 (equip kind) or 0.
__declspec(noinline) u8 func_801C6528(CItemBoxGridFull* self, u16 idx) {
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
__declspec(noinline) u8 func_801C65A0(CItemBoxGridFull* self, u16 idx) {
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
__declspec(noinline) u8 func_801C6618(CItemBoxGridFull* self, u16 idx) {
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
u8 func_801C6690(CItemBoxGridFull* self, u16 idx) {
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
__declspec(noinline) u8 func_801C6708(CItemBoxGridFull* self, u16 idx) {
    s32 tmp = (s8)self->field_2804 * 0x1e + idx;
    u32 offset = (u32)(u16)tmp;
    if (offset < 0x400) {
        return ((u8*)self)[offset * 0xa + 7];
    }
    return 0;
}

__declspec(noinline) u8 func_801C673C(CItemBoxGridFull* self, u16 idx) {
    s32 tmp = (s8)self->field_2804 * 0x1e + idx;
    u32 offset = (u32)(u16)tmp;
    if (offset < 0x400) {
        return ((u8*)self)[offset * 0xa + 8];
    }
    return 0;
}


// Toggle an entry's flag based on category cap.
#pragma optimize_for_size on
void func_801C6770(CItemBoxGridFull* self, u16 idx) {
    u16 offset = (u16)((s8)self->field_2804 * 0x1e + idx);
    if (offset >= 0x400) return;
    u8 cap;
    u8* entry = (u8*)self + offset * 0xa;
    if (entry[8] == 0) {
        cap = (u8)func_801C6840(self);
        if (func_801C67F8(self) >= cap) return;
    }
    entry[8] = (u8)((entry[8] ^ 1) != 0);
}
#pragma optimize_for_size off

// Count entries with non-zero byte at offset 8 in a 10-byte stride array.
extern "C" __declspec(noinline) u8 func_801C67F8(CItemBoxGridFull* self) {
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
extern "C" __declspec(noinline) u8 func_801C6840(CItemBoxGridFull* self) {
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
void func_801C68A0(CItemBoxGridFull* self) {
    u16 i;
    for (i = 0; i < self->field_2800; i++) {
        CItemBoxGridEntry* entry = &self->entries[i];
        if (entry->flags[6] != 0) continue;
        void* obj = func_80157C4C(self->field_2802, entry->id);
        if (!obj || !*(u32*)obj) continue;
        void* inst = CItem_initItemImplInstances(obj);
        ((CItemInstVt10*)inst)->_v10(obj);
    }
}
#pragma optimize_for_size off

// Format the item name of the cell at page*0x1e+idx into the buffer at
// +0x2805 and return it (0 when the cell is empty/invalid). optimize_for_size
// merges the r29-r31 saves into retail's stmw prologue; the shared fail
// block reproduces retail's single return-0 fall-through into the epilogue.
#pragma optimize_for_size on
u32 func_801C6938(void* self, u32 idx) {
    u8* p = (u8*)self;
    s8 base = (s8)p[0x2804];
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset >= *(u16*)(p + 0x2800)) goto fail;

    u8 cat = p[0x2802];
    s16 val = *(s16*)(p + offset * 0xa);
    void* obj = func_80157C4C(cat, val);
    if (!obj || !*(u32*)obj) goto fail;

    void* inst = CItem_initItemImplInstances(obj);
    void* name = ((CItemInstVt20*)inst)->_v20(obj);

    sprintf((char*)(p + 0x2805), (const char*)&lbl_eu_8050566C[0x14c], name);

    if (p[0x2802] == 3) {
        void* inst2 = CItem_initItemImplInstances(obj);
        u32 count = ((CItemInstVt08*)inst2)->_v08(obj);
        u8 b = (u8)count;
        u32 r = (u32)func_80136190((void*)&lbl_eu_8050566C[0x14f], &lbl_eu_8050566C[0x158], (u32)(0x1e - (b - 1)));
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
    if (offset >= *(u16*)(p + 0x2800)) goto fail;

    void* item = func_80157C4C(p[0x2802], *(s16*)(p + offset * 0xa));
    if (!item || !*(u32*)item) goto fail;

    if (func_801C6E90(item)) {
        void* inst = CItem_initItemImplInstances(item);
        u32 v = ((CItemInstVt80*)inst)->_v80(item);
        u32 sel = 0x9d;
        if ((u16)v != 0) sel = 0x9e;
        char* msg = func_80136190((void*)&lbl_eu_8050566C[0x14f], &lbl_eu_8050566C[0x158], sel);
        sprintf((char*)(p + 0x2825), (const char*)&lbl_eu_8050566C[0x14c], msg);
    } else if (((*(u32*)item >> 12) & 0xF) == 9 && (u8)(((u8*)item)[7] & 3) == 3) {
        char* msg = func_80136190((void*)&lbl_eu_8050566C[0x14f], &lbl_eu_8050566C[0x158], 0x9c);
        sprintf((char*)(p + 0x2825), (const char*)&lbl_eu_8050566C[0x14c], msg);
    } else if (p[0x2802] == 3) {
        u32 tbl = lbl_eu_806640D8;
        void* inst1 = CItem_initItemImplInstances(item);
        u32 slotStatus = ((CItemInstVt54*)inst1)->_v54(item);
        void* inst2 = CItem_initItemImplInstances(item);
        u32 slotByte = (u8)((CItemInstVt08*)inst2)->_v08(item);
        void* inst3 = CItem_initItemImplInstances(item);
        char* name90 = (char*)((CItemInstVt90*)inst3)->_v90(item);
        char buf48[32];
        sprintf(buf48, (const char*)&lbl_eu_8050566C[0x162], name90);
        u32 nameKey = func_801361E8(tbl, (const char*)&lbl_eu_8050566C[0x165], (u16)slotStatus);
        char* desc = func_8013639C((const void*)tbl, &lbl_eu_8050566C[0x16e], (u16)slotStatus);
        sprintf((char*)(p + 0x2825), (const char*)&lbl_eu_8050566C[0x14c], desc);

        int langFlag = 1;
        if ((u8)getLanguage__9CDeviceSCFv() != 3) {
            if ((u8)getLanguage__9CDeviceSCFv() != 2) langFlag = 0;
        }

        char* cur = (char*)(p + 0x2825);
        for (;;) {
            char ch = *cur;
            if (ch == 0) break;
            if (ch == '$') {
                char buf28[32];
                memset(buf28, 0, 0x20);
                ch = *++cur;
                if (ch == '1') {
                    cur++;
                    switch ((u8)nameKey) {
                    case 1:
                        sprintf(buf28, (const char*)&lbl_eu_8050566C[0x17a], buf48);
                        break;
                    case 2:
                        sprintf(buf28, (const char*)&lbl_eu_8050566C[0x17a], buf48);
                        break;
                    case 3:
                        if (langFlag) sprintf(buf28, (const char*)&lbl_eu_8050566C[0x190], buf48);
                        else sprintf(buf28, (const char*)&lbl_eu_8050566C[0x1a9], buf48);
                        break;
                    case 4:
                        if (langFlag) sprintf(buf28, (const char*)&lbl_eu_8050566C[0x190], buf48);
                        else sprintf(buf28, (const char*)&lbl_eu_8050566C[0x1a9], buf48);
                        break;
                    }
                } else if (ch == '2') {
                    cur++;
                    // Retail keeps the 7-word language table copy in the '$2'
                    // arm, indexed by the item's slot byte.
                    u32 ti;
                    for (ti = 0; ti < 7; ti++) wordTbl[ti] = ((const u32*)lbl_eu_805055F0)[ti];
                    u32 key = wordTbl[slotByte & 0xFF];
                    u32 nKey = func_801361E8(tbl, (const char*)key, (u16)slotStatus);
                    if (langFlag) sprintf(buf28, (const char*)&lbl_eu_8050566C[0x1c1], (u8)nKey);
                    else sprintf(buf28, (const char*)&lbl_eu_8050566C[0x1da], (u8)nKey);
                }
                {
                    char buf68[32];
                    u32 len = 0;
                    char* q2 = buf28;
                    for (;;) {
                        char c = *q2;
                        len++;
                        q2++;
                        if (c == 0) break;
                    }
                    sprintf(buf68, (const char*)&lbl_eu_8050566C[0x14c], cur);
                    cur -= 2;
                    sprintf(cur, (const char*)&lbl_eu_8050566C[0x14c], buf28);
                    cur += len - 1;
                    sprintf(cur, (const char*)&lbl_eu_8050566C[0x14c], buf68);
                }
            } else if ((ch >= 0x81 && ch <= 0x9f) || (ch >= 0xe0 && ch <= 0xef)) {
                cur += 2;
            } else {
                cur += 1;
            }
        }
    } else {
        // lbl_eu_80664508/0C are single pointer variables (not arrays):
        // retail loads 0C first, then overrides with 08 when the category is
        // in range or exactly 2.
        u32 table = lbl_eu_8066450C;
        if ((u8)(p[0x2802] + 0xFC) <= 5 || p[0x2802] == 2) table = lbl_eu_80664508;
        u16 v = func_80136254((const void*)lbl_eu_806640EC, &lbl_eu_8050566C[0x1f2],
                              (*(u32*)item) >> 20);
        char* s = func_8013639C((const void*)table, &lbl_eu_8050566C[0x1f2], v);
        sprintf((char*)(p + 0x2825), (const char*)&lbl_eu_8050566C[0x14c], s);
    }
    return (char*)(p + 0x2825);
fail:
    return 0;
}
#pragma optimize_for_size off

// Check if an object has type 9 (extracted from vtable bits) and subtype 2.
// noinline: retail callers emit `bl` (the unit builds with -inline and would
// otherwise fold this body into every call site).
extern "C" __declspec(noinline) int func_801C6E90(void* obj) {
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
void func_801C81D0(CItemBoxGridFull* self);
void func_801C82D0(CItemBoxGridFull* self);
void func_801C83E4(CItemBoxGridFull* self);
void func_801C8534(CItemBoxGridFull* self);
void func_801C8634(CItemBoxGridFull* self);
void func_801C87CC(CItemBoxGridFull* self);
void func_801C88B0(CItemBoxGridFull* self);
void func_801C8994(CItemBoxGridFull* self);
void func_801C8ACC(CItemBoxGridFull* self, u32 target);
void func_801C8C58(CItemBoxGridFull* self, u32 target);
void func_801C8DE4(CItemBoxGridFull* self);
void func_801C8F04(CItemBoxGridFull* self);
void func_801C9040(CItemBoxGridFull* self);
void func_801C9158(CItemBoxGridFull* self);
void func_801C9270(CItemBoxGridFull* self);
void func_801C9390(CItemBoxGridFull* self);
void func_801C94E0(CItemBoxGridFull* self);
void func_801C9630(CItemBoxGridFull* self);
void func_801C9780(CItemBoxGridFull* self);
void func_801C98D0(CItemBoxGridFull* self, u32 target);
void func_801C9A3C(CItemBoxGridFull* self);
void func_801C9B8C(CItemBoxGridFull* self);
void func_801C9CCC(CItemBoxGridFull* self);
void func_801C9E1C(CItemBoxGridFull* self, u32 target);

// Re-sort the grid after its contents changed: dispatch on the current
// category (sorting first with func_801C7EF0 and masking the per-category
// func_8015780C result into an 8-bit value v), then apply the category's
// inner sort sequence. Categories 8-13 do nothing (default). optimize_for_size
// merges the r30/r31 saves into retail's stmw prologue. NOTE: MWCC computes
// the outer jump table over [0, max case] = [0,7] (empty/default-shared cases
// do NOT extend the range), while retail bounds-checks `cmplwi 0xd` with a
// 14-slot table pointing entries 8-13 at the epilogue -- the range-extension
// source form is not yet found (5 variants tried: shared/separate break,
// return, labels-before/after default, goto).
#pragma optimize_for_size on
void func_801C6EC0(CItemBoxGridFull* self, u16 idx) {
    func_801C7EF0(self, self->field_2802);
    u8 v = (u8)func_8015780C(self->field_2802);
    switch (self->field_2802) {
    case 0:
        func_801C81D0(self);
        switch (v) {
        case 1:
            func_801C9040(self);
            func_801C8F04(self);
            func_801C8DE4(self);
            func_801C9270(self);
            break;
        case 2:
            func_801C9040(self);
            func_801C8F04(self);
            func_801C8DE4(self);
            func_801C9390(self);
            func_801C9270(self);
            break;
        case 3:
            func_801C9040(self);
            func_801C8F04(self);
            func_801C8DE4(self);
            func_801C94E0(self);
            func_801C9270(self);
            break;
        case 4:
            func_801C8DE4(self);
            func_801C8F04(self);
            func_801C9158(self);
            func_801C9270(self);
            break;
        case 5:
            func_801C8DE4(self);
            func_801C8F04(self);
            func_801C9040(self);
            func_801C9270(self);
            break;
        case 6:
            func_801C82D0(self);
            break;
        }
        break;
    case 1:
        func_801C81D0(self);
        switch (v) {
        case 0:
            func_801C98D0(self, 1);
            func_801C98D0(self, 2);
            func_801C98D0(self, 3);
            func_801C9040(self);
            func_801C8F04(self);
            func_801C9630(self);
            break;
        case 1:
            func_801C98D0(self, 1);
            func_801C98D0(self, 2);
            func_801C98D0(self, 3);
            func_801C9040(self);
            func_801C8F04(self);
            func_801C9780(self);
            break;
        case 2:
            func_801C9040(self);
            func_801C8F04(self);
            func_801C9630(self);
            func_801C98D0(self, 3);
            func_801C98D0(self, 2);
            func_801C98D0(self, 1);
            break;
        case 3:
            func_801C9040(self);
            func_801C8F04(self);
            func_801C9630(self);
            func_801C98D0(self, 3);
            func_801C98D0(self, 1);
            func_801C98D0(self, 2);
            break;
        case 4:
            func_801C9040(self);
            func_801C8F04(self);
            func_801C9630(self);
            func_801C98D0(self, 2);
            func_801C98D0(self, 1);
            func_801C98D0(self, 3);
            break;
        case 5:
            func_801C98D0(self, 1);
            func_801C98D0(self, 2);
            func_801C98D0(self, 3);
            func_801C9630(self);
            func_801C9040(self);
            func_801C8F04(self);
            func_801C9A3C(self);
            break;
        case 6:
            func_801C98D0(self, 1);
            func_801C98D0(self, 2);
            func_801C98D0(self, 3);
            func_801C9630(self);
            func_801C8F04(self);
            func_801C9158(self);
            break;
        case 7:
            func_801C98D0(self, 1);
            func_801C98D0(self, 2);
            func_801C98D0(self, 3);
            func_801C9630(self);
            func_801C8F04(self);
            func_801C9040(self);
            break;
        case 8:
            func_801C82D0(self);
            break;
        case 9:
            func_801C82D0(self);
            break;
        }
        break;
    case 2:
        func_801C81D0(self);
        switch (v) {
        case 0:
            func_801C81D0(self);
            break;
        case 1:
            func_801C9B8C(self);
            func_801C7EF0(self, self->field_2802);
            func_801C8994(self);
            break;
        case 2:
            func_801C7EF0(self, self->field_2802);
            func_801C9B8C(self);
            func_801C8994(self);
            func_801C8C58(self, 2);
            func_801C8C58(self, 0);
            func_801C8C58(self, 1);
            break;
        case 3:
            func_801C7EF0(self, self->field_2802);
            func_801C9B8C(self);
            func_801C8994(self);
            func_801C8C58(self, 1);
            func_801C8C58(self, 0);
            func_801C8C58(self, 2);
            break;
        case 4:
            func_801C7EF0(self, self->field_2802);
            func_801C9B8C(self);
            func_801C8994(self);
            func_801C8C58(self, 2);
            func_801C8C58(self, 1);
            func_801C8C58(self, 0);
            break;
        case 5:
            func_801C8C58(self, 2);
            func_801C8C58(self, 1);
            func_801C8C58(self, 0);
            func_801C9B8C(self);
            func_801C8994(self);
            func_801C7EF0(self, self->field_2802);
            func_801C8ACC(self, 1);
            break;
        case 6:
            func_801C8C58(self, 2);
            func_801C8C58(self, 1);
            func_801C8C58(self, 0);
            func_801C9B8C(self);
            func_801C8994(self);
            func_801C7EF0(self, self->field_2802);
            func_801C8ACC(self, 2);
            break;
        case 7:
            func_801C8C58(self, 2);
            func_801C8C58(self, 1);
            func_801C8C58(self, 0);
            func_801C9B8C(self);
            func_801C8994(self);
            func_801C7EF0(self, self->field_2802);
            func_801C8ACC(self, 3);
            break;
        case 8:
            func_801C8C58(self, 2);
            func_801C8C58(self, 1);
            func_801C8C58(self, 0);
            func_801C9B8C(self);
            func_801C8994(self);
            func_801C7EF0(self, self->field_2802);
            func_801C8ACC(self, 4);
            break;
        case 9:
            func_801C8C58(self, 2);
            func_801C8C58(self, 1);
            func_801C8C58(self, 0);
            func_801C9B8C(self);
            func_801C8994(self);
            func_801C7EF0(self, self->field_2802);
            func_801C8ACC(self, 5);
            break;
        case 10:
            func_801C8C58(self, 2);
            func_801C8C58(self, 1);
            func_801C8C58(self, 0);
            func_801C9B8C(self);
            func_801C8994(self);
            func_801C7EF0(self, self->field_2802);
            func_801C8ACC(self, 6);
            break;
        case 11:
            func_801C82D0(self);
            break;
        }
        break;
    case 3:
        func_801C81D0(self);
        switch (v) {
        case 0:
            func_801C81D0(self);
            break;
        case 1:
            func_801C8994(self);
            break;
        case 2:
            func_801C8994(self);
            func_801C7EF0(self, self->field_2802);
            break;
        case 3:
            func_801C81D0(self);
            func_801C82D0(self);
            break;
        }
        break;
    case 4:
        func_801C81D0(self);
        switch (v) {
        case 1:
            func_801C83E4(self);
            break;
        case 2:
            func_801C8534(self);
            break;
        case 3:
            func_801C8634(self);
            break;
        case 4:
            func_801C82D0(self);
            break;
        }
        break;
    case 5:
        func_801C81D0(self);
        switch (v) {
        case 1:
            func_801C8534(self);
            break;
        case 2:
            func_801C8634(self);
            break;
        case 3:
            func_801C82D0(self);
            break;
        }
        break;
    case 6:
        if (v == 0) {
            func_801C81D0(self);
        }
        break;
    case 7:
        switch (v) {
        case 0:
            func_801C81D0(self);
            break;
        case 1:
            func_801C9E1C(self, 3);
            func_801C9E1C(self, 2);
            func_801C9E1C(self, 1);
            func_801C9CCC(self);
            func_801C87CC(self);
            break;
        case 2:
            func_801C9E1C(self, 3);
            func_801C9E1C(self, 2);
            func_801C9E1C(self, 1);
            func_801C9CCC(self);
            func_801C88B0(self);
            break;
        case 3:
            func_801C81D0(self);
            func_801C82D0(self);
            break;
        }
        break;
    default:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        break;
    }
}
#pragma optimize_for_size off

// Clear inactive entries and sort remaining.
void func_801C7730(CItemBoxGridFull* self) {
    u8* p = (u8*)self;
    u16 count = self->field_2800;
    u16 i;
    for (i = 0; i < count; i++) {
        u8* e = p + i * 10;
        if (e[8]) {
            SetEntry9Bytes((unsigned char*)e, (unsigned short)0xFFFF, (unsigned char)0, (unsigned char)0, (unsigned char)0, (unsigned char)0, (unsigned char)0, (unsigned char)0, (unsigned char)0);
            self->field_2800--;
        }
    }
    // Bubble sort remaining
    u16 pass;
    for (pass = 0; pass < 0x3FF; pass++) {
        u16 stay = 0x3FF - pass;
        u16 j;
        u32 swapped = 0;
        for (j = 0; j < stay; j++) {
            u8* e1 = p + j * 10;
            u8* e2 = p + (j + 1) * 10;
            s16 v1 = *(s16*)e1;
            s16 v2 = *(s16*)e2;
            u32 a = v1 + 1;
            u32 b = v2 + 1;
            if (a <= b) continue;
            char tmp[27];
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// 9-byte copy used by the sort routines' temp shuffle. noinline keeps the
// call sites' `bl` (retail calls it; -inline would fold the body in).
// Returns dst: retail sorters reuse the caller-preserved r3 (the CopyEntry9Bytes
// result) for the following func_801C562C src argument instead of recomputing.
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
//   lbl_eu_8050560C table row indexed by the +0x08 slot value.
// - type 9: per-slot price rows are accumulated over up to 4 slots (each
//   scaled by the table row), then the sum is scaled by 0x67F38 and the
//   branch applies an extra x10 on top of the function's common x10 return.
// - types 2..8: price row scaled by 0x67F78 + 0x67F7C * slot count.
// - anything else: plain price row lookup.
__declspec(noinline) u32 func_801C7958(void* self, void* item) {
    u32 word = *(u32*)item;
    u32 kind = word >> 20;
    u32 type = (word >> 16) & 0xF;
    u32 shortKind = func_80139358(kind);
    u32 result = 0;

    if (type == 3) {        u32 v54 = (u32)((CItemInstVt54*)CItem_initItemImplInstances(item))->_v54(item);
        u16 v08 = (u16)((CItemInstVt08*)CItem_initItemImplInstances(item))->_v08(item);
        u16 lookup = func_80136254((void*)lbl_eu_806640D8, &lbl_eu_8050566C[0x1fa], (u16)v54);
        u32 scale[7];
        u32 i;
        for (i = 0; i < 7; i++) scale[i] = ((const u32*)lbl_eu_8050560C)[i];
        result = (u32)((float)lookup * ((float*)scale)[v08 & 0x3F]);
    } else if (type == 9) {
        u16 v08 = (u16)((CItemInstVt08*)CItem_initItemImplInstances(item))->_v08(item);
        u32 scale[7];
        u32 i;
        for (i = 0; i < 7; i++) scale[i] = ((const u32*)lbl_eu_8050560C)[i];
        u32 slot;
        for (slot = 0; slot < 4; slot++) {
            u16 v = (u16)((CItemInstVt4C*)CItem_initItemImplInstances(item))->_v4C(item, (u8)slot);
            if (!v) continue;
            u16 lookup = func_80136254((void*)lbl_eu_806640D8, &lbl_eu_8050566C[0x1fa], v);
            u32 scale2[7];
            for (i = 0; i < 7; i++) scale2[i] = scale[i];
            result = (u32)(lbl_eu_80667F74 * ((float)lookup * ((float*)scale2)[v08 & 0x3F]) + (float)result);
        }
        result = (u32)((float)result * lbl_eu_80667F38);
        result *= 10;
    } else if (type >= 2 && type <= 8) {
        func_801393CC(kind);
        u16 lookup = func_80136254((void*)lbl_eu_806640D8, &lbl_eu_8050566C[0x1fa], (u16)shortKind);
        u8 count = (u8)((CItemInstVt30*)CItem_initItemImplInstances(item))->_v30(item);
        float f = lbl_eu_80667F78;
        while (count-- != 0) f += lbl_eu_80667F7C;
        result = (u32)((float)lookup * f);
    } else {
        func_801393CC(kind);
        result = func_80136254((void*)lbl_eu_806640D8, &lbl_eu_8050566C[0x1fa], (u16)shortKind);
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
    else type = (u16)func_801392E4(kind & 0xFFFF);
    shortKind = func_80139358(kind & 0xFFFF);
    if ((u32)(u16)type == 3) {
        u32 key;
        u32 slot;
        if (item) {
            key = (u32)((CItemInstVt54*)CItem_initItemImplInstances(obj))->_v54(obj);
            slot = (u16)((CItemInstVt08*)CItem_initItemImplInstances(obj))->_v08(obj);
        } else {
            key = shortKind;
            slot = (u8)func_801361E8((u32)lbl_eu_806640EC, &lbl_eu_8050566C[0x200], shortKind & 0xFFFF);
        }
        u16 lookup = func_80136254((void*)lbl_eu_806640D8, &lbl_eu_8050566C[0x1fa], key & 0xFFFF);
        u32 scale[7];
        u32 i;
        for (i = 0; i < 7; i++) scale[i] = ((const u32*)lbl_eu_8050560C)[i];
        result = (u32)((float)lookup * ((float*)scale)[slot & 0x3F]);
    } else if ((u32)(u16)type == 9) {
        // result stays 0
    } else if ((u32)(u16)type >= 2 && (u32)(u16)type <= 8) {
        u32 tbl = func_801393CC(kind & 0xFFFF);
        u16 lookup = func_80136254((void*)lbl_eu_806640D8, &lbl_eu_8050566C[0x1fa], shortKind & 0xFFFF);
        u32 count;
        if (item) count = (u16)((CItemInstVt30*)CItem_initItemImplInstances(obj))->_v30(obj);
        else count = (u8)func_801361E8(tbl, &lbl_eu_8050566C[0x209], shortKind & 0xFFFF);
        float f = lbl_eu_80667F78;
        while (count-- != 0) f += lbl_eu_80667F7C;
        result = (u32)((float)lookup * f);
    } else {
        func_801393CC(kind & 0xFFFF);
        result = func_80136254((void*)lbl_eu_806640D8, &lbl_eu_8050566C[0x1fa], shortKind & 0xFFFF);
    }
    return result * 10;
}
#pragma optimize_for_size off

// Sort entries with item instance comparison.
void func_801C7EF0(CItemBoxGridFull* self, u32 mode) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            s16 v1 = *(s16*)e1;
            s16 v2 = *(s16*)e2;
            void* obj1 = func_80157C4C(self->field_2802, v1);
            void* obj2 = func_80157C4C(self->field_2802, v2);
            if (!obj1 || !obj2) continue;
            // Compare based on mode
            u32 cmp = 0;
            if (mode == 3) {
                cmp = CItem_initItemImplInstances(self) > CItem_initItemImplInstances(self);
            } else {
                u16 w1 = *(u16*)((u8*)obj1 + 4);
                u16 w2 = *(u16*)((u8*)obj2 + 4);
                cmp = w1 > w2;
            }
            if (!cmp) continue;
            // Swap using CopyEntry9Bytes / func_801C562C
            char tmp[9];
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes((char*)e1, (const char*)e2);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries ascending by the item object's +4 value. The two
// item lookups (func_80157C4C) gate the swap; the 3-slot temp shuffle keeps
// the retail bl sequence (CopyEntry9Bytes x3 / func_801C562C x2).
void func_801C81D0(CItemBoxGridFull* self) {
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
            func_801C562C(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            func_801C562C(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries by item price (func_801C7958). The value calls
// evaluate item j+1 before item j (locals force the call order), matching
// retail's sequence; the 3-slot temp shuffle is the same shape as
// func_801C81D0. Swap when the first item's price is strictly lower.
void func_801C82D0(CItemBoxGridFull* self) {
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
            func_801C562C(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            func_801C562C(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries by kind-name sort key: each item's kind
// (word>>20) is mapped through func_80139358 to a short id, which is
// looked up in the message table (lbl_eu_80664104) with the "name" key
// string; swap when the byte keys differ (descending). Same 3-slot temp
// shuffle shape as func_801C81D0.
void func_801C83E4(CItemBoxGridFull* self) {
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
            u32 k1 = func_80139358(w1 >> 20);
            u32 w2 = *(u32*)obj2;
            u32 k2 = func_80139358(w2 >> 20);
            u32 c1 = func_801361E8(lbl_eu_80664104, (const char*)&lbl_eu_8050566C[0x212], k1 & 0xFFFF);
            u32 c2 = func_801361E8(lbl_eu_80664104, (const char*)&lbl_eu_8050566C[0x212], k2 & 0xFFFF);
            if ((c1 & 0xFF) <= (c2 & 0xFF)) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            func_801C562C(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            func_801C562C(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries descending by the item object's +6 byte (swap when
// the first is strictly smaller). Same temp-shuffle shape as func_801C81D0.
void func_801C8534(CItemBoxGridFull* self) {
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
            func_801C562C(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            func_801C562C(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Two-pass bubble sort: pass 1 orders entries ascending by flag byte 5
// (flags[3]), pass 2 by flag byte 4 (flags[2]). Each pass swaps adjacent
// pairs while the earlier entry's byte is strictly smaller, stopping early
// when a pass makes no swap. The 3-slot temp shuffle (12-byte stride) keeps
// the retail CopyEntry9Bytes / func_801C562C call sequence.
void func_801C8634(CItemBoxGridFull* self) {
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
                func_801C562C(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
                func_801C562C(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
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
                func_801C562C(e1, CopyEntry9Bytes(tmp2 + 12, (const char*)e2));
                func_801C562C(e2, CopyEntry9Bytes(tmp2 + 24, (const char*)tmp2));
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
void func_801C87CC(CItemBoxGridFull* self) {
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
                func_801C562C(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
                func_801C562C(e2, CopyEntry9Bytes(tmp + 24, tmp));
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}
#pragma pop

// Bubble sort entries ascending by byte at offset 6 (mirror of
// func_801C87CC with the comparison inverted).
#pragma push
#pragma optimize_for_size on
void func_801C88B0(CItemBoxGridFull* self) {
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
                func_801C562C(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
                func_801C562C(e2, CopyEntry9Bytes(tmp + 24, tmp));
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}
#pragma pop

// Bubble-sort grid entries ascending by the item instance's vtable+0x08
// slot value (u16). v2 (item j+1) is evaluated before v1 to match retail's
// call order; the 3-slot temp shuffle is the same shape as func_801C81D0.
void func_801C8994(CItemBoxGridFull* self) {
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
            v2 = (u16)((CItemInstVt08*)CItem_initItemImplInstances(obj2))->_v08(obj2);
            u16 v1;
            v1 = (u16)((CItemInstVt08*)CItem_initItemImplInstances(obj1))->_v08(obj1);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            func_801C562C(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            func_801C562C(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
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
void func_801C8ACC(CItemBoxGridFull* self, u32 target) {
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
            u32 v1 = ((CItemInstVt54*)CItem_initItemImplInstances(obj1))->_v54(obj1);
            u32 v2 = ((CItemInstVt54*)CItem_initItemImplInstances(obj2))->_v54(obj2);
            u32 c1 = func_801361E8(lbl_eu_806640D8, (const char*)&lbl_eu_8050566C[0x217], (u16)v1);
            u32 c2 = func_801361E8(lbl_eu_806640D8, (const char*)&lbl_eu_8050566C[0x217], (u16)v2);
            u32 m1 = (u8)c1 == target;
            u32 m2 = (u8)c2 == target;
            if (m1 < m2) {
                CopyEntry9Bytes(tmp, (const char*)e1);
                func_801C562C(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
                func_801C562C(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries so items whose slot-status name lookup matches
// `target` group at the front (same shape as func_801C8ACC, key string at
// +0x220).
void func_801C8C58(CItemBoxGridFull* self, u32 target) {
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
            u32 v1 = ((CItemInstVt54*)CItem_initItemImplInstances(obj1))->_v54(obj1);
            u32 v2 = ((CItemInstVt54*)CItem_initItemImplInstances(obj2))->_v54(obj2);
            u32 c1 = func_801361E8(lbl_eu_806640D8, (const char*)&lbl_eu_8050566C[0x220], (u16)v1);
            u32 c2 = func_801361E8(lbl_eu_806640D8, (const char*)&lbl_eu_8050566C[0x220], (u16)v2);
            u32 m1 = (u8)c1 == target;
            u32 m2 = (u8)c2 == target;
            if (m1 < m2) {
                CopyEntry9Bytes(tmp, (const char*)e1);
                func_801C562C(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
                func_801C562C(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
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
            func_801C562C(e1, tmp + 12);
            CopyEntry9Bytes(tmp + 24, (const char*)tmp);
            func_801C562C(e2, tmp + 24);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries ascending by the item instance's vtable+0x30
// slot value (u16). v2 is evaluated first, matching retail's call order;
// same 3-slot temp shuffle shape as func_801C81D0.
void func_801C8F04(CItemBoxGridFull* self) {
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
            u32 v2 = ((CItemInstVt30*)CItem_initItemImplInstances(obj2))->_v30(obj2);
            u32 v1 = ((CItemInstVt30*)CItem_initItemImplInstances(obj1))->_v30(obj1);
            if ((u16)v1 >= (u16)v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            func_801C562C(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            func_801C562C(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries ascending by func_801CA070 slot status (swap when
// item j's value is strictly lower). Same 3-slot temp shuffle shape as
// func_801C81D0.
void func_801C9040(CItemBoxGridFull* self) {
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
            u32 v1 = func_801CA070(self, obj1);
            u32 v2 = func_801CA070(self, obj2);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            func_801C562C(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            func_801C562C(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries descending by func_801CA070 slot status (swap when
// item j's value is strictly higher). Same 3-slot temp shuffle shape as
// func_801C81D0.
void func_801C9158(CItemBoxGridFull* self) {
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
            u32 v1 = func_801CA070(self, obj1);
            u32 v2 = func_801CA070(self, obj2);
            if (v1 <= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            func_801C562C(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            func_801C562C(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort grid entries by func_801CA110 slot status, ascending (swap
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
            u8 v1 = (u8)func_801CA110(self, obj1);
            u8 v2 = (u8)func_801CA110(self, obj2);
            if (v1 <= v2) continue;
            char tmp[36];
            CopyEntry9Bytes(tmp, (const char*)e1);
            func_801C562C(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            func_801C562C(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries ascending by the byte value of each item's kind
// name lookup: the item's kind (word>>20) is mapped through func_80139358 to
// a short id, which is looked up in the owner table (lbl_eu_806640F4) with
// the sort-key string; swap when item j's key byte is strictly lower. Same
// 3-slot temp shuffle shape as func_801C81D0.
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
            u32 t1 = lbl_eu_806640F4;
            u32 k1 = func_80139358(w1 >> 20);
            u32 c1 = func_801361E8(t1, (const char*)&lbl_eu_8050566C[0x227], k1 & 0xFFFF);
            u32 w2 = *(u32*)obj2;
            u32 t2 = lbl_eu_806640F4;
            u32 k2 = func_80139358(w2 >> 20);
            u32 c2 = func_801361E8(t2, (const char*)&lbl_eu_8050566C[0x227], k2 & 0xFFFF);
            if ((c1 & 0xFF) >= (c2 & 0xFF)) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            func_801C562C(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            func_801C562C(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
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
            u32 w1 = *(u32*)obj1;
            u32 t1 = lbl_eu_806640F4;
            u32 k1 = func_80139358(w1 >> 20);
            u32 c1 = func_801361E8(t1, (const char*)&lbl_eu_8050566C[0x22f], k1 & 0xFFFF);
            u32 w2 = *(u32*)obj2;
            u32 t2 = lbl_eu_806640F4;
            u32 k2 = func_80139358(w2 >> 20);
            u32 c2 = func_801361E8(t2, (const char*)&lbl_eu_8050566C[0x22f], k2 & 0xFFFF);
            if ((c1 & 0xFF) >= (c2 & 0xFF)) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            func_801C562C(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            func_801C562C(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries ascending by the byte value of each item's kind
// name lookup in the capacity table (lbl_eu_806640F8). Both kinds are
// computed first, then both lookups - the func_801C83E4 ordering. Same
// 3-slot temp shuffle shape as func_801C81D0.
void func_801C9630(CItemBoxGridFull* self) {
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
            u32 k1 = func_80139358(w1 >> 20);
            u32 w2 = *(u32*)obj2;
            u32 k2 = func_80139358(w2 >> 20);
            u32 c1 = func_801361E8(lbl_eu_806640F8, (const char*)&lbl_eu_8050566C[0x238], k1 & 0xFFFF);
            u32 c2 = func_801361E8(lbl_eu_806640F8, (const char*)&lbl_eu_8050566C[0x238], k2 & 0xFFFF);
            if ((c1 & 0xFF) >= (c2 & 0xFF)) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            func_801C562C(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            func_801C562C(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries ascending by the byte value of each item's kind
// name lookup in the capacity table (lbl_eu_806640F8), key string offset
// 0x240. Same ordering and temp shuffle shape as func_801C9630.
void func_801C9780(CItemBoxGridFull* self) {
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
            u32 k1 = func_80139358(w1 >> 20);
            u32 w2 = *(u32*)obj2;
            u32 k2 = func_80139358(w2 >> 20);
            u32 c1 = func_801361E8(lbl_eu_806640F8, (const char*)&lbl_eu_8050566C[0x240], k1 & 0xFFFF);
            u32 c2 = func_801361E8(lbl_eu_806640F8, (const char*)&lbl_eu_8050566C[0x240], k2 & 0xFFFF);
            if ((c1 & 0xFF) >= (c2 & 0xFF)) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            func_801C562C(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            func_801C562C(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by float comparison. The `target` argument is not
// used by the sort itself (retail passes 1/2/3 from func_801C6EC0).
void func_801C98D0(CItemBoxGridFull* self, u32 target) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            float v1 = func_801C9F88(self, obj1);
            float v2 = func_801C9F88(self, obj2);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries DESCENDING by the byte value of each item's kind
// name lookup in the capacity table (lbl_eu_806640F8), key string offset
// 0x251 (swap when item j's key byte is strictly higher). Same ordering
// and temp shuffle shape as func_801C9630.
void func_801C9A3C(CItemBoxGridFull* self) {
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
            u32 k1 = func_80139358(w1 >> 20);
            u32 w2 = *(u32*)obj2;
            u32 k2 = func_80139358(w2 >> 20);
            u32 c1 = func_801361E8(lbl_eu_806640F8, (const char*)&lbl_eu_8050566C[0x251], k1 & 0xFFFF);
            u32 c2 = func_801361E8(lbl_eu_806640F8, (const char*)&lbl_eu_8050566C[0x251], k2 & 0xFFFF);
            if ((c1 & 0xFF) <= (c2 & 0xFF)) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            func_801C562C(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            func_801C562C(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries ascending by the item instance's vtable+0x90
// slot value (byte). v1 is evaluated first, matching retail's call order;
// same 3-slot temp shuffle shape as func_801C81D0.
void func_801C9B8C(CItemBoxGridFull* self) {
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
            u32 v1 = ((CItemInstVt90*)CItem_initItemImplInstances(obj1))->_v90(obj1);
            u32 v2 = ((CItemInstVt90*)CItem_initItemImplInstances(obj2))->_v90(obj2);
            if ((u8)v1 >= (u8)v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            func_801C562C(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
            func_801C562C(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by float comparison.
void func_801C9CCC(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            float v1 = func_801C9F88(self, obj1);
            float v2 = func_801C9F88(self, obj2);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort grid entries so items whose kind-name lookup matches `target`
// group at the front: an adjacent pair (j, j+1) is swapped when entry j+1's
// key byte equals target but entry j's does not. Key = message-table
// (lbl_eu_80664110) lookup of the kind's short id ("name" key at +0x109).
void func_801C9E1C(CItemBoxGridFull* self, u32 target) {
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
            u32 c1 = func_801361E8(tbl, (const char*)&lbl_eu_8050566C[0x109], (u16)func_80139358(w1 >> 20));
            u32 w2 = *(u32*)obj2;
            u32 tbl2 = lbl_eu_80664110;
            u32 c2 = func_801361E8(tbl2, (const char*)&lbl_eu_8050566C[0x109], (u16)func_80139358(w2 >> 20));
            u32 m1 = (u8)c1 == target;
            u32 m2 = (u8)c2 == target;
            if (m1 < m2) {
                CopyEntry9Bytes(tmp, (const char*)e1);
                func_801C562C(e1, CopyEntry9Bytes(tmp + 12, (const char*)e2));
                func_801C562C(e2, CopyEntry9Bytes(tmp + 24, (const char*)tmp));
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

// Item sort value for an entry: look up the kind's index in the owner table
// (lbl_eu_806640F4); when the kind's flag entry has bit 2 set, scale the
// index by party member 1's func_800A082C value, then scale by the 0x67F80
// factor, clamp to 999 and return as a float.
// optimize_for_size: retail uses the bl _savegpr_28/_restgpr_28 helper-call
// prologue, which only size-opt emits on this unit.
#pragma push
#pragma optimize_for_size on
float func_801C9F88(void* self, void* entry) {
    u32 word = *(u32*)entry;
    u32 tbl = lbl_eu_806640F4;
    u32 kind = func_80139358(word >> 20);
    u32 idx = func_80136254((void*)tbl, (const char*)&lbl_eu_8050566C[0x25a], (u16)kind);
    // Bit 2 of the kind flag entry gates a party-bonus multiplier.
    if ((func_801361E8(tbl, (const char*)&lbl_eu_8050566C[0x261], (u16)kind) & 4)) {
        void* charData = func_8009EC9C(1);
        // u32->double via the shared 2^52 magic so the lfd reloc names
        // lbl_eu_80667F48 instead of a TU-local pool constant.
        union {
            double d;
            u32 w[2];
        } conv;
        u32 prod = (idx & 0xFFFF) * (u32)(u16)func_800A082C(charData);
        conv.w[0] = 0x43300000;
        conv.w[1] = prod ^ 0x80000000;
        float scaled = (float)(conv.d - lbl_eu_80667F48);
        idx = (u32)(u16)(s32)(scaled * lbl_eu_80667F80);
    }
    if ((u32)(u16)idx > 999) idx = 999;
    // Truncated result converted back through the 2^52 magic (lbl_eu_80667F40).
    return (float)((double)(u16)idx - lbl_eu_80667F40);
}
#pragma pop

#pragma optimize_for_size on
__declspec(noinline) u32 func_801CA070(void* self, void* item) {
    void* inst = CItem_initItemImplInstances(item);
    u16 count = (u16)((CItemInstVt30*)inst)->_v30(item);
    u8 i;
    for (i = 0; i < count; i++) {
        void* inst2 = CItem_initItemImplInstances(item);
        void* obj = ((CItemInstVt2C*)inst2)->_v2C(item, i);
        if (obj && (*(u16*)((u8*)obj + 4) & 1)) return 1;
    }
    return 0;
}
#pragma optimize_for_size off

// Check if entry kind exists in pool.
#pragma optimize_for_size on
__declspec(noinline) u32 func_801CA110(void* self, void* entry) {
    u32 val = *(u32*)entry;
    u32 obj = lbl_eu_806640F4;
    u32 kind = func_80139358(val >> 20);
    u32 i;
    for (i = 1; i <= 10; i++) {
        char buf[40];
        sprintf(buf, (const char*)&lbl_eu_8050566C[0x132], (u8)i);
        if ((u8)func_801361E8(obj, buf, (u16)kind)) return i;
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

    // sort-menu temp: copy its config into the +0xec block
    u8 tmpSort[0xf0];
    __ct__CSortMenu(tmpSort);
    __ct__UnkClass_8011C974(p + 0xec, tmpSort + 4);
    *(u32*)(p + 0xfc) = *(u32*)(tmpSort + 0x14);
    *(u32*)(p + 0x100) = *(u32*)(tmpSort + 0x18);
    *(u32*)(p + 0x104) = *(u32*)(tmpSort + 0x1c);
    *(u32*)(p + 0x108) = *(u32*)(tmpSort + 0x20);
    *(u32*)(p + 0x10c) = *(u32*)(tmpSort + 0x24);
    p[0x110] = tmpSort[0x28];
    p[0x111] = tmpSort[0x29];
    p[0x112] = tmpSort[0x2a];
    p[0x113] = tmpSort[0x2b];
    __ct__UnkClass_8011C974(p + 0x118, tmpSort + 0x30);
    *(u32*)(p + 0x128) = *(u32*)(tmpSort + 0x38);
    *(u32*)(p + 0x12c) = *(u32*)(tmpSort + 0x3c);
    *(u32*)(p + 0x130) = *(u32*)(tmpSort + 0x40);
    *(u32*)(p + 0x134) = *(u32*)(tmpSort + 0x44);
    p[0x138] = tmpSort[0x48];
    p[0x139] = tmpSort[0x49];
    p[0x13a] = tmpSort[0x4a];
    p[0x13b] = tmpSort[0x4b];
    *(float*)(p + 0x13c) = *(float*)(tmpSort + 0x4c);
    *(float*)(p + 0x140) = *(float*)(tmpSort + 0x50);
    *(float*)(p + 0x144) = *(float*)(tmpSort + 0x54);
    *(float*)(p + 0x148) = *(float*)(tmpSort + 0x58);
    *(float*)(p + 0x14c) = *(float*)(tmpSort + 0x5c);
    p[0x150] = tmpSort[0x60];
    {
        u32 i;
        for (i = 0; i < 0x10; i++) {
            ((u32*)(p + 0x150))[i] = ((u32*)(tmpSort + 0x60))[i];
        }
    }
    p[0x1d4] = tmpSort[0xec];
    p[0x1d5] = tmpSort[0xed];
    p[0x1d6] = tmpSort[0xee];
    __dt__9CSortMenuFv(tmpSort, -1);

    // item-info temp (mode-derived init)
    int modeId = 0;
    if (mode == 1) modeId = 1;
    else if (mode == 2) modeId = 3;
    u8 tmpInfo[0x2800];
    __ct__CItemBoxInfo(tmpInfo, modeId, *(u16*)(p + 0x52a));
    __ct__UnkClass_8011C974(p + 0x1dc, tmpInfo + 4);
    __ct__UnkClass_8011C974(p + 0x1ec, tmpInfo + 0x14);
    {
        u32 i;
        for (i = 0; i < 0x27; i++) {
            ((u32*)(p + 0x1fc))[i] = ((u32*)(tmpInfo + 0x24))[i];
        }
    }
    p[0x268] = tmpInfo[0x90];
    *(u32*)(p + 0x26c) = *(u32*)(tmpInfo + 0x94);
    p[0x270] = tmpInfo[0x98];
    p[0x271] = tmpInfo[0x99];
    p[0x272] = tmpInfo[0x9a];
    *(u32*)(p + 0x274) = *(u32*)(tmpInfo + 0x9c);
    *(u32*)(p + 0x278) = *(u32*)(tmpInfo + 0xa0);
    *(u32*)(p + 0x27c) = *(u32*)(tmpInfo + 0xa4);
    *(u32*)(p + 0x280) = *(u32*)(tmpInfo + 0xa8);
    *(u16*)(p + 0x284) = *(u16*)(tmpInfo + 0xac);
    p[0x286] = tmpInfo[0xae];
    {
        u32 i;
        for (i = 0; i < 3; i++) {
            ((u32*)(p + 0x284))[i] = ((u32*)(tmpInfo + 0xac))[i];
        }
    }
    {
        u32 i;
        for (i = 0; i < 0x12; i++) {
            ((u32*)(p + 0x29c))[i] = ((u32*)(tmpInfo + 0xc4))[i];
        }
    }
    *(u32*)(p + 0x330) = *(u32*)(tmpInfo + 0x158);
    *(u32*)(p + 0x334) = *(u32*)(tmpInfo + 0x15c);
    *(u32*)(p + 0x338) = *(u32*)(tmpInfo + 0x160);
    *(u32*)(p + 0x33c) = *(u32*)(tmpInfo + 0x164);
    *(u32*)(p + 0x340) = *(u32*)(tmpInfo + 0x168);
    *(u32*)(p + 0x344) = *(u32*)(tmpInfo + 0x16c);
    {
        u32 i;
        for (i = 0; i < 3; i++) {
            ((u32*)(p + 0x344))[i] = ((u32*)(tmpInfo + 0x16c))[i];
        }
    }
    p[0x360] = tmpInfo[0x188];
    {
        u32 i;
        for (i = 0; i < 0x10; i++) {
            ((u32*)(p + 0x35d))[i] = ((u32*)(tmpInfo + 0x185))[i];
        }
    }
    __dt__12CItemBoxInfoFv(tmpInfo, -1);

    // num-select temp
    u8 tmpNum[0x38];
    __ct__CNumSelect(tmpNum, 1);
    __ct__UnkClass_8011C974(p + 0x3e8, tmpNum + 4);
    *(u32*)(p + 0x3f8) = *(u32*)(tmpNum + 0x14);
    *(u32*)(p + 0x3fc) = *(u32*)(tmpNum + 0x18);
    *(u32*)(p + 0x400) = *(u32*)(tmpNum + 0x1c);
    *(u32*)(p + 0x404) = *(u32*)(tmpNum + 0x20);
    *(u32*)(p + 0x408) = *(u32*)(tmpNum + 0x24);
    *(u32*)(p + 0x40c) = *(u32*)(tmpNum + 0x28);
    p[0x410] = tmpNum[0x2c];
    p[0x411] = tmpNum[0x2d];
    p[0x412] = tmpNum[0x2e];
    p[0x413] = tmpNum[0x2f];
    p[0x414] = tmpNum[0x30];
    __dt__10CNumSelectFv(tmpNum, -1);

    // exchange-win temp (mode 2 only)
    if (mode == 2) {
        u8 tmpEx[0x28];
        __ct__CExchangeWin(tmpEx);
        __ct__UnkClass_8011C974(p + 0x444, tmpEx + 4);
        *(u32*)(p + 0x454) = *(u32*)(tmpEx + 0x14);
        *(u32*)(p + 0x458) = *(u32*)(tmpEx + 0x18);
        *(u32*)(p + 0x45c) = *(u32*)(tmpEx + 0x1c);
        *(u32*)(p + 0x460) = *(u32*)(tmpEx + 0x20);
        p[0x464] = tmpEx[0x24];
        p[0x465] = tmpEx[0x25];
        p[0x466] = tmpEx[0x26];
        p[0x467] = tmpEx[0x27];
        __dt__12CExchangeWinFv(tmpEx, -1);
    }

    // sys-win temps copied via func_8016742C
    u8 tmpWin1[0x3c];
    __ct__CSysWin(tmpWin1, 2);
    func_8016742C(p + 0x4ac, tmpWin1);
    __dt__7CSysWinFv(tmpWin1, -1);
    u8 tmpWin2[0x3c];
    __ct__CSysWin(tmpWin2, 0);
    func_8016742C(p + 0x4e8, tmpWin2);
    __dt__7CSysWinFv(tmpWin2, -1);

    lbl_eu_80664508 = 0;
    lbl_eu_8066450C = 0;
    lbl_eu_80664510 = 0;
    func_8013B2D4();

    // sub-grid temp: init and bulk-copy into the +0x548 region
    u8 tmpGrid[0x800];
    __ct__801C5514(tmpGrid);
    {
        u32 i;
        for (i = 0; i < 0x500; i++) {
            ((u32*)(p + 0x548))[i] = ((u32*)(tmpGrid - 4))[i];
        }
    }
    *(u16*)(p + 0x2d4c) = *(u16*)(tmpGrid + 0x800);
    p[0x2d4e] = tmpGrid[0x802];
    p[0x2d4f] = tmpGrid[0x803];
    p[0x2d50] = tmpGrid[0x804];
    {
        u32 i;
        for (i = 0; i < 4; i++) {
            ((u32*)(p + 0x2d4d))[i] = ((u32*)(tmpGrid + 0x801))[i];
        }
    }
    {
        u32 i;
        for (i = 0; i < 0x10; i++) {
            ((u32*)(p + 0x2d6d))[i] = ((u32*)(tmpGrid + 0x821))[i];
        }
    }
    {
        u32 i;
        for (i = 0; i < 0x80; i++) {
            ((u32*)(p + 0x2ded))[i] = ((u32*)(tmpGrid + 0x8a1))[i];
        }
    }
    func_8018B130(p + 0x31f4, tmpGrid + 0xca8);
    func_8018B130(p + 0x39fc, tmpGrid + 0x12b0);
    func_8018B130(p + 0x4204, tmpGrid + 0x1ab8);
    __dt__801C5670(tmpGrid, -1);

    func_80157824(2, 0);
    func_80157824(4, 0);
    func_80157824(5, 0);
    func_80157824(6, 0);
    func_80157824(7, 0);
    func_80157824(8, 0);
    func_80157824(3, 0);
    func_80157824(9, 0);
    func_80157824(0xa, 0);
    func_80157824(0xb, 0);
    func_80157824(0xc, 0);
    func_80157824(0xd, 0);
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

void func_801CAA6C(void* self, int r4) {
    u8* p = (u8*)self;
    void* allocHandle = getHandleMEM2__Q23mtl10MemManagerFv();

    if (func_80212480()) {
        void* mem1 = getHandleMEM1__Q23mtl10MemManagerFv();
        u32 maxSize = getMaxAllocSize__Q23mtl10MemManagerFUl((u32)mem1);
        if (maxSize >= 0x38000) {
            allocHandle = mem1;
        }
    }

    p[0x28] = (u32)readFile__11CDeviceFileFUlPCcP10IWorkEventii((u32)allocHandle, &lbl_eu_8050566C[0x266], self, 0, 0);

    allocHandle = getHandleMEM2__Q23mtl10MemManagerFv();
    p[0x2c] = (u32)readFile__11CDeviceFileFUlPCcP10IWorkEventii((u32)allocHandle, &lbl_eu_8050566C[0x27e], self, 0, 0);

    if (p[0x543]) {
        allocHandle = (void*)func_800A9D90();
        p[0x30] = (u32)readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii((u32)allocHandle, &lbl_eu_8050566C[0x297], self, 0, 0);

        allocHandle = getHandleMEM2__Q23mtl10MemManagerFv();
        p[0x34] = (u32)readFile__11CDeviceFileFUlPCcP10IWorkEventii((u32)allocHandle, &lbl_eu_8050566C[0x2b4], self, 0, 0);
    } else {
        lbl_eu_80664508 = (u32)getFP__FPCc(&lbl_eu_8050566C[0x2d0]);
        lbl_eu_8066450C = (u32)getFP__FPCc(&lbl_eu_8050566C[0x2df]);
        lbl_eu_80664510 = (u32)getFP__FPCc(&lbl_eu_8050566C[0x2ee]);
    }

    func_801D3064(p + 0xe8);
    func_801D4054(p + 0x1d8);
    func_801EAE8C(p + 0x3e4);

    if (p[0x527] == 2) {
        func_8022CF2C(p + 0x440);
    }

    ((CItemInstVt90*)(p + 0x4ac))->_v88();
    ((CItemInstVt90*)(p + 0x4e8))->_v88();
}

void func_801CABC8(void* self, int r4) {
    u8* p = (u8*)self;
    if (!p[0x54]) return;
    u32 state = *(u32*)(p + 0x58);
    if (!state) return;

    if (state <= 0x1a) {
        switch (state) {
        case 1:
            func_801CE108(self);
            break;
        case 2:
            func_801CE1A0(self);
            break;
        case 3:
            CheckState4_Animate((char*)self);
            break;
        case 4:
            func_801CE2F8(self);
            break;
        case 5:
            func_801CE390(self);
            break;
        case 6:
            if (func_801EB028(p + 0x3e4)) {
                *(u32*)(p + 0x58) = 0x19;
            }
            break;
        case 8:
            func_801CE3E8(self);
            break;
        case 9:
            func_801CE4B4(self);
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
            func_801CE974(self);
            break;
        case 17:
            func_801CE9E8(self);
            break;
        case 18:
            func_801CEA30(self);
            break;
        case 20:
            func_801CEAA0(self);
            break;
        case 21:
            func_801CEAE8(self);
            break;
        case 23:
            func_801CEB3C(self);
            break;
        case 24:
            if (CSysWin_isActive(p + 0x4e8)) {
                *(u32*)(p + 0x58) = 0x13;
            }
            break;
        case 26:
            func_801CEBF0(self);
            break;
        }
    }

    {
        void* layout = *(void**)(p + 0x44);
        void** lvtbl = *(void***)layout;
        ((void(*)(void*, int))lvtbl[0xE])(layout, 0);
    }

    func_801D202C(p + 0x70);
    func_801D202C(p + 0x88);
    func_801D202C(p + 0xa0);
    func_801D202C(p + 0xb8);
    func_801D202C(p + 0xd0);
    func_801D3160(p + 0xe8);
    func_801D40C4(p + 0x1d8);
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
void func_801CAD8C(void* self, int r4) {
    u8* p = (u8*)self;
    if (!p[0x54]) return;
    if (!*(u32*)(p + 0x58)) return;

    func_801D4154(p + 0x1d8);
    func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii((nw4r::lyt::Layout*)*(void**)(p + 0x44), (nw4r::lyt::DrawInfo*)r4, 0, 1);

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
void func_801CAE9C(void* self, int r4) {
    u8* p = (u8*)self;

    if (p[0x543]) {
        func_8003AA8C__5CBdatFUl(2);
        func_8003AA8C__5CBdatFUl(5);
    }

    func_801390E0__FPP11CFileHandle((void**)(p + 0x28));
    func_801390E0__FPP11CFileHandle((void**)(p + 0x2c));
    func_801390E0__FPP11CFileHandle((void**)(p + 0x30));
    func_801390E0__FPP11CFileHandle((void**)(p + 0x34));

    void* ptr38 = *(void**)(p + 0x38);
    p[0x54] = 0;
    if (ptr38) {
        deallocate__Q23mtl10MemManagerFPv(ptr38);
        *(u32*)(p + 0x38) = 0;
    }

    void* obj44 = *(void**)(p + 0x44);
    if (obj44) {
        delete (CItemBoxObjVt08*)obj44;
        *(u32*)(p + 0x44) = 0;
    }

    func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(*(void**)(p + 0x3c));
    func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(*(void**)(p + 0x40));

    void* obj5c = *(void**)(p + 0x5c);
    if (obj5c) {
        delete (CItemBoxObjVt08*)obj5c;
        *(u32*)(p + 0x5c) = 0;
    }

    func_8045F778__17UnkClass_8045F564Fv(p + 0x8);
    func_8045F778__17UnkClass_8045F564Fv(p + 0x18);

    ((CItemBoxObjVt0C*)(p + 0x70))->_v0C();
    ((CItemBoxObjVt0C*)(p + 0x88))->_v0C();
    ((CItemBoxObjVt0C*)(p + 0xa0))->_v0C();
    ((CItemBoxObjVt0C*)(p + 0xb8))->_v0C();
    ((CItemBoxObjVt0C*)(p + 0xd0))->_v0C();

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
u32 func_801CB038(void* self) {
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
u32 func_801CB0FC(void* self) {
    u8* p = (u8*)self;
    if (p[0x528]) return 1;
    if (CSysWin_getUnk34(p + 0x4ac)) return 1;
    if (CSysWin_getUnk34(p + 0x4e8)) return 1;
    if (p[0x544]) return 1;
    return func_801D3320(p + 0xe8);
}

// Get field depending on window state.
u8 func_801CB184(void* self) {
    if (CSysWin_getUnk34((u8*)self + 0x4e8)) return 0;
    s32 state = *(s32*)((u8*)self + 0x58);
    if (state != 3) return 0;
    return ((u8*)self)[0x542];
}

u8 CItemBoxGrid::GetField549() { return reinterpret_cast<CItemBoxGridFull*>(this)->bytes.field_549; }

// Check if any sub-system is active.
u32 func_801CB1E4(void* self) {
    u8* p = (u8*)self;
    if (func_801EB020(p + 0x3e4)) return 1;
    if (func_80208358(p + 0x418)) return 1;
    if (((CExchangeWin*)(p + 0x440))->getField24()) return 1;
    if (func_8022DB6C(p + 0x468)) return 1;
    if (CSysWin_getUnk34(p + 0x4ac)) return 1;
    return CSysWin_getUnk34(p + 0x4e8);
}

// Initialize item display state. `item` (func_801C631C result) is declared
// first so MWCC colors it to r31 (first-declared -> highest saved reg).
void func_801CB28C(void* self) {
    u8* p = (u8*)self;
    if (*(u32*)(p + 0x58)) return;
    *(u32*)(p + 0x58) = 1;
    p[0x61] = 0;
    u32 item;   // func_801C631C result; decl position drives r31/r30 split
    func_801CFD2C(self);
    func_801D0BD8(self);
    func_801D421C(p + 0x1D8);
    func_801D4260(p + 0x1D8, (p + (s8)p[0x6F])[0x62]);
    u8 idx0 = p[0x525];
    u8 idx1 = p[0x524];
    u32 entry_idx = (idx1 + idx0 * 10) & 0xFF;
    u8* sub = p + 0x54C;
    item = func_801C631C(sub, entry_idx);
    u32 r2 = func_801C62AC(sub, entry_idx);
    func_801D47D4(p + 0x1D8, r2 & 0xFFFF, item, 1);
    u32 v = func_801C6938(sub, entry_idx);
    func_801D4AE0((void*)(p + 0x1D8), 1, (void*)v);
    func_801D216C(p + 0xB8, 0);
    p[0x549] = 0;
    if (p[0x527] == 4) func_801D4C3C(p + 0x1D8, 1);
}

// Advance item box state.
void func_801CB38C(void* self) {
    u8* p = (u8*)self;
    if (*(s32*)(p + 0x58) != 3) return;
    if (func_801D3320(p + 0xe8)) return;
    *(u32*)(p + 0x58) = 4;
    ((CItemInstVt2CInt*)(void*)*(u32*)(p + 0x44))->_v2C((void*)*(u32*)(p + 0x50), 0);
    ((CItemInstVt2CInt*)(void*)*(u32*)(p + 0x44))->_v2C((void*)*(u32*)(p + 0x48), 0);
    ((CItemInstVt2CInt*)(void*)*(u32*)(p + 0x44))->_v2C((void*)*(u32*)(p + 0x4c), 1);
    p[0x61] = 0;
    func_801D216C(p + 0x70, 0);
    func_801D216C(p + 0x88, 0);
    func_801D216C(p + 0xd0, 0);
    func_801D216C(p + 0xb8, 0);
    advanceItemBoxState__FP12CItemBoxInfo(p + 0x1d8);
    if (!p[0x52c]) func_80138078__FUl(6);
}

// Clear a 14-byte region (list/array init). Retail uses memset for the
// 12-byte block at +0x62, then two byte clears.
void func_801CB480(void* self) {
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
void func_801CB4E4(void* self) {
    u8* p = (u8*)self;
    if (func_801D3320(p + 0xe8)) return;
    if (p[0x528]) return;
    u8 idx = p[0x6f] + 1;
    p[0x6f] = idx;
    if ((s8)idx >= p[0x6e]) {
        p[0x6f] = 0;
    }
    func_801D05D4(self, 0);
    func_801CFF28(self);
    func_80138078__FUl(0x70);
}

// Decrement a list counter with wrap.
void func_801CB56C(void* self) {
    u8* p = (u8*)self;
    if (func_801D3320(p + 0xe8)) return;
    if (p[0x528]) return;
    u8 idx = p[0x6f] - 1;
    p[0x6f] = idx;
    if ((s8)idx < 0) {
        p[0x6f] = p[0x6e] - 1;
    }
    func_801D05D4(self, 1);
    func_801CFF28(self);
    func_80138078__FUl(0x70);
}

void func_801CB5F0(void* self) {
    u8* p = (u8*)self;
    if (p[0x542]) return;
    if (CSysWin_getUnk34(p + 0x4ac)) {
        if (CSysWin_isActive(p + 0x4ac)) {
            s8 val = (s8)p[0x540] - 1;
            p[0x540] = (u8)val;
            if (val < 0) p[0x540] = 1;
            func_801D0950(self);
            func_80138078__FUl(1);
        }
        return;
    }
    if (CSysWin_getUnk34(p + 0x4e8)) return;
    if (func_80208358(p + 0x418)) {
        if (func_80208360(p + 0x418)) {
            func_802083CC(p + 0x418);
            u8 temp[16];
            func_80208760(temp, p + 0x418);
            void** vtbl = *(void***)(p + 0xa0);
            ((void(*)(void*, void*))vtbl[4])(p + 0xa0, temp);
            func_80138078__FUl(1);
        }
        return;
    }
    if (func_8022DB6C(p + 0x468)) return;
    if (func_801D3320(p + 0xe8)) {
        if (func_801D3328(p + 0xe8)) {
            func_801D3620(p + 0xe8);
            u8 temp[16];
            func_801D3454(temp, p + 0xe8);
            void** vtbl = *(void***)(p + 0xa0);
            ((void(*)(void*, void*))vtbl[4])(p + 0xa0, temp);
            goto sound_and_return;
        }
        return;
    }
    if (p[0x544]) {
        void* arr = p + 0x288;
        s8 idx = (s8)p[0x546] - 1;
        while (idx != (s8)p[0x546]) {
            if (idx < 0) idx = 3;
            if (ArrayGet12((const unsigned short*)arr, (u8)((s8)p[0x545] + idx))) {
                p[0x546] = (u8)idx;
                break;
            }
            idx--;
        }
        u8 combined = (u8)((s8)p[0x545] + (s8)p[0x546]);
        u32 tmp[3];
        func_801CB9D8(tmp, arr, combined);
        void** vtbl = *(void***)(p + 0xa0);
        ((void(*)(void*, void*))vtbl[4])(p + 0xa0, tmp);
        goto sound_and_return;
    }
    if (p[0x528]) {
        if (p[0x527] == 1) {
            if (func_801EB028(p + 0x3e4)) {
                u8* sub = p + 0x54c;
                u8 entry = (u8)((s8)p[0x525] * 10 + (s8)p[0x524]);
                u32 kind = func_801C62AC((CItemBoxGridFull*)sub, entry);
                func_801392E4(kind & 0xFFFF);
                s32 count = func_801C6388((CItemBoxGridFull*)sub, entry);
                if ((s8)count > 0) {
                    s8 val = (s8)p[0x529] + 1;
                    p[0x529] = (u8)val;
                    if (val > (s8)count) p[0x529] = 1;
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
                s8 val = (s8)p[0x529] - 1;
                p[0x529] = (u8)val;
                if (val < 0) p[0x529] = 1;
                u8 temp[16];
                func_8022D0F8(temp, p + 0x440, p[0x529]);
                void** vtbl = *(void***)(p + 0xa0);
                ((void(*)(void*, void*))vtbl[4])(p + 0xa0, temp);
                goto sound_and_return;
            }
            return;
        }
        goto sound_and_return;
    }
    if (p[0x527] == 4) {
        s8 val = (s8)p[0x525] - 1;
        p[0x525] = (u8)val;
        if (val < -2) {
            p[0x525] = (u8)(LookupIndexedByte((char*)self) - 1);
        } else if (val < 0) {
            p[0x525] = (u8)-2;
        }
        func_801D0950(self);
        func_801D0328(self);
        goto sound_and_return;
    }
    {
        s8 val = (s8)p[0x525] - 1;
        p[0x525] = (u8)val;
        if (val < -1) {
            p[0x525] = (u8)(LookupIndexedByte((char*)self) - 1);
        }
        func_801D0950(self);
        func_801D0328(self);
    }
sound_and_return:
    func_80138078__FUl(1);
}

__declspec(noinline) unsigned short ArrayGet12(const unsigned short* p, unsigned char i) {
    if (i < 12) {
        return p[i];
    }
    return 0;
}

// Open item: entry-copy base/value register clash. Retail keeps the base in r5
// (value loads into freed r4); MWCC always bases in r4 (value in r5), same
// ABI-boundary r4<->r5 class as func_800B7680. Decl-order and word-indexed
// variants invariant; witness rejects (no bijection in [3,11)).
// Open item: entry-copy base/value register clash. Retail keeps the base in r5
// (value loads into freed r4); MWCC always bases in r4 (value in r5), same
// ABI-boundary r4<->r5 class as func_800B7680. Decl-order and word-indexed
// variants invariant; witness rejects (no bijection in [3,11)).
__declspec(noinline) void func_801CB9D8(u32* dst, void* src, u32 idx) {
    if (idx >= 12) return;
    u32 v24, v28;
    u8* e = (u8*)src + idx * 12;
    v24 = *(u32*)(e + 0x18);
    v28 = *(u32*)(e + 0x1c);
    *(u32*)((u8*)dst + 4) = v28;
    *(u32*)((u8*)dst + 0) = v24;
    *(u32*)((u8*)dst + 8) = *(u32*)(e + 0x20);
}

void func_801CBA04(void* self) {
    u8* p = (u8*)self;
    if (p[0x542]) return;
    if (CSysWin_getUnk34(p + 0x4ac)) {
        if (CSysWin_isActive(p + 0x4ac)) {
            s8 val = (s8)p[0x540] + 1;
            p[0x540] = (u8)val;
            if (val > 1) p[0x540] = 0;
            func_801D0950(self);
            func_80138078__FUl(1);
        }
        return;
    }
    if (CSysWin_getUnk34(p + 0x4e8)) return;
    if (func_80208358(p + 0x418)) {
        if (func_80208360(p + 0x418)) {
            func_8020844C(p + 0x418);
            u8 temp[16];
            func_80208760(temp, p + 0x418);
            void** vtbl = *(void***)(p + 0xa0);
            ((void(*)(void*, void*))vtbl[4])(p + 0xa0, temp);
            func_80138078__FUl(1);
        }
        return;
    }
    if (func_8022DB6C(p + 0x468)) return;
    if (func_801D3320(p + 0xe8)) {
        if (func_801D3328(p + 0xe8)) {
            func_801D3698(p + 0xe8);
            u8 temp[16];
            func_801D3454(temp, p + 0xe8);
            void** vtbl = *(void***)(p + 0xa0);
            ((void(*)(void*, void*))vtbl[4])(p + 0xa0, temp);
            goto sound_and_return2;
        }
        return;
    }
    if (p[0x544]) {
        void* arr = p + 0x288;
        s8 idx = (s8)p[0x546] + 1;
        while (idx != (s8)p[0x546]) {
            if (idx >= 4) idx = 0;
            if (ArrayGet12((const unsigned short*)arr, (u8)((s8)p[0x545] + idx))) {
                p[0x546] = (u8)idx;
                break;
            }
            idx++;
        }
        u8 combined = (u8)((s8)p[0x545] + (s8)p[0x546]);
        u32 tmp[3];
        func_801CB9D8(tmp, arr, combined);
        void** vtbl = *(void***)(p + 0xa0);
        ((void(*)(void*, void*))vtbl[4])(p + 0xa0, tmp);
        goto sound_and_return2;
    }
    if (p[0x528]) {
        if (p[0x527] == 1) {
            if (func_801EB028(p + 0x3e4)) {
                u8* sub = p + 0x54c;
                u8 entry = (u8)((s8)p[0x525] * 10 + (s8)p[0x524]);
                u32 kind = func_801C62AC((CItemBoxGridFull*)sub, entry);
                func_801392E4(kind & 0xFFFF);
                s32 count = func_801C6388((CItemBoxGridFull*)sub, entry);
                if ((s8)count > 0) {
                    s8 val = (s8)p[0x529] - 1;
                    p[0x529] = (u8)val;
                    if (val < 1) p[0x529] = (u8)count;
                }
                func_801EB314(p + 0x3e4);
                func_801EB04C((void*)(p + 0x3e4), (u32)p[0x529]);
                u32 cost = func_801C5FC0((CItemBoxGridFull*)sub, entry);
                u32 total = (s8)p[0x529] * cost;
                func_801EB064((void*)(p + 0x3e4), total);
                goto sound_and_return2;
            }
            return;
        } else if (p[0x527] == 2) {
            if (((CExchangeWin*)(p + 0x440))->getField27()) {
                s8 val = (s8)p[0x529] + 1;
                p[0x529] = (u8)val;
                if (val > 1) p[0x529] = 0;
                u8 temp[16];
                func_8022D0F8(temp, p + 0x440, p[0x529]);
                void** vtbl = *(void***)(p + 0xa0);
                ((void(*)(void*, void*))vtbl[4])(p + 0xa0, temp);
                goto sound_and_return2;
            }
            return;
        }
        goto sound_and_return2;
    }
    if (p[0x527] == 4) {
        s8 val = (s8)p[0x525] + 1;
        p[0x525] = (u8)val;
        if (val == -1) {
            p[0x525] = 0;
        } else if (val >= 0) {
            u8 max = (u8)LookupIndexedByte((char*)self);
            if ((u8)val >= max) p[0x525] = (u8)-2;
        }
        func_801D0950(self);
        func_801D0328(self);
        goto sound_and_return2;
    }
    {
        s8 val = (s8)p[0x525] + 1;
        p[0x525] = (u8)val;
        u8 max = (u8)LookupIndexedByte((char*)self);
        if ((u8)val >= max) p[0x525] = (u8)-1;
        func_801D0950(self);
        func_801D0328(self);
    }
sound_and_return2:
    func_80138078__FUl(1);
}

// Down navigation. optimize_for_size: retail saves r29-r31 as one stmw block
// (-0x30 frame); sub (r29) and entry (r30) are declared at function scope so
// their live ranges span the calls and MWCC colors them to the saved regs.
#pragma optimize_for_size on
void func_801CBDE8(void* self) {
    u8* sub;
    u16 entry;
    u8* p = (u8*)self;
    if (p[0x542]) return;
    if (CSysWin_getUnk34(p + 0x4ac)) return;
    if (CSysWin_getUnk34(p + 0x4e8)) return;
    if (func_80208358(p + 0x418)) return;
    if (func_8022DB6C(p + 0x468)) {
        if (!func_8022DB74(p + 0x468)) return;
        if (func_8022E490(p + 0x468)) return;
        func_8022E3A8(p + 0x468);
        func_80138078__FUl(1);
        return;
    }
    if (p[0x528]) {
        if (p[0x527] != 1) return;
        if (!func_801EB028(p + 0x3e4)) return;
        sub = p + 0x54c;
        entry = (u8)(p[0x524] + p[0x525] * 10);
        u32 kind = func_801C62AC((CItemBoxGridFull*)sub, entry);
        func_801392E4(kind & 0xFFFF);
        s32 chk = (s8)func_801C6388((CItemBoxGridFull*)sub, entry);
        if (chk > 0) {
            // stack-count step of 10, clamped to a minimum of 1
            s8 cnt = (s8)p[0x529] - 10;
            p[0x529] = (u8)cnt;
            if (cnt < 1) p[0x529] = 1;
        }
        func_801EB04C((void*)(p + 0x3e4), (u32)p[0x529]);
        u32 v = func_801C5FC0((CItemBoxGridFull*)sub, entry);
        func_801EB064((void*)(p + 0x3e4), (s8)p[0x529] * v);
        func_80138078__FUl(1);
        return;
    }
    if (func_801D3320(p + 0xe8)) {
        if (!func_801D3328(p + 0xe8)) return;
        func_801D3724(p + 0xe8);
        u32 tmp[4];
        func_801D3454(tmp, p + 0xe8);
        void** vtbl = *(void***)(p + 0xa0);
        ((void(*)(void*, u32*))vtbl[4])(p + 0xa0, tmp);
        func_80138078__FUl(1);
        return;
    }
    if (p[0x544]) {
        u8 cur = p[0x545];
        s8 idx;
        sub = p + 0x288;
        // scan backwards for a non-empty tab slot, wrapping at 2
        for (idx = (s8)cur - 1; idx != (s8)cur; idx--) {
            if (idx < 0) idx = 2;
            u16 val = ArrayGet12((const u16*)sub, (u8)(p[0x546] + (u8)idx * 4));
            if (val) { p[0x545] = (u8)idx; break; }
        }
        u32 buf[3];
        func_801CB9D8(buf, sub, (u8)(p[0x546] + (u8)p[0x545] * 4));
        void** vtbl = *(void***)(p + 0xa0);
        ((void(*)(void*, u32*))vtbl[4])(p + 0xa0, buf);
        func_80138078__FUl(1);
        return;
    }
    s8 row = (s8)p[0x525];
    if (row == -2) return;
    if (row == -1) {
        func_801CB56C(self);
        func_801D0950(self);
        return;
    }
    if ((s8)p[0x524] == 0) {
        p[0x524] = 9;
        func_801CC4E8(self);
        func_801D0950(self);
    } else {
        p[0x524]--;
        if ((s8)p[0x524] < 0) p[0x524] = 9;
        func_801D0950(self);
        func_801D0328(self);
    }
    func_80138078__FUl(1);
}
#pragma optimize_for_size off

// Up navigation. Same shape as func_801CBDE8 (see notes there).
#pragma optimize_for_size on
void func_801CC0EC(void* self) {
    u8* sub;
    u16 entry;
    u8* p = (u8*)self;
    if (p[0x542]) return;
    if (CSysWin_getUnk34(p + 0x4ac)) return;
    if (CSysWin_getUnk34(p + 0x4e8)) return;
    if (func_80208358(p + 0x418)) return;
    if (func_8022DB6C(p + 0x468)) {
        if (!func_8022DB74(p + 0x468)) return;
        if (func_8022E490(p + 0x468)) return;
        func_8022E2F8(p + 0x468);
        func_80138078__FUl(1);
        return;
    }
    if (p[0x528]) {
        if (p[0x527] != 1) return;
        if (!func_801EB028(p + 0x3e4)) return;
        sub = p + 0x54c;
        entry = (u8)(p[0x524] + p[0x525] * 10);
        u32 kind = func_801C62AC((CItemBoxGridFull*)sub, entry);
        func_801392E4(kind & 0xFFFF);
        s32 chk = (s8)func_801C6388((CItemBoxGridFull*)sub, entry);
        if (chk > 0) {
            // stack-count step of 10, clamped to the item's max stack
            s8 cnt = (s8)p[0x529] + 10;
            p[0x529] = (u8)cnt;
            if (cnt > chk) p[0x529] = (u8)chk;
        }
        func_801EB04C((void*)(p + 0x3e4), (u32)p[0x529]);
        u32 v = func_801C5FC0((CItemBoxGridFull*)sub, entry);
        func_801EB064((void*)(p + 0x3e4), (s8)p[0x529] * v);
        func_80138078__FUl(1);
        return;
    }
    if (func_801D3320(p + 0xe8)) {
        if (!func_801D3328(p + 0xe8)) return;
        func_801D377C(p + 0xe8);
        u32 tmp[4];
        func_801D3454(tmp, p + 0xe8);
        void** vtbl = *(void***)(p + 0xa0);
        ((void(*)(void*, u32*))vtbl[4])(p + 0xa0, tmp);
        func_80138078__FUl(1);
        return;
    }
    if (p[0x544]) {
        u8 cur = p[0x545];
        s8 idx;
        sub = p + 0x288;
        // scan forwards for a non-empty tab slot, wrapping at 3
        for (idx = (s8)cur + 1; idx != (s8)cur; idx++) {
            if (idx >= 3) idx = 0;
            u16 val = ArrayGet12((const u16*)sub, (u8)(p[0x546] + (u8)idx * 4));
            if (val) { p[0x545] = (u8)idx; break; }
        }
        u32 buf[3];
        func_801CB9D8(buf, sub, (u8)(p[0x546] + (u8)p[0x545] * 4));
        void** vtbl = *(void***)(p + 0xa0);
        ((void(*)(void*, u32*))vtbl[4])(p + 0xa0, buf);
        func_80138078__FUl(1);
        return;
    }
    s8 row = (s8)p[0x525];
    if (row == -2) return;
    if (row == -1) {
        func_801CB4E4(self);
        func_801D0950(self);
        return;
    }
    if ((s8)p[0x524] == 9) {
        p[0x524] = 0;
        func_801CC3F4(self);
        func_801D0950(self);
    } else {
        p[0x524]++;
        if ((s8)p[0x524] >= 10) p[0x524] = 0;
        func_801D0950(self);
        func_801D0328(self);
    }
    func_80138078__FUl(1);
}

// Grid state update with conditions.
// Grid state update.
// optimize_for_size: retail uses stmw r30/lmw r30; sub (r31) is declared
// before p (r30) so MWCC colors it to the higher saved register.
#pragma optimize_for_size on
void func_801CC3F4(void* self) {
    u8* sub;
    u8* p = (u8*)self;
    if (func_801D3320(p + 0xe8)) return;
    if (p[0x528]) return;
    if (CSysWin_getUnk34(p + 0x4ac)) return;
    if (CSysWin_getUnk34(p + 0x4e8)) return;
    if (func_80208358(p + 0x418)) return;
    if (func_8022DB6C(p + 0x468)) return;
    sub = p + 0x54c;
    func_801C5EF4((CItemBoxGridFull*)sub);
    // Retail calls LookupIndexedByte twice: once for the compare, once for
    // the store (subi from the fresh call result).
    if ((s8)p[0x525] < (u8)LookupIndexedByte((char*)sub))
        p[0x525] = (u8)(LookupIndexedByte((char*)sub) - 1);
    func_801CFFEC(self);
    func_801D0328(self);
    u8 rows = sub[0x2803];
    u8 f = 1;
    if (rows != 0) f = rows;
    if (f != 1) func_80138078__FUl(0xa);
}
#pragma optimize_for_size off

// Grid state update (decrement variant).
#pragma optimize_for_size on
void func_801CC4E8(void* self) {
    u8* sub;
    u8* p = (u8*)self;
    if (func_801D3320(p + 0xe8)) return;
    if (p[0x528]) return;
    if (CSysWin_getUnk34(p + 0x4ac)) return;
    if (CSysWin_getUnk34(p + 0x4e8)) return;
    if (func_80208358(p + 0x418)) return;
    if (func_8022DB6C(p + 0x468)) return;
    sub = p + 0x54c;
    func_801C5F20((CItemBoxGridFull*)sub);
    if ((s8)p[0x525] < (u8)LookupIndexedByte((char*)sub))
        p[0x525] = (u8)(LookupIndexedByte((char*)sub) - 1);
    func_801CFFEC(self);
    func_801D0328(self);
    u8 rows = sub[0x2803];
    u8 f = rows == 0 ? 1 : rows;
    if (f != 1) func_80138078__FUl(0xa);
}
#pragma optimize_for_size off

// Guard chain shares a single exit label in retail (every check branches to
// the epilogue), so express the checks as one && chain.
#pragma optimize_for_size on
void func_801CC5DC(void* self) {
    u8* p = (u8*)self;
    if (!p[0x542] && !p[0x528] && !CSysWin_getUnk34(p + 0x4ac) &&
        !CSysWin_getUnk34(p + 0x4e8) && !func_80208358(p + 0x418) &&
        !func_8022DB6C(p + 0x468) && !p[0x544]) {
        if (func_801D3320(p + 0xe8)) {
            if (!func_801D3328(p + 0xe8)) return;
            func_801D0950(self);
            func_801D216C(p + 0xa0, 0);
            func_801D3408(p + 0xe8);
            func_80138078__FUl(6);
            return;
        }
        if (*(u32*)(p + 0x58) != 3) return;
        if (!func_801D3328(p + 0xe8)) return;
        void* obj = *(void**)(p + 0x44);
        void* subObj = *(void**)((u8*)obj + 0x10);
        void** vtbl = *(void***)subObj;
        void* a = ((void* (*)(void*, const char*, int))vtbl[0x3c / 4])(subObj, &lbl_eu_8050566C[0x301], 1);
        void* b = ((void* (*)(void*, const char*, int))vtbl[0x3c / 4])(subObj, &lbl_eu_8050566C[0x2f8], 1);
        u32 result;
        func_80137924(&result, b, a, subObj);
        func_801D3430(p + 0xe8, &result);
        func_801D353C(p + 0xe8, (u8)(p[0x547] + p[0x548]));
        u32 tmp[4];
        func_801D3454(tmp, p + 0xe8);
        vtbl = *(void***)(p + 0xa0);
        ((void (*)(void*, u32*))vtbl[4])(p + 0xa0, tmp);
        func_801D216C(p + 0xa0, 1);
        func_801D216C(p + 0x70, 0);
        func_801D216C(p + 0xb8, 0);
        func_801D3330(p + 0xe8);
        p[0x526] = 0;
        func_80138078__FUl(2);
    }
}
#pragma optimize_for_size off

void func_801CC7B0(void* self, int r4) {
    u8* p = (u8*)self;
    if (CSysWin_getUnk34(p + 0x4ac)) {
        if (!CSysWin_isActive(p + 0x4ac)) return;
        func_8022B8E4(p + 0x4ac);
        func_801D216C(p + 0xa0, 0);
        p[0x540] = 1;
        *(u32*)(p + 0x58) = 0x17;
        return;
    }
    if (CSysWin_getUnk34(p + 0x4e8)) {
        if (!CSysWin_isActive(p + 0x4e8)) return;
        func_8022B8E4(p + 0x4e8);
        func_801D216C(p + 0x70, 0);
        *(u32*)(p + 0x58) = 0x14;
        return;
    }
    if (func_80208358(p + 0x418)) {
        if (!func_80208360(p + 0x418)) return;
        func_801D0950(self);
        func_801D216C(p + 0x70, 1);
        func_801D216C(p + 0xa0, 0);
        func_80208838(p + 0x418);
        func_802083A4(p + 0x418);
        *(u32*)(p + 0x58) = 8;
        p[0x528] = 0;
        if (!r4) func_80138078__FUl(6);
        return;
    }
    if (func_8022DB6C(p + 0x468)) {
        if (!func_8022DB74(p + 0x468)) return;
        if (func_8022E490(p + 0x468)) {
            func_8022DD68(p + 0x468);
            *(u32*)(p + 0x58) = 0x11;
            func_801D0950(self);
            func_801D216C(p + 0x70, 1);
            func_801D216C(p + 0xd0, 0);
        } else if (func_8022E488(p + 0x468)) {
            func_8022E3AC(p + 0x468);
            u32 buf[4];
            func_8022E498(buf, p + 0x468);
            void** vtbl = *(void***)(p + 0xd0);
            ((void(*)(void*, u32*))vtbl[4])(p + 0xd0, buf);
        } else {
            func_8022DD68(p + 0x468);
            *(u32*)(p + 0x58) = 0x11;
            func_801D0950(self);
            func_801D216C(p + 0x70, 1);
            func_801D216C(p + 0xd0, 0);
        }
        if (!r4) func_80138078__FUl(6);
        return;
    }
    if (p[0x528]) {
        if (p[0x527] == 1) {
            if (!func_801EB028(p + 0x3e4)) return;
            if (*(u32*)(p + 0x58) != 0x19) return;
            func_801EB178(p + 0x3e4);
            *(u32*)(p + 0x58) = 0x1a;
        } else if (p[0x527] == 2) {
            if (!((CExchangeWin*)(p + 0x440))->getField27()) return;
            func_8022D0D0(p + 0x440);
            *(u32*)(p + 0x58) = 0x0e;
            func_801D216C(p + 0x70, 1);
            func_801D216C(p + 0xa0, 0);
            func_801D0950(self);
        }
        if (!r4) func_80138078__FUl(6);
        return;
    }
    if (p[0x544]) {
        p[0x544] = 0;
        func_801D216C(p + 0x70, 1);
        func_801D216C(p + 0xa0, 0);
        if (!r4) func_80138078__FUl(6);
        return;
    }
    if (func_801D3320(p + 0xe8)) {
        if (!func_801D3328(p + 0xe8)) return;
        func_801D0950(self);
        func_801D216C(p + 0xa0, 0);
        func_801D3408(p + 0xe8);
    } else {
        if (p[0x527] == 4) {
            p[0x525] = (u8)-2;
            func_801D0950(self);
        }
    }
    if (!r4) func_80138078__FUl(6);
}



void func_801CCAF0(void* self) {
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
        u32 fsResult = func_802087B8(p + 0x418);
        if ((fsResult & 0xFF) == 1) {
            u8* sub = p + 0x54c;
            u8 entry = (u8)((s8)p[0x525] * 10 + (s8)p[0x524]);
            void* item = func_801C631C((CItemBoxGridFull*)sub, entry);
            u32 type = (*(u32*)item >> 12) & 0xF;
            if (type == 2 || ((type + 0xfc) & 0xFF) <= 4) {
                void* inst = CItem_initItemImplInstances(item);
                void** vtbl = *(void***)inst;
                u32 count = (u32)((u32(*)(void*, void*))vtbl[0xc])(inst, item);
                u32 j;
                for (j = 0; j < count; j++) {
                    void* inst2 = CItem_initItemImplInstances(item);
                    void** vtbl2 = *(void***)inst2;
                    s16 subId = (s16)((s32(*)(void*, void*, u32))vtbl2[0x10])(inst2, item, j);
                    if (subId != -1) {
                        p[0x541] = 1;
                        break;
                    }
                }
            }
            if (item) {
                void* inst = CItem_initItemImplInstances(item);
                void** vtbl = *(void***)inst;
                ((void(*)(void*, void*))vtbl[4])(inst, item);
            }
            func_801CFF28(self);
            func_80138078__FUl(0x7a);
            func_801D0950(self);
            goto after_fs;
        }
        if (p[0x6f] >= 0) {
            u8 listVal = p[0x62 + (s8)p[0x6f]];
            if (listVal == 0xa) {
                u32 count2 = 0;
                void* ecData = func_8009ECB0();
                u32* ptr = (u32*)((u8*)ecData + 4);
                u32 k;
                for (k = 0; k < 7; k++) {
                    u32 val;
                    if ((k & 0xFF) < 3) {
                        val = ptr[k & 0xFF];
                    } else {
                        val = ptr[k & 0xFF];
                    }
                    if ((s32)val > 0) {
                        void* obj = (void*)func_8009EC9C(val & 0xFFFF);
                        if (*(u32*)((u8*)obj + 0x176c) != 1) count2++;
                    }
                }
                if (count2 <= 1) {
                    func_80138078__FUl(5);
                    return;
                }
            }
            func_80138078__FUl(3);
        }
after_fs:
        func_801D216C(p + 0xa0, 0);
        func_802083A4(p + 0x418);
        *(u32*)(p + 0x58) = 8;
        return;
    }
    if (func_8022DB6C(p + 0x468)) {
        if (func_8022DB74(p + 0x468)) {
            if (func_8022E488(p + 0x468)) {
                if (func_8022E490(p + 0x468)) {
                    func_8022DD68(p + 0x468);
                    *(u32*)(p + 0x58) = 0x11;
                    func_801D0950(self);
                    func_801D216C(p + 0x70, 1);
                    func_801D216C(p + 0xd0, 0);
                } else {
                    u8* sub = p + 0x54c;
                    u8 entry = (u8)((s8)p[0x525] * 10 + (s8)p[0x524]);
                    void* item = func_801C631C((CItemBoxGridFull*)sub, entry);
                    u32 kind = func_801C62AC((CItemBoxGridFull*)sub, entry);
                    s32 count = func_801C6388((CItemBoxGridFull*)sub, entry);
                    func_8022DD90(p + 0x468);
                    func_801D216C(p + 0xd0, 0);
                    u32 teachType = func_8022E4FC(p + 0x468);
                    u32 teachState = func_8022E504(p + 0x468);
                    if (teachState == 1) {
                        u32 k2 = func_80139358(kind & 0xFFFF);
                        u32 catVal = func_801361E8(lbl_eu_80664104, (const char*)&lbl_eu_8050566C[0x212], k2 & 0xFFFF);
                        if ((catVal & 0xFF) == 7) func_8013B428__FUl(0x87);
                    } else if (teachState == 2) {
                        u32 w = *(u32*)item;
                        if ((w >> 20) == 0x75a) func_8013B428__FUl(0x88);
                    } else if (teachState == 4) {
                        u32 w = *(u32*)item;
                        if ((w >> 20) == 0x775) func_8013B428__FUl(0x8a);
                    } else if (teachState == 5) {
                        u32 k3 = func_80139358(kind & 0xFFFF);
                        u32 catVal2 = func_801361E8(lbl_eu_80664104, (const char*)&lbl_eu_8050566C[0x212], k3 & 0xFFFF);
                        if ((catVal2 & 0xFF) == 2) func_8013B428__FUl(0x8b);
                    }
                    if (teachType == 3 || teachType == 8) {
                        if (teachState == 1) func_8013B428__FUl(0x89);
                    } else if (teachType == 6) {
                        u32 k4 = func_80139358(kind & 0xFFFF);
                        u32 catVal3 = func_801361E8(lbl_eu_80664104, (const char*)&lbl_eu_8050566C[0x212], k4 & 0xFFFF);
                        if ((catVal3 & 0xFF) == 5) func_8013B428__FUl(0x8d);
                    } else if (teachType == 7) {
                        func_8013B428__FUl(0x8c);
                    }
                    u32 w = *(u32*)item;
                    if ((w >> 20) == 0x867) func_8013B428__FUl(0x8e);
                    func_801D11B8(self, item, (s8)count - 1);
                    func_801CFF28(self);
                }
                func_80138078__FUl(3);
                return;
            }
            func_8022E3AC(p + 0x468);
            u8 temp[16];
            func_8022E498(temp, p + 0x468);
            void** vtbl = *(void***)(p + 0xd0);
            ((void(*)(void*, void*))vtbl[4])(p + 0xd0, temp);
            func_80138078__FUl(3);
            return;
        }
        return;
    }
    if (func_801D3320(p + 0xe8)) {
        if (func_801D3328(p + 0xe8)) {
            u32 val = func_801D37F4(p + 0xe8);
            s8 listIdx = (s8)p[0x6f];
            u8 listVal = p[0x62 + listIdx];
            func_80157824(listVal, val & 0xFF);
            p[0x547] = (u8)func_801D3808(p + 0xe8);
            p[0x548] = (u8)func_801D3810(p + 0xe8);
            func_801CC7B0(self, 1);
            p[0x524] = 0;
            if ((s8)p[0x525] > 0) p[0x525] = 0;
            func_801D0950(self);
            p[0x2d50] = 0;
            func_801CFF28(self);
            func_80138078__FUl(3);
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
            func_801C68A0((CItemBoxGridFull*)(p + 0x54c));
            func_80138078__FUl(4);
            return;
        }
        u8* sub = p + 0x54c;
        u8 entry = (u8)((s8)p[0x525] * 10 + (s8)p[0x524]);
        u8 cap = func_801C6840((CItemBoxGridFull*)sub);
        u8 active = func_801C67F8((CItemBoxGridFull*)sub);
        if (active >= cap) {
            if (func_801C673C((CItemBoxGridFull*)sub, entry)) {
                func_801C6770((CItemBoxGridFull*)sub, entry);
                func_801CFFEC(self);
                func_801D0328(self);
                func_801D0BD8(self);
                func_80138078__FUl(6);
                return;
            }
            u32 kind = func_801C62AC((CItemBoxGridFull*)sub, entry);
            if (kind) {
                *(u32*)(p + 0x58) = 0x12;
                u32 msg = (u32)func_80136190((void*)&lbl_eu_8050566C[0x14f], &lbl_eu_8050566C[0x158], 0xa1);
                func_8022B90C(p + 0x4e8, 0);
                func_8022B9B4(p + 0x4e8, msg, 0);
                func_8022BFC8(p + 0x4e8, 1);
                func_8022B8B8(p + 0x4e8);
                func_801D216C(p + 0x70, 0);
                func_80138078__FUl(5);
                return;
            }
            return;
        }
        if (func_801C6528((CItemBoxGridFull*)sub, entry)) return;
        u32 kind2 = func_801C62AC((CItemBoxGridFull*)sub, entry);
        if (!kind2) return;
        func_801C6770((CItemBoxGridFull*)sub, entry);
        func_801CFFEC(self);
        func_801D0328(self);
        func_801D0BD8(self);
        if (func_801C673C((CItemBoxGridFull*)sub, entry)) {
            func_80138078__FUl(3);
        } else {
            func_80138078__FUl(6);
        }
        return;
    }
    if (mode == 1) {
        if (p[0x528]) {
            if (func_801EB028(p + 0x3e4)) {
                if (*(u32*)(p + 0x58) == 0x19) {
                    u8* sub = p + 0x54c;
                    u8 entry = (u8)((s8)p[0x525] * 10 + (s8)p[0x524]);
                    void* item = func_801C631C((CItemBoxGridFull*)sub, entry);
                    u32 kind = func_801C62AC((CItemBoxGridFull*)sub, entry);
                    s32 count = func_801C6388((CItemBoxGridFull*)sub, entry);
                    u32 type = (*(u32*)item >> 12) & 0xF;
                    if ((u8)func_801C6528((CItemBoxGridFull*)sub, entry)) {
                        s16 sid = func_801C5F48((CItemBoxGridFull*)sub, entry);
                        func_801D0E88(self, type, (int)sid);
                    }
                    if (type == 2 || ((type + 0xfc) & 0xFF) <= 4) {
                        void* inst = CItem_initItemImplInstances(item);
                        void** vtbl = *(void***)inst;
                        u32 numSlots = (u32)((u32(*)(void*, void*))vtbl[0xc])(inst, item);
                        u32 j;
                        for (j = 0; j < numSlots; j++) {
                            void* inst2 = CItem_initItemImplInstances(item);
                            void** vtbl2 = *(void***)inst2;
                            s16 subId = (s16)((s32(*)(void*, void*, u32))vtbl2[0x10])(inst2, item, j);
                            if (subId != -1) {
                                p[0x541] = 1;
                                break;
                            }
                        }
                    }
                    u32 cost = func_801C5FC0((CItemBoxGridFull*)sub, entry);
                    s32 mult = (s8)p[0x529] * (s32)cost;
                    func_801571FC();
                    func_80157184(mult);
                    s8 extra = (s8)count - (s8)p[0x529];
                    func_801D11B8(self, item, extra);
                    func_801CFF28(self);
                    func_801EB178(p + 0x3e4);
                    *(u32*)(p + 0x58) = 0x1a;
                    func_80138078__FUl(0x2f);
                    return;
                }
                return;
            }
            return;
        }
        if (func_801EB028(p + 0x3e4)) {
            if (*(u32*)(p + 0x58) == 3) {
                u8* sub = p + 0x54c;
                u8 entry = (u8)((u8)((s8)p[0x525] * 10) + (u8)(s8)p[0x524]);
                u32 kind = func_801C62AC((CItemBoxGridFull*)sub, entry);
                if (kind) {
                    p[0x529] = 1;
                    u32 val = func_801C6938((CItemBoxGridFull*)sub, entry);
                    func_801EB030(p + 0x3e4, (void*)val);
                    func_801EB04C((void*)(p + 0x3e4), (u32)p[0x529]);
                    u32 cost2 = func_801C5FC0((CItemBoxGridFull*)sub, entry);
                    u32 total2 = (s8)p[0x529] * cost2;
                    func_801EB064(p + 0x3e4, total2);
                    if ((u8)func_801C6690((CItemBoxGridFull*)sub, entry)) {
                        *(u32*)(p + 0x58) = 0x12;
                        u32 msg2 = (u32)func_80136190((void*)&lbl_eu_8050566C[0x310], &lbl_eu_8050566C[0x158], 0x17);
                        func_8022B90C(p + 0x4e8, 0);
                        func_8022B9B4(p + 0x4e8, msg2, 0);
                        func_8022BFC8(p + 0x4e8, 1);
                        func_8022B8B8(p + 0x4e8);
                        func_801D216C(p + 0x70, 0);
                    } else if ((u8)func_801C6528((CItemBoxGridFull*)sub, entry)) {
                        void* item2 = func_801C631C((CItemBoxGridFull*)sub, entry);
                        u32 type2 = (*(u32*)item2 >> 12) & 0xF;
                        if (type2 == 2) {
                            *(u32*)(p + 0x58) = 0x12;
                            u32 msg3 = (u32)func_80136190((void*)&lbl_eu_8050566C[0x310], &lbl_eu_8050566C[0x158], 0x16);
                            func_8022B90C(p + 0x4e8, 0);
                            func_8022B9B4(p + 0x4e8, msg3, 0);
                            func_8022BFC8(p + 0x4e8, 1);
                            func_8022B8B8(p + 0x4e8);
                            func_801D216C(p + 0x70, 0);
                        } else {
                            *(u32*)(p + 0x58) = 0x15;
                            u32 msg4 = (u32)func_80136190((void*)&lbl_eu_8050566C[0x310], &lbl_eu_8050566C[0x158], 0x13);
                            u32 msg5 = (u32)func_80136190((void*)&lbl_eu_8050566C[0x310], &lbl_eu_8050566C[0x158], 0x14);
                            u32 msg6 = (u32)func_80136190((void*)&lbl_eu_8050566C[0x310], &lbl_eu_8050566C[0x158], 0x15);
                            func_8022B9B4(p + 0x4ac, msg4, 0);
                            func_8022BF6C(p + 0x4ac, msg5, msg6);
                            func_8022BFC8(p + 0x4ac, 0);
                            func_8022B8B8(p + 0x4ac);
                            func_801D216C(p + 0x70, 0);
                            p[0x540] = 1;
                        }
                    } else {
                        s8 listIdx2 = (s8)p[0x6f];
                        u8 listVal2 = p[0x62 + listIdx2];
                        if (listVal2 == 0xd) {
                            u32 kind3 = func_801C62AC((CItemBoxGridFull*)sub, entry);
                            if (func_801D12D4(self, kind3 & 0xFFFF)) {
                                *(u32*)(p + 0x58) = 0x15;
                                u32 msg7 = (u32)func_80136190((void*)&lbl_eu_8050566C[0x310], &lbl_eu_8050566C[0x158], 0x19);
                                u32 msg8 = (u32)func_80136190((void*)&lbl_eu_8050566C[0x310], &lbl_eu_8050566C[0x158], 0x14);
                                u32 msg9 = (u32)func_80136190((void*)&lbl_eu_8050566C[0x310], &lbl_eu_8050566C[0x158], 0x15);
                                func_8022B9B4(p + 0x4ac, msg7, 0);
                                func_8022BF6C(p + 0x4ac, msg8, msg9);
                                func_8022BFC8(p + 0x4ac, 0);
                                func_8022B8B8(p + 0x4ac);
                                func_801D216C(p + 0x70, 0);
                                p[0x540] = 1;
                            }
                        }
                        u32 lv = listVal2;
                        if (lv >= 2 && lv <= 9) {
                            func_801EB410(p + 0x3e4, 0);
                        } else {
                            func_801EB410(p + 0x3e4, 1);
                        }
                        func_801EB0D4(p + 0x3e4);
                        *(u32*)(p + 0x58) = 0x18;
                        p[0x528] = 1;
                        func_801D216C(p + 0x70, 0);
                    }
                    func_80138078__FUl(3);
                    return;
                }
                return;
            }
            return;
        }
        return;
    }
    if (mode == 2) {
        if (p[0x542]) return;
        if (p[0x52c]) return;
        if (p[0x528]) {
            if (((CExchangeWin*)(p + 0x440))->getField27()) {
                if (*(u32*)(p + 0x58) == 0xd) {
                    if ((s8)p[0x529] == 0) {
                        u8* sub = p + 0x54c;
                        u8 entry = (u8)((s8)p[0x525] * 10 + (s8)p[0x524]);
                        void* item = func_801C631C((CItemBoxGridFull*)sub, entry);
                        u32 kind = func_801C62AC((CItemBoxGridFull*)sub, entry);
                        s32 count = func_801C6388((CItemBoxGridFull*)sub, entry);
                        if (kind) {
                            u16 f52e = *(u16*)(p + 0x52e);
                            u16 f52a = *(u16*)(p + 0x52a);
                            u32 v1 = func_801C618C((CItemBoxGridFull*)sub, f52e, 0, f52a);
                            u32 v2 = func_801C618C((CItemBoxGridFull*)sub, kind & 0xFFFF, item, f52a);
                            if (v1 <= v2) {
                                u32 nVal = func_80136254((void*)lbl_eu_80664098, (const char*)&lbl_eu_8050566C[0x319], *(u16*)(p + 0x52a));
                                u32 test = func_801C618C((CItemBoxGridFull*)sub, nVal & 0xFFFF, 0, 0);
                                u32 diff = (v2 - v1) - test;
                                u32 better = (diff == 0) ? 0 : 1;
                                p[0x52d] = (u8)better;
                                p[0x52c] = 1;
                                func_801D11B8(self, item, (s8)count - 1);
                                func_801393CC(*(u16*)(p + 0x52e));
                                func_801392E4(*(u16*)(p + 0x52e));
                                func_80139358(*(u16*)(p + 0x52e));
                                func_801586D4(*(u16*)(p + 0x52e), 1);
                                func_80140E00(2, *(u16*)(p + 0x52e), 0);
                                func_8013B428__FUl(0xbb);
                                func_8013B428__FUl(0xbc);
                                func_8013B428__FUl(0xbd);
                                u32 eqId = *(u16*)(p + 0x52e);
                                if (eqId == 0x7ee) func_8013B428__FUl(0xbe);
                                else if (eqId == 0x835) func_8013B428__FUl(0xbf);
                                else if (eqId == 0x7ef) func_8013B428__FUl(0xc0);
                                else if (eqId == 0x837) func_8013B428__FUl(0xc1);
                                else if (eqId == 0x836) func_8013B428__FUl(0xc2);
                            }
                        }
                    }
                    func_8022D0D0(p + 0x440);
                    *(u32*)(p + 0x58) = 0xe;
                    func_801D216C(p + 0x70, 1);
                    func_801D216C(p + 0xa0, 0);
                    func_801D0950(self);
                    if ((s8)p[0x529] == 0) func_80138078__FUl(0x31);
                    else func_80138078__FUl(3);
                    return;
                }
                return;
            }
            return;
        }
        if (((CExchangeWin*)(p + 0x440))->getField27()) {
            if (*(u32*)(p + 0x58) == 3) {
                u8* sub = p + 0x54c;
                u8 entry = (u8)((s8)p[0x525] * 10 + (s8)p[0x524]);
                u32 kind = func_801C62AC((CItemBoxGridFull*)sub, entry);
                if (kind) {
                    u32 itemName = (u32)func_801394D4(kind & 0xFFFF);
                    char fmtBuf[64];
                    sprintf(fmtBuf, (const char*)&lbl_eu_8050566C[0x14c], itemName);
                    u32 iconName = (u32)func_801394D4(*(u16*)(p + 0x52e));
                    func_8022D19C(p + 0x440, fmtBuf, iconName);
                    func_8022D0A4(p + 0x440);
                    p[0x528] = 1;
                    p[0x529] = 1;
                    *(u32*)(p + 0x58) = 0xc;
                    func_801D216C(p + 0x70, 0);
                    func_80138078__FUl(3);
                    return;
                }
                return;
            }
            return;
        }
        return;
    }
    // Default mode handling
    if (p[0x528]) return;
    if (func_80208360(p + 0x418)) {
        if (func_8022DB74(p + 0x468)) {
            u8* sub = p + 0x54c;
            u8 entry = (u8)((s8)p[0x525] * 10 + (u8)(s8)p[0x524]);
            if ((u8)func_801C6690((CItemBoxGridFull*)sub, entry)) {
                func_80138078__FUl(5);
                return;
            }
            if (!func_801C62AC((CItemBoxGridFull*)sub, entry)) {
                func_80138078__FUl(5);
                return;
            }
            if ((u8)func_801C6528((CItemBoxGridFull*)sub, entry)) {
                func_80138078__FUl(5);
                return;
            }
            s8 listIdx3 = (s8)p[0x6f];
            u8 listVal3 = p[0x62 + listIdx3];
            if (listVal3 == 0xc) {
                func_80138078__FUl(5);
                return;
            }
            u32 sendVal = (mode == 3) ? 0xFF : listVal3;
            func_802084D4(p + 0x418, sendVal & 0xFF);
            func_801D216C(p + 0x70, 0);
            func_80208368(p + 0x418);
            *(u32*)(p + 0x58) = 6;
            p[0x528] = 1;
            func_80138078__FUl(3);
        }
    }
}

// Store value and call helpers.
void func_801CDB94(void* self, u32 val) {
    *(u16*)((u8*)self + 0x52e) = (u16)val;
    void* res = func_801394D4(val & 0xFFFF);
    func_801D4AE0((void*)((u8*)self + 0x1d8), 0, res);
    func_801CFF28(self);
}

// Get field depending on obj state.
u8 func_801CDBE0(void* self) {
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
            obj = (void*)func_8013639C((const void*)*(u32*)(lbl_eu_80664510), &lbl_eu_8050566C[0x321], val & 0xFFFF);
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
        func_801CB9D8(buf, list, (u32)v);
        ((CItemBoxObjA0Vt*)(p + 0xa0))->_v10(buf);
        func_80138078__FUl(2);
    } else {
        func_80138078__FUl(5);
    }
}
#pragma optimize_for_size off

// Check conditions and update state.
void func_801CDEE8(void* self) {
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
    func_801D0950(self);
    func_801D0328(self);
    func_80138078__FUl(2);
}

// Check prerequisites and return a state code: each sub-window must be
// idle (nonzero -> 0); then 2 while the item box is animating, 5 while a
// confirm prompt is up, otherwise derive from the selected category byte
// (0x527) and the pending-selection byte (0x525).
u32 func_801CDFB4(void* self) {
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
void func_801CE108(void* self) {
    u8* p = (u8*)self;
    if (!func_80137444__FPQ34nw4r3lyt13AnimTransformf((void*)*(u32*)(p + 0x48), lbl_eu_80667F78)) return;
    ((CItemInstVt2CInt*)(void*)*(u32*)(p + 0x44))->_v2C((void*)*(u32*)(p + 0x50), 0);
    ((CItemInstVt2CInt*)(void*)*(u32*)(p + 0x44))->_v2C((void*)*(u32*)(p + 0x48), 0);
    ((CItemInstVt2CInt*)(void*)*(u32*)(p + 0x44))->_v2C((void*)*(u32*)(p + 0x4C), 1);
    *(u32*)(p + 0x58) = 2;
}

extern "C" void func_801CE1A0(void* self) {
    u8* p = (u8*)self;
    if (func_80137444__FPQ34nw4r3lyt13AnimTransformf((void*)*(u32*)(p + 0x4c), lbl_eu_80667F78)) {
        *(u32*)(p + 0x58) = 3;
        ((CItemInstVt2CInt*)(void*)*(u32*)(p + 0x44))->_v2C((void*)*(u32*)(p + 0x48), 0);
        ((CItemInstVt2CInt*)(void*)*(u32*)(p + 0x44))->_v2C((void*)*(u32*)(p + 0x4c), 0);
        ((CItemInstVt2CInt*)(void*)*(u32*)(p + 0x44))->_v2C((void*)*(u32*)(p + 0x50), 1);
        p[0x61] = 1;
        func_801D0328(self);
        func_801D216C((void*)(p + 0x70), 1);
        func_801D216C((void*)(p + 0x88), 1);
        func_801D0950(self);
        if (p[0x542]) {
            *(u32*)(p + 0x58) = 0x12;
            {
                u32 msg = (u32)func_80136190((void*)&lbl_eu_8050566C[0x310], &lbl_eu_8050566C[0x158], 0x27);
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
void func_80137444(nw4r::lyt::AnimTransform*, float);

void CheckState4_Animate(char* self) {
    if (*(unsigned char*)((char*)self + 0x527) != 4) {
        return;
    }
    func_80137444(*(nw4r::lyt::AnimTransform**)((char*)self + 0x50), lbl_eu_80667F78);
}


// Animate grid panels.
void func_801CE2F8(void* self) {
    u8* p = (u8*)self;
    if (!func_80137510((nw4r::lyt::AnimTransform*)*(u32*)(p + 0x4c), lbl_eu_80667F78)) return;
    ((CItemPaneAnimVt*)*(u32*)(p + 0x44))->_v2C((void*)*(u32*)(p + 0x4c), 0);
    ((CItemPaneAnimVt*)*(u32*)(p + 0x44))->_v2C((void*)*(u32*)(p + 0x50), 0);
    ((CItemPaneAnimVt*)*(u32*)(p + 0x44))->_v2C((void*)*(u32*)(p + 0x48), 1);
    *(u32*)(p + 0x58) = 5;
}

void func_801CE390(void* self) {
    float f = lbl_eu_80667F78;
    u8* p = (u8*)self;
    u32 obj = *(u32*)(p + 0x48);
    if (func_80137510((nw4r::lyt::AnimTransform*)obj, f)) {
        p[0x61] = 1;
        *(u32*)(p + 0x58) = 0;
        func_801D216C(p + 0x70, 0);
    }
}
#pragma optimize_for_size on
extern "C" void func_801CE3E8(void* self) {
    u8* p = (u8*)self;
    if (func_801EB028((void*)(p + 0x3e4))) {
        *(u32*)(p + 0x58) = 3;
        p[0x528] = 0;
        func_801D216C((void*)(p + 0x70), 1);
        func_801D0950(self);
        if (p[0x541]) {
            p[0x541] = 0;
            *(u32*)(p + 0x58) = 0x12;
            {
                u32 msg = (u32)func_80136190((void*)&lbl_eu_8050566C[0x310], &lbl_eu_8050566C[0x158], 0x10);
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
void func_801CE4B4(void* self) {
    u8* p = (u8*)self;
    if (!func_80208360(p + 0x418)) return;
    *(u32*)(p + 0x58) = 7;
    u8 temp[16];
    func_80208760(temp, p + 0x418);
    ((CItemBoxObjA0Vt*)(p + 0xa0))->_v10(temp);
    func_801D216C(p + 0xa0, 1);
}


void func_801CE524(void* self) {
    u8* p = (u8*)self;
    if (!func_80208360(p + 0x418)) return;
    *(u32*)(p + 0x58) = 3;
    u32 result = func_802087B8(p + 0x418);
    if ((result & 0xFF) != 2) {
        if (p[0x541]) {
            p[0x541] = 0;
            *(u32*)(p + 0x58) = 0x12;
            u32 msg = (u32)func_80136190((void*)&lbl_eu_8050566C[0x310], &lbl_eu_8050566C[0x158], 0x10);
            func_8022B90C(p + 0x4e8, 0);
            func_8022B9B4(p + 0x4e8, msg, 0);
            func_8022BFC8(p + 0x4e8, 1);
            func_8022B8B8(p + 0x4e8);
            func_801D216C(p + 0x70, 0);
        } else {
            func_801D216C(p + 0x70, 1);
        }
        p[0x528] = 0;
        return;
    }
    // result == 2 (confirmed)
    u8 val = p[0x62 + (s8)p[0x6f]];
    if (val == 0xa) {
        u8* sub = p + 0x54c;
        u8 entry = (u8)((s8)p[0x525] * 10 + (s8)p[0x524]);
        u32 kind = func_801C62AC((CItemBoxGridFull*)sub, entry);
        func_8022E204(p + 0x468, kind & 0xFFFF);
        func_8022DB7C(p + 0x468);
        *(u32*)(p + 0x58) = 0xf;
        p[0x528] = 1;
        return;
    }
    if (val == 0xd) {
        u8* sub = p + 0x54c;
        u8 entry = (u8)((s8)p[0x525] * 10 + (s8)p[0x524]);
        u32 kind = func_801C62AC((CItemBoxGridFull*)sub, entry);
        u32 kindId = kind & 0xFFFF;
        func_801392E4(kindId);
        u32 kindShort = func_80139358(kindId);
        
        u32 msgRes = lbl_eu_80664110;
        u32 catVal = func_801361E8(msgRes, (const char*)&lbl_eu_8050566C[0x109], kindShort & 0xFFFF);
        u32 itemNameIdx = func_801361E8(msgRes, (const char*)&lbl_eu_8050566C[0x115], kindShort & 0xFFFF);
        u32 paramVal = func_801361E8(msgRes, (const char*)&lbl_eu_8050566C[0x11d], kindShort & 0xFFFF);
        
        u32 dialogType = func_8013600C((void*)&lbl_eu_8050566C[0x126], &lbl_eu_8050566C[0x12e], paramVal & 0xFF);
        u32 confirmMsg = (u32)func_80136190((void*)&lbl_eu_8050566C[0x126], &lbl_eu_8050566C[0x158], paramVal & 0xFF);
        
        u32 subtitleVisible = 0;
        u32 showFull = 0;
        
        if (func_8008235C__Q22cf13CfGameManagerFv(itemNameIdx & 0xFF)) {
            u32 obj = (u32)func_8009EC9C(itemNameIdx & 0xFF);
            u32 someVal = func_800A32BC();
            s8 cat = (s8)(catVal & 0xFF);
            u8 itemCount = (u8)dialogType;
            
            u32 base = (someVal & 0xFF) * 0x49 + obj;
            u32 idx = (itemCount & 0xFF) * 2;
            void* dataPtr = (void*)(base + idx);
            
            if (cat == 1) {
                u8 flag = *(u8*)((u8*)dataPtr + 0xe8);
                if (flag) {
                    confirmMsg = (u32)func_80136190((void*)&lbl_eu_8050566C[0x326], &lbl_eu_8050566C[0x158], 0x40);
                } else {
                    confirmMsg = (u32)func_80136190((void*)&lbl_eu_8050566C[0x326], &lbl_eu_8050566C[0x158], 0x39);
                    subtitleVisible = 1;
                    showFull = 1;
                    func_800A18A4((void*)obj, paramVal & 0xFF);
                }
            } else if (cat == 2) {
                u8 flag = *(u8*)((u8*)dataPtr + 0xe8);
                if (flag) {
                    u8 b = *(u8*)((u8*)dataPtr + 0xe9);
                    if (!(b & 0x80)) {
                        *(u8*)((u8*)dataPtr + 0xe9) = b | 0x80;
                        confirmMsg = (u32)func_80136190((void*)&lbl_eu_8050566C[0x326], &lbl_eu_8050566C[0x158], 0x3a);
                        subtitleVisible = 1;
                        showFull = 1;
                        func_8013B428__FUl(0x46);
                    } else {
                        confirmMsg = (u32)func_80136190((void*)&lbl_eu_8050566C[0x326], &lbl_eu_8050566C[0x158], 0x40);
                    }
                } else {
                    confirmMsg = (u32)func_80136190((void*)&lbl_eu_8050566C[0x326], &lbl_eu_8050566C[0x158], 0x41);
                }
            } else if (cat == 3) {
                u8 flag = *(u8*)((u8*)dataPtr + 0xe8);
                if (flag) {
                    u8 b = *(u8*)((u8*)dataPtr + 0xe9);
                    if (!(b & 0x40)) {
                        *(u8*)((u8*)dataPtr + 0xe9) = b | 0xc0;
                        confirmMsg = (u32)func_80136190((void*)&lbl_eu_8050566C[0x326], &lbl_eu_8050566C[0x158], 0x3b);
                        subtitleVisible = 1;
                        showFull = 1;
                        func_8013B428__FUl(0x46);
                        func_8013B428__FUl(0x47);
                    } else {
                        confirmMsg = (u32)func_80136190((void*)&lbl_eu_8050566C[0x326], &lbl_eu_8050566C[0x158], 0x40);
                    }
                } else {
                    confirmMsg = (u32)func_80136190((void*)&lbl_eu_8050566C[0x326], &lbl_eu_8050566C[0x158], 0x41);
                }
            }
            
            if (showFull) {
                void* itemObj = func_801C631C((CItemBoxGridFull*)sub, entry);
                s32 count = func_801C6388((CItemBoxGridFull*)sub, entry);
                func_801D11B8(self, itemObj, (s8)count - 1);
                func_801CFF28(self);
            }
        } else {
            confirmMsg = (u32)func_80136190((void*)&lbl_eu_8050566C[0x326], &lbl_eu_8050566C[0x158], 0x42);
        }
        
        func_8022B90C(p + 0x4e8, subtitleVisible);
        func_8022B9B4(p + 0x4e8, confirmMsg, 0);
        func_8022BFC8(p + 0x4e8, 1);
        func_8022B8B8(p + 0x4e8);
        *(u32*)(p + 0x58) = 0x12;
        p[0x528] = 0;
        return;
    }
    // val != 0xa and val != 0xd
    func_801D216C(p + 0x70, 1);
    p[0x528] = 0;
}

// Handle sub-object activation with copy.
void func_801CE974(void* self) {
    u8* p = (u8*)self;
    if (!((CExchangeWin*)(p + 0x440))->getField27()) return;
    *(u32*)(p + 0x58) = 0xd;
    func_801D216C(p + 0xa0, 1);
    u8 temp[16];
    u8 val529 = p[0x529];
    func_8022D0F8(temp, p + 0x440, val529);
    reinterpret_cast<CItemBoxObjA0Vt*>(p + 0xa0)->_v10(temp);
}

// Check if sub-obj is active; set state to 3 and clear flag.
void func_801CE9E8(void* self) {
    u8* p = (u8*)self;
    if (((CExchangeWin*)(p + 0x440))->getField27()) {
        *(u32*)(p + 0x58) = 3;
        p[0x528] = 0;
    }
}

// Handle sub-object activation (variant).
void func_801CEA30(void* self) {
    u8* p = (u8*)self;
    if (!func_8022DB74(p + 0x468)) return;
    *(u32*)(p + 0x58) = 0x10;
    u8 temp[16];
    func_8022E498(temp, p + 0x468);
    ((CItemBoxObjA0Vt*)(p + 0xd0))->_v10(temp);
    func_801D216C(p + 0xd0, 1);
}

// Check if sub-obj is active; set state to 3 and clear flag.
void func_801CEAA0(void* self) {
    u8* p = (u8*)self;
    if (func_8022DB74(p + 0x468)) {
        *(u32*)(p + 0x58) = 3;
        p[0x528] = 0;
    }
}

// Check system window; if active, set state and call helpers.
void func_801CEAE8(void* self) {
    u8* p = (u8*)self;
    if (CSysWin_isActive(p + 0x4ac)) {
        *(u32*)(p + 0x58) = 0x16;
        func_801D216C(p + 0xa0, 1);
        func_801D0950(self);
    }
}

// Handle system window state change.
void func_801CEB3C(void* self) {
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
        func_801D0950(self);
    }
}

// Handle system window activation.
void func_801CEBF0(void* self) {
    u8* p = (u8*)self;
    if (!CSysWin_isActive(p + 0x4e8)) return;
    *(u32*)(p + 0x58) = 3;
    if (!p[0x542] && !p[0x544]) {
        func_801D216C(p + 0x70, 1);
    }
    if (!p[0x544]) {
        func_801D0950(self);
    }
    if (p[0x544]) {
        func_801D216C(p + 0xa0, 1);
    }
}

// Check conditions; set flags.
void func_801CEC80(void* self) {
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
// override when bt is set, and finally the shared sprintf + func_80137E7C
// show.
void func_801CECD0(CItemBoxGridFull* self, u32 kind, void* item, u16 idx, u32 bt) {
    u8* p = (u8*)self;
    void* obj = item ? item : 0;
    u32 msgId = 0;
    if (obj) {
        u32 type = (*(u32*)obj >> 16) & 0xF;
        if (type == 3) {
            msgId = 0;
            void* inst = CItem_initItemImplInstances(obj);
            u32 st = ((CItemInstVt54*)inst)->_v54(obj);
            u8 c = (u8)func_801361E8(lbl_eu_806640D8, (const char*)&lbl_eu_8050566C[0x331], (u16)st);
            switch (c) {
            case 4: msgId = (u32)func_80138F78(0x144); break;
            case 5: msgId = (u32)func_80138F78(0x145); break;
            case 6: msgId = (u32)func_80138F78(0x146); break;
            case 7: msgId = (u32)func_80138F78(0x147); break;
            case 8: msgId = (u32)func_80138F78(0x148); break;
            case 9: msgId = (u32)func_80138F78(0x149); break;
            }
            if (msgId) {
                msgId = (u32)((CItemBoxAccVt0C*)*(void**)(p + 0x40))->_v0C(
                    0x74696d67, (char*)msgId, 0);
            } else {
                msgId = (u32)((CItemBoxAccVt0C*)*(void**)(p + 0x3c))->_v0C(
                    0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
            }
        } else if (type == 9) {
            void* inst = CItem_initItemImplInstances(obj);
            u16 hasArts = (u16)((CItemInstVt80*)inst)->_v80(obj);
            if (hasArts == 0) {
                if (func_801C6E90(obj)) {
                    msgId = (u32)func_80138F78(0x155);
                    msgId = (u32)((CItemBoxAccVt0C*)*(void**)(p + 0x40))->_v0C(
                        0x74696d67, (char*)msgId, 0);
                } else {
                    u32 sub = 0;
                    int v = (((u8*)obj)[7] >> 2) & 0x3F;
                    switch (v) {
                    case 4: sub = (u32)func_80138F78(0x144); break;
                    case 5: sub = (u32)func_80138F78(0x145); break;
                    case 6: sub = (u32)func_80138F78(0x146); break;
                    case 7: sub = (u32)func_80138F78(0x147); break;
                    case 8: sub = (u32)func_80138F78(0x148); break;
                    case 9: sub = (u32)func_80138F78(0x149); break;
                    }
                    if (sub) {
                        msgId = (u32)((CItemBoxAccVt0C*)*(void**)(p + 0x40))->_v0C(
                            0x74696d67, (char*)sub, 0);
                    } else {
                        msgId = (u32)((CItemBoxAccVt0C*)*(void**)(p + 0x3c))->_v0C(
                            0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
                    }
                }
            } else {
                // origin arts - same byte7>>2 subtype mapping
                u32 sub2 = 0;
                int v = (((u8*)obj)[7] >> 2) & 0x3F;
                switch (v) {
                case 4: sub2 = (u32)func_80138F78(0x144); break;
                case 5: sub2 = (u32)func_80138F78(0x145); break;
                case 6: sub2 = (u32)func_80138F78(0x146); break;
                case 7: sub2 = (u32)func_80138F78(0x147); break;
                case 8: sub2 = (u32)func_80138F78(0x148); break;
                case 9: sub2 = (u32)func_80138F78(0x149); break;
                }
                if (sub2) {
                    msgId = (u32)((CItemBoxAccVt0C*)*(void**)(p + 0x40))->_v0C(
                        0x74696d67, (char*)sub2, 0);
                } else {
                    msgId = (u32)((CItemBoxAccVt0C*)*(void**)(p + 0x3c))->_v0C(
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
            u16 v = func_80136254((const void*)lbl_eu_806640EC, &lbl_eu_8050566C[0x34d], kind);
            msgId = (u32)func_80138F78(v);
            msgId = (u32)((CItemBoxAccVt0C*)*(void**)(p + 0x40))->_v0C(
                0x74696d67, (char*)msgId, 0);
            if (!msgId) {
                msgId = (u32)((CItemBoxAccVt0C*)*(void**)(p + 0x3c))->_v0C(
                    0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
            }
        } else {
            msgId = (u32)((CItemBoxAccVt0C*)*(void**)(p + 0x3c))->_v0C(
                0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
        }
    }
    if (bt) {
        u8* q = p + (s8)p[0x6f];
        u8 cat = q[0x62];
        if (cat == 3) {
            msgId = (u32)((CItemBoxAccVt0C*)*(void**)(p + 0x40))->_v0C(
                0x74696d67, (char*)&lbl_eu_8050566C[0x357], 0);
        } else {
            msgId = (u32)((CItemBoxAccVt0C*)*(void**)(p + 0x40))->_v0C(
                0x74696d67, (char*)&lbl_eu_8050566C[0x36d], 0);
        }
    }
    if (msgId) {
        char buf[32];
        sprintf(buf, (const char*)&lbl_eu_8050566C[0x383], idx + 1);
        func_80137E7C(*(void**)(p + 0x44), buf, (void*)msgId);
    }
}

// Resolve the cell's detail-message id and show it: per-item-type message
// lookups (equipment 0x197..0x192 by the +0x08 slot value, arts 0x191..0x18d
// for skill items, 0x19c..0x198 for origin arts), each falling back to the
// +0x3C accessor's default label when the lookup yields nothing, then a kind
// name-table fallback, and finally the shared sprintf + func_80137E7C show.
void func_801CF240(CItemBoxGridFull* self, u32 kind, void* item, u16 idx) {
    u8* p = (u8*)self;
    void* obj = item ? item : 0;
    u32 msgId = 0;
    if (obj) {
        u32 type = (*(u32*)obj >> 16) & 0xF;
        if (type == 3) {
            msgId = 0;
            void* inst = CItem_initItemImplInstances(obj);
            u16 v = (u16)((CItemInstVt08*)inst)->_v08(obj);
            switch (v) {
            case 1: msgId = (u32)func_80138F78(0x197); break;
            case 2: msgId = (u32)func_80138F78(0x196); break;
            case 3: msgId = (u32)func_80138F78(0x195); break;
            case 4: msgId = (u32)func_80138F78(0x194); break;
            case 5: msgId = (u32)func_80138F78(0x193); break;
            case 6: msgId = (u32)func_80138F78(0x192); break;
            }
            if (msgId) {
                msgId = (u32)((CItemBoxAccVt0C*)*(void**)(p + 0x40))->_v0C(
                    0x74696d67, (char*)msgId, 0);
            } else {
                msgId = (u32)((CItemBoxAccVt0C*)*(void**)(p + 0x3c))->_v0C(
                    0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
            }
        } else if (type == 9) {
            void* inst = CItem_initItemImplInstances(obj);
            u16 hasArts = (u16)((CItemInstVt80*)inst)->_v80(obj);
            if (hasArts == 0) {
                if (func_801C6E90(obj)) {
                    // skill item - use the 0x191..0x18d mapping below
                } else {
                    u32 w = *(u32*)obj;
                    int isArts = 0;
                    u32 t = (w >> 16) & 0xF;
                    if (t == 9 && (u8)(((u8*)obj)[7] & 3) == 1) isArts = 1;
                    if (!isArts && t != 9) goto done_item;
                }
                msgId = 0;
                void* inst2 = CItem_initItemImplInstances(obj);
                u16 v = (u16)((CItemInstVt08*)inst2)->_v08(obj);
                switch (v) {
                case 1: msgId = (u32)func_80138F78(0x191); break;
                case 2: msgId = (u32)func_80138F78(0x190); break;
                case 3: msgId = (u32)func_80138F78(0x18f); break;
                case 4: msgId = (u32)func_80138F78(0x18e); break;
                case 5: msgId = (u32)func_80138F78(0x18d); break;
                }
                if (msgId) {
                    msgId = (u32)((CItemBoxAccVt0C*)*(void**)(p + 0x40))->_v0C(
                        0x74696d67, (char*)msgId, 0);
                } else {
                    msgId = (u32)((CItemBoxAccVt0C*)*(void**)(p + 0x3c))->_v0C(
                        0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
                }
            } else {
                // origin arts - use the 0x19c..0x198 mapping
                msgId = 0;
                void* inst3 = CItem_initItemImplInstances(obj);
                u16 v = (u16)((CItemInstVt08*)inst3)->_v08(obj);
                switch (v) {
                case 1: msgId = (u32)func_80138F78(0x19c); break;
                case 2: msgId = (u32)func_80138F78(0x19b); break;
                case 3: msgId = (u32)func_80138F78(0x19a); break;
                case 4: msgId = (u32)func_80138F78(0x199); break;
                case 5: msgId = (u32)func_80138F78(0x198); break;
                }
                if (msgId) {
                    msgId = (u32)((CItemBoxAccVt0C*)*(void**)(p + 0x40))->_v0C(
                        0x74696d67, (char*)msgId, 0);
                } else {
                    msgId = (u32)((CItemBoxAccVt0C*)*(void**)(p + 0x3c))->_v0C(
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
            u16 v = func_80136254((const void*)lbl_eu_806640EC, &lbl_eu_8050566C[0x391], kind);
            msgId = (u32)func_80138F78(v);
            msgId = (u32)((CItemBoxAccVt0C*)*(void**)(p + 0x40))->_v0C(
                0x74696d67, (char*)msgId, 0);
            if (!msgId) {
                msgId = (u32)((CItemBoxAccVt0C*)*(void**)(p + 0x3c))->_v0C(
                    0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
            }
        } else {
            msgId = (u32)((CItemBoxAccVt0C*)*(void**)(p + 0x3c))->_v0C(
                0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
        }
    }
    if (msgId) {
        char buf[32];
        sprintf(buf, (const char*)&lbl_eu_8050566C[0x396], idx + 1);
        func_80137E7C(*(void**)(p + 0x44), buf, (void*)msgId);
    }
}

void func_801CF71C(void* self, int r4, void* r5, int r6) {
    // Asm shows 4 params: (self, int r4, void* r5, int r6)
    // Called with r6=idx from func_801CFFEC
    // Body omitted for now.
}

void func_801CF900(void* self, u32 r4, void* r5, void* r6, u32 r7) {
    u8* p = (u8*)self;
    char buf[0x20];
    sprintf(buf, (const char*)&lbl_eu_8050566C[0x3b0], r7 + 1);

    void* accessor = *(void**)(p + 0x3c);
    void** vtbl = *(void***)accessor;
    u32 tag = 0x74696D67; /* "img" */
    void* pane = ((void*(*)(void*, u32, const char*, int))vtbl[3])(accessor, tag, (const char*)&lbl_eu_8050566C[0x33a], 0);

    if (r4 == 1) {
        accessor = *(void**)(p + 0x3c);
        vtbl = *(void***)accessor;
        pane = ((void*(*)(void*, u32, const char*, int))vtbl[3])(accessor, tag, &lbl_eu_8050566C[0x3bb], 0);
    } else if (r4 == 2) {
        accessor = *(void**)(p + 0x3c);
        vtbl = *(void***)accessor;
        pane = ((void*(*)(void*, u32, const char*, int))vtbl[3])(accessor, tag, &lbl_eu_8050566C[0x3cf], 0);
    }

    if (r6) {
        accessor = *(void**)(p + 0x3c);
        vtbl = *(void***)accessor;
        pane = ((void*(*)(void*, u32, const char*, int))vtbl[3])(accessor, tag, &lbl_eu_8050566C[0x3e3], 0);
    }

    if (r5) {
        accessor = *(void**)(p + 0x3c);
        vtbl = *(void***)accessor;
        pane = ((void*(*)(void*, u32, const char*, int))vtbl[3])(accessor, tag, &lbl_eu_8050566C[0x3f7], 0);
    }

    if (pane) {
        func_80137E7C(*(void**)(p + 0x44), (void*)buf, 0);
    }
}

// Format the current tab's header text: build the category label via the
// switch-driven sprintf (jump table), resolve the pane through the +0x40
// accessor (falling back to +0x3C's default label), then write the
// "<label> <idx+1>" string into the layout. optimize_for_size gives the
// retail stmw r28 prologue (4 saved regs).
#pragma optimize_for_size on
void func_801CFA58(void* self, int r4, int r5) {
    u8* p = (u8*)self;
    char buf[32];
    int result;
    if (r4 != 0) {
        switch (r4) {
        case 1: sprintf(buf, (const char*)&lbl_eu_8050566C[0x40b]); break;
        case 2: sprintf(buf, (const char*)&lbl_eu_8050566C[0x420]); break;
        case 3: sprintf(buf, (const char*)&lbl_eu_8050566C[0x435]); break;
        case 4: sprintf(buf, (const char*)&lbl_eu_8050566C[0x44a]); break;
        case 5: sprintf(buf, (const char*)&lbl_eu_8050566C[0x45f]); break;
        case 6: sprintf(buf, (const char*)&lbl_eu_8050566C[0x474]); break;
        case 7: sprintf(buf, (const char*)&lbl_eu_8050566C[0x489]); break;
        case 8: sprintf(buf, (const char*)&lbl_eu_8050566C[0x49e]); break;
        case 9: sprintf(buf, (const char*)&lbl_eu_8050566C[0x4b3]); break;
        case 10: sprintf(buf, (const char*)&lbl_eu_8050566C[0x4c8]); break;
        case 11: sprintf(buf, (const char*)&lbl_eu_8050566C[0x4dd]); break;
        case 12: sprintf(buf, (const char*)&lbl_eu_8050566C[0x4f2]); break;
        case 13: sprintf(buf, (const char*)&lbl_eu_8050566C[0x507]); break;
        }
        result = (int)((CItemBoxAccVt0C*)*(void**)(p + 0x40))->_v0C(0x74696d67, buf, 0);
        if (!result) {
            result = (int)((CItemBoxAccVt0C*)*(void**)(p + 0x3c))->_v0C(0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
        }
    } else {
        result = (int)((CItemBoxAccVt0C*)*(void**)(p + 0x3c))->_v0C(0x74696d67, (char*)&lbl_eu_8050566C[0x33a], 0);
    }
    if (result) {
        char tmp[32];
        sprintf(tmp, (const char*)&lbl_eu_8050566C[0x507], r5 + 1);
        func_80137E7C(*(void**)(p + 0x44), tmp, (void*)result);
    }
}
#pragma optimize_for_size off

// Format text and set on layout pane.
#pragma optimize_for_size on
void func_801CFCBC(void* self, u32 val, u32 idx) {
    u8* p = (u8*)self;
    char buf[32];
    sprintf(buf, (const char*)&lbl_eu_8050566C[0x513], idx + 1);
    u32 obj = *(u32*)(p + 0x44);
    u32 sub = *(u32*)(obj + 0x10);
    void* ret = ((CItemPaneObjVt*)sub)->_v3C(buf, 1);
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
                func_80139A18(*(void**)(p + 0x44), buf1, &lbl_eu_806644A8, &lbl_eu_806644B0);
                func_80139A18(*(void**)(p + 0x44), buf2, &lbl_eu_806644A8, &lbl_eu_806644B0);
            } else {
                func_80139A18(*(void**)(p + 0x44), buf1, &lbl_eu_80664498, &lbl_eu_806644A0);
                func_80139A18(*(void**)(p + 0x44), buf2, &lbl_eu_80664498, &lbl_eu_806644A0);
            }
        }
        void* pane = *(void**)(*(u32*)(p + 0x44) + 0x10);
        void* t1 = ((CItemPaneObjVt*)pane)->_v3C(buf1, 1);
        func_80124270(t1, isCurTab);
        pane = *(void**)(*(u32*)(p + 0x44) + 0x10);
        void* t2 = ((CItemPaneObjVt*)pane)->_v3C(buf2, 1);
        func_80124270(t2, isOtherTab);
        func_801CFA58(self, cat, (u8)i);
    }
    func_801D05D4(self, 0);
    if (p[0x527] == 2) {
        CItemBoxGridFull* subGrid = (CItemBoxGridFull*)(p + 0x54c);
        p[0x542] = 0;
        func_801C56D8(subGrid, p[0x62], 0, *(u16*)(p + 0x52e), *(u16*)(p + 0x52a));
        u16 count1 = subGrid->field_2800;
        func_801C56D8(subGrid, p[0x63], 0, *(u16*)(p + 0x52e), *(u16*)(p + 0x52a));
        if (!count1 && !subGrid->field_2800) {
            p[0x542] = 1;
        }
    }
    func_801CFF28(self);
}

#pragma push
#pragma optimize_for_size on
void func_801CFF28(void* self) {
    u8* p = (u8*)self;
    u8* sub = p + 0x54c;
    func_801C56D8((CItemBoxGridFull*)sub, (p + 0x62)[(s8)p[0x6f]], 0,
                  *(u16*)(p + 0x52e), *(u16*)(p + 0x52a));
    if (p[0x527] == 4) {
        func_801C7730((CItemBoxGridFull*)sub);
    }
    func_80136910(reinterpret_cast<nw4r::lyt::Layout*>(*(u32*)(p + 0x44)),
                  const_cast<char*>(&lbl_eu_8050566C[0x53e]),
                  (u8)func_801C5E5C((void*)sub));
    func_801CFFEC(self);
    func_801D0328(self);
    func_801D0BD8(self);
    if (getItemBoxState__FP12CItemBoxInfo(p + 0x1d8) != 0) {
        func_801D4260(p + 0x1d8, (p + 0x62)[(s8)p[0x6f]]);
    }
}
#pragma pop

void func_801CFFEC(void* self) {
    u8* p = (u8*)self;
    const char* strbase = lbl_eu_8050566C;
    CItemBoxGridFull* sub = (CItemBoxGridFull*)(p + 0x54c);
    u32 rows = func_801C5E5C(sub);
    u32 r = rows & 0xFF;
    if (r > 1) {
        u32* layout = *(u32**)(p + 0x44);
        u32* pane = *(u32**)(layout + 0x10);
        void** vtbl = *(void***)pane;
        void* ret = ((void*(*)(void*, const char*, int))vtbl[0x3c/4])(pane, strbase + 0x547, 1);
        func_80124270(ret, 1);
        int i;
        for (i = 0; i < 10; i++) {
            char buf[64];
            sprintf(buf, strbase + 0x54e, i + 1);
            u32* pane2 = *(u32**)(layout + 0x10);
            void** vtbl2 = *(void***)pane2;
            void* ret2 = ((void*(*)(void*, char*, int))vtbl2[0x3c/4])(pane2, buf, 1);
            u32 hidden = (r - (u32)(u8)i) >> 31;
            func_80124270(ret2, hidden);
        }
        u32* layout2 = *(u32**)(p + 0x44);
        func_80136910(reinterpret_cast<nw4r::lyt::Layout*>(layout2), const_cast<char*>(strbase + 0x55d), (u8)(sub->field_2804 + 1));
        u32* pane3 = *(u32**)(layout2 + 0x10);
        void** vtbl3 = *(void***)pane3;
        void* ret3 = ((void*(*)(void*, const char*, int))vtbl3[0x3c/4])(pane3, strbase + 0x566, 1);
        float v[3];
        v[0] = *(float*)(p + 0x534);
        v[1] = *(float*)(p + 0x538);
        v[2] = *(float*)(p + 0x53c);
        float step = lbl_eu_80667F84;
        float fRow = (float)(r - 1);
        float fIdx = (float)(u8)(sub->field_2804);
        float fNum = (float)(10 - r);
        float offset = step * (fIdx + fNum);
        v[0] += offset;
        v[1] += offset;
        v[2] += offset;
        copyVEC3((float*)((u8*)ret3 + 0x2c), v);
    } else {
        u32* layout = *(u32**)(p + 0x44);
        u32* pane = *(u32**)(layout + 0x10);
        void** vtbl = *(void***)pane;
        void* ret = ((void*(*)(void*, const char*, int))vtbl[0x3c/4])(pane, strbase + 0x547, 1);
        func_80124270(ret, 0);
    }
    int i;
    for (i = 0; i < 3; i++) {
        char buf[64];
        sprintf(buf, strbase + 0x574, i + 1);
        u32* layout = *(u32**)(p + 0x44);
        u32* pane = *(u32**)(layout + 0x10);
        void** vtbl = *(void***)pane;
        void* ret = ((void*(*)(void*, char*, int))vtbl[0x3c/4])(pane, buf, 1);
        if (ret) {
            u8 max = LookupIndexedByte((char*)sub);
            u32 hidden = ((u32)(u8)i - (u32)max) >> 31;
            func_80124270(ret, hidden);
        }
    }
    u32 idx;
    for (idx = 0; idx < 0x1e; idx++) {
        u32 kind = func_801C62AC(sub, (u16)idx);
        void* obj = func_801C631C(sub, (u16)idx);
        u8 bt = func_801C6708(sub, (u16)idx);
        func_801CECD0((CItemBoxGridFull*)self, kind & 0xFFFF, obj, (u16)idx, (u32)bt);
        func_801CF240((CItemBoxGridFull*)self, kind & 0xFFFF, obj, (u16)idx);
        s32 chk = func_801C6388(sub, (u16)idx);
        func_801CF71C(self, (s8)chk, obj, idx);
        u8 b5 = func_801C6618(sub, (u16)idx);
        u8 b4 = func_801C65A0(sub, (u16)idx);
        u8 b2 = func_801C6528(sub, (u16)idx);
        func_801CF900(self, (u32)b2, (void*)(u32)b4, (void*)(u32)b5, (u32)idx);
        u8 b7 = func_801C673C(sub, (u16)idx);
        func_801CFCBC(self, b7, (u16)idx);
    }
}

void func_801D0328(void* self) {
    u8* p = (u8*)self;
    CItemBoxGridFull* sub = (CItemBoxGridFull*)(p + 0x54c);
    s8 row = (s8)p[0x525];
    u8 col = p[0x524];
    u16 entry = (u16)(u8)(col + (u8)row * 10);
    s8 idx = (s8)entry;
    if (row == -1) {
        u32 val = func_801D1220(self);
        u32* layout = *(u32**)(p + 0x44);
        func_80136B4C((nw4r::lyt::Layout*)layout, (char*)&lbl_eu_8050566C[0x581], (char*)val, 0);
        func_80136B4C((nw4r::lyt::Layout*)layout, (char*)&lbl_eu_8050566C[0x58a], (char*)&lbl_eu_8050566C[0x3af], 0);
    } else if (row == -2) {
        u32* layout = *(u32**)(p + 0x44);
        func_80136B4C((nw4r::lyt::Layout*)layout, (char*)&lbl_eu_8050566C[0x581], (char*)&lbl_eu_8050566C[0x3af], 0);
        func_80136B4C((nw4r::lyt::Layout*)layout, (char*)&lbl_eu_8050566C[0x58a], (char*)&lbl_eu_8050566C[0x3af], 0);
    } else {
        u32 val = func_801C6938(sub, (u16)idx);
        u32* layout = *(u32**)(p + 0x44);
        func_80136B4C((nw4r::lyt::Layout*)layout, (char*)&lbl_eu_8050566C[0x581], (char*)val, 0);
        char* str = func_801C6A44(sub, (u16)idx);
        func_80136B4C((nw4r::lyt::Layout*)layout, (char*)&lbl_eu_8050566C[0x58a], (char*)str, *(u32*)(p + 0x5c));
    }
    u32 qty;
    if (p[0x527] == 2) {
        void* obj = func_801C631C(sub, (u16)idx);
        u32 kind = func_801C62AC(sub, (u16)idx);
        s32 chk = func_801C6388(sub, (u16)idx);
        if ((s8)p[0x525] < 0) qty = 0;
        else qty = func_801C618C(sub, kind & 0xFFFF, obj, *(u16*)(p + 0x52a));
    } else {
        if ((s8)p[0x525] < 0) qty = 0;
        else qty = func_801C5FC0(sub, (u16)idx);
    }
    char buf[64];
    sprintf(buf, (const char*)&lbl_eu_8050566C[0x596], qty, (char*)func_80136190((void*)&lbl_eu_8050566C[0x14f], &lbl_eu_8050566C[0x158], 3));
    u32* layout = *(u32**)(p + 0x44);
    func_80136A1C((nw4r::lyt::Layout*)layout, (char*)&lbl_eu_8050566C[0x59b], buf, 0);
    if (getItemBoxState__FP12CItemBoxInfo(p + 0x1d8)) {
        void* obj2;
        u32 kind2;
        if ((s8)p[0x525] < 0) { obj2 = 0; kind2 = 0; }
        else {
            obj2 = func_801C631C(sub, (u16)idx);
            kind2 = func_801C62AC(sub, (u16)idx);
        }
        func_801D47D4((void*)(p + 0x1d8), kind2 & 0xFFFF, (u32)obj2, 1);
        u32 val2;
        if ((s8)p[0x525] < 0) val2 = 0;
        else val2 = func_801C6938(sub, (u16)idx);
        func_801D4AE0(p + 0x1d8, 1, (void*)val2);
        func_801D4BDC(p + 0x1d8, func_801C67F8(sub), func_801C6840(sub));
    }
}

extern "C" void func_801D05D4(void* self, int val) {
    u8* p = (u8*)self;
    u8 buf[3];
    u8 b = p[0x6f];
    s8 oldIdx;
    s8 newIdx;
    buf[0] = b;
    oldIdx = (s8)b;
    newIdx = oldIdx - 1;
    if (val != 0) newIdx = oldIdx + 1;
    buf[1] = (u8)newIdx;
    if (newIdx < 0) {
        buf[1] = p[0x6e] - 1;
    } else if ((u8)newIdx >= (u8)p[0x6e]) {
        buf[1] = 0;
    }
    {
        const char* strbase = lbl_eu_8050566C;
        u8 i;
        for (i = 0; i < 2; i++) {
            s8 idx = (s8)buf[i];
            char pane1[32];
            char pane2[32];
            int tabNum = idx + 1;
            sprintf(pane1, strbase + 0x520, tabNum);
            sprintf(pane2, strbase + 0x52f, tabNum);
            {
                u8 cat = p[idx + 0x62];
                int isCurTab = 0;
                int isOtherTab = 0;
                if (cat != 0) {
                    if (idx == (s8)p[0x6f]) {
                        isCurTab = 1;
                        isOtherTab = 0;
                    } else {
                        isCurTab = 0;
                        isOtherTab = 1;
                    }
                    {
                        u32 layout = *(u32*)(p + 0x44);
                        void* pane = *(void**)(layout + 0x10);
                        void** vtbl = *(void***)pane;
                        void* textObj = ((void*(*)(void*, char*, int))vtbl[0x3C / 4])(pane, pane1, 1);
                        func_80124270(textObj, isCurTab);
                    }
                    {
                        u32 layout = *(u32*)(p + 0x44);
                        void* pane = *(void**)(layout + 0x10);
                        void** vtbl = *(void***)pane;
                        void* textObj = ((void*(*)(void*, char*, int))vtbl[0x3C / 4])(pane, pane2, 1);
                        func_80124270(textObj, isOtherTab);
                    }
                }
            }
        }
    }
    {
        u8 cat = p[(s8)p[0x6f] + 0x62];
        u32 diff = cat - 4;
        u32 layout = *(u32*)(p + 0x44);
        if (diff <= 4 || cat == 2 || cat == 0xB) {
            func_80139A18((void*)layout, (void*)&lbl_eu_8050566C[0x5aa], (void*)&lbl_eu_806644A8, (void*)&lbl_eu_806644B0);
            func_80139A18((void*)layout, (void*)&lbl_eu_8050566C[0x5b6], (void*)&lbl_eu_806644A8, (void*)&lbl_eu_806644B0);
            func_80139A18((void*)layout, (void*)&lbl_eu_8050566C[0x5c2], (void*)&lbl_eu_806644A8, (void*)&lbl_eu_806644B0);
            func_80139A18((void*)layout, (void*)&lbl_eu_8050566C[0x5ce], (void*)&lbl_eu_806644A8, (void*)&lbl_eu_806644B0);
            func_80139A18((void*)layout, (void*)&lbl_eu_8050566C[0x5da], (void*)&lbl_eu_806644A8, (void*)&lbl_eu_806644B0);
            func_80139A18((void*)layout, (void*)&lbl_eu_8050566C[0x5e6], (void*)&lbl_eu_806644A8, (void*)&lbl_eu_806644B0);
            func_80139A18((void*)layout, (void*)&lbl_eu_8050566C[0x5f2], (void*)&lbl_eu_806644A8, (void*)&lbl_eu_806644B0);
            func_80139A18((void*)layout, (void*)&lbl_eu_8050566C[0x5fe], (void*)&lbl_eu_806644A8, (void*)&lbl_eu_806644B0);
            func_80139A18((void*)layout, (void*)&lbl_eu_8050566C[0x60a], (void*)&lbl_eu_806644A8, (void*)&lbl_eu_806644B0);
            func_80139A18((void*)layout, (void*)&lbl_eu_8050566C[0x616], (void*)&lbl_eu_806644A8, (void*)&lbl_eu_806644B0);
            func_80139A18((void*)layout, (void*)&lbl_eu_8050566C[0x621], (void*)&lbl_eu_806644A8, (void*)&lbl_eu_806644B0);
            func_80139A18((void*)layout, (void*)&lbl_eu_8050566C[0x62c], (void*)&lbl_eu_806644A8, (void*)&lbl_eu_806644B0);
        } else {
            func_80139A18((void*)layout, (void*)&lbl_eu_8050566C[0x5aa], (void*)&lbl_eu_80664498, (void*)&lbl_eu_806644A0);
            func_80139A18((void*)layout, (void*)&lbl_eu_8050566C[0x5b6], (void*)&lbl_eu_80664498, (void*)&lbl_eu_806644A0);
            func_80139A18((void*)layout, (void*)&lbl_eu_8050566C[0x5c2], (void*)&lbl_eu_80664498, (void*)&lbl_eu_806644A0);
            func_80139A18((void*)layout, (void*)&lbl_eu_8050566C[0x5ce], (void*)&lbl_eu_80664498, (void*)&lbl_eu_806644A0);
            func_80139A18((void*)layout, (void*)&lbl_eu_8050566C[0x5da], (void*)&lbl_eu_80664498, (void*)&lbl_eu_806644A0);
            func_80139A18((void*)layout, (void*)&lbl_eu_8050566C[0x5e6], (void*)&lbl_eu_80664498, (void*)&lbl_eu_806644A0);
            func_80139A18((void*)layout, (void*)&lbl_eu_8050566C[0x5f2], (void*)&lbl_eu_80664498, (void*)&lbl_eu_806644A0);
            func_80139A18((void*)layout, (void*)&lbl_eu_8050566C[0x5fe], (void*)&lbl_eu_80664498, (void*)&lbl_eu_806644A0);
            func_80139A18((void*)layout, (void*)&lbl_eu_8050566C[0x60a], (void*)&lbl_eu_80664498, (void*)&lbl_eu_806644A0);
            func_80139A18((void*)layout, (void*)&lbl_eu_8050566C[0x616], (void*)&lbl_eu_80664498, (void*)&lbl_eu_806644A0);
            func_80139A18((void*)layout, (void*)&lbl_eu_8050566C[0x621], (void*)&lbl_eu_80664498, (void*)&lbl_eu_806644A0);
            func_80139A18((void*)layout, (void*)&lbl_eu_8050566C[0x62c], (void*)&lbl_eu_80664498, (void*)&lbl_eu_806644A0);
        }
    }
}

// Refresh the cursor-position readouts: when the +0x4ac window is up, feed
// its cursor position into the +0xa0 cursor; otherwise compute the selected
// cell's pane position (func_801375A0), scale its x by the label pane's
// +0x44 factor, and drive the +0x70/+0xb8 cursors with it.
extern "C" __declspec(noinline) void func_801D0950(void* self) {
    u8* p = (u8*)self;
    nw4r::math::VEC3 posT2;
    nw4r::math::VEC3 posT1;
    nw4r::math::VEC3 posDef;
    nw4r::math::VEC3 posWin;
    if (CSysWin_getUnk34(p + 0x4ac)) {
        func_8022C1B4(&posWin, (void*)(p + 0x4ac), p[0x540]);
        ((CItemBoxObjA0Vt*)(p + 0xa0))->_v10(&posWin);
        return;
    }
    s8 idx = (s8)p[0x525];
    if (idx == -2) {
        void* t1 = ((CItemPaneObjVt*)*(void**)(*(u32*)(p + 0x44) + 0x10))->_v3C((char*)&lbl_eu_8050566C[0x637], 1);
        func_801375A0(&posT2, (void*)t1);
        void* t2 = ((CItemPaneObjVt*)*(void**)(*(u32*)(p + 0x44) + 0x10))->_v3C((char*)&lbl_eu_8050566C[0x301], 1);
        posT2.x = posT2.x * *(float*)((u8*)t2 + 0x44);
        ((CItemBoxObjA0Vt*)(p + 0xb8))->_v10(&posT2);
        func_801D216C(p + 0x70, 0);
        func_801D216C(p + 0xb8, 1);
        return;
    }
    if (idx == -1) {
        char str[32];
        sprintf(str, (const char*)&lbl_eu_8050566C[0x507], (s8)p[0x6f] + 1);
        void* t1 = ((CItemPaneObjVt*)*(void**)(*(u32*)(p + 0x44) + 0x10))->_v3C(str, 1);
        func_801375A0(&posT1, (void*)t1);
        void* t2 = ((CItemPaneObjVt*)*(void**)(*(u32*)(p + 0x44) + 0x10))->_v3C((char*)&lbl_eu_8050566C[0x301], 1);
        posT1.x = posT1.x * *(float*)((u8*)t2 + 0x44);
        ((CItemBoxObjA0Vt*)(p + 0x70))->_v10(&posT1);
        func_801D216C(p + 0x70, 1);
        func_801D216C(p + 0xb8, 0);
        return;
    }
    {
        char str[32];
        sprintf(str, (const char*)&lbl_eu_8050566C[0x396], (s8)p[0x524] + idx * 10 + 1);
        void* t1 = ((CItemPaneObjVt*)*(void**)(*(u32*)(p + 0x44) + 0x10))->_v3C(str, 1);
        func_801375A0(&posDef, (void*)t1);
        void* t2 = ((CItemPaneObjVt*)*(void**)(*(u32*)(p + 0x44) + 0x10))->_v3C((char*)&lbl_eu_8050566C[0x301], 1);
        posDef.x = posDef.x * *(float*)((u8*)t2 + 0x44);
        ((CItemBoxObjA0Vt*)(p + 0x70))->_v10(&posDef);
        func_801D216C(p + 0x70, 1);
        func_801D216C(p + 0xb8, 0);
    }
}

extern "C" __declspec(noinline) void func_801D0BD8(void* self) { }

void func_801D0E88(void* self, int r4, int r5) {
    u8* p = (u8*)self;
    u32 buf32 = *(const u32*)(const void*)&lbl_eu_80667F88;
    u16 buf16 = *(const u16*)(const void*)&lbl_eu_80667F8C;
    u8 cats[6];
    *(u32*)cats = buf32;
    *(u16*)(cats + 4) = buf16;
    int found = 0;
    int pass;
    for (pass = 1; pass <= 8; pass++) {
        void* obj = (void*)func_8009EC9C((u8)pass);
        if (r4 == 3) {
            found = 0;
            int i, j;
            for (i = 0; i < 6; i++) {
                u8 cat = cats[i];
                u16 count = func_80157C20(cat);
                for (j = 0; j < (int)count; j++) {
                    void* item = func_80157C4C(cat, (s16)j);
                    if (!item) continue;
                    if (!*(u32*)item) continue;
                    void* inst = CItem_initItemImplInstances(item);
                    void** vtbl = *(void***)inst;
                    u8 num = (u8)((u32(*)(void*, void*))vtbl[0x30/4])(inst, item);
                    int k;
                    for (k = 0; k < (int)num; k++) {
                        inst = CItem_initItemImplInstances(item);
                        vtbl = *(void***)inst;
                        s16 sv = (s16)((s32(*)(void*, void*, u32))vtbl[0x40/4])(inst, item, (u8)k);
                        if (sv == -1) continue;
                        if (sv == r5) {
                            inst = CItem_initItemImplInstances(item);
                            vtbl = *(void***)inst;
                            ((void(*)(void*, void*, u32, s32))vtbl[0x44/4])(inst, item, (u8)k, -1);
                            found = 1;
                            goto func_801D0E88_break;
                        }
                    }
                }
                if (found) goto func_801D0E88_break;
            }
        } else if (r4 == 2) {
            s16 v = *(s16*)((u8*)obj + 0x26);
            if (v != -1 && v == (s16)r5) {
                func_8009E0A8(obj, -1);
                found = 1;
            }
        } else if (r4 == 4) {
            s16 v = *(s16*)((u8*)obj + 0x1c);
            if (v != -1 && v == (s16)r5) {
                func_8009E024(obj, -1);
                found = 1;
            }
        } else if (r4 == 5) {
            s16 v = *(s16*)((u8*)obj + 0x1e);
            if (v != -1 && v == (s16)r5) {
                func_8009E030(obj, -1);
                found = 1;
            }
        } else if (r4 == 6) {
            s16 v = *(s16*)((u8*)obj + 0x20);
            if (v != -1 && v == (s16)r5) {
                func_8009E03C(obj, -1);
                found = 1;
            }
        } else if (r4 == 7) {
            s16 v = *(s16*)((u8*)obj + 0x22);
            if (v != -1 && v == (s16)r5) {
                func_8009E048(obj, -1);
                found = 1;
            }
        } else if (r4 == 8) {
            s16 v = *(s16*)((u8*)obj + 0x24);
            if (v != -1 && v == (s16)r5) {
                func_8009E054(obj, -1);
                found = 1;
            }
        }
        if (!found) {
            u32 src[3];
            src[0] = *(u32*)(lbl_eu_80505628);
            src[1] = *(u32*)(lbl_eu_80505628 + 4);
            src[2] = *(u32*)(lbl_eu_80505628 + 8);
            u8 camCtx[0x630];
            func_80043D90(camCtx);
            int i;
            for (i = 0; i < 3; i++) {
                void* cam = (void*)func_80043F18(camCtx);
                func_800F4A98(cam, src[i], 0);
                cam = (void*)func_80043F18(camCtx);
                if (*(u32*)((u8*)cam + 0x620) >= 1) {
                    cam = (void*)func_80043F18(camCtx);
                    u32* move = (u32*)func_800F6EC0(cam, 0);
                    if (move && move[1]) {
                        void* mv = func_800BFC68__FPQ22cf12CfObjectMove((void*)move[1]);
                        u16 h = *(u16*)((u8*)mv + 0x3f28);
                        if ((u32)(u8)pass == (u32)h) {
                            func_800BFDE0(mv, 0);
                            found = 1;
                            break;
                        }
                    }
                }
            }
            func_80043E88(camCtx, -1);
        }
        if (found) break;
    }
func_801D0E88_break:
    ;
}

// Cast-only vtable interface for the object returned by
// CItem_initItemImplInstances: with -RTTI on, MWCC prepends 2 hidden RTTI
// header entries, so the Nth declared virtual sits at 4*(N+2). Real virtual
// dispatch reproduces the retail `lwz r12,0(r3); lwz r12,<off>(r12)`
// sequence; manual `(*(void***)x)[N]` casts color a scratch r5 instead.
struct CItemInstVtLocal {
    virtual void v0();                   // +0x08
    virtual void v1();                   // +0x0C
    virtual void v2(void* item);         // +0x10
};

// Handle item event dispatch.
void func_801D11B8(void* self, void* item, int eventType) {
    if (!item) return;
    if (eventType < 1) {
        CItemInstVtLocal* inst = (CItemInstVtLocal*)CItem_initItemImplInstances(item);
        inst->v2(item);
    } else {
        u32 w = *(u32*)item;
        func_80158118(item, w >> 20);
    }
}

// Cast-only vtable interface for the object returned by
// CItem_initItemImplInstances: with -RTTI on, MWCC prepends 2 hidden RTTI
// header entries, so the Nth declared virtual sits at 4*(N+2). Real virtual
// dispatch reproduces the retail `lwz r12,0(r3); lwz r12,<off>(r12)`
// sequence; manual `(*(void***)x)[N]` casts color a scratch r5 instead.
// Dispatch based on entry category.
u32 func_801D1220(void* self) {
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
        return (u32)func_80136190(&lbl_eu_8050566C[0x14f], &lbl_eu_8050566C[0x158], result);
    }
    return 0;
}

// Check item teachability: look up the short-kind row in the arts table
// (lbl_eu_80664110) and test the character's learned-arts flags at
// charData + count*0x49 + (table byte << 1). Returns 1 when the flag test
// fails (item not yet learned); 0 when it is already learned.
u32 func_801D12D4(void* self, u32 kind) {
    u32 bdat = lbl_eu_80664110;
    func_801392E4(kind);
    u32 shortKind = func_80139358(kind);
    u32 flag = func_801361E8(bdat, (const char*)&lbl_eu_8050566C[0x109], (u16)shortKind);
    u32 v;
    u32 charId = func_801361E8(bdat, (const char*)&lbl_eu_8050566C[0x115], (u16)shortKind);
    u32 unk = func_801361E8(bdat, (const char*)&lbl_eu_8050566C[0x11d], (u16)shortKind);
    v = func_8013600C((void*)&lbl_eu_8050566C[0x126], (const char*)&lbl_eu_8050566C[0x12e], unk & 0xFF);
    void* charData = (void*)func_8009EC9C(charId & 0xFF);
    u8* ptr = (u8*)charData + (func_800A32BC() & 0xFF) * 0x49 + ((v & 0xFF) << 1);
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
void func_801D1F9C(short* dst, unsigned long val) {
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
void func_801C4BB4(void* self) {
    u8* p = (u8*)self;
    *(u16*)(p + 0x804) = 0;
    u16 cat;
    for (cat = 1; cat < 0x1b; cat++) {
        u32 start = func_801380A0(cat);
        if ((start & 0xFFFF) == 0) continue;
        u32 end = func_801380A0(cat + 1);
        u16 id;
        for (id = (u16)(start & 0xFFFF); (u32)id < (end & 0xFFFF); id++) {
            u32 type = func_8009CF8C(id + 0x220);
            u32 tp = type & 0xFF;
            if (tp >= 0xFE) continue;
            if (tp == 0) continue;
            if (tp == 0xC8) continue;
            u32 tblIdx = func_80138138(id);
            void* tbl = (void*)((const u32*)lbl_eu_80573D18)[tblIdx];
            if ((u8)func_801361E8((u32)tbl, (const char*)&lbl_eu_8050566C[0], id)) {
                if (tp == 1) {
                    u32 c1 = (u8)func_801361E8((u32)tbl, (const char*)&lbl_eu_8050566C[0xd], id);
                    u32 c2 = (u8)func_801361E8((u32)tbl, (const char*)&lbl_eu_8050566C[0], id);
                    if (c1 == 2) {
                        if (func_80136254((const void*)tbl, &lbl_eu_8050566C[0x1a], id)) {
                            func_801C5158(self, id);
                        }
                    }
                    if (c2 == 2) {
                        if (func_80136254((const void*)tbl, &lbl_eu_8050566C[0x26], id)) {
                            func_801C5158(self, id);
                        }
                    }
                } else if (tp == 0x6f) {
                    if ((u32)(u8)func_801361E8((u32)tbl, (const char*)&lbl_eu_8050566C[0x32], id) == 2) {
                        if (func_80136254((const void*)tbl, &lbl_eu_8050566C[0x3f], id)) {
                            func_801C5158(self, id);
                        }
                    }
                } else if (tp == 0x79) {
                    if ((u32)(u8)func_801361E8((u32)tbl, (const char*)&lbl_eu_8050566C[0x4b], id) == 2) {
                        if (func_80136254((const void*)tbl, &lbl_eu_8050566C[0x58], id)) {
                            func_801C5158(self, id);
                        }
                    }
                } else if (tp == 0x83) {
                    if ((u32)(u8)func_801361E8((u32)tbl, (const char*)&lbl_eu_8050566C[0x64], id) == 2) {
                        if (func_80136254((const void*)tbl, &lbl_eu_8050566C[0x71], id)) {
                            func_801C5158(self, id);
                        }
                    }
                } else if (tp == 0x70) {
                    if ((u32)(u8)func_801361E8((u32)tbl, (const char*)&lbl_eu_8050566C[0x7d], id) == 2) {
                        if (func_80136254((const void*)tbl, &lbl_eu_8050566C[0x8a], id)) {
                            func_801C5158(self, id);
                        }
                    }
                } else if (tp == 0x7a) {
                    if ((u32)(u8)func_801361E8((u32)tbl, (const char*)&lbl_eu_8050566C[0x96], id) == 2) {
                        if (func_80136254((const void*)tbl, &lbl_eu_8050566C[0xa3], id)) {
                            func_801C5158(self, id);
                        }
                    }
                } else if (tp == 0x84) {
                    if ((u32)(u8)func_801361E8((u32)tbl, (const char*)&lbl_eu_8050566C[0xaf], id) == 2) {
                        if (func_80136254((const void*)tbl, &lbl_eu_8050566C[0xbc], id)) {
                            func_801C5158(self, id);
                        }
                    }
                }
            } else {
                if ((u8)func_801361E8((u32)tbl, (const char*)&lbl_eu_8050566C[0xc8], id)) {
                    if (tp == 1) {
                        if ((u32)(u8)func_801361E8((u32)tbl, (const char*)&lbl_eu_8050566C[0xd], id) == 2) {
                            if (func_80136254((const void*)tbl, &lbl_eu_8050566C[0x1a], id)) {
                                func_801C5158(self, id);
                            }
                        }
                    } else if (tp == 0x6f) {
                        if ((u32)(u8)func_801361E8((u32)tbl, (const char*)&lbl_eu_8050566C[0x32], id) == 2) {
                            if (func_80136254((const void*)tbl, &lbl_eu_8050566C[0x3f], id)) {
                                func_801C5158(self, id);
                            }
                        }
                    } else if (tp == 0x79) {
                        if ((u32)(u8)func_801361E8((u32)tbl, (const char*)&lbl_eu_8050566C[0x4b], id) == 2) {
                            if (func_80136254((const void*)tbl, &lbl_eu_8050566C[0x58], id)) {
                                func_801C5158(self, id);
                            }
                        }
                    } else if (tp == 0x83) {
                        if ((u32)(u8)func_801361E8((u32)tbl, (const char*)&lbl_eu_8050566C[0x64], id) == 2) {
                            if (func_80136254((const void*)tbl, &lbl_eu_8050566C[0x71], id)) {
                                func_801C5158(self, id);
                            }
                        }
                    }
                } else {
                    // Fallback: no base name and no teach key - check ability slots
                    void* obj = (void*)func_801412D0(id);
                    if (!func_80140854(obj, 0, 0)) {
                        if ((u32)(u8)func_801361E8((u32)tbl, (const char*)&lbl_eu_8050566C[0xd], id) == 2) {
                            if (func_80136254((const void*)tbl, &lbl_eu_8050566C[0x1a], id)) {
                                func_801C5158(self, id);
                            }
                        }
                    }
                    if (!func_80140854(obj, 0, 1)) {
                        if ((u32)(u8)func_801361E8((u32)tbl, (const char*)&lbl_eu_8050566C[0x32], id) == 2) {
                            if (func_80136254((const void*)tbl, &lbl_eu_8050566C[0x3f], id)) {
                                func_801C5158(self, id);
                            }
                        }
                    }
                    if (!func_80140854(obj, 0, 2)) {
                        if ((u32)(u8)func_801361E8((u32)tbl, (const char*)&lbl_eu_8050566C[0x4b], id) == 2) {
                            if (func_80136254((const void*)tbl, &lbl_eu_8050566C[0x58], id)) {
                                func_801C5158(self, id);
                            }
                        }
                    }
                    if (!func_80140854(obj, 0, 3)) {
                        if ((u32)(u8)func_801361E8((u32)tbl, (const char*)&lbl_eu_8050566C[0x64], id) == 2) {
                            if (func_80136254((const void*)tbl, &lbl_eu_8050566C[0x71], id)) {
                                func_801C5158(self, id);
                            }
                        }
                    }
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
    if (func_801C51BC(self, id)) return;
    if ((func_801392E4(id) & 0xFFFF) == 12) return;
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

        u32 flag1 = func_80136254(bdat, (const char*)&lbl_eu_8050566C[0xe2], (u32)i);
        if (flag1 & 0xFFFF) {
            u8 val = (u8)func_8009CF8C((flag1 & 0xFFFF) + 0x220);
            if (val >= 0xFE) continue;
            if (val > (u8)func_801361E8((u32)bdat, (const char*)&lbl_eu_8050566C[0xea], (u32)i)) continue;
        } else {
            u32 hasVal = func_801361E8((u32)bdat, (const char*)&lbl_eu_8050566C[0xf3], (u32)i);
            if (hasVal & 0xFF) {
                if ((u8)func_801361E8((u32)bdat, (const char*)&lbl_eu_8050566C[0xfa], (u32)i)
                    <= (u8)func_8009CF8C((hasVal & 0xFF) + 0x7fc)) continue;
            }
        }

        u32 type = func_80136254(bdat, (const char*)&lbl_eu_8050566C[0x102], (u32)i);
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
        u32 flag = func_801361E8(bdat, (const char*)&lbl_eu_8050566C[0x109], (u32)i);
        u32 charId = func_801361E8(bdat, (const char*)&lbl_eu_8050566C[0x115], (u32)i);
        u32 unk = func_801361E8(bdat, (const char*)&lbl_eu_8050566C[0x11d], (u32)i);
        u32 sub = func_8013600C((void*)&lbl_eu_8050566C[0x126], (const char*)&lbl_eu_8050566C[0x12e],
                            (u8)unk);
        // Reuse the charId slot for the character-data pointer (retail
        // recycles its register); then add the party stride and sub offset.
        charId = (u32)func_8009EC9C((u8)charId);
        u8* cell = (u8*)charId + (u8)func_800A32BC() * 0x49;
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

void OnFileEvent__12CItemBoxGridFP10CEventFile(void* self, void* event) {
    u8* p = (u8*)self;
    // Keep the handle comparison operandless so MWCC holds it in r4 like retail.
    if (*(u32*)(p + 0x28) != *(u32*)((u8*)event + 4)) return;
    void* memHandle = getHandleMEM2__Q23mtl10MemManagerFv();
    createRegion__17UnkClass_8045F564FiiPCci((void*)(p + 8), 0x28000, (int)&lbl_eu_8050566C[0x64d], (const char*)memHandle, 0);
    u8 regionBuf[16];
    __ct__14Class_8045F858FP17UnkClass_8045F564(regionBuf, p + 8);
    u32* fileData = *(u32**)(p + 0x28);
    void* fileBuf = (void*)fileData[4];
    fileData[4] = 0;
    func_80434A4C__Q23mtl10MemManagerFb(0);
    void* allocHandle = getAllocHandle__10CLibLayoutFv();
    void* tpMem = allocate__Q23mtl10MemManagerFUlUl(0x858, (u32)allocHandle);
    if (tpMem) __ct__CTagProcessor(tpMem);
    *(u32*)(p + 0x5c) = (u32)tpMem;
    void* arcRes = createArcResourceAccessor__10CLibLayoutFv();
    *(u32*)(p + 0x3c) = (u32)arcRes;
    Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc((nw4r::lyt::ArcResourceAccessor*)arcRes, fileBuf, &lbl_eu_8050566C[0x65a]);
    func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc((nw4r::lyt::Layout**)(p + 0x44), (nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x3c), &lbl_eu_8050566C[0x65e]);
    void* layout = *(void**)(p + 0x44);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc((nw4r::lyt::Layout*)layout, (nw4r::lyt::AnimTransform**)(p + 0x48), (nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x3c), (char*)&lbl_eu_8050566C[0x673]);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc((nw4r::lyt::Layout*)layout, (nw4r::lyt::AnimTransform**)(p + 0x4c), (nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x3c), (char*)&lbl_eu_8050566C[0x68b]);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc((nw4r::lyt::Layout*)layout, (nw4r::lyt::AnimTransform**)(p + 0x50), (nw4r::lyt::ArcResourceAccessor*)*(void**)(p + 0x3c), (char*)&lbl_eu_8050566C[0x6a8]);
    void* rootPane = *(void**)(*(u32*)(p + 0x44) + 0x10);
    void* font = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, (nw4r::lyt::Layout*)layout);
    void** vtblFont = *(void***)font;
    void* fontData = ((void*(*)())vtblFont[9])();
    func_8013676C(rootPane, (u32)fontData);
    u32 screenW = (u32)func_801355A0__Fv();
    func_801368C0__FPQ34nw4r3lyt6LayoutPcUl((nw4r::lyt::Layout*)layout, (char*)&lbl_eu_8050566C[0x55d], screenW);
    func_801368C0__FPQ34nw4r3lyt6LayoutPcUl((nw4r::lyt::Layout*)layout, (char*)&lbl_eu_8050566C[0x53e], screenW);
    u32 screenH = (u32)func_801355BC();
    u8 i;
    for (i = 1; i <= 0x1e; i++) {
        char nameBuf[32];
        sprintf(nameBuf, (const char*)&lbl_eu_8050566C[0x3a3], i);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl((nw4r::lyt::Layout*)layout, nameBuf, screenH);
    }
    func_801368C0__FPQ34nw4r3lyt6LayoutPcUl((nw4r::lyt::Layout*)layout, (char*)&lbl_eu_8050566C[0x59b], screenH);
    // Set visibility for animations
    void** vtblLayout1 = *(void***)layout;
    ((void(*)(void*, void*, u32))vtblLayout1[0xb])(layout, *(void**)(p + 0x4c), 0);
    ((void(*)(void*, void*, u32))vtblLayout1[0xb])(layout, *(void**)(p + 0x50), 0);
    ((void(*)(void*, void*, u32))vtblLayout1[0xb])(layout, *(void**)(p + 0x48), 1);
    ((void(*)(void*, u32))vtblLayout1[0xe])(layout, 0);
    func_80139198(0);
    // Set tag processor
    void* pane = ((void*(*)(void*, const char*, u32))vtblLayout1[0xf])(layout, &lbl_eu_8050566C[0x58a], 1);
    if (pane) *(u32*)((u8*)pane + 0xf8) = *(u32*)(p + 0x5c);
    // Set up grid name
    u32 msgA = (u32)func_80136190((void*)&lbl_eu_8050566C[0x14f], &lbl_eu_8050566C[0x158], 5);
    func_80136B4C((nw4r::lyt::Layout*)layout, (char*)&lbl_eu_8050566C[0x6c2], (char*)msgA, 0);
    u32 msgB = (u32)func_80136190((void*)&lbl_eu_8050566C[0x14f], &lbl_eu_8050566C[0x158], 4);
    func_80136B4C((nw4r::lyt::Layout*)layout, (char*)&lbl_eu_8050566C[0x6d1], (char*)msgB, 0);
    func_80136B4C((nw4r::lyt::Layout*)layout, (char*)&lbl_eu_8050566C[0x642], (char*)&lbl_eu_8050566C[0x3af], 0);
    // Check game mode
    const char* modeStr;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1)) {
        modeStr = &lbl_eu_8050566C[0x6dd];
    } else {
        modeStr = &lbl_eu_8050566C[0x6e6];
    }
    u32 msgC = func_8013606C((const char*)&lbl_eu_8050566C[0x6ef], modeStr, 0x49);
    u32 msgId = (u32)func_80138F78(msgC & 0xFFFF);
    void* sysWin = func_801355F4();
    void** vtblSys = *(void***)sysWin;
    u32 result = (u32)((void*(*)(void*, u32, u32, u32))vtblSys[3])(sysWin, 0x74696D67, msgId, 0);
    if (result) {
        u32 res = result;
        func_80137E7C((void*)layout, (void*)&lbl_eu_8050566C[0x6fd], (void*)res);
        void* pane2 = ((void*(*)(void*, const char*, u32))vtblLayout1[0xf])(layout, &lbl_eu_8050566C[0x6fd], 1);
        if (pane2) {
            u32* mat = *(u32**)(res + 8);
            u32* texData = (u32*)mat[0];
            u16 w = *(u16*)(texData + 2);
            u16 h = *(u16*)(texData + 0);
            ((float*)((u8*)pane2 + 0x4c))[0] = (float)(s32)w;
            ((float*)((u8*)pane2 + 0x50))[0] = (float)(s32)h;
        }
    }
    // Set icon visibility based on mode
    if (p[0x527] != 1) {
        void* iconPane = ((void*(*)(void*, const char*, u32))vtblLayout1[0xf])(layout, &lbl_eu_8050566C[0x6c2], 1);
        if (iconPane) func_80124270(iconPane, 0);
        if (p[0x527] != 2) {
            void* iconPane2 = ((void*(*)(void*, const char*, u32))vtblLayout1[0xf])(layout, &lbl_eu_8050566C[0x59b], 1);
            if (iconPane2) func_80124270(iconPane2, 0);
        }
    }
    // Copy cursor pane translation into self+0x534
    void* posPane = ((void*(*)(void*, const char*, u32))vtblLayout1[0xf])(layout, &lbl_eu_8050566C[0x709], 1);
    if (posPane) {
        copyVEC3((float*)(p + 0x534), (float*)((u8*)posPane + 0x2c));
    }
    // Snapshot both cursor colour states into the global colour tables
    CEquipBoxFourShorts colA = func_80139658(layout, &lbl_eu_8050566C[0x716], 0);
    CopyVec4s(&lbl_eu_80664488, &colA);
    CEquipBoxFourShorts colB = func_80139658(layout, &lbl_eu_8050566C[0x716], 1);
    CopyVec4s(&lbl_eu_80664490, &colB);
    // Copy color indices
    *(s16*)((u8*)&lbl_eu_80664498 + 6) = *(s16*)((u8*)&lbl_eu_80664488 + 6);
    *(s16*)((u8*)&lbl_eu_806644A0 + 6) = *(s16*)((u8*)&lbl_eu_80664490 + 6);
    *(s16*)((u8*)&lbl_eu_806644A8 + 6) = *(s16*)((u8*)&lbl_eu_80664488 + 6);
    *(s16*)((u8*)&lbl_eu_806644B0 + 6) = *(s16*)((u8*)&lbl_eu_80664490 + 6);
    *(s16*)((u8*)&lbl_eu_806644B8 + 6) = *(s16*)((u8*)&lbl_eu_80664488 + 6);
    *(s16*)((u8*)&lbl_eu_806644C0 + 6) = *(s16*)((u8*)&lbl_eu_80664490 + 6);
    *(s16*)((u8*)&lbl_eu_806644C8 + 6) = *(s16*)((u8*)&lbl_eu_80664488 + 6);
    *(s16*)((u8*)&lbl_eu_806644D0 + 6) = *(s16*)((u8*)&lbl_eu_80664490 + 6);
    // Create cursor 07
    u8 cur07Buf[0x18];
    __ct__CCur07(cur07Buf, *(void**)(p + 0x3c));
    func_8018B0FC(p + 0x70, cur07Buf);
    __dt__6CCur07Fv(cur07Buf, -1);
    void** vtblCur07 = *(void***)(p + 0x70);
    ((void(*)(void*))vtblCur07[2])(p + 0x70);
    // Create cursor 09
    u8 cur09Buf[0x18];
    __ct__CCur09(cur09Buf, *(void**)(p + 0x3c));
    func_8018B0FC(p + 0x88, cur09Buf);
    __dt__6CCur09Fv(cur09Buf, -1);
    void** vtblCur09 = *(void***)(p + 0x88);
    ((void(*)(void*))vtblCur09[2])(p + 0x88);
    // Set vectors on cursor 09
    u8 vec3Buf1[12];
    code80135FDC_setVec3((float*)vec3Buf1, lbl_eu_80667F90, lbl_eu_80667F94, lbl_eu_80667F34);
    u8 vec3Buf2[12];
    code80135FDC_setVec3((float*)vec3Buf2, lbl_eu_80667F98, lbl_eu_80667F94, lbl_eu_80667F34);
    func_801D24E8(p + 0x88, vec3Buf2, vec3Buf1);
    // Create cursor 18
    u8 cur18Buf[0x18];
    __ct__CCur18(cur18Buf, func_801355F4());
    func_8018B0FC(p + 0xa0, cur18Buf);
    __dt__6CCur18Fv(cur18Buf, -1);
    void** vtblCur18 = *(void***)(p + 0xa0);
    ((void(*)(void*))vtblCur18[2])(p + 0xa0);
    // Create cursor 16
    u8 cur16Buf[0x18];
    __ct__CCur16(cur16Buf, *(void**)(p + 0x3c));
    func_8018B0FC(p + 0xb8, cur16Buf);
    __dt__6CCur16Fv(cur16Buf, -1);
    void** vtblCur16 = *(void***)(p + 0xb8);
    ((void(*)(void*))vtblCur16[2])(p + 0xb8);
    // Create cursor 11
    u8 cur11Buf[0x18];
    __ct__CCur11(cur11Buf, *(void**)(p + 0x3c));
    func_8018B0FC(p + 0xd0, cur11Buf);
    __dt__6CCur11Fv(cur11Buf, -1);
    void** vtblCur11 = *(void***)(p + 0xd0);
    ((void(*)(void*))vtblCur11[2])(p + 0xd0);
    // Init teach display
    func_8022D614(p + 0x468, *(void**)(p + 0x3c));
    // Init file select
    func_80207FC8(p + 0x418, *(void**)(p + 0x3c));
    // Set up remaining UI
    void* grpPane = ((void*(*)(void*, const char*, u32))vtblLayout1[0xf])(layout, &lbl_eu_8050566C[0x723], 1);
    if (grpPane) {
        CEquipBoxFourShorts gcolA = func_801397AC(grpPane, 0);
        CopyVec4s(&lbl_eu_806644D8, &gcolA);
        CEquipBoxFourShorts gcolB = func_801397AC(grpPane, 1);
        CopyVec4s(&lbl_eu_806644E0, &gcolB);
        *(s16*)((u8*)&lbl_eu_806644E8 + 6) = *(s16*)((u8*)&lbl_eu_806644D8 + 6);
        *(s16*)((u8*)&lbl_eu_806644F0 + 6) = *(s16*)((u8*)&lbl_eu_806644E0 + 6);
        *(s16*)((u8*)&lbl_eu_806644F8 + 6) = *(s16*)((u8*)&lbl_eu_806644D8 + 6);
        *(s16*)((u8*)&lbl_eu_80664500 + 6) = *(s16*)((u8*)&lbl_eu_806644E0 + 6);
    }
    u32 msgD = (u32)func_80136190((void*)&lbl_eu_8050566C[0x14f], &lbl_eu_8050566C[0x158], 7);
    func_80136B4C((nw4r::lyt::Layout*)layout, (char*)&lbl_eu_8050566C[0x72d], (char*)msgD, 0);
    if (p[0x527] != 4) {
        void* iconPane3 = ((void*(*)(void*, const char*, u32))vtblLayout1[0xf])(layout, &lbl_eu_8050566C[0x637], 1);
        if (iconPane3) func_80124270(iconPane3, 0);
        func_80136B4C((nw4r::lyt::Layout*)layout, (char*)&lbl_eu_8050566C[0x72d], (char*)&lbl_eu_8050566C[0x3af], 0);
    }
    if (p[0x527] == 2) {
        u8 j;
        for (j = 0; j < (u8)code80135FDC_getByte_64077(); j++) {
            u32 pn = func_801392B4(j);
            void* charData = (void*)func_8009EC9C(pn & 0xFF);
            void* chp = (u8*)charData + 0x3534;
            s32 stat = func_8026178C(chp, 0x8f);
            if (stat) {
                void* pane3 = ((void*(*)(void*, const char*, u32))vtblLayout1[0xf])(layout, &lbl_eu_8050566C[0x5b9], 1);
                u8 nameIdx = j + 1;
                char paneBuf[32];
                sprintf(paneBuf, (const char*)&lbl_eu_8050566C[0x5c8], nameIdx);
                void* pane4 = ((void*(*)(void*, const char*, u32))vtblLayout1[0xf])(layout, paneBuf, 1);
                if (pane4) {
                    // Set up character icon
                    // ... (complex texture setup)
                }
                // ... more per-character setup
            }
        }
    }
}

// Static initialization of color table entries.
void sinit_801D1E30(void) {
    func_801D1F9C((void*)&lbl_eu_80664488, 0);
    func_801D1F9C((void*)&lbl_eu_80664490, 0);
    func_801C4B60((void*)&lbl_eu_80664498, 0x79, 0x49, 0x07, 0x00);
    func_801C4B60((void*)&lbl_eu_806644A0, 0xED, 0xCD, 0x83, 0x00);
    func_801C4B60((void*)&lbl_eu_806644A8, 0x1A, 0x43, 0x53, 0x00);
    func_801C4B60((void*)&lbl_eu_806644B0, 0xC4, 0xE8, 0xEB, 0x00);
    func_801C4B60((void*)&lbl_eu_806644B8, 0x74, 0x54, 0x1D, 0x00);
    func_801C4B60((void*)&lbl_eu_806644C0, 0xD5, 0xB9, 0x78, 0x00);
    func_801C4B60((void*)&lbl_eu_806644C8, 0x3D, 0x68, 0x78, 0x00);
    func_801C4B60((void*)&lbl_eu_806644D0, 0xC4, 0xE8, 0xEB, 0x00);
    func_801D1F9C((void*)&lbl_eu_806644D8, 0);
    func_801D1F9C((void*)&lbl_eu_806644E0, 0);
    func_801C4B60((void*)&lbl_eu_806644E8, 0x12, 0xA3, 0xE7, 0x00);
    func_801C4B60((void*)&lbl_eu_806644F0, 0xFF, 0xFF, 0xFF, 0x00);
    func_801C4B60((void*)&lbl_eu_806644F8, 0xB3, 0x09, 0xC0, 0x00);
    func_801C4B60((void*)&lbl_eu_80664500, 0xFF, 0xFF, 0xFF, 0x00);
}

extern u8 lbl_eu_805347F8[];

