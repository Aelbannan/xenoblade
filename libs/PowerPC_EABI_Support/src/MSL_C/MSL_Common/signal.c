#include <signal.h>

// Retail .bss slice is 0x20 bytes = 8 slots (raise only indexes 0..6; MSL
// over-allocates one entry).
sig_func signal_funcs[8];

//not present in the retail binary; kept commented out for reference
//void signal(){
//}

int raise(int sig) {
    sig_func temp_r31;
    u32 temp_r0;

    temp_r0 = sig - 1;
    if(temp_r0 > 6) {
        return -1;
    }
    temp_r31 = signal_funcs[temp_r0];
    if((u32)temp_r31 != 1) {
        signal_funcs[temp_r0] = 0;
    }
    if((u32)temp_r31 == 1 || ((int)temp_r31 == 0 && sig == 1)) {
        return 0;
    }
    if((u32)temp_r31 == 0) {
        exit(0);
    }
    temp_r31(sig);
    return 0;
}
