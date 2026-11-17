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

void TexMap::ReplaceImage(TPLPalette* pPalette, u32 id) {
    // Not yet converted from offset to pointer (unbound)
    if (reinterpret_cast<u32>(pPalette->descriptorArray) < 0x80000000) {
        TPLBind(pPalette);
    }

    ReplaceImage(TPLGet(pPalette, id));
}

// Not present in the retail link (inlined into the palette overload).
inline void TexMap::ReplaceImage(const TPLDescriptor* pDesc) {
    const TPLHeader* pTexHeader = pDesc->textureHeader;

    SetImage(pTexHeader->data);
    SetSize(pTexHeader->width, pTexHeader->height);
    SetTexelFormat(static_cast<GXTexFmt>(pTexHeader->format));

    const TPLClutHeader* pClutHeader = pDesc->CLUTHeader;

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
