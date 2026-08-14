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
extern "C" void func_801EBB9C(CIBLTabEntry* dest, const CIBLTabEntry* src);
u8 func_801EC23C(const CIBLTab* self, unsigned int index);
u8 func_801EC8B4(const CIBLTab* self, unsigned int index);
void func_801EDA08(CItemBoxLine* self);
void func_801F1E64(CItemBoxLine* self, u32 itemData);
void func_801F20F0(CItemBoxLine* self, u32 itemData);
void func_801F2298(CItemBoxLine* self, u32 itemData);

void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
int sprintf(char*, const char*, ...);
void func_80136B4C(nw4r::lyt::Layout*, char*, char*, u32);
void func_80136910(nw4r::lyt::Layout*, char*, u8);
char* func_80136190(char*, char*, u32);
char* func_8013639C(void*, char*);
extern u32 func_80158068(u16);

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
void func_801EC438(){}

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
        u32 obj = lbl_eu_806640F8;
        if ((u32)sel == 2) {
            obj = lbl_eu_806640F4;
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

void func_801EC9E0(){}

// ============================================================================
// func_801ECC10: build the item-box line layout - load the layout arc, bind
// the two animation transforms, stop all running animations, then run the
// cursor deactivation tail.
// ============================================================================
void func_801ECC10(CItemBoxLine* self) {
    func_80136E84(&self->field08, self->field04, &lbl_eu_805071B0[0x121]);
    func_80136F08(self->field08, &self->field0C, self->field04, &lbl_eu_805071B0[0x139]);
    func_80136F08(self->field08, &self->field10, self->field04, &lbl_eu_805071B0[0x156]);
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

// ============================================================================
// func_801ED31C: load the item-box line's four files (MEM2 handle via
// getHandleMEM2, the third through the common archive) with this as the load
// event, then initialise the info2 state, num-select and scroll-bar members,
// and finally dispatch the syswin's last vtable slot (index 32) to finish.
// ============================================================================
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

void func_801ED4FC(void* self, void* drawInfo) {
    unsigned char* p = (unsigned char*)self;
    if (p[0x4c] == 0) return;
    drawItemBox2Layout__FP13CItemBoxInfo2PQ34nw4r3lyt8DrawInfo(p + 0xd0, drawInfo);
    func_80137038(*(nw4r::lyt::Layout**)(p + 0x40), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
    int r31 = 0;
    if (func_801D2ED8((CBaseCur*)(p + 0xb8)) == 0 && func_801EB020(p + 0x2dc) == 0) r31 = 1;
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

void func_801ED864(CItemBoxLine* self) {
    if (self->field50 != 0) return;
    self->field50 = 1;
    self->field4C = 1;
    self->unk59 = 0;
    self->unk39E = 0;
    func_801E1498(&self->mInfo2D0[0]);
    u8 idx = (u8)(self->unk38C + self->unk38E);
    u8 v = func_801EC8B4((void*)&self->unk3A4, idx);
    func_801E14DC(&self->mInfo2D0[0], (u16)func_801EC3B0(&self->unk3A4, idx), 0, self->field39F, v);
    func_801E16F0(&self->mInfo2D0[0], 0, (char*)func_801EC3D0(&self->unk3A4, idx));
    func_801F08B4((void*)self, (u16)func_801EC3B0(&self->unk3A4, idx));
    float vec[3];
    func_801F3670(&self->mScrollBar310[0],
                  (const float*)code80135FDC_setVec3(vec, lbl_eu_8066811C, lbl_eu_80668120, lbl_eu_806680F8));
    func_801F36BC(&self->mScrollBar310[0], 7, self->unk3A4.count);
    func_801F367C(&self->mScrollBar310[0]);
}

void CItemBoxLine::func_801EDA4C(unsigned char val) {
    unsigned char n = reinterpret_cast<unsigned char*>(this)[0x63];
    if (n >= 9) {
        return;
    }
    reinterpret_cast<unsigned char*>(this)[0x5a + n] = val;
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
        u8 count;
        int j = (s8)(self->field6D - 1);
        if (j < 0) j = (s8)(self->tabCount - 1);
        count = self->tabCount;
        int last = count - 1;
        u8 i = 0;
        while (i < count) {
            if (self->unk64[(s8)j] != 0) {
                self->field6D = (u8)j;
                break;
            }
            j = j - 1;
            if ((s8)j < 0) j = (s8)last;
            i++;
        }
    }
    self->unk38E = 0;
    func_801F061C((void*)self, 1);
    func_801EFFC4((void*)self);
    func_801F071C((void*)self);
    func_80138078__FUl(0x70);
    func_801F36BC(&self->mScrollBar310[0], 7, self->unk3A4.count);
}

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

void func_801EE788(){}

u8 func_801EECC0(void* self) { return static_cast<CItemBoxLine*>(self)->unk39E; }

void CItemBoxLine::func_801EECC8() {
    func_801EC3B0(&unk3A4, (unsigned char)(unk38C + unk38E));
}

void func_801D2E4C(void* self);
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

void func_801EEDF8(){}

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

void func_801EF518(){}

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

void func_801EF954(){}

void func_801EFB24(){}

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

void func_801EFE6C(){}

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
    func_801F0030(self);
    func_801F0488(self);
    return getItemBox2State__FP13CItemBoxInfo2((u8*)self + 0xd0);
}

/* Placeholder bodies removed: func_801F0030 / func_801F0488 / func_801F071C
 * are external retail symbols (resolved via the symbol map). */
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
    u8 v = (u8)func_801361E8((u32)lbl_eu_80664104, &lbl_eu_805071B0[0x5ff], (u16)cat);
    char* name = (char*)func_80136190(&lbl_eu_805071B0[0x604], &lbl_eu_805071B0[0x6c], v);
    func_80136B4C(layout, &lbl_eu_805071B0[0x577], name, 0);
    u8 v2 = (u8)func_801361E8((u32)lbl_eu_80664104, &lbl_eu_805071B0[0x610], (u16)cat);
    char* name2;
    if (v2 == 0x1a) {
        name2 = (char*)func_80136190(&lbl_eu_805071B0[0x604], &lbl_eu_805071B0[0x6c], 0x14);
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
    self->field94 = 0;
    self->fieldB4 = 0;
    self->fieldB8 = 0;
    self->field138 = 0;
    CIBLTabEntry tmp;
    for (u16 i = 0; i < 12; i++) {
        CIBLTabEntry* src = func_801EBB88(&tmp, 0, 0, 0, 0);
        func_801EBB9C(&self->entries[i], src);
    }
}
extern "C" void func_801EC284() {}
