#include <nw4hbm/ut.h>

namespace {

// Opaque word-fill of the color state (mapping + vertex + text, 0x20 bytes):
// runs as part of member initialization so the white fill is emitted before
// the remaining member stores, matching retail's ordering.
void FillColorsWhite(GXColor* colors) {
    u32* words = reinterpret_cast<u32*>(colors);
    for (int i = 0; i < 8; i++) {
        words[i] = 0xFFFFFFFF;
    }
}

} // namespace

namespace nw4hbm {
namespace ut {

/* Defined inline here so MWCC inlines away all ~Color calls in this TU
 * (retail CharWriter emits no dtor calls; the strong ~Color symbol lives
 * in lyt_bounding.cpp). */
inline Color::~Color() {}

CharWriter::LoadingTexture CharWriter::mLoadingTexture;

CharWriter::CharWriter()
    : mAlpha((FillColorsWhite(reinterpret_cast<GXColor*>(&mColorMapping)),
              255)),
      mIsWidthFixed(false), mFixedWidth(0.0f), mFont(NULL) {

    mLoadingTexture.Reset();
    ResetColorMapping();
    SetGradationMode(GRADMODE_NONE);
    SetTextColor(Color::WHITE);
    SetScale(1.0f, 1.0f);
    SetCursor(0.0f, 0.0f, 0.0f);
    EnableLinearFilter(true, true);
}

CharWriter::~CharWriter() {}

namespace {

// Mirror of ut::Color without the non-trivial destructor: the guarded static
// below must be initialized with a plain store and no destruction
// registration to match the retail inlined copies.
struct SetupColor : GXColor {
    SetupColor(u32 color) { *reinterpret_cast<u32*>(this) = color; }
};

static void SetupGXCommon() {
    static const SetupColor fog(0);

    GXSetFog(GX_FOG_NONE, fog, 0.0f, 0.0f, 0.0f, 0.0f);
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE,
                          GX_CH_ALPHA);
    GXSetZTexture(GX_ZT_DISABLE, GX_TF_Z8, 0);

    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL,
                  GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL,
                  GX_DF_NONE, GX_AF_NONE);

    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY,
                      FALSE, GX_PTIDENTITY);

    GXSetNumIndStages(0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
}

} // namespace

void CharWriter::SetFont(const Font& rFont) {
        mFont = &rFont;
}

const Font* CharWriter::GetFont() const {
    return mFont;
}

void CharWriter::SetupGX() {
    ResetTextureCache();

    if (*reinterpret_cast<u32*>(&mColorMapping.min) != DEFAULT_COLOR_MAPPING_MIN ||
        *reinterpret_cast<u32*>(&mColorMapping.max) != DEFAULT_COLOR_MAPPING_MAX) {
        SetupGXWithColorMapping(ut::Color(mColorMapping.min),
                                ut::Color(mColorMapping.max));
        return;
    }

    if (mFont != NULL) {
        switch (mFont->GetTextureFormat()) {
        case GX_TF_I4:
        case GX_TF_I8: {
            SetupGXForI();
            break;
        }

        case GX_TF_IA4:
        case GX_TF_IA8: {
            SetupGXDefault();
            break;
        }

        case GX_TF_RGB565:
        case GX_TF_RGB5A3:
        case GX_TF_RGBA8: {
            SetupGXForRGBA();
            break;
        }

        default: {
            SetupGXDefault();
            break;
        }
        }
    } else {
        SetupGXDefault();
    }
}

void CharWriter::SetColorMapping(Color min, Color max) {
    mColorMapping.min = min;
    mColorMapping.max = max;
}

void CharWriter::SetGradationMode(GradationMode mode) {
    mTextColor.gradationMode = mode;
    UpdateVertexColor();
}

void CharWriter::SetTextColor(Color start, Color end) {
    mTextColor.start = start;
    mTextColor.end = end;
    UpdateVertexColor();
}

f32 CharWriter::GetScaleH() const {
    return mScale.x;
}
f32 CharWriter::GetScaleV() const {
    return mScale.y;
}

void CharWriter::SetFontSize(f32 width, f32 height) {
    SetScale(width / mFont->GetWidth(), height / mFont->GetHeight());
}

void CharWriter::SetFontSize(f32 height){
}

f32 CharWriter::GetFontWidth() const {
    return mScale.x * mFont->GetWidth();
}

f32 CharWriter::GetFontHeight() const {
    return mScale.y * mFont->GetHeight();
}

f32 CharWriter::GetFontAscent() const {
    return mScale.y * mFont->GetAscent();
}

f32 CharWriter::GetFontDescent() const {
    return mScale.y * mFont->GetDescent();
}

bool CharWriter::IsWidthFixed() const {
    return mIsWidthFixed;
}

f32 CharWriter::GetFixedWidth() const {
    return mFixedWidth;
}

void CharWriter::EnableLinearFilter(bool atSmall, bool atLarge) {
    mFilter.atSmall = atSmall ? GX_LINEAR : GX_NEAR;
    mFilter.atLarge = atLarge ? GX_LINEAR : GX_NEAR;
}

f32 CharWriter::Print(u16 ch) {
    f32 width;
    f32 left;

    Glyph glyph;
    mFont->GetGlyph(&glyph, ch);

    if (mIsWidthFixed) {
        f32 margin = (mFixedWidth - glyph.widths.charWidth * mScale.x) / 2;
        width = mFixedWidth;
        left = margin + glyph.widths.left * mScale.x;
    } else {
        width = glyph.widths.charWidth * mScale.x;
        left = glyph.widths.left * mScale.x;
    }

    PrintGlyph(mCursorPos.x + left, mCursorPos.y, mCursorPos.z, glyph);
    mCursorPos.x += width;

    return width;
}

void CharWriter::SetCursor(f32 x, f32 y) {
    mCursorPos.x = x;
    mCursorPos.y = y;
}

void CharWriter::SetCursorX(f32 x) {
    mCursorPos.x = x;
}

void CharWriter::SetCursorY(f32 y) {
    mCursorPos.y = y;
}

void CharWriter::MoveCursorX(f32 dx) {
    mCursorPos.x += dx;
}

void CharWriter::MoveCursorY(f32 dy) {
    mCursorPos.y += dy;
}

f32 CharWriter::GetCursorX() const {
    return mCursorPos.x;
}

f32 CharWriter::GetCursorY() const {
    return mCursorPos.y;
}

//unused
void CharWriter::DrawGlyph(const Glyph& glyph) {
}

void CharWriter::PrintGlyph(f32 x, f32 y, f32 z, const Glyph& rGlyph) {
    f32 x2 = x + rGlyph.widths.glyphWidth * mScale.x;
    f32 y2 = y + rGlyph.height * mScale.y;

    u16 texLeft = rGlyph.cellX * 0x8000U / rGlyph.texWidth;
    u16 texTop = rGlyph.cellY * 0x8000U / rGlyph.texHeight;

    u16 texRight =
        (rGlyph.cellX + rGlyph.widths.glyphWidth) * 0x8000U / rGlyph.texWidth;

    u16 texBottom = (rGlyph.cellY + rGlyph.height) * 0x8000U / rGlyph.texHeight;

    LoadTexture(rGlyph, GX_TEXMAP0);

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    {
        GXPosition3f32(x, y, z);
        GXColor1u32(*reinterpret_cast<u32*>(&mVertexColor.lu));
        GXTexCoord2s16(texLeft, texTop);

        GXPosition3f32(x2, y, z);
        GXColor1u32(*reinterpret_cast<u32*>(&mVertexColor.ru));
        GXTexCoord2s16(texRight, texTop);

        GXPosition3f32(x2, y2, z);
        GXColor1u32(*reinterpret_cast<u32*>(&mVertexColor.rd));
        GXTexCoord2s16(texRight, texBottom);

        GXPosition3f32(x, y2, z);
        GXColor1u32(*reinterpret_cast<u32*>(&mVertexColor.ld));
        GXTexCoord2s16(texLeft, texBottom);
    }
    GXEnd();
}

void CharWriter::LoadTexture(const Glyph& rGlyph, GXTexMapID slot) {
    LoadingTexture loadingTexture;

    loadingTexture.slot = slot;
    loadingTexture.texture = rGlyph.pTexture;
    loadingTexture.filter = mFilter;

    if (loadingTexture != mLoadingTexture) {
        GXTexObj texObj;
        GXInitTexObj(&texObj, rGlyph.pTexture, rGlyph.texWidth,
                     rGlyph.texHeight, rGlyph.texFormat, GX_CLAMP, GX_CLAMP,
                     FALSE);

        GXInitTexObjLOD(&texObj, mFilter.atSmall, mFilter.atLarge, 0.0f, 0.0f,
                        0.0f, FALSE, FALSE, GX_ANISO_1);

        GXLoadTexObj(&texObj, slot);

        mLoadingTexture = loadingTexture;
    }
}

void CharWriter::UpdateVertexColor() {
    // clang-format off
    mVertexColor.lu = mTextColor.start;
    mVertexColor.ru = mTextColor.gradationMode == GRADMODE_H    ? mTextColor.end   : mTextColor.start;
    mVertexColor.ld = mTextColor.gradationMode == GRADMODE_V    ? mTextColor.end   : mTextColor.start;
    mVertexColor.rd = mTextColor.gradationMode != GRADMODE_NONE ? mTextColor.end   : mTextColor.start;
    // clang-format on

    mVertexColor.lu.a = (mVertexColor.lu.a * mAlpha) / 255,
    mVertexColor.ru.a = (mVertexColor.ru.a * mAlpha) / 255;
    mVertexColor.ld.a = (mVertexColor.ld.a * mAlpha) / 255;
    mVertexColor.rd.a = (mVertexColor.rd.a * mAlpha) / 255;
}

void CharWriter::SetupVertexFormat() {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U16, 15);

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
}

void CharWriter::SetupGXDefault() {
    SetupGXCommon();

    GXSetNumTevStages(1);

    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);

    SetupVertexFormat();
}

void CharWriter::SetupGXWithColorMapping(Color min, Color max) {
    SetupGXCommon();

    GXSetNumTevStages(2);

    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevDirect(GX_TEVSTAGE1);

    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);

    GXSetTevColor(GX_TEVREG0, min);
    GXSetTevColor(GX_TEVREG1, max);

    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_A1, GX_CA_TEXA, GX_CA_ZERO);

    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE,
                    GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE,
                    GX_TEVPREV);

    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);

    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC,
                    GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA,
                    GX_CA_ZERO);

    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE,
                    GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE,
                    GX_TEVPREV);

    SetupVertexFormat();
}

void CharWriter::SetupGXForI() {
    SetupGXCommon();

    GXSetNumTevStages(1);

    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);

    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
                    GX_CC_RASC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA,
                    GX_CA_ZERO);

    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE,
                    GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE,
                    GX_TEVPREV);

    SetupVertexFormat();
}

void CharWriter::SetupGXForRGBA() {
    SetupGXDefault();
}
} // namespace ut
} // namespace nw4hbm
