#include "PowerPC_EABI_Support/MSL_C/MSL_Common/misc_io.h"
#include "PowerPC_EABI_Support/MSL_C/MSL_Common/ansi_files.h"

//not present in the retail binary; kept commented out for reference
//void clearerr(){
//}

//void feof(){
//}

//void ferror(){
//}

//void perror(){
//}

void __stdio_atexit(void) {
    __stdio_exit = __close_all;
}
