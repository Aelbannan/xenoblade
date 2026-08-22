#include <nw4r/lyt.h>

#include <revolution/GX.h>
#include <revolution/TPL.h>

namespace nw4r {
namespace lyt {

void TexMap::Get(GXTexObj* pTexObj) const {
    if (detail::IsCITexelFormat(GetTexelFormat())) {
        u32 tlut = GXGetTexObjTlut(pTexObj);

        GXInitTexObjCI(pTexObj, GetImage(), GetWidth(), GetHeight(),
                       GetTexelFormat(), GetWrapModeS(), GetWrapModeT(),
                       IsMipMap(), tlut);
    } else {
        GXInitTexObj(pTexObj, GetImage(), GetWidth(), GetHeight(),
                     GetTexelFormat(), GetWrapModeS(), GetWrapModeT(),
                     IsMipMap());
    }

    GXInitTexObjLOD(pTexObj, GetMinFilter(), GetMagFilter(), GetMinLOD(),
                    GetMaxLOD(), GetLODBias(), IsBiasClampEnable(),
                    IsEdgeLODEnable(), GetAnisotropy());
}

void TexMap::Get(GXTlutObj* pTlutObj) const {
    GXInitTlutObj(pTlutObj, GetPalette(), GetPaletteFormat(),
                  GetPaletteEntryNum());
}

void TexMap::Set(TPLPalette* pPalette, u32 id) {
    if (reinterpret_cast<u32>(pPalette->descriptorArray) < 0x80000000) {
        TPLBind(pPalette);
    }

    Set(TPLGet(pPalette, id));
}

// Retail fully inlines the header copy here (accessors expand inline).
void TexMap::Set(const TPLDescriptor* pDesc) {
    const TPLHeader& rTexHeader = *pDesc->textureHeader;

    SetImage(rTexHeader.data);
    SetSize(rTexHeader.width, rTexHeader.height);
    SetTexelFormat(static_cast<GXTexFmt>(rTexHeader.format));
    SetWrapMode(rTexHeader.wrapS, rTexHeader.wrapT);

    SetMipMap(rTexHeader.minLOD != rTexHeader.maxLOD);
    SetFilter(rTexHeader.minFilter, rTexHeader.magFilter);

    SetLOD(static_cast<f32>(rTexHeader.minLOD),
           static_cast<f32>(rTexHeader.maxLOD));

    SetLODBias(rTexHeader.LODBias);
    SetEdgeLODEnable(rTexHeader.edgeLODEnable);

    const TPLClutHeader* const pClutHeader = pDesc->CLUTHeader;

    if (pClutHeader != NULL) {
        SetPalette(pClutHeader->data);
        SetPaletteFormat(pClutHeader->format);
        SetPaletteEntryNum(pClutHeader->numEntries);
    } else {
        SetPalette(NULL);
        SetPaletteFormat(GX_TL_IA8);
        SetPaletteEntryNum(0);
    }
}


void TexMap::SetNoWrap(const TexMap& rOther) {
    GXTexWrapMode wrapS = GetWrapModeS();
    GXTexWrapMode wrapT = GetWrapModeT();

    Set(rOther);
    SetWrapMode(wrapS, wrapT);
}

void TexMap::SetNoWrap(const TPLDescriptor* pDesc) {
    const TPLHeader& rTexHeader = *pDesc->textureHeader;

    SetImage(rTexHeader.data);
    SetSize(rTexHeader.width, rTexHeader.height);
    SetTexelFormat(static_cast<GXTexFmt>(rTexHeader.format));

    SetMipMap(rTexHeader.minLOD != rTexHeader.maxLOD);
    SetFilter(rTexHeader.minFilter, rTexHeader.magFilter);

    SetLOD(static_cast<f32>(rTexHeader.minLOD),
           static_cast<f32>(rTexHeader.maxLOD));

    SetLODBias(rTexHeader.LODBias);
    SetEdgeLODEnable(rTexHeader.edgeLODEnable);

    const TPLClutHeader* const pClutHeader = pDesc->CLUTHeader;

    if (pClutHeader != NULL) {
        SetPalette(pClutHeader->data);
        SetPaletteFormat(pClutHeader->format);
        SetPaletteEntryNum(pClutHeader->numEntries);
    } else {
        SetPalette(NULL);
        SetPaletteFormat(GX_TL_IA8);
        SetPaletteEntryNum(0);
    }
}

} // namespace lyt
} // namespace nw4r

namespace {
// Mirror of the TexMap object layout, used only to emit the retail
// ReplaceImage symbol (the ctor/member declarations live in the read-only
// lyt_texMap.h, so this free function accesses the object through this projection).
struct ReplaceImageTMRep {
    void* mpImage;        // at 0x0
    void* mpPalette;      // at 0x4
    u16 mWidth;           // at 0x8
    u16 mHeight;          // at 0xa
    u32 mMinLOD;          // at 0xc
    u32 mMaxLOD;          // at 0x10
    u16 mLODBias;         // at 0x14
    u16 mPaletteEntryNum; // at 0x16
    struct {
        u32 textureFormat : 4;
        u32 mipmap : 1;
        u32 wrapS : 2;
        u32 wrapT : 2;
        u32 minFilter : 3;
        u32 magFilter : 1;
        u32 biasClampEnable : 1;
        u32 edgeLODEnable : 1;
        u32 anisotropy : 2;
        u32 paletteFormat : 2;
    } mBits; // at 0x18
};
} // namespace

// Retail TexMap::ReplaceImage(TPLPalette*, u32): replaces only the image,
// size, texel-format and palette fields; wrap/filter/LOD state is preserved.
void ReplaceImage__Q34nw4r3lyt6TexMapFP10TPLPaletteUl(
    ReplaceImageTMRep* self, TPLPalette* pPalette, u32 id) {
    if (reinterpret_cast<u32>(pPalette->descriptorArray) < 0x80000000) {
        TPLBind(pPalette);
    }

    const TPLDescriptor* pDesc = TPLGet(pPalette, id);
    // Declared before pTexHeader: retail colors the height temp into the
    // lower scratch register (r5) and the held texture-header pointer into r6.
    u16 height;
    const TPLHeader* pTexHeader = pDesc->textureHeader;

    self->mpImage   = pTexHeader->data;
    height          = pTexHeader->height;
    self->mWidth    = pTexHeader->width;
    self->mHeight   = height;
    self->mBits.textureFormat = pTexHeader->format;

    const TPLClutHeader* const pClutHeader = pDesc->CLUTHeader;
    if (pClutHeader != NULL) {
        self->mpPalette           = pClutHeader->data;
        self->mBits.paletteFormat = pClutHeader->format;
        self->mPaletteEntryNum    = pClutHeader->numEntries;
    } else {
        self->mpPalette        = NULL;
        self->mBits.paletteFormat = GX_TL_IA8;
        self->mPaletteEntryNum = 0;
    }
}
