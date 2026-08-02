#ifndef TRK_CIRCLE_BUFFER_H
#define TRK_CIRCLE_BUFFER_H

#include "PowerPC_EABI_Support/MetroTRK/dstypes.h"

/*
 * Ring buffer shuttling data between the gdev/TRK transport layers.
 *
 * Invariants:
 *   - mBytesToRead + mBytesToWrite == mBufSize
 *   - mReadPtr / mWritePtr advance within [mBufStart, mBufStart + mBufSize)
 *     and wrap back to mBufStart when they reach the end
 *   - mSection guards all mutations (interrupts disabled while inside)
 */
typedef struct CircleBuffer {
    ui8* mReadPtr;       // next byte to be read
    ui8* mWritePtr;      // next byte to be written
    ui8* mBufStart;      // start of the backing buffer
    ui32 mBufSize;       // capacity of the backing buffer
    ui32 mBytesToRead;   // bytes available for reading
    ui32 mBytesToWrite;  // bytes available for writing
    unsigned int mSection; // critical-section state
    ui32 mPad;           // padding
} CircleBuffer;


ui32 CBGetBytesAvailableForRead(const CircleBuffer* cb);
ui32 CBGetBytesAvailableForWrite(const CircleBuffer* cb);
void CircleBufferInitialize(CircleBuffer* cb, ui8* buf, ui32 size);
void CircleBufferTerminate(CircleBuffer* cb);
int CircleBufferWriteBytes(CircleBuffer* cb, const ui8* buf, ui32 size);
int CircleBufferReadBytes(CircleBuffer* cb, ui8* buf, ui32 size);

#endif
