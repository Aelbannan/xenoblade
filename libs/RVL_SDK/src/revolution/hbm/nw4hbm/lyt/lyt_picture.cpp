#include <nw4hbm/lyt.h>
#include <nw4hbm/ut.h>

namespace nw4hbm {
namespace lyt {

NW4R_UT_RTTI_DEF_DERIVED(Picture, Pane);

Picture::Picture(const res::Picture* pRes, const ResBlockSet& rBlockSet)
    : Pane(pRes) {

    u8 num = ut::Min<u8>(pRes->texCoordNum, 8);

    if (num > 0) {
        mTexCoordAry.Reserve(num);
    }

    mVtxColors[0] = pRes->vtxCols[0];
    mVtxColors[1] = pRes->vtxCols[1];
    mVtxColors[2] = pRes->vtxCols[2];
    mVtxColors[3] = pRes->vtxCols[3];

    if (num > 0 && !mTexCoordAry.IsEmpty()) {
        mTexCoordAry.Copy(
            reinterpret_cast<const u8*>(pRes) + sizeof(res::Picture), num);
    }

    void* pMaterialBuf = Layout::AllocMemory(sizeof(Material));

    if (pMaterialBuf != NULL) {
        const u32* const pMatOffsetTbl = detail::ConvertOffsToPtr<u32>(
            rBlockSet.pMaterialList, sizeof(res::MaterialList));

        const res::Material* const pResMaterial =
            detail::ConvertOffsToPtr<res::Material>(
                rBlockSet.pMaterialList, pMatOffsetTbl[pRes->materialIdx]);

        mpMaterial = new (pMaterialBuf) Material(pResMaterial, rBlockSet);
    }
}

Picture::~Picture() {
    if (mpMaterial != NULL && !mpMaterial->IsUserAllocated()) {
        mpMaterial->~Material();
        Layout::FreeMemory(mpMaterial);
        mpMaterial = NULL;
    }

    mTexCoordAry.Free();
}

ut::Color Picture::GetVtxColor(u32 idx) const {
    return mVtxColors[idx];
}

void Picture::SetVtxColor(u32 idx, ut::Color color) {
    mVtxColors[idx] = color;
}

u8 Picture::GetVtxColorElement(u32 idx) const {
    // Split index into word-offset and byte-offset to match retail pattern.
    const u8* base = reinterpret_cast<const u8*>(mVtxColors);
    return base[(idx & ~3u) + (idx & 3u)];
}

void Picture::SetVtxColorElement(u32 idx, u8 value) {
    detail::SetVtxColorElement(mVtxColors, idx, value);
}

void Picture::Append(TPLPalette* pPalette) {
    GXTexObj texObj;
    detail::InitGXTexObjFromTPL(&texObj, pPalette, 0);
    Append(texObj);
}

void Picture::Append(const GXTexObj& rTexObj) {
    u8 texNum = mpMaterial->GetTextureNum();

    if (texNum >= mpMaterial->GetTextureCap() ||
        texNum >= mpMaterial->GetTexCoordGenCap()) {
        return;
    }

    u8 idx = texNum;

    mpMaterial->SetTextureNum(idx + 1);
    mpMaterial->SetTexture(idx, rTexObj);

    mpMaterial->SetTexCoordGenNum(mpMaterial->GetTextureNum());

    // SetTexCoordGen with default TexCoordGen (GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY)
    mpMaterial->SetTexCoordGen(idx, TexCoordGen());

    mTexCoordAry.SetSize(mpMaterial->GetTextureNum());

    if (mSize == Size(0.0f, 0.0f) && mpMaterial->GetTextureNum() == 1) {
        mSize = detail::GetTextureSize(mpMaterial, 0);
    }
}

void Picture::DrawSelf(const DrawInfo& rInfo) {
    if (mpMaterial == NULL) {
        return;
    }

    LoadMtx(rInfo);

    bool useVtxColor = mpMaterial->SetupGX(
        detail::IsModulateVertexColor(mVtxColors, mGlbAlpha), mGlbAlpha);

    detail::SetVertexFormat(useVtxColor, mTexCoordAry.GetSize());

    detail::DrawQuad(GetVtxPos(), mSize, mTexCoordAry.GetSize(),
                     mTexCoordAry.GetArray(), useVtxColor ? mVtxColors : NULL,
                     mGlbAlpha);
}

} // namespace lyt
} // namespace nw4hbm

extern "C" void* GetRuntimeTypeInfo__Q36nw4hbm3lyt7PictureCFv(void) {
    extern const char typeInfo__Q36nw4hbm3lyt7Picture[];
    return (void*)typeInfo__Q36nw4hbm3lyt7Picture;
}
