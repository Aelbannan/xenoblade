#include <nw4r/lyt.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace lyt {
namespace {

bool RIsSame(f32 a, f32 b, f32 tolerance) {
    f32 c = a - b;
    return (-tolerance < c && c < tolerance);
}

u16 GetStepCurveValue(f32 frame, const res::StepKey *keyArray, u32 keySize) {
    if (keySize == 1 || frame <= keyArray[0].frame) {
        return keyArray[0].value;
    }
    if (frame >= keyArray[keySize - 1].frame) {
        return keyArray[keySize - 1].value;
    }

    int ikeyL = 0;
    int ikeyR = keySize - 1;
    while (ikeyL != ikeyR - 1 && ikeyL != ikeyR) {
        int ikeyCenter = (ikeyL + ikeyR) / 2;
        const res::StepKey &centerKey = keyArray[ikeyCenter];
        if (frame < centerKey.frame) {
            ikeyR = ikeyCenter;
        } else {
            ikeyL = ikeyCenter;
        }
    }

    if (RIsSame(frame, keyArray[ikeyR].frame, 0.001f)) {
        return keyArray[ikeyR].value;
    } else {
        return keyArray[ikeyL].value;
    }
}

f32 GetHermiteCurveValue(f32 frame, const res::HermiteKey *keyArray, u32 keySize) {
    if (keySize == 1 || frame <= keyArray[0].frame) {
        return keyArray[0].value;
    }
    if (frame >= keyArray[keySize - 1].frame) {
        return keyArray[keySize - 1].value;
    }

    int ikeyL = 0;
    int ikeyR = keySize - 1;
    while (ikeyL != ikeyR - 1 && ikeyL != ikeyR) {
        int ikeyCenter = (ikeyL + ikeyR) / 2;
        if (frame <= keyArray[ikeyCenter].frame) {
            ikeyR = ikeyCenter;
        } else {
            ikeyL = ikeyCenter;
        }
    }
    const res::HermiteKey &key0 = keyArray[ikeyL];
    const res::HermiteKey &key1 = keyArray[ikeyR];

    if (RIsSame(frame, key1.frame, 0.001f)) {
        if (ikeyR < keySize - 1 && key1.frame == keyArray[ikeyR + 1].frame) {
            return keyArray[ikeyR + 1].value;
        } else {
            return key1.value;
        }
    }

    f32 t1 = frame - key0.frame;
    f32 t2 = 1.0f / (key1.frame - key0.frame);

    f32 v0 = key0.value;
    f32 v1 = key1.value;
    f32 s0 = key0.slope;
    f32 s1 = key1.slope;

    f32 t1t1t2 = t1 * t1 * t2;
    f32 t1t1t2t2 = t1t1t2 * t2;
    f32 t1t1t1t2t2 = t1 * t1t1t2t2;
    f32 t1t1t1t2t2t2 = t1t1t1t2t2 * t2;

    return v0 * (2.0f * t1t1t1t2t2t2 - 3.0f * t1t1t2t2 + 1.0f) + v1 * (-2.0f * t1t1t1t2t2t2 + 3.0f * t1t1t2t2) +
           s0 * (t1 + (t1t1t1t2t2 - 2.0f * t1t1t2)) + s1 * (t1t1t1t2t2 - t1t1t2);
}

void AnimatePaneSRT(Pane *pPane, const res::AnimationInfo *pAnimInfo, const u32 *animTargetOffsets, f32 frame) {
    for (int i = 0; i < pAnimInfo->num; i++) {
        const res::AnimationTarget *pAnimTarget =
            detail::ConvertOffsToPtr<res::AnimationTarget>(pAnimInfo, animTargetOffsets[i]);
        const res::HermiteKey *keys = detail::ConvertOffsToPtr<res::HermiteKey>(pAnimTarget, pAnimTarget->keysOffset);
        pPane->SetSRTElement(pAnimTarget->target, GetHermiteCurveValue(frame, keys, pAnimTarget->keyNum));
    }
}

void AnimateVisibility(Pane *pPane, const res::AnimationInfo *pAnimInfo, const u32 *animTargetOffsets, f32 frame) {
    for (int i = 0; i < pAnimInfo->num; i++) {
        const res::AnimationTarget *pAnimTarget =
            detail::ConvertOffsToPtr<res::AnimationTarget>(pAnimInfo, animTargetOffsets[i]);
        const res::StepKey *keys = detail::ConvertOffsToPtr<res::StepKey>(pAnimTarget, pAnimTarget->keysOffset);
        pPane->SetVisible(GetStepCurveValue(frame, keys, pAnimTarget->keyNum) != 0);
    }
}

void AnimateVertexColor(Pane *pPane, const res::AnimationInfo *pAnimInfo, const u32 *animTargetOffsets, f32 frame) {
    for (int i = 0; i < pAnimInfo->num; i++) {
        const res::AnimationTarget *pAnimTarget =
            detail::ConvertOffsToPtr<res::AnimationTarget>(pAnimInfo, animTargetOffsets[i]);
        const res::HermiteKey *keys = detail::ConvertOffsToPtr<res::HermiteKey>(pAnimTarget, pAnimTarget->keysOffset);
        f32 value = GetHermiteCurveValue(frame, keys, pAnimTarget->keyNum);
        value += 0.5f;
        u8 u8Val;
        OSf32tou8(&value, &u8Val);
        pPane->SetColorElement(pAnimTarget->target, u8Val);
    }
}

void AnimateMaterialColor(
    Material *pMaterial, const res::AnimationInfo *pAnimInfo, const u32 *animTargetOffsets, f32 frame
) {
    for (int i = 0; i < pAnimInfo->num; i++) {
        const res::AnimationTarget *pAnimTarget =
            detail::ConvertOffsToPtr<res::AnimationTarget>(pAnimInfo, animTargetOffsets[i]);
        const res::HermiteKey *keys = detail::ConvertOffsToPtr<res::HermiteKey>(pAnimTarget, pAnimTarget->keysOffset);
        f32 value = GetHermiteCurveValue(frame, keys, pAnimTarget->keyNum);
        value += 0.5f;
        s16 s16Val;
        OSf32tos16(&value, &s16Val);
        s16Val = ut::Min<s16>(ut::Max<s16>(s16Val, -0x400), 1023);
        pMaterial->SetColorElement(pAnimTarget->target, s16Val);
    }
}

void AnimateTextureSRT(
    Material *pMaterial, const res::AnimationInfo *pAnimInfo, const u32 *animTargetOffsets, f32 frame
) {
    for (int i = 0; i < pAnimInfo->num; i++) {
        const res::AnimationTarget *pAnimTarget =
            detail::ConvertOffsToPtr<res::AnimationTarget>(pAnimInfo, animTargetOffsets[i]);
        if (pAnimTarget->id < pMaterial->GetTexSRTCap()) {
            const res::HermiteKey *keys =
                detail::ConvertOffsToPtr<res::HermiteKey>(pAnimTarget, pAnimTarget->keysOffset);

            pMaterial->SetTexSRTElement(
                pAnimTarget->id, pAnimTarget->target, GetHermiteCurveValue(frame, keys, pAnimTarget->keyNum)
            );
        }
    }
}

void AnimateTexturePattern(
    Material *pMaterial, const res::AnimationInfo *pAnimInfo, const u32 *animTargetOffsets, f32 frame, void **tpls
) {
    for (int i = 0; i < pAnimInfo->num; i++) {
        const res::AnimationTarget *pAnimTarget =
            detail::ConvertOffsToPtr<res::AnimationTarget>(pAnimInfo, animTargetOffsets[i]);
        if (pAnimTarget->id < pMaterial->GetTextureNum()) {
            const res::StepKey *keys = detail::ConvertOffsToPtr<res::StepKey>(pAnimTarget, pAnimTarget->keysOffset);
            u16 fileIdx = GetStepCurveValue(frame, keys, pAnimTarget->keyNum);
            if (tpls[fileIdx] != NULL) {
                pMaterial->GetTexturePtr(pAnimTarget->id)->ReplaceImage((TPLPalette *)tpls[fileIdx], 0);
            }
        }
    }
}

void AnimateIndTexSRT(
    Material *pMaterial, const res::AnimationInfo *pAnimInfo, const u32 *animTargetOffsets, f32 frame
) {
    for (int i = 0; i < pAnimInfo->num; i++) {
        const res::AnimationTarget *pAnimTarget =
            detail::ConvertOffsToPtr<res::AnimationTarget>(pAnimInfo, animTargetOffsets[i]);
        if (pAnimTarget->id < pMaterial->GetIndTexSRTCap()) {
            const res::HermiteKey *keys =
                detail::ConvertOffsToPtr<res::HermiteKey>(pAnimTarget, pAnimTarget->keysOffset);
            pMaterial->SetIndTexSRTElement(
                pAnimTarget->id, pAnimTarget->target, GetHermiteCurveValue(frame, keys, pAnimTarget->keyNum)
            );
        }
    }
}

bool IsBindAnimation(Material *pMaterial, AnimTransform *pAnimTrans) {
    return false; // Early return in release builds, no call to FindAnimationLink in SS, exists in tfp2

    if (pMaterial->FindAnimationLink(pAnimTrans)) {
        return true;
    }
    return false;
}

bool IsBindAnimation(Pane *pPane, AnimTransform *pAnimTrans) {
    return false; // Early return in release builds, no call to FindAnimationLink in SS, exists in tfp2

    if (pPane->FindAnimationLinkSelf(pAnimTrans)) {
        return true;
    }
    return false;
}

} // namespace

AnimTransform::AnimTransform() : mpRes(NULL), mFrame(0.0f) {}

AnimTransform::~AnimTransform() {}

u16 AnimTransform::GetFrameSize() const {
    return mpRes->frameSize;
}

bool AnimTransform::IsLoopData() const {
    return mpRes->loop;
}

AnimTransformBasic::AnimTransformBasic() : mpFileResAry(NULL), mAnimLinkAry(NULL), mAnimLinkNum(0) {}

AnimTransformBasic::~AnimTransformBasic() {
    Layout::DeletePrimArray(mAnimLinkAry);
    Layout::DeletePrimArray(mpFileResAry);
}

void AnimTransformBasic::SetResource(const res::AnimationBlock *pRes, ResourceAccessor *pResAccessor) {
    SetResource(pRes, pResAccessor, pRes->animContNum);
}

void AnimTransformBasic::SetResource(const res::AnimationBlock *pRes, ResourceAccessor *pResAccessor, u16 animNum) {
    mpRes = pRes;
    mpFileResAry = NULL;
    if (mpRes->fileNum != 0) {
        mpFileResAry = Layout::NewArray<void *>(mpRes->fileNum);
        if (mpFileResAry) {
            const u32 *fileNameOffsets = detail::ConvertOffsToPtr<u32>(mpRes, sizeof(*mpRes));
            for (int i = 0; i < mpRes->fileNum; i++) {
                const char *fileName = detail::GetStrTableStr(fileNameOffsets, i);
                mpFileResAry[i] = pResAccessor->GetResource('timg', fileName, NULL);
            }
        }
    }

    mAnimLinkAry = Layout::NewArray<AnimationLink>(animNum);
    if (mAnimLinkAry) {
        mAnimLinkNum = animNum;
    }
}

void AnimTransformBasic::Bind(Pane *pPane, bool bRecursive, bool bDisable) {
    AnimationLink *pCrAnimLink = NULL;
    const u32 *animContOffsets = detail::ConvertOffsToPtr<u32>(mpRes, mpRes->animContOffsetsOffset);
    for (u16 i = 0; i < mpRes->animContNum; i++) {
        const res::AnimationContent &animCont =
            *detail::ConvertOffsToPtr<res::AnimationContent>(mpRes, animContOffsets[i]);
        if (animCont.type == 0) {
            Pane *pFindPane = pPane->FindPaneByName(animCont.name, bRecursive);
            if (pFindPane && !IsBindAnimation(pFindPane, this)) {
                pCrAnimLink = Bind(pFindPane, pCrAnimLink, i, bDisable);
                if (pCrAnimLink == NULL) {
                    break;
                }
            }
        } else {
            Material *pFindMat = pPane->FindMaterialByName(animCont.name, bRecursive);
            if (pFindMat && !IsBindAnimation(pFindMat, this)) {
                pCrAnimLink = Bind(pFindMat, pCrAnimLink, i, bDisable);
                if (pCrAnimLink == NULL) {
                    break;
                }
            }
        }
    }
}

void AnimTransformBasic::Bind(Material *pMaterial, bool bDisable) {
    AnimationLink *pCrAnimLink = NULL;
    const u32 *animContOffsets = detail::ConvertOffsToPtr<u32>(mpRes, mpRes->animContOffsetsOffset);
    for (u16 i = 0; i < mpRes->animContNum; i++) {
        const res::AnimationContent &animCont =
            *detail::ConvertOffsToPtr<res::AnimationContent>(mpRes, animContOffsets[i]);
        if (animCont.type == 1) {
            if (detail::EqualsMaterialName(pMaterial->GetName(), animCont.name) && !IsBindAnimation(pMaterial, this)) {
                pCrAnimLink = Bind(pMaterial, pCrAnimLink, i, bDisable);
                if (pCrAnimLink == NULL) {
                    break;
                }
            }
        }
    }
}

void AnimTransformBasic::Animate(u32 idx, Pane *pPane) {
    u32 animContOffsets = detail::ConvertOffsToPtr<u32>(mpRes, mpRes->animContOffsetsOffset)[idx];
    const res::AnimationContent *pAnimCont = detail::ConvertOffsToPtr<res::AnimationContent>(mpRes, animContOffsets);
    const u32 *animInfoOffsets = detail::ConvertOffsToPtr<u32>(pAnimCont, sizeof(res::AnimationContent));

    for (int i = 0; i < pAnimCont->num; i++) {
        const res::AnimationInfo *pAnimInfo =
            detail::ConvertOffsToPtr<res::AnimationInfo>(pAnimCont, animInfoOffsets[i]);
        const u32 *animTargetOffsets = detail::ConvertOffsToPtr<u32>(pAnimInfo, sizeof(res::AnimationInfo));
        switch (pAnimInfo->kind) {
            case 'RLPA': AnimatePaneSRT(pPane, pAnimInfo, animTargetOffsets, mFrame); break;
            case 'RLVI': AnimateVisibility(pPane, pAnimInfo, animTargetOffsets, mFrame); break;
            case 'RLVC': AnimateVertexColor(pPane, pAnimInfo, animTargetOffsets, mFrame); break;
        }
    }
}

void AnimTransformBasic::Animate(u32 idx, Material *pMaterial) {
    u32 animContOffsets = detail::ConvertOffsToPtr<u32>(mpRes, mpRes->animContOffsetsOffset)[idx];
    const res::AnimationContent *pAnimCont = detail::ConvertOffsToPtr<res::AnimationContent>(mpRes, animContOffsets);
    const u32 *animInfoOffsets = detail::ConvertOffsToPtr<u32>(pAnimCont, sizeof(res::AnimationContent));

    for (int i = 0; i < pAnimCont->num; i++) {
        const res::AnimationInfo *pAnimInfo =
            detail::ConvertOffsToPtr<res::AnimationInfo>(pAnimCont, animInfoOffsets[i]);
        const u32 *animTargetOffsets = detail::ConvertOffsToPtr<u32>(pAnimInfo, sizeof(res::AnimationInfo));
        switch (pAnimInfo->kind) {
            case 'RLMC': AnimateMaterialColor(pMaterial, pAnimInfo, animTargetOffsets, mFrame); break;
            case 'RLTS': AnimateTextureSRT(pMaterial, pAnimInfo, animTargetOffsets, mFrame); break;
            case 'RLTP':
                if (mpFileResAry) {
                    AnimateTexturePattern(pMaterial, pAnimInfo, animTargetOffsets, mFrame, mpFileResAry);
                }
                break;
            case 'RLIM': AnimateIndTexSRT(pMaterial, pAnimInfo, animTargetOffsets, mFrame); break;
        }
    }
}

AnimationLink *AnimTransformBasic::FindUnbindLink(AnimationLink *pLink) const {
    if (pLink == NULL) {
        pLink = mAnimLinkAry;
    }

    while (pLink < &mAnimLinkAry[mAnimLinkNum]) {
        if (pLink->GetAnimTransform() == NULL) {
            return pLink;
        }
        pLink++;
    }

    return NULL;
}

template <typename T>
AnimationLink *AnimTransformBasic::Bind(T *pTarget, AnimationLink *pAnimLink, u16 idx, bool bDisable) {
    pAnimLink = FindUnbindLink(pAnimLink);
    if (!pAnimLink) {
        return NULL;
    }
    pAnimLink->Set(this, idx, bDisable);
    pTarget->AddAnimationLink(pAnimLink);
    pAnimLink++;
    return pAnimLink;
}

AnimResource::AnimResource() {
    Init();
}

void AnimResource::Set(const void *animResBuf) {
    Init();
    const res::BinaryFileHeader *pFileHeader = (res::BinaryFileHeader *)animResBuf;
    if (detail::TestFileHeader(*pFileHeader, 'RLAN')) {
        if (detail::TestFileVersion(*pFileHeader)) {
            mpFileHeader = pFileHeader;
            const res::DataBlockHeader *pDataBlockHead =
                detail::ConvertOffsToPtr<res::DataBlockHeader>(mpFileHeader, mpFileHeader->headerSize);
            for (int i = 0; i < mpFileHeader->dataBlocks; i++) {
                switch (detail::GetSignatureInt(pDataBlockHead->kind)) {
                    case 'pat1': mpTagBlock = (res::AnimationTagBlock *)pDataBlockHead; break;
                    case 'pah1': mpShareBlock = (res::AnimationShareBlock *)pDataBlockHead; break;
                    case 'pai1': mpResBlock = (res::AnimationBlock *)pDataBlockHead; break;
                }
                pDataBlockHead = detail::ConvertOffsToPtr<res::DataBlockHeader>(pDataBlockHead, pDataBlockHead->size);
            }
        }
    }
}

void AnimResource::Init() {
    mpFileHeader = NULL;
    mpResBlock = NULL;
    mpTagBlock = NULL;
    mpShareBlock = NULL;
}

u16 AnimResource::GetGroupNum() const {
    if (mpTagBlock) {
        return mpTagBlock->groupNum;
    }
    return 0;
}

const AnimationGroupRef *AnimResource::GetGroupArray() const {
    if (mpTagBlock) {
        const AnimationGroupRef *groups =
            detail::ConvertOffsToPtr<AnimationGroupRef>(mpTagBlock, mpTagBlock->groupsOffset);
        return groups;
    }
    return NULL;
}

bool AnimResource::IsDescendingBind() const {
    if (mpTagBlock) {
        return detail::TestBit(mpTagBlock->flag, 0);
    }
    return false;
}

u16 AnimResource::GetAnimationShareInfoNum() const {
    if (mpShareBlock) {
        return mpShareBlock->shareNum;
    }
    return 0;
}

const AnimationShareInfo *AnimResource::GetAnimationShareInfoArray() const {
    if (mpShareBlock) {
        return detail::ConvertOffsToPtr<AnimationShareInfo>(mpShareBlock, mpShareBlock->animShareInfoOffset);
    }
    return 0;
}

u16 AnimResource::CalcAnimationNum(Pane *pPane, bool bRecursive) const {
    u16 linkNum = 0;
    const u32 *animContOffsets = detail::ConvertOffsToPtr<u32>(mpResBlock, mpResBlock->animContOffsetsOffset);
    for (u16 i = 0; i < mpResBlock->animContNum; i++) {
        const res::AnimationContent &animCont =
            *detail::ConvertOffsToPtr<res::AnimationContent>(mpResBlock, animContOffsets[i]);
        if (animCont.type == 0) {
            Pane *pFindPane = pPane->FindPaneByName(animCont.name, bRecursive);
            if (pFindPane) {
                linkNum++;
            }
        } else {
            Material *pFindMat = pPane->FindMaterialByName(animCont.name, bRecursive);
            if (pFindMat) {
                linkNum++;
            }
        }
    }
    return linkNum;
}

u16 AnimResource::CalcAnimationNum(Group *pGroup, bool bRecursive) const {
    u16 linkNum = 0;
    detail::PaneLinkList &paneList = pGroup->GetPaneList();
    for (detail::PaneLinkList::Iterator it = paneList.GetBeginIter(); it != paneList.GetEndIter(); it++) {
        linkNum += CalcAnimationNum(it->mTarget, bRecursive);
    }
    return linkNum;
}

namespace detail {

u16 AnimPaneTree::FindAnimContent(const res::AnimationBlock *pAnimBlock, const char *animContName, u8 animContType) {
    const u32 *animContOffsets = detail::ConvertOffsToPtr<u32>(pAnimBlock, pAnimBlock->animContOffsetsOffset);
    for (u16 i = 0; i < pAnimBlock->animContNum; i++) {
        const res::AnimationContent *pAnimCont =
            detail::ConvertOffsToPtr<res::AnimationContent>(pAnimBlock, animContOffsets[i]);
        if (pAnimCont->type == animContType && EqualsMaterialName(pAnimCont->name, animContName)) {
            return i;
        }
    }
    return -1;
}

void AnimPaneTree::Init() {
    mLinkNum = 0;
    mAnimPaneIdx = 0;
    mAnimMatCnt = 0;
    for (u8 i = 0; i < 9; i++) {
        mAnimMatIdx[i] = 0;
    }
}

void AnimPaneTree::Set(Pane *pTargetPane, const AnimResource &animRes) {
    u16 linkNum = 0;
    const res::AnimationBlock *pAnimBlock = animRes.GetResourceBlock();

    u16 animContIdx = FindAnimContent(pAnimBlock, pTargetPane->GetName(), 0);
    if (animContIdx != 0xFFFF) {
        linkNum++;
    }

    u8 animMatCnt = pTargetPane->GetMaterialNum();
    u16 animMatIdxs[9];
    for (u8 i = 0; i < animMatCnt; i++) {
        animMatIdxs[i] = FindAnimContent(pAnimBlock, pTargetPane->GetMaterial(i)->GetName(), 1);
        if (animMatIdxs[i] != 0xFFFF) {
            linkNum++;
        }
    }
    if (linkNum != 0) {
        mAnimRes = animRes;
        mAnimPaneIdx = animContIdx;
        mAnimMatCnt = animMatCnt;
        for (u8 i = 0; i < animMatCnt; i++) {
            mAnimMatIdx[i] = animMatIdxs[i];
        }
        mLinkNum = linkNum;
    }
}

AnimTransform *AnimPaneTree::Bind(Layout *pLayout, Pane *pTargetPane, ResourceAccessor *pResAccessor) const {
    AnimTransformBasic *pAnimTrans = (AnimTransformBasic *)pLayout->CreateAnimTransform();
    pAnimTrans->SetResource(mAnimRes.GetResourceBlock(), pResAccessor, mLinkNum);

    AnimationLink *pCrAnimLink = NULL;
    if (mAnimPaneIdx != 0xFFFF) {
        pCrAnimLink = pAnimTrans->Bind(pTargetPane, NULL, mAnimPaneIdx, true);
    }

    u8 animMatMax = ut::Min<u8>(mAnimMatCnt, pTargetPane->GetMaterialNum());
    for (u8 i = 0; i < animMatMax; i++) {
        if (mAnimMatIdx[i] != 0xFFFF) {
            Material *pMaterial = pTargetPane->GetMaterial(i);
            pCrAnimLink = pAnimTrans->Bind(pMaterial, pCrAnimLink, mAnimMatIdx[i], true);
        }
    }

    return pAnimTrans;
}

AnimationLink *FindAnimationLink(AnimationLinkList *pAnimList, AnimTransform *pAnimTrans) {
    for (AnimationLinkList::Iterator it = pAnimList->GetBeginIter(); it != pAnimList->GetEndIter(); it++) {
        if (pAnimTrans == it->GetAnimTransform()) {
            return &*it;
        }
    }
    return NULL;
}

AnimationLink *FindAnimationLink(AnimationLinkList *pAnimList, const AnimResource &animRes) {
    for (AnimationLinkList::Iterator it = pAnimList->GetBeginIter(); it != pAnimList->GetEndIter(); it++) {
        if (animRes.GetResourceBlock() == it->GetAnimTransform()->GetAnimResource()) {
            return &*it;
        }
    }
    return NULL;
}

void UnbindAnimationLink(AnimationLinkList *pAnimList, AnimTransform *pAnimTrans) {
    AnimationLinkList::Iterator it = pAnimList->GetBeginIter();
    while (it != pAnimList->GetEndIter()) {
        AnimationLinkList::Iterator currIt = it++;
        if (pAnimTrans == NULL || currIt->GetAnimTransform() == pAnimTrans) {
            pAnimList->Erase(currIt);
            currIt->Reset();
        }
    }
}

} // namespace detail

} // namespace lyt

} // namespace nw4r
