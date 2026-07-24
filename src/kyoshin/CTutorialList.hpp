#pragma once

#include <types.h>

/* CTutorialList -- tutorial list widget.
   Vtable at lbl_eu_8053A2A8 (split1 .data). Layout-compatible with IWorkEvent
   via vptr at +0x00 for CDeviceFile::readFile, but not a C++ IWorkEvent
   subclass (avoids emitting weak default stubs into this TU's .text).

   Full field layout documented in __ct__CTutorialList (802AF55C). Only fields
   accessed by FULL_MATCH functions are explicitly declared; intermediate
   regions use opaque byte arrays at the correct offsets (verified against
   retail objdiff at 100% match). */
struct CTutorialList {
    void* mVtbl;            // 0x00

    // 0x04-0x83 -- UnkClass_8045F564 (0x10), 6 pointers, CCur (0x18), CScrollBar (0x40)
    u8 gap04[0x80];

    // 0x84 -- CSortMenu sub-object (in-place, ~0x68 bytes)
    u8 mSortMenu[4];        // minimal placeholder for address-of

    // 0x88-0x176 -- copy proxies, data blocks, control flags
    u8 gap88[0xEF];

    u8 mInitialized;        // 0x177 -- non-zero after construction
};
