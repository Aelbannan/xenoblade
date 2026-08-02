#include "PowerPC_EABI_Support/MetroTRK/dispatch.h"

DSError TRK_DispatchMessage(MessageBuffer* buffer) {
    DSError result = kDispatchError;
    MessageBuffer* msg = buffer;
    ui8 command;

    TRK_SetBufferPosition(buffer, 0);

    command = msg->fData[4];

    switch(command) {
        case kDSConnect:
            result = TRK_DoConnect(msg);
            break;
        case kDSDisconnect:
            result = TRKDoDisconnect(msg);
            break;
        case kDSReset:
            result = TRKDoReset(msg);
            break;
        case kDSOverride:
            result = TRKDoOverride(msg);
            break;
        case kDSReadMemory:
            result = TRKDoReadMemory(msg);
            break;
        case kDSWriteMemory:
            result = TRKDoWriteMemory(msg);
            break;
        case kDSReadRegisters:
            result = TRKDoReadRegisters(msg);
            break;
        case kDSWriteRegisters:
            result = TRKDoWriteRegisters(msg);
            break;
        case kDSContinue:
            result = TRKDoContinue(msg);
            break;
        case kDSStep:
            result = TRKDoStep(msg);
            break;
        case kDSStop:
            result = TRKDoStop(msg);
            break;
        case kDSSetOption:
            result = TRKDoSetOption(msg);
            break;
    }
    return result;
}
