#pragma once

#include <types.h>

#include "monolib/mpfsys/MPFDrawBillboard.hpp"

namespace mpfsys {

// Draw dispatcher for texture-layered billboards; overrides both billboard slots.
class MPFDrawBillLayTex : public MPFDrawBillboard {
public:
    static MPFDrawBillLayTex* getInstance();

    virtual void drawIndexed(MPFDrawBillData* billboard, MPFDrawBillIndexList* list);
    virtual void drawPaired(MPFDrawBillData* billboard, MPFDrawBillPairList* list);
};

} // namespace mpfsys
