// Auto-scaffolded catalog TU for kyoshin/code_80135FDC
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include <nw4r/math/math_types.h>
#include <nw4r/lyt/lyt_animation.h>
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_textBox.h>
#include <nw4r/lyt/lyt_drawInfo.h>
#include <nw4r/lyt/lyt_arcResourceAccessor.h>
#include "monolib/device/CDeviceVI.hpp"
#include <revolution/gx/GXTransform.h>
#include <revolution/gx/GXPixel.h>
#include <revolution/mtx/mtx44.h>
#include <revolution/enc/encunicode.h>
#include <string.h>
#include <wchar.h>
#include <stdio.h>

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

void func_80136400(){}

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

void func_8013775C(){}

void func_80137924(){}

void func_80137B44(){}

void func_80137C1C(){}

void func_80137CD4(){}

void func_80137DB8(){}

void func_80137E7C(){}

void func_80137F88(){}

void func_80138078__FUl(u32 arg) {
    extern void func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(u32, u32, u32, u32, f32);
    extern f32 lbl_eu_80667300;
    func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, arg, 0, 0, lbl_eu_80667300);
}

void code80135FDC_thunk_BFE8C(u8* arg) {
    extern void func_801BFE8C(unsigned long, void*, unsigned long);
    func_801BFE8C(0, arg, 0);
}

void func_801380A0(){}

void func_80138138(){}

void func_80138234(){}

void func_80138574(){}

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
