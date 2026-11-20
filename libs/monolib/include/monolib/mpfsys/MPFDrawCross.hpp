#pragma once

#include <types.h>

#include "monolib/mpfsys/MPFDraw.hpp"

struct MPFDrawBillData;
struct MPFDrawBillIndexList;
struct MPFDrawBillPairList;

namespace mpfsys {

// Draw dispatcher for cross-shaped billboards.
class MPFDrawCross : public MPFDraw {
public:
    static MPFDrawCross* getInstance();

    virtual void submitBillIndexed(MPFDrawBillData* billboard, MPFDrawBillIndexList* list);
    virtual void submitBillPaired(MPFDrawBillData* billboard, MPFDrawBillPairList* list);
};

} // namespace mpfsys
