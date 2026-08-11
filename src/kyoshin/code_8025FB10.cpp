// Auto-scaffolded catalog TU for kyoshin/code_8025FB10
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "monolib/device/CDeviceVI.hpp"

#include <nw4r/ut/ut_TextWriterBase.h>
#include <revolution/GX.h>
#include <revolution/MTX.h>

namespace {

struct GXCacheTextProjection {
    u8 padding[0x4bc];
    s16 width;
    s16 height;
};

#define VALIDATE_NW4R_POINTER(pointer, file, line, message)                    \
    {                                                                         \
        u32 address = (u32)(pointer);                                          \
        bool validMem1 = true;                                                 \
        bool validMem2 = true;                                                 \
        bool validIo = true;                                                   \
        bool validIo2 = true;                                                  \
        bool validRegs = true;                                                 \
        bool validRegs2 = true;                                                \
        if ((address & 0xFF000000) != 0x80000000 &&                            \
            (address & 0xFF800000) != 0x81000000) {                            \
            validMem1 = false;                                                 \
        }                                                                      \
        if (!validMem1 && (address & 0xF8000000) != 0x90000000) {              \
            validMem2 = false;                                                 \
        }                                                                      \
        if (!validMem2 && (address & 0xFF000000) != 0xC0000000) {              \
            validIo = false;                                                   \
        }                                                                      \
        if (!validIo && (address & 0xFF800000) != 0xC1000000) {                \
            validIo2 = false;                                                  \
        }                                                                      \
        if (!validIo2 && (address & 0xF8000000) != 0xD0000000) {               \
            validRegs = false;                                                 \
        }                                                                      \
        if (!validRegs && (address & 0xFFFFC000) != 0xE0000000) {              \
            validRegs2 = false;                                                \
        }                                                                      \
        if (!validRegs2) {                                                     \
            Panic__Q24nw4r2dbFPCciPCce(file, line, message, pointer);          \
        }                                                                      \
    }

extern "C" {
void Panic__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);

extern void* cacheInstance__9CDeviceGX;
u8 func_8044BE24__8CGXCacheFv(void*);
s16* func_8044BE1C__8CGXCacheFv(void*);

extern void* lbl_eu_80664860;
extern nw4r::ut::TagProcessorBase<wchar_t>* lbl_eu_8066486C;
const nw4r::ut::Font* func_80449160__10CFontLayerFv(void*, int);

extern char lbl_eu_805377CC[];
extern char lbl_eu_80537798[];
extern char lbl_eu_80537784[];
extern char lbl_eu_80537748[];
extern char lbl_eu_8052DC70[];
extern char lbl_eu_8052DC3C[];
extern char lbl_eu_8052DD84[];
extern char lbl_eu_8052DD50[];
extern char lbl_eu_8052DC28[];
extern char lbl_eu_8052DBF4[];
extern char lbl_eu_8053785C[];
extern char lbl_eu_80537828[];
extern char lbl_eu_80537818[];
extern char lbl_eu_805377E0[];
extern char lbl_eu_8052DCFC[];
extern char lbl_eu_8052DCC8[];
extern char lbl_eu_805378A0[];
extern char lbl_eu_8053786C[];
extern char lbl_eu_80537734[];
extern char lbl_eu_80537700[];
extern char lbl_eu_805376EC[];
extern char lbl_eu_805376B8[];

extern const f32 lbl_eu_806688D0;
extern const f32 lbl_eu_806688D4;
extern const f32 lbl_eu_806688D8;
extern const f32 lbl_eu_806688DC;
extern const f32 lbl_eu_806688E0;
}

#define VALIDATE_NW4R_POINTER_COMPACT(pointer, file, line, message)            \
    {                                                                         \
        u32 address = (u32)(pointer);                                          \
        if (!((address & 0xFF000000) == 0x80000000 ||                          \
              (address & 0xFF800000) == 0x81000000 ||                          \
              (address & 0xF8000000) == 0x90000000 ||                          \
              (address & 0xFF000000) == 0xC0000000 ||                          \
              (address & 0xFF800000) == 0xC1000000 ||                          \
              (address & 0xF8000000) == 0xD0000000 ||                          \
              (address & 0xFFFFC000) == 0xE0000000)) {                         \
            Panic__Q24nw4r2dbFPCciPCce(file, line, message, pointer);          \
        }                                                                      \
    }

#define VALIDATE_NW4R_POINTER_FLAG(pointer, file, line, message)               \
    {                                                                         \
        u32 address = (u32)(pointer);                                          \
        bool valid = false;                                                    \
        if ((address & 0xFF000000) == 0x80000000 ||                            \
            (address & 0xFF800000) == 0x81000000 ||                            \
            (address & 0xF8000000) == 0x90000000 ||                            \
            (address & 0xFF000000) == 0xC0000000 ||                            \
            (address & 0xFF800000) == 0xC1000000 ||                            \
            (address & 0xF8000000) == 0xD0000000 ||                            \
            (address & 0xFFFFC000) == 0xE0000000) {                            \
            valid = true;                                                      \
        }                                                                      \
        if (!valid) {                                                         \
            Panic__Q24nw4r2dbFPCciPCce(file, line, message, pointer);          \
        }                                                                      \
    }

typedef nw4r::ut::TextWriterBase<wchar_t> WideTextWriter;

static inline void setTagProcessorChecked(
    WideTextWriter* writer,
    nw4r::ut::TagProcessorBase<wchar_t>* processor) {
    VALIDATE_NW4R_POINTER(writer, lbl_eu_805377CC, 151,
                          lbl_eu_80537798);
    VALIDATE_NW4R_POINTER(processor, lbl_eu_80537784, 152,
                          lbl_eu_80537748);
    writer->SetTagProcessor(processor);
}

static inline void setDrawFlagChecked(WideTextWriter* writer, u32 flag) {
    VALIDATE_NW4R_POINTER_COMPACT(writer, lbl_eu_8052DC70, 139,
                          lbl_eu_8052DC3C);
    writer->SetDrawFlag(flag);
}

static inline void setScaleChecked(WideTextWriter* writer, f32 x, f32 y) {
    VALIDATE_NW4R_POINTER_COMPACT(writer, lbl_eu_8052DD84, 171,
                          lbl_eu_8052DD50);
    writer->SetScale(x, y);
}

static inline void setCharSpaceChecked(WideTextWriter* writer, f32 space) {
    VALIDATE_NW4R_POINTER_COMPACT(writer, lbl_eu_8052DC28, 98,
                          lbl_eu_8052DBF4);
    writer->SetCharSpace(space);
}

static inline void setFontChecked(WideTextWriter* writer,
                                  const nw4r::ut::Font* font) {
    VALIDATE_NW4R_POINTER(writer, lbl_eu_8053785C, 65,
                          lbl_eu_80537828);
    VALIDATE_NW4R_POINTER(font, lbl_eu_80537818, 66,
                          lbl_eu_805377E0);
    writer->SetFont(*font);
}

static inline void setTextColorChecked(WideTextWriter* writer,
                                       nw4r::ut::Color color) {
    VALIDATE_NW4R_POINTER_COMPACT(writer, lbl_eu_8052DCFC, 135,
                          lbl_eu_8052DCC8);
    writer->SetTextColor(color);
}

static inline void setCursorChecked(WideTextWriter* writer, f32 x, f32 y,
                                    f32 z) {
    VALIDATE_NW4R_POINTER_FLAG(writer, lbl_eu_805378A0, 258,
                          lbl_eu_8053786C);
    writer->SetCursor(x, y, z);
}

static inline void printCheckedInitial(WideTextWriter* writer,
                                const wchar_t* text) {
    VALIDATE_NW4R_POINTER_FLAG(writer, lbl_eu_80537734, 256,
                          lbl_eu_80537700);
    VALIDATE_NW4R_POINTER(text, lbl_eu_805376EC, 257,
                          lbl_eu_805376B8);
    writer->Print(text, static_cast<int>(wcslen(text)));
}

static inline void printChecked(WideTextWriter* writer,
                                const wchar_t* text) {
    VALIDATE_NW4R_POINTER_FLAG(writer, lbl_eu_80537734, 256,
                          lbl_eu_80537700);
    VALIDATE_NW4R_POINTER_FLAG(text, lbl_eu_805376EC, 257,
                          lbl_eu_805376B8);
    writer->Print(text, static_cast<int>(wcslen(text)));
}

} // namespace

void func_8025FB10(){}

void func_8025FD60(){}

void func_8025FDB8(){}

void func_80260010(){}

void func_80260264(){}

void func_80260518(){}

void func_80260A6C(){}

void func_80260FB0(){}

void func_8026178C(){}

void func_802617B8(){}

void func_80261844(){}

void func_8026187C(){}

void func_802618AC(u8* obj, int value) {
    int* field = (int*)(obj + 0x884);
    *field = value;
    if (value < 0) *field = 0;
    else if (value > 0x3e7) *field = 0x3e7;
}

void func_eu_80263A24(){}

void func_802618D8(){}

int lbl_eu_80664864;
int lbl_eu_80664868;
int lbl_eu_80662980;
int lbl_eu_80664874;

void func_80261944(int arg) {
    lbl_eu_80664864 = arg;
    lbl_eu_80664868 = arg;
    lbl_eu_80662980 = -1;
    lbl_eu_80664874 = 0;
}

void func_80261960(){}

void func_80261A80(){}

void __dt__80261B1C(){}

void func_80261B98(const wchar_t* text, f32 x, f32 y) {
    Mtx identity;
    Mtx44 projection;

    PSMTXIdentity(identity);
    GXLoadPosMtxImm(identity, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    GXCacheTextProjection* cache =
        static_cast<GXCacheTextProjection*>(cacheInstance__9CDeviceGX);
    if (!func_8044BE24__8CGXCacheFv(cache)) {
        f32 width = static_cast<f32>(CDeviceVI::getRenderModeObj()->fbWidth);
        f32 height = static_cast<f32>(CDeviceVI::getRenderModeObj()->efbHeight);
        C_MTXOrtho(projection, lbl_eu_806688D0, height,
                   lbl_eu_806688D0, width, lbl_eu_806688D0,
                   lbl_eu_806688D4);
    } else {
        s16 cacheWidth = cache->width;
        f32 renderWidth =
            static_cast<f32>(CDeviceVI::getRenderModeObj()->fbWidth);
        s16* rect = func_8044BE1C__8CGXCacheFv(cache);
        f32 right = static_cast<f32>(cacheWidth) *
                    (static_cast<f32>(rect[2]) / renderWidth);

        cacheWidth = cache->width;
        renderWidth =
            static_cast<f32>(CDeviceVI::getRenderModeObj()->fbWidth);
        rect = func_8044BE1C__8CGXCacheFv(cache);
        f32 left = static_cast<f32>(cacheWidth) *
                   (static_cast<f32>(rect[0]) / renderWidth);

        s16 cacheHeight = cache->height;
        f32 renderHeight =
            static_cast<f32>(CDeviceVI::getRenderModeObj()->efbHeight);
        rect = func_8044BE1C__8CGXCacheFv(cache);
        f32 bottom = static_cast<f32>(cacheHeight) *
                     (static_cast<f32>(rect[3]) / renderHeight);

        cacheHeight = cache->height;
        renderHeight =
            static_cast<f32>(CDeviceVI::getRenderModeObj()->efbHeight);
        rect = func_8044BE1C__8CGXCacheFv(cache);
        f32 top = static_cast<f32>(cacheHeight) *
                  (static_cast<f32>(rect[1]) / renderHeight);

        C_MTXOrtho(projection, top, bottom, left, right,
                   lbl_eu_806688D0, lbl_eu_806688D4);
    }
    GXSetProjection(projection, GX_ORTHOGRAPHIC);

    WideTextWriter writer;
    writer.SetupGX();

    setTagProcessorChecked(&writer, lbl_eu_8066486C);
    setDrawFlagChecked(&writer, 0x110);
    setScaleChecked(&writer, lbl_eu_806688D8, lbl_eu_806688DC);
    setCharSpaceChecked(&writer, lbl_eu_806688D0);

    const nw4r::ut::Font* font = func_80449160__10CFontLayerFv(
        static_cast<u8*>(lbl_eu_80664860) + 0x1c4, 1);
    setFontChecked(&writer, font);
    setTextColorChecked(&writer, nw4r::ut::Color(0, 0, 0, 255));

    setCursorChecked(&writer, x - lbl_eu_806688D8,
                     y - lbl_eu_806688D8, lbl_eu_806688E0);
    printCheckedInitial(&writer, text);
    setCursorChecked(&writer, x + lbl_eu_806688D8,
                     y - lbl_eu_806688D8, lbl_eu_806688E0);
    printChecked(&writer, text);
    setCursorChecked(&writer, x - lbl_eu_806688D8,
                     y + lbl_eu_806688D8, lbl_eu_806688E0);
    printChecked(&writer, text);
    setCursorChecked(&writer, x + lbl_eu_806688D8,
                     y + lbl_eu_806688D8, lbl_eu_806688E0);
    printChecked(&writer, text);

    setTextColorChecked(&writer, nw4r::ut::Color(255, 255, 255, 255));
    setCursorChecked(&writer, x, y, lbl_eu_806688D0);
    printChecked(&writer, text);
    setCursorChecked(&writer, x, y, lbl_eu_806688D0);
    printChecked(&writer, text);
    setCursorChecked(&writer, x, y, lbl_eu_806688D0);
    printChecked(&writer, text);
}

#undef VALIDATE_NW4R_POINTER_FLAG
#undef VALIDATE_NW4R_POINTER_COMPACT
#undef VALIDATE_NW4R_POINTER
