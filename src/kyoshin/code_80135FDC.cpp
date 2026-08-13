// Auto-scaffolded catalog TU for kyoshin/code_80135FDC
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
// code_80135FDC.hpp declares this TU's exported functions with caller-tuned
// signatures that differ from the (not-yet-matched) definitions below, so
// suppress those declarations when included from this defining TU.
#define CODE_80135FDC_CPP
#include "kyoshin/code_80135FDC.hpp"
#include <nw4r/math/math_types.h>
#include <nw4r/lyt/lyt_animation.h>
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_textBox.h>
#include <nw4r/lyt/lyt_drawInfo.h>
#include <nw4r/lyt/lyt_arcResourceAccessor.h>
#include <nw4r/lyt/lyt_material.h>
#include "monolib/device/CDeviceVI.hpp"
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

extern "C" {
// copyVEC3 stays inline: CCollepedia.hpp declares it as (void*, void*) - a
// move into code_80135FDC.hpp would clash in CCollepedia.cpp.
extern void copyVEC3(nw4r::math::VEC3*, const nw4r::math::VEC3*);
// func_8009CF8C stays inline: include/functions.hpp declares it as
// bool func_8009CF8C(int) - a move into code_80135FDC.hpp would clash in
// CGame / CMenu* units that pull functions.hpp via monolib/scn.hpp.
extern u32 func_8009CF8C(u32);
// forward decls: func_801375A0 calls these helpers before their definitions
// (they are defined later in this TU).
extern void code80135FDC_setVec3(float* self, float a, float b, float c);
extern void func_80137738(nw4r::math::VEC3* output, const nw4r::math::VEC3* value);
}

// ---------- globals ----------
extern "C" {
// lbl_eu_806640A8 / lbl_eu_806640D8 / lbl_eu_80664104 stay inline: they are
// declared as void* in CCollepedia.hpp / CKizunaTalkList.hpp / CMCEffStart.hpp
// (u32 here) - moving them into code_80135FDC.hpp would clash in those callers.
extern u32 lbl_eu_806640A8;
extern u32 lbl_eu_806640D8;
extern u32 lbl_eu_80664104;
// lbl_eu_806640EC stays inline: CMCGetItemBox.hpp declares it as void* - a
// move into code_80135FDC.hpp would clash in CMCGetItemBox.cpp.
extern u32 lbl_eu_806640EC;
}

// lbl_eu_8052E568 / lbl_eu_8052E590 stay inline: CItemBoxInfo.hpp declares
// them as char[] (u8[] here) - a move into code_80135FDC.hpp would clash in
// CMCGetItemBox.cpp.
extern "C" {
extern u8 lbl_eu_8052E568[];
extern u8 lbl_eu_8052E590[];
}

// func_8003AA34 stays inline: CKizunaTalkList.hpp declares it as (void) - a
// move into code_80135FDC.hpp would clash in CKizunaTalkList.cpp.
extern "C" void func_8003AA34(const char*);



// --- helpers ---
// func_8003B1EC stays inline: CKizunaTalkList.hpp declares it as (void*) - a
// move into code_80135FDC.hpp would clash in CKizunaTalkList.cpp.
extern "C" u32 func_8003B1EC(void*);
// func_8009ECB0 stays inline: include/functions.hpp and CChainActorPc.hpp
// declare it as int* - a move into code_80135FDC.hpp would clash in
// CGame / CMenu* units that pull those headers transitively.
extern "C" void* func_8009ECB0(void);
// func_8009D018 stays inline: include/functions.hpp declares it as
// void func_8009D018(int, int) - a move into code_80135FDC.hpp would clash in
// CGame / CMenu* units that pull functions.hpp via monolib/scn.hpp.
extern "C" void func_8009D018(u32 destination, u32 value);

// Cast-only interfaces. RTTI slots (offset 0x0/0x4) are omitted, so each
// virtual lands at its retail vtable offset (dtor slot at 0x8).
struct CAnimOwnerIf {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual CAnimOwnerIf* _v03C(u32 a, u32 b);  // 0x3C
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual void _v058();
    virtual void _v05C();
    virtual void _v060();
    virtual void _v064();
    virtual void* _v068();  // 0x68
};

struct CAnimListOwnerIf {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual void* _v058();  // 0x58
    virtual void _v05C();
    virtual void _v060();
    virtual u8 _v064();     // 0x64
    virtual void _v068();
    virtual void* _v06C(u32 idx);  // 0x6C
};

struct CAnimItemIf {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void* _v024(void* arg);  // 0x24 — returns the found item/result
};

struct CPlayerIf {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual void _v058();
    virtual void _v05C();
    virtual void _v060();
    virtual void _v064();
    virtual void _v068();
    virtual void _v06C();
    virtual void _v070();
    virtual void _v074();
    virtual void _v078();
    virtual void _v07C();
    virtual void _v080();
    virtual void _v084();
    virtual void _v088();
    virtual void _v08C();
    virtual void _v090();
    virtual void _v094();
    virtual void _v098();
    virtual void _v09C();
    virtual void _v0A0();
    virtual void _v0A4();
    virtual void _v0A8();
    virtual nw4r::math::VEC3* _v0AC();  // 0xAC
};

struct FourShorts { s16 a, b, c, d; };

// ---------- init ----------
void func_80135FDC() {
    lbl_eu_80664058 = 0;
    lbl_eu_80664059 = 0;
    lbl_eu_8066405A = 0;
    lbl_eu_8066405B = 0;
    lbl_eu_8066405C = 0;
    lbl_eu_806621F0 = 1;
    lbl_eu_80664068 = 0;
    lbl_eu_8066406C = 0;
    lbl_eu_8066407F = 0;
}

void func_8013BD9C() {
    lbl_eu_80664058 = 1;
}

void func_8013BE38() {
    lbl_eu_8066405A = 0;
    lbl_eu_8066405B = 0;
    lbl_eu_8066405C = 0;
    lbl_eu_80664060 = 0;
}

void func_8013BDE4() {
    lbl_eu_8066405B = 0;
    lbl_eu_8066405C = 0;
    lbl_eu_80664060 = 0;
    lbl_eu_8066405A = 1;
}

u8 func_801392B4(u32 idx) {
    return *(u8*)((u32)(&lbl_eu_80664070) + idx);
}

// ---------- BDAT helpers ----------
// func_8013600C: BDAT string -> u8
// func_8013606C: BDAT string -> u16
// func_801360CC: BDAT string -> s8
// func_80136130: BDAT string -> s16
// func_80136190: BDAT string -> void

void func_80136190(const char* a, const char* b, const char* c) {
    func_8003AA34(a);
    void* fp = getFP__FPCc(a);
    getBdatStringColumnValue(fp, b, c);
}

u8 func_801361E8(const char* a, const char* b, const char* c) {
    if (a == 0) return 0;
    func_8003AA34(a);
    void* result = getBdatStringColumnValue((void*)a, b, c);
    return *(u8*)&result;
}

extern "C" u16 func_80136254(const char* a, const char* b, const char* c) {
    if (a == 0) return 0;
    func_8003AA34(a);
    void* result = getBdatStringColumnValue((void*)a, b, c);
    return *(u16*)&result;
}

extern "C" int func_801362C0(const char* a, const char* b, const char* c) {
    if (a == 0) return 0;
    func_8003AA34(a);
    void* result = getBdatStringColumnValue((void*)a, b, c);
    return (s8)(*(u8*)&result);
}

extern "C" s16 func_80136330(const char* a, const char* b, const char* c) {
    if (a == 0) return 0;
    func_8003AA34(a);
    void* result = getBdatStringColumnValue((void*)a, b, c);
    return *(s16*)&result;
}

extern "C" void* func_8013639C(const char* a, const char* b, const char* c) {
    if (a == 0) return 0;
    func_8003AA34(a);
    return getBdatStringColumnValue((void*)a, b, c);
}

extern "C" u32 func_80136400(const char* src, u16* dst, u32 destLen) {
    u32 srcLen = strlen(src);
    ENCResult result = ENCConvertStringUtf8ToUtf16(
        dst, &destLen, reinterpret_cast<const u8*>(src), &srcLen);
    if (result != 0) {
        wcscpy(reinterpret_cast<wchar_t*>(dst),
               reinterpret_cast<const wchar_t*>(lbl_eu_806621F4));
        return 1;
    }
    dst[destLen] = 0;
    int i = 0;
    int len = (int)destLen;
    while (*dst != 0 && i < len) {
        if (*dst == 0x40u)
            *dst = 0x0Au;
        dst++;
        i++;
    }
    return 1;
}

extern "C" void func_eu_80136F90(char* str) {
    extern int getLanguage__9CDeviceSCFv();
    int lang = getLanguage__9CDeviceSCFv();
    if ((u8)(lang + 0xFE) > 2) return;
    int len = strlen(str);
    int i = 0;
    while (i < len) {
        char c = *str;
        if (c == '.') {
            *str = ',';
        } else if (c >= 0x81 && c <= 0x9F) {
            i++;
            str++;
        } else if (c >= 0xE0 && c <= 0xEF) {
            i++;
            str++;
        }
        i++;
        str++;
    }
}

extern "C" int func_801364B8(char* src, char delim, char** outTokens) {
    int count = 1;
    int idx = 1;
    outTokens[0] = src;
    int len = strlen(src);
    int i = 0;
    while (i < len) {
        char c = src[i];
        if (c == delim) {
            src[i] = 0;
            int j = 1;
            do {
                if (src[i + j] != ' ')
                    break;
                j++;
            } while (true);
            outTokens[idx] = &src[i + j];
            idx++;
            count++;
        } else if (c >= 0x81 && c <= 0x9F) {
            i++;
        } else if (c >= 0xE0 && c <= 0xEF) {
            i++;
        }
        i++;
    }
    for (int k = 0; k < count; k++) {
        char* t = outTokens[k];
        int pos = strlen(t) - 1;
        while (pos >= 0) {
            if (t[pos] == ' ') {
                pos--;
            } else {
                t[pos + 1] = 0;
                break;
            }
        }
    }
    return count;
}

extern "C" int func_801365E4(u16* src, u16 delim, u16** outTokens) {
    int count = 1;
    int idx = 1;
    outTokens[0] = src;
    int len = wcslen((wchar_t*)src);
    u16* p = src;
    for (int i = 0; i < len; i++) {
        if (*p == delim) {
            *p = 0;
            int j = 1;
            u16* q = p + 1;
            while (*q == L' ') {
                q++;
                j++;
            }
            outTokens[idx] = p + j;
            idx++;
            count++;
        }
        p++;
    }
    for (int k = 0; k < count; k++) {
        u16* t = outTokens[k];
        int pos = wcslen((wchar_t*)t) - 1;
        u16* q = t + pos;
        while (pos >= 0) {
            if (*q == L' ') {
                q--;
                pos--;
            } else {
                q[1] = 0;
                break;
            }
        }
    }
    return count;
}

extern "C" void func_801366F4(u16* str) {
    int len = wcslen((wchar_t*)str);
    u16* p = str;
    while (len > 0) {
        *p = (u16)toupper(*p);
        p++;
        len--;
    }
}

extern "C" void func_8013676C(void* node, nw4r::ut::Font* font) {
    extern void Panic__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
    extern char lbl_eu_80500664[];
    if (node == NULL) return;
    void* child = *(void**)((u8*)node + 0x14);
    void* head = (u8*)node + 0x14;
    char buf[32];
    char* tokens[8];
    while (child != head) {
        if (child == NULL) {
            Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052CB40, 0x23D, (const char*)lbl_eu_8052CB1C);
        }
        u32 type = (u32)child - 4;
        if (type == 0) {
            Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052E4E4, 0x193, (const char*)lbl_eu_8052E4C0);
        }
        sprintf(buf, lbl_eu_80500664, type + 0xBC);
        func_801364B8(buf, '_', tokens);
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
        func_8013676C((void*)type, font);
        child = *(void**)child;
    }
}

extern "C" void func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(
    nw4r::lyt::Layout* layout, char* name, u32 font) {
    nw4r::lyt::Pane* pane = layout->GetRootPane()->FindPaneByName(name, true);
    if (pane != NULL) {
        SetFont__Q34nw4r3lyt7TextBoxFPCQ34nw4r2ut4Font(
            static_cast<nw4r::lyt::TextBox*>(pane),
            reinterpret_cast<const nw4r::ut::Font*>(font));
    }
}

extern "C" void func_80136910__FPQ34nw4r3lyt6LayoutPcUc(
    nw4r::lyt::Layout* layout, char* name, u8 value) {
    char buf[512];
    sprintf(buf, "%d", (int)value);
    func_80136A1C(layout, name, buf, 0);
}

extern "C" void func_80136A1C(
    nw4r::lyt::Layout* layout, char* name, char* text, u32 tagProc) {
    nw4r::lyt::Pane* pane = layout->GetRootPane()->FindPaneByName(name, true);
    if (pane == NULL) return;

    u16 buf[1024];
    u32 destLen = 1024;
    u32 srcLen = strlen(text);

    ENCResult result = ENCConvertStringUtf8ToUtf16(
        buf, &destLen, reinterpret_cast<const u8*>(text), &srcLen);

    if (result != 0) {
        wcscpy(reinterpret_cast<wchar_t*>(buf),
               reinterpret_cast<const wchar_t*>(&lbl_eu_806621F4));
    } else {
        buf[destLen] = 0;
        for (u32 i = 0; i < destLen; i++) {
            if (buf[i] == 0x40u)
                buf[i] = 0x0Au;
        }
    }

    if (tagProc != 0) {
        typedef const wchar_t* (*TagProcFn)(void*, wchar_t*, int, float, float);
        TagProcFn fn = (*reinterpret_cast<TagProcFn*>(tagProc));
        const wchar_t* res = fn(reinterpret_cast<void*>(tagProc),
            reinterpret_cast<wchar_t*>(buf), 0, lbl_eu_806672D8, lbl_eu_806672D8);
        wcscpy(reinterpret_cast<wchar_t*>(buf), res);
    }

    static_cast<nw4r::lyt::TextBox*>(pane)->SetString(
        reinterpret_cast<const wchar_t*>(buf), 0);
}

extern "C" void func_80136B4C(
    nw4r::lyt::Layout* layout, char* name, char* fmt, u32 value) {
    char buf[528];
    sprintf(buf, fmt, value);
    func_80136A1C(layout, name, buf, 0);
}

extern "C" void func_80136C98(nw4r::lyt::Layout* layout, u32 value) {
    char buf[512];
    sprintf(buf, "%d", (int)value);
    func_80136D74(layout, buf, 0);
}

extern "C" void func_80136D74(
    nw4r::lyt::Layout* layout, char* text, u32 tagProc) {
    u16 buf[1024];
    u32 destLen = 1024;
    u32 srcLen = strlen(text);

    ENCResult result = ENCConvertStringUtf8ToUtf16(
        buf, &destLen, reinterpret_cast<const u8*>(text), &srcLen);

    if (result != 0) {
        wcscpy(reinterpret_cast<wchar_t*>(buf),
               reinterpret_cast<const wchar_t*>(&lbl_eu_806621F4));
    } else {
        buf[destLen] = 0;
        for (u32 i = 0; i < destLen; i++) {
            if (buf[i] == 0x40u)
                buf[i] = 0x0Au;
        }
    }

    if (tagProc != 0) {
        typedef const wchar_t* (*TagProcFn)(void*, wchar_t*, int, float, float);
        TagProcFn fn = (*reinterpret_cast<TagProcFn*>(tagProc));
        const wchar_t* res = fn(reinterpret_cast<void*>(tagProc),
            reinterpret_cast<wchar_t*>(buf), 0, lbl_eu_806672D8, lbl_eu_806672D8);
        wcscpy(reinterpret_cast<wchar_t*>(buf), res);
    }

    nw4r::lyt::Pane* pane = layout->GetRootPane()->FindPaneByName("", true);
    static_cast<nw4r::lyt::TextBox*>(pane)->SetString(
        reinterpret_cast<const wchar_t*>(buf), 0);
}

extern "C" void func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
    nw4r::lyt::Layout** ppLayout,
    nw4r::lyt::ArcResourceAccessor* accessor,
    const char* name) {
    *ppLayout = createLayout__10CLibLayoutFv();
    void* resource = accessor->GetResource(0, name, 0);
    (*ppLayout)->Build(resource, accessor);
}

extern "C" void func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
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

extern "C" void func_80136FA0(
    nw4r::lyt::Layout* layout,
    void** ppAnimRes,
    nw4r::lyt::ArcResourceAccessor* accessor,
    char* name) {
    void* resource = accessor->GetResource(0, name, 0);
    void* mem = allocate__Q23mtl10MemManagerFUlUl(0x10, getAllocHandle__10CLibLayoutFv());
    if (mem != NULL) {
        Set__Q34nw4r3lyt12AnimResourceFPCv(mem, resource);
    }
    *ppAnimRes = mem;
    layout->BindAnimationAuto(
        *reinterpret_cast<nw4r::lyt::AnimResource*>(mem), accessor);
}

extern "C" void func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(
    nw4r::lyt::Layout* layout,
    nw4r::lyt::DrawInfo* drawInfo,
    int useProjection,
    int calcMtx) {
    if (layout == NULL) return;

    nw4r::ut::Rect rect = layout->GetLayoutRect();

    if (useProjection) {
        void* camera = reinterpret_cast<void*>(lbl_eu_80663E14);
        void* view = getCurrentView__5CViewFv();
        void* viewFrame = func_8049626C(camera, view);
        GXSetProjection(
            *reinterpret_cast<Mtx44(*)>(
                reinterpret_cast<u8*>(viewFrame) + 0x194),
            GX_PERSPECTIVE);

        u32* src = reinterpret_cast<u32*>(
            reinterpret_cast<u8*>(viewFrame) + 0xCC);
        u32* dst = reinterpret_cast<u32*>(drawInfo) + 1;
        for (int i = 0; i < 12; i++) {
            dst[i] = src[i];
        }

        drawInfo->SetViewRect(rect);
        GXSetZMode(true, GX_LEQUAL, false);
    } else {
        Mtx44 ortho;
        C_MTXOrtho(ortho, rect.top, rect.bottom,
                   rect.left, rect.right, -1000.0f, 1000.0f);
        GXSetProjection(ortho, GX_ORTHOGRAPHIC);

        u32* src = &identity__Q22ml6CMat34;
        u32* dst = reinterpret_cast<u32*>(drawInfo) + 1;
        for (int i = 0; i < 12; i++) {
            dst[i] = src[i];
        }

        drawInfo->SetViewRect(rect);
        GXSetZMode(false, GX_NEVER, false);
    }

    if (calcMtx) {
        layout->CalculateMtx(*drawInfo);
    }
    layout->Draw(*drawInfo);
}

extern "C" void func_80137250__FPQ34nw4r3lyt8DrawInfo(
    nw4r::lyt::DrawInfo* drawInfo) {
    if (!CDeviceVI::isWideAspectRatio()) return;

    u8* flagPtr = reinterpret_cast<u8*>(drawInfo) + 0x50;
    u8 bit5 = (*flagPtr >> 5) & 1;
    u8 newBit = (bit5 == 0) ? 1u : 0u;
    *flagPtr = (*flagPtr & ~0x20u) | (newBit << 5);

    f32* scale = reinterpret_cast<f32*>(
        reinterpret_cast<u8*>(drawInfo) + 0x44);
    scale[0] = 0.75f;
    scale[1] = 1.0f;
}

int func_801372B4(int value) {
    if (value >= 11) {
        if (value >= 19) {
            if (value >= 29)
                return 0;
            return 5;
        }
        if (value >= 17)
            return 2;
        return 4;
    }
    if (value >= 4) {
        if (value >= 7)
            return 3;
        return 2;
    }
    if (value >= 2)
        return 1;
    return 0;
}

extern "C" void func_8013732C(const char* name) {
    u16 rowIdx = 0;
    if (lbl_eu_80664098 != 0) {
        func_8003AA34(name);
        void* result = getBdatStringColumnValue(
            reinterpret_cast<void*>(lbl_eu_80664098),
            &lbl_eu_80500664[0x22], name);
        rowIdx = *reinterpret_cast<u16*>(result);
    }

    func_8003AA34(&lbl_eu_80500664[0x15]);
    void* fp = getFP__FPCc(&lbl_eu_80500664[0x15]);
    void* result2 = getBdatStringColumnValue(
        fp, &lbl_eu_80500664[0x0F],
        reinterpret_cast<const char*>(&rowIdx));
    u8 switchVal = *reinterpret_cast<u8*>(result2);

    func_801372B4(switchVal);
}

extern float lbl_eu_806672EC;
extern float lbl_eu_806672F0;
extern float lbl_eu_806672E8;
extern double lbl_eu_806672F8;
extern "C" u32 func_80137444__FPQ34nw4r3lyt13AnimTransformf(
    nw4r::lyt::AnimTransform* anim, float delta) {
    float newFrame = delta + anim->GetFrame();
    u32 result = 0;
    if (CDeviceVI::isTvFormatPal()) {
        newFrame += lbl_eu_806672EC;
    }
    u16 frameSize = anim->GetFrameSize();
    if (newFrame >= static_cast<float>(frameSize)) {
        if (anim->IsLoopData()) {
            newFrame = lbl_eu_806672F0;
        } else {
            newFrame = static_cast<float>(anim->GetFrameSize()) - lbl_eu_806672E8;
        }
        result = 1;
    }
    anim->SetFrame(newFrame);
    return result;
}

extern "C" u32 func_80137510(nw4r::lyt::AnimTransform* anim, float delta) {
    float newFrame = anim->GetFrame() - delta;
    if (CDeviceVI::isTvFormatPal()) {
        newFrame -= 0.2f;
    }
    if (newFrame <= 0.0f) {
        if (anim->IsLoopData()) {
            newFrame = static_cast<float>(anim->GetFrameSize()) - 1.0f;
        } else {
            newFrame = 0.0f;
        }
        anim->SetFrame(newFrame);
        return 1;
    }
    anim->SetFrame(newFrame);
    return 0;
}

extern "C" void func_801375A0(nw4r::math::VEC3* output, nw4r::lyt::Pane* pane) {
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
    code80135FDC_setVec3((float*)&tmp2, 0.0f, 0.0f, 0.0f);
    if (ggp == NULL) goto add_temp;

    copyVEC3(&tmp2, &ggp->GetTranslate());
    nw4r::math::VEC3 tmp3;
    func_801375A0(&tmp3, ggp->GetParent());
    func_80137738(&tmp2, &tmp3);

add_temp:
    nw4r::math::VEC3Add(&temp, &temp, &tmp2);
add_accum:
    nw4r::math::VEC3Add(&accum, &accum, &temp);
add_out:
    nw4r::math::VEC3Add(output, output, &accum);
}

extern "C" void code80135FDC_setVec3(float* self, float a, float b, float c) {
    *(float*)((char*)self + 0) = a;
    *(float*)((char*)self + 4) = b;
    *(float*)((char*)self + 8) = c;
}

extern "C" void func_80137738(nw4r::math::VEC3* output,
                              const nw4r::math::VEC3* value) {
    nw4r::math::VEC3Add(output, output, value);
}

extern "C" void func_8013775C(nw4r::math::VEC3* output, nw4r::lyt::Pane* node) {
    output->x = 0.0f;
    output->y = 0.0f;
    output->z = 0.0f;
    if (node != NULL) {
    if (node->GetParent() != NULL) {
        output->x = node->GetTranslate().x;
        output->y = node->GetTranslate().y;
        output->z = node->GetTranslate().z;

        nw4r::lyt::Pane* parent = node->GetParent();
        nw4r::math::VEC3 accum;
        accum.x = 0.0f;
        accum.y = 0.0f;
        accum.z = 0.0f;
        if (parent == NULL) goto add_out;
        nw4r::lyt::Pane* pp = parent->GetParent();
        if (pp == NULL) goto add_out;

        accum.x = parent->GetTranslate().x;
        accum.y = parent->GetTranslate().y;
        accum.z = parent->GetTranslate().z;

        nw4r::math::VEC3 temp;
        temp.x = 0.0f;
        temp.y = 0.0f;
        temp.z = 0.0f;
        if (pp == NULL) goto add_accum;
        nw4r::lyt::Pane* gp = pp->GetParent();
        if (gp == NULL) goto add_accum;

        temp.x = pp->GetTranslate().x;
        temp.y = pp->GetTranslate().y;
        temp.z = pp->GetTranslate().z;

        nw4r::math::VEC3 tmp2;
        code80135FDC_setVec3((float*)&tmp2, 0.0f, 0.0f, 0.0f);
        if (gp == NULL) goto add_temp;
        nw4r::lyt::Pane* ggp = gp->GetParent();
        if (ggp == NULL) goto add_temp;

        copyVEC3(&tmp2, &gp->GetTranslate());
        nw4r::math::VEC3 recurse;
        func_8013775C(&recurse, ggp);
        func_80137738(&tmp2, &recurse);

    add_temp:
        nw4r::math::VEC3Add(&temp, &temp, &tmp2);
    add_accum:
        nw4r::math::VEC3Add(&accum, &accum, &temp);
    add_out:
        nw4r::math::VEC3Add(output, output, &accum);
    }
    }
}

extern "C" void func_80137924(nw4r::math::VEC3* output, nw4r::lyt::Pane* node,
                              void* scale, void* offset) {
    output->x = 0.0f;
    output->y = 0.0f;
    output->z = 0.0f;
    if (node != NULL && node->GetParent() != NULL) {
        output->x = node->GetTranslate().x;
        output->y = node->GetTranslate().y;
        output->z = node->GetTranslate().z;

        nw4r::lyt::Pane* parent = node->GetParent();
        nw4r::math::VEC3 accum;
        accum.x = 0.0f;
        accum.y = 0.0f;
        accum.z = 0.0f;
        if (parent == NULL) goto add_out;
        nw4r::lyt::Pane* pp = parent->GetParent();
        if (pp == NULL) goto add_out;

        accum.x = parent->GetTranslate().x;
        accum.y = parent->GetTranslate().y;
        accum.z = parent->GetTranslate().z;

        nw4r::math::VEC3 temp;
        temp.x = 0.0f;
        temp.y = 0.0f;
        temp.z = 0.0f;
        if (pp == NULL) goto add_accum;
        nw4r::lyt::Pane* gp = pp->GetParent();
        if (gp == NULL) goto add_accum;

        temp.x = pp->GetTranslate().x;
        temp.y = pp->GetTranslate().y;
        temp.z = pp->GetTranslate().z;

        nw4r::math::VEC3 tmp2;
        tmp2.x = 0.0f;
        tmp2.y = 0.0f;
        tmp2.z = 0.0f;
        if (gp == NULL) goto add_temp;
        nw4r::lyt::Pane* ggp = gp->GetParent();
        if (ggp == NULL) goto add_temp;

        tmp2.x = gp->GetTranslate().x;
        tmp2.y = gp->GetTranslate().y;
        tmp2.z = gp->GetTranslate().z;

        nw4r::math::VEC3 recurse;
        func_8013775C(&recurse, ggp);
        nw4r::math::VEC3Add(&recurse, &recurse, &tmp2);

    add_temp:
        nw4r::math::VEC3Add(&temp, &temp, &tmp2);
    add_accum:
        nw4r::math::VEC3Add(&accum, &accum, &temp);
    add_out:
        nw4r::math::VEC3Add(output, output, &accum);
    }
    output->x *= *(f32*)((u8*)scale + 0x44);
    output->x += *(f32*)((u8*)offset + 0x2C);
    output->y += *(f32*)((u8*)offset + 0x30);
    output->z += *(f32*)((u8*)offset + 0x34);
}

extern "C" void func_80137B44(void* a, u32 b, u32 c) {
    if (a == NULL) return;
    void* obj = *(void**)((u8*)a + 0x10);
    void** vt = *(void***)obj;
    void* result =
        ((void*(*)(void*, u32, u32))vt[0x3C / 4])(obj, b, 1);
    if (result == NULL) return;

    void** vt2 = *(void***)result;
    u32 v0 = c;
    u32 v1 = c;
    u32 v2 = c;
    u32 v3 = c;
    ((void(*)(void*, u32, void*))vt2[0x28 / 4])(result, 0, &v0);
    ((void(*)(void*, u32, void*))vt2[0x28 / 4])(result, 2, &v2);
    ((void(*)(void*, u32, void*))vt2[0x28 / 4])(result, 1, &v1);
    ((void(*)(void*, u32, void*))vt2[0x28 / 4])(result, 3, &v3);
}

extern "C" void func_80137C1C(void* a, u32 b) {
    if (a == NULL) return;
    void** vt = *(void***)a;
    u32 v0 = b;
    u32 v1 = b;
    u32 v2 = b;
    u32 v3 = b;
    ((void(*)(void*, u32, void*))vt[0x28 / 4])(a, 0, &v0);
    ((void(*)(void*, u32, void*))vt[0x28 / 4])(a, 2, &v2);
    ((void(*)(void*, u32, void*))vt[0x28 / 4])(a, 1, &v1);
    ((void(*)(void*, u32, void*))vt[0x28 / 4])(a, 3, &v3);
}

extern "C" void func_80137CD4(void* a, u32 b, u32 c, u32 d) {
    if (a == NULL) return;
    void* obj = *(void**)((u8*)a + 0x10);
    void** vt = *(void***)obj;
    void* result =
        ((void*(*)(void*, u32, u32))vt[0x3C / 4])(obj, b, 1);
    if (result == NULL) return;

    void** vt2 = *(void***)result;
    u32 v0 = c;
    u32 v1 = c;
    u32 v2 = d;
    u32 v3 = d;
    ((void(*)(void*, u32, void*))vt2[0x28 / 4])(result, 0, &v0);
    ((void(*)(void*, u32, void*))vt2[0x28 / 4])(result, 2, &v2);
    ((void(*)(void*, u32, void*))vt2[0x28 / 4])(result, 1, &v1);
    ((void(*)(void*, u32, void*))vt2[0x28 / 4])(result, 3, &v3);
}

extern "C" void func_80137DB8(void* a, u32 b, u32 c) {
    if (a == NULL) return;
    void** vt = *(void***)a;
    u32 v0 = b;
    u32 v1 = b;
    u32 v2 = c;
    u32 v3 = c;
    ((void(*)(void*, u32, void*))vt[0x28 / 4])(a, 0, &v0);
    ((void(*)(void*, u32, void*))vt[0x28 / 4])(a, 2, &v2);
    ((void(*)(void*, u32, void*))vt[0x28 / 4])(a, 1, &v1);
    ((void(*)(void*, u32, void*))vt[0x28 / 4])(a, 3, &v3);
}

extern "C" void func_80137E7C(void* a, u32 b, void* palette) {
    if (a == NULL) return;
    CAnimOwnerIf* owner = *(CAnimOwnerIf**)((u8*)a + 0x10);
    void* res = owner->_v03C(b, 1);
    if (res == NULL) return;
    if (palette == NULL) return;

    // res is an anim-owner object; its 0x68 vtable slot returns the material
    nw4r::lyt::Material* mat =
        (nw4r::lyt::Material*)((CAnimOwnerIf*)res)->_v068();

    nw4r::lyt::TexMap texMap((TPLPalette*)palette, 0);
    if (mat->GetTextureNum() == 0) {
        Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052E524, 0x88,
                                   (const char*)lbl_eu_8052E4F0);
    }
    mat->GetTexMapAry()[0] = texMap;
}

extern "C" void func_80137F88(void* a, void* palette) {
    if (a != NULL) {
        if (palette != NULL) {
            void** vt = *(void***)a;
            nw4r::lyt::Material* mat = (nw4r::lyt::Material*)((void*(*)(void*))vt[0x68 / 4])(a);

            nw4r::lyt::TexMap texMap((TPLPalette*)palette, 0);
            if (mat->GetTextureNum() == 0) {
                Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052E524, 0x88,
                                           (const char*)lbl_eu_8052E4F0);
            }
            mat->GetTexMapAry()[0] = texMap;
        }
    }
}

void func_80138078__FUl(u32 arg) {
    extern void func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(u32, u32, u32, u32, f32);
    extern f32 lbl_eu_80667300;
    func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, arg, 0, 0, lbl_eu_80667300);
}

void code80135FDC_thunk_BFE8C(u8* arg) {
    extern void func_801BFE8C(unsigned long, void*, unsigned long);
    func_801BFE8C(0, arg, 0);
}

struct Table_80500188 {
    u32 words[14];
};

extern "C" u16 func_801380A0(u32 idx) {
    struct Table_80500188 t = lbl_eu_80500188;
    u32 n = idx - 1;
    return ((u16*)&t)[n];
}

extern "C" u32 func_80138138(u32 val) {
    u32 words[28];
    for (int i = 0; i < 28; i++) {
        words[i] = lbl_eu_805001C0[i];
    }
    for (int i = 27; i >= 0; i--) {
        if (val >= words[i]) return i;
    }
    return 0;
}

extern "C" u32 func_80138234(const char* name, u32 id) {
    if (func_8009CF8C(id + 0x220) != 0) return 0;

    u16 v1 = 0;
    if (name != NULL) {
        func_8003AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x2A], (const char*)id);
        v1 = *(u16*)&result;
    }
    u16 v2 = 0;
    if (name != NULL) {
        func_8003AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x30], (const char*)id);
        v2 = *(u16*)&result;
    }
    u8 v3 = 0;
    if (name != NULL) {
        func_8003AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x3C], (const char*)id);
        v3 = *(u8*)&result;
    }
    u16 v4 = 0;
    if (name != NULL) {
        func_8003AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x47], (const char*)id);
        v4 = *(u16*)&result;
    }
    u16 v5 = 0;
    if (name != NULL) {
        func_8003AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x52], (const char*)id);
        v5 = *(u16*)&result;
    }
    u16 v6 = 0;
    if (name != NULL) {
        func_8003AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x5D], (const char*)id);
        v6 = *(u16*)&result;
    }
    u16 v7 = 0;
    if (name != NULL) {
        func_8003AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x68], (const char*)id);
        v7 = *(u16*)&result;
    }
    u8 v8 = 0;
    if (name != NULL) {
        func_8003AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x73], (const char*)id);
        v8 = *(u8*)&result;
    }

    if (((u32)(v5 + v4) + (u32)(v8 + v2)) + ((u32)(v7 + v3) + (u32)(v6 + v1)) == 0) {
        return 0;
    }
    if ((u16)func_8009CF8C(0x20) < v1) return 0;
    if (v2 != 0 && (u8)func_8009CF8C(v2 + 0x220) < 0xFE) return 0;
    if ((u16)func_8009CF8C(v3 + 0x21) < v4) return 0;

    u32 flag = 0;
    if (v5 != 0) {
        void* result = getBdatStringColumnValue((void*)lbl_eu_80664098, &lbl_eu_80500664[0x7E], (const char*)v5);
        u16 val = *(u16*)&result;
        if (func_8009CF8C(val + 0xA20) == 0) {
            flag = 1;
        }
    }
    if (flag != 0) return 0;

    flag = 0;
    if (v6 != 0) {
        void* result = getBdatStringColumnValue((void*)lbl_eu_80664098, &lbl_eu_80500664[0x7E], (const char*)v6);
        u16 val = *(u16*)&result;
        if (func_8009CF8C(val + 0xA20) == 0) {
            flag = 1;
        }
    }
    if (flag != 0) return 0;

    return (v8 == (u16)func_8009CF8C(v7 + 0x608)) ? 1 : 0;
}

extern "C" u32 func_80138574(const char* name, u32 id) {
    void* player = cf::CfGameManager::getPlayer(0);
    if (player == NULL) return 0;

    u8 v1 = 0;
    if (name != NULL) {
        func_8003AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x87], (const char*)id);
        v1 = *(u8*)&result;
    }
    if (v1 != 0 && v1 != *(u16*)((u8*)player + 0x8C)) return 0;

    u16 v2 = 0;
    if (name != NULL) {
        func_8003AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x8F], (const char*)id);
        v2 = *(u16*)&result;
    }
    u16 v3 = 0;
    if (name != NULL) {
        func_8003AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x9A], (const char*)id);
        v3 = *(u16*)&result;
    }

    u32 check = func_8009CF8C(0x20) & 0xFFFF;
    if (v2 > check || check > v3) return 0;

    u8 v4 = 0;
    if (name != NULL) {
        func_8003AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0xA5], (const char*)id);
        v4 = *(u8*)&result;
    }
    if (v4 == 5) {
        void* fp = getFP__FPCc(&lbl_eu_80500664[0xAA]);
        u16 key = 0;
        if (name != NULL) {
            func_8003AA34(&lbl_eu_80500664[0]);
            void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0xB9], (const char*)id);
            key = *(u16*)&result;
        }
        u8 v5 = 0;
        if (fp != NULL) {
            func_8003AA34(&lbl_eu_80500664[0]);
            void* result = getBdatStringColumnValue(fp, &lbl_eu_80500664[0xC0], (const char*)key);
            v5 = *(u8*)&result;
        }
        if (v5 != 0 && v5 != func_8006A6D0()) return 0;

        u8 v6 = 0;
        if (fp != NULL) {
            func_8003AA34(&lbl_eu_80500664[0]);
            void* result = getBdatStringColumnValue(fp, &lbl_eu_80500664[0xC8], (const char*)key);
            v6 = *(u8*)&result;
        }
        if (v6 != 0 && v6 != (u16)func_8016DF2C()) return 0;
    } else {
        u16 tick = 0;
        u16 other = 0;
        func_8006A234(&tick, &other);
        switch (tick) {
        case 0:
        case 1:
        case 2: {
            u8 v = 0;
            if (name != NULL) {
                func_8003AA34(&lbl_eu_80500664[0]);
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
                func_8003AA34(&lbl_eu_80500664[0]);
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
                func_8003AA34(&lbl_eu_80500664[0]);
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
                func_8003AA34(&lbl_eu_80500664[0]);
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
                func_8003AA34(&lbl_eu_80500664[0]);
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
                func_8003AA34(&lbl_eu_80500664[0]);
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
                func_8003AA34(&lbl_eu_80500664[0]);
                void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0xDA], (const char*)id);
                v = *(u8*)&result;
            }
            if (v == 0) return 0;
            break;
        }
        default: {
            u8 v = 0;
            if (name != NULL) {
                func_8003AA34(&lbl_eu_80500664[0]);
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
        func_8003AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x11C], (const char*)id);
        v7 = *(u16*)&result;
    }
    if (v7 != 0) {
        u8 v8 = 0;
        if (name != NULL) {
            func_8003AA34(&lbl_eu_80500664[0]);
            void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x125], (const char*)id);
            v8 = *(u8*)&result;
        }
        if ((func_8009CF8C(v7 + 0x220) & 0xFF) != v8) return 0;
    }
    u8 v9 = 0;
    if (name != NULL) {
        func_8003AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x12B], (const char*)id);
        v9 = *(u8*)&result;
    }
    if (v9 != 0) {
        u8 v10 = 0;
        if (name != NULL) {
            func_8003AA34(&lbl_eu_80500664[0]);
            void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x135], (const char*)id);
            v10 = *(u8*)&result;
        }
        if ((func_8009CF8C(v9 + 0x798) & 0xFF) != v10) return 0;
    }
    u8 v11 = 0;
    if (name != NULL) {
        func_8003AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x13C], (const char*)id);
        v11 = *(u8*)&result;
    }
    if (v11 != 0) {
        u16 v12 = 0;
        if (name != NULL) {
            func_8003AA34(&lbl_eu_80500664[0]);
            void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x147], (const char*)id);
            v12 = *(u16*)&result;
        }
        if ((u16)func_8009CF8C(v11 + 0x21) < v12) return 0;
    }
    u16 v13 = 0;
    if (name != NULL) {
        func_8003AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x14F], (const char*)id);
        v13 = *(u16*)&result;
    }
    if (v13 != 0) {
        if (func_8009CF8C(v13 + 0xA20) == 0) return 0;
    }
    u8 v14 = 0;
    if (name != NULL) {
        func_8003AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x15A], (const char*)id);
        v14 = *(u8*)&result;
    }
    if (v14 != 0) {
        u16 v15 = 0;
        if (name != NULL) {
            func_8003AA34(&lbl_eu_80500664[0]);
            void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x164], (const char*)id);
            v15 = *(u16*)&result;
        }
        if ((u16)func_8009CF8C(v14 + 0x7FC) < v15) return 0;
    }
    u8 v16 = 0;
    if (name != NULL) {
        func_8003AA34(&lbl_eu_80500664[0]);
        void* result = getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x16B], (const char*)id);
        v16 = *(u8*)&result;
    }
    if (v16 != 0) {
        if (name != NULL) {
            func_8003AA34(&lbl_eu_80500664[0]);
            getBdatStringColumnValue((void*)name, &lbl_eu_80500664[0x175], (const char*)id);
        }
        if (func_8009CF8C(v16 + 0x2578) == 0) return 0;
    }
    return 1;
}

extern "C" void* func_80138DA4(const char* str) {
    int v = atoi(str);
    if (v <= 0) return (void*)str;
    char* base = lbl_eu_80500664;
    char* col = base + 0x17C;
    char* fpName = base + 0x181;
    func_8003AA34(base);
    void* fp = getFP__FPCc(fpName);
    return getBdatStringColumnValue(fp, col, (const char*)v);
}

extern "C" u8 func_80138E1C(const char* key) {
    if (lbl_eu_80664098 == 0) return 0;
    func_8003AA34((const char*)lbl_eu_80664098);
    void* result = getBdatStringColumnValue((void*)lbl_eu_80664098, &lbl_eu_80500664[0x18A], key);
    return *(u8*)&result;
}

extern "C" u32 func_80138E90(const char* key) {
    u8 v = 0;
    if (lbl_eu_80664098 != 0) {
        func_8003AA34(key);
        void* result = getBdatStringColumnValue((void*)lbl_eu_80664098, &lbl_eu_80500664[0x18A], key);
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

extern "C" char* func_80138F78(const char* key) {
    if (lbl_eu_80664068 == 0) {
        func_8003AA34(key);
        lbl_eu_80664068 = (u32)getFP__FPCc(&lbl_eu_80500664[0x192]);
    }
    void* fp = (void*)lbl_eu_80664068;
    const char* col = &lbl_eu_80500664[0x19F];
    void* result;
    if (fp == NULL) {
        result = NULL;
    } else {
        // retail: bl func_8003AA34 with no arg move - r3 already holds
        // &lbl_eu_80500664[0] from the shared base computation with col
        func_8003AA34(&lbl_eu_80500664[0]);
        result = getBdatStringColumnValue(fp, col, key);
    }
    sprintf(&lbl_eu_80573B30[0], &lbl_eu_80500664[0x1A8], result);
    return &lbl_eu_80573B30[0];
}

extern "C" char* func_8013902C(const char* key) {
    if (lbl_eu_8066406C == 0) {
        func_8003AA34(key);
        lbl_eu_8066406C = (u32)getFP__FPCc(&lbl_eu_80500664[0x1AF]);
    }
    void* fp = (void*)lbl_eu_8066406C;
    void* result = NULL;
    if (fp != NULL) {
        func_8003AA34(key);
        result = getBdatStringColumnValue(fp, &lbl_eu_80500664[0x1C7], key);
    }
    sprintf(&lbl_eu_80573BB0[0], &lbl_eu_80500664[0x1A8], result);
    return &lbl_eu_80573BB0[0];
}

void func_801390E0__FPP11CFileHandle(void** handlePtr) {
    extern void cancel__11CDeviceFileFP11CFileHandle(void*);
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

// Cast-only view of the item-data object returned by func_8009EC9C: it has a
// per-item flag word at 0x176C (1 = already recorded).
struct CDataObj {
    u8 field_0x00[0x176C];
    s32 field_0x176C;
};

// List blob returned by func_8009ECB0() (+4): 7 words (listA[3] + listB[4],
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
void func_80139198(void* arg) {
    lbl_eu_80664077 = 0;
    lbl_eu_8066407E = 0;
    ListData* list = (ListData*)((u8*)func_8009ECB0() + 4);
    for (u8 i = 0; i < 7; i++) {
        u32 v;
        if (i < 3) {
            v = list->listA[i];
        } else {
            v = list->listB[i - 3];
        }
        if ((s32)v > 0) {
            if (arg != NULL) {
                CDataObj* obj = (CDataObj*)func_8009EC9C(v & 0xFFFF);
                if (obj->field_0x176C != 1) {
                    u8 n = lbl_eu_80664077;
                    ((u8*)&lbl_eu_80664070)[n] = (u8)v;
                    lbl_eu_80664077 = n + 1;
                }
            } else {
                u8 n = lbl_eu_80664077;
                ((u8*)&lbl_eu_80664070)[n] = (u8)v;
                lbl_eu_80664077 = n + 1;
            }
        }
    }
    u16* list16 = (u16*)&lbl_eu_80664078;
    int c = lbl_eu_8066407E;
    u32 w1 = list->listC[0];
    if ((s32)w1 > 0) {
        list16[c] = (u16)w1;
        c = c + 1;
        lbl_eu_8066407E = c;
    }
    u32 w2 = list->listC[1];
    if ((s32)w2 > 0) {
        int cm = c & 0xFF;
        list16[(u8)c] = (u16)w2;
        c = cm + 1;
        lbl_eu_8066407E = c;
    }
    u32 w3 = list->listC[2];
    if ((s32)w3 > 0) {
        int cm2 = c & 0xFF;
        list16[(u8)c] = (u16)w3;
        c = cm2 + 1;
        lbl_eu_8066407E = c;
    }
}



extern u8 lbl_eu_80664077;
extern "C" u8 code80135FDC_getByte_64077() { return lbl_eu_80664077; }

extern "C" u8 func_801392C8(u32 idx) {
    extern u16 lbl_eu_80664078[3];
    return (u8)(((u16*)lbl_eu_80664078)[(u8)idx]);
}

extern u8 lbl_eu_8066407E;
extern "C" u8 code80135FDC_getByte_6407E() { return lbl_eu_8066407E; }

extern "C" u16 func_801392E4(const char* name) {
    if (lbl_eu_806640EC == 0) return 0;
    func_8003AA34(name);
    void* result = getBdatStringColumnValue((void*)lbl_eu_806640EC, &lbl_eu_80500664[0x1D2], name);
    return *(u16*)&result;
}

extern "C" u16 func_80139358(const char* name) {
    if (lbl_eu_806640EC == 0) return 0;
    func_8003AA34(name);
    void* result = getBdatStringColumnValue((void*)lbl_eu_806640EC, &lbl_eu_80500664[0x1DB], name);
    return *(u16*)&result;
}

extern "C" u32 func_801393CC(const char* name) {
    u32 v = 0;
    if (lbl_eu_806640EC != 0) {
        func_8003AA34(name);
        void* result = getBdatStringColumnValue((void*)lbl_eu_806640EC, &lbl_eu_80500664[0x1D2], name);
        v = *(u16*)&result;
    }
    switch (v) {
    case 0: v = lbl_eu_806640F0; break;
    case 1: v = lbl_eu_806640F4; break;
    case 2: v = lbl_eu_806640D8; break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7: v = lbl_eu_806640F8; break;
    case 8: v = lbl_eu_806640FC; break;
    case 9: v = lbl_eu_80664104; break;
    case 10: v = lbl_eu_80664108; break;
    case 11: v = lbl_eu_8066410C; break;
    case 12: v = lbl_eu_80664110; break;
    }
    return v;
}

extern "C" char* func_801394D4(const char* name) {
    u32 a = func_801393CC(name);
    u16 b = 0;
    if (lbl_eu_806640EC != 0) {
        func_8003AA34((const char*)a);
        void* r = getBdatStringColumnValue((void*)lbl_eu_806640EC, &lbl_eu_80500664[0x1DB], name);
        b = *(u16*)&r;
    }
    u16 c = 0;
    if (lbl_eu_806640EC != 0) {
        func_8003AA34(&lbl_eu_80500664[0]);
        void* r = getBdatStringColumnValue((void*)lbl_eu_806640EC, &lbl_eu_80500664[0x1D2], name);
        c = *(u16*)&r;
    }
    void* d0 = 0;
    if (a != 0) {
        func_8003AA34(&lbl_eu_80500664[0]);
        d0 = getBdatStringColumnValue((void*)a, &lbl_eu_80500664[0x17C], (const char*)(u32)b);
    }
    sprintf(&lbl_eu_80573C30[0], &lbl_eu_80500664[0], d0);
    if (c == 3) {
        u8 d = 0;
        void* rv;
        if (lbl_eu_806640EC != 0) {
            func_8003AA34(&lbl_eu_80500664[0]);
            rv = getBdatStringColumnValue((void*)lbl_eu_806640EC, &lbl_eu_80500664[0x1E2], name);
            d = *(u8*)&rv;
        }
        func_8003AA34((const char*)rv);
        void* fp = getFP__FPCc(&lbl_eu_80500664[0x1EB]);
        void* r2 = getBdatStringColumnValue(fp, &lbl_eu_80500664[0x17C],
                                           (const char*)(0x1E - (d - 1)));
        sprintf(&lbl_eu_80573C30[0], &lbl_eu_80500664[0x1F4], &lbl_eu_80573C30[0], r2);
    }
    return &lbl_eu_80573C30[0];
}

extern "C" FourShorts func_80139658(void* obj, void* arg2, u32 idx) {
    CAnimOwnerIf* owner = *(CAnimOwnerIf**)((u8*)obj + 0x10);
    if (owner->_v03C((u32)arg2, 1) == 0) {
        FourShorts r = {0, 0, 0, 0};
        return r;
    }
    u8* data = (u8*)owner->_v068();
    if (data == NULL) {
        FourShorts r = {0, 0, 0, 0};
        return r;
    }
    if (idx >= 3) {
        Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052E558, 0x8C,
                                   (const char*)lbl_eu_8052E530);
    }
    u8* p = data + ((u8)idx << 3) + 0x10;
    FourShorts r;
    r.a = ((s16*)p)[0];
    r.b = ((s16*)p)[1];
    r.c = ((s16*)p)[2];
    r.d = ((s16*)p)[3];
    return r;
}

extern "C" FourShorts func_801397AC(CAnimOwnerIf* owner, u32 idx) {
    FourShorts r = {0, 0, 0, 0};
    if (owner == NULL) return r;
    u8* data = (u8*)owner->_v068();
    if (data != NULL) {
        if (idx >= 3) {
            Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052E558, 0x8C,
                                       (const char*)lbl_eu_8052E530);
        }
        u8* p = data + idx * 8 + 0x10;
        r.a = *(s16*)(p + 0);
        r.b = *(s16*)(p + 2);
        r.c = *(s16*)(p + 4);
        r.d = *(s16*)(p + 6);
    }
    return r;
}

extern "C" void func_801398A4(void* obj, void* arg2, void* src, u32 idx) {
    CAnimOwnerIf* owner = *(CAnimOwnerIf**)((u8*)obj + 0x10);
    if (owner->_v03C((u32)arg2, 1) == 0) return;
    u8* data = (u8*)owner->_v068();
    if (data == NULL) return;
    if (idx >= 3) {
        Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052E590, 0x8F,
                                   (const char*)lbl_eu_8052E568);
    }
    s16* s = (s16*)src;
    s16* d = (s16*)(data + idx * 8 + 0x10);
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
}

extern "C" void func_8013996C(CAnimOwnerIf* owner, void* src, u32 idx) {
    if (owner == NULL) return;
    u8* data = (u8*)owner->_v068();
    if (data == NULL) return;
    if (idx >= 3) {
        Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052E590, 0x8F,
                                   (const char*)lbl_eu_8052E568);
    }
    s16* s = (s16*)src;
    s16* d = (s16*)(data + idx * 8 + 0x10);
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
}

extern "C" void func_80139A18(void* obj, void* arg2, void* src1, void* src2) {
    CAnimOwnerIf* owner = *(CAnimOwnerIf**)((u8*)obj + 0x10);
    CAnimOwnerIf* res = owner->_v03C((u32)arg2, 1);
    if (res == NULL) return;
    if (res == NULL) return;
    s16* data = (s16*)res->_v068();
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

extern "C" void func_80139AC8(CAnimOwnerIf* owner, void* src1, void* src2) {
    if (owner == NULL) return;
    s16* data = (s16*)owner->_v068();
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

extern "C" void func_80139B5C(void* obj, void* arg2, void* src) {
    CAnimOwnerIf* owner = *(CAnimOwnerIf**)((u8*)obj + 0x10);
    void* result = owner->_v03C((u32)arg2, 1);
    if (result == NULL) return;
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
}

extern "C" void func_80139BF4(void* obj, void* arg2, void* a, void* b) {
    CAnimOwnerIf* owner = *(CAnimOwnerIf**)((u8*)obj + 0x10);
    void* result = owner->_v03C((u32)arg2, 1);
    if (result == NULL) return;
    // 8-byte color field (0xDC..0xE3) from two 32-bit words
    u32 words[2] = { (u32)a, (u32)b };
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
}

extern "C" f32 func_80139C98(u32 a, u32 b, u32 c, f32 d) {
    return (lbl_eu_80667304 * ((f32)a + (f32)b) + (f32)c) / d;
}

extern "C" void func_80139CEC(const char* arg) {
    func_8003AA34(arg);
    getFP__FPCc(&lbl_eu_80500664[0x1F9]);
    void* gm = cf::CfGameManager::func_80083298();
    if (gm == NULL) return;

    for (u8 i = 0; i <= 0x63; i++) {
        func_80462D5C__8CTaskLODFv(i);
        void* snd = func_804BC9EC__Fv();
        func_804BCC3C(snd, i);
        func_8047BD94__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, i);
    }

    struct B21 {
        u32 w[5];
        u8 b;
    };
    B21 tab = *(B21*)&lbl_eu_80500230;
    u8* chars = (u8*)&tab;
    u8 j = 0;
    for (;;) {
        s8 ch = (s8)chars[j];
        if (ch < 0) break;
        func_80462D04__8CTaskLODFv(ch);
        void* snd = func_804BC9EC__Fv();
        func_804BCC30(snd, ch);
        func_8047BD8C__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, ch);
        j++;
    }

    if (func_8009CF8C(0x7D0) == 1) {
        func_80462D04__8CTaskLODFv(0x50);
        void* snd = func_804BC9EC__Fv();
        func_804BCC30(snd, 0x50);
        func_8047BD8C__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x50);
        func_80462D04__8CTaskLODFv(0x63);
        snd = func_804BC9EC__Fv();
        func_804BCC30(snd, 0x63);
        func_8047BD8C__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x63);
        func_80462D04__8CTaskLODFv(0x09);
        snd = func_804BC9EC__Fv();
        func_804BCC30(snd, 0x09);
        func_8047BD8C__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x09);
    }

    for (u8 i = 0; i < 4; i++) {
        u8 n = (u8)func_8009CF8C(i + 0x7FE);
        u32 base = (u32)((i + 1) * 10);
        for (u8 k = 1; k <= n; k++) {
            u8 v1 = (u8)(base + k);
            func_80462D04__8CTaskLODFv(v1);
            void* snd = func_804BC9EC__Fv();
            func_804BCC30(snd, v1);
            func_8047BD8C__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, v1);

            u8 v2 = (u8)(k + 0x3C);
            func_80462D04__8CTaskLODFv(v2);
            snd = func_804BC9EC__Fv();
            func_804BCC30(snd, v2);
            func_8047BD8C__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, v2);

            u8 v3 = (u8)(k + 0x46);
            func_80462D5C__8CTaskLODFv(v3);
            snd = func_804BC9EC__Fv();
            func_804BCC3C(snd, v3);
            func_8047BD94__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, v3);

            if (v2 == 0x3D) {
                func_80462D04__8CTaskLODFv(0x55);
                snd = func_804BC9EC__Fv();
                func_804BCC30(snd, 0x55);
                func_8047BD8C__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x55);
            }
            if (v2 == 0x3E) {
                func_80462D04__8CTaskLODFv(0x56);
                snd = func_804BC9EC__Fv();
                func_804BCC30(snd, 0x56);
                func_8047BD8C__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x56);
            }
            if (v2 == 0x3D) {
                func_80462D04__8CTaskLODFv(0x57);
                snd = func_804BC9EC__Fv();
                func_804BCC30(snd, 0x57);
                func_8047BD8C__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x57);
            }
            if (v2 == 0x3D) {
                func_80462D04__8CTaskLODFv(0x58);
                snd = func_804BC9EC__Fv();
                func_804BCC30(snd, 0x58);
                func_8047BD8C__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x58);
            }
            if (func_8009CF8C(0x20) >= 0x16E) {
                func_80462D04__8CTaskLODFv(0x59);
                snd = func_804BC9EC__Fv();
                func_804BCC30(snd, 0x59);
                func_8047BD8C__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x59);
                if (v1 == 0x0E) {
                    func_80462D04__8CTaskLODFv(0x5A);
                    snd = func_804BC9EC__Fv();
                    func_804BCC30(snd, 0x5A);
                    func_8047BD8C__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x5A);
                }
            }
            if (v2 == 0x3D) {
                func_80462D04__8CTaskLODFv(0x5B);
                snd = func_804BC9EC__Fv();
                func_804BCC30(snd, 0x5B);
                func_8047BD8C__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x5B);
            }
            if (v2 == 0x3D) {
                func_80462D04__8CTaskLODFv(0x5C);
                snd = func_804BC9EC__Fv();
                func_804BCC30(snd, 0x5C);
                func_8047BD8C__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x5C);
            }
            if (v2 == 0x3E) {
                func_80462D04__8CTaskLODFv(0x5D);
                snd = func_804BC9EC__Fv();
                func_804BCC30(snd, 0x5D);
                func_8047BD8C__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x5D);
            }
            if (v2 == 0x3F) {
                func_80462D04__8CTaskLODFv(0x5E);
                snd = func_804BC9EC__Fv();
                func_804BCC30(snd, 0x5E);
                func_8047BD8C__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x5E);
            }
            if (v2 == 0x40) {
                func_80462D04__8CTaskLODFv(0x5F);
                snd = func_804BC9EC__Fv();
                func_804BCC30(snd, 0x5F);
                func_8047BD8C__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x5F);
            }
            if (v2 == 0x40) {
                func_80462D04__8CTaskLODFv(0x60);
                snd = func_804BC9EC__Fv();
                func_804BCC30(snd, 0x60);
                func_8047BD8C__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x60);
            }
            if (v2 == 0x41) {
                func_80462D04__8CTaskLODFv(0x61);
                snd = func_804BC9EC__Fv();
                func_804BCC30(snd, 0x61);
                func_8047BD8C__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x61);
            }
            if (v2 == 0x41) {
                func_80462D04__8CTaskLODFv(0x62);
                snd = func_804BC9EC__Fv();
                func_804BCC30(snd, 0x62);
                func_8047BD8C__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x62);
            }

            if (func_80462E1C__8CTaskLODFv(0x2D)) {
                func_80462D5C__8CTaskLODFv(0x50);
                snd = func_804BC9EC__Fv();
                func_804BCC3C(snd, 0x50);
                func_8047BD94__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x50);
            }
            if (func_80462E1C__8CTaskLODFv(0x0E)) {
                func_80462D5C__8CTaskLODFv(0x51);
                snd = func_804BC9EC__Fv();
                func_804BCC3C(snd, 0x51);
                func_8047BD94__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x51);
            }
            if (func_80462E1C__8CTaskLODFv(0x2A)) {
                func_80462D5C__8CTaskLODFv(0x52);
                snd = func_804BC9EC__Fv();
                func_804BCC3C(snd, 0x52);
                func_8047BD94__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x52);
            }
            if (func_80462E1C__8CTaskLODFv(0x22)) {
                func_80462D5C__8CTaskLODFv(0x53);
                snd = func_804BC9EC__Fv();
                func_804BCC3C(snd, 0x53);
                func_8047BD94__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x53);
            }
            if (func_80462E1C__8CTaskLODFv(0x23)) {
                func_80462D5C__8CTaskLODFv(0x54);
                snd = func_804BC9EC__Fv();
                func_804BCC3C(snd, 0x54);
                func_8047BD94__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x54);
            }
            if (func_80462E1C__8CTaskLODFv(0x3E)) {
                func_80462D5C__8CTaskLODFv(0x55);
                snd = func_804BC9EC__Fv();
                func_804BCC3C(snd, 0x55);
                func_8047BD94__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x55);
            }
            if (func_80462E1C__8CTaskLODFv(0x2D)) {
                func_80462D5C__8CTaskLODFv(0x57);
                snd = func_804BC9EC__Fv();
                func_804BCC3C(snd, 0x57);
                func_8047BD94__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x57);
            }
            if (func_80462E1C__8CTaskLODFv(0x2D)) {
                func_80462D5C__8CTaskLODFv(0x58);
                snd = func_804BC9EC__Fv();
                func_804BCC3C(snd, 0x58);
                func_8047BD94__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x58);
            }
            if (func_80462E1C__8CTaskLODFv(0x0E)) {
                func_80462D5C__8CTaskLODFv(0x59);
                snd = func_804BC9EC__Fv();
                func_804BCC3C(snd, 0x59);
                func_8047BD94__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x59);
            }
            if (func_80462E1C__8CTaskLODFv(0x0B)) {
                func_80462D5C__8CTaskLODFv(0x5B);
                snd = func_804BC9EC__Fv();
                func_804BCC3C(snd, 0x5B);
                func_8047BD94__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x5B);
            }
            if (func_80462E1C__8CTaskLODFv(0x15)) {
                func_80462D5C__8CTaskLODFv(0x5C);
                snd = func_804BC9EC__Fv();
                func_804BCC3C(snd, 0x5C);
                func_8047BD94__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x5C);
            }
            if (func_80462E1C__8CTaskLODFv(0x0C)) {
                func_80462D5C__8CTaskLODFv(0x5D);
                snd = func_804BC9EC__Fv();
                func_804BCC3C(snd, 0x5D);
                func_8047BD94__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x5D);
            }
            if (func_80462E1C__8CTaskLODFv(0x0D)) {
                func_80462D5C__8CTaskLODFv(0x5E);
                snd = func_804BC9EC__Fv();
                func_804BCC3C(snd, 0x5E);
                func_8047BD94__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x5E);
            }
            if (func_80462E1C__8CTaskLODFv(0x0E)) {
                func_80462D5C__8CTaskLODFv(0x5F);
                snd = func_804BC9EC__Fv();
                func_804BCC3C(snd, 0x5F);
                func_8047BD94__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x5F);
            }
            if (func_80462E1C__8CTaskLODFv(0x2C)) {
                func_80462D5C__8CTaskLODFv(0x60);
                snd = func_804BC9EC__Fv();
                func_804BCC3C(snd, 0x60);
                func_8047BD94__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x60);
            }
            if (func_80462E1C__8CTaskLODFv(0x0F)) {
                func_80462D5C__8CTaskLODFv(0x61);
                snd = func_804BC9EC__Fv();
                func_804BCC3C(snd, 0x61);
                func_8047BD94__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x61);
            }
            if (func_80462E1C__8CTaskLODFv(0x19)) {
                func_80462D5C__8CTaskLODFv(0x62);
                snd = func_804BC9EC__Fv();
                func_804BCC3C(snd, 0x62);
                func_8047BD94__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, 0x62);
            }
        }
    }

    u8 n2 = (u8)func_8009CF8C(0x802);
    for (u8 k = 1; k <= n2; k++) {
        u8 v = (u8)(k + 0x32);
        func_80462D04__8CTaskLODFv(v);
        void* snd = func_804BC9EC__Fv();
        func_804BCC30(snd, v);
        func_8047BD8C__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, v);
    }
}

extern "C" int func_8013A4B4(const nw4r::math::VEC3* a, const nw4r::math::VEC3* b,
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
        v1 = zero__Q22ml5CVec3;
    } else {
        PSVECNormalize(reinterpret_cast<const Vec*>(&v1), reinterpret_cast<Vec*>(&v1));
    }
    f32 len2C = v2.x * v2.x + v2.z * v2.z;
    if (len2C == 0.0f) {
        v2 = zero__Q22ml5CVec3;
    } else {
        PSVECNormalize(reinterpret_cast<const Vec*>(&v2), reinterpret_cast<Vec*>(&v2));
    }

    f32 dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    if (len2A < 0.0f) {
        Warning__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_80526324, 0x273,
                                    (const char*)lbl_eu_80526300);
    }
    f32 lenA = (len2A <= 0.0f) ? 0.0f : len2A * FrSqrt__Q24nw4r4mathFf(len2A);
    if (len2C < 0.0f) {
        Warning__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_80526324, 0x273,
                                    (const char*)lbl_eu_80526300);
    }
    f32 lenC = (len2C <= 0.0f) ? 0.0f : len2C * FrSqrt__Q24nw4r4mathFf(len2C);

    f32 prod = lenA * lenC;
    if (fabsf(prod) < lbl_eu_8066A208) return 0;

    f32 t = dot / prod;
    if (t < lbl_eu_80667308) t = lbl_eu_80667308;
    if (t > lbl_eu_806672E8) t = lbl_eu_806672E8;
    if (!(t <= lbl_eu_806672E8 && t >= lbl_eu_80667308)) {
        Warning__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_805262F0, 0xEF,
                                    (const char*)lbl_eu_805262C8);
    }
    return ((f32)acos(t) * lbl_eu_8066A20C) <= lbl_eu_8066730C;
}

extern "C" u16 func_8013A7D0(u16 arg1, u16 arg2) {
    int flag = 0;

    func_8003AA34((const char*)arg1);
    void* fp = getFP__FPCc(&lbl_eu_80500664[0x20B]);
    void* r = getBdatStringColumnValue(fp, &lbl_eu_80500664[0x204],
                                       (const char*)5);
    u16 row0 = *(u16*)&r;
    if (func_8009CF8C(0x20) >= row0) flag = 1;

    u32 arrA[2] = { *(u32*)&lbl_eu_80667310, *(u32*)&lbl_eu_80667314 };
    u32 arrB[2] = { *(u32*)&lbl_eu_80667318, *(u32*)&lbl_eu_8066731C };
    void* fp2 = getFP__FPCc(&lbl_eu_80500664[0x21C]);
    u32 colA = arrA[flag];
    u32 colB = arrB[flag];
    s16 result = -1;
    for (u8 i = 1; i <= 0x15; i++) {
        u16 v1 = 0;
        if (fp2 != 0) {
            func_8003AA34((const char*)fp2);
            void* r1 = getBdatStringColumnValue(fp2, (const char*)colA, (const char*)(u32)i);
            v1 = *(u16*)&r1;
        }
        u16 v2 = 0;
        if (fp2 != 0) {
            func_8003AA34((const char*)fp2);
            void* r2 = getBdatStringColumnValue(fp2, (const char*)colB, (const char*)(u32)i);
            v2 = *(u16*)&r2;
        }
        u16 a = (u16)(v1 + 0xFF91);
        u16 b = (u16)(v2 + 0xFF91);
        if ((a == arg1 && b == arg2) || (a == arg2 && b == arg1)) {
            result = (s16)func_8009CF8C(i + 0x28);
            break;
        }
    }
    if (result < 0) result = 0;
    return (u16)result;
}

extern "C" void func_8013A95C(u16 arg1, u16 arg2, s8 delta) {
    int flag = 0;

    func_8003AA34((const char*)arg1);
    void* fp = getFP__FPCc(&lbl_eu_80500664[0x20B]);
    void* r = getBdatStringColumnValue(fp, &lbl_eu_80500664[0x204],
                                       (const char*)5);
    u16 row0 = *(u16*)&r;
    if (func_8009CF8C(0x20) >= row0) flag = 1;

    u32 arrA[2] = { *(u32*)&lbl_eu_80667320, *(u32*)&lbl_eu_80667324 };
    u32 arrB[2] = { *(u32*)&lbl_eu_80667328, *(u32*)&lbl_eu_8066732C };
    void* fp2 = getFP__FPCc(&lbl_eu_80500664[0x21C]);
    u32 colA = arrA[flag];
    u32 colB = arrB[flag];
    for (u8 i = 1; i <= 0x15; i++) {
        u16 v1 = 0;
        if (fp2 != 0) {
            func_8003AA34((const char*)fp2);
            void* r1 = getBdatStringColumnValue(fp2, (const char*)colA, (const char*)(u32)i);
            v1 = *(u16*)&r1;
        }
        u16 v2 = 0;
        if (fp2 != 0) {
            func_8003AA34((const char*)fp2);
            void* r2 = getBdatStringColumnValue(fp2, (const char*)colB, (const char*)(u32)i);
            v2 = *(u16*)&r2;
        }
        u16 a = (u16)(v1 + 0xFF91);
        u16 b = (u16)(v2 + 0xFF91);
        if ((a == arg1 && b == arg2) || (a == arg2 && b == arg1)) {
            u8 idx = i + 0x28;
            s16 v = (s16)func_8009CF8C(idx);
            s16 nv = (s16)(v + (s8)delta);
            if (nv < 0) nv = 0;
            if (nv > 0x1388) nv = 0x1388;
            func_8009D018(idx, (s32)nv);
            break;
        }
    }
}

extern "C" void func_8013AB0C(u8* out1, u8* out2, u32 idx, const char* strBase) {
    u32 arrA[2] = { *(u32*)&lbl_eu_80667330, *(u32*)&lbl_eu_80667334 };
    u32 arrB[2] = { *(u32*)&lbl_eu_80667338, *(u32*)&lbl_eu_8066733C };
    int flag = 0;

    func_8003AA34((const char*)out1);
    void* fp = getFP__FPCc((const char*)&strBase[0x20B]);
    void* r = getBdatStringColumnValue(fp, &strBase[0x204], (const char*)5);
    u16 row0 = *(u16*)&r;
    if (func_8009CF8C(0x20) >= row0) flag = 1;

    u32 row = idx - 0x28;
    void* fp2 = getFP__FPCc((const char*)&strBase[0x21C]);
    u32 colA = arrA[flag];
    u16 v1 = 0;
    if (fp2 != 0) {
        func_8003AA34((const char*)fp2);
        void* r1 = getBdatStringColumnValue(fp2, (const char*)colA, (const char*)row);
        v1 = *(u16*)&r1;
    }
    u32 colB = arrB[flag];
    u16 v2 = 0;
    if (fp2 != 0) {
        func_8003AA34((const char*)fp2);
        void* r2 = getBdatStringColumnValue(fp2, (const char*)colB, (const char*)row);
        v2 = *(u16*)&r2;
    }
    *out1 = (u8)v1 + 0x91;
    *out2 = (u8)v2 + 0x91;
}

extern "C" int func_8013AC3C(u8 max, u8 count, u32 off) {
    u32 table[31];
    memcpy(table, &lbl_eu_80500480, sizeof(table));
    func_8003AA34((const char*)table[30]);
    u32 sum = 0;
    for (u8 i = 2; i < max; i++) {
        u32 p = table[i - 1];
        if (p != 0) {
            void* fp = getFP__FPCc((const char*)p);
            sum += func_8003B1EC((void*)fp) * 0x240;
        }
    }
    sum += (count - 1) * 0x240;
    return func_8009EBE8(sum + off);
}

extern "C" void func_8013ACFC() {
    if (func_8009CF8C(0x20) <= 4) return;
    if ((lbl_eu_80663E24 & 0x100) == 0) return;
    void* cam = (void*)func_8049603C((CScn*)lbl_eu_80663E14);
    f32 f = lbl_eu_806672E8 - *(f32*)((u8*)cam + 0xC);
    if (f < lbl_eu_806672E8) return;

    void* player = cf::CfGameManager::getPlayer(0);
    if (player == NULL) return;
    u8 flag = (u8)lbl_eu_80664184;
    if (flag == 0) return;

    u32 bdat = lbl_eu_806640A8;
    u16 row = 0;
    if (bdat != 0) {
        func_8003AA34(&lbl_eu_80500664[0x22B]);
        void* r = getBdatStringColumnValue((void*)bdat, &lbl_eu_80500664[0x22B],
                                           (const char*)(u32)flag);
        row = *(u16*)&r;
    }
    if (row == 0) return;
    u16 prev = row - 1;

    u8 col2 = 0;
    if (bdat != 0) {
        func_8003AA34(&lbl_eu_80500664[0x23A]);
        void* r = getBdatStringColumnValue((void*)bdat, &lbl_eu_80500664[0x23A],
                                           (const char*)(u32)flag);
        col2 = *(u8*)&r;
    }
    f32 scale = (f32)col2 * lbl_eu_80667340;

    u32 table[35];
    memcpy(table, &lbl_eu_80500108[0x418], sizeof(table));

    CPlayerIf* pif = (CPlayerIf*)player;
    nw4r::math::VEC3* pos = pif->_v0AC();
    f32 sx = pos->x / scale * lbl_eu_80667344;
    f32 sy = pos->y;
    f32 sz = pos->z / scale * lbl_eu_80667344;

    func_8003AA34((const char*)pos);
    u32 name = table[flag - 1];
    if (name == 0) return;
    void* fp = getFP__FPCc((const char*)name);
    u8 n = (u8)func_8003B1EC((void*)fp);

    u16 idx = 0;
    for (u8 i = 1; i <= n; i++) {
        s16 v = 0;
        if (fp != 0) {
            func_8003AA34((const char*)fp);
            void* r = getBdatStringColumnValue(fp, &lbl_eu_80500664[0x247],
                                               (const char*)(u32)i);
            v = *(s16*)&r;
        }
        if ((f32)v > sy) {
            idx = i;
            break;
        }
    }

    u32 x = (u32)prev;
    u32 rem = x % 24;
    u32 q = x / 24;
    f32 R = (f32)(lbl_eu_80667358 * (lbl_eu_80667348 * sqrt(lbl_eu_80667350)));
    f32 R2 = R * R;
    u32 baseOff = (idx - 1) * 0x240;

    for (u16 i2 = 0; i2 < 0x240; i2++) {
        u32 r5 = i2 % 24;
        u32 q2 = i2 / 24;
        f32 dx = (f32)(s32)(r5 - rem) * lbl_eu_80667348 - sx;
        f32 dz = (f32)(s32)(q2 - q) * lbl_eu_80667348 - sz;
        if (dx * dx + dz * dz <= R2) {
            u32 table2[31];
            memcpy(table2, &lbl_eu_80500108[0x378], sizeof(table2));
            func_8003AA34((const char*)table2[30]);
            u32 sum = 0;
            for (u8 j = 2; j < flag; j++) {
                u32 p = table2[j - 1];
                if (p != 0) {
                    void* fp2 = getFP__FPCc((const char*)p);
                    sum += func_8003B1EC((void*)fp2) * 0x240;
                }
            }
            sum += baseOff;
            u32 addr = sum + i2 + 1;
            if (func_8009EBE8(addr) == 0) {
                u32 table3[35];
                memcpy(table3, &lbl_eu_80500108[0x2F0], sizeof(table3));
                func_8003AA34((const char*)table3[34]);
                u32 sum2 = 0;
                for (u8 j = 2; j < flag; j++) {
                    u32 p = table3[j - 1];
                    if (p != 0) {
                        void* fp2 = getFP__FPCc((const char*)p);
                        sum2 += func_8003B1EC((void*)fp2) * 0x240;
                    }
                }
                sum2 += baseOff;
                func_8009EB94(sum2 + i2 + 1, 1);
            }
        }
    }
}

extern "C" void func_8013B1C4(u32 v) {
    if (v == 0) return;
    if (v > 0x1D) return;
    u32 table[34];
    u8* d = (u8*)table;
    const u8* s = (const u8*)&lbl_eu_805005A8;
    for (u32 i = 0; i < 136; i++) {
        d[i] = s[i];
    }
    // retail leaves the last copied word (table[32]) in r3; MWCC's copy-loop
    // recognition reuses it for this call's argument
    func_8003AA34((const char*)table[32]);
    u32 sum = 0;
    for (u8 i = 2; i < v; i++) {
        u32 p = table[i - 1];
        if (p != 0) {
            void* fp = getFP__FPCc((const char*)p);
            sum += func_8003B1EC((void*)fp) * 0x240;
        }
    }
    void* fp = getFP__FPCc((const char*)table[v - 1]);
    u32 n = func_8003B1EC((void*)fp);
    for (u8 j = 0; j < n; j++) {
        u32 base = sum + (u32)j * 0x240;
        for (u16 k = 0; k < 0x240; k++) {
            func_8009EB94(base + k, 1);
        }
    }
}

extern "C" void func_8013B2D4() {
    func_80157824(2, 2);
    func_80157824(4, 2);
    func_80157824(5, 2);
    func_80157824(6, 2);
    func_80157824(7, 2);
    func_80157824(8, 2);
    func_80157824(9, 2);
    func_80157824(3, 2);
    func_80157824(0xD, 2);
    func_80157824(0xA, 2);
    func_80157824(0xB, 0);
    func_80157824(0xC, 0);
}

extern "C" f32 func_8013B380(u32 idx) {
    f32 table[13];
    memcpy(table, &lbl_eu_80500630, sizeof(table));
    return table[idx - 1];
}

extern "C" void func_8013B428__FUl(u32 value) {
    if (func_8009CF8C(0x20) <= 3) return;
    u8 m = (u8)(value % 200);
    if (func_8009CF8C(m + 0x312C) != 0) return;

    u16 n = (u16)((u16)func_8009CF8C(m + 0x40) + 1);
    func_8009D018(m + 0x40, n);

    int flag = 0;
    switch (m) {
    case 0:
        if (func_8009CF8C(0x320) < 0xFE) break;
        if (func_8009CF8C(0x321) < 0xFE) break;
        if (func_8009CF8C(0x322) < 0xFE) break;
        if (func_8009CF8C(0x323) < 0xFE) break;
        if (func_8009CF8C(0x324) >= 0xFE) flag = 1;
        break;
    case 0x79: if (n == 0x1) flag = 1; break;
    case 0x7A: if (n == 0xFA) flag = 1; break;
    case 0x7B: if (n == 0x1F4) flag = 1; break;
    case 0x7C: if (n == 0x1) flag = 1; break;
    case 0x7D: if (n == 0x3) flag = 1; break;
    case 0x7E: if (n == 0x1) flag = 1; break;
    case 0x7F: if (n == 0x5) flag = 1; break;
    case 0x80: if (n == 0x1) flag = 1; break;
    case 0x86: if (n == 0x15) flag = 1; break;
    case 0x87: if (n == 0x14) flag = 1; break;
    case 0x89: if (n == 0x14) flag = 1; break;
    case 0x8B: if (n == 0x1E) flag = 1; break;
    case 0x8C: if (n == 0x14) flag = 1; break;
    case 0x8D: if (n == 0x1E) flag = 1; break;
    case 0x9F: if (n == 0xA) flag = 1; break;
    case 0xA0: if (n == 0x32) flag = 1; break;
    case 0xA1: if (n == 0x64) flag = 1; break;
    case 0xA2: if (n == 0xC8) flag = 1; break;
    case 0xA9:
        for (m = 1; m <= 5; m++) {
            if (func_8009CF8C(m + 0x21) < 0x1F40) return;
        }
        flag = 1;
        break;
    case 0xB4: if (n == 0x1) flag = 1; break;
    case 0xB5: if (n == 0xA) flag = 1; break;
    case 0xB6: if (n == 0x64) flag = 1; break;
    case 0xB7: if (n == 0x64) flag = 1; break;
    case 0xB8: if (n == 0x12C) flag = 1; break;
    case 0xBB: if (n == 0x1) flag = 1; break;
    case 0xBC: if (n == 0x14) flag = 1; break;
    case 0xBD: if (n == 0x64) flag = 1; break;
    default: flag = 1; break;
    }
    if (flag) {
        func_800826F0__Q22cf13CfGameManagerFv(m);
    }
}

extern u8 lbl_eu_8066407F;
extern "C" u8 code80135FDC_setByte_6407F(u8 val) { lbl_eu_8066407F = val; return val; }

extern u8 lbl_eu_8066407F;
extern "C" u8 code80135FDC_getByte_6407F() { return lbl_eu_8066407F; }

extern "C" void func_8013B88C(u8 v) {
    if (v == 0) {
        return;
    } else if (v <= 0x1D) {
        void* fp = getFP__FPCc(&lbl_eu_80500664[0x15]);
        int n = func_8003B1EC((void*)fp);
        u8 count = 0;
        u8 good = 0;
        for (int i = 1; i <= n; i++) {
            u8 c = 0;
            if (fp != 0) {
                func_8003AA34((const char*)fp);
                void* r = getBdatStringColumnValue(fp, &lbl_eu_80500664[0x0F],
                                                   (const char*)(u32)i);
                c = *(u8*)&r;
            }
            if (c == v) {
                count++;
                if (func_8009CF8C(i + 0x20C8) != 0) {
                    good++;
                }
            }
        }
        if (count == good) {
            func_8013B1C4(v);
        }
    }
}

extern "C" unsigned char code80135FDC_postIncByte_64080() {
    extern unsigned char lbl_eu_80664080;
    unsigned char v = lbl_eu_80664080;
    lbl_eu_80664080 = v + 1;
    return v;
}

u8 func_8013B980() {
    u8 val = lbl_eu_80664080;
    u8 dec = val - 1;
    lbl_eu_80664080 = dec;
    if ((s8)dec < 0) lbl_eu_80664080 = 0;
    return val;
}

extern u8 lbl_eu_80664080;
extern "C" u8 code80135FDC_getByte_64080() { return lbl_eu_80664080; }

// Find the anim item matching arg: returns the first non-NULL result from the
// direct item list / vt58 slot, recursing into child lists. The retail returns
// the raw call result (nonzero) rather than a literal 1.
extern "C" void* func_8013B9AC(void* self, void* arg) {
    CAnimListOwnerIf* owner = (CAnimListOwnerIf*)self;
    void* anchor;
    void* current;
    void* found = owner->_v058();
    if (found != 0) return found;
    u8 n = owner->_v064();
    for (u8 i = 0; i < n; i++) {
        CAnimItemIf* item = (CAnimItemIf*)owner->_v06C(i);
        void* r = item->_v024(arg);
        if (r != 0) return r;
    }
    anchor = (u8*)self + 0x14;
    current = *(void**)anchor;
    while (current != anchor) {
        if (current == NULL) {
            Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052CB40, 0x23D,
                                       (const char*)lbl_eu_8052CB1C);
        }
        void* child = (u8*)current - 4;
        if (child == NULL) {
            Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052E4E4, 0x193,
                                       (const char*)lbl_eu_8052E4C0);
        }
        void* r2 = func_8013B9AC(child, arg);
        if (r2 != 0) return r2;
        current = *(void**)current;
    }
    return 0;
}

extern "C" void func_8013BAD8(void* self, void* arg, f32 val) {
    CAnimListOwnerIf* owner = (CAnimListOwnerIf*)self;
    void* r0 = owner->_v058();
    if (r0 != 0) {
        *(f32*)(*(u32*)((u8*)r0 + 8) + 0x10) = val;
    }
    u8 n = owner->_v064();
    for (u8 i = 0; i < n; i++) {
        CAnimItemIf* item = (CAnimItemIf*)owner->_v06C(i);
        if (item->_v024(arg) != 0) {
            *(f32*)(*(u32*)((u8*)item + 8) + 0x10) = val;
        }
    }
    void* listAnchor = (char*)self + 0x14;
    void* current = *(void**)listAnchor;
    while (current != listAnchor) {
        if (current == NULL) {
            Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052CB40, 0x23D,
                                       (const char*)lbl_eu_8052CB1C);
        }
        void* child = (char*)current - 4;
        if (child == NULL) {
            Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052E4E4, 0x193,
                                       (const char*)lbl_eu_8052E4C0);
        }
        func_8013BAD8(child, arg, val);
        current = *(void**)current;
    }
}

extern "C" int func_8013BC0C(void* self, void* arg) {
    void* t = (void*)func_8013B9AC(self, arg);
    if (t != 0) {
        nw4r::lyt::AnimTransform* anim = *(nw4r::lyt::AnimTransform**)((u8*)t + 8);
        f32 f = *(f32*)((u8*)anim + 0x10);
        u16 fs = anim->GetFrameSize();
        f32 limit = (f32)(u32)(fs - 1);
        if (f >= limit) return 1;
        f32 d = f + lbl_eu_806672E8;
        if (d > limit) d = limit;
        func_8013BAD8(self, arg, d);
        return 0;
    }
    return 1;
}

extern "C" void func_8013BCD4(void* self, void* arg) {
    if (func_8013B9AC(self, arg) != 0) {
        func_8013BAD8(self, arg, lbl_eu_806672F0);
    }
}

extern "C" void func_8013BD24(void* self, void* arg) {
    void* t = (void*)func_8013B9AC(self, arg);
    if (t != 0) {
        nw4r::lyt::AnimTransform* anim = *(nw4r::lyt::AnimTransform**)((u8*)t + 8);
        f32 f = (f32)(u32)(anim->GetFrameSize() - 1);
        func_8013BAD8(self, arg, f);
    }
}


extern u8 lbl_eu_80664058;
extern "C" void code80135FDC_clearByte_64058() { lbl_eu_80664058 = 0; }

extern u8 lbl_eu_80664058;
extern "C" u8 code80135FDC_getByte_64058() { return lbl_eu_80664058; }

// retail: li r0,1; stb lbl_eu_80664059; li r3,0; li r4,0; b func_8013EC6C
extern "C" void func_8013BDBC() {
    extern void func_8013EC6C(u32, u32);
    lbl_eu_80664059 = 1;
    func_8013EC6C(0, 0);
}

extern u8 lbl_eu_80664059;
extern "C" void code80135FDC_clearByte_64059() { lbl_eu_80664059 = 0; }

extern u8 lbl_eu_80664059;
extern "C" u8 code80135FDC_getByte_64059() { return lbl_eu_80664059; }


extern u8 lbl_eu_8066405B;
extern "C" void code80135FDC_setByte_6405B() { lbl_eu_8066405B = 1; }

extern u8 lbl_eu_8066405A;
extern "C" u8 code80135FDC_getByte_6405A() { return lbl_eu_8066405A; }

extern u8 lbl_eu_8066405B;
extern "C" u8 code80135FDC_getByte_6405B() { return lbl_eu_8066405B; }

extern "C" void code80135FDC_setPair_6405C_64060(u32 a, u32 b) {
    extern u32 lbl_eu_8066405C;
    extern u32 lbl_eu_80664060;
    lbl_eu_8066405C = a;
    lbl_eu_80664060 = b;
}

extern "C" u32 code80135FDC_getWord_6405C(void) {
    extern u32 lbl_eu_8066405C;
    return lbl_eu_8066405C;
}

extern "C" u32 code80135FDC_getWord_64060(void) {
    extern u32 lbl_eu_80664060;
    return lbl_eu_80664060;
}


extern u8 lbl_eu_806621F0;
extern "C" int func_8013BE50() { return lbl_eu_806621F0; }

int func_8013BE58(){
    unsigned char byte = ((unsigned char*)getInstance__14Class_80296898Fv())[0x11];
    return byte != 0 ? 1 : 0;
}

bool func_8013BE88(){
    void* inst = getInstance__14Class_80296898Fv();
    unsigned char b = ((unsigned char*)inst)[0x10];
    return b != 0;
}

int func_8013BEB8(){
    unsigned char* p = (unsigned char*)getInstance__14Class_80296898Fv();
    return p[0x21] != 0 ? 1 : 0;
}

int func_8013BEE8() {
    void* inst = getInstance__14Class_80296898Fv();
    u8 b = *(u8*)((u32)inst + 0x20);
    return (b != 0) ? 1 : 0;
}

extern u8 lbl_eu_80664064;
extern "C" void code80135FDC_setByte_64064() { lbl_eu_80664064 = 1; }

extern u8 lbl_eu_80664064;
void func_eu_8013C8E8() { lbl_eu_80664064 = 0; }

extern "C" int func_eu_8013C8F4() {
    if (lbl_eu_80664064 != 0) return 1;
    return ((u8*)getInstance__14Class_80296898Fv())[0x30] != 0;
}

extern "C" int func_8013BF48() {
    return ((u8*)getInstance__14Class_80296898Fv())[0x23] != 0;
}

extern "C" int func_8013BF78() {
    return ((u8*)getInstance__14Class_80296898Fv())[0x31] != 0;
}

int func_8013BFA8(){
    void* obj = getInstance__14Class_80296898Fv();
    return (static_cast<unsigned char*>(obj)[0x22] != 0) ? 1 : 0;
}

extern "C" int func_8013BFD8() {
    return ((u8*)getInstance__14Class_80296898Fv())[0x24] != 0;
}

extern "C" int func_8013C008() {
    return ((u8*)getInstance__14Class_80296898Fv())[0x25] != 0;
}

int func_8013C038(void* obj) {
    extern int func_8009CF8C(void*);
    int v = func_8009CF8C((void*)((u32)obj + 0x2CC8));
    return (v == 1) ? 1 : 0;
}