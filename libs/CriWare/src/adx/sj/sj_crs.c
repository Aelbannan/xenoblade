#include "adx/sj/sj_crs.h"
#include <revolution/OS/OSInterrupt.h>

extern volatile int lbl_eu_805F26E0; /* sjcrs_lvl */
extern BOOL lbl_eu_805F26E4;         /* sjcrs_msk */
extern volatile int lbl_eu_805F26E8; /* sj_init_count */

void SJCRS_Init(void){
    lbl_eu_805F26E8++;
    if (lbl_eu_805F26E8 == 1){
        lbl_eu_805F26E0 = 0;
    }
}

void SJCRS_Finish(void){
    lbl_eu_805F26E8--;
    if (lbl_eu_805F26E8 == 0){
        lbl_eu_805F26E0 = 0;
    }
}

void SJCRS_Lock(void){
    if (!lbl_eu_805F26E0){
        lbl_eu_805F26E4 = OSDisableInterrupts();
    }
    lbl_eu_805F26E0++;
}

void SJCRS_Unlock(void){
    lbl_eu_805F26E0--;
    if (!lbl_eu_805F26E0){
        OSRestoreInterrupts(lbl_eu_805F26E4);
    }
}
