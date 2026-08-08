// Auto-scaffolded catalog TU for kyoshin/makecrystal/CMCCrystalBox
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/makecrystal/CMCCrystalBox.hpp"

#include <nw4r/lyt.h>

extern float lbl_eu_80668470;  // anim sentinel constant used by completion checks
extern float lbl_eu_8066845C;
extern float lbl_eu_80668474;
extern float lbl_eu_80668478;
extern "C" void code80135FDC_setVec3(float*, float, float, float);

// Retail symbols: func_80137444 keeps the mangled C++ name, but func_80137510
// is the unmangled C name (per retail relocs).
u32 func_80137444(nw4r::lyt::AnimTransform*, float);
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);

void func_80138078(u32);
extern "C" void func_801D216C(void*, u8);  // retail symbol is unmangled

// External leaf helpers referenced from matched functions in this TU (C retail names).
extern "C" u32 func_801D32DC(void*);
extern "C" u8 func_8021A8EC(void*);
extern "C" u32 CScrollBar_isVisible(void*);
extern "C" u32 CSysWin_isReady(void*);
extern "C" void func_8021A984(void*);
extern "C" void func_8021A918(void*);
extern "C" void func_8021A960(void*);
extern "C" void func_801F369C(void*);

// makecrystal data strings + crystal-state leaf helpers (compiled in code_80213488).
extern "C" char lbl_eu_8050888C[];
extern "C" unsigned long lbl_eu_80508870[];
extern "C" void func_80213570(void*, u8);
void func_80136910(nw4r::lyt::Layout*, char*, u8);

extern "C" char* func_802138B8(CMCCrystalData*, int);

// Crystal-info / crystal sub-object helpers referenced by the targets below.
extern "C" void func_8021A93C(void*);
extern "C" void func_8021A8F4(void*);
extern "C" void func_8021A840(void*, void*);
extern "C" void func_8021A9A8(void*, unsigned short, unsigned char);
extern "C" int func_801D3320(void*);
extern "C" void func_801D31F8(void*, void*);
extern "C" void func_801D20B0(void*, void*);
extern "C" void func_801D350C(void*);
extern "C" void func_801D3518(void*, void*);
extern "C" void func_801D353C(void*, unsigned char);
extern "C" void func_801F35B0(void*, void*);
extern "C" void func_8022B7C8(void*, void*);
extern "C" void func_801F3670(void*, void*);
extern "C" void func_801F36BC(void*, unsigned long, unsigned long);
extern "C" void func_801F3850(void*, unsigned long);
extern "C" void func_801F367C(void*);
extern "C" void* CItem_initItemInstances();
extern "C" unsigned long func_8015780C(unsigned long);
extern "C" char* func_80136190(char*, char*, unsigned long);
extern "C" void func_80136B4C(nw4r::lyt::Layout*, char*, char*, unsigned long);
extern "C" void* func_80157C4C(unsigned char, short);
extern "C" int func_80213710(void*, unsigned char);
extern "C" unsigned char func_8021384C(void*, unsigned char);
extern "C" unsigned short func_802137DC(void*, unsigned char);

// Member-subobject destructors (placed subobjects, called with deleting-flag -1).
extern "C" void __dt__7CSysWinFv(void*, int);
extern "C" void __dt__10CScrollBarFv(void*, int);
extern "C" void __dt__14CMCCrystalInfoFv(void*, int);
extern "C" void __dt__9CSortMenuFv(void*, int);
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dt__6CCur16Fv(void*, int);
extern "C" void __dt__6CCur11Fv(void*, int);
extern "C" void __dt__6CCur09Fv(void*, int);
extern "C" void __dt__6CCur07Fv(void*, int);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);

// Member-subobject constructors (placed subobjects, called with an int arg).
extern "C" void __ct__17UnkClass_8045F564Fv(void*);
extern "C" void __ct__CCur07(void*, int);
extern "C" void __ct__CCur09(void*, int);
extern "C" void __ct__CCur18(void*, int);
extern "C" void __ct__CCur11(void*, int);
extern "C" void __ct__CCur16(void*, int);
extern "C" void __ct__CSortMenu(void*);
extern "C" void __ct__CMCCrystalInfo(void*);
extern "C" void __ct__CScrollBar(void*, int);
extern "C" void __ct__CSysWin(void*, int);

// External leaf helpers referenced by the targets below (C retail names).
extern "C" void func_8003AA8C__5CBdatFUl(u32);
extern "C" void func_801390E0__FPP11CFileHandle(void*);
extern "C" void func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(void*);
extern "C" void func_8045F778__17UnkClass_8045F564Fv(void*);
extern "C" void func_80139198(u32);
extern "C" void func_801D3258(void*);
extern "C" void func_8021A860(void*);
extern "C" void func_801F35DC(void*);
extern "C" void func_8022B7F4(void*);
extern "C" int  func_801D3328(void*);
extern "C" void func_801D3408(void*);
extern "C" void func_801D3330(void*);
extern "C" void func_801D3430(void*, void*);
extern "C" void func_801D3454(void*, void*);
extern "C" void func_80137924(void*, void*, void*, void*);
extern "C" void* CSysWin_getUnk34(void*);
extern "C" void  CSysWin_isActive(void*);
extern "C" void func_8022B8E4(void*);
extern "C" void func_8022B90C(void*, int);
extern "C" void func_8022B9B4(void*, void*, int);
extern "C" void func_8022BFC8(void*, int);
extern "C" void func_8022B8B8(void*);
extern "C" char* func_8013639C(void*, char*, int);
extern "C" void func_801D3160(void*);
extern "C" void func_8021A780(void*);
extern "C" void func_801D202C(void*);
extern "C" void func_801F3540(void*);
extern "C" void func_8022B748(void*);
extern "C" void func_80213488(void*);
extern "C" u32  func_801C6E90(void*);
extern "C" void* CItem_initItemInstances(void);
extern "C" u16 func_80139358(u32);

// Leaf subroutines of this unit called from the matched functions below. They
// are extern (no body in TU) so MWCC emits a real `bl`; retail symbols are
// unmangled C names, so declared `extern "C"`.
extern "C" void func_80218018(CMCCrystalBox* self);
extern "C" void func_8021488C(CMCCrystalBox* self);
extern "C" int func_80216A20(CMCCrystalBox* self);

extern "C" void func_80219094(CMCCrystalBox* self);
extern "C" void func_80215B78(CMCCrystalBox* self);
extern "C" void func_8021852C(CMCCrystalBox* self);
extern "C" void func_8021900C(CMCCrystalBox* self);
extern "C" void func_8021911C(CMCCrystalBox* self);
extern "C" void func_802191A4(CMCCrystalBox* self);
extern "C" void func_8021922C(CMCCrystalBox* self);
extern "C" void func_80219994(CMCCrystalBox* self, int);
extern "C" void func_80213788(void* self);
extern "C" void func_802137B4(void* self);
extern "C" void func_802180B4(CMCCrystalBox* self);
extern "C" void func_80218460(CMCCrystalBox* self);
extern "C" void func_80218B10(CMCCrystalBox* self);
extern "C" void func_8021899C(CMCCrystalBox* self);

// Retail 0x80219E70: refresh the crystal box display / name.
void func_80218018(CMCCrystalBox* self) {
    CMCCrystalData* d = &self->data;
    if (self->field_14A0 == 0) {
        func_80213570(d, self->field_14EC[(s8)self->field_14F2]);
    }
    func_80136910((nw4r::lyt::Layout*)self->subObjPtrs[5],
                  lbl_eu_8050888C + 0x12e, d->limit != 0 ? d->limit : 1);
    func_802180B4(self);
    func_80218460(self);
    func_8021899C(self);
    func_8021A8EC((u8*)self + 0x1ec);
}

// Retail 0x8021A2B8: set the current crystal's name label and mirrored info.
void func_80218460(CMCCrystalBox* self) {
    CMCCrystalData* d = &self->data;
    u8 idx = self->unk2CC + self->unk2CD * 10;
    char* name = func_802138B8(d, idx);
    func_80136B4C((nw4r::lyt::Layout*)self->subObjPtrs[5],
                  lbl_eu_8050888C + 0x18e, name, 0);
    if (func_8021A8EC((u8*)self + 0x1ec) != 0) {
        if ((s8)self->unk2CD < 3) {
            func_8021A9A8((u8*)self + 0x1ec,
                          func_802137DC(d, idx), func_8021384C(d, idx));
        } else {
            func_8021A9A8((u8*)self + 0x1ec, 0, 0);
        }
    }
}

// Retail 0x80215710: resolve a crystal item by adjusted index, get its name
// from a freshly-created item instance, and format it into the name buffer.
char* func_802138B8(CMCCrystalData* d, int v) {
    s16 cur = (s16)d->current;
    u16 adj = (u16)(v + cur * 30);          // row-scaled index
    if (adj >= d->count)
        return 0;
    void* obj = func_80157C4C(d->byte_1002, d->entries[adj].id);
    if (obj == 0 || *(void**)obj == 0)
        return 0;
    void* inst = CItem_initItemInstances();
    void** vt = *(void***)inst;
    char* (*nameFn)(void*, void*) = (char* (*)(void*, void*))vt[8];
    sprintf(d->name, lbl_eu_8050888C, nameFn(inst, obj));
    return d->name;
}

void func_80213964(int unused, void* a, void* b) {
    short b_lo = *(short*)b;
    unsigned char b_hi = ((unsigned char*)b)[2];
    short a_lo = *(short*)a;
    unsigned char a_hi = ((unsigned char*)a)[2];
    *(short*)a = b_lo;
    ((unsigned char*)a)[2] = b_hi;
    *(short*)b = a_lo;
    ((unsigned char*)b)[2] = a_hi;
}

void func_80213988(){}

// Retail 0x80215974: bubble-sort the crystal-state entries by their %+4
// key using func_80157C4C until no swap occurs in a pass.
void func_80213B1C(CMCCrystalData* d) {
    u16 o;
    for (o = 0; o < d->count - 1; o++) {
        int swapped = 0;
        u16 i;
        for (i = 0; i < (d->count - 1) - o; i++) {
            CMCCrystalDataEntry* a = &d->entries[i];
            CMCCrystalDataEntry* b = &d->entries[i + 1];
            u16 av = *(u16*)((char*)func_80157C4C(9, a->id) + 4);
            u16 bv = *(u16*)((char*)func_80157C4C(9, b->id) + 4);
            if (av > bv) {
                func_80213964(reinterpret_cast<int>(d), a, b);
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}

void __ct__CMCCrystalBox(){}

void initCrystalBoxParam_80213E04_impl(CMCCrystalBoxParam* self, s16 a, s16 b);

void copyCrystalBoxParam_80213E20(CMCCrystalBoxParam *dst, const CMCCrystalBoxParam *src) {
    unsigned short *d16 = (unsigned short*)dst;
    const unsigned short *s16 = (const unsigned short*)src;
    d16[0] = s16[0];
    d16[1] = s16[1];
    d16[2] = s16[2];
    unsigned char *d8 = (unsigned char*)dst;
    const unsigned char *s8 = (const unsigned char*)src;
    d8[6] = s8[6];
    d8[7] = s8[7];
}

u8* func_80213D74(u8* base) {
    int i;
    // Initialise the 32-slot param table at base+2, then re-init through the
    // temp-copy path (matching retail's two loops).
    for (i = 0; i < 0x20; i++) {
        ((CMCCrystalBoxParam*)(base + 2 + i * 8))->initCrystalBoxParam_80213E04(0, 0);
    }
    base[0] = 0;
    base[1] = 0;
    for (i = 0; i < 0x20; i++) {
        CMCCrystalBoxParam tmp;
        tmp.initCrystalBoxParam_80213E04(0, 0);
        copyCrystalBoxParam_80213E20(&((CMCCrystalBoxParam*)(base + 2))[i], &tmp);
    }
    return base;
}

void __dt__80213E4C(){}

void func_80213E8C(CMCCrystalBox* self) {
    u32 i;
    self->unk20 = 0;
    ((u8*)self)[0x29] = 0;
    ((u8*)self)[0x6A] = 0;
    for (i = 0; i < 8; i++) {
        self->subObjPtrs[i] = 0;
        ((u8*)self)[0x21 + i] = 0xFF;
    }
}

void __dt__80213ECC(){}

// Retail 0x80215D64: destroy all placed member subobjects in reverse order.
CMCCrystalBox::~CMCCrystalBox() {
    __dt__7CSysWinFv((u8*)this + 0x290, -1);
    __dt__10CScrollBarFv((u8*)this + 0x250, -1);
    __dt__14CMCCrystalInfoFv((u8*)this + 0x1ec, -1);
    __dt__9CSortMenuFv((u8*)this + 0xfc, -1);
    __dt__6CCur18Fv((u8*)this + 0xe4, -1);
    __dt__6CCur16Fv((u8*)this + 0xcc, -1);
    __dt__6CCur11Fv((u8*)this + 0xb4, -1);
    __dt__6CCur18Fv((u8*)this + 0x9c, -1);
    __dt__6CCur09Fv((u8*)this + 0x84, -1);
    __dt__6CCur07Fv((u8*)this + 0x6c, -1);
    __dt__17UnkClass_8045F564Fv((u8*)this + 0x14, -1);
    __dt__17UnkClass_8045F564Fv((u8*)this + 0x4, -1);
}

void func_80213FE4(){}

void func_802142C4(){}

void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

// Retail 0x80216260: draw the crystal box sub-views.
void func_80214408(CMCCrystalBox* self, nw4r::lyt::DrawInfo* di) {
    if (self->unk60 == 0) return;
    if (self->unk64 == 0) return;
    func_80137038((nw4r::lyt::Layout*)self->subObjPtrs[5], di, 0, 1);
    func_8021A840((u8*)self + 0x1ec, di);
    func_801D31F8((u8*)self + 0xfc, di);
    u8 lim = self->data.limit;
    u8 active = lim != 0 ? lim : 1;
    if (active > 1) {
        func_801D20B0((u8*)self + 0x84, di);   // CCur09
    }
    func_801D20B0((u8*)self + 0x6c, di);       // CCur07
    func_801D20B0((u8*)self + 0x9c, di);
    func_801D20B0((u8*)self + 0xb4, di);
    func_801D20B0((u8*)self + 0xcc, di);
    func_801D20B0((u8*)self + 0xe4, di);
    func_801F35B0((u8*)self + 0x250, di);      // scrollbar
    func_8022B7C8((u8*)self + 0x290, di);      // syswin
}

// Retail 0x8021634c: teardown of the whole crystal box. Closes the file
// handles for the first three resource sub-objects, frees the layout/arc
// resources, then destroys every placed member sub-object.
void func_802144F4(CMCCrystalBox* self) {
    func_8003AA8C__5CBdatFUl(2);
    func_801390E0__FPP11CFileHandle(&self->subObjPtrs[0]);
    func_801390E0__FPP11CFileHandle(&self->subObjPtrs[1]);
    func_801390E0__FPP11CFileHandle(&self->subObjPtrs[2]);

    if (self->unk60 != 0) {
        void* layout = self->subObjPtrs[5];
        self->unk60 = 0;
        if (layout != 0) {
            if (layout != 0) {
                (*(void(**)(void*, u32))(*(void***)layout + 2))(layout, 1);
            }
            self->subObjPtrs[5] = 0;
        }
        func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(self->subObjPtrs[3]);
        func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(self->subObjPtrs[4]);
        func_8045F778__17UnkClass_8045F564Fv((u8*)self + 0x4);
        func_8045F778__17UnkClass_8045F564Fv((u8*)self + 0x14);
        func_801D3258((u8*)self + 0xfc);
        func_8021A860((u8*)self + 0x1ec);
        (*(void(**)(void*))(*(void***)((u8*)self + 0x6c))[3])((u8*)self + 0x6c);
        (*(void(**)(void*))(*(void***)((u8*)self + 0x84))[3])((u8*)self + 0x84);
        (*(void(**)(void*))(*(void***)((u8*)self + 0x9c))[3])((u8*)self + 0x9c);
        (*(void(**)(void*))(*(void***)((u8*)self + 0xb4))[3])((u8*)self + 0xb4);
        (*(void(**)(void*))(*(void***)((u8*)self + 0xcc))[3])((u8*)self + 0xcc);
        (*(void(**)(void*))(*(void***)((u8*)self + 0xe4))[3])((u8*)self + 0xe4);
        func_801F35DC((u8*)self + 0x250);
        func_8022B7F4((u8*)self + 0x290);
    }
}

u8 func_80214634(CMCCrystalBox* self) {
    if (func_801D32DC((u8*)self + 0xfc) == 0) return 0;
    if (func_8021A8EC((u8*)self + 0x1ec) == 0) return 0;
    if (CScrollBar_isVisible((u8*)self + 0x250) == 0) return 0;
    if (CSysWin_isReady((u8*)self + 0x290) == 0) return 0;
    return self->unk68;
}

u8 getByte_69_802146C0(void* self) { return static_cast<CMCCrystalBox*>(self)->unk69; }

u8 func_802146C8(CMCCrystalBox* self) {
    if (self->unk2D1 != 0) return 1;
    if (self->unk1500 != 0) return 1;
    return self->unk2CE;
}

extern "C" void* CSysWin_getUnk34(void* self);
void syswinGetUnk34_802146F8(void* self) { CSysWin_getUnk34((char*)self + 0x290); }

// Retail 0x80216558: initialise the crystal box into state 1, mirror the
// current item, and point the scrollbar at the selected vec.
void func_80214700(CMCCrystalBox* self) {
    if (self->unk64 != 0) return;
    self->unk64 = 1;
    self->unk69 = 0;
    func_8021899C(self);
    func_8021A8F4((u8*)self + 0x1ec);
    CMCCrystalData* d = &self->data;
    u8 idx = self->unk2CC + self->unk2CD * 10;
    func_8021A9A8((u8*)self + 0x1ec,
                  func_802137DC(d, idx), func_8021384C(d, idx));
    func_80218B10(self);
    float vec[3];
    code80135FDC_setVec3(vec, lbl_eu_80668474, lbl_eu_80668478,
                         lbl_eu_8066845C);
    func_801F3670((u8*)self + 0x250, vec);
    func_801F36BC((u8*)self + 0x250, 8, 0);
    func_801F3850((u8*)self + 0x250, 0);
    func_801F367C((u8*)self + 0x250);
    func_8021900C(self);
}

void func_802147F4(CMCCrystalBox* self) {
    if (self->unk64 != 3) return;
    if (self->unk2CE != 0) return;
    self->unk64 = 4;
    self->unk69 = 0;
    func_801D216C((u8*)self + 0x6c, 0);
    func_801D216C((u8*)self + 0x84, 0);
    func_801D216C((u8*)self + 0xcc, 0);
    func_801D216C((u8*)self + 0xb4, 0);
    func_8021A918((u8*)self + 0x1ec);
    func_801F369C((u8*)self + 0x250);
    func_80219094(self);
}

// Retail 0x802166E4: set state 6 and reset all sub-views.
void func_8021488C(CMCCrystalBox* self) {
    if (self->unk64 != 3) return;
    if (self->unk2CE != 0) return;
    self->unk64 = 6;
    self->unk69 = 0;
    func_801D216C((u8*)self + 0x6c, 0);
    func_801D216C((u8*)self + 0x84, 0);
    func_801D216C((u8*)self + 0xcc, 0);
    func_801D216C((u8*)self + 0xb4, 0);
    func_801F369C((u8*)self + 0x250);
    func_8021A93C((u8*)self + 0x1ec);
    func_8021922C(self);
    func_80138078(0x6d);
}

void func_8021492C(CMCCrystalBox* self) {
    if (self->unk64 != 3) return;
    if (self->unk2CE != 0) return;
    self->unk64 = 8;
    self->unk69 = 0;
    func_801D216C((u8*)self + 0x6c, 0);
    func_801D216C((u8*)self + 0x84, 0);
    func_801D216C((u8*)self + 0xcc, 0);
    func_801D216C((u8*)self + 0xb4, 0);
    func_8021A960((u8*)self + 0x1ec);
    func_802191A4(self);
    func_80138078(0x6d);
}

void func_802149C4(CMCCrystalBox* self) {
    if (self->unk64 != 3) return;
    if (self->unk2CE != 0) return;
    self->unk64 = 0xd;
    self->unk69 = 0;
    func_801D216C((u8*)self + 0x6c, 0);
    func_801D216C((u8*)self + 0x84, 0);
    func_801D216C((u8*)self + 0xcc, 0);
    func_801D216C((u8*)self + 0xb4, 0);
    func_8021A984((u8*)self + 0x1ec);
    func_8021922C(self);
}

void func_80214A54(){}

void func_80214C7C(){}

void func_80214EBC(){}

void func_80215144(){}

void func_80215408(CMCCrystalBox* self) {
    if (CSysWin_getUnk34((char*)self + 0x290) != 0) return;
    if (self->unk2CE != 0) return;
    if (self->unk2D1 != 0) return;
    if ((s8)self->unk2CD == -2) return;

    func_80213788((char*)self + 0x3d8);
    func_802180B4(self);
    func_80218460(self);
    func_80218B10(self);
    func_80138078(0xa);
}

void func_80215490(CMCCrystalBox* self) {
    if (CSysWin_getUnk34((char*)self + 0x290) != 0) return;
    if (self->unk2CE != 0) return;
    if (self->unk2D1 != 0) return;
    if ((s8)self->unk2CD == -2) return;

    func_802137B4((char*)self + 0x3d8);
    func_802180B4(self);
    func_80218460(self);
    func_80218B10(self);
    func_80138078(0xa);
}
void func_80215518(){}

void func_802156C0(){}

struct CMCCrystalBoxLookup {
    int values[8];
    s8 valueIndex[0x49 - 0x20];
};

int lookupIndexedValue_80215AE8(void* self) {
    CMCCrystalBox* obj = static_cast<CMCCrystalBox*>(self);
    if (obj->unk20 == 0) return 0;
    CMCCrystalBoxLookup* lookup = (CMCCrystalBoxLookup*)obj;
    return lookup->values[lookup->valueIndex[((u8*)obj)[0x29]]];
}

void func_80215B18(CMCCrystalBox* self) {
    // Free-slot bookkeeping: unclaim the slot owned at [0x20+slotIdx] and
    // decrement the header counter. Reads after the store are cached in locals
    // so MWCC reloads them once (aliasing) and reuses t for both stores.
    if (self->unk20 == 0) return;

    int* table = (int*)self;
    signed char owner = (s8)((u8*)self)[0x20 + ((u8*)self)[0x29]];
    if (table[owner] == 0) return;

    table[owner] = 0;
    u8 marker = (u8)-1;
    u8 cnt = self->unk20;
    u8 t = ((u8*)self)[0x29];
    self->unk20 = cnt - 1;
    ((u8*)self)[0x21 + t] = marker;
    ((u8*)self)[0x29] = t - 1;
    func_80215B78(self);
}

void func_80215D98(){}

u8 getByte_2D4_8021624C(void* self) { return static_cast<CMCCrystalBox*>(self)->unk2D4; }

u8 getByte_2D5_80216254(void* self) { return static_cast<CMCCrystalBox*>(self)->unk2D5; }

void func_8021625C(){}

unsigned long tableGet_802165CC(unsigned long* table, unsigned int idx) {
    if (idx >= 8) {
        return 0;
    }
    return table[(unsigned char)idx];
}

void copyCrystalBoxParam_802165E8(CMCCrystalBoxParam *dest, const CMCCrystalBoxParam *src) {
    unsigned short *d = (unsigned short *)dest;
    const unsigned short *s = (const unsigned short *)src;
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    unsigned char *db = (unsigned char *)dest;
    const unsigned char *sb = (const unsigned char *)src;
    db[6] = sb[6];
    db[7] = sb[7];
}

void func_80216614(CMCCrystalBox* self) {
    if (self->unk2CE != 0) return;
    if (self->unk2D1 != 0) return;

    // Countdown timer at +0x14F2 increments; when it exceeds +0x14F1 it wraps to 0.
    u8 v = self->field_14F2 + 1;
    u8 limit = self->field_14F1;
    self->field_14F2 = v;
    if ((s8)v >= limit) {
        self->field_14F2 = 0;
    }

    func_80219994(self, 0);
    func_80218018(self);
    func_80138078(0x70);
}

void func_80216698(CMCCrystalBox* self) {
    if (self->unk2CE != 0) return;
    if (self->unk2D1 != 0) return;

    // Countdown timer at +0x14F2; when it wraps sub-range, reload from +0x14F1.
    u8 v = self->field_14F2 - 1;
    self->field_14F2 = v;
    if ((s8)v < 0) {
        self->field_14F2 = self->field_14F1 - 1;
    }

    func_80219994(self, 1);
    func_80218018(self);
    func_80138078(0x70);
}

void func_80216718(){}

void func_80216850(){}

// Retail 0x80218878: return the current crystal-state action id (0x33-0x36)
// or 0x30+active depending on which sub-view / flag is pending.
int func_80216A20(CMCCrystalBox* self) {
    if (CSysWin_getUnk34((u8*)self + 0x290) != 0) return 0x0;
    if (func_801D3320((u8*)self + 0xfc) != 0) return 0x32;
    if (self->unk1500 != 0) return 0x36;
    s8 c = (s8)self->unk2CD;
    if (c == -1) return 0x35;
    if (c == -2) return 0x34;
    u8 idx = self->unk2CC + self->unk2CD * 10;
    if (func_80213710((u8*)self + 0x3d8, idx) != 0) return 0x33;
    return self->field_14A0 != 0 ? 0x31 : 0x30;
}

void func_80216AEC(CMCCrystalBox* self) {
    if (CSysWin_getUnk34((u8*)self + 0x290) != 0) return;
    if (self->unk1500 != 0) return;
    if (self->unk2CE != 0) return;
    if (self->unk2D1 != 0) return;
    if (self->field_14A0 != 0) return;
    self->unk2CD = (u8)-1;
    func_8021852C(self);
    func_80218460(self);
    func_80218B10(self);
    func_80138078(0x2);
}

void func_80216B7C(){}

void func_80216BC8(CMCCrystalBox* self) {
    if (func_80137444(self->subObjPtrs[7], 1.0f) == 0) return;
    self->unk64 = 3;
    func_8021911C(self);
    self->unk69 = 1;
    func_801D216C((u8*)self + 0x6c, 1);
    func_801D216C((u8*)self + 0x84, 1);
    func_8021852C(self);
}

void func_80216C3C(CMCCrystalBox* self) {
    if (func_80137510(self->subObjPtrs[7], 1.0f) != 0) {
        func_8021900C(self);
        self->unk64 = 5;
    }
}

void func_80216C88(CMCCrystalBox* self) {
    if (func_80137510(self->subObjPtrs[6], 1.0f) == 0) return;
    self->unk69 = 1;
    self->unk64 = 0;
    func_801D216C((u8*)self + 0x6c, 0);
}

void func_80216CE0(CMCCrystalBox* self) {
    if (func_80137444(self->subObjPtrs[14], 1.0f) == 0) return;
    self->unk69 = 1;
    self->unk64 = 0;
    func_801D216C((u8*)self + 0x6c, 0);
}

void func_80216D38(CMCCrystalBox* self) {
    if (func_80137444(self->subObjPtrs[9], 1.0f) == 0) return;
    self->unk64 = 9;
    func_8021922C(self);
}

void func_80216D84(CMCCrystalBox* self) {
    if (func_80137444(self->subObjPtrs[10], 1.0f) == 0) return;
    self->unk69 = 1;
    func_8021852C(self);
    self->unk64 = 3;
}

void func_80216DD8(){}

void func_80216E1C(CMCCrystalBox* self) {
    if (func_80137444(self->subObjPtrs[12], 1.0f) == 0) return;
    self->unk64 = 3;
    self->subObjPtrs[12]->SetFrame(lbl_eu_8066845C);
}

void func_80216E6C(){}

void func_80216EB0(CMCCrystalBox* self) {
    if (func_80137510(self->subObjPtrs[10], 1.0f) == 0) return;
    self->unk64 = 0xe;
    func_802191A4(self);
}

void func_80216EFC(CMCCrystalBox* self) {
    if (func_80137510(self->subObjPtrs[9], lbl_eu_80668470) == 0) return;
    self->unk69 = 1;
    self->data.current = 0;   // +0x13DC
    func_80218018(self);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->Animate(0);
    func_8021911C(self);
    func_8021852C(self);
    func_801D216C((u8*)self + 0x84, 1);
    self->unk64 = 3;
}

void func_80216F8C(){}

void func_80217098(){}

void func_80217434(CMCCrystalBox*, unsigned short, unsigned int*, unsigned char){}

void func_802177D0(){}

u8 func_80217BDC(void* self) {
    u32 vtable = *(u32*)self;
    u32 kind = (vtable >> 16) & 0xF;
    u32 flag = ((u8*)self)[7] & 3;
    if (kind != 9) return 0;
    if (flag != 1) return 0;
    return 1;
}

void func_80217C0C(CMCCrystalBox*, unsigned short, unsigned int*, unsigned char){}

// Retail 0x8021A7F4: refresh the sort-menu label from the current crystal id.
void func_8021899C(CMCCrystalBox* self) {
    u32 arr[3];
    arr[0] = lbl_eu_80508870[0];
    arr[1] = lbl_eu_80508870[1];
    arr[2] = lbl_eu_80508870[2];
    unsigned long id = func_8015780C(9);
    int idx = (int)id & 0xFFFF;
    char* s1 = func_80136190(lbl_eu_8050888C + 0xf4, lbl_eu_8050888C + 0xfd, arr[idx]);
    func_80136B4C((nw4r::lyt::Layout*)self->subObjPtrs[5],
                  lbl_eu_8050888C + 0x1ca, s1, 0);
    if (func_801D32DC((u8*)self + 0xfc) != 0) {
        func_801D350C((u8*)self + 0xfc);
        u8 i = 0;
        while (arr[i] > 0) {
            func_801D3518((u8*)self + 0xfc,
                          func_80136190(lbl_eu_8050888C + 0xf4,
                                        lbl_eu_8050888C + 0xfd, arr[i]));
            i++;
        }
        func_801D353C((u8*)self + 0xfc, self->field_1506 + self->field_1507);
    }
}

void func_80218A80(CMCCrystalBox* self) {
    int i;
    for (i = 0; i < 8; i++) {
        unsigned int* item = (unsigned int*)tableGet_802165CC((unsigned long*)((u8*)self + 0x1480), (unsigned char)i);
        unsigned short id;
        if (item != 0) {
            id = (unsigned short)(item[0] >> 20);
        } else {
            id = 0;
        }
        func_80217434(self, id, item, (unsigned char)i);
        func_80217C0C(self, id, item, (unsigned char)i);
    }
}

struct CMCCrystalRec5 { unsigned short m0; unsigned short m2; unsigned char m4; };
void copyCrystalRec5_80218FD4(CMCCrystalRec5* dst, const CMCCrystalRec5* src) {
    *dst = *src;
}

struct CMCCrystalRec5Ex {
    unsigned short a;
    unsigned short b;
    unsigned char c;
};

void copyCrystalRec5Ex_80218FF0(CMCCrystalRec5Ex* dst, CMCCrystalRec5Ex* src) {
    unsigned short a = src->a;
    unsigned short b = src->b;
    unsigned char c = src->c;
    dst->a = a;
    dst->b = b;
    dst->c = c;
}

// CMCCrystalBox holds a nw4r::lyt::Layout* at +0x38 (== subObjPtrs[5]) with the
// anim transform to play at +0x3C..+0x5F. These play back a bound animation.
void func_8021900C(CMCCrystalBox* self) {
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->BindAnimation(self->subObjPtrs[6]);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->SetAnimationEnable(self->subObjPtrs[6], true);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->Animate(0);
}

void func_80219094(CMCCrystalBox* self) {
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->BindAnimation(self->subObjPtrs[7]);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->SetAnimationEnable(self->subObjPtrs[7], true);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->Animate(0);
}

void func_8021911C(CMCCrystalBox* self) {
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->BindAnimation(self->subObjPtrs[8]);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->SetAnimationEnable(self->subObjPtrs[8], true);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->Animate(0);
}

void func_802191A4(CMCCrystalBox* self) {
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->BindAnimation(self->subObjPtrs[9]);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->SetAnimationEnable(self->subObjPtrs[9], true);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->Animate(0);
}

void func_8021922C(CMCCrystalBox* self) {
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->BindAnimation(self->subObjPtrs[10]);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->SetAnimationEnable(self->subObjPtrs[10], true);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->Animate(0);
}

void func_802192B4(CMCCrystalBox* self) {
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->BindAnimation(self->subObjPtrs[11]);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->SetAnimationEnable(self->subObjPtrs[11], true);
    self->subObjPtrs[11]->SetFrame(lbl_eu_8066845C);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->Animate(0);
}

void func_80219348(CMCCrystalBox* self) {
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->BindAnimation(self->subObjPtrs[12]);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->SetAnimationEnable(self->subObjPtrs[12], true);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->Animate(0);
}

void func_802193D0(CMCCrystalBox* self) {
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->BindAnimation(self->subObjPtrs[13]);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->SetAnimationEnable(self->subObjPtrs[13], true);
    self->subObjPtrs[13]->SetFrame(lbl_eu_8066845C);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->Animate(0);
}

void func_80219464(CMCCrystalBox* self) {
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->BindAnimation(self->subObjPtrs[14]);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->SetAnimationEnable(self->subObjPtrs[14], true);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->Animate(0);
}

void func_802194EC(){}

void func_80219AF0(){}

void copyShortPair_80219D10(void* dst, void* src){
    *(unsigned short*)dst = *(unsigned short*)src;
    *(unsigned short*)((char*)dst + 2) = *(unsigned short*)((char*)src + 2);
}

void CMCCrystalBox::OnFileEvent() {}

void func_80219D10(CMCCrystalBoxParam* dst, const CMCCrystalBoxParam* src) {
    dst->m0 = (unsigned short)src->m0;
    dst->m2 = (unsigned short)src->m2;
}
