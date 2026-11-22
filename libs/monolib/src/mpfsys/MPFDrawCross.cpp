#include "monolib/mpfsys/MPFDrawCross.hpp"

namespace mpfsys {

MPFDrawCross* MPFDrawCross::getInstance(){
    static MPFDrawCross instance;
    return &instance;
}

} // namespace mpfsys
