#pragma once

#include <types.h>

namespace mpfsys {

class MPFDrawBillboard {
public:
    static MPFDrawBillboard* getInstance();
    void drawIndexed();
    void drawPaired();
};

} // namespace mpfsys
