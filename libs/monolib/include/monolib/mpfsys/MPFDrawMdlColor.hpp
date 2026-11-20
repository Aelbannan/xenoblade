#pragma once

#include <types.h>

#include "monolib/mpfsys/MPFDraw.hpp"

struct MPFDrawMdlData;
struct MPFDrawMdlGroupList;
struct MPFDrawMdlGroupPairList;

namespace mpfsys {

// Draw dispatcher object for textured models with vertex colors.
class MPFDrawMdlColor : public MPFDraw {
public:
    static MPFDrawMdlColor* getInstance();

    virtual void drawAllWithColor(MPFDrawMdlData* data, const MPFDrawMdlGroupList* list);
    virtual void drawHalfWithColor(MPFDrawMdlData* data, const MPFDrawMdlGroupPairList* list);
};

} // namespace mpfsys
