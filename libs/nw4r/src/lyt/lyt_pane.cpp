#include <nw4r/lyt.h>
#include <nw4r/math.h>
#include <nw4r/ut.h>

#include <revolution/GX.h>
#include <revolution/MTX.h>

#include <cstring>

/******************************************************************************
 *
 * Utility functions
 *
 ******************************************************************************/
namespace {

using namespace nw4r;
using namespace nw4r::lyt;

void ReverseYAxis(math::MTX34* pMtx) {
    pMtx->m[0][1] = -pMtx->m[0][1];
    pMtx->m[1][1] = -pMtx->m[1][1];
    pMtx->m[2][1] = -pMtx->m[2][1];
}

} // namespace

namespace nw4r {
namespace lyt {

// Definition of the Pane RTTI object (extern + initializer = definition).
extern "C" nw4r::ut::detail::RuntimeTypeInfo lbl_eu_80665470(NULL);

namespace detail {

/******************************************************************************
 *
 * PaneBase
 *
 ******************************************************************************/
PaneBase::PaneBase() {}

PaneBase::~PaneBase() {}

} // namespace detail

/******************************************************************************
 *
 * Pane
 *
 ******************************************************************************/
Pane::Pane() {
    Init();

    mBasePosition = HORIZONTALPOSITION_MAX + 1;

    std::memset(mName, 0, sizeof(mName));
    std::memset(mUserData, 0, sizeof(mUserData));

    mTranslate = math::VEC3(lbl_eu_80669D38, lbl_eu_80669D38, lbl_eu_80669D38);
    mRotate = math::VEC3(lbl_eu_80669D38, lbl_eu_80669D38, lbl_eu_80669D38);
    mScale = math::VEC2(lbl_eu_80669D3C, lbl_eu_80669D3C);
    mSize = Size(lbl_eu_80669D38, lbl_eu_80669D38);
    
    mAlpha = -1;
    mGlbAlpha = -1;
    mFlag = false;
    SetVisible(true);
}

Pane::Pane(const res::Pane* pRes) {
    Init();

    mBasePosition = pRes->basePosition;

    SetName(pRes->name);
    SetUserData(pRes->userData);

    mTranslate = pRes->translate;
    mRotate = pRes->rotate;
    mScale = pRes->scale;
    mSize = pRes->size;

    mAlpha = pRes->alpha;
    mGlbAlpha = mAlpha;
    mFlag = pRes->flag;
}

void Pane::Init() {
    mpParent = NULL;
    mpMaterial = NULL;
    mbUserAllocated = false;
    mpExtUserDataList = NULL;
}

Pane::~Pane() {
    NW4R_UT_LINKLIST_FOREACH_SAFE (it, mChildList, {
        mChildList.Erase(it);

        if (!it->IsUserAllocated()) {
            Layout::DeleteObj(&*it);
        }
    })

    UnbindAnimationSelf(NULL);

    if (mpMaterial != NULL && !mpMaterial->IsUserAllocated()) {
        Layout::DeleteObj(mpMaterial);
    }
}

void Pane::SetName(const char* pName) {
    std::strncpy(mName, pName, NW4R_LYT_RES_NAME_LEN);
    mName[NW4R_LYT_RES_NAME_LEN] = '\0';
}

void Pane::SetUserData(const char* pUserData) {
    std::strncpy(mUserData, pUserData, NW4R_LYT_PANE_USERDATA_LEN);
    mUserData[NW4R_LYT_PANE_USERDATA_LEN] = '\0';
}

void Pane::AppendChild(Pane* pChild) {
    InsertChild(mChildList.GetEndIter(), pChild);
}

void Pane::InsertChild(PaneList::Iterator next, Pane* pChild) {
    mChildList.Insert(next, pChild);
    pChild->mpParent = this;
}

void Pane::PrependChild(Pane* pChild) {
    mChildList.Insert( mChildList.GetBeginIter(), pChild);
    pChild->mpParent = this;
}

void Pane::RemoveChild(Pane* pChild) {
    mChildList.Erase(pChild);
    pChild->mpParent = NULL;
}

ut::Rect Pane::GetPaneRect(const DrawInfo& rInfo) const {
    ut::Rect rect;
    math::VEC2 base = GetVtxPos();

    rect.left = base.x;
    rect.top = base.y;
    rect.right = base.x + mSize.width;
    rect.bottom = base.y + mSize.height;

    if (rInfo.IsYAxisUp()) {
        rect.top = -rect.top;
        rect.bottom = -rect.bottom;
    }

    return rect;
}

ut::Color Pane::GetVtxColor(u32 idx) const {
#pragma unused(idx)

    return ut::Color::WHITE;
}

void Pane::SetVtxColor(u32 idx, ut::Color color) {
#pragma unused(idx)
#pragma unused(color)
}

u8 Pane::GetColorElement(u32 idx) const {
    switch (idx) {
    case ANIMTARGET_PANE_COLOR_ALPHA: {
        return mAlpha;
    }

    default: {
        return GetVtxColorElement(idx);
    }
    }
}

void Pane::SetColorElement(u32 idx, u8 value) {
    switch (idx) {
    case ANIMTARGET_PANE_COLOR_ALPHA: {
        mAlpha = value;
        break;
    }

    default: {
        SetVtxColorElement(idx, value);
        break;
    }
    }
}

u8 Pane::GetVtxColorElement(u32 idx) const {
#pragma unused(idx)

    return 0xFF;
}

void Pane::SetVtxColorElement(u32 idx, u8 value) {
#pragma unused(idx)
#pragma unused(value)
}

Pane* Pane::FindPaneByName(const char* pName, bool recursive) {
    if (detail::EqualsResName(mName, pName)) {
        return this;
    }

    if (recursive) {
        NW4R_UT_LINKLIST_FOREACH (it, mChildList, {
            Pane* pResult = it->FindPaneByName(pName, recursive);

            if (pResult != NULL) {
                return pResult;
            }
        })
    }

    return NULL;
}

Material* Pane::FindMaterialByName(const char* pName, bool recursive) {
    if (mpMaterial != NULL &&
        detail::EqualsMaterialName(mpMaterial->GetName(), pName)) {

        return mpMaterial;
    }

    if (recursive) {
        NW4R_UT_LINKLIST_FOREACH (it, mChildList, {
            Material* pResult = it->FindMaterialByName(pName, recursive);

            if (pResult != NULL) {
                return pResult;
            }
        })
    }

    return NULL;
}

void Pane::CalculateMtx(const DrawInfo& rInfo) {
    if (!IsVisible() && !rInfo.IsInvisiblePaneCalculateMtx()) {
        return;
    }

    math::MTX34 mtx1, mtx2;
    math::MTX34 rotateMtx;

    math::VEC2 scale = mScale;
    if (rInfo.IsLocationAdjust() && IsLocationAdjust()) {
        scale.x *= rInfo.GetLocationAdjustScale().x;
        scale.y *= rInfo.GetLocationAdjustScale().y;
    }

    PSMTXScale(mtx2, scale.x, scale.y, 1.0f);

    PSMTXRotRad(rotateMtx, 'x', NW4R_MATH_DEG_TO_RAD(mRotate.x));
    PSMTXConcat(rotateMtx, mtx2, mtx1);

    PSMTXRotRad(rotateMtx, 'y', NW4R_MATH_DEG_TO_RAD(mRotate.y));
    PSMTXConcat(rotateMtx, mtx1, mtx2);

    PSMTXRotRad(rotateMtx, 'z', NW4R_MATH_DEG_TO_RAD(mRotate.z));
    PSMTXConcat(rotateMtx, mtx2, mtx1);

    PSMTXTransApply(mtx1, mMtx, mTranslate.x, mTranslate.y, mTranslate.z);

    if (mpParent != NULL) {
        math::MTX34Mult(&mGlbMtx, &mpParent->mGlbMtx, &mMtx);
    } else if (rInfo.IsMultipleViewMtxOnDraw()) {
        mGlbMtx = mMtx;
    } else {
        math::MTX34Mult(&mGlbMtx, &rInfo.GetViewMtx(), &mMtx);
    }

    if (rInfo.IsInfluencedAlpha() && mpParent != NULL) {
        mGlbAlpha = static_cast<u8>(mAlpha * rInfo.GetGlobalAlpha());
    } else {
        mGlbAlpha = mAlpha;
    }

    f32 glbAlpha = rInfo.GetGlobalAlpha();
    bool influenced = rInfo.IsInfluencedAlpha();
    bool modifyInfo = IsInfluencedAlpha() && mAlpha != 255;

    if (modifyInfo) {
        DrawInfo& rMtInfo = const_cast<DrawInfo&>(rInfo);
        rMtInfo.SetGlobalAlpha(glbAlpha * mAlpha * (1.0f / 255.0f));
        rMtInfo.SetInfluencedAlpha(true);
    }

    CalculateMtxChild(rInfo);

    if (modifyInfo) {
        DrawInfo& rMtInfo = const_cast<DrawInfo&>(rInfo);
        rMtInfo.SetGlobalAlpha(glbAlpha);
        rMtInfo.SetInfluencedAlpha(influenced);
    }
}

void Pane::CalculateMtxChild(const DrawInfo& rInfo) {
    NW4R_UT_LINKLIST_FOREACH (it, mChildList, { it->CalculateMtx(rInfo); })
}

void Pane::Draw(const DrawInfo& rInfo) {
    if (!IsVisible()) {
        return;
    }

    DrawSelf(rInfo);
    NW4R_UT_LINKLIST_FOREACH (it, mChildList, { it->Draw(rInfo); })
}

void Pane::DrawSelf(const DrawInfo& rInfo) {
#pragma unused(rInfo)
    // Debug draw stripped out
}

void Pane::Animate(u32 option) {
    AnimateSelf(option);

    if (IsVisible() || !(option & ANIMOPTION_SKIP_INVISIBLE)) {
        NW4R_UT_LINKLIST_FOREACH (it, mChildList, { it->Animate(option); })
    }
}

void Pane::AnimateSelf(u32 option) {
    NW4R_UT_LINKLIST_FOREACH (it, mAnimList, {
        if (!it->IsEnable()) {
            continue;
        }

        AnimTransform* pAnimTrans = it->GetAnimTransform();
        pAnimTrans->Animate(it->GetIndex(), this);
    })

    if (IsVisible() || !(option & ANIMOPTION_SKIP_INVISIBLE)) {
        if (mpMaterial != NULL) {
            mpMaterial->Animate();
        }
    }
}

void Pane::BindAnimation(AnimTransform* pAnimTrans, bool recursive, bool disable) {
    pAnimTrans->Bind(this, recursive, disable);
}

void Pane::UnbindAnimation(AnimTransform* pAnimTrans, bool recursive) {
    UnbindAnimationSelf(pAnimTrans);

    if (recursive) {
        NW4R_UT_LINKLIST_FOREACH (it, mChildList, 
            { it->UnbindAnimation(pAnimTrans, recursive); })
    }
}

void Pane::UnbindAllAnimation(bool recursive) {
    UnbindAnimation(NULL, recursive);
}

void Pane::UnbindAnimationSelf(AnimTransform* pAnimTrans) {
    if (mpMaterial != NULL) {
        mpMaterial->UnbindAnimation(pAnimTrans);
    }

    detail::UnbindAnimationLink(&mAnimList, pAnimTrans);
}

void Pane::AddAnimationLink(AnimationLink* pAnimLink) {
    mAnimList.PushBack(pAnimLink);
}

AnimationLink* Pane::FindAnimationLinkSelf(AnimTransform* pAnimTrans) {
    return detail::FindAnimationLink(&mAnimList, pAnimTrans);
}

AnimationLink* Pane::FindAnimationLinkSelf(const AnimResource& rResource) {
    return detail::FindAnimationLink(&mAnimList, rResource);
}

void Pane::SetAnimationEnable(AnimTransform* pAnimTrans, bool enable,
                              bool recursive) {

    AnimationLink* pAnimLink = FindAnimationLinkSelf(pAnimTrans);

    if (pAnimLink != NULL) {
        pAnimLink->SetEnable(enable);
    }

    u8 materialNum = GetMaterialNum();

    for (u8 i = 0; i < materialNum; i++) {
        GetMaterial(i)->SetAnimationEnable(pAnimTrans, enable);
    }
    
    if (recursive) {
        NW4R_UT_LINKLIST_FOREACH (it, mChildList, 
            { it->SetAnimationEnable(pAnimTrans, enable, recursive); })
    }
}

void Pane::SetAnimationEnable(const AnimResource& rResource, bool enable,
                              bool recursive) {
    
    AnimationLink* pAnimLink = FindAnimationLinkSelf(rResource);

    if (pAnimLink != NULL) {
        pAnimLink->SetEnable(enable);
    }

    u8 materialNum = GetMaterialNum();

    for (u8 i = 0; i < materialNum; i++) {
        GetMaterial(i)->SetAnimationEnable(rResource, enable);
    }
    
    if (recursive) {
        NW4R_UT_LINKLIST_FOREACH (it, mChildList, 
            { it->SetAnimationEnable(rResource, enable, recursive); })
    }

}

void Pane::LoadMtx(const DrawInfo& rInfo) {
    math::MTX34 mtx;
    math::MTX34* pMtx = NULL;

    if (rInfo.IsMultipleViewMtxOnDraw()) {
        math::MTX34Mult(&mtx, &rInfo.GetViewMtx(), &mGlbMtx);
        pMtx = &mtx;
    }else{
        pMtx = &mGlbMtx;
    }

    GXLoadPosMtxImm(*pMtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
}

math::VEC2 Pane::GetVtxPos() const {
    math::VEC2 base(0.0f, 0.0f);

    switch (mBasePosition % HORIZONTALPOSITION_MAX) {
    default:
    case HORIZONTALPOSITION_LEFT: {
        base.x = 0.0f;
        break;
    }

    case HORIZONTALPOSITION_CENTER: {
        base.x = -mSize.width / 2;
        break;
    }

    case HORIZONTALPOSITION_RIGHT: {
        base.x = -mSize.width;
        break;
    }
    }

    switch (mBasePosition / HORIZONTALPOSITION_MAX) {
    default:
    case VERTICALPOSITION_TOP: {
        base.y = 0.0f;
        break;
    }

    case HORIZONTALPOSITION_CENTER: {
        base.y = mSize.height / 2;
        break;
    }

    case HORIZONTALPOSITION_RIGHT: {
        base.y = mSize.height;
        break;
    }
    }

    return base;
}

u8 Pane::GetMaterialNum() const {
    return mpMaterial != NULL ? 1 : 0;
}

Material* Pane::GetMaterial() const {
    return mpMaterial;
}

Material* Pane::GetMaterial(u32 index) const {
    if(index == 0) return GetMaterial();
    return NULL;
}

} // namespace lyt
} // namespace nw4r

namespace nw4r {
namespace lyt {

const nw4r::ut::detail::RuntimeTypeInfo* Pane::GetRuntimeTypeInfo() const {
    return &lbl_eu_80665470;
}

} // namespace lyt
} // namespace nw4r

// ABSORB 2026-08-29: lyt_pane now owns .data ranges from lyt_group (0x10),
// lyt_layout (0x40), and shared Picture/TextBox vtables (0x78/0x88) that were
// previously in the blob. Emit them here with retail labels and relocs.
extern "C" void __dt__Q34nw4r3lyt5GroupFv();
extern "C" void __dt__Q34nw4r3lyt6LayoutFv();
extern "C" void Build__Q34nw4r3lyt6LayoutFPCvPQ34nw4r3lyt16ResourceAccessor();
extern "C" void CreateAnimTransform__Q34nw4r3lyt6LayoutFv();
extern "C" void CreateAnimTransform__Q34nw4r3lyt6LayoutFPCvPQ34nw4r3lyt16ResourceAccessor();
extern "C" void CreateAnimTransform__Q34nw4r3lyt6LayoutFRCQ34nw4r3lyt12AnimResourcePQ34nw4r3lyt16ResourceAccessor();
extern "C" void BindAnimation__Q34nw4r3lyt6LayoutFPQ34nw4r3lyt13AnimTransform();
extern "C" void UnbindAnimation__Q34nw4r3lyt6LayoutFPQ34nw4r3lyt13AnimTransform();
extern "C" void UnbindAllAnimation__Q34nw4r3lyt6LayoutFv();
extern "C" void SetAnimationEnable__Q34nw4r3lyt6LayoutFPQ34nw4r3lyt13AnimTransformb();
extern "C" void CalculateMtx__Q34nw4r3lyt6LayoutFRCQ34nw4r3lyt8DrawInfo();
extern "C" void Draw__Q34nw4r3lyt6LayoutFRCQ34nw4r3lyt8DrawInfo();
extern "C" void Animate__Q34nw4r3lyt6LayoutFUl();
extern "C" void GetLayoutRect__Q34nw4r3lyt6LayoutCFv();
extern "C" void __dt__Q34nw4r3lyt7PictureFv();
extern "C" void* GetRuntimeTypeInfo__Q34nw4r3lyt7PictureCFv();
extern "C" void CalculateMtx__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo();
extern "C" void Draw__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo();
extern "C" void DrawSelf__Q34nw4r3lyt7PictureFRCQ34nw4r3lyt8DrawInfo();
extern "C" void Animate__Q34nw4r3lyt4PaneFUl();
extern "C" void AnimateSelf__Q34nw4r3lyt4PaneFUl();
extern "C" void GetVtxColor__Q34nw4r3lyt7PictureCFUl();
extern "C" void SetVtxColor__Q34nw4r3lyt7PictureFUlQ34nw4r2ut5Color();
extern "C" void GetColorElement__Q34nw4r3lyt4PaneCFUl();
extern "C" void SetColorElement__Q34nw4r3lyt4PaneFUlUc();
extern "C" void GetVtxColorElement__Q34nw4r3lyt7PictureCFUl();
extern "C" void SetVtxColorElement__Q34nw4r3lyt7PictureFUlUc();
extern "C" void FindPaneByName__Q34nw4r3lyt4PaneFPCcb();
extern "C" void FindMaterialByName__Q34nw4r3lyt4PaneFPCcb();
extern "C" void BindAnimation__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformbb();
extern "C" void UnbindAnimation__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformb();
extern "C" void UnbindAllAnimation__Q34nw4r3lyt4PaneFb();
extern "C" void UnbindAnimationSelf__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransform();
extern "C" void FindAnimationLinkSelf__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransform();
extern "C" void FindAnimationLinkSelf__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt12AnimResource();
extern "C" void SetAnimationEnable__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformbb();
extern "C" void SetAnimationEnable__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt12AnimResourcebb();
extern "C" void GetMaterialNum__Q34nw4r3lyt4PaneCFv();
extern "C" void GetMaterial__Q34nw4r3lyt4PaneCFv();
extern "C" void GetMaterial__Q34nw4r3lyt4PaneCFUl();
extern "C" void LoadMtx__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo();
extern "C" void Append__Q34nw4r3lyt7PictureFRCQ34nw4r3lyt6TexMap();
extern "C" void __dt__Q34nw4r3lyt7TextBoxFv();
extern "C" void* GetRuntimeTypeInfo__Q34nw4r3lyt7TextBoxCFv();
extern void LoadMtx__Q34nw4r3lyt7TextBoxFRCQ34nw4r3lyt8DrawInfo__FPQ34nw4r3lyt7TextBoxRCQ34nw4r3lyt8DrawInfo();
extern "C" void DrawSelf__Q34nw4r3lyt7TextBoxFRCQ34nw4r3lyt8DrawInfo();
extern "C" void GetTextColor__Q34nw4r3lyt7TextBoxCFUl();
extern "C" void SetVtxColor__Q34nw4r3lyt7TextBoxFUlQ34nw4r2ut5Color();
extern "C" void GetVtxColorElement__Q34nw4r3lyt7TextBoxCFUl();
extern "C" void SetVtxColorElement__Q34nw4r3lyt7TextBoxFUlUc();
extern "C" void AllocStringBuffer__Q34nw4r3lyt7TextBoxFUs();
extern "C" void FreeStringBuffer__Q34nw4r3lyt7TextBoxFv();
extern "C" void SetString__Q34nw4r3lyt7TextBoxFPCwUs();
extern "C" void SetString__Q34nw4r3lyt7TextBoxFPCwUsUs();
extern "C" {
__declspec(section ".data") __attribute__((aligned(8), used)) const void* lbl_eu_80569968[4] = { nullptr, nullptr, (const void*)&__dt__Q34nw4r3lyt5GroupFv, nullptr };
__declspec(section ".data") __attribute__((aligned(8), used)) const void* lbl_eu_80569978[16] = { nullptr, nullptr, (const void*)&__dt__Q34nw4r3lyt6LayoutFv, (const void*)&Build__Q34nw4r3lyt6LayoutFPCvPQ34nw4r3lyt16ResourceAccessor, (const void*)&CreateAnimTransform__Q34nw4r3lyt6LayoutFv, (const void*)&CreateAnimTransform__Q34nw4r3lyt6LayoutFPCvPQ34nw4r3lyt16ResourceAccessor, (const void*)&CreateAnimTransform__Q34nw4r3lyt6LayoutFRCQ34nw4r3lyt12AnimResourcePQ34nw4r3lyt16ResourceAccessor, (const void*)&BindAnimation__Q34nw4r3lyt6LayoutFPQ34nw4r3lyt13AnimTransform, (const void*)&UnbindAnimation__Q34nw4r3lyt6LayoutFPQ34nw4r3lyt13AnimTransform, (const void*)&UnbindAllAnimation__Q34nw4r3lyt6LayoutFv, (const void*)&SetAnimationEnable__Q34nw4r3lyt6LayoutFPQ34nw4r3lyt13AnimTransformb, (const void*)&CalculateMtx__Q34nw4r3lyt6LayoutFRCQ34nw4r3lyt8DrawInfo, (const void*)&Draw__Q34nw4r3lyt6LayoutFRCQ34nw4r3lyt8DrawInfo, (const void*)&Animate__Q34nw4r3lyt6LayoutFUl, (const void*)&GetLayoutRect__Q34nw4r3lyt6LayoutCFv, nullptr };
__declspec(section ".data") __attribute__((aligned(8), used)) const void* lbl_eu_805699B8[30] = { nullptr, nullptr, (const void*)&__dt__Q34nw4r3lyt7PictureFv, (const void*)&GetRuntimeTypeInfo__Q34nw4r3lyt7PictureCFv, (const void*)&CalculateMtx__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo, (const void*)&Draw__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo, (const void*)&DrawSelf__Q34nw4r3lyt7PictureFRCQ34nw4r3lyt8DrawInfo, (const void*)&Animate__Q34nw4r3lyt4PaneFUl, (const void*)&AnimateSelf__Q34nw4r3lyt4PaneFUl, (const void*)&GetVtxColor__Q34nw4r3lyt7PictureCFUl, (const void*)&SetVtxColor__Q34nw4r3lyt7PictureFUlQ34nw4r2ut5Color, (const void*)&GetColorElement__Q34nw4r3lyt4PaneCFUl, (const void*)&SetColorElement__Q34nw4r3lyt4PaneFUlUc, (const void*)&GetVtxColorElement__Q34nw4r3lyt7PictureCFUl, (const void*)&SetVtxColorElement__Q34nw4r3lyt7PictureFUlUc, (const void*)&FindPaneByName__Q34nw4r3lyt4PaneFPCcb, (const void*)&FindMaterialByName__Q34nw4r3lyt4PaneFPCcb, (const void*)&BindAnimation__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformbb, (const void*)&UnbindAnimation__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformb, (const void*)&UnbindAllAnimation__Q34nw4r3lyt4PaneFb, (const void*)&UnbindAnimationSelf__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransform, (const void*)&FindAnimationLinkSelf__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransform, (const void*)&FindAnimationLinkSelf__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt12AnimResource, (const void*)&SetAnimationEnable__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformbb, (const void*)&SetAnimationEnable__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt12AnimResourcebb, (const void*)&GetMaterialNum__Q34nw4r3lyt4PaneCFv, (const void*)&GetMaterial__Q34nw4r3lyt4PaneCFv, (const void*)&GetMaterial__Q34nw4r3lyt4PaneCFUl, (const void*)&LoadMtx__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo, (const void*)&Append__Q34nw4r3lyt7PictureFRCQ34nw4r3lyt6TexMap };
__declspec(section ".data") __attribute__((aligned(8), used)) const void* lbl_eu_80569A30[34] = { nullptr, nullptr, (const void*)&__dt__Q34nw4r3lyt7TextBoxFv, (const void*)&GetRuntimeTypeInfo__Q34nw4r3lyt7TextBoxCFv, (const void*)&CalculateMtx__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo, (const void*)&Draw__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo, (const void*)&DrawSelf__Q34nw4r3lyt7TextBoxFRCQ34nw4r3lyt8DrawInfo, (const void*)&Animate__Q34nw4r3lyt4PaneFUl, (const void*)&AnimateSelf__Q34nw4r3lyt4PaneFUl, (const void*)&GetTextColor__Q34nw4r3lyt7TextBoxCFUl, (const void*)&SetVtxColor__Q34nw4r3lyt7TextBoxFUlQ34nw4r2ut5Color, (const void*)&GetColorElement__Q34nw4r3lyt4PaneCFUl, (const void*)&SetColorElement__Q34nw4r3lyt4PaneFUlUc, (const void*)&GetVtxColorElement__Q34nw4r3lyt7TextBoxCFUl, (const void*)&SetVtxColorElement__Q34nw4r3lyt7TextBoxFUlUc, (const void*)&FindPaneByName__Q34nw4r3lyt4PaneFPCcb, (const void*)&FindMaterialByName__Q34nw4r3lyt4PaneFPCcb, (const void*)&BindAnimation__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformbb, (const void*)&UnbindAnimation__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformb, (const void*)&UnbindAllAnimation__Q34nw4r3lyt4PaneFb, (const void*)&UnbindAnimationSelf__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransform, (const void*)&FindAnimationLinkSelf__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransform, (const void*)&FindAnimationLinkSelf__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt12AnimResource, (const void*)&SetAnimationEnable__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformbb, (const void*)&SetAnimationEnable__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt12AnimResourcebb, (const void*)&GetMaterialNum__Q34nw4r3lyt4PaneCFv, (const void*)&GetMaterial__Q34nw4r3lyt4PaneCFv, (const void*)&GetMaterial__Q34nw4r3lyt4PaneCFUl, (const void*)&LoadMtx__Q34nw4r3lyt7TextBoxFRCQ34nw4r3lyt8DrawInfo__FPQ34nw4r3lyt7TextBoxRCQ34nw4r3lyt8DrawInfo, (const void*)&AllocStringBuffer__Q34nw4r3lyt7TextBoxFUs, (const void*)&FreeStringBuffer__Q34nw4r3lyt7TextBoxFv, (const void*)&SetString__Q34nw4r3lyt7TextBoxFPCwUs, (const void*)&SetString__Q34nw4r3lyt7TextBoxFPCwUsUs, nullptr };
}
extern "C" __attribute__((used)) char lyt_pane_sbss_pad[4];
