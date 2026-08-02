#include <signal.h>

void (*__stdio_exit)(void);
BOOL __aborting;

void abort(void) {
    raise(1);
    __aborting = TRUE;
    exit(1);
}

// not present in the retail binary; kept commented out for reference
//void atexit(){
//}

//void __atexit(){
//}

//void __exit(){
//}
