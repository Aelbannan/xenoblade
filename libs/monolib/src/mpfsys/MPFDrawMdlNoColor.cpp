#include "monolib/mpfsys/MPFDrawMdlNoColor.hpp"

namespace mpfsys {

MPFDrawMdlNoColor* MPFDrawMdlNoColor::getInstance(){
    static MPFDrawMdlNoColor instance;
    return &instance;
}

} // namespace mpfsys
