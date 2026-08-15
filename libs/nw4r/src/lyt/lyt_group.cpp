#include <nw4r/lyt.h>
#include <nw4r/ut.h>

#include <cstring>

// Retail name of lyt::Layout::mspAllocator (shared SDA storage). The static
// member's mangled name (mspAllocator__Q34nw4r3lyt6Layout) cannot be renamed
// from source, so the retail lbl label is referenced directly (PLAN.md §17.6).
extern "C" MEMAllocator* lbl_eu_80665478;

namespace nw4r {
namespace lyt {

/******************************************************************************
 *
 * Group
 *
 ******************************************************************************/
Group::Group(const res::Group* pRes, Pane* pRootPane) {
    Init();

    std::strncpy(mName, pRes->name, NW4R_LYT_RES_NAME_LEN);
    mName[NW4R_LYT_RES_NAME_LEN] = '\0';

    const char* pNameBase =
        detail::ConvertOffsToPtr<char>(pRes, sizeof(res::Group));

    for (int i = 0; i < pRes->paneNum; i++) {
        Pane* pResult = pRootPane->FindPaneByName(
            pNameBase + i * NW4R_LYT_RES_NAME_LEN, true);

        if (pResult != NULL) {
            AppendPane(pResult);
        }
    }
}

void Group::Init() {
    mbUserAllocated = false;
}

Group::~Group() {
    NW4R_UT_LINKLIST_FOREACH_SAFE (it, mPaneLinkList, {
        mPaneLinkList.Erase(it);
        Layout::DeleteObj(&*it);
    })
}

void Group::AppendPane(Pane* pPane) {
    detail::PaneLink* pLink = Layout::NewObj<detail::PaneLink>();

    if (pLink != NULL) {
        pLink->mTarget = pPane;
        mPaneLinkList.PushBack(pLink);
    }
}

/******************************************************************************
 *
 * GroupContainer
 *
 ******************************************************************************/
GroupContainer::~GroupContainer() {
    NW4R_UT_LINKLIST_FOREACH_SAFE (it, mGroupList, {
        mGroupList.Erase(it);

        if (!it->IsUserAllocated()) {
            Group* pGroup = &*it;

            // Layout::DeleteObj spelled out so the allocator is referenced by
            // its retail label (lbl_eu_80665478) instead of the mangled
            // static-member name.
            if (pGroup != NULL) {
                pGroup->~Group();
                MEMFreeToAllocator(lbl_eu_80665478, pGroup);
            }
        }
    })
}

void GroupContainer::AppendGroup(Group* pGroup) {
    mGroupList.PushBack(pGroup);
}

Group* GroupContainer::FindGroupByName(const char* pName) {
    NW4R_UT_LINKLIST_FOREACH (it, mGroupList, {
        if (detail::EqualsResName(it->GetName(), pName)) {
            return &*it;
        }
    })

    return NULL;
}

} // namespace lyt
} // namespace nw4r
