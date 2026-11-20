// Auto-scaffolded catalog TU for kyoshin/code_80135FDC
// Replace stubs with high-level C/C++ during decomp.
#include "kyoshin/cf/CtrlRemoteApi.hpp"
#include "kyoshin/cf/CfBdat.hpp"
#include "libs/monolib/src/scn/CScn_8049603C.hpp" // Scn_QueryUnk80State (single owner decl)
#include "kyoshin/cf/CtrlObjectParamSymbols.hpp"
#include "kyoshin/cf/CfResReloadImpl.hpp"  // getReloadParam0 (owner)
#include "kyoshin/cf/CfSoundMan.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

// signatures that differ from the (not-yet-matched) definitions below, so
// suppress those declarations when included from this defining TU.
#define CODE_80135FDC_CPP
#include "kyoshin/code_80135FDC.hpp"
#include "kyoshin/cf/CfTFile.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include <nw4r/math/math_types.h>
#include <nw4r/lyt/lyt_animation.h>
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_textBox.h>
#include <nw4r/lyt/lyt_drawInfo.h>
#include <nw4r/lyt/lyt_arcResourceAccessor.h>
#include <nw4r/lyt/lyt_material.h>
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/device/CDeviceSC.hpp"
#include <revolution/gx/GXTransform.h>
#include <revolution/gx/GXPixel.h>
#include <revolution/mtx/mtx44.h>
#include <revolution/enc/encunicode.h>
#include <revolution/mtx/vec.h>
#include <nw4r/math/math_arithmetic.h>
#include <math.h>
#include <string.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
namespace nw4r { namespace db { void Panic(const char* file, int line, const char* fmt, ...); } }
namespace nw4r { namespace math { f32 FrSqrt(f32 x); } }
#include "monolib/scn/code_804BC9EC.hpp"
#include "kyoshin/plugin/ocBdat.hpp"

extern "C" void queueEventId__Q22cf13CfGameManagerFv(u32 value);

extern "C" {
// copyVEC3 stays inline
// move into code_80135FDC.hpp would clash in CCollepedia.cpp.
extern void copyVEC3(nw4r::math::VEC3*, const nw4r::math::VEC3*);
// CtrlRemote_TouchBitByArg stays inline: include/functions.hpp declares it as
// bool CtrlRemote_TouchBitByArg(int) - a move into code_80135FDC.hpp would clash in
// CGame / CMenu* units that pull functions.hpp via monolib/scn.hpp.

}

// getBdatStringColumnValue's canonical extern "C" declaration is
// u32(void*, const char*, int) (shared with CfBdat.hpp / CfGimmick.hpp /
// code_801862C0.hpp); extern "C" names cannot be overloaded, so this TU's
// historical pointer-style call sites route through a no-op inline wrapper.
// The casts are register no-ops, so emitted code is unchanged.
static inline void* getBdatStringColumnValue_str(void* bdat, const char* col,
                                                 const void* row) {
    return (void*)getBdatStringColumnValue(bdat, col, (int)(intptr_t)row);
}
#define getBdatStringColumnValue(bdat, col, row) \
    getBdatStringColumnValue_str(bdat, col, row)

// Tag-proc owner for LayoutSetTextBoxText/SetLayoutSelfText's slot-0x14 dispatch.
// The full CTagProcessorBase lives in kyoshin/CTagProcessor.hpp, which
// cannot be included here: it defines its own byte-wise nw4r::ut::Color
// that clashes with the real lyt headers used by this TU. Only the vtable
// prefix matters for the virtual call below (the base provides slots
// 0x08/0x0C/0x10, so Proc lands at retail slot 0x14 = TagProcPreprocessMessage);
// keep this in sync with CTagProcessor.hpp. No TU may include both headers.
struct CTagProcessorBase : public nw4r::ut::TagProcessorBase<wchar_t> {
    virtual const wchar_t* Proc(wchar_t* text, int param, float a, float b); // 0x14
};


/* ===== BEGIN typified .data (was absorb_kyoshin_code_80135FDC_cpp_data[0x118]) ===== */
// Retail order: dispatch jumptable (opaque reloc blob) -> NW4R assert strings.
// Declared before all code so MWCC emits them ahead of its own switch tables
// (enables drop_data_tail=((".data", 0x118))). Gaps fold into array tails.
// RESIDUAL: the 0x38 jumptable of LookupItemTypeTable cannot be expressed in C++
// (branch-table targets are local .text labels mid-function); it stays a sized
// static blob. Retail names it jumptable_eu_8052E488 (local); MWCC's own copy
// of this table plus func_80138574's 21-entry switch table trail the section
// natively. MWCC emits a switch table at its function's definition point, so
// defining LookupItemTypeTable before this block with the retail case shape
// (case 0 -> epilogue, then 13 distinct case bodies) would emit the real table
// at .data+0. The blob keeps a +0 canary only to stay in .data: all-zero
// initializers (static/global, const/volatile/used, explicit section) route to
// .bss under Wii/1.1. The canary is a real 1-byte diff and has no patch_data
// shim; the raw data gate reports MISMATCH (.data tail tables and the .sdata2
// pool are further residuals). No aligned(8): that+section combo is suspected
// of voiding the section.)
__attribute__((used))
unsigned char jumptable_eu_8052E488[0x38] = { 1 };

// NW4R assert strings (writable .data in retail; u8[] matches header decls).
__attribute__((used)) u8 sNw4rNullPointerAssertMsg[0x24] = "NW4R:Pointer must not be NULL (p)";
__attribute__((used)) u8 sNw4rLinkListFile[0xC] = "LinkList.h";
__attribute__((used)) u8 sNw4rTexMapAssertMsg[0x34] = "NW4R:Failed assertion texMapIdx < mGXMemNum.texMap";
__attribute__((used)) u8 sNw4rMaterialFile[0xC] = "material.h";
__attribute__((used)) u8 sNw4rTevColorAssertMsg[0x28] = "NW4R:Failed assertion idx < TevColorMax";
__attribute__((used)) u8 sNw4rTevColorAssertFile[0x10] = "material.h";
__attribute__((used)) u8 lbl_eu_8052E568[0x28] = "NW4R:Failed assertion idx < TevColorMax";
__attribute__((used)) u8 lbl_eu_8052E590[0x10] = "material.h";
/* ===== END typified .data ===== */

// ---------- globals ----------
extern "C" {
// ::cf::CfBdat::spFldMapListFileData / ::cf::CfBdat::spBtlSkillListFileData / ::cf::CfBdat::spItmCollectListFileData stay inline: they are
// declared as void* in CCollepedia.hpp / CKizunaTalkList.hpp / CMCEffStart.hpp
// (u32 here) - moving them into code_80135FDC.hpp would clash in those callers.
// ::cf::CfBdat::spItmItemListFileData stays inline: CMCGetItemBox.hpp declares it as void* - a
// move into code_80135FDC.hpp would clash in CMCGetItemBox.cpp.
}

// lbl_eu_8052E568 / lbl_eu_8052E590 stay inline: CItemBoxInfo.hpp declares
// them as char[] (u8[] here) - a move into code_80135FDC.hpp would clash in
// CMCGetItemBox.cpp.
extern "C" {
extern u8 lbl_eu_8052E568[];
extern u8 lbl_eu_8052E590[];
}

// Bdat_GetTable_AA34: code_801862C0.hpp (via harness_catalog.hpp) declares it as
// void*() - an extern-"C" function cannot be redeclared with a different
// signature here. Retail callers in this TU pass a table name in r3 (the
// callee ignores it), so those call sites go through a signature cast; MWCC
// emits the same direct bl to Bdat_GetTable_AA34 either way.
#define Bdat_GetTable_AA34(name) ((void (*)(const char*))Bdat_GetTable_AA34)(name)



// --- helpers ---
// Bdat_GetMaxRow_B1EC stays inline: CKizunaTalkList.hpp declares it as (void*) - a
// move into code_80135FDC.hpp would clash in CKizunaTalkList.cpp.
extern "C" u32 Bdat_GetMaxRow_B1EC(void*);

// void CtrlRemote_SetSharedBit(int, int) - a move into code_80135FDC.hpp would clash in
// CGame / CMenu* units that pull functions.hpp via monolib/scn.hpp.
extern "C" void CtrlRemote_SetSharedBit(u32 destination, u32 value);

// Real class tree recovered from retail vtables (see header).

// Anim-link item view: the link item holds its AnimTransform at +0x8.
struct AnimLinkItemView {
    u8 _00[8];
    nw4r::lyt::AnimTransform* anim8;
};

// ---------- init ----------
extern "C" void MenuStateInitFlags() {
    gMenuStateMain = 0;
    gMenuNotifyFlag = 0;
    gMenuStateSecondary = 0;
    gMenuStateReady = 0;
    gMenuParamA = 0;
    gSysMenuActive = 1;
    gSysFileListBdat = 0;
    gSysIconListBdat = 0;
    gMenuSubStateFlag = 0;
}

extern "C" void MenuStateSet64058() {
    gMenuStateMain = 1;
}

extern "C" void MenuStateClear6405A() {
    gMenuStateSecondary = 0;
    gMenuStateReady = 0;
    gMenuParamA = 0;
    gMenuParamB = 0;
}

extern "C" void MenuStateSet6405A() {
    gMenuStateReady = 0;
    gMenuParamA = 0;
    gMenuParamB = 0;
    gMenuStateSecondary = 1;
}

extern "C" u8 GetCollectedFlagByte(u32 idx) {
    return gCollectedFlags[idx];
}

// ---------- BDAT helpers ----------
// BdatGetU8ByTableKey: BDAT string -> u8
// BdatGetU16ByTableKey: BDAT string -> u16
// BdatGetS8ByTableKey: BDAT string -> s8
// BdatGetS16ByTableKey: BDAT string -> s16
// BdatTouchStringCell: BDAT string -> void

// BDAT string column readers. Each validates the table (Bdat_GetTable_AA34),
// resolves the file pointer, reads the cell, and reinterprets it at the
// requested width/sign: u8 / u16 / s16 / s8.
u8 BdatGetU8ByTableKey(const char* tableName, const char* column, u32 key) {
    Bdat_GetTable_AA34((const char*)tableName);
    void* fp = getFP((const char*)tableName);
    void* result = getBdatStringColumnValue(fp, (const char*)column,
                                            (const void*)(uintptr_t)key);
    return *(u8*)&result;
}

u16 BdatGetU16ByTableKey(const char* tableName, const char* column, u32 key) {
    Bdat_GetTable_AA34((const char*)tableName);
    void* fp = getFP((const char*)tableName);
    void* result = getBdatStringColumnValue(fp, (const char*)column,
                                            (const void*)(uintptr_t)key);
    return *(u16*)&result;
}

s16 BdatGetS16ByTableKey(const char* tableName, const char* column, u32 key) {
    Bdat_GetTable_AA34((const char*)tableName);
    void* fp = getFP((const char*)tableName);
    void* result = getBdatStringColumnValue(fp, (const char*)column,
                                            (const void*)(uintptr_t)key);
    return *(s16*)&result;
}

s8 BdatGetS8ByTableKey(const char* tableName, const char* column, u32 key) {
    Bdat_GetTable_AA34((const char*)tableName);
    void* fp = getFP((const char*)tableName);
    void* result = getBdatStringColumnValue(fp, (const char*)column,
                                            (const void*)(uintptr_t)key);
    // Retail spills the cell value to the stack and re-reads it sign-extended.
    s32 byte = *(s8*)&result;
    return (s8)byte;
}

char* BdatTouchStringCell(const char* table, const char* column, int row) {
    Bdat_GetTable_AA34(table);
    void* fp = getFP(table);
    return (char*)getBdatStringColumnValue(fp, column, (const void*)row);
}

u8 BdatGetU8Direct(const void* bdat, const char* column, int row) {
    if (bdat == 0) return 0;
    Bdat_GetTable_AA34((const char*)bdat);
    void* result = getBdatStringColumnValue((void*)bdat, column, (const void*)row);
    return *(u8*)&result;
}

u16 BdatGetU16Direct(const void* bdat, const char* column, int row) {
    if (bdat == 0) return 0;
    Bdat_GetTable_AA34((const char*)bdat);
    void* result = getBdatStringColumnValue((void*)bdat, (const char*)column, (const void*)row);
    return *(u16*)&result;
}

extern "C" int BdatGetS8Direct(const void* bdat, const char* column, int row) {
    if (bdat == 0) return 0;
    Bdat_GetTable_AA34((const char*)bdat);
    void* result = getBdatStringColumnValue((void*)bdat, column, (const void*)row);
    return (s8)(*(u8*)&result);
}

extern "C" s16 BdatGetS16Direct(const void* bdat, const char* column, int row) {
    if (bdat == 0) return 0;
    Bdat_GetTable_AA34((const char*)bdat);
    void* result = getBdatStringColumnValue((void*)bdat, column, (const void*)row);
    return *(s16*)&result;
}

extern "C" char* BdatGetPtrDirect(const void* bdat, const char* column, int row) {
    if (bdat == 0) return 0;
    Bdat_GetTable_AA34((const char*)bdat);
    return (char*)getBdatStringColumnValue((void*)bdat, column, (const void*)row);
}

extern "C" u32 Utf8ToWcsCopyFix(const char* src, u16* dst, u32 destLen) {
    u32 srcLen = strlen(src);
    ENCResult result = ENCConvertStringUtf8ToUtf16(
        dst, &destLen, reinterpret_cast<const u8*>(src), &srcLen);
    if (result != 0) {
        wcscpy(reinterpret_cast<wchar_t*>(dst),
               reinterpret_cast<const wchar_t*>(gSjisTablePtr));
        return 1;
    }
    dst[destLen] = 0;
    int i;
    int len;
    i = 0;
    len = (int)destLen;
    while (*dst != 0 && i < len) {
        if (*dst == 0x40u)
            *dst = 0x0Au;
        dst++;
        i++;
    }
    return 1;
}

// Range-pair fusion block (MWCC_CASES CItemBlock_testKindFlag): the second
// comparison of each pair is commuted (constant-first), which blocks MWCC's
// subi/cmpli range fusion while emitting the same cmpi+bgt/bng.
// (An optimize_for_size pragma was tried here and has no effect on fusion.)
extern "C" void SjisDotToCommaEU(char* str) {
    int lang = CDeviceSC::getLanguage();
    if ((u8)(lang + 0xFE) > 2) return;
    s8 c;
    s8 d;
    int len = strlen(str);
    int i = 0;
    while (i < len) {
        c = *str;
        if (c == 0x2e) {
            *str = 0x2c;
            goto next;
        }
        if (c < 0x81)
            goto g2;
        if (0x9f >= c)
            goto match;
g2: {
            d = *str;
            if (d < 0xe0)
                goto next;
            if (0xef < d)
                goto next;
        }
match:
        i += 1;
        str += 1;
next:
        i += 1;
        str += 1;
    }
}

extern "C" int SplitStringTrim(char* src, char delim, char** outTokens) {
    // Token slots via scaled subscript outTokens[count++] (explicit scale
    // keeps a byte offset IV and stwx; a stepped byte offset or u32-slots
    // word index strength-reduces to a walking pointer). count parallels it
    // for the return value.
    // Split declare (colors) from assign (loads): decl order drives the
    // saved-reg allocation, init order keeps the retail load schedule.
    int len;
    int count;
    int i;
    count = 1;
    *outTokens = src;
    len = strlen(src);
    i = 0;
    while (i < len) {
        char c = src[i];
        if (c == delim) {
            src[i] = 0;
            char* q = &src[i] + 1;
            int j = 1;
            // Test-first space skip (retail beq-to-body, exit falls through
            // to the token store; while/do-while rotate or invert it).
spctest:
            if (' ' == *q)
                goto spcbody;
            outTokens[count++] = &src[i] + j;
            goto next;
spcbody:
            q++;
            j++;
            goto spctest;
        } else {
            // Commuted 2nd comparisons block subi/cmpli fusion
            // (MWCC_CASES CItemBlock_testKindFlag); lead falls into i++.
            if (c < 0x81)
                goto sjis2;
            if (0x9f >= c)
                goto lead;
sjis2: {
                char d = src[i];
                if (d < 0xe0)
                    goto next;
                if (0xef < d)
                    goto next;
            }
lead:
            i++;
        }
next:
        i++;
    }
    // trim trailing spaces from each token; retail rotates this into a
    // countdown loop (mtctr/bdnz on the remaining scan length)
    char** p = outTokens;
    for (int k = 0; k < count; k++) {
        char* t = *p;
        p++;
        for (int pos = strlen(t) - 1; pos >= 0; pos--) {
            if (t[pos] != ' ') {
                t[pos + 1] = 0;
                break;
            }
        }
    }
    return count;
}

extern "C" int SplitWStringTrim(u16* src, u16 delim, u16** outTokens) {
    // Mirrors SplitStringTrim: scaled subscript keeps byte-IV + stwx;
    // test-first space skip via hand goto (beq-to-body, no guard);
    // delim-first comparison matches retail cmpl order.
    u16* p = src;
    int count = 1;
    outTokens[0] = src;
    int len = wcslen((wchar_t*)src);
    for (int i = 0; i < len; i++) {
        if (delim == *p) {
            *p = 0;
            int j;
            u16* q;
            q = p + 1;
            j = 1;
wspctest:
            if (L' ' == *q)
                goto wspcbody;
            outTokens[count++] = p + j;
            goto wnextp;
wspcbody:
            q++;
            j++;
            goto wspctest;
        }
wnextp:
        p++;
    }
    for (int k = 0; k < count; k++) {
        u16* t = outTokens[k];
        int pos = wcslen((wchar_t*)t) - 1;
        int bpos = pos * 2;
        u16* q = (u16*)((u8*)t + bpos);
        while (pos >= 0) {
            if (*q == L' ') {
                q--;
                pos--;
                bpos -= 2;
            } else {
                q = t + pos;
                q[1] = 0;
                break;
            }
        }
    }
    return count;
}

extern "C" void WcsToUpperInPlace(u16* str) {
    int len = wcslen((wchar_t*)str);
    u16* p = str;
    while (len > 0) {
        *p = (u16)toupper(*p);
        p++;
        len--;
    }
}

extern "C" void ApplyFontToTxtPanes(nw4r::lyt::Pane* node, const nw4r::ut::Font* font) {
    extern const char lbl_eu_80500664[];
    if (node == NULL) return;
    void* child = *(void**)((u8*)node + 0x14);
    void* head = (u8*)node + 0x14;
    char buf[32];
    // tokens is oversized in retail (stack frame reserves 18 entries);
    // SplitStringTrim never fills more than a handful.
    char* tokens[18];
    while (child != head) {
        if (child == NULL) {
            nw4r::db::Panic((const char*)lbl_eu_8052CB40, 0x23D, (const char*)lbl_eu_8052CB1C);
        }
        u32 type = (u32)child - 4;
        if (type == 0) {
            nw4r::db::Panic((const char*)sNw4rLinkListFile, 0x193, (const char*)sNw4rNullPointerAssertMsg);
        }
        sprintf(buf, lbl_eu_80500664, type + 0xBC);
        SplitStringTrim(buf, '_', tokens);
        char* t0 = tokens[0];
        int tl = strlen(t0);
        int i = 0;
        while (i < tl) {
            t0[i] = (char)toupper(t0[i]);
            i++;
        }
        if (strcmp(tokens[0], &lbl_eu_80500664[3]) == 0) {
            if (type != 0) {
                SetFont__Q34nw4r3lyt7TextBoxFPCQ34nw4r2ut4Font(
                    (nw4r::lyt::TextBox*)type, font);
            }
        }
        ApplyFontToTxtPanes((nw4r::lyt::Pane*)type, font);
        child = *(void**)child;
    }
}

void setLayoutTextBoxFont(
    nw4r::lyt::Layout* layout, char* name, u32 font) {
    nw4r::lyt::Pane* pane = layout->GetRootPane()->FindPaneByName(name, true);
    if (pane != NULL) {
        SetFont__Q34nw4r3lyt7TextBoxFPCQ34nw4r2ut4Font(
            static_cast<nw4r::lyt::TextBox*>(pane),
            reinterpret_cast<const nw4r::ut::Font*>(font));
    }
}

void setLayoutTextBoxNumber(
    nw4r::lyt::Layout* layout, char* name, int value) {
    char buf[512];
    extern const char lbl_eu_80500664[];
    sprintf(buf, &lbl_eu_80500664[7], value);
    LayoutSetTextBoxText(layout, name, buf, 0);
}

extern "C" void LayoutSetTextBoxText(
    nw4r::lyt::Layout* layout, const char* name, const char* text, u32 tagProc) {
    nw4r::lyt::Pane* pane = layout->GetRootPane()->FindPaneByName(name, true);
    if (pane == NULL) return;

    u16 buf[1024];
    u32 destLen = 1024;
    u32 srcLen = strlen(text);

    ENCResult result = ENCConvertStringUtf8ToUtf16(
        buf, &destLen, reinterpret_cast<const u8*>(text), &srcLen);

    if (result != 0) {
        wcscpy(reinterpret_cast<wchar_t*>(buf),
               reinterpret_cast<const wchar_t*>(&gSjisTablePtr));
    } else {
        // Pointer + counter scan (same loop shape as Utf8ToWcsCopyFix): retail
        // keeps len in a saved reg, walks buf with a pointer, and stores
        // the terminator via a zero register before the loop.
        int len = (int)destLen;
        u16* p = buf;
        p[len] = 0;
        int i = 0;
        while (*p != 0 && i < len) {
            if (*p == 0x40u)
                *p = 0x0Au;
            p++;
            i++;
        }
    }

    if (tagProc != 0) {
        // CTagProcessorBase::Proc (slot 0x14, retail TagProcPreprocessMessage)
        const wchar_t* res = reinterpret_cast<CTagProcessorBase*>(tagProc)->Proc(
            reinterpret_cast<wchar_t*>(buf), 0,
            38.0f, 38.0f);
        wcscpy(reinterpret_cast<wchar_t*>(buf), res);
    }

    static_cast<nw4r::lyt::TextBox*>(pane)->SetString(
        reinterpret_cast<const wchar_t*>(buf), 0);
}

extern "C" void LayoutSetTextBoxFmtValue(
    nw4r::lyt::Layout* layout, const char* name, const char* text, u32 tagProc) {
    // Retail ignores the fmt param (r5 is never read) and formats with the
    // constant string-table extern lbl_eu_80500664 (lis/@l relocs, offset 0).
    extern const char lbl_eu_80500664[];
    char buf[520];
    // Retail passes the fmt param (r5) as the sprintf vararg; the format is
    // the constant lbl_eu_80500664 (r4 = lis/@l, fmt param never read as a
    // format). The 4th LayoutSetTextBoxText arg is the value param (r6 -> r31).
    sprintf(buf, lbl_eu_80500664, text);
    LayoutSetTextBoxText(layout, name, buf, tagProc);
}

extern "C" void LayoutSetTextBoxInt(nw4r::lyt::Layout* layout, u32 value) {
    char buf[512];
    sprintf(buf, &lbl_eu_80500664[7], value);
    SetLayoutSelfText(layout, buf, 0);
}

extern "C" void SetLayoutSelfText(
    nw4r::lyt::Layout* layout, const char* text, u32 tagProc) {
    u16 buf[1024];
    u32 destLen = 1024;
    u32 srcLen = strlen(text);

    ENCResult result = ENCConvertStringUtf8ToUtf16(
        buf, &destLen, reinterpret_cast<const u8*>(text), &srcLen);

    if (result != 0) {
        wcscpy(reinterpret_cast<wchar_t*>(buf),
               reinterpret_cast<const wchar_t*>(gSjisTablePtr));
    } else {
        // same loop shape as Utf8ToWcsCopyFix (pointer + counter)
        buf[destLen] = 0;
        int len = (int)destLen;
        u16* p = buf;
        int i = 0;
        while (*p != 0 && i < len) {
            if (*p == 0x40u)
                *p = 0x0Au;
            p++;
            i++;
        }
    }

    if (tagProc != 0) {
        // CTagProcessorBase::Proc (slot 0x14, retail TagProcPreprocessMessage);
        // retail passes the same float in f1 and f2
        const wchar_t* res = reinterpret_cast<CTagProcessorBase*>(tagProc)->Proc(
            reinterpret_cast<wchar_t*>(buf), 0,
            38.0f, 38.0f);
        wcscpy(reinterpret_cast<wchar_t*>(buf), res);
    }

    // TextBox::SetString (slot 0x7C); the target is a text pane
    reinterpret_cast<nw4r::lyt::TextBox*>(layout)->SetString(
        reinterpret_cast<const wchar_t*>(buf), 0);
}

void buildLayout(
    nw4r::lyt::Layout** ppLayout,
    nw4r::lyt::ArcResourceAccessor* accessor,
    const char* name) {
    *ppLayout = createLayout__10CLibLayoutFv();
    void* resource = accessor->GetResource(0, name, 0);
    (*ppLayout)->Build(resource, accessor);
}

void bindLayoutAnimTransform(
    nw4r::lyt::Layout* layout,
    nw4r::lyt::AnimTransform** ppAnimTrans,
    nw4r::lyt::ArcResourceAccessor* accessor,
    char* name) {
    void* resource = accessor->GetResource(0, name, 0);
    nw4r::lyt::AnimTransform* animTrans =
        layout->CreateAnimTransform(resource, accessor);
    *ppAnimTrans = animTrans;
    layout->BindAnimation(animTrans);
}

extern "C" void LayoutBindAnimResource(
    nw4r::lyt::Layout* layout,
    void** ppAnimRes,
    nw4r::lyt::ArcResourceAccessor* accessor,
    char* name) {
    // Declaration order drives MWCC's saved-reg allocation: resource must
    // land in r30 and mem in r31 to match retail.
    void* mem;
    void* resource = accessor->GetResource(0, name, 0);
    mem = mtl::MemManager::allocate(0x10, CLibLayout::getAllocHandle());
    if (mem != NULL) {
        Set__Q34nw4r3lyt12AnimResourceFPCv(mem, resource);
    }
    *ppAnimRes = mem;
    layout->BindAnimationAuto(
        *reinterpret_cast<nw4r::lyt::AnimResource*>(mem), accessor);
}

void drawLayout(
    nw4r::lyt::Layout* layout,
    nw4r::lyt::DrawInfo* drawInfo,
    int useProjection,
    int calcMtx) {
    if (layout == NULL) return;

    nw4r::ut::Rect rect = layout->GetLayoutRect();

    if (useProjection) {
        void* camera = reinterpret_cast<void*>(gmScene);
        void* view = getCurrentView__5CViewFv();
        CViewFrame37038* viewFrame =
            reinterpret_cast<CViewFrame37038*>(Scn_HasCamItem(camera, view));
        GXSetProjection(viewFrame->projection, GX_PERSPECTIVE);

        drawInfo->SetViewMtx(viewFrame->mtx);
        drawInfo->SetViewRect(rect);
        GXSetZMode(true, GX_LEQUAL, false);
    } else {
        Mtx44 ortho;
        f32 right = rect.right;
        f32 left = rect.left;
        f32 bottom = rect.bottom;
        f32 top = rect.top;
        C_MTXOrtho(ortho, top, bottom, left, right,
                   -1000.0f, 1000.0f);
        GXSetProjection(ortho, GX_ORTHOGRAPHIC);

        drawInfo->SetViewMtx(reinterpret_cast<const nw4r::math::MTX34&>(ml::CMat34::identity));
        drawInfo->SetViewRect(rect);
        GXSetZMode(false, GX_NEVER, false);
    }

    if (calcMtx) {
        layout->CalculateMtx(*drawInfo);
    }
    layout->Draw(*drawInfo);
}

void SetWideLocAdjust(nw4r::lyt::DrawInfo* drawInfo) {
    if (!CDeviceSC::isWideAspectRatio()) return;

    // Toggle the locationAdjust flag bit and reload the location-adjust
    // scale from the tuned sdata2 constants (wide aspect only).
    bool adj = !drawInfo->IsLocationAdjust();
    nw4r::math::VEC2 scale;
    scale.x = 0.75f;
    scale.y = 1.0f;
    drawInfo->SetLocationAdjustScale(scale);
    drawInfo->SetLocationAdjust(adj);
}

extern "C" u8 MapValueToRank6(int value) {
    // Range lookup via shared result labels so each value emits exactly one
    // li/blr block, laid out in retail's ascending order.
    if (value < 11) {
        if (value < 4) {
            if (value < 2) goto ret0;
            goto ret1;
        }
        if (value < 7) goto ret2;
        goto ret3;
    }
    if (value >= 19) {
        if (value >= 29) goto ret0;
        goto ret5;
    }
    if (value >= 17) goto ret2;
    goto ret4;

ret1:
    return 1;
ret2:
    return 2;
ret3:
    return 3;
ret4:
    return 4;
ret5:
    return 5;
ret0:
    return 0;
}

extern "C" u32 BdatGetRankByName(u32 row) {
    // BDAT lookup: row index from one table keyed by the caller row, then a u8 column
    // from another table keyed by that index; the byte maps through a range
    // tree identical to MapValueToRank6's (inlined here in retail).
    // col0x22 is computed before the global test (retail hoists it).
    const char* col0x22 = &lbl_eu_80500664[0x22];
    u16 rowIdx;
    if (::cf::CfBdat::spFldNpcListFileData == 0) {
        rowIdx = 0;
    } else {
        Bdat_GetTable_AA34((const char*)row);
        void* result = (void*)(getBdatStringColumnValue)(
            reinterpret_cast<void*>(::cf::CfBdat::spFldNpcListFileData),
            col0x22, row);
        rowIdx = *reinterpret_cast<u16*>(&result);
    }

    // retail validates the raw base row here, then opens base+0x15
    Bdat_GetTable_AA34(lbl_eu_80500664);
    void* fp = getFP(&lbl_eu_80500664[0x15]);
    void* result2 = getBdatStringColumnValue(
        fp, &lbl_eu_80500664[0x0F],
        reinterpret_cast<const void*>(static_cast<u32>(rowIdx)));
    u8 val = *reinterpret_cast<u8*>(&result2);

    // Range lookup via shared result labels so each value emits exactly one
    // li/blr-style block, laid out in retail's ascending order.
    if (val < 11) {
        if (val < 4) {
            if (val < 2) goto ret0;
            goto ret1;
        }
        if (val < 7) goto ret2;
        goto ret3;
    }
    if (val >= 19) {
        if (val >= 29) goto ret0;
        goto ret5;
    }
    if (val >= 17) goto ret2;
    goto ret4;

ret1:
    return 1;
ret2:
    return 2;
ret3:
    return 3;
ret4:
    return 4;
ret5:
    return 5;
ret0:
    return 0;
}

u32 advanceAnimTransform(
    nw4r::lyt::AnimTransform* anim, float delta) {
    float newFrame = delta + anim->GetFrame();
    u32 result = 0;
    if (CDeviceVI::isTvFormatPal()) {
        newFrame += 0.2f;
    }
    u16 frameSize = anim->GetFrameSize();
    if (newFrame >= static_cast<float>(frameSize)) {
        if (anim->IsLoopData()) {
            newFrame = 0.0f;
        } else {
            newFrame = static_cast<float>(anim->GetFrameSize()) - 1.0f;
        }
        result = 1;
    }
    anim->SetFrame(newFrame);
    return result;
}

extern "C" u32 AnimRewindFrame(nw4r::lyt::AnimTransform* anim, float delta) {
    // Single-exit form: result flag stays in a saved register and the frame
    // is stored once at the end (retail has no early return).
    float newFrame = anim->GetFrame() - delta;
    u32 result = 0;
    if (CDeviceVI::isTvFormatPal()) {
        newFrame -= 0.2f;
    }
    if (newFrame <= 0.0f) {
        if (anim->IsLoopData()) {
            // Builtin u16->f32 conversion emits retail's exact 0x4330 stack
            // trick; MWCC pools the same 2^52 magic from this cast in first-use
            // order, so no named pool object is needed here.
            newFrame = static_cast<float>(anim->GetFrameSize()) - 1.0f;
        } else {
            newFrame = 0.0f;
        }
        result = 1;
    }
    anim->SetFrame(newFrame);
    return result;
}

extern "C" void AccumPaneWorldPos(nw4r::math::VEC3* output, nw4r::lyt::Pane* pane) {
    output->x = 0.0f;
    output->y = 0.0f;
    output->z = 0.0f;
    if (pane == NULL) return;

    output->x = pane->GetTranslate().x;
    output->y = pane->GetTranslate().y;
    output->z = pane->GetTranslate().z;

    nw4r::lyt::Pane* parent = pane->GetParent();
    nw4r::math::VEC3 accum;
    accum.x = 0.0f;
    accum.y = 0.0f;
    accum.z = 0.0f;
    if (parent == NULL) goto add_out;

    accum.x = parent->GetTranslate().x;
    accum.y = parent->GetTranslate().y;
    accum.z = parent->GetTranslate().z;

    nw4r::lyt::Pane* grandparent = parent->GetParent();
    nw4r::math::VEC3 temp;
    temp.x = 0.0f;
    temp.y = 0.0f;
    temp.z = 0.0f;
    if (grandparent == NULL) goto add_accum;

    temp.x = grandparent->GetTranslate().x;
    temp.y = grandparent->GetTranslate().y;
    temp.z = grandparent->GetTranslate().z;

    nw4r::lyt::Pane* ggp = grandparent->GetParent();
    nw4r::math::VEC3 tmp2;
    code80135FDC_setVec3(&tmp2, 0.0f, 0.0f, 0.0f);
    if (ggp == NULL) goto add_temp;

    copyVEC3(&tmp2, &ggp->GetTranslate());
    nw4r::math::VEC3 tmp3;
    AccumPaneWorldPos(&tmp3, ggp->GetParent());
    Vec3AddInto(&tmp2, &tmp3);

add_temp:
    nw4r::math::VEC3Add(&temp, &temp, &tmp2);
add_accum:
    nw4r::math::VEC3Add(&accum, &accum, &temp);
add_out:
    nw4r::math::VEC3Add(output, output, &accum);
}

// These helpers stay out-of-line in retail (bl relocs at every call site).
// The TU's -inline noauto flag expresses that policy for all same-TU helpers.
extern "C" nw4r::math::VEC3* code80135FDC_setVec3(nw4r::math::VEC3* self, float a, float b, float c) {
    self->x = a;
    self->y = b;
    self->z = c;
    return self;
}

extern "C" void Vec3AddInto(nw4r::math::VEC3* output,
                             const nw4r::math::VEC3* value) {
    nw4r::math::VEC3Add(output, output, value);
}

extern "C" void AccumPaneWorldPosB(nw4r::math::VEC3* output, nw4r::lyt::Pane* node) {
    // Single load: retail keeps the zero constant live in f1 across the whole
    // function (one lfs up front, stfs at every init site).
    float z = 0.0f;
    output->x = z;
    output->y = z;
    output->z = z;
    if (node != NULL && node->GetParent() != NULL) {
        output->x = node->GetTranslate().x;
        output->y = node->GetTranslate().y;
        output->z = node->GetTranslate().z;

        // Stack-slot order matches retail: rec2 @0x8, recurse @0x14,
        // temp @0x20, accum @0x2C (declared in that order).
        nw4r::math::VEC3 rec2;
        nw4r::math::VEC3 recurse;
        nw4r::math::VEC3 temp;
        nw4r::math::VEC3 accum;

        nw4r::lyt::Pane* parent = node->GetParent();
        accum.x = z;
        accum.y = z;
        accum.z = z;

        if (parent != NULL) {
            nw4r::lyt::Pane* grandparent = parent->GetParent();
            // Retail re-tests the same pointers redundantly; each re-test
            // reuses the cached register/cr0 instead of reloading.
            if (grandparent != NULL) {
                accum.x = parent->GetTranslate().x;
                accum.y = parent->GetTranslate().y;
                accum.z = parent->GetTranslate().z;

                temp.x = z;
                temp.y = z;
                temp.z = z;

                if (grandparent != NULL) {
                    nw4r::lyt::Pane* ggpp = grandparent->GetParent();
                    if (ggpp != NULL) {
                        temp.x = grandparent->GetTranslate().x;
                        temp.y = grandparent->GetTranslate().y;
                        temp.z = grandparent->GetTranslate().z;

                        code80135FDC_setVec3(&recurse, z, z, z);
                        if (ggpp != NULL) {
                            if (ggpp->GetParent() != NULL) {
                                copyVEC3(&recurse, &ggpp->GetTranslate());
                                AccumPaneWorldPosB(&rec2, ggpp->GetParent());
                                Vec3AddInto(&recurse, &rec2);
                            }
                        }
                        nw4r::math::VEC3Add(&temp, &temp, &recurse);
                    }
                }
                nw4r::math::VEC3Add(&accum, &accum, &temp);
            }
        }
        nw4r::math::VEC3Add(output, output, &accum);
    } else {
        goto done;
    }
done:;
}

extern "C" void ScalePaneWorldPos(nw4r::math::VEC3* output, nw4r::lyt::Pane* node,
                              nw4r::lyt::Pane* scale, nw4r::lyt::Pane* offset) {
    output->x = 0.0f;
    output->y = 0.0f;
    output->z = 0.0f;
    if (node != NULL) {
        if (node->GetParent() != NULL) {
            output->x = node->GetTranslate().x;
            output->y = node->GetTranslate().y;
            output->z = node->GetTranslate().z;

            nw4r::lyt::Pane* parent = node->GetParent();
            nw4r::math::VEC3 recurse;
            nw4r::math::VEC3 tmp2;
            nw4r::math::VEC3 temp;
            nw4r::math::VEC3 accum;
            accum.x = 0.0f;
            accum.y = 0.0f;
            accum.z = 0.0f;
            if (parent != NULL) {
                nw4r::lyt::Pane* grandparent = parent->GetParent();
                if (grandparent != NULL) {
                    accum.x = parent->GetTranslate().x;
                    accum.y = parent->GetTranslate().y;
                    accum.z = parent->GetTranslate().z;

                    temp.x = 0.0f;
                    temp.y = 0.0f;
                    temp.z = 0.0f;
                    if (grandparent != NULL) {
                        nw4r::lyt::Pane* ggp = grandparent->GetParent();
                        if (ggp != NULL) {
                            temp.x = grandparent->GetTranslate().x;
                            temp.y = grandparent->GetTranslate().y;
                            temp.z = grandparent->GetTranslate().z;

                            tmp2.x = 0.0f;
                            tmp2.y = 0.0f;
                            tmp2.z = 0.0f;
                            if (ggp != NULL) {
                                nw4r::lyt::Pane* gggp = ggp->GetParent();
                                if (gggp != NULL) {
                                    tmp2.x = ggp->GetTranslate().x;
                                    tmp2.y = ggp->GetTranslate().y;
                                    tmp2.z = ggp->GetTranslate().z;
                                    AccumPaneWorldPosB(&recurse, gggp);
                                    nw4r::math::VEC3Add(&tmp2, &tmp2, &recurse);
                                }
                            }
                            nw4r::math::VEC3Add(&temp, &temp, &tmp2);
                        }
                    }
                    nw4r::math::VEC3Add(&accum, &accum, &temp);
                }
            }
            nw4r::math::VEC3Add(output, output, &accum);
        }
    }
    output->x *= scale->GetScale().x;
    nw4r::math::VEC3Add(output, output, &offset->GetTranslate());
}

extern "C" void PaneSetVtxColorByName(nw4r::lyt::Layout* a, const char* b, u32 c) {
    if (a == NULL) return;
    nw4r::lyt::Pane* result = a->GetRootPane()->FindPaneByName(b, true);
    if (result == NULL) return;

    { u32 v = c; result->SetVtxColor(0, *(nw4r::ut::Color*)&v); }
    { u32 v = c; result->SetVtxColor(2, *(nw4r::ut::Color*)&v); }
    { u32 v = c; result->SetVtxColor(1, *(nw4r::ut::Color*)&v); }
    { u32 v = c; result->SetVtxColor(3, *(nw4r::ut::Color*)&v); }
}

extern "C" void PaneSetVtxColorAll(nw4r::lyt::Pane* obj, u32 value) {
    // Retail reloads the vtable for every call and keeps `obj` (r30) and
    // `value` (r31) live; each call gets its own stack slot. Written as a
    // true virtual call so MWCC emits the r12 -> r12 dispatch sequence.
    u32 vInit = value;
    if (obj == NULL) return;
    { u32 v = vInit; ((nw4r::lyt::Pane*)obj)->SetVtxColor(0, *(nw4r::ut::Color*)&v); }
    { u32 v = value; ((nw4r::lyt::Pane*)obj)->SetVtxColor(2, *(nw4r::ut::Color*)&v); }
    { u32 v = value; ((nw4r::lyt::Pane*)obj)->SetVtxColor(1, *(nw4r::ut::Color*)&v); }
    { u32 v = value; ((nw4r::lyt::Pane*)obj)->SetVtxColor(3, *(nw4r::ut::Color*)&v); }
}

extern "C" void PaneSetVtxColorPairsByName(nw4r::lyt::Layout* a, const char* b, u32 c, u32 d) {
    if (a == NULL) return;
    nw4r::lyt::Pane* result = a->GetRootPane()->FindPaneByName(b, true);
    if (result == NULL) return;

    u32 v0 = c;
    u32 v1 = c;
    u32 v2 = d;
    u32 v3 = d;
    result->SetVtxColor(0, *(nw4r::ut::Color*)&v0);
    result->SetVtxColor(2, *(nw4r::ut::Color*)&v2);
    result->SetVtxColor(1, *(nw4r::ut::Color*)&v1);
    result->SetVtxColor(3, *(nw4r::ut::Color*)&v3);
}

extern "C" void PaneSetVtxColorPairs(nw4r::lyt::Pane* a, u32 b, u32 c) {
    // Retail order: idx0=b, idx2=c, idx1=b, idx3=c. The parameter addresses
    // are passed directly, so MWCC re-stores each value into a fresh stack
    // slot per call (no shared temp).
    // Four distinct temporaries - retail gives every call its own stack
    // slot and keeps obj/b/c in saved regs r29/r30/r31.
    if (a == NULL) return;
    { u32 v = b; ((nw4r::lyt::Pane*)a)->SetVtxColor(0, *(nw4r::ut::Color*)&v); }
    { u32 v = c; ((nw4r::lyt::Pane*)a)->SetVtxColor(2, *(nw4r::ut::Color*)&v); }
    { u32 v = b; ((nw4r::lyt::Pane*)a)->SetVtxColor(1, *(nw4r::ut::Color*)&v); }
    { u32 v = c; ((nw4r::lyt::Pane*)a)->SetVtxColor(3, *(nw4r::ut::Color*)&v); }
}

extern "C" void PaneSetTexPaletteByName(nw4r::lyt::Layout* a, const char* b, TPLPalette* palette) {
    if (a == NULL) return;
    nw4r::lyt::Pane* res = a->GetRootPane()->FindPaneByName(b, true);
    if (res == NULL) return;
    if (palette == NULL) return;

    // res is a pane; its 0x68 vtable slot (Pane::GetMaterial) returns the material
    nw4r::lyt::Material* mat = res->GetMaterial();

    nw4r::lyt::TexMap texMap(palette, 0);
    if (mat->GetTextureNum() == 0) {
        nw4r::db::Panic((const char*)sNw4rMaterialFile, 0x88, (const char*)sNw4rTexMapAssertMsg);
    }
    mat->GetTexMapAry()[0] = texMap;
}

extern "C" void PaneSetTexPalette(nw4r::lyt::Pane* a, TPLPalette* palette) {
    if (a != NULL) {
        if (palette != NULL) {
            nw4r::lyt::Material* mat = ((nw4r::lyt::Pane*)a)->GetMaterial();

            nw4r::lyt::TexMap texMap(palette, 0);
            if (mat->GetTextureNum() == 0) {
                nw4r::db::Panic((const char*)sNw4rMaterialFile, 0x88, (const char*)sNw4rTexMapAssertMsg);
            }
            mat->GetTexMapAry()[0] = texMap;
        }
    }
}

void playUISound(u32 arg) {
    cf::CfSoundMan::playActorSound(0, arg, 0, 0, 0.6f);
}

extern "C" void code80135FDC_thunk_BFE8C(u8* arg) {
    CfSoundMan_StopSlotByMode(0, (u32)(uintptr_t)arg, 0);
}

struct Table_80500188 {
    u32 words[14];
};

extern "C" u16 GetU16PairEntry(u32 idx) {
    struct Table_80500188 t;
    u32 n = idx - 1;
    t = sThresholdPairTable;
    return ((u16*)&t)[n];
}

// Retail keeps the table copy as a CTR loop (no unroll) but unrolls the
// search 7x: size-leaning codegen for the straight-line copy.
#pragma optimize_for_size on
extern "C" u32 SearchThresholdIdx(u32 val) {
    u32 words[28];
    // Two-per-iteration 1-indexed walking copy from -1 (retail -4 bias
    // with lwz+4/lwzu+8 and stw+4/stwu+8 under CTR-14).
    u32* d = words - 1;
    u32* s = sThresholdLookup - 1;
    u32* dd = &d[1];
    for (int i = 0; i < 14; i++) {
        const u32 a = s[1];
        const u32 b = s[2];
        *dd++ = a;
        *dd++ = b;
        s += 2;
    }
    for (int i = 27; i >= 0; i--) {
        if (val >= words[i]) return i;
    }
    return 0;
}
#pragma optimize_for_size off

extern "C" u32 CheckEntryConditions(const char* name, u32 id) {
    u32 flag = 0;
    if (CtrlRemote_TouchBitByArg(id + 0x220) != 0) return 0;

    // One reusable column pointer: retail rematerializes the table base per
    // block and cycles a single register for the column offset.
    // Eight BDAT cells; declared up front so frame slots match retail.
    u16 v1;
    u16 v2;
    u8 v3;
    u16 v4;
    u16 v5;
    u16 v6;
    u16 v7;
    u8 v8;

    const char* col = &lbl_eu_80500664[0x2A];
    if (name == NULL) {
        v1 = 0;
    } else {
        Bdat_GetTable_AA34(lbl_eu_80500664);
        void* result = getBdatStringColumnValue((void*)name, col, (const char*)id);
        v1 = *(u16*)&result;
    }
    col = &lbl_eu_80500664[0x30];
    if (name == NULL) {
        v2 = 0;
    } else {
        Bdat_GetTable_AA34(lbl_eu_80500664);
        void* result = getBdatStringColumnValue((void*)name, col, (const char*)id);
        v2 = *(u16*)&result;
    }
    col = &lbl_eu_80500664[0x3C];
    if (name == NULL) {
        v3 = 0;
    } else {
        Bdat_GetTable_AA34(lbl_eu_80500664);
        void* result = getBdatStringColumnValue((void*)name, col, (const char*)id);
        v3 = *(u8*)&result;
    }
    col = &lbl_eu_80500664[0x47];
    if (name == NULL) {
        v4 = 0;
    } else {
        Bdat_GetTable_AA34(lbl_eu_80500664);
        void* result = getBdatStringColumnValue((void*)name, col, (const char*)id);
        v4 = *(u16*)&result;
    }
    col = &lbl_eu_80500664[0x52];
    if (name == NULL) {
        v5 = 0;
    } else {
        Bdat_GetTable_AA34(lbl_eu_80500664);
        void* result = getBdatStringColumnValue((void*)name, col, (const char*)id);
        v5 = *(u16*)&result;
    }
    col = &lbl_eu_80500664[0x5D];
    if (name == NULL) {
        v6 = 0;
    } else {
        Bdat_GetTable_AA34(lbl_eu_80500664);
        void* result = getBdatStringColumnValue((void*)name, col, (const char*)id);
        v6 = *(u16*)&result;
    }
    col = &lbl_eu_80500664[0x68];
    if (name == NULL) {
        v7 = 0;
    } else {
        Bdat_GetTable_AA34(lbl_eu_80500664);
        void* result = getBdatStringColumnValue((void*)name, col, (const char*)id);
        v7 = *(u16*)&result;
    }
    col = &lbl_eu_80500664[0x73];
    if (name == NULL) {
        v8 = 0;
    } else {
        Bdat_GetTable_AA34(lbl_eu_80500664);
        void* result = getBdatStringColumnValue((void*)name, col, (const char*)id);
        v8 = *(u8*)&result;
    }

    // Retail sums the eight cells in this exact pairwise grouping.
    if (((u32)(v5 + v4) + (u32)(v8 + v2)) + ((u32)(v6 + v3) + (u32)(v7 + v1)) == 0) {
        return 0;
    }
    if ((u16)CtrlRemote_TouchBitByArg(0x20) < v1) return 0;
    if (v2 != 0 && (u8)CtrlRemote_TouchBitByArg(v2 + 0x220) < 0xFE) return 0;
    if ((u16)CtrlRemote_TouchBitByArg(v3 + 0x21) < v4) return 0;

    flag = 0;
    if (v5 != 0) {
        void* result = getBdatStringColumnValue((void*)::cf::CfBdat::spFldNpcListFileData, &lbl_eu_80500664[0x7E], (const char*)v5);
        u16 val = *(u16*)&result;
        if (CtrlRemote_TouchBitByArg(val + 0xA20) == 0) {
            flag = 1;
        }
    }
    if (flag != 0) return 0;

    flag = 0;
    if (v6 != 0) {
        void* result = getBdatStringColumnValue((void*)::cf::CfBdat::spFldNpcListFileData, &lbl_eu_80500664[0x7E], (const char*)v6);
        u16 val = *(u16*)&result;
        if (CtrlRemote_TouchBitByArg(val + 0xA20) == 0) {
            flag = 1;
        }
    }
    if (flag != 0) return 0;

    // subf/cntlzw/srwi booleanization, as in retail
    return ((u16)CtrlRemote_TouchBitByArg(v7 + 0x608) - v8) == 0;
}

extern "C" u32 func_80138574(const char* name, u32 id) {
    void* player = cf::CfGameManager::getPlayer(0);
    if (player == NULL) return 0;

    u8 v1 = 0;
    if (name != NULL) {
        Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x87], (const char*)id);
        v1 = *(u8*)&result;
    }
    if (v1 != 0 && v1 != *(u16*)((u8*)player + 0x8C)) return 0;

    u16 v2 = 0;
    if (name != NULL) {
        Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x8F], (const char*)id);
        v2 = *(u16*)&result;
    }
    u16 v3 = 0;
    if (name != NULL) {
        Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x9A], (const char*)id);
        v3 = *(u16*)&result;
    }

    u32 check = CtrlRemote_TouchBitByArg(0x20) & 0xFFFF;
    if (v2 > check || check > v3) return 0;

    u8 v4 = 0;
    if (name != NULL) {
        Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0xA5], (const char*)id);
        v4 = *(u8*)&result;
    }
    if (v4 == 5) {
        void* fp = getFP(&lbl_eu_80500664[0xAA]);
        u16 key = 0;
        if (name != NULL) {
            Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
            void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0xB9], (const char*)id);
            key = *(u16*)&result;
        }
        u8 v5 = 0;
        if (fp != NULL) {
            Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
            void* result = getBdatStringColumnValue(fp, &lbl_eu_80500664[0xC0], (const char*)key);
            v5 = *(u8*)&result;
        }
        if (v5 != 0 && v5 != CfT_DispatchFrameCounts()) return 0;

        u8 v6 = 0;
        if (fp != NULL) {
            Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
            void* result = getBdatStringColumnValue(fp, &lbl_eu_80500664[0xC8], (const char*)key);
            v6 = *(u8*)&result;
        }
        if (v6 != 0 && v6 != (u16)getReloadParam0()) return 0;
    } else {
        u16 tick = 0;
        u16 other = 0;
        CfT_PlayClockSnapshot(&tick, &other);
        switch (tick) {
        case 0:
        case 1:
        case 2: {
            u8 v = 0;
            if (name != NULL) {
                Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
                void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x113], (const char*)id);
                v = *(u8*)&result;
            }
            if (v == 0) return 0;
            break;
        }
        case 3:
        case 4:
        case 5: {
            u8 v = 0;
            if (name != NULL) {
                Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
                void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x10A], (const char*)id);
                v = *(u8*)&result;
            }
            if (v == 0) return 0;
            break;
        }
        case 6:
        case 7:
        case 8: {
            u8 v = 0;
            if (name != NULL) {
                Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
                void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x101], (const char*)id);
                v = *(u8*)&result;
            }
            if (v == 0) return 0;
            break;
        }
        case 9:
        case 10:
        case 11: {
            u8 v = 0;
            if (name != NULL) {
                Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
                void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0xF8], (const char*)id);
                v = *(u8*)&result;
            }
            if (v == 0) return 0;
            break;
        }
        case 12:
        case 13:
        case 14: {
            u8 v = 0;
            if (name != NULL) {
                Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
                void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0xEE], (const char*)id);
                v = *(u8*)&result;
            }
            if (v == 0) return 0;
            break;
        }
        case 15:
        case 16:
        case 17: {
            u8 v = 0;
            if (name != NULL) {
                Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
                void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0xE4], (const char*)id);
                v = *(u8*)&result;
            }
            if (v == 0) return 0;
            break;
        }
        case 18:
        case 19:
        case 20: {
            u8 v = 0;
            if (name != NULL) {
                Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
                void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0xDA], (const char*)id);
                v = *(u8*)&result;
            }
            if (v == 0) return 0;
            break;
        }
        default: {
            u8 v = 0;
            if (name != NULL) {
                Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
                void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0xD0], (const char*)id);
                v = *(u8*)&result;
            }
            if (v == 0) return 0;
            break;
        }
        }
    }

    u16 v7 = 0;
    if (name != NULL) {
        Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x11C], (const char*)id);
        v7 = *(u16*)&result;
    }
    if (v7 != 0) {
        u8 v8 = 0;
        if (name != NULL) {
            Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
            void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x125], (const char*)id);
            v8 = *(u8*)&result;
        }
        if ((CtrlRemote_TouchBitByArg(v7 + 0x220) & 0xFF) != v8) return 0;
    }
    u8 v9 = 0;
    if (name != NULL) {
        Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x12B], (const char*)id);
        v9 = *(u8*)&result;
    }
    if (v9 != 0) {
        u8 v10 = 0;
        if (name != NULL) {
            Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
            void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x135], (const char*)id);
            v10 = *(u8*)&result;
        }
        if ((CtrlRemote_TouchBitByArg(v9 + 0x798) & 0xFF) != v10) return 0;
    }
    u8 v11 = 0;
    if (name != NULL) {
        Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x13C], (const char*)id);
        v11 = *(u8*)&result;
    }
    if (v11 != 0) {
        u16 v12 = 0;
        if (name != NULL) {
            Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
            void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x147], (const char*)id);
            v12 = *(u16*)&result;
        }
        if ((u16)CtrlRemote_TouchBitByArg(v11 + 0x21) < v12) return 0;
    }
    u16 v13 = 0;
    if (name != NULL) {
        Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x14F], (const char*)id);
        v13 = *(u16*)&result;
    }
    if (v13 != 0) {
        if (CtrlRemote_TouchBitByArg(v13 + 0xA20) == 0) return 0;
    }
    u8 v14 = 0;
    if (name != NULL) {
        Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x15A], (const char*)id);
        v14 = *(u8*)&result;
    }
    if (v14 != 0) {
        u16 v15 = 0;
        if (name != NULL) {
            Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
            void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x164], (const char*)id);
            v15 = *(u16*)&result;
        }
        if ((u16)CtrlRemote_TouchBitByArg(v14 + 0x7FC) < v15) return 0;
    }
    u8 v16 = 0;
    if (name != NULL) {
        Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x16B], (const char*)id);
        v16 = *(u8*)&result;
    }
    if (v16 != 0) {
        if (name != NULL) {
            Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
            getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x175], (const char*)id);
        }
        if (CtrlRemote_TouchBitByArg(v16 + 0x2578) == 0) return 0;
    }
    return 1;
}

extern "C" char* GetBdatNameByNumId(const char* str) {
    int v = atoi(str);
    if (v <= 0) return (char*)str;
    const char* base = lbl_eu_80500664;
    const char* col = base + 0x17C;
    const char* file = base + 0x181;
    Bdat_GetTable_AA34(base);
    void* fp = getFP(file);
    return (char*)(getBdatStringColumnValue)(fp, col, v);
}

extern "C" int BdatGetSexFlag(u32 row) {
    const char* col = &lbl_eu_80500664[0x18A];
    u8* src = (u8*)::cf::CfBdat::spFldNpcListFileData;
    if (src == 0) {
        return 0;
    }
    Bdat_GetTable_AA34((const char*)row);
    void* result = (void*)(getBdatStringColumnValue)((void*)src, col, row);
    return *(u8*)&result;
}

extern "C" u32 MapSexFlagGroup(u32 row) {
    u8 v = 0;
    if (::cf::CfBdat::spFldNpcListFileData != 0) {
        Bdat_GetTable_AA34((const char*)row);
        void* result = (void*)(getBdatStringColumnValue)((void*)::cf::CfBdat::spFldNpcListFileData, &lbl_eu_80500664[0x18A], row);
        v = *(u8*)&result;
    }
    u32 result = 0;
    switch (v) {
    case 0:
    case 1:
        result = 1;
        break;
    case 2:
    case 3:
        result = 2;
        break;
    case 4:
    case 5:
        result = 3;
        break;
    case 6:
    case 7:
        result = 4;
        break;
    case 8:
        result = 5;
        break;
    case 9:
    case 10:
        result = 2;
        break;
    default:
        break;
    }
    return result;
}

extern "C" char* MakeTplNameSysFile(u32 row) {
    if (gSysFileListBdat == 0) {
        Bdat_GetTable_AA34((const char*)row);
        gSysFileListBdat = (u32)getFP(&lbl_eu_80500664[0x192]);
    }
    const char* col = &lbl_eu_80500664[0x19F];
    void* fp = (void*)gSysFileListBdat;
    void* result;
    if (fp == NULL) {
        result = NULL;
    } else {
        // retail: bl Bdat_GetTable_AA34 with no arg move - r3 already holds
        // &lbl_eu_80500664[0] from the shared base computation with col
        Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
        result = (void*)(getBdatStringColumnValue)(fp, col, row);
    }
    sprintf(&gTplNameBuffer[0], &lbl_eu_80500664[0x1A8], result);
    return &gTplNameBuffer[0];
}

extern "C" char* MakeIconTplName(u32 row) {
    // Register mapping in retail: row -> r29, file pointer -> r30, column
    // pointer -> r31; the column pointer is hoisted above the fp check.
    if (gSysIconListBdat == 0) {
        Bdat_GetTable_AA34((const char*)row);
        gSysIconListBdat = (u32)getFP(&lbl_eu_80500664[0x1AF]);
    }
    char* result = NULL;
    const char* col = &lbl_eu_80500664[0x1C7];
    void* fp = (void*)gSysIconListBdat;
    if (fp != NULL) {
        Bdat_GetTable_AA34((const char*)row);
        result = (char*)(getBdatStringColumnValue)(fp, col, row);
    }
    sprintf(&gIconNameBuffer[0], &lbl_eu_80500664[0x1A8], result);
    return &gIconNameBuffer[0];
}

extern "C" void cancel__11CDeviceFileFP11CFileHandle(void*);
void closeFileHandle(CFileHandle** handlePtr) {
    if (*handlePtr) {
        cancel__11CDeviceFileFP11CFileHandle(*handlePtr);
        *handlePtr = 0;
    }
}

extern "C" void func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(nw4r::lyt::ArcResourceAccessor* accessor) {
    if (accessor != NULL) {
        void* resource = accessor->Detach();
        if (accessor != NULL) {
            delete accessor;
        }
        if (resource != NULL) {
            deallocate__Q23mtl10MemManagerFPv(resource);
        }
    }
}

// Cast-only view of the item-data object returned by CtrlObjectParam_GetCharEntry: it has a
// per-item flag word at 0x176C (1 = already recorded).
struct CDataObj {
    u8 field_0x00[0x176C];
    s32 field_0x176C;
};

// List blob returned by CtrlObjectParam_GetSlotTableBase() (+4): 7 words (listA[3] + listB[4],
// contiguous so listB[i-3] aliases listA[i]) then a gap and 3 trailing
// words (listC).
struct ListData {
    u32 listA[3];      // 0x0
    u32 listB[4];      // 0xC
    u32 field_0x1C;    // 0x1C
    u32 field_0x20;    // 0x20
    u32 listC[3];      // 0x24
};

// Collect up to 7 u32 entries from the global list; entries > 0 (filtered by
// the per-item flag when arg is set) are appended to the u8 list, then the 3
// trailing words are appended to the u16 list.
extern "C" void BuildSlotItemLists(int arg) {
    gCollectedFlagCount = 0;
    gSlotItemCount = 0;
    ListData* list = (ListData*)((u8*)CtrlObjectParam_GetSlotTableBase() + 4);
    for (u8 i = 0; i < 7; i++) {
        u32 v;
        if (i < 3) {
            v = list->listA[i];
        } else {
            v = list->listB[i - 3];
        }
        if ((s32)v > 0) {
            if (arg != 0) {
                CDataObj* obj = (CDataObj*)CtrlObjectParam_GetCharEntry(v & 0xFFFF);
                if (obj->field_0x176C != 1) {
                    u8 n = gCollectedFlagCount;
                    ((u8*)&gCollectedFlags)[n] = (u8)v;
                    gCollectedFlagCount = n + 1;
                }
            } else {
                u8 n = gCollectedFlagCount;
                ((u8*)&gCollectedFlags)[n] = (u8)v;
                gCollectedFlagCount = n + 1;
            }
        }
    }
    u16* list16 = (u16*)&gSlotItemIds;
    int c = gSlotItemCount;
    if ((s32)list->listC[0] > 0) {
        list16[c] = (u16)list->listC[0];
        c = c + 1;
        gSlotItemCount = c;
    }
    if ((s32)list->listC[1] > 0) {
        // explicit mask temps reproduce retail's clrlslwi/clrlwi re-masking
        // after the increment (block 1 needs none: c is fresh from lbz)
        int cm = c & 0xFF;
        list16[(u8)c] = (u16)list->listC[1];
        c = cm + 1;
        gSlotItemCount = c;
    }
    if ((s32)list->listC[2] > 0) {
        int cm2 = c & 0xFF;
        list16[(u8)c] = (u16)list->listC[2];
        c = cm2 + 1;
        gSlotItemCount = c;
    }
}



extern u8 gCollectedFlagCount;
extern "C" u8 code80135FDC_getByte_64077() { return gCollectedFlagCount; }

extern "C" u8 GetCollectedFlagWord8(u32 idx) {
    extern u16 gSlotItemIds[3];
    return (u8)(gSlotItemIds[(u8)idx]);
}

extern u8 gSlotItemCount;
extern "C" u8 code80135FDC_getByte_6407E() { return gSlotItemCount; }

extern "C" u16 BdatGetItemType(u32 row) {
    const char* col = &lbl_eu_80500664[0x1D2];
    u8* src = (u8*)::cf::CfBdat::spItmItemListFileData;
    if (src == 0) {
        return 0;
    }
    Bdat_GetTable_AA34((const char*)row);
    void* result = (void*)(getBdatStringColumnValue)((void*)src, col, row);
    return *(u16*)&result;
}

extern "C" u16 BdatGetItemId(u32 row) {
    const char* col = &lbl_eu_80500664[0x1DB];
    u8* src = (u8*)::cf::CfBdat::spItmItemListFileData;
    if (src == 0) {
        return 0;
    }
    Bdat_GetTable_AA34((const char*)row);
    void* result = (void*)(getBdatStringColumnValue)((void*)src, col, row);
    return *(u16*)&result;
}

extern "C" u32 LookupItemTypeTable(u32 row) {
    u32 v = 0;
    if (::cf::CfBdat::spItmItemListFileData != 0) {
        Bdat_GetTable_AA34((const char*)row);
        void* result = (void*)(getBdatStringColumnValue)((void*)::cf::CfBdat::spItmItemListFileData, &lbl_eu_80500664[0x1D2], row);
        v = *(u16*)&result;
    }
    switch (v) {
    case 0: v = (u32)lbl_eu_806640F0; break;
    case 1: v = (u32)::cf::CfBdat::spItmWpnListFileData; break;
    case 2: v = (u32)::cf::CfBdat::spBtlSkillListFileData; break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7: v = (u32)::cf::CfBdat::spItmEquipListFileData; break;
    case 8: v = (u32)::cf::CfBdat::spItmCrystalListFileData; break;
    case 9: v = (u32)::cf::CfBdat::spItmCollectListFileData; break;
    case 10: v = (u32)::cf::CfBdat::spItmMaterialListFileData; break;
    case 11: v = (u32)::cf::CfBdat::spItmValuableListFileData; break;
    case 12:
    case 13: v = (u32)::cf::CfBdat::spItmArtsListFileData; break;
    default: break;
    }
    return v;
}

extern "C" char* BuildItemNameStr(u32 row) {
    u32 a = LookupItemTypeTable(row);
    const char* col1DB = &lbl_eu_80500664[0x1DB];
    u16 b;
    if (::cf::CfBdat::spItmItemListFileData == 0) {
        b = 0;
    } else {
        Bdat_GetTable_AA34((const char*)a);
        void* r = (void*)(getBdatStringColumnValue)((void*)::cf::CfBdat::spItmItemListFileData, col1DB, row);
        b = *(u16*)&r;
    }
    u16 c;
    const char* col1D2 = &lbl_eu_80500664[0x1D2];
    if (::cf::CfBdat::spItmItemListFileData == 0) {
        c = 0;
    } else {
        Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
        void* r = (void*)(getBdatStringColumnValue)((void*)::cf::CfBdat::spItmItemListFileData, col1D2, row);
        c = *(u16*)&r;
    }
    void* d0 = 0;
    const char* col17C = &lbl_eu_80500664[0x17C];
    if (a == 0) {
        d0 = 0;
    } else {
        Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
        d0 = getBdatStringColumnValue((void*)a, col17C, (const char*)(u32)b);
    }
    sprintf(&gItemNameBuffer[0], &lbl_eu_80500664[0], d0);
    if (c == 3) {
        u8 d;
        void* rv;
        if (::cf::CfBdat::spItmItemListFileData == 0) {
            d = 0;
        } else {
            Bdat_GetTable_AA34(&lbl_eu_80500664[0]);
            rv = (void*)(getBdatStringColumnValue)((void*)::cf::CfBdat::spItmItemListFileData, &lbl_eu_80500664[0x1E2], row);
            d = *(u8*)&rv;
        }
        Bdat_GetTable_AA34((const char*)rv);
        void* fp = getFP(&lbl_eu_80500664[0x1EB]);
        void* r2 = getBdatStringColumnValue(fp, col17C,
                                           (const char*)(0x1E - (d - 1)));
        sprintf(&gItemNameBuffer[0], &lbl_eu_80500664[0x1F4], &gItemNameBuffer[0], r2);
    }
    return &gItemNameBuffer[0];
}

extern "C" GXColorS10 PaneMatGetS16QuadByName(nw4r::lyt::Layout* layout, const char* name, u32 idx) {
    nw4r::lyt::Pane* owner = layout->GetRootPane();
    nw4r::lyt::Pane* cur = owner->FindPaneByName(name, true);
    if (cur == 0) {
        GXColorS10 r;
        r.r = 0; r.g = 0; r.b = 0; r.a = 0;
        return r;
    }
    u8* data = (u8*)cur->GetMaterial();
    if (data == NULL) {
        GXColorS10 r;
        r.r = 0; r.g = 0; r.b = 0; r.a = 0;
        return r;
    }
    if (idx >= 3) {
        nw4r::db::Panic((const char*)sNw4rTevColorAssertFile, 0x8C, (const char*)sNw4rTevColorAssertMsg);
    }
    u8* p = data + ((u8)idx << 3) + 0x10;
    GXColorS10 src = *(GXColorS10*)p;
    GXColorS10 r = { src.r, src.g, src.b, src.a };
    return r;
}

extern "C" GXColorS10 PaneMatGetS16Quad(nw4r::lyt::Pane* owner, u32 idx) {
    GXColorS10 r = {0, 0, 0, 0};
    if (owner == NULL) return r;
    u8* data = (u8*)owner->GetMaterial();
    if (data != NULL) {
        if (idx >= 3) {
            nw4r::db::Panic((const char*)sNw4rTevColorAssertFile, 0x8C, (const char*)sNw4rTevColorAssertMsg);
        }
        u8* p = data + idx * 8 + 0x10;
        r.r = *(s16*)(p + 0);
        r.g = *(s16*)(p + 2);
        r.b = *(s16*)(p + 4);
        r.a = *(s16*)(p + 6);
    }
    return r;
}

extern "C" void PaneMatSetS16QuadByName(nw4r::lyt::Layout* layout, const char* name, const GXColorS10* src, u32 idx) {
    nw4r::lyt::Pane* owner = layout->GetRootPane();
    nw4r::lyt::Pane* res = owner->FindPaneByName(name, true);
    if (res == 0) return;
    if (res == 0) return;
    u8* data = (u8*)res->GetMaterial();
    if (data == NULL) return;
    if (idx >= 3) {
        nw4r::db::Panic((const char*)lbl_eu_8052E590, 0x8F, (const char*)lbl_eu_8052E568);
    }
    // clrlslwi r0, idx, 24, 3 => the multiply runs on an 8-bit-masked index
    // ((idx & 0xFF) * 8), same as PaneMatSetS16Quad.
    u8 i8 = (u8)idx;
    s16* s = (s16*)src;
    s16* d = (s16*)(data + i8 * 8 + 0x10);
    s16 tb = s[1];
    s16 ta = s[0];
    d[0] = ta;
    ta = s[2];
    d[1] = tb;
    tb = s[3];
    d[2] = ta;
    d[3] = tb;
}

extern "C" void PaneMatSetS16Quad(nw4r::lyt::Pane* owner, const GXColorS10* src, u32 idx) {
    if (owner == NULL) return;
    u8* data = (u8*)owner->GetMaterial();
    if (data == NULL) return;
    if (idx >= 3) {
        nw4r::db::Panic((const char*)lbl_eu_8052E590, 0x8F, (const char*)lbl_eu_8052E568);
    }
    // clrlslwi r0, idx, 24, 3 => the multiply runs on an 8-bit-masked index
    // ((idx & 0xFF) * 8).
    u8 i8 = (u8)idx;
    s16* s = (s16*)src;
    s16* d = (s16*)(data + i8 * 8 + 0x10);
    s16 tb = s[1];
    s16 ta = s[0];
    d[0] = ta;
    ta = s[2];
    d[1] = tb;
    tb = s[3];
    d[2] = ta;
    d[3] = tb;
}

extern "C" void PaneMatSetTevColorsByName(nw4r::lyt::Layout* layout, const char* name, const GXColorS10* src1,
                                          const GXColorS10* src2) {
    nw4r::lyt::Pane* owner = layout->GetRootPane();
    nw4r::lyt::Pane* res = owner->FindPaneByName(name, true);
    if (res == NULL) return;
    if (res == NULL) return;
    s16* data = (s16*)res->GetMaterial();
    if (data == NULL) return;
    s16* s1 = (s16*)src1;
    s16* s2 = (s16*)src2;
    data[8] = s1[0];
    data[9] = s1[1];
    data[10] = s1[2];
    data[11] = s1[3];
    data[12] = s2[0];
    data[13] = s2[1];
    data[14] = s2[2];
    data[15] = s2[3];
}

extern "C" void PaneMatSetTevColors(nw4r::lyt::Pane* owner, const GXColorS10* src1, const GXColorS10* src2) {
    if (owner == NULL) return;
    s16* data = (s16*)owner->GetMaterial();
    if (data == NULL) return;
    s16* s1 = (s16*)src1;
    s16* s2 = (s16*)src2;
    data[8] = s1[0];
    data[9] = s1[1];
    data[10] = s1[2];
    data[11] = s1[3];
    data[12] = s2[0];
    data[13] = s2[1];
    data[14] = s2[2];
    data[15] = s2[3];
}

extern "C" void PaneSetColorFieldSingle(nw4r::lyt::Layout* layout, const char* name, u32 src) {
    nw4r::lyt::Pane* owner = layout->GetRootPane();
    void* result = owner->FindPaneByName(name, true);
    // Retail emits THREE beq's on the same null test (dead-duplicated-test
    // family); the direct goto-gate with the tripled condition reproduces it.
    if (result == NULL) goto out;
    if (result == NULL) goto out;
    if (result == NULL) goto out;
    // 8-byte color field (0xDC..0xE3): both 32-bit words carry the same value
    u32 words[2] = { (u32)src, (u32)src };
    u8* d = (u8*)result;
    u8* s = (u8*)words;
    d[0xDC] = s[0];
    d[0xDD] = s[1];
    d[0xDE] = s[2];
    d[0xDF] = s[3];
    d[0xE0] = s[4];
    d[0xE1] = s[5];
    d[0xE2] = s[6];
    d[0xE3] = s[7];
out:
    return;
}

extern "C" void PaneSetColorFieldPair(nw4r::lyt::Layout* layout, const char* name, u32 a, u32 b) {
    nw4r::lyt::Pane* owner = layout->GetRootPane();
    void* result = owner->FindPaneByName(name, true);
    // Retail emits the null test three times (three beq to the epilogue).
    if (result != NULL) {
        if (result != NULL) {
            if (result != NULL) {
                // 8-byte color field (0xDC..0xE3) copied byte-by-byte from
                // two 32-bit locals spilled to the stack.
                // hi declared first: MWCC assigns the later slot to it,
                // matching retail (lo -> sp+8, hi -> sp+0xC).
                u32 hi = (u32)b;
                u32 lo = (u32)a;
                u8* d = (u8*)result;
                u8* p = (u8*)&lo;
                d[0xDC] = p[0];
                d[0xDD] = p[1];
                d[0xDE] = p[2];
                d[0xDF] = p[3];
                p = (u8*)&hi;
                d[0xE0] = p[0];
                d[0xE1] = p[1];
                d[0xE2] = p[2];
                d[0xE3] = p[3];
            }
        }
    }
}

extern "C" f32 BlendFloatAvgScale(u32 a, u32 b, u32 c, f32 d) {
    f32 fa = (f32)a;
    f32 fb = (f32)b;
    f32 fc = (f32)c;
    return (0.5f * (fa + fb) + fc) / d;
}

extern "C" void InitCol6LodTables(const char* arg) {
    Bdat_GetTable_AA34(arg);
    getFP(&lbl_eu_80500664[0x1F9]);
    UnkClass_80083298* gm = cf::CfGameManager::getGameSubManager();
    if (gm == NULL) return;

    for (u8 i = 0; i <= 0x63; i++) {
        deactivateLOD__8CTaskLODFv(i);
        void* snd = getScnHandle();
        ScnData_FwdB7DD4(snd, i);
        forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, i);
    }

    struct B21 {
        u32 w[5];
        u8 b;
    };
    B21 tab;
    // copy the 21-byte table as 5 words + 1 byte (a whole-struct assignment
    // would round the copy up to 24 bytes / 6 words)
    for (int i = 0; i < 5; i++) {
        tab.w[i] = ((const u32*)&sLodCharMap)[i];
    }
    tab.b = ((const u8*)&sLodCharMap)[20];
    u8* chars = (u8*)&tab;
    u8 j = 0;
    for (;;) {
        s8 ch = (s8)chars[j];
        if (ch < 0) break;
        activateLOD__8CTaskLODFv(ch);
        void* snd = getScnHandle();
        ScnData_FwdB7D9C(snd, ch);
        forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, ch);
        j++;
    }

    if (CtrlRemote_TouchBitByArg(0x7D0) == 1) {
        activateLOD__8CTaskLODFv(0x50);
        void* snd = getScnHandle();
        ScnData_FwdB7D9C(snd, 0x50);
        forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x50);
        activateLOD__8CTaskLODFv(0x63);
        snd = getScnHandle();
        ScnData_FwdB7D9C(snd, 0x63);
        forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x63);
        activateLOD__8CTaskLODFv(0x09);
        snd = getScnHandle();
        ScnData_FwdB7D9C(snd, 0x09);
        forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x09);
    }

    for (u8 i = 0; i < 4; i++) {
        u8 n = (u8)CtrlRemote_TouchBitByArg(i + 0x7FE);
        u32 base = (u32)((i + 1) * 10);
        for (u8 k = 1; k <= n; k++) {
            u8 v1 = (u8)(base + k);
            activateLOD__8CTaskLODFv(v1);
            void* snd = getScnHandle();
            ScnData_FwdB7D9C(snd, v1);
            forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, v1);

            u8 v2 = (u8)(k + 0x3C);
            activateLOD__8CTaskLODFv(v2);
            snd = getScnHandle();
            ScnData_FwdB7D9C(snd, v2);
            forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, v2);

            u8 v3 = (u8)(k + 0x46);
            deactivateLOD__8CTaskLODFv(v3);
            snd = getScnHandle();
            ScnData_FwdB7DD4(snd, v3);
            forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, v3);

            if (v2 == 0x3D) {
                activateLOD__8CTaskLODFv(0x55);
                snd = getScnHandle();
                ScnData_FwdB7D9C(snd, 0x55);
                forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x55);
            }
            if (v2 == 0x3E) {
                activateLOD__8CTaskLODFv(0x56);
                snd = getScnHandle();
                ScnData_FwdB7D9C(snd, 0x56);
                forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x56);
            }
            if (v2 == 0x3D) {
                activateLOD__8CTaskLODFv(0x57);
                snd = getScnHandle();
                ScnData_FwdB7D9C(snd, 0x57);
                forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x57);
            }
            if (v2 == 0x3D) {
                activateLOD__8CTaskLODFv(0x58);
                snd = getScnHandle();
                ScnData_FwdB7D9C(snd, 0x58);
                forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x58);
            }
            if (CtrlRemote_TouchBitByArg(0x20) >= 0x16E) {
                activateLOD__8CTaskLODFv(0x59);
                snd = getScnHandle();
                ScnData_FwdB7D9C(snd, 0x59);
                forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x59);
                if (v1 == 0x0E) {
                    activateLOD__8CTaskLODFv(0x5A);
                    snd = getScnHandle();
                    ScnData_FwdB7D9C(snd, 0x5A);
                    forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5A);
                }
            }
            if (v2 == 0x3D) {
                activateLOD__8CTaskLODFv(0x5B);
                snd = getScnHandle();
                ScnData_FwdB7D9C(snd, 0x5B);
                forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5B);
            }
            if (v2 == 0x3D) {
                activateLOD__8CTaskLODFv(0x5C);
                snd = getScnHandle();
                ScnData_FwdB7D9C(snd, 0x5C);
                forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5C);
            }
            if (v2 == 0x3E) {
                activateLOD__8CTaskLODFv(0x5D);
                snd = getScnHandle();
                ScnData_FwdB7D9C(snd, 0x5D);
                forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5D);
            }
            if (v2 == 0x3F) {
                activateLOD__8CTaskLODFv(0x5E);
                snd = getScnHandle();
                ScnData_FwdB7D9C(snd, 0x5E);
                forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5E);
            }
            if (v2 == 0x40) {
                activateLOD__8CTaskLODFv(0x5F);
                snd = getScnHandle();
                ScnData_FwdB7D9C(snd, 0x5F);
                forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5F);
            }
            if (v2 == 0x40) {
                activateLOD__8CTaskLODFv(0x60);
                snd = getScnHandle();
                ScnData_FwdB7D9C(snd, 0x60);
                forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x60);
            }
            if (v2 == 0x41) {
                activateLOD__8CTaskLODFv(0x61);
                snd = getScnHandle();
                ScnData_FwdB7D9C(snd, 0x61);
                forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x61);
            }
            if (v2 == 0x41) {
                activateLOD__8CTaskLODFv(0x62);
                snd = getScnHandle();
                ScnData_FwdB7D9C(snd, 0x62);
                forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x62);
            }

            if (getLODData__8CTaskLODFv(0x2D)) {
                deactivateLOD__8CTaskLODFv(0x50);
                snd = getScnHandle();
                ScnData_FwdB7DD4(snd, 0x50);
                forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x50);
            }
            if (getLODData__8CTaskLODFv(0x0E)) {
                deactivateLOD__8CTaskLODFv(0x51);
                snd = getScnHandle();
                ScnData_FwdB7DD4(snd, 0x51);
                forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x51);
            }
            if (getLODData__8CTaskLODFv(0x2A)) {
                deactivateLOD__8CTaskLODFv(0x52);
                snd = getScnHandle();
                ScnData_FwdB7DD4(snd, 0x52);
                forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x52);
            }
            if (getLODData__8CTaskLODFv(0x22)) {
                deactivateLOD__8CTaskLODFv(0x53);
                snd = getScnHandle();
                ScnData_FwdB7DD4(snd, 0x53);
                forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x53);
            }
            if (getLODData__8CTaskLODFv(0x23)) {
                deactivateLOD__8CTaskLODFv(0x54);
                snd = getScnHandle();
                ScnData_FwdB7DD4(snd, 0x54);
                forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x54);
            }
            if (getLODData__8CTaskLODFv(0x3E)) {
                deactivateLOD__8CTaskLODFv(0x55);
                snd = getScnHandle();
                ScnData_FwdB7DD4(snd, 0x55);
                forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x55);
            }
            if (getLODData__8CTaskLODFv(0x2D)) {
                deactivateLOD__8CTaskLODFv(0x57);
                snd = getScnHandle();
                ScnData_FwdB7DD4(snd, 0x57);
                forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x57);
            }
            if (getLODData__8CTaskLODFv(0x2D)) {
                deactivateLOD__8CTaskLODFv(0x58);
                snd = getScnHandle();
                ScnData_FwdB7DD4(snd, 0x58);
                forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x58);
            }
            if (getLODData__8CTaskLODFv(0x0E)) {
                deactivateLOD__8CTaskLODFv(0x59);
                snd = getScnHandle();
                ScnData_FwdB7DD4(snd, 0x59);
                forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x59);
            }
            if (getLODData__8CTaskLODFv(0x0B)) {
                deactivateLOD__8CTaskLODFv(0x5B);
                snd = getScnHandle();
                ScnData_FwdB7DD4(snd, 0x5B);
                forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5B);
            }
            if (getLODData__8CTaskLODFv(0x15)) {
                deactivateLOD__8CTaskLODFv(0x5C);
                snd = getScnHandle();
                ScnData_FwdB7DD4(snd, 0x5C);
                forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5C);
            }
            if (getLODData__8CTaskLODFv(0x0C)) {
                deactivateLOD__8CTaskLODFv(0x5D);
                snd = getScnHandle();
                ScnData_FwdB7DD4(snd, 0x5D);
                forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5D);
            }
            if (getLODData__8CTaskLODFv(0x0D)) {
                deactivateLOD__8CTaskLODFv(0x5E);
                snd = getScnHandle();
                ScnData_FwdB7DD4(snd, 0x5E);
                forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5E);
            }
            if (getLODData__8CTaskLODFv(0x0E)) {
                deactivateLOD__8CTaskLODFv(0x5F);
                snd = getScnHandle();
                ScnData_FwdB7DD4(snd, 0x5F);
                forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5F);
            }
            if (getLODData__8CTaskLODFv(0x2C)) {
                deactivateLOD__8CTaskLODFv(0x60);
                snd = getScnHandle();
                ScnData_FwdB7DD4(snd, 0x60);
                forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x60);
            }
            if (getLODData__8CTaskLODFv(0x0F)) {
                deactivateLOD__8CTaskLODFv(0x61);
                snd = getScnHandle();
                ScnData_FwdB7DD4(snd, 0x61);
                forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x61);
            }
            if (getLODData__8CTaskLODFv(0x19)) {
                deactivateLOD__8CTaskLODFv(0x62);
                snd = getScnHandle();
                ScnData_FwdB7DD4(snd, 0x62);
                forwardMpfCallC__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x62);
            }
        }
    }

    u8 n2 = (u8)CtrlRemote_TouchBitByArg(0x802);
    for (u8 k = 1; k <= n2; k++) {
        u8 v = (u8)(k + 0x32);
        activateLOD__8CTaskLODFv(v);
        void* snd = getScnHandle();
        ScnData_FwdB7D9C(snd, v);
        forwardMpfCallB__17UnkClass_8047BB54Fv(&gm->field_0xF0, v);
    }
}

extern "C" int CheckViewAngle60(const nw4r::math::VEC3* a, const nw4r::math::VEC3* b,
                            const nw4r::math::VEC3* c) {
    if (b->x == 0.0f && b->z == 0.0f) return 0;

    nw4r::math::VEC3 v1;
    v1.x = b->x - a->x;
    v1.y = 0.0f;
    v1.z = b->z - a->z;
    nw4r::math::VEC3 v2;
    v2.x = c->x - a->x;
    v2.y = 0.0f;
    v2.z = c->z - a->z;

    f32 len2A = v1.x * v1.x + v1.z * v1.z;
    if (len2A == 0.0f) {
        v1 = *reinterpret_cast<nw4r::math::VEC3*>(&zero__Q22ml5CVec3);
    } else {
        PSVECNormalize(reinterpret_cast<const Vec*>(&v1), reinterpret_cast<Vec*>(&v1));
    }
    f32 len2C = v2.x * v2.x + v2.z * v2.z;
    if (len2C == 0.0f) {
        v2 = *reinterpret_cast<nw4r::math::VEC3*>(&zero__Q22ml5CVec3);
    } else {
        PSVECNormalize(reinterpret_cast<const Vec*>(&v2), reinterpret_cast<Vec*>(&v2));
    }

    f32 dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    if (len2A < 0.0f) {
        Warning__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_80526324, 0x273,
                                    (const char*)lbl_eu_80526300);
    }
    f32 lenA = (len2A <= 0.0f) ? 0.0f : len2A * nw4r::math::FrSqrt(len2A);
    if (len2C < 0.0f) {
        Warning__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_80526324, 0x273,
                                    (const char*)lbl_eu_80526300);
    }
    f32 lenC = (len2C <= 0.0f) ? 0.0f : len2C * nw4r::math::FrSqrt(len2C);

    f32 prod = lenA * lenC;
    if (fabsf(prod) < ml::epsilon) return 0;

    f32 t = dot / prod;
    if (t < -1.0f) t = -1.0f;
    if (t > 1.0f) t = 1.0f;
    if (!(t <= 1.0f && t >= -1.0f)) {
        Warning__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_805262F0, 0xEF,
                                    (const char*)lbl_eu_805262C8);
    }
    return ((f32)acos(t) * ml::rad2deg) <= 60.0f;
}

extern "C" u16 LookupRelateValue(u16 arg1, u16 arg2) {
    const char* col204 = &lbl_eu_80500664[0x204];
    int flag = 0;
    const char* fpName = &lbl_eu_80500664[0x20B];
    Bdat_GetTable_AA34((const char*)arg1);
    void* fp = getFP(fpName);
    void* r = getBdatStringColumnValue(fp, col204, (const char*)5);
    u16 row0 = *(u16*)&r;
    if (CtrlRemote_TouchBitByArg(0x20) >= row0) flag = 1;

    u32 arrA[2] = { (u32)sFacePaneN01, (u32)sFacePaneM01 };
    u32 arrB[2] = { (u32)sFacePaneN02, (u32)sFacePaneM02 };
    void* fp2 = getFP(&lbl_eu_80500664[0x21C]);
    u32 colA = arrA[flag];
    u32 colB = arrB[flag];
    s16 result = -1;
    for (u8 i = 1; i <= 0x15; i++) {
        u16 v1 = 0;
        if (fp2 != 0) {
            Bdat_GetTable_AA34((const char*)fp2);
            void* r1 = getBdatStringColumnValue(fp2, (const char*)colA, (const char*)(u32)i);
            v1 = *(u16*)&r1;
        }
        u16 v2 = 0;
        if (fp2 != 0) {
            Bdat_GetTable_AA34((const char*)fp2);
            void* r2 = getBdatStringColumnValue(fp2, (const char*)colB, (const char*)(u32)i);
            v2 = *(u16*)&r2;
        }
        u16 a = (u16)(v1 + 0xFF91);
        u16 b = (u16)(v2 + 0xFF91);
        if ((a == arg1 && b == arg2) || (a == arg2 && b == arg1)) {
            result = (s16)CtrlRemote_TouchBitByArg(i + 0x28);
            break;
        }
    }
    if (result < 0) result = 0;
    return (u16)result;
}

extern "C" void AdjustRelateValue(u16 arg1, u16 arg2, s8 delta) {
    const char* col204 = &lbl_eu_80500664[0x204];
    int flag = 0;
    const char* fpName = &lbl_eu_80500664[0x20B];
    Bdat_GetTable_AA34((const char*)arg1);
    void* fp = getFP(fpName);
    void* r = getBdatStringColumnValue(fp, col204, (const char*)5);
    u16 row0 = *(u16*)&r;
    if (CtrlRemote_TouchBitByArg(0x20) >= row0) flag = 1;

    u32 arrA[2] = { (u32)sFacePaneN01, (u32)sFacePaneM01 };
    u32 arrB[2] = { (u32)sFacePaneN02, (u32)sFacePaneM02 };
    void* fp2 = getFP(&lbl_eu_80500664[0x21C]);
    u32 colA = arrA[flag];
    u32 colB = arrB[flag];
    for (u8 i = 1; i <= 0x15; i++) {
        u16 v1 = 0;
        if (fp2 != 0) {
            Bdat_GetTable_AA34((const char*)fp2);
            void* r1 = getBdatStringColumnValue(fp2, (const char*)colA, (const char*)(u32)i);
            v1 = *(u16*)&r1;
        }
        u16 v2 = 0;
        if (fp2 != 0) {
            Bdat_GetTable_AA34((const char*)fp2);
            void* r2 = getBdatStringColumnValue(fp2, (const char*)colB, (const char*)(u32)i);
            v2 = *(u16*)&r2;
        }
        u16 a = (u16)(v1 + 0xFF91);
        u16 b = (u16)(v2 + 0xFF91);
        if ((a == arg1 && b == arg2) || (a == arg2 && b == arg1)) {
            u8 idx = i + 0x28;
            s16 v = (s16)CtrlRemote_TouchBitByArg(idx);
            s16 nv = (s16)(v + (s8)delta);
            if (nv < 0) nv = 0;
            if (nv > 0x1388) nv = 0x1388;
            CtrlRemote_SetSharedBit(idx, (s32)nv);
            break;
        }
    }
}

extern "C" void FetchRelatePair(u8* out1, u8* out2, int idx) {
    // Retail derives every table/column name from the string-table base
    // lbl_eu_80500664 (no 4th param); the base address is re-materialized
    // per use (no persistent copy). The four column-name words are read
    // into two stack arrays AFTER the unlock check, indexed by the flag.
    int flag = 0;
    u32 row = idx - 0x28;
    Bdat_GetTable_AA34((const char*)out1);
    void* fp = getFP(&lbl_eu_80500664[0x20B]);
    void* r = getBdatStringColumnValue(fp, &lbl_eu_80500664[0x204],
                                       (const char*)5);
    u16 row0 = *(u16*)&r;
    if (CtrlRemote_TouchBitByArg(0x20) >= row0) flag = 1;

    u32 arrB[2];
    u32 arrA[2];
    arrA[0] = (u32)sFacePaneN01;
    arrA[1] = (u32)sFacePaneM01;
    arrB[0] = (u32)sFacePaneN02;
    arrB[1] = (u32)sFacePaneM02;

    void* fp2 = getFP(&lbl_eu_80500664[0x21C]);
    u32 colA = arrA[flag];
    u16 v1 = 0;
    if (fp2 != 0) {
        Bdat_GetTable_AA34((const char*)fp2);
        void* r1 = getBdatStringColumnValue(fp2, (const char*)colA,
                                            (const char*)row);
        v1 = *(u16*)&r1;
    }
    u32 colB = arrB[flag];
    u16 v2 = 0;
    if (fp2 != 0) {
        Bdat_GetTable_AA34((const char*)fp2);
        void* r2 = getBdatStringColumnValue(fp2, (const char*)colB,
                                            (const char*)row);
        v2 = *(u16*)&r2;
    }
    *out1 = v1 + 0x91;
    *out2 = v2 + 0x91;
}

// 31-word stack copy of the BDAT name table; struct assignment is the only
// source shape that reproduces retail's inline mtctr/lwzu/stwu copy loop
// (memcpy emits bl memcpy instead).
struct XBMapTableAC3C { u32 w[31]; };

extern "C" int GetMapItemBitFlag(u8 max, u8 count, u32 off) {
    XBMapTableAC3C table = *(const XBMapTableAC3C*)&sMinimapPtrTablePlain[0];
    // Retail's copy loop leaves the tail word (the table name) in r3, which
    // is then passed straight to the validator.
    Bdat_GetTable_AA34((const char*)table.w[30]);
    u32 sum = 0;
    for (u8 i = 2; i < max; i++) {
        u32 p = table.w[i - 1];
        if (p != 0) {
            void* fp = getFP((const char*)p);
            sum += Bdat_GetMaxRow_B1EC((void*)fp) * 0x240;
        }
    }
    sum += (count - 1) * 0x240;
    return CtrlObjectParam_GetItemBitFlag(sum + off);
}

extern "C" void RevealMinimapArea() {
    // retail materializes the table base once (r22) and derives every
    // sub-table (+0x418/+0x378/+0x2F0) and column name from it
    u8* tbl = sSjisWordTable;
    if (CtrlRemote_TouchBitByArg(0x20) <= 4) return;
    if ((gmGameFlags & 0x100) == 0) return;
    void* cam = (void*)Scn_QueryUnk80State((CScn*)gmScene);
    f32 f = 1.0f - *(f32*)((u8*)cam + 0xC);
    if (f < 1.0f) return;

    void* player = cf::CfGameManager::getPlayer(0);
    if (player == NULL) return;
    u8 flag = (u8)lbl_eu_80664184;
    if (flag == 0) return;

    void* bdat = ::cf::CfBdat::spFldMapListFileData;
    u16 row = 0;
    if (bdat != NULL) {
        Bdat_GetTable_AA34((const char*)(tbl + 0x22B));
        void* r = getBdatStringColumnValue(bdat, (const char*)(tbl + 0x22B),
                                           (const char*)(u32)flag);
        row = *(u16*)&r;
    }
    if (row == 0) return;
    u16 prev = row - 1;

    u8 col2 = 0;
    if (bdat != NULL) {
        Bdat_GetTable_AA34((const char*)(tbl + 0x23A));
        void* r = getBdatStringColumnValue(bdat, (const char*)(tbl + 0x23A),
                                           (const char*)(u32)flag);
        col2 = *(u8*)&r;
    }
    f64 dcol2 = (f64)col2;
    f32 scale = (f32)dcol2 * 0.1f;

    // struct assignment: retail's copy is an inline mtctr/lwzu/stwu loop
    XBMapTable1 t1 = *(const XBMapTable1*)&tbl[0x418];

    cf::CfObject* pif = (cf::CfObject*)player;
    nw4r::math::VEC3* pos = (nw4r::math::VEC3*)pif->CfObject_getPosVector();
    f32 sx = pos->x / scale * 1.5f;
    f32 sy = pos->y;
    f32 sz = pos->z / scale * 1.5f;

    Bdat_GetTable_AA34((const char*)pos);
    u32 name = t1.w[flag];
    if (name == 0) return;
    void* fp = getFP((const char*)name);
    u8 n = (u8)Bdat_GetMaxRow_B1EC((void*)fp);

    const char* colName = (const char*)(tbl + 0x247);
    u8 idx = 0;
    for (u8 i = 1; i <= n; i++) {
        s16 v = 0;
        if (fp != NULL) {
            Bdat_GetTable_AA34((const char*)fp);
            void* r = getBdatStringColumnValue(fp, colName,
                                               (const char*)(u32)i);
            v = *(s16*)&r;
        }
        f64 dv = (f64)v;
        if ((f32)dv > sy) {
            idx = i;
            break;
        }
    }

    u16 rem = prev % 24;
    u16 q = prev / 24;
    // kept in a local so MWCC holds it in an fpr across the whole loop
    f32 cell = 64.0f;
    f32 R = (f32)(0.5 * (cell * sqrt(2.0)));
    f32 R2 = R * R;
    u32 baseOff = (idx - 1) * 0x240;

    const u8* t2src = &tbl[0x378];
    const u8* t3src = &tbl[0x2F0];
    for (u16 i2 = 0; i2 < 0x240; i2++) {
        u16 cellCol = i2 % 24;
        u16 q2 = i2 / 24;
        f32 dx = (f32)(s32)(cellCol - rem) * cell - sx;
        f32 dz = (f32)(s32)(q2 - q) * cell - sz;
        if (dx * dx + dz * dz <= R2) {
            XBMapTable2 t2 = *(const XBMapTable2*)t2src;
            Bdat_GetTable_AA34((const char*)t2.w[28]);
            u32 sum = 0;
            for (u8 j = 2; j < flag; j++) {
                u32 p = t2.w[j];
                if (p != 0) {
                    void* fp2 = getFP((const char*)p);
                    sum += Bdat_GetMaxRow_B1EC((void*)fp2) * 0x240;
                }
            }
            sum += baseOff;
            u32 addr = sum + i2 + 1;
            if (CtrlObjectParam_GetItemBitFlag(addr) == 0) {
                XBMapTable3 t3 = *(const XBMapTable3*)t3src;
                Bdat_GetTable_AA34((const char*)t3.w[32]);
                u32 sum2 = 0;
                for (u8 j = 2; j < flag; j++) {
                    u32 p = t3.w[j];
                    if (p != 0) {
                        void* fp2 = getFP((const char*)p);
                        sum2 += Bdat_GetMaxRow_B1EC((void*)fp2) * 0x240;
                    }
                }
                sum2 += baseOff;
                CtrlObjectParam_SetItemBitFlag(sum2 + i2 + 1, 1);
            }
        }
    }
}

extern "C" void SetMapAllBits(u32 v) {
    if (v != 0 && v <= 0x1D) {
    // 136-byte struct assignment -> retail's inline 17x8 mtctr copy loop
    // (same idiom as XBMapTable3).
    struct Table805A8Copy {
        u32 w[34];
    };
    Table805A8Copy table;
    table = *(Table805A8Copy*)&sMinimapPtrTable34;
    // retail reuses the copy loop's last loaded word (w[32]) as the argument
    Bdat_GetTable_AA34((const char*)table.w[32]);
    u32 sum = 0;
    for (u8 i = 2; i < v; i++) {
        u32 p = table.w[i - 1];
        if (p != 0) {
            void* fp = getFP((const char*)p);
            sum += Bdat_GetMaxRow_B1EC((void*)fp) * 0x240;
        }
    }
    u32 last = v - 1;
    void* fp = getFP((const char*)table.w[last]);
    int n = Bdat_GetMaxRow_B1EC((void*)fp);
    for (int j = 0; j < n; j++) {
        u32 base = sum + (u32)j * 0x240;
        for (u16 k = 0; k < 0x240; k++) {
            CtrlObjectParam_SetItemBitFlag(base + k, 1);
        }
    }
    }
}

extern "C" void ItemBlockSetDefaultFlags() {
    CItemBlock_setFlag120EC(2, 2);
    CItemBlock_setFlag120EC(4, 2);
    CItemBlock_setFlag120EC(5, 2);
    CItemBlock_setFlag120EC(6, 2);
    CItemBlock_setFlag120EC(7, 2);
    CItemBlock_setFlag120EC(8, 2);
    CItemBlock_setFlag120EC(9, 2);
    CItemBlock_setFlag120EC(3, 2);
    CItemBlock_setFlag120EC(0xD, 2);
    CItemBlock_setFlag120EC(0xA, 2);
    CItemBlock_setFlag120EC(0xB, 0);
    CItemBlock_setFlag120EC(0xC, 0);
}

extern "C" f32 GetFloatTableEntry(u32 idx) {
    // Retail computes idx - 1 up front, then struct-copies the whole
    // 13-entry table to the stack, then scales the index.
    u32 i = idx - 1;
    FloatTable13 table = sRankFloatTable;
    return table.w[i];
}

void incrementEventCounter(u32 value) {
    if (CtrlRemote_TouchBitByArg(0x20) <= 3) return;
    // retail divides the param as SIGNED (mulhw + srawi + sign-adjust), so
    // cast to int before the modulo even though the ABI type is unsigned.
    u8 m = (u8)((int)value % 200);
    // bool temp: forces the retail neg/or/srwi. booleanize idiom for the
    // nonzero test (a plain != 0 would compile to cmpwi/bne)
    bool busy = (CtrlRemote_TouchBitByArg(m + 0x312C) != 0);
    if (busy) return;

    // u16 arithmetic: the ++ keeps the value's upper bits "dirty" so every
    // use re-masks it (clrlwi) exactly like retail (call arg + each case body)
    u16 n = (u16)CtrlRemote_TouchBitByArg(m + 0x40);
    n++;
    CtrlRemote_SetSharedBit(m + 0x40, (u16)n);

    int flag = 0;
    switch (m) {
    case 0:
        if (CtrlRemote_TouchBitByArg(0x320) < 0xFE) break;
        if (CtrlRemote_TouchBitByArg(0x321) < 0xFE) break;
        if (CtrlRemote_TouchBitByArg(0x322) < 0xFE) break;
        if (CtrlRemote_TouchBitByArg(0x323) < 0xFE) break;
        if (CtrlRemote_TouchBitByArg(0x324) >= 0xFE) flag = 1;
        break;
    case 0x79: if ((u16)n == 0x1) flag = 1; break;
    case 0x7A: if ((u16)n == 0xFA) flag = 1; break;
    case 0x7B: if ((u16)n == 0x1F4) flag = 1; break;
    case 0x7C: if ((u16)n == 0x1) flag = 1; break;
    case 0x7D: if ((u16)n == 0x3) flag = 1; break;
    case 0x7E: if ((u16)n == 0x1) flag = 1; break;
    case 0x7F: if ((u16)n == 0x5) flag = 1; break;
    case 0x80: if ((u16)n == 0x1) flag = 1; break;
    case 0x86: if ((u16)n == 0x15) flag = 1; break;
    case 0x87: if ((u16)n == 0x14) flag = 1; break;
    case 0x89: if ((u16)n == 0x14) flag = 1; break;
    case 0x8B: if ((u16)n == 0x14) flag = 1; break;
    case 0x8C: if ((u16)n == 0x1E) flag = 1; break;
    case 0x8D: if ((u16)n == 0x1E) flag = 1; break;
    case 0x9F: if ((u16)n == 0xA) flag = 1; break;
    case 0xA0: if ((u16)n == 0x32) flag = 1; break;
    case 0xA1: if ((u16)n == 0x64) flag = 1; break;
    case 0xA2: if ((u16)n == 0xC8) flag = 1; break;
    case 0xA9: {
        // separate counter: retail reuses the (dead) n register for the loop
        // and calls queueEventId with the ORIGINAL m afterwards
        u8 i;
        for (i = 1; i <= 5; i++) {
            if (CtrlRemote_TouchBitByArg(i + 0x21) < 0x1F40) return;
        }
        flag = 1;
        break;
    }
    case 0xB4: if ((u16)n == 0x1) flag = 1; break;
    case 0xB5: if ((u16)n == 0xA) flag = 1; break;
    case 0xB6: if ((u16)n == 0x64) flag = 1; break;
    case 0xB7: if ((u16)n == 0xC8) flag = 1; break;
    case 0xB8: if ((u16)n == 0x12C) flag = 1; break;
    case 0xBB: if ((u16)n == 0x1) flag = 1; break;
    case 0xBC: if ((u16)n == 0x14) flag = 1; break;
    case 0xBD: if ((u16)n == 0x64) flag = 1; break;
    default: flag = 1; break;
    }
    if (flag) {
        queueEventId__Q22cf13CfGameManagerFv(m);
    }
}

extern u8 gMenuSubStateFlag;
extern "C" u8 code80135FDC_setByte_6407F(u8 val) { gMenuSubStateFlag = val; return val; }

extern u8 gMenuSubStateFlag;
extern "C" u8 code80135FDC_getByte_6407F() { return gMenuSubStateFlag; }

extern "C" void TryRevealMapArea(u8 v) {
    if (v == 0) {
        return;
    } else if (v <= 0x1D) {
        void* fp = getFP(&lbl_eu_80500664[0x15]);
        int n = Bdat_GetMaxRow_B1EC((void*)fp);
        u8 count = 0;
        u8 good = 0;
        for (int i = 1; i <= n; i++) {
            u8 c = 0;
            if (fp != 0) {
                Bdat_GetTable_AA34((const char*)fp);
                void* r = getBdatStringColumnValue(fp, &lbl_eu_80500664[0x0F],
                                                   (const char*)(u32)i);
                c = *(u8*)&r;
            }
            if (c == v) {
                count++;
                if (CtrlRemote_TouchBitByArg(i + 0x20C8) != 0) {
                    good++;
                }
            }
        }
        if (count == good) {
            SetMapAllBits(v);
        }
    }
}

extern "C" unsigned char code80135FDC_postIncByte_64080() {
    extern unsigned char gMenuDepth[8];
    unsigned char v = gMenuDepth[0];
    gMenuDepth[0] = v + 1;
    return v;
}

extern "C" u8 DecMenuCounter64080() {
    u8 val = gMenuDepth[0];
    u8 dec = val - 1;
    gMenuDepth[0] = dec;
    if ((s8)dec < 0) gMenuDepth[0] = 0;
    return val;
}

extern u8 gMenuDepth[8];
extern "C" u8 code80135FDC_getByte_64080() { return gMenuDepth[0]; }

// Find the anim item matching arg: returns the first non-NULL result from the
// direct item list / vt58 slot, recursing into child lists. The retail returns
// the raw call result (nonzero) rather than a literal 1.
extern "C" nw4r::lyt::AnimationLink* FindAnimLink(nw4r::lyt::Pane* self, const nw4r::lyt::AnimResource* arg) {
    nw4r::lyt::Pane* owner = self;
    void* anchor;
    void* current;
    nw4r::lyt::AnimationLink* found = owner->FindAnimationLinkSelf(*arg);
    if (found != 0) return found;
    u8 n = owner->GetMaterialNum();
    for (u8 i = 0; i < n; i++) {
        nw4r::lyt::Material* item = owner->GetMaterial(i);
        nw4r::lyt::AnimationLink* r = item->FindAnimationLink(*arg);
        if (r != 0) return r;
    }
    anchor = (u8*)self + 0x14;
    current = *(void**)anchor;
    while (current != anchor) {
        if (current == NULL) {
            nw4r::db::Panic((const char*)lbl_eu_8052CB40, 0x23D, (const char*)lbl_eu_8052CB1C);
        }
        nw4r::lyt::Pane* child = (nw4r::lyt::Pane*)((u8*)current - 4);
        if (child == NULL) {
            nw4r::db::Panic((const char*)sNw4rLinkListFile, 0x193, (const char*)sNw4rNullPointerAssertMsg);
        }
        nw4r::lyt::AnimationLink* r2 = FindAnimLink(child, arg);
        if (r2 != 0) return r2;
        current = *(void**)current;
    }
    return 0;
}

extern "C" void SetAnimLinkFrame(nw4r::lyt::Pane* self, const nw4r::lyt::AnimResource* arg, f32 val) {
    nw4r::lyt::Pane* owner = self;
    void* r0 = owner->FindAnimationLinkSelf(*arg);
    if (r0 != 0) {
        *(f32*)(*(u32*)((u8*)r0 + 8) + 0x10) = val;
    }
    u8 n = owner->GetMaterialNum();
    for (u8 i = 0; i < n; i++) {
        nw4r::lyt::Material* item = owner->GetMaterial(i);
        if (item->FindAnimationLink(*arg) != 0) {
            *(f32*)(*(u32*)((u8*)item + 8) + 0x10) = val;
        }
    }
    void* listAnchor = (char*)self + 0x14;
    void* current = *(void**)listAnchor;
    while (current != listAnchor) {
        if (current == NULL) {
            nw4r::db::Panic((const char*)lbl_eu_8052CB40, 0x23D, (const char*)lbl_eu_8052CB1C);
        }
        nw4r::lyt::Pane* child = (nw4r::lyt::Pane*)((char*)current - 4);
        if (child == NULL) {
            nw4r::db::Panic((const char*)sNw4rLinkListFile, 0x193, (const char*)sNw4rNullPointerAssertMsg);
        }
        SetAnimLinkFrame(child, arg, val);
        current = *(void**)current;
    }
}

extern "C" int StepAnimLinkFrame(nw4r::lyt::Pane* self, const nw4r::lyt::AnimResource* arg) {
    void* t = (void*)FindAnimLink(self, arg);
    if (t != 0) {
        nw4r::lyt::AnimTransform* anim = ((nw4r::lyt::AnimationLink*)t)->GetAnimTransform();
        f32 f = anim->GetFrame();
        u16 fs = anim->GetFrameSize();
        f32 limit = (f32)(u32)(fs - 1);
        if (f >= limit) return 1;
        f32 d = f + 1.0f;
        if (d > limit) d = limit;
        SetAnimLinkFrame(self, arg, d);
        return 0;
    }
    return 1;
}

extern "C" void AnimResetToFirst(nw4r::lyt::Pane* self, const nw4r::lyt::AnimResource* arg) {
    if (FindAnimLink(self, arg) != 0) {
        SetAnimLinkFrame(self, arg, 0.0f);
    }
}

extern "C" void AnimJumpToLast(nw4r::lyt::Pane* self, const nw4r::lyt::AnimResource* arg) {
    void* t = (void*)FindAnimLink(self, arg);
    if (t != 0) {
        nw4r::lyt::AnimTransform* anim = ((nw4r::lyt::AnimationLink*)t)->GetAnimTransform();
        // Signed conversion (retail xoris r0,r3,0x8000): (u32) would emit the
        // unsigned double-trick without the sign flip.
        f32 f = (f32)(s32)(anim->GetFrameSize() - 1);
        SetAnimLinkFrame(self, arg, f);
    }
}


extern u8 gMenuStateMain;
extern "C" void code80135FDC_clearByte_64058() { gMenuStateMain = 0; }

extern u8 gMenuStateMain;
extern "C" int code80135FDC_getByte_64058() { return gMenuStateMain; }

// retail: li r0,1; stb gMenuNotifyFlag; li r3,0; li r4,0; b UIWin_ControlWindowLists
extern "C" void MenuStateSet64059Notify() {
    extern void UIWin_ControlWindowLists(u32, u32);
    gMenuNotifyFlag = 1;
    UIWin_ControlWindowLists(0, 0);
}

extern u8 gMenuNotifyFlag;
extern "C" void code80135FDC_clearByte_64059() { gMenuNotifyFlag = 0; }

extern u8 gMenuNotifyFlag;
extern "C" int code80135FDC_getByte_64059() { return gMenuNotifyFlag; }


extern u8 gMenuStateReady;
extern "C" void code80135FDC_setByte_6405B() { gMenuStateReady = 1; }

extern u8 gMenuStateSecondary;
extern "C" u8 code80135FDC_getByte_6405A() { return gMenuStateSecondary; }

extern u8 gMenuStateReady;
extern "C" u8 code80135FDC_getByte_6405B() { return gMenuStateReady; }

extern "C" void code80135FDC_setPair_6405C_64060(u32 a, u32 b) {
    extern u32 gMenuParamA;
    extern u32 gMenuParamB;
    gMenuParamA = a;
    gMenuParamB = b;
}

extern "C" u32 code80135FDC_getWord_6405C(void) {
    extern u32 gMenuParamA;
    return gMenuParamA;
}

extern "C" u32 code80135FDC_getWord_64060(void) {
    extern u32 gMenuParamB;
    return gMenuParamB;
}


extern u8 gSysMenuActive;
extern "C" int IsMenuState621F0() { return gSysMenuActive; }

extern "C" int GetSysStateFlag11(){
    unsigned char byte = ((unsigned char*)getInstance__14Class_80296898Fv())[0x11];
    return byte != 0 ? 1 : 0;
}

extern "C" bool GetSysStateFlag10(){
    void* inst = getInstance__14Class_80296898Fv();
    unsigned char b = ((unsigned char*)inst)[0x10];
    return b != 0;
}

extern "C" int GetSysStateFlag21(){
    unsigned char* p = (unsigned char*)getInstance__14Class_80296898Fv();
    return p[0x21] != 0 ? 1 : 0;
}

extern "C" int GetSysStateFlag20() {
    void* inst = getInstance__14Class_80296898Fv();
    u8 b = static_cast<u8*>(inst)[0x20];
    return (b != 0) ? 1 : 0;
}

extern u8 gMenuBlockedFlag;
extern "C" void code80135FDC_setByte_64064() { gMenuBlockedFlag = 1; }

extern u8 gMenuBlockedFlag;
extern "C" void MenuStateClear64064() { gMenuBlockedFlag = 0; }

extern "C" int MenuStateCheck64064or30() {
    if (gMenuBlockedFlag != 0) return 1;
    return ((u8*)getInstance__14Class_80296898Fv())[0x30] != 0;
}

extern "C" int GetSysStateFlag23() {
    return ((u8*)getInstance__14Class_80296898Fv())[0x23] != 0;
}

extern "C" int GetSysStateFlag31() {
    return ((u8*)getInstance__14Class_80296898Fv())[0x31] != 0;
}

extern "C" int GetSysStateFlag22(){
    void* obj = getInstance__14Class_80296898Fv();
    return (static_cast<unsigned char*>(obj)[0x22] != 0) ? 1 : 0;
}

extern "C" int GetSysStateFlag24() {
    return ((u8*)getInstance__14Class_80296898Fv())[0x24] != 0;
}

extern "C" int GetSysStateFlag25() {
    return ((u8*)getInstance__14Class_80296898Fv())[0x25] != 0;
}

extern "C" int CheckState2CC8Active(u32 id) {
    int v = (int)CtrlRemote_TouchBitByArg(id + 0x2CC8);
    return (v == 1) ? 1 : 0;
}


/* ===== BEGIN typified .rodata (retail 0x7D8) ===== */
// Retail order: SJIS table -> u16-pair table -> u32 table -> byte table ->
// face strings -> minimap strings + pointer tables -> float table -> BDAT pool.
// Hidden gap_* padding folds into each neighbor array tail.

// 0x80500108: encoding tag (u16 "SJIS") + word table + -1 terminator.
__declspec(section ".rodata") __attribute__((used))
u8 sSjisWordTable[0x80] = {
    0x00, 0x53, 0x00, 0x4A, 0x00, 0x49, 0x00, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x56, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x3A,
    0x00, 0x00, 0x00, 0x57, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x28,
    0x00, 0x00, 0x00, 0x72, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x28,
    0x00, 0x00, 0x00, 0x4B, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x28,
    0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C,
    0x00, 0x00, 0x00, 0x27, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x1E,
    0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
};

// 0x80500188: u16-pair table read via ((u16*)&t)[idx-1] (GetU16PairEntry).
__declspec(section ".rodata") __attribute__((used))
struct Table_80500188 sThresholdPairTable = {{
    0x00000001, 0x00560074, 0x00AE0105, 0x01140137, 0x015F01D1, 0x01D201F0, 0x02180263, 0x02720281, 0x02A902D1, 0x02EF032B, 0x032B0352, 0x0353037B, 0x039903C1, 0x03CA0000
}};

// 0x805001C0: u32 lookup with -1 sentinels (SearchThresholdIdx).
__declspec(section ".rodata") __attribute__((used))
u32 sThresholdLookup[28] = {
    0xFFFFFFFF, 0x00000001, 0x00000056, 0x00000074, 0x000000AE, 0x00000105, 0x00000114, 0x00000137, 0x0000015F, 0x000001D1, 0x000001D2, 0x000001F0, 0x00000218, 0x00000263, 0x00000272, 0x00000281, 0x000002A9, 0x000002D1, 0x000002EF, 0xFFFFFFFF, 0x0000032B, 0x00000352, 0x00000353, 0x0000037B, 0x00000399, 0x000003C1, 0xFFFFFFFF, 0xFFFFFFFF
};

// 0x80500230: LOD-char table (InitCol6LodTables copies 5 words + 1 byte).
__declspec(section ".rodata") __attribute__((used))
u8 sLodCharMap[0x18] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x51, 0x52, 0x53, 0x54, 0xFF, 0x00, 0x00, 0x00
};

// Face pane names; each [0x0C] spans its 3-byte retail pad gap.
__attribute__((used)) extern const char sFacePaneN01[0x0C] = "face_n01";
__attribute__((used)) extern const char sFacePaneM01[0x0C] = "face_m01";
__attribute__((used)) extern const char sFacePaneN02[0x0C] = "face_n02";
__attribute__((used)) extern const char sFacePaneM02[0x0C] = "face_m02";

// Minimap list names (contiguous 0x10 each).

// Minimap pointer tables: name + pointer slots (NULLs, strings, self).
struct MinimapPtrTable { char name[16]; const void* entries[34]; };

// 0x805003E8: self-pointer at entries[27].
__attribute__((used))
extern const MinimapPtrTable sMinimapPtrTable2401 = {
    "minimaplist2401",
    { NULL, "minimaplist0101", "minimaplist0201", "minimaplist0301", "minimaplist0401", "minimaplist0402", "minimaplist0501", "minimaplist0601", "minimaplist0701", NULL, "minimaplist0901", "minimaplist1001", "minimaplist1101", "minimaplist1201", "minimaplist1202", "minimaplist1301", "minimaplist1401", "minimaplist1501", "minimaplist1601", "minimaplist1601", "minimaplist1701", NULL, "minimaplist1901", "minimaplist2001", "minimaplist2101", "minimaplist2201", "minimaplist2301", &sMinimapPtrTable2401, NULL, NULL, NULL, NULL, NULL, NULL }
};

// 0x80500480: no name prefix (32 slots).
__declspec(section ".rodata") __attribute__((used))
extern const void* sMinimapPtrTablePlain[32] = {
    NULL, "minimaplist0101", "minimaplist0201", "minimaplist0301", "minimaplist0401", "minimaplist0402", "minimaplist0501", "minimaplist0601", "minimaplist0701", NULL, "minimaplist0901", "minimaplist1001", "minimaplist1101", "minimaplist1201", "minimaplist1202", "minimaplist1301", "minimaplist1401", "minimaplist1501", "minimaplist1601", "minimaplist1601", "minimaplist1701", NULL, "minimaplist1901", "minimaplist2001", "minimaplist2101", "minimaplist2201", "minimaplist2301", &sMinimapPtrTable2401, NULL, NULL, NULL, NULL
};

// 0x80500500: out-of-order minimap name (retail layout).

// 0x80500510: table-08 entry + self at entries[21].
__attribute__((used))
extern const MinimapPtrTable sMinimapPtrTable1801 = {
    "minimaplist1801",
    { NULL, "minimaplist0101", "minimaplist0201", "minimaplist0301", "minimaplist0401", "minimaplist0402", "minimaplist0501", "minimaplist0601", "minimaplist0701", "minimaplist0801", "minimaplist0901", "minimaplist1001", "minimaplist1101", "minimaplist1201", "minimaplist1202", "minimaplist1301", "minimaplist1401", "minimaplist1501", "minimaplist1601", "minimaplist1601", "minimaplist1701", &sMinimapPtrTable1801, "minimaplist1901", "minimaplist2001", "minimaplist2101", "minimaplist2201", "minimaplist2301", &sMinimapPtrTable2401, NULL, NULL, NULL, NULL, NULL, NULL }
};

// 0x805005A8: no name prefix (34 slots).
__declspec(section ".rodata") __attribute__((used))
extern const void* sMinimapPtrTable34[34] = {
    NULL, "minimaplist0101", "minimaplist0201", "minimaplist0301", "minimaplist0401", "minimaplist0402", "minimaplist0501", "minimaplist0601", "minimaplist0701", NULL, "minimaplist0901", "minimaplist1001", "minimaplist1101", "minimaplist1201", "minimaplist1202", "minimaplist1301", "minimaplist1401", "minimaplist1501", "minimaplist1601", "minimaplist1601", "minimaplist1701", NULL, "minimaplist1901", "minimaplist2001", "minimaplist2101", "minimaplist2201", "minimaplist2301", &sMinimapPtrTable2401, NULL, NULL, NULL, NULL, NULL, NULL
};

// 0x80500630: 13-entry float table (GetFloatTableEntry copies it to the stack).
__declspec(section ".rodata") __attribute__((used))
FloatTable13 sRankFloatTable = {{1.5f, 2.0f, 1.7f, 1.7f, 3.7f, 1.5f, 1.9f, 1.7f, 1.7f, 1.6f, 1.5f, 1.7f, 1.7f}};

// 0x80500664: BDAT column-name pool (68 NUL-separated strings, tail-padded).
__attribute__((used))
extern const char lbl_eu_80500664[0x27C] =
    /* 0x000 */ "%s\0"
    /* 0x003 */ "TXT\0"
    /* 0x007 */ "%d\0"
    /* 0x00A */ "%.1f\0"
    /* 0x00F */ "mapID\0"
    /* 0x015 */ "landmarklist\0"
    /* 0x022 */ "rlt_lnd\0"
    /* 0x02A */ "flg_s\0"
    /* 0x030 */ "cnd_questID\0"
    /* 0x03C */ "flg_famous\0"
    /* 0x047 */ "cnd_famous\0"
    /* 0x052 */ "npcmeetID1\0"
    /* 0x05D */ "npcmeetID2\0"
    /* 0x068 */ "flg_relate\0"
    /* 0x073 */ "cnd_relate\0"
    /* 0x07E */ "rlt_meet\0"
    /* 0x087 */ "main_PC\0"
    /* 0x08F */ "scenario_s\0"
    /* 0x09A */ "scenario_e\0"
    /* 0x0A5 */ "type\0"
    /* 0x0AA */ "FLD_valpoplist\0"
    /* 0x0B9 */ "marker\0"
    /* 0x0C0 */ "popTime\0"
    /* 0x0C8 */ "wtrType\0"
    /* 0x0D0 */ "OCLOCK_21\0"
    /* 0x0DA */ "OCLOCK_18\0"
    /* 0x0E4 */ "OCLOCK_15\0"
    /* 0x0EE */ "OCLOCK_12\0"
    /* 0x0F8 */ "OCLOCK_9\0"
    /* 0x101 */ "OCLOCK_6\0"
    /* 0x10A */ "OCLOCK_3\0"
    /* 0x113 */ "OCLOCK_0\0"
    /* 0x11C */ "quest_no\0"
    /* 0x125 */ "quest\0"
    /* 0x12B */ "select_no\0"
    /* 0x135 */ "select\0"
    /* 0x13C */ "popular_no\0"
    /* 0x147 */ "popular\0"
    /* 0x14F */ "npcmeet_no\0"
    /* 0x15A */ "col6_h_no\0"
    /* 0x164 */ "col6_h\0"
    /* 0x16B */ "col6_k_no\0"
    /* 0x175 */ "col6_k\0"
    /* 0x17C */ "name\0"
    /* 0x181 */ "MNU_name\0"
    /* 0x18A */ "rlt_sex\0"
    /* 0x192 */ "SYS_filelist\0"
    /* 0x19F */ "filename\0"
    /* 0x1A8 */ "%s.tpl\0"
    /* 0x1AF */ "SYS_iconlist\0"
    /* 0x1BC */ "filename_1\0"
    /* 0x1C7 */ "filename_2\0"
    /* 0x1D2 */ "itemType\0"
    /* 0x1DB */ "itemID\0"
    /* 0x1E2 */ "rankType\0"
    /* 0x1EB */ "MNU_item\0"
    /* 0x1F4 */ "%s%s\0"
    /* 0x1F9 */ "CL6_uplist\0"
    /* 0x204 */ "flg_s1\0"
    /* 0x20B */ "JNL_PCrelatelist\0"
    /* 0x21C */ "JNL_PClinelist\0"
    /* 0x22B */ "minimap_origin\0"
    /* 0x23A */ "minimap_rate\0"
    /* 0x247 */ "height\0"
    /* 0x24E */ "CL6_materiallist\0"
    /* 0x25F */ "money\0"
    /* 0x265 */ "material%d\0"
    /* 0x270 */ "number%d\0"
;
/* ===== END typified .rodata ===== */
// ---------- typified .bss/.sdata/.sbss (retail sizes) ----------
// Retail section order: .bss (3 buffers) -> .sdata (flag + table base)
// -> .sbss (flag/counter pool). MWCC auto-routes zero-init data by size
// (.bss vs .sbss); explicit section names are avoided because this TU's
// MWCC rejects __declspec(section ".sbss") (33044). Member aligned(8)
// on each section's first symbol keeps the section alignment at 8.
__attribute__((aligned(8))) __attribute__((used)) char gTplNameBuffer[0x80];
__attribute__((used)) char gIconNameBuffer[0x80];
__attribute__((used)) char gItemNameBuffer[0x20];

__attribute__((used)) u8 gSysMenuActive = 1;
__attribute__((used)) u32 gSjisTablePtr = (u32)&sSjisWordTable;

__attribute__((aligned(8))) __attribute__((used)) u8 gMenuStateMain;
__attribute__((used)) u8 gMenuNotifyFlag;
__attribute__((used)) u8 gMenuStateSecondary;
__attribute__((used)) u8 gMenuStateReady;
__attribute__((used)) u32 gMenuParamA;
__attribute__((used)) u32 gMenuParamB;
__attribute__((used)) u8 gMenuBlockedFlag;
__attribute__((used)) u32 gSysFileListBdat;
__attribute__((used)) u32 gSysIconListBdat;
__attribute__((used)) u8 gCollectedFlags[7];
__attribute__((used)) u8 gCollectedFlagCount;
__attribute__((used)) u16 gSlotItemIds[3];
__attribute__((used)) u8 gSlotItemCount;
__attribute__((used)) u8 gMenuSubStateFlag;
__attribute__((used)) u8 gMenuDepth[8];
/* ===== END typified bss/sdata/sbss ===== */
