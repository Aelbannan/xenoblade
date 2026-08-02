#include "PowerPC_EABI_Support/MetroTRK/custconn/cc_gdev.h"
#include "PowerPC_EABI_Support/MetroTRK/custconn/CircleBuffer.h"
#include "PowerPC_EABI_Support/MetroTRK/trk.h"

static ui8 gRecvBuf[0x500];
static CircleBuffer gRecvCB;
static bool gIsInitialized;

//not present in the retail binary; kept commented out so the unit
//stays within its split budget
//void OutputData(){
//}
//
//not present in the retail binary; kept commented out so the unit
//stays within its split budget
//bool IsInitialized(){
//   return gIsInitialized;
//}

int gdev_cc_initialize(void* flagOut, OSInterruptHandler handler) {
    DBInitComm((ui8**)flagOut, handler);
    CircleBufferInitialize(&gRecvCB, gRecvBuf, sizeof(gRecvBuf));
    return 0;
}

int gdev_cc_shutdown(void) {
    return 0;
}

int gdev_cc_open() {
    if(gIsInitialized) {
        return GDEV_RESULT_10005;
    } else {
        gIsInitialized = true;
        return 0;
    }
}

int gdev_cc_close() {
    return 0;
}

int gdev_cc_read(ui8* dest, int size) {
    int sizeTemp = size;
    ui8* destTemp = dest;
    ui8 buf[0x500];
    int readResult = 0;

    if(!gIsInitialized) {
        return GDEV_RESULT_10001;
    }

    while(CBGetBytesAvailableForRead(&gRecvCB) < size) {
        int available;
        readResult = 0;
        available = DBQueryData();
        if(available != 0) {
            readResult = DBRead(buf, size);
            if(readResult == 0) {
                CircleBufferWriteBytes(&gRecvCB, buf, available);
            }
        }
    }

    if(readResult == 0) {
        CircleBufferReadBytes(&gRecvCB, dest, size);
    }

    return readResult;
}

int gdev_cc_write(const ui8* src, int size) {
    int sizeTemp = size;
    ui8* srcTemp = (ui8*)src;

    if(!gIsInitialized) {
        return GDEV_RESULT_10001;
    }

    while(sizeTemp > 0) {
        int result = DBWrite(srcTemp, sizeTemp);
        if(result != 0) {
            srcTemp += result;
            sizeTemp -= result;
        } else {
            break;
        }
    }

    return 0;
}

int gdev_cc_pre_continue() {
    DBClose();
    return 0;
}

int gdev_cc_post_stop() {
    DBOpen();
    return 0;
}

int gdev_cc_peek() {
    int available = DBQueryData();
    ui8 buf[0x500];

    if(available <= 0) {
        return 0;
    }

    if(!DBRead(buf, available)) {
        CircleBufferWriteBytes(&gRecvCB, buf, available);
    } else {
        return GDEV_RESULT_10009;
    }

    return available;
}

int gdev_cc_initinterrupts() {
    DBInitInterrupts();
    return 0;
}
