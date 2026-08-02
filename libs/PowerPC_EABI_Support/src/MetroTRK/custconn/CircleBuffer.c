#include "PowerPC_EABI_Support/MetroTRK/custconn/CircleBuffer.h"
#include "PowerPC_EABI_Support/MetroTRK/custconn/MWCriticalSection_gc.h"
#include "PowerPC_EABI_Support/MetroTRK/trk.h"

ui32 CBGetBytesAvailableForRead(const CircleBuffer* cb){
    return cb->mBytesToRead;
}

// not present in the retail binary; kept commented out so the unit
// stays within its split budget (the functions below would otherwise
// emit 0x48 bytes of unreferenced .text)
//ui32 CBGetBytesAvailableForWrite(const CircleBuffer* cb){
//    return cb->mBytesToWrite;
//}

void CircleBufferInitialize(CircleBuffer* cb, ui8* buf, ui32 size){
    cb->mBufStart = buf;
    cb->mBufSize = size;
    cb->mReadPtr = buf;
    cb->mWritePtr = buf;
    cb->mBytesToRead = 0;
    cb->mBytesToWrite = size;
    MWInitializeCriticalSection(&(cb->mSection));
}

// not present in the retail binary; kept commented out so the unit
// stays within its split budget (the functions below would otherwise
// emit 0x48 bytes of unreferenced .text)
//void CircleBufferTerminate(CircleBuffer* cb){
//    MWTerminateCriticalSection(&(cb->mSection));
//    memset(cb,0,sizeof(CircleBuffer) - 4);
//}

int CircleBufferWriteBytes(CircleBuffer* cb, const ui8* buf, ui32 size){
    ui32 spaceToEnd;
    
    if(size > cb->mBytesToWrite) return -1;

    MWEnterCriticalSection(&(cb->mSection));

    // free space between the write pointer and the end of the buffer
    spaceToEnd = cb->mBufSize - (cb->mWritePtr - cb->mBufStart);

    if(spaceToEnd >= size){
        memcpy(cb->mWritePtr, buf, size);
        cb->mWritePtr += size;
    }else{
        // the write wraps: fill to the end, then continue from the start
        memcpy(cb->mWritePtr, buf, spaceToEnd);
        memcpy(cb->mBufStart, buf + spaceToEnd, size - spaceToEnd);
        cb->mWritePtr = cb->mBufStart + size - spaceToEnd;
    }

    // wrapped exactly to the end of the buffer: reset to the start
    if(cb->mBufSize == cb->mWritePtr - cb->mBufStart){
        cb->mWritePtr = cb->mBufStart;
    }

    cb->mBytesToWrite -= size;
    cb->mBytesToRead += size;

    MWExitCriticalSection(&(cb->mSection));

    return 0;
}

int CircleBufferReadBytes(CircleBuffer* cb, ui8* buf, ui32 size){
    ui32 bytesToEnd;

    if(size > cb->mBytesToRead) return -1;

    MWEnterCriticalSection(&(cb->mSection));

    // data between the read pointer and the end of the buffer
    bytesToEnd = cb->mBufSize - (cb->mReadPtr - cb->mBufStart);

    if(size < bytesToEnd){
        memcpy(buf, cb->mReadPtr, size);
        cb->mReadPtr += size;
    }else{
        // the read wraps: drain to the end, then continue from the start
        memcpy(buf, cb->mReadPtr, bytesToEnd);
        memcpy(buf + bytesToEnd, cb->mBufStart, size - bytesToEnd);
        cb->mReadPtr = cb->mBufStart + size - bytesToEnd;
    }

    // wrapped exactly to the end of the buffer: reset to the start
    if(cb->mBufSize == cb->mReadPtr - cb->mBufStart){
        cb->mReadPtr = cb->mBufStart;
    }

    cb->mBytesToWrite += size;
    cb->mBytesToRead -= size;

    MWExitCriticalSection(&(cb->mSection));
    
    return 0;
}
