#include "PowerPC_EABI_Support/MetroTRK/nubinit.h"

#define TRK_ENDIAN_MAGIC 0x12345678        // magic value used to probe byte order
#define TRK_ENDIAN_MAGIC_SWAPPED 0x78563412 // same value in the opposite byte order
#include "PowerPC_EABI_Support/MetroTRK/serpoll.h"
#include "PowerPC_EABI_Support/MetroTRK/targimpl.h"

bool gTRKBigEndian;

DSError TRK_InitializeNub(void) {
    DSError result;
    DSError resultTemp;

    result = TRK_InitializeEndian();

    if(result == kNoError) {
        result = TRKInitializeEventQueue();
    }
    if(result == kNoError) {
        result = TRK_InitializeMessageBuffers();
    }
    InitializeProgramEndTrap();
    if(result == kNoError) {
        result = TRKInitializeSerialHandler();
    }
    if(result == kNoError) {
        result = TRKInitializeTarget();
    }
    if(result == kNoError) {

        resultTemp = TRK_InitializeIntDrivenUART(1, 0, &gTRKInputPendingPtr);
        TRKTargetSetInputPendingPtr(gTRKInputPendingPtr);
        if(resultTemp != kNoError) {
            result = resultTemp;
        }
    }
    return result;
}

DSError TRK_TerminateNub(void) {
    TRKTerminateSerialHandler();
    return kNoError;
}

void TRK_NubWelcome(void) {
    TRK_board_display("MetroTRK for Revolution v0.4");
    return;
}

bool TRK_InitializeEndian(void) {
    ui8 bendian[4];
    bool result = false;
    gTRKBigEndian = true;

    bendian[0] = (TRK_ENDIAN_MAGIC >> 24) & 0xFF;
    bendian[1] = (TRK_ENDIAN_MAGIC >> 16) & 0xFF;
    bendian[2] = (TRK_ENDIAN_MAGIC >> 8) & 0xFF;
    bendian[3] = TRK_ENDIAN_MAGIC & 0xFF;

    if(*(ui32*)bendian == TRK_ENDIAN_MAGIC) {
        gTRKBigEndian = true;
    } else if(*(ui32*)bendian == TRK_ENDIAN_MAGIC_SWAPPED) {
        gTRKBigEndian = false;
    } else {
        result = true;
    }
    return result;
}
