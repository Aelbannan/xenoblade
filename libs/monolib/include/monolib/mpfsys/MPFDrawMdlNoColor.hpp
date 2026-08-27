#pragma once

#include <types.h>

namespace mpfsys {

class MPFDrawMdlNoColor {
public:
    static MPFDrawMdlNoColor* getInstance();
    void drawAllNoColor();
    void drawHalfNoColor();
};

} // namespace mpfsys
