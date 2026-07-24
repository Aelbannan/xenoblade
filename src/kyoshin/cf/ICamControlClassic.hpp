#pragma once

/**
 * Classic Controller camera control interface.
 * Provides access to classic controller pad input for camera systems.
 */

#include <types.h>

class CPad;

namespace cf {
    class ICamControlClassic {
    public:
        virtual ~ICamControlClassic();
        virtual void* vmethod_04();     // placeholder: vtable +0x4
        virtual void* getPadStatus();   // vtable +0x8 - returns input status object

        // Returns the main controller pad (index 0).
        static CPad* getMainPad();
    };
}
