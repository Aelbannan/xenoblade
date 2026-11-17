#include <nw4r/lyt.h>
#include <nw4r/ut.h>

#include <cstring>

// Retail name of lyt::Layout::mspAllocator (shared SDA storage). The static
// member's mangled name (mspAllocator__Q34nw4r3lyt6Layout) cannot be renamed
// from source, so the retail lbl label is referenced directly (PLAN.md §17.6).


namespace nw4r {
namespace lyt {

/******************************************************************************
 *
 * Group
 *
 ******************************************************************************/
// NOTE: Init/AppendPane are inline: every call site (Group::Group) inlines
// these bodies, and retail keeps no out-of-line copies in this split, so
// no T copy is emitted here (a non-inline definition would always emit one).
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

inline void Group::Init() {
    mbUserAllocated = false;
}

Group::~Group() {
    NW4R_UT_LINKLIST_FOREACH_SAFE (it, mPaneLinkList, {
        mPaneLinkList.Erase(it);

        // Layout::DeleteObj spelled out so the allocator is referenced by its
        // retail label (lbl_eu_80665478) instead of the mangled static-member
        // name (same pattern as GroupContainer::~GroupContainer below).
        detail::PaneLink* pObj = &*it;
        if (pObj != NULL) {
            pObj->~PaneLink();
            Layout::FreeMemory(pObj);
        }
    })
}

inline void Group::AppendPane(Pane* pPane) {
    // Layout::NewObj<detail::PaneLink> spelled out against the retail
    // allocator label (see ~Group above); body mirrors the lyt_layout.h
    // template exactly so the schedule is unchanged.
    void* pMem = Layout::AllocMemory(sizeof(detail::PaneLink));
    detail::PaneLink* pLink;
    if (pMem != NULL) {
        pLink = new (pMem) detail::PaneLink();
    } else {
        pLink = NULL;
    }

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
                Layout::FreeMemory(pGroup);
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
