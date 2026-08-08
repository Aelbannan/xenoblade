// Auto-scaffolded catalog TU for kyoshin/CItemBoxLine
// High-level C/C++ reconstruction from retail PPC.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CItemBoxLine.hpp"
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_pane.h>

// Tab entry accessors (retail symbols are plain func_XXXX names).
u8 func_801EF034(const CIBLTabCur* self, unsigned int index);
u16 func_801EC3B0(const CIBLTab* self, unsigned int index);
void func_801EBB9C(CIBLTabEntry* dest, const CIBLTabEntry* src);
u8 func_801EC23C(const CIBLTab* self, unsigned int index);
u32 func_801EC260(const CIBLTab* self, unsigned int index);
u8 func_801EC8B4(const CIBLTab* self, unsigned int index);
void func_801EDA08(CItemBoxLine* self);
void func_801F1E64(CItemBoxLine* self, u32 itemData);
void func_801F20F0(CItemBoxLine* self, u32 itemData);
void func_801F2298(CItemBoxLine* self, u32 itemData);

extern float lbl_eu_80668114;
void func_80136B4C(nw4r::lyt::Layout*, char*, char*, u32);
void func_80136910(nw4r::lyt::Layout*, char*, u8);
char* func_80136190(char*, char*, u32);
char* func_8013639C(void*, char*);
extern u32 func_80158068(u16);
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

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
// func_801EBB9C: copy a 12-byte tab entry
// ============================================================================
void func_801EBB9C(CIBLTabEntry* dest, const CIBLTabEntry* src) {
    dest->f0 = src->f0;
    dest->f4 = src->f4;
    dest->f8 = src->f8;
    dest->f9 = src->f9;
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

// func_801EC260: u32 at +0x4 (external retail symbol; declared above)

// func_801EC3B0: u16 at +0x0
u16 func_801EC3B0(const CIBLTab* self, unsigned int index) {
    if (index < self->count) {
        return self->entries[index].f0;
    }
    return 0;
}

// func_801EC8B4: u8 at +0x9 (default 1)
u8 func_801EC8B4(const CIBLTab* self, unsigned int index) {
    if (index < self->count) {
        return self->entries[index].f9;
    }
    return 1;
}

// ============================================================================
// CIBLTabCur vtable plumbing (retail ctor/dtor symbols are plain names)
// ============================================================================

// func_801EE840 / __ct__CIBLTabCur
extern "C" CBaseCur* __ct__CIBLTabCur(CBaseCur* _this) {
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
extern "C" void* __dt__801ECBC0(CBaseCur* _this, int flags) {
    if (_this != NULL) {
        __dt__8CBaseCurFv(_this, 0);
        if (flags > 0) {
            __dl__FPv(_this);
        }
    }
    return _this;
}

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
// func_801EC3D0: format entry index's name into a FixStr buffer; null if out of range.
// ============================================================================

// ============================================================================
// func_801EC438
// ============================================================================
void func_801EC438(){}

void func_801EC808(){}

void func_801EC8D8(){}

void func_801EC9E0(){}

void func_801ECC10(){}

void __ct__CItemBoxLine(){}

CItemBoxLine::~CItemBoxLine() {}

void func_801ED31C(){}

void func_801ED3E8(){}

void func_801ED4FC(void* self, void* drawInfo) {
    unsigned char* p = (unsigned char*)self;
    if (p[0x4c] == 0) return;
    drawItemBox2Layout__FP13CItemBoxInfo2PQ34nw4r3lyt8DrawInfo(p + 0xd0, drawInfo);
    func_80137038(*(nw4r::lyt::Layout**)(p + 0x40), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
    int r31 = 0;
    if (func_801D2ED8(p + 0xb8) == 0 && func_801EB020(p + 0x2dc) == 0) r31 = 1;
    if (r31 != 0) {
        if (CSysWin_getUnk34(p + 0x350) == 0) {
            func_801D20B0(p + 0x88, drawInfo);
            func_801D20B0(p + 0xa0, drawInfo);
        }
    }
    func_801D20B0(p + 0xb8, drawInfo);
    func_801F35B0(p + 0x310, drawInfo);
    func_801EAF7C(p + 0x2dc, drawInfo);
    func_8022B7C8(p + 0x350, drawInfo);
    int r30 = 0;
    if (CSysWin_getUnk34(p + 0x350) != 0 && *(int*)(p + 0x50) < 9) r30 = 1;
    if (r31 != 0 && r30 == 0) {
        func_801D20B0(p + 0x70, drawInfo);
    }
}

void func_801ED618(){}


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

void func_801ED864(){}

void func_801ED97C(){}

void CItemBoxLine::func_801EDA4C(unsigned char val) {
    unsigned char n = reinterpret_cast<unsigned char*>(this)[0x63];
    if (n >= 9) {
        return;
    }
    reinterpret_cast<unsigned char*>(this)[0x5a + n] = val;
    reinterpret_cast<unsigned char*>(this)[0x63] = n + 1;
}

void func_801EDA6C(){}

void func_801EDB80(){}

void func_801EDC94(){}

void func_801EDF40(){}

void func_801EE228(){}

void func_801EE448(){}

// ============================================================================
// func_801EE684: item-box line update. When the syswin overlay is armed the
// active tab is committed; otherwise a tab/cursor hint is advanced.
// ============================================================================
void func_801EE684(CItemBoxLine* self) {
    if (CSysWin_getUnk34(&self->mSysWin)) {
        if (CSysWin_isActive((char*)self + 0x350)) {
            func_8022B8E4(&((CSysWinFull*)((u8*)self + 0x350))[0]);
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
                func_801D216C((char*)self + 0xa0, 1);
            } else {
                func_801D216C((char*)self + 0x70, 1);
            }
            func_801D216C((char*)self + 0x88, 0);
            func_80138078__FUl(6);
        } else {
            if (func_801EB020(&self->mNumSel)) return;
            if (func_801EB028((char*)self + 0x2dc)) return;
            func_801EB178(&self->mNumSel);
            self->field50 = 8;
            func_80138078__FUl(6);
        }
    }
}

void func_801EE788(){}

u8 func_801EECC0(void* self) { return static_cast<CItemBoxLine*>(self)->unk39E; }

void CItemBoxLine::func_801EECC8() {
    func_801EC3B0(&unk3A4, (unsigned char)(unk38C + unk38E));
}

void func_801D2E4C(void* self);
void func_801EECE0(void* self) { ((void(*)(void*))func_801D2E4C)((char*)self + 0xb8); }

void func_801EECE8(){}

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
    void* obj = p + 0xb8;
    (*(void(**)(void*, void*))(*(void***)obj + 4))(obj, tmp);
    func_801EFFC4(self);
    func_80138078__FUl(0xa);
}

void func_801EEDF8(){}

void func_801EF050(){}

// ============================================================================
// func_801EF0EC: item-box focus resolver. Returns a cursor/focus id based on
// the current navigation state and tab position.
// ============================================================================
u8 func_801EF0EC(CItemBoxLine* self) {
    if (self->field50 >= 6) return 0;
    if (func_801EB020((char*)self + 0x2dc)) return 0;
    if (CSysWin_getUnk34((char*)self + 0x350)) return 0;
    if (func_801D2ED8((char*)self + 0xb8)) return 0xa;
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

void func_801EF1E4(){}

// ============================================================================
// func_801EF260: animate the item-box intro (0x48), store state 3, prime cursors
// and re-format/push the tab name overlay.
// ============================================================================
void func_801EF260(void* self) {
    unsigned char* p = (unsigned char*)self;
    if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(*(void**)(p + 0x48), lbl_eu_80668114) == 0) return;
    *(unsigned int*)(p + 0x50) = 3;
    p[0x59] = 1;
    func_801F0488(self);
    func_801D216C(p + 0x70, 1);
    func_801D216C(p + 0xb8, 1);
    unsigned char tmp[16];
    func_801E174C(tmp, p + 0xd0, p[0x39f]);
    void* vt = *(void***)(p + 0xb8);
    ((void (*)(void*, void*))((void**)vt)[4])(p + 0xb8, tmp);
    func_801F071C(self);
}

void func_801EF2FC(){}

void func_801EF378(){}

void func_801EF3E8(){}

// ============================================================================
// func_801EF45C: refresh the active tab - select from num-select, set the name,
// and advance to state 6. csvtab index = (0x38c + 0x38e) & 0xFF.
// ============================================================================
void func_801EF45C(void* self) {
    unsigned char* p = (unsigned char*)self;
    if (!CSysWin_isActive(p + 0x350)) return;
    *(unsigned int*)(p + 0x50) = 3;
    if ((signed char)p[0x3a3] != 0) return;
    unsigned char tabidx = (unsigned char)(*(short*)(p + 0x38c) + *(short*)(p + 0x38e));
    *(short*)(p + 0x392) = 1;
    CIBLTabFormat* tabs = (CIBLTabFormat*)(p + 0x3a4);
    void* name = func_801EC3D0(tabs, tabidx);
    func_801EB030(p + 0x2dc, name);
    func_801EB04C(p + 0x2dc, (unsigned char)*(short*)(p + 0x392));
    func_801EB064(p + 0x2dc, (int)*(short*)(p + 0x392) * (int)func_801EC260((const CIBLTab*)tabs, tabidx));
    func_801EB0D4(p + 0x2dc);
    *(unsigned int*)(p + 0x50) = 6;
}

void func_801EF518(){}

void func_801EF734(){}

void func_801EF844(){}

void func_801EF954(){}

void func_801EFB24(){}

void func_801EFDF4(){}

void func_801EFE6C(){}

/* Placeholder bodies removed: func_801EFFC4 / func_801F0030 / func_801F0488 /
 * func_801F071C are external retail symbols (resolved via the symbol map), so
 * the callees are declared above and defined in their owning unit. */
void func_801F061C(){}

void func_801F0A58(CItemBoxLine* self, u32 itemData){}

void func_801F107C(CItemBoxLine* self, u32 itemData){}

void func_801F183C(CItemBoxLine* self, u32 itemData){}

// ============================================================================
// func_801F08B4: dispatch on the active tab (tab entry type) and the item pick
// kind to route to a tab-specific layout refresh / page-navigation helper.
// ============================================================================
void func_801F08B4(CItemBoxLine* self, u32 itemData) {
    func_801F0A58(self, itemData);
    nw4r::lyt::Layout* layout = *(nw4r::lyt::Layout**)((u8*)self + 0x40);
    func_80136910(layout, &lbl_eu_805071B0[0x3e6], (u8)func_80158068(itemData));
    nw4r::lyt::Pane* root = *(nw4r::lyt::Pane**)((u8*)layout + 0x10);
    unsigned char v = self->tabEntries[(s8)self->field6D];
    switch (v) {
    case 2:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 0xd:
        func_80124270(root->FindPaneByName(&lbl_eu_805071B0[0x3f2], true), 1);
        break;
    case 3:
        func_80124270(root->FindPaneByName(&lbl_eu_805071B0[0x3fd], true), 1);
        break;
    case 0xa:
    case 0xb:
        func_80124270(root->FindPaneByName(&lbl_eu_805071B0[0x408], true), 1);
        break;
    }
    unsigned char sel = (u8)func_801392E4(itemData);
    if (sel >= 4 && sel <= 8) {
        func_801F183C(self, itemData);
    } else if (sel == 2) {
        func_801F107C(self, itemData);
    } else if (sel == 3) {
        func_801F1E64(self, itemData);
    } else if (sel == 0xa) {
        func_801F20F0(self, itemData);
    } else if (sel == 0xd) {
        func_801F2298(self, itemData);
    }
}

// ============================================================================
// func_801F20F0: show page tabs and fill the active item's name / category text
// into the syswin layout panes.
// ============================================================================
void func_801F20F0(CItemBoxLine* self, u32 itemData) {
    nw4r::lyt::Layout* layout = *(nw4r::lyt::Layout**)((u8*)self + 0x40);
    nw4r::lyt::Pane* root = *(nw4r::lyt::Pane**)((u8*)layout + 0x10);
    func_80124270(root->FindPaneByName(&lbl_eu_805071B0[0x42b], true), 1);
    func_80124270(root->FindPaneByName(&lbl_eu_805071B0[0x561], true), 1);
    func_80124270(root->FindPaneByName(&lbl_eu_805071B0[0x56c], true), 1);
    func_80124270(root->FindPaneByName(&lbl_eu_805071B0[0x577], true), 1);
    func_80124270(root->FindPaneByName(&lbl_eu_805071B0[0x583], true), 1);
    func_801392E4(itemData);
    u32 cat = func_80139358(itemData);
    u8 v = (u8)func_801361E8(lbl_eu_80664104, &lbl_eu_805071B0[0x5ff], (u16)cat);
    char* name = func_80136190(&lbl_eu_805071B0[0x604], &lbl_eu_805071B0[0x6c], v);
    func_80136B4C(layout, &lbl_eu_805071B0[0x577], name, 0);
    u8 v2 = (u8)func_801361E8(lbl_eu_80664104, &lbl_eu_805071B0[0x610], (u16)cat);
    char* name2;
    if (v2 == 0x1a) {
        name2 = func_80136190(&lbl_eu_805071B0[0x604], &lbl_eu_805071B0[0x6c], 0x14);
    } else {
        name2 = func_8013639C(lbl_eu_806640A8, &lbl_eu_805071B0[0x6c]);
    }
    func_80136B4C(layout, &lbl_eu_805071B0[0x583], name2, 0);
}

void func_801F1E64(CItemBoxLine* self, u32 itemData){}

void func_801F2298(CItemBoxLine* self, u32 itemData){}

void func_801F2434(){}

void func_801F2880(){}

void CItemBoxLine::OnFileEvent() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_801F32EC(){}

extern "C" void func_801EBB88(void* self, u16 r4, u32 r5, u8 r6, u8 r7) {
    *(u16*)self = r4;
    *(u32*)((u8*)self + 4) = r5;
    *((u8*)self + 8) = r6;
    *((u8*)self + 9) = r7;
}

extern "C" void func_801EBAD4() {}
extern "C" void func_801EBC00() {}
extern "C" void func_801EC284() {}
