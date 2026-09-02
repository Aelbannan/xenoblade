#include <nw4r/lyt.h>
#include <nw4r/ut.h>

// Retail CreateAnimTransform references the Layout allocator through its
// sbss pool label (not the lbl_eu_80665488 slot the sibling lyt TUs use).
extern MEMAllocator* lbl_eu_80665478;

// TextBox's runtime type descriptor is resolved through the shared sbss
// pool slot rather than the nw4r::lyt::TextBox::typeInfo symbol itself.
extern const nw4r::ut::detail::RuntimeTypeInfo lbl_eu_80665488;

/******************************************************************************
 *
 * Utility functions
 *
 ******************************************************************************/

namespace nw4r {
namespace lyt {

namespace {

void SetTagProcessorImpl(Pane* pPane, ut::WideTagProcessor* pProcessor) {
    // Inlined ut::DynamicCast<TextBox*> against TextBox's type descriptor
    // (retail resolves the descriptor through the shared sbss pool slot).
    TextBox* pTextBox;

    // Unconditional address materialization: retail loads the descriptor
    // pointer before the pane null-test, so keep the assignment out of the
    // short-circuiting condition.
    const ut::detail::RuntimeTypeInfo* pDerivedTypeInfo;
    pDerivedTypeInfo = &lbl_eu_80665488;

    if (pPane != NULL &&
        pPane->GetRuntimeTypeInfo()->IsDerivedFrom(pDerivedTypeInfo)) {
        pTextBox = static_cast<TextBox*>(pPane);
    } else {
        pTextBox = NULL;
    }

    if (pTextBox != NULL) {
        pTextBox->SetTagProcessor(pProcessor);
    }

    NW4R_UT_LINKLIST_FOREACH (it, pPane->GetChildList(),
                              { SetTagProcessorImpl(&*it, pProcessor); })
}

} // namespace

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

// Retail nw4r::lyt::AnimResource::Set(const void*) - its class members are
// protected and the declaring header isn't extendable here, so reference the
// retail symbol directly.
extern "C" void Set__Q34nw4r3lyt12AnimResourceFPCv(
    AnimResource* _this, const void* pAnmBinary);

// Imports from lyt_animation.cpp (retail literal-mangled identifiers).
extern "C" int CalcAnimationNum__Q34nw4r3lyt12AnimResourceCFPQ34nw4r3lyt5Groupb(
    const AnimResource* _this, Group* pGroup, bool recursive);
extern "C" void* GetAnimationShareInfoArray__Q34nw4r3lyt12AnimResourceCFv(
    AnimResource* _this);
extern "C" void BindAnimation__Q24nw4r3lytFPQ34nw4r3lyt5GroupPQ34nw4r3lyt13AnimTransformbb(
    Group* pGroup, AnimTransform* pAnimTrans, bool descending, bool flag);

// Share info entry from the "pah1" block. The pane name sits at offset 0x0
// and the group name at 0x11; entries are walked with a 0x24-byte stride.
struct AnimationShareInfo {
    char name[NW4R_LYT_RES_NAME_LEN];      // at 0x0
    u8 PADDING_0x10;                       // at 0x10
    char groupName[NW4R_LYT_RES_NAME_LEN]; // at 0x11
    u8 PADDING_0x21[3];                    // at 0x21
};

namespace detail {

// Declaration-only mirror of lyt_animation.cpp's AnimPaneTree: this TU only
// declares the automatic object and calls Init/Set/Bind out-of-line. The
// AnimResource member makes the implicitly-generated constructor emit the
// out-of-line AnimResource() call seen in retail.
class AnimPaneTree {
public:
    void Init();
    void Set(Pane* pPane, const AnimResource& rResource);
    void Bind(Layout* pLayout, Pane* pPane,
              ResourceAccessor* pAccessor) const;

    AnimResource mResource; // at 0x0
    u16 mPaneIdx;           // at 0x10
    u16 mCount;             // at 0x12
};

} // namespace detail

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
        MEMFreeToAllocator(lbl_eu_80665478, pGroupContainer);
    }

    Pane* pRootPane = mpRootPane;

    // Retail evaluates the null-check twice (CSE'd into one compare with a
    // dead re-test); keep the duplicated test for byte parity.
    if (pRootPane != NULL) {
        if (!pRootPane->IsUserAllocated() && pRootPane != NULL) {
            pRootPane->~Pane();
            MEMFreeToAllocator(lbl_eu_80665478, pRootPane);
        }
    }

    NW4R_UT_LINKLIST_FOREACH_SAFE (it, mAnimTransList, {
        mAnimTransList.Erase(it);

        // Object pointer formed after the erase (retail ordering); the
        // destroy/free is guarded on it.
        AnimTransform* pAnimTrans = &*it;
        if (pAnimTrans != NULL) {
            pAnimTrans->~AnimTransform();
            MEMFreeToAllocator(lbl_eu_80665478, pAnimTrans);
        }
    })
    // mAnimTransList's LinkListImpl destructor is emitted automatically as
    // member destruction (guarded addic./beq call) - do not call it here.
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
    // Wrap the raw animation binary in an AnimResource, then defer to the
    // virtual AnimResource-based overload.
    // Retail elides the trivial AnimResource constructor (-ipa), so use
    // raw aligned storage instead of invoking the out-of-line ctor.
    u32 resourceStorage[sizeof(AnimResource) / sizeof(u32)];
    Set__Q34nw4r3lyt12AnimResourceFPCv(
        reinterpret_cast<AnimResource*>(resourceStorage), pAnmBinary);
    // The AnimResource overload returns void in this build; the transform
    // pointer is left in r3 by the virtual call.
    CreateAnimTransform(*reinterpret_cast<AnimResource*>(resourceStorage),
                        pAccessor);
}

// Mirror so we can read the protected AnimResource members (same pattern as
// lyt_animation.cpp).
namespace {
struct AnimResourceData {
    const res::BinaryFileHeader* mpFileHeader;    // at 0x0
    const res::AnimationBlock* mpResBlock;        // at 0x4
    const res::AnimationTagBlock* mpTagBlock;     // at 0x8
    const res::AnimationShareBlock* mpShareBlock; // at 0xC
};
} // namespace

void Layout::CreateAnimTransform(const AnimResource& rResource,
                                 ResourceAccessor* pAccessor) {
    const res::AnimationBlock* pBlock =
        reinterpret_cast<const AnimResourceData*>(&rResource)->mpResBlock;

    if (pBlock == NULL) {
        return;
    }

    AnimTransform* pAnimTrans = CreateAnimTransform();
    if (pAnimTrans != NULL) {
        pAnimTrans->SetResource(pBlock, pAccessor);
    }
}

AnimTransform* Layout::CreateAnimTransform() {
    // Retail builds an AnimTransformBasic from the static allocator and
    // registers it on the layout's animation list.
    void* pMem = MEMAllocFromAllocator(lbl_eu_80665478, sizeof(AnimTransformBasic));
    AnimTransform* pAnimTrans;

    if (pMem != NULL) {
        pAnimTrans = new (pMem) AnimTransformBasic;
    } else {
        pAnimTrans = NULL;
    }

    if (pAnimTrans != NULL) {
        mAnimTransList.PushBack(pAnimTrans);
    }

    return pAnimTrans;
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
    return ut::Rect(-mLayoutSize.width / 2.0f, mLayoutSize.height / 2.0f,
                    mLayoutSize.width / 2.0f, -mLayoutSize.height / 2.0f);
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

__attribute__((used, aligned(1))) char lyt_layout_sbss_pad[4];

// Retail Layout::BindAnimationAuto
// function because the retail vtable entry returns bool while the header
// declares void (same pattern as the other literal-mangled symbols above).
extern "C" bool BindAnimationAuto__Q34nw4r3lyt6LayoutFRCQ34nw4r3lyt12AnimResourcePQ34nw4r3lyt16ResourceAccessor(
    nw4r::lyt::Layout* _this, const nw4r::lyt::AnimResource& rResource,
    nw4r::lyt::ResourceAccessor* pAccessor) {
    using namespace nw4r::lyt;

    if (_this->GetRootPane() == NULL) {
        return false;
    }
    // Mirror read of the protected mpResBlock pointer.
    if (reinterpret_cast<const AnimResourceData*>(&rResource)->mpResBlock ==
        NULL) {
        return false;
    }

    AnimTransform* pAnimTrans = _this->CreateAnimTransform();

    int groupNum = rResource.GetGroupNum();
    int total = 0;

    if ((u16)groupNum == 0) {
        const res::AnimationBlock* pBlock =
            reinterpret_cast<const AnimResourceData*>(&rResource)->mpResBlock;

        // No tag groups: bind the whole animation to the root pane.
        pAnimTrans->SetResource(pBlock, pAccessor, pBlock->animContNum);
        _this->GetRootPane()->BindAnimation(pAnimTrans, true, true);
    } else {
        // Walkers mirror retail: loop 1 walks a copy of the array base, loop
        // 2 consumes the base pointer itself.
        const AnimationGroupRef* pGroupArray =
            static_cast<const AnimationGroupRef*>(rResource.GetGroupArray());
        const AnimationGroupRef* pIt = pGroupArray;

        // First pass: total animation-content count across all named groups.
        for (u16 i = 0; i < (u16)groupNum; i++) {
            Group* pGroup =
                _this->GetGroupContainer()->FindGroupByName(pIt->name);
            total += CalcAnimationNum__Q34nw4r3lyt12AnimResourceCFPQ34nw4r3lyt5Groupb(
                &rResource, pGroup, rResource.IsDescendingBind());
            pIt++;
        }

        pAnimTrans->SetResource(
            reinterpret_cast<const AnimResourceData*>(&rResource)->mpResBlock,
            pAccessor, (u16)total);

        // Second pass: bind each group's panes to the shared transform.
        for (u16 i = 0; i < (u16)groupNum; i++) {
            Group* pGroup =
                _this->GetGroupContainer()->FindGroupByName(pGroupArray->name);
            BindAnimation__Q24nw4r3lytFPQ34nw4r3lyt5GroupPQ34nw4r3lyt13AnimTransformbb(
                pGroup, pAnimTrans, rResource.IsDescendingBind(), true);
            pGroupArray++;
        }
    }

    // Shared animations: each share info names a pane (and a group whose
    // panes receive the same animation through an AnimPaneTree binding).
    int shareNum = rResource.GetAnimationShareInfoNum();
    if ((u16)shareNum != 0) {
        const AnimationShareInfo* pShareInfo = static_cast<const AnimationShareInfo*>(
            GetAnimationShareInfoArray__Q34nw4r3lyt12AnimResourceCFv(
                const_cast<AnimResource*>(&rResource)));

        for (u16 i = 0; i < (u16)shareNum; i++) {
            Pane* pFoundPane =
                _this->GetRootPane()->FindPaneByName(pShareInfo->name, true);

            detail::AnimPaneTree animTree;
            animTree.Init();
            animTree.Set(pFoundPane, rResource);

            if (animTree.mCount != 0) {
                Group* pGroup = _this->GetGroupContainer()->FindGroupByName(
                    pShareInfo->groupName);

                NW4R_UT_LINKLIST_FOREACH (it, pGroup->GetPaneList(), {
                    Pane* pTarget = it->mTarget;

                    // Skip the pane already bound by the tree; other group
                    // panes only bind when they belong to one of the
                    // resource's groups (or when the resource has none).
                    if (pTarget != pFoundPane &&
                        ((u16)groupNum == 0 ||
                         IsIncludeAnimationGroupRef(
                             _this->GetGroupContainer(),
                             static_cast<const AnimationGroupRef*>(
                                 rResource.GetGroupArray()),
                             (u16)groupNum, rResource.IsDescendingBind(),
                             pTarget))) {
                        animTree.Bind(_this, pTarget, pAccessor);
                    }
                })
            }

            pShareInfo++;
        }
    }

    return true;
}





