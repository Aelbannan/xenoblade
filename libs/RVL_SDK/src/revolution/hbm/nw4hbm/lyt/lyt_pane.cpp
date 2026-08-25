#include <nw4hbm/lyt.h>
#include <nw4hbm/math.h>
#include <nw4hbm/ut.h>

#include <revolution/GX.h>
#include <revolution/MTX.h>

#include <cstring>

/******************************************************************************
 *
 * Utility functions
 *
 ******************************************************************************/
namespace {

using namespace nw4hbm;
using namespace nw4hbm::lyt;

/* Literal pool mirroring retail lbl_80518A98 exactly (order matters).
   Functions load through this base so displacements match retail; defining
   it here also makes section layout deterministic (MWCC's anonymous literal
   pool ordering varies run to run). */
extern const f32 sPanePool[6] = {
    0.0f,                  /* +0x00 */
    1.0f,                  /* +0x04 */
    0.5f,                  /* +0x08 */
    1.0f / 255.0f,         /* +0x0C */
    NW4R_MATH_PI / 180.0f, /* +0x10 */
    1.0f / 255.0f,         /* +0x14 dup */
};

void ReverseYAxis(math::MTX34* pMtx) {
    pMtx->m[0][1] = -pMtx->m[0][1];
    pMtx->m[1][1] = -pMtx->m[1][1];
    pMtx->m[2][1] = -pMtx->m[2][1];
}

} // namespace

namespace nw4hbm {
namespace lyt {

NW4R_UT_RTTI_DEF_BASE(Pane);

namespace detail {

/******************************************************************************
 *
 * PaneBase
 *
 ******************************************************************************/
// PaneBase ctor/dtor are inline in lyt_pane.h (retail emits no standalone
// bodies: the base ctor/dtor are inlined into Pane's ctor/dtor, and the
// PaneBase vtable is never materialized in this TU).

} // namespace detail

/******************************************************************************
 *
 * Pane
 *
 ******************************************************************************/
Pane::Pane(const res::Pane* pRes) {
    mpParent = NULL;
    mpMaterial = NULL;
    mUserAllocated = false;

    mBasePosition = pRes->basePosition;

    std::strncpy(mName, pRes->name, NW4R_LYT_RES_NAME_LEN);
    std::strncpy(mUserData, pRes->userData, NW4R_LYT_PANE_USERDATA_LEN);

    mTranslate = pRes->translate;
    mRotate = pRes->rotate;
    mScale = pRes->scale;
    mSize = pRes->size;

    mAlpha = pRes->alpha;
    mGlbAlpha = mAlpha;
    mFlag = pRes->flag;
}

Pane::~Pane() {
    NW4R_UT_LINKLIST_FOREACH_SAFE (it, mChildList, {
        mChildList.Erase(it);

        if (!it->IsUserAllocated()) {
            it->~Pane();
            Layout::FreeMemory(&*it);
        }
    })

    UnbindAnimationSelf(NULL);

    if (mpMaterial != NULL && !mpMaterial->IsUserAllocated()) {
        mpMaterial->~Material();
        Layout::FreeMemory(mpMaterial);
    }
}

void Pane::AppendChild(Pane* pChild) {
    mChildList.Insert(mChildList.GetEndIter(), pChild);
    pChild->mpParent = this;
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
    return ut::Color(0xFFFFFFFFu);
}

void Pane::SetVtxColor(u32 idx, ut::Color color) {
    // empty
}

u8 Pane::GetColorElement(u32 idx) const {
    if (idx == 16) {
        return mAlpha;
    }
    return GetVtxColorElement(idx);
}

void nw4hbm::lyt::Pane::SetColorElement(unsigned long idx, unsigned char value) {
    if (idx == 16) {
        mAlpha = value;
    } else {
        SetVtxColorElement(idx, value);
    }
}

u8 Pane::GetVtxColorElement(u32 idx) const {
    return 0xFF;
}

void Pane::SetVtxColorElement(u32 idx, u8 value) {
#pragma unused(idx)
#pragma unused(value)
}

Pane* Pane::FindPaneByName(const char* pName, bool recursive) {
    if (detail::EqualsPaneName(mName, pName)) {
        return this;
    }

    if (recursive) {
        PaneList::Iterator it = mChildList.GetBeginIter();
        PaneList::Iterator itEnd = mChildList.GetEndIter();

        while (it != itEnd) {
            Pane* pResult = it->FindPaneByName(pName, true);

            if (pResult != NULL) {
                return pResult;
            }

            ++it;
        }
    }

    return NULL;
}

Material* Pane::FindMaterialByName(const char* pName, bool recursive) {
    if (mpMaterial != NULL &&
        detail::EqualsMaterialName(mpMaterial->GetName(), pName)) {

        return mpMaterial;
    }

    if (recursive) {
        PaneList::Iterator it = mChildList.GetBeginIter();
        PaneList::Iterator itEnd = mChildList.GetEndIter();

        while (it != itEnd) {
            Material* pResult = it->FindMaterialByName(pName, true);

            if (pResult != NULL) {
                return pResult;
            }

            ++it;
        }
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

    PSMTXScale(mtx2, scale.x, scale.y, sPanePool[1]);

    PSMTXRotRad(rotateMtx, 'x', (mRotate.x * sPanePool[4]));
    PSMTXConcat(rotateMtx, mtx2, mtx1);

    PSMTXRotRad(rotateMtx, 'y', (mRotate.y * sPanePool[4]));
    PSMTXConcat(rotateMtx, mtx1, mtx2);

    PSMTXRotRad(rotateMtx, 'z', (mRotate.z * sPanePool[4]));
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
        rMtInfo.SetGlobalAlpha(glbAlpha * mAlpha * sPanePool[3]);
        rMtInfo.SetInfluencedAlpha(true);
    }

    NW4R_UT_LINKLIST_FOREACH (it, mChildList, { it->CalculateMtx(rInfo); })

    if (modifyInfo) {
        DrawInfo& rMtInfo = const_cast<DrawInfo&>(rInfo);
        rMtInfo.SetGlobalAlpha(glbAlpha);
        rMtInfo.SetInfluencedAlpha(influenced);
    }
}

void Pane::Draw(const DrawInfo& rInfo) {
    if (!IsVisible()) {
        return;
    }

    DrawSelf(rInfo);
    NW4R_UT_LINKLIST_FOREACH (it, mChildList, { it->Draw(rInfo); })
}

void Pane::DrawSelf(const DrawInfo& rInfo) {
    if (mpParent == NULL) {
        return;
    }
    if (!rInfo.IsDebugDrawMode()) {
        return;
    }

    LoadMtx(rInfo);

    // Debug draw: outline the pane bounds with a green line
    ut::Color color(0x00FF00FFu);

    f32 x = sPanePool[0];
    f32 y = sPanePool[0];

    switch (mBasePosition % HORIZONTALPOSITION_MAX) {
    case HORIZONTALPOSITION_CENTER:
        x = -mSize.width * sPanePool[2];
        break;
    case HORIZONTALPOSITION_RIGHT:
        x = -mSize.width;
        break;
    default:
        x = sPanePool[0];
        break;
    }

    switch (mBasePosition / HORIZONTALPOSITION_MAX) {
    case VERTICALPOSITION_CENTER:
        y = -mSize.height * sPanePool[2];
        break;
    case VERTICALPOSITION_BOTTOM:
        y = -mSize.height;
        break;
    default:
        y = sPanePool[0];
        break;
    }

    detail::DrawLine(math::VEC2(x, y), mSize, color);
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

void Pane::BindAnimation(AnimTransform* pAnimTrans, bool recursive) {
    pAnimTrans->Bind(this, recursive);
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

    AnimationLinkList::Iterator it = mAnimList.GetBeginIter();

    while (it != mAnimList.GetEndIter()) {
        AnimationLink* pLink = &*it;

        ++it;

        if (pAnimTrans == NULL || pLink->GetAnimTransform() == pAnimTrans) {
            mAnimList.Erase(mAnimList.GetIteratorFromPointer(pLink));
            pLink->Reset();
        }
    }
}

void Pane::AddAnimationLink(AnimationLink* pAnimLink) {
    mAnimList.PushBack(pAnimLink);
}

AnimationLink* Pane::FindAnimationLink(AnimTransform* pAnimTrans) {
    AnimationLink* pLink = detail::FindAnimationLink(&mAnimList, pAnimTrans);

    if (pLink != NULL) {
        return pLink;
    }

    if (mpMaterial != NULL) {
        pLink = mpMaterial->FindAnimationLink(pAnimTrans);

        if (pLink != NULL) {
            return pLink;
        }
    }

    return NULL;
}

void Pane::SetAnimationEnable(AnimTransform* pAnimTrans, bool enable,
                              bool recursive) {
    AnimationLink* pAnimLink = detail::FindAnimationLink(&mAnimList, pAnimTrans);

    if (pAnimLink != NULL) {
        pAnimLink->SetEnable(enable);
    }

    if (mpMaterial != NULL) {
        mpMaterial->SetAnimationEnable(pAnimTrans, enable);
    }

    if (recursive) {
        NW4R_UT_LINKLIST_FOREACH (it, mChildList,
            { it->SetAnimationEnable(pAnimTrans, enable, recursive); })
    }
}

void Pane::LoadMtx(const DrawInfo& rInfo) {
    Mtx mtx;
    const f32 (*pMtx)[4];

    if (rInfo.IsMultipleViewMtxOnDraw()) {
        PSMTXConcat(rInfo.GetViewMtx(), mGlbMtx, mtx);

        if (rInfo.IsYAxisUp()) {
            mtx[0][1] = -mtx[0][1];
            mtx[1][1] = -mtx[1][1];
            mtx[2][1] = -mtx[2][1];
        }

        pMtx = mtx;
    } else {
        if (rInfo.IsYAxisUp()) {
            PSMTXCopy(mGlbMtx, mtx);

            mtx[0][1] = -mtx[0][1];
            mtx[1][1] = -mtx[1][1];
            mtx[2][1] = -mtx[2][1];

            pMtx = mtx;
        } else {
            pMtx = mGlbMtx;
        }
    }

    GXLoadPosMtxImm(pMtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
}

math::VEC2 Pane::GetVtxPos() const {
    math::VEC2 base(sPanePool[0], sPanePool[0]);

    switch (mBasePosition % HORIZONTALPOSITION_MAX) {
    default:
    case HORIZONTALPOSITION_LEFT: {
        base.x = sPanePool[0];
        break;
    }

    case HORIZONTALPOSITION_CENTER: {
        base.x = -mSize.width * sPanePool[2];
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
        base.y = sPanePool[0];
        break;
    }

    case VERTICALPOSITION_CENTER: {
        base.y = -mSize.height * sPanePool[2];
        break;
    }

    case VERTICALPOSITION_BOTTOM: {
        base.y = -mSize.height;
        break;
    }
    }

    return base;
}

Material* Pane::GetMaterial() const {
    return mpMaterial;
}

} // namespace lyt
} // namespace nw4hbm

/* Retail keeps Pane's 0x68 vtable in this TU's .data, but -ipa file GCs
   MWCC's weak emission (no .data in the natural object). Materialize the
   slot table here with live relocations so MWCC emits .data/.rela.data;
   postprocess renames it to __vt__Q36nw4hbm3lyt4Pane. Slots +0x00/+0x0C
   reference symbols owned elsewhere at link (the Pane typeinfo ships from
   another homebutton TU; GetRuntimeTypeInfo's weak def lives in
   HBMGUIManager.o - see the drop note above). Slot contents stay zero in
   the object either way (linker-filled). */
extern const void* __RTTI__Q36nw4hbm3lyt4Pane;
extern "C" const void* GetRuntimeTypeInfo__Q36nw4hbm3lyt4PaneCFv();
extern "C" void __dt__Q36nw4hbm3lyt4PaneFv();
extern "C" void CalculateMtx__Q36nw4hbm3lyt4PaneFRCQ36nw4hbm3lyt8DrawInfo();
extern "C" void Draw__Q36nw4hbm3lyt4PaneFRCQ36nw4hbm3lyt8DrawInfo();
extern "C" void DrawSelf__Q36nw4hbm3lyt4PaneFRCQ36nw4hbm3lyt8DrawInfo();
extern "C" void Animate__Q36nw4hbm3lyt4PaneFUl();
extern "C" void AnimateSelf__Q36nw4hbm3lyt4PaneFUl();
extern "C" void GetVtxColor__Q36nw4hbm3lyt4PaneCFUl();
extern "C" void SetVtxColor__Q36nw4hbm3lyt4PaneFUlQ36nw4hbm2ut5Color();
extern "C" void GetColorElement__Q36nw4hbm3lyt4PaneCFUl();
extern "C" void SetColorElement__Q36nw4hbm3lyt4PaneFUlUc();
extern "C" void GetVtxColorElement__Q36nw4hbm3lyt4PaneCFUl();
extern "C" void SetVtxColorElement__Q36nw4hbm3lyt4PaneFUlUc();
extern "C" void FindPaneByName__Q36nw4hbm3lyt4PaneFPCcb();
extern "C" void FindMaterialByName__Q36nw4hbm3lyt4PaneFPCcb();
extern "C" void BindAnimation__Q36nw4hbm3lyt4PaneFPQ36nw4hbm3lyt13AnimTransformb();
extern "C" void UnbindAnimation__Q36nw4hbm3lyt4PaneFPQ36nw4hbm3lyt13AnimTransformb();
extern "C" void UnbindAllAnimation__Q36nw4hbm3lyt4PaneFb();
extern "C" void UnbindAnimationSelf__Q36nw4hbm3lyt4PaneFPQ36nw4hbm3lyt13AnimTransform();
extern "C" void FindAnimationLink__Q36nw4hbm3lyt4PaneFPQ36nw4hbm3lyt13AnimTransform();
extern "C" void SetAnimationEnable__Q36nw4hbm3lyt4PaneFPQ36nw4hbm3lyt13AnimTransformbb();
extern "C" void GetMaterial__Q36nw4hbm3lyt4PaneCFv();
extern "C" void LoadMtx__Q36nw4hbm3lyt4PaneFRCQ36nw4hbm3lyt8DrawInfo();

/* Non-const: MWCC files const pointer tables into .rodata; retail keeps the
   vtable in .data. Slot contents stay zero in the object either way. */
extern const void* sPaneVtableSlots[26] = {
    (const void*)&__RTTI__Q36nw4hbm3lyt4Pane,
    0,
    (const void*)&__dt__Q36nw4hbm3lyt4PaneFv,
    (const void*)&GetRuntimeTypeInfo__Q36nw4hbm3lyt4PaneCFv,
    (const void*)&CalculateMtx__Q36nw4hbm3lyt4PaneFRCQ36nw4hbm3lyt8DrawInfo,
    (const void*)&Draw__Q36nw4hbm3lyt4PaneFRCQ36nw4hbm3lyt8DrawInfo,
    (const void*)&DrawSelf__Q36nw4hbm3lyt4PaneFRCQ36nw4hbm3lyt8DrawInfo,
    (const void*)&Animate__Q36nw4hbm3lyt4PaneFUl,
    (const void*)&AnimateSelf__Q36nw4hbm3lyt4PaneFUl,
    (const void*)&GetVtxColor__Q36nw4hbm3lyt4PaneCFUl,
    (const void*)&SetVtxColor__Q36nw4hbm3lyt4PaneFUlQ36nw4hbm2ut5Color,
    (const void*)&GetColorElement__Q36nw4hbm3lyt4PaneCFUl,
    (const void*)&SetColorElement__Q36nw4hbm3lyt4PaneFUlUc,
    (const void*)&GetVtxColorElement__Q36nw4hbm3lyt4PaneCFUl,
    (const void*)&SetVtxColorElement__Q36nw4hbm3lyt4PaneFUlUc,
    (const void*)&FindPaneByName__Q36nw4hbm3lyt4PaneFPCcb,
    (const void*)&FindMaterialByName__Q36nw4hbm3lyt4PaneFPCcb,
    (const void*)&BindAnimation__Q36nw4hbm3lyt4PaneFPQ36nw4hbm3lyt13AnimTransformb,
    (const void*)&UnbindAnimation__Q36nw4hbm3lyt4PaneFPQ36nw4hbm3lyt13AnimTransformb,
    (const void*)&UnbindAllAnimation__Q36nw4hbm3lyt4PaneFb,
    (const void*)&UnbindAnimationSelf__Q36nw4hbm3lyt4PaneFPQ36nw4hbm3lyt13AnimTransform,
    (const void*)&FindAnimationLink__Q36nw4hbm3lyt4PaneFPQ36nw4hbm3lyt13AnimTransform,
    (const void*)&SetAnimationEnable__Q36nw4hbm3lyt4PaneFPQ36nw4hbm3lyt13AnimTransformbb,
    (const void*)&GetMaterial__Q36nw4hbm3lyt4PaneCFv,
    (const void*)&LoadMtx__Q36nw4hbm3lyt4PaneFRCQ36nw4hbm3lyt8DrawInfo,
    0,
};
