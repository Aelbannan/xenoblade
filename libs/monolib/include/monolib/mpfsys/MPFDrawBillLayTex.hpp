#pragma once

#include <types.h>

namespace mpfsys {

class MPFDrawBillLayTex {
public:
    static MPFDrawBillLayTex* getInstance();
    void drawIndexed();
    void drawPaired();

private:
    // The retail singleton occupies sixteen bytes.
    u8 mData[0x10];
};

} // namespace mpfsys
