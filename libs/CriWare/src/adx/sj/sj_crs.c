#include "adx/sj/sj_crs.h"
#include <revolution/OS/OSInterrupt.h>

volatile int sjcrs_lvl;
int sjcrs_msk;
volatile int sj_init_count[2];

void SJCRS_Init(void){
    sj_init_count[0]++;
    if (sj_init_count[0] == 1){
        sjcrs_lvl = 0;
    }
}

void SJCRS_Finish(void){
    sj_init_count[0]--;
    if (sj_init_count[0] == 0){
        sjcrs_lvl = 0;
    }
}

void SJCRS_Lock(void){
    if (!sjcrs_lvl){
        sjcrs_msk = OSDisableInterrupts();
    }
    sjcrs_lvl++;
}

void SJCRS_Unlock(void){
    sjcrs_lvl--;
    if (!sjcrs_lvl){
        OSRestoreInterrupts(sjcrs_msk);
    }
}
