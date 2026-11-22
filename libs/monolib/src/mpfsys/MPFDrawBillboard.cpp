#include "monolib/mpfsys/MPFDrawBillboard.hpp"

namespace mpfsys {

MPFDrawBillboard* MPFDrawBillboard::getInstance(){
    static MPFDrawBillboard instance;
    return &instance;
}

} // namespace mpfsys
