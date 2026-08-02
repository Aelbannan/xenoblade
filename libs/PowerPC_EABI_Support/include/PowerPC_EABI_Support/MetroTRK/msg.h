#ifndef TRK_MSG
#define TRK_MSG

#include "PowerPC_EABI_Support/MetroTRK/dstypes.h"
#include "PowerPC_EABI_Support/MetroTRK/trk.h"
#include "PowerPC_EABI_Support/MetroTRK/msgbuf.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TRKPacketSeq {
    ui16 sequence; //packet sequence counter
    ui8 pad[6];     //unused; retail .sbss slot is 8 bytes, only the sequence is accessed
} TRKPacketSeq;

DSError TRK_MessageSend(MessageBuffer*);

#ifdef __cplusplus
}
#endif
#endif
