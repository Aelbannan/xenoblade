// Auto-scaffolded catalog TU for kyoshin/makecrystal/CMCGetItemBox
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/makecrystal/CMCGetItemBox.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/device/CDeviceSC.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CEventFile.hpp"

#include <nw4r/lyt.h>
#include <cstdio>

// Resolve ml::FixStr<128>::format calls to the explicit specialization that
// CfScript.cpp defines (retail symbol format__Q22ml10FixStr<128>FPCce).
template <> void ml::FixStr<128>::format(const char* fmt, ...);

namespace nw4r { namespace lyt { class AnimTransform; } }

// ---- C-linkage / foreign retail helper declarations ----------------------
// Only unmangled retail symbols (confirmed via config/us/symbols.txt) are
// declared extern "C"; the mangled ones are declared as normal C++ so the
// Itanium-mangled reloc name is emitted (matches the retail reloc).
// Most imports live in the C-linkage imports section of CMCGetItemBox.hpp.
// The six below stay TU-local: func_801D216C / CSysWin_getUnk34 /
// CSysWin_isActive / func_8022B8E4 conflict with CModelDispMakeCrystal.cpp's
// own extern "C" decls ((void*, u8) / (void*) forms), and func_80137E7C /
// func_8013639C conflict with code_80135FDC.hpp's signatures - hoisting them
// would make that TU ill-formed.
extern "C" {
void func_801D216C(void*, int);
u32 CSysWin_getUnk34(CMCGetItemBoxSysWin*);
int  CSysWin_isActive(CMCGetItemBoxSysWin*);
void func_8022B8E4(CMCGetItemBoxSysWin*);
void func_8022B90C(CMCGetItemBoxSysWin*, int);
void func_8022B9B4(CMCGetItemBoxSysWin*, void*, int);
void func_8022BFC8(CMCGetItemBoxSysWin*, int);
void func_8022B8B8(CMCGetItemBoxSysWin*);
}
// C++-linkage (mangled) retail symbols.
void func_80136910(nw4r::lyt::Layout*, char*, u8);   // func_80136910__FPQ34nw4r3lyt6LayoutPcUc
void func_80138078(u32);                             // func_80138078__FUl
int getItemBoxState(CItemBoxInfo*);                    // getItemBoxState__FP12CItemBoxInfo (retail call sites compare the full 32-bit return)
u32 func_80137444(nw4r::lyt::AnimTransform*, float); // func_80137444__FPQ34nw4r3lyt13AnimTransformf
// Plain-C++ imports for the targets below. MWCC emits the Itanium-mangled
// reloc names at the call sites; the acceptance gate compares reloc sites
// (offset+type) only, not names.
void func_80136A1C(nw4r::lyt::Layout*, char*, char*, u32);
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
int  func_800A9D90();
void func_801D4054(CItemBoxInfo*);
void func_801D4154(CItemBoxInfo*);
void func_801D40C4(CItemBoxInfo*);
void func_801D202C(CMCItemBoxSubObj*);
void func_801D20B0(CMCItemBoxSubObj*, nw4r::lyt::DrawInfo*);
void func_8022B7C8(CMCGetItemBoxSysWin*, nw4r::lyt::DrawInfo*);
void func_8022B748(CMCGetItemBoxSysWin*);
void __dt__12CItemBoxInfoFv(CItemBoxInfo*, int);
void __dt__7CSysWinFv(CMCGetItemBoxSysWin*, int);
void __dt__6CCur18Fv(CMCItemBoxSubObj*, int);
void __dt__6CCur16Fv(CMCItemBoxSubObj*, int);
void __dt__6CCur09Fv(CMCItemBoxSubObj*, int);
void __dt__6CCur07Fv(CMCItemBoxSubObj*, int);
void __dt__17UnkClass_8045F564Fv(UnkClass_8045F564*, int);

// Dispatch shim for the CSysWin vtable entry at vtable offset 0x88. MWCC
// places the first user virtual at vtable offset 8, so declared slot 32
// (init) lands at 0x88.
struct CMCGetItemBoxSysWinInit {
    virtual void slot0() = 0; virtual void slot1() = 0; virtual void slot2() = 0;
    virtual void slot3() = 0; virtual void slot4() = 0; virtual void slot5() = 0;
    virtual void slot6() = 0; virtual void slot7() = 0; virtual void slot8() = 0;
    virtual void slot9() = 0; virtual void slot10() = 0; virtual void slot11() = 0;
    virtual void slot12() = 0; virtual void slot13() = 0; virtual void slot14() = 0;
    virtual void slot15() = 0; virtual void slot16() = 0; virtual void slot17() = 0;
    virtual void slot18() = 0; virtual void slot19() = 0; virtual void slot20() = 0;
    virtual void slot21() = 0; virtual void slot22() = 0; virtual void slot23() = 0;
    virtual void slot24() = 0; virtual void slot25() = 0; virtual void slot26() = 0;
    virtual void slot27() = 0; virtual void slot28() = 0; virtual void slot29() = 0;
    virtual void slot30() = 0; virtual void slot31() = 0;
    virtual void init() = 0;   // vtable offset 0x88
};

// Same-unit helper functions (retail relocs are unmangled -> C linkage).
// func_80296BF0's extra params carry defaults so func_80299530's 1-arg call
// (retail passes garbage r4/r5/r6 there) still compiles.
extern "C" void func_80296BF0(CMCItemBoxSub*, u8 = 0, CMCItemBoxEntry* = 0, u8 = 0);
extern "C" void func_8029967C(CMCGetItemBox*);
extern "C" void func_802998C8(CMCGetItemBox*);
void func_802988BC(CMCGetItemBox*);
void func_80298938(CMCGetItemBox*);
void func_802989A4(CMCGetItemBox*);
void func_80298A20(CMCGetItemBox*);
void func_80299490(CMCGetItemBox*, int, u32);
extern "C" void func_80298378(CMCGetItemBox*);
extern "C" void func_802983E4(CMCGetItemBox*);

// Initialise a CMCItemBoxSub: clear the offset table to 0xFFFF, zero the
// counters and index fields, then reset the whole table again.
void* __dt__80296BB0(CMCGetItemBox* _this, int flags) {
    if (_this != NULL && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

void func_80296B44(CMCItemBoxSub* x) {
    // Retail walks a pointer over the table (cmplw loop), not an unrolled int loop.
    s16* p = x->table;
    do {
        *p++ = -1;
    } while (p < x->table + 0x80);
    x->count = 0;
    x->pad_102 = 0;
    x->limit = 0;
    x->counter = 0;
    x->field_108 = 0;
    x->field_148 = 0;
    x->name.mString[0] = 0;
    x->name.mLength = 0;
    x->listBase = (CMCItemBoxEntry*)0;
    x->field_1D4 = 0;
    // Second pass over the same table: u16 index drives an mtctr/bdnz loop.
    for (u16 i = 0; i < 0x80; i++) x->table[i] = -1;
}

// Rebuild the item-box index table: store the entry base/type, reset the
// offset table to 0xFFFF, then re-index every entry whose type word matches
// pad_102, and finally compute the page limit as ceil(filled-count / 30).
extern "C" void func_80296BF0(CMCItemBoxSub* x, u8 pad102, CMCItemBoxEntry* listBase, u8 field1D4) {
    x->listBase = listBase;
    x->field_1D4 = field1D4;
    x->count = 0;
    x->pad_102 = pad102;
    for (u16 i = 0; i < 0x80; i++) x->table[i] = -1;
    for (u16 i = 0; (u16)i < x->field_1D4; i++) {
        CMCItemBoxEntry* e = &x->listBase[(u16)i];
        if (e != 0) {
            if (x->pad_102 == ((e->field_00 >> 12) & 0xF)) {
                u16 c = x->count;
                x->count = c + 1;
                x->table[c] = i;
            }
        }
    }
    // Page count = ceil(filled count / 30); each page holds up to 30 items.
    f32 v = (f32)x->count / lbl_eu_80668BD0;
    s32 t = (s32)v;              // floor since v >= 0 (fctiwz truncates)
    f32 frac = v - (f32)t;
    if (lbl_eu_80668BD4 != frac) t++;
    x->limit = (u8)t;
    if (x->limit == 0) x->limit = 1;
    x->counter = 0;
}

// Advance the counter; wrap to 0 when past the signed byte limit.
extern "C" __declspec(noinline) void func_80296D00(CMCItemBoxSub* x) {
    u8 v = (u8)(x->counter + 1);
    x->counter = v;
    if ((s8)v < x->limit) return;
    x->counter = 0;
}

// Count the counter down; wrap to (limit-1) when it goes negative.
extern "C" __declspec(noinline) void func_80296D2C(CMCItemBoxSub* x) {
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
    u16 idx = (u16)(index + (s8)x->counter * 30);
    if (idx < x->count) {
        s16 off = x->table[idx];
        CMCItemBoxEntry* p = base + off;
        if (p != 0) {
            return p->field_00 >> 20;
        }
    }
    return 0;
}

s8 func_80296E00(CMCItemBoxSub* x, u32 index) {
    CMCItemBoxEntry* base = x->listBase;
    if (base == 0) return 0;
    u16 idx = (u16)(index + (s8)x->counter * 30);
    if (idx < x->count) {
        CMCItemBoxEntry* p = base + x->table[idx];
        if (p != 0) {
            CMCItemImplShim* inst = (CMCItemImplShim*)CItem_initItemImplInstances(p);
            u32 r = inst->getKind(p);
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
    u16 idx = (u16)(index + (s8)sub->counter * 30);
    if (idx >= sub->count) return 0;
    s16 off = sub->table[idx];
    CMCItemBoxEntry* p = base + off;
    if (p == 0) return 0;
    char* b = (char*)lbl_eu_8050FF8C;
    CMCItemImplShim* inst = (CMCItemImplShim*)CItem_initItemImplInstances(p);
    ((ml::FixStr<64>*)((u8*)sub + 0x108))->format(&b[0], (char*)inst->getName(p));
    if (sub->pad_102 == 3) {
        CMCItemImplShim* inst2 = (CMCItemImplShim*)CItem_initItemImplInstances(p);
        u8 v = (u8)inst2->getKind(p);
        char* itemName = (char*)func_80136190(&b[3], &b[0xc], 0x1e - (v - 1));
        char copy[64];
        u32 len = strlen(((ml::FixStr<64>*)((u8*)sub + 0x108))->c_str());
        strcpy(copy, ((ml::FixStr<64>*)((u8*)sub + 0x108))->c_str());
        ((ml::FixStr<64>*)((u8*)sub + 0x108))->format(&b[0x11], copy, itemName);
    }
    return (char*)((u8*)sub + 0x108);
}

// Retail 0x802995F8: format the selected entry's item text into the
// FixStr<128> buffer at sub+0x14C and return it. Entries owned by the bdat
// manager (func_801C6E90 != 0) use the item-name database; otherwise the
// text is built from the message-string placeholders: '%1' splices the
// item icon name (switch on func_801361E8's kind), '%2' splices a kind-apped
// table string from lbl_eu_8050FF60, and the two-byte shift-JIS ranges are
// stepped over. The whole formatted buffer is then copied back into the
// FixStr (its mLength refreshed).
char* func_80296FC0(CMCItemBoxSub* sub, u16 index) {
    CMCItemBoxEntry* base = sub->listBase;
    if (base == 0) return 0;
    u16 idx = (u16)(index + (s8)sub->counter * 30);
    if (idx >= sub->count) return 0;
    CMCItemBoxEntry* p = base + sub->table[idx];
    if (p == 0) return 0;
    if (func_801C6E90(p) != 0) {
        // bdat-managed entry: name comes from the item database.
        void* inst = CItem_initItemImplInstances(p);
        u32 v = (u32)((u32(*)(void*, void*))(*(void***)inst)[0x22])(inst, p);
        char* s = func_8013639C((void*)lbl_eu_80664100, &lbl_eu_8050FF8C[0x16], (u16)v);
        sub->name.format(&lbl_eu_8050FF8C[0], s);
        return (char*)&sub->name;
    }
    u32 type = (p->field_00 >> 12) & 0xF;
    int special = 0;
    if (type == 9 && (p->bytes[3] & 3) == 3) special = 1;
    if (special) {
        char* s = func_80136190(&lbl_eu_8050FF8C[3], &lbl_eu_8050FF8C[0xc], 0x9c);
        sub->name.format(&lbl_eu_8050FF8C[0], s);
        return (char*)&sub->name;
    }
    u8 pad102 = sub->pad_102;
    u32 id = p->field_00 >> 20;
    if (pad102 == 3) {
        // Gem slot: icon/kind/name triple drives the message placeholders.
        void* db = (void*)lbl_eu_806640D8;
        void* inst1 = CItem_initItemImplInstances(p);
        u32 icon = (u32)((u32(*)(void*, void*))(*(void***)inst1)[0x15])(inst1, p);
        void* inst2 = CItem_initItemImplInstances(p);
        u8 kind = (u8)((u32(*)(void*, void*))(*(void***)inst2)[2])(inst2, p);
        void* inst3 = CItem_initItemImplInstances(p);
        char* nm = (char*)((void*(*)(void*, void*))(*(void***)inst3)[0x24])(inst3, p);
        char buf48[0x20];
        sprintf(buf48, &lbl_eu_8050FF8C[0x1e], nm);
        u8 mkind = (u8)func_801361E8((u32)db, &lbl_eu_8050FF8C[0x21], (u16)icon);
        char* s2 = func_8013639C((void*)db, &lbl_eu_8050FF8C[0x2a], (u16)icon);
        sub->name.format(&lbl_eu_8050FF8C[0], s2);
        u32 len = strlen(sub->name.c_str());
        char copy[0x80];
        strcpy(copy, sub->name.c_str());
        // Japanese/Korean text uses different message variants.
        int langB = 1;
        if (CDeviceSC::getLanguage() != 3 && CDeviceSC::getLanguage() != 2) langB = 0;
        CMCItemMsgTbl tbl;
        char fmtBuf[0x20];
        char tail[0x80];
        char* p2 = copy;
        while (*p2 != 0) {
            if (*p2 == '%') {
                memset(fmtBuf, 0, 0x20);
                p2++;
                if (*p2 == '1') {
                    p2++;
                    switch ((u8)mkind) {
                        case 1:
                        case 2: sprintf(fmtBuf, &lbl_eu_8050FF8C[0x36], buf48); break;
                        case 3:
                        case 4: sprintf(fmtBuf, langB ? &lbl_eu_8050FF8C[0x4c] : &lbl_eu_8050FF8C[0x65], buf48); break;
                    }
                } else if (*p2 == '2') {
                    p2++;
                    tbl = lbl_eu_8050FF60;
                    u8 s3 = func_801361E8((u32)db, (const char*)tbl.v[kind], (u16)icon);
                    sprintf(fmtBuf, langB ? &lbl_eu_8050FF8C[0x7d] : &lbl_eu_8050FF8C[0x96], s3);
                }
                // Splice the formatted text in place of the '%x' pair.
                sprintf(tail, &lbl_eu_8050FF8C[0], p2);
                p2 -= 2;
                sprintf(p2, &lbl_eu_8050FF8C[0], fmtBuf);
                p2 = p2 + (strlen(fmtBuf) + 1) - 1;
                sprintf(p2, &lbl_eu_8050FF8C[0], tail);
            } else if ((s8)*p2 >= 0x81 && (s8)*p2 <= 0x9f || (s8)*p2 >= 0xe0 && (s8)*p2 <= 0xef) {
                p2 += 2;   // shift-JIS lead byte: skip the trail byte too
            } else {
                p2 += 1;
            }
        }
        sub->name.mLength = (int)strlen(copy);
        strcpy(sub->name.mString, copy);
        return (char*)&sub->name;
    } else {
        // Non-gem slot: pick the text table by slot kind, then format.
        u8 v = (u8)(pad102 + 0xFC);
        void* tbl2 = (void*)lbl_eu_80664A1C;
        if (v <= 5 || pad102 == 2) tbl2 = (void*)lbl_eu_80664A18;
        u32 n = func_80136254((void*)lbl_eu_806640EC, &lbl_eu_8050FF8C[0x16], id);
        char* s = func_8013639C((void*)tbl2, &lbl_eu_8050FF8C[0x16], (u16)n);
        sub->name.format(&lbl_eu_8050FF8C[0], s);
        return (char*)&sub->name;
    }
}

CMCGetItemBox::CMCGetItemBox() {}

// Free-function dtor form: the member dtor's implicit vptr re-store (lis/addi/stw
// of __vt__13CMCGetItemBox) is not in the retail; the extern-C form skips it.
// The retail frame is stmw r30 (pragma below) with the outer this-guard,
// member dtors at retail offsets, and the flags>0 delete tail.
#pragma push
#pragma optimize_for_size on
extern "C" void* __dt__13CMCGetItemBoxFv(CMCGetItemBox* this_, int flags) {
    if (this_ != NULL) {
        __dt__12CItemBoxInfoFv((CItemBoxInfo*)this_->itemBox, -1);
        __dt__7CSysWinFv(&this_->sysWin_B8, -1);
        __dt__6CCur18Fv(&this_->subObj_A0, -1);
        __dt__6CCur16Fv(&this_->subObj_88, -1);
        __dt__6CCur09Fv(&this_->subObj_70, -1);
        __dt__6CCur07Fv(&this_->subObj_58, -1);
        __dt__17UnkClass_8045F564Fv(&this_->memRegion2, -1);
        __dt__17UnkClass_8045F564Fv(&this_->memRegion1, -1);
        if (flags > 0) {
            __dl__FPv(this_);
        }
    }
    return this_;
}
#pragma pop

// Load the four item-box resource files into the file-handle members, init
// the item-box info (0xF4) and dispatch the sys-win initialiser (vtable+0x88).
void func_80297928(CMCGetItemBox* self) {
    self->fileHandle1 = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(),
                                              &lbl_eu_8050FF8C[0xae], (IWorkEvent*)self, 0, 0);
    self->fileHandle2 = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(),
                                              &lbl_eu_8050FF8C[0xc7], (IWorkEvent*)self, 0, 0);
    self->fileHandle3 = CDeviceFile::readCommonArchiveFile((mtl::ALLOC_HANDLE)func_800A9D90(),
                                                          &lbl_eu_8050FF8C[0xe0], (IWorkEvent*)self, 0, 0);
    self->fileHandle4 = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(),
                                              &lbl_eu_8050FF8C[0xfd], (IWorkEvent*)self, 0, 0);
    func_801D4054((CItemBoxInfo*)self->itemBox);
    ((CMCGetItemBoxSysWinInit*)&self->sysWin_B8)->init();
}

// Per-frame update: dispatch the layout-animation state machine (states 1/2/4/5),
// then refresh the cursor widgets, the item-box info and the sys-win window.
void func_802979E4(CMCGetItemBox* self) {
    if (self->field_4C == 0) return;
    if (self->field_4D == 0) return;
    switch (self->field_4D) {
        case 1: func_802988BC(self); break;
        case 2: func_80298938(self); break;
        case 4: func_802989A4(self); break;
        case 5: func_80298A20(self);
    }
    self->layout40->Animate(0);
    func_801D202C(&self->subObj_58);
    func_801D202C(&self->subObj_70);
    func_801D202C(&self->subObj_88);
    func_801D202C(&self->subObj_A0);
    func_801D40C4((CItemBoxInfo*)self->itemBox);
    func_8022B748(&self->sysWin_B8);
}

// Draw the item box: item-box info, layout, the (limit-dependent) cursor
// widgets and the sys-win window.
void func_80297AAC(CMCGetItemBox* self, nw4r::lyt::DrawInfo* di) {
    if (self->field_4C == 0) return;
    if (self->field_4D == 0) return;
    func_801D4154((CItemBoxInfo*)self->itemBox);
    func_80137038(self->layout40, di, 0, 1);
    u8 active = 1;
    if (self->sub_314.limit != 0) active = self->sub_314.limit;
    if (active > 1) func_801D20B0(&self->subObj_70, di);
    func_801D20B0(&self->subObj_58, di);
    func_801D20B0(&self->subObj_88, di);
    func_801D20B0(&self->subObj_A0, di);
    func_8022B7C8(&self->sysWin_B8, di);
}

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
// Retail layout: first return-0 inline after the state check (bne over it),
// then the ready!=0 path returns field_54 inline with the final return-0 at the tail.
u8 func_80297CC0(CMCGetItemBox* self) {
    if (getItemBoxState((CItemBoxInfo*)self->itemBox) == 0) return 0;
    if (CSysWin_isReady(&self->sysWin_B8) != 0) return self->field_54;
    return 0;
}

u8 CMCGetItemBox::func_80297D1C() { return mField55; }

u8 CMCGetItemBox::func_80297D24() { return mField303; }

// Open the item box from an external caller: guard on the current state, reset
// the item-box info and refresh both the layout text and the cursor widgets.
extern "C" void func_80297D2C(CMCGetItemBox* self, u16 arg, void* unk, u8 byte) {
    if (self->field_4D != 0) return;
    self->field_4D = 1;
    self->mField55 = 0;
    self->field_300 = 0;
    self->field_301 = 0xFF;
    func_801D421C((CItemBoxInfo*)self->itemBox);
    func_80299530(self, arg, unk, byte);
    func_801D4260((CItemBoxInfo*)self->itemBox, arg);
    u8 idx = (u8)(self->field_301 * 10 + self->field_300);
    CMCItemBoxSub* sub = &self->sub_314;
    CMCItemBoxEntry* entry = func_80296DB0(sub, idx);
    func_801D47D4((CItemBoxInfo*)self->itemBox, (u16)func_80296D54(sub, idx), (u32)entry, 1);
    func_801D4AE0((CItemBoxInfo*)self->itemBox, 1, func_80296E98(sub, idx));
}

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

// Move the cursor / page selection. Cursor mode walks the row backwards for a
// non-empty slot; page mode decrements the sort column and refreshes the
// widgets. Both paths end with the UI click sound.
extern "C" void func_80297E90(CMCGetItemBox* self) {
    if (CSysWin_getUnk34(&self->sysWin_B8) != 0) return;
    if (self->mField303 != 0) {
        u16* arr = self->arr_1A4.table;
        s8 idx = (s8)(self->field_305 - 1);
        while (idx != (s8)self->field_305) {
            if (idx < 0) idx = 3;
            if (ArrayGet12(arr,
                           (u8)((s8)self->field_304 * 4 + idx)) != 0) {
                self->field_305 = (u8)idx;
                break;
            }
            idx--;
        }
        u8 tmp[12];
        func_801CB9D8((u32*)tmp, arr,
                      (u8)((s8)self->field_304 * 4 + self->field_305));
        (*(void(**)(void*, void*))((void**)&self->subObj_A0)[4])(&self->subObj_A0, (void*)tmp);
    } else {
        u8 v = (u8)(self->field_301 - 1);
        self->field_301 = v;
        if ((s8)v < -1) self->field_301 = 2;
        func_802999B0(self);
        func_802998C8(self);
    }
    func_80138078(0x1);
}

// Advance the cursor to the next non-empty slot.
// Cursor branch is the fall-through (retail beq jumps to the sort branch).
void func_80297FB4(CMCGetItemBox* self) {
    if (CSysWin_getUnk34(&self->sysWin_B8) != 0) return;
    if (self->mField303 != 0) {
        u16* arr = self->arr_1A4.table;
        s8 idx = (s8)(self->field_305 + 1);
        while (idx != (s8)self->field_305) {
            if (idx >= 4) idx = 0;
            if (ArrayGet12(arr,
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
    } else {
        u8 v = self->field_301 + 1;
        self->field_301 = v;
        if ((s8)v >= 3) self->field_301 = 0xff;
        func_802999B0(self);
        func_802998C8(self);
    }
    func_80138078(0x1);
}

void func_802980DC(CMCGetItemBox* self) {
    if (CSysWin_getUnk34(&self->sysWin_B8) != 0) return;
    if (self->mField303 != 0) {
        u16* arr = self->arr_1A4.table;
        s8 idx = (s8)(self->field_304 - 1);
        while (idx != (s8)self->field_304) {
            if (idx < 0) idx = 2;
            if (ArrayGet12(arr,
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
    } else {
        // Retail reuses the loaded field_300 for the decrement (subi r3-based).
        u8 s = self->field_300;
        if ((s8)s == 0) {
            self->field_300 = 9;
            self->field_301 = 0;
            func_802983E4(self);
            func_802999B0(self);
        } else {
            u8 n = (u8)(s - 1);
            self->field_300 = n;
            if ((s8)n < 0) self->field_300 = 9;
            func_802999B0(self);
            func_802998C8(self);
        }
    }
    func_80138078(0x1);
}

void func_80298228(CMCGetItemBox* self) {
    if (CSysWin_getUnk34(&self->sysWin_B8) != 0) return;
    if (self->mField303 != 0) {
        u16* arr = self->arr_1A4.table;
        // idx walks the rows; the loop condition reloads field_304 every pass.
        s8 idx = (s8)(self->field_304 + 1);
        while (idx != (s8)self->field_304) {
            if (idx >= 3) idx = 0;
            if (ArrayGet12(arr,
                           (u8)((s8)idx * 4 + self->field_305)) != 0) {
                self->field_304 = (u8)idx;
                break;
            }
            idx++;
        }
        u8 tmp[12];
        func_801CB9D8((u32*)tmp, arr,
                      (u8)((s8)self->field_304 * 4 + self->field_305));
        (*(void(**)(void*, void*))((void**)&self->subObj_A0)[4])(&self->subObj_A0, (void*)tmp);
    } else {
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
    }
    func_80138078(0x1);
}

// Increment the sub counter, refresh helper widgets, and play a sound when the
// item-box limit is anything other than 1.
extern "C" __declspec(noinline) void func_80298378(CMCGetItemBox* self) {
    CMCItemBoxSub* x = &self->sub_314;
    func_80296D00(x);
    func_8029967C(self);
    func_802998C8(self);
    // Retail: li r0,1 default, conditionally overwritten with the limit (select form).
    u8 lim = 1;
    if (x->limit != 0) lim = x->limit;
    if (lim != 1) func_80138078(0xa);
}

// Decrement the sub counter, refresh helper widgets, and play a sound when the
// item-box limit is anything other than 1.
extern "C" __declspec(noinline) void func_802983E4(CMCGetItemBox* self) {
    CMCItemBoxSub* x = &self->sub_314;
    func_80296D2C(x);
    func_8029967C(self);
    func_802998C8(self);
    // Retail: li r0,1 default, conditionally overwritten with the limit (select form).
    u8 lim = 1;
    if (x->limit != 0) lim = x->limit;
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
// item-box entry's place relative to its full range: 2 when the cursor index
// is inside the entry count, 1 otherwise.
u32 func_80298540(CMCGetItemBox* self) {
    if (self->field_4D == 0) return 0;
    if ((s8)self->field_301 == -1) {
        u32 v = (u16)func_80157CD0(self->sub_314.pad_102);
        u32 count = self->sub_314.count;
        return 2 - (v < count);
    }
    return 0;
}

// Visit every item-box entry and hand it to the C-linkage cleanup helper.
#pragma optimize_for_size on  // -O4,s keeps the retail stmw frame + head-jump loop
void func_802985B4(CMCGetItemBox* self) {
    CMCItemBoxSub* x = &self->sub_314;
    for (u32 i = 0; (u32)(u16)i < x->count; i++) {
        func_801599D4(func_80296DB0(x, (u16)i), 0);
    }
}
#pragma optimize_for_size off

// Look up the entry at `index` in the offset table and return its pointer.
// noinline: retail keeps the loop's bl to this symbol.
extern "C" __declspec(noinline) CMCItemBoxEntry* func_80296DB0(CMCItemBoxSub* x, u32 index) {
    CMCItemBoxEntry* base = x->listBase;
    if (base == 0) return 0;
    u32 idx = (u16)(index + (s8)x->counter * 30);
    if (idx < x->count) {
        s16 off = x->table[(u16)idx];
        CMCItemBoxEntry* p = base + off;
        if (p != 0) return p;
    }
    return 0;
}

extern "C" void func_8029860C(void* self) { ((void(*)(void*))func_801D216C)((char*)self + 0x88); }

// Advance the cursor / tab selection in the item box.
void func_80298614(CMCGetItemBox* self) {
    if (self->mField303 != 0) {
        if (CSysWin_getUnk34(&self->sysWin_B8) != 0) {
            if (CSysWin_isActive(&self->sysWin_B8) != 0) {
                func_8022B8E4(&self->sysWin_B8);
                func_801D216C(&self->subObj_A0, 1);
            }
        } else {
            u16* arr = self->arr_1A4.table;
            u8 combined = (u8)((s8)self->field_304 * 4 + self->field_305);
            u32 r = ArrayGet12(arr, combined);
            if ((u16)r != 0) {
                u8 v;
                if (combined < 0xc) {
                    v = self->arr_1A4.flags[combined];
                } else {
                    v = 0;
                }
                void* res;
                if (v == 3) {
                    res = func_801D3C74(arr, combined);
                } else {
                    res = func_80136190(&lbl_eu_8050FF8C[0x119],
                                        &lbl_eu_8050FF8C[0x123], (u16)r);
                }
                func_8022B90C(&self->sysWin_B8, 0);
                func_8022B9B4(&self->sysWin_B8, res, 0);
                func_8022BFC8(&self->sysWin_B8, 1);
                func_8022B8B8(&self->sysWin_B8);
                func_801D216C(&self->subObj_A0, 0);
            }
        }
    } else {
        if (CSysWin_getUnk34(&self->sysWin_B8) != 0) return;
        // Find the first non-empty slot in the 12-entry table; record its
        // index (or 8 when the 8th entry is also non-empty).
        u16* arr = self->arr_1A4.table;
        int found = 0;
        for (u8 i = 0; i < 0xc; i++) {
            if (arr[i] != 0) {
                self->arr_1A4.field_D8 = i;
                if (arr[8] != 0) self->arr_1A4.field_D8 = 8;
                found = 1;
                break;
            }
        }
        if (found != 0) {
            u8 v = self->arr_1A4.field_D8;
            f32 f = lbl_eu_80668BEC * (f32)v;
            s32 r = (s32)f;
            self->mField303 = 1;
            u8 row = (u8)r;
            self->field_304 = row;
            self->field_305 = (u8)(v - (s8)row * 4);
            func_801D216C(&self->subObj_58, 0);
            func_801D216C(&self->subObj_A0, 1);
            u8 tmp[12];
            func_801CB9D8((u32*)tmp, arr, v);
            (*(void(**)(void*, void*))((void**)&self->subObj_A0)[4])(&self->subObj_A0, (void*)tmp);
            func_80138078(2);
        } else {
            func_80138078(5);
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

// Open the item box: advance the second layout animation and, once it has
// finished, initialise the state/widgets and refresh the cursor.
__declspec(noinline) void func_80298938(CMCGetItemBox* self) {
    if (func_80137444((nw4r::lyt::AnimTransform*)self->animTrans2, lbl_eu_80668BF0) != 0) {
        self->field_4D = 3;
        self->mField55 = 1;
        func_801D216C(&self->subObj_58, 1);
        func_801D216C(&self->subObj_70, 1);
        func_802999B0(self);
    }
}

// Advance the first layout animation; when it has finished, enable the two
// anim transforms on the layout and move to state 2.
__declspec(noinline) void func_802988BC(CMCGetItemBox* self) {
    if (func_80137444((nw4r::lyt::AnimTransform*)self->animTrans1, lbl_eu_80668BF0) != 0) {
        self->layout40->SetAnimationEnable((nw4r::lyt::AnimTransform*)self->animTrans1, false);
        self->layout40->SetAnimationEnable((nw4r::lyt::AnimTransform*)self->animTrans2, true);
        self->field_4D = 2;
    }
}

// Rewind the second layout animation via func_80137510; when finished, enable
// the two anim transforms on the layout and move to state 5.
__declspec(noinline) void func_802989A4(CMCGetItemBox* self) {
    if (func_80137510((nw4r::lyt::AnimTransform*)self->animTrans2, lbl_eu_80668BF0) != 0) {
        self->layout40->SetAnimationEnable((nw4r::lyt::AnimTransform*)self->animTrans2, false);
        self->layout40->SetAnimationEnable((nw4r::lyt::AnimTransform*)self->animTrans1, true);
        self->field_4D = 5;
    }
}

// Rewind the first layout animation; when it has finished, rearm the
// state bytes and reattach the cursor sub-object.
__declspec(noinline) void func_80298A20(CMCGetItemBox* self) {
    if (func_80137510(self->animTrans1, lbl_eu_80668BF0) != 0) {
        self->mField55 = 1;
        self->field_4D = 0;
        func_801D216C(&self->subObj_58, 0);
    }
}

// Guard: the item box only accepts input once the layout, the second arc
// accessor and its three state words are all present.
void func_80298A78(CMCGetItemBox* self) {
    if (self->layout40 == 0) return;
    if (self->arcAcc2 == 0) return;
    if (lbl_eu_80664A18 == 0) return;
    if (lbl_eu_80664A1C == 0) return;
    if (lbl_eu_80664A20 != 0) {
        self->field_54 = 1;
        self->field_4C = 1;
    } else {
        return;
    }
}

// nw4r ArcResourceAccessor virtual GetResource at vtable[3] (offset 0x0C).
// MWCC RTTI occupies vtable slots 0,4, so the second declared virtual lands at
// offset 0x0C: one dummy slot + GetResource3.
struct AccessorGetRes3 {
    virtual void _v00();
    virtual void* GetResource3(u32 tag, const char* name, int r6);
};

// Retail 0x80298AC8: resolve the icon resource for a selected item-box entry
// and stamp it into a numbered layout pane. Mirrors func_80298FB4 but uses the
// icon database (func_801361E8) and the 0x144-0x149 icon-name chain for both
// the gem (type 3) and item (type 9) paths, plus a %d pane-name format at
// &lbl[0x14e].
extern "C" void func_80298AC8(CMCGetItemBox* self, u32 idx, CMCItemBoxEntry* entry, u8 n) {
    CMCItemBoxEntry* e = entry != 0 ? entry : 0;
    void* h = 0;
    if (e != 0) {
        u32 type = (e->field_00 >> 16) & 0xF;
        if (type == 3) {
            h = 0;
            CMCItemImplShim* inst = (CMCItemImplShim*)CItem_initItemImplInstances(e);
            u32 k = inst->getIcon(e);
            u8 r = (u8)func_801361E8((u32)lbl_eu_806640D8, &lbl_eu_8050FF8C[0x128], (u16)k);
            switch (r) {
                case 4: h = func_80138F78(0x144); break;
                case 5: h = func_80138F78(0x145); break;
                case 6: h = func_80138F78(0x146); break;
                case 7: h = func_80138F78(0x147); break;
                case 8: h = func_80138F78(0x148); break;
                case 9: h = func_80138F78(0x149); break;
            }
            if (h != 0)
                h = ((AccessorGetRes3*)self->arcAcc2)->GetResource3(0x74696d67u, (const char*)h, 0);
            else
                h = ((AccessorGetRes3*)self->arcAcc1)->GetResource3(0x74696d67u, &lbl_eu_8050FF8C[0x131], 0);
        } else if (type == 9) {
            CMCItemImplShim* inst = (CMCItemImplShim*)CItem_initItemImplInstances(e);
            u32 cnt = inst->getCount(e);
            if ((u16)cnt == 0) {
                // Empty slot: a bdat-managed entry uses the 0x155 chain,
                // otherwise the gem-icon chain keyed on byte 7 >> 2.
                if (func_801C6E90(e) != 0) {
                    h = ((AccessorGetRes3*)self->arcAcc2)->GetResource3(0x74696d67u, (const char*)func_80138F78(0x155), 0);
                } else {
                    char* s = 0;
                    switch ((e->bytes[3] >> 2) & 0x3F) {
                        case 4: s = func_80138F78(0x144); break;
                        case 5: s = func_80138F78(0x145); break;
                        case 6: s = func_80138F78(0x146); break;
                        case 7: s = func_80138F78(0x147); break;
                        case 8: s = func_80138F78(0x148); break;
                        case 9: s = func_80138F78(0x149); break;
                    }
                    if (s != 0)
                        h = ((AccessorGetRes3*)self->arcAcc2)->GetResource3(0x74696d67u, s, 0);
                    else
                        h = ((AccessorGetRes3*)self->arcAcc1)->GetResource3(0x74696d67u, &lbl_eu_8050FF8C[0x131], 0);
                }
            } else {
                char* s = 0;
                switch ((e->bytes[3] >> 2) & 0x3F) {
                    case 4: s = func_80138F78(0x144); break;
                    case 5: s = func_80138F78(0x145); break;
                    case 6: s = func_80138F78(0x146); break;
                    case 7: s = func_80138F78(0x147); break;
                    case 8: s = func_80138F78(0x148); break;
                    case 9: s = func_80138F78(0x149); break;
                }
                if (s != 0)
                    h = ((AccessorGetRes3*)self->arcAcc2)->GetResource3(0x74696d67u, s, 0);
                else
                    h = ((AccessorGetRes3*)self->arcAcc1)->GetResource3(0x74696d67u, &lbl_eu_8050FF8C[0x131], 0);
            }
        }
    }
    if (h == 0) {
        if (idx != 0) {
            char* name = func_80138F78((u16)func_80136254((void*)lbl_eu_806640EC, &lbl_eu_8050FF8C[0x144], idx));
            h = ((AccessorGetRes3*)self->arcAcc2)->GetResource3(0x74696d67u, name, 0);
            if (h == 0)
                h = ((AccessorGetRes3*)self->arcAcc1)->GetResource3(0x74696d67u, &lbl_eu_8050FF8C[0x131], 0);
        } else {
            h = ((AccessorGetRes3*)self->arcAcc1)->GetResource3(0x74696d67u, &lbl_eu_8050FF8C[0x131], 0);
        }
    }
    if (h != 0) {
        char buf[0x20];
        sprintf(buf, &lbl_eu_8050FF8C[0x14e], (int)(n + 1));
        func_80137E7C((void*)self->layout40, buf, (void*)h);
    }
}

// Retail 0x80298FB4: item icon-name resolution for the grid cells.
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
void func_80299490(CMCGetItemBox* self, int r4, u32 r5) {
    char buf2[0x20];
    char buf1[0x20];
    sprintf(buf1, &lbl_eu_8050FF8C[0x16e], r5 + 1);
    if ((s8)r4 == 0) {
        sprintf(buf2, &lbl_eu_8050FF8C[0x17a]);
    } else {
        sprintf(buf2, &lbl_eu_8050FF8C[0],
                func_80136190(&lbl_eu_8050FF8C[0x3], &lbl_eu_8050FF8C[0xc],
                              0x1e - ((s8)r4 - 1)));
    }
    func_80136A1C(self->layout40, buf1, buf2, 0);
}

// Refresh layout text after index/sort change. Extra params are passed through
// by func_80297D2C but ignored here (retail never reads r5/r6).
void func_80299530(CMCGetItemBox* self, u16 arg, void* unk, u8 byte) {
    CMCItemBoxSub* sub = &self->sub_314;
    // Retail calls func_80296BF0 with only r3 set (r4/r5/r6 carry garbage),
    // so invoke it through a 1-arg prototype to avoid li r4/r5/r6 setup.
    ((void (*)(CMCItemBoxSub*))func_80296BF0)(sub);
    u8 count = sub->limit;
    if (count == 0) count = 1;
    func_80136910((nw4r::lyt::Layout*)self->layout40, &lbl_eu_8050FF8C[0x17b], count);
    // Retail reloads the layout/root pane before every FindPaneByName call
    // (no root local), keeping pressure at 3 callee-saved registers.
    if (arg == 9) {
        func_80124270((*(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10))->FindPaneByName(&lbl_eu_8050FF8C[0x184], true), 0);
        func_80124270((*(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10))->FindPaneByName(&lbl_eu_8050FF8C[0x18f], true), 1);
    } else {
        func_80124270((*(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10))->FindPaneByName(&lbl_eu_8050FF8C[0x184], true), 1);
        func_80124270((*(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10))->FindPaneByName(&lbl_eu_8050FF8C[0x18f], true), 0);
    }
    func_8029967C(self);
    func_802998C8(self);
    if (getItemBoxState((CItemBoxInfo*)self->itemBox) != 0) {
        func_801D4260((void*)self->itemBox, arg);
    }
}

extern "C" __declspec(noinline) void func_8029967C(CMCGetItemBox* self) {
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

// Refresh the item-name texts in the layout panes and, when the item box is
// open, re-sync the selected entry's name/icon widgets.
extern "C" __declspec(noinline) void func_802998C8(CMCGetItemBox* self) {
    CMCItemBoxSub* sub = &self->sub_314;
    s8 idx = (s8)(self->field_301 * 10 + self->field_300);
    func_80136B4C(self->layout40, &lbl_eu_8050FF8C[0x1c7], func_80296E98(sub, (u16)idx), 0);
    func_80136B4C(self->layout40, &lbl_eu_8050FF8C[0x1d0], func_80296FC0(sub, (u16)idx), (u32)self->objAt50);
    if (getItemBoxState((CItemBoxInfo*)self->itemBox) != 0) {
        CMCItemBoxEntry* entry = func_80296DB0(sub, (u16)idx);
        func_801D47D4((CItemBoxInfo*)self->itemBox, (u16)func_80296D54(sub, (u16)idx), (u32)entry, 1);
        func_801D4AE0((CItemBoxInfo*)self->itemBox, 1, func_80296E98(sub, (u16)idx));
    }
}

// Refresh the cursor widgets / page label.
extern "C" void func_802999B0(CMCGetItemBox* self) {
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

// Retail 0x8029C200: async file-load callback. Four files can load
// asynchronously: the main item-box layout archive (fileHandle1), a second
// resource archive (fileHandle2), and two bdat data files (fileHandle3/4).
// Branch 1 builds the whole widget: region-guarded buffer, layout + 2 anim
// transforms, font bind, text stamping, cursor sub-objects and activation;
// the other branches only feed bdat tables / the second accessor.
bool CMCGetItemBox::OnFileEvent(CEventFile* pEventFile) {
    if (this->fileHandle1 == pEventFile->mFileHandle) {
        // === main item-box layout file loaded ===
        this->memRegion1.createRegion(mtl::MemManager::getHandleMEM2(), 0x20000,
                                      &lbl_eu_8050FF8C[0x1f8], 0);
        Class_8045F858 regionGuard1(&this->memRegion1);

        CFileHandle* h1 = this->fileHandle1;
        void* fileData = h1->mData;
        h1->mData = 0;
        mtl::MemManager::func_80434A4C(false);

        void* tagMem = mtl::MemManager::allocate(0x858, (mtl::ALLOC_HANDLE)getAllocHandle__10CLibLayoutFv());
        if (tagMem != 0) tagMem = __ct__CTagProcessor(tagMem);
        this->objAt50 = (u8*)tagMem;

        this->arcAcc1 = CLibLayout::createArcResourceAccessor();
        this->arcAcc1->Attach(fileData, &lbl_eu_8050FF8C[0x206]);

        func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
            &this->layout40, this->arcAcc1, &lbl_eu_8050FF8C[0x20a]);
        func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            this->layout40, &this->animTrans1, this->arcAcc1, &lbl_eu_8050FF8C[0x221]);
        func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            this->layout40, &this->animTrans2, this->arcAcc1, &lbl_eu_8050FF8C[0x23b]);

        // Bind the loaded font's pane into the layout root.
        nw4r::lyt::Pane* rootPane = this->layout40->GetRootPane();
        void* font = CDeviceFont::func_80452C10(1, this->layout40);
        void** vtblFont = *(void***)font;
        void* fontData = ((void*(*)())vtblFont[9])();
        func_8013676C(rootPane, (u32)fontData);

        u32 w = (u32)func_801355A0();
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(this->layout40, &lbl_eu_8050FF8C[0x1b0], w);
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(this->layout40, &lbl_eu_8050FF8C[0x17b], w);

        // Stamp every item-slot text pane with the loaded character set.
        u32 sh = (u32)func_801355BC();
        for (u8 i = 1; i <= 0x1e; i++) {
            char buf[0x20];
            sprintf(buf, &lbl_eu_8050FF8C[0x16e], i);
            func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(this->layout40, buf, sh);
        }
        func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(this->layout40, &lbl_eu_8050FF8C[0x25a], sh);

        this->layout40->SetAnimationEnable(this->animTrans2, false);
        this->layout40->SetAnimationEnable(this->animTrans1, true);
        this->layout40->Animate(0);

        // Hand the tag processor to the tag pane (+0xF8).
        nw4r::lyt::Pane* tagPane = rootPane->FindPaneByName(&lbl_eu_8050FF8C[0x1d0], true);
        if (tagPane != 0) *(u32*)((u8*)tagPane + 0xF8) = (u32)this->objAt50;

        func_80136B4C(this->layout40, &lbl_eu_8050FF8C[0x274],
                      (char*)func_80136190(&lbl_eu_8050FF8C[0x269], &lbl_eu_8050FF8C[0xc], 6), 0);

        // Remember the cursor pane's position.
        nw4r::lyt::Pane* curPane = rootPane->FindPaneByName(&lbl_eu_8050FF8C[0x281], true);
        copyVEC3((void*)&this->field_308, (float*)((u8*)curPane + 0x2C));

        // Cursor sub-objects: construct on the stack with the arc accessor,
        // copy into the embedded storage, destroy the temp, then run the
        // embedded cursor's slot-2 virtual (update).
        u8 cur07Buf[0x18];
        __ct__CCur07(cur07Buf, this->arcAcc1);
        func_8018B0FC(&this->subObj_58, cur07Buf);
        __dt__6CCur07Fv((CMCItemBoxSubObj*)cur07Buf, -1);
        (*(void(**)(void*))(*(void***)&this->subObj_58)[2])(&this->subObj_58);

        u8 cur09Buf[0x18];
        __ct__CCur09(cur09Buf, this->arcAcc1);
        func_8018B0FC(&this->subObj_70, cur09Buf);
        __dt__6CCur09Fv((CMCItemBoxSubObj*)cur09Buf, -1);
        (*(void(**)(void*))(*(void***)&this->subObj_70)[2])(&this->subObj_70);

        // Two anchor vectors on cursor 09: the second pair swaps the x base.
        nw4r::math::VEC3 s1;
        nw4r::math::VEC3 v1;
        nw4r::math::VEC3 s2;
        nw4r::math::VEC3 v2;
        code80135FDC_setVec3((float*)&s1, lbl_eu_80668BF8, lbl_eu_80668BFC, lbl_eu_80668BD4);
        v1 = s1;
        code80135FDC_setVec3((float*)&s2, lbl_eu_80668C00, lbl_eu_80668BFC, lbl_eu_80668BD4);
        v2 = s2;
        func_801D24E8(&this->subObj_70, &v2, &v1);

        u8 cur16Buf[0x18];
        __ct__CCur16(cur16Buf, this->arcAcc1);
        func_8018B0FC(&this->subObj_88, cur16Buf);
        __dt__6CCur16Fv((CMCItemBoxSubObj*)cur16Buf, -1);
        (*(void(**)(void*))(*(void***)&this->subObj_88)[2])(&this->subObj_88);

        u8 cur18Buf[0x18];
        __ct__CCur18(cur18Buf, func_801355F4());
        func_8018B0FC(&this->subObj_A0, cur18Buf);
        __dt__6CCur18Fv((CMCItemBoxSubObj*)cur18Buf, -1);
        (*(void(**)(void*))(*(void***)&this->subObj_A0)[2])(&this->subObj_A0);

        func_80298A78(this);
        this->fileHandle1 = 0;
        this->memRegion1.func_8045F810();
        return true;
    } else if (this->fileHandle2 == pEventFile->mFileHandle) {
        // === second resource archive loaded ===
        this->memRegion2.createRegion(mtl::MemManager::getHandleMEM2(), 0x100,
                                      &lbl_eu_8050FF8C[0x28e], 0);
        Class_8045F858 regionGuard2(&this->memRegion2);

        CFileHandle* h2 = this->fileHandle2;
        void* fileData = h2->mData;
        h2->mData = 0;
        mtl::MemManager::func_80434A4C(false);

        this->arcAcc2 = CLibLayout::createArcResourceAccessor();
        this->arcAcc2->Attach(fileData, &lbl_eu_8050FF8C[0x206]);
        func_80298A78(this);
        this->fileHandle2 = 0;
        this->memRegion2.func_8045F810();
        return true;
    } else if (this->fileHandle3 == pEventFile->mFileHandle) {
        // === bdat table 2 (crystal names) loaded ===
        CFileHandle* h3 = this->fileHandle3;
        void* fileData = h3->mData;
        h3->mData = 0;
        func_8003AA34();
        if (getFP__FPCc(&lbl_eu_8050FF8C[0x29f]) == 0) {
            func_8003AA78__5CBdatFUlPv(2, fileData);
        }
        func_8003AA34();
        lbl_eu_80664A18 = (void*)getFP__FPCc(&lbl_eu_8050FF8C[0x29f]);
        func_8003AA34();
        lbl_eu_80664A1C = (void*)getFP__FPCc(&lbl_eu_8050FF8C[0x2ae]);
        func_80298A78(this);
        this->fileHandle3 = 0;
        return true;
    } else if (this->fileHandle4 == pEventFile->mFileHandle) {
        // === bdat table 5 (item names) loaded ===
        CFileHandle* h4 = this->fileHandle4;
        void* fileData = h4->mData;
        h4->mData = 0;
        this->memManagerPtr = (u8*)fileData;
        func_8003AA34();
        if (getFP__FPCc(&lbl_eu_8050FF8C[0x119]) == 0) {
            func_8003AA78__5CBdatFUlPv(5, this->memManagerPtr);
        }
        func_8003AA34();
        lbl_eu_80664A20 = (void*)getFP__FPCc(&lbl_eu_8050FF8C[0x119]);
        func_80298A78(this);
        this->fileHandle4 = 0;
        return true;
    }

    return false;
}