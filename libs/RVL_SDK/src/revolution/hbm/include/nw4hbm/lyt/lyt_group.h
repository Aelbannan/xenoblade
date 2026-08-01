#ifndef NW4HBM_LYT_GROUP_H
#define NW4HBM_LYT_GROUP_H
#include <nw4hbm/types_nw4hbm.h>

#include <nw4hbm/lyt/lyt_resources.h>

#include <nw4hbm/lyt/lyt_layout.h>

#include <nw4hbm/ut.h>

namespace nw4hbm {
namespace lyt {

// Forward declarations
class Pane;

namespace detail {

/******************************************************************************
 *
 * PaneLink
 *
 ******************************************************************************/
struct PaneLink {
    NW4R_UT_LINKLIST_NODE_DECL(); // at 0x4
    Pane* mTarget;                // at 0x8
};

NW4R_UT_LINKLIST_TYPEDEF_DECL(PaneLink);

} // namespace detail

namespace res {

/******************************************************************************
 *
 * GRP1 binary layout
 *
 ******************************************************************************/
struct Group {
    static const u32 SIGNATURE = FOURCC('g', 'r', 'p', '1');

    DataBlockHeader blockHeader;      // at 0x0
    char name[NW4R_LYT_RES_NAME_LEN]; // at 0x8
    u16 paneNum;                      // at 0x18
    u8 PADDING_0x1A[0x1C - 0x1A];     // at 0x1A
};

} // namespace res

/******************************************************************************
 *
 * Group
 *
 ******************************************************************************/
class Group {
public:
    Group(const res::Group* pRes, Pane* pRootPane);
    virtual ~Group(); // at 0x8

    void AppendPane(Pane* pPane) {
        void* pBuffer = Layout::AllocMemory(sizeof(detail::PaneLink));
        if (pBuffer == NULL) {
            return;
        }

        detail::PaneLink* pLink = new (pBuffer) detail::PaneLink();
        pLink->mTarget = pPane;

        mPaneLinkList.PushBack(pLink);
    }

    detail::PaneLinkList& GetPaneList() {
        return mPaneLinkList;
    }

    const char* GetName() const {
        return mName;
    }

    bool IsUserAllocated() const {
        return mbUserAllocated;
    }

public:
    NW4R_UT_LINKLIST_NODE_DECL(); // at 0x4

protected:
    detail::PaneLinkList mPaneLinkList;    // at 0xC
    char mName[NW4R_LYT_RES_NAME_LEN];     // at 0x18 (16 bytes)
    bool mbUserAllocated;                  // at 0x28
    u8 PADDING_0x29[0x2C - 0x29];          // at 0x29

private:
    void Init() {
        mbUserAllocated = false;
    }
};

NW4R_UT_LINKLIST_TYPEDEF_DECL(Group);

/******************************************************************************
 *
 * GroupContainer
 *
 ******************************************************************************/
class GroupContainer {
public:
    GroupContainer() {}
    ~GroupContainer();

    void AppendGroup(Group* pGroup);
    Group* FindGroupByName(const char* pName);

    GroupList& GetGroupList() {
        return mGroupList;
    }

protected:
    GroupList mGroupList; // at 0x0
};

} // namespace lyt
} // namespace nw4hbm

#endif
