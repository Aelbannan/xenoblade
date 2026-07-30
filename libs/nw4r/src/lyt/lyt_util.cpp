#include <nw4r/lyt.h>

namespace nw4r {
namespace lyt {

void BindAnimation(Group* pGroup, AnimTransform* pAnimTrans, bool recursive,
                   bool disable) {
    detail::PaneLinkList& rList = pGroup->GetPaneList();

    for (detail::PaneLinkList::Iterator it = rList.GetBeginIter();
         it != rList.GetEndIter(); ++it) {
        it->mTarget->BindAnimation(pAnimTrans, recursive, disable);
    }
}

} // namespace lyt
} // namespace nw4r
