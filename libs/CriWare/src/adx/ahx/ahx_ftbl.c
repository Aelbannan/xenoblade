#include "adx/ahx/ahx_ftbl.h"

// Retail owns the synthesis filter table (lbl_eu_80560058, criware_data.s), its
// pointer (lbl_eu_805620D8) and size (lbl_eu_805620DC); this .text-only split
// references them rather than defining them.
extern float* lbl_eu_805620D8;
extern s32 lbl_eu_805620DC;

void AHXTBL_GetFtblInfo(float** array_ptr, s32* size_ptr){
    if(array_ptr != NULL){
        *array_ptr = lbl_eu_805620D8;
    }
    if(size_ptr != NULL){
        *size_ptr = lbl_eu_805620DC;
    }
}
