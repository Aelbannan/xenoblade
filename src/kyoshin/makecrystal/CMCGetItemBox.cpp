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
extern "C" void func_80136A1C(nw4r::lyt::Layout*, char*, char*, u32);  // retail reloc is unmangled
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
extern "C" int  func_800A9D90();   // retail reloc is unmangled
extern "C" void func_801D4054(CItemBoxInfo*);   // retail reloc is unmangled
extern "C" void func_801D4154(CItemBoxInfo*);   // retail reloc is unmangled
extern "C" void func_801D40C4(CItemBoxInfo*);   // retail reloc is unmangled
extern "C" void func_801D202C(CMCItemBoxSubObj*);
extern "C" void func_801D20B0(CMCItemBoxSubObj*, nw4r::lyt::DrawInfo*);   // retail reloc is unmangled
extern "C" void func_8022B7C8(CMCGetItemBoxSysWin*, nw4r::lyt::DrawInfo*);   // retail reloc is unmangled
extern "C" void func_8022B748(CMCGetItemBoxSysWin*);   // retail reloc is unmangled
void __dt__12CItemBoxInfoFv(CItemBoxInfo*, int);
void __dt__7CSysWinFv(CMCGetItemBoxSysWin*, int);
// Retail reloc names for the cursor dtors are unmanged C symbols.
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dt__6CCur16Fv(void*, int);
extern "C" void __dt__6CCur09Fv(void*, int);
extern "C" void __dt__6CCur07Fv(void*, int);
void __dt__17UnkClass_8045F564Fv(UnkClass_8045F564*, int);

// Dispatch shim for the CSysWin vtable entry at vtable offset 0x88. MWCC
// places the first user virtual at vtable offset 8, so declared slot 32
// (init) lands at 0x88.
// Dispatch-only shim for the CCur18 sub-object vtable entry at offset 0x10
// (MWCC RTTI occupies slots 0,4, so the third declared virtual lands there).
struct CMCItemBoxSubObjCall {
    virtual void vt08() = 0;
    virtual void vt0C() = 0;
    virtual void call(void* arg) = 0;   // vtable offset 0x10
};

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
// Same-unit helper functions (retail relocs are unmangled -> C linkage).
extern "C" void func_802988BC(CMCGetItemBox*);
extern "C" void func_80298938(CMCGetItemBox*);
extern "C" void func_802989A4(CMCGetItemBox*);
extern "C" void func_80298A20(CMCGetItemBox*);
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

// -O4,s keeps both passes rolled (pointer-walk + mtctr/bdnz sthx) as in retail.
#pragma push
#pragma optimize_for_size on
// noinline: retail keeps this init helper out-of-line (bl from __ct__CMCGetItemBox).
__declspec(noinline) void func_80296B44(CMCItemBoxSub* x) {
    // Retail walks a pointer over the table (cmplw loop); the inline bound is
    // CSE-hoisted as an r0 temp, matching retail coloring. Known residual:
    // retail emits the hoisted addi before the param copy; every tested shape
    // (decl order, dependent derivation, alias, comma-seq, while-guard) keeps
    // the copy first.
    s16* p = x->table;
    do {
        *p++ = -1;
    } while (p < x->table + 0x80);
    x->count = 0;
    x->pad_102 = 0;
    x->limit = 0;
    x->counter = 0;
    x->shortName.mString[0] = 0;
    x->shortName.mLength = 0;
    x->name.mString[0] = 0;
    x->name.mLength = 0;
    x->listBase = (CMCItemBoxEntry*)0;
    x->field_1D4 = 0;
    // Second pass over the same table: u16 index drives an mtctr/bdnz loop.
    // optimize_for_size keeps MWCC from unrolling it (retail stays rolled).
    for (u16 i = 0; i < 0x80; i++) x->table[i] = -1;
}
#pragma pop

// Rebuild the item-box index table: store the entry base/type, reset the
// offset table to 0xFFFF, then re-index every entry whose type word matches
// pad_102, and finally compute the page limit as ceil(filled-count / 30).
// -O4,s keeps the retail stmw r28 frame.
#pragma push
#pragma optimize_for_size on
void func_80296BF0(CMCItemBoxSub* x, u8 pad102, CMCItemBoxEntry* listBase, u8 field1D4) {
    x->listBase = listBase;
    x->field_1D4 = field1D4;
    x->count = 0;
    x->pad_102 = pad102;
    for (u16 i = 0; i < 0x80; i++) x->table[i] = -1;
    // Cached byte limit lets MWCC hoist the bound out of the loop (retail).
    u8 entryCount = x->field_1D4;
    for (u16 i = 0; i < entryCount; i++) {
        CMCItemBoxEntry* e = &x->listBase[(u16)i];
        if (e != 0) {
            // Type nibble lives at bit 16 here (retail rlwinm rot 16).
            if (x->pad_102 == (int)((e->field_00 >> 16) & 0xF)) {
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
#pragma pop

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
__declspec(noinline) u32 func_80296D54(CMCItemBoxSub* x, u32 index) {
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

// auto_inline off: retail keeps the single-call-site bl from func_8029967C
// (MWCC otherwise folds this small helper in despite noinline).
#pragma auto_inline off
__declspec(noinline) s8 func_80296E00(CMCItemBoxSub* x, u32 index) {
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
#pragma auto_inline on

// Format the selected entry's item-name into the FixStr<64> at sub+0x108
// (object offset 0x41C). Returns that FixStr, or null when out of range.
// -O4,s keeps the retail stmw r28 frame.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) char* func_80296E98(CMCItemBoxSub* sub, u16 index) {
    // Declared in this order so MWCC colors callee-saved regs like retail
    // (sub-copy -> r31, string-table base -> r30, entry ptr -> r29).
    CMCItemBoxSub* x = sub;
    const char* strTbl = &lbl_eu_8050FF8C[0];
    CMCItemBoxEntry* base = x->listBase;
    if (base == 0) return 0;
    u16 idx = (u16)(index + (s8)x->counter * 30);
    if (idx >= x->count) return 0;
    // The s16 table offset sign-extends before the *52 scale (retail lhax/mulli).
    CMCItemBoxEntry* p = base + x->table[idx];
    if (p == 0) return 0;
    CMCItemImplShim* inst = (CMCItemImplShim*)CItem_initItemImplInstances(p);
    // Keep only x/p/strTbl live across calls: recompute &x->shortName
    // at every use so MWCC holds just three callee-saved regs (retail r29-r31).
    ((ml::FixStr<64>*)&x->shortName)->format(strTbl, inst->getName(p));
    if (x->pad_102 == 3) {
        // Gem slot: rebuild the name as "<kind suffix><saved name>".
        CMCItemImplShim* inst2 = (CMCItemImplShim*)CItem_initItemImplInstances(p);
        u32 kind = (u8)inst2->getKind(p);
        char* itemName = func_80136190(&strTbl[3], &strTbl[0xc],
                                       0x1e - (kind - 1));
        // Refresh the cached length, then splice: format("%s%s", saved, suffix).
        x->shortName.mLength = strlen(x->shortName.mString);
        char copy[64];
        strcpy(copy, x->shortName.mString);
        ((ml::FixStr<64>*)&x->shortName)->format(&strTbl[0x11], copy, itemName);
    }
    return (char*)&x->shortName;
}
#pragma pop

// Retail 0x802995F8: format the selected entry's item text into the
// FixStr<128> buffer at sub+0x14C and return it. Entries owned by the bdat
// manager (func_801C6E90 != 0) use the item-name database; otherwise the
// text is built from the message-string placeholders: '%1' splices the
// item icon name (switch on func_801361E8's kind), '%2' splices a kind-apped
// table string from lbl_eu_8050FF60, and the two-byte shift-JIS ranges are
// stepped over. The whole formatted buffer is then copied back into the
// FixStr (its mLength refreshed).
// -O4,s keeps the retail stmw r18 frame.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) char* func_80296FC0(CMCItemBoxSub* sub, u16 index) {
    CMCItemBoxEntry* base = sub->listBase;
    if (base == 0) return 0;
    // Retail sign-extends the page counter, scales by 30 and folds the u16
    // clamp into an unsigned compare against count.
    u16 idx = (u16)(index + (s8)sub->counter * 30);
    if (idx >= sub->count) return 0;
    // The s16 table offset sign-extends before the *52 scale.
    CMCItemBoxEntry* p = base + sub->table[idx];
    if (p == 0) return 0;
    if (func_801C6E90(p) != 0) {
        // bdat-managed entry: name comes from the item database.
        void* inst = CItem_initItemImplInstances(p);
        u32 v = (u32)((u32(*)(void*, void*))(*(void***)inst)[0x22])(inst, p);
        char* s = func_8013639C((void*)lbl_eu_80664100, &lbl_eu_8050FF8C[0x16], (u16)v);
        ((ml::FixStr<128>*)&sub->name)->format(&lbl_eu_8050FF8C[0], s);
        return (char*)&sub->name;
    }
    u32 type = (p->field_00 >> 12) & 0xF;
    int special = 0;
    if (type == 9 && (p->bytes[3] & 3) == 3) special = 1;
    if (special) {
        char* s = func_80136190(&lbl_eu_8050FF8C[3], &lbl_eu_8050FF8C[0xc], 0x9c);
        ((ml::FixStr<128>*)&sub->name)->format(&lbl_eu_8050FF8C[0], s);
        return (char*)&sub->name;
    }
    u8 pad102 = sub->pad_102;
    u32 id = p->field_00 >> 20;
    if (pad102 == 3) {
        // Gem slot: icon/kind/name triple drives the message placeholders.
        // Declared in this order so MWCC lays the buffers out at the retail
        // stack offsets (tbl / fmtBuf / buf48 / tail / copy).
        void* db = (void*)lbl_eu_806640D8;
        void* inst1 = CItem_initItemImplInstances(p);
        u32 icon = (u32)((u32(*)(void*, void*))(*(void***)inst1)[0x15])(inst1, p);
        void* inst2 = CItem_initItemImplInstances(p);
        u8 kind = (u8)((u32(*)(void*, void*))(*(void***)inst2)[2])(inst2, p);
        void* inst3 = CItem_initItemImplInstances(p);
        char* nm = (char*)((void*(*)(void*, void*))(*(void***)inst3)[0x24])(inst3, p);
        CMCItemMsgTbl tbl;
        char fmtBuf[0x20];
        char buf48[0x20];
        char tail[0x80];
        char copy[0x80];
        sprintf(buf48, &lbl_eu_8050FF8C[0x1e], nm);
        u8 mkind = (u8)func_801361E8((u32)db, &lbl_eu_8050FF8C[0x21], (u16)icon);
        char* s2 = func_8013639C((void*)db, &lbl_eu_8050FF8C[0x2a], (u16)icon);
        ((ml::FixStr<128>*)&sub->name)->format(&lbl_eu_8050FF8C[0], s2);
        u32 len = strlen(sub->name.mString);
        strcpy(copy, sub->name.mString);
        // Japanese/Korean text uses different message variants; retail calls
        // getLanguage twice instead of caching the result.
        int langB = 1;
        if (CDeviceSC::getLanguage() != 3 && CDeviceSC::getLanguage() != 2) langB = 0;
        // Retail keeps the copied table's element base in a callee-saved
        // register across the entire walk loop ('%2' handler reads it).
        const u32* tvals = &tbl.v[0];
        char* p2 = copy;
        while (*p2 != 0) {
            if (*p2 == '%') {
                memset(fmtBuf, 0, 0x20);
                p2++;
                if (*p2 == '1') {
                    p2++;
                    // Retail keeps every case's sprintf block separate.
                    switch ((u8)mkind) {
                        case 1: sprintf(fmtBuf, &lbl_eu_8050FF8C[0x36], buf48); break;
                        case 2: sprintf(fmtBuf, &lbl_eu_8050FF8C[0x36], buf48); break;
                        case 3:
                            if (langB) sprintf(fmtBuf, &lbl_eu_8050FF8C[0x4c], buf48);
                            else       sprintf(fmtBuf, &lbl_eu_8050FF8C[0x65], buf48);
                            break;
                        case 4:
                            if (langB) sprintf(fmtBuf, &lbl_eu_8050FF8C[0x4c], buf48);
                            else       sprintf(fmtBuf, &lbl_eu_8050FF8C[0x65], buf48);
                            break;
                    }
                } else if (*p2 == '2') {
                    p2++;
                    tbl = lbl_eu_8050FF60;
                    u8 s3 = func_801361E8((u32)db, (const char*)tvals[kind], (u16)icon);
                    if (langB) sprintf(fmtBuf, &lbl_eu_8050FF8C[0x7d], s3);
                    else       sprintf(fmtBuf, &lbl_eu_8050FF8C[0x96], s3);
                }
                // Splice the formatted text in place of the '%x' pair. Retail
                // measures fmtBuf with a rolled inline byte-walk loop, not
                // strlen.
                u32 flen = 0;
                const char* q = fmtBuf;
                while (*q++ != 0) flen++;
                sprintf(tail, &lbl_eu_8050FF8C[0], p2);
                p2 -= 2;
                sprintf(p2, &lbl_eu_8050FF8C[0], fmtBuf);
                p2 += flen;
                sprintf(p2, &lbl_eu_8050FF8C[0], tail);
            } else if (((s8)*p2 >= 0x81 && (s8)*p2 <= 0x9f) ||
                       ((s8)*p2 >= (s8)0xe0 && (s8)*p2 <= 0xef)) {
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
        char* s = func_8013639C(tbl2, &lbl_eu_8050FF8C[0x16], (u16)n);
        ((ml::FixStr<128>*)&sub->name)->format(&lbl_eu_8050FF8C[0], s);
        return (char*)&sub->name;
    }
}
#pragma pop

// Constructor: zero the handle/pointer fields, placement-construct the cursor
// widgets / sys-win / item-box info, then build default temporaries on the
// stack and copy them member-wise into the embedded item-box info, cursor
// table and sub-object storage (everything past each temp's first dword).
// Cursor pair for the rolled pointer-walk copies below.
CMCGetItemBox::CMCGetItemBox() {
    // mVtbl is stored by the CMCGetItemBoxVt base constructor.
    // memRegion1/memRegion2 are constructed implicitly (declaration order).

    fileHandle1 = 0;
    fileHandle2 = 0;
    fileHandle3 = 0;
    fileHandle4 = 0;
    memManagerPtr = 0;
    arcAcc1 = 0;
    arcAcc2 = 0;
    layout40 = 0;
    animTrans1 = 0;
    animTrans2 = 0;
    field_4C = 0;
    field_4D = 0;
    objAt50 = 0;
    field_54 = 0;
    mField55 = 1;

    __ct__CCur07(&subObj_58, 0);
    __ct__CCur09(&subObj_70, 0);
    __ct__CCur16(&subObj_88, 0);
    __ct__CCur18(&subObj_A0, 0);
    __ct__CSysWin(&sysWin_B8, 0);
    __ct__CItemBoxInfo(itemBox, 0, 0);

    field_300 = 0;
    field_301 = 0;
    mField303 = 0;
    field_304 = 0;
    field_305 = 0;
    func_80296B44(&sub_314);

    // --- default item-box temp -> embedded info: one member-wise struct
    //     assignment (dest = src + 0xf4); word-array members lower to the
    //     retail paired lwzu/stwu bdnz loops. ---
    u8 boxTmp[0x210];
    __ct__CItemBoxInfo(boxTmp, 0, 0);
    *(CMCBoxMember*)&itemBox[0x04] = *(CMCBoxMember*)&boxTmp[0x04];
    // trailing 128-byte run starts one byte past a word boundary (retail
    // keeps it outside the aligned member block)
    *(CMCW32*)&itemBox[0x189] = *(CMCW32*)&boxTmp[0x189];
    __dt__12CItemBoxInfoFv(boxTmp, -1);

    // --- default sys-win temp -> embedded window: one struct assignment ---
    {
        u8 winTmp[0x40];
        __ct__CSysWin(winTmp, 0);
        *(CMCSysWinMember*)&sysWin_B8.bytes[4] = *(CMCSysWinMember*)&winTmp[4];
        __dt__7CSysWinFv(winTmp, -1);
    }

    // --- fresh CMCItemBoxSub temp -> embedded sub-object ---
    lbl_eu_80664A18 = 0;
    lbl_eu_80664A1C = 0;
    lbl_eu_80664A20 = 0;
    {
        u8 subTmp[0x1d8];
        func_80296B44((CMCItemBoxSub*)subTmp);
        *(CMCSubTableCopy*)sub_314.table = *(CMCSubTableCopy*)subTmp;
        // FixStr operator= inlines to the retail strlen/strcpy pair.
        CMCItemBoxSub* t = (CMCItemBoxSub*)subTmp;
        sub_314.count = t->count;
        sub_314.pad_102 = t->pad_102;
        sub_314.limit = t->limit;
        sub_314.counter = t->counter;
        *(ml::FixStr<64>*)&sub_314.shortName = *(ml::FixStr<64>*)&t->shortName;
        *(ml::FixStr<128>*)&sub_314.name = *(ml::FixStr<128>*)&t->name;
        sub_314.listBase = t->listBase;
        sub_314.field_1D4 = t->field_1D4;
    }
}

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
// -O4,s keeps the retail stmw r30 frame.
#pragma push
#pragma optimize_for_size on
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
#pragma pop

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
// -O4,s keeps the retail stmw r30 frame.
#pragma push
#pragma optimize_for_size on
void func_80297AAC(CMCGetItemBox* self, nw4r::lyt::DrawInfo* di) {
    if (self->field_4C == 0) return;
    if (self->field_4D == 0) return;
    func_801D4154((CItemBoxInfo*)self->itemBox);
    func_80137038(self->layout40, di, 0, 1);
    u8 active = self->sub_314.limit ? self->sub_314.limit : 1;
    if (active > 1) func_801D20B0(&self->subObj_70, di);
    func_801D20B0(&self->subObj_58, di);
    func_801D20B0(&self->subObj_88, di);
    func_801D20B0(&self->subObj_A0, di);
    func_8022B7C8(&self->sysWin_B8, di);
}
#pragma pop

// Clean up every resource owned by the item-box widget.
// -O4,s keeps the retail stmw r30 frame.
#pragma push
#pragma optimize_for_size on
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
// -O4,s keeps the retail stmw r28 frame.
#pragma push
#pragma optimize_for_size on
extern "C" void func_80297D2C(CMCGetItemBox* self, u16 arg, void* unk, u8 byte) {
    if (self->field_4D != 0) return;
    self->field_4D = 1;
    self->mField55 = 0;
    self->field_300 = 0;
    // Signed lvalue keeps the constant negative (li r0,-1, not li r0,255).
    (s8&)self->field_301 = -1;
    func_801D421C((CItemBoxInfo*)self->itemBox);
    func_80299530(self, arg, unk, byte);
    func_801D4260((CItemBoxInfo*)self->itemBox, arg);
    // Declared in this order so MWCC colors the callee-saved regs like retail
    // (entry->r31, idx->r30, sub->r29).
    CMCItemBoxEntry* entry;
    u8 idx;
    CMCItemBoxSub* sub;
    idx = (u8)(self->field_301 * 10 + self->field_300);
    sub = &self->sub_314;
    entry = func_80296DB0(sub, idx);
    u32 iconId = func_80296D54(sub, idx);   // held in a reg temp in retail
    func_801D47D4((CItemBoxInfo*)self->itemBox, (u16)iconId, (u32)entry, 1);
    func_801D4AE0((CItemBoxInfo*)self->itemBox, 1, func_80296E98(sub, idx));
}
#pragma pop

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
// -O4,s keeps the retail stmw r29 frame.
#pragma push
#pragma optimize_for_size on
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
        ((CMCItemBoxSubObjCall*)&self->subObj_A0)->call((void*)tmp);
    } else {
        u8 v = (u8)(self->field_301 - 1);
        self->field_301 = v;
        if ((s8)v < -1) self->field_301 = 2;
        func_802999B0(self);
        func_802998C8(self);
    }
    func_80138078(0x1);
}
#pragma pop

// Advance the cursor to the next non-empty slot.
// Cursor branch is the fall-through (retail beq jumps to the sort branch).
// -O4,s keeps the retail stmw r29 frame.
#pragma push
#pragma optimize_for_size on
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
        ((CMCItemBoxSubObjCall*)&self->subObj_A0)->call((void*)tmp);
    } else {
        u8 v = self->field_301 + 1;
        self->field_301 = v;
        // Signed lvalue keeps the constant negative: MWCC emits li r0,-1
        // (a plain u8 store folds it to li r0,255).
        if ((s8)v >= 3) (s8&)self->field_301 = -1;
        func_802999B0(self);
        func_802998C8(self);
    }
    func_80138078(0x1);
}
#pragma pop

// -O4,s keeps the retail stmw r29 frame.
#pragma push
#pragma optimize_for_size on
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
        ((CMCItemBoxSubObjCall*)&self->subObj_A0)->call((void*)tmp);
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
#pragma pop

#pragma push
#pragma optimize_for_size on
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
        ((CMCItemBoxSubObjCall*)&self->subObj_A0)->call((void*)tmp);
    } else {
        // Retail reuses the loaded field_300 for the decrement (subi r3-based).
        u8 s = self->field_300;
        if ((s8)s == 9) {
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
// -O4,s keeps the retail stmw r30 frame.
#pragma push
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_80298378(CMCGetItemBox* self) {
    CMCItemBoxSub* x = &self->sub_314;
    func_80296D00(x);
    func_8029967C(self);
    func_802998C8(self);
    // Retail: li r0,1 default, conditionally overwritten with the limit (branchy
    // select); splitting the select across blocks makes MWCC forget the byte
    // load was zero-extended, keeping the redundant clrlwi/cmpli pair.
    // u8-local select: assigning the ternary to a u8 makes MWCC materialize
    // the zero-extension (clrlwi) at the join before the compare (retail).
    u8 lim = x->limit != 0 ? x->limit : 1;
    if (lim != 1) func_80138078(0xa);
}
#pragma pop

// Decrement the sub counter, refresh helper widgets, and play a sound when the
// item-box limit is anything other than 1.
#pragma push
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_802983E4(CMCGetItemBox* self) {
    CMCItemBoxSub* x = &self->sub_314;
    func_80296D2C(x);
    func_8029967C(self);
    func_802998C8(self);
    // Retail: li r0,1 default, conditionally overwritten with the limit (branchy
    // select); the u8-local select makes MWCC forget the byte load was
    // zero-extended, keeping the redundant clrlwi/cmpli pair at the join.
    u8 lim = x->limit != 0 ? x->limit : 1;
    if (lim != 1) func_80138078(0xa);
}
#pragma pop

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
// optimize_for_size: -O4,s lowers the >= into the retail subfc carry chain.
#pragma push
#pragma optimize_for_size on
u32 func_80298540(CMCGetItemBox* self) {
    if (self->field_4D == 0) return 0;
    if ((s8)self->field_301 == -1) {
        // Retail lowers the place calc to a subfc/carry/subf chain.
        u32 v = (u16)func_80157CD0(self->sub_314.pad_102);
        u32 count = self->sub_314.count;
        if (v >= count) return 1;
        return 2;
    }
    return 0;
}
#pragma pop

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
// -O4,s keeps the retail _savegpr_29/_restgpr_29 call-form prologue (three
// callee-saved regs live across calls: self, cursor-table base, index).
#pragma push
#pragma optimize_for_size on
void func_80298614(CMCGetItemBox* self) {
    if (self->mField303 != 0) {
        if (CSysWin_getUnk34(&self->sysWin_B8) != 0) {
            if (CSysWin_isActive(&self->sysWin_B8) != 0) {
                func_8022B8E4(&self->sysWin_B8);
                func_801D216C(&self->subObj_A0, 1);
            }
        } else {
            CMCItemBoxCursor* arr = &self->arr_1A4;
            u8 combined = (u8)(self->field_305 + (s8)self->field_304 * 4);
            u16 r = ArrayGet12(arr->table, combined);
            if ((u16)r != 0) {
                u8 v = combined < 0xc ? arr->flags[combined] : 0;
                void* res;
                if (v == 3) {
                    res = func_801D3C74(arr->table, combined);
                } else {
                    res = func_80136190(&lbl_eu_8050FF8C[0x119],
                                        &lbl_eu_8050FF8C[0x123], r);
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
        CMCItemBoxCursor* arr = &self->arr_1A4;
        int found = 0;
        for (s32 i = 0; i < 0xc; i++) {
            if (arr->table[(u8)i] != 0) {
                arr->field_D8 = (u8)i;
                if (arr->table[8] != 0) arr->field_D8 = 8;
                found = 1;
                break;
            }
        }
        if (found != 0) {
            u8 v = arr->field_D8;
            // Retail's named-pool u32->f64 conversion (0x43300000 trick with
            // lbl_eu_80668BD8 as the subtracted 2^52 constant).
            union { double d; u32 w[2]; } cvt;
            cvt.w[1] = v;
            cvt.w[0] = 0x43300000u;
            s32 rowN = (s32)(lbl_eu_80668BEC * (f32)(cvt.d - lbl_eu_80668BD8));
            self->mField303 = 1;
            self->field_304 = (u8)rowN;
            self->field_305 = (u8)(v - (s8)rowN * 4);
            func_801D216C(&self->subObj_58, 0);
            func_801D216C(&self->subObj_A0, 1);
            u8 tmp[12];
            func_801CB9D8((u32*)tmp, arr->table, v);
            (*(void(**)(void*, void*))((void**)&self->subObj_A0)[4])(&self->subObj_A0, (void*)tmp);
            func_80138078(2);
        } else {
            func_80138078(5);
        }
    }
}
#pragma pop

// Return a UI part index: 0 when the window exists, 0x36 when the layout flag
// is set, otherwise 0x39/0x3A based on the sign of the sort field.
u32 func_80298850(CMCGetItemBox* self) {
    if (CSysWin_getUnk34(&self->sysWin_B8) != 0) return 0;
    if (self->mField303 != 0) return 0x36;
    return 0x39 + ((s8)self->field_301 == -1);
}

// Open the item box: advance the second layout animation and, once it has
// finished, initialise the state/widgets and refresh the cursor.
extern "C" __declspec(noinline) void func_80298938(CMCGetItemBox* self) {
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
extern "C" __declspec(noinline) void func_802988BC(CMCGetItemBox* self) {
    if (func_80137444((nw4r::lyt::AnimTransform*)self->animTrans1, lbl_eu_80668BF0) != 0) {
        self->layout40->SetAnimationEnable((nw4r::lyt::AnimTransform*)self->animTrans1, false);
        self->layout40->SetAnimationEnable((nw4r::lyt::AnimTransform*)self->animTrans2, true);
        self->field_4D = 2;
    }
}

// Rewind the second layout animation via func_80137510; when finished, enable
// the two anim transforms on the layout and move to state 5.
extern "C" __declspec(noinline) void func_802989A4(CMCGetItemBox* self) {
    if (func_80137510((nw4r::lyt::AnimTransform*)self->animTrans2, lbl_eu_80668BF0) != 0) {
        self->layout40->SetAnimationEnable((nw4r::lyt::AnimTransform*)self->animTrans2, false);
        self->layout40->SetAnimationEnable((nw4r::lyt::AnimTransform*)self->animTrans1, true);
        self->field_4D = 5;
    }
}

// Rewind the first layout animation; when it has finished, rearm the
// state bytes and reattach the cursor sub-object.
extern "C" __declspec(noinline) void func_80298A20(CMCGetItemBox* self) {
    if (func_80137510(self->animTrans1, lbl_eu_80668BF0) != 0) {
        self->mField55 = 1;
        self->field_4D = 0;
        func_801D216C(&self->subObj_58, 0);
    }
}

// Guard: the item box only accepts input once the layout, the second arc
// accessor and its three state words are all present. One OR-chain; MWCC
// emits four beqlr shortcuts plus an inverted final test around the stores.
void func_80298A78(CMCGetItemBox* self) {
    if (self->layout40 == 0 || self->arcAcc2 == 0 || lbl_eu_80664A18 == 0 ||
        lbl_eu_80664A1C == 0 || lbl_eu_80664A20 == 0) {
        return;
    }
    self->field_54 = 1;
    self->field_4C = 1;
}

// nw4r ArcResourceAccessor virtual GetResource at vtable[3] (offset 0x0C).
// MWCC RTTI occupies vtable slots 0,4, so the second declared virtual lands at
// offset 0x0C: one dummy slot + GetResource3.
struct AccessorGetRes3 {
    virtual void _v00();
    virtual void* GetResource3(u32 tag, const char* name, int r6);
};

// Dispatch shim for the device-font vtable entry at offset 0x24
// (MWCC RTTI occupies slots 0,4, so the eighth declared virtual lands there):
// makes MWCC emit the lwz r12 / lwz r12,0x24(r12) / bctrl chain.
struct CDeviceFontVt9 {
    virtual void _v00() = 0; virtual void _v04() = 0; virtual void _v08() = 0;
    virtual void _v0C() = 0; virtual void _v10() = 0; virtual void _v14() = 0;
    virtual void _v18() = 0;
    virtual void* getResource() = 0;   // vtable offset 0x24
};

// Retail 0x80298AC8: resolve the icon resource for a selected item-box entry
// and stamp it into a numbered layout pane. Mirrors func_80298FB4 but uses the
// icon database (func_801361E8) and the 0x144-0x149 icon-name chain for both
// the gem (type 3) and item (type 9) paths, plus a %d pane-name format at
// &lbl[0x14e].
// noinline: retail keeps the bl to this symbol from func_8029967C.
extern "C" __declspec(noinline) void func_80298AC8(CMCGetItemBox* self, u32 idx, CMCItemBoxEntry* entry, u8 n) {
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
// -O4,s keeps the retail stmw r27 frame.
#pragma push
#pragma optimize_for_size on
// noinline: retail keeps the bl to this symbol from func_8029967C (its body
// otherwise folds in, surfacing as a stray CItem_initItemImplInstances call).
__declspec(noinline) void func_80298FB4(CMCGetItemBox* self, u32 idx, CMCItemBoxEntry* entry, u8 n) {
    CMCItemBoxEntry* e = entry != 0 ? entry : 0;
    void* h = 0;
    if (e != 0) {
        u32 type = (e->field_00 >> 16) & 0xF;
        if (type == 3) {
            h = 0;
            CMCItemImplShim* inst = (CMCItemImplShim*)CItem_initItemImplInstances(e);
            switch ((u16)inst->getKind(e)) {
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
            CMCItemImplShim* inst = (CMCItemImplShim*)CItem_initItemImplInstances(e);
            if ((u16)inst->getCount(e) == 0) {
                // Empty slot: only resolve a name when the entry is a valid
                // type-9 item (bdat-managed or flagged in byte 7).
                do {
                    if (func_801C6E90(e) == 0) {
                        u32 ex = (e->field_00 >> 16) & 0xF;
                        int f = 0;
                        if (ex == 9 && ((u32)(e->bytes[3] & 3)) == 1) f = 1;
                        if (f == 0 && ex != 9) break;
                    }
                    h = 0;
                    CMCItemImplShim* inst2 = (CMCItemImplShim*)CItem_initItemImplInstances(e);
                    switch ((u16)inst2->getKind(e)) {
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
                } while (0);
            } else {
                // Item present: kind-keyed name chain 0x19c..
                h = 0;
                CMCItemImplShim* inst2 = (CMCItemImplShim*)CItem_initItemImplInstances(e);
                switch ((u16)inst2->getKind(e)) {
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
            }
        }
    }
    if (h == 0) {
        if (idx != 0) {
            char* name = func_80138F78((u16)func_80136254(lbl_eu_806640EC, &lbl_eu_8050FF8C[0x15c], idx));
            h = ((AccessorGetRes3*)self->arcAcc2)->GetResource3(0x74696d67u, name, 0);
            if (h == 0)
                h = ((AccessorGetRes3*)self->arcAcc1)->GetResource3(0x74696d67u, &lbl_eu_8050FF8C[0x131], 0);
        } else {
            h = ((AccessorGetRes3*)self->arcAcc1)->GetResource3(0x74696d67u, &lbl_eu_8050FF8C[0x131], 0);
        }
    }
    if (h != 0) {
        char buf[0x20];
        sprintf(buf, &lbl_eu_8050FF8C[0x161], (int)(n + 1));
        func_80137E7C((void*)self->layout40, buf, (void*)h);
    }
}
#pragma pop
#pragma push
#pragma optimize_for_size on
// noinline + C linkage: called out-of-line from func_8029967C in retail
// (unmangled reloc).
__declspec(noinline) void func_80299490(CMCGetItemBox* self, int r4, u32 r5) {
    char buf1[0x20];   // sp+0x28
    char buf2[0x20];   // sp+0x08
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
#pragma pop

// Refresh layout text after index/sort change. Extra params are passed through
// by func_80297D2C but ignored here (retail never reads r5/r6).
// -O4,s keeps the retail stmw r29 frame (self/arg/sub live across calls).
#pragma push
#pragma optimize_for_size on
// noinline: retail keeps this out-of-line; without it MWCC inlines the whole
// body into func_80297D2C (late-definition inlining defeats auto_inline off).
extern "C" __declspec(noinline) void func_80299530(CMCGetItemBox* self, u16 arg, void* unk, u8 byte) {
    CMCItemBoxSub* sub = &self->sub_314;
    // Retail calls func_80296BF0 with only r3 set (r4/r5/r6 carry garbage),
    // so invoke it through a 1-arg prototype to avoid li r4/r5/r6 setup.
    ((void (*)(CMCItemBoxSub*))func_80296BF0)(sub);
    // Scoped string-table base: the first use dies at the call (retail computes
    // it straight into the arg reg), while each branch's base lives across the
    // two FindPaneByName calls (retail parks it in a callee-saved reg).
    // Int-temp branchy select + u8 conversion at the call site reproduces the
    // retail li r0,1 / mr / clrlwi chain.
    {
        char* tbl = lbl_eu_8050FF8C;
        int count = sub->limit == 0 ? 1 : sub->limit;
        func_80136910((nw4r::lyt::Layout*)self->layout40, &tbl[0x17b], (u8)count);
    }
    // Retail reloads the layout/root pane before every FindPaneByName call
    // (no root local), keeping pressure at 3 callee-saved registers.
    if (arg == 9) {
        char* tbl = lbl_eu_8050FF8C;
        func_80124270((*(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10))->FindPaneByName(&tbl[0x184], true), 0);
        func_80124270((*(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10))->FindPaneByName(&tbl[0x18f], true), 1);
    } else {
        char* tbl = lbl_eu_8050FF8C;
        func_80124270((*(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10))->FindPaneByName(&tbl[0x184], true), 1);
        func_80124270((*(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10))->FindPaneByName(&tbl[0x18f], true), 0);
    }
    func_8029967C(self);
    func_802998C8(self);
    if (getItemBoxState((CItemBoxInfo*)self->itemBox) != 0) {
        func_801D4260((void*)self->itemBox, arg);
    }
}
#pragma pop

// Refresh the item-box grid for the current page. When more than one page
// exists, show the page tabs, stamp each tab's enabled state from the page
// counter, reformat the page label and slide the page-cursor pane to the slot
// matching the counter; then restamp all 30 grid cells.
// -O4,s keeps the retail _savegpr_27 frame.
#pragma push
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_8029967C(CMCGetItemBox* self) {
    CMCItemBoxSub* sub = &self->sub_314;
    // Branchy select: retail defaults r0 to 1 and overwrites with the limit.
    u8 count = sub->limit != 0 ? sub->limit : 1;
    if (count > 1) {
        // Declared first so the VEC3 claims the low stack slot (retail layout:
        // vec at sp+8, tab-name buf at sp+0x18).
        nw4r::math::VEC3 pos;
        // String-table base lives only until the tab loop: retail kills the
        // reg there and re-materializes lis/addi for the 0x1b0/0x1b9 uses.
        char* tbl = lbl_eu_8050FF8C;
        // NOTE: retail reloads layout40 before every root-pane access (no
        // cached layout pointer), keeping only 5 callee-saved regs live.
        func_80124270((*(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10))->FindPaneByName(&tbl[0x19a], true), 1);
        for (u8 i = 0; i < 3; i++) {
            char buf[0x20];
            sprintf(buf, &tbl[0x1a1], (int)(u8)i + 1);
            // Sign bit of (unsigned)(i - count): 1 while the tab is within
            // the page range (retail lowers this to srwi, not srawi).
            func_80124270((*(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10))->FindPaneByName(buf, true),
                          ((u32)i - (u32)count) >> 31);
        }
        // Retail computes counter+1 separately at each use (memory reload
        // across the intervening virtual call), not as a shared temp.
        func_80136910(self->layout40, &lbl_eu_8050FF8C[0x1b0], (u8)(sub->counter + 1));
        // Retail evaluates the pane find first (result held in a volatile
        // reg across the pos computation), then builds/stores the VEC3.
        nw4r::lyt::Pane* pagePane =
            self->layout40->GetRootPane()->FindPaneByName(&lbl_eu_8050FF8C[0x1b9], true);
        // Retail computes the pages low word first (subfic right after the
        // finds), then reloads it when building the second scratch record.
        u32 pagesLow = (u32)(u8)(3 - count);
        pos.y = (float&)self->field_30C;
        pos.x = (float&)self->field_308;
        u8 nextPage = (u8)(sub->counter + 1);
        // One shared conversion scratch (retail materializes 0x4330 once).
        union { double d; u32 w[2]; } cvt;
        cvt.w[1] = 0x43300000u;
        cvt.w[0] = (u32)(nextPage - 1) ^ 0x8000u;
        double counterD = cvt.d;
        pos.z = (float&)self->field_310;
        // Retail parks the scale in f2 right after the z-store.
        float scale = lbl_eu_80668BF4;
        cvt.w[0] = pagesLow;
        pos.x += scale * (float)(counterD - lbl_eu_80668BE0) +
                 scale * (float)(cvt.d - lbl_eu_80668BD8);
        copyVEC3((u8*)pagePane + 0x2C, (float*)&pos);
    } else {
        func_80124270((*(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10))->FindPaneByName(&lbl_eu_8050FF8C[0x19a], true), 0);
    }
    for (u8 i = 0; i < 0x1e; i++) {
        u16 icon = (u16)func_80296D54(sub, i);
        CMCItemBoxEntry* entry = func_80296DB0(sub, i);
        func_80298AC8(self, icon, entry, i);
        func_80298FB4(self, icon, entry, i);
        func_80299490(self, (int)(s8)func_80296E00(sub, i), i);
    }
}
#pragma pop

// Refresh the item-name texts in the layout panes and, when the item box is
// open, re-sync the selected entry's name/icon widgets.
// -O4,s keeps the retail stmw r28 frame.
#pragma push
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_802998C8(CMCGetItemBox* self) {
    CMCGetItemBox* self_ = self;
    s8 idx = (s8)(self->field_301 * 10 + self->field_300);
    CMCItemBoxSub* sub = &self_->sub_314;
    func_80136B4C(self->layout40, &lbl_eu_8050FF8C[0x1c7], func_80296E98(sub, (u16)idx), 0);
    func_80136B4C(self->layout40, &lbl_eu_8050FF8C[0x1d0], func_80296FC0(sub, (u16)idx), (u32)self->objAt50);
    if (getItemBoxState((CItemBoxInfo*)self->itemBox) != 0) {
        CMCItemBoxEntry* entry = func_80296DB0(sub, (u16)idx);
        u32 iconId = func_80296D54(sub, (u16)idx);   // held in a reg temp in retail
        func_801D47D4((CItemBoxInfo*)self_->itemBox, (u16)iconId, (u32)entry, 1);
        func_801D4AE0((CItemBoxInfo*)self_->itemBox, 1, func_80296E98(sub, (u16)idx));
    }
}
#pragma pop

// Refresh the cursor widgets / page label.
// -O4,s keeps the retail stmw r29 frame; the page-label buffer and the two
// branch-local positions get disjoint stack slots (no overlay across branches).
#pragma push
#pragma optimize_for_size on
// noinline: retail keeps this out-of-line; without it MWCC inlines the whole
// body into its earlier callers (late-definition inlining defeats auto_inline off).
extern "C" __declspec(noinline) void func_802999B0(CMCGetItemBox* self) {
    char nameBuf[0x20];
    nw4r::math::VEC3 posIf;
    nw4r::math::VEC3 posElse;
    char* tbl;
    nw4r::lyt::Pane* p1;
    nw4r::lyt::Pane* p2;
    s8 page = (s8)self->field_301;
    if (page == -1) {
        tbl = lbl_eu_8050FF8C;
        p1 = (*(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10))->FindPaneByName(&tbl[0x1dc], true);
        p2 = (*(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10))->FindPaneByName(&tbl[0x1e9], true);
        func_80137924(&posIf, p1, p2, *(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10));
        ((CMCCursorWidget*)&self->subObj_88)->setPos(&posIf);
        func_801D216C(&self->subObj_88, 1);
        func_801D216C(&self->subObj_58, 0);
    } else {
        tbl = lbl_eu_8050FF8C;
        sprintf(nameBuf, &tbl[0x161], (int)(s8)self->field_300 + (int)page * 10 + 1);
        p1 = (*(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10))->FindPaneByName(nameBuf, true);
        p2 = (*(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10))->FindPaneByName(&tbl[0x1e9], true);
        func_80137924(&posElse, p1, p2, *(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10));
        ((CMCCursorWidget*)&self->subObj_58)->setPos(&posElse);
        func_801D216C(&self->subObj_88, 0);
        func_801D216C(&self->subObj_58, 1);
    }
}
#pragma pop

// Retail 0x8029C200: async file-load callback. Four files can load
// asynchronously: the main item-box layout archive (fileHandle1), a second
// resource archive (fileHandle2), and two bdat data files (fileHandle3/4).
// Branch 1 builds the whole widget: region-guarded buffer, layout + 2 anim
// transforms, font bind, text stamping, cursor sub-objects and activation;
// the other branches only feed bdat tables / the second accessor.
// -O4,s keeps the retail _savegpr_28/_restgpr_28 prologue.
#pragma push
#pragma optimize_for_size on
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

        // Bind the loaded font's pane into the layout root. Retail keeps the
        // root pane in a callee-saved register only across these calls.
        nw4r::lyt::Pane* rootPane = this->layout40->GetRootPane();
        void* font = CDeviceFont::func_80452C10(1, this->layout40);
        void* fontData = ((CDeviceFontVt9*)font)->getResource();
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
        this->layout40->Animate();

        // Hand the tag processor to the tag pane (+0xF8). Retail reloads the
        // root pane here rather than keeping it live across the text stamping.
        nw4r::lyt::Pane* tagPane =
            this->layout40->GetRootPane()->FindPaneByName(&lbl_eu_8050FF8C[0x1d0], true);
        if (tagPane != 0) *(u32*)((u8*)tagPane + 0xF8) = (u32)this->objAt50;

        func_80136B4C(this->layout40, &lbl_eu_8050FF8C[0x274],
                      (char*)func_80136190(&lbl_eu_8050FF8C[0x269], &lbl_eu_8050FF8C[0xc], 6), 0);

        // Remember the cursor pane's position.
        nw4r::lyt::Pane* curPane =
            this->layout40->GetRootPane()->FindPaneByName(&lbl_eu_8050FF8C[0x281], true);
        copyVEC3((void*)&this->field_308, (float*)((u8*)curPane + 0x2C));

        // Cursor sub-objects: construct on the stack with the arc accessor,
        // copy into the embedded storage, destroy the temp, then run the
        // embedded cursor's slot-2 virtual (update).
        u8 cur07Buf[0x18];
        __ct__CCur07(cur07Buf, this->arcAcc1);
        func_8018B0FC(&this->subObj_58, cur07Buf);
        __dt__6CCur07Fv(cur07Buf, -1);
        ((CMCCursorWidget*)&this->subObj_58)->vf_00();

        u8 cur09Buf[0x18];
        __ct__CCur09(cur09Buf, this->arcAcc1);
        func_8018B0FC(&this->subObj_70, cur09Buf);
        __dt__6CCur09Fv(cur09Buf, -1);
        ((CMCCursorWidget*)&this->subObj_70)->vf_00();

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
        __dt__6CCur16Fv(cur16Buf, -1);
        ((CMCCursorWidget*)&this->subObj_88)->vf_00();

        u8 cur18Buf[0x18];
        __ct__CCur18(cur18Buf, func_801355F4());
        func_8018B0FC(&this->subObj_A0, cur18Buf);
        __dt__6CCur18Fv(cur18Buf, -1);
        ((CMCCursorWidget*)&this->subObj_A0)->vf_00();

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
        // Retail folds the first lookup's offset directly into lis/addi, then
        // caches the table base for the remaining lookups.
        if (getFP__FPCc(&lbl_eu_8050FF8C[0x29f]) == 0) {
            func_8003AA78__5CBdatFUlPv(2, fileData);
        }
        func_8003AA34();
        lbl_eu_80664A18 = getFP__FPCc(&lbl_eu_8050FF8C[0x29f]);
        func_8003AA34();
        lbl_eu_80664A1C = getFP__FPCc(&lbl_eu_8050FF8C[0x2ae]);
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
        lbl_eu_80664A20 = getFP__FPCc(&lbl_eu_8050FF8C[0x119]);
        func_80298A78(this);
        this->fileHandle4 = 0;
        return true;
    }

    return false;
}
#pragma pop

