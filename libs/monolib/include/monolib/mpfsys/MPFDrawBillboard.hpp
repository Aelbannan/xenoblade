#pragma once

#include <types.h>

#include "monolib/mpfsys/MPFDraw.hpp"

struct MPFDrawBillData;
struct MPFDrawBillIndexList;
struct MPFDrawBillPairList;

namespace mpfsys {

// Draw dispatcher for unlayered billboards.
class MPFDrawBillboard : public MPFDraw {
public:
    static MPFDrawBillboard* getInstance();

    virtual void drawIndexed(MPFDrawBillData* billboard, MPFDrawBillIndexList* list);
    virtual void drawPaired(MPFDrawBillData* billboard, MPFDrawBillPairList* list);
};

} // namespace mpfsys
