#pragma once

/**
 * Classic Controller camera control interface.
 * Provides access to classic controller pad input for camera systems.
 */

#include <types.h>

class CPad;

// Flag-word storage returned by the classic-controller input-state getter.
struct ICamControlStatus {
    u32 field_0x0;
    u32 field_0x4;
};

// Minimal interface exposing the input-state getter as the FIRST virtual method
// (MWCC vtable layout: offset@0, typeinfo@4, then methods -> getter sits at +0x8).
struct ICamControlIf {
    virtual ICamControlStatus* getStatus();   // vtable +0x8
};

namespace cf {
    class ICamControlClassic {
    public:
        virtual ~ICamControlClassic();
        virtual void* vmethod_04();     // placeholder: vtable +0x4
        virtual ICamControlStatus* getPadStatus();   // vtable +0x8 - returns input status object

        // Returns the main controller pad (index 0).
        static CPad* getMainPad();
    };
}
