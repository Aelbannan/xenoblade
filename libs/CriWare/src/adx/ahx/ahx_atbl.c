#include "adx/ahx/ahx_atbl.h"

//old name: ahxdcd_atbl


extern ATblEntry lbl_eu_805629A8[];
extern float lbl_eu_805659A8[];

ATblEntry* ahxtbl_atbl_ptr = lbl_eu_805629A8;
s32 ahxtbl_atbl_size = 0x3000;
float* ahxtbl_mtbl_ptr = lbl_eu_805659A8;
s32 ahxtbl_mtbl_size = 0x100;

void AHXTBL_GetAtblInfo(ATblEntry** array_ptr, s32* size_ptr){
    if(array_ptr != NULL){
        *array_ptr = ahxtbl_atbl_ptr;
    }
    if(size_ptr != NULL){
        *size_ptr = ahxtbl_atbl_size;
    }
}

void AHXTBL_GetMtblInfo(float** array_ptr, s32* size_ptr){
    if(array_ptr != NULL){
        *array_ptr = ahxtbl_mtbl_ptr;
    }
    if(size_ptr != NULL){
        *size_ptr = ahxtbl_mtbl_size;
    }
}
