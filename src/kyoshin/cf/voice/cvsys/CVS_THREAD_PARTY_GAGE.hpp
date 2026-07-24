#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

/**
 * CVS_THREAD_PARTY_GAGE - voice thread for party gauge change events.
 *
 * Inherits from CVS_THREAD and allocates a 0x8C (140) byte buffer
 * for party gauge voice processing.
 */
class CVS_THREAD_PARTY_GAGE : public CVS_THREAD {
public:
    // Size of the thread-local voice buffer in bytes.
    static const int BUFFER_SIZE = 0x8C;

    // Pointer to the party member character data (offset 0x20).
    u32* partyMember;

    // Pointer to associated gauge data (offset 0x24).
    u32* gaugeData;

    // Gauge change threshold level (0=low, 1=mid, 2=high) (offset 0x28).
    u32 thresholdLevel;
};
