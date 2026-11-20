#pragma once

#include <types.h>

#include "monolib/mpfsys/MPFDraw.hpp"

struct MPFDrawMdlData;
struct MPFDrawMdlGroupList;
struct MPFDrawMdlGroupPairList;

namespace mpfsys {

class MPFDrawMdlNoColor : public MPFDraw {
public:
    static MPFDrawMdlNoColor* getInstance();

    virtual void drawAllNoColor(MPFDrawMdlData* data, const MPFDrawMdlGroupList* list);
    virtual void drawHalfNoColor(MPFDrawMdlData* data, const MPFDrawMdlGroupPairList* list);
};

} // namespace mpfsys
