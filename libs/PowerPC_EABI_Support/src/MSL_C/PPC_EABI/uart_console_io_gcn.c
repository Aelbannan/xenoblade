#include <types.h>
#include <revolution/OS.h>
#include "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/UART.h"
#include <stdio.h>

/*
 * Console I/O backend for __write_console: when the console type does not
 * carry the emulated flag (bit 0x20000000), output is routed through the
 * GCN UART (InitializeUART / WriteUARTN) instead of the TRK console.
 */

static BOOL initialized;

int __TRK_write_console(__file_handle, u8*, size_t*, __ref_con);

BOOL __write_console(__file_handle handle, u8* buffer, size_t* count, __ref_con ref_con) {

    if((OSGetConsoleType() & 0x20000000) == 0) {
        int initResult = 0;
        if(initialized == FALSE) {
            initResult = InitializeUART(0xE100);
            if(initResult == 0) {
                initialized = TRUE;
            }
        }
        if(initResult != 0) {
            return TRUE;
        }
        if(WriteUARTN(buffer, *count) != 0) {
            *count = 0;
            return TRUE;
        }
    }
    __TRK_write_console(handle, buffer, count, ref_con);
    return FALSE;
}

int __close_console(__file_handle handle) {
    return 0;
}

//not present in the retail binary; kept commented out so the unit
//stays within its split budget
//void __delete_file(){
//}

//not present in the retail binary; kept commented out so the unit
//stays within its split budget
//void __rename_file(){
//}

//not present in the retail binary; kept commented out so the unit
//stays within its split budget
//void __temp_file_name(){
//}
