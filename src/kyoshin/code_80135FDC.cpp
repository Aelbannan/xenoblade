// Auto-scaffolded catalog TU for kyoshin/code_80135FDC
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
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
#include <string.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

extern "C" {
extern nw4r::lyt::Layout* createLayout__10CLibLayoutFv();
extern void getAllocHandle__10CLibLayoutFv();
extern void* allocate__Q23mtl10MemManagerFUlUl(u32, u32);
extern void Set__Q34nw4r3lyt12AnimResourceFPCv(void*, const void*);
extern void* getCurrentView__5CViewFv();
extern void* func_8049626C(void*, void*);
extern u32 identity__Q22ml6CMat34;
extern void SetFont__Q34nw4r3lyt7TextBoxFPCQ34nw4r2ut4Font(nw4r::lyt::TextBox*, const nw4r::ut::Font*);
extern void copyVEC3(nw4r::math::VEC3*, const nw4r::math::VEC3*);
extern void* getPlayer__Q22cf13CfGameManagerFi(int);
extern u32 func_8009CF8C(u32);
extern void func_8006A234(u16*, u16*);
extern int func_8006A6D0();
extern u16 func_8016DF2C();
extern void Panic__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
extern u8 lbl_eu_8052E524[];
extern u8 lbl_eu_8052E4F0[];
extern u32 lbl_eu_80500188;
extern u32 lbl_eu_805001C0;
extern char lbl_eu_80573B30[];
extern char lbl_eu_80573BB0[];
}

// ---------- globals ----------
extern "C" {
extern u8 lbl_eu_806621F0;
extern u8 lbl_eu_80664058;
extern u8 lbl_eu_80664059;
extern u8 lbl_eu_8066405A;
extern u8 lbl_eu_8066405B;
extern u32 lbl_eu_8066405C;
extern u32 lbl_eu_80664060;
extern u8 lbl_eu_80664064;
extern u32 lbl_eu_80664068;
extern u32 lbl_eu_8066406C;
extern u8 lbl_eu_80664070;
extern u8 lbl_eu_80664077;
extern u16 lbl_eu_80664078;
extern u8  lbl_eu_8066407E;
extern u8 lbl_eu_8066407F;
extern u8 lbl_eu_80664080;
extern u32 lbl_eu_806640EC;
extern u32 lbl_eu_80664098;
extern u32 lbl_eu_806640A8;
extern u32 lbl_eu_806640D8;
extern u32 lbl_eu_806640F0;
extern u32 lbl_eu_806640F4;
extern u32 lbl_eu_806640F8;
extern u32 lbl_eu_806640FC;
extern u32 lbl_eu_80664104;
extern u32 lbl_eu_80664108;
extern u32 lbl_eu_8066410C;
extern u32 lbl_eu_80664110;
extern u32 lbl_eu_80664184;
extern f32 lbl_eu_806672D8;
extern f32 lbl_eu_806672DC;
extern f32 lbl_eu_806672E0;
extern f32 lbl_eu_806672E4;
extern f32 lbl_eu_806672E8;
extern f32 lbl_eu_806672EC;
extern f32 lbl_eu_806672F0;
extern f64 lbl_eu_806672F8;
extern f32 lbl_eu_80667300;
extern f32 lbl_eu_80667304;
extern f32 lbl_eu_80667308;
extern f32 lbl_eu_8066730C;
extern f32 lbl_eu_80667310;
extern f32 lbl_eu_80667314;
extern f32 lbl_eu_80667318;
extern f32 lbl_eu_8066731C;
extern f32 lbl_eu_80667320;
extern f32 lbl_eu_80667324;
extern f32 lbl_eu_80667328;
extern f32 lbl_eu_8066732C;
extern f32 lbl_eu_80667330;
extern f32 lbl_eu_80667334;
extern f32 lbl_eu_80667338;
extern f32 lbl_eu_8066733C;
extern f32 lbl_eu_80667340;
extern f32 lbl_eu_80667344;
extern f32 lbl_eu_80667348;
extern f32 lbl_eu_80667350;
extern f32 lbl_eu_80667358;
extern f32 lbl_eu_80667360;
extern u32 lbl_eu_8066A208;
extern u32 lbl_eu_8066A20C;
extern u32 lbl_eu_806621F4;
extern u32 lbl_eu_80663E14;
extern u32 lbl_eu_80663E24;
extern char lbl_eu_80500664[];
}

extern "C" void func_8003AA34(const char*);
extern "C" void* getFP__FPCc(const char*);
extern "C" void* getBdatStringColumnValue(void*, const char*, const char*);

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
    void* fp = getFP__FPCc(a);
    return *(u8*)getBdatStringColumnValue(fp, b, c);
}

void func_80136254(){}

void func_801362C0(){}

void func_80136330(){}

void func_8013639C(){}

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
    u16* p = dst;
    int i = 0;
    while (*p != 0 && i < (int)destLen) {
        if (*p == 0x40u)
            *p = 0x0Au;
        p++;
        i++;
    }
    return 1;
}

void func_eu_80136F90(){}

void func_801364B8(){}

void func_801365E4(){}

void func_801366F4(){}

void func_8013676C(){}

extern "C" void func_80136A1C(nw4r::lyt::Layout* layout, char* name, char* text, u32 tagProc);

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

extern "C" void func_80136D74(
    nw4r::lyt::Layout* layout, char* text, u32 tagProc);

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
    getAllocHandle__10CLibLayoutFv();
    u32 handle = 0;
    void* mem = allocate__Q23mtl10MemManagerFUlUl(0x10, handle);
    if (mem != NULL) {
        Set__Q34nw4r3lyt12AnimResourceFPCv(mem, resource);
    }
    *ppAnimRes = mem;
    layout->BindAnimationAuto(
        *reinterpret_cast<nw4r::lyt::AnimResource*>(ppAnimRes), accessor);
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

extern "C" u32 func_80137444__FPQ34nw4r3lyt13AnimTransformf(
    nw4r::lyt::AnimTransform* anim, float delta) {
    float newFrame = delta + anim->GetFrame();
    if (CDeviceVI::isTvFormatPal()) {
        newFrame += 0.2f;
    }
    u16 frameSize = anim->GetFrameSize();
    if (newFrame >= static_cast<float>(frameSize)) {
        if (anim->IsLoopData()) {
            newFrame = 0.0f;
        } else {
            newFrame = static_cast<float>(frameSize) - 1.0f;
        }
        anim->SetFrame(newFrame);
        return 1;
    }
    anim->SetFrame(newFrame);
    return 0;
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

    const nw4r::math::VEC3& t = pane->GetTranslate();
    output->x = t.x;
    output->y = t.y;
    output->z = t.z;

    nw4r::lyt::Pane* parent = pane->GetParent();
    nw4r::math::VEC3 accum;
    accum.x = 0.0f; accum.y = 0.0f; accum.z = 0.0f;
    if (parent == NULL) {
        nw4r::math::VEC3Add(output, output, &accum);
        return;
    }

    accum = parent->GetTranslate();

    nw4r::math::VEC3 temp;
    temp.x = 0.0f; temp.y = 0.0f; temp.z = 0.0f;
    nw4r::lyt::Pane* grandparent = parent->GetParent();
    if (grandparent == NULL) {
        nw4r::math::VEC3Add(&accum, &accum, &temp);
        nw4r::math::VEC3Add(output, output, &accum);
        return;
    }

    temp = grandparent->GetTranslate();
    nw4r::math::VEC3 temp2;
    temp2.x = 0.0f; temp2.y = 0.0f; temp2.z = 0.0f;
    nw4r::lyt::Pane* greatGrandparent = grandparent->GetParent();
    if (greatGrandparent == NULL) {
        nw4r::math::VEC3Add(&temp, &temp, &temp2);
        nw4r::math::VEC3Add(&accum, &accum, &temp);
        nw4r::math::VEC3Add(output, output, &accum);
        return;
    }

    temp2 = greatGrandparent->GetTranslate();
    nw4r::math::VEC3 recurse;
    func_801375A0(&recurse, greatGrandparent->GetParent());
    nw4r::math::VEC3Add(&temp2, &temp2, &recurse);
    nw4r::math::VEC3Add(&temp, &temp, &temp2);
    nw4r::math::VEC3Add(&accum, &accum, &temp);
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
    if (node == NULL) return;
    if (node->GetParent() == NULL) return;

    output->x = node->GetTranslate().x;
    output->y = node->GetTranslate().y;
    output->z = node->GetTranslate().z;

    nw4r::lyt::Pane* parent = node->GetParent();
    nw4r::math::VEC3 accum;
    accum.x = 0.0f;
    accum.y = 0.0f;
    accum.z = 0.0f;
    if (parent == NULL) {
        nw4r::math::VEC3Add(output, output, &accum);
        return;
    }
    nw4r::lyt::Pane* pp = parent->GetParent();
    if (pp == NULL) {
        nw4r::math::VEC3Add(output, output, &accum);
        return;
    }

    accum.x = parent->GetTranslate().x;
    accum.y = parent->GetTranslate().y;
    accum.z = parent->GetTranslate().z;

    nw4r::math::VEC3 temp;
    temp.x = 0.0f;
    temp.y = 0.0f;
    temp.z = 0.0f;
    if (pp == NULL) {
        nw4r::math::VEC3Add(&accum, &accum, &temp);
        nw4r::math::VEC3Add(output, output, &accum);
        return;
    }
    nw4r::lyt::Pane* gp = pp->GetParent();
    if (gp == NULL) {
        nw4r::math::VEC3Add(&accum, &accum, &temp);
        nw4r::math::VEC3Add(output, output, &accum);
        return;
    }

    temp.x = pp->GetTranslate().x;
    temp.y = pp->GetTranslate().y;
    temp.z = pp->GetTranslate().z;

    nw4r::math::VEC3 tmp2;
    code80135FDC_setVec3((float*)&tmp2, 0.0f, 0.0f, 0.0f);
    if (gp == NULL) {
        nw4r::math::VEC3Add(&temp, &temp, &tmp2);
        nw4r::math::VEC3Add(&accum, &accum, &temp);
        nw4r::math::VEC3Add(output, output, &accum);
        return;
    }
    nw4r::lyt::Pane* ggp = gp->GetParent();
    if (ggp == NULL) {
        nw4r::math::VEC3Add(&temp, &temp, &tmp2);
        nw4r::math::VEC3Add(&accum, &accum, &temp);
        nw4r::math::VEC3Add(output, output, &accum);
        return;
    }

    copyVEC3(&tmp2, &gp->GetTranslate());
    nw4r::math::VEC3 recurse;
    func_8013775C(&recurse, ggp);
    func_80137738(&tmp2, &recurse);
    nw4r::math::VEC3Add(&temp, &temp, &tmp2);
    nw4r::math::VEC3Add(&accum, &accum, &temp);
    nw4r::math::VEC3Add(output, output, &accum);
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

void func_80137E7C(){}

extern "C" void func_80137E7C(void* a, u32 b, void* palette) {
    if (a == NULL) return;
    void* obj = *(void**)((u8*)a + 0x10);
    void** vt = *(void***)obj;
    void* res = ((void*(*)(void*, u32, u32))vt[0x3C / 4])(obj, b, 1);
    if (res == NULL) return;
    if (palette == NULL) return;

    void** vt2 = *(void***)res;
    nw4r::lyt::Material* mat = (nw4r::lyt::Material*)((void*(*)(void*))vt2[0x68 / 4])(res);

    nw4r::lyt::TexMap texMap((TPLPalette*)palette, 0);
    if (mat->GetTextureNum() == 0) {
        Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052E524, 0x88,
                                   (const char*)lbl_eu_8052E4F0);
    }
    mat->GetTexMapAry()[0] = texMap;
}

extern "C" void func_80137F88(void* a, void* palette) {
    if (a == NULL) return;
    if (palette == NULL) return;

    void** vt = *(void***)a;
    nw4r::lyt::Material* mat = (nw4r::lyt::Material*)((void*(*)(void*))vt[0x68 / 4])(a);

    nw4r::lyt::TexMap texMap((TPLPalette*)palette, 0);
    if (mat->GetTextureNum() == 0) {
        Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052E524, 0x88,
                                   (const char*)lbl_eu_8052E4F0);
    }
    mat->GetTexMapAry()[0] = texMap;
}

void func_801380A0(){}

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
    Table_80500188 t = *(Table_80500188*)&lbl_eu_80500188;
    return ((u16*)&t)[idx - 1];
}

extern "C" u32 func_80138138(u32 val) {
    u32 words[28];
    for (int i = 0; i < 28; i++) {
        words[i] = ((u32*)&lbl_eu_805001C0)[i];
    }
    for (int i = 27; i >= 0; i--) {
        if (val < words[i]) return i;
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
    void* player = getPlayer__Q22cf13CfGameManagerFi(0);
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

void func_80138DA4(){}

void func_80138DA4(){}

void func_80138E1C(){}

void func_80138E90(){}

void func_80138F78(){}

void func_8013902C(){}

void func_801390E0__FPP11CFileHandle(void** handlePtr) {
    extern void cancel__11CDeviceFileFP11CFileHandle(void*);
    if (*handlePtr) {
        cancel__11CDeviceFileFP11CFileHandle(*handlePtr);
        *handlePtr = 0;
    }
}

void func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(){}

void func_80139198(){}



extern u8 lbl_eu_80664077;
u8 code80135FDC_getByte_64077() { return lbl_eu_80664077; }

u8 func_801392C8(u8 idx) {
    extern u16 lbl_eu_80664078;
    return *((u8*)&lbl_eu_80664078 + idx * 2 + 1);
}

extern u8 lbl_eu_8066407E;
u8 code80135FDC_getByte_6407E() { return lbl_eu_8066407E; }

void func_801392E4(){}

void func_80139358(){}

void func_801393CC(){}

void func_801394D4(){}

void func_80139658(){}

void func_801397AC(){}

void func_801398A4(){}

void func_8013996C(){}

void func_80139A18(){}

void func_80139AC8(){}

void func_80139B5C(){}

void func_80139BF4(){}

void func_80139C98(){}

void func_80139CEC(){}

void func_8013A4B4(){}

void func_8013A7D0(){}

void func_8013A95C(){}

void func_8013AB0C(){}

void func_8013AC3C(){}

void func_8013ACFC(){}

void func_8013B1C4(){}

void func_8013B2D4(){}

void func_8013B380(){}

void func_8013B428__FUl(){}

extern u8 lbl_eu_8066407F;
u8 code80135FDC_setByte_6407F(u8 val) { lbl_eu_8066407F = val; return val; }

extern u8 lbl_eu_8066407F;
u8 code80135FDC_getByte_6407F() { return lbl_eu_8066407F; }

void func_8013B88C(){}

unsigned char code80135FDC_postIncByte_64080() {
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
u8 code80135FDC_getByte_64080() { return lbl_eu_80664080; }

void func_8013B9AC(){}

void func_8013BAD8(){}

void func_8013BC0C(){}

void func_8013BCD4(){}

void func_8013BD24(){}


extern u8 lbl_eu_80664058;
void code80135FDC_clearByte_64058() { lbl_eu_80664058 = 0; }

extern u8 lbl_eu_80664058;
u8 code80135FDC_getByte_64058() { return lbl_eu_80664058; }

void func_8013BDBC() {
    extern void func_8013EC6C(u32, u32);
    lbl_eu_80664059 = 1;
    func_8013EC6C(0, 0);
}

extern u8 lbl_eu_80664059;
void code80135FDC_clearByte_64059() { lbl_eu_80664059 = 0; }

extern u8 lbl_eu_80664059;
u8 code80135FDC_getByte_64059() { return lbl_eu_80664059; }


extern u8 lbl_eu_8066405B;
void code80135FDC_setByte_6405B() { lbl_eu_8066405B = 1; }

extern u8 lbl_eu_8066405A;
u8 code80135FDC_getByte_6405A() { return lbl_eu_8066405A; }

extern u8 lbl_eu_8066405B;
u8 code80135FDC_getByte_6405B() { return lbl_eu_8066405B; }

void code80135FDC_setPair_6405C_64060(u32 a, u32 b) {
    extern u32 lbl_eu_8066405C;
    extern u32 lbl_eu_80664060;
    lbl_eu_8066405C = a;
    lbl_eu_80664060 = b;
}

u32 code80135FDC_getWord_6405C(void) {
    extern u32 lbl_eu_8066405C;
    return lbl_eu_8066405C;
}

u32 code80135FDC_getWord_64060(void) {
    extern u32 lbl_eu_80664060;
    return lbl_eu_80664060;
}


extern u8 lbl_eu_806621F0;
u8 code80135FDC_getByte_621F0() { return lbl_eu_806621F0; }

void* getInstance__14Class_80296898Fv();

int func_8013BE58(){
    unsigned char byte = ((unsigned char*)getInstance__14Class_80296898Fv())[0x11];
    return byte != 0 ? 1 : 0;
}

bool func_8013BE88(){
    extern void* getInstance__14Class_80296898Fv();
    void* inst = getInstance__14Class_80296898Fv();
    unsigned char b = ((unsigned char*)inst)[0x10];
    return b != 0;
}

int func_8013BEB8(){
    extern void* getInstance__14Class_80296898Fv();
    unsigned char* p = (unsigned char*)getInstance__14Class_80296898Fv();
    return p[0x21] != 0 ? 1 : 0;
}

int func_8013BEE8() {
    void* getInstance__14Class_80296898Fv();
    void* inst = getInstance__14Class_80296898Fv();
    u8 b = *(u8*)((u32)inst + 0x20);
    return (b != 0) ? 1 : 0;
}

extern u8 lbl_eu_80664064;
void code80135FDC_setByte_64064() { lbl_eu_80664064 = 1; }

extern u8 lbl_eu_80664064;
void func_eu_8013C8E8() { lbl_eu_80664064 = 0; }

void func_eu_8013C8F4(){}

void func_8013BF48(){}

void func_8013BF78(){}

int func_8013BFA8(){
    void* getInstance__14Class_80296898Fv();
    void* obj = getInstance__14Class_80296898Fv();
    return (static_cast<unsigned char*>(obj)[0x22] != 0) ? 1 : 0;
}

void func_8013BFD8(){}

void func_8013C008(){}

int func_8013C038(void* obj) {
    extern int func_8009CF8C(void*);
    int v = func_8009CF8C((void*)((u32)obj + 0x2CC8));
    return (v == 1) ? 1 : 0;
}
