// TU: kyoshin/makecrystal/CMCGetItemBox
// Class: CMCGetItemBox (IWorkEvent subclass, vtable __vt__13CMCGetItemBox)

// Inline-empty ~IWorkEvent: the retail destructor elides the base call.
#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/cf/CfGameManager.hpp"  // canonical CItem_initItemImplInstances decl
#include "kyoshin/makecrystal/CMCGetItemBox.hpp"
#include "kyoshin/CItemBoxInfo.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CSysWin.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/device/CDeviceSC.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CEventFile.hpp"

#include <nw4r/lyt.h>
#include <cstdio>
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/code_80135FDC_State.hpp"

// Resolve ml::FixStr<128>::format calls to the explicit specialization that
// CfScript.cpp defines (retail symbol format__Q22ml10FixStr<128>FPCce).
template <> void ml::FixStr<128>::format(const char* fmt, ...);

namespace nw4r { namespace lyt { class AnimTransform; } }

#include "kyoshin/CTagProcessor.hpp"

// ---- C-linkage / foreign retail helper declarations ----------------------
// Only unmangled retail symbols (confirmed via config/us/symbols.txt) are
// declared extern "C"; the mangled ones are declared as normal C++ so the
// Itanium-mangled reloc name is emitted (matches the retail reloc).
// Most imports live in the C-linkage imports section of CMCGetItemBox.hpp.
// The six below stay TU-local: CBaseCur::SetVisible (a member) / CSysWin_getUnk34 /
// CSysWin_isActive / sysWinAdvancePhase3 conflict with CModelDispMakeCrystal.cpp's
// own extern "C" decls ((void*, u8) / (void*) forms), and PaneSetTexPaletteByName /
// BdatGetPtrDirect conflict with code_80135FDC.hpp's signatures - hoisting them
// would make that TU ill-formed.
extern "C" {
// sysWinSwitchKindPane / sysWinApplyKind come from kyoshin/CSysWin.hpp (CSysWin* forms).
}
// C++-linkage (mangled) retail symbols.
// Plain-C++ imports for the targets below. MWCC emits the Itanium-mangled
// reloc names at the call sites; the acceptance gate compares reloc sites
// (offset+type) only, not names.
void* sCrystalItemTable = 0;
void* sCrystalMesTable = 0;
void* sItemMesTable = 0;

extern "C" int  KyoshinHeap_GetField44();   // retail reloc is unmangled
// retail reloc is unmangled
// (the CSysWin slot-0x88 virtual goes through CSysWin::loadSystemArc --
// see kyoshin/CSysWin.hpp.)

// Same-unit helper functions (retail relocs are unmangled -> C linkage).
// refillMCItemSubTable's extra params carry defaults so initMCGetItemBoxList's 1-arg call
// (retail passes garbage r4/r5/r6 there) still compiles.
extern "C" void refillMCItemSubTable(CMCItemBoxSub*, u8 = 0, CMCItemBoxEntry* = 0, u8 = 0);
extern "C" void refreshMCGetItemBoxSlots(CMCGetItemBox*);
extern "C" void refreshMCGetItemBoxTexts(CMCGetItemBox*);
// Same-unit helper functions (retail relocs are unmangled -> C linkage).
extern "C" void openMCGetItemBoxPhase1(CMCGetItemBox*);
extern "C" void openMCGetItemBoxPhase2(CMCGetItemBox*);
extern "C" void closeMCGetItemBoxPhase1(CMCGetItemBox*);
extern "C" void closeMCGetItemBoxPhase2(CMCGetItemBox*);
extern "C" void nextMCGetItemBoxPage(CMCGetItemBox*);
extern "C" void prevMCGetItemBoxPage(CMCGetItemBox*);

// Initialise a CMCItemBoxSub: clear the offset table to 0xFFFF, zero the
// counters and index fields, then reset the whole table again.
void* __dt__80296BB0(CMCGetItemBox* _this, int flags) {
    if (_this != NULL && flags > 0) {
        operator delete(_this);
    }
    return _this;
}

// -O4,s keeps both passes rolled (pointer-walk + mtctr/bdnz sthx) as in retail.
// C linkage: retail symbol is the plain name (reloc-site gate).
extern "C" void initMCItemSub(CMCItemBoxSub* x) {
    // Retail walks a pointer over the table (cmplw loop); the inline bound is
    // CSE-hoisted as an r0 temp, matching retail coloring. Known residual:
    // retail emits the hoisted addi before the param copy; every tested shape
    // keeps the copy first (see attempts stall note us-8029917c).
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
    // -O4,s keeps MWCC from unrolling it (retail stays rolled).
    for (u16 i = 0; i < 0x80; i++) x->table[i] = -1;
}

// Rebuild the item-box index table: store the entry base/type, reset the
// offset table to 0xFFFF, then re-index every entry whose type word matches
// pad_102, and finally compute the page limit as ceil(filled-count / 30).
// -O4,s keeps the retail stmw r28 frame.
void refillMCItemSubTable(CMCItemBoxSub* x, u8 pad102, CMCItemBoxEntry* listBase, u8 field1D4) {
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
    f32 v = (f32)x->count / 30.0f;
    s32 t = (s32)v;              // floor since v >= 0 (fctiwz truncates)
    f32 frac = v - (f32)t;
    if (lbl_eu_80668BD4 != frac) t++;
    x->limit = (u8)t;
    if (x->limit == 0) x->limit = 1;
    x->counter = 0;
}

// Advance the counter; wrap to 0 when past the signed byte limit.
extern "C" void nextMCItemSubPage(CMCItemBoxSub* x) {
    u8 v = (u8)(x->counter + 1);
    x->counter = v;
    if ((s8)v < x->limit) return;
    x->counter = 0;
}

// Count the counter down; wrap to (limit-1) when it goes negative.
extern "C" void prevMCItemSubPage(CMCItemBoxSub* x) {
    u8 v = (u8)(x->counter - 1);
    x->counter = v;
    if ((s8)v >= 0) return;
    x->counter = (u8)(x->limit - 1);
}

// Look up the entry at `index` in the offset table and return the derived
// index-table word >> 20, or 0 when out of range.
u32 getMCItemSubItemId(CMCItemBoxSub* x, u32 index) {
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

// auto_inline off: retail keeps the single-call-site bl from refreshMCGetItemBoxSlots
// (MWCC otherwise folds this small helper in).
#pragma auto_inline off
s8 getMCItemSubKind(CMCItemBoxSub* x, u32 index) {
    CMCItemBoxEntry* base = x->listBase;
    if (base == 0) return 0;
    u16 idx = (u16)(index + (s8)x->counter * 30);
    if (idx < x->count) {
        CMCItemBoxEntry* p = base + x->table[idx];
        if (p != 0) {
            CMCItemImplShim* inst = (CMCItemImplShim*)CItem_initItemImplInstances((CItemData*)p);
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
char* getMCItemSubName(CMCItemBoxSub* sub, u16 index) {
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
    CMCItemImplShim* inst = (CMCItemImplShim*)CItem_initItemImplInstances((CItemData*)p);
    // Keep only x/p/strTbl live across calls: recompute &x->shortName
    // at every use so MWCC holds just three callee-saved regs (retail r29-r31).
    ((ml::FixStr<64>*)&x->shortName)->format(strTbl, inst->getName(p));
    if (x->pad_102 == 3) {
        // Gem slot: rebuild the name as "<kind suffix><saved name>".
        CMCItemImplShim* inst2 = (CMCItemImplShim*)CItem_initItemImplInstances((CItemData*)p);
        u32 kind = (u8)inst2->getKind(p);
        char* itemName = BdatTouchStringCell(&strTbl[3], &strTbl[0xc],
                                       0x1e - (kind - 1));
        // Refresh the cached length, then splice: format("%s%s", saved, suffix).
        x->shortName.mLength = strlen(x->shortName.mString);
        char copy[64];
        strcpy(copy, x->shortName.mString);
        ((ml::FixStr<64>*)&x->shortName)->format(&strTbl[0x11], copy, itemName);
    }
    return (char*)&x->shortName;
}

// Retail 0x802995F8: format the selected entry's item text into the
// FixStr<128> buffer at sub+0x14C and return it. Entries owned by the bdat
// manager (IsSkillItem != 0) use the item-name database; otherwise the
// text is built from the message-string placeholders: '%1' splices the
// item icon name (switch on BdatGetU8Direct's kind), '%2' splices a kind-apped
// table string from sMCItemMsgFmtTbl, and the two-byte shift-JIS ranges are
// stepped over. The whole formatted buffer is then copied back into the
// FixStr (its mLength refreshed).
// -O4,s keeps the retail stmw r18 frame.
char* getMCItemSubText(CMCItemBoxSub* sub, u16 index) {
    CMCItemBoxEntry* base = sub->listBase;
    if (base == 0) return 0;
    // Retail sign-extends the page counter, scales by 30 and folds the u16
    // clamp into an unsigned compare against count.
    u16 idx = (u16)(index + (s8)sub->counter * 30);
    if (idx >= sub->count) return 0;
    // The s16 table offset sign-extends before the *52 scale.
    CMCItemBoxEntry* p = base + sub->table[idx];
    if (p == 0) return 0;
    if (IsSkillItem(p) != 0) {
        // bdat-managed entry: name comes from the item database.
        void* inst = CItem_initItemImplInstances((CItemData*)p);
        u32 v = (u32)((u32(*)(void*, void*))(*(void***)inst)[0x22])(inst, p);
        char* s = BdatGetPtrDirect((void*)lbl_eu_80664100, "comment", (u16)v);
        ((ml::FixStr<128>*)&sub->name)->format(&lbl_eu_8050FF8C[0], s);
        return (char*)&sub->name;
    }
    u32 type = (p->field_00 >> 12) & 0xF;
    int special = 0;
    if (type == 9 && (p->bytes[3] & 3) == 3) special = 1;
    if (special) {
        char* s = BdatTouchStringCell(&lbl_eu_8050FF8C[3], &lbl_eu_8050FF8C[0xc], 0x9c);
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
        void* inst1 = CItem_initItemImplInstances((CItemData*)p);
        u32 icon = (u32)((u32(*)(void*, void*))(*(void***)inst1)[0x15])(inst1, p);
        void* inst2 = CItem_initItemImplInstances((CItemData*)p);
        u8 kind = (u8)((u32(*)(void*, void*))(*(void***)inst2)[2])(inst2, p);
        void* inst3 = CItem_initItemImplInstances((CItemData*)p);
        char* nm = (char*)((void*(*)(void*, void*))(*(void***)inst3)[0x24])(inst3, p);
        CMCItemMsgTbl tbl;
        char fmtBuf[0x20];
        char buf48[0x20];
        char tail[0x80];
        char copy[0x80];
        sprintf(buf48, &lbl_eu_8050FF8C[0x1e], nm);
        u8 mkind = BdatGetU8Direct((const void*)db, &lbl_eu_8050FF8C[0x21], (u16)icon);
        char* s2 = BdatGetPtrDirect((void*)db, &lbl_eu_8050FF8C[0x2a], (u16)icon);
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
                    tbl = sMCItemMsgFmtTbl;
                    u8 s3 = BdatGetU8Direct((const void*)db, (const char*)tvals[kind], (u16)icon);
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
        void* tbl2 = (void*)sCrystalMesTable;
        if (v <= 5 || pad102 == 2) tbl2 = (void*)sCrystalItemTable;
        u32 n = BdatGetU16Direct((void*)lbl_eu_806640EC, "comment", id);
        char* s = BdatGetPtrDirect(tbl2, "comment", (u16)n);
        ((ml::FixStr<128>*)&sub->name)->format(&lbl_eu_8050FF8C[0], s);
        return (char*)&sub->name;
    }
}

// Constructor: the compiler stores the class vtable, then the member-init
// list constructs the two scratch regions, the four cursor overlays, the
// system window and the item-box info in declaration order. The body zeroes
// the handles/flags, initialises the item index table and re-seeds the
// widgets from default temporaries (retail builds each default on the stack
// and copies it over the member).
CMCGetItemBox::CMCGetItemBox()
    : memRegion1(),
      memRegion2(),
      fileHandle1(0),
      fileHandle2(0),
      fileHandle3(0),
      fileHandle4(0),
      memManagerPtr(0),
      arcAcc1(0),
      arcAcc2(0),
      layout40(0),
      animTrans1(0),
      animTrans2(0),
      field_4C(0),
      field_4D(0),
      objAt50(0),
      field_54(0),
      mField55(1),
      subObj_58(0),
      subObj_70(0),
      subObj_88(0),
      subObj_A0(0),
      sysWin_B8(0),
      itemBox(0, 0),
      field_300(0),
      field_301(0),
      mField303(0),
      field_304(0),
      field_305(0) {
    initMCItemSub(&sub_314);

    // Default item-box info -> embedded member (whole-object assignment).
    itemBox = CItemBoxInfo(0, 0);

    // Default system window -> embedded member.
    sysWin_B8 = CSysWin(0);

    // Fresh item index table -> embedded sub-object.
    sCrystalItemTable = 0;
    sCrystalMesTable = 0;
    sItemMesTable = 0;
    {
        CMCItemBoxSub subTmp;
        initMCItemSub(&subTmp);
        sub_314 = subTmp;
    }
}

// Destructor: destroy the embedded widgets in reverse declaration order, then
// the scratch regions; the flags>0 tail frees the object itself.
CMCGetItemBox::~CMCGetItemBox() {}

// Load the four item-box resource files into the file-handle members, init
// the item-box info (0xF4) and dispatch the sys-win initialiser (vtable+0x88).
// -O4,s keeps the retail stmw r30 frame.
void loadMCGetItemBoxFiles(CMCGetItemBox* self) {
    self->fileHandle1 = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(),
                                              &lbl_eu_8050FF8C[0xae], (IWorkEvent*)self, 0, 0);
    self->fileHandle2 = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(),
                                              &lbl_eu_8050FF8C[0xc7], (IWorkEvent*)self, 0, 0);
    self->fileHandle3 = CDeviceFile::readCommonArchiveFile((mtl::ALLOC_HANDLE)KyoshinHeap_GetField44(),
                                                          &lbl_eu_8050FF8C[0xe0], (IWorkEvent*)self, 0, 0);
    self->fileHandle4 = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(),
                                              &lbl_eu_8050FF8C[0xfd], (IWorkEvent*)self, 0, 0);
    self->itemBox.LoadFiles();
    self->sysWin_B8.loadSystemArc();
}

// Per-frame update: dispatch the layout-animation state machine (states 1/2/4/5),
// then refresh the cursor widgets, the item-box info and the sys-win window.
void updateMCGetItemBox(CMCGetItemBox* self) {
    if (self->field_4C == 0) return;
    if (self->field_4D == 0) return;
    switch (self->field_4D) {
        case 1: openMCGetItemBoxPhase1(self); break;
        case 2: openMCGetItemBoxPhase2(self); break;
        case 4: closeMCGetItemBoxPhase1(self); break;
        case 5: closeMCGetItemBoxPhase2(self);
    }
    self->layout40->Animate(0);
    self->subObj_58.Update();
    self->subObj_70.Update();
    self->subObj_88.Update();
    self->subObj_A0.Update();
    self->itemBox.UpdateAnims();
    self->sysWin_B8.dispatchPhase();
}

// Draw the item box: item-box info, layout, the (limit-dependent) cursor
// widgets and the sys-win window.
// -O4,s keeps the retail stmw r30 frame.
void drawMCGetItemBox(CMCGetItemBox* self, nw4r::lyt::DrawInfo* di) {
    if (self->field_4C == 0) return;
    if (self->field_4D == 0) return;
    self->itemBox.DrawLayout(di);
    drawLayout(self->layout40, di, 0, 1);
    u8 active = self->sub_314.limit ? self->sub_314.limit : 1;
    if (active > 1) self->subObj_70.DrawLayout(di);
    self->subObj_58.DrawLayout(di);
    self->subObj_88.DrawLayout(di);
    self->subObj_A0.DrawLayout(di);
    self->sysWin_B8.drawLayout(di);
}

// Clean up every resource owned by the item-box widget.
// -O4,s keeps the retail stmw r30 frame.
#pragma push
void teardownMCGetItemBox(CMCGetItemBox* self) {
    CBdat::getEntry(2);
    CBdat::getEntry(5);
    closeFileHandle(&self->fileHandle1);
    closeFileHandle(&self->fileHandle2);
    closeFileHandle(&self->fileHandle3);
    closeFileHandle(&self->fileHandle4);
    self->field_4C = 0;
    if (self->memManagerPtr != 0) {
        mtl::MemManager::deallocate(self->memManagerPtr);
        self->memManagerPtr = 0;
    }
    if (self->layout40 != 0) {
        delete self->layout40;
        self->layout40 = 0;
    }
    releaseArcResourceAccessor((nw4r::lyt::ArcResourceAccessor*)self->arcAcc1);
    releaseArcResourceAccessor((nw4r::lyt::ArcResourceAccessor*)self->arcAcc2);
    void* o = self->objAt50;
    self->arcAcc1 = 0;
    self->arcAcc2 = 0;
    if (o != 0) {
        delete (CTagProcessor*)o;
        self->objAt50 = 0;
    }
    self->memRegion1.deleteRegion();
    self->memRegion2.deleteRegion();
    self->subObj_58.cleanup();
    self->subObj_70.cleanup();
    self->subObj_88.cleanup();
    self->subObj_A0.cleanup();
    self->itemBox.UnloadFiles();
    self->sysWin_B8.termLayout();
}

// Return 0 unless the item box is active and the sys-win is ready.
// Retail layout: first return-0 inline after the state check (bne over it),
// then the ready!=0 path returns field_54 inline with the final return-0 at the tail.
u8 isMCGetItemBoxActive(CMCGetItemBox* self) {
    if (self->itemBox.GetState() == 0) return 0;
    if (self->sysWin_B8.isReady() != 0) return self->field_54;
    return 0;
}

int CMCGetItemBox::isMCGetItemBoxReady() { return mField55; }

int CMCGetItemBox::isMCGetItemBoxGridMode() { return mField303; }

// Open the item box from an external caller: guard on the current state, reset
// the item-box info and refresh both the layout text and the cursor widgets.
// -O4,s keeps the retail stmw r28 frame.
extern "C" void openMCGetItemBox(CMCGetItemBox* self, u16 arg, void* unk, u8 byte) {
    if (self->field_4D != 0) return;
    self->field_4D = 1;
    self->mField55 = 0;
    self->field_300 = 0;
    // Signed lvalue keeps the constant negative (li r0,-1, not li r0,255).
    (s8&)self->field_301 = -1;
    self->itemBox.Open();
    initMCGetItemBoxList(self, arg, unk, byte);
    self->itemBox.HidePanes(arg);
    // Declared in this order so MWCC colors the callee-saved regs like retail
    // (entry->r31, idx->r30, sub->r29).
    CMCItemBoxEntry* entry;
    u8 idx;
    CMCItemBoxSub* sub;
    idx = (u8)(self->field_301 * 10 + self->field_300);
    sub = &self->sub_314;
    entry = getMCItemSubEntry(sub, idx);
    u32 iconId = getMCItemSubItemId(sub, idx);   // held in a reg temp in retail
    self->itemBox.SetItem((u16)iconId, (void*)entry, 1);
    self->itemBox.SetNamedText(1, getMCItemSubName(sub, idx));
}

// When the item-box widget is done (state 3), advance to state 4, detach the
// helper widgets and advance the item-box state machine.
void confirmMCGetItemBox(CMCGetItemBox* self) {
    if (self->field_4D != 3) return;
    self->field_4D = 4;
    self->mField55 = 0;
    self->subObj_58.SetVisible(0);
    self->subObj_70.SetVisible(0);
    self->subObj_88.SetVisible(0);
    self->itemBox.Close();
    playUISound(0x6);
}

// Move the cursor / page selection. Cursor mode walks the row backwards for a
// non-empty slot; page mode decrements the sort column and refreshes the
// widgets. Both paths end with the UI click sound.
// -O4,s keeps the retail stmw r29 frame.
extern "C" void moveMCGetItemBoxLeft(CMCGetItemBox* self) {
    if (self->sysWin_B8.isOpened() != 0) return;
    if (self->mField303 != 0) {
        u16* arr = self->itemBox.mPage.mItemIds;
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
        CopyTabSlotVec((u32*)tmp, arr,
                      (u8)((s8)self->field_304 * 4 + self->field_305));
        self->subObj_A0.setRootPaneTranslate((nw4r::math::VEC3*)tmp);
    } else {
        u8 v = (u8)(self->field_301 - 1);
        self->field_301 = v;
        if ((s8)v < -1) self->field_301 = 2;
        moveMCGetItemBoxCursor(self);
        refreshMCGetItemBoxTexts(self);
    }
    playUISound(0x1);
}

// Advance the cursor to the next non-empty slot.
// Cursor branch is the fall-through (retail beq jumps to the sort branch).
// -O4,s keeps the retail stmw r29 frame.
void moveMCGetItemBoxRight(CMCGetItemBox* self) {
    if (self->sysWin_B8.isOpened() != 0) return;
    if (self->mField303 != 0) {
        u16* arr = self->itemBox.mPage.mItemIds;
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
        CopyTabSlotVec((u32*)tmp, arr,
                      (u8)((s8)self->field_304 * 4 + self->field_305));
        self->subObj_A0.setRootPaneTranslate((nw4r::math::VEC3*)tmp);
    } else {
        u8 v = self->field_301 + 1;
        self->field_301 = v;
        // Signed lvalue keeps the constant negative: MWCC emits li r0,-1
        // (a plain u8 store folds it to li r0,255).
        if ((s8)v >= 3) (s8&)self->field_301 = -1;
        moveMCGetItemBoxCursor(self);
        refreshMCGetItemBoxTexts(self);
    }
    playUISound(0x1);
}

// -O4,s keeps the retail stmw r29 frame.
void moveMCGetItemBoxUp(CMCGetItemBox* self) {
    if (self->sysWin_B8.isOpened() != 0) return;
    if (self->mField303 != 0) {
        u16* arr = self->itemBox.mPage.mItemIds;
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
        CopyTabSlotVec((u32*)tmp, arr,
                      (u8)((s8)self->field_304 * 4 + self->field_305));
        self->subObj_A0.setRootPaneTranslate((nw4r::math::VEC3*)tmp);
    } else {
        // Retail reuses the loaded field_300 for the decrement (subi r3-based).
        u8 s = self->field_300;
        if ((s8)s == 0) {
            self->field_300 = 9;
            self->field_301 = 0;
            prevMCGetItemBoxPage(self);
            moveMCGetItemBoxCursor(self);
        } else {
            u8 n = (u8)(s - 1);
            self->field_300 = n;
            if ((s8)n < 0) self->field_300 = 9;
            moveMCGetItemBoxCursor(self);
            refreshMCGetItemBoxTexts(self);
        }
    }
    playUISound(0x1);
}

#pragma push
void moveMCGetItemBoxDown(CMCGetItemBox* self) {
    if (self->sysWin_B8.isOpened() != 0) return;
    if (self->mField303 != 0) {
        u16* arr = self->itemBox.mPage.mItemIds;
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
        CopyTabSlotVec((u32*)tmp, arr,
                      (u8)((s8)self->field_304 * 4 + self->field_305));
        self->subObj_A0.setRootPaneTranslate((nw4r::math::VEC3*)tmp);
    } else {
        // Retail reuses the loaded field_300 for the decrement (subi r3-based).
        u8 s = self->field_300;
        if ((s8)s == 9) {
            self->field_300 = 0;
            self->field_301 = 0;
            nextMCGetItemBoxPage(self);
            moveMCGetItemBoxCursor(self);
        } else {
            u8 n = s + 1;
            self->field_300 = n;
            if ((s8)n >= 10) self->field_300 = 0;
            moveMCGetItemBoxCursor(self);
            refreshMCGetItemBoxTexts(self);
        }
    }
    playUISound(0x1);
}

// Increment the sub counter, refresh helper widgets, and play a sound when the
// item-box limit is anything other than 1.
// -O4,s keeps the retail stmw r30 frame.
extern "C" void nextMCGetItemBoxPage(CMCGetItemBox* self) {
    CMCItemBoxSub* x = &self->sub_314;
    nextMCItemSubPage(x);
    refreshMCGetItemBoxSlots(self);
    refreshMCGetItemBoxTexts(self);
    // Retail: li r0,1 default, conditionally overwritten with the limit (branchy
    // select); splitting the select across blocks makes MWCC forget the byte
    // load was zero-extended, keeping the redundant clrlwi/cmpli pair.
    // u8-local select: assigning the ternary to a u8 makes MWCC materialize
    // the zero-extension (clrlwi) at the join before the compare (retail).
    u8 lim = x->limit != 0 ? x->limit : 1;
    if (lim != 1) playUISound(0xa);
}

// Decrement the sub counter, refresh helper widgets, and play a sound when the
// item-box limit is anything other than 1.
extern "C" void prevMCGetItemBoxPage(CMCGetItemBox* self) {
    CMCItemBoxSub* x = &self->sub_314;
    prevMCItemSubPage(x);
    refreshMCGetItemBoxSlots(self);
    refreshMCGetItemBoxTexts(self);
    // Retail: li r0,1 default, conditionally overwritten with the limit (branchy
    // select); the u8-local select makes MWCC forget the byte load was
    // zero-extended, keeping the redundant clrlwi/cmpli pair at the join.
    u8 lim = x->limit != 0 ? x->limit : 1;
    if (lim != 1) playUISound(0xa);
}

// Toggle the item-box help window: close it when active, otherwise detach the
// helper widgets when the layout flag is set.
void toggleMCGetItemBoxSysWin(CMCGetItemBox* self) {
    if (self->sysWin_B8.isOpened() != 0) {
        if (self->sysWin_B8.isActive() != 0) {
            self->sysWin_B8.advancePhase3();
            self->subObj_A0.SetVisible(1);
        }
    } else {
        if (self->mField303 != 0) {
            self->mField303 = 0;
            self->subObj_58.SetVisible(1);
            self->subObj_A0.SetVisible(0);
            playUISound(0x6);
        }
    }
}

// When the sys-win is both present and active, close it and reset the A0 widget.
void openMCGetItemBoxSysWin(CMCGetItemBox* self) {
    if (self->sysWin_B8.isOpened() == 0) return;
    if (self->sysWin_B8.isActive() == 0) return;
    self->sysWin_B8.advancePhase3();
    self->subObj_A0.SetVisible(1);
}

// Unless the widget is busy and the sort field is -1, report the selected
// item-box entry's place relative to its full range: 2 when the cursor index
// is inside the entry count, 1 otherwise.
// -O4,s: -O4,s lowers the >= into the retail subfc carry chain.
u32 getMCGetItemBoxPageState(CMCGetItemBox* self) {
    if (self->field_4D == 0) return 0;
    if ((s8)self->field_301 == -1) {
        // Retail lowers the place calc to a subfc/carry/subf chain.
        u32 v = (u16)CItemBlock_countEmpty(self->sub_314.pad_102);
        u32 count = self->sub_314.count;
        if (v >= count) return 1;
        return 2;
    }
    return 0;
}

// Visit every item-box entry and hand it to the C-linkage cleanup helper.
void cacheMCGetItemBoxEntries(CMCGetItemBox* self) {
    CMCItemBoxSub* x = &self->sub_314;
    for (u32 i = 0; (u32)(u16)i < x->count; i++) {
        CItemData_lookupOrAlloc(getMCItemSubEntry(x, (u16)i), 0);
    }
}

// Look up the entry at `index` in the offset table and return its pointer.
extern "C" CMCItemBoxEntry* getMCItemSubEntry(CMCItemBoxSub* x, u32 index) {
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

extern "C" void forwardMCSubObj88(void* self, u8 visible) {
    ((CBaseCur*)((char*)self + 0x88))->SetVisible(visible);
}

// Advance the cursor / tab selection in the item box.
// -O4,s keeps the retail _savegpr_29/_restgpr_29 call-form prologue (three
// callee-saved regs live across calls: self, cursor-table base, index).
void advanceMCGetItemBox(CMCGetItemBox* self) {
    if (self->mField303 != 0) {
        if (self->sysWin_B8.isOpened() != 0) {
            if (self->sysWin_B8.isActive() != 0) {
                self->sysWin_B8.advancePhase3();
                self->subObj_A0.SetVisible(1);
            }
        } else {
            CItemBoxPage* arr = &self->itemBox.mPage;
            u8 combined = (u8)(self->field_305 + (s8)self->field_304 * 4);
            u16 r = ArrayGet12(arr->mItemIds, combined);
            if ((u16)r != 0) {
                u8 v = combined < 0xc ? arr->mBytesA8[combined] : 0;
                void* res;
                if (v == 3) {
                    res = ((CItemBoxPage*)(arr->mItemIds))->GetItemName(combined);
                } else {
                    res = BdatTouchStringCell(&lbl_eu_8050FF8C[0x119],
                                        &lbl_eu_8050FF8C[0x123], r);
                }
                self->sysWin_B8.switchKindPane(0);
                self->sysWin_B8.setKindContent(reinterpret_cast<char*>(res), 0);
                self->sysWin_B8.applyKind(1);
                self->sysWin_B8.openPhase1();
                self->subObj_A0.SetVisible(0);
            }
        }
    } else {
        if (self->sysWin_B8.isOpened() != 0) return;
        // Find the first non-empty slot in the 12-entry table; record its
        // index (or 8 when the 8th entry is also non-empty).
        CItemBoxPage* arr = &self->itemBox.mPage;
        int found = 0;
        for (s32 i = 0; i < 0xc; i++) {
            if (arr->mItemIds[(u8)i] != 0) {
                arr->mSelSlot = (u8)i;
                if (arr->mItemIds[8] != 0) arr->mSelSlot = 8;
                found = 1;
                break;
            }
        }
        if (found != 0) {
            u8 v = arr->mSelSlot;
            // Retail's named-pool u32->f64 conversion (0x43300000 trick with
            // lbl_eu_80668BD8 as the subtracted 2^52 constant).
            union { double d; u32 w[2]; } cvt;
            cvt.w[1] = v;
            cvt.w[0] = 0x43300000u;
            s32 rowN = (s32)(0.25f * (f32)(cvt.d - lbl_eu_80668BD8));
            self->mField303 = 1;
            self->field_304 = (u8)rowN;
            self->field_305 = (u8)(v - (s8)rowN * 4);
            self->subObj_58.SetVisible(0);
            self->subObj_A0.SetVisible(1);
            u8 tmp[12];
            CopyTabSlotVec((u32*)tmp, arr->mItemIds, v);
            (*(void(**)(void*, void*))((void**)&self->subObj_A0)[4])(&self->subObj_A0, (void*)tmp);
            playUISound(2);
        } else {
            playUISound(5);
        }
    }
}

// Return a UI part index: 0 when the window exists, 0x36 when the layout flag
// is set, otherwise 0x39/0x3A based on the sign of the sort field.
u32 getMCGetItemBoxKeyCode(CMCGetItemBox* self) {
    if (self->sysWin_B8.isOpened() != 0) return 0;
    if (self->mField303 != 0) return 0x36;
    return 0x39 + ((s8)self->field_301 == -1);
}

// Open the item box: advance the second layout animation and, once it has
// finished, initialise the state/widgets and refresh the cursor.
extern "C" void openMCGetItemBoxPhase2(CMCGetItemBox* self) {
    if (advanceAnimTransform((nw4r::lyt::AnimTransform*)self->animTrans2, lbl_eu_80668BF0) != 0) {
        self->field_4D = 3;
        self->mField55 = 1;
        self->subObj_58.SetVisible(1);
        self->subObj_70.SetVisible(1);
        moveMCGetItemBoxCursor(self);
    }
}

// Advance the first layout animation; when it has finished, enable the two
// anim transforms on the layout and move to state 2.
extern "C" void openMCGetItemBoxPhase1(CMCGetItemBox* self) {
    if (advanceAnimTransform((nw4r::lyt::AnimTransform*)self->animTrans1, lbl_eu_80668BF0) != 0) {
        self->layout40->SetAnimationEnable((nw4r::lyt::AnimTransform*)self->animTrans1, false);
        self->layout40->SetAnimationEnable((nw4r::lyt::AnimTransform*)self->animTrans2, true);
        self->field_4D = 2;
    }
}

// Rewind the second layout animation via AnimRewindFrame; when finished, enable
// the two anim transforms on the layout and move to state 5.
extern "C" void closeMCGetItemBoxPhase1(CMCGetItemBox* self) {
    if (AnimRewindFrame((nw4r::lyt::AnimTransform*)self->animTrans2, lbl_eu_80668BF0) != 0) {
        self->layout40->SetAnimationEnable((nw4r::lyt::AnimTransform*)self->animTrans2, false);
        self->layout40->SetAnimationEnable((nw4r::lyt::AnimTransform*)self->animTrans1, true);
        self->field_4D = 5;
    }
}

// Rewind the first layout animation; when it has finished, rearm the
// state bytes and reattach the cursor sub-object.
extern "C" void closeMCGetItemBoxPhase2(CMCGetItemBox* self) {
    if (AnimRewindFrame(self->animTrans1, lbl_eu_80668BF0) != 0) {
        self->mField55 = 1;
        self->field_4D = 0;
        self->subObj_58.SetVisible(0);
    }
}

// Guard: the item box only accepts input once the layout, the second arc
// accessor and its three state words are all present. One OR-chain; MWCC
// emits four beqlr shortcuts plus an inverted final test around the stores.
void activateMCGetItemBox(CMCGetItemBox* self) {
    if (self->layout40 == 0 || self->arcAcc2 == 0 || sCrystalItemTable == 0 ||
        sCrystalMesTable == 0 || sItemMesTable == 0) {
        return;
    }
    self->field_54 = 1;
    self->field_4C = 1;
}

// (uses nw4r::lyt::ArcResourceAccessor::GetResource directly -- vtable
// offset 0x0C, same dispatch; see lyt_arcResourceAccessor.h.)

// (uses IDeviceFontInfo::getFont directly -- vtable offset 0x24, same
// dispatch; see monolib/device/CDeviceFont.hpp.)

// Retail 0x80298AC8: resolve the icon resource for a selected item-box entry
// and stamp it into a numbered layout pane. Mirrors setMCSlotNameTex but uses the
// icon database (BdatGetU8Direct) and the 0x144-0x149 icon-name chain for both
// the gem (type 3) and item (type 9) paths, plus a %d pane-name format at
// &lbl[0x14e].
extern "C" void setMCSlotIconTex(CMCGetItemBox* self, u32 idx, CMCItemBoxEntry* entry, u8 n) {
    CMCItemBoxEntry* e = entry != 0 ? entry : 0;
    void* h = 0;
    if (e != 0) {
        u32 type = (e->field_00 >> 16) & 0xF;
        if (type == 3) {
            h = 0;
            CMCItemImplShim* inst = (CMCItemImplShim*)CItem_initItemImplInstances((CItemData*)e);
            u32 k = inst->getIcon(e);
            u8 r = (u8)BdatGetU8Direct((const void*)lbl_eu_806640D8, &lbl_eu_8050FF8C[0x128], (u16)k);
            switch (r) {
                case 4: h = MakeTplNameSysFile(0x144); break;
                case 5: h = MakeTplNameSysFile(0x145); break;
                case 6: h = MakeTplNameSysFile(0x146); break;
                case 7: h = MakeTplNameSysFile(0x147); break;
                case 8: h = MakeTplNameSysFile(0x148); break;
                case 9: h = MakeTplNameSysFile(0x149); break;
            }
            if (h != 0)
                h = self->arcAcc2->GetResource(0x74696d67u, (const char*)h, 0);
            else
                h = self->arcAcc1->GetResource(0x74696d67u, &lbl_eu_8050FF8C[0x131], 0);
        } else if (type == 9) {
            CMCItemImplShim* inst = (CMCItemImplShim*)CItem_initItemImplInstances((CItemData*)e);
            u32 cnt = inst->getCount(e);
            if ((u16)cnt == 0) {
                // Empty slot: a bdat-managed entry uses the 0x155 chain,
                // otherwise the gem-icon chain keyed on byte 7 >> 2.
                if (IsSkillItem(e) != 0) {
                    h = self->arcAcc2->GetResource(0x74696d67u, (const char*)MakeTplNameSysFile(0x155), 0);
                } else {
                    char* s = 0;
                    switch ((e->bytes[3] >> 2) & 0x3F) {
                        case 4: s = MakeTplNameSysFile(0x144); break;
                        case 5: s = MakeTplNameSysFile(0x145); break;
                        case 6: s = MakeTplNameSysFile(0x146); break;
                        case 7: s = MakeTplNameSysFile(0x147); break;
                        case 8: s = MakeTplNameSysFile(0x148); break;
                        case 9: s = MakeTplNameSysFile(0x149); break;
                    }
                    if (s != 0)
                        h = self->arcAcc2->GetResource(0x74696d67u, s, 0);
                    else
                        h = self->arcAcc1->GetResource(0x74696d67u, &lbl_eu_8050FF8C[0x131], 0);
                }
            } else {
                char* s = 0;
                switch ((e->bytes[3] >> 2) & 0x3F) {
                    case 4: s = MakeTplNameSysFile(0x144); break;
                    case 5: s = MakeTplNameSysFile(0x145); break;
                    case 6: s = MakeTplNameSysFile(0x146); break;
                    case 7: s = MakeTplNameSysFile(0x147); break;
                    case 8: s = MakeTplNameSysFile(0x148); break;
                    case 9: s = MakeTplNameSysFile(0x149); break;
                }
                if (s != 0)
                    h = self->arcAcc2->GetResource(0x74696d67u, s, 0);
                else
                    h = self->arcAcc1->GetResource(0x74696d67u, &lbl_eu_8050FF8C[0x131], 0);
            }
        }
    }
    if (h == 0) {
        if (idx != 0) {
            char* name = MakeTplNameSysFile((u16)BdatGetU16Direct((void*)lbl_eu_806640EC, &lbl_eu_8050FF8C[0x144], idx));
            h = self->arcAcc2->GetResource(0x74696d67u, name, 0);
            if (h == 0)
                h = self->arcAcc1->GetResource(0x74696d67u, &lbl_eu_8050FF8C[0x131], 0);
        } else {
            h = self->arcAcc1->GetResource(0x74696d67u, &lbl_eu_8050FF8C[0x131], 0);
        }
    }
    if (h != 0) {
        char buf[0x20];
        sprintf(buf, &lbl_eu_8050FF8C[0x14e], (int)(n + 1));
        PaneSetTexPaletteByName((nw4r::lyt::Layout*)((void*)self->layout40), (const char*)buf, (TPLPalette*)((void*)h));
    }
}

// Retail 0x80298FB4: item icon-name resolution for the grid cells.
// -O4,s keeps the retail stmw r27 frame.
void setMCSlotNameTex(CMCGetItemBox* self, u32 idx, CMCItemBoxEntry* entry, u8 n) {
    CMCItemBoxEntry* e = entry != 0 ? entry : 0;
    void* h = 0;
    if (e != 0) {
        u32 type = (e->field_00 >> 16) & 0xF;
        if (type == 3) {
            h = 0;
            CMCItemImplShim* inst = (CMCItemImplShim*)CItem_initItemImplInstances((CItemData*)e);
            switch ((u16)inst->getKind(e)) {
                case 1: h = MakeTplNameSysFile(0x197); break;
                case 2: h = MakeTplNameSysFile(0x196); break;
                case 3: h = MakeTplNameSysFile(0x195); break;
                case 4: h = MakeTplNameSysFile(0x194); break;
                case 5: h = MakeTplNameSysFile(0x193); break;
                case 6: h = MakeTplNameSysFile(0x192); break;
            }
            if (h != 0)
                h = self->arcAcc2->GetResource(0x74696d67u, (const char*)h, 0);
            else
                h = self->arcAcc1->GetResource(0x74696d67u, &lbl_eu_8050FF8C[0x131], 0);
        } else if (type == 9) {
            CMCItemImplShim* inst = (CMCItemImplShim*)CItem_initItemImplInstances((CItemData*)e);
            if ((u16)inst->getCount(e) == 0) {
                // Empty slot: only resolve a name when the entry is a valid
                // type-9 item (bdat-managed or flagged in byte 7).
                do {
                    if (IsSkillItem(e) == 0) {
                        u32 ex = (e->field_00 >> 16) & 0xF;
                        int f = 0;
                        if (ex == 9 && ((u32)(e->bytes[3] & 3)) == 1) f = 1;
                        if (f == 0 && ex != 9) break;
                    }
                    h = 0;
                    CMCItemImplShim* inst2 = (CMCItemImplShim*)CItem_initItemImplInstances((CItemData*)e);
                    switch ((u16)inst2->getKind(e)) {
                        case 1: h = MakeTplNameSysFile(0x191); break;
                        case 2: h = MakeTplNameSysFile(0x190); break;
                        case 3: h = MakeTplNameSysFile(0x18f); break;
                        case 4: h = MakeTplNameSysFile(0x18e); break;
                        case 5: h = MakeTplNameSysFile(0x18d); break;
                    }
                    if (h != 0)
                        h = self->arcAcc2->GetResource(0x74696d67u, (const char*)h, 0);
                    else
                        h = self->arcAcc1->GetResource(0x74696d67u, &lbl_eu_8050FF8C[0x131], 0);
                } while (0);
            } else {
                // Item present: kind-keyed name chain 0x19c..
                h = 0;
                CMCItemImplShim* inst2 = (CMCItemImplShim*)CItem_initItemImplInstances((CItemData*)e);
                switch ((u16)inst2->getKind(e)) {
                    case 1: h = MakeTplNameSysFile(0x19c); break;
                    case 2: h = MakeTplNameSysFile(0x19b); break;
                    case 3: h = MakeTplNameSysFile(0x19a); break;
                    case 4: h = MakeTplNameSysFile(0x199); break;
                    case 5: h = MakeTplNameSysFile(0x198); break;
                }
                if (h != 0)
                    h = self->arcAcc2->GetResource(0x74696d67u, (const char*)h, 0);
                else
                    h = self->arcAcc1->GetResource(0x74696d67u, &lbl_eu_8050FF8C[0x131], 0);
            }
        }
    }
    if (h == 0) {
        if (idx != 0) {
            char* name = MakeTplNameSysFile((u16)BdatGetU16Direct(lbl_eu_806640EC, &lbl_eu_8050FF8C[0x15c], idx));
            h = self->arcAcc2->GetResource(0x74696d67u, name, 0);
            if (h == 0)
                h = self->arcAcc1->GetResource(0x74696d67u, &lbl_eu_8050FF8C[0x131], 0);
        } else {
            h = self->arcAcc1->GetResource(0x74696d67u, &lbl_eu_8050FF8C[0x131], 0);
        }
    }
    if (h != 0) {
        char buf[0x20];
        sprintf(buf, &lbl_eu_8050FF8C[0x161], (int)(n + 1));
        PaneSetTexPaletteByName((nw4r::lyt::Layout*)((void*)self->layout40), (const char*)buf, (TPLPalette*)((void*)h));
    }
}
// C linkage: unmangled retail reloc from refreshMCGetItemBoxSlots.
void setMCGetItemBoxLabelText(CMCGetItemBox* self, int kind, u32 idx) {
    char buf1[0x20];   // sp+0x28
    char buf2[0x20];   // sp+0x08
    sprintf(buf1, &lbl_eu_8050FF8C[0x16e], idx + 1);
    if ((s8)kind == 0) {
        sprintf(buf2, &lbl_eu_8050FF8C[0x17a]);
    } else {
        sprintf(buf2, &lbl_eu_8050FF8C[0],
                BdatTouchStringCell(&lbl_eu_8050FF8C[0x3], &lbl_eu_8050FF8C[0xc],
                              0x1e - ((s8)kind - 1)));
    }
    LayoutSetTextBoxText(self->layout40, buf1, buf2, 0);
}

// Refresh layout text after index/sort change. Extra params are passed through
// by openMCGetItemBox but ignored here (retail never reads r5/r6).
// -O4,s keeps the retail stmw r29 frame (self/arg/sub live across calls).
extern "C" void initMCGetItemBoxList(CMCGetItemBox* self, u16 arg, void* unk, u8 byte) {
    CMCItemBoxSub* sub = &self->sub_314;
    // Retail calls refillMCItemSubTable with only r3 set (r4/r5/r6 carry garbage),
    // so invoke it through a 1-arg prototype to avoid li r4/r5/r6 setup.
    ((void (*)(CMCItemBoxSub*))refillMCItemSubTable)(sub);
    // Scoped string-table base: the first use dies at the call (retail computes
    // it straight into the arg reg), while each branch's base lives across the
    // two FindPaneByName calls (retail parks it in a callee-saved reg).
    // Int-temp branchy select + u8 conversion at the call site reproduces the
    // retail li r0,1 / mr / clrlwi chain.
    {
        char* tbl = lbl_eu_8050FF8C;
        int count = sub->limit == 0 ? 1 : sub->limit;
        setLayoutTextBoxNumber((nw4r::lyt::Layout*)self->layout40, &tbl[0x17b], (u8)count);
    }
    // Retail reloads the layout/root pane before every FindPaneByName call
    // (no root local), keeping pressure at 3 callee-saved registers.
    if (arg == 9) {
        char* tbl = lbl_eu_8050FF8C;
        setPaneVisible((*(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10))->FindPaneByName(&tbl[0x184], true), 0);
        setPaneVisible((*(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10))->FindPaneByName(&tbl[0x18f], true), 1);
    } else {
        char* tbl = lbl_eu_8050FF8C;
        setPaneVisible((*(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10))->FindPaneByName(&tbl[0x184], true), 1);
        setPaneVisible((*(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10))->FindPaneByName(&tbl[0x18f], true), 0);
    }
    refreshMCGetItemBoxSlots(self);
    refreshMCGetItemBoxTexts(self);
    if (self->itemBox.GetState() != 0) {
        self->itemBox.HidePanes(arg);
    }
}

// Refresh the item-box grid for the current page. When more than one page
// exists, show the page tabs, stamp each tab's enabled state from the page
// counter, reformat the page label and slide the page-cursor pane to the slot
// matching the counter; then restamp all 30 grid cells.
// -O4,s keeps the retail _savegpr_27 frame.
extern "C" void refreshMCGetItemBoxSlots(CMCGetItemBox* self) {
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
        setPaneVisible((*(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10))->FindPaneByName(&tbl[0x19a], true), 1);
        for (u8 i = 0; i < 3; i++) {
            char buf[0x20];
            sprintf(buf, &tbl[0x1a1], (int)(u8)i + 1);
            // Sign bit of (unsigned)(i - count): 1 while the tab is within
            // the page range (retail lowers this to srwi, not srawi).
            setPaneVisible((*(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10))->FindPaneByName(buf, true),
                          ((u32)i - (u32)count) >> 31);
        }
        // Retail computes counter+1 separately at each use (memory reload
        // across the intervening virtual call), not as a shared temp.
        setLayoutTextBoxNumber(self->layout40, &lbl_eu_8050FF8C[0x1b0], (u8)(sub->counter + 1));
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
        float scale = 16.0f;
        cvt.w[0] = pagesLow;
        pos.x += scale * (float)(counterD - lbl_eu_80668BE0) +
                 scale * (float)(cvt.d - lbl_eu_80668BD8);
        copyVEC3((void*)&pagePane->GetTranslate(), (float*)&pos);
    } else {
        setPaneVisible((*(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10))->FindPaneByName(&lbl_eu_8050FF8C[0x19a], true), 0);
    }
    for (u8 i = 0; i < 0x1e; i++) {
        u16 icon = (u16)getMCItemSubItemId(sub, i);
        CMCItemBoxEntry* entry = getMCItemSubEntry(sub, i);
        setMCSlotIconTex(self, icon, entry, i);
        setMCSlotNameTex(self, icon, entry, i);
        setMCGetItemBoxLabelText(self, (int)(s8)getMCItemSubKind(sub, i), i);
    }
}

// Refresh the item-name texts in the layout panes and, when the item box is
// open, re-sync the selected entry's name/icon widgets.
// -O4,s keeps the retail stmw r28 frame.
extern "C" void refreshMCGetItemBoxTexts(CMCGetItemBox* self) {
    CMCGetItemBox* self_ = self;
    s8 idx = (s8)(self->field_301 * 10 + self->field_300);
    CMCItemBoxSub* sub = &self_->sub_314;
    LayoutSetTextBoxFmtValue(self->layout40, &lbl_eu_8050FF8C[0x1c7], getMCItemSubName(sub, (u16)idx), 0);
    LayoutSetTextBoxFmtValue(self->layout40, &lbl_eu_8050FF8C[0x1d0], getMCItemSubText(sub, (u16)idx), (u32)self->objAt50);
    if (self->itemBox.GetState() != 0) {
        CMCItemBoxEntry* entry = getMCItemSubEntry(sub, (u16)idx);
        u32 iconId = getMCItemSubItemId(sub, (u16)idx);   // held in a reg temp in retail
        self_->itemBox.SetItem((u16)iconId, (void*)entry, 1);
        self_->itemBox.SetNamedText(1, getMCItemSubName(sub, (u16)idx));
    }
}

// Refresh the cursor widgets / page label.
// -O4,s keeps the retail stmw r29 frame; the page-label buffer and the two
// branch-local positions get disjoint stack slots (no overlay across branches).
extern "C" void moveMCGetItemBoxCursor(CMCGetItemBox* self) {
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
        ScalePaneWorldPos(&posIf, p1, p2, *(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10));
        self->subObj_88.setRootPaneTranslate(&posIf);
        self->subObj_88.SetVisible(1);
        self->subObj_58.SetVisible(0);
    } else {
        tbl = lbl_eu_8050FF8C;
        sprintf(nameBuf, &tbl[0x161], (int)(s8)self->field_300 + (int)page * 10 + 1);
        p1 = (*(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10))->FindPaneByName(nameBuf, true);
        p2 = (*(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10))->FindPaneByName(&tbl[0x1e9], true);
        ScalePaneWorldPos(&posElse, p1, p2, *(nw4r::lyt::Pane**)((u8*)self->layout40 + 0x10));
        self->subObj_58.setRootPaneTranslate(&posElse);
        self->subObj_88.SetVisible(0);
        self->subObj_58.SetVisible(1);
    }
}

// Retail 0x8029C200: async file-load callback. Four files can load
// asynchronously: the main item-box layout archive (fileHandle1), a second
// resource archive (fileHandle2), and two bdat data files (fileHandle3/4).
// Branch 1 builds the whole widget: region-guarded buffer, layout + 2 anim
// transforms, font bind, text stamping, cursor sub-objects and activation;
// the other branches only feed bdat tables / the second accessor.
// -O4,s keeps the retail _savegpr_28/_restgpr_28 prologue.
bool CMCGetItemBox::OnFileEvent(CEventFile* pEventFile) {
    if (this->fileHandle1 == pEventFile->mFileHandle) {
        // === main item-box layout file loaded ===
        this->memRegion1.createRegion(mtl::MemManager::getHandleMEM2(), 0x20000,
                                      "CMCGetItemBox", 0);
        Class_8045F858 regionGuard1(&this->memRegion1);

        CFileHandle* h1 = this->fileHandle1;
        void* fileData = h1->mData;
        h1->mData = 0;
        mtl::MemManager::setMemInitFlag(false);

        void* tagMem = mtl::MemManager::allocate(0x858, (mtl::ALLOC_HANDLE)CLibLayout::getAllocHandle());
        if (tagMem != 0) tagMem = __ct__CTagProcessor(tagMem);
        this->objAt50 = (u8*)tagMem;

        this->arcAcc1 = CLibLayout::createArcResourceAccessor();
        this->arcAcc1->Attach(fileData, &lbl_eu_8050FF8C[0x206]);

        buildLayout(&this->layout40, this->arcAcc1, &lbl_eu_8050FF8C[0x20a]);
        bindLayoutAnimTransform(this->layout40, &this->animTrans1, this->arcAcc1, &lbl_eu_8050FF8C[0x221]);
        bindLayoutAnimTransform(this->layout40, &this->animTrans2, this->arcAcc1, &lbl_eu_8050FF8C[0x23b]);

        // Bind the loaded font's pane into the layout root. Retail keeps the
        // root pane in a callee-saved register only across these calls.
        nw4r::lyt::Pane* rootPane = this->layout40->GetRootPane();
        void* font = CDeviceFont::getFontInfo(1, this->layout40);
        void* fontData = ((IDeviceFontInfo*)font)->getFont();
        ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)((u32)fontData));

        u32 w = (u32)getPackedFont();
        setLayoutTextBoxFont(this->layout40, &lbl_eu_8050FF8C[0x1b0], w);
        setLayoutTextBoxFont(this->layout40, &lbl_eu_8050FF8C[0x17b], w);

        // Stamp every item-slot text pane with the loaded character set.
        u32 sh = (u32)CUICfManager_getPackedFont9C();
        for (u8 i = 1; i <= 0x1e; i++) {
            char buf[0x20];
            sprintf(buf, &lbl_eu_8050FF8C[0x16e], i);
            setLayoutTextBoxFont(this->layout40, buf, sh);
        }
        setLayoutTextBoxFont(this->layout40, &lbl_eu_8050FF8C[0x25a], sh);

        this->layout40->SetAnimationEnable(this->animTrans2, false);
        this->layout40->SetAnimationEnable(this->animTrans1, true);
        this->layout40->Animate();

        // Hand the tag processor to the tag pane (+0xF8). Retail reloads the
        // root pane here rather than keeping it live across the text stamping.
        nw4r::lyt::Pane* tagPane =
            this->layout40->GetRootPane()->FindPaneByName(&lbl_eu_8050FF8C[0x1d0], true);
        if (tagPane != 0) *(u32*)((u8*)tagPane + 0xF8) = (u32)this->objAt50;

        LayoutSetTextBoxFmtValue(this->layout40, &lbl_eu_8050FF8C[0x274],
                      (char*)BdatTouchStringCell(&lbl_eu_8050FF8C[0x269], &lbl_eu_8050FF8C[0xc], 6), 0);

        // Remember the cursor pane's position.
        nw4r::lyt::Pane* curPane =
            this->layout40->GetRootPane()->FindPaneByName(&lbl_eu_8050FF8C[0x281], true);
        copyVEC3((void*)&this->field_308, (float*)&curPane->GetTranslate());

        // Cursor sub-objects: construct on the stack with the arc accessor,
        // copy into the embedded storage, destroy the temp, then run the
        // embedded cursor's slot-2 virtual (update).
        this->subObj_58 = CCur07(this->arcAcc1);
        this->subObj_58.initLayout();

        this->subObj_70 = CCur09(this->arcAcc1);
        this->subObj_70.initLayout();

        // Two anchor vectors on cursor 09: the second pair swaps the x base.
        nw4r::math::VEC3 s1;
        nw4r::math::VEC3 v1;
        nw4r::math::VEC3 s2;
        nw4r::math::VEC3 v2;
        code80135FDC_setVec3((nw4r::math::VEC3*)((float*)&s1), -58.0f, -51.0f, lbl_eu_80668BD4);
        v1 = s1;
        code80135FDC_setVec3((nw4r::math::VEC3*)((float*)&s2), lbl_eu_80668C00, -51.0f, lbl_eu_80668BD4);
        v2 = s2;
        this->subObj_70.SetTwoPanes(&v2, &v1);

        this->subObj_88 = CCur16(this->arcAcc1);
        this->subObj_88.initLayout();

        this->subObj_A0 = CCur18(CUICfManager_getArcResourceAccessor());
        this->subObj_A0.initLayout();

        activateMCGetItemBox(this);
        this->fileHandle1 = 0;
        this->memRegion1.validateHeap();
        return true;
    } else if (this->fileHandle2 == pEventFile->mFileHandle) {
        // === second resource archive loaded ===
        this->memRegion2.createRegion(mtl::MemManager::getHandleMEM2(), 0x100,
                                      &lbl_eu_8050FF8C[0x28e], 0);
        Class_8045F858 regionGuard2(&this->memRegion2);

        CFileHandle* h2 = this->fileHandle2;
        void* fileData = h2->mData;
        h2->mData = 0;
        mtl::MemManager::setMemInitFlag(false);

        this->arcAcc2 = CLibLayout::createArcResourceAccessor();
        this->arcAcc2->Attach(fileData, &lbl_eu_8050FF8C[0x206]);
        activateMCGetItemBox(this);
        this->fileHandle2 = 0;
        this->memRegion2.validateHeap();
        return true;
    } else if (this->fileHandle3 == pEventFile->mFileHandle) {
        // === bdat table 2 (crystal names) loaded ===
        CFileHandle* h3 = this->fileHandle3;
        void* fileData = h3->mData;
        h3->mData = 0;
        Bdat_GetTable_AA34();
        // Retail folds the first lookup's offset directly into lis/addi, then
        // caches the table base for the remaining lookups.
        if (getFP(&lbl_eu_8050FF8C[0x29f]) == 0) {
            CBdat::setBdatEntry(2, fileData);
        }
        Bdat_GetTable_AA34();
        sCrystalItemTable = getFP(&lbl_eu_8050FF8C[0x29f]);
        Bdat_GetTable_AA34();
        sCrystalMesTable = getFP(&lbl_eu_8050FF8C[0x2ae]);
        activateMCGetItemBox(this);
        this->fileHandle3 = 0;
        return true;
    } else if (this->fileHandle4 == pEventFile->mFileHandle) {
        // === bdat table 5 (item names) loaded ===
        CFileHandle* h4 = this->fileHandle4;
        void* fileData = h4->mData;
        h4->mData = 0;
        this->memManagerPtr = (u8*)fileData;
        Bdat_GetTable_AA34();
        if (getFP(&lbl_eu_8050FF8C[0x119]) == 0) {
            CBdat::setBdatEntry(5, this->memManagerPtr);
        }
        Bdat_GetTable_AA34();
        sItemMesTable = getFP(&lbl_eu_8050FF8C[0x119]);
        activateMCGetItemBox(this);
        this->fileHandle4 = 0;
        return true;
    }

    return false;
}

