#include "monolib/mpfsys/MPFDrawMdlNoColor.hpp"

extern "C" {
    s8 sMpfDrawMdlNoColorInitialized; // name-guess: TU-owned .sbss guard read/written by MPFDrawMdlNoColor::getInstance
    mpfsys::MPFDrawMdlNoColor* spMpfDrawMdlNoColorInstance; // name-guess: TU-owned .sbss slot returned by MPFDrawMdlNoColor::getInstance
    // Prototype instance storage ships from the retail data unit (monolibdata2).
    // Array-typed extern so MWCC emits the retail lis/addi (HA/LO) addressing
    // instead of sda21 for this .data object.
    extern u32 __vt__Q26mpfsys17MPFDrawMdlNoColor[4];
}

namespace mpfsys {

MPFDrawMdlNoColor* MPFDrawMdlNoColor::getInstance(){
    if(!sMpfDrawMdlNoColorInitialized){
        spMpfDrawMdlNoColorInstance = (mpfsys::MPFDrawMdlNoColor*)&__vt__Q26mpfsys17MPFDrawMdlNoColor;
        sMpfDrawMdlNoColorInitialized = 1;
    }
    return (mpfsys::MPFDrawMdlNoColor*)&spMpfDrawMdlNoColorInstance;
}

} // namespace mpfsys
