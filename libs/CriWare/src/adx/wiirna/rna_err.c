// CriWare ADX Wii RNA error handler.
//
// The handler state lives in .bss as two adjacent objects (defined by the
// data TU, linked at 0x805F2AF8):
//   lbl_eu_805F2AF8 - the registered error callback (4 bytes)
//   lbl_eu_805F2AFC - the callback argument word + 256-byte message buffer
//                     (0x104 bytes)
//
// Together they form the RNAErrHandler layout below; RNAERR_CallErrFunc
// views them as one contiguous state block based at lbl_eu_805F2AF8.

#include <harness_catalog.h>
#include <string.h>

// Error callback state as one contiguous block (base = 0x805F2AF8):
//   +0x00 cb   - callback invoked with (arg, msg)
//   +0x04 arg  - first word of the state buffer, passed as the callback argument
//   +0x08 msg  - 256-byte message buffer (last entry of lbl_eu_805F2AFC)
typedef struct RNAErrHandler {
    void (*cb)(u32, char*);
    u32 arg;
    char msg[0x100];
} RNAErrHandler;

// Registered error callback (0x805F2AF8).
// name-guess: no retail data name exists in config/{us,eu,jp}/symbols.txt;
// RNAERR_EntryErrFunc stores the callback at this address.
void (*RNAERR_ErrFunc)(u32, char*);

// Callback argument word + message buffer (0x805F2AFC, 0x104 bytes).
// name-guess: no retail data name exists in config/{us,eu,jp}/symbols.txt;
// this is the work area passed with the error callback.
u32 RNAERR_ErrWork[65];

// Register the error callback and its argument word.
void RNAERR_EntryErrFunc(u32 fn, u32 arg) {
    RNAERR_ErrFunc = (void (*)(u32, char*))fn;
    RNAERR_ErrWork[0] = arg;
}

// Copy msg into the handler's buffer (capped at 0xFF bytes) and dispatch
// the registered callback with (arg, msg). No-op when no callback is set.
void RNAERR_CallErrFunc(const char* msg) {
    RNAErrHandler* handler = (RNAErrHandler*)&RNAERR_ErrFunc;

    strncpy(handler->msg, msg, 0xFF);
    if (handler->cb) {
        handler->cb(handler->arg, handler->msg);
    }
}
