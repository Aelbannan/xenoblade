// Auto-scaffolded catalog TU for kyoshin/makecrystal/CMCGetItemBox
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/makecrystal/CMCGetItemBox.hpp"
#include "monolib/util/MemManager.hpp"

#include <nw4r/lyt.h>
#include <cstdio>

namespace nw4r { namespace lyt { class AnimTransform; } }

// ---- C-linkage / foreign retail helper declarations ----------------------
// Only unmangled retail symbols (confirmed via config/us/symbols.txt) are
// declared extern "C"; the mangled ones are declared as normal C++ so the
// Itanium-mangled reloc name is emitted (matches the retail reloc).
extern "C" {
void* CItem_initItemImplInstances(void*);
char* func_80138F78(u16);
int   func_801C6E90(void*);
u32   func_80136254(void*, const char*, u16);
void  func_80137E7C(void*, const char*, void*);
u32   func_801361E8(void*, const char*, u32);
char* func_8013639C(void*, const char*, ...);
void func_801D216C(void*, int);
void func_801599D4(CMCItemBoxEntry*, int);
u32 CSysWin_isReady(CMCGetItemBoxSysWin*);
u32 CSysWin_getUnk34(CMCGetItemBoxSysWin*);
int  CSysWin_isActive(CMCGetItemBoxSysWin*);
void func_8022B8E4(CMCGetItemBoxSysWin*);
u32 func_80157CD0(u8);
void advanceItemBoxState__FP12CItemBoxInfo(CItemBoxInfo*);
u16 ArrayGet12(const unsigned short*, unsigned char);
void func_801CB9D8(u32*, void*, u32);
void func_80137924(void*, void*, void*, void*);
char* func_80136190(const void*, const void*, int);
void func_80124270(nw4r::lyt::Pane*, u32);
void func_801D4174(void*);
void func_801D4260(void*, u16);
void func_8022B7F4(void*);
void* func_801D3C74(void*, u32);
// Retail mangled names kept as source names so the reloc matches exactly.
void func_8003AA8C__5CBdatFUl(u32);
void func_801390E0__FPP11CFileHandle(void*);
void func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(nw4r::lyt::ArcResourceAccessor*);
}
// C++-linkage (mangled) retail symbols.
void func_80136910(nw4r::lyt::Layout*, char*, u8);   // func_80136910__FPQ34nw4r3lyt6LayoutPcUc
void func_80138078(u32);                             // func_80138078__FUl
u8 getItemBoxState(CItemBoxInfo*);                   // getItemBoxState__FP12CItemBoxInfo

// Foreign rodata (string pool used by the item-box layout).
extern "C" char lbl_eu_8050FF8C[];
extern "C" void* lbl_eu_806640EC;
extern "C" void* lbl_eu_806640D8;
extern "C" void* lbl_eu_80664100;
extern "C" void* lbl_eu_80664A18;
extern "C" void* lbl_eu_80664A1C;

// Same-unit helper functions.
void func_80296BF0(CMCItemBoxSub*);
void func_8029967C(CMCGetItemBox*);
void func_802998C8(CMCGetItemBox*);
void func_802999B0(CMCGetItemBox*);
void func_80299490(CMCGetItemBox*, int, u32);
extern "C" void func_80298AC8(CMCGetItemBox*, u32, CMCItemBoxEntry*, u8);
extern "C" void func_80298FB4(CMCGetItemBox*, u32, CMCItemBoxEntry*, u8);
void func_80298378(CMCGetItemBox*);
void func_802983E4(CMCGetItemBox*);

// Initialise a CMCItemBoxSub: clear the offset table to 0xFFFF, zero the
// counters and index fields, then reset the whole table again.
void func_80296B44(CMCItemBoxSub* x) {
    for (int i = 0; i < 0x80; i++) x->table[i] = -1;
    x->count = 0;
    x->pad_102 = 0;
    x->limit = 0;
    x->counter = 0;
    x->field_108 = 0;
    x->field_148 = 0;
    x->field_14C = 0;
    x->field_1CC = 0;
    x->listBase = (CMCItemBoxEntry*)0;
    x->field_1D4 = 0;
    for (int i = 0; i < 0x80; i++) x->table[i] = -1;
}

void __dt__80296BB0(){}

void func_80296BF0(CMCItemBoxSub* x) {}

// Advance the counter; wrap to 0 when past the signed byte limit.
void func_80296D00(CMCItemBoxSub* x) {
    u8 v = (u8)(x->counter + 1);
    x->counter = v;
    if ((s8)v < x->limit) return;
    x->counter = 0;
}

// Count the counter down; wrap to (limit-1) when it goes negative.
void func_80296D2C(CMCItemBoxSub* x) {
    u8 v = (u8)(x->counter - 1);
    x->counter = v;
    if ((s8)v >= 0) return;
    x->counter = (u8)(x->limit - 1);
}

// Look up the entry at `index` in the offset table and return the derived
// index-table word >> 20, or 0 when out of range.
u32 func_80296D54(CMCItemBoxSub* x, u32 index) {
    CMCItemBoxEntry* base = x->listBase;
    if (base == 0) return 0;
    u32 idx = (u16)(index + (s8)x->counter * 30);
    if (idx >= x->count) return 0;
    s16 off = x->table[idx];
    CMCItemBoxEntry* p = base + off;
    if (!p) return 0;
    return p->field_00 >> 20;
}

// Look up the entry at `index` in the offset table and return its pointer.
CMCItemBoxEntry* func_80296DB0(CMCItemBoxSub* x, u32 index) {
    CMCItemBoxEntry* base = x->listBase;
    if (base == 0) return 0;
    u32 idx = (u16)(index + (s8)x->counter * 30);
    if (idx >= x->count) return 0;
    s16 off = x->table[idx];
    CMCItemBoxEntry* p = base + off;
    return p;
}

s8 func_80296E00(CMCItemBoxSub* x, u32 index) {
    CMCItemBoxEntry* base = x->listBase;
    if (base == 0) return 0;
    u16 idx = (u16)(index + (s8)x->counter * 30);
    if (idx < x->count) {
        CMCItemBoxEntry* p = base + x->table[idx];
        if (p != 0) {
            void* inst = CItem_initItemImplInstances(p);
            u32 r = ((u32(*)(void*, void*))(*(void***)inst)[2])(inst, p);
            return (s8)(u16)r;
        }
    }
    return 0;
}

// Format the selected entry's item-name into the FixStr<64> at sub+0x108
// (object offset 0x41C). Returns that FixStr, or null when out of range.
char* func_80296E98(CMCItemBoxSub* sub, u16 index) {
    CMCItemBoxEntry* base = sub->listBase;
    if (base == 0) return 0;
    u32 idx = (u16)(index + (s8)sub->counter * 30);
    if (idx >= sub->count) return 0;
    s16 off = sub->table[idx];
    CMCItemBoxEntry* p = base + off;
    if (!p) return 0;
    char* b = (char*)lbl_eu_8050FF8C;
    void* inst = CItem_initItemImplInstances(p);
    void* itemKind = (*(void*(**)(void*, void*))(*(void***)inst)[8])(inst, p);
    ((ml::FixStr<64>*)((char*)sub + 0x108))->format(&b[0], (char*)itemKind);
    if (sub->pad_102 == 3) {
        void* inst2 = CItem_initItemImplInstances(p);
        u8 v = (u8)((u8(*)(void*, void*))(*(void***)inst2)[2])(inst2, p);
        char* itemName = func_80136190(&b[3], &b[0xc], 0x1e - (v - 1));
        char copy[64];
        u32 len = strlen(((ml::FixStr<64>*)((char*)sub + 0x108))->c_str());
        strcpy(copy, ((ml::FixStr<64>*)((char*)sub + 0x108))->c_str());
        ((ml::FixStr<64>*)((char*)sub + 0x108))->format(&b[0x11], copy, itemName);
    }
    return (char*)((char*)sub + 0x108);
}

void func_80296FC0(){}

CMCGetItemBox::CMCGetItemBox() {}

CMCGetItemBox::~CMCGetItemBox() {}

void func_80297928(){}

void func_802979E4(){}

void func_80297AAC(){}

// Clean up every resource owned by the item-box widget.
void func_80297B68(CMCGetItemBox* self) {
    func_8003AA8C__5CBdatFUl(2);
    func_8003AA8C__5CBdatFUl(5);
    func_801390E0__FPP11CFileHandle(&self->fileHandle1);
    func_801390E0__FPP11CFileHandle(&self->fileHandle2);
    func_801390E0__FPP11CFileHandle(&self->fileHandle3);
    func_801390E0__FPP11CFileHandle(&self->fileHandle4);
    self->field_4C = 0;
    if (self->memManagerPtr != 0) {
        mtl::MemManager::deallocate(self->memManagerPtr);
        self->memManagerPtr = 0;
    }
    if (self->layout40 != 0) {
        if (self->layout40 != 0) {
            (*(void(**)(void*, u32))((void**)self->layout40)[2])(self->layout40, 1);
        }
        self->layout40 = 0;
    }
    func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor((nw4r::lyt::ArcResourceAccessor*)self->arcAcc1);
    func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor((nw4r::lyt::ArcResourceAccessor*)self->arcAcc2);
    void* o = self->objAt50;
    self->arcAcc1 = 0;
    self->arcAcc2 = 0;
    if (o != 0) {
        if (o != 0) {
            (*(void(**)(void*, u32))((void**)o)[2])(o, 1);
        }
        self->objAt50 = 0;
    }
    self->memRegion1.func_8045F778();
    self->memRegion2.func_8045F778();
    (*(void(**)(void*))((void**)&self->subObj_58)[3])(&self->subObj_58);
    (*(void(**)(void*))((void**)&self->subObj_70)[3])(&self->subObj_70);
    (*(void(**)(void*))((void**)&self->subObj_88)[3])(&self->subObj_88);
    (*(void(**)(void*))((void**)&self->subObj_A0)[3])(&self->subObj_A0);
    func_801D4174((void*)self->itemBox);
    func_8022B7F4((void*)&self->sysWin_B8);
}

// Return 0 unless the item box is active and the sys-win is ready.
u8 func_80297CC0(CMCGetItemBox* self) {
    if (getItemBoxState((CItemBoxInfo*)self->itemBox) == 0) return 0;
    if (CSysWin_isReady(&self->sysWin_B8) == 0) return 0;
    return self->field_54;
}

u8 CMCGetItemBox::func_80297D1C() { return mField55; }

u8 CMCGetItemBox::func_80297D24() { return mField303; }

void func_80297D2C(){}

// When the item-box widget is done (state 3), advance to state 4, detach the
// helper widgets and advance the item-box state machine.
void func_80297E18(CMCGetItemBox* self) {
    if (self->field_4D != 3) return;
    self->field_4D = 4;
    self->mField55 = 0;
    func_801D216C(&self->subObj_58, 0);
    func_801D216C(&self->subObj_70, 0);
    func_801D216C(&self->subObj_88, 0);
    advanceItemBoxState__FP12CItemBoxInfo((CItemBoxInfo*)self->itemBox);
    func_80138078(0x6);
}

void func_80297E90(){}

// Advance the cursor to the next non-empty slot.
void func_80297FB4(CMCGetItemBox* self) {
    if (CSysWin_getUnk34(&self->sysWin_B8) != 0) return;
    if (self->mField303 == 0) {
        u8 v = self->field_301 + 1;
        self->field_301 = v;
        if ((s8)v >= 3) self->field_301 = 0xff;
        func_802999B0(self);
        func_802998C8(self);
    } else {
        u8* arr = &self->arr_1A4[0];
        s8 idx = (s8)((s8)self->field_305 + 1);
        while (idx != (s8)self->field_305) {
            if (idx >= 4) idx = 0;
            if (ArrayGet12((const unsigned short*)arr,
                           (u8)((s8)self->field_304 * 4 + idx)) != 0) {
                self->field_305 = (u8)idx;
                break;
            }
            idx++;
        }
        u8 tmp[12];
        func_801CB9D8((u32*)tmp, arr,
                      (u8)((s8)self->field_304 * 4 + self->field_305));
        (*(void(**)(void*, void*))((void**)&self->subObj_A0)[4])(&self->subObj_A0, (void*)tmp);
    }
    func_80138078(0x1);
}

void func_802980DC(CMCGetItemBox* self) {
    if (CSysWin_getUnk34(&self->sysWin_B8) != 0) return;
    if (self->mField303 == 0) {
        if ((s8)self->field_300 == 0) {
            self->field_300 = 9;
            self->field_301 = 0;
            func_802983E4(self);
            func_802999B0(self);
        } else {
            u8 n = self->field_300 - 1;
            self->field_300 = n;
            if ((s8)n < 0) self->field_300 = 9;
            func_802999B0(self);
            func_802998C8(self);
        }
    } else {
        u8* arr = &self->arr_1A4[0];
        s8 idx = (s8)((s8)self->field_304 - 1);
        while (idx != (s8)self->field_304) {
            if (idx < 0) idx = 2;
            if (ArrayGet12((const unsigned short*)arr,
                           (u8)((s8)idx * 4 + self->field_305)) != 0) {
                self->field_304 = (u8)idx;
                break;
            }
            idx--;
        }
        u8 tmp[12];
        func_801CB9D8((u32*)tmp, arr,
                      (u8)((s8)self->field_304 * 4 + self->field_305));
        (*(void(**)(void*, void*))((void**)&self->subObj_A0)[4])(&self->subObj_A0, (void*)tmp);
    }
    func_80138078(0x1);
}

void func_80298228(CMCGetItemBox* self) {
    if (CSysWin_getUnk34(&self->sysWin_B8) != 0) return;
    if (self->mField303 == 0) {
        u8 s = self->field_300;
        if (s == 9) {
            self->field_300 = 0;
            self->field_301 = 0;
            func_80298378(self);
            func_802999B0(self);
        } else {
            u8 n = s + 1;
            self->field_300 = n;
            if ((s8)n >= 10) self->field_300 = 0;
            func_802999B0(self);
            func_802998C8(self);
        }
    } else {
        s32 cur = (s8)self->field_304;
        s32 idx = (s8)self->field_304 + 1;
        while (idx != cur) {
            if (idx >= 3) idx = 0;
            u8 combined = (u8)((s8)idx * 4 + self->field_305);
            if (ArrayGet12((const unsigned short*)&self->arr_1A4[0], combined) != 0) {
                self->field_304 = (u8)idx;
                break;
            }
            idx++;
        }
        u8 tmp[12];
        func_801CB9D8((u32*)tmp, (void*)&self->arr_1A4[0],
                      (u8)((s8)self->field_304 * 4 + self->field_305));
        (*(void(**)(void*, void*))((void**)&self->subObj_A0)[4])(&self->subObj_A0, (void*)tmp);
    }
    func_80138078(0x1);
}

// Increment the sub counter, refresh helper widgets, and play a sound when the
// item-box limit is anything other than 1.
void func_80298378(CMCGetItemBox* self) {
    CMCItemBoxSub* x = &self->sub_314;
    func_80296D00(x);
    func_8029967C(self);
    func_802998C8(self);
    u8 lim = x->limit;
    if (lim == 0) lim = 1;
    if (lim != 1) func_80138078(0xa);
}

// Decrement the sub counter, refresh helper widgets, and play a sound when the
// item-box limit is anything other than 1.
void func_802983E4(CMCGetItemBox* self) {
    CMCItemBoxSub* x = &self->sub_314;
    func_80296D2C(x);
    func_8029967C(self);
    func_802998C8(self);
    u8 lim = x->limit;
    if (lim == 0) lim = 1;
    if (lim != 1) func_80138078(0xa);
}

// Toggle the item-box help window: close it when active, otherwise detach the
// helper widgets when the layout flag is set.
void func_80298450(CMCGetItemBox* self) {
    if (CSysWin_getUnk34(&self->sysWin_B8) != 0) {
        if (CSysWin_isActive(&self->sysWin_B8) != 0) {
            func_8022B8E4(&self->sysWin_B8);
            func_801D216C(&self->subObj_A0, 1);
        }
    } else {
        if (self->mField303 != 0) {
            self->mField303 = 0;
            func_801D216C(&self->subObj_58, 1);
            func_801D216C(&self->subObj_A0, 0);
            func_80138078(0x6);
        }
    }
}

// When the sys-win is both present and active, close it and reset the A0 widget.
void func_802984E4(CMCGetItemBox* self) {
    if (CSysWin_getUnk34(&self->sysWin_B8) == 0) return;
    if (CSysWin_isActive(&self->sysWin_B8) == 0) return;
    func_8022B8E4(&self->sysWin_B8);
    func_801D216C(&self->subObj_A0, 1);
}

// Unless the widget is busy and the sort field is -1, report the selected
// item-box entry's place relative to its full range.
u32 func_80298540(CMCGetItemBox* self) {
    if (self->field_4D == 0) return 0;
    if ((s8)self->field_301 == -1) {
        u16 count = self->sub_314.count;
        u16 v = (u16)func_80157CD0(self->sub_314.pad_102);
        return (v >= count) ? 1 : 2;
    }
    return 0;
}

// Visit every item-box entry and hand it to the C-linkage cleanup helper.
void func_802985B4(CMCGetItemBox* self) {
    CMCItemBoxSub* x = &self->sub_314;
    for (u32 i = 0; (u16)i < x->count; i++) {
        func_801599D4(func_80296DB0(x, (u16)i), 0);
    }
}

void func_8029860C(void* self) {}

// Advance the cursor / tab selection in the item box.
void func_80298614(CMCGetItemBox* self) {
    if (self->mField303 == 0) return;
    if (CSysWin_getUnk34(&self->sysWin_B8) != 0) {
        if (CSysWin_isActive(&self->sysWin_B8) != 0) {
            func_8022B8E4(&self->sysWin_B8);
            func_801D216C(&self->subObj_A0, 1);
        }
        return;
    }
    u8 combined = (u8)((s8)self->field_304 * 4 + self->field_305);
    if (ArrayGet12((const unsigned short*)&self->arr_1A4[0], combined) == 0) return;
    if (self->field_305 < 0xc) {
        if (self->arr_1A4[0xa8 + self->field_305] == 3) {
            func_801D3C74(&self->arr_1A4[0], self->field_305);
        }
    }
}

// Return a UI part index: 0 when the window exists, 0x36 when the layout flag
// is set, otherwise 0x39/0x3A based on the sign of the sort field.
u32 func_80298850(CMCGetItemBox* self) {
    if (CSysWin_getUnk34(&self->sysWin_B8) != 0) return 0;
    if (self->mField303 != 0) return 0x36;
    return 0x39 + ((s8)self->field_301 == -1);
}

void func_802988BC(){}
void func_80298938(){}
void func_802989A4(){}
void func_80298A20(){}
void func_80298A78(){}

// nw4r ArcResourceAccessor virtual GetResource at vtable[3] (offset 0x0C).
struct AccessorGetRes3 {
    virtual void _v00();
    virtual void _v04();
    virtual void _v08();
    virtual void* GetResource3(u32 tag, const char* name, int r6);
};

extern "C" void func_80298FB4(CMCGetItemBox* self, u32 idx, CMCItemBoxEntry* entry, u8 r6) {
    void* h = 0;
    if (entry != 0) {
        u32 type = (entry->field_00 >> 12) & 0xF;
        if (type == 3) {
            void* inst = CItem_initItemImplInstances(entry);
            h = 0;
            u32 k = (u32)((u32(*)(void*, void*))(*(void***)inst)[2])(inst, entry);
            switch ((u16)k) {
                case 1: h = func_80138F78(0x197); break;
                case 2: h = func_80138F78(0x196); break;
                case 3: h = func_80138F78(0x195); break;
                case 4: h = func_80138F78(0x194); break;
                case 5: h = func_80138F78(0x193); break;
                case 6: h = func_80138F78(0x192); break;
            }
            if (h != 0)
                h = ((AccessorGetRes3*)self->arcAcc2)->GetResource3(0x74696d67u, (const char*)h, 0);
            else
                h = ((AccessorGetRes3*)self->arcAcc1)->GetResource3(0x74696d67u, &lbl_eu_8050FF8C[0x131], 0);
        } else if (type == 9) {
            void* inst = CItem_initItemImplInstances(entry);
            u32 k = (u32)((u32(*)(void*, void*))(*(void***)inst)[0x20])(inst, entry);
            if ((u16)k != 0) {
                // L_8029B95C: item present.
                void* inst2 = CItem_initItemImplInstances(entry);
                h = 0;
                u32 k2 = (u32)((u32(*)(void*, void*))(*(void***)inst2)[2])(inst2, entry);
                switch ((u16)k2) {
                    case 1: h = func_80138F78(0x19c); break;
                    case 2: h = func_80138F78(0x19b); break;
                    case 3: h = func_80138F78(0x19a); break;
                    case 4: h = func_80138F78(0x199); break;
                    case 5: h = func_80138F78(0x198); break;
                }
                if (h != 0)
                    h = ((AccessorGetRes3*)self->arcAcc2)->GetResource3(0x74696d67u, (const char*)h, 0);
                else
                    h = ((AccessorGetRes3*)self->arcAcc1)->GetResource3(0x74696d67u, &lbl_eu_8050FF8C[0x131], 0);
            } else {
                // L_8029B858 shared block: item slot valid (gem-type chain 0x191..).
                if (func_801C6E90(entry) == 0) {
                    u32 ex = (entry->field_00 >> 12) & 0xF;
                    int f = 0;
                    if (ex == 9 && (entry->bytes[3] & 3) == 1) f = 1;
                    if (f != 0) goto L858;
                    if (ex != 9) goto after;
                }
            L858:
                {
                    void* inst2 = CItem_initItemImplInstances(entry);
                    h = 0;
                    u32 k2 = (u32)((u32(*)(void*, void*))(*(void***)inst2)[2])(inst2, entry);
                    switch ((u16)k2) {
                        case 1: h = func_80138F78(0x191); break;
                        case 2: h = func_80138F78(0x190); break;
                        case 3: h = func_80138F78(0x18f); break;
                        case 4: h = func_80138F78(0x18e); break;
                        case 5: h = func_80138F78(0x18d); break;
                    }
                    if (h != 0)
                        h = ((AccessorGetRes3*)self->arcAcc2)->GetResource3(0x74696d67u, (const char*)h, 0);
                    else
                        h = ((AccessorGetRes3*)self->arcAcc1)->GetResource3(0x74696d67u, &lbl_eu_8050FF8C[0x131], 0);
                }
            }
        }
    }
after:
    if (h == 0) {
        if (idx != 0) {
            char* name = func_80138F78((u16)func_80136254((void*)lbl_eu_806640EC, &lbl_eu_8050FF8C[0x15c], (u16)idx));
            h = ((AccessorGetRes3*)self->arcAcc2)->GetResource3(0x74696d67u, name, 0);
            if (h == 0)
                h = ((AccessorGetRes3*)self->arcAcc1)->GetResource3(0x74696d67u, &lbl_eu_8050FF8C[0x131], 0);
        } else {
            h = ((AccessorGetRes3*)self->arcAcc1)->GetResource3(0x74696d67u, &lbl_eu_8050FF8C[0x131], 0);
        }
    }
    if (h != 0) {
        char buf[0x20];
        sprintf(buf, &lbl_eu_8050FF8C[0x161], (int)(r6 + 1));
        func_80137E7C((void*)self->layout40, buf, (void*)h);
    }
}
void func_80299490(CMCGetItemBox* self, int r4, u32 r5) {}

// Refresh layout text after index/sort change.
void func_80299530(CMCGetItemBox* self, u16 arg) {
    CMCItemBoxSub* sub = &self->sub_314;
    func_80296BF0(sub);
    u8 count = sub->limit;
    if (count == 0) count = 1;
    func_80136910((nw4r::lyt::Layout*)self->layout40, &lbl_eu_8050FF8C[0x17b], count);
    nw4r::lyt::Pane* root = *(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10);
    if (arg == 9) {
        func_80124270(root->FindPaneByName(&lbl_eu_8050FF8C[0x184], true), 0);
        func_80124270(root->FindPaneByName(&lbl_eu_8050FF8C[0x18f], true), 1);
    } else {
        func_80124270(root->FindPaneByName(&lbl_eu_8050FF8C[0x184], true), 1);
        func_80124270(root->FindPaneByName(&lbl_eu_8050FF8C[0x18f], true), 0);
    }
    func_8029967C(self);
    func_802998C8(self);
    if (getItemBoxState((CItemBoxInfo*)self->itemBox) != 0) {
        func_801D4260((void*)self->itemBox, arg);
    }
}

void func_8029967C(CMCGetItemBox* self) {
    CMCItemBoxSub* sub = &self->sub_314;
    u8 count = sub->limit;
    if (count == 0) count = 1;
    if (count <= 1) return;
    nw4r::lyt::Layout* layout = self->layout40;
    nw4r::lyt::Pane* root = *(nw4r::lyt::Pane**)((u8*)layout + 0x10);
    func_80124270(root->FindPaneByName(&lbl_eu_8050FF8C[0x19a], true), 1);
    for (u8 i = 0; i < 3; i++) {
        char buf[0x20];
        sprintf(buf, &lbl_eu_8050FF8C[0x1a1], (u8)(i + 1));
        nw4r::lyt::Pane* pane = root->FindPaneByName(buf, true);
        func_80124270(pane, ((u8)(i - count)) >> 31);
    }
    func_80136910((nw4r::lyt::Layout*)layout, &lbl_eu_8050FF8C[0x1b0], (u8)(sub->counter + 1));
    root = *(nw4r::lyt::Pane**)((u8*)layout + 0x10);
    func_80124270(root->FindPaneByName(&lbl_eu_8050FF8C[0x1b9], true), 0);
    for (u8 i = 0; i < 0x1e; i++) {
        func_80298AC8(self, func_80296D54(sub, i) & 0xffff, func_80296DB0(sub, i), i);
        func_80298FB4(self, func_80296D54(sub, i) & 0xffff, func_80296DB0(sub, i), i);
        func_80299490(self, (s8)func_80296E00(sub, i), i);
    }
}

void func_802998C8(CMCGetItemBox* self) {}

// Refresh the cursor widgets / page label.
void func_802999B0(CMCGetItemBox* self) {
    if ((s8)self->field_301 == -1) {
        nw4r::lyt::Pane* root = *(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10);
        void* p1 = root->FindPaneByName(&lbl_eu_8050FF8C[0x1dc], true);
        void* p2 = root->FindPaneByName(&lbl_eu_8050FF8C[0x1e9], true);
        u8 tmp[0x20];
        func_80137924(tmp, p1, p2, root);
        (*(void(**)(void*, void*))((void**)&self->subObj_88)[4])(&self->subObj_88, tmp);
        func_801D216C(&self->subObj_88, 1);
        func_801D216C(&self->subObj_58, 0);
    } else {
        char buf[0x20];
        sprintf(buf, &lbl_eu_8050FF8C[0x161], (s8)self->field_300 + (s8)self->field_301 * 10 + 1);
        nw4r::lyt::Pane* root = *(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10);
        void* p1 = root->FindPaneByName(buf, true);
        void* p2 = root->FindPaneByName(&lbl_eu_8050FF8C[0x1e9], true);
        u8 tmp[0x20];
        func_80137924(tmp, p1, p2, root);
        (*(void(**)(void*, void*))((void**)&self->subObj_58)[4])(&self->subObj_58, tmp);
        func_801D216C(&self->subObj_88, 0);
        func_801D216C(&self->subObj_58, 1);
    }
}

void CMCGetItemBox::OnFileEvent() {}