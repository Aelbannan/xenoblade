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

// Minimal interface exposing the input-state getter at vtable +0x8.
struct ICamControlIf {
    virtual ~ICamControlIf();          // vtable +0x0
    virtual void slot4();              // vtable +0x4
    virtual ICamControlStatus* getStatus();   // vtable +0x8
};

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
