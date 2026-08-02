#include "PowerPC_EABI_Support/MetroTRK/notify.h"
#include "PowerPC_EABI_Support/MetroTRK/msgbuf.h"
#include "PowerPC_EABI_Support/MetroTRK/targimpl.h"
#include "PowerPC_EABI_Support/MetroTRK/support.h"

DSError TRKDoNotifyStopped(MessageCommandID cmdId) {
    int replyBufferId;
    int bufferId;
    MessageBuffer* buffer;
    DSError result;

    result = TRK_GetFreeBuffer(&bufferId, &buffer);

    if(result == kNoError) {
        if(cmdId == kDSNotifyStopped) {
            TRKTargetAddStopInfo(buffer);
        } else {
            TRKTargetAddExceptionInfo(buffer);
        }

        result = TRK_RequestSend(buffer, &replyBufferId);

        if(result == kNoError) {
            TRK_ReleaseBuffer(replyBufferId);
        }

        TRK_ReleaseBuffer(bufferId);
    }

    return result;
}
