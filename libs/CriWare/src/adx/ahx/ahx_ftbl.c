#include "adx/ahx/ahx_ftbl.h"

extern float lbl_eu_80560058[];
float* lbl_eu_805620D8 = lbl_eu_80560058;
s32 lbl_eu_805620DC = 0x2080;

void AHXTBL_GetFtblInfo(float** array_ptr, s32* size_ptr){
    if(array_ptr != NULL){
        *array_ptr = lbl_eu_805620D8;
    }
    if(size_ptr != NULL){
        *size_ptr = lbl_eu_805620DC;
    }
}
