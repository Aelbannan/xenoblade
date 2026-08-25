// Auto-scaffolded catalog TU for kyoshin/CItemBoxLine
// High-level C/C++ reconstruction from retail PPC.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CItemBoxLine.hpp"
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_pane.h>
#include "monolib/work/CEventFile.hpp"

// Tab entry accessors (retail symbols are plain func_XXXX names).
u8 func_801EF034(const CIBLTabCur* self, unsigned int index);
u16 func_801EC3B0(const CIBLTab* self, unsigned int index);
extern "C" void func_801EBB9C(CIBLTabEntry* dest, const CIBLTabEntry* src);
extern "C" CIBLTabEntry* func_801EBB88(CIBLTabEntry* self, u16 r4, u32 r5, u8 r6, u8 r7);
u8 func_801EC23C(const CIBLTab* self, unsigned int index);
u8 func_801EC8B4(const CIBLTab* self, unsigned int index);
void func_801EDA08(CItemBoxLine* self);
void func_801F1E64(CItemBoxLine* self, u32 itemData);
void func_801F20F0(CItemBoxLine* self, u32 itemData);
void func_801F2298(CItemBoxLine* self, u32 itemData);
int func_801F2880(u32 unused, u32 key);

void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
int sprintf(char*, const char*, ...);
extern "C" void func_80136910__FPQ34nw4r3lyt6LayoutPcUc(nw4r::lyt::Layout*, char*, u32);
extern "C" void func_801375A0(nw4r::math::VEC3* output, nw4r::lyt::Pane* pane);

// ============================================================================
// func_801ED774: busy-guard chain - only read the selector byte when every
// sub-system (info2 state, num-select, scrollbar, syswin) reports active/ready.
// ============================================================================
u8 func_801ED774(void* self) {
    unsigned char* p = (unsigned char*)self;
    if (getItemBox2State__FP13CItemBoxInfo2(p + 0xd0) == 0) return 0;
    if (func_801EB018(p + 0x2dc) == 0) return 0;
    if (CScrollBar_isVisible(p + 0x310) == 0) return 0;
    if (CSysWin_isReady(p + 0x350) != 0) return p[0x58];
    return 0;
}

// ============================================================================
// func_801ED97C: leave state-3 - store 4, reset byte flag, quiet cursors,
// advance the item-box state machine, refresh scrollbar, and beep if armed.
// ============================================================================
void func_801ED97C(void* self) {
    unsigned char* p = (unsigned char*)self;
    if (*(int*)(p + 0x50) != 3) return;
    *(unsigned int*)(p + 0x50) = 4;
    p[0x59] = 0;
    func_801D216C(p + 0xb8, 0);
    func_801D216C(p + 0x70, 0);
    func_801D216C(p + 0xa0, 0);
    advanceItemBox2State__FP13CItemBoxInfo2(p + 0xd0);
    func_801F369C(p + 0x310);
    if (!p[0x39e]) func_80138078__FUl(6);
}

// ============================================================================
// func_801EF050: invoke the tab-left selection refresh if nothing is busy.
// ============================================================================
void func_801EF050(void* self) {
    unsigned char* p = (unsigned char*)self;
    if (*(short*)(p + 0x38c) == -1) return;
    if (func_801EB020(p + 0x2dc)) return;
    if (CSysWin_getUnk34(p + 0x350)) return;
    if (p[0x3a0]) return;
    *(short*)(p + 0x38c) = -1;
    *(short*)(p + 0x38e) = 0;
    func_801F071C(self);
    func_801F0030(self);
    func_801F0488(self);
    func_801F3850(p + 0x310, *(unsigned short*)(p + 0x38e));
    func_80138078__FUl(2);
}

u8 func_801ED800(void* self) { return static_cast<CItemBoxLine*>(self)->unk59; }

// ============================================================================
// func_801EBB9C: copy a 12-byte tab entry. extern "C" + noinline: the retail
// name is unmangled and func_801EBAD4's calls are external relocs (see the
// func_801EBB88 stub note).
// ============================================================================
extern "C" __declspec(noinline) void func_801EBB9C(CIBLTabEntry* dest, const CIBLTabEntry* src) {
    dest->f0 = src->f0;
    dest->f4 = src->f4;
    dest->f8 = src->f8;
    dest->f9 = src->f9;
}

// ============================================================================
// func_801EBC00: (re)build the tab list. Clears the entries, then either probes
// the item-name tables per tab (field93 != 1) or walks the two-slot name tables
// (field93 == 1), appending each hit as a tab entry with its availability flags
// (f8 from the page-count overflow, f9 from the kind-owner lookups). The
// definition inherits C linkage from the header's extern "C" declaration.
// ============================================================================
void func_801EBC00(CIBLTab* self, u8 arg2, u16 arg3, u8 arg4) {
    ml::FixStr<32> str;
    self->count = 0;
    self->field92 = arg2;
    CIBLTabEntry tmp;
    for (u16 i = 0; i < 12; i++) {
        func_801EBB9C(&self->entries[i], func_801EBB88(&tmp, 0, 0, 0, 0));
    }
    if (self->field93 != 1) {
        // Item-name probe path: per-tab format table, then a 12-entry scan.
        struct TabFmtTable { const char* v[14]; };
        TabFmtTable fmt = *(const TabFmtTable*)&lbl_eu_80506D90[0];
        u32 cur = (u32)func_801571FC();
        for (u16 i = 1; i <= 12; i++) {
            str.format(&lbl_eu_805071B0[0], fmt.v[self->field92], i);
            u16 result = func_8013606C(&lbl_eu_805071B0[5], str.mString, arg3);
            if (result == 0) return;
            CIBLTabEntry* entry = &self->entries[self->count++];
            entry->f0 = result;
            func_801393CC(result);
            func_801392E4(result);
            func_80139358(result);
            entry->f4 = func_801EC9E0(self, result);
            if (entry->f4 > cur) entry->f8 = 1;
            if (arg4 != 0) {
                if (self->field92 == 0xd) {
                    if (func_801361E8(lbl_eu_80664110, &lbl_eu_805071B0[0xe],
                                      func_80139358(result)) != 0) {
                        entry->f9 = 1;
                    }
                } else {
                    u32 t = 0;
                    switch (self->field92) {
                    case 2: t = lbl_eu_806640F4; break;
                    case 4: case 5: case 6: case 7: case 8: t = lbl_eu_806640F8; break;
                    }
                    if (self->field92 == 2) {
                        str.format(&lbl_eu_805071B0[0x16], arg4);
                    } else {
                        str.format(&lbl_eu_805071B0[0x21], arg4);
                    }
                    if (func_801361E8(t, str.mString, func_80139358(result)) != 0) {
                        entry->f9 = 1;
                    }
                    if (self->field92 != 2) {
                        u8 v = (u8)func_801361E8(lbl_eu_806640F8, &lbl_eu_805071B0[0x26],
                                                 func_80139358(result));
                        u32 base = (u32)func_8009EC9C(arg4) + 0x3534;
                        if (v == 3) {
                            if (func_8026178C(base, 0x85) == 0) entry->f9 = 0;
                        } else if (v == 2) {
                            if (func_8026178C(base, 0x84) == 0) entry->f9 = 0;
                        }
                    }
                }
            }
        }
    } else {
        // Name-table path: copy the name list, resolve the file via the name
        // index byte, then walk the two kind-owner pairs.
        struct TabNameTable { const char* v[27]; };
        TabNameTable names = *(const TabNameTable*)&lbl_eu_80506D90[0x240];
        u8 n = (u8)lbl_eu_80664184;
        func_8003AA34();
        void* fp = getFP__FPCc(names.v[n - 1]);
        u8 v = (u8)func_801372B4(n);
        u16 id = func_8009CF8C(v + 0x21);
        u8 r26v;
        if (id >= 0x1f40) {
            r26v = (u8)func_801361E8(lbl_eu_80664098, &lbl_eu_805071B0[0x2f], arg3);
        } else if (id >= 0x1770) {
            r26v = (u8)func_801361E8(lbl_eu_80664098, &lbl_eu_805071B0[0x37], arg3);
        } else if (id >= 0xfa0) {
            r26v = (u8)func_801361E8(lbl_eu_80664098, &lbl_eu_805071B0[0x3f], arg3);
        } else if (id >= 0x7d0) {
            r26v = (u8)func_801361E8(lbl_eu_80664098, &lbl_eu_805071B0[0x47], arg3);
        } else {
            r26v = (u8)func_801361E8(lbl_eu_80664098, &lbl_eu_805071B0[0x4f], arg3);
        }
        struct TabKindTable { const char* v[28]; };
        TabKindTable kinds = *(const TabKindTable*)&lbl_eu_80506D90[0x2f8];
        func_801571FC();
        for (int j = 0; j < 2; j++) {
            if (kinds.v[self->field92 * 2 + j] == 0) return;
            u16 result = func_80136254((u32)fp, kinds.v[self->field92 * 2 + j], r26v);
            if (result == 0) return;
            CIBLTabEntry* entry = &self->entries[self->count++];
            entry->f0 = result;
            func_801393CC(result);
            func_801392E4(result);
            func_80139358(result);
            entry->f4 = func_801EC9E0(self, result);
            if (arg4 != 0) {
                if (self->field92 == 3) {
                    entry->f9 = 1;
                } else if (self->field92 <= 8) {
                    u32 t = 0;
                    switch (self->field92) {
                    case 2: t = lbl_eu_806640F4; break;
                    case 4: case 5: case 6: case 7: case 8: t = lbl_eu_806640F8; break;
                    }
                    if (self->field92 == 2) {
                        str.format(&lbl_eu_805071B0[0x16], arg4);
                    } else {
                        str.format(&lbl_eu_805071B0[0x21], arg4);
                    }
                    if (func_801361E8(t, str.mString, func_80139358(result)) != 0) {
                        entry->f9 = 1;
                    }
                    if (self->field92 != 2) {
                        u8 w = (u8)func_801361E8(lbl_eu_806640F8, &lbl_eu_805071B0[0x26],
                                                 func_80139358(result));
                        u32 base = (u32)func_8009EC9C(arg4) + 0x3534;
                        if (w == 3) {
                            if (func_8026178C(base, 0x85) == 0) entry->f9 = 0;
                        } else if (w == 2) {
                            if (func_8026178C(base, 0x84) == 0) entry->f9 = 0;
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

// func_801EC23C: u8 at +0x8
u8 func_801EC23C(const CIBLTab* self, unsigned int index) {
    if (index < self->count) {
        return self->entries[index].f8;
    }
    return 0;
}

// func_801EC260: u32 at +0x4
u32 func_801EC260(const CIBLTab* self, unsigned int index) {
    if (index < self->count) {
        return self->entries[index].f4;
    }
    return 0;
}

// func_801EC3B0: u16 at +0x0
u16 func_801EC3B0(const CIBLTab* self, unsigned int index) {
    if (index < self->count) {
        return self->entries[index].f0;
    }
    return 0;
}

// func_801EC8B4: u8 at +0x9 (default 1)
// noinline: retail calls this accessor out-of-line from func_801ED864/
// func_801F0488; -ipa file would otherwise fold it into those callers.
__declspec(noinline) u8 func_801EC8B4(const CIBLTab* self, unsigned int index) {
    if (index < self->count) {
        return self->entries[index].f9;
    }
    return 1;
}

// ============================================================================
// CIBLTabCur vtable plumbing (retail ctor/dtor symbols are plain names)
// ============================================================================

// func_801EE840 / __ct__CIBLTabCur (accessor arg unused by the body, but the
// retail caller passes the arc accessor in r4). noinline: retail calls it as an
// external reloc; without it MWCC -inline auto inlines the same-TU body.
extern "C" __declspec(noinline) CBaseCur* __ct__CIBLTabCur(CBaseCur* _this, void* accessor) {
    __ct__8CBaseCurFv(_this);
    _this->mVtable = (void*)lbl_eu_80534D80;
    return _this;
}

// func_801ED7BC / __dt__801EBBC0 (delete-only dtor)
extern "C" void* __dt__801EBBC0(CBaseCur* _this, int flags) {
    if (_this != NULL && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

// func_801EE87C / __dt__801ECBC0
// optimize_for_size merges the r30/r31 saves into stmw/lmw + fixes the copy
// order (CTagProcessor dtor family pattern).
#pragma optimize_for_size on
extern "C" void* __dt__801ECBC0(CBaseCur* _this, int flags) {
    if (_this != NULL) {
        __dt__8CBaseCurFv(_this, 0);
        if (flags > 0) {
            __dl__FPv(_this);
        }
    }
    return _this;
}
#pragma optimize_for_size off

// ============================================================================
// func_801EF034: byte at this+0xA8, index bounded to a 12-entry table
// ============================================================================
u8 func_801EF034(const CIBLTabCur* self, unsigned int index) {
    if (index < 12u) {
        return self->unkA8[index];
    }
    return 0;
}

// ============================================================================
// func_801EDA08: reset tab byte entries + two counters
// ============================================================================
void func_801EDA08(CItemBoxLine* self) {
    memset(self->tabEntries, 0, 9);
    self->tabCount = 0;
    self->field6D = 0;
}

// ============================================================================
// func_801EC3D0: format the entry index's name into the trailing FixStr<32>
// buffer (pool+0x57 "%s"); null if the index is out of range.
// ============================================================================
ml::FixStr<32>* func_801EC3D0(CIBLTabFormat* self, unsigned int index) {
    if (index < self->count) {
        self->str94.format(&lbl_eu_805071B0[0x57], func_801394D4(self->entries[index].f0));
        return &self->str94;
    }
    return 0;
}

// ============================================================================
// func_801EC438
// ============================================================================
// ============================================================================
// func_801EC438: build the tab entry's display name into the trailing
// FixStr<128> buffer (+0xB8) and return it. Selector 3 composes a base name
// from the kind-owner tables and then rewrites "$1"/"$2" placeholders in
// place from per-kind name parts (Shift-JIS aware: lead bytes 0x81-0x9F /
// 0xE0-0xEF advance by 2); any other selector resolves the display name
// through the kind-owner tables directly.
// ============================================================================
extern "C" __declspec(noinline) char* func_801EC438(CIBLTabFull* self, u16 index) {
    if (index >= self->count) return NULL;
    u16 f0 = self->entries[index].f0;
    if (f0 == 0) return NULL;
    char* pool = lbl_eu_805071B0;
    u8 sel = self->field92;
    if (sel == 3) {
        func_801392E4(f0);
        u16 kind = func_80139358(f0);
        u8 n = func_801361E8((u32)lbl_eu_806640EC, pool + 0x5a, f0);
        ml::FixStr<32> partName(true);
        partName.format(pool + 0x57, func_80136190(pool + 0x63, pool + 0x6c, n));
        u8 sub = func_801361E8((u32)lbl_eu_806640D8, pool + 0x71, kind);
        // NOTE: retail sets r5 (kind) for this call, but the shared 2-arg
        // declaration is kept to avoid changing the sibling call's codegen.
        char* base = func_8013639C((void*)lbl_eu_806640D8, pool + 0x7a);
        self->strB8.format(pool + 0x57, base);
        // Only some languages keep the full-width decoration around $-parts.
        int full = 1;
        if ((u8)getLanguage__9CDeviceSCFv() == 3 || (u8)getLanguage__9CDeviceSCFv() == 2) {
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
                    case 4:
                        sprintf(tmp, pool + 0x86, partName.mString);
                        break;
                    case 3:
                        sprintf(tmp, full ? pool + 0x9c : pool + 0xb5, partName.mString);
                        break;
                    }
                } else if (d == '2') {
                    sprintf(tmp, pool + 0x86, partName.mString);
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
        u32 table = lbl_eu_8066464C;
        if ((u8)(sel + 0xfc) > 5 && sel != 2) {
            table = lbl_eu_80664648;
        }
        u16 v = func_80136254((u32)lbl_eu_806640EC, pool + 0xfe, f0);
        // NOTE: retail sets r5 (v) for this call; see 2-arg note above.
        char* name = func_8013639C((void*)table, pool + 0xfe);
        self->strB8.format(pool + 0x57, name);
    }
    return &self->strB8.mString[0];
}

void func_801EC808(CIBLTabFull* self, u32 index, u32 count) {
    if (index >= self->count) return;
    CIBLTabEntry* entry = &self->entries[index];
    func_801393CC(entry->f0);
    func_801392E4(entry->f0);
    func_80139358(entry->f0);
    if (self->field92 != 0xd) {
        for (u8 i = 0; i < count; i++) {
            func_801586D4(entry->f0, 1);
        }
    } else {
        func_801586D4(entry->f0, count);
    }
    func_80157184((s32)((u32)func_801571FC() - entry->f4 * count));
}

// ============================================================================
// func_801EC8D8: name-dispatch lookup. Bounds-check the tab entry, then by the
// selector byte search the kind-owner table (lbl_eu_806640F4 for selector 2,
// lbl_eu_806640F8 otherwise) for the entry's category key: probe slot names
// "%s%u" until func_80136254 finds a match (0xFD), exhaust the count (0xFE),
// or bail with 0. Selector 0xD (name-format mode) returns 0 via its own tail.
// The goto chain reproduces retail's test ladder: the search body sits in the
// fall-through after the tests, with both ret-0 tails kept separate.
// ============================================================================
int func_801EC8D8(CIBLTabFull* self, u32 index) {
    u16 f0;
    u16 v;
    u8 sel;
    if (index >= self->count) goto ret0;
    f0 = self->entries[index].f0;
    if (f0 == 0) goto ret0;
    v = func_80139358(f0);
    sel = self->field92;
    if ((u32)(sel - 4) <= 4) goto search;
    if ((int)sel == 2) goto search;
    if ((int)sel == 0xd) goto retSel13;
    goto ret0;
search:
    {
        u32 obj;
        if ((u32)sel == 2) {
            obj = lbl_eu_806640F4;
        } else {
            obj = lbl_eu_806640F8;
        }
        u8 n = func_801361E8(obj, &lbl_eu_805071B0[0x106], (u16)v);
        if (n == 0) goto ret0;
        char buf[0x20];
        for (u8 i = 0; i < n; i++) {
            sprintf(buf, &lbl_eu_805071B0[0x10f], (u8)i + 1);
            if ((u16)func_80136254(obj, buf, (u16)v) != 0) return 0xfd;
        }
        return 0xfe;
    }
retSel13:
    return 0;
ret0:
    return 0;
}

// ============================================================================
// func_801EC9E0: per-item display-count key (same shape as the equip item
// box's func_80283B60). Category band selects the computation - cat 3 scales
// the bdat count by a per-item float record, cats 2-8 accumulate a per-count
// increment from two sdata2 floats, everything else passes the raw count
// through. Result is returned *10.
// ============================================================================
extern "C" u32 __cvt_fp2unsigned(double);
extern "C" void func_801EB218(void*);
extern const float lbl_eu_80668118;
/* 24-byte float record copied off lbl_eu_8050715C by func_801EC9E0 and read
   back indexed by the owner lookup byte (mask allows past the end on
   purpose, same as the equip item box's F32Record idiom). */
union CIBLF32Rec {
    u32 w[6];
    float f[6];
};
extern const CIBLF32Rec lbl_eu_8050715C;
#pragma push
#pragma optimize_for_size on
extern "C" u32 func_801EC9E0(void* self, unsigned int itemData) {
    u32 itemId = func_801393CC(itemData);
    u32 kind = func_801392E4(itemData);
    u16 cat = func_80139358(itemData);
    char* pool = lbl_eu_805071B0;
    u32 result = 0;
    if ((u16)kind == 3) {
        u32 v = func_801361E8((u32)lbl_eu_806640EC, pool + 0x5a, itemData);
        u16 count = func_80136254(itemId, pool + 0x11b, cat);
        CIBLF32Rec rec = lbl_eu_8050715C;
        result = __cvt_fp2unsigned((double)((float)count * rec.f[v & 0xFF]));
    } else if ((u16)kind == 9 || (u16)kind < 2 || (u16)kind > 8) {
        u16 count = func_80136254(itemId, pool + 0x11b, cat);
        result = count;
    } else {
        u16 count = func_80136254(itemId, pool + 0x11b, cat);
        u32 n = func_801361E8(itemId, pool + 0x106, cat);
        float inc = lbl_eu_80668114;
        float step = lbl_eu_80668118;
        do {
            inc += step;
        } while ((u8)n-- != 0);
        result = __cvt_fp2unsigned((double)((float)count * inc));
    }
    return result * 10;
}
#pragma pop

// ============================================================================
// func_801ECC10: build the item-box line layout - load the layout arc, bind
// the two animation transforms, stop all running animations, then run the
// cursor deactivation tail.
// ============================================================================
void func_801ECC10(CItemBoxLine* self) {
    char* pool = lbl_eu_805071B0;
    func_80136E84(&self->field08, self->field04, pool + 0x121);
    func_80136F08(self->field08, &self->field0C, self->field04, pool + 0x139);
    func_80136F08(self->field08, &self->field10, self->field04, pool + 0x156);
    self->field08->UnbindAllAnimation();
    func_801D21CC(self);
}

void __ct__CItemBoxLine(){}

// ============================================================================
// CItemBoxLine destructor (retail __dt__12CItemBoxLineFv): destroy the
// embedded sub-objects in reverse construction order. The +0xA0 cursor is
// destroyed with the 0 delete-flag through a null guard (MWCC D2-into-D1
// artifact, same shape as ~COption's mSubCur2); the rest use the -1
// embedded-member flag. MWCC supplies the this-null check, the flags>0
// delete guard and the stmw/lmw frame from the dtor shape itself.
// ============================================================================
#pragma push
#pragma optimize_for_size on
CItemBoxLine::~CItemBoxLine() {
    __dt__7CSysWinFv(reinterpret_cast<CSysWin*>(&mSysWin), -1);
    __dt__10CScrollBarFv(reinterpret_cast<CScrollBar*>(&mScrollBar310[0]), -1);
    __dt__10CNumSelectFv(reinterpret_cast<CNumSelect*>(&mNumSel), -1);
    __dt__13CItemBoxInfo2Fv(reinterpret_cast<CItemBoxInfo2*>(&mInfo2D0[0]), -1);
    __dt__7CSubCurFv(&mCurB8, -1);
    CBaseCur* curA0 = &mCurA0;
    if (curA0 != 0) {
        __dt__8CBaseCurFv(curA0, 0);
    }
    __dt__6CCur18Fv(&mCur88, -1);
    __dt__6CCur18Fv(&mCur70, -1);
    __dt__17UnkClass_8045F564Fv(reinterpret_cast<UnkClass_8045F564*>(&pad_14[0]), -1);
    __dt__17UnkClass_8045F564Fv(reinterpret_cast<UnkClass_8045F564*>(&field04), -1);
}
#pragma pop

// ============================================================================
// func_801ED31C: load the item-box line's four files (MEM2 handle via
// getHandleMEM2, the third through the common archive) with this as the load
// event, then initialise the info2 state, num-select and scroll-bar members,
// and finally dispatch the syswin's last vtable slot (index 32) to finish.
// ============================================================================
#pragma push
#pragma optimize_for_size on
void func_801ED31C(CItemBoxLine* self) {
    self->field24 = (u32)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (u32)getHandleMEM2__Q23mtl10MemManagerFv(), &lbl_eu_805071B0[0x171], self, 0, 0);
    self->field28 = (u32)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (u32)getHandleMEM2__Q23mtl10MemManagerFv(), &lbl_eu_805071B0[0x189], self, 0, 0);
    self->field2C = (u32)readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (u32)func_800A9D90(), &lbl_eu_805071B0[0x1a2], self, 0, 0);
    self->field30 = (u32)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (u32)getHandleMEM2__Q23mtl10MemManagerFv(), &lbl_eu_805071B0[0x1bf], self, 0, 0);
    func_801E12E0(&self->mInfo2D0[0]);
    func_801EAE8C(&self->mNumSel);
    func_801F34F4(&self->mScrollBar310[0]);
    reinterpret_cast<CSysWinVtblView*>(&self->mSysWin)->vf32();
}
#pragma pop

void func_801ED3E8(CItemBoxLine* self) {
    if (self->field4C == 0) return;
    switch (self->field50) {
    case 0: func_801EF1E4((void*)self); break;
    case 1: func_801EF260((void*)self); break;
    case 2: func_801EF2FC((void*)self); break;
    case 3: func_801EF378((void*)self); break;
    case 4:
        if (func_801EB028(&self->mNumSel)) {
            self->field50 = 7;
        }
        break;
    case 5:
        if (func_801EB028(&self->mNumSel)) {
            self->field50 = 3;
        }
        break;
    case 6: func_801EF3E8((void*)self); break;
    // states 7..0xB share the syswin-commit body and then fall through into
    // the per-frame tail (the switch exit is the tail start in retail).
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        func_801EF45C((void*)self);
    }
    self->field40->Animate(0);
    func_801D202C(&self->mCur70);
    func_801D202C(&self->mCur88);
    func_801D202C(&self->mCurA0);
    func_801D202C(&self->mCurB8);
    func_801E1348(&self->mInfo2D0[0]);
    func_801EAED4(&self->mNumSel);
    func_801F3540(&self->mScrollBar310[0]);
    func_8022B748(&self->mSysWin);
}

#pragma push
#pragma optimize_for_size on
void func_801ED4FC(CItemBoxLine* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->field4C == 0) return;
    drawItemBox2Layout__FP13CItemBoxInfo2PQ34nw4r3lyt8DrawInfo(&self->mInfo2D0[0], drawInfo);
    func_80137038(self->field40, drawInfo, 0, 1);    // "line not busy" flag: both the tab-name cursor and the num-select idle.
    int noBusy = 0;
    if (func_801D2ED8(&self->mCurB8) == 0 && func_801EB020(&self->mNumSel) == 0) noBusy = 1;
    if (noBusy != 0) {
        if (CSysWin_getUnk34(&self->mSysWin) == 0) {
            func_801D20B0(&self->mCur88, drawInfo);
            func_801D20B0(&self->mCurA0, drawInfo);
        }
    }
    func_801D20B0(&self->mCurB8, drawInfo);
    func_801F35B0(&self->mScrollBar310[0], drawInfo);
    func_801EAF7C(&self->mNumSel, drawInfo);
    func_8022B7C8(&self->mSysWin, drawInfo);
    // "syswin overlay armed" flag: overlay busy and nav state before 9.
    int syswinArmed = 0;
    if (CSysWin_getUnk34(&self->mSysWin) != 0 && self->field50 < 9) syswinArmed = 1;
    if (noBusy != 0 && syswinArmed == 0) {
        func_801D20B0(&self->mCur70, drawInfo);
    }
}
#pragma pop

// ============================================================================
// func_801ED618: teardown of the loaded item-box line. Stop the two UI sounds,
// release the four file handles, free the scratch buffer and the two heap
// objects (virtual deleting-dtor, double-null-checked), destroy the two arc
// resource accessors and the two 16-byte mem regions at +0x04/+0x14, silence
// the four cursors, then destroy the embedded sub-systems.
// ============================================================================
#pragma push
#pragma optimize_for_size on
void func_801ED618(CItemBoxLine* self) {
    CBdat::func_8003AA8C(2);
    CBdat::func_8003AA8C(5);
    func_801390E0(reinterpret_cast<CFileHandle**>(&self->field24));
    func_801390E0(reinterpret_cast<CFileHandle**>(&self->field28));
    func_801390E0(reinterpret_cast<CFileHandle**>(&self->field2C));
    func_801390E0(reinterpret_cast<CFileHandle**>(&self->field30));
    self->field4C = 0;
    if (self->field34 != 0) {
        mtl::MemManager::deallocate(self->field34);
        self->field34 = 0;
    }
    nw4r::lyt::Layout* layout40 = self->field40;
    if (layout40 != 0) {
        if (layout40 != 0) {
            reinterpret_cast<CItemBoxLineDtorView*>(layout40)->vfdtor(1);
        }
        self->field40 = 0;
    }
    func_80139124(reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->field38));
    func_80139124(reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->field3C));
    CItemBoxLineDtorView* obj54 = self->field54;
    if (obj54 != 0) {
        if (obj54 != 0) {
            obj54->vfdtor(1);
        }
        self->field54 = 0;
    }
    reinterpret_cast<UnkClass_8045F564*>(&self->field04)->func_8045F778();
    reinterpret_cast<UnkClass_8045F564*>(&self->pad_14[0])->func_8045F778();
    reinterpret_cast<CIBLCur70View*>(&self->mCur70)->vf01();
    reinterpret_cast<CIBLCur70View*>(&self->mCur88)->vf01();
    reinterpret_cast<CIBLCur70View*>(&self->mCurA0)->vf01();
    reinterpret_cast<CIBLCur70View*>(&self->mCurB8)->vf01();
    func_801E13F8(reinterpret_cast<CItemBoxInfo2*>(&self->mInfo2D0[0]));
    func_801EAF9C(&self->mNumSel);
    func_801F35DC(reinterpret_cast<CScrollBar*>(&self->mScrollBar310[0]));
    func_8022B7F4(&self->mSysWin);
}
#pragma pop


// ============================================================================
// func_801ED808: busy-check - a CSysWin or num-select member is active, else return byte.
// ============================================================================
u8 func_801ED808(CItemBoxLine* self) {
    if (CSysWin_getUnk34(&self->mSysWin)) {
        return 1;
    }
    if (func_801EB020(&self->mNumSel)) {
        return 1;
    }
    return self->field3A0;
}

#pragma push
#pragma optimize_for_size on
void func_801ED864(CItemBoxLine* self) {
    if (self->field50 != 0) return;
    self->field50 = 1;
    self->field4C = 1;
    self->unk59 = 0;
    self->unk39E = 0;
    func_801E1498(&self->mInfo2D0[0]);
    // Four values stay live across the calls below (self / tabs / idx / f9):
    // retail keeps them in r28..r31 behind a _savegpr_28 prologue.
    CIBLTab* tabs = &self->unk3A4;
    u8 idx = (u8)(self->unk38C + self->unk38E);
    int f9 = func_801EC8B4((void*)tabs, idx);
    func_801E14DC(&self->mInfo2D0[0], (u16)func_801EC3B0(tabs, idx), 0, self->field39F, f9);
    func_801E16F0(&self->mInfo2D0[0], 0, (char*)func_801EC3D0(tabs, idx));
    func_801F08B4(self, (u16)func_801EC3B0(tabs, idx));
    float vec[3];
    func_801F3670(&self->mScrollBar310[0],
                  (const float*)code80135FDC_setVec3(vec, lbl_eu_8066811C, lbl_eu_80668120, lbl_eu_806680F8));
    func_801F36BC(&self->mScrollBar310[0], 7, tabs->count);
    func_801F367C(&self->mScrollBar310[0]);
}
#pragma pop

void CItemBoxLine::func_801EDA4C(unsigned char val) {
    unsigned char n = reinterpret_cast<unsigned char*>(this)[0x63];
    if (n >= 9) {
        return;
    }
    *((unsigned char*)this + n + 0x5a) = val;
    reinterpret_cast<unsigned char*>(this)[0x63] = n + 1;
}

void func_801EDA6C(CItemBoxLine* self) {
    if (func_801EB020(&self->mNumSel)) return;
    self->field6D = self->field6D + 1;
    if ((s8)self->field6D >= (int)self->tabCount) {
        self->field6D = 0;
    }
    if (self->unk64[(s8)self->field6D] == 0) {
        u8 count = self->tabCount;
        int j = (s8)(self->field6D + 1);
        if ((s8)j >= (int)count) j = 0;
        for (u8 i = 0; i < count; i++) {
            if (self->unk64[(s8)j] != 0) {
                self->field6D = (u8)j;
                break;
            }
            j = j + 1;
            if ((s8)j >= (int)count) j = 0;
        }
    }
    self->unk38E = 0;
    func_801F061C((void*)self, 0);
    func_801EFFC4((void*)self);
    func_801F071C((void*)self);
    func_80138078__FUl(0x70);
    func_801F36BC(&self->mScrollBar310[0], 7, self->unk3A4.count);
}

void func_801EDB80(CItemBoxLine* self) {
    if (func_801EB020(&self->mNumSel)) return;
    self->field6D = self->field6D - 1;
    if ((s8)self->field6D < 0) {
        self->field6D = self->tabCount - 1;
    }
    if (self->unk64[(s8)self->field6D] == 0) {
        // j-init (with its own tabCount reload for the wrap) precedes the count
        // load, matching retail scheduling; 'last' is only referenced inside
        // the loop.
        int j = (s8)(self->field6D - 1);
        if ((s8)j < 0) j = (s8)(self->tabCount - 1);
        u8 count = self->tabCount;
        int last = count - 1;
        for (u8 i = 0; i < count; i++) {
            if (self->unk64[(s8)j] != 0) {
                self->field6D = (u8)j;
                break;
            }
            j = j - 1;
            if ((s8)j < 0) j = (s8)last;
        }
    }
    self->unk38E = 0;
    func_801F061C((void*)self, 1);
    func_801EFFC4((void*)self);
    func_801F071C((void*)self);
    func_80138078__FUl(0x70);
    func_801F36BC(&self->mScrollBar310[0], 7, self->unk3A4.count);
}

// ============================================================================
// func_801EDC94: tab page-up / cursor-back interaction - the mirror of
// func_801EDF40. Syswin overlay open (and nav state 9, active): step the
// overlay selection byte down (wrapping 0 -> 1) and push the syswin selection
// buffer through the line cursor's vtable[2]; tab cursor active: scan the
// page list backward for the previous occupied slot (wrap at 4) and push its
// VEC3 through mCur88; num-select busy: advance field392 (resetting to 1 past
// the entry's capacity) and repush value*step; otherwise page the tab line up
// by one row (wrapping through the tab count) and refresh the line.
// ============================================================================
#pragma push
#pragma optimize_for_size on
void func_801EDC94(CItemBoxLine* self) {
    if (CSysWin_getUnk34(&self->mSysWin) != 0) {
        if (self->field50 != 9) {
            if (CSysWin_isActive(&self->mSysWin) != 0) {
                u8 v = self->field3A3 - 1;
                self->field3A3 = v;
                if ((s8)v < 0) {
                    self->field3A3 = 1;
                }
                u8 tmp[12];
                func_8022C1B4(tmp, &self->mSysWin, self->field3A3);
                reinterpret_cast<CIBLCur70View*>(&self->mCur70)->vf02(tmp);
                func_80138078__FUl(1);
            }
        }
        return;
    }
    if (self->field3A0 != 0) {
        CIBLPageData* page = reinterpret_cast<CIBLPageData*>(&self->mInfo2D0[0xB0]);
        s8 a = (s8)(self->field3A2 - 1);
        while (a != (s8)self->field3A2) {
            if (a < 0) a = 3;
            if (ArrayGet12(page->pageWords180, (u8)((s8)self->field3A1 * 4 + a)) != 0) {
                self->field3A2 = (u8)a;
                break;
            }
            a--;
        }
        CIBLVec3 vec;
        func_801CB9D8(&vec, page->pageWords180, (u8)((s8)self->field3A1 * 4 + self->field3A2));
        reinterpret_cast<CIBLCur70View*>(&self->mCur88)->vf02(&vec);
    } else if (func_801EB020(&self->mNumSel) != 0) {
        if (func_801EB028(&self->mNumSel) == 0) return;
        u8 idx = (u8)(self->unk38C + self->unk38E);
        CIBLTab* tabs = &self->unk3A4;
        u16 lim = func_801EC284((void*)tabs, idx);
        self->field392 += 1;
        if ((u16)self->field392 > lim) self->field392 = 1;
        func_801EB218(&self->mNumSel);
        func_801EB04C(&self->mNumSel, (u8)self->field392);
        func_801EB064(&self->mNumSel, self->field392 * func_801EC260((void*)tabs, idx));
    } else {
        if (self->unk38C == 0 && self->unk38E == 0) {
            self->unk38C = -1;
        } else {
            self->unk38C--;
            if ((s16)self->unk38C < 0) {
                self->unk38C = 0;
                self->unk38E--;
                if ((s16)self->unk38E < 0) {
                    u16 count = self->unk3A4.count;
                    if (count >= 7) {
                        self->unk38C = 6;
                        self->unk38E = count - 7;
                    } else {
                        self->unk38C = count - 1;
                        self->unk38E = 0;
                        if ((s16)self->unk38C < 0) {
                            self->unk38C = 0;
                        }
                    }
                }
            }
        }
        func_801F071C(self);
        func_801F0030(self);
        func_801F0488(self);
        func_801F3850(&self->mScrollBar310[0], (u16)self->unk38E);
    }
    func_80138078__FUl(1);
}
#pragma pop

// ============================================================================
// func_801EDF40: tab page-down / tab-cursor interaction. When the syswin
// overlay is open and the nav state is in the overlay region (>= 9), advance
// the overlay selection byte (wrapping at 2) and push the syswin selection
// buffer through the cursor's vtable[2]; when the tab cursor is active, scan
// the page list for the previous occupied slot (wrap at 4) and push its VEC3;
// when the num-select is busy, step the page position down by 1 clamped to
// the entry's capacity; otherwise page the tab line down by 7 (wrap at the
// tab count) and refresh the line.
// ============================================================================
#pragma push
#pragma optimize_for_size on
void func_801EDF40(CItemBoxLine* self) {
    if (CSysWin_getUnk34(&self->mSysWin) != 0) {
        if (self->field50 >= 9) {
            if (CSysWin_isActive(&self->mSysWin) != 0) {
                self->field3A3++;
                if ((s8)self->field3A3 > 1) {
                    self->field3A3 = 0;
                }
                u8 tmp[12];
                func_8022C1B4(tmp, &self->mSysWin, self->field3A3);
                reinterpret_cast<CIBLCur70View*>(&self->mCur70)->vf02(tmp);
                func_80138078__FUl(1);
            }
        }
        return;
    }
    if (self->field3A0 != 0) {
        CIBLPageData* page = reinterpret_cast<CIBLPageData*>(&self->mInfo2D0[0xB0]);
        s8 a = (s8)(self->field3A2 + 1);
        while (a != (s8)self->field3A2) {
            if (a >= 4) a = 0;
            if (ArrayGet12(page->pageWords180, (u8)((s8)self->field3A1 * 4 + a)) != 0) {
                self->field3A2 = (u8)a;
                break;
            }
            a++;
        }
        CIBLVec3 vec;
        func_801CB9D8(&vec, page->pageWords180, (u8)((s8)self->field3A1 * 4 + self->field3A2));
        reinterpret_cast<CIBLCur70View*>(&self->mCur88)->vf02(&vec);
    } else if (func_801EB020(&self->mNumSel) != 0) {
        if (func_801EB028(&self->mNumSel) == 0) return;
        u8 idx = (u8)(self->unk38C + self->unk38E);
        CIBLTab* tabs = &self->unk3A4;
        s16 lim = (s16)func_801EC284((void*)tabs, idx);
        self->field392 -= 1;
        if ((s16)self->field392 < 1) self->field392 = lim;
        func_801EB314(&self->mNumSel);
        func_801EB04C(&self->mNumSel, (u8)self->field392);
        func_801EB064(&self->mNumSel, self->field392 * func_801EC260((void*)tabs, idx));
    } else {
        u16 count = self->unk3A4.count;
        if (count >= 7) {
            if (self->unk38C == 6 && self->unk38E == count - 7) {
                self->unk38C = -1;
                self->unk38E = 0;
            } else {
                self->unk38C++;
                if (self->unk38C >= 7) {
                    self->unk38C = 6;
                    self->unk38E++;
                    if (self->unk38E > count - 7) self->unk38E = count - 7;
                }
            }
        } else {
            int last = count - 1;
            if (self->unk38C == last) {
                self->unk38C = -1;
                self->unk38E = 0;
            } else {
                self->unk38C++;
                if (self->unk38C >= count) {
                    self->unk38C = last;
                    self->unk38E = 0;
                }
            }
        }
        func_801F071C(self);
        func_801F0030(self);
        func_801F0488(self);
        func_801F3850(&self->mScrollBar310[0], (u16)self->unk38E);
    }
    func_80138078__FUl(1);
}
#pragma pop

// ============================================================================
// func_801EE228: per-frame tab/cursor interaction. When the tab cursor is
// active, scan the page list for the next non-empty slot and push its VEC3
// to the cursor; when the num-select is busy, step its page position;
// otherwise handle the tab page up/down wrap and refresh the line.
// ============================================================================
#pragma push
#pragma optimize_for_size on
void func_801EE228(CItemBoxLine* self) {
    if (CSysWin_getUnk34(&self->mSysWin) != 0) return;
    if (self->field3A0 != 0) {
        CIBLPageData* page = reinterpret_cast<CIBLPageData*>(&self->mInfo2D0[0xB0]);
        s8 a = (s8)(self->field3A1 - 1);
        while (a != (s8)self->field3A1) {
            if (a < 0) a = 2;
            if (ArrayGet12(page->pageWords180, (u8)(self->field3A2 + (s8)a * 4)) != 0) {
                self->field3A1 = (u8)a;
                break;
            }
            a--;
        }
        CIBLVec3 vec;
        func_801CB9D8(&vec, page->pageWords180, (u8)(self->field3A2 + (s8)self->field3A1 * 4));
        reinterpret_cast<CIBLCur70View*>(&self->mCur88)->vf02(&vec);
        func_80138078__FUl(1);
        return;
    }
    if (func_801EB020(&self->mNumSel) != 0) {
        if (func_801EB028(&self->mNumSel) == 0) return;
        u8 idx = (u8)(self->unk38C + self->unk38E);
        CIBLTab* tabs = &self->unk3A4;
        if (func_801EC284((void*)tabs, idx) != 0) {
            self->field392 -= 10;
            if (self->field392 < 1) self->field392 = 1;
        }
        func_801EB04C(&self->mNumSel, (u8)self->field392);
        func_801EB064(&self->mNumSel, self->field392 * func_801EC260((void*)tabs, idx));
        func_80138078__FUl(1);
    } else {
        if (self->unk38C == -1) {
            func_801EDB80((void*)self);
            return;
        }
        if (self->unk3A4.count >= 7) {
            self->unk38E -= 7;
            if (self->unk38E < 0) {
                self->unk38C = self->unk38E + 6;
                self->unk38E = 0;
                if (self->unk38C < 0) self->unk38C = 0;
            }
        } else {
            self->unk38C = 0;
            self->unk38E = 0;
        }
        func_801F071C((void*)self);
        func_801F0030((void*)self);
        func_801F0488((void*)self);
        func_801F3850(&self->mScrollBar310[0], (u16)self->unk38E);
        func_80138078__FUl(1);
    }
}
#pragma pop

// ============================================================================
// func_801EE448: page-up mirror of func_801EE228. When the tab cursor is
// active, scan the page list for the next non-empty slot in the up direction
// (wrap >= 3 back to 0) and push its VEC3 to the cursor; when the num-select
// is busy, step its page position up by 10 clamped to the page's capacity;
// otherwise handle the tab page-up wrap (unk38E += 7) and refresh the line.
// ============================================================================
#pragma push
#pragma optimize_for_size on
void func_801EE448(CItemBoxLine* self) {
    if (CSysWin_getUnk34(&self->mSysWin) != 0) return;
    if (self->field3A0 != 0) {
        CIBLPageData* page = reinterpret_cast<CIBLPageData*>(&self->mInfo2D0[0xB0]);
        s8 a = (s8)(self->field3A1 + 1);
        while (a != (s8)self->field3A1) {
            if (a >= 3) a = 0;
            if (ArrayGet12(page->pageWords180, (u8)(self->field3A2 + (s8)a * 4)) != 0) {
                self->field3A1 = (u8)a;
                break;
            }
            a++;
        }
        CIBLVec3 vec;
        func_801CB9D8(&vec, page->pageWords180, (u8)(self->field3A2 + (s8)self->field3A1 * 4));
        reinterpret_cast<CIBLCur70View*>(&self->mCur88)->vf02(&vec);
        func_80138078__FUl(1);
        return;
    }
    if (func_801EB020(&self->mNumSel) != 0) {
        if (func_801EB028(&self->mNumSel) == 0) return;
        u8 idx = (u8)(self->unk38C + self->unk38E);
        CIBLTab* tabs = &self->unk3A4;
        u8 lim = func_801EC284((void*)tabs, idx);
        if (lim != 0) {
            self->field392 += 10;
            if ((u32)self->field392 > (u32)lim) self->field392 = (s16)lim;
        }
        func_801EB04C(&self->mNumSel, (u8)self->field392);
        func_801EB064(&self->mNumSel, self->field392 * func_801EC260((void*)tabs, idx));
        func_80138078__FUl(1);
        return;
    }
    if (self->unk38C == -1) {
        func_801EDA6C((void*)self);
        return;
    }
    u16 count = self->unk3A4.count;
    if (count >= 7) {
        int t = (int)count - 7;
        self->unk38E += 7;
        if (self->unk38E > t) {
            self->unk38C = self->unk38E - t;
            self->unk38E = t;
            if (self->unk38C >= 7) self->unk38C = 6;
        }
    } else {
        self->unk38C = (s16)(count - 1);
        self->unk38E = 0;
        if ((s16)(count - 1) < 0) self->unk38C = 0;
    }
    func_801F071C((void*)self);
    func_801F0030((void*)self);
    func_801F0488((void*)self);
    func_801F3850(&self->mScrollBar310[0], (u16)self->unk38E);
    func_80138078__FUl(1);
}
#pragma pop

// ============================================================================
// func_801EE684: item-box line update. When the syswin overlay is armed the
// active tab is committed; otherwise a tab/cursor hint is advanced.
// ============================================================================
void func_801EE684(CItemBoxLine* self) {
    if (CSysWin_getUnk34(&self->mSysWin)) {
        if (CSysWin_isActive(&self->mSysWin)) {
            func_8022B8E4(&self->mSysWin);
            if (self->field50 >= 9) {
                self->field50 = 0xb;
                self->field3A3 = 1;
                func_801F071C(self);
            }
        }
    } else {
        if (self->field3A0 != 0) {
            self->field3A0 = 0;
            if (self->unk38C == -1) {
                func_801D216C(&self->mCurA0, 1);
            } else {
                func_801D216C(&self->mCur70, 1);
            }
            func_801D216C(&self->mCur88, 0);
            func_80138078__FUl(6);
        } else {
            if (func_801EB020(&self->mNumSel) == 0) return;
            if (func_801EB028(&self->mNumSel) == 0) return;
            func_801EB178(&self->mNumSel);
            self->field50 = 8;
            func_80138078__FUl(6);
        }
    }
}

// ============================================================================
// func_801EE788: item-box line input/nav state machine. With the syswin
// overlay open: commit the overlay selection once state reaches 9+. Otherwise
// ignore while no tab is selected (-1) or the cursor hint is armed; when the
// num-select is confirming (field390==1 + busy + state 7) rebuild the tab list
// for the confirmed entry and settle into state 8. Otherwise resolve the
// highlighted entry: occupied slots beep, entries with page data open the
// syswin detail pane with kind-range-dependent text, armed pages either
// dispatch likewise or raise the "unavailable" flag (state 3 beep).
// ============================================================================
#pragma push
#pragma optimize_for_size on
void func_801EE788(CItemBoxLine* self) {
    char* pool = lbl_eu_805071B0;
    if (CSysWin_getUnk34(&self->mSysWin)) {
        if (CSysWin_isActive(&self->mSysWin)) {
            func_8022B8E4(&self->mSysWin);
            if (self->field50 < 9) return;
            self->field50 = 0xb;
            func_801F071C(self);
        }
        return;
    }
    if (self->unk38C == -1) return;
    if (self->field3A0 != 0) return;
    CIBLTab* tabs = &self->unk3A4;
    u32 key = (u8)(self->unk38C + self->unk38E);
    if (self->field390 == 1 && func_801EB020(&self->mNumSel)) {
        // Num-select confirm: rebuild the tab list for the confirmed entry.
        if (func_801EB028(&self->mNumSel) == 0) return;
        if (self->field50 != 7) return;
        func_801EC808(reinterpret_cast<CIBLTabFull*>(tabs), key, (u8)self->field392);
        u32 v = func_801392B4(self->field39F);
        func_801EBC00(tabs, self->tabEntries[(s8)self->field6D], self->field394, (u8)v);
        func_801EFFC4(self);
        func_801EFE6C(self);
        func_801F0030(self);
        func_801F0488(self);
        func_801EB178(&self->mNumSel);
        self->field50 = 8;
        func_80138078__FUl(0x2f);
        return;
    }
    if (func_801EC3B0(tabs, key) == 0) return;
    if (func_801EC23C(tabs, key) != 0) {
        func_80138078__FUl(5);
        return;
    }
    if (func_801EC284(reinterpret_cast<CIBLTabFull*>(tabs), key) == 0) {
        // No page data: open the syswin detail pane with kind-range text.
        char* text = NULL;
        u16 f0 = func_801EC3B0(tabs, key);
        u16 kv = (u16)func_801392E4(f0);
        func_80139358(f0);
        if ((u32)(kv - 2) <= 7) {
            text = func_80136190(pool + 0x63, pool + 0x6c, 0x11);
        } else if ((u32)(kv - 0xa) <= 3) {
            text = (func_80158068(f0) >= 1)
                       ? func_80136190(pool + 0x1db, pool + 0x6c, 3)
                       : func_80136190(pool + 0x63, pool + 0x6c, 0x11);
        }
        func_8022B90C(&self->mSysWin, 0);
        func_8022B9B4(&self->mSysWin, (u32)text, 0);
        func_8022BFC8(&self->mSysWin, 1);
        func_8022B8B8(&self->mSysWin);
        func_80138078__FUl(5);
        return;
    }
    // Armed page: name-format tabs probe availability through func_801F2880,
    // other kinds go through the character-data lookups below.
    if (self->tabEntries[(s8)self->field6D] == 0xd) {
        u16 f0d = func_801EC3B0(tabs, key);
        func_801F2880((u32)self, f0d);
    }
    int unavailable = 0;
    char* text = NULL;
    u16 f0 = func_801EC3B0(tabs, key);
    u16 kv = (u16)func_801392E4(f0);
    func_80139358(key);
    if ((u32)(kv - 2) <= 7) {
        if (func_80157CD0((u8)kv) != 0) {
            unavailable = 1;
        } else {
            text = func_80136190(pool + 0x63, pool + 0x6c, 0x11);
        }
    } else if ((u32)(kv - 0xa) <= 3) {
        u32 t = func_80158068(kv);
        if (t >= 1) {
            if (t >= 0x63) {
                text = func_80136190(pool + 0x1db, pool + 0x6c, 3);
            } else {
                unavailable = 1;
            }
        } else if (func_80157CD0((u8)kv) != 0) {
            unavailable = 1;
        } else {
            text = func_80136190(pool + 0x63, pool + 0x6c, 0x11);
        }
    }
    if (unavailable) {
        self->unk39E = 1;
        func_80138078__FUl(3);
    } else {
        func_8022B90C(&self->mSysWin, 0);
        func_8022B9B4(&self->mSysWin, (u32)text, 0);
        func_8022BFC8(&self->mSysWin, 1);
        func_8022B8B8(&self->mSysWin);
        func_80138078__FUl(5);
    }
}
#pragma pop

// ============================================================================
// func_801F0030: refresh the seven tab-slot panes. Build the twelve RGBA
// palette entries, then for each slot: pick the lit/unlit color pair from the
// palette by slot occupancy, refresh the pane texture/name/visibility, and for
// named tabs bind the tab name + per-pane animation colors (alpha from the
// func_80139658 animation vector) to the layout panes.
// ============================================================================
void func_801F0030(CItemBoxLine* self) {
    CIBLTab* tabs = &self->unk3A4;
    GXColorS10 colors[12];
    func_801C4B60(&colors[0], 0xff, 0xff, 0xfa, 0);
    func_801C4B60(&colors[1], 0xff, 0xff, 0xfa, 0);
    func_801C4B60(&colors[2], 0x48, 0x3a, 0x21, 0);
    func_801C4B60(&colors[3], 0xff, 0xff, 0xfa, 0);
    func_801C4B60(&colors[4], 0xb7, 0xe, 0xe, 0);
    func_801C4B60(&colors[5], 0xb7, 0xe, 0xe, 0);
    func_801C4B60(&colors[6], 0x28, 0x5, 0x5, 0);
    func_801C4B60(&colors[7], 0xf7, 0x8e, 0x8e, 0);
    func_801C4B60(&colors[8], 0, 0, 0, 0);
    func_801C4B60(&colors[9], 0, 0, 0, 0);
    func_801C4B60(&colors[10], 0, 0, 0, 0);
    func_801C4B60(&colors[11], 0, 0, 0, 0);
    char* pool = lbl_eu_805071B0;
    for (u8 i = 0; i < 7; i++) {
        u16 idx = (u16)(self->unk38E + i);
        if (func_801EC23C((void*)tabs, idx) != 0) {
            __as__11_GXColorS10FRC11_GXColorS10(&colors[8], &colors[4]);
            __as__11_GXColorS10FRC11_GXColorS10(&colors[9], &colors[5]);
            __as__11_GXColorS10FRC11_GXColorS10(&colors[10], &colors[6]);
            __as__11_GXColorS10FRC11_GXColorS10(&colors[11], &colors[7]);
        } else {
            __as__11_GXColorS10FRC11_GXColorS10(&colors[8], &colors[0]);
            __as__11_GXColorS10FRC11_GXColorS10(&colors[9], &colors[1]);
            __as__11_GXColorS10FRC11_GXColorS10(&colors[10], &colors[2]);
            __as__11_GXColorS10FRC11_GXColorS10(&colors[11], &colors[3]);
        }
        u16 key = func_801EC3B0(tabs, idx);
        func_801EF734((void*)self, key, i);
        func_801EF844((void*)self, key, i);
        u8 vis = func_801EC8B4((void*)tabs, idx);
        func_801EFDF4((void*)self, i, vis);
        func_801EF954((void*)self, key, (s8)func_801EC8D8((void*)tabs, idx), i);
        if (key == 0) {
            char nameBuf2[0x20];
            sprintf(nameBuf2, pool + 0x39d, i + 1);
            func_80136B4C(self->field40, nameBuf2, pool + 0x254, 0);
            sprintf(nameBuf2, pool + 0x3aa, i + 1);
            func_80136B4C(self->field40, nameBuf2, pool + 0x254, 0);
        } else {
            char nameBuf1[0x20];
            sprintf(nameBuf1, pool + 0x39d, i + 1);
            func_80136B4C(self->field40, nameBuf1,
                          (char*)func_801EC3D0((void*)tabs, idx), 0);
            FourShorts fs0 = func_80139658(self->field40, nameBuf1, 0);
            FourShorts fs1 = fs0;
            FourShorts fs2 = func_80139658(self->field40, nameBuf1, 1);
            FourShorts fs3 = fs2;
            colors[8].a = fs1.d;
            colors[9].a = fs2.d;
            func_80139A18(self->field40, nameBuf1, &colors[8], &colors[9]);
            char* name = func_80136190(pool + 0x248, pool + 0x6c, 3);
            u32 count = func_801EC260((void*)tabs, idx);
            char buf3[0x20];
            sprintf(buf3, pool + 0x3bb, count, name);
            sprintf(nameBuf1, pool + 0x3aa, i + 1);
            func_80136A1C(self->field40, nameBuf1, buf3, 0);
            FourShorts fs4 = func_80139658(self->field40, nameBuf1, 0);
            FourShorts fs5 = fs4;
            FourShorts fs6 = func_80139658(self->field40, nameBuf1, 1);
            FourShorts fs7 = fs6;
            colors[10].a = fs5.d;
            colors[11].a = fs6.d;
            func_80139A18(self->field40, nameBuf1, &colors[10], &colors[11]);
        }
    }
}

u8 func_801EECC0(void* self) { return static_cast<CItemBoxLine*>(self)->unk39E; }

void CItemBoxLine::func_801EECC8() {
    func_801EC3B0(&unk3A4, (unsigned char)(unk38C + unk38E));
}

void func_801EECE0(void* self) { ((void(*)(void*))func_801D2E4C)((char*)self + 0xb8); }

// ============================================================================
// func_801EECE8: step the tab cursor down one slot (wrap to count-1 when it
// underflows), re-format the tab name overlay, then refresh the whole line.
// ============================================================================
void func_801EECE8(CItemBoxLine* self) {
    u8 b = code80135FDC_getByte_64077();
    u8 n = self->field39F - 1;
    self->field39F = n;
    if ((s8)n < 0) {
        self->field39F = b - 1;
    }
    u8 tmp[16];
    func_801E174C(tmp, self->mInfo2D0, self->field39F);
    reinterpret_cast<CIBLCur70View*>(&self->mCurB8)->vf02(tmp);
    func_801EFFC4(static_cast<void*>(self));
    func_80138078__FUl(0xa);
}

// ============================================================================
// func_801EED6C: bump a cursor-position counter, format the tab name into a
// temp buffer, push it to the item-box-info2 overlay (via its vtable[4]), then
// refresh the whole line and beep.
// ============================================================================
void func_801EED6C(void* self) {
    unsigned char* p = (unsigned char*)self;
    unsigned char maxPos = code80135FDC_getByte_64077();
    unsigned char n = (unsigned char)(p[0x39f] + 1);
    p[0x39f] = n;
    if ((signed char)n >= (int)maxPos) {
        p[0x39f] = 0;
    }
    unsigned char tmp[16];
    func_801E174C(tmp, p + 0xd0, p[0x39f]);
    reinterpret_cast<CIBLCur70View*>(p + 0xb8)->vf02(tmp);
    func_801EFFC4(self);
    func_80138078__FUl(0xa);
}

// ============================================================================
// func_801EEDF8: item-box line syswin/tab interaction. When the tab cursor is
// active and the syswin overlay is open, advance the syswin; otherwise open
// the detail window for the current page slot (name via the slot flag: kind-3
// slots use the item-name provider, others the shared table). When the tab
// cursor is idle, scan the page list for the first occupied slot (preferring
// slot 8), decompose it into page/column (field3A1 = slot/4, field3A2 =
// slot%4 via a float multiply), prime the cursors and push the slot VEC3.
// ============================================================================
#pragma push
#pragma optimize_for_size on
void func_801EEDF8(CItemBoxLine* self) {
    if (func_801EB020(&self->mNumSel) != 0) return;
    if (self->field3A0 != 0) {
        if (CSysWin_getUnk34(&self->mSysWin) != 0) {
            if (CSysWin_isActive(&self->mSysWin) != 0) {
                func_8022B8E4(&self->mSysWin);
            }
            return;
        }
        CIBLPageData* page = reinterpret_cast<CIBLPageData*>(&self->mInfo2D0[0xB0]);
        u8 idx = (u8)(self->field3A2 + (s8)self->field3A1 * 4);
        u16 v = ArrayGet12(page->pageWords180, idx);
        if (v != 0) {
            u8 bt = func_801EF034((const u8*)page, idx);
            u32 obj;
            if (bt == 3) {
                obj = func_801D3C74(page, idx);
            } else {
                obj = (u32)func_80136190(&lbl_eu_805071B0[0x1f0], &lbl_eu_805071B0[0x1fa], (u16)v);
            }
            func_8022B90C(reinterpret_cast<CSysWin*>(&self->mSysWin), 0);
            func_8022B9B4(&self->mSysWin, (u32)obj, 0);
            func_8022BFC8(reinterpret_cast<CSysWin*>(&self->mSysWin), 1);
            func_8022B8B8(&self->mSysWin);
        }
        return;
    }
    if (CSysWin_getUnk34(&self->mSysWin) != 0) return;
    CIBLPageData* page2 = reinterpret_cast<CIBLPageData*>(&self->mInfo2D0[0xB0]);
    u8 found = 0;
    for (u8 i = 0; i < 12; i++) {
        if (page2->pageWords180[i] != 0) {
            page2->pageSlot258 = i;
            if (page2->pageWords180[8] != 0) page2->pageSlot258 = 8;
            found = 1;
            break;
        }
    }
    if (found != 0) {
        u8 sel = page2->pageSlot258;
        self->field3A0 = 1;
        self->field3A1 = (u8)((float)sel * lbl_eu_80668124);
        self->field3A2 = (u8)(sel - (s8)self->field3A1 * 4);
        func_801D216C(&self->mCurA0, 0);
        func_801D216C(&self->mCur70, 0);
        func_801D216C(&self->mCur88, 1);
        CIBLVec3 vec;
        func_801CB9D8(&vec, page2->pageWords180, sel);
        reinterpret_cast<CIBLCur70View*>(&self->mCur88)->vf02(&vec);
        func_80138078__FUl(2);
    } else {
        func_80138078__FUl(5);
    }
}
#pragma pop

// ============================================================================
// func_801EF0EC: item-box focus resolver. Returns a cursor/focus id based on
// the current navigation state and tab position.
// ============================================================================
u8 func_801EF0EC(CItemBoxLine* self) {
    if (self->field50 >= 6) return 0;
    if (func_801EB020((char*)self + 0x2dc)) return 0;
    if (CSysWin_getUnk34((char*)self + 0x350)) return 0;
    if (func_801D2ED8((CBaseCur*)((char*)self + 0xb8))) return 0xa;
    bool flag = code80135FDC_getByte_64077() > 1;
    if (self->field3A0 != 0) return 0xb;
    if (self->unk38C == -1) {
        if (self->tabCount > 1) {
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
// func_801EF1E4: when the animation transform at +0x44 has finished, disable
// animation on it and enable it on +0x48 (intro -> loop), entering state 2.
// ============================================================================
void func_801EF1E4(CItemBoxLine* self) {
    if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(self->field44, lbl_eu_80668114) != 0) {
        self->field40->SetAnimationEnable(self->field44, false);
        self->field40->SetAnimationEnable(self->field48, true);
        self->field50 = 2;
    }
}

// ============================================================================
// func_801EF260: animate the item-box intro (0x48), store state 3, prime cursors
// and re-format/push the tab name overlay.
// ============================================================================
void func_801EF260(CItemBoxLine* self) {
    if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(self->field48, lbl_eu_80668114) == 0) return;
    self->field50 = 3;
    self->unk59 = 1;
    func_801F0488(self);
    func_801D216C(&self->mCur70, 1);
    func_801D216C(&self->mCurB8, 1);
    u8 tmp[16];
    func_801E174C(tmp, &self->mInfo2D0[0], self->field39F);
    reinterpret_cast<CIBLCur70View*>(&self->mCurB8)->vf02(tmp);
    func_801F071C(self);
}

// ============================================================================
// func_801EF2FC: when the animation transform at +0x48 has finished, disable
// animation on it and re-enable it on +0x44 (outro -> intro), entering state 5.
// ============================================================================
void func_801EF2FC(CItemBoxLine* self) {
    if (func_80137510(self->field48, lbl_eu_80668114) != 0) {
        self->field40->SetAnimationEnable(self->field48, false);
        self->field40->SetAnimationEnable(self->field44, true);
        self->field50 = 5;
    }
}

// ============================================================================
// func_801EF378: on animation (+0x44) completion, set the busy byte, clear the
// navigation state, silence both cursors (+0x70/+0xA0) and clear the visible
// flag so the line stops rendering.
// ============================================================================
void func_801EF378(CItemBoxLine* self) {
    if (func_80137510(self->field44, lbl_eu_80668114) != 0) {
        self->unk59 = 1;
        self->field50 = 0;
        func_801D216C((u8*)self + 0x70, 0);
        func_801D216C((u8*)self + 0xa0, 0);
        self->field4C = 0;
    }
}

// ============================================================================
// func_801EF3E8: when the syswin overlay is active, enter state 10, prime the
// cursor at +0x70, build the syswin selection buffer and push it through the
// cursor's vtable[2] virtual.
// ============================================================================
void func_801EF3E8(CItemBoxLine* self) {
    if (CSysWin_isActive(&self->mSysWin)) {
        self->field50 = 0xa;
        func_801D216C(&self->mCur70, 1);
        u8 tmp[0x10];
        func_8022C1B4(tmp, &self->mSysWin, self->field3A3);
        reinterpret_cast<CIBLCur70View*>(&self->mCur70)->vf02(tmp);
    }
}

// ============================================================================
// func_801EF45C: refresh the active tab - select from num-select, set the name,
// and advance to state 6. csvtab index = (0x38c + 0x38e) & 0xFF.
// ============================================================================
#pragma push
#pragma optimize_for_size on
void func_801EF45C(CItemBoxLine* self) {
    if (!CSysWin_isActive(&self->mSysWin)) return;
    self->field50 = 3;
    if ((s8)self->field3A3 != 0) return;
    u8 tabidx = (u8)(self->unk38C + self->unk38E);
    self->field392 = 1;
    // tabs pointer is CSE'd into one callee-saved register (retail r30). The
    // (u8*) arg type routes both calls through the extern "C" (void*,
    // unsigned) overloads so MWCC emits external relocs to func_801EC3D0 /
    // func_801EC260 (retail object boundary) instead of inlining the same-TU
    // definitions.
    u8* tabs = (u8*)&self->unk3A4;
    func_801EB030(&self->mNumSel, func_801EC3D0(tabs, tabidx));
    func_801EB04C(&self->mNumSel, (u8)self->field392);
    func_801EB064(&self->mNumSel, self->field392 * func_801EC260(tabs, tabidx));
    func_801EB0D4(&self->mNumSel);
    self->field50 = 6;
}
#pragma pop

// ============================================================================
// func_801EF518: refresh the tab list - re-register each tab entry with the
// layout, compact empty slots (bubble-shift), recount, re-register, then
// refresh the line and its slot panes.
// ============================================================================
#pragma push
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_801EF518(CItemBoxLine* self) {
    if (self->field40 == 0) return;
    if (self->field3C == 0) return;
    if (lbl_eu_80664648 == 0) return;
    if (lbl_eu_8066464C == 0) return;
    if (lbl_eu_80664650 == 0) return;
    func_80139198(0);
    if (self->field390 == 2) self->unk3A4.field93 = 1;
    u8 i = 0;
    while (i < self->tabCount) {
        func_801EBC00(&self->unk3A4, self->tabEntries[i], self->field394, 0);
        self->unk64[i] = (self->unk3A4.count != 0);
        i++;
    }
    for (u8 j = 0; j < self->tabCount; j++) {
        if (self->unk64[j] == 0) self->tabEntries[j] = 0;
    }
    for (u8 k = 0; k < self->tabCount - 1; k++) {
        u8 did = 0;
        for (u8 m = 0; m < (self->tabCount - 1) - k; m++) {
            if (self->tabEntries[m] == 0) {
                self->tabEntries[m] = self->tabEntries[m + 1];
                self->tabEntries[m + 1] = 0;
                did = 1;
            }
        }
        if (!did) break;
    }
    self->tabCount = 0;
    for (u8 n = 0; n < 9; n++) {
        if (self->tabEntries[n] == 0) break;
        self->tabCount++;
    }
    i = 0;
    while (i < self->tabCount) {
        func_801EBC00(&self->unk3A4, self->tabEntries[i], self->field394, 0);
        self->unk64[i] = (self->unk3A4.count != 0);
        i++;
    }
    func_801EFFC4((void*)self);
    func_801EFE6C((void*)self);
    self->field58 = 1;
}
#pragma pop

// ============================================================================
// func_801EF734: refresh a tab pane's texture. When a key id is given, resolve
// its texture name via the shared table (lbl_eu_806640EC), try object A
// (+0x3C) first and fall back to object B (+0x38) with the default name; with
// no key id, B is used directly. If a resource was found, format the pane
// name from arg3 and bind the resource to the active layout.
// ============================================================================
void func_801EF734(CItemBoxLine* self, u32 arg2, u32 arg3) {
    u32 result;
    if (arg2 != 0) {
        u32 v = func_80136254((u32)lbl_eu_806640EC, &lbl_eu_805071B0[0x1ff], arg2);
        result = self->field3C->vf01(0x74696d67, func_80138F78((u16)v), 0);
        if (result == 0) {
            result = self->field38->vf01(0x74696d67, &lbl_eu_805071B0[0x209], 0);
        }
    } else {
        result = self->field38->vf01(0x74696d67, &lbl_eu_805071B0[0x209], 0);
    }
    if (result != 0) {
        char buf[0x20];
        sprintf(buf, &lbl_eu_805071B0[0x21c], arg3 + 1);
        func_80137E7C(self->field40, buf, result);
    }
}

void func_801EF844(CItemBoxLine* self, u32 arg2, u32 arg3) {
    u32 result;
    if (arg2 != 0) {
        u32 v = func_80136254((u32)lbl_eu_806640EC, &lbl_eu_805071B0[0x22a], arg2);
        result = self->field3C->vf01(0x74696d67, func_80138F78((u16)v), 0);
        if (result == 0) {
            result = self->field38->vf01(0x74696d67, &lbl_eu_805071B0[0x209], 0);
        }
    } else {
        result = self->field38->vf01(0x74696d67, &lbl_eu_805071B0[0x209], 0);
    }
    if (result != 0) {
        char buf[0x20];
        sprintf(buf, &lbl_eu_805071B0[0x22f], arg3 + 1);
        func_80137E7C(self->field40, buf, result);
    }
}

// ============================================================================
// func_801EF954: format the item-slot name into the line. The kind selector
// (func_801392E4) picks the item-name path (kinds 3/9) which resolves the
// name via the owner table; otherwise a signed hint byte picks one of the
// fixed strings / name providers.
// ============================================================================
#pragma push
#pragma optimize_for_size on
void func_801EF954(CItemBoxLine* self, u32 arg4, s8 arg5, u32 arg6) {
    char buf[0x20];
    sprintf(buf, &lbl_eu_805071B0[0x23c], arg6 + 1);
    ml::FixStr<32> str;
    str.clear();
    u8 sel = (u8)func_801392E4(arg4);
    if (sel == 3 || sel == 9) {
        u8 v = func_801361E8((u32)lbl_eu_806640EC, &lbl_eu_805071B0[0x5a], arg4);
        char* name = func_80136190(&lbl_eu_805071B0[0x248], &lbl_eu_805071B0[0x6c], 0x1e - (v - 1));
        str.format(&lbl_eu_805071B0[0x57], name);
        func_80139A18(self->field40, buf, lbl_eu_80664618, lbl_eu_80664620);
    } else if (arg5 > 0) {
        str.format(&lbl_eu_805071B0[0x251]);
        func_80139A18(self->field40, buf, lbl_eu_80664618, lbl_eu_80664620);
    } else if (arg5 == 0) {
        const char* s = &lbl_eu_805071B0[0x254];
        str = s;
    } else if (arg5 == -2) {
        str = func_eu_802B148C();
        func_80139A18(self->field40, buf, lbl_eu_80664628, lbl_eu_80664630);
    } else if (arg5 == -3) {
        str = func_eu_802B1474();
        func_80139A18(self->field40, buf, lbl_eu_80664638, lbl_eu_80664640);
    } else {
        const char* s = &lbl_eu_805071B0[0x254];
        str = s;
    }
    func_80136B4C(self->field40, buf, str.mString, 0);
}
#pragma pop

// ============================================================================
// func_801EFDF4: format the pane name (skipping the leading marker char of the
// caller's string) into a temp buffer, look the pane up on the layout root,
// and toggle its visibility from the caller's flag.
// ============================================================================
void func_801EFDF4(CItemBoxLine* self, const char* str, bool visible) {
    char buf[0x20];
    sprintf(buf, &lbl_eu_805071B0[0x35d], str + 1);
    nw4r::lyt::Pane* root = *(nw4r::lyt::Pane**)((u8*)self->field40 + 0x10);
    nw4r::lyt::Pane* pane = root->FindPaneByName(buf, true);
    func_80124270(pane, !visible);
}

// ============================================================================
// func_801EFE6C: refresh the seven tab-slot panes (same shape as
// func_801F061C), then, when the slot is unoccupied, bind a texture resource
// (object A fallback path) onto the slot's pane name; finally dispatch the
// slot entry through func_801EFB24. r25 keeps tabEntries[i] across the calls.
// ============================================================================
void func_801EFE6C(CItemBoxLine* self) {
    for (u8 i = 0; i < 7; i++) {
        char nameA[0x20];
        char nameB[0x20];
        sprintf(nameA, &lbl_eu_805071B0[0x369], (u8)i + 1);
        sprintf(nameB, &lbl_eu_805071B0[0x378], (u8)i + 1);
        u32 visA = 0;
        u32 visB = 0;
        u8 entry = self->tabEntries[i];
        if (entry != 0) {
            if ((u8)i == (s8)self->field6D) {
                visA = 1;
                visB = 0;
            } else {
                visA = 0;
                visB = 1;
            }
        }
        func_80124270(self->field40->GetRootPane()->FindPaneByName(nameA, true), visA);
        func_80124270(self->field40->GetRootPane()->FindPaneByName(nameB, true), visB);
        if (self->unk64[i] == 0) {
            u32 tex = self->field38->vf01(0x74696d67, &lbl_eu_805071B0[0x387], 0);
            if (tex != 0) {
                func_80137E7C(self->field40, nameB, tex);
            }
        }
        func_801EFB24(self, entry, i);
    }
}

// ============================================================================
// func_801EFB24: bind the per-slot tab texture and nameplate colour. Resolve
// the texture name by slot kind (kinds 1..12 format a fixed pool string,
// kind 0 uses the shared "timeg" fallback name directly), fetch the texture
// through resource object A (falling back to B), bind it to the pane named
// "%d"-style by index+1, and - unless the slot's occupancy byte is set -
// push the default nameplate colour through the pane's colour-source virtual.
// noinline: retail func_801EFE6C calls func_801EFB24 as an EXTERNAL reloc; the
// body would otherwise be inlined away, dropping the call from the .o.
// ============================================================================
extern "C" __declspec(noinline) void func_801EFB24(CItemBoxLine* self, u8 kind, u32 index) {
    const u32 tag = 0x74696d67;
    char buf[0x20];
    u32 tex;
    if (kind == 0) {
        tex = self->field38->vf01(tag, &lbl_eu_805071B0[0x209], 0);
    } else {
        switch (kind) {
        case 1:  sprintf(buf, &lbl_eu_805071B0[0x255]); break;
        case 2:  sprintf(buf, &lbl_eu_805071B0[0x26a]); break;
        case 3:  sprintf(buf, &lbl_eu_805071B0[0x27f]); break;
        case 4:  sprintf(buf, &lbl_eu_805071B0[0x294]); break;
        case 5:  sprintf(buf, &lbl_eu_805071B0[0x2a9]); break;
        case 6:  sprintf(buf, &lbl_eu_805071B0[0x2be]); break;
        case 7:  sprintf(buf, &lbl_eu_805071B0[0x2d3]); break;
        case 8:  sprintf(buf, &lbl_eu_805071B0[0x2e8]); break;
        case 9:  sprintf(buf, &lbl_eu_805071B0[0x2fd]); break;
        case 10: sprintf(buf, &lbl_eu_805071B0[0x312]); break;
        case 11: sprintf(buf, &lbl_eu_805071B0[0x327]); break;
        case 12: sprintf(buf, &lbl_eu_805071B0[0x33c]); break;
        }
        tex = self->field3C->vf01(tag, buf, 0);
            if (tex == 0) {
                tex = self->field38->vf01(tag, &lbl_eu_805071B0[0x209], 0);
        }
    }
    if (tex != 0) {
        char nameB[0x20];
        sprintf(nameB, &lbl_eu_805071B0[0x351], index + 1);
        func_80137E7C(self->field40, nameB, tex);
        if (self->unk64[index] == 0) {
            GXColorS10 col;
            col.b = 0x14;
            col.g = 0x14;
            col.r = 0x14;
            col.a = self->field396.a;
            nw4r::lyt::Pane* pane = self->field40->GetRootPane()->FindPaneByName(nameB, true);
            if (pane != NULL) {
                CIBLColorSrc* src = reinterpret_cast<CIBLNameplateView*>(pane)->vf24();
                if (src != NULL) {
                    __as__11_GXColorS10FRC11_GXColorS10(&src->color, &col);
                }
            }
        }
    }
}

// ============================================================================
// func_801EFFC4: refresh the item-box line - push the current tab entry (tab
// type byte, page id, selection cursor) into the tab layout, re-run the
// focus/page refresh, then poll the info2 overlay state. The tab-buffer,
// focus and page helpers are external retail symbols (same merged unit,
// different retail objects), so they are called through their C-linkage
// declarations to keep the relocs external.
// ============================================================================
u32 func_801EFFC4(CItemBoxLine* self) {
    u32 sel = func_801392B4(self->field39F);
    func_801EBC00(&self->unk3A4, self->tabEntries[(s8)self->field6D],
                  self->field394, (u8)sel);
    func_801F0030((void*)self);
    func_801F0488((void*)self);
    return getItemBox2State__FP13CItemBoxInfo2((unsigned char*)self + 0xd0);
}

// ============================================================================
// func_801F0488: refresh the tab-line page list. Set the tab name overlay,
// then (when the info2 overlay is open) push the active tab entry into the
// info2 state and rebuild the 12 page slots: copy the per-page word, flag and
// position data from the page tables into the +0x180 page region.
// ============================================================================
#pragma push
#pragma optimize_for_size on
void func_801F0488(CItemBoxLine* self) {
    CIBLTab* tabs = &self->unk3A4;
    u8 idx = (u8)(self->unk38C + self->unk38E);
    char* name = func_801EC438(reinterpret_cast<CIBLTabFull*>(tabs), idx);
    func_80136B4C(self->field40, &lbl_eu_805071B0[0x3c0], name,
                  reinterpret_cast<u32>(self->field54));
    if (getItemBox2State__FP13CItemBoxInfo2(&self->mInfo2D0[0]) != 0) {
        u8 f9 = func_801EC8B4(reinterpret_cast<u8*>(tabs), idx);
        u16 f0 = func_801EC3B0(tabs, idx);
        func_801E14DC(reinterpret_cast<u8*>(&self->mInfo2D0[0]), f0, 0,
                      self->field39F, f9);
        char* tabName = reinterpret_cast<char*>(
            func_801EC3D0(reinterpret_cast<u8*>(tabs), idx));
        func_801E16F0(reinterpret_cast<u8*>(&self->mInfo2D0[0]), 0, tabName);
        func_801F08B4(reinterpret_cast<u8*>(self),
                      func_801EC3B0(tabs, idx));
        CIBLPageData* page = reinterpret_cast<CIBLPageData*>(&self->mInfo2D0[0xB0]);
        u16 i = 0;
        do {
            u16 v = ArrayGet12(page->pageWords4E0, (u8)i);
            if (v != 0) {
                page->pageWords180[(u8)i] = v;
                page->pageFlags228[(u8)i] = func_801EF034(
                    reinterpret_cast<const u8*>(page->pageWords4E0), (u8)i);
                u8 b594 = 0;
                if ((u8)i < 12) {
                    b594 = page->pageBytes594[(u8)i];
                }
                page->pageFlags234[(u8)i] = b594;
                s16 s5A0 = 0;
                if ((u8)i < 12) {
                    s5A0 = page->pageShorts5A0[(u8)i];
                }
                page->pageShorts2C0[(u8)i] = s5A0;
                CIBLVec3 vec;
                func_801CB9D8(&vec, page->pageWords4E0, (u8)i);
                copyVEC3(&page->pageVec198[(u8)i], &vec);
            }
            i++;
        } while (i < 12);
    }
}
#pragma pop
// ============================================================================
// func_801F061C: refresh the seven tab-slot panes - format each slot's two
// pane names, then show/hide them: the slot matching the current tab position
// is lit when its entry is active, otherwise the paired pane is lit instead.
// ============================================================================
void func_801F061C(CItemBoxLine* self) {
    for (u8 i = 0; i < 7; i++) {
        char nameA[0x20];
        char nameB[0x20];
        sprintf(nameA, &lbl_eu_805071B0[0x369], (u8)i + 1);
        sprintf(nameB, &lbl_eu_805071B0[0x378], (u8)i + 1);
        u32 visA = 0;
        u32 visB = 0;
        if (self->tabEntries[i] != 0) {
            if (i == (s8)self->field6D) {
                visA = 1;
                visB = 0;
            } else {
                visA = 0;
                visB = 1;
            }
        }
        func_80124270(self->field40->GetRootPane()->FindPaneByName(nameA, true), visA);
        func_80124270(self->field40->GetRootPane()->FindPaneByName(nameB, true), visB);
    }
}

// ============================================================================
// func_801F071C: move the active cursor onto its pane. When no tab entry is
// selected (unk38C == -1) the tab cursor (mCurA0) tracks the pane named from
// the current tab position and the line cursor (mCur70) is hidden; otherwise
// the line cursor tracks the slot pane and the tab cursor is hidden. The
// cursor Move virtual receives the pane's accumulated translate with x scaled
// by the fixed reference pane's +0x44 scale.
// ============================================================================
extern "C" void func_801F071C(void* selfPtr) {
    CItemBoxLine* self = (CItemBoxLine*)selfPtr;
    s16 sel = self->unk38C;
    if (sel == -1) {
        char name[0x20];
        sprintf(name, &lbl_eu_805071B0[0x351], (s8)self->field6D + 1);
        nw4r::lyt::Pane* pane =
            self->field40->GetRootPane()->FindPaneByName(name, true);
        nw4r::math::VEC3 pos;
        func_801375A0(&pos, pane);
        void* refPane =
            self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x3cc], true);
        pos.x *= *(f32*)((u8*)refPane + 68);
        reinterpret_cast<CIBLCur70View*>(&self->mCurA0)->vf02(&pos);
        func_801D216C(&self->mCurA0, 1);
        func_801D216C(&self->mCur70, 0);
    } else {
        char name[0x20];
        sprintf(name, &lbl_eu_805071B0[0x3db], sel + 1);
        nw4r::lyt::Pane* pane =
            self->field40->GetRootPane()->FindPaneByName(name, true);
        nw4r::math::VEC3 pos;
        func_801375A0(&pos, pane);
        void* refPane =
            self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x3cc], true);
        pos.x *= *(f32*)((u8*)refPane + 68);
        reinterpret_cast<CIBLCur70View*>(&self->mCur70)->vf02(&pos);
        func_801D216C(&self->mCur70, 1);
        func_801D216C(&self->mCurA0, 0);
    }
}

// ============================================================================
// func_801F0A58: reset the page word/vector tables and hide every line pane.
// ============================================================================
#pragma push
#pragma optimize_for_size on
void func_801F0A58(CItemBoxLine* self, u32 itemData) {
    for (u8 i = 0; i < 12; i++) {
        self->pageWords4E0[i] = 0;
        CIBLVec3 vec;
        copyVEC3(&self->pageVecs4F8[i],
                 reinterpret_cast<const CIBLVec3*>(code80135FDC_setVec3(
                     (float*)&vec, lbl_eu_806680F8, lbl_eu_806680F8, lbl_eu_806680F8)));
    }
    // Hide every line pane (pane names from the shared pool; all hidden).
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x413], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x41f], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x42b], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x437], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x3f2], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x3fd], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x408], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x443], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x44e], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x459], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x464], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x46f], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x47a], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x485], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x490], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x49b], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x4a6], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x4b2], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x4be], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x4ca], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x4d6], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x4e2], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x4ee], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x4fb], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x508], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x515], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x51f], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x529], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x533], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x53e], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x549], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x555], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x561], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x56c], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x577], true), 0);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x583], true), 0);
}
#pragma pop

// ============================================================================
// func_801F107C: show the item-slot panes, bind per-kind slot data (counts,
// names, per-slot bytes) into the layout, accumulate a ratio from the slot-
// entry lookup loop and refresh the pane colors.
// ============================================================================
void func_801F107C(CItemBoxLine* self, u32 itemData) {
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x413], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x443], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x44e], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x459], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x464], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x46f], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x47a], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x485], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x490], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x49b], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x4a6], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x4b2], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x4be], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x4ca], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x4d6], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x4ee], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x4fb], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x508], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x515], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x51f], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x529], true), 1);

    // Per-kind slot lookups from the kind-owner table (lbl_eu_806640F4).
    u32 table = lbl_eu_806640F4;
    func_801392E4(itemData);
    u16 kind = func_80139358(itemData);
    u16 a = func_80136254(table, &lbl_eu_805071B0[0x58f], kind);
    u16 b = func_80136254(table, &lbl_eu_805071B0[0x597], kind);
    u8 c = func_801361E8(table, &lbl_eu_805071B0[0x59e], kind);
    u8 d = func_801361E8(table, &lbl_eu_805071B0[0x5a6], kind);
    u8 e = func_801361E8(table, &lbl_eu_805071B0[0x5ae], kind);
    u8 f = func_801361E8(table, &lbl_eu_805071B0[0x5b6], kind);
    float total = (float)f / lbl_eu_80668128;
    u8 g = func_801361E8(table, &lbl_eu_805071B0[0x5bc], kind);
    int v23;
    int v22;
    if (func_801361E8(table, &lbl_eu_805071B0[0x5c5], kind) & 4) {
        // retail reuses func_8009EC9C's r3 as the arg to func_800A082C
        u16 c1 = func_800A082C(func_8009EC9C(1));
        v23 = (int)(lbl_eu_8066812C * (float)(a * c1));
        u16 c2 = func_800A082C(func_8009EC9C(1));
        v22 = (int)(lbl_eu_80668130 * (float)(b * c2));
        if ((u16)v23 >= 0x3e7) v23 = 0x3e7;
        if ((u16)v22 >= 0x3e7) v22 = 0x3e7;
    }

    // Slot-entry ratio loop: probe slot names "%d" until an entry exists,
    // adding the per-slot bonus; then refresh the item counts.
    char buf[0x20];
    u8 i = 1;
    do {
        sprintf(buf, &lbl_eu_805071B0[0x16], i);
        if (func_801361E8(table, buf, kind) != 0) {
            total += func_8013B380(i);
            break;
        }
        i++;
    } while (i <= 10);
    func_80139C98((u16)v23, (u16)v22, 0, total);

    // Format the slot-count text and bind it plus the per-slot bytes.
    ml::FixStr<32> str;
    char* nameB = func_80136190(&lbl_eu_805071B0[0x248], &lbl_eu_805071B0[0x6c], 0xb);
    str.format(&lbl_eu_805071B0[0x5ca], (u16)v23, nameB, (u16)v22);
    func_80136B4C(self->field40, &lbl_eu_805071B0[0x4a6], str.mString, 0);
    func_80136910__FPQ34nw4r3lyt6LayoutPcUc(self->field40, &lbl_eu_805071B0[0x4ca], c);
    func_80136910__FPQ34nw4r3lyt6LayoutPcUc(self->field40, &lbl_eu_805071B0[0x4d6], d);
    char* n7f = func_80136190(&lbl_eu_805071B0[0x248], &lbl_eu_805071B0[0x6c], 0x7f);
    char* n80 = func_80136190(&lbl_eu_805071B0[0x248], &lbl_eu_805071B0[0x6c], 0x80);
    if (e != 0) {
        str.format(&lbl_eu_805071B0[0x5d1], n7f, n80);
    } else {
        str.format(&lbl_eu_805071B0[0x3bb], e, n80);
    }
    func_80136B4C(self->field40, &lbl_eu_805071B0[0x4b2], str.mString, 0);
    char* m7f = func_80136190(&lbl_eu_805071B0[0x248], &lbl_eu_805071B0[0x6c], 0x7f);
    char* m80 = func_80136190(&lbl_eu_805071B0[0x248], &lbl_eu_805071B0[0x6c], 0x80);
    if (g != 0) {
        str.format(&lbl_eu_805071B0[0x5d1], m7f, m80);
    } else {
        str.format(&lbl_eu_805071B0[0x3bb], g, m80);
    }
    func_80136B4C(self->field40, &lbl_eu_805071B0[0x4be], str.mString, 0);
    func_80136B4C(self->field40, &lbl_eu_805071B0[0x4e2], &lbl_eu_805071B0[0x254], 0);
    func_80136B4C(self->field40, &lbl_eu_805071B0[0x490], &lbl_eu_805071B0[0x254], 0);
    func_80136B4C(self->field40, &lbl_eu_805071B0[0x49b], &lbl_eu_805071B0[0x254], 0);
    func_80139A18(self->field40, &lbl_eu_805071B0[0x443], lbl_eu_806645D8, lbl_eu_806645E0);
    func_80139A18(self->field40, &lbl_eu_805071B0[0x464], lbl_eu_806645D8, lbl_eu_806645E0);
    func_80139A18(self->field40, &lbl_eu_805071B0[0x46f], lbl_eu_806645D8, lbl_eu_806645E0);
    func_80139A18(self->field40, &lbl_eu_805071B0[0x44e], lbl_eu_806645D8, lbl_eu_806645E0);
    func_80139A18(self->field40, &lbl_eu_805071B0[0x459], lbl_eu_806645D8, lbl_eu_806645E0);
    func_80139A18(self->field40, &lbl_eu_805071B0[0x47a], lbl_eu_806645E8, lbl_eu_806645F0);
    func_80139A18(self->field40, &lbl_eu_805071B0[0x485], lbl_eu_806645E8, lbl_eu_806645F0);
    func_801F2434((void*)self, itemData);
}


// ============================================================================
// func_801F183C: show the item-slot panes, bind the per-kind counter text into
// the three slots, then pick the slot-name format strings via the kind selector
// (a compare ladder plus a jump-table switch) and refresh the pane colors.
// ============================================================================
#pragma push
#pragma optimize_for_size on
void func_801F183C(CItemBoxLine* self, u32 itemData) {
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x413], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x443], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x44e], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x459], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x464], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x46f], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x47a], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x485], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x490], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x49b], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x4ca], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x4d6], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x4e2], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x4ee], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x4fb], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x508], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x515], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x51f], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x529], true), 1);

    // Kind-based counter text into the three slots.
    u32 table = lbl_eu_806640F8;
    func_801392E4(itemData);
    u16 kind = func_80139358(itemData);
    u8 a = func_801361E8(table, &lbl_eu_805071B0[0x59e], kind);
    u8 b = func_801361E8(table, &lbl_eu_805071B0[0x5a6], kind);
    u8 c = func_801361E8(table, &lbl_eu_805071B0[0x5d8], kind);
    func_80136910__FPQ34nw4r3lyt6LayoutPcUc(self->field40, &lbl_eu_805071B0[0x4ca], a);
    func_80136910__FPQ34nw4r3lyt6LayoutPcUc(self->field40, &lbl_eu_805071B0[0x4d6], b);
    func_80136910__FPQ34nw4r3lyt6LayoutPcUc(self->field40, &lbl_eu_805071B0[0x4e2], c);

    // Slot-name format ladder (kind selector). The range check and the three
    // equality tests each branch to their case body; the bodies sit after the
    // tests in retail order (0x32, 0x31, 0x30, 0x2e) with the default last.
    u32 selTable = lbl_eu_806640F8;
    u8 sel = func_801361E8(selTable, &lbl_eu_805071B0[0x26],
                           func_80139358(itemData));
    char* name;
    if ((u32)(sel - 4) <= 9) goto sel2e;
    if ((int)sel == 3) goto sel32;
    if ((int)sel == 2) goto sel31;
    if ((int)sel == 1) goto sel30;
    goto selElse;
sel32:
    name = func_80136190(&lbl_eu_805071B0[0x248], &lbl_eu_805071B0[0x6c], 0x32);
    goto selDone;
sel31:
    name = func_80136190(&lbl_eu_805071B0[0x248], &lbl_eu_805071B0[0x6c], 0x31);
    goto selDone;
sel30:
    name = func_80136190(&lbl_eu_805071B0[0x248], &lbl_eu_805071B0[0x6c], 0x30);
    goto selDone;
sel2e:
    name = func_80136190(&lbl_eu_805071B0[0x248], &lbl_eu_805071B0[0x6c], 0x2e);
    goto selDone;
selElse:
    name = 0;
selDone:
    func_80136B4C(self->field40, &lbl_eu_805071B0[0x490], name, 0);

    // Secondary slot-name switch (selectors 4..12 via jump table).
    char* name2 = 0;
    switch (sel) {
    case 4: name2 = func_80136190(&lbl_eu_805071B0[0x5e1], &lbl_eu_805071B0[0x6c], 0x77); break;
    case 5: name2 = func_80136190(&lbl_eu_805071B0[0x5e1], &lbl_eu_805071B0[0x6c], 0x78); break;
    case 6: name2 = func_80136190(&lbl_eu_805071B0[0x5e1], &lbl_eu_805071B0[0x6c], 0x79); break;
    case 7: name2 = func_80136190(&lbl_eu_805071B0[0x5e1], &lbl_eu_805071B0[0x6c], 0x7a); break;
    case 8: name2 = func_80136190(&lbl_eu_805071B0[0x5e1], &lbl_eu_805071B0[0x6c], 0x7b); break;
    case 9: name2 = func_80136190(&lbl_eu_805071B0[0x5e1], &lbl_eu_805071B0[0x6c], 0x7c); break;
    case 10: name2 = func_80136190(&lbl_eu_805071B0[0x5e1], &lbl_eu_805071B0[0x6c], 0x7d); break;
    case 11: name2 = func_80136190(&lbl_eu_805071B0[0x5e1], &lbl_eu_805071B0[0x6c], 0x7e); break;
    case 12: name2 = func_80136190(&lbl_eu_805071B0[0x5e1], &lbl_eu_805071B0[0x6c], 0x7f); break;
    }
    func_80136B4C(self->field40, &lbl_eu_805071B0[0x49b], name2, 0);

    // Refresh the slot pane colors.
    func_80139A18(self->field40, &lbl_eu_805071B0[0x443], lbl_eu_806645E8, lbl_eu_806645F0);
    func_80139A18(self->field40, &lbl_eu_805071B0[0x464], lbl_eu_806645D8, lbl_eu_806645E0);
    func_80139A18(self->field40, &lbl_eu_805071B0[0x46f], lbl_eu_806645D8, lbl_eu_806645E0);
    func_80139A18(self->field40, &lbl_eu_805071B0[0x44e], lbl_eu_806645E8, lbl_eu_806645F0);
    func_80139A18(self->field40, &lbl_eu_805071B0[0x459], lbl_eu_806645E8, lbl_eu_806645F0);
    func_80139A18(self->field40, &lbl_eu_805071B0[0x47a], lbl_eu_806645D8, lbl_eu_806645E0);
    func_80139A18(self->field40, &lbl_eu_805071B0[0x485], lbl_eu_806645D8, lbl_eu_806645E0);
    func_801F2434((void*)self, itemData);
}
#pragma pop


// ============================================================================
// func_801F08B4: dispatch on the active tab (tab entry type) and the item pick
// kind to route to a tab-specific layout refresh / page-navigation helper.
// ============================================================================
#pragma push
#pragma optimize_for_size on
void func_801F08B4(CItemBoxLine* self, u32 itemData) {
    func_801F0A58((void*)self, itemData);
    func_80136910__FPQ34nw4r3lyt6LayoutPcUc(self->field40, &lbl_eu_805071B0[0x3e6], func_80158068(itemData));
    switch ((int)self->tabEntries[(s8)self->field6D]) {
    case 2:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 0xd:
        func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x3f2], true), 1);
        break;
    case 3:
        func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x3fd], true), 1);
        break;
    case 0xa:
    case 0xb:
        func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x408], true), 1);
        break;
    }
    switch ((int)(func_801392E4(itemData) & 0xff)) {
    case 2:
        func_801F107C((void*)self, itemData);
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        func_801F183C((void*)self, itemData);
        break;
    case 3:
        func_801F1E64((void*)self, itemData);
        break;
    case 0xa:
        func_801F20F0((void*)self, itemData);
        break;
    case 0xd:
        func_801F2298((void*)self, itemData);
        break;
    }
}
#pragma pop

// ============================================================================
// func_801F20F0: show the page tab panes and fill the active item's name /
// category text into the syswin layout panes.
// ============================================================================
#pragma push
#pragma optimize_for_size on
void func_801F20F0(CItemBoxLine* self, u32 itemData) {
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x42b], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x561], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x56c], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x577], true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x583], true), 1);
    u32 owner = (u32)lbl_eu_80664104;
    func_801392E4(itemData);
    u16 cat = func_80139358(itemData);
    u8 v = func_801361E8(owner, &lbl_eu_805071B0[0x5ff], cat);
    char* name = func_80136190(&lbl_eu_805071B0[0x604], &lbl_eu_805071B0[0x6c], v);
    func_80136B4C(self->field40, &lbl_eu_805071B0[0x577], name, 0);
    u8 v2 = func_801361E8(owner, &lbl_eu_805071B0[0x610], cat);
    char* name2;
    if (0x1a == v2) {        name2 = func_80136190(&lbl_eu_805071B0[0x604], &lbl_eu_805071B0[0x6c], 0x14);
    } else {
        name2 = func_8013639C(lbl_eu_806640A8, &lbl_eu_805071B0[0x6c]);
    }
    func_80136B4C(self->field40, &lbl_eu_805071B0[0x583], name2, 0);
}
#pragma pop

// ============================================================================
// func_801F1E64: kind-3 item slot. Shows the five slot panes, resolves the
// kind-3 display name via the EC owner table (index 30-(v-1)), looks up the
// gem id (func_80136254) and per-gem strings: id==1 pins the fixed "none"
// string, otherwise the two D8-owner lookups fill the gem name / +21-indexed
// level name. Finally stores the category halfword + state 9 and copies the
// pane-anchor position into the +0x558 vec.
// ============================================================================
#pragma push
#pragma optimize_for_size on
void func_801F1E64(CItemBoxLine* self, u32 itemData) {
    char* pool = lbl_eu_805071B0;
    func_80124270(self->field40->GetRootPane()->FindPaneByName(pool + 0x41f, true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(pool + 0x533, true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(pool + 0x53e, true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(pool + 0x549, true), 1);
    func_80124270(self->field40->GetRootPane()->FindPaneByName(pool + 0x555, true), 1);
    func_801392E4(itemData);
    u16 cat = func_80139358(itemData);
    u32 vEC = func_801361E8((u32)lbl_eu_806640EC, pool + 0x5a, itemData);
    char* name = func_80136190(pool + 0x248, pool + 0x6c, 30 - ((u8)vEC - 1));
    func_80136B4C(self->field40, pool + 0x549, name, 0);
    u16 id = func_80136254((u32)lbl_eu_806640D8, pool + 0x5e9, cat);
    char* txt = func_80136190(pool + 0x63, pool + 0x6c, 15);
    if (id == 1) {
        txt = pool + 0x254;
        func_80136B4C(self->field40, pool + 0x555, txt, 0);
    }
    char* pool2 = lbl_eu_805071B0;
    char* gemName = func_8013639C((void*)lbl_eu_806640D8, pool2 + 0x6c);
    func_80136B4C(self->field40, pool2 + 0x53e, gemName, 0);
    char* lvlName = func_80136190(
        pool2 + 0x248, pool2 + 0x6c,
        (u8)func_801361E8((u32)lbl_eu_806640D8, pool2 + 0x5ed, cat) + 21);
    func_80136B4C(self->field40, pool2 + 0x5f4, lvlName, 0);
    nw4r::lyt::Pane* anchor =
        self->field40->GetRootPane()->FindPaneByName(pool2 + 0x53e, true);
    nw4r::lyt::Pane* ref =
        self->field40->GetRootPane()->FindPaneByName(pool2 + 0x3cc, true);
    nw4r::math::VEC3 pos;
    func_80137924(&pos, anchor, ref, self->field40->GetRootPane());
    *(s16*)((u8*)self + 0x4f0) = cat;
    *((u8*)self + 0x590) = 9;
    nw4r::math::VEC3 pos2 = pos;
    copyVEC3((nw4r::math::VEC3*)((u8*)self + 0x558), &pos2);
}
#pragma pop


// ============================================================================
// func_801F2298: kind-0xD item slot name. Shows the slot pane, resolves the
// gem/ferron entry via the owner tables (entry = table + 73*kindIndex +
// 2*nameIndex) and picks the display string: kind 1 requires byte +0xE8 set,
// kinds 2/3 require bit 6 / bit 5 of byte +0xE9, each falling back to the
// kind-agnostic default (index 44 vs 43), then binds it to the text pane.
// ============================================================================
#pragma push
#pragma optimize_for_size on
void func_801F2298(CItemBoxLine* self, u32 itemData) {
    func_80124270(self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x437], true), 1);
    u32 owner = (u32)lbl_eu_80664110;
    func_801392E4(itemData);
    u16 cat = func_80139358(itemData);
    u8 vA = func_801361E8(owner, &lbl_eu_805071B0[0x616], cat);
    u8 vB = func_801361E8(owner, &lbl_eu_805071B0[0xe], cat);
    u8 vC = func_801361E8(owner, &lbl_eu_805071B0[0x622], cat);
    u8 defName = func_8013600C(&lbl_eu_805071B0[0x5e1], &lbl_eu_805071B0[0x62b], vC);
    void* tbl = func_8009EC9C(vB);
    u8 idx = (u8)func_800A32BC();
    const u8* entry = (const u8*)tbl + idx * 73 + (defName << 1);
    char* str = func_80136190(&lbl_eu_805071B0[0x248], &lbl_eu_805071B0[0x6c], 44);
    switch (vA) {
    case 1:
        if (entry[232] != 0) {
            str = func_80136190(&lbl_eu_805071B0[0x248], &lbl_eu_805071B0[0x6c], 43);
        }
        break;
    case 2:
        if ((entry[233] >> 7) & 1) {
            str = func_80136190(&lbl_eu_805071B0[0x248], &lbl_eu_805071B0[0x6c], 43);
        }
        break;
    case 3:
        if ((entry[233] >> 6) & 1) {
            str = func_80136190(&lbl_eu_805071B0[0x248], &lbl_eu_805071B0[0x6c], 43);
        }
        break;
    }
    func_80136B4C(self->field40, &lbl_eu_805071B0[0x62f], str, 0);
}
#pragma pop

// ============================================================================
// func_801F2434: build the item-box page data.
// the pane, resolve the slot entry (itemId+kind via the owner tables) and bind
// a texture/name; then push the slot's kind/counters/position into the page
// arrays at index i+8 (guarded to 12) and refresh the pane hierarchy.
// ============================================================================
#pragma push
#pragma optimize_for_size on
void func_801F2434(CItemBoxLine* self, u32 itemData) {
    u32 itemId = func_801393CC(itemData);
    u16 kind = func_80139358(itemData);
    u8 n = func_801361E8(itemId, &lbl_eu_805071B0[0x106], (u16)kind);
    for (u8 i = 0; i < 3; i++) {
        char nameBuf[0x20];
        ml::FixStr<32> buf;
        buf.clear();
        sprintf(nameBuf, &lbl_eu_805071B0[0x63a], i + 1);
        nw4r::lyt::Pane* pane = self->field40->GetRootPane()->FindPaneByName(nameBuf, true);
        u16 kindV = 0;
        u8 b = 0;
        u8 a = 0;
        if (i < n) {
            func_80124270(pane, 1);
            u32 tex = 0;
            buf.format(&lbl_eu_805071B0[0x10f], i + 1);
            u16 v = func_80136254(itemId, buf.mString, (u16)kind);
            if (v != 0) {
                a = (u8)func_801361E8((u32)lbl_eu_806640EC, &lbl_eu_805071B0[0x649], (u16)v);
                b = (u8)func_801361E8((u32)lbl_eu_806640EC, &lbl_eu_805071B0[0x5a], (u16)v);
                u8 sel = (u8)func_801361E8((u32)lbl_eu_806640D8, &lbl_eu_805071B0[0x651],
                                          (u16)func_80139358(v));
                if (sel == 0) {
                    tex = self->field3C->vf01(0x74696d67, &lbl_eu_805071B0[0x65a], 0);
                } else if (sel == 4) {
                    tex = self->field3C->vf01(0x74696d67, &lbl_eu_805071B0[0x670], 0);
                } else if (sel == 5) {
                    tex = self->field3C->vf01(0x74696d67, &lbl_eu_805071B0[0x686], 0);
                } else if (sel == 6) {
                    tex = self->field3C->vf01(0x74696d67, &lbl_eu_805071B0[0x69c], 0);
                } else if (sel == 7) {
                    tex = self->field3C->vf01(0x74696d67, &lbl_eu_805071B0[0x6b2], 0);
                } else if (sel == 8) {
                    tex = self->field3C->vf01(0x74696d67, &lbl_eu_805071B0[0x6c8], 0);
                } else if (sel == 9) {
                    tex = self->field3C->vf01(0x74696d67, &lbl_eu_805071B0[0x6de], 0);
                }
                buf.format(&lbl_eu_805071B0[0x57], func_801394D4(v));
                kindV = func_80139358(v);
            } else {
                tex = self->field3C->vf01(0x74696d67, &lbl_eu_805071B0[0x65a], 0);
                buf.format(&lbl_eu_805071B0[0x57],
                           func_80136190(&lbl_eu_805071B0[0x248], &lbl_eu_805071B0[0x6c], 0x2a));
            }
            sprintf(nameBuf, &lbl_eu_805071B0[0x6f4], i + 1);
            func_80136B4C(self->field40, nameBuf, buf.mString, 0);
            if (tex != 0) {
                func_80137F88(pane, tex);
            }
        } else {
            sprintf(nameBuf, &lbl_eu_805071B0[0x6f4], i + 1);
            func_80136B4C(self->field40, nameBuf, &lbl_eu_805071B0[0x254], 0);
            func_80124270(pane, 0);
        }
        nw4r::lyt::Pane* find = self->field40->GetRootPane()->FindPaneByName(&lbl_eu_805071B0[0x3cc], true);
        CIBLVec3 pos;
        func_80137924((nw4r::math::VEC3*)&pos, pane, find, self->field40->GetRootPane());
        u8 idx = (u8)(i + 8);
        CIBLPageData* page = reinterpret_cast<CIBLPageData*>(&self->mInfo2D0[0xB0]);
        if (idx < 12) {
            page->pageWords4E0[idx] = kindV;
        }
        if (idx < 12) {
            page->pageBytes588[idx] = 3;
        }
        if (idx < 12) {
            page->pageBytes594[idx] = b;
        }
        if (idx < 12) {
            page->pageShorts5A0[idx] = a;
        }
        CIBLVec3 tmp;
        tmp.x = pos.x;
        tmp.y = pos.y;
        tmp.z = pos.z;
        if (idx < 12) {
            copyVEC3(&page->pageVecs4F8[idx], &tmp);
        }
    }
}
#pragma pop

// ============================================================================
// func_801F2880: kind/availability probe. Resolve the item's kind via the
// owner tables, index into the 0x49-byte character-data rows with the page
// lookup, and report whether the flagged slot (kind-dependent bit in +0xE8/
// +0xE9 of the row entry) is set. The first parameter is unused (retail keeps
// it in r3 and works from r4).
// ============================================================================
int func_801F2880(u32 unused, u32 key) {
    u32 table = lbl_eu_80664110;
    func_801392E4(key);
    // No named pool local: retail CSEs the lbl_eu_805071B0 base into a
    // temp register that is later reused for the func_8013600C result.
    u16 v = func_80139358(key);
    u8 a = func_801361E8(table, &lbl_eu_805071B0[0x616], v);
    u8 b = func_801361E8(table, &lbl_eu_805071B0[0xe], v);
    u8 c = func_801361E8(table, &lbl_eu_805071B0[0x622], v);
    u8 d = func_8013600C(&lbl_eu_805071B0[0x5e1], &lbl_eu_805071B0[0x62b], c);
    u8* base = (u8*)func_8009EC9C(b);
    u32 f = (u8)func_800A32BC();
    u32 d2 = (u32)d << 1;
    u32 prod = f * 0x49;
    u8* p = base + prod + d2;
    switch (a) {
    case 1:
        if (p[0xe8] != 0) return 1;
        break;
    case 2:
        if ((p[0xe9] >> 7) & 1) return 1;
        break;
    case 3:
        if ((p[0xe9] >> 6) & 1) return 1;
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
#pragma push
#pragma optimize_for_size on
bool CItemBoxLine::OnFileEvent(CEventFile* evt) {
    if (this->field24 == (u32)evt->mFileHandle) {
        reinterpret_cast<UnkClass_8045F564*>(&this->field04)->createRegion(
            mtl::MemManager::getHandleMEM2(), 0x18000, &lbl_eu_805071B0[0x700], 0);
        Class_8045F858 host(reinterpret_cast<UnkClass_8045F564*>(&this->field04));
        CFileHandle* fh = (CFileHandle*)this->field24;
        u8* data = (u8*)fh->getData();
        mtl::MemManager::func_80434A4C(false);
        void* tpMem = mtl::MemManager::allocate(0x858,
                                                getAllocHandle__10CLibLayoutFv());
        // The ctor returns self in r3, so the store can reuse r3 after the call
        // (retail: cmpwi/beq/bl __ct__CTagProcessor; stw r3).
        this->field54 = (CItemBoxLineDtorView*)(tpMem != 0 ? __ct__CTagProcessor(tpMem) : tpMem);
        nw4r::lyt::ArcResourceAccessor* acc =
            createArcResourceAccessor__10CLibLayoutFv();
        this->field38 = (CItemBoxLineResView*)acc;
        acc->Attach(data, &lbl_eu_805071B0[0x70d]);
        func_80136E84(&this->field40, (nw4r::lyt::ArcResourceAccessor*)this->field38,
                      &lbl_eu_805071B0[0x711]);
        func_80136F08(this->field40, &this->field44,
                      (nw4r::lyt::ArcResourceAccessor*)this->field38,
                      &lbl_eu_805071B0[0x728]);
        func_80136F08(this->field40, &this->field48,
                      (nw4r::lyt::ArcResourceAccessor*)this->field38,
                      &lbl_eu_805071B0[0x742]);
        nw4r::lyt::Pane* root = this->field40->GetRootPane();
        void* fontObj = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1,
                                                                          this->field40);
        func_8013676C(root, reinterpret_cast<CItemBoxFontView*>(fontObj)->vf7());

        // Seed the seven slot-name textboxes with the line text.
        char* text = func_801355BC();
        for (u8 i = 1; i <= 7; i++) {
            char buf[0x20];
            sprintf(buf, &lbl_eu_805071B0[0x3aa], i);
            func_801368C0(this->field40, buf, (u32)text);
        }
        this->field40->SetAnimationEnable(this->field48, false);
        this->field40->SetAnimationEnable(this->field44, true);
        this->field40->Animate(0);

        // Bind the tag processor to the text pane, seed the nameplate colour.
        nw4r::lyt::TextBox* tagPane =
            (nw4r::lyt::TextBox*)this->field40->GetRootPane()->
                FindPaneByName(&lbl_eu_805071B0[0x3c0], true);
        if (tagPane != 0) {
            tagPane->SetTagProcessor((nw4r::ut::WideTagProcessor*)this->field54);
        }
        nw4r::lyt::Pane* namePane = this->field40->GetRootPane()->
                                        FindPaneByName(&lbl_eu_805071B0[0x761], true);
        if (namePane != 0) {
            CIBLColorSrc* src = reinterpret_cast<CIBLNameplateView*>(namePane)->vf24();
            GXColorS10 colorTmp = src->color;
            __as__11_GXColorS10FRC11_GXColorS10(&this->field396, &colorTmp);
        }

        // Bind the line text into the sixteen label panes.
        char* text2 = func_801355BC();
        func_801368C0(this->field40, &lbl_eu_805071B0[0x4a6], (u32)text2);
        func_801368C0(this->field40, &lbl_eu_805071B0[0x4ca], (u32)text2);
        func_801368C0(this->field40, &lbl_eu_805071B0[0x4d6], (u32)text2);
        func_801368C0(this->field40, &lbl_eu_805071B0[0x4b2], (u32)text2);
        func_801368C0(this->field40, &lbl_eu_805071B0[0x4be], (u32)text2);
        func_801368C0(this->field40, &lbl_eu_805071B0[0x4e2], (u32)text2);
        func_801368C0(this->field40, &lbl_eu_805071B0[0x549], (u32)text2);
        func_801368C0(this->field40, &lbl_eu_805071B0[0x555], (u32)text2);
        func_801368C0(this->field40, &lbl_eu_805071B0[0x3e6], (u32)text2);
        func_801368C0(this->field40, &lbl_eu_805071B0[0x76b], (u32)text2);
        func_801368C0(this->field40, &lbl_eu_805071B0[0x775], (u32)text2);
        func_801368C0(this->field40, &lbl_eu_805071B0[0x77f], (u32)text2);
        func_801368C0(this->field40, &lbl_eu_805071B0[0x789], (u32)text2);
        func_801368C0(this->field40, &lbl_eu_805071B0[0x793], (u32)text2);
        func_801368C0(this->field40, &lbl_eu_805071B0[0x79d], (u32)text2);
        func_801368C0(this->field40, &lbl_eu_805071B0[0x7a7], (u32)text2);

        // Per-kind slot name providers (index into the shared name table).
        func_80136B4C(this->field40, &lbl_eu_805071B0[0x443],
                      func_80136190(&lbl_eu_805071B0[0x248], &lbl_eu_805071B0[0x6c], 0xa), 0);
        func_80136B4C(this->field40, &lbl_eu_805071B0[0x44e],
                      func_80136190(&lbl_eu_805071B0[0x248], &lbl_eu_805071B0[0x6c], 0xd), 0);
        func_80136B4C(this->field40, &lbl_eu_805071B0[0x459],
                      func_80136190(&lbl_eu_805071B0[0x248], &lbl_eu_805071B0[0x6c], 0x11), 0);
        func_80136B4C(this->field40, &lbl_eu_805071B0[0x464],
                      func_80136190(&lbl_eu_805071B0[0x248], &lbl_eu_805071B0[0x6c], 0x12), 0);
        func_80136B4C(this->field40, &lbl_eu_805071B0[0x46f],
                      func_80136190(&lbl_eu_805071B0[0x248], &lbl_eu_805071B0[0x6c], 0xc), 0);
        func_80136B4C(this->field40, &lbl_eu_805071B0[0x47a],
                      func_80136190(&lbl_eu_805071B0[0x248], &lbl_eu_805071B0[0x6c], 0x13), 0);
        func_80136B4C(this->field40, &lbl_eu_805071B0[0x533],
                      func_80136190(&lbl_eu_805071B0[0x248], &lbl_eu_805071B0[0x6c], 0x18), 0);
        func_80136B4C(this->field40, &lbl_eu_805071B0[0x561],
                      func_80136190(&lbl_eu_805071B0[0x248], &lbl_eu_805071B0[0x6c], 0x24), 0);
        func_80136B4C(this->field40, &lbl_eu_805071B0[0x56c],
                      func_80136190(&lbl_eu_805071B0[0x248], &lbl_eu_805071B0[0x6c], 0x25), 0);
        func_80136B4C(this->field40, &lbl_eu_805071B0[0x485],
                      func_80136190(&lbl_eu_805071B0[0x248], &lbl_eu_805071B0[0x6c], 0x2f), 0);
        func_80136B4C(this->field40, &lbl_eu_805071B0[0x7b1],
                      func_80136190(&lbl_eu_805071B0[0x604], &lbl_eu_805071B0[0x6c], 0x10), 0);

        // Clear three auxiliary labels.
        func_80136B4C(this->field40, &lbl_eu_805071B0[0x515], &lbl_eu_805071B0[0x254], 0);
        func_80136B4C(this->field40, &lbl_eu_805071B0[0x51f], &lbl_eu_805071B0[0x254], 0);
        func_80136B4C(this->field40, &lbl_eu_805071B0[0x529], &lbl_eu_805071B0[0x254], 0);

        // Seed the .sbss colour tables from the pane material colours and copy
        // the alpha shorts into the paired highlight entries.
        nw4r::lyt::Pane* pane443 = this->field40->GetRootPane()->
                                       FindPaneByName(&lbl_eu_805071B0[0x443], true);
        FourShorts c0 = func_801397AC((void*)pane443, 0);
        __as__11_GXColorS10FRC11_GXColorS10((GXColorS10*)&lbl_eu_806645D8[0],
                                            (const GXColorS10*)&c0);
        FourShorts c1 = func_801397AC((void*)pane443, 1);
        __as__11_GXColorS10FRC11_GXColorS10((GXColorS10*)&lbl_eu_806645E0[0],
                                            (const GXColorS10*)&c1);
        *(s16*)&lbl_eu_806645E8[6] = *(s16*)&lbl_eu_806645D8[6];
        *(s16*)&lbl_eu_806645F0[6] = *(s16*)&lbl_eu_806645E0[6];

        nw4r::lyt::Pane* pane4A6 = this->field40->GetRootPane()->
                                       FindPaneByName(&lbl_eu_805071B0[0x4a6], true);
        FourShorts c2 = func_801397AC((void*)pane4A6, 0);
        __as__11_GXColorS10FRC11_GXColorS10((GXColorS10*)&lbl_eu_806645F8[0],
                                            (const GXColorS10*)&c2);
        FourShorts c3 = func_801397AC((void*)pane4A6, 1);
        __as__11_GXColorS10FRC11_GXColorS10((GXColorS10*)&lbl_eu_80664600[0],
                                            (const GXColorS10*)&c3);
        *(s16*)&lbl_eu_80664608[6] = *(s16*)&lbl_eu_806645F8[6];
        *(s16*)&lbl_eu_80664610[6] = *(s16*)&lbl_eu_80664600[6];

        nw4r::lyt::Pane* pane76B = this->field40->GetRootPane()->
                                       FindPaneByName(&lbl_eu_805071B0[0x76b], true);
        FourShorts c4 = func_801397AC((void*)pane76B, 0);
        __as__11_GXColorS10FRC11_GXColorS10((GXColorS10*)&lbl_eu_80664618[0],
                                            (const GXColorS10*)&c4);
        FourShorts c5 = func_801397AC((void*)pane76B, 1);
        __as__11_GXColorS10FRC11_GXColorS10((GXColorS10*)&lbl_eu_80664620[0],
                                            (const GXColorS10*)&c5);
        *(s16*)&lbl_eu_80664628[6] = *(s16*)&lbl_eu_80664618[6];
        *(s16*)&lbl_eu_80664638[6] = *(s16*)&lbl_eu_80664618[6];
        *(s16*)&lbl_eu_80664630[6] = *(s16*)&lbl_eu_80664620[6];
        *(s16*)&lbl_eu_80664640[6] = *(s16*)&lbl_eu_80664620[6];

        // Build the four embedded cursors from stack temps (copy + teardown).
        u8 cur70[0x18];
        __ct__CCur18((CBaseCur*)cur70, func_801355F4());
        func_8018B0FC(&this->mCur70, cur70);
        __dt__6CCur18Fv((CBaseCur*)cur70, -1);
        reinterpret_cast<CIBLCur70View*>(&this->mCur70)->vf00();

        u8 cur88[0x18];
        __ct__CCur18((CBaseCur*)cur88, func_801355F4());
        func_8018B0FC(&this->mCur88, cur88);
        __dt__6CCur18Fv((CBaseCur*)cur88, -1);
        reinterpret_cast<CIBLCur70View*>(&this->mCur88)->vf00();

        u8 curA0[0x18];
        __ct__CIBLTabCur((CBaseCur*)curA0, this->field38);
        func_8018B0FC(&this->mCurA0, curA0);
        __dt__8CBaseCurFv((CBaseCur*)curA0, 0);
        reinterpret_cast<CIBLCur70View*>(&this->mCurA0)->vf00();

        u8 curB8[0x18];
        __ct__CSubCur((CBaseCur*)curB8, (nw4r::lyt::ArcResourceAccessor*)this->field38);
        func_8018B0FC(&this->mCurB8, curB8);
        __dt__7CSubCurFv((CBaseCur*)curB8, -1);
        reinterpret_cast<CIBLCur70View*>(&this->mCurB8)->vf00();

        func_801D2E4C(&this->mCurB8, 0);
        func_801EF518(this);
        this->field24 = 0;
        reinterpret_cast<UnkClass_8045F564*>(&this->field04)->func_8045F810();
        return true;
    }
    if (this->field28 == (u32)evt->mFileHandle) {
        reinterpret_cast<UnkClass_8045F564*>(&this->pad_14[0])->createRegion(
            mtl::MemManager::getHandleMEM2(), 0x100, &lbl_eu_805071B0[0x7ba], 0);
        Class_8045F858 host2(reinterpret_cast<UnkClass_8045F564*>(&this->pad_14[0]));
        CFileHandle* fh28 = (CFileHandle*)this->field28;
        u8* data28 = (u8*)fh28->getData();
        mtl::MemManager::func_80434A4C(false);
        nw4r::lyt::ArcResourceAccessor* acc2 =
            createArcResourceAccessor__10CLibLayoutFv();
        this->field3C = (CItemBoxLineResView*)acc2;
        acc2->Attach(data28, &lbl_eu_805071B0[0x70d]);
        func_801EF518(this);
        this->field28 = 0;
        reinterpret_cast<UnkClass_8045F564*>(&this->pad_14[0])->func_8045F810();
        return true;
    }
    if (this->field2C == (u32)evt->mFileHandle) {
        CFileHandle* fh2C = (CFileHandle*)this->field2C;
        u8* data2C = (u8*)fh2C->getData();
        func_8003AA34();
        void* fpA = getFP__FPCc(&lbl_eu_805071B0[0x7ca]);
        if (fpA == 0) {
            CBdat::func_8003AA78(2, data2C);
        }
        func_8003AA34();
        lbl_eu_80664648 = (u32)getFP__FPCc(&lbl_eu_805071B0[0x7ca]);
        func_8003AA34();
        lbl_eu_8066464C = (u32)getFP__FPCc(&lbl_eu_805071B0[0x7d9]);
        func_801EF518(this);
        this->field2C = 0;
        return true;
    }
    if (this->field30 == (u32)evt->mFileHandle) {
        CFileHandle* fh30 = (CFileHandle*)this->field30;
        u8* data30 = (u8*)fh30->getData();
        this->field34 = data30;
        func_8003AA34();
        void* fpB = getFP__FPCc(&lbl_eu_805071B0[0x1f0]);
        if (fpB == 0) {
            CBdat::func_8003AA78(5, this->field34);
        }
        lbl_eu_80664650 = (u32)getFP__FPCc(&lbl_eu_805071B0[0x1f0]);
        func_801EF518(this);
        this->field30 = 0;
        return true;
    }
    return false;
}
#pragma pop

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// ============================================================================
// sinit_801F32EC: static initializer for the fourteen 8-byte .sbss color
// objects at 0x806645D8..0x80664640 - reset via func_801D1F9C(ptr, 0) or set
// the RGBA via func_801C4B60(ptr, r, g, b, 0), in retail call order.
// ============================================================================
void sinit_801F32EC() {
    func_801D1F9C(&lbl_eu_806645D8, 0);
    func_801D1F9C(&lbl_eu_806645E0, 0);
    func_801C4B60(&lbl_eu_806645E8, 0x80, 0x80, 0x80, 0);
    func_801C4B60(&lbl_eu_806645F0, 0x80, 0x80, 0x80, 0);
    func_801D1F9C(&lbl_eu_806645F8, 0);
    func_801D1F9C(&lbl_eu_80664600, 0);
    func_801C4B60(&lbl_eu_80664608, 0xff, 0xff, 0xfa, 0);
    func_801C4B60(&lbl_eu_80664610, 0x80, 0x80, 0x80, 0);
    func_801D1F9C(&lbl_eu_80664618, 0);
    func_801D1F9C(&lbl_eu_80664620, 0);
    func_801C4B60(&lbl_eu_80664628, 0x12, 0xa3, 0xe7, 0);
    func_801C4B60(&lbl_eu_80664630, 0xff, 0xff, 0xff, 0);
    func_801C4B60(&lbl_eu_80664638, 0xb3, 0x9, 0xc0, 0);
    func_801C4B60(&lbl_eu_80664640, 0xff, 0xff, 0xff, 0);
}

// noinline: retail func_801EBAD4 calls these as EXTERNAL relocs (they live in
// a different retail object of this merged unit); without it MWCC -inline auto
// inlines the same-TU definitions and the call-site relocs disappear.
extern "C" __declspec(noinline) CIBLTabEntry* func_801EBB88(CIBLTabEntry* self, u16 r4, u32 r5, u8 r6, u8 r7) {
    self->f0 = r4;
    self->f4 = r5;
    self->f8 = r6;
    self->f9 = r7;
    return self;
}

// ============================================================================
// func_801EBAD4: initialise a tab-format object - zero every entry via the
// entry writer, clear the count/selector/name/counter fields, then re-copy
// zero entries over the table through a stack temp (retail keeps the r3
// reuse from func_801EBB88 for the copy source).
// ============================================================================
void func_801EBAD4(CIBLTabFull* self) {
    CIBLTabEntry* e = self->entries;
    do {
        func_801EBB88(e, 0, 0, 0, 0);
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
        CIBLTabEntry* src = func_801EBB88(&tmp, 0, 0, 0, 0);
        func_801EBB9C(&self->entries[i], src);
    }
}
// ============================================================================
// func_801EC284: page-slot probe on a tab-format entry. Start from 99, then
// scan the entry's per-page capacity (f4) for the smallest page whose
// cumulative total exceeds the current count (func_801571FC), returning the
// page's item offset rounded via the float path; finally clamp against the
// kind-owner count (field92 == 0xD uses the item-name owner table instead).
// ============================================================================
u8 func_801EC284(CIBLTabFull* self, u32 index) {
    int result = 0x63;
    if (index < self->count) {
        u16 f0 = self->entries[index].f0;
        CIBLTabEntry* entry = &self->entries[index];
        u32 cur = (u32)func_801571FC();
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
            int v2 = 0x63 - (int)func_80158068(f0);
            if (v2 < result) result = v2;
        } else {
            int v = (int)func_80157CD0(self->field92);
            if (v < result) result = v;
        }
    }
    return (u8)result;
}
