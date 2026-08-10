#include <nw4r/lyt.h>
#include <nw4r/ut.h>

/******************************************************************************
 *
 * Utility functions
 *
 ******************************************************************************/
namespace {

using namespace nw4r;
using namespace nw4r::lyt;

void SetTagProcessorImpl(Pane* pPane, ut::WideTagProcessor* pProcessor) {
    TextBox* pTextBox = ut::DynamicCast<TextBox*>(pPane);
    if (pTextBox != NULL) {
        pTextBox->SetTagProcessor(pProcessor);
    }

    NW4R_UT_LINKLIST_FOREACH (it, pPane->GetChildList(),
                              { SetTagProcessorImpl(&*it, pProcessor); })
}

} // namespace

namespace nw4r {
namespace lyt {

// IsIncludeAnimationGroupRef: anonymous-namespace helper (retail scope
// nw4r::lyt::@unnamed@lyt_layout_cpp@). The extern reference keeps MWCC's
// -ipa file from dead-code-eliminating the unused anon-ns function.
struct AnimationGroupRef {
    char name[NW4R_LYT_RES_NAME_LEN]; // at 0x0
    u16 groupNum;                     // at 0x10
    u8 PADDING_0x12[0x14 - 0x12];     // at 0x12
};

// IsIncludeAnimationGroupRef: anonymous-namespace helper (retail scope
// nw4r::lyt::@unnamed@lyt_layout_cpp@). The extern reference keeps MWCC's
// -ipa file from dead-code-eliminating the unused anon-ns function.
//
// Walks each group referenced by pGroupRef[0..groupRefNum) and checks
// whether pPane (or, when descending, any ancestor of pPane) is a member
// of that group. Returns true on the first hit.
namespace {
bool IsIncludeAnimationGroupRef(nw4r::lyt::GroupContainer* pGroupContainer,
                                const nw4r::lyt::AnimationGroupRef* pGroupRef,
                                u16 groupRefNum, bool descending,
                                nw4r::lyt::Pane* pPane) {
    for (u16 i = 0; i < groupRefNum; i++) {
        nw4r::lyt::Group* pGroup =
            pGroupContainer->FindGroupByName(pGroupRef[i].name);

        NW4R_UT_LINKLIST_FOREACH (it, pGroup->GetPaneList(), {
            nw4r::lyt::Pane* pTargetPane = it->mTarget;

            if (pTargetPane == pPane) {
                return true;
            }

            if (descending) {
                for (nw4r::lyt::Pane* pParent = pPane->GetParent();
                     pParent != NULL; pParent = pParent->GetParent()) {
                    if (pTargetPane == pParent) {
                        return true;
                    }
                }
            }
        })
    }

    return false;
}
}

extern void* LLMH_force_us_80402184 = (void*)&IsIncludeAnimationGroupRef;

MEMAllocator* Layout::mspAllocator = NULL;

/******************************************************************************
 *
 * Layout
 *
 ******************************************************************************/

Layout::Layout()
    : mpRootPane(NULL),
      mpGroupContainer(NULL),
      mLayoutSize(0.0f, 0.0f) {}

Layout::~Layout() {
    GroupContainer* pGroupContainer = mpGroupContainer;

    if (pGroupContainer != NULL) {
        pGroupContainer->~GroupContainer();
        FreeMemory(pGroupContainer);
    }

    // Loaded only now (retail schedules the mpRootPane load after the
    // group-container block; hoisting it would add an extra instruction).
    Pane* pRootPane = mpRootPane;

    if (pRootPane != NULL && !pRootPane->IsUserAllocated()) {
        pRootPane->~Pane();
        FreeMemory(pRootPane);
    }

    NW4R_UT_LINKLIST_FOREACH_SAFE (it, mAnimTransList, {
        mAnimTransList.Erase(it);
        it->~AnimTransform();
        Layout::FreeMemory(&*it);
    })

    nw4r::ut::detail::LinkListImpl &rListImpl = mAnimTransList;
    rListImpl.~LinkListImpl();
}

bool Layout::Build(const void* pLytBinary, ResourceAccessor* pAccessor) {
    const res::BinaryFileHeader* const pHeader =
        static_cast<const res::BinaryFileHeader*>(pLytBinary);

    if (!detail::TestFileHeader(*pHeader, SIGNATURE)) {
        return false;
    }

    // Header version must be 0x0008..0x000A (minor byte in [8, 10], major
    // byte zero), like animation resources.
    u32 version = pHeader->version;
    bool versionOk = ((version >> 8) & 0xFF) == 0 && (version & 0xFF) >= 8 &&
                     (version & 0xFF) <= 0xA;
    if (!versionOk) {
        return false;
    }

    ResBlockSet blockSet;
    blockSet.pTextureList = NULL;
    blockSet.pFontList = NULL;
    blockSet.pMaterialList = NULL;
    blockSet.pResAccessor = pAccessor;

    Pane* pParentPane = NULL;
    Pane* pPrevPane = NULL;

    bool readRootGroup = false;
    int groupDepth = 0;

    const u8* pBlockData =
        static_cast<const u8*>(pLytBinary) + pHeader->headerSize;

    for (int i = 0; i < pHeader->dataBlocks; i++) {
        const res::DataBlockHeader* pBlockHeader =
            reinterpret_cast<const res::DataBlockHeader*>(pBlockData);

        // Signatures are compared as signed values (retail dispatches with
        // `cmpw`), so cast the u32 FOURCC constants down to s32.
        s32 kind = detail::GetSignatureInt(pBlockHeader->kind);

        switch (kind) {
        case static_cast<s32>(res::Layout::SIGNATURE): {
            const res::Layout* pRes =
                reinterpret_cast<const res::Layout*>(pBlockData);

            mLayoutSize = pRes->layoutSize;
            break;
        }

        case static_cast<s32>(SIGNATURE_TEXTURELIST): {
            blockSet.pTextureList =
                reinterpret_cast<const res::TextureList*>(pBlockData);
            break;
        }

        case static_cast<s32>(SIGNATURE_FONTLIST): {
            blockSet.pFontList =
                reinterpret_cast<const res::FontList*>(pBlockData);
            break;
        }

        case static_cast<s32>(SIGNATURE_MATERIALLIST): {
            blockSet.pMaterialList =
                reinterpret_cast<const res::MaterialList*>(pBlockData);
            break;
        }

        case static_cast<s32>(res::Pane::SIGNATURE):
        case static_cast<s32>(res::Picture::SIGNATURE):
        case static_cast<s32>(res::TextBox::SIGNATURE):
        case static_cast<s32>(res::Window::SIGNATURE):
        case static_cast<s32>(res::Bounding::SIGNATURE): {
            Pane* pPane = BuildPaneObj(kind, pBlockData, blockSet);

            if (pPane != NULL) {
                if (mpRootPane == NULL) {
                    mpRootPane = pPane;
                }

                if (pParentPane != NULL) {
                    pParentPane->AppendChild(pPane);
                }

                pPrevPane = pPane;
            }

            break;
        }

        case static_cast<s32>(SIGNATURE_PANESTART): {
            pParentPane = pPrevPane;
            break;
        }

        case static_cast<s32>(SIGNATURE_PANEEND): {
            pPrevPane = pParentPane;
            pParentPane = pPrevPane->GetParent();
            break;
        }

        case static_cast<s32>(res::Group::SIGNATURE): {
            if (!readRootGroup) {
                readRootGroup = true;
                mpGroupContainer = Layout::NewObj<GroupContainer>();
                break;
            }

            if (mpGroupContainer != NULL && groupDepth == 1) {
                Group* pGroup = Layout::NewObj<Group>(
                    reinterpret_cast<const res::Group*>(pBlockHeader),
                    mpRootPane);

                if (pGroup != NULL) {
                    mpGroupContainer->AppendGroup(pGroup);
                }
            }

            break;
        }

        case static_cast<s32>(SIGNATURE_GROUPSTART): {
            groupDepth++;
            break;
        }

        case static_cast<s32>(SIGNATURE_GROUPEND): {
            groupDepth--;
            break;
        }
        }

        pBlockData = pBlockData + pBlockHeader->size;
    }

    return true;
}

AnimTransform* Layout::CreateAnimTransform(const void* pAnmBinary,
                                           ResourceAccessor* pAccessor) {

    const res::BinaryFileHeader* const pHeader =
        static_cast<const res::BinaryFileHeader*>(pAnmBinary);

    if (!detail::TestFileHeader(*pHeader)) {
        return NULL;
    }

    const res::AnimationBlock* pAnimBlock = NULL;

    const res::DataBlockHeader* pBlockHeader =
        detail::ConvertOffsToPtr<res::DataBlockHeader>(pHeader,
                                                       pHeader->headerSize);

    AnimTransform* pResult = NULL;

    for (int i = 0; i < pHeader->dataBlocks; i++) {
        switch (detail::GetSignatureInt(pBlockHeader->kind)) {
        case SIGNATURE_ANIMATIONINFO: {

            switch (detail::GetSignatureInt(pHeader->signature)) {
            case SIGNATURE_ANIMATION:
            case res::AnimationInfo::SIGNATURE_ANMPANESRT:
            case res::AnimationInfo::SIGNATURE_ANMPANEVIS:
            case res::AnimationInfo::SIGNATURE_ANMVTXCLR:
            case res::AnimationInfo::SIGNATURE_ANMMATCLR:
            case res::AnimationInfo::SIGNATURE_ANMTEXSRT:
            case res::AnimationInfo::SIGNATURE_ANMTEXPAT: {
                AnimTransformBasic* pAnimTrans =
                    Layout::NewObj<AnimTransformBasic>();

                if (pAnimTrans != NULL) {
                    pAnimBlock = reinterpret_cast<const res::AnimationBlock*>(
                        pBlockHeader);

                    pAnimTrans->SetResource(pAnimBlock, pAccessor);
                    pResult = pAnimTrans;
                }

                break;
            }
            }

            if (pResult != NULL) {
                mAnimTransList.PushBack(pResult);
            }

            break;
        }
        }

        pBlockHeader = detail::ConvertOffsToPtr<res::DataBlockHeader>(
            pBlockHeader, pBlockHeader->size);
    }

    return pResult;
}

void Layout::BindAnimation(AnimTransform* pAnimTrans) {
    if (mpRootPane == NULL) {
        return;
    }

    mpRootPane->BindAnimation(pAnimTrans, true, false);
}

void Layout::UnbindAnimation(AnimTransform* pAnimTrans) {
    if (mpRootPane == NULL) {
        return;
    }

    mpRootPane->UnbindAnimation(pAnimTrans, true);
}

void Layout::UnbindAllAnimation() {
    UnbindAnimation(NULL);
}

void Layout::SetAnimationEnable(AnimTransform* pAnimTrans, bool enable) {
    if (mpRootPane == NULL) {
        return;
    }

    mpRootPane->SetAnimationEnable(pAnimTrans, enable, true);
}

void Layout::CalculateMtx(const DrawInfo& rInfo) {
    if (mpRootPane == NULL) {
        return;
    }

    mpRootPane->CalculateMtx(rInfo);
}

void Layout::Draw(const DrawInfo& rInfo) {
    if (mpRootPane == NULL) {
        return;
    }

    mpRootPane->Draw(rInfo);
}

void Layout::Animate(u32 option) {
    if (mpRootPane == NULL) {
        return;
    }

    mpRootPane->Animate(option);
}

ut::Rect Layout::GetLayoutRect() const {
    if (mOriginType == ORIGINTYPE_CENTER) {
        return ut::Rect(-mLayoutSize.width / 2, mLayoutSize.height / 2,
                        mLayoutSize.width / 2, -mLayoutSize.height / 2);
    }

    return ut::Rect(0.0f, 0.0f, mLayoutSize.width, mLayoutSize.height);
}

void Layout::SetTagProcessor(ut::WideTagProcessor* pProcessor) {
    SetTagProcessorImpl(mpRootPane, pProcessor);
}

Pane* Layout::BuildPaneObj(s32 kind, const void* pBinary,
                           const ResBlockSet& rBlockSet) {

    switch (kind) {
    case res::Pane::SIGNATURE: {
        const res::Pane* pRes = static_cast<const res::Pane*>(pBinary);
        return Layout::NewObj<Pane>(pRes);
    }

    case res::Picture::SIGNATURE: {
        const res::Picture* pRes = static_cast<const res::Picture*>(pBinary);
        return Layout::NewObj<Picture>(pRes, rBlockSet);
    }

    case res::TextBox::SIGNATURE: {
        const res::TextBox* pRes = static_cast<const res::TextBox*>(pBinary);
        return Layout::NewObj<TextBox>(pRes, rBlockSet);
    }

    case res::Window::SIGNATURE: {
        const res::Window* pRes = static_cast<const res::Window*>(pBinary);
        return Layout::NewObj<Window>(pRes, rBlockSet);
    }

    case res::Bounding::SIGNATURE: {
        const res::Bounding* pRes = static_cast<const res::Bounding*>(pBinary);
        return Layout::NewObj<Bounding>(pRes, rBlockSet);
    }

    default: {
        return NULL;
    }
    }
}

} // namespace lyt
} // namespace nw4r

void BindAnimationAuto__Q34nw4r3lyt6LayoutFRCQ34nw4r3lyt12AnimResourcePQ34nw4r3lyt16ResourceAccessor(){}





